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
 * IOFWIsochPort is an abstract object that represents hardware on the bus
 * (locally or remotely) that sends or receives isochronous packets.
 * Local ports are implemented by the local device driver,
 * Remote ports are implemented by the driver for the remote device.
 *
 * HISTORY
 *
 */


#ifndef _IOKIT_IOFWISOCHPORT_H
#define _IOKIT_IOFWISOCHPORT_H

#include <libkern/c++/OSObject.h>
#include <IOKit/firewire/IOFireWireFamilyCommon.h>

/*! @class IOFWIsochPort
*/

class IOFWIsochPort : public OSObject
{
    OSDeclareAbstractStructors(IOFWIsochPort)

public:
	// Return maximum speed and channels supported
	// (bit n set = chan n supported)
    virtual IOReturn getSupported(IOFWSpeed &maxSpeed, UInt64 &chanSupported) = 0;

	// Allocate hardware resources for port
    virtual IOReturn allocatePort(IOFWSpeed speed, UInt32 chan) = 0;
    virtual IOReturn releasePort() = 0;		// Free hardware resources
    virtual IOReturn start() = 0;		// Start port processing packets
    virtual IOReturn stop() = 0;		// Stop processing packets

private:
    OSMetaClassDeclareReservedUnused(IOFWIsochPort, 0);
    OSMetaClassDeclareReservedUnused(IOFWIsochPort, 1);

};

#endif /* ! _IOKIT_IOFWISOCHPORT_H */

