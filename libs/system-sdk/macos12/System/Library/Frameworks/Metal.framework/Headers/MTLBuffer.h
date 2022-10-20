//
//  MTLBuffer.h
//  Metal
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/MTLDefines.h>
#import <Metal/MTLPixelFormat.h>
#import <Metal/MTLResource.h>

NS_ASSUME_NONNULL_BEGIN
/*!
 @header MTLBuffer.h
 @discussion Header file for MTLBuffer
 */

@class MTLTextureDescriptor;
@protocol MTLTexture;
@protocol MTLResource;

/*!
 @protocol MTLBuffer
 @abstract A typeless allocation accessible by both the CPU and the GPU (MTLDevice) or by only the GPU when the storage mode is
 MTLResourceStorageModePrivate.
 
 @discussion
 Unlike in OpenGL and OpenCL, access to buffers is not synchronized.  The caller may use the CPU to modify the data at any time
 but is also responsible for ensuring synchronization and coherency.
 
 The contents become undefined if both the CPU and GPU write to the same buffer without a synchronizing action between those writes.
 This is true even when the regions written do not overlap.
 */
API_AVAILABLE(macos(10.11), ios(8.0))
@protocol MTLBuffer <MTLResource>

/*!
 @property length
 @abstract The length of the buffer in bytes.
 */
@property (readonly) NSUInteger length;

/*!
 @method contents
 @abstract Returns the data pointer of this buffer's shared copy.
 */
- (void*)contents NS_RETURNS_INNER_POINTER;

/*!
 @method didModifyRange:
 @abstract Inform the device of the range of a buffer that the CPU has modified, allowing the implementation to invalidate 
 its caches of the buffer's content.
 @discussion When the application writes to a buffer's sysmem copy via @a contents, that range of the buffer immediately 
 becomes undefined for any accesses by the GPU (MTLDevice).  To restore coherency, the buffer modification must be followed
 by -didModifyRange:, and then followed by a commit of the MTLCommandBuffer that will access the buffer.
 -didModifyRange does not make the contents coherent for any previously committed command buffers.
 Note: This method is only required if buffer is created with a storage mode of MTLResourceStorageModeManaged.
 It is not valid to invoke this method on buffers of other storage modes.
 @param range The range of bytes that have been modified.
 */
- (void)didModifyRange:(NSRange)range API_AVAILABLE(macos(10.11), macCatalyst(13.0)) API_UNAVAILABLE(ios);

/*!
 @method newTextureWithDescriptor:offset:bytesPerRow:
 @abstract Create a 2D texture or texture buffer that shares storage with this buffer.
 */
- (nullable id <MTLTexture>)newTextureWithDescriptor:(MTLTextureDescriptor*)descriptor offset:(NSUInteger)offset bytesPerRow:(NSUInteger)bytesPerRow API_AVAILABLE(macos(10.13), ios(8.0));

/*!
 @method addDebugMarker:range:
 @abstract Adds a marker to a specific range in the buffer.
 When inspecting a buffer in the GPU debugging tools the marker will be shown.
 @param marker A label used for the marker.
 @param range The range of bytes the marker is using.
 */
- (void)addDebugMarker:(NSString*)marker range:(NSRange)range API_AVAILABLE(macos(10.12), ios(10.0));

/*!
 @method removeAllDebugMarkers
 @abstract Removes all debug markers from a buffer.
 */
- (void)removeAllDebugMarkers API_AVAILABLE(macos(10.12), ios(10.0));

/*!
 @property remoteStorageBuffer
 @abstract For Metal buffer objects that are remote views, this returns the buffer associated with the storage on the originating device.
 */
@property (nullable, readonly) id<MTLBuffer> remoteStorageBuffer API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios);

/*!
 @method newRemoteBufferViewForDevice:
 @abstract On Metal devices that support peer to peer transfers, this method is used to create a remote buffer view on another device
 within the peer group.  The receiver must use MTLStorageModePrivate or be backed by an IOSurface.
 */
- (nullable id <MTLBuffer>) newRemoteBufferViewForDevice:(id <MTLDevice>)device API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios);

@end
NS_ASSUME_NONNULL_END
