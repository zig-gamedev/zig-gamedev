/*
     File:       LSInfoDeprecated.h
 
     Contains:   Deprecated interfaces for LaunchServices.framework
 
     Copyright:  (c) 2001-2015 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/

*/

#ifndef __LSINFODEPRECATED__
#define __LSINFODEPRECATED__

#include <LaunchServices/LSInfo.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)



typedef CF_OPTIONS(OptionBits, LSRequestedInfo) {
  kLSRequestExtension				__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyPathExtension(), -[NSURL pathExtension], or -[NSString pathExtension] instead.") = 0x00000001, /* thread-safe in 10.0*/
  kLSRequestTypeCreator				__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Creator codes are deprecated on OS X.") = 0x00000002, /* thread-safe in 10.0*/
  kLSRequestBasicFlagsOnly			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.") = 0x00000004, /* thread-safe in 10.2*/
  kLSRequestAppTypeFlags			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.") = 0x00000008, /* thread-safe in 10.2*/
  kLSRequestAllFlags				__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.") = 0x00000010, /* thread-safe in 10.2*/
  kLSRequestIconAndKind				__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.") = 0x00000020, /* thread-safe in 10.2*/
  kLSRequestExtensionFlagsOnly		__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.") = 0x00000040, /* thread-safe in 10.0*/
  kLSRequestAllInfo					__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.") = (UInt32)0xFFFFFFFF /* thread-safe in 10.2*/
};



typedef CF_OPTIONS(OptionBits, LSItemInfoFlags) {
  kLSItemInfoIsPlainFile			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsRegularFileKey or NSURLIsRegularFileKey instead.") = 0x00000001, /* Not a directory, volume, or symlink*/
  kLSItemInfoIsPackage				__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsPackageKey or NSURLIsPackageKey instead.") = 0x00000002, /* Packaged directory*/
  kLSItemInfoIsApplication			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsApplicationKey or NSURLIsApplicationKey instead.") = 0x00000004, /* Single-file or packaged application*/
  kLSItemInfoIsContainer			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsDirectoryKey or NSURLIsDirectoryKey instead.") = 0x00000008, /* Directory (includes packages) or volume*/
  kLSItemInfoIsAliasFile			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsAliasFileKey or NSURLIsAliasFileKey instead.") = 0x00000010, /* Alias file (includes sym links)*/
  kLSItemInfoIsSymlink				__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsSymbolicLinkKey or NSURLIsSymbolicLinkKey.") = 0x00000020, /* UNIX sym link*/
  kLSItemInfoIsInvisible			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsHiddenKey or NSURLIsHiddenKey instead.") = 0x00000040, /* Invisible by any known mechanism*/
  kLSItemInfoIsNativeApp			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "The Classic environment is no longer supported.") = 0x00000080, /* Carbon or Cocoa native app*/
  kLSItemInfoIsClassicApp			__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "The Classic environment is no longer supported.") = 0x00000100, /* CFM/68K Classic app*/
  kLSItemInfoAppPrefersNative		__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "The Classic environment is no longer supported.") = 0x00000200, /* Carbon app that prefers to be launched natively*/
  kLSItemInfoAppPrefersClassic		__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "The Classic environment is no longer supported.") = 0x00000400, /* Carbon app that prefers to be launched in Classic*/
  kLSItemInfoAppIsScriptable		__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLApplicationIsScriptableKey or NSURLApplicationIsScriptableKey instead.") = 0x00000800, /* App can be scripted*/
  kLSItemInfoIsVolume				__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLIsVolumeKey or NSURLIsVolumeKey instead.") = 0x00001000, /* Item is a volume*/
  kLSItemInfoExtensionIsHidden		__OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use the URL resource property kCFURLHasHiddenExtensionKey or NSURLHasHiddenExtensionKey instead.") = 0x00100000 /* Item has a hidden extension*/
};

	
#define kLSInvalidExtensionIndex ULONG_MAX      // Index returned from LSGetExtensionInfo when name has no extension
	

typedef struct LSItemInfoRecord {
  LSItemInfoFlags flags;
  OSType filetype;
  OSType creator;
  CFStringRef extension;              /* release when finished*/
} LSItemInfoRecord __OS_AVAILABILITY_MSG(macosx, deprecated=10.11, "Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.");


#if TARGET_OS_OSX
/*
 *  LSCopyItemInfoForURL()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return information about an item.
 *  
 *  Discussion:
 *    Returns as much or as little information as requested about inURL.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inURL:
 *      The CFURLRef of the item about which information is requested.
 *    
 *    inWhichInfo:
 *      Flags indicating which information to return
 *    
 *    outItemInfo:
 *      Information is returned in this structure. Must not be NULL
 *  
 *  Deprecated:
 *    Use URL resource properties instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyItemInfoForURL(
  CFURLRef            inURL,
  LSRequestedInfo     inWhichInfo,
  LSItemInfoRecord *  outItemInfo)                            API_DEPRECATED("Use URL resource properties instead.", macos(10.0,10.11) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyItemInfoForRef()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return information about an item.
 *  
 *  Discussion:
 *    Returns as much or as little information as requested about
 *    inItemRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inItemRef:
 *      The FSRef of the item about which information is requested.
 *
 *    inWhichInfo:
 *      Flags indicating which information to return
 *    
 *    outItemInfo:
 *      Information is returned in this structure. Must not be NULL
 *
 *  Deprecated:
 *    Use URL resource properties instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyItemInfoForRef(
  const FSRef *       inItemRef,
  LSRequestedInfo     inWhichInfo,
  LSItemInfoRecord *  outItemInfo)                            API_DEPRECATED("Use URL resource properties instead.", macos(10.0,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  LSGetExtensionInfo()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Get information about the extension for a file system name.
 *  
 *  Discussion:
 *    Returns the starting index of the extension (not including the
 *    period) or kLSInvalidExtensionIndex if the input name has no
 *    extension.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inNameLen:
 *      The number of the UniChars in inNameBuffer.
 *    
 *    inNameBuffer:
 *      The buffer containing the name's Unicode characters.
 *    
 *    outExtStartIndex:
 *      On success, the starting index of the extension if there is one
 *      (not including the period). Set to kLSInvalidExtensionIndex if
 *      inNameBuffer does not contain a valid extension.
 *
 *  Deprecated:
 *    Use CFURLCopyPathExtension(), -[NSURL pathExtension], or -[NSString pathExtension] instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSGetExtensionInfo(
  UniCharCount    inNameLen,
  const UniChar   inNameBuffer[],
  UniCharCount *  outExtStartIndex)                           API_DEPRECATED("Use CFURLCopyPathExtension(), -[NSURL pathExtension], or -[NSString pathExtension] instead.", macos(10.1,10.11) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyDisplayNameForRef()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Get the display name for an FSRef.
 *  
 *  Discussion:
 *    Return a copy of the display name for an FSRef. Takes into
 *    consideration whether this item has a hidden extension or not.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inRef:
 *      The FSRef for which the display name is desired.
 *    
 *    outDisplayName:
 *      Pointer to the CFString into which the display name should be
 *      copied. Callers must dispose of the resulting CFString.
 *
 *  Deprecated:
 *    Use the URL resource property kCFURLLocalizedNameKey or NSURLLocalizedNameKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyDisplayNameForRef(
  const FSRef *  inRef,
  CFStringRef *  outDisplayName)                              API_DEPRECATED("Use the URL resource property kCFURLLocalizedNameKey or NSURLLocalizedNameKey instead.", macos(10.1,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyDisplayNameForURL()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Get the display name for a CFURLRef.
 *  
 *  Discussion:
 *    Return a copy of the display name for a CFURLRef. Takes into
 *    consideration whether this item has a hidden extension or not.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inURL:
 *      The URL for which the display name is desired.
 *    
 *    outDisplayName:
 *      Pointer to the CFString into which the display name should be
 *      copied. Callers must dispose of the resulting CFString.
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLLocalizedNameKey or NSURLLocalizedNameKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyDisplayNameForURL(
  CFURLRef       inURL,
  CFStringRef *  outDisplayName)                              API_DEPRECATED("Use the URL resource property kCFURLLocalizedNameKey or NSURLLocalizedNameKey instead.", macos(10.1,10.11) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSSetExtensionHiddenForRef()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Sets whether the extension for an FSRef is hidden or not.
 *  
 *  Discussion:
 *    Sets the necessary file system state to indicate that the
 *    extension for inRef is hidden, as in the Finder. You can
 *    determine if an FSRef's extension is hidden using
 *    LSCopyItemInfoForRef.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inRef:
 *      The FSRef for which the extension is to be hidden or shown.
 *    
 *    inHide:
 *      True to hide inRef's extension, false to show it.
 *
 *  Deprecated:
 *    Use the URL resource property kCFURLHasHiddenExtensionKey or NSURLHasHiddenExtensionKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSSetExtensionHiddenForRef(
  const FSRef *  inRef,
  Boolean        inHide)                                      API_DEPRECATED("Use the URL resource property kCFURLHasHiddenExtensionKey or NSURLHasHiddenExtensionKey instead.", macos(10.1,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSSetExtensionHiddenForURL()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Sets whether the extension for a CFURLRef is hidden or not.
 *  
 *  Discussion:
 *    Sets the necessary file system state to indicate that the
 *    extension for inURL is hidden, as in the Finder. You can
 *    determine if a CFURLRef's extension is hidden using
 *    kCFURLHasHiddenExtensionKey.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inURL:
 *      The CFURLRef for which the extension is to be hidden or shown.
 *    
 *    inHide:
 *      True to hide inURL's extension, false to show it.
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLHasHiddenExtensionKey or NSURLHasHiddenExtensionKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSSetExtensionHiddenForURL(
  CFURLRef   inURL,
  Boolean    inHide)                                          API_DEPRECATED("Use the URL resource property kCFURLHasHiddenExtensionKey or NSURLHasHiddenExtensionKey instead.", macos(10.1,10.11) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyKindStringForRef()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Get the kind string for an item.
 *  
 *  Discussion:
 *    Returns the kind string as used in the Finder and elsewhere for
 *    inFSRef. The CFStringRef must be released after use.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inFSRef:
 *      The item for which the kind string is requested.
 *    
 *    outKindString:
 *      A CFStringRef* to receive the copied kind string object. This
 *      CFStringRef must be released eventually.
 *
 *  Deprecated:
 *    Use the URL resource property kCFURLLocalizedTypeDescriptionKey or NSURLLocalizedTypeDescriptionKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyKindStringForRef(
  const FSRef *  inFSRef,
  CFStringRef *  outKindString)                               API_DEPRECATED("Use the URL resource property kCFURLLocalizedTypeDescriptionKey or NSURLLocalizedTypeDescriptionKey instead.", macos(10.0,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyKindStringForURL()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Get the kind string for an item.
 *  
 *  Discussion:
 *    Returns the kind string as used in the Finder and elsewhere for
 *    inURL. The CFStringRef must be released after use.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inURL:
 *      The item for which the kind string is requested.
 *    
 *    outKindString:
 *      A CFStringRef* to receive the copied kind string object. This
 *      CFStringRef must be released eventually.
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLLocalizedTypeDescriptionKey or NSURLLocalizedTypeDescriptionKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyKindStringForURL(
  CFURLRef       inURL,
  CFStringRef *  outKindString)                               API_DEPRECATED("Use the URL resource property kCFURLLocalizedTypeDescriptionKey or NSURLLocalizedTypeDescriptionKey instead.", macos(10.0,10.11) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyKindStringForTypeInfo()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the kind string for items like the provided info
 *  
 *  Discussion:
 *    Returns the kind string as shown in the Finder for the those
 *    items whose type, creator, and/or extension match the provided
 *    information. The kind string returned will be the one that most
 *    closely describes all the information provided. The kind string
 *    is subject to the document binding preferences that have been
 *    specified by the user. For example, if a creator is specified but
 *    the user has asked for files with the given
 *    creator/type/extension combination to open in an application with
 *    a different creator, the kind string will be loaded from the
 *    user's preferred application.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inType:
 *      The OSType file type for which you want a kind string. Specify
 *      kLSUnknownType if no file type information is available.
 *    
 *    inCreator:
 *      The OSType creator for which you want a kind string. Specify
 *      kLSUnknownCreator if no creator information is available.
 *    
 *    inExtension:
 *      The extension for which you want a kind string. Specify NULL if
 *      no extension information is available.
 *    
 *    outKindString:
 *      A CFStringRef* to receive the copied kind string object. This
 *      CFStringRef must be released eventually.
 *  
 *  Deprecated:
 *    Use UTTypeCopyDescription instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyKindStringForTypeInfo(
  OSType         inType,
  OSType         inCreator,
  CFStringRef    inExtension,         /* can be NULL */
  CFStringRef *  outKindString)                               API_DEPRECATED("Use UTTypeCopyDescription instead.", macos(10.2,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyKindStringForMIMEType()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Get the kind string for the specified MIME type.
 *  
 *  Discussion:
 *    Returns the localized kind string describing the specified MIME
 *    type.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inMIMEType:
 *      The string specifying the MIME type.
 *    
 *    outKindString:
 *      A CFStringRef* to receive the copied kind string object. This
 *      CFStringRef must be released eventually.
 *  
 *  Deprecated:
 *    Use UTTypeCopyDescription instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyKindStringForMIMEType(
  CFStringRef    inMIMEType,
  CFStringRef *  outKindString)                               API_DEPRECATED("Use UTTypeCopyDescription instead.", macos(10.2,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  LSGetApplicationForItem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the application used to open an item.
 *  
 *  Discussion:
 *    Consults the binding tables to return the application that would
 *    be used to open inItemRef if it were double-clicked in the
 *    Finder. This application will be the user-specified override if
 *    appropriate or the default otherwise. If no application is known
 *    to LaunchServices suitable for opening this item,
 *    kLSApplicationNotFoundErr will be returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inItemRef:
 *      The FSRef of the item for which the application is requested.
 *    
 *    inRoleMask:
 *      Whether to return the editor or viewer for inItemRef. If you
 *      don't care which, use kLSRolesAll.
 *    
 *    outAppRef:
 *      Filled in with the FSRef of the application if not NULL.
 *    
 *    outAppURL:
 *      Filled in with the CFURLRef of the application if not NULL.
 *      THIS FUNCTION, DESPITE ITS NAME, RETAINS THE URL REFERENCE ON
 *      BEHALF OF THE CALLER. THE CALLER MUST EVENTUALLY RELEASE THE
 *      RETURNED URL REFERENCE.
 *
 *  Deprecated:
 *    Use LSCopyDefaultApplicationURLForURL instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSGetApplicationForItem(
  const FSRef *  inItemRef,
  LSRolesMask    inRoleMask,
  FSRef *        outAppRef,        /* can be NULL */
  CFURLRef *     outAppURL)        /* can be NULL */          API_DEPRECATED("Use LSCopyDefaultApplicationURLForURL instead.", macos(10.0,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );


	
/*
 *  LSGetApplicationForInfo()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the application used to open items with particular data.
 *  
 *  Discussion:
 *    Consults the binding tables to return the application that would
 *    be used to open items with type, creator, and/or extension as
 *    provided if they were double-clicked in the Finder. This
 *    application will be the default for items like this if one has
 *    been set. If no application is known to LaunchServices suitable
 *    for opening such items, kLSApplicationNotFoundErr will be
 *    returned. Not all three input parameters can be NULL at the same
 *    time nor can both output parameters be NULL at the same time.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inType:
 *      The file type to consider. Can be kLSUnknownType.
 *    
 *    inCreator:
 *      The file creator to consider. Can be kLSUnknownCreator.
 *    
 *    inExtension:
 *      The file name extension to consider. Can be NULL.
 *    
 *    inRoleMask:
 *      Whether to return the editor or viewer for inItemRef. If you
 *      don't care which, use kLSRolesAll.
 *    
 *    outAppRef:
 *      Filled in with the FSRef of the application if not NULL.
 *    
 *    outAppURL:
 *      Filled in with the CFURLRef of the application if not NULL.
 *      THIS FUNCTION, DESPITE ITS NAME, RETAINS THE URL REFERENCE ON
 *      BEHALF OF THE CALLER. THE CALLER MUST EVENTUALLY RELEASE THE
 *      RETURNED URL REFERENCE.
 *
 *  Deprecated:
 *    Use LSCopyDefaultApplicationURLForContentType instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSGetApplicationForInfo(
  OSType        inType,
  OSType        inCreator,
  CFStringRef   inExtension,       /* can be NULL */
  LSRolesMask   inRoleMask,
  FSRef *       outAppRef,         /* can be NULL */
  CFURLRef *    outAppURL)         /* can be NULL */          API_DEPRECATED("Use -[NSWorkspace URLForApplicationToOpenContentType:] instead.", macos(10.0,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyApplicationForMIMEType()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the application used to handle data with the specified
 *    MIME type.
 *  
 *  Discussion:
 *    The returned application URL will be the user's preferred handler
 *    for the MIME type if one has been set. If no user preferred
 *    application has been set, Launch Services will select a default
 *    handler for the MIME type. If no application is known to handle
 *    the MIME type, kLSApplicationNotFoundErr will be returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inMIMEType:
 *      The string specifying the MIME type.
 *    
 *    inRoleMask:
 *      A role mask that the chosen application must satisfy. Use
 *      kLSRolesAll if the role is not important.
 *    
 *    outAppURL:
 *      Receives the copied CFURLRef, which must be released by the
 *      caller.
 *
 *  Deprecated:
 *    Use LSCopyDefaultApplicationURLForContentType instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyApplicationForMIMEType(
  CFStringRef   inMIMEType,
  LSRolesMask   inRoleMask,
  CFURLRef *    outAppURL)                                    API_DEPRECATED("Use -[NSWorkspace URLForApplicationToOpenContentType:] instead.", macos(10.2,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSGetApplicationForURL()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Return the application used to open an item.
 *  
 *  Discussion:
 *    Consults the binding tables to return the application that would
 *    be used to open inURL if it were double-clicked in the Finder.
 *    This application will be the user-specified override if
 *    appropriate or the default otherwise. If no application is known
 *    to LaunchServices suitable for opening this item,
 *    kLSApplicationNotFoundErr will be returned.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inURL:
 *      The CFURLRef of the item for which the application is requested.
 *    
 *    inRoleMask:
 *      Whether to return the editor or viewer for inItemRef. If you
 *      don't care which, use kLSRolesAll.
 *    
 *    outAppRef:
 *      Filled in with the FSRef of the application if not NULL.
 *    
 *    outAppURL:
 *      Filled in with the CFURLRef of the application if not NULL.
 *      THIS FUNCTION, DESPITE ITS NAME, RETAINS THE URL REFERENCE ON
 *      BEHALF OF THE CALLER. THE CALLER MUST EVENTUALLY RELEASE THE
 *      RETURNED URL REFERENCE.
 *
 *  Deprecated:
 *    Use LSCopyDefaultApplicationURLForURL instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSGetApplicationForURL(
  CFURLRef      inURL,
  LSRolesMask   inRoleMask,
  FSRef *       outAppRef,        /* can be NULL */
  CFURLRef *    outAppURL)        /* can be NULL */           API_DEPRECATED("Use LSCopyDefaultApplicationURLForURL instead.", macos(10.0,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSFindApplicationForInfo()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Locate a specific application.
 *  
 *  Discussion:
 *    Returns the application with the corresponding input information.
 *    The registry of applications is consulted first in order of
 *    bundleID, then creator, then name. All comparisons are case
 *    insensitive and 'ties' are decided first by version, then by
 *    native vs. Classic.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inCreator:
 *      The file creator to consider. Can be kLSUnknownCreator.
 *    
 *    inBundleID:
 *      The bundle ID to consider. Can be NULL.
 *    
 *    inName:
 *      The name to consider. Can be NULL. Must include any extensions
 *      that are part of the file system name, e.g. '.app'.
 *    
 *    outAppRef:
 *      Filled in with the FSRef of the application if not NULL.
 *    
 *    outAppURL:
 *      Filled in with the CFURLRef of the application if not NULL.
 *      THIS FUNCTION, DESPITE ITS NAME, RETAINS THE URL REFERENCE ON
 *      BEHALF OF THE CALLER. THE CALLER MUST EVENTUALLY RELEASE THE
 *      RETURNED URL REFERENCE.
 *
 *  Deprecated:
 *    Use LSCopyApplicationURLsForBundleIdentifier instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSFindApplicationForInfo(
  OSType        inCreator,
  CFStringRef   inBundleID,       /* can be NULL */
  CFStringRef   inName,           /* can be NULL */
  FSRef *       outAppRef,        /* can be NULL */
  CFURLRef *    outAppURL)        /* can be NULL */           API_DEPRECATED("Use LSCopyApplicationURLsForBundleIdentifier instead.", macos(10.0,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCanRefAcceptItem()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Determine whether an item can accept another item.
 *  
 *  Discussion:
 *    Returns in outAcceptsItem whether inTargetRef can accept
 *    inItemFSRef as in a drag and drop operation. If inRoleMask is
 *    other than kLSRolesAll then make sure inTargetRef claims to
 *    fulfill the requested role.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Parameters:
 *    
 *    inItemFSRef:
 *      FSRef of the item about which acceptance is requested.
 *    
 *    inTargetRef:
 *      FSRef of the potential target.
 *    
 *    inRoleMask:
 *      The role(s) the target must claim in order to consider
 *      acceptance.
 *    
 *    inFlags:
 *      Use kLSAcceptDefault.
 *    
 *    outAcceptsItem:
 *      Filled in with result. Must not be NULL.
 *
 *  Deprecated:
 *    Use LSCanURLAcceptURL instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCanRefAcceptItem(
  const FSRef *       inItemFSRef,
  const FSRef *       inTargetRef,
  LSRolesMask         inRoleMask,
  LSAcceptanceFlags   inFlags,
  Boolean *           outAcceptsItem)                         API_DEPRECATED("Use LSCanURLAcceptURL instead.", macos(10.0,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSRegisterFSRef()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    If the specified FSRef refers to an application or other bundle
 *    claiming to handle documents or URLs, add the bundle's document
 *    and URL claims to the Launch Services database.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    inRef:
 *      The FSRef of the item to be registered.
 *    
 *    inUpdate:
 *      When false, LSRegisterFSRef does not register the item if it
 *      has already been registered and the current modification date
 *      of the item has not changed from when it was last registered.
 *      When true, the item's registered info is updated, even if the
 *      modification has not changed.
 *  
 *  Result:
 *    An OSStatus value: noErr - Success kLSNoRegistrationInfoErr - The
 *    item does not contain info requiring registration kLSDataErr -
 *    The item's property list info is malformed.
 *
 *  Deprecated:
 *    Use LSRegisterURL instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSRegisterFSRef(
  const FSRef *  inRef,
  Boolean        inUpdate)                                    API_DEPRECATED("Use LSRegisterURL instead.", macos(10.3,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );

	
#endif // TARGET_OS_OSX


/* ================================================================================== */
/*   API for retrieving item attributes                                               */
/* ================================================================================== */
/* 
 *  Attributes Names
 *
 *  kLSItemContentType
 *
 *    The item's content type identifier (a uniform type identifier string)
 *    Value type CFStringRef
 *
 *
 *  kLSItemFileType
 *
 *    The item's file type (OSType)
 *    Value type CFStringRef
 *
 *
 *  kLSItemFileCreator
 *
 *    The item's file creator (OSType)
 *    Value type CFStringRef 
 *
 *
 *  kLSItemExtension
 *
 *    The item's filename extension
 *    Value type CFStringRef 
 *
 *
 *  kLSItemDisplayName
 *
 *    The item's name as displayed to the user
 *    (The display name reflects localization and
 *    extension hiding which may be in effect)
 *    Value type CFStringRef 
 *
 *
 *  kLSItemDisplayKind
 *
 *    The localized kind string describing this item's type
 *    Value type CFStringRef 
 *
 *
 *  kLSItemRoleHandlerDisplayName
 *
 *    The display name of the application set to handle (open) this item
 *    (subject to the role mask)
 *    value type CFStringRef 
 *
 *
 *  kLSItemIsInvisible
 *
 *    True if the item is normally hidden from users
 *    Value type CFBooleanRef 
 *
 *
 *  kLSItemExtensionIsHidden
 *
 *    True if the item's extension is set to be hidden
 *    Value type CFBooleanRef 
 *
 *
 *  kLSItemQuarantineProperties (can be set)
 *
 *    A dictionary of quarantine properties. See LSQuarantine.h for
 *    quarantine property definitions. This attribute is not
 *    present if the item is not quarantined.
 *    Value type CFDictionaryRef. May be NULL.
 */
/*
 *  kLSItemContentType   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the URL resource property NSURLContentTypeKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemContentType                          API_DEPRECATED("Use the URL resource property NSURLContentTypeKey instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemFileType   *** DEPRECATED ***
 *
 *  Deprecated:
 *    Use the URL resource property NSURLContentTypeKey to get the file's UTI instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemFileType                             API_DEPRECATED("Use the URL resource property NSURLContentTypeKey to get the file's UTI instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemFileCreator   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the URL resource property NSURLContentTypeKey to get the file's UTI instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemFileCreator                          API_DEPRECATED("Use the URL resource property NSURLContentTypeKey to get the file's UTI instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemExtension   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CFURLCopyPathExtension or -[NSURL pathExtension] instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemExtension                            API_DEPRECATED("Use CFURLCopyPathExtension or -[NSURL pathExtension] instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemDisplayName   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLLocalizedNameKey or NSURLLocalizedNameKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemDisplayName                          API_DEPRECATED("Use the URL resource property kCFURLLocalizedNameKey or NSURLLocalizedNameKey instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemDisplayKind   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLLocalizedTypeDescriptionKey or NSURLLocalizedTypeDescriptionKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemDisplayKind                          API_DEPRECATED("Use the URL resource property kCFURLLocalizedTypeDescriptionKey or NSURLLocalizedTypeDescriptionKey instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemRoleHandlerDisplayName   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Instead, resolve the desired role handler for the file, then use the URL resource
 *    property kCFURLLocalizedNameKey or NSURLLocalizedNameKey on the role handler's URL.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemRoleHandlerDisplayName               API_DEPRECATED("Instead, resolve the desired role handler for the file, then use the URL resource property kCFURLLocalizedNameKey or NSURLLocalizedNameKey on the role handler's URL.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemIsInvisible   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLIsHiddenKey or NSURLIsHiddenKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemIsInvisible                          API_DEPRECATED("Use the URL resource property kCFURLIsHiddenKey or NSURLIsHiddenKey instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemExtensionIsHidden   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLHasHiddenExtensionKey or NSURLHasHiddenExtensionKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemExtensionIsHidden                    API_DEPRECATED("Use the URL resource property kCFURLHasHiddenExtensionKey or NSURLHasHiddenExtensionKey instead.", ios(4.0,8.0), macos(10.4,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );
/*
 *  kLSItemQuarantineProperties   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use the URL resource property kCFURLQuarantinePropertiesKey or NSURLQuarantinePropertiesKey instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kLSItemQuarantineProperties                 API_DEPRECATED("Use the URL resource property kCFURLQuarantinePropertiesKey or NSURLQuarantinePropertiesKey instead.", ios(4.0,8.0), macos(10.5,10.10), tvos(9.0,9.0), watchos(4.0,4.0) );

#if TARGET_OS_OSX
/*
 *  LSCopyItemAttribute()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Assigns the value of the specified item's attribute (or NULL, if
 *    the item has no such attribute or an error occurs) to *outValue.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    inItem:
 *      The FSRef of the item
 *    
 *    inRoles:
 *      The role(s), at least one of which must be provided by the
 *      application selected when computing attributes related to
 *      document binding (such as kLSItemRoleHandlerDisplayName). Pass
 *      kLSRolesAll if any role is acceptable.
 *    
 *    inAttributeName:
 *      The name of the attribute to copy
 *    
 *    outValue:
 *      Receives the attribute value
 *  
 *  Result:
 *    an OSStatus value. Returns kLSAttributeNotFoundErr if the item
 *    does not have the requested attribute.
 *
 *  Deprecated:
 *    Use CFURLCopyResourcePropertyForKey or -[NSURL getResourceValue:forKey:error:] instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyItemAttribute(
  const FSRef *  inItem,
  LSRolesMask    inRoles,
  CFStringRef    inAttributeName,
  CFTypeRef *    outValue)                                    API_DEPRECATED("Use CFURLCopyResourcePropertyForKey or -[NSURL getResourceValue:forKey:error:] instead.", macos(10.4,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSCopyItemAttributes()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Creates a dictionary containing the specified attribute values
 *    and assigns it to *outValues. The output dictionary keys are the
 *    attribute names. The CFTypeID of each value in the dictionary
 *    varies by attribute. See each attribute name constant for a
 *    description of its value type. An attribute key will be absent
 *    from the values dictionary if the item has no such attribute.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    inItem:
 *      The FSRef of the item
 *    
 *    inRoles:
 *      The role(s), at least one of which must be provided by the
 *      application selected when computing attributes related to
 *      document binding (such as kLSItemRoleHandlerDisplayName). Pass
 *      kLSRolesAll if any role is acceptable.
 *    
 *    inAttributeNames:
 *      The array of attribute names
 *    
 *    outValues:
 *      Receives the dictionary of attribure name-value pairs
 *  
 *  Result:
 *    an OSStatus value.
 *
 *  Deprecated:
 *    Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSCopyItemAttributes(
  const FSRef *      inItem,
  LSRolesMask        inRoles,
  CFArrayRef         inAttributeNames,
  CFDictionaryRef *  outValues)                               API_DEPRECATED("Use CFURLCopyResourcePropertiesForKeys or -[NSURL resourceValuesForKeys:error:] instead.", macos(10.4,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  LSSetItemAttribute()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Sets the value of a settable item's attribute. Currently, only
 *    the kLSItemQuarantineProperties attribute may be set.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    inItem:
 *      The FSRef of the item
 *    
 *    inRoles:
 *      Reserved for future use. To ensure compatibility, pass
 *      kLSRolesAll for this argument.
 *    
 *    inAttributeName:
 *      The name of the attribute to set
 *    
 *    inValue:
 *      The new value of the attribute. If NULL, removes the attribute
 *      from the item.
 *  
 *  Result:
 *    an OSStatus value. Returns kLSAttributeNotSettableErr if the
 *    attribute is read-only.
 *
 *  Deprecated:
 *    Use CFURLSetResourcePropertyForKey or -[NSURL setResourceValue:forKey:error:] instead.
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSSetItemAttribute(
  const FSRef *  inItem,
  LSRolesMask    inRoles,
  CFStringRef    inAttributeName,
  CFTypeRef      inValue)               /* can be NULL */     API_DEPRECATED("Use CFURLSetResourcePropertyForKey or -[NSURL setResourceValue:forKey:error:] instead.", macos(10.5,10.10) ) API_UNAVAILABLE( ios, tvos, watchos );

#endif // TARGET_OS_OSX

/*
 *  LSHandlerOptions   *** DEPRECATED ***
 *
 *  Options controlling how content handlers are selected.
 *
 *    kLSHandlerOptionsDefault - by default, Launch Services will
 *        use a content item's creator (when available) to select a handler
 *    kLSHandlerOptionsIgnoreCreator - Launch Services will ignore content item 
 *        creator information when selecting a role handler for the specified 
 *        content type
 *
 *  Deprecated:
 *    Creator codes are deprecated on OS X.
 *
 */
typedef CF_OPTIONS(OptionBits, LSHandlerOptions) {
  kLSHandlerOptionsDefault       API_DEPRECATED("Creator codes are deprecated on OS X.", ios(4.0,9.0), macos(10.4,10.11), tvos(9.0,9.0), watchos(4.0,4.0) ) = 0,
  kLSHandlerOptionsIgnoreCreator API_DEPRECATED("Creator codes are deprecated on OS X.", ios(4.0,9.0), macos(10.4,10.11), tvos(9.0,9.0), watchos(4.0,4.0) ) = 1
};


/*
 *  LSGetHandlerOptionsForContentType()   *** DEPRECATED ***
 *
 *  Get the handler options for the specified content type (UTI).
 *
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *
 *  Deprecated:
 *    Creator codes are deprecated on OS X.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern LSHandlerOptions 
LSGetHandlerOptionsForContentType(CFStringRef inContentType)  API_DEPRECATED("Creator codes are deprecated on OS X.", ios(4.0,9.0), macos(10.4,10.11), tvos(9.0,9.0), watchos(4.0,4.0) );



/*
 *  LSSetHandlerOptionsForContentType()   *** DEPRECATED ***
 *
 *  Set the handler options for the specified content type (UTI).
 *
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *
 *  Deprecated:
 *    Creator codes are deprecated on OS X.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
LSSetHandlerOptionsForContentType(
  CFStringRef        inContentType,
  LSHandlerOptions   inOptions)                               API_DEPRECATED("Creator codes are deprecated on OS X.", ios(4.0,9.0), macos(10.4,10.11), tvos(9.0,9.0), watchos(4.0,4.0) );



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __LSINFODEPRECATED__ */

