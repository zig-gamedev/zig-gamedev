/* iig(DriverKit-191.30.2) generated from IOServiceNotificationDispatchSource.iig */

/* IOServiceNotificationDispatchSource.iig:1-45 */
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

#ifndef _IOKIT_UIOSERVICEDISPATCHSOURCE_H
#define _IOKIT_UIOSERVICEDISPATCHSOURCE_H

#include <DriverKit/IODispatchQueue.h>  /* .iig include */
#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */


typedef void (^IOServiceNotificationBlock)(uint64_t type, IOService * service, uint64_t options);

enum {
	kIOServiceNotificationTypeTerminated = 0x00000000,
	kIOServiceNotificationTypeMatched    = 0x00000001,
	kIOServiceNotificationTypeLast       = kIOServiceNotificationTypeMatched,
	kIOServiceNotificationTypeNone       = 0xFFFFFFFF,
};

/* source class IOServiceNotificationDispatchSource IOServiceNotificationDispatchSource.iig:46-128 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

class NATIVE KERNEL IOServiceNotificationDispatchSource : public IODispatchSource
{
public:

    /*!
     * @brief       Create an IOServiceNotificationDispatchSource for IOService matching and termination events.
     * @param       matching An IOService matching dictionary.
     * @param       options None defined, pass zero.
     * @param       queue IODispatchQueue the source is attached to. Note that the ServiceNotificationReady
     *              handler is invoked on the queue set for the target method
     *              of the OSAction, not this queue.
     * @param       source Created source with +1 retain count to be released by the caller.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	static kern_return_t
	Create(
		OSDictionary	  *	matching,
		uint64_t            options,
	    IODispatchQueue   * queue,
		IOServiceNotificationDispatchSource ** notification) LOCAL;

	virtual bool
	init() override;

	virtual void
	free() override;

    /*!
     * @brief       Control the enable state of the notification.
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

    /*!
     * @brief       Set the handler block to run when the notification has become ready.
     * @param       action OSAction instance specifying the callback method. The OSAction object will be retained
     *              until SetHandler is called again or the event source is cancelled.
     *              The ServiceNotificationReady handler is invoked on the queue set for the target method of the
     *              OSAction.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	SetHandler(
	OSAction * action TYPE(ServiceNotificationReady));

    /*!
     * @brief       Invoke a block for each notification available in response to ServiceNotificationReady.
     * @discussion  The IOService object passed to the notification is only retained for the duration of the block.
     *              It should be retained by the block code if used beyond the invocation.
     * @param       block to be invoked with each notification
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	kern_return_t
	DeliverNotifications(IOServiceNotificationBlock block) LOCALONLY;

private:
	virtual kern_return_t
	CheckForWork(bool synchronous) override LOCAL;

	virtual void
	ServiceNotificationReady(
		OSAction  * action TARGET) LOCAL = 0;

	virtual kern_return_t
	CopyNextNotification(
		uint64_t   * type,
		IOService ** service,
		uint64_t   * options);
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOServiceNotificationDispatchSource IOServiceNotificationDispatchSource.iig:46-128 */

#define IOServiceNotificationDispatchSource_Create_ID            0xb40d274e974e946aULL
#define IOServiceNotificationDispatchSource_SetHandler_ID            0xca55c48f95fc6f8eULL
#define IOServiceNotificationDispatchSource_ServiceNotificationReady_ID            0xfbaa85f2cdb990b4ULL
#define IOServiceNotificationDispatchSource_CopyNextNotification_ID            0xca420abbda782780ULL

#define IOServiceNotificationDispatchSource_Create_Args \
        OSDictionary * matching, \
        uint64_t options, \
        IODispatchQueue * queue, \
        IOServiceNotificationDispatchSource ** notification

#define IOServiceNotificationDispatchSource_SetEnableWithCompletion_Args \
        bool enable, \
        IODispatchSourceCancelHandler handler

#define IOServiceNotificationDispatchSource_Cancel_Args \
        IODispatchSourceCancelHandler handler

#define IOServiceNotificationDispatchSource_SetHandler_Args \
        OSAction * action

#define IOServiceNotificationDispatchSource_CheckForWork_Args \
        const IORPC rpc, \
        bool synchronous

#define IOServiceNotificationDispatchSource_ServiceNotificationReady_Args \
        OSAction * action

#define IOServiceNotificationDispatchSource_CopyNextNotification_Args \
        uint64_t * type, \
        IOService ** service, \
        uint64_t * options

#define IOServiceNotificationDispatchSource_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOServiceNotificationDispatchSource * self, const IORPC rpc);\
\
    static kern_return_t\
    Create(\
        OSDictionary * matching,\
        uint64_t options,\
        IODispatchQueue * queue,\
        IOServiceNotificationDispatchSource ** notification);\
\
    kern_return_t\
    SetHandler(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    DeliverNotifications(\
        IOServiceNotificationBlock block);\
\
    void\
    ServiceNotificationReady(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyNextNotification(\
        uint64_t * type,\
        IOService ** service,\
        uint64_t * options,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Call(IOServiceNotificationDispatchSource_Create_Args);\
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
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*Create_Handler)(IOServiceNotificationDispatchSource_Create_Args);\
    static kern_return_t\
    Create_Invoke(const IORPC rpc,\
        Create_Handler func);\
\
    typedef kern_return_t (*SetHandler_Handler)(OSMetaClassBase * target, IOServiceNotificationDispatchSource_SetHandler_Args);\
    static kern_return_t\
    SetHandler_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetHandler_Handler func);\
\
    typedef void (*ServiceNotificationReady_Handler)(OSMetaClassBase * target, IOServiceNotificationDispatchSource_ServiceNotificationReady_Args);\
    static kern_return_t\
    ServiceNotificationReady_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        ServiceNotificationReady_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    ServiceNotificationReady_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        ServiceNotificationReady_Handler func);\
\
    typedef kern_return_t (*CopyNextNotification_Handler)(OSMetaClassBase * target, IOServiceNotificationDispatchSource_CopyNextNotification_Args);\
    static kern_return_t\
    CopyNextNotification_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyNextNotification_Handler func);\
\


#define IOServiceNotificationDispatchSource_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Impl(IOServiceNotificationDispatchSource_Create_Args);\
\
    kern_return_t\
    SetHandler_Impl(IOServiceNotificationDispatchSource_SetHandler_Args);\
\
    kern_return_t\
    CopyNextNotification_Impl(IOServiceNotificationDispatchSource_CopyNextNotification_Args);\
\


#define IOServiceNotificationDispatchSource_VirtualMethods \
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



class IOServiceNotificationDispatchSourceInterface : public OSInterface
{
public:
};

struct IOServiceNotificationDispatchSource_IVars;
struct IOServiceNotificationDispatchSource_LocalIVars;

class IOServiceNotificationDispatchSource : public IODispatchSource, public IOServiceNotificationDispatchSourceInterface
{
    OSDeclareDefaultStructorsWithDispatch(IOServiceNotificationDispatchSource);


public:
#ifdef IOServiceNotificationDispatchSource_DECLARE_IVARS
IOServiceNotificationDispatchSource_DECLARE_IVARS
#else /* IOServiceNotificationDispatchSource_DECLARE_IVARS */
    union
    {
        IOServiceNotificationDispatchSource_IVars * ivars;
        IOServiceNotificationDispatchSource_LocalIVars * lvars;
    };
#endif /* IOServiceNotificationDispatchSource_DECLARE_IVARS */

    using super = IODispatchSource;


    IOServiceNotificationDispatchSource_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */

/* IOServiceNotificationDispatchSource.iig:130- */

#endif /* ! _IOKIT_UIOSERVICEDISPATCHSOURCE_H */
