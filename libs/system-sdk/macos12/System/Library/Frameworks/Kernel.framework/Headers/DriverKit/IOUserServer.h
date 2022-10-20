/* iig(DriverKit-191.30.2) generated from IOUserServer.iig */

/* IOUserServer.iig:1-36 */
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

#ifndef _IOKIT_UIOUSERSERVER_H
#define _IOKIT_UIOUSERSERVER_H

#include <DriverKit/OSObject.h>  /* .iig include */
#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */


/* source class IOUserServer IOUserServer.iig:37-67 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
*/

class KERNEL IOUserServer : public IOService
{
public:
	static kern_return_t
	Create(
		const char      name[64],
		uint64_t        tag,
		uint64_t        options,
		OSString      * bundleID,
		IOUserServer ** server);

	virtual bool
	init() override;

	virtual void
	free() override;

	virtual kern_return_t
	Exit(const char reason[1024]) LOCAL;

	virtual kern_return_t
	LoadModule(const char path[1024]) LOCAL;

	virtual kern_return_t
	RegisterService() override;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOUserServer IOUserServer.iig:37-67 */

#define IOUserServer_Create_ID            0xc1dbaee5e75e22b9ULL
#define IOUserServer_Exit_ID            0xe949d58832ebe980ULL
#define IOUserServer_LoadModule_ID            0xd96f074a91a53982ULL

#define IOUserServer_Create_Args \
        const char * name, \
        uint64_t tag, \
        uint64_t options, \
        OSString * bundleID, \
        IOUserServer ** server

#define IOUserServer_Exit_Args \
        const char * reason

#define IOUserServer_LoadModule_Args \
        const char * path

#define IOUserServer_RegisterService_Args \


#define IOUserServer_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOUserServer * self, const IORPC rpc);\
\
    static kern_return_t\
    Create(\
        const char * name,\
        uint64_t tag,\
        uint64_t options,\
        OSString * bundleID,\
        IOUserServer ** server);\
\
    kern_return_t\
    Exit(\
        const char * reason,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    LoadModule(\
        const char * path,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    Exit_Impl(IOUserServer_Exit_Args);\
\
    kern_return_t\
    LoadModule_Impl(IOUserServer_LoadModule_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*Create_Handler)(IOUserServer_Create_Args);\
    static kern_return_t\
    Create_Invoke(const IORPC rpc,\
        Create_Handler func);\
\
    typedef kern_return_t (*Exit_Handler)(OSMetaClassBase * target, IOUserServer_Exit_Args);\
    static kern_return_t\
    Exit_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Exit_Handler func);\
\
    typedef kern_return_t (*LoadModule_Handler)(OSMetaClassBase * target, IOUserServer_LoadModule_Args);\
    static kern_return_t\
    LoadModule_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        LoadModule_Handler func);\
\


#define IOUserServer_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Impl(IOUserServer_Create_Args);\
\
    kern_return_t\
    RegisterService_Impl(IOService_RegisterService_Args);\
\


#define IOUserServer_VirtualMethods \
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





#endif /* !__DOCUMENTATION__ */

/* IOUserServer.iig:69- */

#endif /* ! _IOKIT_UIOUSERSERVER_H */
