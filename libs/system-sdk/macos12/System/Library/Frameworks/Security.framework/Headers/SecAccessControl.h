/*
 * Copyright (c) 2014 Apple Inc. All Rights Reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*!
 @header SecAccessControl
 SecAccessControl defines access rights for items.
 */

#ifndef _SECURITY_SECACCESSCONTROL_H_
#define _SECURITY_SECACCESSCONTROL_H_

#include <Security/SecBase.h>
#include <CoreFoundation/CFError.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

CF_ASSUME_NONNULL_BEGIN
CF_IMPLICIT_BRIDGING_ENABLED

/*!
 @function SecAccessControlGetTypeID
 @abstract Returns the type identifier of SecAccessControl instances.
 @result The CFTypeID of SecAccessControl instances.
 */
CFTypeID SecAccessControlGetTypeID(void)
__OSX_AVAILABLE_STARTING(__MAC_10_10, __IPHONE_8_0);

/*!
 @typedef SecAccessControlCreateFlags
 
 @constant kSecAccessControlUserPresence
 User presence policy using biometry or Passcode. Biometry does not have to be available or enrolled. Item is still
 accessible by Touch ID even if fingers are added or removed. Item is still accessible by Face ID if user is re-enrolled.
 
 @constant kSecAccessControlBiometryAny
 Constraint: Touch ID (any finger) or Face ID. Touch ID or Face ID must be available. With Touch ID
 at least one finger must be enrolled. With Face ID user has to be enrolled. Item is still accessible by Touch ID even
 if fingers are added or removed. Item is still accessible by Face ID if user is re-enrolled.

 @constant kSecAccessControlTouchIDAny
 Deprecated, please use kSecAccessControlBiometryAny instead.
 
 @constant kSecAccessControlBiometryCurrentSet
 Constraint: Touch ID from the set of currently enrolled fingers. Touch ID must be available and at least one finger must
 be enrolled. When fingers are added or removed, the item is invalidated. When Face ID is re-enrolled this item is invalidated.

 @constant kSecAccessControlTouchIDCurrentSet
 Deprecated, please use kSecAccessControlBiometryCurrentSet instead.
 
 @constant kSecAccessControlDevicePasscode
 Constraint: Device passcode

 @constant kSecAccessControlWatch
 Constraint: Watch

 @constant kSecAccessControlOr
 Constraint logic operation: when using more than one constraint, at least one of them must be satisfied.
 
 @constant kSecAccessControlAnd
 Constraint logic operation: when using more than one constraint, all must be satisfied.
 
 @constant kSecAccessControlPrivateKeyUsage
 Create access control for private key operations (i.e. sign operation)
 
 @constant kSecAccessControlApplicationPassword
 Security: Application provided password for data encryption key generation. This is not a constraint but additional item
 encryption mechanism.
*/
typedef CF_OPTIONS(CFOptionFlags, SecAccessControlCreateFlags) {
    kSecAccessControlUserPresence           = 1u << 0,
    kSecAccessControlBiometryAny            API_AVAILABLE(macos(10.13.4), ios(11.3)) = 1u << 1,
    kSecAccessControlTouchIDAny             API_DEPRECATED_WITH_REPLACEMENT("kSecAccessControlBiometryAny", macos(10.12.1, 10.13.4), ios(9.0, 11.3)) = 1u << 1,
    kSecAccessControlBiometryCurrentSet     API_AVAILABLE(macos(10.13.4), ios(11.3)) = 1u << 3,
    kSecAccessControlTouchIDCurrentSet      API_DEPRECATED_WITH_REPLACEMENT("kSecAccessControlBiometryCurrentSet", macos(10.12.1, 10.13.4), ios(9.0, 11.3)) = 1u << 3,
    kSecAccessControlDevicePasscode         API_AVAILABLE(macos(10.11), ios(9.0)) = 1u << 4,
    kSecAccessControlWatch                  API_AVAILABLE(macos(10.15), ios(NA), macCatalyst(13.0)) = 1u << 5,
    kSecAccessControlOr                     API_AVAILABLE(macos(10.12.1), ios(9.0)) = 1u << 14,
    kSecAccessControlAnd                    API_AVAILABLE(macos(10.12.1), ios(9.0)) = 1u << 15,
    kSecAccessControlPrivateKeyUsage        API_AVAILABLE(macos(10.12.1), ios(9.0)) = 1u << 30,
    kSecAccessControlApplicationPassword    API_AVAILABLE(macos(10.12.1), ios(9.0)) = 1u << 31,
} __OSX_AVAILABLE_STARTING(__MAC_10_10, __IPHONE_8_0);

/*!
 @function SecAccessControlCreateWithFlags
 @abstract Creates new access control object based on protection type and additional flags.
 @discussion Created access control object should be used as a value for kSecAttrAccessControl attribute in SecItemAdd,
 SecItemUpdate or SecKeyGeneratePair functions.  Accessing keychain items or performing operations on keys which are
 protected by access control objects can block the execution because of UI which can appear to satisfy the access control
 conditions, therefore it is recommended to either move those potentially blocking operations out of the main
 application thread or use combination of kSecUseAuthenticationContext and kSecUseAuthenticationUI attributes to control
 where the UI interaction can appear.
 @param allocator Allocator to be used by this instance.
 @param protection Protection class to be used for the item. One of kSecAttrAccessible constants.
 @param flags If no flags are set then all operations are allowed.
 @param error Additional error information filled in case of failure.
 @result Newly created access control object.
 */
__nullable
SecAccessControlRef SecAccessControlCreateWithFlags(CFAllocatorRef __nullable allocator, CFTypeRef protection,
                                                    SecAccessControlCreateFlags flags, CFErrorRef *error)
API_AVAILABLE(macos(10.10), ios(8.0));

CF_IMPLICIT_BRIDGING_DISABLED
CF_ASSUME_NONNULL_END

__END_DECLS

#endif // _SECURITY_SECACCESSCONTROL_H_
