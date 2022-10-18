/*
     File:       CarbonCore/UnicodeUtilities.h
 
     Contains:   Types, constants, prototypes for Unicode Utilities (Unicode input and text utils)
 
     Copyright:  © 1997-2016 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __UNICODEUTILITIES__
#define __UNICODEUTILITIES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif


#ifndef __MACLOCALES__
#include <CarbonCore/MacLocales.h>
#endif

#ifndef __TEXTCOMMON__
#include <CarbonCore/TextCommon.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __CFCHARACTERSET__
#include <CoreFoundation/CFCharacterSet.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for UCKeyTranslate & UCKeyboardLayout ('uchr' resource)
   -------------------------------------------------------------------------------------------------
*/

/*
   -------------------------------------------------------------------------------------------------
   UCKeyOutput & related stuff
   The interpretation of UCKeyOutput depends on bits 15-14.
   If they are 01, then bits 0-13 are an index in UCKeyStateRecordsIndex (resource-wide list).
   If they are 10, then bits 0-13 are an index in UCKeySequenceDataIndex (resource-wide list),
     or if UCKeySequenceDataIndex is not present or the index is beyond the end of the list,
     then bits 0-15 are a single Unicode character.
   Otherwise, bits 0-15 are a single Unicode character; a value of 0xFFFE-0xFFFF means no character
     output.
   UCKeyCharSeq is similar, but does not support indices in UCKeyStateRecordsIndex. For bits 15-14:
   If they are 10, then bits 0-13 are an index in UCKeySequenceDataIndex (resource-wide list),
     or if UCKeySequenceDataIndex is not present or the index is beyond the end of the list,
     then bits 0-15 are a single Unicode character.
   Otherwise, bits 0-15 are a single Unicode character; a value of 0xFFFE-0xFFFF means no character
     output.
   -------------------------------------------------------------------------------------------------
*/

typedef UInt16                          UCKeyOutput;
typedef UInt16                          UCKeyCharSeq;
enum {
  kUCKeyOutputStateIndexMask    = 0x4000,
  kUCKeyOutputSequenceIndexMask = 0x8000,
  kUCKeyOutputTestForIndexMask  = 0xC000, /* test bits 14-15*/
  kUCKeyOutputGetIndexMask      = 0x3FFF /* get bits 0-13*/
};

/*
   -------------------------------------------------------------------------------------------------
   UCKeyStateRecord & related stuff
   The UCKeyStateRecord information is used as follows. If the current state is zero,
   output stateZeroCharData and set the state to stateZeroNextState. If the current state
   is non-zero and there is an entry for it in stateEntryData, then output the corresponding
   charData and set the state to nextState. Otherwise, output the state terminator from
   UCKeyStateTerminators for the current state (or nothing if there is no UCKeyStateTerminators
   table or it has no entry for the current state), then output stateZeroCharData and set the
   state to stateZeroNextState.
   -------------------------------------------------------------------------------------------------
*/

struct UCKeyStateRecord {
  UCKeyCharSeq        stateZeroCharData;
  UInt16              stateZeroNextState;
  UInt16              stateEntryCount;
  UInt16              stateEntryFormat;
                                              /* This is followed by an array of stateEntryCount elements*/
                                              /* in the specified format. Here we just show a dummy array.*/
  UInt32              stateEntryData[1];
};
typedef struct UCKeyStateRecord         UCKeyStateRecord;
/*
   Here are the codes for entry formats currently defined.
   Each entry maps from curState to charData and nextState.
*/
enum {
  kUCKeyStateEntryTerminalFormat = 0x0001,
  kUCKeyStateEntryRangeFormat   = 0x0002
};

/*
   For UCKeyStateEntryTerminal -
   nextState is always 0, so we don't have a field for it
*/

struct UCKeyStateEntryTerminal {
  UInt16              curState;
  UCKeyCharSeq        charData;
};
typedef struct UCKeyStateEntryTerminal  UCKeyStateEntryTerminal;
/*
   For UCKeyStateEntryRange -
   If curState >= curStateStart and curState <= curStateStart+curStateRange,
   then it matches the entry, and we transform charData and nextState as follows:
   If charData < 0xFFFE, then charData += (curState-curStateStart)*deltaMultiplier
   If nextState != 0, then nextState += (curState-curStateStart)*deltaMultiplier
*/
struct UCKeyStateEntryRange {
  UInt16              curStateStart;
  UInt8               curStateRange;
  UInt8               deltaMultiplier;
  UCKeyCharSeq        charData;
  UInt16              nextState;
};
typedef struct UCKeyStateEntryRange     UCKeyStateEntryRange;
/*
   -------------------------------------------------------------------------------------------------
   UCKeyboardLayout & related stuff
   The UCKeyboardLayout struct given here is only for the resource header. It specifies
   offsets to the various subtables which each have their own structs, given below.
   The keyboardTypeHeadList array selects table offsets that depend on keyboardType. The
   first entry in keyboardTypeHeadList is the default entry, which will be used if the
   keyboardType passed to UCKeyTranslate does not match any other entry - i.e. does not fall
   within the range keyboardTypeFirst..keyboardTypeLast for some entry. The first entry
   should have keyboardTypeFirst = keyboardTypeLast = 0.
   -------------------------------------------------------------------------------------------------
*/
struct UCKeyboardTypeHeader {
  UInt32              keyboardTypeFirst;      /* first keyboardType in this entry*/
  UInt32              keyboardTypeLast;       /* last keyboardType in this entry*/
  UInt32              keyModifiersToTableNumOffset; /* required*/
  UInt32              keyToCharTableIndexOffset; /* required*/
  UInt32              keyStateRecordsIndexOffset; /* 0 => no table*/
  UInt32              keyStateTerminatorsOffset; /* 0 => no table*/
  UInt32              keySequenceDataIndexOffset; /* 0 => no table*/
};
typedef struct UCKeyboardTypeHeader     UCKeyboardTypeHeader;
struct UCKeyboardLayout {
                                              /* header only; other tables accessed via offsets*/
  UInt16              keyLayoutHeaderFormat;  /* =kUCKeyLayoutHeaderFormat*/
  UInt16              keyLayoutDataVersion;   /* 0x0100 = 1.0, 0x0110 = 1.1, etc.*/
  UInt32              keyLayoutFeatureInfoOffset; /* may be 0                       */
  UInt32              keyboardTypeCount;      /* Dimension for keyboardTypeHeadList[]     */
  UCKeyboardTypeHeader  keyboardTypeList[1];
};
typedef struct UCKeyboardLayout         UCKeyboardLayout;
/* -------------------------------------------------------------------------------------------------*/
struct UCKeyLayoutFeatureInfo {
  UInt16              keyLayoutFeatureInfoFormat; /* =kUCKeyLayoutFeatureInfoFormat*/
  UInt16              reserved;
  UInt32              maxOutputStringLength;  /* longest possible output string*/
};
typedef struct UCKeyLayoutFeatureInfo   UCKeyLayoutFeatureInfo;
/* -------------------------------------------------------------------------------------------------*/
struct UCKeyModifiersToTableNum {
  UInt16              keyModifiersToTableNumFormat; /* =kUCKeyModifiersToTableNumFormat*/
  UInt16              defaultTableNum;        /* For modifier combos not in tableNum[]*/
  UInt32              modifiersCount;         /* Dimension for tableNum[]*/
  UInt8               tableNum[1];

                                              /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyModifiersToTableNum UCKeyModifiersToTableNum;
/* -------------------------------------------------------------------------------------------------*/
struct UCKeyToCharTableIndex {
  UInt16              keyToCharTableIndexFormat; /* =kUCKeyToCharTableIndexFormat*/
  UInt16              keyToCharTableSize;     /* Max keyCode (128 for ADB keyboards)*/
  UInt32              keyToCharTableCount;    /* Dimension for keyToCharTableOffsets[] (usually 6 to 12 tables)*/
  UInt32              keyToCharTableOffsets[1];

                                              /* Each offset in keyToCharTableOffsets is from the beginning of the resource to a*/
                                              /* table as follows:*/
                                              /*    UCKeyOutput       keyToCharData[keyToCharTableSize];*/
                                              /* These tables follow the UCKeyToCharTableIndex.*/
                                              /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyToCharTableIndex    UCKeyToCharTableIndex;
/* -------------------------------------------------------------------------------------------------*/
struct UCKeyStateRecordsIndex {
  UInt16              keyStateRecordsIndexFormat; /* =kUCKeyStateRecordsIndexFormat*/
  UInt16              keyStateRecordCount;    /* Dimension for keyStateRecordOffsets[]*/
  UInt32              keyStateRecordOffsets[1];

                                              /* Each offset in keyStateRecordOffsets is from the beginning of the resource to a*/
                                              /* UCKeyStateRecord. These UCKeyStateRecords follow the keyStateRecordOffsets[] array.*/
                                              /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyStateRecordsIndex   UCKeyStateRecordsIndex;
/* -------------------------------------------------------------------------------------------------*/
struct UCKeyStateTerminators {
  UInt16              keyStateTerminatorsFormat; /* =kUCKeyStateTerminatorsFormat*/
  UInt16              keyStateTerminatorCount; /* Dimension for keyStateTerminators[] (# of nonzero states)*/
  UCKeyCharSeq        keyStateTerminators[1];

                                              /* Note: keyStateTerminators[0] is terminator for state 1, etc.*/
                                              /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeyStateTerminators    UCKeyStateTerminators;
/* -------------------------------------------------------------------------------------------------*/
struct UCKeySequenceDataIndex {
  UInt16              keySequenceDataIndexFormat; /* =kUCKeySequenceDataIndexFormat*/
  UInt16              charSequenceCount;      /* Dimension of charSequenceOffsets[] is charSequenceCount+1*/
  UInt16              charSequenceOffsets[1];

                                              /* Each offset in charSequenceOffsets is in bytes, from the beginning of*/
                                              /* UCKeySequenceDataIndex to a sequence of UniChars; the next offset indicates the*/
                                              /* end of the sequence. The UniChar sequences follow the UCKeySequenceDataIndex.*/
                                              /* Then there is padding to a 4-byte boundary with bytes containing 0, if necessary.*/
};
typedef struct UCKeySequenceDataIndex   UCKeySequenceDataIndex;
/* -------------------------------------------------------------------------------------------------*/
/* Current format codes for the various tables (bits 12-15 indicate which table)*/

enum {
  kUCKeyLayoutHeaderFormat      = 0x1002,
  kUCKeyLayoutFeatureInfoFormat = 0x2001,
  kUCKeyModifiersToTableNumFormat = 0x3001,
  kUCKeyToCharTableIndexFormat  = 0x4001,
  kUCKeyStateRecordsIndexFormat = 0x5001,
  kUCKeyStateTerminatorsFormat  = 0x6001,
  kUCKeySequenceDataIndexFormat = 0x7001
};


/*
   -------------------------------------------------------------------------------------------------
   Constants for keyAction parameter in UCKeyTranslate() 
   -------------------------------------------------------------------------------------------------
*/

enum {
  kUCKeyActionDown              = 0,    /* key is going down*/
  kUCKeyActionUp                = 1,    /* key is going up*/
  kUCKeyActionAutoKey           = 2,    /* auto-key down*/
  kUCKeyActionDisplay           = 3     /* get information for key display (as in Key Caps)      */
};

/*
   -------------------------------------------------------------------------------------------------
   Bit assignments & masks for keyTranslateOptions parameter in UCKeyTranslate() 
   -------------------------------------------------------------------------------------------------
*/

enum {
  kUCKeyTranslateNoDeadKeysBit  = 0     /* Prevents setting any new dead-key states*/
};

enum {
  kUCKeyTranslateNoDeadKeysMask = 1L << kUCKeyTranslateNoDeadKeysBit
};

/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for Unicode Collation
   -------------------------------------------------------------------------------------------------
*/

/* constant for LocaleOperationClass*/
enum {
  kUnicodeCollationClass        = 'ucol'
};

typedef struct OpaqueCollatorRef*       CollatorRef;
typedef UInt32 UCCollateOptions;
enum {
                                        /* Sensitivity options*/
  kUCCollateComposeInsensitiveMask = 1L << 1,
  kUCCollateWidthInsensitiveMask = 1L << 2,
  kUCCollateCaseInsensitiveMask = 1L << 3,
  kUCCollateDiacritInsensitiveMask = 1L << 4, /* Other general options */
  kUCCollatePunctuationSignificantMask = 1L << 15, /* Number-handling options */
  kUCCollateDigitsOverrideMask  = 1L << 16,
  kUCCollateDigitsAsNumberMask  = 1L << 17
};

enum {
  kUCCollateStandardOptions     = kUCCollateComposeInsensitiveMask | kUCCollateWidthInsensitiveMask
};

/*
   Special values to specify various invariant orders for UCCompareTextNoLocale.
   These values use the high 8 bits of UCCollateOptions.
*/
enum {
  kUCCollateTypeHFSExtended     = 1
};

/* These constants are used for masking and shifting the invariant order type.*/
enum {
  kUCCollateTypeSourceMask      = 0x000000FF,
  kUCCollateTypeShiftBits       = 24
};

enum {
  kUCCollateTypeMask            = (UInt32)kUCCollateTypeSourceMask << kUCCollateTypeShiftBits
};


typedef UInt32                          UCCollationValue;
/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for Unicode TypeSelect
   -------------------------------------------------------------------------------------------------
*/

/*
   UCTypeSelectRef
   This is the single opaque object needed to implement the Unicode TypeSelect
   utilities. It is created and initialized via a call to UCTypeSelectCreateSelector
*/
typedef struct OpaqueUCTypeSelectRef*   UCTypeSelectRef;
/*
   UCTypeSelectCompareResult
   Used as the return value for UCTypeSelectCompare()
*/
typedef SInt32                          UCTypeSelectCompareResult;
/*
   UCTSWalkDirection
   Used for UCTypeSelectWalkList to determine the direction of the walk
*/
typedef UInt16 UCTSWalkDirection;
enum {
  kUCTSDirectionNext            = 0,
  kUCTSDirectionPrevious        = 1
};

/*
   UCTypeSelectOptions
   These constants may be returned from an IndexToUCString callback function
   in the location pointed to by the tsOptions parameter. *tsOptions is pre-
   initialized to zero before the callback function is called. A callback
   function does not need to set *tsOptions unless it wants behavior different
   from the default.
   kUCTSOptionsReleaseStringMask: indicates that UCTypeSelectFindItem should
   release the CFStringRef returned by the IndexToUCString callback function
   once it is done with the string. If this bit is not set, the string will
   not be released.
   kUCTSOptionsDataIsOrderedMask: indicates that the data being returned by the
   IndexToUCString callback is already in the correct alphabetical order. If so,
   UCTypeSelectFindItem can optimize its search through the data to find the closest
   matching item.
*/
typedef UInt16 UCTypeSelectOptions;
enum {
  kUCTSOptionsNoneMask          = 0,
  kUCTSOptionsReleaseStringMask = 1,
  kUCTSOptionsDataIsOrderedMask = 2
  // Additional option bits are defined for internal use
};


/*
   IndexToUCStringProcPtr
   This is the type used to define the user's IndexToUCString callback
*/
typedef CALLBACK_API_C( Boolean , IndexToUCStringProcPtr )(UInt32 index, void *listDataPtr, void *refcon, CFStringRef *outString, UCTypeSelectOptions *tsOptions);

typedef STACK_UPP_TYPE(IndexToUCStringProcPtr)                  IndexToUCStringUPP;
/*
 *  NewIndexToUCStringUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern IndexToUCStringUPP
NewIndexToUCStringUPP(IndexToUCStringProcPtr userRoutine)     __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);

/*
 *  DisposeIndexToUCStringUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeIndexToUCStringUPP(IndexToUCStringUPP userUPP)         __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);

/*
 *  InvokeIndexToUCStringUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeIndexToUCStringUPP(
  UInt32                 index,
  void *                 listDataPtr,
  void *                 refcon,
  CFStringRef *          outString,
  UCTypeSelectOptions *  tsOptions,
  IndexToUCStringUPP     userUPP)                             __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline IndexToUCStringUPP                                   NewIndexToUCStringUPP(IndexToUCStringProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeIndexToUCStringUPP(IndexToUCStringUPP) { }
    inline Boolean                                              InvokeIndexToUCStringUPP(UInt32 index, void * listDataPtr, void * refcon, CFStringRef * outString, UCTypeSelectOptions * tsOptions, IndexToUCStringUPP userUPP) { return (*userUPP)(index, listDataPtr, refcon, outString, tsOptions); }
  #else
    #define NewIndexToUCStringUPP(userRoutine)                  ((IndexToUCStringUPP)userRoutine)
    #define DisposeIndexToUCStringUPP(userUPP)
    #define InvokeIndexToUCStringUPP(index, listDataPtr, refcon, outString, tsOptions, userUPP) (*userUPP)(index, listDataPtr, refcon, outString, tsOptions)
  #endif
#endif

/*
   kUCTypeSelectMaxListSize can be used for any listSize arguement
   when the length of the list is unknown.
*/
enum {
  kUCTypeSelectMaxListSize      = (UInt32)0xFFFFFFFF
};


/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for Unicode TextBreak
   -------------------------------------------------------------------------------------------------
*/

/* constant for LocaleOperationClass*/
enum {
  kUnicodeTextBreakClass        = 'ubrk'
};

typedef struct OpaqueTextBreakLocatorRef*  TextBreakLocatorRef;

/*
 *  UCTextBreakType
 *  
 *  Discussion:
 *    Specifies kinds of text boundaries.
 */
typedef UInt32 UCTextBreakType;
enum {

  /*
   * If the bit specified by this mask is set, boundaries of characters
   * may be located (with surrogate pairs treated as a single
   * character).
   */
  kUCTextBreakCharMask          = 1L << 0,

  /*
   * If the bit specified by this mask is set, boundaries of character
   * clusters may be located. A cluster is a group of characters that
   * should be treated as single text element for editing operations
   * such as cursor movement. Typically this includes groups such as a
   * base character followed by a sequence of combining characters, for
   * example, a Hangul syllable represented as a sequence of conjoining
   * jamo characters or an Indic consonant cluster.
   */
  kUCTextBreakClusterMask       = 1L << 2,

  /*
   * If the bit specified by this mask is set, boundaries of words may
   * be located. This can be used to determine what to highlight as the
   * result of a double-click.
   */
  kUCTextBreakWordMask          = 1L << 4,
  kUCTextBreakLineMask          = 1L << 6,

  /*
   * If the bit specified by this mask is set, boundaries of paragraphs
   * may be located. This just finds the next hard-line break as
   * defined by the Unicode standard.
   */
  kUCTextBreakParagraphMask     = 1L << 8
};

typedef UInt32 UCTextBreakOptions;
enum {
  kUCTextBreakLeadingEdgeMask   = 1L << 0,
  kUCTextBreakGoBackwardsMask   = 1L << 1,
  kUCTextBreakIterateMask       = 1L << 2
};


/*
   -------------------------------------------------------------------------------------------------
   FUNCTION PROTOTYPES
   -------------------------------------------------------------------------------------------------
*/

/*
 *  UCKeyTranslate()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesCoreLib 8.5 and later
 */
extern OSStatus 
UCKeyTranslate(
  const UCKeyboardLayout *  keyLayoutPtr,
  UInt16                    virtualKeyCode,
  UInt16                    keyAction,
  UInt32                    modifierKeyState,
  UInt32                    keyboardType,
  OptionBits                keyTranslateOptions,
  UInt32 *                  deadKeyState,
  UniCharCount              maxStringLength,
  UniCharCount *            actualStringLength,
  UniChar                   unicodeString[])                  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Standard collation functions*/

/*
 *  UCCreateCollator()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 8.6 and later
 */
extern OSStatus 
UCCreateCollator(
  LocaleRef                locale,
  LocaleOperationVariant   opVariant,
  UCCollateOptions         options,
  CollatorRef *            collatorRef)                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  UCGetCollationKey()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 8.6 and later
 */
extern OSStatus 
UCGetCollationKey(
  CollatorRef        collatorRef,
  const UniChar *    textPtr,
  UniCharCount       textLength,
  ItemCount          maxKeySize,
  ItemCount *        actualKeySize,
  UCCollationValue   collationKey[])                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  UCCompareCollationKeys()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesCoreLib 8.6 and later
 */
extern OSStatus 
UCCompareCollationKeys(
  const UCCollationValue *  key1Ptr,
  ItemCount                 key1Length,
  const UCCollationValue *  key2Ptr,
  ItemCount                 key2Length,
  Boolean *                 equivalent,
  SInt32 *                  order)                            __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  UCCompareText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 8.6 and later
 */
extern OSStatus 
UCCompareText(
  CollatorRef      collatorRef,
  const UniChar *  text1Ptr,
  UniCharCount     text1Length,
  const UniChar *  text2Ptr,
  UniCharCount     text2Length,
  Boolean *        equivalent,
  SInt32 *         order)                                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  UCDisposeCollator()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 8.6 and later
 */
extern OSStatus 
UCDisposeCollator(CollatorRef * collatorRef)                  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Simple collation using default locale*/

/*
 *  UCCompareTextDefault()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 8.6 and later
 */
extern OSStatus 
UCCompareTextDefault(
  UCCollateOptions   options,
  const UniChar *    text1Ptr,
  UniCharCount       text1Length,
  const UniChar *    text2Ptr,
  UniCharCount       text2Length,
  Boolean *          equivalent,
  SInt32 *           order)                                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/* Simple locale-independent collation*/

/*
 *  UCCompareTextNoLocale()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesCoreLib 8.6 and later
 */
extern OSStatus 
UCCompareTextNoLocale(
  UCCollateOptions   options,
  const UniChar *    text1Ptr,
  UniCharCount       text1Length,
  const UniChar *    text2Ptr,
  UniCharCount       text2Length,
  Boolean *          equivalent,
  SInt32 *           order)                                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/*
*===============================================================================
*   Text break (text boundary) functions
*
*   These are deprecated. Replacements are as follows:
*
*   1. To determine locale-sensitive text breaks for word, line, sentence and
*   paragraph boundaries, use the CFStringTokenizer functions:
*   CFStringTokenizerCreate (balanced by CFRelease), CFStringTokenizerAdvanceToNextToken
*   or CFStringTokenizerGoToTokenAtIndex, then CFStringTokenizerGetCurrentTokenRange...
*
*   2. To determine cluster breaks, use CFStringGetRangeOfComposedCharactersAtIndex.
*
*   3. For handling character boundaries / surrogate pairs in UTF16 text, the
*   following inline functions are available in CFString.h:
*   CFStringIsSurrogateHighCharacter, CFStringIsSurrogateLowCharacter,
*   CFStringGetLongCharacterForSurrogatePair, and CFStringGetSurrogatePairForLongCharacter.
*   However, CFString clients do not usually need to worry about handling surrogate pairs
*   directly.
*
*===============================================================================
*/
/*
 *  UCCreateTextBreakLocator()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CFStringTokenizer functions or
 *    CFStringGetRangeOfComposedCharactersAtIndex, see discussion above
 *    for details.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 9.0 and later
 */
extern OSStatus 
UCCreateTextBreakLocator(
  LocaleRef                locale,
  LocaleOperationVariant   opVariant,
  UCTextBreakType          breakTypes,
  TextBreakLocatorRef *    breakRef)                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_NA, __IPHONE_NA);


/*
 *  UCFindTextBreak()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CFStringTokenizer functions or
 *    CFStringGetRangeOfComposedCharactersAtIndex, see discussion above
 *    for details.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 9.0 and later
 */
extern OSStatus 
UCFindTextBreak(
  TextBreakLocatorRef   breakRef,
  UCTextBreakType       breakType,
  UCTextBreakOptions    options,
  const UniChar *       textPtr,
  UniCharCount          textLength,
  UniCharArrayOffset    startOffset,
  UniCharArrayOffset *  breakOffset)                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_NA, __IPHONE_NA);


/*
 *  UCDisposeTextBreakLocator()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CFStringTokenizer functions or
 *    CFStringGetRangeOfComposedCharactersAtIndex, see discussion above
 *    for details.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeUtilitiesLib 9.0 and later
 */
extern OSStatus 
UCDisposeTextBreakLocator(TextBreakLocatorRef * breakRef)     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_NA, __IPHONE_NA);




/*
   -------------------------------------------------------------------------------------------------
   UNICODE TYPESELECT - FUNCTION APIs
   -------------------------------------------------------------------------------------------------
*/

/*
 *  UCTypeSelectCreateSelector()
 *  
 *  Summary:
 *    Responsible for creating the opaque UCTypeSelectRef object.
 *  
 *  Parameters:
 *    
 *    locale:
 *      LocaleRef obtained by client from a call such as
 *      LocaleRefFromLangOrRegionCode. This can be set to NULL if the
 *      default system locale is desired.
 *    
 *    opVariant:
 *      Variant of the locale. Specify 0 if no variant is needed.
 *    
 *    options:
 *      Any collation options the client wishes to specify. These will
 *      have an impact on the order in which selection will occur.
 *      Specify kUCCollateStandardOptions for the default options.
 *    
 *    newSelector:
 *      The newly created UCTypeSelectRef object.
 *  
 *  Result:
 *    Will return paramErr if newSelector is NULL, or any other error
 *    that may be returned by an internal function call.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCTypeSelectCreateSelector(
  LocaleRef                locale,            /* can be NULL */
  LocaleOperationVariant   opVariant,
  UCCollateOptions         options,
  UCTypeSelectRef *        newSelector)                       __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



/*
 *  UCTypeSelectFlushSelectorData()
 *  
 *  Summary:
 *    Flushes the key list and resets the timeout timer for the
 *    UCTypeSelectRef.
 *  
 *  Parameters:
 *    
 *    ref:
 *      UCTypeSelectRef to be flushed.
 *  
 *  Result:
 *    Returns paramErr if ref is invalid.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCTypeSelectFlushSelectorData(UCTypeSelectRef ref)            __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



/*
 *  UCTypeSelectReleaseSelector()
 *  
 *  Summary:
 *    Cleans up and disposes of any temporary memory acquired by the
 *    UCTypeSelectRef object.
 *  
 *  Parameters:
 *    
 *    ref:
 *      A pointer to the UCTypeSelectRef to be disposed of. On exit,
 *      the UCTypeSelectRef to which this parameter points will be set
 *      to NULL.
 *  
 *  Result:
 *    Returns paramErr if ref is invalid.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCTypeSelectReleaseSelector(UCTypeSelectRef * ref)            __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



/*
 *  UCTypeSelectWouldResetBuffer()
 *  
 *  Summary:
 *    Indicates whether, if the specified text were added to the
 *    buffer, the current text in the buffer would be discarded.
 *  
 *  Parameters:
 *    
 *    inRef:
 *      The type-selection object.
 *    
 *    inText:
 *      The text that would be added to the buffer. Some text (such as
 *      Backspace, Enter, and Clear keys) always causes the buffer to
 *      be reset. May be NULL; in that case, the implementation only
 *      considers the event time.
 *    
 *    inEventTime:
 *      The time in seconds since boot (as returned by
 *      GetCurrentEventTime) that the text event occurred. If the event
 *      occurred at a time greater than the type-select timeout, then
 *      the current buffered text would be discarded.
 *  
 *  Result:
 *    Whether the current text in the buffer would be discarded while
 *    processing the specified text.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
UCTypeSelectWouldResetBuffer(
  UCTypeSelectRef   inRef,
  CFStringRef       inText,            /* can be NULL */
  double            inEventTime)                              __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



/*
 *  UCTypeSelectAddKeyToSelector()
 *  
 *  Summary:
 *    Appends the given Unicode values to the selector's internal
 *    buffer of keystrokes. It also handles timeouts and delete/clear
 *    keys. If the key sent is a delete/clear/esc code, the key buffer
 *    will be flushed and false will be returned.
 *  
 *  Parameters:
 *    
 *    inRef:
 *      The type-selection object.
 *    
 *    inText:
 *      A CFString that contains the keystroke to be added.
 *    
 *    inEventTime:
 *      The time in seconds since boot (as returned by
 *      GetCurrentEventTime) that the text event occurred.  If zero is
 *      passed, then the current time is used automatically.
 *    
 *    updateFlag:
 *      On exit, notifies the client if it needs to update its current
 *      selection, as follows: 
 *      
 *      TRUE - indicates that the client needs to update its selection
 *      based on the keystroke passed in. A call to UCTypeSelectCompare
 *      or UCTypeSelectFindItem should be made to find the new item to
 *      select based on the new keys added. 
 *      
 *      FALSE - indicates that the client does not need to update its
 *      selection. This would occur if a delete/clear/esc key was
 *      passed in.
 *  
 *  Result:
 *    returns paramErr if ref or textPtr are invalid. Can also return
 *    other errors from intermediate function calls.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCTypeSelectAddKeyToSelector(
  UCTypeSelectRef   inRef,
  CFStringRef       inText,
  double            inEventTime,
  Boolean *         updateFlag)                               __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



/*
 *  UCTypeSelectCompare()
 *  
 *  Summary:
 *    Compares the Unicode text buffer passed in inText with the
 *    keystroke buffer stored in the UCTypeSelectRef. This function
 *    works the same as the non-Unicode TypeSelectCompare() call.
 *  
 *  Parameters:
 *    
 *    ref:
 *      UCTypeSelectRef to which the Unicode text sent in inText will
 *      be compared.
 *    
 *    inText:
 *      A reference to the text to be compared
 *    
 *    result:
 *      Just as in TypeSelectCompare(), the following values are 
 *      returned: -1 if characters in UCTypeSelectRefÕs keystroke
 *      buffer sort before those in inText, 0 if characters in
 *      UCTypeSelectRefÕs keystroke buffer are the same as those in
 *      inText, and 1 if the characters in UCTypeSelectRefÕs keystroke
 *      buffer sort after those in inText.
 *  
 *  Result:
 *    This function can return three different types of values. First,
 *    it will return paramErr if ref, inText, or result are invalid.
 *    Second, if there have been no keys added to the UCTypeSelectRef
 *    via calls to UCTypeSelectAddKeyToSelectorData(),
 *    kUCTSNoKeysAddedToObjectErr will be returned. Finally, it can
 *    also return other errors should any be encountered by
 *    intermediate function calls.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCTypeSelectCompare(
  UCTypeSelectRef              ref,
  CFStringRef                  inText,
  UCTypeSelectCompareResult *  result)                        __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



/*
 *  UCTypeSelectFindItem()
 *  
 *  Summary:
 *    In a given list, pointed to by listDataPtr, find the closest
 *    match to the keystrokes stored in the UCTypeSelectRef. The
 *    closest matchÕs index will be returned in closestItem. The list
 *    to be searched may be ordered or unordered. In order for this
 *    call to work, the client needs to provide a an IndexToUCString
 *    callback UPP. This callback is necessary in order to provide the
 *    client with data structure independence through client-side
 *    indexing.
 *  
 *  Parameters:
 *    
 *    ref:
 *      UCTypeSelectRef holding the state and keystrokes to be compared.
 *    
 *    listSize:
 *      Size of the list to be searched through. If the size of the
 *      list is unknown, pass in kUCTypeSelectMaxListSize (0xFFFFFFFF)
 *      and have the IndexToUCString function return false after it has
 *      reached the last item in the list.
 *    
 *    listDataPtr:
 *      Pointer to the head or first node of the clientÕs data
 *      structure. This will be passed into to the clientÕs
 *      IndexToUCString function. Can be NULL, depending on the
 *      clientÕs IndexToUCString implementation.
 *    
 *    refcon:
 *      Any parameter the calling function wishes to pass as a
 *      reference into its IndexToUCString callback function. This
 *      parameter can be set to NULL if not needed.
 *    
 *    userUPP:
 *      The UPP pointing to the clientÕs IndexToUCString callback
 *      function.
 *    
 *    closestItem:
 *      Upon return, this will contain the index of the item that
 *      matches the text in the keystroke buffer of UCTypeSelectRef.
 *  
 *  Result:
 *    This function has four possibilities for return values. First,
 *    paramErr will be returned if ref or closestItem are invalid.
 *    Second, if the search list is empty or if the first item cannot
 *    be read, kUCTSSearchListErr will be returned. Third, if there
 *    have been no keys added to the UCTypeSelectRef via calls to
 *    UCTypeSelectAddKeyToSelectorData(), kUCTSNoKeysAddedToObjectErr
 *    will be returned. Finally, this function can return other OS
 *    errors should any be encountered by an internal function call.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCTypeSelectFindItem(
  UCTypeSelectRef      ref,
  UInt32               listSize,
  void *               listDataPtr,       /* can be NULL */
  void *               refcon,            /* can be NULL */
  IndexToUCStringUPP   userUPP,
  UInt32 *             closestItem)                           __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



/*
 *  UCTypeSelectWalkList()
 *  
 *  Summary:
 *    UCTypeSelectWalkList can perform an in-order alphabetical walk of
 *    an unsorted list. To do this, the client passes a pointer to the
 *    current selectionÕs text in the currSelect parameter.
 *    UCTypeSelectWalkList will then search the list pointed to by
 *    listDataPtr for the closest item that is lexicographically either
 *    right before or right after the selected item. The client can
 *    choose which one to be returned by setting the direction
 *    parameter to kUCTSDirectionPrevious or kUCTSDirectionNext to get
 *    the item either lexicographically right before or after
 *    respectively. This call will not walk off the end of the list or
 *    do any wraparound searching. That is, if the item selected is the
 *    last item in the list and kUCDirectionNext is specified for the
 *    direction, that same item will be returned. Likewise for the case
 *    where the first item is selected nd kUCTSDirectionPrevious is
 *    specified. In order for this call to work, the client needs to
 *    provide an IndexToUCString callback UPP. This callback is
 *    necessary in order to provide the client with data structure
 *    independence through client-side indexing.
 *  
 *  Parameters:
 *    
 *    ref:
 *      UCTypeSelectRef holding state information as well as the
 *      function pointer needed to call the clientÕs IndexToUCString
 *      function
 *    
 *    currSelect:
 *      CFString reference to the current selectionÕs text.
 *    
 *    direction:
 *      The direction of the walk. The valid values for this parameter
 *      are: 
 *      
 *      kUCTSDirectionNext  - find the next item in the list
 *       kUCTSDirectionPrevious - find the previous item in the list
 *      
 *      
 *      If kUCTSDirectionNext is specified and the selected item is the
 *      last item in the list or if kUCTSDirectionPrevious is specified
 *      and the selected item is the first item in the list, the index
 *      of the selected item will be returned in closestItem.
 *    
 *    listSize:
 *      Size of the list to be searched through. If the size of the
 *      list is unknown, pass in kUCTypeSelectMaxListSize (0xFFFFFFFF)
 *      and have the IndexToUCString function return false after it has
 *      reached the last item in the list.
 *    
 *    listDataPtr:
 *      Pointer to the head or first node of the clientÕs data
 *      structure. This will be passed into to the clientÕs
 *      IndexToUCString function. Can be NULL, depending on the
 *      clientÕs IndexToUCString implementation.
 *    
 *    refcon:
 *      Any parameter the calling function wishes to pass as a
 *      reference into its IndexToUCString callback function. This
 *      parameter can be set to NULL if not needed.
 *    
 *    userUPP:
 *      The UPP pointing to the clientÕs IndexToUCString callback
 *      function.
 *    
 *    closestItem:
 *      Upon return, this will contain the index of the item that
 *      matches the text in the keystroke buffer of UCTypeSelectRef.
 *  
 *  Result:
 *    This function has three possibilities for return values. First,
 *    paramErr will be returned if ref, currSelect, or closestItem are
 *    invalid. Second, if the search list is empty or if the first item
 *    cannot be read, kUCTSSearchListErr  will be returned. Finally,
 *    this function can return other OS errors should any be
 *    encountered by an internal function call.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCTypeSelectWalkList(
  UCTypeSelectRef      ref,
  CFStringRef          currSelect,
  UCTSWalkDirection    direction,
  UInt32               listSize,
  void *               listDataPtr,       /* can be NULL */
  void *               refcon,            /* can be NULL */
  IndexToUCStringUPP   userUPP,
  UInt32 *             closestItem)                           __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __UNICODEUTILITIES__ */

