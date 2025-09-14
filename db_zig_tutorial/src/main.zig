const std = @import("std");
const root = @import("./common.zig");

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
    var allocator = gpa.allocator();

    var input_buffer = try root.InputBuffer.init(&allocator, 1024);
    defer input_buffer.deinit();

    var running: bool = true;
    const stdin = std.fs.File.stdin();
    const exit = ".exit";
    while (running) {
        std.debug.print(">", .{});
        input_buffer.consume(stdin) catch |err| {
            if (err != error.EndOfStream) {
                std.debug.print("Error reading file: {}\n", .{err});
            }
            std.debug.print("bye...\n", .{});
            running = false;
            return;
        };
        std.debug.print("Compare {} {}\n", .{ exit.len, input_buffer.input_view.len });
        if (std.mem.eql(u8, input_buffer.input_view, exit)) {
            std.debug.print("bye...\n", .{});
            return;
        }
    }
}
