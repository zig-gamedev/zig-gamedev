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
 @header     IOUSBHostInterface.h
 @brief      IOUSBHostInterface is an IOService representing a USB interface.
 @discussion
 
 <h3>Session Management</h3>
 
 A driver that has successfully matched on an IOUSBHostInterface is able to take ownership of the interface by calling the <code>open</code> method defined by IOService on the IOUSBHostInterface.  Once <code>open</code> has completed successfully, the driver has an open session, and may use the deviceRequest interface to send control requests, the selectAlternateSetting interface to change the endpoints and behavior of the interface.
 
 When the driver is finished with control of the IOUSBHostInterface, it must call <code>close</code> to end its session.  Calling <code>close</code> will synchronously abort any IO associated with that session, including requests on the control endpoint.  If the interface is terminating for any reason, such as the device being unplugged, the driver must call <code>close</code> on the IOUSBHostInterface, or termination will be blocked and the port will not register newly attached devices for matching.  The willTerminate call into the driver is the recommended location to call <code>close</code>.
 
 <h3>deviceRequest Interface</h3>
 
 The <code>deviceRequest</code> methods are used to send control requests to the device's default endpoint.  The <code>deviceRequest</code> methods may not be used for the following standard requests:
 <ul>
    <li>CLEAR_FEATURE ENDPOINT_HALT (USB 2.0 9.4.1) - Use <code>IOUSBHostPipe::clearStall</code> to send this request</li>
    <li>SET_ADDRESS (USB 2.0 9.4.6)</li>
    <li>SET_CONFIGURATION (USB 2.0 9.4.7) - Use <code>setConfiguration</code> to send this request</li>
    <li>SET_INTERFACE (USB 2.0 9.4.10) - Use <code>IOUSBHostInterface::selectAlternateSetting</code> to send this request</li>
 </ul>
 
 <h3>IOUSBInterface Migration</h3>
 
 IOUSBHostInterface serves as a replacement for IOUSBInterface.  Clients that previously matched on IOUSBInterface can use the following guide to convert to IOUSBHostInterface.
 
 <code>virtual const IOUSBInterfaceDescriptor* IOUSBInterface::FindNextAltInterface(const IOUSBInterfaceDescriptor*, IOUSBFindInterfaceRequest*);</code><br>
 Replacement: <code>StandardUSB::getNextInterfaceDescriptor(...);</code>
 
 <code>virtual IOUSBPipe* IOUSBInterface::FindNextPipe(IOUSBHostPipe*, IOUSBFindEndpointRequest*);<br>
 virtual IOUSBPipe* IOUSBInterface::FindNextPipe(IOUSBHostPipe*, IOUSBFindEndpointRequest*, bool);</code><br>
 Replacement: 
 <pre>const EndpointDescriptor* endpointCandidate = NULL;
IOUSBHostPipe* foundPipe = NULL;
while((endpointCandidate = StandardUSB::getNextAssociatedDescriptorWithType(getConfigurationDescriptor(), getInterfaceDescriptor(), endpointCandidate, kDescriptorTypeEndpoint)) != NULL)
{
   if(endpointCandidate->bEndpointAddress == desiredAddress>)
   {
       foundPipe = copyPipe(StandardUSB::getEndpointAddress(endpointCandidate));
       break;
   }
}</pre>
 
 <code>virtual const IOUSBDescriptorHeader* IOUSBInterface::FindNextAssociatedDescriptor(const void*, UInt8);</code><br>
 Replacement: <code>StandardUSB::getNextAssociatedDescriptorWithType(getConfigurationDescriptor(), getInterfaceDescriptor(), currentDescriptor, type);</code>
 
 <code>virtual IOReturn IOUSBInterface::SetAlternateInterface(IOService*, UInt16);
 Replacement: <code>selectAlternateSetting(...);</code>
 
 <code>virtual IOUSBHostPipe* IOUSBInterface::GetPipeObj(UInt8);<br>
 virtual IOUSBHostPipe* IOUSBInterface::GetPipeObjRetain(UInt8);</code><br>
 Replacement: <code>copyPipe(endpointAddress);</code><br>
 Note that <code>copyPipe</code> behaves like <code>GetPipeObjRetain</code>, and the returned pipe must be released by the caller during termination.
 
 <code>virtual UInt8 IOUSBInterface::GetConfigValue();</code><br>
 Replacement: <code>getConfigurationDescriptor()->bConfigurationValue;</code>
 
 <code>virtual IOUSBHostDevice* IOUSBInterface::GetDevice();</code><br>
 Replacement: 
 <pre>// Function drivers should not assume the immediate provider of an IOUSBHostInterface is an IOUSBHostDevice.
IOService* deviceCandidate = getProvider();
IOUSBHostDevice* device = NULL;
while(deviceCandidate != NULL)
{
    device = OSDynamicCast(IOUSBHostDevice, deviceCandidate);
    if(device != NULL)
    {
        break;
    }
 
    deviceCandidate = deviceCandidate->getProvider();
}</pre>

 <code>virtual UInt8 IOUSBInterface::GetInterfaceNumber();</code><br>
 Replacement: <code>getInterfaceDescriptor()->bInterfaceNumber;</code>
 
 <code>virtual UInt8 IOUSBInterface::GetAlternateSetting();</code><br>
 Replacement: <code>getInterfaceDescriptor()->bAlternateSetting;</code>
 
 <code>virtual UInt8 IOUSBInterface::GetNumEndpoints();</code><br>
 Replacement: <code>getInterfaceDescriptor()->bNumEndpoints;</code>
 
 <code>virtual UInt8 IOUSBInterface::GetInterfaceClass();</code><br>
 Replacement: <code>getInterfaceDescriptor()->bInterfaceClass;</code>
 
 <code>virtual UInt8 IOUSBInterface::GetInterfaceSubClass();</code><br>
 Replacement: <code>getInterfaceDescriptor()->bInterfaceSubClass;</code>

 <code>virtual UInt8 IOUSBInterface::GetInterfaceProtocol();</code><br>
 Replacement: <code>getInterfaceDescriptor()->bInterfaceProtocol;</code>

 <code>virtual UInt8 IOUSBInterface::GetInterfaceStringIndex();</code><br>
 Replacement: <code>getInterfaceDescriptor()->iInterface;</code>
 
 <code>virtual IOReturn IOUSBInterface::DeviceRequest(IOUSBDevRequest* request, IOUSBCompletion* completion = 0);<br>
 virtual IOReturn IOUSBInterface::DeviceRequest(IOUSBDevRequestDesc* request, IOUSBCompletion* completion = 0);</code><br>
 Replacement: <code>deviceRequest(...)</code>
 
 <code>virtual IOReturn IOUSBInterface::GetEndpointProperties(UInt8 alternateSetting, UInt8 endpointNumber, UInt8 direction, UInt8* transferType, UInt16* maxPacketSize, UInt8* interval);<br>
 virtual IOReturn GetEndpointPropertiesV3(IOUSBEndpointProperties*);</code><br>
 Replacement: Use <code>StandardUSB::getEndpoint*</code> methods to retrieve endpoint properties.

 <code>virtual IOReturn RememberStreams();</code><br>
 Replacement: none

 <code>virtual IOReturn RecreateStreams();</code><br>
 Replacement: none

 <code>virtual void UnlinkPipes();</code><br>
 Replacement: none

 <code>virtual void ReopenPipes();</code><br>
 Replacement: none

 <code>virtual IOReturn GetInterfaceStatus(USBStatus*);</code><br>
 Replacement: Use <code>deviceRequest(...)</code> to manually craft the control request.

 <code>virtual IOReturn SetFunctionSuspendFeature(UInt8);</code><br>
 Replacement: It is not recommended for function drivers to send this request.

 <code>virtual IOReturn EnableRemoteWake(bool);</code><br>
 Replacement: Use <code>deviceRequest(...)</code> to manually craft the control request.  <code>IOUSBHostDevice::setConfiguration</code> will automatically enable remote wake if the descriptors indicate it is supported.
 */

#ifndef IOUSBHostFamily_IOUSBHostInterface_h
#define IOUSBHostFamily_IOUSBHostInterface_h

#include <TargetConditionals.h>
#include <IOKit/IOService.h>
#include <libkern/c++/OSData.h>

#include <IOKit/usb/IOUSBHostFamily.h>
#include <IOKit/usb/IOUSBHostDevice.h>

#define TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTINTERFACE __has_include(<USBDriverKit/IOUSBHostInterface.h>)

#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTINTERFACE
#include <USBDriverKit/IOUSBHostInterface.h>
#endif

#if TARGET_CPU_ARM64 || TARGET_CPU_ARM
/*!
* @class       IOUSBInterface
* @brief       A compatibility base class to faciliate service discovery for legacy projects.
* @discussion  New software should not reference this class.
*/
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBInterface : public IOService
{
    typedef IOService super;
    OSDeclareAbstractStructors(IOUSBInterface)
};
#endif

/*!
 * @class       IOUSBHostInterface
 * @brief       The IOService object representing a USB interface
 * @discussion  This class provides functionality to send control requests to the default control endpoint, as well as create IOUSBHostPipe objects to transfer data.  Function drivers should not subclass IOUSBHostInterface.
 */
#if TARGET_CPU_ARM64 || TARGET_CPU_ARM
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBHostInterface : public IOUSBInterface
#else
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBHostInterface : public IOService
#endif
{
#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTINTERFACE
    OSDeclareDefaultStructorsWithDispatch(IOUSBHostInterface)
#else
    OSDeclareDefaultStructors(IOUSBHostInterface)
#endif

public:
    /*
     * @brief       Factory method for creating an IOUSBHostInterface object
     * @discussion  This method should not be called by function drivers.  To create an IOUSBHostInterface, use IOUSBHostDevice::setConfiguration(...)
     * @param       configurationDescriptor Descriptor for the configuration this interface belongs to
     * @param       interfaceDescriptor Descriptor for this interface's default alt setting
     * @return      Pointer to an IOUSBHostInterface object if successful, otherwise NULL
     */
    static IOUSBHostInterface* withDescriptors(const StandardUSB::ConfigurationDescriptor* configurationDescriptor, const StandardUSB::InterfaceDescriptor* interfaceDescriptor);

protected:
    virtual bool initWithDescriptors(const StandardUSB::ConfigurationDescriptor* configurationDescriptor, const StandardUSB::InterfaceDescriptor* interfaceDescriptor);

    // Pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 0);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 1);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 2);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 3);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 4);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 5);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 6);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 7);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 8);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 9);

#pragma mark IOService overrides
public:
    // The following methods, with the notable exception of <code>open</code> and <code>close</code> should be considered private and should not be called by function drivers.  For a discussion of proper usage for <code>open</code> and <code>close</code>, see the discussion of "Session Management" above.
    virtual bool        attach(IOService* provider);
    virtual bool        start(IOService* provider);
    virtual bool        terminate(IOOptionBits options = 0);
    virtual void        stop(IOService* provider);
    virtual void        free();

    virtual bool compareProperty(OSDictionary* matching, const char* key);
    virtual bool compareProperty(OSDictionary* matching, const OSString* key);
    virtual bool matchPropertyTable(OSDictionary* table, SInt32* score);
    virtual bool matchPropertyTable(OSDictionary* table);


    enum
    {
        kOpenOptionsSelectAlternateInterface = kUSBHostOpenOptionSelectAlternateSetting
    } __attribute__((deprecated));

    /*! @functiongroup IOService overrides */

    /*!
     * @brief       Open a session to the IOUSBHostInterface
     * @discussion  This method opens a session to an IOUSBHostInterface.  It will acquire the service's workloop lock.  Only one service may open a session at a time.
     * @param       forClient The IOService that is opening a session.
     * @param       options See IOService.h, <code>kOpenOptionsSelectAlternateInterface</code> in the options mask will immediately select the alternate setting passed by value through the <code>arg</code> parameter
     * @param       arg See IOService.h, or the value of the alt setting to use if <code>kUSBHostOpenOptionSelectAlternateSetting</code> is included in the options mask
     * @return      bool true if the session could be opened, otherwise false.
     */
    virtual bool        open(IOService* forClient, IOOptionBits options = 0, void* arg = 0);
    
    /*!
     * @brief       Close a session to the IOUSBHostInterface
     * @discussion  This method closes an open session to an IOUSBHostInterface.  It will acquire the service's workloop lock, abort any IO for the interface and its endpoints, and may call commandSleep to allow processing of aborted IO before returning.
     * @param       forClient The IOService that is closing its session.
     * @param       options See IOService.h
     */
    virtual void        close(IOService* forClient, IOOptionBits options = 0);

    virtual IOReturn    newUserClient(task_t owningTask, void* securityID, UInt32 type, IOUserClient** handler);

    virtual IOReturn    message(UInt32 type, IOService* provider,  void* argument = 0);

    virtual const char* stringFromReturn(IOReturn code);

protected:
    virtual IOReturn openGated(IOService* forClient, IOOptionBits options, void* arg);
    virtual IOReturn closeGated(IOService* forClient, IOOptionBits options);
    
    // Pad slots for IOService overrides
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 10);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 11);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 12);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 13);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 14);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 15);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 16);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 17);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 18);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 19);
    
protected:
    IOCommandGate* _commandGate;
    IOWorkLoop*    _workLoop;

#pragma mark Power management
public:
    /*! @functiongroup Power management */

    /*!
     * @brief       Sets the desired idle suspend timeout for the interface
     * @discussion  Once the interface is considered idle, it will defer electrical suspend of the device for the specified duration.  For a more complete discussion of idle policies, refer to "Idle suspend" in IOUSBHostFamily.h.
     * @param       deviceIdleTimeout The amount of time, in milliseconds, after all pipes are idle to wait before suspending the device.
     * @return     IOReturn result code
     */
    virtual IOReturn setIdlePolicy(uint32_t deviceIdleTimeout);
    
    /*!
     * @brief       Retrieve the current idle suspend timeout.  See @link setIdlePolicy @/link
     * @return      The amount of time, in milliseconds, after all pipes are idle to wait before suspending the device,
     */
    virtual uint32_t getIdlePolicy();
    
    // Public pad slots for power management
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 20);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 21);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 22);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 23);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 24);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 25);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 26);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 27);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 28);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 29);
    
protected:
    // Protected pad slots for power management
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 30);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 31);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 32);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 33);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 34);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 35);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 36);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 37);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 38);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 39);

protected:
    uint32_t _deviceIdleTimeout;
    
#pragma mark Descriptors
public:
    /*! @functiongroup Descriptors */

    /*!
     * @brief       Retrieve the configuration descriptor in which this interface is defined
     * @return      ConfigurationDescriptor pointer
     */
    virtual const StandardUSB::ConfigurationDescriptor* getConfigurationDescriptor();
    
    /*!
     * @brief       Retrieve the interface descriptor associated with this interface
     * @return      InterfaceDescriptor pointer
     */
    virtual const StandardUSB::InterfaceDescriptor* getInterfaceDescriptor();
    
    /*!
     * @brief       Return a string descriptor from the device
     * @discussion  This method uses IOUSBHostDevice::getDescriptor to return a string descriptor.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       languageID Descriptor language ID.  <code>wIndex</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).     *
     * @return      Pointer to the descriptor if found
     */
    virtual const StandardUSB::StringDescriptor* getStringDescriptor(uint8_t index, uint16_t languageID = StandardUSB::kLanguageIDEnglishUS);
    
    // Public pad slots for descriptors
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 40);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 41);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 42);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 43);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 44);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 45);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 46);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 47);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 48);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 49);
    
protected:
    virtual IOReturn getInterfaceDescriptorGated(const StandardUSB::InterfaceDescriptor*& result);
    
    // Protected pad slots for descriptors
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 50);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 51);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 52);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 53);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 54);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 55);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 56);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 57);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 58);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 59);
    
#pragma mark Alternate setting and pipe management
public:
    /*! @functiongroup Alternate setting and pipe management */

    /*!
     * @brief       Select an alternate setting for this interface
     * @discussion  This method is used to select an alternate setting for the interface.  All pending IO on the interface's pipes will be aborted, and the open pipes will be closed.  The new alternate setting will be selected via SET_INTERFACE control request (USB 2.0 9.4.10).
     * @param       bAlternateSetting Alternate interface number to activate
     * @return      IOReturn result code
     */
    virtual IOReturn selectAlternateSetting(uint8_t bAlternateSetting);
    
    /*!
     * @brief       Return the pipe whose <code>bEndpointAddress</code> matches <code>address</code>
     * @discussion  This method will return the pipe whose <code>bEndpointAddress</code> matches <code>address</code>.  If the pipe doesn't exist yet, but is part of the interface, it will first be created.  The caller must release the IOUSBHostPipe when finished using it.
     * @param       address Address of the pipe
     * @return      Pointer to a retain()ed IOUSBHostPipe object or NULL
     */
    virtual IOUSBHostPipe* copyPipe(uint8_t address);
    
    /*!
     * @brief       Enqueue a request on the default control endpoint
     * @discussion  This method will enqueue an asynchronous request on the default control endpoint.  If successful, the provided completion routine will be called to report the status of the completed IO.
     * @param       request Reference to a valid StandardUSB::DeviceRequest structure.  The structure is copied and can therefore be stack-allocated.
     * @param       dataBuffer A void* or IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       completion Pointer to a IOUSBHostCompletion structure.  This will be copied and can therefore be stack-allocated.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      kIOReuturnSuccess if the completion will be called in the future, otherwise error
     */
    virtual IOReturn deviceRequest(StandardUSB::DeviceRequest& request, void* dataBuffer, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);
    virtual IOReturn deviceRequest(StandardUSB::DeviceRequest& request, IOMemoryDescriptor* dataBuffer, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);
    
    /*!
     * @brief       Send a request on the default control endpoint
     * @discussion  This method will send a synchronous request on the default control endpoint, and will not return until the request is complete.  This method will acquire the service's workloop lock, and will call commandSleep to send the control request.
     * @param       request Reference to a valid StandardUSB::DeviceRequest structure.
     * @param       dataBuffer A void* or IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       bytesTransferred A uint32_t reference which will be updated with the byte count of the completed data phase.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      IOReturn value indicating the result of the IO request
     */
    virtual IOReturn deviceRequest(StandardUSB::DeviceRequest& request, void* dataBuffer, uint32_t& bytesTransferred, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);
    virtual IOReturn deviceRequest(StandardUSB::DeviceRequest& request, IOMemoryDescriptor* dataBuffer, uint32_t& bytesTransferred, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);
    
    /*!
     * @brief       Abort device requests made via the @link deviceRequest @/link methods by <code>forClient</code>
     * @discussion  This method will abort any requests associated with a specific client made via the @link deviceRequest @/link methods.  It will not abort requests made by other clients.
     * @param       options IOUSBHostIOSource::tAbortOptions
     * @param       withError IOReturn error value to return with the requests.  The default value of kIOReturnAborted should be used.
     * @return      IOReturn result code
     */
    IOReturn abortDeviceRequests(IOOptionBits options = IOUSBHostIOSource::kAbortAsynchronous, IOReturn withError = kIOReturnAborted);
    
    // Public pad slots for alternate setting and pipe management
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 60);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 61);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 62);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 63);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 64);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 65);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 66);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 67);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 68);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 69);

protected:
    enum
    {
        kInitIORetries       = 3,
        kInitIORetryInterval = 100,
        kDestroyPipesOptionAll = StandardUSBBit(0)
    };
    
    virtual void updateMatchingProperties();
    
    virtual IOReturn selectAlternateSettingGated(uint8_t bAlternateSetting);
    
    virtual IOReturn copyPipeGated(uint8_t address, IOUSBHostPipe*& result);

    virtual void closePipes();
    virtual IOReturn closePipesGated();

    // Must be called on the command gate
    virtual void pipeLockLock();
    virtual void pipeLockUnlock();


    OSMetaClassDeclareReservedUsed(IOUSBHostInterface, 70);
    virtual void destroyPipes(IOOptionBits options = 0);
    
    OSMetaClassDeclareReservedUsed(IOUSBHostInterface, 71);
    virtual IOReturn destroyPipesGated(IOOptionBits options);

    // Protected pad slots for alternate setting and pipe management
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 72);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 73);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 74);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 75);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 76);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 77);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 78);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 79);
    
protected:
    IOUSBHostPipe*                      _pipeList[kUSBHostMaxPipes];
    
    bool _pipeLock;

    const StandardUSB::ConfigurationDescriptor* _configurationDescriptor;
    const StandardUSB::InterfaceDescriptor*     _interfaceDescriptor;
    IOUSBHostDevice*                            _device;
    
    uint32_t _debugLoggingMask;
    uint32_t _locationID;           // TODO: Initialize this for tracepoints

#pragma mark Miscellaneous
public:
    /*! @functiongroup Provider access */

    /*!
     * @brief       Return the parent/provider IOUSBHostDevice object of this interface.
     * @return      IOUSBHostDevice pointer
     */
    virtual IOUSBHostDevice* getDevice() const;
    
    /*!
     * @brief       Return the current frame number of the USB controller
     * @description This method will return the current frame number of the USB controller, omitting microframe.  This is most useful for scheduling future isochronous requests.
     * @param       theTime If not NULL, this will be updated with the current system time
     * @return      The current frame number
     */
    virtual uint64_t getFrameNumber(AbsoluteTime* theTime = NULL) const;
    
    /*!
     * @brief       Return the current port status
     * @discussion  Combination of tUSBHostPortStatus values
     * @return      port status
     */
    virtual uint32_t getPortStatus() const;

    /*!
     * @brief       Allocate a buffer to be used for I/O
     * @discussion  This method will allocate an IOBufferMemoryDescriptor optimized for use by the underlying controller hardware.  A buffer allocated by this method will not be bounced to perform DMA operations.
     * @param       options kIODirectionOut, kIODirectionIn to set the direction of the I/O transfer.
     * @param       capacity Size of the buffer to allocate
     * @return      Pointer to an IOBufferMemoryDescriptor if successful, otherwise NULL
     */
    virtual IOBufferMemoryDescriptor* createIOBuffer(IOOptionBits options, mach_vm_size_t capacity);
    
    // Public pad slots for miscellaneous
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 80);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 81);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 82);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 83);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 84);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 85);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 86);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 87);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 88);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 89);

protected:
    // Protected pad slots for miscellaneous
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 90);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 91);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 92);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 93);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 94);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 95);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 96);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 97);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 98);
    OSMetaClassDeclareReservedUnused(IOUSBHostInterface, 99);

protected:
    struct tExpansionData
    {
        uint16_t _persistentOutEndpointMask;
        uint16_t _persistentInEndpointMask;
        bool     _allPipesDestroyed;
    };
    tExpansionData* _expansionData;
};

#endif // IOUSBHostFamily_IOUSBHostInterface_h
