/*
     File:       QD/ATSUnicodeTypes.h
 
     Contains:   ATSUI types and constants.
 
     Version:    Quickdraw-291~1
 
     Copyright:  � 2003-2008 by Apple Inc. all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSUNICODETYPES__
#define __ATSUNICODETYPES__

#ifndef __ATSLAYOUTTYPES__
#include <ATS/ATSLayoutTypes.h>
#endif

// The #include of Quickdraw.h is not needed to compile the ATSUI framework
// Removing it might break those that were relying on an implicit #include
#ifndef __QUICKDRAW__
#include <QD/Quickdraw.h>
#endif

#ifndef __SFNTTYPES__
#include <ATS/SFNTTypes.h>
#endif

#ifndef __SFNTLAYOUTTYPES__
#include <ATS/SFNTLayoutTypes.h>
#endif

#ifndef __ATSTYPES__
#include <ATS/ATSTypes.h>
#endif


/* See also ATSLayoutTypes.h for more ATSUI-related types and constants */
/* ---------------------------------------------------------------------------- */
/* ATSUI types and related constants                                            */
/* ---------------------------------------------------------------------------- */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)


/*
 *  ATSUTextLayout
 *  
 *  Discussion:
 *    Text layout objects are the heart of ATSUI. These opaque objects
 *    associate Unicode text with style runs, store information about
 *    softbreaks, line and layout controls, and other information about
 *    the text. Text drawing is done by passing a valid layout object
 *    and a range of text to draw to the function ATSUDrawText, after
 *    initial setup work on the layout has been done. See the
 *    definitions of the functions ATSUCreateTextLayout and
 *    ATSUCreateTextLayoutWithTextPtr for more information about
 *    creating layouts.
 */
typedef struct OpaqueATSUTextLayout*    ATSUTextLayout;

/*
 *  ATSUStyle
 *  
 *  Discussion:
 *    Style objects retain information about text such as font, point
 *    size, color and other attributes. Styles are associated with text
 *    through a layout object. See the definitions of the functions
 *    ATSUSetRunStyle and ATSUCreateTextLayoutWithTextPtr for more
 *    information about assigning styles to runs of text in a layout
 *    object. See the definitions of the functions ATSUCreateStyle and
 *    ATSUSetAttributes for more information on creating and
 *    manipulating styles.
 */
typedef struct OpaqueATSUStyle*         ATSUStyle;

/*
 *  ATSUFontFallbacks
 *  
 *  Discussion:
 *    ATSUFontFallbacks objects are used to store the desired font
 *    fallback mode, list, and associated cache information. See the
 *    definitions of ATSUFontFallbackMethod and ATSUSetObjFontFallbacks
 *    for more information about setting up font fallbacks.
 */
typedef struct OpaqueATSUFontFallbacks*  ATSUFontFallbacks;

/*
 *  ATSUTextMeasurement
 *  
 *  Discussion:
 *    ATSUTextMeasurement is specific to ATSUI in that measurement
 *    quantities are in fractional Fixed format instead of shorts used
 *    in QuickDraw Text. This provides exact outline metrics and line
 *    specifications such as line width, ascent, descent, and so on.
 *    See FixMath.h for conversion functions for Fixed numbers.
 */
typedef Fixed                           ATSUTextMeasurement;

/*
 *  ATSUFontID
 *  
 *  Discussion:
 *    ATSUFontID indicates a particular font family and face.
 *    ATSUFontID's are not guaranteed to remain constant across
 *    sessions. Clients should use the font's unique name to get a font
 *    token to store in documents which is guaranteed to remain
 *    constant across sessions.
 */
typedef FMFont                          ATSUFontID;

/*
 *  ATSUFontFeatureType
 *  
 *  Discussion:
 *    Used to identify a font feature type for a particular font. See
 *    the definitions of the functions ATSUGetAllFontFeatures and
 *    ATSUGetFontFeature for more information about font features.
 */
typedef UInt16                          ATSUFontFeatureType;

/*
 *  ATSUFontFeatureSelector
 *  
 *  Discussion:
 *    Used to identify a font feature selector for a particular font.
 *    See the definitions of the functions ATSUGetAllFontFeatures and
 *    ATSUGetFontFeature for more information about font features.
 */
typedef UInt16                          ATSUFontFeatureSelector;

/*
 *  ATSUFontVariationAxis
 *  
 *  Discussion:
 *    Used to identify a font variation axis for a particular font. See
 *    the definitions of the functions ATSUGetAllFontVariations and
 *    ATSUGetFontVariation for more information about font variations.
 */
typedef FourCharCode                    ATSUFontVariationAxis;

/*
 *  ATSUFontVariationValue
 *  
 *  Discussion:
 *    Used to identify a font variation value for a particular font.
 *    See the definitions of the functions ATSUGetAllFontVariations and
 *    ATSUGetFontVariation for more information about font variations.
 */
typedef Fixed                           ATSUFontVariationValue;

/*
 *  ATSUAttributeTag
 *  
 *  Summary:
 *    Constants used for style attributes, line controls, and layout
 *    controls.
 *  
 *  Discussion:
 *    The following constants are used to change settings in text
 *    layout and style objects. Use the functions ATSUSetLineControls,
 *    ATSUSetLayoutControls, and ATSUSetAttributes to set these values
 *    in lines, layouts, and styles, respectively. Apple reserves tag
 *    values 0 to 65535 (0 to 0x0000FFFF). ATSUI clients may create
 *    their own tags with any other value.
 */
typedef UInt32 ATSUAttributeTag;
enum {

  /*
   * (Type: ATSUTextMeasurement) (Default value: 0) Must not be less
   * than zero. May be set as a line or layout control.
   */
  kATSULineWidthTag             = 1,

  /*
   * (Type: Fixed) (Default value: 0) Angle is specified in degrees in
   * right-handed coordinate system. May be set as a line control.
   */
  kATSULineRotationTag          = 2,

  /*
   * (Type: Boolean) (Default value: GetSysDirection()) Must be 0 or 1.
   * See below for convenience constants. May be set as a layout
   * control.
   */
  kATSULineDirectionTag         = 3,

  /*
   * (Type: Fract) (Default value: kATSUNoJustification) May be set as
   * a line or layout control.
   */
  kATSULineJustificationFactorTag = 4,

  /*
   * (Type: Fract) (Default value: kATSUStartAlignment) May be set as a
   * line or layout control.
   */
  kATSULineFlushFactorTag       = 5,

  /*
   * (Type: BslnBaselineRecord) (Default value: all zeros) Calculated
   * from other style attributes (e.g., font and point size). May be
   * set as a line or layout control.
   */
  kATSULineBaselineValuesTag    = 6,

  /*
   * (Type: ATSLineLayoutOptions) (Default value: all zeros) See
   * ATSLayoutTypes.h for a definition of the ATSLineLayoutOptions type
   * and a list of possible values. May be set as a line or layout
   * control.
   */
  kATSULineLayoutOptionsTag     = 7,

  /*
   * (Type: ATSUTextMeasurement) (Default value: determined by font(s))
   * Must not be less than zero. Starting in Mac OS X 10.2, you can
   * retrieve this value as a line or layout control, even if you have
   * not explicitly set it. This makes it easy to calculate line
   * height. May be set as a line or layout control.
   */
  kATSULineAscentTag            = 8,

  /*
   * (Type: ATSUTextMeasurement) (Default value: determined by font(s))
   * Must not be less than zero. Starting in Mac OS X, you can retrieve
   * this value as a line or layout control, even if you have not
   * explicitly set it. This makes it easy to calculate line height.
   * May be set as a line or layout control.
   */
  kATSULineDescentTag           = 9,

  /*
   * (Type: RegionCode) (Default value: kTextRegionDontCare) See
   * Script.h for possible values. May be set as a line or layout
   * control.
   */
  kATSULineLangRegionTag        = 10,

  /*
   * (Type: TextBreakLocatorRef) (Default value: NULL) See
   * UnicodeUtilities.h for more information on creating a
   * TextBreakLocator. May be set as a line or layout control.
   */
  kATSULineTextLocatorTag       = 11,

  /*
   * (Type: ATSULineTruncation) (Default value: kATSUTruncateNone) See
   * the definition of ATSULineTruncation for possible values. May be
   * set as a line or layout control.
   */
  kATSULineTruncationTag        = 12,

  /*
   * (Type: ATSUFontFallbacks) (Default value: current global fallback
   * state) The current global fallback state is determined using the
   * ATSUSetFontFallbacks function. The use of this function is not
   * recommended. Instead, use the functions ATSUCreateFontFallbacks
   * and ATSUSetObjFontFallbacks to create a ATSUFontFallbacks object,
   * and then use the kATSULineFontFallbacksTag attribute to set the
   * font fallbacks object as a layout control. See the definition of
   * ATSUFontFallbacks for more information. May be set as a layout
   * control.
   */
  kATSULineFontFallbacksTag     = 13,

  /*
   * (Type: CFStringRef) (Default value: user setting in System
   * Preferences) Indicates current setting for the decimal separator.
   * This affects the behavior of decimal tabs. May be set as a line or
   * layout control.
   */
  kATSULineDecimalTabCharacterTag = 14,

  /*
   * (Type: ATSULayoutOperationOverrideSpecifier) (Default value: NULL)
   * See ATSLayoutTypes.h for a definition of the
   * ATSULayoutOperationOverrideSpecifier structure. May be set as a
   * layout control.
   */
  kATSULayoutOperationOverrideTag = 15,

  /*
   * (Type: CGColorRef) (Default value: user setting in System
   * Preferences) Indicates current setting for the highlight color.
   * May be set as a line or layout control.
   */
  kATSULineHighlightCGColorTag  = 17,

  /*
   * This is just for convenience. It is the upper limit of the line
   * and layout tags.
   */
  kATSUMaxLineTag               = 18,

  /*
   * This tag is obsolete. Please use kATSULineLangRegionTag instead.
   */
  kATSULineLanguageTag          = 10,

  /*
   * (Type: CGContextRef) (Default value: NULL) Use this tag to produce
   * Quartz rendering with ATSUI. See the definitions of the functions
   * QDBeginCGContext and QDEndCGContext in Quickdraw.h for more
   * information about creating a CGContext from a graphics port. May
   * be set as a layout control.
   */
  kATSUCGContextTag             = 32767,

  /*
   * (Type: Boolean) (Default value: false) For compatability purposes
   * only. Choosing typographic styles from font families is preferred.
   * Note this tag will produce a synthetic style for fonts that do not
   * have a typographic style. May be set as a style attribute.
   */
  kATSUQDBoldfaceTag            = 256,

  /*
   * (Type: Boolean) (Default value: false) For compatability purposes
   * only. Choosing typographic styles from font families is preferred.
   * Note this tag will produce a synthetic style for fonts that do not
   * have a typographic style. May be set as a style attribute.
   */
  kATSUQDItalicTag              = 257,

  /*
   * (Type: Boolean) (Default value: false) For compatability purposes
   * only. May be set as a style attribute.
   */
  kATSUQDUnderlineTag           = 258,

  /*
   * (Type: Boolean) (Default value: false) For compatability purposes
   * only. May be set as a style attribute.
   */
  kATSUQDCondensedTag           = 259,

  /*
   * (Type: Boolean) (Default value: false) For compatability purposes
   * only. May be set as a style attribute.
   */
  kATSUQDExtendedTag            = 260,

  /*
   * (Type: ATSUFontID) (Default value: LMGetApFontID() or if not
   * valid, LMGetSysFontFam()) May be set as a style attribute.
   */
  kATSUFontTag                  = 261,

  /*
   * (Type: Fixed) (Default value: Long2Fix(LMGetSysFontSize())) May be
   * set as a style attribute.
   */
  kATSUSizeTag                  = 262,

  /*
   * (Type: RGBColor) (Default value: (0, 0, 0)) May be set as a style
   * attribute.
   */
  kATSUColorTag                 = 263,

  /*
   * (Type: RegionCode) (Default value:
   * GetScriptManagerVariable(smRegionCode)) See Script.h for a list of
   * possible values. May be set as a style attribute.
   */
  kATSULangRegionTag            = 264,

  /*
   * (Type: ATSUVerticalCharacterType) (Default value:
   * kATSUStronglyHorizontal) See the definition of
   * ATSUVerticalCharacterType for a list of possible values. May be
   * set as a style attribute.
   */
  kATSUVerticalCharacterTag     = 265,

  /*
   * (Type: ATSUTextMeasurement) (Default value: kATSUseGlyphAdvance)
   * Must not be less than zero. May be set as a style attribute.
   */
  kATSUImposeWidthTag           = 266,

  /*
   * (Type: Fixed) (Default value: 0) May be set as a style attribute.
   */
  kATSUBeforeWithStreamShiftTag = 267,

  /*
   * (Type: Fixed) (Default value: 0) May be set as a style attribute.
   */
  kATSUAfterWithStreamShiftTag  = 268,

  /*
   * (Type: Fixed) (Default value: 0) May be set as a style attribute.
   */
  kATSUCrossStreamShiftTag      = 269,

  /*
   * (Type: Fixed) (Default value: kATSNoTracking) May be set as a
   * style attribute.
   */
  kATSUTrackingTag              = 270,

  /*
   * (Type: Fract) (Default value: 0) May be set as a style attribute.
   */
  kATSUHangingInhibitFactorTag  = 271,

  /*
   * (Type: Fract) (Default value: 0) May be set as a style attribute.
   */
  kATSUKerningInhibitFactorTag  = 272,

  /*
   * (Type: Fixed) (Default value: 0) Must be between -1.0 and 1.0. May
   * be set as a style attribute.
   */
  kATSUDecompositionFactorTag   = 273,

  /*
   * (Type: BslnBaselineClass) (Default value: kBSLNRomanBaseline) See
   * SFNTLayoutTypes.h for more information. Use the constant
   * kBSLNNoBaselineOverride to use intrinsic baselines. May be set as
   * a style attribute.
   */
  kATSUBaselineClassTag         = 274,

  /*
   * (Type: ATSJustPriorityWidthDeltaOverrides) (Default value: all
   * zeros) See ATSLayoutTypes.h for more information. May be set as a
   * style attribute.
   */
  kATSUPriorityJustOverrideTag  = 275,

  /*
   * (Type: Boolean) (Default value: false) When set to true, ligatures
   * and compound characters will not have divisable components. May be
   * set as a style attribute.
   */
  kATSUNoLigatureSplitTag       = 276,

  /*
   * (Type: Boolean) (Default value: false) When set to true, ATSUI
   * will not use a glyph's angularity to determine its boundaries. May
   * be set as a style attribute.
   */
  kATSUNoCaretAngleTag          = 277,

  /*
   * (Type: Boolean) (Default value: false) When set to true, ATSUI
   * will suppress automatic cross kerning (defined by font). May be
   * set as a style attribute.
   */
  kATSUSuppressCrossKerningTag  = 278,

  /*
   * (Type: Boolean) (Default value: false) When set to true, ATSUI
   * will suppress glyphs' automatic optical positional alignment. May
   * be set as a style attribute.
   */
  kATSUNoOpticalAlignmentTag    = 279,

  /*
   * (Type: Boolean) (Default value: false) When set to true, ATSUI
   * will force glyphs to hang beyond the line boundaries. May be set
   * as a style attribute.
   */
  kATSUForceHangingTag          = 280,

  /*
   * (Type: Boolean) (Default value: false) When set to true, ATSUI
   * will not perform post-compensation justification if needed. May be
   * set as a style attribute.
   */
  kATSUNoSpecialJustificationTag = 281,

  /*
   * (Type: TextBreakLocatorRef) (Default value: NULL) See
   * UnicodeUtilities.h for more information about creating a
   * TextBreakLocator. May be set as a style attribute.
   */
  kATSUStyleTextLocatorTag      = 282,

  /*
   * (Type: ATSStyleRenderingOptions) (Default value:
   * kATSStyleNoOptions) See ATSLayoutTypes.h for a definition of
   * ATSStyleRenderingOptions and a list of possible values. May be set
   * as a style attribute.
   */
  kATSUStyleRenderingOptionsTag = 283,

  /*
   * (Type: ATSUTextMeasurement) (Default value: determined by font)
   * Must not be less than zero. Starting in Mac OS X 10.2, you can
   * retrieve a value for this attribute, even if you have not
   * explicitly set it. This can make calculating line height easier.
   * May be set as a style attribute.
   */
  kATSUAscentTag                = 284,

  /*
   * (Type: ATSUTextMeasurement) (Default value: determined by font)
   * Must not be less than zero. Starting in Mac OS X 10.2, you can
   * retrieve a value for this attribute, even if you have not
   * explicitly set it. This can make calculating line height easier.
   * May be set as a style attribute.
   */
  kATSUDescentTag               = 285,

  /*
   * (Type: ATSUTextMeasurement) (Default value: determined by font)
   * Must not be less than zero. Starting in Mac OS X 10.2, you can
   * retrieve a value for this attribute, even if you have not
   * explicitly set it. This can make calculating line height easier.
   * May be set as a style attribute.
   */
  kATSULeadingTag               = 286,

  /*
   * (Type: ATSUGlyphSelector) (Default value: 0) See the definition of
   * ATSUGlyphSelector for more information and a list of possible
   * values. May be set as a style attribute.
   */
  kATSUGlyphSelectorTag         = 287,

  /*
   * (Type: ATSURGBAlphaColor) (Default value: (0, 0, 0, 1)) See the
   * definition of ATSURGBAlphaColor for more information. May be set
   * as a style attribute.
   */
  kATSURGBAlphaColorTag         = 288,

  /*
   * (Type: CGAffineTransform) (Default value:
   * CGAffineTransformIdentity) See the definition of CGAffineTransform
   * in CGAffineTransform.h for more information. May be set as a style
   * attribute.
   */
  kATSUFontMatrixTag            = 289,

  /*
   * (Type: ATSUStyleLineCountType) (Default value:
   * kATSUStyleSingleLineCount) Used to specify the number of strokes
   * to be drawn for an underline. May be set as a style attribute.
   */
  kATSUStyleUnderlineCountOptionTag = 290,

  /*
   * (Type: CGColorRef) (Default value: NULL) Used to specify the color
   * of the strokes to draw for an underlined run of text. If NULL, the
   * text color is used. May be set as a style attribute.
   */
  kATSUStyleUnderlineColorOptionTag = 291,

  /*
   * (Type: Boolean) (Default value: false) Used to specify
   * strikethrough style. May be set as a style attribute.
   */
  kATSUStyleStrikeThroughTag    = 292,

  /*
   * (Type: ATSUStyleLineCountType) (Default value:
   * kATSUStyleSingleLineCount) Used to specify the number of strokes
   * to be drawn for a strikethrough. May be set as a style attribute.
   */
  kATSUStyleStrikeThroughCountOptionTag = 293,

  /*
   * (Type: CGColorRef) (Default value: NULL) Used to specify the color
   * of the strokes to draw for a strikethrough style. If NULL, the
   * text color is used. May be set as a style attribute.
   */
  kATSUStyleStrikeThroughColorOptionTag = 294,

  /*
   * (Type: Boolean) (Default value: false) Used to specify if text
   * should be drawn with a drop shadow. Only takes effect if a
   * CGContext is used for drawing. May be set as a style attribute.
   */
  kATSUStyleDropShadowTag       = 295,

  /*
   * (Type: float) (Default value: 0.0) Used to specify the amount of
   * blur for a dropshadow. May be set as a style attribute.
   */
  kATSUStyleDropShadowBlurOptionTag = 296,

  /*
   * (Type: CGSize) (Default value: (3.0, -3.0)) Used to specify the
   * amount of offset from the text to be used when drawing a
   * dropshadow. May be set as a style attribute.
   */
  kATSUStyleDropShadowOffsetOptionTag = 297,

  /*
   * (Type: CGColorRef) (Default value: NULL) Used to specify the color
   * of the dropshadow. May be set as a style attribute.
   */
  kATSUStyleDropShadowColorOptionTag = 298,

  /*
   * This is just for convenience. It is the upper limit of the style
   * tags.
   */
  kATSUMaxStyleTag              = 299,

  /*
   * This tag is obsolete. Please use kATSULangRegionTag instead. This
   * is the maximum Apple ATSUI reserved tag value.  Client defined
   * tags must be larger.
   */
  kATSULanguageTag              = 264,
  kATSUMaxATSUITagValue         = 65535
};


/*
 *  ATSUAttributeValuePtr
 *  
 *  Summary:
 *    Used to provide generic access for storage of attribute values,
 *    which vary in size.
 */
typedef void *                          ATSUAttributeValuePtr;
typedef const void *                    ConstATSUAttributeValuePtr;

/*
 *  ATSUAttributeInfo
 *  
 *  Discussion:
 *    ATSUAttributeInfo is used to provide a tag/size pairing. This
 *    makes it possible to provide the client information about all the
 *    attributes for a given range of text.  This structure is only
 *    used to return to the client information about a complete set of
 *    attributes.  An array of ATSUAttributeInfos is passed as a
 *    parameter so that the client can find out what attributes are set
 *    and what their individual sizes are; with that information, they
 *    can then query about the values of the attributes they're
 *    interested in. Because arrays of ATSUAttributeInfos are used as
 *    parameters to functions, they have to be of a fixed size, hence
 *    the value is not included in the structure.
 */
struct ATSUAttributeInfo {
  ATSUAttributeTag    fTag;
  ByteCount           fValueSize;
};
typedef struct ATSUAttributeInfo        ATSUAttributeInfo;

/*
 *  ATSUCaret
 *  
 *  Discussion:
 *    Contains the complete information needed to render a caret.  fX
 *    and fY is the position of one of the caret's ends relative to the
 *    origin position of the line the caret belongs. fDeltaX and
 *    fDeltaY is the position of the caret's other end. Hence, to draw
 *    a caret, simply call MoveTo(fX, fY) followed by LineTo(fDeltaX,
 *    fDeltaY) or equivalent.  The ATSUCaret will contain the positions
 *    needed to draw carets on angled lines and reflect angled carets
 *    and leading/trailing split caret appearances.
 */
struct ATSUCaret {
  Fixed               fX;
  Fixed               fY;
  Fixed               fDeltaX;
  Fixed               fDeltaY;
};
typedef struct ATSUCaret                ATSUCaret;

/*
 *  ATSUCursorMovementType
 *  
 *  Discussion:
 *    Used to indicate how much to move the cursor when using the ATSUI
 *    cusor movement routines. Note that kATSUByCharacterCluster is
 *    only available in Mac OS X and in CarbonLib versions 1.3 and
 *    later.
 */
typedef UInt16 ATSUCursorMovementType;
enum {

  /*
   * Cursor movement based on individual characters. The cursor will
   * step through individual characters within ligatures.
   */
  kATSUByCharacter              = 0,

  /*
   * Like kATSUByCharacter, but the cursor will treat ligatures as
   * single entities.
   */
  kATSUByTypographicCluster     = 1,

  /*
   * Cursor movement by whole words.
   */
  kATSUByWord                   = 2,

  /*
   * Cursor movement by clusters based on characters only.
   */
  kATSUByCharacterCluster       = 3,

  /*
   * Obsolete name for kATSUByTypographicCluster; do not use.
   */
  kATSUByCluster                = 1
};


/*
 *  ATSULineTruncation
 *  
 *  Summary:
 *    Constants used with the kATSULineTruncationTag layout and line
 *    control.
 *  
 *  Discussion:
 *    The constants kATSUTruncateNone, kATSUTruncateStart,
 *    kATSUTruncateEnd, and kATSUTruncateMiddle represent different
 *    places in the text where glyphs should be replaced with an
 *    elipsis should the text not fit within the width set by the
 *    kATSULineWidthTag line and layout control. The constant
 *    kATSUTruncFeatNoSquishing is special and can be bitwise OR'd with
 *    any of the other constants. It indicates that ATSUI should not
 *    perform negative justification to make the text fit. This can be
 *    desirable for situations such as live resize, to prevent the text
 *    from "wiggling".
 */
typedef UInt32 ATSULineTruncation;
enum {
  kATSUTruncateNone             = 0,
  kATSUTruncateStart            = 1,
  kATSUTruncateEnd              = 2,
  kATSUTruncateMiddle           = 3,
  kATSUTruncateSpecificationMask = 0x00000007,
  kATSUTruncFeatNoSquishing     = 0x00000008
};


/*
 *  ATSUStyleLineCountType
 *  
 *  Discussion:
 *    ATSUStyleLineCountType is used to designate how many lines will
 *    be drawn for a given style type.  Currently only the underline
 *    and strikethrough styles support this type.
 */
typedef UInt16 ATSUStyleLineCountType;
enum {
  kATSUStyleSingleLineCount     = 1,
  kATSUStyleDoubleLineCount     = 2
};


/*
 *  ATSUVerticalCharacterType
 *  
 *  Discussion:
 *    Use these constants along with the kATSUVerticalCharacterTag
 *    layout control to determine whether the vertical or horizontal
 *    forms of glyphs should be used. Note that this is independent of
 *    line rotation.
 */
typedef UInt16 ATSUVerticalCharacterType;
enum {
  kATSUStronglyHorizontal       = 0,
  kATSUStronglyVertical         = 1
};



/*
 *  ATSUStyleComparison
 *  
 *  Discussion:
 *    ATSUStyleComparison is an enumeration with four values, and is
 *    used by ATSUCompareStyles() to indicate if the first style
 *    parameter contains as a proper subset, is equal to, or is
 *    contained by the second style parameter.
 */
typedef UInt16 ATSUStyleComparison;
enum {
  kATSUStyleUnequal             = 0,
  kATSUStyleContains            = 1,
  kATSUStyleEquals              = 2,
  kATSUStyleContainedBy         = 3
};


/*
 *  ATSUFontFallbackMethod
 *  
 *  Discussion:
 *    ATSUFontFallbackMethod type defines the method by which ATSUI
 *    will try to find an appropriate font for a character if the
 *    assigned font does not contain the needed glyph(s) to represent
 *    it.  This affects ATSUMatchFontsToText and font selection during
 *    layout and drawing when ATSUSetTransientFontMatching is set ON.
 */
typedef UInt16 ATSUFontFallbackMethod;
enum {

  /*
   * When this constant is specified, all fonts on the system are
   * searched for substitute glyphs.
   */
  kATSUDefaultFontFallbacks     = 0,

  /*
   * This constant specifies that only the special last resort font be
   * used for substitute glyphs.
   */
  kATSULastResortOnlyFallback   = 1,

  /*
   * This constant specifies that a font list you provide should be
   * sequentially searched for substitute glyphs before the ATSUI
   * searches through all available fonts on the system. You specify
   * this list through the iFonts parameter to the
   * ATSUSetObjFontFallbacks function.
   */
  kATSUSequentialFallbacksPreferred = 2,

  /*
   * This constants specifies that only the font list you provide
   * should be sequentially searched for substitute glyphs. All other
   * fonts on the system are ignored, except for the special last
   * resort font. You specify the list of fonts you want ATSUI to use
   * by passing it to the iFonts parameter of the
   * ATSUSetObjFontFallbacks function.
   */
  kATSUSequentialFallbacksExclusive = 3
};


/*
 *  ATSUTabType
 *  
 *  Discussion:
 *    ATSUTabType type defines the characteristic of ATSUI tabs. A Left
 *    tab type specifies that the left side of affected text is to be
 *    maintained flush against the tab stop.  A Right tab type
 *    specifies that the right side of affected text is to be
 *    maintained flush against the tab stop.  A Center tab type
 *    specifies that the affected text centered about the tab stop. A
 *    Decimal tab type specifies that the affected text will be aligned
 *    such that the decimal character will by flush against the tab
 *    stop.  The default decimal character is defined in System
 *    Preferences.  A different decimal character can be defined by
 *    using the ATSUAttributeTag kATSULineDecimalTabCharacterTag.
 */

typedef UInt16                          ATSUTabType;
enum {
  kATSULeftTab                  = 0,
  kATSUCenterTab                = 1,
  kATSURightTab                 = 2,
  kATSUDecimalTab               = 3,
  kATSUNumberTabTypes           = 4
};


/*
 *  ATSUTab
 *  
 *  Discussion:
 *    ATSUTab specifies the position and type of tab stop to be applied
 *    to a ATSUTextLayout set through the ATSUI routine ATSUSetTabArray
 *    and returned through ATSUGetTabArray.
 */
struct ATSUTab {
  ATSUTextMeasurement  tabPosition;
  ATSUTabType         tabType;
};
typedef struct ATSUTab                  ATSUTab;

/*
 *  ATSURGBAlphaColor
 *  
 *  Discussion:
 *    Use this structure with the kATSURGBAlphaColorTag attribute to
 *    specify color for your text in an ATSUStyle. All values range
 *    from 0.0 to 1.0.
 */
struct ATSURGBAlphaColor {
  float               red;
  float               green;
  float               blue;
  float               alpha;
};
typedef struct ATSURGBAlphaColor        ATSURGBAlphaColor;

/*
 *  GlyphCollection
 *  
 *  Discussion:
 *    GlyphCollection types represent the specific character
 *    collection.  If the value is zero, kGlyphCollectionGID, then this
 *    indicates that the glyph value represents the actual glyphID of a
 *    specific font. Adobe collections are based on CID, rather than
 *    glyph ID.
 */

typedef UInt16                          GlyphCollection;
enum {
  kGlyphCollectionGID           = 0,
  kGlyphCollectionAdobeCNS1     = 1,
  kGlyphCollectionAdobeGB1      = 2,
  kGlyphCollectionAdobeJapan1   = 3,
  kGlyphCollectionAdobeJapan2   = 4,
  kGlyphCollectionAdobeKorea1   = 5,
  kGlyphCollectionUnspecified   = 0xFF
};



/*
 *  ATSUGlyphSelector
 *  
 *  Discussion:
 *    ATSUGlyphSelector can direct ATSUI to use a specific glyph
 *    instead of the one that ATSUI normally derives.  The glyph can be
 *    specified either as a glyphID (specific to the font used) or CID
 *    from a specfic collection defined by the collection entry.
 */
struct ATSUGlyphSelector {

  /*
   * A glyph collection constant. See the definition of GlyphCollection
   * for possible values for this field.
   */
  GlyphCollection     collection;

  /*
   * The glyph ID of the glyph (when collection is
   * kGlyphCollectionGID). For Adobe glyph collections, this value
   * represents a CID
   */
  GlyphID             glyphID;
};
typedef struct ATSUGlyphSelector        ATSUGlyphSelector;

/*
 *  ATSUGlyphInfo
 *  
 *  Summary:
 *    Structure returned by ATSUGetGlyphInfo
 *  
 *  Discussion:
 *    ATSUGetGlyphInfo will return an array of these structs, one for
 *    each glyph in the specified range. You can then make changes to
 *    the data before drawing it with ATSUDrawGlyphInfo. These
 *    functions are no longer recommended; see ATSUnicodeDirectAccess.h
 *    for replacement functions.
 */
struct ATSUGlyphInfo {
  GlyphID             glyphID;
  UInt16              reserved;
  UInt32              layoutFlags;
  UniCharArrayOffset  charIndex;
  ATSUStyle           style;
  Float32             deltaY;
  Float32             idealX;
  SInt16              screenX;
  SInt16              caretX;
};
typedef struct ATSUGlyphInfo            ATSUGlyphInfo;

/*
 *  ATSUGlyphInfoArray
 *  
 *  Summary:
 *    Structure returned by ATSUGetGlyphInfo
 *  
 *  Discussion:
 *    This data structure is returned by ATSUGetGlyphInfo. layout is
 *    the same layout you pass in to ATSUGetGlyphInfo, numGlyphs is the
 *    number of glyphs stored in the array glyphs. See the definition
 *    of ATSUGlyphInfo for more information about the data structures
 *    contained in the glyphs array. The ATSUGetGlyphInfo function is
 *    no longer recommended; see ATSUnicodeDirectAccess.h for
 *    replacement functions.
 */
struct ATSUGlyphInfoArray {
  ATSUTextLayout      layout;
  ItemCount           numGlyphs;
  ATSUGlyphInfo       glyphs[1];
};
typedef struct ATSUGlyphInfoArray       ATSUGlyphInfoArray;
/*********************************************************************************/
/*  ATSUI highlighting method constants and typedefs                             */
/*********************************************************************************/

/*
 *  ATSUHighlightMethod
 *  
 *  Discussion:
 *    Use the constants with the function ATSUSetHighlightingMethod to
 *    determine the method of highlighting to use. kInvertHighlighting
 *    will cause ATSUI to perform a simple color inversion on the area
 *    around the text. Although this method requires the least work, it
 *    does not produce the best visual results. kRedrawHighlighting
 *    will cause ATSUI to redraw the text whenever highlighting or
 *    unhighlighting it. This method produces the best visual results,
 *    but it does require you to specify a way for ATSUI to redraw the
 *    background behind the text after an unhighlight operation. See
 *    the definitions of ATSUUnhighlightData, ATSUBackgroundData,
 *    ATSUBackgroundDataType, and RedrawBackgroundProcPtr for more
 *    information.
 */
typedef UInt32 ATSUHighlightMethod;
enum {
  kInvertHighlighting           = 0,
  kRedrawHighlighting           = 1
};


/*
 *  ATSUBackgroundDataType
 *  
 *  Discussion:
 *    Use these constants for the dataType field in the
 *    ATSUUnhighlightData structure. kATSUBackgroundColor refers to a
 *    solid color background, while kATSUBackgroundCallback refers to a
 *    redrawing callback function. Note that if you specify
 *    kATSUBackgroundCallback, you must provide a callback function.
 */
typedef UInt32 ATSUBackgroundDataType;
enum {
  kATSUBackgroundColor          = 0,
  kATSUBackgroundCallback       = 1
};


/*
 *  ATSUBackgroundColor
 *  
 *  Discussion:
 *    A background color used by ATSUI to redraw the background after a
 *    call to ATSUUnhighlight text when the highlighting method is set
 *    to kRedrawHighlighting. See the definitions of ATSUBackgroundData
 *    and ATSUUnhighlightData for more information.
 */

typedef ATSURGBAlphaColor               ATSUBackgroundColor;

/*
 *  RedrawBackgroundProcPtr
 *  
 *  Discussion:
 *    RedrawBackgroundProcPtr is a pointer to a client-supplied
 *    callback function (e.g. MyRedrawBackgroundProc) for redrawing
 *    complex backgrounds (and optionally the text as well) that can be
 *    called by ATSUI for highlighting if the client has called
 *    ATSUSetHighlightingMethod with kRedrawHighlighting for the
 *    iMethod parameter. In order for ATSUI to call the client
 *    function, the client must (1) pass a pointer to the client
 *    function to NewRedrawBackgroundUPP() in order to obtain a
 *    RedrawBackgroundUPP, and (2) pass the RedrawBackgroundUPP in the
 *    unhighlightData.backgroundUPP field of the iUnhighlightData
 *    parameter for the ATSUSetHighlightingMethod call. When finished,
 *    the client should call DisposeRedrawBackgroundUPP with the
 *    RedrawBackgroundUPP.
 *  
 *  Parameters:
 *    
 *    iLayout:
 *      The layout to which the highlighting is being applied. The
 *      client function can use this to redraw the text.
 *    
 *    iTextOffset:
 *      The offset of the text that is being highlighted; can be used
 *      by the client function to redraaw the text.
 *    
 *    iTextLength:
 *      The length of the text that is being highlighted; can be used
 *      by the client function to redraaw the text.
 *    
 *    iUnhighlightArea:
 *      An array of ATSTrapezoids that describes the highlight area.
 *      The ATSTrapezoid array is ALWAYS in QD coordinates.
 *    
 *    iTrapezoidCount:
 *      The count of ATSTrapezoids in iUnhighlightArea.
 *  
 *  Result:
 *    A Boolean result indicating whether ATSUI should redraw the text.
 *    If the client function redraws the text, it should return false,
 *    otherwise it should return true to have ATSUI redraw any text
 *    that needs to be redrawn.
 */
typedef CALLBACK_API( Boolean , RedrawBackgroundProcPtr )(ATSUTextLayout iLayout, UniCharArrayOffset iTextOffset, UniCharCount iTextLength, ATSTrapezoid iUnhighlightArea[], ItemCount iTrapezoidCount);
typedef STACK_UPP_TYPE(RedrawBackgroundProcPtr)                 RedrawBackgroundUPP;

/*
 *  ATSUBackgroundData
 *  
 *  Summary:
 *    Data describing one of two methods for ATSUI to unhighlight text.
 *  
 *  Discussion:
 *    When you call ATSUUnhighlightText and the highlighting method
 *    used is kRedrawHighlighting, ATSUI must redraw the text,
 *    including the background, when unhighlighting. The two ways of
 *    doing this are by telling ATSUI to (1) use a solid color to
 *    repaint the background (2) use a callback function to repaint the
 *    background. This union provides that information to ATSUI. See
 *    the definitions of ATSUUnhighlightData, RedrawBackgroundProcPtr,
 *    and ATSUSetHighlightingMethod for more information.
 */
union ATSUBackgroundData {

  /*
   * Specifies a color for ATSUI to repaint the background.
   */
  ATSUBackgroundColor  backgroundColor;

  /*
   * Specifies a Universal Procedure Pointer for ATSUI to call to
   * redraw the background.
   */
  RedrawBackgroundUPP  backgroundUPP;
};
typedef union ATSUBackgroundData        ATSUBackgroundData;

/*
 *  ATSUUnhighlightData
 *  
 *  Summary:
 *    Struct for defining a method for ATSUI to unhighlight text.
 *  
 *  Discussion:
 *    There are two methods of highlighting available on Mac OS X:
 *    invert and redraw. For the invert method, no unhighlight method
 *    needs to be specified. ATSUI will simply higlight text by
 *    performing a color inversion on the area surrounding the test.
 *    However, for best results, the redraw method is perferred. With
 *    this method, ATSUI will redraw text with a new background when
 *    highlighting, and redraw it again when unhighlighting. When using
 *    the redraw method, ATSUI needs to know how to restore the
 *    backround when unhighlighting text. That is where the unhighlight
 *    data comes in. This struct tells ATSUI how to restore the
 *    background after a highlight. There are two methods for
 *    specifying this information to ATSUI. One is by specifying a
 *    solid color, the other by providing a callback for redrawing part
 *    of the background.
 */
struct ATSUUnhighlightData {

  /*
   * Determines which method to use for restoring the background after
   * a highlight; solid color (kATSUBackgroundColor), or drawing
   * callback (kATSUBackgroundCallback). See also the definition of
   * ATSUBackgroundDataType.
   */
  ATSUBackgroundDataType  dataType;

  /*
   * This union provides the actual data for ATSUI to use when
   * redrawing the background. See the definition of ATSUBackgroundData
   * for more information.
   */
  ATSUBackgroundData  unhighlightData;
};
typedef struct ATSUUnhighlightData      ATSUUnhighlightData;
/********************************************************************************/
/* Other ATSUI constants                                                        */
/********************************************************************************/

/*
 *  Summary:
 *    Line direction types
 *  
 *  Discussion:
 *    These constants are used with the kATSULineDirectionTag control
 *    to determine overall line direction.
 */
enum {

  /*
   * Imposes left-to-right or top-to-bottom dominant direction.
   */
  kATSULeftToRightBaseDirection = 0,

  /*
   * Impose right-to-left or bottom-to-top dominant direction.
   */
  kATSURightToLeftBaseDirection = 1
};

#define kATSUStartAlignment         ((Fract) 0x00000000L)
#define kATSUEndAlignment           ((Fract) 0x40000000L)
#define kATSUCenterAlignment        ((Fract) 0x20000000L)
#define kATSUNoJustification        ((Fract) 0x00000000L)
#define kATSUFullJustification      ((Fract) 0x40000000L)

/*
 *  Summary:
 *    This constant will be returned from ATSUFindFontFromName if no
 *    valid font can be found. If you pass this constant to
 *    ATSUSetAttributes, it will produce an error.
 */
enum {
  kATSUInvalidFontID            = 0
};


/*
 *  Summary:
 *    Pass this constant to line breaking functions (i.e.,
 *    ATSUBreakLine, ATSUBatchBreakLines) if you have already set a
 *    line width as a layout control via the kATSULineWidthTag
 *    attribute.
 */
enum {
  kATSUUseLineControlWidth      = 0x7FFFFFFF
};


/*
 *  Summary:
 *    Pass this constant to the iSelector parameter of the
 *    ATSUGetFontFeatureNameCode function if you wish to obtain the
 *    name code for a feature type rather than a feature selector.
 */
enum {
  kATSUNoSelector               = 0x0000FFFF
};


/*
 *  Summary:
 *    Text buffer convenience constants.
 *  
 *  Discussion:
 *    These constants refer to the beginning and end of a text buffer.
 *    Functions which accept these constants are marked below. Do not
 *    pass these constants to functions which do not explicity state
 *    they will accept them.
 */
enum {

  /*
   * Refers to the beginning of a text buffer.
   */
  kATSUFromTextBeginning        = (UInt32)0xFFFFFFFF,

  /*
   * Refers to the end of a text buffer.
   */
  kATSUToTextEnd                = (UInt32)0xFFFFFFFF,

  /*
   * Used for bidi cursor movement between paragraphs.
   */
  kATSUFromPreviousLayout       = (UInt32)0xFFFFFFFE,

  /*
   * Used for bidi cursor movement between paragraphs.
   */
  kATSUFromFollowingLayout      = (UInt32)0xFFFFFFFD
};


/*
 *  Summary:
 *    Other convenience constants.
 */
enum {

  /*
   * Pass this constant to functions that require a set of coordinates
   * (i.e., ATSUDrawText, ATSUHighlightText) if you want ATSUI to use
   * the current Quickdraw graphics port pen location.
   */
  kATSUUseGrafPortPenLoc        = (UInt32)0xFFFFFFFF,

  /*
   * Pass this constant to functions such as ATSUClearAttributes and
   * ATSUClearLayoutControls if you wish to clear all settings instead
   * of a specific array of settings.
   */
  kATSUClearAll                 = (UInt32)0xFFFFFFFF
};


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __ATSUNICODETYPES__ */

