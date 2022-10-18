/*
 * Copyright (c) 2009-2013 Apple Inc. All rights reserved.
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

#if !defined(__ODMODULECONFIG_H)
#define __ODMODULECONFIG_H

__BEGIN_DECLS

OS_EXPORT const CFStringRef kODModuleConfigServerCategoryPrimary;
OS_EXPORT const CFStringRef kODModuleConfigServerCategorySecondary;

/*!
 * @function odmoduleconfig_create
 *
 * @abstract
 * creates a new moduleconfig object
 *
 * @discussion
 * creates a new object to be used for later operations
 *
 * @param module
 * is an od_module_t object associated with this config
 *
 * @param options_uuid
 * is a uuid for this moduleconfig
 *
 * @result
 * an od_moduleconfig_t object created or NULL on failure
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_moduleconfig_t
odmoduleconfig_create(od_module_t module, CFDictionaryRef options, CFStringRef options_uuid);

/*!
 * @function odmoduleconfig_copy_options
 *
 * @abstract
 * copy options for a moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is a CFDictionaryRef with moduleconfig options
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFDictionaryRef
odmoduleconfig_copy_options(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_copy_dynamicdata
 *
 * @abstract
 * copy value for a moduleconfig dynamicdata key
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param key
 * is a CFStringRef name for a key
 *
 * @result 
 * is a CFTypeRef value
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFTypeRef
odmoduleconfig_copy_dynamicdata(od_moduleconfig_t moduleconfig, CFStringRef key);

/*!
 * @function odmoduleconfig_set_dynamicdata
 *
 * @abstract
 * sets keys/values for a moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param key
 * is a CFStringRef name for a key
 * 
 * @param data
 * is a CFTypeRef value
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_set_dynamicdata(od_moduleconfig_t moduleconfig, CFStringRef key, CFTypeRef data);

/*!
 * @function odmoduleconfig_get_context
 *
 * @abstract
 * get context associated with moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is a void*
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
void *
odmoduleconfig_get_context(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_set_context
 *
 * @abstract
 * set context associated with moduleconfig
 *
 * @discussion
 * this replaces any context that is currently set, changing context while in use can cause a crash if not protected
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param context
 * is a void*
 * 
 * @param dealloc
 * is a od_context_dealloc_fn_t function invoked with context when clearing the context
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_set_context(od_moduleconfig_t moduleconfig, void *context, od_context_dealloc_fn_t dealloc);

/*!
 * @function odmoduleconfig_apply_subnodes
 *
 * @abstract
 * apply block to every subnode of a moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to enumerate
 *
 * @param applier
 * is block applied to every subnode
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_apply_subnodes(od_moduleconfig_t moduleconfig, void (^applier)(od_moduleconfig_t subnode, CFStringRef nodename));

/*!
 * @function odmoduleconfig_copy_nodename
 *
 * @abstract
 * copy nodename
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is a CFStringRef name
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odmoduleconfig_copy_nodename(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_copy_option_type_ext
 *
 * @abstract
 * copy option for moduleconfig/module
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param module
 * is an od_module_t object
 *
 * @param key
 * is a CFStringRef objecting naming the key in the option dictionary
 * 
 * @param cfTypeID
 * is a CFTypeID object specifying the type expected
 *
 * @result
 * is a CFTypeRef object
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFTypeRef
odmoduleconfig_copy_option_type_ext(od_moduleconfig_t moduleconfig,
                                    od_module_t module,
                                    CFStringRef key,
                                    CFTypeID cfTypeID);

/*!
 * @function odmoduleconfig_copy_preferred
 *
 * @abstract
 * copy preferred server for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is a CFDictionaryRef object
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFDictionaryRef
odmoduleconfig_copy_preferred(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_copy_lastused_server
 *
 * @abstract
 * copy preferred server for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is a CFDictionaryRef object
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFDictionaryRef
odmoduleconfig_copy_lastused_server(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_copy_rootconfig
 *
 * @abstract
 * copy the root moduleconfig for a moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is an od_moduleconfig_t object
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_moduleconfig_t
odmoduleconfig_copy_rootconfig(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_copy_servers
 *
 * @abstract
 * copy primary/secondary server list for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param isGlobal
 * is a bool
 *
 * @param listCategory
 * is a CFStringRef with value of kODModuleConfigServerCategoryPrimary or kODModuleConfigServerCategorySecondary
 *
 * @result
 * is a CFArrayRef object
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFArrayRef
odmoduleconfig_copy_servers(od_moduleconfig_t moduleconfig, bool isGlobal, CFStringRef listCategory);

/*!
 * @function odmoduleconfig_copy_trustcredential
 *
 * @abstract
 * copy trust credential for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is an od_credential_t object
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_credential_t
odmoduleconfig_copy_trustcredential(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_create_subnode
 *
 * @abstract
 * create a subnode
 *
 * @param parent_config
 * is an od_moduleconfig_t object to modify
 *
 * @param request
 * is an od_request_t object that triggered this change
 *
 * @param requester
 * is an od_moduleconfig_t object that requested the subnode
 *
 * @param options
 * is a CFDictionaryRef object with moduleconfig options
 *
 * @param subname
 * is a CFStringRef of the relative name/path (i.e., "node1")
 *
 * @param flags
 * is a long
 *
 * @result
 * is an od_moduleconfig_t object
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_moduleconfig_t
odmoduleconfig_create_subnode(od_moduleconfig_t parent_config,
                              od_request_t request,
                              od_moduleconfig_t requester,
                              CFDictionaryRef options,
                              CFStringRef subname,
                              long flags);

/*!
 * @function odmoduleconfig_store_password
 *
 * @abstract
 * store password for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param isGlobal
 * is a bool
 *
 * @param request
 * is an od_request_t object that triggered this change
 *
 * @param accountName
 * is a CFStringRef object naming the password account
 *
 * @param password
 * is a CFStringRef object
 *
 * @result
 * is a bool
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odmoduleconfig_store_password(od_moduleconfig_t moduleconfig,
                              bool isGlobal,
                              od_request_t request,
                              CFStringRef accountName,
                              CFStringRef password);

/*!
 * @function odmoduleconfig_delete_password
 *
 * @abstract
 * delete password for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param isGlobal
 * is a bool
 *
 * @param request
 * is an od_request_t object that triggered this change
 *
 * @param accountName
 * is a CFStringRef object naming the password account
 *
 * @result
 * is a bool
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odmoduleconfig_delete_password(od_moduleconfig_t moduleconfig,
                               bool isGlobal,
                               od_request_t request,
                               CFStringRef accountName);


/*!
 * @function odmoduleconfig_copy_password
 *
 * @abstract
 * copy password for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param isGlobal
 * is a bool
 *
 * @param request
 * is an od_request_t object that triggered this query
 *
 * @param accountName
 * is a CFStringRef object naming the password account
 *
 * @result
 * is a CFStringRef
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFStringRef
odmoduleconfig_copy_password(od_moduleconfig_t moduleconfig,
                             bool isGlobal,
                             od_request_t request,
                             CFStringRef accountName);

/*!
 * @function odmoduleconfig_fetch_password_timestamps
 *
 * @abstract
 * copy password metadata for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param isGlobal
 * is a bool
 *
 * @param request
 * is an od_request_t object that triggered this query
 *
 * @param accountName
 * is a CFStringRef object naming the password account
 *
 * @param creationDate
 * is a CFDateRef object created upon success
 *
 * @param modificationDate
 * is a CFDateRef object created upon success
 *
 * @result
 * is a bool
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odmoduleconfig_fetch_password_timestamps(od_moduleconfig_t moduleconfig,
                                         bool isGlobal,
                                         od_request_t request,
                                         CFStringRef accountName,
                                         CFDateRef *creationDate,
                                         CFDateRef *modificationDate);

/*!
 * @function odmoduleconfig_get_option_bool
 *
 * @abstract
 * get boolean option for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param module
 * is an od_module_t object
 *
 * @param key
 * is a CFStringRef object
 *
 * @result
 * is a bool
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odmoduleconfig_get_option_bool(od_moduleconfig_t moduleconfig,
                               od_module_t module,
                               CFStringRef key);

/*!
 * @function odmoduleconfig_get_option_int32
 *
 * @abstract
 * get int32 option for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param module
 * is an od_module_t object
 *
 * @param key
 * is a CFStringRef object
 *
 * @param defaultValue
 * is an int32_t that is returned if option is not present
 *
 * @result
 * is a int32
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
int32_t
odmoduleconfig_get_option_int32(od_moduleconfig_t moduleconfig,
                                od_module_t module,
                                CFStringRef key,
                                int32_t defaultValue);

/*!
 * @function odmoduleconfig_is_online
 *
 * @abstract
 * get moduleconfig online status
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @param request
 * is an od_request_t object that triggered this query
 *
 * @result
 * is a bool
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odmoduleconfig_is_online(od_moduleconfig_t moduleconfig, od_request_t request);

/*!
 * @function odmoduleconfig_is_rootconfig
 *
 * @abstract
 * test if moduleconfig is subnode or root
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to query
 *
 * @result
 * is a bool
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odmoduleconfig_is_rootconfig(od_moduleconfig_t moduleconfig);

/*!
 * @function odmoduleconfig_set_preferred
 *
 * @abstract
 * sets preferred destination for a moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param request
 * is an od_request_t object that triggered this change
 *
 * @param destination
 * is a CFDictionaryRef designating the server address
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_set_preferred(od_moduleconfig_t moduleconfig, od_request_t request,
                             CFDictionaryRef destination);

/*!
 * @function odmoduleconfig_set_servers
 *
 * @abstract
 * sets primary or secondary list of servers for moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param request
 * is an od_request_t object that triggered this change
 * 
 * @param listCategory
 * is a CFStringRef with value of kODModuleConfigServerCategoryPrimary or kODModuleConfigServerCategorySecondary
 * 
 * @param list
 * is a CFArrayRef of servers for this moduleconfig
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_set_servers(od_moduleconfig_t moduleconfig,
                           od_request_t request,
                           CFStringRef listCategory,
                           CFArrayRef list);

/*!
 * @function odmoduleconfig_set_state_unknown
 *
 * @abstract
 * sets online state for a moduleconfig to unknown
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param request
 * is an od_request_t object that triggered this change.
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_set_state_unknown(od_moduleconfig_t moduleconfig, od_request_t request);

/*!
 * @function odmoduleconfig_set_subnode_visibility
 *
 * @abstract
 * sets visibility for a node
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param visible
 * is a boolean
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_set_subnode_visibility(od_moduleconfig_t moduleconfig, bool visible);

/*!
 * @function odmoduleconfig_set_netbiosname
 *
 * @abstract
 * sets netBIOSName for a moduleconfig
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object to modify
 *
 * @param netBIOSName
 * is a CFStringRef name
 */
OS_EXPORT OS_NOTHROW
void
odmoduleconfig_set_netbiosname(od_moduleconfig_t moduleconfig, CFStringRef netBIOSName);

/*!
 * @function odmoduleconfig_create_connection
 *
 * @abstract
 * creates a new connection object
 *
 * @discussion
 * creates a new object to be used for later operations
 *
 * @param moduleconfig
 * is an od_moduleconfig_t object associated with this config
 *
 * @param request
 * is an od_request_t object that needs this connection
 *
 * @param name
 * is a CStringRef name for this connection
 *
 * @param credential
 * is an od_credential_t object for this connection
 *
 * @param flags
 * flags for this connection
 *
 * @param timeout
 * is how many seconds before the connection should be closed
 *
 * @param info
 * is od_connection_info_t object for this connection
 *
 * @result
 * an od_connection_t object created or NULL on failure
 */
OS_EXPORT OS_NOTHROW OS_WARN_RESULT OD_RETURNS_RETAINED
od_connection_t
odmoduleconfig_create_connection(od_moduleconfig_t moduleconfig, od_request_t request,
                                 CFStringRef name, od_credential_t credential,
                                 eODNodeConfigServerFlags flags,
                                 uint64_t timeout, od_connection_info_t *info);

__END_DECLS

#endif
