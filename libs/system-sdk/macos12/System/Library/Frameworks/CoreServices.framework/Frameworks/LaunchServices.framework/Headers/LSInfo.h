/*
     File:       LSInfo.h
 
     Contains:   Public interfaces for LaunchServices.framework
 
     Copyright:  (c) 2001-2012 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/

*/

#ifndef __LSINFO__
#define __LSINFO__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#if TARGET_OS_OSX

#ifndef __CARBONCORE__
#include <CarbonCore/CarbonCore.h>
#endif
#endif // TARGET_OS_OSX

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

CF_ENUM(OSStatus) {
  kLSNo32BitEnvironmentErr      = -10386, /* i386 is no longer supported*/
  kLSAppInTrashErr              = -10660, /* The app cannot be run when inside a Trash folder*/
  kLSExecutableIncorrectFormat  = -10661, /* No compatible executable was found*/
  kLSAttributeNotFoundErr       = -10662, /* An item attribute value could not be found with the specified name*/
  kLSAttributeNotSettableErr    = -10663, /* The attribute is not settable*/
  kLSIncompatibleApplicationVersionErr = -10664, /* The app is incompatible with the current OS*/
  kLSNoRosettaEnvironmentErr    = -10665, /* PowerPC apps are no longer supported */
  kLSGarbageCollectionUnsupportedErr = -10666, /* Objective-C garbage collection is no longer supported*/
  kLSUnknownErr                 = -10810, /* Unexpected internal error*/
  kLSNotAnApplicationErr        = -10811, /* Item needs to be an application, but is not*/
  kLSNotInitializedErr          = -10812, /* Not used in 10.2 and later*/
  kLSDataUnavailableErr         = -10813, /* E.g. no kind string*/
  kLSApplicationNotFoundErr     = -10814, /* E.g. no application claims the file*/
  kLSUnknownTypeErr             = -10815, /* Don't know anything about the type of the item*/
  kLSDataTooOldErr              = -10816, /* Not used in 10.3 and later*/
  kLSDataErr                    = -10817, /* Not used in 10.4 and later*/
  kLSLaunchInProgressErr        = -10818, /* E.g. launching an already launching application*/
  kLSNotRegisteredErr           = -10819, /* Not used in 10.3 and later*/
  kLSAppDoesNotClaimTypeErr     = -10820, /* One or more documents are of types (and/or one or more URLs are of schemes) not supported by the target application (sandboxed callers only)*/
  kLSAppDoesNotSupportSchemeWarning = -10821, /* Not used in 10.2 and later*/
  kLSServerCommunicationErr     = -10822, /* The server process (registration and recent items) is not available*/
  kLSCannotSetInfoErr           = -10823, /* The extension visibility on this item cannot be changed*/
  kLSNoRegistrationInfoErr      = -10824, /* The item contains no registration info*/
  kLSIncompatibleSystemVersionErr = -10825, /* The app cannot run on the current OS version*/
  kLSNoLaunchPermissionErr      = -10826, /* User doesn't have permission to launch the app (managed networks)*/
  kLSNoExecutableErr            = -10827, /* The executable is missing*/
  kLSNoClassicEnvironmentErr    = -10828, /* The Classic environment was required but is not available*/
  kLSMultipleSessionsNotSupportedErr = -10829, /* The app cannot run simultaneously in two different sessions*/
};

typedef CF_OPTIONS(OptionBits, LSRolesMask) {
  kLSRolesNone                  = 0x00000001, /* no claim is made about support for this type/scheme*/
  kLSRolesViewer                = 0x00000002, /* claim to view items of this type*/
  kLSRolesEditor                = 0x00000004, /* claim to edit items of this type/scheme*/
  kLSRolesShell                 = 0x00000008, /* claim to execute items of this type*/
  kLSRolesAll                   = (UInt32)0xFFFFFFFF /* claim to do it all*/
};

CF_ENUM(OSType) {
  kLSUnknownType                = 0,
  kLSUnknownCreator             = 0
};

typedef CF_OPTIONS(OptionBits, LSAcceptanceFlags) {
  kLSAcceptDefault              = 0x00000001,
  kLSAcceptAllowLoginUI         = 0x00000002 /* show UI to log in if necessary*/
};


/* ======================================================================================================== */
/* LaunchServices API                                                                                       */
/* ======================================================================================================== */



/*!
 *  @abstract
 *    Return the application used to open an item.
 *  
 *  @discussion
 *    Consults the binding tables to return the application that would
 *    be used to open inURL if it were double-clicked in the
 *    Finder. This application will be the user-specified override if
 *    appropriate or the default otherwise.
 *
 *  @param inURL
 *    The URL of the item for which the application is requested.
 *
 *  @param inRoleMask
 *    Whether to return the editor or viewer for inURL. If you
 *    don't care which, use kLSRolesAll.
 *    
 *  @param outError
 *    On failure, set to a CFError describing the problem. If you are
 *    not interested in this information, pass NULL. The caller is
 *    responsible for releasing this object.
 *
 *  @result
 *    If an acceptable application is found, its URL is returned.
 *    If the URL is a file:// URL, the application bound to the specified
 *    file or directory's type is returned. If the URL's scheme is something
 *    else, its default scheme handler is returned. If no application could
 *    be found, NULL is returned and outError (if not NULL) is populated.
 *    with kLSApplicationNotFoundErr.
 *    The caller is responsible for releasing this URL.
 */
extern __nullable CFURLRef
LSCopyDefaultApplicationURLForURL(
  CFURLRef                           inURL,
  LSRolesMask                        inRoleMask,
  __nullable CFErrorRef *__nullable  outError)                       API_DEPRECATED("Use -[NSWorkspace URLForApplicationToOpenURL:] instead.", macos(10.10, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(ios, tvos, watchos);

	
	
/*!
 *  @abstract
 *    Return the application used to open a content type (UTI).
 *  
 *  @discussion
 *    Consults the binding tables to return the application that would
 *    be used to open a file of type inContentType if it were double-clicked
 *    in the Finder. This application will be the user-specified override if
 *    appropriate or the default otherwise.
 *  
 *    @param inContentType
 *      The Uniform Type Identifier (UTI) of the item for which the
 *      application is requested.
 *
 *    @param inRoleMask
 *      Whether to return the editor or viewer for inContentType. If you
 *      don't care which, use kLSRolesAll.
 *    
 *    @param outError
 *      On failure, set to a CFError describing the problem. If you are
 *      not interested in this information, pass NULL. The caller is
 *      responsible for releasing this object.
 *
 *  @result
 *    If an acceptable application is found, its URL is returned.
 *    If no application could be found, NULL is returned and
 *    outError (if not NULL) is populated with kLSApplicationNotFoundErr.
 *    The caller is responsible for releasing this URL.
 */
extern __nullable CFURLRef
LSCopyDefaultApplicationURLForContentType(
  CFStringRef                        inContentType,
  LSRolesMask                        inRoleMask,
  __nullable CFErrorRef *__nullable  outError)                       API_DEPRECATED("Use -[NSWorkspace URLForApplicationToOpenContentType:] instead.", macos(10.10, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(ios, tvos, watchos);
	
	
	
/*!
 *  @abstract Given a bundle identifier (such as com.apple.finder), find all URLs to the corresponding application.
 *
 *  @discussion
 *    Returns zero or more URLs to applications that have the specified
 *    bundle identifier.
 *  
 *  @param inBundleIdentifier The bundle identifier of interest, such as "com.apple.finder". Must
 *    not be NULL.
 *    
 *  @param outError
 *    On failure, set to a CFError describing the problem. If you are
 *    not interested in this information, pass NULL. The caller is
 *    responsible for releasing this object.
 *    
 *  @result
 *    If any applications with the specified bundle identifier are found,
 *    their URLs are returned in a CFArray. If no application could be found,
 *    NULL is returned and outError (if not NULL) is populated with kLSApplicationNotFoundErr.
 *    In macOS 10.15 and later, the returned array is sorted with the first element containing the
 *    best available application with the specified bundle identifier. Prior to macOS 10.15, the
 *    order of elements in the array was undefined.
 *
 */
extern __nullable CFArrayRef
LSCopyApplicationURLsForBundleIdentifier(
  CFStringRef                        inBundleIdentifier,
  __nullable CFErrorRef *__nullable  outError)                       API_DEPRECATED("Use -[NSWorkspace URLsForApplicationsWithBundleIdentifier:] instead.", macos(10.10, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(ios, tvos, watchos);
	
	
	
/*!
 *  @abstract
 *    Returns an array of URLs to applications that offer the requested
 *    role(s) for the input item.
 *  
 *    @param inURL
 *      The CFURLRef of the item for which all suitable applications
 *      are desired. If the URL is a file URL, it is treated as a
 *      document, and applications are selected based on the document's
 *      type information. Otherwise, applications are selected based on
 *      the URL's scheme.
 *    
 *    @param inRoleMask
 *      The role(s) which must intersect with the role provided by an
 *      application for the specified item in order for the application
 *      to be included in the result. Pass kLSRolesAll if any role is
 *      acceptable.
 *  
 *  @result
 *    An array of CFURLRefs, one for each application which can open
 *    inURL with at least one of the roles in inRoleMask, or NULL if no
 *    applications can open the item. When an array is returned, you
 *    must eventually release it.
 *
 *    The order of the resulting array is undefined. If you need the
 *    default application for the specified URL, use LSCopyDefaultApplicationURLForURL.
 *  
 */
extern __nullable CFArrayRef
LSCopyApplicationURLsForURL(
  CFURLRef      inURL,
  LSRolesMask   inRoleMask)                                          API_DEPRECATED("Use -[NSWorkspace URLsForApplicationsToOpenURL:] instead.", macos(10.3, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(ios, tvos, watchos);

	
	
/*!
 *  @abstract
 *    Determine whether an item can accept another item.
 *  
 *  @discussion
 *    Returns in outAcceptsItem whether inTargetURL can accept
 *    inItemURL as in a drag and drop operation. If inRoleMask is other
 *    than kLSRolesAll then make sure inTargetRef claims to fulfill the
 *    requested role.
 *  
 *    @param inItemURL
 *      CFURLRef of the item about which acceptance is requested.
 *    
 *    @param inTargetURL
 *      CFURLRef of the potential target.
 *    
 *    @param inRoleMask
 *      The role(s) the target must claim in order to consider
 *      acceptance.
 *    
 *    @param inFlags
 *      Use kLSAcceptDefault.
 *    
 *    @param outAcceptsItem
 *      Filled in with result. Must not be NULL.
 *  
 */
extern OSStatus 
LSCanURLAcceptURL(
  CFURLRef            inItemURL,
  CFURLRef            inTargetURL,
  LSRolesMask         inRoleMask,
  LSAcceptanceFlags   inFlags,
  Boolean *           outAcceptsItem)                                API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/*!
 *  @discussion
 *    If the specified URL refers to an application or other bundle
 *    claiming to handle documents or URLs, add the bundle's document
 *    and URL claims to the Launch Services database.
 *  
 *    @param inURL
 *      The CFURLRef of the item (a directory or file) to be registered.
 *    
 *    @param inUpdate
 *      When false, LSRegisterURL does not register the item if it has
 *      already been registered and the current modification date of
 *      the item has not changed from when it was last registered. When
 *      true, the item's registered info is updated, even if the
 *      modification has not changed.
 *  
 *  @result
 *    An OSStatus value: noErr - Success kLSNoRegistrationInfoErr - The
 *    item does not contain info requiring registration kLSDataErr -
 *    The item's property list info is malformed.
 *  
 */
extern OSStatus 
LSRegisterURL(
  CFURLRef   inURL,
  Boolean    inUpdate)                                               API_AVAILABLE( ios(4.0), macos(10.3), tvos(9.0), watchos(1.0) );



/* ================================================================================== */
/*   API for accessing content and URL handler preferences                            */
/* ================================================================================== */

/*!
 *  @discussion Returns the application bundle identifier of the default handler
 *  for the specified content type (UTI), in the specified role(s).
 *  For any role, specify kLSRolesAll. Returns NULL if no handler
 *  is available.
 *
 *  @param inContentType a string UTI type identifier
 *  @param inRole a LSRole
 *
 *  @result an application bundle identifier which is the default handler for the given type and role, or NULL if there is no default handler
 */
extern __nullable CFStringRef
LSCopyDefaultRoleHandlerForContentType(
  CFStringRef   inContentType,
  LSRolesMask   inRole)                                              API_DEPRECATED("Use -[NSWorkspace URLForApplicationToOpenContentType:] instead.", ios(4.0, API_TO_BE_DEPRECATED), macos(10.4, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED), watchos(1.0, API_TO_BE_DEPRECATED));

/*!
 *  @discussion Returns an array of application bundle identifiers for
 *  applications capable of handling the specified content type 
 *  (UTI) with the specified role(s). Application content handling 
 *  capabilities are determined according to the kCFBundleDocumentTypes 
 *  listed in an application's Info.plist). For any role, specify kLSRolesAll. 
 *  Returns NULL if no handlers are available.
 *
 *  @param inContentType a string UTI type identifier
 *  @param inRole a LSRole
 *
 *  @return an array of of CFStringRef bundle identifiers, or NULL
 */
extern __nullable CFArrayRef
LSCopyAllRoleHandlersForContentType(
  CFStringRef   inContentType,
  LSRolesMask   inRole)                                              API_DEPRECATED("Use -[NSWorkspace URLsForApplicationsToOpenContentType:] instead.", macos(10.4, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(ios, tvos, watchos);

/*!
 *  @discussion Sets the user's preferred handler for the specified content
 *  type (UTI) in the specified role(s). For all roles, specify
 *  kLSRolesAll. The handler is specified as an application
 *  bundle identifier.
 *
 *  @param inContentType a string UTI type identifier
 *  @param inRole the role type(s) to set
 *  @param inHandlerBundleID the bundle identifier to set as the default handler for the given contet type and role(s)
 *
 *  @result noErr on success, or an error indicating why the call failed
 *
 */
extern OSStatus 
LSSetDefaultRoleHandlerForContentType(
  CFStringRef   inContentType,
  LSRolesMask   inRole,
  CFStringRef   inHandlerBundleID)                                   API_DEPRECATED("Use -[NSWorkspace setDefaultApplicationAtURL:toOpenContentType:completionHandler:] instead.", ios(4.0, API_TO_BE_DEPRECATED), macos(10.4, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED), watchos(1.0, API_TO_BE_DEPRECATED));



/*!
 *  @discussion Returns the bundle identifier of the default handler for
 *  the specified URL scheme. Returns NULL if no handler
 *  is available.
 *
 *	@param	inURLScheme	the scheme to return the default handler bundle identifier for
 *
 *	@result	a CFStringRef bundle identifier of the default handler, or NULL if no handler is available
 *
 */
extern __nullable CFStringRef
LSCopyDefaultHandlerForURLScheme(CFStringRef inURLScheme)            API_DEPRECATED("Use -[NSWorkspace URLForApplicationToOpenURL:] instead.", macos(10.4, 10.15)) API_UNAVAILABLE(ios, watchos, tvos);



/*!
 *  @discussion Returns an array of application bundle identifiers for
 *  applications capable of handling the specified URL scheme.
 *  URL handling capability is determined according to the
 *  kCFBundleURLTypes listed in an application's Info.plist).
 *  Returns NULL if no handlers are available.
 *
 *  @param inURLScheme the scheme to return an array of bundle identifiers for applications capable of handling the scheme
 *
 *  @result
 *  		An array of bundle identifier strings
 *
 */
extern __nullable CFArrayRef
LSCopyAllHandlersForURLScheme(CFStringRef inURLScheme)               API_DEPRECATED("Use -[NSWorkspace URLsForApplicationsToOpenURL:] instead.", macos(10.4, 10.15)) API_UNAVAILABLE(ios, watchos, tvos);



/*!
 *  @abstract Sets the user's preferred handler for the specified URL
 *  scheme. The handler is specified as an application
 *  bundle identifier.
 *
 *	@param	inURLScheme	the url scheme to set a default handler for
 *	@param inHandlerBundeID	the bundle identifier to be set as the default handler for the given scheme
 */
extern OSStatus 
LSSetDefaultHandlerForURLScheme(
  CFStringRef   inURLScheme,
  CFStringRef   inHandlerBundleID)                            API_DEPRECATED("Use -[NSWorkspace setDefaultApplicationAtURL:toOpenURLsWithScheme:completionHandler:] instead.", ios(4.0, API_TO_BE_DEPRECATED), macos(10.4, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED), watchos(1.0, API_TO_BE_DEPRECATED));



CF_ASSUME_NONNULL_END



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#include <LaunchServices/LSInfoDeprecated.h>
	

#endif /* __LSINFO__ */

