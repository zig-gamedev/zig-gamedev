/* iig(DriverKit-191.30.2) generated from IOMemoryMap.iig */

/* IOMemoryMap.iig:1-46 */
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
#include <IOKit/IOMemoryDescriptor.h>
#endif

#ifndef _IOKIT_UIOMEMORYMAP_H
#define _IOKIT_UIOMEMORYMAP_H

#include <DriverKit/OSObject.h>  /* .iig include */

struct _IOMemoryMapPrivateState {
	uint64_t length;
	uint64_t offset;
	uint64_t options;
	uint64_t address;
};

/* source class IOMemoryMap IOMemoryMap.iig:47-87 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
 * @class IOMemoryMap
 *
 * @abstract
 * IOMemoryMap describes a memory mapping created with IOMemoryDescriptor::CreateMapping()
 *
 * @discussion
 * To allocate memory for I/O or sharing, use IOBufferMemoryDescriptor::Create()
 * Methods in this class are used for memory that was supplied as a parameter.
 */

class KERNEL IOMemoryMap : public OSObject
{
public:

	virtual bool
	init() override;

	virtual void
	free() override;
	
    /*!
     * @brief       Obtain the address of the memory mapping.
     * @return      Address.
     */
	uint64_t
	GetAddress() LOCALONLY;

    /*!
     * @brief       Obtain the length of the memory mapping.
     * @return      Length.
     */
	uint64_t
	GetLength() LOCALONLY;

    /*!
     * @brief       Obtain the offset of the memory mapping.
     * @return      Offset.
     */
	uint64_t
	GetOffset() LOCALONLY;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOMemoryMap IOMemoryMap.iig:47-87 */

#define IOMemoryMap__CopyState_ID            0xfc92b3d7f2d48ec7ULL

#define IOMemoryMap__CopyState_Args \
        _IOMemoryMapPrivateState * state

#define IOMemoryMap_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOMemoryMap * self, const IORPC rpc);\
\
    kern_return_t\
    _CopyState(\
        _IOMemoryMapPrivateState * state,\
        OSDispatchMethod supermethod = NULL);\
\
    uint64_t\
    GetAddress(\
);\
\
    uint64_t\
    GetLength(\
);\
\
    uint64_t\
    GetOffset(\
);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*_CopyState_Handler)(OSMetaClassBase * target, IOMemoryMap__CopyState_Args);\
    static kern_return_t\
    _CopyState_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _CopyState_Handler func);\
\


#define IOMemoryMap_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    _CopyState_Impl(IOMemoryMap__CopyState_Args);\
\


#define IOMemoryMap_VirtualMethods \
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


/* IOMemoryMap.iig:96- */

#endif /* ! _IOKIT_UIOMEMORYMAP_H */
