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

#ifndef _IOKIT_IOAUDIOSTREAM_H
#define _IOKIT_IOAUDIOSTREAM_H

#include <IOKit/IOService.h>
#include <AvailabilityMacros.h>

#ifndef IOAUDIOFAMILY_SELF_BUILD
#include <IOKit/audio/IOAudioEngine.h>
#include <IOKit/audio/IOAudioTypes.h>
#else
#include "IOAudioEngine.h"
#include "IOAudioTypes.h"
#endif

class OSSymbol;
class OSArray;
class OSDictionary;
class OSSet;

class IOCommandGate;
class IOAudioControl;

struct IOAudioClientBuffer;
struct IOAudioStreamFormatDesc;

/*!
 * @class IOAudioStream
 * @abstract This class wraps a single sample buffer in an audio driver.
 * @discussion An IOAudioStream represents one hardware sample buffer as well as the direction
 *  of that buffer, the mix buffer that multiple clients mix into as well as a list of
 *  all of the formats to which this buffer can be set.
 *
 *  When an IOAudioEngine is created during init time in the driver, an IOAudioStream must be
 *  created for each sample buffer in the device.  Typically, the sample buffer will be interleaved 
 *  (or single channel), as a non-interleaved buffer should be divided into multiple single-channel
 *  buffers (and multiple IOAudioStreams).
 *
 *  Additionally, when an IOAudioStream is created it must have all of the possible formats (and
 *  allowed sample rates for each format) set and must have the currently set format specified
 *  (addAvailableFormat() and setFormat()).
 */

class IOAudioStream : public IOService
{
    OSDeclareDefaultStructors(IOAudioStream)
    
    friend class IOAudioEngine;
    friend class IOAudioEngineUserClient;

public:

    typedef IOReturn (*AudioIOFunction)(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    static const OSSymbol	*gDirectionKey;
    static const OSSymbol	*gNumChannelsKey;
    static const OSSymbol	*gSampleFormatKey;
    static const OSSymbol	*gNumericRepresentationKey;
    static const OSSymbol	*gBitDepthKey;
    static const OSSymbol	*gBitWidthKey;
    static const OSSymbol	*gAlignmentKey;
    static const OSSymbol	*gByteOrderKey;
    static const OSSymbol	*gIsMixableKey;
    static const OSSymbol	*gDriverTagKey;
    static const OSSymbol	*gMinimumSampleRateKey;
    static const OSSymbol	*gMaximumSampleRateKey;

    static void initKeys() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    static OSDictionary *createDictionaryFromFormat(const IOAudioStreamFormat *streamFormat, const IOAudioStreamFormatExtension *formatExtension, OSDictionary *formatDict = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    static IOAudioStreamFormat *createFormatFromDictionary(const OSDictionary *formatDict, IOAudioStreamFormat *streamFormat = 0, IOAudioStreamFormatExtension *formatExtension = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    IOAudioEngine 				*audioEngine;
    IOWorkLoop					*workLoop;
    IOCommandGate				*commandGate;
    IORecursiveLock				*streamIOLock;
    
    UInt32						numClients;

    IOAudioStreamDirection		direction;

    IOAudioStreamFormat			format;
    IOAudioStreamFormatDesc		*availableFormats;
    OSArray						*availableFormatDictionaries;
    UInt32						numAvailableFormats;
    
    UInt32						startingChannelID;
    UInt32						maxNumChannels;
    
    void						*sampleBuffer;
    UInt32						sampleBufferSize;

    void						*mixBuffer;
    UInt32						mixBufferSize;
    bool						streamAllocatedMixBuffer;
    
    AudioIOFunction				*audioIOFunctions;
    UInt32						numIOFunctions;
    
    bool						streamAvailable;
    
    OSSet						*defaultAudioControls;
    
    IOAudioEnginePosition		startingPosition;
    IOAudioEnginePosition		clippedPosition;
    
    IOAudioClientBuffer			*clientBufferListStart;
    IOAudioClientBuffer			*clientBufferListEnd;
    
    IOAudioClientBuffer			*userClientList;
	
protected:

	enum {
		kErrorLogClipMoreThanOneBufferAhead = 0,
		kErrorLogClipMoreThanOneBufferAheadPart2,
		kErrorLogClipPositionIsOff,
		kErrorLogAlreadyClipped,
		kErrorLogClipBuffersAreNULL,
		kErrorLogClipReturnsAnError,
		kErrorLogNumberOfCounters,			// needs to be after the last counter
		kErrorLogDumpCounters = 0xFFFF
	};

    struct ExpansionData {
		IOAudioStreamFormatExtension	streamFormatExtension;
		UInt32							mSampleFramesReadByEngine;
		IOReturn						mClipOutputStatus;
		unsigned int					mStreamErrorCounts[kErrorLogNumberOfCounters];
		bool							mStreamErrorCountsUpdated;						 
	};
    
    ExpansionData *reserved;

public:
// New code added here:
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 0);
    virtual const IOAudioStreamFormatExtension *getFormatExtension() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 1);
    virtual IOReturn setFormat(const IOAudioStreamFormat *streamFormat, const IOAudioStreamFormatExtension *formatExtension, bool callDriver = true) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 2);
    virtual IOReturn setFormat(const IOAudioStreamFormat *streamFormat, const IOAudioStreamFormatExtension *formatExtension, OSDictionary *formatDict, bool callDriver = true) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 3);
    virtual void addAvailableFormat(const IOAudioStreamFormat *streamFormat, const IOAudioStreamFormatExtension *formatExtension, const IOAudioSampleRate *minRate, const IOAudioSampleRate *maxRate, const AudioIOFunction *ioFunctionList = NULL, UInt32 numFunctions = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 4);
    virtual void addAvailableFormat(const IOAudioStreamFormat *streamFormat, const IOAudioStreamFormatExtension *formatExtension, const IOAudioSampleRate *minRate, const IOAudioSampleRate *maxRate, AudioIOFunction ioFunction) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 5);
    virtual bool validateFormat(IOAudioStreamFormat *streamFormat, IOAudioStreamFormatExtension *formatExtension, IOAudioStreamFormatDesc *formatDesc) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 6);
	virtual void setTerminalType(const UInt32 terminalType) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 7);
	virtual IOReturn mixOutputSamples(const void *sourceBuf, void *mixBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 8);
	virtual void setSampleLatency(UInt32 numSamples) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 9);
	virtual bool validateFormat(IOAudioStreamFormat *streamFormat, IOAudioStreamFormatExtension *formatExtension, IOAudioStreamFormatDesc *formatDesc, const IOAudioSampleRate *sampleRate) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 10);
	virtual UInt32 getNumSampleFramesRead() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	// OSMetaClassDeclareReservedUsed(IOAudioStream, 11);
	virtual void setDefaultNumSampleFramesRead(UInt32) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

private:
    OSMetaClassDeclareReservedUsed(IOAudioStream, 0);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 1);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 2);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 3);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 4);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 5);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 6);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 7);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 8);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 9);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 10);
    OSMetaClassDeclareReservedUsed(IOAudioStream, 11);

    OSMetaClassDeclareReservedUnused(IOAudioStream, 12);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 13);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 14);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 15);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 16);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 17);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 18);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 19);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 20);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 21);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 22);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 23);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 24);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 25);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 26);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 27);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 28);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 29);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 30);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 31);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 32);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 33);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 34);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 35);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 36);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 37);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 38);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 39);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 40);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 41);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 42);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 43);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 44);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 45);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 46);
    OSMetaClassDeclareReservedUnused(IOAudioStream, 47);

public:
    virtual bool initWithAudioEngine(IOAudioEngine *engine, IOAudioStreamDirection dir, UInt32 startChannelID, const char *streamDescription = NULL, OSDictionary *properties = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void free() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void stop(IOService *provider) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOWorkLoop *getWorkLoop() const;

    virtual IOReturn setProperties(OSObject *properties) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual IOAudioStreamDirection getDirection() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual void setSampleBuffer(void *buffer, UInt32 size) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void *getSampleBuffer() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual UInt32 getSampleBufferSize() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void setMixBuffer(void *buffer, UInt32 size) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void *getMixBuffer() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual UInt32 getMixBufferSize() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void numSampleFramesPerBufferChanged() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual void clearSampleBuffer() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void setIOFunction(AudioIOFunction ioFunction) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void setIOFunctionList(const AudioIOFunction *ioFunctionList, UInt32 numFunctions) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

    virtual const IOAudioStreamFormat *getFormat() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    static IOReturn setFormatAction(OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	static IOReturn _setFormatAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;	// <rdar://8568040,8691669>
    virtual IOReturn setFormat(const IOAudioStreamFormat *streamFormat, bool callDriver = true) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn setFormat(OSDictionary *formatDict) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn setFormat(const IOAudioStreamFormat *streamFormat, OSDictionary *formatDict, bool callDriver = true) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn hardwareFormatChanged(const IOAudioStreamFormat *streamFormat) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void addAvailableFormat(const IOAudioStreamFormat *streamFormat, const IOAudioSampleRate *minRate, const IOAudioSampleRate *maxRate, const AudioIOFunction *ioFunctionList = NULL, UInt32 numFunctions = 0) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void addAvailableFormat(const IOAudioStreamFormat *streamFormat, const IOAudioSampleRate *minRate, const IOAudioSampleRate *maxRate, AudioIOFunction ioFunction) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void clearAvailableFormats() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual bool validateFormat(IOAudioStreamFormat *streamFormat, IOAudioStreamFormatDesc *formatDesc) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual UInt32 getStartingChannelID() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual UInt32 getMaxNumChannels() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void setStreamAvailable(bool available) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual bool getStreamAvailable() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOReturn addDefaultAudioControl(IOAudioControl *defaultAudioControl) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void removeDefaultAudioControls() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

protected:
    virtual void lockStreamForIO() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void unlockStreamForIO() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void updateNumClients() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn addClient(IOAudioClientBuffer *clientBuffer) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void removeClient(IOAudioClientBuffer *clientBuffer) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual UInt32 getNumClients() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual IOReturn processOutputSamples(IOAudioClientBuffer *clientBuffer, UInt32 firstSampleFrame, UInt32 loopCount, bool samplesAvailable) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual IOReturn readInputSamples(IOAudioClientBuffer *clientBuffer, UInt32 firstSampleFrame) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void resetClipInfo() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void clipIfNecessary() AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    virtual void clipOutputSamples(UInt32 startingSampleFrame, UInt32 numSampleFrames) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
    
    virtual void setStartingChannelNumber(UInt32 channelNumber) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

private:
    virtual void setDirection(IOAudioStreamDirection dir) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;
	
	void safeLogError(int error , long unsigned int arg1 , long unsigned int arg2 , long unsigned int arg3 , long unsigned int arg4 , void* arg5, void* arg6) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_10;

};

#endif /* _IOKIT_IOAUDIOSTREAM_H */
