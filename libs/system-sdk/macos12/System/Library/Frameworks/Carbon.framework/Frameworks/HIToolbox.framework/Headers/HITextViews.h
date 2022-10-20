/*
     File:       HIToolbox/HITextViews.h
 
     Contains:   Definitions of text-display and text-editing views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HITEXTVIEWS__
#define __HITEXTVIEWS__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __APPEARANCE__
#include <HIToolbox/Appearance.h>
#endif

#ifndef __CARBONEVENTS__
#include <HIToolbox/CarbonEvents.h>
#endif

#ifndef __CONTROLS__
#include <HIToolbox/Controls.h>
#endif

#ifndef __MACTEXTEDITOR__
#include <HIToolbox/MacTextEditor.h>
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
 *  HITextViews.h
 *  
 *  Discussion:
 *    API definitions for the standard text-display and text-editing
 *    views: Static text view, HITextView, EditUnicodeText view, and
 *    (deprecated) classic EditText view.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ Static Text (CDEF 18)                                                             */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Static Text proc IDs */
enum {
  kControlStaticTextProc        = 288
};

/* Control Kind Tag */
enum {
  kControlKindStaticText        = 'stxt'
};

/* The HIObject class ID for the HIStaticTextView class. */
#define kHIStaticTextViewClassID        CFSTR("com.apple.HIStaticTextView")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateStaticTextControl()
 *  
 *  Summary:
 *    Creates a new static text control.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window in which the control should be placed. May be NULL
 *      in 10.3 and later.
 *    
 *    boundsRect:
 *      The bounds of the control, in local coordinates of the window.
 *    
 *    text:
 *      The text of the control. May be NULL.
 *    
 *    style:
 *      The control's font style, size, color, and so on. May be NULL.
 *    
 *    outControl:
 *      On exit, contains the new control.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateStaticTextControl(
  WindowRef                    window,           /* can be NULL */
  const Rect *                 boundsRect,
  CFStringRef                  text,             /* can be NULL */
  const ControlFontStyleRec *  style,            /* can be NULL */
  ControlRef *                 outControl)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Tagged data supported by the static text control
 */
enum {

  /*
   * Used to get or set the control's current text style. Data is of
   * type ControlFontStyleRec. Available with Appearance Manager 1.0
   * (Mac OS 8.0) and later.
   */
  kControlStaticTextStyleTag    = kControlFontStyleTag,

  /*
   * Used to get or set the control's current text. Data is an array of
   * chars. Generally you should used GetControlDataSize to determine
   * the length of the text, and allocate a buffer of that length,
   * before calling GetControlData with this selector. Deprecated in
   * Carbon in favor of kControlStaticTextCFStringTag. Available with
   * Appearance Manager 1.0 (Mac OS 8.0) and later.
   */
  kControlStaticTextTextTag     = 'text',

  /*
   * Used to get the height of the control's text. May not be used with
   * SetControlData. Data is of type SInt16. Available with Appearance
   * Manager 1.0 (Mac OS 8.0) and later.
   */
  kControlStaticTextTextHeightTag = 'thei',

  /*
   * Used to get or set the control's text truncation style. Data is of
   * type TruncCode; pass a truncation code of -1 to indication no
   * truncation. Available with Appearance Manager 1.1 (Mac OS 8.5) and
   * later. Truncation will not occur unless
   * kControlStaticTextIsMultilineTag is set to false.
   */
  kControlStaticTextTruncTag    = 'trun',

  /*
   * Used to get or set the control's current text. Data is of type
   * CFStringRef. When setting the text, the control will retain the
   * string, so you may release the string after calling
   * SetControlData; if the string is mutable, the control will make a
   * copy of the string, so any changes to the string after calling
   * SetControlData will not affect the control. When getting the text,
   * the control retains the string before returning it to you, so you
   * must release the string after you are done with it. Available in
   * CarbonLib 1.5 and Mac OS X 10.0 and later.
   */
  kControlStaticTextCFStringTag = 'cfst',

  /*
   * Used to get or set whether the control draws its text in multiple
   * lines if the text is too wide for the control bounds. If false,
   * then the control always draws the text in a single line. Data is
   * of type Boolean. Default is true. Available in Mac OS X 10.1 and
   * later.
   */
  kControlStaticTextIsMultilineTag = 'stim'
};


/*==============================================================================*/
/*  Text field events                                                           */
/*                                                                              */
/*  A text field is the part of some views that you can enter text into.        */
/*  A text field is common to the EditText, EditUnicodeText, ComboBox,          */
/*  HISearchField, and HITextView views. The kEventClassTextField event allows  */
/*  you to receive notifications when the text has been accepted by the user.   */
/*  For example, you can install a handler for a kEventClassTextField /         */
/*  kEventTextAccepted event on a HISearchField view to receive a notification  */
/*  that the user has initiated a search by hitting the return or enter key.    */
/*  You can also filter the text that will replace a selection before the       */
/*  change has been made to either accept or reject the replacement.            */
/*==============================================================================*/
/*
    kEventClassTextField quick reference:
    
    kEventTextAccepted              = 1,
    kEventTextShouldChangeInRange   = 2,
    kEventTextDidChange             = 3
*/
enum {
  kEventClassTextField          = 'txfd'
};

enum {
  kEventParamTextSelection      = 'txsl', /* typeCFRange*/
  kEventParamCandidateText      = 'tstx', /* typeCFStringRef*/
  kEventParamReplacementText    = 'trtx', /* typeCFStringRef*/
  kEventParamUnconfirmedRange   = 'tunr', /* typeCFRange*/
  kEventParamUnconfirmedText    = 'txun' /* typeCFStringRef*/
};

/*
 *  kEventClassTextField / kEventTextAccepted
 *  
 *  Summary:
 *    Notification that the text in a view's editable text field has
 *    been accepted.
 *  
 *  Discussion:
 *    This event is sent as a notification when the text contained in a
 *    view's editable text field has been accepted by the user. Text is
 *    accepted when the user presses return or enter on the keyboard
 *    for the EditUnicodeText, HIComboBox, and HISearchField views, or
 *    when the text has changed in the field and the field loses focus
 *    for the EditUnicodeText, HIComboBox, HISearchField and HITextView
 *    views. 
 *    
 *    This event is sent to the view containing the text field only, it
 *    will not propagate. It is sent to all handlers installed on the
 *    view containing the text field.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The editable text field that has sent the notification.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventTextAccepted            = 1
};

/*
 *  kEventClassTextField / kEventTextShouldChangeInRange
 *  
 *  Summary:
 *    Returns whether the text should be changed in editable text
 *    fields.
 *  
 *  Discussion:
 *    There are several editable text field views, such as the
 *    HIComboBox, HISearchField, HITextView, and EditUnicodeText views.
 *    There are times when you may require fine-grained control over
 *    what text is inserted into the text field and either accept the
 *    changes, reject them or modify what is to be entered. This event
 *    is sent whenever the text is about to be modified in a text
 *    field, either by user input or in other scenarios such as a paste
 *    from the clipboard, spell-checking word correction, or Mac OS X
 *    Service operation. You can change what text is inserted by
 *    providing a replacement string as a parameter to this event. This
 *    event is only sent for Unicode text views; it is not sent for the
 *    classic non-Unicode EditText control. 
 *    
 *    This event is not sent prior to programmatic modification of the
 *    text field contents using SetControlData. 
 *    
 *    This event is not sent while an active inline editing session is
 *    in progress. Once the inline text has been confirmed, this event
 *    will be sent prior to the confirmed text being inserted into the
 *    text field. If you need control over keystrokes during an inline
 *    editing session, you can use the kEventTextInputFilterText event.
 *    
 *    
 *    This event is sent to the view containing the text field only; it
 *    will not propagate.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The editable text field that has sent the notification.
 *          Available in Mac OS X 10.5 and later.
 *    
 *    --> kEventParamTextSelection (in, typeCFRange)
 *          The range of the selection that is about to be changed. The
 *          units of the selection are in the same units that are
 *          returned in a EditTextSelectionRec, when called with
 *          GetControlData using kControlEditTextSelectionTag.
 *    
 *    --> kEventParamCandidateText (in, typeCFStringRef)
 *          The text that is going to replace the selection. Note that
 *          this string was originally created with
 *          CFStringCreateWithCharactersNoCopy, and the original text
 *          has a limited lifespan. If for some reason you need to
 *          retain the text past the end of your event handler, you
 *          should extract the characters from the string with
 *          CFStringGetCharacters, and then store those characters or
 *          create a new CFString from them.
 *    
 *    <-- kEventParamReplacementText (out, typeCFStringRef)
 *          On output, can contain optional replacement text.
 *  
 *  Result:
 *    If noErr is returned from your handler and the
 *    kEventParamReplacementText parameter is added to the event, then
 *    the contents of that parameter, rather than the candidate text,
 *    will be added to the text field. 
 *    
 *    If noErr is returned from your handler and the
 *    kEventParamReplacementText parameter is _not_ added to the event,
 *    then the candidate text will be filtered out and no text will be
 *    entered in the text field. The current selection will be deleted,
 *    however. 
 *    
 *    If userCanceledErr is returned from your handler, then no text
 *    will be entered in the text field and the current selection will
 *    remain unchanged. Effectively, the editing operation will be
 *    ignored. Note that when an inline text editing session is active,
 *    you will only receive this event when the user attempts to
 *    confirm the inline input. You should not return userCanceledErr
 *    at this point; doing so will not really prevent the text from
 *    being committed. 
 *    
 *    If eventNotHandledErr is returned from your handler, the contents
 *    of the kEventParamReplacementText parameter are ignored, and the
 *    candidate text will replace the selection. 
 *    
 *    Any other return value will result in the default behavior, as if
 *    eventNotHandledErr had been returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventTextShouldChangeInRange = 2
};

/*
 *  kEventClassTextField / kEventTextDidChange
 *  
 *  Summary:
 *    Indicates that the contents of an editable text field have
 *    changed.
 *  
 *  Discussion:
 *    This event is sent by all of the Unicode-based editable text
 *    views: HIComboBox, HISearchField, HITextView and EditUnicodeText.
 *    This event is not sent for the classic non-Unicode EditText view.
 *    
 *    
 *    Note that this event is sent after inline editing operations,
 *    such as pressing a dead key, or using a input method that creates
 *    an inline editing hole. Most clients of this event should ignore
 *    the event during inline editing, and only respond to changes to
 *    the text after inline editing completes. A client can check for
 *    the presence of the kEventParamUnconfirmedRange parameter to
 *    determine whether inline editing is currently active; if this
 *    parameter is present, the client may wish to ignore the event.
 *    
 *    
 *    This event is not sent after programmatic modification of the
 *    text field contents using SetControlData. 
 *    
 *    This event is sent only to the view containing the text field; it
 *    will not propagate. It is sent to all handlers registered for it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The editable text field that has sent the notification.
 *          Available in Mac OS X 10.5 and later.
 *    
 *    --> kEventParamUnconfirmedRange (in, typeCFRange)
 *          If the text field currently has an open inline hole, this
 *          parameter contains the range of text inside the hole. This
 *          parameter is optional and is only present during inline
 *          editing.
 *    
 *    --> kEventParamUnconfirmedText (in, typeCFStringRef)
 *          If the text field currently has an open inline hole, this
 *          parameter contains the non-confirmed text currently being
 *          edited inside the hole. This parameter is optional and is
 *          only present during inline editing. Note that this string
 *          was originally created with
 *          CFStringCreateWithCharactersNoCopy, and the original text
 *          has a limited lifespan. If for some reason you need to
 *          retain the text past the end of your event handler, you
 *          should extract the characters from the string with
 *          CFStringGetCharacters, and then store those characters or
 *          create a new CFString from them.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventTextDidChange           = 3
};


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ HITextView                                                                                        */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*==============================================================================*/
/* HITextView is a MLTE view that can be embedded in the HIView hierarchy. The  */
/* view can be embedded in an HIScrollView if scroll bars are desired and can   */
/* be used in a composited window. On creation, a TXNObject is created to back  */
/* the view. You can extract the TXNObject at any time and use a subset of the  */
/* MLTE API with that object as an argument.                                    */
/*==============================================================================*/
/*
    In Mac OS X 10.4 and later, HITextView supports these tags previously defined for the EditUnicodeText control:
    
        kControlEditTextCharCount
        kControlEditTextSelectionTag
        kControlEditTextCFStringTag
        kControlEditTextInsertCFStringRefTag
*/

/* The HIObject class ID for the HITextView class. */
/*
 *  kHITextViewClassID
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kHITextViewClassID                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/* ControlKind*/
enum {
  kControlKindHITextView        = 'hitx'
};


#if !__LP64__
/*
 *  HITextViewCreate()
 *  
 *  Summary:
 *    Creates a text view. The new view is initially invisible.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBoundsRect:
 *      The bounding box of the view. If NULL, the bounds of the view
 *      will be initialized to 0.
 *    
 *    inOptions:
 *      There are currently no options. This must be 0.
 *    
 *    inTXNFrameOptions:
 *      Any frame options desired for the TXN object creation.
 *    
 *    outTextView:
 *      On exit, contains the new view.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HITextViewCreate(
  const HIRect *    inBoundsRect,            /* can be NULL */
  OptionBits        inOptions,
  TXNFrameOptions   inTXNFrameOptions,
  HIViewRef *       outTextView)                              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HITextViewGetTXNObject()
 *  
 *  Summary:
 *    Obtains the TXNObject that backs the text view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTextView:
 *      The text view that contains the TXNObject you wish to retrieve.
 *  
 *  Result:
 *    The TXNObject backing the given view.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern TXNObject 
HITextViewGetTXNObject(HIViewRef inTextView)                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HITextViewSetBackgroundColor()
 *  
 *  Summary:
 *    Sets the background color of the view. This allows you to provide
 *    alpha as well. If inColor is NULL, the background of the text
 *    view will not draw.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTextView:
 *      The text view that you are modifying the background color of.
 *    
 *    inColor:
 *      A CGColorRef representing the color or pattern that will fill
 *      the background of the text view. The CGColorRef will be
 *      retained by this API. If the text view already contains a
 *      background color, it will be released prior to the new color
 *      being retained. If inColor is NULL, the background of the text
 *      view will not draw.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HITextViewSetBackgroundColor(
  HIViewRef    inTextView,
  CGColorRef   inColor)          /* can be NULL */            AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HITextViewCopyBackgroundColor()
 *  
 *  Summary:
 *    Gets the background color of the view. If the background color
 *    returned is NULL, the background does not draw.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTextView:
 *      The text view from which you want to obtain the background
 *      color.
 *    
 *    outColor:
 *      A CGColorRef representing the color or pattern that is used for
 *      drawing the background of the text view. If the returned value
 *      is NULL, the background does not draw. If the returned
 *      CGColorRef is not NULL, it will be retained on return. You are
 *      responsible for releasing this CGColorRef when you are no
 *      longer referencing it.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HITextViewCopyBackgroundColor(
  HIViewRef     inTextView,
  CGColorRef *  outColor)                                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;






/*---------------------------------------------------------------------------------------*/
/* EditUnicodeText                                                                       */
/*---------------------------------------------------------------------------------------*/
/* This view is only available in Mac OS X.  It is super similar to Edit Text control    */
/* Use all the same Get/Set tags.  But don't ask for the TEHandle.                       */
/*---------------------------------------------------------------------------------------*/
#endif  /* !__LP64__ */

enum {
  kControlEditUnicodeTextProc   = 912,
  kControlEditUnicodeTextPasswordProc = 914
};

/* Control Kind Tag */
enum {
  kControlKindEditUnicodeText   = 'eutx'
};

/* The HIObject class ID for the HITextField class. */
#define kHITextFieldClassID             CFSTR("com.apple.HITextField")
#if !__LP64__
/*
 *  CreateEditUnicodeTextControl()
 *  
 *  Summary:
 *    Creates a new edit text control.
 *  
 *  Discussion:
 *    This is the preferred edit text control. Use it instead of the
 *    EditText control. This control handles Unicode and draws its text
 *    using antialiasing, which the other control does not.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window in which the control should be placed. May be NULL
 *      in 10.3 and later.
 *    
 *    boundsRect:
 *      The bounds of the control, in local coordinates of the window.
 *    
 *    text:
 *      The text of the control. May be NULL.
 *    
 *    isPassword:
 *      A Boolean indicating whether the field is to be used as a
 *      password field. Passing false indicates that the field is to
 *      display entered text normally. True means that the field will
 *      be used as a password field and any text typed into the field
 *      will be displayed only as bullets.
 *    
 *    style:
 *      The control's font style, size, color, and so on. May be NULL.
 *    
 *    outControl:
 *      On exit, contains the new control (if noErr is returned as the
 *      result code).
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateEditUnicodeTextControl(
  WindowRef                    window,
  const Rect *                 boundsRect,
  CFStringRef                  text,             /* can be NULL */
  Boolean                      isPassword,
  const ControlFontStyleRec *  style,            /* can be NULL */
  ControlRef *                 outControl)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by edit text */
#endif  /* !__LP64__ */

enum {
  kControlEditTextStyleTag      = kControlFontStyleTag, /* ControlFontStyleRec*/
  kControlEditTextTextTag       = 'text', /* Buffer of chars - you supply the buffer*/
  kControlEditTextKeyFilterTag  = kControlKeyFilterTag,
  kControlEditTextSelectionTag  = 'sele', /* ControlEditTextSelectionRec*/
  kControlEditTextPasswordTag   = 'pass', /* The clear text password text*/
  kControlEditTextCharCount     = 'chrc' /* [UInt32] Count of characters in the control's text*/
};

/* tags available with Appearance 1.1 or later */
enum {
  kControlEditTextKeyScriptBehaviorTag = 'kscr', /* ControlKeyScriptBehavior. Defaults to "PrefersRoman" for password fields,*/
                                        /*       or "AllowAnyScript" for non-password fields.*/
  kControlEditTextLockedTag     = 'lock', /* Boolean. Locking disables editability.*/
  kControlEditTextFixedTextTag  = 'ftxt', /* Like the normal text tag, but fixes inline input first*/
  kControlEditTextValidationProcTag = 'vali' /* ControlEditTextValidationUPP. Called when a key filter can't be: after cut, paste, etc.*/
};


/*
 *  Discussion:
 *    EditText ControlData tags available with Mac OS X and later.
 */
enum {

  /*
   * Extract the content of the edit text field as a CFString.  Don't
   * forget that you own the returned CFStringRef and are responsible
   * for CFReleasing it.
   */
  kControlEditTextCFStringTag   = 'cfst', /* CFStringRef (Also available on CarbonLib 1.5)*/

  /*
   * Extract the content of the edit text field as a CFString, if it is
   * a password field.  Don't forget that you own the returned
   * CFStringRef and are responsible for CFReleasing it.
   */
  kControlEditTextPasswordCFStringTag = 'pwcf' /* CFStringRef*/
};

enum {
  kControlEditTextSingleLineTag = 'sglc', /* data is a Boolean; indicates whether the control should always be single-line*/
  kControlEditTextInsertTextBufferTag = 'intx', /* data is an array of char; get or set the control's text as WorldScript-encoded text*/
  kControlEditTextInsertCFStringRefTag = 'incf', /* data is a CFStringRef; get or set the control's text as a CFStringRef. Caller should release CFString if getting.*/
  kControlEditUnicodeTextPostUpdateProcTag = 'upup', /* data is a UnicodePostUpdateUPP; get or set the post-update proc*/
  kControlEditTextSpellCheckingTag = 'spck', /* data is a Boolean; indicates whether the control wants to have spell checking support. Available in Leopard and later.*/
  kControlEditTextSpellCheckAsYouTypeTag = 'scat' /* data is a Boolean; indicates whether you want to support spell-checking-as-you-type. Available in Leopard and later.*/
};

/* Structure for getting the edit text selection. Used with kControlEditTextSelectionTag. */
struct ControlEditTextSelectionRec {
  SInt16              selStart;
  SInt16              selEnd;
};
typedef struct ControlEditTextSelectionRec ControlEditTextSelectionRec;
typedef ControlEditTextSelectionRec *   ControlEditTextSelectionPtr;
typedef CALLBACK_API( void , ControlEditTextValidationProcPtr )(ControlRef control);
/* This callback supplies the functionality of the TSMTEPostUpdateProcPtr that is used */
/* in the EditText control.  A client should supply this call if they want to look at  */
/* inline text that has been fixed before it is included in the actual body text       */
/* if the new text (i.e. the text in the handle) should be included in the body text    */
/* the client should return true.  If the client wants to block the inclusion of the    */
/* text they should return false.                                                       */
typedef CALLBACK_API( Boolean , EditUnicodePostUpdateProcPtr )(UniCharArrayHandle uniText, UniCharCount uniTextLength, UniCharArrayOffset iStartOffset, UniCharArrayOffset iEndOffset, void *refcon);
typedef STACK_UPP_TYPE(ControlEditTextValidationProcPtr)        ControlEditTextValidationUPP;
typedef STACK_UPP_TYPE(EditUnicodePostUpdateProcPtr)            EditUnicodePostUpdateUPP;
/*
 *  NewControlEditTextValidationUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlEditTextValidationUPP
NewControlEditTextValidationUPP(ControlEditTextValidationProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewEditUnicodePostUpdateUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern EditUnicodePostUpdateUPP
NewEditUnicodePostUpdateUPP(EditUnicodePostUpdateProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlEditTextValidationUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlEditTextValidationUPP(ControlEditTextValidationUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeEditUnicodePostUpdateUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void
DisposeEditUnicodePostUpdateUPP(EditUnicodePostUpdateUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlEditTextValidationUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeControlEditTextValidationUPP(
  ControlRef                    control,
  ControlEditTextValidationUPP  userUPP)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeEditUnicodePostUpdateUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean
InvokeEditUnicodePostUpdateUPP(
  UniCharArrayHandle        uniText,
  UniCharCount              uniTextLength,
  UniCharArrayOffset        iStartOffset,
  UniCharArrayOffset        iEndOffset,
  void *                    refcon,
  EditUnicodePostUpdateUPP  userUPP)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline ControlEditTextValidationUPP                         NewControlEditTextValidationUPP(ControlEditTextValidationProcPtr userRoutine) { return userRoutine; }
    inline EditUnicodePostUpdateUPP                             NewEditUnicodePostUpdateUPP(EditUnicodePostUpdateProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeControlEditTextValidationUPP(ControlEditTextValidationUPP) { }
    inline void                                                 DisposeEditUnicodePostUpdateUPP(EditUnicodePostUpdateUPP) { }
    inline void                                                 InvokeControlEditTextValidationUPP(ControlRef control, ControlEditTextValidationUPP userUPP) { (*userUPP)(control); }
    inline Boolean                                              InvokeEditUnicodePostUpdateUPP(UniCharArrayHandle uniText, UniCharCount uniTextLength, UniCharArrayOffset iStartOffset, UniCharArrayOffset iEndOffset, void * refcon, EditUnicodePostUpdateUPP userUPP) { return (*userUPP)(uniText, uniTextLength, iStartOffset, iEndOffset, refcon); }
  #else
    #define NewControlEditTextValidationUPP(userRoutine)        ((ControlEditTextValidationUPP)userRoutine)
    #define NewEditUnicodePostUpdateUPP(userRoutine)            ((EditUnicodePostUpdateUPP)userRoutine)
    #define DisposeControlEditTextValidationUPP(userUPP)
    #define DisposeEditUnicodePostUpdateUPP(userUPP)
    #define InvokeControlEditTextValidationUPP(control, userUPP) (*userUPP)(control)
    #define InvokeEditUnicodePostUpdateUPP(uniText, uniTextLength, iStartOffset, iEndOffset, refcon, userUPP) (*userUPP)(uniText, uniTextLength, iStartOffset, iEndOffset, refcon)
  #endif
#endif


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __HITEXTVIEWS__ */

