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
 
#ifndef _IOKIT_IOFWPHYSICALADDRESSSPACE_H
#define _IOKIT_IOFWPHYSICALADDRESSSPACE_H

#include <IOKit/firewire/IOFWAddressSpace.h>
#include <IOKit/IODMACommand.h>

/*
 * Direct physical memory <-> FireWire address.
 * Accesses to these addresses may be handled automatically by the
 * hardware without notification.
 *
 * The 64 bit FireWire address of (32 bit) physical addr xxxx:xxxx is hostNode:0000:xxxx:xxxx
 */

class IOFWPhysicalAddressSpace;

struct FWSegment
{
	FWAddress	address;
	UInt32		length;
};

#pragma mark -

/*! 
	@class IOFWPhysicalAddressSpaceAux
*/

class IOFWPhysicalAddressSpaceAux : public IOFWAddressSpaceAux
{
    OSDeclareDefaultStructors(IOFWPhysicalAddressSpaceAux)

	friend class IOFWAddressSpace;
	friend class IOFWPhysicalAddressSpace;
	
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
    
	ExpansionData *reserved;
	
	IODMACommand *	fDMACommand;
	bool			fDMACommandPrepared;
	
public:
	virtual bool init( IOFWAddressSpace * primary ) APPLE_KEXT_OVERRIDE;
	virtual	void free() APPLE_KEXT_OVERRIDE;

	void setDMACommand( IODMACommand * dma_command );
	IODMACommand * getDMACommand( void ); 
	UInt64 getPhysicalSegment( UInt64 offset, UInt64 * length );
	
	IOReturn prepare( void );
	IOReturn synchronize( IOOptionBits options );
	IOReturn complete( void );

	bool isPrepared( void );

	IOReturn getSegments( UInt64 * offset, FWSegment * fw_segments, UInt32 * num_segments );
	
private:
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 0);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 1);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 2);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 3);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 4);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 5);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 6);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 7);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 8);
    OSMetaClassDeclareReservedUnused(IOFWPhysicalAddressSpaceAux, 9);
	
};

#pragma mark -

/*! 
	@class IOFWPhysicalAddressSpace
*/

class IOFWPhysicalAddressSpace : public IOFWAddressSpace
{
    OSDeclareDefaultStructors(IOFWPhysicalAddressSpace)

	friend class IOFWPhysicalAddressSpaceAux;

protected:
    
	IOMemoryDescriptor *	fMem;	// unused
    vm_size_t				fLen;	// unused
	
    virtual	void	free() APPLE_KEXT_OVERRIDE;

public:

/*!	@function	init
	@abstract	Initialize physical address space.
	@param		bus	Points to IOFireWireBus object.
	@result		returns true if success, else false */
	virtual bool init( IOFireWireBus * bus ) APPLE_KEXT_OVERRIDE;
	
/*!	@function	initWithDesc
	@abstract	Initialize physical address space with IOMemoryDescriptor.
	@param		bus	Points to IOFireWireBus object.
	@param		mem	Points to IOMemoryDescriptor.
	@result		returns true if success, else false */
    virtual bool initWithDesc(IOFireWireBus *bus,
                                        IOMemoryDescriptor *mem);

/*!	@function	doRead
	@abstract	A method for processing an address space read request
	@param		nodeID	FireWire Read from nodeID.
	@param		speed	at this 'speed'.
	@param		addr	with FireWire address 'addr'.
	@param		len		read 'len' bytes from nodeID.
	@param		buf		points to a memory descriptor containing the packet data.
	@param		offset	start from this 'offset' in 'buf'.
	@param		refcon  Can be queried for extra info about the request.
	@result		UIn32	returns kFWResponseComplete on success */
    virtual UInt32 doRead(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len, 
							IOMemoryDescriptor **buf, IOByteCount * offset,
							IOFWRequestRefCon refcon) APPLE_KEXT_OVERRIDE;

/*!	@function	doWrite
	@abstract	A method for processing an address space write request
	@param		nodeID	FireWire Write to nodeID.
	@param		speed	at this 'speed'.
	@param		addr	with FireWire address 'addr'.
	@param		len		write 'len' bytes to nodeID.
	@param		buf		obtain bytes from location given by 'buf'.
	@param		reqrefcon  Can be queried for extra info about the request.
	@result		UIn32	returns kFWResponseComplete on success */
    virtual UInt32 doWrite(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
                           const void *buf, IOFWRequestRefCon refcon) APPLE_KEXT_OVERRIDE;

/*!	@function	getMemoryDescriptor
	@abstract	Gets the memory descriptor, which is associated to this 
				PhysicalAddressSpace.
	@param		none.
	@result		returns the IOMemoryDescriptor */
	IOMemoryDescriptor * getMemoryDescriptor( void );

/*!	@function	setMemoryDescriptor
	@abstract	Sets the memory descriptor, which will be associated to this 
				PhysicalAddressSpace.
	@param		none.
	@result		returns the IOMemoryDescriptor */
	IOReturn setMemoryDescriptor( IOMemoryDescriptor * descriptor );

/*!	@function	getLength
	@abstract	Get the length of the memory backed by PhysicalAddressSpace.
	@param		none.
	@result		returns the length */
	UInt64 getLength( void );

/*!	@function	setDMACommand
	@abstract	Set the DMACommand for this PhysicalAddressSpace.
	@param		dma_command	Points to IODMACommand object.
	@result		none */
	inline void setDMACommand( IODMACommand * dma_command )
		{ ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->setDMACommand( dma_command ); };
		
/*!	@function	getDMACommand
	@abstract	Get the DMACommand from this PhysicalAddressSpace.
	@param		none.
	@result		return previously assigned IODMACommand, null if not initialized */
	inline IODMACommand * getDMACommand( void )
		{ return ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->getDMACommand(); };

/*!	@function	initWithDMACommand
	@abstract	Initialize physical address space with IODMACommand.
	@param		bus	Points to IOFireWireBus object.
	@param		command	Points to IODMACommand.
	@result		returns true if success, else false */
	virtual bool initWithDMACommand( IOFireWireBus * control, IODMACommand * command );
	
/*!	@function	prepare
	@abstract	Prepare the IODMACommand used by this PhysicalAddressSpace.
	@param		none.
	@result		returns kIOReturnSuccess on success */
	inline IOReturn prepare( void )
		{ return ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->prepare(); };

/*!	@function	synchronize
	@abstract	synchronize the IODMACommand used by this PhysicalAddressSpace.
	@param		none.
	@result		returns kIOReturnSuccess on success */
	inline IOReturn synchronize( IOOptionBits options )
		{ return ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->synchronize( options ); };
		 
/*!	@function	complete
	@abstract	complete the IODMACommand used by this PhysicalAddressSpace.
	@param		none.
	@result		returns kIOReturnSuccess on success */
	inline IOReturn complete( void )
		{ return ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->complete(); };

/*!	@function	isPrepared
	@abstract	Inspects whether the IODMACommand was prepared in this PhysicalAddressSpace.
	@param		none.
	@result		returns true if prepared, else false */
	inline bool isPrepared( void )
		{ return ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->isPrepared(); };

/*!	@function	getSegments
	@abstract	Returns the scatter gather list of memory segments from the IODMACommand
				used in this PhysicalAddressSpace.
	@param		offset		input/output parameter, defines the starting and ending offset in the memory 
							descriptor, relative to any offset passed to the prepare() method.
				FWSegment 	Points to an array of memory segments.
				num_segments Size of the FWSegment array.   
	@result		returns kIOReturnSuccess on success */
	inline IOReturn getSegments( UInt64 * offset, FWSegment * fw_segments, UInt32 * num_segments )
		{ return ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->getSegments( offset, fw_segments, num_segments ); };

/*!	@function	checkMemoryInRange
	@abstract	Validates the IOMemoryDescriptor, which is used to initialize the PhysicalAddressSpace.
	@param		memory	Points to a valid IOMemoryDescriptor.
	@result		returns kIOReturnSuccess on success */
	IOReturn checkMemoryInRange( IOMemoryDescriptor * memory );
		
protected:

	UInt64 getPhysicalSegment( UInt64 offset, UInt64 * length )
		{ return ((IOFWPhysicalAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->getPhysicalSegment( offset, length); };
			
	virtual IOFWAddressSpaceAux * createAuxiliary( void ) APPLE_KEXT_OVERRIDE;
    	
};

#endif
