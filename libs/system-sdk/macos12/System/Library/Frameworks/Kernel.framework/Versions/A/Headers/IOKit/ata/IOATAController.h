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
 
 
#ifndef _IOATACONTROLLER_H
#define _IOATACONTROLLER_H

#include <IOKit/IOTypes.h>
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include "ATATimerEventSource.h"

class IOATADevice;
class IOATABusCommand;
class IOATABusInfo;
class IOATADevConfig;

/*! @class IOATAController
    @abstract The base class for ata controller family. Provides the interface common to all ata bus controllers.
    @discussion Subclasses of IOATAController implement drivers for specific bus hardware. Disk devices are clients of 
    IOATAController and communicate via the IOATABusNub instantiated for each device discovered by the specific IOATAController
    subclass when it probes hardware.  Via the nub, the ATA Controller provides standard information about bus capability, accepts 
    requests for transfer mode configuration (timing), accepts requests for IO and bus operations, notifies the device driver about 
    bus events which may affect the device, such as soft-resets or device removal (ie, media-bay and PC-card) and removal of queued 
    IO requests which have not been dispatched into the hardware. 
    
    @discussion The header doc for this class is incomplete. The source however is heavily commented and should be consulted until 
    such time as complete header doc is available.
*/



class IOATAController : public IOService
{
    OSDeclareDefaultStructors(IOATAController);

public:


	/*--- Common ATA Controller Interface ---*/
	// find out bus capability
	virtual IOReturn provideBusInfo( IOATABusInfo* infoOut);

	// set and get bus timing configuration for a specific unit 
	virtual IOReturn selectConfig( IOATADevConfig* configRequest, UInt32 unitNumber);
	virtual IOReturn getConfig( IOATADevConfig* configRequest, UInt32 unitNumber); 

	// The main call puts something on the work loop
	virtual IOReturn executeCommand( IOATADevice* nub, IOATABusCommand* cmd);	

	
	
	/*-- Power Management ---*/
	
	// TBD


	/*--- Overrides from IOService ---*/
	virtual bool init(OSDictionary * properties);

	virtual IOService* probe( IOService* provider,	SInt32*	score );
    virtual bool start( IOService* provider );


protected:
	
	enum {  	
			kBusFree = 'free',   // bus is available
			kBusBusy = 'busy',   // bus is busy with request
			kQueueOpen = '!lck',  // queue is not locked
			kQueueLocked = 'LOCK', // queue is frozen and not
			kImmediateLocked = '!Imd', // immediate commands may not be processed
			kImmediateOK	=  'Immd'  // immediate commands allowed.
		};	

	/* Transaction State indicator definition - indicates what action is next*/
	enum transState {
		kATAInitial					= 0x00,  // in queue
		kATAStarted					= 0x01,  // issue taskfile  
		kATAPICmd					= 0x02,	 // issue packet						
		kATADataTx					= 0x03,  // data transfer phase
		kATAStatus					= 0x04,  // read status
		kATAComplete				= 0x05,  // io complete
		kATADone					= 0x06   // completion callout called
	};
	
	struct ataDevInfo {
		ataDeviceType   type;       // ata, atapi, unknown
		atapiConfig		packetSend; // slow DRQ, IRQ, or fast DRQ for packet
	};
	

	struct ataDoubleBuffer
	{
		IOPhysicalAddress	physicalBuffer;
		IOLogicalAddress	logicalBuffer;
		IOByteCount	bufferSize;
		
	};

	
 	IOService*					_provider;
    IOWorkLoop*					_workLoop;
    IOCommandGate*      		_cmdGate;
	ATATimerEventSource*			_timer;
	queue_head_t				_commandQueue;
	IOATABusCommand*			_currentCommand;
	UInt32						_busState;
	UInt32						_queueState;
	UInt32						_immediateGate;
	ataUnitID					_selectedUnit;
	ataDevInfo					_devInfo[2];
	IOATADevice*				_nub[2];
	ataDoubleBuffer				_doubleBuffer;
	
	IOATARegPtr8				_tfFeatureReg;
	IOATARegPtr8				_tfSCountReg;
	IOATARegPtr8				_tfSectorNReg;
	IOATARegPtr8				_tfCylLoReg;
	IOATARegPtr8				_tfCylHiReg;
	IOATARegPtr8				_tfSDHReg;
	IOATARegPtr8				_tfStatusCmdReg;  
	IOATARegPtr16				_tfDataReg;
	IOATARegPtr8				_tfAltSDevCReg;   

	
	// false if couldn't allocate the per-bus double buffer.
	// controllers should provide implementation where needed 
	// for DMA hardware compatibility. The default method provides
	// a 4K buffer for PIO since MemoryDescriptors do not by default have 
	// logical addresses in the kernel space.
	virtual bool allocateDoubleBuffer( void );

	// perform 2-byte endian swap. Only useful on PIO transfers and identify data
	virtual void swapBytes16( UInt8* dataBuffer, IOByteCount length);

/*! @function handleCommand
    @abstract Called by executeCommand() to handle the client command
    from the workloop context.
    @param command The command code.
    @param param1 Command parameter.
    @param param2 Command parameter.
    @param param3 Command parameter.
	@result kIOReturnSuccess on success, or an error code otherwise. */

    virtual IOReturn handleCommand( void *     command,
                                   	void *     param1 = 0,
                                   	void *     param2 = 0,
                                   	void *     param3 = 0);


/*! @function busCanDispatch
    @abstract answers whether the bus is in state such that the next command 
    can be dispatched.
	@result true - bus is free to issue commands. false - bus cannot issue
	commands at this time. */
	virtual bool busCanDispatch( void );


/*! @function dispatchNext
    @abstract Causes the command at the front of the queue to dequeue, made the 
    current command and begin execution.
	@result  noErr indicates successful dispatch.  */
	virtual IOReturn dispatchNext( void );

	// sets the result code, free's the bus state, dispatch next command and execute completion
	virtual void completeIO( IOReturn commandResult );

	// Command queue handlers.	
	virtual IOReturn enqueueCommand( IOATABusCommand* command);	
	virtual IOATABusCommand* dequeueFirstCommand( void );

	// event notifier for clients
	virtual void executeEventCallouts(  ataEventCode event, ataUnitID unit);

	// default handler for device interrupts.
	virtual IOReturn handleDeviceInterrupt( void );


	// timer functions
	// starts the timeout on the current command
	virtual IOReturn startTimer( UInt32 inMS);
	//disable and clear a running timer.
	virtual void stopTimer( void );
	// called when a timeout occurs.
	virtual void handleTimeout( void );
	// true if the timer has expired 
	virtual bool checkTimeout( void );

	// handle IO opcodes
	virtual IOReturn handleExecIO( void );
	virtual IOReturn handleRegAccess( void );
	virtual IOReturn handleBusReset(void);
	virtual IOReturn handleQueueFlush( void );
	
	// various protocol phases
	virtual IOReturn asyncData(void);
	virtual IOReturn asyncStatus(void);
	virtual IOReturn asyncIO(void);
	virtual IOReturn asyncCommand(void);
	virtual IOReturn synchronousIO(void);	
	
	// hardware access
	virtual IOReturn selectDevice( ataUnitID unit );
	virtual IOReturn issueCommand( void );
	virtual IOReturn writePacket( void );
	virtual IOReturn softResetBus( bool doATAPI = false );
	
	virtual IOReturn startDMA( void );
	virtual IOReturn stopDMA( void );

	virtual bool ATAPISlaveExists( void );
	virtual UInt32 scanForDrives( void );

	virtual bool waitForU8Status (UInt8 mask, UInt8 value);
	
	virtual IOByteCount readATAPIByteCount( void );
	virtual void handleOverrun( IOByteCount length);
	virtual IOReturn registerAccess(bool isWrite);
	
	// PIO data transfers
	virtual IOReturn txDataIn (IOLogicalAddress buf, IOByteCount length);
	virtual IOReturn txDataOut (IOLogicalAddress buf, IOByteCount length);

	virtual IOATAController::transState	determineATAPIState(void);

	// device should set the controller to the config for this device
	virtual void selectIOTiming( ataUnitID unit );

	// subclasses MUST implement this function in order to initialize
	// the pointers to the ATA task file registers during start() time.
	virtual bool configureTFPointers(void);

	// convert a bit-significant indicator to a numeric value.
	virtual UInt16 bitSigToNumeric( UInt16 binary);

	// for 48 bit register reading and writing 
	UInt16 readExtRegister( IOATARegPtr8 inRegister );
	void writeExtRegister( IOATARegPtr8 inRegister, UInt16 inValue );

	// overrides
	virtual void free();
	
private:

	// used called by the commandgate in executeCommand.
    static void executeCommandAction(OSObject * owner,
                                     void *     arg0,
                                     void *     arg1,
                                     void *     arg2,
                                     void *     arg3);

	// callout used by the timer to indicate the timeout failure.
	static void timeoutOccured(	OSObject *owner, 
								IOTimerEventSource *sender);

protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOATAController in the future.
    */
    typedef struct ExpansionData
    {
    	IOBufferMemoryDescriptor*	_doubleBufferDesc;
    } ExpansionData;

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

private:
    OSMetaClassDeclareReservedUnused(IOATAController, 0);
    OSMetaClassDeclareReservedUnused(IOATAController, 1);
    OSMetaClassDeclareReservedUnused(IOATAController, 2);
    OSMetaClassDeclareReservedUnused(IOATAController, 3);
    OSMetaClassDeclareReservedUnused(IOATAController, 4);
    OSMetaClassDeclareReservedUnused(IOATAController, 5);
    OSMetaClassDeclareReservedUnused(IOATAController, 6);
    OSMetaClassDeclareReservedUnused(IOATAController, 7);
    OSMetaClassDeclareReservedUnused(IOATAController, 8);
    OSMetaClassDeclareReservedUnused(IOATAController, 9);
    OSMetaClassDeclareReservedUnused(IOATAController, 10);
    OSMetaClassDeclareReservedUnused(IOATAController, 11);
    OSMetaClassDeclareReservedUnused(IOATAController, 12);
    OSMetaClassDeclareReservedUnused(IOATAController, 13);
    OSMetaClassDeclareReservedUnused(IOATAController, 14);
    OSMetaClassDeclareReservedUnused(IOATAController, 15);
    OSMetaClassDeclareReservedUnused(IOATAController, 16);
    OSMetaClassDeclareReservedUnused(IOATAController, 17);
    OSMetaClassDeclareReservedUnused(IOATAController, 18);
    OSMetaClassDeclareReservedUnused(IOATAController, 19);
    OSMetaClassDeclareReservedUnused(IOATAController, 20);
};
#endif /* !_IOATACONTROLLER_H */
