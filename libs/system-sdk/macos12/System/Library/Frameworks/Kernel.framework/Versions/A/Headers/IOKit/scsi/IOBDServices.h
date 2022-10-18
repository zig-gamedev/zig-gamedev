/*
 * Copyright (c) 2006-2009 Apple Inc. All rights reserved.
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


#ifndef _IOKIT_IO_BD_SERVICES_H_
#define _IOKIT_IO_BD_SERVICES_H_

#if defined(KERNEL) && defined(__cplusplus)


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// IOKit includes
#include <IOKit/IOTypes.h>

// Generic IOKit storage related headers
#include <IOKit/storage/IOBDTypes.h>
#include <IOKit/storage/IOBDBlockStorageDevice.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi/IOSCSIPeripheralDeviceType05.h>

class IOMemoryDescriptor;

//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------

class __exported IOBDServices : public IOBDBlockStorageDevice
{
	
	OSDeclareDefaultStructors ( IOBDServices )
		
protected:
	
	OSSet *								fClients;
	IOSCSIPeripheralDeviceType05 *		fProvider;
	
	virtual void	free ( void ) APPLE_KEXT_OVERRIDE;
	
    // Reserve space for future expansion.
    struct IOBDServicesExpansionData { };
    IOBDServicesExpansionData *		fIOBDServicesReserved;
	
public:
	
	static void			AsyncReadWriteComplete ( void * 		clientData,
		                          				 IOReturn		status,
		                      					 UInt64 		actualByteCount );
	
	// Deprecated
	virtual IOReturn	doAsyncReadWrite ( 	IOMemoryDescriptor *	buffer,
											UInt32					block,
											UInt32					nblks,
											IOStorageCompletion		completion );

	virtual IOReturn	doAsyncReadWrite ( 	IOMemoryDescriptor *	buffer,
											UInt64					block,
											UInt64					nblks,
											IOStorageCompletion		completion );
											
	virtual IOReturn	doAsyncReadWrite ( 	IOMemoryDescriptor *	buffer,
											UInt64					block,
											UInt64					nblks,
											IOStorageAttributes *   attributes,
											IOStorageCompletion *	completion ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	doEjectMedia ( void ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	doFormatMedia ( UInt64 byteCapacity ) APPLE_KEXT_OVERRIDE;

    virtual UInt32		doGetFormatCapacities ( UInt64 *	capacities,
												UInt32		capacitiesMaxCount ) const APPLE_KEXT_OVERRIDE;
    virtual IOReturn	doSynchronizeCache ( void ) APPLE_KEXT_OVERRIDE;
	
	virtual IOReturn	getWriteCacheState ( bool * enabled ) APPLE_KEXT_OVERRIDE;
	
	virtual IOReturn	setWriteCacheState ( bool enabled ) APPLE_KEXT_OVERRIDE;
        
    virtual char *		getVendorString ( void ) APPLE_KEXT_OVERRIDE;
    
    virtual char *		getProductString ( void ) APPLE_KEXT_OVERRIDE;
    
    virtual char *		getRevisionString ( void ) APPLE_KEXT_OVERRIDE;
    
    virtual char *		getAdditionalDeviceInfoString ( void ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn	reportBlockSize ( UInt64 * blockSize ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn	reportEjectability ( bool * isEjectable ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn	reportMediaState ( bool * mediaPresent, bool * changed ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn	reportMaxValidBlock ( UInt64 * maxBlock ) APPLE_KEXT_OVERRIDE;
        
    virtual IOReturn	reportRemovability ( bool * isRemovable ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn	reportWriteProtection ( bool * isWriteProtected ) APPLE_KEXT_OVERRIDE;

	/* CD Specific */
	virtual IOReturn	doAsyncReadCD ( IOMemoryDescriptor * buffer,
										UInt32 block,
										UInt32 nblks,
										CDSectorArea sectorArea,
										CDSectorType sectorType,
										IOStorageCompletion completion ) APPLE_KEXT_OVERRIDE;
	
	virtual IOReturn	readISRC ( UInt8 track, CDISRC isrc ) APPLE_KEXT_OVERRIDE;
	
	virtual IOReturn	readMCN ( CDMCN mcn) APPLE_KEXT_OVERRIDE;
	
	virtual IOReturn	readTOC ( IOMemoryDescriptor * buffer ) APPLE_KEXT_OVERRIDE;
	
	virtual IOReturn	audioPause ( bool pause );
	
	virtual IOReturn	audioPlay ( CDMSF timeStart, CDMSF timeStop );
	
	virtual IOReturn	audioScan ( CDMSF timeStart, bool reverse );
	
	virtual IOReturn	audioStop ( void );
	
	virtual IOReturn	getAudioStatus ( CDAudioStatus * status );
	
	virtual IOReturn	getAudioVolume ( UInt8 * leftVolume, UInt8 * rightVolume );
	
	virtual IOReturn	setAudioVolume ( UInt8 leftVolume, UInt8 rightVolume );

	virtual IOReturn	getSpeed ( UInt16 * kilobytesPerSecond ) APPLE_KEXT_OVERRIDE;
	
	virtual IOReturn	setSpeed ( UInt16 kilobytesPerSecond ) APPLE_KEXT_OVERRIDE;
	
	/* DVD Specific */
	virtual UInt32			getMediaType		( void ) APPLE_KEXT_OVERRIDE;
    
	virtual IOReturn		reportKey			( IOMemoryDescriptor * buffer,
												  const DVDKeyClass keyClass,
												  const UInt32 lba,
												  const UInt8 agid,
												  const DVDKeyFormat keyFormat ) APPLE_KEXT_OVERRIDE __attribute__ ((deprecated));

	virtual IOReturn		reportKey			( IOMemoryDescriptor * buffer,
												  const DVDKeyClass keyClass,
												  const UInt32 lba,
												  const UInt8 blockCount,
												  const UInt8 agid,
												  const DVDKeyFormat keyFormat ) APPLE_KEXT_OVERRIDE;
												  
	virtual IOReturn		sendKey				( IOMemoryDescriptor * buffer,
												  const DVDKeyClass keyClass,
												  const UInt8 agid,
												  const DVDKeyFormat keyFormat ) APPLE_KEXT_OVERRIDE;

	virtual IOReturn		readDVDStructure 	( 	IOMemoryDescriptor * 		buffer,
													const UInt8					structureFormat,
													const UInt32				logicalBlockAddress,
													const UInt8					layer,
													const UInt8 				agid ) APPLE_KEXT_OVERRIDE;

	/* BD Specific */
    virtual IOReturn		readDiscStructure	(	IOMemoryDescriptor * buffer,
													UInt8                structureFormat,
													UInt32               logicalBlockAddress,
													UInt8                layer,
													UInt8                agid,
													UInt8                mediaType ) APPLE_KEXT_OVERRIDE;

	/* 10.6.0 */
    virtual IOReturn		requestIdle ( void ) APPLE_KEXT_OVERRIDE;
	
	/* System Specific */
	virtual IOReturn 	message ( UInt32 type, IOService * provider, void * argument ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn	setProperties ( OSObject * properties ) APPLE_KEXT_OVERRIDE;

	/* User Client Specific */
	virtual bool start ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	virtual bool open ( IOService * client, IOOptionBits options, IOStorageAccess access );
	
	virtual bool handleOpen	( IOService * client, IOOptionBits options, void * access ) APPLE_KEXT_OVERRIDE;
	virtual void handleClose ( IOService * client, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
	virtual bool handleIsOpen ( const IOService * client ) const APPLE_KEXT_OVERRIDE;

	/* Added with 10.1.3 */
	virtual IOReturn	readTOC (	IOMemoryDescriptor *	buffer,
									CDTOCFormat				format,
									UInt8					msf,
									UInt8					trackSessionNumber,
									UInt16 *				actualByteCount ) APPLE_KEXT_OVERRIDE;
	
	/* Added with 10.1.3 */
	virtual IOReturn	readDiscInfo (	IOMemoryDescriptor *	buffer,
										UInt16 *				actualByteCount ) APPLE_KEXT_OVERRIDE;
	
	/* Added with 10.1.3 */
	virtual IOReturn	readTrackInfo (	IOMemoryDescriptor *	buffer,
										UInt32					address,
										CDTrackInfoAddressType	addressType,
										UInt16 *				actualByteCount ) APPLE_KEXT_OVERRIDE;
	
	/* Added with 10.5 */
	virtual IOReturn	splitTrack ( UInt32 address ) APPLE_KEXT_OVERRIDE;
	
private:
	
	// Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused ( IOBDServices, 1 );
    OSMetaClassDeclareReservedUnused ( IOBDServices, 2 );
    OSMetaClassDeclareReservedUnused ( IOBDServices, 3 );
    OSMetaClassDeclareReservedUnused ( IOBDServices, 4 );
    OSMetaClassDeclareReservedUnused ( IOBDServices, 5 );
    OSMetaClassDeclareReservedUnused ( IOBDServices, 6 );
    OSMetaClassDeclareReservedUnused ( IOBDServices, 7 );
    OSMetaClassDeclareReservedUnused ( IOBDServices, 8 );
	
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif /* _IOKIT_IO_BD_SERVICES_H_ */
