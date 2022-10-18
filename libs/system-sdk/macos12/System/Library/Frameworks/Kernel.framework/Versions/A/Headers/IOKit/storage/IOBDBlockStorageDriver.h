/*
 * Copyright (c) 2006-2014 Apple Inc. All rights reserved.
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

/*
 * @header IOBDBlockStorageDriver
 * @abstract
 * This header contains the IOBDBlockStorageDriver class definition.
 * @discussion
 * This class implements BD functionality, independent of
 * the physical connection protocol (e.g. SCSI, ATA, USB).
 *
 * A protocol-specific provider implements the functionality using an appropriate
 * protocol and commands.
 */

#ifndef	_IOBDBLOCKSTORAGEDRIVER_H
#define	_IOBDBLOCKSTORAGEDRIVER_H

#include <IOKit/storage/IOBDBlockStorageDevice.h>
#include <IOKit/storage/IODVDBlockStorageDriver.h>

/*
 * @defined kIOBDBlockStorageDriverClass
 * @abstract
 * kIOBDBlockStorageDriverClass is the name of the IOBDBlockStorageDriver class.
 * @discussion
 * kIOBDBlockStorageDriverClass is the name of the IOBDBlockStorageDriver class.
 */

#define kIOBDBlockStorageDriverClass "IOBDBlockStorageDriver"

/*
 * @class
 * IOBDBlockStorageDriver
 * @abstract
 * Generic BD Driver.
 * @discussion
 * Storage drivers are split into two parts: the Generic Driver handles
 * all generic device issues, independent of the lower-level transport
 * mechanism (e.g. SCSI, ATA, USB, FireWire). All storage operations
 * at the Generic Driver level are translated into a series of generic
 * device operations. These operations are passed via the Device Nub
 * to a Transport Driver, which implements the appropriate
 * transport-dependent protocol to execute these operations.
 * 
 * To determine the write-protect state of a device (or media), for
 * example, the generic driver would issue a call to the
 * Transport Driver's reportWriteProtection method. If this were a SCSI
 * device, its Transport Driver would issue a Mode Sense command to
 * extract the write-protection status bit. The Transport Driver then
 * reports true or false to the generic driver.
 * 
 * The generic driver therefore has no knowledge of, or involvement
 * with, the actual commands and mechanisms used to communicate with
 * the device. It is expected that the generic driver will rarely, if
 * ever, need to be subclassed to handle device idiosyncrasies; rather,
 * the Transport Driver should be changed via overrides.
 * 
 * A generic driver could be subclassed to create a different type of
 * generic device. The generic driver IOBDBlockStorageDriver class is a subclass
 * of IODVDBlockStorageDriver, adding BD functions. Similarly, the Transport Driver
 * IOBDBlockStorageDevice is a subclass of IODVDBlockStorageDevice, adding BD 
 * functions.
*/

class __exported IOBDBlockStorageDriver : public IODVDBlockStorageDriver
{
    OSDeclareDefaultStructors(IOBDBlockStorageDriver)

protected:

    struct ExpansionData { /* */ };
    ExpansionData * _expansionData;

    /* Overrides of IODVDBlockStorageDriver. */

    virtual IOReturn acceptNewMedia(void);

    /* End of IODVDBlockStorageDriver overrides. */

public:

    /*
     * Obtain this object's provider.  We override the superclass's method to
     * return a more specific subclass of IOService -- IOBDBlockStorageDevice.
     * This method serves simply as a convenience to subclass developers.
     */

    virtual IOBDBlockStorageDevice * getProvider() const;

    /* Overrides of IODVDBlockStorageDriver: */

    virtual const char * getDeviceTypeName(void);
    virtual IOMedia * instantiateDesiredMediaObject(void);
    virtual IOMedia * instantiateMediaObject(UInt64 base,UInt64 byteSize,
                                            UInt32 blockSize,char *mediaName);
    virtual IOReturn readStructure(IOMemoryDescriptor *buffer,const DVDStructureFormat format,
                                        const UInt32 address,const UInt8 layer,const UInt8 agid);

    /* End of IODVDBlockStorageDriver overrides. */

    /*
     * @function splitTrack
     * @discussion
     * Issue an MMC RESERVE TRACK command with the ARSV bit.
     * @param address
     * As documented by MMC.
     * @result
     * Returns the status of the operation.
     */

    virtual IOReturn splitTrack(UInt32 address);

    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  0);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  1);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  2);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  3);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  4);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  5);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  6);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  7);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  8);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver,  9);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver, 10);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver, 11);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver, 12);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver, 13);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver, 14);
    OSMetaClassDeclareReservedUnused(IOBDBlockStorageDriver, 15);
};

#endif  /* !_IOBDBLOCKSTORAGEDRIVER_H */
