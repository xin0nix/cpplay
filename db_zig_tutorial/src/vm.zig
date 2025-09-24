const std = @import("std");
const common = @import("./common.zig");

const Table = common.Table;
const Row = common.Row;

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
                const slot = try self.table.new_row_slot();
                try common.serializeTo(Row, &x, slot);
            },
            .select => {
                var row = Row.init(0, "", "");
                for (0..self.table.index.num_rows) |i| {
                    const slice = try self.table.row_slot(i);
                    try common.deserializeFrom(Row, slice, &row);
                    row.dump();
                }
            },
        }
    }
};
