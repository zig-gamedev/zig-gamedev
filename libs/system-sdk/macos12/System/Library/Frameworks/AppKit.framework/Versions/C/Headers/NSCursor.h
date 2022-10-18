/*
	NSCursor.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/NSApplication.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN

@class NSColor, NSEvent, NSImage;
#if TARGET_OS_IPHONE
@class UIImage;
#endif

API_AVAILABLE(macos(10.0), ios(13.0))
@interface NSCursor : NSObject <NSCoding>

/* Returns the application's current cursor. This is not necessarily the cursor that is currently being displayed on the system. If you need the currently displayed cursor, use currentSystemCursor.
*/
@property (class, readonly, strong) NSCursor *currentCursor;

/* Returns an NSCursor whose image and hot spot match those of the cursor that is currently being displayed on the system, regardless of which application set that cursor. For example, this allows an inactive application to examine the image and hot spot of a cursor set by the active application. This API doesn't care whether the cursor is hidden; it will return an NSCursor whose image matches the cursor you would see if it were visible.
 
 Only the cursor image and hot spot are replicated in the returned NSCursor. Other properties (such as isSetOnMouseExited and isSetOnMouseEntered) will not necessarily be accurate.
 
 This method returns nil if the currently displayed cursor could not be fetched.
 */
@property (class, readonly, nullable, strong) NSCursor *currentSystemCursor API_AVAILABLE(macos(10.6));

@property (class, readonly, strong) NSCursor *arrowCursor;
@property (class, readonly, strong) NSCursor *IBeamCursor;
@property (class, readonly, strong) NSCursor *pointingHandCursor;
@property (class, readonly, strong) NSCursor *closedHandCursor;
@property (class, readonly, strong) NSCursor *openHandCursor;
@property (class, readonly, strong) NSCursor *resizeLeftCursor;
@property (class, readonly, strong) NSCursor *resizeRightCursor;
@property (class, readonly, strong) NSCursor *resizeLeftRightCursor;
@property (class, readonly, strong) NSCursor *resizeUpCursor;
@property (class, readonly, strong) NSCursor *resizeDownCursor;
@property (class, readonly, strong) NSCursor *resizeUpDownCursor;
@property (class, readonly, strong) NSCursor *crosshairCursor;
@property (class, readonly, strong) NSCursor *disappearingItemCursor;
@property (class, readonly, strong) NSCursor *operationNotAllowedCursor API_AVAILABLE(macos(10.5));
@property (class, readonly, strong) NSCursor *dragLinkCursor API_AVAILABLE(macos(10.6));
@property (class, readonly, strong) NSCursor *dragCopyCursor API_AVAILABLE(macos(10.6));
@property (class, readonly, strong) NSCursor *contextualMenuCursor API_AVAILABLE(macos(10.6));
@property (class, readonly, strong) NSCursor *IBeamCursorForVerticalLayout API_AVAILABLE(macos(10.7));

#if !TARGET_OS_IPHONE
- (instancetype)initWithImage:(NSImage *)newImage hotSpot:(NSPoint)point NS_DESIGNATED_INITIALIZER;
#else
- (instancetype)initWithImage:(UIImage *)newImage hotSpot:(NSPoint)point NS_DESIGNATED_INITIALIZER;
#endif
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

+ (void)hide;
+ (void)unhide;
+ (void)setHiddenUntilMouseMoves:(BOOL)flag;
+ (void)pop;

#if !TARGET_OS_IPHONE
@property (readonly, strong) NSImage *image;
#else
@property (readonly, strong) UIImage *image;
#endif
@property (readonly) NSPoint hotSpot;

- (void)push;
- (void)pop;
- (void)set;

@end

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST
static const NSAppKitVersion NSAppKitVersionNumberWithCursorSizeSupport = 682.0;
@interface NSCursor (NSDeprecated)
- (instancetype)initWithImage:(NSImage *)newImage foregroundColorHint:(nullable NSColor *)fg backgroundColorHint:(nullable NSColor *)bg hotSpot:(NSPoint)hotSpot API_DEPRECATED("Color hints are ignored. Use -initWithImage:hotSpot: instead", macos(10.0,10.12));
- (void)setOnMouseExited:(BOOL)flag API_DEPRECATED("setOnMouseExited is unused and should not be called", macos(10.0,10.13));
- (void)setOnMouseEntered:(BOOL)flag API_DEPRECATED("setOnMouseEntered is unused and should not be called", macos(10.0,10.13));
@property (getter=isSetOnMouseExited, readonly) BOOL setOnMouseExited API_DEPRECATED("isSetOnMouseExited is unused", macos(10.0,10.13));
@property (getter=isSetOnMouseEntered, readonly) BOOL setOnMouseEntered API_DEPRECATED("isSetOnMouseEntered is unused", macos(10.0,10.13));
- (void)mouseEntered:(NSEvent *)event API_DEPRECATED("mouseEntered: is unused and should not be called", macos(10.0,10.13));
- (void)mouseExited:(NSEvent *)event API_DEPRECATED("mouseExited: is unused and should not be called", macos(10.0,10.13));
@end
API_UNAVAILABLE_END

NS_ASSUME_NONNULL_END
