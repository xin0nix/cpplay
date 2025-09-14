//! By convention, root.zig is the root source file when making a library.
const std = @import("std");

const InputError = error{
    invalid_token,
};

pub const Statement = enum {
    select,
    insert,
};

pub const Command = union(enum) {
    exit: void,
    statement: Statement,

    pub fn equals(self: Command, other: Command) bool {
        switch (self) {
            .exit => {
                switch (other) {
                    .exit => return true,
                    .statement => return false,
                }
            },
            .statement => {
                switch (other) {
                    .exit => return false,
                    .statement => return self.statement == other.statement,
                }
            },
        }
    }
};

pub const Parser = struct {
    pub fn identify(_: *Parser, input_buffer: *InputBuffer) !Command {
        const view = input_buffer.input_view;
        if (view.len == 0) {
            // FIXME: dedicated error
            return InputError.invalid_token;
        }
        if (view.len > 0 and view[0] == '.') {
            if (std.mem.eql(u8, view, ".exit")) {
                return Command.exit;
            }
            return InputError.invalid_token;
        }
        const insert = "insert";
        if (std.mem.eql(u8, view[0..insert.len], insert)) {
            return Command{ .statement = Statement.insert };
        }
        const select = "select";
        if (std.mem.eql(u8, view[0..select.len], select)) {
            return Command{ .statement = Statement.select };
        }
        return InputError.invalid_token;
    }
};

pub const InputBuffer = struct {
    allocator: *std.mem.Allocator,
    buffer: []u8,
    input_view: []const u8,

    pub fn init(allocator: *std.mem.Allocator, size: usize) !InputBuffer {
        return InputBuffer{
            .allocator = allocator,
            .buffer = try allocator.alloc(u8, size),
            .input_view = "",
        };
    }

    pub fn deinit(self: *InputBuffer) void {
        if (self.buffer.len != 0) {
            self.allocator.free(self.buffer);
            self.buffer = &[_]u8{};
        }
    }

    pub fn consume(self: *InputBuffer, file: std.fs.File) !void {
        var temp_buffer: [1024]u8 = undefined;
        var reader = file.reader(&temp_buffer);
        var bytes_read: usize = 0;
        while (bytes_read == 0) {
            bytes_read = try reader.read(self.buffer);
        }
        self.input_view = self.buffer[0 .. bytes_read - 1];
    }
};

test "InputBuffer allocates and frees correctly" {
    var debug_allocator = std.testing.allocator;
    var input_buffer = try InputBuffer.init(&debug_allocator, 1024);
    defer input_buffer.deinit();
}

test "Parser identify test" {
    var parser = Parser{};
    var debug_allocator = std.testing.allocator;
    var input_buffer = try InputBuffer.init(&debug_allocator, 1024);
    defer input_buffer.deinit();

    input_buffer.input_view = ".exit";
    try std.testing.expect((try parser.identify(&input_buffer)).equals(Command{ .exit = {} }));

    input_buffer.input_view = "insert into table";
    try std.testing.expect((try parser.identify(&input_buffer)).equals(Command{ .statement = Statement.insert }));
}
