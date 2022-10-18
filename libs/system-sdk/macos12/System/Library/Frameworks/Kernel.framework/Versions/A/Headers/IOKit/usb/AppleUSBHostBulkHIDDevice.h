/*
 * Copyright (c) 2020 Apple Computer, Inc. All rights reserved.
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
#ifndef APPLEUSBHOSTBULKHIDDEVICE_H
#define APPLEUSBHOSTBULKHIDDEVICE_H

#include <IOKit/usb/IOUSBHostHIDDevice.h>

/*!
 @class AppleUSBHostBulkHIDDevice
 @abstract Driver that matches to USB HID devices, adds support for using USB Bulk pipes for HID Devices
 @discussion This class extends HID Devices to use USB Bulk pipes is available.
 */
class AppleUSBHostBulkHIDDevice : public IOUSBHostHIDDevice
{
    OSDeclareDefaultStructors(AppleUSBHostBulkHIDDevice);

private:
    typedef struct
    {
        uint8_t             _bInterfaceNumber;
        IOUSBHostPipe      *_inputPipe;
        IOUSBHostPipe      *_outputPipe;
        uint32_t            _outputMaxSize;
    } AppleUSBBulkHidExpansionData;

    AppleUSBBulkHidExpansionData *_expansionData;

public:
    virtual bool handleStart(IOService *provider) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
};


#endif /* APPLEUSBHOSTBULKHIDDEVICE_H */
