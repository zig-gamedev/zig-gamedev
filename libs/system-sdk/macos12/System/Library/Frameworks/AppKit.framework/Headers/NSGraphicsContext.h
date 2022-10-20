/*
        NSGraphicsContext.h
        Application Kit
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <AppKit/NSGraphics.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSDictionary.h>
#import <CoreGraphics/CGContext.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString;
@class NSWindow;
@class NSBitmapImageRep;

/* Attributes that can be passed to graphicsContextWithAttributes: method */
typedef NSString * NSGraphicsContextAttributeKey NS_TYPED_ENUM;
APPKIT_EXTERN NSGraphicsContextAttributeKey NSGraphicsContextDestinationAttributeName; // Can be an instance of NSWindow, NSMutableData, NSBitmapImageRep, or NSURL.
APPKIT_EXTERN NSGraphicsContextAttributeKey NSGraphicsContextRepresentationFormatAttributeName; // Specifies destination file format. Used only for NSMutableData or NSURL destinations

// Supported file format
typedef NSString * NSGraphicsContextRepresentationFormatName NS_TYPED_ENUM;
APPKIT_EXTERN NSGraphicsContextRepresentationFormatName NSGraphicsContextPSFormat;
APPKIT_EXTERN NSGraphicsContextRepresentationFormatName NSGraphicsContextPDFFormat;

typedef NS_ENUM(NSUInteger, NSImageInterpolation) {
   NSImageInterpolationDefault = 0,
   NSImageInterpolationNone = 1,
   NSImageInterpolationLow = 2, /* Low quality, fast interpolation. */
   NSImageInterpolationMedium API_AVAILABLE(macos(10.6)) = 4, /* Medium quality, slower than NSImageInterpolationLow. */
   NSImageInterpolationHigh = 3 /* Highest quality, slower than NSImageInterpolationMedium. */
};

@interface NSGraphicsContext : NSObject

// Instantiates from an appropriate concrete subclass depending on NSGraphicsContextDestinationAttributeName attribute
+ (nullable NSGraphicsContext *)graphicsContextWithAttributes:(NSDictionary<NSGraphicsContextAttributeKey, id> *)attributes;

// Convenience cover method for [NSGraphicsContext graphicsContextWithAttributes:[NSDictionary dictionaryWithObject:<NSWindow instance> forKey:NSGraphicsContextDestinationAttributeName]]
+ (NSGraphicsContext *)graphicsContextWithWindow:(NSWindow *)window;

/* Convenience cover method for [NSGraphicsContext graphicsContextWithAttributes:[NSDictionary dictionaryWithObject:<NSBitmapImageRep instance> forKey:NSGraphicsContextDestinationAttributeName]]. Only NSBitmapImageRep instances with isPlanar == NO are supported.
*/
+ (nullable NSGraphicsContext *)graphicsContextWithBitmapImageRep:(NSBitmapImageRep *)bitmapRep;

/* Instantiates from a CGContextRef.  initialFlippedState is used to determine the return value from -isFlipped when no view is focused in the context.
*/
+ (NSGraphicsContext *)graphicsContextWithCGContext:(CGContextRef)graphicsPort flipped:(BOOL)initialFlippedState API_AVAILABLE(macos(10.10));

// Setting and identifying the current context in the thread
@property (class, nullable, strong) NSGraphicsContext *currentContext;

// Convenience cover method for [[NSGraphicsContext currentContext] isDrawingToScreen]
+ (BOOL)currentContextDrawingToScreen;

// Calls -saveGraphicsState on the current context, and pushes it on the per-thread stack.
+ (void)saveGraphicsState;

// Pops a context from the per-thread stack, makes it current, and calls -restoreGraphicsState on it.
+ (void)restoreGraphicsState;

// Returns attributes used to create this instance
@property (nullable, readonly, copy) NSDictionary<NSGraphicsContextAttributeKey, id> *attributes;

// Testing the drawing destination
@property (getter=isDrawingToScreen, readonly) BOOL drawingToScreen;

// Controlling the context
- (void)saveGraphicsState;
- (void)restoreGraphicsState;

- (void)flushGraphics;

@property (readonly) CGContextRef CGContext NS_RETURNS_INNER_POINTER API_AVAILABLE(macos(10.10));

/* Returns the flip state of the receiver.  The state is determined by messaging -isFlipped to the focus view in the context.  If no view has focus, returns NO unless the receiver is instantiated via graphicsContextWithGraphicsPort:drawingToScreen:flipped: with initialFlippedState == YES.
*/
@property (getter=isFlipped, readonly) BOOL flipped;
@end

@interface NSGraphicsContext(NSGraphicsContext_RenderingOptions)

@property BOOL shouldAntialias;
@property NSImageInterpolation imageInterpolation;

@property NSPoint patternPhase;
@property NSCompositingOperation compositingOperation;
@property NSColorRenderingIntent colorRenderingIntent API_AVAILABLE(macos(10.5));
@end

@class CIContext;
@interface NSGraphicsContext (NSQuartzCoreAdditions)
@property (nullable, readonly, strong) CIContext *CIContext;
@end

@interface NSGraphicsContext (NSGraphicsContextDeprecated)

+ (void)setGraphicsState:(NSInteger)gState API_DEPRECATED("This method has no effect", macos(10.0,10.10));

- (null_unspecified id)focusStack API_DEPRECATED("", macos(10.0,10.6));
- (void)setFocusStack:(null_unspecified id)stack API_DEPRECATED("", macos(10.0,10.6));

+ (NSGraphicsContext *)graphicsContextWithGraphicsPort:(void *)graphicsPort flipped:(BOOL)initialFlippedState API_DEPRECATED_WITH_REPLACEMENT("graphicsContextWithCGContext:flipped:", macos(10.0,10.14));
@property (readonly) void *graphicsPort NS_RETURNS_INNER_POINTER API_DEPRECATED_WITH_REPLACEMENT("CGContext", macos(10.0,10.14));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
