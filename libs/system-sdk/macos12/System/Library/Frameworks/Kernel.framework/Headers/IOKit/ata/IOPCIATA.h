/*
 * Copyright (c) 1998-2008 Apple Inc. All rights reserved.
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
 
#ifndef _DRV_PCI_ATA_H
#define _DRV_PCI_ATA_H

#include <libkern/c++/OSObject.h>
#include <IOKit/IOTypes.h>
#include "IOATAController.h"
#include <IOKit/IOMemoryCursor.h>
#include <IOKit/IOBufferMemoryDescriptor.h>

#include <IOKit/IOInterruptEventSource.h>

/*! @class IOPCIATA
    @abstract The base class for  PCI-IDE ata controller family.
    @discussion class defining the common elements of bus-mastering PCI ATA controllers which meet or at least loosely follow the pci bus mastering pci-ide controller spec. Header doc is incomplete, but source is heavily commented.

*/



class IOPCIATA : public IOATAController
{
    OSDeclareDefaultStructors(IOPCIATA);

public:

	/*--- Overrides from IOATAController ---*/
	virtual bool init(OSDictionary * properties);
    virtual bool start( IOService* provider );

protected:

	// The DMA states: not in use, in use and running with additional passes needed,
	// in use on final pass, transfer complete, and failure
	enum ATADMAState 
	{
		// DMA state flags
		kATADMAInactive,
		kATADMAStarting,
		kATADMAActive,
		kATADMAStatus,
		kATADMAComplete,
		kATADMAError,
	
	};
	
	enum {
		// PRD flags	
		kLast_PRD =	0x8000,
		kContinue_PRD = 0,
	
	};
	
		
	enum {
		mBMCmdStartOutput = 0x01,  						// start engine to transfer from memory to device.
		mBMCmdStartInput = (1 << 3 ) | 0x01,  	// start engine to transfer from device to memory
		mBMCmdStop = 0x00,								// halt engine.
	};
	
	enum {	
		// bus master status register definitions.
		bBMStatusSimplex = 7,  	// 0 = simultaneous transactions allowed. 1 = primary and secondary busses may not be active at same time.
		bBMStatusDrv1 = 6,     	// 1 = device 1 and bus are already configured by some other software/firmware
		bBMStatusDrv0 = 5,     	// 1 = device 0 and bus are already configured by some other software/firmware
		bBMStatusInt = 2,		// 1 = device has asserted INTRQ and all data is flushed to/from memory.
		bBMStatusError = 1,		// 1 = an error in the DMA has occured. Software clears by writing 1 to this bit.
		bBMStatusActive = 0,	// 1 = DMA engine is active.
	};
	
	enum{
	
		mBMStatusSimplex = 	1 << 7,
		mBMStatusDrv1 = 	1 << 6,
		mBMStatusDrv0 = 	1 << 5,
		mBMStatusInt = 		1 << 2,
		mBMStatusError = 	1 << 1,
		mBMStatusActive = 	1
	};
		
	// the physical region descriptor used for the dma engine.
	struct PRD
	{
		UInt32	bufferPtr;		// address
		UInt16	byteCount;		// 16 bit byte count where 0x0000 = 64K
		UInt16	flags;			// 0 in flags means contine, 0x80 means stop
	};	


	// descendants of this class MUST initialize these values
	// prior to activating any DMA command.
	IOATARegPtr8 _bmCommandReg;
	IOATARegPtr8 _bmStatusReg;
	IOATARegPtr32 _bmPRDAddresReg;	
	
	// semaphore for DMA state
	UInt32	_dmaState;
	
	// table of PRD descriptors
	PRD*	_prdTable;
	IOPhysicalAddress _prdTablePhysical;	
	
	IONaturalMemoryCursor*	_DMACursor;		

	// override from IOATAController
	// activate the DMA engine as per the current command
	virtual IOReturn startDMA( void );

	// override from IOATAController
	// safely halt the DMA engine  regardless of state
	virtual IOReturn stopDMA( void );

	// allocate memory for the PRD descriptors.
	virtual bool allocDMAChannel(void);
	
	// fill CC with stop commands.
	virtual void initATADMAChains (PRD* descPtr);
	
	// fill out a PRD, respecting endianess
	virtual void setPRD(UInt8 *bffr, UInt16 count, PRD *tableElement, UInt16 end);
	
	// setup the CC with IO commands
	virtual IOReturn createChannelCommands(void);
	
	// deallocate memory for the DMA engine
	virtual bool freeDMAChannel(void);
	
	// clean up on device interrupt
	virtual IOReturn handleDeviceInterrupt(void);
	
	// activate the DMA engine
	virtual void activateDMAEngine(void);
	
	// shutdown the DMA engine
	virtual void stopDMAEngine(void);
	
	// safely suspend the DMA engine
	virtual void shutDownATADMA (void);
	
	// overrides
	virtual void free();
protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOPCIATA class in the future.
    */
    typedef struct ExpansionData
    {
    	IOBufferMemoryDescriptor*	_prdBuffer;
    } ExpansionData;

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

private:
    OSMetaClassDeclareReservedUnused(IOPCIATA, 0);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 1);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 2);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 3);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 4);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 5);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 6);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 7);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 8);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 9);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 10);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 11);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 12);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 13);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 14);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 15);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 16);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 17);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 18);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 19);
    OSMetaClassDeclareReservedUnused(IOPCIATA, 20);
};

#endif // _DRV_PCI_ATA_H
