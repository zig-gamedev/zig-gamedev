/*
     File:       CarbonCore/UnicodeConverter.h
 
     Contains:   Types, constants, and prototypes for Unicode Converter
 
     Copyright:  © 1994-2011 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __UNICODECONVERTER__
#define __UNICODECONVERTER__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __TEXTCOMMON__
#include <CarbonCore/TextCommon.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/* Unicode conversion contexts: */
typedef struct OpaqueTextToUnicodeInfo*  TextToUnicodeInfo;
typedef struct OpaqueUnicodeToTextInfo*  UnicodeToTextInfo;
typedef struct OpaqueUnicodeToTextRunInfo*  UnicodeToTextRunInfo;
typedef const TextToUnicodeInfo         ConstTextToUnicodeInfo;
typedef const UnicodeToTextInfo         ConstUnicodeToTextInfo;
/* UnicodeMapVersion type & values */
typedef SInt32                          UnicodeMapVersion;
enum {
  kUnicodeUseLatestMapping      = -1,
  kUnicodeUseHFSPlusMapping     = 4
};

/* Types used in conversion */
struct UnicodeMapping {
  TextEncoding        unicodeEncoding;
  TextEncoding        otherEncoding;
  UnicodeMapVersion   mappingVersion;
};
typedef struct UnicodeMapping           UnicodeMapping;
typedef UnicodeMapping *                UnicodeMappingPtr;
typedef const UnicodeMapping *          ConstUnicodeMappingPtr;
/* Control flags for ConvertFromUnicodeToText and ConvertFromTextToUnicode */
enum {
  kUnicodeUseFallbacksBit       = 0,
  kUnicodeKeepInfoBit           = 1,
  kUnicodeDirectionalityBits    = 2,
  kUnicodeVerticalFormBit       = 4,
  kUnicodeLooseMappingsBit      = 5,
  kUnicodeStringUnterminatedBit = 6,
  kUnicodeTextRunBit            = 7,
  kUnicodeKeepSameEncodingBit   = 8,
  kUnicodeForceASCIIRangeBit    = 9,
  kUnicodeNoHalfwidthCharsBit   = 10,
  kUnicodeTextRunHeuristicsBit  = 11,
  kUnicodeMapLineFeedToReturnBit = 12,  /*    if kUnicodeUseExternalEncodingFormBit is not set, */
                                        /*    input/output UTF-16 (and UTF-32) is assumed to be in native endian. */
                                        /*    if kUnicodeUseExternalEncodingFormBit is set, */
                                        /*    input UTF-16 (and UTF-32) is assumed to be in big endian */
                                        /*    unless it begins with a byte-order-mark, */
                                        /*    and output UTF-16 (and UTF-32) will be in big endian. */
  kUnicodeUseExternalEncodingFormBit = 13 /*    Bits 16-17 are defined in TextEncodingConverter.h for TECSetBasicOptions */
};

enum {
  kUnicodeUseFallbacksMask      = 1L << kUnicodeUseFallbacksBit,
  kUnicodeKeepInfoMask          = 1L << kUnicodeKeepInfoBit,
  kUnicodeDirectionalityMask    = 3L << kUnicodeDirectionalityBits,
  kUnicodeVerticalFormMask      = 1L << kUnicodeVerticalFormBit,
  kUnicodeLooseMappingsMask     = 1L << kUnicodeLooseMappingsBit,
  kUnicodeStringUnterminatedMask = 1L << kUnicodeStringUnterminatedBit,
  kUnicodeTextRunMask           = 1L << kUnicodeTextRunBit,
  kUnicodeKeepSameEncodingMask  = 1L << kUnicodeKeepSameEncodingBit,
  kUnicodeForceASCIIRangeMask   = 1L << kUnicodeForceASCIIRangeBit,
  kUnicodeNoHalfwidthCharsMask  = 1L << kUnicodeNoHalfwidthCharsBit,
  kUnicodeTextRunHeuristicsMask = 1L << kUnicodeTextRunHeuristicsBit,
  kUnicodeMapLineFeedToReturnMask = 1L << kUnicodeMapLineFeedToReturnBit, /*    if kUnicodeUseExternalEncodingFormBit is not set, */
                                        /*    input/output UTF-16 (and UTF-32) is assumed to be in native endian. */
                                        /*    if kUnicodeUseExternalEncodingFormBit is set, */
                                        /*    input UTF-16 (and UTF-32) is assumed to be in big endian */
                                        /*    unless it begins with a byte-order-mark, */
                                        /*    and output UTF-16 (and UTF-32) will be in big endian. */
  kUnicodeUseExternalEncodingFormMask = 1L << kUnicodeUseExternalEncodingFormBit
};

/* Values for kUnicodeDirectionality field */
enum {
  kUnicodeDefaultDirection      = 0,
  kUnicodeLeftToRight           = 1,
  kUnicodeRightToLeft           = 2
};

/* Directionality masks for control flags */
enum {
  kUnicodeDefaultDirectionMask  = kUnicodeDefaultDirection << kUnicodeDirectionalityBits,
  kUnicodeLeftToRightMask       = kUnicodeLeftToRight << kUnicodeDirectionalityBits,
  kUnicodeRightToLeftMask       = kUnicodeRightToLeft << kUnicodeDirectionalityBits
};


/* Control flags for TruncateForUnicodeToText: */
/*
   Now TruncateForUnicodeToText uses control flags from the same set as used by
   ConvertFromTextToUnicode, ConvertFromUnicodeToText, etc., but only
   kUnicodeStringUnterminatedMask is meaningful for TruncateForUnicodeToText.
   
   Previously two special control flags were defined for TruncateForUnicodeToText:
        kUnicodeTextElementSafeBit = 0
        kUnicodeRestartSafeBit = 1
   However, neither of these was implemented.
   Instead of implementing kUnicodeTextElementSafeBit, we now use
   kUnicodeStringUnterminatedMask since it accomplishes the same thing and avoids
   having special flags just for TruncateForUnicodeToText
   Also, kUnicodeRestartSafeBit is unnecessary, since restart-safeness is handled by
   setting kUnicodeKeepInfoBit with ConvertFromUnicodeToText.
   If TruncateForUnicodeToText is called with one or both of the old special control
   flags set (bits 0 or 1), it will not generate a paramErr, but the old bits have no
   effect on its operation.
*/

/* Filter bits for filter field in QueryUnicodeMappings and CountUnicodeMappings: */
enum {
  kUnicodeMatchUnicodeBaseBit   = 0,
  kUnicodeMatchUnicodeVariantBit = 1,
  kUnicodeMatchUnicodeFormatBit = 2,
  kUnicodeMatchOtherBaseBit     = 3,
  kUnicodeMatchOtherVariantBit  = 4,
  kUnicodeMatchOtherFormatBit   = 5
};

enum {
  kUnicodeMatchUnicodeBaseMask  = 1L << kUnicodeMatchUnicodeBaseBit,
  kUnicodeMatchUnicodeVariantMask = 1L << kUnicodeMatchUnicodeVariantBit,
  kUnicodeMatchUnicodeFormatMask = 1L << kUnicodeMatchUnicodeFormatBit,
  kUnicodeMatchOtherBaseMask    = 1L << kUnicodeMatchOtherBaseBit,
  kUnicodeMatchOtherVariantMask = 1L << kUnicodeMatchOtherVariantBit,
  kUnicodeMatchOtherFormatMask  = 1L << kUnicodeMatchOtherFormatBit
};

/* Control flags for SetFallbackUnicodeToText */
enum {
  kUnicodeFallbackSequencingBits = 0
};

enum {
  kUnicodeFallbackSequencingMask = 3L << kUnicodeFallbackSequencingBits,
  kUnicodeFallbackInterruptSafeMask = 1L << 2 /* To indicate that caller fallback routine doesnÕt move memory*/
};

/* values for kUnicodeFallbackSequencing field */
enum {
  kUnicodeFallbackDefaultOnly   = 0,
  kUnicodeFallbackCustomOnly    = 1,
  kUnicodeFallbackDefaultFirst  = 2,
  kUnicodeFallbackCustomFirst   = 3
};


/* Caller-supplied entry point to a fallback handler */
typedef CALLBACK_API( OSStatus , UnicodeToTextFallbackProcPtr )(UniChar *iSrcUniStr, ByteCount iSrcUniStrLen, ByteCount *oSrcConvLen, TextPtr oDestStr, ByteCount iDestStrLen, ByteCount *oDestConvLen, LogicalAddress iInfoPtr, ConstUnicodeMappingPtr iUnicodeMappingPtr);
typedef STACK_UPP_TYPE(UnicodeToTextFallbackProcPtr)            UnicodeToTextFallbackUPP;
/*
 *  NewUnicodeToTextFallbackUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern UnicodeToTextFallbackUPP
NewUnicodeToTextFallbackUPP(UnicodeToTextFallbackProcPtr userRoutine) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  DisposeUnicodeToTextFallbackUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeUnicodeToTextFallbackUPP(UnicodeToTextFallbackUPP userUPP) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  InvokeUnicodeToTextFallbackUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeUnicodeToTextFallbackUPP(
  UniChar *                 iSrcUniStr,
  ByteCount                 iSrcUniStrLen,
  ByteCount *               oSrcConvLen,
  TextPtr                   oDestStr,
  ByteCount                 iDestStrLen,
  ByteCount *               oDestConvLen,
  LogicalAddress            iInfoPtr,
  ConstUnicodeMappingPtr    iUnicodeMappingPtr,
  UnicodeToTextFallbackUPP  userUPP)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline UnicodeToTextFallbackUPP                             NewUnicodeToTextFallbackUPP(UnicodeToTextFallbackProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeUnicodeToTextFallbackUPP(UnicodeToTextFallbackUPP) { }
    inline OSStatus                                             InvokeUnicodeToTextFallbackUPP(UniChar * iSrcUniStr, ByteCount iSrcUniStrLen, ByteCount * oSrcConvLen, TextPtr oDestStr, ByteCount iDestStrLen, ByteCount * oDestConvLen, LogicalAddress iInfoPtr, ConstUnicodeMappingPtr iUnicodeMappingPtr, UnicodeToTextFallbackUPP userUPP) { return (*userUPP)(iSrcUniStr, iSrcUniStrLen, oSrcConvLen, oDestStr, iDestStrLen, oDestConvLen, iInfoPtr, iUnicodeMappingPtr); }
  #else
    #define NewUnicodeToTextFallbackUPP(userRoutine)            ((UnicodeToTextFallbackUPP)userRoutine)
    #define DisposeUnicodeToTextFallbackUPP(userUPP)
    #define InvokeUnicodeToTextFallbackUPP(iSrcUniStr, iSrcUniStrLen, oSrcConvLen, oDestStr, iDestStrLen, oDestConvLen, iInfoPtr, iUnicodeMappingPtr, userUPP) (*userUPP)(iSrcUniStr, iSrcUniStrLen, oSrcConvLen, oDestStr, iDestStrLen, oDestConvLen, iInfoPtr, iUnicodeMappingPtr)
  #endif
#endif

/* Function prototypes */
/*
 *  CreateTextToUnicodeInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CreateTextToUnicodeInfo(
  ConstUnicodeMappingPtr   iUnicodeMapping,
  TextToUnicodeInfo *      oTextToUnicodeInfo)                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  CreateTextToUnicodeInfoByEncoding()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CreateTextToUnicodeInfoByEncoding(
  TextEncoding         iEncoding,
  TextToUnicodeInfo *  oTextToUnicodeInfo)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  CreateUnicodeToTextInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CreateUnicodeToTextInfo(
  ConstUnicodeMappingPtr   iUnicodeMapping,
  UnicodeToTextInfo *      oUnicodeToTextInfo)                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  CreateUnicodeToTextInfoByEncoding()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CreateUnicodeToTextInfoByEncoding(
  TextEncoding         iEncoding,
  UnicodeToTextInfo *  oUnicodeToTextInfo)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  CreateUnicodeToTextRunInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CreateUnicodeToTextRunInfo(
  ItemCount               iNumberOfMappings,
  const UnicodeMapping    iUnicodeMappings[],
  UnicodeToTextRunInfo *  oUnicodeToTextInfo)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  CreateUnicodeToTextRunInfoByEncoding()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CreateUnicodeToTextRunInfoByEncoding(
  ItemCount               iNumberOfEncodings,
  const TextEncoding      iEncodings[],
  UnicodeToTextRunInfo *  oUnicodeToTextInfo)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  CreateUnicodeToTextRunInfoByScriptCode()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CreateUnicodeToTextRunInfoByScriptCode(
  ItemCount               iNumberOfScriptCodes,
  const ScriptCode        iScripts[],
  UnicodeToTextRunInfo *  oUnicodeToTextInfo)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Change the TextToUnicodeInfo to another mapping. */
/*
 *  ChangeTextToUnicodeInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ChangeTextToUnicodeInfo(
  TextToUnicodeInfo        ioTextToUnicodeInfo,
  ConstUnicodeMappingPtr   iUnicodeMapping)                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Change the UnicodeToTextInfo to another mapping. */
/*
 *  ChangeUnicodeToTextInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ChangeUnicodeToTextInfo(
  UnicodeToTextInfo        ioUnicodeToTextInfo,
  ConstUnicodeMappingPtr   iUnicodeMapping)                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/*
 *  DisposeTextToUnicodeInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
DisposeTextToUnicodeInfo(TextToUnicodeInfo * ioTextToUnicodeInfo) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  DisposeUnicodeToTextInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
DisposeUnicodeToTextInfo(UnicodeToTextInfo * ioUnicodeToTextInfo) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  DisposeUnicodeToTextRunInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
DisposeUnicodeToTextRunInfo(UnicodeToTextRunInfo * ioUnicodeToTextRunInfo) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ConvertFromTextToUnicode()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ConvertFromTextToUnicode(
  TextToUnicodeInfo     iTextToUnicodeInfo,
  ByteCount             iSourceLen,
  ConstLogicalAddress   iSourceStr,
  OptionBits            iControlFlags,
  ItemCount             iOffsetCount,
  const ByteOffset      iOffsetArray[],           /* can be NULL */
  ItemCount *           oOffsetCount,             /* can be NULL */
  ByteOffset            oOffsetArray[],           /* can be NULL */
  ByteCount             iOutputBufLen,
  ByteCount *           oSourceRead,
  ByteCount *           oUnicodeLen,
  UniChar               oUnicodeStr[])                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ConvertFromUnicodeToText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ConvertFromUnicodeToText(
  UnicodeToTextInfo   iUnicodeToTextInfo,
  ByteCount           iUnicodeLen,
  const UniChar       iUnicodeStr[],
  OptionBits          iControlFlags,
  ItemCount           iOffsetCount,
  const ByteOffset    iOffsetArray[],           /* can be NULL */
  ItemCount *         oOffsetCount,             /* can be NULL */
  ByteOffset          oOffsetArray[],           /* can be NULL */
  ByteCount           iOutputBufLen,
  ByteCount *         oInputRead,
  ByteCount *         oOutputLen,
  LogicalAddress      oOutputStr)                             __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ConvertFromUnicodeToTextRun()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ConvertFromUnicodeToTextRun(
  UnicodeToTextRunInfo   iUnicodeToTextInfo,
  ByteCount              iUnicodeLen,
  const UniChar          iUnicodeStr[],
  OptionBits             iControlFlags,
  ItemCount              iOffsetCount,
  const ByteOffset       iOffsetArray[],           /* can be NULL */
  ItemCount *            oOffsetCount,             /* can be NULL */
  ByteOffset             oOffsetArray[],           /* can be NULL */
  ByteCount              iOutputBufLen,
  ByteCount *            oInputRead,
  ByteCount *            oOutputLen,
  LogicalAddress         oOutputStr,
  ItemCount              iEncodingRunBufLen,
  ItemCount *            oEncodingRunOutLen,
  TextEncodingRun        oEncodingRuns[])                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ConvertFromUnicodeToScriptCodeRun()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ConvertFromUnicodeToScriptCodeRun(
  UnicodeToTextRunInfo   iUnicodeToTextInfo,
  ByteCount              iUnicodeLen,
  const UniChar          iUnicodeStr[],
  OptionBits             iControlFlags,
  ItemCount              iOffsetCount,
  const ByteOffset       iOffsetArray[],           /* can be NULL */
  ItemCount *            oOffsetCount,             /* can be NULL */
  ByteOffset             oOffsetArray[],           /* can be NULL */
  ByteCount              iOutputBufLen,
  ByteCount *            oInputRead,
  ByteCount *            oOutputLen,
  LogicalAddress         oOutputStr,
  ItemCount              iScriptRunBufLen,
  ItemCount *            oScriptRunOutLen,
  ScriptCodeRun          oScriptCodeRuns[])                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Truncate a multibyte string at a safe place. */
/*
 *  TruncateForTextToUnicode()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
TruncateForTextToUnicode(
  ConstTextToUnicodeInfo   iTextToUnicodeInfo,
  ByteCount                iSourceLen,
  ConstLogicalAddress      iSourceStr,
  ByteCount                iMaxLen,
  ByteCount *              oTruncatedLen)                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Truncate a Unicode string at a safe place. */
/*
 *  TruncateForUnicodeToText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
TruncateForUnicodeToText(
  ConstUnicodeToTextInfo   iUnicodeToTextInfo,
  ByteCount                iSourceLen,
  const UniChar            iSourceStr[],
  OptionBits               iControlFlags,
  ByteCount                iMaxLen,
  ByteCount *              oTruncatedLen)                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Convert a Pascal string to Unicode string. */
/*
 *  ConvertFromPStringToUnicode()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ConvertFromPStringToUnicode(
  TextToUnicodeInfo   iTextToUnicodeInfo,
  ConstStr255Param    iPascalStr,
  ByteCount           iOutputBufLen,
  ByteCount *         oUnicodeLen,
  UniChar             oUnicodeStr[])                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Convert a Unicode string to Pascal string. */
/*
 *  ConvertFromUnicodeToPString()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ConvertFromUnicodeToPString(
  UnicodeToTextInfo   iUnicodeToTextInfo,
  ByteCount           iUnicodeLen,
  const UniChar       iUnicodeStr[],
  Str255              oPascalStr)                             __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Count the available conversion mappings. */
/*
 *  CountUnicodeMappings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
CountUnicodeMappings(
  OptionBits               iFilter,
  ConstUnicodeMappingPtr   iFindMapping,
  ItemCount *              oActualCount)                      __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Get a list of the available conversion mappings. */
/*
 *  QueryUnicodeMappings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
QueryUnicodeMappings(
  OptionBits               iFilter,
  ConstUnicodeMappingPtr   iFindMapping,
  ItemCount                iMaxCount,
  ItemCount *              oActualCount,
  UnicodeMapping           oReturnedMappings[])               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Setup the fallback handler for converting Unicode To Text. */
/*
 *  SetFallbackUnicodeToText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
SetFallbackUnicodeToText(
  UnicodeToTextInfo          iUnicodeToTextInfo,
  UnicodeToTextFallbackUPP   iFallback,
  OptionBits                 iControlFlags,
  LogicalAddress             iInfoPtr)                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Setup the fallback handler for converting Unicode To TextRuns. */
/*
 *  SetFallbackUnicodeToTextRun()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
SetFallbackUnicodeToTextRun(
  UnicodeToTextRunInfo       iUnicodeToTextRunInfo,
  UnicodeToTextFallbackUPP   iFallback,
  OptionBits                 iControlFlags,
  LogicalAddress             iInfoPtr)                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Re-initialize all state information kept by the context objects. */
/*
 *  ResetTextToUnicodeInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.3 and later
 */
extern OSStatus 
ResetTextToUnicodeInfo(TextToUnicodeInfo ioTextToUnicodeInfo) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Re-initialize all state information kept by the context objects. */
/*
 *  ResetUnicodeToTextInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ResetUnicodeToTextInfo(UnicodeToTextInfo ioUnicodeToTextInfo) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Re-initialize all state information kept by the context objects in TextRun conversions. */
/*
 *  ResetUnicodeToTextRunInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in UnicodeConverter 1.1 and later
 */
extern OSStatus 
ResetUnicodeToTextRunInfo(UnicodeToTextRunInfo ioUnicodeToTextRunInfo) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __UNICODECONVERTER__ */

