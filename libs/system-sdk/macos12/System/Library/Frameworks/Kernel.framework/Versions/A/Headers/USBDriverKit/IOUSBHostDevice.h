/* iig(DriverKit-191.30.2) generated from IOUSBHostDevice.iig */

/* IOUSBHostDevice.iig:1-38 */
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

#ifndef _IOKIT_IOUSBHOSTDEVICE_H
#define _IOKIT_IOUSBHOSTDEVICE_H

#include <USBDriverKit/AppleUSBDefinitions.h>
#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */
#include <DriverKit/IOBufferMemoryDescriptor.h>  /* .iig include */

class IOUSBHostInterface;

/* source class IOUSBHostDevice IOUSBHostDevice.iig:39-376 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
*/

class KERNEL IOUSBHostDevice: public IOService
{
public:
    /*!
     * @brief       Open a session to the IOUSBHostDevice
     * @discussion  This method opens a session to an IOUSBHostDevice.  It will acquire the service's workloop lock.
     *              Child IOUSBHostInterfaces may open simultaneous sessions, but only one other service may open a session.
     * @param       forClient The IOService that is opening a session.
     * @param       options See IOService.h
     * @param       arg See IOService.h
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    Open (IOService   *forClient,
          IOOptionBits options,
          uintptr_t    arg);


    /*!
     * @brief       Close a session to the IOUSBHostDevice
     * @discussion  This method closes an open session to an IOUSBHostDevice.  It will acquire the service's workloop lock,
     *              abort any IO associated with the specified client, and may call commandSleep to allow processing of
     *              aborted control requests before returning.
     * @param       forClient The IOService that is closing its session.  Any IO associated with the specified client will
     *              be aborted.
     * @param       options See IOService.h
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    Close (IOService   *forClient,
           IOOptionBits options);


    /*!
     * @brief       Retrieve a descriptor from the cache or the device
     * @discussion  This method will search the descriptor cache for the descriptor that matches the input arguments.
     *              If the descriptor is not in the cache, a GET_DESCRIPTOR control request (USB 2.0 9.4.3) will be issued
     *              to retrieve the descriptor from the device.  If the device request is successful, the retrieved descriptor
     *              will be added to the cache.
     *              This method will acquire the service's workloop lock, and may call commandSleep to perform the GET_DESCRIPTOR
     *              control request.
     * @param       type <code>bDescriptorType</code> of the descriptor to find.
     * @param       length Pointer to a uint16_t which will be updated with the length of the descriptor.  As input, used
     *              as <code>wLength</code> when fetching variable-length configuration or BOS descriptors, or when fetching
     *              nonstandard descriptor types.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       languageID Descriptor language ID.  <code>wIndex</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       requestType tDeviceRequestType to be used for a GET_DESCRIPTOR control request.
     * @param       requestRecipient tDeviceRequestRecipient to be used for a GET_DESCRIPTOR control request.
     * @param       descriptor Pointer to the data to be updated with the retrieved descriptor.  Must be at least <code>length</code>
     *              bytes.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    CopyDescriptor (uint8_t   type,
                    uint16_t *length,
                    uint8_t   index,
                    uint16_t  languageID,
                    uint8_t   requestType,
                    uint8_t   requestRecipient,
                    uint8_t  *descriptor) LOCALONLY;

    /*!
     * @brief       Return the device descriptor
     * @discussion  This method uses getDescriptor to retrieve the device descriptor.
     * @return      Pointer to the device descriptor, or NULL if the service is terminating.  It is the caller's
     *              responsibility to free the returned descriptor.
     */
    virtual const IOUSBDeviceDescriptor *
    CopyDeviceDescriptor (void) LOCALONLY;


    /*!
     * @brief       Return the configuration descriptor with the specified index
     * @discussion  This method uses getDescriptor to retrieve a configuration descriptor.
     * @param       index Descriptor index value
     * @return      Pointer to the configuration descriptor if found. It is the caller's responsibility to free the
     *              returned descriptor.
     */
    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor (uint8_t index) LOCALONLY;


    /*!
     * @brief       Return the configuration descriptor with the specified <code>bConfigurationValue</code>
     * @discussion  This method uses getDescriptor to search for a configuration descriptor with a specific <code>bConfigurationValue</code> field.
     * @param       bConfigurationValue Value to match
     * @return      Pointer to the configuration descriptor if found.  It is the caller's responsibility to free the
     *              returned descriptor.
     */
    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptorWithValue (uint8_t bConfigurationValue) LOCALONLY;


    /*!
     * @brief       Return the currently selected configuration descriptor
     * @discussion  This method uses getDescriptor to return the configuration descriptor currently selected after a successful setConfiguration call
     * @return      Pointer to the configuration descriptor if found, or NULL if the device is not configured. It is the
     *              caller's responsibility to free the returned descriptor.
     */
    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor (IOService *forClient) LOCALONLY;


    /*!
     * @brief       Return the capability descriptors of the device
     * @discussion  This method uses getDescriptor to return the device's BOS descriptors
     * @return      Pointer to the BOS descriptor if found, otherwise NULL.  It is the caller's responsibility to free the
     *              returned descriptor.  It is the caller's responsibility to free the returned descriptor.
     */
    virtual const IOUSBBOSDescriptor *
    CopyCapabilityDescriptors (void) LOCALONLY;


    /*!
     * @brief       Return a string descriptor from the device
     * @discussion  This method uses getDescriptor to return a string descriptor.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       languageID Descriptor language ID.  <code>wIndex</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).     *
     * @return      Pointer to the descriptor if found.  It is the caller's responsibility to free the returned descriptor.
     */
    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor (uint8_t  index,
                          uint16_t languageID) LOCALONLY;

    /*!
     * @brief       Return a string descriptor from the device. Uses default language of US English.
     * @discussion  This method uses getDescriptor to return a string descriptor.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @return      Pointer to the descriptor if found.  It is the caller's responsibility to free the returned descriptor.
     */
    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor (uint8_t  index) LOCALONLY;

    
    /*!
     * @brief       Allocate a buffer to be used for I/O
     * @discussion  This method will allocate an IOBufferMemoryDescriptor optimized for use by the underlying controller hardware.  A buffer allocated by this method will not be bounced to perform DMA operations.
     * @param       options kIODirectionOut, kIODirectionIn to set the direction of the I/O transfer.
     * @param       capacity Size of the buffer to allocate
     * @param       Pointer to a Pointer to an IOBufferMemoryDescriptor. Passes out a valid pointer upon success.
     * @return      KERN_SUCCESS if the completion will be called in the future, otherwise error
     */
    virtual kern_return_t
    CreateIOBuffer (IOOptionBits   options,
                    uint64_t capacity,
                    IOBufferMemoryDescriptor ** buffer);

    /*!
     * @brief       Type definition for asynchronous completion routines.
     * @param       action Pointer to the OSAction object of the async request
     * @param       status Result of the operation
     * @param       actualByteCount Byte could of the completed data phase.
     * @return      void
     */
    virtual void
    CompleteAsyncDeviceRequest (OSAction              *action TARGET,
                                IOReturn              status,
                                uint32_t              bytesTransferred) = 0;


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
    AsyncDeviceRequest (IOService                     *forClient,
                        uint8_t                        bmRequestType,
                        uint8_t                        bRequest,
                        uint16_t                       wValue,
                        uint16_t                       wIndex,
                        uint16_t                       wLength,
                        IOMemoryDescriptor            *dataBuffer,
                        OSAction                      *completion TYPE(CompleteAsyncDeviceRequest),
                        uint32_t                       completionTimeoutMs);


    /*!
     * @brief       Send a request on the default control endpoint
     * @discussion  This method will send a synchronous request on the default control endpoint, and will not return until
     *              the request is complete.  This method will acquire the service's workloop lock, and will call commandSleep
     *              to send the control request.  See USB 2.0 9.3.
     * @param       forClient The service with an open session issuing the request.
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
    DeviceRequest (IOService          *forClient,
                   uint8_t             bmRequestType,
                   uint8_t             bRequest,
                   uint16_t            wValue,
                   uint16_t            wIndex,
                   uint16_t            wLength,
                   IOMemoryDescriptor *dataBuffer,
                   uint16_t           *bytesTransferred,
                   uint32_t            completionTimeoutMs);


    /*!
     * @brief       Abort device requests made via the @link deviceRequest @/link methods by <code>forClient</code>
     * @discussion  This method will abort any requests associated with a specific client made via the @link AsyncDeviceRequest @/link methods.
     *              It will not abort requests made by other clients.
     * @param       forClient Client which issued the requests
     * @param       options IOUSBHostIOSource::tAbortOptions
     * @param       withError IOReturn error value to return with the requests.  The default value of kIOReturnAborted should be used.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    AbortDeviceRequests (IOService   *forClient,
                         IOOptionBits options,
                         IOReturn     withError);



    /*!
     * @brief       Select a new configuration for the device
     * @discussion  This method will select a new configuration for a device.  If the device was previously configured all child
     *              interfaces will be terminated prior to setting the new configuration.  This method will send the SET_CONFIGURATION
     *              control request (USB 2.0 9.4.7) to the device.
     *              This method will acquire the service's workloop lock, and may call commandSleep to perform the SET_CONFIGURATION control request.
     * @param       bConfigurationValue Configuration to select
     * @param       matchInterfaces If true, any interfaces within the new configuration will be registered for matching.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    SetConfiguration (uint8_t bConfigurationValue,
                      bool    matchInterfaces);


    /*!
     * @brief   Retrieve the device's address
     * @param   address uint8_t pointer to be updated with the device's address.
     * @return  KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    GetAddress (uint8_t *address) const;


    /*!
     * @brief       Retrieve the device's operational speed
     * @param       speed uint8_t pointer to be updated with the device's operational speed.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    GetSpeed (uint8_t *speed) const;


    /*!
     * @brief       Return the current frame number of the USB controller
     * @description This method will return the current frame number of the USB controller, omitting microframe.
     *              This is most useful for scheduling future isochronous requests.
     * @param       frameNumber uint64_t pointer to be updated with the current frame number
     * @param       theTime If not NULL, this will be updated with the current system time
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    GetFrameNumber (uint64_t     *frameNumber,
                    uint64_t *theTime);

    /*!
     * @brief       Return the current port status
     * @discussion  Combination of IOUSBHostPortStatus values
     * @param       uint32_t pointer to be updated with port status
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    GetPortStatus(uint32_t * portStatus);

    /*!
     * @brief       Terminate the device and attempt to reenumerate it
     * @discussion  This function will reset and attempt to reenumerate the USB device.  The current IOUSBHostDevice
     *              object and all of its children will be terminated.  A new IOUSBHostDevice object will be created
     *              and registered if the reset is successful and the previous object has finished terminating.
     *              This function may not be called from the port workloop thread.
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    Reset ();
    
    /*!
     * @brief       Create a child interface interator
     * @discussion  This iterator is used to retrieve all the IOUSBHostInterface * children belonging to this device
     * @param       pointer to an opaque representation of the iterator
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    CreateInterfaceIterator(uintptr_t * ref);

    /*!
     * @brief       Destroy a child interface interator.
     * @discussion  Used to destroy iterator created by CreateInterfaceIterator()
     * @param       opaque representation of the iterator
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    DestroyInterfaceIterator(uintptr_t ref);

    /*!
     * @brief       Get the next IOUSBHostIterface * child associated with this IOUSBHostDevice
     * @discussion  Keep calling this until NULL is returned via the the interface**
     * @param       IOUSBHostInterface ** used to return the next interface
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    virtual kern_return_t
    CopyInterface(uintptr_t ref, IOUSBHostInterface ** interface);
    
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOUSBHostDevice IOUSBHostDevice.iig:39-376 */

#define IOUSBHostDevice__GetDescriptor_ID            0xc769617535436f0eULL
#define IOUSBHostDevice_Open_ID            0xe139dec6668972a6ULL
#define IOUSBHostDevice_Close_ID            0xf5b5354a1a05c969ULL
#define IOUSBHostDevice_CreateIOBuffer_ID            0xb5f78ffc0afa7e23ULL
#define IOUSBHostDevice_CompleteAsyncDeviceRequest_ID            0xc489759689d7d751ULL
#define IOUSBHostDevice_AsyncDeviceRequest_ID            0xb6ab02bb32783f83ULL
#define IOUSBHostDevice_DeviceRequest_ID            0x862ae6f7392b79feULL
#define IOUSBHostDevice_AbortDeviceRequests_ID            0xe0da9ed8caee6fbeULL
#define IOUSBHostDevice_SetConfiguration_ID            0xa67a186949eaf68cULL
#define IOUSBHostDevice_GetAddress_ID            0xb682fa0b4e80b1d3ULL
#define IOUSBHostDevice_GetSpeed_ID            0xa2715c4284f99e61ULL
#define IOUSBHostDevice_GetFrameNumber_ID            0xc660b158ed52fa93ULL
#define IOUSBHostDevice_GetPortStatus_ID            0xa04ec9e19b0a96b7ULL
#define IOUSBHostDevice_Reset_ID            0xd0fbe57a84270361ULL
#define IOUSBHostDevice_CreateInterfaceIterator_ID            0xa7d9f292889bbb05ULL
#define IOUSBHostDevice_DestroyInterfaceIterator_ID            0xc49102faaad2fb20ULL
#define IOUSBHostDevice_CopyInterface_ID            0xcdb581972196795fULL

#define IOUSBHostDevice__GetDescriptor_Args \
        uint8_t type, \
        uint8_t index, \
        uint16_t languageID, \
        uint8_t requestType, \
        uint8_t requestRecipient, \
        IOMemoryDescriptor * dataBuffer, \
        uint16_t * bytesTransferred

#define IOUSBHostDevice_Open_Args \
        IOService * forClient, \
        IOOptionBits options, \
        uintptr_t arg

#define IOUSBHostDevice_Close_Args \
        IOService * forClient, \
        IOOptionBits options

#define IOUSBHostDevice_CreateIOBuffer_Args \
        IOOptionBits options, \
        uint64_t capacity, \
        IOBufferMemoryDescriptor ** buffer

#define IOUSBHostDevice_CompleteAsyncDeviceRequest_Args \
        OSAction * action, \
        IOReturn status, \
        uint32_t bytesTransferred

#define IOUSBHostDevice_AsyncDeviceRequest_Args \
        IOService * forClient, \
        uint8_t bmRequestType, \
        uint8_t bRequest, \
        uint16_t wValue, \
        uint16_t wIndex, \
        uint16_t wLength, \
        IOMemoryDescriptor * dataBuffer, \
        OSAction * completion, \
        uint32_t completionTimeoutMs

#define IOUSBHostDevice_DeviceRequest_Args \
        IOService * forClient, \
        uint8_t bmRequestType, \
        uint8_t bRequest, \
        uint16_t wValue, \
        uint16_t wIndex, \
        uint16_t wLength, \
        IOMemoryDescriptor * dataBuffer, \
        uint16_t * bytesTransferred, \
        uint32_t completionTimeoutMs

#define IOUSBHostDevice_AbortDeviceRequests_Args \
        IOService * forClient, \
        IOOptionBits options, \
        IOReturn withError

#define IOUSBHostDevice_SetConfiguration_Args \
        uint8_t bConfigurationValue, \
        bool matchInterfaces

#define IOUSBHostDevice_GetAddress_Args \
        uint8_t * address

#define IOUSBHostDevice_GetSpeed_Args \
        uint8_t * speed

#define IOUSBHostDevice_GetFrameNumber_Args \
        uint64_t * frameNumber, \
        uint64_t * theTime

#define IOUSBHostDevice_GetPortStatus_Args \
        uint32_t * portStatus

#define IOUSBHostDevice_Reset_Args \


#define IOUSBHostDevice_CreateInterfaceIterator_Args \
        uintptr_t * ref

#define IOUSBHostDevice_DestroyInterfaceIterator_Args \
        uintptr_t ref

#define IOUSBHostDevice_CopyInterface_Args \
        uintptr_t ref, \
        IOUSBHostInterface ** interface

#define IOUSBHostDevice_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOUSBHostDevice * self, const IORPC rpc);\
\
    kern_return_t\
    _GetDescriptor(\
        uint8_t type,\
        uint8_t index,\
        uint16_t languageID,\
        uint8_t requestType,\
        uint8_t requestRecipient,\
        IOMemoryDescriptor * dataBuffer,\
        uint16_t * bytesTransferred,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Open(\
        IOService * forClient,\
        IOOptionBits options,\
        uintptr_t arg,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Close(\
        IOService * forClient,\
        IOOptionBits options,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateIOBuffer(\
        IOOptionBits options,\
        uint64_t capacity,\
        IOBufferMemoryDescriptor ** buffer,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    CompleteAsyncDeviceRequest(\
        OSAction * action,\
        IOReturn status,\
        uint32_t bytesTransferred,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    AsyncDeviceRequest(\
        IOService * forClient,\
        uint8_t bmRequestType,\
        uint8_t bRequest,\
        uint16_t wValue,\
        uint16_t wIndex,\
        uint16_t wLength,\
        IOMemoryDescriptor * dataBuffer,\
        OSAction * completion,\
        uint32_t completionTimeoutMs,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    DeviceRequest(\
        IOService * forClient,\
        uint8_t bmRequestType,\
        uint8_t bRequest,\
        uint16_t wValue,\
        uint16_t wIndex,\
        uint16_t wLength,\
        IOMemoryDescriptor * dataBuffer,\
        uint16_t * bytesTransferred,\
        uint32_t completionTimeoutMs,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    AbortDeviceRequests(\
        IOService * forClient,\
        IOOptionBits options,\
        IOReturn withError,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetConfiguration(\
        uint8_t bConfigurationValue,\
        bool matchInterfaces,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetAddress(\
        uint8_t * address,\
        OSDispatchMethod supermethod = NULL) const;\
\
    kern_return_t\
    GetSpeed(\
        uint8_t * speed,\
        OSDispatchMethod supermethod = NULL) const;\
\
    kern_return_t\
    GetFrameNumber(\
        uint64_t * frameNumber,\
        uint64_t * theTime,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetPortStatus(\
        uint32_t * portStatus,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Reset(\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateInterfaceIterator(\
        uintptr_t * ref,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    DestroyInterfaceIterator(\
        uintptr_t ref,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyInterface(\
        uintptr_t ref,\
        IOUSBHostInterface ** interface,\
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
    typedef kern_return_t (*_GetDescriptor_Handler)(OSMetaClassBase * target, IOUSBHostDevice__GetDescriptor_Args);\
    static kern_return_t\
    _GetDescriptor_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _GetDescriptor_Handler func);\
\
    typedef kern_return_t (*Open_Handler)(OSMetaClassBase * target, IOUSBHostDevice_Open_Args);\
    static kern_return_t\
    Open_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Open_Handler func);\
\
    typedef kern_return_t (*Close_Handler)(OSMetaClassBase * target, IOUSBHostDevice_Close_Args);\
    static kern_return_t\
    Close_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Close_Handler func);\
\
    typedef kern_return_t (*CreateIOBuffer_Handler)(OSMetaClassBase * target, IOUSBHostDevice_CreateIOBuffer_Args);\
    static kern_return_t\
    CreateIOBuffer_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CreateIOBuffer_Handler func);\
\
    typedef void (*CompleteAsyncDeviceRequest_Handler)(OSMetaClassBase * target, IOUSBHostDevice_CompleteAsyncDeviceRequest_Args);\
    static kern_return_t\
    CompleteAsyncDeviceRequest_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CompleteAsyncDeviceRequest_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    CompleteAsyncDeviceRequest_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CompleteAsyncDeviceRequest_Handler func);\
\
    typedef kern_return_t (*AsyncDeviceRequest_Handler)(OSMetaClassBase * target, IOUSBHostDevice_AsyncDeviceRequest_Args);\
    static kern_return_t\
    AsyncDeviceRequest_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        AsyncDeviceRequest_Handler func);\
\
    typedef kern_return_t (*DeviceRequest_Handler)(OSMetaClassBase * target, IOUSBHostDevice_DeviceRequest_Args);\
    static kern_return_t\
    DeviceRequest_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        DeviceRequest_Handler func);\
\
    typedef kern_return_t (*AbortDeviceRequests_Handler)(OSMetaClassBase * target, IOUSBHostDevice_AbortDeviceRequests_Args);\
    static kern_return_t\
    AbortDeviceRequests_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        AbortDeviceRequests_Handler func);\
\
    typedef kern_return_t (*SetConfiguration_Handler)(OSMetaClassBase * target, IOUSBHostDevice_SetConfiguration_Args);\
    static kern_return_t\
    SetConfiguration_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetConfiguration_Handler func);\
\
    typedef kern_return_t (*GetAddress_Handler)(OSMetaClassBase * target, IOUSBHostDevice_GetAddress_Args);\
    static kern_return_t\
    GetAddress_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetAddress_Handler func);\
\
    typedef kern_return_t (*GetSpeed_Handler)(OSMetaClassBase * target, IOUSBHostDevice_GetSpeed_Args);\
    static kern_return_t\
    GetSpeed_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetSpeed_Handler func);\
\
    typedef kern_return_t (*GetFrameNumber_Handler)(OSMetaClassBase * target, IOUSBHostDevice_GetFrameNumber_Args);\
    static kern_return_t\
    GetFrameNumber_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetFrameNumber_Handler func);\
\
    typedef kern_return_t (*GetPortStatus_Handler)(OSMetaClassBase * target, IOUSBHostDevice_GetPortStatus_Args);\
    static kern_return_t\
    GetPortStatus_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetPortStatus_Handler func);\
\
    typedef kern_return_t (*Reset_Handler)(OSMetaClassBase * targetIOUSBHostDevice_Reset_Args);\
    static kern_return_t\
    Reset_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Reset_Handler func);\
\
    typedef kern_return_t (*CreateInterfaceIterator_Handler)(OSMetaClassBase * target, IOUSBHostDevice_CreateInterfaceIterator_Args);\
    static kern_return_t\
    CreateInterfaceIterator_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CreateInterfaceIterator_Handler func);\
\
    typedef kern_return_t (*DestroyInterfaceIterator_Handler)(OSMetaClassBase * target, IOUSBHostDevice_DestroyInterfaceIterator_Args);\
    static kern_return_t\
    DestroyInterfaceIterator_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        DestroyInterfaceIterator_Handler func);\
\
    typedef kern_return_t (*CopyInterface_Handler)(OSMetaClassBase * target, IOUSBHostDevice_CopyInterface_Args);\
    static kern_return_t\
    CopyInterface_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyInterface_Handler func);\
\


#define IOUSBHostDevice_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    _GetDescriptor_Impl(IOUSBHostDevice__GetDescriptor_Args);\
\
    kern_return_t\
    Open_Impl(IOUSBHostDevice_Open_Args);\
\
    kern_return_t\
    Close_Impl(IOUSBHostDevice_Close_Args);\
\
    kern_return_t\
    CreateIOBuffer_Impl(IOUSBHostDevice_CreateIOBuffer_Args);\
\
    kern_return_t\
    AsyncDeviceRequest_Impl(IOUSBHostDevice_AsyncDeviceRequest_Args);\
\
    kern_return_t\
    DeviceRequest_Impl(IOUSBHostDevice_DeviceRequest_Args);\
\
    kern_return_t\
    AbortDeviceRequests_Impl(IOUSBHostDevice_AbortDeviceRequests_Args);\
\
    kern_return_t\
    SetConfiguration_Impl(IOUSBHostDevice_SetConfiguration_Args);\
\
    kern_return_t\
    GetAddress_Impl(IOUSBHostDevice_GetAddress_Args);\
\
    kern_return_t\
    GetSpeed_Impl(IOUSBHostDevice_GetSpeed_Args);\
\
    kern_return_t\
    GetFrameNumber_Impl(IOUSBHostDevice_GetFrameNumber_Args);\
\
    kern_return_t\
    GetPortStatus_Impl(IOUSBHostDevice_GetPortStatus_Args);\
\
    kern_return_t\
    Reset_Impl(IOUSBHostDevice_Reset_Args);\
\
    kern_return_t\
    CreateInterfaceIterator_Impl(IOUSBHostDevice_CreateInterfaceIterator_Args);\
\
    kern_return_t\
    DestroyInterfaceIterator_Impl(IOUSBHostDevice_DestroyInterfaceIterator_Args);\
\
    kern_return_t\
    CopyInterface_Impl(IOUSBHostDevice_CopyInterface_Args);\
\


#define IOUSBHostDevice_VirtualMethods \
\
public:\
\
    virtual kern_return_t\
    CopyDescriptor(\
        uint8_t type,\
        uint16_t * length,\
        uint8_t index,\
        uint16_t languageID,\
        uint8_t requestType,\
        uint8_t requestRecipient,\
        uint8_t * descriptor) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBDeviceDescriptor *\
    CopyDeviceDescriptor(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBConfigurationDescriptor *\
    CopyConfigurationDescriptor(\
        uint8_t index) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBConfigurationDescriptor *\
    CopyConfigurationDescriptorWithValue(\
        uint8_t bConfigurationValue) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBConfigurationDescriptor *\
    CopyConfigurationDescriptor(\
        IOService * forClient) APPLE_KEXT_OVERRIDE;\
\
    virtual const IOUSBBOSDescriptor *\
    CopyCapabilityDescriptors(\
) APPLE_KEXT_OVERRIDE;\
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


#if !KERNEL

extern OSMetaClass          * gIOUSBHostDeviceMetaClass;
extern const OSClassLoadInformation IOUSBHostDevice_Class;

class IOUSBHostDeviceMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class IOUSBHostDeviceInterface : public OSInterface
{
public:
    virtual kern_return_t
    CopyDescriptor(uint8_t type,
        uint16_t * length,
        uint8_t index,
        uint16_t languageID,
        uint8_t requestType,
        uint8_t requestRecipient,
        uint8_t * descriptor) = 0;

    virtual const IOUSBDeviceDescriptor *
    CopyDeviceDescriptor() = 0;

    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor(uint8_t index) = 0;

    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptorWithValue(uint8_t bConfigurationValue) = 0;

    virtual const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor(IOService * forClient) = 0;

    virtual const IOUSBBOSDescriptor *
    CopyCapabilityDescriptors() = 0;

    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor(uint8_t index,
        uint16_t languageID) = 0;

    virtual const IOUSBStringDescriptor *
    CopyStringDescriptor(uint8_t index) = 0;

    kern_return_t
    CopyDescriptor_Call(uint8_t type,
        uint16_t * length,
        uint8_t index,
        uint16_t languageID,
        uint8_t requestType,
        uint8_t requestRecipient,
        uint8_t * descriptor)  { return CopyDescriptor(type, length, index, languageID, requestType, requestRecipient, descriptor); };\

    const IOUSBDeviceDescriptor *
    CopyDeviceDescriptor_Call()  { return CopyDeviceDescriptor(); };\

    const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor_Call(uint8_t index)  { return CopyConfigurationDescriptor(index); };\

    const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptorWithValue_Call(uint8_t bConfigurationValue)  { return CopyConfigurationDescriptorWithValue(bConfigurationValue); };\

    const IOUSBConfigurationDescriptor *
    CopyConfigurationDescriptor_Call(IOService * forClient)  { return CopyConfigurationDescriptor(forClient); };\

    const IOUSBBOSDescriptor *
    CopyCapabilityDescriptors_Call()  { return CopyCapabilityDescriptors(); };\

    const IOUSBStringDescriptor *
    CopyStringDescriptor_Call(uint8_t index,
        uint16_t languageID)  { return CopyStringDescriptor(index, languageID); };\

    const IOUSBStringDescriptor *
    CopyStringDescriptor_Call(uint8_t index)  { return CopyStringDescriptor(index); };\

};

struct IOUSBHostDevice_IVars;
struct IOUSBHostDevice_LocalIVars;

class IOUSBHostDevice : public IOService, public IOUSBHostDeviceInterface
{
#if !KERNEL
    friend class IOUSBHostDeviceMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
#ifdef IOUSBHostDevice_DECLARE_IVARS
IOUSBHostDevice_DECLARE_IVARS
#else /* IOUSBHostDevice_DECLARE_IVARS */
    union
    {
        IOUSBHostDevice_IVars * ivars;
        IOUSBHostDevice_LocalIVars * lvars;
    };
#endif /* IOUSBHostDevice_DECLARE_IVARS */
#endif /* !KERNEL */

#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gIOUSBHostDeviceMetaClass; };
#endif /* KERNEL */

    using super = IOService;

#if !KERNEL
    IOUSBHostDevice_Methods
    IOUSBHostDevice_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#endif /* !__DOCUMENTATION__ */

/* IOUSBHostDevice.iig:378-379 */


/* IOUSBHostDevice.iig:392- */


#endif /* ! _IOKIT_IOUSBHOSTDEVICE_H */

