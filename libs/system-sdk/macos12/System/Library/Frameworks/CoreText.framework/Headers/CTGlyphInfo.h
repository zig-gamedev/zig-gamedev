/*
 *  CTGlyphInfo.h
 *  CoreText
 *
 *  Copyright (c) 2006-2019 Apple Inc. All rights reserved.
 *
 */

/*!
    @header

    Thread Safety Information

    All functions in this header are thread safe unless otherwise specified.
*/

#ifndef __CTGLYPHINFO__
#define __CTGLYPHINFO__

#include <CoreText/CTFont.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/* --------------------------------------------------------------------------- */
/* Glyph Info Types */
/* --------------------------------------------------------------------------- */

#if TARGET_OS_IPHONE
typedef const struct CF_BRIDGED_TYPE(id) __CTGlyphInfo * CTGlyphInfoRef;
#else
typedef const struct CF_BRIDGED_TYPE(NSGlyphInfo) __CTGlyphInfo * CTGlyphInfoRef;
#endif

/*!
    @function   CTGlyphInfoGetTypeID
    @abstract   Returns the CFType of the glyph info object
*/

CFTypeID CTGlyphInfoGetTypeID( void ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Glyph Info Values */
/* --------------------------------------------------------------------------- */

/*!
    @enum       CTCharacterCollection
    @abstract   These constants specify character collections.

    @constant   kCTCharacterCollectionIdentityMapping
                Indicates that the character identifier is equal to the CGGlyph
                glyph index.

    @constant   kCTCharacterCollectionAdobeCNS1
                Indicates the Adobe-CNS1 mapping.

    @constant   kCTCharacterCollectionAdobeGB1
                Indicates the Adobe-GB1 mapping.

    @constant   kCTCharacterCollectionAdobeJapan1
                Indicates the Adobe-Japan1 mapping.

    @constant   kCTCharacterCollectionAdobeJapan2
                Indicates the Adobe-Japan2 mapping.

    @constant   kCTCharacterCollectionAdobeKorea1
                Indicates the Adobe-Korea1 mapping.
*/

typedef CF_ENUM(uint16_t, CTCharacterCollection) {
    kCTCharacterCollectionIdentityMapping = 0,
    kCTCharacterCollectionAdobeCNS1       = 1,
    kCTCharacterCollectionAdobeGB1        = 2,
    kCTCharacterCollectionAdobeJapan1     = 3,
    kCTCharacterCollectionAdobeJapan2     = 4,
    kCTCharacterCollectionAdobeKorea1     = 5,

    kCTIdentityMappingCharacterCollection CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTCharacterCollectionIdentityMapping,
    kCTAdobeCNS1CharacterCollection CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTCharacterCollectionAdobeCNS1,
    kCTAdobeGB1CharacterCollection CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTCharacterCollectionAdobeGB1,
    kCTAdobeJapan1CharacterCollection CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTCharacterCollectionAdobeJapan1,
    kCTAdobeJapan2CharacterCollection CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTCharacterCollectionAdobeJapan2,
    kCTAdobeKorea1CharacterCollection CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTCharacterCollectionAdobeKorea1
};


/* --------------------------------------------------------------------------- */
/* Glyph Info Creation */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTGlyphInfoCreateWithGlyphName
    @abstract   Creates an immutable glyph info object.

    @discussion This function creates an immutable glyph info object for a glyph
                name such as "copyright" and a specified font.

    @param      glyphName
                The name of the glyph.

    @param      font
                The font to be associated with the returned CTGlyphInfo object.

    @param      baseString
                The part of the string the returned object is intended
                to override.

    @result     This function will return a reference to a CTGlyphInfo object.
*/

CTGlyphInfoRef _Nullable CTGlyphInfoCreateWithGlyphName(
    CFStringRef glyphName,
    CTFontRef font,
    CFStringRef baseString ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTGlyphInfoCreateWithGlyph
    @abstract   Creates an immutable glyph info object.

    @discussion This function creates an immutable glyph info object for a glyph
                index and a specified font.

    @param      glyph
                The glyph identifier.

    @param      font
                The font to be associated with the returned CTGlyphInfo object.

    @param      baseString
                The part of the string the returned object is intended
                to override.

    @result     This function will return a reference to a CTGlyphInfo object.
*/

CTGlyphInfoRef _Nullable CTGlyphInfoCreateWithGlyph(
    CGGlyph glyph,
    CTFontRef font,
    CFStringRef baseString ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTGlyphInfoCreateWithCharacterIdentifier
    @abstract   Creates an immutable glyph info object.

    @discussion This function creates an immutable glyph info object for a
                character identifier and a character collection.

    @param      cid
                A character identifier.

    @param      collection
                A character collection identifier.

    @param      baseString
                The part of the string the returned object is intended
                to override.

    @result     This function will return a reference to a CTGlyphInfo object.
*/

CTGlyphInfoRef _Nullable CTGlyphInfoCreateWithCharacterIdentifier(
    CGFontIndex cid,
    CTCharacterCollection collection,
    CFStringRef baseString ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Glyph Info Access */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTGlyphInfoGetGlyphName
    @abstract   Gets the glyph name for a glyph info, if applicable.

    @discussion This function will return the glyph name.

    @param      glyphInfo
                The glyph info for which you would like the glyph name.

    @result     If the glyph info object was created with a glyph name, it will
                be returned. Otherwise, this function will return NULL.
*/

CFStringRef _Nullable CTGlyphInfoGetGlyphName(
    CTGlyphInfoRef glyphInfo ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTGlyphInfoGetGlyph
    @abstract   Gets the glyph for a glyph info, if applicable.

    @discussion This function will return the glyph.

    @param      glyphInfo
                The glyph info from which you would like the glyph.

    @result     If the glyph info object was created with a font, it will be
                returned. Otherwise, this function will return 0.
*/

CGGlyph CTGlyphInfoGetGlyph(
    CTGlyphInfoRef glyphInfo ) CT_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));


/*!
    @function   CTGlyphInfoGetCharacterIdentifier
    @abstract   Gets the character identifier for a glyph info.

    @discussion This function will return the character identifier.

    @param      glyphInfo
                The glyph info for which you would like the character identifier.

    @result     If the glyph info object was created with a character identifier,
                it will be returned. Otherwise, this function will return 0.
*/

CGFontIndex CTGlyphInfoGetCharacterIdentifier(
    CTGlyphInfoRef glyphInfo ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTGlyphInfoGetCharacterCollection
    @abstract   Gets the character collection for a glyph info.

    @discussion This function will return the character collection. If the glyph
                info object was created with a glyph name or a glyph index, its
                character collection will be
                kCTIdentityMappingCharacterCollection.

    @param      glyphInfo
                The glyph info for which you would like the character collection.

    @result     This function will return the character collection of the given
                glyph info.
*/

CTCharacterCollection CTGlyphInfoGetCharacterCollection(
    CTGlyphInfoRef glyphInfo ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


CF_ASSUME_NONNULL_END
CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
