/*
 * Copyright (c) 1998-2006 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef IOUSBHOSTHIDDEVICE_H
#define IOUSBHOSTHIDDEVICE_H

#include <IOKit/IOBufferMemoryDescriptor.h>

#include <IOKit/hid/IOHIDDevice.h>

#include <IOKit/usb/IOUSBHostInterface.h>
#include <IOKit/usb/StandardUSB.h>
#include <IOKit/usb/USBSpec.h>
#include <IOKit/usb/IOUSBHostFamily.h>
#include <IOKit/IOTimerEventSource.h>


#define ENABLE_HIDREPORT_LOGGING	0

enum
{
    kInterruptRetries      = 10,
    kErrorRecoveryInterval = 50     // Milliseconds
};

//	from IOHIDDescriptorParser.h:
//    // types of HID reports (input, output, feature)
//    enum
//    {
//        kHIDInputReport			= 	1,
//        kHIDOutputReport,
//        kHIDFeatureReport,
//        kHIDUnknownReport		=	255
//    };
//
// Report types from high level HID Manager:
//	from IOHIDKeys.h:
//    enum IOHIDReportType
//    {
//        kIOHIDReportTypeInput = 0,
//        kIOHIDReportTypeOutput,
//        kIOHIDReportTypeFeature,
//        kIOHIDReportTypeCount
//    };
//
#define HID_MGR_2_USB_REPORT_TYPE(x) (x + 1)

#define USB_2_HID_MGR_REPORT_TYPE(x) (x - 1)

#define kHIDDriverRetryCount	3

#define	kUSBHIDReportLoggingLevel	"USB HID Report Logging Level"

#define kUSBHID_DeviceIdleTimeout "Device Idle Timeout"

#define kUSBHID_IoIdleTimeout "I/O Idle Timeout"

#define kUSBHostHIDDevicePropertyIdlePolicy "kUSBHIDDeviceIdlePolicy"

/*!
 @typedef IOUSBHostHIDDescriptor
 @discussion USB HID Descriptor.  See the USB HID Specification at <a href="http://www.usb.org"TARGET="_blank">http://www.usb.org</a>.  (This structure
 should have used the #pragma pack(1) compiler directive to get byte alignment.
 */
struct IOUSBHostHIDDescriptor : public StandardUSB::Descriptor
{
    uint16_t descVersNum;
    uint8_t  hidCountryCode;
    uint8_t  hidNumDescriptors;
    uint8_t  hidDescriptorType;
    uint8_t  hidDescriptorLengthLo;
    uint8_t  hidDescriptorLengthHi;
} __attribute__((packed));

/*!
 @typedef IOUSBHIDReportDesc
 @discussion USB HID Report Descriptor header.  See the USB HID Specification at <a href="http://www.usb.org"TARGET="_blank">http://www.usb.org</a>.  (This structure
 should have used the #pragma pack(1) compiler directive to get byte alignment.
 */
struct IOUSBHostHIDReportDescriptor
{
    uint8_t hidDescriptorType;
    uint8_t hidDescriptorLengthLo;
    uint8_t hidDescriptorLengthHi;
} __attribute__((packed));

/*!
 @class IOUSBHostHIDDevice
 @abstract Driver that matches to USB HID devices.
 @discussion This class can be overriden to provide for specific behaviors.
 */
class IOUSBHostHIDDevice : public IOHIDDevice
{
    OSDeclareDefaultStructors(IOUSBHostHIDDevice)
    
private:
    typedef struct IOUSBHidExpansionData
    {
        uint8_t _bInterfaceNumber;
    } tIOUSBHidExpansionData;
    
    tIOUSBHidExpansionData *_expansionData;
    
    typedef struct
    {
        IOUSBHostCompletion     usbCompletion;
        IOHIDCompletion*        hidCompletion;
        IOMemoryDescriptor*     report;
    } tAsyncContext;
    
    
protected:
    IOWorkLoop               *_workLoop;
    IOCommandGate            *_gate;
    IOUSBHostDevice          *_device;
    IOUSBHostInterface       *_interface;
    IOUSBHostPipe            *_interruptPipe;
    IOUSBHostPipe            *_interruptOutPipe;
    IOBufferMemoryDescriptor *_buffer;
    IOMemoryDescriptor       *_zlpDescriptor;
    IOUSBHostCompletion       _completion;
    IOUSBHostCompletion       _zlpCompletion;
    bool                      _interruptReadPending;
    uint32_t                  _interruptOutMaxSize;
    uint32_t                  _zlpBuffer;
    uint32_t                  _debugLoggingMask;
    int32_t                   _interruptRetries;
    IOTimerEventSource       *_interruptRetriesTimer;

    // IOHIDDevice Methods
    virtual bool      handleStart (IOService *provider);
    virtual void      interruptRetry(IOTimerEventSource* timer);

    // IOUSBHIDDevice Methods
    virtual IOReturn  setProtocol (uint16_t protocol);
    
    virtual OSString* getStringAtIndex (uint8_t index, uint16_t lang = 0x0409) const;
    
    virtual IOReturn  getHidDescriptor (uint8_t inDescriptorType, uint8_t inDescriptorIndex, uint8_t *vOutBuf, uint32_t *vOutSize) const;
    
    virtual IOReturn  getHidDescriptorGated (uint8_t inDescriptorType, uint8_t inDescriptorIndex, uint8_t *vOutBuf, uint32_t *vOutSize) const;
    
    virtual IOReturn  readInterruptPipeAsync (void);
    
    virtual IOReturn  readInterruptPipeAsyncGated (void);
    
    virtual void      interruptReadComplete (void *parameter, IOReturn status, uint32_t bytesTransferred);
    
    virtual void      setReportComplete (void *parameter, IOReturn status, uint32_t bytesTransferred);
    
    virtual void      zlpWriteComplete (void *parameter, IOReturn status, uint32_t bytesTransferred);
    
    // Protected pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 0);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 1);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 2);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 3);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 4);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 5);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 6);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 7);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 8);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 9);

public:
    // IOService Methods
    virtual bool start (IOService *provider);
    
    virtual bool willTerminate (IOService *provider, IOOptionBits options);
    
    virtual void stop (IOService *provider);
    
    virtual void free (void);

    virtual bool setProperty(const OSSymbol* aKey, OSObject* anObject);

    virtual bool setProperty(const OSString* aKey, OSObject* anObject);

    virtual bool setProperty(const char* aKey, OSObject* anObject);

    virtual bool setProperty(const char* aKey, const char * aString);

    virtual bool setProperty(const char* aKey, bool aBoolean);

    virtual bool setProperty(const char* aKey, unsigned long long aValue, unsigned int aNumberOfBits);

    virtual bool setProperty(const char* aKey, void* bytes, unsigned int length);
    
    
    // IOHIDDevice Methods
    virtual IOReturn  newReportDescriptor (IOMemoryDescriptor **descriptor ) const;
    
    virtual OSString* newTransportString (void) const;
    
    virtual OSString* newManufacturerString (void) const;
    
    virtual OSString* newProductString (void) const;
    
    virtual OSString* newSerialNumberString (void) const;
    
    virtual OSNumber* newVendorIDNumber (void) const;
    
    virtual OSNumber* newProductIDNumber (void) const;
    
    virtual OSNumber* newVersionNumber (void) const;
    
    virtual OSNumber* newLocationIDNumber (void) const;
    
    virtual OSNumber* newCountryCodeNumber (void) const;
    
    virtual IOReturn getReport (IOMemoryDescriptor *report, IOHIDReportType reportType, IOOptionBits options);
    
    virtual IOReturn setReport (IOMemoryDescriptor *report, IOHIDReportType reportType, IOOptionBits options = 0);
    
    virtual IOReturn setReport(IOMemoryDescriptor * report, IOHIDReportType reportType, IOOptionBits options, UInt32 completionTimeout, IOHIDCompletion* completion = 0);
    
    
    // IOUSBHIDDevice Methods
    virtual IOReturn  SetIdleMillisecs (UInt16 msecs);
    
    virtual UInt32    getMaxReportSize (void) const;
    
    virtual OSString* newIndexedString (UInt8 index) const;

    virtual OSNumber* newReportIntervalNumber() const;
    
    // Public pad slots for future expansion
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 10);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 11);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 12);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 13);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 14);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 15);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 16);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 17);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 18);
    OSMetaClassDeclareReservedUnused(IOUSBHostHIDDevice, 19);
};

#endif	// IOUSBHOSTHIDDEVICE_H
