//--------------------------------------------------------------------------------------------------
//
// OpenGL 1.0 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const Enum = c_uint;
pub const Float = f32;
pub const Int = c_int;
pub const Sizei = c_int;
pub const Bitfield = c_uint;
pub const Double = f64;
pub const Uint = c_uint;
pub const Boolean = u8;
pub const Ubyte = u8;

pub const DEPTH_BUFFER_BIT = 0x00000100;
pub const STENCIL_BUFFER_BIT = 0x00000400;
pub const COLOR_BUFFER_BIT = 0x00004000;
pub const FALSE = 0;
pub const TRUE = 1;
pub const POINTS = 0x0000;
pub const LINES = 0x0001;
pub const LINE_LOOP = 0x0002;
pub const LINE_STRIP = 0x0003;
pub const TRIANGLES = 0x0004;
pub const TRIANGLE_STRIP = 0x0005;
pub const TRIANGLE_FAN = 0x0006;
pub const QUADS = 0x0007;
pub const NEVER = 0x0200;
pub const LESS = 0x0201;
pub const EQUAL = 0x0202;
pub const LEQUAL = 0x0203;
pub const GREATER = 0x0204;
pub const NOTEQUAL = 0x0205;
pub const GEQUAL = 0x0206;
pub const ALWAYS = 0x0207;
pub const ZERO = 0;
pub const ONE = 1;
pub const SRC_COLOR = 0x0300;
pub const ONE_MINUS_SRC_COLOR = 0x0301;
pub const SRC_ALPHA = 0x0302;
pub const ONE_MINUS_SRC_ALPHA = 0x0303;
pub const DST_ALPHA = 0x0304;
pub const ONE_MINUS_DST_ALPHA = 0x0305;
pub const DST_COLOR = 0x0306;
pub const ONE_MINUS_DST_COLOR = 0x0307;
pub const SRC_ALPHA_SATURATE = 0x0308;
pub const NONE = 0;
pub const FRONT_LEFT = 0x0400;
pub const FRONT_RIGHT = 0x0401;
pub const BACK_LEFT = 0x0402;
pub const BACK_RIGHT = 0x0403;
pub const FRONT = 0x0404;
pub const BACK = 0x0405;
pub const LEFT = 0x0406;
pub const RIGHT = 0x0407;
pub const FRONT_AND_BACK = 0x0408;
pub const NO_ERROR = 0;
pub const INVALID_ENUM = 0x0500;
pub const INVALID_VALUE = 0x0501;
pub const INVALID_OPERATION = 0x0502;
pub const OUT_OF_MEMORY = 0x0505;
pub const CW = 0x0900;
pub const CCW = 0x0901;
pub const POINT_SIZE = 0x0B11;
pub const POINT_SIZE_RANGE = 0x0B12;
pub const POINT_SIZE_GRANULARITY = 0x0B13;
pub const LINE_SMOOTH = 0x0B20;
pub const LINE_WIDTH = 0x0B21;
pub const LINE_WIDTH_RANGE = 0x0B22;
pub const LINE_WIDTH_GRANULARITY = 0x0B23;
pub const POLYGON_MODE = 0x0B40;
pub const POLYGON_SMOOTH = 0x0B41;
pub const CULL_FACE = 0x0B44;
pub const CULL_FACE_MODE = 0x0B45;
pub const FRONT_FACE = 0x0B46;
pub const DEPTH_RANGE = 0x0B70;
pub const DEPTH_TEST = 0x0B71;
pub const DEPTH_WRITEMASK = 0x0B72;
pub const DEPTH_CLEAR_VALUE = 0x0B73;
pub const DEPTH_FUNC = 0x0B74;
pub const STENCIL_TEST = 0x0B90;
pub const STENCIL_CLEAR_VALUE = 0x0B91;
pub const STENCIL_FUNC = 0x0B92;
pub const STENCIL_VALUE_MASK = 0x0B93;
pub const STENCIL_FAIL = 0x0B94;
pub const STENCIL_PASS_DEPTH_FAIL = 0x0B95;
pub const STENCIL_PASS_DEPTH_PASS = 0x0B96;
pub const STENCIL_REF = 0x0B97;
pub const STENCIL_WRITEMASK = 0x0B98;
pub const VIEWPORT = 0x0BA2;
pub const DITHER = 0x0BD0;
pub const BLEND_DST = 0x0BE0;
pub const BLEND_SRC = 0x0BE1;
pub const BLEND = 0x0BE2;
pub const LOGIC_OP_MODE = 0x0BF0;
pub const DRAW_BUFFER = 0x0C01;
pub const READ_BUFFER = 0x0C02;
pub const SCISSOR_BOX = 0x0C10;
pub const SCISSOR_TEST = 0x0C11;
pub const COLOR_CLEAR_VALUE = 0x0C22;
pub const COLOR_WRITEMASK = 0x0C23;
pub const DOUBLEBUFFER = 0x0C32;
pub const STEREO = 0x0C33;
pub const LINE_SMOOTH_HINT = 0x0C52;
pub const POLYGON_SMOOTH_HINT = 0x0C53;
pub const UNPACK_SWAP_BYTES = 0x0CF0;
pub const UNPACK_LSB_FIRST = 0x0CF1;
pub const UNPACK_ROW_LENGTH = 0x0CF2;
pub const UNPACK_SKIP_ROWS = 0x0CF3;
pub const UNPACK_SKIP_PIXELS = 0x0CF4;
pub const UNPACK_ALIGNMENT = 0x0CF5;
pub const PACK_SWAP_BYTES = 0x0D00;
pub const PACK_LSB_FIRST = 0x0D01;
pub const PACK_ROW_LENGTH = 0x0D02;
pub const PACK_SKIP_ROWS = 0x0D03;
pub const PACK_SKIP_PIXELS = 0x0D04;
pub const PACK_ALIGNMENT = 0x0D05;
pub const MAX_TEXTURE_SIZE = 0x0D33;
pub const MAX_VIEWPORT_DIMS = 0x0D3A;
pub const SUBPIXEL_BITS = 0x0D50;
pub const TEXTURE_1D = 0x0DE0;
pub const TEXTURE_2D = 0x0DE1;
pub const TEXTURE_WIDTH = 0x1000;
pub const TEXTURE_HEIGHT = 0x1001;
pub const TEXTURE_BORDER_COLOR = 0x1004;
pub const DONT_CARE = 0x1100;
pub const FASTEST = 0x1101;
pub const NICEST = 0x1102;
pub const BYTE = 0x1400;
pub const UNSIGNED_BYTE = 0x1401;
pub const SHORT = 0x1402;
pub const UNSIGNED_SHORT = 0x1403;
pub const INT = 0x1404;
pub const UNSIGNED_INT = 0x1405;
pub const FLOAT = 0x1406;
pub const STACK_OVERFLOW = 0x0503;
pub const STACK_UNDERFLOW = 0x0504;
pub const CLEAR = 0x1500;
pub const AND = 0x1501;
pub const AND_REVERSE = 0x1502;
pub const COPY = 0x1503;
pub const AND_INVERTED = 0x1504;
pub const NOOP = 0x1505;
pub const XOR = 0x1506;
pub const OR = 0x1507;
pub const NOR = 0x1508;
pub const EQUIV = 0x1509;
pub const INVERT = 0x150A;
pub const OR_REVERSE = 0x150B;
pub const COPY_INVERTED = 0x150C;
pub const OR_INVERTED = 0x150D;
pub const NAND = 0x150E;
pub const SET = 0x150F;
pub const TEXTURE = 0x1702;
pub const COLOR = 0x1800;
pub const DEPTH = 0x1801;
pub const STENCIL = 0x1802;
pub const STENCIL_INDEX = 0x1901;
pub const DEPTH_COMPONENT = 0x1902;
pub const RED = 0x1903;
pub const GREEN = 0x1904;
pub const BLUE = 0x1905;
pub const ALPHA = 0x1906;
pub const RGB = 0x1907;
pub const RGBA = 0x1908;
pub const POINT = 0x1B00;
pub const LINE = 0x1B01;
pub const FILL = 0x1B02;
pub const KEEP = 0x1E00;
pub const REPLACE = 0x1E01;
pub const INCR = 0x1E02;
pub const DECR = 0x1E03;
pub const VENDOR = 0x1F00;
pub const RENDERER = 0x1F01;
pub const VERSION = 0x1F02;
pub const EXTENSIONS = 0x1F03;
pub const NEAREST = 0x2600;
pub const LINEAR = 0x2601;
pub const NEAREST_MIPMAP_NEAREST = 0x2700;
pub const LINEAR_MIPMAP_NEAREST = 0x2701;
pub const NEAREST_MIPMAP_LINEAR = 0x2702;
pub const LINEAR_MIPMAP_LINEAR = 0x2703;
pub const TEXTURE_MAG_FILTER = 0x2800;
pub const TEXTURE_MIN_FILTER = 0x2801;
pub const TEXTURE_WRAP_S = 0x2802;
pub const TEXTURE_WRAP_T = 0x2803;
pub const REPEAT = 0x2901;

pub var cullFace: *const fn (mode: Enum) callconv(.C) void = undefined;
pub var frontFace: *const fn (mode: Enum) callconv(.C) void = undefined;
pub var hint: *const fn (target: Enum, mode: Enum) callconv(.C) void = undefined;
pub var lineWidth: *const fn (width: Float) callconv(.C) void = undefined;
pub var pointSize: *const fn (size: Float) callconv(.C) void = undefined;
pub var polygonMode: *const fn (face: Enum, mode: Enum) callconv(.C) void = undefined;
pub var scissor: *const fn (x: Int, y: Int, width: Sizei, height: Sizei) callconv(.C) void = undefined;
pub var texParameterf: *const fn (target: Enum, pname: Enum, param: Float) callconv(.C) void = undefined;
pub var texParameterfv: *const fn (target: Enum, pname: Enum, params: [*c]const Float) callconv(.C) void = undefined;
pub var texParameteri: *const fn (target: Enum, pname: Enum, param: Int) callconv(.C) void = undefined;
pub var texParameteriv: *const fn (target: Enum, pname: Enum, params: [*c]const Int) callconv(.C) void = undefined;
pub var texImage1D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    width: Sizei,
    border: Int,
    format: Enum,
    type: Enum,
    pixels: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var texImage2D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    border: Int,
    format: Enum,
    type: Enum,
    pixels: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var drawBuffer: *const fn (buf: Enum) callconv(.C) void = undefined;
pub var clear: *const fn (mask: Bitfield) callconv(.C) void = undefined;
pub var clearColor: *const fn (red: Float, green: Float, blue: Float, alpha: Float) callconv(.C) void = undefined;
pub var clearStencil: *const fn (s: Int) callconv(.C) void = undefined;
pub var clearDepth: *const fn (depth: Double) callconv(.C) void = undefined;
pub var stencilMask: *const fn (mask: Uint) callconv(.C) void = undefined;
pub var colorMask: *const fn (
    red: Boolean,
    green: Boolean,
    blue: Boolean,
    alpha: Boolean,
) callconv(.C) void = undefined;
pub var depthMask: *const fn (flag: Boolean) callconv(.C) void = undefined;
pub var disable: *const fn (cap: Enum) callconv(.C) void = undefined;
pub var enable: *const fn (cap: Enum) callconv(.C) void = undefined;
pub var finish: *const fn () callconv(.C) void = undefined;
pub var flush: *const fn () callconv(.C) void = undefined;
pub var blendFunc: *const fn (sfactor: Enum, dfactor: Enum) callconv(.C) void = undefined;
pub var logicOp: *const fn (opcode: Enum) callconv(.C) void = undefined;
pub var stencilFunc: *const fn (func: Enum, ref: Int, mask: Uint) callconv(.C) void = undefined;
pub var stencilOp: *const fn (fail: Enum, zfail: Enum, zpass: Enum) callconv(.C) void = undefined;
pub var depthFunc: *const fn (func: Enum) callconv(.C) void = undefined;
pub var pixelStoref: *const fn (pname: Enum, param: Float) callconv(.C) void = undefined;
pub var pixelStorei: *const fn (pname: Enum, param: Int) callconv(.C) void = undefined;
pub var readBuffer: *const fn (src: Enum) callconv(.C) void = undefined;
pub var readPixels: *const fn (
    x: Int,
    y: Int,
    width: Sizei,
    height: Sizei,
    format: Enum,
    type: Enum,
    pixels: ?*anyopaque,
) callconv(.C) void = undefined;
pub var getBooleanv: *const fn (pname: Enum, data: [*c]Boolean) callconv(.C) void = undefined;
pub var getDoublev: *const fn (pname: Enum, data: [*c]Double) callconv(.C) void = undefined;
pub var getError: *const fn () callconv(.C) Enum = undefined;
pub var getFloatv: *const fn (pname: Enum, data: [*c]Float) callconv(.C) void = undefined;
pub var getIntegerv: *const fn (pname: Enum, data: [*c]Int) callconv(.C) void = undefined;
pub var getString: *const fn (name: Enum) callconv(.C) [*c]const Ubyte = undefined;
pub var getTexImage: *const fn (
    target: Enum,
    level: Int,
    format: Enum,
    type: Enum,
    pixels: ?*anyopaque,
) callconv(.C) void = undefined;
pub var getTexParameterfv: *const fn (target: Enum, pname: Enum, params: [*c]Float) callconv(.C) void = undefined;
pub var getTexParameteriv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getTexLevelParameterfv: *const fn (
    target: Enum,
    level: Int,
    pname: Enum,
    params: [*c]Float,
) callconv(.C) void = undefined;
pub var getTexLevelParameteriv: *const fn (
    target: Enum,
    level: Int,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var isEnabled: *const fn (cap: Enum) callconv(.C) Boolean = undefined;
pub var depthRange: *const fn (n: Double, f: Double) callconv(.C) void = undefined;
pub var viewport: *const fn (x: Int, y: Int, width: Sizei, height: Sizei) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 1.1 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const Clampf = Float;
pub const Clampd = Double;

pub const COLOR_LOGIC_OP = 0x0BF2;
pub const POLYGON_OFFSET_UNITS = 0x2A00;
pub const POLYGON_OFFSET_POINT = 0x2A01;
pub const POLYGON_OFFSET_LINE = 0x2A02;
pub const POLYGON_OFFSET_FILL = 0x8037;
pub const POLYGON_OFFSET_FACTOR = 0x8038;
pub const TEXTURE_BINDING_1D = 0x8068;
pub const TEXTURE_BINDING_2D = 0x8069;
pub const TEXTURE_INTERNAL_FORMAT = 0x1003;
pub const TEXTURE_RED_SIZE = 0x805C;
pub const TEXTURE_GREEN_SIZE = 0x805D;
pub const TEXTURE_BLUE_SIZE = 0x805E;
pub const TEXTURE_ALPHA_SIZE = 0x805F;
pub const DOUBLE = 0x140A;
pub const PROXY_TEXTURE_1D = 0x8063;
pub const PROXY_TEXTURE_2D = 0x8064;
pub const R3_G3_B2 = 0x2A10;
pub const RGB4 = 0x804F;
pub const RGB5 = 0x8050;
pub const RGB8 = 0x8051;
pub const RGB10 = 0x8052;
pub const RGB12 = 0x8053;
pub const RGB16 = 0x8054;
pub const RGBA2 = 0x8055;
pub const RGBA4 = 0x8056;
pub const RGB5_A1 = 0x8057;
pub const RGBA8 = 0x8058;
pub const RGB10_A2 = 0x8059;
pub const RGBA12 = 0x805A;
pub const RGBA16 = 0x805B;
pub const VERTEX_ARRAY = 0x8074;

pub var drawArrays: *const fn (mode: Enum, first: Int, count: Sizei) callconv(.C) void = undefined;
pub var drawElements: *const fn (
    mode: Enum,
    count: Sizei,
    type: Enum,
    indices: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var polygonOffset: *const fn (factor: Float, units: Float) callconv(.C) void = undefined;
pub var copyTexImage1D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    x: Int,
    y: Int,
    width: Sizei,
    border: Int,
) callconv(.C) void = undefined;
pub var copyTexImage2D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    x: Int,
    y: Int,
    width: Sizei,
    height: Sizei,
    border: Int,
) callconv(.C) void = undefined;
pub var copyTexSubImage1D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    x: Int,
    y: Int,
    width: Sizei,
) callconv(.C) void = undefined;
pub var copyTexSubImage2D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    x: Int,
    y: Int,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
pub var texSubImage1D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    width: Sizei,
    format: Enum,
    type: Enum,
    pixels: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var texSubImage2D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    width: Sizei,
    height: Sizei,
    format: Enum,
    type: Enum,
    pixels: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var bindTexture: *const fn (target: Enum, texture: Uint) callconv(.C) void = undefined;
pub var deleteTextures: *const fn (n: Sizei, textures: [*c]const Uint) callconv(.C) void = undefined;
pub var genTextures: *const fn (n: Sizei, textures: [*c]Uint) callconv(.C) void = undefined;
pub var isTexture: *const fn (texture: Uint) callconv(.C) Boolean = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 1.2 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const UNSIGNED_BYTE_3_3_2 = 0x8032;
pub const UNSIGNED_SHORT_4_4_4_4 = 0x8033;
pub const UNSIGNED_SHORT_5_5_5_1 = 0x8034;
pub const UNSIGNED_INT_8_8_8_8 = 0x8035;
pub const UNSIGNED_INT_10_10_10_2 = 0x8036;
pub const TEXTURE_BINDING_3D = 0x806A;
pub const PACK_SKIP_IMAGES = 0x806B;
pub const PACK_IMAGE_HEIGHT = 0x806C;
pub const UNPACK_SKIP_IMAGES = 0x806D;
pub const UNPACK_IMAGE_HEIGHT = 0x806E;
pub const TEXTURE_3D = 0x806F;
pub const PROXY_TEXTURE_3D = 0x8070;
pub const TEXTURE_DEPTH = 0x8071;
pub const TEXTURE_WRAP_R = 0x8072;
pub const MAX_3D_TEXTURE_SIZE = 0x8073;
pub const UNSIGNED_BYTE_2_3_3_REV = 0x8362;
pub const UNSIGNED_SHORT_5_6_5 = 0x8363;
pub const UNSIGNED_SHORT_5_6_5_REV = 0x8364;
pub const UNSIGNED_SHORT_4_4_4_4_REV = 0x8365;
pub const UNSIGNED_SHORT_1_5_5_5_REV = 0x8366;
pub const UNSIGNED_INT_8_8_8_8_REV = 0x8367;
pub const UNSIGNED_INT_2_10_10_10_REV = 0x8368;
pub const BGR = 0x80E0;
pub const BGRA = 0x80E1;
pub const MAX_ELEMENTS_VERTICES = 0x80E8;
pub const MAX_ELEMENTS_INDICES = 0x80E9;
pub const CLAMP_TO_EDGE = 0x812F;
pub const TEXTURE_MIN_LOD = 0x813A;
pub const TEXTURE_MAX_LOD = 0x813B;
pub const TEXTURE_BASE_LEVEL = 0x813C;
pub const TEXTURE_MAX_LEVEL = 0x813D;
pub const SMOOTH_POINT_SIZE_RANGE = 0x0B12;
pub const SMOOTH_POINT_SIZE_GRANULARITY = 0x0B13;
pub const SMOOTH_LINE_WIDTH_RANGE = 0x0B22;
pub const SMOOTH_LINE_WIDTH_GRANULARITY = 0x0B23;
pub const ALIASED_LINE_WIDTH_RANGE = 0x846E;

pub var drawRangeElements: *const fn (
    mode: Enum,
    start: Uint,
    end: Uint,
    count: Sizei,
    type: Enum,
    indices: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var texImage3D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    depth: Sizei,
    border: Int,
    format: Enum,
    type: Enum,
    pixels: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var texSubImage3D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    zoffset: Int,
    width: Sizei,
    height: Sizei,
    depth: Sizei,
    format: Enum,
    type: Enum,
    pixels: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var copyTexSubImage3D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    zoffset: Int,
    x: Int,
    y: Int,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 1.3 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const TEXTURE0 = 0x84C0;
pub const TEXTURE1 = 0x84C1;
pub const TEXTURE2 = 0x84C2;
pub const TEXTURE3 = 0x84C3;
pub const TEXTURE4 = 0x84C4;
pub const TEXTURE5 = 0x84C5;
pub const TEXTURE6 = 0x84C6;
pub const TEXTURE7 = 0x84C7;
pub const TEXTURE8 = 0x84C8;
pub const TEXTURE9 = 0x84C9;
pub const TEXTURE10 = 0x84CA;
pub const TEXTURE11 = 0x84CB;
pub const TEXTURE12 = 0x84CC;
pub const TEXTURE13 = 0x84CD;
pub const TEXTURE14 = 0x84CE;
pub const TEXTURE15 = 0x84CF;
pub const TEXTURE16 = 0x84D0;
pub const TEXTURE17 = 0x84D1;
pub const TEXTURE18 = 0x84D2;
pub const TEXTURE19 = 0x84D3;
pub const TEXTURE20 = 0x84D4;
pub const TEXTURE21 = 0x84D5;
pub const TEXTURE22 = 0x84D6;
pub const TEXTURE23 = 0x84D7;
pub const TEXTURE24 = 0x84D8;
pub const TEXTURE25 = 0x84D9;
pub const TEXTURE26 = 0x84DA;
pub const TEXTURE27 = 0x84DB;
pub const TEXTURE28 = 0x84DC;
pub const TEXTURE29 = 0x84DD;
pub const TEXTURE30 = 0x84DE;
pub const TEXTURE31 = 0x84DF;
pub const ACTIVE_TEXTURE = 0x84E0;
pub const MULTISAMPLE = 0x809D;
pub const SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
pub const SAMPLE_ALPHA_TO_ONE = 0x809F;
pub const SAMPLE_COVERAGE = 0x80A0;
pub const SAMPLE_BUFFERS = 0x80A8;
pub const SAMPLES = 0x80A9;
pub const SAMPLE_COVERAGE_VALUE = 0x80AA;
pub const SAMPLE_COVERAGE_INVERT = 0x80AB;
pub const TEXTURE_CUBE_MAP = 0x8513;
pub const TEXTURE_BINDING_CUBE_MAP = 0x8514;
pub const TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
pub const TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
pub const TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
pub const TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
pub const TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
pub const TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
pub const PROXY_TEXTURE_CUBE_MAP = 0x851B;
pub const MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;
pub const COMPRESSED_RGB = 0x84ED;
pub const COMPRESSED_RGBA = 0x84EE;
pub const TEXTURE_COMPRESSION_HINT = 0x84EF;
pub const TEXTURE_COMPRESSED_IMAGE_SIZE = 0x86A0;
pub const TEXTURE_COMPRESSED = 0x86A1;
pub const NUM_COMPRESSED_TEXTURE_FORMATS = 0x86A2;
pub const COMPRESSED_TEXTURE_FORMATS = 0x86A3;
pub const CLAMP_TO_BORDER = 0x812D;

pub var activeTexture: *const fn (texture: Enum) callconv(.C) void = undefined;
pub var sampleCoverage: *const fn (value: Float, invert: Boolean) callconv(.C) void = undefined;
pub var compressedTexImage3D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    depth: Sizei,
    border: Int,
    imageSize: Sizei,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var compressedTexImage2D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    border: Int,
    imageSize: Sizei,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var compressedTexImage1D: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    width: Sizei,
    border: Int,
    imageSize: Sizei,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var compressedTexSubImage3D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    zoffset: Int,
    width: Sizei,
    height: Sizei,
    depth: Sizei,
    format: Enum,
    imageSize: Sizei,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var compressedTexSubImage2D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    width: Sizei,
    height: Sizei,
    format: Enum,
    imageSize: Sizei,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var compressedTexSubImage1D: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    width: Sizei,
    format: Enum,
    imageSize: Sizei,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var getCompressedTexImage: *const fn (target: Enum, level: Int, img: ?*anyopaque) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 1.4 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const BLEND_DST_RGB = 0x80C8;
pub const BLEND_SRC_RGB = 0x80C9;
pub const BLEND_DST_ALPHA = 0x80CA;
pub const BLEND_SRC_ALPHA = 0x80CB;
pub const POINT_FADE_THRESHOLD_SIZE = 0x8128;
pub const DEPTH_COMPONENT16 = 0x81A5;
pub const DEPTH_COMPONENT24 = 0x81A6;
pub const DEPTH_COMPONENT32 = 0x81A7;
pub const MIRRORED_REPEAT = 0x8370;
pub const MAX_TEXTURE_LOD_BIAS = 0x84FD;
pub const TEXTURE_LOD_BIAS = 0x8501;
pub const INCR_WRAP = 0x8507;
pub const DECR_WRAP = 0x8508;
pub const TEXTURE_DEPTH_SIZE = 0x884A;
pub const TEXTURE_COMPARE_MODE = 0x884C;
pub const TEXTURE_COMPARE_FUNC = 0x884D;
pub const BLEND_COLOR = 0x8005;
pub const BLEND_EQUATION = 0x8009;
pub const CONSTANT_COLOR = 0x8001;
pub const ONE_MINUS_CONSTANT_COLOR = 0x8002;
pub const CONSTANT_ALPHA = 0x8003;
pub const ONE_MINUS_CONSTANT_ALPHA = 0x8004;
pub const FUNC_ADD = 0x8006;
pub const FUNC_REVERSE_SUBTRACT = 0x800B;
pub const FUNC_SUBTRACT = 0x800A;
pub const MIN = 0x8007;
pub const MAX = 0x8008;

pub var blendFuncSeparate: *const fn (
    sfactorRGB: Enum,
    dfactorRGB: Enum,
    sfactorAlpha: Enum,
    dfactorAlpha: Enum,
) callconv(.C) void = undefined;
pub var multiDrawArrays: *const fn (
    mode: Enum,
    first: [*c]const Int,
    count: [*c]const Sizei,
    drawcount: Sizei,
) callconv(.C) void = undefined;
pub var multiDrawElements: *const fn (
    mode: Enum,
    count: [*c]const Sizei,
    type: Enum,
    indices: [*c]const ?*const anyopaque,
    drawcount: Sizei,
) callconv(.C) void = undefined;
pub var pointParameterf: *const fn (pname: Enum, param: Float) callconv(.C) void = undefined;
pub var pointParameterfv: *const fn (pname: Enum, params: [*c]const Float) callconv(.C) void = undefined;
pub var pointParameteri: *const fn (pname: Enum, param: Int) callconv(.C) void = undefined;
pub var pointParameteriv: *const fn (pname: Enum, params: [*c]const Int) callconv(.C) void = undefined;
pub var blendColor: *const fn (red: Float, green: Float, blue: Float, alpha: Float) callconv(.C) void = undefined;
pub var blendEquation: *const fn (mode: Enum) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 1.5 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const Sizeiptr = isize;
pub const Intptr = isize;

pub const BUFFER_SIZE = 0x8764;
pub const BUFFER_USAGE = 0x8765;
pub const QUERY_COUNTER_BITS = 0x8864;
pub const CURRENT_QUERY = 0x8865;
pub const QUERY_RESULT = 0x8866;
pub const QUERY_RESULT_AVAILABLE = 0x8867;
pub const ARRAY_BUFFER = 0x8892;
pub const ELEMENT_ARRAY_BUFFER = 0x8893;
pub const ARRAY_BUFFER_BINDING = 0x8894;
pub const ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
pub const VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;
pub const READ_ONLY = 0x88B8;
pub const WRITE_ONLY = 0x88B9;
pub const READ_WRITE = 0x88BA;
pub const BUFFER_ACCESS = 0x88BB;
pub const BUFFER_MAPPED = 0x88BC;
pub const BUFFER_MAP_POINTER = 0x88BD;
pub const STREAM_DRAW = 0x88E0;
pub const STREAM_READ = 0x88E1;
pub const STREAM_COPY = 0x88E2;
pub const STATIC_DRAW = 0x88E4;
pub const STATIC_READ = 0x88E5;
pub const STATIC_COPY = 0x88E6;
pub const DYNAMIC_DRAW = 0x88E8;
pub const DYNAMIC_READ = 0x88E9;
pub const DYNAMIC_COPY = 0x88EA;
pub const SAMPLES_PASSED = 0x8914;
pub const SRC1_ALPHA = 0x8589;

pub var genQueries: *const fn (n: Sizei, ids: [*c]Uint) callconv(.C) void = undefined;
pub var deleteQueries: *const fn (n: Sizei, ids: [*c]const Uint) callconv(.C) void = undefined;
pub var isQuery: *const fn (id: Uint) callconv(.C) Boolean = undefined;
pub var beginQuery: *const fn (target: Enum, id: Uint) callconv(.C) void = undefined;
pub var endQuery: *const fn (target: Enum) callconv(.C) void = undefined;
pub var getQueryiv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getQueryObjectiv: *const fn (id: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getQueryObjectuiv: *const fn (id: Uint, pname: Enum, params: [*c]Uint) callconv(.C) void = undefined;
pub var bindBuffer: *const fn (target: Enum, buffer: Uint) callconv(.C) void = undefined;
pub var deleteBuffers: *const fn (n: Sizei, buffers: [*c]const Uint) callconv(.C) void = undefined;
pub var genBuffers: *const fn (n: Sizei, buffers: [*c]Uint) callconv(.C) void = undefined;
pub var isBuffer: *const fn (buffer: Uint) callconv(.C) Boolean = undefined;
pub var bufferData: *const fn (
    target: Enum,
    size: Sizeiptr,
    data: ?*const anyopaque,
    usage: Enum,
) callconv(.C) void = undefined;
pub var bufferSubData: *const fn (
    target: Enum,
    offset: Intptr,
    size: Sizeiptr,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var getBufferSubData: *const fn (
    target: Enum,
    offset: Intptr,
    size: Sizeiptr,
    data: ?*anyopaque,
) callconv(.C) void = undefined;
pub var mapBuffer: *const fn (target: Enum, access: Enum) callconv(.C) ?*anyopaque = undefined;
pub var unmapBuffer: *const fn (target: Enum) callconv(.C) Boolean = undefined;
pub var getBufferParameteriv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getBufferPointerv: *const fn (
    target: Enum,
    pname: Enum,
    params: [*c]?*anyopaque,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 2.0 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const Char = u8;
pub const Short = i16;
pub const Byte = i8;
pub const Ushort = u16;

pub const BLEND_EQUATION_RGB = 0x8009;
pub const VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
pub const VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
pub const VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
pub const VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
pub const CURRENT_VERTEX_ATTRIB = 0x8626;
pub const VERTEX_PROGRAM_POINT_SIZE = 0x8642;
pub const VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
pub const STENCIL_BACK_FUNC = 0x8800;
pub const STENCIL_BACK_FAIL = 0x8801;
pub const STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
pub const STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
pub const MAX_DRAW_BUFFERS = 0x8824;
pub const DRAW_BUFFER0 = 0x8825;
pub const DRAW_BUFFER1 = 0x8826;
pub const DRAW_BUFFER2 = 0x8827;
pub const DRAW_BUFFER3 = 0x8828;
pub const DRAW_BUFFER4 = 0x8829;
pub const DRAW_BUFFER5 = 0x882A;
pub const DRAW_BUFFER6 = 0x882B;
pub const DRAW_BUFFER7 = 0x882C;
pub const DRAW_BUFFER8 = 0x882D;
pub const DRAW_BUFFER9 = 0x882E;
pub const DRAW_BUFFER10 = 0x882F;
pub const DRAW_BUFFER11 = 0x8830;
pub const DRAW_BUFFER12 = 0x8831;
pub const DRAW_BUFFER13 = 0x8832;
pub const DRAW_BUFFER14 = 0x8833;
pub const DRAW_BUFFER15 = 0x8834;
pub const BLEND_EQUATION_ALPHA = 0x883D;
pub const MAX_VERTEX_ATTRIBS = 0x8869;
pub const VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
pub const MAX_TEXTURE_IMAGE_UNITS = 0x8872;
pub const FRAGMENT_SHADER = 0x8B30;
pub const VERTEX_SHADER = 0x8B31;
pub const MAX_FRAGMENT_UNIFORM_COMPONENTS = 0x8B49;
pub const MAX_VERTEX_UNIFORM_COMPONENTS = 0x8B4A;
pub const MAX_VARYING_FLOATS = 0x8B4B;
pub const MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
pub const MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
pub const SHADER_TYPE = 0x8B4F;
pub const FLOAT_VEC2 = 0x8B50;
pub const FLOAT_VEC3 = 0x8B51;
pub const FLOAT_VEC4 = 0x8B52;
pub const INT_VEC2 = 0x8B53;
pub const INT_VEC3 = 0x8B54;
pub const INT_VEC4 = 0x8B55;
pub const BOOL = 0x8B56;
pub const BOOL_VEC2 = 0x8B57;
pub const BOOL_VEC3 = 0x8B58;
pub const BOOL_VEC4 = 0x8B59;
pub const FLOAT_MAT2 = 0x8B5A;
pub const FLOAT_MAT3 = 0x8B5B;
pub const FLOAT_MAT4 = 0x8B5C;
pub const SAMPLER_1D = 0x8B5D;
pub const SAMPLER_2D = 0x8B5E;
pub const SAMPLER_3D = 0x8B5F;
pub const SAMPLER_CUBE = 0x8B60;
pub const SAMPLER_1D_SHADOW = 0x8B61;
pub const SAMPLER_2D_SHADOW = 0x8B62;
pub const DELETE_STATUS = 0x8B80;
pub const COMPILE_STATUS = 0x8B81;
pub const LINK_STATUS = 0x8B82;
pub const VALIDATE_STATUS = 0x8B83;
pub const INFO_LOG_LENGTH = 0x8B84;
pub const ATTACHED_SHADERS = 0x8B85;
pub const ACTIVE_UNIFORMS = 0x8B86;
pub const ACTIVE_UNIFORM_MAX_LENGTH = 0x8B87;
pub const SHADER_SOURCE_LENGTH = 0x8B88;
pub const ACTIVE_ATTRIBUTES = 0x8B89;
pub const ACTIVE_ATTRIBUTE_MAX_LENGTH = 0x8B8A;
pub const FRAGMENT_SHADER_DERIVATIVE_HINT = 0x8B8B;
pub const SHADING_LANGUAGE_VERSION = 0x8B8C;
pub const CURRENT_PROGRAM = 0x8B8D;
pub const POINT_SPRITE_COORD_ORIGIN = 0x8CA0;
pub const LOWER_LEFT = 0x8CA1;
pub const UPPER_LEFT = 0x8CA2;
pub const STENCIL_BACK_REF = 0x8CA3;
pub const STENCIL_BACK_VALUE_MASK = 0x8CA4;
pub const STENCIL_BACK_WRITEMASK = 0x8CA5;

pub var blendEquationSeparate: *const fn (modeRGB: Enum, modeAlpha: Enum) callconv(.C) void = undefined;
pub var drawBuffers: *const fn (n: Sizei, bufs: [*c]const Enum) callconv(.C) void = undefined;
pub var stencilOpSeparate: *const fn (
    face: Enum,
    sfail: Enum,
    dpfail: Enum,
    dppass: Enum,
) callconv(.C) void = undefined;
pub var stencilFuncSeparate: *const fn (face: Enum, func: Enum, ref: Int, mask: Uint) callconv(.C) void = undefined;
pub var stencilMaskSeparate: *const fn (face: Enum, mask: Uint) callconv(.C) void = undefined;
pub var attachShader: *const fn (program: Uint, shader: Uint) callconv(.C) void = undefined;
pub var bindAttribLocation: *const fn (
    program: Uint,
    index: Uint,
    name: [*c]const Char,
) callconv(.C) void = undefined;
pub var compileShader: *const fn (shader: Uint) callconv(.C) void = undefined;
pub var createProgram: *const fn () callconv(.C) Uint = undefined;
pub var createShader: *const fn (type: Enum) callconv(.C) Uint = undefined;
pub var deleteProgram: *const fn (program: Uint) callconv(.C) void = undefined;
pub var deleteShader: *const fn (shader: Uint) callconv(.C) void = undefined;
pub var detachShader: *const fn (program: Uint, shader: Uint) callconv(.C) void = undefined;
pub var disableVertexAttribArray: *const fn (index: Uint) callconv(.C) void = undefined;
pub var enableVertexAttribArray: *const fn (index: Uint) callconv(.C) void = undefined;
pub var getActiveAttrib: *const fn (
    program: Uint,
    index: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    size: [*c]Int,
    type: [*c]Enum,
    name: [*c]Char,
) callconv(.C) void = undefined;
pub var getActiveUniform: *const fn (
    program: Uint,
    index: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    size: [*c]Int,
    type: [*c]Enum,
    name: [*c]Char,
) callconv(.C) Int = undefined;
pub var getAttachedShaders: *const fn (
    program: Uint,
    maxCount: Sizei,
    count: [*c]Sizei,
    shaders: [*c]Uint,
) callconv(.C) void = undefined;
pub var getAttribLocation: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
pub var getProgramiv: *const fn (program: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getProgramInfoLog: *const fn (
    program: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    infoLog: [*c]Char,
) callconv(.C) void = undefined;
pub var getShaderiv: *const fn (shader: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getShaderInfoLog: *const fn (
    shader: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    infoLog: [*c]Char,
) callconv(.C) void = undefined;
pub var getShaderSource: *const fn (
    shader: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    source: [*c]Char,
) callconv(.C) void = undefined;
pub var getUniformLocation: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
pub var getUniformfv: *const fn (program: Uint, location: Int, params: [*c]Float) callconv(.C) void = undefined;
pub var getUniformiv: *const fn (program: Uint, location: Int, params: [*c]Int) callconv(.C) void = undefined;
pub var getVertexAttribdv: *const fn (index: Uint, pname: Enum, params: [*c]Double) callconv(.C) void = undefined;
pub var getVertexAttribfv: *const fn (index: Uint, pname: Enum, params: [*c]Float) callconv(.C) void = undefined;
pub var getVertexAttribiv: *const fn (index: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getVertexAttribPointerv: *const fn (
    index: Uint,
    pname: Enum,
    pointer: [*c]?*anyopaque,
) callconv(.C) void = undefined;
pub var isProgram: *const fn (program: Uint) callconv(.C) Boolean = undefined;
pub var isShader: *const fn (shader: Uint) callconv(.C) Boolean = undefined;
pub var linkProgram: *const fn (program: Uint) callconv(.C) void = undefined;
pub var shaderSource: *const fn (
    shader: Uint,
    count: Sizei,
    string: [*c]const [*c]const Char,
    length: [*c]const Int,
) callconv(.C) void = undefined;
pub var useProgram: *const fn (program: Uint) callconv(.C) void = undefined;
pub var uniform1f: *const fn (location: Int, v0: Float) callconv(.C) void = undefined;
pub var uniform2f: *const fn (location: Int, v0: Float, v1: Float) callconv(.C) void = undefined;
pub var uniform3f: *const fn (location: Int, v0: Float, v1: Float, v2: Float) callconv(.C) void = undefined;
pub var uniform4f: *const fn (
    location: Int,
    v0: Float,
    v1: Float,
    v2: Float,
    v3: Float,
) callconv(.C) void = undefined;
pub var uniform1i: *const fn (location: Int, v0: Int) callconv(.C) void = undefined;
pub var uniform2i: *const fn (location: Int, v0: Int, v1: Int) callconv(.C) void = undefined;
pub var uniform3i: *const fn (location: Int, v0: Int, v1: Int, v2: Int) callconv(.C) void = undefined;
pub var uniform4i: *const fn (
    location: Int,
    v0: Int,
    v1: Int,
    v2: Int,
    v3: Int,
) callconv(.C) void = undefined;
pub var uniform1fv: *const fn (
    location: Int,
    count: Sizei,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniform2fv: *const fn (
    location: Int,
    count: Sizei,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniform3fv: *const fn (
    location: Int,
    count: Sizei,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniform4fv: *const fn (
    location: Int,
    count: Sizei,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniform1iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniform2iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniform3iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniform4iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniformMatrix2fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix3fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix4fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var validateProgram: *const fn (program: Uint) callconv(.C) void = undefined;
pub var vertexAttrib1d: *const fn (index: Uint, x: Double) callconv(.C) void = undefined;
pub var vertexAttrib1dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
pub var vertexAttrib1f: *const fn (index: Uint, x: Float) callconv(.C) void = undefined;
pub var vertexAttrib1fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
pub var vertexAttrib1s: *const fn (index: Uint, x: Short) callconv(.C) void = undefined;
pub var vertexAttrib1sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
pub var vertexAttrib2d: *const fn (index: Uint, x: Double, y: Double) callconv(.C) void = undefined;
pub var vertexAttrib2dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
pub var vertexAttrib2f: *const fn (index: Uint, x: Float, y: Float) callconv(.C) void = undefined;
pub var vertexAttrib2fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
pub var vertexAttrib2s: *const fn (index: Uint, x: Short, y: Short) callconv(.C) void = undefined;
pub var vertexAttrib2sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
pub var vertexAttrib3d: *const fn (index: Uint, x: Double, y: Double, z: Double) callconv(.C) void = undefined;
pub var vertexAttrib3dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
pub var vertexAttrib3f: *const fn (index: Uint, x: Float, y: Float, z: Float) callconv(.C) void = undefined;
pub var vertexAttrib3fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
pub var vertexAttrib3s: *const fn (index: Uint, x: Short, y: Short, z: Short) callconv(.C) void = undefined;
pub var vertexAttrib3sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
pub var vertexAttrib4Nbv: *const fn (index: Uint, v: [*c]const Byte) callconv(.C) void = undefined;
pub var vertexAttrib4Niv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
pub var vertexAttrib4Nsv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
pub var vertexAttrib4Nub: *const fn (
    index: Uint,
    x: Ubyte,
    y: Ubyte,
    z: Ubyte,
    w: Ubyte,
) callconv(.C) void = undefined;
pub var vertexAttrib4Nubv: *const fn (index: Uint, v: [*c]const Ubyte) callconv(.C) void = undefined;
pub var vertexAttrib4Nuiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
pub var vertexAttrib4Nusv: *const fn (index: Uint, v: [*c]const Ushort) callconv(.C) void = undefined;
pub var vertexAttrib4bv: *const fn (index: Uint, v: [*c]const Byte) callconv(.C) void = undefined;
pub var vertexAttrib4d: *const fn (
    index: Uint,
    x: Double,
    y: Double,
    z: Double,
    w: Double,
) callconv(.C) void = undefined;
pub var vertexAttrib4dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
pub var vertexAttrib4f: *const fn (
    index: Uint,
    x: Float,
    y: Float,
    z: Float,
    w: Float,
) callconv(.C) void = undefined;
pub var vertexAttrib4fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
pub var vertexAttrib4iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
pub var vertexAttrib4s: *const fn (
    index: Uint,
    x: Short,
    y: Short,
    z: Short,
    w: Short,
) callconv(.C) void = undefined;
pub var vertexAttrib4sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
pub var vertexAttrib4ubv: *const fn (index: Uint, v: [*c]const Ubyte) callconv(.C) void = undefined;
pub var vertexAttrib4uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
pub var vertexAttrib4usv: *const fn (index: Uint, v: [*c]const Ushort) callconv(.C) void = undefined;
pub var vertexAttribPointer: *const fn (
    index: Uint,
    size: Int,
    type: Enum,
    normalized: Boolean,
    stride: Sizei,
    pointer: ?*const anyopaque,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 2.1 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const PIXEL_PACK_BUFFER = 0x88EB;
pub const PIXEL_UNPACK_BUFFER = 0x88EC;
pub const PIXEL_PACK_BUFFER_BINDING = 0x88ED;
pub const PIXEL_UNPACK_BUFFER_BINDING = 0x88EF;
pub const FLOAT_MAT2x3 = 0x8B65;
pub const FLOAT_MAT2x4 = 0x8B66;
pub const FLOAT_MAT3x2 = 0x8B67;
pub const FLOAT_MAT3x4 = 0x8B68;
pub const FLOAT_MAT4x2 = 0x8B69;
pub const FLOAT_MAT4x3 = 0x8B6A;
pub const SRGB = 0x8C40;
pub const SRGB8 = 0x8C41;
pub const SRGB_ALPHA = 0x8C42;
pub const SRGB8_ALPHA8 = 0x8C43;
pub const COMPRESSED_SRGB = 0x8C48;
pub const COMPRESSED_SRGB_ALPHA = 0x8C49;

pub var uniformMatrix2x3fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix3x2fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix2x4fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix4x2fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix3x4fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix4x3fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*c]const Float,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 3.0 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const Half = u16;

pub const COMPARE_REF_TO_TEXTURE = 0x884E;
pub const CLIP_DISTANCE0 = 0x3000;
pub const CLIP_DISTANCE1 = 0x3001;
pub const CLIP_DISTANCE2 = 0x3002;
pub const CLIP_DISTANCE3 = 0x3003;
pub const CLIP_DISTANCE4 = 0x3004;
pub const CLIP_DISTANCE5 = 0x3005;
pub const CLIP_DISTANCE6 = 0x3006;
pub const CLIP_DISTANCE7 = 0x3007;
pub const MAX_CLIP_DISTANCES = 0x0D32;
pub const MAJOR_VERSION = 0x821B;
pub const MINOR_VERSION = 0x821C;
pub const NUM_EXTENSIONS = 0x821D;
pub const CONTEXT_FLAGS = 0x821E;
pub const COMPRESSED_RED = 0x8225;
pub const COMPRESSED_RG = 0x8226;
pub const CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT = 0x00000001;
pub const RGBA32F = 0x8814;
pub const RGB32F = 0x8815;
pub const RGBA16F = 0x881A;
pub const RGB16F = 0x881B;
pub const VERTEX_ATTRIB_ARRAY_INTEGER = 0x88FD;
pub const MAX_ARRAY_TEXTURE_LAYERS = 0x88FF;
pub const MIN_PROGRAM_TEXEL_OFFSET = 0x8904;
pub const MAX_PROGRAM_TEXEL_OFFSET = 0x8905;
pub const CLAMP_READ_COLOR = 0x891C;
pub const FIXED_ONLY = 0x891D;
pub const MAX_VARYING_COMPONENTS = 0x8B4B;
pub const TEXTURE_1D_ARRAY = 0x8C18;
pub const PROXY_TEXTURE_1D_ARRAY = 0x8C19;
pub const TEXTURE_2D_ARRAY = 0x8C1A;
pub const PROXY_TEXTURE_2D_ARRAY = 0x8C1B;
pub const TEXTURE_BINDING_1D_ARRAY = 0x8C1C;
pub const TEXTURE_BINDING_2D_ARRAY = 0x8C1D;
pub const R11F_G11F_B10F = 0x8C3A;
pub const UNSIGNED_INT_10F_11F_11F_REV = 0x8C3B;
pub const RGB9_E5 = 0x8C3D;
pub const UNSIGNED_INT_5_9_9_9_REV = 0x8C3E;
pub const TEXTURE_SHARED_SIZE = 0x8C3F;
pub const TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH = 0x8C76;
pub const TRANSFORM_FEEDBACK_BUFFER_MODE = 0x8C7F;
pub const MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS = 0x8C80;
pub const TRANSFORM_FEEDBACK_VARYINGS = 0x8C83;
pub const TRANSFORM_FEEDBACK_BUFFER_START = 0x8C84;
pub const TRANSFORM_FEEDBACK_BUFFER_SIZE = 0x8C85;
pub const PRIMITIVES_GENERATED = 0x8C87;
pub const TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x8C88;
pub const RASTERIZER_DISCARD = 0x8C89;
pub const MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A;
pub const MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS = 0x8C8B;
pub const INTERLEAVED_ATTRIBS = 0x8C8C;
pub const SEPARATE_ATTRIBS = 0x8C8D;
pub const TRANSFORM_FEEDBACK_BUFFER = 0x8C8E;
pub const TRANSFORM_FEEDBACK_BUFFER_BINDING = 0x8C8F;
pub const RGBA32UI = 0x8D70;
pub const RGB32UI = 0x8D71;
pub const RGBA16UI = 0x8D76;
pub const RGB16UI = 0x8D77;
pub const RGBA8UI = 0x8D7C;
pub const RGB8UI = 0x8D7D;
pub const RGBA32I = 0x8D82;
pub const RGB32I = 0x8D83;
pub const RGBA16I = 0x8D88;
pub const RGB16I = 0x8D89;
pub const RGBA8I = 0x8D8E;
pub const RGB8I = 0x8D8F;
pub const RED_INTEGER = 0x8D94;
pub const GREEN_INTEGER = 0x8D95;
pub const BLUE_INTEGER = 0x8D96;
pub const RGB_INTEGER = 0x8D98;
pub const RGBA_INTEGER = 0x8D99;
pub const BGR_INTEGER = 0x8D9A;
pub const BGRA_INTEGER = 0x8D9B;
pub const SAMPLER_1D_ARRAY = 0x8DC0;
pub const SAMPLER_2D_ARRAY = 0x8DC1;
pub const SAMPLER_1D_ARRAY_SHADOW = 0x8DC3;
pub const SAMPLER_2D_ARRAY_SHADOW = 0x8DC4;
pub const SAMPLER_CUBE_SHADOW = 0x8DC5;
pub const UNSIGNED_INT_VEC2 = 0x8DC6;
pub const UNSIGNED_INT_VEC3 = 0x8DC7;
pub const UNSIGNED_INT_VEC4 = 0x8DC8;
pub const INT_SAMPLER_1D = 0x8DC9;
pub const INT_SAMPLER_2D = 0x8DCA;
pub const INT_SAMPLER_3D = 0x8DCB;
pub const INT_SAMPLER_CUBE = 0x8DCC;
pub const INT_SAMPLER_1D_ARRAY = 0x8DCE;
pub const INT_SAMPLER_2D_ARRAY = 0x8DCF;
pub const UNSIGNED_INT_SAMPLER_1D = 0x8DD1;
pub const UNSIGNED_INT_SAMPLER_2D = 0x8DD2;
pub const UNSIGNED_INT_SAMPLER_3D = 0x8DD3;
pub const UNSIGNED_INT_SAMPLER_CUBE = 0x8DD4;
pub const UNSIGNED_INT_SAMPLER_1D_ARRAY = 0x8DD6;
pub const UNSIGNED_INT_SAMPLER_2D_ARRAY = 0x8DD7;
pub const QUERY_WAIT = 0x8E13;
pub const QUERY_NO_WAIT = 0x8E14;
pub const QUERY_BY_REGION_WAIT = 0x8E15;
pub const QUERY_BY_REGION_NO_WAIT = 0x8E16;
pub const BUFFER_ACCESS_FLAGS = 0x911F;
pub const BUFFER_MAP_LENGTH = 0x9120;
pub const BUFFER_MAP_OFFSET = 0x9121;
pub const DEPTH_COMPONENT32F = 0x8CAC;
pub const DEPTH32F_STENCIL8 = 0x8CAD;
pub const FLOAT_32_UNSIGNED_INT_24_8_REV = 0x8DAD;
pub const INVALID_FRAMEBUFFER_OPERATION = 0x0506;
pub const FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING = 0x8210;
pub const FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE = 0x8211;
pub const FRAMEBUFFER_ATTACHMENT_RED_SIZE = 0x8212;
pub const FRAMEBUFFER_ATTACHMENT_GREEN_SIZE = 0x8213;
pub const FRAMEBUFFER_ATTACHMENT_BLUE_SIZE = 0x8214;
pub const FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE = 0x8215;
pub const FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE = 0x8216;
pub const FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE = 0x8217;
pub const FRAMEBUFFER_DEFAULT = 0x8218;
pub const FRAMEBUFFER_UNDEFINED = 0x8219;
pub const DEPTH_STENCIL_ATTACHMENT = 0x821A;
pub const MAX_RENDERBUFFER_SIZE = 0x84E8;
pub const DEPTH_STENCIL = 0x84F9;
pub const UNSIGNED_INT_24_8 = 0x84FA;
pub const DEPTH24_STENCIL8 = 0x88F0;
pub const TEXTURE_STENCIL_SIZE = 0x88F1;
pub const TEXTURE_RED_TYPE = 0x8C10;
pub const TEXTURE_GREEN_TYPE = 0x8C11;
pub const TEXTURE_BLUE_TYPE = 0x8C12;
pub const TEXTURE_ALPHA_TYPE = 0x8C13;
pub const TEXTURE_DEPTH_TYPE = 0x8C16;
pub const UNSIGNED_NORMALIZED = 0x8C17;
pub const FRAMEBUFFER_BINDING = 0x8CA6;
pub const DRAW_FRAMEBUFFER_BINDING = 0x8CA6;
pub const RENDERBUFFER_BINDING = 0x8CA7;
pub const READ_FRAMEBUFFER = 0x8CA8;
pub const DRAW_FRAMEBUFFER = 0x8CA9;
pub const READ_FRAMEBUFFER_BINDING = 0x8CAA;
pub const RENDERBUFFER_SAMPLES = 0x8CAB;
pub const FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
pub const FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
pub const FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
pub const FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;
pub const FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER = 0x8CD4;
pub const FRAMEBUFFER_COMPLETE = 0x8CD5;
pub const FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
pub const FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
pub const FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER = 0x8CDB;
pub const FRAMEBUFFER_INCOMPLETE_READ_BUFFER = 0x8CDC;
pub const FRAMEBUFFER_UNSUPPORTED = 0x8CDD;
pub const MAX_COLOR_ATTACHMENTS = 0x8CDF;
pub const COLOR_ATTACHMENT0 = 0x8CE0;
pub const COLOR_ATTACHMENT1 = 0x8CE1;
pub const COLOR_ATTACHMENT2 = 0x8CE2;
pub const COLOR_ATTACHMENT3 = 0x8CE3;
pub const COLOR_ATTACHMENT4 = 0x8CE4;
pub const COLOR_ATTACHMENT5 = 0x8CE5;
pub const COLOR_ATTACHMENT6 = 0x8CE6;
pub const COLOR_ATTACHMENT7 = 0x8CE7;
pub const COLOR_ATTACHMENT8 = 0x8CE8;
pub const COLOR_ATTACHMENT9 = 0x8CE9;
pub const COLOR_ATTACHMENT10 = 0x8CEA;
pub const COLOR_ATTACHMENT11 = 0x8CEB;
pub const COLOR_ATTACHMENT12 = 0x8CEC;
pub const COLOR_ATTACHMENT13 = 0x8CED;
pub const COLOR_ATTACHMENT14 = 0x8CEE;
pub const COLOR_ATTACHMENT15 = 0x8CEF;
pub const COLOR_ATTACHMENT16 = 0x8CF0;
pub const COLOR_ATTACHMENT17 = 0x8CF1;
pub const COLOR_ATTACHMENT18 = 0x8CF2;
pub const COLOR_ATTACHMENT19 = 0x8CF3;
pub const COLOR_ATTACHMENT20 = 0x8CF4;
pub const COLOR_ATTACHMENT21 = 0x8CF5;
pub const COLOR_ATTACHMENT22 = 0x8CF6;
pub const COLOR_ATTACHMENT23 = 0x8CF7;
pub const COLOR_ATTACHMENT24 = 0x8CF8;
pub const COLOR_ATTACHMENT25 = 0x8CF9;
pub const COLOR_ATTACHMENT26 = 0x8CFA;
pub const COLOR_ATTACHMENT27 = 0x8CFB;
pub const COLOR_ATTACHMENT28 = 0x8CFC;
pub const COLOR_ATTACHMENT29 = 0x8CFD;
pub const COLOR_ATTACHMENT30 = 0x8CFE;
pub const COLOR_ATTACHMENT31 = 0x8CFF;
pub const DEPTH_ATTACHMENT = 0x8D00;
pub const STENCIL_ATTACHMENT = 0x8D20;
pub const FRAMEBUFFER = 0x8D40;
pub const RENDERBUFFER = 0x8D41;
pub const RENDERBUFFER_WIDTH = 0x8D42;
pub const RENDERBUFFER_HEIGHT = 0x8D43;
pub const RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
pub const STENCIL_INDEX1 = 0x8D46;
pub const STENCIL_INDEX4 = 0x8D47;
pub const STENCIL_INDEX8 = 0x8D48;
pub const STENCIL_INDEX16 = 0x8D49;
pub const RENDERBUFFER_RED_SIZE = 0x8D50;
pub const RENDERBUFFER_GREEN_SIZE = 0x8D51;
pub const RENDERBUFFER_BLUE_SIZE = 0x8D52;
pub const RENDERBUFFER_ALPHA_SIZE = 0x8D53;
pub const RENDERBUFFER_DEPTH_SIZE = 0x8D54;
pub const RENDERBUFFER_STENCIL_SIZE = 0x8D55;
pub const FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = 0x8D56;
pub const MAX_SAMPLES = 0x8D57;
pub const FRAMEBUFFER_SRGB = 0x8DB9;
pub const HALF_FLOAT = 0x140B;
pub const MAP_READ_BIT = 0x0001;
pub const MAP_WRITE_BIT = 0x0002;
pub const MAP_INVALIDATE_RANGE_BIT = 0x0004;
pub const MAP_INVALIDATE_BUFFER_BIT = 0x0008;
pub const MAP_FLUSH_EXPLICIT_BIT = 0x0010;
pub const MAP_UNSYNCHRONIZED_BIT = 0x0020;
pub const COMPRESSED_RED_RGTC1 = 0x8DBB;
pub const COMPRESSED_SIGNED_RED_RGTC1 = 0x8DBC;
pub const COMPRESSED_RG_RGTC2 = 0x8DBD;
pub const COMPRESSED_SIGNED_RG_RGTC2 = 0x8DBE;
pub const RG = 0x8227;
pub const RG_INTEGER = 0x8228;
pub const R8 = 0x8229;
pub const R16 = 0x822A;
pub const RG8 = 0x822B;
pub const RG16 = 0x822C;
pub const R16F = 0x822D;
pub const R32F = 0x822E;
pub const RG16F = 0x822F;
pub const RG32F = 0x8230;
pub const R8I = 0x8231;
pub const R8UI = 0x8232;
pub const R16I = 0x8233;
pub const R16UI = 0x8234;
pub const R32I = 0x8235;
pub const R32UI = 0x8236;
pub const RG8I = 0x8237;
pub const RG8UI = 0x8238;
pub const RG16I = 0x8239;
pub const RG16UI = 0x823A;
pub const RG32I = 0x823B;
pub const RG32UI = 0x823C;
pub const VERTEX_ARRAY_BINDING = 0x85B5;

pub var colorMaski: *const fn (
    index: Uint,
    r: Boolean,
    g: Boolean,
    b: Boolean,
    a: Boolean,
) callconv(.C) void = undefined;
pub var getBooleani_v: *const fn (target: Enum, index: Uint, data: [*c]Boolean) callconv(.C) void = undefined;
pub var getIntegeri_v: *const fn (target: Enum, index: Uint, data: [*c]Int) callconv(.C) void = undefined;
pub var enablei: *const fn (target: Enum, index: Uint) callconv(.C) void = undefined;
pub var disablei: *const fn (target: Enum, index: Uint) callconv(.C) void = undefined;
pub var isEnabledi: *const fn (target: Enum, index: Uint) callconv(.C) Boolean = undefined;
pub var beginTransformFeedback: *const fn (primitiveMode: Enum) callconv(.C) void = undefined;
pub var endTransformFeedback: *const fn () callconv(.C) void = undefined;
pub var bindBufferRange: *const fn (
    target: Enum,
    index: Uint,
    buffer: Uint,
    offset: Intptr,
    size: Sizeiptr,
) callconv(.C) void = undefined;
pub var bindBufferBase: *const fn (target: Enum, index: Uint, buffer: Uint) callconv(.C) void = undefined;
pub var transformFeedbackVaryings: *const fn (
    program: Uint,
    count: Sizei,
    varyings: [*c]const [*c]const Char,
    bufferMode: Enum,
) callconv(.C) void = undefined;
pub var getTransformFeedbackVarying: *const fn (
    program: Uint,
    index: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    size: [*c]Sizei,
    type: [*c]Enum,
    name: [*c]Char,
) callconv(.C) void = undefined;
pub var clampColor: *const fn (target: Enum, clamp: Enum) callconv(.C) void = undefined;
pub var beginConditionalRender: *const fn (id: Uint, mode: Enum) callconv(.C) void = undefined;
pub var endConditionalRender: *const fn () callconv(.C) void = undefined;
pub var vertexAttribIPointer: *const fn (
    index: Uint,
    size: Int,
    type: Enum,
    stride: Sizei,
    pointer: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var getVertexAttribIiv: *const fn (index: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getVertexAttribIuiv: *const fn (index: Uint, pname: Enum, params: [*c]Uint) callconv(.C) void = undefined;
pub var vertexAttribI1i: *const fn (index: Uint, x: Int) callconv(.C) void = undefined;
pub var vertexAttribI2i: *const fn (index: Uint, x: Int, y: Int) callconv(.C) void = undefined;
pub var vertexAttribI3i: *const fn (index: Uint, x: Int, y: Int, z: Int) callconv(.C) void = undefined;
pub var vertexAttribI4i: *const fn (index: Uint, x: Int, y: Int, z: Int, w: Int) callconv(.C) void = undefined;
pub var vertexAttribI1ui: *const fn (index: Uint, x: Uint) callconv(.C) void = undefined;
pub var vertexAttribI2ui: *const fn (index: Uint, x: Uint, y: Uint) callconv(.C) void = undefined;
pub var vertexAttribI3ui: *const fn (index: Uint, x: Uint, y: Uint, z: Uint) callconv(.C) void = undefined;
pub var vertexAttribI4ui: *const fn (index: Uint, x: Uint, y: Uint, z: Uint, w: Uint) callconv(.C) void = undefined;
pub var vertexAttribI1iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
pub var vertexAttribI2iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
pub var vertexAttribI3iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
pub var vertexAttribI4iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
pub var vertexAttribI1uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
pub var vertexAttribI2uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
pub var vertexAttribI3uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
pub var vertexAttribI4uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
pub var vertexAttribI4bv: *const fn (index: Uint, v: [*c]const Byte) callconv(.C) void = undefined;
pub var vertexAttribI4sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
pub var vertexAttribI4ubv: *const fn (index: Uint, v: [*c]const Ubyte) callconv(.C) void = undefined;
pub var vertexAttribI4usv: *const fn (index: Uint, v: [*c]const Ushort) callconv(.C) void = undefined;
pub var getUniformuiv: *const fn (program: Uint, location: Int, params: [*c]Uint) callconv(.C) void = undefined;
pub var bindFragDataLocation: *const fn (
    program: Uint,
    color: Uint,
    name: [*c]const Char,
) callconv(.C) void = undefined;
pub var getFragDataLocation: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
pub var uniform1ui: *const fn (location: Int, v0: Uint) callconv(.C) void = undefined;
pub var uniform2ui: *const fn (location: Int, v0: Uint, v1: Uint) callconv(.C) void = undefined;
pub var uniform3ui: *const fn (location: Int, v0: Uint, v1: Uint, v2: Uint) callconv(.C) void = undefined;
pub var uniform4ui: *const fn (location: Int, v0: Uint, v1: Uint, v2: Uint, v3: Uint) callconv(.C) void = undefined;
pub var uniform1uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
pub var uniform2uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
pub var uniform3uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
pub var uniform4uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
pub var texParameterIiv: *const fn (target: Enum, pname: Enum, params: [*c]const Int) callconv(.C) void = undefined;
pub var texParameterIuiv: *const fn (
    target: Enum,
    pname: Enum,
    params: [*c]const Uint,
) callconv(.C) void = undefined;
pub var getTexParameterIiv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getTexParameterIuiv: *const fn (target: Enum, pname: Enum, params: [*c]Uint) callconv(.C) void = undefined;
pub var clearBufferiv: *const fn (buffer: Enum, drawbuffer: Int, value: [*c]const Int) callconv(.C) void = undefined;
pub var clearBufferuiv: *const fn (
    buffer: Enum,
    drawbuffer: Int,
    value: [*c]const Uint,
) callconv(.C) void = undefined;
pub var clearBufferfv: *const fn (
    buffer: Enum,
    drawbuffer: Int,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var clearBufferfi: *const fn (
    buffer: Enum,
    drawbuffer: Int,
    depth: Float,
    stencil: Int,
) callconv(.C) void = undefined;
pub var getStringi: *const fn (name: Enum, index: Uint) callconv(.C) [*c]const Ubyte = undefined;
pub var isRenderbuffer: *const fn (renderbuffer: Uint) callconv(.C) Boolean = undefined;
pub var bindRenderbuffer: *const fn (target: Enum, renderbuffer: Uint) callconv(.C) void = undefined;
pub var deleteRenderbuffers: *const fn (n: Sizei, renderbuffers: [*c]const Uint) callconv(.C) void = undefined;
pub var genRenderbuffers: *const fn (n: Sizei, renderbuffers: [*c]Uint) callconv(.C) void = undefined;
pub var renderbufferStorage: *const fn (
    target: Enum,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
pub var getRenderbufferParameteriv: *const fn (
    target: Enum,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var isFramebuffer: *const fn (framebuffer: Uint) callconv(.C) Boolean = undefined;
pub var bindFramebuffer: *const fn (target: Enum, framebuffer: Uint) callconv(.C) void = undefined;
pub var deleteFramebuffers: *const fn (n: Sizei, framebuffers: [*c]const Uint) callconv(.C) void = undefined;
pub var genFramebuffers: *const fn (n: Sizei, framebuffers: [*c]Uint) callconv(.C) void = undefined;
pub var checkFramebufferStatus: *const fn (target: Enum) callconv(.C) Enum = undefined;
pub var framebufferTexture1D: *const fn (
    target: Enum,
    attachment: Enum,
    textarget: Enum,
    texture: Uint,
    level: Int,
) callconv(.C) void = undefined;
pub var framebufferTexture2D: *const fn (
    target: Enum,
    attachment: Enum,
    textarget: Enum,
    texture: Uint,
    level: Int,
) callconv(.C) void = undefined;
pub var framebufferTexture3D: *const fn (
    target: Enum,
    attachment: Enum,
    textarget: Enum,
    texture: Uint,
    level: Int,
    zoffset: Int,
) callconv(.C) void = undefined;
pub var framebufferRenderbuffer: *const fn (
    target: Enum,
    attachment: Enum,
    renderbuffertarget: Enum,
    renderbuffer: Uint,
) callconv(.C) void = undefined;
pub var getFramebufferAttachmentParameteriv: *const fn (
    target: Enum,
    attachment: Enum,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var generateMipmap: *const fn (target: Enum) callconv(.C) void = undefined;
pub var blitFramebuffer: *const fn (
    srcX0: Int,
    srcY0: Int,
    srcX1: Int,
    srcY1: Int,
    dstX0: Int,
    dstY0: Int,
    dstX1: Int,
    dstY1: Int,
    mask: Bitfield,
    filter: Enum,
) callconv(.C) void = undefined;
pub var renderbufferStorageMultisample: *const fn (
    target: Enum,
    samples: Sizei,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
pub var framebufferTextureLayer: *const fn (
    target: Enum,
    attachment: Enum,
    texture: Uint,
    level: Int,
    layer: Int,
) callconv(.C) void = undefined;
pub var mapBufferRange: *const fn (
    target: Enum,
    offset: Intptr,
    length: Sizeiptr,
    access: Bitfield,
) callconv(.C) ?*anyopaque = undefined;
pub var flushMappedBufferRange: *const fn (
    target: Enum,
    offset: Intptr,
    length: Sizeiptr,
) callconv(.C) void = undefined;
pub var bindVertexArray: *const fn (array: Uint) callconv(.C) void = undefined;
pub var deleteVertexArrays: *const fn (n: Sizei, arrays: [*c]const Uint) callconv(.C) void = undefined;
pub var genVertexArrays: *const fn (n: Sizei, arrays: [*c]Uint) callconv(.C) void = undefined;
pub var isVertexArray: *const fn (array: Uint) callconv(.C) Boolean = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 3.1 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const SAMPLER_2D_RECT = 0x8B63;
pub const SAMPLER_2D_RECT_SHADOW = 0x8B64;
pub const SAMPLER_BUFFER = 0x8DC2;
pub const INT_SAMPLER_2D_RECT = 0x8DCD;
pub const INT_SAMPLER_BUFFER = 0x8DD0;
pub const UNSIGNED_INT_SAMPLER_2D_RECT = 0x8DD5;
pub const UNSIGNED_INT_SAMPLER_BUFFER = 0x8DD8;
pub const TEXTURE_BUFFER = 0x8C2A;
pub const MAX_TEXTURE_BUFFER_SIZE = 0x8C2B;
pub const TEXTURE_BINDING_BUFFER = 0x8C2C;
pub const TEXTURE_BUFFER_DATA_STORE_BINDING = 0x8C2D;
pub const TEXTURE_RECTANGLE = 0x84F5;
pub const TEXTURE_BINDING_RECTANGLE = 0x84F6;
pub const PROXY_TEXTURE_RECTANGLE = 0x84F7;
pub const MAX_RECTANGLE_TEXTURE_SIZE = 0x84F8;
pub const R8_SNORM = 0x8F94;
pub const RG8_SNORM = 0x8F95;
pub const RGB8_SNORM = 0x8F96;
pub const RGBA8_SNORM = 0x8F97;
pub const R16_SNORM = 0x8F98;
pub const RG16_SNORM = 0x8F99;
pub const RGB16_SNORM = 0x8F9A;
pub const RGBA16_SNORM = 0x8F9B;
pub const SIGNED_NORMALIZED = 0x8F9C;
pub const PRIMITIVE_RESTART = 0x8F9D;
pub const PRIMITIVE_RESTART_INDEX = 0x8F9E;
pub const COPY_READ_BUFFER = 0x8F36;
pub const COPY_WRITE_BUFFER = 0x8F37;
pub const UNIFORM_BUFFER = 0x8A11;
pub const UNIFORM_BUFFER_BINDING = 0x8A28;
pub const UNIFORM_BUFFER_START = 0x8A29;
pub const UNIFORM_BUFFER_SIZE = 0x8A2A;
pub const MAX_VERTEX_UNIFORM_BLOCKS = 0x8A2B;
pub const MAX_GEOMETRY_UNIFORM_BLOCKS = 0x8A2C;
pub const MAX_FRAGMENT_UNIFORM_BLOCKS = 0x8A2D;
pub const MAX_COMBINED_UNIFORM_BLOCKS = 0x8A2E;
pub const MAX_UNIFORM_BUFFER_BINDINGS = 0x8A2F;
pub const MAX_UNIFORM_BLOCK_SIZE = 0x8A30;
pub const MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS = 0x8A31;
pub const MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS = 0x8A32;
pub const MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = 0x8A33;
pub const UNIFORM_BUFFER_OFFSET_ALIGNMENT = 0x8A34;
pub const ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH = 0x8A35;
pub const ACTIVE_UNIFORM_BLOCKS = 0x8A36;
pub const UNIFORM_TYPE = 0x8A37;
pub const UNIFORM_SIZE = 0x8A38;
pub const UNIFORM_NAME_LENGTH = 0x8A39;
pub const UNIFORM_BLOCK_INDEX = 0x8A3A;
pub const UNIFORM_OFFSET = 0x8A3B;
pub const UNIFORM_ARRAY_STRIDE = 0x8A3C;
pub const UNIFORM_MATRIX_STRIDE = 0x8A3D;
pub const UNIFORM_IS_ROW_MAJOR = 0x8A3E;
pub const UNIFORM_BLOCK_BINDING = 0x8A3F;
pub const UNIFORM_BLOCK_DATA_SIZE = 0x8A40;
pub const UNIFORM_BLOCK_NAME_LENGTH = 0x8A41;
pub const UNIFORM_BLOCK_ACTIVE_UNIFORMS = 0x8A42;
pub const UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES = 0x8A43;
pub const UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER = 0x8A44;
pub const UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER = 0x8A45;
pub const UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER = 0x8A46;
pub const INVALID_INDEX = 0xFFFFFFFF;

pub var drawArraysInstanced: *const fn (
    mode: Enum,
    first: Int,
    count: Sizei,
    instancecount: Sizei,
) callconv(.C) void = undefined;
pub var drawElementsInstanced: *const fn (
    mode: Enum,
    count: Sizei,
    type: Enum,
    indices: ?*const anyopaque,
    instancecount: Sizei,
) callconv(.C) void = undefined;
pub var texBuffer: *const fn (target: Enum, internalformat: Enum, buffer: Uint) callconv(.C) void = undefined;
pub var primitiveRestartIndex: *const fn (index: Uint) callconv(.C) void = undefined;
pub var copyBufferSubData: *const fn (
    readTarget: Enum,
    writeTarget: Enum,
    readOffset: Intptr,
    writeOffset: Intptr,
    size: Sizeiptr,
) callconv(.C) void = undefined;
pub var getUniformIndices: *const fn (
    program: Uint,
    uniformCount: Sizei,
    uniformNames: [*c]const [*c]const Char,
    uniformIndices: [*c]Uint,
) callconv(.C) void = undefined;
pub var getActiveUniformsiv: *const fn (
    program: Uint,
    uniformCount: Sizei,
    uniformIndices: [*c]const Uint,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var getActiveUniformName: *const fn (
    program: Uint,
    uniformIndex: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    uniformName: [*c]Char,
) callconv(.C) void = undefined;
pub var getUniformBlockIndex: *const fn (
    program: Uint,
    uniformBlockName: [*c]const Char,
) callconv(.C) Uint = undefined;
pub var getActiveUniformBlockiv: *const fn (
    program: Uint,
    uniformBlockIndex: Uint,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var getActiveUniformBlockName: *const fn (
    program: Uint,
    uniformBlockIndex: Uint,
    bufSize: Sizei,
    length: [*c]Sizei,
    uniformBlockName: [*c]Char,
) callconv(.C) void = undefined;
pub var uniformBlockBinding: *const fn (
    program: Uint,
    uniformBlockIndex: Uint,
    uniformBlockBinding: Uint,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 3.2 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const Sync = *opaque {};
pub const Uint64 = u64;
pub const Int64 = i64;

pub const CONTEXT_CORE_PROFILE_BIT = 0x00000001;
pub const CONTEXT_COMPATIBILITY_PROFILE_BIT = 0x00000002;
pub const LINES_ADJACENCY = 0x000A;
pub const LINE_STRIP_ADJACENCY = 0x000B;
pub const TRIANGLES_ADJACENCY = 0x000C;
pub const TRIANGLE_STRIP_ADJACENCY = 0x000D;
pub const PROGRAM_POINT_SIZE = 0x8642;
pub const MAX_GEOMETRY_TEXTURE_IMAGE_UNITS = 0x8C29;
pub const FRAMEBUFFER_ATTACHMENT_LAYERED = 0x8DA7;
pub const FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS = 0x8DA8;
pub const GEOMETRY_SHADER = 0x8DD9;
pub const GEOMETRY_VERTICES_OUT = 0x8916;
pub const GEOMETRY_INPUT_TYPE = 0x8917;
pub const GEOMETRY_OUTPUT_TYPE = 0x8918;
pub const MAX_GEOMETRY_UNIFORM_COMPONENTS = 0x8DDF;
pub const MAX_GEOMETRY_OUTPUT_VERTICES = 0x8DE0;
pub const MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS = 0x8DE1;
pub const MAX_VERTEX_OUTPUT_COMPONENTS = 0x9122;
pub const MAX_GEOMETRY_INPUT_COMPONENTS = 0x9123;
pub const MAX_GEOMETRY_OUTPUT_COMPONENTS = 0x9124;
pub const MAX_FRAGMENT_INPUT_COMPONENTS = 0x9125;
pub const CONTEXT_PROFILE_MASK = 0x9126;
pub const DEPTH_CLAMP = 0x864F;
pub const QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION = 0x8E4C;
pub const FIRST_VERTEX_CONVENTION = 0x8E4D;
pub const LAST_VERTEX_CONVENTION = 0x8E4E;
pub const PROVOKING_VERTEX = 0x8E4F;
pub const TEXTURE_CUBE_MAP_SEAMLESS = 0x884F;
pub const MAX_SERVER_WAIT_TIMEOUT = 0x9111;
pub const OBJECT_TYPE = 0x9112;
pub const SYNC_CONDITION = 0x9113;
pub const SYNC_STATUS = 0x9114;
pub const SYNC_FLAGS = 0x9115;
pub const SYNC_FENCE = 0x9116;
pub const SYNC_GPU_COMMANDS_COMPLETE = 0x9117;
pub const UNSIGNALED = 0x9118;
pub const SIGNALED = 0x9119;
pub const ALREADY_SIGNALED = 0x911A;
pub const TIMEOUT_EXPIRED = 0x911B;
pub const CONDITION_SATISFIED = 0x911C;
pub const WAIT_FAILED = 0x911D;
pub const TIMEOUT_IGNORED = 0xFFFFFFFFFFFFFFFF;
pub const SYNC_FLUSH_COMMANDS_BIT = 0x00000001;
pub const SAMPLE_POSITION = 0x8E50;
pub const SAMPLE_MASK = 0x8E51;
pub const SAMPLE_MASK_VALUE = 0x8E52;
pub const MAX_SAMPLE_MASK_WORDS = 0x8E59;
pub const TEXTURE_2D_MULTISAMPLE = 0x9100;
pub const PROXY_TEXTURE_2D_MULTISAMPLE = 0x9101;
pub const TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9102;
pub const PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9103;
pub const TEXTURE_BINDING_2D_MULTISAMPLE = 0x9104;
pub const TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY = 0x9105;
pub const TEXTURE_SAMPLES = 0x9106;
pub const TEXTURE_FIXED_SAMPLE_LOCATIONS = 0x9107;
pub const SAMPLER_2D_MULTISAMPLE = 0x9108;
pub const INT_SAMPLER_2D_MULTISAMPLE = 0x9109;
pub const UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE = 0x910A;
pub const SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910B;
pub const INT_SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910C;
pub const UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910D;
pub const MAX_COLOR_TEXTURE_SAMPLES = 0x910E;
pub const MAX_DEPTH_TEXTURE_SAMPLES = 0x910F;
pub const MAX_INTEGER_SAMPLES = 0x9110;

pub var drawElementsBaseVertex: *const fn (
    mode: Enum,
    count: Sizei,
    type: Enum,
    indices: ?*const anyopaque,
    basevertex: Int,
) callconv(.C) void = undefined;
pub var drawRangeElementsBaseVertex: *const fn (
    mode: Enum,
    start: Uint,
    end: Uint,
    count: Sizei,
    type: Enum,
    indices: ?*const anyopaque,
    basevertex: Int,
) callconv(.C) void = undefined;
pub var drawElementsInstancedBaseVertex: *const fn (
    mode: Enum,
    count: Sizei,
    type: Enum,
    indices: ?*const anyopaque,
    instancecount: Sizei,
    basevertex: Int,
) callconv(.C) void = undefined;
pub var multiDrawElementsBaseVertex: *const fn (
    mode: Enum,
    count: [*c]const Sizei,
    type: Enum,
    indices: [*c]const ?*const anyopaque,
    drawcount: Sizei,
    basevertex: [*c]const Int,
) callconv(.C) void = undefined;
pub var provokingVertex: *const fn (mode: Enum) callconv(.C) void = undefined;
pub var fenceSync: *const fn (condition: Enum, flags: Bitfield) callconv(.C) Sync = undefined;
pub var isSync: *const fn (sync: Sync) callconv(.C) Boolean = undefined;
pub var deleteSync: *const fn (sync: Sync) callconv(.C) void = undefined;
pub var clientWaitSync: *const fn (sync: Sync, flags: Bitfield, timeout: Uint64) callconv(.C) Enum = undefined;
pub var waitSync: *const fn (sync: Sync, flags: Bitfield, timeout: Uint64) callconv(.C) void = undefined;
pub var getInteger64v: *const fn (pname: Enum, data: [*c]Int64) callconv(.C) void = undefined;
pub var getSynciv: *const fn (
    sync: Sync,
    pname: Enum,
    count: Sizei,
    length: [*c]Sizei,
    values: [*c]Int,
) callconv(.C) void = undefined;
pub var getInteger64i_v: *const fn (target: Enum, index: Uint, data: [*c]Int64) callconv(.C) void = undefined;
pub var getBufferParameteri64v: *const fn (
    target: Enum,
    pname: Enum,
    params: [*c]Int64,
) callconv(.C) void = undefined;
pub var framebufferTexture: *const fn (
    target: Enum,
    attachment: Enum,
    texture: Uint,
    level: Int,
) callconv(.C) void = undefined;
pub var texImage2DMultisample: *const fn (
    target: Enum,
    samples: Sizei,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    fixedsamplelocations: Boolean,
) callconv(.C) void = undefined;
pub var texImage3DMultisample: *const fn (
    target: Enum,
    samples: Sizei,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    depth: Sizei,
    fixedsamplelocations: Boolean,
) callconv(.C) void = undefined;
pub var getMultisamplefv: *const fn (pname: Enum, index: Uint, val: [*c]Float) callconv(.C) void = undefined;
pub var sampleMaski: *const fn (maskNumber: Uint, mask: Bitfield) callconv(.C) void = undefined;

//--------------------------------------------------------------------------------------------------
//
// OpenGL 3.3 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const VERTEX_ATTRIB_ARRAY_DIVISOR = 0x88FE;
pub const SRC1_COLOR = 0x88F9;
pub const ONE_MINUS_SRC1_COLOR = 0x88FA;
pub const ONE_MINUS_SRC1_ALPHA = 0x88FB;
pub const MAX_DUAL_SOURCE_DRAW_BUFFERS = 0x88FC;
pub const ANY_SAMPLES_PASSED = 0x8C2F;
pub const SAMPLER_BINDING = 0x8919;
pub const RGB10_A2UI = 0x906F;
pub const TEXTURE_SWIZZLE_R = 0x8E42;
pub const TEXTURE_SWIZZLE_G = 0x8E43;
pub const TEXTURE_SWIZZLE_B = 0x8E44;
pub const TEXTURE_SWIZZLE_A = 0x8E45;
pub const TEXTURE_SWIZZLE_RGBA = 0x8E46;
pub const TIME_ELAPSED = 0x88BF;
pub const TIMESTAMP = 0x8E28;
pub const INT_2_10_10_10_REV = 0x8D9F;

pub var bindFragDataLocationIndexed: *const fn (
    program: Uint,
    colorNumber: Uint,
    index: Uint,
    name: [*:0]const Char,
) callconv(.C) void = undefined;
pub var getFragDataIndex: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
pub var genSamplers: *const fn (count: Sizei, samplers: [*c]Uint) callconv(.C) void = undefined;
pub var deleteSamplers: *const fn (count: Sizei, samplers: [*c]const Uint) callconv(.C) void = undefined;
pub var isSampler: *const fn (sampler: Uint) callconv(.C) Boolean = undefined;
pub var bindSampler: *const fn (unit: Uint, sampler: Uint) callconv(.C) void = undefined;
pub var samplerParameteri: *const fn (sampler: Uint, pname: Enum, param: Int) callconv(.C) void = undefined;
pub var samplerParameteriv: *const fn (
    sampler: Uint,
    pname: Enum,
    param: [*c]const Int,
) callconv(.C) void = undefined;
pub var samplerParameterf: *const fn (sampler: Uint, pname: Enum, param: Float) callconv(.C) void = undefined;
pub var samplerParameterfv: *const fn (
    sampler: Uint,
    pname: Enum,
    param: [*c]const Float,
) callconv(.C) void = undefined;
pub var samplerParameterIiv: *const fn (
    sampler: Uint,
    pname: Enum,
    param: [*c]const Int,
) callconv(.C) void = undefined;
pub var samplerParameterIuiv: *const fn (
    sampler: Uint,
    pname: Enum,
    param: [*c]const Uint,
) callconv(.C) void = undefined;
pub var getSamplerParameteriv: *const fn (
    sampler: Uint,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var getSamplerParameterIiv: *const fn (
    sampler: Uint,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var getSamplerParameterfv: *const fn (
    sampler: Uint,
    pname: Enum,
    params: [*c]Float,
) callconv(.C) void = undefined;
pub var getSamplerParameterIuiv: *const fn (
    sampler: Uint,
    pname: Enum,
    params: [*c]Uint,
) callconv(.C) void = undefined;
pub var queryCounter: *const fn (id: Uint, target: Enum) callconv(.C) void = undefined;
pub var getQueryObjecti64v: *const fn (id: Uint, pname: Enum, params: [*c]Int64) callconv(.C) void = undefined;
pub var getQueryObjectui64v: *const fn (id: Uint, pname: Enum, params: [*c]Uint64) callconv(.C) void = undefined;
pub var vertexAttribDivisor: *const fn (index: Uint, divisor: Uint) callconv(.C) void = undefined;
pub var vertexAttribP1ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
pub var vertexAttribP1uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;
pub var vertexAttribP2ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
pub var vertexAttribP2uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;
pub var vertexAttribP3ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
pub var vertexAttribP3uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;
pub var vertexAttribP4ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
pub var vertexAttribP4uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;

// TODO: Where do these belong?
// pub var vertexP2ui: *const fn (type: Enum, value: Uint) callconv(.C) void = undefined;
// pub var vertexP2uiv: *const fn (type: Enum, value: *const Uint) callconv(.C) void = undefined;
// pub var vertexP3ui: *const fn (type: Enum, value: Uint) callconv(.C) void = undefined;
// pub var vertexP3uiv: *const fn (type: Enum, value: *const Uint) callconv(.C) void = undefined;
// pub var vertexP4ui: *const fn (type: Enum, value: Uint) callconv(.C) void = undefined;
// pub var vertexP4uiv: *const fn (type: Enum, value: *const Uint) callconv(.C) void = undefined;
// pub var texCoordP1ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var texCoordP1uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var texCoordP2ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var texCoordP2uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var texCoordP3ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var texCoordP3uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var texCoordP4ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var texCoordP4uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP1ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP1uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP2ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP2uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP3ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP3uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP4ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var multiTexCoordP4uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var normalP3ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
// pub var normalP3uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
// pub var colorP3ui: *const fn (type: Enum, color: Uint) callconv(.C) void = undefined;
// pub var colorP3uiv: *const fn (type: Enum, color: *const Uint) callconv(.C) void = undefined;
// pub var colorP4ui: *const fn (type: Enum, color: Uint) callconv(.C) void = undefined;
// pub var colorP4uiv: *const fn (type: Enum, color: *const Uint) callconv(.C) void = undefined;
// pub var secondaryColorP3ui: *const fn (type: Enum, color: Uint) callconv(.C) void = undefined;
// pub var secondaryColorP3uiv: *const fn (type: Enum, color: *const Uint) callconv(.C) void = undefined;

//--------------------------------------------------------------------------------------------------
//
// OpenGL 4.0 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const SAMPLE_SHADING = 0x8C36;
pub const MIN_SAMPLE_SHADING_VALUE = 0x8C37;
pub const MIN_PROGRAM_TEXTURE_GATHER_OFFSET = 0x8E5E;
pub const MAX_PROGRAM_TEXTURE_GATHER_OFFSET = 0x8E5F;
pub const TEXTURE_CUBE_MAP_ARRAY = 0x9009;
pub const TEXTURE_BINDING_CUBE_MAP_ARRAY = 0x900A;
pub const PROXY_TEXTURE_CUBE_MAP_ARRAY = 0x900B;
pub const SAMPLER_CUBE_MAP_ARRAY = 0x900C;
pub const SAMPLER_CUBE_MAP_ARRAY_SHADOW = 0x900D;
pub const INT_SAMPLER_CUBE_MAP_ARRAY = 0x900E;
pub const UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY = 0x900F;
pub const DRAW_INDIRECT_BUFFER = 0x8F3F;
pub const DRAW_INDIRECT_BUFFER_BINDING = 0x8F43;
pub const GEOMETRY_SHADER_INVOCATIONS = 0x887F;
pub const MAX_GEOMETRY_SHADER_INVOCATIONS = 0x8E5A;
pub const MIN_FRAGMENT_INTERPOLATION_OFFSET = 0x8E5B;
pub const MAX_FRAGMENT_INTERPOLATION_OFFSET = 0x8E5C;
pub const FRAGMENT_INTERPOLATION_OFFSET_BITS = 0x8E5D;
pub const DOUBLE_VEC2 = 0x8FFC;
pub const DOUBLE_VEC3 = 0x8FFD;
pub const DOUBLE_VEC4 = 0x8FFE;
pub const DOUBLE_MAT2 = 0x8F46;
pub const DOUBLE_MAT3 = 0x8F47;
pub const DOUBLE_MAT4 = 0x8F48;
pub const DOUBLE_MAT2x3 = 0x8F49;
pub const DOUBLE_MAT2x4 = 0x8F4A;
pub const DOUBLE_MAT3x2 = 0x8F4B;
pub const DOUBLE_MAT3x4 = 0x8F4C;
pub const DOUBLE_MAT4x2 = 0x8F4D;
pub const DOUBLE_MAT4x3 = 0x8F4E;
pub const ACTIVE_SUBROUTINES = 0x8DE5;
pub const ACTIVE_SUBROUTINE_UNIFORMS = 0x8DE6;
pub const ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS = 0x8E47;
pub const ACTIVE_SUBROUTINE_MAX_LENGTH = 0x8E48;
pub const ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH = 0x8E49;
pub const MAX_SUBROUTINES = 0x8DE7;
pub const MAX_SUBROUTINE_UNIFORM_LOCATIONS = 0x8DE8;
pub const NUM_COMPATIBLE_SUBROUTINES = 0x8E4A;
pub const COMPATIBLE_SUBROUTINES = 0x8E4B;
pub const PATCHES = 0x000E;
pub const PATCH_VERTICES = 0x8E72;
pub const PATCH_DEFAULT_INNER_LEVEL = 0x8E73;
pub const PATCH_DEFAULT_OUTER_LEVEL = 0x8E74;
pub const TESS_CONTROL_OUTPUT_VERTICES = 0x8E75;
pub const TESS_GEN_MODE = 0x8E76;
pub const TESS_GEN_SPACING = 0x8E77;
pub const TESS_GEN_VERTEX_ORDER = 0x8E78;
pub const TESS_GEN_POINT_MODE = 0x8E79;
pub const ISOLINES = 0x8E7A;
pub const FRACTIONAL_ODD = 0x8E7B;
pub const FRACTIONAL_EVEN = 0x8E7C;
pub const MAX_PATCH_VERTICES = 0x8E7D;
pub const MAX_TESS_GEN_LEVEL = 0x8E7E;
pub const MAX_TESS_CONTROL_UNIFORM_COMPONENTS = 0x8E7F;
pub const MAX_TESS_EVALUATION_UNIFORM_COMPONENTS = 0x8E80;
pub const MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS = 0x8E81;
pub const MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS = 0x8E82;
pub const MAX_TESS_CONTROL_OUTPUT_COMPONENTS = 0x8E83;
pub const MAX_TESS_PATCH_COMPONENTS = 0x8E84;
pub const MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS = 0x8E85;
pub const MAX_TESS_EVALUATION_OUTPUT_COMPONENTS = 0x8E86;
pub const MAX_TESS_CONTROL_UNIFORM_BLOCKS = 0x8E89;
pub const MAX_TESS_EVALUATION_UNIFORM_BLOCKS = 0x8E8A;
pub const MAX_TESS_CONTROL_INPUT_COMPONENTS = 0x886C;
pub const MAX_TESS_EVALUATION_INPUT_COMPONENTS = 0x886D;
pub const MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS = 0x8E1E;
pub const MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS = 0x8E1F;
pub const UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER = 0x84F0;
pub const UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x84F1;
pub const TESS_EVALUATION_SHADER = 0x8E87;
pub const TESS_CONTROL_SHADER = 0x8E88;
pub const TRANSFORM_FEEDBACK = 0x8E22;
pub const TRANSFORM_FEEDBACK_BUFFER_PAUSED = 0x8E23;
pub const TRANSFORM_FEEDBACK_BUFFER_ACTIVE = 0x8E24;
pub const TRANSFORM_FEEDBACK_BINDING = 0x8E25;
pub const MAX_TRANSFORM_FEEDBACK_BUFFERS = 0x8E70;
pub const MAX_VERTEX_STREAMS = 0x8E71;

pub const DrawArraysIndirectCommand = extern struct {
    count: Uint,
    instance_count: Uint,
    first: Uint,
    /// base_instance should always be set to zero for GL versions < 4.2
    base_instance: Uint = 0,
};

pub const DrawElementsIndirectCommand = extern struct {
    count: Uint,
    instance_count: Uint,
    first_index: Uint,
    base_vertex: Int,
    /// base_instance should always be set to zero for GL versions < 4.2
    base_instance: Uint = 0,
};

pub var minSampleShading: *const fn (value: Float) callconv(.C) void = undefined;
pub var blendEquationi: *const fn (buf: Uint, mode: Enum) callconv(.C) void = undefined;
pub var blendEquationSeparatei: *const fn (buf: Uint, modeRGB: Enum, modeAlpha: Enum) callconv(.C) void = undefined;
pub var blendFunci: *const fn (buf: Uint, src: Enum, dst: Enum) callconv(.C) void = undefined;
pub var blendFuncSeparatei: *const fn (buf: Uint, srcRGB: Enum, dstRGB: Enum, srcAlpha: Enum, dstAlpha: Enum) callconv(.C) void = undefined;
pub var drawArraysIndirect: *const fn (mode: Enum, indirect: *const DrawArraysIndirectCommand) callconv(.C) void = undefined;
pub var drawElementsIndirect: *const fn (mode: Enum, type: Enum, indirect: *const DrawElementsIndirectCommand) callconv(.C) void = undefined;
pub var uniform1d: *const fn (location: Int, x: Double) callconv(.C) void = undefined;
pub var uniform2d: *const fn (location: Int, x: Double, y: Double) callconv(.C) void = undefined;
pub var uniform3d: *const fn (location: Int, x: Double, y: Double, z: Double) callconv(.C) void = undefined;
pub var uniform4d: *const fn (location: Int, x: Double, y: Double, z: Double, w: Double) callconv(.C) void = undefined;
pub var uniform1dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniform2dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniform3dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniform4dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix2dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix3dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix4dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix2x3dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix2x4dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix3x2dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix3x4dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix4x2dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var uniformMatrix4x3dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
pub var getUniformdv: *const fn (program: Uint, location: Int, params: [*c]Double) callconv(.C) void = undefined;
pub var getSubroutineUniformLocation: *const fn (program: Uint, shadertype: Enum, name: [*c]const Char) callconv(.C) Int = undefined;
pub var getSubroutineIndex: *const fn (program: Uint, shadertype: Enum, name: [*c]const Char) callconv(.C) Uint = undefined;
pub var getActiveSubroutineUniformiv: *const fn (program: Uint, shadertype: Enum, index: Uint, pname: Enum, values: [*c]Int) callconv(.C) void = undefined;
pub var getActiveSubroutineUniformName: *const fn (program: Uint, shadertype: Enum, index: Uint, bufsize: Sizei, length: [*c]Sizei, name: [*c]Char) callconv(.C) void = undefined;
pub var getActiveSubroutineName: *const fn (program: Uint, shadertype: Enum, index: Uint, bufsize: Sizei, length: [*c]Sizei, name: [*c]Char) callconv(.C) void = undefined;
pub var uniformSubroutinesuiv: *const fn (shadertype: Enum, count: Sizei, indices: [*c]const Uint) callconv(.C) void = undefined;
pub var getUniformSubroutineuiv: *const fn (shadertype: Enum, location: Int, params: [*c]const Uint) callconv(.C) void = undefined;
pub var getProgramStageiv: *const fn (program: Uint, shadertype: Enum, pname: Enum, values: [*c]Int) callconv(.C) void = undefined;
pub var patchParameteri: *const fn (pname: Enum, value: Int) callconv(.C) void = undefined;
pub var patchParameterfv: *const fn (pname: Enum, values: [*c]const Float) callconv(.C) void = undefined;
pub var bindTransformFeedback: *const fn (target: Enum, id: Uint) callconv(.C) Boolean = undefined;
pub var deleteTransformFeedbacks: *const fn (n: Sizei, ids: [*c]const Uint) callconv(.C) void = undefined;
pub var genTransformFeedbacks: *const fn (n: Sizei, ids: [*c]Uint) callconv(.C) void = undefined;
pub var isTransformFeedback: *const fn (id: Uint) callconv(.C) void = undefined;
pub var pauseTransformFeedback: *const fn () callconv(.C) void = undefined;
pub var resumeTransformFeedback: *const fn () callconv(.C) void = undefined;
pub var drawTransformFeedback: *const fn (mode: Enum, id: Uint) callconv(.C) void = undefined;
pub var drawTransformFeedbackStream: *const fn (mode: Enum, id: Uint, stream: Uint) callconv(.C) void = undefined;
pub var beginQueryIndexed: *const fn (target: Enum, index: Uint, id: Uint) callconv(.C) void = undefined;
pub var endQueryIndexed: *const fn (target: Enum, index: Uint) callconv(.C) void = undefined;
pub var glGetQueryIndexediv: *const fn (target: Enum, index: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;

//--------------------------------------------------------------------------------------------------
//
// OpenGL 4.1 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const FIXED = 0x140C;
pub const IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
pub const IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;
pub const LOW_FLOAT = 0x8DF0;
pub const MEDIUM_FLOAT = 0x8DF1;
pub const HIGH_FLOAT = 0x8DF2;
pub const LOW_INT = 0x8DF3;
pub const MEDIUM_INT = 0x8DF4;
pub const HIGH_INT = 0x8DF5;
pub const SHADER_COMPILER = 0x8DFA;
pub const SHADER_BINARY_FORMATS = 0x8DF8;
pub const NUM_SHADER_BINARY_FORMATS = 0x8DF9;
pub const MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
pub const MAX_VARYING_VECTORS = 0x8DFC;
pub const MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
pub const RGB565 = 0x8D62;
pub const PROGRAM_BINARY_RETRIEVABLE_HINT = 0x8257;
pub const PROGRAM_BINARY_LENGTH = 0x8741;
pub const NUM_PROGRAM_BINARY_FORMATS = 0x87FE;
pub const PROGRAM_BINARY_FORMATS = 0x87FF;
pub const VERTEX_SHADER_BIT = 0x00000001;
pub const FRAGMENT_SHADER_BIT = 0x00000002;
pub const GEOMETRY_SHADER_BIT = 0x00000004;
pub const TESS_CONTROL_SHADER_BIT = 0x00000008;
pub const TESS_EVALUATION_SHADER_BIT = 0x00000010;
pub const ALL_SHADER_BITS = 0xFFFFFFFF;
pub const PROGRAM_SEPARABLE = 0x8258;
pub const ACTIVE_PROGRAM = 0x8259;
pub const PROGRAM_PIPELINE_BINDING = 0x825A;
pub const MAX_VIEWPORTS = 0x825B;
pub const VIEWPORT_SUBPIXEL_BITS = 0x825C;
pub const VIEWPORT_BOUNDS_RANGE = 0x825D;
pub const LAYER_PROVOKING_VERTEX = 0x825E;
pub const VIEWPORT_INDEX_PROVOKING_VERTEX = 0x825F;
pub const UNDEFINED_VERTEX = 0x8260;

pub var releaseShaderCompiler: *const fn () callconv(.C) void = undefined;
pub var shaderBinary: *const fn (
    count: Sizei,
    shaders: [*]const Uint,
    binary_format: Enum,
    binary: *const anyopaque,
    length: Sizei,
) callconv(.C) void = undefined;
pub var getShaderPrecisionFormat: *const fn (
    shader_type: Enum,
    precisionType: Enum,
    range: *Int,
    precision: *Int,
) callconv(.C) void = undefined;
// depthRangef first defined by OpenGL ES 1.0
// clearDepthf first defined by OpenGL ES 1.0
pub var getProgramBinary: *const fn (
    program: Uint,
    buf_size: Sizei,
    length: *Sizei,
    binary_format: *Enum,
    binary: *anyopaque,
) callconv(.C) void = undefined;
pub var programBinary: *const fn (
    program: Uint,
    binary_format: Enum,
    binary: *const anyopaque,
    length: Sizei,
) callconv(.C) void = undefined;
pub var programParameteri: *const fn (
    program: Uint,
    pname: Enum,
    value: Int,
) callconv(.C) void = undefined;
pub var useProgramStages: *const fn (
    pipeline: Uint,
    stages: Bitfield,
    program: Uint,
) callconv(.C) void = undefined;
pub var activeShaderProgram: *const fn (
    pipeline: Uint,
    program: Uint,
) callconv(.C) void = undefined;
pub var createShaderProgramv: *const fn (
    type: Enum,
    count: Sizei,
    strings: [*c]const [*c]const Char,
) callconv(.C) Uint = undefined;
pub var bindProgramPipeline: *const fn (pipeline: Uint) callconv(.C) void = undefined;
pub var deleteProgramPipelines: *const fn (
    n: Sizei,
    pipelines: [*]const Uint,
) callconv(.C) void = undefined;
pub var genProgramPipelines: *const fn (n: Sizei, pipelines: [*]Uint) callconv(.C) void = undefined;
pub var isProgramPipeline: *const fn (pipeline: Uint) callconv(.C) Boolean = undefined;
pub var getProgramPipelineiv: *const fn (
    pipeline: Uint,
    pname: Enum,
    params: [*]Int,
) callconv(.C) void = undefined;
pub var programUniform1i: *const fn (
    program: Uint,
    location: Int,
    x: Int,
) callconv(.C) void = undefined;
pub var programUniform2i: *const fn (
    program: Uint,
    location: Int,
    x: Int,
    y: Int,
) callconv(.C) void = undefined;
pub var programUniform3i: *const fn (
    program: Uint,
    location: Int,
    x: Int,
    y: Int,
    z: Int,
) callconv(.C) void = undefined;
pub var programUniform4i: *const fn (
    program: Uint,
    location: Int,
    x: Int,
    y: Int,
    z: Int,
    w: Int,
) callconv(.C) void = undefined;
pub var programUniform1ui: *const fn (
    program: Uint,
    location: Int,
    x: Uint,
) callconv(.C) void = undefined;
pub var programUniform2ui: *const fn (
    program: Uint,
    location: Int,
    x: Uint,
    y: Uint,
) callconv(.C) void = undefined;
pub var programUniform3ui: *const fn (
    program: Uint,
    location: Int,
    x: Uint,
    y: Uint,
    z: Uint,
) callconv(.C) void = undefined;
pub var programUniform4ui: *const fn (
    program: Uint,
    location: Int,
    x: Uint,
    y: Uint,
    z: Uint,
    w: Uint,
) callconv(.C) void = undefined;
pub var programUniform1f: *const fn (
    program: Uint,
    location: Int,
    x: Float,
) callconv(.C) void = undefined;
pub var programUniform2f: *const fn (
    program: Uint,
    location: Int,
    x: Float,
    y: Float,
) callconv(.C) void = undefined;
pub var programUniform3f: *const fn (
    program: Uint,
    location: Int,
    x: Float,
    y: Float,
    z: Float,
) callconv(.C) void = undefined;
pub var programUniform4f: *const fn (
    program: Uint,
    location: Int,
    x: Float,
    y: Float,
    z: Float,
    w: Float,
) callconv(.C) void = undefined;
pub var programUniform1d: *const fn (
    program: Uint,
    location: Int,
    x: Double,
) callconv(.C) void = undefined;
pub var programUniform2d: *const fn (
    program: Uint,
    location: Int,
    x: Double,
    y: Double,
) callconv(.C) void = undefined;
pub var programUniform3d: *const fn (
    program: Uint,
    location: Int,
    x: Double,
    y: Double,
    z: Double,
) callconv(.C) void = undefined;
pub var programUniform4d: *const fn (
    program: Uint,
    location: Int,
    x: Double,
    y: Double,
    z: Double,
    w: Double,
) callconv(.C) void = undefined;
pub var programUniform1iv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Int,
) callconv(.C) void = undefined;
pub var programUniform2iv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Int,
) callconv(.C) void = undefined;
pub var programUniform3iv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Int,
) callconv(.C) void = undefined;
pub var programUniform4iv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Int,
) callconv(.C) void = undefined;
pub var programUniform1uiv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Uint,
) callconv(.C) void = undefined;
pub var programUniform2uiv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Uint,
) callconv(.C) void = undefined;
pub var programUniform3uiv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Uint,
) callconv(.C) void = undefined;
pub var programUniform4uiv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Uint,
) callconv(.C) void = undefined;
pub var programUniform1fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniform2fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniform3fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniform4fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniform1dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniform2dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniform3dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniform4dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix2fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix3fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix4fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix2dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix3dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix4dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix2x3fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix3x2fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix2x4fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix4x2fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix3x4fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix4x3fv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var programUniformMatrix2x3dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix3x2dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix2x4dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix4x2dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix3x4dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var programUniformMatrix4x3dv: *const fn (
    program: Uint,
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Double,
) callconv(.C) void = undefined;
pub var validateProgramPipeline: *const fn (pipeline: Uint) callconv(.C) void = undefined;
pub var getProgramPipelineInfoLog: *const fn (
    pipeline: Uint,
    bufSize: Sizei,
    length: *Sizei,
    infoLog: [*]u8,
) callconv(.C) void = undefined;
pub var vertexAttribL1d: *const fn (index: Uint, x: Double) callconv(.C) void = undefined;
pub var vertexAttribL2d: *const fn (index: Uint, x: Double, y: Double) callconv(.C) void = undefined;
pub var vertexAttribL3d: *const fn (
    index: Uint,
    x: Double,
    y: Double,
    z: Double,
) callconv(.C) void = undefined;
pub var vertexAttribL4d: *const fn (
    index: Uint,
    x: Double,
    y: Double,
    z: Double,
    w: Double,
) callconv(.C) void = undefined;
pub var vertexAttribL1dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
pub var vertexAttribL2dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
pub var vertexAttribL3dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
pub var vertexAttribL4dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
pub var viewportArrayv: *const fn (
    first: Uint,
    count: Sizei,
    v: [*]const Float,
) callconv(.C) void = undefined;
pub var viewportIndexedf: *const fn (
    index: Uint,
    x: Float,
    y: Float,
    w: Float,
    h: Float,
) callconv(.C) void = undefined;
pub var viewportIndexedfv: *const fn (index: Uint, v: [*]const Float) callconv(.C) void = undefined;
pub var scissorArrayv: *const fn (
    first: Uint,
    count: Sizei,
    v: [*]const Int,
) callconv(.C) void = undefined;
pub var scissorIndexed: *const fn (
    index: Uint,
    left: Int,
    bottom: Int,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
pub var scissorIndexedv: *const fn (index: Uint, v: [*]const Int) callconv(.C) void = undefined;
pub var depthRangeArrayv: *const fn (
    first: Uint,
    count: Sizei,
    v: [*]const Clampd,
) callconv(.C) void = undefined;
pub var depthRangeIndexed: *const fn (
    index: Uint,
    n: Clampd,
    f: Clampd,
) callconv(.C) void = undefined;
pub var getFloati_v: *const fn (
    target: Enum,
    index: Uint,
    data: [*]Float,
) callconv(.C) void = undefined;
pub var getDoublei_v: *const fn (
    target: Enum,
    index: Uint,
    data: [*]Double,
) callconv(.C) void = undefined;

//--------------------------------------------------------------------------------------------------
//
// OpenGL 4.2 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const COPY_READ_BUFFER_BINDING = COPY_READ_BUFFER;
pub const COPY_WRITE_BUFFER_BINDING = COPY_WRITE_BUFFER;
pub const TRANSFORM_FEEDBACK_PAUSED = TRANSFORM_FEEDBACK_BUFFER_PAUSED;
pub const TRANSFORM_FEEDBACK_ACTIVE = TRANSFORM_FEEDBACK_BUFFER_ACTIVE;
pub const UNPACK_COMPRESSED_BLOCK_WIDTH = 0x9127;
pub const UNPACK_COMPRESSED_BLOCK_HEIGHT = 0x9128;
pub const UNPACK_COMPRESSED_BLOCK_DEPTH = 0x9129;
pub const UNPACK_COMPRESSED_BLOCK_SIZE = 0x912A;
pub const PACK_COMPRESSED_BLOCK_WIDTH = 0x912B;
pub const PACK_COMPRESSED_BLOCK_HEIGHT = 0x912C;
pub const PACK_COMPRESSED_BLOCK_DEPTH = 0x912D;
pub const PACK_COMPRESSED_BLOCK_SIZE = 0x912E;
pub const NUM_SAMPLE_COUNTS = 0x9380;
pub const MIN_MAP_BUFFER_ALIGNMENT = 0x90BC;
pub const ATOMIC_COUNTER_BUFFER = 0x92C0;
pub const ATOMIC_COUNTER_BUFFER_BINDING = 0x92C1;
pub const ATOMIC_COUNTER_BUFFER_START = 0x92C2;
pub const ATOMIC_COUNTER_BUFFER_SIZE = 0x92C3;
pub const ATOMIC_COUNTER_BUFFER_DATA_SIZE = 0x92C4;
pub const ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS = 0x92C5;
pub const ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES = 0x92C6;
pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER = 0x92C7;
pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER = 0x92C8;
pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x92C9;
pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER = 0x92CA;
pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER = 0x92CB;
pub const MAX_VERTEX_ATOMIC_COUNTER_BUFFERS = 0x92CC;
pub const MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS = 0x92CD;
pub const MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS = 0x92CE;
pub const MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS = 0x92CF;
pub const MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS = 0x92D0;
pub const MAX_COMBINED_ATOMIC_COUNTER_BUFFERS = 0x92D1;
pub const MAX_VERTEX_ATOMIC_COUNTERS = 0x92D2;
pub const MAX_TESS_CONTROL_ATOMIC_COUNTERS = 0x92D3;
pub const MAX_TESS_EVALUATION_ATOMIC_COUNTERS = 0x92D4;
pub const MAX_GEOMETRY_ATOMIC_COUNTERS = 0x92D5;
pub const MAX_FRAGMENT_ATOMIC_COUNTERS = 0x92D6;
pub const MAX_COMBINED_ATOMIC_COUNTERS = 0x92D7;
pub const MAX_ATOMIC_COUNTER_BUFFER_SIZE = 0x92D8;
pub const MAX_ATOMIC_COUNTER_BUFFER_BINDINGS = 0x92DC;
pub const ACTIVE_ATOMIC_COUNTER_BUFFERS = 0x92D9;
pub const UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX = 0x92DA;
pub const UNSIGNED_INT_ATOMIC_COUNTER = 0x92DB;
pub const VERTEX_ATTRIB_ARRAY_BARRIER_BIT = 0x00000001;
pub const ELEMENT_ARRAY_BARRIER_BIT = 0x00000002;
pub const UNIFORM_BARRIER_BIT = 0x00000004;
pub const TEXTURE_FETCH_BARRIER_BIT = 0x00000008;
pub const SHADER_IMAGE_ACCESS_BARRIER_BIT = 0x00000020;
pub const COMMAND_BARRIER_BIT = 0x00000040;
pub const PIXEL_BUFFER_BARRIER_BIT = 0x00000080;
pub const TEXTURE_UPDATE_BARRIER_BIT = 0x00000100;
pub const BUFFER_UPDATE_BARRIER_BIT = 0x00000200;
pub const FRAMEBUFFER_BARRIER_BIT = 0x00000400;
pub const TRANSFORM_FEEDBACK_BARRIER_BIT = 0x00000800;
pub const ATOMIC_COUNTER_BARRIER_BIT = 0x00001000;
pub const ALL_BARRIER_BITS = 0xFFFFFFFF;
pub const MAX_IMAGE_UNITS = 0x8F38;
pub const MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS = 0x8F39;
pub const IMAGE_BINDING_NAME = 0x8F3A;
pub const IMAGE_BINDING_LEVEL = 0x8F3B;
pub const IMAGE_BINDING_LAYERED = 0x8F3C;
pub const IMAGE_BINDING_LAYER = 0x8F3D;
pub const IMAGE_BINDING_ACCESS = 0x8F3E;
pub const IMAGE_1D = 0x904C;
pub const IMAGE_2D = 0x904D;
pub const IMAGE_3D = 0x904E;
pub const IMAGE_2D_RECT = 0x904F;
pub const IMAGE_CUBE = 0x9050;
pub const IMAGE_BUFFER = 0x9051;
pub const IMAGE_1D_ARRAY = 0x9052;
pub const IMAGE_2D_ARRAY = 0x9053;
pub const IMAGE_CUBE_MAP_ARRAY = 0x9054;
pub const IMAGE_2D_MULTISAMPLE = 0x9055;
pub const IMAGE_2D_MULTISAMPLE_ARRAY = 0x9056;
pub const INT_IMAGE_1D = 0x9057;
pub const INT_IMAGE_2D = 0x9058;
pub const INT_IMAGE_3D = 0x9059;
pub const INT_IMAGE_2D_RECT = 0x905A;
pub const INT_IMAGE_CUBE = 0x905B;
pub const INT_IMAGE_BUFFER = 0x905C;
pub const INT_IMAGE_1D_ARRAY = 0x905D;
pub const INT_IMAGE_2D_ARRAY = 0x905E;
pub const INT_IMAGE_CUBE_MAP_ARRAY = 0x905F;
pub const INT_IMAGE_2D_MULTISAMPLE = 0x9060;
pub const INT_IMAGE_2D_MULTISAMPLE_ARRAY = 0x9061;
pub const UNSIGNED_INT_IMAGE_1D = 0x9062;
pub const UNSIGNED_INT_IMAGE_2D = 0x9063;
pub const UNSIGNED_INT_IMAGE_3D = 0x9064;
pub const UNSIGNED_INT_IMAGE_2D_RECT = 0x9065;
pub const UNSIGNED_INT_IMAGE_CUBE = 0x9066;
pub const UNSIGNED_INT_IMAGE_BUFFER = 0x9067;
pub const UNSIGNED_INT_IMAGE_1D_ARRAY = 0x9068;
pub const UNSIGNED_INT_IMAGE_2D_ARRAY = 0x9069;
pub const UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY = 0x906A;
pub const UNSIGNED_INT_IMAGE_2D_MULTISAMPLE = 0x906B;
pub const UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY = 0x906C;
pub const MAX_IMAGE_SAMPLES = 0x906D;
pub const IMAGE_BINDING_FORMAT = 0x906E;
pub const IMAGE_FORMAT_COMPATIBILITY_TYPE = 0x90C7;
pub const IMAGE_FORMAT_COMPATIBILITY_BY_SIZE = 0x90C8;
pub const IMAGE_FORMAT_COMPATIBILITY_BY_CLASS = 0x90C9;
pub const MAX_VERTEX_IMAGE_UNIFORMS = 0x90CA;
pub const MAX_TESS_CONTROL_IMAGE_UNIFORMS = 0x90CB;
pub const MAX_TESS_EVALUATION_IMAGE_UNIFORMS = 0x90CC;
pub const MAX_GEOMETRY_IMAGE_UNIFORMS = 0x90CD;
pub const MAX_FRAGMENT_IMAGE_UNIFORMS = 0x90CE;
pub const MAX_COMBINED_IMAGE_UNIFORMS = 0x90CF;
pub const COMPRESSED_RGBA_BPTC_UNORM = 0x8E8C;
pub const COMPRESSED_SRGB_ALPHA_BPTC_UNORM = 0x8E8D;
pub const COMPRESSED_RGB_BPTC_SIGNED_FLOAT = 0x8E8E;
pub const COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = 0x8E8F;
pub const TEXTURE_IMMUTABLE_FORMAT = 0x912F;

pub var drawArraysInstancedBaseInstance: *const fn (
    mode: Enum,
    first: Int,
    count: Sizei,
    instancecount: Sizei,
    baseinstance: Uint,
) callconv(.C) void = undefined;
pub var drawElementsInstancedBaseInstance: *const fn (
    mode: Enum,
    count: Sizei,
    type: Enum,
    indices: *const anyopaque,
    instancecount: Sizei,
    baseinstance: Uint,
) callconv(.C) void = undefined;
pub var drawElementsInstancedBaseVertexBaseInstance: *const fn (
    mode: Enum,
    count: Sizei,
    type: Enum,
    indices: *const anyopaque,
    instancecount: Sizei,
    basevertex: Int,
    baseinstance: Uint,
) callconv(.C) void = undefined;
pub var getInternalformativ: *const fn (
    target: Enum,
    internalformat: Enum,
    pname: Enum,
    count: Sizei,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var getActiveAtomicCounterBufferiv: *const fn (
    program: Uint,
    bufferIndex: Uint,
    pname: Enum,
    params: [*c]Int,
) callconv(.C) void = undefined;
pub var bindImageTexture: *const fn (
    unit: Uint,
    texture: Uint,
    level: Int,
    layered: Boolean,
    layer: Int,
    access: Enum,
    format: Enum,
) callconv(.C) void = undefined;
pub var memoryBarrier: *const fn (
    barriers: Bitfield,
) callconv(.C) void = undefined;
pub var texStorage1D: *const fn (
    target: Enum,
    levels: Sizei,
    internalformat: Enum,
    width: Sizei,
) callconv(.C) void = undefined;
pub var texStorage2D: *const fn (
    target: Enum,
    levels: Sizei,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
pub var texStorage3D: *const fn (
    target: Enum,
    levels: Sizei,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    depth: Sizei,
) callconv(.C) void = undefined;
pub var drawTransformFeedbackInstanced: *const fn (
    mode: Enum,
    id: Uint,
    instancecount: Sizei,
) callconv(.C) void = undefined;
pub var drawTransformFeedbackStreamInstanced: *const fn (
    mode: Enum,
    id: Uint,
    stream: Uint,
    instancecount: Sizei,
) callconv(.C) void = undefined;

//--------------------------------------------------------------------------------------------------
//
// OpenGL 4.3 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const DEBUGPROC = *const fn (
    source: Enum,
    type: Enum,
    id: Uint,
    severity: Enum,
    length: Sizei,
    message: [*c]const Char,
    userParam: *const anyopaque,
) callconv(.C) void;
pub const DEBUG_OUTPUT = 0x92E0;
pub const DEBUG_SOURCE_API = 0x8246;
pub const DEBUG_SOURCE_WINDOW_SYSTEM = 0x8247;
pub const DEBUG_SOURCE_SHADER_COMPILER = 0x8248;
pub const DEBUG_SOURCE_THIRD_PARTY = 0x8249;
pub const DEBUG_SOURCE_APPLICATION = 0x824A;
pub const DEBUG_SOURCE_OTHER = 0x824B;
pub const DEBUG_TYPE_ERROR = 0x824C;
pub const DEBUG_TYPE_DEPRECATED_BEHAVIOR = 0x824D;
pub const DEBUG_TYPE_UNDEFINED_BEHAVIOR = 0x824E;
pub const DEBUG_TYPE_PORTABILITY = 0x824F;
pub const DEBUG_TYPE_PERFORMANCE = 0x8250;
pub const DEBUG_TYPE_MARKER = 0x8268;
pub const DEBUG_TYPE_PUSH_GROUP = 0x8269;
pub const DEBUG_TYPE_POP_GROUP = 0x826A;
pub const DEBUG_TYPE_OTHER = 0x8251;
pub const DEBUG_SEVERITY_HIGH = 0x9146;
pub const DEBUG_SEVERITY_MEDIUM = 0x9147;
pub const DEBUG_SEVERITY_LOW = 0x9148;
pub const DEBUG_SEVERITY_NOTIFICATION = 0x826B;
pub const SHADER_STORAGE_BUFFER = 0x90D2;
pub const SHADER_STORAGE_BLOCK = 0x92e6;

pub var debugMessageControl: *const fn (
    source: Enum,
    type: Enum,
    severity: Enum,
    count: Sizei,
    ids: [*c]const Uint,
    enabled: Boolean,
) callconv(.C) void = undefined;
pub var debugMessageInsert: *const fn (
    source: Enum,
    type: Enum,
    id: Uint,
    severity: Enum,
    length: Sizei,
    buf: [*c]const u8,
) callconv(.C) void = undefined;
pub var debugMessageCallback: *const fn (
    callback: DEBUGPROC,
    userParam: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var getDebugMessageLog: *const fn (
    count: Uint,
    bufSize: Sizei,
    sources: [*c]Enum,
    types: [*c]Enum,
    ids: [*c]Uint,
    severities: [*c]Enum,
    lengths: [*c]Sizei,
    messageLog: [*c]Char,
) callconv(.C) Uint = undefined;
pub var getPointerv: *const fn (
    pname: Enum,
    params: *anyopaque,
) callconv(.C) void = undefined;
pub var pushDebugGroup: *const fn (
    source: Enum,
    id: Uint,
    length: Sizei,
    message: [*c]const Char,
) callconv(.C) void = undefined;
pub var popDebugGroup: *const fn () callconv(.C) void = undefined;
pub var objectLabel: *const fn (
    identifier: Enum,
    name: Uint,
    length: Sizei,
    label: [*c]const Char,
) callconv(.C) void = undefined;
pub var getObjectLabel: *const fn (
    identifier: Enum,
    name: Uint,
    bufSize: Sizei,
    length: *Sizei,
    label: [*c]Char,
) callconv(.C) void = undefined;
pub var objectPtrLabel: *const fn (
    ptr: *anyopaque,
    length: Sizei,
    label: [*c]const Char,
) callconv(.C) void = undefined;
pub var getObjectPtrLabel: *const fn (
    ptr: *anyopaque,
    bufSize: Sizei,
    length: *Sizei,
    label: [*c]Char,
) callconv(.C) void = undefined;
pub var getProgramResourceIndex: *const fn (
    program: Uint,
    programInterface: Enum,
    name: [*c]const Char,
) callconv(.C) Uint = undefined;
pub var shaderStorageBlockBinding: *const fn (
    program: Uint,
    storageBlockIndex: Uint,
    storageBlockBinding: Uint,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 4.4 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub var clearTexImage: *const fn (
    texture: Uint,
    level: Int,
    format: Enum,
    type: Enum,
    data: ?*const anyopaque,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 4.5 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub var textureStorage2D: *const fn (
    texture: Uint,
    levels: Sizei,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
pub var textureStorage2DMultisample: *const fn (
    texture: Uint,
    samples: Sizei,
    internalformat: Enum,
    width: Sizei,
    height: Sizei,
    fixedsamplelocations: Boolean,
) callconv(.C) void = undefined;
pub var createTextures: *const fn (target: Enum, n: Sizei, textures: [*c]Uint) callconv(.C) void = undefined;
pub var createFramebuffers: *const fn (n: Sizei, framebuffers: [*c]Uint) callconv(.C) void = undefined;
pub var namedFramebufferTexture: *const fn (
    framebuffer: Uint,
    attachment: Enum,
    texture: Uint,
    level: Int,
) callconv(.C) void = undefined;
pub var blitNamedFramebuffer: *const fn (
    readFramebuffer: Uint,
    drawFramebuffer: Uint,
    srcX0: Int,
    srcY0: Int,
    srcX1: Int,
    srcY1: Int,
    dstX0: Int,
    dstY0: Int,
    dstX1: Int,
    dstY1: Int,
    mask: Bitfield,
    filter: Enum,
) callconv(.C) void = undefined;
pub var createBuffers: *const fn (n: Sizei, buffers: [*c]Uint) callconv(.C) void = undefined;
pub var clearNamedFramebufferfv: *const fn (
    framebuffer: Uint,
    buffer: Enum,
    drawbuffer: Int,
    value: [*c]const Float,
) callconv(.C) void = undefined;
pub var namedBufferStorage: *const fn (
    buffer: Uint,
    size: Sizeiptr,
    data: ?*const anyopaque,
    flags: Bitfield,
) callconv(.C) void = undefined;
pub var bindTextureUnit: *const fn (unit: Uint, texture: Uint) callconv(.C) void = undefined;
pub var textureBarrier: *const fn () callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 1.0 and 1.1 (Compatibility Profile)
//
//--------------------------------------------------------------------------------------------------
pub const MODELVIEW = 0x1700;
pub const PROJECTION = 0x1701;
pub const COMPILE = 0x1300;
pub const COMPILE_AND_EXECUTE = 0x1301;
pub const QUAD_STRIP = 0x0008;
pub const POLYGON = 0x0009;

pub var begin: *const fn (mode: Enum) callconv(.C) void = undefined;
pub var end: *const fn () callconv(.C) void = undefined;
pub var newList: *const fn (list: Uint, mode: Enum) callconv(.C) void = undefined;
pub var callList: *const fn (list: Uint) callconv(.C) void = undefined;
pub var endList: *const fn () callconv(.C) void = undefined;
pub var loadIdentity: *const fn () callconv(.C) void = undefined;
pub var vertex2fv: *const fn (v: [*c]const Float) callconv(.C) void = undefined;
pub var vertex3fv: *const fn (v: [*c]const Float) callconv(.C) void = undefined;
pub var vertex4fv: *const fn (v: [*c]const Float) callconv(.C) void = undefined;
pub var color3fv: *const fn (v: [*c]const Float) callconv(.C) void = undefined;
pub var color4fv: *const fn (v: [*c]const Float) callconv(.C) void = undefined;
pub var rectf: *const fn (x1: Float, y1: Float, x2: Float, y2: Float) callconv(.C) void = undefined;
pub var matrixMode: *const fn (mode: Enum) callconv(.C) void = undefined;
pub var vertex2f: *const fn (x: Float, y: Float) callconv(.C) void = undefined;
pub var vertex2d: *const fn (x: Double, y: Double) callconv(.C) void = undefined;
pub var vertex2i: *const fn (x: Int, y: Int) callconv(.C) void = undefined;
pub var color3f: *const fn (r: Float, g: Float, b: Float) callconv(.C) void = undefined;
pub var color4f: *const fn (r: Float, g: Float, b: Float, a: Float) callconv(.C) void = undefined;
pub var color4ub: *const fn (r: Ubyte, g: Ubyte, b: Ubyte, a: Ubyte) callconv(.C) void = undefined;
pub var pushMatrix: *const fn () callconv(.C) void = undefined;
pub var popMatrix: *const fn () callconv(.C) void = undefined;
pub var rotatef: *const fn (angle: Float, x: Float, y: Float, z: Float) callconv(.C) void = undefined;
pub var scalef: *const fn (x: Float, y: Float, z: Float) callconv(.C) void = undefined;
pub var translatef: *const fn (x: Float, y: Float, z: Float) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL misc. extensions (Compatibility Profile)
//
//--------------------------------------------------------------------------------------------------
pub var matrixLoadIdentityEXT: *const fn (mode: Enum) callconv(.C) void = undefined;
pub var matrixOrthoEXT: *const fn (
    mode: Enum,
    left: Double,
    right: Double,
    bottom: Double,
    top: Double,
    zNear: Double,
    zFar: Double,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// EXT_copy_texture
//
//--------------------------------------------------------------------------------------------------
pub var copyTexImage1DEXT: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    x: Int,
    y: Int,
    width: Sizei,
    border: Int,
) callconv(.C) void = undefined;
pub var copyTexImage2DEXT: *const fn (
    target: Enum,
    level: Int,
    internalformat: Enum,
    x: Int,
    y: Int,
    width: Sizei,
    height: Sizei,
    border: Int,
) callconv(.C) void = undefined;
pub var copyTexSubImage1DEXT: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    x: Int,
    y: Int,
    width: Sizei,
) callconv(.C) void = undefined;
pub var copyTexSubImage2DEXT: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    x: Int,
    y: Int,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
pub var copyTexSubImage3DEXT: *const fn (
    target: Enum,
    level: Int,
    xoffset: Int,
    yoffset: Int,
    zoffset: Int,
    x: Int,
    y: Int,
    width: Sizei,
    height: Sizei,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// NV_bindless_texture
//
//--------------------------------------------------------------------------------------------------
pub var getTextureHandleNV: *const fn (texture: Uint) callconv(.C) Uint64 = undefined;
pub var makeTextureHandleResidentNV: *const fn (handle: Uint64) callconv(.C) void = undefined;
pub var programUniformHandleui64NV: *const fn (
    program: Uint,
    location: Int,
    value: Uint64,
) callconv(.C) void = undefined;
// TODO: Add the rest
//--------------------------------------------------------------------------------------------------
//
// NV_shader_buffer_load
//
//--------------------------------------------------------------------------------------------------
pub const BUFFER_GPU_ADDRESS_NV = 0x8F1D;

pub var makeNamedBufferResidentNV: *const fn (buffer: Uint, access: Enum) callconv(.C) void = undefined;
pub var getNamedBufferParameterui64vNV: *const fn (
    buffer: Uint,
    pname: Enum,
    params: [*c]Uint64,
) callconv(.C) void = undefined;
pub var programUniformui64NV: *const fn (
    program: Uint,
    location: Int,
    value: Uint64,
) callconv(.C) void = undefined;
// TODO: Add the rest
//--------------------------------------------------------------------------------------------------
//
// OpenGL ES 1.0
//
//--------------------------------------------------------------------------------------------------
pub var clearDepthf: *const fn (depth: Float) callconv(.C) void = undefined;
pub var depthRangef: *const fn (n: Clampf, f: Clampf) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL ES 2.0
//
//--------------------------------------------------------------------------------------------------
pub const FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
//--------------------------------------------------------------------------------------------------
//
// OES_vertex_array_object (OpenGL ES Extension #71)
//
//--------------------------------------------------------------------------------------------------
pub const VERTEX_ARRAY_BINDING_OES = VERTEX_ARRAY_BINDING;

pub var bindVertexArrayOES: *const fn (array: Uint) callconv(.C) void = undefined;
pub var deleteVertexArraysOES: *const fn (
    n: Sizei,
    arrays: [*c]const Uint,
) callconv(.C) void = undefined;
pub var genVertexArraysOES: *const fn (n: Sizei, arrays: [*c]Uint) callconv(.C) void = undefined;
pub var isVertexArrayOES: *const fn (array: Uint) callconv(.C) Boolean = undefined;
//--------------------------------------------------------------------------------------------------
//
// KHR_debug (OpenGL ES Extension #118)
//
//--------------------------------------------------------------------------------------------------
pub var debugMessageControlKHR: *const fn (
    source: Enum,
    type: Enum,
    severity: Enum,
    count: Sizei,
    ids: [*c]const Uint,
    enabled: Boolean,
) callconv(.C) void = undefined;
pub var debugMessageInsertKHR: *const fn (
    source: Enum,
    type: Enum,
    id: Uint,
    severity: Enum,
    length: Sizei,
    buf: [*c]const u8,
) callconv(.C) void = undefined;
pub var debugMessageCallbackKHR: *const fn (
    callback: DEBUGPROC,
    userParam: ?*const anyopaque,
) callconv(.C) void = undefined;
pub var getDebugMessageLogKHR: *const fn (
    count: Uint,
    bufSize: Sizei,
    sources: [*c]Enum,
    types: [*c]Enum,
    ids: [*c]Uint,
    severities: [*c]Enum,
    lengths: [*c]Sizei,
    messageLog: [*c]Char,
) callconv(.C) Uint = undefined;
pub var getPointervKHR: *const fn (
    pname: Enum,
    params: *anyopaque,
) callconv(.C) void = undefined;
pub var pushDebugGroupKHR: *const fn (
    source: Enum,
    id: Uint,
    length: Sizei,
    message: [*c]const Char,
) callconv(.C) void = undefined;
pub var popDebugGroupKHR: *const fn () callconv(.C) void = undefined;
pub var objectLabelKHR: *const fn (
    identifier: Enum,
    name: Uint,
    length: Sizei,
    label: [*c]const Char,
) callconv(.C) void = undefined;
pub var getObjectLabelKHR: *const fn (
    identifier: Enum,
    name: Uint,
    bufSize: Sizei,
    length: *Sizei,
    label: [*c]Char,
) callconv(.C) void = undefined;
pub var objectPtrLabelKHR: *const fn (
    ptr: *anyopaque,
    length: Sizei,
    label: [*c]const Char,
) callconv(.C) void = undefined;
pub var getObjectPtrLabelKHR: *const fn (
    ptr: *anyopaque,
    bufSize: Sizei,
    length: *Sizei,
    label: [*c]Char,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
