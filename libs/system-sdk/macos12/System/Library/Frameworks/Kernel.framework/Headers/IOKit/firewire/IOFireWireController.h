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
 * Copyright (c) 1999-2002 Apple Computer, Inc.  All rights reserved.
 *
 * HISTORY
 *
 */

#ifndef _IOKIT_IOFIREWIRECONTROLLER_H
#define _IOKIT_IOFIREWIRECONTROLLER_H

#ifndef FIREWIREPRIVATE
#warning Please do not include this file. Include IOFireWireBus.h instead.
#endif

#include <IOKit/IOEventSource.h>
#include <IOKit/firewire/IOFireWireBus.h>
#include <IOKit/firewire/IOFireWireFamilyCommon.h>
#include <IOKit/firewire/IOFireWireIRMAllocation.h>
#include <IOKit/firewire/IOFWPHYPacketListener.h>
#include <IOKit/firewire/IOFireWireMultiIsochReceive.h>
#include <IOKit/IOMapper.h>

class OSData;
class IOWorkLoop;
class IOEventSource;
class IOFWQEventSource;
class IOTimerEventSource;
class IOMemoryDescriptor;
class IOFireWireController;
class IOFWAddressSpace;
class IOFWPseudoAddressSpace;
class IOFireWireNub;
class IOFireWireDevice;
class IOFireWireDeviceAux;
class IOFireWireUnit;
class IODCLProgram;
class IOLocalConfigDirectory;
class IOFireWireLink;
class IOFireWireSBP2ORB;
class IOFireWireSBP2Login;
class IOFireWireROMCache;
class IOFireWireLocalNode;
class IOFWWorkLoop;
class IOFireWireIRM;
class IOFireWirePowerManager;
class IOFWSimplePhysicalAddressSpace;
class IOFWSimpleContiguousPhysicalAddressSpace;
class IOFWAsyncStreamReceiver;
class IOFWAsyncStreamListener;
class IOFWUserVectorCommand;
class IOFWAsyncPHYCommand;
class IOFWPHYPacketListener;
class IOFWUserPHYPacketListener;
class IOFireWireUserClient;

const UInt32 kMaxWaitForValidSelfID = 20; // Still invalid SelfID after 20 retries

// Phy packet defs.

enum
{
	kFWPhyPacketID					= FWBitRange (0, 1),
	kFWPhyPacketIDPhase				= FWBitRangePhase (0, 1),
	
	kFWPhyPacketPhyID				= FWBitRange (2, 7),
	kFWPhyPacketPhyIDPhase			= FWBitRangePhase (2, 7)
};

enum
{
	kSelfIDPacketSize				= 8,
	kMaxSelfIDs						= 4	// SelfID 0,1,3,8
};

enum
{
	kFWConfigurationPacketID		= 0,
	kFWLinkOnPacketID				= 1,
	kFWSelfIDPacketID				= 2
};

enum
{
	kFWPhyConfigurationR					= FW_BIT(8),
	kFWPhyConfigurationT					= FW_BIT(9),
	kFWPhyConfigurationGapCnt				= FWBitRange (10, 15),
	kFWPhyConfigurationGapCntPhase			= FWBitRangePhase (10, 15)
};

enum
{
	kFWSelfIDPortStatusChild				= 3,
	kFWSelfIDPortStatusParent				= 2,
	kFWSelfIDPortStatusNotConnected			= 1,
	kFWSelfIDPortStatusNotPresent			= 0,
	
	kFWSelfIDNoPower						= 0,
	kFWSelfIDSelfPowered15W					= 1,
	kFWSelfIDSelfPowered30W					= 2,
	kFWSelfIDSelfPowered45W					= 3,
	kFWSelfIDBusPowered1W					= 4,
	kFWSelfIDBusPowered3W					= 5,
	kFWSelfIDBusPowered6W					= 6,
	kFWSelfIDBusPowered10W					= 7,
	
	kFWSelfIDPhyID							= kFWPhyPacketPhyID,//zzz do we need or want this?
	kFWSelfIDPhyIDPhase						= kFWPhyPacketPhyIDPhase,
	kFWSelfIDM								= FW_BIT(31),
	
	kFWSelfID0L								= FW_BIT(9),
	kFWSelfID0GapCnt						= FWBitRange (10, 15),
	kFWSelfID0GapCntPhase					= FWBitRangePhase (10, 15),
	kFWSelfID0SP							= FWBitRange (16, 17),
	kFWSelfID0SPPhase						= FWBitRangePhase (16, 17),
	kFWSelfID0Del							= FWBitRange (18, 19),
	kFWSelfID0DelPhase						= FWBitRangePhase (18, 19),
	kFWSelfID0C								= FW_BIT(20),
	kFWSelfID0Pwr							= FWBitRange (21, 23),
	kFWSelfID0PwrPhase						= FWBitRangePhase (21, 23),
	kFWSelfID0P0							= FWBitRange (24, 25),
	kFWSelfID0P0Phase						= FWBitRangePhase (24, 25),
	kFWSelfID0P1							= FWBitRange (26, 27),
	kFWSelfID0P1Phase						= FWBitRangePhase (26, 27),
	kFWSelfID0P2							= FWBitRange (28, 29),
	kFWSelfID0P2Phase						= FWBitRangePhase (28, 29),
	kFWSelfID0I								= FW_BIT(30),
	
	kFWSelfIDPacketType						= FW_BIT(8),
	kFWSelfIDNN								= FWBitRange (9, 11),
	kFWSelfIDNNPhase						= FWBitRangePhase (9, 11),
	kFWSelfIDNPa							= FWBitRange (14, 15),
	kFWSelfIDNPaPhase						= FWBitRangePhase (14, 15),
	kFWSelfIDNPb							= FWBitRange (16, 17),
	kFWSelfIDNPbPhase						= FWBitRangePhase (16, 17),
	kFWSelfIDNPc							= FWBitRange (18, 19),
	kFWSelfIDNPcPhase						= FWBitRangePhase (18, 19),
	kFWSelfIDNPd							= FWBitRange (20, 21),
	kFWSelfIDNPdPhase						= FWBitRangePhase (20, 21),
	kFWSelfIDNPe							= FWBitRange (22, 23),
	kFWSelfIDNPePhase						= FWBitRangePhase (22, 23),
	kFWSelfIDNPf							= FWBitRange (24, 25),
	kFWSelfIDNPfPhase						= FWBitRangePhase (24, 25),
	kFWSelfIDNPg							= FWBitRange (26, 27),
	kFWSelfIDNPgPhase						= FWBitRangePhase (26, 27),
	kFWSelfIDNPh							= FWBitRange (28, 29),
	kFWSelfIDNPhPhase						= FWBitRangePhase (28, 29),
	kFWSelfIDMore							= FW_BIT(31)
};

// Primary packet defs.
enum
{
	kFWPacketTCode							= FWBitRange (24, 27),
	kFWPacketTCodePhase						= FWBitRangePhase (24, 27)
};


enum
{
	kFWAsynchSpd							= FWBitRange (14, 15),
	kFWAsynchSpdPhase						= FWBitRangePhase (14, 15),
	
	kFWAsynchTLabel							= FWBitRange (16, 21),
	kFWAsynchTLabelPhase					= FWBitRangePhase (16, 21),
	kFWAsynchTTotal 						= ((0xffffffff & kFWAsynchTLabel) >> kFWAsynchTLabelPhase)+1,
	kFWAsynchRt								= FWBitRange (22, 23),
	kFWAsynchRtPhase						= FWBitRangePhase (22, 23),
	kFWAsynchNew							= 0,
	kFWAsynchRetryA							= 2,
	kTIAsycnhRetryB							= 3,
	
	kFWAsynchPriority						= FWBitRange (28, 31),
	kFWAsynchPriorityPhase					= FWBitRangePhase (28, 31),
	
	kFWAsynchDestinationID					= FWBitRange (0, 15),
	kFWAsynchDestinationIDPhase				= FWBitRangePhase (0, 15),
	
	kFWAsynchSourceID						= FWBitRange (0, 15),
	kFWAsynchSourceIDPhase					= FWBitRangePhase (0, 15),
	
	kFWAsynchDestinationOffsetHigh			= FWBitRange (16, 31),
	kFWAsynchDestinationOffsetHighPhase		= FWBitRangePhase (16, 31),
	
	kFWAsynchDestinationOffsetLow			= FWBitRange (0, 31),
	kFWAsynchDestinationOffsetLowPhase		= FWBitRangePhase (0, 31),
	
	kFWAsynchDataLength						= FWBitRange (0, 15),
	kFWAsynchDataLengthPhase				= FWBitRangePhase (0, 15),
	
	kFWAsynchExtendedTCode					= FWBitRange (16, 31),
	kFWAsynchExtendedTCodePhase				= FWBitRangePhase (16, 31),
	
	kFWAsynchAckSent						= FWBitRange (28, 31),
	kFWAsynchAckSentPhase					= FWBitRangePhase (28, 31),
	
	kFWAsynchRCode							= FWBitRange (16, 19),
	kFWAsynchRCodePhase						= FWBitRangePhase (16, 19)
};

enum
{
	kFWTCodeWriteQuadlet					= 0,
	kFWTCodeWriteBlock						= 1,
	kFWTCodeWriteResponse					= 2,
	kFWTCodeReadQuadlet						= 4,
	kFWTCodeReadBlock						= 5,
	kFWTCodeReadQuadletResponse				= 6,
	kFWTCodeReadBlockResponse				= 7,
	kFWTCodeCycleStart						= 8,
	kFWTCodeLock							= 9,
	kFWTCodeIsochronousBlock				= 10,
	kFWTCodeLockResponse					= 11,
	kFWTCodePHYPacket						= 14
};

enum
{
	kFWExtendedTCodeMaskSwap				= 1,
	kFWExtendedTCodeCompareSwap				= 2,
	kFWExtendedTCodeFetchAdd				= 3,
	kFWExtendedTCodeLittleAdd				= 4,
	kFWExtendedTCodeBoundedAdd				= 5,
	kFWExtendedTCodeWrapAdd					= 6,
	kFWExtendedTCodeVendorDependent			= 7
};

// debug boot-arg constants
enum
{
	kFWDebugIgnoreNodeNone					= 0xFFFFFFFF
};

struct AsyncPendingTrans {
	IOFWAsyncCommand *	fHandler;
	IOFWCommand *		fAltHandler;
	int			fTCode;
	bool		fInUse;
};

struct IOFWNodeScan {
	IOFireWireController 	*	fControl;
	FWAddress					fAddr;
	UInt32						fBuf[5];	// Enough for bus info block
	UInt32 					*	fSelfIDs;
	int							fNumSelfIDs;
	int							fROMSize;
	int							fRead;
	IOFWReadQuadCommand 	* 	fCmd;
	IOFWCompareAndSwapCommand * fLockCmd;
	UInt32						generation;
	UInt32						fIRMBitBucketOld;
	UInt32						fIRMBitBucketNew;
	bool						fIRMisBad;
	bool						speedChecking;
	bool						fContenderNeedsChecking;
	bool						fIRMCheckingRead;
	bool						fIRMCheckingLock;
	int							fRetriesBumped;
	bool						fMustNotBeRoot;
};


typedef struct IOFWDuplicateGUIDStruct IOFWDuplicateGUIDRec;
struct IOFWDuplicateGUIDStruct
{
	IOFWDuplicateGUIDRec		* 	fNextGUID;
	CSRNodeUniqueID				fGUID;
	UInt32						fLastGenSeen;
};


// IOFireWireDuplicateGUIDList
//
// A little class for keeping track of GUIDs which where we have observed 2 nodes with
// the same GUID

class IOFireWireDuplicateGUIDList : public OSObject
{
	OSDeclareDefaultStructors(IOFireWireDuplicateGUIDList);
	
private:
	IOFWDuplicateGUIDRec		* 	fFirstGUID;
	
protected:
	virtual void free(void) APPLE_KEXT_OVERRIDE;
	
public:
	
	static IOFireWireDuplicateGUIDList * create( void );
	
	void addDuplicateGUID( CSRNodeUniqueID guid, UInt32 gen );
	void removeDuplicateGUID( CSRNodeUniqueID guid );
	
	bool findDuplicateGUID( CSRNodeUniqueID guid, UInt32 gen );
	
};

#define kMaxPendingTransfers kFWAsynchTTotal

class IOFireWireController;

#pragma mark -

/*!
 @class IOFireWireControllerAux
 */

class IOFireWireControllerAux : public IOFireWireBusAux
{
	OSDeclareDefaultStructors(IOFireWireControllerAux)
	
	friend class IOFireWireController;
	
protected:
	
	IOFireWireController * 		fPrimary;
	
	UInt8						fMaxRec;
	
	UInt8						fPadding;
	UInt16						fPadding2;
	
	IOFWUserObjectExporter *	fSessionRefExporter;
	
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
	
	virtual bool 									init (
														  IOFireWireController * 	primary );
	virtual	void 									free (void) APPLE_KEXT_OVERRIDE;
	virtual IOFWDCLPool *							createDCLPool ( unsigned capacity ) const APPLE_KEXT_OVERRIDE;
	virtual UInt8									getMaxRec( void ) APPLE_KEXT_OVERRIDE;
	
	virtual UInt64 getFireWirePhysicalAddressMask( void ) APPLE_KEXT_OVERRIDE;
	virtual UInt32 getFireWirePhysicalAddressBits( void ) APPLE_KEXT_OVERRIDE;
	virtual UInt64 getFireWirePhysicalBufferMask( void ) APPLE_KEXT_OVERRIDE;
	virtual UInt32 getFireWirePhysicalBufferBits( void ) APPLE_KEXT_OVERRIDE;
	
	virtual IOFWSimpleContiguousPhysicalAddressSpace * createSimpleContiguousPhysicalAddressSpace( vm_size_t size, IODirection direction ) APPLE_KEXT_OVERRIDE;
	virtual IOFWSimplePhysicalAddressSpace * createSimplePhysicalAddressSpace( vm_size_t size, IODirection direction ) APPLE_KEXT_OVERRIDE;
	
	virtual IOFWUserObjectExporter * getSessionRefExporter( void ) APPLE_KEXT_OVERRIDE;
	
private:
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 0);
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 1);
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 2);
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 3);
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 4);
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 5);
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 6);
	OSMetaClassDeclareReservedUnused(IOFireWireControllerAux, 7);
	
};


#pragma mark -

/*! @class IOFireWireController
 */
class IOFireWireController : public IOFireWireBus
{
	OSDeclareAbstractStructors(IOFireWireController)
	
protected:
	enum busState {
		kStarting = 0,
		kAsleep,			// Link off, zzzzzz
		kWaitingBusReset,
		kWaitingSelfIDs,	// Bus has been reset, no selfIDs yet
		kWaitingScan,		// Got selfIDs, waiting a bit before hitting lame devices
		kScanning,			// Reading node ROMs
		kWaitingPrune,		// Read all ROMs, pausing before pruning missing devices
		kRunning,			// Normal happy state,
		kWaitingBusResetStart	// bus reset is desired, but not yet sent to the fwim
	};
	
	enum ResetState
	{
		kResetStateResetting,
		kResetStateDisabled,
		kResetStateArbitrated
	};
	
	enum
	{
		kDisablePhysicalAccess 	= (1 << 0)
	};
	
	struct timeoutQ: public IOFWCmdQ
	{
		IOTimerEventSource *fTimer;
		virtual void headChanged(IOFWCommand *oldHead);
		void busReset();
	};
	
	struct pendingQ: public IOFWCmdQ
	{
		IOFWQEventSource *fSource;
		virtual void headChanged(IOFWCommand *oldHead);
	};
	
	friend class IOFireWireLink;
	friend class IOFireWireDevice;
	friend class IOFireWireDeviceAux;
	friend class IOFWAddressSpace;
	friend class IOFWAddressSpaceAux;
	friend class IOFWPseudoAddressSpace;
	friend class IOFireWireSBP2ORB;
	friend class IOFireWireSBP2Login;
	friend class IOFWLocalIsochPort;
	friend class IOFWCommand;
	friend class IOFireWireUnit;
	friend class IOFireWirePCRSpace;
	friend class IOFireWireROMCache;
	friend class IOFWAsyncStreamCommand;
	friend class IOFireWireAVCLocalUnit;
	friend class IOFireWireAVCUnit;
	friend class IOFireWireAVCCommand;
	friend class IOFireWirePowerManager;
	friend class IOFWWriteQuadCommand;
	friend class IOFWWriteCommand;
	friend class IOFWCompareAndSwapCommand;
	friend class IOFWAsyncCommand;
	friend class IOFireWireAVCTargetSpace;
	friend class AppleFWOHCI;
	friend class IOFireWireNub;
	friend class IOFWAsyncStreamListener;
	friend class IOFireWireLocalNode;
	friend class IOFireWireIRMAllocation;
	friend class IOFWUserVectorCommand;
	friend class IOFWAsyncPHYCommand;
	friend class IOFWUserPHYPacketListener;
	friend class IOFWAsyncStreamReceiver;
	friend class IOFireWireUserClient;
	
	IOFireWireLink *			fFWIM;
	IOFWWorkLoop *				fWorkLoop;
	IOTimerEventSource *		fTimer;
	OSSet *						fLocalAddresses;	// Collection of local adress spaces
	OSIterator *				fSpaceIterator;		// Iterator over local addr spaces
	
	OSSet *						fAllocatedChannels;	// Need to be informed of bus resets
	OSIterator *				fAllocChannelIterator;	// Iterator over channels
	
	OSSet *						fIRMAllocations;	// Need to be informed of bus resets
	OSIterator *				fIRMAllocationsIterator;	// Iterator over channels
	OSSet *						fIRMAllocationsAllocated;	// Need to be informed of bus resets
	
	// Bus management variables (although we aren't a FireWire Bus Manager...)
	AbsoluteTime				fResetTime;		// Time of last reset
	UInt32						fBusGeneration;		// ID of current bus topology.
	UInt16						fLocalNodeID;		// ID of local node, ie. this computer
	UInt16						fRootNodeID;		// ID of root, ie. highest node id in use.
	UInt16						fIRMNodeID;		// ID of Isochronous resource manager, or kFWBadNodeID
	bool						fBusMgr;		// true if at least one node is bus manager capable
	IORegistryEntry *			fNodes[kFWMaxNodesPerBus];	// FireWire nodes on this bus
	UInt32 *					fNodeIDs[kFWMaxNodesPerBus+1];	// Pointer to SelfID list for each node
	// +1 so we know how many selfIDs the last node has
	
	UInt32						fGapCount;		// What we think the gap count should be
	//UInt8						fSpeedCodes[(kFWMaxNodesPerBus+1)*kFWMaxNodesPerBus];
	UInt8						fSpeedVector[((kFWMaxNodesPerBus+1)*kFWMaxNodesPerBus)/2];
	// Max speed between two nodes
	busState					fBusState;		// Which state are we in?
	int							fNumROMReads;		// Number of device ROMs we are still reading
	// SelfIDs
	int							fNumSelfIDs;		// Total number of SelfID packets
	UInt32						fSelfIDs[kMaxSelfIDs*kFWMaxNodesPerBus];
	
	// The local device's Config ROM
	UInt32						fROMHeader[5];		// More or less fixed header and bus info block
	IOLocalConfigDirectory *	fRootDir;		// Local Config ROM root directory.
	
	// log base 2 of maximum packet size the FWIM can send/receive
	// Normally calculated from bus info block.
	int	fMaxSendLog;
	int fMaxRecvLog;
	
	IOFWAddressSpace *			fROMAddrSpace;
	IOMemoryDescriptor *		fBadReadResponse;	// Send back easily identified bad data to out of range addrs.
	
	// Array for outstanding requests (up to 64)
	AsyncPendingTrans			fTrans[kMaxPendingTransfers];
	int							fLastTrans;
	
	// queue for executing commands that may timeout
	timeoutQ					fTimeoutQ;
	
	// queue for commands that can't execute yet
	pendingQ					fPendingQ;
	
	// queue for async commands interrupted by bus reset
	IOFWCmdQ					fAfterResetHandledQ;
	
	// Command to change bus state after a delay.
	IOFWDelayCommand *			fDelayedStateChangeCmd;
	bool						fDelayedStateChangeCmdNeedAbort;
	
	UInt32						fDelayedPhyPacket;
	bool						fBusResetScheduled;
	ResetState					fBusResetState;
	IOFWDelayCommand *			fBusResetStateChangeCmd;
	UInt32						fBusResetDisabledCount;
	
	void *						fFireLogPublisher;	// DEPRECATED
	
	OSData *					fAllocatedAddresses;
	
	UInt32						fDevicePruneDelay;
	
	IOFWPhysicalAccessMode		fPhysicalAccessMode;
	IOFWSecurityMode			fSecurityMode;
	IONotifier *				fKeyswitchNotifier;
	
	IOFireWireIRM *				fIRM;
	IOFireWirePowerManager *	fBusPowerManager;
	
	bool						fGapCountMismatch;
	
	bool						fUseHalfSizePackets;
	bool						fRequestedHalfSizePackets;
	
	IOFWNodeScan *					fScans[kFWMaxNodesPerBus];
	IOFireWireDuplicateGUIDList	*	fGUIDDups;
	
	bool						fDelegateCycleMaster;
	bool						fBadIRMsKnown;
	
	UInt32						fPreviousGap;
	
	UInt32						fOutOfTLabels;
	UInt32						fOutOfTLabels10S;
	UInt32						fOutOfTLabelsThreshold;
	
#ifdef LEGACY_SHUTDOWN
	IONotifier *				fPowerEventNotifier;
#endif
	
	bool						fStarted;
	
	UInt32						fIOCriticalSectionCount;
	UInt32						fHubPort;
	UInt32						fDebugIgnoreNode;
	
	OSSet *						fLocalAsyncStreamReceivers;
	OSIterator *				fAsyncStreamReceiverIterator;
	
	bool						fInstantiated;
	
	IOReturn					fStartStatus;
	UInt32						fWaitingForSelfID;
	
	UInt32						fForcedRootNodeID;
	bool						fNodeMustBeRootFlag;
	bool						fNodeMustNotBeRootFlag;
	
	UInt32						fForcedGapCount;
	bool						fForcedGapFlag;
	
	OSSet *						fPHYPacketListeners;
	OSIterator *				fPHYPacketListenersIterator;
	
	bool						fDSLimited;
	
	IONotifier *				fConsoleLockNotifier;
	IOFireWireLocalNode *       fLocalNode;
	
	/*! @struct ExpansionData
	 @discussion This structure will be used to expand the capablilties of the class in the future.
	 */
	struct ExpansionData { };
	
	/*! @var reserved
	 Reserved for future use.  (Internal use only)  */
	ExpansionData *reserved;
	
	static void clockTick(OSObject *, IOTimerEventSource *);
	static void readROMGlue(void *refcon, IOReturn status,
							IOFireWireNub *device, IOFWCommand *fwCmd);
	static void delayedStateChange(void *refcon, IOReturn status,
								   IOFireWireBus *bus, IOFWBusCommand *fwCmd);
	
	virtual void processBusReset();
	virtual void processSelfIDs(UInt32 *IDs, int numIDs, UInt32 *ownIDs, int numOwnIDs);
	virtual void processTimeout(IOTimerEventSource *src);
	virtual void processRcvPacket( UInt32 *data, int numQuads, IOFWSpeed speed );
	virtual void processWriteRequest(UInt16 sourceID, UInt32 tlabel,
									 UInt32 *hdr, void *buf, int len, IOFWSpeed speed);
	virtual void processLockRequest(UInt16 sourceID, UInt32 tlabel,
									UInt32 *hdr, void *buf, int len, IOFWSpeed speed);
	
	// Process read from a local address, return rcode
	virtual UInt32 doReadSpace(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
							   IOMemoryDescriptor **buf, IOByteCount * offset, IODMACommand **dma_command,
							   IOFWRequestRefCon refcon);
	
	// Process write to a local address, return rcode
	virtual UInt32 doWriteSpace(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
								const void *buf, IOFWRequestRefCon refcon);
	
	// Process lock to a local address, return rcode
	UInt32 doLockSpace(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 inlen,
					   const UInt32 *newVal, UInt32 &outLen, UInt32 *oldVal,
					   UInt32 extType, IOFWRequestRefCon refcon);
	
	virtual void updatePlane();
	virtual void startBusScan();
	
	// Called when all devices on bus have been examined
	virtual void finishedBusScan();
	
	virtual void buildTopology(bool doFWPlane);
	
	virtual void readDeviceROM(IOFWNodeScan *refCon, IOReturn status);
	
	virtual IOReturn UpdateROM();
	virtual IOReturn allocAddress(IOFWAddressSpace *space);
	virtual void freeAddress(IOFWAddressSpace *space);
	
	IOFireWireBusAux * createAuxiliary( void ) APPLE_KEXT_OVERRIDE;
	
public:
	
	// Initialization
	virtual bool init(IOFireWireLink *fwim);
	virtual void free(void) APPLE_KEXT_OVERRIDE;
	virtual bool start(IOService *provider) APPLE_KEXT_OVERRIDE;
	virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
	virtual bool finalize( IOOptionBits options ) APPLE_KEXT_OVERRIDE;
	virtual bool requestTerminate( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
	
	// Power management
	virtual IOReturn setPowerState ( unsigned long powerStateOrdinal, IOService* whatDevice ) APPLE_KEXT_OVERRIDE;
	
	// Implement IOService::getWorkLoop
	virtual IOWorkLoop *getWorkLoop() const APPLE_KEXT_OVERRIDE;
	
	// Allocate struct for tracking a transaction
	virtual AsyncPendingTrans *allocTrans(IOFWAsyncCommand *cmd=NULL);
	virtual void freeTrans(AsyncPendingTrans *trans);
	
	// Really public methods
	
	virtual IOReturn getCycleTime(UInt32 &cycleTime) APPLE_KEXT_OVERRIDE;
	virtual IOReturn getBusCycleTime(UInt32 &busTime, UInt32 &cycleTime) APPLE_KEXT_OVERRIDE;
	
	// Methods to manipulate the local Config ROM
	virtual IOReturn AddUnitDirectory(IOLocalConfigDirectory *unitDir) APPLE_KEXT_OVERRIDE;
	virtual IOReturn RemoveUnitDirectory(IOLocalConfigDirectory *unitDir) APPLE_KEXT_OVERRIDE;
	
	// Cause a bus reset
	virtual IOReturn resetBus(void) APPLE_KEXT_OVERRIDE;
	
	// Send async request packets
	virtual IOReturn asyncRead(	UInt32 				generation,
							   UInt16 				nodeID,
							   UInt16 				addrHi,
							   UInt32 				addrLo,
							   int 				speed,
							   int 				label,
							   int 				size,
							   IOFWAsyncCommand *	cmd );
	
	virtual IOReturn asyncWrite(	UInt32 					generation,
								UInt16 					nodeID,
								UInt16 					addrHi,
								UInt32 					addrLo,
								int 					speed,
								int 					label,
								IOMemoryDescriptor *	buf,
								IOByteCount 			offset,
								int 					size,
								IOFWAsyncCommand *		cmd );
	
	/* DEPRECATED */ virtual IOReturn asyncWrite(	UInt32 				generation,
												 /* DEPRECATED */								UInt16 				nodeID,
												 /* DEPRECATED */								UInt16 				addrHi,
												 /* DEPRECATED */								UInt32 				addrLo,
												 /* DEPRECATED */								int 				speed,
												 /* DEPRECATED */								int 				label,
												 /* DEPRECATED */								void *				data,
												 /* DEPRECATED */								int 				size,
												 /* DEPRECATED */								IOFWAsyncCommand *	cmd );
	
	/* DEPRECATED */ virtual IOReturn asyncLock(	UInt32 				generation,
												/* DEPRECATED */								UInt16 				nodeID,
												/* DEPRECATED */								UInt16 				addrHi,
												/* DEPRECATED */								UInt32 				addrLo,
												/* DEPRECATED */								int 				speed,
												/* DEPRECATED */								int 				label,
												/* DEPRECATED */								int 				type,
												/* DEPRECATED */								void *				data,
												/* DEPRECATED */								int 				size,
												/* DEPRECATED */								IOFWAsyncCommand *	cmd);
	
	
	// Send async read response packets
	// useful for pseudo address spaces that require servicing outside the FireWire work loop.
	virtual IOReturn asyncReadResponse(	UInt32 					generation,
									   UInt16 					nodeID,
									   int 					speed,
									   IOMemoryDescriptor *	buf,
									   IOByteCount 			offset,
									   int 					len,
									   IOFWRequestRefCon 		refcon );
	
	virtual IOReturn asyncLockResponse( UInt32 					generation,
									   UInt16 					nodeID,
									   int 					speed,
									   IOMemoryDescriptor *	buf,
									   IOByteCount 			offset,
									   int 					len,
									   IOFWRequestRefCon 		refcon );
	
	// Try to fix whatever might have caused the other device to not respond
	virtual IOReturn handleAsyncTimeout(IOFWAsyncCommand *cmd);
	
	// Convert a firewire nodeID into the IOFireWireDevice for it
	virtual IOFireWireDevice * nodeIDtoDevice(UInt32 generation, UInt16 nodeID) APPLE_KEXT_OVERRIDE;
	
	// Add/remove a channel from the list informed of bus resets
	virtual void addAllocatedChannel(IOFWIsochChannel *channel);
	virtual void removeAllocatedChannel(IOFWIsochChannel *channel);
	
	// Add/remove a IRM allocation from the list informed of bus resets
	virtual void addIRMAllocation(IOFireWireIRMAllocation *irmAllocation);
	virtual void removeIRMAllocation(IOFireWireIRMAllocation *irmAllocation);
	
	// Create an Isochronous Channel object
	// doIRM = true => allocate channel and bandwith in Isochronous Resource Manager
	// packetSize packet size (in bytes), used to calculate bandwidth needed.
	virtual IOFWIsochChannel *createIsochChannel(
												 bool doIRM, UInt32 packetSize, IOFWSpeed prefSpeed,
												 FWIsochChannelForceStopNotificationProc stopProc=NULL,
												 void *stopRefCon=NULL) APPLE_KEXT_OVERRIDE;
	
	// Create a local isochronous port to run the given DCL program
	// if task is 0, the DCL program is for the kernel task,
	// otherwise all DCL pointers are valid in the specified task.
	// opcodes is also pointer valid in the specified task.
	virtual IOFWLocalIsochPort *createLocalIsochPort(bool talking,
													 DCLCommand* opcodes, DCLTaskInfo *info = 0,
													 UInt32 startEvent = 0, UInt32 startState = 0, UInt32 startMask = 0) APPLE_KEXT_OVERRIDE;
	
	// Execute specified function on workloop after specified delay
	// Returned command is for delay, call it's cancel() function to cancel timeout.
	virtual IOFWDelayCommand * createDelayedCmd(UInt32 uSecDelay, FWBusCallback func, void *refcon) APPLE_KEXT_OVERRIDE;
	
	virtual IOFWPhysicalAddressSpace *createPhysicalAddressSpace(IOMemoryDescriptor *mem) APPLE_KEXT_OVERRIDE;
	virtual IOFWPseudoAddressSpace *createPseudoAddressSpace(FWAddress *addr, UInt32 len,
															 FWReadCallback reader, FWWriteCallback writer, void *refcon) APPLE_KEXT_OVERRIDE;
	
	// Extract info about the async request
	virtual bool isLockRequest(IOFWRequestRefCon refcon) APPLE_KEXT_OVERRIDE;
	virtual bool isQuadRequest(IOFWRequestRefCon refcon) APPLE_KEXT_OVERRIDE;
	virtual UInt32 getExtendedTCode(IOFWRequestRefCon refcon) APPLE_KEXT_OVERRIDE;
	
	// Inline accessors for protected member variables
	IOFWCmdQ &getTimeoutQ();
	IOFWCmdQ &getPendingQ();
	IOFWCmdQ &getAfterResetHandledQ();
	IOFireWireLink * getLink() const;
	
	IOLocalConfigDirectory *getRootDir() const;
	bool checkGeneration(UInt32 gen) const;
	UInt32 getGeneration() const;
	UInt16 getLocalNodeID() const;
	IOReturn getIRMNodeID(UInt32 &generation, UInt16 &id);
	
	const AbsoluteTime * getResetTime() const;
	
	IOFWSpeed FWSpeed(UInt16 nodeAddress) const;
	IOFWSpeed FWSpeed(UInt16 nodeA, UInt16 nodeB) const;
	
	// How big (as a power of two) can packets sent to/received from the node be?
	virtual int maxPackLog(bool forSend, UInt16 nodeAddress) const APPLE_KEXT_OVERRIDE;
	
	// How big (as a power of two) can packets sent from A to B be?
	virtual int maxPackLog(UInt16 nodeA, UInt16 nodeB) const APPLE_KEXT_OVERRIDE;
	
	// Force given node to be root (via root holdoff Phy packet)
	virtual IOReturn makeRoot(UInt32 generation, UInt16 nodeID) APPLE_KEXT_OVERRIDE;
	
	virtual IOFWPseudoAddressSpace *createInitialAddressSpace(UInt32 addressLo, UInt32 len,
															  FWReadCallback reader, FWWriteCallback writer, void *refcon) APPLE_KEXT_OVERRIDE;
	
	virtual IOFWAddressSpace *getAddressSpace(FWAddress address) APPLE_KEXT_OVERRIDE;
	
	// Extract info about the async request - was the request ack'ed complete already?
	virtual bool isCompleteRequest(IOFWRequestRefCon refcon) APPLE_KEXT_OVERRIDE;
	
	// Are we currently scanning the bus?
	bool scanningBus() const;
	
protected:
	
	void openGate();
	void closeGate();
	
protected:
	virtual void doBusReset( void );
	static void resetStateChange( void *refcon, IOReturn status,
								 IOFireWireBus *bus, IOFWBusCommand *fwCmd);
	
public:
	virtual IOReturn disableSoftwareBusResets( void );
	virtual void enableSoftwareBusResets( void );
	
	virtual IOFWAsyncStreamCommand * createAsyncStreamCommand( UInt32 generation,
															  UInt32 channel, UInt32 sync, UInt32 tag, IOMemoryDescriptor *hostMem,
															  UInt32 size, int speed,FWAsyncStreamCallback completion, void *refcon) APPLE_KEXT_OVERRIDE;
	
	virtual	IOReturn asyncStreamWrite(UInt32 generation,
									  int speed, int tag, int sync, int channel,
									  IOMemoryDescriptor *buf, IOByteCount offset,
									  int size, IOFWAsyncStreamCommand *cmd);
	
protected:
	bool inGate();
	
	virtual IOReturn allocatePseudoAddress(FWAddress *addr, UInt32 lenDummy);
	virtual void freePseudoAddress(FWAddress addr, UInt32 lenDummy);
	
	virtual IORegistryEntry * createDummyRegistryEntry( IOFWNodeScan *scan );
	
	static IOFireWireLocalNode * getLocalNode(IOFireWireController *control);
	
	virtual void setPhysicalAccessMode( IOFWPhysicalAccessMode mode );
	virtual IOFWPhysicalAccessMode getPhysicalAccessMode( void );
	virtual void physicalAccessProcessBusReset( void );
	virtual void setNodeIDPhysicalFilter( UInt16 nodeID, bool state );
	
	virtual void initSecurity( void );
	virtual void freeSecurity( void );
	static bool serverKeyswitchCallback( void * target, void * refCon, IOService * service, IONotifier * notifier );
	virtual void setSecurityMode( IOFWSecurityMode mode );
	virtual IOFWSecurityMode getSecurityMode( void );
	
	virtual IOReturn createTimeoutQ( void );
	virtual void destroyTimeoutQ( void );
	virtual IOReturn createPendingQ( void );
	virtual void destroyPendingQ( void );
	
	virtual UInt32 countNodeIDChildren( UInt16 nodeID, int hub_port = 0, int * hubChildRemainder = NULL, bool * hubParentFlag = NULL );
	
public:
	virtual UInt32 hopCount(UInt16 nodeAAddress, UInt16 nodeBAddress ) APPLE_KEXT_OVERRIDE;
	virtual UInt32 hopCount(UInt16 nodeAAddress ) APPLE_KEXT_OVERRIDE;
	
	virtual IOFireWirePowerManager * getBusPowerManager( void ) APPLE_KEXT_OVERRIDE;
	
protected:
	virtual void handleARxReqIntComplete();
	
	virtual IOReturn asyncLock(	UInt32 					generation,
							   UInt16 					nodeID,
							   UInt16 					addrHi,
							   UInt32 					addrLo,
							   int 					speed,
							   int 					label,
							   int 					type,
							   IOMemoryDescriptor *	buf,
							   IOByteCount 			offset,
							   int 					size,
							   IOFWAsyncCommand *		cmd );
	
	virtual IOReturn asyncWrite(	UInt32 					generation,
								UInt16 					nodeID,
								UInt16 					addrHi,
								UInt32 					addrLo,
								int 					speed,
								int 					label,
								IOMemoryDescriptor *	buf,
								IOByteCount 			offset,
								int 					size,
								IOFWAsyncCommand *		cmd,
								IOFWWriteFlags 			flags );
	
protected:
	bool delayedStateCommandInUse() const;
	void enterBusResetDisabledState( );
	
	virtual UInt32 getPortNumberFromIndex( UInt16 index );
	
	virtual bool checkForDuplicateGUID(IOFWNodeScan *scan, CSRNodeUniqueID *currentGUIDs );
	virtual void updateDevice(IOFWNodeScan *scan );
	virtual bool AssignCycleMaster();
	
public:
	
	IOReturn clipMaxRec2K(Boolean clipMaxRec );
	void setNodeSpeed( UInt16 nodeAddress, IOFWSpeed speed );
	void useHalfSizePackets( void );
	void disablePhyPortOnSleepForNodeID( UInt32 nodeID );
	
	IOReturn handleAsyncCompletion( IOFWCommand *cmd, IOReturn status );
	void processCycle64Int();
	
#ifdef LEGACY_SHUTDOWN
	static IOReturn systemShutDownHandler( void * target, void * refCon,
										  UInt32 messageType, IOService * service,
										  void * messageArgument, vm_size_t argSize );
#else
	virtual void systemWillShutdown( IOOptionBits specifier ) APPLE_KEXT_OVERRIDE;
#endif
	
	IOReturn beginIOCriticalSection( void );
	void endIOCriticalSection( void );
	
protected:
	IOReturn poweredStart( void );
	void setNodeSpeed( UInt16 nodeA, UInt16 nodeB, UInt8 speed );
	void setNodeSpeed( UInt16 nodeAddress, UInt8 speed );
	
public:
	bool isPhysicalAccessEnabledForNodeID( UInt16 nodeID );
	
	// Allocate IRM bandwidth if the specified generation is the current FireWire generation.
	IOReturn allocateIRMBandwidthInGeneration(UInt32 bandwidthUnits, UInt32 generation) ;
	
	// Release IRM bandwidth if the specified generation is the current FireWire generation.
	IOReturn releaseIRMBandwidthInGeneration(UInt32 bandwidthUnits, UInt32 generation) ;
	
	// Allocate IRM channel if the specified generation is the current FireWire generation.
	IOReturn allocateIRMChannelInGeneration(UInt8 isochChannel, UInt32 generation) ;
	
	// Release IRM channel if the specified generation is the current FireWire generation.
	IOReturn releaseIRMChannelInGeneration(UInt8 isochChannel, UInt32 generation) ;
	
	// Create an IOFireWireIRMAllocation object which can be used to allocate isoch resources that are automatically reallocated after bus-resets!
	IOFireWireIRMAllocation *createIRMAllocation(Boolean releaseIRMResourcesOnFree = true,
												 IOFireWireIRMAllocation::AllocationLostNotificationProc allocationLostProc = NULL,
												 void *pLostNotificationProcRefCon = NULL);
	
	IOFWAsyncStreamListener *createAsyncStreamListener( UInt32	channel, FWAsyncStreamReceiveCallback proc, void *refcon );
	
	void removeAsyncStreamListener( IOFWAsyncStreamListener *listener );
	
	IOFWSpeed getBroadcastSpeed(){ return FWSpeed( fLocalNodeID ); };
	
private:
	
	IOFWAsyncStreamReceiver *allocAsyncStreamReceiver( UInt32 channel, FWAsyncStreamReceiveCallback proc, void *refcon );
	
	IOFWAsyncStreamReceiver *getAsyncStreamReceiver( UInt32 channel );
	
	void freeAllAsyncStreamReceiver();
	
	void activateAsyncStreamReceivers();
	
	void deactivateAsyncStreamReceivers();
	
protected:
	IOService *findKeyswitchDevice( void );
	void suspendBus( void );
	
public:
	virtual IOReturn			asyncRead(	UInt32 				generation,
										  UInt16 				nodeID,
										  UInt16 				addrHi,
										  UInt32 				addrLo,
										  int 				speed,
										  int 				label,
										  int 				size,
										  IOFWAsyncCommand *	cmd,
										  IOFWReadFlags		flags );
	
	void checkProgress( void );
	
	void terminateDevice( IOFireWireDevice * device );
	
	void nodeMustBeRoot( UInt32 nodeID );
	
	void nodeMustNotBeRoot( UInt32 nodeID );
	
	void setGapCount( UInt32	gapCount );
	
	IOReturn asyncPHYPacket(	UInt32					generation,
							UInt32					data,
							UInt32					data2,
							IOFWAsyncPHYCommand *	cmd );
	
	IOFWAsyncPHYCommand * createAsyncPHYCommand(	UInt32				generation,
												UInt32				data1,
												UInt32				data2,
												FWAsyncPHYCallback	completion,
												void *				refcon,
												bool				failOnReset );
	
private:
	AsyncPendingTrans * allocTrans( IOFWAsyncCommand * cmd, IOFWCommand * altcmd );
	
public:
	
	IOReturn activatePHYPacketListener( IOFWPHYPacketListener * listener );
	void deactivatePHYPacketListener( IOFWPHYPacketListener * listener );
	
	IOFWPHYPacketListener * createPHYPacketListener( FWPHYPacketCallback proc, void * refcon );
	
private:
	void processPHYPacket( UInt32 data1, UInt32 data2 );
	void enterLoggingMode( void );
	
public:
	IOReturn getCycleTimeAndUpTime( UInt32 &cycleTime, UInt64 &uptime );
	
protected:
	void removeAsyncStreamReceiver( IOFWAsyncStreamReceiver *receiver );
	
public:
	
	// Create a multi-isoch-receive listener
	IOFireWireMultiIsochReceiveListener * createMultiIsochReceiveListener(UInt32 channel,
																		  FWMultiIsochReceiveListenerCallback callback,
																		  void *pCallbackRefCon,
																		  FWMultiIsochReceiveListenerParams *pListenerParams = NULL);
	
	// Activate a multi-isoch-receive listener
	IOReturn activateMultiIsochReceiveListener(IOFireWireMultiIsochReceiveListener *pListener);
	
	// Deactivate a multi-isoch-receive listener
	IOReturn deactivateMultiIsochReceiveListener(IOFireWireMultiIsochReceiveListener *pListener);
	
	// Call for client to specify he is done with a multi-isoch receiver isoch packet
	void clientDoneWithMultiIsochReceivePacket(IOFireWireMultiIsochReceivePacket *pPacket);
	
public:
	virtual IOFWAsyncStreamCommand * createAsyncStreamCommand( UInt32 generation,
															  UInt32 channel, UInt32 sync, UInt32 tag, IOMemoryDescriptor *hostMem,
															  UInt32 size, int speed,FWAsyncStreamCallback completion, void *refcon, bool	failOnReset) APPLE_KEXT_OVERRIDE;
	
private:
	void addToIRMAllocationSet(IOFireWireIRMAllocation *anObject);
	void removeFromIRMAllocationSet(IOFireWireIRMAllocation *anObject);
	
	static IOReturn consoleLockInterestHandler( void * target, void * refCon,
											   UInt32 messageType, IOService * provider,
											   void * messageArgument, vm_size_t argSize );
	
public:
	
	IOMapper * copyMapper( void );
	
protected:
	OSMetaClassDeclareReservedUnused(IOFireWireController, 0);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 1);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 2);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 3);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 4);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 5);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 6);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 7);
	OSMetaClassDeclareReservedUnused(IOFireWireController, 8);
	
};

#endif /* ! _IOKIT_IOFIREWIRECONTROLLER_H */



