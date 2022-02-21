const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_pix", options.enable_pix);
    exe_options.addOption(bool, "enable_dx_debug", options.enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", options.enable_dx_gpu_debug);
    exe_options.addOption(bool, "enable_tracy", options.tracy != null);

    const exe = b.addExecutable("vector_graphics_test", thisDir() ++ "/src/vector_graphics_test.zig");
    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);
    exe.addOptions("build_options", exe_options);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;
    exe.want_lto = false;

    const options_pkg = std.build.Pkg{
        .name = "build_options",
        .path = exe_options.getSource(),
    };

    const zwin32_pkg = std.build.Pkg{
        .name = "zwin32",
        .path = .{ .path = thisDir() ++ "/../../libs/zwin32/zwin32.zig" },
    };
    exe.addPackage(zwin32_pkg);

    const ztracy_pkg = std.build.Pkg{
        .name = "ztracy",
        .path = .{ .path = thisDir() ++ "/../../libs/ztracy/src/ztracy.zig" },
        .dependencies = &[_]std.build.Pkg{options_pkg},
    };
    exe.addPackage(ztracy_pkg);
    @import("../../libs/ztracy/build.zig").link(b, exe, .{ .tracy_path = options.tracy });

    const zd3d12_pkg = std.build.Pkg{
        .name = "zd3d12",
        .path = .{ .path = thisDir() ++ "/../../libs/zd3d12/src/zd3d12.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32_pkg,
            ztracy_pkg,
            options_pkg,
        },
    };
    exe.addPackage(zd3d12_pkg);
    @import("../../libs/zd3d12/build.zig").link(b, exe);

    const common_pkg = std.build.Pkg{
        .name = "common",
        .path = .{ .path = thisDir() ++ "/../../libs/common/src/common.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32_pkg,
            zd3d12_pkg,
            ztracy_pkg,
            options_pkg,
        },
    };
    exe.addPackage(common_pkg);
    @import("../../libs/common/build.zig").link(b, exe);

    return exe;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
