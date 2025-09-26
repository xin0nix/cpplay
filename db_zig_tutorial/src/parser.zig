const std = @import("std");
const common = @import("common.zig");
const rows = @import("./row.zig");

const Command = common.Command;
const Statement = common.Statement;
const Row = rows.Row;
const Select = common.Select;

const Tokenizer = std.mem.TokenIterator(u8, std.mem.DelimiterType.any);

const InputError = error{
    invalid_token,
};

fn readToken(tokenizer: *Tokenizer) ![]const u8 {
    if (tokenizer.next()) |token| {
        return token;
    } else {
        return InputError.invalid_token;
    }
}

fn parseInteger(tokenizer: *Tokenizer) !u32 {
    return try std.fmt.parseInt(u32, try readToken(tokenizer), 10);
}

fn parseString(tokenizer: *Tokenizer, field: []u8) !void {
    const token = try readToken(tokenizer);
    if (token.len >= field.len) {
        std.debug.print("Token exceeds field capacity: {d} >= {d}\n", .{ token.len, field.len });
        return InputError.invalid_token;
    }
    @memcpy(field[0..token.len], token);
}

pub const Parser = struct {
    pub fn parse(_: *Parser, tokenizer: *Tokenizer) !Command {
        const stmt = try readToken(tokenizer);

        if (std.mem.eql(u8, stmt, ".exit")) {
            return Command.exit;
        }
        if (std.mem.eql(u8, stmt, "insert")) {
            var insert = Row.init(0, "", "");
            insert.id = try parseInteger(tokenizer);
            try parseString(tokenizer, &insert.username);
            try parseString(tokenizer, &insert.email);

            return Command{
                .statement = Statement{
                    .insert = insert,
                },
            };
        }
        const select = "select";
        if (std.mem.eql(u8, stmt, select)) {
            return Command{ .statement = Statement.select };
        }
        return InputError.invalid_token;
    }
};

test "Parser identify test" {
    var parser = Parser{};

    // Test .exit command
    {
        const view = ".exit";
        var tokenizer = std.mem.tokenizeAny(u8, view, " \n\t");
        try std.testing.expect((try parser.parse(&tokenizer)).equals(Command{ .exit = {} }));
    }
    {
        const view = ".e";
        var tokenizer = std.mem.tokenizeAny(u8, view, " \n\t");
        try std.testing.expect(parser.parse(&tokenizer) == InputError.invalid_token);
    }
    {
        const view = ".exits";
        var tokenizer = std.mem.tokenizeAny(u8, view, " \n\t");
        try std.testing.expect(parser.parse(&tokenizer) == InputError.invalid_token);
    }
    // Test insert statement
    {
        const view = "insert 42 joe joe@mail.com";
        const other = Command{
            .statement = Statement{
                .insert = Row.init(42, "joe", "joe@mail.com"),
            },
        };
        var tokenizer = std.mem.tokenizeAny(u8, view, " \n\t");
        try std.testing.expect((try parser.parse(&tokenizer)).equals(other));
    }
    {
        const view = "insert";
        var tokenizer = std.mem.tokenizeAny(u8, view, " \n\t");
        try std.testing.expect(parser.parse(&tokenizer) == InputError.invalid_token);
    }
    // Test select statement
    {
        const view = "select";
        const other = Command{
            .statement = Statement{
                .select = Select{},
            },
        };
        var tokenizer = std.mem.tokenizeAny(u8, view, " \n\t");
        try std.testing.expect((try parser.parse(&tokenizer)).equals(other));
    }
    {
        const view = "sele";
        var tokenizer = std.mem.tokenizeAny(u8, view, " \n\t");
        try std.testing.expect(parser.parse(&tokenizer) == InputError.invalid_token);
    }
}
