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

#ifndef _IOKIT_IO_SCSI_PERIPHERAL_DEVICE_TYPE_05_H_
#define _IOKIT_IO_SCSI_PERIPHERAL_DEVICE_TYPE_05_H_

#if defined(KERNEL) && defined(__cplusplus)


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// SCSI Architecture Model Family includes
#include <IOKit/scsi/IOSCSIMultimediaCommandsDevice.h>


//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------
class __exported IOSCSIPeripheralDeviceType05 : public IOSCSIMultimediaCommandsDevice
{
	
	OSDeclareDefaultStructors ( IOSCSIPeripheralDeviceType05 )
	
protected:
    
    // Reserve space for future expansion.
    struct IOSCSIPeripheralDeviceType05ExpansionData { };
    IOSCSIPeripheralDeviceType05ExpansionData * fIOSCSIPeripheralDeviceType05Reserved;
	
public:
    
    bool			init	( OSDictionary * propTable ) APPLE_KEXT_OVERRIDE;
    virtual bool	start	( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void 	stop	( IOService *  provider ) APPLE_KEXT_OVERRIDE;
	
private:
	
	// Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 1 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 2 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 3 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 4 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 5 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 6 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 7 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPeripheralDeviceType05, 8 );
    
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif	/* _IOKIT_IO_SCSI_PERIPHERAL_DEVICE_TYPE_05_H_ */
