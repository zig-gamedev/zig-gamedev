/*
 * Copyright (c) 2002-2008 Apple Inc. All rights reserved.
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

#ifndef __IOKIT_IO_SCSI_PARALLEL_INTERFACE_CONTROLLER_H__
#define __IOKIT_IO_SCSI_PARALLEL_INTERFACE_CONTROLLER_H__


 /*!
  @header IOSCSIParallelInterfaceController
	The IOSCSIParallelInterfaceController class and the associated HBA child
	class is responsible for the management of all related hardware. This
	includes the onboard HBA controller chip and the physical state of the
	bus. These classes are not responsible for any of the management of
	the SCSI Devices on the bus with the exception of maintaining the queue that 
	holds the objects representing those SCSI Devices.
*/


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// General IOKit includes
#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IODMACommand.h>
#include <IOKit/IOInterruptEventSource.h>
#include <IOKit/IOFilterInterruptEventSource.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOCommandPool.h>

// IOKit SCSI ArchitectureModel Family includes
#include <IOKit/scsi/SCSITask.h>
#include <IOKit/scsi/SCSICmds_REQUEST_SENSE_Defs.h>
#include <IOKit/scsi/SCSIPort.h>

// SCSI Parallel Interface includes
#include "IOSCSIParallelControllerCharacteristics.h"

// System includes
#include <sys/cdefs.h>

//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------


#define kIOPropertySCSIDeviceFeaturesKey			"SCSI Device Features"
#define kIOPropertySCSI_I_T_NexusFeaturesKey		"SCSI I_T Nexus Features"

// The SCSI Message Codes used for MESSAGE IN and MESSAGE OUT phases.
enum SCSIParallelMessages
{
	// Link Control Messages
	kSCSIParallelMessage_TASK_COMPLETE						= 0x00,
	kSCSIParallelMessage_EXTENDED_MESSAGE					= 0x01,
	kSCSIParallelMessage_SAVE_DATA_POINTER	 				= 0x02,
	kSCSIParallelMessage_RESTORE_POINTERS					= 0x03,
	kSCSIParallelMessage_DISCONNECT							= 0x04,
	kSCSIParallelMessage_INITIATOR_DETECTED_ERROR 			= 0x05,
	kSCSIParallelMessage_MESSAGE_REJECT		 				= 0x07,
	kSCSIParallelMessage_NO_OPERATION		 				= 0x08,
	kSCSIParallelMessage_MESSAGE_PARITY_ERROR 				= 0x09,
	kSCSIParallelMessage_IGNORE_WIDE_RESIDUE				= 0x23,
	kSCSIParallelMessage_QAS_REQUEST		 				= 0x55,
	kSCSIParallelMessage_IDENTIFY							= 0x80,
	
	// The Message Codes used in the EXTENDED_MESSAGE message.
	kSCSIParallelMessage_MODIFY_DATA_POINTER				= 0x00,
	kSCSIParallelMessage_SYNCHONOUS_DATA_TRANSFER_REQUEST	= 0x01,
	// Reserved												= 0x02
	kSCSIParallelMessage_WIDE_DATA_TRANSFER_REQUEST			= 0x03,
	kSCSIParallelMessage_PARALLEL_PROTOCOL_REQUEST			= 0x04,
	// Reserved												= 0x05 through 0xFF
	
	// Task Attribute Message Codes
	kSCSIParallelMessage_ACA								= 0x24,
	kSCSIParallelMessage_HEAD_OF_QUEUE						= 0x21,
	kSCSIParallelMessage_LINKED_COMMAND_COMPLETE			= 0x0A,
	kSCSIParallelMessage_ORDERED							= 0x22,
	kSCSIParallelMessage_SIMPLE								= 0x20,
	
	// Task Management Message Codes
	kSCSIParallelMessage_ABORT_TASK							= 0x0D,
	kSCSIParallelMessage_ABORT_TASK_SET						= 0x06,
	kSCSIParallelMessage_CLEAR_ACA							= 0x16,
	kSCSIParallelMessage_CLEAR_TASK_SET						= 0x0E,
	kSCSIParallelMessage_LOGICAL_UNIT_RESET					= 0x17,
	kSCSIParallelMessage_TARGET_RESET						= 0x0C
};

enum
{
	kSCSIParallelTaskControllerIDQueueHead 		= 0
};

// Notifications
enum
{
	kSCSIControllerNotificationBusReset			= 0x68000000
};

// Forward declaration for the internally used Parallel Device object.
class IOSCSIParallelInterfaceDevice;

// This is the identifier that is used to specify a given parallel Task.
typedef OSObject *	SCSIParallelTaskIdentifier;


//-----------------------------------------------------------------------------
//	Class Declarations
//-----------------------------------------------------------------------------

__exported_push;

/*! @class IOSCSIParallelInterfaceController
	@abstract Class that represents a SCSI Host Bus Adapter.
	@discussion Class that represents a SCSI Host Bus Adapter.
*/
class __kpi_deprecated("Use IOUserSCSIParallelInterfaceController with DriverKit, instead") IOSCSIParallelInterfaceController : public IOService
{

	OSDeclareAbstractStructors ( IOSCSIParallelInterfaceController )
	
#if 0
#pragma mark -
#pragma mark Client API
#endif
	
	
public:
	
	/*!
		@function GetSCSIParallelTask
		@abstract Method to allow the client to get a SCSIParallelTask
		@discussion Get a SCSIParallelTask from the controller so that a request
		can be issued to the HBA driver.
		@param blockForCommand If the blockForCommand parameter is set to false
		and there are no free SCSIParallelTasks, this method will return NULL, 
		otherwise it will wait for one to become available before returning.
		@result If there is a SCSI Parallel Task available, a reference to it 
		will be returned.
	*/
	
	SCSIParallelTaskIdentifier	GetSCSIParallelTask ( bool blockForCommand );
	
	/*!
		@function FreeSCSIParallelTask
		@abstract Method to allow the client to release a SCSIParallelTask
		@discussion	The FreeSCSIParallelTask method is called by the client when 
		a SCSIParallelTask has been completed and the associated returnTask
		needs to be returned to the pool.
		@param returnTask is a reference to the SCSIParallelTaskIdentifier to be 
		returned.
	*/
	
	void FreeSCSIParallelTask ( SCSIParallelTaskIdentifier returnTask );
	
	/*!
		@function FindTaskForAddress
		@abstract Find a task for a given Task Address, if one exists.
		@discussion If a valid Tagged Task Identifier is specified, this method 
		will return the task specified by the Tagged Task Address if one is 
		found, or else NULL will be returned.  If zero is used as the Tagged 
		Task Identifier, then this routine will search for an outstanding task 
		based on the Untagged Task Address and return the task or else, if one 
		is not found, return NULL.
		@param theT is the Target component of the I_T_L or I_T_L_Q nexus.
		@param theL is the Logical Unit component of the I_T_L or I_T_L_Q nexus.
		@param theQ is the Queue Tag component of the I_T_L_Q nexus.  If this is
		an I_T_L nexus, then the kSCSIUntaggedTaskIdentifier constant should be
		used for theQ.
		@result returns a valid SCSIParallelTaskIdentifier or NULL if none 
		found.
	*/

	SCSIParallelTaskIdentifier	FindTaskForAddress (
							SCSIDeviceIdentifier 		theT,
							SCSILogicalUnitNumber		theL,
							SCSITaggedTaskIdentifier	theQ );

	
	/*!
		@function FindTaskForControllerIdentifier
		@abstract Find a task for a given Target and Controller Task Identifier
		@discussion Allows the controller child class to find an outstanding task
		for a specified target and controller task identifier
		@param theTarget is the Target that the task .
		@param theIdentifier is the controller task identifier set using the SCSI
		Parallel Task's SetControllerTaskIdentifier() method.
		@result returns a valid SCSIParallelTaskIdentifier or NULL if none 
		found.
	*/
	
	SCSIParallelTaskIdentifier	FindTaskForControllerIdentifier ( 
							SCSIDeviceIdentifier 		theTarget,
							UInt64						theIdentifier );
	
	
	/*!
		@function ExecuteParallelTask
		@abstract Submit a SCSIParallelTask for execution.
		@discussion	The ExecuteParallelTask call is made by the client to submit 
		a SCSIParallelTask for execution.
		@param parallelRequest is a reference to the SCSIParallelTaskIdentifier
		to be executed.
		@result is an appropriate SCSIServiceResponse which are defined in the
		file <IOKit/scsi/SCSITask.h>.
	*/
	
	SCSIServiceResponse ExecuteParallelTask ( 
							SCSIParallelTaskIdentifier	parallelRequest );
	
	// --- Public API methods provided by HBA child classes ----
	
	/*!
		@function ReportHBAHighestLogicalUnitNumber
		@abstract Gets the Highest Logical Unit Number.
		@discussion	This method is used to query the HBA child class to 
		determine what the highest Logical Unit Number that the controller can 
		address.
		@result returns a valid 64-bit logical unit number.
	*/
	
	virtual SCSILogicalUnitNumber	ReportHBAHighestLogicalUnitNumber ( void ) = 0;
	
	/*!
		@function DoesHBASupportSCSIParallelFeature
		@abstract Queries the HBA child class to determine if it supports a 
		specific SPI feature.
		@discussion	Queries the HBA child class to determine if it supports the 
		specified feature as defined by the SCSI Parallel Interconnect 
		specifications.
		@result Returns true if requested feature is supported.
	*/
	
	virtual bool	DoesHBASupportSCSIParallelFeature ( 
							SCSIParallelFeature 		theFeature ) = 0;
	
	/*!
		@function InitializeTargetForID
		@abstract Called to initialize a target device.
		@discussion	This method will be called to initialize a target device in 
		a single-threaded manner.  The HBA can use this method to probe the 
		target or do anything else necessary before the device object is 
		registered with IOKit for matching.
		@result Returns true if the target was successfully initialized.
	*/
	
	virtual bool	InitializeTargetForID (  
							SCSITargetIdentifier 		targetID ) = 0;
	
	// The SCSI Task Management Functions as defined in the SCSI Architecture
	// Model - 2 (SAM-2) specification.  These are used by the client to request
	// the specified function.  The controller can complete these immmediately 
	// by returning the appropriate SCSIServiceResponse, or these can be completed
	// asyncronously by the controller returning a SCSIServiceResponse of
	// kSCSIServiceResponse_Request_In_Process and then calling the appropriate
	// function complete member routine listed in the child class API section.
	
	virtual SCSIServiceResponse	AbortTaskRequest ( 	
							SCSITargetIdentifier 		theT,
							SCSILogicalUnitNumber		theL,
							SCSITaggedTaskIdentifier	theQ ) = 0;
	
	virtual	SCSIServiceResponse AbortTaskSetRequest (
							SCSITargetIdentifier 		theT,
							SCSILogicalUnitNumber		theL ) = 0;
	
	virtual	SCSIServiceResponse ClearACARequest (
							SCSITargetIdentifier 		theT,
							SCSILogicalUnitNumber		theL ) = 0;
	
	virtual	SCSIServiceResponse ClearTaskSetRequest (
							SCSITargetIdentifier 		theT,
							SCSILogicalUnitNumber		theL ) = 0;
	
	virtual	SCSIServiceResponse LogicalUnitResetRequest (
							SCSITargetIdentifier 		theT,
							SCSILogicalUnitNumber		theL ) = 0;
	
	virtual	SCSIServiceResponse TargetResetRequest (
							SCSITargetIdentifier 		theT ) = 0;
	
	

	/*!
		@function DoesHBAPerformAutoSense
		@abstract Queries the HBA child class to determine if it automatically
		performs AutoSense and provides AutoSense data for each I/O. If the HBA
		allocates space for AutoSense in its HBA specific data region on a per
		task basis, the HBA should respond true.
		@discussion	Queries the HBA child class to determine if it automatically
		performs AutoSense and provides AutoSense data for each I/O. If the HBA
		allocates space for AutoSense in its HBA specific data region on a per
		task basis, the HBA should respond true.
		@result Return true if HBA performs AutoSense into its own private data
		buffer.
	*/
	
	OSMetaClassDeclareReservedUsed ( IOSCSIParallelInterfaceController, 1 );
	
	virtual bool	DoesHBAPerformAutoSense ( void );
	
	/*!
		@function ReportHBAConstraints
		@abstract Called to report the I/O constraints for this controller.
		A list of valid keys includes:
			kIOMaximumSegmentCountReadKey, (required)
			kIOMaximumSegmentCountWriteKey, (required)
			kIOMaximumSegmentByteCountReadKey, (required)
			kIOMaximumSegmentByteCountWriteKey, (required)
			kIOMinimumSegmentAlignmentByteCountKey, (required)
			kIOMaximumSegmentAddressableBitCountKey, (required)
			kIOMinimumHBADataAlignmentMaskKey (required)
			kIOHierarchicalLogicalUnitSupportKey (optional).
		NB: These keys and their values are described in this header and <IOKit/IOKitKeys.h>
		@param constraints. An OSDictionary object used to aggregate the key/value pairs.
		Subclasses must set the required keys if they override this method. If a subclass does
		not provide the required keys, the system will panic.
	*/
	OSMetaClassDeclareReservedUsed ( IOSCSIParallelInterfaceController, 2 );
	
	virtual void	ReportHBAConstraints ( OSDictionary * constraints );
	
	/*!
		@function DoesHBASupportMultiPathing
		@abstract Queries the HBA child class to determine if it supports
		Multi-Pathing.
		@discussion	Queries the HBA child class to determine if it supports
		Multi-Pathing.
		@result Returns true if requested feature is supported.
	*/
	OSMetaClassDeclareReservedUsed ( IOSCSIParallelInterfaceController, 3 );

	virtual bool	DoesHBASupportMultiPathing ( void );
							
	
	// Padding for the Client API
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 4 );
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 5 );
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 6 );
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 7 );
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 8 );
	
	
#if 0
#pragma mark -
#pragma mark Child Class API
#endif
	
	
protected:
	
	// ---- Target Creation and Destruction methods ---
	
	/*!
		@function CreateTargetForID
		@abstract Method to perform device creation.
		@discussion	For HBA child classes that report true to the 
		DoesHBAPerformDeviceManagement() method, the child class will be 
		responsible for all device management by using these methods;
		otherwise, the superclass will be responsible for all device management.
		This method must be used to perform SCSI Parallel Device creation and 
		cannot be overridden.
		@param  targetID SCSIDeviceIdentifier of desired targetID.
		@result returns true if successful.
	*/
	
	bool	CreateTargetForID ( SCSIDeviceIdentifier targetID );
	
	 /*!
		@function CreateTargetForID
		@abstract Method to perform device creation.
		@discussion	For HBA child classes that report true to the
		DoesHBAPerformDeviceManagement() method, the child class will be
		responsible for all device management by using these methods;
		otherwise, the superclass will be responsible for all device management.
		This method must be used to perform SCSI Parallel Device creation and
		cannot be overridden.
		@param  targetID SCSIDeviceIdentifier of desired targetID.
		@param	properties A dictionary of properties to associate with the device
				upon creation. The list of valid property keys is as follows:
				kIOPropertySASAddressKey,
				kIOPropertyFibreChannelNodeWorldWideNameKey,
				kIOPropertyFibreChannelPortWorldWideNameKey,
				kIOPropertyFibreChannelAddressIdentifierKey,
				kIOPropertyFibreChannelALPAKey, and
				kIOPropertyRetryCountKey
				These keys are defined in
				<IOKit/storage/IOStorageProtocolCharacteristics.h> and the values
				associated with these keys must be of the proper type/size,
				or the target creation will not succeed.
		@result returns true if successful.
	*/
	
	bool	CreateTargetForID ( SCSIDeviceIdentifier 	targetID,
								OSDictionary * 			properties );
	
	 /*!
		@function DestroyTargetForID
		@abstract Method to perform device destruction.
		@discussion	For HBA child classes that report true to the 
		DoesHBAPerformDeviceManagement() method, the child class will be 
		responsible for all device management by using these methods; otherwise, 
		the superclass will be responsible for all device management.
		This method must be used to perform SCSI Parallel Device destruction and
		cannot be overridden.
		@param  targetID SCSIDeviceIdentifier of desired targetID.
	*/
	
	void	DestroyTargetForID ( SCSIDeviceIdentifier targetID );
	
	/*!
		@function GetTargetForID
		@abstract Accessor for getting pointer to IOSCSIParallelInterfaceDevice.
		@param targetID SCSIDeviceIdentifier of desired targetID.
		@result returns pointer to IOSCSIParallelInterfaceDevice or NULL if not 
		found.
	*/
	
	IOSCSIParallelInterfaceDevice *	GetTargetForID ( 
							SCSIDeviceIdentifier 		targetID ); 
	
	/*!
		@function SetTargetProperty
		@abstract Accessor for setting a property for a specific target.
		@param device A pointer to a valid IOSCSIParallelInterfaceDevice.
		@param key A pointer to a valid OSString object which represents the key.
		A list of valid keys includes:
			kIOPropertySASAddressKey,
			kIOPropertyFibreChannelNodeWorldWideNameKey,
			kIOPropertyFibreChannelPortWorldWideNameKey,
			kIOPropertyFibreChannelAddressIdentifierKey, and
			kIOPropertyFibreChannelALPAKey.
		NB: These keys and their values are described in <IOKit/storage/IOStorageProtocolCharacteristics.h>
		@param value Pointer to an OSObject (one of type OSData, OSString, etc.)
		which represents the value for the property. The value must be of the proper type
		and size for the specified key.
		@result returns true if identifier was properly set, otherwise false. 
	*/
	
	bool	SetTargetProperty ( SCSIDeviceIdentifier 		targetID,
								const char *		 		key,
								OSObject *					value );

	/*!
		@function RemoveTargetProperty
		@abstract Accessor for removing a property from a specific target.
		@param device A pointer to a valid IOSCSIParallelInterfaceDevice.
		@param key A pointer to a valid OSString object which represents the key.
	*/
	
	void	RemoveTargetProperty ( SCSIDeviceIdentifier 		targetID,
								   const char *		 			key );
	
	// ---- Methods for HBA specifics. ----
	
	/*!
		@function SetHBAProperty
		@abstract Accessor for setting a property for this object.
		@param key A pointer to a valid OSString object which represents the key.
		A list of valid keys includes:
			kIOPropertyVendorNameKey,
			kIOPropertyProductNameKey,
			kIOPropertyProductRevisionLevelKey,
			kIOPropertyPortDescriptionKey,
			kIOPropertyPortSpeedKey,
			kIOPropertyPortTopologyKey,
			kIOPropertySCSIParallelSignalingTypeKey,
			kIOPropertyFibreChannelCableDescriptionKey,
			kIOPropertyFibreChannelNodeWorldWideNameKey,
			kIOPropertyFibreChannelPortWorldWideNameKey,
			kIOPropertyFibreChannelAddressIdentifierKey, 
			kIOPropertyFibreChannelALPAKey, and
			kIOPropertySASAddressKey.
		NB: These keys and their values are described in <IOKit/storage/IOStorageDeviceCharacteristics.h>
		and <IOKit/storage/IOStorageProtocolCharacteristics.h>
		@param value Pointer to an OSObject (one of type OSData, OSString, etc.)
		which represents the value for the property. The value must be of the proper type,
		and/or size for the specified key.
		@result returns true if identifier was properly set, otherwise false. 
	*/
	
	bool	SetHBAProperty ( const char *	key,
							 OSObject *	 	value );

	/*!
		@function RemoveHBAProperty
		@abstract Accessor for removing a property for this object.
		@param key A pointer to a valid OSString object which represents the key.
		See the SetHBAProperty() method for a list of valid keys.
	*/
	
	void	RemoveHBAProperty ( const char * key );
	
	// These methods will not be called before the InitializeController() call,
	// and will not be called after the TerminateController() call.  But in the
	// interval between those calls, they shall report the correct requested
	// information. They are implemented as seperate pure virtual methods
	// instead of a selector driven method because the HBA child class is
	// required to report this information.
	
	/*!
		@function ReportInitiatorIdentifier
		@abstract Get the SCSI Device Identifier for the HBA.
		@discussion This method will be called to determine the SCSI Device 
		Identifier that the Initiator has assigned for this HBA.
		@result returns SCSIInitiatorIdentifier.
	*/
	
	virtual SCSIInitiatorIdentifier	ReportInitiatorIdentifier ( void ) = 0;
	
	/*!
		@function ReportHighestSupportedDeviceID
		@abstract Get the highest supported SCSI Device Identifier.
		@discussion This method will be called to determine the value of the 
		highest SCSI Device Identifier supported by the HBA. This value will be 
		used to determine the last ID to process.
		@result returns highest SCSIDeviceIdentifier
	*/
	
	virtual SCSIDeviceIdentifier	ReportHighestSupportedDeviceID ( void ) = 0;
	
	/*!
		@function ReportMaximumTaskCount
		@abstract Report Maximum Task Count
		@discussion This method will be called to retrieve the maximum number of
		outstanding tasks the HBA can process. This number must be greater than
		zero or the controller driver will fail to match and load.
		@result returns maximum (non-zero) task count.
	*/
	
	virtual UInt32		ReportMaximumTaskCount ( void ) = 0;
	
  	/*!
		@function ReportHBASpecificTaskDataSize
		@abstract Determine memory needed for HBA Task specific use.
		@discussion This method is used to retrieve the amount of memory that 
		will be allocated in the SCSI Parallel Task for HBA specific use.
		@result returns memory required in bytes
	*/
	
	virtual UInt32		ReportHBASpecificTaskDataSize ( void ) = 0;
	
  	/*!
		@function ReportHBASpecificDeviceDataSize
		@abstract  Determine memory needed for HBA Device specific use.
		@discussion This method is used to retrieve the amount of memory that 
		will be allocated in the SCSI Parallel Device for HBA specific use.
		@result  returns memory required in bytes
	*/
	
	virtual UInt32		ReportHBASpecificDeviceDataSize ( void ) = 0;
	
  	/*!
		@function DoesHBAPerformDeviceManagement
		@abstract  Determine if HBA will manage devices.
		@discussion This method is used to determine if the HBA will manage 
		target device creation and destruction.  
		@result return true means objects for target devices will only be 	
		created when the child class calls the CreateTargetForID method.
	*/
	
	virtual bool		DoesHBAPerformDeviceManagement ( void ) = 0;
	
	// ---- Initialize and Terminate methods for the subclass to implement -----
	// The subclass shall not override the IOKit init and terminate methods,
	// but shall instead rely on these methods for initialization and
	// termination.
	
	// This is done to allow for this superclass to manage all IOKit specifics 
	// and to require only a Family specific API to be implemented by the
	// subclass drivers.
	
  	/*!
		@function InitializeController
		@abstract  Called to initialize the controller
		@discussion It is guaranteed that the InitializeController() will only be 
		called once per instantiation.  The InitializeController() methods allows 
		the subclass driver to do all the necessary initialization required by 
		the hardware before it is able to accept requests to execute. All 
		necessary allocation of resources should be made during this method 
		call. This is the first method that will be called in the subclass.
		@result return true means that initialization was successful.
	*/
	
	virtual bool	InitializeController ( void ) = 0;
	
  	/*!
		@function TerminateController
		@abstract  Called to terminate the controller
		@discussion It is guaranteed that the TerminateController() will only be 
		called once and only after the InitializeController() method and only if 
		true was returned in response to the InitializeController() method.
		The TerminateController() method allows the subclass to release all 
		resources that were acquired for operation of the hardware and shutdown 
		all hardware services.
		This is the last method of the subclass that will be called before the 		
		class is destroyed.
	*/
	
	virtual void	TerminateController ( void ) = 0;
	
	// ---- Start and Stop methods for the subclass ----
	
	/*!
		@function StartController
		@abstract Called to start the controller
		@discussion The StartController will always be called before any 
		requests are sent to the driver for execution. This method is called 
		after an initialize to start the services provided by the specific HBA 
		driver or called after a StopController call to restart those services. 
		After this call completes, all services provided by the HBA driver are 
		available to the client.
		@result return true means that start was successful.
	*/
	
	virtual bool	StartController ( void ) = 0;
	
	/*!
		@function StopController
		@abstract Called to stop the controller
		@discussion The StopController method will be called any time that the 
		system wants the card to stop accepting requests. ( See StartController 
		discussion ). The subclass should disable the hardware interrupt for
		the particular controller (if possible) in this method.
	*/
	
	virtual void	StopController ( void ) = 0;
	
	// ---- Suspend and Resume Methods for the subclass ----
	
	/*!
		@function SuspendServices
		@abstract Called to suspend controller services
		@discussion Method will be called when the system wants to suspend the
		services that are provided by the HBA driver. This call is not a reset
		and the driver shall retain all state data between this so that if a
		ResumeServices call is received, the driver can continue providing
		services without a visible difference to the client. The driver may 
		receive multiple SuspendServices calls without receiving a 
		ResumeServices call and should ignore any after the first until a 
		ResumeServices call is received.
	*/
	
	virtual void	SuspendServices ( void );
	
	/*!
		@function ResumeServices
		@abstract Called to resume controller services
		@discussion Method that will be called to resume services
		provided by the driver. ( See SuspendServices discussion )
	*/
	
	virtual void	ResumeServices ( void );
	
	/*!
		@function HandleInterruptRequest
		@abstract Handle Interrupt Request
		@discussion The HandleInterruptRequest is used to notify an HBA 
		specific subclass that an interrupt request needs to be serviced. It is 
		called on the workloop (it holds the gate) at secondary interrupt level.
	*/
	
	virtual void	HandleInterruptRequest ( void ) = 0;
	
	/*!
		@function EnableInterrupt
		@abstract Enable Interrupt
		@discussion Method that the HBA child class can call to enable 
		the associated IOInterruptEventSource.
	*/
	
	void	EnableInterrupt ( void );
	
	/*!
		@function DisableInterrupt
		@abstract Disable Interrupt
		@discussion Method that the HBA child class can call to disable 
		the associated IOInterruptEventSource.
	*/
	
	void	DisableInterrupt ( void );
	
	/*!
		@function SignalInterrupt
		@abstract Signals that an interrupt has occurred.
		@discussion Subclasses of IOSCSIParallelInterfaceController
		should call this method in order to get the secondary interrupt
		thread scheduled if and only if they will be returning false from
		their overriden FilterInterruptRequest() method. See the
		discussion for the FilterInterruptRequest() method for more
		details.
		
		NOTE: This method should only be called from within the
		FilterInterruptRequest() method and at no other time.
		
		Available in 10.3.3 or later.
		
	*/
	
	void	SignalInterrupt ( void );
	
	/*!
		@function ProcessParallelTask
		@abstract Called by client to process a parallel task.
		@discussion This method is called to process a parallel task (i.e. put
		the command on the bus). The HBA specific sublcass must implement this 
		method.
		@param parallelRequest A valid SCSIParallelTaskIdentifier.
		@result serviceResponse (see <IOKit/scsi/SCSITask.h>)
	*/
	
	virtual SCSIServiceResponse ProcessParallelTask (
							SCSIParallelTaskIdentifier parallelRequest ) = 0;
	
	/*!
		@function CompleteParallelTask
		@abstract Parallel Task Completion
		@discussion The HBA specific sublcass inherits the CompleteParallelTask() 
		method which shall be called when the HBA has completed the processing 
		of a parallel task.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param completionStatus The status of the SCSI bus.
		@param serviceResponse (see <IOKit/scsi/SCSITask.h>)
	*/
	
	void	CompleteParallelTask (
						SCSIParallelTaskIdentifier	parallelRequest,
						SCSITaskStatus 				completionStatus,
						SCSIServiceResponse 		serviceResponse );
	
	
	// Completion routines for the SCSI Task Management functions as described
	// in the SCSI ArchitectureModel - 2 (SAM-2) specification.  Each of these
	// correspond to a client request for the specific Task Management functions.
	// If the Controller Child Class completed the request by returning a 
	// SCSIServiceResponse of anything other than kSCSIServiceResponse_Request_In_Process,
	// then the controller class does not need to call the completion member routine.
	// If the controller did not complete the request immediately, then it will
	// need to call the appropriate completion member routine listed here.
	void	CompleteAbortTask ( 	
						SCSITargetIdentifier 		theT,
						SCSILogicalUnitNumber		theL,
						SCSITaggedTaskIdentifier	theQ,
						SCSIServiceResponse 		serviceResponse );
	
	void 	CompleteAbortTaskSet (
						SCSITargetIdentifier 		theT,
						SCSILogicalUnitNumber		theL,
						SCSIServiceResponse 		serviceResponse );
	
	void 	CompleteClearACA (
						SCSITargetIdentifier 		theT,
						SCSILogicalUnitNumber		theL,
						SCSIServiceResponse 		serviceResponse );
	
	void 	CompleteClearTaskSet (
						SCSITargetIdentifier 		theT,
						SCSILogicalUnitNumber		theL,
						SCSIServiceResponse 		serviceResponse );
	
	void 	CompleteLogicalUnitReset (
						SCSITargetIdentifier 		theT,
						SCSILogicalUnitNumber		theL,
						SCSIServiceResponse 		serviceResponse );
	
	void 	CompleteTargetReset (
						SCSITargetIdentifier 		theT,
						SCSIServiceResponse 		serviceResponse );
	
	/*!
		@function NotifyClientsOfBusReset
		@abstract Method called to notify clients that a bus reset has occurred.
		@discussion This method is used by the HBA child class to inform the 
		parent class and any clients that a bus reset has occurred.
	*/
	
	void	NotifyClientsOfBusReset ( void );
	
	/*!
		@function NotifyClientsOfPortStatusChange
		@abstract Method called to notify clients of port status change events.
		@discussion This method is used by the HBA child class to inform the 
		parent class and any clients that a port has changed status.
	*/
	
	void	NotifyClientsOfPortStatusChange ( SCSIPortStatus newStatus );
	
	/*!
		@function GetSCSIDomainIdentifier
		@abstract Accessor method to get the SCSI Domain Identifier.
		@discussion Accessor method to get the SCSI Domain Identifier.
		@result returns SCSI Domain Identifier.
	*/
	
	SInt32	GetSCSIDomainIdentifier ( void );
	
	/*!
		@function GetProvider
		@abstract Accessor method to get the IOService which is the controller's
		provider.
		@discussion Accessor method to get the IOService which is the 
		controller's provider.
		@result returns pointer to IOService.
	*/
	
	IOService *		GetProvider ( void );
	
	/*!
		@function GetWorkLoop
		@abstract Accessor method to get the IOWorkLoop associated with this 
		HBA.
		@discussion Accessor method to get the IOWorkLoop associated with this 
		HBA.
		@result returns pointer to IOWorkLoop.
	*/
	
	IOWorkLoop *	GetWorkLoop ( void ) const;
	
	/*!
		@function GetCommandGate
		@abstract Accessor to get an IOCommandGate associated with the workloop.
		@discussion Accessor to get an IOCommandGate associated with the 
		workloop.
		@result returns pointer to IOCommandGate.
	*/
	
	IOCommandGate *		GetCommandGate ( void );
	
	// ---- SCSI Parallel Task Object Accessors ----
	
	/*!
		@function GetSCSITaskIdentifier
		@abstract Method to retrieve a SCSITaskIdentifier from a valid 
		SCSIParallelTaskIdentifier.
		@discussion Method to retrieve a SCSITaskIdentifier from a valid 
		SCSIParallelTaskIdentifier.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns SCSITaskIdentifier that represents the original request 
		from the SCSI Application Layer client.
	*/
	
	SCSITaskIdentifier	GetSCSITaskIdentifier ( 
							SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function GetTargetIdentifier
		@abstract Method to get the SCSITargetIdentifier associated with a 
		request.
		@discussion	Method to get the SCSITargetIdentifier associated with a 
		request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns SCSITargetIdentifier 
	*/
	
	SCSITargetIdentifier	GetTargetIdentifier ( 
							SCSIParallelTaskIdentifier 	parallelTask );
	
	// ---- Methods for Accessing data in the client's SCSI Task Object ----	
	// Method to retrieve the LUN that identifies the Logical Unit whose Task
	// Set to which this task is to be added.
	
	/*!
		@function GetLogicalUnitNumber
		@abstract Method to get the logical unit number associated with a 
		request.
		@discussion Method to get the logical unit number associated with a 
		request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns a valid 64-bit logical unit number.
	*/
	
	SCSILogicalUnitNumber	GetLogicalUnitNumber ( 
							SCSIParallelTaskIdentifier 	parallelTask ); // DEPRECATED, use GetLogicalUnitBytes instead.
	
	/*!
		@function GetLogicalUnitBytes
		@abstract Method to get the logical unit bytes associated with a
		request.
		@discussion Method to get the logical unit bytes associated with a
		request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns a valid 8-byte logical unit address.
	*/

    void    GetLogicalUnitBytes (
                            SCSIParallelTaskIdentifier  parallelTask,
                            SCSILogicalUnitBytes *      logicalUnitBytes );

	/*!
		@function GetTaggedTaskIdentifier
		@abstract Method to retrieve the SCSI Tagged Task Identifier of the 
		task.  If the returned value is equal to kSCSIUntaggedTaskIdentifier,
		then this task is untagged.  
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result an SCSITaskAttribute value.
	*/
	
	SCSITaggedTaskIdentifier GetTaggedTaskIdentifier (
							SCSIParallelTaskIdentifier	parallelTask );
	
	/*!
		@function GetTaskAttribute
		@abstract Method to retrieve the SCSI Task Attribute of the task 
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result an SCSITaskAttribute value.
	*/
	
	SCSITaskAttribute		GetTaskAttribute (
							SCSIParallelTaskIdentifier	parallelTask );
	
	/*!
		@function GetCommandDescriptorBlockSize
		@abstract Method to retrieve the size of the SCSI Command Descriptor 
		Block (CDB).
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns the size of the SCSI Command Descriptor Block in bytes.
	*/
	
	UInt8	GetCommandDescriptorBlockSize ( 
							SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function GetCommandDescriptorBlock
		@abstract Method to retrieve the SCSI Command Descriptor Block (CDB).
		@discussion This will always return a 16 Byte CDB. If the Protocol Layer 
		driver does not support 16 Byte CDBs, it will have to create a local 
		SCSICommandDescriptorBlock variable to get the CDB data and then 
		transfer the needed bytes from there.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param cdbData is a SCSICommandDescriptorBlock pointer to 16 byte CDB
		@result returns true if data was copied to cdbData pointer
	*/
	
	bool	GetCommandDescriptorBlock (
							SCSIParallelTaskIdentifier 		parallelTask,
							SCSICommandDescriptorBlock * 	cdbData );
	
	/*!
		@function GetDataTransferDirection
		@abstract Retrieves the data transfer direction for any data associated
		with the request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result One of the valid data transfer directions described in 
		<IOKit/scsi/SCSITask.h>
	*/
	
	UInt8	GetDataTransferDirection ( SCSIParallelTaskIdentifier parallelTask );
	
	/*!
		@function GetRequestedDataTransferCount
		@abstract Retrieves the requested data transfer count for any data 
		associated with the request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result The requested data transfer count in bytes.
	*/
	
	UInt64	GetRequestedDataTransferCount ( 
							SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function GetRealizedDataTransferCount
		@abstract Retrieves the realized data transfer count for any data 
		associated with the request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result The realized data transfer count in bytes.
	*/
	
	UInt64	GetRealizedDataTransferCount (
							SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function SetRealizedDataTransferCount
		@abstract Sets the realized data transfer count in bytes.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param realizedTransferCountInBytes is the number of bytes actually 
		transferred.
		@result true means the data transfer count was successfully set.
	*/
	
	bool	SetRealizedDataTransferCount ( 
							SCSIParallelTaskIdentifier 	parallelTask,
							UInt64 		realizedTransferCountInBytes );
	
	/*!
		@function IncrementRealizedDataTransferCount
		@abstract Increments the realized data transfer count. This method is
		helpful for when the HBA has to do multiple passes of DMA because there 
		are more scatter-gather elements than it can process in one pass.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param realizedTransferCountInBytes is the number of bytes to add to the
		realized data count for the task.
	*/
	
	void	IncrementRealizedDataTransferCount (
							SCSIParallelTaskIdentifier 	parallelTask,
							UInt64 		realizedTransferCountInBytes );
	
	/*!
		@function GetDataBuffer
		@abstract Method to retrieve client buffer from the request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns pointer to an IOMemoryDescriptor which represents the 
		buffer.
	*/
	
	IOMemoryDescriptor * GetDataBuffer ( 
							SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function GetDataBufferOffset
		@abstract Method to retrieve offset into client buffer at which to start
		processing.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns offset in bytes
	*/
	
	UInt64	GetDataBufferOffset ( SCSIParallelTaskIdentifier parallelTask );

	/*!
		@function GetDMACommand
		@abstract Method to retrieve a pointer to an IODMACommand from the request.
		@discussion For devices utilizing DMA, the IODMACommand object should be 
		obtained via GetDMACommand(). The subclass is responsible for calling prepare()
		on the IODMACommand object using the proper offset obtained via GetDataBufferOffset()
		and correct size obtained via GetRequestedDataTransferCount(). The subclass
		is further responsible for calling complete() on the IODMACommand object once
		all DMA operations have finished.
		NB: Subclasses should not call IODMACommand::setMemoryDescriptor().
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns pointer to an IODMACommand which is used in conjunction
		with the task.
	*/
	
	IODMACommand * GetDMACommand ( 
							SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function GetTimeoutDuration
		@abstract Method to retrieve the timeout duration in milliseconds for a 
		request.
		@discussion Method to retrieve the timeout duration in milliseconds for
		a request. A value of zero represents an infinite timeout, or on 
		hardware where infinite timeouts are not possible, substitute the 
		longest timeout possible.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns timeout duration in milliseconds
	*/
	
	UInt32	GetTimeoutDuration ( SCSIParallelTaskIdentifier parallelTask );

	/*!
		@function SetAutoSenseData
		@abstract Method to set the auto sense data buffer associated with a 
		request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param newSensedata pointer to auto sense data buffer
		@result returns true if data in newSenseData was succesfully into the 
		task object
	*/
	
	bool	SetAutoSenseData ( 
							SCSIParallelTaskIdentifier 	parallelTask,
							SCSI_Sense_Data * 			newSenseData,
							UInt8						senseDataSize );
	
	/*!
		@function GetAutoSenseData
		@abstract Method to retrieve auto sense data buffer associated with a 
		request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param receivingBuffer pointer to auto sense data buffer
		@result returns true if successfully copied data into receivingBuffer
	*/
	
 	bool	GetAutoSenseData ( 
 							SCSIParallelTaskIdentifier 	parallelTask,
 							SCSI_Sense_Data * 			receivingBuffer,
 							UInt8						senseDataSize );
 	
	/*!
		@function GetAutoSenseDataSize
		@abstract Method to retrieve auto sense data buffer size associated with a 
		request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns Size of auto sense data buffer.
	*/
	
 	UInt8	GetAutoSenseDataSize ( 
 							SCSIParallelTaskIdentifier 	parallelTask );
 	
	
	/*!
		@function GetSCSIParallelFeatureNegotiation
		@abstract Method to retrieve the requested value for negotiation of the.
		@discussion Query as to whether the SCSI Parallel Device object has 
		negotiated wide data transfers.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result A valid SCSIParallelFeatureControl.
	*/
	
	SCSIParallelFeatureRequest		GetSCSIParallelFeatureNegotiation ( 
							SCSIParallelTaskIdentifier 	parallelTask,
							SCSIParallelFeature 		requestedFeature );
	
	/*!
		@function GetSCSIParallelFeatureNegotiationCount
		@abstract Method to retrieve the number of requested negotiations.
		@discussion Query as to the number of SCSI Parallel Features that are
		requested to either be negotitated or cleared.  These are all features
		that are set to either kSCSIParallelFeature_AttemptNegotiation or 
		kSCSIParallelFeature_ClearNegotiation.  If the return value is zero,
		then all features are set to kSCSIParallelFeature_NoNegotiation
		and all feature negotiations are to remain as they currently exist.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result an unsigned integer up to 64 bits in size.
	*/
	
	UInt64		GetSCSIParallelFeatureNegotiationCount ( 
							SCSIParallelTaskIdentifier 	parallelTask);
	
	/*!
		@function SetSCSIParallelFeatureNegotiationResult
		@abstract Method to set the wide data transfer negotiation result.
		@discussion Method to set the wide data transfer negotiation result.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param requestedFeature The SCSIParallelFeature that the has been set to 	
		newResult.
		@param newResult A valid SCSIParallelFeatureResult value.
	*/
	
	void		SetSCSIParallelFeatureNegotiationResult ( 
							SCSIParallelTaskIdentifier 	parallelTask,
							SCSIParallelFeature 		requestedFeature, 
							SCSIParallelFeatureResult 	newResult );
	
	/*!
		@function GetSCSIParallelFeatureNegotiationResult
		@abstract Method to retrieve the result of any wide transfer 
		negotiations.
		@discussion Query as to whether the SCSI Parallel Controller object has 
		negotiated wide data transfers.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result A valid SCSIParallelFeatureResult.
	*/
	
	SCSIParallelFeatureResult		GetSCSIParallelFeatureNegotiationResult ( 
							SCSIParallelTaskIdentifier 	parallelTask,
							SCSIParallelFeature 		requestedFeature );
	
	/*!
		@function GetSCSIParallelFeatureNegotiationResultCount
		@abstract Method to retrieve the number of changed negotiations.
		@discussion Query as to the number of SCSI Parallel Features that have
		been changed to either negotitated or cleared.  These are all features
		that are set to either kSCSIParallelFeature_NegotitiationCleared or 
		kSCSIParallelFeature_NegotitiationSuccess.  If the return value is zero,
		then all features are set to kSCSIParallelFeature_NegotitiationUnchanged.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result an unsigned integer up to 64 bits in size.
	*/
	
	UInt64		GetSCSIParallelFeatureNegotiationResultCount ( 
							SCSIParallelTaskIdentifier 	parallelTask);
	
	// Controller Task Identifier related member routines
	
	/*!
		@function SetControllerTaskIdentifier
		@abstract Method to set the Controller Task Identifier.
		@discussion This method allows the Controller Child Class
		driver to set a unique identifier to associate with the specified
		SCSI Parallel Task.  This identifier is designed to be used by
		controllers that do not have access to the LUN and Tag information
		when notified by the HBA that a request has completed.
		If the kSCSIParallelTaskControllerIDQueueHead is used, this
		member routine will return the first Task on the queue.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param newIdentifier unsigned 64 bit integer token.
		@result none
	*/
	
	void	SetControllerTaskIdentifier (
							SCSIParallelTaskIdentifier 	parallelTask,
							UInt64 						newIdentifier );
	
	UInt64	GetControllerTaskIdentifier (
							SCSIParallelTaskIdentifier 	parallelTask);
	
	
	// The HBA Data related fields
	
	/*!
		@function GetHBADataSize
		@abstract Method to retrieve the HBA Data Size in bytes.
		@discussion Method to retrieve the HBA Data Size in bytes.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns HBA Data size in bytes.
	*/
	
	UInt32	GetHBADataSize ( SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function GetHBADataPointer
		@abstract Method to retrieve the HBA Data pointer.
		@discussion Method to retrieve the HBA Data pointer.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns pointer to buffer for HBA specific data, NULL if 
		none found or GetHBADataSize() returns zero.
	*/
	
	void *	GetHBADataPointer ( SCSIParallelTaskIdentifier 	parallelTask );
	
	/*!
		@function GetHBADataDescriptor
		@abstract Method to retrieve the IOMemoryDescriptor associated with
		the HBA Data.
		@discussion Method to retrieve the IOMemoryDescriptor associated with
		the HBA Data.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@result returns pointer to an IOMemoryDescriptor that wraps the HBA 
		specific data buffer, NULL if none found or GetHBADataSize() returns zero.
	*/
	
	IOMemoryDescriptor *	GetHBADataDescriptor (
							SCSIParallelTaskIdentifier 	parallelTask );
	
	// ---- SCSI Parallel Device Object Accessors ----
	
	// The HBA Data related fields
	
	/*!
		@function GetHBATargetDataSize
		@abstract Method to retrieve the HBA Data Size in bytes.
		@discussion Method to retrieve the HBA Data Size in bytes.
		@param targetDevice A valid SCSITargetIdentifier.
		@result returns HBA Data size in bytes.
	*/
	
	UInt32	GetHBATargetDataSize ( SCSITargetIdentifier 	targetID );
	
	/*!
		@function GetHBATargetDataPointer
		@abstract Method to retrieve the HBA Data pointer.
		@discussion Method to retrieve the HBA Data pointer.
		@param targetDevice A valid SCSITargetIdentifier.
		@result returns pointer to buffer for HBA specific data, NULL if 
		none found or GetHBADataSize is zero.
	*/
	
	void *	GetHBATargetDataPointer ( SCSITargetIdentifier 	targetID );
	
	
#if 0
#pragma mark -
#pragma mark Additional Child Class APIs
#endif
	
	
	// ---- Timeout Related Methods ----
	
	/*!
		@function SetTimeoutForTask
		@abstract Method to set the timeout duration in milliseconds for a 
		request.
		@discussion Method to set the timeout duration in milliseconds for a 
		request.
		@param parallelTask A valid SCSIParallelTaskIdentifier.
		@param timeoutOverride A timeout value in milliseconds in case the
		HBA driver wishes to override the default value provided in the
		parallelTask.
	*/
	
	void	SetTimeoutForTask ( SCSIParallelTaskIdentifier 	parallelTask,
								UInt32						timeoutOverride = 0 );
	
	/*!
		@function HandleTimeout
		@abstract Method to handle command timeouts.
		@discussion Method to handle command timeouts. This should
		be overridden by the child class in order to clean up HBA
		specific structures after a timeout has occurred. This method
		is called on the workloop (it holds the gate).
		@param parallelRequest A valid SCSIParallelTaskIdentifier.
	*/
	
	OSMetaClassDeclareReservedUsed ( IOSCSIParallelInterfaceController, 9 );
	
	virtual void		HandleTimeout (
							SCSIParallelTaskIdentifier parallelRequest );
	
	
	// ---- Filter Interrupt ----
	
	/*!
		@function FilterInterruptRequest
		@abstract Filter method called at primary interrupt time.
		@discussion Filter method called at primary interrupt time.
		This should only be overridden by the child class in order
		to determine if an interrupt occurred for this controller instance.
		Since all work occurs at primary interrupt time, this routine
		should be quick and efficient and defer as much processing as
		possible to the HandleInterruptRequest() method.
		
		NOTE: Unlike the HandleInterruptRequest() and HandleTimeout()
		methods, FilterInterruptRequest() is NOT called with the
		workloop lock held.
		
		If the value returned by FilterInterruptRequest() is true, the
		secondary interrupt thread will be scheduled and the hardware
		interrupt line will be disabled. If the controller instance shares
		that interrupt line with other devices, it can cause large
		interrupt latencies. If the controller instance can disable the
		interrupt in the chip itself, the following can be done to reduce
		interrupt latencies:
		
		- Interrupt occurs
		- FilterInterruptRequest() method is called.
			- If the interrupt is not for this controller, return false
			  immediately.
			- If the interrupt is for this controller, and the controller
			  can disable interrupts for this chip, the controller should
			  disable the interrupts for this chip, call SignalInterrupt(),
			  and return false. This causes the secondary interrupt thread
			  to get scheduled, yet does not disable the interrupt line for
			  all devices tied to that interrupt. This effectively allows
			  other devices to process their interrrupts, thus reducing
			  interrupt latency for those devices.
		- HandleInterruptRequest() method is called.
			- Controller processes interrupt and completes I/O requests.
			- Controller re-enables interrupts for the device.
		
		NOTE: If you use this approach, the interrupting condition MUST be
		cleared from the hardware, otherwise an infinite process interrupt
		loop will occur.
		
		If the controller cannot disable interrupts on the chip, it should
		simply return true if an interrupt has occurred for its device.
		
		@result True if the hardware interrupt line should be disabled,
		otherwise false.
	*/
	
	OSMetaClassDeclareReservedUsed ( IOSCSIParallelInterfaceController, 10 );
	
	virtual bool		FilterInterruptRequest ( void );
	
	/*!
		@function InitializeDMASpecification
		@abstract Called to initialize an IODMACommand with a DMA specification.
		@param command A pointer to a valid IODMACommand object. Subclasses
		should override this method and call IODMACommand::initWithSpecification()
		supplying the proper arguments to that method based on the DMA strategy.
		@result boolean value indicating success or failure.
	*/
	OSMetaClassDeclareReservedUsed ( IOSCSIParallelInterfaceController, 11 );
	
	virtual bool	InitializeDMASpecification ( IODMACommand * command );
	
	/*!
		@function CreateDeviceInterrupt
		@abstract Called to create an IOInterruptEventSource for the device. Subclasses
		may wish to use a different interrupt index than 0 (e.g. for using PCI Message
		Signaled Interrupts) or might not need an interrupt at all (virtual HBA).
		@param action A pointer to the action routine that should be passed to either
		IOInterruptEventSource::interruptEventSource() or
		IOFilterInterruptEventSource::filterInterruptEventSource as the method to call
		when an interrupt occurs for the device (sometimes called the "deferred procedure call"
		or the "secondary context method". By passing this routine along, it will
		properly wire up the HandleInterruptRequest() method you should override to handle
		interrupts.
		@param filter A pointer to the filter routine that should be passed to
		IOFilterInterruptEventSource::filterInterruptEventSource as the method to call
		at primary interrupt time when an interrupt occurs for the device.
		By passing this routine along, it will properly wire up the
		FilterInterruptRequest() method you may override to handle primary interrupts.
		@result IOInterruptEventSource. May return NULL if and only if there is no
		hardware interrupt associated with this device.
	*/
	OSMetaClassDeclareReservedUsed ( IOSCSIParallelInterfaceController, 12 );
	
	virtual IOInterruptEventSource *	CreateDeviceInterrupt (
											IOInterruptEventSource::Action			action,
											IOFilterInterruptEventSource::Filter	filter,
											IOService *								provider );
	
	// Padding for the Child Class API
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 13 );
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 14 );
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 15 );
	OSMetaClassDeclareReservedUnused ( IOSCSIParallelInterfaceController, 16 );
	
#if 0
#pragma mark -
#pragma mark Internal Use Only
#endif
	
private:
	
	// binary compatibility instance variable expansion
	struct ExpansionData { };
	ExpansionData * fIOSCSIParallelInterfaceControllerExpansionData;
	
	IOService *					fProvider;
	OSSet *						fClients;
	
	static SInt32				fSCSIParallelDomainCount;
	SInt32						fSCSIDomainIdentifier;
	
	// The HBA attributes
	SCSIInitiatorIdentifier		fInitiatorIdentifier;
	
	// The maximum SCSI Device Identifier support by the HBA
	// This is retreived from the child class via the 
	SCSIDeviceIdentifier		fHighestSupportedDeviceID;
	
	// The total number of tasks that the HBA can proccess at a time.
	// This is retrieved from the child class via ReportMaximumTaskCount
	UInt32						fSupportedTaskCount;
	
	// The Number of requests that are currently outstanding for the current
	// instantiation.
	UInt16						fOutstandingRequests;
	
	// The member variable to indicate if the current instantiation has been
	// succesfully intialized.
	bool						fHBAHasBeenInitialized;
	
	// The member variable to indicate if the current instantiation is running.
	// A true means that the last or only Start call made was successful.  A
	// false value means that either a successful Start has not been made or a
	// Stop call has been made.
	bool						fHBACanAcceptClientRequests;
	
	// The pool for the available SCSI Parallel Task objects
	IOCommandPool *				fParallelTaskPool;
	
	// WorkLoop variables
	IOWorkLoop *				fWorkLoop;
	IOTimerEventSource *		fTimerEvent;
	IOInterruptEventSource *	fDispatchEvent;
	
	IOCommandGate *				fControllerGate;
	
	bool						AllocateSCSIParallelTasks ( void );
	void						DeallocateSCSIParallelTasks ( void );
	
	IOWorkLoop *				getWorkLoop ( void ) const;
	bool 						CreateWorkLoop ( IOService * provider );
	void 						ReleaseWorkLoop ( void );
	
	// SCSI Parallel Device List
	// The SCSI Parallel Device List will consist of 16 elements to represent 
	// identifiers that end in 0h through Fh.  Each array element will point
	// to a device object that represents the beginning of a linked list of
	// device objects.  By using an array of linked lists, the traversal time
	// to find an object on a bus that supports a large number of devices, such
	// as Fibre Channel, will be significantly lower than having to walk a list 
	// that is comprised of all devices on the bus.  For parallel wide and 
	// narrow busses, which support 16 and 8 devices respectively, this will act 
	// like a simple array of device objects.
	enum
	{
		kSCSIParallelDeviceListArrayCount 	= 16,
		kSCSIParallelDeviceListIndexMask	= 0x0F
	};
	
	IOSimpleLock * 					fDeviceLock;
	IOSCSIParallelInterfaceDevice *	
					fParallelDeviceList[kSCSIParallelDeviceListArrayCount];
	
	void			InitializeDeviceList ( void );
	void			AddDeviceToTargetList ( 
							IOSCSIParallelInterfaceDevice *	newDevice );
	void			RemoveDeviceFromTargetList ( 
							IOSCSIParallelInterfaceDevice * victimDevice );
	void			CompleteOutstandingTasksForTargetID ( SCSIDeviceIdentifier targetID );
	void			CompleteOutstandingTasksForTarget ( IOSCSIParallelInterfaceDevice * device );
	
	// The Interrupt Service Routine for the controller.
	static void		ServiceInterrupt (
							OSObject *					theObject, 
							IOInterruptEventSource *	theSource,
							int							count );
	
	static void		TimeoutOccurred ( OSObject * owner, IOTimerEventSource * sender );
	
	static bool		FilterInterrupt (
							OSObject *						theObject,
							IOFilterInterruptEventSource *	theSource );
	
	// IOService support methods
	// These shall not be overridden by the HBA child classes.
	bool			start ( IOService *                 provider );
	void			stop ( 	IOService *  				provider );
	
	
protected:
	
	// These may be overriden by the HBA child classes if necessary, but should
	// call the superclass implementation.
	virtual bool	handleOpen ( 
							IOService * 				client, 
							IOOptionBits 				options, 
							void * 						arg );

	virtual void	handleClose ( 
							IOService * 				client, 
							IOOptionBits 				options );

	virtual bool	handleIsOpen ( 
							const IOService * 			client ) const;
	
	virtual bool	willTerminate ( IOService * provider, IOOptionBits options );
	virtual bool	didTerminate ( IOService * provider, IOOptionBits options, bool * defer );
	virtual void	free ( void );
};

__exported_pop;

#endif	/* __IOKIT_IO_SCSI_PARALLEL_INTERFACE_CONTROLLER_H__ */
