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
 @header     IOUSBHostPipe.h
 @brief      IOUSBHostPipe is an object representing a USB endpoint.
 @discussion
 
 <h3>IOUSBPipe Migration</h3>
 
 IOUSBHostPipe serves as a replacement for IOUSBPipe.  Clients that previously used an IOUSBPipe can use the following guide to convert to IOUSBHostPipe.
 
 <code>virtual IOReturn IOUSBPipe::Abort(IOOptionBits, IOReturn);</code><br>
 Replacement: <code>abort(...);</code>
 
 <code>virtual IOReturn IOUSBPipe::Reset();<br>
 virtual IOReturn IOUSBPipe::ClearStall();</code><br>
 Replacement: <code>clearStall(false)</code>
 
 <code>virtual IOReturn IOUSBPipe::ClearPipeStall(bool);</code><br>
 Replacement: <code>clearStall(...);</code>
 
 <code>virtual IOReturn IOUSBPipe::Read(IOMemoryDescriptor*, IOUSBCompletion*, UInt32*);<br>
 virtual IOReturn IOUSBPipe::Read(IOMemoryDescriptor*, UInt32, UInt32, IOUSBCompletion*, UInt32*);<br>
 virtual IOReturn IOUSBPipe::Read(IOMemoryDescriptor*, UInt32, UInt32, IOByteCount, IOUSBCompletion*, UInt32*);<br>
 virtual IOReturn IOUSBPipe::Read(IOMemoryDescriptor*, UInt32, UInt32, IOByteCount, IOUSBCompletionWithTimeStamp*, UInt32*);<br>
 virtual IOReturn IOUSBPipe::Write(IOMemoryDescriptor*, UInt32, UInt32, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBPipe::Write(IOMemoryDescriptor*, UInt32, UInt32, IOByteCount, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBPipe::Write(IOMemoryDescriptor*, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBPipe::Read(IOMemoryDescriptor*, UInt64, UInt32, IOUSBIsocFrame*, IOUSBIsocCompletion*);<br>
 virtual IOReturn IOUSBPipe::Read(IOMemoryDescriptor*, UInt64, UInt32, IOUSBLowLatencyIsocFrame*, IOUSBLowLatencyIsocCompletion*, UInt32);<br>
 virtual IOReturn IOUSBPipe::Write(IOMemoryDescriptor*, UInt64, UInt32, IOUSBIsocFrame*, IOUSBIsocCompletion*);<br>
 virtual IOReturn IOUSBPipe::Write(IOMemoryDescriptor*, UInt64, UInt32, IOUSBLowLatencyIsocFrame*, IOUSBLowLatencyIsocCompletion*, UInt32);</code><br>
 Replacement: <code>io(...);</code>
 
 <code>virtual IOReturn IOUSBPipe::ControlRequest(IOUSBDevRequestDesc*, IOUSBCompletion*, UInt32, UInt32);<br>
 virtual IOReturn IOUSBPipe::ControlRequest(IOUSBDevRequest*, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBPipe::ControlRequest(IOUSBDevRequestDesc*, UInt32, UInt32, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBPipe::ControlRequest(IOUSBDevRequest*, UInt32, UInt32, IOUSBCompletion*);</code><br>
 Replacement: <code>controlRequest(...);</code>
 
 <code>virtual const IOUSBController::Endpoint* IOUSBPipe::GetEndpoint();<br>
 virtual UInt8 IOUSBPipe::GetDirection();<br>
 virtual UInt8 IOUSBPipe::GetType();<br>
 virtual UInt8 IOUSBPipe::GetEndpointNumber();<br>
 virtual UInt16 IOUSBPipe::GetMaxPacketSize();<br>
 virtual UInt8 IOUSBPipe::GetInterval();</code><br>
 Replacement: Use <code>getEndpointDescriptor()</code>, <code>getSuperSpeedEndpointCompanionDescriptor()</code>, and <code>getDescriptors()</code> with <code>StandardUSB::getEndpoint*</code> methods to retrieve endpoint characteristics
 
 <code>virtual tUSBHostDeviceAddress IOUSBPipe::GetAddress();</code><br>
 Replacement: <code>getDeviceAddress();</code>
 
 <code>virtual UInt8 IOUSBPipe::GetSpeed();</code><br>
 Replacement: <code>getSpeed();</code>
 
 <code>virtual IOReturn IOUSBPipe::GetPipeStatus();</code><br>
 Replacement: If an endpoint is halted, new IO requests will be rejected with an appropriate error until the condition is cleared.
 
 <code>virtual IOReturn IOUSBPipe::SetPipePolicy(UInt16 maxPacketSize, UInt8 maxInterval);</code><br>
 Replacement: <code>adjustPipe(...);</code>
 
 <h3>IOUSBPipeV2 Migration</h3>
 
 IOUSBPipeV2 implemented interfaces for USB 3.0 streaming endpoint support.  IOUSBHostPipe provides interfaces to enable or disable streaming, and IOUSBHostStream provides interfaces to perform streaming IO.
 
 <code>virtual IOReturn IOUSBPipeV2::CreateStreams(UInt32);</code><br>
 Replacement: <code>enableStreams();</code> and <code>disableStreams();</code>

 <code>virtual IOReturn IOUSBPipeV2::Read(UInt32, IOMemoryDescriptor*, UInt32, UInt32, IOByteCount, IOUSBCompletion*, IOByteCount*);<br>
 virtual IOReturn IOUSBPipeV2::Write(UInt32, IOMemoryDescriptor*, UInt32, UInt32, IOByteCount, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBPipeV2::Abort(UInt32, IOOptionBits, IOReturn);</code><br>
 Replacement: Use <code>copyStream</code> and IOUSBHostStream interfaces to perform IO to a streaming endpoint.
 
 <code>virtual UInt32 IOUSBPipeV2::GetConfiguredStreams();</code><br>
 Replacement: none
 
 <code>virtual UInt32 IOUSBPipeV2::SupportsStreams();</code><br>
 Replacement: <code>StandardUSB::getEndpointMaxStreams(...);</code>
 
 <code>virtual const IOUSBSuperSpeedEndpointCompanionDescriptor* IOUSBPipeV2::GetSuperSpeedEndpointCompanionDescriptor();</code><br>
 Replacement: <code>getSuperSpeedEndpointCompanionDescriptor(...);</code>

 <code>virtual UInt8 IOUSBPipeV2::GetMult();</code><br>
 Replacement: <code>StandardUSB::getEndpointMult(...);</code>
 Refer to documentation for this method, as it may not return the same values as <code>IOUSBPipeV2::GetMult()</code>.

 <code>virtual UInt8 IOUSBPipeV2::GetMaxBurst();<br>
 virtual UInt16 IOUSBPipeV2::GetBytesPerInterval();</code><br>
 Replacement: <code>StandardUSB::getEndpointBurstSize(...)</code> or <code>StandardUSB::getEndpointBurstSize32(...)</code>.  Refer to documentation for the replacement methods, as they may not return the same values as <code>IOUSBPipeV2::GetMaxBurst()</code> or <code>IOUSBPipeV2::GetBytesPerInterval;</code>
 */

#ifndef IOUSBHostFamily_IOUSBHostPipe_h
#define IOUSBHostFamily_IOUSBHostPipe_h

#include <IOKit/IOService.h>
#include <IOKit/IOMemoryDescriptor.h>

#include <IOKit/usb/StandardUSB.h>
#include <IOKit/usb/IOUSBHostFamily.h>
#include <IOKit/usb/IOUSBHostIOSource.h>
#include <IOKit/usb/IOUSBHostStream.h>

#define TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTPIPE __has_include(<USBDriverKit/IOUSBHostPipe.h>)

#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTPIPE
#include <USBDriverKit/IOUSBHostPipe.h>
#endif

#include <AvailabilityMacros.h>

class IOUSBHostInterface;
class AppleUSBHostController;

/*!
 * @class       IOUSBHostPipe
 * @brief       The OSObject representing a USB endpoint
 * @discussion  This class provides functionality to transfer data across USB.  Function drivers should not subclass IOUSBHostPipe.
 */
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBHostPipe : public IOUSBHostIOSource
{
    friend class IOUSBHostInterface;
    friend class IOUSBHostDevice;
    friend class AppleUSBHostController;
    friend class AppleUSB20HostController;
    friend class AppleUSBIORequest;

#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTPIPE
    OSDeclareDefaultStructorsWithDispatch(IOUSBHostPipe)
#else
    OSDeclareDefaultStructors(IOUSBHostPipe)
#endif

public:
    /*!
     * @struct      StandardUSBDescriptors
     * @brief       Encapsulates descriptors for a single endpoint
     * @discussion  The StandardUSBDescriptors struct is used to intialize and adjust pipes in the system.  The bcdUSB member must be initialized to the USB revision supported by the device.  Acceptable values are 0x0110, 0x0200, 0x0300, 0x0310.  The descriptor member must always be initialized with a pointer to a valid endpoint descriptor.  The ssCompanionDescriptor and sspCompanionDescriptor members may be required for bcdUSB versions 0x0300 and greater, depending on device operating speed and values set in the descriptors.  See USB 3.1 ¤ 9.5 for more information on when these descriptors may be required.
     */
    struct StandardUSBDescriptors
    {
        uint16_t                                                                    bcdUSB;
        const StandardUSB::EndpointDescriptor*                                      descriptor;
        const StandardUSB::SuperSpeedEndpointCompanionDescriptor*                   ssCompanionDescriptor;
        const StandardUSB::SuperSpeedPlusIsochronousEndpointCompanionDescriptor*    sspCompanionDescriptor;
    };
    
    /*
     * @brief       Factory method for creating an IOUSBHostPipe object
     * @discussion  This method should not be called by function drivers.  To create an IOUSBHostPipe, use IOUSBHostInterface::copyPipe(...)
     * @param       descriptor EndpointDescriptor for the endpoint
     * @param       companionDescriptor SuperSpeedEndpointCompanionDescriptor for the endpoint, or NULL
     * @param       controller AppleUSBHostController to which the USB endpoint is associated
     * @param       device IOUSBHostDevice to which this endpoint belongs, or NULL
     * @param       interface IOUSBHostInterface to which this endpoint belongs, or NULL
     * @param       speed Operational speed of the device
     * @param       address tUSBHostDeviceAddress that has been allocated by the controller
     * @return      Pointer to an IOUSBHostPipe object if successful, otherwise NULL
     */
    static IOUSBHostPipe* withDescriptorsAndOwners(const StandardUSB::EndpointDescriptor* descriptor,
                                                   const StandardUSB::SuperSpeedEndpointCompanionDescriptor* companionDescriptor,
                                                   AppleUSBHostController* controller,
                                                   IOUSBHostDevice* device,
                                                   IOUSBHostInterface* interface,
                                                   UInt8 speed,
                                                   tUSBHostDeviceAddress address);

protected:
    virtual bool initWithDescriptorsAndOwners(const StandardUSB::EndpointDescriptor* descriptor,
                                              const StandardUSB::SuperSpeedEndpointCompanionDescriptor* companionDescriptor,
                                              AppleUSBHostController* controller,
                                              IOUSBHostDevice* device,
                                              IOUSBHostInterface* interface,
                                              UInt8 speed,
                                              tUSBHostDeviceAddress address);

#pragma mark IOUSBHostIOSource overrides
public:
    virtual void free();

    /*! @functiongroup IO 
     *  @discussion All I/O calls will synchronize with the workloop.  Furthermore, all completion callbacks will also synchronize with the workloop.  Therefore, when using the asynchronous I/O methods it is most performant to make subsequent calls from completion callback as the workloop lock will already be owned.
     */

    /*!
     * @brief       Abort pending I/O requests.
     * @discussion  See IOUSBHostIOSource::abort for documentation
     */
    virtual IOReturn abort(IOOptionBits options = kAbortAsynchronous, IOReturn withError = kIOReturnAborted, IOService* forClient = NULL);
    
protected:
    virtual IOReturn openGated();
    virtual IOReturn closeGated();
    virtual IOReturn destroyGated();
    virtual IOReturn abortGated(IOOptionBits options, IOReturn withError, IOService* forClient);
    virtual UInt32 getOutstandingIO(IOService* forClient);
    virtual UInt32 adjustOutstandingIO(IOService* forClient, SInt32 increment);

    // Protected pad slots for IOUSBHostIOSource overrides
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 0);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 1);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 2);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 3);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 4);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 5);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 6);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 7);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 8);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 9);
    
protected:
    UInt8               _speed;
    IOUSBHostInterface* _interface;

#pragma mark Descriptors and policies
public:
    /*!
     * @functiongroup Descriptors
     */
    
    /*!
     * @brief Options for <code>getEndpointDescriptor</code>, <code>getSuperSpeedEndpointCompanionDescriptor</code>, and <code>getDescriptors</code>
     *
     * @discussion
     * @constant kGetEndpointDescriptorOriginal - Original descriptor used when creating the pipe
     * @constant kGetEndpointDescriptorCurrentPolicy - Descriptor controlling the current endpoint policy, including changes made via the <code>adjustPolicy</code> method.
     */
    enum tGetEndpointDescriptorOptions
    {
        kGetEndpointDescriptorOriginal = 0,
        kGetEndpointDescriptorCurrentPolicy
    };
    
    /*!
     * @brief   Retrieve the EndpointDescriptor associated with this object
     * @param   type tGetEndpointDescriptorOptions indicating which descriptor to retrieve
     * @return  EndpointDescriptor pointer if successful, otherwise NULL
     */
    virtual const StandardUSB::EndpointDescriptor* getEndpointDescriptor(tGetEndpointDescriptorOptions type = kGetEndpointDescriptorCurrentPolicy);
    
    /*!
     * @brief   Retrieve the SuperSpeedEndpointCompanionDescriptor associated with this object
     * @param   type tGetEndpointDescriptorOptions indicating which descriptor to retrieve
     * @return  SuperSpeedEndpointCompanionDescriptor pointer if successful, otherwise NULL
     */
    virtual const StandardUSB::SuperSpeedEndpointCompanionDescriptor* getSuperSpeedEndpointCompanionDescriptor(tGetEndpointDescriptorOptions type = kGetEndpointDescriptorCurrentPolicy);

    /*! @functiongroup Policies */
    /*!
     * @brief       Adjust behavior of periodic endpoints to consume a different amount of bus bandwidth
     * @discussion  Periodic (interrupt and isochronous) endpoints reserve bus bandwidth when they are created, which takes into account max packet size, burst size, and the endpoint service interval.  If a function driver knows the endpoint will not use all of the allocated bandwidth, the <code>adjustPolicy</code> method may be used to reduce the bandwidth reserved for the endpoint.  The original endpoint descriptors should be copied and modified to adjust max packet size, mult, burst, and interval, and then passed to <code>adjustPolicy</code>.  The altered descriptors must pass <code>StandardUSB::validateEndpointDescriptor(...)</code> for policy changes to be processed.
     * @param       endpointDescriptor Pointer to an EndpointDescriptor describing the new endpoint policy
     * @param       companionDescriptor Pointer to a SuperSpeedEndpointCompanionDescriptor describing the bandwidth request
     * @return      IOReturn result code
     */
    virtual IOReturn adjustPipe(const StandardUSB::EndpointDescriptor* endpointDescriptor, const StandardUSB::SuperSpeedEndpointCompanionDescriptor* companionDescriptor);
    
    /*!
     * @brief       Sets the desired idle timeout for the pipe
     * @discussion  When a bulk or interrupt pipe is actively servicing an IO request, it will be considered "busy" for idleTimeoutMs.  For a more complete discussion of idle policies, refer to "Pausing IO" in IOUSBHostFamily.h.
     * @param       idleTimeoutMs The amount of time, in milliseconds, before an active transfer is considered idle.  If 0 the pipe will not be considered idle if there is an IO request enqueued.
     * @return      IOReturn result code
     */
    virtual IOReturn setIdlePolicy(uint32_t idleTimeoutMs);
    
    /*!
     * @brief       Retrieve the current idle timeout for the pipe
     * @return      Current idle timeout in milliseconds
     */
    virtual uint32_t getIdlePolicy();

    /*! @functiongroup IO */
    /*!
     * @brief       Clear the halt condition of the pipe.
     * @discussion  When a bulk or interrupt USB endpoint encounters any IO error other than a timeout, it transitions to a Halted state which must be cleared to perform additional IO on the endpoint.  This method will clear the halted condition for the endpoint, including sending a CLEAR_TT_BUFFER control request  (USB 2.0 11.24.2.3) to an intermediate hub if required.  All pending IO on the endpoint will be aborted, and the data toggle for the endpoint will also be reset.  To keep the device's data toggle synchronized with the host's data toggle, it is recommended that the withRequest parameter is always set to true, which causes the <code>clearStall</code> call to send a CLEAR_FEATURE ENDPOINT_HALT (USB 2.0 9.4.1) command to the device.  clearStall is not required for control endpoints.
     * @param       withRequest If true a CLEAR_FEATURE ENDPOINT_HALT (USB 2.0 9.4.1) will be sent to the device.
     *
     * @return      IOReturn result code
     */
    virtual IOReturn clearStall(bool withRequest);
    
    // Public pad slots for descriptors and policies
    /*! @functiongroup Policies */
    OSMetaClassDeclareReservedUsed(IOUSBHostPipe, 10);
    /*!
     * @brief       Adjust behavior of periodic endpoints to consume a different amount of bus bandwidth
     * @discussion  Periodic (interrupt and isochronous) endpoints reserve bus bandwidth when they are created, which takes into account max packet size, burst size, and the endpoint service interval.  If a function driver knows the endpoint will not use all of the allocated bandwidth, the <code>adjustPolicy</code> method may be used to reduce the bandwidth reserved for the endpoint.  The original endpoint descriptors should be copied and modified to adjust max packet size, mult, burst, and interval, and then passed to <code>adjustPolicy</code>.  The altered descriptors must pass <code>StandardUSB::validateEndpointDescriptor(...)</code> for policy changes to be processed.
     * @param       descriptors Pointer to a StandardUSBDescriptors structure describing the new endpoint policy
     * @return      IOReturn result code
     */
    virtual IOReturn adjustPipe(StandardUSBDescriptors* descriptors);

    /*! @functiongroup Descriptors */
    OSMetaClassDeclareReservedUsed(IOUSBHostPipe, 11);
    /*!
     * @brief   Retrieve the descriptors associated with this object
     * @param   descriptors StandardUSBDescriptors pointer to populate
     * @param   type tGetEndpointDescriptorOptions indicating which descriptors to retrieve
     * @return  IOReturn result
     */
    virtual IOReturn getDescriptors(StandardUSBDescriptors* descriptors, tGetEndpointDescriptorOptions type = kGetEndpointDescriptorCurrentPolicy);

    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 12);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 13);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 14);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 15);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 16);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 17);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 18);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 19);
    
protected:
    virtual IOReturn adjustPipeGated(const StandardUSB::EndpointDescriptor* endpointDescriptor,
                                     const StandardUSB::SuperSpeedEndpointCompanionDescriptor* companionDescriptor);
    
    virtual IOReturn setIdlePolicyGated(uint32_t idleTimeoutMs);
    virtual IOReturn getIdlePolicyGated(uint32_t& result);
    
    virtual IOReturn clearStallGated(bool withRequest);

    // Protected pad slots for descriptors and policies
    OSMetaClassDeclareReservedUsed(IOUSBHostPipe, 20);
    virtual IOReturn adjustPipeGatedV2(StandardUSBDescriptors* descriptors);

    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 21);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 22);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 23);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 24);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 25);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 26);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 27);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 28);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 29);
    
protected:
    const StandardUSB::EndpointDescriptor*                                   _descriptor;
    const StandardUSB::SuperSpeedEndpointCompanionDescriptor*                _companionDescriptor;
    const StandardUSB::SuperSpeedPlusIsochronousEndpointCompanionDescriptor* _sspCompanionDescriptor;

    StandardUSB::EndpointDescriptor                                     _currentPolicyDescriptor;
    StandardUSB::SuperSpeedEndpointCompanionDescriptor                  _currentPolicyCompanionDescriptor;
    StandardUSB::SuperSpeedPlusIsochronousEndpointCompanionDescriptor   _currentPolicySSPCompanionDescriptor;

    // Cached values
    UInt32 _maxPacketSize;
    UInt32 _burstSize;

    UInt32 _ioIdleTimeout;

#pragma mark Control requests
public:
    /*! @functiongroup IO */
    /*!
     * @brief       Enqueue a request on a control endpoint
     * @discussion  This method will enqueue an asynchronous request on a control endpoint.  If successful, the provided completion routine will be called to report the status of the completed IO.
     * @param       forClient The service issuing the request.
     * @param       request Reference to a valid StandardUSB::DeviceRequest structure.  The structure is copied and can therefore be stack-allocated.
     * @param       dataBuffer A void* or IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       completion Pointer to a IOUSBHostCompletion structure.  This will be copied and can therefore be stack-allocated.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      kIOReuturnSuccess if the completion will be called in the future, otherwise error
     */
    virtual IOReturn controlRequest(IOService* forClient, StandardUSB::DeviceRequest& request, void* dataBuffer, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs);
    virtual IOReturn controlRequest(IOService* forClient, StandardUSB::DeviceRequest& request, IOMemoryDescriptor* dataBuffer, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs);
    
    /*!
     * @brief       Send a request on a control endpoint
     * @discussion  This method will send a synchronous request on a control endpoint, and will not return until the request is complete.  This method will acquire the owning device's workloop lock, and will call commandSleep to send the control request.
     * @param       forClient The service issuing the request.
     * @param       request Reference to a valid StandardUSB::DeviceRequest structure.
     * @param       dataBuffer A void* or IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       bytesTransferred A uint32_t reference which will be updated with the byte count of the completed data phase.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      IOReturn value indicating the result of the IO request
     */
    virtual IOReturn controlRequest(IOService* forClient, StandardUSB::DeviceRequest& request, void* dataBuffer, uint32_t& bytesTransferred, uint32_t completionTimeoutMs);
    virtual IOReturn controlRequest(IOService* forClient, StandardUSB::DeviceRequest& request, IOMemoryDescriptor* dataBuffer, uint32_t& bytesTransferred, uint32_t completionTimeoutMs);

    // Public pad slots for control requests
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 30);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 31);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 32);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 33);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 34);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 35);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 36);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 37);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 38);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 39);

protected:
    struct tInternalControlRequestParameters
    {
        IOService*                  forClient;
        StandardUSB::DeviceRequest* request;
        void*                       rawBuffer;
        IOMemoryDescriptor*         descriptorBuffer;
        uint32_t*                   bytesTransferred;
        IOUSBHostCompletion*        completion;
        uint32_t                    completionTimeoutMs;
    };
    virtual IOReturn controlRequestGated(tInternalControlRequestParameters& parameters);

    static void rawBufferControlRequestCompletion(void* target, void* parameter, IOReturn status, uint32_t bytesTransferred);

    // Protected pad slots for control requests
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 40);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 41);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 42);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 43);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 44);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 45);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 46);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 47);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 48);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 49);

#pragma mark IO
public:
    /*!
     * @functiongroup IO
     */
    
    /*!
     * @brief       Enqueue a request on a bulk or interrupt endpoint
     * @discussion  See IOUSBHostIOSource::io for documentation
     * @param       completionTimeoutMs Must be 0 for interrupt endpoints.
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs = 0);
    
    /*!
     * @brief       Perform a request on a bulk or interrupt endpoint
     * @discussion  See IOUSBHostIOSource::io for documentation
     * @param       completionTimeoutMs Must be 0 for interrupt endpoints.
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, uint32_t& bytesTransferred, uint32_t completionTimeoutMs = 0);
    
    /*!
     * @brief       Enqueue or perform a request on an isochronous endpoint
     * @discussion  
     * This method is deprecated. Use <code>io(..., IOUSBHostIsochronousTransaction* frameList, ..., IOUSBHostIsochronousTransactionCompletion* completion);</code> instead.
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, IOUSBHostIsochronousFrame* frameList, uint32_t frameListCount, uint64_t firstFrameNumber = 0, IOUSBHostIsochronousCompletion* completion = NULL) DEPRECATED_ATTRIBUTE;
    
    /*!
     * @brief       Enqueue a request on a bulk or interrupt endpoint
     * @discussion  See IOUSBHostIOSource::io for documentation
     * @param       completionTimeoutMs Must be 0 for interrupt endpoints.
     */
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, uint32_t dataBufferLength, IOUSBHostBundledCompletion* completion, uint32_t completionTimeoutMs = 0);

    /*!
     * @brief       Enqueue or perform a request on an isochronous endpoint
     * @discussion
     * This method is used to issue isochronous requests.  The caller allocates and initializes an array of IOUSBHostIsochronousTransaction structures, which is used to describe the frames or microframes that will be transferred.  See @link IOUSBHostIsochronousTransaction @/link for information regarding structure initialization requirements and usage.  If the method determines that a bounce buffer is necessary then the entire dataBuffer will be bounced, not just the subset referred to by transactionList. To ensure a bounce buffer is not required, allocate the descriptor with <code>IOUSBHostInterface::createIOBuffer()</code>.
     * @param       dataBuffer Pointer to a valid memory descriptor to be used as the backing store for the I/O.
     * @param       transactionList Pointer first element in an IOUSBHostIsochronousTransaction array.  The array must contain at least transactionListCount elements.
     * @param       transactionListCount Number of elements in <code>transactionList</code>.
     * @param       firstFrameNumber Frame number which this request should begin on.  The current frame number can be queried via <code>IOUSHostDevice::getFrameNumber()</code> or <code>IOUSBHostInterface::getFrameNumber()</code>.  If 0, the transfer will start on the next available frame (XHCI only).
     * @param       options Flags that specify additional behavior for every transaction in this transfer.  See @link tIsochronousTransferOptions @/link for more details.
     * @param       completion To create a synchronous I/O request, this parameter must be NULL.  For an asynchronous request this parameter must be properly filled out prior to calling this method.  If not NULL, this parameter will be copied and can therefore be stack-allocated.
     * @return      IOReturn result code
     */
    OSMetaClassDeclareReservedUsed(IOUSBHostPipe, 50);
    virtual IOReturn io(IOMemoryDescriptor* dataBuffer, IOUSBHostIsochronousTransaction* transactionList, uint32_t transactionListCount, uint64_t firstFrameNumber = 0, IOOptionBits options = kIsochronousTransferOptionsNone, IOUSBHostIsochronousTransactionCompletion* completion = NULL);

    // Public pad slots for IO
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 51);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 52);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 53);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 54);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 55);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 56);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 57);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 58);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 59);

protected:
    virtual IOReturn isochronousIoGated(tInternalDataTransferParameters& parameters);

#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTPIPE
    static void asyncIOCompletionCallback(void* owner, void* parameter, IOReturn status, uint32_t bytesTransferred);
    static void asyncIOCompletionCallbackBundled(void *owner, uint32_t ioCompletionCount, IOMemoryDescriptor** dataBufferArray, void** parameterArray, IOReturn* statusArray, uint32_t* actualByteCountArray);
    static void asyncIsochIOCompletionCallback(void* owner, void* parameter, IOReturn status, IOUSBHostIsochronousFrame* frameList);
    static void asyncIsochIOTransactionCompletionCallback(void* owner, void* parameter, IOReturn status, IOUSBHostIsochronousTransaction* transactionList);
#endif
    static void isochIOTransactionCompatCallback(void* owner, void* parameter, IOReturn status, IOUSBHostIsochronousTransaction* transactionList);
    
    // Protected pad slots for IO
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 60);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 61);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 62);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 63);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 64);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 65);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 66);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 67);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 68);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 69);
    
#pragma mark Streams
public:
    /*!
     * @functiongroup Streams
     * @discussion IOUSBHostPipe defines interfaces to enable and disable streams for an endpoint, and to create IOUSBHostStream objects.  See IOUSBHostStream.h for additional documentation.
     */
    
    /*!
     * @brief       Enable streams for the IOUSBHostPipe
     * @discussion  This method changes the operational mode of the IOUSBHostPipe to allow streaming endpoint transfers, and must be called before copyStream will return any IOUSBHostStream objects.
     * @return      IOReturn result code.  An error will be returned if the pipe, device, or underlying host controller does not support streams.
     */
    virtual IOReturn enableStreams();
    
    /*!
     * @brief       Disable streams for the IOUSBHostPipe
     * @discussion  This method changes the operational mode of the IOUSBHostPipe to disable streaming endpoint transfers.  Calling this method will synchronously abort any outstanding calls on existing IOUSBHostStream objects, and therefore all stream contexts should first be set as non-active on the device via an out-of-band (class-defined) mechanism (USB 3.1 8.12.1.4).
     * @return      IOReturn result code.  An error will be returned if streams were not enabled for this IOUSBHostPipe.
     */
    virtual IOReturn disableStreams();
    
    /*!
     * @brief       Return the stream associated with <code>streamID</code>
     * @discussion  This method will return the stream associated with <code>streamID</code>.  If the stream doesn't exist yet it will be created.  The caller must release the IOUSBHostStream when finished using it.  <code>IOUSBHostPipe::enableStreams</code> must be called before this method will return a stream object.
     * @param       streamID Stream ID in the range of 1 to <code>max</code>, where <code>max</code> can be retrieved by calling <code>StandardUSB::getEndpointMaxStreams</code> with the endpoint descriptors.
     * @return      Pointer to a retain()ed IOUSBHostStream object or NULL.  NULL may be returned if either the device or the underlying host controller do not support that stream ID.
     */
    virtual IOUSBHostStream* copyStream(uint32_t streamID);
    
    // Public pad slots for streams
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 70);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 71);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 72);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 73);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 74);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 75);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 76);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 77);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 78);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 79);

protected:
    virtual IOReturn enableStreamsGated();
    
    virtual IOReturn disableStreamsGated();
    
    virtual IOReturn copyStreamGated(uint32_t streamID, IOUSBHostStream*& result);

    // Protected pad slots for streams
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 80);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 81);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 82);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 83);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 84);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 85);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 86);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 87);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 88);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 89);

protected:
    IOUSBHostStream** _streamArray;
    bool              _streamsEnabled;
    
#pragma mark Miscellaneous
public:
    /*! @functiongroup Provider interfaces */

    /*!
     * @brief       Retrieve the device's operational speed
     * @return      tInternalUSBHostConnectionSpeed
     */
    virtual uint8_t getSpeed() const;
    
    /*!
     * @brief   Retrieve the device's address
     * @return  Current address of the device
     */
    virtual tUSBHostDeviceAddress getDeviceAddress() const;
    
    // Public pad slots for miscellaneous
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 90);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 91);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 92);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 93);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 94);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 95);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 96);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 97);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 98);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 99);
    
protected:
    struct tRawBufferControlRequestCompletionParameter
    {
        IOMemoryDescriptor* memoryDescriptor;
        IOUSBHostCompletion clientCompletion;
    };
        
    // Protected pad slots for miscellaneous
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 100);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 101);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 102);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 103);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 104);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 105);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 106);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 107);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 108);
    OSMetaClassDeclareReservedUnused(IOUSBHostPipe, 109);


private:
    void destroyMemoryDescriptorRing();
    IOReturn io(IOMemoryDescriptor* dataBuffer, IOUSBHostIsochronousTransaction* transactionList, uint32_t transactionListCount, uint64_t firstFrameNumber, IOOptionBits options, IOUSBHostIsochronousTransactionCompletion* completion, IOUSBHostIsochronousFrame* frameList);

#pragma mark Deprecated
public:
    enum tPipeAbortOptions
    {
        kIOUSBPipeAbortAsync = IOUSBHostIOSource::kAbortAsynchronous,
        kIOUSBPipeAbortSync  = IOUSBHostIOSource::kAbortSynchronous
    };

    enum tUSBPipeState
    {
        kUSBPipeStateReady              = IOUSBHostIOSource::kStateReady,
        kUSBPipeStateRunningCompletions = IOUSBHostIOSource::kStateRunningCompletions,
        kUSBPipeStateAborting           = IOUSBHostIOSource::kStateAborting,
        kUSBPipeStateInactive           = IOUSBHostIOSource::kStateInactive
    };
};

#endif // IOUSBHostFamily_IOUSBHostPipe_h
