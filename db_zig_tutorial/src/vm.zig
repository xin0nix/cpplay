const std = @import("std");
const common = @import("./common.zig");
const rows = @import("./row.zig");

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
                var cursor = self.table.end();
                const slot = try cursor.emplace();
                try common.serializeTo(Row, &x, slot);
            },
            .select => {
                var row = Row.init(0, "", "");
                var cursor = self.table.start();
                while (!cursor.reached_end()) {
                    const slice = try cursor.value();
                    try common.deserializeFrom(Row, slice, &row);
                    row.dump();
                    cursor.advance();
                }
            },
        }
    }
};
