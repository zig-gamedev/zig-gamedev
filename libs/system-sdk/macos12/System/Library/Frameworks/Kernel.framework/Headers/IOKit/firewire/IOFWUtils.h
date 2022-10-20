/*
 *  IOFWUtils.h
 *  IOFireWireFamily
 *
 *  Created by Niels on Fri Aug 16 2002.
 *  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 */

#import <IOKit/IOTypes.h>

////////////////////////////////////////////////////////////////////////////////
//
// Useful FireWire utility functions.
//

#ifdef __cplusplus
extern "C" {
#endif

UInt16 FWComputeCRC16(const UInt32 *pQuads, UInt32 numQuads);
UInt16 FWUpdateCRC16(UInt16 crc16, UInt32 quad);

UInt32 AddFWCycleTimeToFWCycleTime( UInt32 cycleTime1, UInt32 cycleTime2 );
UInt32 SubtractFWCycleTimeFromFWCycleTime( UInt32 cycleTime1, UInt32 cycleTime2);

void IOFWGetAbsoluteTime( AbsoluteTime * result );
	
#ifdef __cplusplus
}
#endif

