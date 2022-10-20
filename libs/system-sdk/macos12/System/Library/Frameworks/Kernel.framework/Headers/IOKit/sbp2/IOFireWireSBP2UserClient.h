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

#ifndef _IOKIT_IOFIREWIRESBP2USERCLIENT_H
#define _IOKIT_IOFIREWIRESBP2USERCLIENT_H

#include <IOKit/IOUserClient.h>

#define FIREWIREPRIVATE
#include <IOKit/firewire/IOFireWireController.h>
#undef FIREWIREPRIVATE

#include <IOKit/firewire/IOFWUserObjectExporter.h>

#include <IOKit/sbp2/IOFireWireSBP2UserClientCommon.h>
#include <IOKit/sbp2/IOFireWireSBP2LUN.h>

class IOFireWireSBP2UserClient : public IOUserClient
{
    OSDeclareDefaultStructors(IOFireWireSBP2UserClient)

protected:

    bool					fOpened;
	bool					fStarted;
    IOFireWireSBP2Login * 	fLogin;
    task_t					fTask;
	
    IOFireWireSBP2LUN *		fProviderLUN;
    OSAsyncReference64		fMessageCallbackAsyncRef;
    OSAsyncReference64		fLoginCallbackAsyncRef;
    OSAsyncReference64		fLogoutCallbackAsyncRef;
    OSAsyncReference64		fUnsolicitedStatusNotifyAsyncRef;
    OSAsyncReference64		fStatusNotifyAsyncRef;
    OSAsyncReference64		fFetchAgentResetAsyncRef;
	OSAsyncReference64		fFetchAgentWriteAsyncRef;
	
	IOFWUserObjectExporter	*	fExporter;
	
	IOFireWireLib::UserObjectHandle		fSessionRef;
	
    IOLock *                            fUserClientLock;

 	virtual IOReturn externalMethod(	uint32_t selector, 
										IOExternalMethodArguments * args,
										IOExternalMethodDispatch * dispatch, 
										OSObject * target, 
										void * reference ) APPLE_KEXT_OVERRIDE;
public:

	virtual bool initWithTask( task_t owningTask, void * securityToken, UInt32 type, OSDictionary * properties ) APPLE_KEXT_OVERRIDE;
	virtual void free () APPLE_KEXT_OVERRIDE;
				
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn clientClose( void ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn clientDied( void ) APPLE_KEXT_OVERRIDE;

	// IOFireWireSBP2ManagementORB friend class wrappers
	virtual void flushAllManagementORBs( void );

    /////////////////////////////////////////////////
    // IOFireWireSBP2LUN

    IOReturn open( IOExternalMethodArguments * arguments );
	IOReturn openWithSessionRef( IOExternalMethodArguments * arguments );
	IOReturn getSessionRef( IOExternalMethodArguments * arguments );
    IOReturn close( IOExternalMethodArguments * arguments );

    // callbacks
	IOReturn setMessageCallback( IOExternalMethodArguments * arguments );
    virtual IOReturn message( UInt32 type, IOService * provider, void * arg ) APPLE_KEXT_OVERRIDE;

    /////////////////////////////////////////////////
    // IOFireWireSBP2Login
    
    IOReturn setLoginCallback( IOExternalMethodArguments * arguments );
	IOReturn setLogoutCallback( IOExternalMethodArguments * arguments );
    IOReturn setUnsolicitedStatusNotify( IOExternalMethodArguments * arguments );
    IOReturn setStatusNotify( IOExternalMethodArguments * arguments );
	IOReturn createLogin( IOExternalMethodArguments * arguments );
    IOReturn releaseLogin( IOExternalMethodArguments * arguments );
    IOReturn submitLogin( IOExternalMethodArguments * arguments );
    IOReturn submitLogout( IOExternalMethodArguments * arguments );
	IOReturn setLoginFlags( IOExternalMethodArguments * arguments );
    IOReturn getMaxCommandBlockSize( IOExternalMethodArguments * arguments );
    IOReturn getLoginID( IOExternalMethodArguments * arguments );
    IOReturn setReconnectTime( IOExternalMethodArguments * arguments );
	IOReturn setMaxPayloadSize( IOExternalMethodArguments * arguments );
    
	IOReturn submitFetchAgentReset( IOExternalMethodArguments * arguments );
	IOReturn setFetchAgentWriteCompletion( IOExternalMethodArguments * arguments );
	IOReturn ringDoorbell( IOExternalMethodArguments * arguments );
	IOReturn enableUnsolicitedStatus( IOExternalMethodArguments * arguments );
	IOReturn setBusyTimeoutRegisterValue( IOExternalMethodArguments * arguments );
    IOReturn setPassword( IOExternalMethodArguments * arguments );

	// callbacks
	
	static void staticLoginCallback( void * refCon, FWSBP2LoginCompleteParamsPtr params );
    virtual void loginCallback( FWSBP2LoginCompleteParamsPtr params );

	static void staticLogoutCallback( void * refCon, FWSBP2LogoutCompleteParamsPtr params );
    virtual void logoutCallback( FWSBP2LogoutCompleteParamsPtr params );

	static void staticStatusNotify( void * refCon, FWSBP2NotifyParams * params );
    virtual void statusNotify( FWSBP2NotifyParams * params );

	static void staticUnsolicitedNotify( void * refCon, FWSBP2NotifyParams * params );
    virtual void unsolicitedNotify( FWSBP2NotifyParams * params );

	static void staticFetchAgentWriteComplete( void * refCon, IOReturn status, IOFireWireSBP2ORB * orb );
	virtual void fetchAgentWriteComplete( IOReturn status, IOFireWireSBP2ORB * orb );

    static void staticFetchAgentResetComplete( void * refCon, IOReturn status );
    virtual void fetchAgentResetComplete( IOReturn status );
	
    /////////////////////////////////////////////////
    // IOFireWireSBP2ORB

    IOReturn createORB(  IOExternalMethodArguments * arguments );
    IOReturn releaseORB(  IOExternalMethodArguments * arguments );
	IOReturn submitORB(  IOExternalMethodArguments * arguments );
    IOReturn setCommandFlags(  IOExternalMethodArguments * arguments );
    IOReturn setORBRefCon(  IOExternalMethodArguments * arguments );
	IOReturn setMaxORBPayloadSize(  IOExternalMethodArguments * arguments );
    IOReturn setCommandTimeout(  IOExternalMethodArguments * arguments );
	IOReturn setCommandGeneration(  IOExternalMethodArguments * arguments );
    IOReturn setToDummy(  IOExternalMethodArguments * arguments );
    IOReturn setCommandBuffersAsRanges(  IOExternalMethodArguments * arguments );
    IOReturn releaseCommandBuffers(  IOExternalMethodArguments * arguments );
    IOReturn setCommandBlock(  IOExternalMethodArguments * arguments );
	
	// LSI workaround
    IOReturn LSIWorkaroundSetCommandBuffersAsRanges(  IOExternalMethodArguments * arguments );
	IOReturn LSIWorkaroundSyncBuffersForOutput(  IOExternalMethodArguments * arguments );
	IOReturn LSIWorkaroundSyncBuffersForInput(  IOExternalMethodArguments * arguments );
															
    /////////////////////////////////////////////////
    // IOFireWireSBP2MgmtORB

	IOReturn createMgmtORB(  IOExternalMethodArguments * arguments );
    IOReturn releaseMgmtORB(  IOExternalMethodArguments * arguments );
    IOReturn setMgmtORBCallback(  IOExternalMethodArguments * arguments );
    IOReturn submitMgmtORB(  IOExternalMethodArguments * arguments );	
	IOReturn setMgmtORBCommandFunction(  IOExternalMethodArguments * arguments );
	IOReturn setMgmtORBManageeORB(  IOExternalMethodArguments * arguments );
	IOReturn setMgmtORBManageeLogin(  IOExternalMethodArguments * arguments );
	IOReturn setMgmtORBResponseBuffer(  IOExternalMethodArguments * arguments );

	// callbacks
    static void staticMgmtORBCallback( void * refCon, IOReturn status, IOFireWireSBP2ManagementORB * orb );
    virtual void mgmtORBCallback( IOReturn status, IOFireWireSBP2ManagementORB * orb );
	
	// IOFireWireSBP2MgmtORB friend class wrappers
	virtual void setMgmtORBAsyncCallbackReference( IOFireWireSBP2ManagementORB * orb, void * asyncRef );    
	virtual void getMgmtORBAsyncCallbackReference( IOFireWireSBP2ManagementORB * orb, void * asyncRef );

	    uint32_t		   checkScalarInputCount;
    uint32_t		   checkStructureInputSize;
    uint32_t		   checkScalarOutputCount;
    uint32_t		   checkStructureOutputSize;
    
    IOReturn checkArguments( IOExternalMethodArguments * args, uint32_t scalarInCount, uint32_t structInCount, 
    													uint32_t scalarOutCount, uint32_t structOutCount );

};

#endif
