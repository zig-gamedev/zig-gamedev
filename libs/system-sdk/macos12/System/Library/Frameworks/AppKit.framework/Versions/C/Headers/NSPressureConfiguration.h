/*
    NSPressureConfiguration.h
    Application Kit
    Copyright (c) 2013-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSObject.h>
#import <AppKit/NSEvent.h>
#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(10.10.3))
@interface NSPressureConfiguration : NSObject

@property(readonly) NSPressureBehavior pressureBehavior;

/* Designated initializer. An NSPressureBehavior must be inited with its behavior type and shall not change for the object's lifetime
 */
- (instancetype)initWithPressureBehavior:(NSPressureBehavior)pressureBehavior NS_DESIGNATED_INITIALIZER;

/* Sets the configuration; it will automatically be reset on mouse up or gesture end. Only takes effect during a mouse drag or pressure event sequence. However, if your application is slow to respond to the mouseDown event, the user may complete the mouse click sequence before this configuration can take effect. When possible you should configure the pressure behavior before the mouse down via NSView's pressureConfiguration property.
*/
- (void)set;
@end


@interface NSView (NSPressureConfiguration)

/* The pressure configuration a Force Click trackpad should use when the cursor is over this view. The system can set this configuration before the application receives the initial mouse down event. Does not affect an active pressure event sequence.
*/
@property(nullable, strong) NSPressureConfiguration *pressureConfiguration API_AVAILABLE(macos(10.11));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
