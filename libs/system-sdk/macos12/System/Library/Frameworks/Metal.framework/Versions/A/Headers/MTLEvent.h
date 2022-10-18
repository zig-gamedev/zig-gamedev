//
//  MTLEvent.h
//  Metal
//
//  Copyright © 2018 Apple, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/MTLDefines.h>
#import <Metal/MTLDevice.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.14), ios(12.0))
@protocol MTLEvent <NSObject>

/*!
 @property device
 @abstract The device this event can be used with. Will be nil when the event is shared across devices (i.e. MTLSharedEvent).
 */
@property (nullable, readonly) id <MTLDevice> device;

/*!
 @property label
 @abstract A string to help identify this object.
 */
@property (nullable, copy, atomic) NSString *label;

@end

/*!
 @class MTLSharedEventListener
 @abstract This class provides a simple interface for handling the dispatching of MTLSharedEvent notifications from Metal.
*/
MTL_EXPORT API_AVAILABLE(macos(10.14), ios(12.0))
@interface MTLSharedEventListener : NSObject
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithDispatchQueue:(dispatch_queue_t)dispatchQueue NS_DESIGNATED_INITIALIZER;
@property (nonnull, readonly) dispatch_queue_t dispatchQueue;
@end

@class MTLSharedEventHandle;
@protocol MTLSharedEvent;

typedef void (^MTLSharedEventNotificationBlock)(id <MTLSharedEvent>, uint64_t value);

API_AVAILABLE(macos(10.14), ios(12.0))
@protocol MTLSharedEvent <MTLEvent>

// When the event's signaled value reaches value or higher, invoke the block on the dispatch queue owned by the listener.
- (void)notifyListener:(MTLSharedEventListener *)listener atValue:(uint64_t)value block:(MTLSharedEventNotificationBlock)block;

// Convenience method for creating a shared event handle that may be passed to other processes via XPC.
- (MTLSharedEventHandle *)newSharedEventHandle;

@property (readwrite) uint64_t signaledValue; // Read or set signaled value

@end


// MTLSharedEventHandle objects may be passed between processes via XPC connections and then used to recreate
// a MTLSharedEvent via an existing MTLDevice.
MTL_EXPORT API_AVAILABLE(macos(10.14), ios(12.0))
@interface MTLSharedEventHandle : NSObject <NSSecureCoding>
{
    struct MTLSharedEventHandlePrivate *_priv;
}
@property (readonly, nullable) NSString *label;
@end

NS_ASSUME_NONNULL_END
