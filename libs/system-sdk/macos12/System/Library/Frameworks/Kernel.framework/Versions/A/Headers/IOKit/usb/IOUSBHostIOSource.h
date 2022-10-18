/*
 * Copyright (c) 1998-2016 Apple Inc. All rights reserved.
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

/*!
 @header     IOUSBHostIOSource.h
 @brief      IOUSBHostIOSource is the base class for objects that perform USB IO.
 */

#ifndef IOUSBHostFamily_IOUSBHostIOSource_h
#define IOUSBHostFamily_IOUSBHostIOSource_h

#include <sys/queue.h>
#include <libkern/c++/OSObject.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/usb/IOUSBHostFamily.h>

#define kUSBHostTransferLimit (1024)

class AppleUSBHostController;
class IOUSBHostDevice;
class AppleUSBRequestPool;

typedef void (*IOUSBHostCompletionAction)(void* owner, void* parameter, IOReturn status, uint32_t bytesTransferred);

/*!
 * @struct      IOUSBHostCompletion
 * @discussion  Struture describing the completion callback for an asynchronous bulk, control, or interrupt IO operation
 * @field       owner Pointer to an object that owns the transfer.  May be used as <code>this</code> for an action passed via OSMemberFunctionCast.
 * @field       action IOUSBHostCompletionAction to run when the IO request completes.
 * @field       parameter Pointer to be used as context within the completion action.
 */
struct IOUSBHostCompletion
{
    void* owner;
    IOUSBHostCompletionAction action;
    void* parameter;
};

typedef void (*IOUSBHostBundledCompletionAction)(void *owner, uint32_t ioCompletionCount, IOMemoryDescriptor** dataBufferArray, void** parameter, IOReturn* statusArray, uint32_t* actualByteCountArray);

/*!
 * @struct      IOUSBHostBundledCompletion
 * @discussion  Struture describing the completion callback for an asynchronous bulk, control, or interrupt IO operation
 * @field       owner Pointer to an object that owns the transfer.  May be used as <code>this</code> for an action passed via OSMemberFunctionCast.
 * @field       action IOUSBHostBundledCompletion to run when the IO request completes.
 * @field       parameter Pointer to be used as context within the completion action.
 */
struct IOUSBHostBundledCompletion
{
    void* owner;
    IOUSBHostBundledCompletionAction action;
    void* parameter;
};



/*!
 * @struct      IOUSBHostIsochronousFrame
 * @discussion  Structure representing a single frame in an isochronous transfer.  Use of this structure is discouraged, use @link IOUSBHostIsochronousTransaction @/link instead.
 * @field       status Completion status for this individual frame.  IOUSBHostFamily will initialize this to kIOReturnInvalid and will update the field with a valid status code upon completion of the frame.
 * @field       requestCount The number of bytes requested to transfer for this frame.  This field must be initialized by the caller before this structure is submitted to IOUSBHostFamily.
 * @field       completeCount The number of bytes actually transferred for this frame.  IOUSBHostFamily will update this field upon completion of the frame.
 * @field       reserved Reserved for future use.
 * @field       timeStamp The observed AbsoluteTime for this frame's completion.  Note that interrupt latency and system load may result in more than one frame completing with the same timestamp.
 */
struct IOUSBHostIsochronousFrame
{
    IOReturn     status;
    uint32_t     requestCount;
    uint32_t     completeCount;
    uint32_t     reserved;
    AbsoluteTime timeStamp;
} __attribute__((packed));

/*!
 * @struct      IOUSBHostIsochronousTransaction
 * @discussion  Structure representing a single frame or microframe in an isochronous transfer.
 * @field       status Completion status for this individual transaction.  IOUSBHostFamily will initialize this to kIOReturnInvalid and will update the field with a valid status code upon completion of the transaction.
 * @field       requestCount The number of bytes requested to transfer for this transaction.  This field must be initialized by the caller before this structure is submitted to IOUSBHostFamily.
 * @field       offset The number of bytes between the start of the memory descriptor in which this transaction resides and the start of the transaction.  The offset cannot exceed 4GB.
 * @field       completeCount The number of bytes actually transferred for this transaction.  IOUSBHostFamily will update this field upon completion of the transaction.
 * @field       timeStamp The observed AbsoluteTime for this transaction's completion.  Note that interrupt latency and system load may result in more than one transaction completing with the same timestamp.
 * @field       options Flags that specify additional transaction behavior.  See @link tIsochronousTransactionOptions @/link for more details.
 */
struct IOUSBHostIsochronousTransaction
{
    IOReturn     status;
    uint32_t     requestCount;
    uint32_t     offset;
    uint32_t     completeCount;
    AbsoluteTime timeStamp;
    IOOptionBits options;
} __attribute__((packed));

typedef void (*IOUSBHostIsochronousCompletionAction)(void* owner, void* parameter, IOReturn status, IOUSBHostIsochronousFrame* frameList);

/*!
 * @struct      IOUSBHostIsochronousCompletion
 * @discussion  Struture describing the completion callback for an asynchronous isochronous operation
 * @field       owner Pointer to an object that owns the transfer.  May be used as <code>this</code> for an action passed via OSMemberFunctionCast.
 * @field       action IOUSBHostIsochronousCompletionAction to run when the IO request completes.
 * @field       parameter Pointer to be used as context within the completion action.
 */
struct IOUSBHostIsochronousCompletion
{
    void* owner;
    IOUSBHostIsochronousCompletionAction action;
    void* parameter;
};

typedef void (*IOUSBHostIsochronousTransactionCompletionAction)(void* owner, void* parameter, IOReturn status, IOUSBHostIsochronousTransaction* transactionList);

/*!
 * @struct      IOUSBHostIsochronousTransactionCompletion
 * @discussion  Structure describing the completion callback for an asynchronous isochronous operation
 * @field       owner Pointer to an object that owns the transfer.  May be used as <code>this</code> for an action passed via OSMemberFunctionCast.
 * @field       action IOUSBHostIsochronousTransactionCompletionAction to run when the IO request completes.
 * @field       parameter Pointer to be used as context within the completion action.
 */
struct IOUSBHostIsochronousTransactionCompletion
{
    void* owner;
    IOUSBHostIsochronousTransactionCompletionAction action;
    void* parameter;
};

/*!
 * @enum        tIsochronousTransferOptions
 * @brief       Options for controlling isochronous transfer behavior
 * @constant    kIsochronousTransferOptionsNone No options are selected for this transfer
 */
enum tIsochronousTransferOptions
{
    kIsochronousTransferOptionsNone = 0,
};

/*!
 * @enum        tIsochronousTransactionOptions
 * @brief       Options for <code>IOUSBHostIsochronousTransaction</code>
 * @constant    kIsochronousTransactionOptionsNone No options are selected for this transaction
 * @constant    kIsochronousTransactionOptionsWrap This transaction's data reaches the end of the memory descriptor and continues at the descriptor's start.  If this option is selected, the transaction's offset + requestCount should exceed the memory descriptor's length. This option is not supported on UHCI or UserHCI controllers.
 */
enum tIsochronousTransactionOptions
{
    kIsochronousTransactionOptionsNone = 0,
    kIsochronousTransactionOptionsWrap = (1 << 0),
};

typedef LIST_HEAD (IOUSBHostIOSourceClientRecordList, IOUSBHostIOSourceClientRecord) IOUSBHostIOSourceClientRecordList;
typedef LIST_ENTRY (IOUSBHostIOSourceClientRecord) IOUSBHostIOSourceClientRecordLink;

struct IOUSBHostIOSourceClientRecord
{
    IOService*                  forClient;
    UInt32                      outstandingIO;
    IOUSBHostIOSourceClientRecordLink link;
};

/*!
 * @struct      tPacketFilterMetadata
 * @discussion  Struture providing information about the IOUSBHostIOSource's parents
 * @field       deviceAddress Address of the parent IOUSBHostDevice.
 * @field       interfaceClass If enpointAddress is non-zero, the class of the parent IOUSBHostInteface.
 * @field       interfaceSubClass If enpointAddress is non-zero, the sub-class of the parent IOUSBHostInteface.
 * @field       interfaceProtocol If enpointAddress is non-zero, the protocol of the parent IOUSBHostInteface.
 * @field       interfaceAltSetting Current alternate setting of the parent IOUSBHostInterface.
 * @field       endpointAddress The address of the endpoint including the direction.
 * @field       endpointType See tEndpointType in IOUSBHostFamily.h.
 * @field       speed See tUSBHostConnectionSpeed in IOUSBHostFamily.h.
 * @field       vid Vendor ID of the parent IOUSBHostDevice.
 * @field       pid Product ID of the parent IOUSBHostDevice.
 * @field       locationID USB locationID of the parent IOUSBHostDevice.
 */
struct tPacketFilterMetadata
{
    uint8_t  deviceAddress;
    uint8_t  interfaceClass;
    uint8_t  interfaceSubclass;
    uint8_t  interfaceProtocol;
    uint8_t  interfaceAltSetting;
    uint8_t  endpointAddress;
    uint8_t  endpointType;
    uint8_t  speed;
    uint16_t vid;
    uint16_t pid;
    uint32_t locationID;
} __attribute__((packed));

/*!
 * @class       IOUSBHostIOSource
 * @brief       The base class for objects that perform USB IO.
 * @discussion  This class provides functionality to transfer data across USB.  Function drivers should not subclass IOUSBHostIOSource.
 */
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBHostIOSource : public OSObject
{
    friend class AppleUSBIORequest;
    friend class IOUSBHostInterface;

    OSDeclareAbstractStructors(IOUSBHostIOSource)

protected:
    virtual void free();
    
    virtual bool initWithOwners(AppleUSBHostController* controller, IOUSBHostDevice* device);

    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 0);
    virtual IOReturn open();

    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 1);
    virtual IOReturn openGated();

    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 2);
    virtual IOReturn destroy();

    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 3);
    virtual IOReturn destroyGated();

    // Pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 4);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 5);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 6);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 7);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 8);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 9);
    
public:
    /*!
     * @enum        tState
     * @namespace   IOUSBHostIOSource
     * @brief       Return value for <code>getState()</code>
     * @constant    kStateReady The I/O source is idle and fully-functional.
     * @constant    kStateRunningCompletions The I/O source is currently running completions.
     * @constant    kStateAborting The I/O source is currently aborting all requests.
     * @constant    kStateInactive The I/O source has been closed.
     */
    enum tState
    {
        kStateReady,
        kStateRunningCompletions,
        kStateAborting,
        kStateInactive,
        kStateDestroyed
    };

    /*!
     * @brief   Returns the current state of the I/O source.
     * @return  @link tState @/link
     */
    virtual tState getState();

    /*!
     * @enum        tAbortOptions
     * @brief       Options for <code>abort()</code>
     * @constant    kAbortAsynchronous abort() should return immediately without waiting for the aborted IO to complete
     * @constant    kAbortSynchronous abort() should not return until the aborted IO has completed
     */
    enum tAbortOptions
    {
        kAbortAsynchronous = 0x0,
        kAbortSynchronous  = 0x1
    };

    /*!
     * @brief       Abort pending I/O requests.
     * @discussion  This method will abort all pending I/O requests.  If <code>options</code> includes <code>kAbortSynchronous</code>, this method will block any new IO requests unless they are submitted from an aborted IO's completion routine.
     * @param       options tAbortOptions
     * @param       withError IOReturn error value to return with the requests.  The default value of kIOReturnAborted should be used.
     * @param       forClient Service for which to abort requests.  If NULL, all requests will be aborted.  Only control endpoints can specify a non-NULL value.
     * @return      IOReturn result code
     */
    virtual IOReturn abort(IOOptionBits options = kAbortAsynchronous, IOReturn withError = kIOReturnAborted, IOService* forClient = NULL);
    
    /*!
     * @brief       Retrieve a pointer to the IOUSBHostDevice that encapsulates this endpoint or stream
     * @return      IOUSBHostDevice pointer
     */
    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 10);
    virtual IOUSBHostDevice* getDevice() const;
    
    // Public pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 11);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 12);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 13);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 14);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 15);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 16);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 17);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 18);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 19);
    
protected:
    // Protected pad slots for future expansion
    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 20);
    virtual UInt32 getOutstandingIO(IOService* forClient);

    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 21);
    virtual UInt32 adjustOutstandingIO(IOService* forClient, SInt32 increment);

    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 22);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 23);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 24);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 25);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 26);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 27);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 28);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 29);
        
protected:
    struct tSynchronousParameter
    {
        uint32_t bytesTransferred;
        IOReturn status;
    };

    virtual void synchronousCompletion(void* parameter, IOReturn status, uint32_t bytesTransferred);
    virtual void synchronousIsochronousCompletion(void* parameter, IOReturn status, IOUSBHostIsochronousFrame* pFrames);
    
    virtual IOReturn getStateGated(tState& state);

    virtual IOReturn close();

    virtual IOReturn closeGated();

    virtual IOReturn abortGated(IOOptionBits options, IOReturn withError, IOService* forClient);

    virtual void timerCompletion(IOTimerEventSource* timer);

    AppleUSBHostController* _controller;
    IOUSBHostDevice*        _device;

    IOWorkLoop*              _workloop;
    IOCommandGate*           _commandGate;
    IOTimerEventSource*      _timer;
    AppleUSBRequestPool*     _requestPool;

    tState                  _state;
    UInt32                  _outstandingIO;
    UInt32                  _completionRunning;
    
#pragma mark IO
public:
    /*!
     * @brief       Enqueue an IO request on the source
     * @discussion  This method is used to issue an asynchronous I/O request on a bulk or interrupt pipe.  See IOUSBHostPipe::io and IOUSBHostStream::io for object-specific interface notes.
     * @param       dataBuffer IOMemoryDescriptor pointer containing the buffer to use for the transfer
     * @param       dataBufferLength Length of the request.  Must be <= <code>dataBuffer->getLength()</code>
     * @param       completion Pointer to a IOUSBHostCompletion structure.  This will be copied and can therefore be stack-allocated.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.  Must be 0 for interrupt pipes and streams.
     * @return      kIOReuturnSuccess if the completion will be called in the future, otherwise error
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs = 0);
    
    /*!
     * @brief       Perform an IO request on the source
     * @discussion  This method will send a synchronous request on the IO source, and will not return until the request is complete.  This method will acquire the service's workloop lock, and will call commandSleep to send the request.  See IOUSBHostPipe::io and IOUSBHostStream::io for object-specific interface notes.
     * @param       dataBuffer IOMemoryDescriptor pointer containing the buffer to use for the transfer
     * @param       dataBufferLength Length of the request.  Must be <= <code>dataBuffer->getLength()</code>
     * @param       bytesTransferred uint32_t reference which will be updated with the bytes transferred during the request
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.  Must be 0 for interrupt pipes and streams.
     * @return      IOReturn value indicating the result of the IO request
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, uint32_t& bytesTransferred, uint32_t completionTimeoutMs = 0);


    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 30);

    /*!
     * @brief       Enqueue an IO request on the source
     * @discussion  This method is used to issue an asynchronous I/O request on a bulk or interrupt pipe that will trigger a IOUSBHostBundledCompletionAction callback.
     *              I/O requests enqueued with this method may be completed in bundles that may reduce the total number of callbacks.
     *              The 'ioCompletionCount'  callback parameter indicates the number of completed I/Os associated with this callback and the size of the
     *              'dataBufferArray', 'statusArray' and' actualByteCountArray.  Clients must iterate over these arrays to extract the completion status/length
     *              of each individual IOMemoryDescriptor.
     *              See IOUSBHostPipe::io and IOUSBHostStream::io for object-specific interface notes.
     * @param       dataBuffer IOMemoryDescriptor pointer containing the buffer to use for the transfer
     * @param       dataBufferLength Length of the request.  Must be <= <code>dataBuffer->getLength()</code>
     * @param       completion Pointer to a IOUSBHostBundledCompletion structure.  This will be copied and can therefore be stack-allocated.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.  Must be 0 for interrupt pipes and streams.
     * @return      kIOReuturnSuccess if the completion will be called in the future, otherwise error
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, IOUSBHostBundledCompletion* completion, uint32_t completionTimeoutMs = 0);
    
    


    // Public pad slots for IO
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 31);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 32);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 33);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 34);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 35);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 36);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 37);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 38);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 39);

protected:
    // Protected pad slots for IO
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 40);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 41);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 42);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 43);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 44);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 45);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 46);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 47);
    OSMetaClassDeclareReservedUnused(IOUSBHostIOSource, 48);
    
protected:
    struct tInternalDataTransferParameters
    {
        IOMemoryDescriptor*                        dataBuffer;
        uint32_t                                   dataBufferLength;
        uint32_t*                                  bytesTransferred;
        IOUSBHostCompletion*                       completion;
        IOUSBHostBundledCompletion*                bundledCompletion;
        uint32_t                                   completionTimeoutMs;
        IOUSBHostIsochronousTransaction*           transactionList;
        uint32_t                                   transactionListCount;
        uint64_t                                   firstFrameNumber;
        IOUSBHostIsochronousTransactionCompletion* isochronousCompletion;
        IOUSBHostIsochronousFrame*                 frameList;
    };
    
    OSMetaClassDeclareReservedUsed(IOUSBHostIOSource, 49);
    virtual IOReturn ioBundledGated(tInternalDataTransferParameters& parameters);
    
    virtual IOReturn ioGated(tInternalDataTransferParameters& parameters);

    struct tExpansionData
    {
        IOUSBHostIOSourceClientRecordList _ioRecordList;
        tPacketFilterMetadata             _metadata;
        void*                             _dkMemoryDescriptorRing;
        IOCommandGate*                    _controllerCommandGate;
        uint32_t                          _ioSourceSignature;
    };
    
    tExpansionData* _expansionData;
    
#pragma mark Debug
    uint32_t                _debugLoggingMask;
};

#endif
