/*
 * Copyright (c) 1998-2014 Apple Computer, Inc. All rights reserved.
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

#ifndef _IOKIT_IOAUDIOENGINEUSERCLIENT_H
#define _IOKIT_IOAUDIOENGINEUSERCLIENT_H

#include <IOKit/IOUserClient.h>
#include <AvailabilityMacros.h>

#ifndef IOAUDIOFAMILY_SELF_BUILD
#include <IOKit/audio/IOAudioEngine.h>
#include <IOKit/audio/IOAudioTypes.h>
#else
#include "IOAudioEngine.h"
#include "IOAudioTypes.h"
#endif
#include <IOKit/IOBufferMemoryDescriptor.h>

class IOAudioEngine;
class IOAudioStream;
class IOMemoryDescriptor;
class IOCommandGate;
class IOWorkLoop;

class IOAudioEngineUserClient;
class IOAudioClientBufferSet;
struct IOAudioFormatNotification;


#define kDriverHelper_DriverHostEntitlement	"com.apple.private.audio.driver-host"		// XXX copied from AudioHAL/DriverHelper/DriverHelper.h

typedef struct IOAudioClientBuffer
{
    IOAudioEngineUserClient		*userClient;
    IOAudioStream 				*audioStream;
    void						*sourceBuffer;
    IOMemoryDescriptor			*sourceBufferDescriptor;
    IOMemoryMap					*sourceBufferMap;
    void						*unmappedSourceBuffer; 
    UInt32						numSampleFrames;
    UInt32						numChannels;
    IOAudioEnginePosition		mixedPosition;
    struct IOAudioClientBuffer	*mNextBuffer32;
    struct IOAudioClientBuffer	*nextClip;
    struct IOAudioClientBuffer	*previousClip;
    struct IOAudioClientBuffer	*nextClient;
	IOAudioBufferDataDescriptor *bufferDataDescriptor;
} IOAudioClientBuffer;

/* IOAudioClientBuffer64 added for binary compatibility with old PPC drivers covered by <rdar://problem/4651809> */
typedef struct IOAudioClientBuffer64 
{
    IOAudioClientBuffer				mAudioClientBuffer32;
    mach_vm_address_t				mUnmappedSourceBuffer64;
    struct IOAudioClientBuffer64	*mNextBuffer64;
} IOAudioClientBuffer64;

typedef struct IOAudioClientBufferExtendedInfo
{
	// Added stuff for registerClientParameterBuffer
	UInt32						bufferSetID;
	void						*paramBuffer;
	IOMemoryDescriptor			*paramBufferDescriptor;
	IOMemoryMap					*paramBufferMap;
	void						*unmappedParamBuffer;
	struct IOAudioClientBufferExtendedInfo	*mNextExtended;
} IOAudioClientBufferExtendedInfo;

/* IOAudioClientBufferExtendedInfo64 added for binary compatibility with old PPC drivers covered by <rdar://problem/4651809>  */
typedef struct IOAudioClientBufferExtendedInfo64 
{
	IOAudioClientBufferExtendedInfo mAudioClientBufferExtended32;
	mach_vm_address_t			mUnmappedParamBuffer64;
	struct IOAudioClientBufferExtendedInfo64	*mNextExtended64;
}	IOAudioClientBufferExtendedInfo64;


class IOAudioEngineUserClient : public IOUserClient
{
	OSDeclareDefaultStructors(IOAudioEngineUserClient)
	
	friend class IOAudioEngine;
	friend class IOAudioClientBufferSet;
	friend class IOAudioStream;
	
protected:
	IOAudioEngine				*audioEngine;
	
	IOWorkLoop					*workLoop;
	IOCommandGate				*commandGate;
	
	IOExternalMethod			old_methods[5];		// It's size can't be changed for binary compatibility reasons, no longer used.
	IOExternalTrap				trap;
	
	task_t						clientTask;
	UInt32						numSampleFrames;		// Never used...
	
	IOAudioClientBufferSet		*clientBufferSetList;
	IORecursiveLock 			*clientBufferLock;
	
	IOAudioNotificationMessage	*notificationMessage;
	
	bool						online;
	
protected:
	struct ExpansionData {
		IOAudioClientBufferExtendedInfo64	*extendedInfo;
		UInt32								classicMode;                            // System 9! - no longer need
		UInt32								commandGateStatus;						// <rdar://8518215>
		SInt32								commandGateUsage;						// <rdar://8518215>
		UInt32								connectionID;
	};

// <rdar://101000004> START
	ExpansionData *reserved;
	
public:
	virtual IOReturn externalMethod( uint32_t selector, IOExternalMethodArguments * arguments, 	IOExternalMethodDispatch * dispatch, 
									OSObject * target, 	void * reference) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// New code added here...
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 0);	
	virtual IOReturn registerClientParameterBuffer (void * parameterBuffer, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;  // unused function
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 1);	
	virtual IOAudioClientBufferExtendedInfo * findExtendedInfo(UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 2);	
	virtual IOReturn getNearestStartTime(IOAudioStream *audioStream, IOAudioTimeStamp *ioTimeStamp, UInt32 isInput) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 3);	
	virtual IOReturn getClientNearestStartTime(IOAudioStream *audioStream, IOAudioTimeStamp *ioTimeStamp, UInt32 isInput) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 4);	
	virtual IOReturn safeRegisterClientBuffer(UInt32 audioStreamIndex, void * sourceBuffer, UInt32 bufSizeInBytes, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 5);	
	virtual bool initWithAudioEngine(IOAudioEngine *engine, task_t task, void *securityToken, UInt32 type, OSDictionary *properties) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 6);	<rdar://problems/5321701>
	virtual IOReturn safeRegisterClientBuffer64(UInt32 audioStreamIndex, mach_vm_address_t * sourceBuffer, UInt32 bufSizeInBytes, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 7);	<rdar://problems/5321701>
	virtual IOReturn registerClientBuffer64(IOAudioStream *audioStream, mach_vm_address_t sourceBuffer, UInt32 bufSizeInBytes, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 8);	<rdar://problems/5321701>
	virtual IOReturn registerBuffer64(IOAudioStream *audioStream, mach_vm_address_t sourceBuffer, UInt32 bufSizeInBytes, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 9);	<rdar://problems/5321701>
	virtual IOReturn unregisterBuffer64(mach_vm_address_t sourceBuffer, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 10);	<rdar://problems/5321701>
	virtual IOReturn unregisterClientBuffer64(mach_vm_address_t  * sourceBuffer, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 11);	 <rdar://problems/5321701>
	virtual IOAudioClientBufferExtendedInfo64 * findExtendedInfo64(UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	
	
private:
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 0);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 1);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 2);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 3);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 4);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 5);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 6);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 7);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 8);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 9);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 10);
	OSMetaClassDeclareReservedUsed(IOAudioEngineUserClient, 11);
	
	
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 12);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 13);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 14);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 15);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 16);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 17);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 18);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 19);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 20);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 21);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 22);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 23);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 24);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 25);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 26);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 27);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 28);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 29);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 30);
	OSMetaClassDeclareReservedUnused(IOAudioEngineUserClient, 31);
	
protected:
	virtual IOReturn clientClose() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn clientDied() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	static IOReturn _closeClientAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	static IOReturn closeClientAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn closeClient() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	
	virtual IOReturn clientMemoryForType(UInt32 type, UInt32 *flags, IOMemoryDescriptor **memory) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOExternalMethod *getExternalMethodForIndex(UInt32 index) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOExternalTrap *getExternalTrapForIndex(UInt32 index) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn registerNotificationPort(mach_port_t port, UInt32 type, UInt32 refCon) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	static IOReturn _registerNotificationAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	static IOReturn registerNotificationAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn registerNotification(mach_port_t port, UInt32 refCon) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

	virtual void setOnline(bool newOnline) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOReturn performClientOutput(UInt32 firstSampleFrame, UInt32 loopCount, IOAudioClientBufferSet *bufferSet, UInt32 sampleIntervalHi, UInt32 sampleIntervalLo) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn performClientInput(UInt32 firstSampleFrame, IOAudioClientBufferSet *bufferSet) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual void performWatchdogOutput(IOAudioClientBufferSet *clientBufferSet, UInt32 generationCount) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual void lockBuffers() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual void unlockBuffers() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	static void setCommandGateUsage(IOAudioEngineUserClient *userClient, bool increment) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;		// <rdar://8518215>

public:
	
	static IOAudioEngineUserClient *withAudioEngine(IOAudioEngine *engine, task_t clientTask, void *securityToken, UInt32 type) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	static IOAudioEngineUserClient *withAudioEngine(IOAudioEngine *engine, task_t clientTask, void *securityToken, UInt32 type, OSDictionary *properties) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual bool initWithAudioEngine(IOAudioEngine *engine, task_t task, void *securityToken, UInt32 type) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual void free() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual void freeClientBufferSetList() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual void freeClientBuffer(IOAudioClientBuffer64 *clientBuffer) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual void stop(IOService *provider) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual bool isOnline() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOReturn registerBuffer(IOAudioStream *audioStream, void* sourceBuffer, UInt32 bufSizeInBytes, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn unregisterBuffer( void * sourceBuffer, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	static IOReturn _registerBufferAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	static IOReturn registerBufferAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	static IOReturn _unregisterBufferAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	static IOReturn unregisterBufferAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOReturn registerClientBuffer(IOAudioStream *audioStream, void * sourceBuffer, UInt32 bufSizeInBytes, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn unregisterClientBuffer(void * sourceBuffer, UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	static IOReturn _getNearestStartTimeAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	static IOReturn getNearestStartTimeAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOAudioClientBufferSet *findBufferSet(UInt32 bufferSetID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual void removeBufferSet(IOAudioClientBufferSet *bufferSet) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOReturn getConnectionID(UInt32 *connectionID) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOReturn clientStart() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn clientStop() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	static IOReturn _startClientAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	static IOReturn startClientAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	static IOReturn _stopClientAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://7529580>
	static IOReturn stopClientAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOReturn startClient() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn stopClient() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual IOReturn performClientIO(UInt32 firstSampleFrame, UInt32 loopCount, bool inputIO, UInt32 bufferSetID, UInt32 sampleIntervalHi, UInt32 sampleIntervalLo) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	virtual void sendFormatChangeNotification(IOAudioStream *audioStream) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	virtual IOReturn sendNotification(UInt32 notificationType) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
};

// <rdar://101000004> END


#endif /* _IOKIT_IOAUDIOENGINEUSERCLIENT_H */
