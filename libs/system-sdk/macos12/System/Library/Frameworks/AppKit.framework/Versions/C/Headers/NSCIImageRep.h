/*
        NSCIImageRep.h
        Application Kit
        Copyright (c) 2003-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSImageRep.h>
#import <AppKit/NSGraphics.h>
#import <AppKit/AppKitDefines.h>
#import <CoreImage/CIImage.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSBitmapImageRep;

@interface NSCIImageRep : NSImageRep
+ (instancetype)imageRepWithCIImage:(CIImage *)image;
- (instancetype)initWithCIImage:(CIImage *)image;
@property (readonly, strong) CIImage *CIImage;
@end

@interface CIImage (NSAppKitAdditions)
- (nullable instancetype)initWithBitmapImageRep:(NSBitmapImageRep *)bitmapImageRep;

- (void)drawInRect:(NSRect)rect fromRect:(NSRect)fromRect operation:(NSCompositingOperation)op fraction:(CGFloat)delta;
- (void)drawAtPoint:(NSPoint)point fromRect:(NSRect)fromRect operation:(NSCompositingOperation)op fraction:(CGFloat)delta;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

