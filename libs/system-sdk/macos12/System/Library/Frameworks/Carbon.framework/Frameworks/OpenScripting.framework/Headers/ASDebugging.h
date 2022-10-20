/*
     File:       OpenScripting/ASDebugging.h
 
     Contains:   AppleScript Debugging Interfaces.
 
     Version:    OpenScripting
 
     Copyright:  © 1992-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ASDEBUGGING__
#define __ASDEBUGGING__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __OSA__
#include <OpenScripting/OSA.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
    Mode Flags
**************************************************************************/
/*    This mode flag can be passed to OSASetProperty or OSASetHandler
    and will prevent properties or handlers from being defined in a context
    that doesn't already have bindings for them. An error is returned if
    a current binding doesn't already exist. 
*/
enum {
  kOSAModeDontDefine            = 0x0001
};

/**************************************************************************
    Component Selectors
**************************************************************************/
enum {
  kASSelectSetPropertyObsolete  = 0x1101,
  kASSelectGetPropertyObsolete  = 0x1102,
  kASSelectSetHandlerObsolete   = 0x1103,
  kASSelectGetHandlerObsolete   = 0x1104,
  kASSelectGetAppTerminologyObsolete = 0x1105,
  kASSelectSetProperty          = 0x1106,
  kASSelectGetProperty          = 0x1107,
  kASSelectSetHandler           = 0x1108,
  kASSelectGetHandler           = 0x1109,
  kASSelectGetAppTerminology    = 0x110A,
  kASSelectGetSysTerminology    = 0x110B,
  kASSelectGetPropertyNames     = 0x110C,
  kASSelectGetHandlerNames      = 0x110D
};

/**************************************************************************
    Context Accessors
**************************************************************************/
/*
 *  OSASetProperty()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetProperty(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  OSAID               contextID,
  const AEDesc *      variableName,
  OSAID               scriptValueID)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSAGetProperty()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetProperty(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  OSAID               contextID,
  const AEDesc *      variableName,
  OSAID *             resultingScriptValueID)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSAGetPropertyNames()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetPropertyNames(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  OSAID               contextID,
  AEDescList *        resultingPropertyNames)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSASetHandler()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetHandler(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  OSAID               contextID,
  const AEDesc *      handlerName,
  OSAID               compiledScriptID)                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSAGetHandler()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetHandler(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  OSAID               contextID,
  const AEDesc *      handlerName,
  OSAID *             resultingCompiledScriptID)              __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSAGetHandlerNames()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetHandlerNames(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  OSAID               contextID,
  AEDescList *        resultingHandlerNames)                  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


#if !__LP64__
/*
 *  OSAGetAppTerminology()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use OSACopyScriptingDefinition instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetAppTerminology(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  FSSpec *            fileSpec,
  short               terminologyID,
  Boolean *           didLaunch,
  AEDesc *            terminologyList)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  OSAGetSysTerminology()
 *  
 *  Discussion:
 *    A terminology ID is derived from script code and language code as
 *    follows: terminologyID = ((scriptCode & 0x7F) << 8) | (langCode &
 *    0xFF)
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetSysTerminology(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  short               terminologyID,
  AEDesc *            terminologyList)                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSACopyScriptingDefinition()
 *  
 *  Discussion:
 *    Gets the scripting definition of the specified bundle.  See
 *    sdef(5) for details of the sdef format.
 *  
 *  Parameters:
 *    
 *    ref:
 *      The file (or bundle) to look in.
 *    
 *    modeFlags:
 *      There are no flags defined at this time; pass 0.
 *    
 *    sdef:
 *      The resulting sdef as XML data.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSACopyScriptingDefinition(
  const FSRef *  ref,
  SInt32         modeFlags,
  CFDataRef *    sdef)                                        __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_NA);


/*
 *  OSACopyScriptingDefinitionFromURL()
 *  
 *  Discussion:
 *    Gets the scripting definition of the specified URL.  See sdef(5)
 *    for details of the sdef format.  If used with a file: URL, this
 *    call is equivalent to OSACopyScriptingDefinition.
 *  
 *  Parameters:
 *    
 *    url:
 *      The URL to look in; this should be a file: or eppc: URL.
 *    
 *    modeFlags:
 *      There are no flags defined at this time; pass 0.
 *    
 *    sdef:
 *      The resulting sdef as XML data.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSACopyScriptingDefinitionFromURL(
  CFURLRef     url,
  SInt32       modeFlags,
  CFDataRef *  sdef)                                          __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_NA);



/**************************************************************************
    Obsolete versions provided for backward compatibility:
*/
#if !__LP64__
/*
 *  ASSetProperty()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASSetProperty(
  ComponentInstance   scriptingComponent,
  OSAID               contextID,
  const AEDesc *      variableName,
  OSAID               scriptValueID)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ASGetProperty()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASGetProperty(
  ComponentInstance   scriptingComponent,
  OSAID               contextID,
  const AEDesc *      variableName,
  OSAID *             resultingScriptValueID)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ASSetHandler()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASSetHandler(
  ComponentInstance   scriptingComponent,
  OSAID               contextID,
  const AEDesc *      handlerName,
  OSAID               compiledScriptID)                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ASGetHandler()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASGetHandler(
  ComponentInstance   scriptingComponent,
  OSAID               contextID,
  const AEDesc *      handlerName,
  OSAID *             resultingCompiledScriptID)              __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ASGetAppTerminology()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASGetAppTerminology(
  ComponentInstance   scriptingComponent,
  FSSpec *            fileSpec,
  short               terminologID,
  Boolean *           didLaunch,
  AEDesc *            terminologyList)                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/* Errors:
        errOSASystemError       operation failed
    */
/**************************************************************************/


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __ASDEBUGGING__ */

