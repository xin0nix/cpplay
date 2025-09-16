const std = @import("std");
const p = @import("./parser.zig");
const common = @import("./common.zig");
const vm = @import("./vm.zig");

const Parser = p.Parser;
const Table = common.Table;
const Executor = vm.Executor;

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

    const running: bool = true;
    const stdin = std.fs.File.stdin();
    var parser = Parser{};
    var table = Table.init(&allocator);
    defer table.deinit();
    var executor = Executor.init(&table);

    var buffer: [1024]u8 = undefined;
    // FIXME: write a proper parser
    var line: [4096 * 4096]u8 = undefined;
    var reader = std.fs.File.Reader.init(stdin, &buffer);

    while (running) {
        const count = reader.read(&line) catch |err| {
            switch (err) {
                else => {
                    return;
                },
            }
        };
        var tokenizer = std.mem.tokenizeAny(u8, line[0..count], " \n\t");
        while (tokenizer.rest().len > 0) {
            const command = parser.parse(&tokenizer) catch |err| {
                std.debug.print("Failed to parse: ", .{});
                switch (err) {
                    else => {
                        std.debug.print("invalid token\n", .{});
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
                    executor.evaluate(s) catch |err| {
                        switch (err) {
                            error.out_of_range => std.debug.print("Out of range!\n", .{}),
                            else => std.debug.print("Failed to evaluate statement\n", .{}),
                        }
                    };
                },
            }
        }
    }
}
