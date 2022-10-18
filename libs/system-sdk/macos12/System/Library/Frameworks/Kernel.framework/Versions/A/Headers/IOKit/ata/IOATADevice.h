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
/*
 * IOATADevice.h
 *
 * This object implements a relay to an ATA Bus where a drive is attached.
 */


#ifndef _IOATADEVICE_H
#define _IOATADEVICE_H

#include <IOKit/IOService.h>
#include <IOKit/IOTypes.h>
#include "IOATATypes.h"
#include "IOATACommand.h"
#include "IOATABusInfo.h"
#include "IOATADevConfig.h"

class IOATAController;


/*!
	@class IOATADevice
	@abstract This object implements a relay to an ATA Bus where a drive is attached.
	@discussion IOATADevice is the superclass which represents a particular device attached to a particular IOATAController (bus). IOATADevice is the provider for ATA mass-storage device drivers.IOATADevice is the factory for all IOATACommand objects and is responsible for creating and freeing IOATACommands. IOATAControllers will create an instance of IOATADevice for each device physically connected to the ata bus. IOATADevice is virtual and specific subclass should be implemented for particular types of IOATAController. In this manner, controller-specifc IOATACommands may be paired with the proper type of controller.
*/

class IOATADevice : public IOService
{
    OSDeclareDefaultStructors(IOATADevice);

public:

	// 
	/*!@function getUnitID
	@abstract Determine whether this device is number 0 or 1 (ie, master/slave)
	@result ataUnitID - 0 or 1.
	*/
	virtual ataUnitID	getUnitID( void );

	// 
	/*!@function getDeviceType
	@abstract Find out what kind of device this nub is (ata or atapi)
	@result ataDeviceType as defined in IOATATypes.h
	*/
	virtual ataDeviceType getDeviceType( void );

	// 
	/*!@function provideBusInfo
	@abstract Find out the bus capability so the client can choose the features to set and commands to run.
	@param getInfo a pointer to a valid IOATABusInfo object.
	@result kIOSuccess (0) and the getInfo object will be filled out by the bus controller with information about the bus.
	*/
 	virtual IOReturn provideBusInfo( IOATABusInfo* getInfo);  

	// 
	/*!@function selectConfig
	@abstract Tell the bus what speed to use for your device.
	@param configRequest pointer to a valid and initialized IOATADevConfig object.
	@result kIOSuccess (0) if the configuration was succesfully selected.
	@discussion This should only be called once during a disk drivers start method before registering its availability, and must be called prior to issuing any data IO transactions.
	*/
	virtual IOReturn selectConfig( IOATADevConfig* configRequest); 

	// 
	/*!@function provideConfig
	@abstract Find out what speed the bus has configured for this unit. 
	@param configRequest pointer to a valid IOATADevConfig object.
	@result kIOSuccess (0) on successful completion and configRequest will contain the configuration information. 
	*/
	virtual IOReturn provideConfig( IOATADevConfig* configRequest);    

	// Submit IO requests 
 	/*!
 	@function executeCommand
	@abstract Submit IO requests
	@param command pointer to a valid IOATACommand with the command to be executed.
	@result kIOSuccess (0) if the command was successfully queued in the controller. 
	*/
	virtual IOReturn		executeCommand(IOATACommand* command);
 
	// create and destroy IOATACommands
	/*!
	@function allocCommand
	@abstract create IOATACommands. Device drivers should allocate command objects only through this method.
	@result null if allocation failed. Retain count is one.
	*/
	virtual IOATACommand*	allocCommand( void );
	
	/*!
	@function freeCommand
	@abstract release a command object that is no longer needed. Do not free an object in use and do not release the object anymore times than you have retained it.
	@param inCommand the command to be released.
	*/
	virtual void			freeCommand( IOATACommand* inCommand); 

	// matching stuff for IOBSDInit and so on.	
	/*!@function matchPropertyTable
	@abstract matching stuff for IOBSDInit and so on.
	*/
	virtual bool matchPropertyTable(OSDictionary * table);
	
	/*!@function matchLocation
	@abstract matching stuff for IOBSDInit and so on.
	*/
	virtual IOService* matchLocation(IOService * client);
	
	/*!@function matchPropertyTable
	@abstract matching stuff for IOBSDInit and so on.	
	*/
	virtual bool matchPropertyTable(OSDictionary * table, SInt32 * score);

	// called by controllers when they need to send a message to client drivers.
	/*!
	@function notifyEvent
	@abstract called by controllers when they need to send a message to client (disk) drivers.
	*/
	virtual void notifyEvent( UInt32 event );

	
protected:
    IOATAController*     _provider;
    ataUnitID			_unitNumber;  // 0 - master, 1 - slave, -1 = not Valid
    ataDeviceType		_deviceType;  // ata, atapi, unknown	    
protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOWorkLoop in the future.
    */
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

private:
    OSMetaClassDeclareReservedUnused(IOATADevice, 0);
    OSMetaClassDeclareReservedUnused(IOATADevice, 1);
    OSMetaClassDeclareReservedUnused(IOATADevice, 2);
    OSMetaClassDeclareReservedUnused(IOATADevice, 3);
    OSMetaClassDeclareReservedUnused(IOATADevice, 4);
    OSMetaClassDeclareReservedUnused(IOATADevice, 5);
    OSMetaClassDeclareReservedUnused(IOATADevice, 6);
    OSMetaClassDeclareReservedUnused(IOATADevice, 7);
    OSMetaClassDeclareReservedUnused(IOATADevice, 8);
    OSMetaClassDeclareReservedUnused(IOATADevice, 9);
    OSMetaClassDeclareReservedUnused(IOATADevice, 10);
    OSMetaClassDeclareReservedUnused(IOATADevice, 11);
    OSMetaClassDeclareReservedUnused(IOATADevice, 12);
    OSMetaClassDeclareReservedUnused(IOATADevice, 13);
    OSMetaClassDeclareReservedUnused(IOATADevice, 14);
    OSMetaClassDeclareReservedUnused(IOATADevice, 15);
    OSMetaClassDeclareReservedUnused(IOATADevice, 16);
    OSMetaClassDeclareReservedUnused(IOATADevice, 17);
    OSMetaClassDeclareReservedUnused(IOATADevice, 18);
    OSMetaClassDeclareReservedUnused(IOATADevice, 19);
    OSMetaClassDeclareReservedUnused(IOATADevice, 20);    
};

#endif /* !_IOATABUSNUB_H */
