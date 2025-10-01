const std = @import("std");
const utils = @import("./utils.zig");
const pgr = @import("./pager.zig");
const rw = @import("./row.zig");
const nd = @import("./node.zig");

pub const Select = struct {};

const EMAIL_SIZE = utils.EMAIL_SIZE;
const USERNAME_SIZE = utils.USERNAME_SIZE;
const PAGE_SIZE = utils.PAGE_SIZE;
const TABLE_MAX_PAGES = utils.TABLE_MAX_PAGES;
const ROWS_PER_PAGE = PAGE_SIZE / @sizeOf(Row);
const TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

const InternalError = utils.InternalError;
const Pager = pgr.Pager(TABLE_MAX_PAGES);
const Row = rw.Row;

const cStringLen = utils.cStringLen;

pub const Cursor = struct {
    table: *Table,
    page_num: u32,
    cell_num: u32,
    end_of_table: bool,

    pub fn reached_end(self: *Cursor) bool {
        return self.end_of_table;
    }

    pub fn advance(self: *Cursor) !void {
        const page = try self.table.pager.getPage(self.page_num);
        var node = nd.NodeView{ .node = page };
        var leaf_view = node.asLeaf();
        self.cell_num += 1;
        if (self.cell_num >= try leaf_view.getNumCells()) {
            self.end_of_table = true;
        }
    }

    pub fn leafNodeInsert(self: *Cursor, key: u32, row: *const Row) !void {
        const page = try self.table.pager.getPage(self.page_num);
        var node = nd.NodeView{ .node = page };
        var leaf_view = node.asLeaf();
        const num_cells = try leaf_view.getNumCells();
        if (num_cells >= nd.LEAF_NODE_MAX_CELLS) {
            std.debug.print("Need to implement splitting a leaf node: {d}\n", .{num_cells});
            return InternalError.out_of_range;
        }
        if (self.cell_num < num_cells) {
            // Make room for new cell
            var i: u32 = num_cells;
            while (i > self.cell_num) : (i -= 1) {
                @memcpy(leaf_view.getCell(i).key, leaf_view.getCell(i - 1).key);
                @memcpy(leaf_view.getCell(i).value, leaf_view.getCell(i - 1).value);
            }
        }
        const cell = leaf_view.getCell(self.cell_num);
        try utils.serializeTo(u32, &key, cell.key);
        try utils.serializeTo(Row, row, cell.value);
        try leaf_view.setNumCells(num_cells + 1);
    }

    pub fn value(self: *Cursor) !nd.LeafNodeCellView {
        const page = try self.table.pager.getPage(self.page_num);
        var node = nd.NodeView{ .node = page };
        var leaf_view = node.asLeaf();
        return leaf_view.getCell(self.cell_num);
    }
};

pub const Table = struct {
    pager: pgr.Pager(TABLE_MAX_PAGES),
    root_page_num: u32,
    allocator: *std.mem.Allocator,

    pub fn start(self: *Table) !Cursor {
        const root_page = try self.pager.getPage(self.root_page_num);
        var root_node_view = nd.NodeView{ .node = root_page };
        var leaf_view = root_node_view.asLeaf();
        const num_cells = try leaf_view.getNumCells();
        return Cursor{
            .table = self,
            .page_num = self.root_page_num,
            .cell_num = 0,
            .end_of_table = (num_cells == 0),
        };
    }

    // Return the position of the given key.
    // If the key is not present, return the position where it should be inserted
    pub fn find(self: *Table, key: u32) !Cursor {
        const root_page = try self.pager.getPage(self.root_page_num);
        var root_node = nd.NodeView{ .node = root_page };
        // TODO: get node type
        if (try root_node.getType() == .leaf) {
            return self.leafNodeFind(self.root_page_num, key);
        }
        std.debug.print("Need to implement searching an internal node\n", .{});
        return InternalError.not_implemented;
    }

    // This will either return
    // - the position of the key,
    // - the position of another key that we’ll need to move if we want to insert the new key, or
    // - the position one past the last key
    fn leafNodeFind(self: *Table, page_num: u32, key: u32) !Cursor {
        const page = try self.pager.getPage(page_num);
        var node = nd.NodeView{ .node = page };
        var leaf_view = node.asLeaf();
        const num_cells = try leaf_view.getNumCells();
        var cursor = Cursor{
            .table = self,
            .page_num = page_num,
            .cell_num = undefined,
            .end_of_table = (num_cells == 0),
        };
        var min_index: u32 = 0;
        var one_past_max_index = num_cells;
        while (min_index != one_past_max_index) {
            const index = (min_index + one_past_max_index) / 2;
            var cell = leaf_view.getCell(index);
            const key_at_index = try cell.getKey();
            if (key == key_at_index) {
                cursor.cell_num = index;
                return cursor;
            } else if (key < key_at_index) {
                one_past_max_index = index;
            } else {
                min_index = index + 1;
            }
        }
        cursor.cell_num = min_index;
        return cursor;
    }

    pub fn open(db_path: []const u8, allocator: *std.mem.Allocator) !Table {
        var pager = try Pager.open(db_path, allocator);
        if (pager.num_pages == 0) {
            // New database file. Initialize page 0 as leaf node.
            const root_node = try pager.getPage(0);
            _ = try nd.NodeView.initLeaf(root_node);
        }
        const table = Table{
            .pager = pager,
            .root_page_num = 0,
            .allocator = allocator,
        };
        return table;
    }

    pub fn close(self: *Table) !void {
        try self.pager.close();
    }
};

pub const Statement = union(enum) {
    select: Select,
    insert: Row,

    pub fn equals(self: Statement, other: Statement) bool {
        switch (self) {
            .select => switch (other) {
                .select => return true,
                .insert => return false,
            },
            .insert => switch (other) {
                .select => return false,
                .insert => return self.insert.equals(other.insert),
            },
        }
    }
};

pub const Command = union(enum) {
    exit,
    constants,
    btree,
    statement: Statement,

    pub fn equals(self: Command, other: Command) bool {
        switch (self) {
            .exit => {
                switch (other) {
                    .exit => return true, // ok
                    .constants => return false,
                    .btree => return false,
                    .statement => return false,
                }
            },
            .constants => {
                switch (other) {
                    .exit => return false,
                    .constants => return true, // ok
                    .btree => return false,
                    .statement => return false,
                }
            },
            .btree => {
                switch (other) {
                    .exit => return false,
                    .constants => return false,
                    .btree => return true, // ok
                    .statement => return false,
                }
            },
            .statement => {
                switch (other) {
                    .exit => return false,
                    .constants => return false,
                    .btree => return false,
                    .statement => return self.statement.equals(other.statement),
                }
            },
        }
    }
};

test "comptime constants check" {
    try std.testing.expect(PAGE_SIZE == 4096);
    try std.testing.expect(ROWS_PER_PAGE == 14);
    try std.testing.expect(TABLE_MAX_ROWS == 140);
}
