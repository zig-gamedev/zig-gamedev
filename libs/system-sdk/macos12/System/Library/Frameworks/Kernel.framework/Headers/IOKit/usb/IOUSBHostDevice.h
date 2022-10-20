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
 @header     IOUSBHostDevice.h
 @brief      IOUSBHostDevice is an IOService representing a USB device.
 @discussion 
 
 <h3>Session Management</h3>
 
 A driver that has successfully matched on an IOUSBHostDevice is able to take ownership of the device by calling the <code>open</code> method defined by IOService on the IOUSBHostDevice.  Once <code>open</code> has completed successfully, the driver has an open session, and may use the deviceRequest interface to send control requests, and the setConfiguration interface to create or destroy child IOUSBHostInterface services.
 
 When the driver is finished with control of the IOUSBHostDevice, it must call <code>close</code> to end its session.  Calling <code>close</code> will synchronously abort that session's outstanding IO on the control endpoint.  If the device is terminating for any reason, such as being unplugged, the driver must call <code>close</code> on the IOUSBHostDevice, or termination will be blocked and the port will not register newly attached devices for matching.  The willTerminate call into the driver is the recommended location to call <code>close</code>.
 
 Drivers that match on an IOUSBHostInterface service do not need to open a session to the IOUSBHostDevice, as the IOUSBHostInterface will open a session to the IOUSBHostDevice on the driver's behalf.
 
 <h3>deviceRequest Interface</h3>
 
 The <code>deviceRequest</code> methods are used to send control requests to the device's default endpoint.  The <code>deviceRequest</code> methods may not be used for the following standard requests:
 <ul>
    <li>CLEAR_FEATURE ENDPOINT_HALT (USB 2.0 9.4.1) - Use <code>IOUSBHostPipe::clearStall</code> to send this request</li>
    <li>SET_ADDRESS (USB 2.0 9.4.6)</li>
    <li>SET_CONFIGURATION (USB 2.0 9.4.7) - Use <code>setConfiguration</code> to send this request</li>
    <li>SET_INTERFACE (USB 2.0 9.4.10) - Use <code>IOUSBHostInterface::selectAlternateSetting</code> to send this request</li>
 </ul>
 
 <h3>IOUSBDevice Migration</h3>
 
 IOUSBHostDevice serves as a replacement for IOUSBDevice.  Clients that previously matched on IOUSBDevice can use the following guide to convert to IOUSBHostDevice.
 
 <code>virtual UInt16 IOUSBDevice::GetbcdUSB();</code><br>
 Replacement: <code>USBToHost16(getDeviceDescriptor()->bcdUSB);</code>
 
 <code>virtual UInt8 IOUSBDevice::GetDeviceClass();</code><br>
 Replacement: <code>getDeviceDescriptor()->bDeviceClass;</code>
 
 <code>virtual UInt8 IOUSBDevice::GetDeviceSubClass();</code><br>
 Replacement: <code>getDeviceDescriptor()->bDeviceSubClass</code>
 
 <code>virtual UInt8 IOUSBDevice::GetProtocol();</code><br>
 Replacement: <code>getDeviceDescriptor()->bDeviceProtocol</code>
 
 <code>virtual UInt8 IOUSBDevice::GetMaxPacketSize();</code><br>
 Replacement: <code>getDeviceDescriptor->bMaxPacketSize0</code>
 
 <code>virtual UInt16 IOUSBDevice::GetVendorID();</code><br>
 Replacement: <code>USBToHost16(getDeviceDescriptor()->idVendor)</code>
 
 <code>virtual UInt16 IOUSBDevice::GetProductID();</code><br>
 Replacement: <code>USBToHost16(getDeviceDescriptor()->idProduct)</code>
 
 <code>virtual UInt16 IOUSBDevice::GetDeviceRelease();</code><br>
 Replacement: <code>USBToHost16(getDeviceDescriptor()->bcdDevice)</code>
 
 <code>virtual UInt8 IOUSBDevice::GetNumConfigurations();</code><br>
 Replacement: <code>getDeviceDescriptor()->bNumConfigurations</code>
 
 <code>virtual UInt8 IOUSBDevice::GetManufacturerStringIndex();</code><br>
 Replacement: <code>getDeviceDescriptor()->iManufacturer</code>
 
 <code>virtual UInt8 IOUSBDevice::GetProductStringIndex();</code><br>
 Replacement: <code>getDeviceDescriptor()->iProduct</code>
 
 <code>virtual UInt8 IOUSBDevice::GetSerialNumberStringIndex();</code><br>
 Replacement: <code>getDeviceDescriptor()->iSerialNumber</code>
 
 <code>virtual IOReturn IOUSBDevice::FindNextInterfaceDescriptor(const IOUSBConfigurationDescriptor*, const IOUSBInterfaceDescriptor*, const IOUSBFindInterfaceRequest*, IOUSBInterfaceDescriptor**);</code><br>
 Replacement:<br>
 <pre>ConfigurationDescriptor* confDesc = getConfigurationDescriptor();
InterfaceDescriptor* intDesc = NULL;
while((intDesc = StandardUSB::getNextInterfaceDescriptor(confDesc, intDesc)) != NULL)
{
    if(intDesc->bInterfaceClass == kMyClass)
    {
        break;
    }
}</pre>
 
 <code>virtual const IOUSBConfigurationDescriptor* IOUSBDevice::GetCachedConfigurationDescriptor(UInt8 configIndex);</code><br>
 <code>virtual const IOUSBConfigurationDescriptor* IOUSBDevice::GetFullConfigurationDescriptor(UInt8 configIndex);</code><br>
 Replacement: <code>getConfigurationDescriptor(configIndex)</code>
 
 <code>virtual IOReturn IOUSBDevice::GetConfigurationDescriptor(UInt8 configValue, void* data, UInt32 len);</code><br>
 Replacement: <code>getConfigurationDescriptorWithValue(configValue);</code>
 
 <code>virtual IOUSBHostInterface* IOUSBDevice::FindNextInterface(IOUSBHostInterface*, IOUSBFindInterfaceRequest* request);</code><br>
 <code>virtual OSIterator* IOUSBDevice::CreateInterfaceIterator(IOUSBFindInterfaceRequest* request);</code><br>
 Replacement:<br>
 <pre>OSIterator* iterator = _device->getChildIterator(gIOServicePlane);
OSObject* candidate = NULL;
while(iterator != NULL && (candidate = iterator->getNextObject()) != NULL)
{
    IOUSBHostInterface* interfaceCandidate = OSDynamicCast(IOUSBHostInterface, candidate);
    if(   interfaceCandidate != NULL
       && interfaceCandidate->getInterfaceDescriptor()->bInterfaceClass == kMyClass)
    {
        _interface = interfaceCandidate;
        break;
    }
}
OSSafeReleaseNULL(iterator);</pre>
 
 <code>virtual IOReturn IOUSBDevice::SetConfiguration(IOService*, UInt8, bool);</code><br>
 Replacement: <code>setConfiguration(configNumber, startMatchingInterfaces);</code>
 
 <code>virtual UInt8 IOUSBDevice::GetSpeed();</code><br>
 Replacement: <code>getSpeed();</code>
 
 <code>virtual IOUSBController* IOUSBDevice::GetBus();</code><br>
 Replacement: The controller cannot be directly accessed.  Use <code>getFrameNumber(...)</code> and <code>createIOBuffer(...)</code> instead.
 
 <code>virtual UInt32 IOUSBDevice::GetBusPowerAvailable();</code><br>
 Replacement: <code>allocateDownstreamBusCurrent(...);</code>
 
 <code>virtual IOReturn IOUSBDevice::DeviceRequest(IOUSBDevRequest*, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBDevice::DeviceRequest(IOUSBDevRequestDesc*, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBDevice::DeviceRequest(IOUSBDevRequest*, UInt32, UInt32, IOUSBCompletion*);<br>
 virtual IOReturn IOUSBDevice::DeviceRequest(IOUSBDevRequestDesc*, UInt32, UInt32, IOUSBCompletion*);<br></code>
 Replacment: <code>deviceRequest(...);</code>
 
 <code>virtual IOReturn IOUSBDevice::GetConfiguration(UInt8*);</code><br>
 Replacment:
 <pre>uint8_t configNumber  = 0;
StandardUSB::DeviceRequest request;
request.bmRequestType = makeDeviceRequestbmRequestType(kRequestDirectionIn, kRequestTypeStandard, kRequestRecipientDevice);
request.bRequest      = kDeviceRequestGetConfiguration;
request.wValue        = 0;
request.wIndex        = 0;
request.wLength       = sizeof(configNumber);
 
uint32_t bytesTransferred = 0;
 
deviceRequest(this, request, &configNumber, bytesTransferred, kUSBHostStandardRequestCompletionTimeout);</pre>
 
 <code>virtual IOReturn IOUSBDevice::GetDeviceStatus(USBStatus*);</code><br>
 Replacement:
 <pre>uint16_t status       = 0;
StandardUSB::DeviceRequest request;
request.bmRequestType = makeDeviceRequestbmRequestType(kRequestDirectionIn, kRequestTypeStandard, kRequestRecipientDevice);
request.bRequest      = kDeviceRequestGetStatus;
request.wValue        = 0;
request.wIndex        = 0;
request.wLength       = sizeof(status);
 
uint32_t bytesTransferred = 0;
 
deviceRequest(this, request, &status, bytesTransferred, kUSBHostStandardRequestCompletionTimeout);</pre>
 
 <code>virtual IOUSBPipe* IOUSBDevice::GetPipeZero();</code><br>
 Replacement: The IOUSBHostPipe representing the default control endpoint is not accessible.  Use <code>deviceRequest(...)</code> and <code>abortDeviceRequests(...)</code> interfaces to interact with the default control endpoint.
 
 <code>virtual IOReturn IOUSBDevice::GetStringDescriptor(UInt8, char*, int, UInt16);</code><br>
 Replacement:
 <pre>char stringBuffer[256] = { 0 };
size_t stringLength = sizeof(stringBuffer);
const StringDescriptor* stringDescriptor = getStringDescriptor(index);
if(   stringDescriptor != NULL
   && stringDescriptor->bLength > StandardUSB::kDescriptorSize)
{
   StandardUB::stringDescriptorToUTF8(stringDescriptor, stringBuffer, stringLength);
}</pre>
 
 <code>virtual const IOUSBDescriptorHeader* IOUSBDevice::FindNextDescriptor(const void*, UInt8);</code><br>
 Replacement: <code>StandardUSB::getNextDescriptorWithType(getConfigurationDescriptor(), currentDescriptor, descriptorType);</code>
 
 <code>virtual IOReturn IOUSBDevice::SuspendDevice(bool);</code><br>
 Replacement: Power management should be done using the idling system described in IOUSBHostFamily.h.
 
 <code>virtual IOReturn IOUSBDevice::ReEnumerateDevice(UInt32);</code><br>
 Replacement: <code>reset();</code>
 
 <code>virtual IOReturn IOUSBDevice::GetDeviceInformation(UInt32*);</code><br>
 Replacement: <code>getPortStatus();</code>
 
 <code>virtual UInt32 IOUSBDevice::RequestExtraPower(UInt32, UInt32);<br>
 virtual IOReturn IOUSBDevice::ReturnExtraPower(UInt32, UInt32);</code><br>
 Replacement: <code>allocateDownstreamBusCurrent(...);</code>
 
 <code>virtual tUSBHostDeviceAddress IOUSBDevice::GetAddress(void);</code><br>
 Replacement: <code>getAddress();</code>
 
 <code>virtual IOReturn IOUSBDevice::ResetDevice();<br>
 virtual UInt32 IOUSBDevice::GetExtraPowerAllocated(UInt32);<br>
 void IOUSBDevice::SetBusPowerAvailable(UInt32);</code><br>
 Replacement: none
 */

#ifndef IOUSBHostFamily_IOUSBHostDevice_h
#define IOUSBHostFamily_IOUSBHostDevice_h

#include <TargetConditionals.h>
#include <IOKit/usb/StandardUSB.h>
#include <IOKit/usb/IOUSBHostFamily.h>
#include <IOKit/usb/IOUSBHostPipe.h>
#include <IOKit/IOBufferMemoryDescriptor.h>

#define TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTDEVICE __has_include(<USBDriverKit/IOUSBHostDevice.h>)

#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTDEVICE
#include <USBDriverKit/IOUSBHostDevice.h>
#endif

#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/pwr_mgt/RootDomain.h>
#include <kern/thread_call.h>

#define kUSBHostDeviceForceSuspend "kUSBHostDeviceForceSuspend"

class AppleUSBHostController;
class IOUSBHostInterface;
class AppleUSBHostDeviceIdler;
class AppleUSBHostRequestCompleter;
class AppleUSBHostPort;
class AppleUSBDescriptorCache;
class AppleUSBHostResources;

#if TARGET_CPU_ARM64 || TARGET_CPU_ARM
/*!
* @class       IOUSBDevice
* @brief       A compatibility base class to faciliate service discovery for legacy projects.
* @discussion  New software should not reference this class.
*/
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBDevice : public IOService
{
    typedef IOService super;
    OSDeclareAbstractStructors(IOUSBDevice)
    
public:
    virtual bool start(IOService* provider);
    
    virtual void stop(IOService* provider);
    
protected:
    static const IORegistryPlane* gIOUSBPlane;
};
#endif

/*!
* @class       IOUSBHostDevice
* @brief       The IOService object representing a USB device
* @discussion  This class provides functionality to send control requests to the default control endpoint, as well as create IOUSBHostInterface objects to represent the interfaces contained in the selected configuration.  Function drivers should not subclass IOUSBHostDevice.
*/
#if TARGET_CPU_ARM64 || TARGET_CPU_ARM
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBHostDevice : public IOUSBDevice
#else
class __IOUSBHOSTFAMILY_DEPRECATED IOUSBHostDevice : public IOService
#endif
{
    friend class AppleUSBHostController;
    friend class IOUSBHostInterface;
    friend class IOUSBHostIOSource;
    friend class IOUSBHostPipe;
    friend class IOUSBHostStream;
    friend class AppleUSBIORequest;
    friend class AppleUSBHostDeviceIdler;

#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTDEVICE
    OSDeclareDefaultStructorsWithDispatch(IOUSBHostDevice)
#else
    OSDeclareDefaultStructors(IOUSBHostDevice)
#endif

public:
    /*!
     * @namespace   IOUSBHostDevice
     * @brief       Powerstates supported by an IOUSBHostDevice
     * @constant    kPowerStateOff The device has been powered off and will be terminated.
     * @constant    kPowerStateSuspended The device is suspended.  All I/O has been paused and the bus will be suspended once its AppleUSBHostPort power parent enters the suspend state.
     * @constant    kPowerStateOn The device is powered on and I/O is possible.
     * @constant    kPowerStateCount Number of possible power states.
     */
    enum tPowerState
    {
        kPowerStateOff = 0,
        kPowerStateSuspended,
        kPowerStateOn,
        kPowerStateCount
    };

    /*
     * @brief       Factory method for creating an IOUSBHostDevice object
     * @discussion  This method should not be called by function drivers.  IOUSBHostDevice objects are created by AppleUSBHostPort services when a device is connected.
     * @param       controller Controller to which the USB device is attached
     * @param       speed Speed at which the device enumerated
     * @param       address tUSBHostDeviceAddress that has been allocated by the controller
     * @return      Pointer to an IOUSBHostDevice object if successful, otherwise NULL
     */
    static IOUSBHostDevice* withController(AppleUSBHostController* controller, UInt8 speed, tUSBHostDeviceAddress address);

protected:
    virtual bool initWithController(AppleUSBHostController* controller, UInt8 speed, tUSBHostDeviceAddress address);
    
    // Pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 0);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 1);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 2);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 3);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 4);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 5);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 6);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 7);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 8);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 9);
    
#pragma mark IOService overrides
public:
    /*!
     * @functiongroup   IOService overrides
     * @discussion      For a discussion of proper usage for <code>open</code> and <code>close</code>, see the discussion of "Session Management" above.
     */

    virtual bool attach(IOService* provider);
    virtual bool start(IOService* provider);
    virtual bool terminate(IOOptionBits options = 0);
    virtual void stop(IOService* provider);
    virtual void free(void);

    virtual bool compareProperty(OSDictionary* matching, const char* key);
    virtual bool compareProperty(OSDictionary* matching, const OSString* key);
    virtual bool matchPropertyTable(OSDictionary* table, SInt32* score);
    virtual bool matchPropertyTable(OSDictionary* table);

    virtual IOReturn setProperties(OSObject* properties);

    /*!
     * @brief       Open a session to the IOUSBHostDevice
     * @discussion  This method opens a session to an IOUSBHostDevice.  It will acquire the service's workloop lock.  Child IOUSBHostInterfaces may open simultaneous sessions, but only one other service may open a session.
     * @param       forClient The IOService that is opening a session.
     * @param       options See IOService.h
     * @param       arg See IOService.h
     * @return      bool true if the session could be opened, otherwise false.
     */
    virtual bool open(IOService* forClient, IOOptionBits options = 0, void* arg = 0);
    virtual bool handleOpen(IOService* forClient, IOOptionBits options, void* arg);
    virtual bool handleIsOpen(const IOService* forClient) const;
    
    /*!
     * @brief       Close a session to the IOUSBHostDevice
     * @discussion  This method closes an open session to an IOUSBHostDevice.  It will acquire the service's workloop lock, abort any IO associated with the specified client, and may call commandSleep to allow processing of aborted control requests before returning.
     * @param       forClient The IOService that is closing its session.  Any IO associated with the specified client will be aborted.
     * @param       options See IOService.h
     */
    virtual void close(IOService* forClient, IOOptionBits options = 0);
    virtual void handleClose(IOService* forClient, IOOptionBits options);

    virtual IOReturn newUserClient(task_t owningTask, void* securityID, UInt32 type, IOUserClient** handler);

    virtual IOReturn message(UInt32 type, IOService* provider,  void* argument = 0);

    virtual const char* stringFromReturn(IOReturn code);

    virtual bool setProperty(const OSSymbol* aKey, OSObject*   anObject);
    virtual bool setProperty(const OSString* aKey, OSObject*   anObject);
    virtual bool setProperty(const char*     aKey, OSObject*   anObject);
    virtual bool setProperty(const char*     aKey, const char* aString);
    virtual bool setProperty(const char*     aKey, bool        aBoolean);

    virtual bool setProperty(const char* aKey, unsigned long long aValue, unsigned int aNumberOfBits);
    virtual bool setProperty(const char* aKey, void*      	      bytes,  unsigned int length);

protected:
    virtual IOReturn terminateGated(IOOptionBits options = 0);
    
    virtual IOReturn openGated(IOService* forClient, IOOptionBits options, void* arg);
    virtual IOReturn closeGated(IOService* forClient, IOOptionBits options);

    // Protected pad slots for IOService overrides
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 10);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 11);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 12);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 13);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 14);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 15);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 16);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 17);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 18);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 19);
    
protected:
    int32_t                                  _terminationPending;
    AppleUSBHostResources*                   _resources;
    AppleUSBHostController*                  _controller;
    AppleUSBHostPort*                        _port;
    OSSet*                                   _clients;
    IOWorkLoop*                              _workLoop;
    IOCommandGate*                           _commandGate;
    
#pragma mark Power management
public:
    /*!
     * @functiongroup Power management
     */

    virtual void          registerPowerService();
    virtual IOReturn      addPowerChild(IOService* theChild);
    virtual IOReturn      removePowerChild(IOPowerConnection* theChild);
    virtual IOReturn      powerStateWillChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice);
    virtual IOReturn      setPowerState(unsigned long stateNumber, IOService* whatDevice);
    virtual IOReturn      powerStateDidChangeTo(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice);
    virtual unsigned long initialPowerStateForDomainState(IOPMPowerFlags domainState);
    virtual void          PMstop(void);
    virtual IOReturn      forcePower(tPowerState powerState, bool clamp, uint32_t timeoutMs = 0);
    
    /*!
     * @brief       Allocate bus current that does not match the bMaxPower value specified by the device's configuration descriptor
     * @discussion 
     * Devices can use this interface to request more or less bus current than what is specified in the selected configuration descriptor's bMaxPower field.  The parameters passed in are abolute values of current required for operation in kPowerStateOn and kPowerStateSuspended IOUSBHostDevice power levels, and override the bMaxPower field in the configuration descriptor.  For example, if bMaxPower indicates the device requires 500mA to operate, then a request for 1000mA will double the allocated power for the device.
     * A setConfiguration call to select a different configuration or unconfigure the device will reset the bus current allocations.
     * The parameters are passed by reference, and may be modified before being returned.  If more bus current is being requested than is available, the values may be reduced to stay within system limits.
     * @param       wakeUnits As input, the number of mA requested when the device is in kPowerStateOn.  As output, the number of mA allocated to the device in kPowerStateOn.
     * @param       sleepUnits As input, the number of mA requested when the device is in kPowerStateSuspended.  As output, the number of mA allocated to the device in kPowerStateSuspended.
     * @return      IOReturn kIOReturnSuccess if the device's allocation amounts were updated, otherwise an error code
     */
    virtual IOReturn allocateDownstreamBusCurrent(uint32_t& wakeUnits, uint32_t& sleepUnits);
    
    enum tIdleAssertion
    {
        kIdleAssertionInhibit = 0,
        kIdleAssertionPermit
    };
    /*!
     * @brief       This method should be considered private and should not be called by function drivers.
     */
    virtual IOReturn idleAssertion(IOService* forService, tIdleAssertion assertion);

    // Public pad slots for power management
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 20);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 21);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 22);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 23);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 24);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 25);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 26);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 27);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 28);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 29);

protected:
    static IOPMPowerState  _sPowerStates[kPowerStateCount];
    static const OSSymbol* _sPowerStateSymbols[kPowerStateCount];
    
    virtual void     powerChangeDone(unsigned long stateNumber);
    virtual IOReturn powerStateWillChangeToGated(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice);
    virtual IOReturn setPowerStateGated(unsigned long stateNumber, IOService* whatDevice);
    virtual IOReturn powerStateDidChangeToGated(IOPMPowerFlags capabilities, unsigned long stateNumber, IOService* whatDevice);
    virtual IOReturn forcePowerGated(tPowerState powerState, bool clamp, uint32_t timeoutMs);
    virtual void     addPowerChildThreadCall(thread_call_t threadCall);
    virtual IOReturn addPowerChildGated(void);

    virtual void     pmStopThreadCall(thread_call_t threadCall);

    virtual IOReturn allocateDownstreamBusCurrentGated(uint32_t& wakeUnits, uint32_t& sleepUnits);

    virtual IOReturn updateIdlePolicy(bool asynchronous = false);
    virtual void updateIdlePolicyAsync(thread_call_t threadCall);
    virtual IOReturn updateIdlePolicyGated();

    // Protected pad slots for power management
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 30);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 31);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 32);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 33);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 34);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 35);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 36);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 37);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 38);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 39);
    
protected:
    bool                    _allowUpdateIdlePolicy;
    bool                    _forcePowerInProgress;
    IOLock*                 _powerChildrenLock;
    OSSet*                  _powerChildren;
    OSSet*                  _activePowerChildren;
    IOPMDriverAssertionID   _externalDevicePowerAssertion;
    tPowerState             _currentPowerState;
    bool                    _powerOffPending;

#pragma mark Descriptors
public:
    /*! @functiongroup Descriptors */

    /*!
     * @brief       Store the provided descriptor in the set of cached descriptors.
     * @discussion  This method will store a descriptor in a set of cached descriptors.  If a descriptor with an identical <code>bDescriptorType</code>, <code>index</code>, and <code>languageID</code> already exists in the cache, it will be replaced.  The provided descriptor is copied, and may be discarded after being stored in the cache.  Future descriptor fetch calls such as getDescriptor will return the cached value.  Future GET_DESCRIPTOR control requests (USB 2.0 9.4.3) will also return the cached descriptor.  SET_DESCRIPTOR control requests (USB 2.0 9.4.8) will remove the cached descriptor.
     * @param       descriptor Pointer to the descriptor to cache
     * @param       length Length of the descriptor, or 0 the descriptor's bLength field should be used.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       languageID Descriptor language ID.  <code>wIndex</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @return      IOReturn result code
     */
    virtual IOReturn cacheDescriptor(const StandardUSB::Descriptor* descriptor, uint16_t length = 0, uint8_t index = 0, uint16_t languageID = 0);
    
    /*!
     * @brief       Retrieve a descriptor from the cache or the device
     * @discussion  
     * This method will search the descriptor cache for the descriptor that matches the input arguments.  If the descriptor is not in the cache, a GET_DESCRIPTOR control request (USB 2.0 9.4.3) will be issued to retrieve the descriptor from the device.  If the device request is successful, the retrieved descriptor will be added to the cache.
     * This method will acquire the service's workloop lock, and may call commandSleep to perform the GET_DESCRIPTOR control request.
     * @param       type <code>bDescriptorType</code> of the descriptor to find.
     * @param       length Reference to a uint16_t which will be updated with the length of the descriptor.  As input, used as <code>wLength</code> when fetching variable-length configuration or BOS descriptors, or when fetching nonstandard descriptor types.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       languageID Descriptor language ID.  <code>wIndex</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       requestType tDeviceRequestType to be used for a GET_DESCRIPTOR control request.
     * @param       requestRecipient tDeviceRequestRecipient to be used for a GET_DESCRIPTOR control request.
     * @return      Pointer to the cached descriptor if found, otherwise NULL.
     */
    virtual const StandardUSB::Descriptor* getDescriptor(uint8_t type,
                                                         uint16_t& length,
                                                         uint8_t index = 0,
                                                         uint16_t languageID = 0,
                                                         tDeviceRequestType requestType = kRequestTypeStandard,
                                                         tDeviceRequestRecipient requestRecipient = kRequestRecipientDevice);
    
    /*!
     * @brief       Return the device descriptor
     * @discussion  This method uses getDescriptor to retrieve the device descriptor.
     * @return      Pointer to the device descriptor, or NULL if the service is terminating.
     */
    virtual const StandardUSB::DeviceDescriptor* getDeviceDescriptor();

    /*!
     * @brief       Return the configuration descriptor with the specified index
     * @discussion  This method uses getDescriptor to retrieve a configuration descriptor.
     * @param       index Descriptor index value
     * @return      Pointer to the configuration descriptor if found
     */
    virtual const StandardUSB::ConfigurationDescriptor* getConfigurationDescriptor(uint8_t index);

    /*!
     * @brief       Return the configuration descriptor with the specified <code>bConfigurationValue</code>
     * @discussion  This method uses getDescriptor to search for a configuration descriptor with a specific <code>bConfigurationValue</code> field.
     * @param       bConfigurationValue Value to match
     * @return      Pointer to the configuration descriptor if found
     */
    virtual const StandardUSB::ConfigurationDescriptor* getConfigurationDescriptorWithValue(uint8_t bConfigurationValue);
    
    /*!
     * @brief       Return the currently selected configuration descriptor
     * @discussion  This method uses getDescriptor to return the configuration descriptor currently selected after a successful setConfiguration call
     * @return      Pointer to the configuration descriptor if found, or NULL if the device is not configured
     */
    virtual const StandardUSB::ConfigurationDescriptor* getConfigurationDescriptor();

    /*!
     * @brief       Return the capability descriptors of the device
     * @discussion  This method uses getDescriptor to return the device's BOS descriptors
     * @return      Pointer to the BOS descriptor if found, otherwise NULL
     */
    virtual const StandardUSB::BOSDescriptor* getCapabilityDescriptors();

    /*!
     * @brief       Return a string descriptor from the device
     * @discussion  This method uses getDescriptor to return a string descriptor.
     * @param       index Descriptor index value.  Low byte of <code>wValue</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).
     * @param       languageID Descriptor language ID.  <code>wIndex</code> of the SET_DESCRIPTOR control request (USB 2.0 9.4.8).     *
     * @return      Pointer to the descriptor if found
     */
    virtual const StandardUSB::StringDescriptor* getStringDescriptor(uint8_t index, uint16_t languageID = StandardUSB::kLanguageIDEnglishUS);

    // Public pad slots for descriptors
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 40);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 41);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 42);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 43);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 44);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 45);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 46);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 47);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 48);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 49);

protected:
    virtual IOReturn cacheDescriptorGated(const StandardUSB::Descriptor* descriptor, uint16_t length, uint8_t index, uint16_t languageID);
    
    struct tDescriptorSelectors
    {
        uint8_t                 index;
        uint16_t                languageID;
        tDeviceRequestType      requestType;
        tDeviceRequestRecipient requestRecipient;
    };
    virtual IOReturn getDescriptorGated(uint8_t type, uint16_t& length, tDescriptorSelectors& selectors, const StandardUSB::Descriptor*& result);
    
    // Protected pad slots for descriptors
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 50);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 51);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 52);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 53);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 54);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 55);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 56);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 57);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 58);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 59);

protected:
    bool                         _getDescriptorInProgress;
    AppleUSBDescriptorCache* _descriptorCache;
    
#pragma mark Configuration, interface, and pipe management
public:
    /*! @functiongroup Configuration, interface, and pipe management */
    
    /*!
     * @brief       Select a new configuration for the device
     * @discussion
     * This method will select a new configuration for a device.  If the device was previously configured all child interfaces will be terminated prior to setting the new configuration.  This method will send the SET_CONFIGURATION control request (USB 2.0 9.4.7) to the device.
     * This method will acquire the service's workloop lock, and may call commandSleep to perform the SET_CONFIGURATION control request.
     * @param       bConfigurationValue Configuration to select
     * @param       matchInterfaces If true, any interfaces within the new configuration will be registered for matching.
     * @return      IOReturn result code
     */
    virtual IOReturn setConfiguration(uint8_t bConfigurationValue, bool matchInterfaces = true);
    
    /*!
     * @brief       The following method should be considered private and should not be called by function drivers.
     * @discussion  Drivers should use <code>IOUSBHostInterface::copyPipe</code> to retrieve handles to IOUSBHostPipe objects.
     */
    virtual IOUSBHostPipe* createPipe(const StandardUSB::EndpointDescriptor* descriptor, const StandardUSB::SuperSpeedEndpointCompanionDescriptor* companionDescriptor, IOUSBHostInterface* interface);

    /*!
     * @brief       Enqueue a request on the default control endpoint
     * @discussion  This method will enqueue an asynchronous request on the default control endpoint.  If successful, the provided completion routine will be called to report the status of the completed IO.
     * @param       forClient The service with an open session issuing the request.
     * @param       request Reference to a valid StandardUSB::DeviceRequest structure.  The structure is copied and can therefore be stack-allocated.
     * @param       dataBuffer A void* or IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       completion Pointer to a IOUSBHostCompletion structure.  This will be copied and can therefore be stack-allocated.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      kIOReuturnSuccess if the completion will be called in the future, otherwise error
     */
    virtual IOReturn deviceRequest(IOService* forClient, StandardUSB::DeviceRequest& request, void* dataBuffer, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);
    virtual IOReturn deviceRequest(IOService* forClient, StandardUSB::DeviceRequest& request, IOMemoryDescriptor* dataBuffer, IOUSBHostCompletion* completion, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);
    
    /*!
     * @brief       Send a request on the default control endpoint
     * @discussion  This method will send a synchronous request on the default control endpoint, and will not return until the request is complete.  This method will acquire the service's workloop lock, and will call commandSleep to send the control request.
     * @param       forClient The service with an open session issuing the request.
     * @param       request Reference to a valid StandardUSB::DeviceRequest structure.
     * @param       dataBuffer A void* or IOMemoryDescriptor* defining the memory to use for the request's data phase.
     * @param       bytesTransferred A uint32_t reference which will be updated with the byte count of the completed data phase.
     * @param       completionTimeoutMs Timeout of the request in milliseconds.  If 0, the request will never timeout.
     * @return      IOReturn value indicating the result of the IO request
     */
    virtual IOReturn deviceRequest(IOService* forClient, StandardUSB::DeviceRequest& request, void* dataBuffer, uint32_t& bytesTransferred, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);
    virtual IOReturn deviceRequest(IOService* forClient, StandardUSB::DeviceRequest& request, IOMemoryDescriptor* dataBuffer, uint32_t& bytesTransferred, uint32_t completionTimeoutMs = kUSBHostDefaultControlCompletionTimeoutMS);

    /*!
     * @brief       Abort device requests made via the @link deviceRequest @/link methods by <code>forClient</code>
     * @discussion  This method will abort any requests associated with a specific client made via the @link deviceRequest @/link methods.  It will not abort requests made by other clients.
     * @param       forClient Client which issued the requests
     * @param       options IOUSBHostIOSource::tAbortOptions
     * @param       withError IOReturn error value to return with the requests.  The default value of kIOReturnAborted should be used.
     * @return      IOReturn result code
     */
    virtual IOReturn abortDeviceRequests(IOService* forClient = NULL, IOOptionBits options = IOUSBHostIOSource::kAbortAsynchronous, IOReturn withError = kIOReturnAborted);
    
    // Public pad slots for configuration, interface, and pipe management
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 60);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 61);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 62);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 63);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 64);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 65);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 66);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 67);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 68);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 69);

protected:
    virtual IOReturn setConfigurationGated(uint8_t bConfigurationValue, bool matchInterfaces);
    
    virtual IOReturn createPipeGated(const StandardUSB::EndpointDescriptor* descriptor, const StandardUSB::SuperSpeedEndpointCompanionDescriptor* companionDescriptor, IOUSBHostInterface* interface, IOUSBHostPipe*& result);

    virtual IOReturn deviceRequest(IOService* forClient, StandardUSB::DeviceRequest& request,
                                   void* rawBuffer, IOMemoryDescriptor* descriptorBuffer,
                                   uint32_t& bytesTransferred, IOUSBHostCompletion* completion,
                                   uint32_t completionTimeoutMs);

    virtual IOReturn internalDeviceRequest(IOService* forClient, StandardUSB::DeviceRequest& request,
                                           void* rawBuffer, IOMemoryDescriptor* descriptorBuffer,
                                           uint32_t& bytesTransferred, IOUSBHostCompletion* completion,
                                           uint32_t completionTimeoutMs);
    
    virtual IOReturn internalDeviceRequestGated(IOUSBHostPipe::tInternalControlRequestParameters& parameters);

    virtual IOReturn abortDeviceRequestsGated(IOService* forClient, IOOptionBits options, IOReturn withError);

    virtual IOReturn getLPMExitLatencyGated(tUSBLinkState linkState, tUSBLPMExitLatency latencyType, uint32_t& latencyNs);
    
    virtual IOReturn updateLPMPolicyGated();
    
    // Protected pad slots for configuration, interface, and pipe management
    OSMetaClassDeclareReservedUsed(IOUSBHostDevice, 70);
    OSMetaClassDeclareReservedUsed(IOUSBHostDevice, 71);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 72);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 73);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 74);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 75);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 76);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 77);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 78);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 79);

#pragma mark Miscellaneous
public:
    /*! @functiongroup Provider and state accessors */

    /*!
     * @brief   Retrieve the device's address
     * @return  Current address of the device
     */
    virtual tUSBHostDeviceAddress getAddress() const;
    
    /*!
     * @brief       Retrieve the device's operational speed
     * @return      tInternalUSBHostConnectionSpeed
     */
    virtual uint8_t getSpeed() const;
    
    /*!
     * @brief       Return the current frame number of the USB controller
     * @description This method will return the current frame number of the USB controller, omitting microframe.  This is most useful for scheduling future isochronous requests.
     * @param       theTime If not NULL, this will be updated with the current system time
     * @return      The current frame number
     */
    virtual uint64_t getFrameNumber(AbsoluteTime* theTime = NULL);
    
    /*!
     * @brief       Terminate the device and attempt to reenumerate it
     * @discussion  This function will reset and attempt to reenumerate the USB device.  The current IOUSBHostDevice object and all of its children will be terminated.  A new IOUSBHostDevice object will be created and registered if the reset is successful and the previous object has finished terminating.  This function may not be called from the port workloop thread.
     * @return      IOReturn result code
     */
    virtual IOReturn reset();
    
    /*!
     * @brief       Return the current port status
     * @discussion  This method will return the current port status as described by @link tUSBHostPortStatus @/link
     * @return      port status
     */
    virtual uint32_t getPortStatus();
    
    /*!
     * @brief       Allocate a buffer to be used for I/O
     * @discussion  This method will allocate an IOBufferMemoryDescriptor optimized for use by the underlying controller hardware.  A buffer allocated by this method will not be bounced to perform DMA operations.
     * @param       options kIODirectionOut, kIODirectionIn to set the direction of the I/O transfer.
     * @param       capacity Size of the buffer to allocate
     * @return      Pointer to an IOBufferMemoryDescriptor if successful, otherwise NULL
     */
    virtual IOBufferMemoryDescriptor* createIOBuffer(IOOptionBits options, mach_vm_size_t capacity);

    /*!
     * @brief       Calculate desired USB LPM Exit Latency
     * @discussion  This method can be called to get various USB LPM Exit Latencies
     * @param       linkState The link state we want to exit from
     * @param       latencyType Desired type of Exit Latency to be calculated
     * @param       latencyNs Calculated Exit Latency value in Nanoseconds
     * @return      IOReturn result code
     */
    virtual IOReturn getLPMExitLatency(tUSBLinkState linkState, tUSBLPMExitLatency latencyType, uint32_t& latencyNs);

    // Public pad slots for miscellaneous
    OSMetaClassDeclareReservedUsed(IOUSBHostDevice, 80);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 81);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 82);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 83);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 84);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 85);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 86);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 87);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 88);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 89);

protected:
    // Protected pad slots for miscellaneous
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 90);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 91);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 92);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 93);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 94);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 95);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 96);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 97);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 98);
    OSMetaClassDeclareReservedUnused(IOUSBHostDevice, 99);
    
#if TARGET_OS_HAS_USBDRIVERKIT_IOUSBHOSTDEVICE
    static void asyncDeviceRequestCompletionCallback(void *owner, void *parameter, IOReturn status, uint32_t bytesTransferred);
#endif

protected:
    enum
    {
        kClientsDefaultCapacity          = 16,
        kPowerChildrenDefaultCapacity    = 16,
        kSetConfigurationIORetries       = 3,
        kSetConfigurationIORetryInterval = 100
    };

    AppleUSBHostRequestCompleter*            _requestCompleter;
    AppleUSBHostDeviceIdler*                 _deviceIdler;
    tUSBHostDeviceAddress                    _address;
    IOUSBHostPipe*                           _pipeZero;
    UInt8                                    _speed;
    StandardUSB::EndpointDescriptor          _endpointZero;
    IOBufferMemoryDescriptor**               _configList;
    uint32_t                                 _configListCount;
    uint8_t                                  _currentConfigValue;
    bool                                     _setConfigurationInProgress;
    uint32_t                                 _configurationBusCurrent; // mA
    uint32_t                                 _allocatedWakeBusCurrent; // mA
    uint32_t                                 _allocatedSleepBusCurrent;// mA
    uint32_t                                 _pendingCloseCount;
    
    uint32_t _debugLoggingMask;
    uint32_t _locationID;

    struct tExpansionData
    {
        IOTimerEventSource* _lifecycleTimer;
        OSDictionary*       _lpmLatencyCache;
        tUSBDeviceLPMStatus _lpmU1Status;
        tUSBDeviceLPMStatus _lpmU2Status;
        tUSBDeviceLPMStatus _lpmL1Status;
        OSDictionary*       _dkInterfaceIteratorDict;
        uintptr_t           _dkInterfaceInteratorRefID;
    };

    tExpansionData* _expansionData;


private:
    enum
    {
        kForcePowerTimeoutMs  = 1000ULL,      // Milliseconds
        kSuspendDeviceTimeout = 1000ULL,      // Milliseconds
        kTerminationTimeout   = 5000ULL,      // Milliseconds
        kDeviceAddressInvalid = 0xffff
    };
};

#endif // IOUSBHostFamily_IOUSBHostDevice_h
