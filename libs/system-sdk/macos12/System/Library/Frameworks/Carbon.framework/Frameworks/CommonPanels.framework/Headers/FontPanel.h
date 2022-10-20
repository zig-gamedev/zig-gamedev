/*
     File:       CommonPanels/FontPanel.h
 
     Contains:   Carbon Font Panel package Interfaces.
 
     Version:    CommonPanels-94~674
 
     Copyright:  © 2002-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FONTPANEL__
#define __FONTPANEL__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __CARBON__
#include <Carbon/Carbon.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Font Panel-Related Events
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
 *  Discussion:
 *    Event classes
 */
enum {

  /*
   * Events related to font selection or handling.
   */
  kEventClassFont               = 'font'
};


/*
 *  Summary:
 *    Common command IDs
 */
enum {

  /*
   * The state of the Font Panel should be toggled, displaying it or
   * hiding it as necessary. If the user closes the Font Panel directly
   * from the window, the application will receive a
   * kEventFontPanelClosed event.
   */
  kHICommandShowHideFontPanel   = 'shfp'
};

/* Font Events */

/*
 *  Summary:
 *    Font events (kEventClassFont)
 *  
 *  Discussion:
 *    When the user closes the Font Panel, a kEventWindowClosed event
 *    will be detected by the Carbon event handler installed by the
 *    system. The system then notifies the application that the Font
 *    Panel has closed by posting a Carbon Event Manager event. This
 *    allows the application to update any menu items or other controls
 *    whose state may have to change because the Font Panel has closed.
 *    kEventWindowClosed has no parameters. When the user selects an
 *    item in the Font Panel, the system will send a
 *    kEventFontSelection event to the event target specified when the
 *    application called SetFontInfoForSelection(). kEventFontSelection
 *    will contain parameters reflecting the current Font Panel
 *    selection in all supported formats. Font events are available
 *    after Mac OS X 10.2 in the Carbon framework.
 */
enum {

  /*
   * The Font Panel has been closed. The application should update its
   * corresponding UI element (e.g., a menu item) accordingly.
   */
  kEventFontPanelClosed         = 1,

  /*
   * The user has specified font settings in the Font Panel. The
   * application can obtain these settings from the event, in which
   * they are stored as parameters. Not all parameters are guaranteed
   * to be present; the application should check for all those which it
   * recognizes and apply the ones found as appropriate to the target
   * text.
   */
  kEventFontSelection           = 2
};

/*
    Parameters for font events:

    kEventFontPanelClosed
        None.
        
    kEventFontSelection
        -->     kEventParamATSUFontID               typeATSUFontID
        -->     kEventParamATSUFontSize             typeATSUSize
        -->     kEventParamFMFontFamily             typeFMFontFamily
        -->     kEventParamFMFontSize               typeFMFontSize
        -->     kEventParamFontColor                typeFontColor
        -->     kEventParamDictionary               typeCFDictionaryRef 
        -->     kEventParamViewAttributesDictionary typeCFDictionaryRef
                A dictionary containing attributes that can be applied to an entire text view.  An example of this is the background color to 
                apply to the view.
*/
enum {
  typeCTFontDescriptorRef       = typeCFTypeRef, /* CTFontDescriptor reference.*/
  typeATSUFontID                = typeUInt32, /* ATSUI font ID.*/
  typeATSUSize                  = typeFixed, /* ATSUI font size.*/
  typeFMFontFamily              = typeSInt16, /* Font family reference.*/
  typeFMFontStyle               = typeSInt16, /* Quickdraw font style*/
  typeFMFontSize                = typeSInt16, /* Integer font size.*/
  typeFontColor                 = typeRGBColor, /* Font color spec (optional).*/
  kEventParamCTFontDescriptor   = 'ctfd', /* typeCTFontDescriptorRef*/
  kEventParamATSUFontID         = 'auid', /* typeATSUFontID*/
  kEventParamATSUFontSize       = 'ausz', /* typeATSUSize*/
  kEventParamFMFontFamily       = 'fmfm', /* typeFMFontFamily*/
  kEventParamFMFontStyle        = 'fmst', /* typeFMFontStyle*/
  kEventParamFMFontSize         = 'fmsz', /* typeFMFontSize*/
  kEventParamFontColor          = 'fclr', /* typeFontColor*/
  kEventParamDictionary         = 'dict', /*    typeCFDictionaryRef*/
  kEventParamViewAttributesDictionary = 'dadc' /*    typeCFDictionaryRef*/
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Key constants to be used to access data inside the dictionary that may
    be contained in the kEventFontSelection dictionary. (kEventParamDictionary)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 *  kFontPanelATSUFontIDKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelATSUFontIDKey                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*Value is a CFNumber containing the ATSU Font ID*/
/*
 *  kFontPanelVariationAxesKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelVariationAxesKey                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/* Value is a CFDataRef containing one or more ATSUI Variation Axes*/
/*
 *  kFontPanelVariationValuesKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelVariationValuesKey                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*Value is a CFDataRef containing one or more ATSU Variation values*/
/*
 *  kFontPanelFeatureTypesKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelFeatureTypesKey                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*  Value is a CFDataRef containing one or more ATSUI feature types*/
/*
 *  kFontPanelFeatureSelectorsKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelFeatureSelectorsKey               AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*  Value is a CFDataRef containing one or more ATSUI feature selectors*/
/*
 *  kFontPanelAttributesKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelAttributesKey                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*
  const <CFString> string kFontPanelAttributesKey           =   "FontAttributes";
    Value is a CFDictionaryRef containing three keyed values.  Each value is
    a CFDataRef.  One CFDataRef contains one or more ATSUAttributeTags.
    One CFDataRef contains one or more value sizes for each tag.  And the last
    CFDataRef contains the actual values.  It is important to understand that
    these are the actual values and not value ptrs.  To pass these values to
    ATSUI they must be converted into ptrs.  The following code fragment demonstrates
    one technique
    CFDataRef       values;
    CFDataRef       tags;
    CFDataRef       sizes;
    if (    CFDictionaryGetValueIfPresent( attributesDict, kFontPanelAttributeValuesKey, &values ) &&
            CFDictionaryGetValueIfPresent( attributesDict, kFontPanelAttributeTagsKey, &tags )
            CFDictionaryGetValueIfPresent( attributesDict, kFontPanelAttributeSizesKey, &sizes ))
    {
        ItemCount               count = CFDataGetLength( tags )/sizeof(ATSUAttributeTag);
        CFIndex                 index;
        ATSUAttributeValuePtr   valuePtrs = malloc( count * sizeof(ATSUAttributeValuePtr) );
        UInt32*                 sizePtr = (UInt32*)CFDataGetBytePtr(sizes);
        UInt32*                 bytePtr = (UInt32*)CFDataGetBytePtr(values);
        for ( index = 0; index < count; index++ )
        {
            valuePtrs[index] = bytePtr;
            bytePtr = (UInt32*)( (UInt8*)bytePtr + sizePtr[index]);
        }
        verify_noerr( ATSUSetAttributes( someATSUStyle, count, (ATSUAttributeTag*)CFDataGetBytePtr(tags),sizePtr, valuePtrs ) );
        free( valuePtrs );
*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Keys to access the CFDataRefs inside the attributes dictionary (see above)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 *  kFontPanelAttributeTagsKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelAttributeTagsKey                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*Value is a CFDataRef containing one or more style attribute tags*/
/*
 *  kFontPanelAttributeSizesKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelAttributeSizesKey                 AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*Value is a CFDataRef containing one or more style attribute sizes*/
/*
 *  kFontPanelAttributeValuesKey
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelAttributeValuesKey                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*Value is a CFDataRef containing one or more style values*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Keys to access to access the optional mouse tracking state if the font attribute/feature control is tracking
An application can look for this optional value to aid in supporting undo/redo for a font attribute/feature that is represented by
a control that tracks such as a slider.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 *  kFontPanelMouseTrackingState
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelMouseTrackingState                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
   The value referenced by this key is a CFNumberRef that will contain one of the following values
  from CarbonEvents.h
   kEventMouseDown
   kEventMouseUp
   kEventMouseDragged
*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Keys to access the data from the document attributes dictionary (kEventParamViewAttributesDictionary)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 *  kFontPanelBackgroundColorAttributeName
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFontPanelBackgroundColorAttributeName      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Other Font Panel Constants
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
Error codes (Font Panel codes in range [-8880,-8899]).
*/
enum {
  fontPanelShowErr              = -8880, /* Can't display the Font Panel.*/
  fontPanelSelectionStyleErr    = -8881, /* Bad font selection style info.*/
  fontPanelFontSelectionQDStyleVersionErr = -8882 /* Unsupported record version.*/
};

/*
Type of font information passed in SetFontInfoForSelection(). If the client is
sending ATSUI style data, it specifies kFontSelectionATSUIType; if it is
sending Quickdraw style data, it specifies kFontSelectionQDType.
*/
enum {
  kFontSelectionATSUIType       = 'astl', /* Use ATSUIStyle collection.*/
  kFontSelectionQDType          = 'qstl', /* Use FontSelectionQDStyle record.*/
  kFontSelectionCoreTextType    = 'ctfd' /* Use CTFontDescriptorRef.*/
};

/*
Supported versions of the FontSelectionQDStyle record. Clients should always set
the <version> field to one of these values.
*/
enum {
  kFontSelectionQDStyleVersionZero = 0
};


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Font Panel Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
Record specifying the font information to be specified in the Font
Panel. This record is used if the client is sending Quickdraw style data
(i.e., it specified kFontSelectionQDType in SetFontInfoForSelection()).
*/
struct FontSelectionQDStyle {
  UInt32              version;                /* Version number of struct.*/
  FMFontFamilyInstance  instance;             /* Font instance data.*/
  FMFontSize          size;                   /* Size of font in points.*/
  Boolean             hasColor;               /* true if color info supplied.*/
  UInt8               reserved;               /* Filler byte.*/
  RGBColor            color;                  /* Color specification for font.*/
};
typedef struct FontSelectionQDStyle     FontSelectionQDStyle;
typedef FontSelectionQDStyle *          FontSelectionQDStylePtr;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Font Panel Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 *  FPIsFontPanelVisible()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
FPIsFontPanelVisible(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  FPShowHideFontPanel()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FPShowHideFontPanel(void)                                     AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  SetFontInfoForSelection()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetFontInfoForSelection(
  OSType           iStyleType,
  UInt32           iNumStyles,
  void *           iStyles,
  EventTargetRef   iFPEventTarget)                            AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Font Collection Functions

    In 10.3 the ability to create and modify font collections is available.  Font
    collections are files containing font descriptions.  Font descriptions are
    encapsulated in the opaque object FCFontDescriptorRef. A FCFontDescriptroRef
    is a CFType.  To release one call CFRelease.
    
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Font Collection Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct OpaqueFCFontDescriptorRef*  FCFontDescriptorRef;
/*
 *  FCCopyCollectionNames()
 *  
 *  Discussion:
 *    FCCopyCollectionNames returns a copy of the CFArrayRef containing
 *    the displayable names of every font collection available to the
 *    current user.
 *  
 *  Result:
 *    A CFArrayRef containing CFStringRefs where each CFStringRef
 *    contains a font collection's displayable name. Callers are
 *    responsible for releasing the returned CFArrayRef.  If the
 *    operation is not successful NULL is returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
FCCopyCollectionNames(void)                                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  FCCopyFontDescriptorsInCollection()
 *  
 *  Discussion:
 *    FCCopyFontDescriptorsInCollection copies the fontDescriptors in a
 *    named collection into an array.
 *  
 *  Parameters:
 *    
 *    iCollection:
 *      The name of a collection that descriptors should be copied from.
 *  
 *  Result:
 *    A CFArrayRef containing copies of the FCFontDescriptorRefs
 *    contained in the name collection.  Callers are responsible for
 *    releasing the returned CFArrayRef.  The FCFontDescriptorRefs are
 *    retained when added to the array and released when the array is
 *    destroyed.  You can access a font descriptor in the array in the
 *    following manner: fd =
 *    (FCFontDescriptorRef)CFArrayGetValueAtIndex(array, i);
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
FCCopyFontDescriptorsInCollection(CFStringRef iCollection)    AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  FCAddCollection()
 *  
 *  Discussion:
 *    Add a collection to the font descriptor collections available to
 *    the current user. If the collection is successfully added noErr
 *    is returned.  If the collection is not added an error code is
 *    returned.
 *  
 *  Parameters:
 *    
 *    iCollection:
 *      the name of the collection to add.
 *    
 *    iCollectionOptions:
 *      currently there are no options.  Set to kNilOptions.  This
 *      parameter is available for future expansion.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FCAddCollection(
  CFStringRef   iCollection,
  OptionBits    iCollectionOptions)                           AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  FCRemoveCollection()
 *  
 *  Discussion:
 *    Remove a named collection from the font descriptor collections
 *    available to the current user. Returns noErr if the collection
 *    was successfully removed.  An appropriate error code is returned
 *    if the operation was not successful.
 *  
 *  Parameters:
 *    
 *    iCollection:
 *      the name of the collection to remove.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FCRemoveCollection(CFStringRef iCollection)                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  FCAddFontDescriptorToCollection()
 *  
 *  Discussion:
 *    Add a font descriptor to the named collection.  noErr is returned
 *    if the font descriptor is added. An error code describing the
 *    failure is returned if the descriptor is not added.
 *  
 *  Parameters:
 *    
 *    iDescriptor:
 *      the font descriptor that should be added.  The
 *      FCFontDescriptorRef is retained when it is added to the
 *      collection.  After calling this function the caller may release
 *      their copy.
 *    
 *    iCollection:
 *      the name of the collection to which the font descriptor should
 *      be added.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FCAddFontDescriptorToCollection(
  FCFontDescriptorRef   iDescriptor,
  CFStringRef           iCollection)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  FCRemoveFontDescriptorFromCollection()
 *  
 *  Discussion:
 *    Remove a font descriptor from the named collection.  An error is
 *    returned if the font descriptor can not be removed.  noErr is
 *    returned if the descriptor is removed.
 *  
 *  Parameters:
 *    
 *    iDescriptor:
 *      the descriptor that should be removed.
 *    
 *    iCollection:
 *      the name of the collection that the descriptor should be
 *      removed from.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FCRemoveFontDescriptorFromCollection(
  FCFontDescriptorRef   iDescriptor,
  CFStringRef           iCollection)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Font Descriptor Attribute Keys
    
    Font Descriptors contain font attributes that are set and accessed via a set of 
    keys.  The keys are all constant CFStringRefs.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
 *  kFCFontFamilyAttribute
 *  
 *  Discussion:
 *    The key for a CFStringRef that contains a font family name (e.g.
 *    Baskerville).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFCFontFamilyAttribute                      AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*
 *  kFCFontNameAttribute
 *  
 *  Discussion:
 *    The key for a CFStringRef containing a font name (e.g.
 *    Baskerville-Italic).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFCFontNameAttribute                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*
 *  kFCFontFaceAttribute
 *  
 *  Discussion:
 *    The key for a CFStringRef containing a face name (e.g. Italic).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFCFontFaceAttribute                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*
 *  kFCFontSizeAttribute
 *  
 *  Discussion:
 *    The key for a CFNumber containg the font size (e.g. 12).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFCFontSizeAttribute                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*
 *  kFCFontVisibleNameAttribute
 *  
 *  Discussion:
 *    The Key for a CFStringRef containing the name that should be used
 *    in a UI to describe the font.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFCFontVisibleNameAttribute                 AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*
 *  kFCFontCGColorAttribute
 *  
 *  Discussion:
 *    The Key for a CGColorRef containing the fonts color.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kFCFontCGColorAttribute                     AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
/*
 *  FCFontDescriptorCreateWithFontAttributes()
 *  
 *  Discussion:
 *    Create a font descriptor using the attributes contained in the
 *    dictionary.
 *  
 *  Parameters:
 *    
 *    iAttributes:
 *      a dictionary containing one or more of the attributes described
 *      above.
 *  
 *  Result:
 *    A valid FCFontDescriptorRef.  NULL if one cannot be created.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern FCFontDescriptorRef 
FCFontDescriptorCreateWithFontAttributes(CFDictionaryRef iAttributes) AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



/*
 *  FCFontDescriptorCreateWithName()
 *  
 *  Discussion:
 *    Create a font descriptor using a fontname and font size.
 *  
 *  Parameters:
 *    
 *    iFontName:
 *      The name of the font (e.g. Baskerville-Italic).
 *    
 *    iSize:
 *      the size of the font. (e.g. 12.0).
 *  
 *  Result:
 *    A valid FCFontDescriptorRef.  NULL if one cannot be created.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern FCFontDescriptorRef 
FCFontDescriptorCreateWithName(
  CFStringRef   iFontName,
  CGFloat       iSize)                                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __FONTPANEL__ */

