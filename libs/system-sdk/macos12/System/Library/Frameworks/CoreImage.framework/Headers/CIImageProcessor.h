/* 
   CoreImage - CIImageProcessor.h

   Copyright (c) 2016 Apple Inc.
   All rights reserved. 
*/

#import <CoreImage/CIImage.h>

#if TARGET_OS_OSX
#import <IOSurface/IOSurface.h>
#elif !TARGET_OS_SIMULATOR
#import <IOSurface/IOSurfaceRef.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@protocol MTLTexture, MTLCommandBuffer;

@protocol CIImageProcessorInput;
@protocol CIImageProcessorOutput;

// In order to use a CIImageProcessorInput & CIImageProcessorOutput you must
// subclass from a CIImageProcessorKernel and override the methods you need to
// produce the desired output.


NS_CLASS_AVAILABLE(10_12, 10_0)
@interface CIImageProcessorKernel : NSObject

// Override this class method to implement your processor's subclass of CIImageProcessorKernel.
// The class method will be called to produce the requested region of the output image
// given the required regions of the input images and other arguments.
//
// The class method is passed two objects:
//     'inputs’  An array of id<CIImageProcessorInput> that the block consumes to produces output.
//               The input.region may be larger than the rect returned by 'roiForInputAtIndex'.
//     'output'  The id<CIImageProcessorOutput> that the block must provide results to.
//     ‘arguments’ The arguments dictionary passed to applyWithExtent:inputs:arguments:error:
// The contents of these objects are not valid outside the scope of this method.
//
// Note that since this is a class method you cannot use or capture any state by accident.
// All the parameters that affect the output results must be passed in ‘inputs’ and ‘arguments'.
// This supports 0, 1, 2 or more input images.
//
+ (BOOL)processWithInputs:(nullable NSArray<id<CIImageProcessorInput>> *)inputs
                arguments:(nullable NSDictionary<NSString*,id> *)arguments
                   output:(id <CIImageProcessorOutput>)output
                    error:(NSError **)error;

// Override this class method to implement your processor’s ROI callback, the default implementation would return outputRect.
// This will be called one or more times per render to determine what portion
// of the input images are needed to render a given 'outputRect' of the output.
// This will not be called if there are 0 input images.
//
// Note that since this is a class method you cannot use or capture any state by accident.
// All the parameters that affect the output results must be passed in ‘inputs’ and ‘arguments’.
//
+ (CGRect)roiForInput:(int)input
            arguments:(nullable NSDictionary<NSString*,id> *)arguments
           outputRect:(CGRect)outputRect;

// Override this class method if you want your any of the inputs to be in a specific supported CIPixelFormat.
// The format must be one of kCIFormatBGRA8, kCIFormatRGBAh, kCIFormatRGBAf or kCIFormatR8.
// On iOS 12 and macOS 10.14, the formats kCIFormatRh and kCIFormatRf are also supported.
// If the requested inputFormat is 0, then the input will be a supported format that best
// matches the rendering context's workingFormat.
//
// If a processor wants data in a colorspace other than the context workingspace,
// then call imageByColorMatchingWorkingSpaceToColorSpace on the processor input.
// If a processor wants it input as alpha-unpremultiplied RGBA data, then call
// imageByUnpremultiplyingAlpha on the processor input.
//
+ (CIFormat)formatForInputAtIndex:(int)input;

// Override this class property if you want your processor's output to be in a specific supported CIPixelFormat.
// The format must be one of kCIFormatBGRA8, kCIFormatRGBAh, kCIFormatRGBAf or kCIFormatR8.
// On iOS 12 and macOS 10.14, the formats kCIFormatRh and kCIFormatRf are also supported.
// If the outputFormat is 0, then the output will be a supported format that best
// matches the rendering context's workingFormat.
//
// If a processor returns data in a colorspace other than the context workingspace,
// then call imageByColorMatchingColorSpaceToWorkingSpace on the processor output.
// If a processor returns data as alpha-unpremultiplied RGBA data, then call,
// imageByPremultiplyingAlpha on the processor output.
//
#if __has_feature(objc_class_property)
@property (class, readonly) CIFormat outputFormat;
#else
+(CIFormat)outputFormat;
#endif

// Override this class property if your processor's output stores 1.0 into the
// alpha channel of all pixels within the output extent.
// If not overridden, false is returned.
//
#if __has_feature(objc_class_property)
@property (class, readonly) bool outputIsOpaque NS_AVAILABLE(10_13, 11_0);
#else
+ (bool)outputIsOpaque NS_AVAILABLE(10_13, 11_0);
#endif

// Override this class property to return false if you want your processor to be given
// CIImageProcessorInput objects that have not been synchonized for CPU access.
//
// Generally, if your subclass uses the GPU your should override this method to return false.
// If not overridden, true is returned.
//
#if __has_feature(objc_class_property)
@property (class, readonly) bool synchronizeInputs;
#else
+(bool)synchronizeInputs;
#endif


// Call this method on your CIImageProcessorKernel subclass to create a new CIImage of the specified extent.
// The inputs and arguments will be retained so that your subclass can be called when the image is drawn.
// Arguments is a dictionary containing inmutable objects of type NSData, NSString, NSNumber,
// CIVector or CIColor.
//
// This method will return [CIImage emptyImage] if extent is empty.
//
// This method will return nil and an error if:
// * calling outputFormat on your subclass returns an unsupported format
// * calling formatForInputAtIndex: on your subclass returns an unsupported format
// * your subclass does not implement processWithInputs:arguments:output:error:
//
+ (nullable CIImage *)applyWithExtent:(CGRect)extent
                               inputs:(nullable NSArray<CIImage*> *)inputs
                            arguments:(nullable NSDictionary<NSString*,id> *)args
                                error:(NSError **)error;
@end


NS_CLASS_AVAILABLE(10_12, 10_0)
@protocol CIImageProcessorInput

// The rectangular region of the input image that the processor block can use to provide the output.
// This will be contain (but may be larger than) the rect returned by 'roiCallback'.
@property (nonatomic, readonly) CGRect region;


// The bytes per row of the input buffer that the processor block can read from.
@property (nonatomic, readonly) size_t bytesPerRow;

// The pixel format of the input buffer that the processor block can read from.
@property (nonatomic, readonly) CIFormat format;

// The base address of the input buffer that the processor block can read from.
// This memory must not be modified by the block.
@property (readonly, nonatomic) const void *baseAddress NS_RETURNS_INNER_POINTER;

#if COREIMAGE_SUPPORTS_IOSURFACE
// An input IOSurface that the processor block can read from.
// This surface must not be modified by the block.
@property (nonatomic, readonly) IOSurfaceRef surface;
#endif

// An input CVPixelBuffer that the processor block can read from.
// This buffer must not be modified by the block.
@property (nonatomic, readonly, nullable) CVPixelBufferRef pixelBuffer;


// A MTLTexture object that can be bound as input (if processing using Metal).
// This texture must not be modified by the block.
@property (nonatomic, readonly, nullable) id<MTLTexture> metalTexture;

@end


NS_CLASS_AVAILABLE(10_12, 10_0)
@protocol CIImageProcessorOutput

// The rectangular region of the output image that the processor block must provide.
@property (nonatomic, readonly) CGRect region;


// The bytes per row of the output buffer that the processor block can write to.
@property (nonatomic, readonly) size_t bytesPerRow;

// The pixel format of the output buffer that the processor block can write to.
@property (nonatomic, readonly) CIFormat format;

// The base address of the output buffer that the processor block can write output pixels to.
@property (readonly, nonatomic) void *baseAddress NS_RETURNS_INNER_POINTER;

#if COREIMAGE_SUPPORTS_IOSURFACE
// An output IOSurface that the processor block can write to.
@property (nonatomic, readonly) IOSurfaceRef surface;
#endif

// A output CVPixelBuffer that the processor block can write to.
@property (nonatomic, readonly, nullable) CVPixelBufferRef pixelBuffer;


// A MTLTexture object that can be bound as output (if processing using Metal).
@property (nonatomic, readonly, nullable) id<MTLTexture> metalTexture;

// Returns a MTLCommandBuffer that can be used for encoding commands (if rendering using Metal).
@property (nonatomic, readonly, nullable) id<MTLCommandBuffer> metalCommandBuffer;

@end


NS_ASSUME_NONNULL_END
