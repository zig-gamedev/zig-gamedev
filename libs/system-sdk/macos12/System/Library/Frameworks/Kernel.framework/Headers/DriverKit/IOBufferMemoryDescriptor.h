/* iig(DriverKit-191.30.2) generated from IOBufferMemoryDescriptor.iig */

/* IOBufferMemoryDescriptor.iig:1-40 */
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
#include <IOKit/IOBufferMemoryDescriptor.h>
#endif


#ifndef _IOKIT_UIOBUFFERMEMORYDESCRIPTOR_H
#define _IOKIT_UIOBUFFERMEMORYDESCRIPTOR_H

#include <DriverKit/IOMemoryDescriptor.h>  /* .iig include */

/* source class IOBufferMemoryDescriptor IOBufferMemoryDescriptor.iig:41-96 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
 * @class IOBufferMemoryDescriptor
 *
 * @abstract
 * IOBufferMemoryDescriptor describes a memory buffer allocated in the callers address space.
 *
 * @discussion
 * To allocate memory for I/O or sharing, use IOBufferMemoryDescriptor::Create().
 * IOBufferMemoryDescriptor can be handed to any API that expects an IOMemoryDescriptor.
 */

class KERNEL IOBufferMemoryDescriptor : public IOMemoryDescriptor
{
public:

    /*!
     * @brief       Create an IOBufferMemoryDescriptor.
     * @param       options Pass the flags 	kIOMemoryDirectionIn, kIOMemoryDirectionOut or kIOMemoryDirectionOutIn
     *              to set the direction of the i/o
     * @param       capacity Maximum length of the memory buffer. The descriptor has no valid data
     *              and zero length until set with SetLength().
     * @param       memory Created descriptor with +1 retain count to be released by the caller.
     * @param       alignment For small less-than-page-size buffers, control the alignment of the memory buffer.
     *              Pass zero for no guaranteed alignment.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	static kern_return_t
	Create(
		uint64_t options,
		uint64_t capacity,
		uint64_t alignment,
		IOBufferMemoryDescriptor ** memory);

	virtual bool
	init() override;

	virtual void
	free() override;

    /*!
     * @brief       Obtain the address and length of the memory buffer.
     * @param       range An IOAddressSegment structure filled out with the address and length of the memory buffer.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	kern_return_t
	GetAddressRange(IOAddressSegment * range) LOCALONLY;

    /*!
     * @brief       Set the valid length of the memory buffer.
     * @discussion  IOBufferMemoryDescriptor have capacity allocated at Create() but no valid data until set
     *              with this method.
     * @param       length New valid length of the memory described.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	SetLength(uint64_t length);
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOBufferMemoryDescriptor IOBufferMemoryDescriptor.iig:41-96 */

#define IOBufferMemoryDescriptor_Create_ID            0xb78de684e17d5a4bULL
#define IOBufferMemoryDescriptor_SetLength_ID            0xc115230c191a6a9aULL

#define IOBufferMemoryDescriptor_Create_Args \
        uint64_t options, \
        uint64_t capacity, \
        uint64_t alignment, \
        IOBufferMemoryDescriptor ** memory

#define IOBufferMemoryDescriptor_SetLength_Args \
        uint64_t length

#define IOBufferMemoryDescriptor_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOBufferMemoryDescriptor * self, const IORPC rpc);\
\
    static kern_return_t\
    Create(\
        uint64_t options,\
        uint64_t capacity,\
        uint64_t alignment,\
        IOBufferMemoryDescriptor ** memory);\
\
    kern_return_t\
    GetAddressRange(\
        IOAddressSegment * range);\
\
    kern_return_t\
    SetLength(\
        uint64_t length,\
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
    typedef kern_return_t (*Create_Handler)(IOBufferMemoryDescriptor_Create_Args);\
    static kern_return_t\
    Create_Invoke(const IORPC rpc,\
        Create_Handler func);\
\
    typedef kern_return_t (*SetLength_Handler)(OSMetaClassBase * target, IOBufferMemoryDescriptor_SetLength_Args);\
    static kern_return_t\
    SetLength_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetLength_Handler func);\
\


#define IOBufferMemoryDescriptor_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Impl(IOBufferMemoryDescriptor_Create_Args);\
\
    kern_return_t\
    SetLength_Impl(IOBufferMemoryDescriptor_SetLength_Args);\
\


#define IOBufferMemoryDescriptor_VirtualMethods \
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

/* IOBufferMemoryDescriptor.iig:98- */

#endif /* ! _IOKIT_UIOBUFFERMEMORYDESCRIPTOR_H */
