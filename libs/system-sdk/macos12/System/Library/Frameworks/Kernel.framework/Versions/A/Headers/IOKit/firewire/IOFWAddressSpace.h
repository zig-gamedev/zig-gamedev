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
 *
 *	IOFWAddressSpace.h
 *
 * Classes which describe addresses in the local node which are accessable to other nodes
 * via firewire asynchronous read/write/lock requests.
 */
 
#ifndef _IOKIT_IOFWADDRESSSPACE_H
#define _IOKIT_IOFWADDRESSSPACE_H

#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/firewire/IOFireWireFamilyCommon.h>

class IOFireWireDevice;
class IOFireWireBus;
class IOFireWireController;

typedef void * IOFWRequestRefCon;

/*!	@function	FWWriteCallback
	@abstract	Callback called when a write request packet is received for
				a 'virtual' firewire address.
	@param		device	is the node originating the request
	@param		speed	is the FireWire speed of the request, update it if you need to control
						the speed of the reply, otherwise the response will be the same speed.
	@param		addr	is the address the device is requesting to write to
	@param		len		is the number of bytes to write
	@param		buf		contains the packet data
	@param		requestRefcon	refcon Can be queried for extra info about the request,
								using IOFireWireController::isLockRequest(), isQuadRequest()
	@result		return:
				kFWResponseComplete			= 0,	OK
				kFWResponseConflictError	= 4,	Resource conflict, may retry
				kFWResponseDataError		= 5,	Data not available
				kFWResponseTypeError		= 6,	Operation not supported
				kFWResponseAddressError		= 7		Address not valid in target device */
typedef UInt32 (*FWWriteCallback)(void *refcon, UInt16 nodeID, IOFWSpeed &speed,
                                  FWAddress addr, UInt32 len, const void *buf, IOFWRequestRefCon requestRefcon);

/*!	@function	FWReadCallback
	@abstract	Callback called when a read request packet is received for
				a 'virtual' firewire address.
	@param		 nodeID is the node originating the request
	@param		speed is the FireWire speed of the request, update it if you need to control
						the speed of the reply, otherwise the response will be the same speed.
	@param		addr is the address the device is requesting to read from
	@param		len is the number of bytes to read
	@param		buf contains the packet data
	@param		offset on return points to the offset into *buf of the packet data
	@param		requestRefcon   refcon to pass back if sending a delayed response. Also can be queried 
								for extra info about the request
	@result		return:
    kFWResponsePending			= -1,	Pseudo response, real response sent later.
	kFWResponseComplete			= 0,	OK!
	kFWResponseConflictError	= 4,	Resource conflict, may retry
	kFWResponseDataError		= 5,	Data not available
	kFWResponseTypeError		= 6,	Operation not supported
	kFWResponseAddressError		= 7		Address not valid in target device 

	A return of kFWResponsePending should be followed at some later time by a call to
	IOFireWireController::asyncReadResponse	*/ 
typedef UInt32 (*FWReadCallback)(void *refcon, UInt16 nodeID, IOFWSpeed &speed,
                                FWAddress addr, UInt32 len, IOMemoryDescriptor **buf,
                                 IOByteCount * offset, IOFWRequestRefCon requestRefcon);

class IOFWAddressSpace;

#pragma mark -

/*! @class IOFWAddressSpaceAux
	@discussion An IOFWAddressSpaceAux is for internal use only. You should never subclass IOFWAddressSpaceAux 
*/

class IOFWAddressSpaceAux : public OSObject
{
    OSDeclareDefaultStructors(IOFWAddressSpaceAux)

	friend class IOFWAddressSpace;
	
protected:
	
	IOFWAddressSpace * 		fPrimary;
	IOFireWireController *	fControl;
	
	OSSet *					fTrustedNodeSet;
    OSIterator *			fTrustedNodeSetIterator;
	
	bool					fExclusive;
	
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

    virtual bool init( IOFWAddressSpace * primary );
	virtual	void free() APPLE_KEXT_OVERRIDE;

	virtual bool isTrustedNode( UInt16 nodeID );
	virtual void addTrustedNode( IOFireWireDevice * device );
	virtual void removeTrustedNode( IOFireWireDevice * device );
	virtual void removeAllTrustedNodes( void );

	bool isExclusive( void );
	void setExclusive( bool exclusive );
	
	virtual bool intersects( IOFWAddressSpace * space );
		
private:
    OSMetaClassDeclareReservedUsed(IOFWAddressSpaceAux, 0);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 1);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 2);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 3);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 4);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 5);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 6);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 7);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 8);
    OSMetaClassDeclareReservedUnused(IOFWAddressSpaceAux, 9);
	
};

#pragma mark -

/*
 * Base class for FireWire address space objects
 */
 
/*! 
	@class IOFWAddressSpace
*/

class IOFWAddressSpace : public OSObject
{
    OSDeclareAbstractStructors(IOFWAddressSpace)

	friend class IOFWAddressSpaceAux;
	
protected:
    
	IOFireWireController *fControl;

	/*!
		@struct ExpansionData
		@discussion This structure will be used to expand the capablilties of the class in the future.
    */  
	  
    struct ExpansionData 
	{ 
		IOFWAddressSpaceAux * fAuxiliary; 
	};

	/*! 
		@var reserved
		Reserved for future use.  (Internal use only)  
	*/
	
    ExpansionData * fIOFWAddressSpaceExpansion;

    virtual bool init(IOFireWireBus *bus);
	virtual	void free() APPLE_KEXT_OVERRIDE;
  
public:

	/*!	@function	doRead
		@abstract	An abstract method for processing an address space read request
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
                          IOFWRequestRefCon refcon) = 0;
						  
	/*!	@function	doWrite
		@abstract	An abstract method for processing an address space write request
		@param		nodeID	FireWire Write to nodeID.
		@param		speed	at this 'speed'.
		@param		addr	with FireWire address 'addr'.
		@param		len		write 'len' bytes to nodeID.
		@param		buf		obtain bytes from location given by 'buf'.
		@param		refcon  Can be queried for extra info about the request.
		@result		UIn32	returns kFWResponseComplete on success */
    virtual UInt32 doWrite(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
                           const void *buf, IOFWRequestRefCon refcon) = 0;

	/*!	@function	doLock
		@abstract	A method for processing a lock request.
		@param		nodeID	FireWire Lock request for nodeID.
		@param		speed	at this 'speed'.
		@param		addr	with FireWire address 'addr'.
		@param		inlen	'inlen' bytes to use.
		@param		newVal	new value to write at 'addr' location .
		@param		outLen	'outLen' bytes for result.
		@param		oldVal	old value read from 'addr' location.
		@param		extType	Type like kFWExtendedTCodeCompareSwap.
		@param		refcon  Can be queried for extra info about the request.
		@result		UIn32	returns kFWResponseComplete on success */
    virtual UInt32 doLock(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 inlen,
                          const UInt32 *newVal, UInt32 &outLen, UInt32 *oldVal,
                          UInt32 extType, IOFWRequestRefCon refcon);

	/*!	@function	activate
		@abstract	Address space is ready for handling requests.
		@result		IOReturn
		*/
    virtual IOReturn activate();
	
	/*!	@function	deactivate
		@abstract	Address space request handler is disabled.
		@result		none
		*/
    virtual void deactivate();
    
	/*!	@function	contains
		@abstract	returns number of bytes starting at addr in this space
		@result		0 if it doesn't contain the address
		*/
    virtual UInt32 contains(FWAddress addr);

	/*!	@function	isTrustedNode
		@abstract	returns true if the node is added as a trusted node
		@param		nodeID is the nodeID to verify whether its trusted.
		@result		false if nodeID is not trusted
		*/
	inline bool isTrustedNode( UInt16 nodeID ) 
				{ return fIOFWAddressSpaceExpansion->fAuxiliary->isTrustedNode( nodeID ); }

	/*!	@function	addTrustedNode
		@abstract	Add a trusted node.
		@param		device object pointing to a FireWire node on the bus.
		@result		none
		*/
	inline void addTrustedNode( IOFireWireDevice * device )
				{ fIOFWAddressSpaceExpansion->fAuxiliary->addTrustedNode( device ); }
	
	/*!	@function	removeTrustedNode
		@abstract	Remove a trusted node.
		@param		device object pointing to a FireWire node on the bus.
		@result		none
		*/
	inline void removeTrustedNode( IOFireWireDevice * device )
				{ fIOFWAddressSpaceExpansion->fAuxiliary->removeTrustedNode( device ); }
		
	/*!	@function	removeAllTrustedNodes
		@abstract	Remove all trusted nodes.
		@result		none
		*/
	inline void removeAllTrustedNodes( void )
				{ fIOFWAddressSpaceExpansion->fAuxiliary->removeAllTrustedNodes(); }

	/*!	@function	isExclusive
		@abstract	Checks if an address space wants exclusive control of its address range
		@result		True if the address space is marked exclusive false otherwise
		*/

	inline bool isExclusive( void )
		{ return fIOFWAddressSpaceExpansion->fAuxiliary->isExclusive(); }

	/*!	@function	setExclusive
		@abstract	Sets if this address space requires exclusive control of its address range. Exclusivity should be set before an address space is activated.
		@param		exclusive  True if address space should be exclusive, false otherwise
		@result		none
	*/
		
	inline void setExclusive( bool exclusive )
		{ fIOFWAddressSpaceExpansion->fAuxiliary->setExclusive( exclusive ); }

	/*!	@function	intersects
		@abstract	Checks this address space intersects with the given address range. Currently only supports IOFWPsuedoAddressSpaces.
		@param		space  An address space to compare against
		@result		True if the address spaces intersect false otherwise
	*/
		
	inline bool intersects( IOFWAddressSpace * space )
		{ return fIOFWAddressSpaceExpansion->fAuxiliary->intersects( space ); }

		
protected:
	
	virtual IOFWAddressSpaceAux * createAuxiliary( void );
    
private:
    OSMetaClassDeclareReservedUsed(IOFWAddressSpace, 0);
    OSMetaClassDeclareReservedUsed(IOFWAddressSpace, 1);
    
};

// the physical and psuedo address space classes used to be defined here
// for backwards compatibility, we pull them in now.  the ifdefs surrounding
// the content of the header files ensures we do not multiply include a header.

#include <IOKit/firewire/IOFWPseudoAddressSpace.h>
#include <IOKit/firewire/IOFWPhysicalAddressSpace.h>

#endif /* _IOKIT_IOFWADDRESSSPACE */
