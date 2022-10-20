/*
 * Copyright (c) 1998-2009 Apple Inc. All rights reserved.
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


#ifndef _IOKIT_IO_REDUCED_BLOCK_SERVICES_H_
#define _IOKIT_IO_REDUCED_BLOCK_SERVICES_H_

#if defined(KERNEL) && defined(__cplusplus)


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// Generic IOKit related headers
#include <IOKit/IOTypes.h>

// Generic IOKit storage related headers
#include <IOKit/storage/IOBlockStorageDevice.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi/IOSCSIPeripheralDeviceType0E.h>


//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------
class __exported IOReducedBlockServices : public IOBlockStorageDevice
{
	
	OSDeclareDefaultStructors ( IOReducedBlockServices )
	
	
protected:
    // Reserve space for future expansion.
    struct IOReducedBlockServicesExpansionData { };
    IOReducedBlockServicesExpansionData * fIOReducedBlockServicesReserved;
	
	IOSCSIPeripheralDeviceType0E *     fProvider;
	
	virtual bool	attach ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	virtual void	detach ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	virtual void	free ( void ) APPLE_KEXT_OVERRIDE;
	
	
public:

	virtual IOReturn 	message ( UInt32 type, IOService * provider, void * argument ) APPLE_KEXT_OVERRIDE;
	
	static void			AsyncReadWriteComplete ( void * 			clientData,
                                				 IOReturn			status,
                                				 UInt64 			actualByteCount );
                                				
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

private:
	
	// Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 1 );
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 2 );
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 3 );
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 4 );
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 5 );
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 6 );
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 7 );
    OSMetaClassDeclareReservedUnused ( IOReducedBlockServices, 8 );
    
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif	/* _IOKIT_IO_REDUCED_BLOCK_SERVICES_H_ */
