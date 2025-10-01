const std = @import("std");
const tbl = @import("./table.zig");
const rw = @import("./row.zig");
const utils = @import("./utils.zig");
const nd = @import("./node.zig");

const Table = tbl.Table;
const Row = rw.Row;
const InternalError = utils.InternalError;

pub const Executor = struct {
    table: *Table,

    pub fn init(table: *Table) Executor {
        return Executor{
            .table = table,
        };
    }

    pub fn evaluate(self: *Executor, statement: tbl.Statement) !void {
        switch (statement) {
            .insert => |row| {
                const root_page = try self.table.pager.getPage(self.table.root_page_num);
                var root_node = nd.NodeView{ .node = root_page };
                var leaf_view = root_node.asLeaf();
                const num_cells = try leaf_view.getNumCells();
                if (num_cells >= nd.LEAF_NODE_MAX_CELLS) {
                    std.debug.print("Table full: {d}\n", .{num_cells});
                    return InternalError.table_full;
                }
                const key_to_insert = row.id;
                var cursor = try self.table.find(key_to_insert);
                if (cursor.cell_num < num_cells) {
                    const cell = leaf_view.getCell(cursor.cell_num);
                    const key_at_index = try cell.getKey();
                    if (key_at_index == key_to_insert) {
                        return InternalError.duplicate_key;
                    }
                }
                try cursor.leafNodeInsert(key_to_insert, &row);
            },
            .select => {
                var row = Row.init(0, "", "");
                var cursor = try self.table.start();
                while (!cursor.reached_end()) {
                    const cell_view = try cursor.value();
                    try utils.deserializeFrom(Row, cell_view.value, &row);
                    row.dump();
                    try cursor.advance();
                }
            },
        }
    }
};
