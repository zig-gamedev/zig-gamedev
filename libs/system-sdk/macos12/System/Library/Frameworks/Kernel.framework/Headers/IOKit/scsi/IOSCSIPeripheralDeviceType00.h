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

#ifndef _IOKIT_IO_SCSI_PERIPHERAL_DEVICE_TYPE_OO_H_
#define _IOKIT_IO_SCSI_PERIPHERAL_DEVICE_TYPE_OO_H_

#if defined(KERNEL) && defined(__cplusplus)


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// SCSI Architecture Model Family includes
#include <IOKit/scsi/IOSCSIBlockCommandsDevice.h>


//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------
class __exported IOSCSIPeripheralDeviceType00 : public IOSCSIBlockCommandsDevice
{
	
	OSDeclareDefaultStructors ( IOSCSIPeripheralDeviceType00 )
	
protected:
	
	// Reserve space for future expansion.
	struct IOSCSIPeripheralDeviceType00ExpansionData
	{
		OSSet *	fClients;
	};
	IOSCSIPeripheralDeviceType00ExpansionData * fIOSCSIPeripheralDeviceType00Reserved;
	
public:
	
	bool				init ( OSDictionary * propTable ) APPLE_KEXT_OVERRIDE;
	virtual bool		start ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	virtual void		free ( void ) APPLE_KEXT_OVERRIDE;

	virtual bool		handleOpen (
							  IOService * 	client,
							  IOOptionBits 	options,
							  void * 		access ) APPLE_KEXT_OVERRIDE;
	
	virtual void		handleClose (
							IOService * 	client,
							IOOptionBits 	options ) APPLE_KEXT_OVERRIDE;
	
	virtual bool		handleIsOpen ( const IOService * client ) const APPLE_KEXT_OVERRIDE;
	
    virtual IOReturn	newUserClient (
    						   task_t			owningTask,
    						   void *			securityID,
    						   UInt32			type,
    						   OSDictionary * 	properties,
							   IOUserClient **	handler ) APPLE_KEXT_OVERRIDE;
	virtual void 		CreateStorageServiceNub ( void ) APPLE_KEXT_OVERRIDE;

	virtual char *		GetVendorString ( void ) APPLE_KEXT_OVERRIDE;
	virtual char *		GetProductString ( void ) APPLE_KEXT_OVERRIDE;
	virtual char *		GetRevisionString ( void ) APPLE_KEXT_OVERRIDE;
	
	
private:
	
#if !TARGET_OS_IPHONE
	// Space reserved for future expansion.
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 1 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 2 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 3 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 4 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 5 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 6 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 7 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType00, 8 );
#endif /* !TARGET_OS_IPHONE */
	
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif	/* _IOKIT_IO_SCSI_PERIPHERAL_DEVICE_TYPE_OO_H_ */
