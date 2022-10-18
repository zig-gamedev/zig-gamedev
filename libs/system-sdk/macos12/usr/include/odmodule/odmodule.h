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

#if !defined(__ODMODULE_H)
#define __ODMODULE_H

#include <stdint.h>
#include <xpc/xpc.h>
#include <odmodule/odtypes.h>

enum {
	eODModuleTypeSession		= 1L << 0,
	eODModuleTypeAuthentication	= 1L << 1,
	eODModuleTypeDiscovery		= 1L << 2,
	eODModuleTypeSystemCache	= 1L << 3,
	eODModuleTypeInternal		= 1L << 4,
};

typedef int32_t eODModuleType;

typedef eODCallbackResponse (*odm_customfunction_t)(od_request_t request, od_connection_t connection, xpc_object_t custom_payload);

struct odmodule_vtable_s {
#define ODMODULE_VTABLE_VERSION 3
    int version; /* set to ODMODULE_VTABLE_VERSION */
    
    /* Callbacks not associated with API calls */
    
    /* module initialization */
    void (*odm_initialize)(od_module_t module);
    
    xpc_object_t (*odm_copy_auth_information)(od_request_t request, od_moduleconfig_t moduleconfig, long info);
    
    /* locate service initiated for this moduleconfig */
    bool (*odm_locate_service)(od_request_t request, od_moduleconfig_t moduleconfig);
    
    /* a new configuration was loaded with this moduleconfig */
    void (*odm_configuration_loaded)(od_request_t request, od_moduleconfig_t moduleconfig);
    
    /* opportunity to parse a dynamic destination that might be understood by this module */
    xpc_object_t (*odm_parse_dynamic_destination)(od_request_t request, od_moduleconfig_t moduleconfig, const char *destination);
    
    /* create a new connection with the given options */
    od_connection_t (*odm_create_connection_with_options)(od_request_t request, od_moduleconfig_t moduleconfig, od_credential_t credential, eODModuleConfigServerFlags flags, xpc_object_t option_dict);
    
    /* copy details related to the provided connection */
    xpc_object_t (*odm_copy_details)(od_request_t request, od_connection_t connection);

    /* this callback doesn't require a response, it's a notification that the record is being deleted and to remove any resources related to it */
    void (*odm_RecordDeleteAuth)(od_request_t request, od_connection_t connection, xpc_object_t addinfo_dict);
    
    /*
     * API callbacks (no need to set ones that are not implemented)
     */
    
    eODCallbackResponse (*odm_NodeSetCredentials)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                  const char *recordname, const char *password, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_NodeSetCredentialsExtended)(od_request_t request, od_connection_t connection, const char *record_type, const char *auth_type, eODAuthType od_auth_type, xpc_object_t auth_items,
                                                          od_context_t auth_context, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_NodeVerifyCredentialsExtended)(od_request_t request, od_connection_t connection, const char *record_type, const char *auth_type, eODAuthType od_auth_type, xpc_object_t auth_items,
                                                             od_context_t auth_context, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_NodeCreateRecord)(od_request_t request, od_connection_t connection, const char *record_type, const char *recordname, xpc_object_t attrib_dict);
    
    eODCallbackResponse (*odm_NodeCopyPolicies)(od_request_t request, od_connection_t connection);
    eODCallbackResponse (*odm_NodeCopySupportedPolicies)(od_request_t request, od_connection_t connection);
    eODCallbackResponse (*odm_NodeSetPolicies)(od_request_t request, od_connection_t connection, xpc_object_t policies_dict);
    eODCallbackResponse (*odm_NodeRemovePolicy)(od_request_t request, od_connection_t connection, const char *policyType);

    eODCallbackResponse (*odm_QueryCreateWithPredicates)(od_request_t request, od_connection_t connection, xpc_object_t predicates, int32_t maxResults, int32_t pageSize, xpc_object_t returnAttributes);
    eODCallbackResponse (*odm_QuerySynchronize)(od_request_t request, od_connection_t connection, od_context_t query_context);
    eODCallbackResponse (*odm_QueryCancel)(od_request_t request, od_connection_t connection, od_context_t query_context);
    
    eODCallbackResponse (*odm_RecordVerifyPassword)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                    const char *recordname, const char *password, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordVerifyPasswordExtended)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                            const char *recordname, const char *auth_type, eODAuthType od_auth_type, xpc_object_t auth_items,
                                                            od_context_t auth_context, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordChangePassword)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                    const char *recordname, const char *old_password, const char *new_password, xpc_object_t addinfo_dict);
                         
    eODCallbackResponse (*odm_RecordSetValue)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                              const char *recordname, const char *attribute, xpc_object_t value_array);
    eODCallbackResponse (*odm_RecordAddValue)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                              const char *recordname, const char *attribute, xpc_object_t value_array);
    eODCallbackResponse (*odm_RecordRemoveValue)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                 const char *recordname, const char *attribute, xpc_object_t value_array);
    eODCallbackResponse (*odm_RecordDelete)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                            const char *recordname);
    
    eODCallbackResponse (*odm_RecordCopyEffectivePolicies)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                           const char *recordname, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordCopyPolicies)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                  const char *recordname, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordCopySupportedPolicies)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                           const char *recordname, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordSetPolicies)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                               const char *recordname, xpc_object_t policies_dict, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordRemovePolicy)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                  const char *recordname, const char *policyType, xpc_object_t addinfo_dict);

    /*
     * Version 2 additions
     */

    eODCallbackResponse (*odm_NodeAddAccountPolicy)(od_request_t request, od_connection_t connection, const char *policyCategory, xpc_object_t policy);
    eODCallbackResponse (*odm_NodeRemoveAccountPolicy)(od_request_t request, od_connection_t connection, const char *policyCategory, xpc_object_t policy);
    eODCallbackResponse (*odm_NodeSetAccountPolicies)(od_request_t request, od_connection_t connection, xpc_object_t policies);
    eODCallbackResponse (*odm_NodeCopyAccountPolicies)(od_request_t request, od_connection_t connection);
    eODCallbackResponse (*odm_NodePasswordContentCheck)(od_request_t request, od_connection_t connection, const char *recordtype, const char *recordname, const char *password);

    eODCallbackResponse (*odm_RecordAddAccountPolicy)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                      const char *recordname, const char *policyCategory, xpc_object_t policy, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordRemoveAccountPolicy)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                         const char *recordname, const char *policyCategory, xpc_object_t policy, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordSetAccountPolicies)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                        const char *recordname, xpc_object_t policies, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordCopyAccountPolicies)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                         const char *recordname, xpc_object_t addinfo_dict);

    eODCallbackResponse (*odm_RecordAuthenticationAllowed)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                           const char *recordname, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordPasswordChangeAllowed)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                           const char *recordname, const char *password, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordWillAuthenticationsExpire)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                               const char *recordname, int64_t expires_in, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordWillPasswordExpire)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                        const char *recordname, int64_t expires_in, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordSecondsUntilAuthenticationsExpire)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                                       const char *recordname, xpc_object_t addinfo_dict);
    eODCallbackResponse (*odm_RecordSecondsUntilPasswordExpires)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                                 const char *recordname, xpc_object_t addinfo_dict);

    /*
     * Version 3 additions
     */

    eODCallbackResponse (*odm_RecordCopyPasswordContentSummary)(od_request_t request, od_connection_t connection, const char *record_type, const char *metarecordname,
                                                                const char *recordname, xpc_object_t addinfo_dict);

};

typedef struct odmodule_vtable_s *odmodule_vtable_t;

__BEGIN_DECLS

/*!
    @function   odmodule_log_message
    @abstract   log a module specific message, that is not associated with a request
    @discussion use only for things done internally to a module that are not associated with a request
 */
API_DEPRECATED("Switch to using os_log API for logging", macos(10.10, 10.13))
OS_EXPORT OS_NOTHROW OS_NOT_TAIL_CALLED
void
odmodule_log_message(od_module_t module, eODLogLevel level, CFStringRef format, ...) CF_FORMAT_FUNCTION(3, 4);

/*!
    @function   odmodule_dispatch_queue_create
    @abstract   utility function for creating new dispatch queues to associate with a module
    @discussion creates a new queue based on the bundleIdentifier or the bundle info plus additional components
                (e.g., "com.apple.opendirectoryd.module.ldapv3")
    @param      module is an od_module_t object
    @param      name is a const char string that is appended to the queue name (e.g., "global", "objectqueue")
                to form "com.apple.opendirectoryd.module.ldapv3.global".
    @param      attr is a valid dispatch_queue_attr_t
    @result     a valid dispatch queue object
*/
OS_EXPORT OS_NOTHROW OS_WARN_RESULT
dispatch_queue_t
odmodule_dispatch_queue_create(od_module_t module, const char *name, dispatch_queue_attr_t attr);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
int32_t
odmodule_get_option_int32(od_module_t module, CFStringRef key, int32_t defaultValue);

OS_EXPORT OS_NOTHROW
void
odmodule_set_option_int32(od_module_t module, CFStringRef key, int32_t value);

OS_EXPORT OS_NOTHROW OS_WARN_RESULT
bool
odmodule_options_current(od_module_t module);

OS_NOTHROW OS_WARN_RESULT
bool
odmodule_supports_customfunction(od_module_t module, CFStringRef function);

/*!
    @function   odmodule_main
    @abstract   only called XPCService based modules
    @discussion only called XPCService based modules
    @param      vtable is an odmodule_vtable_t.  Pointer is stored and should not be modified
                or released after calling odmodule_main()
*/
OS_NORETURN
void
odmodule_main(const odmodule_vtable_t vtable);

OS_NOTHROW OS_WARN_RESULT
bool
odmodule_supports_customfunction(od_module_t module, CFStringRef function);

CFStringRef
odmodule_copy_identifier(od_module_t module);

void
odmodule_set_identifier(od_module_t module, CFStringRef name);

__END_DECLS

#endif
