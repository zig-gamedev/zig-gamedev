/*
     File:       CarbonCore/TextEncodingConverter.h
 
     Contains:   Text Encoding Conversion Interfaces.
 
     Copyright:  © 1994-2011 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTENCODINGCONVERTER__
#define __TEXTENCODINGCONVERTER__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __TEXTCOMMON__
#include <CarbonCore/TextCommon.h>
#endif

#ifndef __CFSTRING__
#include <CoreFoundation/CFString.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

typedef OSType                          TECPluginSignature;
typedef UInt32                          TECPluginVersion;
/* plugin signatures */
enum {
  kTECSignature                 = 'encv',
  kTECUnicodePluginSignature    = 'puni',
  kTECJapanesePluginSignature   = 'pjpn',
  kTECChinesePluginSignature    = 'pzho',
  kTECKoreanPluginSignature     = 'pkor'
};


/* converter object reference */
typedef struct OpaqueTECObjectRef*      TECObjectRef;
typedef struct OpaqueTECSnifferObjectRef*  TECSnifferObjectRef;
typedef OSType                          TECPluginSig;
struct TECConversionInfo {
  TextEncoding        sourceEncoding;
  TextEncoding        destinationEncoding;
  UInt16              reserved1;
  UInt16              reserved2;
};
typedef struct TECConversionInfo        TECConversionInfo;

/*
 *  TECInternetNameUsageMask
 *  
 *  Discussion:
 *    Mask values that control the mapping between TextEncoding and
 *    IANA charset name or MIB enum.
 */
typedef UInt32 TECInternetNameUsageMask;
enum {
                                        /* Use one of the following*/

  /*
   * Use the default type of mapping given other usage information
   * (none currently defined).
   */
  kTECInternetNameDefaultUsageMask = 0,

  /*
   * Use the closest possible match between TextEncoding value and IANA
   * charset name or MIB enum
   */
  kTECInternetNameStrictUsageMask = 1,

  /*
   * When mapping from IANA charset name or MIB enum to TextEncoding,
   * map to the largest superset of the encoding specified by the
   * charset name or MIB enum (i.e. be tolerant). When mapping from
   * TextEncoding to IANA charset name or MIB enum, typically map to
   * the most generic or widely recognized charset name or MIB enum.
   */
  kTECInternetNameTolerantUsageMask = 2
};

/* Special values for MIB enums */
enum {
  kTEC_MIBEnumDontCare          = -1
};

/* Additional control flags for TECSetBasicOptions */
enum {
  kTECDisableFallbacksBit       = 16,
  kTECDisableLooseMappingsBit   = 17
};

enum {
  kTECDisableFallbacksMask      = 1L << kTECDisableFallbacksBit,
  kTECDisableLooseMappingsMask  = 1L << kTECDisableLooseMappingsBit
};


/* return number of encodings types supported by user's configuraton of the encoding converter */
/*
 *  TECCountAvailableTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.1 and later
 */
extern OSStatus 
TECCountAvailableTextEncodings(ItemCount * numberEncodings)   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* fill in an array of type TextEncoding passed in by the user with types of encodings the current configuration of the encoder can handle. */
/*
 *  TECGetAvailableTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECGetAvailableTextEncodings(
  TextEncoding   availableEncodings[],
  ItemCount      maxAvailableEncodings,
  ItemCount *    actualAvailableEncodings)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* return number of from-to encoding conversion pairs supported  */
/*
 *  TECCountDirectTextEncodingConversions()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCountDirectTextEncodingConversions(ItemCount * numberOfEncodings) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* fill in an array of type TextEncodingPair passed in by the user with types of encoding pairs the current configuration of the encoder can handle. */
/*
 *  TECGetDirectTextEncodingConversions()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECGetDirectTextEncodingConversions(
  TECConversionInfo   availableConversions[],
  ItemCount           maxAvailableConversions,
  ItemCount *         actualAvailableConversions)             __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* return number of encodings a given encoding can be converter into */
/*
 *  TECCountDestinationTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCountDestinationTextEncodings(
  TextEncoding   inputEncoding,
  ItemCount *    numberOfEncodings)                           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* fill in an array of type TextEncodingPair passed in by the user with types of encodings pairs the current configuration of the encoder can handle. */
/*
 *  TECGetDestinationTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECGetDestinationTextEncodings(
  TextEncoding   inputEncoding,
  TextEncoding   destinationEncodings[],
  ItemCount      maxDestinationEncodings,
  ItemCount *    actualDestinationEncodings)                  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* get info about a text encoding */
/*
 *  TECGetTextEncodingInternetName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.1 and later
 */
extern OSStatus 
TECGetTextEncodingInternetName(
  TextEncoding   textEncoding,
  Str255         encodingName)                                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECGetTextEncodingFromInternetName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.1 and later
 */
extern OSStatus 
TECGetTextEncodingFromInternetName(
  TextEncoding *     textEncoding,
  ConstStr255Param   encodingName)                            __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* create/dispose converters */
/*
 *  TECCreateConverter()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.1 and later
 */
extern OSStatus 
TECCreateConverter(
  TECObjectRef *  newEncodingConverter,
  TextEncoding    inputEncoding,
  TextEncoding    outputEncoding)                             __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECCreateConverterFromPath()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCreateConverterFromPath(
  TECObjectRef *       newEncodingConverter,
  const TextEncoding   inPath[],
  ItemCount            inEncodings)                           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECDisposeConverter()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.1 and later
 */
extern OSStatus 
TECDisposeConverter(TECObjectRef newEncodingConverter)        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* convert text encodings */
/*
 *  TECClearConverterContextInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECClearConverterContextInfo(TECObjectRef encodingConverter)  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECConvertText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECConvertText(
  TECObjectRef   encodingConverter,
  ConstTextPtr   inputBuffer,
  ByteCount      inputBufferLength,
  ByteCount *    actualInputLength,
  TextPtr        outputBuffer,
  ByteCount      outputBufferLength,
  ByteCount *    actualOutputLength)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECFlushText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECFlushText(
  TECObjectRef   encodingConverter,
  TextPtr        outputBuffer,
  ByteCount      outputBufferLength,
  ByteCount *    actualOutputLength)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* one-to-many routines */
/*
 *  TECCountSubTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCountSubTextEncodings(
  TextEncoding   inputEncoding,
  ItemCount *    numberOfEncodings)                           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECGetSubTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECGetSubTextEncodings(
  TextEncoding   inputEncoding,
  TextEncoding   subEncodings[],
  ItemCount      maxSubEncodings,
  ItemCount *    actualSubEncodings)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECGetEncodingList()
 *  
 *  Parameters:
 *    
 *    encodingConverter:
 *      The encodingConverter to return the encoding list for
 *    
 *    numEncodings:
 *      On exit, the number of encodings in encodingList
 *    
 *    encodingList:
 *      On exit, a handle containing numEncodings values of type
 *      TextEncoding, for each known encoding.  Do not dispose of this
 *      handle.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.1 and later
 */
extern OSStatus 
TECGetEncodingList(
  TECObjectRef   encodingConverter,
  ItemCount *    numEncodings,
  Handle *       encodingList)                                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECCreateOneToManyConverter()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCreateOneToManyConverter(
  TECObjectRef *       newEncodingConverter,
  TextEncoding         inputEncoding,
  ItemCount            numOutputEncodings,
  const TextEncoding   outputEncodings[])                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECConvertTextToMultipleEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECConvertTextToMultipleEncodings(
  TECObjectRef      encodingConverter,
  ConstTextPtr      inputBuffer,
  ByteCount         inputBufferLength,
  ByteCount *       actualInputLength,
  TextPtr           outputBuffer,
  ByteCount         outputBufferLength,
  ByteCount *       actualOutputLength,
  TextEncodingRun   outEncodingsBuffer[],
  ItemCount         maxOutEncodingRuns,
  ItemCount *       actualOutEncodingRuns)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECFlushMultipleEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECFlushMultipleEncodings(
  TECObjectRef      encodingConverter,
  TextPtr           outputBuffer,
  ByteCount         outputBufferLength,
  ByteCount *       actualOutputLength,
  TextEncodingRun   outEncodingsBuffer[],
  ItemCount         maxOutEncodingRuns,
  ItemCount *       actualOutEncodingRuns)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* international internet info */
/*
 *  TECCountWebTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCountWebTextEncodings(
  RegionCode   locale,
  ItemCount *  numberEncodings)                               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECGetWebTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECGetWebTextEncodings(
  RegionCode     locale,
  TextEncoding   availableEncodings[],
  ItemCount      maxAvailableEncodings,
  ItemCount *    actualAvailableEncodings)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECCountMailTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCountMailTextEncodings(
  RegionCode   locale,
  ItemCount *  numberEncodings)                               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECGetMailTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECGetMailTextEncodings(
  RegionCode     locale,
  TextEncoding   availableEncodings[],
  ItemCount      maxAvailableEncodings,
  ItemCount *    actualAvailableEncodings)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* examine text encodings */
/*
 *  TECCountAvailableSniffers()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCountAvailableSniffers(ItemCount * numberOfEncodings)      __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECGetAvailableSniffers()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECGetAvailableSniffers(
  TextEncoding   availableSniffers[],
  ItemCount      maxAvailableSniffers,
  ItemCount *    actualAvailableSniffers)                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECCreateSniffer()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECCreateSniffer(
  TECSnifferObjectRef *  encodingSniffer,
  const TextEncoding     testEncodings[],
  ItemCount              numTextEncodings)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECSniffTextEncoding()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECSniffTextEncoding(
  TECSnifferObjectRef   encodingSniffer,
  ConstTextPtr          inputBuffer,
  ByteCount             inputBufferLength,
  TextEncoding          testEncodings[],
  ItemCount             numTextEncodings,
  ItemCount             numErrsArray[],
  ItemCount             maxErrs,
  ItemCount             numFeaturesArray[],
  ItemCount             maxFeatures)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECDisposeSniffer()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECDisposeSniffer(TECSnifferObjectRef encodingSniffer)        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECClearSnifferContextInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.2 and later
 */
extern OSStatus 
TECClearSnifferContextInfo(TECSnifferObjectRef encodingSniffer) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECSetBasicOptions()
 *  
 *  Summary:
 *    Sets encodingConverter options affecting
 *    TECConvertText[ToMultipleEncodings].
 *  
 *  Parameters:
 *    
 *    encodingConverter:
 *      The high-level encoding converter object created by
 *      TECCreateConverter or TECCreateOneToManyConverter whose
 *      behavior is to be modified by the options specified in
 *      controlFlags.
 *    
 *    controlFlags:
 *      A bit mask specifying the desired options. The following mask
 *      constants are valid for this parameter; multiple mask constants
 *      may be ORed together to set multiple options; passing 0 for
 *      this parameter clears all options: 
 *      
 *      kUnicodeForceASCIIRangeMask, kUnicodeNoHalfwidthCharsMask
 *      (defined in UnicodeConverter.h) 
 *      
 *      kTECDisableFallbacksMask, kTECDisableLooseMappingsMask (defined
 *      above) - loose and fallback mappings are both enabled by
 *      default for the TextEncodingConverter.h conversion APIs
 *      (TECConvertText, TECConvertTextToMultipleEncodings), unlike the
 *      behavior of the conversion APIs in UnicodeConverter.h. These
 *      options may be used to disable loose and/or fallback mappings
 *      for the TextEncodingConverter.h conversion APIs.
 *  
 *  Result:
 *    The function returns paramErr for invalid masks,
 *    kTECCorruptConverterErr for an invalid encodingConverter, noErr
 *    otherwise.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   in TextEncodingConverter 1.5 and later
 */
extern OSStatus 
TECSetBasicOptions(
  TECObjectRef   encodingConverter,
  OptionBits     controlFlags)                                __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_NA);


/* Map TextEncoding values to/from IANA charset names and/or MIB enums, with usage control */
/*
 *  TECCopyTextEncodingInternetNameAndMIB()
 *  
 *  Summary:
 *    Converts a TextEncoding value to an IANA charset name and/or a
 *    MIB enum value
 *  
 *  Discussion:
 *    Given a TextEncoding value, this function maps it to an IANA
 *    charset name (if encodingNamePtr is non-NULL) and/or a MIB enum
 *    value (if mibEnumPtr is non-NULL), as specified by the usage
 *    parameter.
 *  
 *  Parameters:
 *    
 *    textEncoding:
 *      A TextEncoding value to map to a charset name and/or MIB enum.
 *    
 *    usage:
 *      Specifies the type of mapping desired (see
 *      TECInternetNameUsageMask above).
 *    
 *    encodingNamePtr:
 *      If non-NULL, is a pointer to a CStringRef for an immutable
 *      CFString created by this function; when the caller is finished
 *      with it, the caller must dispose of it by calling CFRelease.
 *    
 *    mibEnumPtr:
 *      If non-NULL, is a pointer to an SInt32 that will be set to the
 *      appropriate MIB enum value, or to 0 (or kTEC_MIBEnumDontCare)
 *      if there is no appropriate MIB enum value (valid MIB enums
 *      begin at 3).
 *  
 *  Result:
 *    The function returns paramErr if encodingNamePtr and mibEnumPtr
 *    are both NULL. It returns kTextUnsupportedEncodingErr if it has
 *    no data for the supplied textEncoding. It returns noErr if it
 *    found useful data.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TECCopyTextEncodingInternetNameAndMIB(
  TextEncoding               textEncoding,
  TECInternetNameUsageMask   usage,
  CFStringRef *              encodingNamePtr,       /* can be NULL */
  SInt32 *                   mibEnumPtr)            /* can be NULL */ __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_NA);


/*
 *  TECGetTextEncodingFromInternetNameOrMIB()
 *  
 *  Summary:
 *    Converts an IANA charset name or a MIB enum value to a
 *    TextEncoding value
 *  
 *  Discussion:
 *    If encodingName is non-NULL, this function treats it as an IANA
 *    charset name and maps it to a TextEncoding value; in this case
 *    mibEnum is ignored, and may be set to kTEC_MIBEnumDontCare.
 *    Otherwise, this function maps the mibEnum to a TextEncoding
 *    value. In either case, the mapping is controlled by the usage
 *    parameter. The textEncodingPtr parameter must be non-NULL.
 *  
 *  Result:
 *    The function returns paramErr if textEncodingPtr is NULL. It
 *    returns kTextUnsupportedEncodingErr if it has no data for the
 *    supplied encodingName or mibEnum. It returns noErr if it found
 *    useful data.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TECGetTextEncodingFromInternetNameOrMIB(
  TextEncoding *             textEncodingPtr,
  TECInternetNameUsageMask   usage,
  CFStringRef                encodingName,
  SInt32                     mibEnum)                         __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_NA);






#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TEXTENCODINGCONVERTER__ */

