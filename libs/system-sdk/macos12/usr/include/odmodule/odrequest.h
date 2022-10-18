/*
 * Copyright (c) 2010 Apple Inc. All rights reserved.
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

#if !defined(__ODREQUEST_H)
#define __ODREQUEST_H

typedef bool (^od_result_validation_block_t)(od_request_t request, CFArrayRef values, CFDictionaryRef result);

__BEGIN_DECLS

/*!
    @function   odrequest_log_message
    @abstract   allows logging of a message, it will be prefixed accordingly, etc.
*/
API_DEPRECATED("Switch to using os_log API for logging", macos(10.10, 10.13))
OS_EXPORT OS_NOTHROW OS_NOT_TAIL_CALLED
void
odrequest_log_message(od_request_t request, eODLogLevel level, CFStringRef format, ...) CF_FORMAT_FUNCTION(3, 4);
/*!
    @function	odrequest_copy_context_with_uuid
    @abstract   retains the context associated with an identifier
    @discussion since requests are not persistent, contexts are stored in the od_node_t object associated
				with the request.
*/
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
od_context_t
odrequest_copy_context_with_uuid(od_request_t request, uuid_t uuid);

/*!
    @function	odrequest_remove_context
    @abstract   call to remove a context from the od_node_t associated with a request
    @discussion requests are not persistent, so contexts are attached to an od_node_t object.  When that
				node closes, these contexts are released and are no long valid.
*/
OS_EXPORT OS_NOTHROW
void
odrequest_remove_context(od_request_t request, od_context_t context);


/*!
 * @function odrequest_respond_success
 *
 * @abstract
 * Sends a response to the client that the request succeeded.
 *
 * @discussion
 * Sends a response to the client that the request succeeded.  This is used for most
 * API callbacks as generally there is an error or a response.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @result
 * Returns a eODCallbackResponse that can be returned from the API callback if necessary.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_success(od_request_t request);

/*!
 * @function odrequest_respond_error
 *
 * @abstract
 * Sends an error response to the client.
 *
 * @discussion
 * Sends an error response to the client along with additional unlocalized text used as
 * additional information only.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @result
 * Returns a eODCallbackResponse that can be returned from the API callback if necessary.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_error(od_request_t request, uint32_t error, const char *error_desc);

/*!
 * @function odrequest_respond_recordcreate
 *
 * @abstract
 * Responds to a successful record creation request.
 *
 * @discussion
 * Responds to a record creation request with a dictionary of arrays.  If record cannot be
 * created then use odrequest_respond_error with appropriate error.
 *
 * @param request
 * A valid od_request_t object that is being responded to.
 *
 * @param record_dict
 * A valid xpc_object_t of type XPC_DICTIONARY containing the native keys and values
 * for the record which will be sent through the translation layer for the client.
 *
 * @result
 * Returns a eODCallbackResponse as a convenience that can be returned from the API callback.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_recordcreate(od_request_t request, xpc_object_t record_dict);

/*!
 * @function odrequest_respond_recordcopy
 *
 * @abstract
 * Responds to a record copy request.
 *
 * @discussion
 * Responds to a record copy request with a dictionary of arrays.  If record cannot
 * be found it should be called with NULL or use odrequest_respond_error with appropriate
 * error.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @param record_dict
 * A valid xpc_object_t of type XPC_DICTIONARY containing the native keys and values
 * for the record which will be sent through the translation layer for the client.
 *
 * @result
 * Returns a eODCallbackResponse that can be returned from the API callback if necessary.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_recordcopy(od_request_t request, xpc_object_t record_dict);

/*!
 * @function odrequest_respond_customfunction
 *
 * @abstract
 * Responds to a custom function request.
 *
 * @discussion
 * Responds to a custom function request with a value appropriate for the custom function.
 * The type is defined by the custom function of the module.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @param func_defined
 * A valid xpc_object_t with native keys and values that will be sent through the translation
 * layer for the client.
 *
 * @result
 * Returns a eODCallbackResponse that can be returned from the API callback if necessary.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_customfunction(od_request_t request, xpc_object_t func_defined);

/*!
 * @function odrequest_respond_policy
 *
 * @abstract
 * Responds to a policy function request.
 *
 * @discussion
 * Responds to a policy function request with a value appropriate for the policy function.
 * An XPC_DICTIONARY should be used for the combined policy calls, otherwise the explict
 * value for the type of policy should be returned.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @param policy
 * A valid xpc_object_t appropriate for the request in question.
 *
 * @result
 * Returns a eODCallbackResponse that can be returned from the API callback if necessary.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_policy(od_request_t request, xpc_object_t policy);

/*!
 * @function odrequest_respond_authentication_continuation
 *
 * @abstract
 * Responds to an authentication request that requires more steps.
 *
 * @discussion
 * Responds to an authentication request that requires more steps.  If there are no
 * additional continuations, then module should call odrequest_respond_success.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @param auth_ctx
 * A valid od_context_t object that creates the continuation context.
 *
 * @param item_array
 * A valid xpc_object_t of type XPC_ARRAY that is to be sent to the caller.
 *
 * @result
 * Returns a eODCallbackResponse that can be returned from the API callback if necessary.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_authentication_continuation(od_request_t request, od_context_t auth_ctx, xpc_object_t item_array);

/*!
 * @function odrequest_respond_query_start
 *
 * @abstract
 * Responds to the initial query creation with a query context to be used for
 * either cancellation or resync.
 *
 * @discussion
 * Responds to the initial query creation with a query context to be used for
 * either cancellation or resync.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @param query_ctx
 * A valid od_context_t object for the query.
 *
 * @result
 * Returns a eODCallbackResponse that can be returned from the API callback if necessary.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
eODCallbackResponse
odrequest_respond_query_start(od_request_t request, od_context_t query_ctx);

/*!
 * @function odrequest_respond_query_result
 *
 * @abstract
 * Responds to the query with a result from the module.
 *
 * @discussion
 * Responds to the query with a result from the module.  If the query has completed
 * module can call odrequest_respond_success to signify the request is complete.
 *
 * @param request
 * A valid od_request_t object that is being responded to.
 *
 * @param query_ctx
 * A valid od_context_t object for the query.
 *
 * @param result_dict
 * A valid xpc_object_t object of type XPC_DICTIONARY that contains key/value pairs for
 * the record found.
 */
__OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_NA)
void
odrequest_respond_query_result(od_request_t request, od_context_t query_ctx, xpc_object_t result_dict);

/*!
 * @function odrequest_respond_query_sync
 *
 * @abstract
 * Sync a synchronization message to the client telling it that new results
 * are on their way.
 *
 * @discussion
 * Sync a synchronization message to the client telling it that new results
 * are on their way.  The client should forget all prior results.
 *
 * @param request
 * A valid od_request_t object that is being responded to
 *
 * @param query_ctx
 * A valid od_context_t object for the query.
 */
void
odrequest_respond_query_sync(od_request_t request, od_context_t query_ctx);

OS_EXPORT OS_NOTHROW
void
odrequest_submit_rpc_response(od_request_t request, xpc_object_t reply);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odrequest_testcancel(od_request_t request);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
uid_t
odrequest_get_uid(od_request_t request);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
uid_t
odrequest_get_euid(od_request_t request);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
gid_t
odrequest_get_gid(od_request_t request);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
gid_t
odrequest_get_egid(od_request_t request);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
pid_t
odrequest_get_pid(od_request_t request);

// will return the username associated with the UID of the request
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odrequest_copy_process_username(od_request_t request);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
int
odrequest_verify_access(od_request_t request, od_credential_t credential, CFStringRef authed_user);

/*!
    @function	odrequest_set_auth_connection
    @abstract   set a new connection on the request, for future continuation calls
    @discussion set a new connection on the request, for future continuation calls
    @param      request the request to attach the connection to
    @param      connection is a new usable connection, will replace any existing.  The existing will
                be released immediately.  It is retained when called.
*/
OS_EXPORT OS_NOTHROW
void
odrequest_set_auth_connection(od_request_t request, od_connection_t connection);


OS_EXPORT OS_NOTHROW OS_WARN_RESULT
eODRequestType
odrequest_get_typeid(od_request_t request);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_connection_t
odrequest_copy_session_connection(od_request_t request);

__END_DECLS

#endif
