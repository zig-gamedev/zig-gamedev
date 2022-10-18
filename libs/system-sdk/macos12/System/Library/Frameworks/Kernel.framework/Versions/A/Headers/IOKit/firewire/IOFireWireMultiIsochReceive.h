/*
 * Copyright (c) 2008 Apple Computer, Inc. All rights reserved.
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

#ifndef _IOKIT_IOFIREWIREMULTIISOCHRECEIVE_H_
#define _IOKIT_IOFIREWIREMULTIISOCHRECEIVE_H_

class IOFireWireMultiIsochReceiveListener;
class IOFireWireMultiIsochReceivePacket;
class IOFireWireController;

typedef IOReturn (*FWMultiIsochReceiveListenerCallback)(void *refcon, IOFireWireMultiIsochReceivePacket *pPacket);

// These are the parameters clients can set which help us to optimize the mult-isoch-receiver 
// polling interval, and memory resources 
typedef struct FWMultiIsochReceiveListenerParamsStruct
	{
		// How much latency, from when the packet arrives to when the client is notified, can the client tolerate. 
		UInt32 maxLatencyInFireWireCycles;
		
		// In bits per second, the expected bit-rate of the incoming stream
		UInt32 expectedStreamBitRate;
		
		// How long does the client expect to hold onto packets objects before returning them back to the receiver
		UInt32 clientPacketReturnLatencyInFireWireCycles;
	}FWMultiIsochReceiveListenerParams;

/*! @class IOFireWireMultiIsochReceiveListener
*/

class IOFireWireMultiIsochReceiveListener : public OSObject
	{
		friend class IOFireWireLink;
		
	protected:
		OSDeclareDefaultStructors(IOFireWireMultiIsochReceiveListener)
		bool init(IOFireWireController *fwController,
				  UInt32 receiveChannel,
				  FWMultiIsochReceiveListenerCallback callback,
				  void *pCallbackRefCon,
				  FWMultiIsochReceiveListenerParams *pListenerParams);
		void free() APPLE_KEXT_OVERRIDE;
	public:
		static IOFireWireMultiIsochReceiveListener *create(IOFireWireController *fwController,
														   UInt32 channel,
														   FWMultiIsochReceiveListenerCallback callback,
														   void *pCallbackRefCon,
														   FWMultiIsochReceiveListenerParams *pListenerParams);
		
		// Call this to activate the listener
		IOReturn Activate();
		
		// Call this to deactivate the listener
		IOReturn Deactivate();
		
		// Call this to modify the callback/refcon pointers. Only call this when not activated!
		IOReturn SetCallback(FWMultiIsochReceiveListenerCallback callback,
							 void *pCallbackRefCon);
		
		// Accessors
		inline UInt32 getReceiveChannel(void) {return fChannel;};
		inline FWMultiIsochReceiveListenerCallback getCallback(void){return fClientCallback;}; 
		inline void * getRefCon(void){return fClientCallbackRefCon;};
		inline bool getActivatedState(void) {return fActivated;};
		
	protected:
		IOFireWireController *fControl;
		UInt32 fChannel;
		FWMultiIsochReceiveListenerCallback fClientCallback;
		void *fClientCallbackRefCon;
		bool fActivated;
		FWMultiIsochReceiveListenerParams *fListenerParams;
	};

#define kMaxRangesPerMultiIsochReceivePacket 6

/*! @class IOFireWireMultiIsochReceivePacket
*/

class IOFireWireMultiIsochReceivePacket : public OSObject
	{
		OSDeclareDefaultStructors(IOFireWireMultiIsochReceivePacket)
		bool init(IOFireWireController *fwController);
		void free() APPLE_KEXT_OVERRIDE;
	public:
		static IOFireWireMultiIsochReceivePacket *create(IOFireWireController *fwController);
		
		// The clients who are passed this packet by the 
		// multi-isoch receiver calling their callback
		// MUST call clientDone() on this packet to
		// return it back for reuse!
		void clientDone(void);
		
		UInt32 isochChannel(void);
		UInt32 packetReceiveTime(void);
		
		UInt32 isochPayloadSize(void);	// The size of just the isoch payload, not including header/trailer quads.
		inline UInt32 isochPacketSize(void) {return isochPayloadSize()+8; };	// The size of the packet, including header/trailer quads.
		
		// This returns a memory descriptor to the client. The client must call complete(), and release() on the
		// memory descriptor when done.
		IOMemoryDescriptor *createMemoryDescriptorForRanges(void);
		
		// These should be treated as read-only by clients,
		// as should the data contained in these buffers!
		IOAddressRange ranges[kMaxRangesPerMultiIsochReceivePacket] ;
		UInt32 numRanges;
		
		// These should be treated private for clients!
		// Messing with them will screw up the bookkeepping
		// in the Multi-Isoch Receiver!
		UInt32 numClientReferences;
		void* elements[kMaxRangesPerMultiIsochReceivePacket];
		
	protected:
		IOFireWireController *fControl;
	};

#endif
