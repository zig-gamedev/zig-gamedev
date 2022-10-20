#ifndef	_vfs_nspace_user_
#define	_vfs_nspace_user_

/* Module vfs_nspace */

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

#ifndef	vfs_nspace_MSG_COUNT
#define	vfs_nspace_MSG_COUNT	7
#endif	/* vfs_nspace_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>
#include <atm/atm_types.h>

#ifdef __BeforeMigUserHeader
__BeforeMigUserHeader
#endif /* __BeforeMigUserHeader */

#include <sys/cdefs.h>
__BEGIN_DECLS


/* Routine nspace_handle */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t send_nspace_handle
(
	mach_port_t nspace_handler_port,
	uint32_t pid,
	vfs_path_t path,
	int *handler_error
);

/* Routine nspace_resolve_cancel */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t send_nspace_resolve_cancel
(
	mach_port_t nspace_handler_port,
	uint32_t req_id
);

/* Routine nspace_resolve_path */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t send_nspace_resolve_path
(
	mach_port_t nspace_handler_port,
	uint32_t req_id,
	uint32_t pid,
	uint32_t op,
	nspace_path_t path,
	int *xxx_rdar44371223
);

/* Routine vfs_resolve_file */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t send_vfs_resolve_file
(
	mach_port_t nspace_handler_port,
	uint32_t req_id,
	uint32_t pid,
	uint32_t op,
	int64_t offset,
	int64_t size,
	nspace_path_t path
);

/* Routine vfs_resolve_dir */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t send_vfs_resolve_dir
(
	mach_port_t nspace_handler_port,
	uint32_t req_id,
	uint32_t pid,
	uint32_t op,
	nspace_name_t file_name,
	nspace_path_t path
);

/* Routine vfs_resolve_file_with_audit_token */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t send_vfs_resolve_file_with_audit_token
(
	mach_port_t nspace_handler_port,
	uint32_t req_id,
	uint32_t op,
	int64_t offset,
	int64_t size,
	nspace_path_t path,
	audit_token_t req_atoken
);

/* Routine vfs_resolve_dir_with_audit_token */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t send_vfs_resolve_dir_with_audit_token
(
	mach_port_t nspace_handler_port,
	uint32_t req_id,
	uint32_t op,
	nspace_name_t file_name,
	nspace_path_t path,
	audit_token_t req_atoken
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

#ifndef __Request__vfs_nspace_subsystem__defined
#define __Request__vfs_nspace_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t pid;
		vfs_path_t path;
	} __Request__nspace_handle_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t req_id;
	} __Request__nspace_resolve_cancel_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t req_id;
		uint32_t pid;
		uint32_t op;
		nspace_path_t path;
	} __Request__nspace_resolve_path_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t req_id;
		uint32_t pid;
		uint32_t op;
		int64_t offset;
		int64_t size;
		nspace_path_t path;
	} __Request__vfs_resolve_file_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t req_id;
		uint32_t pid;
		uint32_t op;
		nspace_name_t file_name;
		nspace_path_t path;
	} __Request__vfs_resolve_dir_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t req_id;
		uint32_t op;
		int64_t offset;
		int64_t size;
		nspace_path_t path;
		audit_token_t req_atoken;
	} __Request__vfs_resolve_file_with_audit_token_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t req_id;
		uint32_t op;
		nspace_name_t file_name;
		nspace_path_t path;
		audit_token_t req_atoken;
	} __Request__vfs_resolve_dir_with_audit_token_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__vfs_nspace_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__send_vfs_nspace_subsystem__defined
#define __RequestUnion__send_vfs_nspace_subsystem__defined
union __RequestUnion__send_vfs_nspace_subsystem {
	__Request__nspace_handle_t Request_send_nspace_handle;
	__Request__nspace_resolve_cancel_t Request_send_nspace_resolve_cancel;
	__Request__nspace_resolve_path_t Request_send_nspace_resolve_path;
	__Request__vfs_resolve_file_t Request_send_vfs_resolve_file;
	__Request__vfs_resolve_dir_t Request_send_vfs_resolve_dir;
	__Request__vfs_resolve_file_with_audit_token_t Request_send_vfs_resolve_file_with_audit_token;
	__Request__vfs_resolve_dir_with_audit_token_t Request_send_vfs_resolve_dir_with_audit_token;
};
#endif /* !__RequestUnion__send_vfs_nspace_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__vfs_nspace_subsystem__defined
#define __Reply__vfs_nspace_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int handler_error;
	} __Reply__nspace_handle_t __attribute__((unused));
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
	} __Reply__nspace_resolve_cancel_t __attribute__((unused));
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
		int xxx_rdar44371223;
	} __Reply__nspace_resolve_path_t __attribute__((unused));
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
	} __Reply__vfs_resolve_file_t __attribute__((unused));
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
	} __Reply__vfs_resolve_dir_t __attribute__((unused));
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
	} __Reply__vfs_resolve_file_with_audit_token_t __attribute__((unused));
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
	} __Reply__vfs_resolve_dir_with_audit_token_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__vfs_nspace_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__send_vfs_nspace_subsystem__defined
#define __ReplyUnion__send_vfs_nspace_subsystem__defined
union __ReplyUnion__send_vfs_nspace_subsystem {
	__Reply__nspace_handle_t Reply_send_nspace_handle;
	__Reply__nspace_resolve_cancel_t Reply_send_nspace_resolve_cancel;
	__Reply__nspace_resolve_path_t Reply_send_nspace_resolve_path;
	__Reply__vfs_resolve_file_t Reply_send_vfs_resolve_file;
	__Reply__vfs_resolve_dir_t Reply_send_vfs_resolve_dir;
	__Reply__vfs_resolve_file_with_audit_token_t Reply_send_vfs_resolve_file_with_audit_token;
	__Reply__vfs_resolve_dir_with_audit_token_t Reply_send_vfs_resolve_dir_with_audit_token;
};
#endif /* !__RequestUnion__send_vfs_nspace_subsystem__defined */

#ifndef subsystem_to_name_map_vfs_nspace
#define subsystem_to_name_map_vfs_nspace \
    { "nspace_handle", 867800 },\
    { "nspace_resolve_cancel", 867801 },\
    { "nspace_resolve_path", 867802 },\
    { "vfs_resolve_file", 867803 },\
    { "vfs_resolve_dir", 867804 },\
    { "vfs_resolve_file_with_audit_token", 867805 },\
    { "vfs_resolve_dir_with_audit_token", 867806 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _vfs_nspace_user_ */
