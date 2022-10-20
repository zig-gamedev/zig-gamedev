/*
     File:       HIToolbox/Keyboards.h
 
     Contains:   Keyboard API.
 
     Copyright:  © 1997-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __KEYBOARDS__
#define __KEYBOARDS__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Keyboard API constants                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 *  PhysicalKeyboardLayoutType
 *  
 *  Summary:
 *    Physical keyboard layout types indicate the physical keyboard
 *    layout. They are returned by the KBGetLayoutType API.
 */
typedef OSType PhysicalKeyboardLayoutType;
enum {

  /*
   * A JIS keyboard layout type.
   */
  kKeyboardJIS                  = 'JIS ',

  /*
   * An ANSI keyboard layout type.
   */
  kKeyboardANSI                 = 'ANSI',

  /*
   * An ISO keyboard layout type.
   */
  kKeyboardISO                  = 'ISO ',

  /*
   * An unknown physical keyboard layout type.
   */
  kKeyboardUnknown              = kUnknownType /* '????'*/
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Keyboard API types                                                               */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 *  KeyboardLayoutRef
 *  
 *  Summary:
 *    The opaque keyboard layout contains information about a keyboard
 *    layout. It is used with the keyboard layout APIs.
 *  
 *  Discussion:
 *    KeyboardLayoutRef APIs follow CoreFoundation function naming
 *    convention. You mustn't release any references you get from APIs
 *    named "Get."
 */
typedef struct OpaqueKeyboardLayoutRef*  KeyboardLayoutRef;

/*
 *  KeyboardLayoutPropertyTag
 *  
 *  Summary:
 *    Keyboard layout property tags specify the value you want to
 *    retrieve. They are used with the KLGetKeyboardLayoutProperty API.
 */
typedef UInt32 KeyboardLayoutPropertyTag;
enum {

  /*
   * The keyboard layout data (const void *).  It is used with the
   * KeyTranslate API.
   */
  kKLKCHRData                   = 0,

  /*
   * The keyboard layout data (const void *).  It is used with the
   * UCKeyTranslate API.
   */
  kKLuchrData                   = 1,

  /*
   * The keyboard layout identifier (KeyboardLayoutIdentifier).
   */
  kKLIdentifier                 = 2,

  /*
   * The keyboard layout icon (IconRef).
   */
  kKLIcon                       = 3,

  /*
   * The localized keyboard layout name (CFStringRef).
   */
  kKLLocalizedName              = 4,

  /*
   * The keyboard layout name (CFStringRef).
   */
  kKLName                       = 5,

  /*
   * The keyboard layout group identifier (SInt32).
   */
  kKLGroupIdentifier            = 6,

  /*
   * The keyboard layout kind (KeyboardLayoutKind).
   */
  kKLKind                       = 7,

  /*
   * The language/locale string associated with the keyboard, if any
   * (CFStringRef). This string uses ISO 639 and ISO 3166 codes
   * (examples: "fr", "en_US". Note: The CFStringRef may be NULL for
   * some keyboards.
   */
  kKLLanguageCode               = 9
};


/*
 *  KeyboardLayoutKind
 *  
 *  Summary:
 *    Keyboard layout kinds indicate available keyboard layout formats.
 */
typedef SInt32 KeyboardLayoutKind;
enum {

  /*
   * Both KCHR and uchr formats are available.
   */
  kKLKCHRuchrKind               = 0,

  /*
   * Only KCHR format is avaiable.
   */
  kKLKCHRKind                   = 1,

  /*
   * Only uchr format is available.
   */
  kKLuchrKind                   = 2
};


/*
 *  KeyboardLayoutIdentifier
 *  
 *  Summary:
 *    Keyboard layout identifiers specify particular keyboard layouts.
 */
typedef SInt32 KeyboardLayoutIdentifier;
enum {
  kKLUSKeyboard                 = 0
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Keyboard API routines                                                            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  KBGetLayoutType()
 *  
 *  Summary:
 *    Returns the physical keyboard layout type.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    iKeyboardType:
 *      The keyboard type ID.  LMGetKbdType().
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   in KeyboardsLib 1.0 and later
 */
extern PhysicalKeyboardLayoutType 
KBGetLayoutType(SInt16 iKeyboardType)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* iterate keyboard layouts*/

#if !__LP64__
/*
 *  KLGetKeyboardLayoutCount()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCreateInputSourceList API to create a list of input
 *    sources that match specified properties.
 *  
 *  Summary:
 *    Returns the number of keyboard layouts.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    oCount:
 *      On exit, the number of keyboard layouts
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
KLGetKeyboardLayoutCount(CFIndex * oCount)                    AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  KLGetKeyboardLayoutAtIndex()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCreateInputSourceList API to create a list of input
 *    sources that match specified properties.
 *  
 *  Summary:
 *    Retrieves the keyboard layout at the given index.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    iIndex:
 *      The index of the keyboard layout to retrieve. If the index is
 *      outside the index space of the keyboard layouts (0 to N-1
 *      inclusive, where N is the count of the keyboard layouts), the
 *      behavior is undefined.
 *    
 *    oKeyboardLayout:
 *      On exit, the keyboard layout with the given index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
KLGetKeyboardLayoutAtIndex(
  CFIndex              iIndex,
  KeyboardLayoutRef *  oKeyboardLayout)                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
   *** deprecated. ***
   NOTE: "Indexed" is a wrong name, please use "AtIndex"...
   OSStatus KLGetIndexedKeyboardLayout(
                CFIndex                     iIndex,
                KeyboardLayoutRef           *oKeyboardLayout    );
*/

/* get keyboard layout info*/

/*
 *  KLGetKeyboardLayoutProperty()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISGetInputSourceProperty API to query properties of a
 *    TISInputSourceRef.
 *  
 *  Summary:
 *    Retrives property value for the given keyboard layout and tag.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    iKeyboardLayout:
 *      The keyboard layout to be queried. If this parameter is not a
 *      valid KeyboardLayoutRef, the behavior is undefined.
 *    
 *    iPropertyTag:
 *      The property tag.
 *    
 *    oValue:
 *      On exit, the property value for the given keyboard layout and
 *      tag.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
KLGetKeyboardLayoutProperty(
  KeyboardLayoutRef           iKeyboardLayout,
  KeyboardLayoutPropertyTag   iPropertyTag,
  const void **               oValue)                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/* get keyboard layout with identifier or name*/

/*
 *  KLGetKeyboardLayoutWithIdentifier()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCreateInputSourceList API to create a list of input
 *    sources that match specified properties, such as the
 *    kTISPropertyInputSourceID property.
 *  
 *  Summary:
 *    Retrieves the keyboard layout with the given identifier.
 *  
 *  Discussion:
 *    For now, the identifier is in the range of SInt16 which is
 *    compatible with the Resource Manager resource ID. However, it
 *    will become an arbitrary SInt32 value at some point, so do not
 *    assume it is in SInt16 range or falls into the "script range" of
 *    the resource IDs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    iIdentifier:
 *      The keyboard layout identifier.
 *    
 *    oKeyboardLayout:
 *      On exit, the keyboard layout with the given identifier.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
KLGetKeyboardLayoutWithIdentifier(
  KeyboardLayoutIdentifier   iIdentifier,
  KeyboardLayoutRef *        oKeyboardLayout)                 AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  KLGetKeyboardLayoutWithName()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCreateInputSourceList API to create a list of input
 *    sources that match specified properties, such as the
 *    kTISPropertyInputSourceID property.
 *  
 *  Summary:
 *    Retrieves the keyboard layout with the given name.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    iName:
 *      The keyboard layout name.
 *    
 *    oKeyboardLayout:
 *      On exit, the keyboard layout with the given name.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
KLGetKeyboardLayoutWithName(
  CFStringRef          iName,
  KeyboardLayoutRef *  oKeyboardLayout)                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/* get/set current keyboard layout of the current group identifier*/

/*
 *  KLGetCurrentKeyboardLayout()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCopyCurrentKeyboardLayoutInputSource.
 *     TISCopyCurrentKeyboardLayoutInputSource will return the current
 *    input source if it is a keyboard layout.  If the current input
 *    source is an input method or one of its input modes, the TIS API
 *    will also return any keyboard override specified via
 *    TISSetInputMethodKeyboardLayoutOverride.
 *  
 *  Summary:
 *    Retrieves the current keyboard layout.
 *  
 *  Discussion:
 *    Retrieves the current keyboard layout for the current keyboard
 *    script.  To retrive the current keyboard script for Roman
 *    keyboard script, you need to call KeyScript( smRoman |
 *    smKeyForceKeyScriptMask ) then call KLGetCurrentKeyboardLayout().
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    oKeyboardLayout:
 *      On exit, the current keyboard layout.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
KLGetCurrentKeyboardLayout(KeyboardLayoutRef * oKeyboardLayout) AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  KLSetCurrentKeyboardLayout()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    To make a keyboard layout become the current input source, use
 *    TISSelectInputSource API.
 *    
 *    Input methods have used this API to override the keyboard layout
 *    used when the input method or one of its input modes is selected.
 *     TISSetInputMethodKeyboardLayoutOverride should be used for this
 *    purpose and TISCopyCurrentKeyboardLayoutInputSource will return
 *    the keyboard layout specified.
 *    An benefit of using the TIS API to specify an override is that
 *    when the input method is selected, the keyboard layout specified
 *    will be visible to users of the Keyboard Viewer and allow them to
 *    see the underlying keyboard used for event delivery to your input
 *    method.
 *  
 *  Summary:
 *    Sets the current keyboard layout.
 *  
 *  Discussion:
 *    Sets the current keyboard layout for the current keyboard script.
 *     Returns "paramErr" when the current keyboard layout is not
 *    Unicode and the specified keyboard layout belongs to Unicode
 *    group.  To set Roman keyboard script's current keyboard layout to
 *    "U.S." for example, you need to call KeyScript( smRoman |
 *    smKeyForceKeyScriptMask ) then call KLSetCurrentKeyboardLayout(
 *    theUSKeyboardLayoutRef ).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    iKeyboardLayout:
 *      The keyboard layout.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
KLSetCurrentKeyboardLayout(KeyboardLayoutRef iKeyboardLayout) AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  OBSOLETE                                                                        */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* These are obsolete.  Carbon does not support these. */
/* Keyboard API Trap Number. Should be moved to Traps.i */
#endif  /* !__LP64__ */

enum {
  _KeyboardDispatch             = 0xAA7A
};

/* Gestalt selector and values for the Keyboard API */
enum {
  gestaltKeyboardsAttr          = 'kbds',
  gestaltKBPS2Keyboards         = 1,
  gestaltKBPS2SetIDToAny        = 2,
  gestaltKBPS2SetTranslationTable = 4
};

/* Keyboard API Error Codes */
/*
   I stole the range blow from the empty space in the Allocation project but should
   be updated to the officially registered range.
*/
enum {
  errKBPS2KeyboardNotAvailable  = -30850,
  errKBIlligalParameters        = -30851,
  errKBFailSettingID            = -30852,
  errKBFailSettingTranslationTable = -30853,
  errKBFailWritePreference      = -30854
};


#ifdef __cplusplus
}
#endif

#endif /* __KEYBOARDS__ */

