/* iig(DriverKit-191.30.2) generated from IOServiceStateNotificationDispatchSource.iig */

/* IOServiceStateNotificationDispatchSource.iig:1-36 */
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

#ifndef _IOKIT_UIOSERVICESTATEDISPATCHSOURCE_H
#define _IOKIT_UIOSERVICESTATEDISPATCHSOURCE_H

#include <DriverKit/IODispatchQueue.h>  /* .iig include */
#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */


/* source class IOServiceStateNotificationDispatchSource IOServiceStateNotificationDispatchSource.iig:37-106 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

class NATIVE KERNEL IOServiceStateNotificationDispatchSource : public IODispatchSource
{
public:

	virtual bool
	init() override;

	virtual void
	free() override;

    /*!
     * @brief       Create an IOServiceStateNotificationDispatchSource for notification of IOService state events sent by the StateNotificationSet() api.
     * @param       service The object hosting state, typically returned by IOService::CopySystemStateNotificationService().
     * @param       items Array of state item names to be notified about.
     * @param       queue IODispatchQueue the source is attached to. Note that the StateNotificationReady
     *              handler is invoked on the queue set for the target method
     *              of the OSAction, not this queue.
     * @param       source Created source with +1 retain count to be released by the caller.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	static kern_return_t
	Create(IOService * service, OSArray * items, IODispatchQueue * queue, IOServiceStateNotificationDispatchSource ** source) LOCAL;


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
     *              The StateNotificationReady handler is invoked on the queue set for the target method of the
     *              OSAction.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	SetHandler(
	OSAction * action TYPE(StateNotificationReady));

    /*!
     * @brief       Rearm the notification.
     * @discussion  Call this method in the implementation of your StateNotificationReady action
     *              before reading any item values.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	StateNotificationBegin(void);

private:
	virtual void
	StateNotificationReady(
		OSAction  * action TARGET) LOCAL = 0;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOServiceStateNotificationDispatchSource IOServiceStateNotificationDispatchSource.iig:37-106 */

#define IOServiceStateNotificationDispatchSource_Create_ID            0xddfc03b5eb685059ULL
#define IOServiceStateNotificationDispatchSource_SetHandler_ID            0x91981670bc7b5188ULL
#define IOServiceStateNotificationDispatchSource_StateNotificationBegin_ID            0xe0940d907a5fc5dcULL
#define IOServiceStateNotificationDispatchSource_StateNotificationReady_ID            0xda6f23bbb0b49076ULL

#define IOServiceStateNotificationDispatchSource_Create_Args \
        IOService * service, \
        OSArray * items, \
        IODispatchQueue * queue, \
        IOServiceStateNotificationDispatchSource ** source

#define IOServiceStateNotificationDispatchSource_SetEnableWithCompletion_Args \
        bool enable, \
        IODispatchSourceCancelHandler handler

#define IOServiceStateNotificationDispatchSource_Cancel_Args \
        IODispatchSourceCancelHandler handler

#define IOServiceStateNotificationDispatchSource_SetHandler_Args \
        OSAction * action

#define IOServiceStateNotificationDispatchSource_StateNotificationBegin_Args \


#define IOServiceStateNotificationDispatchSource_StateNotificationReady_Args \
        OSAction * action

#define IOServiceStateNotificationDispatchSource_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOServiceStateNotificationDispatchSource * self, const IORPC rpc);\
\
    static kern_return_t\
    Create(\
        IOService * service,\
        OSArray * items,\
        IODispatchQueue * queue,\
        IOServiceStateNotificationDispatchSource ** source);\
\
    kern_return_t\
    SetHandler(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    StateNotificationBegin(\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    StateNotificationReady(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Call(IOServiceStateNotificationDispatchSource_Create_Args);\
\
    kern_return_t\
    SetEnableWithCompletion_Impl(IODispatchSource_SetEnableWithCompletion_Args);\
\
    kern_return_t\
    Cancel_Impl(IODispatchSource_Cancel_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*Create_Handler)(IOServiceStateNotificationDispatchSource_Create_Args);\
    static kern_return_t\
    Create_Invoke(const IORPC rpc,\
        Create_Handler func);\
\
    typedef kern_return_t (*SetHandler_Handler)(OSMetaClassBase * target, IOServiceStateNotificationDispatchSource_SetHandler_Args);\
    static kern_return_t\
    SetHandler_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetHandler_Handler func);\
\
    typedef kern_return_t (*StateNotificationBegin_Handler)(OSMetaClassBase * targetIOServiceStateNotificationDispatchSource_StateNotificationBegin_Args);\
    static kern_return_t\
    StateNotificationBegin_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        StateNotificationBegin_Handler func);\
\
    typedef void (*StateNotificationReady_Handler)(OSMetaClassBase * target, IOServiceStateNotificationDispatchSource_StateNotificationReady_Args);\
    static kern_return_t\
    StateNotificationReady_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        StateNotificationReady_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    StateNotificationReady_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        StateNotificationReady_Handler func);\
\


#define IOServiceStateNotificationDispatchSource_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Impl(IOServiceStateNotificationDispatchSource_Create_Args);\
\
    kern_return_t\
    SetHandler_Impl(IOServiceStateNotificationDispatchSource_SetHandler_Args);\
\
    kern_return_t\
    StateNotificationBegin_Impl(IOServiceStateNotificationDispatchSource_StateNotificationBegin_Args);\
\


#define IOServiceStateNotificationDispatchSource_VirtualMethods \
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



class IOServiceStateNotificationDispatchSourceInterface : public OSInterface
{
public:
};

struct IOServiceStateNotificationDispatchSource_IVars;
struct IOServiceStateNotificationDispatchSource_LocalIVars;

class IOServiceStateNotificationDispatchSource : public IODispatchSource, public IOServiceStateNotificationDispatchSourceInterface
{
    OSDeclareDefaultStructorsWithDispatch(IOServiceStateNotificationDispatchSource);


public:
#ifdef IOServiceStateNotificationDispatchSource_DECLARE_IVARS
IOServiceStateNotificationDispatchSource_DECLARE_IVARS
#else /* IOServiceStateNotificationDispatchSource_DECLARE_IVARS */
    union
    {
        IOServiceStateNotificationDispatchSource_IVars * ivars;
        IOServiceStateNotificationDispatchSource_LocalIVars * lvars;
    };
#endif /* IOServiceStateNotificationDispatchSource_DECLARE_IVARS */

    using super = IODispatchSource;


    IOServiceStateNotificationDispatchSource_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */

/* IOServiceStateNotificationDispatchSource.iig:108- */

#endif /* ! _IOKIT_UIOSERVICESTATEDISPATCHSOURCE_H */
