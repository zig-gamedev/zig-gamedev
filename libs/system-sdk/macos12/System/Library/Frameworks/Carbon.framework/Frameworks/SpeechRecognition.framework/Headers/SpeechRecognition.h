/*
     File:       SpeechRecognition/SpeechRecognition.h
 
     Note:       This API has been deprecated. Please use NSSpeechRecognizer class instead.
 
     Copyright:  1992-2017 by Apple Inc., all rights reserved.

*/
#ifndef __SPEECHRECOGNITION__
#define __SPEECHRECOGNITION__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#include <stdint.h>


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/* Error Codes [Speech recognition gets -5100 through -5199] */
enum {
  kSRNotAvailable               = -5100, /* the service requested is not avail or applicable */
  kSRInternalError              = -5101, /* a system internal or hardware error condition */
  kSRComponentNotFound          = -5102, /* a needed system resource was not located */
  kSROutOfMemory                = -5103, /* an out of memory error occurred in the toolbox memory space */
  kSRNotASpeechObject           = -5104, /* the object specified is no longer or never was valid */
  kSRBadParameter               = -5105, /* an invalid parameter was specified */
  kSRParamOutOfRange            = -5106, /* when we say 0-100, don't pass in 101. */
  kSRBadSelector                = -5107, /* an unrecognized selector was specified */
  kSRBufferTooSmall             = -5108, /* returned from attribute access functions */
  kSRNotARecSystem              = -5109, /* the object used was not a SRRecognitionSystem */
  kSRFeedbackNotAvail           = -5110, /* there is no feedback window associated with SRRecognizer */
  kSRCantSetProperty            = -5111, /* a non-settable property was specified */
  kSRCantGetProperty            = -5112, /* a non-gettable property was specified */
  kSRCantSetDuringRecognition   = -5113, /* the property can't be set while recognition is in progress -- do before or between utterances. */
  kSRAlreadyListening           = -5114, /* in response to SRStartListening */
  kSRNotListeningState          = -5115, /* in response to SRStopListening */
  kSRModelMismatch              = -5116, /* no acoustical models are avail to match request */
  kSRNoClientLanguageModel      = -5117, /* trying to access a non-specified SRLanguageModel */
  kSRNoPendingUtterances        = -5118, /* nothing to continue search on */
  kSRRecognitionCanceled        = -5119, /* an abort error occurred during search */
  kSRRecognitionDone            = -5120, /* search has finished, but nothing was recognized */
  kSROtherRecAlreadyModal       = -5121, /* another recognizer is modal at the moment, so can't set this recognizer's kSRBlockModally property right now */
  kSRHasNoSubItems              = -5122, /* SRCountItems or related routine was called on an object without subelements -- e.g. a word -- rather than phrase, path, or LM. */
  kSRSubItemNotFound            = -5123, /* returned when accessing a non-existent sub item of a container */
  kSRLanguageModelTooBig        = -5124, /* Cant build language models so big */
  kSRAlreadyReleased            = -5125, /* this object has already been released before */
  kSRAlreadyFinished            = -5126, /* the language model can't be finished twice */
  kSRWordNotFound               = -5127, /* the spelling couldn't be found in lookup(s) */
  kSRNotFinishedWithRejection   = -5128, /* property not found because the LMObj is not finished with rejection */
  kSRExpansionTooDeep           = -5129, /* Language model is left recursive or is embedded too many levels */
  kSRTooManyElements            = -5130, /* Too many elements added to phrase or path or other langauge model object */
  kSRCantAdd                    = -5131, /* Can't add given type of object to the base SRLanguageObject (e.g.in SRAddLanguageObject)   */
  kSRSndInSourceDisconnected    = -5132, /* Sound input source is disconnected */
  kSRCantReadLanguageObject     = -5133, /* An error while trying to create new Language object from file or pointer -- possibly bad format */
                                        /* non-release debugging error codes are included here */
  kSRNotImplementedYet          = -5199 /* you'd better wait for this feature in a future release */
};


/* Type Definitions */
typedef struct OpaqueSRSpeechObject*    SRSpeechObject;
typedef SRSpeechObject                  SRRecognitionSystem;
typedef SRSpeechObject                  SRRecognizer;
typedef SRSpeechObject                  SRSpeechSource;
typedef SRSpeechSource                  SRRecognitionResult;
typedef SRSpeechObject                  SRLanguageObject;
typedef SRLanguageObject                SRLanguageModel;
typedef SRLanguageObject                SRPath;
typedef SRLanguageObject                SRPhrase;
typedef SRLanguageObject                SRWord;
/* between 0 and 100 */
typedef UInt16                          SRSpeedSetting;
/* between 0 and 100 */
typedef UInt16                          SRRejectionLevel;
/* When an event occurs, the user supplied proc will be called with a pointer   */
/*  to the param passed in and a flag to indicate conditions such               */
/*  as interrupt time or system background time.                                */
struct SRCallBackStruct {
  UInt32              what;                   /* one of notification flags */
  long                message;                /* contains SRRecognitionResult id (32 / 64 bits) */
  SRRecognizer        instance;               /* ID of recognizer being notified */
  OSErr               status;                 /* result status of last search */
  SInt16              flags;                  /* non-zero if occurs during interrupt */
  SRefCon             refCon;                 /* user defined - set from SRCallBackParam */
};
typedef struct SRCallBackStruct         SRCallBackStruct;
/* Call back procedure definition */
typedef CALLBACK_API( void , SRCallBackProcPtr )(SRCallBackStruct * param);
typedef STACK_UPP_TYPE(SRCallBackProcPtr)                       SRCallBackUPP;
/*
 *  NewSRCallBackUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern SRCallBackUPP
NewSRCallBackUPP(SRCallBackProcPtr userRoutine)               __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");

/*
 *  DisposeSRCallBackUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeSRCallBackUPP(SRCallBackUPP userUPP)                   __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");

/*
 *  InvokeSRCallBackUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeSRCallBackUPP(
  SRCallBackStruct *  param,
  SRCallBackUPP       userUPP)                                __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");

#if __MACH__
  #ifdef __cplusplus
    inline SRCallBackUPP                                        NewSRCallBackUPP(SRCallBackProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeSRCallBackUPP(SRCallBackUPP) { }
    inline void                                                 InvokeSRCallBackUPP(SRCallBackStruct * param, SRCallBackUPP userUPP) { (*userUPP)(param); }
  #else
    #define NewSRCallBackUPP(userRoutine)                       ((SRCallBackUPP)userRoutine)
    #define DisposeSRCallBackUPP(userUPP)
    #define InvokeSRCallBackUPP(param, userUPP)                 (*userUPP)(param)
  #endif
#endif

struct SRCallBackParam {
  SRCallBackUPP       callBack;
  SRefCon             refCon;
};
typedef struct SRCallBackParam          SRCallBackParam;
/* Recognition System Types */
enum {
  kSRDefaultRecognitionSystemID = 0
};

/* Recognition System Properties */
enum {
  kSRFeedbackAndListeningModes  = 'fbwn', /* short: one of kSRNoFeedbackHasListenModes, kSRHasFeedbackHasListenModes, kSRNoFeedbackNoListenModes */
  kSRRejectedWord               = 'rejq', /* the SRWord used to represent a rejection */
  kSRCleanupOnClientExit        = 'clup' /* Boolean: Default is true. The rec system and everything it owns is disposed when the client application quits */
};

enum {
  kSRNoFeedbackNoListenModes    = 0,    /* next allocated recognizer has no feedback window and doesn't use listening modes   */
  kSRHasFeedbackHasListenModes  = 1,    /* next allocated recognizer has feedback window and uses listening modes          */
  kSRNoFeedbackHasListenModes   = 2     /* next allocated recognizer has no feedback window but does use listening modes  */
};

/* Speech Source Types */
enum {
  kSRDefaultSpeechSource        = 0,
  kSRLiveDesktopSpeechSource    = 'dklv', /* live desktop sound input */
  kSRCanned22kHzSpeechSource    = 'ca22' /* AIFF file based 16 bit, 22.050 KHz sound input */
};

/* Notification via Apple Event or Callback */
/* Notification Flags */
enum {
  kSRNotifyRecognitionBeginning = 1L << 0, /* recognition can begin. client must now call SRContinueRecognition or SRCancelRecognition */
  kSRNotifyRecognitionDone      = 1L << 1 /* recognition has terminated. result (if any) is available. */
};

/* Apple Event selectors */
/* AppleEvent message class  */
enum {
  kAESpeechSuite                = 'sprc'
};

/* AppleEvent message event ids */
enum {
  kAESpeechDone                 = 'srsd',
  kAESpeechDetected             = 'srbd'
};

/* AppleEvent Parameter ids */
enum {
  keySRRecognizer               = 'krec',
  keySRSpeechResult             = 'kspr',
  keySRSpeechStatus             = 'ksst'
};

/* AppleEvent Parameter types */
enum {
  typeSRRecognizer              = 'trec',
  typeSRSpeechResult            = 'tspr'
};


/* SRRecognizer Properties */
enum {
  kSRNotificationParam          = 'noti', /* SInt32: See notification flags below */
  kSRCallBackParam              = 'call', /* type SRCallBackParam */
  kSRSearchStatusParam          = 'stat', /* SInt32: see status flags below */
  kSRAutoFinishingParam         = 'afin', /* SInt32: Automatic finishing applied on LM for search */
  kSRForegroundOnly             = 'fgon', /* Boolean: Default is true. If true, client recognizer only active when in foreground.   */
  kSRBlockBackground            = 'blbg', /* Boolean: Default is false. If true, when client recognizer in foreground, rest of LMs are inactive.    */
  kSRBlockModally               = 'blmd', /* Boolean: Default is false. When true, this client's LM is only active LM; all other LMs are inactive. Be nice, don't be modal for long periods! */
  kSRWantsResultTextDrawn       = 'txfb', /* Boolean: Default is true. If true, search results are posted to Feedback window */
  kSRWantsAutoFBGestures        = 'dfbr', /* Boolean: Default is true. If true, client needn't call SRProcessBegin/End to get default feedback behavior */
  kSRSoundInVolume              = 'volu', /* short in [0..100] log scaled sound input power. Can't set this property */
  kSRReadAudioFSSpec            = 'aurd', /* *FSSpec: Specify FSSpec where raw audio is to be read (AIFF format) using kSRCanned22kHzSpeechSource. Reads until EOF */
  kSRReadAudioURL               = 'aurl', /* CFURLRef: Specify CFURLRef where raw audio is to be read (AIFF format) using kSRCanned22kHzSpeechSource. Reads until EOF */
  kSRCancelOnSoundOut           = 'caso', /* Boolean: Default is true.  If any sound is played out during utterance, recognition is aborted. */
  kSRSpeedVsAccuracyParam       = 'sped' /* SRSpeedSetting between 0 and 100 */
};

/* 0 means more accurate but slower. */
/* 100 means (much) less accurate but faster. */
enum {
  kSRUseToggleListen            = 0,    /* listen key modes */
  kSRUsePushToTalk              = 1
};

enum {
  kSRListenKeyMode              = 'lkmd', /* short: either kSRUseToggleListen or kSRUsePushToTalk */
  kSRListenKeyCombo             = 'lkey', /* short: Push-To-Talk key combination; high byte is high byte of event->modifiers, the low byte is the keycode from event->message */
  kSRListenKeyName              = 'lnam', /* Str63: string representing ListenKeyCombo */
  kSRKeyWord                    = 'kwrd', /* Str255: keyword preceding spoken commands in kSRUseToggleListen mode */
  kSRKeyExpected                = 'kexp' /* Boolean: Must the PTT key be depressed or the key word spoken before recognition can occur? */
};

/* Operational Status Flags */
enum {
  kSRIdleRecognizer             = 1L << 0, /* engine is not active */
  kSRSearchInProgress           = 1L << 1, /* search is in progress */
  kSRSearchWaitForAllClients    = 1L << 2, /* search is suspended waiting on all clients' input */
  kSRMustCancelSearch           = 1L << 3, /* something has occurred (sound played, non-speech detected) requiring the search to abort */
  kSRPendingSearch              = 1L << 4 /* we're about to start searching */
};

/* Recognition Result Properties */
enum {
  kSRTEXTFormat                 = 'TEXT', /* raw text in user supplied memory */
  kSRPhraseFormat               = 'lmph', /* SRPhrase containing result words */
  kSRPathFormat                 = 'lmpt', /* SRPath containing result phrases or words */
  kSRLanguageModelFormat        = 'lmfm' /* top level SRLanguageModel for post parse */
};

/* SRLanguageObject Family Properties */
enum {
  kSRSpelling                   = 'spel', /* spelling of a SRWord or SRPhrase or SRPath, or name of a SRLanguageModel */
  kSRLMObjType                  = 'lmtp', /* Returns one of SRLanguageObject Types listed below */
  kSRRefCon                     = 'refc', /* long (4/8 bytes) for user storage */
  kSROptional                   = 'optl', /* Boolean -- true if SRLanguageObject is optional    */
  kSREnabled                    = 'enbl', /* Boolean -- true if SRLanguageObject enabled */
  kSRRepeatable                 = 'rptb', /* Boolean -- true if SRLanguageObject is repeatable */
  kSRRejectable                 = 'rjbl', /* Boolean -- true if SRLanguageObject is rejectable (Recognition System's kSRRejectedWord */
                                        /*       object can be returned in place of SRLanguageObject with this property)   */
  kSRRejectionLevel             = 'rjct' /* SRRejectionLevel between 0 and 100 */
};

/* LM Object Types -- returned as kSRLMObjType property of language model objects */
enum {
  kSRLanguageModelType          = 'lmob', /* SRLanguageModel */
  kSRPathType                   = 'path', /* SRPath */
  kSRPhraseType                 = 'phra', /* SRPhrase */
  kSRWordType                   = 'word' /* SRWord */
};

/* a normal and reasonable rejection level */
enum {
  kSRDefaultRejectionLevel      = 50
};

/********************************************************************************/
/*                      NOTES ON USING THE API                                  */
/*                                                                              */
/*      All operations (with the exception of SRGetRecognitionSystem) are       */
/*      directed toward an object allocated or begot from New, Get and Read     */
/*      type calls.                                                             */
/*                                                                              */
/*      There is a simple rule in dealing with allocation and disposal:         */
/*                                                                              */
/*      *   all toolbox allocations are obtained from a SRRecognitionSystem     */
/*                                                                              */
/*      *   if you obtain an object via New or Get, then you own a reference    */
/*          to that object and it must be released via SRReleaseObject when     */
/*          you no longer need it                                               */
/*                                                                              */
/*      *   when you receive a SRRecognitionResult object via AppleEvent or     */
/*          callback, it has essentially been created on your behalf and so     */
/*          you are responsible for releasing it as above                       */
/*                                                                              */
/*      *   when you close a SRRecognitionSystem, all remaining objects which   */
/*          were allocated with it will be forcefully released and any          */
/*          remaining references to those objects will be invalid.              */
/*                                                                              */
/*      This translates into a very simple guideline:                           */
/*          If you allocate it or have it allocated for you, you must release   */
/*          it.  If you are only peeking at it, then don't release it.          */
/*                                                                              */
/********************************************************************************/
/* Opening and Closing of the SRRecognitionSystem */
/*
 *  SROpenRecognitionSystem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SROpenRecognitionSystem(
  SRRecognitionSystem *  system,
  OSType                 systemID)                            __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");



/*
 *  SRCloseRecognitionSystem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRCloseRecognitionSystem(SRRecognitionSystem system)          __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/* Accessing Properties of any Speech Object */
/*
 *  SRSetProperty()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRSetProperty(
  SRSpeechObject   srObject,
  OSType           selector,
  const void *     property,
  Size             propertyLen)                               __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRGetProperty()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRGetProperty(
  SRSpeechObject   srObject,
  OSType           selector,
  void *           property,
  Size *           propertyLen)                               __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/* Any object obtained via New or Get type calls must be released */
/*
 *  SRReleaseObject()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRReleaseObject(SRSpeechObject srObject)                      __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRGetReference()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRGetReference(
  SRSpeechObject    srObject,
  SRSpeechObject *  newObjectRef)                             __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/* SRRecognizer Instance Functions */
/*
 *  SRNewRecognizer()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRNewRecognizer(
  SRRecognitionSystem   system,
  SRRecognizer *        recognizer,
  OSType                sourceID)                             __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRStartListening()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRStartListening(SRRecognizer recognizer)                     __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRStopListening()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRStopListening(SRRecognizer recognizer)                      __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRSetLanguageModel()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRSetLanguageModel(
  SRRecognizer      recognizer,
  SRLanguageModel   languageModel)                            __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRGetLanguageModel()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRGetLanguageModel(
  SRRecognizer       recognizer,
  SRLanguageModel *  languageModel)                           __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRContinueRecognition()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRContinueRecognition(SRRecognizer recognizer)                __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRCancelRecognition()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRCancelRecognition(SRRecognizer recognizer)                  __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRIdle()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRIdle(void)                                                  __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/* Language Model Building and Manipulation Functions */
/*
 *  SRNewLanguageModel()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRNewLanguageModel(
  SRRecognitionSystem   system,
  SRLanguageModel *     model,
  const void *          name,
  SInt32                nameLength)                           __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRNewPath()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRNewPath(
  SRRecognitionSystem   system,
  SRPath *              path)                                 __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRNewPhrase()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRNewPhrase(
  SRRecognitionSystem   system,
  SRPhrase *            phrase,
  const void *          text,
  SInt32                textLength)                           __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRNewWord()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRNewWord(
  SRRecognitionSystem   system,
  SRWord *              word,
  const void *          text,
  SInt32                textLength)                           __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/* Operations on any object of the SRLanguageObject family */
/*
 *  SRPutLanguageObjectIntoHandle()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRPutLanguageObjectIntoHandle(
  SRLanguageObject   languageObject,
  Handle             lobjHandle)                              __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRPutLanguageObjectIntoDataFile()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRPutLanguageObjectIntoDataFile(
  SRLanguageObject   languageObject,
  short              fRefNum)                                 __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRNewLanguageObjectFromHandle()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRNewLanguageObjectFromHandle(
  SRRecognitionSystem   system,
  SRLanguageObject *    languageObject,
  Handle                lObjHandle)                           __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRNewLanguageObjectFromDataFile()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRNewLanguageObjectFromDataFile(
  SRRecognitionSystem   system,
  SRLanguageObject *    languageObject,
  short                 fRefNum)                              __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SREmptyLanguageObject()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SREmptyLanguageObject(SRLanguageObject languageObject)        __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRChangeLanguageObject()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRChangeLanguageObject(
  SRLanguageObject   languageObject,
  const void *       text,
  SInt32             textLength)                              __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRAddLanguageObject()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRAddLanguageObject(
  SRLanguageObject   base,
  SRLanguageObject   addon)                                   __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRAddText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRAddText(
  SRLanguageObject   base,
  const void *       text,
  SInt32             textLength,
  SRefCon            refCon)                                  __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRRemoveLanguageObject()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRRemoveLanguageObject(
  SRLanguageObject   base,
  SRLanguageObject   toRemove)                                __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/* Traversing SRRecognitionResults or SRLanguageObjects */
/*
 *  SRCountItems()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRCountItems(
  SRSpeechObject   container,
  long *           count)                                     __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRGetIndexedItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRGetIndexedItem(
  SRSpeechObject    container,
  SRSpeechObject *  item,
  long              index)                                    __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRSetIndexedItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRSetIndexedItem(
  SRSpeechObject   container,
  SRSpeechObject   item,
  long             index)                                     __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRRemoveIndexedItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRRemoveIndexedItem(
  SRSpeechObject   container,
  long             index)                                     __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/* Utilizing the System Feedback Window */
/*
 *  SRDrawText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRDrawText(
  SRRecognizer   recognizer,
  const void *   dispText,
  SInt32         dispLength)                                  __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRDrawRecognizedText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRDrawRecognizedText(
  SRRecognizer   recognizer,
  const void *   dispText,
  SInt32         dispLength)                                  __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRSpeakText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRSpeakText(
  SRRecognizer   recognizer,
  const void *   speakText,
  SInt32         speakLength)                                 __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRSpeakAndDrawText()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRSpeakAndDrawText(
  SRRecognizer   recognizer,
  const void *   text,
  SInt32         textLength)                                  __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRStopSpeech()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr 
SRStopSpeech(SRRecognizer recognizer)                         __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRSpeechBusy()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern Boolean 
SRSpeechBusy(SRRecognizer recognizer)                         __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRProcessBegin()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRProcessBegin(
  SRRecognizer   recognizer,
  Boolean        failed)                                      __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");


/*
 *  SRProcessEnd()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in SpeechRecognitionLib 1.0 and later
 */
extern OSErr
SRProcessEnd(
  SRRecognizer   recognizer,
  Boolean        failed)                                      __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_13, __IPHONE_NA, __IPHONE_NA,
                                                                                                 "Please use NSSpeechRecognizer class instead.");





#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __SPEECHRECOGNITION__ */

