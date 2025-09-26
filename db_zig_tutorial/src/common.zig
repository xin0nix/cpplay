const std = @import("std");
const utils = @import("./utils.zig");
const pgr = @import("./pager.zig");
const idx = @import("./index.zig");
const rows = @import("./row.zig");

pub const Select = struct {};

const EMAIL_SIZE = utils.EMAIL_SIZE;
const USERNAME_SIZE = utils.USERNAME_SIZE;
const PAGE_SIZE = utils.PAGE_SIZE;
const TABLE_MAX_PAGES = utils.TABLE_MAX_PAGES;
const ROWS_PER_PAGE = PAGE_SIZE / @sizeOf(Row);
const TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

const SerializationError = utils.SerializationError;
const Index = idx.Index;
const Pager = pgr.Pager(TABLE_MAX_PAGES);
const Row = rows.Row;

const cStringLen = utils.cStringLen;

pub const Cursor = struct {
    table: *Table,
    position: usize,

    pub fn reached_end(self: *Cursor) bool {
        if (self.position == self.table.index.num_rows) {
            return true;
        }
        return false;
    }

    pub fn advance(self: *Cursor) void {
        self.position += 1;
    }

    pub fn emplace(self: *Cursor) ![]u8 {
        self.table.index.num_rows += 1;
        return self.value();
    }

    pub fn value(self: *Cursor) ![]u8 {
        const row_num = self.position;
        const page_num = row_num / ROWS_PER_PAGE;
        const row_offset = row_num % ROWS_PER_PAGE;
        const byte_offset = row_offset * @sizeOf(Row);
        const page = try self.table.pager.get_page(page_num);
        return page[byte_offset .. byte_offset + @sizeOf(Row)];
    }
};

pub const Table = struct {
    pager: pgr.Pager(TABLE_MAX_PAGES),
    index: Index,
    allocator: *std.mem.Allocator,

    pub fn start(self: *Table) Cursor {
        return Cursor{
            .table = self,
            .position = 0,
        };
    }

    pub fn end(self: *Table) Cursor {
        return Cursor{
            .table = self,
            .position = self.index.num_rows,
        };
    }

    pub fn open(db_path: []const u8, index_path: []const u8, allocator: *std.mem.Allocator) !Table {
        const pager = try Pager.open(db_path, allocator);
        const index = try Index.open(index_path);
        const table = Table{
            .pager = pager,
            .index = index,
            .allocator = allocator,
        };
        return table;
    }

    pub fn close(self: *Table) !void {
        try self.pager.close();
        try self.index.close();
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
