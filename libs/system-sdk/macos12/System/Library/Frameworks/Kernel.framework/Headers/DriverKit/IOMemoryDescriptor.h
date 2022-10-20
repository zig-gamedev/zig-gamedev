/* iig(DriverKit-191.30.2) generated from IOMemoryDescriptor.iig */

/* IOMemoryDescriptor.iig:1-78 */
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

#ifndef _IOKIT_UIOMEMORYDESCRIPTOR_H
#define _IOKIT_UIOMEMORYDESCRIPTOR_H

#include <DriverKit/OSObject.h>  /* .iig include */

class IOService;
class IOMemoryMap;


// IOMemoryDescriptor Create options
enum {
	kIOMemoryDirectionNone       = 0x00000000,
	kIOMemoryDirectionIn         = 0x00000001,
	kIOMemoryDirectionOut        = 0x00000002,
	kIOMemoryDirectionOutIn      = kIOMemoryDirectionIn | kIOMemoryDirectionOut,
	kIOMemoryDirectionInOut      = kIOMemoryDirectionOutIn,
	kIOMemoryDisableCopyOnWrite  = 0x00000010
};

// IOMemoryDescriptor CreateMapping options
enum {
	kIOMemoryMapFixedAddress          = 0x00000001,
	kIOMemoryMapReadOnly              = 0x00000002,
	kIOMemoryMapGuardedMask           = 0x0000001C,
	kIOMemoryMapGuardedDefault        = 0x00000000,
	kIOMemoryMapGuardedNone           = 0x00000004,
	kIOMemoryMapGuardedSmall          = 0x00000008,
	kIOMemoryMapGuardedLarge          = 0x0000000C,
	kIOMemoryMapCacheModeDefault      = 0x00000000,
	kIOMemoryMapCacheModeInhibit      = 0x00000100,
	kIOMemoryMapCacheModeCopyback     = 0x00000200,
	kIOMemoryMapCacheModeWriteThrough = 0x00000400,
};

struct IOAddressSegment {
	uint64_t address;
	uint64_t length;
};

struct _IOMDPrivateState {
	uint64_t length;
	uint64_t options;
};

/* source class IOMemoryDescriptor IOMemoryDescriptor.iig:79-191 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
 * @class IOMemoryDescriptor
 *
 * @abstract
 * IOMemoryDescriptor describes a memory buffer. 
 *
 * @discussion
 * To allocate memory for I/O or sharing, use IOBufferMemoryDescriptor::Create()
 * Methods in this class are used for memory that was supplied as a parameter.
 *

*/

class KERNEL IOMemoryDescriptor : public OSObject
{
public:


	virtual bool
	init() override;

	virtual void
	free() override;

    /*!
     * @brief       Obtain the length of the memory described.
     * @param       returnLength Returned length.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	kern_return_t
	GetLength(
		uint64_t * returnLength) LOCALONLY;

    /*!
     * @brief       Create a mapping of the memory in the callers address space.
     * @param       options
	 *              kIOMemoryMapFixedAddress map at the address requested
	 *              kIOMemoryMapReadOnly create a read only mapping
	 *              kIOMemoryMapCacheModeDefault default cache mode
	 *              kIOMemoryMapCacheModeInhibit inhibited cache mode
	 *              kIOMemoryMapCacheModeCopyback copyback cache mode
	 *              kIOMemoryMapCacheModeWriteThrough write through cache mode
	 * @param       address Requested address if kIOMemoryMapFixedAddress was passed
	 * @param       offset Start offset of the mapping in the descriptor.
	 * @param       length Pass zero to map the entire memory, or a value <= the length of the descriptor.
	 * @param       alignment of the memory virtual mapping. Only zero for no alignment is supported.
	 * @param       map Returned IOMemoryMap object with +1 retain count.
	 *              It should be retained until the map is no longer required.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	CreateMapping(
		uint64_t options,
		uint64_t address,
		uint64_t offset,
		uint64_t length,
		uint64_t alignment,
		IOMemoryMap ** map);

    /*!
     * @brief       Create a memory descriptor that is a subrange of another memory
     *              descriptor
     * @param       memoryDescriptorCreateOptions
	 *              kIOMemoryDirectionIn memory described will be writable
	 *              kIOMemoryDirectionOut memory described will be readable
	 * @param       offset Start offset of the memory relative to the descriptor ofDescriptor.
	 * @param       length Length of the memory.
	 * @param       ofDescriptor Memory descriptor describing source memory.
	 * @param       memory Returned IOMemoryDescriptor object with +1 retain count.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	static kern_return_t
	CreateSubMemoryDescriptor(
	    uint64_t memoryDescriptorCreateOptions,
	    uint64_t offset,
	    uint64_t length,
		IOMemoryDescriptor * ofDescriptor,
		IOMemoryDescriptor ** memory) __attribute__((availability(driverkit,introduced=20.0)));

    /*!
     * @brief       Create a memory descriptor that is a concatenation of a set of memory
     *              descriptors
     * @param       memoryDescriptorCreateOptions
	 *              kIOMemoryDirectionIn memory described will be writable. The source
	 *              descriptors must include this direction.
	 *              kIOMemoryDirectionOut memory described will be readable. The source
	 *              descriptors must include this direction.
	 * @param       withDescriptorsCount Number of valid memory descriptors being passed
	 *              in the withDescriptors array.
	 * @param       withDescriptors Source memory descriptor array.
	 * @param       memory Returned IOMemoryDescriptor object with +1 retain count.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	static kern_return_t
	CreateWithMemoryDescriptors(
	    uint64_t memoryDescriptorCreateOptions,
		uint32_t withDescriptorsCount,
		IOMemoryDescriptor * const withDescriptors[32],
		IOMemoryDescriptor ** memory) __attribute__((availability(driverkit,introduced=20.0)));

private:
	kern_return_t
	Map(
		uint64_t options,
		uint64_t address,
		uint64_t length,
		uint64_t alignment,

		uint64_t * returnAddress,
		uint64_t * returnLength) LOCALONLY;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOMemoryDescriptor IOMemoryDescriptor.iig:79-191 */

#define IOMemoryDescriptor__CopyState_ID            0xa2c0861d4118ce5eULL
#define IOMemoryDescriptor_CreateMapping_ID            0xc5e69b0414ff6ee5ULL
#define IOMemoryDescriptor_CreateSubMemoryDescriptor_ID            0xb085b5ee60ac732fULL
#define IOMemoryDescriptor_CreateWithMemoryDescriptors_ID            0xba1530c996c4febcULL

#define IOMemoryDescriptor__CopyState_Args \
        _IOMDPrivateState * state

#define IOMemoryDescriptor_CreateMapping_Args \
        uint64_t options, \
        uint64_t address, \
        uint64_t offset, \
        uint64_t length, \
        uint64_t alignment, \
        IOMemoryMap ** map

#define IOMemoryDescriptor_CreateSubMemoryDescriptor_Args \
        uint64_t memoryDescriptorCreateOptions, \
        uint64_t offset, \
        uint64_t length, \
        IOMemoryDescriptor * ofDescriptor, \
        IOMemoryDescriptor ** memory

#define IOMemoryDescriptor_CreateWithMemoryDescriptors_Args \
        uint64_t memoryDescriptorCreateOptions, \
        uint32_t withDescriptorsCount, \
        IOMemoryDescriptor ** const withDescriptors, \
        IOMemoryDescriptor ** memory

#define IOMemoryDescriptor_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOMemoryDescriptor * self, const IORPC rpc);\
\
    kern_return_t\
    _CopyState(\
        _IOMDPrivateState * state,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetLength(\
        uint64_t * returnLength);\
\
    kern_return_t\
    CreateMapping(\
        uint64_t options,\
        uint64_t address,\
        uint64_t offset,\
        uint64_t length,\
        uint64_t alignment,\
        IOMemoryMap ** map,\
        OSDispatchMethod supermethod = NULL);\
\
    static kern_return_t\
    CreateSubMemoryDescriptor(\
        uint64_t memoryDescriptorCreateOptions,\
        uint64_t offset,\
        uint64_t length,\
        IOMemoryDescriptor * ofDescriptor,\
        IOMemoryDescriptor ** memory) __attribute__((availability(driverkit,introduced=20.0)));\
\
    static kern_return_t\
    CreateWithMemoryDescriptors(\
        uint64_t memoryDescriptorCreateOptions,\
        uint32_t withDescriptorsCount,\
        IOMemoryDescriptor ** const withDescriptors,\
        IOMemoryDescriptor ** memory) __attribute__((availability(driverkit,introduced=20.0)));\
\
    kern_return_t\
    Map(\
        uint64_t options,\
        uint64_t address,\
        uint64_t length,\
        uint64_t alignment,\
        uint64_t * returnAddress,\
        uint64_t * returnLength);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*_CopyState_Handler)(OSMetaClassBase * target, IOMemoryDescriptor__CopyState_Args);\
    static kern_return_t\
    _CopyState_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _CopyState_Handler func);\
\
    typedef kern_return_t (*CreateMapping_Handler)(OSMetaClassBase * target, IOMemoryDescriptor_CreateMapping_Args);\
    static kern_return_t\
    CreateMapping_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CreateMapping_Handler func);\
\
    typedef kern_return_t (*CreateSubMemoryDescriptor_Handler)(IOMemoryDescriptor_CreateSubMemoryDescriptor_Args);\
    static kern_return_t\
    CreateSubMemoryDescriptor_Invoke(const IORPC rpc,\
        CreateSubMemoryDescriptor_Handler func);\
\
    typedef kern_return_t (*CreateWithMemoryDescriptors_Handler)(IOMemoryDescriptor_CreateWithMemoryDescriptors_Args);\
    static kern_return_t\
    CreateWithMemoryDescriptors_Invoke(const IORPC rpc,\
        CreateWithMemoryDescriptors_Handler func);\
\


#define IOMemoryDescriptor_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    _CopyState_Impl(IOMemoryDescriptor__CopyState_Args);\
\
    kern_return_t\
    CreateMapping_Impl(IOMemoryDescriptor_CreateMapping_Args);\
\
    static kern_return_t\
    CreateSubMemoryDescriptor_Impl(IOMemoryDescriptor_CreateSubMemoryDescriptor_Args);\
\
    static kern_return_t\
    CreateWithMemoryDescriptors_Impl(IOMemoryDescriptor_CreateWithMemoryDescriptors_Args);\
\


#define IOMemoryDescriptor_VirtualMethods \
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


/* IOMemoryDescriptor.iig:200- */



#endif /* ! _IOKIT_UIOMEMORYDESCRIPTOR_H */
