/*
 * Copyright (c) 1998-2008 Apple Inc. All rights reserved.
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

/*!
 @header ATADeviceNub.h
 @abstract A concrete implementation of IOATADevice.
 */
 
#ifndef _ATADEVICENUB_H
#define _ATADEVICENUB_H

#include <IOKit/IOTypes.h>
#include "IOATATypes.h"
#include "IOATADevice.h"
#include "IOATAController.h"

#include "IOATABusCommand.h"


/*!
@class ATADeviceNub
@abstract ATADeviceNub is a concrete implementation of IOATADevice.
@discussion clients of IOATA (disk drivers) should use the interface presented
by IOATADevice.  Concrete nubs are private to the IOATA family and 
specific subclasses of IOATADevice are instantiated by controller 
drivers to provide the abstract interface to clients.

*/
class ATADeviceNub : public IOATADevice
{
    OSDeclareDefaultStructors(ATADeviceNub);

public:

	/*!@function ataDeviceNub
	@abstract static creator function - used by IOATAControllers to create nubs.
	*/
	static ATADeviceNub* ataDeviceNub( IOATAController* provider, ataUnitID unit, ataDeviceType devType);
 
	/*!@function attach
	@abstract override of IOService method.
	*/
    virtual bool attach(IOService* provider );
 
 
 	// overrides from IOATADevice to provide actual client interface
 	
 
	/*!@function executeCommand
	@abstract Submit IO requests 
	*/
    virtual IOReturn executeCommand(IOATACommand* command);

	// create and destroy IOATACommands
	/*!@function allocCommand
	@abstract create command objects for clients.
	*/
	virtual IOATACommand*	allocCommand( void );
	
	/*!@function freeCommand
	@abstract Clients use this method to dispose of command objects.
	*/
	virtual void freeCommand( IOATACommand* inCommand); 
	
	
protected:
	
    /*!@function init
	@abstract used after creating the nub.
	*/
    virtual bool init(IOATAController* provider, ataUnitID unit, ataDeviceType devType);
	
	/*!@function publishProperties
	@abstract publish the nub's properties in the device tree.
	*/
	virtual void publishProperties( void );
	
	/*!@function publishBusProperties
	@abstract puts info about this device's bus capability in the device tree.
	*/
	virtual void publishBusProperties(void);
	
	/*!@function publishVendorProperties
	@abstract will be deprecated.
	*/
	virtual void publishVendorProperties( void );

	/*!@function getDeviceID
	@abstract get the unit id of this drive (0 or 1)
	*/
	virtual IOReturn getDeviceID(void);

	/*!@function MyATACallback
	@abstract to be deprecated.
	*/
	static void MyATACallback(IOATACommand* command );
	
	/*!@function processCallback
	@abstract to be deprecated.
	*/
	void processCallback(IOATACommand* command );
	
	/*!@function swapBytes16
	@abstract to be deprecated.
	*/
	void swapBytes16( UInt8* dataBuffer, IOByteCount length);
    
    UInt8* buffer;
	
protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the ATADeviceNub in the future.
    */
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

private:
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 0);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 1);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 2);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 3);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 4);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 5);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 6);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 7);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 8);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 9);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 10);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 11);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 12);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 13);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 14);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 15);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 16);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 17);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 18);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 19);
    OSMetaClassDeclareReservedUnused(ATADeviceNub, 20);

};

#endif /* !_IOATABUSNUB_H */
