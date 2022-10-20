/*
 CoreImage - CIRenderDestination.h
 
 Copyright (c) 2017 Apple, Inc.
 All rights reserved.
 */


#import <Foundation/Foundation.h>
#import <CoreImage/CoreImageDefines.h>
#import <CoreImage/CIImage.h>
#import <CoreImage/CIContext.h>
#import <CoreImage/CIKernel.h>
#import <CoreVideo/CoreVideo.h>
#if COREIMAGE_SUPPORTS_IOSURFACE
#import <IOSurface/IOSurfaceObjC.h>
#endif
#import <Metal/MTLPixelFormat.h>

@protocol MTLTexture, MTLCommandBuffer;

NS_ASSUME_NONNULL_BEGIN


// This is a lightweight API to allow clients to specify all the
// attributes of a render that pertain to the render's destination.
// It is intended to be used for issuing renders that return to the
// caller as soon as all the work has been issued but before it completes/
//
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIRenderDestination : NSObject
{
    void *_priv;
}

// MARK: PixelBuffer destinations

// Create a CIRenderDestination based on a CVPixelBufferRef object.
//
// The destination's 'colorspace' property will default a CGColorSpace created by,
// querying the CVPixelBufferRef attributes.
//
- (instancetype) initWithPixelBuffer:(CVPixelBufferRef)pixelBuffer;

// MARK: Surface destinations

#if COREIMAGE_SUPPORTS_IOSURFACE
// Create a CIRenderDestination based on an IOSurface object.
//
// The destination's 'colorspace' property will default a CGColorSpace created by,
// querying the IOSurface attributes.
//
- (instancetype) initWithIOSurface:(IOSurface*)surface;
#endif

// MARK: Metal destinations

// A render to a MTLTexture-backed CIRenderDestination is only supported by MTLTexture-backed CIContexts.
// The texture must have a MTLTextureType of MTLTextureType2D
//
// An optional MTLCommandBuffer can be specified, with which to use for rendering to the MTLTexture.
// NOTE: Rendering to a texture initialized with a commandBuffer requires encoding all the commands to render an image into the specified buffer.
// This may impact system responsiveness and may result in higher memory usage if the image requires many passes to render.
// To avoid this impact, it is recommended to create a context using [CIContext contextWithMTLCommandQueue:] and create the CIRenderDestination without specifying a buffer.
//
// The destination's 'colorspace' property will default a CGColorSpace created with kCGColorSpaceSRGB,
// kCGColorSpaceExtendedSRGB, or kCGColorSpaceGenericGrayGamma2_2.
//
- (instancetype) initWithMTLTexture:(id<MTLTexture>)texture
                      commandBuffer:(nullable id<MTLCommandBuffer>)commandBuffer;

// Create a CIRenderDestination based on a Metal texture.
//
// Rendering to a MTLTexture-backed CIRenderDestination is only supported by MTLTexture-backed CIContexts.
// The provider 'block' will be called lazily when the destination is rendered to.
// The block must return a texture with a MTLTextureType of MTLTextureType2D.
// The 'width', 'height' and 'pixelFormat' argument values should be the same as the
// width, height and pixelFormat of the MTLTexture that will be returned by 'block'
//
// An optional MTLCommandBuffer can be specified, with which to use for rendering to the MTLTexture.
// NOTE: Rendering to a texture initialized with a commandBuffer requires encoding all the commands to render an image into the specified buffer.
// This may impact system responsiveness and may result in higher memory usage if the image requires many passes to render.
// To avoid this impact, it is recommended to create a context using [CIContext contextWithMTLCommandQueue:] and create the CIRenderDestination without specifying a buffer.
//
// The destination's 'colorspace' property will default a CGColorSpace created with kCGColorSpaceSRGB,
// kCGColorSpaceExtendedSRGB, or kCGColorSpaceGenericGrayGamma2_2.
//
- (instancetype) initWithWidth:(NSUInteger)width
                        height:(NSUInteger)height
                   pixelFormat:(MTLPixelFormat)pixelFormat
                 commandBuffer:(nullable id<MTLCommandBuffer>)commandBuffer
            mtlTextureProvider:(nullable id<MTLTexture> (^)(void))block;

// MARK: OpenGL destination

// Create a CIRenderDestination based on an OpenGL texture.
//
// Rendering to a GLTexture-backed CIRenderDestination is only supported by GLContext-backed CIContexts.
// The texture id must be bound to a GLContext that is shared with that of the GLContext-backed CIContext.
//
// The destination's 'colorspace' property will default a CGColorSpace created with kCGColorSpaceSRGB,
// kCGColorSpaceExtendedSRGB, or kCGColorSpaceGenericGrayGamma2_2.
//
- (instancetype) initWithGLTexture:(unsigned int)texture
		                    target:(unsigned int)target // GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_EXT
                             width:(NSUInteger)width
                            height:(NSUInteger)height;

// MARK: Bitmap data destination

// Create a CIRenderDestination based on client-managed buffer.
//
// The 'data' parameter must point to a buffer that is at least bytesPerRow * height bytes in size.
// 
// The destination's 'colorspace' property will default a CGColorSpace created with kCGColorSpaceSRGB,
// kCGColorSpaceExtendedSRGB, or kCGColorSpaceGenericGrayGamma2_2.
- (instancetype) initWithBitmapData:(void *)data
                              width:(NSUInteger)width
                             height:(NSUInteger)height
                        bytesPerRow:(NSUInteger)bytesPerRow
                             format:(CIFormat)format;


// MARK: Properties

@property (readonly) NSUInteger width;
@property (readonly) NSUInteger height;


typedef NS_ENUM(NSUInteger, CIRenderDestinationAlphaMode) {
    CIRenderDestinationAlphaNone            = 0,
    CIRenderDestinationAlphaPremultiplied   = 1,
    CIRenderDestinationAlphaUnpremultiplied = 2
};

// This property will defualt to an appropriate value given
// the object that the CIRenderDestination was initialized with.
// This property can be set to a different value if desired.
@property CIRenderDestinationAlphaMode alphaMode;

// The logical coordinate system of a CIRenderDestination is always cartesian:
//   (0,0) represents the lower-left corner
//   (0.5,0.5) represents the lower-left pixel center
//   (pixelsWide-0.5,pixelsHigh-0.5) represents the upper-right pixel center
//   (pixelsWide,pixelsHigh) represents the upper-right corner.
//
// The 'flipped' property controls how pixels this logical coordinate system
// are stored into the memory of the object backing the destination.
//
// If 'flipped' is false, then the base address of the backing stores the
// pixel centered on the logical coordinate (0.5,0.5)
//
// If 'flipped' is true, then the base address of the backing stores the
// pixel centered on the logical coordinate (0.5,height-0.5)
//
@property (getter=isFlipped) BOOL flipped;

// Instructs the render to add pseudo-random luma noise given the depth of the destination.
// The magnitude of the noise is approximatly ±pow(2,-(bitPerComponent+1))
@property (getter=isDithered) BOOL dithered;

// If true, the render will clamp color channels
// to 0..alpha if 'alphaMode' is premultiplied otherwise 0..1
// This property is initialized to false if the destination's format supports extended range
@property (getter=isClamped) BOOL clamped;

// This property will defualt to an appropriate value given
// the object that the CIRenderDestination was initialized with.
// This property can be  set to a different colorSpace if desired.
// This property can be set to nil to disable color matching
// from the working space to the destination.
@property (nullable, nonatomic) CGColorSpaceRef colorSpace;

// Allow client to specify a CIBlendKernel (e.g. CIBlendKernel.sourceOver)
// to be used on the destination.
@property (nullable, nonatomic, retain) CIBlendKernel* blendKernel;

// If true, then the blendKernel is applied in the destination's colorSpace.
// If false, then the blendKernel is applied in the CIContext's working colorspace.
// This is false by default.
@property BOOL blendsInDestinationColorSpace;

@end


// An Xcode quicklook of this object will show a graph visualization of the render
// with detailed timing information.
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIRenderInfo : NSObject
{
    void *_priv;
}

// This property will return how much time a render spent executing kernels.
@property (readonly) NSTimeInterval kernelExecutionTime;

// This property will return how many passes the render requires.
// If passCount is 1 than the render can be fully concatinated and no
// intermediate buffers will be required.
@property (readonly) NSInteger passCount;

// This property will return how many pixels a render produced executing kernels.
@property (readonly) NSInteger pixelsProcessed;

@end



// An Xcode quicklook of this object will show a graph visualization of the render
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIRenderTask : NSObject
{
    void *_priv;
}

- (nullable CIRenderInfo*) waitUntilCompletedAndReturnError:(NSError**)error;

@end


@interface CIContext (CIRenderDestination)

// Renders a portion of image to a point of destination
// It renders as if 'image' is cropped to 'fromRect'
// and the origin of the result is placed at 'atPoint'
//
// If image.extent and fromRect are infinite, then it renders
// so that point (0,0) of image is placed at 'atPoint'
//
// MTLTexture-backed CIRenderDestinations are only supported by MTLTexture-backed CIContexts.
// GLTexture-backed CIRenderDestinations are only supported by GLContext-backed CIContexts.
//
// For contexts that are initialized with a command queue, this call will return as soon as all the work for the render is enqueued on the
// context's device.
// Otherwise, it will return as soon as all the work is scheduled.
//
// In many situations, after issuing a render, the client can use the destination
// or its backing object without waiting for the enqueued work to complete.
// For example, after rendering a surface CIRenderDestination, the surface can be passed
// on for further processing by the GPU.
//
// In other situations, the client may need to wait for the render to be complete.
// For example, after rendering a surface CIRenderDestination, the surface can be accessed
// by CPU code by calling IOSurfaceGetBytePointer only after the render is completed.
//
// In this case the client can use the returned CIRenderTask like this:
//   CIRenderTask* task = [context render:...];
//   [task waitUntilCompletedAndReturnError:&error];
//
- (nullable CIRenderTask*) startTaskToRender:(CIImage*)image
                                    fromRect:(CGRect)fromRect
                               toDestination:(CIRenderDestination*)destination
                                     atPoint:(CGPoint)atPoint
                                       error:(NSError**)error NS_AVAILABLE(10_13, 11_0);

// Renders an image to a destination so that point (0,0) of image.
// is placed at point (0,0) of the destination.
//
- (nullable CIRenderTask*) startTaskToRender:(CIImage*)image
                               toDestination:(CIRenderDestination*)destination
                                       error:(NSError**)error NS_AVAILABLE(10_13, 11_0);


// This is an optional call which can be used to "warm up" a CIContext so that
// a subsequent call to render with the same arguments can be more efficient.
// By making this call, Core Image will ensure that
//  - any needed kernels will be compiled
//  - any intermedate buffers are allocated and marked volatile
//
- (BOOL) prepareRender:(CIImage*)image
              fromRect:(CGRect)fromRect
         toDestination:(CIRenderDestination*)destination
               atPoint:(CGPoint)atPoint
                 error:(NSError**)error NS_AVAILABLE(10_13, 11_0);

// Fill the entire destination with black (0,0,0,1) if its alphaMode is None
// or clear (0,0,0,0) if its alphaMode is Premultiplied or Unpremultiplied.
//
- (nullable CIRenderTask*) startTaskToClear:(CIRenderDestination*)destination
                                      error:(NSError**)error NS_AVAILABLE(10_13, 11_0);

@end

NS_ASSUME_NONNULL_END
