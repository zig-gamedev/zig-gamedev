/*
 *  SFNTLayoutTypes.h
 *  CoreText
 *
 *  Copyright 1994-2021 Apple Inc. All rights reserved.
 *
 */

#ifndef __SFNTLAYOUTTYPES__
#define __SFNTLAYOUTTYPES__

#include <TargetConditionals.h>
#include <Availability.h>

#if !TARGET_OS_WIN32
#include <MacTypes.h>
#elif !defined(__MACTYPES__)
typedef SInt32 Fixed;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/* ----------------------------------------------------------------------------------------- */
/* CONSTANTS */
/*
    The following values can be used to set run feature values. Note that unless the
    feature is defaulted differently in different fonts, the zero value for the
    selectors represents the default value. Consult the following URL for further info:
    <https://developer.apple.com/fonts/TrueType-Reference-Manual/RM09/AppendixF.html>
*/


/*
 *  Summary:
 *    Feature types
 */
enum {
  kAllTypographicFeaturesType   = 0,
  kLigaturesType                = 1,
  kCursiveConnectionType        = 2,
  kLetterCaseType               = 3,    /* deprecated - use kLowerCaseType or kUpperCaseType instead */
  kVerticalSubstitutionType     = 4,
  kLinguisticRearrangementType  = 5,
  kNumberSpacingType            = 6,
  kSmartSwashType               = 8,
  kDiacriticsType               = 9,
  kVerticalPositionType         = 10,
  kFractionsType                = 11,
  kOverlappingCharactersType    = 13,
  kTypographicExtrasType        = 14,
  kMathematicalExtrasType       = 15,
  kOrnamentSetsType             = 16,
  kCharacterAlternativesType    = 17,
  kDesignComplexityType         = 18,
  kStyleOptionsType             = 19,
  kCharacterShapeType           = 20,
  kNumberCaseType               = 21,
  kTextSpacingType              = 22,
  kTransliterationType          = 23,
  kAnnotationType               = 24,
  kKanaSpacingType              = 25,
  kIdeographicSpacingType       = 26,
  kUnicodeDecompositionType     = 27,
  kRubyKanaType                 = 28,
  kCJKSymbolAlternativesType    = 29,
  kIdeographicAlternativesType  = 30,
  kCJKVerticalRomanPlacementType = 31,
  kItalicCJKRomanType           = 32,
  kCaseSensitiveLayoutType      = 33,
  kAlternateKanaType            = 34,
  kStylisticAlternativesType    = 35,
  kContextualAlternatesType     = 36,
  kLowerCaseType                = 37,
  kUpperCaseType                = 38,
  kLanguageTagType              = 39,
  kCJKRomanSpacingType          = 103,
  kLastFeatureType              = -1
};


/*
 *  Summary:
 *    Selectors for feature type kAllTypographicFeaturesType
 */
enum {
  kAllTypeFeaturesOnSelector    = 0,
  kAllTypeFeaturesOffSelector   = 1
};



/*
 *  Summary:
 *    Selectors for feature type kLigaturesType
 */
enum {
  kRequiredLigaturesOnSelector  = 0,
  kRequiredLigaturesOffSelector = 1,
  kCommonLigaturesOnSelector    = 2,
  kCommonLigaturesOffSelector   = 3,
  kRareLigaturesOnSelector      = 4,
  kRareLigaturesOffSelector     = 5,
  kLogosOnSelector              = 6,
  kLogosOffSelector             = 7,
  kRebusPicturesOnSelector      = 8,
  kRebusPicturesOffSelector     = 9,
  kDiphthongLigaturesOnSelector = 10,
  kDiphthongLigaturesOffSelector = 11,
  kSquaredLigaturesOnSelector   = 12,
  kSquaredLigaturesOffSelector  = 13,
  kAbbrevSquaredLigaturesOnSelector = 14,
  kAbbrevSquaredLigaturesOffSelector = 15,
  kSymbolLigaturesOnSelector    = 16,
  kSymbolLigaturesOffSelector   = 17,
  kContextualLigaturesOnSelector = 18,
  kContextualLigaturesOffSelector = 19,
  kHistoricalLigaturesOnSelector = 20,
  kHistoricalLigaturesOffSelector = 21
};


/*
 *  Summary:
 *    Selectors for feature type kCursiveConnectionType
 */
enum {
  kUnconnectedSelector          = 0,
  kPartiallyConnectedSelector   = 1,
  kCursiveSelector              = 2
};


/*
 *  Summary:
 *    Selectors for feature type kLetterCaseType
 */
enum {
  kUpperAndLowerCaseSelector    = 0,    /* deprecated */
  kAllCapsSelector              = 1,    /* deprecated */
  kAllLowerCaseSelector         = 2,    /* deprecated */
  kSmallCapsSelector            = 3,    /* deprecated */
  kInitialCapsSelector          = 4,    /* deprecated */
  kInitialCapsAndSmallCapsSelector = 5  /* deprecated */
};


/*
 *  Summary:
 *    Selectors for feature type kVerticalSubstitutionType
 */
enum {
  kSubstituteVerticalFormsOnSelector = 0,
  kSubstituteVerticalFormsOffSelector = 1
};


/*
 *  Summary:
 *    Selectors for feature type kLinguisticRearrangementType
 */
enum {
  kLinguisticRearrangementOnSelector = 0,
  kLinguisticRearrangementOffSelector = 1
};


/*
 *  Summary:
 *    Selectors for feature type kNumberSpacingType
 */
enum {
  kMonospacedNumbersSelector    = 0,
  kProportionalNumbersSelector  = 1,
  kThirdWidthNumbersSelector    = 2,
  kQuarterWidthNumbersSelector  = 3
};


/*
 *  Summary:
 *    Selectors for feature type kSmartSwashType
 */
enum {
  kWordInitialSwashesOnSelector = 0,
  kWordInitialSwashesOffSelector = 1,
  kWordFinalSwashesOnSelector   = 2,
  kWordFinalSwashesOffSelector  = 3,
  kLineInitialSwashesOnSelector = 4,
  kLineInitialSwashesOffSelector = 5,
  kLineFinalSwashesOnSelector   = 6,
  kLineFinalSwashesOffSelector  = 7,
  kNonFinalSwashesOnSelector    = 8,
  kNonFinalSwashesOffSelector   = 9
};


/*
 *  Summary:
 *    Selectors for feature type kDiacriticsType
 */
enum {
  kShowDiacriticsSelector       = 0,
  kHideDiacriticsSelector       = 1,
  kDecomposeDiacriticsSelector  = 2
};


/*
 *  Summary:
 *    Selectors for feature type kVerticalPositionType
 */
enum {
  kNormalPositionSelector       = 0,
  kSuperiorsSelector            = 1,
  kInferiorsSelector            = 2,
  kOrdinalsSelector             = 3,
  kScientificInferiorsSelector  = 4
};


/*
 *  Summary:
 *    Selectors for feature type kFractionsType
 */
enum {
  kNoFractionsSelector          = 0,
  kVerticalFractionsSelector    = 1,
  kDiagonalFractionsSelector    = 2
};


/*
 *  Summary:
 *    Selectors for feature type kOverlappingCharactersType
 */
enum {
  kPreventOverlapOnSelector     = 0,
  kPreventOverlapOffSelector    = 1
};


/*
 *  Summary:
 *    Selectors for feature type kTypographicExtrasType
 */
enum {
  kHyphensToEmDashOnSelector    = 0,
  kHyphensToEmDashOffSelector   = 1,
  kHyphenToEnDashOnSelector     = 2,
  kHyphenToEnDashOffSelector    = 3,
  kSlashedZeroOnSelector        = 4,
  kSlashedZeroOffSelector       = 5,
  kFormInterrobangOnSelector    = 6,
  kFormInterrobangOffSelector   = 7,
  kSmartQuotesOnSelector        = 8,
  kSmartQuotesOffSelector       = 9,
  kPeriodsToEllipsisOnSelector  = 10,
  kPeriodsToEllipsisOffSelector = 11
};


/*
 *  Summary:
 *    Selectors for feature type kMathematicalExtrasType
 */
enum {
  kHyphenToMinusOnSelector      = 0,
  kHyphenToMinusOffSelector     = 1,
  kAsteriskToMultiplyOnSelector = 2,
  kAsteriskToMultiplyOffSelector = 3,
  kSlashToDivideOnSelector      = 4,
  kSlashToDivideOffSelector     = 5,
  kInequalityLigaturesOnSelector = 6,
  kInequalityLigaturesOffSelector = 7,
  kExponentsOnSelector          = 8,
  kExponentsOffSelector         = 9,
  kMathematicalGreekOnSelector  = 10,
  kMathematicalGreekOffSelector = 11
};


/*
 *  Summary:
 *    Selectors for feature type kOrnamentSetsType
 */
enum {
  kNoOrnamentsSelector          = 0,
  kDingbatsSelector             = 1,
  kPiCharactersSelector         = 2,
  kFleuronsSelector             = 3,
  kDecorativeBordersSelector    = 4,
  kInternationalSymbolsSelector = 5,
  kMathSymbolsSelector          = 6
};


/*
 *  Summary:
 *    Selectors for feature type kCharacterAlternativesType
 */
enum {
  kNoAlternatesSelector         = 0
};


/*
 *  Summary:
 *    Selectors for feature type kDesignComplexityType
 */
enum {
  kDesignLevel1Selector         = 0,
  kDesignLevel2Selector         = 1,
  kDesignLevel3Selector         = 2,
  kDesignLevel4Selector         = 3,
  kDesignLevel5Selector         = 4
};


/*
 *  Summary:
 *    Selectors for feature type kStyleOptionsType
 */
enum {
  kNoStyleOptionsSelector       = 0,
  kDisplayTextSelector          = 1,
  kEngravedTextSelector         = 2,
  kIlluminatedCapsSelector      = 3,
  kTitlingCapsSelector          = 4,
  kTallCapsSelector             = 5
};


/*
 *  Summary:
 *    Selectors for feature type kCharacterShapeType
 */
enum {
  kTraditionalCharactersSelector = 0,
  kSimplifiedCharactersSelector = 1,
  kJIS1978CharactersSelector    = 2,
  kJIS1983CharactersSelector    = 3,
  kJIS1990CharactersSelector    = 4,
  kTraditionalAltOneSelector    = 5,
  kTraditionalAltTwoSelector    = 6,
  kTraditionalAltThreeSelector  = 7,
  kTraditionalAltFourSelector   = 8,
  kTraditionalAltFiveSelector   = 9,
  kExpertCharactersSelector     = 10,
  kJIS2004CharactersSelector    = 11,
  kHojoCharactersSelector       = 12,
  kNLCCharactersSelector        = 13,
  kTraditionalNamesCharactersSelector = 14
};


/*
 *  Summary:
 *    Selectors for feature type kNumberCaseType
 */
enum {
  kLowerCaseNumbersSelector     = 0,
  kUpperCaseNumbersSelector     = 1
};


/*
 *  Summary:
 *    Selectors for feature type kTextSpacingType
 */
enum {
  kProportionalTextSelector     = 0,
  kMonospacedTextSelector       = 1,
  kHalfWidthTextSelector        = 2,
  kThirdWidthTextSelector       = 3,
  kQuarterWidthTextSelector     = 4,
  kAltProportionalTextSelector  = 5,
  kAltHalfWidthTextSelector     = 6
};


/*
 *  Summary:
 *    Selectors for feature type kTransliterationType
 */
enum {
  kNoTransliterationSelector    = 0,
  kHanjaToHangulSelector        = 1,
  kHiraganaToKatakanaSelector   = 2,
  kKatakanaToHiraganaSelector   = 3,
  kKanaToRomanizationSelector   = 4,
  kRomanizationToHiraganaSelector = 5,
  kRomanizationToKatakanaSelector = 6,
  kHanjaToHangulAltOneSelector  = 7,
  kHanjaToHangulAltTwoSelector  = 8,
  kHanjaToHangulAltThreeSelector = 9
};


/*
 *  Summary:
 *    Selectors for feature type kAnnotationType
 */
enum {
  kNoAnnotationSelector         = 0,
  kBoxAnnotationSelector        = 1,
  kRoundedBoxAnnotationSelector = 2,
  kCircleAnnotationSelector     = 3,
  kInvertedCircleAnnotationSelector = 4,
  kParenthesisAnnotationSelector = 5,
  kPeriodAnnotationSelector     = 6,
  kRomanNumeralAnnotationSelector = 7,
  kDiamondAnnotationSelector    = 8,
  kInvertedBoxAnnotationSelector = 9,
  kInvertedRoundedBoxAnnotationSelector = 10
};


/*
 *  Summary:
 *    Selectors for feature type kKanaSpacingType
 */
enum {
  kFullWidthKanaSelector        = 0,
  kProportionalKanaSelector     = 1
};


/*
 *  Summary:
 *    Selectors for feature type kIdeographicSpacingType
 */
enum {
  kFullWidthIdeographsSelector  = 0,
  kProportionalIdeographsSelector = 1,
  kHalfWidthIdeographsSelector  = 2
};


/*
 *  Summary:
 *    Selectors for feature type kUnicodeDecompositionType
 */
enum {
  kCanonicalCompositionOnSelector = 0,
  kCanonicalCompositionOffSelector = 1,
  kCompatibilityCompositionOnSelector = 2,
  kCompatibilityCompositionOffSelector = 3,
  kTranscodingCompositionOnSelector = 4,
  kTranscodingCompositionOffSelector = 5
};


/*
 *  Summary:
 *    Selectors for feature type kRubyKanaType
 */
enum {
  kNoRubyKanaSelector           = 0,    /* deprecated - use kRubyKanaOffSelector instead */
  kRubyKanaSelector             = 1,    /* deprecated - use kRubyKanaOnSelector instead */
  kRubyKanaOnSelector           = 2,
  kRubyKanaOffSelector          = 3
};


/*
 *  Summary:
 *    Selectors for feature type kCJKSymbolAlternativesType
 */
enum {
  kNoCJKSymbolAlternativesSelector = 0,
  kCJKSymbolAltOneSelector      = 1,
  kCJKSymbolAltTwoSelector      = 2,
  kCJKSymbolAltThreeSelector    = 3,
  kCJKSymbolAltFourSelector     = 4,
  kCJKSymbolAltFiveSelector     = 5
};


/*
 *  Summary:
 *    Selectors for feature type kIdeographicAlternativesType
 */
enum {
  kNoIdeographicAlternativesSelector = 0,
  kIdeographicAltOneSelector    = 1,
  kIdeographicAltTwoSelector    = 2,
  kIdeographicAltThreeSelector  = 3,
  kIdeographicAltFourSelector   = 4,
  kIdeographicAltFiveSelector   = 5
};


/*
 *  Summary:
 *    Selectors for feature type kCJKVerticalRomanPlacementType
 */
enum {
  kCJKVerticalRomanCenteredSelector = 0,
  kCJKVerticalRomanHBaselineSelector = 1
};


/*
 *  Summary:
 *    Selectors for feature type kItalicCJKRomanType
 */
enum {
  kNoCJKItalicRomanSelector     = 0,    /* deprecated - use kCJKItalicRomanOffSelector instead */
  kCJKItalicRomanSelector       = 1,    /* deprecated - use kCJKItalicRomanOnSelector instead */
  kCJKItalicRomanOnSelector     = 2,
  kCJKItalicRomanOffSelector    = 3
};


/*
 *  Summary:
 *    Selectors for feature type kCaseSensitiveLayoutType
 */
enum {
  kCaseSensitiveLayoutOnSelector = 0,
  kCaseSensitiveLayoutOffSelector = 1,
  kCaseSensitiveSpacingOnSelector = 2,
  kCaseSensitiveSpacingOffSelector = 3
};


/*
 *  Summary:
 *    Selectors for feature type kAlternateKanaType
 */
enum {
  kAlternateHorizKanaOnSelector = 0,
  kAlternateHorizKanaOffSelector = 1,
  kAlternateVertKanaOnSelector  = 2,
  kAlternateVertKanaOffSelector = 3
};


/*
 *  Summary:
 *    Selectors for feature type kStylisticAlternativesType
 */
enum {
  kNoStylisticAlternatesSelector = 0,
  kStylisticAltOneOnSelector    = 2,
  kStylisticAltOneOffSelector   = 3,
  kStylisticAltTwoOnSelector    = 4,
  kStylisticAltTwoOffSelector   = 5,
  kStylisticAltThreeOnSelector  = 6,
  kStylisticAltThreeOffSelector = 7,
  kStylisticAltFourOnSelector   = 8,
  kStylisticAltFourOffSelector  = 9,
  kStylisticAltFiveOnSelector   = 10,
  kStylisticAltFiveOffSelector  = 11,
  kStylisticAltSixOnSelector    = 12,
  kStylisticAltSixOffSelector   = 13,
  kStylisticAltSevenOnSelector  = 14,
  kStylisticAltSevenOffSelector = 15,
  kStylisticAltEightOnSelector  = 16,
  kStylisticAltEightOffSelector = 17,
  kStylisticAltNineOnSelector   = 18,
  kStylisticAltNineOffSelector  = 19,
  kStylisticAltTenOnSelector    = 20,
  kStylisticAltTenOffSelector   = 21,
  kStylisticAltElevenOnSelector = 22,
  kStylisticAltElevenOffSelector = 23,
  kStylisticAltTwelveOnSelector = 24,
  kStylisticAltTwelveOffSelector = 25,
  kStylisticAltThirteenOnSelector = 26,
  kStylisticAltThirteenOffSelector = 27,
  kStylisticAltFourteenOnSelector = 28,
  kStylisticAltFourteenOffSelector = 29,
  kStylisticAltFifteenOnSelector = 30,
  kStylisticAltFifteenOffSelector = 31,
  kStylisticAltSixteenOnSelector = 32,
  kStylisticAltSixteenOffSelector = 33,
  kStylisticAltSeventeenOnSelector = 34,
  kStylisticAltSeventeenOffSelector = 35,
  kStylisticAltEighteenOnSelector = 36,
  kStylisticAltEighteenOffSelector = 37,
  kStylisticAltNineteenOnSelector = 38,
  kStylisticAltNineteenOffSelector = 39,
  kStylisticAltTwentyOnSelector = 40,
  kStylisticAltTwentyOffSelector = 41
};


/*
 *  Summary:
 *    Selectors for feature type kContextualAlternatesType
 */
enum {
  kContextualAlternatesOnSelector = 0,
  kContextualAlternatesOffSelector = 1,
  kSwashAlternatesOnSelector    = 2,
  kSwashAlternatesOffSelector   = 3,
  kContextualSwashAlternatesOnSelector = 4,
  kContextualSwashAlternatesOffSelector = 5
};


/*
 *  Summary:
 *    Selectors for feature type kLowerCaseType
 */
enum {
  kDefaultLowerCaseSelector     = 0,
  kLowerCaseSmallCapsSelector   = 1,
  kLowerCasePetiteCapsSelector  = 2
};


/*
 *  Summary:
 *    Selectors for feature type kUpperCaseType
 */
enum {
  kDefaultUpperCaseSelector     = 0,
  kUpperCaseSmallCapsSelector   = 1,
  kUpperCasePetiteCapsSelector  = 2
};


/*
 *  Summary:
 *    Selectors for feature type kCJKRomanSpacingType
 */
enum {
  kHalfWidthCJKRomanSelector    = 0,
  kProportionalCJKRomanSelector = 1,
  kDefaultCJKRomanSelector      = 2,
  kFullWidthCJKRomanSelector    = 3
};

/* --------------------------------------------------------------------------- */
/* ---------------- Table Specific Typedefs and Constants -------------------- */
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: lookup tables - used within various other tables */
enum {
  kSFNTLookupSimpleArray        = 0,    /* a simple array indexed by glyph code */
  kSFNTLookupSegmentSingle      = 2,    /* segment mapping to single value */
  kSFNTLookupSegmentArray       = 4,    /* segment mapping to lookup array */
  kSFNTLookupSingleTable        = 6,    /* sorted list of glyph, value pairs */
  kSFNTLookupTrimmedArray       = 8,    /* a simple trimmed array indexed by glyph code */
  kSFNTLookupVector             = 10    /* a simple trimmed vector indexed by glyph code */
};

typedef UInt16                          SFNTLookupTableFormat;
typedef UInt16                          SFNTLookupValue;
typedef UInt16                          SFNTLookupOffset;
typedef UInt32                          SFNTLookupKind;
/*
    A BinarySearchHeader defines the five standard fields needed to perform quick
    lookups in a lookup table (note that using UInt16s, and not ItemCounts or
    similar types, is important here, since these tables are in fonts, and the
    documented font formats specify 16-bit quantities).
*/
struct SFNTLookupBinarySearchHeader {
  UInt16              unitSize;               /* size of a unit in bytes */
  UInt16              nUnits;                 /* number of units in table */
  UInt16              searchRange;            /* (largest power of two <= nUnits) * unitSize */
  UInt16              entrySelector;          /* log2 (largest power of two <= nUnits) */
  UInt16              rangeShift;             /* (nUnits - largest power of two <= nUnits) * unitSize */
};
typedef struct SFNTLookupBinarySearchHeader SFNTLookupBinarySearchHeader;
/* A format 0 lookup table maps all glyphs in the font to lookup values */
struct SFNTLookupArrayHeader {
  SFNTLookupValue     lookupValues[1];
};
typedef struct SFNTLookupArrayHeader    SFNTLookupArrayHeader;
/* A format 8 lookup table maps some range of glyphs in the font to lookup values */
struct SFNTLookupTrimmedArrayHeader {
  UInt16              firstGlyph;
  UInt16              count;
  SFNTLookupValue     valueArray[1];
};
typedef struct SFNTLookupTrimmedArrayHeader SFNTLookupTrimmedArrayHeader;
/* A format 10 lookup table maps some range of glyphs in the font to lookup values of the specified size */
struct SFNTLookupVectorHeader {
  UInt16              valueSize;
  UInt16              firstGlyph;
  UInt16              count;
  UInt8               values[1];
};
typedef struct SFNTLookupVectorHeader SFNTLookupVectorHeader;
/*
    Format 2 and format 4 lookup tables map ranges of glyphs to either single lookup
    values (format 2), or per-glyph lookup values (format 4). Since both formats
    use the same kind of data, only one unified set of segment-related structures
    is defined.
*/
struct SFNTLookupSegment {
  UInt16              lastGlyph;
  UInt16              firstGlyph;
  UInt16              value[1];
};
typedef struct SFNTLookupSegment        SFNTLookupSegment;
struct SFNTLookupSegmentHeader {
  SFNTLookupBinarySearchHeader  binSearch;
  SFNTLookupSegment   segments[1];
};
typedef struct SFNTLookupSegmentHeader  SFNTLookupSegmentHeader;
/* A format 6 lookup table maps single glyphs to lookup values. */
struct SFNTLookupSingle {
  UInt16              glyph;
  UInt16              value[1];
};
typedef struct SFNTLookupSingle         SFNTLookupSingle;
struct SFNTLookupSingleHeader {
  SFNTLookupBinarySearchHeader  binSearch;
  SFNTLookupSingle    entries[1];
};
typedef struct SFNTLookupSingleHeader   SFNTLookupSingleHeader;
/* The format-specific part of the subtable header */
union SFNTLookupFormatSpecificHeader {
  SFNTLookupArrayHeader  theArray;
  SFNTLookupSegmentHeader  segment;
  SFNTLookupSingleHeader  single;
  SFNTLookupTrimmedArrayHeader  trimmedArray;
  SFNTLookupVectorHeader  vector;
};
typedef union SFNTLookupFormatSpecificHeader SFNTLookupFormatSpecificHeader;
/* The overall subtable header */
struct SFNTLookupTable {
  SFNTLookupTableFormat  format;              /* table format */
  SFNTLookupFormatSpecificHeader  fsHeader;   /* format specific header */
};
typedef struct SFNTLookupTable          SFNTLookupTable;
typedef SFNTLookupTable *               SFNTLookupTablePtr;
typedef SFNTLookupTablePtr *            SFNTLookupTableHandle;
/* --------------------------------------------------------------------------- */
/* GENERAL FORMATS FOR STATE TABLES -- prefix "ST" */
enum {
  kSTClassEndOfText             = 0,
  kSTClassOutOfBounds           = 1,
  kSTClassDeletedGlyph          = 2,
  kSTClassEndOfLine             = 3,
  kSTSetMark                    = 0x8000,
  kSTNoAdvance                  = 0x4000,
  kSTMarkEnd                    = 0x2000,
  kSTLigActionMask              = 0x3FFF,
  kSTRearrVerbMask              = 0x000F
};

typedef UInt8                           STClass;
typedef UInt8                           STEntryIndex;
struct STHeader {
  UInt8               filler;
  STClass             nClasses;
  UInt16              classTableOffset;
  UInt16              stateArrayOffset;
  UInt16              entryTableOffset;
};
typedef struct STHeader                 STHeader;
struct STClassTable {
  UInt16              firstGlyph;
  UInt16              nGlyphs;
  STClass             classes[1];
};
typedef struct STClassTable             STClassTable;
struct STEntryZero {
  UInt16              newState;
  UInt16              flags;
};
typedef struct STEntryZero              STEntryZero;
struct STEntryOne {
  UInt16              newState;
  UInt16              flags;
  UInt16              offset1;
};
typedef struct STEntryOne               STEntryOne;
struct STEntryTwo {
  UInt16              newState;
  UInt16              flags;
  UInt16              offset1;
  UInt16              offset2;
};
typedef struct STEntryTwo               STEntryTwo;
/* --------------------------------------------------------------------------- */
/* GENERAL FORMATS FOR STATE TABLES to be used with 'morx' tables -- prefix "STX" */
enum {
  kSTXHasLigAction              = 0x2000
};


typedef UInt16                          STXClass;
typedef UInt16                          STXStateIndex;
typedef UInt16                          STXEntryIndex;
struct STXHeader {
  UInt32              nClasses;
  UInt32              classTableOffset;
  UInt32              stateArrayOffset;
  UInt32              entryTableOffset;
};
typedef struct STXHeader                STXHeader;

typedef SFNTLookupTable                 STXClassTable;
struct STXEntryZero {
  STXStateIndex       newState;
  UInt16              flags;
};
typedef struct STXEntryZero             STXEntryZero;
struct STXEntryOne {
  STXStateIndex       newState;
  UInt16              flags;
  UInt16              index1;
};
typedef struct STXEntryOne              STXEntryOne;
struct STXEntryTwo {
  STXStateIndex       newState;
  UInt16              flags;
  UInt16              index1;
  UInt16              index2;
};
typedef struct STXEntryTwo              STXEntryTwo;
/* --------------------------------------------------------------------------- */
/* GENERAL FORMATS FOR STATE TABLES to be used with 'kerx' tables -- prefix "STK" */
enum {
  kSTKCrossStreamReset          = 0x2000
};
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'lcar' */
/* CONSTANTS */
enum {
  kLCARTag                      = 0x6C636172, /* 'lcar' */
  kLCARCurrentVersion           = 0x00010000, /* current version number for 'lcar' table */
  kLCARLinearFormat             = 0,
  kLCARCtlPointFormat           = 1
};

/* TYPES */
struct LcarCaretClassEntry {
  UInt16              count;
  UInt16              partials[1];            /* these are either FUnits or control-point numbers */
};
typedef struct LcarCaretClassEntry      LcarCaretClassEntry;
struct LcarCaretTable {
  Fixed               version;
  UInt16              format;
  SFNTLookupTable     lookup;
};
typedef struct LcarCaretTable           LcarCaretTable;
typedef LcarCaretTable *                LcarCaretTablePtr;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'just' */
/* CONSTANTS */
enum {
  kJUSTTag                      = 0x6A757374, /* 'just' */
  kJUSTCurrentVersion           = 0x00010000,
  kJUSTStandardFormat           = 0,
  kJUSTnoGlyphcode              = 0xFFFF, /* used in a pcConditionalAddAction */
  kJUSTpcDecompositionAction    = 0,
  kJUSTpcUnconditionalAddAction = 1,
  kJUSTpcConditionalAddAction   = 2,
  kJUSTpcGlyphStretchAction     = 3,
  kJUSTpcDuctilityAction        = 4,
  kJUSTpcGlyphRepeatAddAction   = 5
};

/* Justification priority levels */
enum {
  kJUSTKashidaPriority          = 0,
  kJUSTSpacePriority            = 1,
  kJUSTLetterPriority           = 2,
  kJUSTNullPriority             = 3,
  kJUSTPriorityCount            = 4
};

/* Justification flags */
enum {
  kJUSTOverridePriority         = 0x8000,
  kJUSTOverrideLimits           = 0x4000,
  kJUSTOverrideUnlimited        = 0x2000,
  kJUSTUnlimited                = 0x1000,
  kJUSTPriorityMask             = 0x0003
};

/* TYPES */
typedef UInt16                          JustPCActionType;
typedef UInt16                          JustificationFlags;
/* A JustPCDecompositionAction defines a ligature decomposition action. */
struct JustPCDecompositionAction {
  Fixed               lowerLimit;
  Fixed               upperLimit;
  UInt16              order;
  UInt16              count;
  UInt16              glyphs[1];
};
typedef struct JustPCDecompositionAction JustPCDecompositionAction;
/* A JUSTPCUnconditionalAddAction defines an unconditional glyph add action. */

typedef UInt16                          JustPCUnconditionalAddAction;
/*
    A JUSTPCConditionalAddAction defines a glyph substitution and add action. If the addGlyph
    is equal to kJUSTnoGlyphcode, then no glyph will be added, and the justification for
    the line will be redone.
*/
struct JustPCConditionalAddAction {
  Fixed               substThreshold;        /* threshold of growth factor at which subst occurs */
  UInt16              addGlyph;
  UInt16              substGlyph;
};
typedef struct JustPCConditionalAddAction JustPCConditionalAddAction;
/* A PCDuctilityAction defines a ductile axis along which the glyph will be varied. */
struct JustPCDuctilityAction {
  UInt32              ductilityAxis;
  Fixed               minimumLimit;
  Fixed               noStretchValue;
  Fixed               maximumLimit;
};
typedef struct JustPCDuctilityAction    JustPCDuctilityAction;
/*
    A PCGlyphRepetitionAction defines a glyph which will not be stretched or otherwise
    transformed, but rather which will be emplaced however many times are needed to fill
    the needed gap.
*/
struct JustPCGlyphRepeatAddAction {
  UInt16              flags;
  UInt16              glyph;
};
typedef struct JustPCGlyphRepeatAddAction JustPCGlyphRepeatAddAction;
/* PCActionSubrecords contain the actual postcompensation actions. */
struct JustPCActionSubrecord {
  UInt16              theClass;               /* justification class value associated with this rec */
  JustPCActionType    theType;
  UInt32              length;
  UInt32              data;                   /* not really a UInt32; cast as ptr to appropriate action */
};
typedef struct JustPCActionSubrecord    JustPCActionSubrecord;
/* The set of postcompensation records is defined in a PCAction struct. */
struct JustPCAction {
  UInt32              actionCount;            /* long for alignment purposes */
  JustPCActionSubrecord  actions[1];
};
typedef struct JustPCAction             JustPCAction;
/*
    JustWidthDeltaEntry is the justification table entry structure.  The justClass value (which is
    actually limited to 7 bits by the state table structure) is defined as a long for PPC alignment reasons.
*/
struct JustWidthDeltaEntry {
  UInt32              justClass;
  Fixed               beforeGrowLimit;        /* ems AW can grow by at most on LT */
  Fixed               beforeShrinkLimit;      /* ems AW can shrink by at most on LT */
  Fixed               afterGrowLimit;         /* ems AW can grow by at most on RB */
  Fixed               afterShrinkLimit;       /* ems AW can shrink by at most on RB */
  JustificationFlags  growFlags;              /* flags controlling grow case */
  JustificationFlags  shrinkFlags;            /* flags controlling shrink case */
};
typedef struct JustWidthDeltaEntry      JustWidthDeltaEntry;
struct JustWidthDeltaGroup {
  UInt32              count;
  JustWidthDeltaEntry  entries[1];
};
typedef struct JustWidthDeltaGroup      JustWidthDeltaGroup;
/* Overall structure of a postcompensation table is defined in PostcompTable. */
struct JustPostcompTable {
  SFNTLookupTable     lookupTable;
                                              /* action records here */
};
typedef struct JustPostcompTable        JustPostcompTable;
struct JustDirectionTable {
  UInt16              justClass;              /* offset to state table (0=none) */
  UInt16              widthDeltaClusters;     /* offset to clusters */
  UInt16              postcomp;               /* offset to postcomp table (0=none) */
  SFNTLookupTable     lookup;
};
typedef struct JustDirectionTable       JustDirectionTable;
struct JustTable {
  Fixed               version;
  UInt16              format;
  UInt16              horizHeaderOffset;
  UInt16              vertHeaderOffset;
};
typedef struct JustTable                JustTable;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'opbd' */
/* CONSTANTS */
enum {
  kOPBDTag                      = 0x6F706264, /* 'opbd' */
  kOPBDCurrentVersion           = 0x00010000,
  kOPBDDistanceFormat           = 0,
  kOPBDControlPointFormat       = 1
};

/* TYPES */

typedef UInt16                          OpbdTableFormat;
/*
    The OpbdSideValues struct is the lookup result from the FindSingle call for the
    optical tables. It contains the 4 FUnit values that are relevant to the specified
    glyph, or the 4 control gxPoint values.
*/
struct OpbdSideValues {
  SInt16              leftSideShift;
  SInt16              topSideShift;
  SInt16              rightSideShift;
  SInt16              bottomSideShift;
};
typedef struct OpbdSideValues           OpbdSideValues;
struct OpbdTable {
  Fixed               version;
  OpbdTableFormat     format;
  SFNTLookupTable     lookupTable;
};
typedef struct OpbdTable                OpbdTable;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'mort' */
/* CONSTANTS */
enum {
  kMORTTag                      = 0x6D6F7274, /* 'mort' */
  kMORTCurrentVersion           = 0x00010000, /* current version number for 'mort' table */
                                        /* Coverage masks */
  kMORTCoverVertical            = 0x8000,
  kMORTCoverDescending          = 0x4000,
  kMORTCoverIgnoreVertical      = 0x2000,
  kMORTCoverTypeMask            = 0x000F, /* Subtable types */
  kMORTRearrangementType        = 0,
  kMORTContextualType           = 1,
  kMORTLigatureType             = 2,
  kMORTSwashType                = 4,
  kMORTInsertionType            = 5,    /* Ligature subtable constants */
  kMORTLigLastAction            = (int)0x80000000,
  kMORTLigStoreLigature         = 0x40000000,
  kMORTLigFormOffsetMask        = 0x3FFFFFFF,
  kMORTLigFormOffsetShift       = 2,    /* Rearrangement subtable actions */
  kMORTraNoAction               = 0,    /*      no action      */
  kMORTraxA                     = 1,    /*      Ax => xA   */
  kMORTraDx                     = 2,    /*      xD => Dx   */
  kMORTraDxA                    = 3,    /*     AxD => DxA      */
  kMORTraxAB                    = 4,    /*     ABx => xAB      */
  kMORTraxBA                    = 5,    /*     ABx => xBA      */
  kMORTraCDx                    = 6,    /*     xCD => CDx      */
  kMORTraDCx                    = 7,    /*     xCD => DCx      */
  kMORTraCDxA                   = 8,    /*    AxCD => CDxA  */
  kMORTraDCxA                   = 9,    /*    AxCD => DCxA  */
  kMORTraDxAB                   = 10,   /*    ABxD => DxAB  */
  kMORTraDxBA                   = 11,   /*    ABxD => DxBA  */
  kMORTraCDxAB                  = 12,   /* ABxCD => CDxAB */
  kMORTraCDxBA                  = 13,   /* ABxCD => CDxBA */
  kMORTraDCxAB                  = 14,   /* ABxCD => DCxAB */
  kMORTraDCxBA                  = 15,   /* ABxCD => DCxBA */
                                        /* Insertion subtable constants */
  kMORTDoInsertionsBefore       = 0x80,
  kMORTIsSplitVowelPiece        = 0x40,
  kMORTInsertionsCountMask      = 0x3F,
  kMORTCurrInsertKashidaLike    = 0x2000,
  kMORTMarkInsertKashidaLike    = 0x1000,
  kMORTCurrInsertBefore         = 0x0800,
  kMORTMarkInsertBefore         = 0x0400,
  kMORTMarkJustTableCountMask   = 0x3F80,
  kMORTMarkJustTableCountShift  = 7,    /* JustTableIndex for marked character */
  kMORTCurrJustTableCountMask   = 0x007F,
  kMORTCurrJustTableCountShift  = 0,    /* JustTableIndex for current character */
  kMORTCurrInsertCountMask      = 0x03E0,
  kMORTCurrInsertCountShift     = 5,    /* count to insert after current glyphRec */
  kMORTMarkInsertCountMask      = 0x001F,
  kMORTMarkInsertCountShift     = 0     /* count to insert after marked glyphRec */
};

/* TYPES */

typedef UInt32                          MortSubtableMaskFlags;
typedef UInt32                          MortLigatureActionEntry;
struct MortRearrangementSubtable {
  STHeader            header;
};
typedef struct MortRearrangementSubtable MortRearrangementSubtable;
struct MortContextualSubtable {
  STHeader            header;
  UInt16              substitutionTableOffset;
};
typedef struct MortContextualSubtable   MortContextualSubtable;
struct MortLigatureSubtable {
  STHeader            header;
  UInt16              ligatureActionTableOffset;
  UInt16              componentTableOffset;
  UInt16              ligatureTableOffset;
};
typedef struct MortLigatureSubtable     MortLigatureSubtable;
struct MortSwashSubtable {
  SFNTLookupTable     lookup;
};
typedef struct MortSwashSubtable        MortSwashSubtable;
struct MortInsertionSubtable {
  STHeader            header;
};
typedef struct MortInsertionSubtable    MortInsertionSubtable;
union MortSpecificSubtable {
  MortRearrangementSubtable  rearrangement;
  MortContextualSubtable  contextual;
  MortLigatureSubtable  ligature;
  MortSwashSubtable   swash;
  MortInsertionSubtable  insertion;
};
typedef union MortSpecificSubtable      MortSpecificSubtable;
struct MortSubtable {
  UInt16              length;
  UInt16              coverage;
  MortSubtableMaskFlags  flags;
  MortSpecificSubtable  u;
};
typedef struct MortSubtable             MortSubtable;
struct MortFeatureEntry {
  UInt16              featureType;
  UInt16              featureSelector;
  MortSubtableMaskFlags  enableFlags;
  MortSubtableMaskFlags  disableFlags;
};
typedef struct MortFeatureEntry         MortFeatureEntry;
struct MortChain {
  MortSubtableMaskFlags  defaultFlags;        /* default flags for this chain */
  UInt32              length;                 /* byte length of this chain */
  UInt16              nFeatures;              /* number of feature entries */
  UInt16              nSubtables;             /* number of subtables */
  MortFeatureEntry    featureEntries[1];
                                              /* the subtables follow */
};
typedef struct MortChain                MortChain;
struct MortTable {
  Fixed               version;
  UInt32              nChains;
  MortChain           chains[1];
};
typedef struct MortTable                MortTable;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'morx' (version 2 and beyond metamorphosis tables) */
/* CONSTANTS */
enum {
  kMORXTag                      = 0x6D6F7278, /* 'morx' */
  kMORXCurrentVersion           = 0x00020000, /* version number for current 'morx' table */
                                        /* Coverage masks */
  kMORXCoverVertical            = (int)0x80000000,
  kMORXCoverDescending          = 0x40000000,
  kMORXCoverIgnoreVertical      = 0x20000000,
  kMORXCoverLogicalOrder        = 0x10000000,
  kMORXCoverTypeMask            = 0x000000FF
};

/* TYPES */
struct MorxRearrangementSubtable {
  STXHeader           header;
};
typedef struct MorxRearrangementSubtable MorxRearrangementSubtable;
struct MorxContextualSubtable {
  STXHeader           header;
  UInt32              substitutionTableOffset;
};
typedef struct MorxContextualSubtable   MorxContextualSubtable;
struct MorxLigatureSubtable {
  STXHeader           header;
  UInt32              ligatureActionTableOffset;
  UInt32              componentTableOffset;
  UInt32              ligatureTableOffset;
};
typedef struct MorxLigatureSubtable     MorxLigatureSubtable;
struct MorxInsertionSubtable {
  STXHeader           header;
  UInt32              insertionGlyphTableOffset;
};
typedef struct MorxInsertionSubtable    MorxInsertionSubtable;
union MorxSpecificSubtable {
  MorxRearrangementSubtable  rearrangement;
  MorxContextualSubtable  contextual;
  MorxLigatureSubtable  ligature;
  MortSwashSubtable   swash;
  MorxInsertionSubtable  insertion;
};
typedef union MorxSpecificSubtable      MorxSpecificSubtable;
struct MorxSubtable {
  UInt32              length;
  UInt32              coverage;
  MortSubtableMaskFlags  flags;
  MorxSpecificSubtable  u;
};
typedef struct MorxSubtable             MorxSubtable;
struct MorxChain {
  MortSubtableMaskFlags  defaultFlags;        /* default flags for this chain */
  UInt32              length;                 /* byte length of this chain */
  UInt32              nFeatures;              /* number of feature entries */
  UInt32              nSubtables;             /* number of subtables */
  MortFeatureEntry    featureEntries[1];
                                              /* the subtables follow */
};
typedef struct MorxChain                MorxChain;
struct MorxTable {
  Fixed               version;
  UInt32              nChains;
  MorxChain           chains[1];
};
typedef struct MorxTable                MorxTable;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'prop' */
/* CONSTANTS */
enum {
  kPROPTag                      = 0x70726F70, /* 'prop' */
  kPROPCurrentVersion           = 0x00030000, /* current version number for 'prop' table */
  kPROPPairOffsetShift          = 8,
  kPROPPairOffsetSign           = 7,
  kPROPIsFloaterMask            = 0x8000, /* glyph is floater */
  kPROPCanHangLTMask            = 0x4000, /* glyph can hang left/top */
  kPROPCanHangRBMask            = 0x2000, /* glyph can hang right/bottom */
  kPROPUseRLPairMask            = 0x1000, /* if glyph lands in RL streak, use paired glyph */
  kPROPPairOffsetMask           = 0x0F00, /* 4-bit signed offset to other pair member */
  kPROPRightConnectMask         = 0x0080, /* glyph connects to glyph on right */
  kPROPZeroReserved             = 0x0060, /* must be zero */
  kPROPDirectionMask            = 0x001F /* direction bits */
};

/* These are the Unicode direction classes (plus the Special European Number class). */
enum {
  kPROPLDirectionClass          = 0,    /* Left-to-Right */
  kPROPRDirectionClass          = 1,    /* Right-to-Left */
  kPROPALDirectionClass         = 2,    /* Right-to-Left Arabic Letter */
  kPROPENDirectionClass         = 3,    /* European Number */
  kPROPESDirectionClass         = 4,    /* European Number Separator */
  kPROPETDirectionClass         = 5,    /* European Number Terminator */
  kPROPANDirectionClass         = 6,    /* Arabic Number */
  kPROPCSDirectionClass         = 7,    /* Common Number Separator */
  kPROPPSDirectionClass         = 8,    /* Paragraph Separator (also referred to as Block Separator) */
  kPROPSDirectionClass          = 9,    /* Segment Separator */
  kPROPWSDirectionClass         = 10,   /* Whitespace */
  kPROPONDirectionClass         = 11,   /* Other Neutral */
  kPROPSENDirectionClass        = 12,   /* Special European Number (not a Unicode class) */
  kPROPLREDirectionClass        = 13,   /* Left-to-Right Embedding */
  kPROPLRODirectionClass        = 14,   /* Left-to-Right Override */
  kPROPRLEDirectionClass        = 15,   /* Right-to-Left Embedding */
  kPROPRLODirectionClass        = 16,   /* Right-to-Left Override */
  kPROPPDFDirectionClass        = 17,   /* Pop Directional Format */
  kPROPNSMDirectionClass        = 18,   /* Non-Spacing Mark */
  kPROPBNDirectionClass         = 19,   /* Boundary Neutral */
  kPROPNumDirectionClasses      = 20    /* Number of Unicode directional types + Special European Number */
};

/* TYPES */

typedef UInt16                          PropCharProperties;
struct PropTable {
  Fixed               version;
  UInt16              format;
  PropCharProperties  defaultProps;
  SFNTLookupTable     lookup;
};
typedef struct PropTable                PropTable;
struct PropLookupSegment {
  UInt16              lastGlyph;
  UInt16              firstGlyph;
  UInt16              value;
};
typedef struct PropLookupSegment        PropLookupSegment;
struct PropLookupSingle {
  UInt16              glyph;
  PropCharProperties  props;
};
typedef struct PropLookupSingle         PropLookupSingle;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'trak' */
/* CONSTANTS */
enum {
  kTRAKTag                      = 0x7472616B, /* 'trak' */
  kTRAKCurrentVersion           = 0x00010000, /* current version number for 'trak' table */
  kTRAKUniformFormat            = 0     /*    kTRAKPerGlyphFormat         = 2*/
};

/* TYPES */

typedef SInt16                          TrakValue;
struct TrakTableEntry {
  Fixed               track;
  UInt16              nameTableIndex;
  UInt16              sizesOffset;            /* offset to array of TrackingValues */
};
typedef struct TrakTableEntry           TrakTableEntry;
struct TrakTableData {
  UInt16              nTracks;
  UInt16              nSizes;
  UInt32              sizeTableOffset;
  TrakTableEntry      trakTable[1];
};
typedef struct TrakTableData            TrakTableData;
struct TrakTable {
  Fixed               version;
  UInt16              format;
  UInt16              horizOffset;
  UInt16              vertOffset;
};
typedef struct TrakTable                TrakTable;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'kern' */
/* CONSTANTS */
enum {
  kKERNTag                      = 0x6B65726E, /* 'kern' */
  kKERNCurrentVersion           = 0x00010000,
  kKERNVertical                 = 0x8000, /* set if this table has vertical kerning information */
  kKERNResetCrossStream         = 0x8000, /* this value in a cross-stream table means reset to zero */
  kKERNCrossStream              = 0x4000, /* set if this table contains cross-stream kerning values */
  kKERNVariation                = 0x2000, /* set if this table contains variation kerning values */
  kKERNUnusedBits               = 0x1F00, /* UNUSED, MUST BE ZERO */
  kKERNFormatMask               = 0x00FF /* format of this subtable */
};

enum {
  kKERNOrderedList              = 0,    /* ordered list of kerning pairs */
  kKERNStateTable               = 1,    /* state table for n-way contextual kerning */
  kKERNSimpleArray              = 2,    /* simple n X m array of kerning values */
  kKERNIndexArray               = 3     /* modified version of SimpleArray */
};

/* Message Type Flags */
enum {
  kKERNLineStart                = 0x00000001, /* Array of glyphs starts a line */
  kKERNLineEndKerning           = 0x00000002, /* Array of glyphs ends a line */
  kKERNNoCrossKerning           = 0x00000004, /* Prohibit cross kerning */
  kKERNNotesRequested           = 0x00000008, /* Caller wants kerning notes */
  kKERNNoStakeNote              = 1,    /* Indicates a glyph was involved in a kerning pair/group */
  kKERNCrossStreamResetNote     = 2,    /* Indicates a return-to-baseline in cross-stream kerning */
  kKERNNotApplied               = 0x00000001 /* All kerning values were zero, kerning call had no effect */
};

/* TYPES */

typedef UInt8                           KernTableFormat;
typedef UInt16                          KernSubtableInfo;
typedef SInt16                          KernKerningValue;
typedef UInt16                          KernArrayOffset;
/* header for version 0 kerning table */
struct KernVersion0Header {
  UInt16              version;                /* font version number (will be 0!) */
  UInt16              nTables;                /* number of subtables present */
  UInt16              firstSubtable[1];       /* first subtable starts here */
};
typedef struct KernVersion0Header       KernVersion0Header;
/* Header for a kerning table */
struct KernTableHeader {
  Fixed               version;                /* font version number (currently 1.0) */
  SInt32              nTables;                /* number of subtables present */
  UInt16              firstSubtable[1];       /* first subtable starts here */
};
typedef struct KernTableHeader          KernTableHeader;
typedef KernTableHeader *               KernTableHeaderPtr;
typedef KernTableHeaderPtr *            KernTableHeaderHandle;
/*
    F O R M A T   S P E C I F I C   D E F I N I T I O N S

    kernOrderedList:
    
    The table is a sorted list of [left glyph, right glyph, value] triples.
    There's enough information in the header so that the list can be
    efficiently binary searched. 
*/
/* defines a single kerning pair of Glyphcodes  */
struct KernKerningPair {
  UInt16              left;
  UInt16              right;
};
typedef struct KernKerningPair          KernKerningPair;
/* a single list entry */
struct KernOrderedListEntry {
  KernKerningPair     pair;                   /* the kerning pair */
  KernKerningValue    value;                  /* the kerning value for the above pair */
};
typedef struct KernOrderedListEntry     KernOrderedListEntry;
typedef KernOrderedListEntry *          KernOrderedListEntryPtr;
/* the header information for binary searching the list */
struct KernOrderedListHeader {
  UInt16              nPairs;                 /* number of kerning pairs in table */
  UInt16              searchRange;            /* (largest power of two <= nPairs) * entry size */
  UInt16              entrySelector;          /* log2 (largest power of two <= nPairs) */
  UInt16              rangeShift;             /* (nPairs - largest power of two <= nPairs) * entry size */
  UInt16              table[1];               /* entries are first glyph, second glyph, and value */
};
typedef struct KernOrderedListHeader    KernOrderedListHeader;
/* KernStateTable: like the the generic state tables */
struct KernStateHeader {
  STHeader            header;                 /* state table header */
  UInt16              valueTable;             /* offset to kerning value table */
  UInt8               firstTable[1];          /* first table starts here */
};
typedef struct KernStateHeader          KernStateHeader;
struct KernStateEntry {
  UInt16              newState;
  UInt16              flags;                  /* flags per above enum */
};
typedef struct KernStateEntry           KernStateEntry;
/*
    Kern offset table header.
    The offset table is a trimmed array from firstGlyph to limitGlyph.
    Glyphs outside of this range should get zero for right-hand glyphs
    and the offset of the beginning of the kerning array for left-hand glyphs.
*/
struct KernOffsetTable {
  UInt16              firstGlyph;             /* first glyph in class range */
  UInt16              nGlyphs;                /* number of glyphs in class range */
  KernArrayOffset     offsetTable[1];         /* offset table starts here */
};
typedef struct KernOffsetTable          KernOffsetTable;
typedef KernOffsetTable *               KernOffsetTablePtr;
/* Header information for accessing offset tables and kerning array */
/*
    KernSimpleArray:
    
    The array is an nXm array of kerning values. Each row in the array
    represents one left-hand glyph, and each column one right-hand glyph.
    The zeroth row and column always represent glyphs that are out of bounds
    and will always contain zero.
    
    A pair is looked up by indexing the left-hand glyph through the left
    offset table, the right-hand glyph through the right offset table,
    adding both offsets to the starting address of the kerning array,
    and fetching the kerning value pointed to.
*/
/* Kern offset table header. */
/* The offset table is a trimmed array from firstGlyph to limitGlyph. */
/* Glyphs outside of this range should get zero for right-hand glyphs */
/* and the offset of the beginning of the kerning array for left- */
/* hand glyphs. */
struct KernSimpleArrayHeader {
  UInt16              rowWidth;               /* width, in bytes, of a row in the table */
  UInt16              leftOffsetTable;        /* offset to left-hand offset table */
  UInt16              rightOffsetTable;       /* offset to right-hand offset table */
  KernArrayOffset     theArray;               /* offset to start of kerning array */
  UInt16              firstTable[1];          /* first offset table starts here... */
};
typedef struct KernSimpleArrayHeader    KernSimpleArrayHeader;
/* Index Array */
struct KernIndexArrayHeader {
  UInt16              glyphCount;
  UInt8               kernValueCount;
  UInt8               leftClassCount;
  UInt8               rightClassCount;
  UInt8               flags;                  /* set to 0 for now */
  SInt16              kernValue[1];           /* actual kerning values reference by index in kernIndex */
  UInt8               leftClass[1];           /* maps left glyph to offset into kern index */
  UInt8               rightClass[1];          /* maps right glyph to offset into kern index */
  UInt8               kernIndex[1];           /* contains indicies into kernValue */
};
typedef struct KernIndexArrayHeader     KernIndexArrayHeader;
/* format specific part of subtable header */
union KernFormatSpecificHeader {
  KernOrderedListHeader  orderedList;
  KernStateHeader     stateTable;
  KernSimpleArrayHeader  simpleArray;
  KernIndexArrayHeader  indexArray;
};
typedef union KernFormatSpecificHeader  KernFormatSpecificHeader;
/* version 0 subtable header */
struct KernVersion0SubtableHeader {
  UInt16              version;                /* kerning table version number */
  UInt16              length;                 /* length in bytes (including this header) */
  KernSubtableInfo    stInfo;                 /* sub-table info */
  KernFormatSpecificHeader  fsHeader;         /* format specific sub-header */
};
typedef struct KernVersion0SubtableHeader KernVersion0SubtableHeader;
/* Overall Subtable header format */
struct KernSubtableHeader {
  SInt32              length;                 /* length in bytes (including this header) */
  KernSubtableInfo    stInfo;                 /* subtable info */
  SInt16              tupleIndex;             /* tuple index for variation subtables */
  KernFormatSpecificHeader  fsHeader;         /* format specific sub-header */
};
typedef struct KernSubtableHeader       KernSubtableHeader;
typedef KernSubtableHeader *            KernSubtableHeaderPtr;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'kerx' */
/* CONSTANTS */
enum {
  kKERXTag                      = 0x6B657278, /* 'kerx' */
  kKERXCurrentVersion           = 0x00020000,
  kKERXVertical                 = (int)0x80000000, /* set if this table has vertical kerning information */
  kKERXResetCrossStream         = 0x8000, /* this value in a cross-stream table means reset to zero */
  kKERXCrossStream              = 0x40000000, /* set if this table contains cross-stream kerning values */
  kKERXVariation                = 0x20000000, /* set if this table contains variation kerning values */
  kKERXDescending               = 0x10000000,
  kKERXUnusedBits               = 0x0FFFFF00, /* UNUSED, MUST BE ZERO */
  kKERXFormatMask               = 0x000000FF /* format of this subtable */
};

enum {
  kKERXOrderedList              = 0,    /* ordered list of kerning pairs */
  kKERXStateTable               = 1,    /* state table for n-way contextual kerning */
  kKERXSimpleArray              = 2,    /* simple n X m array of kerning values */
  kKERXControlPoint             = 4,    /* state table for control point positioning */
  kKERXIndexArray               = 6     /* index-based n X m array of kerning values */
};

/* Message Type Flags */
enum {
  kKERXLineStart                = 0x00000001, /* Array of glyphs starts a line */
  kKERXLineEndKerning           = 0x00000002, /* Array of glyphs ends a line */
  kKERXNoCrossKerning           = 0x00000004, /* Prohibit cross kerning */
  kKERXNotesRequested           = 0x00000008, /* Caller wants kerning notes */
  kKERXNoStakeNote              = 1,    /* Indicates a glyph was involved in a kerning pair/group */
  kKERXCrossStreamResetNote     = 2,    /* Indicates a return-to-baseline in cross-stream kerning */
  kKERXNotApplied               = 0x00000001 /* All kerning values were zero, kerning call had no effect */
};

/* Flags in KerxControlPointHeader */
enum {
  kKERXActionTypeMask           = (3U << 30), /* Mask to extract action type */
  kKERXActionTypeControlPoints  = (0U << 30), /* Actions have control point numbers */
  kKERXActionTypeAnchorPoints   = (1U << 30), /* Actions have anchor point numbers */
  kKERXActionTypeCoordinates    = (2U << 30), /* Actions have control point coordinates */
  kKERXUnusedFlags              = 0x3F000000, /* Unused, must be zero */
  kKERXActionOffsetMask         = 0x00FFFFFF, /* Mask to extract offset to action table */
};

/* Flags in KerxIndexArrayHeader */
enum {
  kKERXValuesAreLong            = 0x00000001
};

/* TYPES */
typedef UInt32                          KerxSubtableCoverage;
typedef UInt32                          KerxArrayOffset;
/* Header for an extended kerning table */
struct KerxTableHeader {
  Fixed               version;                /* font version number (currently 1.0) */
  UInt32              nTables;                /* number of subtables present */
  UInt32              firstSubtable[1];       /* first subtable starts here */
};
typedef struct KerxTableHeader          KerxTableHeader;
typedef KerxTableHeader *               KerxTableHeaderPtr;
typedef KerxTableHeaderPtr *            KerxTableHeaderHandle;
/*
 F O R M A T   S P E C I F I C   D E F I N I T I O N S
 
 kerxOrderedList:
 
 The table is a sorted list of [left glyph, right glyph, value] triples.
 There's enough information in the header so that the list can be
 efficiently binary searched. 
 */
/* defines a single kerning pair of Glyphcodes  */
struct KerxKerningPair {
  UInt16              left;
  UInt16              right;
};
typedef struct KerxKerningPair          KerxKerningPair;
/* a single list entry */
struct KerxOrderedListEntry {
  KerxKerningPair     pair;                   /* the kerning pair */
  KernKerningValue    value;                  /* the kerning value for the above pair */
};
typedef struct KerxOrderedListEntry     KerxOrderedListEntry;
typedef KerxOrderedListEntry *          KerxOrderedListEntryPtr;
/* the header information for binary searching the list */
struct KerxOrderedListHeader {
  UInt32              nPairs;                 /* number of kerning pairs in table */
  UInt32              searchRange;            /* (largest power of two <= nPairs) * entry size */
  UInt32              entrySelector;          /* log2 (largest power of two <= nPairs) */
  UInt32              rangeShift;             /* (nPairs - largest power of two <= nPairs) * entry size */
  UInt32              table[1];               /* entries are first glyph, second glyph, and value */
};
typedef struct KerxOrderedListHeader    KerxOrderedListHeader;
/* KernStateTable: like the the generic state tables */
struct KerxStateHeader {
  STXHeader           header;                 /* state table header */
  UInt32              valueTable;             /* offset to kerning value table */
  UInt8               firstTable[1];          /* first table starts here */
};
typedef struct KerxStateHeader          KerxStateHeader;
struct KerxStateEntry {
  UInt16              newState;
  UInt16              flags;                  /* flags per above enum */
  UInt16              valueIndex;
};
typedef struct KerxStateEntry           KerxStateEntry;
/* KerxControlPointTable: like the the generic state tables */
struct KerxControlPointHeader {
  STXHeader           header;                 /* state table header */
  UInt32              flags;                  /* flags */
  UInt8               firstTable[1];          /* first table starts here */
};
typedef struct KerxControlPointHeader   KerxControlPointHeader;
struct KerxControlPointEntry {
  UInt16              newState;
  UInt16              flags;                  /* flags per above enum */
  UInt16              actionIndex;
};
typedef struct KerxControlPointEntry    KerxControlPointEntry;
struct KerxControlPointAction {
  UInt16              markControlPoint;
  UInt16              currControlPoint;
};
typedef struct KerxControlPointAction   KerxControlPointAction;
struct KerxAnchorPointAction {
  UInt16              markAnchorPoint;
  UInt16              currAnchorPoint;
};
typedef struct KerxAnchorPointAction   KerxAnchorPointAction;
struct KerxCoordinateAction {
  UInt16              markX;
  UInt16              markY;
  UInt16              currX;
  UInt16              currY;
};
typedef struct KerxCoordinateAction     KerxCoordinateAction;
/*
 KerxSimpleArray:
 
 The array is an nXm array of kerning values. Each row in the array
 represents one left-hand glyph, and each column one right-hand glyph.
 The zeroth row and column always represent glyphs that are out of bounds
 and will always contain zero.
 
 A pair is looked up by indexing the left-hand glyph through the left
 offset table, the right-hand glyph through the right offset table,
 adding both offsets to the starting address of the kerning array,
 and fetching the kerning value pointed to.
 */
struct KerxSimpleArrayHeader {
  UInt32              rowWidth;               /* width, in bytes, of a row in the table */
  UInt32              leftOffsetTable;        /* offset to left-hand offset table */
  UInt32              rightOffsetTable;       /* offset to right-hand offset table */
  KerxArrayOffset     theArray;               /* offset to start of kerning array */
  UInt32              firstTable[1];          /* first offset table starts here... */
};
typedef struct KerxSimpleArrayHeader    KerxSimpleArrayHeader;
/* Index Array */
struct KerxIndexArrayHeader {
  UInt32              flags;
  UInt16              rowCount;
  UInt16              columnCount;
  UInt32              rowIndexTableOffset;    /* offset to row index lookup table */
  UInt32              columnIndexTableOffset; /* offset to column index offset table */
  UInt32              kerningArrayOffset;     /* offset to start of kerning array */
  UInt32              kerningVectorOffset;    /* offset to start of kerning vectors (if tupleCount is 1 or more) */
};
typedef struct KerxIndexArrayHeader     KerxIndexArrayHeader;
/* format specific part of subtable header */
union KerxFormatSpecificHeader {
  KerxOrderedListHeader  orderedList;
  KerxStateHeader     stateTable;
  KerxSimpleArrayHeader  simpleArray;
  KerxIndexArrayHeader  indexArray;
  KerxControlPointHeader  controlPoint;
};
typedef union KerxFormatSpecificHeader  KerxFormatSpecificHeader;
/* Overall Subtable header format */
struct KerxSubtableHeader {
  UInt32              length;                 /* length in bytes (including this header) */
  KerxSubtableCoverage  stInfo;               /* subtable coverage */
  UInt32              tupleCount;             /* tuple count for variation subtables (ignored if the 'kerx' table version is less than 4) */
  KerxFormatSpecificHeader  fsHeader;         /* format specific sub-header */
};
typedef struct KerxSubtableHeader       KerxSubtableHeader;
typedef KerxSubtableHeader *            KerxSubtableHeaderPtr;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'bsln' */
/* CONSTANTS */
enum {
  kBSLNTag                      = 0x62736C6E, /* 'bsln' */
  kBSLNCurrentVersion           = 0x00010000, /* current version number for 'bsln' table */
  kBSLNDistanceFormatNoMap      = 0,
  kBSLNDistanceFormatWithMap    = 1,
  kBSLNControlPointFormatNoMap  = 2,
  kBSLNControlPointFormatWithMap = 3
};

/* Baseline classes and constants */
enum {
  kBSLNRomanBaseline            = 0,
  kBSLNIdeographicCenterBaseline = 1,
  kBSLNIdeographicLowBaseline   = 2,
  kBSLNHangingBaseline          = 3,
  kBSLNMathBaseline             = 4,
  kBSLNIdeographicHighBaseline  = 5,
  kBSLNLastBaseline             = 31,
  kBSLNNumBaselineClasses       = kBSLNLastBaseline + 1,
  kBSLNNoBaseline               = 255,
  kBSLNNoBaselineOverride       = 255
};

/* TYPES */
typedef UInt32                          BslnBaselineClass;
/* The BslnBaselineRecord array defines the baseline deltas for the line. */
typedef Fixed                           BslnBaselineRecord[32];
/*
    BslnFormat0Part is the format-specific data for a distance table with no mapping (i.e.
    all the glyphs belong to the defaultBaseline).
*/
struct BslnFormat0Part {
  SInt16              deltas[32];
};
typedef struct BslnFormat0Part          BslnFormat0Part;
/* BslnFormat1Part is the format-specific data for a distance table with a gxMapping. */
struct BslnFormat1Part {
  SInt16              deltas[32];
  SFNTLookupTable     mappingData;
};
typedef struct BslnFormat1Part          BslnFormat1Part;
/*
    BslnFormat2Part is the format-specific data for a control-point table with no
    mapping (i.e. all the glyphs belong to the defaultBaseline). It specifies a single
    glyph to use and the set of control points in that glyph that designate each of
    the baselines.
*/
struct BslnFormat2Part {
  UInt16              stdGlyph;
  SInt16              ctlPoints[32];
};
typedef struct BslnFormat2Part          BslnFormat2Part;
/*
    BslnFormat3Part is the format-specific data for a distance table with a mapping. Like
    format 2, it contains a single glyph and its set of control-point values for each
    of the baselines.
*/
struct BslnFormat3Part {
  UInt16              stdGlyph;
  SInt16              ctlPoints[32];
  SFNTLookupTable     mappingData;
};
typedef struct BslnFormat3Part          BslnFormat3Part;
/* The BslnFormatUnion is a union containing the format-specific parts of the baseline table. */
union BslnFormatUnion {
  BslnFormat0Part     fmt0Part;
  BslnFormat1Part     fmt1Part;
  BslnFormat2Part     fmt2Part;
  BslnFormat3Part     fmt3Part;
};
typedef union BslnFormatUnion           BslnFormatUnion;
/* The table format used in BaselineTable */

typedef UInt16                          BslnTableFormat;
/* BaselineTable defines the top-level format of the baseline table in the font. */
struct BslnTable {
  Fixed               version;
  BslnTableFormat     format;
  UInt16              defaultBaseline;
  BslnFormatUnion     parts;
};
typedef struct BslnTable                BslnTable;
typedef BslnTable *                     BslnTablePtr;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'ALMX' */
/* TYPES */
struct ALMXHeader {
  Fixed               Version;                /* 16.16 format 1.0 by default */
  UInt16              Flags;
  UInt16              NMasters;
  UInt16              FirstGlyph;
  UInt16              LastGlyph;

  SFNTLookupTable     lookup;                 /* lookup table */
};
typedef struct ALMXHeader               ALMXHeader API_DEPRECATED("Superseded by OpenType", macos(10.0, 12.0), ios(1.0, 15.0), watchos(1.0, 8.0), tvos(1.0, 15.0));
struct ALMXGlyphEntry {                       /* lookup data for ALMX table */
  SInt16              GlyphIndexOffset;
  SInt16              HorizontalAdvance;
  SInt16              XOffsetToHOrigin;
  SInt16              VerticalAdvance;
  SInt16              YOffsetToVOrigin;
};
typedef struct ALMXGlyphEntry           ALMXGlyphEntry API_DEPRECATED("Superseded by OpenType", macos(10.0, 12.0), ios(1.0, 15.0), watchos(1.0, 8.0), tvos(1.0, 15.0));
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE: 'ROTA' */
/* TYPES */
struct ROTAHeader {
  Fixed               Version;                /* 16.16 format 1.0 by default */
  UInt16              Flags;
  UInt16              NMasters;
  UInt16              FirstGlyph;
  UInt16              LastGlyph;

  SFNTLookupTable     lookup;                 /* lookup table */
};
typedef struct ROTAHeader               ROTAHeader API_DEPRECATED("Superseded by OpenType", macos(10.0, 12.0), ios(1.0, 15.0), watchos(1.0, 8.0), tvos(1.0, 15.0));
struct ROTAGlyphEntry {                       /* lookup data for ROTA table */
  SInt16              GlyphIndexOffset;
  SInt16              HBaselineOffset;        /* y offset to the rotated horizontal baseline */
  SInt16              VBaselineOffset;        /* x offset to the rotated vertical baseline */
};
typedef struct ROTAGlyphEntry           ROTAGlyphEntry API_DEPRECATED("Superseded by OpenType", macos(10.0, 12.0), ios(1.0, 15.0), watchos(1.0, 8.0), tvos(1.0, 15.0));
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE 'ankr' */
/* CONSTANTS */
enum {
  kANKRCurrentVersion = 0
};

/* TYPES */
struct AnchorPoint {
  SInt16               x;                     /* x coordinate of anchor point */
  SInt16               y;                     /* y coordinate of anchor point */
};
typedef struct AnchorPoint              AnchorPoint;

struct AnchorPointTable {
  UInt32              nPoints;                /* number of anchor points defined for this glyph */
  AnchorPoint         points[1];              /* first anchor point starts here */
};
typedef struct AnchorPointTable         AnchorPointTable;

struct AnkrTable {
  UInt16              version;                /* 0 */
  UInt16              flags;                  /* never leave home without them (see 'Zapf') */
  UInt32              lookupTableOffset;      /* Byte offset to lookup table mapping glyphs to offset into anchor point table */
  UInt32              anchorPointTableOffset; /* Byte offset to start of anchor point table */
};
typedef struct AnkrTable                AnkrTable;
/* --------------------------------------------------------------------------- */
/* FORMATS FOR TABLE 'ltag' */
/* CONSTANTS */
enum {
  kLTAGCurrentVersion = 1
};

/* TYPES */
struct LtagStringRange {
  UInt16               offset;                /* offset to the beginning of the string */
  UInt16               length;                /* string length in bytes */
};
typedef struct LtagStringRange          LtagStringRange;

struct LtagTable {
  UInt32              version;                /* 1 */
  UInt32              flags;                  /* none currently defined */
  UInt32              numTags;                /* number of language tags which follow */
  LtagStringRange     tagRange[1];            /* first string range starts here */
};
typedef struct LtagTable                LtagTable;
/* --------------------------------------------------------------------------- */

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif // __SFNTLAYOUTTYPES__
