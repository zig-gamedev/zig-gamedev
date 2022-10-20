/*
 *  CTRubyAnnotation.h
 *  CoreText
 *
 *  Copyright (c) 2012-2018 Apple Inc. All rights reserved.
 *
 */

/*!
 @header
 
 Thread Safety Information
 
 All functions in this header are thread safe unless otherwise specified.
 */

#ifndef __CTRUBYANNOTATION__
#define __CTRUBYANNOTATION__

#include <CoreText/CTDefines.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/* --------------------------------------------------------------------------- */
/* RubyAnnotation Types                                                        */
/* --------------------------------------------------------------------------- */

typedef const struct CF_BRIDGED_TYPE(id) __CTRubyAnnotation * CTRubyAnnotationRef;


/*!
    @function	CTRubyAnnotationGetTypeID
    @abstract	Returns the CFType of the ruby annotation object
*/

CFTypeID CTRubyAnnotationGetTypeID( void ) CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Ruby alignment Values                                                       */
/* --------------------------------------------------------------------------- */

/*!
    @enum       CTRubyAlignment
    @abstract   These constants specify how to align the ruby annotation and the base text relative to each other when they don't have the same length.

    @constant   kCTRubyAlignmentAuto
                CoreText will determine the alignment.

    @constant   kCTRubyAlignmentStart
                The ruby text is aligned with the start edge of the base text.

    @constant   kCTRubyAlignmentCenter
                The ruby text is centered within the width of the base text. If the ruby text is wider than the base text the base text is centered in the width of the ruby text.

    @constant   kCTRubyAlignmentEnd
                The ruby text is aligned with the end edge of the base text.

    @constant   kCTRubyAlignmentDistributeLetter
                If the width of the ruby text is less than the width of the base text, the ruby text is evenly distributed over the width of the base text, with the first letter of the ruby text aligning with the first letter of the base text and the last letter of the ruby text aligning with the last letter of the base text. If the width of the base text is less than the width of the ruby text, the base text is evenly distributed over the width of the ruby text.

    @constant   kCTRubyAlignmentDistributeSpace
                If the width of the ruby text is less than the width of the base text, the ruby text is evenly distributed over the width of the base text, with a certain amount of space, usually half the inter-character width of the ruby text, before the first and after the last character. If the width of the base text is less than the width of the ruby text, the base text is similarly aligned to the width of the ruby text.

    @constant   kCTRubyAlignmentLineEdge
                If the ruby text is not adjacent to a line edge it is aligned as with kCTRubyAlignmentAuto. If it is adjacent to a line edge the end of ruby text adjacent to the line edge is aligned to the line edge. This is only relevant if the width of the ruby text is greater than the width of the base text; otherwise alignment is as with kCTRubyAlignmentAuto.
*/

typedef CF_ENUM(uint8_t, CTRubyAlignment) {
    kCTRubyAlignmentInvalid = (uint8_t)-1,
    kCTRubyAlignmentAuto = 0,
    kCTRubyAlignmentStart = 1,
    kCTRubyAlignmentCenter = 2,
    kCTRubyAlignmentEnd = 3,
    kCTRubyAlignmentDistributeLetter = 4,
    kCTRubyAlignmentDistributeSpace = 5,
    kCTRubyAlignmentLineEdge = 6
} CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Ruby overhang Values                                                        */
/* --------------------------------------------------------------------------- */

/*!
    @enum       CTRubyOverhang
    @abstract   These constants specify whether, and on which side, ruby text is allowed to overhang adjacent text if it is wider than the base text.

    @constant   kCTRubyOverhangAuto
                The ruby text can overhang adjacent text on both sides.

    @constant   kCTRubyOverhangStart
                The ruby text can overhang the text that proceeds it.

    @constant   kCTRubyOverhangEnd
                The ruby text can overhang the text that follows it.

    @constant   kCTRubyOverhangNone
                The ruby text cannot overhang the proceeding or following text.
*/

typedef CF_ENUM(uint8_t, CTRubyOverhang) {
    kCTRubyOverhangInvalid = (uint8_t)-1,
    kCTRubyOverhangAuto = 0,
    kCTRubyOverhangStart = 1,
    kCTRubyOverhangEnd = 2,
    kCTRubyOverhangNone = 3
} CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Ruby position Values                                                        */
/* --------------------------------------------------------------------------- */

/*!
    @enum       CTRubyPosition
    @abstract   These constants specify the position of the ruby text with respect to the base text.

    @constant   kCTRubyPositionBefore
                The ruby text is positioned before the base text; i.e. above horizontal text and to the right of vertical text.

    @constant   kCTRubyPositionAfter
                The ruby text is positioned after the base text; i.e. below horizontal text and to the left of vertical text.

    @constant   kCTRubyPositionInterCharacter
                The ruby text is positioned to the right of the base text whether it is horizontal or vertical. This is the way that Bopomofo annotations are attached to Chinese text in Taiwan.

    @constant   kCTRubyPositionInline
                The ruby text follows the base text with no special styling.
*/

typedef CF_ENUM(uint8_t, CTRubyPosition) {
    kCTRubyPositionBefore = 0,
    kCTRubyPositionAfter = 1,
    kCTRubyPositionInterCharacter = 2,
    kCTRubyPositionInline = 3,
    kCTRubyPositionCount
} CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Ruby Annotation Creation */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTRubyAnnotationCreate
    @abstract   Creates an immutable ruby annotation object.

    @discussion Using this function is the easiest and most efficient way to
                create a ruby annotation object.

    @param      alignment
                Specifies how the ruby text and the base text should be aligned relative to each other.

    @param      overhang
                Specifies how the ruby text can overhang adjacent characters.

    @param		sizeFactor
                Specifies the size of the annotation text as a percent of the size of the base text.

    @param      text
                An array of CFStringRef, indexed by CTRubyPosition. Supply NULL for any unused positions.

    @result     This function will return a reference to a CTRubyAnnotation object.
*/

CTRubyAnnotationRef CTRubyAnnotationCreate(
    CTRubyAlignment alignment,
    CTRubyOverhang overhang,
    CGFloat sizeFactor,
    CFStringRef _Nullable text[_Nonnull kCTRubyPositionCount] ) CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/*!
    @const      kCTRubyAnnotationSizeFactorAttributeName
    @abstract   Specifies the size of the annotation text as a percent of the size of the base text.

    @discussion Value must be a CFNumberRef.
*/

CT_EXPORT const CFStringRef kCTRubyAnnotationSizeFactorAttributeName CT_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0), tvos(10.0));

/*!
    @const      kCTRubyAnnotationScaleToFitAttributeName
    @abstract   Treat the size specified in kCTRubyAnnotationSizeFactorAttributeName as the maximum
                scale factor, when the base text size is smaller than annotation text size, we will
                try to scale the annotation font size down so that it will fit the base text without
                overhang or adding extra padding between base text.

    @discussion Value must be a CFBooleanRef. Default is false.
*/

CT_EXPORT const CFStringRef kCTRubyAnnotationScaleToFitAttributeName CT_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0), tvos(10.0));

/*!
    @function   CTRubyAnnotationCreateWithAttributes
    @abstract   Creates an immutable ruby annotation object.

    @discussion Using this function to create a ruby annotation object with more precise
                control of the annotation text.

    @param      alignment
                Specifies how the ruby text and the base text should be aligned relative to each other.

    @param      overhang
                Specifies how the ruby text can overhang adjacent characters.

    @param      position
                The position of the annotation text.

    @param      string
                A string without any formatting, its format will be derived from the attrs specified below.

    @param      attributes
                A attribute dictionary to combine with the string specified above. If you don't specify
                kCTFontAttributeName, the font used by the Ruby annotation will be deduced from the base
                text, with a size factor specified by a CFNumberRef value keyed by
                kCTRubyAnnotationSizeFactorAttributeName.

    @result     This function will return a reference to a CTRubyAnnotation object.
*/

CTRubyAnnotationRef CTRubyAnnotationCreateWithAttributes(
    CTRubyAlignment alignment,
    CTRubyOverhang overhang,
    CTRubyPosition position,
    CFStringRef string,
    CFDictionaryRef attributes ) CT_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0), tvos(10.0));

/*!
    @function   CTRubyAnnotationCreateCopy
    @abstract   Creates an immutable copy of a ruby annotation object.

    @param      rubyAnnotation
                The ruby annotation that you wish to copy.

    @result     If the "rubyAnnotation" reference is valid, then this
                function will return valid reference to an immutable
                CTRubyAnnotation object that is a copy of the one passed into
                "rubyAnnotation".
*/

CTRubyAnnotationRef CTRubyAnnotationCreateCopy(
    CTRubyAnnotationRef rubyAnnotation ) CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


/*!
    @function   CTRubyAnnotationGetAlignment
    @abstract   Get the alignment value of a ruby annotation object.

    @param      rubyAnnotation
                The ruby annotation object.

    @result     If the "rubyAnnotation" reference is valid, then this
                function will return its alignment. Otherwise it will return kCTRubyAlignmentInvalid.
*/

CTRubyAlignment CTRubyAnnotationGetAlignment(
    CTRubyAnnotationRef rubyAnnotation ) CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


/*!
    @function   CTRubyAnnotationGetOverhang
    @abstract   Get the overhang value of a ruby annotation object.

    @param      rubyAnnotation
                The ruby annotation object.

    @result     If the "rubyAnnotation" reference is valid, then this
                function will return its overhang value. Otherwise it will return kCTRubyOverhangInvalid.
*/

CTRubyOverhang CTRubyAnnotationGetOverhang(
    CTRubyAnnotationRef rubyAnnotation ) CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/*!
    @function   CTRubyAnnotationGetSizeFactor
    @abstract   Get the size factor of a ruby annotation object.

    @param      rubyAnnotation
                The ruby annotation object.

    @result     If the "rubyAnnotation" reference is valid, then this
                function will return its sizeFactor. Otherwise it will return 0.
*/

CGFloat CTRubyAnnotationGetSizeFactor(
    CTRubyAnnotationRef rubyAnnotation ) CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


/*!
    @function   CTRubyAnnotationGetTextForPosition
    @abstract   Get the ruby text for a particular position in a ruby annotation.

    @param      rubyAnnotation
                The ruby annotation object.

    @param      position
                The position for which you want to get the ruby text.

    @result     If the "rubyAnnotation" reference and the position are valid, then this
                function will return a CFStringRef for the text. Otherwise it will return NULL.
*/

CFStringRef _Nullable CTRubyAnnotationGetTextForPosition(
    CTRubyAnnotationRef rubyAnnotation,
    CTRubyPosition position ) CT_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));


CF_ASSUME_NONNULL_END
CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
