const std = @import("std");
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const dxgi = zwin32.dxgi;
const d3d12 = zwin32.d3d12;
const d3d12d = zwin32.d3d12d;
const hrPanicOnFail = zwin32.hrPanicOnFail;

pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: minimal d3d12";

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
        w32.WM_GETMINMAXINFO => {
            var info: *w32.MINMAXINFO = @ptrFromInt(@as(usize, @intCast(lparam)));
            info.ptMinTrackSize.x = 400;
            info.ptMinTrackSize.y = 400;
            return 0;
        },
        w32.WM_DESTROY => {
            w32.PostQuitMessage(0);
            return 0;
        },
        else => {},
    }
    return w32.DefWindowProcA(window, message, wparam, lparam);
}

fn createWindow(width: u32, height: u32) w32.HWND {
    const winclass = w32.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(w32.GetModuleHandleA(null)),
        .hIcon = null,
        .hCursor = w32.LoadCursorA(null, @ptrFromInt(32512)),
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
        .right = @intCast(width),
        .bottom = @intCast(height),
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

    return window;
}

pub fn main() !void {
    _ = w32.CoInitializeEx(null, w32.COINIT_MULTITHREADED);
    defer w32.CoUninitialize();

    _ = w32.SetProcessDPIAware();

    const window = createWindow(1600, 1200);

    var dx12 = Dx12State.init(window);
    defer dx12.deinit();

    const root_signature: *d3d12.IRootSignature, const pipeline: *d3d12.IPipelineState = blk: {
        const vs_cso = @embedFile("./minimal_d3d12.vs.cso");
        const ps_cso = @embedFile("./minimal_d3d12.ps.cso");

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.VS = .{ .pShaderBytecode = vs_cso, .BytecodeLength = vs_cso.len };
        pso_desc.PS = .{ .pShaderBytecode = ps_cso, .BytecodeLength = ps_cso.len };

        var root_signature: *d3d12.IRootSignature = undefined;
        hrPanicOnFail(dx12.device.CreateRootSignature(
            0,
            pso_desc.VS.pShaderBytecode.?,
            pso_desc.VS.BytecodeLength,
            &d3d12.IID_IRootSignature,
            @ptrCast(&root_signature),
        ));

        var pipeline: *d3d12.IPipelineState = undefined;
        hrPanicOnFail(dx12.device.CreateGraphicsPipelineState(
            &pso_desc,
            &d3d12.IID_IPipelineState,
            @ptrCast(&pipeline),
        ));

        break :blk .{ root_signature, pipeline };
    };
    defer _ = pipeline.Release();
    defer _ = root_signature.Release();

    var frac: f32 = 0.0;
    var frac_delta: f32 = 0.005;

    var window_rect: w32.RECT = undefined;
    _ = w32.GetClientRect(window, &window_rect);

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

            var rect: w32.RECT = undefined;
            _ = w32.GetClientRect(window, &rect);
            if (rect.right == 0 and rect.bottom == 0) {
                // Window is minimized
                w32.Sleep(10);
                continue :main_loop;
            }

            if (rect.right != window_rect.right or rect.bottom != window_rect.bottom) {
                rect.right = @max(1, rect.right);
                rect.bottom = @max(1, rect.bottom);
                std.log.info("Window resized to {d}x{d}", .{ rect.right, rect.bottom });

                dx12.finishGpuCommands();

                for (dx12.swap_chain_textures) |texture| _ = texture.Release();

                hrPanicOnFail(dx12.swap_chain.ResizeBuffers(0, 0, 0, .UNKNOWN, .{}));

                for (&dx12.swap_chain_textures, 0..) |*texture, i| {
                    hrPanicOnFail(dx12.swap_chain.GetBuffer(
                        @intCast(i),
                        &d3d12.IID_IResource,
                        @ptrCast(&texture.*),
                    ));
                }

                for (dx12.swap_chain_textures, 0..) |texture, i| {
                    dx12.device.CreateRenderTargetView(
                        texture,
                        null,
                        .{ .ptr = dx12.rtv_heap_start.ptr +
                            i * dx12.device.GetDescriptorHandleIncrementSize(.RTV) },
                    );
                }
            }
            window_rect = rect;
        }

        const command_allocator = dx12.command_allocators[dx12.frame_index];

        hrPanicOnFail(command_allocator.Reset());
        hrPanicOnFail(dx12.command_list.Reset(command_allocator, null));

        dx12.command_list.RSSetViewports(1, &.{
            .{
                .TopLeftX = 0.0,
                .TopLeftY = 0.0,
                .Width = @floatFromInt(window_rect.right),
                .Height = @floatFromInt(window_rect.bottom),
                .MinDepth = 0.0,
                .MaxDepth = 1.0,
            },
        });
        dx12.command_list.RSSetScissorRects(1, &.{
            .{
                .left = 0,
                .top = 0,
                .right = @intCast(window_rect.right),
                .bottom = @intCast(window_rect.bottom),
            },
        });

        const back_buffer_index = dx12.swap_chain.GetCurrentBackBufferIndex();
        const back_buffer_descriptor = d3d12.CPU_DESCRIPTOR_HANDLE{
            .ptr = dx12.rtv_heap_start.ptr +
                back_buffer_index * dx12.device.GetDescriptorHandleIncrementSize(.RTV),
        };

        dx12.command_list.ResourceBarrier(1, &.{
            .{
                .Type = .TRANSITION,
                .Flags = .{},
                .u = .{
                    .Transition = .{
                        .pResource = dx12.swap_chain_textures[back_buffer_index],
                        .Subresource = d3d12.RESOURCE_BARRIER_ALL_SUBRESOURCES,
                        .StateBefore = d3d12.RESOURCE_STATES.PRESENT,
                        .StateAfter = .{ .RENDER_TARGET = true },
                    },
                },
            },
        });

        dx12.command_list.OMSetRenderTargets(
            1,
            &.{back_buffer_descriptor},
            w32.TRUE,
            null,
        );
        dx12.command_list.ClearRenderTargetView(back_buffer_descriptor, &.{ 0.2, frac, 0.8, 1.0 }, 0, null);

        dx12.command_list.IASetPrimitiveTopology(.TRIANGLELIST);
        dx12.command_list.SetPipelineState(pipeline);
        dx12.command_list.SetGraphicsRootSignature(root_signature);
        dx12.command_list.DrawInstanced(3, 1, 0, 0);

        dx12.command_list.ResourceBarrier(1, &.{
            .{
                .Type = .TRANSITION,
                .Flags = .{},
                .u = .{
                    .Transition = .{
                        .pResource = dx12.swap_chain_textures[back_buffer_index],
                        .Subresource = d3d12.RESOURCE_BARRIER_ALL_SUBRESOURCES,
                        .StateBefore = .{ .RENDER_TARGET = true },
                        .StateAfter = d3d12.RESOURCE_STATES.PRESENT,
                    },
                },
            },
        });
        hrPanicOnFail(dx12.command_list.Close());

        dx12.command_queue.ExecuteCommandLists(1, &.{@ptrCast(dx12.command_list)});

        dx12.present();

        frac += frac_delta;
        if (frac > 1.0 or frac < 0.0) {
            frac_delta = -frac_delta;
        }
    }

    dx12.finishGpuCommands();
}

const Dx12State = struct {
    dxgi_factory: *dxgi.IFactory6,
    device: *d3d12.IDevice9,

    swap_chain: *dxgi.ISwapChain3,
    swap_chain_textures: [num_frames]*d3d12.IResource,

    rtv_heap: *d3d12.IDescriptorHeap,
    rtv_heap_start: d3d12.CPU_DESCRIPTOR_HANDLE,

    frame_fence: *d3d12.IFence,
    frame_fence_event: w32.HANDLE,
    frame_fence_counter: u64 = 0,
    frame_index: u32 = 0,

    command_queue: *d3d12.ICommandQueue,
    command_allocators: [num_frames]*d3d12.ICommandAllocator,
    command_list: *d3d12.IGraphicsCommandList6,

    const num_frames = 2;

    fn init(window: w32.HWND) Dx12State {
        //
        // DXGI Factory
        //
        var dxgi_factory: *dxgi.IFactory6 = undefined;
        hrPanicOnFail(dxgi.CreateDXGIFactory2(
            0,
            &dxgi.IID_IFactory6,
            @ptrCast(&dxgi_factory),
        ));

        std.log.info("DXGI factory created", .{});

        {
            var maybe_debug: ?*d3d12d.IDebug1 = null;
            _ = d3d12.GetDebugInterface(&d3d12d.IID_IDebug1, @ptrCast(&maybe_debug));
            if (maybe_debug) |debug| {
                // Uncomment below line to enable debug layer
                //debug.EnableDebugLayer();
                _ = debug.Release();
            }
        }

        //
        // D3D12 Device
        //
        var device: *d3d12.IDevice9 = undefined;
        if (d3d12.CreateDevice(null, .@"11_0", &d3d12.IID_IDevice9, @ptrCast(&device)) != w32.S_OK) {
            _ = w32.MessageBoxA(
                window,
                "Failed to create Direct3D 12 Device. This applications requires graphics card " ++
                    "with DirectX 12 Feature Level 11.0 support.",
                "Your graphics card driver may be old",
                w32.MB_OK | w32.MB_ICONERROR,
            );
            w32.ExitProcess(0);
        }

        std.log.info("D3D12 device created", .{});

        //
        // Command Queue
        //
        var command_queue: *d3d12.ICommandQueue = undefined;
        hrPanicOnFail(device.CreateCommandQueue(&.{
            .Type = .DIRECT,
            .Priority = @intFromEnum(d3d12.COMMAND_QUEUE_PRIORITY.NORMAL),
            .Flags = .{},
            .NodeMask = 0,
        }, &d3d12.IID_ICommandQueue, @ptrCast(&command_queue)));

        std.log.info("D3D12 command queue created", .{});

        //
        // Swap Chain
        //
        var rect: w32.RECT = undefined;
        _ = w32.GetClientRect(window, &rect);

        var swap_chain: *dxgi.ISwapChain3 = undefined;
        {
            var desc = dxgi.SWAP_CHAIN_DESC{
                .BufferDesc = .{
                    .Width = @intCast(rect.right),
                    .Height = @intCast(rect.bottom),
                    .RefreshRate = .{ .Numerator = 0, .Denominator = 0 },
                    .Format = .R8G8B8A8_UNORM,
                    .ScanlineOrdering = .UNSPECIFIED,
                    .Scaling = .UNSPECIFIED,
                },
                .SampleDesc = .{ .Count = 1, .Quality = 0 },
                .BufferUsage = .{ .RENDER_TARGET_OUTPUT = true },
                .BufferCount = num_frames,
                .OutputWindow = window,
                .Windowed = w32.TRUE,
                .SwapEffect = .FLIP_DISCARD,
                .Flags = .{},
            };
            var temp_swap_chain: *dxgi.ISwapChain = undefined;
            hrPanicOnFail(dxgi_factory.CreateSwapChain(
                @ptrCast(command_queue),
                &desc,
                @ptrCast(&temp_swap_chain),
            ));
            defer _ = temp_swap_chain.Release();

            hrPanicOnFail(temp_swap_chain.QueryInterface(&dxgi.IID_ISwapChain3, @ptrCast(&swap_chain)));
        }

        // Disable ALT + ENTER
        hrPanicOnFail(dxgi_factory.MakeWindowAssociation(window, .{ .NO_WINDOW_CHANGES = true }));

        var swap_chain_textures: [num_frames]*d3d12.IResource = undefined;

        for (&swap_chain_textures, 0..) |*texture, i| {
            hrPanicOnFail(swap_chain.GetBuffer(@intCast(i), &d3d12.IID_IResource, @ptrCast(&texture.*)));
        }

        std.log.info("Swap chain created", .{});

        //
        // Render Target View Heap
        //
        var rtv_heap: *d3d12.IDescriptorHeap = undefined;
        hrPanicOnFail(device.CreateDescriptorHeap(&.{
            .Type = .RTV,
            .NumDescriptors = 16,
            .Flags = .{},
            .NodeMask = 0,
        }, &d3d12.IID_IDescriptorHeap, @ptrCast(&rtv_heap)));

        const rtv_heap_start = rtv_heap.GetCPUDescriptorHandleForHeapStart();

        for (swap_chain_textures, 0..) |texture, i| {
            device.CreateRenderTargetView(
                texture,
                null,
                .{ .ptr = rtv_heap_start.ptr + i * device.GetDescriptorHandleIncrementSize(.RTV) },
            );
        }

        std.log.info("Render target view (RTV) heap created ", .{});

        //
        // Frame Fence
        //
        var frame_fence: *d3d12.IFence = undefined;
        hrPanicOnFail(device.CreateFence(0, .{}, &d3d12.IID_IFence, @ptrCast(&frame_fence)));

        const frame_fence_event = w32.CreateEventExA(null, "frame_fence_event", 0, w32.EVENT_ALL_ACCESS).?;

        std.log.info("Frame fence created ", .{});

        //
        // Command Allocators
        //
        var command_allocators: [num_frames]*d3d12.ICommandAllocator = undefined;

        for (&command_allocators) |*cmdalloc| {
            hrPanicOnFail(device.CreateCommandAllocator(
                .DIRECT,
                &d3d12.IID_ICommandAllocator,
                @ptrCast(&cmdalloc.*),
            ));
        }

        std.log.info("Command allocators created ", .{});

        //
        // Command List
        //
        var command_list: *d3d12.IGraphicsCommandList6 = undefined;
        hrPanicOnFail(device.CreateCommandList(
            0,
            .DIRECT,
            command_allocators[0],
            null,
            &d3d12.IID_IGraphicsCommandList6,
            @ptrCast(&command_list),
        ));
        hrPanicOnFail(command_list.Close());

        return .{
            .dxgi_factory = dxgi_factory,
            .device = device,
            .command_queue = command_queue,
            .swap_chain = swap_chain,
            .swap_chain_textures = swap_chain_textures,
            .rtv_heap = rtv_heap,
            .rtv_heap_start = rtv_heap_start,
            .frame_fence = frame_fence,
            .frame_fence_event = frame_fence_event,
            .command_allocators = command_allocators,
            .command_list = command_list,
        };
    }

    fn deinit(dx12: *Dx12State) void {
        _ = dx12.command_list.Release();
        for (dx12.command_allocators) |cmdalloc| _ = cmdalloc.Release();
        _ = dx12.frame_fence.Release();
        _ = w32.CloseHandle(dx12.frame_fence_event);
        _ = dx12.rtv_heap.Release();
        for (dx12.swap_chain_textures) |texture| _ = texture.Release();
        _ = dx12.swap_chain.Release();
        _ = dx12.command_queue.Release();
        _ = dx12.device.Release();
        _ = dx12.dxgi_factory.Release();
        dx12.* = undefined;
    }

    fn present(dx12: *Dx12State) void {
        dx12.frame_fence_counter += 1;

        hrPanicOnFail(dx12.swap_chain.Present(1, .{}));
        hrPanicOnFail(dx12.command_queue.Signal(dx12.frame_fence, dx12.frame_fence_counter));

        const gpu_frame_counter = dx12.frame_fence.GetCompletedValue();
        if ((dx12.frame_fence_counter - gpu_frame_counter) >= num_frames) {
            hrPanicOnFail(dx12.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, dx12.frame_fence_event));
            _ = w32.WaitForSingleObject(dx12.frame_fence_event, w32.INFINITE);
        }

        dx12.frame_index = (dx12.frame_index + 1) % num_frames;
    }

    fn finishGpuCommands(dx12: *Dx12State) void {
        dx12.frame_fence_counter += 1;

        hrPanicOnFail(dx12.command_queue.Signal(dx12.frame_fence, dx12.frame_fence_counter));
        hrPanicOnFail(dx12.frame_fence.SetEventOnCompletion(dx12.frame_fence_counter, dx12.frame_fence_event));

        _ = w32.WaitForSingleObject(dx12.frame_fence_event, w32.INFINITE);
    }
};
