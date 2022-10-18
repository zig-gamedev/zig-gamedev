/* iig(DriverKit-191.30.2) generated from IOUSBHostInterface.iig */

/* IOUSBHostInterface.iig:1-39 */
/*
 * Copyright (c) 2018-2019 Apple Inc. All rights reserved.
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

#ifndef _IOKIT_IOUSBHOSTINTERFACE_H
#define _IOKIT_IOUSBHOSTINTERFACE_H

#include <USBDriverKit/AppleUSBDefinitions.h>
#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */
#include <DriverKit/IOMemoryDescriptor.h>  /* .iig include */
#include <USBDriverKit/IOUSBHostPipe.h>  /* .iig include */
#include <USBDriverKit/IOUSBHostDevice.h>  /* .iig include */
#include <DriverKit/IOBufferMemoryDescriptor.h>  /* .iig include */

/* source class IOUSBHostInterface IOUSBHostInterface.iig:40-260 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
*/

class KERNEL IOUSBHostInterface: public IOService
{
public:

    /*!
     * @brief       Open a session to the IOUSBHostInterface
     * @discussion  This method opens a session to an IOUSBHostInterface.  It will acquire the service's workloop lock.  Only one service may open a session at a time.
     * @param       forClient The IOService that is opening a session.
     * @param       options See IOService.h, <code>kOpenOptionsSelectAlternateInterface</code> in the options mask will immediately select the alternate setting passed by value through the <code>arg</code> parameter
     * @param       arg See IOService.h, or the value of the alt setting to use if <code>kOpenOptionsSelectAlternateInterface</code> is included in the options mask
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    Open (IOService   *forClient,
          IOOptionBits options,
          uint8_t * arg );

    /*!
     * @brief       Close a session to the IOUSBHostInterface
     * @discussion  This method closes an open session to an IOUSBHostInterface.  It will acquire the service's workloop lock, abort any IO for the interface and its endpoints, and may call commandSleep to allow processing of aborted IO before returning.
     * @param       forClient The IOService that is closing its session.
     * @param       options See IOService.h
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    Close (IOService   *forClient,
           IOOptionBits options);

    /*!
     * @brief       Return the current frame number of the USB controller
     * @discussion  This method will return the current frame number of the USB controller, omitting microframe.
     *              This is most useful for scheduling future isochronous requests.
     * @param       frameNumber uint64_t pointer to be updated with the current frame number
     * @param       theTime If not NULL, this will be updated with the current system time
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    GetFrameNumber (uint64_t     *frameNumber,
                    uint64_t *theTime);

    /*!
     * @brief       Retrieve the configuration descriptor in which this interface is defined
     * @return      ConfigurationDescriptor pointer. It is the caller's responsibility to free the returned descriptor.
     */
    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor() LOCALONLY;

    /*!
     * @brief       Retrieve the interface descriptor
     * @param       pointer to IOUSBConfigurationDescriptor
     * @return      IOUSBInterfaceDescriptor pointer relative to the passed in IOUSBConfigurationDescriptor pointer
     */
    virtual const IOUSBInterfaceDescriptor *
    GetInterfaceDescriptor(const IOUSBConfigurationDescriptor * configurationDescriptor) LOCALONLY;

    /*!
     * @brief       Return a string descriptor from the device
     * @discussion  This method uses CopyDescriptor() to return a string descriptor.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       languageID Descriptor language ID.  <code>wIndex</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).     *
     * @return      Pointer to the descriptor if found.  It is the caller's responsibility to free the returned descriptor.
     */
    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor (uint8_t  index,
                          uint16_t languageID) LOCALONLY;
    
    /*!
     * @brief       Return a string descriptor from the device. Uses default language of US English.
     * @discussion  This method uses CopyDescriptor() to return a string descriptor.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @return      Pointer to the descriptor if found.  It is the caller's responsibility to free the returned descriptor.
     */
    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor (uint8_t  index) LOCALONLY;
    
    
    /*!
     * @brief       Return the pipe whose <code>bEndpointAddress</code> matches <code>address</code>
     * @discussion  This method will return the pipe whose <code>bEndpointAddress</code> matches <code>address</code>.  If the pipe doesn't exist yet, but is part of the interface, it will first be created.  The caller must release the IOUSBHostPipe when finished using it.
     * @param       address Address of the pipe
     * @param       returns a IOUSBHostPipe object
     * @return      InterfaceDescriptor pointer.
     */
    virtual kern_return_t
    CopyPipe(uint8_t address, IOUSBHostPipe ** pipe);


    /*!
     * @brief       Select an alternate setting for this interface
     * @discussion  This method is used to select an alternate setting for the interface.  All pending IO on the interface's pipes will be aborted, and the open pipes will be closed.  The new alternate setting will be selected via SET_INTERFACE control request (USB 2.0 9.4.10).
     * @param       bAlternateSetting Alternate interface number to activate
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    SelectAlternateSetting(uint8_t bAlternateSetting);

    /*!
     * @brief       Return the current port status
     * @discussion  Combination of IOUSBHostPortStatus values
     * @param       uint32_t pointer to be updated with port status
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    GetPortStatus(uint32_t * portStatus);

    /*!
     * @brief       Sets the desired idle suspend timeout for the interface
     * @discussion  Once the interface is considered idle, it will defer electrical suspend of the device for the specified duration.  For a more complete discussion of idle policies, refer to "Idle suspend" in IOUSBHostFamily.h.
     * @param       deviceIdleTimeout The amount of time, in milliseconds, after all pipes are idle to wait before suspending the device.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    SetIdlePolicy(uint32_t deviceIdleTimeout);

    /*!
     * @brief       Retrieve the current idle suspend timeout.  See @link setIdlePolicy @/link
     * @param       uint32_t pointer to be updated with the amount of time, in milliseconds, after all pipes are idle to wait before suspending the device.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    GetIdlePolicy(uint32_t *deviceIdleTimeout);

    /*!
     * @brief       Return the parent/provider IOUSBHostDevice object of this interface.
     * @param       pointer used to pass out the IOUSBHostDevice pointer.  The caller must release the IOUSBHostDevice when finished using it.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    CopyDevice(IOUSBHostDevice ** device);
    
   
    /*!
     * @brief       Allocate a buffer to be used for I/O
     * @discussion  This method will allocate an IOBufferMemoryDescriptor optimized for use by the underlying controller hardware.  A buffer allocated by this method will not be bounced to perform DMA operations.
     * @param       options kIODirectionOut, kIODirectionIn to set the direction of the I/O transfer.
     * @param       capacity Size of the buffer to allocate
     * @param       Pointer to a Pointer to an IOBufferMemoryDescriptor. Passes out a valid pointer upon success. The caller must release the buffer when the buffer when finished using it
     * @return      KERN_SUCCESS if the completion will be called in the future, otherwise error
     */
    virtual kern_return_t
    CreateIOBuffer (IOOptionBits   options,
                    uint64_t capacity,
                    IOBufferMemoryDescriptor ** buffer) LOCALONLY;
    

    
    /*!
     * @brief       Enqueue a request on the default control endpoint
     * @discussion  This method will enqueue an asynchronous request on the default control endpoint.  If successful,
     *              the provided completion routine will be called to report the status of the completed IO.  See USB 2.0 9.3.
     * @param       forClient The service with an open session issuing the request.
     * @param       bmRequestType The characteristics of the device request.  See USBmakebmRequestType() macro for constructing.
     * @param       bRequest The specific request
     * @param       wValue Data specific to the request
     * @param       wIndex Data specific to the request
     * @param       wLength Number of bytes to transfer if there is a data stage.
     * @param       dataBuffer An IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       completion Pointer to an OSAction object defining the callback.
     *  <code>
     *  OSAction *callback;
     *
     *  ret = CreateActionDeviceRequestComplete(sizeof(<YourDriver>CallbackContext), &callback);
     *  </code>
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      KERN_SUCCESS if the completion will be called in the future, otherwise error
     */
    virtual kern_return_t
    AsyncDeviceRequest (uint8_t                        bmRequestType,
                        uint8_t                        bRequest,
                        uint16_t                       wValue,
                        uint16_t                       wIndex,
                        uint16_t                       wLength,
                        IOMemoryDescriptor            *dataBuffer,
                        OSAction                      *completion/*TYPE(CompleteAsyncDeviceRequest)*/,
                        uint32_t                       completionTimeoutMs) LOCALONLY;
    
    /*!
     * @brief       Send a request on the default control endpoint
     * @discussion  This method will send a synchronous request on the default control endpoint, and will not return until
     *              the request is complete.  This method will acquire the service's workloop lock, and will call commandSleep
     *              to send the control request.  See USB 2.0 9.3.
     * @param       bmRequestType The characteristics of the device request.  See USBmakebmRequestType() macro for constructing.
     * @param       bRequest The specific request
     * @param       wValue Data specific to the request
     * @param       wIndex Data specific to the request
     * @param       wLength Number of bytes to transfer if there is a data stage.
     * @param       dataBuffer An IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       bytesTransferred A uint32_t pointer which will be updated with the byte count of the completed data phase.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      KERN_SUCCESS if the completion will be called in the future, otherwise error
     */
    virtual kern_return_t
    DeviceRequest (uint8_t             bmRequestType,
                   uint8_t             bRequest,
                   uint16_t            wValue,
                   uint16_t            wIndex,
                   uint16_t            wLength,
                   IOMemoryDescriptor *dataBuffer,
                   uint16_t           *bytesTransferred,
                   uint32_t            completionTimeoutMs) LOCALONLY;
    
    
    /*!
     * @brief       Abort device requests made via the @link deviceRequest @/link methods by <code>forClient</code>
     * @discussion  This method will abort any requests associated with a specific client made via the @link AsyncDeviceRequest @/link methods.
     *              It will not abort requests made by other clients.
     * @param       options IOUSBHostIOSource::tAbortOptions
     * @param       withError IOReturn error value to return with the requests.  The default value of kIOReturnAborted should be used.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    AbortDeviceRequests (IOOptionBits options,
                         IOReturn     withError) LOCALONLY;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOUSBHostInterface IOUSBHostInterface.iig:40-260 */

#define IOUSBHostInterface__GetInterfaceDescriptorHeader_ID            0x9434ede8968ad899ULL
#define IOUSBHostInterface_Open_ID            0xe1d4ddd5e2cbe425ULL
#define IOUSBHostInterface_Close_ID            0xcc4d8955a6ca402dULL
#define IOUSBHostInterface_GetFrameNumber_ID            0xa54dc3018b39ad1dULL
#define IOUSBHostInterface_CopyPipe_ID            0x86bb6756a5becea0ULL
#define IOUSBHostInterface_SelectAlternateSetting_ID            0xbf28ca8ee6edc0d3ULL
#define IOUSBHostInterface_GetPortStatus_ID            0xbed0143a5a239d9cULL
#define IOUSBHostInterface_SetIdlePolicy_ID            0x832000ee4e988f9fULL
#define IOUSBHostInterface_GetIdlePolicy_ID            0xc5f9b873ec9e7322ULL
#define IOUSBHostInterface_CopyDevice_ID            0xba535ca623a6536cULL

#define IOUSBHostInterface__GetInterfaceDescriptorHeader_Args \
        IOUSBInterfaceDescriptor * descriptor

#define IOUSBHostInterface_Open_Args \
        IOService * forClient, \
        IOOptionBits options, \
        uint8_t * arg

#define IOUSBHostInterface_Close_Args \
        IOService * forClient, \
        IOOptionBits options

#define IOUSBHostInterface_GetFrameNumber_Args \
        uint64_t * frameNumber, \
        uint64_t * theTime

#define IOUSBHostInterface_CopyPipe_Args \
        uint8_t address, \
        IOUSBHostPipe ** pipe

#define IOUSBHostInterface_SelectAlternateSetting_Args \
        uint8_t bAlternateSetting

#define IOUSBHostInterface_GetPortStatus_Args \
        uint32_t * portStatus

#define IOUSBHostInterface_SetIdlePolicy_Args \
        uint32_t deviceIdleTimeout

#define IOUSBHostInterface_GetIdlePolicy_Args \
        uint32_t * deviceIdleTimeout

#define IOUSBHostInterface_CopyDevice_Args \
        IOUSBHostDevice ** device

#define IOUSBHostInterface_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOUSBHostInterface * self, const IORPC rpc);\
\
    kern_return_t\
    _GetInterfaceDescriptorHeader(\
        IOUSBInterfaceDescriptor * descriptor,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Open(\
        IOService * forClient,\
        IOOptionBits options,\
        uint8_t * arg,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Close(\
        IOService * forClient,\
        IOOptionBits options,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetFrameNumber(\
        uint64_t * frameNumber,\
        uint64_t * theTime,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyPipe(\
        uint8_t address,\
        IOUSBHostPipe ** pipe,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SelectAlternateSetting(\
        uint8_t bAlternateSetting,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetPortStatus(\
        uint32_t * portStatus,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetIdlePolicy(\
        uint32_t deviceIdleTimeout,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetIdlePolicy(\
        uint32_t * deviceIdleTimeout,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyDevice(\
        IOUSBHostDevice ** device,\
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
    typedef kern_return_t (*_GetInterfaceDescriptorHeader_Handler)(OSMetaClassBase * target, IOUSBHostInterface__GetInterfaceDescriptorHeader_Args);\
    static kern_return_t\
    _GetInterfaceDescriptorHeader_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _GetInterfaceDescriptorHeader_Handler func);\
\
    typedef kern_return_t (*Open_Handler)(OSMetaClassBase * target, IOUSBHostInterface_Open_Args);\
    static kern_return_t\
    Open_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Open_Handler func);\
\
    typedef kern_return_t (*Close_Handler)(OSMetaClassBase * target, IOUSBHostInterface_Close_Args);\
    static kern_return_t\
    Close_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Close_Handler func);\
\
    typedef kern_return_t (*GetFrameNumber_Handler)(OSMetaClassBase * target, IOUSBHostInterface_GetFrameNumber_Args);\
    static kern_return_t\
    GetFrameNumber_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetFrameNumber_Handler func);\
\
    typedef kern_return_t (*CopyPipe_Handler)(OSMetaClassBase * target, IOUSBHostInterface_CopyPipe_Args);\
    static kern_return_t\
    CopyPipe_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyPipe_Handler func);\
\
    typedef kern_return_t (*SelectAlternateSetting_Handler)(OSMetaClassBase * target, IOUSBHostInterface_SelectAlternateSetting_Args);\
    static kern_return_t\
    SelectAlternateSetting_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SelectAlternateSetting_Handler func);\
\
    typedef kern_return_t (*GetPortStatus_Handler)(OSMetaClassBase * target, IOUSBHostInterface_GetPortStatus_Args);\
    static kern_return_t\
    GetPortStatus_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetPortStatus_Handler func);\
\
    typedef kern_return_t (*SetIdlePolicy_Handler)(OSMetaClassBase * target, IOUSBHostInterface_SetIdlePolicy_Args);\
    static kern_return_t\
    SetIdlePolicy_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetIdlePolicy_Handler func);\
\
    typedef kern_return_t (*GetIdlePolicy_Handler)(OSMetaClassBase * target, IOUSBHostInterface_GetIdlePolicy_Args);\
    static kern_return_t\
    GetIdlePolicy_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetIdlePolicy_Handler func);\
\
    typedef kern_return_t (*CopyDevice_Handler)(OSMetaClassBase * target, IOUSBHostInterface_CopyDevice_Args);\
    static kern_return_t\
    CopyDevice_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyDevice_Handler func);\
\


#define IOUSBHostInterface_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    _GetInterfaceDescriptorHeader_Impl(IOUSBHostInterface__GetInterfaceDescriptorHeader_Args);\
\
    kern_return_t\
    Open_Impl(IOUSBHostInterface_Open_Args);\
\
    kern_return_t\
    Close_Impl(IOUSBHostInterface_Close_Args);\
\
    kern_return_t\
    GetFrameNumber_Impl(IOUSBHostInterface_GetFrameNumber_Args);\
\
    kern_return_t\
    CopyPipe_Impl(IOUSBHostInterface_CopyPipe_Args);\
\
    kern_return_t\
    SelectAlternateSetting_Impl(IOUSBHostInterface_SelectAlternateSetting_Args);\
\
    kern_return_t\
    GetPortStatus_Impl(IOUSBHostInterface_GetPortStatus_Args);\
\
    kern_return_t\
    SetIdlePolicy_Impl(IOUSBHostInterface_SetIdlePolicy_Args);\
\
    kern_return_t\
    GetIdlePolicy_Impl(IOUSBHostInterface_GetIdlePolicy_Args);\
\
    kern_return_t\
    CopyDevice_Impl(IOUSBHostInterface_CopyDevice_Args);\
\


#define IOUSBHostInterface_VirtualMethods \
\
public:\
\
    virtual const IOUSBConfigurationDescriptor *\
    CopyConfigurationDescriptor(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBInterfaceDescriptor *\
    GetInterfaceDescriptor(\
        const IOUSBConfigurationDescriptor * configurationDescriptor) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBStringDescriptor *\
    CopyStringDescriptor(\
        uint8_t index,\
        uint16_t languageID) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBStringDescriptor *\
    CopyStringDescriptor(\
        uint8_t index) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    CreateIOBuffer(\
        IOOptionBits options,\
        uint64_t capacity,\
        IOBufferMemoryDescriptor ** buffer) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    AsyncDeviceRequest(\
        uint8_t bmRequestType,\
        uint8_t bRequest,\
        uint16_t wValue,\
        uint16_t wIndex,\
        uint16_t wLength,\
        IOMemoryDescriptor * dataBuffer,\
        OSAction * completion,\
        uint32_t completionTimeoutMs) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    DeviceRequest(\
        uint8_t bmRequestType,\
        uint8_t bRequest,\
        uint16_t wValue,\
        uint16_t wIndex,\
        uint16_t wLength,\
        IOMemoryDescriptor * dataBuffer,\
        uint16_t * bytesTransferred,\
        uint32_t completionTimeoutMs) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    AbortDeviceRequests(\
        IOOptionBits options,\
        IOReturn withError) APPLE_KEXT_OVERRIDE;\
\


#if !KERNEL

extern OSMetaClass          * gIOUSBHostInterfaceMetaClass;
extern const OSClassLoadInformation IOUSBHostInterface_Class;

class IOUSBHostInterfaceMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class IOUSBHostInterfaceInterface : public OSInterface
{
public:
    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor() = 0;

    virtual const IOUSBInterfaceDescriptor *
    GetInterfaceDescriptor(const IOUSBConfigurationDescriptor * configurationDescriptor) = 0;

    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor(uint8_t index,
        uint16_t languageID) = 0;

    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor(uint8_t index) = 0;

    virtual kern_return_t
    CreateIOBuffer(IOOptionBits options,
        uint64_t capacity,
        IOBufferMemoryDescriptor ** buffer) = 0;

    virtual kern_return_t
    AsyncDeviceRequest(uint8_t bmRequestType,
        uint8_t bRequest,
        uint16_t wValue,
        uint16_t wIndex,
        uint16_t wLength,
        IOMemoryDescriptor * dataBuffer,
        OSAction * completion,
        uint32_t completionTimeoutMs) = 0;

    virtual kern_return_t
    DeviceRequest(uint8_t bmRequestType,
        uint8_t bRequest,
        uint16_t wValue,
        uint16_t wIndex,
        uint16_t wLength,
        IOMemoryDescriptor * dataBuffer,
        uint16_t * bytesTransferred,
        uint32_t completionTimeoutMs) = 0;

    virtual kern_return_t
    AbortDeviceRequests(IOOptionBits options,
        IOReturn withError) = 0;

    const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor_Call()  { return CopyConfigurationDescriptor(); };\

    const IOUSBInterfaceDescriptor *
    GetInterfaceDescriptor_Call(const IOUSBConfigurationDescriptor * configurationDescriptor)  { return GetInterfaceDescriptor(configurationDescriptor); };\

    const IOUSBStringDescriptor *
    CopyStringDescriptor_Call(uint8_t index,
        uint16_t languageID)  { return CopyStringDescriptor(index, languageID); };\

    const IOUSBStringDescriptor *
    CopyStringDescriptor_Call(uint8_t index)  { return CopyStringDescriptor(index); };\

    kern_return_t
    CreateIOBuffer_Call(IOOptionBits options,
        uint64_t capacity,
        IOBufferMemoryDescriptor ** buffer)  { return CreateIOBuffer(options, capacity, buffer); };\

    kern_return_t
    AsyncDeviceRequest_Call(uint8_t bmRequestType,
        uint8_t bRequest,
        uint16_t wValue,
        uint16_t wIndex,
        uint16_t wLength,
        IOMemoryDescriptor * dataBuffer,
        OSAction * completion,
        uint32_t completionTimeoutMs)  { return AsyncDeviceRequest(bmRequestType, bRequest, wValue, wIndex, wLength, dataBuffer, completion, completionTimeoutMs); };\

    kern_return_t
    DeviceRequest_Call(uint8_t bmRequestType,
        uint8_t bRequest,
        uint16_t wValue,
        uint16_t wIndex,
        uint16_t wLength,
        IOMemoryDescriptor * dataBuffer,
        uint16_t * bytesTransferred,
        uint32_t completionTimeoutMs)  { return DeviceRequest(bmRequestType, bRequest, wValue, wIndex, wLength, dataBuffer, bytesTransferred, completionTimeoutMs); };\

    kern_return_t
    AbortDeviceRequests_Call(IOOptionBits options,
        IOReturn withError)  { return AbortDeviceRequests(options, withError); };\

};

struct IOUSBHostInterface_IVars;
struct IOUSBHostInterface_LocalIVars;

class IOUSBHostInterface : public IOService, public IOUSBHostInterfaceInterface
{
#if !KERNEL
    friend class IOUSBHostInterfaceMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
#ifdef IOUSBHostInterface_DECLARE_IVARS
IOUSBHostInterface_DECLARE_IVARS
#else /* IOUSBHostInterface_DECLARE_IVARS */
    union
    {
        IOUSBHostInterface_IVars * ivars;
        IOUSBHostInterface_LocalIVars * lvars;
    };
#endif /* IOUSBHostInterface_DECLARE_IVARS */
#endif /* !KERNEL */

#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gIOUSBHostInterfaceMetaClass; };
#endif /* KERNEL */

    using super = IOService;

#if !KERNEL
    IOUSBHostInterface_Methods
    IOUSBHostInterface_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#endif /* !__DOCUMENTATION__ */

/* IOUSBHostInterface.iig:262-263 */


/* IOUSBHostInterface.iig:270- */

#endif /* ! _IOKIT_IOUSBHOSTINTERFACE_H */

