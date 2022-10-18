/*
        NSOpenGLLayer.h
        Application Kit
        Copyright (c) 2008-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSView.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/AppKitDefines.h>
#import <QuartzCore/CAOpenGLLayer.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

NS_OPENGL_CLASS_DEPRECATED("Please use CAMetalLayer instead.", 10.6, 10.14)
@interface NSOpenGLLayer : CAOpenGLLayer

/* Provides access to the layer's associated view.  Subclasses shouldn't invoke -setView:, but can override it if desired to intercept the layer's association to, or dissociation from, a view.
*/
@property (nullable, weak) NSView *view;

/* Provides access to the layer's associated NSOpenGLPixelFormat.  Subclasses shouldn't invoke -setOpenGLPixelFormat:, but can override it if desired to intercept assignment of the layer's pixel format.
*/
@property (nullable, strong) NSOpenGLPixelFormat *openGLPixelFormat;

/* Provides access to the layer's associated NSOpenGLContext.  Subclasses shouldn't invoke -setOpenGLContext:, but can override it if desired to intercept assignment of the layer's context.
*/
@property (nullable, strong) NSOpenGLContext *openGLContext;

/* Invoked by AppKit to ask for the pixel format to use.  Should return an autoreleased NSOpenGLPixelFormat suitable for the displays specified by "mask".  (In practice, this means including an NSOpenGLPFAScreenMask specification in the pixel format attribute list that's used to instantiate the NSOpenGLPixelFormat.)
*/
- (NSOpenGLPixelFormat *)openGLPixelFormatForDisplayMask:(uint32_t)mask;

/* Invoked by AppKit to ask for the OpenGL context to use.  Should return an autoreleased NSOpenGLContext.
*/
- (NSOpenGLContext *)openGLContextForPixelFormat:(NSOpenGLPixelFormat *)pixelFormat;

/* Invoked by AppKit to ask the layer whether it can draw.  Normally one would return YES, but one can return NO to cause the current frame to be skipped.
*/
- (BOOL)canDrawInOpenGLContext:(NSOpenGLContext *)context pixelFormat:(NSOpenGLPixelFormat *)pixelFormat forLayerTime:(CFTimeInterval)t displayTime:(const CVTimeStamp *)ts;

/* Invoked by AppKit to ask the layer to draw.
*/
- (void)drawInOpenGLContext:(NSOpenGLContext *)context pixelFormat:(NSOpenGLPixelFormat *)pixelFormat forLayerTime:(CFTimeInterval)t displayTime:(const CVTimeStamp *)ts;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
