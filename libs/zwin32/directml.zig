const std = @import("std");
const windows = @import("windows.zig");
const d3d12 = @import("d3d12.zig");
const UINT = windows.UINT;
const UINT64 = windows.UINT64;
const FLOAT = windows.FLOAT;
const IUnknown = windows.IUnknown;
const HRESULT = windows.HRESULT;
const WINAPI = windows.WINAPI;
const GUID = windows.GUID;
const LPCWSTR = windows.LPCWSTR;
const LPCSTR = windows.LPCSTR;
const BOOL = windows.BOOL;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const assert = std.debug.assert;

//
// DirectML constants.
//
pub const TARGET_VERSION = 0x4100;

pub const TENSOR_DIMENSION_COUNT_MAX: UINT = 5;
pub const TENSOR_DIMENSION_COUNT_MAX1: UINT = 8;

pub const TEMPORARY_BUFFER_ALIGNMENT: UINT = 256;
pub const PERSISTENT_BUFFER_ALIGNMENT: UINT = 256;

pub const MINIMUM_BUFFER_TENSOR_ALIGNMENT: UINT = 16;

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

pub const TENSOR_FLAGS = UINT;
pub const TENSOR_FLAG_NONE: TENSOR_FLAGS = 0x0;
pub const TENSOR_FLAG_OWNED_BY_DML: TENSOR_FLAGS = 0x1;

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
    // TARGET_VERSION >= 0x2000

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
    // TARGET_VERSION >= 0x2100

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
    // TARGET_VERSION >= 0x3000

    // TARGET_VERSION >= 0x3100
    ELEMENT_WISE_ATAN_YX,
    ELEMENT_WISE_CLIP_GRAD,
    ELEMENT_WISE_DIFFERENCE_SQUARE,
    LOCAL_RESPONSE_NORMALIZATION_GRAD,
    CUMULATIVE_PRODUCT,
    BATCH_NORMALIZATION_GRAD,
    // TARGET_VERSION >= 0x3100

    // TARGET_VERSION >= 0x4000
    ELEMENT_WISE_QUANTIZED_LINEAR_ADD,
    DYNAMIC_QUANTIZE_LINEAR,
    ROI_ALIGN1,
    // TARGET_VERSION >= 0x4000

    // TARGET_VERSION >= 0x4100
    ROI_ALIGN_GRAD,
    BATCH_NORMALIZATION_TRAINING,
    BATCH_NORMALIZATION_TRAINING_GRAD,
    // TARGET_VERSION >= 0x4100
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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetPrivateData(self: *T, guid: *const GUID, data_size: *UINT, data: ?*anyopaque) HRESULT {
                return self.v.object.GetPrivateData(self, guid, data_size, data);
            }
            pub inline fn SetPrivateData(self: *T, guid: *const GUID, data_size: UINT, data: ?*const anyopaque) HRESULT {
                return self.v.object.SetPrivateData(self, guid, data_size, data);
            }
            pub inline fn SetPrivateDataInterface(self: *T, guid: *const GUID, data: ?*const IUnknown) HRESULT {
                return self.v.object.SetPrivateDataInterface(self, guid, data);
            }
            pub inline fn SetName(self: *T, name: LPCWSTR) HRESULT {
                return self.v.object.SetName(self, name);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetPrivateData: fn (*T, *const GUID, *UINT, ?*anyopaque) callconv(WINAPI) HRESULT,
            SetPrivateData: fn (*T, *const GUID, UINT, ?*const anyopaque) callconv(WINAPI) HRESULT,
            SetPrivateDataInterface: fn (*T, *const GUID, ?*const IUnknown) callconv(WINAPI) HRESULT,
            SetName: fn (*T, LPCWSTR) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_IDeviceChild = GUID.parse("{27e83142-8165-49e3-974e-2fd66e4cb69d}");
pub const IDeviceChild = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDevice(self: *T, guid: *const GUID, device: *?*anyopaque) HRESULT {
                return self.v.devchild.GetDevice(self, guid, device);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetDevice: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_IPageable = GUID.parse("{b1ab0825-4542-4a4b-8617-6dde6e8f6201}");
pub const IPageable = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IID_IOperator = GUID.parse("{26caae7a-3081-4633-9581-226fbe57695d}");
pub const IOperator = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        operator: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const BINDING_PROPERTIES = extern struct {
    RequiredDescriptorCount: UINT,
    TemporaryResourceSize: UINT64,
    PersistentResourceSize: UINT64,
};

pub const IID_IDispatchable = GUID.parse("{dcb821a8-1039-441e-9f1c-b1759c2f3cec}");
pub const IDispatchable = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        dispatchable: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetBindingProperties(self: *T) BINDING_PROPERTIES {
                var properties: BINDING_PROPERTIES = undefined;
                _ = self.v.dispatchable.GetBindingProperties(self, &properties);
                return properties;
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetBindingProperties: fn (*T, *BINDING_PROPERTIES) callconv(WINAPI) *BINDING_PROPERTIES,
        };
    }
};

pub const IID_ICompiledOperator = GUID.parse("{6b15e56a-bf5c-4902-92d8-da3a650afea4}");
pub const ICompiledOperator = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        dispatchable: IDispatchable.VTable(Self),
        coperator: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace IDispatchable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IID_IOperatorInitializer = GUID.parse("{427c1113-435c-469c-8676-4d5dd072f813}");
pub const IOperatorInitializer = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        pageable: IPageable.VTable(Self),
        dispatchable: IDispatchable.VTable(Self),
        init: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace IPageable.Methods(Self);
    usingnamespace IDispatchable.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Reset(self: *T, num_operators: UINT, operators: [*]const *ICompiledOperator) HRESULT {
                return self.v.init.Reset(self, num_operators, operators);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Reset: fn (*T, UINT, [*]const *ICompiledOperator) callconv(WINAPI) HRESULT,
        };
    }
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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        bindtable: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn BindInputs(self: *T, num: UINT, bindings: ?[*]const BINDING_DESC) void {
                self.v.bindtable.BindInputs(self, num, bindings);
            }
            pub inline fn BindOutputs(self: *T, num: UINT, bindings: ?[*]const BINDING_DESC) void {
                self.v.bindtable.BindOutputs(self, num, bindings);
            }
            pub inline fn BindTemporaryResource(self: *T, binding: ?*const BINDING_DESC) void {
                self.v.bindtable.BindTemporaryResource(self, binding);
            }
            pub inline fn BindPersistentResource(self: *T, binding: ?*const BINDING_DESC) void {
                self.v.bindtable.BindPersistentResource(self, binding);
            }
            pub inline fn Reset(self: *T, desc: ?*const BINDING_TABLE_DESC) HRESULT {
                return self.v.bindtable.Reset(self, desc);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            BindInputs: fn (*T, UINT, ?[*]const BINDING_DESC) callconv(WINAPI) void,
            BindOutputs: fn (*T, UINT, ?[*]const BINDING_DESC) callconv(WINAPI) void,
            BindTemporaryResource: fn (*T, ?*const BINDING_DESC) callconv(WINAPI) void,
            BindPersistentResource: fn (*T, ?*const BINDING_DESC) callconv(WINAPI) void,
            Reset: fn (*T, ?*const BINDING_TABLE_DESC) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IID_ICommandRecorder = GUID.parse("{e6857a76-2e3e-4fdd-bff4-5d2ba10fb453}");
pub const ICommandRecorder = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        devchild: IDeviceChild.VTable(Self),
        cmdrec: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDeviceChild.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn RecordDispatch(
                self: *T,
                cmdlist: *d3d12.ICommandList,
                dispatchable: *IDispatchable,
                bindings: *IBindingTable,
            ) void {
                self.v.cmdrec.RecordDispatch(self, cmdlist, dispatchable, bindings);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            RecordDispatch: fn (*T, *d3d12.ICommandList, *IDispatchable, *IBindingTable) callconv(WINAPI) void,
        };
    }
};

pub const IID_IDebugDevice = GUID.parse("{7d6f3ac9-394a-4ac3-92a7-390cc57a8217}");
pub const IDebugDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        debugdev: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetMuteDebugOutput(self: *T, mute: BOOL) void {
                self.v.debugdev.SetMuteDebugOutput(self, mute);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            SetMuteDebugOutput: fn (*T, BOOL) callconv(WINAPI) void,
        };
    }
};

pub const IID_IDevice = GUID.parse("{6dbd6437-96fd-423f-a98c-ae5e7c2a573f}");
pub const IDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CheckFeatureSupport(
                self: *T,
                feature: FEATURE,
                feature_query_data_size: UINT,
                feature_query_data: ?*const anyopaque,
                feature_support_data_size: UINT,
                feature_support_data: *anyopaque,
            ) HRESULT {
                return self.v.device.CheckFeatureSupport(
                    self,
                    feature,
                    feature_query_data_size,
                    feature_query_data,
                    feature_support_data_size,
                    feature_support_data,
                );
            }
            pub inline fn CreateOperator(self: *T, desc: *const OPERATOR_DESC, guid: *const GUID, ppv: ?*?*anyopaque) HRESULT {
                return self.v.device.CreateOperator(self, desc, guid, ppv);
            }
            pub inline fn CompileOperator(
                self: *T,
                op: *IOperator,
                flags: EXECUTION_FLAGS,
                guid: *const GUID,
                ppv: ?*?*anyopaque,
            ) HRESULT {
                return self.v.device.CompileOperator(self, op, flags, guid, ppv);
            }
            pub inline fn CreateOperatorInitializer(
                self: *T,
                num_ops: UINT,
                ops: ?[*]const *ICompiledOperator,
                guid: *const GUID,
                ppv: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateOperatorInitializer(self, num_ops, ops, guid, ppv);
            }
            pub inline fn CreateCommandRecorder(self: *T, guid: *const GUID, ppv: *?*anyopaque) HRESULT {
                return self.v.device.CreateCommandRecorder(self, guid, ppv);
            }
            pub inline fn CreateBindingTable(
                self: *T,
                desc: ?*const BINDING_TABLE_DESC,
                guid: *const GUID,
                ppv: *?*anyopaque,
            ) HRESULT {
                return self.v.device.CreateBindingTable(self, desc, guid, ppv);
            }
            pub inline fn Evict(self: *T, num: UINT, objs: [*]const *IPageable) HRESULT {
                return self.v.device.Evict(self, num, objs);
            }
            pub inline fn MakeResident(self: *T, num: UINT, objs: [*]const *IPageable) HRESULT {
                return self.v.device.MakeResident(self, num, objs);
            }
            pub inline fn GetDeviceRemovedReason(self: *T) HRESULT {
                return self.v.device.GetDeviceRemovedReason(self);
            }
            pub inline fn GetParentDevice(self: *T, guid: *const GUID, ppv: *?*anyopaque) HRESULT {
                return self.v.device.GetParentDevice(self, guid, ppv);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CheckFeatureSupport: fn (*T, FEATURE, UINT, ?*const anyopaque, UINT, *anyopaque) callconv(WINAPI) HRESULT,
            CreateOperator: fn (*T, *const OPERATOR_DESC, *const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT,
            CompileOperator: fn (*T, *IOperator, EXECUTION_FLAGS, *const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT,
            CreateOperatorInitializer: fn (
                *T,
                UINT,
                ?[*]const *ICompiledOperator,
                *const GUID,
                *?*anyopaque,
            ) callconv(WINAPI) HRESULT,
            CreateCommandRecorder: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            CreateBindingTable: fn (*T, ?*const BINDING_TABLE_DESC, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
            Evict: fn (*T, UINT, [*]const *IPageable) callconv(WINAPI) HRESULT,
            MakeResident: fn (*T, UINT, [*]const *IPageable) callconv(WINAPI) HRESULT,
            GetDeviceRemovedReason: fn (*T) callconv(WINAPI) HRESULT,
            GetParentDevice: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
        };
    }
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
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        object: IObject.VTable(Self),
        device: IDevice.VTable(Self),
        device1: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IObject.Methods(Self);
    usingnamespace IDevice.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn CompileGraph(
                self: *T,
                desc: *const GRAPH_DESC,
                flags: EXECUTION_FLAGS,
                guid: *const GUID,
                ppv: ?*?*anyopaque,
            ) HRESULT {
                return self.v.device1.CompileGraph(self, desc, flags, guid, ppv);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            CompileGraph: fn (*T, *const GRAPH_DESC, EXECUTION_FLAGS, *const GUID, ?*?*anyopaque) callconv(WINAPI) HRESULT,
        };
    }
};

//
// DML feature support queries.
//
pub const FEATURE_LEVEL = enum(UINT) {
    FL_1_0 = 0x1000,
    FL_2_0 = 0x2000,
    FL_2_1 = 0x2100,
    FL_3_0 = 0x3000,
    FL_3_1 = 0x3100,
    FL_4_0 = 0x4000,
    FL_4_1 = 0x4100,
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

pub const EXECUTION_FLAGS = UINT;
pub const EXECUTION_FLAG_NONE: EXECUTION_FLAGS = 0;
pub const EXECUTION_FLAG_ALLOW_HALF_PRECISION_COMPUTATION: EXECUTION_FLAGS = 0x1;
pub const EXECUTION_FLAG_DISABLE_META_COMMANDS: EXECUTION_FLAGS = 0x2;
pub const EXECUTION_FLAG_DESCRIPTORS_VOLATILE: EXECUTION_FLAGS = 0x4;

pub const CREATE_DEVICE_FLAGS = UINT;
pub const CREATE_DEVICE_FLAG_NONE: CREATE_DEVICE_FLAGS = 0;
pub const CREATE_DEVICE_FLAG_DEBUG: CREATE_DEVICE_FLAGS = 0x1;

pub fn createDevice(
    d3d12_device: *d3d12.IDevice,
    flags: CREATE_DEVICE_FLAGS,
    min_feature_level: FEATURE_LEVEL,
    guid: *const GUID,
    ppv: ?*?*anyopaque,
) HRESULT {
    var DMLCreateDevice1: fn (
        *d3d12.IDevice,
        CREATE_DEVICE_FLAGS,
        FEATURE_LEVEL,
        *const GUID,
        ?*?*anyopaque,
    ) callconv(WINAPI) HRESULT = undefined;

    var directml_dll = windows.kernel32.GetModuleHandleW(L("DirectML.dll"));
    if (directml_dll == null) {
        directml_dll = (std.DynLib.openZ("DirectML.dll") catch unreachable).dll;
    }

    DMLCreateDevice1 = @ptrCast(
        @TypeOf(DMLCreateDevice1),
        windows.kernel32.GetProcAddress(directml_dll.?, "DMLCreateDevice1").?,
    );

    return DMLCreateDevice1(d3d12_device, flags, min_feature_level, guid, ppv);
}

pub fn calcBufferTensorSize(data_type: TENSOR_DATA_TYPE, sizes: []const UINT, strides: ?[]const UINT) UINT64 {
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
