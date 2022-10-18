/*
 * Copyright (c) 2000-2008 Apple, Inc. All rights reserved.
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


#ifndef _IOATATYPES_H
#define _IOATATYPES_H

#include <IOKit/IOTypes.h>


/*!

@header IOATAtypes.h
@discussion contains various definitions and constants for use in the IOATAFamily and clients. Header Doc is incomplete at this point, but file is heavily commented.

*/
// property strings
#define kATADevPropertyKey "ata device type"
#define kATATypeATAString "ata"
#define kATATypeATAPIString "atapi"
#define kATATypeUnknownString "unknown"

#define kATAVendorPropertyKey "device model"
#define kATARevisionPropertyKey "device revision"
#define kATASerialNumPropertyKey "device serial"

#define kATAUnitNumberKey "unit number"

#define kATASocketKey "socket type"
#define kATAInternalSocketString "internal"
#define kATAMediaBaySocketString "media-bay"
#define kATAPCCardSocketString "pccard"
#define kATAInternalSATAString "serial-ata"
#define kATASATABayString "sata-bay"
#define kATAInternalSATA2 "serial-ata-2"
#define kATASATA2BayString "sata-2-bay"
#define kATAUnkownSocketString "unknown"

#define kATANotifyOnChangeKey "media-notify"

// allows for porting to non-memory-mapped IO systems, such as x86.
// for such a platform, create a class and overload the assignment operators
// so that the correct IO operation is performed and define the type for that architecture port.
#if defined( __i386__ ) || defined( __x86_64__ )
#include <IOKit/ata/IOATARegI386.h>
#else
#define IOATARegPtr8 volatile UInt8*
#define IOATARegPtr16 volatile UInt16*
#define IOATARegPtr32 volatile UInt32*
#define IOATARegPtr8Cast(x) ((IOATARegPtr8)(x))
#endif

enum ataSocketType{

	kUnknownSocket	= 0,
	kInternalATASocket,
	kMediaBaySocket	,
	kPCCardSocket,
	kInternalSATA,
	kSATABay,
	kInternalSATA2,
	kSATA2Bay

} ;


enum ataDeviceType {

	kUnknownATADeviceType = 0,
	kATADeviceType,
	kATAPIDeviceType

} ;

// enum for bits 5 and 6 of word zero of 
// the identify packet device info data.
// shift word-0 5-bits left, mask 0x03 and these enums apply.
enum atapiConfig {

	kATAPIDRQSlow 	= 0x00,
	kATAPIIRQPacket = 0x01,
	kATAPIDRQFast	= 0x10,
	kATAPIUnknown   = 0x11

};


enum ataUnitID {

	kATAInvalidDeviceID			= -1,
	kATADevice0DeviceID			= 0,							/* aka, Master. Device 0 is the correct terminology */
	kATADevice1DeviceID			= 1								/* aka, Slave. Device 1 is the correct terminology */

} ;

enum {

	kATADefaultSectorSize = 512
};

/* Task file definition *** Error Register *** */
enum {
	bATABadBlock				= 7,							/* bit number of bad block error bit*/
	bATAUncorrectable			= 6,							/* bit number of uncorrectable error bit*/
	bATAMediaChanged			= 5,							/* bit number of media changed indicator*/
	bATAIDNotFound				= 4,							/* bit number of ID not found error bit*/
	bATAMediaChangeReq			= 3,							/* bit number of media changed request*/
	bATACommandAborted			= 2,							/* bit number of command abort bit*/
	bATATrack0NotFound			= 1,							/* bit number of track not found*/
	bATAAddressNotFound			= 0,							/* bit number of address mark not found*/
	mATABadBlock				= 1 << bATABadBlock,			/* Bad Block Detected*/
	mATAUncorrectable			= 1 << bATAUncorrectable,		/* Uncorrectable Data Error*/
	mATAMediaChanged			= 1 << bATAMediaChanged,		/* Media Changed Indicator (for removable)*/
	mATAIDNotFound				= 1 << bATAIDNotFound,			/* ID Not Found*/
	mATAMediaChangeReq			= 1 << bATAMediaChangeReq,		/* Media Change Requested (NOT IMPLEMENTED)*/
	mATACommandAborted			= 1 << bATACommandAborted,		/* Aborted Command*/
	mATATrack0NotFound			= 1 << bATATrack0NotFound,		/* Track 0 Not Found*/
	mATAAddressNotFound			= 1 << bATAAddressNotFound		/* Address Mark Not Found*/
};

/* Task file definition *** Features register *** */
enum {
	bATAPIuseDMA				= 0,							/* bit number of useDMA bit (ATAPI)*/
	mATAPIuseDMA				= 1 << bATAPIuseDMA
};

/* Task file definition *** ataTFSDH Register *** */
enum {
	mATAHeadNumber				= 0x0F,							/* Head Number (bits 0-3) */
	mATASectorSize				= 0xA0,							/* bit 7=1; bit 5 = 01 (512 sector size) <DP4>*/
	mATADriveSelect				= 0x10,							/* Drive (0 = master, 1 = slave) */
	mATALBASelect				= 0x40							/* LBA mode bit (0 = chs, 1 = LBA)*/
};

/* Task file definition *** Status Register *** */
enum {
	bATABusy					= 7,							/* bit number of BSY bit*/
	bATADriveReady				= 6,							/* bit number of drive ready bit*/
	bATAWriteFault				= 5,							/* bit number of write fault bit*/
	bATASeekComplete			= 4,							/* bit number of seek complete bit*/
	bATADataRequest				= 3,							/* bit number of data request bit*/
	bATADataCorrected			= 2,							/* bit number of data corrected bit*/
	bATAIndex					= 1,							/* bit number of index mark*/
	bATAError					= 0,							/* bit number of error bit*/
	mATABusy					= 1 << bATABusy,				/* Unit is busy*/
	mATADriveReady				= 1 << bATADriveReady,			/* Unit is ready*/
	mATAWriteFault				= 1 << bATAWriteFault,			/* Unit has a write fault condition*/
	mATASeekComplete			= 1 << bATASeekComplete,		/* Unit seek complete*/
	mATADataRequest				= 1 << bATADataRequest,			/* Unit data request*/
	mATADataCorrected			= 1 << bATADataCorrected,		/* Data corrected*/
	mATAIndex					= 1 << bATAIndex,				/* Index mark - NOT USED*/
	mATAError					= 1 << bATAError				/* Error condition - see error register*/
};

/* Task file definition *** Device Control Register *** */
enum {
	bATADCROne					= 3,							/* bit number of always one bit*/
	bATADCRReset				= 2,							/* bit number of reset bit*/
	bATADCRnIntEnable			= 1,							/* bit number of interrupt disable*/
	mATADCROne					= 1 << bATADCROne,				/* always one bit*/
	mATADCRReset				= 1 << bATADCRReset,			/* Reset (1 = reset)*/
	mATADCRnIntEnable			= 1 << bATADCRnIntEnable		/* Interrupt Disable(0 = enabled)*/
};


/* 'ataRegMask' field of the ataRegAccess definition*/
enum ataRegMask{

	bATAAltSDevCValid			= 14,							/* bit number of alternate status/device cntrl valid bit*/
	bATAStatusCmdValid			= 7,							/* bit number of status/command valid bit*/
	bATASDHValid				= 6,							/* bit number of ataTFSDH valid bit*/
	bATACylinderHiValid			= 5,							/* bit number of cylinder high valid bit*/
	bATACylinderLoValid			= 4,							/* bit number of cylinder low valid bit*/
	bATASectorNumValid			= 3,							/* bit number of sector number valid bit*/
	bATASectorCntValid			= 2,							/* bit number of sector count valid bit*/
	bATAErrFeaturesValid		= 1,							/* bit number of error/features valid bit*/
	bATADataValid				= 0,							/* bit number of data valid bit*/
	mATAAltSDevCValid			= 1 << bATAAltSDevCValid,		/* alternate status/device control valid*/
	mATAStatusCmdValid			= 1 << bATAStatusCmdValid,		/* status/command valid*/
	mATASDHValid				= 1 << bATASDHValid,			/* ataTFSDH valid*/
	mATACylinderHiValid			= 1 << bATACylinderHiValid,		/* cylinder high valid*/
	mATACylinderLoValid			= 1 << bATACylinderLoValid,		/* cylinder low valid*/
	mATASectorNumValid			= 1 << bATASectorNumValid,		/* sector number valid*/
	mATASectorCntValid			= 1 << bATASectorCntValid,		/* sector count valid*/
	mATAErrFeaturesValid		= 1 << bATAErrFeaturesValid,	/* error/features valid*/
	mATADataValid				= 1 << bATADataValid			/* data valid*/

} ;


enum ataFlags{
	bATAFlagQuiesce				= 20,
	bATAFlagNoIRQ				= 19,							/* bit Number of no IRQ protocol flag*/
	bATAFlag48BitLBA			= 18,
	bATAFlagDMAQueued			= 17,
	bATAFlagOverlapped			= 16,
	bATAFlagUseConfigSpeed		= 15,							/* bit number of use configured speed flag*/
	bATAFlagByteSwap			= 14,							/* bit number of byte swap flag*/
	bATAFlagIORead				= 13,							/* bit number of I/O read flag*/
	bATAFlagIOWrite				= 12,							/* bit number of I/O write flag*/
	bATAFlagTFAccessResult 		= 8,							/* bit number of get register results on command completion.*/
	bATAFlagUseDMA				= 7,							/* bit number of use DMA flag*/
	bATAFlagProtocolATAPI		= 5,							/* bit number of ATAPI protocol*/
	bATAFlagImmediate			= 1,							/* bit number of immediate flag */
	bATAFlagTFAccess			= 0,							/* bit number of TF access */

	mATAFlagQuiesce				= 1 << bATAFlagQuiesce,
	mATAFlagUseNoIRQ			= 1 << bATAFlagNoIRQ,  			/* Special purpose! Avoid using! No-IRQ, polled synchronous protocol valid only for PIO commands*/
	mATAFlag48BitLBA			= 1 << bATAFlag48BitLBA,		/* Use 48 bit extended LBA protocol on this command. Requires support from the controller.*/
	mATAFlagDMAQueued			= 1 << bATAFlagDMAQueued,		/* Use tagged dma queuing protocol on this command. Requires support from the controller.*/
	mATAFlagOverlapped			= 1 << bATAFlagOverlapped,		/* Use overllaped protocol on this command. Requires support from the controller.*/
	mATAFlagUseConfigSpeed		= 1 << bATAFlagUseConfigSpeed,  /* Use the configured interface speed = true. False = use default PIO (slow) speed. valid only for PIO commands*/
	mATAFlagByteSwap			= 1 << bATAFlagByteSwap,		/* Swap data bytes (read - after; write - before)*/
	mATAFlagIORead				= 1 << bATAFlagIORead,			/* Read (in) operation*/
	mATAFlagIOWrite				= 1 << bATAFlagIOWrite,			/* Write (out) operation*/
	mATAFlagTFAccessResult		= 1 << bATAFlagTFAccessResult,  /* get contents of TaskFile registers indicated in TFMask on command completion, even if no error*/
	mATAFlagUseDMA				= 1 << bATAFlagUseDMA,
	mATAFlagProtocolATAPI		= 1 << bATAFlagProtocolATAPI,	/* ATAPI protocol indicator*/
	mATAFlagImmediate			= 1 << bATAFlagImmediate,		/* Put command at head of queue */
	mATAFlagTFAccess			= 1 << bATAFlagTFAccess,		/* Return Taskfile on error status*/

} ;

/* The Function codes sent to controllers*/
enum ataOpcode {

	kATANoOp = 0, 
	kATAFnExecIO ,		/* Execute ATA I/O */
	kATAPIFnExecIO,		/* ATAPI I/O */
	kATAFnRegAccess ,	/* Register Access */

	kATAFnQFlush ,		/* I/O Queue flush requests for your unit number */
	kATAFnBusReset 	/* Reset ATA bus */

} ;


/* The ATA Event codes */
/* sent when calling the device driver's event handler*/
enum ataEventCode {
	kATANullEvent				= 0x00,							/* Just kidding -- nothing happened*/
	kATAOnlineEvent				= 0x01,							/* An ATA device has come online*/
	kATAOfflineEvent			= 0x02,							/* An ATA device has gone offline*/
	kATARemovedEvent			= 0x03,							/* An ATA device has been removed from the bus*/
	kATAResetEvent				= 0x04,							/* Someone gave a hard reset to the drive*/
	kATAOfflineRequest			= 0x05,							/* Someone requesting to offline the drive*/
	kATAEjectRequest			= 0x06,							/* Someone requesting to eject the drive*/
	kATAPIResetEvent			= 0x07,							/* Someone gave a ATAPI reset to the drive*/
	kATAReservedEvent			= 0x80							/* RESERVED*/
};


// These need to be combined with a new enumeration of the current ATA/ATAPI command set.
// Some opcodes are of interest to ATA controllers, since they imply special protocols 
// or handling.   Device Reset, Execute Device Diagnostics have subtle side effects that
// controllers need to be aware of, so we snoop for those commands being issued.
// the rest are here for informational purposes.

// BUG make new enum for all current ATA commands.

enum {
	kSOFTRESET	=	0x008,			// ATAPI Soft Reset command
	kPACKET		=	0x0A0,			// ATAPI Packet command
	kID_DRIVE	=	0x0A1			// ATAPI Identify drive command
};
/* ATA Command Opcode definition*/
enum {
	kATAcmdWORetry				= 0x01,							/* Without I/O retry option*/
	kATAcmdNOP					= 0x0000,						/* NOP operation - media detect*/
	kATAcmdRecal				= 0x0010,						/* Recalibrate command */
	kATAcmdRead					= 0x0020,						/* Read command */
	kATAcmdReadLong				= 0x0022,						/* Read Long command*/
	kATAcmdReadExtended			= 0x0024,						/* Read Extended (with retries)*/
	kATAcmdReadDMAExtended		= 0x0025,						/* Read DMA Extended (with retries)*/
	kATAcmdWrite				= 0x0030,						/* Write command */
	kATAcmdWriteLong			= 0x0032,						/* Write Long*/
	kATAcmdWriteExtended		= 0x0034,						/* Write Extended (with retries)*/
	kATAcmdWriteDMAExtended		= 0x0035,						/* Write DMA Extended (with retries)*/
	kATAcmdWriteVerify			= 0x003C,						/* Write verify*/
	kATAcmdReadVerify			= 0x0040,						/* Read Verify command */
	kATAcmdFormatTrack			= 0x0050,						/* Format Track command */
	kATAcmdSeek					= 0x0070,						/* Seek command */
	kATAcmdDiagnostic			= 0x0090,						/* Drive Diagnostic command */
	kATAcmdInitDrive			= 0x0091,						/* Init drive parameters command */
	kATAcmdReadMultiple			= 0x00C4,						/* Read multiple*/
	kATAcmdWriteMultiple		= 0x00C5,						/* Write multiple*/
	kATAcmdSetRWMultiple		= 0x00C6,						/* Set Multiple for Read/Write Multiple*/
	kATAcmdReadDMA				= 0x00C8,						/* Read DMA (with retries)*/
	kATAcmdWriteDMA				= 0x00CA,						/* Write DMA (with retries)*/
	kATAcmdMCAcknowledge		= 0x00DB,						/* Acknowledge media change - removable*/
	kATAcmdDoorLock				= 0x00DE,						/* Door lock*/
	kATAcmdDoorUnlock			= 0x00DF,						/* Door unlock*/
	kATAcmdStandbyImmed			= 0x00E0,						/* Standby Immediate*/
	kATAcmdIdleImmed			= 0x00E1,						/* Idle Immediate*/
	kATAcmdStandby				= 0x00E2,						/* Standby*/
	kATAcmdIdle					= 0x00E3,						/* Idle*/
	kATAcmdReadBuffer			= 0x00E4,						/* Read sector buffer command */
	kATAcmdCheckPowerMode		= 0x00E5,						/* Check power mode command	<04/04/94>*/
	kATAcmdSleep				= 0x00E6,						/* Sleep*/
	kATAcmdFlushCache			= 0x00E7,						/* Flush Cache */
	kATAcmdWriteBuffer			= 0x00E8,						/* Write sector buffer command */
	kATAcmdWriteSame			= 0x00E9,						/* Write same data to multiple sectors*/
	kATAcmdFlushCacheExtended	= 0x00EA,						/* Flush Cache Extended */
	kATAcmdDriveIdentify		= 0x00EC,						/* Identify Drive command */
	kATAcmdMediaEject			= 0x00ED,						/* Media Eject*/
	kATAcmdSetFeatures			= 0x00EF						/* Set Features*/
};

/* Set feature command opcodes*/
enum {
	kATAEnableWriteCache		= 0x02,							/*		Enable write cache*/
	kATASetTransferMode			= 0x03,							/*		Set transfer mode*/
	kATAEnableAPM				= 0x05,							/* 		Enable Advanced Power Management*/
	kATASetPIOMode				= 0x08,							/*		PIO Flow Control Tx Mode bit*/
	kATADisableWriteCache		= 0x82,							/*		disable write cache*/
	kATAEnableReadAhead			= 0xAA							/*		Read look-ahead enable*/
};

// revisit the opcode enumerations.

//////////////////////


/* task file for ata */
typedef struct ataTaskFile {

	UInt8 							ataTFFeatures;				/* <-> Error(R) or ataTFFeatures(W) register image */
	UInt8 							ataTFCount;					/* <-> Sector count/remaining */
	UInt8 							ataTFSector;				/* <-> Sector start/finish */
	UInt8 							ataTFCylLo;					/* <-> ataTFCylLo					*/
	UInt8 							ataTFCylHigh;				/* <-> ataTFCylHigh  */
	UInt8 							ataTFSDH;					/* <-> ataTFSDH register image*/
	UInt8 							ataTFCommand;				/* <-> Status(R) or Command(W) register image */

} ataTaskFile;


typedef struct ataRegisterImage {

	ataTaskFile						taskFile;
	UInt16							ataDataRegister;			/* <-> Data register. */
	UInt8 							ataAltSDevCReg;				/* <->: Alternate status(R) or Device Control(W) register image*/

} ataRegisterImage ;


typedef struct ATAPICmdPacket{

	UInt16 							atapiPacketSize;			/* Size of command packet in bytes	*/
	UInt16 							atapiCommandByte[8];		/* The command packet itself*/

}ATAPICmdPacket;



// Error and result codes:  TBD
enum {
		kATAErrUnknownType = -1,
		kATANoErr = 0,
		kATAQueueEmpty = 1,
		kATAUnknownOpcode,
		kATATimeoutErr,
		kATAInvalidDevID,
		kATAErrDevBusy,
		kATAModeNotSupported,
		kATADevIntNoCmd,
		kATADeviceError,
		kATADMAErr
};


#endif /* !_IOATATYPES_H */
