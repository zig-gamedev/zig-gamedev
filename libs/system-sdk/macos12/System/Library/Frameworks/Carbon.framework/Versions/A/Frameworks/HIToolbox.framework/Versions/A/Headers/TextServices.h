/*
     File:       HIToolbox/TextServices.h
 
     Contains:   Text Services Manager Interfaces.
 
     Copyright:  © 1991-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTSERVICES__
#define __TEXTSERVICES__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif

#ifndef __MENUS__
#include <HIToolbox/Menus.h>
#endif

#ifndef __AEINTERACTION__
#include <HIToolbox/AEInteraction.h>
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
  kTextService                  = 'tsvc' /* component type for the component description */
};



/*
 *  Summary:
 *    TSM Version Gestalt values
 *  
 *  Discussion:
 *    Gestalt values for gestaltTSMgrVersion selector (Gestalt.h)
 */
enum {
  kTSMVersion                   = 0x0150, /* Version 1.5 of the Text Services Manager */
  kTSM15Version                 = kTSMVersion,
  kTSM20Version                 = 0x0200, /* Version 2.0 as of Mac OS X 10.0 */
  kTSM22Version                 = 0x0220, /* Version 2.2 as of Mac OS X 10.3 */
  kTSM23Version                 = 0x0230, /* Version 2.3 as of Mac OS X 10.4 */
  kTSM24Version                 = 0x0240 /* Version 2.4 as of Mac OS X 10.5 */
};


/*  Interface types for NewTSMDocument*/
typedef OSType                          InterfaceTypeList[1];
typedef OSType                          TSMDocumentInterfaceType;

/*
 *  Summary:
 *    InterfaceTypeList - array of OSTypes passed to NewTSMDocument to
 *    specify capabilities of the new TSMDocument.
 *  
 *  Discussion:
 *    TSM's Interface types, as of 10.3, are also stored as TSMDocument
 *    properties, so once a TSMDocument is created, you can easily find
 *    out what were its InterfaceTypes at document creation.
 */
enum {

  /*
   * This is the traditional TSMDocument type.  It does not support
   * Unicode. TSM will convert all Unicode produced by input methods to
   * the Mac encoding represented by the current keyboard script (or
   * the Mac encoding specified by the input method producing text.) 
   * Full Unicode input sources may not be selectable when this
   * TSMDocument is active.
   */
  kTextServiceDocumentInterfaceType = kTextService, /* TSM Document type for traditional (non-Unicode) NewTSMDocument */

  /*
   * TSMTE document type.  This requests automatic management of inline
   * input sessions by TextEdit (the text engine.)  See Technote TE27 -
   * Inline Input for TextEdit with TSMTE.  This property (like
   * TextEdit) is not supported on 64-bit.
   */
  kTSMTEDocumentInterfaceType   = 'tmTE', /* TSM Document type for TSMTE document (see kTSMTEInterfaceType - TSMTE.h) */

  /*
   * Unicode-savvy TSMDocument.  TSM will pass thru all Unicode text
   * unchanged. When this TSMDocument is active, the full range of
   * input sources is available to the user, such as Unicode keyboard
   * layouts.
   */
  kUnicodeDocumentInterfaceType = 'udoc', /* TSM Document type for Unicode-savvy application */
                                        /*    Older names, to be deprecated*/
  kUnicodeDocument              = kUnicodeDocumentInterfaceType /* TSM Document type for Unicode-savvy application */
};

/*
    TextServiceClass constants supported by TSM
    Same as component subtype for the component description
*/
typedef OSType                          TextServiceClass;

/*
 *  Summary:
 *    TextService classes
 *  
 *  Discussion:
 *    Text Service classes fall into three categories or behaviors.
 *    
 *    (1) KeyboardInputMethodClass text services are exclusive of one
 *    another in terms of being the default for a given Mac script
 *    code, and at most one of these - regardless of script - can be
 *    currently selected. 
 *    (2) Only zero or one InkInputMethodClass text services can be
 *    selected; this is independent of what other input sources are
 *    selected. 
 *    (3) Zero or more input methods of other classes can be selected,
 *    regardless of what other input sources are selected (they are
 *    additive in nature, regardless of the current keyboard script).
 */
enum {

  /*
   * Text service class for keyboard input methods. Behavior is
   * exclusive. Input Methods in this class are normally associated
   * with a Mac ScriptCode or Unicode, although they can be associated
   * with several scripts by adopting the Input Mode protocol. Keyboard
   * input methods are always visible in the System UI.
   */
  kKeyboardInputMethodClass     = 'inpm',

  /*
   * Text service class for Ink (Handwriting) input methods. At most
   * one of these can be enabled and active regardless of script. Text
   * Services in the Ink class do not belong to any given script in the
   * sense that those of the Keyboard class do. Once selected, this
   * kind of text service will remain active regardless of the current
   * keyboard script. Although text services in this class are keyboard
   * script agnostic, like input methods of the keyboard class they can
   * still profess to produce only those Unicodes that are encoded in
   * the mac encoding specified in their component description record
   * or their implementation of the GetScriptLanguageSupport component
   * call. 
   * 
   * Mac OS X only provides System UI for enabling Apple's Ink input
   * method, not for other Ink input methods.
   */
  kInkInputMethodClass          = 'ink ',

  /*
   * Text service class for Character Palette input methods.  Behavior
   * is Additive. Text Services in the character palette class do not
   * belong to any given script in the same sense that do those of the
   * Keyboard class.  Once selected, this kind of text service will
   * remain active regardless of the current keyboard script. Although
   * text services in this class are keyboard script agnostic, like
   * input methods of the keyboard class they can still profess to
   * produce only those Unicodes that are encoded in the mac encoding
   * specified in their component description record or their
   * implementation of the GetScriptLanguageSupport component call.
   * 
   * 
   * Unlike input methods in the keyboard class, multiple such text
   * services can be activated in parallel, and unlike input methods in
   * the Ink class, Mac OS X provides System UI to allow the user to
   * both enable and select multiple such input methods. Use the
   * kComponentBundleInvisibleInSystemUIKey plist key to make Character
   * Palette input methods invisible to the System UI. 
   * 
   * (Dictionary Service input methods are visible in the system UI by
   * default.  Use the kComponentBundleInvisibleInSystemUIKey plist key
   * to make them invisible if a developer-provided UI is to be used
   * instead.)
   */
  kCharacterPaletteInputMethodClass = 'cplt',

  /*
   * Text Service class for Speech input methods.  Behavior is
   * Additive. 
   * 
   * Similar to Character palette class.  System UI for these has not
   * yet been determined.
   */
  kSpeechInputMethodClass       = 'voic',

  /*
   * Text Service class for Optical Character Recognition input
   * methods.  Behavior is Additive. 
   * 
   * Similar to Character palette class.  System UI for these has not
   * yet been determined.
   */
  kOCRInputMethodClass          = 'ocr '
};

/* New opaque definitions for types */
typedef struct OpaqueTSMDocumentID*     TSMDocumentID;
/* TSMDocumentID Properties*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* TSMDocumentID Properties                                                             */
/*                                                                                      */
/* With the following property APIs, you can attach any piece of data you'd like to a   */
/* TSMDocument.*/
/* to attach the data to the menu itself, and not to any specific menu item.            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
typedef OSType                          TSMDocumentPropertyTag;

/*
 *  Summary:
 *    TSMDocument Property Tags
 *  
 *  Discussion:
 *    With the following property APIs, you can attach any piece of
 *    data you'd like to a TSMDocument.  The property tags pre-defined
 *    below by TSM fall in several categories. Read-only properties: 
 *    these property tags are actually the interface type used to
 *    create a TSMDocument, such as kUnicodeDocumentInterfaceType.
 *    These properties are all property value-independent... see below.
 *    Value-independent properties:  these property tags are value
 *    independent. Only the fact that the tag is or is not a property
 *    of a TSMDocument is important.  These are used where the
 *    existence of the property is sufficient... Read-only TSMDocment
 *    properties are examples of value independent properties.
 *    SupportGlyphInfo is another. Pass a non-NULL actualSize parameter
 *    and a NULL propertyBuffer to check if the property exists (i.e.
 *    to obtain the size.) Value-dependent properties:  these are
 *    properties for which the value is meaningful.  Examples of this
 *    are the Refcon ProtocolVersion properties. NOTE: 
 *    Value-independent properties can read by other clients, in
 *    particular input methods.  For example, input methods may want to
 *    query the current TSMDocument to see if supports unrestricted
 *    Unicode input, or if it supports the GlyphInfo protocol.
 */
enum {
                                        /*    */
                                        /*    Property values for value-independent property tags (i.e. only the existence of*/
                                        /*    the property is meaningful).  To remove these properties, use TSMRemoveDocumentProperty.*/
                                        /*    To test these properties, it is sufficient to obtain the property's size, i.e. buffer = NULL*/
                                        /*    and test for error.*/
                                        /*    Read-only (value-independent) properties*/
                                        /*    Identical to Interface types passed to NewTSMDocument*/

  /*
   * Property is read-only, value-independent.  The value is identical
   * to the TextServiceDocument interface type, but is not needed. 
   * Simply test for existence of the property. Property available in
   * TSM 2.2 and later
   */
  kTSMDocumentTextServicePropertyTag = kTextServiceDocumentInterfaceType,

  /*
   * Property is read-only, value-independent.  The value is identical
   * to the UnicodeDocument interface type, but is not needed.  Simply
   * test for existence of the property. Property available in TSM 2.2
   * and later
   */
  kTSMDocumentUnicodePropertyTag = kUnicodeDocumentInterfaceType,

  /*
   * Property is read-only, value-independent.  The value is identical
   * to the TSMTEDocument interface type, but is not needed.  Simply
   * test for existence of the property. Property available in TSM 2.2
   * and later
   */
  kTSMDocumentTSMTEPropertyTag  = kTSMTEDocumentInterfaceType, /*    */
                                        /*    Settable value-independent properties*/

  /*
   * Property is value-independent.  The existence of this property in
   * a TSMDocument indicates that the event handlers associated with
   * the TSMDocument are aware of TSM's GlyhInfo data structure.  This
   * structure allows the input source producing text to apply Glyph
   * IDs, CIDs, or Fonts to subranges of text produced. This is useful
   * for characters in Unicode private use area, such as Windings. For
   * more information, see the Glyph Access protocol described in
   * TechNote TN20TT. By convention, this value can be a UInt32 with a
   * value of 0, but this is really arbitrary.  Simply test for
   * existence of the property. Property available in TSM 1.5 from
   * MacOSX 10.2 and later
   */
  kTSMDocumentSupportGlyphInfoPropertyTag = 'dpgi', /*  property value is arbitrary*/

  /*
   * Property is value-independent.  The presence of this property tag
   * indicates that the TSMDocument should use TSM's floating input
   * window to handle input from input methods.  This form of input
   * does not support Unicode input by default, unless the
   * UnicodeInputWindow property is set. By convention, this value can
   * be a UInt32 with a value of 0, but this is really arbitrary. 
   * Simply test for existence of the property. Property available in
   * TSM 2.2 and later
   */
  kTSMDocumentUseFloatingWindowPropertyTag = 'uswm', /*    use bottom-line input (floating TSM) window for text input*/

  /*
   * Property is value-independent.  The presence of this property tag
   * indicates that although the TSMDocument has been told to use TSM's
   * floating input window to handle input from input methods, the
   * floating window is to support Unicode input.  This is useful when
   * non input-related activity is to produce Unicode, such as keyboard
   * navigation . By convention, this value can be a UInt32 with a
   * value of 0, but this is really arbitrary.  Simply test for
   * existence of the property. Property available in TSM 2.2 and later
   */
  kTSMDocumentUnicodeInputWindowPropertyTag = 'dpub', /*    Unicode support in bottom line input window*/

  /*
   * Property is value-independent.  The presence of this property tag
   * indicates that the event handlers associated with this TSMDocument
   * support TSM's DocumentAccess event suite (see CarbonEvents.h) This
   * property also indicates that the handler for TSM's
   * kEventTextInputUpdateActiveInputArea event supports the
   * 'replaceRange' parameter and that the handler is a Carbon event
   * handler, not an AppleEvent handler. By convention, this value can
   * be a UInt32 with a value of 0, but this is really arbitrary. 
   * Simply test for existence of the property. Property available in
   * TSM 2.2 and later
   */
  kTSMDocumentSupportDocumentAccessPropertyTag = 'dapy', /*    support TSM Document Access protocol*/
                                        /*    Older names, to be deprecated*/
  kTSMDocumentPropertySupportGlyphInfo = kTSMDocumentSupportGlyphInfoPropertyTag,
  kTSMDocumentPropertyUnicodeInputWindow = kTSMDocumentUnicodeInputWindowPropertyTag, /*    */
                                        /*    Settable value-dependent properties*/
                                        /*    Property values for the following property tags are meaningful*/

  /*
   * Property is value-dependent.  The property value initially
   * contains the Refcon value passed to NewTSMDocument. This property
   * is useful for changing the refcon on-the-fly after the TSMDocument
   * has been created. Property available in TSM 2.2 and later
   */
  kTSMDocumentRefconPropertyTag = 'refc', /*    refcon passed to NewTSMDocument (SRefCon)*/

  /*
   * Property is value-dependent.  The property value indicates which
   * input mode should be used by the current keyboard-class input
   * method.  It is useful for temporarily restricting text input to a
   * subset of characters normally produced by an input method in a
   * given script, such as Katakana for Japanese input.  See details in
   * TextServiceProperty API below. Also note that this property tag
   * and value are passed unchanged to the TextServiceProperty API, so
   * it also serves as a TextServicePropertyTag. See
   * kTextServiceInputModePropertyTag for discussion on the values
   * associated with this property. Usage Note:  Property value is a
   * CFStringRef. With TSMGetDocumentProperty, the behavior is that of
   * a Copy function, and TSMSetDocumentProperty retain the CFString...
   * in either case the caller is responsible for releasing its
   * reference. Property available in TSM 2.2 and later
   */
  kTSMDocumentInputModePropertyTag = 'imim', /*    Input mode property for input methods (CFStringRef - see Input Modes below)*/

  /*
   * Property is value-dependent.  The property value indicates the
   * level of the window that will host inline input sessions for a
   * given TSMDocument. Input Methods will query this property to
   * determine the level above which its "candidate" (floating)
   * window(s) should be positioned, typically by adding its window to
   * a window group (see Carbon's WindowGroup API) and incrementing the
   * level for the group.  If no level is available, the input method
   * is expected to use the default behavior, i.e. let the Window
   * Manager manage the level for floating windows. Window levels are
   * typed as CGWindowLevel. NSWindow levels are accessed through
   * NSWindow's (int)level method. Property available in TSM 2.3 and
   * later
   */
  kTSMDocumentWindowLevelPropertyTag = 'twlp', /*    document window level (CGWindowLevel)*/

  /*
   * Property is value-dependent.  The property value is a
   * TISInputSourceRef specifying a keyboard input source that should
   * override whatever the System considers to be the current input
   * source at the time the TSMDocument is activated (i.e. when the
   * text field gains focus).  If the TSMDocument is already active
   * when this property is set, the specified input source is selected.
   * Usage Note:  Property value is a TISInputSourceRef. With
   * TSMGetDocumentProperty, the behavior is that of a Copy function,
   * and TSMSetDocumentProperty retain the CFString... in either case
   * the caller is responsible for releasing its reference. Property
   * available in TSM 2.4 (Mac OS X 10.5) and later
   */
  kTSMDocumentInputSourceOverridePropertyTag = 'inis', /*  Set an override (Initial) Input source*/

  /*
   * Property is value-dependent.  The property value is a CFArrayRef
   * containing an array of keyboard input sources (TISInputSourceRef)
   * available to the user. This is used to restrict keyboard input to
   * the specified set of input sources. The array should be a subset
   * of those keyboard input sources available to the user according to
   * the System, i.e. the enabled input sources in the International
   * Preferences pane or the Text Input menu. Since this input source
   * may need to be recomputed when input sources are enabled or
   * disabled either by the user or programmatically, this value should
   * be recomputed when the
   * kTISNotifyEnabledKeyboardInputSourcesChanged notification is
   * received or before a call to ActivateTSMDocument. If the set of
   * enabled input sources specified consists of only one input source,
   * it will be automatically selected. Usage Note:  Property value is
   * a CFArrayRef. With TSMGetDocumentProperty, the behavior is that of
   * a Copy function, and TSMSetDocumentProperty retain the CFString...
   * in either case the caller is responsible for releasing its
   * reference. Property available in TSM 2.4 (Mac OS X 10.5) and later
   */
  kTSMDocumentEnabledInputSourcesPropertyTag = 'enis' /*  Restrict input to this set of enabled Input sources*/
};


/*
 *  TSMSetDocumentProperty()
 *  
 *  Summary:
 *    Set a TSMDocument property
 *  
 *  Discussion:
 *    With the following property APIs, you can attach any piece of
 *    data you'd like to a TSMDocument.  Other uses include setting a
 *    (value-independent) property for input methods to query in order
 *    to determine an application's compliance with various TSM
 *    protocols. NOTE:  Property values are currently typed as SInt32,
 *    but should really be a void* since a variety of data types are
 *    used in practice. Note that the semantics for refcounted objects
 *    is ambiguous, so retain/release model is discussed for each
 *    TSM-defined property individually.  (See notes for
 *    TSMDocumentPropertyTag) Error Codes (MacErrors.h): 
 *    tsmComponentPropertyUnsupportedErr tsmInputModeChangeFailedErr
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TSMSetDocumentProperty(
  TSMDocumentID            docID,
  TSMDocumentPropertyTag   propertyTag,
  UInt32                   propertySize,
  void *                   propertyData)                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  TSMGetDocumentProperty()
 *  
 *  Summary:
 *    Get a TSMDocument property
 *  
 *  Discussion:
 *    With the following property APIs, you can attach any piece of
 *    data you'd like to a TSMDocument.  Other uses include setting a
 *    (value-independent) property for input methods to query in order
 *    to determine an application's compliance with various TSM
 *    protocols. NOTE:  Property values are currently typed as SInt32,
 *    but should really be a void* since a variety of data types are
 *    used in practice. Note that the semantics for refcounted objects
 *    is ambiguous, so retain/release model is discussed for each
 *    TSM-defined property individually.  (See notes for
 *    TSMDocumentPropertyTag) Error Codes (MacErrors.h)
 *    tsmDocPropertyNotFoundErr tsmDocPropertyBufferTooSmallErr
 *    tsmComponentPropertyNotFoundErr tsmComponentPropertyUnsupportedErr
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TSMGetDocumentProperty(
  TSMDocumentID            docID,
  TSMDocumentPropertyTag   propertyTag,
  UInt32                   bufferSize,
  UInt32 *                 actualSize,
  void *                   propertyBuffer)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  TSMRemoveDocumentProperty()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TSMRemoveDocumentProperty(
  TSMDocumentID            docID,
  TSMDocumentPropertyTag   propertyTag)                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;



/* Language and Script constants*/
enum {
  kUnknownLanguage              = 0xFFFF,
  kUnknownScript                = 0xFFFF,
  kNeutralScript                = 0xFFFF
};


enum {
                                        /* Component Flags in ComponentDescription */
  bTakeActiveEvent              = 15,   /* bit set if the component takes active event */
  bHandleAERecording            = 16,   /* bit set if the component takes care of recording Apple Events <new in vers2.0> */
  bScriptMask                   = 0x00007F00, /* bit 8 - 14 */
  bLanguageMask                 = 0x000000FF, /* bit 0 - 7  */
  bScriptLanguageMask           = bScriptMask + bLanguageMask /* bit 0 - 14  */
};


enum {
                                        /* Low level routines which are dispatched directly to the Component Manager */
  kCMGetScriptLangSupport       = 0x0001, /* Component Manager call selector 1 */
  kCMInitiateTextService        = 0x0002, /* Component Manager call selector 2 */
  kCMTerminateTextService       = 0x0003, /* Component Manager call selector 3 */
  kCMActivateTextService        = 0x0004, /* Component Manager call selector 4 */
  kCMDeactivateTextService      = 0x0005, /* Component Manager call selector 5 */
  kCMTextServiceEvent           = 0x0006, /* Component Manager call selector 6 */
  kCMTextServiceEventRef        = kCMTextServiceEvent,
  kCMGetTextServiceMenu         = 0x0007, /* Component Manager call selector 7 */
  kCMTextServiceMenuSelect      = 0x0008, /* Component Manager call selector 8 */
  kCMFixTextService             = 0x0009, /* Component Manager call selector 9 */
  kCMSetTextServiceCursor       = 0x000A, /* Component Manager call selector 10 */
  kCMHidePaletteWindows         = 0x000B, /* Component Manager call selector 11 */
  kCMGetTextServiceProperty     = 0x000C, /* Component Manager call selector 12 */
  kCMSetTextServiceProperty     = 0x000D, /* Component Manager call selector 13 */
  kCMUCTextServiceEvent         = 0x000E, /* Component Manager call selector 14 */
  kCMCopyTextServiceInputModeList = 0x000F, /* Component Manager call selector 15 */
  kCMInputModePaletteItemHit    = 0x0010, /* Component Manager call selector 16 */
  kCMGetInputModePaletteMenu    = 0x0011 /* Component Manager call selector 17 */
};



/* extract Script/Language code from Component flag ... */
#define     mGetScriptCode(cdRec)       ((ScriptCode)   ((cdRec.componentFlags & bScriptMask) >> 8))
#define     mGetLanguageCode(cdRec)     ((LangCode)     cdRec.componentFlags & bLanguageMask)

/* Text Service Info List */
struct TextServiceInfo {
  Component           fComponent;
  Str255              fItemName;
};
typedef struct TextServiceInfo          TextServiceInfo;
typedef TextServiceInfo *               TextServiceInfoPtr;
struct TextServiceList {
  short               fTextServiceCount;      /* number of entries in the 'fServices' array */
  TextServiceInfo     fServices[1];           /* Note: array of 'TextServiceInfo' records follows */
};
typedef struct TextServiceList          TextServiceList;
typedef TextServiceList *               TextServiceListPtr;
typedef TextServiceListPtr *            TextServiceListHandle;
struct ScriptLanguageRecord {
  ScriptCode          fScript;
  LangCode            fLanguage;
};
typedef struct ScriptLanguageRecord     ScriptLanguageRecord;
struct ScriptLanguageSupport {
  SInt16              fScriptLanguageCount;   /* number of entries in the 'fScriptLanguageArray' array */
  ScriptLanguageRecord  fScriptLanguageArray[1]; /* Note: array of 'ScriptLanguageRecord' records follows */
};
typedef struct ScriptLanguageSupport    ScriptLanguageSupport;
typedef ScriptLanguageSupport *         ScriptLanguageSupportPtr;
typedef ScriptLanguageSupportPtr *      ScriptLanguageSupportHandle;
struct TSMGlyphInfo {
  CFRange             range;
  ATSFontRef          fontRef;
  UInt16              collection;             /*    kGlyphCollectionXXX enum*/
  UInt16              glyphID;                /*    GID (when collection==0) or CID*/
};
typedef struct TSMGlyphInfo             TSMGlyphInfo;
struct TSMGlyphInfoArray {
  ItemCount           numGlyphInfo;
  TSMGlyphInfo        glyphInfo[1];
};
typedef struct TSMGlyphInfoArray        TSMGlyphInfoArray;

/* High level TSM Doucment routines */
#if !__LP64__
/*
 *  NewTSMDocument()
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
NewTSMDocument(
  SInt16              numOfInterface,
  InterfaceTypeList   supportedInterfaceTypes,
  TSMDocumentID *     idocID,
  SRefCon             refcon)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  DeleteTSMDocument()
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
DeleteTSMDocument(TSMDocumentID idocID)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ActivateTSMDocument()
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
ActivateTSMDocument(TSMDocumentID idocID)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DeactivateTSMDocument()
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
DeactivateTSMDocument(TSMDocumentID idocID)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FixTSMDocument()
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
FixTSMDocument(TSMDocumentID idocID)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetServiceList()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCreateInputSourceList API instead:
 *     kTISPropertyInputSourceType =
 *    kTISTypeKeyboardInputMethodWithoutModes and/or
 *    kTISTypeKeyboardInputMethodModeEnabled
 *    kTISPropertyBundleID = <input method bundle identifier of
 *    interest>
 *  
 *  Summary:
 *    Used to get a list of installed input method Components
 *  
 *  Discussion:
 *    This API was typically used to track down the Component ID for a
 *    specific input method from the Component Manager's list of
 *    installed Components, usually by componentType, componentSubType,
 *    and/or componentManufacturer.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetServiceList(
  SInt16                   numOfInterface,
  const OSType *           supportedInterfaceTypes,
  TextServiceListHandle *  serviceInfo,
  SInt32 *                 seedValue)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  OpenTextService()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
OpenTextService(
  TSMDocumentID        idocID,
  Component            aComponent,
  ComponentInstance *  aComponentInstance)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  CloseTextService()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
CloseTextService(
  TSMDocumentID       idocID,
  ComponentInstance   aComponentInstance)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SendAEFromTSMComponent()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SendAEFromTSMComponent(
  const AppleEvent *  theAppleEvent,
  AppleEvent *        reply,
  AESendMode          sendMode,
  AESendPriority      sendPriority,
  SInt32              timeOutInTicks,
  AEIdleUPP           idleProc,
  AEFilterUPP         filterProc)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SendTextInputEvent()
 *  
 *  Discussion:
 *    This API replaces SendAEFromTSMComponent on Mac OS X only. Input
 *    Methods on Mac OS X are Carbon Event based instead of AppleEvent
 *    based.  The Carbon TextInput events which they generate are
 *    provided to TSM for dispatching via this API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib N.e.v.e.r and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SendTextInputEvent(EventRef inEvent)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetDefaultInputMethod()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISSelectInputSource API
 *  
 *  Summary:
 *    Set the input method Component ID to be used for the specified
 *    script/language.
 *  
 *  Discussion:
 *    This API also switches to the specified script/language.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetDefaultInputMethod(
  Component               ts,
  ScriptLanguageRecord *  slRecordPtr)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetDefaultInputMethod()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCopyCurrentKeyboardInputSource API and query its
 *    properties.  If the current input source does not support the
 *    specified script/language, use TISCopyInputSourceForLanguage to
 *    find one that does.
 *  
 *  Summary:
 *    Get the default (last used) input method Component ID for the
 *    specified script/language.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetDefaultInputMethod(
  Component *             ts,
  ScriptLanguageRecord *  slRecordPtr)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetTextServiceLanguage()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISSelectInputSource API
 *  
 *  Summary:
 *    Switch to the last used input method (if any) for the specified
 *    script/language.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetTextServiceLanguage(ScriptLanguageRecord * slRecordPtr)    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetTextServiceLanguage()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCopyCurrentKeyboardInputSource API and query its
 *    properties, such as kTISPropertyInputSourceLanguages.
 *  
 *  Summary:
 *    Get the current script/language.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetTextServiceLanguage(ScriptLanguageRecord * slRecordPtr)    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  UseInputWindow()
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
UseInputWindow(
  TSMDocumentID   idocID,
  Boolean         useWindow)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  TSMSetInlineInputRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Implement kEventTextInputIsMouseEventInInlineInputArea TSM Carbon
 *    event.
 *  
 *  Summary:
 *    Tell TSM what region and which window make up the inline input
 *    session.
 *  
 *  Discussion:
 *    Tell TSM about the region occupied by an inline input session. If
 *    the location of certain mouse events (clicks, mouse moved) occur
 *    within the specified inline input region, TSM will forward these
 *    events to the current Input Method so that it can interact with
 *    the user. Note: If you do not specify this information, TSM will
 *    need to intercept mouse events in the entire content region as
 *    the default, when an input method is active, in order to ensure
 *    that input methods can manage user interaction properly.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inTSMDocument:
 *      The document.
 *    
 *    inWindow:
 *      The window that contains the inline input session. You can pass
 *      NULL for this parameter to indicate the user focus window.
 *    
 *    inRegion:
 *      The region occupied by the current inline input region. This
 *      should be in the coordinates of the port associated with the
 *      window you passed to inPort. It will need to be recomputed when
 *      the text content of the inline input session content changes
 *      (i.e. due to Update Active Input Area events) and when the
 *      region moves for other reasons, such as window resized,
 *      scrolling, etc. If you pass a NULL region for this parameter,
 *      TSM will default to intercept mouse events in the focus
 *      window's content region.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TSMSetInlineInputRegion(
  TSMDocumentID   inTSMDocument,
  WindowRef       inWindow,
  RgnHandle       inRegion)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/* Component Manager Interfaces to Input Methods */
/*
 *  GetScriptLanguageSupport()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
GetScriptLanguageSupport(
  ComponentInstance              ts,
  ScriptLanguageSupportHandle *  scriptHdl)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  InitiateTextService()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
InitiateTextService(ComponentInstance ts)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  TerminateTextService()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
TerminateTextService(ComponentInstance ts)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ActivateTextService()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
ActivateTextService(ComponentInstance ts)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DeactivateTextService()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
DeactivateTextService(ComponentInstance ts)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetTextServiceMenu()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
GetTextServiceMenu(
  ComponentInstance   ts,
  MenuRef *           serviceMenu)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* New Text Service call in Carbon. */
/* Note: Only Raw Key and Mouse-flavored events are passed to Text Services on MacOS X. */
/*
 *  TextServiceEventRef()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern ComponentResult 
TextServiceEventRef(
  ComponentInstance   ts,
  EventRef            event)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FixTextService()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
FixTextService(ComponentInstance ts)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HidePaletteWindows()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ComponentResult 
HidePaletteWindows(ComponentInstance ts)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
    Text Service Properties and standard values
    (used with GetTextServiceProperty/SetTextServiceProperty)
*/
#endif  /* !__LP64__ */


typedef OSType                          TextServicePropertyTag;

/*
 *  Summary:
 *    TextService Properties
 *  
 *  Discussion:
 *    Text Service properties are passed directly to a text service
 *    component via Component Mgr glue provided by TSM.  Applications
 *    that have special knowledge of specific input methods can
 *    communicate private properties between each other. The 2
 *    following errors have been introduced (as of Mac OS X 10.3) to
 *    better support these API: tsmComponentPropertyUnsupportedErr  // 
 *    SetTextServiceProperty failed tsmComponentPropertyNotFoundErr    
 *    //  GetTextServiceProperty failed
 */
enum {
                                        /* Typing input mode property tag */

  /*
   * The JaTypingMethod property is becoming obsolete.  It was
   * introduced as a way for the Setup Assistant to tell Apple's
   * Kotoeri (Japanese) input method to switch to either Roman or
   * Katakana input mode in certain text fields. The entire input mode
   * mechanism is being superceded by kTextServiceInputModePropertyTag.
   * Property available in TSM 1.5 on MacOSX 10.0 and later
   */
  kTextServiceJaTypingMethodPropertyTag = 'jtyp', /* Japanese input method typing property*/

  /*
   * The InputMode property is a CFStringRef that uniquely identifies
   * which input mode should be made current by a keyboard class input
   * method, if possible. Input modes are either generic, i.e.
   * pre-defined by TSM, or specific to an input method.  An example of
   * a generic input mode is Katakana input (Japanese) where input in a
   * text field needs to be restricted to that character subset.
   * Another is Roman input mode.  This is useful to temporarily
   * provide Roman input from an input method that normally allows text
   * input in another script. The advantage to this over forcing the
   * keyboard script to Roman is that the same input method's UI
   * continues to be available to the user, even though the input
   * script has changed. An example of a special input mode (input
   * method specific) is Hanin input mode in Traditional Chinese input
   * methods. To temporarily change the current input mode from
   * whatever it is to a generic one, use GetTextServiceProperty to
   * obtain the current input mode, then SetTextServiceProperty to
   * switch to the generic mode, and when done restore the original
   * input mode. Input Methods themselves make known what input modes
   * they support via the CopyTextServiceInputModeList component call. 
   * If the input method does not support a specified inputMode,
   * GetTextServiceProperty and SetTextServiceProperty will return
   * tsmComponentPropertyUnsupportedErr.  GetTextServiceProperty will
   * return tsmComponentPropertyNotFoundErr.  RemoveTextServiceProperty
   * is not supported with this tag. NOTE: This property tag is
   * identical to the kTSMDocumentInputModePropertyTag passed to the
   * TSMDocumentProperty API.  This allows the tag and value to be
   * passed through without interpretation. NOTE: Input modes
   * CFStringRef's are cast to SInt32 in these API. Property available
   * in TSM 2.2 and later Error Codes (MacErrors.h)
   * tsmComponentPropertyUnsupportedErr - SetTextServiceProperty does
   * not support the specified inputMode. tsmInputModeChangeFailedErr -
   * can be returned if an input method failed to succeed in switching
   * to the requested input mode.
   */
  kTextServiceInputModePropertyTag = kTSMDocumentInputModePropertyTag, /* input mode property for input methods*/
                                        /*    NOTE:  This property is being deprecated.  Use kTextServiceInputModePropertyTag instead. */
  kIMJaTypingMethodProperty     = kTextServiceJaTypingMethodPropertyTag, /* Typing method property for Japanese input methods*/
                                        /* Typing method property values for kTextServiceJaTypingMethodPropertyTag */
  kIMJaTypingMethodRoman        = 'roma', /* Roman typing*/
  kIMJaTypingMethodKana         = 'kana' /* Kana typing*/
};


#if __LP64__
typedef void *                          TextServicePropertyValue;
#else
typedef SInt32                          TextServicePropertyValue;
#endif  /* __LP64__ */


/*
    Generic, restricted, input modes
    Used as values for kTextServiceInputModePropertyTag TextServiceProperty API.
    These values require a cast from CFStringRef to SInt32.
*/

/*  Restrict output to Roman characters only*/
#define kTextServiceInputModeRoman      CFSTR("com.apple.inputmethod.Roman")
#define kTextServiceInputModePassword   CFSTR("com.apple.inputmethod.Password")
/*  Restrict output to Hiragana characters only (no conversion to Kanji, i.e. yomi)*/
#define kTextServiceInputModeJapaneseHiragana  CFSTR("com.apple.inputmethod.Japanese.Hiragana")
/*  Restrict output to Katakana characters only (no conversion to Kanji)*/
#define kTextServiceInputModeJapaneseKatakana  CFSTR("com.apple.inputmethod.Japanese.Katakana")
#define kTextServiceInputModeJapaneseFullWidthRoman  CFSTR("com.apple.inputmethod.Japanese.FullWidthRoman")
#define kTextServiceInputModeJapaneseHalfWidthKana  CFSTR("com.apple.inputmethod.Japanese.HalfWidthKana")
#define kTextServiceInputModeJapanesePlaceName  CFSTR("com.apple.inputmethod.Japanese.PlaceName")
#define kTextServiceInputModeJapaneseFirstName  CFSTR("com.apple.inputmethod.Japanese.FirstName")
#define kTextServiceInputModeJapaneseLastName  CFSTR("com.apple.inputmethod.Japanese.LastName")
/*  Restrict output to Bopomofo characters only (no conversion to Han)*/
#define kTextServiceInputModeBopomofo   CFSTR("com.apple.inputmethod.TradChinese.Bopomofo")
#define kTextServiceInputModeTradChinesePlaceName  CFSTR("com.apple.inputmethod.TradChinese.PlaceName")
/*  Restrict output to Hangul syllables only (no conversion to Hanja)*/
#define kTextServiceInputModeHangul     CFSTR("com.apple.inputmethod.Korean.Hangul")
/*
    Generic, unrestricted, Input Mode strings
    Used as values for kTextServiceInputModePropertyTag in TextServiceProperty API.
    Unrestricted Japanese output
*/
#define kTextServiceInputModeJapanese   CFSTR("com.apple.inputmethod.Japanese")
/*  Traditional Chinese generic (unrestricted) input mode*/
#define kTextServiceInputModeTradChinese  CFSTR("com.apple.inputmethod.TradChinese")
/*  Simplified Chinese generic (unrestricted) input mode*/
#define kTextServiceInputModeSimpChinese  CFSTR("com.apple.inputmethod.SimpChinese")
/*  Korean generic (unrestricted) output (i.e. Hanja possible)*/
#define kTextServiceInputModeKorean     CFSTR("com.apple.inputmethod.Korean")


#if !__LP64__
/*
 *  GetTextServiceProperty()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern ComponentResult 
GetTextServiceProperty(
  ComponentInstance           ts,
  TextServicePropertyTag      inPropertyTag,
  TextServicePropertyValue *  outPropertyValue)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetTextServiceProperty()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern ComponentResult 
SetTextServiceProperty(
  ComponentInstance          ts,
  TextServicePropertyTag     inPropertyTag,
  TextServicePropertyValue   inPropertyValue)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyTextServiceInputModeList()
 *  
 *  Summary:
 *    Obtain a copy of the set of input modes supported by a
 *    keyboard-class input method.
 *  
 *  Discussion:
 *    This Component call is only intended to be supported by input
 *    methods that adopt the Input Mode protocol. If this component
 *    call is not supported by an input method, calls to the
 *    TextServiceProperty API with the kTextServiceInputModePropertyTag
 *    property will fail with tsmComponentPropertyUnsupportedErr. Below
 *    is the layout of the CFDictionary returned, using the constants
 *    defined for the keys (i.e. not using the actual strings for the
 *    keys). <dict> <key>kTSInputModeListKey</key> <dict> <key>
 *    modeSignature : (internal ascii name) </key> <!-- This can be any
 *    of the generic input modes defined in this file, --> <!--    such
 *    as kTextServiceInputModeRoman, or can be a private input --> <!--
 *       mode such as CFSTR(
 *    "com.apple.MyInputmethod.Japanese.CoolInputMode" ) --> <dict>
 *    <key>kTSInputModeMenuIconFileKey</key> <string> (path for menu
 *    icon image file) </string>
 *    <key>kTSInputModeAlternateMenuIconFileKey</key> <string> (path
 *    for alternate menu icon image file -- when item is hilited)
 *    </string> <key>kTSInputModePaletteIconFileKey</key> <string>
 *    (path for palette icon image file) </string>
 *    <key>kTSInputModeDefaultStateKey</key> <boolean> (default on/off
 *    state) </boolean> <key>kTSInputModeScriptKey</key> <string>
 *    (scriptCode string for this mode, i.e. "smRoman", "smJapanese",
 *    ...) </string> <key>kTSInputModePrimaryInScriptKey</key>
 *    <boolean> (true if this is primary mode in this script)
 *    </boolean> <key>kTSInputModeIsVisibleKey</key> <boolean> (true if
 *    this input mode should appear in System UI) </boolean>
 *    <key>kTSInputModeKeyEquivalentModifiersKey</key> <integer>
 *    (modifiers) </integer> <key>kTSInputModeKeyEquivalentKey</key>
 *    <string> (key equivalent character) </string>
 *    <key>kTSInputModeJISKeyboardShortcutKey</key> <integer>
 *    (optional: 0=none,1=hiragana,2=katakana,3=eisu) </integer>
 *    </dict> </dict> <key>kTSVisibleInputModeOrderedArrayKey</key>
 *    <!-- This array defines the ordering (for UI purposes) of input
 *    modes that are --> <!--    both visible and enabled (either by
 *    default, i.e. by the System, or by --> <!--    the user, i.e. via
 *    System provided UI) --> <array> <value> modeSignature </value>
 *    </array> </dict> Available in TSM 2.2 and later Important:  This
 *    dictionary must also be present in the component bundle's
 *    Info.plist, in addition to being available through this component
 *    call.  This will allow retrival of input modes by the System
 *    without opening the component.  The component call will be used
 *    whenever the System is notified of a change in the contents of
 *    the inputMode list, such as when the name or key-equivalents of
 *    individual input modes have changed. Note:       If, when the
 *    input method is first activated in a login session, the settings
 *    of the individual input modes (names or key-equivalents) differ
 *    from the default settings as found in the component bundle's
 *    Info.plist, the System will need to be notified of the change. 
 *    The input method does this by sending out a
 *    kEventTextInputInputMenuChanged Carbon event, just as it is when
 *    the change originally took place.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern ComponentResult 
CopyTextServiceInputModeList(
  ComponentInstance   ts,
  CFDictionaryRef *   outInputModes)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
    Key for InputMode dictionary in Component bundle's Info.plist
    Value is a dictionary describing the set of input modes implemented by the component.
        The supported keys in this dictionary are:
            kTSInputModeListKey
            kTSVisibleInputModeOrderedArrayKey
    See CopyTextServiceInputModeList() for more details.
*/
#endif  /* !__LP64__ */

#define kComponentBundleInputModeDictKey  CFSTR("ComponentInputModeDict")

/*
    Info.plist keys for an Input Method icon TIFF file.
    The filepath (.tif or .icns file) specified for an icon is relative to the input method bundle's Resources directory.
    NOTE:  This key is required if the input method (Component) does not contain a resource fork icon.  This icon will
            also be used for any input modes that do not specify their own icon.
*/

#define kTSInputMethodIconFileKey       CFSTR("tsInputMethodIconFileKey")
#define kTSInputMethodAlternateIconFileKey  CFSTR("tsInputMethodAlternateIconFileKey")

/*
    Key for dictionary of individual input modes
    NOTE:  This key is used both in the Component bundle's Info.plist dictionary and the dictionary
            returned by the CopyTextServiceInputModeList() component call.
*/
#define kTSInputModeListKey             CFSTR("tsInputModeListKey")
#define kTSInputModeMenuIconFileKey     CFSTR("tsInputModeMenuIconFileKey")
#define kTSInputModeAlternateMenuIconFileKey  CFSTR("tsInputModeAlternateMenuIconFileKey")
#define kTSInputModePaletteIconFileKey  CFSTR("tsInputModePaletteIconFileKey")
#define kTSInputModeDefaultStateKey     CFSTR("tsInputModeDefaultStateKey")
#define kTSInputModeScriptKey           CFSTR("tsInputModeScriptKey")
#define kTSInputModePrimaryInScriptKey  CFSTR("tsInputModePrimaryInScriptKey")
#define kTSInputModeIsVisibleKey        CFSTR("tsInputModeIsVisibleKey")
#define kTSInputModeKeyEquivalentModifiersKey  CFSTR("tsInputModeKeyEquivalentModifiersKey")
#define kTSInputModeKeyEquivalentKey    CFSTR("tsInputModeKeyEquivalentKey")
#define kTSInputModeJISKeyboardShortcutKey  CFSTR("tsInputModeJISKeyboardShortcutKey")
/*
    Key for array of visible input modes to determine UI ordering
    NOTE:  This key is intended to be used both in the Component bundle's Info.plist dictionary and in
            in the dictionary returned by the CopyTextServiceInputModeList() component call.  In the latter
            case, the array should contain only the set of visible input modes that are also enabled.
*/
#define kTSVisibleInputModeOrderedArrayKey  CFSTR("tsVisibleInputModeOrderedArrayKey")

/*
    Key for Component bundle's Info.plist, controls visibility in System UI.
    Value is a CFBoolean.
    NOTE:  Keyboard input methods are always visible in System UI.
*/
#define kComponentBundleInvisibleInSystemUIKey  CFSTR("ComponentInvisibleInSystemUI")

#if !__LP64__
/*
 *  TSMCopyInputMethodEnabledInputModes()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISCreateInputSourceList API with:
 *     kTISPropertyInputSourceType = kTISTypeKeyboardInputMode
 *     kTISPropertyBundleID = <your input method bundle identifier>
 *    <BR> kTISPropertyInputSourceIsEnabled = true
 *  
 *  Summary:
 *    Obtain the array of a component's enabled (and visible) input
 *    modes.
 *  
 *  Discussion:
 *    The intended use of this API is to allow an input method to query
 *    the System for the subset of its own input modes that are
 *    enabled, so that the component's own UI can ommit input modes
 *    disabled by the user or the System. This API is only meaningful
 *    for input methods that adopt the Input Mode protocol.  If the
 *    component passed is not inputMode-savvy, the returned array will
 *    be NULL. The enabled input modes returned in the array are always
 *    visible ones, i.e. those input modes for which
 *    kTSInputModeIsVisibleKey is true because non-visible input modes
 *    are not tracked by the System. It is the responsibility of the
 *    caller to release the returned array. <array> <value>
 *    modeSignature : (internal ascii name) </value> </array> Available
 *    in TSM 2.2 and later
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
TSMCopyInputMethodEnabledInputModes(
  Component     inComponent,
  CFArrayRef *  outInputModeArray)                            AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  TSMSelectInputMode()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISSelectInputSource API passing your input mode's
 *    TISInputSourceRef
 *  
 *  Summary:
 *    Make the specified input method input mode the current input
 *    source.
 *  
 *  Discussion:
 *    The intended use of this API is to allow an input method to
 *    select one of its own input modes as the current input source and
 *    update the TextInput menu icon in the menubar. This API is only
 *    meaningful for input methods that adopt TSM's Input Mode
 *    protocol. Available in TSM 2.2 and later
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TSMSelectInputMode(
  Component     inComponent,
  CFStringRef   inInputMode)                                  AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/* Get the active TSMDocument in the current application context.       */
/* If TSM has enabled bottom line input mode because no TSMDocument     */
/* is active, NULL will be returned.                                    */
#endif  /* !__LP64__ */

/*
 *  TSMGetActiveDocument()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern TSMDocumentID 
TSMGetActiveDocument(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#if !__LP64__
/*
 *  GetDefaultInputMethodOfClass()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    See GetDefaultInputMethod.
 *  
 *  Summary:
 *    Get the default (last used) input method Component ID for the
 *    specified script/language.
 *  
 *  Discussion:
 *    Only kKeyboardInputMethodClass is supported.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetDefaultInputMethodOfClass(
  Component *             aComp,
  ScriptLanguageRecord *  slRecPtr,
  TextServiceClass        tsClass)                            AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetDefaultInputMethodOfClass()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    See SetDefaultInputMethod.
 *  
 *  Summary:
 *    Set the input method Component ID to be used for the specified
 *    script/language.
 *  
 *  Discussion:
 *    Only kKeyboardInputMethodClass is supported.
 *    This API also switches to the specified script/language.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetDefaultInputMethodOfClass(
  Component               aComp,
  ScriptLanguageRecord *  slRecPtr,
  TextServiceClass        tsClass)                            AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SelectTextService()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISSelectInputSource API.
 *  
 *  Summary:
 *    Select a text service component
 *  
 *  Discussion:
 *    This API is currently only intended for use by input methods in
 *    text service classes which are "additive" in nature, that is
 *    where the input method can operate in parallel to other input
 *    methods in the same class and other additive text service
 *    classes.  An example of such a class is the Character Palette
 *    class.  It is not for use by traditional input methods, such as
 *    those that belong to the keyboard input method class.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SelectTextService(Component aComp)                            AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DeselectTextService()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISDeselectInputSource API.
 *  
 *  Summary:
 *    Deselect a text service component
 *  
 *  Discussion:
 *    See SelectTextService.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DeselectTextService(Component aComp)                          AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IsTextServiceSelected()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use TISGetInputSourceProperty API with
 *    kTISPropertyInputSourceIsSelected property.
 *  
 *  Summary:
 *    Checks if a text service component is selected
 *  
 *  Discussion:
 *    See SelectTextService.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsTextServiceSelected(Component aComp)                        AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


#endif  /* !__LP64__ */

#if OLDROUTINENAMES
enum {
  kInputMethodService           = kKeyboardInputMethodClass
};

enum {
  kUnicodeTextService           = 'utsv' /* Component type for Unicode Text Service */
};

#endif  /* OLDROUTINENAMES */


/*-------------------------------------------*/
/* Input Mode Palette configuration routines */
/*-------------------------------------------*/

#if !__LP64__
/*
 *  TSMInputModePaletteLoadButtons()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Not used anymore.
 *  
 *  Discussion:
 *    Notifies the Input Mode Palette of changes in an input method's
 *    controls.  Replaces the current controls with the new control
 *    array.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    paletteButtonsArray:
 *      CFArray containing descriptions of the controls.  Each control
 *      is described using a CFDictionary, using the keys described
 *      below.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
TSMInputModePaletteLoadButtons(CFArrayRef paletteButtonsArray) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  TSMInputModePaletteUpdateButtons()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Not used anymore.
 *  
 *  Discussion:
 *    Notifies the Input Mode Palette of updates in an input method's
 *    controls.  Will update controls based on the control tag ID. 
 *    Does not replace/remove existing controls.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    paletteButtonsArray:
 *      CFArray containing descriptions of the controls.  Each control
 *      is described using a CFDictionary, using the keys described
 *      below.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
TSMInputModePaletteUpdateButtons(CFArrayRef paletteButtonsArray) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;




/*----------------------------------------------------------------------------*/
/* CFDictionary keys used to build the control description passed             */
/* to TSMInputModePaletteLoadButtons() and TSMInputModePaletteUpdateButtons() */
/*----------------------------------------------------------------------------*/


/*******************************************************

Example of palette controls CFArray passed to TSMInputModePaletteLoadButtons():

    Buttons consist of a CFDictionary containing:
        - itemType
        - icon
        - alternate icon
        - ID
        - enabled
        - state
        
    Pulldown menus consist of a CFDictionary containing:
        - itemType
        - icon
        - alternate icon
        - ID

    <array>    
        <dict>
        <!-- item 1 - a push button -->
            <key>tsInputModePaletteItemTypeKey</key>
            <integer>0</integer>
            <key>tsInputModePaletteItemIconKey</key>
            <string>modeButton1.tif</string>
            <key>tsInputModePaletteItemAltIconKey</key>
            <string>modeButton1Alt.tif</string>
            <key>tsInputModePaletteItemIDKey</key>
            <integer>1</integer>
            <key>tsInputModePaletteItemEnabledKey</key>
            <true/>
            <key>tsInputModePaletteItemStateKey</key>
            <integer>0</integer>
        </dict>
        <dict>
        <!-- item 2 - a pulldown menu -->
            <key>tsInputModePaletteItemTypeKey</key>
            <integer>2</integer>
            <key>tsInputModePaletteItemIconKey</key>
            <string>modeMenu1.tif</string>
            <key>tsInputModePaletteItemAltIconKey</key>
            <string>modeMenu1Alt.tif</string>
            <key>tsInputModePaletteItemIDKey</key>
            <integer>2</integer>
        </dict>
    </array>         

*******************************************************/


#endif  /* !__LP64__ */

#define kTSInputModePaletteItemTypeKey  CFSTR("tsInputModePaletteItemTypeKey")
/* CFNumber - the type of control (0: push button, 1: toggle button, 2: pulldown menu) */

#define kTSInputModePaletteItemIconKey  CFSTR("tsInputModePaletteItemIconKey")
/* CFString - icon file name (File located in IM bundle's resource directory, so this is just the file name, not full path) */

#define kTSInputModePaletteItemAltIconKey  CFSTR("tsInputModePaletteItemAltIconKey")
/* CFString - alternate icon file name (File located in IM bundle's resource directory, so this is just the file name, not full path) */

#define kTSInputModePaletteItemStateKey  CFSTR("tsInputModePaletteItemStateKey")
/* CFNumber - state of the control (0: clear/unpressed, 1: checked/pressed, 2: mixed) */

#define kTSInputModePaletteItemEnabledKey  CFSTR("tsInputModePaletteItemEnabledKey")
/* CFBoolean - enabled state of the control */

#define kTSInputModePaletteItemIDKey    CFSTR("tsInputModePaletteItemIDKey")
/* CFNumber - UInt32 tag ID for control */


#if !__LP64__
/*
 *  InputModePaletteItemHit()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    No longer called.
 *  
 *  Discussion:
 *    Component Manager call to tell an Input Method that a function
 *    button on the Input Mode Palette was pressed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inInstance:
 *      The component instance.
 *    
 *    inItemID:
 *      The item ID of the function button pressed on the palette.
 *    
 *    inItemState:
 *      The new state of the button.
 *  
 *  Result:
 *    Return non-null on successful handling of call.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern ComponentResult 
InputModePaletteItemHit(
  ComponentInstance   inInstance,
  UInt32              inItemID,
  UInt32              inItemState)                            AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetInputModePaletteMenu()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    No longer called.
 *  
 *  Discussion:
 *    Component Manager call to ask an Input Method for the menu to
 *    display for a pull-down menu on the Input Mode Palette.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inInstance:
 *      The component instance.
 *    
 *    inItemID:
 *      The item ID of the pull-down menu button.
 *    
 *    outMenuItemsArray:
 *      The menu array to return to the Input Mode Palette.
 *  
 *  Result:
 *    Return non-null on successful handling of call.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern ComponentResult 
GetInputModePaletteMenu(
  ComponentInstance   inInstance,
  UInt32              inItemID,
  CFArrayRef *        outMenuItemsArray)                      AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*----------------------------------------------------------*/
/* CFDictionary keys for menu definition returned           */
/* for kCMGetInputModePaletteMenu/GetInputModePaletteMenu() */
/*----------------------------------------------------------*/
/*******************************************************

Example of menu CFArray returned to GetInputModePaletteMenu():

    Pull-down menus consist of an array of CFDictionary's containing:
        - title
        - ID
        - state
        - enabled
        - key equivalent (optional)
        - key equivalent modifiers (optional)

    <array>    
        <dict>
        <!-- menu item w/ modifiers -->
            <key>tsInputModePaletteItemTitleKey</key>
            <string>menu item 1</string>
            <key>tsInputModePaletteItemIDKey</key>
            <integer>10</integer>
            <key>tsInputModePaletteItemEnabledKey</key>
            <true/>
            <key>tsInputModePaletteItemStateKey</key>
            <integer>0</integer>
            <key>tsInputModePaletteItemKeyEquivalentKey</key>
            <string>j</string>
            <key>tsInputModePaletteItemKeyEquivalentModifiersKey</key>
            <integer>2048</integer>
        </dict>
        <dict>
        <!-- divider -->
            <key>tsInputModePaletteItemTypeKey</key>
            <string>-</string>
        </dict>
        <!-- menu item w/o modifiers -->
            <key>tsInputModePaletteItemTitleKey</key>
            <string>menu item 2</string>
            <key>tsInputModePaletteItemIDKey</key>
            <integer>11</integer>
            <key>tsInputModePaletteItemEnabledKey</key>
            <true/>
            <key>tsInputModePaletteItemStateKey</key>
            <integer>0</integer>
        </dict>
    </array>         

*******************************************************/
#endif  /* !__LP64__ */

#define kTSInputModePaletteItemTitleKey  CFSTR("tsInputModePaletteItemTitleKey")
/* CFString - menu item title (use '-' for a separator) */

#define kTSInputModePaletteItemKeyEquivalentKey  CFSTR("tsInputModePaletteItemKeyEquivalentKey")
/* CFString - menu item keyboard shortcut (example: 'j') */

#define kTSInputModePaletteItemKeyEquivalentModifiersKey  CFSTR("tsInputModePaletteItemKeyEquivalentModifiersKey")
/* CFNumber - menu item keyboard shortcut modifiers (from Events.h) */




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TEXTSERVICES__ */

