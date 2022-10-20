/*
 * Copyright (c) 2001 Apple Computer, Inc. All rights reserved.
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
 *	IOFireWirePCRSpace.h
 *
 * Class to multiplex access to the PCR addresses.
 */
#ifndef _IOKIT_IOFIREWIREPCRSPACE_H
#define _IOKIT_IOFIREWIREPCRSPACE_H

#include <IOKit/firewire/IOFWAddressSpace.h>
#include <IOKit/avc/IOFireWireAVCTargetSpace.h>

/*!
    @typedef IOFireWirePCRCallback
	@abstract Callback called after a successful lock transaction to a plug.
    @param refcon refcon supplied to the IOFireWireFCPSpace when a client is registered
	@param nodeID is the node originating the request
	@param plugNo is the plug number
	@param oldVal is the value the plug used to contain
    @param newVal is the quad written into the plug

 */
typedef void (*IOFireWirePCRCallback)(void *refcon, UInt16 nodeID, UInt32 plug, UInt32 oldVal, UInt32 newVal);

class IOFireWireBus;

/*!
    @class IOFireWirePCRSpace
    @abstract object to multiplex users of the PCR plug registers
*/
class IOFireWirePCRSpace : public IOFWPseudoAddressSpace
{
    OSDeclareDefaultStructors(IOFireWirePCRSpace)
    
protected:
    struct Client {
        IOFireWirePCRCallback func;
        void * refcon;
    };
    UInt32 fBuf[64];
    
    Client fClients[64];
    UInt32 fActivations;
	IOFireWireAVCTargetSpace *	fAVCTargetSpace;
	IONotifier *fNotifier;
    
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;
    
    // Override to notify client of each plug seperately
    virtual UInt32 doWrite(UInt16 nodeID, IOFWSpeed &speed, FWAddress addr, UInt32 len,
                           const void *buf, IOFWRequestRefCon refcon);
                          
    virtual IOReturn allocatePlug(void *refcon, IOFireWirePCRCallback func, UInt32 &plug, Client* head);
    virtual void freePlug(UInt32 plug, Client* head);
    virtual UInt32 readPlug(UInt32 plug);
    virtual IOReturn updatePlug(UInt32 plug, UInt32 oldVal, UInt32 newVal);
    
public:

    // Override to handle multiple activations (one per client)
    virtual IOReturn activate();
    virtual void deactivate();

/*!
    @function init
    @abstract initializes the IOFireWirePCRSpace object
*/
    virtual bool init(IOFireWireBus *bus);
/*!
    @function getPCRAddressSpace
    @abstract returns the IOFireWirePCRSpace object for the given FireWire bus
    @param bus The FireWire bus
*/
    static IOFireWirePCRSpace *getPCRAddressSpace(IOFireWireBus *bus);
/*!
    @function allocateInputPlug
    @abstract allocates an input plug.
    @param refcon arbitrary value passed back as first argument of callback.
    @param func callback function when a successful lock transaction to the plug has been performed
    @param plug set to the plug number if a plug is successfully allocated
*/
    virtual IOReturn allocateInputPlug(void *refcon, IOFireWirePCRCallback func, UInt32 &plug);
/*!
    @function freeInputPlug
    @abstract deallocates an input plug.
    @param plug value returned by allocateInputPlug.
*/
    virtual void freeInputPlug(UInt32 plug);
/*!
    @function readInputPlug
    @abstract returns the current value of an input plug.
    @param plug value returned by allocateInputPlug.
*/
    virtual UInt32 readInputPlug(UInt32 plug);
/*!
    @function updateInputPlug
    @abstract updates the value of an input plug (simulating a lock transaction).
    @param plug value returned by allocateInputPlug.
    @param oldVal value returned by readInputPlug.
    @param newVal new value to store in plug if it's current value is oldVal.
*/
    virtual IOReturn updateInputPlug(UInt32 plug, UInt32 oldVal, UInt32 newVal);
/*!
    @function allocateOutputPlug
    @abstract allocates an output plug.
    @param refcon arbitrary value passed back as first argument of callback.
    @param func callback function when a successful lock transaction to the plug has been performed
    @param plug set to the plug number if a plug is successfully allocated
*/
    virtual IOReturn allocateOutputPlug(void *refcon, IOFireWirePCRCallback func, UInt32 &plug);
/*!
    @function freeOutputPlug
    @abstract deallocates an output plug.
    @param plug value returned by allocateOutputPlug.
*/
    virtual void freeOutputPlug(UInt32 plug);
/*!
    @function readOutputPlug
    @abstract returns the current value of an output plug.
    @param plug value returned by allocateOutputPlug.
*/
    virtual UInt32 readOutputPlug(UInt32 plug);
/*!
    @function updateOutputPlug
    @abstract updates the value of an output plug (simulating a lock transaction).
    @param plug value returned by allocateOutputPlug.
    @param oldVal value returned by readOutputPlug.
    @param newVal new value to store in plug if it's current value is oldVal.
*/
    virtual IOReturn updateOutputPlug(UInt32 plug, UInt32 oldVal, UInt32 newVal);
/*!
    @function readOutputMasterPlug
    @abstract returns the current value of the output master plug.
*/
    virtual UInt32 readOutputMasterPlug();
/*!
    @function updateOutputMasterPlug
    @abstract updates the value of the master output plug (simulating a lock transaction).
    @param oldVal value returned by readOutputMasterPlug.
    @param newVal new value to store in plug if it's current value is oldVal.
*/
    virtual IOReturn updateOutputMasterPlug(UInt32 oldVal, UInt32 newVal);
/*!
    @function readInputMasterPlug
    @abstract returns the current value of the input master plug.
*/
    virtual UInt32 readInputMasterPlug();
/*!
    @function updateInputMasterPlug
    @abstract updates the value of the master input plug (simulating a lock transaction).
    @param oldVal value returned by readInputMasterPlug.
    @param newVal new value to store in plug if it's current value is oldVal.
*/
    virtual IOReturn updateInputMasterPlug(UInt32 oldVal, UInt32 newVal);

	/*!
		@function setAVCTargetSpacePointer
	 */
    virtual void setAVCTargetSpacePointer(IOFireWireAVCTargetSpace *pAVCTargetSpace);

	/*!
		@function clearAllP2PConnections
	 */
    virtual void clearAllP2PConnections(void);
	
private:
    OSMetaClassDeclareReservedUnused(IOFireWirePCRSpace, 0);
    OSMetaClassDeclareReservedUnused(IOFireWirePCRSpace, 1);
    OSMetaClassDeclareReservedUnused(IOFireWirePCRSpace, 2);
    OSMetaClassDeclareReservedUnused(IOFireWirePCRSpace, 3);

};

#endif /* _IOKIT_IOFIREWIREPCRSPACE_H */
