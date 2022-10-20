//
//  MTLHeap.h
//  Metal
//
//  Copyright (c) 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/MTLDefines.h>
#import <Metal/MTLResource.h>
#import <Metal/MTLBuffer.h>
#import <Metal/MTLTexture.h>
#import <Metal/MTLTypes.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 @enum MTLHeapType
 @abstract Describes the mode of operation for an MTLHeap.
 @constant MTLHeapTypeAutomatic
 In this mode, resources are placed in the heap automatically.
 Automatically placed resources have optimal GPU-specific layout, and may perform better than MTLHeapTypePlacement.
 This heap type is recommended when the heap primarily contains temporary write-often resources.
 @constant MTLHeapTypePlacement
 In this mode, the app places resources in the heap.
 Manually placed resources allow the app to control memory usage and heap fragmentation directly.
 This heap type is recommended when the heap primarily contains persistent write-rarely resources.
 */
typedef NS_ENUM(NSInteger, MTLHeapType)
{
    MTLHeapTypeAutomatic = 0,
    MTLHeapTypePlacement = 1,
    MTLHeapTypeSparse API_AVAILABLE(macos(11.0), macCatalyst(14.0)) = 2 ,
} API_AVAILABLE(macos(10.15), ios(13.0));

/*!
 @class MTLHeapDescriptor
 */
MTL_EXPORT API_AVAILABLE(macos(10.13), ios(10.0))
@interface MTLHeapDescriptor : NSObject <NSCopying>

/*!
 @property size
 @abstract Requested size of the heap's backing memory.
 @discussion The size may be rounded up to GPU page granularity.
 */
@property (readwrite, nonatomic) NSUInteger size;

/*!
 @property storageMode
 @abstract Storage mode for the heap. Default is MTLStorageModePrivate.
 @discussion All resources created from this heap share the same storage mode.
 MTLStorageModeManaged and MTLStorageModeMemoryless are disallowed.
 */
@property (readwrite, nonatomic) MTLStorageMode storageMode;

/*!
 @property cpuCacheMode
 @abstract CPU cache mode for the heap. Default is MTLCPUCacheModeDefaultCache.
 @discussion All resources created from this heap share the same cache mode.
 CPU cache mode is ignored for MTLStorageModePrivate.
 */
@property (readwrite, nonatomic) MTLCPUCacheMode cpuCacheMode;

/*!
 @property hazardTrackingMode
 @abstract Set hazard tracking mode for the heap. The default value is MTLHazardTrackingModeDefault.
 @discussion For heaps, MTLHazardTrackingModeDefault is treated as MTLHazardTrackingModeUntracked.
 Setting hazardTrackingMode to MTLHazardTrackingModeTracked causes hazard tracking to be enabled heap.
 When a resource on a hazard tracked heap is modified, reads and writes from all resources suballocated on that heap will be delayed until the modification is complete.
 Similarly, modifying heap resources will be delayed until all in-flight reads and writes from all resources suballocated on that heap have completed.
 For optimal performance, perform hazard tracking manually through MTLFence or MTLEvent instead.
 All resources created from this heap shared the same hazard tracking mode.
 */
@property (readwrite, nonatomic) MTLHazardTrackingMode hazardTrackingMode API_AVAILABLE(macos(10.15), ios(13.0));

/*!
 @property resourceOptions
 @abstract A packed tuple of the storageMode, cpuCacheMode and hazardTrackingMode properties.
 @discussion Modifications to this property are reflected in the other properties and vice versa.
 */
@property (readwrite, nonatomic) MTLResourceOptions resourceOptions API_AVAILABLE(macos(10.15), ios(13.0));

/*!
 @property type
 @abstract The type of the heap. The default value is MTLHeapTypeAutomatic.
 @discussion This constrains the resource creation functions that are available.
 */
@property (readwrite, nonatomic) MTLHeapType type API_AVAILABLE(macos(10.15), ios(13.0));

@end


/*!
 @protocol MTLHeap
 */
API_AVAILABLE(macos(10.13), ios(10.0))
@protocol MTLHeap <NSObject>

/*!
 @property label
 @abstract A string to help identify this heap.
 */
@property (nullable, copy, atomic) NSString *label;

/*!
 @property device
 @abstract The device this heap was created against. This heap can only be used with this device.
 */
@property (readonly) id <MTLDevice> device;

/*!
 @property storageMode
 @abstract Current heap storage mode, default is MTLStorageModePrivate.
 @discussion All resources created from this heap share the same storage mode.
 */
@property (readonly) MTLStorageMode storageMode;

/*!
 @property cpuCacheMode
 @abstract CPU cache mode for the heap. Default is MTLCPUCacheModeDefaultCache.
 @discussion All resources created from this heap share the same cache mode.
 */
@property (readonly) MTLCPUCacheMode cpuCacheMode;

/*!
 @property hazardTrackingMode
 @abstract Whether or not the heap is hazard tracked.
 @discussion
 When a resource on a hazard tracked heap is modified, reads and writes from any other resource on that heap will be delayed until the modification is complete.
 Similarly, modifying heap resources will be delayed until all in-flight reads and writes from resources suballocated on that heap have completed.
 For optimal performance, perform hazard tracking manually through MTLFence or MTLEvent instead.
 Resources on the heap may opt-out of hazard tracking individually when the heap is hazard tracked,
 however resources cannot opt-in to hazard tracking when the heap is not hazard tracked.
 */
@property (readonly) MTLHazardTrackingMode hazardTrackingMode API_AVAILABLE(macos(10.15), ios(13.0));

/*!
 @property resourceOptions
 @abstract A packed tuple of the storageMode, cpuCacheMode and hazardTrackingMode properties.
 */
@property (readonly) MTLResourceOptions resourceOptions API_AVAILABLE(macos(10.15), ios(13.0));

/*!
 @property size
 @abstract Heap size in bytes, specified at creation time and rounded up to device specific alignment.
 */
@property (readonly) NSUInteger size;


/*!
 @property usedSize
 @abstract The size in bytes, of all resources allocated from the heap.
 */
@property (readonly) NSUInteger usedSize;

/*!
 @property currentAllocatedSize
 @abstract The size in bytes of the current heap allocation.
 */
@property (readonly) NSUInteger currentAllocatedSize API_AVAILABLE(macos(10.13), ios(11.0));

/*!
 @method maxAvailableSizeWithAlignment:
 @abstract The maximum size that can be successfully allocated from the heap in bytes, taking into notice given alignment. Alignment needs to be zero, or power of two.
 @discussion Provides a measure of fragmentation within the heap.
 */
- (NSUInteger)maxAvailableSizeWithAlignment:(NSUInteger)alignment;

/*!
 @method newBufferWithLength:options:
 @abstract Create a new buffer backed by heap memory.
 @discussion The requested storage and CPU cache modes must match the storage and CPU cache modes of the heap.
 @return The buffer or nil if heap is full.
 */
- (nullable id <MTLBuffer>)newBufferWithLength:(NSUInteger)length
                                       options:(MTLResourceOptions)options;

/*!
 @method newTextureWithDescriptor:
 @abstract Create a new texture backed by heap memory.
 @discussion The requested storage and CPU cache modes must match the storage and CPU cache modes of the heap, with the exception that the requested storage mode can be MTLStorageModeMemoryless. 
 @return The texture or nil if heap is full.
 */
- (nullable id <MTLTexture>)newTextureWithDescriptor:(MTLTextureDescriptor *)desc;

/*!
 @method setPurgeabilityState:
 @abstract Set or query the purgeability state of the heap.
 */
- (MTLPurgeableState)setPurgeableState:(MTLPurgeableState)state;

/*!
 @property type
 @abstract The type of the heap. The default value is MTLHeapTypeAutomatic.
 @discussion This constrains the resource creation functions that are available on the heap.
 */
@property (readonly) MTLHeapType type API_AVAILABLE(macos(10.15), ios(13.0));
 
/*!
 @method newBufferWithLength:options:offset:
 @abstract Create a new buffer backed by heap memory at the specified placement offset.
 @discussion This method can only be used when heapType is set to MTLHeapTypePlacement.
 Use "MTLDevice heapBufferSizeAndAlignWithLength:options:" to determine requiredSize and requiredAlignment.
 Any resources that exist in this heap at overlapping half-open range [offset, offset + requiredSize) are implicitly aliased with the new resource.
 @param length The requested size of the buffer, in bytes.
 @param options The requested options of the buffer, of which the storage and CPU cache mode must match these of the heap.
 @param offset The requested offset of the buffer inside the heap, in bytes. Behavior is undefined if "offset + requiredSize > heap.size" or "offset % requiredAlignment != 0".
 @return The buffer, or nil if the heap is not a placement heap
 */
- (nullable id<MTLBuffer>)newBufferWithLength:(NSUInteger)length
                                      options:(MTLResourceOptions)options
                                       offset:(NSUInteger)offset API_AVAILABLE(macos(10.15), ios(13.0));

/*!
 @method newTextureWithDescriptor:offset:
 @abstract Create a new texture backed by heap memory at the specified placement offset.
 @discussion This method can only be used when heapType is set to MTLHeapTypePlacement.
 Use "MTLDevice heapTextureSizeAndAlignWithDescriptor:" to determine requiredSize and requiredAlignment.
 Any resources that exist in this heap at overlapping half-open range [offset, offset + requiredSize) are implicitly aliased with the new resource.
 @param descriptor The requested properties of the texture, of which the storage and CPU cache mode must match those of the heap.
 @param offset The requested offset of the texture inside the heap, in bytes. Behavior is undefined if "offset + requiredSize > heap.size" and "offset % requiredAlignment != 0".
 @return The texture, or nil if the heap is not a placement heap.
 */
- (nullable id<MTLTexture>)newTextureWithDescriptor:(MTLTextureDescriptor *)descriptor
                                             offset:(NSUInteger)offset API_AVAILABLE(macos(10.15), ios(13.0));
@end

NS_ASSUME_NONNULL_END
