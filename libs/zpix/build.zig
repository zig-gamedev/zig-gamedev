const std = @import("std");
const zwin32 = @import("../zwin32/build.zig");

pub fn getPkg(b: *std.build.Builder, options_pkg: std.build.Pkg) std.build.Pkg {
    const pkg = std.build.Pkg{
        .name = "zpix",
        .path = .{ .path = thisDir() ++ "/src/zpix.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32.pkg,
            options_pkg,
        },
    };
    return b.dupePkg(pkg);
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
