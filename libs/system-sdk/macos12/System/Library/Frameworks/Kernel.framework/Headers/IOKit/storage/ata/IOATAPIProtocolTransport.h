/*
 * Copyright (c) 2000-2006 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
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

/*!
  @header IOATAPIProtocolTransport
  Contains the class definition for IOATAPIProtocolTransport.
*/


#ifndef _IOKIT_IO_ATAPI_PROTOCOL_TRANSPORT_H_
#define _IOKIT_IO_ATAPI_PROTOCOL_TRANSPORT_H_

#define kIOPropertyATAPIMassStorageCharacteristics 		"ATAPI Mass Storage Characteristics"

#if defined(KERNEL) && defined(__cplusplus)

/* General IOKit includes */
#include <IOKit/IOLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOService.h>
#include <IOKit/IOCommandPool.h>

/* IOKit ATA Family includes */
#include <IOKit/ata/IOATADevice.h>
#include <IOKit/ata/IOATATypes.h>

/* IOKit ATA Storage includes */
#include <IOKit/storage/ata/IOATAStorageDefines.h>

/* SCSI Architecture Model Family includes */
#include <IOKit/scsi/IOSCSIProtocolServices.h>

// Forward class declaration
class IOATAPIProtocolTransport;

/*! 
	@typedef ATAPIClientData
	@param cmd IOATACommand for request.
	@param self Pointer to the object. 
	@param scsiTask SCSITaskIdentifier of request. 
	@discussion This structure is stuffed into the refcon so we can associate which
	IOATACommand and SCSITaskIdentifier is completing.
*/

struct ATAPIClientData
{
	IOATACommand *				cmd;
	IOATAPIProtocolTransport *	self;
	SCSITaskIdentifier			scsiTask;
};
typedef struct ATAPIClientData ATAPIClientData;


/*!
	@class IOATAPIProtocolTransport
	@abstract SCSI Protocol Driver Family for ATAPI Devices. 
	@discussion IOATAPIProtocolTransport contains all the bus specific support for ATAPI compliant devices.
	To add vendor specific features or workarounds you will sub-class the appropriate
	methods of this family. 
*/

class IOATAPIProtocolTransport : public IOSCSIProtocolServices
{
	
	OSDeclareDefaultStructors ( IOATAPIProtocolTransport )
	
public:

	// ---- IOService methods overridden ----
	
	bool 			init 	( OSDictionary * propTable );
	
	// The start method is called to start our services. If the device
	// can be controlled by this object, then we return true, else we
	// return false. start() is usually used to allocate resources once
	// it is determined that the device can be controlled by this object.
	virtual bool 	start 	( IOService * provider );
	
	// The stop method is called to stop our services. It is primarily
	// called for deallocation of resources.
	virtual void 	stop 	( IOService *  provider );
	
	// This method is our last chance to free all resources allocated.
	virtual void	free	( void );
	
	
protected:

	// ---- member variables ----
	
	IOATADevice *			fATADevice;
	ataUnitID				fATAUnitID;
	ataDeviceType			fATADeviceType;
	ataSocketType			fATASocketType;
	atapiConfig				fATAPIPacketConfig;
	UInt8					fPIOMode;
	UInt8					fDMAMode;
	UInt8					fUltraDMAMode;
	IOCommandGate *			fCommandGate;
	IOCommandPool *			fCommandPool;
	IOATACommand *			fResetCommand;
	IOATACommand *			fConfigCommand;
	IOATACommand *			fIdentifyCommand;
	char					fRevision[kSizeOfATARevisionString + 1];
	char					fModel[kSizeOfATAModelString + 1];
	bool					fWakeUpResetOccurred;
	bool					fPhysicallyConnected;
	
	// Used for low-power polling support
	thread_call_t			fPollingThread;
	
	UInt16					fDeviceIdentifyData[256];
	IOMemoryDescriptor *	fDeviceIdentifyBuffer;
	bool					fResetInProgress;

	// Binary Compatibility instance variable expansion
	struct ExpansionData
	{
		UInt32		fSemaphore;
		UInt32		fMediaNotifyValue;
	};
	ExpansionData * reserved;
	
	// ---- IOService methods overridden ----
	
	// The message method is used to listen to messages from our provider, the ATA controller.
	// It sends messages for bus reset notifications and for device removal (such as MediaBay,
	// PC card, etc.)
	virtual IOReturn message ( UInt32 type, IOService * provider, void * argument = 0 );
	
	// ---- Protocol transport methods overridden ----

	// Send a SCSI Command to the device.  If the command was sent to the
	// device and is pending completion, the subclass should return true and
	// return back the kSCSIServiceResponse_Request_In_Process response. 
	// If the command completes immediately with an error, the subclass will
	// return true and return back the appropriate status.
	// if the subclass is currently processing all the commands it can, the
	// subclass will return false and the command will be resent next time
	// CommandCompleted is called.  
	virtual bool SendSCSICommand ( 	SCSITaskIdentifier 		request,
									SCSIServiceResponse * 	serviceResponse,
									SCSITaskStatus *		taskStatus );
		
	// This is a stub - not implemented in upper layer yet. Eventually, there
	// will be a way to abort a task or task set and this method will be called.
	// It will abort any commands which have not been executed by the ATA controller
	virtual SCSIServiceResponse AbortSCSICommand ( SCSITaskIdentifier request );
	
	// The IsProtocolServiceSupported method will return true if the protocol
	// layer supports the specified feature.
	virtual bool IsProtocolServiceSupported ( SCSIProtocolFeature feature, void * serviceValue );
	
	// The HandleProtocolServiceFeature method will return true if the protocol
	// layer properly handled the specified feature.
	virtual bool HandleProtocolServiceFeature ( SCSIProtocolFeature feature, void * serviceValue );

	// ------ Power Management Support ------

	// The HandlePowerOff method is called to do any bus specific activity
	// necessary before shutting down and going to sleep.
	virtual IOReturn 		HandlePowerOff 	( void );

	// The HandlePowerOn method is called to do any bus specific activity
	// necessary to recover from power-on/wake from sleep (e.g. device reset on ATAPI)
	virtual IOReturn 		HandlePowerOn 	( void );

	
	// ---- Methods defined by this class ----

	// Static callback proc for all SCSITask objects, it calls through to
	// SCSITaskCallbackFunction.
	static void 			sSCSITaskCallbackProc ( IOATACommand * cmd );
	
	// Callback proc for synchronous ATA only type commands.
	static void 			sATACallbackSync ( IOATACommand * cmd );

	// Callback proc for asynchronous ATAPI resets.
	static void 			sATAPIResetCallback ( IOATACommand * cmd );

	// Callback proc that does nothing.
	static void 			sATAPIVoidCallback ( IOATACommand * cmd );

	// State machine for device configuration.
	static void				sATAPIConfigStateMachine ( IOATACommand * cmd );
	
	// Used for low-power polling.
	static void				sPollStatusRegister ( void * xptDriver, void * refCon );
	
	// Static callback for low-power polling.
	static void				sPollStatusRegisterCallback ( IOATACommand * cmd );

	// This method is called by the SCSITaskCallbackFunction and it calls the
	// inherited CommandCompleted message with the results of the task.
	virtual void CompleteSCSITask ( SCSITaskIdentifier 	scsiTask,
									SCSIServiceResponse serviceResponse,
									SCSITaskStatus		taskStatus );

	// The SCSITaskCallbackFunction method is called by the static callback procedure
	// to complete SCSITask operations. This method may be subclassed if a device
	// workaround is necessary.
	virtual void SCSITaskCallbackFunction ( IOATACommand * 		cmd,
											SCSITaskIdentifier 	scsiTask );
	
	// The ReportATAPIDeviceType method returns the ataDeviceType for ATAPI
	virtual ataDeviceType 	ReportATAPIDeviceType ( void ) const;
	
	// The InspectDevice method is called to inspect the ATA device and verify
	// that it is a device which can be controlled by this object
	virtual bool			InspectDevice ( IOATADevice * ataDevice );
	
	// The AllocateATACommandObjects method is called to allocate a pool of
	// IOATACommands for general purpose I/O usage.
	virtual void 			AllocateATACommandObjects ( void );
	
	// The DeallocateATACommandObjects method is called to deallocate a pool of
	// IOATACommands for general purpose I/O usage.
	virtual void 			DeallocateATACommandObjects ( void );
	
	// The GetATACommandObject method is called to retrieve a command from
	// the pool. Pass true to allow the method to sleep waiting for a command,
	// else pass false. If false is passed, there is NO guarantee that a valid
	// command will be returned. If true is passed, it IS guaranteed that a
	// valid command will be returned, but the thread may be slept waiting for
	// those resources.
	virtual IOATACommand *	GetATACommandObject	( bool okToSleep = true );
	
	// The ReturnATACommandObject method is called to return a command to
	// the pool.
	virtual void			ReturnATACommandObject	( IOATACommand * cmd );

	// The sSwapBytes16 method is used to word-swap the Device Identify data
	// on big endian systems. Since all identify data is returned in little endian
	// values, it must be word-swapped before inspected.
	static void				sSwapBytes16 ( UInt8 * buffer, IOByteCount numBytesToSwap );
	
	// The sConvertHighestBitToNumber method is used to find the highest bit in a
	// word and return a numeric value.	This is used to find the highest possible
	// values for PIO, DMA, and UltraDMA transfer modes.
	static UInt8			sConvertHighestBitToNumber ( UInt16 bitField );

	// The sSetWakeupResetOccurred method is used to safely set member variables
	// behind the command gate.
	static void				sSetWakeupResetOccurred ( IOATAPIProtocolTransport * driver,
												   	  bool resetOccurred );

	// The sCheckWakeupResetOccur method is used to safely check member variables
	// behind the command gate.
	static void				sCheckWakeupResetOccurred ( IOATAPIProtocolTransport * driver,
														bool * resetOccurred );
	
	// The DidWakeupResetOccur method is used to safely find out if a reset
	// occurred while we were asleep.
	virtual bool			CheckWakeupResetOccurred ( void );
	
	// The WakeupResetOccurred method is used to safely set/clear the reset flag.
	virtual void			SetWakeupResetOccurred ( bool resetOccurred );

	// The SetPIOTransferMode method is used to set the programmed input-output (PIO)
	// transfer mode. The highest value reported by the device is used.
	virtual IOReturn		SetPIOTransferMode ( IOATACommand * cmd, bool forceSync );
	
	// The SetDMATransferMode method is used to set the direct memory access (DMA)
	// transfer mode. The highest value reported by the device is used.
	virtual IOReturn		SetDMATransferMode ( IOATACommand * cmd, bool forceSync );
	
	// The IdentifyAndConfigureATAPIDevice method is called to correctly identify
	// and configure an ATAPI device.
	virtual IOReturn 		IdentifyAndConfigureATAPIDevice ( void );
	
	// The ConfigureATAPIDevice method is called to correctly configure the ATAPI
	// device. It currently configures any PIO/DMA/UDMA transfer modes but may be
	// expanded in the future.
	virtual IOReturn 		ConfigureATAPIDevice ( void );

	// The ReconfigureATAPIDevice method is called to correctly reconfigure the ATAPI
	// device after a reset event has occurred (bus reset or device reset). It
	// currently reconfigures any PIO/DMA/UDMA transfer modes but may be expanded in
	// the future.
	virtual IOReturn 		ReconfigureATAPIDevice ( void );
	
	// The IdentifyATAPIDevice method is called to correctly identify the ATAPI
	// device. It currently issues the Identify ATAPI Device packet command and
	// uses the data to correctly configure the device.
	virtual IOReturn 		IdentifyATAPIDevice ( void );
	
	// The ResetATAPIDevice method is called to issue a Device Reset (also known
	// as a SOFT RESET). This method will only reset the specified device on the
	// bus.
	virtual IOReturn 		ResetATAPIDevice ( void );
	
	// The SendATASleepCommand method is called to put an ATAPI drive in sleep
	// mode.
	virtual IOReturn		SendATASleepCommand ( void );
	
	// The EnablePollingOfStatusRegister method is called to set up a timer for
	// the call to PollStatusRegister. It is called when the SCSI Application Layer
	// driver wants to enable low-power polling and after a poll which does not
	// see a change in the register status.
	virtual void			EnablePollingOfStatusRegister ( void );

	// The DisablePollingOfStatusRegister method is called to cancel any thread
	// call which is doing a poll and drop the retain count on the object.
	virtual void			DisablePollingOfStatusRegister ( void );
	
	// The PollStatusRegister method is used to do low-power polling on drives which
	// support it. This feature is enabled by the SCSI Application Layer driver.
	virtual void			PollStatusRegister ( void * refCon );
	
	// Callback method for the polling of the status register.
	virtual void			PollStatusRegisterCallback ( IOATACommand * cmd );
	
	// The TurnDrivePowerOff method is called to turn power to the drive OFF
	IOReturn				TurnDrivePowerOff ( void );
	
	IOReturn				SendCommand ( IOATACommand * cmd );
	IOReturn				GatedWaitForRequest ( void * data );
	
private:
	
	// Binary Compatibility reserved method space
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 1 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 2 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 3 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 4 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 5 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 6 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 7 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 8 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 9 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 10 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 11 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 12 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 13 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 14 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 15 );
	OSMetaClassDeclareReservedUnused ( IOATAPIProtocolTransport, 16 );
	
};

#endif /* defined(KERNEL) && defined(__cplusplus) */

#endif	/* _IOKIT_IO_ATAPI_PROTOCOL_TRANSPORT_H_ */