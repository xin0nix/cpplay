const std = @import("std");
const common = @import("./common.zig");

pub const Executor = struct {
    pub fn evaluate(self: *Executor, statement: common.Statement) void {
        _ = self;
        switch (statement) {
            .insert => {
                std.debug.print("INSERT ...\n", .{});
            },
            .select => {
                std.debug.print("SELECT ...\n", .{});
            },
        }
    }
};
