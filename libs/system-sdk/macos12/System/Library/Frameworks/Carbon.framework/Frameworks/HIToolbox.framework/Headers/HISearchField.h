/*
     File:       HIToolbox/HISearchField.h
 
     Contains:   Definition of the search field view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HISEARCHFIELD__
#define __HISEARCHFIELD__

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
 *  HISearchField.h
 *  
 *  Discussion:
 *    API definitions for the search field view.
 */
/*==============================================================================*/
/*  HISearchField                                                               */
/*                                                                              */
/*  HISearchField is a new view available in Mac OS X 10.3.                     */
/*  This view is designed to be used for applications that provide searching    */
/*  functionality. Visually, it is a standard text field optionally adorned     */
/*  with a search icon on the left and/or a cancel image on the right.          */
/*  When the user has accepted the text by pressing the return or enter key     */
/*  a Carbon Event of kEventClassTextField / kEventTextAccepted will be sent    */
/*  to the view. This will be the indication that the search should begin.      */
/*  This view will also respond to all the standard control data tags that are  */
/*  used by the EditUnicodeText view.                                           */
/*==============================================================================*/
/* The HIObject class ID for the HISearchField class. */
#define kHISearchFieldClassID           CFSTR("com.apple.HISearchField")
/* ControlKind*/
enum {
  kControlKindHISearchField     = 'srfd'
};

/* HISearchField part codes*/
enum {
  kControlSearchFieldCancelPart = 30,
  kControlSearchFieldMenuPart   = 31
};

/*
    The SearchField view supports those tags previously defined for the EditUnicodeText view.
    These tags are available through Get/SetControlData with ControlPartCode of kControlEditTextPart:
        
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
 *  Summary:
 *    HISearchField attributes
 */
enum {

  /*
   * A constant with value zero; the lack of any attributes.
   */
  kHISearchFieldNoAttributes    = 0,

  /*
   * This view contains the cancel icon in the text field.
   */
  kHISearchFieldAttributesCancel = (1 << 0),

  /*
   * This view contains the search icon in the text field. If a menu is
   * associated with the search field, this attribute is implicitly set
   * and the search icon will display with a menu disclosure badge.
   * Available in Mac OS X 10.4 and later.
   */
  kHISearchFieldAttributesSearchIcon = (1 << 1)
};

/* Event Classes*/
enum {
  kEventClassSearchField        = 'srfd'
};

/*
 *  kEventClassSearchField / kEventSearchFieldCancelClicked
 *  
 *  Summary:
 *    Notification that the cancel icon has been depressed.
 *  
 *  Discussion:
 *    This event is sent by the HISearchField view if the cancel icon
 *    is enabled (attribute of kHISearchFieldAtttributesCancel), and
 *    the cancel has been clicked.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The HISearchField that has sent the notification.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventSearchFieldCancelClicked = 1
};

/*
 *  kEventClassSearchField / kEventSearchFieldSearchClicked
 *  
 *  Summary:
 *    Notification that the search icon has been depressed.
 *  
 *  Discussion:
 *    This event is sent by the HISearchField view if the search icon
 *    is enabled (attribute of kHISearchFieldAttributesSearchIcon or a
 *    menu is associated with the search field), and the search icon
 *    has been clicked. If a menu is associated with the search field,
 *    the search field will handle the display and tracking of the menu
 *    by default. This event is sent to the search field only, it will
 *    not propagate.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The HISearchField that has sent the notification.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventSearchFieldSearchClicked = 2
};

#if !__LP64__
/*
 *  HISearchFieldCreate()
 *  
 *  Summary:
 *    Creates a search field view. The new view is initially invisible.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inBounds:
 *      The initial bounds of the view. If this parameter is NULL, the
 *      view defaults to have empty bounds ( 0, 0, 0, 0 ).
 *    
 *    inAttributes:
 *      The initial attributes of the search field. Indicates whether
 *      the field should contain the cancel icon.
 *    
 *    inSearchMenu:
 *      The menu to be associated with this search field. If
 *      inSearchMenu is non-NULL, it will be retained by the search
 *      field and the search icon will be enabled in the left side of
 *      the text field. If this parameter is NULL, the view will not
 *      display the search icon in the left portion of the text field.
 *      You are expected to install handlers on this menu to handle the
 *      visual appearance of the menu (for example, to draw check marks
 *      or enable items when the menu receives the
 *      kEventMenuEnableItems Carbon Event), and to keep track of what
 *      action should be performed by associating HICommands with each
 *      menu item and installing a handler for the {
 *      kEventClassCommand, kEventCommandProcess } Carbon Event.
 *    
 *    inDescriptiveText:
 *      The text to be displayed in the text field when the field does
 *      not have focus and contains no user entered text. This is meant
 *      to be an indication of what the search criteria is. For
 *      example, you may wish to identify to the user that the search
 *      will cover the "Subject" or "Contents" of a selected range of
 *      items. If inDescriptiveText is non-NULL it will be retained by
 *      the search field.
 *    
 *    outRef:
 *      On exit, contains the new view.
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
HISearchFieldCreate(
  const HIRect *  inBounds,                /* can be NULL */
  OptionBits      inAttributes,
  MenuRef         inSearchMenu,            /* can be NULL */
  CFStringRef     inDescriptiveText,       /* can be NULL */
  HIViewRef *     outRef)                                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISearchFieldSetSearchMenu()
 *  
 *  Summary:
 *    Set the search menu associated with the view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSearchField:
 *      The search field to associate the search menu with.
 *    
 *    inSearchMenu:
 *      The menu to associate with the search field. If there is
 *      already a menu associated with the search field, that menu will
 *      be released. If inSearchMenu is non-NULL, it will be retained
 *      by the search field and the search icon will be enabled in the
 *      left side of the text field. You are expected to install
 *      handlers on this menu to handle the visual appearance of the
 *      menu (for example, to draw check marks or enable items when the
 *      menu receives the kEventMenuEnableItems Carbon Event), and to
 *      keep track of what action should be performed by associating
 *      HICommands with each menu item and installing a handler for the
 *      { kEventClassCommand, kEventCommandProcess } Carbon Event. If
 *      inSearchMenu is NULL, the search icon will be removed from the
 *      left side of the text field and no menu will be associated with
 *      this field.
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
HISearchFieldSetSearchMenu(
  HIViewRef   inSearchField,
  MenuRef     inSearchMenu)        /* can be NULL */          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISearchFieldGetSearchMenu()
 *  
 *  Summary:
 *    Get the menu that is associated with the search field
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSearchField:
 *      The search field you wish to retrieve the search menu from.
 *    
 *    outSearchMenu:
 *      On exit, will contain the menu that is associated with search
 *      field. The menu will _not_ be retained on output and this
 *      parameter cannot be NULL.
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
HISearchFieldGetSearchMenu(
  HIViewRef   inSearchField,
  MenuRef *   outSearchMenu)                                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISearchFieldChangeAttributes()
 *  
 *  Summary:
 *    Set the attributes for the given search field.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSearchField:
 *      The search field to change the attributes of.
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
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HISearchFieldChangeAttributes(
  HIViewRef    inSearchField,
  OptionBits   inAttributesToSet,
  OptionBits   inAttributesToClear)                           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISearchFieldGetAttributes()
 *  
 *  Summary:
 *    Returns the attributes of the search field.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSearchField:
 *      The search field to get the attributes of.
 *    
 *    outAttributes:
 *      On exit, will contain the attributes of the search field. This
 *      parameter cannot be NULL.
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
HISearchFieldGetAttributes(
  HIViewRef     inSearchField,
  OptionBits *  outAttributes)                                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISearchFieldSetDescriptiveText()
 *  
 *  Summary:
 *    Set the description of the search action of the search field.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSearchField:
 *      The search field to change the description of.
 *    
 *    inDescription:
 *      The new description for the search field. If the search field
 *      contains a description, it will be released. If inDescription
 *      is non-NULL, it will be retained by the search field. If it is
 *      NULL, no description will be associated with the search field.
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
HISearchFieldSetDescriptiveText(
  HIViewRef     inSearchField,
  CFStringRef   inDescription)       /* can be NULL */        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HISearchFieldCopyDescriptiveText()
 *  
 *  Summary:
 *    Get the description that is associated with the search field.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSearchField:
 *      The search field you wish to retrieve the description from.
 *    
 *    outDescription:
 *      On exit, will contain the description that is associated with
 *      the search field. This parameter cannot be NULL. If there is no
 *      description associated with the search field, outDescription
 *      will be set to NULL. If there is a description, a CFStringRef
 *      will be created that contains the contents of the description.
 *      You posess ownership of this string and will need to release it
 *      when you no longer need it.
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
HISearchFieldCopyDescriptiveText(
  HIViewRef      inSearchField,
  CFStringRef *  outDescription)                              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HISEARCHFIELD__ */

