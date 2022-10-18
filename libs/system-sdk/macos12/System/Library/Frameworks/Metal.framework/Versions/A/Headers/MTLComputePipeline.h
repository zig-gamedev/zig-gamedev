//
//  MTLComputePipeline.h
//  Metal
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Metal/MTLDefines.h>
#import <Metal/MTLDevice.h>
#import <Metal/MTLArgument.h>
#import <Metal/MTLStageInputOutputDescriptor.h>
#import <Metal/MTLPipeline.h>

#import <Metal/MTLLinkedFunctions.h>
@protocol MTLFunctionHandle;

@protocol MTLDynamicLibrary;

NS_ASSUME_NONNULL_BEGIN

MTL_EXPORT API_AVAILABLE(macos(10.11), ios(8.0))
@interface MTLComputePipelineReflection : NSObject

@property (readonly) NSArray <MTLArgument *> *arguments;


@end

MTL_EXPORT API_AVAILABLE(macos(10.11), ios(9.0))
@interface MTLComputePipelineDescriptor : NSObject <NSCopying>

/*!
 @property label
 @abstract A string to help identify this object.
 */
@property (nullable, copy, nonatomic) NSString *label;

/*!
 @property computeFunction
 @abstract The function to use with the MTLComputePipelineState
 */
@property (nullable, readwrite, nonatomic, strong) id <MTLFunction> computeFunction;

/*!
 @property threadGroupSizeIsMultipleOfThreadExecutionWidth
 @abstract An optimization flag, set if the thread group size will always be a multiple of thread execution width
 */
@property (readwrite, nonatomic) BOOL threadGroupSizeIsMultipleOfThreadExecutionWidth;

/*!
 @property maxTotalThreadsPerThreadgroup
 @abstract Optional property. Set the maxTotalThreadsPerThreadgroup. If it is not set, returns zero.
 */
@property (readwrite, nonatomic) NSUInteger maxTotalThreadsPerThreadgroup API_AVAILABLE(macos(10.14), ios(12.0));


/*!
 @property computeDataDescriptor
 @abstract An MTLStageInputOutputDescriptor to fetch data from buffers
 */
@property (nullable, copy, nonatomic) MTLStageInputOutputDescriptor *stageInputDescriptor API_AVAILABLE(macos(10.12), ios(10.0));

/*!
 @property buffers
 @abstract Optional properties for each buffer binding used by the compute function.
 */
@property (readonly) MTLPipelineBufferDescriptorArray *buffers API_AVAILABLE(macos(10.13), ios(11.0));

/*!
 @property supportIndirectCommandBuffers
 @abstract This flag makes this pipeline usable with indirect command buffers.
 */
@property (readwrite, nonatomic) BOOL supportIndirectCommandBuffers API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(13.0));

/*!
 @property insertLibraries
 @abstract The set of MTLDynamicLibrary to use to resolve external symbols before considering symbols from dependent MTLDynamicLibrary.
 @discussion Typical workflows use the libraries property of MTLCompileOptions to record dependent libraries at compile time without having to use insertLibraries.
 This property can be used to override symbols from dependent libraries for experimentation or evaluating alternative implementations.
 It can also be used to provide dynamic libraries that are dynamically created (for example, from source) that have no stable installName that can be used to automatically load from the file system.
 @see MTLDynamicLibrary
 */
@property (readwrite, nullable, nonatomic, copy) NSArray<id<MTLDynamicLibrary>>* insertLibraries API_DEPRECATED_WITH_REPLACEMENT("Use preloadedLibraries instead.", macos(11.0, 12.0), ios(14.0, 15.0));

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
 @property binaryArchives
 @abstract The set of MTLBinaryArchive to search for compiled code when creating the pipeline state.
 @discussion Accelerate pipeline state creation by providing archives of compiled code such that no compilation needs to happen on the fast path.
 @see MTLBinaryArchive
 */
@property (readwrite, nullable, nonatomic, copy) NSArray<id<MTLBinaryArchive>> *binaryArchives API_AVAILABLE(macos(11.0), ios(14.0));


/*!
 @method reset
 @abstract Restore all compute pipeline descriptor properties to their default values.
 */
- (void)reset;


/*!
 @property linkedFunctions
 @abstract The set of functions to be linked with the pipeline state and accessed from the compute function. 
 @see MTLLinkedFunctions
 */
@property (nullable, copy, nonatomic) MTLLinkedFunctions *linkedFunctions
    API_AVAILABLE(macos(11.0), ios(14.0));



/*!
 @property supportAddingBinaryFunctions
 @abstract This flag makes this pipeline support creating a new pipeline by adding binary functions.
 */
@property (readwrite, nonatomic) BOOL supportAddingBinaryFunctions
    API_AVAILABLE(macos(11.0), ios(14.0));


/*!
 @property maxCallStackDepth
 @abstract The maximum depth of the call stack in stack frames from the kernel. Defaults to 1 additional stack frame.
 */
@property (readwrite, nonatomic) NSUInteger maxCallStackDepth
    API_AVAILABLE(macos(11.0), ios(14.0));



@end

/*!
 @protocol MTLComputePipelineState
 @abstract A handle to compiled code for a compute function.
 @discussion MTLComputePipelineState is a single compute function.  It can only be used with the device that it was created against.
*/
API_AVAILABLE(macos(10.11), ios(8.0))
@protocol MTLComputePipelineState <NSObject>

@property (nullable, readonly) NSString *label API_AVAILABLE(macos(10.13), ios(11.0));

/*!
 @property device
 @abstract The device this resource was created against.  This resource can only be used with this device.
 */
@property (readonly) id <MTLDevice> device;

/*!
 @property maxTotalThreadsPerThreadgroup
 @abstract The maximum total number of threads that can be in a single compute threadgroup.
 */
@property (readonly) NSUInteger maxTotalThreadsPerThreadgroup;

/*!
 @property threadExecutionWidth
 @abstract For most efficient execution, the threadgroup size should be a multiple of this when executing the kernel.
 */
@property (readonly) NSUInteger threadExecutionWidth;


/*!
 @property staticThreadgroupMemoryLength
 @abstract The length in bytes of threadgroup memory that is statically allocated.
 */
@property (readonly) NSUInteger staticThreadgroupMemoryLength API_AVAILABLE(macos(10.13), ios(11.0));


/*!
 @method imageblockMemoryLengthForDimensions:
 @brief Returns imageblock memory length for given image block dimensions.
 */
- (NSUInteger)imageblockMemoryLengthForDimensions:(MTLSize)imageblockDimensions API_AVAILABLE(ios(11.0), macos(11.0), macCatalyst(14.0), tvos(14.5));


/*!
 @property supportIndirectCommandBuffers
 @abstract Tells whether this pipeline state is usable through an Indirect Command Buffer.
 */
@property (readonly) BOOL supportIndirectCommandBuffers API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(13.0));


/*!
 @method functionHandleWithFunction:
 @brief Get the function handle for the specified function from the pipeline state.
 */
- (nullable id<MTLFunctionHandle>)functionHandleWithFunction:(id<MTLFunction>)function API_AVAILABLE(macos(11.0), ios(14.0));


/*!
 @method newRenderPipelineStateWithAdditionalBinaryFunctions:stage:
 @brief Allocate a new compute pipeline state by adding binary functions to this pipeline state.
 */
- (nullable id <MTLComputePipelineState>)newComputePipelineStateWithAdditionalBinaryFunctions:(nonnull NSArray<id<MTLFunction>> *)functions error:(__autoreleasing NSError **)error API_AVAILABLE(macos(11.0), ios(14.0));

/*!
 @method newVisibleFunctionTableWithDescriptor:
 @brief Allocate a visible function table for the pipeline with the provided descriptor.
 */
- (nullable id<MTLVisibleFunctionTable>)newVisibleFunctionTableWithDescriptor:(MTLVisibleFunctionTableDescriptor * __nonnull)descriptor API_AVAILABLE(macos(11.0), ios(14.0));

/*!
 @method newIntersectionFunctionTableWithDescriptor:
 @brief Allocate an intersection function table for the pipeline with the provided descriptor.
 */
- (nullable id <MTLIntersectionFunctionTable>)newIntersectionFunctionTableWithDescriptor:(MTLIntersectionFunctionTableDescriptor * _Nonnull)descriptor API_AVAILABLE(macos(11.0), ios(14.0));


@end

NS_ASSUME_NONNULL_END
