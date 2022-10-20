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
  @header IOFireWireSBP2ManagementORB
  Contains the class definition for IOFireWireSBP2ManagementORB.
*/

#ifndef _IOKIT_IOFIREWIRESBP2MANAGEMENTORB_H
#define _IOKIT_IOFIREWIRESBP2MANAGEMENTORB_H

#include <IOKit/firewire/IOFWCommand.h>
#include <IOKit/firewire/IOFireWireUnit.h>

#include <IOKit/IOUserClient.h>

class IOFireWireSBP2LUN;
class IOFireWireSBP2ManagementORB;
class IOFireWireSBP2ORB;

enum
{
    kFWSBP2QueryLogins			= 1,
    kFWSBP2AbortTask			= 0xb,
    kFWSBP2AbortTaskSet			= 0xc,
    kFWSBP2LogicalUnitReset		= 0xe,
    kFWSBP2TargetReset			= 0xf
};

// Callback when management command completes asynchronously

/*! 
    @typedef FWSBP2ManagementCallback
    @param refCon Reference constant supplied when the notification was registered.
    @param status Indicates success or failure of operation. 
    @param orb Indicates current orb status is for.
*/

typedef void (*FWSBP2ManagementCallback)(void * refCon, IOReturn status, IOFireWireSBP2ManagementORB * orb);

/*!
    @class IOFireWireSBP2ManagementORB
    @abstract Supplies non login related management ORBs.  Management ORBs can be executed independent 
    of a login, if necessary.  Management ORBs are created using the IOFireWireSBP2LUN interface.
*/

class IOFireWireSBP2ManagementORB : public IOFWCommand
{
    OSDeclareDefaultStructors( IOFireWireSBP2ManagementORB )

	friend class IOFireWireSBP2LUN;
	friend class IOFireWireSBP2UserClient;
	
protected:

	////////////////////////////////////////            
    // rom keys
	
    enum
    {
        kUnitCharacteristicsKey 	= 0x3A,
        kManagementAgentOffsetKey	= 0x54
    };

	////////////////////////////////////////        
	// structs
	
    typedef struct
    {
        UInt32		orbOffsetHi;
        UInt32		orbOffsetLo;
        UInt32		reserved1[2];
        UInt16		options;
        UInt16		loginID;
        UInt32		reserved2;
        UInt32		statusFIFOAddressHi;
        UInt32		statusFIFOAddressLo;
    } FWSBP2TaskManagementORB;

    typedef struct
    {
        UInt32		reserved1[2];
        UInt32		queryResponseAddressHi;
        UInt32		queryResponseAddressLo;
        UInt16		options;
        UInt16		lun;
        UInt16		reserved2;
        UInt16		queryResponseLength;
        UInt32		statusFIFOAddressHi;
        UInt32		statusFIFOAddressLo;
    } FWSBP2QueryLoginsORB;
    
    typedef struct
    {
        UInt8		details;
        UInt8		sbpStatus;
        UInt16		orbOffsetHi;
        UInt32		orbOffsetLo;
        UInt32		status[6];
    } FWSBP2StatusBlock;

protected:

	////////////////////////////////////////        
	// friend methods
	
    // IOFireWireSBP2LUN methods
    virtual bool initWithLUN( 	IOFireWireSBP2LUN * lun, 
								void * refCon,
								FWSBP2ManagementCallback completion  );
	
	// IOFireWireSBP2UserClient methods
    virtual void setAsyncCallbackReference( void * asyncRef );
    virtual void getAsyncCallbackReference( void * asyncRef );

protected:
   
    // reserved for future use
    struct ExpansionData 
	{
		bool fInCriticalSection; 
	};
    ExpansionData * fExpansionData; 
         
    IOFireWireSBP2LUN * 		fLUN;
    IOFireWireUnit *			fUnit;
    
    UInt32						fManagementOffset;
    UInt32						fManagementTimeout;
    UInt32						fFunction;
    OSObject * 					fManageeCommand;
    
	// our orb
    FWSBP2TaskManagementORB		fManagementORB;
    IOFWAddressSpace *			fManagementORBAddressSpace;
    FWAddress					fManagementORBAddress;

	// command to write management agent
    IOFWWriteCommand *			fWriteCommand;
    IOMemoryDescriptor *		fWriteCommandMemory;

	// timeout
    IOFWCommand *				fTimeoutCommand;
    bool						fTimeoutTimerSet;

	// status block
    FWSBP2StatusBlock			fStatusBlock;
    IOFWAddressSpace *			fStatusBlockAddressSpace;
    FWAddress					fStatusBlockAddress;

	// response buffer
    void *						fResponseBuf;
    UInt32						fResponseLen;
    IOFWAddressSpace *			fResponseAddressSpace;
    FWAddress					fResponseAddress;

	// competion routine
	FWSBP2ManagementCallback	fCompletionCallback;
    void * 						fCompletionRefCon;

    // for user client
    OSAsyncReference64			fCallbackAsyncRef;
	
	IOMemoryMap *				fResponseMap;
	
	bool						fCompleting;
	
	////////////////////////////////////////        

    // init / destroy
    virtual IOReturn getUnitInformation( void );
    virtual IOReturn allocateResources( void );
    virtual void free( void ) APPLE_KEXT_OVERRIDE;
        
    // command execution
    virtual IOReturn execute( void ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn complete( IOReturn state ) APPLE_KEXT_OVERRIDE;

    // write complete handler
    static void writeCompleteStatic( 	void *refcon, 
										IOReturn status, 
										IOFireWireNub *device, 
										IOFWCommand *fwCmd );
    virtual void writeComplete( IOReturn status, 
								IOFireWireNub *device, 
								IOFWCommand *fwCmd );

    // timeout handler
    static void handleTimeoutStatic( 	void *refcon, 
										IOReturn status, 
										IOFireWireBus *bus, 
										IOFWBusCommand *fwCmd );
    virtual void handleTimeout( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );

    // status block write handler
    static UInt32 statusBlockWriteStatic( 	void *refcon, 
											UInt16 nodeID, 
											IOFWSpeed &speed, 
											FWAddress addr, 
											UInt32 len,
											const void *buf, 
											IOFWRequestRefCon lockRead );
    virtual UInt32 statusBlockWrite( 	UInt16 nodeID, 
										FWAddress addr, 
										UInt32 len, 
										const void *buf, 
										IOFWRequestRefCon lockRead );

	// IOFireWireSBP2LUN friend class wrappers
	virtual void clearAllTasksInSet( void );
	virtual void removeManagementORB( IOFireWireSBP2ManagementORB * orb );
	
public:
    
	////////////////////////////////////////        
    // client methods

    /*!
		@function setCommandFunction
		@abstract Sets the function of the management ORB.
		@discussion Sets the the function of the management ORB.  Legal values are kFWSBP2QueryLogins,
        kFWSBP2AbortTask, kFWSBP2AbortTaskSet,  kFWSBP2LogicalUnitReset, and kFWSBP2TargetReset.
        @param function a value indicating the desired management function.
        @result Returns kIOReturnSuccess if function was a legal function.
	*/
    	
    virtual IOReturn setCommandFunction( UInt32 function );
    
    /*!
		@function getCommandFunction
		@abstract Returns the current function of the management ORB.
		@discussion Returns the function of the management ORB.  This is the same value that was 
        set with setCommandFunction.
        @result Returns the function of the management ORB.
	*/
    
    virtual UInt32 getCommandFunction( void );

    /*!
		@function setManageeCommand
		@abstract Sets the command to be managed by the management ORB.
		@discussion All management functions except kFWSBP2QueryLogins require a reference to an ORB of 
        some sort.  kFWSBP2AbortTaskSet,  kFWSBP2LogicalUnitReset, and kFWSBP2TargetReset require a 
        reference to the login ORB.  kFWSBP2AbortTask requires a reference to the ORB to be aborted.  
        This method allows you to set the ORB to be managed.
        @param command a reference to an IOFireWireSBP2Login or an IOFireWireSBP2ORB.
 	*/
    
    virtual void setManageeCommand( OSObject * command );
    
    /*!
		@function getCommandFunction
		@abstract Returns the current managee command of the management ORB.
		@discussion Returns the current managee command of the management ORB.  This is the same value that was 
        set with setManageeCommand.
        @result  Returns the current managee command of the management ORB.
	*/
    
    virtual OSObject* getManageeCommand( void );

    /*!
		@function setResponseBuffer
		@abstract Sets the response buffer for the management ORB.
		@discussion Sets the response buffer for the management ORB.  kFWSBP2QueryLogins returns 
        a response to its query and needs to write it somewhere.  This routine allows you to 
        specify the location.
        @param desc memory descriptor for buffer.
        @result Returns kIOReturnSuccess on a success.
	*/
    
	virtual IOReturn setResponseBuffer( IOMemoryDescriptor * desc );
    
    /*!
		@function setResponseBuffer
		@abstract Sets the response buffer for the management ORB.
		@discussion Sets the response buffer for the management ORB.  kFWSBP2QueryLogins returns 
        a response to its query and needs to write it somewhere.  This routine allows you to 
        specify the location.        
        @param buf backing store for buffer
        @param len length of buffer.
        @result Returns kIOReturnSuccess on a success.
	*/
    
    virtual IOReturn setResponseBuffer( void * buf, UInt32 len );
    
    /*!
		@function getResponseBuffer
		@abstract Returns the response buffer for the management ORB.
		@discussion Returns the response buffer set in setResponseBuffer above        
        @param desc memory descriptor for buffer.
        @param buf output parameter for backing store for buffer
        @param len output parameter for length of buffer.
	*/
    
    virtual void getResponseBuffer( void ** buf, UInt32 * len );

    /*! 
        @function release
        @abstract Primary implementation of the release mechanism.
        @discussion See OSObject.h for more information. 
        @param when When retainCount == when then call free(). 
    */
    
	virtual void release() const APPLE_KEXT_OVERRIDE;

protected:

	virtual void setORBToDummy( IOFireWireSBP2ORB * orb );

	virtual void suspendedNotify();

private:

    OSMetaClassDeclareReservedUsed(IOFireWireSBP2ManagementORB, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 3);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 4);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 5);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 6);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 7);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ManagementORB, 8);
    
};

#endif
