const std = @import("std");
const db_zig_tutorial = @import("db_zig_tutorial");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const err = gpa.deinit();
        switch (err) {
            .ok => {},
            .leak => {
                std.debug.print("Leak detected", .{});
            },
        }
    }
    const allocator = gpa.allocator();

    var inputBuffer = try db_zig_tutorial.InputBuffer.init(allocator, 1024);
    defer inputBuffer.deinit();
}
