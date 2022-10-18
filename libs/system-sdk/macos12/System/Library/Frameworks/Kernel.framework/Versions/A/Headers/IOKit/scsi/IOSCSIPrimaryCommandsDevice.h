/*
 * Copyright (c) 1998-2015 Apple Inc. All rights reserved.
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


#ifndef _IOKIT_IO_SCSI_PRIMARY_COMMANDS_DEVICE_H_
#define _IOKIT_IO_SCSI_PRIMARY_COMMANDS_DEVICE_H_

#if defined(KERNEL) && defined(__cplusplus)


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// General IOKit headers
#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/IOReturn.h>
#include <IOKit/IOMemoryDescriptor.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi/SCSICommandDefinitions.h>
#include <IOKit/scsi/SCSICmds_INQUIRY_Definitions.h>
#include <IOKit/scsi/SCSICmds_REQUEST_SENSE_Defs.h>
#include <IOKit/scsi/IOSCSIProtocolInterface.h>

// Build includes
#include <TargetConditionals.h>


//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------

// Notification messages
enum
{
	kSCSIServicesNotification_Suspend	= 0x69000080,
	kSCSIServicesNotification_Resume	= 0x69000090
};


// Timeout values
enum
{
	kOneSecondTimeoutInMS 		    = 1000,
	kTenSecondTimeoutInMS 		    = 10 * kOneSecondTimeoutInMS,
    kTwentyFiveSecondTimeoutInMS    = 25 * kOneSecondTimeoutInMS,
	kThirtySecondTimeoutInMS	    = 30 * kOneSecondTimeoutInMS,
    kFortyFiveSecondTimeoutInMS	    = 45 * kOneSecondTimeoutInMS
};

// Mode page values for page control field
enum
{
	kModePageControlCurrentValues		= 0x00,
	kModePageControlChangeableValues	= 0x01,
	kModePageControlDefaultValues		= 0x02,
	kModePageControlSavedValues			= 0x03
};

// The command should be tried 5 times.  The original attempt 
// plus 4 retries.
#define kDefaultRetryCount		4

// Forward declarations for internal use only classes
class SCSIPrimaryCommands;


//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------

class __exported IOSCSIPrimaryCommandsDevice : public IOSCSIProtocolInterface
{
	
	OSDeclareAbstractStructors ( IOSCSIPrimaryCommandsDevice )
	
	friend class SCSITaskUserClient;
	
private:

	IOSCSIProtocolInterface *		fProtocolDriver;
	
	// The fProtocolAccessEnabled member variable indicates whether
	// requests coming from the client can be sent to the device.
	// This will be set to true after the Protocol Service driver is successfully 
	// opened and before the call to InitializeDeviceSupport and set to false before 
	// TerminateDeviceSupport is called.
	bool							fProtocolAccessEnabled;
	
	// The fDeviceAccessEnabled member variable indicates whether
	// requests coming from the client can be sent to the device.
	// This will be set to true after InitializeDeviceSupport is called and
	// set to false before StopDeviceSupport is called.
	bool							fDeviceAccessEnabled;
	bool							fDeviceAccessSuspended;
	
	static bool		ServerKeyswitchCallback ( void *		target,
											  void *		refCon,
											  IOService *	newService );
	
	static void		TaskCallback ( SCSITaskIdentifier completedTask );
	void			TaskCompletion ( SCSITaskIdentifier completedTask );
	
	static IOReturn	sWaitForTask ( void * object, SCSITaskIdentifier request );
	IOReturn		GatedWaitForTask ( SCSITaskIdentifier request );
	
protected:
	
	// Reserve space for future expansion.
	struct IOSCSIPrimaryCommandsDeviceExpansionData
	{
		IONotifier *				fKeySwitchNotifier;
		UInt8						fANSIVersion;
		UInt32						fReadTimeoutDuration;
		UInt32						fWriteTimeoutDuration;
		bool						fCMDQUE;
		SCSITaggedTaskIdentifier	fTaskID;
		IOSimpleLock *				fTaskIDLock;
		UInt32						fRetryCount;
        UInt32                      fNumCommandsExecuting;
        int                         fMaxPollRetries;
        int                         fPollDebounceRetriesLeft;
	};
	IOSCSIPrimaryCommandsDeviceExpansionData * fIOSCSIPrimaryCommandsDeviceReserved;
	
	#define	fReadTimeoutDuration		fIOSCSIPrimaryCommandsDeviceReserved->fReadTimeoutDuration
	#define	fWriteTimeoutDuration		fIOSCSIPrimaryCommandsDeviceReserved->fWriteTimeoutDuration
	#define	fRetryCount					fIOSCSIPrimaryCommandsDeviceReserved->fRetryCount
    #define	fNumCommandsExecuting       fIOSCSIPrimaryCommandsDeviceReserved->fNumCommandsExecuting
    #define fMaxPollRetries             fIOSCSIPrimaryCommandsDeviceReserved->fMaxPollRetries
    #define fPollDebounceRetriesLeft    fIOSCSIPrimaryCommandsDeviceReserved->fPollDebounceRetriesLeft

	UInt8							fDefaultInquiryCount;
	OSDictionary *					fDeviceCharacteristicsDictionary;
	UInt32							fNumCommandsOutstanding;
	
	virtual void 					free ( void ) APPLE_KEXT_OVERRIDE;
	void							SetANSIVersion ( UInt8 );
	void							SetCMDQUE ( bool value );
	IOReturn						GetModeSense ( 
										IOMemoryDescriptor *		dataBuffer,
										SCSICmdField6Bit 			PAGE_CODE,
										SCSICmdField2Byte 			ALLOCATION_LENGTH,
										bool *						use10ByteModeSense );
	bool							RetrieveINQUIRYData (
										UInt8						EVPD,
										UInt8						inquiryPage,
										UInt8 *						inquiryBuffer,
										UInt16 *					dataSize );
	
	// This flag is set if the device responds to a MODE_SENSE_10 command
	// with the page code set to 0x1A (Power Conditions Mode Page)
	bool							fDeviceSupportsPowerConditions;
	
	// This method is called by the start method to obtain information from
	// the device with regards to whether it supports the power conditions mode page.
	virtual void					CheckPowerConditionsModePage ( void );
	
	// This will return back the Protocol driver that is used to
	// send service requests to the device.
	virtual IOSCSIProtocolInterface * GetProtocolDriver ( void );
	
	// This will get a new SCSITask for the caller
	virtual SCSITaskIdentifier		GetSCSITask ( void );
	
	// This will release a SCSITask (eventually return it to a pool)
	virtual void					ReleaseSCSITask ( SCSITaskIdentifier request );
	
	// This will return a unique value for the tagged task identifier
	SCSITaggedTaskIdentifier		GetUniqueTagID ( void );
	
	// Call for executing the command synchronously	
	SCSIServiceResponse 			SendCommand ( 	
										SCSITaskIdentifier 	request,
										UInt32 				timeoutDuration );
														
	// Call for executing the command asycnchronously	
	void 							SendCommand ( 
										SCSITaskIdentifier 	request,
										UInt32 				timeoutDuration,
										SCSITaskCompletion 	taskCompletion );
	
	
	virtual bool 					InitializeDeviceSupport ( void ) = 0;
	virtual void 					StartDeviceSupport ( void ) = 0;
	virtual void 					SuspendDeviceSupport ( void ) = 0;
	virtual void 					ResumeDeviceSupport ( void ) = 0;
	virtual void 					StopDeviceSupport ( void ) = 0;
	virtual void 					TerminateDeviceSupport ( void ) = 0;
	virtual UInt32					GetNumberOfPowerStateTransitions ( void ) = 0;
	
	virtual IOReturn				VerifyDeviceState ( void );
	// Called to clear any power-on/reset status in the drive
	virtual bool					ClearPowerOnReset ( void );
	virtual bool					ClearNotReadyStatus ( void ) = 0;
	
	bool							IsProtocolAccessEnabled ( void );
	bool							IsDeviceAccessEnabled ( void );
	bool							IsDeviceAccessSuspended ( void );

	// Accessors for saving and retrieving data from an SCSITask object.
	bool 							ResetForNewTask(	
										SCSITaskIdentifier 		request );
	bool							SetTaskAttribute ( 
										SCSITaskIdentifier 		request, 
										SCSITaskAttribute 		newAttribute );
	SCSITaskAttribute				GetTaskAttribute ( 
										SCSITaskIdentifier 		request );
	bool							SetTaggedTaskIdentifier ( 
										SCSITaskIdentifier 			request, 
										SCSITaggedTaskIdentifier	taggedTaskIdentifier );
	SCSITaggedTaskIdentifier		GetTaggedTaskIdentifier ( 
										SCSITaskIdentifier 		request );
	bool							SetTaskState ( 	
										SCSITaskIdentifier 		request,
										SCSITaskState 			newTaskState );
	SCSITaskState					GetTaskState ( 
										SCSITaskIdentifier 		request );
	bool							SetTaskStatus ( 
										SCSITaskIdentifier 		request, 
										SCSITaskStatus 			newStatus );
	SCSITaskStatus					GetTaskStatus ( 
										SCSITaskIdentifier 		request );
	bool 							SetCommandDescriptorBlock ( 
										SCSITaskIdentifier 		request,
										UInt8					cdbByte0,
										UInt8					cdbByte1,
										UInt8					cdbByte2,
										UInt8					cdbByte3,
										UInt8					cdbByte4,
										UInt8					cdbByte5 );
	
	// Populate the 10 Byte Command Descriptor Block
	bool 							SetCommandDescriptorBlock ( 
										SCSITaskIdentifier 		request,
										UInt8					cdbByte0,
										UInt8					cdbByte1,
										UInt8					cdbByte2,
										UInt8					cdbByte3,
										UInt8					cdbByte4,
										UInt8					cdbByte5,
										UInt8					cdbByte6,
										UInt8					cdbByte7,
										UInt8					cdbByte8,
										UInt8					cdbByte9 );
	
	// Populate the 12 Byte Command Descriptor Block
	bool 							SetCommandDescriptorBlock ( 
										SCSITaskIdentifier 		request,
										UInt8					cdbByte0,
										UInt8					cdbByte1,
										UInt8					cdbByte2,
										UInt8					cdbByte3,
										UInt8					cdbByte4,
										UInt8					cdbByte5,
										UInt8					cdbByte6,
										UInt8					cdbByte7,
										UInt8					cdbByte8,
										UInt8					cdbByte9,
										UInt8					cdbByte10,
										UInt8					cdbByte11 );
	
	// Populate the 16 Byte Command Descriptor Block
	bool 							SetCommandDescriptorBlock ( 
										SCSITaskIdentifier 		request,
										UInt8					cdbByte0,
										UInt8					cdbByte1,
										UInt8					cdbByte2,
										UInt8					cdbByte3,
										UInt8					cdbByte4,
										UInt8					cdbByte5,
										UInt8					cdbByte6,
										UInt8					cdbByte7,
										UInt8					cdbByte8,
										UInt8					cdbByte9,
										UInt8					cdbByte10,
										UInt8					cdbByte11,
										UInt8					cdbByte12,
										UInt8					cdbByte13,
										UInt8					cdbByte14,
										UInt8					cdbByte15 );
										
	bool							SetDataTransferDirection ( 
										SCSITaskIdentifier 		request, 
										UInt8 					newDirection );
	UInt8							GetDataTransferDirection ( 
										SCSITaskIdentifier 		request );
	bool							SetRequestedDataTransferCount (
										SCSITaskIdentifier 		request, 
										UInt64 					newRequestedCount );
	UInt64							GetRequestedDataTransferCount (
										SCSITaskIdentifier 		request );
	bool							SetRealizedDataTransferCount (
										SCSITaskIdentifier 		request,
										UInt64 					newRealizedDataCount );
	UInt64							GetRealizedDataTransferCount ( 
										SCSITaskIdentifier 		request );
	bool							SetDataBuffer ( 
										SCSITaskIdentifier 		request, 
										IOMemoryDescriptor * 	newBuffer );
	IOMemoryDescriptor *			GetDataBuffer ( 
										SCSITaskIdentifier 		request );
	bool							SetTimeoutDuration ( 
										SCSITaskIdentifier 		request, 
										UInt32 					newTimeout );
	UInt32							GetTimeoutDuration ( 
										SCSITaskIdentifier 		request );
	bool							SetTaskCompletionCallback ( 
										SCSITaskIdentifier 		request,
										SCSITaskCompletion 		newCallback );
	void							TaskCompletedNotification ( 
										SCSITaskIdentifier 		request );
	
	bool							SetServiceResponse (
										SCSITaskIdentifier 		request,
										SCSIServiceResponse 	serviceResponse );
	SCSIServiceResponse 			GetServiceResponse (
										SCSITaskIdentifier 		request );
	bool							SetAutosenseCommand (
										SCSITaskIdentifier 		request,
										UInt8					cdbByte0,
										UInt8					cdbByte1,
										UInt8					cdbByte2,
										UInt8					cdbByte3,
										UInt8					cdbByte4,
										UInt8					cdbByte5 );
	bool							GetAutoSenseData (
										SCSITaskIdentifier 		request,
										SCSI_Sense_Data * 		senseData ); // DEPRECATED, use below function instead
	bool							GetAutoSenseData (
										SCSITaskIdentifier 		request,
										SCSI_Sense_Data * 		senseData,
										UInt8					senseDataSize );
	UInt8							GetAutoSenseDataSize (
										SCSITaskIdentifier 		request );
	
	bool							SetApplicationLayerReference (
										SCSITaskIdentifier 		request,
										void * 					newReferenceValue );
	void *							GetApplicationLayerReference (
										SCSITaskIdentifier 		request );
	
	void 							IncrementOutstandingCommandsCount ( void );
	static void						sIncrementOutstandingCommandsCount ( 
										IOSCSIPrimaryCommandsDevice * self );
	virtual void					HandleIncrementOutstandingCommandsCount ( void );	
	

	// This static member routine provides a mechanism for retrieving a pointer to
	// the object that is claimed as the owner of the specified SCSITask.
	static OSObject *				sGetOwnerForTask ( SCSITaskIdentifier request );
	
public:
	
	bool				init ( OSDictionary * propTable ) APPLE_KEXT_OVERRIDE;
	virtual bool		start ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	virtual void		stop ( IOService *  provider ) APPLE_KEXT_OVERRIDE;
	virtual IOReturn 	message ( UInt32 type, IOService * nub, void * arg ) APPLE_KEXT_OVERRIDE;
	
	// The setAgressiveness method is called by the power manager
	// to notify us of certain power management settings. We override
	// this method in order to catch the kPMMinutesToSpinDown message
	// in order to set our idle timer.
	virtual IOReturn 	setAggressiveness ( unsigned long type, unsigned long minutes ) APPLE_KEXT_OVERRIDE;
	
	// Methods for getting device information strings
	virtual char *		GetVendorString ( void );
	virtual char *		GetProductString ( void );
	virtual char *		GetRevisionString ( void );
	OSDictionary *		GetProtocolCharacteristicsDictionary ( void );
	OSDictionary *		GetDeviceCharacteristicsDictionary ( void );	
	UInt8				GetANSIVersion ( void );
	bool				GetCMDQUE ( void );
#if !TARGET_OS_IPHONE
    OSString *          MapINQUIRYDataToIconFile ( void );
#endif /* !TARGET_OS_IPHONE */
	UInt32				GetRetryCount ( void );
	
	// -- SCSI Protocol Interface Methods	--
	// The ExecuteCommand method will take a SCSI Task and transport
	// it across the physical wire(s) to the device
	virtual void		ExecuteCommand ( SCSITaskIdentifier request ) APPLE_KEXT_OVERRIDE;
	
	// The Task Management function to allow the SCSI Application Layer client to request
	// that a specific task be aborted.
	SCSIServiceResponse		AbortTask ( UInt8 theLogicalUnit, SCSITaggedTaskIdentifier theTag ) APPLE_KEXT_OVERRIDE;

	// The Task Management function to allow the SCSI Application Layer client to request
	// that a all tasks curerntly in the task set be aborted.
	SCSIServiceResponse		AbortTaskSet ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;

	SCSIServiceResponse		ClearACA ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;

	SCSIServiceResponse		ClearTaskSet ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;
    
	SCSIServiceResponse		LogicalUnitReset ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;

	SCSIServiceResponse		TargetReset ( void ) APPLE_KEXT_OVERRIDE;

	// The AbortCommand method will abort the indicated SCSI Task,
	// if it is possible and the task has not already completed.
	virtual SCSIServiceResponse	AbortCommand ( SCSITaskIdentifier request ) APPLE_KEXT_OVERRIDE;
	
	// The IsProtocolServiceSupported will return true if the specified
	// feature is supported by the protocol layer.  If the service has a value that must be
	// returned, it will be returned in the serviceValue output parameter.
	virtual bool		IsProtocolServiceSupported (
											SCSIProtocolFeature feature,
											void *				serviceValue ) APPLE_KEXT_OVERRIDE;
	
	// The HandleProtocolServiceFeature will return true if the specified feature could
	// be handled properly by the protocol layer.
	virtual bool		HandleProtocolServiceFeature (
											SCSIProtocolFeature	feature,
											void *				serviceValue ) APPLE_KEXT_OVERRIDE;
	
	// Utility methods for use by all peripheral device objects.

	// isParameterValid are used to validate that the parameter passed into
	// the command methods are of the correct value.
	
	// Validate Parameter used for 1 bit to 1 byte paramaters
    bool 				IsParameterValid ( 
							SCSICmdField1Byte 			param,
							SCSICmdField1Byte 			mask );
	
	// Validate Parameter used for 9 bit to 2 byte paramaters
	bool 				IsParameterValid ( 
							SCSICmdField2Byte 			param,
							SCSICmdField2Byte 			mask );
	
	// Validate Parameter used for 17 bit to 4 byte paramaters
	bool 				IsParameterValid ( 
							SCSICmdField4Byte 			param,
							SCSICmdField4Byte 			mask );
	
	// Validate Parameter used for 33 bit to 8 byte paramaters
	bool 				IsParameterValid ( 
							SCSICmdField8Byte 			param,
							SCSICmdField8Byte 			mask );
	

	bool 				IsMemoryDescriptorValid (
							IOMemoryDescriptor * 		dataBuffer );
	
	bool 				IsMemoryDescriptorValid (
							IOMemoryDescriptor * 		dataBuffer,
							UInt64						requiredSize );
	
	virtual bool		INQUIRY (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			CMDDT,
							SCSICmdField1Bit 			EVPD,
							SCSICmdField1Byte 			PAGE_OR_OPERATION_CODE,
							SCSICmdField1Byte 			ALLOCATION_LENGTH, 
							SCSICmdField1Byte 			CONTROL );

#if !TARGET_OS_IPHONE
	
	virtual bool		LOG_SELECT (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			PCR,
							SCSICmdField1Bit 			SP,
							SCSICmdField2Bit 			PC,
							SCSICmdField2Byte 			PARAMETER_LIST_LENGTH,
							SCSICmdField1Byte 			CONTROL );  

	virtual bool		LOG_SENSE (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			PPC,
							SCSICmdField1Bit 			SP,
							SCSICmdField2Bit 			PC,
							SCSICmdField6Bit 			PAGE_CODE,
							SCSICmdField2Byte 			PARAMETER_POINTER,
							SCSICmdField2Byte 			ALLOCATION_LENGTH, 
							SCSICmdField1Byte 			CONTROL );  
	
#endif /* !TARGET_OS_IPHONE */
	
	virtual bool		MODE_SELECT_6 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			PF,
							SCSICmdField1Bit 			SP,
							SCSICmdField1Byte 			PARAMETER_LIST_LENGTH, 
							SCSICmdField1Byte 			CONTROL );  

   	virtual bool		MODE_SELECT_10 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			PF,
							SCSICmdField1Bit 			SP,
							SCSICmdField2Byte 			PARAMETER_LIST_LENGTH, 
							SCSICmdField1Byte 			CONTROL );  

   	virtual bool		MODE_SENSE_6 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			DBD,
	   						SCSICmdField2Bit 			PC,
	   						SCSICmdField6Bit 			PAGE_CODE,
	   						SCSICmdField1Byte 			ALLOCATION_LENGTH,
							SCSICmdField1Byte 			CONTROL );

   	virtual bool		MODE_SENSE_10 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			LLBAA,
							SCSICmdField1Bit 			DBD,
	   						SCSICmdField2Bit 			PC,
	   						SCSICmdField6Bit 			PAGE_CODE,
	   						SCSICmdField2Byte 			ALLOCATION_LENGTH,
							SCSICmdField1Byte 			CONTROL );

#if !TARGET_OS_IPHONE
	
  	virtual bool		PERSISTENT_RESERVE_IN (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
	   						SCSICmdField5Bit 			SERVICE_ACTION,
	   						SCSICmdField2Byte 			ALLOCATION_LENGTH,
							SCSICmdField1Byte 			CONTROL );

   	virtual bool		PERSISTENT_RESERVE_OUT (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
	   						SCSICmdField5Bit 			SERVICE_ACTION,
	   						SCSICmdField4Bit 			SCOPE,
	   						SCSICmdField4Bit 			TYPE,
							SCSICmdField1Byte 			CONTROL );
	
#endif /* !TARGET_OS_IPHONE */

	virtual bool		PREVENT_ALLOW_MEDIUM_REMOVAL (
							SCSITaskIdentifier			request,
		 					SCSICmdField2Bit 			PREVENT, 
							SCSICmdField1Byte 			CONTROL );
	
#if !TARGET_OS_IPHONE
	
	virtual bool		READ_BUFFER (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField4Bit 			MODE,
							SCSICmdField1Byte 			BUFFER_ID,
							SCSICmdField3Byte 			BUFFER_OFFSET,
							SCSICmdField3Byte 			ALLOCATION_LENGTH, 
							SCSICmdField1Byte 			CONTROL );

	virtual bool		RECEIVE (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
		 					SCSICmdField3Byte 			TRANSFER_LENGTH,
							SCSICmdField1Byte 			CONTROL );

	virtual bool		RECEIVE_DIAGNOSTICS_RESULTS (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
		 					SCSICmdField1Bit 			PCV,
		 					SCSICmdField1Byte			PAGE_CODE,
		 					SCSICmdField2Byte 			ALLOCATION_LENGTH,
							SCSICmdField1Byte 			CONTROL );

	virtual bool		RELEASE_6 (
							SCSITaskIdentifier			request,
							SCSICmdField1Byte 			CONTROL );
	
	virtual bool		RELEASE_10 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			THRDPTY,
							SCSICmdField1Bit 			LONGID,
							SCSICmdField1Byte 			THIRD_PARTY_DEVICE_ID,
							SCSICmdField2Byte 			PARAMETER_LIST_LENGTH,
							SCSICmdField1Byte 			CONTROL );

#endif /* !TARGET_OS_IPHONE */
    
	virtual bool		REPORT_DEVICE_IDENTIFIER ( 
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField4Byte 			ALLOCATION_LENGTH,
							SCSICmdField1Byte 			CONTROL );
	


   	virtual bool		REPORT_LUNS ( 
							SCSITaskIdentifier			request,
   							IOMemoryDescriptor *		dataBuffer,
				   			SCSICmdField4Byte 			ALLOCATION_LENGTH,
				   			SCSICmdField1Byte 			CONTROL );
	
	virtual bool		REQUEST_SENSE (
							SCSITaskIdentifier			request,
   							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Byte 			ALLOCATION_LENGTH, 
							SCSICmdField1Byte 			CONTROL );
 	
#if !TARGET_OS_IPHONE
	
 	virtual bool		RESERVE_6 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Byte 			CONTROL );
	
	virtual bool		RESERVE_10 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			THRDPTY,
							SCSICmdField1Bit 			LONGID,
							SCSICmdField1Byte 			THIRD_PARTY_DEVICE_ID,
							SCSICmdField2Byte 			PARAMETER_LIST_LENGTH,
							SCSICmdField1Byte 			CONTROL );
	
	virtual bool		SEND (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit			AER,
		 					SCSICmdField3Byte 			TRANSFER_LENGTH,
							SCSICmdField1Byte 			CONTROL );

  	virtual bool		SEND_DIAGNOSTICS (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
				  		 	SCSICmdField3Bit 			SELF_TEST_CODE,
				  		 	SCSICmdField1Bit 			PF,
				  		 	SCSICmdField1Bit 			SELF_TEST,
				  		 	SCSICmdField1Bit 			DEVOFFL,
				  		 	SCSICmdField1Bit 			UNITOFFL,
				  	 		SCSICmdField2Byte 			PARAMETER_LIST_LENGTH,
				  	 		SCSICmdField1Byte 			CONTROL );
	
	virtual bool		SET_DEVICE_IDENTIFIER (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField5Bit 			SERVICE_ACTION,
							SCSICmdField4Byte 			PARAMETER_LIST_LENGTH,
							SCSICmdField1Byte 			CONTROL );
	
#endif /* !TARGET_OS_IPHONE */
	
	virtual bool		TEST_UNIT_READY (
							SCSITaskIdentifier			request,
							SCSICmdField1Byte 			CONTROL );
	
#if !TARGET_OS_IPHONE
 	
	virtual bool		WRITE_BUFFER (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField4Bit 			MODE,
							SCSICmdField1Byte 			BUFFER_ID,
							SCSICmdField3Byte 			BUFFER_OFFSET,
							SCSICmdField3Byte 			PARAMETER_LIST_LENGTH,
							SCSICmdField1Byte 			CONTROL );
	
#endif /* !TARGET_OS_IPHONE */
	
	// The SPC-3 INQUIRY command as defined in section 6.4.1 of SPC-3.
	bool				INQUIRY (
							SCSITaskIdentifier			request,
  							IOMemoryDescriptor *		dataBuffer,
    						SCSICmdField1Bit 			EVPD,
    						SCSICmdField1Byte 			PAGE_CODE,
    						SCSICmdField2Byte 			ALLOCATION_LENGTH,
    						SCSICmdField1Byte 			CONTROL );
	
    OSMetaClassDeclareReservedUsed ( IOSCSIPrimaryCommandsDevice,  1 );
    
    // ClampPowerState() will check if it is currently running behind the
    // workloop lock, if not, it will call itself again to run behind the
    // workloop gate.  When subclassing ClampPowerState() make sure to take
    // workloop synchronization in consideration.
    
    virtual void        ClampPowerState ( void );
    
    OSMetaClassDeclareReservedUsed ( IOSCSIPrimaryCommandsDevice,  2 );
    
    virtual void        ReleasePowerStateClamp ( void );

private:

#if !TARGET_OS_IPHONE
    // Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice,  3 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice,  4 );
    OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice,  5 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice,  6 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice,  7 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice,  8 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice,  9 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice, 10 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice, 11 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice, 12 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice, 13 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice, 14 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice, 15 );
	OSMetaClassDeclareReservedUnused ( IOSCSIPrimaryCommandsDevice, 16 );
#endif /* !TARGET_OS_IPHONE */
	
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif /* _IOKIT_IO_SCSI_PRIMARY_COMMANDS_DEVICE_H_ */
