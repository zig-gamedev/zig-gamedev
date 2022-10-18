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
 *
 *	IOATABusCommand.h
 *
 */
 
#ifndef _IOATABUSCOMMAND_H
#define _IOATABUSCOMMAND_H

#include <IOKit/IOTypes.h>
#include "IOATATypes.h"
#include "IOATACommand.h"


class IOSyncer;


/*!

@class IOATABusCommand

@discussion ATA Device (disk) drivers should use the superclass, IOATACommand 
and may not derive or use any subclass of IOATACommand. 

IOATABusCommand is the subclass of IOATACommand used by 
IOATAControllers. Controller classes may override this class to 
provide additional fields as their needs dictate or may use this 
as a concrete class if it is sufficient.

IOATAControllers are always paired with specific IOATADevices 
and each specific subclass of IOATADevice is in turn the factory method 
for IOATACommands for use by disk drivers.

In this manner, mass-storage device drivers (disk drivers, clients of 
ATA bus controllers) see only the generalized interface of IOATADevice 
and the generalized interface of IOATACommand. This provides isolation 
from specific bus details for disk drivers and offers flexibility to 
controllers to add per-command fields and state variables for their own 
internal use.

*/

class IOATABusCommand : public IOATACommand {

	OSDeclareDefaultStructors( IOATABusCommand );
	
	public:
	
	// data items for use by IOATAController
	
	/*! @var queueChain queue header for use by IOATAController. */
	queue_chain_t	queueChain; 

	/*! @var state state-semaphore for use by IOATAController */
	UInt32 state;
	
	/*! @var syncer IOSyncer for use by IOATAController */
	IOSyncer* syncer;



	/*!@function allocateCmd
	@abstract factory method to create an instance of this class used by subclasses of IOATADevice
	*/
	static IOATABusCommand* allocateCmd(void);

	/*!@function zeroCommand
	@abstract set to blank state, call prior to re-use of this object
	*/
	virtual void zeroCommand(void);  	  	  	
  	
 	/*!@function getOpcode
	@abstract return the command opcode
	*/
	virtual ataOpcode getOpcode( void );  
	
	/*!@function getFlags
	@abstract return the flags for this command.
	*/
	virtual ataFlags getFlags ( void ); 
		
	/*!@function getRegMask
	@abstract  get the register mask for desired regs
	*/
	virtual ataRegMask getRegMask( void );
	
	/*!@function getUnit
	@abstract return the unit id (0 master, 1 slave)
	*/
	virtual ataUnitID getUnit( void ); 
	
	/*!@function getTimeoutMS
	@abstract return the timeout value for this command
	*/
	virtual UInt32 getTimeoutMS (void ); 
	
	/*!@function setResult
	@abstract set the result code
	*/
	virtual void setResult( IOReturn );

	/*!@function getCallbackPtr
	@abstract return the callback pointer
	*/
	virtual IOATACompletionFunction* getCallbackPtr (void ); 
	
	
	/*!@function executeCallback
	@abstract call the completion callback function
	*/
	virtual void executeCallback(void);

	/*!@function getTaskFilePtr
	@abstract return the taskfile structure pointer.
	*/
	virtual ataTaskFile* getTaskFilePtr(void);
	
	/*!@function getPacketSize
	@abstract return the size of atapi packet if any.
	*/
	virtual UInt16 getPacketSize(void);
	
	/*!@function getPacketData
	@abstract return pointer to the array of packet data. 
	*/
	virtual UInt16*	getPacketData(void);
	
	/*!@function getTransferChunkSize
	@abstract number of bytes between interrupts.
	*/
	virtual IOByteCount getTransferChunkSize(void);

	/*!@function setActualTransfer
	@abstract set the byte count of bytes actually transferred.
	*/
	virtual void setActualTransfer ( IOByteCount bytesTransferred );
	
	/*!@function getBuffer
	@abstract get pointer to the memory descriptor for this transaction
	*/
	virtual IOMemoryDescriptor* getBuffer ( void);
	
	/*!@function getPosition
	@abstract the position within the memory buffer for the transaction.
	*/
	virtual IOByteCount getPosition (void);
	
	/*!@function getByteCount
	@abstract return the byte count for this transaction to transfer.
	*/
	virtual IOByteCount getByteCount (void);
	
	/*!@function setCommandInUse
	@abstract mark the command as being in progress.
	*/
	virtual void setCommandInUse( bool inUse = true);


	protected:
	
	//
	/*!@function init
	@abstract Zeroes all data, returns false if allocation fails. protected.
	*/
  	virtual bool init();  

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOWorkLoop in the future.
    */
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

private:
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 0);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 1);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 2);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 3);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 4);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 5);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 6);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 7);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 8);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 9);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 10);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 11);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 12);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 13);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 14);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 15);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 16);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 17);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 18);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 19);
    OSMetaClassDeclareReservedUnused(IOATABusCommand, 20);
};

#include <IOKit/IODMACommand.h>

class IOATABusCommand64 : public IOATABusCommand
{

	OSDeclareDefaultStructors( IOATABusCommand64 );
	
	public:
	
	// new features
	static IOATABusCommand64* allocateCmd32(void);	
	virtual IODMACommand* GetDMACommand( void );
	
	
	
	// overrides for IODMACommand setup
	virtual void zeroCommand(void);  	 
	virtual void setBuffer ( IOMemoryDescriptor* inDesc);
	virtual void setCommandInUse( bool inUse = true);
	virtual void executeCallback(void);


	protected:
	IODMACommand* _dmaCmd;
	virtual bool init();  
	virtual void free();
};

#endif /*_IOATABUSCOMMAND_H*/
