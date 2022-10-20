/*
 * Copyright (c) 1998-2014 Apple Inc. All rights reserved.
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

/*!
 * @header IOCDBlockStorageDevice
 * @abstract
 * This header contains the IOCDBlockStorageDevice class definition.
 */

#ifndef _IOCDBLOCKSTORAGEDEVICE_H
#define _IOCDBLOCKSTORAGEDEVICE_H

#include <IOKit/storage/IOCDTypes.h>

/*!
 * @defined kIOCDBlockStorageDeviceClass
 * @abstract
 * kIOCDBlockStorageDeviceClass is the name of the IOCDBlockStorageDevice class.
 * @discussion
 * kIOCDBlockStorageDeviceClass is the name of the IOCDBlockStorageDevice class.
 */

#define kIOCDBlockStorageDeviceClass "IOCDBlockStorageDevice"

#ifdef KERNEL
#ifdef __cplusplus

/*
 * Kernel
 */

#include <IOKit/storage/IOBlockStorageDevice.h>

/* Property used for matching, so the generic driver gets the nub it wants. */
#define	kIOBlockStorageDeviceTypeCDROM	"CDROM"

/*!
 * @class
 * IOCDBlockStorageDevice : public IOBlockStorageDevice
 * @abstract
 * The IOCDBlockStorageDevice class is a generic CD block storage device
 * abstraction.
 * @discussion
 * This class is the protocol for generic CD functionality, independent of
 * the physical connection protocol (e.g. SCSI, ATA, USB).
 *
 * The APIs are the union of CD (block storage) data APIs and all
 * necessary low-level CD APIs.
 *
 * A subclass implements relay methods that translate our requests into
 * calls to a protocol- and device-specific provider.
 */

class IOCDBlockStorageDevice : public IOBlockStorageDevice {

    OSDeclareAbstractStructors(IOCDBlockStorageDevice)

protected:

    struct ExpansionData { /* */ };
    ExpansionData * _expansionData;

public:

    /* Overrides from IORegistryEntry */
    
    virtual bool	init(OSDictionary * properties);

    /*-----------------------------------------*/
    /* CD APIs                                 */
    /*-----------------------------------------*/

    virtual IOReturn	doAsyncReadCD(IOMemoryDescriptor *buffer,
                    	              UInt32 block,UInt32 nblks,
                    	              CDSectorArea sectorArea,
                    	              CDSectorType sectorType,
                    	              IOStorageCompletion completion) = 0;
    virtual UInt32	getMediaType(void)					= 0;
    virtual IOReturn	readISRC(UInt8 track,CDISRC isrc)			= 0;
    virtual IOReturn	readMCN(CDMCN mcn)					= 0;
    virtual IOReturn	readTOC(IOMemoryDescriptor *buffer) = 0;

    /*-----------------------------------------*/
    /* CD APIs                                 */
    /*-----------------------------------------*/

    virtual IOReturn	getSpeed(UInt16 * kilobytesPerSecond)	= 0;

    virtual IOReturn	setSpeed(UInt16 kilobytesPerSecond)	= 0;

    virtual IOReturn	readTOC(IOMemoryDescriptor *buffer,CDTOCFormat format,
                    	        UInt8 msf,UInt8 trackSessionNumber,
                    	        UInt16 *actualByteCount)	= 0;

    virtual IOReturn	readDiscInfo(IOMemoryDescriptor *buffer,
                    	             UInt16 *actualByteCount)	= 0;

    virtual IOReturn	readTrackInfo(IOMemoryDescriptor *buffer,UInt32 address,
                    	              CDTrackInfoAddressType addressType,
                    	              UInt16 *actualByteCount)	= 0;

    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  0);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  1);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  2);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  3);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  4);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  5);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  6);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  7);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  8);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice,  9);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice, 10);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice, 11);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice, 12);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice, 13);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice, 14);
    OSMetaClassDeclareReservedUnused(IOCDBlockStorageDevice, 15);
};

#endif /* __cplusplus */
#endif /* KERNEL */
#endif /* !_IOCDBLOCKSTORAGEDEVICE_H */
