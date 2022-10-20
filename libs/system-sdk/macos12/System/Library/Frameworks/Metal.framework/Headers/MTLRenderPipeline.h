//
//  MTLRenderPipeline.h
//  Metal
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Metal/MTLDefines.h>
#import <Metal/MTLDevice.h>
#import <Metal/MTLRenderCommandEncoder.h>
#import <Metal/MTLRenderPass.h>
#import <Metal/MTLPixelFormat.h>
#import <Metal/MTLArgument.h>
#import <Metal/MTLFunctionConstantValues.h>
#import <Metal/MTLPipeline.h>


#import <Metal/MTLLinkedFunctions.h>
#import <Metal/MTLFunctionHandle.h>

NS_ASSUME_NONNULL_BEGIN
@class MTLVertexDescriptor;

typedef NS_ENUM(NSUInteger, MTLBlendFactor) {
    MTLBlendFactorZero = 0,
    MTLBlendFactorOne = 1,
    MTLBlendFactorSourceColor = 2,
    MTLBlendFactorOneMinusSourceColor = 3,
    MTLBlendFactorSourceAlpha = 4,
    MTLBlendFactorOneMinusSourceAlpha = 5,
    MTLBlendFactorDestinationColor = 6,
    MTLBlendFactorOneMinusDestinationColor = 7,
    MTLBlendFactorDestinationAlpha = 8,
    MTLBlendFactorOneMinusDestinationAlpha = 9,
    MTLBlendFactorSourceAlphaSaturated = 10,
    MTLBlendFactorBlendColor = 11,
    MTLBlendFactorOneMinusBlendColor = 12,
    MTLBlendFactorBlendAlpha = 13,
    MTLBlendFactorOneMinusBlendAlpha = 14,
    MTLBlendFactorSource1Color              API_AVAILABLE(macos(10.12), ios(10.11)) = 15,
    MTLBlendFactorOneMinusSource1Color      API_AVAILABLE(macos(10.12), ios(10.11)) = 16,
    MTLBlendFactorSource1Alpha              API_AVAILABLE(macos(10.12), ios(10.11)) = 17,
    MTLBlendFactorOneMinusSource1Alpha      API_AVAILABLE(macos(10.12), ios(10.11)) = 18,
} API_AVAILABLE(macos(10.11), ios(8.0));

typedef NS_ENUM(NSUInteger, MTLBlendOperation) {
    MTLBlendOperationAdd = 0,
    MTLBlendOperationSubtract = 1,
    MTLBlendOperationReverseSubtract = 2,
    MTLBlendOperationMin = 3,
    MTLBlendOperationMax = 4,
} API_AVAILABLE(macos(10.11), ios(8.0));

typedef NS_OPTIONS(NSUInteger, MTLColorWriteMask) {
    MTLColorWriteMaskNone  = 0,
    MTLColorWriteMaskRed   = 0x1 << 3,
    MTLColorWriteMaskGreen = 0x1 << 2,
    MTLColorWriteMaskBlue  = 0x1 << 1,
    MTLColorWriteMaskAlpha = 0x1 << 0,
    MTLColorWriteMaskAll   = 0xf
} API_AVAILABLE(macos(10.11), ios(8.0));

typedef NS_ENUM(NSUInteger, MTLPrimitiveTopologyClass) {
    MTLPrimitiveTopologyClassUnspecified = 0,
    MTLPrimitiveTopologyClassPoint = 1,
    MTLPrimitiveTopologyClassLine = 2,
    MTLPrimitiveTopologyClassTriangle = 3,
} API_AVAILABLE(macos(10.11), ios(12.0));

typedef NS_ENUM(NSUInteger, MTLTessellationPartitionMode) {
    MTLTessellationPartitionModePow2 = 0,
    MTLTessellationPartitionModeInteger = 1,
    MTLTessellationPartitionModeFractionalOdd = 2,
    MTLTessellationPartitionModeFractionalEven = 3,
} API_AVAILABLE(macos(10.12), ios(10.0));

typedef NS_ENUM(NSUInteger, MTLTessellationFactorStepFunction) {
    MTLTessellationFactorStepFunctionConstant = 0,
    MTLTessellationFactorStepFunctionPerPatch = 1,
    MTLTessellationFactorStepFunctionPerInstance = 2,
    MTLTessellationFactorStepFunctionPerPatchAndPerInstance = 3,
} API_AVAILABLE(macos(10.12), ios(10.0));

typedef NS_ENUM(NSUInteger, MTLTessellationFactorFormat) {
    MTLTessellationFactorFormatHalf = 0,
} API_AVAILABLE(macos(10.12), ios(10.0));

typedef NS_ENUM(NSUInteger, MTLTessellationControlPointIndexType) {
    MTLTessellationControlPointIndexTypeNone = 0,
    MTLTessellationControlPointIndexTypeUInt16 = 1,
    MTLTessellationControlPointIndexTypeUInt32 = 2,
} API_AVAILABLE(macos(10.12), ios(10.0));

@class MTLRenderPipelineColorAttachmentDescriptorArray;

MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLRenderPipelineColorAttachmentDescriptor : NSObject <NSCopying>

/*! Pixel format.  Defaults to MTLPixelFormatInvalid */
@property (nonatomic) MTLPixelFormat pixelFormat;

/*! Enable blending.  Defaults to NO. */
@property (nonatomic, getter = isBlendingEnabled) BOOL blendingEnabled;

/*! Defaults to MTLBlendFactorOne */
@property (nonatomic) MTLBlendFactor sourceRGBBlendFactor;

/*! Defaults to MTLBlendFactorZero */
@property (nonatomic) MTLBlendFactor destinationRGBBlendFactor;

/*! Defaults to MTLBlendOperationAdd */
@property (nonatomic) MTLBlendOperation rgbBlendOperation;

/*! Defaults to MTLBlendFactorOne */
@property (nonatomic) MTLBlendFactor sourceAlphaBlendFactor;

/*! Defaults to MTLBlendFactorZero */
@property (nonatomic) MTLBlendFactor destinationAlphaBlendFactor;

/*! Defaults to MTLBlendOperationAdd */
@property (nonatomic) MTLBlendOperation alphaBlendOperation;

/* Other Options */

/*! Defaults to MTLColorWriteMaskAll */
@property (nonatomic) MTLColorWriteMask writeMask;

@end


MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLRenderPipelineReflection : NSObject

@property (nullable, readonly) NSArray <MTLArgument *> *vertexArguments;
@property (nullable, readonly) NSArray <MTLArgument *> *fragmentArguments;
@property (nullable, readonly) NSArray <MTLArgument *> *tileArguments API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5));
@end

MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLRenderPipelineDescriptor : NSObject <NSCopying>

@property (nullable, copy, nonatomic) NSString *label;

@property (nullable, readwrite, nonatomic, strong) id <MTLFunction> vertexFunction;
@property (nullable, readwrite, nonatomic, strong) id <MTLFunction> fragmentFunction;

@property (nullable, copy, nonatomic) MTLVertexDescriptor *vertexDescriptor;

/* Rasterization and visibility state */
@property (readwrite, nonatomic) NSUInteger sampleCount; //DEPRECATED - aliases rasterSampleCount property
@property (readwrite, nonatomic) NSUInteger rasterSampleCount;
@property (readwrite, nonatomic, getter = isAlphaToCoverageEnabled) BOOL alphaToCoverageEnabled;
@property (readwrite, nonatomic, getter = isAlphaToOneEnabled) BOOL alphaToOneEnabled;
@property (readwrite, nonatomic, getter = isRasterizationEnabled) BOOL rasterizationEnabled;


@property (readwrite, nonatomic) NSUInteger maxVertexAmplificationCount API_AVAILABLE(macos(10.15.4), ios(13.0), macCatalyst(13.4));

@property (readonly) MTLRenderPipelineColorAttachmentDescriptorArray *colorAttachments;

@property (nonatomic) MTLPixelFormat depthAttachmentPixelFormat;
@property (nonatomic) MTLPixelFormat stencilAttachmentPixelFormat;

@property (readwrite, nonatomic) MTLPrimitiveTopologyClass inputPrimitiveTopology API_AVAILABLE(macos(10.11), ios(12.0), tvos(14.5));

@property (readwrite, nonatomic) MTLTessellationPartitionMode tessellationPartitionMode API_AVAILABLE(macos(10.12), ios(10.0));
@property (readwrite, nonatomic) NSUInteger maxTessellationFactor API_AVAILABLE(macos(10.12), ios(10.0));
@property (readwrite, nonatomic, getter = isTessellationFactorScaleEnabled) BOOL tessellationFactorScaleEnabled API_AVAILABLE(macos(10.12), ios(10.0));
@property (readwrite, nonatomic) MTLTessellationFactorFormat tessellationFactorFormat API_AVAILABLE(macos(10.12), ios(10.0));
@property (readwrite, nonatomic) MTLTessellationControlPointIndexType tessellationControlPointIndexType API_AVAILABLE(macos(10.12), ios(10.0));
@property (readwrite, nonatomic) MTLTessellationFactorStepFunction tessellationFactorStepFunction API_AVAILABLE(macos(10.12), ios(10.0));
@property (readwrite, nonatomic) MTLWinding tessellationOutputWindingOrder API_AVAILABLE(macos(10.12), ios(10.0));

@property (readonly) MTLPipelineBufferDescriptorArray *vertexBuffers API_AVAILABLE(macos(10.13), ios(11.0));
@property (readonly) MTLPipelineBufferDescriptorArray *fragmentBuffers API_AVAILABLE(macos(10.13), ios(11.0));

@property (readwrite, nonatomic) BOOL supportIndirectCommandBuffers API_AVAILABLE(macos(10.14), ios(12.0));

/*!
 @property binaryArchives
 @abstract The set of MTLBinaryArchive to search for compiled code when creating the pipeline state.
 @discussion Accelerate pipeline state creation by providing archives of compiled code such that no compilation needs to happen on the fast path.
 @see MTLBinaryArchive
 */
@property (readwrite, nullable, nonatomic, copy) NSArray<id<MTLBinaryArchive>> *binaryArchives API_AVAILABLE(macos(11.0), ios(14.0));


/*!
 @property vertexPreloadedLibraries
 @abstract The set of MTLDynamicLibrary to use to resolve external symbols for the vertexFunction before considering symbols from dependent MTLDynamicLibrary.
 @discussion Typical workflows use the libraries property of MTLCompileOptions to record dependent libraries at compile time without having to use vertexPreloadedLibraries.
 This property can be used to override symbols from dependent libraries for experimentation or evaluating alternative implementations.
 It can also be used to provide dynamic libraries that are dynamically created (for example, from source) that have no stable installName that can be used to automatically load from the file system.
 @see MTLDynamicLibrary
 */
@property (readwrite, nonnull, nonatomic, copy) NSArray<id<MTLDynamicLibrary>>* vertexPreloadedLibraries API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property fragmentPreloadedLibraries
 @abstract The set of MTLDynamicLibrary to use to resolve external symbols for the fragmentFunction before considering symbols from dependent MTLDynamicLibrary.
 @discussion Typical workflows use the libraries property of MTLCompileOptions to record dependent libraries at compile time without having to use fragmentPreloadedLibraries.
 This property can be used to override symbols from dependent libraries for experimentation or evaluating alternative implementations.
 It can also be used to provide dynamic libraries that are dynamically created (for example, from source) that have no stable installName that can be used to automatically load from the file system.
 @see MTLDynamicLibrary
 */
@property (readwrite, nonnull, nonatomic, copy) NSArray<id<MTLDynamicLibrary>>* fragmentPreloadedLibraries API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property vertexLinkedFunctions
 @abstract The set of functions to be linked with the pipeline state and accessed from the vertex function.
 @see MTLLinkedFunctions
 */
@property (null_resettable, copy, nonatomic) MTLLinkedFunctions *vertexLinkedFunctions
    API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property fragmentLinkedFunctions
 @abstract The set of functions to be linked with the pipeline state and accessed from the fragment function.
 @see MTLLinkedFunctions
 */
@property (null_resettable, copy, nonatomic) MTLLinkedFunctions *fragmentLinkedFunctions
    API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property supportAddingVertexBinaryFunctions
 @abstract This flag makes this pipeline support creating a new pipeline by adding binary functions.
 */
@property (readwrite, nonatomic) BOOL supportAddingVertexBinaryFunctions
API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property supportFragmentAddingBinaryFunctions
 @abstract This flag makes this pipeline support creating a new pipeline by adding binary functions.
 */
@property (readwrite, nonatomic) BOOL supportAddingFragmentBinaryFunctions
API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property maxVertexCallStackDepth
 @abstract The maximum depth of the call stack in stack frames from the shader. Defaults to 1 additional stack frame.
 */
@property (readwrite, nonatomic) NSUInteger maxVertexCallStackDepth
API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property maxFragmentCallStackDepth
 @abstract The maximum depth of the call stack in stack frames from the shader. Defaults to 1 additional stack frame.
 */
@property (readwrite, nonatomic) NSUInteger maxFragmentCallStackDepth
API_AVAILABLE(macos(12.0), ios(15.0));


/*!
 @method reset
 @abstract Restore all pipeline descriptor properties to their default values.
 */
- (void)reset;

@end

MTL_EXPORT API_AVAILABLE(macos(12.0), ios(15.0))
@interface MTLRenderPipelineFunctionsDescriptor : NSObject <NSCopying>

/*!
 @property vertexAdditionalBinaryFunctions
 @abstract The set of additional binary functions to be accessed from the vertex function in an incrementally created pipeline state.
 */
@property (nullable, nonatomic, copy) NSArray<id<MTLFunction>> *vertexAdditionalBinaryFunctions;

/*!
 @property fragmentAdditionalBinaryFunctions
 @abstract The set of additional binary functions to be accessed from the fragment function in an incrementally created pipeline state.
 */
@property (nullable, nonatomic, copy) NSArray<id<MTLFunction>> *fragmentAdditionalBinaryFunctions;

/*!
 @property tileAdditionalBinaryFunctions
 @abstract The set of additional binary functions to be accessed from the tile function in an incrementally created pipeline state.
 */
@property (nullable, nonatomic, copy) NSArray<id<MTLFunction>> *tileAdditionalBinaryFunctions;
@end

/*!
 @protocol MTLRenderPipelineState
 @abstract MTLRenderPipelineState represents a compiled render pipeline
 
 @discussion MTLRenderPipelineState is a compiled render pipeline and can be set on a MTLRenderCommandEncoder.
 */
API_AVAILABLE(macos(10.11), ios(8.0))
@protocol MTLRenderPipelineState <NSObject>

@property (nullable, readonly) NSString *label;
@property (readonly) id <MTLDevice> device;

/*!
 @property maxTotalThreadsPerThreadgroup
 @abstract The maximum total number of threads that can be in a single tile shader threadgroup.
 */
@property (readonly) NSUInteger maxTotalThreadsPerThreadgroup API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5));

/*!
 @property threadgroupSizeMatchesTileSize
 @abstract Returns true when the pipeline state requires a tile shader threadgroup size equal to the tile size
 */
@property (readonly) BOOL threadgroupSizeMatchesTileSize API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5));



/*!
 @property imageblockSampleLength
 @brief Returns imageblock memory length used by a single sample when rendered using this pipeline.
 */
@property (readonly) NSUInteger imageblockSampleLength API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5));

/*!
 @method imageblockMemoryLengthForDimensions:sampleCount:
 @brief Returns imageblock memory length for given image block dimensions. Dimensions must be valid tile dimensions.
 */
- (NSUInteger)imageblockMemoryLengthForDimensions:(MTLSize)imageblockDimensions API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5));


@property (readonly) BOOL supportIndirectCommandBuffers API_AVAILABLE(macos(10.14), ios(12.0));


/*!
 @method functionHandleWithFunction:stage:
 @brief Gets the function handle for the specified function on the specified stage of the pipeline.
 */
- (nullable id<MTLFunctionHandle>)functionHandleWithFunction:(id<MTLFunction>)function stage:(MTLRenderStages)stage API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @method newVisibleFunctionTableWithDescriptor:stage:
 @brief Allocate a visible function table for the specified stage of the pipeline with the provided descriptor.
 */
- (nullable id<MTLVisibleFunctionTable>)newVisibleFunctionTableWithDescriptor:(MTLVisibleFunctionTableDescriptor * __nonnull)descriptor stage:(MTLRenderStages)stage API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @method newIntersectionFunctionTableWithDescriptor:stage:
 @brief Allocate an intersection function table for the specified stage of the pipeline with the provided descriptor.
 */
- (nullable id <MTLIntersectionFunctionTable>)newIntersectionFunctionTableWithDescriptor:(MTLIntersectionFunctionTableDescriptor * _Nonnull)descriptor stage:(MTLRenderStages)stage API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @method newRenderPipelineStateWithAdditionalBinaryFunctions:error:
 @brief Allocate a new render pipeline state by adding binary functions for each stage of this pipeline state.
 */
- (nullable id <MTLRenderPipelineState>)newRenderPipelineStateWithAdditionalBinaryFunctions:(nonnull MTLRenderPipelineFunctionsDescriptor *)additionalBinaryFunctions error:(__autoreleasing NSError **)error API_AVAILABLE(macos(12.0), ios(15.0));

@end

MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLRenderPipelineColorAttachmentDescriptorArray : NSObject

/* Individual attachment state access */
- (MTLRenderPipelineColorAttachmentDescriptor *)objectAtIndexedSubscript:(NSUInteger)attachmentIndex;

/* This always uses 'copy' semantics.  It is safe to set the attachment state at any legal index to nil, which resets that attachment descriptor state to default vaules. */
- (void)setObject:(nullable MTLRenderPipelineColorAttachmentDescriptor *)attachment atIndexedSubscript:(NSUInteger)attachmentIndex;

@end


MTL_EXPORT API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5))
@interface MTLTileRenderPipelineColorAttachmentDescriptor : NSObject <NSCopying>

/*! Pixel format.  Defaults to MTLPixelFormatInvalid */
@property (nonatomic) MTLPixelFormat pixelFormat;

@end

MTL_EXPORT API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5))
@interface MTLTileRenderPipelineColorAttachmentDescriptorArray : NSObject

/* Individual tile attachment state access */
- (MTLTileRenderPipelineColorAttachmentDescriptor*)objectAtIndexedSubscript:(NSUInteger)attachmentIndex;

/* This always uses 'copy' semantics.  It is safe to set the attachment state at any legal index to nil, which resets that attachment descriptor state to default vaules. */
- (void)setObject:(MTLTileRenderPipelineColorAttachmentDescriptor*)attachment atIndexedSubscript:(NSUInteger)attachmentIndex;

@end

MTL_EXPORT API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0), tvos(14.5))
@interface MTLTileRenderPipelineDescriptor : NSObject <NSCopying>

/*!
 @property label:
 @abstract The descriptor label.
 */
@property (copy, nonatomic, nullable) NSString *label;

/*!
 @property tileFunction:
 @abstract  The kernel or fragment function that serves as the tile shader for this pipeline.
 @discussion Both kernel-based and fragment-based tile pipelines dispatches will barrier against previous
 draws and other dispatches. Kernel-based pipelines will wait until all prior access to the tile completes.
 Fragment-based pipelines will only wait until all prior access to the fragment's location completes.
 */
@property (readwrite, nonatomic, strong) id <MTLFunction> tileFunction;

/* Rasterization and visibility state */
@property (readwrite, nonatomic) NSUInteger rasterSampleCount;
@property (readonly) MTLTileRenderPipelineColorAttachmentDescriptorArray *colorAttachments;

/*!
 @property threadgroupSizeMatchesTileSize:
 @abstract Whether all threadgroups associated with this pipeline will cover tiles entirely.
 @discussion Metal can optimize code generation for this case.
 */
@property (readwrite, nonatomic) BOOL threadgroupSizeMatchesTileSize;

@property (readonly) MTLPipelineBufferDescriptorArray *tileBuffers API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(11.0));

/*!
 @property maxTotalThreadsPerThreadgroup
 @abstract Optional property. Set the maxTotalThreadsPerThreadgroup. If it is not set, returns zero.
 */
@property (readwrite, nonatomic) NSUInteger maxTotalThreadsPerThreadgroup API_AVAILABLE(macos(10.15), ios(12.0));

/*!
 @property binaryArchives
 @abstract The set of MTLBinaryArchive to search for compiled code when creating the pipeline state.
 @discussion Accelerate pipeline state creation by providing archives of compiled code such that no compilation needs to happen on the fast path.
 @see MTLBinaryArchive
 */
@property (readwrite, nullable, nonatomic, copy) NSArray<id<MTLBinaryArchive>> *binaryArchives API_AVAILABLE(macos(11.0), ios(14.0));


/*!
 @property preloadedLibraries
 @abstract The set of MTLDynamicLibrary to use to resolve external symbols before considering symbols from dependent MTLDynamicLibrary.
 @discussion Typical workflows use the libraries property of MTLCompileOptions to record dependent libraries at compile time without having to use preloadedLibraries.
 This property can be used to override symbols from dependent libraries for experimentation or evaluating alternative implementations.
 It can also be used to provide dynamic libraries that are dynamically created (for example, from source) that have no stable installName that can be used to automatically load from the file system.
 @see MTLDynamicLibrary
 */
@property (readwrite, nonnull, nonatomic, copy) NSArray<id<MTLDynamicLibrary>>* preloadedLibraries API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property linkedFunctions
 @abstract The set of functions to be linked with the pipeline state and accessed from the tile function.
 @see MTLLinkedFunctions
 */
@property (null_resettable, copy, nonatomic) MTLLinkedFunctions *linkedFunctions
    API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property supportAddingBinaryFunctions
 @abstract This flag makes this pipeline support creating a new pipeline by adding binary functions.
 */
@property (readwrite, nonatomic) BOOL supportAddingBinaryFunctions
API_AVAILABLE(macos(12.0), ios(15.0));

/*!
 @property maxCallStackDepth
 @abstract The maximum depth of the call stack in stack frames from the tile function. Defaults to 1 additional stack frame.
 */
@property (readwrite, nonatomic) NSUInteger maxCallStackDepth
API_AVAILABLE(macos(12.0), ios(15.0));

- (void)reset;

@end


NS_ASSUME_NONNULL_END
