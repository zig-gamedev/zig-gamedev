#ifndef	_lockd_mach_user_
#define	_lockd_mach_user_

/* Module lockd_mach */

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

#if	(__MigKernelSpecificCode) || (_MIG_KERNEL_SPECIFIC_CODE_)
#include <kern/ipc_mig.h>
#endif /* __MigKernelSpecificCode */

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

#ifndef	lockd_mach_MSG_COUNT
#define	lockd_mach_MSG_COUNT	3
#endif	/* lockd_mach_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>
#include <lockd/lockd_mach_types.h>

#ifdef __BeforeMigUserHeader
__BeforeMigUserHeader
#endif /* __BeforeMigUserHeader */

#include <sys/cdefs.h>
__BEGIN_DECLS


/* SimpleRoutine lockd_request */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t lockd_request
(
	mach_port_t server,
	uint32_t vers,
	uint32_t flags,
	uint64_t xid,
	int64_t flk_start,
	int64_t flk_len,
	int32_t flk_pid,
	int32_t flk_type,
	int32_t flk_whence,
	sock_storage sock_address,
	xcred cred,
	uint32_t fh_len,
	nfs_handle fh
);

/* Routine lockd_ping */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t lockd_ping
(
	mach_port_t server
);

/* SimpleRoutine lockd_shutdown */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t lockd_shutdown
(
	mach_port_t server
);

__END_DECLS

/********************** Caution **************************/
/* The following data types should be used to calculate  */
/* maximum message sizes only. The actual message may be */
/* smaller, and the position of the arguments within the */
/* message layout may vary from what is presented here.  */
/* For example, if any of the arguments are variable-    */
/* sized, and less than the maximum is sent, the data    */
/* will be packed tight in the actual message to reduce  */
/* the presence of holes.                                */
/********************** Caution **************************/

/* typedefs for all requests */

#ifndef __Request__lockd_mach_subsystem__defined
#define __Request__lockd_mach_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t vers;
		uint32_t flags;
		uint64_t xid;
		int64_t flk_start;
		int64_t flk_len;
		int32_t flk_pid;
		int32_t flk_type;
		int32_t flk_whence;
		sock_storage sock_address;
		xcred cred;
		uint32_t fh_len;
		nfs_handle fh;
	} __Request__lockd_request_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__lockd_ping_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
	} __Request__lockd_shutdown_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__lockd_mach_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__lockd_mach_subsystem__defined
#define __RequestUnion__lockd_mach_subsystem__defined
union __RequestUnion__lockd_mach_subsystem {
	__Request__lockd_request_t Request_lockd_request;
	__Request__lockd_ping_t Request_lockd_ping;
	__Request__lockd_shutdown_t Request_lockd_shutdown;
};
#endif /* !__RequestUnion__lockd_mach_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__lockd_mach_subsystem__defined
#define __Reply__lockd_mach_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__lockd_request_t __attribute__((unused));
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
	} __Reply__lockd_ping_t __attribute__((unused));
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
	} __Reply__lockd_shutdown_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__lockd_mach_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__lockd_mach_subsystem__defined
#define __ReplyUnion__lockd_mach_subsystem__defined
union __ReplyUnion__lockd_mach_subsystem {
	__Reply__lockd_request_t Reply_lockd_request;
	__Reply__lockd_ping_t Reply_lockd_ping;
	__Reply__lockd_shutdown_t Reply_lockd_shutdown;
};
#endif /* !__RequestUnion__lockd_mach_subsystem__defined */

#ifndef subsystem_to_name_map_lockd_mach
#define subsystem_to_name_map_lockd_mach \
    { "lockd_request", 1023 },\
    { "lockd_ping", 1024 },\
    { "lockd_shutdown", 1025 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _lockd_mach_user_ */
