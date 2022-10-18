//
//  MTLCommandQueue.h
//  Metal
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Metal/MTLDefines.h>
#import <Metal/Metal.h>

NS_ASSUME_NONNULL_BEGIN
@protocol MTLDevice;
@protocol MTLCommandBuffer;

/*!
 @protocol MTLCommandQueue
 @brief A serial queue of command buffers to be executed by the device.
 */
API_AVAILABLE(macos(10.11), ios(8.0))
@protocol MTLCommandQueue <NSObject>

/*! @brief A string to help identify this object */
@property (nullable, copy, atomic) NSString *label;

/*! @brief The device this queue will submit to */
@property (readonly) id <MTLDevice> device;

/*! 
 @method commandBuffer
 @abstract Returns a new autoreleased command buffer used to encode work into this queue that 
 maintains strong references to resources used within the command buffer.
*/
- (nullable id <MTLCommandBuffer>)commandBuffer;

/*!
 @method commandBufferWithDescriptor
 @param descriptor The requested properties of the command buffer.
 @abstract Returns a new autoreleased command buffer used to encode work into this queue.
*/
- (nullable id <MTLCommandBuffer>)commandBufferWithDescriptor:(MTLCommandBufferDescriptor*)descriptor API_AVAILABLE(macos(11.0), ios(14.0));


/*!
 @method commandBufferWithUnretainedReferences
 @abstract Returns a new autoreleased command buffer used to encode work into this queue that 
 does not maintain strong references to resources used within the command buffer.
*/
- (nullable id <MTLCommandBuffer>)commandBufferWithUnretainedReferences;

/*!
 @method insertDebugCaptureBoundary
 @abstract Inform Xcode about when debug capture should start and stop.
 */
- (void)insertDebugCaptureBoundary API_DEPRECATED("Use MTLCaptureScope instead", macos(10.11, 10.13), ios(8.0, 11.0));

@end

NS_ASSUME_NONNULL_END
