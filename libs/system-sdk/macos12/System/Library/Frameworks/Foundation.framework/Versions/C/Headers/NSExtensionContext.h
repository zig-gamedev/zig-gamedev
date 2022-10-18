/*	NSExtensionContext.h
        Copyright (c) 2013-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/Foundation.h>

#if __OBJC2__

// Class representing the extension request's context
NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
@interface NSExtensionContext : NSObject

// The list of input NSExtensionItems associated with the context. If the context has no input items, this array will be empty.
@property(readonly, copy, NS_NONATOMIC_IOSONLY) NSArray *inputItems;

// Signals the host to complete the app extension request with the supplied result items. The completion handler optionally contains any work which the extension may need to perform after the request has been completed, as a background-priority task. The `expired` parameter will be YES if the system decides to prematurely terminate a previous non-expiration invocation of the completionHandler. Note: calling this method will eventually dismiss the associated view controller.
- (void)completeRequestReturningItems:(nullable NSArray *)items completionHandler:(void(^ _Nullable)(BOOL expired))completionHandler NS_SWIFT_DISABLE_ASYNC;

// Signals the host to cancel the app extension request, with the supplied error, which should be non-nil. The userInfo of the NSError will contain a key NSExtensionItemsAndErrorsKey which will have as its value a dictionary of NSExtensionItems and associated NSError instances.
- (void)cancelRequestWithError:(NSError *)error;

// Asks the host to open a URL on the extension's behalf
- (void)openURL:(NSURL *)URL completionHandler:(void (^ _Nullable)(BOOL success))completionHandler;

@end

// Key in userInfo. Value is a dictionary of NSExtensionItems and associated NSError instances.
FOUNDATION_EXTERN NSString * __null_unspecified const NSExtensionItemsAndErrorsKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

// The host process will enter the foreground
FOUNDATION_EXTERN NSString * __null_unspecified const NSExtensionHostWillEnterForegroundNotification API_AVAILABLE(ios(8.2), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

// The host process did enter the background
FOUNDATION_EXTERN NSString * __null_unspecified const NSExtensionHostDidEnterBackgroundNotification API_AVAILABLE(ios(8.2), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

// The host process will resign active status (stop receiving events), the extension may be suspended
FOUNDATION_EXTERN NSString * __null_unspecified const NSExtensionHostWillResignActiveNotification API_AVAILABLE(ios(8.2), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

// The host process did become active (begin receiving events)
FOUNDATION_EXTERN NSString * __null_unspecified const NSExtensionHostDidBecomeActiveNotification API_AVAILABLE(ios(8.2), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

NS_ASSUME_NONNULL_END

#endif
