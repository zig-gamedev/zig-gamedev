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
 * @header IODVDBlockStorageDevice
 * @abstract
 * This header contains the IODVDBlockStorageDevice class definition.
 */

#ifndef	_IODVDBLOCKSTORAGEDEVICE_H
#define	_IODVDBLOCKSTORAGEDEVICE_H

#include <IOKit/storage/IODVDTypes.h>

/*!
 * @defined kIODVDBlockStorageDeviceClass
 * @abstract
 * kIODVDBlockStorageDeviceClass is the name of the IODVDBlockStorageDevice class.
 * @discussion
 * kIODVDBlockStorageDeviceClass is the name of the IODVDBlockStorageDevice class.
 */

#define kIODVDBlockStorageDeviceClass "IODVDBlockStorageDevice"

#ifdef KERNEL
#ifdef __cplusplus

/*
 * Kernel
 */

#include <IOKit/storage/IOCDBlockStorageDevice.h>

/* Property used for matching, so the generic driver gets the nub it wants. */
#define	kIOBlockStorageDeviceTypeDVD	"DVD"

/*!
 * @class
 * IODVDBlockStorageDevice : public IOCDBlockStorageDevice
 * @abstract
 * The IODVDBlockStorageDevice class is a generic DVD block storage device
 * abstraction.
 * @discussion
 * This class is the protocol for generic DVD functionality, independent of
 * the physical connection protocol (e.g. SCSI, ATA, USB).
 *
 * The APIs are the union of CD APIs and all
 * necessary new low-level DVD APIs.
 *
 * A subclass implements relay methods that translate our requests into
 * calls to a protocol- and device-specific provider.
 */

class IODVDBlockStorageDevice : public IOCDBlockStorageDevice {

    OSDeclareAbstractStructors(IODVDBlockStorageDevice)

protected:

    struct ExpansionData { /* */ };
    ExpansionData * _expansionData;

public:

    /* Overrides from IORegistryEntry */
    
    virtual bool	init(OSDictionary * properties);

    /* New APIs for DVD */

    virtual IOReturn	reportKey(IOMemoryDescriptor *buffer,const DVDKeyClass keyClass,
                                        const UInt32 lba,const UInt8 agid,const DVDKeyFormat keyFormat)	__attribute__ ((deprecated));

    virtual IOReturn	sendKey(IOMemoryDescriptor *buffer,const DVDKeyClass keyClass,
                                        const UInt8 agid,const DVDKeyFormat keyFormat)	= 0;

    virtual IOReturn	readDVDStructure(IOMemoryDescriptor *buffer,const DVDStructureFormat format,
                                        const UInt32 address,const UInt8 layer,const UInt8 agid)	= 0;

    virtual IOReturn	reportKey(IOMemoryDescriptor *buffer,const DVDKeyClass keyClass,
                                        const UInt32 lba,const UInt8 blockCount,
                                        const UInt8 agid,const DVDKeyFormat keyFormat); /* 10.12.0 */

    OSMetaClassDeclareReservedUsed(IODVDBlockStorageDevice,  0);        /* reportKey */
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  1);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  2);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  3);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  4);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  5);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  6);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  7);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  8);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice,  9);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 10);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 11);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 12);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 13);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 14);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 15);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 16);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 17);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 18);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 19);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 20);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 21);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 22);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 23);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 24);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 25);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 26);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 27);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 28);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 29);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 30);
    OSMetaClassDeclareReservedUnused(IODVDBlockStorageDevice, 31);
};

#endif /* __cplusplus */
#endif /* KERNEL */
#endif /* !_IODVDBLOCKSTORAGEDEVICE_H */
