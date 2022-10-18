///
///  @header AppleEXR.h
///  @library  /usr/lib/libAppleEXR.dylib
///  @dependency -stdlib=libc++ -lobjc
///  @copyright Copyright © 2019-2020 Apple Inc. All rights reserved.
///  @discussion Apple OpenEXR decoder / encoder
///
///     Created by Ian Ollmann on 8/19/19.
///

/*  ************************************************
 *  ***  SAVE YOURSELF SOME TIME -- Please Read  ***
 *  ************************************************
 *
 *  This is the low level API to a software OpenEXR encoder / decoder,
 *  intended to support the subset of operations needed by ImageIO in a
 *  lightweight, secure and performant manner. The same code is also usable
 *  through the higher level ImageIO.framework CGImageSource / CGImageDestination
 *  APIs and more generally through CGImageRef and associated AppKit / UIKit
 *  NSImage/UIImage interfaces. In most cases, those interfaces will prove
 *  simpler to use. This low level API is provided for those applications that
 *  need more direct access to the unique feature set of the OpenEXR file format
 *  that might otherwise be obscured by more general APIs that must service
 *  numerous other compressed image formats. If there is sufficient in interest,
 *  the feature set may be expanded in the future.
 *
 *  While this library attempts to be file format compatible with the ILM / Academy
 *  Software Foundation OpenEXR implementation (www.openexr.com), it is not API
 *  compatible with that work. Software written to use those interfaces will need to
 *  be modified in order to use these interfaces instead. No effort has been made to
 *  ensure that all operations in other OpenEXR implementations have a counterpart here.
 *
 *  Except for the _rgba interfaces, AppleEXR does not provide pixel type conversion services.
 *  Image data is provided as stored (after decompression) at the same precision found in the
 *  file. Most OpenEXR content is HDR linear gamma and care must be exercised when converting
 *  to other formats, especially traditional unorm based formats. Please see
 *  vImageConvert_AnyToAny() or MPSImageConversion.
 *
 *  Feature requests and bug reports may be made through to the ImageIO Radar component.
 *  See https://feedbackassistant.apple.com.
 *
 *  Overview of interfaces:
 *    Library versioning:
 *      axr_get_version                     major.minor.bugfix
 *
 *    Informational / Utility:
 *      axr_channel_type_get_name           const char[] representation of enum axr_channel_type_t
 *      axr_compression_get_name            const char[] representation of enum axr_compression_t
 *      axr_error_get_name                  const char[] representation of enum axr_error_t
 *      axr_flags_get_name                  const char[] representation of enum axr_flags_t
 *      axr_line_order_get_name             const char[] representation of enum axr_line_order_t
 *      axr_property_flags_get_name         const char[] representation of enum axr_property_flags_t
 *      axr_type_get_name                   const char[] representation of enum axr_type_t
 *      axr_type_get_size                   sizeof type named by axr_type_t in bytes
 *
 *    File Data container creation / metadata
 *      axr_introspect_data                 Quick check to see if file is EXR
 *      axr_data_create                     The axr_data_t is a container that holds a EXR file loaded into memory
 *      axr_data_get_channel_count          Report how many channels are in a part
 *      axr_data_get_channel_info           Report metadata about a channel
 *      axr_data_get_layer_count            Number of groups of channels with the same name prefix
 *      axr_data_get_layer_info             Metadata about a group of channels with the same name prefix
 *      axr_data_get_level_count            Number of mip- or ripmap levels for the image
 *      axr_data_get_level_size             Size in pixels of a layer level
 *      axr_data_get_part_count             Report how many parts are in a v2 OpenEXR file. Otherwise 1.
 *      axr_data_get_part_info              Report metadata for a part
 *      axr_data_get_property_count         Number of raw metadata items in a part
 *      axr_data_get_property               Raw meta data access for a part. Provides access to all attributes.
 *
 *    Decoding
 *      axr_decoder_create                  create a new decoder for a axr_data_t.
 *      axr_decoder_create_rgba             Convenience method to create a decoder for RGBA output.
 *                                              See also: axr_decoder_read_rgba_pixels, axr_decoder_create_rgba_colorspace.
 *      axr_decoder_copy                    create a copy of a axr_data_t
 *      axr_decoder_append_channel          Add a new channel to the list to be decoded
 *      axr_decoder_append_constant_value_channel   Add a new channel to the output. The channel doesn't occur in the EXR file. A constant value (e.g. 1.0) will be used.
 *      axr_decoder_clear_channel_list      Delete all of the channels in the list of channels to be decoded
 *      axr_decoder_create_colorspace       Create a CGColorSpaceRef to represent the colorspace of the decoded channels
 *      axr_decoder_create_rgba_colorspace  Like axr_decoder_create_colorspace, but intended for use with axr_decoder_read_rgba_pixels
 *      axr_decoder_get_channel_count       Get the number of channels to be decoded. (See also axr_decoder_append_channel)
 *      axr_decoder_get_channel_info        Get metadata about the decoded channel
 *      axr_decoder_get_data                Get the axr_data_t that contains the data for the decoder
 *      axr_decoder_get_info                Get the current decoder settings
 *      axr_decoder_translate_channel_index Get the axr_data_t part index for the channels to be decoded
 *      axr_decoder_set_subregion           Set the X,Y spatial subregion of the image to be decoded
 *      axr_decoder_read_pixels             Read the pixel data as either 1, 2 or 4 interleaved channels
 *      axr_decoder_read_rgba_pixels        Read the pixel data as 4 channel RGBA content, even if the channels are grayscale, XYZ, or YCbCr (YCC, YUV)
 *      axr_decoder_read_planes             Read the pixel data as separate planes
 *
 *   Encoding
 *      axr_encoder_append_part             Add a part to a axr_encoder_t
 *      axr_encoder_compress                Compress the parts encapsulated by axr_encoder_t to serial file
 *      axr_encoder_create                  Create a axr_encoder_t object. This can be used to compress a OpenEXR image
 *      axr_part_append_attribute           Add an attribute to a part
 *      axr_part_append_channels            add 1,2 or 4 channels to a part
 *      axr_part_create                     create a new part
 *      axr_part_set_chromaticities         Configure the color space of the part
 *      axr_part_set_preview                Add a flat RGBA unorm8 preview image to the part
 *      axr_part_create_colorspace          Get a CGColorSpaceRef for the part.  Useful for converting pixels to values suitable for encoding with OpenEXR
 *
 *
 *  Conceptual organization of files:
 *      The hierarchy of subdivision of a EXR file, is as follows:  EXR file > part > layer > channel > level
 *      At each level, there is a many to one relationship with the level before it. Each file may have many parts. Each
 *      part may have many layers. Each layer may have many channels, etc. A rough description of what each is follows:
 *
 *          EXR file:   A serialized file container that starts with little endian int32_t 20000630
 *          part:       A set of layers with shared metadata like a unified set of coordinates, color space,
 *                      and time stamp. Only channels in the same part can be decoded together.
 *                      Since generally all channels in a part must be at least partially decoded to read
 *                      any channel in the part, the part serves to provide some file segmentation
 *                      to reduce the amount of unnecessary work in files that contain a lot of data. Generally
 *                      only channels read together should appear in the same part. Except where adjusted by
 *                      a x- or y- downsampling rate (e.g. YCbCr 420), all channels in a part are the same height and width.
 *          layer:      A set of channels with the same name prefix, e.g. "camera1.R", "camera1.G", "camera1.B".  These
 *                      typically belong to the same conceptual data set, such as a RGB image shot from a particular
 *                      position in space. Do not mix unrelated data into the same layer.
 *          channel:    A sequence of float, uint32_t or half precision floating point values that encode a single
 *                      data channel in a file such as the alpha information for a layer. Channels may be anything,
 *                      and may include color channels, alpha channels, zbuffer channels, raw data, etc.
 *          level:      A channel may be available in a series of variants that have been down sampled by
 *                      a factor of a power of two. The full size image is available as level 0.  The downsampled
 *                      variants grow progressively smaller until the image is 1x1 in size.  Each such level is a rectangular
 *                      data plane belonging to a single channel.
 *
 *      Channels that have the same part and mip-/ripmap level can be read together as a interleaved (chunky) image,
 *      provided that their x/y downsampling rate is also the same. This is more efficient than reading data planes one at a time.
 *
 *  C++ sample code for quickly obtaining RGBA images with AppleEXR:
 *
 *      void * fileData = ...;  // pointer to contents of an EXR file in memory
 *      size_t fileSize = ...;  // number of bytes in the file
 *      axr_error_t axrErr = axr_error_success;
 *
 *      // create a axr_data_t to represent the file. See also axr_introspect_data()
 *      axr_data_t axrData = axr_data_create( fileData, fileSize, &axrErr); 
 *      if( NULL == axrData )
 *          return axrErr;
 *
 *      // Make a decoder for the part of the file you want to read
 *      //    default parameters will read the default layer from the first part
 *      axr_decoder_t d = axr_decoder_create_rgba(axrData);
 *      if( NULL == d )
 *      {
 *          os_release(axrData);
 *          return axr_error_insufficient_memory;
 *      }
 *
 *      // Figure out how big to make the buffer to receive the pixels
 *      axr_type_t sampleType = axr_decoder_get_channel_info(d, 0).sampleType;
 *      uint32_t channelSize = (uint32_t) axr_type_get_size(sampleType);
 *      uint32_t pixelSize = 4 * channelSize;       // RGBA = 4 channels
 *      axr_decoder_info_t info = axr_decoder_get_info(d);
 *      size_t rowBytes = info.subregion.size.width * pixelSize;
 *      void * buf = malloc( info.subregion.size.height * rowBytes );
 *             // ...handle malloc failure ...
 *
 *      // read the desired content from the file into buf as RGBA pixels
 *      if( axr_error_success == (axrErr = axr_decoder_read_rgba_pixels( d, buf, rowBytes )))
 *      {
 *          // Do something with the pixels here
 *      }
 *
 *      // Clean up. Note that if Automatic Reference Counting (ARC) is on,
 *      // this part isn't necessary
 *      os_release(d);
 *      os_release(axrData);
 *
 *      free(buf);      // ARC doesn't clean this up
 *
 *      return axrErr;
 */

#ifndef AppleEXR_h
#define AppleEXR_h  1

#include <os/object.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <Availability.h>

/* As a C++ convenience, some argments can have default values. */
#if defined __cplusplus && ! defined(DOXYGEN)
    extern "C" {
#   define _AXR_DEFAULT(_a)            = _a
#   ifndef _AXR_DEFAULT_FLAGS
#       if DEBUG
#          define _AXR_DEFAULT_FLAGS   = axr_flags_print_debug_info
#       else
#          define _AXR_DEFAULT_FLAGS   = axr_flags_default
#       endif
#   endif
#else
#   define _AXR_DEFAULT(_a)
#   ifndef _AXR_DEFAULT_FLAGS
#       define _AXR_DEFAULT_FLAGS
#   endif
#endif
    
#ifndef AXR_RETURNS_RETAINED
#   define AXR_RETURNS_RETAINED         OS_OBJECT_RETURNS_RETAINED
#   define AXR_RETURNS_INNER_POINTER   
#endif


/*  Availability information for AppleEXR (Apple OpenEXR)
 *  version 1:
 *      Initial feature set.
 *         Decodes OpenEXR 1.7 and 2.0 images
 *      Unsupported features:
 *         Deep Tile and Deep scanline images
 *         features introduced after file format 2.0
 */

#ifdef AXR_SKIP_AVAILABILITY
#   define AXR_AVAILABILITY_v1
#   define AXR_ENUM_AVAILABILITY_v1
#else
#   define AXR_AVAILABILITY_v1          __API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0), watchos(7.0))
#   define AXR_ENUM_AVAILABILITY_v1     __API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0), watchos(7.0))
#endif

    
/*! @abstract   Get AppleEXR dylib.current_version
 *  @return     A uint32_t encoding library version as major.minor.bugfix  (16.8.8)
 *              The major field of the build version corresponds with the compatibility
 *              version (e.g. AXR_AVAILABILITY_v1) */
uint32_t  axr_get_version(void)  AXR_AVAILABILITY_v1;

    
#pragma mark constants

/*! @enum       axr_error_t
 *  @abstract   Error codes for axr_data_t
 *  @constant   axr_error_success                             The operation completed successfully
 *  @constant   axr_error_invalid_parameter              One of the parameters was incorrect.
 *                                             Try axr_flags_print_debug_info for more information
 *  @constant   axr_error_unknown_file_type             This is not a recognized OpenEXR file variant
 *  @constant   axr_error_unsupported_EXR_type     This OpenEXR file variant is not supported
 *  @constant   axr_error_data_truncated                   The file is smaller than it should be.
 *  @constant   axr_error_insufficient_memory           Malloc failure
 *  @constant   axr_error_data_corrupted                   Data within the file is internally inconsistent, or illegal values found
 *  @constant   axr_error_null_data                             The axr_data_t parameter may not be NULL
 *  @constant   axr_error_compression_method_not_supported  This version of AppleEXR does not support the compression options  provided or found within the file
 *  @constant   axr_error_reserved_attribute_name    The attribute is already saved as a standard part of saving the axr_part_t. It can not be added again.
 *  @constant   axr_error_encoding_suspended         Your application's axr_data_provider_read_t returned 0.
 *  @constant   axr_error_encoding_insufficient_data Your application's axr_data_provider_read_t returned -1.
 *  @constant   axr_error_encoding_read_error           Your application's axr_data_provider_read_t returned -2.
 *  @constant   axr_error_encoding_write_error          The internal compressor failed.  Pass axr_flags_print_debug_info to see more.  This shouldn't happen. File a Radar.
 *
 *  @memberof   axr_data_t
 */
#if defined(DOXYGEN)
typedef enum axr_error_t
{
    axr_error_success                       AXR_ENUM_AVAILABILITY_v1  =     0,
    
    /* All failures are negative */
    axr_error_invalid_parameter             AXR_ENUM_AVAILABILITY_v1  =    -1,
    axr_error_unknown_file_type             AXR_ENUM_AVAILABILITY_v1  =    -2,
    axr_error_unsupported_EXR_type          AXR_ENUM_AVAILABILITY_v1  =    -3,
    axr_error_data_truncated                AXR_ENUM_AVAILABILITY_v1  =    -4,
    axr_error_insufficient_memory           AXR_ENUM_AVAILABILITY_v1  =    -5,
    axr_error_data_corrupted                AXR_ENUM_AVAILABILITY_v1  =    -6,
    axr_error_null_data                     AXR_ENUM_AVAILABILITY_v1  =    -7,
    axr_error_compression_method_not_supported AXR_ENUM_AVAILABILITY_v1  = -8,
    axr_error_reserved_attribute_name       AXR_ENUM_AVAILABILITY_v1  =    -9,
    axr_error_encoding_suspended            AXR_ENUM_AVAILABILITY_v1  =   -10,
    axr_error_encoding_insufficient_data    AXR_ENUM_AVAILABILITY_v1  =   -11,
    axr_error_encoding_read_error           AXR_ENUM_AVAILABILITY_v1  =   -12,
    axr_error_encoding_write_error          AXR_ENUM_AVAILABILITY_v1  =   -13,

    /* other error codes are reserved */
};
#else
OS_ENUM( axr_error, long,
    axr_error_success                       AXR_ENUM_AVAILABILITY_v1  =     0,
    
    /* All failures are negative */
    axr_error_invalid_parameter             AXR_ENUM_AVAILABILITY_v1  =    -1,
    axr_error_unknown_file_type             AXR_ENUM_AVAILABILITY_v1  =    -2,
    axr_error_unsupported_EXR_type          AXR_ENUM_AVAILABILITY_v1  =    -3,
    axr_error_data_truncated                AXR_ENUM_AVAILABILITY_v1  =    -4,
    axr_error_insufficient_memory           AXR_ENUM_AVAILABILITY_v1  =    -5,
    axr_error_data_corrupted                AXR_ENUM_AVAILABILITY_v1  =    -6,
    axr_error_null_data                     AXR_ENUM_AVAILABILITY_v1  =    -7,
    axr_error_compression_method_not_supported AXR_ENUM_AVAILABILITY_v1  = -8,
    axr_error_reserved_attribute_name       AXR_ENUM_AVAILABILITY_v1  =    -9,
    axr_error_encoding_suspended            AXR_ENUM_AVAILABILITY_v1  =   -10,
    axr_error_encoding_insufficient_data    AXR_ENUM_AVAILABILITY_v1  =   -11,
    axr_error_encoding_read_error           AXR_ENUM_AVAILABILITY_v1  =   -12,
    axr_error_encoding_write_error          AXR_ENUM_AVAILABILITY_v1  =   -13,

    /* other error codes are reserved */
);
#endif /* DOXYGEN */
    
/*! @function axr_error_get_name
 *  @abstract Get a human readable string for a axr_error_t
 *  @memberof axr_error_t */
const char * __nonnull axr_error_get_name( axr_error_t error ) AXR_AVAILABILITY_v1;

/*! @enum       axr_flags_t
 *  @discussion These are flags to control aspects of operation of various AppleEXR interfaces. If multiple flags are needed, bitwise-OR them together.
 *
 *              Skip Parameter Checking
 *                  Some methods may do extensive parameter checking. If this is a performance problem, you may try axr_flags_skip_parameter_checking
 *                  to disable this activity.  Note that as a consequence, the library may not catch or report usage that leads to undefined behavior and
 *                  undefined behavior will be the result in such cases.
 *
 *              Print Debug Info:
 *                  if something goes wrong, or there is something that AppleEXR wants to nag you about, human readable text will be
 *                  sent to the Apple System Logger to help explain the problem when axr_flags_print_debug_info is on. These messages
 *                  should show up on your debugger console. See also behavior of _AXR_DEFAULT_FLAGS for C++ automatic parameters.
 *                  Defining AXR_PRINT_DEBUG_INFO in the environment is another way to turn this on.
 *
 *              Disable multithreading:
 *                 By default, expensive routines such as axr_decoder_read_pixels and axr_decoder_read_planes are likely to
 *                 use separate threads to read tiles from the EXR image so as to better take advantage of multiple processors on
 *                 your system.  If your application is reading multiple EXR files in parallel or otherwise is likely to be executing
 *                 multithreaded workloads at the time, it may not be advantageous for AppleEXR to spawn yet more GCD tasks to
 *                 do its work, since the machine is already saturated. It would just add unnecessary synchronization overhead and
 *                 pollute more of the caches.  In such cases, you may pass axr_flags_disable_multithreading to force serial operation
 *                 on the calling thread.
 *
 *              Disable automatic line order handling:
 *                 OpenEXR -- "When a scan-line based image is written, application code must set the lineOrder attribute in the header to INCREASING_Y
 *                           order (top scan line first) or DECREASING_Y order (bottom scan line first). Application code must then write the scan lines
 *                           in the specified order"   https://www.openexr.com/documentation/TechnicalIntroduction.pdf
 *
 *                 We interpret this to mean that if DECREASING_Y line order is set, you are responsible for vertically flipping the image yourself when reading
 *                 or writing the image when using ILM OpenEXR. AppleEXR, however, will by default attempt to automatically flip the image for you when it
 *                 detects axr_line_order_decreasing_y.    While better for users, this approach might be undesired if we misunderstand the situation, encounter
 *                 malformed images, or your application, having recently been ported from ILM OpenEXR, is not expecting this behavior.  Your application
 *                 may use axr_flags_disable_automatic_line_order_handling to cause the scanlines to be read or written in the order they appear in the file.
 *                 AppleEXR will still attempt to reorder tiles in the case that the order is axr_line_order_random_y.
 *
 *                 In the event your application needs to reverse the order of scanlines itself, this is most easily done by changing the pointer to your pixel buffer to point
 *                 to the bottom left corner of the region of interest that you wish to read/write, and negate the rowBytes parameter.  For example, when working
 *                 with axr_decoder_read_planes, you can adjust each plane as follows:
 *
 *                  // defeat C operator+ increment by sizeof(ptr[0]).  Increment by byte instead.  Defeat asan assertion for pointer overflow.
 *                  #define  ADVANCE_PTR( _ptr, _bytes )   ((__typeof__(_ptr)) (((uintptr_t) (_ptr)) + (_bytes)) )
 *
 *                  if( axr_part_info_t.lineOrder == axr_line_order_decreasing_y)
 *                  {
 *                      // move data to point to bottom left of image instead
 *                      axr_pixel_data_t.data = ADVANCE_PTR( axr_pixel_data_t.data,  (roiHeight - 1) * axr_pixel_data_t.rowBytes);      // roiHeight: see axr_decoder_set_subregion(... ySize). For entire image, see axr_data_get_level_size().height.
 *
 *                      //walk bottom to top through image
 *                      axr_pixel_data_t.rowBytes = -axr_pixel_data_t.rowBytes
 *                  }
 *
 *
 *  @abstract   Flags to influence the operation of Apple EXR
 *  @constant   axr_flags_default                                                       Default operation
 *  @constant   axr_flags_skip_parameter_checking                         Assume parameters are sane and proceed blindly forward without parameter error checking.
 *  @constant   axr_flags_print_debug_info                                       Print additional debug info to stderr in case of error
 *  @constant   axr_flags_disable_multithreading                              Run entirely in the calling thread. May be multithreaded otherwise.
 *  @constant   axr_flags_disable_automatic_line_order_handling   Turn off automatic image flipping for axr_line_order_decreasing_y cases  (See discussion)
 *
 *  @memberof   axr_data_t
 */
#if defined(DOXYGEN)
typedef enum axr_flags_t
{
    axr_flags_default                       AXR_ENUM_AVAILABILITY_v1  =   0,
    axr_flags_skip_parameter_checking       AXR_ENUM_AVAILABILITY_v1  =   1U << 0,          //*<  Do not spend time checking input parameters */
    axr_flags_print_debug_info              AXR_ENUM_AVAILABILITY_v1  =   1U << 1,          //*<  Print additional debug info to stderr */
    axr_flags_disable_multithreading        AXR_ENUM_AVAILABILITY_v1  =   1U << 2,          //*<  Run entirely in this thread */
    axr_flags_disable_automatic_line_order_handling AXR_ENUM_AVAILABILITY_v1  =   1U << 3,  //*<  Turn off image flipping for axr_line_order_decreasing_y */

    /* other bits are reserved */
};
#else
OS_ENUM( axr_flags, uint64_t,
    axr_flags_default                               AXR_ENUM_AVAILABILITY_v1  =   0,
    axr_flags_skip_parameter_checking               AXR_ENUM_AVAILABILITY_v1  =   1U << 0,    //*<  Do not spend time checking input parameters */
    axr_flags_print_debug_info                      AXR_ENUM_AVAILABILITY_v1  =   1U << 1,    //*<  Print additional debug info to stderr */
    axr_flags_disable_multithreading                AXR_ENUM_AVAILABILITY_v1  =   1U << 2,    //*<  Run entirely in this thread */
    axr_flags_disable_automatic_line_order_handling AXR_ENUM_AVAILABILITY_v1  =   1U << 3,    //*<  Turn off image flipping for axr_line_order_decreasing_y */

    /* other bits are reserved */
);
#endif
    
/*! @function   axr_flags_get_name
 *  @abstract   Get a user readable string that describes the contents of  a flags bitfield
 *  @param      buf     The memory location to write the string
 *  @param      len     The size of the memory location
 *  @param      flags   The flags value to print
 *  @return     The number of bytes consumed in buf
 *  @memberof  axr_flags_t*/
size_t axr_flags_get_name( char * __nonnull buf,
                           size_t len,
                           axr_flags_t flags) AXR_AVAILABILITY_v1;


/*! @enum       axr_type_t
 *  @abstract   The data type of each individual sample, e.g. half, float or uint32_t
 *  @constant   axr_type_uint  uint32_t
 *  @constant   axr_type_half  IEEE-754-2008 16-bit floating-point  (1 bit sign, 5 bit exponent (bias 15), 10 bit significand)
 *  @constant   axr_type_float  IEEE-754 32-bit floating-point (e.g. "float" in C)
 *
 *  @memberof   axr_data_t
 */
#if defined(DOXYGEN)
typedef enum axr_type_t
{
    axr_type_uint                    AXR_ENUM_AVAILABILITY_v1  =   0,          //*<  uint32_t   */
    axr_type_half                    AXR_ENUM_AVAILABILITY_v1  =   1,          //*<  __fp16     */
    axr_type_float                   AXR_ENUM_AVAILABILITY_v1  =   2,          //*<  float      */
    
    /* other values are reserved */
};
#else
OS_ENUM( axr_type, uint32_t,
    axr_type_uint                    AXR_ENUM_AVAILABILITY_v1  =   0,          //*<  uint32_t   */
    axr_type_half                    AXR_ENUM_AVAILABILITY_v1  =   1,          //*<  __fp16     */
    axr_type_float                   AXR_ENUM_AVAILABILITY_v1  =   2,          //*<  float      */

    /* other bits are reserved */
);
#endif

/*! @function axr_type_get_name
    @abstract Return a human readable name for the type
    @memberof axr_type_t */
const char * __nonnull axr_type_get_name( axr_type_t type ) AXR_AVAILABILITY_v1;


/*! @enum       axr_level_mode_t
 *  @abstract   Indicates what sort of lower resolution versions are available for anisotropic filtering
 *  @constant   axr_level_mode_one_level      A simple flat image
 *  @constant   axr_level_mode_mipmap        Images are stored as a series of power-of-two downsampled images
 *  @constant   axr_level_mode_ripmap          Images are stored as a series of power-of-two downsampled images, with x and y downsampling rates varying independently
 *  @constant   axr_level_mode_round_down When downsampling, round the size of odd dimensions down
 *  @constant   axr_level_mode_round_up      When downsampling, round the size of odd dimensions up
 *
 *  @memberof   axr_data_t
 */
#if defined(DOXYGEN)
typedef enum axr_level_mode_t
{
    axr_level_mode_one_level         AXR_ENUM_AVAILABILITY_v1  =   0U,         //*<  flat image */
    axr_level_mode_mipmap            AXR_ENUM_AVAILABILITY_v1  =   1U,         //*<  mipmap */
    axr_level_mode_ripmap            AXR_ENUM_AVAILABILITY_v1  =   2U,         //*<  ripmap */
    axr_level_mode_format_mask       AXR_ENUM_AVAILABILITY_v1  = 0xfU,         //*<  bits encoding for level mode */

    axr_level_mode_round_down        AXR_ENUM_AVAILABILITY_v1  =   0U << 4,    //*<  round odd dimensions down when downsampling */
    axr_level_mode_round_up          AXR_ENUM_AVAILABILITY_v1  =   1U << 4,    //*<  round odd dimensions up when downsampling */
    axr_level_mode_round_mask        AXR_ENUM_AVAILABILITY_v1  = 0xfU << 4,    //*<  bits encoding for level rounding mode */
    
    /* other values are reserved */
};
#else
OS_ENUM( axr_level_mode, uint32_t,
    axr_level_mode_one_level         AXR_ENUM_AVAILABILITY_v1  =   0U,         //*<  flat image */
    axr_level_mode_mipmap            AXR_ENUM_AVAILABILITY_v1  =   1U,         //*<  mipmap */
    axr_level_mode_ripmap            AXR_ENUM_AVAILABILITY_v1  =   2U,         //*<  ripmap */
    axr_level_mode_format_mask       AXR_ENUM_AVAILABILITY_v1  = 0xfU,         //*<  bits encoding for level mode */

    axr_level_mode_round_down        AXR_ENUM_AVAILABILITY_v1  =   0U << 4,    //*<  round odd dimensions down when downsampling */
    axr_level_mode_round_up          AXR_ENUM_AVAILABILITY_v1  =   1U << 4,    //*<  round odd dimensions up when downsampling */
    axr_level_mode_round_mask        AXR_ENUM_AVAILABILITY_v1  = 0xfU << 4,    //*<  bits encoding for level rounding mode */

    /* other bits are reserved */
);
#endif

/*! @function axr_level_mode_get_name
    @abstract Return a human readable name for the axr_level_mode
    @memberof axr_level_mode_t */
const char * __nonnull axr_level_mode_get_name( axr_level_mode_t mode ) AXR_AVAILABILITY_v1;


/*! @abstract Return the number of bytes needed to store the axr_type_t */
size_t axr_type_get_size( axr_type_t )  AXR_AVAILABILITY_v1;

/*! @enum       axr_channel_type_t
 *  @abstract   A color channel classification,  e.g.  red, green or blue
 *  @constant   axr_channel_type_unknown      unknown channel type
 *  @constant   axr_channel_type_red      red
 *  @constant   axr_channel_type_green  green
 *  @constant   axr_channel_type_blue    blue
 *  @constant   axr_channel_type_alpha        a shared alpha channel
 *  @constant   axr_channel_type_alpha_red an alpha channel specific to the red channel
 *  @constant   axr_channel_type_alpha_green an alpha channel specific to the green channel
 *  @constant   axr_channel_type_alpha_blue an alpha channel specific to the blue channel
 *  @constant   axr_channel_type_alpha_cb  The blue correction channel in YCbCr
 *  @constant   axr_channel_type_alpha_cr  The red correction channel in YCbCr
 *  @constant   axr_channel_type_alpha_y    The luminance correction channel in YCbCr, grayscale or Y in XYZ
 *  @constant   axr_channel_type_alpha_x    X in XYZ
 *  @constant   axr_channel_type_alpha_z    Z in XYZ
 *  @constant   axr_channel_type_distance  Distance of the front of a sample from the viewer.
 *  @constant   axr_channel_type_distance_back    Distance of the back of a sample from the viewer.
 *  @constant   axr_channel_type_id                      a numerical identifier for an object
 *  @memberof   axr_data_t
 */
#if defined(DOXYGEN)
typedef enum axr_channel_type_t
{
    axr_channel_type_unknown                AXR_ENUM_AVAILABILITY_v1  =   0,
    axr_channel_type_red                    AXR_ENUM_AVAILABILITY_v1,
    axr_channel_type_green                  AXR_ENUM_AVAILABILITY_v1,
    axr_channel_type_blue                   AXR_ENUM_AVAILABILITY_v1,
    
    axr_channel_type_alpha                  AXR_ENUM_AVAILABILITY_v1,      /// a shared alpha channel
    axr_channel_type_alpha_red              AXR_ENUM_AVAILABILITY_v1,      /// a alpha channel specific to the red channel
    axr_channel_type_alpha_green            AXR_ENUM_AVAILABILITY_v1,      /// a alpha channel specific to the green channel
    axr_channel_type_alpha_blue             AXR_ENUM_AVAILABILITY_v1,      /// a alpha channel specific to the blue channel

    axr_channel_type_cb                     AXR_ENUM_AVAILABILITY_v1,      /// YCbCr blue correction
    axr_channel_type_cr                     AXR_ENUM_AVAILABILITY_v1,      /// YCbCr red correction
    axr_channel_type_y                      AXR_ENUM_AVAILABILITY_v1,      /// Grayscale or luminance 
    axr_channel_type_x                      AXR_ENUM_AVAILABILITY_v1,      /// XYZ X channel
    axr_channel_type_z                      AXR_ENUM_AVAILABILITY_v1,      /// XYZ Z channel
    
    axr_channel_type_distance               AXR_ENUM_AVAILABILITY_v1,      /// Distance of the front of a sample from the viewer.
    axr_channel_type_distance_back          AXR_ENUM_AVAILABILITY_v1,      /// Distance of the back of a sample from the viewer.
    axr_channel_type_id                     AXR_ENUM_AVAILABILITY_v1,      /// a numerical identifier for an object

    /* other values are reserved */
};
#else
OS_ENUM( axr_channel_type, uint32_t,
    axr_channel_type_unknown                AXR_ENUM_AVAILABILITY_v1  =   0,
    axr_channel_type_red                    AXR_ENUM_AVAILABILITY_v1,
    axr_channel_type_green                  AXR_ENUM_AVAILABILITY_v1,
    axr_channel_type_blue                   AXR_ENUM_AVAILABILITY_v1,
    
    axr_channel_type_alpha                  AXR_ENUM_AVAILABILITY_v1,      /// a shared alpha channel
    axr_channel_type_alpha_red              AXR_ENUM_AVAILABILITY_v1,      /// a alpha channel specific to the red channel
    axr_channel_type_alpha_green            AXR_ENUM_AVAILABILITY_v1,      /// a alpha channel specific to the green channel
    axr_channel_type_alpha_blue             AXR_ENUM_AVAILABILITY_v1,      /// a alpha channel specific to the blue channel

    axr_channel_type_cr                     AXR_ENUM_AVAILABILITY_v1,      /// YCbCr red correction
    axr_channel_type_cb                     AXR_ENUM_AVAILABILITY_v1,      /// YCbCr blue correction
    axr_channel_type_y                      AXR_ENUM_AVAILABILITY_v1,      /// Grayscale, luminance or Y in XYZ
    axr_channel_type_x                      AXR_ENUM_AVAILABILITY_v1,      /// XYZ X channel
    axr_channel_type_z                      AXR_ENUM_AVAILABILITY_v1,      /// XYZ Z channel
    
    axr_channel_type_distance               AXR_ENUM_AVAILABILITY_v1,      /// Distance of the front of a sample from the viewer.
    axr_channel_type_distance_back          AXR_ENUM_AVAILABILITY_v1,      /// Distance of the back of a sample from the viewer.
    axr_channel_type_id                     AXR_ENUM_AVAILABILITY_v1,      /// a numerical identifier for an object

    /* other values are reserved */
);
#endif
    
/*! @abstract Return a human readable string for a axr_channel_type_t value*/
const char * __nonnull axr_channel_type_get_name( axr_channel_type_t type ) AXR_AVAILABILITY_v1;
    
    
/*! @enum       axr_compression_t
 *  @abstract   Different compression methods used by EXR
 *  @constant   axr_compression_none   uncompressed
 *  @constant   axr_compression_rle       split into even and odd bytes and run length encoded. (l)ossless)
 *  @constant   axr_compression_zips    split into even and odd bytes and compressed with zlib  (lossless)
 *  @constant   axr_compression_zip      split into even and odd bytes and compressed with zlib in larger tiles (lossless)
 *  @constant   axr_compression_piz      wavelet transform, ordinal transform, Huffman encoded (lossy?)
 *  @constant   axr_compression_pxr24  each channel byte split into a separate stream and compressed with zlib. last 8 LSBs of float32s rounded off. lossless for fp16 and uint32. lossy for fp32
 *  @constant   axr_compression_b44     each 4x4 block is stored as difference vs. top left pixel with some truncation  (lossy) Excellent for random access into image subregions
 *  @constant   axr_compression_b44a   Same as B44 but an additional smaller encoding for constant color blocks/alpha is added  (lossy)  Not as good for random access into image
 *
 *  @memberof   axr_data_t
 */
#if  defined(DOXYGEN)
typedef enum axr_compression_t
{
    axr_compression_none        AXR_ENUM_AVAILABILITY_v1  =   0,    //*<  NO_COMPRESSION
    axr_compression_rle         AXR_ENUM_AVAILABILITY_v1  =   1,    //*<  RLE_COMPRESSION
    axr_compression_zips        AXR_ENUM_AVAILABILITY_v1  =   2,    //*<  ZIPS_COMPRESSION
    axr_compression_zip         AXR_ENUM_AVAILABILITY_v1  =   3,    //*<  ZIP_COMPRESSION
    axr_compression_piz         AXR_ENUM_AVAILABILITY_v1  =   4,    //*<  PIZ_COMPRESSION
    axr_compression_pxr24       AXR_ENUM_AVAILABILITY_v1  =   5,    //*<  PXR24_COMPRESSION
    axr_compression_b44         AXR_ENUM_AVAILABILITY_v1  =   6,    //*<  B44_COMPRESSION
    axr_compression_b44a        AXR_ENUM_AVAILABILITY_v1  =   7,    //*<  B44A_COMPRESSION

    /* other values are reserved */
};
#else
OS_ENUM( axr_compression, uint32_t,
    axr_compression_none        AXR_ENUM_AVAILABILITY_v1  =   0,    //*<  NO_COMPRESSION
    axr_compression_rle         AXR_ENUM_AVAILABILITY_v1  =   1,    //*<  RLE_COMPRESSION
    axr_compression_zips        AXR_ENUM_AVAILABILITY_v1  =   2,    //*<  ZIPS_COMPRESSION
    axr_compression_zip         AXR_ENUM_AVAILABILITY_v1  =   3,    //*<  ZIP_COMPRESSION
    axr_compression_piz         AXR_ENUM_AVAILABILITY_v1  =   4,    //*<  PIZ_COMPRESSION
    axr_compression_pxr24       AXR_ENUM_AVAILABILITY_v1  =   5,    //*<  PXR24_COMPRESSION
    axr_compression_b44         AXR_ENUM_AVAILABILITY_v1  =   6,    //*<  B44_COMPRESSION
    axr_compression_b44a        AXR_ENUM_AVAILABILITY_v1  =   7,    //*<  B44A_COMPRESSION

    /* other values are reserved */
);
#endif

/*! @function axr_compression_get_name
    @abstract Return a human readable name for the type
    @memberof axr_compression_t */
const char * __nonnull axr_compression_get_name( axr_compression_t compressionType ) AXR_AVAILABILITY_v1;

    
/*! @enum       axr_line_order_t
 *  @abstract   Different orderings used used by EXR
 *  @discussion See https://www.openexr.com/documentation/TechnicalIntroduction.pdf, sections on scan lines and tiles.
 *              Application code must read / write the scanlines in the specified order.
 *  @constant   axr_line_order_increasing_y         top scanline first
 *  @constant   axr_line_order_decreasing_y       bottom scanline first
 *  @constant   axr_line_order_random_y            tiles are not sorted
 *
 *  @memberof   axr_data_t
 */
#if  defined(DOXYGEN)
typedef enum axr_line_order_t
{
    axr_line_order_increasing_y     AXR_ENUM_AVAILABILITY_v1  =   0,    //*<  top scanline first.     Read: Origin at top-left.
    axr_line_order_decreasing_y     AXR_ENUM_AVAILABILITY_v1  =   1,    //*<  bottom scanline first.  Read: Origin at bottom-left.
    axr_line_order_random_y         AXR_ENUM_AVAILABILITY_v1  =   2,    //*<  tiles are not sorted.   Read: Origin at top-left

    /* other values are reserved */
};
#else
OS_ENUM( axr_line_order, uint32_t,
    axr_line_order_increasing_y     AXR_ENUM_AVAILABILITY_v1  =   0,    //*<  top scanline first.  Origin at top-left.
    axr_line_order_decreasing_y     AXR_ENUM_AVAILABILITY_v1  =   1,    //*<  bottom scanline first.  Origin at bottom-left.
    axr_line_order_random_y         AXR_ENUM_AVAILABILITY_v1  =   2,    //*<  tiles are not sorted. Origin at top-left

    /* other values are reserved */
);
#endif

/*! @function axr_line_order_get_name
    @abstract Return a human readable name for the type
    @memberof axr_line_order_t */
const char * __nonnull axr_line_order_get_name( axr_line_order_t line_orderType ) AXR_AVAILABILITY_v1;
    
    
/*! @enum       axr_property_flags_t
 *  @abstract   Comments on AppleEXR file properties
 *  @discussion  Depending on circumstance, your application may want to walk the complete list of EXR properties or
 *               just those that AppleEXR did not handle for you. For example, when debugging the complete
 *               list, it is nice to avoid missing any information.  However, when printing out the metadata associated with the
 *               file it would be redundant to print out the contents of the axr_part_info_t and the complete set of
 *               properties because much of the information in the properties is already in the axr_part_info_t. Likewise,
 *               being able to quickly determine which properties that AppleEXR already handled for you is a useful shortcut.
 *               For this reason (and future expansion) the axr_property_flags are included to better help classify information.
 *  @constant   axr_property_unhandled   AppleEXR did not understand this property and did not attempt to handle it
 *  @constant   axr_property_flags_part_info_v1   Information in this property is redundant with the information presented in the axr_part_info_t
 *  @constant   axr_property_flags_channel_info_v1 Information in this property is redundant with the information presented in the axr_channel_info_t
 *
 *  @memberof   axr_data_t
 */
#if defined(DOXYGEN)
typedef enum axr_property_flags_t
{
     axr_property_flags_unhandled           AXR_ENUM_AVAILABILITY_v1  =   0,  //*< Information is not understood or otherwise reported by AppleEXR
     axr_property_flags_part_info_v1        AXR_ENUM_AVAILABILITY_v1  =   1,  //*< Information is redundant with axr_part_info_t
     axr_property_flags_part_mask           AXR_ENUM_AVAILABILITY_v1  = 0xff,
         
     axr_property_flags_channel_info_v1     AXR_ENUM_AVAILABILITY_v1  = (1<<8),  //*< Information is redundant with axr_channel_info_t
     axr_property_flags_channel_mask        AXR_ENUM_AVAILABILITY_v1  = 0xff00,

    /* other bits are reserved */
};
#else
OS_ENUM( axr_property_flags, uint64_t,
    axr_property_flags_unhandled           AXR_ENUM_AVAILABILITY_v1  =   0,  //*< Information is not understood or otherwise reported by AppleEXR
    axr_property_flags_part_info_v1        AXR_ENUM_AVAILABILITY_v1  =   1,  //*< Information is redundant with axr_part_info_t
    axr_property_flags_part_mask           AXR_ENUM_AVAILABILITY_v1  = 0xff,
        
    axr_property_flags_channel_info_v1     AXR_ENUM_AVAILABILITY_v1  = (1<<8),  //*< Information is redundant with axr_channel_info_t
    axr_property_flags_channel_mask        AXR_ENUM_AVAILABILITY_v1  = 0xff00,

    /* other bits are reserved */
);
#endif

/*! @abstract Return a human readable name for the properties flags */
const char * __nonnull axr_property_flags_get_name( axr_property_flags_t flags) AXR_AVAILABILITY_v1;
    
typedef struct axr_box2i
{
    int32_t     xMin, xMax, yMin, yMax;
}axr_box2i_t;

#pragma mark file classification

/*! @function   axr_introspect_data
 *  @abstract   Examine if the indicated data to see if it is a OpenEXR file likely to be readable by this library
 *  @discussion Use to determine whether or not a byte stream represents a decodable by this library
 *  @param fileData     A raw pointer to the first byte of memory containing data from an OpenEXR file
 *  @param fileSize     The number of valid bytes in the region of memory pointed to by fileData
 *  @param flags            Flags to control the decoding process. Valid flags: axr_flags_print_debug_info
 *  @return          Returns axr_error_success if the file is a readable OpenEXR file.
 *                   Returns a non-zero error code  otherwise that describes the failure.  */
axr_error_t axr_introspect_data(
    const void * __nonnull fileData,
    size_t fileSize,
    axr_flags_t flags   _AXR_DEFAULT_FLAGS
) AXR_AVAILABILITY_v1;

/*! @abstract   Block to release or free the file data. It is called when the axr_data_t is destroyed  */
typedef void (^axr_data_destructor_t)( void * __nonnull fileData, size_t fileSize ) AXR_AVAILABILITY_v1;
        
#pragma mark axr_data_t
    
/*!
 *  @class      axr_data_t
 *  A OpenEXR file representation
 *  The axr_data_t defines an interface for an opaque object that can
 *  compress or decompress an OpenEXR image.
 *
 *  The axr_data_t is an os_object.  If your application is not using ARC,
 *  it should be released with os_release when you are done using it.
 */
#if OS_OBJECT_USE_OBJC && ! defined(DOXYGEN)
    OS_OBJECT_DECL(axr_data);
#else
    typedef struct axr_data *  axr_data_t;
#endif
        
/*! @function   axr_data_create
 *  @abstract   Create a axr_data_t object
 *  @param      fileBytes    A pointer to the raw bytes comprising the OpenEXR file
 *  @param      fileSize    The size of the block of memory pointed to by fileData
 *  @param      flags           Options to govern the operation of axr_data_create
 *  @param      outError    If non-NULL, an error code will be written here
 *  @param      dataDestructor    If non-NULL, this method is called when the axr_data_t
 *                              is destroyed. It is intended to help manage the release of the
 *                              memory referenced by fileData.
 *  @return     On success, a valid axr_data_t will be returned capable of decoding the
 *              file.  On failure, NULL is returned.
 *  @memberof   axr_data_t */
__nullable axr_data_t axr_data_create(
                                        const void * __nonnull fileBytes,
                                        size_t fileSize,
                                        axr_error_t * __nonnull outError,
                                        axr_flags_t flags _AXR_DEFAULT_FLAGS,
                                        __nullable axr_data_destructor_t dataDestructor _AXR_DEFAULT(NULL)
                                    ) AXR_RETURNS_RETAINED AXR_AVAILABILITY_v1;
    
/*! @abstract   Get the number of parts contained within the EXR file */
unsigned long axr_data_get_part_count( __nullable axr_data_t ) AXR_AVAILABILITY_v1;

/*! @abstract   A height and width of a rectangular region of pixels */
typedef struct{ uint32_t width, height;} axr_size_t;

/*! @abstract   A horizontal and vertical offset into a rectangular region of pixels */
typedef struct{ uint32_t x, y;} axr_origin_t;

/*! @abstract   A rectangular subregion  of a region of pixels*/
typedef struct{ axr_origin_t origin, axr_size_t;} axr_rect_t;

    
/*! @struct axr_part_info_t
    @abstract information about a EXR  part */
typedef struct
{
    axr_box2i_t                                    dataWindow;          /// actual size of encoded data
    axr_box2i_t                                    displayWindow;       /// region within dataWindow intended to be drawn. Note: individual mip- or rip-levels may be smaller.
    axr_size_t                                     tileSize;            /// size of natural encoding block. It is more efficient to structure decode in chunks of these.
    axr_compression_t                              compression;         /// compression format used for the part
    axr_line_order_t                               lineOrder;           /// line order for the part
    float                                          pixelAspectRatio;    /// EXR pixel aspect ratio
    struct{ float       x, y;}                     screenWindowCenter;  /// EXR screen window center
    float                                          screenWindowWidth;   /// EXR screen window width
    union
    {
        struct{
            struct{ float red, green, blue, white;} x;
            struct{ float red, green, blue, white;} y;
        }                                          chromaticities;   // Chromaticities for color space. Default: Rec. ITU-R BT.709-3  TF: Pixel data is by convention linear. See also axr_decoder_create_colorspace
    };
    struct{
        axr_size_t                  size;
        const uint8_t * __nullable  rgbaData;
    }                                              preview;     // A rgba unorm8 preview image. If not present, rgbaData is NULL.
    const char * __nullable                        name;        // owned by axr_data_t. Do not free.
    const char * __nullable                        view;        // owned by axr_data_t. Do not free.
    axr_level_mode_t                               levelMode;   // {single level, mipmap, ripmap} and rounding direction as bitfield
    float                                          xDensity;    // horizontal pixel density in Pixels Per Inch (PPI, DPI).  The vertical DPI is calculated using the pixelAspectRatio.
}axr_part_info_t;
typedef enum  { axr_part_info_current = 1 } axr_part_info_struct_version_t;

/*! @function   axr_data_get_part_info
 *  @abstract   Get characteristics for the part at index
 *  @discussion Some help understanding OpenEXR deep file formats may be found
 *              here: https://www.openexr.com/documentation/InterpretingDeepPixels.pdf
 *              and https://www.openexr.com/documentation/TechnicalIntroduction.pdf
 *  @param      fileData     The EXR file data
 *  @param      partIndex   An EXR file may have multiple parts. This is the index of the part.
 *  @param      version     Pass axr_part_info_current here
 *  @return     a valid axr_part_info_t describing the part
 *  @memberof   axr_data_t */
axr_part_info_t axr_data_get_part_info( __nullable axr_data_t fileData,
                                        unsigned long partIndex,
                                        axr_part_info_struct_version_t version _AXR_DEFAULT(axr_part_info_current))
                                        AXR_AVAILABILITY_v1;

/*! @abstract   Get the number of channels for the part at partIndex */
unsigned long axr_data_get_channel_count( __nullable axr_data_t fileData, unsigned long partIndex) AXR_AVAILABILITY_v1;
    
    
/*! @struct axr_layer_info_t
    @abstract information about a EXR image layer
    @discussion  Especially in EXR v1 files (before multipart files were introduced) it may be common to have
                 dozens of channels in a part. While it is possible that these all encode different
                 types of data channel for the same part, it may also be that the multiplicity of channels
                 encodes a series of sub-images packaged together in the same file. For example, for a stereo
                 view image, there may be a left eye view and a right eye view. By convention these channels
                 might have names like:
 
                    "left.R", "left.G", "left.B"
                    "right.R", "right.G", "right.B"
 
                 The EXR layer abstraction groups these into "left" and "right" image layers, each with a red, green
                 and blue channel.
 
                 Why do we need layers if we we already have parts?  Version 1 of the file format did not
                 allow for multipart files. These files will be reported with a part count of 1. If multiple images
                 needed to be put in the same file, then one would have to use layers to describe how to
                 group the channels. A problem with this approach is that for most compression formats,
                 (not B44) it may be necessary to fully decode all of the channels in the file in order to get the
                 subset you are asking for in the image layer. When the file is a multipart file, then each part
                 has its own table of tile offsets and we only need to decode the part to get those channels
                 out.  It is possible that a version 2 file may also include layers within each part.
 *  @memberof    axr_data_t */
typedef struct
{
    const char * __nullable         name;           /// the layer name
    const uint32_t * __nullable     channels;       /// an array of channelCount channel indicies into the list of image channels, which indicate the channels present in the layer
    unsigned long                   channelCount;   /// the number of channels in the layer
}axr_layer_info_t;
typedef enum  { axr_layer_info_current = 1 } axr_layer_info_struct_version_t;

/*! @abstract   Get the number of layers in the part
 *  @discussion Layers are derived from the file channel names. The channels may have names of
 *              the format  <layername>.<channel name>.  It is not required that you use layers. They
 *              help disambiguate cases when there are multiple channels of the same type (e.g. red)
 *  @param      fileData     A valid AppleEXR file reference
 *  @param      partIndex         The part index to query. See axr_data_get_part_count()
 *  @return     The number of layers found in the part.
 *  @memberof   axr_data_t */
unsigned long axr_data_get_layer_count( __nullable axr_data_t fileData,
                                        unsigned long partIndex )
                                        AXR_AVAILABILITY_v1;

/*! @abstract Get  information about a specific image layer
 *  @param      fileData     A valid AppleEXR file reference
 *  @param      partIndex  The part index to query. See axr_data_get_part_count()
 *  @param      layerIndex  The index of the layer within the part.
 *  @param      version   You must pass axr_layer_info_current here.
 *  @memberof   axr_data_t */
axr_layer_info_t axr_data_get_layer_info(__nullable axr_data_t fileData,
                                         unsigned long partIndex,
                                         unsigned long layerIndex,
                                         axr_layer_info_struct_version_t version _AXR_DEFAULT(axr_layer_info_current))
                                         AXR_AVAILABILITY_v1;

/*! @struct     axr_channel_info_t
 *  @discussion Summary information about a particular channel in a EXR part
 *  @var        sampleType Data type of the value stored for each channel
 *  @var        sampleBits sizeof(dataType)*8: 16 bits for half, 32 bits for UInt and float
 *  @var        xSampling The sampling rate in horizontal dimension. If > 1, samples are left aligned. Subsampled width is (width + xSampling - 1) / xSampling
 *  @var        ySampling The sampling rate in vertical dimension. If > 1, samples are top aligned. Subsampled height is (height + ySampling - 1) / ySampling
 *  @var        pLinear  0 or 1
 *  @var        name  For conventions on naming, see: https://www.openexr.com/documentation/TechnicalIntroduction.pdf
 *  @memberof   axr_data_t */
typedef struct
{
    axr_type_t          sampleType;     /// Data type of the value stored for each channel
    axr_channel_type_t  channelType;    /// AppleEXR attempts to classify the string in name[] into a well known channel type. on Failure: axr_channel_type_unknown
    uint32_t            sampleBits;     /// sizeof(sampleType)*8: 16 bits for half, 32 bits for UInt and float
    uint32_t            xSampling;      /// The sampling rate in horizontal dimension. If > 1, samples are left aligned. Subsampled width is (width + xSampling - 1) / xSampling
    uint32_t            ySampling;      /// The sampling rate in vertical dimension. If > 1, samples are top aligned. Subsampled height is (height + ySampling - 1) / ySampling
    uint8_t             pLinear;        /// 0 or 1
    uint8_t             _reserved[3];
    char                name[256];	    /// For conventions on naming, see: https://www.openexr.com/documentation/TechnicalIntroduction.pdf
}axr_channel_info_t;
typedef enum  { axr_channel_info_current = 1 } axr_channel_info_struct_version_t;

/*! @function axr_data_get_channel_info
 *  @abstract get information corresponding to an individual channel in a part
 *  @param      fileData                    The EXR file reference
 *  @param      partIndex       An EXR file may have multiple parts. This is the index of the part.
 *  @param      channelIndex  The index of the channel within the part
 *  @param      version              Pass axr_channel_info_current here
 *  @return     A valid axr_channel_info_t
 *  @memberof   axr_data_t */
axr_channel_info_t axr_data_get_channel_info( __nullable axr_data_t fileData,
                                              unsigned long partIndex,
                                              unsigned long channelIndex,
                                              axr_channel_info_struct_version_t version _AXR_DEFAULT(axr_channel_info_current))
                                              AXR_AVAILABILITY_v1;

    
/*! @function   axr_data_get_property_count
    @abstract   Get the number of  properties associated with the part at partIndex.
    @discussion Information in common properties that are understood by the AppleEXR
                are also returned by axr_data_get_channel_info() or axr_data_get_part_info()
    @param      fileData     The Apple EXR part file reference
    @param      partIndex  EXR files may have multiple parts. Each part is described as an part here.
    @return     The number of properties in the EXR part
    @memberof   axr_data_t */
unsigned long axr_data_get_property_count( __nullable axr_data_t fileData,
                                           unsigned long partIndex ) AXR_AVAILABILITY_v1;

/*! @abstract A property found within the OpenEXR metadata  */
typedef struct
{
    const char * __nonnull  name;      /// the name of the property as a '\0' terminated string
    const char * __nonnull  type;      /// the data type  of the property as a '\0' terminated string. https://www.openexr.com/documentation/openexrfilelayout.pdf
    size_t                  size;      /// the size of the data pointed to by value in bytes
    const void * __nonnull  value;     /// a raw pointer to the data contained in the property
    axr_property_flags_t    flags;     /// additional information about the property, including where else in the AppleEXR API the information is available
}axr_property_t;
typedef enum  { axr_property_current AXR_ENUM_AVAILABILITY_v1 = 1 } axr_property_struct_version_t;

/*! @function axr_data_get_property
 *  @abstract Get additional information about an image
 *  @discussion OpenEXR files contain many metadata tags. Commonly used information can often
 *              be obtained from axr_data_get_channel_info() or axr_data_get_part_info().
 *              However, for completeness, the full set of tags are available here.
 *  @param      fileData                       The AppleEXR file reference
 *  @param      partIndex          The index of the part in the file
 *  @param      propertyIndex    The index of the property in the file
 *  @param      version                 Pass axr_property_current here.
 *  @return     Property info corresponding with the part and property. The pointers point directly to file data
 *              and will become invalid if the file data is freed, unmapped, etc. The data should be copied, not cached.
 *  @memberof   axr_data_t */
axr_property_t axr_data_get_property( __nullable axr_data_t fileData,
                                      unsigned long partIndex,
                                      unsigned long propertyIndex,
                                      axr_property_struct_version_t version _AXR_DEFAULT(axr_property_current) )
                                      AXR_AVAILABILITY_v1;
    
/*! @abstract return the number of mip- or ripmap levels for a part
 *  @param      fileData              The AppleEXR file reference
 *  @param      partIndex          The index of the part in the file
 *  @return   The number of mip- or ripmap levels in a part. If axr_level_mode_one_level, 1 is returned. */
unsigned long axr_data_get_level_count( __nullable axr_data_t fileData,
                                        unsigned long partIndex )
                                        AXR_AVAILABILITY_v1;
    
/*! @abstract return the number of mip- or ripmap levels for an part
 *  @param      fileData              The AppleEXR file reference
 *  @param      partIndex          The index of the part in the file
 *  @param      levelIndex          The index of the level in the index
 *  @return   The number of pixels in each dimension for the part level  */
axr_size_t axr_data_get_level_size( __nullable axr_data_t fileData,
                                    unsigned long partIndex,
                                    unsigned long levelIndex _AXR_DEFAULT(0) )
                                    AXR_AVAILABILITY_v1;

#pragma mark axr_decoder_t
/*!
 *  @class      axr_decoder_t
 *  @abstract   A decoder for a EXR file
 *  @discussion The axr_decoder_t defines an interface for an opaque object that
 *              allows OpenEXR pixel content to be decoded.
 *
 *              The axr_decoder_t is an os_object.  If your application is not using ARC,
 *              it should be released with os_release when you are done using it.
 */

#if OS_OBJECT_USE_OBJC && ! defined(DOXYGEN)
    OS_OBJECT_DECL(axr_decoder);
#else
    typedef struct axr_decoder *  axr_decoder_t;
#endif


    
/*! @function   axr_decoder_create
 *  @abstract   create a new decoder
 *  @discussion This is an opaque object that describes the subregion of the input image to read, and the
 *              channel order and contents of the results.  At creation:
 *
 *              - The subregion of interest is the entire dataWindow.
 *              - The starting output channel list is empty.
 *                  More channels can be added  to the output stream with axr_decoder_append_result_channel()
 *                  or axr_decoder_append_constant_value_result_channel.
 *
 *              The object is a subclass of os_object and must be reference counted with os_retain / os_release
 *              if automatic reference counting (ARC) is not enabled.  Other NSObject methods such as -description and
 *              -debugDecription are available for debugging purposes. When used with axr_decoder_read_pixels,
 *              the order the channels appear in memory in the destination matches the  the order they are appended
 *              to the decoder.
 *
 *              For the purpose of reading pixels from an axr_data_t, each axr_decoder_t is internally multithreaded on
 *              the chunk (tile) level. If your application desires to itself manage the threading, the internal multithreading
 *              may be disabled using the axr_flags_disable_multithreading flag with axr_decoder_read_pixels/planes().
 *              To avoid redundant work, the threading granularity should align with and be a multiple of the
 *              axr_part_info_t.tileSize. Your application may create many axr_decoder_t objects for each axr_file_t.
 *              Behavior is undefined if the axr_decoder_t properties (e.g. subregion, channel lists) are modified concurrently
 *              with calls to axr_decoder_read_pixels/planes. Your application is responsible for creating appropriate locks
 *              or queues as  necessary to prevent this from happening.  axr_decoder_t objects are light weight.
 *              Significant memory may be transiently allocated to support the operation of axr_decoder_read_pixels/planes()
 *              for compression formats that are not B44, B44a and uncompressed.
 *
 *
 *  @param      fileData    The file reference that will be used with the decoder. Retained by read decoder.
 *                        Used to provide sensible default values for decoder.
 *  @param      partIndex The index of the part within the OpenEXR file to decode
 *  @param      levelIndex   The index of the mip- or ripmap level to use
 *  @result     On success, a valid axr_decoder_t object.  */
axr_decoder_t __nonnull axr_decoder_create( __nonnull axr_data_t fileData,
                                            unsigned long partIndex _AXR_DEFAULT(0),
                                            unsigned long levelIndex _AXR_DEFAULT(0),
                                            axr_flags_t   flags _AXR_DEFAULT_FLAGS
                                           ) AXR_RETURNS_RETAINED AXR_AVAILABILITY_v1;

/*! @function   axr_decoder_create_rgba
 *  @abstract   convenience method to create a new decoder for RGBA output
 *  @discussion This method combines the function of axr_decoder_create, axr_decoder_append_channels, and various
 *              information getters to attempt to select appropriate channels for RGBA output for a particular layer and part.
 *              The decoder created is intended to be suitable for use with axr_decoder_read_rgba_pixels, though it is not
 *              necessary to use axr_decoder_create_rgba to create a decoder appropriate for axr_decoder_read_rgba_pixels.
 *
 *              The function will attempt to select a subset of channels from the part / layer that best match the RGBA color
 *              model. The channels selected are in order of priority:
 *
 *                  R, G, B, (A)
 *                  X, Y, Z, (A)
 *                  Y, BY, RY, (A)
 *                  Y (A)
 *
 *              If the channels have unusual names (i.e. axr_channel_info_t.channelType doesn't identify them) then it is likely
 *              your application will need to fall back to selecting the channels by hand.
 *
 *  @param      fileData    The file reference that will be used with the decoder. Retained by read decoder.
 *                        Used to provide sensible default values for decoder.
 *  @param      layerName  If not NULL, the algorithm will restrict its search for channels in the indicated layer.
 *                        Use NULL to search the entire part. Use "" to search the default layer only.
 *  @param      partIndex The index of the part within the OpenEXR file to decode
 *  @param      levelIndex   The index of the mip- or ripmap level to use
 *  @param      flags           Options to govern the operation of axr_decoder_create_rgba
 *  @return     On failure, NULL will be returned.  Otherwise, a valid axr_decoder_t will be returned, suitable for use with
 *              axr_decoder_read_rgba_pixels and axr_decoder_create_rgba_colorspace. */
axr_decoder_t __nullable axr_decoder_create_rgba( __nonnull axr_data_t fileData,
                                                  const char * __nullable layerName _AXR_DEFAULT(NULL),
                                                  unsigned long partIndex _AXR_DEFAULT(0),
                                                  unsigned long levelIndex _AXR_DEFAULT(0),
                                                  axr_flags_t   flags _AXR_DEFAULT_FLAGS) AXR_RETURNS_RETAINED AXR_AVAILABILITY_v1;

/*! @function axr_decoder_copy
 *  @abstract Create a copy of the axr_decoder_t
 *  @discussion A copy may be necessary for managing multiple decoders on the same axr_data_t, particularly
 *              achieving thread safety for changes to decoder properties while the decoder is reading pixels or planes
 *  @return   A copy of the decoder */
axr_decoder_t __nonnull axr_decoder_copy( axr_decoder_t __nonnull decoder ) AXR_RETURNS_RETAINED AXR_AVAILABILITY_v1;
    
/*! @function   axr_decoder_set_subregion
 *  @abstract   Change the subregion of the image decoded by the read operation
 *  @discussion Coordinates are given within the dataWindow reference frame. {0,0}
 *              is the top left corner of the dataWindow, the first encoded byte in the image
 *              Example: to read the entire data window, set origins to 0, xSize to
 *              dataWindow.xMax - dataWindow.xMin +1, and ySize to
 *              dataWindow.yMax - dataWindow.yMin + 1.
 *
 *              Caution:  OpenEXR is generally not a randomly addressable image format. The tiles
 *              are randomly addressable, but the regions within the tiles are not. If you want to read
 *              1 pixel from a tile, the entire tile may need to be decoded. Consequently, if your
 *              application reads data 1 scanline at a time, it is entirely likely that by the time you
 *              have finished decoding the image, you will have actually decoded it 16 or 32 times
 *              over.  To avoid this problem, read data from the image in multiples of the tile size.
 *              Tile alignment starts aligned with the top left corner of the image {0,0} and proceeds
 *              in a packed fashion to the right and downward from there.
 *              See: axr_part_info_t.tileSize
 *
 *  @param      xOrigin     The horizontal position of the left edge of the region of interest in the dataWindow coordinate space.
 *  @param      yOrigin     The vertical position of the top edge of the subregion of interest in the dataWindow coordinate space.
 *  @param      xSize          The number of pixels to read in the horizontal dimension. Should be tile aligned for best performance.
 *  @param      ySize           The number of pixels to read in the vertical dimension. Should be tile aligned for best performance.
 *  @param      flags           flags to control the behavior of the operation
 *  @return     An axr_error_t indicating the success or failure of the operation.
 *  @memberof   axr_decoder_t */
axr_error_t axr_decoder_set_subregion( __nonnull axr_decoder_t d,
                                       uint32_t xOrigin,
                                       uint32_t yOrigin,
                                       uint32_t xSize,
                                       uint32_t ySize,
                                       axr_flags_t flags _AXR_DEFAULT_FLAGS);
    
/*! @function axr_decoder_append_channel
 *  @abstract Append a channel to the output stream
 *  @discussion Not atomic
 *          The correct choice of channel can sometimes be complicated by the presence of multiple
 *          views in the same part each with seemingly redundant color content. For example
 *          a part might reasonably contain 9 channels:
 *
 *              "red",         "green",        "blue",
 *              "left.red",   "left.green",  "left.blue",       // left stereo image
 *              "right.red", "right.green" "right.blue      // right stereo image
 *
 *          in any order. A simple heuristic such as pick the first few channels
 *          isn't likely to be robust.
 *
 *          In OpenEXR channel nomenclature, the channels are grouped into layers. Make sure
 *          that all channels that appear together in a part share the same layer. For the above set,
 *          the layers would be "", "left" and "right". Layers may themselves have multiple levels of
 *          hierarchy, such as "camera1.left.<channelname>" and do not need to share a consistent
 *          layer depth. As a general guideline for picking the default layer to show in the absence of
 *          other identifying information, prefer layers that have fewer prefixes over those with more.
 *          If two layers have the same number of prefixes, prefer the one that appears first in the file.
 *          Only channels in the same part may be read concurrently in the same call to
 *          axr_decoder_read_planes / axr_decoder_read_pixels. Layers with the same name in
 *          in different parts should be considered different layers. They may be associated
 *          with one another through some other dimension like time while still being different frames.
 *
 *  @param  decoder        The decoder
 *  @param  channelIndex    The index of the new channel to be appended to the result list.
 *                        If the channel will be used with axr_decoder_read_pixels, the
 *                        axr_type_t of the channel must match that of the previous channels.
 *  @return axr_error_success on success, a non-zero error otherwise
 *  @memberof   axr_decoder_t */
axr_error_t axr_decoder_append_channel( __nonnull axr_decoder_t decoder,
                                        uint32_t channelIndex,
                                        axr_flags_t flags _AXR_DEFAULT_FLAGS);

/*! @function axr_decoder_append_constant_value_channel
 *  @abstract Append a constant value channel to the  output stream
 *  @discussion A constant value channel is a channel filled entirely by a single value.
 *              For example, if the part contains only RGB data but you need RGBA with alpha 1.0
 *              everywhere, then append a constant value channel after red, green, blue with value 1.0.
 *              The x- and y-sampling rates are inferred from the other channels present in the part.
 *              It is an error to use constant value channels with axr_decoder_read_planes.
 *  @param  decoder              The axr_decoder_t
 *  @param  value                   The value held by the channel. All pixels in the image will contain this value.
 *                        A IEEE-754 double precision value can represent all uint32_t, float and __fp16
 *                        values without loss of precision.
 *  @param  sampleType        The data type used by the additional channel. I attempted to infer this value
 *                        from context but found too many corner cases in which the missing information
 *                        is needed resulting  in expense reanalyzing the decoder for context and
 *                        too many error conditions. So as to simplify error handling for the application,
 *                        the type is passed in explicitly. When used with axr_decoder_read_pixels,
 *                        the axr_type_t for all channels in the pixel must match or undefined behavior will occur.
 *                        axr_decoder_read_planes will function as desired for cases with heterogeneous
 *                        axr_types.
 *  @param  channelType      The type of channel to be added. Used for colorspace inference in axr_decoder_create_colorspace
 *                        and axr_decoder_create_rgba_colorspace .
 *  @param  flags                   Flags to control the operation of axr_decoder_append_constant_value_channel
 *  @return axr_error_success on success, a non-zero error otherwise
 *  @memberof   axr_decoder_t */
axr_error_t axr_decoder_append_constant_value_channel( __nonnull axr_decoder_t decoder,
                                                       double value,
                                                       axr_type_t sampleType,
                                                       axr_channel_type_t channelType,
                                                       axr_flags_t flags _AXR_DEFAULT_FLAGS );

/*! @function  axr_decoder_get_channel_count
 *  @abstract  Get the number of  output channels appended so far
 *  @param     decoder   A valid axr_decoder_t
 *  @memberof  axr_decoder_t */
uint32_t axr_decoder_get_channel_count( __nonnull axr_decoder_t decoder);
    
/*! @function axr_decoder_translate_channel_index
 *  @abstract Get the part index of an individual decoder channel
 *  @discussion  A decoder may reference channels {1, 2, 5, 9} as the set of channels
 *               to be decoded. When your application calls axr_decoder_get_channel_info()
 *               it will refer to these as channel indices {0,1,2,3}. However, it may be necessary
 *               at times to lookup the original indices of those channels in the axr_data_t.
 *               This function will map [0,3] back to, for example, {1,2,5,9}.
 *  @param      decoder             The axr_decoder_t
 *  @param      channelIndex  The index of the channel within the decoder.
 *                           The index[0, axr_decoder_get_channel_count()-1]
 *  @return     The index of the channel within the part or -1 if the channelIndex is too large or the channel is
 *              a constant value channel (and therefore not in the part)
 *  @memberof   axr_decoder_t */
long axr_decoder_translate_channel_index( __nonnull axr_decoder_t decoder,
                                          unsigned long channelIndex)
                                          AXR_AVAILABILITY_v1;

/*! @function axr_decoder_get_channel_info
 *  @abstract Get axr_channel_info_t for a channel described by a decoder
 *  @param decoder       A valid axr_decoder_t
 *  @param channelIndex  The index of the channel in the decoder. (Not the same index as that used in axr_data_get_channel_info!)
 *                      If there are N channels described in the decoder, this has value [0,N-1]
 *  @param version             You must pass axr_channel_info_current here.
 *  @result A axr_channel_info_t with information about the channel.  Do not free any pointers passed in the struct.
 *  @memberof   axr_decoder_t */
axr_channel_info_t axr_decoder_get_channel_info(__nonnull axr_decoder_t decoder,
                                                unsigned long channelIndex,
                                                axr_channel_info_struct_version_t version _AXR_DEFAULT(axr_channel_info_current) )
                                                AXR_AVAILABILITY_v1;

/*! @function  axr_decoder_clear_channel_list
 *  @abstract  Remove all result channels from the output channel list
 *  @param     decoder   A valid axr_decoder_t
 *  @memberof  axr_decoder_t */
axr_error_t axr_decoder_clear_channel_list( __nonnull axr_decoder_t decoder ) AXR_AVAILABILITY_v1;

/*! @struct axr_decoder_info_t
    @abstract information about the segment of a EXR part referenced by the decoder  */
typedef struct
{
    struct axr_data * __nonnull                    data;            /// the axr_data_t used to create the decoder
    uint32_t                                       partIndex;       /// part index used  to create decoder
    axr_level_mode_t                               levelMode;       /// the axr_level_mode_t for the image
    uint32_t                                       levelIndex;      /// the index of the mip level
    axr_size_t                                     levelSize;       /// the size in pixels of this mip- or  rip-level
    struct
    {
        struct{ uint32_t x, y;}                    origin;          /// The offset to the top left corner of the subregion from the the top left corner of the data window
        axr_size_t                                 size;            /// the size of the sub region
    }subregion;                                                     /// the spatial subregion to decode. See axr_decoder_set_subregion
}axr_decoder_info_t;
typedef enum  { axr_decoder_info_current = 1 } axr_decoder_info_struct_version_t;

axr_decoder_info_t axr_decoder_get_info( __nonnull axr_decoder_t decoder,
                                         axr_decoder_info_struct_version_t version _AXR_DEFAULT(axr_decoder_info_current))
                                         AXR_AVAILABILITY_v1;
    
/*! @function axr_decoder_get_data
 *  @abstract Get the axr_data_t used to create the decoder
 *  @memberof axr_decoder_t */
__nonnull axr_data_t axr_decoder_get_data( __nonnull axr_decoder_t data) AXR_AVAILABILITY_v1;

/*! @function axr_decoder_get_data_window_size
 *  @abstract returns the size of the data window for the decoder mipmap level */

#pragma mark Read the image
        
/*! @abstract  interleave two or four channels from a subregion of an part into a single buffer
 *  @discussion Each pixel is a interleaved set of channels, e.g. RGBA, RGBA, RGBA...
 *              The channels must all be the same size. The number of channels must be 1, 2, or 4
 *              The x & y subsampling for the channels must match. The mip/rip levels must match.
 *              The position and size of the region of interest in the part must match.
 *              This method should be  faster than reading out planes separately then interleaving them.
 *              This method is not suitable for mixed sampling rates like 422, 420, etc. though it may
 *              be used to create the {Cb,Cr,Cb,Cr...} plane in 420.
 *
 *              Performance Advisory:  Most OpenEXR compression formats are best described as a
 *              2-pass decode process. Typically, the first pass decodes a lossless bitstream
 *              compression scheme like Zip or Huffman. The second pass extracts out the requested
 *              channels from the others, often performing some data manipulations along the way such
 *              as interleaving bytes from different points in the compressed stream, wavelet
 *              reconstruction or the B44 transform. Importantly, the first pass is generally very
 *              serial and not dynamically addressable due to variable code word size and lack of
 *              restart markers or other context clues. The second pass is typically very parallel, easily
 *              vectorizable and trivial to calculate the location of data leading to any particular pixel.
 *
 *              Because prior to 1st pass decompression the file contents are generally not dynamically addressable
 *              below part and chunk granularity, this library may have to do 1st pass decompression on much
 *              more data than is ultimately needed to fulfil the request. For this purpose, image subregions are
 *              rounded outward to the chunks (tiles) that contain them, and there is no dynamic addressability
 *              for channels at all. So, if one channel is needed from a part with 20 channels, the library might be
 *              forced to do 1st pass decoding on all 20 channels to retrieve the one requested.  Since the first pass
 *              may dominate the total decompression time -- recall this stage is also usually not vectorizable or
 *              trivially multithreaded -- and may have to work on much more data than is needed and AppleEXR
 *              does not currently cache the first pass decoding results for use by later calls to axr_decoder_read_planes
 *              or axr_decoder_read_pixels -- they are big -- it is important that your application acts to minimize the 1st
 *              pass workload. An application that does not take this file format limitation into consideration may find
 *              itself redundantly running 1st pass decompression on the same data over and over, triggering a needless
 *              performance regression of possibly an order of magnitude or more.
 *
 *              There are several things your application can do to mitigate this problem:
 *
 *              1) When possible, encode your images taking advantage of OpenEXR multipart (OpenEXR 2.0
 *              and later) file format, taking care to bundle channels that are used together into the same part.
 *              Move channels that are not used together or are rarely used off to other parts. This is most
 *              important when there are many channels in the part, only some of which are typically used.
 *
 *              2) If your application needs to read multiple channels from a part, be sure to read them all at once
 *              with a single call to axr_decoder_read_pixels or axr_decoder_read_planes. This will ensure the
 *              1st pass decoding stage happens only once per part.
 *
 *              3) The axr_compression_none and axr_compression_b44 methods do not involve a 1st pass
 *              and consequently do not suffer this limitation. axr_compression_b44a has a variable
 *              block encoding size and does not enjoy the same benefit as axr_compression_b44.
 * *
 *              Terms: An OpenEXR "part" is a OpenEXR file segment containing one or more channels. Prior to
 *              OpenEXR 2.0, all files were single part files, meaning that all channels were encoded together.
 *              Please see the OpenEXR File Layout specification for a more details. The number of parts returned by
 *              axr_data_get_part_count() is the number of parts in the file. Chunks are spatial subregions
 *              of each part. They are described as tiles in the AppleEXR interface. See axr_part_info_t.tileSize.
 *
 *              The above information is provided for software tuning purposes only. Implementation details
 *              are subject to change in future versions of the library.
 * *
 *  @param  decoder      The decoder object that specifies which part of the data to read
 *  @param  destination     A pointer to the top left corner of the place to write the output data. See discussion
 *                       above for special treatment of axr_line_order_decreasing_y.
 *                       Note that addressing here is different from OpenEXR. The OpenEXR setFrameBuffer( ptr, ...)
 *                       call expects ptr to point to {0,0} in the dataWindow coordinate space. Whereas,
 *                       AppleEXR, the destination pointer points to {dataWindow.min.x, dataWindow.min.y}
 *  @param  destinationRowBytes  The distance in bytes from a sample to the next vertically adjacent sample in the destination buffer. Supports negative rowBytes. See  discussion above for special treatment of axr_line_order_decreasing_y
 *  @param  flags        Options to control the decode process
 *  @return axr_error_success if the operation succeeds.  Another axr_error code otherwise.
 *  @memberof axr_decoder_t */
axr_error_t axr_decoder_read_pixels( __nonnull axr_decoder_t decoder,
                                     void * __nonnull destination,
                                     unsigned long destinationRowBytes,
                                     axr_flags_t flags _AXR_DEFAULT_FLAGS ) AXR_AVAILABILITY_v1;
    

/*! @abstract  Read the channels described by the decoder as RGBA content
 *  @discussion  Behavior varies according to available channels:
 *
 *                  RGB, RGBA:                 Decoded as is and interleaved to RGBA
 *                  Y RY BY, Y RY BY A:    Decoded and converted to RGBA  (YUV / YCC / YCbCr content)
 *                  XYZ, XYZA                    Decoded to XYZA
 *                  Y, YA                             Decoded to Y,Y,Y,A    (grayscale content as RGBA)
 *
 *                  Only these channel combinations are supported. All channels must have the same axr_type_t.
 *
 *              In each case, the colorspace returned by axr_decoder_create_rgba_colorspace will be appropriate
 *              to the content returned by this function.
 *
 *          The  size of the destination buffer is given by axr_decoder_info_t.subregion.size.height * rowBytes.
 *          The minimum rowBytes is given by 4 * width * sampleBytes.   If the rowBytes is larger than that, the extra
 *          memory in the row will not be modified. SampleBytes is 2 for half, and 4 for float. Uint32_t is not supported
 *          by this function.
 *
 *  @param  decoder      The decoder object that specifies which part of the data to read
 *  @param  destination     A pointer to the top left corner of the place to write the output data. See discussion above for special treatment of axr_line_order_decreasing_y
 *  @param  destinationRowBytes  The distance in bytes from a sample to the next vertically adjacent sample in the destination buffer. Supports negative rowBytes. See  discussion above for special treatment of axr_line_order_decreasing_y
 *  @param  alphaVal    If no alpha channel is present, the value in alphaVal is used instead
 *  @param  flags        Options to control the decode process
 *  @return axr_error_success if the operation succeeds.  Another axr_error code otherwise.  Use axr_decoder_create_rgba_colorspace to
 *          obtain a CGColorSpaceRef to represent this content.
 *                  */
axr_error_t axr_decoder_read_rgba_pixels(__nonnull axr_decoder_t decoder,
                                         void * __nonnull destination,
                                         unsigned long destinationRowBytes,
                                         double alphaVal,
                                         axr_flags_t flags _AXR_DEFAULT_FLAGS ) AXR_AVAILABILITY_v1;

/*! @struct axr_pixel_data_t
 *  @abstract A structure that describes each plane written to by axr_decoder_read_planes */
typedef struct axr_pixel_data_t
{
    void * __nonnull        data;       /// pointer to top left corner of region to write to
    size_t                  rowBytes;   /// the distance in bytes from the start of a pixel and the same position one row below
}axr_pixel_data_t;
typedef enum  { axr_pixel_data_current AXR_ENUM_AVAILABILITY_v1 = 0 } axr_pixel_data_struct_version_t;

/*! @function axr_decoder_read_planes
 *  @abstract read one or more channels from a subregion of an image as separate image planes
 *  @discussion Each plane contains a single channel. It is much more efficient to read multiple planes at once than one at a time.
 *              The planes may not be the result of axr_decoder_append_constant_value_channel(). They must be real
 *              data in the file.
 *
 *              Performance Advisory:  Most OpenEXR compression formats are best described as a
 *              2-pass decode process. Typically, the first pass decodes a lossless bitstream
 *              compression scheme like Zip or Huffman. The second pass extracts out the requested
 *              channels from the others, often performing some data manipulations along the way such
 *              as interleaving bytes from different points in the compressed stream, wavelet
 *              reconstruction or the B44 transform. Importantly, the first pass is generally very
 *              serial and not dynamically addressable due to variable code word size and lack of
 *              restart markers or other context clues. The second pass is typically very parallel, easily
 *              vectorizable and trivial to calculate the location of data leading to any particular pixel.
 *
 *              Because prior to 1st pass decompression the file contents are generally not dynamically addressable
 *              below part and chunk granularity, this library may have to do 1st pass decompression on much
 *              more data than is ultimately needed to fulfill the request. For this purpose, image subregions are
 *              rounded outward to the chunks (tiles) that contain them, and there is no dynamic addressability
 *              for channels at all. So, if one channel is needed from a part with 20 channels, the library might be
 *              forced to do 1st pass decoding on all 20 channels to retrieve the one requested.  Since the first pass
 *              may dominate the total decompression time -- recall this stage is also usually not vectorizable or
 *              trivially multithreaded -- and may have to work on much more data than is needed and AppleEXR
 *              does not currently cache the first pass decoding results for use by later calls to axr_decoder_read_planes
 *              or axr_decoder_read_pixels -- they are big -- it is important that your application acts to minimize the 1st
 *              pass workload. An application that does not take this file format limitation into consideration may find
 *              itself redundantly running 1st pass decompression on the same data over and over, triggering a needless
 *              performance regression of possibly an order of magnitude or more.
 *
 *              There are several things your application can do to mitigate this problem:
 *
 *              1) When possible, encode your images taking advantage of OpenEXR multipart (OpenEXR 2.0
 *              and later) file format, taking care to bundle channels that are used together into the same part.
 *              Move channels that are not used together or are rarely used off to other parts. This is most
 *              important when there are many channels in the part, only some of which are typically used.
 *
 *              2) If your application needs to read multiple channels from a part, be sure to read them all at once
 *              with a single call to axr_decoder_read_pixels or axr_decoder_read_planes. This will ensure the
 *              1st pass decoding stage happens only once per part.
 *
 *              3) The axr_compression_none and axr_compression_b44 methods do not involve a 1st pass
 *              and consequently do not suffer this limitation. axr_compression_b44a has a variable
 *              block encoding size and does not enjoy the same benefit as axr_compression_b44.
 * *
 *              Terms: An OpenEXR "part" is a OpenEXR file segment containing one or more channels. Prior to
 *              OpenEXR 2.0, all files were single part files, meaning that all channels were encoded together.
 *              Please see the OpenEXR File Layout specification for a more details. The number of images returned by
 *              axr_data_get_part_count() is the number of parts in the file. Chunks are spatial subregions
 *              of each part. They are described as tiles in the AppleEXR interface. See axr_part_info_t.tileSize.
 *
 *              The above information is provided for software tuning purposes only. Implementation details
 *              are subject to change in future versions of the library.
 * *
 *  @param  decoder     The list of channels to read
 *  @param  destinationPlaneList     A pointer to an array of axr_pixel_data_t structures describing where to put each plane.
 *                                The number of such is given by axr_decoder_get_channel_count(decoder)
 *                                The size of each is given by the decoder. The order matches the order the channels
 *                                were added to the decoder.
 *  @param  flags                        Options to control the decode process
 *  @param  structVersion       Pass axr_pixel_data_current here.
 *  @return axr_error_success if the operation succeeds.  Another axr_error code otherwise.
 *  @memberof axr_decoder_t */
axr_error_t axr_decoder_read_planes( __nonnull axr_decoder_t decoder,
                                     axr_pixel_data_t * __nonnull destinationPlaneList,  // Address of first struct in axr_pixel_data_t[ axr_decoder_get_channel_count(decoder) ]
                                     axr_flags_t flags _AXR_DEFAULT_FLAGS,
                                     axr_pixel_data_struct_version_t structVersion _AXR_DEFAULT(axr_pixel_data_current) ) AXR_AVAILABILITY_v1;

/*! @function axr_decoder_create_colorspace
 *  @abstract Create a CGColorSpaceRef to represent the EXR image
 *  @discussion Use this to create a CGColorSpaceRef suitable for use to represent the data with CoreGraphics
 *              in a CGImageRef, CGColorConversionInfoRef or a CGContextRef.  Note that if the data is YCbCr,
 *              the CGColorSpaceRef for the corresponding RGB colorspace is returned.
 *  @param      decoder   A valid axr_decoder_t
 *  @param      options         An options dictionary to used to provide additional configuration at the CGColorSpaceRef creation step.
 *                   Available options:
 *                      <none>      AXR_AVAILABILITY_v1
 *  @param      flags           flags to control the operation of the function. axr_flags_print_debug_info may provide additional information for failure cases.
 *  @return If the color model could be inferred from the channel names and corresponds to a representable CGColorSpaceRef, a valid CGColorSpaceRef will
 *          be returned. Otherwise, NULL will be returned. For the purpose of color model identification, alpha is ignored, but non-color components such as
 *          unknown, distance, distance_back, id, etc. will trigger failure. (CG doesn't well tolerate  non-alpha data interleaved with color data, even when
 *          CGImageAlphaNoneSkip{First/Last} is used to describe those channels.)
 */
void * __nullable /*CGColorSpaceRef*/
    axr_decoder_create_colorspace( __nonnull axr_decoder_t decoder,
                                   axr_flags_t flags,
                                   void * __nullable /* CFDictionaryRef */ options _AXR_DEFAULT(NULL)) AXR_AVAILABILITY_v1;
    
    /*! @function axr_decoder_create_rgba_colorspace
     *  @abstract Create a CGColorSpaceRef to represent the EXR image as read by axr_decoder_read_rgba_pixels
     *  @discussion Use this to create a CGColorSpaceRef suitable for use to represent the data with CoreGraphics
     *              in a CGImageRef, CGColorConversionInfoRef or a CGContextRef for data produced by axr_decoder_read_rgba_pixels.
     *              The difference in behavior between this interface and axr_decoder_create_colorspace is that grayscale
     *              content will return a RGB colorspace. axr_decoder_read_rgba_pixels returns YYYA for grayscale content
     *              and should be associated with a RGB colorspace.
     *
     *              CAUTION: The colorspace returned by this function is not guaranteed to be a RGBA colorspace. If the
     *              part chromaticities indicate XYZ, the colorspace will be XYZ instead.
     *
     *  @param      decoder   A valid axr_decoder_t
     *  @param      options         An options dictionary to used to provide additional configuration at the CGColorSpaceRef creation step.
     *                   Available options:
     *                      <none>      AXR_AVAILABILITY_v1
     *  @param      flags           flags to control the operation of the function. axr_flags_print_debug_info may provide additional information for failure cases.
     *  @return     Decoder channels         Output colorspace                                         axr_decoder_create_colorspace behavor
     *              ==============        ==============                                        ==============================
     *                R, G, B, (A)              RGB linear extended colorspace                    RGB linear extended colorspace
     *                X, Y, Z, (A)                XYZ linear extended colorspace                    XYZ linear extended colorspace
     *                Y BY RY (A)             RGB linear extended colorspace                   RGB linear extended colorspace
     *                Y (A)                        RGB linear extended colorspace                    Grayscale linear extended colorspace
     *                other                       NULL
     */
void * __nullable /*CGColorSpaceRef*/
    axr_decoder_create_rgba_colorspace( __nonnull axr_decoder_t decoder,
                                        axr_flags_t flags,
                                        void * __nullable /* CFDictionaryRef */ options _AXR_DEFAULT(NULL)) AXR_AVAILABILITY_v1;

#pragma mark -
#pragma mark axr_encoder_t
/*!
 *  @class      axr_encoder_t
 *  @abstract   A encoder for a EXR file
 *  @discussion The axr_encoder_t defines an interface for an opaque object that
 *              allows  pixel content to be encoded to an OpenEXR file.
 *
 *              The axr_encoder_t is an os_object.  If your application is not using ARC,
 *              it should be released with os_release when you are done using it.
 */

#if OS_OBJECT_USE_OBJC && ! defined(DOXYGEN)
    OS_OBJECT_DECL(axr_encoder);
#else
    typedef struct axr_encoder *  axr_encoder_t;
#endif




/*! @abstract  Create a mutable axr_encoder_t object
 *  @discussion In version 1, deep data is not supported.
 *  @param      flags               A default set of flags to use with the object, when  other flags are not available
 *  @return     On success, a pointer to a axr_encoder_t */
__nonnull axr_encoder_t axr_encoder_create( axr_flags_t flags ) AXR_RETURNS_RETAINED AXR_AVAILABILITY_v1;
    

/*!
 *  @class     axr_part_t
 *  @abstract   An description for a part of a EXR file
 *  @discussion The axr_part_t defines an interface for an opaque object that
 *              allows OpenEXR  part to be encoded.  The object belongs to
 *              the axr_encoder_t and should not be freed or released by you. */
#if OS_OBJECT_USE_OBJC && ! defined(DOXYGEN)
    OS_OBJECT_DECL(axr_part);
#else
    typedef struct axr_part *  axr_part_t;
#endif

/*! @abstract Append a new part for encoding an EXR file
 *  @discussion  The following default values are also initialized:
 *                  chromaticities:  ITU-BT.709
 *                  whiteLuminance: 1
 *                  preview: <none>
 *
 *               axr_compression_piz  should use a tile height that is a power of two such as 32
 *               tile dimensions do not need to evenly divide into data window dimensions
 *
 *               The parts are saved in the order they are created.
 *
 *  @param  viewName  An optional human readable name for the part  (attribute: view)
 *  @param  compression     The compression style to use. ZIP is a good choice. PIZ (also lossless) can be better for size
 *                        for large images. B44 (lossy) is better for speedy random access into image subregions, and
 *                        for parts with lots of channels, only some of which are typically read.
 *                        Note: version 1 supports only axr_compression_zip. Other choices will be mapped to zip.
 *  @param  dataWindow       The pixel coordinates for the data window
 *  @param  displayWindow  The pixel coordinates for the display window
 *  @param  lineOrder            The ordering of the scanlines within the part. axr_line_order_increasing_y is typical
 *  
 *  @param  pixelAspectRatio  The aspect ratio of the pixels. Typically 1
 *  @param  screenWindowCenterX  The center of the screen window in the x axis
 *  @param  screenWindowCenterY  The center of the screen window in the y axis
 *  @param  screenWindowWidth       The width of the screen window
 *  @param  xDensity                           The intended resolution of the image in pixels per inch. The vertical density is xDensity * pixelAspectRatio
 *  @param  tileWidth       The width of the tile. Will be ignored for axr_compression_none/zips/rle, in which case it is the rip width. If 0, AppleEXR will pick a value.
 *  @param  tileHeight     The height of the tile. Will be ignored for axr_compression_none/zips/rle, in which case it is 1. If 0, AppleEXR will pick a value.
 *  @param  levelMode       {ripmap/mipmap/one_level} | {round_up / round_down}.   Must be axr_level_mode_one_level for axr_compression_none/zips/rle
 *  @return  on success, a valid axr_part_t owned by the encoder. You should not release this object unless you have also retained it. */
__nonnull axr_part_t axr_encoder_append_part(__nonnull axr_encoder_t encoder,
                                             const char * __nonnull  partName,      // "name" attribute
                                             const char * __nullable viewName,      // "view" attribute
                                             axr_compression_t compression,
                                             axr_box2i_t dataWindow,
                                             axr_box2i_t displayWindow,
                                             axr_line_order_t lineOrder,
                                             float pixelAspectRatio,
                                             float screenWindowCenterX,
                                             float screenWindowCenterY,
                                             float screenWindowWidth,
                                             float xDensity _AXR_DEFAULT(72.0f),
                                             uint32_t tileWidth _AXR_DEFAULT(0),
                                             uint32_t tileHeight _AXR_DEFAULT(0),
                                             axr_level_mode_t levelMode  _AXR_DEFAULT(axr_level_mode_one_level)
                                            ) AXR_RETURNS_INNER_POINTER AXR_AVAILABILITY_v1;

/*! @abstract Adjust the primaries used by the part
 *  @discussion  If parameters x or y are NULL, the chromaticities are inferred from the CGColorSpaceModel.
 *  Channel names are in part inferred from the colorspace model. The ColorSpace model must be set before
 *
 *  @param   part      The part
 *  @param   cgColorSpaceModel    The CGColorSpaceModel for the colorspace:
 *                                  kCGColorSpaceModelUnknown        Suitable when the image has only  non-color data
 *                                  kCGColorSpaceModelMonochrome  Grayscale only
 *                                  kCGColorSpaceModelRGB                RGB and YCbCr  {use corresponding RGB x, y}
 *                                  kCGColorSpaceModelXYZ                 XYZ      (pass NULL for x, y}
 *                              For this purpose, ignore alpha and other non-color channels.
 *  @param   whiteLuminance           1.0f is the default
 *  @param   x              {red, green, blue, white}   "little x" components of the chromaticities
 *  @param   y              {red, green, blue, white}   "little y" components of the chromaticities  */
void axr_part_set_chromaticities( __nonnull axr_part_t part,
                                  int32_t  cgColorSpaceModel,
                                  float whiteLuminance  _AXR_DEFAULT(1.0f),
                                  float x[__nullable 4] _AXR_DEFAULT(NULL),
                                  float y[__nullable 4] _AXR_DEFAULT(NULL)) AXR_AVAILABILITY_v1;

/*! @abstract Callback to free your data when AppleEXR is done with it
 *  @discussion  This callback is an opportunity for you to dispose of memory when AppleEXR is done with it.
 *               You may ignore it, release or free objects, and kick off another GCD task to the same.
 *               It is possible that your data destructor might be called reentrantly, if it is handed to
 *               AppleEXR many times. Use appropriate synchronization primitives to ensure your code in the
 *               destructor is thread safe and run on the main thread if necessary.  This destructor will be
 *               called even if data is NULL. Destructors are not guaranteed to be called in any particular order.
 *  @param  data            A pointer to your data to be freed / released
 *  @param  userInfo   This is extra information you can hand to yourself to help with the destruction of data.
 *                    It might be a type code, a size, or a pointer to some other data structure of yours, for example.
 *                    The definition of what this is is up to you. AppleEXR merely collects the value at the
 *                    time you hand it the destructor and hands it back to you when the destructor is called.*/
typedef void (*axr_user_data_destructor_t)(const void * __nullable data, uintptr_t userInfo);
    
/*! @abstract add a preview image to the axr_part_t
 *  @discussion  There can only be 1 preview image.
 *  @param  part  The part
 *  @param  data    A pointer to the top left corner of a RGBA unorm8 image
 *  @param  width  The number of pixel columns in the preview image
 *  @param  height The number of pixel rows in the preview image
 *  @param  rowBytes The number of bytes from one row to the next in the same column of the preview image
 *  @param  userData   This is handed back to you as the second parameter of dataDestructor.  AppleEXR doesn't know what it is.
 *  @param  dataDestructor  An optional callback you can use to free the data pointed to by the data parameter.  Will be called even if data is NULL.
 *                         Note that dataDestructor may be called before axr_part_set_preview returns.  */
void axr_part_set_preview( __nonnull axr_part_t part,
                            const uint8_t * __nullable data,
                            uint32_t width,
                            uint32_t height,
                            size_t rowBytes,
                            uintptr_t userData,
                            __nullable axr_user_data_destructor_t dataDestructor) AXR_AVAILABILITY_v1;
    
/*! @abstract   Add an arbitrary EXR attribute to an part
 *  @discussion All EXR attributes have a attribute name and a type code  See "Predefined Attribute Types" https://www.openexr.com/documentation/openexrfilelayout.pdf
 *              EXR uses little endian byte order for types larger than one byte. Your application needs to apply this permutation before handing the data to AppleEXR.
 *              All data is packed, with no byte padding for alignment.  No effort is made to avoid creating duplicate attributes. This interface is not intended as a method
 *              to change data passed to axr_part_create()
 *  @param  part   The part to add the attribute
 *  @param  attributeName   A '\0' terminated C-string. See URL in discussion for attribute names.
 *  @param  attributeType   A '\0' terminated C string. See URL in discussion for type names.
 *  @param  attributeSize   The number of bytes in the buffer pointed to by attributeBytes
 *  @param  attributeBytes  A pointer to the data to be store in the EXR metadata. Data should be byte-swapped to little endian order. */
axr_error_t axr_part_append_attribute( __nonnull axr_part_t part,
                                       const char * __nonnull attributeName,
                                       const char * __nonnull attributeType,
                                       size_t attributeSize,
                                       const uint8_t * __nonnull attributeBytes,
                                       uintptr_t userData,
                                       __nullable axr_user_data_destructor_t dataDestructor ) AXR_AVAILABILITY_v1;

/*! @abstract   Create a CGColorSpaceRef to match the image content to be written to a axr_part_t
 *  @discussion Your application is responsible for converting  image pixels to this colorspace before handing
 *              it to AppleEXR.  Please see vImageConvert_AnyToAny for one way to do this.
 *  @param      part  The part to contain the data
 *  @param      flags  Flags to control the operation
 *  @param      options  A set of optional options to add to the colorspace you get back
 *  @return     A CGColorSpaceRef. Make sure to call CFRelease() on it when you no longer need it.     */
void * __nullable /*CGColorSpaceRef*/
axr_part_create_colorspace( __nonnull axr_part_t part,
                            axr_flags_t flags,
                            void * __nullable /* CFDictionaryRef */ options _AXR_DEFAULT(NULL)) AXR_AVAILABILITY_v1;
    
/*! @abstract  Get the number of rip levels in X and Y dimensions */
axr_size_t axr_part_get_rip_level_counts( __nonnull axr_part_t part );
    
/*! @abstract called to read more pixel data from your data store
 *  @discussion  The callback will not be called until after your application calls axr_encoder_compress
 *                When possible, the data copied to buf should be materialized  lazily to avoid using too much memory.
 *  @param buf    The location to copy the data to
 *  @param size   The number of bytes to copy
 *  @param levelIndex   This is the mip level. If it is a rip level, then ripLevelX = levelIndex % ripLevelCountX.  ripLevelY = levelIndex / ripLevelCountX.
 *  @param userInfo  The userInfo pointer passed to axr_data_provider_create. This is your object. AppleEXR doesn't know what it is.
 *  @return  The number of bytes copied in.   If there is an error, return one of the following error codes:
 *              0:   The read operation is suspended. Do no further encoding.
 *              -1:  There is no more data
 *              -2:  An unrecoverable error occurred. */
typedef long (*axr_data_provider_read_t)( void * __nonnull buf,
                                          size_t size,
                                          unsigned long levelIndex,
                                          axr_size_t levelSize,
                                          uintptr_t userInfo ) AXR_AVAILABILITY_v1;

/*! @abstract   Called when the encode operation described by axr_part_append_channels is finished.
 *  @discussion This provides you with an opportunity to destroy or release storage used during encode operations.
 *              The callback will not be called until sometime after your application calls axr_encoder_compress
 *  @param userInfo  The userInfo pointer passed to axr_data_provider_create. This is your object. AppleEXR doesn't know what it is. */
typedef void (*axr_data_provider_release_t)( uintptr_t userInfo ) AXR_AVAILABILITY_v1;
    

    
/*! @abstract Append channels to the part
 *  @discussion You may append channels in sets of 1, 2 or 4.  Channels added together will appear
 *              consecutively in the file.  Note that EXR files with too many channels per part are
 *              inefficient to read.  Consider making a file with just a few channels per part and many parts.
 *              Channels that are read together must be stored together in the same part and should
 *              share the same layerName.  If adding more than one channel at a time with axr_part_append_channels
 *              your application will later be asked to produce the channels together as a single pixel buffer,
 *              with channels interleaved as {RGBArgbaRGBArgba...} rather than {RRRRR...gggggg...BBBBBB...aaaaa...}
 *              If your content is planar in nature, add the channels one at a time.
 *
 *              If multiple calls to axr_part_append_channels are used to encode a part, or there are multiple parts,
 *              the encoder may work on multiple parts or channel groups concurrently on different threads at the same
 *              time. Your readFunc/releaseFunc callbacks must take appropriate steps to ensure this does
 *              not cause problems for your internal application state. If this can not be accomplished, you may
 *              pass axr_flags_disable_multithreading to axr_encoder_compress to force single threaded operation.
 *              A significant performance penalty may apply in such cases, so it is preferred that your application strive
 *              toward thread safety.  Data access within a particular channel group (as delineated by each separate call to
 *              axr_part_append_channels) is serialized.
 *
 *  @param   part  The part to append the channels
 *  @param   layerName  A name prefix to prepend to the channels to help group them.  The
 *                     name may be reused for multiple calls to axr_part_append_channels
 *                     with the same part. The channels will all appear to be part of the same
 *                     layer.
 *  @param   channelType  The data type shared by the channels appended by this call
 *  @param   xSampling      The sampling rate in the x dimension.        Values > 1 indicate downsampled channels
 *  @param   ySampling      The sampling rate in the y dimension.        Values > 1 indicate downsampled channels
 *  @param   perceptuallyLinear   True indicates the data is perceptually linear, such as L*a*b* / YCbCr chroma or hue.  Unless you are sure, false is recommended.
 *  @param   channelList   A pointer to a flat array of axr_channel_type_t. The number of elements in the array is given by channel count
 *                       The ordering of the channels should match the ordering of the channels in the interleaved pixel buffer in memory.
 *  @param   channelNames An optional list of optional names. If the name is unavailable, AppleEXR will use the default name for the channel type
 *                        given by the channelList parameter. Channel names are required for  axr_channel_type_unknown channels.
 *  @param   channelCount  The number of channels to be added together. Must be 1, 2, or 4
 *  @param   readFunc        This is called to get the next N bytes of the input source.  It should be a 1, 2, or 4 channel interleaved image with
 *                       properties conforming to those above
 *  @param   releaseFunc  This is called when the encoder is done with the pixel data in the image. Your application should release
 *                       storage associated with the pixel buffer and the write operation at this time.
 *  @param   userInfo         an optional extra piece of application state. It is passed unmodified to readFunc and doneFunc. This can be a selector,
 *                       an opaque pointer to your object, a file handle, etc.  AppleEXR doesn't know this integer is and does not try to interact
 *                       with it.  If the same object can be used by multiple encoders concurrently, your application must take steps to make sure
 *                       its operation is thread safe. For example, there should likely be a separate copy of the buffer position offset implicitly
 *                       used by readFunc per instance. */
void axr_part_append_channels( __nonnull axr_part_t part,
                               const char * __nullable layerName,
                               axr_type_t channelType,
                               uint32_t xSampling,
                               uint32_t ySampling,
                               bool perceptuallyLinear,
                               const axr_channel_type_t channelList[__nonnull],
                               const char * __nullable channelNames[__nullable],
                               unsigned long channelCount,
                               axr_data_provider_read_t __nonnull readFunc,
                               axr_data_provider_release_t __nullable releaseFunc,
                               uintptr_t userInfo );
                             
    
/*! @abstract   AppleEXR doesn't know what this is.  This is a pointer to your storage object that will be handed back to you as needed */
typedef void * axr_encoder_storage_t;
    
/*! @abstract Move the current file position to byte_position bytes from the start of the file
 *  @discussion This will be called with offset 0 once before any writes are performed, and then as needed after that.
 *  @param storage                Your storage object
 *  @param bytePosition     This is the byte offset into the storage. If axr_encoder_seek_end is passed, the byte after the last byte written to the file is desired (e.g. fseek(file, 0, SEEK_END))
 *  @return return 0 for success, non-zero for failure  */
typedef long (*axr_encoder_seek_proc_t)( __nullable axr_encoder_storage_t storage, unsigned long bytePosition );
static const unsigned long axr_encoder_seek_end = -1UL;

/*! @abstract write bytes to the storage starting at the current file location and extending size bytes
 *  @param storage  A pointer to your storage object
 *  @param bytes      A pointer to the start of the byte stream to write
 *  @param  size     The number of bytes in the region of memory pointed to by bytes
 *  @return  The number of bytes written, or a negative number if an error occurred*/
typedef long (*axr_encoder_write_proc_t)( __nullable axr_encoder_storage_t storage, const uint8_t * __nonnull bytes, size_t size  );
    
/*! @abstract Called once  when the encoding is done.
 *  @param storage      A pointer to your storage object.  It should be freed or released as appropriate. */
typedef void (*axr_encoder_storage_destroy_proc_t)( __nullable axr_encoder_storage_t storage );

/*! @abstract Serialize the axr_encoder_t to a stream of bytes decodable as a OpenEXR file
 *  @discussion  if no seekProc is provided, AppleEXR will have to compress the entire file before any of it can be written.  This will
 *               result in much, much more memory usage.
 *
 *               Each tile EXR file contains all planes of the part. As AppleEXR encodes the tile, it will read a tile row sized
 *               chunk of pixels from each of your layer data providers in turn. This will cause all of your layer data providers
 *               to be open concurrently. They should attempt to minimize the amount of persistent storage they keep around
 *               between calls, if possible. 
 *
 *               *** Once axr_encoder_compress() returns, the object can not be reused. It should be released. ***
 *               This is because the axr_user_data_destructor_ts have been called and your resources no longer available.
 *               Since the encoder consumes itself as part of the compression operation (releases your resources to save
 *               memory), it should not be called reentrantly. You may call it from multiple threads as long as care is taken
 *               to protect the object with appropriate locks. While the object is likely to be internally multithreaded, calls to
 *               your applications storage object will be serialized.
 *
 *               Flags:
 *                  axr_flags_disable_multithreading — Pass this flag to force the compressor to run synchronously.
 *  @param encoder           A valid AppleEXR encoder
 *  @param storage           A pointer to your storage object.  AppleEXR doesn't know what this object is and will interact with it only through writeProc, seekProc, and destroyProc
 *  @param writeProc       A pointer to a function to write bytes to the storage
 *  @param seekProc         A optional pointer to a function to move the storage index around. If NULL, encoding will consume much more system memory.
 *  @param destroyProc  An optional pointer to a function to destroy or release the storage object
 *  @param flags            Flags to control the operation of axr_encoder_compress
 *  @return  On success, axr_error_success.  Otherwise, an error code is returned */
axr_error_t axr_encoder_compress( __nonnull axr_encoder_t encoder,
                                  __nonnull axr_encoder_storage_t storage,
                                  __nonnull axr_encoder_write_proc_t writeProc,
                                  __nullable axr_encoder_seek_proc_t seekProc,
                                  __nullable axr_encoder_storage_destroy_proc_t destroyProc,
                                  axr_flags_t flags ) AXR_AVAILABILITY_v1;
    
#if defined __cplusplus
    }
#endif

#endif /* AppleEXR_h */
