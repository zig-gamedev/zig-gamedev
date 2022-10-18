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
#ifndef _IOKIT_IOFireWireAVCCommand_H
#define _IOKIT_IOFireWireAVCCommand_H
 
#include <IOKit/firewire/IOFWCommand.h>

/*! @class IOFireWireAVCCommand
*/
class IOFireWireAVCCommand : public IOFWCommand
{
    OSDeclareDefaultStructors(IOFireWireAVCCommand)
    
protected:
    IOFWCommand 		*fWriteCmd;
    IOMemoryDescriptor	*fMem;
    const UInt8 		*fCommand;
    
    UInt32 	fCmdLen;
    UInt8 	*fResponse;
    UInt32 	*fResponseLen;
    int		fCurRetries;
    int		fMaxRetries;
    
    UInt32	fWriteGen;
    UInt16	fWriteNodeID;
    bool	bypassRobustCommandResponseMatching;
	
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { 
		bool 	fStarted;
		bool 	fSyncWakeupSignaled;
	};

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *fIOFireWireAVCCommandExpansion;
    
    static void writeDone(void *refcon, IOReturn status, IOFireWireNub *device, IOFWCommand *fwCmd);
    
    virtual IOReturn	complete(IOReturn status);
    virtual IOReturn	execute();
    virtual void		free();
    
public:
    virtual bool init(IOFireWireNub *device, const UInt8 * command, UInt32 cmdLen,
                                                    UInt8 * response, UInt32 * responseLen);
    virtual IOReturn reinit(IOFireWireNub *device, const UInt8 * command, UInt32 cmdLen,
                                                    UInt8 * response, UInt32 * responseLen);
                                                    
    static IOFireWireAVCCommand *withNub(IOFireWireNub *device, const UInt8 * command, UInt32 cmdLen,
                                                    UInt8 * response, UInt32 * responseLen);
                                                    
    static IOFireWireAVCCommand *withNub(IOFireWireNub *device, UInt32 generation,
                const UInt8 * command, UInt32 cmdLen, UInt8 * response, UInt32 * responseLen);
                                                    
    virtual UInt32 handleResponse(UInt16 nodeID, UInt32 len, const void *buf);

    virtual IOReturn resetInterimTimeout();

	virtual UInt32 handleResponseWithSimpleMatching(UInt16 nodeID, UInt32 len, const void *buf);

    virtual IOReturn 	submit(bool queue = false);

private:
    OSMetaClassDeclareReservedUsed(IOFireWireAVCCommand, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCCommand, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCCommand, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireAVCCommand, 3);
};

#endif // _IOKIT_IOFireWireAVCCommand_H

