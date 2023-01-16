//! Disclaimer: You should probably precompile your shaders with dxc and not use this!

const w32 = @import("w32.zig");
const WINAPI = w32.WINAPI;
const HRESULT = w32.HRESULT;
const LPCSTR = w32.LPCSTR;
const UINT = w32.UINT;
const SIZE_T = w32.SIZE_T;

const d3dcommon = @import("d3dcommon.zig");

pub const COMPILE_FLAG = UINT;
pub const COMPILE_DEBUG: COMPILE_FLAG = (1 << 0);
pub const COMPILE_SKIP_VALIDATION: COMPILE_FLAG = (1 << 1);
pub const COMPILE_SKIP_OPTIMIZATION: COMPILE_FLAG = (1 << 2);
pub const COMPILE_PACK_MATRIX_ROW_MAJOR: COMPILE_FLAG = (1 << 3);
pub const COMPILE_PACK_MATRIX_COLUMN_MAJOR: COMPILE_FLAG = (1 << 4);
pub const COMPILE_PARTIAL_PRECISION: COMPILE_FLAG = (1 << 5);
pub const COMPILE_FORCE_VS_SOFTWARE_NO_OPT: COMPILE_FLAG = (1 << 6);
pub const COMPILE_FORCE_PS_SOFTWARE_NO_OPT: COMPILE_FLAG = (1 << 7);
pub const COMPILE_NO_PRESHADER: COMPILE_FLAG = (1 << 8);
pub const COMPILE_AVOID_FLOW_CONTROL: COMPILE_FLAG = (1 << 9);
pub const COMPILE_PREFER_FLOW_CONTROL: COMPILE_FLAG = (1 << 10);
pub const COMPILE_ENABLE_STRICTNESS: COMPILE_FLAG = (1 << 11);
pub const COMPILE_ENABLE_BACKWARDS_COMPATIBILITY: COMPILE_FLAG = (1 << 12);
pub const COMPILE_IEEE_STRICTNESS: COMPILE_FLAG = (1 << 13);
pub const COMPILE_OPTIMIZATION_LEVEL0: COMPILE_FLAG = (1 << 14);
pub const COMPILE_OPTIMIZATION_LEVEL1: COMPILE_FLAG = 0;
pub const COMPILE_OPTIMIZATION_LEVEL2: COMPILE_FLAG = ((1 << 14) | (1 << 15));
pub const COMPILE_OPTIMIZATION_LEVEL3: COMPILE_FLAG = (1 << 15);
pub const COMPILE_RESERVED16: COMPILE_FLAG = (1 << 16);
pub const COMPILE_RESERVED17: COMPILE_FLAG = (1 << 17);
pub const COMPILE_WARNINGS_ARE_ERRORS: COMPILE_FLAG = (1 << 18);
pub const COMPILE_RESOURCES_MAY_ALIAS: COMPILE_FLAG = (1 << 19);
pub const COMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES: COMPILE_FLAG = (1 << 20);
pub const COMPILE_ALL_RESOURCES_BOUND: COMPILE_FLAG = (1 << 21);
pub const COMPILE_DEBUG_NAME_FOR_SOURCE: COMPILE_FLAG = (1 << 22);
pub const COMPILE_DEBUG_NAME_FOR_BINARY: COMPILE_FLAG = (1 << 23);

pub extern "D3DCompiler_47" fn D3DCompile(
    pSrcData: *const anyopaque,
    SrcDataSize: SIZE_T,
    pSourceName: ?LPCSTR,
    pDefines: ?*const d3dcommon.SHADER_MACRO,
    pInclude: ?*const d3dcommon.IInclude,
    pEntrypoint: LPCSTR,
    pTarget: LPCSTR,
    Flags1: UINT,
    Flags2: UINT,
    ppCode: **d3dcommon.IBlob,
    ppErrorMsgs: ?**d3dcommon.IBlob,
) callconv(WINAPI) HRESULT;
