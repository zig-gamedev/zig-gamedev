/*
 * Copyright (c) 1998-2009 Apple Inc. All rights reserved.
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


/*!
  @header IOFireWireSerialBusProtocolTransport
  Contains the class definition for IOFireWireSerialBusProtocolTransport.
*/


#ifndef _IOKIT_IO_FIREWIRE_SERIAL_BUS_PROTOCOL_TRANSPORT_H_
#define _IOKIT_IO_FIREWIRE_SERIAL_BUS_PROTOCOL_TRANSPORT_H_


#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IOLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOService.h>
#include <IOKit/IOCommandPool.h>
#include <IOKit/firewire/IOFireWireUnit.h>
#include <IOKit/sbp2/IOFireWireSBP2LUN.h>

#include <IOKit/scsi/IOSCSIProtocolServices.h>

/*!
	@class IOFireWireSerialBusProtocolTransport
	@abstract SCSI Protocol Driver Family for FireWire SBP2 Devices. 
	@discussion IOFireWireSerialBusProtocolTransport contains all the bus specific support for FireWire
	SBP2 compliant devices. To add vendor specific features or workarounds you will sub-class the appropriate
	methods of this family. 
*/

class __exported IOFireWireSerialBusProtocolTransport : public IOSCSIProtocolServices
{
	
	OSDeclareDefaultStructors ( IOFireWireSerialBusProtocolTransport )
	
private:
	
	IOFireWireUnit *				fUnit;
	IOFireWireSBP2LUN *				fSBPTarget;
	IOFireWireSBP2Login *			fLogin;
	IOFireWireSBP2ORB *				fORB;
	IOFireWireSBP2ManagementORB * 	fLUNResetORB;
	
	// /!\ WARNING! NOT USED left behind for legacy binary reasons
	__unused IOSimpleLock *					fQueueLock;
	
	UInt32							fLoginRetryCount;
	bool							fDeferRegisterService;
	bool							fNeedLogin;
    
    // /!\ WARNING! NOT USED left behind for legacy binary reasons
	__unused bool							fPhysicallyConnected;
	
	static void
	StatusNotifyStatic ( void * refCon, FWSBP2NotifyParamsPtr params );
	
	static void
	UnsolicitedStatusNotifyStatic (	void * 					refCon,
									FWSBP2NotifyParamsPtr 	params );
	
	static void
	LunResetCompleteStatic (	void * 							refCon,
								IOReturn						status,
								IOFireWireSBP2ManagementORB * 	orb );
	
	static void
	FetchAgentResetCompleteStatic (	void * refcon,
									IOReturn status );
	
	static IOReturn
	ConnectToDeviceStatic (	OSObject * refCon, void *, void *, void *, void * );
	
	virtual void
	FetchAgentResetComplete ( IOReturn status );
	
	static void LoginCompletionStatic ( void * refCon, FWSBP2LoginCompleteParams * params );
	
	static void
	LogoutCompletionStatic ( void * refCon, FWSBP2LogoutCompleteParams * params );
	
	/*!
		@function CoalesceSenseData
		@abstract CoalesceSenseData convert a SBP-2 status block into a SPC-2 sense block.
		@discussion	CoalesceSenseData pulls the appropriate bits out of the SBP2 sense block
		as defined in SBP-2 Annex B section B.2 and dynamically builds a sense data block as
		defined in SPC-2 section 7.23.2.
	*/
	
	SCSITaskStatus
	CoalesceSenseData (	FWSBP2StatusBlock *	sourceData,
						UInt8				quadletCount,
						SCSI_Sense_Data *	targetData );
	
	virtual void ConnectToDevice ( void );
	
	virtual void DisconnectFromDevice ( void );
	
	virtual bool IsDeviceCPUInDiskMode ( void );

protected:
    
	/*!
		@function AllocateResources
		@abstract Allocate Resources.
		@discussion	Called from start method to allocate needed resources.
	*/
    
	virtual IOReturn AllocateResources ( void );
    
	/*!
		@function DeallocateResources
		@abstract Deallocate Resources.
		@discussion	Called from cleanUp method to deallocate resources.
	*/
    
	virtual void DeallocateResources ( void );
	
	enum SBP2LoginState
	{
		kFirstTimeLoggingInState,
		kLogginSucceededState,
		kLogginFailedState
	};
	
	/*! 
		@typedef SBP2ClientOrbData
		@param orb IOFireWireSBP2ORB for request.
		@param scsiTask SCSITaskIdentifier of request. 
		@param serviceResponse SCSIServiceResponse of request.
		@param taskStatus SCSITaskStatus of request.
		@discussion This structure is stuffed into the refcon so we can associate which
		IOFireWireSBP2ORB and SCSITaskIdentifier is completing.
	*/
	
	typedef struct {
		IOFireWireSBP2ORB *				orb;
		SCSITaskIdentifier 				scsiTask;
		SCSIServiceResponse 			serviceResponse;
		SCSITaskStatus					taskStatus;
		IOBufferMemoryDescriptor *		quadletAlignedBuffer;
	} SBP2ClientOrbData;
	
	static const UInt32 kDefaultBusyTimeoutValue	= 0x0000000F;
	static const UInt64 kMaxFireWireLUN				= 0xFFFF;
	static const UInt32 kMaxFireWirePayload			= 4096;
	static const UInt32 kMaxLoginRetryCount			= 8;
	static const UInt32 kMaxReconnectCount			= 128;
	static const UInt32 kCSRModelInfoKey			= 0x17;

	UInt32	fReconnectCount;
	bool 	fLoggedIn;
	
	// binary compatibility instance variable expansion
	struct ExpansionData
	{ 
		IOCommandPool *		fCommandPool;
		IOCommandPool *		fSubmitQueue;
		SBP2LoginState		fLoginState;
		bool				fLUNResetPathFlag;
		int					fLUNResetCount;
		bool				fAlwaysSetSenseData;
		bool				fAutonomousSpinDownWorkAround;
	};
	
	ExpansionData * reserved;
	
	bool fObjectIsOpen;

	/*!
		@function CommandORBAccessor
		@abstract accessor function for fORB.
		@discussion	xxx.
	*/

	IOFireWireSBP2ORB * CommandORBAccessor ( void );
   
	/*!
		@function SBP2LoginAccessor
		@abstract accessor function for fLogin.
		@discussion	xxx.
	*/

	IOFireWireSBP2Login * SBP2LoginAccessor ( void );

	virtual IOReturn
	message ( UInt32 type, IOService * provider, void * argument = 0 ) APPLE_KEXT_OVERRIDE;

	/*!
		@function SendSCSICommand
		@abstract Prepare and send a SCSI command to the device.
		@discussion	The incoming SCSITaskIdentifier gets turned into a IOFireWireSBP2ORB
		and is submitted to the SBP2 layer. See IOSCSIProtocolServices.h for more details
		regarding SendSCSICommand. Also see IOFireWireSBP2Lib.h for details regarding the
		IOFireWireSBP2ORB structure and the submitORB method.
		@result If the command was sent to the device and is pending completion, the
		subclass should return true and return back the kSCSIServiceResponse_Request_In_Process response.
		If the command completes immediately with an error, the subclass will return true
		and return back the appropriate status. If the subclass is currently processing all the
		commands it can, the subclass will return false and the command will be resent next time
		CommandCompleted is called.
	*/

	virtual bool 
	SendSCSICommand (	SCSITaskIdentifier 		request,
						SCSIServiceResponse *	serviceResponse,
						SCSITaskStatus *		taskStatus ) APPLE_KEXT_OVERRIDE;

	/*!
		@function SetCommandBuffers
		@abstract Method to set orb's buffers.
		@discussion	This method was added so that subclasses can override and massage buffers as
		needed. The default simply calls setCommandBuffers. See IOFireWireSBP2Lib.h for details
		regarding the setCommandBuffers method.
		@result xxx.
	*/
	
	virtual IOReturn
	SetCommandBuffers (	IOFireWireSBP2ORB * orb, SCSITaskIdentifier request );

	/*!
		@function CompleteSCSITask
		@abstract This qualifies and sets appropriate data then calls CommandCompleted.
		@discussion	See IOSCSIProtocolServices.h for more details
		regarding CommandCompleted.
	*/
	
	virtual void 
	CompleteSCSITask ( IOFireWireSBP2ORB * orb );

	/*!
		@function AbortSCSICommand
		@abstract This method is intended to abort an in progress SCSI Task.
		@discussion	Currently not implemented in super class. This is a stub method for adding
		the abort command in the near future.
		@result See SCSITask.h for SCSIServiceResponse codes.
	*/
	
	virtual SCSIServiceResponse 
	AbortSCSICommand ( SCSITaskIdentifier request ) APPLE_KEXT_OVERRIDE;

	/*!
		@function StatusNotify
		@abstract This is our handler for status.
		@discussion See IOFireWireSBP2Lib.h for details regarding the FWSBP2NotifyParams
		structure that is passed in to the completion..
	*/
	
	virtual void
	StatusNotify ( FWSBP2NotifyParams * params );

	/*!
		@function SetValidAutoSenseData
		@abstract Set the auto sense data that was returned for a given SCSI Task.
		@discussion	SetValidAutoSenseData is called to qualify sense data that is copied to the
		client via the SetAutoSenseData method. See IOSCSIProtocolServices.h for more details
		regarding SetAutoSenseData.
	*/
	
	void
	SetValidAutoSenseData (	SBP2ClientOrbData *	clientData,
							FWSBP2StatusBlock *	statusBlock,
							SCSI_Sense_Data *	targetData );
	
	/*!
		@function UnsolicitedStatusNotify
		@abstract This is our handler for unsolicited status.
		@discussion	After we have parsed and handled the unsolicited status we call 
		enableUnsolicitedStatus. See IOFireWireSBP2Lib.h for details regarding the
		enableUnsolicitedStatus method.
	*/
	
	virtual void
	UnsolicitedStatusNotify ( FWSBP2NotifyParamsPtr params );

	/*!
		@function LoginCompletion
		@abstract Completion routine for login complete.
		@discussion	See IOFireWireSBP2Lib.h for details regarding the FWSBP2LogoutCompleteParams
		structure that is passed in to the completion.
	*/
	
	virtual void 
	LoginCompletion ( FWSBP2LoginCompleteParams * params );

	/*!
		@function LogoutCompletion
		@abstract Completion routine for logout complete.
		@discussion	See IOFireWireSBP2Lib.h for details regarding the FWSBP2LogoutCompleteParams
		structure that is passed in to the completion.
	*/
	
	virtual void 
	LogoutCompletion ( FWSBP2LogoutCompleteParams * params );

	/*!
		@function IsProtocolServiceSupported
		@abstract Determine is specified feature is supported by the protocol layer.
		@discussion	If the service has a value that must be returned, it will be returned in the
		serviceValue output parameter. See IOSCSIProtocolServices.h for more details regarding
		IsProtocolServiceSupported.
		@result Will return true if the specified feature is supported by the protocol layer.
	*/

	virtual bool
	IsProtocolServiceSupported ( SCSIProtocolFeature feature, void * serviceValue ) APPLE_KEXT_OVERRIDE;

	/*!
		@function HandleProtocolServiceFeature
		@abstract Handle specified feature supported by the protocol layer.
		@discussion	See IOSCSIProtocolServices.h for more details regarding HandleProtocolServiceFeature.
		@result Will return true if the specified feature is supported by the protocol layer.
	*/

	virtual bool
	HandleProtocolServiceFeature ( SCSIProtocolFeature feature, void * serviceValue ) APPLE_KEXT_OVERRIDE;
   
	/*!
		@function LunResetComplete
		@abstract Callback to submit Fetch Agent Reset.
		@discussion	See IOFireWireSBP2Lib.h for details regarding the submitFetchAgentReset
		method.
	*/
	
	virtual void
	LunResetComplete ( IOReturn status, IOFireWireSBP2ManagementORB * orb );

public:

	/*!
		@function init
		@abstract See IOService for discussion.
		@discussion	Setup and prime class into known state.
	*/
	
	bool init ( OSDictionary * propTable ) APPLE_KEXT_OVERRIDE;

	/*! 
		@function start
		@discussion See IOService for discussion.
		@result Return true if the start was successful, false otherwise ( which will
		cause the instance to be detached and usually freed ).
	*/

	virtual bool start ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	
	/*!
	 	@function cleanUp
		@abstract cleanUp is called to tear down IOFireWireSerialBusProtocolTransport.
		@discussion	cleanUp is called when we receive a kIOFWMessageServiceIsRequestingClose
		message or if we fail our initialization.
	*/
	
	virtual void cleanUp ( void );
	
	/*!
		@function finalize
		@abstract See IOService for discussion.
		@result Returns true.
	*/
	
	virtual bool finalize ( IOOptionBits options ) APPLE_KEXT_OVERRIDE;

	/*! 
		@function free
		@discussion See IOService for discussion.
		@result none.
	*/
	
	virtual void free ( void ) APPLE_KEXT_OVERRIDE;
		
protected:
	
	virtual IOReturn login ( void );
    OSMetaClassDeclareReservedUsed ( IOFireWireSerialBusProtocolTransport, 1 );
    
	virtual IOReturn submitLogin ( void );
    OSMetaClassDeclareReservedUsed ( IOFireWireSerialBusProtocolTransport, 2 );
    
	virtual void loginLost ( void );
    OSMetaClassDeclareReservedUsed ( IOFireWireSerialBusProtocolTransport, 3 );
    
    void loginSuspended ( void );
	OSMetaClassDeclareReservedUsed ( IOFireWireSerialBusProtocolTransport, 4 );
   
	virtual void loginResumed ( void );
	OSMetaClassDeclareReservedUsed ( IOFireWireSerialBusProtocolTransport, 5 );

	static IOReturn CriticalOrbSubmissionStatic ( 
			OSObject * refCon, 
			void * val1,
			void * val2,
			void * val3,
			void * val4 );

	/*!
		@function CriticalOrbSubmission
		@abstract xxx.
		@discussion	xxx.
		@result none.
	*/
	
	void
		CriticalOrbSubmission (
			IOFireWireSBP2ORB * orb,
			SCSITaskIdentifier request );
	
	virtual void submitOrbFromQueue ( void );
	OSMetaClassDeclareReservedUsed ( IOFireWireSerialBusProtocolTransport, 6 );
	
private:
	
	// binary compatibility reserved method space
    
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 7 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 8 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 9 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 10 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 11 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 12 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 13 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 14 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 15 );
	OSMetaClassDeclareReservedUnused ( IOFireWireSerialBusProtocolTransport, 16 );
	
};

#endif	/* _IOKIT_IO_FIREWIRE_SERIAL_BUS_PROTOCOL_TRANSPORT_H_ */
