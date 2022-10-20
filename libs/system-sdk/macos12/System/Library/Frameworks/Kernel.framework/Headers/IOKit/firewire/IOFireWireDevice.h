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
 /*! @header
	This header contains the definition of the <code>IOFireWireDevice</code> and <code>IOFireWireDeviceAux</code> classes. 
	An <code>IOFireWireDevice</code> object represents a FireWire device in the I/O Registry.
	It is strongly recommended that you write applications that access FireWire devices rather than in-kernel device drivers. 
	For more information on how to do this, see {@linkdoc //apple_ref/doc/uid/TP40000969 FireWire Device Interface Guide}.
	@indexgroup FireWire
 */
#ifndef _IOKIT_IOFIREWIREDEVICE_H
#define _IOKIT_IOFIREWIREDEVICE_H

#include <IOKit/firewire/IOFireWireNub.h>

class IOFireWireROMCache;

struct IOFWNodeScan;
struct RomScan;

class IOFireWireDevice;

#pragma mark -

/*! @class IOFireWireDeviceAux
	@discussion An IOFireWireDeviceAux is for internal use only. You should never subclass IOFireWireDeviceAux 
*/

class IOFireWireDeviceAux : public IOFireWireNubAux
{
    OSDeclareDefaultStructors(IOFireWireDeviceAux)

	friend class IOFireWireDevice;
	
protected:
	
	UInt32			fUnitCount;
	IOFWSpeed		fMaxSpeed;
	OSSet *			fOpenUnitSet;
	AbsoluteTime	fResumeTime;
	
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

    virtual bool init( IOFireWireDevice * primary );
	
	virtual	void free(void) APPLE_KEXT_OVERRIDE;

	virtual bool isTerminated( void );
	
	virtual void setTerminationState( TerminationState state ) APPLE_KEXT_OVERRIDE;
	
	void setMaxSpeed( IOFWSpeed speed );
	
	void setUnitCount( UInt32 count );
	
	UInt32 getUnitCount( void );

	bool isPhysicalAccessEnabled( void ) APPLE_KEXT_OVERRIDE;

	virtual IOFWSimpleContiguousPhysicalAddressSpace * createSimpleContiguousPhysicalAddressSpace( vm_size_t size, IODirection direction ) APPLE_KEXT_OVERRIDE;
		
    virtual IOFWSimplePhysicalAddressSpace * createSimplePhysicalAddressSpace( vm_size_t size, IODirection direction ) APPLE_KEXT_OVERRIDE;
	
	OSSet * getOpenUnitSet() const;
	
	void  latchResumeTime( void );
	
	AbsoluteTime getResumeTime( void );
	
private:
    OSMetaClassDeclareReservedUnused(IOFireWireDeviceAux, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireDeviceAux, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireDeviceAux, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireDeviceAux, 3);	
};

#pragma mark -
/*! @class IOFireWireDevice
	@abstract Represents a FireWire device.
	@discussion The FireWire family tries to read the configuration ROM of each device on the FireWire bus. For each device that responds with 
				its bus information block, the FireWire family publishes an <code>IOFireWireDevice</code> object in the I/O Registry. An 
				<code>IOFireWireDevice</code> object keeps track of the device's node ID, copies config ROM properties into the object's property 
				list, and scans the config ROM for unit directories, publishing an <code>IOFireWireUnit</code> object for each unit directory it finds.
*/

class IOFireWireDevice : public IOFireWireNub
{
    OSDeclareDefaultStructors(IOFireWireDevice)

    friend class IOFireWireController;
	friend class IOFireWireDeviceAux;

protected:

	enum RegistrationState
	{
		kDeviceRegistered,
		kDeviceNeedsRegisterService,
		kDeviceNotRegistered
	};
	
    IOFireWireROMCache	*fDeviceROM;
    bool				fOpenFromDevice;
    UInt32				fOpenFromUnitCount;
    UInt32				fROMGeneration;
    IORecursiveLock		*fROMLock;
    RegistrationState	fRegistrationState;
	UInt32				fROMReadRetry;
	
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

    static	void readROMDirGlue(void *refcon, IOReturn status,
                               IOFireWireNub *device, IOFWCommand *fwCmd);
    static	void readROMThreadFunc(void *arg);

    static	void terminateDevice(void *arg);
    
    void	processROM(RomScan *romScan);
    
    virtual void free() APPLE_KEXT_OVERRIDE;
    
public:
    virtual IOReturn message( UInt32 type, IOService * provider, void * argument ) APPLE_KEXT_OVERRIDE;

    virtual bool handleOpen( IOService * forClient, IOOptionBits options, void * arg ) APPLE_KEXT_OVERRIDE;

    virtual void handleClose( IOService * forClient, IOOptionBits options ) APPLE_KEXT_OVERRIDE;

    virtual bool handleIsOpen( const IOService * forClient ) const APPLE_KEXT_OVERRIDE;

protected:    
    virtual IOReturn cacheROM(OSData *rom, UInt32 offset, const UInt32 *&romBase);

    virtual const UInt32 * getROMBase();
    
    virtual void setNodeROM(UInt32 generation, UInt16 localNodeID, const IOFWNodeScan *info);

public:
    virtual bool matchPropertyTable(OSDictionary * table) APPLE_KEXT_OVERRIDE;

	/*!	@function	init
		@abstract	Initializes the nub.
		@param		propTable	Property table passed to the standard nub initialization.
		@param		scan		Pointer to the node scan structure.
		@result		Returns <code>true</code> if initialization was successful; <code>false</code> otherwise.
	*/
    virtual bool init(OSDictionary * propTable, const IOFWNodeScan *scan);

    virtual bool attach(IOService * provider ) APPLE_KEXT_OVERRIDE;

    virtual bool finalize( IOOptionBits options ) APPLE_KEXT_OVERRIDE;

	/*!	@function	setNodeFlags
		@abstract	Sets the node's characteristics.
		@param		flags Refer to "node flags" in IOFireWireFamilyCommon.h.
	*/
    virtual void setNodeFlags( UInt32 flags ) APPLE_KEXT_OVERRIDE;

	/*!	@function	clearNodeFlags
		@abstract	Resets the node's characteristics.
		@param		flags Refer to "node flags" in IOFireWireFamilyCommon.h.
	*/
	virtual void clearNodeFlags( UInt32 flags ) APPLE_KEXT_OVERRIDE;

	/*!	@function	getNodeFlags
		@abstract	Retrieves the node's characteristics.
		@param		flags	Refer to "node flags" in IOFireWireFamilyCommon.h.
		@result		UInt32	The flags set for a particular node. 
	*/
    virtual UInt32 getNodeFlags( void ) APPLE_KEXT_OVERRIDE;

protected:
	virtual IOReturn configureNode( void );

public:
	/*!	@function	createPhysicalAddressSpace
		@abstract	Creates local physical FireWire address spaces for the device to access.
		@param		mem  Memory area allocated to back the physical access by Link hardware.
		@result		A valid <code>IOFWPhysicalAddressSpace</code> object on success; NULL on failure.
	*/
    virtual IOFWPhysicalAddressSpace *createPhysicalAddressSpace(IOMemoryDescriptor *mem) APPLE_KEXT_OVERRIDE;

	/*!	@function	createPseudoAddressSpace
		@abstract	Creates local pseudo FireWire address spaces for the device to access.
		@param		addr	The FireWire address that is mapped to the pseudo address access.
		@param		len		Size of the address space to allocate.
		@param		reader  Read callback, when the device reads from this address space.
		@param		writer  Write callback, when the device writes to this address space.
		@param		refcon  Client's callback object returned during reader/writer callbacks.
		@result		A valid <code>IOFWPseudoAddressSpace</code> object on success; NULL on failure.
	*/
    virtual IOFWPseudoAddressSpace *createPseudoAddressSpace(FWAddress *addr, UInt32 len,
                    FWReadCallback reader, FWWriteCallback writer, void *refcon) APPLE_KEXT_OVERRIDE;

protected:
	virtual IOReturn readRootDirectory( IOConfigDirectory * directory, OSDictionary * propTable );

	virtual IOReturn processRootDirectory( OSDictionary * propTable );

	virtual IOReturn readUnitDirectories( IOConfigDirectory * directory, OSSet * unitInfo );

	virtual IOReturn processUnitDirectories( OSSet * unitSet );
    
	virtual void setRegistrationState( RegistrationState fRegistrationState );
	
	virtual void preprocessDirectories( OSDictionary * rootPropTable, OSSet * unitSet );
	
	virtual void configurePhysicalFilter( void );

protected:
	virtual IOFireWireNubAux * createAuxiliary( void ) APPLE_KEXT_OVERRIDE;

public:
	inline bool isTerminated( void )
		{ return ((IOFireWireDeviceAux*)fAuxiliary)->isTerminated(); }
		
	/*!	@function	setMaxSpeed
		@abstract	Sets the maximum speed for this node.
		@param		speed Maximum speed. Refer to "bus speed numbers" in IOFireWireFamilyCommon.h.
	*/
	inline void setMaxSpeed( IOFWSpeed speed )
		{ ((IOFireWireDeviceAux*)fAuxiliary)->setMaxSpeed( speed ); }		

protected:
	inline void setUnitCount( UInt32 count )
		{ ((IOFireWireDeviceAux*)fAuxiliary)->setUnitCount( count ); }		

	inline OSSet * getOpenUnitSet( void ) const
		{ return ((IOFireWireDeviceAux*)fAuxiliary)->getOpenUnitSet(); }
			
public:
	/*!	@function	getUnitCount
		@abstract	Returns number of units attached to this device.
		@result		UInt32 The number of units attached to this device.
	*/
	inline UInt32 getUnitCount( void )
		{ return ((IOFireWireDeviceAux*)fAuxiliary)->getUnitCount(); }		

protected:
	inline AbsoluteTime getResumeTime( void )
		{ return ((IOFireWireDeviceAux*)fAuxiliary)->getResumeTime(); }		

	inline void latchResumeTime( void )
		{ ((IOFireWireDeviceAux*)fAuxiliary)->latchResumeTime(); }		
		
private:
    OSMetaClassDeclareReservedUnused(IOFireWireDevice, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireDevice, 1);

};

#endif /* ! _IOKIT_IOFIREWIREDEVICE_H */
