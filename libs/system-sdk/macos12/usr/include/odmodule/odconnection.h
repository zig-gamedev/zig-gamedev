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

#if !defined(__ODCONNECTION_H)
#define __ODCONNECTION_H

__BEGIN_DECLS

/*!
 * @function odconnection_create
 *
 * @abstract
 * creates a new connection object
 *
 * @discussion
 * creates a new object to be used for later operations
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object associated with this connection
 *
 * @param name
 * is a CFStringRef to call the connection
 *
 * @param shared
 * is a boolean specifying if the connection is allowed to be shared
 *
 * @param timeout
 * is how many seconds before the connection should be closed
 *
 * @result
 * an od_connection_t object created or NULL on failure
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_connection_t
odconnection_create(od_moduleconfig_t moduleconfig, CFStringRef name, bool shared, uint64_t timeout);

/*!
 * @function odconnection_create_ext
 *
 * @abstract
 * creates a connection object that has not been established with an alternative credential and flags
 *
 * @discussion
 * creates a connection object that has not been established with an alternative credential and flags
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object associated with this connection
 *
 * @param request
 * is an od_request_t that has triggered this connection
 *
 * @param name
 * is a CFStringRef to call the connection
 *
 * @param guid
 * is a guid_t to assign to this connection (optional, NULL is allowed)
 *
 * @param credential
 * is an od_credential_t attached to this connection
 *
 * @param flags
 * requested for this connection
 *
 * @param timeout
 * is how many seconds before the connection should be closed
 *
 * @param info
 * is od_connection_info_t (callbacks, context info, etc.)
 *
 * @result
 * An od_connection_t object created or NULL on failure
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED OS_NONNULL1 OS_NONNULL3
od_connection_t
odconnection_create_ext(od_moduleconfig_t moduleconfig, od_request_t request, CFStringRef name, guid_t *guid,
                        od_credential_t credential, eODModuleConfigServerFlags flags, uint64_t timeout, od_connection_info_t *info);


/*!
 * @function odconnection_connect_if_necessary
 *
 * @abstract
 * Forces a connection if not already connected.
 *
 * @discussion
 *  * Forces a connection if not already connected.  Will block for the connection setup timeout.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 *
 * @param request
 * A valid od_request_t object that triggered this activity (optional)
 *
 * @result
 * returns false if not connected or failed to reconnect
 */
bool
odconnection_connect_if_necessary(od_connection_t connection, od_request_t request);

/*!
 * @function odconnection_set_failed
 *
 * @abstract
 * Flags a connection as failed.
 *
 * @discussion
 * Flags a connection as failed.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 *
 * @param request
 * A valid od_request_t object that triggered this activity (optional)
 */
OS_EXPORT OS_NOTHROW
void
odconnection_set_failed(od_connection_t connection, od_request_t request);

/*!
 * @function odconnection_disconnect
 *
 * @abstract
 * Forces the connection to disconnect, cancelling all inflight requests.
 *
 * @discussion
 * Forces the connection to disconnect, cancelling all inflight requests.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 */
OS_EXPORT OS_NOTHROW
void
odconnection_disconnect(od_connection_t connection);

/*!
 * @function odconnection_get_moduleconfig
 *
 * @abstract
 * Returns the moduleconfig associated with the connection.
 *
 * @discussion
 * Returns the moduleconfig associated with the connection.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 *
 * @result
 * Returns a valid od_moduleconfig_t object.
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_NOT_RETAINED
od_moduleconfig_t
odconnection_get_moduleconfig(od_connection_t connection);


/*!
 * @function odconnection_set_context
 *
 * @abstract
 * Sets a context on the connection.
 *
 * @discussion
 * Sets a context on the connection.  Only one context can be present at any given time.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 *
 * @param context
 * A pointer to data defined by the caller
 *
 * @param dealloc
 * An optional deallocation function for the context.
 */
OS_EXPORT OS_NOTHROW
void
odconnection_set_context(od_connection_t connection, void *context, od_context_dealloc_fn_t dealloc);

/*!
 * @function odconnection_copy_destination
 *
 * @abstract
 * Returns a copy of the destination associated with the current connection.
 *
 * @discussion
 * Returns a copy of the destination associated with the current connection.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 *
 * @result
 * Returns a valid CFStringRef if a destination is present.
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odconnection_copy_destination(od_connection_t connection);

/*!
 * @function odconnection_has_credentials
 *
 * @abstract
 * Returns a boolean signifying the presence of credentials associated with connection.
 *
 * @discussion
 * Returns a boolean signifying the presence of credentials associated with connection.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 *
 * @result
 * Returns a true if the connection has credentials associated with it.
 */
OS_EXPORT OS_WARN_RESULT OS_NOTHROW
bool
odconnection_has_credentials(od_connection_t connection);

OS_EXPORT OS_NOTHROW
void
odconnection_set_credential(od_connection_t connection, od_request_t request, od_credential_t credential);

/*!
 * @function odconnection_copy_credential
 *
 * @abstract
 * Returns a copy of the credentials attached to the connection.
 *
 * @discussion
 * Returns a copy of the credentials attached to the connection.
 *
 * @param connection
 * A valid od_connection_t object, must not be NULL
 *
 * @result
 * Returns a valid od_credential_t if present.
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_credential_t
odconnection_copy_credential(od_connection_t connection);


__END_DECLS

#endif
