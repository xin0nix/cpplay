//! By convention, root.zig is the root source file when making a library.
const std = @import("std");

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
