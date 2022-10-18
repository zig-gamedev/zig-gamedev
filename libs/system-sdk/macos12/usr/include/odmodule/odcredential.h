/*
 * Copyright (c) 2009-2011 Apple Inc. All rights reserved.
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

#if !defined(__ODCREDENTIAL_H)
#define __ODCREDENTIAL_H

__BEGIN_DECLS


/*!
 * @function odcredential_create_ext
 *
 * @abstract
 * creates an od_credential_t object
 *
 * @param request
 * is an od_request_t object requesting this creation
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object
 *
 * @param rectype
 * is a CFStringRef
 *
 * @param recname
 * is a CFSTringRef
 *
 * @param metarecname
 * is a CFStringRef
 *
 * @param password
 * is a CFStringRef
 *
 * @param authorities
 * is a CFArrayRef
 *
 * @param flags
 * is a long
 *
 * @result
 * an od_credential_t
 */
OS_NOTHROW OS_WARN_RESULT
od_credential_t
odcredential_create_ext(od_request_t request, od_moduleconfig_t moduleconfig,
					CFStringRef rectype, CFStringRef recname, CFStringRef metarecname, CFStringRef password, CFArrayRef authorities,
					long flags);

/*!
 * @function odcredential_copy_details
 *
 * @abstract
 * copies details from credential
 *
 * @param credential
 * is an od_credential_t object to query
 *
 * @result
 * a CFDictionaryRef
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFDictionaryRef
odcredential_copy_details(od_credential_t credential);

/*!
 * @function odcredential_copy_recordname
 *
 * @abstract
 * copies recordname from credential
 *
 * @param credential
 * is an od_credential_t object to query
 *
 * @result
 * a CFSTringRef
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odcredential_copy_recordname(od_credential_t credential);

/*!
 * @function odcredential_copy_recordtype
 *
 * @abstract
 * copies recordtype from credential
 *
 * @param credential
 * is an od_credential_t object to query
 *
 * @result
 * a CFSTringRef
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odcredential_copy_recordtype(od_credential_t credential);

/*!
 * @function odcredential_copy_password
 *
 * @abstract
 * copies details from credential
 *
 * @param credential
 * is an od_credential_t object to query
 *
 * @result
 * a CFSTringRef
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odcredential_copy_password(od_credential_t credential);

/*!
 * @function odcredential_copy_metarecordname
 *
 * @abstract
 * copies details from credential
 *
 * @param credential
 * is an od_credential_t object to query
 *
 * @result
 * a CFStringRef
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odcredential_copy_metarecordname(od_credential_t credential);

/*!
 * @function odcredential_set_password
 *
 * @abstract
 * copies details from credential
 *
 * @discussion
 * changes the internal password that is used in all operations
 *
 * @param credential
 * is an od_credential_t object to modify
 *
 * @param password
 * is a CFStringRef
 */
void
odcredential_set_password(od_credential_t credential, CFStringRef password);

/*!
 * @function odcredential_get_uuid
 *
 * @abstract
 * gets uuid from credential
 *
 * @param credential
 * is an od_credential_t object to modify
 *
 * @param uuid
 * is a uuid_t to populate
 */
void
odcredential_get_uuid(od_credential_t credential, uuid_t uuid);

__END_DECLS

#endif
