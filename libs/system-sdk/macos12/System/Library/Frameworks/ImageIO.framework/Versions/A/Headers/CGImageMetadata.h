/*
 * ImageIO - CGImageMetadata.h
 * Copyright (c) 2004-2016 Apple Inc. All rights reserved.
 *
 */

#ifndef CGIMAGEMETADATA_H_
#define CGIMAGEMETADATA_H_

#include <ImageIO/ImageIOBase.h>

CF_IMPLICIT_BRIDGING_ENABLED

/*!
 * @header CGImageMetadata.h
 * @abstract Implements access to image metadata
 * @description CGImageMetadata APIs allow clients to view and modify metadata
 * for popular image formats. ImageIO supports the EXIF, IPTC, and XMP
 * metadata specifications. Please refer to CGImageSource.h for functions to 
 * read metadata from a CGImageSource, and CGImageDestination.h for functions to
 * write metadata to a CGImageDestination. CGImageDestinationCopyImageSource can
 * be used to modify metadata without recompressing the image.
 *
 * Developers can enable additional debugging information by setting the
 * environment variable IIO_DEBUG_METADATA=1.
 * @related CGImageSource.h
 * @related CGImageDestination.h
 * @ignorefuncmacro IMAGEIO_AVAILABLE_STARTING
 * @ignore IMAGEIO_EXTERN
 * @unsorted
 */

/*!
 * @typedef CGImageMetadataRef 
 * @abstract an immutable container for CGImageMetadataTags
 */
typedef const struct IIO_BRIDGED_TYPE(id) CGImageMetadata *CGImageMetadataRef;

///*! @functiongroup Creating and identifying CGImageMetadata containers */
/*!
 * @function CGImageMetadataGetTypeID
 * @abstract Gets the type identifier for the CGImageMetadata opaque type
 * @return the type identifier for the CGImageMetadata opaque type
 */
IMAGEIO_EXTERN CFTypeID CGImageMetadataGetTypeID(void);

/*!
 * @typedef CGMutableImageMetadataRef
 * @abstract a mutable container for CGImageMetadataTags
 * @discussion A CGMutableImageMetadataRef can be used in any function that
 * accepts a CGImageMetadataRef.
 */
typedef struct IIO_BRIDGED_TYPE(id) CGImageMetadata *CGMutableImageMetadataRef;

/*!
 * @function CGImageMetadataCreateMutable
 * @abstract Creates an empty CGMutableImageMetadataRef
 */
IMAGEIO_EXTERN CGMutableImageMetadataRef _iio_Nonnull CGImageMetadataCreateMutable(void) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataCreateMutableCopy
 * @abstract Creates a deep mutable copy of another CGImageMetadataRef
 * @discussion Before modifying an immutable CGImageMetadataRef (such as metadata
 * from CGImageSourceCopyMetadataAtIndex) you must first make a copy.
 * This function makes a deep copy of all CGImageMetadataTags and their values.
 */
IMAGEIO_EXTERN CGMutableImageMetadataRef _iio_Nullable CGImageMetadataCreateMutableCopy(CGImageMetadataRef _iio_Nonnull metadata) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

// ****************************************************************************
// CGImageMetadataTag - an individual metadata tag, encapsulating an EXIF tag, 
// IPTC tag, or XMP property.
// ****************************************************************************

/*!
 * @typedef CGImageMetadataTagRef
 * @abstract an individual metadata tag
 * @discussion A CGImageMetadataTag encapsulates an EXIF, IPTC, or XMP property.
 * All tags contain a namespace, prefix, name, type, and value. Please see
 * @link CGImageMetadataTagCreate @/link for more details.
 */
typedef struct IIO_BRIDGED_TYPE(id) CGImageMetadataTag *CGImageMetadataTagRef;

///*! @functiongroup Creating and identifying CGImageMetadataTags */
/*!
 * @function CGImageMetadataTagGetTypeID
 * @abstract Gets the type identifier for the CGImageMetadataTag opaque type
 * @return the type identifier for the CGImageMetadataTagGetTypeID opaque type
 */
IMAGEIO_EXTERN CFTypeID CGImageMetadataTagGetTypeID(void) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);


// ****************************************************************************
// Constants for use in a CGImageMetadataTag
// ****************************************************************************

CF_ASSUME_NONNULL_BEGIN

// All metadata tags must contain a namespace. Clients may use one of the 
// public namespaces defined below or create their own namespace. If a caller
// defines their own namespace, it should comply with the guidelines set forth
// by Adobe in their XMP specification at:
// http://www.adobe.com/devnet/xmp.html.
// For example: "http://ns.adobe.com/exif/1.0/". 
// The caller must also register a custom namespace using 
// CGImageMetadataRegisterNamespaceForPrefix.

// Public, common namespaces.
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceExif IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceExifAux IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceExifEX IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceDublinCore IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceIPTCCore IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceIPTCExtension IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespacePhotoshop IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceTIFF IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceXMPBasic IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataNamespaceXMPRights IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

// All metadata tags must contain a prefix. For the public namespaces defined
// above, no prefix is required - ImageIO will use appropriate defaults.  For 
// other namespaces a prefix is required. For example, while the exif namespace 
// is rather long ("http://ns.adobe.com/exif/1.0/"), when exported the shorter 
// string "exif" will be used to prefix all properties in the exif namespace 
// (example - "exif:Flash").

// Public, common prefixes.
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixExif IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixExifAux IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixExifEX IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixDublinCore IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixIPTCCore IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixIPTCExtension IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixPhotoshop IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixTIFF IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixXMPBasic IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageMetadataPrefixXMPRights IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

CF_ASSUME_NONNULL_END

// Metadata value type constants.
/*!
 * @typedef CGImageMetadataType
 * @abstract The XMP type for a CGImageMetadataTag
 * @discussion CGImageMetadataType defines a list of constants used to indicate
 * the type for a CGImageMetadataTag. If you are reading metadata, use the type 
 * to determine how to interpret the CGImageMetadataTag's value. If you are 
 * creating a CGImageMetadataTag, use the type to specify how the tag 
 * should be serialized in XMP. String types have CFStringRef values, array 
 * types have CFArray values, and structure types have CFDictionary values.
 * @const kCGImageMetadataTypeDefault The type will be interpretted based on the
 * CFType of the tag's value. This is only used when creating a new 
 * CGImageMetadataTag - no existing tags should have this value. CFString 
 * defaults to kCGImageMetadataTypeString, CFArray defaults to 
 * kCGImageMetadataTypeArrayOrdered, and CFDictionary defaults to
 * kCGImageMetadataTypeStructure.
 * @const kCGImageMetadataTypeString A string value. CFNumber and CFBoolean 
 * values will be converted to a string.
 * @const kCGImageMetadataTypeArrayUnordered An array where order does not matter.
 * Serialized in XMP as <rdf:Bag>.
 * @const kCGImageMetadataTypeArrayOrdered An array where order is preserved.
 * Serialized in XMP as <rdf:Seq>.
 * @const kCGImageMetadataTypeAlternateArray An ordered array where all elements
 * are alternates for the same value. Serialized in XMP as <rdf:Alt>.
 * @const kCGImageMetadataTypeAlternateText A special case of an alternate array
 * where all elements are different localized strings for the same value. 
 * Serialized in XMP as an alternate array of strings with xml:lang qualifiers.
 * @const kCGImageMetadataTypeStructure A collection of keys and values. Unlike
 * array elements, fields of a structure may belong to different namespaces.
 */
typedef CF_ENUM(int32_t, CGImageMetadataType) {
    kCGImageMetadataTypeInvalid = -1,
    kCGImageMetadataTypeDefault = 0,
    kCGImageMetadataTypeString = 1,
    kCGImageMetadataTypeArrayUnordered = 2,
    kCGImageMetadataTypeArrayOrdered = 3,
    kCGImageMetadataTypeAlternateArray = 4,
    kCGImageMetadataTypeAlternateText = 5,
    kCGImageMetadataTypeStructure = 6
};

// ****************************************************************************
// Creating a CGImageMetadataTag
// ****************************************************************************
/*!
 * @function CGImageMetadataTagCreate
 * @abstract Creates a new CGImageMetadataTag
 * @param xmlns The namespace for the tag. The value can be a common XMP namespace
 * defined above, such as kCGImageMetadataNamespaceExif, or a CFString with a
 * custom namespace URI. Custom namespaces must be a valid XML namespace. By
 * convention, namespaces should end with either '/' or '#'. For example, exif
 * uses the namespace "http://ns.adobe.com/exif/1.0/".
 * @param prefix An abbreviation for the XML namespace. The value can be NULL if
 * the namespace is defined as a constant. Custom prefixes must be a valid XML
 * name. For example, the prefix used for "http://ns.adobe.com/exif/1.0/" is "exif".
 * The XMP serialization of the tag will use the prefix. Prefixes are also
 * important for path-based CGImageMetadata functions, such as 
 * @link CGImageMetadataCopyStringValueWithPath @/link or 
 * @link CGImageMetadataSetValueWithPath @/link.
 * @param name The name of the tag. It must be a valid XMP name.
 * @param type The type of the tag's value. Must be a constant from @link 
 * CGImageMetadataType @/link.
 * @param value The value of the tag. Allowable CFTypes include CFStringRef,
 * CFNumberRef, CFBooleanRef, CFArrayRef, and CFDictionaryRef. The CFType of 'value'
 * must correspond to the 'type'. The elements of a CFArray must be either a 
 * CFStringRef or CGImageMetadataTagRef. The keys of a CFDictionary must be 
 * CFStringRefs with valid XMP names. The values of a CFDictionary must be either
 * CFStringRefs or CGImageMetadataTagRefs. A shallow copy of the value is stored
 * in the tag. Therefore, modifying a mutable value after the tag is created 
 * will not affect the tag's value.
 * @result Returns a pointer to a new CGImageMetadataTag. Returns NULL if a tag
 * could not be created with the specified parameters.
 */
IMAGEIO_EXTERN CGImageMetadataTagRef _iio_Nullable CGImageMetadataTagCreate (CFStringRef _iio_Nonnull xmlns, CFStringRef _iio_Nullable prefix, CFStringRef _iio_Nonnull name, CGImageMetadataType type, CFTypeRef _iio_Nonnull value) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

// ****************************************************************************
// Getting attributes of a CGImageMetadataTag
// ****************************************************************************
///*! @functiongroup Getting attributes of a CGImageMetadataTag */
/*!
 * @function CGImageMetadataTagCopyNamespace 
 * @abstract Returns a copy of the tag's namespace
 */
IMAGEIO_EXTERN CFStringRef _iio_Nullable CGImageMetadataTagCopyNamespace(CGImageMetadataTagRef _iio_Nonnull tag) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataTagCopyPrefix 
 * @abstract Returns a copy of the tag's prefix
 */
IMAGEIO_EXTERN CFStringRef _iio_Nullable CGImageMetadataTagCopyPrefix(CGImageMetadataTagRef _iio_Nonnull tag) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataTagCopyName 
 * @abstract Returns a copy of the tag's name
 */
IMAGEIO_EXTERN CFStringRef _iio_Nullable CGImageMetadataTagCopyName(CGImageMetadataTagRef _iio_Nonnull tag) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataTagCopyValue 
 * @abstract Returns a shallow copy of the tag's value
 * @discussion This function should only be used to read the tag's value. 
 * CGImageMetadataCopyTagWithPath returns a copy of the tag (including a copy of
 * the tag's value). Therefore mutating a tag's value returned from this function
 * may not actually mutate the value in the CGImageMetadata. It is recommended 
 * to create a new tag followed by CGImageMetadataSetTagWithPath, or use 
 * CGImageMetadataSetValueWithPath to mutate a metadata value. 
 */
IMAGEIO_EXTERN CFTypeRef _iio_Nullable CGImageMetadataTagCopyValue(CGImageMetadataTagRef _iio_Nonnull tag) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataTagGetType
 * @abstract Get the type of the CGImageMetadataTag
 * @return Returns a CGImageMetadataType constant for the CGImageMetadataTag.
 * This is primarily used to determine how to interpret the tag's value.
 */
IMAGEIO_EXTERN CGImageMetadataType CGImageMetadataTagGetType(CGImageMetadataTagRef _iio_Nonnull tag) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataTagCopyQualifiers
 * @abstract Return a copy of the tag's qualifiers
 * @discussion XMP allows properties to contain supplemental properties called
 * qualifiers. Qualifiers are themselves CGImageMetadataTags with their own 
 * namespace, prefix, name, and value. A common use is the xml:lang qualifier
 * for elements of an alternate-text array.
 * @return Returns a copy of the array of qualifiers. Elements of the array are 
 * CGImageMetadataTags. Returns NULL if the tag does not have any qualifiers. 
 * The copy is shallow, the qualifiers are not deep copied.
 */
IMAGEIO_EXTERN CFArrayRef _iio_Nullable CGImageMetadataTagCopyQualifiers(CGImageMetadataTagRef _iio_Nonnull tag) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);


// ****************************************************************************
// Functions for copying CGImageMetadataTagRefs from a CGImageMetadataRef. 
// These functions make it easier for the caller to traverse complex nested 
// structures of metadata, similar to KVC-compliant Objective-C classes.
// ****************************************************************************
///*! @functiongroup Retrieving CGImageMetadataTagRefs from a CGImageMetadataRef */
/*!
 * @function CGImageMetadataCopyTags
 * @abstract Obtain an array of tags from a CGImageMetadataRef
 * @return Returns an array with a shallow copy of all top-level 
 * CGImageMetadataTagRefs in a CGImageMetadataRef.
 */
IMAGEIO_EXTERN CFArrayRef _iio_Nullable CGImageMetadataCopyTags(CGImageMetadataRef _iio_Nonnull metadata) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataCopyTagWithPath
 * @abstract Searches for a specific CGImageMetadataTag in a CGImageMetadataRef
 * @discussion This is the primary function for clients to obtain specific 
 * metadata properties from an image. The 'path' mechanism provides a way to 
 * access both simple top-level properties, such as Date & Time, or complex 
 * deeply-nested properties with ease.
 * @param metadata A collection of metadata tags.
 * @param parent A parent tag. If NULL, the path is relative to the root of the
 * CGImageMetadataRef (i.e. it is not a child of another property). If the parent
 * is provided, the effective path will be the concatenation of the parent's path
 * and the 'path' parameter. This is useful for accessing array elements or 
 * structure fields inside nested tags.
 * @param path A string representing a path to the desired tag. Paths consist of
 * a tag prefix (i.e. "exif") joined with a tag name (i.e. "Flash") by a colon 
 * (":"), such as CFSTR("exif:Flash"). 
 * Elements of ordered and unordered arrays are accessed via 0-based indices inside square [] brackets.
 * Elements of alternate-text arrays are accessed by an RFC 3066 language code inside square [] brackets.
 * Fields of a structure are delimited by a period, '.'.
 * Qualifiers are delimited by the '?' character. Only tags with string values (kCGImageMetadataTypeString)
 * are allowed to have qualifiers - arrays and structures may not contain qualifiers.
 *
 * If parent is NULL, a prefix must be specified for the first tag. Prefixes for
 * all subsequent tags are optional. If unspecified, the prefix is 
 * inherented from the nearest parent tag with a prefix. Custom prefixes must be
 * registered using @link CGImageMetadataRegisterNamespaceForPrefix @/link prior to use 
 * in any path-based functions.
 *
 * Examples:
 *  <ul>
 *    <li>'path' = CFSTR("xmp:CreateDate")</li>       
 *    <li>'path' = CFSTR("exif:Flash.Fired")</li>
 *    <li>'parent' = tag at path CFSTR("exif:Flash"), path = CFSTR("exif:Fired") (equivilent to previous)</li>
 *    <li>'path' = CFSTR("exif:Flash.RedEyeMode")</li>
 *    <li>'path' = CFSTR("dc:title")</li>
 *    <li>'path' = CFSTR("dc:subject")</li>
 *    <li>'path' = CFSTR("dc:subject[2]") </li>
 *    <li>'parent' = tag at path CFSTR("dc:subject"), path = CFSTR("[2]") (equivilent to previous)</li>
 *    <li>'path' = CFSTR("dc:description[x-default])"</li>
 *    <li>'path' = CFSTR("dc.description[de])"</li>
 *    <li>'path' = CFSTR("dc.description[fr])"</li>
 *    <li>'path' = CFSTR("foo:product)"</li>
 *    <li>'path' = CFSTR("foo:product?bar:manufacturer)"</li>
 *  </ul>
 * @return Returns a copy of CGImageMetadataTag matching 'path', or NULL if no 
 * match is found. The copy of the tag's value is shallow. Tags
 * copied from an immutable CGImageMetadataRef are also immutable. Because this
 * function returns a copy of the tag's value, any modification of the tag's 
 * value must be followed by a CGImageMetadataSetTagWithPath to commit the 
 * change to the metadata container.
 */
IMAGEIO_EXTERN CGImageMetadataTagRef _iio_Nullable CGImageMetadataCopyTagWithPath(CGImageMetadataRef _iio_Nonnull metadata, CGImageMetadataTagRef _iio_Nullable parent, CFStringRef _iio_Nonnull path) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataCopyStringValueWithPath
 * @abstract Searches for a specific tag in a CGImageMetadataRef and returns its
 * string value.
 * @discussion This is a convenience method for searching for a tag at path and
 * extracting the string value.
 * @param metadata A collection of metadata tags.
 * @param parent A parent tag. If NULL, the path is relative to the root of the
 * CGImageMetadataRef (i.e. it is not a child of another property).
 * @param path A string with the path to the desired tag. Please consult
 * the documentation of @link CGImageMetadataCopyTagWithPath @/link for 
 * information about path syntax.
 * @return Returns a string from a CGImageMetadataTag located at 'path'. The 
 * tag must be of type kCGImageMetadataTypeString or kCGImageMetadataTypeAlternateText.
 * For AlternateText tags, the element with the "x-default" language qualifier 
 * will be returned. For other types, NULL will be returned.
 */
IMAGEIO_EXTERN CFStringRef _iio_Nullable CGImageMetadataCopyStringValueWithPath(CGImageMetadataRef _iio_Nonnull metadata, CGImageMetadataTagRef _iio_Nullable parent, CFStringRef _iio_Nonnull path) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

// ****************************************************************************
// Functions for modifying a CGMutableImageMetadataRef
// ****************************************************************************
///*! @functiongroup Modifying a CGMutableImageMetadataRef */
/*!
 * @function CGImageMetadataRegisterNamespaceForPrefix
 * @abstract Associates an XMP namespace URI with a prefix string.
 * @discussion This allows ImageIO to create custom metadata when it encounters 
 * an unrecognized prefix in a path (see CGImageMetadataCopyTagWithPath for more
 * information about path syntax). A namespace must be registered before it can 
 * be used to add custom metadata. All namespaces found in the image's metadata,
 * or defined as a constant above, will be pre-registered. Namespaces and 
 * prefixes must be unique.
 * @return Returns true if successful. Returns false and sets 'err' if an error 
 * or conflict occurs.
 */
IMAGEIO_EXTERN bool CGImageMetadataRegisterNamespaceForPrefix(CGMutableImageMetadataRef _iio_Nonnull metadata, CFStringRef _iio_Nonnull xmlns, CFStringRef _iio_Nonnull prefix, _iio_Nullable CFErrorRef * _iio_Nullable err) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataSetTagWithPath
 * @abstract Sets the tag at a specific path in a CGMutableImageMetadata container or a parent tag
 * @discussion This is the primary function for adding new metadata tags to a 
 * metadata container, or updating existing tags. All tags required to reach 
 * the final tag (at the end of the path) will be created, if needed. Tags will
 * created with default types (ordered arrays). Creating tags will fail if a 
 * prefix is encountered that has not been registered. Use
 * @link CGImageMetadataRegisterNamespaceForPrefix @/link to associate a prefix
 * with a namespace prior to using a path-based CGImageMetadata function.
 * Note that if a parent tag is provided,
 * the children of that tag reference will be modified, which may be a different
 * reference from the tag stored in the metadata container. Since tags are normally
 * obtained as a copy, it is typically neccesary to use CGImageMetadataSetTagWithPath
 * to commit the changed parent object back to the metadata container (using
 * the parent's path and NULL for the parent).
 * @param metadata A mutable collection of metadata tags. 
 * Use @link CGImageMetadataCreateMutableCopy @/link or 
 * @link CGImageMetadataCreateMutable @/link to obtain a mutable metadata container.
 * @param parent A parent tag. If NULL, the path is relative to the root of the
 * CGImageMetadataRef (i.e. it is not a child of another property). 
 * Note that if a parent tag is provided,
 * the children of that tag reference will be modified, which may be a different
 * reference from the tag stored in the metadata container. Since tags are normally
 * obtained as a copy, it is typically neccesary to use CGImageMetadataSetTagWithPath
 * to commit the changed parent object back to the metadata container (using
 * the parent's path and NULL for the parent).
 * @param path A string with the path to the desired tag. Please consult
 * the documentation of @link CGImageMetadataCopyTagWithPath @/link for 
 * information about path syntax.
 * @param tag The CGImageMetadataTag to be added to the metadata. The tag
 * will be retained.
 * @return Returns true if successful, false otherwise.
 */
IMAGEIO_EXTERN bool CGImageMetadataSetTagWithPath(CGMutableImageMetadataRef _iio_Nonnull metadata, CGImageMetadataTagRef _iio_Nullable parent, CFStringRef _iio_Nonnull path, CGImageMetadataTagRef _iio_Nonnull tag) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataSetValueWithPath
 * @abstract Sets the value of the tag at a specific path in a CGMutableImageMetadataRef container or a parent tag
 * @discussion This function is used to add new metadata values to a 
 * metadata container, or update existing tag values. All tags required to reach 
 * the final tag (at the end of the path) are created, if needed. Tags are
 * created with default types (i.e. arrays will be ordered). Creating tags will 
 * fail if a prefix is encountered that has not been registered. Use
 * @link CGImageMetadataRegisterNamespaceForPrefix @/link to associate a prefix
 * with a namespace prior to using a path-based CGImageMetadata function.
 *
 * Examples
 * <ul>
 *     <li>'path' = CFSTR("xmp:CreateDate"), 'value' = CFSTR("2011-09-20T14:54:47-08:00")</li>
 *     <li>'path' = CFSTR("dc:subject[0]"), 'value' = CFSTR("San Francisco")</li>
 *     <li>'path' = CFSTR("dc:subject[1]"), 'value' = CFSTR("Golden Gate Bridge")</li>
 *     <li>'path' = CFSTR("dc:description[en]") 'value' = CFSTR("my image description")</li>
 *     <li>'path' = CFSTR("dc:description[de]") 'value' = CFSTR("meine bildbeschreibung")</li>
 * </ul>
 * Note that if a parent tag is provided,
 * the children of that tag reference will be modified, which may be a different
 * reference from the tag stored in the metadata container. Since tags are normally
 * obtained as a copy, it is typically neccesary to use CGImageMetadataSetTagWithPath
 * to commit the changed parent object back to the metadata container (using
 * the parent's path and NULL for the parent).
 * @param metadata A mutable collection of metadata tags. 
 * Use @link CGImageMetadataCreateMutableCopy @/link or 
 * @link CGImageMetadataCreateMutable @/link to obtain a mutable metadata container.
 * @param parent A parent tag. If NULL, the path is relative to the root of the
 * CGImageMetadataRef (i.e. it is not a child of another property).
 * @param path A string with the path to the desired tag. Please consult
 * the documentation of @link CGImageMetadataCopyTagWithPath @/link for 
 * information about path syntax.
 * @param value The value to be added to the CGImageMetadataTag matching the path.
 * The tag will be retained. The restrictions for the value are the same as in @link
 * CGImageMetadataTagCreate @/link.
 * @return Returns true if successful, false otherwise.
 */
IMAGEIO_EXTERN bool CGImageMetadataSetValueWithPath(CGMutableImageMetadataRef _iio_Nonnull metadata, CGImageMetadataTagRef _iio_Nullable parent, CFStringRef _iio_Nonnull path, CFTypeRef _iio_Nonnull value) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataRemoveTagWithPath
 * @abstract Removes the tag at a specific path from a CGMutableImageMetadata container or from the parent tag
 * @discussion Use this function to delete a metadata tag matching a specific 
 * path from a mutable metadata container. Note that if a parent tag is provided,
 * the children of that tag reference will be modified, which may be a different
 * reference from the tag stored in the metadata container. Since tags are normally
 * obtained as a copy, it is typically neccesary to use CGImageMetadataSetTagWithPath
 * to commit the changed parent object back to the metadata container (using
 * the parent's path and NULL for the parent).
 * @param parent A parent tag. If NULL, the path is relative to the root of the
 * CGImageMetadataRef (i.e. it is not a child of another property).
 * @param path A string with the path to the desired tag. Please consult
 * the documentation of @link CGImageMetadataCopyTagWithPath @/link for 
 * information about path syntax.
 */
IMAGEIO_EXTERN bool CGImageMetadataRemoveTagWithPath(CGMutableImageMetadataRef _iio_Nonnull metadata,  CGImageMetadataTagRef _iio_Nullable parent, CFStringRef _iio_Nonnull path) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);


#ifdef __BLOCKS__
/*  Iterating through tags of CGImageMetadataRef */
/*!
 * @typedef CGImageMetadataTagBlock
 * @abstract The block type used by CGImageMetadataEnumerateTagsUsingBlock
 * @param path The full path to the tag in the metadata container.
 * @param tag The CGImageMetadataTagRef corresponding to the path in metadata.
 * @return Return true to continue iterating through the tags, or return false to stop.
 */
typedef bool(^CGImageMetadataTagBlock)(CFStringRef _iio_Nonnull path, CGImageMetadataTagRef _iio_Nonnull tag);

// Enumerates the children of the tag located at 'path'.
// If 'rootPath' is nil, all top-level tags in 'metadata' will be enumerated.
// Executes a CGImageMetadataTagBlock, 'block', for each CGImageMetadataTag.
// The block should return true to continue enumeration, or false to stop.
// Behavior is undefined if 'metadata' is modified during enumeration.

/*!
 * @function CGImageMetadataEnumerateTagsUsingBlock
 * @abstract Executes a given block using each tag in the metadata
 * @discussion This function iterates over all of the tags in a
 * CGImageMetadataRef, executing the block for each tag. The default behavior 
 * iterates over all top-level tags in the metadata. The path of the tag and 
 * the tag itself is passed to the block. The metadata cannot be modified inside
 * the block - consider adding the tags of interest into another collection.
 * @param metadata A collection of metadata tags.
 * @param rootPath Iteration will occur for all children of the tag matching
 * the root path. Please refer to CGImageMetadataCopyTagWithPath for information
 * about path syntax. If NULL or an empty string, the block will be executed 
 * for all top-level tags in the metadata container.
 * @param options A dictionary of options for iterating through the tags.
 * Currently the only supported option is kCGImageMetadataEnumerateRecursively,
 * which should be set to a CFBoolean.
 * @param block The block that is executed for each tag in metadata.
 */
IMAGEIO_EXTERN void CGImageMetadataEnumerateTagsUsingBlock(CGImageMetadataRef _iio_Nonnull metadata, CFStringRef _iio_Nullable rootPath, CFDictionaryRef _iio_Nullable options, CGImageMetadataTagBlock _iio_Nonnull block) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);
#endif

// A key for the 'options' of CGImageMetadataEnumerateTagsUsingBlock. If present,
// the value should be a CFBoolean. If true, tags will be enumerated recursively,
// if false, only the direct children of 'rootPath' will be enumerated. 
// The default is non-recursive.
IMAGEIO_EXTERN const CFStringRef _iio_Nonnull kCGImageMetadataEnumerateRecursively IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);


// ****************************************************************************
// Functions for working with constants defined in CGImageProperties.h
// Provides a bridge for values from CGImageCopyPropertiesAtIndex().
// Simplifies metadata access for properties defined EXIF and IPTC standards, 
// which have no notion of namespaces, prefixes, or XMP property types.
// Metadata Working Group guidance is factored into the mapping of 
// CGImageProperties to XMP. For example, setting Exif DateTimeOriginal will
// set the value of the corresponding XMP tag, which is photoshop:DateCreated
// ****************************************************************************

///*! @functiongroup Working with CGImageProperties */
/*!
 * @function CGImageMetadataCopyTagMatchingImageProperty
 * @abstract Searches for a specific CGImageMetadataTag matching a kCGImageProperty constant
 * @discussion Provides a bridge for values from CGImageCopyPropertiesAtIndex, simplifying 
 * access for properties defined in EXIF and IPTC standards, which have no notion of 
 * namespaces, prefixes, or XMP property types.
 * Metadata Working Group guidance is factored into the mapping of CGImageProperties to 
 * XMP compatible CGImageMetadataTags.
 * For example, kCGImagePropertyExifDateTimeOriginal will get the value of the 
 * corresponding XMP tag, which is photoshop:DateCreated. Note that property values will 
 * still be in their XMP forms, such as "YYYY-MM-DDThh:mm:ss" for DateTime, rather than
 * the EXIF or IPTC DateTime formats.
 * @param metadata A collection of metadata tags
 * @param dictionaryName the metadata subdictionary to which the image property belongs,
 * such as kCGImagePropertyExifDictionary or kCGImagePropertyIPTCDictionary. Not all
 * dictionaries and properties are supported at this time.
 * @param propertyName the name of the property. This must be a defined property constant
 * corresponding to the 'dictionaryName'. For example, kCGImagePropertyTIFFOrientation,
 * kCGImagePropertyExifDateTimeOriginal, or kCGImagePropertyIPTCKeywords. A warning
 * will be logged if the CGImageProperty is unsupported by CGImageMetadata.
 * @return Returns a CGImageMetadataTagRef with the appropriate namespace, prefix, 
 * tag name, and XMP value for the corresponding CGImageProperty. Returns NULL if the 
 * property could not be found.
 */
IMAGEIO_EXTERN CGImageMetadataTagRef _iio_Nullable CGImageMetadataCopyTagMatchingImageProperty(CGImageMetadataRef _iio_Nonnull metadata, CFStringRef _iio_Nonnull dictionaryName, CFStringRef _iio_Nonnull propertyName) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataSetValueMatchingImageProperty
 * @abstract Sets the value of the CGImageMetadataTag matching a kCGImageProperty constant
 * @discussion Provides a bridge for values from CGImageCopyPropertiesAtIndex, simplifying 
 * changing property values defined in EXIF and IPTC standards, which have no notion of 
 * namespaces, prefixes, or XMP property types.
 * Metadata Working Group guidance is factored into the mapping of CGImageProperties to 
 * XMP compatible CGImageMetadataTags.
 * For example, setting kCGImagePropertyExifDateTimeOriginal will set the value of the 
 * corresponding XMP tag, which is photoshop:DateCreated. Note that property values should 
 * still be in their XMP forms, such as "YYYY-MM-DDThh:mm:ss" for DateTime, rather than
 * the EXIF or IPTC DateTime formats. Although this function will allow the caller to set
 * custom values for these properties, you should consult the appropriate specifications 
 * for details about property value formats for EXIF and IPTC tags in XMP.
 * @param metadata A mutable collection of metadata tags
 * @param dictionaryName the metadata subdictionary to which the image property belongs,
 * such as kCGImagePropertyExifDictionary or kCGImagePropertyIPTCDictionary. Not all
 * dictionaries and properties are supported at this time.
 * @param propertyName the name of the property. This must be a defined property constant
 * corresponding to the 'dictionaryName'. For example, kCGImagePropertyTIFFOrientation,
 * kCGImagePropertyExifDateTimeOriginal, or kCGImagePropertyIPTCKeywords. A warning
 * will be logged if the CGImageProperty is unsupported by CGImageMetadata.
 * @param value A CFTypeRef with the value for the tag. The same value restrictions apply
 * as in @link CGImageMetadataTagCreate @/link.
 * @return Returns true if successful, false otherwise.
 */
IMAGEIO_EXTERN bool CGImageMetadataSetValueMatchingImageProperty(CGMutableImageMetadataRef _iio_Nonnull metadata, CFStringRef _iio_Nonnull dictionaryName, CFStringRef _iio_Nonnull propertyName, CFTypeRef _iio_Nonnull value) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);


// ****************************************************************************
// Functions for converting metadata to and from XMP packets
// ****************************************************************************
///*! @functiongroup Reading and Writing XMP */

/*!
 * @function CGImageMetadataCreateXMPData
 * @abstract Serializes the CGImageMetadataRef to XMP data
 * @discussion This converts all of the metadata tags to a block of XMP data. Common uses
 * include creating sidecar files that contain metadata for image formats that do not 
 * support embedded XMP, or cannot be edited due to other format restrictions (such as
 * proprietary RAW camera formats).
 * @param metadata A collection of metadata tags.
 * @param options should be NULL. Options are currently not used, but may be used in 
 * future release.
 * @return Returns a CFData containing an XMP representation of the metadata. Returns
 * NULL if an error occurred. 
 */
IMAGEIO_EXTERN CFDataRef _iio_Nullable CGImageMetadataCreateXMPData (CGImageMetadataRef _iio_Nonnull metadata, CFDictionaryRef _iio_Nullable options) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 * @function CGImageMetadataCreateFromXMPData
 * @abstract Creates a collection of CGImageMetadataTags from a block of XMP data
 * @discussion Converts XMP data into a collection of metadata tags.
 * The data must be a complete XMP tree. XMP packet  headers (<?xpacket .. ?>) are 
 * supported.
 * @param data The XMP data.
 * @return Returns a collection of CGImageMetadata tags. Returns NULL if an error occurred. 
 */
IMAGEIO_EXTERN CGImageMetadataRef _iio_Nullable CGImageMetadataCreateFromXMPData (CFDataRef _iio_Nonnull data) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/*!
 *  @constant kCFErrorDomainCGImageMetadata
 *  @discussion Error domain for all errors originating in ImageIO for CGImageMetadata APIs.
 *      Error codes may be interpreted using the list below.
 */
IMAGEIO_EXTERN const CFStringRef _iio_Nonnull kCFErrorDomainCGImageMetadata;

/*!
 *  @enum CGImageMetadataErrors
 *  @discussion the list of all error codes returned under the error domain kCFErrorDomainCGImageMetadata
 */
typedef CF_ENUM(int32_t, CGImageMetadataErrors) {
    kCGImageMetadataErrorUnknown = 0,
    kCGImageMetadataErrorUnsupportedFormat = 1,
    kCGImageMetadataErrorBadArgument = 2,
    kCGImageMetadataErrorConflictingArguments = 3,
    kCGImageMetadataErrorPrefixConflict = 4,
};

CF_IMPLICIT_BRIDGING_DISABLED

#endif // CGIMAGEMETADATA_H_
