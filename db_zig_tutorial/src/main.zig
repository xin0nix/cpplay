const std = @import("std");
const common = @import("./common.zig");
const vm = @import("./vm.zig");

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

    var input_buffer = try common.InputBuffer.init(&allocator, 1024);
    defer input_buffer.deinit();

    var running: bool = true;
    const stdin = std.fs.File.stdin();
    var parser = common.Parser{};
    var executor = vm.Executor{};
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

        const command = parser.identify(&input_buffer) catch |err| {
            std.debug.print("Failed to parse {s}: ", .{input_buffer.input_view});
            switch (err) {
                error.invalid_token => {
                    std.debug.print("starts from invalid command\n", .{});
                },
            }
            continue;
        };

        switch (command) {
            .exit => {
                std.debug.print("bye...\n", .{});
                return;
            },
            .statement => |s| {
                executor.evaluate(s);
            },
        }
    }
}
