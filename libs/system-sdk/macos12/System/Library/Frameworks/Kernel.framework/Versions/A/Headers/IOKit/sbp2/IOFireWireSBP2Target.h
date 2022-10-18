/*
 * Copyright (c) 1998-2000 Apple Computer, Inc. All rights reserved.
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
  @header IOFireWireSBP2Target
  Contains the class definition for IOFireWireSBP2Target.
*/

#ifndef _IOKIT_IOFIREWIRESBP2TARGET_H
#define _IOKIT_IOFIREWIRESBP2TARGET_H

#include <IOKit/firewire/IOFireWireUnit.h>
#include <IOKit/IOService.h>

enum
{
    kIOFWSBP2FailsOnAckBusy = (1 << 0),
	kIOFWSBP2FailsOnBusResetsDuringIO = (1 << 1),
	kIOFWSBP2DontUsePTPacketLimit = (1 << 2)
};

/*!
    @class IOFireWireSBP2Target
    @abstract Serves as bridge between IOFireWireUnit and IOFireWireLUN.
    @discussion Matches against IOFireWireUnits supporting the SBP2 protocol.  Creates IOFireWireSBP2LUN nubs 	for matching. Most drivers will match against an IOFireWireSBP2LUN, but matching against an 	IOFireWireSBP2Target is also supported. This can be useful in cases where a single driver wishes to 
    control all LUNs on a device. Support for this technique is minimal, however, and the driver will be
    required to discover it's LUNs through the registry.
*/

class IOFireWireSBP2Target : public IOService
{
    OSDeclareDefaultStructors(IOFireWireSBP2Target);

protected:
	
	/////////////////////////////////////////
    // rom keys
	
    enum
    {
        kCmdSpecIDKey 				= 0x38,
        kCmdSetKey					= 0x39,
        kSoftwareRevKey				= 0x3b,
        kFirmwareRevKey	 			= 0x3c,
        kLUNKey						= 0x14,
        kLUNDirectoryKey			= 0xd4,
        kManagementAgentOffsetKey	= 0x54,
		kUnitCharacteristicsKey 	= 0x3A,
		kRevisionKey				= 0x21,
		kFastStartKey				= 0x3E
    };

    typedef struct
    {
		UInt32 cmdSpecID; 
		UInt32 cmdSet;
		UInt32 vendorID;
		UInt32 softwareRev;
		UInt32 firmwareRev; 
		UInt32 lun;
		UInt32 devType;
		UInt32 unitCharacteristics;
		UInt32 managementOffset;
		UInt32 revision;
		bool fastStartSupported;
		UInt32 fastStart;
    } LUNInfo;
	
    // reserved for future use
    struct ExpansionData 
	{
		bool				fStarted;
		OSArray *			fPendingMgtAgentCommands ;
		UInt32				fNumberPendingMgtAgentOrbs;
		UInt32				fNumLUNs;
	};
    ExpansionData * fExpansionData; 
    
	/////////////////////////////////////////
	// private fields
	
    bool 				fOpenFromTarget;
    UInt32				fOpenFromLUNCount;    
    IOFireWireUnit * 	fProviderUnit;
    UInt32				fFlags;
    
	IOFireWireController * fControl;
	
	UInt32 				fIOCriticalSectionCount;
	
	/////////////////////////////////////////
	// private internals

    virtual void free( void ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn message( 	UInt32 type, 
								IOService * provider,
								void * argument = 0) APPLE_KEXT_OVERRIDE;
    
    virtual void scanForLUNs( void );
    IOReturn createLUN( LUNInfo * info );

public:

	/////////////////////////////////////////
	// public methods
	
	/*! @function handleOpen
		@abstract Overrideable method to control the open / close behaviour of an IOService.
		@discussion See IOService for discussion.		
		@param forClient Designates the client of the provider requesting the open.
		@param options Options for the open, may be interpreted by the implementor of handleOpen.
		@result Return true if the open was successful, false otherwise. */

    virtual bool handleOpen( IOService * forClient, IOOptionBits options, void * arg ) APPLE_KEXT_OVERRIDE;
	
	/*! 
		@function handleClose
		@abstract Overrideable method to control the open / close behaviour of an IOService.
		@discussion See IOService for discussion.
		@param forClient Designates the client of the provider requesting the close.
		@param options Options for the close, may be interpreted by the implementor of handleOpen. 
	*/

	virtual void handleClose( IOService * forClient, IOOptionBits options ) APPLE_KEXT_OVERRIDE;

	/*! 
		@function handleIsOpen
		@abstract Overrideable method to control the open / close behaviour of an IOService.
		@discussion See IOService for discussion.		
		@param forClient If non-zero, isOpen returns the open state for that client. If zero is passed, isOpen returns the open state for all clients.
		@result Returns true if the specific, or any, client has the IOService open. 
	*/
	
    virtual bool handleIsOpen(  const IOService * forClient ) const APPLE_KEXT_OVERRIDE;
    
	/*! 
		@function start
		@abstract During an IOService instantiation, the start method is called when the IOService has been selected to run on the provider.
		@discussion See IOService for discussion.
		@result Return true if the start was successful, false otherwise (which will cause the instance to be detached and usually freed).
	*/
	
    virtual bool start( IOService *provider ) APPLE_KEXT_OVERRIDE;
 
	/*! 
		@function stop
		@abstract During an IOService termination, the stop method is called in its clients before they are detached & it is destroyed.
		@discussion See IOService for discussion.
	*/
   
	virtual void stop( IOService *provider ) APPLE_KEXT_OVERRIDE;

	/*!
		@function getFireWireUnit
		@abstract Returns an IOFireWireUnit object.
		@discussion An IOFireWireUnit is the provider of an IOFireWireSBP2Target.  In order to use the base FireWire services
		you will need a reference to the unit.  This method returns that reference.
		@result Returns a pointer to an IOFireWireUnit.
	*/
	
    virtual IOFireWireUnit * getFireWireUnit( void );
	
	/*! 
		@function matchPropertyTable
		@abstract Implements SBP2 specific matching.
		@discussion See IOService for discussion.
	    @param table The dictionary of properties to be matched against.
		@result Returns false if the family considers the matching dictionary does not match in properties it understands, true otherwise.
	*/
	
	virtual bool matchPropertyTable( OSDictionary * table ) APPLE_KEXT_OVERRIDE;

    virtual void setTargetFlags( UInt32 flags );
    virtual UInt32 getTargetFlags( void );

protected:
    virtual void configurePhysicalFilter( void );

public:
    virtual void clearTargetFlags( UInt32 flags );
	virtual IOReturn beginIOCriticalSection( void );
	virtual void endIOCriticalSection( void );

    virtual bool finalize( IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    
    IOReturn 	synchMgmtAgentAccess( IOFWCommand * mgmtOrbCommand );
	void		completeMgmtAgentAccess( );
	void 		clearMgmtAgentAccess( );
	void		cancelMgmtAgentAccess( IOFWCommand * mgmtOrbCommand );

protected:

    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 3);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 4);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 5);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 6);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 7);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Target, 8);	
    
};

#endif
