#ifndef __XPC_VPROC_H__
#define __XPC_VPROC_H__

#ifndef __XPC_INDIRECT__
#define __XPC_INDIRECT__
#endif // __XPC_INDIRECT__

#include <xpc/base.h>
#include <Availability.h>

__BEGIN_DECLS;

#include <sys/types.h>

typedef struct vproc_s * vproc_t;
typedef void * vproc_err_t;
typedef void * vproc_transaction_t;
typedef void * vproc_standby_t;

__OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_6, __MAC_10_13, __IPHONE_5_0, __IPHONE_11_0)
XPC_DEPRECATED("Please use xpc_transaction_begin/end instead")
XPC_EXPORT XPC_WARN_RESULT
vproc_transaction_t
vproc_transaction_begin(vproc_t virtual_proc);

__OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_6, __MAC_10_13, __IPHONE_5_0, __IPHONE_11_0)
XPC_DEPRECATED("Please use xpc_transaction_begin/end instead")
XPC_EXPORT XPC_NONNULL2
void
vproc_transaction_end(vproc_t virtual_proc, vproc_transaction_t handle);

__OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_6, __MAC_10_13, __IPHONE_5_0, __IPHONE_11_0)
XPC_DEPRECATED("This API has not done anything since 2013. Please remove your calls to it.")
XPC_EXPORT XPC_WARN_RESULT
vproc_standby_t
vproc_standby_begin(vproc_t virtual_proc);

__OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_6, __MAC_10_13, __IPHONE_5_0, __IPHONE_11_0)
XPC_DEPRECATED("This API has not done anything since 2013. Please remove your calls to it.")
XPC_EXPORT XPC_NONNULL2
void
vproc_standby_end(vproc_t virtual_proc, vproc_standby_t handle);

__END_DECLS;

#endif // __XPC_VPROC_H__ 
