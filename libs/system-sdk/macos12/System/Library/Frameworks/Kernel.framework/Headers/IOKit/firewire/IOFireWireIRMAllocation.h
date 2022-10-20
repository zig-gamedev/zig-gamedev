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

#ifndef _IOKIT_IOFIREWIREIRMALLOCATION_H
#define _IOKIT_IOFIREWIREIRMALLOCATION_H

#include <libkern/c++/OSObject.h>

class IOFireWireController;

//#include <IOKit/firewire/IOFireWireController.h>
//#include <IOKit/firewire/IOFireWireFamilyCommon.h>

/*! @class IOFireWireIRMAllocation
*/

class IOFireWireIRMAllocation : public OSObject
{
	friend class IOFireWireController;

    OSDeclareDefaultStructors(IOFireWireIRMAllocation)
	
public:
		
		// Prototype for the callback if reallocation after bus-reset is unsuccessful.
		typedef IOReturn (*AllocationLostNotificationProc)(void* refCon, class IOFireWireIRMAllocation* allocation);

	// Initialize the IRM allocation object. 
    virtual bool init( IOFireWireController * control,
					   Boolean releaseIRMResourcesOnFree = true, 
					   AllocationLostNotificationProc allocationLostProc = NULL,
					   void *pLostProcRefCon = NULL);
    	
	// Specify whether of not the IRM resources should automatically
	// be released when freeing this allocation object.
	virtual void setReleaseIRMResourcesOnFree(Boolean doRelease);
	
	// Use the IRMAllocation object to allocate isoch resources
	virtual IOReturn allocateIsochResources(UInt8 isochChannel, UInt32 bandwidthUnits);

	// Free isoch resources previously allocated with a call to allocateIsochResources
	virtual IOReturn deallocateIsochResources(void);
	
	// Returns true if isoch resources are currently allocated, and if true, the allocated channel, and amount of isoch bandwidth.
	virtual Boolean areIsochResourcesAllocated(UInt8 *pAllocatedIsochChannel, UInt32 *pAllocatedBandwidthUnits);
	
	// Get the refcon
	virtual void * GetRefCon(void);
	virtual void SetRefCon(void* refCon); 

	// Override the base-class release function for special processing
	virtual void release() const APPLE_KEXT_OVERRIDE;
	
protected:

		/*! @struct ExpansionData
		@discussion This structure will be used to expand the capablilties of the class in the future.
		*/    
		struct ExpansionData { };
	
		/*! @var reserved
		Reserved for future use.  (Internal use only)  */
		ExpansionData *reserved;

		// Free the allocation object (and release IRM resources if needed)
		virtual void free( void ) APPLE_KEXT_OVERRIDE;

		// Controller will call this to notify about bus-reset complete.
		virtual void handleBusReset(UInt32 generation);
	
		virtual void failedToRealloc(void);
		virtual UInt32 getAllocationGeneration(void);
		static void threadFunc( void * arg );

private:
	
	AllocationLostNotificationProc fAllocationLostProc;
	void *fLostProcRefCon;
	Boolean fReleaseIRMResourcesOnFree;
	UInt8 fIsochChannel; 
	UInt32 fBandwidthUnits;
	UInt32 fAllocationGeneration;
	IORecursiveLock *fLock ;
	IOFireWireController *fControl;
	Boolean isAllocated;
	
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 0);
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 1);
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 2);
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 3);
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 4);
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 5);
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 6);
	OSMetaClassDeclareReservedUnused(IOFireWireIRMAllocation, 7);
};

#endif // _IOKIT_IOFIREWIREIRMALLOCATION_H
