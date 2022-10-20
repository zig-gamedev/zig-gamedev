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


#ifndef _IOATADEVCONFIG_H
#define _IOATADEVCONFIG_H

#include <libkern/c++/OSObject.h>
#include <IOKit/IOTypes.h>
#include "IOATATypes.h"
#include "IOATABusInfo.h"


/*!
	@class IOATADevConfig 
	@abstract used for configuring and communicating the desired transfer modes of a device.
	 A disk driver would typically use this object in conjunction with the 512-bytes of identification 
	 data from the drive and the IOATABusInfo object for the bus it is connected to.
	 This object will determine the best matching transfer speeds available.
	 the device driver will then send a series of Set Features commands to configure the drive
	 and this object to the bus through the IOATADevice nub in order to configure the optimum transfer mode.
	 The driver for the disk drive may choose to populate this object with whatever transfer mode 
	 desired, in the event that a different mode is required.
	@discussion usually use the initWithBestSelection to make a best mode match.
	The Mode accessors use bit significance to indicate a selected mode or supported modes(s)
	ie, 00000001b indicates Mode-0, 00000010b indicates mode 1, etc.
	Selected mode is indicated by a single set bit. No bit set indicates no mode in that class is selected.
	ie, a bus will support multiple possible modes, but will only have one mode selected at that time.

*/

class IOATADevConfig : public OSObject {

	OSDeclareDefaultStructors( IOATADevConfig );
	
	public:
	
	/*!@function atadevconfig
	@abstract static creator function.
	*/
    static	IOATADevConfig* atadevconfig(void);
    
   /*!@function initWithBestSelection
	@abstract Handy initializer: pass the 512-byte result of the Identify Device or 
    Identify Packet Device in endian-order for your platform (byte-swapped on PPC) 
    and the IOATABusInfo object for the bus. The object will initialize all fields 
    and select the best transfer modes that match on bus and device. 
    If the return value was 0 (success or noErr), then a matching mode is supported.
    Examine the PIO and UDMA/DMA fields and to generate the apropriate SET FEATURES 
    parameters for your drive and send this initialised object to the IOATAController 
    when requesting a speed configuration.
    failure means no supported transfer modes matched between bus and device info.
	
	@param identifyData 512 bytes of data obtained from the device via IDENTIFY DEVICE or IDENTIFY PACKET DEVICE command.
	@param busInfo pointer to an IOATAbusInfo object obtained from a previous atanub->provideBusInfo() call.
	@result kIOSuccess (0) when a matching transfer mode is available between the device and controller.
	*/
	IOReturn	initWithBestSelection( const UInt16* identifyData, IOATABusInfo* busInfo);
    
    // intitialize with the 512 byte data from an ATA device identify command
	IOReturn assignFromData( const UInt16* identifyData );

	/*!@function setPacketConfig
	@param packetConfig
	@abstract 	For ATAPI devices, if the device asserts interrupt after the Packet Command when it is ready to accept the packet, set this value to true (mostly older devices). If the device accepts the packet only by asserting DRQ bit in status, then set this value false. Tells the bus controller whether to wait for packet acceptance or set pending interrupt.
	*/
	void setPacketConfig ( atapiConfig packetConfig);

	/*!@function getPacketConfig
	@result atapiConfig as defined in IOATATypes.h 
	*/
	atapiConfig getPacketConfig( void );	

	// The following Mode accessors use bit significance to indicate a selected mode or supported modes(s)
	// ie, 00000001b indicates Mode-0, 00000010b indicates mode 1, etc.
	// Selected mode is indicated by a single set bit. No bit set indicates no mode in that class is selected.
	// ie, a bus will support multiple possible modes, but will only have one mode selected at that time.

	/*!@function setPIOMode
	@param inModeBitMap bit-significant map of PIO mode
	*/
	void setPIOMode( UInt8 inModeBitMap);		// bit-significant map of PIO mode(s)
	
	/*!@function getPIOMode
	@result bit-significant map of PIO mode
	*/
	UInt8 getPIOMode( void );
	
	/*!@function setDMAMode
	@param inModeBitMap bit-significant map of DMA mode
	*/
	void setDMAMode( UInt8 inModeBitMap );	// bit-significant map of DMA mode(s)
	
	/*!@function getDMAMode
	@result bit-significant map of DMA mode
	*/
	UInt8 getDMAMode( void );
	
	/*!@function setUltraMode
	@param inModeBitMap bit-significant map of Ultra mode
	*/
	void setUltraMode( UInt8 inModeBitMap );	// bit-significant map of Ultra mode(s)

	/*!@function getUltraMode
	@result  bit-significant map of Ultra mode
	*/
	UInt8 getUltraMode( void );
	
	// The following cycle time accessors report cycle times in nanoseconds.
	// A device requesting a mode should also request a cycle time as reported in the device's identification page.
	// A bus controller will configure the bus not to exceed (go faster than) the minimum time requested.
	// When a bus controller reports a mode configuration, the cycle time indicates the busses contract not to go 
	// faster than. However an actual cycle time may be slower than the indicated time.	
	/*!@function setPIOCycleTime
	@param inNS PIO cycle time in nanoseconds. 
	*/
	void setPIOCycleTime( UInt16 inNS );
	
	/*!@function getPIOCycleTime
	@result reported PIO CycleTime in nanoseconds. 
	*/
	UInt16 getPIOCycleTime( void );

	/*!@function setDMACycleTime
	@param inNS reported multiword DMA Cycle time in nanoseconds.
	*/
	void setDMACycleTime( UInt16 inNS );
	
	/*!@function getDMACycleTime
	@result Reported multiword DMA cycle time in nanoseconds. 
	*/
	UInt16 getDMACycleTime( void );
	
	// Ultra ATA defines cycle times a device must meet to comply with standards.
	// No cycle time field is needed.

	// convert bit-significant to numeric value
	/*!@function bitSigToNumeric
	@abstract converts a bit-significant field to a numerical value. Note that a bit field of 0x00 has no defined result.
	@param binary the bit significant field.
	@result the numerical value of the highest bit set in the field.
	*/
	virtual UInt8 bitSigToNumeric( UInt16 binary);

	
	protected:
	
	atapiConfig 					_atapiIRQForPacket;			// enum for bits 5 and 6 of word zero of 
																// the identify packet device info data for PACKET devices ONLY.
																// shift word-0 5-bits left, mask 0x03 and these enums apply.
																// values are:
																
																//	kATAPIDRQSlow 	= 0x00   - wait up to 3MS for packet on DRQ
																//	kATAPIIRQPacket = 0x01,  - device asserts IRQ for packet
																//	kATAPIDRQFast	= 0x10,  - packet on DRQ within 50us
																//	kATAPIUnknown   = 0x11   - reserved as of ATA/ATAPI-5




	UInt8 							_ataPIOMode;					/* <->: PIO Mode Timing class bit-significant */
	UInt16 							_ataPIOCycleTime;			/* <->: Cycle time in ns for PIO mode */
	UInt8 							_ataMultiDMAMode;			/* <->: Multiple Word DMA Timing Class bit-significant*/
	UInt16 							_ataMultiCycleTime;			/* <->: Cycle time in ns for Multiword DMA mode */
	UInt8 							_ataUltraDMAMode;			/* <->: Ultra DMA timing class bit-significant */

	IOReturn						_AssignPIOData( const UInt16* identifyData);
	IOReturn						_AssignDMAData(const UInt16* identifyData);
	IOReturn						_AssignUltraData(const UInt16* identifyData);
	UInt8							_MostSignificantBit( UInt8 inByte);


    virtual bool init();  
protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the IOWorkLoop in the future.
    */
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

private:
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 0);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 1);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 2);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 3);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 4);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 5);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 6);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 7);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 8);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 9);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 10);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 11);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 12);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 13);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 14);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 15);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 16);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 17);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 18);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 19);
    OSMetaClassDeclareReservedUnused(IOATADevConfig, 20);
	
public:

//some static utility functions to parse the identify data for feature support

  static bool sDriveSupports48BitLBA( const UInt16* identifyData );
  static UInt32 sDriveExtendedLBASize( UInt32* lbaHi, UInt32* lbaLo, const UInt16* identifyData); // result returned is same as lbaLo. 
	
};


// header doc info goes here. I find putting it within the delcarations more confusing than putting it all in the bottom of the header.


#endif /* !_IOATADEVCONFIG_H */
