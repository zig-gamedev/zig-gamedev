/*
     File:       HIToolbox/TranslationExtensions.h
 
     Contains:   Macintosh Easy Open Translation Extension Interfaces.
 
     Copyright:  © 1993-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TRANSLATIONEXTENSIONS__
#define __TRANSLATIONEXTENSIONS__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
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
   Translation Extensions are no longer supported. Carbon clients interested in extending translations
   should use filter services as described in TranslationServices.h.  The definitions below will NOT work
   for Carbon and are only defined for those files that need to build pre-Carbon applications.
*/
enum {
  kSupportsFileTranslation      = 1,
  kSupportsScrapTranslation     = 2,
  kTranslatorCanGenerateFilename = 4
};

/******************************************************************************************/
/* better names for 4-char codes*/
typedef OSType                          FileType;
typedef ResType                         ScrapType;
/******************************************************************************************/
typedef UInt32 TranslationAttributes;
enum {
  taDstDocNeedsResourceFork     = 1,
  taDstIsAppTranslation         = 2
};

/******************************************************************************************/
struct FileTypeSpec {
  FileType            format;
  long                hint;
  TranslationAttributes  flags;               /* taDstDocNeedsResourceFork, taDstIsAppTranslation*/
  OSType              catInfoType;
  OSType              catInfoCreator;
};
typedef struct FileTypeSpec             FileTypeSpec;
struct FileTranslationList {
  unsigned long       modDate;
  unsigned long       groupCount;

                                              /* conceptual declarations:*/

                                              /*    unsigned long group1SrcCount;*/
                                              /*    unsigned long group1SrcEntrySize = sizeof(FileTypeSpec);*/
                                              /*  FileTypeSpec  group1SrcTypes[group1SrcCount]*/
                                              /*  unsigned long group1DstCount;*/
                                              /*  unsigned long group1DstEntrySize = sizeof(FileTypeSpec);*/
                                              /*  FileTypeSpec  group1DstTypes[group1DstCount]*/
};
typedef struct FileTranslationList      FileTranslationList;
typedef FileTranslationList *           FileTranslationListPtr;
typedef FileTranslationListPtr *        FileTranslationListHandle;
/******************************************************************************************/
struct ScrapTypeSpec {
  ScrapType           format;
  long                hint;
};
typedef struct ScrapTypeSpec            ScrapTypeSpec;
struct ScrapTranslationList {
  unsigned long       modDate;
  unsigned long       groupCount;

                                              /* conceptual declarations:*/

                                              /*    unsigned long     group1SrcCount;*/
                                              /*    unsigned long     group1SrcEntrySize = sizeof(ScrapTypeSpec);*/
                                              /*  ScrapTypeSpec     group1SrcTypes[group1SrcCount]*/
                                              /*  unsigned long     group1DstCount;*/
                                              /*    unsigned long     group1DstEntrySize = sizeof(ScrapTypeSpec);*/
                                              /*  ScrapTypeSpec     group1DstTypes[group1DstCount]*/
};
typedef struct ScrapTranslationList     ScrapTranslationList;
typedef ScrapTranslationList *          ScrapTranslationListPtr;
typedef ScrapTranslationListPtr *       ScrapTranslationListHandle;
/*******************************************************************************************

    definition of callbacks to update progress dialog

*******************************************************************************************/
typedef long                            TranslationRefNum;
/*******************************************************************************************

    This routine sets the advertisement in the top half of the progress dialog.
    It is called once at the beginning of your DoTranslateFile routine.

    Enter   :   refNum          Translation reference supplied to DoTranslateFile.
                advertisement   A handle to the picture to display.  This must be non-purgable.
                                Before returning from DoTranslateFile, you should dispose
                                of the memory.  (Normally, it is in the temp translation heap
                                so it is cleaned up for you.)

    Exit    :   returns         noErr, paramErr, or memFullErr

*******************************************************************************************/
#if !__LP64__
/*
 *  SetTranslationAdvertisement()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    There is no direct replacement at this time.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 thru 1.0.2
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
SetTranslationAdvertisement(
  TranslationRefNum   refNum,
  PicHandle           advertisement)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;



/*******************************************************************************************

    This routine updates the progress bar in the progress dialog.
    It is called repeatedly from within your DoTranslateFile routine.
    It should be called often, so that the user will get feedback if
    he tries to cancel.

    Enter   :   refNum      translation reference supplied to DoTranslateFile.
                progress    percent complete (0-100)

    Exit    :   canceled    TRUE if the user clicked the Cancel button, FALSE otherwise

    Return  :   noErr, paramErr, or memFullErr

*******************************************************************************************/
/*
 *  UpdateTranslationProgress()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    There is no direct replacement at this time.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 thru 1.0.2
 *    Non-Carbon CFM:   in Translation 1.0 and later
 */
extern OSErr 
UpdateTranslationProgress(
  TranslationRefNum   refNum,
  short               percentDone,
  Boolean *           canceled)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;


/*******************************************************************************************

    Component Manager component selectors for translation extension routines

*******************************************************************************************/
#endif  /* !__LP64__ */

enum {
  kTranslateGetFileTranslationList = 0,
  kTranslateIdentifyFile        = 1,
  kTranslateTranslateFile       = 2,
  kTranslateGetTranslatedFilename = 3,
  kTranslateGetScrapTranslationList = 10,
  kTranslateIdentifyScrap       = 11,
  kTranslateTranslateScrap      = 12,
  kTranslateGetScrapTranslationListConsideringData = 13
};


/*******************************************************************************************

    routines which implement translation extensions

*******************************************************************************************/
typedef CALLBACK_API( ComponentResult , DoGetFileTranslationListProcPtr )(ComponentInstance self, FileTranslationListHandle translationList);
typedef CALLBACK_API( ComponentResult , DoIdentifyFileProcPtr )(ComponentInstance self, const FSSpec *theDocument, FileType *docType);
typedef CALLBACK_API( ComponentResult , DoTranslateFileProcPtr )(ComponentInstance self, TranslationRefNum refNum, const FSSpec *sourceDocument, FileType srcType, long srcTypeHint, const FSSpec *dstDoc, FileType dstType, long dstTypeHint);
typedef CALLBACK_API( ComponentResult , DoGetTranslatedFilenameProcPtr )(ComponentInstance self, FileType dstType, long dstTypeHint, FSSpec *theDocument);
typedef CALLBACK_API( ComponentResult , DoGetScrapTranslationListProcPtr )(ComponentInstance self, ScrapTranslationListHandle list);
typedef CALLBACK_API( ComponentResult , DoIdentifyScrapProcPtr )(ComponentInstance self, const void *dataPtr, Size dataLength, ScrapType *dataFormat);
typedef CALLBACK_API( ComponentResult , DoTranslateScrapProcPtr )(ComponentInstance self, TranslationRefNum refNum, const void *srcDataPtr, Size srcDataLength, ScrapType srcType, long srcTypeHint, Handle dstData, ScrapType dstType, long dstTypeHint);

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TRANSLATIONEXTENSIONS__ */

