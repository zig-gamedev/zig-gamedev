#ifndef	_task_access_user_
#define	_task_access_user_

/* Module task_access */

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

#ifndef	task_access_MSG_COUNT
#define	task_access_MSG_COUNT	3
#endif	/* task_access_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>

#ifdef __BeforeMigUserHeader
__BeforeMigUserHeader
#endif /* __BeforeMigUserHeader */

#include <sys/cdefs.h>
__BEGIN_DECLS


/* Routine check_task_access */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t check_task_access
(
	mach_port_t task_access_port,
	int32_t calling_pid,
	uint32_t calling_gid,
	int32_t target_pid
);

/* Routine find_code_signature */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t find_code_signature
(
	mach_port_t task_access_port,
	int32_t new_pid
);

/* Routine check_task_access_with_flavor */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t check_task_access_with_flavor
(
	mach_port_t task_access_port,
	int32_t calling_pid,
	uint32_t calling_gid,
	int32_t target_pid,
	mach_task_flavor_t flavor
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

#ifndef __Request__task_access_subsystem__defined
#define __Request__task_access_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		int32_t calling_pid;
		uint32_t calling_gid;
		int32_t target_pid;
	} __Request__check_task_access_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		int32_t new_pid;
	} __Request__find_code_signature_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		int32_t calling_pid;
		uint32_t calling_gid;
		int32_t target_pid;
		mach_task_flavor_t flavor;
	} __Request__check_task_access_with_flavor_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__task_access_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__task_access_subsystem__defined
#define __RequestUnion__task_access_subsystem__defined
union __RequestUnion__task_access_subsystem {
	__Request__check_task_access_t Request_check_task_access;
	__Request__find_code_signature_t Request_find_code_signature;
	__Request__check_task_access_with_flavor_t Request_check_task_access_with_flavor;
};
#endif /* !__RequestUnion__task_access_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__task_access_subsystem__defined
#define __Reply__task_access_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__check_task_access_t __attribute__((unused));
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
	} __Reply__find_code_signature_t __attribute__((unused));
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
	} __Reply__check_task_access_with_flavor_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__task_access_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__task_access_subsystem__defined
#define __ReplyUnion__task_access_subsystem__defined
union __ReplyUnion__task_access_subsystem {
	__Reply__check_task_access_t Reply_check_task_access;
	__Reply__find_code_signature_t Reply_find_code_signature;
	__Reply__check_task_access_with_flavor_t Reply_check_task_access_with_flavor;
};
#endif /* !__RequestUnion__task_access_subsystem__defined */

#ifndef subsystem_to_name_map_task_access
#define subsystem_to_name_map_task_access \
    { "check_task_access", 27000 },\
    { "find_code_signature", 27001 },\
    { "check_task_access_with_flavor", 27002 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _task_access_user_ */
