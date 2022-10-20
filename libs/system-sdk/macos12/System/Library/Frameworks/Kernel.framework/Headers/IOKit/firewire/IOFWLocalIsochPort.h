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
 * IOFWIsochPort is an abstract object that represents hardware on the bus
 * (locally or remotely) that sends or receives isochronous packets.
 * Local ports are implemented by the local device driver,
 * Remote ports are implemented by the driver for the remote device.
 *
 * HISTORY
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.9  2003/08/30 00:16:44  collin
 * *** empty log message ***
 *
 * Revision 1.8  2003/08/15 04:36:55  niels
 * *** empty log message ***
 *
 * Revision 1.7  2003/07/29 22:49:22  niels
 * *** empty log message ***
 *
 * Revision 1.6  2003/07/21 06:52:58  niels
 * merge isoch to TOT
 *
 * Revision 1.5.14.1  2003/07/01 20:54:07  niels
 * isoch merge
 *
 */


#ifndef _IOKIT_IOFWLOCALISOCHPORT_H
#define _IOKIT_IOFWLOCALISOCHPORT_H

#import <IOKit/firewire/IOFireWireFamilyCommon.h>
#import <IOKit/firewire/IOFWIsochPort.h>

class IOFireWireController;
class IODCLProgram;

/*! @class IOFWLocalIsochPort
*/
class IOFWLocalIsochPort : public IOFWIsochPort
{
    OSDeclareDefaultStructors(IOFWLocalIsochPort)

	protected:
	
		IOFireWireController *	fControl;
		IODCLProgram *			fProgram;
	
	/*! @struct ExpansionData
		@discussion This structure will be used to expand the capablilties of the class in the future.
		*/    
		struct ExpansionData
		{
		} ;
	
		ExpansionData *			fExpansion ;

	protected :
	
		virtual void 			free ( void ) APPLE_KEXT_OVERRIDE;

	public:
	
		virtual bool 			init (
										IODCLProgram *			program, 
										IOFireWireController *	control ) ;
	
		// Return maximum speed and channels supported
		// (bit n set = chan n supported)
		virtual IOReturn 		getSupported (
										IOFWSpeed &				maxSpeed, 
										UInt64 &				chanSupported ) APPLE_KEXT_OVERRIDE;
	
		// Allocate hardware resources for port
		virtual IOReturn 		allocatePort (
										IOFWSpeed 				speed, 
										UInt32 					chan ) APPLE_KEXT_OVERRIDE;
		virtual IOReturn 		releasePort ( void ) APPLE_KEXT_OVERRIDE;	// Free hardware resources
		virtual IOReturn 		start ( void ) APPLE_KEXT_OVERRIDE;		// Start port processing packets
		virtual IOReturn 		stop ( void ) APPLE_KEXT_OVERRIDE;		// Stop processing packets
	
		/*! @function notify
			@abstract Informs hardware of a change to the DCL program.
			@param notificationType Type of change.
			@param dclCommandList List of DCL commands that have been changed.
			@param numDCLCommands Number of commands in list.
			@result IOKit error code. */
		virtual IOReturn 		notify(
										IOFWDCLNotificationType 	notificationType,
										DCLCommand ** 				dclCommandList, 
										UInt32 						numDCLCommands ) ;
		static void				printDCLProgram (
										const DCLCommand *		dcl,
										UInt32					count = 0,
										void (*printFN)( const char *format, ...) = NULL,
										unsigned				lineDelayMS = 0 ) ;
		IOReturn				setIsochResourceFlags (
										IOFWIsochResourceFlags	flags ) ;
		IODCLProgram *			getProgramRef() const ;

		IOReturn				synchronizeWithIO() ;

	private:

		OSMetaClassDeclareReservedUnused ( IOFWLocalIsochPort, 0 ) ;
		OSMetaClassDeclareReservedUnused ( IOFWLocalIsochPort, 1 ) ;
};

#endif /* ! _IOKIT_IOFWLOCALISOCHPORT_H */

