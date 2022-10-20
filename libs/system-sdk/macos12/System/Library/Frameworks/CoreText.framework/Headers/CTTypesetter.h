/*
 *  CTTypesetter.h
 *  CoreText
 *
 *  Copyright (c) 2003-2018 Apple Inc. All rights reserved.
 *
 */

/*!
    @header

    Thread Safety Information

    All functions in this header are thread safe unless otherwise specified.
*/

#ifndef __CTTYPESETTER__
#define __CTTYPESETTER__

#include <CoreText/CTLine.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/* --------------------------------------------------------------------------- */
/* Typesetter Types */
/* --------------------------------------------------------------------------- */

typedef const struct CF_BRIDGED_TYPE(id) __CTTypesetter * CTTypesetterRef;


/*!
    @function   CTTypesetterGetTypeID
    @abstract   Returns the CFType of the typesetter object
*/

CFTypeID CTTypesetterGetTypeID( void ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Typesetter Values */
/* --------------------------------------------------------------------------- */

/*!
    @const      kCTTypesetterOptionAllowUnboundedLayout
    @abstract   Allows layout requiring a potentially unbounded amount of work.
    @discussion Value must be a CFBooleanRef. Default is false for clients linked on or after macOS 10.14 or iOS 12.
                Proper Unicode layout of some text requires unreasonable effort;
                unless this option is set to kCFBooleanTrue such inputs will
                result in CTTypesetterCreateWithAttributedStringAndOptions
                returning NULL.
*/

CT_EXPORT const CFStringRef kCTTypesetterOptionAllowUnboundedLayout CT_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0));

/*!
    @const      kCTTypesetterOptionDisableBidiProcessing
    @abstract   Disables bidi processing.
    @discussion Value must be a CFBooleanRef. Default is false.
                Normally, typesetting applies the Unicode Bidirectional
                Algorithm as described in UAX #9. If a typesetter is created
                with this option set to true, no directional reordering is
                performed and any directional control characters are ignored.
*/

CT_EXPORT const CFStringRef kCTTypesetterOptionDisableBidiProcessing CT_DEPRECATED("Deprecated", macos(10.5, 10.8), ios(3.2, 6.0), watchos(2.0, 2.0), tvos(9.0, 9.0));

/*!
    @const      kCTTypesetterOptionForcedEmbeddingLevel
    @abstract   Specifies the embedding level.
    @discussion Value must be a CFNumberRef. Default is unset. Normally,
                typesetting applies the Unicode Bidirectional Algorithm as
                described in UAX #9. If present, this specifies the embedding
                level and any directional control characters are ignored.
*/

CT_EXPORT const CFStringRef kCTTypesetterOptionForcedEmbeddingLevel CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Typesetter Creation */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTTypesetterCreateWithAttributedString
    @abstract   Creates an immutable typesetter object using an attributed
                string.

    @discussion The resultant typesetter can be used to create lines, perform
                line breaking, and do other contextual analysis based on the
                characters in the string.

    @param      string
                The CFAttributedStringRef that you want to typeset. This
                parameter must be filled in with a valid CFAttributedString.

    @result     This function will return a reference to a CTTypesetter.
*/

CTTypesetterRef CTTypesetterCreateWithAttributedString(
    CFAttributedStringRef string ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTTypesetterCreateWithAttributedStringAndOptions
    @abstract   Creates an immutable typesetter object using an attributed
                string and a dictionary of options.

    @discussion The resultant typesetter can be used to create lines, perform
                line breaking, and do other contextual analysis based on the
                characters in the string.

    @param      string
                The CFAttributedStringRef that you want to typeset. This
                parameter must be filled in with a valid CFAttributedString.

    @param      options
                A CFDictionary of typesetter options, or NULL if there are none.

    @result     This function will return either a reference to a CTTypesetter
                or NULL if layout cannot be performed due to an attributed
                string that would require unreasonable effort.

    @seealso    kCTTypesetterOptionAllowUnboundedLayout
*/

CTTypesetterRef _Nullable CTTypesetterCreateWithAttributedStringAndOptions(
    CFAttributedStringRef string,
    CFDictionaryRef _Nullable options ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Typeset Line Creation */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTTypesetterCreateLineWithOffset
    @abstract   Creates an immutable line from the typesetter.

    @discussion The resultant line will consist of glyphs in the correct visual
                order, ready to draw.

    @param      typesetter
                The typesetter which the line will come from.

    @param      stringRange
                The string range which the line will be based on. If the length
                portion of range is set to 0, then the typesetter will continue
                to add glyphs to the line until it runs out of characters in the
                string. The location and length of the range must be within the
                bounds of the string, otherwise the call will fail.

    @param      offset
                The line position offset.

    @result     This function will return a reference to a CTLine.
*/

CTLineRef CTTypesetterCreateLineWithOffset(
    CTTypesetterRef typesetter,
    CFRange stringRange,
    double offset ) CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTTypesetterCreateLine
    @abstract   Equivalent to CTTypesetterCreateLineWithOffset with offset = 0.0.
*/

CTLineRef CTTypesetterCreateLine(
    CTTypesetterRef typesetter,
    CFRange stringRange ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Typeset Line Breaking */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTTypesetterSuggestLineBreakWithOffset
    @abstract   Suggests a contextual line break point based on the width
                provided.

    @discussion The line break can be triggered either by a hard break character
                in the stream or by filling the specified width with characters.

    @param      typesetter
                The typesetter which the line will come from.

    @param      startIndex
                The starting point for the line break calculations. The break
                calculations will include the character starting at startIndex.

    @param      width
                The requested line break width.

    @param      offset
                The line position offset.

    @result     The value returned is a count of the characters from startIndex
                that would cause the line break. This value returned can be used
                to construct a character range for CTTypesetterCreateLine.
*/

CFIndex CTTypesetterSuggestLineBreakWithOffset(
    CTTypesetterRef typesetter,
    CFIndex startIndex,
    double width,
    double offset ) CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTTypesetterSuggestLineBreak
    @abstract   Equivalent to CTTypesetterSuggestLineBreakWithOffset with offset = 0.0.
*/

CFIndex CTTypesetterSuggestLineBreak(
    CTTypesetterRef typesetter,
    CFIndex startIndex,
    double width ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTTypesetterSuggestClusterBreakWithOffset
    @abstract   Suggests a cluster line break point based on the width provided.

    @discussion Suggests a typographic cluster line break point based on the width
                provided. This cluster break is similar to a character break,
                except that it will not break apart linguistic clusters. No other
                contextual analysis will be done. This can be used by the caller
                to implement a different line breaking scheme, such as
                hyphenation. Note that a typographic cluster break can also be
                triggered by a hard break character in the stream.

    @param      typesetter
                The typesetter which the line will come from.

    @param      startIndex
                The starting point for the typographic cluster break
                calculations. The break calculations will include the character
                starting at startIndex.

    @param      width
                The requested typographic cluster break width.

    @param      offset
                The line position offset.

    @result     The value returned is a count of the characters from startIndex
                that would cause the cluster break. This value returned can be
                used to construct a character range for CTTypesetterCreateLine.
*/

CFIndex CTTypesetterSuggestClusterBreakWithOffset(
    CTTypesetterRef typesetter,
    CFIndex startIndex,
    double width,
    double offset ) CT_AVAILABLE(macos(10.6), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTTypesetterSuggestClusterBreak
    @abstract   Equivalent to CTTypesetterSuggestClusterBreakWithOffset with offset = 0.0.
*/

CFIndex CTTypesetterSuggestClusterBreak(
    CTTypesetterRef typesetter,
    CFIndex startIndex,
    double width ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


CF_ASSUME_NONNULL_END
CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
