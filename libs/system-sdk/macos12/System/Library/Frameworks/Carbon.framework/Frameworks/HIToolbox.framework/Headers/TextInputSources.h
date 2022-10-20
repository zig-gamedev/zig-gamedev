/*
     File:       HIToolbox/TextInputSources.h
 
     Copyright:  © 2006-2008 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTINPUTSOURCES__
#define __TEXTINPUTSOURCES__

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


/*
 *  TextInputSources.h
 *  
 *  Summary:
 *    Specifies the modern, non-Script-Manager-based interfaces for
 *    operating on text input sources: finding information about them,
 *    selecting/enabling/disabling them, and receiving notifications
 *    about relevant changes.
 *  
 *  Discussion:
 *    Text input sources are of three general categories: keyboard
 *    input sources (keyboard layouts, keyboard input methods and input
 *    modes), palette input sources (character palette, keyboard
 *    viewer, private dictionary panels), and ink. Palette input
 *    sources and ink input sources are categorized as non-keyboard
 *    input sources, although palette input sources may still involve
 *    some keyboard interaction. Keyboard input methods may be
 *    mode-enabled (e.g. Kotoeri), in which case they may be the parent
 *    of several input modes which are directly selectable in the user
 *    interface (e.g. hiragana, katakana, romaji); in this case the
 *    parent input method is not directly selectable. Non-mode-enabled
 *    input methods are directly selectable. 
 *    
 *    
 *    Some input sources are invisible to system UI; they do not appear
 *    in the normal user interface for manipulating input sources.
 *    Examples include input sources such as ink, the dictionary panel,
 *    and some assistiveware; these have their own special UI for
 *    enabling, disabling, etc. Other examples include special keyboard
 *    layouts used by input methods, which have their own ways of
 *    controlling use of these keyboard layouts. 
 *    
 *    
 *    Some input sources are pre-installed by Apple. Other input
 *    sources (of any type) may be installed as third party products
 *    themselves or as part of installing certain applications. Most
 *    non-invisible input sources may be enabled or disabled by users
 *    using International Preferences, which displays a list of all
 *    visible installed input sources (Setup Assistant also enables
 *    some input sources); there is a separate UI for enabling ink.
 *    Applications may enable or disable input sources programmatically
 *    using some of the functions here. At least one keyboard input
 *    source is enabled (the system ensures this). At most one ink
 *    input source may be enabled; multiple instances of other input
 *    source types may be enabled. 
 *    
 *    
 *    Some enabled input sources are invisible but programmatically
 *    selectable, such as ink. Some are visible but not
 *    programmatically selectable, such as mode-savvy parent input
 *    methods (which must be visible so that International Preferences
 *    can display the parent input method for a group of input modes).
 *    
 *    
 *    
 *    Input modes can only be changed from disabled to enabled if their
 *    parent input method is enabled. Input modes can only be selected
 *    if both they and their parent input method are enabled. 
 *     
 *    
 *    Exactly one keyboard input source is selected at any time; this
 *    is the current keyboard input source. Selecting a new keyboard
 *    input source deselects the previous keyboard input source.
 *    Multiple palette input sources may be selected - e.g. there may
 *    be one or more character palettes and one or more keyboard
 *    viewers selected in addition to the selected keyboard input
 *    source. Selecting or deselecting a palette (or ink) input source
 *    does not affect any other input source. Input methods that
 *    provide associated input palettes may programmatically deselect
 *    the palette when the input method is deselected, for example.
 *
 *
 *    Mac OS X threading:
 *    TextInputSources API is not thread safe. If you are a UI application, you must call TextInputSources API on the main thread.
 *    If you are a non-UI application (such as a cmd-line tool or a launchagent that does not run an event loop), you must not call TextInputSources API from multiple
 *    threads concurrently.
 */
/*
*===============================================================================
*   Basic type
*===============================================================================
*/

/*
 *  TISInputSourceRef
 *  
 *  Summary:
 *    Opaque CF object that unambiguously (within a single process)
 *    represents a text input source.
 *  
 *  Discussion:
 *    Cannot be shared cross-process.
 */
typedef struct __TISInputSource*        TISInputSourceRef;
/*
 *  TISInputSourceGetTypeID()
 *  
 *  Summary:
 *    Gets the CFTypeID of a TISInputSourceRef.
 *  
 *  Result:
 *    Returns the CFTypeID of a TISInputSourceRef, for comparison with
 *    the result of CFGetTypeID().
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
TISInputSourceGetTypeID(void)                                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
*===============================================================================
*   Property key constants,
*
*   Input sources may have additional properties beyond those listed here,
*   and some input sources do not have values for some of the properties
*   listed here. The property value for a particular input source can be
*   obtained using TISGetInputSourceProperty. A set of specific property
*   key-value pairs can also be used as a filter when creating a list of
*   input sources using TISCreateInputSourceList.
*
*   The following keys may be used with both TISGetInputSourceProperty and
*   TISCreateInputSourceList:
*===============================================================================
*/
/*
 *  kTISPropertyInputSourceCategory
 *  
 *  Summary:
 *    The property key constant for a CFStringRef value that indicates
 *    the category of input source.
 *  
 *  Discussion:
 *    The possible values are specified by property value constants
 *    kTISCategoryKeyboardInputSource, kTISCategoryPaletteInputSource,
 *    kTISCategoryInkInputSource.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceCategory             AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputSourceType
 *  
 *  Summary:
 *    The property key constant for a CFStringRef value that indicates
 *    the specific type of input source.
 *  
 *  Discussion:
 *    The possible values are specified by property value constants
 *    kTISTypeKeyboardLayout, kTISTypeKeyboardInputMethodWithoutModes,
 *    kTISTypeKeyboardInputMethodModeEnabled,
 *    kTISTypeKeyboardInputMode, kTISTypeCharacterPalette,
 *    kTISTypeKeyboardViewer, kTISTypeInk.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceType                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputSourceIsASCIICapable
 *  
 *  Summary:
 *    The property key constant for a CFBooleanRef value that indicates
 *    whether the input source identifies itself as ASCII-capable.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceIsASCIICapable       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputSourceIsEnableCapable
 *  
 *  Summary:
 *    The property key constant for a CFBooleanRef value that indicates
 *    whether the input source can ever (given the right conditions) be
 *    programmatically enabled using TISEnableInputSource.
 *  
 *  Discussion:
 *    This is a static property of an input source, and does not depend
 *    on any current state. 
 *    
 *    Most input sources can be programmatically enabled at any time;
 *    kTISPropertyInputSourceIsEnableCapable is true for these.
 *    
 *    
 *    Some input sources can never be programmatically enabled. These
 *    are mainly input method private keyboard layouts that are used by
 *    the input method via TISSetInputMethodKeyboardLayoutOverride, but
 *    which cannot be directly enabled and used as keyboard layout
 *    input sources. kTISPropertyInputSourceIsEnableCapable is false
 *    for these. 
 *    
 *    Some input sources can only be programmatically enabled under the
 *    correct conditions. These are mainly input modes, which can only
 *    be changed from disabled to enabled if their parent input method
 *    is enabled (however, they can already be in the enabled state -
 *    but not currently selectable - if their parent input method is
 *    disabled). kTISPropertyInputSourceIsEnableCapable is true for
 *    these.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceIsEnableCapable      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputSourceIsSelectCapable
 *  
 *  Summary:
 *    The property key constant for a CFBooleanRef value that indicates
 *    whether the input source can ever (given the right conditions) be
 *    programmatically selected using TISSelectInputSource.
 *  
 *  Discussion:
 *    This is a static property of an input source, and does not depend
 *    on any current state. 
 *    
 *    Most input sources can be programmatically selected if they are
 *    enabled; kTISPropertyInputSourceIsSelectCapable is true for
 *    these. 
 *    
 *    Some input sources can never be programmatically selected even if
 *    they are enabled. These are mainly input methods that have modes
 *    (parent input methods); only their modes can be selected.
 *    kTISPropertyInputSourceIsSelectCapable is false for these.
 *    
 *    
 *    Some input sources which are enabled can only be programmatically
 *    selected under the correct conditions. These are mainly input
 *    modes, which can only be selected if both they and their parent
 *    input method are enabled.  kTISPropertyInputSourceIsSelectCapable
 *    is true for these. 
 *    
 *    Input source which can never be enabled - i.e. for which
 *    kTISPropertyInputSourceIsEnableCapable is false - can also never
 *    be selected. kTISPropertyInputSourceIsSelectCapable is false for
 *    these.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceIsSelectCapable      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputSourceIsEnabled
 *  
 *  Summary:
 *    The property key constant for a CFBooleanRef value that indicates
 *    whether the input source is currently enabled.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceIsEnabled            AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputSourceIsSelected
 *  
 *  Summary:
 *    The property key constant for a CFBooleanRef value that indicates
 *    whether the input source is currently selected.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceIsSelected           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputSourceID
 *  
 *  Summary:
 *    The property key constant for a CFStringRef value for the unique
 *    reverse DNS name associated with the input source.
 *  
 *  Discussion:
 *    1. For keyboard input methods and for input sources of the
 *    palette or ink category, this is typically the bundle ID, e.g.
 *    "com.apple.Kotoeri". 
 *    
 *    2. For keyboard input modes, this is typically the bundle ID of
 *    the parent input method plus a suffix that uniquely identifies
 *    the input mode, e.g. "com.apple.Kotoeri.Katakana" (it is not the
 *    generic input mode name used across input methods, e.g.
 *    "com.apple.inputmethod.Japanese.Katakana"). 
 *    
 *    3. For keyboard layouts this is a new identification mechanism
 *    typically structured as "com.company.keyboardlayout.name", e.g.
 *    "com.apple.keyboardlayout.US".
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceID                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyBundleID
 *  
 *  Summary:
 *    The property key constant for a CFStringRef value for the reverse
 *    DNS BundleID associated with the input source.
 *  
 *  Discussion:
 *    Not valid for all input sources (especially some keyboard
 *    layouts).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyBundleID                        AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyInputModeID
 *  
 *  Summary:
 *    The property key constant for a CFStringRef value that identifies
 *    a particular usage class for input modes.
 *  
 *  Discussion:
 *    For example, "com.apple.inputmethod.Japanese.Katakana" identifies
 *    a standard Katakana-input usage class that may be associated with
 *    input modes from several different input methods. 
 *    
 *    This InputModeID can be attached to a TSMDocument using
 *    TSMSetDocumentProperty with the tag
 *    kTSMDocumentInputModePropertyTag, in order to control which input
 *    mode usage class should be used with that TSMDocument.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputModeID                     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyLocalizedName
 *  
 *  Summary:
 *    The property key constant for a CFStringRef value for the input
 *    source's localized name for UI purposes.
 *  
 *  Discussion:
 *    Uses the best match (determined by CFBundle) between the
 *    localization being used by the caller and the available
 *    localizations of the input source name. In some cases this may
 *    fall back to an unlocalized name.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyLocalizedName                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
*===============================================================================
*   The following keys may be used with TISGetInputSourceProperty, but may NOT
*   be used in the filter dictionary passed to TISCreateInputSourceList:
*===============================================================================
*/
/*
 *  kTISPropertyInputSourceLanguages
 *  
 *  Summary:
 *    The property key constant for a value which is a CFArrayRef of
 *    CFStringRefs, where each CFString is the language code for a
 *    language that can be input using the input source.
 *  
 *  Discussion:
 *    Languages codes are in the same BCP 47 format as returned by
 *    CFLocaleCreateCanonicalLanguageIdentifierFromString. The first
 *    language code in the array is the language for which the input
 *    source is intended. If there is no such language (e.g. for the
 *    Unicode Hex Input keyboard layout), the first language code is an
 *    empty string. 
 *    
 *    NOTE: This key (and a corresponding value) may not be used in the
 *    filter dictionary passed to TISCreateInputSourceList.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyInputSourceLanguages            AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyUnicodeKeyLayoutData
 *  
 *  Summary:
 *    The property key constant for a value which is a CFDataRef that
 *    refers to the 'uchr' keyboard layout data for a keyboard layout
 *    input source.
 *  
 *  Discussion:
 *    The uchr data is in native-endian order. If the input source is
 *    not a keyboard layout, or is a keyboard layout for which only
 *    'KCHR data' is available, the value is NULL. 
 *    
 *    NOTE: This key (and a corresponding value) may not be used in the
 *    filter dictionary passed to TISCreateInputSourceList.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyUnicodeKeyLayoutData            AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyIconRef
 *  
 *  Summary:
 *    The property key constant for an IconRef value for the input
 *    source icon.
 *  
 *  Discussion:
 *    IconRefs are the normal icon format for keyboard layouts and
 *    input methods. If an IconRef is not available for the specified
 *    input source, the value is NULL. 
 *    
 *    NOTE: This key (and a corresponding value) may not be used in the
 *    filter dictionary passed to TISCreateInputSourceList.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyIconRef                         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISPropertyIconImageURL
 *  
 *  Summary:
 *    The property key constant for a CFURLRef value indicating the
 *    file containing the image (typically TIFF) to be used as the
 *    input source icon.
 *  
 *  Discussion:
 *    TIFF files are the normal icon format for input modes. If an
 *    image file URL is not available for the specified input source,
 *    the value will be NULL. Note that other image formats (e.g. JPEG,
 *    PNG, PDF) may also be supported.
 *    
 *    Clients should be prepared for a URL to be unreachable, such as when
 *    an Input Method Info.plist mis-declares its icon path extension in its Info.plist.
 *    In this case, the client should try other path extensions, by using, for example,
 *    a combination of CFURLResourceIsReachable, CFURLCopyPathExtension,
 *    CFURLCreateCopyDeletingPathExtension, and CFURLCreateCopyAppendingPathExtension.
 *        For example, if the URL indicates ".png", be prepared to look for a ".tiff".
 *    TIS uses [NSBundle(NSBundleImageExtension) imageForResource:],
 *    where possible, to obtain an input source image, so the path extension (i.e. ".png")
 *    is not critical for the System to find and display the image properly.
 *    
 *    NOTE: This key (and a corresponding value) may not be used in the
 *    filter dictionary passed to TISCreateInputSourceList.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISPropertyIconImageURL                    AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
*===============================================================================
*   Property value constants for input source category
*===============================================================================
*/
/*
 *  kTISCategoryKeyboardInputSource
 *  
 *  Summary:
 *    The property value constant for one input source category value
 *    associated with the property key kTISPropertyInputSourceCategory.
 *  
 *  Discussion:
 *    This category includes keyboard layouts, keyboard input methods
 *    (both with modes and without), and keyboard input modes. At least
 *    one input source in this category is installed. Of all input
 *    sources in this category, exactly one is selected; selecting a
 *    new one deselects the previous one.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISCategoryKeyboardInputSource             AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISCategoryPaletteInputSource
 *  
 *  Summary:
 *    The property value constant for one input source category value
 *    associated with the property key kTISPropertyInputSourceCategory.
 *  
 *  Discussion:
 *    This category includes character palettes and keyboard viewers.
 *    Zero or more of these can be selected.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISCategoryPaletteInputSource              AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISCategoryInkInputSource
 *  
 *  Summary:
 *    The property value constant for one input source category value
 *    associated with the property key kTISPropertyInputSourceCategory.
 *  
 *  Discussion:
 *    Zero or one ink input sources can be installed and selected.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISCategoryInkInputSource                  AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
*===============================================================================
*   Property value constants for input source type
*===============================================================================
*/
/*
 *  kTISTypeKeyboardLayout
 *  
 *  Summary:
 *    The property value constant for one input source type value
 *    associated with the property key kTISPropertyInputSourceType.
 *  
 *  Discussion:
 *    This type belongs to the category kTISCategoryKeyboardInputSource.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISTypeKeyboardLayout                      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISTypeKeyboardInputMethodWithoutModes
 *  
 *  Summary:
 *    The property value constant for one input source type value
 *    associated with the property key kTISPropertyInputSourceType.
 *  
 *  Discussion:
 *    This type belongs to the category kTISCategoryKeyboardInputSource.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISTypeKeyboardInputMethodWithoutModes     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISTypeKeyboardInputMethodModeEnabled
 *  
 *  Summary:
 *    The property value constant for one input source type value
 *    associated with the property key kTISPropertyInputSourceType.
 *  
 *  Discussion:
 *    This type belongs to the category kTISCategoryKeyboardInputSource.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISTypeKeyboardInputMethodModeEnabled      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISTypeKeyboardInputMode
 *  
 *  Summary:
 *    The property value constant for one input source type value
 *    associated with the property key kTISPropertyInputSourceType.
 *  
 *  Discussion:
 *    This type belongs to the category kTISCategoryKeyboardInputSource.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISTypeKeyboardInputMode                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISTypeCharacterPalette
 *  
 *  Summary:
 *    The property value constant for one input source type value
 *    associated with the property key kTISPropertyInputSourceType.
 *  
 *  Discussion:
 *    This type belongs to the category kTISCategoryPaletteInputSource.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISTypeCharacterPalette                    AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISTypeKeyboardViewer
 *  
 *  Summary:
 *    The property value constant for one input source type value
 *    associated with the property key kTISPropertyInputSourceType.
 *  
 *  Discussion:
 *    This type belongs to the category kTISCategoryPaletteInputSource.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISTypeKeyboardViewer                      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISTypeInk
 *  
 *  Summary:
 *    The property value constant for one input source type value
 *    associated with the property key kTISPropertyInputSourceType.
 *  
 *  Discussion:
 *    This type belongs to the category kTISCategoryInkInputSource.
 *    Even though it is the only type in that category, a type is
 *    provided so that clients who don’t need category information can
 *    just check input source type.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISTypeInk                                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
*===============================================================================
*   Find out information about text input sources
*===============================================================================
*/
/*
 *  TISGetInputSourceProperty()
 *  
 *  Summary:
 *    Gets value of specified property for specified input source.
 *  
 *  Parameters:
 *    
 *    inputSource:
 *      The text input source for which a property value is requested.
 *    
 *    propertyKey:
 *      The property key constant specifying the desired property value.
 *  
 *  Result:
 *    Returns a pointer type appropriate for value object associated
 *    with the property key. The specific pointer type is specified for
 *    each key. Typically it is a CFTypeRef of some sort, but in one
 *    case it is IconRef. The function may return NULL if the specified
 *    property is missing or invalid for the specified input source.
 *    The objects referred to by the pointer follow the "Get" rule and
 *    should not be be released by the caller (unless first retained by
 *    the caller).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void * 
TISGetInputSourceProperty(
  TISInputSourceRef   inputSource,
  CFStringRef         propertyKey)                            AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISCreateInputSourceList()
 *  
 *  Summary:
 *    Creates a list of input sources that match specified properties.
 *  
 *  Discussion:
 *    This list represents a snapshot of input sources that matched the
 *    specified properties at the time the call was made. If the caller
 *    desires to include input sources that are installed but not
 *    currently enabled, the includeAllInstalled parameter may be set
 *    true. Typically this is done in order to obtain a
 *    TISInputSourceRef for a newly-installed input source; in this
 *    case the properties parameter would include very specific
 *    criteria limiting the matching input sources. 
 *    
 *    Warning: Calling this with includeAllInstalled true can have
 *    significant memory impact on the calling application if the
 *    properties parameter is NULL (match all) or if it specifies
 *    criteria that may match many installed input sources, since this
 *    may force caching of data for all matching input sources (which
 *    can result in allocation of up to 120K). If
 *    TISCreateInputSourceList is being called in order to find a
 *    specific input source or sources from among the sources included
 *    in the list, then it is best to first call
 *    TISCreateInputSourceList with includeAllInstalled = false and
 *    check whether the returned array includes the desired input
 *    source(s); if not, then call TISCreateInputSourceList again with
 *    includeAllInstalled = true.
 *  
 *  Parameters:
 *    
 *    properties:
 *      Dictionary of property keys and corresponding values to filter
 *      the input source list. May be NULL, in which case no filtering
 *      is performed.
 *    
 *    includeAllInstalled:
 *      Normally false so that only enabled input sources will be
 *      included; set true to include all installed input sources that
 *      match the filter (see discussion).
 *  
 *  Result:
 *    Returns a CFArrayRef for a list of TISInputSourceRefs that match
 *    the specified properties.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
TISCreateInputSourceList(
  CFDictionaryRef   properties,
  Boolean           includeAllInstalled)                      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
*===============================================================================
*   Get specific input sources
*===============================================================================
*/
/*
 *  TISCopyCurrentKeyboardInputSource()
 *  
 *  Summary:
 *    Copies a TISInputSourceRef for the currently-selected keyboard
 *    input source; convenience function.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern TISInputSourceRef 
TISCopyCurrentKeyboardInputSource(void)                       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISCopyCurrentKeyboardLayoutInputSource()
 *  
 *  Summary:
 *    Copies a TISInputSourceRef for the keyboard layout currently
 *    being used. If the currently-selected keyboard input source is a
 *    keyboard layout, the TISInputSourceRef refers to that layout; if
 *    the currently-selected keyboard input source is an input method
 *    or mode, the TISInputSourceRef refers to the keyboard layout
 *    being used by that input method or mode.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern TISInputSourceRef 
TISCopyCurrentKeyboardLayoutInputSource(void)                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISCopyCurrentASCIICapableKeyboardInputSource()
 *  
 *  Summary:
 *    Copies a TISInputSourceRef for the most-recently-used
 *    ASCII-capable keyboard input source.
 *  
 *  Discussion:
 *    If no ASCII-capable keyboard input source has been used yet,
 *    returns the default ASCII-capable keyboard layout (chosen by
 *    Setup Assistant).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern TISInputSourceRef 
TISCopyCurrentASCIICapableKeyboardInputSource(void)           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISCopyCurrentASCIICapableKeyboardLayoutInputSource()
 *  
 *  Summary:
 *    Copies a TISInputSourceRef for the most-recently-used
 *    ASCII-capable keyboard layout.
 *  
 *  Discussion:
 *    If no ASCII-capable keyboard input source has been used yet,
 *    returns the default ASCII-capable keyboard layout (chosen by
 *    Setup Assistant). 
 *    
 *    This is used by input methods to get the keyboard layout that
 *    will be used for key translation if there is no specific keyboard
 *    layout override. 
 *    
 *    Note the similar TISCopyCurrentASCIICapableKeyboardInputSource,
 *    which can return input sources that are not keyboard layouts.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern TISInputSourceRef 
TISCopyCurrentASCIICapableKeyboardLayoutInputSource(void)     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISCopyInputSourceForLanguage()
 *  
 *  Summary:
 *    Copies a TISInputSourceRef for the input source that should be
 *    used to input the specified language.
 *  
 *  Discussion:
 *    Sample usage: If a text field is expected to have input in a
 *    particular language, an application can call
 *    TISCopyInputSourceForLanguage and then TISSelectInputSource to
 *    select an input source that would be appropriate for that
 *    language. 
 *    
 *    This is intended to provide a replacement for one way in which
 *    the now-deprecated KeyScript API was used: Selection of the
 *    default input source associated with a particular ScriptCode.
 *  
 *  Parameters:
 *    
 *    language:
 *      A language tag in BCP 47 format (i.e. in the same form as
 *      returned by
 *      CFLocaleCreateCanonicalLanguageIdentifierFromString) that
 *      represents the language for which an input source should be
 *      returned.
 *  
 *  Result:
 *    TISInputSourceRef for an enabled input source that can input the
 *    specified language. If there is more than one such input source
 *    and at least one has previously been used, then the
 *    most-recently-used one will be chosen. If none of them has
 *    previously been used, one will be chosen based on the intended
 *    languages of the input sources. If there is no enabled input
 *    source that can input the specified language, the function will
 *    return NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern TISInputSourceRef 
TISCopyInputSourceForLanguage(CFStringRef language)           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISCreateASCIICapableInputSourceList()
 *  
 *  Summary:
 *    Creates a list of ASCII capable keyboard input sources.
 *  
 *  Discussion:
 *    This list represents a snapshot of ASCII capable keyboard input
 *    sources that were enabled at the time the call was made.
 *    Successive calls to TISCreateASCIICapableInputSourceList may
 *    return different results because, for example, in between the
 *    calls the user may enable or disable an input source in the
 *    International Preferences pane. When a keyboard input source is
 *    enabled or disabled, whether by the user or programmatically, the
 *    kTISNotifyEnabledKeyboardInputSourcesChanged CF distributed
 *    notification is posted.
 *  
 *  Result:
 *    Returns a CFArrayRef containing a list of TISInputSourceRefs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
TISCreateASCIICapableInputSourceList(void)                    AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
*===============================================================================
*   Manipulate input sources
*===============================================================================
*/
/*
 *  TISSelectInputSource()
 *  
 *  Summary:
 *    Selects the specified input source.
 *  
 *  Discussion:
 *    Calling TISSelectInputSource on a keyboard input source that can
 *    be selected makes the specified input source the new current
 *    keyboard input source, and deselects the previous one. Calling
 *    TISSelectInputSource on a palette input source usually results in
 *    the palette being displayed and available for input. Ink input
 *    sources are typically enabled and selected at the same time.
 *    Calling TISSelectInputSource on a palette or ink input source has
 *    no effect on other input sources. Calling TISSelectInputSource
 *    for an already-selected input source has no effect. 
 *    
 *    For TISSelectInputSource to succeed, the input source must be
 *    capable of being selected (kTISPropertyInputSourceIsSelectCapable
 *    must be true) and the input source must be enabled
 *    (kTISPropertyInputSourceIsEnabled must be true). Furthermore, if
 *    if the input source is an input mode, its parent must be enabled
 *    for it to be selected.
 *  
 *  Result:
 *    Returns an error code: paramErr if the input source is not
 *    selectable, else noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TISSelectInputSource(TISInputSourceRef inputSource)           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISDeselectInputSource()
 *  
 *  Summary:
 *    Deselects the specified input source.
 *  
 *  Discussion:
 *    TISDeselectInputSource is only intended for use with palette or
 *    ink input sources; calling it has no effect on other input
 *    sources. When palette input sources are disabled, the palette
 *    disappears. Ink input sources are usually deselected and disabled
 *    at the same time.
 *  
 *  Result:
 *    Returns an error code: paramErr if the input source is not
 *    deselectable, else noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TISDeselectInputSource(TISInputSourceRef inputSource)         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISEnableInputSource()
 *  
 *  Summary:
 *    Enables the specified input source.
 *  
 *  Discussion:
 *    TISEnableInputSource is mainly intended for input methods, or for
 *    applications that supply their own input sources (e.g.
 *    applications that provide keyboard layouts or palette input
 *    methods, and keyboard input methods that provide their own
 *    keyboard layouts and/or input modes). It makes the specified
 *    input source available in UI for selection. 
 *    
 *    For TISEnableInputSource to succeed, the input source must be
 *    capable of being enabled (kTISPropertyInputSourceIsEnableCapable
 *    must be true). Furthermore, if the input source is an input mode,
 *    its parent must already be enabled for the mode to become enabled.
 *  
 *  Result:
 *    Returns an error code: paramErr if the input source cannot be
 *    enabled, else noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TISEnableInputSource(TISInputSourceRef inputSource)           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISDisableInputSource()
 *  
 *  Summary:
 *    Disables the specified input source.
 *  
 *  Discussion:
 *    TISDisableInputSource is mainly intended for input methods, or
 *    for applications that supply their own input sources (e.g.
 *    applications that provide keyboard layouts or palette input
 *    methods, and keyboard input methods that provide their own
 *    keyboard layouts and/or input modes). It makes the specified
 *    input source unavailable for selection, and removes it from
 *    system UI.
 *  
 *  Result:
 *    Returns an error code: paramErr if the input source cannot be
 *    disabled, else noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TISDisableInputSource(TISInputSourceRef inputSource)          AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
*===============================================================================
*   Notification constants
*===============================================================================
*/
/*
 *  kTISNotifySelectedKeyboardInputSourceChanged
 *  
 *  Summary:
 *    The name of the CF distributed notification for a change to the
 *    selected keyboard input source.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISNotifySelectedKeyboardInputSourceChanged AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kTISNotifyEnabledKeyboardInputSourcesChanged
 *  
 *  Summary:
 *    The name of the CF distributed notification for a change to the
 *    set of enabled keyboard input sources.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kTISNotifyEnabledKeyboardInputSourcesChanged AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
*===============================================================================
*   Allow input method to override keyboard layout
*===============================================================================
*/
/*
 *  TISSetInputMethodKeyboardLayoutOverride()
 *  
 *  Summary:
 *    Sets the keyboard layout override for an input method or mode.
 *  
 *  Discussion:
 *    When an input method or mode is the selected input source, TSM
 *    will by default use the most-recently-used ASCII-capable keyboard
 *    layout to translate key events* (this keyboard layout is also the
 *    one that will appear in Keyboard Viewer); an input source for
 *    this keyboard layout is returned by
 *    TISCopyCurrentASCIICapableKeyboardLayout. If a different keyboard
 *    layout should be used for a particular input method or mode, then
 *    when that input method/mode is activated it should call
 *    TISSetInputMethodKeyboardLayoutOverride to specify the desired
 *    keyboard layout. 
 *    
 *    For example, when a Kotoeri user selects kana layout for kana
 *    input, Kotoeri should call
 *    TISSetInputMethodKeyboardLayoutOverride to set the kana keyboard
 *    as the override for the appropriate input modes. 
 *    
 *    The keyboard layout set in this way will be used for the final
 *    stage of key translation in the Window Server - the connection or
 *    application-specific key translation. 
 *    
 *    The override setting is lost when the input method that set it is
 *    deactivated. 
 *    
 *    The keyboardLayout to be used for overriding need not be enabled
 *    or explicitly selectable. It can be a non-selectable layout that
 *    is included in an input method bundle and automatically
 *    registered. 
 *    
 *    *The default behavior is new for Mac OS X 10.5, and is meant to
 *    eliminate the necessity for input methods to have UI for setting
 *    which ASCII- capable keyboard to use for latin-character-based
 *    phonetic input.
 *  
 *  Parameters:
 *    
 *    keyboardLayout:
 *      TISInputSourceRef for the keyboard layout that should be used
 *      until the current input method is deactivated (if it should be
 *      something other than the most-recently-used ASCII-capable
 *      keyboard layout).
 *  
 *  Result:
 *    Returns an error code: paramErr if the current keyboard input
 *    source is not an input method/mode or if keyboardLayout does not
 *    designate a keyboard layout, else noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TISSetInputMethodKeyboardLayoutOverride(TISInputSourceRef keyboardLayout) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TISCopyInputMethodKeyboardLayoutOverride()
 *  
 *  Summary:
 *    Copies a TISInputSourceRef for the currently-selected input
 *    method's keyboard layout override, if any.
 *  
 *  Result:
 *    If the current keyboard input source is an input method or mode
 *    that has a keyboard layout override, then a TISInputSourceRef for
 *    that keyboard layout is returned; otherwise, NULL is returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern TISInputSourceRef 
TISCopyInputMethodKeyboardLayoutOverride(void)                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
*===============================================================================
*   Install/register an input source
*===============================================================================
*/
/*
 *  TISRegisterInputSource()
 *  
 *  Summary:
 *    Registers the new input source(s) in a file or bundle so that a
 *    TISInputSourceRef can immediately be obtained for each of the new
 *    input source(s).
 *  
 *  Discussion:
 *    This allows an installer for an input method bundle or a keyboard
 *    layout file or bundle to notify the system that these new input
 *    sources should be registered. The system can then locate the
 *    specified file or bundle and perform any necessary cache rebuilds
 *    so that the installer can immediately call
 *    TISCreateInputSourceList with appropriate properties (e.g.
 *    BundleID or InputSourceID) in order to get TISInputSourceRefs for
 *    one or more of the newly registered input sources. 
 *    
 *    This can only be used to register the following: 
 *    
 *    - Keyboard layout files or bundles in "/Library/Keyboard
 *    Layouts/" or "~/Library/Keyboard Layouts/" (available to all
 *    users or current user, respectively). Such keyboard layouts, once
 *    enabled, are selectable. 
 *    
 *    - Input method bundles in the new "/Library/Input Methods/" or
 *    "~/Library/Input Methods/" directories (available to all users or
 *    current user, respectively). 
 *    
 *    Note: Input method bundles can include private non-selectable
 *    keyboard layouts for use with
 *    TISSetInputMethodKeyboardLayoutOverride. These are registered
 *    automatically when the input method is registered, and do not
 *    need to be separately registered. 
 *    
 *    Security: Any code that calls TISRegisterInputSource is part of
 *    an application or service that has already been validated in some
 *    way (e.g. by the user).
 *  
 *  Parameters:
 *    
 *    location:
 *      CFURLRef for the location of the input source(s), a file or
 *      bundle.
 *  
 *  Result:
 *    Error code: paramErr if location is invalid or the input
 *    source(s) in the specified location cannot be registered;
 *    otherwise noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TISRegisterInputSource(CFURLRef location)                     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
*===============================================================================
*   Text Input Sources properties and bundle-packaged text input sources
*===============================================================================
*
*   For Leopard, there are two new keys for use in plists to provide information
*   that supports the Text Input Sources functions above (these keys will be
*   ignored in earlier systems):
*
*   "TISInputSourceID" - a key to specify the InputSourceID, a reverse-DNS-style
*   string meant to uniquely identify any input source. If this key is not
*   specified, the Text Input Sources functions will attempt to construct an
*   InputSourceID from other information.
*
*   "TISIntendedLanguage" - a key to specify the primary language which the
*   input source is intended to input. If there is none - as with the Unicode
*   Hex Input key layout, for example - this key need not be specified. The
*   language is indicated by a string in in the format described by BCP 47
*   (the successor to RFC 3066).
*
*   How these keys are used depends on the type of input source, as described
*   below.
*
*   1. Keyboard layouts ( in <domain>/Library/Keyboard Layouts/ )
*
*   Keyboard layouts packaged in bundles use either a resource file or a set of
*   xml keylayout files together with optional icns files. The following example
*   shows the two methods of packaging a set of two key layouts in Unicode 'uchr'
*   format with key layout names "MyLayoutOne" and "MyLayoutTwo" and corresponding
*   numeric IDs -9001 and -9002 (see Tech Note 2056).
*
*   MyKeyboardLayouts.bundle/
*       Contents/
*           Info.plist
*           version.plist
*           Resources/
*               MyKeyboardLayouts.rsrc, containing the following resources:
*                   resources 'uchr' (-9001, "MyLayoutOne"), 'kcs#' (-9001), 'kcs4' (-9001)
*                   resources 'uchr' (-9002, "MyLayoutTwo"), 'kcs#' (-9002), 'kcs4' (-9002)
*               en.lproj/InfoPlist.strings, maps "MyLayoutOne" & "MyLayoutTwo" to localized names
*               ja.lproj/InfoPlist.strings, maps "MyLayoutOne" & "MyLayoutTwo" to localized names
*               ...
*
*   MyKeyboardLayouts.bundle/
*       Contents/
*           Info.plist
*           version.plist
*           Resources/
*               MyLayoutOne.keylayout, specifying name="MyLayoutOne" and id=-9001
*               MyLayoutOne.icns (optional)
*               MyLayoutTwo.keylayout, specifying name="MyLayoutTwo" and id=-9002
*               MyLayoutTwo.icns (optional)
*               en.lproj/InfoPlist.strings, maps "MyLayoutOne" & "MyLayoutTwo" to localized names
*               ja.lproj/InfoPlist.strings, maps "MyLayoutOne" & "MyLayoutTwo" to localized names
*               ...
*
*   In the Info.plist file, the value for the CFBundleIdentifier key must be a
*   string that includes ".keyboardlayout."; typically this might be something
*   like "com.companyname.keyboardlayout.MyKeyboardLayouts" (Before Leopard,
*   it was required to be a string that began "com.apple.keyboardlayout", even
*   for keyboard layouts not supplied by Apple).
*
*   A dictionary of properties for each key layout in the bundle should be
*   provided using a key of the form "KLInfo_keylayoutname" (even if
*   keylayoutname includes spaces or punctuation). This dictionary is where to
*   specify the keys "TISInputSourceID" and "TISIntendedLanguage" and their
*   associated values.
*
*   "TISInputSourceID" note: For keyboard layouts this should typically be
*   something like "com.companyname.keylayout.keylayoutname". If this key is
*   not specified, an InputSourceID will be constructed by combining
*   bundleID + ". keylayout." + keylayoutname.
*
*   If the keyboard layouts in the above example were intended to input
*   Azerbaijani in Latin script, then the Info.plist entries could be:
*
*       <key>KLInfo_MyLayoutOne</key>
*       <dict>
*           <key>TISInputSourceID</key>
*           <string>com.companyname.keylayout.MyLayoutOne</string>
*           <key>TISIntendedLanguage</key>
*           <string>az-Latn</string>
*       </dict>
*       <key>KLInfo_MyLayoutTwo</key>
*       <dict>
*           <key>TISInputSourceID</key>
*           <string>com.companyname.keylayout.MyLayoutTwo</string>
*           <key>TISIntendedLanguage</key>
*           <string>az-Latn</string>
*       </dict>
*
*   2. Input methods
*
*   Input methods are always packaged as bundles, either as Component bundles
*   in "<domain>/Library/Components/" (the old way, still supported in Leopard)
*   or as application bundles in "<domain>/Library/Input Methods/" (new for
*   Leopard).
*
*   The new keys keys "TISInputSourceID" and "TISIntendedLanguage" and their
*   associated values are added at the top level of the Info.plist file.
*
*   "TISInputSourceID" note: For input methods this is typically the same as
*   the BundleID, and if this key is not specified the BundleID will be used
*   as the InputSourceID.
*
*   3. Input modes
*
*   An input method's input modes are defined using the "ComponentInputModeDict"
*   key at the top level of the input method's Info.plist file (even for
*   non-component application-based input methods). The value of this key is a
*   dictionary, one of whose keys is "tsInputModeListKey"; the value of this
*   key is also a dictionary of input modes, with the InputModeID as the key
*   and the input mode's dictionary as the value (see TextServices.h).
*
*   The new keys keys "TISInputSourceID" and "TISIntendedLanguage" and their
*   associated values are added to the input mode's dictionary.
*
*   "TISInputSourceID" note: For input modes this is a string that begins with
*   the parent input method's InputSourceID or BundleID, followed by something
*   that identifies the mode. For example, "com.apple.Kotoeri.Japanese.Katakana".
*   In general it is not necessarily the same as the InputModeID, since a
*   particular InputModeID such as "com.apple.inputmethod.Japanese.Katakana"
*   may be used by multiple input methods. If this key is not specified, an
*   InputSourceID will be constructed by combining the BundleID with an
*   InputModeID suffix formed by deleting any prefix that matches the BundleID
*   or that ends in ".inputmethod."
*/
/*
*===============================================================================
*/

#ifdef __cplusplus
}
#endif

#endif /* __TEXTINPUTSOURCES__ */

