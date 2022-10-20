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

#ifndef _IOKIT_IOACPIPLATFORMEXPERT_H
#define _IOKIT_IOACPIPLATFORMEXPERT_H

#include <IOKit/IOPlatformExpert.h>           // superclass
#include <IOKit/acpi/IOACPIPlatformDevice.h>  // children

class __exported IOACPIPlatformExpert : public IODTPlatformExpert
{
    OSDeclareAbstractStructors( IOACPIPlatformExpert )

    friend class IOACPIPlatformDevice;

protected:
    /*! @struct ExpansionData
        @discussion This structure will be used to expand the capablilties
                    of the class in the future.
     */
    struct ExpansionData { };

    /*! @var reserved
        Reserved for future use. (Internal use only)
     */
    ExpansionData *  reserved;

public:
    virtual bool     start( IOService * provider ) APPLE_KEXT_OVERRIDE;

protected:
    // Map ACPI event to interrupt event source index

    virtual SInt32   installDeviceInterruptForFixedEvent(
                                  IOService *  device,
                                  UInt32       fixedEvent ) = 0;

    virtual SInt32   installDeviceInterruptForGPE(
                                  IOService *  device,
                                  UInt32       gpeNumber,
                                  void *       gpeBlockDevice,
                                  IOOptionBits options ) = 0;

    // ACPI global lock acquisition

    virtual IOReturn acquireGlobalLock( IOService *             client,
                                        UInt32 *                lockToken,
                                        const mach_timespec_t * timeout ) = 0;

    virtual void     releaseGlobalLock( IOService * client,
                                        UInt32      lockToken ) = 0;

    // ACPI method and object evaluation

    virtual IOReturn validateObject( IOACPIPlatformDevice * device,
                                     const OSSymbol *       objectName ) = 0;

    virtual IOReturn validateObject( IOACPIPlatformDevice * device,
                                     const char *           objectName );

    virtual IOReturn evaluateObject( IOACPIPlatformDevice * device,
                                     const OSSymbol *       objectName,
                                     OSObject **            result,
                                     OSObject *             params[],
                                     IOItemCount            paramCount,
                                     IOOptionBits           options ) = 0;

    virtual IOReturn evaluateObject( IOACPIPlatformDevice * device,
                                     const char *           objectName,
                                     OSObject **            result,
                                     OSObject *             params[],
                                     IOItemCount            paramCount,
                                     IOOptionBits           options );

    // ACPI table

    virtual const OSData * getACPITableData(
                                     const char * tableName,
                                     UInt32       tableInstance ) = 0;

    // Address space handler

    virtual IOReturn registerAddressSpaceHandler(
                                   IOACPIPlatformDevice *    device,
                                   IOACPIAddressSpaceID      spaceID,
                                   IOACPIAddressSpaceHandler handler,
                                   void *                    context,
                                   IOOptionBits              options ) = 0;

    virtual void     unregisterAddressSpaceHandler(
                                   IOACPIPlatformDevice *    device,
                                   IOACPIAddressSpaceID      spaceID,
                                   IOACPIAddressSpaceHandler handler,
                                   IOOptionBits              options ) = 0;

    // Address space read/write

    virtual IOReturn readAddressSpace(  UInt64 *             value,
                                        IOACPIAddressSpaceID spaceID,
                                        IOACPIAddress        address,
                                        UInt32               bitWidth,
                                        UInt32               bitOffset,
                                        IOOptionBits         options ) = 0;

    virtual IOReturn writeAddressSpace( UInt64               value,
                                        IOACPIAddressSpaceID spaceID,
                                        IOACPIAddress        address,
                                        UInt32               bitWidth,
                                        UInt32               bitOffset,            
                                        IOOptionBits         options ) = 0;

    // Device power management

    virtual IOReturn setDevicePowerState( IOACPIPlatformDevice * device,
                                          UInt32 powerState ) = 0;

    virtual IOReturn getDevicePowerState( IOACPIPlatformDevice * device,
                                          UInt32 * powerState ) = 0;

    virtual IOReturn setDeviceWakeEnable( IOACPIPlatformDevice * device,
                                          bool enable ) = 0;

    // vtable padding

    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  0 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  1 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  2 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  3 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  4 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  5 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  6 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  7 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  8 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert,  9 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 10 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 11 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 12 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 13 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 14 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 15 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 16 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 17 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 18 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 19 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 20 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 21 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 22 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 23 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 24 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 25 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 26 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 27 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 28 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 29 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 30 );
    OSMetaClassDeclareReservedUnused( IOACPIPlatformExpert, 31 );
};

#endif /* !_IOKIT_IOACPIPLATFORMEXPERT_H */
