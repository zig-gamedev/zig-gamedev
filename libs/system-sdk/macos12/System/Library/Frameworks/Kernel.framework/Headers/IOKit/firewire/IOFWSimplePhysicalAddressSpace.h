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
 
#ifndef _IOFWSIMPLEPHYSICALADDRESSSPACE_H_
#define _IOFWSIMPLEPHYSICALADDRESSSPACE_H_

#include <libkern/c++/OSObject.h>
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IODMACommand.h>
#include <IOKit/firewire/IOFWPhysicalAddressSpace.h>

/*! @class IOFWSimplePhysicalAddressSpace
*/

class IOFWSimplePhysicalAddressSpace : public IOFWPhysicalAddressSpace
{
	OSDeclareDefaultStructors( IOFWSimplePhysicalAddressSpace )
	
private:

	void *	fSimplePhysSpaceMembers;
		
	IOReturn allocateMemory( void );	
	void deallocateMemory( void );

protected:	
	virtual bool createMemberVariables( void );
	virtual void destroyMemberVariables( void );

public:

	virtual bool init( IOFireWireBus * control, vm_size_t size, IODirection direction, bool contiguous = false );
	virtual void free( void ) APPLE_KEXT_OVERRIDE;

	IOVirtualAddress getVirtualAddress( void );

private:
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 0);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 1);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 2);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 3);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 4);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 5);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 6);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 7);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 8);
    OSMetaClassDeclareReservedUnused(IOFWSimplePhysicalAddressSpace, 9);
	
};

#endif
