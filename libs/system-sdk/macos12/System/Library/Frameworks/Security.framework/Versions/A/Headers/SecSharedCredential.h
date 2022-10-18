/*
 * Copyright (c) 2014-2020 Apple Inc. All Rights Reserved.
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
 @header SecSharedCredential
 SecSharedCredential defines CoreFoundation-based functions for
 storing and requesting shared password-based credentials.
 These credentials are currently able to be shared with Safari and
 applications which have a 'com.apple.developer.associated-domains'
 entitlement that includes the domain being requested.
 */

#ifndef _SECURITY_SECSHAREDCREDENTIAL_H_
#define _SECURITY_SECSHAREDCREDENTIAL_H_

#include <Security/SecItem.h>
#include <CoreFoundation/CoreFoundation.h>
#include <AvailabilityMacros.h>

__BEGIN_DECLS

CF_ASSUME_NONNULL_BEGIN
CF_IMPLICIT_BRIDGING_ENABLED

#ifdef __BLOCKS__

/*!
    @enum Credential Key Constants
    @discussion Predefined key constants used to get values in a dictionary
        of credentials returned by SecRequestWebCredential.
    @constant kSecSharedPassword Specifies a dictionary key whose value is a
        shared password. You use this key to get a value of type CFStringRef
        that contains a password.
*/
extern const CFStringRef kSecSharedPassword
    API_AVAILABLE(ios(8.0), macCatalyst(14.0), macos(11.0))
    API_UNAVAILABLE(tvos, watchos);

/*!
 @function SecAddSharedWebCredential
 @abstract Asynchronously store (or update) a shared password for a website.
 @param fqdn The fully qualified domain name of the website requiring the password.
 @param account The account name associated with this password.
 @param password The password to be stored. Pass NULL to remove a shared password if it exists.
 @param completionHandler A block which will be invoked when the function has completed. If the shared password was successfully added (or removed), the CFErrorRef parameter passed to the block will be NULL. If the error parameter is non-NULL, an error occurred and the error reference will hold the result. Note: the error reference will be automatically released after this handler is called, though you may optionally retain it for as long as needed.
 @discussion This function adds a shared password item which will be accessible by Safari and applications that have the specified fully-qualified domain name in their 'com.apple.developer.associated-domains' entitlement. If a shared password item already exists for the specified website and account, it will be updated with the provided password. To remove a password, pass NULL for the password parameter.

 Note: since a request involving shared web credentials may potentially require user interaction or other verification to be approved, this function is dispatched asynchronously; your code provides a completion handler that will be called once the results (if any) are available.
 */
void SecAddSharedWebCredential(CFStringRef fqdn, CFStringRef account, CFStringRef __nullable password,
    void (^completionHandler)(CFErrorRef __nullable error))
    API_AVAILABLE(ios(8.0), macCatalyst(14.0), macos(11.0))
    API_UNAVAILABLE(tvos, watchos);

/*!
 @function SecRequestSharedWebCredential
 @abstract Asynchronously obtain one or more shared passwords for a website.
 @param fqdn (Optional) Fully qualified domain name of the website for which passwords are being requested. If NULL is passed in this argument, the domain name(s) listed in the calling application's 'com.apple.developer.associated-domains' entitlement are searched implicitly.
 @param account (Optional) Account name for which passwords are being requested. The account may be NULL to request all shared credentials which are available for the site, allowing the caller to discover an existing account.
 @param completionHandler A block which will be called to deliver the requested credentials. If no matching items were found, the credentials array will be empty, and the CFErrorRef parameter will provide the error result. Note: the credentials and error references will be automatically released after this handler is called, though you may optionally retain either for as long as needed.
 @discussion This function requests one or more shared passwords for a given website, depending on whether the optional account parameter is supplied. To obtain results, the website specified in the fqdn parameter must be one which matches an entry in the calling application's 'com.apple.developer.associated-domains' entitlement.

 If matching shared password items are found, the credentials provided to the completionHandler will be a CFArrayRef containing CFDictionaryRef entries. Each dictionary entry will contain the following pairs (see Security/SecItem.h):
        key: kSecAttrServer     value: CFStringRef (the website)
        key: kSecAttrAccount    value: CFStringRef (the account)
        key: kSecSharedPassword value: CFStringRef (the password)

 If the found item specifies a non-standard port number (i.e. other than 443 for https), the following key may also be present:
        key: kSecAttrPort       value: CFNumberRef (the port number)

 Note: since a request involving shared web credentials may potentially require user interaction or other verification to be approved, this function is dispatched asynchronously; your code provides a completion handler that will be called once the results (if any) are available.
 */
void SecRequestSharedWebCredential(CFStringRef __nullable fqdn, CFStringRef __nullable account,
    void (^completionHandler)(CFArrayRef __nullable credentials, CFErrorRef __nullable error))
    API_DEPRECATED("Use ASAuthorizationController to make an ASAuthorizationPasswordRequest (AuthenticationServices framework)",
                    ios(8.0,14.0), macCatalyst(14.0,14.0), macos(10.16,11.0))
    API_UNAVAILABLE(tvos, watchos);

/*!
 @function SecCreateSharedWebCredentialPassword
 @abstract Returns a randomly generated password.
 @return CFStringRef password in the form xxx-xxx-xxx-xxx where x is taken from the sets "abcdefghkmnopqrstuvwxy", "ABCDEFGHJKLMNPQRSTUVWXYZ", "3456789" with at least one character from each set being present.
*/
__nullable
CFStringRef SecCreateSharedWebCredentialPassword(void)
    API_AVAILABLE(ios(8.0), macCatalyst(14.0), macos(11.0))
    API_UNAVAILABLE(tvos, watchos);


#endif /* __BLOCKS__ */

CF_IMPLICIT_BRIDGING_DISABLED
CF_ASSUME_NONNULL_END

__END_DECLS

#endif /* !_SECURITY_SECSHAREDCREDENTIAL_H_ */

