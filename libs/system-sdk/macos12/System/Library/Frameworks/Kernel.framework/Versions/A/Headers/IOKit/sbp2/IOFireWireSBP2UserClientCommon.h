/*
 * Copyright (c) 1998-2000 Apple Computer, Inc. All rights reserved.
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

#ifndef _IOKIT_IOFIREWIRESBP2USERCLIENTCOMMON_H_
#define _IOKIT_IOFIREWIRESBP2USERCLIENTCOMMON_H_

#define kIOFireWireSBP2LibConnection 12

enum IOFWSBP2UserClientCommandCodes {
    kIOFWSBP2UserClientOpen,						// kIOUCScalarIScalarO 0,0
    kIOFWSBP2UserClientClose,						// kIOUCScalarIScalarO 0,0
    kIOFWSBP2UserClientCreateLogin,					// kIOUCScalarIScalarO 0,1
    kIOFWSBP2UserClientReleaseLogin,				// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSubmitLogin,					// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSubmitLogout,				// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSetLoginFlags,				// kIOUCScalarIScalarO 2,0
    kIOFWSBP2UserClientGetMaxCommandBlockSize,		// kIOUCScalarIScalarO 1,1
    kIOFWSBP2UserClientGetLoginID,					// kIOUCScalarIScalarO 1,1
    kIOFWSBP2UserClientSetReconnectTime,			// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSetMaxPayloadSize,			// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientCreateORB,					// kIOUCScalarIScalarO 0,1
    kIOFWSBP2UserClientReleaseORB,					// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSubmitORB,					// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSetCommandFlags,  			// kIOUCScalarIScalarO 2,0
    kIOFWSBP2UserClientSetMaxORBPayloadSize, 		// kIOUCScalarIScalarO 2,0
    kIOFWSBP2UserClientSetCommandTimeout, 			// kIOUCScalarIScalarO 2,0
    kIOFWSBP2UserClientSetCommandGeneration, 		// kIOUCScalarIScalarO 2,0
    kIOFWSBP2UserClientSetToDummy,	  				// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSetCommandBuffersAsRanges,	// kIOUCScalarIScalarO 6,0
    kIOFWSBP2UserClientReleaseCommandBuffers, 		// kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientSetCommandBlock,	  			// kIOUCScalarIScalarO 3,0
	kIOFWSBP2UserClientCreateMgmtORB,     			// kIOUCScalarIScalarO 0,1
	kIOFWSBP2UserClientReleaseMgmtORB,   			// kIOUCScalarIScalarO 1,0
	kIOFWSBP2UserClientSubmitMgmtORB,    			// kIOUCScalarIScalarO 1,0
	kIOFWSBP2UserClientMgmtORBSetCommandFunction,   // kIOUCScalarIScalarO 2,0
	kIOFWSBP2UserClientMgmtORBSetManageeORB,  		// kIOUCScalarIScalarO 2,0
	kIOFWSBP2UserClientMgmtORBSetManageeLogin,    	// kIOUCScalarIScalarO 2,0
	kIOFWSBP2UserClientMgmtORBSetResponseBuffer,    // kIOUCScalarIScalarO 3,0
	kIOFWSBP2UserClientLSIWorkaroundSetCommandBuffersAsRanges, // kIOUCScalarIScalarO 6,0
	kIOFWSBP2UserClientMgmtORBLSIWorkaroundSyncBuffersForOutput, // kIOUCScalarIScalarO 1,0
	kIOFWSBP2UserClientMgmtORBLSIWorkaroundSyncBuffersForInput, // kIOUCScalarIScalarO 1,0
    kIOFWSBP2UserClientOpenWithSessionRef,			// kIOUCScalarIScalarO 1,0
	kIOFWSBP2UserClientGetSessionRef,				// kIOUCScalarIScalarO 0,1
	kIOFWSBP2UserClientRingDoorbell,				// kIOUCScalarIScalarO 1, 0
	kIOFWSBP2UserClientEnableUnsolicitedStatus, // kIOUCScalarIScalarO 1, 0
	kIOFWSBP2UserClientSetBusyTimeoutRegisterValue,   // kIOUCScalarIScalarO 2, 0
	kIOFWSBP2UserClientSetORBRefCon, 				// kIOUCScalarIScalarO 2, 0
	kIOFWSBP2UserClientSetPassword,					// kIOUCScalarIScalarO 3, 0
    kIOFWSBP2UserClientSetMessageCallback,   		// kIOUCScalarIScalarO 2, 0
    kIOFWSBP2UserClientSetLoginCallback,			// kIOUCScalarIScalarO 2, 0
    kIOFWSBP2UserClientSetLogoutCallback,			// kIOUCScalarIScalarO 2, 0
    kIOFWSBP2UserClientSetUnsolicitedStatusNotify, 	// kIOUCScalarIScalarO 2, 0
    kIOFWSBP2UserClientSetStatusNotify, 			// kIOUCScalarIScalarO 2, 0
	kIOFWSBP2UserClientSetMgmtORBCallback,  		// kIOUCScalarIScalarO 3, 0
	kIOFWSBP2UserClientSubmitFetchAgentReset,  		// kIOUCScalarIScalarO 3, 0
	kIOFWSBP2UserClientSetFetchAgentWriteCompletion, // kIOUCScalarIScalaO 2, 0
    kIOFWSBP2UserClientNumCommands
};

#endif