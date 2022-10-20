#ifndef	_gssd_mach_user_
#define	_gssd_mach_user_

/* Module gssd_mach */

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

#ifndef	gssd_mach_MSG_COUNT
#define	gssd_mach_MSG_COUNT	9
#endif	/* gssd_mach_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>
#include <gssd/gssd_mach_types.h>

#ifdef __BeforeMigUserHeader
__BeforeMigUserHeader
#endif /* __BeforeMigUserHeader */

#include <sys/cdefs.h>
__BEGIN_DECLS


/* Routine mach_gss_init_sec_context */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_init_sec_context
(
	mach_port_t server,
	gssd_mechtype mech,
	gssd_byte_buffer intoken,
	mach_msg_type_number_t intokenCnt,
	uint32_t uid,
	gssd_string princ_namestr,
	gssd_string svc_namestr,
	uint32_t flags,
	uint32_t gssd_flags,
	gssd_ctx *context,
	gssd_cred *cred_handle,
	uint32_t *ret_flags,
	gssd_byte_buffer *key,
	mach_msg_type_number_t *keyCnt,
	gssd_byte_buffer *outtoken,
	mach_msg_type_number_t *outtokenCnt,
	uint32_t *major_stat,
	uint32_t *minor_stat
);

/* Routine mach_gss_accept_sec_context */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_accept_sec_context
(
	mach_port_t server,
	gssd_byte_buffer intoken,
	mach_msg_type_number_t intokenCnt,
	gssd_string svc_namestr,
	uint32_t gssd_flags,
	gssd_ctx *context,
	gssd_cred *cred_handle,
	uint32_t *flags,
	uint32_t *uid,
	gssd_gid_list gids,
	mach_msg_type_number_t *gidsCnt,
	gssd_byte_buffer *key,
	mach_msg_type_number_t *keyCnt,
	gssd_byte_buffer *outtoken,
	mach_msg_type_number_t *outtokenCnt,
	uint32_t *major_stat,
	uint32_t *minor_stat
);

/* SimpleRoutine mach_gss_log_error */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_log_error
(
	mach_port_t server,
	gssd_string mnt,
	uint32_t uid,
	gssd_string source,
	uint32_t major_stat,
	uint32_t minor_stat
);

/* Routine mach_gss_init_sec_context_v2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_init_sec_context_v2
(
	mach_port_t server,
	gssd_mechtype mech,
	gssd_byte_buffer intoken,
	mach_msg_type_number_t intokenCnt,
	uint32_t uid,
	gssd_nametype clnt_nt,
	gssd_byte_buffer clnt_princ,
	mach_msg_type_number_t clnt_princCnt,
	gssd_nametype svc_nt,
	gssd_byte_buffer svc_princ,
	mach_msg_type_number_t svc_princCnt,
	uint32_t flags,
	uint32_t *gssd_flags,
	gssd_ctx *context,
	gssd_cred *cred_handle,
	uint32_t *ret_flags,
	gssd_byte_buffer *key,
	mach_msg_type_number_t *keyCnt,
	gssd_byte_buffer *outtoken,
	mach_msg_type_number_t *outtokenCnt,
	gssd_dstring displayname,
	uint32_t *major_stat,
	uint32_t *minor_stat
);

/* Routine mach_gss_accept_sec_context_v2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_accept_sec_context_v2
(
	mach_port_t server,
	gssd_byte_buffer intoken,
	mach_msg_type_number_t intokenCnt,
	gssd_nametype svc_nt,
	gssd_byte_buffer svc_princ,
	mach_msg_type_number_t svc_princCnt,
	uint32_t *gssd_flags,
	gssd_ctx *context,
	gssd_cred *cred_handle,
	uint32_t *flags,
	uint32_t *uid,
	gssd_gid_list gids,
	mach_msg_type_number_t *gidsCnt,
	gssd_byte_buffer *key,
	mach_msg_type_number_t *keyCnt,
	gssd_byte_buffer *outtoken,
	mach_msg_type_number_t *outtokenCnt,
	uint32_t *major_stat,
	uint32_t *minor_stat
);

/* Routine mach_gss_init_sec_context_v3 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_init_sec_context_v3
(
	mach_port_t server,
	gssd_mechtype mech,
	gssd_byte_buffer intoken,
	mach_msg_type_number_t intokenCnt,
	uint32_t uid,
	gssd_nametype clnt_nt,
	gssd_byte_buffer clnt_princ,
	mach_msg_type_number_t clnt_princCnt,
	gssd_nametype svc_nt,
	gssd_byte_buffer svc_princ,
	mach_msg_type_number_t svc_princCnt,
	uint32_t flags,
	gssd_etype_list etypes,
	mach_msg_type_number_t etypesCnt,
	uint32_t *gssd_flags,
	gssd_ctx *context,
	gssd_cred *cred_handle,
	uint32_t *ret_flags,
	gssd_byte_buffer *key,
	mach_msg_type_number_t *keyCnt,
	gssd_byte_buffer *outtoken,
	mach_msg_type_number_t *outtokenCnt,
	gssd_dstring displayname,
	uint32_t *major_stat,
	uint32_t *minor_stat
);

/* Routine mach_gss_hold_cred */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_hold_cred
(
	mach_port_t server,
	gssd_mechtype mech,
	gssd_nametype nt,
	gssd_byte_buffer princ,
	mach_msg_type_number_t princCnt,
	uint32_t *major_stat,
	uint32_t *minor_stat
);

/* Routine mach_gss_unhold_cred */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_unhold_cred
(
	mach_port_t server,
	gssd_mechtype mech,
	gssd_nametype nt,
	gssd_byte_buffer princ,
	mach_msg_type_number_t princCnt,
	uint32_t *major_stat,
	uint32_t *minor_stat
);

/* Routine mach_gss_lookup */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t mach_gss_lookup
(
	mach_port_t server,
	uint32_t uid,
	int32_t asid,
	mach_port_t *gssd_session_port
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

#ifndef __Request__gssd_mach_subsystem__defined
#define __Request__gssd_mach_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t intoken;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		gssd_mechtype mech;
		mach_msg_type_number_t intokenCnt;
		uint32_t uid;
		mach_msg_type_number_t princ_namestrOffset; /* MiG doesn't use it */
		mach_msg_type_number_t princ_namestrCnt;
		char princ_namestr[1024];
		mach_msg_type_number_t svc_namestrOffset; /* MiG doesn't use it */
		mach_msg_type_number_t svc_namestrCnt;
		char svc_namestr[1024];
		uint32_t flags;
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
	} __Request__mach_gss_init_sec_context_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t intoken;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t intokenCnt;
		mach_msg_type_number_t svc_namestrOffset; /* MiG doesn't use it */
		mach_msg_type_number_t svc_namestrCnt;
		char svc_namestr[1024];
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
	} __Request__mach_gss_accept_sec_context_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		mach_msg_type_number_t mntOffset; /* MiG doesn't use it */
		mach_msg_type_number_t mntCnt;
		char mnt[1024];
		uint32_t uid;
		mach_msg_type_number_t sourceOffset; /* MiG doesn't use it */
		mach_msg_type_number_t sourceCnt;
		char source[1024];
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Request__mach_gss_log_error_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t intoken;
		mach_msg_ool_descriptor_t clnt_princ;
		mach_msg_ool_descriptor_t svc_princ;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		gssd_mechtype mech;
		mach_msg_type_number_t intokenCnt;
		uint32_t uid;
		gssd_nametype clnt_nt;
		mach_msg_type_number_t clnt_princCnt;
		gssd_nametype svc_nt;
		mach_msg_type_number_t svc_princCnt;
		uint32_t flags;
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
	} __Request__mach_gss_init_sec_context_v2_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t intoken;
		mach_msg_ool_descriptor_t svc_princ;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t intokenCnt;
		gssd_nametype svc_nt;
		mach_msg_type_number_t svc_princCnt;
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
	} __Request__mach_gss_accept_sec_context_v2_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t intoken;
		mach_msg_ool_descriptor_t clnt_princ;
		mach_msg_ool_descriptor_t svc_princ;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		gssd_mechtype mech;
		mach_msg_type_number_t intokenCnt;
		uint32_t uid;
		gssd_nametype clnt_nt;
		mach_msg_type_number_t clnt_princCnt;
		gssd_nametype svc_nt;
		mach_msg_type_number_t svc_princCnt;
		uint32_t flags;
		mach_msg_type_number_t etypesCnt;
		int32_t etypes[64];
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
	} __Request__mach_gss_init_sec_context_v3_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t princ;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		gssd_mechtype mech;
		gssd_nametype nt;
		mach_msg_type_number_t princCnt;
	} __Request__mach_gss_hold_cred_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t princ;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		gssd_mechtype mech;
		gssd_nametype nt;
		mach_msg_type_number_t princCnt;
	} __Request__mach_gss_unhold_cred_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		uint32_t uid;
		int32_t asid;
	} __Request__mach_gss_lookup_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__gssd_mach_subsystem__defined */

/* union of all requests */

#ifndef __RequestUnion__gssd_mach_subsystem__defined
#define __RequestUnion__gssd_mach_subsystem__defined
union __RequestUnion__gssd_mach_subsystem {
	__Request__mach_gss_init_sec_context_t Request_mach_gss_init_sec_context;
	__Request__mach_gss_accept_sec_context_t Request_mach_gss_accept_sec_context;
	__Request__mach_gss_log_error_t Request_mach_gss_log_error;
	__Request__mach_gss_init_sec_context_v2_t Request_mach_gss_init_sec_context_v2;
	__Request__mach_gss_accept_sec_context_v2_t Request_mach_gss_accept_sec_context_v2;
	__Request__mach_gss_init_sec_context_v3_t Request_mach_gss_init_sec_context_v3;
	__Request__mach_gss_hold_cred_t Request_mach_gss_hold_cred;
	__Request__mach_gss_unhold_cred_t Request_mach_gss_unhold_cred;
	__Request__mach_gss_lookup_t Request_mach_gss_lookup;
};
#endif /* !__RequestUnion__gssd_mach_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__gssd_mach_subsystem__defined
#define __Reply__gssd_mach_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t key;
		mach_msg_ool_descriptor_t outtoken;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		gssd_ctx context;
		gssd_cred cred_handle;
		uint32_t ret_flags;
		mach_msg_type_number_t keyCnt;
		mach_msg_type_number_t outtokenCnt;
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Reply__mach_gss_init_sec_context_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t key;
		mach_msg_ool_descriptor_t outtoken;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		gssd_ctx context;
		gssd_cred cred_handle;
		uint32_t flags;
		uint32_t uid;
		mach_msg_type_number_t gidsCnt;
		uint32_t gids[16];
		mach_msg_type_number_t keyCnt;
		mach_msg_type_number_t outtokenCnt;
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Reply__mach_gss_accept_sec_context_t __attribute__((unused));
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
	} __Reply__mach_gss_log_error_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t key;
		mach_msg_ool_descriptor_t outtoken;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
		uint32_t ret_flags;
		mach_msg_type_number_t keyCnt;
		mach_msg_type_number_t outtokenCnt;
		mach_msg_type_number_t displaynameOffset; /* MiG doesn't use it */
		mach_msg_type_number_t displaynameCnt;
		char displayname[128];
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Reply__mach_gss_init_sec_context_v2_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t key;
		mach_msg_ool_descriptor_t outtoken;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
		uint32_t flags;
		uint32_t uid;
		mach_msg_type_number_t gidsCnt;
		uint32_t gids[16];
		mach_msg_type_number_t keyCnt;
		mach_msg_type_number_t outtokenCnt;
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Reply__mach_gss_accept_sec_context_v2_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t key;
		mach_msg_ool_descriptor_t outtoken;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint32_t gssd_flags;
		gssd_ctx context;
		gssd_cred cred_handle;
		uint32_t ret_flags;
		mach_msg_type_number_t keyCnt;
		mach_msg_type_number_t outtokenCnt;
		mach_msg_type_number_t displaynameOffset; /* MiG doesn't use it */
		mach_msg_type_number_t displaynameCnt;
		char displayname[128];
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Reply__mach_gss_init_sec_context_v3_t __attribute__((unused));
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
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Reply__mach_gss_hold_cred_t __attribute__((unused));
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
		uint32_t major_stat;
		uint32_t minor_stat;
	} __Reply__mach_gss_unhold_cred_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_port_descriptor_t gssd_session_port;
		/* end of the kernel processed data */
	} __Reply__mach_gss_lookup_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__gssd_mach_subsystem__defined */

/* union of all replies */

#ifndef __ReplyUnion__gssd_mach_subsystem__defined
#define __ReplyUnion__gssd_mach_subsystem__defined
union __ReplyUnion__gssd_mach_subsystem {
	__Reply__mach_gss_init_sec_context_t Reply_mach_gss_init_sec_context;
	__Reply__mach_gss_accept_sec_context_t Reply_mach_gss_accept_sec_context;
	__Reply__mach_gss_log_error_t Reply_mach_gss_log_error;
	__Reply__mach_gss_init_sec_context_v2_t Reply_mach_gss_init_sec_context_v2;
	__Reply__mach_gss_accept_sec_context_v2_t Reply_mach_gss_accept_sec_context_v2;
	__Reply__mach_gss_init_sec_context_v3_t Reply_mach_gss_init_sec_context_v3;
	__Reply__mach_gss_hold_cred_t Reply_mach_gss_hold_cred;
	__Reply__mach_gss_unhold_cred_t Reply_mach_gss_unhold_cred;
	__Reply__mach_gss_lookup_t Reply_mach_gss_lookup;
};
#endif /* !__RequestUnion__gssd_mach_subsystem__defined */

#ifndef subsystem_to_name_map_gssd_mach
#define subsystem_to_name_map_gssd_mach \
    { "mach_gss_init_sec_context", 999 },\
    { "mach_gss_accept_sec_context", 1000 },\
    { "mach_gss_log_error", 1001 },\
    { "mach_gss_init_sec_context_v2", 1002 },\
    { "mach_gss_accept_sec_context_v2", 1003 },\
    { "mach_gss_init_sec_context_v3", 1004 },\
    { "mach_gss_hold_cred", 1005 },\
    { "mach_gss_unhold_cred", 1006 },\
    { "mach_gss_lookup", 1007 }
#endif

#ifdef __AfterMigUserHeader
__AfterMigUserHeader
#endif /* __AfterMigUserHeader */

#endif	 /* _gssd_mach_user_ */
