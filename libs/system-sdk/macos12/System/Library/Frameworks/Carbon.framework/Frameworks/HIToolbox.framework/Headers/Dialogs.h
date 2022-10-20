/*
     File:       HIToolbox/Dialogs.h
 
     Contains:   Dialog Manager interfaces.
 
     Copyright:  © 1985-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DIALOGS__
#define __DIALOGS__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif

#ifndef __MACWINDOWS__
#include <HIToolbox/MacWindows.h>
#endif

#ifndef __TEXTEDIT__
#include <HIToolbox/TextEdit.h>
#endif

#ifndef __CONTROLS__
#include <HIToolbox/Controls.h>
#endif

#ifndef __CARBONEVENTS__
#include <HIToolbox/CarbonEvents.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

enum {
                                        /* new, more standard names for dialog item types*/
  kControlDialogItem            = 4,
  kButtonDialogItem             = kControlDialogItem | 0,
  kCheckBoxDialogItem           = kControlDialogItem | 1,
  kRadioButtonDialogItem        = kControlDialogItem | 2,
  kResourceControlDialogItem    = kControlDialogItem | 3,
  kStaticTextDialogItem         = 8,
  kEditTextDialogItem           = 16,
  kIconDialogItem               = 32,
  kPictureDialogItem            = 64,
  kUserDialogItem               = 0,
  kHelpDialogItem               = 1,
  kItemDisableBit               = 128
};

enum {
                                        /* old names for dialog item types*/
  ctrlItem                      = 4,
  btnCtrl                       = 0,
  chkCtrl                       = 1,
  radCtrl                       = 2,
  resCtrl                       = 3,
  statText                      = 8,
  editText                      = 16,
  iconItem                      = 32,
  picItem                       = 64,
  userItem                      = 0,
  itemDisable                   = 128
};

enum {
                                        /* standard dialog item numbers*/
  kStdOkItemIndex               = 1,
  kStdCancelItemIndex           = 2,    /* old names*/
  ok                            = kStdOkItemIndex,
  cancel                        = kStdCancelItemIndex
};

enum {
                                        /* standard icon resource id's    */
  kStopIcon                     = 0,
  kNoteIcon                     = 1,
  kCautionIcon                  = 2,    /* old names*/
  stopIcon                      = kStopIcon,
  noteIcon                      = kNoteIcon,
  cautionIcon                   = kCautionIcon
};




#if OLDROUTINENAMES
/*
   These constants lived briefly on ETO 16.  They suggest
   that there is only one index you can use for the OK 
   item, which is not true.  You can put the ok item 
   anywhere you want in the DITL.
*/
enum {
  kOkItemIndex                  = 1,
  kCancelItemIndex              = 2
};

#endif  /* OLDROUTINENAMES */

/*  Dialog Item List Manipulation Constants */
typedef SInt16                          DITLMethod;
enum {
  overlayDITL                   = 0,
  appendDITLRight               = 1,
  appendDITLBottom              = 2
};

typedef SInt16                          StageList;
/* DialogPtr is obsolete. Use DialogRef instead.*/
typedef DialogPtr                       DialogRef;
struct DialogTemplate {
  Rect                boundsRect;
  SInt16              procID;
  Boolean             visible;
  Boolean             filler1;
  Boolean             goAwayFlag;
  Boolean             filler2;
  SInt32              refCon;
  SInt16              itemsID;
  Str255              title;
};
typedef struct DialogTemplate           DialogTemplate;
typedef DialogTemplate *                DialogTPtr;
typedef DialogTPtr *                    DialogTHndl;
struct AlertTemplate {
  Rect                boundsRect;
  SInt16              itemsID;
  StageList           stages;
};
typedef struct AlertTemplate            AlertTemplate;
typedef AlertTemplate *                 AlertTPtr;
typedef AlertTPtr *                     AlertTHndl;
/* new type abstractions for the dialog manager */
typedef SInt16                          DialogItemIndexZeroBased;
typedef SInt16                          DialogItemIndex;
typedef SInt16                          DialogItemType;
/* dialog manager callbacks */
typedef CALLBACK_API( void , SoundProcPtr )(SInt16 soundNumber);
typedef CALLBACK_API( Boolean , ModalFilterProcPtr )(DialogRef theDialog, EventRecord *theEvent, DialogItemIndex *itemHit);
/* ModalFilterYDProcPtr was previously in StandardFile.h */
typedef CALLBACK_API( Boolean , ModalFilterYDProcPtr )(DialogRef theDialog, EventRecord *theEvent, short *itemHit, void *yourDataPtr);
typedef CALLBACK_API( void , UserItemProcPtr )(DialogRef theDialog, DialogItemIndex itemNo);
typedef STACK_UPP_TYPE(SoundProcPtr)                            SoundUPP;
typedef STACK_UPP_TYPE(ModalFilterProcPtr)                      ModalFilterUPP;
typedef STACK_UPP_TYPE(ModalFilterYDProcPtr)                    ModalFilterYDUPP;
typedef STACK_UPP_TYPE(UserItemProcPtr)                         UserItemUPP;
/*
 *  NewSoundUPP()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */

/*
 *  NewModalFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ModalFilterUPP
NewModalFilterUPP(ModalFilterProcPtr userRoutine)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewModalFilterYDUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ModalFilterYDUPP
NewModalFilterYDUPP(ModalFilterYDProcPtr userRoutine)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewUserItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern UserItemUPP
NewUserItemUPP(UserItemProcPtr userRoutine)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeSoundUPP()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */

/*
 *  DisposeModalFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeModalFilterUPP(ModalFilterUPP userUPP)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeModalFilterYDUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeModalFilterYDUPP(ModalFilterYDUPP userUPP)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeUserItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeUserItemUPP(UserItemUPP userUPP)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeSoundUPP()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */

/*
 *  InvokeModalFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeModalFilterUPP(
  DialogRef          theDialog,
  EventRecord *      theEvent,
  DialogItemIndex *  itemHit,
  ModalFilterUPP     userUPP)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeModalFilterYDUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeModalFilterYDUPP(
  DialogRef         theDialog,
  EventRecord *     theEvent,
  short *           itemHit,
  void *            yourDataPtr,
  ModalFilterYDUPP  userUPP)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeUserItemUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeUserItemUPP(
  DialogRef        theDialog,
  DialogItemIndex  itemNo,
  UserItemUPP      userUPP)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline ModalFilterUPP                                       NewModalFilterUPP(ModalFilterProcPtr userRoutine) { return userRoutine; }
    inline ModalFilterYDUPP                                     NewModalFilterYDUPP(ModalFilterYDProcPtr userRoutine) { return userRoutine; }
    inline UserItemUPP                                          NewUserItemUPP(UserItemProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeModalFilterUPP(ModalFilterUPP) { }
    inline void                                                 DisposeModalFilterYDUPP(ModalFilterYDUPP) { }
    inline void                                                 DisposeUserItemUPP(UserItemUPP) { }
    inline Boolean                                              InvokeModalFilterUPP(DialogRef theDialog, EventRecord * theEvent, DialogItemIndex * itemHit, ModalFilterUPP userUPP) { return (*userUPP)(theDialog, theEvent, itemHit); }
    inline Boolean                                              InvokeModalFilterYDUPP(DialogRef theDialog, EventRecord * theEvent, short * itemHit, void * yourDataPtr, ModalFilterYDUPP userUPP) { return (*userUPP)(theDialog, theEvent, itemHit, yourDataPtr); }
    inline void                                                 InvokeUserItemUPP(DialogRef theDialog, DialogItemIndex itemNo, UserItemUPP userUPP) { (*userUPP)(theDialog, itemNo); }
  #else
    #define NewModalFilterUPP(userRoutine)                      ((ModalFilterUPP)userRoutine)
    #define NewModalFilterYDUPP(userRoutine)                    ((ModalFilterYDUPP)userRoutine)
    #define NewUserItemUPP(userRoutine)                         ((UserItemUPP)userRoutine)
    #define DisposeModalFilterUPP(userUPP)
    #define DisposeModalFilterYDUPP(userUPP)
    #define DisposeUserItemUPP(userUPP)
    #define InvokeModalFilterUPP(theDialog, theEvent, itemHit, userUPP) (*userUPP)(theDialog, theEvent, itemHit)
    #define InvokeModalFilterYDUPP(theDialog, theEvent, itemHit, yourDataPtr, userUPP) (*userUPP)(theDialog, theEvent, itemHit, yourDataPtr)
    #define InvokeUserItemUPP(theDialog, itemNo, userUPP)       (*userUPP)(theDialog, itemNo)
  #endif
#endif


/*
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
    • Following types are valid with Appearance 1.0 and later
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
*/
enum {
                                        /* Alert types to pass into StandardAlert */
  kAlertStopAlert               = 0,
  kAlertNoteAlert               = 1,
  kAlertCautionAlert            = 2,
  kAlertPlainAlert              = 3
};

typedef SInt16                          AlertType;
enum {
  kAlertDefaultOKText           = -1,   /* "OK"*/
  kAlertDefaultCancelText       = -1,   /* "Cancel"*/
  kAlertDefaultOtherText        = -1    /* "Don't Save"*/
};

/* StandardAlert alert button numbers */
enum {
  kAlertStdAlertOKButton        = 1,
  kAlertStdAlertCancelButton    = 2,
  kAlertStdAlertOtherButton     = 3,
  kAlertStdAlertHelpButton      = 4
};


/*
 *  Dialog feature flags
 *  
 *  Summary:
 *    These feature flags can be used in a 'dlgx' resource or in the
 *    inFlags parameter to NewFeaturesDialog.
 */
enum {

  /*
   * Requests the Dialog Manager to set the dialog box’s background
   * color or pattern, and to use Appearance Manager APIs to draw the
   * dialog frame.
   */
  kDialogFlagsUseThemeBackground = (1 << 0),

  /*
   * Requests the Dialog Manager to create a root control in the dialog
   * window and establish a control embedding hierarchy. Any dialog
   * items become controls once the embedding hierarchy is established.
   */
  kDialogFlagsUseControlHierarchy = (1 << 1),

  /*
   * Requests the Dialog Manager to automatically handle movable modal
   * behavior such as dragging a dialog window by its title bar.
   */
  kDialogFlagsHandleMovableModal = (1 << 2),

  /*
   * On Mac OS 8.x and 9.x, requests the Dialog Manager to create
   * Appearance-compliant controls in the dialog window. Otherwise,
   * push buttons, checkboxes, and radio buttons are displayed in their
   * pre-Appearance forms when systemwide Appearance is off. On Mac OS
   * X, this flag is ignored; dialogs always use controls to implement
   * standard dialog items.
   */
  kDialogFlagsUseThemeControls  = (1 << 3),

  /*
   * Requests the Dialog Manager to create a compositing window to
   * contain the dialog items. The window will also use the standard
   * window event handler and will have live resize enabled. Available
   * in Mac OS X 10.5 and later.
   */
  kDialogFlagsUseCompositing    = (1 << 8)
};


/*
 *  Alert feature flags
 *  
 *  Summary:
 *    These feature flags can be used in a 'alrx' resource.
 */
enum {
                                        /* Alert Flags for use in alrx resource */

  /*
   * Requests the Dialog Manager to set the alert’s background color or
   * pattern, and to use Appearance Manager APIs to draw the alert
   * frame.
   */
  kAlertFlagsUseThemeBackground = (1 << 0),

  /*
   * Requests the Dialog Manager to create a root control in the alert
   * window and establish a control embedding hierarchy. Any alert
   * items become controls once the embedding hierarchy is established.
   */
  kAlertFlagsUseControlHierarchy = (1 << 1),

  /*
   * Requests the Dialog Manager to make the alert window movable, and
   * to automatically handle dragging the alert by its title bar.
   */
  kAlertFlagsAlertIsMovable     = (1 << 2),

  /*
   * On Mac OS 8.x and 9.x, requests the Dialog Manager to create
   * Appearance-compliant controls in the alert window. Otherwise, push
   * buttons, checkboxes, and radio buttons are displayed in their
   * pre-Appearance forms when systemwide Appearance is off. On Mac OS
   * X, this flag is ignored; alerts always use controls to implement
   * standard alert items.
   */
  kAlertFlagsUseThemeControls   = (1 << 3),

  /*
   * Requests the Dialog Manager to create a compositing window to
   * contain the alert items. The window will also use the standard
   * window event handler. Available in Mac OS X 10.5 and later.
   */
  kAlertFlagsUseCompositing     = (1 << 8)
};

/* For dftb resource */
enum {
  kDialogFontNoFontStyle        = 0,
  kDialogFontUseFontMask        = 0x0001,
  kDialogFontUseFaceMask        = 0x0002,
  kDialogFontUseSizeMask        = 0x0004,
  kDialogFontUseForeColorMask   = 0x0008,
  kDialogFontUseBackColorMask   = 0x0010,
  kDialogFontUseModeMask        = 0x0020,
  kDialogFontUseJustMask        = 0x0040,
  kDialogFontUseAllMask         = 0x00FF,
  kDialogFontAddFontSizeMask    = 0x0100,
  kDialogFontUseFontNameMask    = 0x0200,
  kDialogFontAddToMetaFontMask  = 0x0400
};

/* Also for dftb resource. This one is available in Mac OS X or later. */
/* It corresponds directly to kControlUseThemeFontIDMask from Controls.h. */
enum {
  kDialogFontUseThemeFontIDMask = 0x0080
};


/*
 *  AlertStdAlertParamRec
 *  
 *  Summary:
 *    AlertStdCFStringAlertParamRec is preferred.
 *  
 *  Discussion:
 *    As of Mac OS X 10.5, defaultButton can be the same as the
 *    cancelButton. Prior to Mac OS X 10.5, having cancelButton and
 *    defaultButton the same will cause a paramErr.
 */
struct AlertStdAlertParamRec {

  /*
   * Is the alert movable? This parameter is ignored on Mac OS X; all
   * standard alerts are movable.
   */
  Boolean             movable;

  /*
   * Is there a help button?
   */
  Boolean             helpButton;

  /*
   * The event filter to be used.
   */
  ModalFilterUPP      filterProc;

  /*
   * Text for the button in the OK position. kAlertDefaultOKText can be
   * used for the system standard text.
   */
  ConstStringPtr      defaultText;

  /*
   * Text for the button in the Cancel position.
   * kAlertDefaultCancelText can be used for the system standard text.
   */
  ConstStringPtr      cancelText;

  /*
   * Text for the button in the leftmost position.
   * kAlertDefaultOtherText can be used for the system standard text.
   */
  ConstStringPtr      otherText;

  /*
   * Which button is default, i.e. activated by pressing Return. Prior
   * to Mac OS X 10.5, this field must be set to a non-zero value in
   * the range kAlertStdAlertOKButton..kAlertStdAlertOtherButton, and
   * your alert will always have a default button. In Mac OS X 10.5 and
   * later, you may set this field to zero to request no default button.
   */
  SInt16              defaultButton;

  /*
   * Which button is default for cancelling, i.e. activated by pressing
   * Escape. May be zero to request no cancel button; otherwise, must
   * be in the range kAlertStdAlertOKButton..kAlertStdAlertOtherButton.
   */
  SInt16              cancelButton;

  /*
   * Where to position the alert. kWindowDefaultPosition is the default
   * and is equal to kWindowAlertPositionParentWindowScreen.
   */
  UInt16              position;
};
typedef struct AlertStdAlertParamRec    AlertStdAlertParamRec;
typedef AlertStdAlertParamRec *         AlertStdAlertParamPtr;
enum {
  kHICommandOther               = 'othr' /* sent by standard sheet dialogs when the "other" button is pressed */
};


/*
 */
enum {

  /*
   * AlertStdCFStringAlertParamRec version prior to Mac OS X 10.5.
   */
  kStdCFStringAlertVersionOne   = 1,

  /*
   * AlertStdCFStringAlertParamRec version for Mac OS X 10.5 and later.
   * When using this version, you must set the icon field of the
   * structure to NULL or a valid IconRef.
   */
  kStdCFStringAlertVersionTwo   = 2
};


/*
 *  Summary:
 *    Flags to CreateStandardAlert that are specified in the
 *    AlertStdCFStringAlertParamRec.flags field.
 */
enum {

  /*
   * Applies to StandardSheet only. Do not dispose of the sheet window
   * after closing it; allows the sheet to be re-used again in a later
   * call to ShowSheetWindow.
   */
  kStdAlertDoNotDisposeSheet    = 1 << 0,

  /*
   * Applies to StandardSheet only. Causes the sheet window to be
   * hidden immediately without animation effects when the default
   * button is chosen by the user.
   */
  kStdAlertDoNotAnimateOnDefault = 1 << 1,

  /*
   * Applies to StandardSheet only. Causes the sheet window to be
   * hidden immediately without animation effects when the cancel
   * button is chosen by the user.
   */
  kStdAlertDoNotAnimateOnCancel = 1 << 2,

  /*
   * Applies to StandardSheet only. Causes the sheet window to be
   * hidden immediately without animation effects when the other button
   * is chosen by the user.
   */
  kStdAlertDoNotAnimateOnOther  = 1 << 3,

  /*
   * Allows dialog to stay up even after clicking the Help button.
   * Normally, it would close immediately. It is not necessary to set
   * this option for sheets, as they merely send the HICommandHelp
   * command to the target provided. RunStandardAlert will return with
   * the help button item in the itemHit parameter, but the window will
   * remain up. You can then perform whatever help function you wish
   * and then call RunStandardAlert again. This option is available in
   * Mac OS X 10.4 or later.
   */
  kStdAlertDoNotCloseOnHelp     = 1 << 4
};


/*
 *  AlertStdCFStringAlertParamRec
 *  
 *  Summary:
 *    A CFString variant of AlertStdAlertParamRec. CFStrings are
 *    preferred.
 *  
 *  Discussion:
 *    As of Mac OS X 10.5, defaultButton can be the same as the
 *    cancelButton. Prior to Mac OS X 10.5, having cancelButton and
 *    defaultButton the same will cause a paramErr.
 */
struct AlertStdCFStringAlertParamRec {

  /*
   * The version of the structure. Prior to Mac OS X 10.5, you must use
   * kStdCFStringAlertVersionOne. In Mac OS X 10.5 or later, you may
   * use either VersionOne or VersionTwo. If you use VersionTwo, you
   * must set the icon field to NULL or a valid IconRef.
   */
  UInt32              version;

  /*
   * Is the alert movable? This parameter is ignored on Mac OS X; all
   * standard alerts are movable.
   */
  Boolean             movable;

  /*
   * Is there a help button?
   */
  Boolean             helpButton;

  /*
   * Text for the button in the OK position.
   */
  CFStringRef         defaultText;

  /*
   * Text for the button in the Cancel position.
   */
  CFStringRef         cancelText;

  /*
   * Text for the button in the leftmost/help position.
   */
  CFStringRef         otherText;

  /*
   * Which button is default, i.e. activated by pressing Return. Prior
   * to Mac OS X 10.5, this field must be set to a non-zero value in
   * the range kAlertStdAlertOKButton..kAlertStdAlertOtherButton, and
   * your alert will always have a default button. In Mac OS X 10.5 and
   * later, you may set this field to zero to request no default button.
   */
  SInt16              defaultButton;

  /*
   * Which button is default for cancelling, i.e. activated by pressing
   * Escape. May be zero to request no cancel button; otherwise, must
   * be in the range kAlertStdAlertOKButton..kAlertStdAlertOtherButton.
   */
  SInt16              cancelButton;

  /*
   * Where to position the alert. kWindowDefaultPosition is the default
   * and is equal to kWindowAlertPositionParentWindowScreen.
   */
  UInt16              position;

  /*
   * Options for the behavior of the alert or sheet.
   */
  OptionBits          flags;

  /*
   * Custom icon for display in the alert. Available on Mac OS X 10.5
   * and later when the version field is set to
   * kStdCFStringAlertVersionTwo.
   */
  IconRef             icon;
};
typedef struct AlertStdCFStringAlertParamRec AlertStdCFStringAlertParamRec;
typedef AlertStdCFStringAlertParamRec * AlertStdCFStringAlertParamPtr;
/* ——— end Appearance 1.0 or later stuff*/


#if !__LP64__
/*
 *  NewDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogRef 
NewDialog(
  void *             dStorage,
  const Rect *       boundsRect,
  ConstStr255Param   title,
  Boolean            visible,
  SInt16             procID,
  WindowRef          behind,
  Boolean            goAwayFlag,
  SRefCon            refCon,
  Handle             items)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetNewDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogRef 
GetNewDialog(
  SInt16      dialogID,
  void *      dStorage,
  WindowRef   behind)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  NewColorDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogRef 
NewColorDialog(
  void *             dStorage,
  const Rect *       boundsRect,
  ConstStr255Param   title,
  Boolean            visible,
  SInt16             procID,
  WindowRef          behind,
  Boolean            goAwayFlag,
  SRefCon            refCon,
  Handle             items)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DisposeDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DisposeDialog(DialogRef theDialog)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ModalDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ModalDialog(
  ModalFilterUPP     modalFilter,
  DialogItemIndex *  itemHit)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsDialogEvent()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
IsDialogEvent(const EventRecord * theEvent)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DialogSelect()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
DialogSelect(
  const EventRecord *  theEvent,
  DialogRef *          theDialog,
  DialogItemIndex *    itemHit)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DrawDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DrawDialog(DialogRef theDialog)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  UpdateDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
UpdateDialog(
  DialogRef   theDialog,
  RgnHandle   updateRgn)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HideDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HideDialogItem(
  DialogRef         theDialog,
  DialogItemIndex   itemNo)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ShowDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ShowDialogItem(
  DialogRef         theDialog,
  DialogItemIndex   itemNo)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FindDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogItemIndexZeroBased 
FindDialogItem(
  DialogRef   theDialog,
  Point       thePt)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  DialogCut()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DialogCut(DialogRef theDialog)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DialogPaste()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DialogPaste(DialogRef theDialog)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DialogCopy()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DialogCopy(DialogRef theDialog)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DialogDelete()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DialogDelete(DialogRef theDialog)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  Alert()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogItemIndex 
Alert(
  SInt16           alertID,
  ModalFilterUPP   modalFilter)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  StopAlert()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogItemIndex 
StopAlert(
  SInt16           alertID,
  ModalFilterUPP   modalFilter)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  NoteAlert()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogItemIndex 
NoteAlert(
  SInt16           alertID,
  ModalFilterUPP   modalFilter)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CautionAlert()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogItemIndex 
CautionAlert(
  SInt16           alertID,
  ModalFilterUPP   modalFilter)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetDialogItem(
  DialogRef         theDialog,
  DialogItemIndex   itemNo,
  DialogItemType *  itemType,
  Handle *          item,
  Rect *            box)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetDialogItem(
  DialogRef         theDialog,
  DialogItemIndex   itemNo,
  DialogItemType    itemType,
  Handle            item,
  const Rect *      box)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ParamText()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ParamText(
  ConstStr255Param   param0,
  ConstStr255Param   param1,
  ConstStr255Param   param2,
  ConstStr255Param   param3)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SelectDialogItemText()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SelectDialogItemText(
  DialogRef         theDialog,
  DialogItemIndex   itemNo,
  SInt16            strtSel,
  SInt16            endSel)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDialogItemText()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetDialogItemText(
  Handle   item,
  Str255   text)                                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDialogItemText()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetDialogItemText(
  Handle             item,
  ConstStr255Param   text)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetAlertStage()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SInt16 
GetAlertStage(void)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDialogFont()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetDialogFont(SInt16 fontNum)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ResetAlertStage()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ResetAlertStage(void)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* APIs in Carbon*/
/*
 *  GetParamText()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later
 */
extern void 
GetParamText(
  StringPtr   param0,
  StringPtr   param1,
  StringPtr   param2,
  StringPtr   param3)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  AppendDITL()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
AppendDITL(
  DialogRef    theDialog,
  Handle       theHandle,
  DITLMethod   method)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CountDITL()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern DialogItemIndex 
CountDITL(DialogRef theDialog)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ShortenDITL()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ShortenDITL(
  DialogRef         theDialog,
  DialogItemIndex   numberItems)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  InsertDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
InsertDialogItem(
  DialogRef         theDialog,
  DialogItemIndex   afterItem,
  DialogItemType    itemType,
  Handle            itemHandle,
  const Rect *      box)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RemoveDialogItems()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RemoveDialogItems(
  DialogRef         theDialog,
  DialogItemIndex   itemNo,
  DialogItemIndex   amountToRemove,
  Boolean           disposeItemData)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  StdFilterProc()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
StdFilterProc(
  DialogRef          theDialog,
  EventRecord *      event,
  DialogItemIndex *  itemHit)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  GetStdFilterProc()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetStdFilterProc(ModalFilterUPP * theProc)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDialogDefaultItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetDialogDefaultItem(
  DialogRef         theDialog,
  DialogItemIndex   newItem)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDialogCancelItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetDialogCancelItem(
  DialogRef         theDialog,
  DialogItemIndex   newItem)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDialogTracksCursor()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetDialogTracksCursor(
  DialogRef   theDialog,
  Boolean     tracks)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;




/*
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
    • Appearance Dialog Routines (available only with Appearance 1.0 and later)
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
*/

/*
 *  NewFeaturesDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern DialogRef 
NewFeaturesDialog(
  void *             inStorage,
  const Rect *       inBoundsRect,
  ConstStr255Param   inTitle,
  Boolean            inIsVisible,
  SInt16             inProcID,
  WindowRef          inBehind,
  Boolean            inGoAwayFlag,
  SRefCon            inRefCon,
  Handle             inItemListHandle,
  UInt32             inFlags)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  AutoSizeDialog()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
AutoSizeDialog(DialogRef inDialog)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
    Regarding StandardAlert and constness:
    Even though the inAlertParam parameter is marked const here, there was
    a chance Dialog Manager would modify it on versions of Mac OS prior to 9.
*/
/*
 *  StandardAlert()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
StandardAlert(
  AlertType                      inAlertType,
  ConstStr255Param               inError,
  ConstStr255Param               inExplanation,
  const AlertStdAlertParamRec *  inAlertParam,        /* can be NULL */
  SInt16 *                       outItemHit)                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* CFString-based StandardAlert and StandardSheet APIs are only available on Mac OS X and later*/

/*
 *  GetStandardAlertDefaultParams()
 *  
 *  Summary:
 *    Fills out an AlertStdCFStringAlertParamRec with default
 *    values:
 *    -   movable
 *    -   no help button
 *    -   default button with title kAlertDefaultOKText
 *    -   no cancel or other buttons
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    param:
 *      The parameter block to initialize.
 *    
 *    version:
 *      The parameter block version; pass kStdCFStringAlertVersionOne.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetStandardAlertDefaultParams(
  AlertStdCFStringAlertParamPtr   param,
  UInt32                          version)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CreateStandardAlert()
 *  
 *  Summary:
 *    Creates an alert containing standard elements and using standard
 *    formatting rules.
 *  
 *  Discussion:
 *    CreateStandardAlert should be used in conjunction with
 *    RunStandardAlert. After CreateStandardAlert returns, the alert is
 *    still invisible. RunStandardAlert will show the alert and run a
 *    modal dialog loop to process events in the alert. 
 *    
 *    The strings passed to this API in the error, explanation, and
 *    AlertStdCFStringAlertParamRec button title parameters will all be
 *    retained during the creation of the alert, and released when the
 *    alert is disposed by RunStandardAlert. There is no net change to
 *    the refcount of these strings across CreateStandardAlert and
 *    RunStandardAlert.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    alertType:
 *      The type of alert to create.
 *    
 *    error:
 *      The error string to display. CreateStandardAlert increments the
 *      refcount on this string, so you may release it after
 *      CreateStandardAlert returns if you don't need it later.
 *    
 *    explanation:
 *      The explanation string to display. May be NULL or empty to
 *      display no explanation. CreateStandardAlert increments the
 *      refcount on this string, so you may release it after
 *      CreateStandardAlert returns if you don't need it later.
 *    
 *    param:
 *      The parameter block describing how to create the alert. May be
 *      NULL. CreateStandardAlert increments the refcount on the button
 *      title strings in the parameter block, so you may release them
 *      after CreateStandardAlert returns if you don't need them later.
 *    
 *    outAlert:
 *      On exit, contains the new alert.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateStandardAlert(
  AlertType                              alertType,
  CFStringRef                            error,
  CFStringRef                            explanation,       /* can be NULL */
  const AlertStdCFStringAlertParamRec *  param,             /* can be NULL */
  DialogRef *                            outAlert)            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RunStandardAlert()
 *  
 *  Summary:
 *    Shows, runs, and destroys a standard alert using a modal dialog
 *    loop.
 *  
 *  Discussion:
 *    RunStandardAlert displays and runs an alert created by
 *    CreateStandardAlert. It handles all user interaction with the
 *    alert. After the user has dismissed the alert, RunStandardAlert
 *    destroys the alert dialog; the DialogRef will be invalid after
 *    RunStandardAlert returns. DO NOT call DisposeDialog. 
 *    
 *    NOTE: DO NOT call this function for a dialog that was not created
 *    with CreateStandardAlert! You will sorely regret it, I promise
 *    you.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inAlert:
 *      The alert to display.
 *    
 *    filterProc:
 *      An event filter function for handling events that do not apply
 *      to the alert. May be NULL.
 *    
 *    outItemHit:
 *      On exit, contains the item index of the button that was pressed
 *      to close the alert.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RunStandardAlert(
  DialogRef          inAlert,
  ModalFilterUPP     filterProc,       /* can be NULL */
  DialogItemIndex *  outItemHit)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CreateStandardSheet()
 *  
 *  Summary:
 *    Creates an alert containing standard elements and using standard
 *    formatting rules, and prepares it to be displayed as a sheet.
 *  
 *  Discussion:
 *    CreateStandardSheet should be used in conjunction with
 *    ShowSheetWindow. After CreateStandardSheet returns, the alert is
 *    still invisible. ShowSheetWindow will show the alert and then
 *    return. Events in the sheet are handled asynchronously; the
 *    application should be prepared for the sheet window to be part of
 *    its windowlist while running its own event loop. When a button in
 *    the sheet is pressed, the EventTargetRef passed to
 *    CreateStandardSheet will receive a command event with one of the
 *    command IDs kHICommandOK, kHICommandCancel, or kHICommandOther.
 *    The sheet is hidden and the sheet dialog destroyed before the
 *    command is sent; the caller does not have to call HideSheetWindow
 *    or DisposeDialog. 
 *    
 *    If the caller needs to destroy the sheet before showing it, then
 *    it is sufficient to call DisposeDialog on the sheet. This is the
 *    only case in which the caller would need to destroy the sheet
 *    explicitly. 
 *    
 *    The strings passed to this API in the error, explanation, and
 *    AlertStdCFStringAlertParamRec button title parameters will all be
 *    retained during the creation of the sheet, and released when the
 *    sheet is disposed. There is no net change to the refcount of
 *    these strings across CreateStandardSheet and sheet destruction.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    alertType:
 *      The type of alert to create.
 *    
 *    error:
 *      The error string to display. CreateStandardSheet increments the
 *      refcount on this string, so you may release it after
 *      CreateStandardSheet returns if you don't need it later.
 *    
 *    explanation:
 *      The explanation string to display. May be NULL or empty to
 *      display no explanation. CreateStandardSheet increments the
 *      refcount on this string, so you may release it after
 *      CreateStandardSheet returns if you don't need it later.
 *    
 *    param:
 *      The parameter block describing how to create the alert. May be
 *      NULL. CreateStandardSheet increments the refcount on the button
 *      title strings in the parameter block, so you may release them
 *      after CreateStandardSheet returns if you don't need them later.
 *    
 *    notifyTarget:
 *      The event target to be notified when the sheet is closed. The
 *      caller should install an event handler on this target for the
 *      [kEventClassCommand, kEventProcessCommand] event. May be NULL
 *      if the caller does not need the command event to be sent to any
 *      target. 
 *      
 *      Typically, this will be the event target for the parent window
 *      of the sheet; a standard practice is to install a handler on
 *      the parent window just before showing the sheet window, and to
 *      remove the handler from the parent window after the sheet has
 *      been closed. It is also possible to install a handler on the
 *      sheet window itself, in which case you would pass NULL for this
 *      parameter, since the command event is automatically sent to the
 *      sheet window already. If you install a handler on the sheet
 *      itself, make sure to return eventNotHandledErr from your
 *      handler, because CreateStandardSheet installs its own handler
 *      on the sheet and that handler must be allowed to run to close
 *      the sheet window and release the DialogRef.
 *    
 *    outSheet:
 *      On exit, contains the new alert.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateStandardSheet(
  AlertType                              alertType,
  CFStringRef                            error,
  CFStringRef                            explanation,        /* can be NULL */
  const AlertStdCFStringAlertParamRec *  param,              /* can be NULL */
  EventTargetRef                         notifyTarget,       /* can be NULL */
  DialogRef *                            outSheet)            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CloseStandardSheet()
 *  
 *  Summary:
 *    Closes a standard sheet dialog and releases the dialog data
 *    structures.
 *  
 *  Discussion:
 *    CloseStandardSheet is meant to be used when you need to remove a
 *    sheet because of a higher-priority request to close the sheet's
 *    document window. For example, you might have a Save Changes sheet
 *    open on a document window. Meanwhile, the user drags the document
 *    into the trash. When your application sees that the document has
 *    been moved to the trash, it knows that it should close the
 *    document window, but first it needs to close the sheet. 
 *    
 *    CloseStandardSheet should not be used by your Carbon event
 *    handler in response to a click in one of the sheet buttons; the
 *    Dialog Manager will close the sheet automatically in that case.
 *    
 *    
 *    If kStdAlertDoNotDisposeSheet was specified when the sheet was
 *    created, the sheet dialog will be hidden but not released, and
 *    you can reuse the sheet later.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSheet:
 *      The sheet to close.
 *    
 *    inResultCommand:
 *      This command, if not zero, will be sent to the EventTarget
 *      specified when the sheet was created.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CloseStandardSheet(
  DialogRef   inSheet,
  UInt32      inResultCommand)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDialogItemAsControl()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
GetDialogItemAsControl(
  DialogRef         inDialog,
  DialogItemIndex   inItemNo,
  ControlRef *      outControl)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  MoveDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
MoveDialogItem(
  DialogRef         inDialog,
  DialogItemIndex   inItemNo,
  SInt16            inHoriz,
  SInt16            inVert)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SizeDialogItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
SizeDialogItem(
  DialogRef         inDialog,
  DialogItemIndex   inItemNo,
  SInt16            inWidth,
  SInt16            inHeight)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  AppendDialogItemList()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in DialogsLib 8.5 and later
 */
extern OSErr 
AppendDialogItemList(
  DialogRef    dialog,
  SInt16       ditlID,
  DITLMethod   method)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
    • Dialog Routines available only with Appearance 1.1 (Mac OS 8.5) and later
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
*/

/*
 *  SetDialogTimeout()
 *  
 *  Summary:
 *    Sets the timeout for a modal dialog.
 *  
 *  Discussion:
 *    SetDialogTimeout sets the delay after which a dialog will be
 *    automatically dismissed. When SetDialogTimeout is called, the
 *    Dialog Manager takes the current time, adds the timeout to it,
 *    and stores the result as the time to dismiss the dialog. If the
 *    dismissal time is reached, the dialog is automatically closed and
 *    the specified dialog item index is returned from ModalDialog in
 *    the itemHit parameter. If the user moves the mouse or presses a
 *    key, the dismissal time is reset by adding the original timeout
 *    to the time of the event. Only the ModalDialog API observes the
 *    timeout value; if you are handling events in a modeless dialog or
 *    sheet using IsDialogEvent and DialogSelect, the timeout will be
 *    ignored.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDialog:
 *      The dialog on which to set a timeout.
 *    
 *    inButtonToPress:
 *      The dialog item index that should be returned from ModalDialog
 *      when the timeout expires.
 *    
 *    inSecondsToWait:
 *      The time to wait before dismissing the dialog, in seconds.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in DialogsLib 8.5 and later
 */
extern OSStatus 
SetDialogTimeout(
  DialogRef         inDialog,
  DialogItemIndex   inButtonToPress,
  UInt32            inSecondsToWait)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetDialogTimeout()
 *  
 *  Summary:
 *    Retrieves the timeout for a modal dialog.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDialog:
 *      The dialog from which to retrieve the timeout.
 *    
 *    outButtonToPress:
 *      On exit, contains the dialog item index that should be returned
 *      from ModalDialog when the dialog is dismissed. May be NULL if
 *      you do not need this information.
 *    
 *    outSecondsToWait:
 *      On exit, contains the time to wait before dismissing the
 *      dialog, in seconds. May be NULL if you do not need this
 *      information.
 *    
 *    outSecondsRemaining:
 *      On exit, contains the time until the dialog is dismissed, in
 *      seconds. May be NULL if you do not need this information.
 *  
 *  Result:
 *    An operating system result code. Returns dialogNoTimeoutErr if no
 *    timeout has been set for this dialog.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in DialogsLib 8.5 and later
 */
extern OSStatus 
GetDialogTimeout(
  DialogRef          inDialog,
  DialogItemIndex *  outButtonToPress,          /* can be NULL */
  UInt32 *           outSecondsToWait,          /* can be NULL */
  UInt32 *           outSecondsRemaining)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetModalDialogEventMask()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in DialogsLib 8.5 and later
 */
extern OSStatus 
SetModalDialogEventMask(
  DialogRef   inDialog,
  EventMask   inMask)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetModalDialogEventMask()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in DialogsLib 8.5 and later
 */
extern OSStatus 
GetModalDialogEventMask(
  DialogRef    inDialog,
  EventMask *  outMask)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;




/*
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
    • Accessor functions
  —————————————————————————————————————————————————————————————————————————————————————————————————————————
*/


/*
 *  GetDialogWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern WindowRef 
GetDialogWindow(DialogRef dialog)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetDialogTextEditHandle()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern TEHandle 
GetDialogTextEditHandle(DialogRef dialog)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetDialogDefaultItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern SInt16 
GetDialogDefaultItem(DialogRef dialog)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetDialogCancelItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern SInt16 
GetDialogCancelItem(DialogRef dialog)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetDialogKeyboardFocusItem()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern SInt16 
GetDialogKeyboardFocusItem(DialogRef dialog)                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetPortDialogPort()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern void 
SetPortDialogPort(DialogRef dialog)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetDialogPort()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern CGrafPtr 
GetDialogPort(DialogRef dialog)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetDialogFromWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern DialogRef 
GetDialogFromWindow(WindowRef window)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */

#if OLDROUTINENAMES && !__LP64__
#define DisposDialog(theDialog) DisposeDialog(theDialog)
#define UpdtDialog(theDialog, updateRgn) UpdateDialog(theDialog, updateRgn)
#define GetDItem(theDialog, itemNo, itemType, item, box) GetDialogItem(theDialog, itemNo, itemType, item, box)
#define SetDItem(theDialog, itemNo, itemType, item, box) SetDialogItem(theDialog, itemNo, itemType, item, box)
#define HideDItem(theDialog, itemNo) HideDialogItem(theDialog, itemNo)
#define ShowDItem(theDialog, itemNo) ShowDialogItem(theDialog, itemNo)
#define SelIText(theDialog, itemNo, strtSel, endSel) SelectDialogItemText(theDialog, itemNo, strtSel, endSel)
#define GetIText(item, text) GetDialogItemText(item, text)
#define SetIText(item, text) SetDialogItemText(item, text)
#define FindDItem(theDialog, thePt) FindDialogItem(theDialog, thePt)
#define NewCDialog(dStorage, boundsRect, title, visible, procID, behind, goAwayFlag, refCon, items) \
NewColorDialog(dStorage, boundsRect, title, visible, procID, behind, goAwayFlag, refCon, items)
#define GetAlrtStage() GetAlertStage()
#define ResetAlrtStage() ResetAlertStage()
#define DlgCut(theDialog) DialogCut(theDialog)
#define DlgPaste(theDialog) DialogPaste(theDialog)
#define DlgCopy(theDialog) DialogCopy(theDialog)
#define DlgDelete(theDialog) DialogDelete(theDialog)
#define SetDAFont(fontNum) SetDialogFont(fontNum)
#define SetGrafPortOfDialog(dialog) SetPortDialogPort(dialog)
#endif  /* OLDROUTINENAMES && !__LP64__ */











#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __DIALOGS__ */

