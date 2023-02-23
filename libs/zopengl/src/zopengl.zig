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
    first: [*]const Int,
    count: [*]const Sizei,
    drawcount: Sizei,
) callconv(.C) void = undefined;
pub var multiDrawElements: *const fn (
    mode: Enum,
    count: [*]const Sizei,
    type: Enum,
    indices: [*]?*const anyopaque,
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
pub var getBufferPointerv: *const fn (target: Enum, pname: Enum, params: *?*anyopaque) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
//
// OpenGL 2.0 (Core Profile)
//
//--------------------------------------------------------------------------------------------------
pub const Char = i8;
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
    name: [*:0]const Char,
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
    length: ?*Sizei,
    size: ?*Int,
    type: ?*Enum,
    name: ?[*:0]Char,
) callconv(.C) void = undefined;
pub var getActiveUniform: *const fn (
    program: Uint,
    index: Uint,
    bufSize: Sizei,
    length: ?*Sizei,
    size: ?*Int,
    type: ?*Enum,
    name: ?[*:0]Char,
) callconv(.C) Int = undefined;
pub var getAttachedShaders: *const fn (
    program: Uint,
    maxCount: Sizei,
    count: ?*Sizei,
    shaders: ?[*]Uint,
) callconv(.C) void = undefined;
pub var getAttribLocation: *const fn (program: Uint, name: [*:0]const Char) callconv(.C) Int = undefined;
pub var getProgramiv: *const fn (program: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getProgramInfoLog: *const fn (
    program: Uint,
    bufSize: Sizei,
    length: ?*Sizei,
    infoLog: ?[*:0]Char,
) callconv(.C) void = undefined;
pub var getShaderiv: *const fn (shader: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getShaderInfoLog: *const fn (
    shader: Uint,
    bufSize: Sizei,
    length: ?*Sizei,
    infoLog: ?[*:0]Char,
) callconv(.C) void = undefined;
pub var getShaderSource: *const fn (
    shader: Uint,
    bufSize: Sizei,
    length: ?*Sizei,
    source: ?[*:0]Char,
) callconv(.C) void = undefined;
pub var getUniformLocation: *const fn (program: Uint, name: [*:0]const Char) callconv(.C) void = undefined;
pub var getUniformfv: *const fn (program: Uint, location: Int, params: [*c]Float) callconv(.C) void = undefined;
pub var getUniformiv: *const fn (program: Uint, location: Int, params: [*c]Int) callconv(.C) void = undefined;
pub var getVertexAttribdv: *const fn (index: Uint, pname: Enum, params: [*c]Double) callconv(.C) void = undefined;
pub var getVertexAttribfv: *const fn (index: Uint, pname: Enum, params: [*c]Float) callconv(.C) void = undefined;
pub var getVertexAttribiv: *const fn (index: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
pub var getVertexAttribPointerv: *const fn (
    index: Uint,
    pname: Enum,
    pointer: *?*anyopaque,
) callconv(.C) void = undefined;
pub var isProgram: *const fn (program: Uint) callconv(.C) Boolean = undefined;
pub var isShader: *const fn (shader: Uint) callconv(.C) Boolean = undefined;
pub var linkProgram: *const fn (program: Uint) callconv(.C) void = undefined;
pub var shaderSource: *const fn (
    shader: Uint,
    count: Sizei,
    string: [*][*:0]const Char,
    length: ?[*]const Int,
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
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var uniform2fv: *const fn (
    location: Int,
    count: Sizei,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var uniform3fv: *const fn (
    location: Int,
    count: Sizei,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var uniform4fv: *const fn (
    location: Int,
    count: Sizei,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var uniform1iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniform2iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniform3iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniform4iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
pub var uniformMatrix2fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix3fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var uniformMatrix4fv: *const fn (
    location: Int,
    count: Sizei,
    transpose: Boolean,
    value: [*]const Float,
) callconv(.C) void = undefined;
pub var validateProgram: *const fn (program: Uint) callconv(.C) void = undefined;
pub var vertexAttrib1d: *const fn (index: Uint, x: Double) callconv(.C) void = undefined;
pub var vertexAttrib1dv: *const fn (index: Uint, v: *const Double) callconv(.C) void = undefined;
pub var vertexAttrib1f: *const fn (index: Uint, x: Float) callconv(.C) void = undefined;
pub var vertexAttrib1fv: *const fn (index: Uint, v: *const Float) callconv(.C) void = undefined;
pub var vertexAttrib1s: *const fn (index: Uint, x: Short) callconv(.C) void = undefined;
pub var vertexAttrib1sv: *const fn (index: Uint, v: *const Short) callconv(.C) void = undefined;
pub var vertexAttrib2d: *const fn (index: Uint, x: Double, y: Double) callconv(.C) void = undefined;
pub var vertexAttrib2dv: *const fn (index: Uint, v: *[2]Double) callconv(.C) void = undefined;
pub var vertexAttrib2f: *const fn (index: Uint, x: Float, y: Float) callconv(.C) void = undefined;
pub var vertexAttrib2fv: *const fn (index: Uint, v: *[2]Float) callconv(.C) void = undefined;
pub var vertexAttrib2s: *const fn (index: Uint, x: Short, y: Short) callconv(.C) void = undefined;
pub var vertexAttrib2sv: *const fn (index: Uint, v: *[2]Short) callconv(.C) void = undefined;
pub var vertexAttrib3d: *const fn (index: Uint, x: Double, y: Double, z: Double) callconv(.C) void = undefined;
pub var vertexAttrib3dv: *const fn (index: Uint, v: *[3]Double) callconv(.C) void = undefined;
pub var vertexAttrib3f: *const fn (index: Uint, x: Float, y: Float, z: Float) callconv(.C) void = undefined;
pub var vertexAttrib3fv: *const fn (index: Uint, v: *[3]Float) callconv(.C) void = undefined;
pub var vertexAttrib3s: *const fn (index: Uint, x: Short, y: Short, z: Short) callconv(.C) void = undefined;
pub var vertexAttrib3sv: *const fn (index: Uint, v: *[3]Short) callconv(.C) void = undefined;
pub var vertexAttrib4Nbv: *const fn (index: Uint, v: *[4]Byte) callconv(.C) void = undefined;
pub var vertexAttrib4Niv: *const fn (index: Uint, v: *[4]Int) callconv(.C) void = undefined;
pub var vertexAttrib4Nsv: *const fn (index: Uint, v: *[4]Short) callconv(.C) void = undefined;
pub var vertexAttrib4Nub: *const fn (
    index: Uint,
    x: Ubyte,
    y: Ubyte,
    z: Ubyte,
    w: Ubyte,
) callconv(.C) void = undefined;
pub var vertexAttrib4Nubv: *const fn (index: Uint, v: *[4]Ubyte) callconv(.C) void = undefined;
pub var vertexAttrib4Nuiv: *const fn (index: Uint, v: *[4]Uint) callconv(.C) void = undefined;
pub var vertexAttrib4Nusv: *const fn (index: Uint, v: *[4]Ushort) callconv(.C) void = undefined;
pub var vertexAttrib4bv: *const fn (index: Uint, v: *[4]Byte) callconv(.C) void = undefined;
pub var vertexAttrib4d: *const fn (
    index: Uint,
    x: Double,
    y: Double,
    z: Double,
    w: Double,
) callconv(.C) void = undefined;
pub var vertexAttrib4dv: *const fn (index: Uint, v: *[4]Double) callconv(.C) void = undefined;
pub var vertexAttrib4f: *const fn (
    index: Uint,
    x: Float,
    y: Float,
    z: Float,
    w: Float,
) callconv(.C) void = undefined;
pub var vertexAttrib4fv: *const fn (index: Uint, v: *[4]Float) callconv(.C) void = undefined;
pub var vertexAttrib4iv: *const fn (index: Uint, v: *[4]Int) callconv(.C) void = undefined;
pub var vertexAttrib4s: *const fn (
    index: Uint,
    x: Short,
    y: Short,
    z: Short,
    w: Short,
) callconv(.C) void = undefined;
pub var vertexAttrib4sv: *const fn (index: Uint, v: *[4]Short) callconv(.C) void = undefined;
pub var vertexAttrib4ubv: *const fn (index: Uint, v: *[4]Ubyte) callconv(.C) void = undefined;
pub var vertexAttrib4uiv: *const fn (index: Uint, v: *[4]Uint) callconv(.C) void = undefined;
pub var vertexAttrib4usv: *const fn (index: Uint, v: *[4]Ushort) callconv(.C) void = undefined;
pub var vertexAttribPointer: *const fn (
    index: Uint,
    size: Int,
    type: Enum,
    normalized: Boolean,
    stride: Sizei,
    pointer: ?*const anyopaque,
) callconv(.C) void = undefined;
//--------------------------------------------------------------------------------------------------
