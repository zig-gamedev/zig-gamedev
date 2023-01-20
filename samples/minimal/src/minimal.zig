const std = @import("std");
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const dxgi = zwin32.dxgi;
const d3d12 = zwin32.d3d12;
const d3d12d = zwin32.d3d12d;
const hrPanicOnFail = zwin32.hrPanicOnFail;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

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

fn createWindow() w32.HWND {
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

    //const style = w32.WS_OVERLAPPEDWINDOW;
    const style = w32.WS_OVERLAPPED + w32.WS_SYSMENU + w32.WS_CAPTION + w32.WS_MINIMIZEBOX;

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

    return window;
}

pub fn main() !void {
    _ = w32.CoInitializeEx(null, w32.COINIT_MULTITHREADED);
    defer w32.CoUninitialize();

    _ = w32.SetProcessDPIAware();

    var dx12 = Dx12State.init(createWindow());
    defer dx12.deinit();

    var root_signature: *d3d12.IRootSignature = undefined;
    var pipeline: *d3d12.IPipelineState = undefined;
    {
        const vs_cso = @embedFile("./minimal.vs.cso");
        const ps_cso = @embedFile("./minimal.ps.cso");

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.VS = .{ .pShaderBytecode = vs_cso, .BytecodeLength = vs_cso.len };
        pso_desc.PS = .{ .pShaderBytecode = ps_cso, .BytecodeLength = ps_cso.len };

        hrPanicOnFail(dx12.device.CreateRootSignature(
            0,
            pso_desc.VS.pShaderBytecode.?,
            pso_desc.VS.BytecodeLength,
            &d3d12.IID_IRootSignature,
            @ptrCast(*?*anyopaque, &root_signature),
        ));

        hrPanicOnFail(dx12.device.CreateGraphicsPipelineState(
            &pso_desc,
            &d3d12.IID_IPipelineState,
            @ptrCast(*?*anyopaque, &pipeline),
        ));
    }
    defer _ = pipeline.Release();
    defer _ = root_signature.Release();

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

        const command_allocator = dx12.command_allocators[dx12.frame_index];

        hrPanicOnFail(command_allocator.Reset());
        hrPanicOnFail(dx12.command_list.Reset(command_allocator, null));

        dx12.command_list.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
            .TopLeftX = 0.0,
            .TopLeftY = 0.0,
            .Width = @intToFloat(f32, window_width),
            .Height = @intToFloat(f32, window_height),
            .MinDepth = 0.0,
            .MaxDepth = 1.0,
        }});
        dx12.command_list.RSSetScissorRects(1, &[_]d3d12.RECT{.{
            .left = 0,
            .top = 0,
            .right = @intCast(c_long, window_width),
            .bottom = @intCast(c_long, window_height),
        }});

        const back_buffer_index = dx12.swap_chain.GetCurrentBackBufferIndex();
        const back_buffer_descriptor = d3d12.CPU_DESCRIPTOR_HANDLE{
            .ptr = dx12.rtv_heap_start.ptr +
                back_buffer_index * dx12.device.GetDescriptorHandleIncrementSize(.RTV),
        };

        dx12.command_list.ResourceBarrier(1, &[_]d3d12.RESOURCE_BARRIER{.{
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
        }});

        dx12.command_list.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer_descriptor},
            w32.TRUE,
            null,
        );
        dx12.command_list.ClearRenderTargetView(back_buffer_descriptor, &.{ 0.2, frac, 0.8, 1.0 }, 0, null);

        dx12.command_list.IASetPrimitiveTopology(.TRIANGLELIST);
        dx12.command_list.SetPipelineState(pipeline);
        dx12.command_list.SetGraphicsRootSignature(root_signature);
        dx12.command_list.DrawInstanced(3, 1, 0, 0);

        dx12.command_list.ResourceBarrier(1, &[_]d3d12.RESOURCE_BARRIER{.{
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
        }});
        hrPanicOnFail(dx12.command_list.Close());

        dx12.command_queue.ExecuteCommandLists(
            1,
            &[_]*d3d12.ICommandList{@ptrCast(*d3d12.ICommandList, dx12.command_list)},
        );

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
    device: *d3d12.IDevice,

    swap_chain: *dxgi.ISwapChain3,
    swap_chain_textures: [2]*d3d12.IResource,

    rtv_heap: *d3d12.IDescriptorHeap,
    rtv_heap_start: d3d12.CPU_DESCRIPTOR_HANDLE,

    frame_fence: *d3d12.IFence,
    frame_fence_event: w32.HANDLE,
    frame_fence_counter: u64 = 0,
    frame_index: u32 = 0,

    command_queue: *d3d12.ICommandQueue,
    command_allocators: [2]*d3d12.ICommandAllocator,
    command_list: *d3d12.IGraphicsCommandList,

    fn init(window: w32.HWND) Dx12State {
        //
        // DXGI Factory
        //
        var dxgi_factory: *dxgi.IFactory6 = undefined;
        hrPanicOnFail(dxgi.CreateDXGIFactory2(
            0,
            &dxgi.IID_IFactory6,
            @ptrCast(*?*anyopaque, &dxgi_factory),
        ));

        std.log.info("DXGI factory created", .{});

        {
            var maybe_debug: ?*d3d12d.IDebug1 = null;
            _ = d3d12.GetDebugInterface(&d3d12d.IID_IDebug1, @ptrCast(*?*anyopaque, &maybe_debug));
            if (maybe_debug) |debug| {
                // Uncomment below line to enable debug layer
                //debug.EnableDebugLayer();
                _ = debug.Release();
            }
        }

        //
        // D3D12 Device
        //
        var device: *d3d12.IDevice = undefined;
        if (d3d12.CreateDevice(
            null,
            .@"11_0",
            &d3d12.IID_IDevice,
            @ptrCast(?*?*anyopaque, &device),
        ) != w32.S_OK) {
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
            .Priority = @enumToInt(d3d12.COMMAND_QUEUE_PRIORITY.NORMAL),
            .Flags = .{},
            .NodeMask = 0,
        }, &d3d12.IID_ICommandQueue, @ptrCast(*?*anyopaque, &command_queue)));

        std.log.info("D3D12 command queue created", .{});

        //
        // Swap Chain
        //
        var swap_chain: *dxgi.ISwapChain3 = undefined;
        {
            var temp_swap_chain: *dxgi.ISwapChain = undefined;
            hrPanicOnFail(dxgi_factory.CreateSwapChain(
                @ptrCast(*w32.IUnknown, command_queue),
                &dxgi.SWAP_CHAIN_DESC{
                    .BufferDesc = .{
                        .Width = window_width,
                        .Height = window_height,
                        .RefreshRate = .{ .Numerator = 0, .Denominator = 0 },
                        .Format = .R8G8B8A8_UNORM,
                        .ScanlineOrdering = .UNSPECIFIED,
                        .Scaling = .UNSPECIFIED,
                    },
                    .SampleDesc = .{ .Count = 1, .Quality = 0 },
                    .BufferUsage = .{ .RENDER_TARGET_OUTPUT = true },
                    .BufferCount = 2,
                    .OutputWindow = window,
                    .Windowed = w32.TRUE,
                    .SwapEffect = .FLIP_DISCARD,
                    .Flags = .{},
                },
                @ptrCast(*?*dxgi.ISwapChain, &temp_swap_chain),
            ));
            defer _ = temp_swap_chain.Release();

            hrPanicOnFail(temp_swap_chain.QueryInterface(
                &dxgi.IID_ISwapChain3,
                @ptrCast(*?*anyopaque, &swap_chain),
            ));
        }

        // Disable ALT + ENTER
        hrPanicOnFail(dxgi_factory.MakeWindowAssociation(window, .{ .NO_WINDOW_CHANGES = true }));

        var swap_chain_textures: [2]*d3d12.IResource = undefined;

        hrPanicOnFail(swap_chain.GetBuffer(
            0,
            &d3d12.IID_IResource,
            @ptrCast(*?*anyopaque, &swap_chain_textures[0]),
        ));
        hrPanicOnFail(swap_chain.GetBuffer(
            1,
            &d3d12.IID_IResource,
            @ptrCast(*?*anyopaque, &swap_chain_textures[1]),
        ));

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
        }, &d3d12.IID_IDescriptorHeap, @ptrCast(*?*anyopaque, &rtv_heap)));

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
        var frame_fence: *d3d12.IFence = undefined;
        hrPanicOnFail(device.CreateFence(0, .{}, &d3d12.IID_IFence, @ptrCast(*?*anyopaque, &frame_fence)));

        var frame_fence_event = w32.CreateEventExA(null, "frame_fence_event", 0, w32.EVENT_ALL_ACCESS).?;

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
        hrPanicOnFail(device.CreateCommandAllocator(
            .DIRECT,
            &d3d12.IID_ICommandAllocator,
            @ptrCast(*?*anyopaque, &command_allocators[1]),
        ));

        std.log.info("Command allocators created ", .{});

        //
        // Command List
        //
        var command_list: *d3d12.IGraphicsCommandList = undefined;
        hrPanicOnFail(device.CreateCommandList(
            0,
            .DIRECT,
            command_allocators[0],
            null,
            &d3d12.IID_IGraphicsCommandList,
            @ptrCast(*?*anyopaque, &command_list),
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
        _ = dx12.command_allocators[0].Release();
        _ = dx12.command_allocators[1].Release();
        _ = dx12.frame_fence.Release();
        _ = w32.CloseHandle(dx12.frame_fence_event);
        _ = dx12.rtv_heap.Release();
        _ = dx12.swap_chain_textures[0].Release();
        _ = dx12.swap_chain_textures[1].Release();
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
        if ((dx12.frame_fence_counter - gpu_frame_counter) >= 2) {
            hrPanicOnFail(dx12.frame_fence.SetEventOnCompletion(gpu_frame_counter + 1, dx12.frame_fence_event));
            _ = w32.WaitForSingleObject(dx12.frame_fence_event, w32.INFINITE);
        }

        dx12.frame_index = (dx12.frame_index + 1) % 2;
    }

    fn finishGpuCommands(dx12: *Dx12State) void {
        dx12.frame_fence_counter += 1;
        hrPanicOnFail(dx12.command_queue.Signal(dx12.frame_fence, dx12.frame_fence_counter));
        hrPanicOnFail(dx12.frame_fence.SetEventOnCompletion(dx12.frame_fence_counter, dx12.frame_fence_event));
        _ = w32.WaitForSingleObject(dx12.frame_fence_event, w32.INFINITE);
    }
};
