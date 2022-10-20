/* iig(DriverKit-191.30.2) generated from IOInterruptDispatchSource.iig */

/* IOInterruptDispatchSource.iig:1-50 */
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

#ifndef _IOKIT_UIOINTERRUPTDISPATCHSOURCE_H
#define _IOKIT_UIOINTERRUPTDISPATCHSOURCE_H

#include <DriverKit/IODispatchQueue.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */

struct IOInterruptDispatchSourcePayload {
	uint64_t time;
	uint64_t count;
};

enum {
	kIOInterruptDispatchSourceTypeEdge  = 0x00000000,
	kIOInterruptDispatchSourceTypeLevel = 0x00000001
};

enum {
	kIOInterruptSourceIndexMask      = 0x0000FFFF,
	kIOInterruptSourceAbsoluteTime   = 0x00000000,
	kIOInterruptSourceContinuousTime = 0x00010000
};

/* source class IOInterruptDispatchSource IOInterruptDispatchSource.iig:51-143 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
 * @class IOInterruptDispatchSource
 *
 * @abstract
 * IOInterruptDispatchSource delivers interrupts to a handler block on a dispatch queue.
 *
 * @discussion
 * A driver can run code in response to an interrupt from a device, specified as an IOService
 * and index. The code runs at normal thread level, but is notified with the mach_absolute_time
 * the primary interrupt fired. For IOPCIDevices, only MSI interrupt sources are supported.
 */
 
class NATIVE KERNEL IOInterruptDispatchSource : public IODispatchSource
{
public:

    /*!
     * @brief       Create an IOInterruptDispatchSource for an interrupt by index from an IOService provider.
     * @param       provider The IOService object representing the HW device producing the interrupt.
     * @param       index Index for the interrupt, optionally or'ed with one of the following constants:
                    kIOInterruptSourceContinuousTime time values sent to the InterruptOccurred() method will be in mach_continuous_time() units.
     * @param       queue Target queue to run the handler block.
     * @param       source Created source with +1 retain count to be released by the caller.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	static kern_return_t
	Create(IOService * provider,
	    uint32_t index,
	    IODispatchQueue * queue,
	    IOInterruptDispatchSource ** source) LOCAL;

    /*!
     * @brief       Returns the type of interrupt used for a device supplying hardware interrupts, by index from an IOService provider.
     * @param       provider The IOService object representing the HW device producing the interrupt.
     * @param       index Index for the interrupt.
     * @param 		interruptType The interrupt type for the interrupt source will be stored here.
     *              kIOInterruptTypeEdge will be returned for edge-trigggered sources.
     *              kIOInterruptTypeLevel will be returned for level-trigggered sources.
     *              Other flags may be returned depending on the provider, for example PCI flags for messaged interrupts.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */

	static kern_return_t
	GetInterruptType(IOService * provider,
	    uint32_t index,
	    uint64_t * interruptType);

	virtual bool
	init() override;

	virtual void
	free() override;

    /*!
     * @brief       Set the handler block to run when the interupt fires.
     * @param       action OSAction instance specifying the callback method. The OSAction object will be retained
     *              until SetHandler is called again or the event source is cancelled.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	    SetHandler(
		OSAction * action TYPE(InterruptOccurred)) LOCAL;

    /*!
     * @brief       Control the enable state of the interrupt source.
     * @param       enable Pass true to enable the source or false to disable.
     * @param       handler Optional block to be executed after the interrupt has been disabled and any pending
     *              interrupt handlers completed.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	SetEnableWithCompletion(
		bool enable,
		IODispatchSourceCancelHandler handler) override LOCAL;

    /*!
     * @brief       Cancel all callbacks from the event source.
     * @discussion  After cancellation, the source can only be freed. It cannot be reactivated.
     * @param       handler Handler block to be invoked after any callbacks have completed.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	Cancel(IODispatchSourceCancelHandler handler) override LOCAL;

private:
	virtual kern_return_t
	CheckForWork(bool synchronous) override LOCAL;

	virtual void
	InterruptOccurred(
		OSAction * action TARGET,
		uint64_t   count,
		uint64_t   time) REPLY LOCAL;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOInterruptDispatchSource IOInterruptDispatchSource.iig:51-143 */

#define IOInterruptDispatchSource_Create_ID            0xb6a948b1585fc259ULL
#define IOInterruptDispatchSource_GetInterruptType_ID            0x846d2df6b6bef33bULL
#define IOInterruptDispatchSource_SetHandler_ID            0xfcc79b0928501bb1ULL
#define IOInterruptDispatchSource_InterruptOccurred_ID            0xce0513291cfa1ee1ULL

#define IOInterruptDispatchSource_Create_Args \
        IOService * provider, \
        uint32_t index, \
        IODispatchQueue * queue, \
        IOInterruptDispatchSource ** source

#define IOInterruptDispatchSource_GetInterruptType_Args \
        IOService * provider, \
        uint32_t index, \
        uint64_t * interruptType

#define IOInterruptDispatchSource_SetHandler_Args \
        OSAction * action

#define IOInterruptDispatchSource_SetEnableWithCompletion_Args \
        bool enable, \
        IODispatchSourceCancelHandler handler

#define IOInterruptDispatchSource_Cancel_Args \
        IODispatchSourceCancelHandler handler

#define IOInterruptDispatchSource_CheckForWork_Args \
        const IORPC rpc, \
        bool synchronous

#define IOInterruptDispatchSource_InterruptOccurred_Args \
        OSAction * action, \
        uint64_t count, \
        uint64_t time

#define IOInterruptDispatchSource_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOInterruptDispatchSource * self, const IORPC rpc);\
\
    static kern_return_t\
    Create(\
        IOService * provider,\
        uint32_t index,\
        IODispatchQueue * queue,\
        IOInterruptDispatchSource ** source);\
\
    static kern_return_t\
    GetInterruptType(\
        IOService * provider,\
        uint32_t index,\
        uint64_t * interruptType);\
\
    kern_return_t\
    SetHandler(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    InterruptOccurred(\
        IORPC rpc,\
        OSAction * action,\
        uint64_t count,\
        uint64_t time,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Call(IOInterruptDispatchSource_Create_Args);\
\
    kern_return_t\
    SetHandler_Impl(IOInterruptDispatchSource_SetHandler_Args);\
\
    kern_return_t\
    SetEnableWithCompletion_Impl(IODispatchSource_SetEnableWithCompletion_Args);\
\
    kern_return_t\
    Cancel_Impl(IODispatchSource_Cancel_Args);\
\
    kern_return_t\
    CheckForWork_Impl(IODispatchSource_CheckForWork_Args);\
\
    void\
    InterruptOccurred_Impl(IOInterruptDispatchSource_InterruptOccurred_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*Create_Handler)(IOInterruptDispatchSource_Create_Args);\
    static kern_return_t\
    Create_Invoke(const IORPC rpc,\
        Create_Handler func);\
\
    typedef kern_return_t (*GetInterruptType_Handler)(IOInterruptDispatchSource_GetInterruptType_Args);\
    static kern_return_t\
    GetInterruptType_Invoke(const IORPC rpc,\
        GetInterruptType_Handler func);\
\
    typedef kern_return_t (*SetHandler_Handler)(OSMetaClassBase * target, IOInterruptDispatchSource_SetHandler_Args);\
    static kern_return_t\
    SetHandler_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetHandler_Handler func);\
\
    typedef void (*InterruptOccurred_Handler)(OSMetaClassBase * target, IOInterruptDispatchSource_InterruptOccurred_Args);\
    static kern_return_t\
    InterruptOccurred_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        InterruptOccurred_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    InterruptOccurred_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        InterruptOccurred_Handler func);\
\


#define IOInterruptDispatchSource_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Impl(IOInterruptDispatchSource_Create_Args);\
\
    static kern_return_t\
    GetInterruptType_Impl(IOInterruptDispatchSource_GetInterruptType_Args);\
\


#define IOInterruptDispatchSource_VirtualMethods \
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



class IOInterruptDispatchSourceInterface : public OSInterface
{
public:
};

struct IOInterruptDispatchSource_IVars;
struct IOInterruptDispatchSource_LocalIVars;

class IOInterruptDispatchSource : public IODispatchSource, public IOInterruptDispatchSourceInterface
{
    OSDeclareDefaultStructorsWithDispatch(IOInterruptDispatchSource);


public:
#ifdef IOInterruptDispatchSource_DECLARE_IVARS
IOInterruptDispatchSource_DECLARE_IVARS
#else /* IOInterruptDispatchSource_DECLARE_IVARS */
    union
    {
        IOInterruptDispatchSource_IVars * ivars;
        IOInterruptDispatchSource_LocalIVars * lvars;
    };
#endif /* IOInterruptDispatchSource_DECLARE_IVARS */

    using super = IODispatchSource;


    IOInterruptDispatchSource_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */

/* IOInterruptDispatchSource.iig:145- */

#endif /* ! _IOKIT_UIOINTERRUPTDISPATCHSOURCE_H */
