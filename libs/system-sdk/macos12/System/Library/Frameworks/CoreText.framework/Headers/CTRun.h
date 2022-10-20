/*
 *  CTRun.h
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

#ifndef __CTRUN__
#define __CTRUN__

#include <CoreText/CTDefines.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/* --------------------------------------------------------------------------- */
/* Glyph Run Types */
/* --------------------------------------------------------------------------- */

typedef const struct CF_BRIDGED_TYPE(id) __CTRun * CTRunRef;


/*!
    @enum       CTRunStatus
    @abstract   A bitfield passed back by CTRunGetStatus that is used to
                indicate the disposition of the run.

    @constant   kCTRunStatusNoStatus
                The run has no special attributes.

    @constant   kCTRunStatusRightToLeft
                When set, the run is right to left.

    @constant   kCTRunStatusNonMonotonic
                When set, the run has been reordered in some way such that
                the string indices associated with the glyphs are no longer
                strictly increasing (for left to right runs) or decreasing
                (for right to left runs).

    @constant   kCTRunStatusHasNonIdentityMatrix
                When set, the run requires a specific text matrix to be set
                in the current CG context for proper drawing.
*/

typedef CF_OPTIONS(uint32_t, CTRunStatus)
{
    kCTRunStatusNoStatus = 0,
    kCTRunStatusRightToLeft = (1 << 0),
    kCTRunStatusNonMonotonic = (1 << 1),
    kCTRunStatusHasNonIdentityMatrix = (1 << 2)
};

 
/*!
    @function   CTRunGetTypeID
    @abstract   Returns the CFType of the run object
*/

CFTypeID CTRunGetTypeID( void ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/* --------------------------------------------------------------------------- */
/* Glyph Run Access */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTRunGetGlyphCount
    @abstract   Gets the glyph count for the run.

    @param      run
                The run whose glyph count you wish to access.

    @result     The number of glyphs that the run contains. It is totally
                possible that this function could return a value of zero,
                indicating that there are no glyphs in this run.
*/

CFIndex CTRunGetGlyphCount(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetAttributes
    @abstract   Returns the attribute dictionary that was used to create the
                glyph run.

    @discussion This dictionary returned is either the same exact one that was
                set as an attribute dictionary on the original attributed string
                or a dictionary that has been manufactured by the layout engine.
                Attribute dictionaries can be manufactured in the case of font
                substitution or if they are missing critical attributes.

    @param      run
                The run whose attributes you wish to access.

    @result     The attribute dictionary.
*/

CFDictionaryRef CTRunGetAttributes(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetStatus
    @abstract   Returns the run's status.

    @discussion In addition to attributes, runs also have status that can be
                used to expedite certain operations. Knowing the direction and
                ordering of a run's glyphs can aid in string index analysis,
                whereas knowing whether the positions reference the identity
                text matrix can avoid expensive comparisons. Note that this
                status is provided as a convenience, since this information is
                not strictly necessary but can certainly be helpful.

    @param      run
                The run whose status you wish to access.

    @result     The run's status.
*/

CTRunStatus CTRunGetStatus(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetGlyphsPtr
    @abstract   Returns a direct pointer for the glyph array stored in the run.

    @discussion The glyph array will have a length equal to the value returned by
                CTRunGetGlyphCount. The caller should be prepared for this
                function to return NULL even if there are glyphs in the stream.
                Should this function return NULL, the caller will need to
                allocate their own buffer and call CTRunGetGlyphs to fetch the
                glyphs.

    @param      run
                The run whose glyphs you wish to access.

    @result     A valid pointer to an array of CGGlyph structures or NULL.
*/

const CGGlyph * _Nullable CTRunGetGlyphsPtr(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetGlyphs
    @abstract   Copies a range of glyphs into user-provided buffer.

    @param      run
                The run whose glyphs you wish to copy.

    @param      range
                The range of glyphs to be copied, with the entire range having a
                location of 0 and a length of CTRunGetGlyphCount. If the length
                of the range is set to 0, then the operation will continue from
                the range's start index to the end of the run.

    @param      buffer
                The buffer where the glyphs will be copied to. The buffer must be
                allocated to at least the value specified by the range's length.
*/

void CTRunGetGlyphs(
    CTRunRef run,
    CFRange range,
    CGGlyph buffer[_Nonnull] ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetPositionsPtr
    @abstract   Returns a direct pointer for the glyph position array stored in
                the run.

    @discussion The glyph positions in a run are relative to the origin of the
                line containing the run. The position array will have a length
                equal to the value returned by CTRunGetGlyphCount. The caller
                should be prepared for this function to return NULL even if there
                are glyphs in the stream. Should this function return NULL, the
                caller will need to allocate their own buffer and call
                CTRunGetPositions to fetch the positions.

    @param      run
                The run whose positions you wish to access.

    @result     A valid pointer to an array of CGPoint structures or NULL.
*/

const CGPoint * _Nullable CTRunGetPositionsPtr(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetPositions
    @abstract   Copies a range of glyph positions into a user-provided buffer.

    @discussion The glyph positions in a run are relative to the origin of the
                line containing the run.

    @param      run
                The run whose positions you wish to copy.

    @param      range
                The range of glyph positions to be copied, with the entire range
                having a location of 0 and a length of CTRunGetGlyphCount. If the
                length of the range is set to 0, then the operation will continue
                from the range's start index to the end of the run.

    @param      buffer
                The buffer where the glyph positions will be copied to. The buffer
                must be allocated to at least the value specified by the range's
                length.
*/

void CTRunGetPositions(
    CTRunRef run,
    CFRange range,
    CGPoint buffer[_Nonnull] ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetAdvancesPtr
    @abstract   Returns a direct pointer for the glyph advance array stored in
                the run.

    @discussion The advance array will have a length equal to the value returned
                by CTRunGetGlyphCount. The caller should be prepared for this
                function to return NULL even if there are glyphs in the stream.
                Should this function return NULL, the caller will need to
                allocate their own buffer and call CTRunGetAdvances to fetch the
                advances. Note that advances alone are not sufficient for correctly
                positioning glyphs in a line, as a run may have a non-identity
                matrix or the initial glyph in a line may have a non-zero origin;
                callers should consider using positions instead.

    @param      run
                The run whose advances you wish to access.

    @result     A valid pointer to an array of CGSize structures or NULL.
*/

const CGSize * _Nullable CTRunGetAdvancesPtr(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetAdvances
    @abstract   Copies a range of glyph advances into a user-provided buffer.

    @param      run
                The run whose advances you wish to copy.

    @param      range
                The range of glyph advances to be copied, with the entire range
                having a location of 0 and a length of CTRunGetGlyphCount. If the
                length of the range is set to 0, then the operation will continue
                from the range's start index to the end of the run.

    @param      buffer
                The buffer where the glyph advances will be copied to. The buffer
                must be allocated to at least the value specified by the range's
                length.
*/

void CTRunGetAdvances(
    CTRunRef run,
    CFRange range,
    CGSize buffer[_Nonnull] ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetStringIndicesPtr
    @abstract   Returns a direct pointer for the string indices stored in the run.

    @discussion The indices are the character indices that originally spawned the
                glyphs that make up the run. They can be used to map the glyphs in
                the run back to the characters in the backing store. The string
                indices array will have a length equal to the value returned by
                CTRunGetGlyphCount. The caller should be prepared for this
                function to return NULL even if there are glyphs in the stream.
                Should this function return NULL, the caller will need to allocate
                their own buffer and call CTRunGetStringIndices to fetch the
                indices.

    @param      run
                The run whose string indices you wish to access.

    @result     A valid pointer to an array of CFIndex structures or NULL.
*/

const CFIndex * _Nullable CTRunGetStringIndicesPtr(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetStringIndices
    @abstract   Copies a range of string indices into a user-provided buffer.

    @discussion The indices are the character indices that originally spawned the
                glyphs that make up the run. They can be used to map the glyphs
                in the run back to the characters in the backing store.

    @param      run
                The run whose string indices you wish to copy.

    @param      range
                The range of string indices to be copied, with the entire range
                having a location of 0 and a length of CTRunGetGlyphCount. If the
                length of the range is set to 0, then the operation will continue
                from the range's start index to the end of the run.

    @param      buffer
                The buffer where the string indices will be copied to. The buffer
                must be allocated to at least the value specified by the range's
                length.
*/

void CTRunGetStringIndices(
    CTRunRef run,
    CFRange range,
    CFIndex buffer[_Nonnull] ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetStringRange
    @abstract   Gets the range of characters that originally spawned the glyphs
                in the run.

    @param      run
                The run whose string range you wish to access.

    @result     Returns the range of characters that originally spawned the
                glyphs. If run is invalid, this will return an empty range.
*/

CFRange CTRunGetStringRange(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetTypographicBounds
    @abstract   Gets the typographic bounds of the run.

    @param      run
                The run that you want to calculate the typographic bounds for.

    @param      range
                The range of glyphs to be measured, with the entire range having
                a location of 0 and a length of CTRunGetGlyphCount. If the length
                of the range is set to 0, then the operation will continue from
                the range's start index to the end of the run.

    @param      ascent
                Upon return, this parameter will contain the ascent of the run.
                This may be set to NULL if not needed.

    @param      descent
                Upon return, this parameter will contain the descent of the run.
                This may be set to NULL if not needed.

    @param      leading
                Upon return, this parameter will contain the leading of the run.
                This may be set to NULL if not needed.

    @result     The typographic width of the run. If run or range is
                invalid, then this function will always return zero.
*/

double CTRunGetTypographicBounds(
    CTRunRef run,
    CFRange range,
    CGFloat * _Nullable ascent,
    CGFloat * _Nullable descent,
    CGFloat * _Nullable leading ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetImageBounds
    @abstract   Calculates the image bounds for a glyph range.

    @discussion The image bounds for a run is the union of all non-empty glyph
                bounding rects, each positioned as it would be if drawn using
                CTRunDraw using the current context (for clients linked against
                macOS High Sierra or iOS 11 and later) or the text position of
                the supplied context (for all others). Note that the result is
                ideal and does not account for raster coverage due to rendering.
                This function is purely a convenience for using glyphs as an
                image and should not be used for typographic purposes.

    @param      run
                The run that you want to calculate the image bounds for.

    @param      context
                The context which the image bounds will be calculated for or NULL,
                in which case the bounds are relative to CGPointZero.

    @param      range
                The range of glyphs to be measured, with the entire range having
                a location of 0 and a length of CTRunGetGlyphCount. If the length
                of the range is set to 0, then the operation will continue from
                the range's start index to the end of the run.

    @result     A rect that tightly encloses the paths of the run's glyphs. The
                rect origin will match the drawn position of the requested range;
                that is, it will be translated by the supplied context's text
                position and the positions of the individual glyphs. If the run
                or range is invalid, CGRectNull will be returned.

    @seealso    CTRunGetTypographicBounds
*/

CGRect CTRunGetImageBounds(
    CTRunRef run,
    CGContextRef _Nullable context,
    CFRange range ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetTextMatrix
    @abstract   Returns the text matrix needed to draw this run.

    @discussion To properly draw the glyphs in a run, the fields 'tx' and 'ty' of
                the CGAffineTransform returned by this function should be set to
                the current text position.

    @param      run
                The run object from which to get the text matrix.

    @result     A CGAffineTransform.
*/

CGAffineTransform CTRunGetTextMatrix(
    CTRunRef run ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunGetBaseAdvancesAndOrigins
    @abstract   Copies a range of base advances and/or origins into user-provided
                buffers.

    @discussion A run's base advances and origins determine the positions of its
                glyphs but require additional processing before being used for
                drawing. Similar to the advances returned by CTRunGetAdvances,
                base advances are the displacement from the origin of a glyph
                to the origin of the next glyph, except base advances do not
                include any positioning the font layout tables may have done
                relative to another glyph (such as a mark relative to its base).
                The actual position of the current glyph is determined by the
                displacement of its origin from the starting position, and the
                position of the next glyph by the displacement of the current
                glyph's base advance from the starting position.

    @param      runRef
                The run whose base advances and/or origins you wish to copy.

    @param      range
                The range of values to be copied. If the length of the
                range is set to 0, then the copy operation will continue from the
                range's start index to the end of the run.

    @param      advancesBuffer
                The buffer where the base advances will be copied to, or NULL.
                If not NULL, the buffer must allow for at least as many elements
                as specified by the range's length.

    @param      originsBuffer
                The buffer where the origins will be copied to, or NULL. If not
                NULL, the buffer must allow for at least as many elements as
                specified by the range's length.
*/

void CTRunGetBaseAdvancesAndOrigins(
    CTRunRef runRef,
    CFRange range,
    CGSize advancesBuffer[_Nullable],
    CGPoint originsBuffer[_Nullable] ) CT_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));


/*!
    @function   CTRunDraw
    @abstract   Draws a complete run or part of one.

    @discussion This is a convenience call, since the run could also be drawn by
                accessing its glyphs, positions, and text matrix. Unlike when
                drawing the entire line containing the run with CTLineDraw, the
                run's underline (if any) will not be drawn, since the underline's
                appearance may depend on other runs in the line. This call may
                leave the graphics context in any state and does not flush the
                context after drawing. This call also expects a text matrix with
                `y` values increasing from bottom to top; a flipped text matrix
                may result in misplaced diacritics.

    @param      run
                The run that you want to draw.

    @param      context
                The context to draw the run to.

    @param      range
                The range of glyphs to be drawn, with the entire range having a
                location of 0 and a length of CTRunGetGlyphCount. If the length
                of the range is set to 0, then the operation will continue from
                the range's start index to the end of the run.
*/

void CTRunDraw(
    CTRunRef run,
    CGContextRef context,
    CFRange range ) CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));


CF_ASSUME_NONNULL_END
CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif
