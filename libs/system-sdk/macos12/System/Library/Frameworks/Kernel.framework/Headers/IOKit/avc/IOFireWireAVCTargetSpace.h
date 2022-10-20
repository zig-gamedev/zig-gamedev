/*
 * Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
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
 *	IOFireWireAVCTargetSpace.h
 *
 * Class to centralize the AVC Target mode support
 *
 */

#ifndef _IOKIT_IOFIREWIREAVCTARGETSPACE_H
#define _IOKIT_IOFIREWIREAVCTARGETSPACE_H

#include <IOKit/IOUserClient.h>
#include <IOKit/IOBufferMemoryDescriptor.h>

#include <IOKit/firewire/IOFWAddressSpace.h>
#include <IOKit/firewire/IOLocalConfigDirectory.h>
#include <IOKit/avc/IOFireWireAVCConsts.h>
#include <IOKit/avc/IOFireWireAVCUserClientCommon.h>

class IOFireWireAVCProtocolUserClient;
class AVCCommandHandlerInfo;
class AVCSubunitInfo;

typedef void (*IOFireWireAVCTargetCommandHandlerCallback)(const AVCCommandHandlerInfo *pCmdInfo,
														  UInt32 generation,
														  UInt16 nodeID,
														  const void *command,
														  UInt32 cmdLen,
														  IOFWSpeed &speed,
														  UInt32 handlerSearchIndex);

typedef void (*IOFireWireAVCSubunitPlugHandlerCallback)(const AVCSubunitInfo *pSubunitInfo,
														IOFWAVCSubunitPlugMessages plugMessage,
														IOFWAVCPlugTypes plugType,
														UInt32 plugNum,
														UInt32 messageParams,
														UInt32 generation,
														UInt16 nodeID);

/*!
@class AVCCommandHandlerInfo
@abstract internal class to manage installed command handlers
*/
class AVCCommandHandlerInfo : public OSObject
{
    OSDeclareDefaultStructors(AVCCommandHandlerInfo)
public:
	IOFireWireAVCProtocolUserClient * userClient;
	IOFireWireAVCTargetCommandHandlerCallback callBack;
	OSAsyncReference64 asyncRef;
	UInt32 subUnitTypeAndID;
	UInt32 opCode;
	uint64_t userCallBack;
	uint64_t userRefCon;
};

typedef struct _AVCSubunitPlugRecord
{
	UInt32 plugSignalFormat;
	UInt32 connectionCount;
}AVCSubunitPlugRecord;

/*!
@class AVCSubunitInfo
@abstract internal class to manage installed subunits
*/
class AVCSubunitInfo : public OSObject
{
    OSDeclareDefaultStructors(AVCSubunitInfo)
    bool init();
    void free();
public:
	static AVCSubunitInfo *create();
	IOFireWireAVCProtocolUserClient * userClient;
	IOFireWireAVCSubunitPlugHandlerCallback callBack;
	OSAsyncReference64 asyncRef;
	UInt32 subunitTypeAndID;
	UInt32 numSourcePlugs;
	UInt32 numDestPlugs;
	uint64_t userCallBack;
	uint64_t userRefCon;
	AVCSubunitPlugRecord *sourcePlugRecords;
	AVCSubunitPlugRecord *destPlugRecords;
};

typedef struct _AVCUnitPlugRecord
{
	UInt32 connectionCount;
}AVCUnitPlugRecord;

typedef struct _AVCUnitPlugs
{
	UInt32 numIsochInPlugs;
	UInt32 numIsochOutPlugs;
	UInt32 numExternalInPlugs;
	UInt32 numExternalOutPlugs;
	AVCUnitPlugRecord isochInPlugRecord[kAVCMaxNumPlugs];
	AVCUnitPlugRecord isochOutPlugRecord[kAVCMaxNumPlugs];
	AVCUnitPlugRecord externalInPlugRecord[kAVCMaxNumPlugs];
	AVCUnitPlugRecord externalOutPlugRecord[kAVCMaxNumPlugs];
}AVCUnitPlugs;

/*!
@class UCInfo
@abstract internal class to manage multiple protocol user clients
*/
class UCInfo : public OSObject
{
    OSDeclareDefaultStructors(UCInfo)
public:
    IOFireWireAVCProtocolUserClient *fUserClient;
};

/*!
@class AVCConnectionRecord
@abstract internal class to manage AVC connections
*/
class AVCConnectionRecord : public OSObject
{
    OSDeclareDefaultStructors(AVCConnectionRecord)
public:
	UInt32 sourceSubunitTypeAndID;
	IOFWAVCPlugTypes sourcePlugType;
	UInt32 sourcePlugNum;
	UInt32 destSubunitTypeAndID;
	IOFWAVCPlugTypes destPlugType;
	UInt32 destPlugNum;
	bool lockConnection;
	bool permConnection;
};

/*!
@class IOFireWireAVCTargetSpace
@abstract object to centralize the AVC Target mode support
*/
class IOFireWireAVCTargetSpace : public IOFWPseudoAddressSpace
{
    OSDeclareDefaultStructors(IOFireWireAVCTargetSpace)

protected:
    UInt32 fBuf[512];
    UInt32 fActivations;
	IOFireWireController *fController;
	IOLocalConfigDirectory * fAVCLocalConfigDirectory;
    OSArray * fUserClients;
	OSArray * fCommandHandlers;
	OSArray * fSubunits;
	OSArray * fConnectionRecords;
	AVCUnitPlugs fUnitPlugs;
	IORecursiveLock * fLock;

	/*! @struct ExpansionData
	@discussion This structure will be used to expand the capablilties of the class in the future.
	*/
    struct ExpansionData { };

	/*! @var reserved
	Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

    virtual UInt32 doWrite(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
                           const void *buf, IOFWRequestRefCon refcon);


	IOReturn targetSendAVCResponse(UInt32 generation, UInt16 nodeID, IOBufferMemoryDescriptor *pBufMemDesc, UInt32 size);

	// Internal AVC Target Command Handlers
	IOReturn handleUnitInfoCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handleSubUnitInfoCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handlePlugInfoCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handlePowerCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handleConnectCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handleDisconnectCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handleInputPlugSignalFormatCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handleOutputPlugSignalFormatCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handleConnectionsCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);
	IOReturn handleSignalSourceCommand(UInt16 nodeID, UInt32 generation, const char *buf, UInt32 len);

	UInt32 subUnitOfTypeCount(UInt32 type);
	AVCSubunitInfo *getSubunitInfo(UInt32 subunitTypeAndID);
	bool canConnectDestPlug(UInt32 destSubunitTypeAndID,
						 IOFWAVCPlugTypes destPlugType,
						 UInt32 *destPlugNum);
	
public:

	// Activate/Deactivate Functions
	virtual IOReturn activateWithUserClient(IOFireWireAVCProtocolUserClient *userClient);
    virtual void deactivateWithUserClient(IOFireWireAVCProtocolUserClient *userClient);

	/*!
	@function init
	@abstract initializes the IOFireWireAVCTargetSpace object
	*/
    virtual bool init(IOFireWireController *controller);

	/*!
	@function getAVCTargetSpace
	@abstract returns the IOFireWireAVCTargetSpace object for the given FireWire bus
	@param bus The FireWire bus
	*/
    static IOFireWireAVCTargetSpace *getAVCTargetSpace(IOFireWireController *controller);

	/*!
	@function publishAVCUnitDirectory
	@abstract Creates a local AVC Unit directory if it doesn't already exist
	*/
    virtual IOReturn publishAVCUnitDirectory(void);

	virtual IOReturn installAVCCommandHandler(IOFireWireAVCProtocolUserClient *userClient,
										   IOFireWireAVCTargetCommandHandlerCallback callBack,
										   OSAsyncReference64 asyncRef,
										   UInt32 subUnitTypeAndID,
										   UInt32 opCode,
										   uint64_t userCallBack,
										   uint64_t userRefCon);

	virtual IOReturn addSubunit(IOFireWireAVCProtocolUserClient *userClient,
							 IOFireWireAVCSubunitPlugHandlerCallback callBack,
							 OSAsyncReference64 asyncRef,
							 UInt32 subunitType,
							 UInt32 numSourcePlugs,
							 UInt32 numDestPlugs,
							 uint64_t userCallBack,
							 uint64_t userRefCon,
							 UInt32 *subUnitID);

	virtual IOReturn setSubunitPlugSignalFormat(IOFireWireAVCProtocolUserClient *userClient,
											 UInt32 subunitTypeAndID,
											 IOFWAVCPlugTypes plugType,
											 UInt32 plugNum,
											 UInt32 signalFormat);

	virtual IOReturn getSubunitPlugSignalFormat(IOFireWireAVCProtocolUserClient *userClient,
											 UInt32 subunitTypeAndID,
											 IOFWAVCPlugTypes plugType,
											 UInt32 plugNum,
											 UInt32 *pSignalFormat);

	virtual IOReturn connectTargetPlugs(IOFireWireAVCProtocolUserClient *userClient,
									 AVCConnectTargetPlugsInParams *inParams,
									 AVCConnectTargetPlugsOutParams *outParams);

	virtual IOReturn disconnectTargetPlugs(IOFireWireAVCProtocolUserClient *userClient,
										UInt32 sourceSubunitTypeAndID,
										IOFWAVCPlugTypes sourcePlugType,
										UInt32 sourcePlugNum,
										UInt32 destSubunitTypeAndID,
										IOFWAVCPlugTypes destPlugType,
										UInt32 destPlugNum);

	virtual IOReturn getTargetPlugConnection(IOFireWireAVCProtocolUserClient *userClient,
										  AVCGetTargetPlugConnectionInParams *inParams,
										  AVCGetTargetPlugConnectionOutParams *outParams);

    virtual IOReturn findAVCRequestHandler(IOFireWireAVCProtocolUserClient *userClient,
										  UInt32 generation,
										  UInt16 nodeID,
										  IOFWSpeed speed,
										  UInt32 handlerSearchIndex,
										  const char *pCmdBuf,
										  UInt32 cmdLen);
	
	virtual void pcrModified(IOFWAVCPlugTypes plugType,
						  UInt32 plugNum,
						  UInt32 newValue);
	
private:
	OSMetaClassDeclareReservedUnused(IOFireWireAVCTargetSpace, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCTargetSpace, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCTargetSpace, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCTargetSpace, 3);
};

#endif /*_IOKIT_IOFIREWIREAVCTARGETSPACE_H */