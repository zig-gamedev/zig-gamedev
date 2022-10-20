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
 * @header IODVDMedia
 * @abstract
 * This header contains the IODVDMedia class definition.
 */

#ifndef _IODVDMEDIA_H
#define _IODVDMEDIA_H

/*!
 * @defined kIODVDMediaClass
 * @abstract
 * kIODVDMediaClass is the name of the IODVDMedia class.
 * @discussion
 * kIODVDMediaClass is the name of the IODVDMedia class.
 */

#define kIODVDMediaClass "IODVDMedia"

/*!
 * @defined kIODVDMediaTypeKey
 * @abstract
 * kIODVDMediaTypeKey is a property of IODVDMedia objects.  It has an OSString
 * value.
 * @discussion
 * The kIODVDMediaTypeKey property identifies the DVD media type (DVD-ROM,
 * DVD-R, DVD-RW, DVD+RW, DVD-RAM, etc).  See the kIODVDMediaType contants
 * for possible values.
 */

#define kIODVDMediaTypeKey "Type"

/*!
 * @defined kIODVDMediaTypeROM
 * The kIODVDMediaTypeKey constant for DVD-ROM media.
 */

#define kIODVDMediaTypeROM "DVD-ROM"

/*!
 * @defined kIODVDMediaTypeR
 * The kIODVDMediaTypeKey constant for DVD Recordable (DVD-R) media.
 */

#define kIODVDMediaTypeR "DVD-R"

/*!
 * @defined kIODVDMediaTypeRW
 * The kIODVDMediaTypeKey constant for DVD ReWritable (DVD-RW) media.
 */

#define kIODVDMediaTypeRW "DVD-RW"

/*!
 * @defined kIODVDMediaTypePlusR
 * The kIODVDMediaTypeKey constant for DVD "Plus" Recordable (DVD+R) media.
 */

#define kIODVDMediaTypePlusR "DVD+R"

/*!
 * @defined kIODVDMediaTypePlusRW
 * The kIODVDMediaTypeKey constant for DVD "Plus" ReWritable (DVD+RW) media.
 */

#define kIODVDMediaTypePlusRW "DVD+RW"

/*!
 * @defined kIODVDMediaTypeRAM
 * The kIODVDMediaTypeKey constant for DVD-RAM media.
 */

#define kIODVDMediaTypeRAM "DVD-RAM"

/*!
 * @defined kIODVDMediaTypeHDROM
 * The kIODVDMediaTypeKey constant for HD DVD-ROM media.
 */

#define kIODVDMediaTypeHDROM "HD DVD-ROM"

/*!
 * @defined kIODVDMediaTypeHDR
 * The kIODVDMediaTypeKey constant for HD DVD Recordable (HD DVD-R) media.
 */

#define kIODVDMediaTypeHDR "HD DVD-R"

/*!
 * @defined kIODVDMediaTypeHDRW
 * The kIODVDMediaTypeKey constant for HD DVD ReWritable (HD DVD-RW) media.
 */

#define kIODVDMediaTypeHDRW "HD DVD-RW"

/*!
 * @defined kIODVDMediaTypeHDRAM
 * The kIODVDMediaTypeKey constant for HD DVD-RAM media.
 */

#define kIODVDMediaTypeHDRAM "HD DVD-RAM"

#ifdef KERNEL
#ifdef __cplusplus

/*
 * Kernel
 */

#include <IOKit/storage/IODVDBlockStorageDriver.h>
#include <IOKit/storage/IOMedia.h>

/*!
 * @class IODVDMedia
 * @abstract
 * The IODVDMedia class is a random-access disk device abstraction for DVDs.
 * @discussion
 * The IODVDMedia class is a random-access disk device abstraction for DVDs.
 */

class IODVDMedia : public IOMedia
{
    OSDeclareDefaultStructors(IODVDMedia)

protected:

    struct ExpansionData { /* */ };
    ExpansionData * _expansionData;

public:

    /*
     * Obtain this object's provider.   We override the superclass's method to
     * return a more specific subclass of IOService -- IODVDBlockStorageDriver.
     * This method serves simply as a convenience to subclass developers.
     */

    virtual IODVDBlockStorageDriver * getProvider() const;

    /*
     * Compare the properties in the supplied table to this object's properties.
     */

    virtual bool matchPropertyTable(OSDictionary * table, SInt32 * score);

    /*
     * Issue an MMC REPORT KEY command.
     * Obsoleted, replaced by this interface:
     *    virtual IOReturn reportKey( IOMemoryDescriptor * buffer,
     *                                UInt8                keyClass,
     *                                UInt32               address,
     *                                UInt8                blockCount,
     *                                UInt8                grantID,
     *                                UInt8                format );
     */

    virtual IOReturn reportKey( IOMemoryDescriptor * buffer,
                                const DVDKeyClass    keyClass,
                                const UInt32         address,
                                const UInt8          grantID,
                                const DVDKeyFormat   format ) __attribute__ ((deprecated));

    /*!
     * @function sendKey
     * @discussion
     * Issue an MMC SEND KEY command.
     * @param buffer
     * Buffer for the data transfer.  The size of the buffer implies the size of
     * the data transfer.  Pass null for the kDVDKeyFormatAGID_Invalidate format
     * case.
     * @param keyClass
     * As documented by MMC.
     * @param grantID
     * As documented by MMC.
     * @param format
     * As documented by MMC.
     * @result
     * Returns the status of the data transfer.
     */

    virtual IOReturn sendKey( IOMemoryDescriptor * buffer,
                              const DVDKeyClass    keyClass,
                              const UInt8          grantID,
                              const DVDKeyFormat   format );

    /*!
     * @function readStructure
     * @discussion
     * Issue an MMC READ DVD STRUCTURE command.
     * @param buffer
     * Buffer for the data transfer.  The size of the buffer implies the size of
     * the data transfer.
     * @param format
     * As documented by MMC.
     * @param address
     * As documented by MMC.
     * @param layer
     * As documented by MMC.
     * @param grantID
     * As documented by MMC.
     * @result
     * Returns the status of the data transfer.
     */

    virtual IOReturn readStructure( IOMemoryDescriptor *     buffer,
                                    const DVDStructureFormat format,
                                    const UInt32             address,
                                    const UInt8              layer,
                                    const UInt8              grantID );

    /*!
     * @function getSpeed
     * @discussion
     * Get the current speed used for data transfers.
     * @param kilobytesPerSecond
     * Returns the current speed used for data transfers, in kB/s.
     *
     * kDVDSpeedMin specifies the minimum speed for all DVD media (1X).
     * kDVDSpeedMax specifies the maximum speed supported in hardware.
     * @result
     * Returns the status of the operation.
     */

    virtual IOReturn getSpeed(UInt16 * kilobytesPerSecond);

    /*!
     * @function setSpeed
     * @discussion
     * Set the speed to be used for data transfers.
     * @param kilobytesPerSecond
     * Speed to be used for data transfers, in kB/s.
     *
     * kDVDSpeedMin specifies the minimum speed for all DVD media (1X).
     * kDVDSpeedMax specifies the maximum speed supported in hardware.
     * @result
     * Returns the status of the operation.
     */

    virtual IOReturn setSpeed(UInt16 kilobytesPerSecond);

    /*!
     * @function readDiscInfo
     * @discussion
     * Issue an MMC READ DISC INFORMATION command.
     * @param buffer
     * Buffer for the data transfer.  The size of the buffer implies the size of
     * the data transfer.
     * @param actualByteCount
     * Returns the actual number of bytes transferred in the data transfer.
     * @result
     * Returns the status of the data transfer.
     */

    virtual IOReturn readDiscInfo( IOMemoryDescriptor * buffer,
                                   UInt16 *             actualByteCount );

    /*!
     * @function readRZoneInfo
     * @discussion
     * Issue an MMC READ RZONE INFORMATION (READ TRACK INFORMATION) command.
     * @param buffer
     * Buffer for the data transfer.  The size of the buffer implies the size of
     * the data transfer.
     * @param address
     * As documented by MMC.
     * @param addressType
     * As documented by MMC.
     * @param actualByteCount
     * Returns the actual number of bytes transferred in the data transfer.
     * @result
     * Returns the status of the data transfer.
     */

    virtual IOReturn readRZoneInfo( IOMemoryDescriptor *    buffer,
                                    UInt32                  address,
                                    DVDRZoneInfoAddressType addressType,
                                    UInt16 *                actualByteCount );

    /*!
     * @function reportKey
     * @discussion
     * Issue an MMC REPORT KEY command.
     * @param buffer
     * Buffer for the data transfer.  The size of the buffer implies the size of
     * the data transfer.
     * @param keyClass
     * As documented by MMC.
     * @param address
     * As documented by MMC.
     * @param blockCount
     * As documented by MMC.
     * @param grantID
     * As documented by MMC.
     * @param format
     * As documented by MMC.
     * @result
     * Returns the status of the data transfer.
     */

    virtual IOReturn reportKey( IOMemoryDescriptor * buffer,
                                UInt8                keyClass,
                                UInt32               address,
                                UInt8                blockCount,
                                UInt8                grantID,
                                UInt8                format );

    OSMetaClassDeclareReservedUsed(IODVDMedia,  0);         /* reportKey */
    OSMetaClassDeclareReservedUnused(IODVDMedia,  1);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  2);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  3);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  4);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  5);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  6);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  7);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  8);
    OSMetaClassDeclareReservedUnused(IODVDMedia,  9);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 10);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 11);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 12);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 13);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 14);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 15);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 16);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 17);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 18);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 19);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 20);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 21);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 22);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 23);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 24);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 25);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 26);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 27);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 28);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 29);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 30);
    OSMetaClassDeclareReservedUnused(IODVDMedia, 31);
};

#endif /* __cplusplus */
#endif /* KERNEL */
#endif /* !_IODVDMEDIA_H */
