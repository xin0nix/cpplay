const std = @import("std");
const utils = @import("./utils.zig");

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

const cStringLen = utils.cStringLen;

pub const Index = struct {
    file: std.fs.File,
    num_rows: usize,

    pub fn open(sub_path: []const u8) !Index {
        const cwd = std.fs.cwd();
        const file = try cwd.openFile(sub_path, .{
            .mode = .read_write,
        });
        const file_length = (try file.stat()).size;
        if (file_length == 0) {
            return Index{
                .file = file,
                .num_rows = 0,
            };
        } else {
            var bytes: [@sizeOf(usize)]u8 = undefined;
            const read = try file.readAll(&bytes);
            if (read != 8) {
                return SerializationError.out_of_range;
            }
            const num_rows = std.mem.readInt(usize, &bytes, .little);
            return Index{
                .file = file,
                .num_rows = num_rows,
            };
        }
    }

    pub fn flush(self: *Index) !void {
        var bytes: [@sizeOf(usize)]u8 = undefined;
        std.mem.writeInt(usize, &bytes, self.num_rows, .little);
        try self.file.writeAll(&bytes);
    }

    pub fn close(self: *Index) !void {
        defer self.file.close();
        try self.flush();
    }
};

pub const Pager = struct {
    file: std.fs.File,
    file_length: u64,
    pages: [TABLE_MAX_PAGES]?[]u8,
    allocator: *std.mem.Allocator,

    pub fn open(sub_path: []const u8, allocator: *std.mem.Allocator) !Pager {
        const cwd = std.fs.cwd();
        const file = try cwd.openFile(sub_path, .{
            .mode = .read_write,
        });
        const file_length = (try file.stat()).size;
        if (file_length % PAGE_SIZE > 0) {
            std.debug.print("File size does not fit into page size {d}\n", .{file_length});
            return SerializationError.out_of_range;
        }
        var pager = Pager{
            .file = file,
            .file_length = file_length,
            .pages = undefined,
            .allocator = allocator,
        };
        for (&pager.pages) |*page_slot| {
            page_slot.* = null;
        }
        return pager;
    }

    pub fn get_page(self: *Pager, page_num: usize) ![]u8 {
        if (page_num >= TABLE_MAX_PAGES) {
            return SerializationError.out_of_range;
        }
        const pageLookup = self.pages[page_num];
        if (pageLookup) |page| {
            return page;
        } else {
            const page = try self.allocator.alloc(u8, PAGE_SIZE);
            const num_pages = self.file_length / PAGE_SIZE;
            if (page_num < num_pages) {
                const seek = page_num * PAGE_SIZE;
                try self.file.seekTo(seek);
                const read = try self.file.read(page);
                if (read != PAGE_SIZE) {
                    std.debug.print("Read less than a page size, {d}\n", .{read});
                    return SerializationError.out_of_range;
                }
            }
            self.pages[page_num] = page;
            return page;
        }
    }

    pub fn flush_all(self: *Pager) !void {
        for (self.pages, 0..) |page_slot, page_num| {
            if (page_slot) |page| {
                try self.flush(page, page_num);
            }
        }
    }

    pub fn flush(self: *Pager, page: []u8, page_num: usize) !void {
        try self.file.seekTo(page_num * PAGE_SIZE);
        const wrote = try self.file.write(page);
        if (wrote != PAGE_SIZE) {
            std.debug.print("Wrote less than a page size: {d}", .{wrote});
            return SerializationError.out_of_range;
        }
    }

    pub fn close(self: *Pager) !void {
        try self.flush_all();
        defer self.file.close();
        for (self.pages) |page_opt| {
            if (page_opt) |page| {
                self.allocator.free(page);
            }
        }
    }
};

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
    pager: Pager,
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
