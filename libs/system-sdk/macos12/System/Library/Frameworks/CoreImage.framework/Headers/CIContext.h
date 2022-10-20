/* CoreImage - CIContext.h

   Copyright (c) 2014 Apple, Inc.
   All rights reserved. */

#import <CoreImage/CIImage.h>
#import <CoreImage/CoreImageDefines.h>
#import <CoreVideo/CoreVideo.h>

#if TARGET_OS_IPHONE && (TARGET_OS_EMBEDDED || TARGET_OS_SIMULATOR || !TARGET_OS_MACCATALYST)
 #import <OpenGLES/EAGL.h>
#elif TARGET_OS_OSX
 #import <OpenGL/CGLTypes.h>
#endif 

@class CIFilter;

@protocol MTLDevice, MTLTexture, MTLCommandBuffer, MTLCommandQueue;

NS_ASSUME_NONNULL_BEGIN


NS_CLASS_AVAILABLE(10_4, 5_0)
@interface CIContext : NSObject
{
    void *_priv;
}

/* Keys that may be passed in the dictionary while creating contexts: */

typedef NSString * CIContextOption NS_TYPED_ENUM;

/* A CGColorSpaceRef object defining the color space that images are
 * converted to before rendering into the context. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextOutputColorSpace;

/* A CGColorSpaceRef object defining the color space in which all
 * intermediate operations are performed. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextWorkingColorSpace;

/* An NSNumber with a CIFormat value defining the pixel format to use for intermediate buffers.
 * On iOS the supported values for this key are RGBA8 and RGBAh. If not specified:
 *   RGBA8 is used if app is linked against OSX 10.12 SDK or earlier.
 *   RGBAh is used if app is linked against OSX 10.13 SDK or later.
 * On OSX the supported values for this key are RGBA8, RGBAh and RGBAf. If not specified, RGBAh is used. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextWorkingFormat NS_AVAILABLE(10_4,8_0);

/* A boolean NSNumber controlling the quality of affine downsample operations.
 * @YES implies that more quality is desired.
 * On iOS the default value is @NO.
 * On OSX the default value is @YES. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextHighQualityDownsample NS_AVAILABLE(10_11,9_0);

/* A boolean NSNumber controlling whether output renders produce alpha-premultiplied pixels.
 * The default value is @YES. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextOutputPremultiplied NS_AVAILABLE(10_4,7_0);

/* A boolean NSNumber controlling how intermediate buffers are cached.
 * If @NO, the context will empty intermediates during and after renders.
 * The default value is @YES. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextCacheIntermediates NS_AVAILABLE(10_12,10_0);

/* An NSNumber with a boolean value. When @YES the context will use
 * software rendering on macOS. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextUseSoftwareRenderer;

/* An NSNumber with a boolean value. When @YES the context will use 
 * low priority rendering on the GPU. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextPriorityRequestLow NS_AVAILABLE(10_12, 8_0);

/* A boolean value specifying whether or not to allow use of low-power devices for GPU rendering.
 * If @YES, the context will use a low power GPU if available and the high power device is not already in use.
 * The default value is @NO which instructs the context to use the highest power/performance device. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextAllowLowPower NS_AVAILABLE(10_12, 13_0);

/* An NSString specifying a client-provided name for a context.
 * This name will be used in QuickLook graphs and the output of CI_PRINT_TREE. */
CORE_IMAGE_EXPORT CIContextOption const kCIContextName NS_AVAILABLE(10_14,12_0);


#pragma mark - contextWithCGLContext

/* Create a new CoreImage context object, all output will be drawn
 * into the surface attached to the OpenGL context 'cglctx'. If 'pixelFormat' is
 * non-null it should be the pixel format object used to create 'cglctx';
 * it's required to be valid for the lifetime of the CIContext.
 * The colorspace should be set to the colorspace of your target otherwise
 * CI will take the colorspace from the CGLContext if available. */
#if TARGET_OS_OSX
+ (CIContext *)contextWithCGLContext:(CGLContextObj)cglctx
						 pixelFormat:(nullable CGLPixelFormatObj)pixelFormat
						  colorSpace:(nullable CGColorSpaceRef)colorSpace
							 options:(nullable NSDictionary<CIContextOption, id> *)options
    CI_GL_DEPRECATED_MAC(10_6,10_14);
#endif

/* DEPRECATED, please use the method above or if you need this
* for backward capability, make sure that you specify the colorspace
* in the options dictionary */
#if TARGET_OS_OSX
+ (CIContext *)contextWithCGLContext:(CGLContextObj)cglctx
						 pixelFormat:(nullable CGLPixelFormatObj)pixelFormat
							 options:(nullable NSDictionary<CIContextOption, id> *)options
    CI_GL_DEPRECATED_MAC(10_4,10_6);
#endif


#pragma mark - contextWithCGContext

/* Create a context specifying a destination CGContext.
 *
 * Core Image will use an internal destination context when methods such
 * as [context render:to...] or [context createCGImage:...] are called.
 *
 * The [context drawImage:...] render methods will render to the CGContext.
 */
+ (CIContext *)contextWithCGContext:(CGContextRef)cgctx
                            options:(nullable NSDictionary<CIContextOption, id> *)options
    NS_AVAILABLE(10_4,9_0);


#pragma mark - context without specifying a destination

/* Create a context without specifying a destination CG/GL/Metal context.
 *
 * Core Image will use an internal destination context when methods such
 * as [context render:to...] or [context createCGImage:...] are called.
 *
 * The [context drawImage:...] render methods will not operate on this type
 * of context.
 */

+ (CIContext *)contextWithOptions:(nullable NSDictionary<CIContextOption, id> *)options
    NS_AVAILABLE(10_4,5_0);

+ (CIContext *)context NS_AVAILABLE(10_4,5_0);

- (instancetype)initWithOptions:(nullable NSDictionary<CIContextOption, id> *)options
NS_AVAILABLE(10_4,5_0);

- (instancetype)init NS_AVAILABLE(10_4,5_0);


#pragma mark - contextWithEAGLContext

/* Create a context specifying a destination EAGLContext.
 *
 * Core Image will use an internal destination context when methods such
 * as [context render:to...] or [context createCGImage:...] are called.
 *
 * The [context drawImage:...] render methods will render to the EAGLContext.
 */
#if TARGET_OS_IPHONE && (TARGET_OS_EMBEDDED || TARGET_OS_SIMULATOR || !TARGET_OS_MACCATALYST)
+ (CIContext *)contextWithEAGLContext:(EAGLContext *)eaglContext
    CI_GL_DEPRECATED_IOS(5_0,12_0);

+ (CIContext *)contextWithEAGLContext:(EAGLContext *)eaglContext
                              options:(nullable NSDictionary<CIContextOption, id> *)options
    CI_GL_DEPRECATED_IOS(5_0,12_0);
#endif


#pragma mark - contextWithMTLDevice

/* If a system has more than one MTLDevice, then you can create a CIContext
 * that uses a specific device. If a client wishes to use the default MTLDevice
 * then call [CIContext contextWithOptions:] instead. */
+ (CIContext *)contextWithMTLDevice:(id<MTLDevice>)device NS_AVAILABLE(10_11,9_0);

+ (CIContext *)contextWithMTLDevice:(id<MTLDevice>)device
                            options:(nullable NSDictionary<CIContextOption, id> *)options
    NS_AVAILABLE(10_11,9_0);

#pragma mark - contextWithMTLCommandQueue

/* Create a CIContext that commits commands to a specific queue.*/
+ (CIContext *)contextWithMTLCommandQueue:(id<MTLCommandQueue>)commandQueue NS_AVAILABLE(10_15,13_0);

+ (CIContext *)contextWithMTLCommandQueue:(id<MTLCommandQueue>)commandQueue
                            options:(nullable NSDictionary<CIContextOption, id> *)options
NS_AVAILABLE(10_15,13_0);


#pragma mark - properties

// The working color space of the CIContext
// The property will be null if the context was created with color management disabled.
@property (nullable, nonatomic, readonly) CGColorSpaceRef workingColorSpace NS_AVAILABLE(10_11,9_0);

// The working pixel format of the CIContext used for intermediate buffers
@property (nonatomic, readonly) CIFormat workingFormat NS_AVAILABLE(10_11,9_0);


#pragma mark - render methods

/* DEPRECATED, please use drawImage:inRect:fromRect: instead.
 * Render the subregion 'fromRect' of 'image' to point 'atPoint' in the context's destination. */
- (void)drawImage:(CIImage *)image
          atPoint:(CGPoint)atPoint
         fromRect:(CGRect)fromRect NS_DEPRECATED(10_4,10_8, 5_0,6_0);

/* Render the rectangle 'fromRect' of 'image' to the rectangle 'inRect' in the
 * context's destination. */
- (void)drawImage:(CIImage *)image
           inRect:(CGRect)inRect
         fromRect:(CGRect)fromRect;

/* Create a CoreGraphics layer object suitable for creating content for
 * subsequently rendering into this CI context. The 'info' parameter is
 * passed into CGLayerCreate () as the auxiliaryInfo dictionary.
 * This will return null if size is empty or infinite. */
- (nullable CGLayerRef)createCGLayerWithSize:(CGSize)size
                                        info:(nullable CFDictionaryRef)info
CF_RETURNS_RETAINED NS_DEPRECATED_MAC(10_4,10_11);

/* Render 'image' to the given bitmap.
 * The 'data' parameter must point to at least rowBytes*floor(bounds.size.height) bytes.
 * The 'bounds' parameter has the following behavior:
 *    The 'bounds' parameter acts to specify the region of 'image' to render.
 *    This region (regardless of its origin) is rendered at upper-left corner of 'data'.
 * Passing a 'colorSpace' value of null means:
 *   Disable output color management if app is linked against iOS SDK
 *   Disable output color management if app is linked against OSX 10.11 SDK or later
 *   Match to context's output color space if app is linked against OSX 10.10 SDK or earlier
 */
- (void)render:(CIImage *)image
	  toBitmap:(void *)data
	  rowBytes:(ptrdiff_t)rowBytes
		bounds:(CGRect)bounds
		format:(CIFormat)format
	colorSpace:(nullable CGColorSpaceRef)colorSpace;

#if COREIMAGE_SUPPORTS_IOSURFACE
/* Render 'image' to the given IOSurface.
 * The 'bounds' parameter has the following behavior:
 *    The 'image' is rendered into 'surface' so that
 *      point (0,0) of 'image' aligns to the lower left corner of 'surface'.
 *      The 'bounds' acts like a clip rect to limit what region of 'surface' is modified.
 */
- (void)render:(CIImage *)image
   toIOSurface:(IOSurfaceRef)surface
		bounds:(CGRect)bounds
	colorSpace:(nullable CGColorSpaceRef)colorSpace NS_AVAILABLE(10_6,5_0);
#endif

/* Render 'image' into the given CVPixelBuffer. 
 * In OS X 10.11.3 and iOS 9.3 and later
 *   CI will color match to the colorspace of the buffer.
 * otherwise
 *   CI will color match to the context's output colorspace.
 */
- (void)render:(CIImage *)image 
toCVPixelBuffer:(CVPixelBufferRef)buffer NS_AVAILABLE(10_11,5_0);

/* Render 'image' to the given CVPixelBufferRef.
 * The 'bounds' parameter has the following behavior:
 *    In OS X and iOS 9 and later:  The 'image' is rendered into 'buffer' so that
 *      point (0,0) of 'image' aligns to the lower left corner of 'buffer'.
 *      The 'bounds' acts like a clip rect to limit what region of 'buffer' is modified.
 *    In iOS 8 and earlier: The 'bounds' parameter acts to specify the region of 'image' to render.
 *      This region (regardless of its origin) is rendered at upper-left corner of 'buffer'.
 * If 'colorSpace' is nil, CI will not color match to the destination.
 */
- (void)render:(CIImage *)image
toCVPixelBuffer:(CVPixelBufferRef)buffer
        bounds:(CGRect)bounds
    colorSpace:(nullable CGColorSpaceRef)colorSpace NS_AVAILABLE(10_11,5_0);

/* Render 'bounds' of 'image' to a Metal texture, optionally specifying what command buffer to use.
 * Texture type must be MTLTexture2D.
 * NOTE: Rendering to a texture initialized with a commandBuffer requires encoding all the commands to render an image into the specified buffer.
 * This may impact system responsiveness and may result in higher memory usage if the image requires many passes to render.
 * To avoid this impact, it is recommended to create a context using [CIContext contextWithMTLCommandQueue:].
 */
- (void)render:(CIImage *)image
  toMTLTexture:(id<MTLTexture>)texture
 commandBuffer:(nullable id<MTLCommandBuffer>)commandBuffer
        bounds:(CGRect)bounds
    colorSpace:(CGColorSpaceRef)colorSpace NS_AVAILABLE(10_11,9_0);


#pragma mark -

/* Runs the context's garbage collector to reclaim any resources that
 * are no longer required (e.g. removes textures from the texture cache
 * that reference deleted images.) This method is called automatically
 * after every rendering operation. */
- (void)reclaimResources NS_AVAILABLE_MAC(10_4);

/* Frees any cached data (such as temporary images) associated with the
 * context. This also runs the garbage collector. */
- (void)clearCaches NS_AVAILABLE(10_4,10_0);

/* Returns the maximum dimension for input images that can be processed 
 * on the context. */
- (CGSize)inputImageMaximumSize NS_AVAILABLE_IOS(5_0);

/* Returns the maximum dimension for image that can be rendered 
 * on the context. */
- (CGSize)outputImageMaximumSize NS_AVAILABLE_IOS(5_0);

@end


@interface CIContext (createCGImage)

/* Render the region 'fromRect' of image 'image' into a temporary buffer using
 * the context, then create and return a new CoreGraphics image with
 * the results. The caller is responsible for releasing the returned image.
 * The return value will be null if size is empty or too big. */
- (nullable CGImageRef)createCGImage:(CIImage *)image
                            fromRect:(CGRect)fromRect
CF_RETURNS_RETAINED;

/* Create a new CGImage from the specified subrect of the image. If
 * non-nil the new image will be created in the specified format and colorspace.
 * The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome
 * and must match the specified CIFormat.
 * This will return null if fromRect is empty or infinite or the format isn't supported.
 */
- (nullable CGImageRef)createCGImage:(CIImage *)image
                            fromRect:(CGRect)fromRect
                              format:(CIFormat)format
                          colorSpace:(nullable CGColorSpaceRef)colorSpace
CF_RETURNS_RETAINED;

/* Create a new CGImage from the specified subrect of the image.
 * The new CGImageRef will be created in the specified format and colorspace.
 * The return value will be null if fromRect is empty or infinite.
 * The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome
 * and must match the specified CIFormat.
 * This will return null if fromRect is empty or infinite or the format isn't supported.
 * If deferred is NO, then the CIImage will be rendered once when this method is called.
 * If deferred is YES, then the CIImage will be rendered whenever the CGImage is rendered.
 */
- (nullable CGImageRef)createCGImage:(CIImage *)image
                            fromRect:(CGRect)fromRect
                              format:(CIFormat)format
                          colorSpace:(nullable CGColorSpaceRef)colorSpace
                            deferred:(BOOL)deferred
CF_RETURNS_RETAINED NS_AVAILABLE(10_12,10_0);

@end


@interface CIContext (OfflineGPUSupport)

/* Not all GPUs will be driving a display. If they are offline we can still use them
 * to do work with Core Image. This method returns the number of offline GPUs which
 * can be used for this purpose */
+(unsigned int)offlineGPUCount NS_AVAILABLE_MAC(10_10);

/* These two methods lets you create a CIContext based on an offline gpu index.
 * The first method takes only the GPU index as a parameter, the second, takes
 * an optional colorspace, options dictionary and a CGLContext which can be
 * shared with other GL resources.  The return value will be null if index is 
 * out of range (e.g. if the device has no offline GPUs).
 */
#if TARGET_OS_OSX
+ (nullable CIContext *)contextForOfflineGPUAtIndex:(unsigned int)index CI_GL_DEPRECATED_MAC(10_10,10_14);
+ (nullable CIContext *)contextForOfflineGPUAtIndex:(unsigned int)index
                                         colorSpace:(nullable CGColorSpaceRef)colorSpace
                                            options:(nullable NSDictionary<CIContextOption, id> *)options
                                      sharedContext:(nullable CGLContextObj)sharedContext CI_GL_DEPRECATED_MAC(10_10,10_14);
#endif


@end

typedef NSString * CIImageRepresentationOption NS_TYPED_ENUM; 

@interface CIContext (ImageRepresentation)

// The value for kCIImageRepresentationAVDepthData should be an AVDepthData object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationAVDepthData NS_AVAILABLE(10_13,11_0);

// The value for kCIImageRepresentationDepthImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationDepthImage NS_AVAILABLE(10_13,11_0);

// The value for kCIImageRepresentationDisparityImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationDisparityImage NS_AVAILABLE(10_13,11_0);


// The value for kCIImageRepresentationAVPortraitEffectsMatte should be an AVPortraitEffectsMatte object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationAVPortraitEffectsMatte NS_AVAILABLE(10_14,12_0);

// The value for kCIImageRepresentationPortraitEffectsMatteImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationPortraitEffectsMatteImage NS_AVAILABLE(10_14,12_0);


// The value for kCIImageRepresentationAVSemanticSegmentationMattes should be an array of AVSemanticSegmentationMatte objects. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationAVSemanticSegmentationMattes NS_AVAILABLE(10_15, 13_0);

// The value for kCIImageRepresentationSemanticSegmentationSkinMatteImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationSemanticSegmentationSkinMatteImage NS_AVAILABLE(10_15, 13_0);

// The value for kCIImageRepresentationSemanticSegmentationHairMatteImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationSemanticSegmentationHairMatteImage NS_AVAILABLE(10_15, 13_0);

// The value for kCIImageRepresentationSemanticSegmentationTeethMatteImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationSemanticSegmentationTeethMatteImage NS_AVAILABLE(10_15, 13_0);

// The value for kCIImageRepresentationSemanticSegmentationGlassesMatteImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationSemanticSegmentationGlassesMatteImage NS_AVAILABLE(11_0, 14_1);

// The value for kCIImageRepresentationSemanticSegmentationSkyMatteImage should be a monochome CIImage object. */
CORE_IMAGE_EXPORT CIImageRepresentationOption const kCIImageRepresentationSemanticSegmentationSkyMatteImage NS_AVAILABLE(11_1, 14_3);


/* Render a CIImage to TIFF data. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome */
/* and must match the specified CIFormat. */
/* No options keys are supported at this time. */
- (nullable NSData*) TIFFRepresentationOfImage:(CIImage*)image
                                        format:(CIFormat)format
                                    colorSpace:(CGColorSpaceRef)colorSpace
                                       options:(NSDictionary<CIImageRepresentationOption, id>*)options NS_AVAILABLE(10_12,10_0);

/* Render a CIImage to JPEG data. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome. */
/* Supported options keys are kCGImageDestinationLossyCompressionQuality, */
/* and the depth, disparity, and matte options. */
- (nullable NSData*) JPEGRepresentationOfImage:(CIImage*)image
                                    colorSpace:(CGColorSpaceRef)colorSpace
                                       options:(NSDictionary<CIImageRepresentationOption, id>*)options NS_AVAILABLE(10_12,10_0);

/* Render a CIImage to HEIF data. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome */
/* and must match the specified CIFormat. */
/* Supported options keys are kCGImageDestinationLossyCompressionQuality, */
/* and the depth, disparity, and matte options. */
- (nullable NSData*) HEIFRepresentationOfImage:(CIImage*)image
                                        format:(CIFormat)format
                                    colorSpace:(CGColorSpaceRef)colorSpace
                                       options:(NSDictionary<CIImageRepresentationOption, id>*)options NS_AVAILABLE(10_13_4,11_0);

/* Render a CIImage to HEIF data. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome. */
/* Supported options keys are kCGImageDestinationLossyCompressionQuality, */
/* and the depth, disparity, and matte options. */
- (nullable NSData*) HEIF10RepresentationOfImage:(CIImage*)image
                                      colorSpace:(CGColorSpaceRef)colorSpace
                                         options:(NSDictionary<CIImageRepresentationOption, id>*)options
                                           error:(NSError **)errorPtr NS_AVAILABLE(12_0,15_0);

/* Render a CIImage to PNG data. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome */
/* and must match the specified CIFormat. */
/* No options keys are supported at this time. */
- (nullable NSData*) PNGRepresentationOfImage:(CIImage*)image
                                       format:(CIFormat)format
                                   colorSpace:(CGColorSpaceRef)colorSpace
                                      options:(NSDictionary<CIImageRepresentationOption, id>*)options NS_AVAILABLE(10_13,11_0);

/* Render a CIImage to TIFF file. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome */
/* and must match the specified CIFormat. */
/* No options keys are supported at this time. */
- (BOOL) writeTIFFRepresentationOfImage:(CIImage*)image
                                  toURL:(NSURL*)url
                                 format:(CIFormat)format
                             colorSpace:(CGColorSpaceRef)colorSpace 
                                options:(NSDictionary<CIImageRepresentationOption, id>*)options
                                  error:(NSError **)errorPtr NS_AVAILABLE(10_12,10_0);

/* Render a CIImage to PNG file. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome */
/* and must match the specified CIFormat. */
/* No options keys are supported at this time. */
- (BOOL) writePNGRepresentationOfImage:(CIImage*)image
                                 toURL:(NSURL*)url
                                format:(CIFormat)format
                            colorSpace:(CGColorSpaceRef)colorSpace
                               options:(NSDictionary<CIImageRepresentationOption, id>*)options
                                 error:(NSError **)errorPtr NS_AVAILABLE(10_13,11_0);

/* Render a CIImage to JPEG file. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome. */
/* Supported options keys are kCGImageDestinationLossyCompressionQuality, */
/* and the depth, disparity, and matte options. */
- (BOOL) writeJPEGRepresentationOfImage:(CIImage*)image
                                  toURL:(NSURL*)url
                             colorSpace:(CGColorSpaceRef)colorSpace
                                options:(NSDictionary<CIImageRepresentationOption, id>*)options
                                  error:(NSError **)errorPtr NS_AVAILABLE(10_12,10_0);

/* Render a CIImage to HEIF file. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome */
/* and must match the specified CIFormat. */
/* Supported options keys are kCGImageDestinationLossyCompressionQuality, */
/* and the depth, disparity, and matte options. */
- (BOOL) writeHEIFRepresentationOfImage:(CIImage*)image
                                  toURL:(NSURL*)url
                                 format:(CIFormat)format
                             colorSpace:(CGColorSpaceRef)colorSpace
                                options:(NSDictionary<CIImageRepresentationOption, id>*)options
                                  error:(NSError **)errorPtr NS_AVAILABLE(10_13_4,11_0);

/* Render a CIImage to 10-bit deep HEIF file. Image must have a finite non-empty extent. */
/* The CGColorSpace must be kCGColorSpaceModelRGB or kCGColorSpaceModelMonochrome. */
/* Supported options keys are kCGImageDestinationLossyCompressionQuality, */
/* and the depth, disparity, and matte options. */
- (BOOL) writeHEIF10RepresentationOfImage:(CIImage*)image
                                    toURL:(NSURL*)url
                               colorSpace:(CGColorSpaceRef)colorSpace
                                  options:(NSDictionary<CIImageRepresentationOption, id>*)options
                                    error:(NSError **)errorPtr NS_AVAILABLE(12_0,15_0);


@end

@interface CIContext (CIDepthBlurEffect)

/* Create and CIFilter instance for 'image' that can be used to apply the CIDepthBlurEffect.
 *
 * The receiver context is user to render the image in order to get the facial landmarks
 *
 * The 'options' parameter is a key value/pair reserved for future use.
 *
 */

- (nullable CIFilter *) depthBlurEffectFilterForImageURL:(NSURL *)url options:(nullable NSDictionary *)options NS_AVAILABLE(10_14,12_0);

/* This is the same as the method above expect it uses NSData to instantiate the image data
 * instead of the contents of a NSURL.
 *
 */

- (nullable CIFilter *) depthBlurEffectFilterForImageData:(NSData *)data options:(nullable NSDictionary *)options NS_AVAILABLE(10_14,12_0);


/* Create and CIFilter instance for 'image' that can be used to apply the CIDepthBlurEffect.
 *
 * The receiver context is user to render the image in order to get the facial landmarks
 *
 * The 'orientation' parameter should be CGImagePropertyOrientation enum value
 * as defined in the TIFF spec.
 *
 * The 'options' parameter is a key value/pair reserved for future use.
 *
 */

- (nullable CIFilter *) depthBlurEffectFilterForImage:(CIImage *)image
                                       disparityImage:(CIImage *)disparityImage
                                 portraitEffectsMatte:(nullable CIImage *)portraitEffectsMatte
                                          orientation:(CGImagePropertyOrientation)orientation
                                              options:(nullable NSDictionary *)options NS_AVAILABLE(10_14,12_0);

- (nullable CIFilter *) depthBlurEffectFilterForImage:(CIImage *)image
                                       disparityImage:(CIImage *)disparityImage
                                 portraitEffectsMatte:(nullable CIImage *)portraitEffectsMatte
                             hairSemanticSegmentation:(nullable CIImage *)hairSemanticSegmentation
                                          orientation:(CGImagePropertyOrientation)orientation
                                              options:(nullable NSDictionary *)options NS_AVAILABLE(10_15,13_0);

- (nullable CIFilter *) depthBlurEffectFilterForImage:(CIImage *)image
                                       disparityImage:(CIImage *)disparityImage
                                 portraitEffectsMatte:(nullable CIImage *)portraitEffectsMatte
                             hairSemanticSegmentation:(nullable CIImage *)hairSemanticSegmentation
                                         glassesMatte:(nullable CIImage *)glassesMatte
                                              gainMap:(nullable CIImage *)gainMap
                                          orientation:(CGImagePropertyOrientation)orientation
                                              options:(nullable NSDictionary *)options NS_AVAILABLE(11_0,14_1);

@end


NS_ASSUME_NONNULL_END
