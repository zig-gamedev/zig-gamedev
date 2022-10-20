/*
 * Copyright (c) 2004-2009 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_SCSI_PORTS_H_
#define _IOKIT_SCSI_PORTS_H_


/*! @header SCSIPort
	@discussion
	This file contains all the definitions for SCSIPort notifications and status.
*/


/*!
@enum kSCSIPort_NotificationStatusChange
@discussion
Message definition to be used with the messageClients
call to inform of a change in port status.
*/
enum
{
	kSCSIPort_NotificationStatusChange		= 0x68000001
};


/*! @typedef SCSIPortStatus
	@abstract 32-bit number to represent a SCSIPortStatus.
	@discussion Type for SCSIPortStatus.
*/

typedef UInt32 SCSIPortStatus;

/*! @enum SCSI Port Status Values
	@discussion Definitions for the possible port status values
*/
enum
{
	/*!
	@constant kSCSIPort_StatusOnline
	The port is online.
	*/
	kSCSIPort_StatusOnline	= 0,

	/*!
	@constant kSCSIPort_StatusOffline
	The port is offline (e.g. unplugged cable).
	*/
	kSCSIPort_StatusOffline	= 1,

	/*!
	@constant kSCSIPort_StatusFailure
	Driver has detected unrecoverable port failure (e.g. hardware port failure)
	*/
	kSCSIPort_StatusFailure	= 2
};

#endif	/* _IOKIT_SCSI_PORTS_H_ */
