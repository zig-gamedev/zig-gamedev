/*
        NSOpenGLView.h
        Application Kit
        Copyright (c) 2000-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSView.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSOpenGLContext, NSOpenGLPixelFormat;

NS_OPENGL_CLASS_DEPRECATED("Please use MTKView instead.", 10.0, 10.14)
@interface NSOpenGLView : NSView

+ (NSOpenGLPixelFormat*)defaultPixelFormat;

- (nullable instancetype)initWithFrame:(NSRect)frameRect pixelFormat:(nullable NSOpenGLPixelFormat*)format;

@property (nullable, strong) NSOpenGLContext *openGLContext;
- (void)clearGLContext;

- (void)update NS_REQUIRES_SUPER;		// moved or resized
- (void)reshape NS_REQUIRES_SUPER;	// scrolled, moved or resized

@property (nullable, strong) NSOpenGLPixelFormat *pixelFormat;

- (void)prepareOpenGL NS_REQUIRES_SUPER;

@property BOOL wantsBestResolutionOpenGLSurface NS_OPENGL_DEPRECATED(10.7, 10.14);
@property BOOL wantsExtendedDynamicRangeOpenGLSurface NS_OPENGL_DEPRECATED(10.11, 10.14);
@end

@interface NSView (NSOpenGLSurfaceResolution)

/* Specifies whether a given view instance wants, and is capable of correctly handling, an OpenGL backing surface (framebuffer) with resolution greater than 1 pixel per point.  This property is relevant only for views to which an NSOpenGLContext is bound (including, but not limited to, NSOpenGLViews); its value does not affect the behavior of other views, including CALayer-backed views (which may choose to render at a higher surface resolution independent of this property's value.  For compatibility, wantsBestResolutionOpenGLSurface defaults to NO, providing a 1 pixel per point framebuffer regardless of the backing scale factor for the display the view occupies.  (When the backing scale factor is > 1.0, the rendered surface contents are scaled up to the appropriate apparent size.)  Setting this property to YES for a given view gives AppKit permission to allocate a higher-resolution framebuffer when appropriate for the backing scale factor and target display.  AppKit may vary the surface resolution when the display mode is changed or the view is moved to a different display, but with this property set to YES it is capable of allocating a surface of greater than 1 pixel per point for the view.

To function correctly with wantsBestResolutionOpenGLSurface set to YES, a view must take care to perform correct conversions between view units and pixel units when needed.  For example: The common practice of passing the width and height of [self bounds] to glViewport() will yield incorrect results (partial instead of complete coverage of the render surface) at backing scale factors other than 1.0, since the parameters to glViewport() must be expressed in pixels. When wantsBestResolutionOpenGLSurface is set to YES, glViewPort will automatically be called before update and reshape. If you wish to call it yourself (for backwards compatibility, or to draw to a subrect of the surface) use the dimensions of [self convertRectToBacking:[self bounds]], which are in appropriate (pixel) units.

This property is archived (keyed archiving required).

*/
@property BOOL wantsBestResolutionOpenGLSurface API_DEPRECATED("Use NSOpenGLView instead.", macos(10.7,10.14));

@end

@interface NSView (NSExtendedDynamicRange)

/* When set to YES on a view with an attached OpenGL context, the NSScreen in which that views resides may have its maximumExtendedDynamicRangeColorComponentValue increased.  When composited by the Window Server, color values rendered by this OpenGL surface will be clamped to the NSScreen’s maximumExtendedDynamicRangeColorComponentValue rather than 1.0.
 */
@property BOOL wantsExtendedDynamicRangeOpenGLSurface API_DEPRECATED("Use NSOpenGLView instead.", macos(10.11,10.14));

@end


API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
