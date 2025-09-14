//! By convention, root.zig is the root source file when making a library.
const std = @import("std");

pub const InputBuffer = struct {
    allocator: std.mem.Allocator,
    buffer: []u8,
    input_length: isize,

    pub fn init(allocator: std.mem.Allocator, size: usize) !InputBuffer {
        return InputBuffer{
            .allocator = allocator,
            .buffer = try allocator.alloc(u8, size),
            .input_length = 0,
        };
    }

    pub fn deinit(self: *InputBuffer) void {
        if (self.buffer.len != 0) {
            self.allocator.free(self.buffer);
            self.buffer = &[_]u8{};
        }
    }
};

test "InputBuffer allocates and frees correctly" {
    const debug_allocator = std.testing.allocator;
    var input_buffer = try InputBuffer.init(debug_allocator, 1024);
    defer input_buffer.deinit();
}
