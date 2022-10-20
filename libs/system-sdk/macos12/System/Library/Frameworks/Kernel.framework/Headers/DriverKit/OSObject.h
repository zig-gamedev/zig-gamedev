/* iig(DriverKit-191.30.2) generated from OSObject.iig */

/* OSObject.iig:1-147 */
/*
 * Copyright (c) 2019-2019 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#if !__IIG
#include <libkern/c++/OSObject.h>
#endif

#ifndef _IOKIT_UOSOBJECT_H
#define _IOKIT_UOSOBJECT_H


#if !__IIG_ATTRIBUTES_DEFINED__

#define __IIG_ATTRIBUTES_DEFINED__	1

#if __IIG || __DOCUMENTATION__

#define IIG_KERNEL				__attribute__((annotate("kernel")))
#define IIG_NATIVE       		__attribute__((annotate("native")))
#define IIG_LOCAL        		__attribute__((annotate("local")))
#define IIG_LOCALONLY    		__attribute__((annotate("localonly")))
#define IIG_REMOTE       		__attribute__((annotate("remote")))
#define IIG_LOCALHOST    		__attribute__((annotate("localhost")))
#define IIG_INVOKEREPLY  		__attribute__((annotate("invokereply")))
#define IIG_REPLY        		__attribute__((annotate("reply")))
#define IIG_PORTMAKESEND 		__attribute__((annotate("MACH_MSG_TYPE_MAKE_SEND")))
#define IIG_PORTCOPYSEND 		__attribute__((annotate("MACH_MSG_TYPE_COPY_SEND")))
#define IIG_TARGET       		__attribute__((annotate("target")))
#define IIG_TYPE(p)      		__attribute__((annotate("type=" # p)))
//#define IIG_ARRAY(maxcount)	__attribute__((annotate(# maxcount), annotate("array")))
#define IIG_EXTENDS(cls) 		__attribute__((annotate("extends=" # cls)))
//#define IIG_INTERFACE  		__attribute__((annotate("interface")))
//#define IIG_IMPLEMENTS(i)		void __implements(i *);
#define IIG_QUEUENAME(name)		__attribute__((annotate("queuename=" # name)))
#define IIG_SERIALIZABLE		__attribute__((annotate("serializable")))

#if __IIG
#define KERNEL					IIG_KERNEL
#endif /* __IIG */
#define NATIVE					IIG_NATIVE
#define LOCAL					IIG_LOCAL
#define LOCALONLY				IIG_LOCALONLY
#define REMOTE					IIG_REMOTE
#define LOCALHOST				IIG_LOCALHOST
#define INVOKEREPLY				IIG_INVOKEREPLY
#define REPLY					IIG_REPLY
#define PORTMAKESEND			IIG_PORTMAKESEND
#define PORTCOPYSEND			IIG_PORTCOPYSEND
#define TARGET					IIG_TARGET
#define TYPE(p)					IIG_TYPE(p)
//#define ARRAY(maxcount)		IIG_ARRAY(maxcount)
#define EXTENDS(cls)			IIG_EXTENDS(cls)
//#define INTERFACE				IIG_INTERFACE
//#define IMPLEMENTS(i)			IIG_IMPLEMENTS(i)
#define QUEUENAME(name)			IIG_QUEUENAME(name)

#else /* __IIG || __DOCUMENTATION__ */

#define IIG_KERNEL
#define IIG_NATIVE
#define IIG_LOCAL
#define IIG_LOCALONLY
#define IIG_REMOTE
#define IIG_LOCALHOST
#define IIG_INVOKEREPLY
#define IIG_REPLY
#define IIG_PORTMAKESEND
#define IIG_PORTCOPYSEND
#define IIG_TARGET
#define IIG_TYPE(p)
//#define IIG_ARRAY(maxcount)
#define IIG_EXTENDS(cls)
//#define IIG_INTERFACE
//#define IIG_IMPLEMENTS(i)
#define IIG_QUEUENAME(name)
#define IIG_SERIALIZABLE

#endif /* __IIG || __DOCUMENTATION__ */

#endif /* __IIG_ATTRIBUTES_DEFINED__ */


#if !__IIG
typedef OSObject OSContainer;
#else /* !__IIG */
class IIG_SERIALIZABLE OSContainer;
#endif  /* !__IIG */

class IIG_SERIALIZABLE OSData;
class IIG_SERIALIZABLE OSNumber;
class IIG_SERIALIZABLE OSString;
class IIG_SERIALIZABLE OSBoolean;
class IIG_SERIALIZABLE OSDictionary;
class IIG_SERIALIZABLE OSArray;
class IIG_SERIALIZABLE OSSet;
class IIG_SERIALIZABLE OSOrderedSet;

class OSMetaClass;
class IODispatchQueue;
typedef char IODispatchQueueName[256];

#if __IIG
/* OSObject.iig:162-164 */
#endif /* __IIG */


/* source class OSObject OSObject.iig:165-195 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
*/

class OSObject : public OSMetaClassBase
{
public:

	virtual bool
	init() LOCALONLY;

	virtual void
	free() LOCALONLY;

    virtual void
    retain() const override;

    virtual void
    release() const override;

	virtual kern_return_t
	SetDispatchQueue(
		const IODispatchQueueName name,
		IODispatchQueue         * queue) KERNEL = 0;

	virtual kern_return_t
	CopyDispatchQueue(
		const IODispatchQueueName name,
		IODispatchQueue        ** queue) KERNEL = 0;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class OSObject OSObject.iig:165-195 */

#define OSObject_SetDispatchQueue_ID            0xe608ae8273dae1bcULL
#define OSObject_CopyDispatchQueue_ID            0x95115b48fd29f7c9ULL

#define OSObject_SetDispatchQueue_Args \
        const char * name, \
        IODispatchQueue * queue

#define OSObject_CopyDispatchQueue_Args \
        const char * name, \
        IODispatchQueue ** queue

#define OSObject_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(OSObject * self, const IORPC rpc);\
\
    kern_return_t\
    SetDispatchQueue(\
        const IODispatchQueueName name,\
        IODispatchQueue * queue,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyDispatchQueue(\
        const IODispatchQueueName name,\
        IODispatchQueue ** queue,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*SetDispatchQueue_Handler)(OSMetaClassBase * target, OSObject_SetDispatchQueue_Args);\
    static kern_return_t\
    SetDispatchQueue_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetDispatchQueue_Handler func);\
\
    typedef kern_return_t (*CopyDispatchQueue_Handler)(OSMetaClassBase * target, OSObject_CopyDispatchQueue_Args);\
    static kern_return_t\
    CopyDispatchQueue_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyDispatchQueue_Handler func);\
\


#define OSObject_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define OSObject_VirtualMethods \
\
public:\
\
    virtual bool\
    init(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    free(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    retain(\
) const APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    release(\
) const APPLE_KEXT_OVERRIDE;\
\





#endif /* !__DOCUMENTATION__ */

/* OSObject.iig:197- */

#define DEFN(classname, name)                                       \
name ## _Impl(classname ## _ ## name ## _Args)

/*
 * Use of the IMPL macro is discouraged and should be replaced by a normal c++
 * method implementation (with the all method arguments) and the name of the method
 * given a suffix '_Impl'
 */

#define IMPL(classname, name)                                       \
classname :: DEFN(classname, name)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif /* ! _IOKIT_UOSOBJECT_H */
