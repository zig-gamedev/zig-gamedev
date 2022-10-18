/* CoreAnimation - CAMetalLayer.h

   Copyright (c) 2013-2021, Apple Inc.
   All rights reserved. */

#import <QuartzCore/CALayer.h>
#import <QuartzCore/CAEDRMetadata.h>
#import <Metal/MTLPixelFormat.h>
#import <Metal/MTLDrawable.h>

@protocol MTLDevice;
@protocol MTLTexture;
@protocol MTLDrawable;

@class CAMetalLayer;

NS_ASSUME_NONNULL_BEGIN

/* CAMetalDrawable represents a displayable buffer that vends an object
 * that conforms to the MTLTexture protocol that may be used to create
 * a render target for Metal.
 *
 * Note: CAMetalLayer maintains an internal pool of textures used for
 * display. In order for a texture to be re-used for a new CAMetalDrawable,
 * any prior CAMetalDrawable must have been deallocated and another
 * CAMetalDrawable presented. */

@protocol CAMetalDrawable <MTLDrawable>

/* This is an object that conforms to the MTLTexture protocol and will
 * typically be used to create an MTLRenderTargetDescriptor. */

@property(readonly) id<MTLTexture> texture;

/* This is the CAMetalLayer responsible for displaying the drawable */

@property(readonly) CAMetalLayer *layer;

@end

/* Note: The default value of the `opaque' property for CAMetalLayer
 * instances is true. */

API_AVAILABLE(macos(10.11), ios(8.0), watchos(2.0), tvos(9.0))
@interface CAMetalLayer : CALayer
{
@private
  struct _CAMetalLayerPrivate *_priv;
}

/* This property determines which MTLDevice the MTLTexture objects for
 * the drawables will be created from.
 * On iOS this defaults to MTLCreateSystemDefaultDevice().
 * On macOS this defaults to nil and must be set explicitly before asking for
 * the first drawable. */

@property(nullable, retain) id<MTLDevice> device;

/* This property returns the preferred MTLDevice for this CAMetalLayer. */

@property(nullable, readonly) id<MTLDevice> preferredDevice
  API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* This property controls the pixel format of the MTLTexture objects.
 * The two supported values are MTLPixelFormatBGRA8Unorm and
 * MTLPixelFormatBGRA8Unorm_sRGB. */

@property MTLPixelFormat pixelFormat;

/* This property controls whether or not the returned drawables'
 * MTLTextures may only be used for framebuffer attachments (YES) or
 * whether they may also be used for texture sampling and pixel
 * read/write operations (NO). A value of YES allows CAMetalLayer to
 * allocate the MTLTexture objects in ways that are optimized for display
 * purposes that makes them unsuitable for sampling. The recommended
 * value for most applications is YES. */

@property BOOL framebufferOnly;

/* This property controls the pixel dimensions of the returned drawable
 * objects. The most typical value will be the layer size multiplied by
 * the layer contentsScale property. */

@property CGSize drawableSize;

/* Get the swap queue's next available drawable. Always blocks until a drawable
 * is available. Can return nil under the following conditions:
 *     1) The layer has an invalid combination of drawable properties.
 *     2) All drawables in the swap queue are in-use and the 1 second timeout
 *        has elapsed. (except when `allowsNextDrawableTimeout' is set to NO)
 *     3) Process is out of memory. */

- (nullable id<CAMetalDrawable>)nextDrawable;

/* Controls the number maximum number of drawables in the swap queue. The
 * default value is 3. Values set outside of range [2, 3] are ignored and an
 * exception will be thrown. */

@property NSUInteger maximumDrawableCount
  API_AVAILABLE(macos(10.13.2), ios(11.2), watchos(4.2), tvos(11.2));

/* When false (the default value) changes to the layer's render buffer
 * appear on-screen asynchronously to normal layer updates. When true,
 * changes to the MTL content are sent to the screen via the standard
 * CATransaction mechanisms. */

@property BOOL presentsWithTransaction;

/* The colorspace of the rendered frames. If nil, no colormatching occurs.
 * If non-nil, the rendered content will be colormatched to the colorspace of
 * the context containing this layer (typically the display's colorspace). */

@property (nullable) CGColorSpaceRef colorspace;

/* If any rendering context on the screen has this enabled, all content will be
 * clamped to its NSScreen’s maximumExtendedDynamicRangeColorComponentValue
 * rather than 1.0. The default is NO.  */

@property BOOL wantsExtendedDynamicRangeContent;

/* Metadata describing extended dynamic range content in the layer's drawable.
 * Must be set before calling nextDrawable. If non-nil, content may be
 * tone mapped to match the current display characteristics. If nil, samples
 * will be rendered without tone mapping and values above the maximum EDR value
 * -[NSScreen maximumExtendedDynamicRangeColorComponentValue] may be clamped.
 * Defaults to nil. */
@property (strong, nullable) CAEDRMetadata *EDRMetadata API_AVAILABLE(macos(10.15));

/* This property controls if this layer and its drawables will be synchronized
 * to the display's Vsync. The default value is YES. */

@property BOOL displaySyncEnabled API_AVAILABLE(macos(10.13));

/* Controls if `-nextDrawable' is allowed to timeout after 1 second and return
 * nil if * the system does not have a free drawable available. The default
 * value is YES. If set to NO, then `-nextDrawable' will block forever until a
 * free drawable is available. */

@property BOOL allowsNextDrawableTimeout
  API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

@end

NS_ASSUME_NONNULL_END
