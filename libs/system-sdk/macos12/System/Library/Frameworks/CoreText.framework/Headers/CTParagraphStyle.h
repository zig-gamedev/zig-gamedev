/*
 *  CTParagraphStyle.h
 *  CoreText
 *
 *  Copyright (c) 2004-2018 Apple Inc. All rights reserved.
 *
 */

/*!
    @header

    Thread Safety Information

    All functions in this header are thread safe unless otherwise specified.
*/

#ifndef __CTPARAGRAPHSTYLE__
#define __CTPARAGRAPHSTYLE__

#include <CoreText/CTDefines.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/* --------------------------------------------------------------------------- */
/* Paragraph Style Types */
/* --------------------------------------------------------------------------- */

typedef const struct CF_RELATED_TYPE(NSParagraphStyle,,) __CTParagraphStyle * CTParagraphStyleRef;


/*!
    @function   CTParagraphStyleGetTypeID
    @abstract   Returns the CFType of the paragraph style object
*/

CFTypeID CTParagraphStyleGetTypeID( void ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Paragraph Style Values */
/* --------------------------------------------------------------------------- */

/*!
    @enum       CTTextAlignment
    @abstract   These constants specify text alignment.

    @constant   kCTTextAlignmentLeft
                Text is visually left-aligned.

    @constant   kCTTextAlignmentRight
                Text is visually right-aligned.

    @constant   kCTTextAlignmentCenter
                Text is visually center-aligned.

    @constant   kCTTextAlignmentJustified
                Text is fully justified. The last line in a paragraph is
                naturally aligned.

    @constant   kCTTextAlignmentNatural
                Use the natural alignment of the text's script.
*/

typedef CF_ENUM(uint8_t, CTTextAlignment) {
    kCTTextAlignmentLeft      = 0,
    kCTTextAlignmentRight     = 1,
    kCTTextAlignmentCenter    = 2,
    kCTTextAlignmentJustified = 3,
    kCTTextAlignmentNatural   = 4,

    kCTLeftTextAlignment CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTTextAlignmentLeft,
    kCTRightTextAlignment CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
    = kCTTextAlignmentRight,
    kCTCenterTextAlignment CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTTextAlignmentCenter,
    kCTJustifiedTextAlignment CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTTextAlignmentJustified,
    kCTNaturalTextAlignment CT_ENUM_DEPRECATED("Deprecated", macos(10.5, 10.11), ios(3.2, 9.0)) CT_ENUM_UNAVAILABLE(watchos, tvos)
        = kCTTextAlignmentNatural
};


/*!
    @enum       CTLineBreakMode
    @abstract   These constants specify what happens when a line is too long for
                its frame.

    @constant   kCTLineBreakByWordWrapping
                Wrapping occurs at word boundaries, unless the word itself doesn't
                fit on a single line.

    @constant   kCTLineBreakByCharWrapping
                Wrapping occurs before the first character that doesn't fit.

    @constant   kCTLineBreakByClipping
                Lines are simply not drawn past the edge of the frame.

    @constant   kCTLineBreakByTruncatingHead
                Each line is displayed so that the end fits in the frame and the
                missing text is indicated by some kind of ellipsis glyph.

    @constant   kCTLineBreakByTruncatingTail
                Each line is displayed so that the beginning fits in the
                container and the missing text is indicated by some kind of
                ellipsis glyph.

    @constant   kCTLineBreakByTruncatingMiddle
                Each line is displayed so that the beginning and end fit in the
                container and the missing text is indicated by some kind of
                ellipsis glyph in the middle.
*/

typedef CF_ENUM(uint8_t, CTLineBreakMode) {
    kCTLineBreakByWordWrapping = 0,
    kCTLineBreakByCharWrapping = 1,
    kCTLineBreakByClipping = 2,
    kCTLineBreakByTruncatingHead = 3,
    kCTLineBreakByTruncatingTail = 4,
    kCTLineBreakByTruncatingMiddle = 5
};


/*!
    @enum       CTWritingDirection
    @abstract   These constants specify the writing direction

    @constant   kCTWritingDirectionNatural
                The writing direction is algorithmically determined
                using the Unicode Bidirectional Algorithm rules P2 and P3.

    @constant   kCTWritingDirectionLeftToRight
                The writing direction is left to right.

    @constant   kCTWritingDirectionRightToLeft
                The writing direction is right to left.
*/

typedef CF_ENUM(int8_t, CTWritingDirection) {
    kCTWritingDirectionNatural = -1,
    kCTWritingDirectionLeftToRight = 0,
    kCTWritingDirectionRightToLeft = 1
};


/*!
    @enum       CTParagraphStyleSpecifier
    @abstract   These constants are used to query and modify the CTParagraphStyle
                object.

    @discussion Each specifier has a type and a default value associated with it.
                The type must always be observed when setting or fetching the
                value from the CTParagraphStyle object. In addition, some
                specifiers affect the behavior of both the framesetter and
                the typesetter, and others only affect the behavior of the
                framesetter; this is also noted below.

    @constant   kCTParagraphStyleSpecifierAlignment
                The text alignment. Natural text alignment is realized as
                left or right alignment, depending on the line sweep direction
                of the first script contained in the paragraph.

                Type: CTTextAlignment
                Default: kCTTextAlignmentNatural
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierFirstLineHeadIndent
                The distance in points from the leading margin of a frame to
                the beginning of the paragraph's first line. This value is always
                nonnegative.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierHeadIndent
                The distance in points from the leading margin of a text
                container to the beginning of lines other than the first.
                This value is always nonnegative.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierTailIndent
                The distance in points from the margin of a frame to the end of
                lines. If positive, this value is the distance from the leading
                margin (for example, the left margin in left-to-right text).
                If 0 or negative, it's the distance from the trailing margin.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierTabStops
                The CTTextTab objects, sorted by location, that define the tab
                stops for the paragraph style.

                Type: CFArray of CTTextTabRef
                Default: 12 left-aligned tabs, spaced by 28.0 points
                Application: CTFramesetter, CTTypesetter


    @constant   kCTParagraphStyleSpecifierDefaultTabInterval
                The document-wide default tab interval. Tabs after the last
                specified by kCTParagraphStyleSpecifierTabStops are placed at
                integer multiples of this distance (if positive).

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter, CTTypesetter


    @constant   kCTParagraphStyleSpecifierLineBreakMode
                The mode that should be used to break lines when laying out
                the paragraph's text.

                Type: CTLineBreakMode
                Default: kCTLineBreakByWordWrapping
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierLineHeightMultiple
                The line height multiple. The natural line height of the
                receiver is multiplied by this factor (if positive) before
                being constrained by minimum and maximum line height.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierMaximumLineHeight
                The maximum height that any line in the frame will occupy,
                regardless of the font size or size of any attached graphic.
                Glyphs and graphics exceeding this height will overlap
                neighboring lines. A maximum height of 0 implies
                no line height limit. This value is always nonnegative.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierMinimumLineHeight
                The minimum height that any line in the frame will occupy,
                regardless of the font size or size of any attached graphic.
                This value is always nonnegative.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierLineSpacing
                Deprecated.
                Use kCTParagraphStyleSpecifierMaximumLineSpacing, kCTParagraphStyleSpecifierMinimumLineSpacing,
                and kCTParagraphStyleSpecifierLineSpacingAdjustment to control
                space between lines.


    @constant   kCTParagraphStyleSpecifierParagraphSpacing
                The space added at the end of the paragraph to separate it from
                the following paragraph. This value is always nonnegative and is
                determined by adding the previous paragraph's
                kCTParagraphStyleSpecifierParagraphSpacing setting and the
                current paragraph's kCTParagraphStyleSpecifierParagraphSpacingBefore
                setting.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierParagraphSpacingBefore
                The distance between the paragraph's top and the beginning of
                its text content.

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierBaseWritingDirection
                The base writing direction of the lines.

                Type: CTWritingDirection
                Default: kCTWritingDirectionNatural
                Application: CTFramesetter, CTTypesetter


    @constant   kCTParagraphStyleSpecifierMaximumLineSpacing
                The maximum space in points between lines within the paragraph
                (commonly known as leading).

                Type: CGFloat
                Default: some large number.
                Application: CTFramesetter
 
 
    @constant   kCTParagraphStyleSpecifierMinimumLineSpacing
                The minimum space in points between lines within the paragraph
                (commonly known as leading).

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter
 
 
    @constant   kCTParagraphStyleSpecifierLineSpacingAdjustment
                The space in points added between lines within the paragraph
                (commonly known as leading). 

                Type: CGFloat
                Default: 0.0
                Application: CTFramesetter


    @constant   kCTParagraphStyleSpecifierLineBoundsOptions
                The options controlling the alignment of the line edges with
                the leading and trailing margins.

                Type: CTLineBoundsOptions
                Default: 0 (no options)
                Application: CTTypesetter
*/

typedef CF_ENUM(uint32_t, CTParagraphStyleSpecifier) {
    kCTParagraphStyleSpecifierAlignment = 0,
    kCTParagraphStyleSpecifierFirstLineHeadIndent = 1,
    kCTParagraphStyleSpecifierHeadIndent = 2,
    kCTParagraphStyleSpecifierTailIndent = 3,
    kCTParagraphStyleSpecifierTabStops = 4,
    kCTParagraphStyleSpecifierDefaultTabInterval = 5,
    kCTParagraphStyleSpecifierLineBreakMode = 6,
    kCTParagraphStyleSpecifierLineHeightMultiple = 7,
    kCTParagraphStyleSpecifierMaximumLineHeight = 8,
    kCTParagraphStyleSpecifierMinimumLineHeight = 9,
    kCTParagraphStyleSpecifierLineSpacing CT_ENUM_DEPRECATED("See documentation for replacements", macos(10.5, 10.8), ios(3.2, 6.0)) CT_ENUM_UNAVAILABLE(watchos, tvos) = 10,
    kCTParagraphStyleSpecifierParagraphSpacing = 11,
    kCTParagraphStyleSpecifierParagraphSpacingBefore = 12,
    kCTParagraphStyleSpecifierBaseWritingDirection = 13,
    kCTParagraphStyleSpecifierMaximumLineSpacing = 14,
    kCTParagraphStyleSpecifierMinimumLineSpacing = 15,
    kCTParagraphStyleSpecifierLineSpacingAdjustment = 16,
    kCTParagraphStyleSpecifierLineBoundsOptions = 17,

    kCTParagraphStyleSpecifierCount
};


/*!
    @struct     CTParagraphStyleSetting
    @abstract   This structure is used to alter the paragraph style.

    @field      spec
                The specifier of the setting.

    @field      valueSize
                The size of the value pointed to by the "value" field. This
                must match the size of the value required by the
                CTParagraphStyleSpecifier set in the "spec" field.

    @field      value
                A reference to the value of the setting specified by the
                "spec" field. The value must be in the proper range for the
                spec value. The value must also be at least valueSize.
*/
typedef struct CTParagraphStyleSetting
{
    CTParagraphStyleSpecifier spec;
    size_t valueSize;
    const void * value;

} CTParagraphStyleSetting;


/* --------------------------------------------------------------------------- */
/* Paragraph Style Creation */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTParagraphStyleCreate
    @abstract   Creates an immutable paragraph style.

    @discussion Using this function is the easiest and most efficient way to
                create a paragraph style. Paragraph styles should be kept
                immutable for totally lock-free operation.

                If an invalid paragraph style setting specifier is passed into
                the "settings" parameter, nothing bad will happen but just don't
                expect to be able to query for this value. This is to allow
                backwards compatibility with style setting specifiers that may
                be introduced in future versions.

    @param      settings
                The settings that you wish to pre-load the paragraph style
                with. If you wish to specify the default set of settings,
                then this parameter may be set to NULL.

    @param      settingCount
                The number of settings that you have specified in the
                "settings" parameter. This must be greater than or equal
                to zero.

    @result     If the paragraph style creation was successful, this function
                will return a valid reference to an immutable CTParagraphStyle
                object. Otherwise, this function will return NULL.
*/

CTParagraphStyleRef CTParagraphStyleCreate(
    const CTParagraphStyleSetting * _Nullable settings,
    size_t settingCount ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTParagraphStyleCreateCopy
    @abstract   Creates an immutable copy of a paragraph style.

    @param      paragraphStyle
                The style that you wish to copy.

    @result     If the "paragraphStyle" reference is valid, then this
                function will return valid reference to an immutable
                CTParagraphStyle object that is a copy of the one passed into
                "paragraphStyle".
*/

CTParagraphStyleRef CTParagraphStyleCreateCopy(
    CTParagraphStyleRef paragraphStyle ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Paragraph Style Access */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTParagraphStyleGetValueForSpecifier
    @abstract   Obtains the current value for a single setting specifier.

    @discussion This function will return the current value of the specifier
                whether or not the user had actually set it. If the user has
                not set it, this function will return the default value.

                If an invalid paragraph style setting specifier is passed into
                the "spec" parameter, nothing bad will happen and the buffer
                value will simply be zeroed out. This is to allow backwards
                compatibility with style setting specifier that may be introduced
                in future versions.

    @param      paragraphStyle
                The paragraph style that you wish to get the value from.

    @param      spec
                The setting specifier that you want to get the value for.

    @param      valueBufferSize
                The size of the buffer pointed to by the "valueBuffer" parameter.
                This value must be at least as large as the size the required by
                the CTParagraphSpecifier value set in the "spec" parameter.

    @param      valueBuffer
                The buffer where the requested setting value will be written
                upon successful completion. The buffer's size needs to be at least
                as large as the value passed into "valueBufferSize".

    @result     This function will return "true" if the valueBuffer had been
                successfully filled. Otherwise, this function will return false,
                indicating that one or more of the parameters is not valid.
*/

bool CTParagraphStyleGetValueForSpecifier(
    CTParagraphStyleRef paragraphStyle,
    CTParagraphStyleSpecifier spec,
    size_t valueBufferSize,
    void * valueBuffer ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


CF_ASSUME_NONNULL_END
CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
