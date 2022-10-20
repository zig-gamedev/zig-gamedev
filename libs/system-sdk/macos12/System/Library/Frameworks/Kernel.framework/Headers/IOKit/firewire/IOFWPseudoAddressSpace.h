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
 
#ifndef _IOKIT_IOFWPSEUDOADDRESSSPACE_H
#define _IOKIT_IOFWPSEUDOADDRESSSPACE_H

#include <IOKit/firewire/IOFWAddressSpace.h>

/*
 * If installed, this callback is invoked for drivers which 
 * would like to coalesce incoming writes and do batch processing 
 * of incoming block write packets.
 * This callback can be installed by calling setARxReqIntCompleteHandler
 * method in IOFWPseudoAddressSpace object. 
 */
typedef void (*IOFWARxReqIntCompleteHandler)( void * refcon );

/*
 * Pseudo firewire addresses usually represent emulated registers of some kind.
 * Accesses to these addresses will result in the owner being notified.
 * 
 * Virtual addresses should not have zero as the top 16 bits of the 48 bit local address,
 * since that may look like a physical address to hardware (eg. OHCI).
 * if reader is NULL then reads will not be allowed.
 * if writer is NULL then writes will not be allowed.
 * if either is NULL then lock requests will not be allowed.
 * refcon is passed back as the first argument of read and write callbacks.
 */

class IOFWPseudoAddressSpace;

#pragma mark -

/*! 
	@class IOFWPseudoAddressSpaceAux
*/

class IOFWPseudoAddressSpaceAux : public IOFWAddressSpaceAux
{
    OSDeclareDefaultStructors(IOFWPseudoAddressSpaceAux)

	friend class IOFWAddressSpace;
	friend class IOFWPseudoAddressSpace;
	friend class IOFireWireController;
		
protected:
	
    struct MemberVariables 
	{ 		
		IOFWARxReqIntCompleteHandler		fARxReqIntCompleteHandler;
		void *	  							fARxReqIntCompleteHandlerRefcon;	
	};
	  
    MemberVariables * fMembers;

public:
     
	virtual bool init( IOFWAddressSpace * primary ) APPLE_KEXT_OVERRIDE;
	virtual	void free() APPLE_KEXT_OVERRIDE;

protected:

	bool createMemberVariables( void );
	void destroyMemberVariables( void );
	
	virtual void handleARxReqIntComplete();
		
public:

/*!	@function setARxReqIntCompleteHandler
	@abstract Installs a callback to receive notification, when FWIM has completed
			  ARxReqInt processing and no incoming packets are left in the queue.
	@param refcon	Client's callback object.
	@param handler	Client callback to be invoked, at the end of interrupt processing.
	@result none.	*/	
	virtual void setARxReqIntCompleteHandler( void * refcon, IOFWARxReqIntCompleteHandler handler );

	virtual bool intersects( IOFWAddressSpace * space ) APPLE_KEXT_OVERRIDE;
	
private:

    OSMetaClassDeclareReservedUsed(IOFWPseudoAddressSpaceAux, 0);
    OSMetaClassDeclareReservedUsed(IOFWPseudoAddressSpaceAux, 1);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 2);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 3);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 4);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 5);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 6);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 7);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 8);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpaceAux, 9);
	
};

/*! 
	@class IOFWPseudoAddressSpace
*/

class IOFWPseudoAddressSpace : public IOFWAddressSpace
{
    OSDeclareDefaultStructors(IOFWPseudoAddressSpace)

	friend class IOFWPseudoAddressSpaceAux;
	friend class IOFireWireController;
	
protected:

    IOMemoryDescriptor*	fDesc;
    void *				fRefCon;
    FWReadCallback		fReader;
    FWWriteCallback		fWriter;
    FWAddress			fBase;
    UInt32				fLen;

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

    static	OSData *	allocatedAddresses; // unused
    
    virtual	void	free() APPLE_KEXT_OVERRIDE;
	
public:

/*!	@function	simpleReader
	@abstract	A method for processing an address space read request
	@param		refcon		Client's callback object.
	@param		nodeID		FireWire Read from nodeID.
	@param		speed		at this 'speed'.
	@param		addr		with FireWire address 'addr'.
	@param		len			read 'len' bytes from nodeID.
	@param		buf			points to a memory descriptor containing the packet data.
	@param		offset		start from this 'offset' in 'buf'.
	@param		reqrefcon	Can be queried for extra info about the request.
	@result		UIn32		returns kFWResponseComplete on success */
    static 	UInt32 					simpleReader(
											void*					refcon,
											UInt16 					nodeID,
											IOFWSpeed &				speed,
											FWAddress 				addr,
											UInt32 					len,
											IOMemoryDescriptor**	buf,
											IOByteCount* 			offset,
                                            IOFWRequestRefCon		reqrefcon);

/*!	@function	simpleWriter
	@abstract	A method for processing an address space write request
	@param		refcon		Client's callback object.
	@param		nodeID		FireWire Write to nodeID.
	@param		speed		at this 'speed'.
	@param		addr		with FireWire address 'addr'.
	@param		len			write 'len' bytes to nodeID.
	@param		buf			obtain bytes from location given by 'buf'.
	@param		reqrefcon	Can be queried for extra info about the request.
	@result		UIn32		returns kFWResponseComplete on success */
    static 	UInt32 					simpleWriter(
											void*					refcon,
											UInt16 					nodeID,
											IOFWSpeed&				speed,
											FWAddress 				addr,
											UInt32 					len,
											const void*				buf,
                                            IOFWRequestRefCon		reqrefcon);
protected:

    // Get a unique address range
    IOReturn						allocateAddress(
											FWAddress*				addr,
											UInt32 					len);
    // free address
    void							freeAddress(
											FWAddress 				addr,
											UInt32 					len);

public:

    static IOFWPseudoAddressSpace*	readWrite(
											FWAddress 				addr,
											UInt32 					len, 
											FWReadCallback 			reader,
											FWWriteCallback 		writer,
											void*					refcon);

/*!	@function	simpleRead
	@abstract	Create an address space object to handle read-only memory (eg. the local ROM)
				handles everything itself
	@param		bus		Points to IOFireWireBus object.
	@param		addr	Points to starting address for the Pseudo Address Space.
	@param		len		Length of the Pseudo Address Space.
	@param		data	The virtual address of the first byte in the memory.
	@result		returns valid IOFWPseudoAddressSpace on success, null on failure */
    static IOFWPseudoAddressSpace*	simpleRead(
                                            IOFireWireBus*			bus,
			                                FWAddress*				addr,
											UInt32 					len,
											const void*				data);

/*!	@function	simpleReadFixed
	@abstract	Create an address space object to handle fixed read-only memory (eg. the local ROM)
				handles everything itself
	@param		bus		Points to IOFireWireBus object.
	@param		addr	Points to starting address for the Pseudo Address Space.
	@param		len		Length of the Pseudo Address Space.
	@param		data	The virtual address of the first byte in the memory.
	@result		returns valid IOFWPseudoAddressSpace on success, null on failure */
    static IOFWPseudoAddressSpace*	simpleReadFixed(
                                            IOFireWireBus*			bus,
			                                FWAddress 				addr,
											UInt32 					len,
											const void*				data);

/*!	@function	simpleRW
	@abstract	Create an address space object to handle r/w memory
				handles everything itself
	@param		bus		Points to IOFireWireBus object.
	@param		addr	Points to starting address for the Pseudo Address Space.
	@param		len		Length of the Pseudo Address Space.
	@param		data	The virtual address of the first byte in the memory.
	@result		returns valid IOFWPseudoAddressSpace on success, null on failure */
    static IOFWPseudoAddressSpace*	simpleRW(
                                            IOFireWireBus*			bus,
                                			FWAddress*				addr,
											UInt32 					len,
											void *					data);
											
/*!	@function	simpleRW
	@abstract	Create an address space object to handle r/w memory
				handles everything itself
	@param		bus		Points to IOFireWireBus object.
	@param		addr	Points to starting address for the Pseudo Address Space.
	@param		data	The virtual address of the first byte in the memory.
	@result		returns valid IOFWPseudoAddressSpace on success, null on failure */
    static IOFWPseudoAddressSpace*	simpleRW(
                                            IOFireWireBus*			bus,
                                			FWAddress*				addr,
											IOMemoryDescriptor *	data);
											
/*!	@function	initAll
	@abstract	Initialize an address space object to handle r/w memory
	@param		bus		Points to IOFireWireBus object.
	@param		addr	Points to starting address for the Pseudo Address Space.
	@param		len		Length of the Pseudo Address Space.
	@param		reader	Callback handler for incoming Read.
	@param		writer	Callback handler for incoming Write.
	@param		refcon	Client's callback object.
	@result		returns true on success, false on failure */
    virtual bool 					initAll(
                                            IOFireWireBus*			bus,
                							FWAddress*				addr,
											UInt32 					len, 
											FWReadCallback 			reader,
											FWWriteCallback 		writer,
											void*					refcon);
											
/*!	@function	initFixed
	@abstract	Initialize a fixed address space at top of kCSRRegisterSpaceBaseAddressHi
	@param		bus		Points to IOFireWireBus object.
	@param		addr	Points to starting address for the Pseudo Address Space.
	@param		reader	Callback handler for incoming Read.
	@param		writer	Callback handler for incoming Write.
	@param		refcon	Client's callback object.
	@result		returns true on success, false on failure */
    virtual bool 					initFixed(
                                            IOFireWireBus*			bus,
							                FWAddress 				addr,
											UInt32 					len,
                							FWReadCallback 			reader,
											FWWriteCallback 		writer,
											void*					refcon);

/*!	@function	doRead
	@abstract	A method for processing an address space read request
	@param		nodeID	FireWire Read from nodeID.
	@param		speed	at this 'speed'.
	@param		addr	with FireWire address 'addr'.
	@param		len		read 'len' bytes from nodeID.
	@param		buf		points to a memory descriptor containing the packet data.
	@param		offset	start from this 'offset' in 'buf'.
	@param		reqrefcon  Can be queried for extra info about the request.
	@result		UIn32	returns kFWResponseComplete on success */
    virtual UInt32 					doRead(
											UInt16 					nodeID, 
											IOFWSpeed &				speed, 
											FWAddress 				addr, 
											UInt32 					len,
                               				IOMemoryDescriptor **	buf, 
											IOByteCount * 			offset,
                                            IOFWRequestRefCon		reqrefcon) APPLE_KEXT_OVERRIDE;
/*!	@function	doWrite
	@abstract	A method for processing an address space write request
	@param		nodeID	FireWire Write to nodeID.
	@param		speed	at this 'speed'.
	@param		addr	with FireWire address 'addr'.
	@param		len		write 'len' bytes to nodeID.
	@param		buf		obtain bytes from location given by 'buf'.
	@param		reqrefcon  Can be queried for extra info about the request.
	@result		UIn32	returns kFWResponseComplete on success */
    virtual UInt32 					doWrite(
											UInt16 					nodeID,
											IOFWSpeed&				speed,
											FWAddress 				addr,
											UInt32 					len, 
											const void*				buf,
                                            IOFWRequestRefCon		reqrefcon) APPLE_KEXT_OVERRIDE;

/*!	@function	contains
	@abstract	returns number of bytes starting at addr in this space
	@result		0 if it doesn't contain the address
	*/
    virtual UInt32					contains(FWAddress addr) APPLE_KEXT_OVERRIDE;

/*!	@function	simpleRWFixed
	@abstract	Create a Read/Write fixed address space at top of kCSRRegisterSpaceBaseAddressHi.
	@param		control	Points to IOFireWireBus object.
	@param		addr	Points to starting address for the Pseudo Address Space.
	@param		len		Length of the address range.
	@param		data	The virtual address of the first byte in the memory.
	@result		returns valid IOFWPseudoAddressSpace on success, null on failure */
	static IOFWPseudoAddressSpace * simpleRWFixed( IOFireWireBus *control, FWAddress addr, UInt32 len, const void *data );

protected:
	
	virtual IOFWAddressSpaceAux * createAuxiliary( void ) APPLE_KEXT_OVERRIDE;

protected:
	inline void handleARxReqIntComplete( void )
		{ ((IOFWPseudoAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->handleARxReqIntComplete(); }
	
public:
/*!	@function setARxReqIntCompleteHandler
	@abstract Installs a callback to receive notification, when FWIM has completed
			  ARxReqInt processing and no incoming packets are left in the queue.
	@param refcon	Client's callback object.
	@param handler	Client callback to be invoked, at the end of interrupt processing.
	@result none.	*/	
	inline void setARxReqIntCompleteHandler( void * refcon, IOFWARxReqIntCompleteHandler handler )
		{ ((IOFWPseudoAddressSpaceAux*)fIOFWAddressSpaceExpansion->fAuxiliary)->setARxReqIntCompleteHandler( refcon, handler ); }
			
private:

    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpace, 0);
    OSMetaClassDeclareReservedUnused(IOFWPseudoAddressSpace, 1);
    
};

#endif
