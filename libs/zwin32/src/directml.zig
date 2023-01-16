const std = @import("std");
const assert = std.debug.assert;
const w32 = @import("w32.zig");
const UINT = w32.UINT;
const UINT64 = w32.UINT64;
const FLOAT = w32.FLOAT;
const IUnknown = w32.IUnknown;
const HRESULT = w32.HRESULT;
const WINAPI = w32.WINAPI;
const GUID = w32.GUID;
const LPCWSTR = w32.LPCWSTR;
const LPCSTR = w32.LPCSTR;
const BOOL = w32.BOOL;
const d3d12 = @import("d3d12.zig");

//
// DirectML constants.
//
pub const TARGET_VERSION = 0x5200;

pub const TENSOR_DIMENSION_COUNT_MAX = 5;
pub const TENSOR_DIMENSION_COUNT_MAX1 = 8;

pub const TEMPORARY_BUFFER_ALIGNMENT = 256;
pub const PERSISTENT_BUFFER_ALIGNMENT = 256;

pub const MINIMUM_BUFFER_TENSOR_ALIGNMENT = 16;

//
// Tensor descriptions.
//
pub const TENSOR_DATA_TYPE = enum(UINT) {
    UNKNOWN,
    FLOAT32,
    FLOAT16,
    UINT32,
    UINT16,
    UINT8,
    INT32,
    INT16,
    INT8,
    FLOAT64,
    UINT64,
    INT64,
};

pub const TENSOR_TYPE = enum(UINT) {
    INVALID = 0,
    BUFFER = 1,
};

pub const TENSOR_FLAGS = packed struct(UINT) {
    OWNED_BY_DML: bool = false,
    __unused: u31 = 0,
};

pub const BUFFER_TENSOR_DESC = extern struct {
    DataType: TENSOR_DATA_TYPE,
    Flags: TENSOR_FLAGS,
    DimensionCount: UINT,
    Sizes: [*]const UINT,
    Strides: ?[*]const UINT,
    TotalTensorSizeInBytes: UINT64,
    GuaranteedBaseOffsetAlignment: UINT,
};

pub const TENSOR_DESC = extern struct {
    Type: TENSOR_TYPE,
    Desc: *const anyopaque,
};

//
// Operator types.
//
pub const OPERATOR_TYPE = enum(UINT) {
    INVALID,

    ELEMENT_WISE_IDENTITY,
    ELEMENT_WISE_ABS,
    ELEMENT_WISE_ACOS,
    ELEMENT_WISE_ADD,
    ELEMENT_WISE_ASIN,
    ELEMENT_WISE_ATAN,
    ELEMENT_WISE_CEIL,
    ELEMENT_WISE_CLIP,
    ELEMENT_WISE_COS,
    ELEMENT_WISE_DIVIDE,
    ELEMENT_WISE_EXP,
    ELEMENT_WISE_FLOOR,
    ELEMENT_WISE_LOG,
    ELEMENT_WISE_LOGICAL_AND,
    ELEMENT_WISE_LOGICAL_EQUALS,
    ELEMENT_WISE_LOGICAL_GREATER_THAN,
    ELEMENT_WISE_LOGICAL_LESS_THAN,
    ELEMENT_WISE_LOGICAL_NOT,
    ELEMENT_WISE_LOGICAL_OR,
    ELEMENT_WISE_LOGICAL_XOR,
    ELEMENT_WISE_MAX,
    ELEMENT_WISE_MEAN,
    ELEMENT_WISE_MIN,
    ELEMENT_WISE_MULTIPLY,
    ELEMENT_WISE_POW,
    ELEMENT_WISE_CONSTANT_POW,
    ELEMENT_WISE_RECIP,
    ELEMENT_WISE_SIN,
    ELEMENT_WISE_SQRT,
    ELEMENT_WISE_SUBTRACT,
    ELEMENT_WISE_TAN,
    ELEMENT_WISE_THRESHOLD,
    ELEMENT_WISE_QUANTIZE_LINEAR,
    ELEMENT_WISE_DEQUANTIZE_LINEAR,
    ACTIVATION_ELU,
    ACTIVATION_HARDMAX,
    ACTIVATION_HARD_SIGMOID,
    ACTIVATION_IDENTITY,
    ACTIVATION_LEAKY_RELU,
    ACTIVATION_LINEAR,
    ACTIVATION_LOG_SOFTMAX,
    ACTIVATION_PARAMETERIZED_RELU,
    ACTIVATION_PARAMETRIC_SOFTPLUS,
    ACTIVATION_RELU,
    ACTIVATION_SCALED_ELU,
    ACTIVATION_SCALED_TANH,
    ACTIVATION_SIGMOID,
    ACTIVATION_SOFTMAX,
    ACTIVATION_SOFTPLUS,
    ACTIVATION_SOFTSIGN,
    ACTIVATION_TANH,
    ACTIVATION_THRESHOLDED_RELU,
    CONVOLUTION,
    GEMM,
    REDUCE,
    AVERAGE_POOLING,
    LP_POOLING,
    MAX_POOLING,
    ROI_POOLING,
    SLICE,
    CAST,
    SPLIT,
    JOIN,
    PADDING,
    VALUE_SCALE_2D,
    UPSAMPLE_2D,
    GATHER,
    SPACE_TO_DEPTH,
    DEPTH_TO_SPACE,
    TILE,
    TOP_K,
    BATCH_NORMALIZATION,
    MEAN_VARIANCE_NORMALIZATION,
    LOCAL_RESPONSE_NORMALIZATION,
    LP_NORMALIZATION,
    RNN,
    LSTM,
    GRU,

    // TARGET_VERSION >= 0x2000
    ELEMENT_WISE_SIGN,
    ELEMENT_WISE_IS_NAN,
    ELEMENT_WISE_ERF,
    ELEMENT_WISE_SINH,
    ELEMENT_WISE_COSH,
    ELEMENT_WISE_TANH,
    ELEMENT_WISE_ASINH,
    ELEMENT_WISE_ACOSH,
    ELEMENT_WISE_ATANH,
    ELEMENT_WISE_IF,
    ELEMENT_WISE_ADD1,
    ACTIVATION_SHRINK,
    MAX_POOLING1,
    MAX_UNPOOLING,
    DIAGONAL_MATRIX,
    SCATTER_ELEMENTS,
    ONE_HOT,
    RESAMPLE,

    // TARGET_VERSION >= 0x2100
    ELEMENT_WISE_BIT_SHIFT_LEFT,
    ELEMENT_WISE_BIT_SHIFT_RIGHT,
    ELEMENT_WISE_ROUND,
    ELEMENT_WISE_IS_INFINITY,
    ELEMENT_WISE_MODULUS_TRUNCATE,
    ELEMENT_WISE_MODULUS_FLOOR,
    FILL_VALUE_CONSTANT,
    FILL_VALUE_SEQUENCE,
    CUMULATIVE_SUMMATION,
    REVERSE_SUBSEQUENCES,
    GATHER_ELEMENTS,
    GATHER_ND,
    SCATTER_ND,
    MAX_POOLING2,
    SLICE1,
    TOP_K1,
    DEPTH_TO_SPACE1,
    SPACE_TO_DEPTH1,
    MEAN_VARIANCE_NORMALIZATION1,
    RESAMPLE1,
    MATRIX_MULTIPLY_INTEGER,
    QUANTIZED_LINEAR_MATRIX_MULTIPLY,
    CONVOLUTION_INTEGER,
    QUANTIZED_LINEAR_CONVOLUTION,

    // TARGET_VERSION >= 0x3000
    ELEMENT_WISE_BIT_AND,
    ELEMENT_WISE_BIT_OR,
    ELEMENT_WISE_BIT_XOR,
    ELEMENT_WISE_BIT_NOT,
    ELEMENT_WISE_BIT_COUNT,
    ELEMENT_WISE_LOGICAL_GREATER_THAN_OR_EQUAL,
    ELEMENT_WISE_LOGICAL_LESS_THAN_OR_EQUAL,
    ACTIVATION_CELU,
    ACTIVATION_RELU_GRAD,
    AVERAGE_POOLING_GRAD,
    MAX_POOLING_GRAD,
    RANDOM_GENERATOR,
    NONZERO_COORDINATES,
    RESAMPLE_GRAD,
    SLICE_GRAD,
    ADAM_OPTIMIZER,
    ARGMIN,
    ARGMAX,
    ROI_ALIGN,
    GATHER_ND1,

    // TARGET_VERSION >= 0x3100
    ELEMENT_WISE_ATAN_YX,
    ELEMENT_WISE_CLIP_GRAD,
    ELEMENT_WISE_DIFFERENCE_SQUARE,
    LOCAL_RESPONSE_NORMALIZATION_GRAD,
    CUMULATIVE_PRODUCT,
    BATCH_NORMALIZATION_GRAD,

    // TARGET_VERSION >= 0x4000
    ELEMENT_WISE_QUANTIZED_LINEAR_ADD,
    DYNAMIC_QUANTIZE_LINEAR,
    ROI_ALIGN1,

    // TARGET_VERSION >= 0x4100
    ROI_ALIGN_GRAD,
    BATCH_NORMALIZATION_TRAINING,
    BATCH_NORMALIZATION_TRAINING_GRAD,

    // TARGET_VERSION >= 0x5000
    ELEMENT_WISE_CLIP1,
    ELEMENT_WISE_CLIP_GRAD1,
    PADDING1,
    ELEMENT_WISE_NEGATE,

    // TARGET_VERSION >= 0x5100
    ACTIVATION_GELU,
    ACTIVATION_SOFTMAX1,
    ACTIVATION_LOG_SOFTMAX1,
    ACTIVATION_HARDMAX1,
    RESAMPLE2,
    RESAMPLE_GRAD1,
    DIAGONAL_MATRIX1,
};

//
// Operator enumerations and structures.
//
pub const CONVOLUTION_MODE = enum(UINT) {
    CONVOLUTION,
    CROSS_CORRELATION,
};

pub const CONVOLUTION_DIRECTION = enum(UINT) {
    FORWARD,
    BACKWARD,
};

pub const PADDING_MODE = enum(UINT) {
    CONSTANT,
    EDGE,
    REFLECTION,
    SYMMETRIC,
};

pub const INTERPOLATION_MODE = enum(UINT) {
    NEAREST_NEIGHBOR,
    LINEAR,
};

pub const SCALE_BIAS = extern struct {
    Scale: FLOAT,
    Bias: FLOAT,
};

pub const RANDOM_GENERATOR_TYPE = enum(UINT) {
    PHILOX_4X32_10,
};

//
// Operator descriptions.
//
pub const OPERATOR_DESC = extern struct {
    Type: OPERATOR_TYPE,
    Desc: *const anyopaque,
};

pub const ELEMENT_WISE_IDENTITY_OPERATOR_DESC = extern struct {
    InputTensor: *const TENSOR_DESC,
    OutputTensor: *const TENSOR_DESC,
    ScaleBias: ?*const SCALE_BIAS,
};

pub const RANDOM_GENERATOR_OPERATOR_DESC = extern struct {
    InputStateTensor: *const TENSOR_DESC,
    OutputTensor: *const TENSOR_DESC,
    OutputStateTensor: ?*const TENSOR_DESC,
    Type: RANDOM_GENERATOR_TYPE,
};

pub const CAST_OPERATOR_DESC = extern struct {
    InputTensor: *const TENSOR_DESC,
    OutputTensor: *const TENSOR_DESC,
};

pub const CONVOLUTION_OPERATOR_DESC = extern struct {
    InputTensor: *const TENSOR_DESC,
    FilterTensor: *const TENSOR_DESC,
    BiasTensor: ?*const TENSOR_DESC,
    OutputTensor: *const TENSOR_DESC,
    Mode: CONVOLUTION_MODE,
    Direction: CONVOLUTION_DIRECTION,
    DimensionCount: UINT,
    Strides: [*]const UINT,
    Dilations: [*]const UINT,
    StartPadding: [*]const UINT,
    EndPadding: [*]const UINT,
    OutputPadding: [*]const UINT,
    GroupCount: UINT,
    FusedActivation: ?*const OPERATOR_DESC,
};

//
// Interfaces.
//
pub const IID_IObject = GUID.parse("{c8263aac-9e0c-4a2d-9b8e-007521a3317c}");
pub const IObject = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn GetPrivateData(
                self: *T,
                guid: *const GUID,
                data_size: *UINT,
                data: ?*anyopaque,
            ) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v)
                    .GetPrivateData(@ptrCast(*IObject, self), guid, data_size, data);
            }
            pub inline fn SetPrivateData(
                self: *T,
                guid: *const GUID,
                data_size: UINT,
                data: ?*const anyopaque,
            ) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v)
                    .SetPrivateData(@ptrCast(*IObject, self), guid, data_size, data);
            }
            pub inline fn SetPrivateDataInterface(
                self: *T,
                guid: *const GUID,
                data: ?*const IUnknown,
            ) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v)
                    .SetPrivateDataInterface(@ptrCast(*IObject, self), guid, data);
            }
            pub inline fn SetName(self: *T, name: LPCWSTR) HRESULT {
                return @ptrCast(*const IObject.VTable, self.__v)
                    .SetName(@ptrCast(*IObject, self), name);
            }
        };
    }

    pub const VTable = extern struct {
        base: IUnknown.VTable,
        GetPrivateData: *const fn (*IObject, *const GUID, *UINT, ?*anyopaque) callconv(WINAPI) HRESULT,
        SetPrivateData: *const fn (*IObject, *const GUID, UINT, ?*const anyopaque) callconv(WINAPI) HRESULT,
        SetPrivateDataInterface: *const fn (*IObject, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
        SetName: *const fn (*IObject, LPCWSTR) callconv(WINAPI) HRESULT,
    };
};

pub const IID_IDeviceChild = GUID.parse("{27e83142-8165-49e3-974e-2fd66e4cb69d}");
pub const IDeviceChild = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IObject.Methods(T);

            pub inline fn GetDevice(self: *T, guid: *const GUID, device: *?*anyopaque) HRESULT {
                return @ptrCast(*const IDeviceChild.VTable, self.__v)
                    .GetDevice(@ptrCast(*IDeviceChild, self), guid, device);
            }
        };
    }

    pub const VTable = extern struct {
        base: IObject.VTable,
        GetDevice: *const fn (*IDeviceChild, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
    };
};

pub const IID_IPageable = GUID.parse("{b1ab0825-4542-4a4b-8617-6dde6e8f6201}");
pub const IPageable = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
    };
};

pub const IID_IOperator = GUID.parse("{26caae7a-3081-4633-9581-226fbe57695d}");
pub const IOperator = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
    };
};

pub const BINDING_PROPERTIES = extern struct {
    RequiredDescriptorCount: UINT,
    TemporaryResourceSize: UINT64,
    PersistentResourceSize: UINT64,
};

pub const IID_IDispatchable = GUID.parse("{dcb821a8-1039-441e-9f1c-b1759c2f3cec}");
pub const IDispatchable = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IPageable.Methods(T);

            pub inline fn GetBindingProperties(self: *T) BINDING_PROPERTIES {
                var properties: BINDING_PROPERTIES = undefined;
                _ = @ptrCast(*const IDispatchable.VTable, self.__v)
                    .GetBindingProperties(@ptrCast(*IDispatchable, self), &properties);
                return properties;
            }
        };
    }

    pub const VTable = extern struct {
        base: IPageable.VTable,
        GetBindingProperties: *const fn (
            *IDispatchable,
            *BINDING_PROPERTIES,
        ) callconv(WINAPI) *BINDING_PROPERTIES,
    };
};

pub const IID_ICompiledOperator = GUID.parse("{6b15e56a-bf5c-4902-92d8-da3a650afea4}");
pub const ICompiledOperator = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDispatchable.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IDispatchable.VTable,
    };
};

pub const IID_IOperatorInitializer = GUID.parse("{427c1113-435c-469c-8676-4d5dd072f813}");
pub const IOperatorInitializer = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDispatchable.Methods(T);

            pub inline fn Reset(self: *T, num_operators: UINT, operators: [*]const *ICompiledOperator) HRESULT {
                return @ptrCast(*const IOperatorInitializer.VTable, self.__v)
                    .Reset(@ptrCast(*IOperatorInitializer, self), num_operators, operators);
            }
        };
    }

    pub const VTable = extern struct {
        base: IDispatchable.VTable,
        Reset: *const fn (*IOperatorInitializer, UINT, [*]const *ICompiledOperator) callconv(WINAPI) HRESULT,
    };
};

pub const BINDING_TYPE = enum(UINT) {
    NONE,
    BUFFER,
    BUFFER_ARRAY,
};

pub const BINDING_DESC = extern struct {
    Type: BINDING_TYPE,
    Desc: ?*const anyopaque,
};

pub const BUFFER_BINDING = extern struct {
    Buffer: ?*d3d12.IResource,
    Offset: UINT64,
    SizeInBytes: UINT64,
};

pub const BUFFER_ARRAY_BINDING = extern struct {
    BindingCount: UINT,
    Bindings: [*]const BUFFER_BINDING,
};

pub const IID_IBindingTable = GUID.parse("{29c687dc-de74-4e3b-ab00-1168f2fc3cfc}");
pub const IBindingTable = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);

            pub inline fn BindInputs(self: *T, num: UINT, bindings: ?[*]const BINDING_DESC) void {
                @ptrCast(*const IBindingTable.VTable, self.__v)
                    .BindInputs(@ptrCast(*IBindingTable, self), num, bindings);
            }
            pub inline fn BindOutputs(self: *T, num: UINT, bindings: ?[*]const BINDING_DESC) void {
                @ptrCast(*const IBindingTable.VTable, self.__v)
                    .BindOutputs(@ptrCast(*IBindingTable, self), num, bindings);
            }
            pub inline fn BindTemporaryResource(self: *T, binding: ?*const BINDING_DESC) void {
                @ptrCast(*const IBindingTable.VTable, self.__v)
                    .BindTemporaryResource(@ptrCast(*IBindingTable, self), binding);
            }
            pub inline fn BindPersistentResource(self: *T, binding: ?*const BINDING_DESC) void {
                @ptrCast(*const IBindingTable.VTable, self.__v)
                    .BindPersistentResource(@ptrCast(*IBindingTable, self), binding);
            }
            pub inline fn Reset(self: *T, desc: ?*const BINDING_TABLE_DESC) HRESULT {
                return @ptrCast(*const IBindingTable.VTable, self.__v)
                    .Reset(@ptrCast(*IBindingTable, self), desc);
            }
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        BindInputs: *const fn (*IBindingTable, UINT, ?[*]const BINDING_DESC) callconv(WINAPI) void,
        BindOutputs: *const fn (*IBindingTable, UINT, ?[*]const BINDING_DESC) callconv(WINAPI) void,
        BindTemporaryResource: *const fn (*IBindingTable, ?*const BINDING_DESC) callconv(WINAPI) void,
        BindPersistentResource: *const fn (*IBindingTable, ?*const BINDING_DESC) callconv(WINAPI) void,
        Reset: *const fn (*IBindingTable, ?*const BINDING_TABLE_DESC) callconv(WINAPI) HRESULT,
    };
};

pub const IID_ICommandRecorder = GUID.parse("{e6857a76-2e3e-4fdd-bff4-5d2ba10fb453}");
pub const ICommandRecorder = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDeviceChild.Methods(T);

            pub inline fn RecordDispatch(
                self: *T,
                cmdlist: *d3d12.ICommandList,
                dispatchable: *IDispatchable,
                bindings: *IBindingTable,
            ) void {
                @ptrCast(*const ICommandRecorder.VTable, self.__v)
                    .RecordDispatch(@ptrCast(*ICommandRecorder, self), cmdlist, dispatchable, bindings);
            }
        };
    }

    pub const VTable = extern struct {
        base: IDeviceChild.VTable,
        RecordDispatch: *const fn (
            *ICommandRecorder,
            *d3d12.ICommandList,
            *IDispatchable,
            *IBindingTable,
        ) callconv(WINAPI) void,
    };
};

pub const IID_IDebugDevice = GUID.parse("{7d6f3ac9-394a-4ac3-92a7-390cc57a8217}");
pub const IDebugDevice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn SetMuteDebugOutput(self: *T, mute: BOOL) void {
                @ptrCast(*const IDebugDevice.VTable, self.v)
                    .SetMuteDebugOutput(@ptrCast(*IDebugDevice, self), mute);
            }
        };
    }

    pub const VTable = extern struct {
        base: IUnknown.VTable,
        SetMuteDebugOutput: *const fn (*IDebugDevice, BOOL) callconv(WINAPI) void,
    };
};

pub const IID_IDevice = GUID.parse("{6dbd6437-96fd-423f-a98c-ae5e7c2a573f}");
pub const IDevice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IObject.Methods(T);

            pub inline fn CheckFeatureSupport(
                self: *T,
                feature: FEATURE,
                feature_query_data_size: UINT,
                feature_query_data: ?*const anyopaque,
                feature_support_data_size: UINT,
                feature_support_data: *anyopaque,
            ) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v).CheckFeatureSupport(
                    @ptrCast(*IDevice, self),
                    feature,
                    feature_query_data_size,
                    feature_query_data,
                    feature_support_data_size,
                    feature_support_data,
                );
            }
            pub inline fn CreateOperator(
                self: *T,
                desc: *const OPERATOR_DESC,
                guid: *const GUID,
                ppv: ?*?*anyopaque,
            ) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .CreateOperator(@ptrCast(*IDevice, self), desc, guid, ppv);
            }
            pub inline fn CompileOperator(
                self: *T,
                op: *IOperator,
                flags: EXECUTION_FLAGS,
                guid: *const GUID,
                ppv: ?*?*anyopaque,
            ) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .CompileOperator(@ptrCast(*IDevice, self), op, flags, guid, ppv);
            }
            pub inline fn CreateOperatorInitializer(
                self: *T,
                num_ops: UINT,
                ops: ?[*]const *ICompiledOperator,
                guid: *const GUID,
                ppv: *?*anyopaque,
            ) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .CreateOperatorInitializer(@ptrCast(*IDevice, self), num_ops, ops, guid, ppv);
            }
            pub inline fn CreateCommandRecorder(self: *T, guid: *const GUID, ppv: *?*anyopaque) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .CreateCommandRecorder(@ptrCast(*IDevice, self), guid, ppv);
            }
            pub inline fn CreateBindingTable(
                self: *T,
                desc: ?*const BINDING_TABLE_DESC,
                guid: *const GUID,
                ppv: *?*anyopaque,
            ) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .CreateBindingTable(@ptrCast(*IDevice, self), desc, guid, ppv);
            }
            pub inline fn Evict(self: *T, num: UINT, objs: [*]const *IPageable) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .Evict(@ptrCast(*IDevice, self), num, objs);
            }
            pub inline fn MakeResident(self: *T, num: UINT, objs: [*]const *IPageable) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .MakeResident(@ptrCast(*IDevice, self), num, objs);
            }
            pub inline fn GetDeviceRemovedReason(self: *T) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .GetDeviceRemovedReason(@ptrCast(*IDevice, self));
            }
            pub inline fn GetParentDevice(self: *T, guid: *const GUID, ppv: *?*anyopaque) HRESULT {
                return @ptrCast(*const IDevice.VTable, self.__v)
                    .GetParentDevice(@ptrCast(*IDevice, self), guid, ppv);
            }
        };
    }

    pub const VTable = extern struct {
        base: IObject.VTable,
        CheckFeatureSupport: *const fn (
            *IDevice,
            FEATURE,
            UINT,
            ?*const anyopaque,
            UINT,
            *anyopaque,
        ) callconv(WINAPI) HRESULT,
        CreateOperator: *const fn (
            *IDevice,
            *const OPERATOR_DESC,
            *const GUID,
            ?*?*anyopaque,
        ) callconv(WINAPI) HRESULT,
        CompileOperator: *const fn (
            *IDevice,
            *IOperator,
            EXECUTION_FLAGS,
            *const GUID,
            ?*?*anyopaque,
        ) callconv(WINAPI) HRESULT,
        CreateOperatorInitializer: *const fn (
            *IDevice,
            UINT,
            ?[*]const *ICompiledOperator,
            *const GUID,
            *?*anyopaque,
        ) callconv(WINAPI) HRESULT,
        CreateCommandRecorder: *const fn (*IDevice, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        CreateBindingTable: *const fn (
            *IDevice,
            ?*const BINDING_TABLE_DESC,
            *const GUID,
            *?*anyopaque,
        ) callconv(WINAPI) HRESULT,
        Evict: *const fn (*IDevice, UINT, [*]const *IPageable) callconv(WINAPI) HRESULT,
        MakeResident: *const fn (*IDevice, UINT, [*]const *IPageable) callconv(WINAPI) HRESULT,
        GetDeviceRemovedReason: *const fn (*IDevice) callconv(WINAPI) HRESULT,
        GetParentDevice: *const fn (*IDevice, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
    };
};

pub const GRAPH_EDGE_TYPE = enum(UINT) {
    INVALID,
    INPUT,
    OUTPUT,
    INTERMEDIATE,
};

pub const GRAPH_EDGE_DESC = extern struct {
    Type: GRAPH_EDGE_TYPE,
    Desc: *const anyopaque,
};

pub const INPUT_GRAPH_EDGE_DESC = extern struct {
    GraphInputIndex: UINT,
    ToNodeIndex: UINT,
    ToNodeInputIndex: UINT,
    Name: ?LPCSTR,
};

pub const OUTPUT_GRAPH_EDGE_DESC = extern struct {
    FromNodeIndex: UINT,
    FromNodeOutputIndex: UINT,
    GraphOutputIndex: UINT,
    Name: ?LPCSTR,
};

pub const INTERMEDIATE_GRAPH_EDGE_DESC = extern struct {
    FromNodeIndex: UINT,
    FromNodeOutputIndex: UINT,
    ToNodeIndex: UINT,
    ToNodeInputIndex: UINT,
    Name: ?LPCSTR,
};

pub const GRAPH_NODE_TYPE = enum(UINT) {
    INVALID,
    OPERATOR,
};

pub const GRAPH_NODE_DESC = extern struct {
    Type: GRAPH_NODE_TYPE,
    Desc: *const anyopaque,
};

pub const OPERATOR_GRAPH_NODE_DESC = extern struct {
    Operator: *IOperator,
    Name: ?LPCSTR,
};

pub const GRAPH_DESC = extern struct {
    InputCount: UINT,
    OutputCount: UINT,

    NodeCount: UINT,
    Nodes: [*]const GRAPH_NODE_DESC,

    InputEdgeCount: UINT,
    InputEdges: ?[*]const GRAPH_EDGE_DESC,

    OutputEdgeCount: UINT,
    OutputEdges: [*]const GRAPH_EDGE_DESC,

    IntermediateEdgeCount: UINT,
    IntermediateEdges: ?[*]const GRAPH_EDGE_DESC,
};

pub const IID_IDevice1 = GUID.parse("{a0884f9a-d2be-4355-aa5d-5901281ad1d2}");
pub const IDevice1 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IDevice.Methods(T);

            pub inline fn CompileGraph(
                self: *T,
                desc: *const GRAPH_DESC,
                flags: EXECUTION_FLAGS,
                guid: *const GUID,
                ppv: ?*?*anyopaque,
            ) HRESULT {
                return @ptrCast(*const IDevice1.VTable, self.__v)
                    .CompileGraph(@ptrCast(*IDevice1, self), desc, flags, guid, ppv);
            }
        };
    }

    pub const VTable = extern struct {
        base: IDevice.VTable,
        CompileGraph: *const fn (
            *IDevice1,
            *const GRAPH_DESC,
            EXECUTION_FLAGS,
            *const GUID,
            ?*?*anyopaque,
        ) callconv(WINAPI) HRESULT,
    };
};

//
// DML feature support queries.
//
pub const FEATURE_LEVEL = enum(UINT) {
    @"1_0" = 0x1000,
    @"2_0" = 0x2000,
    @"2_1" = 0x2100,
    @"3_0" = 0x3000,
    @"3_1" = 0x3100,
    @"4_0" = 0x4000,
    @"4_1" = 0x4100,
    @"5_0" = 0x5000,
    @"5_1" = 0x5100,
    @"5_2" = 0x5200,
};

pub const FEATURE = enum(UINT) {
    TENSOR_DATA_TYPE_SUPPORT,
    FEATURE_LEVELS,
};

pub const FEATURE_QUERY_TENSOR_DATA_TYPE_SUPPORT = extern struct {
    DataType: TENSOR_DATA_TYPE,
};

pub const FEATURE_DATA_TENSOR_DATA_TYPE_SUPPORT = extern struct {
    IsSupported: BOOL,
};

pub const FEATURE_QUERY_FEATURE_LEVELS = extern struct {
    RequestedFeatureLevelCount: UINT,
    RequestedFeatureLevels: [*]const FEATURE_LEVEL,
};

pub const FEATURE_DATA_FEATURE_LEVELS = extern struct {
    MaxSupportedFeatureLevel: FEATURE_LEVEL,
};

//
// DML device functions, enumerations, and structures.
//
pub const BINDING_TABLE_DESC = extern struct {
    Dispatchable: *IDispatchable,
    CPUDescriptorHandle: d3d12.CPU_DESCRIPTOR_HANDLE,
    GPUDescriptorHandle: d3d12.GPU_DESCRIPTOR_HANDLE,
    SizeInDescriptors: UINT,
};

pub const EXECUTION_FLAGS = packed struct(UINT) {
    ALLOW_HALF_PRECISION_COMPUTATION: bool = false,
    DISABLE_META_COMMANDS: bool = false,
    DESCRIPTORS_VOLATILE: bool = false,
    __unused: u29 = 0,
};

pub const CREATE_DEVICE_FLAGS = packed struct(UINT) {
    DEBUG: bool = false,
    __unused: u31 = 0,
};

pub fn createDevice(
    d3d12_device: *d3d12.IDevice,
    flags: CREATE_DEVICE_FLAGS,
    min_feature_level: FEATURE_LEVEL,
    guid: *const GUID,
    ppv: ?*?*anyopaque,
) HRESULT {
    var directml_dll = w32.GetModuleHandleA("DirectML.dll");
    if (directml_dll == null) {
        directml_dll = w32.LoadLibraryA("DirectML.dll");
    }

    var dmlCreateDevice1: *const fn (
        *d3d12.IDevice,
        CREATE_DEVICE_FLAGS,
        FEATURE_LEVEL,
        *const GUID,
        ?*?*anyopaque,
    ) callconv(WINAPI) HRESULT = undefined;

    dmlCreateDevice1 = @ptrCast(
        @TypeOf(dmlCreateDevice1),
        w32.GetProcAddress(directml_dll.?, "DMLCreateDevice1").?,
    );

    return dmlCreateDevice1(d3d12_device, flags, min_feature_level, guid, ppv);
}

pub fn calcBufferTensorSize(
    data_type: TENSOR_DATA_TYPE,
    sizes: []const UINT,
    strides: ?[]const UINT,
) UINT64 {
    if (strides != null) assert(sizes.len == strides.?.len);

    const element_size_in_bytes: UINT = switch (data_type) {
        .FLOAT32, .UINT32, .INT32 => 4,
        .FLOAT16, .UINT16, .INT16 => 2,
        .UINT8, .INT8 => 1,
        .UNKNOWN, .FLOAT64, .UINT64, .INT64 => unreachable,
    };
    const min_implied_size_in_bytes = blk: {
        if (strides == null) {
            var size: UINT = 1;
            for (sizes) |s| size *= s;
            break :blk size * element_size_in_bytes;
        } else {
            var index_of_last_element: UINT = 0;
            for (sizes) |_, i| {
                index_of_last_element += (sizes[i] - 1) * strides.?[i];
            }
            break :blk (index_of_last_element + 1) * element_size_in_bytes;
        }
    };
    return (min_implied_size_in_bytes + 3) & ~@as(UINT64, 3);
}
