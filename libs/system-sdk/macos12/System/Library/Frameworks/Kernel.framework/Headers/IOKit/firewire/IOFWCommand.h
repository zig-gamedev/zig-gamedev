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
 *	IOFWCommand.h
 *
 */
#ifndef _IOKIT_IOFWCOMMAND_H
#define _IOKIT_IOFWCOMMAND_H

#include <IOKit/IOCommand.h>
#include <IOKit/IOLib.h>

#include <IOKit/firewire/IOFireWireFamilyCommon.h>

#include <IOKit/firewire/IOFWSyncer.h>

#define kFWCmdDefaultRetries 3
#define kFWCmdZeroRetries 0
#define kFWCmdReducedRetries 2
#define kFWCmdIncreasedRetries 6

class IOMemoryDescriptor;
class IOSyncer;
class IOFireWireBus;
class IOFireWireController;
class IOFireWireNub;
class IOFWAddressSpace;	// Description of chunk of local FW address space
class IOFWCommand;
class IOFWBusCommand;
class IOFWAsyncStreamCommand;
class IOCommandGate;
class IOFWAsyncPHYCommand;

struct AsyncPendingTrans;

// Struct for head of command queue
/*!
    @struct IOFWCmdQ
    @abstract Structure for head of a queue of IOFWCommands
    @field fHead Points to the head of the queue, or NULL if queue is empty
    @field fTail Points to the tail of the queue, or NULL if queue is empty
    @function headChanged called when head command is changed, or the command
 	itself changes state.
*/

struct IOFWCmdQ
{
    IOFWCommand *fHead;
    IOFWCommand *fTail;
    bool executeQueue(bool all);
    virtual void headChanged(IOFWCommand *oldHead);
	
	virtual ~IOFWCmdQ() {}

	void checkProgress( void );
};

// Callback when device command completes asynchronously
typedef void (*FWDeviceCallback)(void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd);

// Callback when bus command completes asynchronously
typedef void (*FWBusCallback)(void *refcon, IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd);

// Callback when async stream command completes asynchronously
typedef void (*FWAsyncStreamCallback)(void *refcon, IOReturn status, IOFireWireBus *bus, IOFWAsyncStreamCommand *fwCmd);

// Callback when async stream command completes asynchronously
typedef void (*FWAsyncPHYCallback)(void *refcon, IOReturn status, IOFireWireBus *bus, IOFWAsyncPHYCommand *fwCmd );

// Callback when async stream packet is received
typedef void (*FWAsyncStreamReceiveCallback)(void *refcon, const void *buf);

#pragma mark -

/*
 * Base class for FireWire commands
 */
/*! @class IOFWCommand
*/
class IOFWCommand : public IOCommand
{
    OSDeclareAbstractStructors(IOFWCommand)

protected:
    IOReturn		fStatus;
    IOFireWireController *fControl;
    IOFWCommand *	fQueuePrev;
    IOFWCommand *	fQueueNext;
    IOFWCmdQ *		fQueue;
    UInt32			fTimeout;	// How long (in microsecs) after execute() to timeout
    AbsoluteTime	fDeadline;	// Time after which this command has timed out.
    IOFWSyncer *	fSyncWakeup;
    UInt8			fSync;
    UInt8			fCancelOnReset;
    UInt8			spare[2];
    
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct MemberVariables 
	{
		void *			fFWIMRefCon;
		IOReturn		fCompletionStatus;
		bool			fSubmitTimeLatched;
	    AbsoluteTime	fSubmitTime;
		bool			fFlush;
	};

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    MemberVariables * fMembers;

    virtual IOReturn	complete(IOReturn status);
    virtual void	updateTimer();
    virtual IOReturn	startExecution();
    
    /*	
     *	Execute the FWCommand immediately
     *	must be called with the workloop gate closed
     */
    virtual IOReturn	execute() = 0;

public:

    virtual bool	initWithController(IOFireWireController *control);
	virtual void	free( void ) APPLE_KEXT_OVERRIDE;
	
    IOReturn		getStatus() const { return fStatus; };
    
    /*	
     *	Submit the FWCommand.
     *	if queue is false the command's execute()
     *	method will be called on the caller's thread, otherwise
     *	the command wil be queued for execution on the work loop thread.
     */                          
    virtual IOReturn 	submit(bool queue = false);

    /*
     * Cancel command, causes it to complete with given status
     */
    virtual IOReturn	cancel(IOReturn reason);

    /*!
        @function setHead
        inserts a command at the head of a queue.
        @param queue queue command is being added to
    */
    virtual void setHead(IOFWCmdQ &queue);
    /*!
        @function insertAfter
        inserts a command after the specified one.
        @param prev command to insert after
        @param queue queue command is being added to
    */
    virtual void insertAfter(IOFWCommand &prev);

    /*!
        @function removeFromQ
        Removes command from current queue.
    */
    virtual void removeFromQ();

    IOFWCommand *getPrevious() const
	{ return fQueuePrev; };
    IOFWCommand *getNext() const
        { return fQueueNext; };
    const AbsoluteTime &getDeadline() const
	{ return fDeadline; };

    bool cancelOnReset() const
    { return fCancelOnReset; };

    bool Busy() const
        { return fStatus == kIOReturnBusy || fStatus == kIOFireWirePending;};
    
    void setTimeout( UInt32 timeout )
        { fTimeout = timeout; };
        
    friend struct IOFWCmdQ;

	void * getFWIMRefCon( void )
	{
		return fMembers->fFWIMRefCon;
	}
	
	void setFWIMRefCon( void * refcon )
	{
		fMembers->fFWIMRefCon = refcon;
	}

	void setFlush( bool flush )
	{
		fMembers->fFlush = flush;		
	}
	
	virtual IOReturn checkProgress( void );
	
private:
    OSMetaClassDeclareReservedUsed(IOFWCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWCommand, 1);

};

#pragma mark -

/*
 * Bus control commands
 */
/*! @class IOFWBusCommand
*/
class IOFWBusCommand : public IOFWCommand
{
    OSDeclareAbstractStructors(IOFWBusCommand)

protected:
    FWBusCallback	fComplete;
    void *		fRefCon;

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

    virtual IOReturn	complete(IOReturn status) APPLE_KEXT_OVERRIDE;

    virtual bool	initWithController(IOFireWireController *control,
				FWBusCallback completion=NULL, void *refcon=NULL);
    virtual IOReturn	reinit(FWBusCallback completion, void *refcon);

private:
    OSMetaClassDeclareReservedUnused(IOFWBusCommand, 0);

};

#pragma mark -

/*
 * Command to execute some code after a specified delay (in microseconds)
 * All it does is timeout after the specified delay, hence calling the completion
 * callback.
 */
/*! @class IOFWDelayCommand
*/
class IOFWDelayCommand : public IOFWBusCommand
{
    OSDeclareDefaultStructors(IOFWDelayCommand)

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

protected:
    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

public:
    virtual bool	initWithDelay(IOFireWireController *control, UInt32 uSecs,
                                FWBusCallback completion, void *refcon);
    virtual IOReturn	reinit(UInt32 uSecs, FWBusCallback completion, void *refcon);

private:
    OSMetaClassDeclareReservedUnused(IOFWDelayCommand, 0);

};

/*
 * Send an async request to a device
 */
class IOFWUserReadQuadletCommand ;
class IOFWUserWriteCommand ;

#pragma mark -

/*! @class IOFWAsyncCommand
*/
class IOFWAsyncCommand : public IOFWCommand
{
	OSDeclareAbstractStructors(IOFWAsyncCommand)

protected:
    IOFireWireNub *	fDevice;
    FWDeviceCallback	fComplete;
    void *		fRefCon;
    IOMemoryDescriptor *fMemDesc;
    AsyncPendingTrans *	fTrans;
    UInt32		fAddressHi;
    UInt32		fAddressLo;
    IOByteCount		fBytesTransferred;
    int			fSize;
    int			fSpeed;
    int			fMaxPack;
    int			fCurRetries;
    int			fMaxRetries;
    UInt32		fGeneration;	// bus topology fNodeID is valid for.
    UInt16		fNodeID;
    bool		fFailOnReset;
    bool		fWrite;

	typedef struct 
	{ 
		// some of our subclasses didn't have room for expansion data, so
		// we've reserved space for their use here.
		
		void *			fSubclassMembers;
		int				fMaxSpeed;
		int				fAckCode;
		UInt32			fResponseCode;
		UInt32			fFastRetryCount;
		int				fResponseSpeed;
		bool			fForceBlockRequests;
	} 
	MemberVariables;

    MemberVariables * fMembers;

    virtual IOReturn	complete(IOReturn status) APPLE_KEXT_OVERRIDE;
	virtual bool	initWithController(IOFireWireController *control) APPLE_KEXT_OVERRIDE;
    virtual bool	initAll(IOFireWireNub *device, FWAddress devAddress,
				IOMemoryDescriptor *hostMem,
				FWDeviceCallback completion, void *refcon, bool failOnReset);
    virtual bool	initAll(IOFireWireController *control,
                                UInt32 generation, FWAddress devAddress,
                                IOMemoryDescriptor *hostMem,
                                FWDeviceCallback completion, void *refcon);
	virtual void free( void ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn	reinit(FWAddress devAddress, IOMemoryDescriptor *hostMem,
				FWDeviceCallback completion, void *refcon, bool failOnReset);
    virtual IOReturn	reinit(UInt32 generation, FWAddress devAddress, IOMemoryDescriptor *hostMem,
                                FWDeviceCallback completion, void *refcon);
	bool createMemberVariables( void );
	void destroyMemberVariables( void );
public:
	// Utility for setting generation on newly created command
	virtual void	setGeneration(UInt32 generation)
	{ fGeneration = generation; }

    // To be called by IOFireWireController and derived classes.
    virtual void 	gotPacket(int rcode, const void* data, int size) = 0;
    virtual void	gotAck(int ackCode);

    // update nodeID/generation after bus reset, from the device object
    IOReturn		updateGeneration();
    // explicitly update nodeID/generation after bus reset
    IOReturn		updateNodeID(UInt32 generation, UInt16 nodeID);
    
    // Generally useful stuff
    IOByteCount		getBytesTransferred() const
    { return fBytesTransferred; };

    FWAddress		getAddress() const
    { return FWAddress(fAddressHi, fAddressLo, fNodeID); }

    bool		failOnReset() const
    { return fFailOnReset; }
    
    IOFireWireNub *	getDevice() const
    { return fDevice; }
    
    /*!
        @function setMaxPacket
        Sets the maximum size for block transfers used by the command.
        The command is initialized to use the maximum packet size calculated from the device's
        PHY speed, bus info block and the bus topology.
        Call this method before calling submit().
        @param maxBytes Maximum packet size in bytes. If the maxsize is 4 then quadlet transfers will be used.
    */
    IOReturn	setMaxPacket(UInt32 maxBytes)
    {
        if(fStatus == kIOReturnBusy || fStatus == kIOFireWirePending)
            return fStatus;
        fMaxPack = maxBytes;
        return kIOReturnSuccess;
    }

	void setMaxSpeed( int speed );
	
	void setAckCode( int ack );
	int getAckCode( void );
	
	void setRetries( int retries);
	int getMaxRetries( void );
	
	void setResponseCode( UInt32 rcode );
	UInt32 getResponseCode( void ) const;

	void setFastRetryCount( UInt32 count ) 
		{ fMembers->fFastRetryCount = count; };

	UInt32 getFastRetryCount( void ) 
		{ return fMembers->fFastRetryCount; };
		
	void setResponseSpeed( int speed ) 
		{ fMembers->fResponseSpeed = speed; };

	int getResponseSpeed( void ) 
		{ return fMembers->fResponseSpeed; };

	// forces even 4 byte transactions to be block requests
	void setForceBlockRequests( bool enabled )
		{ fMembers->fForceBlockRequests = enabled; }

	virtual IOReturn checkProgress( void ) APPLE_KEXT_OVERRIDE;
			
private:
    OSMetaClassDeclareReservedUnused(IOFWAsyncCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWAsyncCommand, 1);
    OSMetaClassDeclareReservedUnused(IOFWAsyncCommand, 2);
    OSMetaClassDeclareReservedUnused(IOFWAsyncCommand, 3);

};

#pragma mark -

/*
 * Concrete async requests - read, write and hordes of read/modify/write
 */
class IOFWReadCommand : public IOFWAsyncCommand
{
    OSDeclareDefaultStructors(IOFWReadCommand)

protected:
	
    virtual void 	gotPacket(int rcode, const void* data, int size) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

public:
    virtual bool	initAll(IOFireWireNub *device, FWAddress devAddress,
				IOMemoryDescriptor *hostMem,
				FWDeviceCallback completion, void *refcon, bool failOnReset) APPLE_KEXT_OVERRIDE;
    virtual bool	initAll(IOFireWireController *control,
                                UInt32 generation, FWAddress devAddress,
                                IOMemoryDescriptor *hostMem,
                                FWDeviceCallback completion, void *refcon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn	reinit(FWAddress devAddress, IOMemoryDescriptor *hostMem,
				FWDeviceCallback completion=NULL, void *refcon=NULL,
				bool failOnReset=false) APPLE_KEXT_OVERRIDE;
    virtual IOReturn	reinit(UInt32 generation, FWAddress devAddress, IOMemoryDescriptor *hostMem,
                                FWDeviceCallback completion=NULL, void *refcon=NULL) APPLE_KEXT_OVERRIDE;

private:
    OSMetaClassDeclareReservedUnused(IOFWReadCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWReadCommand, 1);
};

#pragma mark -

/*!
	@class IOFWReadQuadCommand
	@discussion An easier to use version of IOFWReadCommand for use when the data to be transferred
	is an integer number of quads.
	Note that block read requests will be used for transfers greater than one quad unless setMaxPacket(4)
	is called.
*/

class IOFWReadQuadCommand : public IOFWAsyncCommand
{
    OSDeclareDefaultStructors(IOFWReadQuadCommand)

protected:

    UInt32 *	fQuads;

	typedef struct 
	{ 
		bool	fPingTime;
	}
	MemberVariables;
	
	bool createMemberVariables( void );
	void destroyMemberVariables( void );
	virtual void free( void ) APPLE_KEXT_OVERRIDE;
	
    virtual void 	gotPacket(int rcode, const void* data, int size) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

public:
    virtual bool	initAll(IOFireWireNub *device, FWAddress devAddress,
				UInt32 *quads, int numQuads,
				FWDeviceCallback completion, void *refcon, bool failOnReset);

    virtual bool	initAll(IOFireWireController *control,
                                UInt32 generation, FWAddress devAddress,
                                UInt32 *quads, int numQuads,
                                FWDeviceCallback completion, void *refcon);
    
    virtual IOReturn	reinit(FWAddress devAddress, UInt32 *quads, int numQuads,
				FWDeviceCallback completion=NULL, void *refcon=NULL,
				bool failOnReset=false);

    virtual IOReturn	reinit(UInt32 generation, FWAddress devAddress, UInt32 *quads, int numQuads,
                                FWDeviceCallback completion=NULL, void *refcon=NULL);

	void setPingTime( bool state ) 
		{ ((MemberVariables*)fMembers->fSubclassMembers)->fPingTime = state; };    

private:
    OSMetaClassDeclareReservedUnused(IOFWReadQuadCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWReadQuadCommand, 1);
};

#pragma mark -

class IOFWWriteCommand : public IOFWAsyncCommand
{

    OSDeclareDefaultStructors(IOFWWriteCommand)

protected:

    int			fPackSize;
	
	typedef struct 
	{ 
		bool 	fDeferredNotify;
		bool	fFastRetryOnBusy;
	}
	MemberVariables;
		
    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

    virtual void 	gotPacket( int rcode, const void* data, int size ) APPLE_KEXT_OVERRIDE;

	bool createMemberVariables( void );
	void destroyMemberVariables( void );
	
public:

	virtual bool	initWithController(IOFireWireController *control) APPLE_KEXT_OVERRIDE;
    virtual bool	initAll(	IOFireWireNub *			device, 
								FWAddress 				devAddress,
								IOMemoryDescriptor *	hostMem,
								FWDeviceCallback 		completion, 
								void *					refcon, 
								bool 					failOnReset ) APPLE_KEXT_OVERRIDE;
								
    virtual bool	initAll(	IOFireWireController *	control,
                                UInt32 					generation, 
								FWAddress 				devAddress,
                                IOMemoryDescriptor *	hostMem,
                                FWDeviceCallback 		completion, 
								void *					refcon ) APPLE_KEXT_OVERRIDE;
	virtual void free( void ) APPLE_KEXT_OVERRIDE;
								
    virtual IOReturn	reinit(	FWAddress 				devAddress, 
								IOMemoryDescriptor *	hostMem,
								FWDeviceCallback 		completion = NULL, 
								void *					refcon = NULL,
								bool 					failOnReset = false ) APPLE_KEXT_OVERRIDE;
								
    virtual IOReturn	reinit(	UInt32 					generation, 
								FWAddress 				devAddress, 
								IOMemoryDescriptor *	hostMem,
                                FWDeviceCallback		completion = NULL, 
								void *					refcon = NULL ) APPLE_KEXT_OVERRIDE;

	void setDeferredNotify( bool state ) 
		{ ((MemberVariables*)fMembers->fSubclassMembers)->fDeferredNotify = state; };

	void setFastRetryOnBusy( bool state ) 
		{ ((MemberVariables*)fMembers->fSubclassMembers)->fFastRetryOnBusy = state; };
	
private:

    OSMetaClassDeclareReservedUnused(IOFWWriteCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWWriteCommand, 1);

};

#pragma mark -

/*!
	@class IOFWWriteQuadCommand
	@discussion An easier to use version of IOFWWriteCommand for use when the data to be transferred
	is small and an integer number of quads.
	Note that block read requests will be used for transfers greater than one quad unless setMaxPacket(4)
	is called.
	kMaxWriteQuads is the largest legal number of quads that this object can be asked to transfer
	(the data is copied into an internal buffer in init() and reinit()).
*/

class IOFWWriteQuadCommand : public IOFWAsyncCommand
{

    OSDeclareDefaultStructors(IOFWWriteQuadCommand)

public:

    enum 
	{
        kMaxWriteQuads = 8
    };
    
protected:

    UInt32					fQuads[kMaxWriteQuads];
    UInt32 *				fQPtr;
    int						fPackSize;
	
	typedef struct 
	{ 
		bool 					fDeferredNotify;
		IOMemoryDescriptor *	fMemory;
	} 
	MemberVariables;
	
    virtual void 	gotPacket( int rcode, const void* data, int size ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

	bool createMemberVariables( void );
	void destroyMemberVariables( void );
	
public:
	virtual bool	initWithController(IOFireWireController *control) APPLE_KEXT_OVERRIDE;
    
	virtual bool	initAll(	IOFireWireNub *		device, 
								FWAddress 			devAddress,
								UInt32 *			quads, 
								int 				numQuads,
								FWDeviceCallback	completion, 
								void *				refcon, 
								bool 				failOnReset );
								
    virtual bool	initAll(	IOFireWireController *	control,
                                UInt32 					generation, 
								FWAddress 				devAddress,
                                UInt32 *				quads, 
								int 					numQuads,
                                FWDeviceCallback 		completion, 
								void *					refcon );

	virtual void free( void ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	reinit(	FWAddress 			devAddress, 
								UInt32 *			quads, 
								int 				numQuads,
								FWDeviceCallback	completion = NULL, 
								void *				refcon = NULL,
								bool 				failOnReset = false );
								
    virtual IOReturn	reinit(	UInt32 				generation, 
								FWAddress 			devAddress, 
								UInt32 *			quads, 
								int 				numQuads,
                                FWDeviceCallback 	completion = NULL, 
								void *				refcon = NULL );

protected:
	
	void setQuads( UInt32 * quads, int numQuads );
	bool createMemoryDescriptor( void );
	void destroyMemoryDescriptor( void );

public:

 	void setDeferredNotify( bool state ) 
		{ ((MemberVariables*)fMembers->fSubclassMembers)->fDeferredNotify = state; };
	
private:
    
	OSMetaClassDeclareReservedUnused(IOFWWriteQuadCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWWriteQuadCommand, 1);

};

/*
 * May need more parameters for some of these,
 * and/or derive from a base Lock transaction command
 */

#pragma mark -

/*! @class IOFWCompareAndSwapCommand
*/
class IOFWCompareAndSwapCommand : public IOFWAsyncCommand
{
    OSDeclareDefaultStructors(IOFWCompareAndSwapCommand)

protected:
    UInt32 					fInputVals[4];
    UInt32 					fOldVal[2];
	
	typedef struct 
	{ 
		IOMemoryDescriptor *	fMemory;
	} 
	MemberVariables;
	
	MemberVariables * fMembers;

    virtual void 	gotPacket(int rcode, const void* data, int size) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

public:
    // Compare to cmpVal, and if equal replace with newVal.
    // Size = 1 for 32 bit operation (one quad), 2 for 64 bit (two quads)
	virtual bool	initWithController(IOFireWireController *control) APPLE_KEXT_OVERRIDE;
    virtual bool	initAll(IOFireWireNub *device, FWAddress devAddress,
				const UInt32 *cmpVal, const UInt32 *newVal, int size,
				FWDeviceCallback completion, void *refcon, bool failOnReset);
    virtual bool	initAll(IOFireWireController *control,
                                UInt32 generation, FWAddress devAddress,
                                const UInt32 *cmpVal, const UInt32 *newVal, int size,
                                FWDeviceCallback completion, void *refcon);

    virtual IOReturn	reinit(FWAddress devAddress, const UInt32 *cmpVal, const UInt32 *newVal, int size,
                                FWDeviceCallback completion=NULL, void *refcon=NULL, bool failOnReset=false);
    virtual IOReturn	reinit(UInt32 generation, FWAddress devAddress,
                                const UInt32 *cmpVal, const UInt32 *newVal, int size,
                                FWDeviceCallback completion=NULL, void *refcon=NULL);

    // sets oldVal to the old value returned by the device, and
    // returns true if it was the expected value, ie. the lock succeeded
    virtual bool	locked(UInt32 *oldVal);

	virtual void free( void ) APPLE_KEXT_OVERRIDE;

protected:

	bool createMemberVariables( void );
	void destroyMemberVariables( void );
	
	void setInputVals( const UInt32 *	cmpVal, const UInt32 * newVal, int size );

	bool createMemoryDescriptor( void );
	void destroyMemoryDescriptor( void );
    
private:
    OSMetaClassDeclareReservedUnused(IOFWCompareAndSwapCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWCompareAndSwapCommand, 1);
    OSMetaClassDeclareReservedUnused(IOFWCompareAndSwapCommand, 2);
    OSMetaClassDeclareReservedUnused(IOFWCompareAndSwapCommand, 3);

};

/*
 * Send an async stream packet
 */

#pragma mark -

/*! @class IOFWAsyncStreamCommand
*/
class IOFWAsyncStreamCommand : public IOFWCommand
{
	// temporary for debugging:
	friend class IOFireWireUserClient ;

	OSDeclareDefaultStructors(IOFWAsyncStreamCommand)

protected:
    FWAsyncStreamCallback	fComplete;
    void *					fRefCon;
    IOMemoryDescriptor 		* fMemDesc;
    int						fSpeed;
    int						fSize;
    int						fCurRetries;
    int						fMaxRetries;
    int						fChannel;
    int						fSyncBits;
    int						fTag;
    UInt32					fGeneration;	// bus topology fNodeID is valid for.
    bool					fFailOnReset;

	typedef struct 
	{ 	} 
	MemberVariables;

    MemberVariables * fMembers;		
	
    virtual IOReturn	complete(
    							IOReturn 				status) APPLE_KEXT_OVERRIDE;
    							
	// To be called by IOFireWireController and derived classes.
    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

public:

    virtual bool		initAll(
    							IOFireWireController 	* control,
                                UInt32 					generation, 
                                UInt32 					channel,
                                UInt32 					sync,
                                UInt32 					tag,
                                IOMemoryDescriptor 		* hostMem,
                                UInt32					size,
                                int						speed,
                                FWAsyncStreamCallback	completion,
                                void 					* refcon);

	virtual void free( void ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	reinit(	UInt32 					generation, 
                                UInt32 					channel,
                                UInt32 					sync,
                                UInt32 					tag,
                                IOMemoryDescriptor 		* hostMem,
                                UInt32					size,
                                int						speed,
                               	FWAsyncStreamCallback	completion,
                                void 					* refcon);
								
    virtual void				gotAck(
    							int 					ackCode);
	// Utility for setting generation on newly created command
	virtual void				setGeneration(
								UInt32 					generation)
	{ fGeneration = generation; }

 
    // update nodeID/generation after bus reset, from the device object
    IOReturn		updateGeneration();
    
    bool		failOnReset() const
    { return fFailOnReset; }

private:
    OSMetaClassDeclareReservedUnused(IOFWAsyncStreamCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWAsyncStreamCommand, 1);

public:
    virtual bool		initAll(
    							IOFireWireController 	* control,
                                UInt32 					generation, 
                                UInt32 					channel,
                                UInt32 					sync,
                                UInt32 					tag,
                                IOMemoryDescriptor 		* hostMem,
                                UInt32					size,
                                int						speed,
                                FWAsyncStreamCallback	completion,
                                void 					* refcon,
								bool					failOnReset );
								
    
	virtual IOReturn	reinit(
								UInt32 					generation, 
                                UInt32 					channel,
                                UInt32 					sync,
                                UInt32 					tag,
                                IOMemoryDescriptor 		* hostMem,
                                UInt32					size,
                                int						speed,
                                FWAsyncStreamCallback 	completion,
                                void 					* refcon,
								bool					failOnReset);
	

};

/*
 * Send an async PHY packet
 */

#pragma mark -

/*! @class IOFWAsyncPHYCommand
*/
class IOFWAsyncPHYCommand : public IOFWCommand
{
	// temporary for debugging:
	friend class IOFireWireUserClient;

	OSDeclareDefaultStructors( IOFWAsyncPHYCommand )

protected:
	AsyncPendingTrans *		fTrans;
    FWAsyncPHYCallback		fComplete;
    void *					fRefCon;
    int						fCurRetries;
    int						fMaxRetries;
    UInt32					fGeneration;
    bool					fFailOnReset;
	UInt32					fData1;
	UInt32					fData2;
	int						fAckCode;
	UInt32					fResponseCode;
		
	typedef struct 
	{ 	} 
	MemberVariables;

    MemberVariables * fMembers;		
	
    virtual IOReturn	complete(
    							IOReturn 				status ) APPLE_KEXT_OVERRIDE;
    							
	// To be called by IOFireWireController and derived classes.
    virtual IOReturn	execute() APPLE_KEXT_OVERRIDE;

	void setResponseCode( UInt32 rcode );
	void setAckCode( int ack );

public:

    virtual bool		initAll(
    							IOFireWireController 	* control,
                                UInt32 					generation, 
                                UInt32					data1,
								UInt32					data2,
                                FWAsyncPHYCallback		completion,
                                void 					* refcon,
								bool 					failOnReset );
	virtual void free( void ) APPLE_KEXT_OVERRIDE;

    virtual IOReturn	reinit(	UInt32 					generation, 
								UInt32					data1,
								UInt32					data2,
                               	FWAsyncPHYCallback		completion,
                                void 					* refcon,
								bool 					failOnReset );

    virtual void				gotAck(
    							int 					ackCode );
								
	// Utility for setting generation on newly created command
	virtual void				setGeneration(
								UInt32 					generation )
			{ fGeneration = generation; }

 
    // update nodeID/generation after bus reset, from the device object
    IOReturn		updateGeneration();
    
    bool		failOnReset() const
		{ return fFailOnReset; }
    

	virtual void gotPacket( int rcode  );

	int getAckCode( void );
	UInt32 getResponseCode( void ) const;

	void setRetries( int retries);
private:
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 1);
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 2);
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 3);
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 4);
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 5);
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 6);
    OSMetaClassDeclareReservedUnused(IOFWAsyncPHYCommand, 7);
};

#endif /* _IOKIT_IOFWCOMMAND_H */
