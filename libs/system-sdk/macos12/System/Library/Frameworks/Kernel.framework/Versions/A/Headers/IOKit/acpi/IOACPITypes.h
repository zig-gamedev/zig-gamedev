/*
 * Copyright (c) 2003-2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef __IOKIT_IOACPITYPES_H
#define __IOKIT_IOACPITYPES_H

#include <IOKit/IOMessage.h>

extern const IORegistryPlane * gIOACPIPlane;
extern const OSSymbol *        gIOACPIHardwareIDKey;
extern const OSSymbol *        gIOACPIUniqueIDKey;
extern const OSSymbol *        gIOACPIAddressKey;
extern const OSSymbol *        gIOACPIDeviceStatusKey;

#define kIOACPIInterruptTypeValid  (1 << 1)
#pragma pack(1)

struct IOACPIAddressSpaceDescriptor {
    UInt32  resourceType;
    UInt32  generalFlags;
    UInt32  typeSpecificFlags;
    UInt32  reserved1;
    UInt64  granularity;
    UInt64  minAddressRange;
    UInt64  maxAddressRange;
    UInt64  translationOffset;
    UInt64  addressLength;
    UInt64  reserved2;
    UInt64  reserved3;
    UInt64  reserved4;
};

enum {
    kIOACPIMemoryRange    = 0,
    kIOACPIIORange        = 1,
    kIOACPIBusNumberRange = 2
};

typedef UInt32 IOACPIAddressSpaceID;

enum {
    kIOACPIAddressSpaceIDSystemMemory       = 0,
    kIOACPIAddressSpaceIDSystemIO           = 1,
    kIOACPIAddressSpaceIDPCIConfiguration   = 2,
    kIOACPIAddressSpaceIDEmbeddedController = 3,
    kIOACPIAddressSpaceIDSMBus              = 4
};

/*
 * Address space operation
 */
enum {
    kIOACPIAddressSpaceOpRead  = 0,
    kIOACPIAddressSpaceOpWrite = 1
};

/*
 * 64-bit ACPI address
 */
union IOACPIAddress {
    UInt64 addr64;
    struct {
        unsigned int offset     :16;
        unsigned int function   :3;
        unsigned int device     :5;
        unsigned int bus        :8;
        unsigned int segment    :16;
        unsigned int reserved   :16;
    } pci;
};

/*
 * Address space handler
 */
typedef IOReturn (*IOACPIAddressSpaceHandler)( UInt32         operation,
                                               IOACPIAddress  address,
                                               UInt64 *       value,
                                               UInt32         bitWidth,
                                               UInt32         bitOffset,
                                               void *         context );

/*
 * ACPI fixed event types
 */
enum {
    kIOACPIFixedEventPMTimer       = 0,
    kIOACPIFixedEventPowerButton   = 2,
    kIOACPIFixedEventSleepButton   = 3,
    kIOACPIFixedEventRealTimeClock = 4
};

#pragma pack()

/*
 * FIXME: Move to xnu/iokit to reserve the ACPI family code.
 */
#ifndef sub_iokit_acpi
#define sub_iokit_acpi   err_sub(10)
#endif

/*
 * ACPI notify message sent to all clients and interested parties.
 * The notify code can be read from the argument as an UInt32.
 */
#define kIOACPIMessageDeviceNotification  iokit_family_msg(sub_iokit_acpi, 0x10)

/*
 * ACPI device power states
 */
enum {
    kIOACPIDevicePowerStateD0    = 0,
    kIOACPIDevicePowerStateD1    = 1,
    kIOACPIDevicePowerStateD2    = 2,
    kIOACPIDevicePowerStateD3    = 3,
    kIOACPIDevicePowerStateCount = 4
};

#endif /* !__IOKIT_IOACPITYPES_H */
