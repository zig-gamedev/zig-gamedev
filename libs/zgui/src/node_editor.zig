const std = @import("std");

const Style = extern struct {
    node_padding: [4]f32 = .{ 8, 8, 8, 8 },
    node_rounding: f32 = 12,
    node_border_width: f32 = 1.5,
    hovered_node_border_width: f32 = 3.5,
    hover_node_border_offset: f32 = 0,
    selected_node_border_width: f32 = 3.5,
    selected_node_border_offset: f32 = 0,
    pin_rounding: f32 = 4,
    pin_border_width: f32 = 0,
    link_strength: f32 = 100,
    source_direction: [2]f32 = .{ 1, 0 },
    target_direction: [2]f32 = .{ -1, 0 },
    scroll_duration: f32 = 0.35,
    flow_marker_distance: f32 = 30,
    flow_speed: f32 = 150.0,
    flow_duration: f32 = 2.0,
    pivot_alignment: [2]f32 = .{ 0.5, 0.5 },
    pivot_size: [2]f32 = .{ 0, 0 },
    pivot_scale: [2]f32 = .{ 1, 1 },
    pin_corners: f32 = 240,
    pin_radius: f32 = 0,
    pin_arrow_size: f32 = 0,
    pin_arrow_width: f32 = 0,
    group_rounding: f32 = 6,
    group_border_width: f32 = 1,
    highlight_connected_links: f32 = 0,
    snap_link_to_pin_dir: f32 = 0,

    colors: [@typeInfo(StyleColor).Enum.fields.len][4]f32,

    pub fn getColor(style: Style, idx: StyleColor) [4]f32 {
        return style.colors[@intCast(@intFromEnum(idx))];
    }
    pub fn setColor(style: *Style, idx: StyleColor, color: [4]f32) void {
        style.colors[@intCast(@intFromEnum(idx))] = color;
    }
};

const StyleColor = enum(c_int) {
    bg,
    grid,
    node_bg,
    node_border,
    hov_node_border,
    sel_node_border,
    node_sel_rect,
    node_sel_rect_border,
    hov_link_border,
    sel_link_border,
    highlight_link_border,
    link_sel_rect,
    link_sel_rect_border,
    pin_rect,
    pin_rect_border,
    flow,
    flow_marker,
    group_bg,
    group_border,
    count,
};

const StyleVar = enum(c_int) {
    node_padding,
    node_rounding,
    node_border_width,
    hovered_node_border_width,
    selected_node_border_width,
    pin_rounding,
    pin_border_width,
    link_strength,
    source_direction,
    target_direction,
    scroll_duration,
    flow_marker_distance,
    flow_speed,
    flow_duration,
    pivot_alignment,
    pivot_size,
    pivot_scale,
    pin_corners,
    pin_radius,
    pin_arrow_size,
    pin_arrow_width,
    group_rounding,
    group_border_width,
    highlight_connected_links,
    snap_link_to_pin_dir,
    hovered_node_border_offset,
    selected_node_border_offset,
    count,
};

pub const EditorContext = opaque {
    pub fn create(config: Config) *EditorContext {
        return node_editor_CreateEditor(&config);
    }
    extern fn node_editor_CreateEditor(config: *const Config) *EditorContext;

    pub fn destroy(self: *EditorContext) void {
        return node_editor_DestroyEditor(self);
    }
    extern fn node_editor_DestroyEditor(editor: *EditorContext) void;
};

const CanvasSizeMode = enum(c_int) {
    FitVerticalView, // Previous view will be scaled to fit new view on Y axis
    FitHorizontalView, // Previous view will be scaled to fit new view on X axis
    CenterOnly, // Previous view will be centered on new view
};

const SaveNodeSettings = fn (nodeId: NodeId, data: [*]const u8, size: usize, reason: SaveReasonFlags, userPointer: *anyopaque) callconv(.C) bool;
const LoadNodeSettings = fn (nodeId: NodeId, data: [*]u8, userPointer: *anyopaque) callconv(.C) usize;
const SaveSettings = fn (data: [*]const u8, size: usize, reason: SaveReasonFlags, userPointer: *anyopaque) callconv(.C) bool;
const LoadSettings = fn (data: [*]u8, userPointer: *anyopaque) callconv(.C) usize;
const ConfigSession = fn (userPointer: *anyopaque) callconv(.C) void;

const _ImVector = extern struct {
    Size: c_int = 0,
    Capacity: c_int = 0,
    Data: ?*anyopaque = null,
};

pub const Config = extern struct {
    settings_file: ?*const u8 = null,
    begin_save_session: ?*const ConfigSession = null,
    end_save_session: ?*const ConfigSession = null,
    save_settings: ?*const SaveSettings = null,
    load_settings: ?*const LoadSettings = null,
    save_node_settings: ?*const SaveNodeSettings = null,
    load_node_settings: ?*const LoadNodeSettings = null,
    user_pointer: ?*anyopaque = null,
    custom_zoom_levels: _ImVector = .{},
    canvas_size_mode: CanvasSizeMode = .FitVerticalView,
    drag_button_index: c_int = 0,
    select_button_index: c_int = 0,
    navigate_button_index: c_int = 1,
    context_menu_button_index: c_int = 1,
    enable_smooth_zoom: bool = false,
    smooth_zoom_power: f32 = 1.1,
};

//
// Editor
//
const SaveReasonFlags = packed struct(u32) {
    navigation: bool,
    position: bool,
    size: bool,
    selection: bool,
    add_node: bool,
    remove_node: bool,
    user: bool,
    _pad: u25,
};

pub fn setCurrentEditor(editor: ?*EditorContext) void {
    node_editor_SetCurrentEditor(editor);
}
extern fn node_editor_SetCurrentEditor(editor: ?*EditorContext) void;

pub fn begin(id: [:0]const u8, size: [2]f32) void {
    node_editor_Begin(id, &size);
}
extern fn node_editor_Begin(id: [*c]const u8, size: [*]const f32) void;

pub fn end() void {
    node_editor_End();
}
extern fn node_editor_End() void;

pub fn showBackgroundContextMenu() bool {
    return node_editor_ShowBackgroundContextMenu();
}
extern fn node_editor_ShowBackgroundContextMenu() bool;

pub fn showNodeContextMenu(id: *NodeId) bool {
    return node_editor_ShowNodeContextMenu(id);
}
extern fn node_editor_ShowNodeContextMenu(id: *NodeId) bool;

pub fn showLinkContextMenu(id: *LinkId) bool {
    return node_editor_ShowLinkContextMenu(id);
}
extern fn node_editor_ShowLinkContextMenu(id: *LinkId) bool;

pub fn showPinContextMenu(id: *PinId) bool {
    return node_editor_ShowPinContextMenu(id);
}
extern fn node_editor_ShowPinContextMenu(id: *PinId) bool;

pub fn suspend_() void {
    return node_editor_Suspend();
}
extern fn node_editor_Suspend() void;

pub fn resume_() void {
    return node_editor_Resume();
}
extern fn node_editor_Resume() void;

pub fn navigateToContent(duration: f32) void {
    node_editor_NavigateToContent(duration);
}
extern fn node_editor_NavigateToContent(duration: f32) void;

pub fn navigateToSelection(zoomIn: bool, duration: f32) void {
    node_editor_NavigateToSelection(zoomIn, duration);
}
extern fn node_editor_NavigateToSelection(zoomIn: bool, duration: f32) void;

pub fn selectNode(nodeId: NodeId, append: bool) void {
    node_editor_SelectNode(nodeId, append);
}
extern fn node_editor_SelectNode(nodeId: NodeId, append: bool) void;

pub fn selectLink(linkId: LinkId, append: bool) void {
    node_editor_SelectLink(linkId, append);
}
extern fn node_editor_SelectLink(linkId: LinkId, append: bool) void;

//
// Node
//
const NodeId = u64;

pub fn beginNode(id: NodeId) void {
    node_editor_BeginNode(id);
}
extern fn node_editor_BeginNode(id: NodeId) void;

pub fn endNode() void {
    node_editor_EndNode();
}
extern fn node_editor_EndNode() void;

pub fn setNodePosition(id: NodeId, pos: [2]f32) void {
    node_editor_SetNodePosition(id, &pos);
}
extern fn node_editor_SetNodePosition(id: NodeId, pos: [*]const f32) void;

pub fn getNodePosition(id: NodeId) [2]f32 {
    var pos: [2]f32 = .{ 0, 0 };
    node_editor_getNodePosition(id, &pos);
    return pos;
}
extern fn node_editor_getNodePosition(id: NodeId, pos: [*]f32) void;

pub fn getNodeSize(id: NodeId) [2]f32 {
    var size: [2]f32 = .{ 0, 0 };
    node_editor_getNodeSize(id, &size);
    return size;
}
extern fn node_editor_getNodeSize(id: NodeId, size: [*]f32) void;

pub fn deleteNode(id: NodeId) bool {
    return node_editor_DeleteNode(id);
}
extern fn node_editor_DeleteNode(id: NodeId) bool;

//
// Pin
//
const PinId = u64;

const PinKind = enum(u32) {
    input = 0,
    output,
};

pub fn beginPin(id: PinId, kind: PinKind) void {
    node_editor_BeginPin(id, kind);
}
extern fn node_editor_BeginPin(id: PinId, kind: PinKind) void;

pub fn endPin() void {
    node_editor_EndPin();
}
extern fn node_editor_EndPin() void;

pub fn pinHadAnyLinks(pinId: PinId) bool {
    return node_editor_PinHadAnyLinks(pinId);
}
extern fn node_editor_PinHadAnyLinks(pinId: PinId) bool;

pub fn pinRect(a: [2]f32, b: [2]f32) void {
    node_editor_PinRect(&a, &b);
}
extern fn node_editor_PinRect(a: [*]const f32, b: [*]const f32) void;

pub fn pinPivotRect(a: [2]f32, b: [2]f32) void {
    node_editor_PinPivotRect(&a, &b);
}
extern fn node_editor_PinPivotRect(a: [*]const f32, b: [*]const f32) void;

pub fn pinPivotSize(size: [2]f32) void {
    node_editor_PinPivotSize(&size);
}
extern fn node_editor_PinPivotSize(size: [*]const f32) void;

pub fn pinPivotScale(scale: [2]f32) void {
    node_editor_PinPivotScale(&scale);
}
extern fn node_editor_PinPivotScale(scale: [*]const f32) void;

pub fn pinPivotAlignment(alignment: [2]f32) void {
    node_editor_PinPivotAlignment(&alignment);
}
extern fn node_editor_PinPivotAlignment(alignment: [*]const f32) void;

//
// Link
//
const LinkId = u64;

pub fn link(id: LinkId, startPinId: PinId, endPinId: PinId, color: [4]f32, thickness: f32) bool {
    return node_editor_Link(id, startPinId, endPinId, &color, thickness);
}
extern fn node_editor_Link(id: LinkId, startPinId: PinId, endPinId: PinId, color: [*]const f32, thickness: f32) bool;

pub fn deleteLink(id: LinkId) bool {
    return node_editor_DeleteLink(id);
}
extern fn node_editor_DeleteLink(id: LinkId) bool;

pub fn breakPinLinks(id: PinId) i32 {
    return node_editor_BreakPinLinks(id);
}
extern fn node_editor_BreakPinLinks(id: PinId) c_int;

//
// Created
//

pub fn beginCreate() bool {
    return node_editor_BeginCreate();
}
extern fn node_editor_BeginCreate() bool;

pub fn endCreate() void {
    node_editor_EndCreate();
}
extern fn node_editor_EndCreate() void;

pub fn queryNewLink(startId: *?PinId, endId: *?PinId) bool {
    var sid: PinId = 0;
    var eid: PinId = 0;
    const result = node_editor_QueryNewLink(&sid, &eid);

    startId.* = if (sid == 0) null else sid;
    endId.* = if (eid == 0) null else eid;

    return result;
}
extern fn node_editor_QueryNewLink(startId: *PinId, endId: *PinId) bool;

pub fn acceptNewItem(color: [4]f32, thickness: f32) bool {
    return node_editor_AcceptNewItem(&color, thickness);
}
extern fn node_editor_AcceptNewItem(color: [*]const f32, thickness: f32) bool;

pub fn rejectNewItem(color: [4]f32, thickness: f32) void {
    node_editor_RejectNewItem(&color, thickness);
}
extern fn node_editor_RejectNewItem(color: [*]const f32, thickness: f32) void;

//
// Deleted
//
pub fn beginDelete() bool {
    return node_editor_BeginDelete();
}
extern fn node_editor_BeginDelete() bool;

pub fn endDelete() void {
    node_editor_EndDelete();
}
extern fn node_editor_EndDelete() void;

pub fn queryDeletedLink(linkId: *LinkId, startId: ?*PinId, endId: ?*PinId) bool {
    const result = node_editor_QueryDeletedLink(linkId, startId, endId);
    return result;
}
extern fn node_editor_QueryDeletedLink(linkId: *LinkId, startId: ?*PinId, endId: ?*PinId) bool;

pub fn queryDeletedNode(nodeId: *NodeId) bool {
    var nid: LinkId = 0;
    const result = node_editor_QueryDeletedNode(&nid);

    nodeId.* = nid;

    return result;
}
extern fn node_editor_QueryDeletedNode(nodeId: *NodeId) bool;

pub fn acceptDeletedItem(deleteDependencies: bool) bool {
    return node_editor_AcceptDeletedItem(deleteDependencies);
}
extern fn node_editor_AcceptDeletedItem(deleteDependencies: bool) bool;

pub fn rejectDeletedItem() void {
    node_editor_RejectDeletedItem();
}
extern fn node_editor_RejectDeletedItem() void;

//
// Style
//

pub fn getStyle() Style {
    return node_editor_GetStyle();
}
extern fn node_editor_GetStyle() Style;

pub fn getStyleColorName(colorIndex: StyleColor) [*c]const u8 {
    return node_editor_GetStyleColorName(colorIndex);
}
extern fn node_editor_GetStyleColorName(colorIndex: StyleColor) [*c]const u8;

pub fn pushStyleColor(colorIndex: StyleColor, color: [4]f32) void {
    node_editor_PushStyleColor(colorIndex, &color);
}
extern fn node_editor_PushStyleColor(colorIndex: StyleColor, color: [*]const f32) void;

pub fn popStyleColor(count: c_int) void {
    node_editor_PopStyleColor(count);
}
extern fn node_editor_PopStyleColor(count: c_int) void;

pub fn pushStyleVar1f(varIndex: StyleVar, value: f32) void {
    node_editor_PushStyleVarF(varIndex, value);
}
extern fn node_editor_PushStyleVarF(varIndex: StyleVar, value: f32) void;

pub fn pushStyleVar2f(varIndex: StyleVar, value: [2]f32) void {
    node_editor_PushStyleVar2f(varIndex, &value);
}
extern fn node_editor_PushStyleVar2f(varIndex: StyleVar, value: [*]const f32) void;

pub fn pushStyleVar4f(varIndex: StyleVar, value: [4]f32) void {
    node_editor_PushStyleVar4f(varIndex, &value);
}
extern fn node_editor_PushStyleVar4f(varIndex: StyleVar, value: [*]const f32) void;

pub fn popStyleVar(count: c_int) void {
    node_editor_PopStyleVar(count);
}
extern fn node_editor_PopStyleVar(count: c_int) void;

//
// Selection
//
pub fn hasSelectionChanged() bool {
    return node_editor_HasSelectionChanged();
}
extern fn node_editor_HasSelectionChanged() bool;

pub fn getSelectedObjectCount() c_int {
    return node_editor_GetSelectedObjectCount();
}
extern fn node_editor_GetSelectedObjectCount() c_int;

pub fn clearSelection() void {
    node_editor_ClearSelection();
}
extern fn node_editor_ClearSelection() void;

pub fn getSelectedNodes(nodes: []NodeId) c_int {
    return node_editor_GetSelectedNodes(nodes.ptr, @intCast(nodes.len));
}
extern fn node_editor_GetSelectedNodes(nodes: [*]NodeId, size: c_int) c_int;

pub fn getSelectedLinks(links: []LinkId) c_int {
    return node_editor_GetSelectedLinks(links.ptr, @intCast(links.len));
}
extern fn node_editor_GetSelectedLinks(links: [*]LinkId, size: c_int) c_int;

pub fn group(size: [2]f32) void {
    node_editor_Group(&size);
}
extern fn node_editor_Group(size: [*]const f32) void;

//
// Drawlist
//
pub fn getHintForegroundDrawList() *anyopaque {
    return node_editor_GetHintForegroundDrawList();
}
extern fn node_editor_GetHintForegroundDrawList() *anyopaque;

pub fn getHintBackgroundDrawLis() *anyopaque {
    return node_editor_GetHintBackgroundDrawList();
}
extern fn node_editor_GetHintBackgroundDrawList() *anyopaque;

pub fn getNodeBackgroundDrawList(node_id: NodeId) *anyopaque {
    return node_editor_GetNodeBackgroundDrawList(node_id);
}
extern fn node_editor_GetNodeBackgroundDrawList(node_id: NodeId) *anyopaque;
