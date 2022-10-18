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
 * HISTORY
 *
 */


#ifndef _IOKIT_IOFWDCLPROGRAM_H
#define _IOKIT_IOFWDCLPROGRAM_H

#include <libkern/c++/OSObject.h>
#include <IOKit/firewire/IOFireWireFamilyCommon.h>
#include <IOKit/firewire/IOFireWireBus.h>
#include <IOKit/IOMemoryCursor.h>
#include <IOKit/IOMapper.h>

/*! @class IODCLProgram
 */
class IODCLProgram : public OSObject
{
	OSDeclareAbstractStructors(IODCLProgram)
	
	private :
	
	void * 						reserved0 ;//fDCLTaskToKernel;
	void * 						reserved1 ;//fDataTaskToKernel;
	void *		 				reserved2 ;//fDataBase;
	void *		 				reserved3 ;//		IOMemoryDescriptor *		fDCLDesc;
	IOMemoryMap *				fBufferMem ;
	void *		 				reserved5 ;//		IOMemoryCursor *			fDataCursor;
	
protected:
	
	/*! @struct ExpansionData
	 @discussion This structure will be used to expand the capablilties of the class in the future.
	 */
	struct ExpansionData
	{
		IOFWIsochResourceFlags		resourceFlags ;
		IODMACommand *              fDMACommand;
	};
	
	/*! @var reserved
	 Reserved for future use.  (Internal use only)  */
	ExpansionData *					fExpansionData ;
	
	public :
	
	virtual void			setIsochResourceFlags ( IOFWIsochResourceFlags flags ) ;	// formerly getPhysicalSegs()
	IOFWIsochResourceFlags	getIsochResourceFlags () const ;
	
protected:
	
	virtual void 			free (void) APPLE_KEXT_OVERRIDE;
	
public:
	
	virtual bool 			init ( IOFireWireBus::DCLTaskInfo * info = NULL ) ;
	virtual IOReturn 		allocateHW (
										IOFWSpeed 			speed,
										UInt32 				chan) = 0;
	virtual IOReturn 		releaseHW () = 0;
	virtual IOReturn 		compile (
									 IOFWSpeed 			speed,
									 UInt32 				chan) = 0;
	virtual IOReturn 		notify (
									IOFWDCLNotificationType		notificationType,
									DCLCommand ** 				dclCommandList,
									UInt32 						numDCLCommands ) = 0;
	virtual IOReturn 		start () = 0;
	virtual void 			stop () = 0;
	virtual IOReturn 		pause ();
	virtual IOReturn 		resume ();
	
	virtual void			setForceStopProc(
											 IOFWIsochChannel::ForceStopNotificationProc proc,
											 void * 						refCon,
											 IOFWIsochChannel *			channel ) ;
	protected :
	
	void					generateBufferMap( DCLCommand * program ) ;
	IOReturn				virtualToPhysical(
											  IOVirtualRange						ranges[],
											  unsigned							rangeCount,
											  IOMemoryCursor::IOPhysicalSegment	outSegments[],
											  unsigned &							outPhysicalSegmentCount,
											  unsigned							maxSegments ) ;
	
	public :
	
	IOMemoryMap *			getBufferMap() const ;
	
	public :
	
	// close/open isoch workloop gate...
	// clients should not need to call these.
	virtual void			closeGate() = 0 ;
	virtual void			openGate() = 0 ;
	
	virtual IOReturn		synchronizeWithIO() = 0 ;
	
	virtual IOMapper *      copyMapper() = 0;
	
private:
	
	OSMetaClassDeclareReservedUsed(IODCLProgram, 0);
	OSMetaClassDeclareReservedUsed(IODCLProgram, 1);
	
	// *** VERIFY: IODCLProgram is now using one of the OSMetaClassDeclareReservedUsed
	OSMetaClassDeclareReservedUsed(IODCLProgram, 2);
	OSMetaClassDeclareReservedUnused(IODCLProgram, 3);
	OSMetaClassDeclareReservedUnused(IODCLProgram, 4);
	
};

#endif /* ! _IOKIT_IOFWDCLPROGRAM_H */




