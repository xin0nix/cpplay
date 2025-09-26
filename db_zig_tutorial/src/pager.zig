const std = @import("std");

const utils = @import("./utils.zig");

const SerializationError = utils.SerializationError;

const PAGE_SIZE = utils.PAGE_SIZE;

pub fn Pager(comptime kMaxPages: usize) type {
    return struct {
        file: std.fs.File,
        file_length: u64,
        pages: [kMaxPages]?[]u8,
        allocator: *std.mem.Allocator,

        pub fn open(sub_path: []const u8, allocator: *std.mem.Allocator) !Pager(kMaxPages) {
            const cwd = std.fs.cwd();
            const file = try cwd.openFile(sub_path, .{
                .mode = .read_write,
            });
            const file_length = (try file.stat()).size;
            if (file_length % PAGE_SIZE > 0) {
                std.debug.print("File size does not fit into page size {d}\n", .{file_length});
                return SerializationError.out_of_range;
            }
            var pager = Pager(kMaxPages){
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

        pub fn get_page(self: *Pager(kMaxPages), page_num: usize) ![]u8 {
            if (page_num >= kMaxPages) {
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

        pub fn flush_all(self: *Pager(kMaxPages)) !void {
            for (self.pages, 0..) |page_slot, page_num| {
                if (page_slot) |page| {
                    try self.flush(page, page_num);
                }
            }
        }

        pub fn flush(self: *Pager(kMaxPages), page: []u8, page_num: usize) !void {
            try self.file.seekTo(page_num * PAGE_SIZE);
            const wrote = try self.file.write(page);
            if (wrote != PAGE_SIZE) {
                std.debug.print("Wrote less than a page size: {d}\n", .{wrote});
                return SerializationError.out_of_range;
            }
        }

        pub fn close(self: *Pager(kMaxPages)) !void {
            try self.flush_all();
            defer self.file.close();
            for (self.pages) |page_opt| {
                if (page_opt) |page| {
                    self.allocator.free(page);
                }
            }
        }
    };
}
