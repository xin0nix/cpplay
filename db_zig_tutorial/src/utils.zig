const std = @import("std");

pub const USERNAME_SIZE = 32;
pub const EMAIL_SIZE = 256;
pub const PAGE_SIZE = std.heap.pageSize();
pub const TABLE_MAX_PAGES = 10;

pub const SerializationError = error{
    out_of_range,
};

pub fn cStringLen(slice: []const u8) usize {
    for (0..slice.len) |i| {
        if (slice[i] == 0) {
            return i;
        }
    }
    return slice.len;
}

test "c string no UB" {
    const buffer = [_]u8{ 'a', 'b', 'c', 'd' };
    const len = cStringLen(&buffer);
    std.debug.assert(len == 4);
}

test "c string with null terminator" {
    const buffer = [_]u8{ 'a', 'b', 0, 'c' };
    const len = cStringLen(&buffer);
    std.debug.assert(len == 2);
}
