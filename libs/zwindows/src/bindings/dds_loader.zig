// Ported from DirectXTK
// https://github.com/microsoft/DirectXTK12/blob/main/Src/DDSTextureLoader.cpp
const std = @import("std");
const assert = std.debug.assert;
const windows = @import("windows.zig");
const dxgi = @import("dxgi.zig");
const d3d12 = @import("d3d12.zig");

const DDS_HEADER_FLAGS_TEXTURE: u32 = 0x00001007; // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
const DDS_HEADER_FLAGS_MIPMAP: u32 = 0x00020000; // DDSD_MIPMAPCOUNT
const DDS_HEADER_FLAGS_VOLUME: u32 = 0x00800000; // DDSD_DEPTH
const DDS_HEADER_FLAGS_PITCH: u32 = 0x00000008; // DDSD_PITCH
const DDS_HEADER_FLAGS_LINEARSIZE: u32 = 0x00080000; // DDSD_LINEARSIZE

const DDS_HEIGHT: u32 = 0x00000002; // DDSD_HEIGHT
const DDS_WIDTH: u32 = 0x00000004; // DDSD_WIDTH

const DDS_SURFACE_FLAGS_TEXTURE: u32 = 0x00001000; // DDSCAPS_TEXTURE
const DDS_SURFACE_FLAGS_MIPMAP: u32 = 0x00400008; // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
const DDS_SURFACE_FLAGS_CUBEMAP: u32 = 0x00000008; // DDSCAPS_COMPLEX

const DDS_CUBEMAP_POSITIVEX: u32 = 0x00000600; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
const DDS_CUBEMAP_NEGATIVEX: u32 = 0x00000a00; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
const DDS_CUBEMAP_POSITIVEY: u32 = 0x00001200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
const DDS_CUBEMAP_NEGATIVEY: u32 = 0x00002200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
const DDS_CUBEMAP_POSITIVEZ: u32 = 0x00004200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
const DDS_CUBEMAP_NEGATIVEZ: u32 = 0x00008200; // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

const DDS_CUBEMAP_ALLFACES: u32 = (DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX | DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY | DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ);

const DDS_CUBEMAP: u32 = 0x00000200; // DDSCAPS2_CUBEMAP

const DDS_FLAGS_VOLUME: u32 = 0x00200000; // DDSCAPS2_VOLUME

const DDS_MISC_FLAGS2_ALPHA_MODE_MASK: u32 = 0x7;

const DDS_MAGIC: u32 = 0x20534444; // "DDS "

const DDS_FOURCC: u32 = 0x00000004; // DDPF_FOURCC
const DDS_RGB: u32 = 0x00000040; // DDPF_RGB
const DDS_RGBA: u32 = 0x00000041; // DDPF_RGB | DDPF_ALPHAPIXELS
const DDS_LUMINANCE: u32 = 0x00020000; // DDPF_LUMINANCE
const DDS_LUMINANCEA: u32 = 0x00020001; // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
const DDS_ALPHA: u32 = 0x00000002; // DDPF_ALPHA
const DDS_PAL8: u32 = 0x00000020; // DDPF_PALETTEINDEXED8
const DDS_BUMPDUDV: u32 = 0x00080000; // DDPF_BUMPDUDV

inline fn makeFourCC(ch0: u8, ch1: u8, ch2: u8, ch3: u8) u32 {
    return (@as(u32, @intCast(ch0))) | (@as(u32, @intCast(ch1)) << 8) | (@as(u32, @intCast(ch2)) << 16) | (@as(u32, @intCast(ch3)) << 24);
}

inline fn isBitMask(pixelFormat: DDS_PIXELFORMAT, r: u32, g: u32, b: u32, a: u32) bool {
    return (pixelFormat.dwRBitMask == r and pixelFormat.dwGBitMask == g and pixelFormat.dwBBitMask == b and pixelFormat.dwABitMask == a);
}

pub const DDS_ALPHA_MODE = enum(u32) {
    unknown,
    straight,
    premultiplied,
    @"opaque",
    custom,
};

pub const DDS_PIXELFORMAT = extern struct {
    dwSize: u32,
    dwFlags: u32,
    dwFourCC: u32,
    dwRGBBitCount: u32,
    dwRBitMask: u32,
    dwGBitMask: u32,
    dwBBitMask: u32,
    dwABitMask: u32,
};

pub const DDS_HEADER = extern struct {
    dwSize: u32,
    dwFlags: u32,
    dwHeight: u32,
    dwWidth: u32,
    dwPitchOrLinearSize: u32,
    dwDepth: u32, // only if DDS_HEADER_FLAGS_VOLUME is set in dwFlags
    dwMipMapCount: u32,
    dwReserved1: [11]u32,
    ddspf: DDS_PIXELFORMAT,
    dwCaps: u32,
    dwCaps2: u32,
    dwCaps3: u32,
    dwCaps4: u32,
    dwReserved2: u32,
};

pub const DDS_HEADER_DXT10 = extern struct {
    dxgiFormat: dxgi.FORMAT,
    resourceDimension: u32,
    miscFlag: u32, // see DDS_RESOURCE_MISC_FLAG
    arraySize: u32,
    miscFlags2: u32, // see DDS_MISC_FLAGS2
};

pub const DdsImageInfo = struct {
    width: u32,
    height: u32,
    depth: u32,
    array_size: u32,
    mip_map_count: u32,
    format: dxgi.FORMAT,
    alpha_mode: DDS_ALPHA_MODE,
    cubemap: bool,
    resource_dimension: d3d12.RESOURCE_DIMENSION,
};

pub const DdsError = error{
    InvalidDDSData,
    NotSupported,
    EndOfFile,
};

pub fn loadTextureFromFile(
    path: []const u8,
    arena: std.mem.Allocator,
    device: *d3d12.IDevice9,
    max_size: u32,
    resources: *std.ArrayList(d3d12.SUBRESOURCE_DATA),
) !DdsImageInfo {
    var file = std.fs.cwd().openFile(path, .{}) catch |err| {
        return err;
    };
    defer file.close();

    const metadata = try file.metadata();

    const file_size = metadata.size();
    if (file_size < @sizeOf(u32) + @sizeOf(DDS_HEADER)) {
        return DdsError.InvalidDDSData;
    }

    // Read all file
    const file_data = try arena.alloc(u8, file_size);
    const read_bytes = try file.readAll(file_data);
    if (read_bytes != file_size) {
        return DdsError.InvalidDDSData;
    }

    return loadTextureFromMemory(file_data, arena, device, max_size, resources);
}

pub fn loadTextureFromMemory(file_data: []u8, arena: std.mem.Allocator, device: *d3d12.IDevice9, max_size: u32, resources: *std.ArrayList(d3d12.SUBRESOURCE_DATA)) !DdsImageInfo {
    if (file_data.len > std.math.maxInt(u32)) {
        return DdsError.InvalidDDSData;
    }

    // Create a stream
    var stream = std.io.StreamSource{ .buffer = std.io.fixedBufferStream(file_data) };
    var reader = stream.reader();

    // Check DDS_MAGIC
    const magic = try reader.readInt(u32, .little);
    if (magic != DDS_MAGIC) {
        return DdsError.InvalidDDSData;
    }

    // Extract DDS_HEADER
    const header = try reader.readStruct(DDS_HEADER);
    if (header.dwSize != @as(u32, @intCast(@sizeOf(DDS_HEADER)))) {
        return DdsError.InvalidDDSData;
    }

    if (header.ddspf.dwSize != @as(u32, @intCast(@sizeOf(DDS_PIXELFORMAT)))) {
        return DdsError.InvalidDDSData;
    }

    // Check for DX10 Extension
    var has_dx10_extension = false;
    var dx10: DDS_HEADER_DXT10 = undefined;
    if ((header.ddspf.dwFlags & DDS_FOURCC) == DDS_FOURCC and makeFourCC('D', 'X', '1', '0') == header.ddspf.dwFourCC) {
        if (file_data.len < @sizeOf(u32) + @sizeOf(DDS_HEADER) + @sizeOf(DDS_HEADER_DXT10)) {
            return DdsError.InvalidDDSData;
        }

        has_dx10_extension = true;
        dx10 = try reader.readStruct(DDS_HEADER_DXT10);
    }

    // Check alpha mode
    var alpha_mode = DDS_ALPHA_MODE.unknown;
    if (has_dx10_extension) {
        alpha_mode = @as(DDS_ALPHA_MODE, @enumFromInt(dx10.miscFlags2 & DDS_MISC_FLAGS2_ALPHA_MODE_MASK));
    } else {
        if (makeFourCC('D', 'X', 'T', '2') == header.ddspf.dwFourCC or makeFourCC('D', 'X', 'T', '4') == header.ddspf.dwFourCC) {
            alpha_mode = .premultiplied;
        }
    }

    var data_size = file_data.len - (@sizeOf(u32) + @sizeOf(DDS_HEADER));
    if (has_dx10_extension) {
        data_size -= @sizeOf(DDS_HEADER_DXT10);
    }

    var data = try arena.alloc(u8, data_size);
    try reader.readNoEof(data);

    const width: u32 = header.dwWidth;
    var height: u32 = header.dwHeight;
    var depth: u32 = header.dwDepth;

    var resource_dimension = d3d12.RESOURCE_DIMENSION.UNKNOWN;
    var array_size: u32 = 1;
    var format = dxgi.FORMAT.UNKNOWN;
    var cubemap = false;

    var mip_count = header.dwMipMapCount;
    if (mip_count == 0) {
        mip_count = 1;
    }

    if (has_dx10_extension) {
        array_size = dx10.arraySize;
        if (array_size == 0) {
            return DdsError.InvalidDDSData;
        }

        format = try getDXGIFormatFromDX10(dx10, width, height);

        if (dx10.resourceDimension == @intFromEnum(d3d12.RESOURCE_DIMENSION.TEXTURE1D)) {
            // D3DX writes 1D textures with a fixed Height of 1
            if ((header.dwFlags & DDS_HEIGHT) == DDS_HEIGHT and height != 1) {
                return DdsError.InvalidDDSData;
            }
            height = 1;
            depth = 1;
        } else if (dx10.resourceDimension == @intFromEnum(d3d12.RESOURCE_DIMENSION.TEXTURE2D)) {
            if ((dx10.miscFlag & 0x4) == 0x4) { // RESOURCE_MISC_TEXTURECUBE
                array_size *= 6;
                cubemap = true;
            }

            depth = 1;
        } else if (dx10.resourceDimension == @intFromEnum(d3d12.RESOURCE_DIMENSION.TEXTURE3D)) {
            if ((header.dwFlags & DDS_HEADER_FLAGS_VOLUME) != DDS_HEADER_FLAGS_VOLUME) {
                return DdsError.InvalidDDSData;
            }

            if (array_size > 1) {
                std.log.debug("[DDS Loader] Volume textures are not texture arrays.", .{});
                return DdsError.InvalidDDSData;
            }
        } else if (dx10.resourceDimension == @intFromEnum(d3d12.RESOURCE_DIMENSION.BUFFER)) {
            std.log.debug("[DDS Loader] Resource dimension buffer type not supported for textures.", .{});
            return DdsError.InvalidDDSData;
        } else {
            std.log.debug("[DDS Loader] Unknown resource dimension {}", .{dx10.resourceDimension});
            return DdsError.InvalidDDSData;
        }

        resource_dimension = @as(d3d12.RESOURCE_DIMENSION, @enumFromInt(dx10.resourceDimension));
    } else {
        format = getDXGIFormat(header.ddspf);

        if (format == .UNKNOWN) {
            std.log.debug("[DDS Loader] Legacy DDS formats are not supported. Consider using DirectXTex.", .{});
            return DdsError.InvalidDDSData;
        }

        if ((header.dwFlags & DDS_HEADER_FLAGS_VOLUME) == DDS_HEADER_FLAGS_VOLUME) {
            resource_dimension = .TEXTURE3D;
        } else {
            if ((header.dwCaps2 & DDS_CUBEMAP) == DDS_CUBEMAP) {
                // We require all six faces to be defined
                if ((header.dwCaps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES) {
                    std.log.debug("[DDS Loader] DirectX 12 doesn't support partial cubemaps.", .{});
                    return DdsError.NotSupported;
                }

                array_size = 6;
                cubemap = true;
            }

            depth = 1;
            resource_dimension = .TEXTURE2D;
        }

        assert(format.pixelSizeInBits() != 0);
    }

    if (mip_count > d3d12.req_mip_levels) {
        std.log.debug("[DDS Loader] Too many mipmap levels defined for DirectX 12 ({d}).", .{mip_count});
        return DdsError.NotSupported;
    }

    if (resource_dimension == .TEXTURE1D) {
        if (array_size > d3d12.req_texture1d_array_axis_dimension or width > d3d12.req_texture1d_u_dimension) {
            std.log.debug("[DDS Loader] Resource dimensions too large for DirectX 12 (1D: array {d}, size {d}).", .{ array_size, width });
            return DdsError.NotSupported;
        }
    } else if (resource_dimension == .TEXTURE2D) {
        if (cubemap) {
            // This is the right bound because we set arraySize to (NumCubes*6) above
            if (array_size > d3d12.req_texture2d_array_axis_dimension or width > d3d12.req_texturecube_dimension or height > d3d12.req_texturecube_dimension) {
                std.log.debug("[DDS Loader] Resource dimensions too large for DirectX 12 (2D cubemap: array {d}, size {d} by {d}).", .{ array_size, width, height });
                return DdsError.NotSupported;
            }
        } else if (array_size > d3d12.req_texture2d_array_axis_dimension or width > d3d12.req_texture2d_u_or_v_dimension or height > d3d12.req_texture2d_u_or_v_dimension) {
            std.log.debug("[DDS Loader] Resource dimensions too large for DirectX 12 (2D: array {d}, size {d} by {d}).", .{ array_size, width, height });
            return DdsError.NotSupported;
        }
    } else if (resource_dimension == .TEXTURE3D) {
        if (array_size > 1 or width > d3d12.req_texture3d_u_v_or_w_dimension or height > d3d12.req_texture3d_u_v_or_w_dimension or depth > d3d12.req_texture3d_u_v_or_w_dimension) {
            std.log.debug("[DDS Loader] Resource dimensions too large for DirectX 12 (3D: array {d}, size {d} by {d} by {d}).", .{ array_size, width, height, depth });
            return DdsError.NotSupported;
        }
    } else if (resource_dimension == .BUFFER) {
        std.log.debug("[DDS Loader] Resource dimension buffer type not supported for textures.", .{});
        return DdsError.InvalidDDSData;
    } else {
        std.log.debug("[DDS Loader] Unknown resource dimension ({}).", .{resource_dimension});
        return DdsError.InvalidDDSData;
    }

    const number_of_planes = getFormatPlaneCount(device, format);
    if (number_of_planes == 0) {
        return DdsError.InvalidDDSData;
    }

    if (number_of_planes > 1 and format.isDepthStencil()) {
        return DdsError.NotSupported;
    }

    // Create the texture
    var number_of_subresources = array_size;
    if (resource_dimension == .TEXTURE3D) {
        number_of_subresources = 1;
    }
    number_of_subresources *= mip_count;
    number_of_subresources *= number_of_planes;

    if (number_of_subresources > d3d12.req_subresources) {
        return DdsError.InvalidDDSData;
    }

    var skip_mip: u32 = 0;
    var total_width: u32 = 0;
    var total_height: u32 = 0;
    var total_depth: u32 = 0;

    var plane_index: u32 = 0;
    while (plane_index < number_of_planes) : (plane_index += 1) {
        var data_offset: u32 = 0;

        var array_index: u32 = 0;
        while (array_index < array_size) : (array_index += 1) {
            var w: u32 = width;
            var h: u32 = height;
            var d: u32 = depth;

            var mip_map_index: u32 = 0;
            while (mip_map_index < mip_count) : (mip_map_index += 1) {
                const surface_info = getSurfaceInfo(w, h, format);
                if (surface_info.num_bytes > std.math.maxInt(u32)) {
                    return DdsError.InvalidDDSData;
                }

                if (surface_info.row_bytes > std.math.maxInt(u32)) {
                    return DdsError.InvalidDDSData;
                }

                if (mip_count <= 1 or max_size == 0 or (w <= max_size and h <= max_size and d <= max_size)) {
                    if (total_width == 0) {
                        total_width = w;
                        total_height = h;
                        total_depth = d;
                    }

                    var resource = d3d12.SUBRESOURCE_DATA{
                        .pData = @as([*]u8, @ptrCast(data[data_offset..])),
                        .RowPitch = @as(c_uint, @intCast(surface_info.row_bytes)),
                        .SlicePitch = @as(c_uint, @intCast(surface_info.num_bytes)),
                    };

                    adjustPlaneResource(format, h, plane_index, &resource);

                    resources.append(resource) catch unreachable;
                } else if (mip_map_index == 0) {
                    skip_mip += 1;
                }

                data_offset += @as(u32, @intCast(surface_info.num_bytes)) * d;
                if (data_offset > data.len) {
                    return DdsError.EndOfFile;
                }

                w = w >> 1;
                h = h >> 1;
                d = d >> 1;

                if (w == 0) {
                    w = 1;
                }

                if (h == 0) {
                    h = 1;
                }

                if (d == 0) {
                    d = 1;
                }
            }
        }
    }

    return .{
        .width = header.dwWidth,
        .height = header.dwHeight,
        .depth = depth,
        .array_size = array_size,
        .mip_map_count = mip_count,
        .resource_dimension = resource_dimension,
        .format = format,
        .cubemap = cubemap,
        .alpha_mode = alpha_mode,
    };
}

fn getDXGIFormatFromDX10(header: DDS_HEADER_DXT10, width: u32, height: u32) !dxgi.FORMAT {
    return switch (header.dxgiFormat) {
        .NV12, .P010, .P016, .@"420_OPAQUE" => blk: {
            if ((header.resourceDimension != @intFromEnum(d3d12.RESOURCE_DIMENSION.TEXTURE2D)) or (width % 2 != 0) or (height % 2 != 0)) {
                std.log.debug("[DDS Loader] Video texture does not meet width/height requirements.", .{});
                break :blk DdsError.NotSupported;
            } else {
                break :blk header.dxgiFormat;
            }
        },

        .YUY2, .Y210, .Y216, .P208 => blk: {
            if (width % 2 != 0) {
                std.log.debug("[DDS Loader] Video texture does not meet width requirements.", .{});
                break :blk DdsError.NotSupported;
            } else {
                break :blk header.dxgiFormat;
            }
        },

        .NV11 => blk: {
            if (width % 4 != 0) {
                std.log.debug("[DDS Loader] Video texture does not meet width requirements.", .{});
                break :blk DdsError.NotSupported;
            } else {
                break :blk header.dxgiFormat;
            }
        },

        .AI44, .IA44, .P8, .A8P8 => blk: {
            std.log.debug("[DDS Loader] Legacy stream video texture formats are not supported by Direct3D", .{});
            break :blk DdsError.NotSupported;
        },

        .V208 => blk: {
            if ((header.resourceDimension != @intFromEnum(d3d12.RESOURCE_DIMENSION.TEXTURE2D)) or (height % 2 != 0)) {
                std.log.debug("[DDS Loader] Video texture does not meet height requirements.", .{});
                break :blk DdsError.NotSupported;
            } else {
                break :blk header.dxgiFormat;
            }
        },

        else => blk: {
            if (header.dxgiFormat.pixelSizeInBits() == 0) {
                std.log.debug("[DDS Loader] Unknown DXGI format {}.", .{header.dxgiFormat});
                break :blk DdsError.NotSupported;
            } else {
                break :blk header.dxgiFormat;
            }
        },
    };
}

fn getDXGIFormat(pixelFormat: DDS_PIXELFORMAT) dxgi.FORMAT {
    if ((pixelFormat.dwFlags & DDS_RGB) == DDS_RGB) {
        // Note that sRGB formats are written using the "DX10" extended header
        if (pixelFormat.dwRGBBitCount == 32) {
            if (isBitMask(pixelFormat, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000)) {
                return .R8G8B8A8_UNORM;
            }

            if (isBitMask(pixelFormat, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000)) {
                return .B8G8R8A8_UNORM;
            }

            if (isBitMask(pixelFormat, 0x00ff0000, 0x0000ff00, 0x000000ff, 0)) {
                return .B8G8R8X8_UNORM;
            }

            // No DXGI format maps to (0x000000ff,0x0000ff00,0x00ff0000,0) aka D3DFMT_X8B8G8R8

            // Note that many common DDS reader/writers (including D3DX) swap the
            // the RED/BLUE masks for 10:10:10:2 formats. We assume
            // below that the 'backwards' header mask is being used since it is most
            // likely written by D3DX. The more robust solution is to use the 'DX10'
            // header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

            // For 'correct' writers, this should be (0x000003ff,0x000ffc00,0x3ff00000) for RGB data
            if (isBitMask(pixelFormat, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000)) {
                return .R10G10B10A2_UNORM;
            }

            // No DXGI format maps to (0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

            if (isBitMask(pixelFormat, 0x0000ffff, 0xffff0000, 0, 0)) {
                return .R16G16_UNORM;
            }

            if (isBitMask(pixelFormat, 0xffffffff, 0, 0, 0)) {
                // Only 32-bit color channel format in D3D9 was R32F
                return .R32_FLOAT; // D3DX writes this out as a FourCC of 114
            }
        } else if (pixelFormat.dwRGBBitCount == 16) {
            if (isBitMask(pixelFormat, 0x7c00, 0x03e0, 0x001f, 0x8000)) {
                return .B5G5R5A1_UNORM;
            }
            if (isBitMask(pixelFormat, 0xf800, 0x07e0, 0x001f, 0)) {
                return .B5G6R5_UNORM;
            }

            // No DXGI format maps to (0x7c00,0x03e0,0x001f,0) aka D3DFMT_X1R5G5B5

            if (isBitMask(pixelFormat, 0x0f00, 0x00f0, 0x000f, 0xf000)) {
                return .B4G4R4A4_UNORM;
            }

            // NVTT versions 1.x wrote this as RGB instead of LUMINANCE
            if (isBitMask(pixelFormat, 0x00ff, 0, 0, 0xff00)) {
                return .R8G8_UNORM;
            }
            if (isBitMask(pixelFormat, 0xffff, 0, 0, 0)) {
                return .R16_UNORM;
            }

            // No DXGI format maps to (0x0f00,0x00f0,0x000f,0) aka D3DFMT_X4R4G4B4

            // No 3:3:2:8 or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_A8P8, etc.
        } else if (pixelFormat.dwRGBBitCount == 8) {
            // NVTT versions 1.x wrote this as RGB instead of LUMINANCE
            if (isBitMask(pixelFormat, 0xff, 0, 0, 0)) {
                return .R8_UNORM;
            }

            // No 3:3:2 or paletted DXGI formats aka D3DFMT_R3G3B2, D3DFMT_P8
        }
    } else if ((pixelFormat.dwFlags & DDS_LUMINANCE) == DDS_LUMINANCE) {
        if (pixelFormat.dwRGBBitCount == 16) {
            if (isBitMask(pixelFormat, 0xffff, 0, 0, 0)) {
                return .R16_UNORM; // D3DX10/11 writes this out as DX10 extension
            }
            if (isBitMask(pixelFormat, 0x00ff, 0, 0, 0xff00)) {
                return .R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
            }
        } else if (pixelFormat.dwRGBBitCount == 8) {
            if (isBitMask(pixelFormat, 0xff, 0, 0, 0)) {
                return .R8_UNORM; // D3DX10/11 writes this out as DX10 extension
            }

            // No DXGI format maps to isBitMask(pixelFormat, 0x0f,0,0,0xf0) aka D3DFMT_A4L4

            if (isBitMask(pixelFormat, 0x00ff, 0, 0, 0xff00)) {
                return .R8G8_UNORM; // Some DDS writers assume the bitcount should be 8 instead of 16
            }
        }
    } else if ((pixelFormat.dwFlags & DDS_ALPHA) == DDS_ALPHA) {
        if (pixelFormat.dwRGBBitCount == 8) {
            return .A8_UNORM;
        }
    } else if ((pixelFormat.dwFlags & DDS_BUMPDUDV) == DDS_BUMPDUDV) {
        if (pixelFormat.dwRGBBitCount == 32) {
            if (isBitMask(pixelFormat, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000)) {
                return .R8G8B8A8_SNORM; // D3DX10/11 writes this out as DX10 extension
            }
            if (isBitMask(pixelFormat, 0x0000ffff, 0xffff0000, 0, 0)) {
                return .R16G16_SNORM; // D3DX10/11 writes this out as DX10 extension
            }
        } else if (pixelFormat.dwRGBBitCount == 16) {
            if (isBitMask(pixelFormat, 0x00ff, 0xff00, 0, 0)) {
                return .R8G8_SNORM; // D3DX10/11 writes this out as DX10 extension
            }
        }
    } else if ((pixelFormat.dwFlags & DDS_FOURCC) == DDS_FOURCC) {
        if (makeFourCC('D', 'X', 'T', '1') == pixelFormat.dwFourCC) {
            return .BC1_UNORM;
        }
        if (makeFourCC('D', 'X', 'T', '3') == pixelFormat.dwFourCC) {
            return .BC2_UNORM;
        }
        if (makeFourCC('D', 'X', 'T', '5') == pixelFormat.dwFourCC) {
            return .BC3_UNORM;
        }

        // While pre-multiplied alpha isn't directly supported by the DXGI formats,
        // they are basically the same as these BC formats so they can be mapped
        if (makeFourCC('D', 'X', 'T', '2') == pixelFormat.dwFourCC) {
            return .BC2_UNORM;
        }
        if (makeFourCC('D', 'X', 'T', '4') == pixelFormat.dwFourCC) {
            return .BC3_UNORM;
        }
        if (makeFourCC('A', 'T', 'I', '1') == pixelFormat.dwFourCC) {
            return .BC4_UNORM;
        }
        if (makeFourCC('B', 'C', '4', 'U') == pixelFormat.dwFourCC) {
            return .BC4_UNORM;
        }
        if (makeFourCC('B', 'C', '4', 'S') == pixelFormat.dwFourCC) {
            return .BC4_SNORM;
        }
        if (makeFourCC('A', 'T', 'I', '2') == pixelFormat.dwFourCC) {
            return .BC5_UNORM;
        }
        if (makeFourCC('B', 'C', '5', 'U') == pixelFormat.dwFourCC) {
            return .BC5_UNORM;
        }
        if (makeFourCC('B', 'C', '5', 'S') == pixelFormat.dwFourCC) {
            return .BC5_SNORM;
        }

        // BC6H and BC7 are written using the "DX10" extended header
        if (makeFourCC('R', 'G', 'B', 'G') == pixelFormat.dwFourCC) {
            return .R8G8_B8G8_UNORM;
        }
        if (makeFourCC('G', 'R', 'G', 'B') == pixelFormat.dwFourCC) {
            return .G8R8_G8B8_UNORM;
        }
        if (makeFourCC('Y', 'U', 'Y', '2') == pixelFormat.dwFourCC) {
            return .YUY2;
        }

        // Check for D3DFORMAT enums being set here
        if (pixelFormat.dwFourCC == 36) {
            return .R16G16B16A16_UNORM;
        } else if (pixelFormat.dwFourCC == 110) {
            return .R16G16B16A16_SNORM;
        } else if (pixelFormat.dwFourCC == 111) {
            return .R16_FLOAT;
        } else if (pixelFormat.dwFourCC == 112) {
            return .R16G16_FLOAT;
        } else if (pixelFormat.dwFourCC == 113) {
            return .R16G16B16A16_FLOAT;
        } else if (pixelFormat.dwFourCC == 114) {
            return .R32_FLOAT;
        } else if (pixelFormat.dwFourCC == 115) {
            return .R32G32_FLOAT;
        } else if (pixelFormat.dwFourCC == 116) {
            return .R32G32B32A32_FLOAT;
        }
    }

    return .UNKNOWN;
}

const FormatData = struct {
    bc: bool = false,
    @"packed": bool = false,
    planar: bool = false,
    bpe: u32 = 0,
};

fn getSurfaceInfo(
    width: u32,
    height: u32,
    format: dxgi.FORMAT,
) struct { num_bytes: u64, row_bytes: u64, num_rows: u64 } {
    var num_bytes: u64 = 0;
    var row_bytes: u64 = 0;
    var num_rows: u64 = 0;

    const format_data = switch (format) {
        .BC1_TYPELESS, .BC1_UNORM, .BC1_UNORM_SRGB, .BC4_TYPELESS, .BC4_UNORM, .BC4_SNORM => FormatData{
            .bc = true,
            .@"packed" = false,
            .planar = false,
            .bpe = 8,
        },

        .BC2_TYPELESS, .BC2_UNORM, .BC2_UNORM_SRGB, .BC3_TYPELESS, .BC3_UNORM, .BC3_UNORM_SRGB, .BC5_TYPELESS, .BC5_UNORM, .BC5_SNORM, .BC6H_TYPELESS, .BC6H_UF16, .BC6H_SF16, .BC7_TYPELESS, .BC7_UNORM, .BC7_UNORM_SRGB => FormatData{
            .bc = true,
            .@"packed" = false,
            .planar = false,
            .bpe = 16,
        },

        .R8G8_B8G8_UNORM, .G8R8_G8B8_UNORM, .YUY2 => FormatData{
            .bc = false,
            .@"packed" = true,
            .planar = false,
            .bpe = 4,
        },

        .Y210, .Y216 => FormatData{
            .bc = false,
            .@"packed" = true,
            .planar = false,
            .bpe = 8,
        },

        .NV12, .@"420_OPAQUE" => blk: {
            // Requires a height alignment of 2.
            // return E_INVALIDARG;
            assert(height % 2 == 0);
            break :blk FormatData{
                .bc = false,
                .@"packed" = false,
                .planar = true,
                .bpe = 2,
            };
        },

        .P208 => FormatData{
            .bc = false,
            .@"packed" = false,
            .planar = true,
            .bpe = 2,
        },

        .P010, .P016 => blk: {
            // Requires a height alignment of 2.
            // return E_INVALIDARG;
            assert(height % 2 == 0);
            break :blk FormatData{
                .bc = false,
                .@"packed" = false,
                .planar = true,
                .bpe = 4,
            };
        },

        else => FormatData{
            .bc = false,
            .@"packed" = false,
            .planar = false,
            .bpe = 0,
        },
    };

    if (format_data.bc) {
        var num_blocks_wide: u64 = 0;
        if (width > 0) {
            num_blocks_wide = @max(1, @divTrunc(@as(u64, @intCast(width)) + 3, 4));
        }
        var num_blocks_high: u64 = 0;
        if (height > 0) {
            num_blocks_high = @max(1, @divTrunc(@as(u64, @intCast(height)) + 3, 4));
        }
        row_bytes = num_blocks_wide * format_data.bpe;
        num_rows = num_blocks_high;
        num_bytes = row_bytes * num_blocks_high;
    } else if (format_data.@"packed") {
        row_bytes = ((@as(u64, @intCast(width)) + 1) >> 1) * format_data.bpe;
        num_rows = @as(u64, @intCast(height));
        num_bytes = row_bytes * height;
    } else if (format == .NV11) {
        row_bytes = ((@as(u64, @intCast(width)) + 3) >> 2) * 4;
        num_rows = @as(u64, @intCast(height)) * 2; // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
        num_bytes = row_bytes * num_rows;
    } else if (format_data.planar) {
        row_bytes = ((@as(u64, @intCast(width)) + 1) >> 1) * format_data.bpe;
        num_bytes = (row_bytes * @as(u64, @intCast(height))) + ((row_bytes * @as(u64, @intCast(height)) + 1) >> 1);
        num_rows = height + ((@as(u64, @intCast(height)) + 1) >> 1);
    } else {
        const bpp = format.pixelSizeInBits();
        assert(bpp > 0);

        row_bytes = @divFloor(@as(u64, @intCast(width)) * bpp + 7, 8); // round up to nearest byte
        num_rows = @as(u64, @intCast(height));
        num_bytes = row_bytes * height;
    }

    return .{
        .num_bytes = num_bytes,
        .row_bytes = row_bytes,
        .num_rows = num_rows,
    };
}

fn adjustPlaneResource(format: dxgi.FORMAT, height: u32, slice_plane: u32, resource: *d3d12.SUBRESOURCE_DATA) void {
    if (format == .NV12 or format == .P010 or format == .P016) {
        if (slice_plane == 0) {
            // Plane 0
            resource.SlicePitch = resource.RowPitch * height;
        } else {
            // Plane 1
            const offset: u32 = resource.RowPitch * height;
            // resource.pData = resource.pData[offset..];
            resource.pData = resource.pData.? + offset;
            resource.SlicePitch = resource.RowPitch * ((height + 1) >> 1);
        }
    } else if (format == .NV11) {
        if (slice_plane == 0) {
            // Plane 0
            resource.SlicePitch = resource.RowPitch * height;
        } else {
            // Plane 1
            const offset: u32 = resource.RowPitch * height;
            // resource.pData = resource.pData[offset..];
            resource.pData = resource.pData.? + offset;
            resource.RowPitch = (resource.RowPitch >> 1);
            resource.SlicePitch = resource.RowPitch * height;
        }
    }
}

fn getFormatPlaneCount(device: *d3d12.IDevice9, format: dxgi.FORMAT) u8 {
    var data: d3d12.FEATURE_DATA_FORMAT_INFO = .{ .Format = format, .PlaneCount = 0 };
    const hr = device.CheckFeatureSupport(.FORMAT_INFO, &data, @sizeOf(d3d12.FEATURE_DATA_FORMAT_INFO));
    if (hr != windows.S_OK) {
        return 0;
    }

    return data.PlaneCount;
}
