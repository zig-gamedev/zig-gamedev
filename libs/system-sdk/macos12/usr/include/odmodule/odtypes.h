/*
 * Copyright (c) 2009-2017 Apple Inc. All rights reserved.
 */

#if !defined(__ODTYPES__)
#define __ODTYPES__

#include <stdbool.h>
#include <stdint.h>
#include <uuid/uuid.h>
#include <malloc/malloc.h>
#include <os/object.h>
#include <CoreFoundation/CoreFoundation.h>
#include <odmodule/odconstants.h>
#ifndef __od_object_t__
#define __od_object_t__ 1
typedef union {
    struct _os_object_s         *_os_obj;
    struct od_object_s          *_object;
    struct od_connection_s      *_connection;
    struct od_context_s         *_context;
    struct od_credential_s      *_credential;
    struct od_module_s          *_module;
    struct od_moduleconfig_s    *_moduleconfig;
    struct od_request_s         *_request;
    struct od_session_s         *_session;
    struct od_nodeconfig_s      *_nodeconfig;
} od_object_t __attribute__((transparent_union));
#endif // !__od_object_t__

#if __OBJC__ && !defined(__OD_USE_OBJC__)
#define __OD_USE_OBJC__ 1
#endif

#define OD_EXPORT extern __attribute__((visibility("default")))
#define OD_NOEXPORT __attribute__((visibility("hidden")))

#define OD_NORETURN                 __attribute__((__noreturn__))
#define OD_NOTHROW                  __attribute__((__nothrow__))
#define OD_SENTINEL                 __attribute__((__sentinel__))
#define OD_PURE                     __attribute__((__pure__))
#define OD_WARN_RESULT              __attribute__((__warn_unused_result__))
#define OD_ALWAYS_INLINE            __attribute__((always_inline))
#define OD_IGNORE_UNUSED            __attribute__((__nowarn_unused__))

#ifndef OD_RETURNS_RETAINED
#define OD_RETURNS_RETAINED         OS_OBJECT_RETURNS_RETAINED
#endif // !OD_RETURNS_RETAINED
#if __OD_USE_OBJC__ && defined(__has_attribute) && __has_attribute(ns_returns_not_retained)
#define OD_RETURNS_NOT_RETAINED __attribute__((__ns_returns_not_retained__))
#else
#define OD_RETURNS_NOT_RETAINED     /**/
#endif
#define OD_TYPEDECL_OBJ(name)       typedef struct od_##name##_s *od_##name##_t

#define OD_CFRETAIN_SAFE(a)         do { if ((a) != NULL) { CFRetain(a); } } while (0)
#define OD_CFRELEASE_SAFE(a)        do { if ((a) != NULL) { CFRelease(a); } } while (0)

#define OD_CFRELEASE_NULL(a)        do { if ((a) != NULL) { CFRelease(a); (a) = NULL; } } while (0)
#define OD_RELEASE_NULL(a)          do { od_release(a); a = NULL; } while (0)
#define OD_FREE(a)                  __extension__({ if (a) free(a); })
#define OD_FREE_NULL(a)             __extension__({ if (a) { free(a); (a) = NULL; } })
#define OD_ZERO_FREE_NULL(a)        do { if ((a) != NULL) { memset((a), 0, malloc_size(a)); free(a); (a) = NULL; } } while (0)
#define OD_DISPATCH_RETAIN_SAFE(a)  do { if ((a) != NULL) { dispatch_retain(a); } } while (0)
#define OD_DISPATCH_RELEASE_NULL(a) do { if ((a) != NULL) { dispatch_release(a); (a) = NULL; } } while (0)
#define OD_BLOCKRELEASE_NULL(a)		do { Block_release(a); (a) = NULL; } while (0)

#define OD_XPC_RELEASE_NULL(a)      do { if ((a) != NULL) { xpc_release(a); (a) = NULL; } } while (0)

#define OD_ATOMIC_XCHG(p, n)		__sync_lock_test_and_set((p), (n))
#define OD_ATOMIC_CMPXCHG(p, o, n)	__sync_bool_compare_and_swap((p), (o), (n))
#define OD_ATOMIC_INC(p)			__sync_add_and_fetch((p), 1)
#define OD_ATOMIC_DEC(p)			__sync_sub_and_fetch((p), 1)
#define OD_ATOMIC_ADD(p, v)			__sync_add_and_fetch((p), (v))
#define OD_ATOMIC_SUB(p, v)			__sync_sub_and_fetch((p), (v))
#define OD_ATOMIC_OR(p, v)			__sync_fetch_and_or((p), (v))
#define OD_ATOMIC_AND(p, v)			__sync_fetch_and_and((p), (v))
#define OD_ATOMIC_BARRIER()			__sync_synchronize()

#pragma mark -
#pragma mark enum types

enum {
    /* use these in critical only situations */
    eODLogAlert                     = 1L,      // P1 -- redirects to system.log
    eODLogCritical                  = 2L,      // P2 -- redirects to system.log
    
    /* typical logging levels */
    eODLogError                     = 3L,      // P3 -- some error occurred -- default logging level
    eODLogWarning                   = 4L,      // P4 -- concerning, but not fatal
    eODLogNotice                    = 5L,      // P5 -- normal log details for high-level information
    eODLogInfo                      = 6L,      // P6 -- some information info (like connection checks, scans, etc.)
    eODLogDebug                     = 7L,      // P7 -- full debug information needed to help diagnose involved isues
    eODLogDefault                   = eODLogError,
};

enum {
    eODConnectionOperationConnect       = 1L,
    eODConnectionOperationDisconnect    = 2L,
    eODConnectionOperationReconnect     = 3L,
};

enum {
    eODTriggerEventNodeOnline               = 1L << 0,
    eODTriggerEventNodeOffline              = 1L << 1,
    eODTriggerEventNodeRegister             = 1L << 2,        
    eODTriggerEventNodeUnregister           = 1L << 3,
    eODTriggerEventNodeLastServerChanged    = 1L << 4,
    eODTriggerEventNodeOptionsChanged       = 1L << 5,
    eODTriggerEventNodeDeleted              = 1L << 6,
};

enum {
    eODResponseTypeGeneralResponse      = 0,
    eODResponseTypeODQueryResponse      = 1,
    eODResponseTypeODQueryResponseSync  = 2,
};

enum {
    eODAccessModeRead          = 0L,
    eODAccessModeCreate,
    eODAccessModeDelete,
    eODAccessModeReadAttr,
    eODAccessModeWriteAttr,
};

enum {
    eODAuthInfoAttributes   = 1L,
    eODAuthInfoAuthTypes    = 2L,
    eODAuthInfoMechanisms   = 3L,
};

enum {
	eODEqualityRuleNone				= 0,
	eODEqualityRuleCaseIgnore		= 1,
	eODEqualityRuleCaseExact		= 2,
	eODEqualityRuleNumber			= 3,
	eODEqualityRuleCertificate		= 4,
	eODEqualityRuleTime				= 5,
	eODEqualityRuleTelephoneNumber	= 6,
	eODEqualityRuleOctetMatch		= 7,
};

enum {
    eODCallbackResponseSkip         = 0,    // skip this module/connection if possible (go to next module/connection)
    eODCallbackResponseAccepted     = 1,    // has been accepted and will respond directly
    eODCallbackResponseForward      = 2,    // request modified, forward to next module
    eODCallbackResponseRefused      = 3,    // will reply to client operation is unsupported
};

enum {
    eODCredentialFlagKerberos               = 1 << 0,
    eODCredentialFlagSystemKeychain         = 1 << 1,   // password is stored in system keychain
    eODCredentialFlagMutualAuthentication   = 1 << 2,   // validates tickets against local keytab accordingly
    eODCredentialFlagAuthorizationRight     = 1 << 3,   // means it's derived from a authorization ref
    
    // non-configurable options go last
    eODCredentialFlagProxyKerberos          = 1 << 31,
};

typedef uint32_t eODNodeConfigServerFlags;
typedef uint32_t eODModuleConfigServerFlags;
typedef uint32_t eODConnectionCreateFlags; // temporary to prevent breakage
typedef uint32_t eODQueryMatchType;
typedef long eODCallbackResponse;

enum {
    eODAuthTypeUnknown                            = 0l,
    eODAuthTypeAPOP                               = 1l,
    eODAuthTypeCRAM_MD5                           = 2l,
    eODAuthTypeDIGEST_MD5                         = 3l,
    eODAuthTypeMPPEPrimaryKeys                    = 4l,
    eODAuthTypeMPPEMasterKeys API_DEPRECATED_WITH_REPLACEMENT("eODAuthTypeMPPEPrimaryKeys", macos(10.7, 10.7)) = eODAuthTypeMPPEPrimaryKeys,
    eODAuthTypeMSCHAP2                            = 5l,
    eODAuthTypeNTLMv2                             = 6l,
    eODAuthTypeNTLMv2WithSessionKey               = 7l,
    eODAuthTypeSMB_LM_Key                         = 8l,
    eODAuthTypeSMB_NT_Key                         = 9l,
    eODAuthTypeSMB_NT_WithUserSessionKey          = 10l,
    eODAuthTypeWithAuthorizationRef               = 11l,
    eODAuthTypePPS                                = 12l,
    eODAuthTypeSetCertificateHash                 = 13l,
    eODAuthTypeRetainCredential                   = 14l,
    eODAuthTypeGSSAPI                             = 15l,
    eODAuthTypeSetGlobalPolicy                    = 56l,
    eODAuthTypeSetPolicyAsCurrent                 = 60l,
    eODAuthTypeGetGlobalPolicy                    = 70l,
    eODAuthTypeGetPolicy                          = 72l,
};

typedef long eODAuthType;

enum {
    eODMatchTypeAll             = kODMatchAny,
    
    eODMatchTypeEqualTo         = kODMatchEqualTo,
    eODMatchTypeBeginsWith      = kODMatchBeginsWith,
    eODMatchTypeContains        = kODMatchContains,
    eODMatchTypeEndsWith        = kODMatchEndsWith,
    
    eODMatchTypeGreaterThan     = kODMatchGreaterThan,
    eODMatchTypeLessThan        = kODMatchLessThan,
};

typedef long        eODEqualityRule;
typedef int32_t     eODMatchType;


#pragma mark -
#pragma mark typedefs

typedef uint32_t    eODLogLevel;
typedef uint32_t    eODRequestType;
typedef uint32_t    eODResponseType;

typedef long        eODConnectionOperation;

typedef char        *cstring;

OD_TYPEDECL_OBJ(connection);
OD_TYPEDECL_OBJ(context);
OD_TYPEDECL_OBJ(credential);
OD_TYPEDECL_OBJ(module);
OD_TYPEDECL_OBJ(moduleconfig);
OD_TYPEDECL_OBJ(node);
OD_TYPEDECL_OBJ(request);
OD_TYPEDECL_OBJ(session);
typedef void (*od_context_dealloc_fn_t)(void *context);
/*
 * prototypes for module callback functions
 */

typedef struct od_connection_info_s {
	void					*context;
	od_context_dealloc_fn_t context_dealloc;
	uint32_t				(*reconnect_cb)(od_connection_t connection, od_request_t request, void *context);
	void					(*disconnect_cb)(od_connection_t connection, int sock, void *context);
} od_connection_info_t;

/*!
    @function   odm_initialize
    @abstract   required callback called on first load of a module
    @discussion this callback is only called once on load of the module
    @param      module is an opaque datatype used for other calls
 */
void
odm_initialize(od_module_t module);

/*!
    @function   odm_configuration_loaded
    @abstract   notifies the module that a new configuration was loaded that uses this module
    @discussion notifies the module that a new configuration was loaded that uses this module
    @param      module is an opaque datatype used for other calls
    @param      moduleconfig is an opaque datatype used for other calls
    @param      session is an opaque session handle that can be used to gather session details
 */
void
odm_configuration_loaded(od_module_t module, od_moduleconfig_t moduleconfig, od_session_t session);

/*!
    @function   odm_copy_auth_information
    @abstract   requests information specific to this module such as required attributes, etc.
    @discussion requests information specific for this module to fulfill authentications.  The dictionary should
                contain the following keys "attributes", "auth_types" and "mechanisms".  
    @param      module is an opaque datatype used for other calls
    @result     returns a CFArrayRef value:
 
                eODAuthInfoAttributes are the attributes required from the record in order to do the authentication. Specific
                attributes are already included, so only additional attributes beyond the list below should be returned:

                    kODAttributeTypeAuthenticationAuthority
                    kODAttributeTypePasswordPolicyOptions
                    kODAttributeTypePassword
                    kODAttributeTypeGUID
                    kODAttributeTypeUniqueID

                eODAuthInfoAuthTypes are the authentication types supported by this module (kODAuthenticationTypeDIGEST_MD5, etc.)
                eODAuthInfoMechanisms are what authority tags this module handles "PasswordServer", "Kerberos", "Basic", etc.
 */
OS_NOTHROW OS_WARN_RESULT CF_RETURNS_RETAINED
CFTypeRef
odm_copy_auth_information(od_module_t module, od_moduleconfig_t moduleconfig, long info);


/*!
    @function   module entry points
    @abstract   most callbacks are identical for most API calls
    @param      module is the od_module_t of the module being asked to process
    @param      connection is the connection (if any) associated with this request (may be NULL).
                caller must handle a NULL connection.  This means the primary connection for the request
                is not this module.  Modules are guaranteed to get connections it created.
    @param      request is the request to be processed
    @param      moduleconfig is the configuration for this module callback
    @result     returns an eODCallbackResponse value as appropriate
 */

#define ODM_CALLBACK_DECL(type)     eODCallbackResponse odm_##type(od_module_t module, od_connection_t connection, od_request_t request,    \
                                                                   od_moduleconfig_t moduleconfig)
#define ODM_CUSTOMFUNCTION_DECL(type)   eODCallbackResponse type(od_module_t module, od_connection_t connection, od_request_t request,    \
                                                                 od_moduleconfig_t moduleconfig, CFTypeRef payload)
ODM_CALLBACK_DECL(RecordCopyPasswordPolicy);
ODM_CALLBACK_DECL(RecordVerifyPassword);
ODM_CALLBACK_DECL(RecordVerifyPasswordWithOptions);
ODM_CALLBACK_DECL(RecordVerifyPasswordExtended);
ODM_CALLBACK_DECL(RecordChangePassword);
ODM_CALLBACK_DECL(NodeVerifyCredentialsExtended);

ODM_CALLBACK_DECL(NodeCopySubnodeNames);
ODM_CALLBACK_DECL(NodeCopyUnreachableSubnodeNames);
ODM_CALLBACK_DECL(NodeCopySupportedRecordTypes);
ODM_CALLBACK_DECL(NodeCopySupportedAttributes);
ODM_CALLBACK_DECL(NodeSetCredentials);
ODM_CALLBACK_DECL(NodeSetCredentialsExtended);
ODM_CALLBACK_DECL(NodeSetCredentialsWithSecureToken);
ODM_CALLBACK_DECL(NodeCreateRecord);
ODM_CALLBACK_DECL(NodeReplaceUserRecord);
ODM_CALLBACK_DECL(NodeCustomCall);
ODM_CALLBACK_DECL(NodeCustomFunction);
ODM_CALLBACK_DECL(QueryCreateWithNode);
ODM_CALLBACK_DECL(QuerySynchronize);
ODM_CALLBACK_DECL(QueryCancel);
ODM_CALLBACK_DECL(RecordSetValue);
ODM_CALLBACK_DECL(RecordAddValue);
ODM_CALLBACK_DECL(RecordRemoveValue);
ODM_CALLBACK_DECL(RecordDelete);
ODM_CALLBACK_DECL(RecordCopyPolicies);
ODM_CALLBACK_DECL(RecordCopyEffectivePolicies);
ODM_CALLBACK_DECL(RecordCopySupportedPolicies);
ODM_CALLBACK_DECL(RecordSetPolicies);
ODM_CALLBACK_DECL(RecordRemovePolicy);
ODM_CALLBACK_DECL(NodeCopyPolicies);
ODM_CALLBACK_DECL(NodeCopySupportedPolicies);
ODM_CALLBACK_DECL(NodeSetPolicies);
ODM_CALLBACK_DECL(NodeRemovePolicy);

ODM_CALLBACK_DECL(RecordAddAccountPolicy);
ODM_CALLBACK_DECL(RecordRemoveAccountPolicy);
ODM_CALLBACK_DECL(RecordSetAccountPolicies);
ODM_CALLBACK_DECL(RecordCopyAccountPolicies);
ODM_CALLBACK_DECL(RecordAuthenticationAllowed);
ODM_CALLBACK_DECL(RecordPasswordChangeAllowed);
ODM_CALLBACK_DECL(RecordWillPasswordExpire);
ODM_CALLBACK_DECL(RecordWillAuthenticationsExpire);
ODM_CALLBACK_DECL(RecordSecondsUntilPasswordExpires);
ODM_CALLBACK_DECL(RecordSecondsUntilAuthenticationsExpire);
ODM_CALLBACK_DECL(RecordCopyPasswordContentSummary);
ODM_CALLBACK_DECL(NodeAddAccountPolicy);
ODM_CALLBACK_DECL(NodeRemoveAccountPolicy);
ODM_CALLBACK_DECL(NodeSetAccountPolicies);
ODM_CALLBACK_DECL(NodeCopyAccountPolicies);
ODM_CALLBACK_DECL(NodePasswordContentCheck);

#endif // __ODTYPES__
