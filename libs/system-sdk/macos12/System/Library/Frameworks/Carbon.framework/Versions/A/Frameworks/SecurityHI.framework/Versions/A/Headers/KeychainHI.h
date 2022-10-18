/*
     File:       SecurityHI/KeychainHI.h
 
     Contains:   Keychain API's with Human Interfaces
 
     Copyright:  © 2000-2008 by Apple Inc., all rights reserved
 
     Warning:    *** APPLE INTERNAL USE ONLY ***
                 This file may contain unreleased API's
 
     BuildInfo:  Built by:            khenrickson
                 On:                  Fri Aug 10 12:01:37 2012
                 With Interfacer:     3.0d46   (Mac OS X for PowerPC)
                 From:                KeychainHI.i
                     Revision:        1.2
                     Dated:           2003/05/22 21:07:47
                     Last change by:  mortay
                     Last comment:    merged #3195145
 
     Bugs:       Report bugs to Radar component "System Interfaces", "Latest"
                 List the version information (from above) in the Problem Description.
 
*/
#ifndef __KEYCHAINHI__
#define __KEYCHAINHI__

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

/* High-level interface for storing passwords */
/*
 *  KCAddAppleSharePassword()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddInternetPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
KCAddAppleSharePassword(
  AFPServerSignature *  serverSignature,
  StringPtr             serverAddress,
  StringPtr             serverName,
  StringPtr             volumeName,
  StringPtr             accountName,
  UInt32                passwordLength,
  const void *          passwordData,
  KCItemRef *           item)                  /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  KCAddInternetPassword()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddInternetPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
KCAddInternetPassword(
  StringPtr     serverName,
  StringPtr     securityDomain,
  StringPtr     accountName,
  UInt16        port,
  OSType        protocol,
  OSType        authType,
  UInt32        passwordLength,
  const void *  passwordData,
  KCItemRef *   item)                 /* can be NULL */       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  KCAddInternetPasswordWithPath()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddInternetPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 2.0 and later
 */
extern OSStatus 
KCAddInternetPasswordWithPath(
  StringPtr     serverName,
  StringPtr     securityDomain,
  StringPtr     accountName,
  StringPtr     path,
  UInt16        port,
  OSType        protocol,
  OSType        authType,
  UInt32        passwordLength,
  const void *  passwordData,
  KCItemRef *   item)                 /* can be NULL */       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  KCAddGenericPassword()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddGenericPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
KCAddGenericPassword(
  StringPtr     serviceName,
  StringPtr     accountName,
  UInt32        passwordLength,
  const void *  passwordData,
  KCItemRef *   item)                 /* can be NULL */       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/* Low-level interface for storing keychain items */
/*
 *  KCAddItem()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainItemCreateFromContent
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
KCAddItem(KCItemRef item)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/* Managing keychains */
/*
 *  KCUnlock()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainUnlock
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
KCUnlock(
  KCRef       keychain,       /* can be NULL */
  StringPtr   password)       /* can be NULL */               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  KCCreateKeychain()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainCreate
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
KCCreateKeychain(
  StringPtr   password,       /* can be NULL */
  KCRef *     keychain)       /* can be NULL */               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  KCChangeSettings()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainSetSettings
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 2.0 and later
 */
extern OSStatus 
KCChangeSettings(KCRef keychain)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  kcunlock()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainUnlock
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
kcunlock(
  KCRef         keychain,       /* can be NULL */
  const char *  password)       /* can be NULL */             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  kccreatekeychain()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainCreate
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
kccreatekeychain(
  const char *  password,       /* can be NULL */
  KCRef *       keychain)       /* can be NULL */             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  kcaddapplesharepassword()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddInternetPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
kcaddapplesharepassword(
  AFPServerSignature *  serverSignature,
  const char *          serverAddress,
  const char *          serverName,
  const char *          volumeName,
  const char *          accountName,
  UInt32                passwordLength,
  const void *          passwordData,
  KCItemRef *           item)                  /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  kcaddinternetpassword()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddInternetPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
kcaddinternetpassword(
  const char *  serverName,
  const char *  securityDomain,
  const char *  accountName,
  UInt16        port,
  OSType        protocol,
  OSType        authType,
  UInt32        passwordLength,
  const void *  passwordData,
  KCItemRef *   item)                 /* can be NULL */       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  kcaddinternetpasswordwithpath()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddInternetPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 2.0 and later
 */
extern OSStatus 
kcaddinternetpasswordwithpath(
  const char *  serverName,
  const char *  securityDomain,
  const char *  accountName,
  const char *  path,
  UInt16        port,
  OSType        protocol,
  OSType        authType,
  UInt32        passwordLength,
  const void *  passwordData,
  KCItemRef *   item)                 /* can be NULL */       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/*
 *  kcaddgenericpassword()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SecKeychainAddGenericPassword
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.6
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   in KeychainLib 1.0 and later
 */
extern OSStatus 
kcaddgenericpassword(
  const char *  serviceName,
  const char *  accountName,
  UInt32        passwordLength,
  const void *  passwordData,
  KCItemRef *   item)                 /* can be NULL */       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6;


/* Working with certificates */
/*
 *  KCFindX509Certificates()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in KeychainLib 2.0 and later
 */


/*
 *  KCChooseCertificate()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in KeychainLib 2.0 and later
 */




#ifdef __cplusplus
}
#endif

#endif /* __KEYCHAINHI__ */

