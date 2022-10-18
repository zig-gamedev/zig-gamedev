//
//  MTLArgument.h
//  Metal
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/MTLDefines.h>
#import <Metal/MTLTexture.h>


NS_ASSUME_NONNULL_BEGIN
typedef NS_ENUM(NSUInteger, MTLDataType) {

    MTLDataTypeNone = 0,

    MTLDataTypeStruct = 1,
    MTLDataTypeArray  = 2,

    MTLDataTypeFloat  = 3,
    MTLDataTypeFloat2 = 4,
    MTLDataTypeFloat3 = 5,
    MTLDataTypeFloat4 = 6,

    MTLDataTypeFloat2x2 = 7,
    MTLDataTypeFloat2x3 = 8,
    MTLDataTypeFloat2x4 = 9,

    MTLDataTypeFloat3x2 = 10,
    MTLDataTypeFloat3x3 = 11,
    MTLDataTypeFloat3x4 = 12,

    MTLDataTypeFloat4x2 = 13,
    MTLDataTypeFloat4x3 = 14,
    MTLDataTypeFloat4x4 = 15,

    MTLDataTypeHalf  = 16,
    MTLDataTypeHalf2 = 17,
    MTLDataTypeHalf3 = 18,
    MTLDataTypeHalf4 = 19,

    MTLDataTypeHalf2x2 = 20,
    MTLDataTypeHalf2x3 = 21,
    MTLDataTypeHalf2x4 = 22,

    MTLDataTypeHalf3x2 = 23,
    MTLDataTypeHalf3x3 = 24,
    MTLDataTypeHalf3x4 = 25,

    MTLDataTypeHalf4x2 = 26,
    MTLDataTypeHalf4x3 = 27,
    MTLDataTypeHalf4x4 = 28,

    MTLDataTypeInt  = 29,
    MTLDataTypeInt2 = 30,
    MTLDataTypeInt3 = 31,
    MTLDataTypeInt4 = 32,

    MTLDataTypeUInt  = 33,
    MTLDataTypeUInt2 = 34,
    MTLDataTypeUInt3 = 35,
    MTLDataTypeUInt4 = 36,

    MTLDataTypeShort  = 37,
    MTLDataTypeShort2 = 38,
    MTLDataTypeShort3 = 39,
    MTLDataTypeShort4 = 40,

    MTLDataTypeUShort = 41,
    MTLDataTypeUShort2 = 42,
    MTLDataTypeUShort3 = 43,
    MTLDataTypeUShort4 = 44,

    MTLDataTypeChar  = 45,
    MTLDataTypeChar2 = 46,
    MTLDataTypeChar3 = 47,
    MTLDataTypeChar4 = 48,

    MTLDataTypeUChar  = 49,
    MTLDataTypeUChar2 = 50,
    MTLDataTypeUChar3 = 51,
    MTLDataTypeUChar4 = 52,

    MTLDataTypeBool  = 53,
    MTLDataTypeBool2 = 54,
    MTLDataTypeBool3 = 55,
    MTLDataTypeBool4 = 56,

    MTLDataTypeTexture API_AVAILABLE(macos(10.13), ios(11.0)) = 58,
    MTLDataTypeSampler API_AVAILABLE(macos(10.13), ios(11.0)) = 59,
    MTLDataTypePointer API_AVAILABLE(macos(10.13), ios(11.0)) = 60,

    MTLDataTypeR8Unorm         API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 62,
    MTLDataTypeR8Snorm         API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 63,
    MTLDataTypeR16Unorm        API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 64,
    MTLDataTypeR16Snorm        API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 65,
    MTLDataTypeRG8Unorm        API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 66,
    MTLDataTypeRG8Snorm        API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 67,
    MTLDataTypeRG16Unorm       API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 68,
    MTLDataTypeRG16Snorm       API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 69,
    MTLDataTypeRGBA8Unorm      API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 70,
    MTLDataTypeRGBA8Unorm_sRGB API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 71,
    MTLDataTypeRGBA8Snorm      API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 72,
    MTLDataTypeRGBA16Unorm     API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 73,
    MTLDataTypeRGBA16Snorm     API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 74,
    MTLDataTypeRGB10A2Unorm    API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 75,
    MTLDataTypeRG11B10Float    API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 76,
    MTLDataTypeRGB9E5Float     API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5)) = 77,
    MTLDataTypeRenderPipeline  API_AVAILABLE(macos(10.14), ios(13.0)) = 78,
    MTLDataTypeComputePipeline API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(13.0)) = 79,
    MTLDataTypeIndirectCommandBuffer   API_AVAILABLE(macos(10.14), ios(12.0)) = 80,
    MTLDataTypeLong  API_AVAILABLE(macos(12.0), ios(14.0)) = 81,
    MTLDataTypeLong2 API_AVAILABLE(macos(12.0), ios(14.0)) = 82,
    MTLDataTypeLong3 API_AVAILABLE(macos(12.0), ios(14.0)) = 83,
    MTLDataTypeLong4 API_AVAILABLE(macos(12.0), ios(14.0)) = 84,

    MTLDataTypeULong  API_AVAILABLE(macos(12.0), ios(14.0)) = 85,
    MTLDataTypeULong2 API_AVAILABLE(macos(12.0), ios(14.0)) = 86,
    MTLDataTypeULong3 API_AVAILABLE(macos(12.0), ios(14.0)) = 87,
    MTLDataTypeULong4 API_AVAILABLE(macos(12.0), ios(14.0)) = 88,
    MTLDataTypeVisibleFunctionTable API_AVAILABLE(macos(11.0), ios(14.0)) = 115,
    MTLDataTypeIntersectionFunctionTable API_AVAILABLE(macos(11.0), ios(14.0)) = 116,
    MTLDataTypePrimitiveAccelerationStructure API_AVAILABLE(macos(11.0), ios(14.0)) = 117,
    MTLDataTypeInstanceAccelerationStructure API_AVAILABLE(macos(11.0), ios(14.0)) = 118,
} API_AVAILABLE(macos(10.11), ios(8.0));

@class MTLArgument;

/*!
 @enum MTLArgumentType
 @abstract The type for an input to a MTLRenderPipelineState or a MTLComputePipelineState
 
 @constant MTLArgumentTypeBuffer
 This input is a MTLBuffer
 
 @constant MTLArgumentTypeThreadgroupMemory
 This input is a pointer to the threadgroup memory.
 
 @constant MTLArgumentTypeTexture
 This input is a MTLTexture.
 
 @constant MTLArgumentTypeSampler
 This input is a sampler.
*/
typedef NS_ENUM(NSUInteger, MTLArgumentType) {

    MTLArgumentTypeBuffer = 0,
    MTLArgumentTypeThreadgroupMemory= 1,
    MTLArgumentTypeTexture = 2,
    MTLArgumentTypeSampler = 3,

    MTLArgumentTypeImageblockData API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5))     = 16,
    MTLArgumentTypeImageblock API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5))         = 17,
    MTLArgumentTypeVisibleFunctionTable API_AVAILABLE(macos(11.0), ios(14.0)) = 24,
    MTLArgumentTypePrimitiveAccelerationStructure API_AVAILABLE(macos(11.0), ios(14.0)) = 25,
    MTLArgumentTypeInstanceAccelerationStructure API_AVAILABLE(macos(11.0), ios(14.0)) = 26,
    MTLArgumentTypeIntersectionFunctionTable API_AVAILABLE(macos(11.0), ios(14.0)) = 27,
} API_AVAILABLE(macos(10.11), ios(8.0));

/*!
 @enum MTLArgumentAccess
*/
typedef NS_ENUM(NSUInteger, MTLArgumentAccess) {

    MTLArgumentAccessReadOnly   = 0,
    MTLArgumentAccessReadWrite  = 1,
    MTLArgumentAccessWriteOnly  = 2,
} API_AVAILABLE(macos(10.11), ios(8.0));

@class MTLStructType;
@class MTLArrayType;
@class MTLTextureReferenceType;
@class MTLPointerType;

MTL_EXPORT API_AVAILABLE(macos(10.13), ios(11.0))
@interface MTLType : NSObject
@property (readonly) MTLDataType dataType;
@end

MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLStructMember : NSObject
 
@property (readonly) NSString *name;
@property (readonly) NSUInteger offset;
@property (readonly) MTLDataType dataType;

- (nullable MTLStructType *)structType;
- (nullable MTLArrayType *)arrayType;

- (nullable MTLTextureReferenceType *)textureReferenceType  API_AVAILABLE(macos(10.13), ios(11.0));
- (nullable MTLPointerType *)pointerType  API_AVAILABLE(macos(10.13), ios(11.0));

@property (readonly) NSUInteger argumentIndex API_AVAILABLE(macos(10.13), ios(11.0));


@end
 
MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLStructType  : MTLType

@property (readonly) NSArray <MTLStructMember *> *members;

- (nullable MTLStructMember *)memberByName:(NSString *)name;

@end

MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLArrayType  : MTLType

@property (readonly) MTLDataType elementType;

@property (readonly) NSUInteger arrayLength;
@property (readonly) NSUInteger stride;
@property (readonly) NSUInteger argumentIndexStride API_AVAILABLE(macos(10.13), ios(11.0));
- (nullable MTLStructType *)elementStructType;
- (nullable MTLArrayType *)elementArrayType;
- (nullable MTLTextureReferenceType *)elementTextureReferenceType  API_AVAILABLE(macos(10.13), ios(11.0));
- (nullable MTLPointerType *)elementPointerType  API_AVAILABLE(macos(10.13), ios(11.0));


@end

MTL_EXPORT API_AVAILABLE(macos(10.13), ios(11.0))
@interface MTLPointerType : MTLType

@property (readonly) MTLDataType elementType;           // MTLDataTypeFloat, MTLDataTypeFloat4, MTLDataTypeStruct, ...
@property (readonly) MTLArgumentAccess access;
@property (readonly) NSUInteger alignment;              // min alignment for the element data
@property (readonly) NSUInteger dataSize;               // sizeof(T) for T *argName

@property (readonly) BOOL elementIsArgumentBuffer API_AVAILABLE(macos(10.13), ios(11.0));

- (nullable MTLStructType *)elementStructType  API_AVAILABLE(macos(10.13), ios(11.0));
- (nullable MTLArrayType *)elementArrayType  API_AVAILABLE(macos(10.13), ios(11.0));

@end

MTL_EXPORT API_AVAILABLE(macos(10.13), ios(11.0))
@interface MTLTextureReferenceType : MTLType

@property (readonly) MTLDataType textureDataType; // half, float, int, or uint.
@property (readonly) MTLTextureType textureType;  // texture1D, texture2D...
@property (readonly) MTLArgumentAccess access;    // read, write, read-write
@property (readonly) BOOL isDepthTexture;         // true for depth textures

@end

/*!
 MTLArgument
*/
MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLArgument : NSObject

@property (readonly) NSString *name;
@property (readonly) MTLArgumentType type;
@property (readonly) MTLArgumentAccess access;
@property (readonly) NSUInteger index;

@property (readonly, getter=isActive) BOOL active;

// for buffer arguments
@property (readonly) NSUInteger      bufferAlignment;		// min alignment of starting offset in the buffer
@property (readonly) NSUInteger      bufferDataSize; 		// sizeof(T) for T *argName
@property (readonly) MTLDataType     bufferDataType; 		// MTLDataTypeFloat, MTLDataTypeFloat4, MTLDataTypeStruct, ...
@property (readonly, nullable) MTLStructType  *bufferStructType;
@property (readonly, nullable) MTLPointerType *bufferPointerType API_AVAILABLE(macos(10.13), ios(11.0));

// for threadgroup memory arguments
@property (readonly) NSUInteger     threadgroupMemoryAlignment;
@property (readonly) NSUInteger     threadgroupMemoryDataSize; // sizeof(T) for T *argName

// for texture arguments
@property (readonly) MTLTextureType textureType; // texture1D, texture2D...
@property (readonly) MTLDataType    textureDataType; // half, float, int, or uint.
@property (readonly) BOOL           isDepthTexture API_AVAILABLE(macos(10.12), ios(10.0)); // true for depth textures
@property (readonly) NSUInteger     arrayLength API_AVAILABLE(macos(10.13), ios(10.0));

@end

NS_ASSUME_NONNULL_END
