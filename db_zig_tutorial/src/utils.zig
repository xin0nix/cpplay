const std = @import("std");

pub fn cStringLen(slice: []const u8) usize {
    var i: usize = 0;
    while (i < slice.len) : (i += 1) {
        if (slice[i] == 0) return i;
    }
    return slice.len; // No null terminator found in slice range
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
