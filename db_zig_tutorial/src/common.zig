const std = @import("std");

pub const Select = struct {};

const USERNAME_SIZE = 32;
const EMAIL_SIZE = 256;
const PAGE_SIZE = std.heap.pageSize();
const TABLE_MAX_PAGES = 10;
const ROWS_PER_PAGE = PAGE_SIZE / @sizeOf(Row);
const TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

const SerializationError = error{
    out_of_range,
};

pub fn cStringLen(slice: []const u8) usize {
    var i: usize = 0;
    while (i < slice.len) : (i += 1) {
        if (slice[i] == 0) return i;
    }
    return slice.len; // No null terminator found in slice range
}

pub const Table = struct {
    num_rows: usize,
    pages: [TABLE_MAX_PAGES]?[]u8,
    allocator: *std.mem.Allocator,

    pub fn new_row_slot(self: *Table) ![]u8 {
        defer self.num_rows += 1;
        return self.row_slot(self.num_rows);
    }

    pub fn row_slot(self: *Table, row_num: usize) ![]u8 {
        const page_num = row_num / ROWS_PER_PAGE;
        if (page_num >= TABLE_MAX_PAGES) {
            return SerializationError.out_of_range;
        }
        const row_offset = row_num % ROWS_PER_PAGE;
        const byte_offset = row_offset * @sizeOf(Row);
        const pageLookup = self.pages[page_num];
        if (pageLookup) |page| {
            return page[byte_offset .. byte_offset + @sizeOf(Row)];
        } else {
            const page = try self.allocator.alloc(u8, PAGE_SIZE);
            self.pages[page_num] = page;
            return page[byte_offset .. byte_offset + @sizeOf(Row)];
        }
    }

    pub fn init(allocator: *std.mem.Allocator) Table {
        var table = Table{
            .num_rows = 0,
            .pages = undefined,
            .allocator = allocator,
        };
        for (&table.pages) |*page_slot| {
            page_slot.* = null;
        }
        return table;
    }

    pub fn deinit(self: *Table) void {
        for (self.pages) |page_opt| {
            if (page_opt) |page| {
                self.allocator.free(page);
            }
        }
    }
};

pub const Row = struct {
    id: u32,
    username: [USERNAME_SIZE]u8,
    email: [EMAIL_SIZE]u8,

    pub fn init(id: u32, name: []const u8, email: []const u8) Row {
        var insert = Row{
            .id = 0,
            .username = undefined,
            .email = undefined,
        };
        @memset(&insert.username, 0);
        @memset(&insert.email, 0);
        insert.id = id;
        @memcpy(insert.username[0..name.len], name);
        @memcpy(insert.email[0..email.len], email);
        return insert;
    }

    pub fn equals(self: Row, other: Row) bool {
        return self.id == other.id and
            std.mem.eql(u8, &self.username, &other.username) and
            std.mem.eql(u8, &self.email, &other.email);
    }

    pub fn dump(self: *const Row) void {
        const username_len = cStringLen(&self.username);
        const email_len = cStringLen(&self.email);
        std.debug.print("({d}, {s}, {s})\n", .{ self.id, self.username[0..username_len], self.email[0..email_len] });
    }
};

pub const Statement = union(enum) {
    select: Select,
    insert: Row,

    pub fn equals(self: Statement, other: Statement) bool {
        switch (self) {
            .select => switch (other) {
                .select => return true,
                .insert => return false,
            },
            .insert => switch (other) {
                .select => return false,
                .insert => return self.insert.equals(other.insert),
            },
        }
    }
};

pub const Command = union(enum) {
    exit,
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
                    .statement => return self.statement.equals(other.statement),
                }
            },
        }
    }
};

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

test "serialize Row" {
    var allocator = std.testing.allocator;
    var insert = Row.init(120, "joe", "my@mail.com");
    const serialized = try serialize(Row, &insert, &allocator);
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
    const deserialized = try deserialize(Row, serialized, &allocator);
    defer allocator.destroy(deserialized);
    const same = deserialized.equals(insert);
    try std.testing.expect(same);
}

test "comptime constants check" {
    try std.testing.expect(PAGE_SIZE == 4096);
    try std.testing.expect(ROWS_PER_PAGE == 14);
    try std.testing.expect(TABLE_MAX_ROWS == 140);
}

test "fetch Row slot" {
    var allocator = std.testing.allocator;
    var table = Table.init(&allocator);
    defer table.deinit();
    const row_num = 17;
    const sliceA0 = try table.row_slot(row_num);
    const rowA0 = Row.init(120, "joe", "my@mail.com");
    try serializeTo(Row, &rowA0, sliceA0);
    const sliceA1 = try table.row_slot(row_num);
    var rowA1 = Row.init(0, "", "");
    try deserializeFrom(Row, sliceA1, &rowA1);
    try std.testing.expect(rowA0.equals(rowA1));
}
