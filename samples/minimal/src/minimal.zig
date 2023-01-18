const std = @import("std");
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const dxgi = zwin32.dxgi;
const d3d12 = zwin32.d3d12;
const hrPanicOnFail = zwin32.hrPanicOnFail;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: minimal";
const window_width = 1600;
const window_height = 1200;

fn processWindowMessage(
    window: w32.HWND,
    message: w32.UINT,
    wparam: w32.WPARAM,
    lparam: w32.LPARAM,
) callconv(w32.WINAPI) w32.LRESULT {
    switch (message) {
        w32.WM_KEYDOWN => {
            if (wparam == w32.VK_ESCAPE) {
                w32.PostQuitMessage(0);
                return 0;
            }
        },
        w32.WM_DESTROY => {
            w32.PostQuitMessage(0);
            return 0;
        },
        else => {},
    }
    return w32.DefWindowProcA(window, message, wparam, lparam);
}

pub fn main() !void {
    _ = w32.CoInitializeEx(null, w32.COINIT_MULTITHREADED);
    defer w32.CoUninitialize();

    _ = w32.SetProcessDPIAware();

    //
    // Window
    //
    const winclass = w32.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(w32.HINSTANCE, w32.GetModuleHandleA(null)),
        .hIcon = null,
        .hCursor = w32.LoadCursorA(null, @intToPtr(w32.LPCSTR, 32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = window_name,
        .hIconSm = null,
    };
    _ = w32.RegisterClassExA(&winclass);

    const style = w32.WS_OVERLAPPEDWINDOW;
    var rect = w32.RECT{
        .left = 0,
        .top = 0,
        .right = @intCast(w32.LONG, window_width),
        .bottom = @intCast(w32.LONG, window_height),
    };
    _ = w32.AdjustWindowRectEx(&rect, style, w32.FALSE, 0);

    const window = w32.CreateWindowExA(
        0,
        window_name,
        window_name,
        style + w32.WS_VISIBLE,
        w32.CW_USEDEFAULT,
        w32.CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    ).?;

    std.log.info("Application window created", .{});

    //
    // DXGI Factory
    //
    const dxgi_factory = dxgi_factory: {
        var factory: ?*dxgi.IFactory6 = undefined;
        hrPanicOnFail(dxgi.CreateDXGIFactory2(
            0,
            &dxgi.IID_IFactory6,
            @ptrCast(*?*anyopaque, &factory),
        ));
        break :dxgi_factory factory.?;
    };
    defer _ = dxgi_factory.Release();

    std.log.info("DXGI factory created", .{});

    //
    // D3D12 Device
    //
    const device = blk: {
        var device: ?*d3d12.IDevice = null;
        const hr = d3d12.CreateDevice(
            null,
            .@"11_0",
            &d3d12.IID_IDevice,
            @ptrCast(?*?*anyopaque, &device),
        );
        if (hr != w32.S_OK) {
            _ = w32.MessageBoxA(
                window,
                "Failed to create Direct3D 12 Device. This applications requires graphics card " ++
                    "with DirectX 12 Feature Level 11.0 support.",
                "Your graphics card driver may be old",
                w32.MB_OK | w32.MB_ICONERROR,
            );
            w32.ExitProcess(0);
        }
        break :blk device.?;
    };
    defer _ = device.Release();

    std.log.info("D3D12 device created", .{});

    //
    // Command Queue
    //
    const command_queue = command_queue: {
        var cmdqueue: ?*d3d12.ICommandQueue = null;
        hrPanicOnFail(device.CreateCommandQueue(&.{
            .Type = .DIRECT,
            .Priority = @enumToInt(d3d12.COMMAND_QUEUE_PRIORITY.NORMAL),
            .Flags = .{},
            .NodeMask = 0,
        }, &d3d12.IID_ICommandQueue, @ptrCast(*?*anyopaque, &cmdqueue)));
        break :command_queue cmdqueue.?;
    };
    defer _ = command_queue.Release();

    std.log.info("D3D12 command queue created", .{});

    //
    // Swap Chain
    //
    const swap_chain = swap_chain: {
        var swap_chain: *dxgi.ISwapChain = undefined;
        hrPanicOnFail(dxgi_factory.CreateSwapChain(
            @ptrCast(*w32.IUnknown, command_queue),
            &dxgi.SWAP_CHAIN_DESC{
                .BufferDesc = .{
                    .Width = window_width,
                    .Height = window_height,
                    .RefreshRate = .{ .Numerator = 0, .Denominator = 0 },
                    .Format = .R8G8B8A8_UNORM,
                    .ScanlineOrdering = .UNSPECIFIED,
                    .Scaling = .STRETCHED,
                },
                .SampleDesc = .{ .Count = 1, .Quality = 0 },
                .BufferUsage = .{ .RENDER_TARGET_OUTPUT = true },
                .BufferCount = 2,
                .OutputWindow = window,
                .Windowed = w32.TRUE,
                .SwapEffect = .FLIP_DISCARD,
                .Flags = .{},
            },
            @ptrCast(*?*dxgi.ISwapChain, &swap_chain),
        ));
        defer _ = swap_chain.Release();

        var swap_chain3: ?*dxgi.ISwapChain3 = null;
        hrPanicOnFail(swap_chain.QueryInterface(
            &dxgi.IID_ISwapChain3,
            @ptrCast(*?*anyopaque, &swap_chain3),
        ));
        break :swap_chain swap_chain3.?;
    };
    defer _ = swap_chain.Release();

    var swap_chain_textures: [2]*d3d12.IResource = undefined;

    hrPanicOnFail(swap_chain.GetBuffer(
        0,
        &d3d12.IID_IResource,
        @ptrCast(*?*anyopaque, &swap_chain_textures[0]),
    ));
    defer _ = swap_chain_textures[0].Release();

    hrPanicOnFail(swap_chain.GetBuffer(
        1,
        &d3d12.IID_IResource,
        @ptrCast(*?*anyopaque, &swap_chain_textures[1]),
    ));
    defer _ = swap_chain_textures[1].Release();

    std.log.info("Swap chain created", .{});

    //
    // Render Target View Heap
    //
    const rtv_heap = rtv_heap: {
        var heap: ?*d3d12.IDescriptorHeap = null;
        hrPanicOnFail(device.CreateDescriptorHeap(&.{
            .Type = .RTV,
            .NumDescriptors = 16,
            .Flags = .{},
            .NodeMask = 0,
        }, &d3d12.IID_IDescriptorHeap, @ptrCast(*?*anyopaque, &heap)));
        break :rtv_heap heap.?;
    };
    defer _ = rtv_heap.Release();

    const rtv_heap_start = rtv_heap.GetCPUDescriptorHandleForHeapStart();

    device.CreateRenderTargetView(swap_chain_textures[0], null, rtv_heap_start);
    device.CreateRenderTargetView(
        swap_chain_textures[1],
        null,
        .{ .ptr = rtv_heap_start.ptr + device.GetDescriptorHandleIncrementSize(.RTV) },
    );

    std.log.info("Render target view (RTV) heap created ", .{});

    //
    // Frame Fence
    //
    const frame_fence = frame_fence: {
        var frame_fence: *d3d12.IFence = undefined;
        hrPanicOnFail(device.CreateFence(0, .{}, &d3d12.IID_IFence, @ptrCast(*?*anyopaque, &frame_fence)));
        break :frame_fence frame_fence;
    };
    defer _ = frame_fence.Release();

    const frame_fence_event = w32.CreateEventExA(null, "frame_fence_event", 0, w32.EVENT_ALL_ACCESS).?;
    defer _ = w32.CloseHandle(frame_fence_event);

    std.log.info("Frame fence created ", .{});

    //
    // Command Allocators
    //
    var command_allocators: [2]*d3d12.ICommandAllocator = undefined;

    hrPanicOnFail(device.CreateCommandAllocator(
        .DIRECT,
        &d3d12.IID_ICommandAllocator,
        @ptrCast(*?*anyopaque, &command_allocators[0]),
    ));
    defer _ = command_allocators[0].Release();

    hrPanicOnFail(device.CreateCommandAllocator(
        .DIRECT,
        &d3d12.IID_ICommandAllocator,
        @ptrCast(*?*anyopaque, &command_allocators[1]),
    ));
    defer _ = command_allocators[1].Release();

    std.log.info("Command allocators created ", .{});

    //
    // Command List
    //
    const command_list = command_list: {
        var cmdlist: ?*d3d12.IGraphicsCommandList = null;
        hrPanicOnFail(device.CreateCommandList(
            0,
            .DIRECT,
            command_allocators[0],
            null,
            &d3d12.IID_IGraphicsCommandList,
            @ptrCast(*?*anyopaque, &cmdlist),
        ));
        break :command_list cmdlist.?;
    };
    defer _ = command_list.Release();
    hrPanicOnFail(command_list.Close());

    std.log.info("Command list created ", .{});

    var frame_index: u32 = 0;
    var frame_fence_counter: u64 = 0;

    var frac: f32 = 0.0;
    var frac_delta: f32 = 0.005;

    //
    // Main Loop
    //
    main_loop: while (true) {
        {
            var message = std.mem.zeroes(w32.MSG);
            while (w32.PeekMessageA(&message, null, 0, 0, w32.PM_REMOVE) == w32.TRUE) {
                _ = w32.TranslateMessage(&message);
                _ = w32.DispatchMessageA(&message);
                if (message.message == w32.WM_QUIT) {
                    break :main_loop;
                }
            }
        }

        const command_allocator = command_allocators[frame_index];

        hrPanicOnFail(command_allocator.Reset());
        hrPanicOnFail(command_list.Reset(command_allocator, null));

        _ = w32.GetClientRect(window, &rect);

        command_list.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
            .TopLeftX = 0.0,
            .TopLeftY = 0.0,
            .Width = @intToFloat(f32, rect.right),
            .Height = @intToFloat(f32, rect.bottom),
            .MinDepth = 0.0,
            .MaxDepth = 1.0,
        }});
        command_list.RSSetScissorRects(1, &[_]d3d12.RECT{.{
            .left = 0,
            .top = 0,
            .right = @intCast(c_long, rect.right),
            .bottom = @intCast(c_long, rect.bottom),
        }});

        const back_buffer_index = swap_chain.GetCurrentBackBufferIndex();
        const back_buffer_descriptor = d3d12.CPU_DESCRIPTOR_HANDLE{
            .ptr = rtv_heap_start.ptr + back_buffer_index * device.GetDescriptorHandleIncrementSize(.RTV),
        };

        command_list.ResourceBarrier(1, &[_]d3d12.RESOURCE_BARRIER{.{
            .Type = .TRANSITION,
            .Flags = .{},
            .u = .{
                .Transition = .{
                    .pResource = swap_chain_textures[back_buffer_index],
                    .Subresource = d3d12.RESOURCE_BARRIER_ALL_SUBRESOURCES,
                    .StateBefore = d3d12.RESOURCE_STATES.PRESENT,
                    .StateAfter = .{ .RENDER_TARGET = true },
                },
            },
        }});

        command_list.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer_descriptor},
            w32.TRUE,
            null,
        );
        command_list.ClearRenderTargetView(back_buffer_descriptor, &.{ 0.2, frac, 0.8, 1.0 }, 0, null);

        command_list.ResourceBarrier(1, &[_]d3d12.RESOURCE_BARRIER{.{
            .Type = .TRANSITION,
            .Flags = .{},
            .u = .{
                .Transition = .{
                    .pResource = swap_chain_textures[back_buffer_index],
                    .Subresource = d3d12.RESOURCE_BARRIER_ALL_SUBRESOURCES,
                    .StateBefore = .{ .RENDER_TARGET = true },
                    .StateAfter = d3d12.RESOURCE_STATES.PRESENT,
                },
            },
        }});
        hrPanicOnFail(command_list.Close());

        command_queue.ExecuteCommandLists(
            1,
            &[_]*d3d12.ICommandList{@ptrCast(*d3d12.ICommandList, command_list)},
        );

        frame_fence_counter += 1;
        hrPanicOnFail(swap_chain.Present(1, .{}));
        hrPanicOnFail(command_queue.Signal(frame_fence, frame_fence_counter));

        const gpu_frame_counter = frame_fence.GetCompletedValue();
        if ((frame_fence_counter - gpu_frame_counter) >= 2) {
            hrPanicOnFail(frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, frame_fence_event));
            _ = w32.WaitForSingleObject(frame_fence_event, w32.INFINITE);
        }

        frame_index = (frame_index + 1) % 2;

        frac += frac_delta;
        if (frac > 1.0 or frac < 0.0) {
            frac_delta = -frac_delta;
        }
    }

    frame_fence_counter += 1;
    hrPanicOnFail(command_queue.Signal(frame_fence, frame_fence_counter));
    hrPanicOnFail(frame_fence.SetEventOnCompletion(frame_fence_counter, frame_fence_event));
    _ = w32.WaitForSingleObject(frame_fence_event, w32.INFINITE);
}
