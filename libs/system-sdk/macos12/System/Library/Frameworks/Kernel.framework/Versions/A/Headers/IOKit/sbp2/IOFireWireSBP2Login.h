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
  @header IOFireWireSBP2Login
  Contains the class definition for IOFireWireSBP2Login.
*/

#ifndef _IOKIT_IOFIREWIRESBP2LOGIN_H
#define _IOKIT_IOFIREWIRESBP2LOGIN_H

#include <IOKit/IOCommandGate.h>

#include <IOKit/firewire/IOFireWireUnit.h>

#include <IOKit/sbp2/IOFireWireSBP2ORB.h>
#include <IOKit/sbp2/IOFireWireSBP2Target.h>

// login option flags
enum
{
	kFWSBP2DontSynchronizeMgmtAgent = (1 << 0),
    kFWSBP2ExclusiveLogin 			= (1 << 5)
};

// notification events
enum
{
    kFWSBP2NormalCommandStatus	= 6,
    kFWSBP2NormalCommandTimeout	= 7,
    kFWSBP2UnsolicitedStatus	= 8,
    kFWSBP2NormalCommandReset	= 9
};

#define kIOMessageFWSBP2ReconnectComplete	iokit_fw_err(0x3E8)
#define kIOMessageFWSBP2ReconnectFailed		iokit_fw_err(0x3E9)

/*! 
    @typedef FWSBP2LoginResponse
    @param length Length of login response.
    @param loginID Unique id representing this login. 
    @param commandBlockAgentAddressHi High 32 bits of command block agent address.
    @param commandBlockAgentAddressLo Low 32 bits of command block agent address.
    @param reserved Reserved.
    @param reconnectHold Reconnect timeout encoded as 2^reconnectHold seconds.
*/

typedef struct
{
    UInt16		length;
    UInt16		loginID;
    UInt32		commandBlockAgentAddressHi;
    UInt32		commandBlockAgentAddressLo;
    UInt16		reserved;
    UInt16		reconnectHold;
} FWSBP2LoginResponse, *FWSBP2LoginResponsePtr;

/*! 
    @typedef FWSBP2StatusBlock
    @param details Src, Resp, D, Len fields of status block format
    @param sbpStatus SBP2 specific status
    @param orbOffsetHi High 32 bits of address of orb status is for.
    @param orbOffsetLo Low 32 bits of address of orb status is for.
    @param status[6] Up to 48 bytes of additional data. Length is determined by len field.
*/

typedef struct
{
    UInt8		details;
    UInt8		sbpStatus;
    UInt16		orbOffsetHi;
    UInt32		orbOffsetLo;
    UInt32		status[6];	
} FWSBP2StatusBlock;

class IOFireWireSBP2LUN;
class IOFireWireSBP2Login;

// struct sent to login complete handler

/*! 
    @typedef FWSBP2LoginCompleteParams
    @param login Pointer to IOFireWireSBP2Login object.
    @param generation FireWire generation value.
    @param status Status of login attempt.
    @param loginResponse Pointer to login response struct.
    @param statusBlock Pointer to status block buffer.
    @param statusBlockLength Length of entire status block.
*/

typedef struct
{
    IOFireWireSBP2Login * 	login;			// login object this param is sent from
    UInt32					generation;		// generation this login was attempted in 
  
    IOReturn				status;			// status of login attempt
	
    FWSBP2LoginResponsePtr	loginResponse;		// pointer to loginResponse buffer
    FWSBP2StatusBlock *		statusBlock;		// pointer to statusBlock buffer
    UInt32					statusBlockLength;	// size of statusBlock buffer
	
} FWSBP2LoginCompleteParams, *FWSBP2LoginCompleteParamsPtr;

// struct sent to logout complete handler

/*! 
    @typedef FWSBP2LogoutCompleteParams
    @param login Pointer to IOFireWireSBP2Login object.
    @param generation FireWire generation value.
    @param status Status of login attempt.
    @param statusBlock Pointer to status block buffer.
    @param statusBlockLength Length of entire status block.
*/

typedef struct
{
    IOFireWireSBP2Login * 	login;			// login object this param is sent from
    UInt32					generation;		// generation this login was attempted in

    IOReturn				status;			// status of login attempt
	
    FWSBP2StatusBlock *		statusBlock;		// pointer to statusBlock buffer
    UInt32					statusBlockLength;	// size of statusBlock buffer
	
} FWSBP2LogoutCompleteParams, *FWSBP2LogoutCompleteParamsPtr;

// struct sent with reconnect notification

/*! 
    @typedef FWSBP2LogoutCompleteParams
    @param login Pointer to IOFireWireSBP2Login object.
    @param generation FireWire generation value.
    @param status Status of login attempt.
    @param reconnectStatusBlock Pointer to status block buffer.
    @param reconnectStatusBlockLength Length of entire status block.
*/

typedef struct
{
    IOFireWireSBP2Login * 	login;			// login object this param is sent from
    UInt32					generation;		// generation this login was attempted in

    IOReturn			status;			// status of reconnect attempt

    void *			reconnectStatusBlock;		// pointer to statusBlock buffer
    UInt32			reconnectStatusBlockLength;	// size of statusBlock buffer
} FWSBP2ReconnectParams, *FWSBP2ReconnectParamsPtr;

// Callback when login and logout commands complete asynchronously

/*! 
    @typedef FWSBP2LoginCallback
    @param refCon Reference constant supplied when the notification was registered.
    @param params Structure containing additional information about the status of the login. 
*/
    
typedef void (*FWSBP2LoginCallback)(void * refCon, FWSBP2LoginCompleteParamsPtr params);

/*! 
    @typedef FWSBP2LogoutCallback
    @param refCon Reference constant supplied when the notification was registered.
    @param params Structure containing additional information about the status of the logout. 
*/

typedef void (*FWSBP2LogoutCallback)(void * refCon, FWSBP2LogoutCompleteParamsPtr params);

/*! 
    @typedef FWSBP2StatusCallback
    @param refCon Reference constant supplied when the notification was registered.
    @param status Indicates success or failure of operation. 
*/

typedef void (*FWSBP2StatusCallback)(void * refCon, IOReturn status);

/*! 
    @typedef FWSBP2FetchAgentWriteCallback
    @param refCon Reference constant supplied when the notification was registered.
    @param status Indicates success or failure of operation. 
    @param orb Indicates current last orb in chain.
*/

typedef void (*FWSBP2FetchAgentWriteCallback)(void * refCon, IOReturn status, IOFireWireSBP2ORB * orb);

/*! 
    @typedef FWSBP2NotifyParams
    @param notificationEvent Type of event we are being notified of.
    @param message buffer containing message.
    @param length length of message field.
    @param generation FireWire generation value.
*/

typedef struct
{
    void *				commandObject;
    UInt32 				notificationEvent;
    const void * 		message;
    UInt32				length;
    UInt32				generation;
} FWSBP2NotifyParams, *FWSBP2NotifyParamsPtr;


/*! 
    @typedef FWSBP2NotifyCallback
    @param refCon Reference constant supplied when the notification was registered.
    @param params FWSBP2NotifyParams containing notification information.
*/

typedef void (*FWSBP2NotifyCallback)(void * refCon, FWSBP2NotifyParamsPtr params);

/*!
    @class IOFireWireSBP2Login
    @abstract Supplies the login maintenance and Normal Command ORB execution portions of the API.
    @discussion Supplies APIs for login maintenance and command execution.  Drivers can use this 
    object to create IOFireWireSBP2ORB objects and execute them.  Solicited and unsolicited status 
    callback routines can be registered and the SBP2 services will notify the driver when the 
    appropriate status arrives.
    This class also handles login maintenance.  Supplies APIs for logging in and logging out and 
    attempts to reconnect to the LUN after bus resets.  The base FireWire services deliver bus 
    reset notification via the IOKit message routine.  The SBP2 services build on this behavior 
    and deliver reconnectFailed and reconnectComplete through the message routine as well.
*/

class IOFireWireSBP2Login : public OSObject
{
    OSDeclareDefaultStructors( IOFireWireSBP2Login )

	friend class IOFireWireSBP2ORB;
	friend class IOFireWireSBP2LUN;
	friend class IOFireWireSBP2UserClient;
	
protected:

    // command selectors
    enum
    {
        kLoginCommandIdle		= 0,
        kLoginCommandDoLogin 	= 1,
        kLoginCommandDoLogout	= 2,
        kLoginAddORB			= 3,
        kLoginRemoveORB			= 4,
		kLoginFetchAgentReset	= 5
     };

    // internal login states
    enum
    {
        kLoginStateIdle			= 0,
        kLoginStateLoggingIn	= 1,
        kLoginStateConnected	= 2,
        kLoginStateReconnect	= 3,
        kLoginStateLoggingOut	= 4,
		kLoginStateTerminated	= 5
    };

    // rom keys
    enum
    {
        kUnitCharacteristicsKey 	= 0x3A,
        kManagementAgentOffsetKey	= 0x54
    };

    // sbp2 defs
    enum
    {
        kFWSBP2RequestComplete 	= 0,
        kFWSBP2TransportFailure	= 1,
        kFWSBP2IllegalRequest	= 2,
        kFWSBP2VendorDependent	= 3
    };

    enum
    {
        kFWSBP2NoSense					= 0,
        kFWSBP2RequestTypeNotSupported	= 1,
        kFWSBP2SpeedNotSupported		= 2,
        kFWSBP2PageSizeNotSupported		= 3,
        kFWSBP2AccessDenied				= 4,
        kFWSBP2LogicalUnitNotSupported	= 5,
        kFWSBP2MaxPayloadTooSmall		= 6,
        kFWSBP2FunctionRejected			= 9,
        kFWSBP2LoginIDNotRecognized		= 10,
        kFWSBP2DummyORBCompleted		= 11,
        kFWSBP2RequestAborted			= 12,
        kFWSBP2UnspecifiedError			= 0xFF
    };

    typedef struct
    {
        UInt32		password[2];
        UInt32		loginResponseAddressHi;
        UInt32		loginResponseAddressLo;
        UInt16		options;
        UInt16		lun;
        UInt16		passwordLength;
        UInt16		loginResponseLength;
        UInt32		statusFIFOAddressHi;
        UInt32		statusFIFOAddressLo;
    } FWSBP2LoginORB;

    typedef struct
    {
        UInt32		reserved1[4];
        UInt16		options;
        UInt16		loginID;
        UInt32		reserved2;
        UInt32		statusFIFOAddressHi;
        UInt32		statusFIFOAddressLo;
    } FWSBP2ReconnectORB;
    
    typedef struct
    {
        UInt32		reserved1[4];
        UInt16		options;
        UInt16		loginID;
        UInt32		reserved2;
        UInt32		statusFIFOAddressHi;
        UInt32		statusFIFOAddressLo;
    } FWSBP2LogoutORB;

protected:

    ///////////////////////////////////////////////////////////////////
    // private interface methods

	// IOFireWireSBP2LUN methods
	virtual void clearAllTasksInSet( void );
    virtual bool initWithLUN( IOFireWireSBP2LUN * lun );
    virtual void suspendedNotify( void );
    virtual void resumeNotify( void );
	
	// IOFireWireSBP2ORB methods
	virtual IOFireWireUnit * getFireWireUnit( void );
    virtual IOFireWireSBP2LUN * getFireWireLUN( void );
    virtual bool isFetchAgentWriteInProgress( void );
    virtual bool isConnected( void );
    virtual IOReturn removeORB( IOFireWireSBP2ORB * orb );
    virtual IOReturn appendORBImmediate( IOFireWireSBP2ORB * orb );
    virtual IOReturn appendORB( IOFireWireSBP2ORB * orb );
    virtual void sendTimeoutNotification( IOFireWireSBP2ORB * orb );
     
protected:
  
    // reserved for future use
    struct ExpansionData { };
    ExpansionData *reserved; 
          
    IOFireWireSBP2LUN * 	fLUN;
    IOFireWireUnit *		fUnit;
	IOFireWireController *	fControl;
	IOCommandGate * 		fGate;

    FWSBP2LoginCallback		fLoginCompletionCallback;
    void *	  				fLoginCompletionRefCon;

    FWSBP2LogoutCallback	fLogoutCompletionCallback;
    void *					fLogoutCompletionRefCon;
    
    FWSBP2NotifyCallback	fStatusNotifyCallback;
    void *					fStatusNotifyRefCon;

    FWSBP2NotifyCallback	fUnsolicitedStatusNotifyCallback;
    void *					fUnsolicitedStatusNotifyRefCon;
    
    UInt32					fLoginFlags;
    UInt32					fReconnectTime;
    
    UInt32					fLoginState;
    UInt32					fManagementOffset;
    UInt32					fManagementTimeout;
    UInt32					fMaxORBSize;
    UInt32					fMaxCommandBlockSize;
    UInt16					fLoginNodeID;
    UInt32					fLoginGeneration;
    UInt32					fLoginID;
    UInt32					fReconnectHold;
    UInt32					fMaxPayloadSize;
    void *					fRefCon;
    
    // resources
    
    FWSBP2LoginORB			fLoginORB;
    IOFWAddressSpace *		fLoginORBAddressSpace;
    FWAddress				fLoginORBAddress;

    FWSBP2LoginResponse		fLoginResponse;
    IOFWAddressSpace *		fLoginResponseAddressSpace;
    FWAddress				fLoginResponseAddress;

    FWSBP2ReconnectORB		fReconnectORB;
    IOFWAddressSpace *		fReconnectORBAddressSpace;
    FWAddress				fReconnectORBAddress;
    
    FWSBP2StatusBlock		fStatusBlock;
    IOFWAddressSpace *		fStatusBlockAddressSpace;
    FWAddress				fStatusBlockAddress;

    FWSBP2StatusBlock		fReconnectStatusBlock;
    IOFWAddressSpace *		fReconnectStatusBlockAddressSpace;
    FWAddress				fReconnectStatusBlockAddress;

    FWSBP2LogoutORB			fLogoutORB;
    IOFWAddressSpace *		fLogoutORBAddressSpace;
    FWAddress				fLogoutORBAddress;
    bool					fLogoutPending;
	
    IOFWWriteCommand *		fLoginWriteCommand;
    IOMemoryDescriptor *	fLoginWriteCommandMemory;
    bool					fLoginWriteInProgress;
     
    IOFWWriteCommand *		fReconnectWriteCommand;
    IOMemoryDescriptor *	fReconnectWriteCommandMemory;
    bool					fReconnectWriteInProgress;
    bool					fReconnectWriteInterrupted;
    
    IOFWWriteCommand *		fLogoutWriteCommand;
    IOMemoryDescriptor *	fLogoutWriteCommandMemory;
    bool					fLogoutWriteInProgress;
    
    IOFWCommand *			fLoginTimeoutCommand;
    bool					fLoginTimeoutTimerSet;

    IOFWDelayCommand *		fReconnectTimeoutCommand;
    bool					fReconnectTimeoutTimerSet;

    IOFWCommand *			fLogoutTimeoutCommand;
    bool					fLogoutTimeoutTimerSet;

    FWAddress 				fFetchAgentAddress;
    IOMemoryDescriptor *	fFetchAgentWriteCommandMemory;
    FWAddress 				fLastORBAddress;
    IOFireWireSBP2ORB *		fLastORB;
    IOFWWriteCommand *		fFetchAgentWriteCommand;
    bool					fFetchAgentWriteCommandInUse;
	FWSBP2FetchAgentWriteCallback 	fFetchAgentWriteCompletion;
	void * 							fFetchAgentWriteRefCon;
	IOFireWireSBP2ORB *		fORBToWrite;
	
    OSSet *					fORBSet;
    OSIterator *			fORBSetIterator;
 
    void *					fPasswordBuf;
    UInt32					fPasswordLen;
    IOFWAddressSpace *		fPasswordAddressSpace;
    FWAddress				fPasswordAddress;
	IOMemoryDescriptor *	fPasswordDescriptor;

	bool fSuspended;
	
	UInt32					fLoginRetryDelay;
	UInt32					fLoginRetryCount;
	UInt32					fLoginRetryMax;
	IOFWDelayCommand *		fLoginRetryTimeoutCommand;
    bool					fLoginRetryTimeoutTimerSet;
	IOFireWireSBP2Target * 	fTarget;
	
	bool					fUnsolicitedStatusEnableRequested;
	
	IOFWDelayCommand *		fReconnectRetryTimeoutCommand;
    bool					fReconnectRetryTimeoutTimerSet;	

	int						fCriticalSectionCount;
	
	// init / destroy
    virtual IOReturn getUnitInformation( void );
    virtual IOReturn allocateResources( void );
    virtual void free( void ) APPLE_KEXT_OVERRIDE;
        	
	// orb add / remove
	virtual IOReturn addORB( IOFireWireSBP2ORB * orb );
	static IOReturn staticExecuteAddORB( OSObject *self, void * orb, void *, void *, void * );
	virtual IOReturn executeAddORB( IOFireWireSBP2ORB * orb );

	static IOReturn staticExecuteRemoveORB( OSObject *self, void * orb, void *, void *, void * );
	virtual IOReturn executeRemoveORB( IOFireWireSBP2ORB * orb );

    //
    // login
    //

    static IOReturn staticExecuteLogin( OSObject *self, void *, void *, void *, void * );
    virtual IOReturn executeLogin( void );
    virtual void abortLogin( void );
    
    // login write complete handler
    static void loginWriteCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
    virtual void loginWriteComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

    // login timeout handler
    static void loginTimeoutStatic( void *refcon, IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
    virtual void loginTimeout( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );

    // status block write handler
    static UInt32 statusBlockWriteStatic(void *refcon, UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
                                         const void *buf, IOFWRequestRefCon lockRead);
   	virtual UInt32 statusBlockWrite( UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len, const void *buf, IOFWRequestRefCon lockRead);
	virtual void completeLogin( IOReturn state, const void *buf = NULL, UInt32 len = 0, void * buf2 = NULL );

    //
    // reconnect
    //
    
    virtual void doReconnect( void );
    virtual void restartReconnect( void );
    virtual void startReconnectTimer( void );

    // reconnect write complete handler
    static void reconnectWriteCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
    virtual void reconnectWriteComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

    // reconnect timeout handler
    static void reconnectTimeoutStatic( void *refcon, IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
    virtual void reconnectTimeout( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );

    // reconnect status block
    static UInt32 reconnectStatusBlockWriteStatic(void *refcon, UInt16 nodeID, IOFWSpeed &speed, FWAddress addr,
                                                  UInt32 len, const void *buf, IOFWRequestRefCon lockRead);
   	virtual UInt32 reconnectStatusBlockWrite( UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
                                              const void *buf, IOFWRequestRefCon lockRead);

    virtual void sendReconnectNotification( UInt32 event );
    virtual void sendReconnectNotificationWithStatusBlock( UInt32 event );

    //
    // logout
    //
    
	static IOReturn staticExecuteLogout( OSObject *self, void *, void *, void *, void * );
    virtual IOReturn executeLogout( void );
    
    // logout write complete handler
    static void logoutWriteCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
    virtual void logoutWriteComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

    // logout timeout handler
    static void logoutTimeoutStatic( void *refcon, IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
    virtual void logoutTimeout( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
	
	virtual void completeLogout( IOReturn state, const void *buf = NULL, UInt32 len = 0);

	// fetch agent write complete handler
    static void fetchAgentWriteCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
    virtual void fetchAgentWriteComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

    //
    // fetch agent
    //

	bool 							fFetchAgentResetInProgress;
	UInt32 							fFetchAgentResetBuffer;
	FWAddress 						fFetchAgentResetAddress;
	IOFWWriteQuadCommand *			fFetchAgentResetCommand;
	void * 							fFetchAgentResetRefCon;
	FWSBP2StatusCallback 			fFetchAgentResetCompletion;

	static IOReturn staticExecuteFetchAgentReset( OSObject *self, void *, void *, void *, void * );
	virtual IOReturn executeFetchAgentReset( void );
	static void fetchAgentResetCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
	virtual void fetchAgentResetComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

	//
	// doorbell
	//
	
	bool 							fDoorbellInProgress;
	bool							fDoorbellRingAgain;
	UInt32 							fDoorbellBuffer;
	FWAddress 						fDoorbellAddress;
	IOFWWriteQuadCommand *			fDoorbellCommand;
	
	static IOReturn staticExecuteDoorbell( OSObject *self, void *, void *, void *, void * );
	virtual IOReturn executeDoorbell( void );
	static void doorbellCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
	virtual void doorbellComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

	//
	// enable unsolicited satus
	//

	bool 							fUnsolicitedStatusEnableInProgress;
	UInt32 							fUnsolicitedStatusEnableBuffer;
	FWAddress 						fUnsolicitedStatusEnableAddress;
	IOFWWriteQuadCommand *			fUnsolicitedStatusEnableCommand;

	static IOReturn staticExecuteUnsolicitedStatusEnable( OSObject *self, void *, void *, void *, void * );
	virtual IOReturn executeUnsolicitedStatusEnable( void );
	static void unsolicitedStatusEnableCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
	virtual void unsolicitedStatusEnableComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

	//
	// busy timeout stuff
	//
	
	bool 							fSetBusyTimeoutInProgress;
	UInt32 							fSetBusyTimeoutBuffer;
	FWAddress 						fSetBusyTimeoutAddress;
	IOFWWriteQuadCommand *			fSetBusyTimeoutCommand;
	
	bool							fInCriticalSection;

	UInt16					fLocalNodeID;
	bool					fFastStartSupported;
	UInt32					fFastStartOffset;
	UInt32					fFastStartMaxPayload;
	
	UInt32					fUserLoginGeneration;
	bool					fUserLoginGenerationSet;
	
	IOFWDelayCommand *		fFetchAgentRetryTimerCommand;
    bool					fFetchAgentRetryTimerSet;	
	
	virtual IOReturn executeSetBusyTimeout( void );
	static void setBusyTimeoutCompleteStatic( void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );
	virtual void setBusyTimeoutComplete( IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd );

private:
	// IOFireWireSBP2ORB friend class wrappers
	OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 0);

protected: 									
	virtual bool initORBWithLogin( IOFireWireSBP2ORB * orb, IOFireWireSBP2Login * login );
	virtual void setNextORBAddress( IOFireWireSBP2ORB * orb, FWAddress address );
    virtual void fetchAgentWriteComplete( IOFireWireSBP2ORB * orb, IOReturn status );
    virtual bool isORBTimerSet( IOFireWireSBP2ORB * orb );
    virtual void cancelORBTimer( IOFireWireSBP2ORB * orb );
	
	// IOFireWireSBP2LUN friend class wrappers
	virtual void removeLogin( void );
	virtual IOFireWireSBP2Target * getTarget( void );
	
	UInt32		fARDMAMax;
	bool		fPhysicalAccessEnabled;

	bool					fLoginStatusReceived;
	FWSBP2StatusBlock		fLoginStatusBlock;
	UInt32					fLoginStatusBlockLen;
		
private:

    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 3);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 4);
	OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 5);

public:

	//////////////////////////////

    /*!
		@function createORB
		@abstract Creates a new IOFireWireSBP2ORB for this login.
		@discussion	Create a new IOFireWireSBP2ORB for this login.  It can be configured 
        with it's accessors and executed with submitORB below.
        @result Returns a pointer to the new ORB object.
	*/
    	
	virtual IOFireWireSBP2ORB * createORB( void );
	
    /*!
		@function submitORB
		@abstract Submits the given orb
		@discussion	Starts execution of the given ORB.  If the ORB is an immediate ORB it's 
        addresss is written to the fetch agent.  If it is a non immediate orb its address 
        is appended to the last orb of the currently processing chain.  The doorbell is not 
        rung automatically it must be run manually with the ringDoorbell command described below.
        @param orb The orb to be executed.	
        @result Returns kIOReturnSuccess if the ORB has been started successfully.
	*/
    
    virtual IOReturn submitORB( IOFireWireSBP2ORB * orb );

    // set callbacks
    
    /*!
		@function setStatusNotifyProc
		@abstract Sets the callback to be called on normal command status.
		@discussion The supplied callback is called when normal command status is recieved, when 
        a normal command times out, or when a normal command is aborted. 
        "notificationEvent" in the callback's params will indicate what happened.
        It will be set to one of the following values: kFWSBP2NormalCommandReset, kFWSBP2NormalCommandStatus, 
        or kFWSBP2NormalCommandTimeout.  If the event type is kFWSBP2NormalCommandTimeout and "len" is 
        non-zero then "message" contains the data written to the status block.
        @param refCon refCon passed to callback.	
        @param callback address of callback method of type FWSBP2NotifyCallback.
	*/
    
    virtual void setStatusNotifyProc( void * refCon, FWSBP2NotifyCallback callback );

    /*!
		@function getStatusNotifyProc
		@abstract Returns the callback to be called on normal command status.
		@discussion Returns the refCon and callback address of the status notify callback.
        @param refCon output parameter returning the refCon to be passed to the callback.	
        @param callback output parameter returning the address of the callback.
	*/
    
    virtual void getStatusNotifyProc( void ** refCon, FWSBP2NotifyCallback * callback );

    /*!
		@function setUnsolicitedStatusNotifyProc
		@abstract Sets the callback to be called on normal command status.
		@discussion The supplied callback is called when unsolicited status is recieved.
        "notificationEvent" in the callback's params will indicate what happened.  In this 
        case it will be set to kFWSBP2UnsolicitedStatus. If "len" is 
        non-zero then "message" contains the data written to the status block. Note: any buffers 
        returned by callbacks are only valid for the duration of the login and should not have 
        their contents modified.
        @param refCon refCon passed to callback.	
        @param callback address of callback method of type FWSBP2NotifyCallback.
	*/
    
    virtual void setUnsolicitedStatusNotifyProc( void * refCon, FWSBP2NotifyCallback callback );
    
    /*!
		@function getUnsolicitedStatusNotifyProc
		@abstract Returns the callback to be called on unsolicited status.
		@discussion Returns the refCon and callback address of the unsolicited status notify callback.
        @param refCon output parameter returning the refCon to be passed to the callback.	
        @param callback output parameter returning the address of the callback.
	*/
    
    virtual void getUnsolicitedStatusNotifyProc( void ** refCon, FWSBP2NotifyCallback * callback );

    // command handling
    
    /*!
		@function setLoginCompletion
		@abstract Sets the callback to be called when a login attempt is complete.
		@discussion The supplied callback is called when a login attempt has completed. "status" in the
        callback's params should be checked to determine the success or failure of the login attempt.  
        If "statusBlock" is non-null then login status was written and it has been supplied here.  If 
        the login attempt was successful then the login response will be supplied in the "loginResponse" 
        buffer.  Note: all buffers supplied to callbacks are only valid for the duration of the callback.  
        Also, you are not to modify the contents of any supplied buffer.
        @param refCon refCon passed to callback.	
        @param callback address of callback method of type FWSBP2LoginCallback.
	*/
    
	virtual void setLoginCompletion( void * refCon, FWSBP2LoginCallback completion );
    
    /*!
		@function submitLogin
		@abstract Attempts to login to the LUN.		
        @discussion This call begins the login process.  The login object should be configured prior 
        to this call. If kIOReturnSuccess is returned from this call then the loginCompletion routine 
        will be called when the login completes (successfully or unsuccesfully). 
        @result Returns kIOReturnSuccess login has successlly begun.
	*/
    
    virtual IOReturn submitLogin( void );
	
    /*!
		@function submitLogout
		@abstract Attempts to logout of the LUN.		
        @discussion This call begins the logout process.  If kIOReturnSuccess is returned from this call then
        the logoutCompletion routine will be called when the logout completes (successfully or unsuccesfully). 
        @result Returns kIOReturnSuccess if logout has successfully begun.
	*/
    
    virtual IOReturn submitLogout( void );
 
    /*!
		@function setLogoutCompletion
		@abstract Sets the callback to be called when a logout attempt is complete.
		@discussion The supplied callback is called when a logout attempt has completed. "status" in the
        callback's params should be checked to determine the success or failure of the logout attempt.  
        If "statusBlock" is non-null then logout status was written and it has been supplied here. 
        Note: all buffers supplied to callbacks are only valid for the duration of the callback.  
        Also, you are not to modify the contents of any supplied buffer.
        @param refCon refCon passed to callback.	
        @param callback address of callback method of type FWSBP2LogoutCallback.
	*/
   
    virtual void setLogoutCompletion( void * refCon, FWSBP2LogoutCallback completion );
 
    /*!
		@function setFetchAgentWriteCompletion
		@abstract Sets the callback to be called when the fetch agent write completes.
		@discussion When an immediate orb is executed with submitORB, it's address is written to a 
        specific address on the device.  This address is called the fetch agent.  The device the 
        reads that orb from the Mac's memory and executes it.  With this call you can register to 
        be called back when this write to the fetch agent completes.  The SBP2 services guarantee 
        that the fetch agent write will be complete before status is reported for an ORB, so for 
        most drivers this notification is not required.
        @param refCon refCon passed to callback.	
        @param callback address of callback method of type FWSBP2FetchAgentWriteCallback.
	*/
       
	virtual void setFetchAgentWriteCompletion( void * refCon, FWSBP2FetchAgentWriteCallback completion );

    /*!
		@function setFetchAgentResetCompletion
		@abstract Sets the callback to be called when a fetch agent reset completes.
		@discussion The fetch agent state machine on the device can be reset by a write to a specific 
        register.  The SBP2 services offer a utility method to reset the fetch agent.  You can register 
        a callback routine here to be notified when this rest write completes.
        @param refCon refCon passed to callback.	
        @param callback address of callback method of type FWSBP2FetchAgentWriteCallback.
	*/
    
    virtual void setFetchAgentResetCompletion( void * refCon, FWSBP2StatusCallback completion );

    /*!
		@function submitFetchAgentReset
		@abstract Resets the LUN's fetch agent.
		@discussion The fetch agent state machine on the device can be reset by a write to a specific 
        register.  This reset can be intiated by a call to this method.  Notification of the completion 
        of this write can be had by registering a callback with the setFetchAgentResetCompletion method.
        @result Returns kIOReturnSuccess if the reset started successfully.
	*/
    
	virtual IOReturn submitFetchAgentReset( void );

    /*!
		@function ringDoorbell
		@abstract Rings the doorbell on the LUN.
		@discussion Non-immediate appends to the ORB chain may require the fetch agent state machine 
        to be notified of the new ORB's presence.  This is accomplished by writing to the so called 
        doorbell register.  This method begins one of those writes.
        @result Returns kIOReturnSuccess if the ring started successfully.
	*/
    
 	virtual IOReturn ringDoorbell( void );
    
    /*!
		@function enableUnsolicitedStatus
		@abstract Enables unsolicited status.
		@discussion After unsolicited is sent the device will not send any additional unsolicited status 
        until a specific register is written.  This serves as a sort of flow-control for unsolicited status.  
        After unsolicited status is recieved and processed drivers will want to reenable the delivery 
        of unsolicted status by a call to this method.
        @result Returns kIOReturnSuccess if the status enable write started successfully.
	*/
        
	virtual IOReturn enableUnsolicitedStatus( void );
	
    // accessors
    
    /*!
		@function getMaxCommandBlockSize
		@abstract Returns the maximum command block size.
		@discussion The device publishes an upper limit on the size of command block that it can 
        accept.  That value can be accessed via this method.
        @result Returns a UInt32 containing the maximum command block size.
	*/
    
    virtual UInt32 getMaxCommandBlockSize( void );
    
    /*!
		@function getLoginID
		@abstract Returns the current login ID.
		@discussion When we successfully login to a device.  The device gives us a unique login id.  
        This is used internally for reconnecting to the device after bus resets and for certain other 
        management ORBs.  Most drivers are probably not interested in this value.
        @result Returns a UInt32 containing the current login ID.
	*/
    
    virtual UInt32 getLoginID( void );

    /*!
		@function setLoginFlags
		@abstract Sets login configuration flags.
		@discussion Configures the login behavior according to the provided flags.  Currently two 
        flags are defined for this API.  kFWSBP2ExclusiveLogin sets the exclusive login bit in the 
        login ORB. kFWSBP2DontSynchronizeMgmtAgent allows simultaneous logins or reconnects to LUNs
        with a common management agent (ie LUNs in the same unit directory).
        @param loginFlags the login configuration flags.
	*/
    
    virtual void setLoginFlags( UInt32 loginFlags );
    
    /*!
		@function getLoginFlags
		@abstract Returns the currently set login flags.
		@discussion Returns the current state of the login flags.  Currently there is only one
        flag defined for this API.  kFWSBP2ExclusiveLogin indicates that the exclusive login bit
        should be set in the login ORB.
        @result Returns a UInt32 containing the currently set login flags.
	*/
    
    virtual UInt32 getLoginFlags( void );

    /*!
		@function setReconnectTime
		@abstract Sets the desired reconnect duration.
		@discussion The target and initiator arbitrate the duration of the reconnect timeout.  Here 
        the initiator specifies its desired timeout time in 2^reconnectTime seconds.  After a 
        successful login the device returns the actual timeout value it wishes to use.  This value 
        may be less than the reconnect timeout that the intiator specified if this is all that the 
        device can support.
        @param reconnectTime The desired reconnect timeout encoded as 2^reconnectTime seconds.
	*/
    
    virtual void setReconnectTime( UInt32 reconnectTime );
    
    /*!
		@function getReconnectTime
		@abstract Returns the currently set reconnect time.
		@discussion Returns the currently desired initiator reconnect time encoded as 2^time seconds.
        @result Returns a UInt32 containing the currently set reconnect time.
	*/
    
    virtual UInt32 getReconnectTime( void );

    /*!
		@function setMaxPayloadSize
		@abstract Sets the maximum data transfer length for a normal command ORB.
		@discussion Sets the maximum data transfer length for a normal command ORB.  This value is 
        the maximum for all ORBs sent to this LUN.  This can be trimmed further on an ORB by ORB basis, 
        by a similar call in the IOFireWireSBP2ORB itself.
        @param reconnectTime The desired maximum payload size in bytes.
	*/
    
    virtual void setMaxPayloadSize( UInt32 maxPayloadSize );
    
    /*!
		@function getMaxPayloadSize
		@abstract Returns the currently set maximum payload size.
		@discussion Returns the currently global maximum payload size in bytes.
        @result Returns a UInt32 containing the currently set maximum payload size.
	*/

    virtual UInt32 getMaxPayloadSize( void );

    /*!
		@function setPassword
		@abstract Sets the login password.
		@discussion Sets the login password using a buffer and a length.  An alternate version exists 
        that accepts an IOMemoryDescriptor.  If the password length is 8 or less the password is copied 
        directly into the login orb.  If the length is greater than 8 the buffer is referenced by address 
        in the login ORB.  In this case the buffer is not copied and should remain allocated for the 
        duration of the login attempt.
        @param buf a pointer to the password buffer.
        @param len the length in bytes of the password buffer.
        @result Returns kIOReturnSuccess on success.
	*/

    virtual IOReturn setPassword( void * buf, UInt32 len );

    /*!
		@function setPassword
		@abstract Sets the login password.
		@discussion Sets the login password using an IOMemoryDescriptor.  An alternate version exists 
        that accepts a buffer and a length.  If the password length is 8 or less the password is copied 
        directly into the login orb.  If the length is greater than 8 the buffer is referenced by address 
        in the login ORB.
        @param memory an IOMemoryDescriptor referencing the password.
        @result Returns kIOReturnSuccess on success.
	*/
    
    virtual IOReturn setPassword( IOMemoryDescriptor * memory );


    /*!
		@function setRefCon
		@abstract Sets the login refCon.
		@discussion Sets a user defined value on the login that can be retrieved later with the 
        method getRefCon.
        @param refCon a user defined value.
    */
    
    virtual void setRefCon( void * refCon );
    
    /*!
		@function getRefCon
		@abstract Returns the refCon set with setRefCon.
		@discussion Returns the user defined value previously stored in the login with setRefCon.
        @result Returns the previously stored user defined value.
	*/
    
    virtual void * getRefCon( void );
	
    /*!
		@function setBusyTimeoutRegisterValue
		@abstract Sets the value to be written to the BUSY_TIMEOUT register.
		@discussion 1394-1995 defines a register known as the BUSY_TIMEOUT register. This register 
        controls the busy retry behavior of your device.  The initial value for this register is 
        0x00000000.  Which means busied transactions will not be retried.  Since most devices want 
        their transactions retired on busy acks, the SBP2 service automatically updates the 
        BUSY_TIMEOUT register with the value specified here whenever necessary.  Most drivers should 
        set this value to 0x0000000f.
        @param timeout desired value of the BUSY_TIMEOUT register.
    */
    
	virtual void setBusyTimeoutRegisterValue( UInt32 timeout );
    
protected:

    virtual void startORBTimer( IOFireWireSBP2ORB * orb );
    virtual void prepareORBForExecution( IOFireWireSBP2ORB * orb );

    static IOReturn staticExecuteORB( OSObject *self, void * orb, void *, void *, void * );
    virtual IOReturn executeORB( IOFireWireSBP2ORB * orb );

public:

    /*! 
        @function release
        @abstract Primary implementation of the release mechanism.
        @discussion See OSObject.h for more information.  When retainCount == when then call free(). 
    */
    
    virtual void release() const APPLE_KEXT_OVERRIDE;
	
	/*! 
        @function setLoginRetryCountAndDelayTime
        @abstract Sets login retry behavior.
        @discussion Sets login retry behavior.
        @param retryCount number of times to retry logins
		@param uSecs delay time in microseconds between login retries
    */
	
	virtual void setLoginRetryCountAndDelayTime( UInt32 retryCount, UInt32 uSecs );
	
protected:
	virtual IOReturn initialExecuteLogin( void );
	virtual void startLoginRetryTimer( void );
	virtual void stopLoginRetryTimer( void );
	static void loginRetryTimeoutStatic( void *refcon, IOReturn status,
										 IOFireWireBus *bus, IOFWBusCommand *fwCmd );
	virtual void loginRetryTimeout( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd);

	virtual void startReconnectRetryTimer( void );
	virtual void stopReconnectRetryTimer( void );
	static void reconnectRetryTimeoutStatic( void *refcon, IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
	virtual void reconnectRetryTimeout( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );

	virtual bool isORBAppended( IOFireWireSBP2ORB * orb );
	virtual void setORBIsAppended( IOFireWireSBP2ORB * orb, bool state );

public:    
	virtual void setAddressLoForLoginORBAndResponse( UInt32 addressLoORB, UInt32 addresLoResponse );
	
	virtual void setLoginGeneration( UInt32 generation );
	virtual void clearLoginGeneration( void );

protected:
	void startFetchAgentRetryTimer( UInt32 duration );
	void stopFetchAgentRetryTimer( void );
	static void fetchAgentRetryTimerStatic( void *refcon, IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
	void fetchAgentRetryTimer( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
	
	void terminateNotify( void );
	void processLoginWrite( void );

public:
	
	bool	isPhysicalAccessEnabled( void );
	UInt32	getARDMMax( void );
	
private:
    
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 6);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 7);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2Login, 8);

 };

#endif
