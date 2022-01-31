const std = @import("std");
const panic = std.debug.panic;
const assert = std.debug.assert;
const win32 = @import("win32");
const w = win32.base;
const dwrite = win32.dwrite;
const d2d1 = win32.d2d1;
const c = @import("c.zig");

const L = std.unicode.utf8ToUtf16LeStringLiteral;

// TODO(mziulek): Handle more error codes from:
// https://docs.microsoft.com/en-us/windows/win32/com/com-error-codes-10
pub const HResultError = error{
    E_FAIL,
    E_OUTOFMEMORY,
    E_INVALIDARG,
    E_NOTIMPL,
    E_FILE_NOT_FOUND,
    E_NOINTERFACE,
    D3D12_ERROR_ADAPTER_NOT_FOUND,
    D3D12_ERROR_DRIVER_VERSION_MISMATCH,
    DXGI_ERROR_INVALID_CALL,
    DXGI_ERROR_NOT_FOUND,
    DXGI_ERROR_WAS_STILL_DRAWING,
    DXGI_STATUS_MODE_CHANGED,
    DWRITE_E_FILEFORMAT,
    XAPO_E_FORMAT_UNSUPPORTED,
};

pub fn hrPanic(err: HResultError) noreturn {
    panic(
        "HRESULT error detected (0x{x}, {}).",
        .{ @bitCast(c_ulong, hrErrorToCode(err)), err },
    );
}

pub inline fn hrPanicOnFail(hr: w.HRESULT) void {
    if (hr != w.S_OK) {
        hrPanic(hrCodeToError(hr));
    }
}

pub inline fn hrErrorOnFail(hr: w.HRESULT) HResultError!void {
    if (hr != w.S_OK) {
        return hrCodeToError(hr);
    }
}

fn hrErrorToCode(err: HResultError) w.HRESULT {
    return switch (err) {
        HResultError.D3D12_ERROR_ADAPTER_NOT_FOUND => w.D3D12_ERROR_ADAPTER_NOT_FOUND,
        HResultError.D3D12_ERROR_DRIVER_VERSION_MISMATCH => w.D3D12_ERROR_DRIVER_VERSION_MISMATCH,
        HResultError.DXGI_ERROR_INVALID_CALL => w.DXGI_ERROR_INVALID_CALL,
        HResultError.DXGI_ERROR_NOT_FOUND => w.DXGI_ERROR_NOT_FOUND,
        HResultError.DXGI_ERROR_WAS_STILL_DRAWING => w.DXGI_ERROR_WAS_STILL_DRAWING,
        HResultError.DXGI_STATUS_MODE_CHANGED => w.DXGI_STATUS_MODE_CHANGED,
        HResultError.DWRITE_E_FILEFORMAT => w.DWRITE_E_FILEFORMAT,
        HResultError.E_FAIL => w.E_FAIL,
        HResultError.E_OUTOFMEMORY => w.E_OUTOFMEMORY,
        HResultError.E_INVALIDARG => w.E_INVALIDARG,
        HResultError.E_NOTIMPL => w.E_NOTIMPL,
        HResultError.E_FILE_NOT_FOUND => w.E_FILE_NOT_FOUND,
        HResultError.E_NOINTERFACE => w.E_NOINTERFACE,
        HResultError.XAPO_E_FORMAT_UNSUPPORTED => w.XAPO_E_FORMAT_UNSUPPORTED,
    };
}

fn hrCodeToError(hr: w.HRESULT) HResultError {
    assert(hr != w.S_OK);
    const code = @bitCast(c_ulong, hr);
    return switch (code) {
        @bitCast(c_ulong, w.D3D12_ERROR_ADAPTER_NOT_FOUND) => HResultError.D3D12_ERROR_ADAPTER_NOT_FOUND,
        @bitCast(c_ulong, w.D3D12_ERROR_DRIVER_VERSION_MISMATCH) => HResultError.D3D12_ERROR_DRIVER_VERSION_MISMATCH,
        @bitCast(c_ulong, w.DXGI_ERROR_INVALID_CALL) => HResultError.DXGI_ERROR_INVALID_CALL,
        @bitCast(c_ulong, w.DXGI_ERROR_NOT_FOUND) => HResultError.DXGI_ERROR_NOT_FOUND,
        @bitCast(c_ulong, w.DXGI_ERROR_WAS_STILL_DRAWING) => HResultError.DXGI_ERROR_WAS_STILL_DRAWING,
        @bitCast(c_ulong, w.DXGI_STATUS_MODE_CHANGED) => HResultError.DXGI_STATUS_MODE_CHANGED,
        @bitCast(c_ulong, w.DWRITE_E_FILEFORMAT) => HResultError.DWRITE_E_FILEFORMAT,
        @bitCast(c_ulong, w.E_OUTOFMEMORY) => HResultError.E_OUTOFMEMORY,
        @bitCast(c_ulong, w.E_INVALIDARG) => HResultError.E_INVALIDARG,
        @bitCast(c_ulong, w.E_NOTIMPL) => HResultError.E_NOTIMPL,
        @bitCast(c_ulong, w.E_FILE_NOT_FOUND) => HResultError.E_FILE_NOT_FOUND,
        @bitCast(c_ulong, w.E_NOINTERFACE) => HResultError.E_NOINTERFACE,
        @bitCast(c_ulong, w.XAPO_E_FORMAT_UNSUPPORTED) => HResultError.XAPO_E_FORMAT_UNSUPPORTED,
        else => blk: {
            std.debug.print("HRESULT error 0x{x} not recognized treating as E_FAIL.", .{@bitCast(c_ulong, hr)});
            break :blk HResultError.E_FAIL;
        },
    };
}

pub const FrameStats = struct {
    time: f64,
    delta_time: f32,
    fps: f32,
    average_cpu_time: f32,
    timer: std.time.Timer,
    previous_time_ns: u64,
    fps_refresh_time_ns: u64,
    frame_counter: u64,

    pub fn init() FrameStats {
        return .{
            .time = 0.0,
            .delta_time = 0.0,
            .fps = 0.0,
            .average_cpu_time = 0.0,
            .timer = std.time.Timer.start() catch unreachable,
            .previous_time_ns = 0,
            .fps_refresh_time_ns = 0,
            .frame_counter = 0,
        };
    }

    pub fn update(self: *FrameStats) void {
        const now_ns = self.timer.read();
        self.time = @intToFloat(f64, now_ns) / std.time.ns_per_s;
        self.delta_time = @intToFloat(f32, now_ns - self.previous_time_ns) / std.time.ns_per_s;
        self.previous_time_ns = now_ns;

        if ((now_ns - self.fps_refresh_time_ns) >= std.time.ns_per_s) {
            const t = @intToFloat(f64, now_ns - self.fps_refresh_time_ns) / std.time.ns_per_s;
            const fps = @intToFloat(f64, self.frame_counter) / t;
            const ms = (1.0 / fps) * 1000.0;

            self.fps = @floatCast(f32, fps);
            self.average_cpu_time = @floatCast(f32, ms);
            self.fps_refresh_time_ns = now_ns;
            self.frame_counter = 0;
        }
        self.frame_counter += 1;
    }
};

fn processWindowMessage(
    window: w.HWND,
    message: w.UINT,
    wparam: w.WPARAM,
    lparam: w.LPARAM,
) callconv(w.WINAPI) w.LRESULT {
    assert(c.igGetCurrentContext() != null);
    var ui = c.igGetIO().?;
    var ui_backend = @ptrCast(*GuiBackendState, @alignCast(8, ui.*.BackendPlatformUserData));
    switch (message) {
        w.user32.WM_LBUTTONDOWN,
        w.user32.WM_RBUTTONDOWN,
        w.user32.WM_MBUTTONDOWN,
        w.user32.WM_LBUTTONDBLCLK,
        w.user32.WM_RBUTTONDBLCLK,
        w.user32.WM_MBUTTONDBLCLK,
        => {
            var button: u32 = 0;
            if (message == w.user32.WM_LBUTTONDOWN or message == w.user32.WM_LBUTTONDBLCLK) button = 0;
            if (message == w.user32.WM_RBUTTONDOWN or message == w.user32.WM_RBUTTONDBLCLK) button = 1;
            if (message == w.user32.WM_MBUTTONDOWN or message == w.user32.WM_MBUTTONDBLCLK) button = 2;
            if (c.igIsAnyMouseDown() == false and w.GetCapture() == null) {
                _ = w.SetCapture(window);
            }
            ui.*.MouseDown[button] = true;
        },
        w.user32.WM_LBUTTONUP,
        w.user32.WM_RBUTTONUP,
        w.user32.WM_MBUTTONUP,
        => {
            var button: u32 = 0;
            if (message == w.user32.WM_LBUTTONUP) button = 0;
            if (message == w.user32.WM_RBUTTONUP) button = 1;
            if (message == w.user32.WM_MBUTTONUP) button = 2;
            ui.*.MouseDown[button] = false;
            if (c.igIsAnyMouseDown() == false and w.GetCapture() == window) {
                _ = w.ReleaseCapture();
            }
        },
        w.user32.WM_MOUSEWHEEL => {
            ui.*.MouseWheel += @intToFloat(f32, w.GET_WHEEL_DELTA_WPARAM(wparam)) / @intToFloat(f32, w.WHEEL_DELTA);
        },
        w.user32.WM_MOUSEMOVE => {
            ui_backend.*.mouse_window = window;
            if (ui_backend.*.mouse_tracked == false) {
                var tme = w.TRACKMOUSEEVENT{
                    .cbSize = @sizeOf(w.TRACKMOUSEEVENT),
                    .dwFlags = w.TME_LEAVE,
                    .hwndTrack = window,
                    .dwHoverTime = 0,
                };
                _ = w.TrackMouseEvent(&tme);
                ui_backend.*.mouse_tracked = true;
            }
        },
        w.user32.WM_MOUSELEAVE => {
            if (ui_backend.*.mouse_window == window) {
                ui_backend.*.mouse_window = null;
            }
            ui_backend.*.mouse_tracked = false;
        },
        w.user32.WM_KEYDOWN,
        w.user32.WM_KEYUP,
        w.user32.WM_SYSKEYDOWN,
        w.user32.WM_SYSKEYUP,
        => {
            if (wparam == w.VK_ESCAPE) {
                w.user32.PostQuitMessage(0);
            }
            const down = if (message == w.user32.WM_KEYDOWN or message == w.user32.WM_SYSKEYDOWN) true else false;
            if (wparam < 256)
                ui.*.KeysDown[wparam] = down;
            if (wparam == w.VK_CONTROL)
                ui.*.KeyCtrl = down;
            if (wparam == w.VK_SHIFT)
                ui.*.KeyShift = down;
            if (wparam == w.VK_MENU)
                ui.*.KeyAlt = down;
        },
        w.user32.WM_SETFOCUS,
        w.user32.WM_KILLFOCUS,
        => {
            c.ImGuiIO_AddFocusEvent(ui, if (message == w.user32.WM_SETFOCUS) true else false);
        },
        w.user32.WM_CHAR => {
            if (wparam > 0 and wparam < 0x10000) {
                c.ImGuiIO_AddInputCharacterUTF16(ui, @intCast(u16, wparam & 0xffff));
            }
        },
        w.user32.WM_DESTROY => {
            w.user32.PostQuitMessage(0);
        },
        else => {
            return w.user32.defWindowProcA(window, message, wparam, lparam);
        },
    }
    return 0;
}

const GuiBackendState = struct {
    window: ?w.HWND,
    mouse_window: ?w.HWND,
    mouse_tracked: bool,
};

pub fn initWindow(allocator: std.mem.Allocator, name: [*:0]const u8, width: u32, height: u32) !w.HWND {
    assert(c.igGetCurrentContext() == null);
    _ = c.igCreateContext(null);

    var ui = c.igGetIO().?;
    assert(ui.*.BackendPlatformUserData == null);

    const ui_backend = allocator.create(GuiBackendState) catch unreachable;
    errdefer allocator.destroy(ui_backend);

    ui_backend.*.window = null;
    ui_backend.*.mouse_window = null;
    ui_backend.*.mouse_tracked = false;

    ui.*.BackendPlatformUserData = ui_backend;
    ui.*.BackendFlags |= c.ImGuiBackendFlags_RendererHasVtxOffset;

    const winclass = w.user32.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(w.HINSTANCE, w.kernel32.GetModuleHandleW(null)),
        .hIcon = null,
        .hCursor = w.LoadCursorA(null, @intToPtr(w.LPCSTR, 32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = name,
        .hIconSm = null,
    };
    _ = try w.user32.registerClassExA(&winclass);

    const style = w.user32.WS_OVERLAPPED +
        w.user32.WS_SYSMENU +
        w.user32.WS_CAPTION +
        w.user32.WS_MINIMIZEBOX;

    var rect = w.RECT{ .left = 0, .top = 0, .right = @intCast(i32, width), .bottom = @intCast(i32, height) };
    // HACK(mziulek): For exact FullHD window size it is better to stick to requested total window size (looks better on 1920x1080 displays).
    if (width != 1920 and height != 1080) {
        try w.user32.adjustWindowRectEx(&rect, style, false, 0);
    }

    const window = try w.user32.createWindowExA(
        0,
        name,
        name,
        style + w.user32.WS_VISIBLE,
        -1,
        -1,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    );
    ui_backend.*.window = window;

    var actual_rect: w.RECT = undefined;
    _ = w.GetClientRect(window, &actual_rect);
    const viewport_width = @intCast(u32, actual_rect.right - actual_rect.left);
    const viewport_height = @intCast(u32, actual_rect.bottom - actual_rect.top);

    ui.*.KeyMap[c.ImGuiKey_Tab] = w.VK_TAB;
    ui.*.KeyMap[c.ImGuiKey_LeftArrow] = w.VK_LEFT;
    ui.*.KeyMap[c.ImGuiKey_RightArrow] = w.VK_RIGHT;
    ui.*.KeyMap[c.ImGuiKey_UpArrow] = w.VK_UP;
    ui.*.KeyMap[c.ImGuiKey_DownArrow] = w.VK_DOWN;
    ui.*.KeyMap[c.ImGuiKey_PageUp] = w.VK_PRIOR;
    ui.*.KeyMap[c.ImGuiKey_PageDown] = w.VK_NEXT;
    ui.*.KeyMap[c.ImGuiKey_Home] = w.VK_HOME;
    ui.*.KeyMap[c.ImGuiKey_End] = w.VK_END;
    ui.*.KeyMap[c.ImGuiKey_Delete] = w.VK_DELETE;
    ui.*.KeyMap[c.ImGuiKey_Backspace] = w.VK_BACK;
    ui.*.KeyMap[c.ImGuiKey_Enter] = w.VK_RETURN;
    ui.*.KeyMap[c.ImGuiKey_Escape] = w.VK_ESCAPE;
    ui.*.KeyMap[c.ImGuiKey_Space] = w.VK_SPACE;
    ui.*.KeyMap[c.ImGuiKey_Insert] = w.VK_INSERT;
    ui.*.KeyMap[c.ImGuiKey_A] = 'A';
    ui.*.KeyMap[c.ImGuiKey_C] = 'C';
    ui.*.KeyMap[c.ImGuiKey_V] = 'V';
    ui.*.KeyMap[c.ImGuiKey_X] = 'X';
    ui.*.KeyMap[c.ImGuiKey_Y] = 'Y';
    ui.*.KeyMap[c.ImGuiKey_Z] = 'Z';
    ui.*.ImeWindowHandle = window;
    ui.*.DisplaySize = .{ .x = @intToFloat(f32, viewport_width), .y = @intToFloat(f32, viewport_height) };
    c.igGetStyle().?.*.WindowRounding = 0.0;

    return window;
}

pub fn deinitWindow(allocator: std.mem.Allocator) void {
    var ui = c.igGetIO().?;
    assert(ui.*.BackendPlatformUserData != null);
    allocator.destroy(@ptrCast(*GuiBackendState, @alignCast(8, ui.*.BackendPlatformUserData)));
    c.igDestroyContext(null);
}

pub fn newImGuiFrame(delta_time: f32) void {
    assert(c.igGetCurrentContext() != null);

    var ui = c.igGetIO().?;
    var ui_backend = @ptrCast(*GuiBackendState, @alignCast(8, ui.*.BackendPlatformUserData));
    assert(ui_backend.*.window != null);

    ui.*.MousePos = c.ImVec2{ .x = -c.igGET_FLT_MAX(), .y = -c.igGET_FLT_MAX() };

    const focused_window = w.GetForegroundWindow();
    const hovered_window = ui_backend.*.mouse_window;
    var mouse_window: ?w.HWND = null;
    if (hovered_window != null and
        (hovered_window == ui_backend.*.window or w.IsChild(hovered_window, ui_backend.*.window) == w.TRUE))
    {
        mouse_window = hovered_window;
    } else if (focused_window != null and
        (focused_window == ui_backend.*.window or w.IsChild(focused_window, ui_backend.*.window) == w.TRUE))
    {
        mouse_window = focused_window;
    }

    if (mouse_window != null) {
        var pos: w.POINT = undefined;
        if (w.GetCursorPos(&pos) == w.TRUE and w.ScreenToClient(mouse_window, &pos) == w.TRUE) {
            ui.*.MousePos = c.ImVec2{ .x = @intToFloat(f32, pos.x), .y = @intToFloat(f32, pos.y) };
        }
    }

    ui.*.DeltaTime = delta_time;
    c.igNewFrame();
}

pub fn drawText(
    devctx: *d2d1.IDeviceContext6,
    text: []const u8,
    format: *dwrite.ITextFormat,
    layout_rect: *const d2d1.RECT_F,
    brush: *d2d1.IBrush,
) void {
    var utf16: [128:0]u16 = undefined;
    assert(text.len < utf16.len);
    const len = std.unicode.utf8ToUtf16Le(utf16[0..], text) catch unreachable;
    utf16[len] = 0;
    devctx.DrawText(
        &utf16,
        @intCast(u32, len),
        format,
        layout_rect,
        brush,
        d2d1.DRAW_TEXT_OPTIONS_NONE,
        .NATURAL,
    );
}

pub fn init() void {
    _ = w.ole32.CoInitializeEx(
        null,
        @enumToInt(w.COINIT_APARTMENTTHREADED) | @enumToInt(w.COINIT_DISABLE_OLE1DDE),
    );
    _ = w.SetProcessDPIAware();

    var version: w.OSVERSIONINFOW = undefined;
    _ = w.ntdll.RtlGetVersion(&version);

    var os_is_supported = false;
    if (version.dwMajorVersion > 10) {
        os_is_supported = true;
    } else if (version.dwMajorVersion == 10 and version.dwBuildNumber >= 18363) {
        os_is_supported = true;
    }

    const d3d12core_dll = w.kernel32.LoadLibraryW(L("D3D12Core.dll"));
    if (d3d12core_dll == null) {
        os_is_supported = false;
    } else {
        _ = w.kernel32.FreeLibrary(d3d12core_dll.?);
    }

    if (!os_is_supported) {
        _ = w.user32.messageBoxA(
            null,
            \\This application can't run on currently installed version of Windows.
            \\Following versions are supported:
            \\
            \\Windows 10 May 2021 (Build 19043) or newer
            \\Windows 10 October 2020 (Build 19042.789+)
            \\Windows 10 May 2020 (Build 19041.789+)
            \\Windows 10 November 2019 (Build 18363.1350+)
            \\
            \\Please update your Windows version and try again.
        ,
            "Error",
            w.user32.MB_OK | w.user32.MB_ICONERROR,
        ) catch 0;
        w.kernel32.ExitProcess(0);
    }
}

pub fn deinit() void {
    w.ole32.CoUninitialize();
}

pub fn parseAndLoadGltfFile(gltf_path: []const u8) *c.cgltf_data {
    var data: *c.cgltf_data = undefined;
    const options = std.mem.zeroes(c.cgltf_options);
    // Parse.
    {
        const result = c.cgltf_parse_file(&options, gltf_path.ptr, @ptrCast([*c][*c]c.cgltf_data, &data));
        assert(result == c.cgltf_result_success);
    }
    // Load.
    {
        const result = c.cgltf_load_buffers(&options, data, gltf_path.ptr);
        assert(result == c.cgltf_result_success);
    }
    return data;
}

pub fn appendMeshPrimitive(
    data: *c.cgltf_data,
    mesh_index: u32,
    prim_index: u32,
    indices: *std.ArrayList(u32),
    positions: *std.ArrayList([3]f32),
    normals: ?*std.ArrayList([3]f32),
    texcoords0: ?*std.ArrayList([2]f32),
    tangents: ?*std.ArrayList([4]f32),
) void {
    assert(mesh_index < data.meshes_count);
    assert(prim_index < data.meshes[mesh_index].primitives_count);
    const num_vertices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].attributes[0].data.*.count);
    const num_indices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].indices.*.count);

    // Indices.
    {
        indices.ensureTotalCapacity(indices.items.len + num_indices) catch unreachable;

        const accessor = data.meshes[mesh_index].primitives[prim_index].indices;

        assert(accessor.*.buffer_view != null);
        assert(accessor.*.stride == accessor.*.buffer_view.*.stride or accessor.*.buffer_view.*.stride == 0);
        assert((accessor.*.stride * accessor.*.count) == accessor.*.buffer_view.*.size);
        assert(accessor.*.buffer_view.*.buffer.*.data != null);

        const data_addr = @alignCast(4, @ptrCast([*]const u8, accessor.*.buffer_view.*.buffer.*.data) +
            accessor.*.offset + accessor.*.buffer_view.*.offset);

        if (accessor.*.stride == 1) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_8u);
            const src = @ptrCast([*]const u8, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.*.stride == 2) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_16u);
            const src = @ptrCast([*]const u16, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.*.stride == 4) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_32u);
            const src = @ptrCast([*]const u32, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else {
            unreachable;
        }
    }

    // Attributes.
    {
        positions.resize(positions.items.len + num_vertices) catch unreachable;
        if (normals != null) normals.?.resize(normals.?.items.len + num_vertices) catch unreachable;
        if (texcoords0 != null) texcoords0.?.resize(texcoords0.?.items.len + num_vertices) catch unreachable;
        if (tangents != null) tangents.?.resize(tangents.?.items.len + num_vertices) catch unreachable;

        const num_attribs: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].attributes_count);

        var attrib_index: u32 = 0;
        while (attrib_index < num_attribs) : (attrib_index += 1) {
            const attrib = &data.meshes[mesh_index].primitives[prim_index].attributes[attrib_index];
            const accessor = attrib.data;

            assert(accessor.*.buffer_view != null);
            assert(accessor.*.stride == accessor.*.buffer_view.*.stride or accessor.*.buffer_view.*.stride == 0);
            assert((accessor.*.stride * accessor.*.count) == accessor.*.buffer_view.*.size);
            assert(accessor.*.buffer_view.*.buffer.*.data != null);

            const data_addr = @ptrCast([*]const u8, accessor.*.buffer_view.*.buffer.*.data) +
                accessor.*.offset + accessor.*.buffer_view.*.offset;

            if (attrib.*.type == c.cgltf_attribute_type_position) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &positions.items[positions.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_normal and normals != null) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &normals.?.items[normals.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_texcoord and texcoords0 != null) {
                assert(accessor.*.type == c.cgltf_type_vec2);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &texcoords0.?.items[texcoords0.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_tangent and tangents != null) {
                assert(accessor.*.type == c.cgltf_type_vec4);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &tangents.?.items[tangents.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            }
        }
    }
}
