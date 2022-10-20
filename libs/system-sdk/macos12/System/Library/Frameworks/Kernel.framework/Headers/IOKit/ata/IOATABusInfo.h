/*
 * Copyright (c) 2000-2008 Apple Inc. All rights reserved.
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


#ifndef _IOATABUSINFO_H
#define _IOATABUSINFO_H

#include <libkern/c++/OSObject.h>
#include <IOKit/IOTypes.h>
#include "IOATATypes.h"

/*!
@class IOATABusInfo
@discussion used to indicate the capabilities of the bus the device is connected to, PIO and DMA modes supported, etc.
*/

class IOATABusInfo : public OSObject
{
	OSDeclareDefaultStructors( IOATABusInfo );
	
	public:
	
	/*!@function atabusinfo
	@abstract factory method
	*/
	static IOATABusInfo* atabusinfo(void);
	
	/*!@function zeroData
	@abstract set this object to a blank state.
	*/
	virtual void zeroData(void);

	// Used by clients of ATAControllers to find out about the bus
	// capability.

	/*!@function getSocketType
	@abstract returns the socket type, internal fixed, media-bay, PC-Card
	Used by clients of ATAControllers to find out about the bus
	*/
	ataSocketType getSocketType( void );  

	/*!@function getPIOModes
	@abstract returns the bit-significant map of PIO mode(s) supported on the bus. 
	Used by clients of ATAControllers to find out about the bus.
	*/
	UInt8 getPIOModes( void );
	
	/*!@function getDMAModes
	@abstract bit-significant map of DMA mode(s) supported on the bus. 
	Used by clients of ATAControllers to find out about the bus.
	*/
	UInt8 getDMAModes( void );
	
	/*!@function getUltraModes
	@abstract bit-significant map of Ultra mode(s) supported on the bus. 
	Used by clients of ATAControllers to find out about the bus.
	*/
	UInt8 getUltraModes( void );
	
	/*!@function getUnits
	@abstract How many devices are present on bus. 
	Used by clients of ATAControllers to find out about the bus.
	*/
	UInt8 getUnits( void );	 

	/*!@function supportsDMA
	@abstract  True = DMA supported on bus - inferred by looking at the DMA mode bits.
	Used by clients of ATAControllers to find out about the bus.
	*/
	bool supportsDMA( void );   		
	
	/*!@function supportsExtendedLBA
	@abstract Supports 48-bit LBA if true.
	Used by clients of ATAControllers to find out about the bus.
	*/
	bool supportsExtendedLBA( void );
	
	/*!@function maxBlocksExtended
	@abstract The maximum number of 512-byte blocks this controller supports
	in a single Extended LBA transfer. Some controllers may be limited to less than 
	the maximum sector count allowed under extended LBA protocol. 
	*/
	UInt16 maxBlocksExtended(void);
	
	/*!@function supportsOverlapped
	@abstract Supports overlapped packet feature set if true. 
	Used by clients of ATAControllers to find out about the bus.
	*/
	bool supportsOverlapped( void );  
	
	/*!@function supportsDMAQueued
	@abstract Supports DMA Queued Feature set if true. 
	Used by clients of ATAControllers to find out about the bus.
	*/
	bool supportsDMAQueued( void ); 
	
	
	// Used by ATAControllers to generate an information object.
	// Would not be used by disk device drivers normally.
	// Bus controllers should set these items everytime.
	
	/*!@function setSocketType
	@abstract internal fixed, media-bay, PC-Card. Set by ATAControllers.
	*/
	void setSocketType( ataSocketType inSocketType ); 
	
	/*!@function setPIOModes
	@abstract Bit significant map of supported transfer modes. Set by ATAControllers.
	*/
	void setPIOModes( UInt8 inModeBitMap);		
	
	/*!@function setDMAModes
	@abstract Bit significant map of supported transfer modes. Set by ATAControllers.
	*/
	void setDMAModes( UInt8 inModeBitMap );	
	
	/*!@function setUltraModes
	@abstract Bit significant map of supported transfer modes. Set by ATAControllers.
	*/
	void setUltraModes( UInt8 inModeBitMap );	
	
	/*!@function setUnits
	@abstract set to indicate how many devices are on this bus. Set by ATAControllers.
	*/
	void setUnits( UInt8 inNumUnits );

	// Optional bus protocols some busses may support
	/*!@function setExtendedLBA
	@abstract Set true for supports 48-bit LBA. Set by ATAControllers.
	*/
	void setExtendedLBA( bool	inState ); 	
	
	/*!function setMaxBlocksExtended 
	 @abstract value set by controllers to indicate the maximum number of blocks 
	 allowed in a single transfer of data. Some dma engines may not be capable of supporting the full 
	 16-bit worth of sector count allowed under 48 bit extended LBA. Default is 256 blocks, same as 
	 standard ATA. 
	 */
	void setMaxBlocksExtended( UInt16 inMaxBlocks);
	
	/*!@function setOverlapped
	@abstract Set true for supports overlapped packet feature set. Set by ATAControllers.
	*/
	void setOverlapped( bool inState);  	  
	
	/*!@function setDMAQueued
	@abstract Set true if supports DMA Queued Feature. Set by ATAControllers.
	*/
	void setDMAQueued( bool inState);   	//    

	protected:

	UInt8 							_PIOModes;				/* PIO modes supported (bit-significant) */
	UInt8 							_MultiDMAModes;			/* <--: Multiword DMA modes supported (b-sig) */
	UInt8 							_UltraDMAModes;			/* <--: Ultra DMA modes supported (b-sig) */
	bool							_ExtendedLBA;           /* <--: Suppports 48-bit LBA protocol */
	bool							_Overlapped;			/* <--: Supports overlapped packet feature set */
	bool							_DMAQueued;				/* <--: Supports DMA Queued Feature set */
	ataSocketType					_SocketType;			/* <--: Indicates bus is fixed internal, removable media-bay, removable PC-Card or unknown type */
	UInt8							_numUnits;				/* <--: How many devices on this bus */
	UInt16							_maxBlocksExtended;
	
	protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOWorkLoop in the future.
    */
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;
 
	virtual bool init(); 

private:
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 0);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 1);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 2);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 3);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 4);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 5);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 6);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 7);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 8);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 9);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 10);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 11);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 12);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 13);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 14);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 15);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 16);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 17);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 18);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 19);
    OSMetaClassDeclareReservedUnused(IOATABusInfo, 20);
};


#endif /* !_IOATABUSINFO_H */
