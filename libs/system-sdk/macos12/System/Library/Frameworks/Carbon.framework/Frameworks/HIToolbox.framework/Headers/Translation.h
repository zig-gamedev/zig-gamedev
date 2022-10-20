/*
     File:       HIToolbox/Translation.h
 
     Contains:   Translation Manager (Macintosh Easy Open) Interfaces.
 
     Copyright:  © 1991-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TRANSLATION__
#define __TRANSLATION__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __TRANSLATIONEXTENSIONS__
#include <HIToolbox/TranslationExtensions.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/*
   Carbon clients should use Translation Services. The definitions below will NOT work for Carbon and
   are only defined for those files that need to build pre-Carbon applications.
*/
/* enumerated types on how a document can be opened*/
typedef short                           DocOpenMethod;
enum {
  domCannot                     = 0,
  domNative                     = 1,
  domTranslateFirst             = 2,
  domWildcard                   = 3
};

/* 0L terminated array of OSTypes, or FileTypes*/
typedef OSType                          TypesBlock[64];
typedef OSType *                        TypesBlockPtr;
/* Progress dialog resource ID*/
enum {
  kTranslationScrapProgressDialogID = -16555
};

/* block of data that describes how to translate*/
struct FileTranslationSpec {
  OSType              componentSignature;
  const void *        translationSystemInfo;
  FileTypeSpec        src;
  FileTypeSpec        dst;
};
typedef struct FileTranslationSpec      FileTranslationSpec;
typedef FileTranslationSpec *           FileTranslationSpecArrayPtr;
typedef FileTranslationSpecArrayPtr *   FileTranslationSpecArrayHandle;
#if !__LP64__
/*
 *  GetFileTypesThatAppCanNativelyOpen()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    There is no direct replacement at this time.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
GetFileTypesThatAppCanNativelyOpen(
  short       appVRefNumHint,
  OSType      appSignature,
  FileType *  nativeTypes)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;



/*
 *  ExtendFileTypeList()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TranslationCreateWithSourceArray instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
ExtendFileTypeList(
  const FileType *  originalTypeList,
  short             numberOriginalTypes,
  FileType *        extendedTypeList,
  short *           numberExtendedTypes)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;


/*
 *  CanDocBeOpened()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the Launch Services API instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
CanDocBeOpened(
  const FSSpec *         targetDocument,
  short                  appVRefNumHint,
  OSType                 appSignature,
  const FileType *       nativeTypes,
  Boolean                onlyNative,
  DocOpenMethod *        howToOpen,
  FileTranslationSpec *  howToTranslate)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;



/*
 *  GetFileTranslationPaths()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TranslationCreateWithSourceArray instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern short 
GetFileTranslationPaths(
  const FSSpec *                srcDocument,
  FileType                      dstDocType,
  unsigned short                maxResultCount,
  FileTranslationSpecArrayPtr   resultBuffer)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;


/*
 *  GetPathFromTranslationDialog()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    There is no direct replacement at this time, but all the
 *    necessary information can be obtained from the Launch Services,
 *    Translation Services and Uniform Type Identification APIs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
GetPathFromTranslationDialog(
  const FSSpec *         theDocument,
  const FSSpec *         theApplication,
  TypesBlockPtr          typeList,
  DocOpenMethod *        howToOpen,
  FileTranslationSpec *  howToTranslate)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;


/*
 *  TranslateFile()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TranslationPerformForFile or TranslationPerformForURL instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
TranslateFile(
  const FSSpec *               sourceDocument,
  const FSSpec *               destinationDocument,
  const FileTranslationSpec *  howToTranslate)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;


/*
 *  GetDocumentKindString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the Launch Services API instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
GetDocumentKindString(
  short    docVRefNum,
  OSType   docType,
  OSType   docCreator,
  Str63    kindString)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;


/*
 *  GetTranslationExtensionName()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    There is no direct replacement at this time, but
 *    TranslationCopySourceType and TranslationCopyDestinationType in
 *    conjunction with UTTypeCopyDescription will provide useful user
 *    level information.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
GetTranslationExtensionName(
  const FileTranslationSpec *  translationMethod,
  Str31                        extensionName)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;


#endif  /* !__LP64__ */


/*
 *  GetScrapDataProcPtr
 */
typedef CALLBACK_API( OSErr , GetScrapDataProcPtr )(ScrapType requestedFormat, Handle dataH, void *srcDataGetterRefCon);
typedef STACK_UPP_TYPE(GetScrapDataProcPtr)                     GetScrapDataUPP;
/*
 *  NewGetScrapDataUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern GetScrapDataUPP
NewGetScrapDataUPP(GetScrapDataProcPtr userRoutine)           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;

/*
 *  DisposeGetScrapDataUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeGetScrapDataUPP(GetScrapDataUPP userUPP)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;

/*
 *  InvokeGetScrapDataUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeGetScrapDataUPP(
  ScrapType        requestedFormat,
  Handle           dataH,
  void *           srcDataGetterRefCon,
  GetScrapDataUPP  userUPP)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;

#if __MACH__
  #ifdef __cplusplus
    inline GetScrapDataUPP                                      NewGetScrapDataUPP(GetScrapDataProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeGetScrapDataUPP(GetScrapDataUPP) { }
    inline OSErr                                                InvokeGetScrapDataUPP(ScrapType requestedFormat, Handle dataH, void * srcDataGetterRefCon, GetScrapDataUPP userUPP) { return (*userUPP)(requestedFormat, dataH, srcDataGetterRefCon); }
  #else
    #define NewGetScrapDataUPP(userRoutine)                     ((GetScrapDataUPP)userRoutine)
    #define DisposeGetScrapDataUPP(userUPP)
    #define InvokeGetScrapDataUPP(requestedFormat, dataH, srcDataGetterRefCon, userUPP) (*userUPP)(requestedFormat, dataH, srcDataGetterRefCon)
  #endif
#endif

typedef GetScrapDataUPP                 GetScrapData;
#if !__LP64__
/*
 *  TranslateScrap()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TranslationPerformForData instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
TranslateScrap(
  GetScrapDataUPP   sourceDataGetter,
  void *            sourceDataGetterRefCon,
  ScrapType         destinationFormat,
  Handle            destinationData,
  short             progressDialogID)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;



#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TRANSLATION__ */

