/*
 *  CVPixelFormatDescription.h
 *  CoreVideo
 *
 *  Copyright (c) 2013-2015 Apple Inc. All rights reserved.
 *
 */

#if !defined(__COREVIDEO_CVPIXELFORMATDESCRIPTION_H__)
#define __COREVIDEO_CVPIXELFORMATDESCRIPTION_H__

#include <TargetConditionals.h>
#include <Availability.h>
#include <AvailabilityMacros.h>

#include <CoreVideo/CoreVideo.h>
#include <CoreFoundation/CFDictionary.h>
#include <CoreFoundation/CFArray.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* This document is influenced by Ice Floe #19: http://developer.apple.com/quicktime/icefloe/dispatch019.html */

/* The canonical name for the format.  This should bethe same as the codec name you'd use in QT */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatName __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* QuickTime/QuickDraw Pixel Format Type constant (OSType) */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatConstant __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* This is the codec type constant, i.e. '2vuy' or k422YpCbCr8CodecType */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatCodecType __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* This is the equivalent Microsoft FourCC code for this pixel format */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatFourCC __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* kCFBooleanTrue indicates that the format contains alpha and some images may be considered transparent;
   kCFBooleanFalse indicates that there is no alpha and images are always opaque. */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatContainsAlpha __OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_4_3);
	
/* kCFBooleanTrue indicates that the format contains YCbCr data; */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatContainsYCbCr __OSX_AVAILABLE_STARTING(__MAC_10_10,__IPHONE_8_0);
	
/* kCFBooleanTrue indicates that the format contains RGB data; */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatContainsRGB __OSX_AVAILABLE_STARTING(__MAC_10_10,__IPHONE_8_0);

/* kCFBooleanTrue indicates that the format contains Grayscale data; */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatContainsGrayscale __OSX_AVAILABLE_STARTING(__MAC_10_14,__IPHONE_12_0);

CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatComponentRange __OSX_AVAILABLE_STARTING(__MAC_10_10,__IPHONE_9_0);

CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatComponentRange_VideoRange __OSX_AVAILABLE_STARTING(__MAC_10_10,__IPHONE_9_0);

CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatComponentRange_FullRange __OSX_AVAILABLE_STARTING(__MAC_10_10,__IPHONE_9_0);

CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatComponentRange_WideRange __OSX_AVAILABLE_STARTING(__MAC_10_10,__IPHONE_9_0);

/* All buffers have one or more image planes.  Each plane may contain a single or an interleaved set of components */   
/* For simplicity sake, pixel formats that are not planar may place the required format keys at the top
   level dictionary. */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatPlanes __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* The following keys describe the requirements/layout of a a single image plane. */

/* Used to assist with allocating memory for pixel formats that don't have an integer value for
   bytes per pixel */
/* Block width is essentially the width in pixels of the smallest "byte addressable" group of pixels */
/* This works in close conjunction with BitsPerBlock */
/* Examples:
   8-bit luminance only, BlockWidth would be 1, BitsPerBlock would be 8
   16-bit 1555 RGB, BlockWidth would be 1, BitsPerBlock would be 16
   32-bit 8888 ARGB, BlockWidth would be 1, BitsPerBlock would be 32
   2vuy (CbYCrY), BlockWidth would be 2, BitsPerBlock would be 32
   1-bit bitmap, BlockWidth would be 8, BitsPerBlock would be 8
   v210, BlockWidth would be 6, BitsPerBlock would be 128 */
/* Values assumed to 1 be one if not present */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatBlockWidth __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatBlockHeight __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* This value must be present.  For simple pixel formats this will be equivalent to the traditional 
   bitsPerPixel value. */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatBitsPerBlock __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* Used to state requirements on block multiples.  v210 would be '8' here for the horizontal case, 
   to match the standard v210 row alignment value of 48.
   These may be assumed as 1 if not present. */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatBlockHorizontalAlignment __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatBlockVerticalAlignment __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* CFData containing the bit pattern for a block of black pixels.  If absent, black is assumed to be all zeros.
   If present, this should be bitsPerPixel bits long -- if bitsPerPixel is less than a byte, repeat the bit pattern 
   for the full byte.  */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatBlackBlock __OSX_AVAILABLE_STARTING(__MAC_10_6,__IPHONE_4_0);

/* Subsampling information for this plane.  Assumed to be '1' if not present. */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatHorizontalSubsampling __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatVerticalSubsampling __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* If present, these two keys describe the OpenGL format and type enums you would use to describe this
   image plane to OpenGL */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatOpenGLFormat __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatOpenGLType __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatOpenGLInternalFormat __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* CGBitmapInfo value, if required */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatCGBitmapInfo __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* Pixel format compatibility flags */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatQDCompatibility API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatCGBitmapContextCompatibility API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatCGImageCompatibility API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatOpenGLCompatibility API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatOpenGLESCompatibility API_AVAILABLE(ios(5.0), tvos(9.0)) API_UNAVAILABLE(macosx) API_UNAVAILABLE(macCatalyst) __WATCHOS_PROHIBITED;
    
/* This callback routine implements code to handle the functionality of CVPixelBufferFillExtendedPixels.  
   For custom pixel formats where you will never need to use that call, this is not required. */
typedef Boolean (*CVFillExtendedPixelsCallBack)(CVPixelBufferRef CV_NONNULL pixelBuffer, void * CV_NULLABLE refCon);
typedef struct {
    CFIndex version;
    CVFillExtendedPixelsCallBack CV_NULLABLE fillCallBack;
    void * CV_NULLABLE refCon;
} CVFillExtendedPixelsCallBackData;

/* The value for this key is a CFData containing a CVFillExtendedPixelsCallBackData struct */
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatFillExtendedPixelsCallback __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* Create a description of a pixel format from a provided OSType */
CV_EXPORT CFDictionaryRef CF_RETURNS_RETAINED CV_NULLABLE CVPixelFormatDescriptionCreateWithPixelFormatType(CFAllocatorRef CV_NULLABLE allocator, OSType pixelFormat) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* Get a CFArray of CFNumbers containing all known pixel formats as OSTypes */
CV_EXPORT CFArrayRef CF_RETURNS_RETAINED CV_NULLABLE CVPixelFormatDescriptionArrayCreateWithAllPixelFormatTypes(CFAllocatorRef CV_NULLABLE allocator) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/* Register a new pixel format with CoreVideo */
CV_EXPORT void CVPixelFormatDescriptionRegisterDescriptionWithPixelFormatType(CFDictionaryRef CV_NONNULL description, OSType pixelFormat) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
	@function   CVIsCompressedPixelFormatAvailable
	@abstract   Checks if a compressed pixel format is supported on the current platform.
	@param      pixelFormatType compressed pixel format.
	@result     True if pixel format is supported on the current platform.
*/
CV_EXPORT Boolean CVIsCompressedPixelFormatAvailable( OSType pixelFormatType ) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0));

#if COREVIDEO_SUPPORTS_DIRECT3D
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDirect3DFormat;
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDirect3DType;
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDirect3DInternalFormat;
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDirect3DCompatibility;
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDXGICompatibility;
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDXGIFormat;
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDXGIType;
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelFormatDXGIInternalFormat;
#endif

#if defined(__cplusplus)
}
#endif

#endif

