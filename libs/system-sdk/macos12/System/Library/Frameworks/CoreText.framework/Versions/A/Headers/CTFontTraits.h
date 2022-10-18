/*
 *  CTFontTraits.h
 *  CoreText
 *
 *  Copyright (c) 2006-2018 Apple Inc. All rights reserved.
 *
 */

#ifndef __CTFONTTRAITS__
#define __CTFONTTRAITS__

#include <CoreText/CTDefines.h>

#include <CoreFoundation/CoreFoundation.h>

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*!
    @defined    kCTFontSymbolicTrait
    @abstract   Dictionary key to access the symbolic traits value.
    @discussion Use this key to access the symbolic traits value from the font traits dictionary. The value is returned as a CFNumberRef.
*/
CT_EXPORT const CFStringRef kCTFontSymbolicTrait CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontWeightTrait
    @abstract   Dictionary key to access the weight trait value.
    @discussion Use this key to access the normalized weight trait from the font traits dictionary. The value returned is a CFNumberRef representing a float value between -1.0 and 1.0 for normalized weight. The value of 0.0 corresponds to the regular or medium font weight.
*/
CT_EXPORT const CFStringRef kCTFontWeightTrait CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontWidthTrait
    @abstract   Dictionary key to access the width (condense/expand) trait value.
    @discussion Use this key to access the normalized proportion trait from the font traits dictionary. This value corresponds to the relative inter-glyph spacing for a given font. The value returned is a CFNumberRef representing a float between -1.0 and 1.0. The value of 0.0 corresponds to regular glyph spacing while negative values represent condensed glyph spacing.
*/
CT_EXPORT const CFStringRef kCTFontWidthTrait CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));
/*!
    @defined    kCTFontSlantTrait
    @abstract   Dictionary key to access the slant trait value.
    @discussion Use this key to access the normalized slant angle from the font traits dictionary. The value returned is a CFNumberRef representing a float value between -1.0 and 1.0 for normalized slant angle. The value or 0.0 corresponds to 0 degree clockwise rotation from the vertical and 1.0 corresponds to 30 degrees clockwise rotation.
*/
CT_EXPORT const CFStringRef kCTFontSlantTrait CT_AVAILABLE(macos(10.5), ios(3.2), watchos(2.0), tvos(9.0));

/*!
    @const      kCTFontClassMaskShift
    @abstract   The font class shift.
    @discussion This is used to shift the font class to the upper most 4 bits of the symbolic traits.
*/
enum {
    kCTFontClassMaskShift = 28
};

/*!
    @enum       CTFontSymbolicTraits
    @abstract   Symbolic representation of stylistic font attributes.
    @discussion CTFontSymbolicTraits symbolically describes stylistic aspects of a font. The top 4 bits is used to describe appearance of the font while the lower 28 bits for typeface. The font appearance information represented by the upper 4 bits can be used for stylistic font matching.
*/
typedef CF_OPTIONS(uint32_t, CTFontSymbolicTraits) {
    kCTFontTraitItalic              = (1 << 0),     // Additional detail available via kCTFontSlantTrait
    kCTFontTraitBold                = (1 << 1),     // Additional detail available via kCTFontWeightTrait
    kCTFontTraitExpanded            = (1 << 5),     // Expanded and condensed traits are mutually exclusive
    kCTFontTraitCondensed           = (1 << 6),     // Additional detail available via kCTFontWidthTrait
    kCTFontTraitMonoSpace           = (1 << 10),    // Use fixed-pitch glyphs if available. May have multiple glyph advances (most CJK glyphs may contain two spaces)
    kCTFontTraitVertical            = (1 << 11),    // Use vertical glyph variants and metrics
    kCTFontTraitUIOptimized         = (1 << 12),    // Synthesize appropriate attributes for UI rendering such as control titles if necessary
    kCTFontTraitColorGlyphs         = (1 << 13),    // Color glyphs ('sbix', 'COLR', or 'SVG ') are available. 
    kCTFontTraitComposite           = (1 << 14),    // The font is a CFR (Composite font reference), a cascade list is expected per font. 

    kCTFontTraitClassMask           = (15U << kCTFontClassMaskShift),
                                                    // Mask for the font class

    kCTFontItalicTrait = kCTFontTraitItalic,
    kCTFontBoldTrait = kCTFontTraitBold,
    kCTFontExpandedTrait = kCTFontTraitExpanded,
    kCTFontCondensedTrait = kCTFontTraitCondensed,
    kCTFontMonoSpaceTrait = kCTFontTraitMonoSpace,
    kCTFontVerticalTrait = kCTFontTraitVertical,
    kCTFontUIOptimizedTrait = kCTFontTraitUIOptimized,
    kCTFontColorGlyphsTrait = kCTFontTraitColorGlyphs,
    kCTFontCompositeTrait = kCTFontTraitComposite,
    kCTFontClassMaskTrait = kCTFontTraitClassMask
};

/*!
    @enum       CTFontStylisticClass
    @abstract   Stylistic class values.
    @discussion CTFontStylisticClass classifies certain stylistic qualities of the font. These values correspond closely to the font class values in the OpenType 'OS/2' table. The class values are bundled in the upper four bits of the CTFontSymbolicTraits and can be obtained via the kCTFontTraitClassMask.
*/
typedef CF_OPTIONS(uint32_t, CTFontStylisticClass) {
    kCTFontClassUnknown             = (0 << kCTFontClassMaskShift),
    kCTFontClassOldStyleSerifs      = (1 << kCTFontClassMaskShift),
    kCTFontClassTransitionalSerifs  = (2 << kCTFontClassMaskShift),
    kCTFontClassModernSerifs        = (3 << kCTFontClassMaskShift),
    kCTFontClassClarendonSerifs     = (4 << kCTFontClassMaskShift),
    kCTFontClassSlabSerifs          = (5 << kCTFontClassMaskShift),
    kCTFontClassFreeformSerifs      = (7 << kCTFontClassMaskShift),
    kCTFontClassSansSerif           = (8U << kCTFontClassMaskShift),
    kCTFontClassOrnamentals         = (9U << kCTFontClassMaskShift),
    kCTFontClassScripts             = (10U << kCTFontClassMaskShift),
    kCTFontClassSymbolic            = (12U << kCTFontClassMaskShift),

    kCTFontUnknownClass = kCTFontClassUnknown,
    kCTFontOldStyleSerifsClass = kCTFontClassOldStyleSerifs,
    kCTFontTransitionalSerifsClass = kCTFontClassTransitionalSerifs,
    kCTFontModernSerifsClass = kCTFontClassModernSerifs,
    kCTFontClarendonSerifsClass = kCTFontClassClarendonSerifs,
    kCTFontSlabSerifsClass = kCTFontClassSlabSerifs,
    kCTFontFreeformSerifsClass = kCTFontClassFreeformSerifs,
    kCTFontSansSerifClass = kCTFontClassSansSerif,
    kCTFontOrnamentalsClass = kCTFontClassOrnamentals,
    kCTFontScriptsClass = kCTFontClassScripts,
    kCTFontSymbolicClass = kCTFontClassSymbolic
};

CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif
