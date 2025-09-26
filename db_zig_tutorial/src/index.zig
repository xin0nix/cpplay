const std = @import("std");
const utils = @import("./utils.zig");

const SerializationError = utils.SerializationError;

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
