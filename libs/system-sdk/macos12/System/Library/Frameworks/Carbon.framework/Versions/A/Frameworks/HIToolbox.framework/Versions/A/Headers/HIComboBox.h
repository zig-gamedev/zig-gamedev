/*
     File:       HIToolbox/HIComboBox.h
 
     Contains:   Definition of the combo box view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HICOMBOBOX__
#define __HICOMBOBOX__

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



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  HIComboBox.h
 *  
 *  Discussion:
 *    API definitions for the combo box view.
 */
/*==============================================================================*/
/* HIComboBox                                                                   */
/*                                                                              */
/* The combo box is a new view starting in Mac OS 10.2. It can be used in       */
/* both the new compositing mode, as well as the traditional Control Manager    */
/* mode. Like all new HIFoo views, this view is created invisible. You must     */
/* show the view after creation if you want to, like, see it and stuff.         */
/*==============================================================================*/
/* The HIObject class ID for the HIComboBox class. */
#define kHIComboBoxClassID              CFSTR("com.apple.HIComboBox")
/*
    kEventClassHIComboBox quick reference:
    
    kEventComboBoxListItemSelected  = 1
*/
enum {
  kEventClassHIComboBox         = 'hicb'
};

enum {
  kEventParamComboBoxListSelectedItemIndex = 'cbli'
};

/*
 *  kEventClassHIComboBox / kEventComboBoxListItemSelected
 *  
 *  Summary:
 *    Notification that an item in the ComboBox disclosure list has
 *    been selected.
 *  
 *  Discussion:
 *    This event is sent as a notification when an item in the ComboBox
 *    disclosure list has been selected.  This event is sent to all
 *    handlers installed on the view. This does not imply that the
 *    selection has been accepted; for that you will need to register
 *    for the kEventClassTextField/kEventTextAccepted event; you can
 *    register for that event in order to make live selections however.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The ComboBox view that has sent the notification.
 *    
 *    --> kEventParamComboBoxListSelectedItemIndex (in, typeCFIndex)
 *          The index of the combo box list item that has been selected.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventComboBoxListItemSelected = 1
};


/*
 *  Summary:
 *    ComboBox attributes
 */
enum {

  /*
   * A constant with value zero; the lack of any attributes.
   */
  kHIComboBoxNoAttributes       = 0,

  /*
   * The view will attempt to auto complete the text the user is typing
   * with an item in the ComboBox list that is the closest appropriate
   * match.
   */
  kHIComboBoxAutoCompletionAttribute = (1 << 0),

  /*
   * The view will disclose the ComboBox list after the user enters
   * text.
   */
  kHIComboBoxAutoDisclosureAttribute = (1 << 1),

  /*
   * The items in the ComboBox list will be automatically sorted in
   * alphabetical order.
   */
  kHIComboBoxAutoSortAttribute  = (1 << 2),

  /*
   * The ComboBox list will be automatically sized to fit the Human
   * Interface Guidelines.
   */
  kHIComboBoxAutoSizeListAttribute = (1 << 3),

  /*
   * The minimum set of ComboBox attributes commonly used.
   */
  kHIComboBoxStandardAttributes = (kHIComboBoxAutoCompletionAttribute | kHIComboBoxAutoDisclosureAttribute | kHIComboBoxAutoSizeListAttribute)
};

/* ControlKind*/
enum {
  kControlKindHIComboBox        = 'cbbx'
};

/* ComboBox Part codes*/
enum {
  kHIComboBoxEditTextPart       = 5,
  kHIComboBoxDisclosurePart     = 28
};

/*
    The ComboBox view supports these tags previously defined for the EditUnicodeText view.
    These tags are available through Get/SetControlData with a ControlPartCode of kHIComboBoxEditTextPart:
    
        kControlFontStyleTag
        kControlEditTextFixedTextTag
        kControlEditTextTextTag
        kControlEditTextKeyFilterTag
        kControlEditTextValidationProcTag
        kControlEditUnicodeTextPostUpdateProcTag
        kControlEditTextSelectionTag
        kControlEditTextKeyScriptBehaviorTag
        kControlEditTextCharCount
        kControlEditTextCFStringTag
*/

/*
 *  Discussion:
 *    ComboBox ControlData tags available with Mac OS X 10.2 and later.
 */
enum {

  /*
   * Extract the contents of the ComboBox list as a CFArray. The
   * CFArray will be retained: if you get the array, you own it and
   * will be required to release it; if you set it the toolbox makes a
   * copy of it and you are free to release your reference.
   */
  kHIComboBoxListTag            = 'cbls', /* CFArrayRef; bumps the refCount on get/retains on set*/

  /*
   * The width of the ComboBox list. This can be customized. This
   * disables the autosize attribute.
   */
  kHIComboBoxListPixelWidthTag  = 'cblw', /* UInt32 */

  /*
   * The height of the ComboBox list. This can be customized. This
   * disables the autosize attribute.
   */
  kHIComboBoxListPixelHeightTag = 'cblh', /* UInt32*/

  /*
   * The number of visible items in the list. This can be customized.
   * This disables the autosize attribute.
   */
  kHIComboBoxNumVisibleItemsTag = 'cbni' /* UInt32*/
};

#if !__LP64__
/*
 *  HIComboBoxCreate()
 *  
 *  Summary:
 *    Creates a combo box view. The new view is initially invisible.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    boundsRect:
 *      The bounding box of the view.
 *    
 *    text:
 *      The default text in the editable portion of the view. Can be
 *      NULL.
 *    
 *    style:
 *      The font style of the both editable text and the text in the
 *      disclosure list. Can be NULL.
 *    
 *    list:
 *      The default values available in the disclosure list. Can be
 *      NULL.
 *    
 *    inAttributes:
 *      The default attributes of the combo box.
 *    
 *    outComboBox:
 *      On exit, contains the new view.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxCreate(
  const HIRect *               boundsRect,
  CFStringRef                  text,               /* can be NULL */
  const ControlFontStyleRec *  style,              /* can be NULL */
  CFArrayRef                   list,               /* can be NULL */
  OptionBits                   inAttributes,
  HIViewRef *                  outComboBox)                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxGetItemCount()
 *  
 *  Summary:
 *    Get the number of items in the combo box disclosure list.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box.
 *  
 *  Result:
 *    The number of items in the combo box disclosure list.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern ItemCount 
HIComboBoxGetItemCount(HIViewRef inComboBox)                  AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxInsertTextItemAtIndex()
 *  
 *  Summary:
 *    Inserts a CFString in the disclosure list
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box whose disclosure list the text will be inserted
 *      in.
 *    
 *    inIndex:
 *      The index that the text should be inserted in. If the index
 *      does not fall within the number of items in the combo box list,
 *      it will be appended to the end of the list.
 *    
 *    inText:
 *      The text item to be inserted in the combo box disclosure list.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxInsertTextItemAtIndex(
  HIViewRef     inComboBox,
  CFIndex       inIndex,
  CFStringRef   inText)                                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxAppendTextItem()
 *  
 *  Summary:
 *    Appends a text item to the combo box disclosure list.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box whose disclosure list the text will be appended
 *      to.
 *    
 *    inText:
 *      The text item to be appended to the combo box disclosure list.
 *    
 *    outIndex:
 *      On exit, the index of the new item. Can be NULL if the caller
 *      does not require this information.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxAppendTextItem(
  HIViewRef     inComboBox,
  CFStringRef   inText,
  CFIndex *     outIndex)         /* can be NULL */           AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxCopyTextItemAtIndex()
 *  
 *  Summary:
 *    Copy the text from the combo box disclosure list
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box that contains the text item you would like to
 *      copy.
 *    
 *    inIndex:
 *      The index of the text item. Will return paramErr if the index
 *      is out of bounds of the combo box list.
 *    
 *    outString:
 *      A copy of the string at the given index. Remember this is now
 *      your copy that you will need to release.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxCopyTextItemAtIndex(
  HIViewRef      inComboBox,
  CFIndex        inIndex,
  CFStringRef *  outString)                                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxRemoveItemAtIndex()
 *  
 *  Summary:
 *    Remove an item from a combo box disclosure list.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box that contains the disclosure list that you would
 *      like to remove an item from.
 *    
 *    inIndex:
 *      The index of the item to remove.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxRemoveItemAtIndex(
  HIViewRef   inComboBox,
  CFIndex     inIndex)                                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxChangeAttributes()
 *  
 *  Summary:
 *    Change the attributes of a combo box
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box whose attributes you would like to change.
 *    
 *    inAttributesToSet:
 *      The attributes to set.
 *    
 *    inAttributesToClear:
 *      The attributes to clear.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxChangeAttributes(
  HIViewRef    inComboBox,
  OptionBits   inAttributesToSet,
  OptionBits   inAttributesToClear)                           AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxGetAttributes()
 *  
 *  Summary:
 *    Get the attributes of a combo box.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box whose attributes you would like to obtain.
 *    
 *    outAttributes:
 *      The attributes of the combo box.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxGetAttributes(
  HIViewRef     inComboBox,
  OptionBits *  outAttributes)                                AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIComboBoxIsListVisible()
 *  
 *  Summary:
 *    Returns whether the combo box list is currently disclosed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box whose list visibility you would like to obtain.
 *  
 *  Result:
 *    A boolean value indicating whether the combo box list is
 *    disclosed (true) or hidden (false).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIComboBoxIsListVisible(HIViewRef inComboBox)                 AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIComboBoxSetListVisible()
 *  
 *  Summary:
 *    Hides or shows the combo box list.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inComboBox:
 *      The combo box whose list will be hidden or shown.
 *    
 *    inVisible:
 *      A boolean value indicating whether you wish to hide the list
 *      (false) or show the list (true).
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIComboBoxSetListVisible(
  HIViewRef   inComboBox,
  Boolean     inVisible)                                      AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HICOMBOBOX__ */

