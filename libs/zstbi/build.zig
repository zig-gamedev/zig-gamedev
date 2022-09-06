const std = @import("std");

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addCSourceFile(thisDir() ++ "/libs/stbi/stb_image.c", &.{"-std=c99"});
}

pub const pkg = std.build.Pkg{
    .name = "zstbi",
    .source = .{ .path = thisDir() ++ "/src/zstbi.zig" },
};

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
