/* iig(DriverKit-191.30.2) generated from IODispatchSource.iig */

/* IODispatchSource.iig:1-36 */
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

#ifndef _IOKIT_UIODISPATCHSOURCE_H
#define _IOKIT_UIODISPATCHSOURCE_H

#include <DriverKit/OSObject.h>  /* .iig include */


typedef void (^IODispatchSourceCancelHandler)(void);

/* source class IODispatchSource IODispatchSource.iig:37-66 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
 * @class IODispatchSource

 * @abstract
 * IODispatchSource common base class for dispatch sources.
 */

class NATIVE KERNEL IODispatchSource : public OSObject
{
public:

	virtual bool
	init() override;

	virtual void
	free() override;

	virtual kern_return_t
	Cancel(IODispatchSourceCancelHandler handler) = 0;

	virtual kern_return_t
	SetEnableWithCompletion(
		bool enable,
		IODispatchSourceCancelHandler handler) = 0;

	virtual kern_return_t
	CheckForWork(bool synchronous) INVOKEREPLY = 0;

	virtual kern_return_t
	SetEnable(bool enable) LOCAL;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IODispatchSource IODispatchSource.iig:37-66 */

#define IODispatchSource_Cancel_ID            0xd963bb196f70bb93ULL
#define IODispatchSource_SetEnableWithCompletion_ID            0xbb42c489fe4dee8eULL
#define IODispatchSource_CheckForWork_ID            0xef19d38d4f945fd0ULL
#define IODispatchSource_SetEnable_ID            0xbbc036b4c2a26c45ULL

#define IODispatchSource_Cancel_Args \
        IODispatchSourceCancelHandler handler

#define IODispatchSource_SetEnableWithCompletion_Args \
        bool enable, \
        IODispatchSourceCancelHandler handler

#define IODispatchSource_CheckForWork_Args \
        const IORPC rpc, \
        bool synchronous

#define IODispatchSource_SetEnable_Args \
        bool enable

#define IODispatchSource_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IODispatchSource * self, const IORPC rpc);\
\
    kern_return_t\
    Cancel(\
        IODispatchSourceCancelHandler handler,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetEnableWithCompletion(\
        bool enable,\
        IODispatchSourceCancelHandler handler,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CheckForWork(\
        bool synchronous,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetEnable(\
        bool enable,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    SetEnable_Impl(IODispatchSource_SetEnable_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*Cancel_Handler)(OSMetaClassBase * target, IODispatchSource_Cancel_Args);\
    static kern_return_t\
    Cancel_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Cancel_Handler func);\
\
    typedef kern_return_t (*SetEnableWithCompletion_Handler)(OSMetaClassBase * target, IODispatchSource_SetEnableWithCompletion_Args);\
    static kern_return_t\
    SetEnableWithCompletion_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetEnableWithCompletion_Handler func);\
\
    typedef kern_return_t (*CheckForWork_Handler)(OSMetaClassBase * target, IODispatchSource_CheckForWork_Args);\
    static kern_return_t\
    CheckForWork_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CheckForWork_Handler func);\
\
    typedef kern_return_t (*SetEnable_Handler)(OSMetaClassBase * target, IODispatchSource_SetEnable_Args);\
    static kern_return_t\
    SetEnable_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetEnable_Handler func);\
\


#define IODispatchSource_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define IODispatchSource_VirtualMethods \
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



class IODispatchSourceInterface : public OSInterface
{
public:
};

struct IODispatchSource_IVars;
struct IODispatchSource_LocalIVars;

class IODispatchSource : public OSObject, public IODispatchSourceInterface
{
    OSDeclareDefaultStructorsWithDispatch(IODispatchSource);


public:
#ifdef IODispatchSource_DECLARE_IVARS
IODispatchSource_DECLARE_IVARS
#else /* IODispatchSource_DECLARE_IVARS */
    union
    {
        IODispatchSource_IVars * ivars;
        IODispatchSource_LocalIVars * lvars;
    };
#endif /* IODispatchSource_DECLARE_IVARS */

    using super = OSObject;


    IODispatchSource_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */

/* IODispatchSource.iig:68- */

#endif /* ! _IOKIT_UIODISPATCHSOURCE_H */
