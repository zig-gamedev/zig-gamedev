/*
     File:       LSOpen.h
 
     Contains:   Public interfaces for LaunchServices.framework
 
     Copyright:  (c) 2003-2012 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/

*/

#ifndef __LSOPEN__
#define __LSOPEN__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#if TARGET_OS_OSX
#ifndef __CARBONCORE__
#include <CarbonCore/CarbonCore.h>
#endif
#endif

#ifndef __LSINFO__
#include <LaunchServices/LSInfo.h>
#endif

#if TARGET_OS_OSX
#ifndef __AE__
#include <AE/AE.h>
#endif
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)



CF_ASSUME_NONNULL_BEGIN



/* ======================================================================================================== */
/* LaunchServices Type & Constants                                                                          */
/* ======================================================================================================== */

typedef CF_OPTIONS(OptionBits, LSLaunchFlags) {
  kLSLaunchDefaults             = 0x00000001, /* Defaults = open, async*/
  kLSLaunchAndPrint             = 0x00000002, /* Print items instead of open them*/
  kLSLaunchAndDisplayErrors     = 0x00000040, /* Report launch/open failures in the UI*/
  kLSLaunchDontAddToRecents     = 0x00000100, /* Do not add app or documents to recents menus.*/
  kLSLaunchDontSwitch           = 0x00000200, /* Do not bring new app to the foreground.*/
  kLSLaunchAsync                = 0x00010000, /* Asynchronous launch; return as soon as the app starts launching.*/
  kLSLaunchNewInstance          = 0x00080000, /* Instantiate app even if it is already running.*/
  kLSLaunchAndHide              = 0x00100000, /* Send child a "hide" request as soon as it checks in.*/
  kLSLaunchAndHideOthers        = 0x00200000, /* Hide all other apps when the app checks in.*/
};

#if TARGET_OS_OSX
typedef struct LSLaunchURLSpec {
  __nullable CFURLRef           appURL;                 /* app to use*/
  __nullable CFArrayRef         itemURLs;               /* items to open/print*/
  const AEDesc * __nullable     passThruParams;         /* passed untouched to application as optional parameter*/
  LSLaunchFlags                 launchFlags;
  void * __nullable             asyncRefCon;            /* used if you register for app birth/death notification*/
} LSLaunchURLSpec                                                    API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSOpenCFURLRef()
 *  
 *  Summary:
 *    Open an application, document, or folder.
 *  
 *  Discussion:
 *    Opens applications, documents, and folders. Applications are
 *    opened via an 'oapp' or 'rapp' event. Documents are opened in
 *    their user-overridden or default applications as appropriate.
 *    Folders are opened in the Finder. Use the more specific
 *    LSOpenFromURLSpec for more control over launching.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inURL:
 *      The CFURLRef of the item to launch.
 *    
 *    outLaunchedURL:
 *      The CFURLRef of the item actually launched. For inURLs that are
 *      documents, outLaunchedURL will be the application used to
 *      launch the document. Can be NULL. THIS FUNCTION, DESPITE ITS
 *      NAME, RETAINS THE URL REFERENCE ON BEHALF OF THE CALLER. THE
 *      CALLER MUST EVENTUALLY RELEASE THE RETURNED URL REFERENCE.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSOpenCFURLRef(
  CFURLRef                         inURL,
  __nullable CFURLRef *__nullable  outLaunchedURL)                   API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSOpenFromURLSpec()
 *  
 *  Summary:
 *    Opens an application or one or more documents or folders.
 *  
 *  Discussion:
 *    Opens applications, documents, and folders.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inLaunchSpec:
 *      The specification of what to launch and how to launch it.
 *    
 *    outLaunchedURL:
 *      The CFURLRef of the item actually launched. For inURLs that are
 *      documents, outLaunchedURL will be the application used to
 *      launch the document. Can be NULL. THIS FUNCTION, DESPITE ITS
 *      NAME, RETAINS THE URL REFERENCE ON BEHALF OF THE CALLER. THE
 *      CALLER MUST EVENTUALLY RELEASE THE RETURNED URL REFERENCE.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSOpenFromURLSpec(
  const LSLaunchURLSpec *          inLaunchSpec,
  __nullable CFURLRef *__nullable  outLaunchedURL)                   API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );
#endif



CF_ASSUME_NONNULL_END



#pragma pack(pop)

#ifdef __cplusplus
}
#endif



#include <LaunchServices/LSOpenDeprecated.h>



#endif /* __LSOPEN__ */

