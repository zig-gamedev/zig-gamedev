pub const c_import_bullet = @import("build_options").c_import_bullet;

pub usingnamespace @cImport({
    @cDefine("CIMGUI_DEFINE_ENUMS_AND_STRUCTS", "");
    @cDefine("CIMGUI_NO_EXPORT", "");
    @cInclude("cimgui/cimgui.h");
    @cInclude("cgltf.h");
    @cInclude("stb_perlin.h");
    @cInclude("stb_image.h");
    if (c_import_bullet) {
        @cInclude("bullet.h");
    }
});
