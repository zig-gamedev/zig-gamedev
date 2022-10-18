/*      MDItem.h
        Copyright (c) 2003-2019, Apple Inc. All rights reserved.
*/

/*!
        @header MDItem
        MDItem is a CF-compliant object, and follows the CF conventions,
        and can be used with the CF polymorphic functions, like CFRetain().

        An MDItemRef represents a file in the Metadata database, and the
        metadata associated with the file.

        Undefined Behavior
                For functions which take an MDItemRef parameter, if this
                parameter is not a valid MDItemRef, the behavior is
                undefined. NULL is not a valid MDItemRef.

                For functions which take CF*Ref parameters, such as
                CFStringRef and CFArrayRef, if this parameter is not a
                valid CF object of the correct type, the behavior is
                undefined. NULL is not a valid CF*Ref.
                A NULL CF*Ref parameter value is allowed only where
                explicitly noted below.

                Additional constraints or allowed values on parameters
                are noted with the specific functions.

*/

#if !defined(__MDITEM__)
#define __MDITEM__ 1

#include <CoreFoundation/CoreFoundation.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4

#if !defined(MD_EXPORT)
        #if defined(macintosh) && defined(__MWERKS__)
                #define MD_EXPORT __declspec(export) extern
        #else
                #define MD_EXPORT extern
        #endif
        #define MD_AVAIL API_AVAILABLE( macos(10.4) ) API_UNAVAILABLE( ios, tvos, watchos )
        #define MD_AVAIL_LEOPARD API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos )
        #if defined(__cplusplus)
                #define MD_BEGIN_C_DECLS extern "C" {
                #define MD_END_C_DECLS   }
        #else
                #define MD_BEGIN_C_DECLS
                #define MD_END_C_DECLS
        #endif
        #if defined(macintosh) && defined(__MWERKS__)
                #define MD_DEPRECATED
        #else
                #define MD_DEPRECATED __attribute__ ((deprecated))
        #endif
#endif

CF_IMPLICIT_BRIDGING_ENABLED
MD_BEGIN_C_DECLS


/*!
        @typedef MDItemRef
        This is the type of a reference to MDItems.
*/
typedef struct CF_BRIDGED_TYPE(id) __MDItem * MDItemRef;

/*!
        @function MDItemGetTypeID
        Returns the type identifier of all MDItem instances.
*/
MD_EXPORT CFTypeID MDItemGetTypeID(void) MD_AVAIL;

/*!
        @function MDItemCreate
        Returns an metadata item for the given path.
        @param allocator The CFAllocator which should be used to allocate
                memory for the query and its sub-storage. This
                parameter may be NULL in which case the current default
                CFAllocator is used.
        @param path A path to the file for which to create the MDItem.
                [[Currently, the path must exist. MDItemRefs may or
                may not be uniqued. Use CFEqual() to compare them.]]
        @result An MDItemRef, or NULL on failure.
*/
MD_EXPORT MDItemRef MDItemCreate(CFAllocatorRef allocator, CFStringRef path) MD_AVAIL;

/*!
 @function MDItemCreateWithURL
 Returns an metadata item for the given path.
 @param allocator The CFAllocator which should be used to allocate
 memory for the query and its sub-storage. This
 parameter may be NULL in which case the current default
 CFAllocator is used.
 @param url A url to the file for which to create the MDItem.
 [[Currently, the file must exist. MDItemRefs may or
 may not be uniqued. Use CFEqual() to compare them.]]
 @result An MDItemRef, or NULL on failure.
 */
MD_EXPORT MDItemRef MDItemCreateWithURL(CFAllocatorRef allocator, CFURLRef url) API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDItemsCreateWithURLs
 Returns metadata items for the given urls.
 @param allocator The CFAllocator which should be used to allocate
 memory for the array. This parameter may be NULL in which case the current default
 CFAllocator is used.
 @param urls A CFArray of urls to the file for which to create the MDItem.
 @result A CFArrayRef of MDItemRefs, or NULL on failure. Missing items will have kCFNull entries in the result array.
 */
MD_EXPORT CFArrayRef MDItemsCreateWithURLs(CFAllocatorRef allocator, CFArrayRef urls) API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
        @function MDItemCopyAttribute
        Returns the value of the given attribute for the item.
        @param item The item to be interrogated.
        @param name The name of the desired attribute.
        @result A CFTypeRef, or NULL on failure, or if the attribute
                does not exist, of if the attribute is not readable.
*/
MD_EXPORT CFTypeRef MDItemCopyAttribute(MDItemRef item, CFStringRef name) MD_AVAIL;

/*!
        @function MDItemCopyAttributes
        Returns the values of the given attributes for the item.
        @param item The item to be interrogated.
        @param names A CFArray of the names of the desired attributes.
        @result A CFDictionary where the keys are the attribute names,
                and the values are the attribute values, or NULL on
                failure. If an attribute does not exist, or is
                unreadable, there will be no key-value pair for it
                in the dictionary.
*/
MD_EXPORT CFDictionaryRef MDItemCopyAttributes(MDItemRef item, CFArrayRef names) MD_AVAIL;

/*!
        @function MDItemCopyAttributeList
        Returns the values of the given attributes for the item.
        @param item The item to be interrogated.
        @param ... A comma-separated varargs list of the string
                attribute names.
        @result A CFDictionary where the keys are the attribute names,
                and the values are the attribute values, or NULL on
                failure. If an attribute does not exist, or is
                unreadable, there will be no key-value pair for
                it in the dictionary.
*/
MD_EXPORT CFDictionaryRef MDItemCopyAttributeList(MDItemRef item, ... /* CFStringRef names */) MD_AVAIL;

/*!
        @function MDItemCopyAttributeNames
        Returns an array of the attribute names existing in the item.
        @param item The item to be interrogated.
        @result A CFArray of CFString attribute names, or NULL on
                failure.
*/
MD_EXPORT CFArrayRef MDItemCopyAttributeNames(MDItemRef item) MD_AVAIL;

/*!
 @function MDItemsCopyAttributes
 Returns metadata for the given items.
 @param items A CFArray of MDItemRefs to items for which to fetch data
 @param names A CFArray of attribute names for which to fetch data. 
				The attribute names are CFStrings
 @result A CFArrayRef, or NULL on failure. Each entry in the array is either kCFNull, 
  if the item is not accessible, or a CFArray of attribute values. 
  If an attribute is not available, there will be a kCFNull in its slot in the nested array.
 */
MD_EXPORT CFArrayRef MDItemsCopyAttributes(CFArrayRef items, CFArrayRef names) API_AVAILABLE( macos(10.12) ) API_UNAVAILABLE( ios, tvos, watchos );

/* List of well-known attributes */

/*!
   @constant kMDItemAttributeChangeDate
   This is the date that the last metadata attribute was changed.

   @constant kMDItemContentType
   UTI Type pedigree for a file for example a jpeg file will have the
   following ItemContentType public.jpeg/public.image/public.data
   the kMDItemContentType is set by the sniffer, any changes to this
   value will get over written by the sniffer when the contents of
   this MDItemRef changes. Type is a CFStringRef

   @constant kMDItemKeywords
   Represents keywords associated with this particular
   MDItemRef. Example Keywords might be Birthday,Important etc. Type
   is a CFArray of CFStrings

   @constant kMDItemTitle
   The title of this particular MDItemRef. Title of the document, or
   it could be the title of this mp3 or a subject of a mail
   message. Type is a CFString

   @constant kMDItemAuthors
   The list of author/authors that has worked on this file. There
   could be 0 or more authors of a particular file. The order of the
   authors in the array is preserved, but is not intended to represent
   the main author or relative importance of the authors. Type is a
   CFArray of CFStrings.
 
   @constant kMDItemEditors
   The list of editor/editors that has worked on this file. There
   could be 0 or more editors of a particular file. The order of the
   editors in the array is preserved, but is not intended to represent
   the main editor or relative importance of the editors. Type is a
   CFArray of CFStrings.
 
   @constant kMDItemParticipants
   The list of people who are visible in an image or movie or
   written about in a document. Type is CFArray of CFStrings.

   @constant kMDItemProjects
   The list of projects etc that this file is part of. For example if
   you were working on a movie, all of the movie files could be marked
   as belonging to the project "My movie" then a query could be done
   kMDItemProjects = "My movie" and all of the related files would
   show up. Type is a CFArray of CFStrings

   @constant kMDItemComment
   This is a comment related to a file, and can be any random
   string. Type is a CFString

   @constant kMDItemCopyright
   This is the copyright of the content. Type is a CFString
   
   @constant kMDItemDownloadedDate
   This is the date that the file was last downloaded / received.

   @constant kMDItemWhereFroms
   This attribute indicates where the item was obtained from.
   Examples:
   - downloaded file may refer to the site they were downloaded from,
    the refering URL, etc
  - files reveived by email may indicate who sent the file, the
    message subject, etc
   Type is a CFArray of CFStrings

   @constant kMDItemLastUsedDate
   This is the date that the file was last used, this field is updated
   by LaunchServices everytime a file is opend by double clicking or
   by asking LaunchServices to open a file. Type is a CFDate

   @constant kMDItemContentCreationDate
   This is the date that the contents of the file were created,
   has an application specific semantic.
   Type is a CFDate.

   @constant kMDItemContentModificationDate
   This is the date that the contents of the file were last
   modified, has an application specific semantic. For example an
   application can use this field to mark when the file was last
   modified, this date is not related to the file system modification
   date, but can be independent of that. This allows tracking of the
   last time the content was modified irrespective of the last time the
   file was modified. Type is a CFDate.
   
   @constant kMDItemDateAdded
   This is the date that the file was moved into the current location.
   Not all files will have this attribute.  Not all file systems support
   this attribute.

   @constant kMDItemDurationSeconds
   This is the duration, in seconds, of the content of the file (if
   appropriate).  A value of 10.5 represents media whose content is
   10 and 1/2 seconds long.  Type is a CFNumber.

   @constant kMDItemContactKeywords
   A list of contacts that are somehow associated with this document,
   beyond what is captured as Author.

   @constant kMDItemVersion
   A version number for this item. Type is a CFString

   @constant kMDItemPixelHeight
   The height of the document in pixels (ie Image height or Video frame height)

   @constant kMDItemPixelWidth
   The width of the document in pixels (ie Image width or Video frame width)

   @constant kMDItemPixelCount
   The total number of pixels in the document.  Type is a CFNumber.

   @constant kMDItemColorSpace
   What color space model is this document following
   (For example, are examples "RGB", "CMYK", "YUV", "YCbCr")

   @constant kMDItemBitsPerSample
   Number of bits per sample
   For example bit depth of an image (8-bit, 16-bit etc..) or bit
   depth per audio sample of uncompressed audio data (8, 16, 24, 32,
   64, etc..)

   @constant kMDItemFlashOnOff
   Indicates if the flash was used to take the picture. 0 means flash did not fire

   @constant kMDItemFocalLength
   The actual focal length of the lens in mm.

   @constant kMDItemAcquisitionMake
   Device make that was used to acquire this document

   @constant kMDItemAcquisitionModel
   Device model that was used to acquire this document

   @const kMDItemISOSpeed
   The ISO Speed the camera was set to when the image was
   taken. Examples are 100, 200, 400, etc.

   @const kMDItemOrientation
   The orientation of the data. Values are 0 is "Landscape" or 1 is "Portrait"

   @const kMDItemLayerNames
   The names of the various layers in the file

   @const kMDItemWhiteBalance
   The white balance setting of the camera when the image was
   acquired. 0 is auto white balance and 1 is manual

   @const kMDItemAperture
   The size of the lens aperture as a log-scale APEX value
   when the image was acquired.

   @const kMDItemProfileName
   Name of the color profile used for the image

   @const kMDItemResolutionWidthDPI
   Resolution width of this image in DPI

   @const kMDItemResolutionHeightDPI
   Resolution height of this image in DPI

   @const kMDItemExposureMode
   Mode that was used for the exposure. 0 is auto exposure, 1 is
   manual, and 2 is auto bracket.

   @const kMDItemExposureTimeSeconds
   Time that the lens was open during exposure

   @const kMDItemLensModel
   Lens model that was used to acquire this document
 
   @const kMDItemEXIFVersion
   The verion of the EXIF header that was used to generate the metadata

   @const kMDItemEXIFGPSVersion
   The version of GPSInfoIFD header that was used to generate the metadata
 
   @const kMDItemCodecs
   The codecs used to encode/decode the media

   @const kMDItemMediaTypes
   Media types present in the content

   @const kMDItemStreamable
   Whether the content is prepared for streaming

   @const kMDItemTotalBitRate
   The total byte rate (audio & video combined) of the media

   @const kMDItemVideoBitRate
   The video byte rate

   @const kMDItemAudioBitRate
   The audio byte rate

   @const kMDItemDeliveryType
   Delivery type Fast start or RTSP

   @constant kMDItemAlbum
   The title for a collection of media. This is analagous to a record album,
   or photo album whichs are collections of audio or images. Type is a CFString.

   @constant kMDItemHasAlphaChannel
   Boolean indicating if this image file has an alpha channel. Type is
   a CFBoolean.

   @constant kMDItemRedEyeOnOff
   Indicates if the flash was used to take the picture. 0 means no
   red-eye reduction mode or unknown. 1 means red-eye reduction
   supported.

   @const kMDItemMeteringMode
   The metering mode (Unknown, Average, CenterWeightedAverage, Spot,
   MultiSpot, Pattern, Partial)

   @const kMDItemMaxAperture
   The smallest F number of the lens. The unit is the APEX
   value. Ordinarily it is given in the range of 00.00 to 99.99.

   @const kMDItemFNumber
   The focal length of the lens divided by the diameter of the aperture
   when the image was acquired.

   @const kMDItemExposureProgram
   The class of the program used by the camera to set exposure when
   the picture is taken (Manual, Normal, Aperture priority, ...)

   @const kMDItemExposureTimeString
   The time  of the exposure.

   @const kMDItemHeadline
   A publishable entry providing a synopsis of the contents of the
   objectdata.

   @const kMDItemInstructions
   Other editorial instructions concerning the use of the objectdata,
   such as embargoes and warnings.

   @const kMDItemCity
   Identifies city of objectdata origin according to guidelines
   established by the provider.

   @const kMDItemStateOrProvince
   Identifies Province/State of origin according to guidelines
   established by the provider.

   @const kMDItemCountry
   Provides full, publishable, name of the country/primary location
   where the intellectual property of the objectdata was created,
   according to guidelines of the provider.
 
 @const kMDItemEXIFGPSVersion
 The version of GPSInfoIFD in EXIF used to generate the metadata.
 
 @const kMDItemAltitude
 The altitude of the item in meters above sea level, expressed 
 using the WGS84 datum.  Negative values lie below sea level.
 
 @const kMDItemLatitude
 The latitude of the item in degrees north of the equator, expressed
 using the WGS84 datum.  Negative values lie south of the equator.
 
 @const kMDItemLongitude
 The longitude of the item in degrees east of the prime meridian,
 expressed using the WGS84 datum.  Negative values lie west of the prime meridian.
 
 @const kMDItemTimestamp
 The timestamp on the item.  This generally is used to indicate the time at
 which the event captured by the item took place.
 
 @const kMDItemSpeed
 The speed of the item, in kilometers per hour.
 
 @const kMDItemGPSTrack
 The direction of travel of the item, in degrees from true north.
 
 @const kMDItemImageDirection
 The direction of the item's image, in degrees from true north.
 
 @const kMDItemNamedLocation
 The name of the location or point of interest associated with the item.
 The name may be user provided.
*/

MD_EXPORT const CFStringRef     kMDItemAttributeChangeDate MD_AVAIL;       // CFDate
MD_EXPORT const CFStringRef     kMDItemContentType MD_AVAIL;               // CFString
MD_EXPORT const CFStringRef     kMDItemContentTypeTree MD_AVAIL_LEOPARD;   // CFArray of CFStringRef
MD_EXPORT const CFStringRef     kMDItemKeywords MD_AVAIL;                  // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemTitle MD_AVAIL;                     // CFString
MD_EXPORT const CFStringRef     kMDItemAuthors MD_AVAIL;                   // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemEditors MD_AVAIL_LEOPARD;           // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemParticipants API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemProjects MD_AVAIL;                  // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemDownloadedDate API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFDate
MD_EXPORT const CFStringRef     kMDItemWhereFroms MD_AVAIL;                // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemComment MD_AVAIL;                   // CFString
MD_EXPORT const CFStringRef     kMDItemCopyright MD_AVAIL;                 // CFString
MD_EXPORT const CFStringRef     kMDItemLastUsedDate MD_AVAIL;              // CFDate
MD_EXPORT const CFStringRef     kMDItemContentCreationDate MD_AVAIL;       // CFDate
MD_EXPORT const CFStringRef     kMDItemContentModificationDate MD_AVAIL;   // CFDate
MD_EXPORT const CFStringRef     kMDItemDateAdded API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFDate
MD_EXPORT const CFStringRef     kMDItemDurationSeconds MD_AVAIL;           // CFNumber
MD_EXPORT const CFStringRef     kMDItemContactKeywords MD_AVAIL;           // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemVersion MD_AVAIL;                   // CFString

MD_EXPORT const CFStringRef     kMDItemPixelHeight MD_AVAIL;               // CFNumber
MD_EXPORT const CFStringRef     kMDItemPixelWidth MD_AVAIL;                // CFNumber
MD_EXPORT const CFStringRef     kMDItemPixelCount API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFNumber
MD_EXPORT const CFStringRef     kMDItemColorSpace MD_AVAIL;                // CFString
MD_EXPORT const CFStringRef     kMDItemBitsPerSample MD_AVAIL;             // CFNumber
MD_EXPORT const CFStringRef     kMDItemFlashOnOff MD_AVAIL;                // CFBoolean
MD_EXPORT const CFStringRef     kMDItemFocalLength MD_AVAIL;               // CFNumber
MD_EXPORT const CFStringRef     kMDItemAcquisitionMake MD_AVAIL;           // CFString
MD_EXPORT const CFStringRef     kMDItemAcquisitionModel MD_AVAIL;          // CFString
MD_EXPORT const CFStringRef     kMDItemISOSpeed MD_AVAIL;                  // CFNumber
MD_EXPORT const CFStringRef     kMDItemOrientation MD_AVAIL;               // CFNumber
MD_EXPORT const CFStringRef     kMDItemLayerNames MD_AVAIL;                // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemWhiteBalance MD_AVAIL;              // CFNumber
MD_EXPORT const CFStringRef     kMDItemAperture MD_AVAIL;                  // CFNumber
MD_EXPORT const CFStringRef     kMDItemProfileName MD_AVAIL;               // CFString
MD_EXPORT const CFStringRef     kMDItemResolutionWidthDPI MD_AVAIL;        // CFNumber
MD_EXPORT const CFStringRef     kMDItemResolutionHeightDPI MD_AVAIL;       // CFNumber
MD_EXPORT const CFStringRef     kMDItemExposureMode MD_AVAIL;              // CFNumber
MD_EXPORT const CFStringRef     kMDItemExposureTimeSeconds MD_AVAIL;       // CFNumber
MD_EXPORT const CFStringRef     kMDItemEXIFVersion MD_AVAIL;               // CFString

MD_EXPORT const CFStringRef kMDItemCameraOwner API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );       // CFString
MD_EXPORT const CFStringRef kMDItemFocalLength35mm API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );   // CFNumber
MD_EXPORT const CFStringRef kMDItemLensModel API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );         // CFString

MD_EXPORT const CFStringRef     kMDItemEXIFGPSVersion MD_AVAIL_LEOPARD;    // CFString
MD_EXPORT const CFStringRef     kMDItemAltitude MD_AVAIL_LEOPARD;          // CFNumber
MD_EXPORT const CFStringRef     kMDItemLatitude MD_AVAIL_LEOPARD;          // CFNumber
MD_EXPORT const CFStringRef     kMDItemLongitude MD_AVAIL_LEOPARD;         // CFNumber
MD_EXPORT const CFStringRef     kMDItemSpeed MD_AVAIL_LEOPARD;             // CFNumber
MD_EXPORT const CFStringRef     kMDItemTimestamp MD_AVAIL_LEOPARD;         // CFDate
MD_EXPORT const CFStringRef     kMDItemGPSTrack MD_AVAIL_LEOPARD;          // CFNumber
MD_EXPORT const CFStringRef     kMDItemImageDirection MD_AVAIL_LEOPARD;    // CFNumber
MD_EXPORT const CFStringRef     kMDItemNamedLocation API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFString

MD_EXPORT const CFStringRef kMDItemGPSStatus API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );             // CFString
MD_EXPORT const CFStringRef kMDItemGPSMeasureMode API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );        // CFString
MD_EXPORT const CFStringRef kMDItemGPSDOP API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );                // CFNumber
MD_EXPORT const CFStringRef kMDItemGPSMapDatum API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );           // CFString
MD_EXPORT const CFStringRef kMDItemGPSDestLatitude API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );       // CFNumber
MD_EXPORT const CFStringRef kMDItemGPSDestLongitude API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );      // CFNumber
MD_EXPORT const CFStringRef kMDItemGPSDestBearing API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );        // CFNumber
MD_EXPORT const CFStringRef kMDItemGPSDestDistance API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );       // CFNumber
MD_EXPORT const CFStringRef kMDItemGPSProcessingMethod API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );   // CFString
MD_EXPORT const CFStringRef kMDItemGPSAreaInformation API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );    // CFString
MD_EXPORT const CFStringRef kMDItemGPSDateStamp API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );          // CFDate
MD_EXPORT const CFStringRef kMDItemGPSDifferental API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );        // CFNumber

MD_EXPORT const CFStringRef     kMDItemCodecs MD_AVAIL;                    // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemMediaTypes MD_AVAIL;                // CFArray of CFString
MD_EXPORT const CFStringRef     kMDItemStreamable MD_AVAIL;                // CFBoolean
MD_EXPORT const CFStringRef     kMDItemTotalBitRate MD_AVAIL;              // CFNumber
MD_EXPORT const CFStringRef     kMDItemVideoBitRate MD_AVAIL;              // CFNumber
MD_EXPORT const CFStringRef     kMDItemAudioBitRate MD_AVAIL;              // CFNumber
MD_EXPORT const CFStringRef     kMDItemDeliveryType MD_AVAIL;              // CFString

MD_EXPORT const CFStringRef     kMDItemAlbum MD_AVAIL;                     // CFString
MD_EXPORT const CFStringRef     kMDItemHasAlphaChannel MD_AVAIL;		   // CFBoolean
MD_EXPORT const CFStringRef     kMDItemRedEyeOnOff MD_AVAIL;               // CFBoolean
MD_EXPORT const CFStringRef     kMDItemMeteringMode MD_AVAIL;              // CFString
MD_EXPORT const CFStringRef     kMDItemMaxAperture MD_AVAIL;               // CFNumber
MD_EXPORT const CFStringRef     kMDItemFNumber MD_AVAIL;                   // CFNumber
MD_EXPORT const CFStringRef     kMDItemExposureProgram MD_AVAIL;           // CFString
MD_EXPORT const CFStringRef     kMDItemExposureTimeString MD_AVAIL;        // CFString

/* From IPTC */
MD_EXPORT const CFStringRef     kMDItemHeadline MD_AVAIL;                  // CFString
MD_EXPORT const CFStringRef     kMDItemInstructions MD_AVAIL;              // CFString
MD_EXPORT const CFStringRef     kMDItemCity MD_AVAIL;                      // CFString
MD_EXPORT const CFStringRef     kMDItemStateOrProvince MD_AVAIL;           // CFString
MD_EXPORT const CFStringRef     kMDItemCountry MD_AVAIL;                   // CFString

/*!
   @constant kMDItemHTMLContent
   Contains the HTML content of the document. Type is a CFString.
   This field is only used by Spotlight importers to return HTML contents of a file.  Except in special cases,
   this field is not a replacement for kMDItemTextContent which should still be returned.
   This field does not particpate in Spotlight queries.

   @constant kMDItemTextContent
   Contains the text content of the document. Type is a CFString.

   @constant kMDItemDisplayName
   This is the localized version of the LaunchServices call
   LSCopyDisplayNameForURL()/LSCopyDisplayNameForRef().
 
   @constant kMDItemFSName
   This is the file name of the MDItemRef. Type is a CFString

   @constant kMDItemPath
   This is the complete path to the MDItemRef. Type is a CFString.

   @constant kMDItemFSSize
   The total logical size of the file (data and resources) on disk in bytes. Type is a CFNumber.

   @constant kMDItemFSCreationDate
   This is the date that the file was created. Type is a CFDate.

   @constant  kMDItemFSContentChangeDate
   This is the date the the file content last changed. This is a CFDate.

   @constant kMDItemFSOwnerUserID
   User-id of owner of the file. Type is a CFNumber.

   @constant kMDItemFSOwnerGroupID
   Group-id of owner of the file. Type is a CFNumber.

   @constant kMDItemFSExists *** DEPRECATED ***
   Boolean indicating if this MDItem references a file that still
   exists. The file that the MDItem references might have been
   deleted. Type is a CFBoolean.

   @constant kMDItemFSIsReadable *** DEPRECATED ***
   Boolean indicating if this file is readable. Type is a CFBoolean.

   @constant kMDItemFSIsWriteable *** DEPRECATED ***
   Boolean indicating if this file is writable. Type is a CFBoolean.

   @constant kMDItemFSNodeCount
   Number of files in directory. Type is a CFNumber.

   @constant kMDItemFSHasCustomIcon
   Boolean indicating if this file has a custom icon. Type is a CFBoolean.

   @constant kMDItemFSIsExtensionHidden
   Boolean indicating if this file has its extension hidden. Type is a CFBoolean.

   @constant kMDItemFSIsStationery
   Boolean indicating if this file is stationery. Type is a CFBoolean.

   @constant kMDItemFSInvisible
   Boolean indicating if this file is visible. Type is a CFBoolean.

   @constant kMDItemFSLabel
   Number indicating which finder label is in use (0-7). Type is a CFNumber.

*/

MD_EXPORT const CFStringRef     kMDItemFSName MD_AVAIL;                  // CFString
MD_EXPORT const CFStringRef     kMDItemDisplayName MD_AVAIL;             // CFString
MD_EXPORT const CFStringRef     kMDItemPath MD_AVAIL;                    // CFString
MD_EXPORT const CFStringRef     kMDItemFSSize MD_AVAIL;                  // CFNumber
MD_EXPORT const CFStringRef     kMDItemFSCreationDate MD_AVAIL;          // CFDate
MD_EXPORT const CFStringRef     kMDItemFSContentChangeDate MD_AVAIL;     // CFDate
MD_EXPORT const CFStringRef     kMDItemFSOwnerUserID MD_AVAIL;           // CFNumber
MD_EXPORT const CFStringRef     kMDItemFSOwnerGroupID MD_AVAIL;          // CFNumber
MD_EXPORT const CFStringRef     kMDItemFSExists API_DEPRECATED("No longer supported", macos(10.4, 10.4)) API_UNAVAILABLE( ios, tvos, watchos );
MD_EXPORT const CFStringRef     kMDItemFSIsReadable API_DEPRECATED("No longer supported", macos(10.4, 10.4)) API_UNAVAILABLE( ios, tvos, watchos );
MD_EXPORT const CFStringRef     kMDItemFSIsWriteable API_DEPRECATED("No longer supported", macos(10.4, 10.4)) API_UNAVAILABLE( ios, tvos, watchos );
MD_EXPORT const CFStringRef     kMDItemFSHasCustomIcon MD_AVAIL;         // CFBoolean
MD_EXPORT const CFStringRef     kMDItemFSIsExtensionHidden MD_AVAIL;     // CFBoolean
MD_EXPORT const CFStringRef     kMDItemFSIsStationery MD_AVAIL;          // CFBoolean
MD_EXPORT const CFStringRef     kMDItemFSInvisible MD_AVAIL;             // CFBoolean
MD_EXPORT const CFStringRef     kMDItemFSLabel MD_AVAIL;                 // CFNumber
MD_EXPORT const CFStringRef     kMDItemFSNodeCount MD_AVAIL;             // CFNumber

MD_EXPORT const CFStringRef     kMDItemHTMLContent API_AVAILABLE( macos(10.11) ) API_UNAVAILABLE( ios, tvos, watchos );      // a Write-Only CFString
MD_EXPORT const CFStringRef     kMDItemTextContent MD_AVAIL;             // a Write-Only CFString

/*!
   @constant kMDItemAudioSampleRate
   The sample rate of the audio data contained in the file. The sample rate is a
   float value representing hz (audio_frames/second). For example: 44100.0, 22254.54.
   Type is a CFNumber (float).

   @constant kMDItemAudioChannelCount
   The number of channels in the audio data contained in the file. This item only represents
   the number of discreet channels of audio data found in the file. It does not indicate
   any configuration of the data in regards to a user's speaker setup.
   Type is a CFNumber (integer).

   @constant kMDItemTempo
   The tempo of the music contained in the audio file in Beats Per Minute.
   Type is a CFNumber (float).

   @constant kMDItemKeySignature
   The musical key of the song/composition contained in an audio file.
   For example: C, Dm, F#m, Bb. Type is a CFString.

   @constant kMDItemTimeSignature
   The time signature of the musical composition contained in the audio/MIDI file.
   For example: "4/4", "7/8". Type is a CFString.

   @constant kMDItemAudioEncodingApplication
   The name of the application that encoded the data contained in the audio file.
   Type is a CFString.

   @constant kMDItemComposer
   The composer of the song/composition contained in the audio file.
   Type is a CFString.

   @constant kMDItemLyricist
   The lyricist/text writer for song/composition contained in the audio file.
   Type is a CFString.

   @constant kMDItemAudioTrackNumber
   The track number of a song/composition when it is part of an album (kMDItemAlbum).
   Type is a CFNumber (integer).

   @constant kMDItemRecordingDate
   The recording date of the song/composition. This information differs from
   the kMDItemContentCreationDate attribute as it indicates the date that the
   'art' was created, in contrast to ContentCreationDate which for example, could indicate
   the creation date of an edited or 'mastered' version of the original art.
   Type is a CFDate.

   @constant kMDItemMusicalGenre
   The musical genre of the song/composition contained in the audio file.
   For example: Jazz, Pop, Rock, Classical. Type is a CFString.

   @constant kMDItemIsGeneralMIDISequence
   This attribute indicates whether the MIDI sequence contained in the file
   is setup for use with a General MIDI device. Type is a CFBoolean.

   @const kMDItemRecordingYear
   This attribute indicates what year the item was recorded on.
   Type is a CFNumber
*/

MD_EXPORT const CFStringRef     kMDItemAudioSampleRate MD_AVAIL;                 // CFNumber
MD_EXPORT const CFStringRef     kMDItemAudioChannelCount MD_AVAIL;               // CFNumber
MD_EXPORT const CFStringRef     kMDItemTempo MD_AVAIL;                           // CFNumber
MD_EXPORT const CFStringRef     kMDItemKeySignature MD_AVAIL;                    // CFString
MD_EXPORT const CFStringRef     kMDItemTimeSignature MD_AVAIL;                   // CFString
MD_EXPORT const CFStringRef     kMDItemAudioEncodingApplication MD_AVAIL;        // CFString
MD_EXPORT const CFStringRef     kMDItemComposer MD_AVAIL;                        // CFString
MD_EXPORT const CFStringRef     kMDItemLyricist MD_AVAIL;                        // CFString
MD_EXPORT const CFStringRef     kMDItemAudioTrackNumber MD_AVAIL;                // CFNumber
MD_EXPORT const CFStringRef     kMDItemRecordingDate MD_AVAIL;                   // CFDate
MD_EXPORT const CFStringRef     kMDItemMusicalGenre MD_AVAIL;                    // CFString
MD_EXPORT const CFStringRef     kMDItemIsGeneralMIDISequence MD_AVAIL;           // CFBoolean
MD_EXPORT const CFStringRef     kMDItemRecordingYear MD_AVAIL;                   // CFNumber

/*!
        @const kMDItemOrganizations
        Used to indicate company/Organization that created the document.
        Type is a CFArray of CFStrings.

        @const kMDItemLanguages
        Used to designate the languages of the intellectual content of the
        resource. Recommended best practice for the values of the Language
        element is defined by RFC 3066.
        Type is a CFArray of CFStrings.

        @const kMDItemRights
        Used to provide a link to information about rights held in and
        over the resource. Typically a Rights element will contain a
        rights management statement for the resource, or reference a
        service providing such information. Rights information often
        encompasses Intellectual Property Rights (IPR), Copyright, and
        various Property Rights. If the rights element is absent, no
        assumptions can be made about the status of these and other rights
        with respect to the resource. Type is a CFString type.

        @const kMDItemPublishers
        Used to designate the entity responsible for making the resource
        available. Examples of a Publisher include a person, an
        organization, or a service. Typically, the name of a Publisher
        should be used to indicate the entity. Type is a CFArray of CFStrings.

        @const kMDItemContributors
        Used to designate the entity responsible for making contributions
        to the content of the resource. Examples of a Contributor include
        a person, an organization or a service. Typically, the name of a
        Contributor should be used to indicate the entity. Type is a
        CFArray of CFStrings.

        @const kMDItemCoverage
        Used to designate the extent or scope of the content of the
        resource. Coverage will typically include spatial location (a
        place name or geographic co-ordinates), temporal period (a period
        label, date, or date range) or jurisdiction (such as a named
        administrative entity). Recommended best practice is to select a
        value from a controlled vocabulary, and that, where appropriate,
        named places or time periods be used in preference to numeric
        identifiers such as sets of co-ordinates or date ranges. Type is a
        CFString.

        @const kMDItemSubject
        Subject of the this item. Type is a CFString.

        @const kMDItemTheme
        Theme of the this item. Type is a CFString.
 
        @const kMDItemDescription
        An account of the content of the resource. Description may include
        but is not limited to: an abstract, table of contents, reference
        to a graphical representation of content or a free-text account of
        the content. Type is a CFString.

        @const kMDItemIdentifier
        Used  to reference to the resource within a given
        context. Recommended best practice is to identify the resource by
        means of a string or number conforming to a formal identification
        system. Type is a CFString.

        @const kMDItemAudiences
        A class of entity for whom the resource is intended or useful. A
        class of entity may be determined by the creator or the publisher
        or by a third party. Type is a  CFArray of CFString.
*/

MD_EXPORT const CFStringRef     kMDItemOrganizations MD_AVAIL;                    // CFArray of CFStrings
MD_EXPORT const CFStringRef     kMDItemLanguages MD_AVAIL;                        // CFArray of CFStrings
MD_EXPORT const CFStringRef     kMDItemRights MD_AVAIL;                           // CFString
MD_EXPORT const CFStringRef     kMDItemPublishers MD_AVAIL;                       // CFArray of CFStrings
MD_EXPORT const CFStringRef     kMDItemContributors MD_AVAIL;                     // CFArray of CFStrings
MD_EXPORT const CFStringRef     kMDItemCoverage MD_AVAIL;                         // CFArray of CFStrings
MD_EXPORT const CFStringRef     kMDItemSubject MD_AVAIL;                          // CFString
MD_EXPORT const CFStringRef     kMDItemTheme MD_AVAIL;                            // CFString
MD_EXPORT const CFStringRef     kMDItemDescription MD_AVAIL;                      // CFString
MD_EXPORT const CFStringRef     kMDItemIdentifier MD_AVAIL;                       // CFString
MD_EXPORT const CFStringRef     kMDItemAudiences MD_AVAIL;                        // CFArray of CFStrings


/*!
        @const kMDItemNumberOfPages
        Number of pages in the item. Type is a CFNumberRef

        @const kMDItemPageWidth
        Width in points (72 points per inch) of the document page
        (first page only for PDF's - other pages within the PDF may
        not be the same width). Type is a CFNumber.

        @const kMDItemPageHeight
        Height in points (72 points per inch) of the document page
        (first page only for PDF's - other pages within the PDF may
        not be the same height). Type is a CFNumber.

        @const kMDItemSecurityMethod
        Security (encryption) method used in the file, for a PDF will be one of:
        "Password Encrypted" or "None". Type is a CFStrings.

        @const kMDItemCreator
        Application used to create the document content (e.g. "Word",
        "Framemaker", etc.). Type is a CFStrings.

        @const kMDItemEncodingApplications
        Software used to convert the original content into a PDF stream
        (e.g. "Distiller", etc.). Type is a Array of CFStrings.

        @const kMDItemDueDate
        Date this item is due. Type is a CFDate.

        @const kMDItemStarRating
        User rate of this item like iTunes. Type is a CFNumber

        @const kMDItemPhoneNumbers
        Phone numbers for this item. Type is an Array of CFStrings.

        @const kMDItemEmailAddresses
        Email addresses for this item. Type is an Array of CFStrings.

        @const  kMDItemInstantMessageAddresses
        Instant message addresses for this item. Type is an Array of CFStrings.

        @const kMDItemKind
        Kind that this item represents. Type is a CFString.

        @const kMDItemRecipients
        This attribute indicates the recipients of this item. Type is a Array of CFStrings

        @const  kMDItemFinderComment
        These are the finder comments for this item. Type is a CFString.

        @const kMDItemFonts
        Array of font names used in the item. Attribute would store the Fonts
        full name, the postscript name or the font family name based on whats available.
        Type is an Array of CFStrings.
*/

MD_EXPORT const CFStringRef    kMDItemNumberOfPages MD_AVAIL;                    // CFNumber
MD_EXPORT const CFStringRef    kMDItemPageWidth MD_AVAIL;                        // CFNumber
MD_EXPORT const CFStringRef    kMDItemPageHeight MD_AVAIL;                       // CFNumber
MD_EXPORT const CFStringRef    kMDItemSecurityMethod MD_AVAIL;                   // CFNumber
MD_EXPORT const CFStringRef    kMDItemCreator MD_AVAIL;                          // CFString
MD_EXPORT const CFStringRef    kMDItemEncodingApplications MD_AVAIL;             // CFArray of CFStrings
MD_EXPORT const CFStringRef    kMDItemDueDate MD_AVAIL;                          // CFDate
MD_EXPORT const CFStringRef    kMDItemStarRating MD_AVAIL;                       // CFNumber
MD_EXPORT const CFStringRef    kMDItemPhoneNumbers MD_AVAIL;                     // CFArray of CFStrings
MD_EXPORT const CFStringRef    kMDItemEmailAddresses MD_AVAIL;                   // CFArray of CFStrings
MD_EXPORT const CFStringRef    kMDItemInstantMessageAddresses MD_AVAIL;          // CFArray of CFStrings
MD_EXPORT const CFStringRef    kMDItemKind MD_AVAIL;                             // CFStrings
MD_EXPORT const CFStringRef    kMDItemRecipients MD_AVAIL;                       // CFArray of CFStrings
MD_EXPORT const CFStringRef    kMDItemFinderComment MD_AVAIL;                    // CFString
MD_EXPORT const CFStringRef    kMDItemFonts MD_AVAIL;                            // CFArray of CFString


/*!
        @const kMDItemAppleLoopsRootKey
        Meta data attribute that stores the root note or tonic for the
        loop, and does not include the scale type. The root key is
        represented as follows: "C" "C#/Db" "D" "D#/Eb" "E" "F"
        "F#/Gb" "G" "G#/Ab" "A" "A#/Bb" "B" "NoKey"

        @const kMDItemAppleLoopsKeyFilterType
        Meta data attribute that stores key filtering information
        about a loop. Loops are matched against projects that often in
        a major or minor key. To assist users in identifying loops
        that will "fit" with their compositions, loops can be tagged
        with one of the following key filters: "AnyKey" "Minor"
        "Major" "NeitherKey" "BothKeys". AnyKey means that it fits
        with anything (whether in a major key, minor key or
        neither). Minor fits with compositions in a minor
        key. NeitherKey doesn't work well with compositions that are
        in major or minor key. BothKeys means it fits with major or
        minor key.

        @const kMDItemAppleLoopsLoopMode
        Meta data attribute that stores how a file should be
        played. Tagged files can either be loops or non-loops (e.g., a
        cymbal crash). "Looping" indicates if the file should be
        treated as a loop. "Non-looping" indicates the file should not
        be treated as a loop.

        @const kMDItemAppleLoopDescriptors
        Meta data attribute that stores multiple pieces of descriptive
        information about a loop. Besides genre and instrument, files
        can contain descriptive information that help users in
        refining searches. A file can have multiple descriptors
        associated with them, though they come in pairs of antonyms
        (e.g., "Acoustic" and "Electric"). A file can have zero or
        more descriptors.


        @const kMDItemMusicalInstrumentCategory
        Meta data attribute that stores the category of
        instrument. Files should have an instrument associated with
        them ("Other Instrument" is provided as a catch-all). For some
        categories, like "Keyboards" there are instrument names which
        provide a more detailed instrument definition (e.g., Piano,
        Organ, etc.)

        @const kMDItemMusicalInstrumentName
        Meta data attribute that stores the name of instrument
        (relative to the instrument category) Files can have an
        instrument name associated with them if they have certain
        instrument categories (e.g., the category Percussion has
        multiple instruments, including Conga and Bongo).

        @const kMDItemCFBundleIdentifier
        If this item is a bundle, then this is the CFBundleIdentifier
*/

MD_EXPORT const CFStringRef    kMDItemAppleLoopsRootKey MD_AVAIL;                // CFString
MD_EXPORT const CFStringRef    kMDItemAppleLoopsKeyFilterType MD_AVAIL;          // CFString
MD_EXPORT const CFStringRef    kMDItemAppleLoopsLoopMode MD_AVAIL;               // CFString
MD_EXPORT const CFStringRef    kMDItemAppleLoopDescriptors MD_AVAIL;             // CFArray of CFStrings
MD_EXPORT const CFStringRef    kMDItemMusicalInstrumentCategory MD_AVAIL;        // CFString
MD_EXPORT const CFStringRef    kMDItemMusicalInstrumentName MD_AVAIL;            // CFString

MD_EXPORT const CFStringRef    kMDItemCFBundleIdentifier MD_AVAIL_LEOPARD;       // CFString
MD_EXPORT const CFStringRef    kMDItemSupportFileType API_DEPRECATED("No longer supported", macos(10.5, 10.5)) API_UNAVAILABLE( ios, tvos, watchos );          // CFArray of CFStrings

/*!
        @const kMDItemInformation
        Information about the item

        @const kMDItemDirector
        Director of the movie

        @const kMDItemProducer
        Producer of the content

        @const kMDItemGenre
        Genre of the movie

        @const kMDItemPerformers
        Performers in the movie

        @const kMDItemOriginalFormat
        Original format of the movie

        @const kMDItemOriginalSource
        Original source of the movie

        @const kMDItemAuthorEmailAddresses
        This attribute indicates the author of the emails message addresses. (This is always
        the email address, and not the human readable version)

        @const kMDItemRecipientEmailAddresses
        This attribute indicates the reciepients email addresses. (This is always the email
        address,  and not the human readable version).

        @const kMDItemAuthorAddresses
        This attribute indicates the author addresses of the document.
 
        @const kMDItemRecipientAddresses
        This attribute indicates the recipient addresses of the document. 
 
        @const kMDItemURL
        Url of the item
        
        @const kMDItemIsLikelyJunk
        This attribute indicates if the document is likely to be considered junk.
        
        @const kMDItemExecutableArchitectures
        Array of executables architectures the item contains.
 
        @const kMDItemExecutablePlatform
        Indicates platform required to execute this application.

        @const kMDItemApplicationCategories
        Array of categories the item application is a member of.

*/
MD_EXPORT const CFStringRef    kMDItemInformation MD_AVAIL_LEOPARD;              // CFString
MD_EXPORT const CFStringRef    kMDItemDirector MD_AVAIL_LEOPARD;                 // CFString
MD_EXPORT const CFStringRef    kMDItemProducer MD_AVAIL_LEOPARD;                 // CFString
MD_EXPORT const CFStringRef    kMDItemGenre MD_AVAIL_LEOPARD;                    // CFString
MD_EXPORT const CFStringRef    kMDItemPerformers MD_AVAIL_LEOPARD;               // CFArray of CFString
MD_EXPORT const CFStringRef    kMDItemOriginalFormat MD_AVAIL_LEOPARD;           // CFString
MD_EXPORT const CFStringRef    kMDItemOriginalSource MD_AVAIL_LEOPARD;           // CFString
MD_EXPORT const CFStringRef    kMDItemAuthorEmailAddresses MD_AVAIL_LEOPARD;     // CFArray of CFString
MD_EXPORT const CFStringRef    kMDItemRecipientEmailAddresses MD_AVAIL_LEOPARD;  // CFArray of CFString
MD_EXPORT const CFStringRef    kMDItemAuthorAddresses API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );     // CFArray of CFString
MD_EXPORT const CFStringRef    kMDItemRecipientAddresses API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );  // CFArray of CFString
MD_EXPORT const CFStringRef    kMDItemURL MD_AVAIL_LEOPARD;                      // CFString

MD_EXPORT const CFStringRef    kMDItemLabelIcon API_DEPRECATED("No longer supported", macos(10.7, 10.7)) API_UNAVAILABLE( ios, tvos, watchos );
MD_EXPORT const CFStringRef    kMDItemLabelID API_DEPRECATED("No longer supported", macos(10.7, 10.7)) API_UNAVAILABLE( ios, tvos, watchos );
MD_EXPORT const CFStringRef    kMDItemLabelKind API_DEPRECATED("No longer supported", macos(10.7, 10.7)) API_UNAVAILABLE( ios, tvos, watchos );
MD_EXPORT const CFStringRef    kMDItemLabelUUID API_DEPRECATED("No longer supported", macos(10.7, 10.7)) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT const CFStringRef    kMDItemIsLikelyJunk API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFBoolean
MD_EXPORT const CFStringRef    kMDItemExecutableArchitectures API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFArray of CFString
MD_EXPORT const CFStringRef    kMDItemExecutablePlatform API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFString
MD_EXPORT const CFStringRef    kMDItemApplicationCategories API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFArray of CFString

MD_EXPORT const CFStringRef    kMDItemIsApplicationManaged API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos ); // CFBoolean

MD_END_C_DECLS

/* ================================================================ */

#if defined(__GNUC__) || __STDC_VERSION__ >= 199901L
/* Private function rewriting */
MD_BEGIN_C_DECLS
#define MDItemCopyAttributeList(item, ...) __MDItemCopyAttributesEllipsis1(item, __VA_ARGS__, NULL)
extern CFDictionaryRef __MDItemCopyAttributesEllipsis1(MDItemRef item, ...) MD_AVAIL;
MD_END_C_DECLS
#endif

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */

#endif /* __MDITEM__ */
