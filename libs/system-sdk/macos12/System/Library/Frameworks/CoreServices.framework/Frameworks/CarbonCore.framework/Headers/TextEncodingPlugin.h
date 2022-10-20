/*
     File:       CarbonCore/TextEncodingPlugin.h
 
     Contains:   Required interface for Text Encoding Converter-Plugins
 
     Copyright:  © 1996-2011 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTENCODINGPLUGIN__
#define __TEXTENCODINGPLUGIN__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __TEXTCOMMON__
#include <CarbonCore/TextCommon.h>
#endif

#ifndef __TEXTENCODINGCONVERTER__
#include <CarbonCore/TextEncodingConverter.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#if defined(__x86_64__)
#pragma pack(push, 2)
#endif

/*
  ####################################################################################
        Constants
  ####################################################################################
*/
/*
   This constant is needed for MacOS X development only. It is the name in which the
   function to grab the plugin's dispatch table must go by. 
*/
#define kTECMacOSXDispatchTableNameString   "ConverterPluginGetPluginDispatchTable"
/* These constant are needed for TEC plugins.*/
enum {
  kTECAvailableEncodingsResType = 'cven',
  kTECAvailableSniffersResType  = 'cvsf',
  kTECSubTextEncodingsResType   = 'cvsb',
  kTECConversionInfoResType     = 'cvif',
  kTECMailEncodingsResType      = 'cvml',
  kTECWebEncodingsResType       = 'cvwb',
  kTECInternetNamesResType      = 'cvmm'
};

enum {
  kTECPluginType                = 'ecpg',
  kTECPluginCreator             = 'encv',
  kTECPluginOneToOne            = 'otoo',
  kTECPluginOneToMany           = 'otom',
  kTECPluginManyToOne           = 'mtoo',
  kTECPluginSniffObj            = 'snif'
};

enum {
  verUnspecified                = 32767,
  kTECResourceID                = 128
};

/*
  ####################################################################################
        Structs
  ####################################################################################
*/

/* These structs are needed for TEC plugins.*/

struct TextEncodingRec {
  UInt32              base;
  UInt32              variant;
  UInt32              format;
};
typedef struct TextEncodingRec          TextEncodingRec;
/* supported encodings & sniffers lists, type TECEncodingsListRec */
struct TECEncodingsListRec {
  UInt32              count;
  TextEncodingRec     encodings;              /* first of many*/
};
typedef struct TECEncodingsListRec      TECEncodingsListRec;
typedef TECEncodingsListRec *           TECEncodingsListPtr;
typedef TECEncodingsListPtr *           TECEncodingsListHandle;
/* sub encodings list - type TECSubTextEncodingsRec */
struct TECSubTextEncodingRec {
  UInt32              offset;                 /* offset to next variable-length record*/
  TextEncodingRec     searchEncoding;         /* the encoding*/
  UInt32              count;
  TextEncodingRec     subEncodings;           /* first of many sub encodings for searchEncoding*/
};
typedef struct TECSubTextEncodingRec    TECSubTextEncodingRec;
struct TECSubTextEncodingsRec {
  UInt32              count;
  TECSubTextEncodingRec  subTextEncodingRec;  /* first of many*/
};
typedef struct TECSubTextEncodingsRec   TECSubTextEncodingsRec;
typedef TECSubTextEncodingsRec *        TECSubTextEncodingsPtr;
typedef TECSubTextEncodingsPtr *        TECSubTextEncodingsHandle;
/* conversions pairs list - type TECEncodingPairsRec */
struct TECEncodingPairRec {
  TextEncodingRec     source;
  TextEncodingRec     dest;
};
typedef struct TECEncodingPairRec       TECEncodingPairRec;
struct TECEncodingPairs {
  TECEncodingPairRec  encodingPair;
  UInt32              flags;                  /* 'flags' name is not really used yet (JKC 9/5/97)*/
  UInt32              speed;                  /* 'speed' name is not really used yet (JKC 9/5/97)*/
};
typedef struct TECEncodingPairs         TECEncodingPairs;
struct TECEncodingPairsRec {
  UInt32              count;
  TECEncodingPairs    encodingPairs;
};
typedef struct TECEncodingPairsRec      TECEncodingPairsRec;
typedef TECEncodingPairsRec *           TECEncodingPairsPtr;
typedef TECEncodingPairsPtr *           TECEncodingPairsHandle;
/* mail & web encodings lists - type TECLocaleToEncodingsListRec */
struct TECLocaleListToEncodingListRec {
  UInt32              offset;                 /* offset to next variable-length record*/
  UInt32              count;
  RegionCode          locales;                /* first in list of locales*/
                                              /* TECEncodingListRec encodingList;     // after local variable length array*/
};
typedef struct TECLocaleListToEncodingListRec TECLocaleListToEncodingListRec;
typedef TECLocaleListToEncodingListRec * TECLocaleListToEncodingListPtr;
struct TECLocaleToEncodingsListRec {
  UInt32              count;
  TECLocaleListToEncodingListRec  localeListToEncodingList; /* language of name*/
};
typedef struct TECLocaleToEncodingsListRec TECLocaleToEncodingsListRec;
typedef TECLocaleToEncodingsListRec *   TECLocaleToEncodingsListPtr;
typedef TECLocaleToEncodingsListPtr *   TECLocaleToEncodingsListHandle;
/* internet names list - type TECInternetNamesRec */
struct TECInternetNameRec {
  UInt32              offset;                 /* offset to next variable-length record*/
  TextEncodingRec     searchEncoding;         /* named encoding*/
  UInt8               encodingNameLength;
  UInt8               encodingName[1];        /* first byte of many */
};
typedef struct TECInternetNameRec       TECInternetNameRec;
struct TECInternetNamesRec {
  UInt32              count;
  TECInternetNameRec  InternetNames;          /* first of many*/
};
typedef struct TECInternetNamesRec      TECInternetNamesRec;
typedef TECInternetNamesRec *           TECInternetNamesPtr;
typedef TECInternetNamesPtr *           TECInternetNamesHandle;
/* plugin context record */
struct TECBufferContextRec {
  ConstTextPtr        textInputBuffer;
  ConstTextPtr        textInputBufferEnd;
  TextPtr             textOutputBuffer;
  TextPtr             textOutputBufferEnd;

  ConstTextEncodingRunPtr  encodingInputBuffer;
  ConstTextEncodingRunPtr  encodingInputBufferEnd;
  TextEncodingRunPtr  encodingOutputBuffer;
  TextEncodingRunPtr  encodingOutputBufferEnd;
};
typedef struct TECBufferContextRec      TECBufferContextRec;
struct TECPluginStateRec {

  UInt8               state1;
  UInt8               state2;
  UInt8               state3;
  UInt8               state4;

  UInt32              longState1;
  UInt32              longState2;
  UInt32              longState3;
  UInt32              longState4;
};
typedef struct TECPluginStateRec        TECPluginStateRec;
struct TECConverterContextRec {

                                              /* public - manipulated externally and by plugin*/
  Ptr                 pluginRec;
  TextEncoding        sourceEncoding;
  TextEncoding        destEncoding;
  UInt32              reserved1;
  UInt32              reserved2;
  TECBufferContextRec  bufferContext;
                                              /* private - manipulated only within Plugin*/
  URefCon             contextRefCon;
  ProcPtr             conversionProc;
  ProcPtr             flushProc;
  ProcPtr             clearContextInfoProc;
  UInt32              options1;
  UInt32              options2;
  TECPluginStateRec   pluginState;
};
typedef struct TECConverterContextRec   TECConverterContextRec;
struct TECSnifferContextRec {
                                              /* public - manipulated externally*/
  Ptr                 pluginRec;
  TextEncoding        encoding;
  ItemCount           maxErrors;
  ItemCount           maxFeatures;
  ConstTextPtr        textInputBuffer;
  ConstTextPtr        textInputBufferEnd;
  ItemCount           numFeatures;
  ItemCount           numErrors;
                                              /* private - manipulated only within Plugin*/
  URefCon             contextRefCon;
  ProcPtr             sniffProc;
  ProcPtr             clearContextInfoProc;
  TECPluginStateRec   pluginState;
};
typedef struct TECSnifferContextRec     TECSnifferContextRec;
/*
  ####################################################################################
        Functional Messages
  ####################################################################################
*/

typedef CALLBACK_API_C( OSStatus , TECPluginNewEncodingConverterPtr )(TECObjectRef *newEncodingConverter, TECConverterContextRec *plugContext, TextEncoding inputEncoding, TextEncoding outputEncoding);
typedef CALLBACK_API_C( OSStatus , TECPluginClearContextInfoPtr )(TECObjectRef encodingConverter, TECConverterContextRec *plugContext);
typedef CALLBACK_API_C( OSStatus , TECPluginConvertTextEncodingPtr )(TECObjectRef encodingConverter, TECConverterContextRec *plugContext);
typedef CALLBACK_API_C( OSStatus , TECPluginFlushConversionPtr )(TECObjectRef encodingConverter, TECConverterContextRec *plugContext);
typedef CALLBACK_API_C( OSStatus , TECPluginDisposeEncodingConverterPtr )(TECObjectRef newEncodingConverter, TECConverterContextRec *plugContext);
typedef CALLBACK_API_C( OSStatus , TECPluginNewEncodingSnifferPtr )(TECSnifferObjectRef *encodingSniffer, TECSnifferContextRec *snifContext, TextEncoding inputEncoding);
typedef CALLBACK_API_C( OSStatus , TECPluginClearSnifferContextInfoPtr )(TECSnifferObjectRef encodingSniffer, TECSnifferContextRec *snifContext);
typedef CALLBACK_API_C( OSStatus , TECPluginSniffTextEncodingPtr )(TECSnifferObjectRef encodingSniffer, TECSnifferContextRec *snifContext);
typedef CALLBACK_API_C( OSStatus , TECPluginDisposeEncodingSnifferPtr )(TECSnifferObjectRef encodingSniffer, TECSnifferContextRec *snifContext);
typedef CALLBACK_API_C( OSStatus , TECPluginGetCountAvailableTextEncodingsPtr )(TextEncoding *availableEncodings, ItemCount maxAvailableEncodings, ItemCount *actualAvailableEncodings);
typedef CALLBACK_API_C( OSStatus , TECPluginGetCountAvailableTextEncodingPairsPtr )(TECConversionInfo *availableEncodings, ItemCount maxAvailableEncodings, ItemCount *actualAvailableEncodings);
typedef CALLBACK_API_C( OSStatus , TECPluginGetCountDestinationTextEncodingsPtr )(TextEncoding inputEncoding, TextEncoding *destinationEncodings, ItemCount maxDestinationEncodings, ItemCount *actualDestinationEncodings);
typedef CALLBACK_API_C( OSStatus , TECPluginGetCountSubTextEncodingsPtr )(TextEncoding inputEncoding, TextEncoding subEncodings[], ItemCount maxSubEncodings, ItemCount *actualSubEncodings);
typedef CALLBACK_API_C( OSStatus , TECPluginGetCountAvailableSniffersPtr )(TextEncoding *availableEncodings, ItemCount maxAvailableEncodings, ItemCount *actualAvailableEncodings);
typedef CALLBACK_API_C( OSStatus , TECPluginGetTextEncodingInternetNamePtr )(TextEncoding textEncoding, Str255 encodingName);
typedef CALLBACK_API_C( OSStatus , TECPluginGetTextEncodingFromInternetNamePtr )(TextEncoding *textEncoding, ConstStr255Param encodingName);
typedef CALLBACK_API_C( OSStatus , TECPluginGetCountWebEncodingsPtr )(TextEncoding *availableEncodings, ItemCount maxAvailableEncodings, ItemCount *actualAvailableEncodings);
typedef CALLBACK_API_C( OSStatus , TECPluginGetCountMailEncodingsPtr )(TextEncoding *availableEncodings, ItemCount maxAvailableEncodings, ItemCount *actualAvailableEncodings);
/*
  ####################################################################################
        Dispatch Table Definition
  ####################################################################################
*/

enum {
  kTECPluginDispatchTableVersion1 = 0x00010000, /* 1.0 through 1.0.3 releases*/
  kTECPluginDispatchTableVersion1_1 = 0x00010001, /* 1.1 releases*/
  kTECPluginDispatchTableVersion1_2 = 0x00010002, /* 1.2 releases*/
  kTECPluginDispatchTableCurrentVersion = kTECPluginDispatchTableVersion1_2
};

struct TECPluginDispatchTable {

  TECPluginVersion    version;
  TECPluginVersion    compatibleVersion;
  TECPluginSignature  PluginID;

  TECPluginNewEncodingConverterPtr  PluginNewEncodingConverter;
  TECPluginClearContextInfoPtr  PluginClearContextInfo;
  TECPluginConvertTextEncodingPtr  PluginConvertTextEncoding;
  TECPluginFlushConversionPtr  PluginFlushConversion;
  TECPluginDisposeEncodingConverterPtr  PluginDisposeEncodingConverter;

  TECPluginNewEncodingSnifferPtr  PluginNewEncodingSniffer;
  TECPluginClearSnifferContextInfoPtr  PluginClearSnifferContextInfo;
  TECPluginSniffTextEncodingPtr  PluginSniffTextEncoding;
  TECPluginDisposeEncodingSnifferPtr  PluginDisposeEncodingSniffer;

  TECPluginGetCountAvailableTextEncodingsPtr  PluginGetCountAvailableTextEncodings;
  TECPluginGetCountAvailableTextEncodingPairsPtr  PluginGetCountAvailableTextEncodingPairs;
  TECPluginGetCountDestinationTextEncodingsPtr  PluginGetCountDestinationTextEncodings;
  TECPluginGetCountSubTextEncodingsPtr  PluginGetCountSubTextEncodings;
  TECPluginGetCountAvailableSniffersPtr  PluginGetCountAvailableSniffers;
  TECPluginGetCountWebEncodingsPtr  PluginGetCountWebTextEncodings;
  TECPluginGetCountMailEncodingsPtr  PluginGetCountMailTextEncodings;

  TECPluginGetTextEncodingInternetNamePtr  PluginGetTextEncodingInternetName;
  TECPluginGetTextEncodingFromInternetNamePtr  PluginGetTextEncodingFromInternetName;

};
typedef struct TECPluginDispatchTable   TECPluginDispatchTable;
/*
   The last prototype here is for MacOS X plugins only. TEC Plugins in MacOS X need to export a
   a function called ConverterPluginGetPluginDispatchTable with the following prototype:
   extern TECPluginDispatchTable *ConverterPluginGetPluginDispatchTable( void )
   This function will need to return a pointer to the plugin's function dispatch table 
   when called. It is important that the function be called exactly 
   "ConverterPluginGetPluginDispatchTable". TECPluginGetPluginDispatchTablePtr is a 
   function pointer to this function.
*/
typedef CALLBACK_API_C( TECPluginDispatchTable *, TECPluginGetPluginDispatchTablePtr )(void);






#if defined(__x86_64__)
#pragma pack(pop)
#endif


#endif /* __TEXTENCODINGPLUGIN__ */

