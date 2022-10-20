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
 * A DCL program to interpret (in software) a program that's too complicated
 * for the DMA engine.
 *
 * HISTORY
 *
 */


#ifndef _IOKIT_IOFWDCLTRANSLATOR_H
#define _IOKIT_IOFWDCLTRANSLATOR_H

#include <libkern/c++/OSObject.h>
#include <IOKit/firewire/IOFWDCLProgram.h>


/*! @class IODCLTranslator
*/

class IODCLTranslator : public IODCLProgram
{
    OSDeclareAbstractStructors(IODCLTranslator)

protected:
    enum
    {
        kNumPingPongs				= 2,
        kNumPacketsPerPingPong		= 500,
        kMaxIsochPacketSize			= 1000,
        kPingPongBufferSize			= kNumPingPongs * kNumPacketsPerPingPong * kMaxIsochPacketSize
    };

    // Opcodes and buffer for pingpong program
    DCLLabel			fStartLabel;
    DCLTransferPacket	fTransfers[kNumPingPongs*kNumPacketsPerPingPong];
    DCLCallProc			fCalls[kNumPingPongs];
    DCLJump				fJumpToStart;
    UInt8				fBuffer[kPingPongBufferSize];

    IODCLProgram *		fHWProgram;				// Hardware program executing our opcodes
    DCLCommand*			fToInterpret;			// The commands to interpret
    DCLCommand*			fCurrentDCLCommand;		// Current command to interpret
    int					fPingCount;				// Are we pinging or ponging?
    UInt32				fPacketHeader;

    static void ListeningDCLPingPongProc(DCLCommand* pDCLCommand);
    static void TalkingDCLPingPongProc(DCLCommand* pDCLCommand);

public:
    virtual bool init(DCLCommand* toInterpret);
    virtual IOReturn allocateHW(IOFWSpeed speed, UInt32 chan) APPLE_KEXT_OVERRIDE;
    virtual IOReturn releaseHW() APPLE_KEXT_OVERRIDE;
    virtual IOReturn notify(IOFWDCLNotificationType notificationType,
	DCLCommand** dclCommandList, UInt32 numDCLCommands) APPLE_KEXT_OVERRIDE;
    virtual void stop() APPLE_KEXT_OVERRIDE;

    DCLCommand* getTranslatorOpcodes();
    void setHWProgram(IODCLProgram *program);
};

/*! @class IODCLTranslateTalk
*/

class IODCLTranslateTalk : public IODCLTranslator
{
    OSDeclareDefaultStructors(IODCLTranslateTalk)

protected:

public:
    virtual IOReturn compile(IOFWSpeed speed, UInt32 chan) APPLE_KEXT_OVERRIDE;
    virtual IOReturn start() APPLE_KEXT_OVERRIDE;

};

/*! @class IODCLTranslateListen
*/

class IODCLTranslateListen : public IODCLTranslator
{
    OSDeclareDefaultStructors(IODCLTranslateListen)

protected:

public:
    virtual IOReturn compile(IOFWSpeed speed, UInt32 chan) APPLE_KEXT_OVERRIDE;
    virtual IOReturn start() APPLE_KEXT_OVERRIDE;

};
#endif /* ! _IOKIT_IOFWDCLPROGRAM_H */

