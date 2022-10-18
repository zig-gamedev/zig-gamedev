/*
     File:       AE/AEHelpers.h
 
     Contains:   AEPrint, AEBuild and AEStream for Carbon
 
    
 
     Copyright:  (c) 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
/*
 * Originally from AEGIzmos by Jens Alfke, circa 1992.
 */
#ifndef __AEHELPERS__
#define __AEHELPERS__

#include <stdarg.h>
#ifndef __APPLEEVENTS__
#include <AE/AppleEvents.h>
#endif

#ifndef __AEDATAMODEL__
#include <AE/AEDataModel.h>
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

/*
 * AEBuild:
 *
 * AEBuild provides a very high level abstraction for building
 * complete AppleEvents and complex ObjectSpeciers.  Using AEBuild it
 * is easy to produce a textual representation of an AEDesc.  The
 * format is similar to the stdio printf call, where meta data is
 * extracted from a format string and used to build the final
 * representation.
 * 
 * For more information on AEBuild and other APIs in AEHelpers, see:
 *     <http://developer.apple.com/technotes/tn/tn2045.html>
 */
/* Syntax Error Codes: */
typedef UInt32 AEBuildErrorCode;
enum {
  aeBuildSyntaxNoErr            = 0,    /* (No error) */
  aeBuildSyntaxBadToken         = 1,    /* Illegal character */
  aeBuildSyntaxBadEOF           = 2,    /* Unexpected end of format string */
  aeBuildSyntaxNoEOF            = 3,    /* Unexpected extra stuff past end */
  aeBuildSyntaxBadNegative      = 4,    /* "-" not followed by digits */
  aeBuildSyntaxMissingQuote     = 5,    /* Missing close "'" */
  aeBuildSyntaxBadHex           = 6,    /* Non-digit in hex string */
  aeBuildSyntaxOddHex           = 7,    /* Odd # of hex digits */
  aeBuildSyntaxNoCloseHex       = 8,    /* Missing hex quote close "È" */
  aeBuildSyntaxUncoercedHex     = 9,    /* Hex string must be coerced to a type */
  aeBuildSyntaxNoCloseString    = 10,   /* Missing close quote */
  aeBuildSyntaxBadDesc          = 11,   /* Illegal descriptor */
  aeBuildSyntaxBadData          = 12,   /* Bad data value inside (Ç È) */
  aeBuildSyntaxNoCloseParen     = 13,   /* Missing ")" after data value */
  aeBuildSyntaxNoCloseBracket   = 14,   /* Expected "," or "]" */
  aeBuildSyntaxNoCloseBrace     = 15,   /* Expected "," or "}" */
  aeBuildSyntaxNoKey            = 16,   /* Missing keyword in record */
  aeBuildSyntaxNoColon          = 17,   /* Missing ":" after keyword in record */
  aeBuildSyntaxCoercedList      = 18,   /* Cannot coerce a list */
  aeBuildSyntaxUncoercedDoubleAt = 19   /* "@@" substitution must be coerced */
};

/* A structure containing error state.*/

struct AEBuildError {
  AEBuildErrorCode    fError;
  UInt32              fErrorPos;
};
typedef struct AEBuildError             AEBuildError;
/*
   Create an AEDesc from the format string.  AEBuildError can be NULL, in which case
   no explicit error information will be returned.
*/
/*
 *  AEBuildDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEBuildDesc(
  AEDesc *        dst,
  AEBuildError *  error,       /* can be NULL */
  const char *    src,
  ...)                                                        API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* varargs version of AEBuildDesc*/
/*
 *  vAEBuildDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
vAEBuildDesc(
  AEDesc *        dst,
  AEBuildError *  error,       /* can be NULL */
  const char *    src,
  va_list         args)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/* Append parameters to an existing AppleEvent*/
/*
 *  AEBuildParameters()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEBuildParameters(
  AppleEvent *    event,
  AEBuildError *  error,        /* can be NULL */
  const char *    format,
  ...)                                                        API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* varargs version of AEBuildParameters*/
/*
 *  vAEBuildParameters()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
vAEBuildParameters(
  AppleEvent *    event,
  AEBuildError *  error,        /* can be NULL */
  const char *    format,
  va_list         args)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Building an entire Apple event:*/
/*
 *  AEBuildAppleEvent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEBuildAppleEvent(
  AEEventClass    theClass,
  AEEventID       theID,
  DescType        addressType,
  const void *    addressData,
  Size            addressLength,
  SInt16          returnID,
  SInt32          transactionID,
  AppleEvent *    result,
  AEBuildError *  error,               /* can be NULL */
  const char *    paramsFmt,
  ...)                                                        API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* varargs version of AEBuildAppleEvent*/
/*
 *  vAEBuildAppleEvent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
vAEBuildAppleEvent(
  AEEventClass    theClass,
  AEEventID       theID,
  DescType        addressType,
  const void *    addressData,
  Size            addressLength,
  SInt16          returnID,
  SInt32          transactionID,
  AppleEvent *    resultEvt,
  AEBuildError *  error,               /* can be NULL */
  const char *    paramsFmt,
  va_list         args)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 * AEPrintDescToHandle
 *
 * AEPrintDescToHandle provides a way to turn an AEDesc into a textual
 * representation.  This is most useful for debugging calls to
 * AEBuildDesc and friends.  The Handle returned should be disposed by
 * the caller.  The size of the handle is the actual number of
 * characters in the string.
 */
/*
 *  AEPrintDescToHandle()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEPrintDescToHandle(
  const AEDesc *  desc,
  Handle *        result)                                     API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 * AEStream:
 *
 * The AEStream interface allows you to build AppleEvents by appending
 * to an opaque structure (an AEStreamRef) and then turning this
 * structure into an AppleEvent.  The basic idea is to open the
 * stream, write data, and then close it - closing it produces an
 * AEDesc, which may be partially complete, or may be a complete
 * AppleEvent.
 */
typedef struct OpaqueAEStreamRef*       AEStreamRef;
/*
   Create and return an AEStreamRef
   Returns NULL on memory allocation failure
*/
/*
 *  AEStreamOpen()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern AEStreamRef 
AEStreamOpen(void)                                            API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   Closes and disposes of an AEStreamRef, producing
   results in the desc.  You must dispose of the desc yourself.
   If you just want to dispose of the AEStreamRef, you can pass NULL for desc.
*/
/*
 *  AEStreamClose()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamClose(
  AEStreamRef   ref,
  AEDesc *      desc)                                         API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   Prepares an AEStreamRef for appending data to a newly created desc.
   You append data with AEStreamWriteData
*/
/*
 *  AEStreamOpenDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamOpenDesc(
  AEStreamRef   ref,
  DescType      newType)                                      API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Append data to the previously opened desc.*/
/*
 *  AEStreamWriteData()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamWriteData(
  AEStreamRef   ref,
  const void *  data,
  Size          length)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   Finish a desc.  After this, you can close the stream, or adding new
   descs, if you're assembling a list.
*/
/*
 *  AEStreamCloseDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamCloseDesc(AEStreamRef ref)                            API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Write data as a desc to the stream*/
/*
 *  AEStreamWriteDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamWriteDesc(
  AEStreamRef   ref,
  DescType      newType,
  const void *  data,
  Size          length)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Write an entire desc to the stream*/
/*
 *  AEStreamWriteAEDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamWriteAEDesc(
  AEStreamRef     ref,
  const AEDesc *  desc)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   Begin a list.  You can then append to the list by doing
   AEStreamOpenDesc, or AEStreamWriteDesc.
*/
/*
 *  AEStreamOpenList()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamOpenList(AEStreamRef ref)                             API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Finish a list.*/
/*
 *  AEStreamCloseList()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamCloseList(AEStreamRef ref)                            API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   Begin a record.  A record usually has type 'reco', however, this is
   rather generic, and frequently a different type is used.
*/
/*
 *  AEStreamOpenRecord()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamOpenRecord(
  AEStreamRef   ref,
  DescType      newType)                                      API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Change the type of a record.*/
/*
 *  AEStreamSetRecordType()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamSetRecordType(
  AEStreamRef   ref,
  DescType      newType)                                      API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Finish a record*/
/*
 *  AEStreamCloseRecord()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamCloseRecord(AEStreamRef ref)                          API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   Add a keyed descriptor to a record.  This is analogous to AEPutParamDesc.
   it can only be used when writing to a record.
*/
/*
 *  AEStreamWriteKeyDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamWriteKeyDesc(
  AEStreamRef   ref,
  AEKeyword     key,
  DescType      newType,
  const void *  data,
  Size          length)                                       API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   OpenDesc for a keyed record entry.  You can use AEStreamWriteData
   after opening a keyed desc.
*/
/*
 *  AEStreamOpenKeyDesc()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamOpenKeyDesc(
  AEStreamRef   ref,
  AEKeyword     key,
  DescType      newType)                                      API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Write a key to the stream - you can follow this with an AEWriteDesc.*/
/*
 *  AEStreamWriteKey()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamWriteKey(
  AEStreamRef   ref,
  AEKeyword     key)                                          API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   Create a complete AppleEvent.  This creates and returns a new stream.
   Use this call to populate the meta fields in an AppleEvent record.
   After this, you can add your records, lists and other parameters.
*/
/*
 *  AEStreamCreateEvent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern AEStreamRef 
AEStreamCreateEvent(
  AEEventClass   clazz,
  AEEventID      id,
  DescType       targetType,
  const void *   targetData,
  Size           targetLength,
  SInt16         returnID,
  SInt32         transactionID)                               API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
   This call lets you augment an existing AppleEvent using the stream
   APIs.  This would be useful, for example, in constructing the reply
   record in an AppleEvent handler.  Note that AEStreamOpenEvent will
   consume the AppleEvent passed in - you can't access it again until the
   stream is closed.  When you're done building the event, AEStreamCloseStream
    will reconstitute it.
*/
/*
 *  AEStreamOpenEvent()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern AEStreamRef 
AEStreamOpenEvent(AppleEvent * event)                         API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/* Mark a keyword as being an optional parameter.*/
/*
 *  AEStreamOptionalParam()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
AEStreamOptionalParam(
  AEStreamRef   ref,
  AEKeyword     key)                                          API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __AEHELPERS__ */

