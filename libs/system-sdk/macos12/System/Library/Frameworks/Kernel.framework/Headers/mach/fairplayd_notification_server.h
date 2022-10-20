#ifndef	_fairplay_server_
#define	_fairplay_server_

/* Module fairplay */

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

#ifndef	fairplay_MSG_COUNT
#define	fairplay_MSG_COUNT	1
#endif	/* fairplay_MSG_COUNT */

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


/* SimpleRoutine fairplayd_arcade_request */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
MIG_SERVER_ROUTINE
kern_return_t fairplayd_arcade_request
(
	mach_port_t fairplayd_port,
	mach_port_t arcade_reg_port
);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
boolean_t fairplay_server(
		mach_msg_header_t *InHeadP,
		mach_msg_header_t *OutHeadP);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
mig_routine_t fairplay_server_routine(
		mach_msg_header_t *InHeadP);


/* Description of this subsystem, for use in direct RPC */
extern const struct fairplay_subsystem {
	mig_server_routine_t	server;	/* Server routine */
	mach_msg_id_t	start;	/* Min routine number */
	mach_msg_id_t	end;	/* Max routine number + 1 */
	unsigned int	maxsize;	/* Max msg size */
	vm_address_t	reserved;	/* Reserved */
	struct routine_descriptor	/*Array of routine descriptors */
		routine[1];
} fairplay_subsystem;

/* typedefs for all requests */

#ifndef __Request__fairplay_subsystem__defined
#define __Request__fairplay_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_port_descriptor_t arcade_reg_port;
		/* end of the kernel processed data */
	} __Request__fairplayd_arcade_request_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Request__fairplay_subsystem__defined */


/* union of all requests */

#ifndef __RequestUnion__fairplay_subsystem__defined
#define __RequestUnion__fairplay_subsystem__defined
union __RequestUnion__fairplay_subsystem {
	__Request__fairplayd_arcade_request_t Request_fairplayd_arcade_request;
};
#endif /* __RequestUnion__fairplay_subsystem__defined */
/* typedefs for all replies */

#ifndef __Reply__fairplay_subsystem__defined
#define __Reply__fairplay_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(push, 4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply__fairplayd_arcade_request_t __attribute__((unused));
#ifdef  __MigPackStructs
#pragma pack(pop)
#endif
#endif /* !__Reply__fairplay_subsystem__defined */


/* union of all replies */

#ifndef __ReplyUnion__fairplay_subsystem__defined
#define __ReplyUnion__fairplay_subsystem__defined
union __ReplyUnion__fairplay_subsystem {
	__Reply__fairplayd_arcade_request_t Reply_fairplayd_arcade_request;
};
#endif /* __ReplyUnion__fairplay_subsystem__defined */

#ifndef subsystem_to_name_map_fairplay
#define subsystem_to_name_map_fairplay \
    { "fairplayd_arcade_request", 41471 }
#endif

#ifdef __AfterMigServerHeader
__AfterMigServerHeader
#endif /* __AfterMigServerHeader */

#endif	 /* _fairplay_server_ */
