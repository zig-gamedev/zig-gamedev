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
pub var glGetIntegerv: *const fn (pname: Enum, data: [*c]Int) callconv(.C) void = undefined;
pub var getString: *const fn (name: Enum) callconv(.C) ?[*:0]const Ubyte = undefined;
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
