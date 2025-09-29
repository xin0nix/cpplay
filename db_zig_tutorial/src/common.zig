const std = @import("std");
const utils = @import("./utils.zig");
const pgr = @import("./pager.zig");
const rows = @import("./row.zig");
const nodes = @import("./node.zig");

pub const Select = struct {};

const EMAIL_SIZE = utils.EMAIL_SIZE;
const USERNAME_SIZE = utils.USERNAME_SIZE;
const PAGE_SIZE = utils.PAGE_SIZE;
const TABLE_MAX_PAGES = utils.TABLE_MAX_PAGES;
const ROWS_PER_PAGE = PAGE_SIZE / @sizeOf(Row);
const TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

const InternalError = utils.InternalError;
const Pager = pgr.Pager(TABLE_MAX_PAGES);
const Row = rows.Row;

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
        const page = try self.table.pager.get_page(self.page_num);
        var node = nodes.NodeView{ .node = page };
        self.cell_num += 1;
        if (self.cell_num >= try node.leafNodeNumCells()) {
            self.end_of_table = true;
        }
    }

    pub fn leaf_node_insert(self: *Cursor, key: u32, row: *const Row) !void {
        const page = try self.table.pager.get_page(self.page_num);
        var node = nodes.NodeView{ .node = page };
        const num_cells = try node.leafNodeNumCells();
        if (num_cells >= nodes.LEAF_NODE_MAX_CELLS) {
            std.debug.print("Need to implement splitting a leaf node: {d}\n", .{num_cells});
            return InternalError.out_of_range;
        }
        if (self.cell_num < num_cells) {
            // Make room for new cell
            var i: u32 = num_cells;
            while (i > self.cell_num) : (i -= 1) {
                @memcpy(node.leafNodeCell(i).value, node.leafNodeCell(i - 1).value);
            }
        }
        const cell = node.leafNodeCell(self.cell_num);
        try utils.serializeTo(u32, &key, cell.key);
        try utils.serializeTo(Row, row, cell.value);
        try node.setLeafNodeNumCells(num_cells + 1); // ??
    }

    pub fn value(self: *Cursor) !nodes.LeafNodeCellView {
        const page = try self.table.pager.get_page(self.page_num);
        var node = nodes.NodeView{ .node = page };
        return node.leafNodeCell(self.cell_num);
    }
};

pub const Table = struct {
    pager: pgr.Pager(TABLE_MAX_PAGES),
    root_page_num: u32,
    allocator: *std.mem.Allocator,

    pub fn start(self: *Table) !Cursor {
        const root_node = try self.pager.get_page(self.root_page_num);
        var root_node_view = nodes.NodeView{ .node = root_node };
        const num_cells = try root_node_view.leafNodeNumCells();
        return Cursor{
            .table = self,
            .page_num = self.root_page_num,
            .cell_num = 0,
            .end_of_table = (num_cells == 0),
        };
    }

    pub fn end(self: *Table) !Cursor {
        const root_node = try self.pager.get_page(self.root_page_num);
        var root_node_view = nodes.NodeView{ .node = root_node };
        const num_cells = try root_node_view.leafNodeNumCells();
        const cursor = Cursor{
            .table = self,
            .page_num = self.root_page_num,
            .cell_num = num_cells,
            .end_of_table = true,
        };
        return cursor;
    }

    pub fn open(db_path: []const u8, allocator: *std.mem.Allocator) !Table {
        var pager = try Pager.open(db_path, allocator);
        if (pager.num_pages == 0) {
            // New database file. Initialize page 0 as leaf node.
            const root_node = try pager.get_page(0);
            _ = try nodes.NodeView.init(root_node);
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
