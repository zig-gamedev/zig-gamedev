#ifndef	_arcade_upcall_server_
#define	_arcade_upcall_server_

/* Module arcade_upcall */

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

#ifndef	arcade_upcall_MSG_COUNT
#define	arcade_upcall_MSG_COUNT	1
#endif	/* arcade_upcall_MSG_COUNT */

#include <Availability.h>
#include <mach/std_types.h>
#include <mach/mig.h>
#include <mach/mig.h>
#include <mach/mach_types.h>

#ifdef __BeforeMigServerHeader
__BeforeMigServerHeader
#endif /* __BeforeMigServerHeader */

#ifndef MIG_SERVER_ROUTINE
#define MIG_SERVER_ROUTINE
#endif


/* Routine arcade_upcall */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t arcade_upcall
(
	mach_port_t arcade_upcall,
	vm_offset_t path,
	mach_msg_type_number_t pathCnt,
	uint64_t offset,
	boolean_t *should_kill
);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
boolean_t arcade_upcall_server(
		mach_msg_header_t *InHeadP,
		mach_msg_header_t *OutHeadP);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
mig_routine_t arcade_upcall_server_routine(
		mach_msg_header_t *InHeadP);


/* Description of this subsystem, for use in direct RPC */
extern const struct arcade_upcall_subsystem {
	mig_server_routine_t	server;	/* Server routine */
	mach_msg_id_t	start;	/* Min routine number */
	mach_msg_id_t	end;	/* Max routine number + 1 */
	unsigned int	maxsize;	/* Max msg size */
	vm_address_t	reserved;	/* Reserved */
	struct routine_descriptor	/*Array of routine descriptors */
		routine[1];
} arcade_upcall_subsystem;

/* typedefs for all requests */

#ifndef __Request__arcade_upcall_subsystem__defined
#define __Request__arcade_upcall_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t path;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t pathCnt;
		uint64_t offset;
	} __Request__arcade_upcall_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__arcade_upcall_subsystem__defined */


/* union of all requests */

#ifndef __RequestUnion__arcade_upcall_subsystem__defined
#define __RequestUnion__arcade_upcall_subsystem__defined
union __RequestUnion__arcade_upcall_subsystem {
	__Request__arcade_upcall_t Request_arcade_upcall;
};
#endif /* __RequestUnion__arcade_upcall_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__arcade_upcall_subsystem__defined
#define __Reply__arcade_upcall_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		boolean_t should_kill;
	} __Reply__arcade_upcall_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__arcade_upcall_subsystem__defined */


/* union of all replies */

#ifndef __ReplyUnion__arcade_upcall_subsystem__defined
#define __ReplyUnion__arcade_upcall_subsystem__defined
union __ReplyUnion__arcade_upcall_subsystem {
	__Reply__arcade_upcall_t Reply_arcade_upcall;
};
#endif /* __ReplyUnion__arcade_upcall_subsystem__defined */

#ifndef subsystem_to_name_map_arcade_upcall
#define subsystem_to_name_map_arcade_upcall \
    { "arcade_upcall", 61471 }
#endif

#ifdef __AfterMigServerHeader
__AfterMigServerHeader
#endif /* __AfterMigServerHeader */

#endif	 /* _arcade_upcall_server_ */
