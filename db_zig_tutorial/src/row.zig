const std = @import("std");
const utils = @import("./utils.zig");

const USERNAME_SIZE = utils.USERNAME_SIZE;
const EMAIL_SIZE = utils.EMAIL_SIZE;

const cStringLen = utils.cStringLen;

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
