const std = @import("std");
const utils = @import("./utils.zig");
const rows = @import("./row.zig");

const Row = rows.Row;

const NodeType = enum {
    internal,
    leaf,
};

const InternalError = utils.InternalError;

// Common Node Header Layout
pub const NODE_TYPE_SIZE = @sizeOf(u8);
pub const IS_ROOT_SIZE = @sizeOf(u8);
pub const PARENT_POINTER_SIZE = @sizeOf(u32);
pub const COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;
pub const NODE_TYPE_OFFSET = 0;
pub const IS_ROOT_OFFSET = NODE_TYPE_SIZE;
pub const PARENT_POINTER_OFFSET = NODE_TYPE_SIZE + IS_ROOT_SIZE;

// Leaf Node Header Layout
pub const LEAF_NODE_NUM_CELLS_SIZE = @sizeOf(u32);
pub const LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
pub const LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

// Leaf Node Body Layout
pub const LEAF_NODE_KEY_SIZE = @sizeOf(u32);
pub const LEAF_NODE_VALUE_SIZE = @sizeOf(Row);
pub const LEAF_NODE_KEY_OFFSET = 0;
pub const LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_SIZE;

pub const LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
pub const LEAF_NODE_SPACE_FOR_CELLS = utils.PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
pub const LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

pub const LeafNodeCellView = struct {
    key: []u8,
    value: []u8,

    pub fn getKey(self: *const LeafNodeCellView) !u32 {
        var result: u32 = undefined;
        try utils.deserializeFrom(u32, self.key, &result);
        return result;
    }
};

pub const NodeView = struct {
    node: []u8,

    pub fn initLeaf(node: []u8) !NodeView {
        var view = NodeView{ .node = node };
        try view.setLeafNodeNumCells(0);
        try view.setType(.leaf);
        return view;
    }

    pub fn getType(self: *const NodeView) !NodeType {
        const offset = NODE_TYPE_OFFSET;
        const slice = self.node[offset .. offset + NODE_TYPE_SIZE];
        var result: u8 = undefined;
        try utils.deserializeFrom(u8, slice, &result);
        return @enumFromInt(result);
    }

    pub fn setType(self: *const NodeView, nodeType: NodeType) !void {
        const offset = NODE_TYPE_OFFSET;
        const slice = self.node[offset .. offset + NODE_TYPE_SIZE];
        var obj: u8 = @intFromEnum(nodeType);
        try utils.serializeTo(u8, &obj, slice);
    }

    pub fn getLeafNodeNumCells(self: *const NodeView) !u32 {
        const offset = LEAF_NODE_NUM_CELLS_OFFSET;
        const slice = self.node[offset .. offset + LEAF_NODE_NUM_CELLS_SIZE];
        var result: u32 = undefined;
        try utils.deserializeFrom(u32, slice, &result);
        return result;
    }

    pub fn setLeafNodeNumCells(self: *NodeView, num_cells: u32) !void {
        const offset = LEAF_NODE_NUM_CELLS_OFFSET;
        const slice = self.node[offset .. offset + LEAF_NODE_NUM_CELLS_SIZE];
        try utils.serializeTo(u32, &num_cells, slice);
    }

    pub fn getLeafNodeCell(self: *const NodeView, cell_num: u32) LeafNodeCellView {
        const offset = LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
        const slice = self.node[offset .. offset + LEAF_NODE_CELL_SIZE];
        return LeafNodeCellView{
            .key = slice[0..LEAF_NODE_KEY_SIZE],
            .value = slice[LEAF_NODE_KEY_SIZE..],
        };
    }

    pub fn dumpLeafNode(self: *NodeView) !void {
        const num_cells = try self.getLeafNodeNumCells();
        std.debug.print("Leaf (size {d})\n", .{num_cells});
        for (0..num_cells) |i| {
            const cell = self.getLeafNodeCell(@intCast(i));
            var key: u32 = undefined;
            try utils.deserializeFrom(u32, cell.key, &key);
            std.debug.print(" - {d}: {d}\n", .{ i, key });
        }
    }
};
