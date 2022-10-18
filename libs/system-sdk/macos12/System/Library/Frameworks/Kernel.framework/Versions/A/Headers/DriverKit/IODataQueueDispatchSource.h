/* iig(DriverKit-191.30.2) generated from IODataQueueDispatchSource.iig */

/* IODataQueueDispatchSource.iig:1-37 */
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

#ifndef _IOKIT_UIODATAQUEUEDISPATCHSOURCE_H
#define _IOKIT_UIODATAQUEUEDISPATCHSOURCE_H

#include <DriverKit/IODispatchQueue.h>  /* .iig include */
#include <DriverKit/IOMemoryDescriptor.h>  /* .iig include */

typedef void (^IODataQueueClientEnqueueEntryBlock)(void *data, size_t dataSize);
typedef void (^IODataQueueClientDequeueEntryBlock)(const void *data, size_t dataSize);

/* source class IODataQueueDispatchSource IODataQueueDispatchSource.iig:38-207 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

class NATIVE KERNEL IODataQueueDispatchSource : public IODispatchSource
{
public:

    /*!
     * @brief       Create an IODataQueueDispatchSource for a shared memory data queue.
     * @param       queueByteCount The size of the queue in bytes.
     * @param       queue IODispatchQueue the source is attached to. Note that the DataAvailable
     *              and DataServiced handlers are invoked on the queue set for the target method
     *              of the OSAction, not this queue.
     * @param       source Created source with +1 retain count to be released by the caller.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	static kern_return_t
	Create(
		uint64_t queueByteCount,
	    IODispatchQueue * queue,
	    IODataQueueDispatchSource ** source);

	virtual bool
	init() override;

	virtual void
	free() override;

    /*!
     * @brief       As a consumer, set the handler block to run when the queue becomes non-empty.
     * @param       action OSAction instance specifying the callback method. The OSAction object will be retained
     *              until SetHandler is called again or the event source is cancelled.
     *              The DataAvailable handler is invoked on the queue set for the target method of the OSAction.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	SetDataAvailableHandler(
	OSAction * action TYPE(DataAvailable));

    /*!
     * @brief       As a producer, set the handler block to run when the queue becomes non-full, after an attempt
     *              to enqueue data failed.
     * @param       action OSAction instance specifying the callback method. The OSAction object will be retained
     *              until SetHandler is called again or the event source is cancelled.
     *              The DataServiced handler is invoked on the queue set for the target method of the OSAction.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	SetDataServicedHandler(
	OSAction * action TYPE(DataServiced));

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


    /*!
     * @brief       As a consumer, check if the data queue is non-empty.
     * @return      True if the queue is non-empty.
     */
	bool
	IsDataAvailable(void) LOCALONLY;

    /*!
     * @brief       As a consumer, get access to the next queue entry without dequeuing it.
     * @param       callback to invoked if the queue is non-empty with the next entry to be dequeued.
     * @return      kIOReturnSuccess if the callback was invoked.
     *              kIOReturnUnderrun if the queue was empty.
     *              kIOReturnError if the queue was corrupt.
     */
	kern_return_t
	Peek(IODataQueueClientDequeueEntryBlock callback) LOCALONLY;

    /*!
     * @brief       As a consumer, dequeue the next queue entry.
     * @param       callback invoked if the queue was non-empty with the entry that was dequeued.
     * @return      kIOReturnSuccess if the callback was invoked.
     *              kIOReturnUnderrun if the queue was empty.
     *              kIOReturnError if the queue was corrupt.
     */
	kern_return_t
	Dequeue(IODataQueueClientDequeueEntryBlock callback) LOCALONLY;

    /*!
     * @brief       As a producer, enqueue a queue entry.
     * @param       dataSize size of the data to enqueue.
     * @param       callback invoked if the queue has enough space to enqueue the data.
     * @return      kIOReturnSuccess if the callback was invoked.
     *              kIOReturnOverrun if the queue was full.
     *              kIOReturnError if the queue was corrupt.
     */
	kern_return_t
	Enqueue(uint32_t dataSize, IODataQueueClientEnqueueEntryBlock callback) LOCALONLY;

    /*!
     * @brief       As a consumer, dequeue the next queue entry, but don't send any DataServiced notification.
     * @param       sendDataServiced Flag that indicates a DataServiced notification would have sent.
     *              It should be initialized to false before a series of calls to this method,
     *              and if true after those calls, the notification sent with SendDataServiced().
     * @param       callback invoked if the queue was non-empty with the entry that was dequeued.
     * @return      kIOReturnSuccess if the callback was invoked.
     *              kIOReturnUnderrun if the queue was empty.
     *              kIOReturnError if the queue was corrupt.
     */
	kern_return_t
	DequeueWithCoalesce(bool * sendDataServiced, IODataQueueClientDequeueEntryBlock callback) LOCALONLY;

    /*!
     * @brief       As a producer, enqueue a queue entry, but don't send any DataAvailable notification.
     * @param       dataSize size of the data to enqueue
     * @param       sendDataAvailable Flag that indicates a DataAvailable notification would have been sent.
     *              It should be initialized to false before a series of calls to this method,
     *              and if true after those calls, the notification sent with SendDataAvailable().
     * @param       callback invoked if the queue has enough space to enqueue the data.
     * @return      kIOReturnSuccess if the callback was invoked.
     *              kIOReturnOverrun if the queue was full.
     *              kIOReturnError if the queue was corrupt.
     */
	kern_return_t
	EnqueueWithCoalesce(uint32_t dataSize,  bool * sendDataAvailable, IODataQueueClientEnqueueEntryBlock callback) LOCALONLY;

    /*!
     * @brief       As a consumer, send the DataServiced notification indicated by DequeueWithCoalesce.
	 */
	void
	SendDataServiced(void) LOCALONLY;

    /*!
     * @brief       As a producer, send the DataAvailable notification indicated by EnqueueWithCoalesce.
	 */
	void
	SendDataAvailable(void) LOCALONLY;

private:
	virtual kern_return_t
	CopyMemory(
	IOMemoryDescriptor ** memory);

	virtual kern_return_t
	CopyDataAvailableHandler(
	OSAction ** action);

	virtual kern_return_t
	CopyDataServicedHandler(
	OSAction ** action);

	virtual kern_return_t
	CheckForWork(bool synchronous) override LOCAL;

	virtual void
	DataAvailable(
		OSAction * action TARGET) LOCAL = 0;

	virtual void
	DataServiced(
		OSAction * action TARGET) LOCAL = 0;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IODataQueueDispatchSource IODataQueueDispatchSource.iig:38-207 */

#define IODataQueueDispatchSource_Create_ID            0xe8544306a54d09e0ULL
#define IODataQueueDispatchSource_SetDataAvailableHandler_ID            0xd2c1d8cc6ec3a591ULL
#define IODataQueueDispatchSource_SetDataServicedHandler_ID            0xd0aac29bba67b644ULL
#define IODataQueueDispatchSource_CopyMemory_ID            0x9be617ec7d8cd728ULL
#define IODataQueueDispatchSource_CopyDataAvailableHandler_ID            0xc856b17471f65d99ULL
#define IODataQueueDispatchSource_CopyDataServicedHandler_ID            0xd8759c8da406b3fcULL
#define IODataQueueDispatchSource_DataAvailable_ID            0xf799c876baf566f3ULL
#define IODataQueueDispatchSource_DataServiced_ID            0x8daa79f78047d8d8ULL

#define IODataQueueDispatchSource_Create_Args \
        uint64_t queueByteCount, \
        IODispatchQueue * queue, \
        IODataQueueDispatchSource ** source

#define IODataQueueDispatchSource_SetDataAvailableHandler_Args \
        OSAction * action

#define IODataQueueDispatchSource_SetDataServicedHandler_Args \
        OSAction * action

#define IODataQueueDispatchSource_SetEnableWithCompletion_Args \
        bool enable, \
        IODispatchSourceCancelHandler handler

#define IODataQueueDispatchSource_Cancel_Args \
        IODispatchSourceCancelHandler handler

#define IODataQueueDispatchSource_CopyMemory_Args \
        IOMemoryDescriptor ** memory

#define IODataQueueDispatchSource_CopyDataAvailableHandler_Args \
        OSAction ** action

#define IODataQueueDispatchSource_CopyDataServicedHandler_Args \
        OSAction ** action

#define IODataQueueDispatchSource_CheckForWork_Args \
        const IORPC rpc, \
        bool synchronous

#define IODataQueueDispatchSource_DataAvailable_Args \
        OSAction * action

#define IODataQueueDispatchSource_DataServiced_Args \
        OSAction * action

#define IODataQueueDispatchSource_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IODataQueueDispatchSource * self, const IORPC rpc);\
\
    static kern_return_t\
    Create(\
        uint64_t queueByteCount,\
        IODispatchQueue * queue,\
        IODataQueueDispatchSource ** source);\
\
    kern_return_t\
    SetDataAvailableHandler(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetDataServicedHandler(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    bool\
    IsDataAvailable(\
);\
\
    kern_return_t\
    Peek(\
        IODataQueueClientDequeueEntryBlock callback);\
\
    kern_return_t\
    Dequeue(\
        IODataQueueClientDequeueEntryBlock callback);\
\
    kern_return_t\
    Enqueue(\
        uint32_t dataSize,\
        IODataQueueClientEnqueueEntryBlock callback);\
\
    kern_return_t\
    DequeueWithCoalesce(\
        bool * sendDataServiced,\
        IODataQueueClientDequeueEntryBlock callback);\
\
    kern_return_t\
    EnqueueWithCoalesce(\
        uint32_t dataSize,\
        bool * sendDataAvailable,\
        IODataQueueClientEnqueueEntryBlock callback);\
\
    void\
    SendDataServiced(\
);\
\
    void\
    SendDataAvailable(\
);\
\
    kern_return_t\
    CopyMemory(\
        IOMemoryDescriptor ** memory,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyDataAvailableHandler(\
        OSAction ** action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyDataServicedHandler(\
        OSAction ** action,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    DataAvailable(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    DataServiced(\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
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
    typedef kern_return_t (*Create_Handler)(IODataQueueDispatchSource_Create_Args);\
    static kern_return_t\
    Create_Invoke(const IORPC rpc,\
        Create_Handler func);\
\
    typedef kern_return_t (*SetDataAvailableHandler_Handler)(OSMetaClassBase * target, IODataQueueDispatchSource_SetDataAvailableHandler_Args);\
    static kern_return_t\
    SetDataAvailableHandler_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetDataAvailableHandler_Handler func);\
\
    typedef kern_return_t (*SetDataServicedHandler_Handler)(OSMetaClassBase * target, IODataQueueDispatchSource_SetDataServicedHandler_Args);\
    static kern_return_t\
    SetDataServicedHandler_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetDataServicedHandler_Handler func);\
\
    typedef kern_return_t (*CopyMemory_Handler)(OSMetaClassBase * target, IODataQueueDispatchSource_CopyMemory_Args);\
    static kern_return_t\
    CopyMemory_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyMemory_Handler func);\
\
    typedef kern_return_t (*CopyDataAvailableHandler_Handler)(OSMetaClassBase * target, IODataQueueDispatchSource_CopyDataAvailableHandler_Args);\
    static kern_return_t\
    CopyDataAvailableHandler_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyDataAvailableHandler_Handler func);\
\
    typedef kern_return_t (*CopyDataServicedHandler_Handler)(OSMetaClassBase * target, IODataQueueDispatchSource_CopyDataServicedHandler_Args);\
    static kern_return_t\
    CopyDataServicedHandler_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyDataServicedHandler_Handler func);\
\
    typedef void (*DataAvailable_Handler)(OSMetaClassBase * target, IODataQueueDispatchSource_DataAvailable_Args);\
    static kern_return_t\
    DataAvailable_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        DataAvailable_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    DataAvailable_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        DataAvailable_Handler func);\
\
    typedef void (*DataServiced_Handler)(OSMetaClassBase * target, IODataQueueDispatchSource_DataServiced_Args);\
    static kern_return_t\
    DataServiced_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        DataServiced_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    DataServiced_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        DataServiced_Handler func);\
\


#define IODataQueueDispatchSource_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    static kern_return_t\
    Create_Impl(IODataQueueDispatchSource_Create_Args);\
\
    kern_return_t\
    SetDataAvailableHandler_Impl(IODataQueueDispatchSource_SetDataAvailableHandler_Args);\
\
    kern_return_t\
    SetDataServicedHandler_Impl(IODataQueueDispatchSource_SetDataServicedHandler_Args);\
\
    kern_return_t\
    CopyMemory_Impl(IODataQueueDispatchSource_CopyMemory_Args);\
\
    kern_return_t\
    CopyDataAvailableHandler_Impl(IODataQueueDispatchSource_CopyDataAvailableHandler_Args);\
\
    kern_return_t\
    CopyDataServicedHandler_Impl(IODataQueueDispatchSource_CopyDataServicedHandler_Args);\
\


#define IODataQueueDispatchSource_VirtualMethods \
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



class IODataQueueDispatchSourceInterface : public OSInterface
{
public:
};

struct IODataQueueDispatchSource_IVars;
struct IODataQueueDispatchSource_LocalIVars;

class IODataQueueDispatchSource : public IODispatchSource, public IODataQueueDispatchSourceInterface
{
    OSDeclareDefaultStructorsWithDispatch(IODataQueueDispatchSource);


public:
#ifdef IODataQueueDispatchSource_DECLARE_IVARS
IODataQueueDispatchSource_DECLARE_IVARS
#else /* IODataQueueDispatchSource_DECLARE_IVARS */
    union
    {
        IODataQueueDispatchSource_IVars * ivars;
        IODataQueueDispatchSource_LocalIVars * lvars;
    };
#endif /* IODataQueueDispatchSource_DECLARE_IVARS */

    using super = IODispatchSource;


    IODataQueueDispatchSource_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */

/* IODataQueueDispatchSource.iig:209- */

#endif /* ! _IOKIT_UIODATAQUEUEDISPATCHSOURCE_H */
