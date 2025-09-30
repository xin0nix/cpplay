const std = @import("std");

const utils = @import("./utils.zig");

const InternalError = utils.InternalError;

const PAGE_SIZE = utils.PAGE_SIZE;

pub fn Pager(comptime kMaxPages: usize) type {
    return struct {
        file: std.fs.File,
        file_length: u64,
        pages: [kMaxPages]?[]u8,
        num_pages: u32,
        allocator: *std.mem.Allocator,

        pub fn open(sub_path: []const u8, allocator: *std.mem.Allocator) !Pager(kMaxPages) {
            const cwd = std.fs.cwd();
            const file = try cwd.openFile(sub_path, .{
                .mode = .read_write,
            });
            const file_length = (try file.stat()).size;
            const num_pages: u32 = @intCast(file_length / PAGE_SIZE);
            if (file_length % PAGE_SIZE != 0) {
                std.debug.print("File size is not aligned with a page size: {d}\n", .{file_length});
                return InternalError.out_of_range;
            }
            var pager = Pager(kMaxPages){
                .file = file,
                .file_length = file_length,
                .pages = undefined,
                .num_pages = num_pages,
                .allocator = allocator,
            };
            for (&pager.pages) |*page_slot| {
                page_slot.* = null;
            }
            return pager;
        }

        pub fn getPage(self: *Pager(kMaxPages), page_num: u32) ![]u8 {
            if (page_num >= kMaxPages) {
                return InternalError.out_of_range;
            }
            const pageLookup = self.pages[page_num];
            if (pageLookup) |page| {
                return page;
            } else {
                const page = try self.allocator.alloc(u8, PAGE_SIZE);
                if (page_num >= self.num_pages) {
                    self.num_pages = page_num + 1; // ???
                } else {
                    const seek = page_num * PAGE_SIZE;
                    try self.file.seekTo(seek);
                    const read = try self.file.read(page);
                    if (read != PAGE_SIZE) {
                        std.debug.print("Read less than a page size, {d}\n", .{read});
                        return InternalError.out_of_range;
                    }
                }

                self.pages[page_num] = page;
                return page;
            }
        }

        pub fn flushAll(self: *Pager(kMaxPages)) !void {
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
                return InternalError.out_of_range;
            }
        }

        pub fn close(self: *Pager(kMaxPages)) !void {
            try self.flushAll();
            defer self.file.close();
            for (self.pages) |page_opt| {
                if (page_opt) |page| {
                    self.allocator.free(page);
                }
            }
        }
    };
}
