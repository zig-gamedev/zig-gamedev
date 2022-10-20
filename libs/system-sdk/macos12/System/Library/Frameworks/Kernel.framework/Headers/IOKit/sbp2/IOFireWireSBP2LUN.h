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
  @header IOFireWireSBP2LUN
  Contains the class definition for IOFireWireSBP2LUN.
*/

#ifndef _IOKIT_IOFIREWIRESBP2LUN_H
#define _IOKIT_IOFIREWIRESBP2LUN_H

#include <IOKit/IOService.h>
#include <IOKit/IOCommandGate.h>

#include <IOKit/firewire/IOFireWireUnit.h>

#include <IOKit/sbp2/IOFireWireSBP2Login.h>
#include <IOKit/sbp2/IOFireWireSBP2ManagementORB.h>

class IOFireWireSBP2Target;

/*!
    @class IOFireWireSBP2LUN
    @abstract Provider for most drivers. 
    @discussion IOFireWireSBP2LUN objects are created by IOFireWireSBP2Target objects.  Each target may have zero or more IOFireWireSBP2LUN children. The LUN object serves as the matching nub for most drivers and therefore will be the provider for most drivers.  It supplies the methods that control the operation of the LUN as a whole.  Methods that control the behavior and execution of an SBP2 login session are supplied in a separate IOFireWireSBP2Login object. The LUN can be used to create one of these login objects.
    The LUN can also create IOFireWireSBP2ManagementORBs for configuring and appending non-login related management functions.  Login related management functions (ie. Login, Logout, Reconnect) are supplied by the IOFireWireSBP2Login.
    Finally the LUN can supply a reference to the IOFireWireUnit.  This can be useful if a driver wishes to access the standard FireWire APIs.  
*/

class IOFireWireSBP2LUN : public IOService
{
    OSDeclareDefaultStructors( IOFireWireSBP2LUN );

	friend class IOFireWireSBP2ManagementORB;
	friend class IOFireWireSBP2Login;
	friend class IOFireWireSBP2UserClient;
	
protected: 

    // reserved for future use
    struct ExpansionData { };
    ExpansionData *reserved;

	////////////////////////////////////////////
	// methods called by friend classes

	// IOFireWireSBP2Login methods
    virtual void removeLogin( IOFireWireSBP2Login * login );
	virtual IOFireWireSBP2Target * getTarget( void );
	
	// IOFireWireSBP2ManagementORB methods
	virtual void clearAllTasksInSet( void );  // assumes caller is on workloop
	virtual void removeManagementORB( IOFireWireSBP2ManagementORB * orb ); 
	
	// IOFireWireSBP2UserClient methods
	virtual void flushAllManagementORBs( void );

public:

	////////////////////////////////////////////
	// IOService overrides

    /*! @function attach
        @abstract Attaches an IOService client to a provider in the registry.
        @discussion See IOService for discussion.
        @param provider The IOService object which will serve as this objects provider.
        @result false if the provider is inactive or on a resource failure, otherwise true. 
    */
    
    virtual bool attach( IOService *provider ) APPLE_KEXT_OVERRIDE;

	/*! @function handleOpen
		@abstract Overrideable method to control the open / close behaviour of an IOService.
		@discussion See IOService for discussion.		
		@param forClient Designates the client of the provider requesting the open.
		@param options Options for the open, may be interpreted by the implementor of handleOpen.
		@result Return true if the open was successful, false otherwise. */

    virtual bool handleOpen(  IOService *	  forClient,
                              IOOptionBits	  options,
                              void *		  arg ) APPLE_KEXT_OVERRIDE;
	/*! 
		@function handleClose
		@abstract Overrideable method to control the open / close behaviour of an IOService.
		@discussion See IOService for discussion.
		@param forClient Designates the client of the provider requesting the close.
		@param options Options for the close, may be interpreted by the implementor of handleOpen. 
	*/

    virtual void handleClose(   IOService *		forClient,
                                IOOptionBits	options ) APPLE_KEXT_OVERRIDE;
    
 	/*! 
		@function matchPropertyTable
		@abstract Implements SBP2 specific matching.
		@discussion See IOService for discussion.
	    @param table The dictionary of properties to be matched against.
		@result Returns false if the family considers the matching dictionary does not match in properties it 			understands, true otherwise.
	*/
    	
	virtual bool matchPropertyTable(OSDictionary * table) APPLE_KEXT_OVERRIDE;
																	
protected:

	////////////////////////////////////////////
	// private fields
	
    IOFireWireSBP2Target * 	fProviderTarget;
    UInt32					fLUNumber;
    OSSet *					fLoginSet;
    OSIterator *			fLoginSetIterator;
	IOCommandGate * 		fGate;
	OSSet *					fORBSet;
    OSIterator *			fORBSetIterator;
	OSObject * 				fDiagnostics;
	
	////////////////////////////////////////////
	// private methods
	
	// IOService methods
    virtual void free( void ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn message( UInt32 type, IOService * provider, void * argument = 0 ) APPLE_KEXT_OVERRIDE;
    
	// create management orb internals
	static IOReturn staticCreateManagementORBAction( OSObject *self,
													 void * refCon,
													 void * completion, 
													 void * orb, 
													 void * );
	virtual IOReturn createManagementORBAction( void * refCon,
												FWSBP2ManagementCallback completion, 
												IOFireWireSBP2ManagementORB ** orb );

	// remove management orb internals
	static IOReturn staticRemoveManagementORBAction( OSObject * self, void * orb, 
													void *, void *, void * );
	virtual IOReturn removeManagementORBAction( IOFireWireSBP2ManagementORB * orb );

	// flush all management orb internals
	static IOReturn staticExecuteFlushAllMgmtORBs( OSObject * self, void *, 
													void *, void *, void * );
	virtual IOReturn executeFlushAllMgmtORBs( void );

	// IOFireWireSBP2ManagementORB friend class wrappers
    virtual bool initMgmtORBWithLUN( IOFireWireSBP2ManagementORB * orb, IOFireWireSBP2LUN * lun, 
									 void * refCon, 
									 FWSBP2ManagementCallback completion );
									 		
	// IOFireWireSBP2Login friend class wrappers
    virtual bool initLoginWithLUN( IOFireWireSBP2Login * login, IOFireWireSBP2LUN * lun );
	virtual void suspendedNotify( void );
	virtual void resumeNotify( void );
		
private:

    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 0);

public:

	////////////////////////////////////////////
    // client methods

	// getters
	
    /*!
		@function getFireWireUnit
		@abstract Returns an IOFireWireUnit object.
		@discussion An IOFireWireUnit is the provider of an IOFireWireSBP2Target.  In order to 
        use the base FireWire services you will need a reference to the unit.  This method 
        returns that reference.
		@result Returns a pointer to an IOFireWireUnit.
	*/
    
    virtual IOFireWireUnit * getFireWireUnit( void );
    
    /*!
		@function getLUNumber
		@abstract Returns the LUNs number.
		@discussion Each LUN has a number to uniquely identify it on a device.  This method returns 
        this value in a UInt32.		
        @result Returns a UInt32 containing the Logical Unit Number.
	*/
    
	virtual UInt32 getLUNumber( void );
    
	// factory methods
    
    /*!
		@function createLogin
		@abstract Creates a new IOFireWireSBP2Login object.
		@discussion	Creates a new IOFireWireSBP2Login object for the LUN.  Login objects supply most 
        of the SBP2 APIs related to login maintenance and Normal Command ORB execution.
        @result Returns a pointer to a new IOFireWireSBP2Login.
	*/

    virtual IOFireWireSBP2Login *createLogin( void );	

    /*!
		@function createManagementORB
		@abstract Creates a new IOFireWireSBP2ManagementORB object.
		@discussion	Creates a new IOFireWireSBP2ManagementORB object.  Management objects let you 
        execute commands like QueryLogins, LogicalUnitReset, and AbortTask.  These commands are 
        configured after they are created here.  When they are done executing (after a call to submit) 
        the supplied completion routine will be called with the supplied refcon.  Usually this refCon
        is the "this" pointer of completion method's object.
        @param refCon The refcon passed to the completion routine.
        @param completion The completion routine.  Called when the ORB finishes execution.	
        @result Returns a pointer to a new IOFireWireSBP2Login.
	*/
        
    virtual IOFireWireSBP2ManagementORB * createManagementORB( void * refCon, FWSBP2ManagementCallback completion );

protected:

    static IOReturn staticCreateLogin( OSObject *self, void * login, void *, void *, void * );
    virtual IOReturn createLoginAction( IOFireWireSBP2Login ** login );

    static IOReturn staticRemoveLoginAction( OSObject *self, void * login, void *, void *, void * );
    virtual IOReturn removeLoginAction( IOFireWireSBP2Login * login );

public:

    /*!
		@function getDiagnostics
		@abstract Debug-only method.
		@discussion	Returns a reference to the internal diagnostics object when the services are built 
        in debug mode. Should be a no-op in release builds.
        @result Returns a pointer to the diagnostics object (if any).
	*/
    	
	virtual OSObject * getDiagnostics( void );

    virtual bool finalize( IOOptionBits options ) APPLE_KEXT_OVERRIDE;

protected:

	void terminateNotify( void );
	
private:
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 3);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 4);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 5);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 6);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 7);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LUN, 8);

};

#endif 
