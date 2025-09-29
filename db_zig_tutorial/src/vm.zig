const std = @import("std");
const common = @import("./common.zig");
const rows = @import("./row.zig");
const utils = @import("./utils.zig");

const Table = common.Table;
const Row = rows.Row;

pub const Executor = struct {
    table: *Table,

    pub fn init(table: *Table) Executor {
        return Executor{
            .table = table,
        };
    }

    pub fn evaluate(self: *Executor, statement: common.Statement) !void {
        switch (statement) {
            .insert => |x| {
                var cursor = try self.table.end();
                try cursor.leaf_node_insert(x.id, &x);
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
