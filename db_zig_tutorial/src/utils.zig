const std = @import("std");
const rows = @import("./row.zig");

pub const USERNAME_SIZE = 32;
pub const EMAIL_SIZE = 256;
pub const PAGE_SIZE = std.heap.pageSize();
pub const TABLE_MAX_PAGES = 10;

pub const InternalError = error{
    not_implemented,
    out_of_range,
    table_full,
    duplicate_key,
};

pub fn cStringLen(slice: []const u8) usize {
    for (0..slice.len) |i| {
        if (slice[i] == 0) {
            return i;
        }
    }
    return slice.len;
}

pub fn serializeTo(comptime T: type, obj: *const T, slice: []u8) !void {
    const num_bytes = @sizeOf(T);
    const raw: []const u8 = @ptrCast(obj);
    if (slice.len != num_bytes) {
        return error.InvalidDataLength;
    }
    @memcpy(slice, raw[0..num_bytes]);
}

pub fn serialize(comptime T: type, obj: *const T, allocator: *std.mem.Allocator) ![]u8 {
    const num_bytes = @sizeOf(T);
    const buffer = try allocator.alloc(u8, num_bytes);
    try serializeTo(T, obj, buffer);
    return buffer;
}

pub fn deserializeFrom(comptime T: type, slice: []const u8, obj: *T) !void {
    const num_bytes = @sizeOf(T);
    if (slice.len != num_bytes) {
        return error.InvalidDataLength;
    }
    const raw: []u8 = @ptrCast(obj);
    @memcpy(raw[0..num_bytes], slice);
}

pub fn deserialize(comptime T: type, slice: []const u8, allocator: *std.mem.Allocator) !*T {
    const ptr = try allocator.create(T);
    try deserializeFrom(T, slice, ptr);
    return ptr;
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

test "serialize and Row" {
    var allocator = std.testing.allocator;
    var insert = rows.Row.init(120, "joe", "my@mail.com");
    const serialized = try serialize(rows.Row, &insert, &allocator);
    defer allocator.free(serialized);
    {
        const expected = "x";
        const view = serialized[0..1];
        try std.testing.expect(std.mem.eql(u8, view, expected));
    }
    {
        const expected = "joe";
        const view = serialized[4 .. 4 + expected.len];
        try std.testing.expect(std.mem.eql(u8, view, expected));
    }
    {
        const expected = "my@mail.com";
        const view = serialized[4 + 32 .. 4 + 32 + expected.len];
        try std.testing.expect(std.mem.eql(u8, view, expected));
    }
    const deserialized = try deserialize(rows.Row, serialized, &allocator);
    defer allocator.destroy(deserialized);
    const same = deserialized.equals(insert);
    try std.testing.expect(same);
}
