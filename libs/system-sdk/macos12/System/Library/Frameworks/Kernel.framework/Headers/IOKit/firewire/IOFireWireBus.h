/*
 * Copyright (c) 1998-2002 Apple Computer, Inc. All rights reserved.
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
 * Copyright (c) 1999-2002 Apple Computer, Inc.  All rights reserved.
 *
 * HISTORY
 *
 */


#ifndef _IOKIT_IOFIREWIREBUS_H
#define _IOKIT_IOFIREWIREBUS_H

#include <IOKit/IOService.h>
#include <IOKit/firewire/IOFWIsochChannel.h>
#include <IOKit/firewire/IOFWAddressSpace.h>
#include <IOKit/firewire/IOFWCommand.h>
#include <IOKit/firewire/IOFireWireFamilyCommon.h>

extern const OSSymbol *gFireWireROM;
extern const OSSymbol *gFireWireNodeID;
extern const OSSymbol *gFireWireSelfIDs;
extern const OSSymbol *gFireWireSpeed;
extern const OSSymbol *gFireWireUnit_Spec_ID;
extern const OSSymbol *gFireWireUnit_SW_Version;
extern const OSSymbol *gFireWireVendor_ID;
extern const OSSymbol *gFireWire_GUID;
extern const OSSymbol *gFireWireVendor_Name;
extern const OSSymbol *gFireWireProduct_Name;
extern const OSSymbol *gFireWireModel_ID;
extern const OSSymbol *gFireWireTDM;

class IOFireWireDevice;
class IOLocalConfigDirectory;
class IOFWLocalIsochPort;
class IOFireWirePowerManager;
class IOFireWireBus;
class IOFWDCLPool;
class IOFWSimpleContiguousPhysicalAddressSpace;
class IOFWSimplePhysicalAddressSpace;
class IOFWUserObjectExporter;

#pragma mark -

/*! 
	@class IOFireWireBusAux
*/

class IOFireWireBusAux : public OSObject
{
    OSDeclareAbstractStructors(IOFireWireBusAux)

	friend class IOFireWireBus;
	
	protected:
			
		/*! 
			@struct ExpansionData
			@discussion This structure will be used to expand the capablilties of the class in the future.
		*/  
		
		struct ExpansionData { };
	
		/*! 
			@var reserved
			Reserved for future use.  (Internal use only)  
		*/
		
		ExpansionData * reserved;
	
	public :
	
		virtual IOFWDCLPool *							createDCLPool ( unsigned capacity ) const = 0 ;
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 32);
		virtual UInt8 getMaxRec( void ) = 0;

		virtual UInt64 getFireWirePhysicalAddressMask( void ) = 0;
		virtual UInt32 getFireWirePhysicalAddressBits( void ) = 0;
		virtual UInt64 getFireWirePhysicalBufferMask( void ) = 0;
		virtual UInt32 getFireWirePhysicalBufferBits( void ) = 0;

		virtual IOFWSimpleContiguousPhysicalAddressSpace * createSimpleContiguousPhysicalAddressSpace( vm_size_t size, IODirection direction ) = 0;
		virtual IOFWSimplePhysicalAddressSpace * createSimplePhysicalAddressSpace( vm_size_t size, IODirection direction ) = 0;

		virtual IOFWUserObjectExporter * getSessionRefExporter( void ) = 0;
	
	private:
		OSMetaClassDeclareReservedUsed(IOFireWireBusAux, 0);
		OSMetaClassDeclareReservedUsed(IOFireWireBusAux, 1);
		OSMetaClassDeclareReservedUsed(IOFireWireBusAux, 2);
		OSMetaClassDeclareReservedUsed(IOFireWireBusAux, 3);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 4);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 5);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 6);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 7);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 8);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 9);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 10);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 11);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 12);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 13);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 14);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 15);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 16);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 17);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 18);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 19);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 20);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 21);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 22);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 23);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 24);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 25);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 26);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 27);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 28);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 29);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 30);
		OSMetaClassDeclareReservedUnused(IOFireWireBusAux, 31);
		
};

#pragma mark -

/*! 
	@class IOFireWireBus
	@abstract IOFireWireBus is a public class the provides access to
		general FireWire functionality...
*/

class IOFireWireBus : public IOService
{
    OSDeclareAbstractStructors(IOFireWireBus)

public:

	struct DCLTaskInfoAux
	{
		unsigned version ;
		union	// u
		{
			struct	// v0
			{
				IOMemoryMap * bufferMemoryMap ;				// This field required to get physical addresses to program DMA.
															// If NULL, we try to make the map ourselves.
															// If you created a buffer memory descriptor
															// for your program's buffers, just call map() on it
															// and pass the returned object here...
			} v0 ;
			
			struct  // v1
			{
				IOMemoryMap * bufferMemoryMap ;				// same as 'bufferMemoryMap' from version 0, above.
				
				IOWorkLoop * workloop ;						// A workloop on which to run callbacks for this port..
															// Can be NULL to use FireWire isochronous workloop...
															// The workloop will be retained by the program object.
			} v1 ;
			
			struct
			{
				IOMemoryMap * bufferMemoryMap ;				// same as 'bufferMemoryMap' from version 0, above.
				
				IOWorkLoop * workloop ;						// A workloop on which to run callbacks for this port..
															// Can be NULL to use FireWire isochronous workloop...
															// The workloop will be retained by the program object.
				IOFWIsochPortOptions options ;				// extra options
			} v2 ;
		} u ;
	} ;
	
	// this struct has been redefined for our next generation isochronous architecture,
	// but is backwards compatible with the old definition.. This means we should
	// be safe when an old-style driver is loaded..
	// To use DCLTaskInfo (see createLocalIsochPort) make sure all 'unused' fields are set to 0 or NULL
	// and that auxInfo points to a valid DCLTaskInfoAux struct, defined above.
	
	struct DCLTaskInfo
	{
		task_t 				unused0 ;
		vm_address_t 		unused1 ;
		UInt32 				unused2 ;
		vm_address_t 		unused3 ;
		UInt32 				unused4 ;
		void				(*unused5)(void) ;
		DCLTaskInfoAux *	auxInfo ;	// Refcon for user call
	} ;

	static const IORegistryPlane * gIOFireWirePlane;

	IOFireWireBusAux * fAuxiliary;

    // Create an Isochronous Channel object
    virtual IOFWIsochChannel *createIsochChannel(
        bool doIRM, UInt32 bandwidth, IOFWSpeed prefSpeed,
        IOFWIsochChannel::ForceStopNotificationProc stopProc=NULL,
        void *stopRefCon=NULL) = 0;
 
	/*!	@function createLocalIsochPort
		@abstract Create a local isochronous port to run the given DCL program
		@param talking Pass true to create a talker port; pass false to create a listener port.
		@param opcodes A pointer to your DCL program (linked list of DCLCommand structs)
			To use an IOFWDCL/IOFWDCLPool program, pass the DCLCommand returned by
			IOFWDCLPool::getProgram().
		@param info (Optional) Pointer to DCLTaskInfo struct containing additional
			configuration information. If you have an IOMemoryMap for your DCL program data buffers,
			pass it here. You can also pass an IOWorkLoop if you want to use your own
			workloop to handle callbacks for the created port object.
		@param startEvent Specifies a bus condition on which the port should start receiving/sending packets
			Must be kFWDCLImmediateEvent, kFWDCLCycleEvent, or kFWDCLSyBitsEvent.
			Pass kFWDCLImmediateEvent to start without waiting when start() is called. Pass kFWDCLCycleEvent
			to start() transmitting at a specified bus cycle time. Pass kFWDCLSyBitsEvent (receive only)
			to start receiving packets once an isochronous packet with a specified sync field arrives.
		@param startState Pass the value for the desired start condition, as specified by 'startEvent'
			kFWDCLImmediateEvent: set to 0
			kFWDCLCycleEvent: the cycle timer value on which to start processing packets. For talker
				ports, This value will be masked by 'startMask' and packet processing will be begin on the 
				next cycle whose lowest bits match the masked value. For listener ports, pass a 15-bit value
				containg to the low order two bits of cycleSeconds and the 13-bit cycleCount on which to start
				processing packets.
			kFWDCLSyBitsEvent: The value of the sync field on which to start receive packets. The value will be masked
				by 'startMask'. For DCLCommand based isoch ports, processing will begin on the first received packet 
				that has an isochronous header sync field matching 'startState'. For IOFWDCL/IOFWDCLPool based
				ports, processing will pause on each IOFWDCL that has wait set to true until a packet that has
				an isochronous header sync field matching 'startState' is received.
		@result Returns an IOFWLocalIsochPort on success.*/
    virtual IOFWLocalIsochPort *createLocalIsochPort(bool talking,
        DCLCommand *opcodes, DCLTaskInfo *info = 0,
        UInt32 startEvent = 0, UInt32 startState = 0, UInt32 startMask = 0) = 0;

    virtual IOReturn getCycleTime(UInt32 &cycleTime) = 0;
    virtual IOReturn getBusCycleTime(UInt32 &busTime, UInt32 &cycleTime) = 0;

    // Methods to manipulate the local Config ROM
    virtual IOReturn AddUnitDirectory(IOLocalConfigDirectory *unitDir) = 0;
    virtual IOReturn RemoveUnitDirectory(IOLocalConfigDirectory *unitDir) = 0;

    // Cause a bus reset
    virtual IOReturn resetBus() = 0;

    // Convert a firewire nodeID into the IOFireWireDevice for it
    virtual IOFireWireDevice * nodeIDtoDevice(UInt32 generation, UInt16 nodeID) = 0;

    // Execute specified function on workloop after specified delay
    // Returned command is for delay, call it's cancel() function to cancel timeout.
    virtual IOFWDelayCommand * createDelayedCmd(UInt32 uSecDelay, FWBusCallback func, void *refcon) = 0;

    virtual IOFWPhysicalAddressSpace *createPhysicalAddressSpace(IOMemoryDescriptor *mem) = 0;
    virtual IOFWPseudoAddressSpace *createPseudoAddressSpace(FWAddress *addr, UInt32 len,
                                FWReadCallback reader, FWWriteCallback writer, void *refcon) = 0;


    // Extract info about the async request
    virtual bool isLockRequest(IOFWRequestRefCon refcon) = 0;
    virtual bool isQuadRequest(IOFWRequestRefCon refcon) = 0;
    virtual UInt32 getExtendedTCode(IOFWRequestRefCon refcon) = 0;

    // How big (as a power of two) can packets sent to/received from the node be?
    virtual int maxPackLog ( bool forSend, UInt16 nodeAddress) const = 0;

    // How big (as a power of two) can packets sent from A to B be?
    virtual int maxPackLog ( UInt16 nodeA, UInt16 nodeB) const = 0;

    // Force given node to be root (via root holdoff Phy packet)
    virtual IOReturn makeRoot ( UInt32 generation, UInt16 nodeID) = 0;

    // Create address space at fixed address in initial register space
    virtual IOFWPseudoAddressSpace * createInitialAddressSpace (	UInt32 addressLo, UInt32 len,
																	FWReadCallback reader, FWWriteCallback writer, void *refcon) = 0;

    // Get address space object for given address, if any
    virtual IOFWAddressSpace *						getAddressSpace(FWAddress address) = 0;

    // Extract info about the async request - was the request ack'ed complete already?
    virtual bool									isCompleteRequest(IOFWRequestRefCon refcon) = 0;
    
    virtual IOFWAsyncStreamCommand *				createAsyncStreamCommand( UInt32 generation,
															UInt32 channel, UInt32 sync, UInt32 tag, IOMemoryDescriptor *hostMem,
															UInt32 size, int speed,FWAsyncStreamCallback completion=NULL, void *refcon=NULL) = 0;
	virtual UInt32									hopCount(UInt16 nodeAAddress, UInt16 nodeBAddress ) = 0;
	virtual UInt32									hopCount(UInt16 nodeAAddress ) = 0;
	virtual IOFireWirePowerManager *				getBusPowerManager( void ) = 0;

protected:

	virtual IOFireWireBusAux *						createAuxiliary( void ) = 0;

	public :
	
		inline IOFWDCLPool *						createDCLPool ( UInt32 capacity = 0 )			{ return fAuxiliary->createDCLPool ( capacity ) ; }
		inline UInt8								getMaxRec( void )								{ return fAuxiliary->getMaxRec(); }

		// get the physical addressing limitations for this controller

		// returns the physical mask for memory addressable by the bus and this controller's DMA engine
		// intended for use with IOBufferMemoryDescriptor::inTaskWithPhysicalMask()
		// all current hardware is 32 bit --- currently returns 0x00000000FFFFFFFF
		// this API is intended for allocating physical buffers. 
		// it will not return more than 48 bits so that buffer addresses can be turned into FWAddresses
		inline UInt64 getFireWirePhysicalAddressMask( void )
			{ return fAuxiliary->getFireWirePhysicalAddressMask(); }
		
		// returns a count of the maximum addressing bits supported by the bus and this controller
		// intended for use with IODMACommand::withSpecification()
		// all current hardware is 32 bit --- currently returns 32
		// this API is intended for allocating physical buffers. 
		// it will not return more than 48 bits so that buffer addresses can be turned into FWAddresses
		inline UInt32 getFireWirePhysicalAddressBits( void )
			{ return fAuxiliary->getFireWirePhysicalAddressBits(); }

		// returns the physical mask for memory addressable by this controller's DMA engine
		// intended for use with IOBufferMemoryDescriptor::inTaskWithPhysicalMask()
		// all current hardware is 32 bit --- currently returns 0x00000000FFFFFFFF
		// this API is to allocate isoch and other buffers that don't need to be addressable by the bus 
		// it may someday return as high 64 bits
		inline UInt64 getFireWirePhysicalBufferMask( void )
			{ return fAuxiliary->getFireWirePhysicalBufferMask(); }
		
		// returns a count of the maximum addressing bits supported by this controller
		// intended for use with IODMACommand::withSpecification()
		// all current hardware is 32 bit --- currently returns 32
		// this API is to allocate isoch and other buffers that don't need to be addressable by the bus 
		// it may someday return as high 64 bits
		inline UInt32 getFireWirePhysicalBufferBits( void )
			{ return fAuxiliary->getFireWirePhysicalBufferBits(); }

		inline IOFWSimpleContiguousPhysicalAddressSpace * createSimpleContiguousPhysicalAddressSpace( vm_size_t size, IODirection direction )
			{ return fAuxiliary->createSimpleContiguousPhysicalAddressSpace( size, direction ); }
			
		inline IOFWSimplePhysicalAddressSpace * createSimplePhysicalAddressSpace( vm_size_t size, IODirection direction )
			{ return fAuxiliary->createSimplePhysicalAddressSpace( size, direction ); }

		virtual IOFWAsyncStreamCommand * createAsyncStreamCommand( UInt32 generation,
															UInt32 channel, UInt32 sync, UInt32 tag, IOMemoryDescriptor *hostMem,
															UInt32 size, int speed,FWAsyncStreamCallback completion, void *refcon, bool	failOnReset) = 0;
	
		inline IOFWUserObjectExporter * getSessionRefExporter( void )
			{ return fAuxiliary->getSessionRefExporter(); }
	
	private:
	
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 0);
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 1);
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 2);
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 3);
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 4);
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 5);
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 6);
		OSMetaClassDeclareReservedUsed(IOFireWireBus, 7);

};

#endif /* ! _IOKIT_IOFIREWIREBUS_H */

