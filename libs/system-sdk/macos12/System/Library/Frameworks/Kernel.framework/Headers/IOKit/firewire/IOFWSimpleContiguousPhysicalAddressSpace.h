/*
 * Copyright (c) 1998-2006 Apple Computer, Inc. All rights reserved.
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
 
#ifndef _IOFWSIMPLECONTIGUOUSPHYSICALADDRESSSPACE_H_
#define _IOFWSIMPLECONTIGUOUSPHYSICALADDRESSSPACE_H_

#include <IOKit/firewire/IOFWSimplePhysicalAddressSpace.h>

/*! @class IOFWSimpleContiguousPhysicalAddressSpace
*/

class IOFWSimpleContiguousPhysicalAddressSpace : public IOFWSimplePhysicalAddressSpace
{
	OSDeclareDefaultStructors( IOFWSimpleContiguousPhysicalAddressSpace )

private:

	void * fSimpleContigPhysSpaceMembers;
	
	IOReturn cachePhysicalAddress( void );

protected:	
	virtual bool createMemberVariables( void ) APPLE_KEXT_OVERRIDE;
	virtual void destroyMemberVariables( void ) APPLE_KEXT_OVERRIDE;

public:

	virtual bool init( IOFireWireBus * control, vm_size_t size, IODirection direction );
	virtual void free( void ) APPLE_KEXT_OVERRIDE;

	FWAddress getFWAddress( void );

private:
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 0);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 1);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 2);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 3);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 4);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 5);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 6);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 7);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 8);
    OSMetaClassDeclareReservedUnused(IOFWSimpleContiguousPhysicalAddressSpace, 9);

};

#endif
