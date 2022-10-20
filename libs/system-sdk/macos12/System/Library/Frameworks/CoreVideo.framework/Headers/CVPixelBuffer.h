/*
 *  CVPixelBuffer.h
 *  CoreVideo
 *
 *  Copyright (c) 2004-2021 Apple Inc. All rights reserved.
 *
 */
 
  /*! @header CVPixelBuffer.h
	@copyright 2004-2021 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later, and iOS 4.0 or later
    @discussion CVPixelBuffers are CVImageBuffers that hold the pixels in main memory
		   
*/

#if !defined(__COREVIDEO_CVPIXELBUFFER_H__)
#define __COREVIDEO_CVPIXELBUFFER_H__ 1

#include <TargetConditionals.h>
#include <Availability.h>
#include <AvailabilityMacros.h>

#include <CoreVideo/CVImageBuffer.h>
#include <CoreFoundation/CFArray.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
CoreVideo pixel format type constants.
CoreVideo does not provide support for all of these formats; this list just defines their names.
*/
#if COREVIDEO_USE_DERIVED_ENUMS_FOR_CONSTANTS
enum : OSType
#else
enum
#endif
{
  kCVPixelFormatType_1Monochrome    = 0x00000001, /* 1 bit indexed */
  kCVPixelFormatType_2Indexed       = 0x00000002, /* 2 bit indexed */
  kCVPixelFormatType_4Indexed       = 0x00000004, /* 4 bit indexed */
  kCVPixelFormatType_8Indexed       = 0x00000008, /* 8 bit indexed */
  kCVPixelFormatType_1IndexedGray_WhiteIsZero = 0x00000021, /* 1 bit indexed gray, white is zero */
  kCVPixelFormatType_2IndexedGray_WhiteIsZero = 0x00000022, /* 2 bit indexed gray, white is zero */
  kCVPixelFormatType_4IndexedGray_WhiteIsZero = 0x00000024, /* 4 bit indexed gray, white is zero */
  kCVPixelFormatType_8IndexedGray_WhiteIsZero = 0x00000028, /* 8 bit indexed gray, white is zero */
  kCVPixelFormatType_16BE555        = 0x00000010, /* 16 bit BE RGB 555 */
  kCVPixelFormatType_16LE555        = 'L555',     /* 16 bit LE RGB 555 */
  kCVPixelFormatType_16LE5551       = '5551',     /* 16 bit LE RGB 5551 */
  kCVPixelFormatType_16BE565        = 'B565',     /* 16 bit BE RGB 565 */
  kCVPixelFormatType_16LE565        = 'L565',     /* 16 bit LE RGB 565 */
  kCVPixelFormatType_24RGB          = 0x00000018, /* 24 bit RGB */
  kCVPixelFormatType_24BGR          = '24BG',     /* 24 bit BGR */
  kCVPixelFormatType_32ARGB         = 0x00000020, /* 32 bit ARGB */
  kCVPixelFormatType_32BGRA         = 'BGRA',     /* 32 bit BGRA */
  kCVPixelFormatType_32ABGR         = 'ABGR',     /* 32 bit ABGR */
  kCVPixelFormatType_32RGBA         = 'RGBA',     /* 32 bit RGBA */
  kCVPixelFormatType_64ARGB         = 'b64a',     /* 64 bit ARGB, 16-bit big-endian samples */
  kCVPixelFormatType_64RGBALE       = 'l64r',     /* 64 bit RGBA, 16-bit little-endian full-range (0-65535) samples */
  kCVPixelFormatType_48RGB          = 'b48r',     /* 48 bit RGB, 16-bit big-endian samples */
  kCVPixelFormatType_32AlphaGray    = 'b32a',     /* 32 bit AlphaGray, 16-bit big-endian samples, black is zero */
  kCVPixelFormatType_16Gray         = 'b16g',     /* 16 bit Grayscale, 16-bit big-endian samples, black is zero */
  kCVPixelFormatType_30RGB          = 'R10k',     /* 30 bit RGB, 10-bit big-endian samples, 2 unused padding bits (at least significant end). */
  kCVPixelFormatType_422YpCbCr8     = '2vuy',     /* Component Y'CbCr 8-bit 4:2:2, ordered Cb Y'0 Cr Y'1 */
  kCVPixelFormatType_4444YpCbCrA8   = 'v408',     /* Component Y'CbCrA 8-bit 4:4:4:4, ordered Cb Y' Cr A */
  kCVPixelFormatType_4444YpCbCrA8R  = 'r408',     /* Component Y'CbCrA 8-bit 4:4:4:4, rendering format. full range alpha, zero biased YUV, ordered A Y' Cb Cr */
  kCVPixelFormatType_4444AYpCbCr8   = 'y408',     /* Component Y'CbCrA 8-bit 4:4:4:4, ordered A Y' Cb Cr, full range alpha, video range Y'CbCr. */
  kCVPixelFormatType_4444AYpCbCr16  = 'y416',     /* Component Y'CbCrA 16-bit 4:4:4:4, ordered A Y' Cb Cr, full range alpha, video range Y'CbCr, 16-bit little-endian samples. */
  kCVPixelFormatType_444YpCbCr8     = 'v308',     /* Component Y'CbCr 8-bit 4:4:4 */
  kCVPixelFormatType_422YpCbCr16    = 'v216',     /* Component Y'CbCr 10,12,14,16-bit 4:2:2 */
  kCVPixelFormatType_422YpCbCr10    = 'v210',     /* Component Y'CbCr 10-bit 4:2:2 */
  kCVPixelFormatType_444YpCbCr10    = 'v410',     /* Component Y'CbCr 10-bit 4:4:4 */
  kCVPixelFormatType_420YpCbCr8Planar = 'y420',   /* Planar Component Y'CbCr 8-bit 4:2:0.  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrPlanar struct */
  kCVPixelFormatType_420YpCbCr8PlanarFullRange    = 'f420',   /* Planar Component Y'CbCr 8-bit 4:2:0, full range.  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrPlanar struct */
  kCVPixelFormatType_422YpCbCr_4A_8BiPlanar = 'a2vy', /* First plane: Video-range Component Y'CbCr 8-bit 4:2:2, ordered Cb Y'0 Cr Y'1; second plane: alpha 8-bit 0-255 */
  kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange = '420v', /* Bi-Planar Component Y'CbCr 8-bit 4:2:0, video-range (luma=[16,235] chroma=[16,240]).  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrBiPlanar struct */
  kCVPixelFormatType_420YpCbCr8BiPlanarFullRange  = '420f', /* Bi-Planar Component Y'CbCr 8-bit 4:2:0, full-range (luma=[0,255] chroma=[1,255]).  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrBiPlanar struct */ 
  kCVPixelFormatType_422YpCbCr8BiPlanarVideoRange = '422v', /* Bi-Planar Component Y'CbCr 8-bit 4:2:2, video-range (luma=[16,235] chroma=[16,240]).  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrBiPlanar struct */
  kCVPixelFormatType_422YpCbCr8BiPlanarFullRange  = '422f', /* Bi-Planar Component Y'CbCr 8-bit 4:2:2, full-range (luma=[0,255] chroma=[1,255]).  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrBiPlanar struct */
  kCVPixelFormatType_444YpCbCr8BiPlanarVideoRange = '444v', /* Bi-Planar Component Y'CbCr 8-bit 4:4:4, video-range (luma=[16,235] chroma=[16,240]).  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrBiPlanar struct */
  kCVPixelFormatType_444YpCbCr8BiPlanarFullRange  = '444f', /* Bi-Planar Component Y'CbCr 8-bit 4:4:4, full-range (luma=[0,255] chroma=[1,255]).  baseAddr points to a big-endian CVPlanarPixelBufferInfo_YCbCrBiPlanar struct */
  kCVPixelFormatType_422YpCbCr8_yuvs = 'yuvs',     /* Component Y'CbCr 8-bit 4:2:2, ordered Y'0 Cb Y'1 Cr */
  kCVPixelFormatType_422YpCbCr8FullRange = 'yuvf', /* Component Y'CbCr 8-bit 4:2:2, full range, ordered Y'0 Cb Y'1 Cr */
  kCVPixelFormatType_OneComponent8  = 'L008',     /* 8 bit one component, black is zero */
  kCVPixelFormatType_TwoComponent8  = '2C08',     /* 8 bit two component, black is zero */
  kCVPixelFormatType_30RGBLEPackedWideGamut = 'w30r', /* little-endian RGB101010, 2 MSB are zero, wide-gamut (384-895) */
  kCVPixelFormatType_ARGB2101010LEPacked = 'l10r',     /* little-endian ARGB2101010 full-range ARGB */
  kCVPixelFormatType_40ARGBLEWideGamut      = 'w40a', /* little-endian ARGB10101010, each 10 bits in the MSBs of 16bits, wide-gamut (384-895, including alpha) */
  kCVPixelFormatType_40ARGBLEWideGamutPremultiplied = 'w40m', /* little-endian ARGB10101010, each 10 bits in the MSBs of 16bits, wide-gamut (384-895, including alpha). Alpha premultiplied */
  kCVPixelFormatType_OneComponent10      = 'L010',     /* 10 bit little-endian one component, stored as 10 MSBs of 16 bits, black is zero */
  kCVPixelFormatType_OneComponent12      = 'L012',     /* 12 bit little-endian one component, stored as 12 MSBs of 16 bits, black is zero */
  kCVPixelFormatType_OneComponent16      = 'L016',     /* 16 bit little-endian one component, black is zero */
  kCVPixelFormatType_TwoComponent16      = '2C16',     /* 16 bit little-endian two component, black is zero */
  kCVPixelFormatType_OneComponent16Half  = 'L00h',     /* 16 bit one component IEEE half-precision float, 16-bit little-endian samples */
  kCVPixelFormatType_OneComponent32Float = 'L00f',     /* 32 bit one component IEEE float, 32-bit little-endian samples */
  kCVPixelFormatType_TwoComponent16Half  = '2C0h',     /* 16 bit two component IEEE half-precision float, 16-bit little-endian samples */
  kCVPixelFormatType_TwoComponent32Float = '2C0f',     /* 32 bit two component IEEE float, 32-bit little-endian samples */
  kCVPixelFormatType_64RGBAHalf          = 'RGhA',     /* 64 bit RGBA IEEE half-precision float, 16-bit little-endian samples */
  kCVPixelFormatType_128RGBAFloat        = 'RGfA',     /* 128 bit RGBA IEEE float, 32-bit little-endian samples */
  kCVPixelFormatType_14Bayer_GRBG        = 'grb4',     /* Bayer 14-bit Little-Endian, packed in 16-bits, ordered G R G R... alternating with B G B G... */
  kCVPixelFormatType_14Bayer_RGGB        = 'rgg4',     /* Bayer 14-bit Little-Endian, packed in 16-bits, ordered R G R G... alternating with G B G B... */
  kCVPixelFormatType_14Bayer_BGGR        = 'bgg4',     /* Bayer 14-bit Little-Endian, packed in 16-bits, ordered B G B G... alternating with G R G R... */
  kCVPixelFormatType_14Bayer_GBRG        = 'gbr4',     /* Bayer 14-bit Little-Endian, packed in 16-bits, ordered G B G B... alternating with R G R G... */
  kCVPixelFormatType_DisparityFloat16	 = 'hdis',     /* IEEE754-2008 binary16 (half float), describing the normalized shift when comparing two images. Units are 1/meters: ( pixelShift / (pixelFocalLength * baselineInMeters) ) */
  kCVPixelFormatType_DisparityFloat32	 = 'fdis',     /* IEEE754-2008 binary32 float, describing the normalized shift when comparing two images. Units are 1/meters: ( pixelShift / (pixelFocalLength * baselineInMeters) ) */
  kCVPixelFormatType_DepthFloat16	     = 'hdep',     /* IEEE754-2008 binary16 (half float), describing the depth (distance to an object) in meters */
  kCVPixelFormatType_DepthFloat32	     = 'fdep',     /* IEEE754-2008 binary32 float, describing the depth (distance to an object) in meters */
  kCVPixelFormatType_420YpCbCr10BiPlanarVideoRange = 'x420', /* 2 plane YCbCr10 4:2:0, each 10 bits in the MSBs of 16bits, video-range (luma=[64,940] chroma=[64,960]) */
  kCVPixelFormatType_422YpCbCr10BiPlanarVideoRange = 'x422', /* 2 plane YCbCr10 4:2:2, each 10 bits in the MSBs of 16bits, video-range (luma=[64,940] chroma=[64,960]) */
  kCVPixelFormatType_444YpCbCr10BiPlanarVideoRange = 'x444', /* 2 plane YCbCr10 4:4:4, each 10 bits in the MSBs of 16bits, video-range (luma=[64,940] chroma=[64,960]) */
  kCVPixelFormatType_420YpCbCr10BiPlanarFullRange  = 'xf20', /* 2 plane YCbCr10 4:2:0, each 10 bits in the MSBs of 16bits, full-range (Y range 0-1023) */
  kCVPixelFormatType_422YpCbCr10BiPlanarFullRange  = 'xf22', /* 2 plane YCbCr10 4:2:2, each 10 bits in the MSBs of 16bits, full-range (Y range 0-1023) */
  kCVPixelFormatType_444YpCbCr10BiPlanarFullRange  = 'xf44', /* 2 plane YCbCr10 4:4:4, each 10 bits in the MSBs of 16bits, full-range (Y range 0-1023) */
  kCVPixelFormatType_420YpCbCr8VideoRange_8A_TriPlanar   = 'v0a8', /* first and second planes as per 420YpCbCr8BiPlanarVideoRange (420v), alpha 8 bits in third plane full-range.  No CVPlanarPixelBufferInfo struct. */
  kCVPixelFormatType_16VersatileBayer    = 'bp16',   /* Single plane Bayer 16-bit little-endian sensor element ("sensel") samples from full-size decoding of ProRes RAW images; Bayer pattern (sensel ordering) and other raw conversion information is described via buffer attachments */
  kCVPixelFormatType_64RGBA_DownscaledProResRAW    = 'bp64',   /* Single plane 64-bit RGBA (16-bit little-endian samples) from downscaled decoding of ProRes RAW images; components--which may not be co-sited with one another--are sensel values and require raw conversion, information for which is described via buffer attachments */
  kCVPixelFormatType_422YpCbCr16BiPlanarVideoRange       = 'sv22', /* 2 plane YCbCr16 4:2:2, video-range (luma=[4096,60160] chroma=[4096,61440]) */
  kCVPixelFormatType_444YpCbCr16BiPlanarVideoRange       = 'sv44', /* 2 plane YCbCr16 4:4:4, video-range (luma=[4096,60160] chroma=[4096,61440]) */
  kCVPixelFormatType_444YpCbCr16VideoRange_16A_TriPlanar = 's4as', /* 3 plane video-range YCbCr16 4:4:4 with 16-bit full-range alpha (luma=[4096,60160] chroma=[4096,61440] alpha=[0,65535]).  No CVPlanarPixelBufferInfo struct. */
};

	
/*
	Lossless-Compressed Pixel Formats
	
	The following pixel formats can be used to reduce the memory bandwidth involved in large-scale pixel data flow, which can have benefits for battery life and thermal efficiency.
	They work by dividing pixel buffers into fixed-width, fixed-height, fixed-byte-size blocks.  Hardware units (video codecs, GPU, ISP, etc.) attempt to write a compressed encoding for each block using a lossless algorithm.  If a block of pixels is successfully encoded using fewer bytes than the uncompressed pixel data, the hardware unit does not need to write as many bytes for that pixel block.  If the encoding is unsuccessful, the uncompressed pixel data is written, filling the whole pixel block.  Each compressed pixel buffer has a separate area of metadata recording the encoding choices for each pixel block.
	
	Padding bits are eliminated, so for example, 10-bit-per-component lossless-compressed pixel buffers are slightly smaller than their uncompressed equivalents.  For pixel formats with no padding, the lossless-compressed pixel buffers are slightly larger due to the metadata.
	
	IMPORTANT CAVEATS:
	Some devices do not support these pixel formats at all. 
	Before using one of these pixel formats, call CVIsCompressedPixelFormatAvailable() to check that it is available on the current device.
	On different devices, the concrete details of these formats may be different.
	On different devices, the degree and details of support by hardware units (video codecs, GPU, ISP, etc.) may be different.
	Do not ship code that reads the contents of lossless-compressed pixel buffers directly with the CPU, or which saves or transfers it to other devices, as this code will break with future hardware.
	The bandwidth benefits of these formats are generally outweighed by the cost of buffer copies to convert to uncompressed pixel formats, so if you find that you need to perform a buffer copy to covert for CPU usage, it's likely that you would have been better served by using the equivalent uncompressed pixel formats in the first place.
*/
#if COREVIDEO_USE_DERIVED_ENUMS_FOR_CONSTANTS
enum : OSType
#else
enum
#endif
{
	kCVPixelFormatType_Lossless_32BGRA                               = '&BGA', /* Lossless-compressed form of kCVPixelFormatType_32BGRA. */
	
	// Lossless-compressed Bi-planar YCbCr pixel format types
	kCVPixelFormatType_Lossless_420YpCbCr8BiPlanarVideoRange         = '&8v0', /* Lossless-compressed form of kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange.  No CVPlanarPixelBufferInfo struct. */
	kCVPixelFormatType_Lossless_420YpCbCr8BiPlanarFullRange          = '&8f0', /* Lossless-compressed form of kCVPixelFormatType_420YpCbCr8BiPlanarFullRange.  No CVPlanarPixelBufferInfo struct. */
	kCVPixelFormatType_Lossless_420YpCbCr10PackedBiPlanarVideoRange  = '&xv0', /* Lossless-compressed-packed form of kCVPixelFormatType_420YpCbCr10BiPlanarVideoRange.  No CVPlanarPixelBufferInfo struct. Format is compressed-packed with no padding bits between pixels. */
	kCVPixelFormatType_Lossless_422YpCbCr10PackedBiPlanarVideoRange  = '&xv2', /* Lossless-compressed form of kCVPixelFormatType_422YpCbCr10BiPlanarVideoRange.  No CVPlanarPixelBufferInfo struct. Format is compressed-packed with no padding bits between pixels. */
};

/*
	Lossy-Compressed Pixel Formats
	
	The following pixel formats can be used to reduce memory bandwidth and memory footprint involved in large-scale pixel data flow, which can have benefits for battery life and thermal efficiency.
	Similar to lossless pixel formats, they work by dividing pixel buffers into fixed-width, fixed-height, fixed-byte-size blocks. Pixel buffers allocated using lossy formats have reduced memory footprint than their lossless equivalents; this reduced footprint may or may not result in loss of quality depending on the content of the individual block. Hardware units (video codecs, GPU, ISP, etc.) attempt to write a compressed encoding for each block using either a lossless or lossy algorithm.  If a block of pixels is successfully encoded within its pre-defined memory footprint, then the lossless alogrithm is applied; if the encoded block of pixels exceeds the pre-defined memory footprint then the lossy algorithm is applied.  Each compressed pixel buffer has a separate area of metadata recording the encoding choices for each pixel block.
	
	IMPORTANT CAVEATS:
	Some devices do not support these pixel formats at all.
	Before using one of these pixel formats, call CVIsCompressedPixelFormatAvailable() to check that it is available on the current device.
	On different devices, the concrete details of these formats may be different.
	On different devices, the degree and details of support by hardware units (video codecs, GPU, ISP, etc.) may be different.
	Do not ship code that reads the contents of lossless-compressed pixel buffers directly with the CPU, or which saves or transfers it to other devices, as this code will break with future hardware.
	The bandwidth benefits of these formats are generally outweighed by the cost of buffer copies to convert to uncompressed pixel formats, so if you find that you need to perform a buffer copy to covert for CPU usage, it's likely that you would have been better served by using the equivalent uncompressed pixel formats in the first place.
*/
#if COREVIDEO_USE_DERIVED_ENUMS_FOR_CONSTANTS
enum : OSType
#else
enum
#endif
{
	kCVPixelFormatType_Lossy_32BGRA									= '-BGA', /* Lossy-compressed form of kCVPixelFormatType_32BGRA. No CVPlanarPixelBufferInfo struct.  */
	kCVPixelFormatType_Lossy_420YpCbCr8BiPlanarVideoRange			= '-8v0', /* Lossy-compressed form of kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange.  No CVPlanarPixelBufferInfo struct. */
	kCVPixelFormatType_Lossy_420YpCbCr8BiPlanarFullRange			= '-8f0', /* Lossy-compressed form of kCVPixelFormatType_420YpCbCr8BiPlanarFullRange.  No CVPlanarPixelBufferInfo struct. */
	kCVPixelFormatType_Lossy_420YpCbCr10PackedBiPlanarVideoRange	= '-xv0', /* Lossy-compressed form of kCVPixelFormatType_420YpCbCr10BiPlanarVideoRange.  No CVPlanarPixelBufferInfo struct. Format is compressed-packed with no padding bits between pixels. */
	kCVPixelFormatType_Lossy_422YpCbCr10PackedBiPlanarVideoRange	= '-xv2', /* Lossy-compressed form of kCVPixelFormatType_422YpCbCr10BiPlanarVideoRange.  No CVPlanarPixelBufferInfo struct. Format is compressed-packed with no padding bits between pixels. */
};

/*!
	@enum Pixel Buffer Locking Flags
	@discussion Flags to pass to CVPixelBufferLockBaseAddress() / CVPixelBufferUnlockBaseAddress()
	@constant kCVPixelBufferLock_ReadOnly
		If you are not going to modify the data while you hold the lock, you should set this flag
		to avoid potentially invalidating any existing caches of the buffer contents.  This flag
		should be passed both to the lock and unlock functions.  Non-symmetrical usage of this
		flag will result in undefined behavior.
*/
typedef CF_OPTIONS(CVOptionFlags, CVPixelBufferLockFlags)
{
	kCVPixelBufferLock_ReadOnly = 0x00000001,
};

/*
Planar pixel buffers have the following descriptor at their base address.  
Clients should generally use CVPixelBufferGetBaseAddressOfPlane, 
CVPixelBufferGetBytesPerRowOfPlane, etc. instead of accessing it directly.
*/
struct CVPlanarComponentInfo {
  int32_t             offset;    /* offset from main base address to base address of this plane, big-endian */
  uint32_t            rowBytes;  /* bytes per row of this plane, big-endian */
};
typedef struct CVPlanarComponentInfo      CVPlanarComponentInfo;
struct CVPlanarPixelBufferInfo {
  CVPlanarComponentInfo  componentInfo[1];
};
typedef struct CVPlanarPixelBufferInfo         CVPlanarPixelBufferInfo;
struct CVPlanarPixelBufferInfo_YCbCrPlanar {
  CVPlanarComponentInfo  componentInfoY;
  CVPlanarComponentInfo  componentInfoCb;
  CVPlanarComponentInfo  componentInfoCr;
};
typedef struct CVPlanarPixelBufferInfo_YCbCrPlanar   CVPlanarPixelBufferInfo_YCbCrPlanar;
struct CVPlanarPixelBufferInfo_YCbCrBiPlanar {
  CVPlanarComponentInfo  componentInfoY;
  CVPlanarComponentInfo  componentInfoCbCr;
};
typedef struct CVPlanarPixelBufferInfo_YCbCrBiPlanar   CVPlanarPixelBufferInfo_YCbCrBiPlanar;

#pragma mark BufferAttributeKeys
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferPixelFormatTypeKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));		    // A single CFNumber or a CFArray of CFNumbers (OSTypes)
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferMemoryAllocatorKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));		    // CFAllocatorRef
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferWidthKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));			    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferHeightKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));			    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferExtendedPixelsLeftKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));	    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferExtendedPixelsTopKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));		    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferExtendedPixelsRightKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));	    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferExtendedPixelsBottomKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));	    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferBytesPerRowAlignmentKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));	    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferCGBitmapContextCompatibilityKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));  // CFBoolean
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferCGImageCompatibilityKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));	    // CFBoolean
	CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferOpenGLCompatibilityKey API_AVAILABLE(macosx(10.4), ios(4.0), tvos(9.0), watchos(4.0));   // CFBoolean
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferPlaneAlignmentKey API_AVAILABLE(macosx(10.6), ios(4.0), tvos(9.0), watchos(4.0));		    // CFNumber
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferIOSurfacePropertiesKey API_AVAILABLE(macosx(10.6), ios(4.0), tvos(9.0), watchos(4.0));     // CFDictionary; presence requests buffer allocation via IOSurface
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferOpenGLESCompatibilityKey API_AVAILABLE(ios(6.0), tvos(9.0)) API_UNAVAILABLE(macosx) API_UNAVAILABLE(macCatalyst) __WATCHOS_PROHIBITED;	    // CFBoolean
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferMetalCompatibilityKey API_AVAILABLE(macosx(10.11), ios(8.0), tvos(9.0), watchos(4.0));	    // CFBoolean
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferOpenGLTextureCacheCompatibilityKey API_AVAILABLE(macosx(10.11)) API_UNAVAILABLE(ios, tvos, watchos);
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferOpenGLESTextureCacheCompatibilityKey API_AVAILABLE(ios(9.0), tvos(9.0)) API_UNAVAILABLE(macosx) API_UNAVAILABLE(macCatalyst) __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferVersatileBayerKey_BayerPattern
    @abstract   Buffer attachment key for code indicating Bayer pattern (sensel arrangement).
    @discussion Associated attachment is a CFNumber of type kCFNumberSInt32Type.  The value follows the semantics of the ProRes RAW bayer_pattern bitstream syntax element, namely 0, 1, 2, or 3, where 0 means the top-left sensel of the frame is red-filtered ("RGGB"); 1 means the top-left sensel of the frame is green-filtered, with the top row alternating between green- and red-filtered sensels ("GRBG"); 2 means the top-left sensel of the frame is green- filtered, with the top row alternating between green- and blue-filtered sensels ("GBRG"); and 3 means the top-left sensel of the frame is blue-filtered ("BGGR").  This attachment applies only to buffers with VersatileBayer formats.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferVersatileBayerKey_BayerPattern API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

enum {
	kCVVersatileBayer_BayerPattern_RGGB = 0,
	kCVVersatileBayer_BayerPattern_GRBG = 1,
	kCVVersatileBayer_BayerPattern_GBRG = 2,
	kCVVersatileBayer_BayerPattern_BGGR = 3,
};

/*!
    @const      kCVPixelBufferProResRAWKey_SenselSitingOffsets
    @abstract   Buffer attachment key for siting offsets, relative to pixel center, of individual sensels/components constituting each pixel.
    @discussion Associated attachment is CFData containing an array of 8 32-bit floats.  The eight CFData array elements specify, in order, the following sensel/component offsets from pixel center: red horizontal offset, red vertical offset, green horizontal offset, green vertical offset, blue horizontal offset, blue vertical offset, alpha horizontal offset, and alpha vertical offset.  A positive offset value indicates that the sensel/component lies to the right of or below the center of its pixel, while a negative value indicates that the sensel/component lies to the left of or above the center of its pixel.  Horizontal and vertical offset magnitudes are respectively in terms of the spacing between horizontally- and vertically-adjacent pixel centers.  This attachment applies only to buffers with the bp64 format, and is optional for those buffers; if not present, all offsets are considered to be 0.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_SenselSitingOffsets API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_BlackLevel
    @abstract   Buffer attachment key for sensel black level.
    @discussion Associated attachment is a CFNumber of type kCFNumberSInt32Type.  The value is the sensel level corresponding to no light exposure.  This attachment is required for buffers with either the bp16 or bp64 format.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_BlackLevel API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_WhiteLevel
    @abstract   Buffer attachment key for sensel white level.
    @discussion Associated attachment is a CFNumber of type kCFNumberSInt32Type.  The value is the sensel level corresponding to sensor (or camera A-to-D converter) saturation.  This attachment is required for buffers with either the bp16 or bp64 format.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_WhiteLevel API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_WhiteBalanceCCT
    @abstract   Buffer attachment key for illuminant correlated color temperature.
    @discussion Associated attachment is a CFNumber of type kCFNumberSInt32Type.  The value is the illuminant correlated color temperature (CCT), in kelvins, selected at the time of capture.  May be 0, indicating that the CCT is unknown or unspecified.  This attachment is optional for buffers with either the bp16 or bp64 format; if not present, the CCT is considered unknown or unspecified.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_WhiteBalanceCCT API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_WhiteBalanceRedFactor
    @abstract   Buffer attachment key for white balance red factor.
    @discussion Associated attachment is a CFNumber of type kCFNumberFloat32Type.  The value is the white balance multiplication factor for red-filtered sensels.  This attachment is required for buffers with either the bp16 or bp64 format.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_WhiteBalanceRedFactor API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_WhiteBalanceBlueFactor
    @abstract   Buffer attachment key for white balance blue factor.
    @discussion Associated attachment is a CFNumber of type kCFNumberFloat32Type.  The value is the white balance multiplication factor for blue-filtered sensels.  This attachment is required for buffers with either the bp16 or bp64 format.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_WhiteBalanceBlueFactor API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_ColorMatrix
    @abstract   Buffer attachment key for color translation matrix.
    @discussion Associated attachment is CFData containing an array of 9 32-bit floats.  The value is a 3x3 matrix which transforms linear RGB pixel values in the camera native color space to CIE 1931 XYZ values relative to the D65 illuminant, where the matrix entries are stored in the CFData in row-major order.  This attachment is required for buffers with either the bp16 or bp64 format.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_ColorMatrix API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_GainFactor
    @abstract   Buffer attachment key for gain factor.
    @discussion Associated attachment is a CFNumber of type kCFNumberFloat32Type.  The value is the overall gain factor for raw conversion.  This attachment is required for buffers with either the bp16 or bp64 format.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_GainFactor API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @const      kCVPixelBufferProResRAWKey_RecommendedCrop
    @abstract   Buffer attachment key for recommended number of pixels/rows to discard from the sides of the image after raw conversion.
    @discussion Associated attachment is CFData containing an array of 4 32-bit floats.  The four CFData array elements specify, in order, the recommended number of: pixels to discard from the start (left) of each row of the image; pixels to discard from the end (right) of each row of the image; rows of pixels to discard from the top of the image; and rows of pixels to discard from the bottom of the image.  (Pixels/rows are discarded after raw conversion.)  This attachment is optional for buffers with either the bp16 or bp64 format; if not present, the recommended crop values are considered to be 0.  For buffers with the bp64 format, the values may be nonintegral due to downscaling, in which case the handling of fractional parts is implementation-dependent.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_RecommendedCrop API_AVAILABLE(ios(14.0), macosx(11.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
	@const      kCVPixelBufferProResRAWKey_MetadataExtension
	@abstract   Buffer attachment key for metadata extension.
    @discussion Associated attachment is CFData containing ProRes RAW metadata extension. This attachment is optional for buffers with either bp16 or bp64.  The CFData contains a big-endian uint32 representing the size of the item in bytes followed by a 4-character code ('psim') followed by a variable-length pascal string identifying the metadata (like a key string) followed by the metadata payload.
*/
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferProResRAWKey_MetadataExtension API_AVAILABLE(ios(15.0), macosx(12.0)) __TVOS_PROHIBITED __WATCHOS_PROHIBITED;

/*!
    @typedef	CVPixelBufferRef
    @abstract   Based on the image buffer type. The pixel buffer implements the memory storage for an image buffer.

*/
typedef CVImageBufferRef CVPixelBufferRef;

CV_EXPORT CFTypeID CVPixelBufferGetTypeID(void) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferRetain
    @abstract   Retains a CVPixelBuffer object
    @discussion Equivalent to CFRetain, but NULL safe
    @param      buffer A CVPixelBuffer object that you want to retain.
    @result     A CVPixelBuffer object that is the same as the passed in buffer.
*/
CV_EXPORT CVPixelBufferRef CV_NULLABLE CVPixelBufferRetain( CVPixelBufferRef CV_NULLABLE texture ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferRelease
    @abstract   Releases a CVPixelBuffer object
    @discussion Equivalent to CFRelease, but NULL safe
    @param      buffer A CVPixelBuffer object that you want to release.
*/
CV_EXPORT void CVPixelBufferRelease( CV_RELEASES_ARGUMENT CVPixelBufferRef CV_NULLABLE texture ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferCreateResolvedAttributesDictionary
    @abstract   Takes a CFArray of CFDictionary objects describing various pixel buffer attributes and tries to resolve them into a
                single dictionary.
    @discussion This is useful when you need to resolve multiple requirements between different potential clients of a buffer.
    @param      attributes CFArray of CFDictionaries containing kCVPixelBuffer key/value pairs.
    @param      resolvedDictionaryOut The resulting dictionary will be placed here.
    @result     Return value that may be useful in discovering why resolution failed.
*/
CV_EXPORT CVReturn CVPixelBufferCreateResolvedAttributesDictionary(
    CFAllocatorRef CV_NULLABLE allocator,
    CFArrayRef CV_NULLABLE attributes,
    CV_RETURNS_RETAINED_PARAMETER CFDictionaryRef CV_NULLABLE * CV_NONNULL resolvedDictionaryOut) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);


/*!
    @function   CVPixelBufferCreate
    @abstract   Call to create a single PixelBuffer for a given size and pixelFormatType.
    @discussion Creates a single PixelBuffer for a given size and pixelFormatType. It allocates the necessary memory based on the pixel dimensions, pixelFormatType and extended pixels described in the pixelBufferAttributes. Not all parameters of the pixelBufferAttributes will be used here.
    @param      width   Width of the PixelBuffer in pixels.
    @param      height  Height of the PixelBuffer in pixels.
    @param	pixelFormatType		Pixel format indentified by its respective OSType.
    @param	pixelBufferAttributes      A dictionary with additional attributes for a pixel buffer. This parameter is optional. See BufferAttributeKeys for more details.
    @param      pixelBufferOut          The new pixel buffer will be returned here
    @result	returns kCVReturnSuccess on success.
*/    
CV_EXPORT CVReturn CVPixelBufferCreate(
    CFAllocatorRef CV_NULLABLE allocator,
    size_t width,
    size_t height,
    OSType pixelFormatType,
    CFDictionaryRef CV_NULLABLE pixelBufferAttributes,
    CV_RETURNS_RETAINED_PARAMETER CVPixelBufferRef CV_NULLABLE * CV_NONNULL pixelBufferOut) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

typedef void (*CVPixelBufferReleaseBytesCallback)( void * CV_NULLABLE releaseRefCon, const void * CV_NULLABLE baseAddress );

/*!
    @function   CVPixelBufferCreateWithBytes
    @abstract   Call to create a single PixelBuffer for a given size and pixelFormatType based on a passed in piece of memory.
    @discussion Creates a single PixelBuffer for a given size and pixelFormatType. Not all parameters of the pixelBufferAttributes will be used here. It requires a release callback function that will be called, when the PixelBuffer gets destroyed so that the owner of the pixels can free the memory.
    @param      width   Width of the PixelBuffer in pixels
    @param      height  Height of the PixelBuffer in pixels
    @param      pixelFormatType		Pixel format indentified by its respective OSType.
    @param      baseAddress		Address of the memory storing the pixels.
    @param      bytesPerRow		Row bytes of the pixel storage memory.
    @param      releaseCallback         CVPixelBufferReleaseBytePointerCallback function that gets called when the PixelBuffer gets destroyed.
    @param      releaseRefCon           User data identifying the PixelBuffer for the release callback.
    @param      pixelBufferAttributes      A dictionary with additional attributes for a a pixel buffer. This parameter is optional. See PixelBufferAttributes for more details.
    @param      pixelBufferOut          The new pixel buffer will be returned here
    @result	returns kCVReturnSuccess on success.
*/
CV_EXPORT CVReturn CVPixelBufferCreateWithBytes(
    CFAllocatorRef CV_NULLABLE allocator,
    size_t width,
    size_t height,
    OSType pixelFormatType,
    void * CV_NONNULL baseAddress,
    size_t bytesPerRow,
    CVPixelBufferReleaseBytesCallback CV_NULLABLE releaseCallback,
    void * CV_NULLABLE releaseRefCon,
    CFDictionaryRef CV_NULLABLE pixelBufferAttributes,
    CV_RETURNS_RETAINED_PARAMETER CVPixelBufferRef CV_NULLABLE * CV_NONNULL pixelBufferOut) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

typedef void (*CVPixelBufferReleasePlanarBytesCallback)( void * CV_NULLABLE releaseRefCon, const void * CV_NULLABLE dataPtr, size_t dataSize, size_t numberOfPlanes, const void * CV_NULLABLE planeAddresses[CV_NULLABLE ] );

/*!
    @function   CVPixelBufferCreateWithPlanarBytes
    @abstract   Call to create a single PixelBuffer in planar format for a given size and pixelFormatType based on a passed in piece of memory.
    @discussion Creates a single PixelBuffer for a given size and pixelFormatType. Not all parameters of the pixelBufferAttributes will be used here. It requires a release callback function that will be called, when the PixelBuffer gets destroyed so that the owner of the pixels can free the memory.
    @param      width			Width of the PixelBuffer in pixels
    @param      height			Height of the PixelBuffer in pixels
    @param      pixelFormatType		Pixel format indentified by its respective OSType.
    @param	dataPtr			Pass a pointer to a plane descriptor block, or NULL.
    @param	dataSize		pass size if planes are contiguous, NULL if not.
    @param	numberOfPlanes		Number of planes.
    @param	planeBaseAddress	Array of base addresses for the planes.
    @param	planeWidth		Array of plane widths.
    @param	planeHeight		Array of plane heights.
    @param	planeBytesPerRow	Array of plane bytesPerRow values.
    @param	releaseCallback		CVPixelBufferReleaseBytePointerCallback function that gets called when the PixelBuffer gets destroyed.
    @param	releaseRefCon		User data identifying the PixelBuffer for the release callback.
    @param	pixelBufferAttributes      A dictionary with additional attributes for a a pixel buffer. This parameter is optional. See PixelBufferAttributes for more details.
    @param      pixelBufferOut          The new pixel buffer will be returned here
    @result	returns kCVReturnSuccess on success.
*/
CV_EXPORT CVReturn CVPixelBufferCreateWithPlanarBytes(
    CFAllocatorRef CV_NULLABLE allocator,
    size_t width,
    size_t height,
    OSType pixelFormatType,
    void * CV_NULLABLE dataPtr, // pass a pointer to a plane descriptor block, or NULL
    size_t dataSize, // pass size if planes are contiguous, NULL if not
    size_t numberOfPlanes,
    void * CV_NULLABLE planeBaseAddress[CV_NONNULL ],
    size_t planeWidth[CV_NONNULL ],
    size_t planeHeight[CV_NONNULL ],
    size_t planeBytesPerRow[CV_NONNULL ],
    CVPixelBufferReleasePlanarBytesCallback CV_NULLABLE releaseCallback,
    void * CV_NULLABLE releaseRefCon,
    CFDictionaryRef CV_NULLABLE pixelBufferAttributes,
    CV_RETURNS_RETAINED_PARAMETER CVPixelBufferRef CV_NULLABLE * CV_NONNULL pixelBufferOut) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);


/*!
	@function   CVPixelBufferLockBaseAddress
	@abstract   Description Locks the BaseAddress of the PixelBuffer to ensure that the memory is accessible.
	@discussion This API ensures that the CVPixelBuffer is accessible in system memory. This should only be called if the base address is going to be used and the pixel data will be accessed by the CPU.
	@param      pixelBuffer Target PixelBuffer.
	@param      lockFlags See CVPixelBufferLockFlags.
	@result     kCVReturnSuccess if the lock succeeded, or error code on failure
*/
CV_EXPORT CVReturn CVPixelBufferLockBaseAddress( CVPixelBufferRef CV_NONNULL pixelBuffer, CVPixelBufferLockFlags lockFlags ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
	@function   CVPixelBufferUnlockBaseAddress
	@abstract   Description Unlocks the BaseAddress of the PixelBuffer.
	@param      pixelBuffer Target PixelBuffer.
	@param      unlockFlags See CVPixelBufferLockFlags.
	@result     kCVReturnSuccess if the unlock succeeded, or error code on failure
*/
CV_EXPORT CVReturn CVPixelBufferUnlockBaseAddress( CVPixelBufferRef CV_NONNULL pixelBuffer, CVPixelBufferLockFlags unlockFlags ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetWidth
    @abstract   Returns the width of the PixelBuffer.
    @param      pixelBuffer Target PixelBuffer.
    @result     Width in pixels.
*/
CV_EXPORT size_t CVPixelBufferGetWidth( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetHeight
    @abstract   Returns the height of the PixelBuffer.
    @param      pixelBuffer Target PixelBuffer.
    @result     Height in pixels.
*/
CV_EXPORT size_t CVPixelBufferGetHeight( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetPixelFormatType
    @abstract   Returns the PixelFormatType of the PixelBuffer.
    @param      pixelBuffer Target PixelBuffer.
    @result     OSType identifying the pixel format by its type.
*/
CV_EXPORT OSType CVPixelBufferGetPixelFormatType( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetBaseAddress
    @abstract   Returns the base address of the PixelBuffer.
    @discussion Retrieving the base address for a PixelBuffer requires that the buffer base address be locked
                via a successful call to CVPixelBufferLockBaseAddress.
    @param      pixelBuffer Target PixelBuffer.
    @result     Base address of the pixels.
		For chunky buffers, this will return a pointer to the pixel at 0,0 in the buffer
		For planar buffers this will return a pointer to a PlanarComponentInfo struct (defined in QuickTime).
*/
CV_EXPORT void * CV_NULLABLE CVPixelBufferGetBaseAddress( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetBytesPerRow
    @abstract   Returns the rowBytes of the PixelBuffer.
    @param      pixelBuffer Target PixelBuffer.
    @result     Bytes per row of the image data.   For planar buffers this will return a rowBytes value such that bytesPerRow * height
                will cover the entire image including all planes.
*/
CV_EXPORT size_t CVPixelBufferGetBytesPerRow( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetDataSize
    @abstract   Returns the data size for contigous planes of the PixelBuffer.
    @param      pixelBuffer Target PixelBuffer.
    @result     Data size used in CVPixelBufferCreateWithPlanarBytes.
*/
CV_EXPORT size_t CVPixelBufferGetDataSize( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferIsPlanar
    @abstract   Returns if the PixelBuffer is planar.
    @param      pixelBuffer Target PixelBuffer.
    @result     True if the PixelBuffer was created using CVPixelBufferCreateWithPlanarBytes.
*/
CV_EXPORT Boolean CVPixelBufferIsPlanar( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetPlaneCount
    @abstract   Returns number of planes of the PixelBuffer.
    @param      pixelBuffer Target PixelBuffer.
    @result     Number of planes.  Returns 0 for non-planar CVPixelBufferRefs.
*/
CV_EXPORT size_t CVPixelBufferGetPlaneCount( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetWidthOfPlane
    @abstract   Returns the width of the plane at planeIndex in the PixelBuffer.
    @discussion On OSX 10.10 and earlier, or iOS 8 and earlier, calling this 
                function with a non-planar buffer will have undefined behavior.
    @param      pixelBuffer Target PixelBuffer.
    @param      planeIndex  Identifying the plane.
    @result     Width in pixels, or 0 for non-planar CVPixelBufferRefs.
*/
CV_EXPORT size_t CVPixelBufferGetWidthOfPlane( CVPixelBufferRef CV_NONNULL pixelBuffer, size_t planeIndex ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetHeightOfPlane
    @abstract   Returns the height of the plane at planeIndex in the PixelBuffer.
    @discussion On OSX 10.10 and earlier, or iOS 8 and earlier, calling this
                function with a non-planar buffer will have undefined behavior.
    @param      pixelBuffer Target PixelBuffer.
    @param      planeIndex  Identifying the plane.
    @result     Height in pixels, or 0 for non-planar CVPixelBufferRefs.
*/
CV_EXPORT size_t CVPixelBufferGetHeightOfPlane( CVPixelBufferRef CV_NONNULL pixelBuffer, size_t planeIndex ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetBaseAddressOfPlane
    @abstract   Returns the base address of the plane at planeIndex in the PixelBuffer.
    @discussion Retrieving the base address for a PixelBuffer requires that the buffer base address be locked
                via a successful call to CVPixelBufferLockBaseAddress. On OSX 10.10 and earlier, or iOS 8 and
                earlier, calling this function with a non-planar buffer will have undefined behavior.
    @param      pixelBuffer Target PixelBuffer.
    @param      planeIndex  Identifying the plane.
    @result     Base address of the plane, or NULL for non-planar CVPixelBufferRefs.
*/
CV_EXPORT void * CV_NULLABLE CVPixelBufferGetBaseAddressOfPlane( CVPixelBufferRef CV_NONNULL pixelBuffer, size_t planeIndex ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetBytesPerRowOfPlane
    @abstract   Returns the row bytes of the plane at planeIndex in the PixelBuffer.
    @discussion On OSX 10.10 and earlier, or iOS 8 and earlier, calling this
                function with a non-planar buffer will have undefined behavior.
    @param      pixelBuffer Target PixelBuffer.
    @param      planeIndex  Identifying the plane.
    @result     Row bytes of the plane, or NULL for non-planar CVPixelBufferRefs.
*/
CV_EXPORT size_t CVPixelBufferGetBytesPerRowOfPlane( CVPixelBufferRef CV_NONNULL pixelBuffer, size_t planeIndex ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferGetExtendedPixels
    @abstract   Returns the size of extended pixels of the PixelBuffer.
    @discussion On OSX 10.10 and earlier, or iOS 8 and earlier, calling this
                function with a non-planar buffer will have undefined behavior.
    @param      pixelBuffer Target PixelBuffer.
    @param      extraColumnsOnLeft Returns the pixel row padding to the left.  May be NULL.
    @param      extraRowsOnTop Returns the pixel row padding to the top.  May be NULL. 
    @param      extraColumnsOnRight Returns the pixel row padding to the right. May be NULL.
    @param      extraRowsOnBottom Returns the pixel row padding to the bottom. May be NULL.
*/
CV_EXPORT void CVPixelBufferGetExtendedPixels(
						  CVPixelBufferRef CV_NONNULL pixelBuffer,
						  size_t * CV_NULLABLE extraColumnsOnLeft,
						  size_t * CV_NULLABLE extraColumnsOnRight,
						  size_t * CV_NULLABLE extraRowsOnTop,
						  size_t * CV_NULLABLE extraRowsOnBottom ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferFillExtendedPixels
    @abstract   Fills the extended pixels of the PixelBuffer.   This function replicates edge pixels to fill the entire extended region of the image.
    @param      pixelBuffer Target PixelBuffer.
*/
CV_EXPORT CVReturn CVPixelBufferFillExtendedPixels( CVPixelBufferRef CV_NONNULL pixelBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferCopyCreationAttributes
    @abstract   Returns a copy of pixelBufferAttributes dictionary used to create the PixelBuffer.
    @discussion Can be used to create similar pixelbuffers.
    @param      pixelBuffer Target PixelBuffer.
*/
CV_EXPORT CFDictionaryRef CV_NONNULL CVPixelBufferCopyCreationAttributes( CVPixelBufferRef CV_NONNULL pixelBuffer ) CV_RETURNS_RETAINED API_AVAILABLE(macosx(12.0), ios(15.0), tvos(15.0), watchos(8.0));

#if defined(__cplusplus)
}
#endif

#if !0
#if COREVIDEO_SUPPORTS_IOSURFACE
#if __has_include(<CoreVideo/CVPixelBufferIOSurface.h>)
#include <CoreVideo/CVPixelBufferIOSurface.h>
#endif
#endif
#endif

#endif
