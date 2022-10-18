/*
 * Copyright (c) 1998-2001 Apple Computer, Inc. All rights reserved.
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
#ifndef _IOKIT_IOFWASYNCSTREAMLISTENER_H
#define _IOKIT_IOFWASYNCSTREAMLISTENER_H

#include <IOKit/firewire/IOFireWireLink.h>
#include <IOKit/firewire/IOFWCommand.h>
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/firewire/IOFWDCLProgram.h>

class IOFWAsyncStreamReceiver;
class IOFWAsyncStreamReceivePort;

/*! @class IOFWAsyncStreamListener
*/
class IOFWAsyncStreamListener : public OSObject
{
    OSDeclareDefaultStructors(IOFWAsyncStreamListener)

friend class IOFWAsyncStreamReceiver;
friend class IOFireWireController;

public:

/*!	@function initAll
	@abstract Creates an AsyncStream client for an Isoch channel.
	@param control	Points to IOFireWireController.
	@param channel	Isoch channel for listening.
	@param proc		Callback on packet reception.
	@param obj  Client's callback object.
	@result returns true on success, else false.	*/	
	bool initAll( IOFireWireController *control, UInt32 channel, FWAsyncStreamReceiveCallback proc, void *refcon );

/*!	@function setListenerHandler
	@abstract Set the callback that should be called to handle incoming async stream packets
	@param inReceiver The callback to set.
	@result Returns the callback that was previously set or nil for none.*/
	const FWAsyncStreamReceiveCallback setListenerHandler( FWAsyncStreamReceiveCallback inReceiver );

/*!	@function TurnOffNotification
	@abstract Turns off client callback notification.
	@result   none.	*/	
	inline void TurnOffNotification() { fNotify = false; };

/*!	@function TurnOnNotification
	@abstract Turns on client callback notification.
	@result   none.	*/	
	inline void TurnOnNotification() { fNotify = true; };

/*!	@function IsNotificationOn
	@abstract checks the notification state.
	@result   true if ON, else false	*/	
	inline bool IsNotificationOn() { return fNotify; };

/*!	@function setFlags
	@abstract set flags for the listener.
	@param flags indicate performance metrics.
	@result none.	*/	
	void setFlags( UInt32 flags );
	
/*!	@function getFlags
	@abstract get the flags of listener.
	@param none.
	@result flags.	*/	
	UInt32 getFlags();

/*!	@function getRefCon
	@abstract get the refcon specific to this listener.
	@param none.
	@result fRefCon refcon passed during initialization. */	
	inline void* getRefCon() { return fRefCon; };
	
/*!	@function getOverrunCounter
	@abstract get overrun counter from the DCL program.
	@param none.
	@result returns the counter value.	*/	
	UInt32 getOverrunCounter();
	
protected:

	FWAsyncStreamReceiveCallback	fClientProc; 
	void							*fRefCon;
	IOFWAsyncStreamReceiver			*fReceiver;
	bool							 fNotify;
	UInt32							 fFlags;
	IOFireWireController			*fControl;

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;
    
    virtual void		free(void) APPLE_KEXT_OVERRIDE;

private:
/*!	function getReceiver
	abstract Returns the Async Stream receiver object which tracks multiple
	         IOFWAsyncStreamListeners for the same Isoc channel. */	
	inline IOFWAsyncStreamReceiver *getReceiver() { return fReceiver; };

/*!	function invokeClients
	abstract Invokes client's callback function with fRefCon.	*/	
	void invokeClients( UInt8 *buffer );
	
    OSMetaClassDeclareReservedUnused(IOFWAsyncStreamListener, 0);
    OSMetaClassDeclareReservedUnused(IOFWAsyncStreamListener, 1);
};
#endif // _IOKIT_IOFWASYNCSTREAMLISTENER_H

