/*
    NSAnimationContext.h
    Application Kit
    Copyright (c) 2006-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class CAMediaTimingFunction;

API_AVAILABLE(macos(10.5))
@interface NSAnimationContext : NSObject

+ (void)runAnimationGroup:(void (NS_NOESCAPE ^)(NSAnimationContext * context))changes completionHandler:(nullable void (^)(void))completionHandler API_AVAILABLE(macos(10.7));

+ (void)runAnimationGroup:(void (NS_NOESCAPE ^)(NSAnimationContext * context))changes API_AVAILABLE(macos(10.12));

+ (void)beginGrouping;
+ (void)endGrouping;

@property (class, readonly, strong) NSAnimationContext *currentContext;

@property NSTimeInterval duration;
@property (nullable, strong) CAMediaTimingFunction *timingFunction API_AVAILABLE(macos(10.7));

@property (nullable, copy) void (^completionHandler)(void) API_AVAILABLE(macos(10.7));

/* Determine if animations are enabled or not. Using the -animator proxy will automatically set allowsImplicitAnimation to YES. When YES, other properties can implicitly animate along with the initially changed property. For instance, calling [[view animator] setFrame:frame] will allow subviews to also animate their frame positions. This is only applicable when layer backed on Mac OS 10.8 and later. The default value is NO.
 */
@property BOOL allowsImplicitAnimation API_AVAILABLE(macos(10.8));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
