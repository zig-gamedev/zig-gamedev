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
 *	IOATACommand.h
 *
 */

#ifndef _IOATACOMMAND_H
#define _IOATACOMMAND_H

#include <libkern/c++/OSObject.h>
#include <IOKit/IOTypes.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/IOCommand.h>
#include "IOATATypes.h"


class IOExtendedLBA;
class IOATACommand;


/*! @typedef IOATACompletionFunction callback function for ATA disk devices.
*/
typedef void (IOATACompletionFunction)(IOATACommand* command );


/*!
@class IOATACommand

@discussion
Command structure superclass, created and freed only by IOATADevice objects 
populated by disk device drivers with command parameters
then submitted for operation to their IOATADevice provider for execution.

IOATACommand is a virtual class, a concrete subclass contains the methods and fields needed 
by IOATAControllers. Subclasses may be specific to particular controller hardware.
Disk device drivers will only have visibility to this interface and may not subclass this object.
Disk device drivers should instead make use of the void* refcon field which the controllers will not 
touch
*/

class IOATACommand : public IOCommand {

	OSDeclareAbstractStructors( IOATACommand );
	
	public:
	
	/*!@function zeroCommand
	@abstract set to blank state, MUST call prior to re-use of this object
	*/
	virtual void zeroCommand(void);  
	
	/*!@function setOpcode
	@abstract command opcode as defined in IOATATypes.
	*/
	virtual void setOpcode( ataOpcode inCode);

	/*!@function setFlags
	@abstract set the flags for this command, as defined in IOATATypes.
	*/
	virtual void setFlags( UInt32 inFlags);

	/*!@function setUnit
	@abstract set the unit number for this command.
	*/
	virtual void setUnit( ataUnitID inUnit);
	
	/*!@function setTimeoutMS
	@abstract how long to allow this command to complete, in milliseconds, once issued to
	 the hardware. if the time period expires, this command will return with a timeout error.
	*/
	virtual void setTimeoutMS( UInt32 inMs);

	/*!@function setCallbackPtr
	@abstract set the function pointer to call when this command completes.
	*/
	virtual void setCallbackPtr (IOATACompletionFunction* inCompletion);
	
	/*!@function setRegMask
	@abstract used when accessing registers or reading registers on an error result. Mask is defined
	in IOATATypes.h
	*/
	virtual void setRegMask( ataRegMask mask);

	// memory information
	// Since ATA hardware is limited in the amount of bytes 
	// that can be transfered in a command, the disk driver shall supply
	// a seperate offset and byte count per transfer.
	// the offset may be any amount. The byte count must be a multiple of the  
	// sector size of the device, ie, N * 512 bytes for ata hard drives.
	/*!@function setBuffer
	@abstract set the IIOMemoryDescriptor for this transaction.
	*/
	virtual void setBuffer ( IOMemoryDescriptor* inDesc);

	/*!@function setPosition
	@abstract used to set an offset into the memory descriptor for this transfer.
	*/
	virtual void setPosition (IOByteCount fromPosition);

	/*!@function setByteCount
	@abstract set the byte count for this transaction. Should agree with the device command and the 
	memory descriptor in use.
	*/
	virtual void setByteCount (IOByteCount numBytes);

	/*!@function setTransferChunkSize
	@abstract set the size of transfer between intervening interrupts. necessary when doing PIO Read/Write Multiple, etc. so the controller knows when to expect an interrupt during multi-sector data transfers.
	*/
	virtual void setTransferChunkSize( IOByteCount chunk = kATADefaultSectorSize);
	
	/*!@function setFeatures
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setFeatures( UInt8 in);

	/*!@function getErrorReg
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getErrorReg (void );
	
	/*!@function setSectorCount
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setSectorCount( UInt8 in);

	/*!@function getSectorCount
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getSectorCount (void );
	
	/*!@function setSectorNumber
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setSectorNumber( UInt8 in);

	/*!@function getSectorNumber
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getSectorNumber (void );
	
	/*!@function setCylLo
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setCylLo ( UInt8 in);

	/*!@function getCylLo
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getCylLo (void );
	
	/*!@function setCylHi
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setCylHi( UInt8 in);

	/*!@function getCylHi
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getCylHi (void );
	
	/*!@function setDevice_Head
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setDevice_Head( UInt8 in);

	/*!@function getDevice_Head
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getDevice_Head (void );
	
	/*!@function setCommand
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setCommand ( UInt8 in);

	/*!@function getStatus
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getStatus (void );
	
	/*!@function setLBA28
	@abstract convenience method that sets the taskfile registers into a 28-bit LBA address, with unit selected and LBA bit set. return err if param out of range, return kIOSuccess (kATANoErr) = 0 on return if successful
	*/
	virtual IOReturn setLBA28( UInt32 lba, ataUnitID inUnit);
	
	/*!@function setPacketCommand
	@abstract ATAPI command packet max size is 16 bytes. Makes deep copy of data. 
	*/
	virtual IOReturn setPacketCommand( UInt16 packetSizeBytes, UInt8* command); 
	
	// the following registers are only accessed by register access 
	// commands. Not by normal command dispatch where they are handled 
	// by the controller.
	
	// not part of task file params. not written to device when sending commands.
	virtual void setDataReg ( UInt16 in);
	virtual UInt16 getDataReg (void );

	// not part of taskfile. Not usually used except by controller.
	virtual void setControl ( UInt8 in);
	virtual UInt8 getAltStatus (void );

	// return values
	/*!@function getResult
	@abstract IOReturn value of the result of this command. ATA family errors are defined in IOATATypes.h
	*/
	virtual IOReturn getResult (void);

	/*!@function getBuffer
	@abstract the IOMemoryDescriptor used in this transaction.
	*/
	virtual IOMemoryDescriptor* getBuffer ( void ); 

	/*!@function getActualTransfer
	@abstract The byte count on the ending result, as best as can be determined by the controller. May be zero, but partial transfer may have occurred on error in some cases.
	*/
	virtual IOByteCount getActualTransfer ( void );

	/*!@function getEndStatusReg
	@abstract the value of the status register on the end of the command.
	*/
	virtual UInt8	getEndStatusReg (void);   // always returned

	/*!@function getEndErrorReg
	@abstract If the error bit was set in the status register, the value of the error register is returned at the end of a command.
	*/
	virtual UInt8 	getEndErrorReg( void );   

	/*!@function getCommandInUse
	@abstract returns true if IOATAController is still in control of the command.
	*/
	virtual bool	getCommandInUse( void );  // returns true if IOATAController is using the command.
	
	// for use by disk drivers, clients of IOATADevice only.
	// IOATADevice and IOATAControllers shall not use this field in any manner
	/*!@var refCon
	@abstract for use by disk drivers, clients of IOATADevice only. IOATADevice and IOATAControllers shall not use this field in any manner.
	*/
	void*	refCon;
	
	/*!@var refCon2
	@abstract for use by disk drivers, clients of IOATADevice only. IOATADevice and IOATAControllers shall not use this field in any manner.
	*/
	void* 	refCon2;

	protected:
  											// < 	return from ATA controllers to disk drivers
  											//  > 	sent to ATA controllers from disk drivers	
	ataOpcode			_opCode;			//  >	Command code for the controller.
	UInt32				_flags;		 		//  > 	Flags for this command
	ataRegisterImage 	_taskFile;   		// <> 	Taskfile + data and control registers.
	ATAPICmdPacket		_packet;	 		//  > 	ATAPI packet
	ataUnitID			_unit;				//  > 	Unit number
	UInt32				_timeoutMS;			//  > 	timeout command in ms.
	IOMemoryDescriptor* _desc;        		//  >	Buffer for data may be nil if command transfer no data
	IOByteCount			_position;			//  >	Position within the descriptor for this command
	IOByteCount			_byteCount;			//	>	How many bytes to transfer.
	IOByteCount			_logicalChunkSize;	//  >	How many bytes between intervening interrupts (R/W Multiple)
	ataRegMask			_regMask;			//  >	Which registers to write or read for reg access commands
	IOATACompletionFunction*	_callback;  // 	> 	if nil, command is synchronous
 
 	IOReturn			_result;			// < 	result
	IOByteCount			_actualByteCount;  	// <	actual bytes transfered.
 	UInt8				_status;			// <  	Status register at end of command
 	UInt8				_errReg;			// <	Error register at end of command if error bit set.
	bool				_inUse;				// <	true while IOATAController has possesion of the command


	virtual bool		init();
protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOWorkLoop in the future.
    */
    struct ExpansionData {IOExtendedLBA* extLBA; };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *fExpansionData;
	
	// overrides
	virtual void free();


private:
    OSMetaClassDeclareReservedUsed(IOATACommand, 0);   // set end result
    OSMetaClassDeclareReservedUsed(IOATACommand, 1); // get extendedLBAPtr
    OSMetaClassDeclareReservedUnused(IOATACommand, 2);
    OSMetaClassDeclareReservedUnused(IOATACommand, 3);
    OSMetaClassDeclareReservedUnused(IOATACommand, 4);
    OSMetaClassDeclareReservedUnused(IOATACommand, 5);
    OSMetaClassDeclareReservedUnused(IOATACommand, 6);
    OSMetaClassDeclareReservedUnused(IOATACommand, 7);
    OSMetaClassDeclareReservedUnused(IOATACommand, 8);
    OSMetaClassDeclareReservedUnused(IOATACommand, 9);
    OSMetaClassDeclareReservedUnused(IOATACommand, 10);
    OSMetaClassDeclareReservedUnused(IOATACommand, 11);
    OSMetaClassDeclareReservedUnused(IOATACommand, 12);
    OSMetaClassDeclareReservedUnused(IOATACommand, 13);
    OSMetaClassDeclareReservedUnused(IOATACommand, 14);
    OSMetaClassDeclareReservedUnused(IOATACommand, 15);
    OSMetaClassDeclareReservedUnused(IOATACommand, 16);
    OSMetaClassDeclareReservedUnused(IOATACommand, 17);
    OSMetaClassDeclareReservedUnused(IOATACommand, 18);
    OSMetaClassDeclareReservedUnused(IOATACommand, 19);
    OSMetaClassDeclareReservedUnused(IOATACommand, 20);

public:
	virtual void setEndResult(UInt8 inStatus, UInt8 endError  );
	virtual IOExtendedLBA* getExtendedLBA(void);
	

};


/*!
@class IOExtendedLBA

@discussion
If 48-bit LBAs are supported, IOExtendedLBA is used to represent a 48-bit LBA.
The driver examines the ATA identify data to determine if 48-bit addressing is
supported.
*/

class IOExtendedLBA : public OSObject
{
	OSDeclareDefaultStructors( IOExtendedLBA );
	
	public:
	static IOExtendedLBA* createIOExtendedLBA(IOATACommand* owner);
	
	// terminology as established in ATA/ATAPI-6. 
	// for the extended LBA address
	
	/*!@function setLBALow16
	@abstract convenience method that sets the lower 16 bits of a 48-bit LBA
	*/
	virtual void setLBALow16( UInt16 lbaLow);
	
	/*!@function getLBALow16
	@abstract convenience method that gets the lower 16 bits of a 48-bit LBA
	*/
	virtual UInt16 getLBALow16 (void);

	/*!@function setLBAMid16
	@abstract convenience method that sets the middle 16 bits of a 48-bit LBA
	*/
	virtual void setLBAMid16 (UInt16 lbaMid);

	/*!@function getLBAMid16
	@abstract convenience method that gets the middle 16 bits of a 48-bit LBA
	*/
	virtual UInt16 getLBAMid16( void );
	
	/*!@function setLBAHigh16
	@abstract convenience method that sets the high 16 bits of a 48-bit LBA
	*/
	virtual void setLBAHigh16( UInt16 lbaHigh );
	
	/*!@function getLBAHigh16
	@abstract convenience method that gets the high 16 bits of a 48-bit LBA
	*/
	virtual UInt16 getLBAHigh16( void );

	/*!@function setSectorCount16
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setSectorCount16( UInt16 sectorCount );
	
	/*!@function getSectorCount16
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt16 getSectorCount16( void );
	
	/*!@function setFeatures16
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setFeatures16( UInt16 features );
	
	/*!@function getFeatures16
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt16 getFeatures16( void );

	/*!@function setDevice
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setDevice( UInt8 inDevice );

	/*!@function getDevice
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getDevice( void );

	/*!@function setCommand
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual void setCommand( UInt8 inCommand );

	/*!@function getCommand
	@abstract  Taskfile access. Registers are named in accordance with ATA Standards conventions
	*/
	virtual UInt8 getCommand( void );

	/*!@function setExtendedLBA
	@abstract convenience method that sets the taskfile registers into a 48-bit LBA address, along with sector count, and unit selected and LBA bit set
	*/
	virtual void setExtendedLBA( UInt32 inLBAHi, UInt32 inLBALo, ataUnitID inUnit, UInt16 extendedCount, UInt8 extendedCommand);
	
	/*!@function getExtendedLBA
	@abstract convenience method that gets a 48-bit LBA
	*/
	virtual void getExtendedLBA(  UInt32* outLBAHi, UInt32* outLBALo );

	/*!@function zeroData
	@abstract convenience method that zeros out the lba, sector count, features, device, and command member variables
	*/
	virtual void zeroData(void);
	
	/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties in the future.
    */
    struct ExpansionData { };

	/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;


	protected:
	
	IOATACommand* owner;
	UInt16 lbaLow;
	UInt16 lbaMid;
	UInt16 lbaHigh;
	UInt16 sectorCount;
	UInt16 features;
	UInt16 device;
	UInt16 command;
	
	private:
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 0);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 1);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 2);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 3);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 4);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 5);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 6);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 7);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 8);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 9);
    OSMetaClassDeclareReservedUnused(IOExtendedLBA, 10);


};

#endif
