#ifndef	_audit_triggers_server_
#define	_audit_triggers_server_

/* Module audit_triggers */

#include <string.h>
#include <mach/ndr.h>
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/notify.h>
#include <mach/mach_types.h>
#include <mach/message.h>
#include <mach/mig_errors.h>
#include <mach/port.h>
	
/* BEGIN MIG_STRNCPY_ZEROFILL CODE */

#if defined(__has_include)
#if __has_include(<mach/mig_strncpy_zerofill_support.h>)
#ifndef USING_MIG_STRNCPY_ZEROFILL
#define USING_MIG_STRNCPY_ZEROFILL
#endif
#ifndef __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__
#define __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__
#ifdef __cplusplus
extern "C" {
#endif
	extern int mig_strncpy_zerofill(char *dest, const char *src, int len) __attribute__((weak_import));
#ifdef __cplusplus
}
#endif
#endif /* __MIG_STRNCPY_ZEROFILL_FORWARD_TYPE_DECLS__ */
#endif /* __has_include(<mach/mig_strncpy_zerofill_support.h>) */
#endif /* __has_include */
	
/* END MIG_STRNCPY_ZEROFILL CODE */


#ifdef AUTOTEST
#ifndef FUNCTION_PTR_T
#define FUNCTION_PTR_T
typedef void (*function_ptr_t)(mach_port_t, char *, mach_msg_type_number_t);
typedef struct {
        char            *name;
        function_ptr_t  function;
} function_table_entry;
typedef function_table_entry   *function_table_t;
#endif /* FUNCTION_PTR_T */
#endif /* AUTOTEST */

#ifndef	audit_triggers_MSG_COUNT
#define	audit_triggers_MSG_COUNT	2
#endif	/* audit_triggers_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>
#include <mach/audit_triggers_types.h>

#ifdef __BeforeMigServerHeader
__BeforeMigServerHeader
#endif /* __BeforeMigServerHeader */

#ifndef MIG_SERVER_ROUTINE
#define MIG_SERVER_ROUTINE
#endif


/* SimpleRoutine audit_triggers */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t audit_triggers
(
	mach_port_t audit_port,
	int flags
);

/* SimpleRoutine audit_analytics */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t audit_analytics
(
	mach_port_t audit_port,
	string_t caller_id,
	string_t caller_name
);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
boolean_t audit_triggers_server(
		mach_msg_header_t *InHeadP,
		mach_msg_header_t *OutHeadP);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
mig_routine_t audit_triggers_server_routine(
		mach_msg_header_t *InHeadP);


/* Description of this subsystem, for use in direct RPC */
extern const struct audit_triggers_subsystem {
	mig_server_routine_t	server;	/* Server routine */
	mach_msg_id_t	start;	/* Min routine number */
	mach_msg_id_t	end;	/* Max routine number + 1 */
	unsigned int	maxsize;	/* Max msg size */
	vm_address_t	reserved;	/* Reserved */
	struct routine_descriptor	/*Array of routine descriptors */
		routine[2];
} audit_triggers_subsystem;

/* typedefs for all requests */

#ifndef __Request__audit_triggers_subsystem__defined
#define __Request__audit_triggers_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		int flags;
	} __Request__audit_triggers_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t caller_idOffset; /* MiG doesn't use it */
		mach_msg_type_number_t caller_idCnt;
		char caller_id[1024];
		mach_msg_type_number_t caller_nameOffset; /* MiG doesn't use it */
		mach_msg_type_number_t caller_nameCnt;
		char caller_name[1024];
	} __Request__audit_analytics_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__audit_triggers_subsystem__defined */


/* union of all requests */

#ifndef __RequestUnion__audit_triggers_subsystem__defined
#define __RequestUnion__audit_triggers_subsystem__defined
union __RequestUnion__audit_triggers_subsystem {
	__Request__audit_triggers_t Request_audit_triggers;
	__Request__audit_analytics_t Request_audit_analytics;
};
#endif /* __RequestUnion__audit_triggers_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__audit_triggers_subsystem__defined
#define __Reply__audit_triggers_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__audit_triggers_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__audit_analytics_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__audit_triggers_subsystem__defined */


/* union of all replies */

#ifndef __ReplyUnion__audit_triggers_subsystem__defined
#define __ReplyUnion__audit_triggers_subsystem__defined
union __ReplyUnion__audit_triggers_subsystem {
	__Reply__audit_triggers_t Reply_audit_triggers;
	__Reply__audit_analytics_t Reply_audit_analytics;
};
#endif /* __ReplyUnion__audit_triggers_subsystem__defined */

#ifndef subsystem_to_name_map_audit_triggers
#define subsystem_to_name_map_audit_triggers \
    { "audit_triggers", 123 },\
    { "audit_analytics", 124 }
#endif

#ifdef __AfterMigServerHeader
__AfterMigServerHeader
#endif /* __AfterMigServerHeader */

#endif	 /* _audit_triggers_server_ */
