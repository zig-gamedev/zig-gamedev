/*
 * Copyright (c) 1998-2011 Apple Inc. All rights reserved.
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


#ifndef _IOKIT_IO_SCSI_PROTOCOL_SERVICES_H_
#define _IOKIT_IO_SCSI_PROTOCOL_SERVICES_H_

#if defined(KERNEL) && defined(__cplusplus)


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// Mach includes
#include <kern/queue.h>

// General IOKit headers
#include <IOKit/IOLib.h>
#include <IOKit/IOCommandGate.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi/SCSITask.h>
#include <IOKit/scsi/IOSCSIProtocolInterface.h>
#include <IOKit/scsi/SCSICmds_REQUEST_SENSE_Defs.h>

// Build includes
#include <TargetConditionals.h>


//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------

// Power Management values
enum
{
	kSCSIProtocolLayerPowerStateOff				= 0,
	kSCSIProtocolLayerPowerStateOn				= 1,
	kSCSIProtocolLayerNumDefaultStates			= 2
};

// Forward definitions of internal use only classes
class SCSITask;

//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------

/*!
 @class IOSCSIProtocolServices
 @superclass IOSCSIProtocolInterface
 @discussion
 This class defines the public SCSI Protocol Services Layer API for any class
 that implements SCSI protocol services. A protocol services layer driver is
 responsible for taking incoming SCSITaskIdentifier objects and translating
 them to the native command type for the native protocol interface
 (e.g. SBP-2 ORB on FireWire).
 */
class __exported IOSCSIProtocolServices : public IOSCSIProtocolInterface
{
	
	OSDeclareAbstractStructors ( IOSCSIProtocolServices )
	
private:
	
	// The head pointer for the queue of waiting SCSI Tasks.
	SCSITask *		fSCSITaskQueueHead;		/* OBSOLETE */
	
	// This is the lock for preventing multiple access while
	// manipulating the SCSI Task queue.
	IOSimpleLock *	fQueueLock;
	
	// The internal flag to indicate whether service requests should be
	// executed or immediately errored, such as when a device is removed.
	bool			fAllowServiceRequests;
	
protected:
	
	// Reserve space for future expansion.
	struct IOSCSIProtocolServicesExpansionData
	{
		// For internal use only. Do not use.
		UInt32				fSemaphore;
		bool				fRequiresAutosenseDescriptor;
		SCSITaskCompletion	fCompletionRoutine;
		queue_head_t		fTaskQueueHead;
		queue_head_t		fAutoSenseQueueHead;
	};
	IOSCSIProtocolServicesExpansionData * fIOSCSIProtocolServicesReserved;
			
	/*!
	@function SendSCSICommand
	@abstract Pure virtual method subclasses must implement in order to send SCSITasks on the wire.
	@discussion Send a SCSI Command to the device.  If the command was sent to the
	device and is pending completion, the subclass should return true and
	return back the kSCSIServiceResponse_Request_In_Process response. 
	If the command completes immediately with an error, the subclass will
	return true and return back the appropriate status.
	If the subclass is currently processing all the commands it can, the
	subclass will return false and the command will be resent next time
	CommandCompleted is called.
	@param request A valid SCSITaskIdentifier representing the command to send on the wire.
	@param serviceResponse Pointer to a SCSIServiceResponse value returned to the caller.
	@param taskStatus Pointer to a SCSITaskStatus value returned to the caller.
	@result False if no more commands can be processed at this time, otherwise true.
	*/
	virtual bool	SendSCSICommand ( 	SCSITaskIdentifier 		request, 
										SCSIServiceResponse *	serviceResponse,
										SCSITaskStatus *		taskStatus ) = 0;
	
	/*!
	@function AbortSCSICommand
	@abstract Pure virtual method subclasses must implement so that SCSITasks may be aborted.
	@discussion Provides the capability for a caller to request that a particular SCSITask
	be aborted.
	@param request A valid SCSITaskIdentifier representing the command to be aborted.
	@result A valid SCSIServiceResponse.
	*/
	virtual SCSIServiceResponse	AbortSCSICommand ( SCSITaskIdentifier request ) = 0;
	
	/*!
	@function CommandCompleted
	@abstract Method subclass calls to complete a SCSITask.
	@discussion Subclasses will call this inherited method when the command
	executed by SendSCSICommand has completed.
	@param request A valid SCSITaskIdentifier indicating the request to complete.
	@param serviceResponse A valid SCSIServiceResponse value.
	@param taskStatus A valid SCSITaskStatus value.
	*/
	void	CommandCompleted ( 	SCSITaskIdentifier 	request, 
								SCSIServiceResponse serviceResponse,
								SCSITaskStatus		taskStatus );
	
	/*!
	@function GetTaskAttribute
	@abstract Accessor method to retrieve the SCSITaskAttribute associated with the specified request.
	@discussion Accessor method to retrieve the SCSITaskAttribute associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result A valid SCSITaskAttribute value.
	*/
	SCSITaskAttribute	GetTaskAttribute ( SCSITaskIdentifier request );
	
	/*!
	@function SetTaskState
	@abstract Accessor method to set the SCSITaskState associated with the specified request.
	@discussion Accessor method to set the SCSITaskState associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@param newTaskState A valid SCSITaskState value.
	@result True on success, otherwise false.
	*/
	bool			SetTaskState ( 	SCSITaskIdentifier request, 
									SCSITaskState newTaskState );
	/*!
	@function GetTaskState
	@abstract Accessor method to retrieve the SCSITaskState associated with the specified request.
	@discussion Accessor method to retrieve the SCSITaskState associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result A valid SCSITaskState value.
	*/
	SCSITaskState	GetTaskState ( 	SCSITaskIdentifier request );
	
	/*!
	@function GetLogicalUnitNumber
	@abstract Accessor method to retrieve the logical unit number associated with the specified request.
	@discussion Accessor method to retrieve the logical unit number associated with the specified request.
	NOTE: This method is deprecated in favor of GetLogicalUnitBytes().
	@param request A valid SCSITaskIdentifier.
	@result A valid single-byte LUN value.
	*/
	UInt8	GetLogicalUnitNumber ( SCSITaskIdentifier request );

	/*!
	@function GetLogicalUnitBytes
	@abstract Accessor method to retrieve the logical unit bytes associated with the specified request.
	@discussion Accessor method to retrieve the logical unit bytes associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@param lunBytes A pointer to SCSILogicalUnitBytes to be filled in by this method.
	*/
	void	GetLogicalUnitBytes ( SCSITaskIdentifier request, SCSILogicalUnitBytes * lunBytes );
	
	/*!
	@function GetCommandDescriptorBlockSize
	@abstract Accessor method to retrieve the Command Descriptor Block size associated with the specified request.
	@discussion Accessor method to retrieve the Command Descriptor Block size associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result A valid CDB size (e.g. 6, 10, 12, or 16 bytes).
	*/
	UInt8	GetCommandDescriptorBlockSize ( SCSITaskIdentifier request );
	
	/*!
	@function GetCommandDescriptorBlock
	@abstract Accessor method to retrieve the Command Descriptor Block associated with the specified request.
	@discussion Accessor method to retrieve the Command Descriptor Block associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@param cdbData A pointer to SCSICommandDescriptorBlock to be filled in by this method.
	NOTE: This routine will always fill in 16 bytes, so if the protocol layer driver supports less than this amount
	it will have to create a local SCSICommandDescriptorBlock variable to get the CDB data.
	@result True on success, otherwise false.
	*/
	bool	GetCommandDescriptorBlock ( SCSITaskIdentifier 				request, 
										SCSICommandDescriptorBlock * 	cdbData );
	
	/*!
	@function GetDataTransferDirection
	@abstract Accessor method to retrieve the data transfer direction associated with the specified request.
	@discussion Accessor method to retrieve the data transfer direction associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result The data transfer direction (e.g. kSCSIDataTransfer_NoDataTransfer).
	*/
	UInt8	GetDataTransferDirection ( SCSITaskIdentifier request );
	
	/*!
	@function GetRequestedDataTransferCount
	@abstract Accessor method to retrieve the requested data transfer count associated with the specified request.
	@discussion Accessor method to retrieve the requested data transfer count associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result The requested data transfer count.
	*/
	UInt64	GetRequestedDataTransferCount ( SCSITaskIdentifier request );
	
	/*!
	@function SetRealizedDataTransferCount
	@abstract Accessor method to set the realized (actual) data transfer count associated with the specified request.
	@discussion Accessor method to set the realized (actual) data transfer count associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@param newRealizedDataCount The realized (actual) data count transferred.
	@result True on success, otherwise false.   
	*/
	bool	SetRealizedDataTransferCount ( SCSITaskIdentifier request,
										   UInt64 newRealizedDataCount );
	
	/*!
	@function GetRealizedDataTransferCount
	@abstract Accessor method to retrieve the realized data transfer count associated with the specified request.
	@discussion Accessor method to retrieve the realized data transfer count associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result The realized data transfer count.
	*/
	UInt64	GetRealizedDataTransferCount ( SCSITaskIdentifier request );
	
	/*!
	@function GetDataBuffer
	@abstract Accessor method to retrieve the data buffer associated with the specified request.
	@discussion Accessor method to retrieve the data buffer associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result An IOMemoryDescriptor associated with the request. May be NULL if data transfer
	direction is kSCSIDataTransfer_NoDataTransfer.
	*/
	IOMemoryDescriptor *	GetDataBuffer ( SCSITaskIdentifier request );
	
	/*!
	@function GetDataBufferOffset
	@abstract Accessor method to retrieve the data buffer offset associated with the specified request.
	@discussion Accessor method to retrieve the data buffer offset associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result Offset into the data buffer at which to start the transfer of data.
	*/
	UInt64	GetDataBufferOffset ( SCSITaskIdentifier request );
	
	/*!
	@function GetTimeoutDuration
	@abstract Accessor method to retrieve the timeout duration in milliseconds associated with the specified request.
	@discussion Accessor method to retrieve the timeout duration in milliseconds associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result Timeout duration in milliseconds.
	*/
	UInt32	GetTimeoutDuration ( SCSITaskIdentifier request );
	
	/*!
	@function GetAutosenseRequestedDataTransferCount
	@abstract Accessor method to retrieve the requested data transfer count for autosense data
	associated with the specified request.
	@discussion Accessor method to retrieve the requested data transfer count for autosense data
	associated with the specified request.
	@param request A valid SCSITaskIdentifier.
	@result The requested autosense data transfer count.
	*/
	UInt64	GetAutosenseRequestedDataTransferCount ( SCSITaskIdentifier	request );
	
	/*!
	@function SetAutoSenseData
	@abstract Accessor method to set the autosense data. NOTE: This method is deprecated.
	@discussion Accessor method to set the autosense data. NOTE: This method is deprecated.
	@param request A valid SCSITaskIdentifier.
	@param senseData A pointer to a SCSI_Sense_Data structure to be copied. Only
	sizeof(struct SCSI_Sense_Data) bytes will be copied.
	@result True if sense data was successfully copied, otherwise false.
	*/
	bool	SetAutoSenseData ( SCSITaskIdentifier	request,
							   SCSI_Sense_Data *	senseData ) __attribute__ ((deprecated));
	
	/*!
	@function SetAutoSenseData
	@abstract Accessor method to set the autosense data.
	@discussion Accessor method to set the autosense data.
	@param request A valid SCSITaskIdentifier.
	@param senseData A pointer to sense data to be copied.
	@param senseDataSize Number of bytes to copy.
	@result True if sense data was successfully copied, otherwise false.
	*/
	bool	SetAutoSenseData ( SCSITaskIdentifier	request,
							   SCSI_Sense_Data *	senseData,
							   UInt8				senseDataSize );
	
	/*!
	@function EnsureAutosenseDescriptorExists
	@abstract Internal method, not to be called by subclasses.
	@discussion Internal method, not to be called by subclasses.
	@param request A valid SCSITaskIdentifier.
	*/
	void	EnsureAutosenseDescriptorExists ( SCSITaskIdentifier request );
	
	/*!
	@function SetProtocolLayerReference
	@abstract Accessor method to set the protocol layer reference.
	@discussion Accessor method to set the protocol layer reference.
	@param request A valid SCSITaskIdentifier.
	@param newReferenceValue Pointer to reference data.
	@result True on success, otherwise false.
	*/
	bool	SetProtocolLayerReference ( 
				SCSITaskIdentifier 		request, 
				void *					newReferenceValue );

	/*!
	@function GetProtocolLayerReference
	@abstract Accessor method to retrieve the protocol layer reference.
	@discussion Accessor method to retrieve the protocol layer reference.
	@param request A valid SCSITaskIdentifier.
	@result The protocol layer reference value. May be NULL.
	*/
	void *	GetProtocolLayerReference ( SCSITaskIdentifier request );
	
	/*!
	@function SetTaskExecutionMode
	@abstract Internal method used to set the task execution mode.
	@discussion Internal method used to set the task execution mode.
	@param request A valid SCSITaskIdentifier.
	@param newTaskMode A valid SCSITaskMode value.
	@result True on success, otherwise false.
	*/
	bool	SetTaskExecutionMode (
				SCSITaskIdentifier 		request, 
				SCSITaskMode 			newTaskMode );

	/*!
	@function GetTaskExecutionMode
	@abstract Internal method used to retrieve the task execution mode.
	@discussion Internal method used to retrieve the task execution mode.
	@param request A valid SCSITaskIdentifier.
	@result A valid SCSITaskMode value.
	*/
	SCSITaskMode	GetTaskExecutionMode ( SCSITaskIdentifier request );
	
	/*!
	@function SendNotification_DeviceRemoved
	@abstract Method called by subclasses when a device is physically removed from the bus.
	@discussion Method called by subclasses when a device is physically removed from the bus.
	*/
	void 	SendNotification_DeviceRemoved ( void );
	
	/*!
	@function SendNotification_VerifyDeviceState
	@abstract Method called by subclasses when a device state needs to be re-verified due to some
	bus condition which may have changed the device state.
	@discussion Method called by subclasses when a device state needs to be re-verified due to some
	bus condition which may have changed the device state.
	*/
	void 	SendNotification_VerifyDeviceState ( void );   
	
	// -- SCSI Task Queue Management Methods --
	// Following are the commands used to manipulate the queue of pending SCSI Tasks.
	
	/*!
	@function AddSCSITaskToQueue
	@abstract Internal method called to add a SCSITask to the processing queue.
	@discussion Internal method called to add a SCSITask to the processing queue.
	@param request A valid SCSITaskIdentifier.
	*/
	void 	AddSCSITaskToQueue ( SCSITaskIdentifier request );
	
	/*!
	@function AddSCSITaskToHeadOfQueue
	@abstract Internal method called to add a SCSITask to the head of the processing queue.
	@discussion Internal method called to add a SCSITask to the head of the processing queue.
	@param request A valid SCSITask pointer.
	*/
	void 	AddSCSITaskToHeadOfQueue ( SCSITask * request );
	
	/*!
	@function RetrieveNextSCSITaskFromQueue
	@abstract Internal method called to retrieve the next SCSITask to process.
	@discussion Internal method called to retrieve the next SCSITask to process.
	@result A valid SCSITask pointer or NULL if there are no tasks to process.
	*/
	SCSITask * RetrieveNextSCSITaskFromQueue ( void );
	
	/*!
	@function AbortSCSITaskFromQueue
	@abstract Deprecated internal method.
	@discussion Deprecated internal method.
	*/
	bool 	AbortSCSITaskFromQueue ( SCSITask * request );
	
	/*!
	@function SendSCSITasksFromQueue
	@abstract Internal method called to start processing SCSITasks.
	@discussion Internal method called to start processing SCSITasks. Only one client or workloop
	thread may process SCSITasks at any point in time. This method coordinates to ensure only one
	thread does so.
	*/
	void	SendSCSITasksFromQueue ( void );
	
	/*!
	@function RejectSCSITasksCurrentlyQueued
	@abstract Internal method called to reject currently enqueued SCSITasks.
	@discussion Internal method called to reject currently enqueued SCSITasks. This method is
	typically called in response to device termination.
	*/
	void	RejectSCSITasksCurrentlyQueued ( void );
	
	/*!
	@function ProcessCompletedTask
	@abstract Internal method called to process completed SCSITasks.
	@discussion Internal method called to process completed SCSITasks. This method determines if
	a CHECK_CONDITION has occurred and if sense data was requested and autosense data is not present.
	If so, it will change the execution mode of the SCSITask and request sense data on behalf of
	the caller.
	@param request A valid SCSITaskIdentifier.
	@param serviceResponse A valid SCSIServiceResponse value.
	@param taskStatus A valid SCSITaskStatus value.
	*/
	void	ProcessCompletedTask ( 	SCSITaskIdentifier 	request, 
									SCSIServiceResponse serviceResponse,
									SCSITaskStatus		taskStatus );
	
	/*!
	@function RejectTask
	@abstract Internal method called to reject a particular SCSITask.
	@discussion Internal method called to reject a particular SCSITask.
	@param request A valid SCSITaskIdentifier.
	*/
	void	RejectTask ( SCSITaskIdentifier	request );
	
	// ------ Power Management Support ------
	
	/*!
	@function InitializePowerManagement
	@abstract Subclasses call this method to initialize power management.
	@discussion Subclasses call this method to initialize power management.
	In the protocol services layer, this method calls the protocol interface layer
	to initialize power management state variables and then registers the protocol
	layer driver with the power manager with two(2) states, ON and OFF. Subclasses may
	override this behavior.
	@param provider The provider to be joined to in the power management tree.
	*/
	virtual void		InitializePowerManagement ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function GetInitialPowerState
	@abstract This method is called once, right after InitializePowerManagement()
	in order to determine what state the device is initially in at startup time (usually
	the highest power mode).
	@discussion This method is called once, right after InitializePowerManagement()
	in order to determine what state the device is initially in at startup time (usually
	the highest power mode).
	@result The power state the device is currently in.
	*/
	virtual UInt32		GetInitialPowerState ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function HandlePowerChange
	@abstract This method is called to handle a power change.
	@discussion This method is called to handle a power change. It is called from a clean thread
	context (i.e. new thread, no locks held) and can make synchronous or asynchronous calls.
	*/
	virtual void		HandlePowerChange ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function HandleCheckPowerState
	@abstract Method called to check if the device is in the correct power state for an I/O.
	@discussion The HandleCheckPowerState (void) method is on the serialized side of the command
	gate and can change member variables safely without multi-threading issues. Its main purpose is
	to call the superclass' HandleCheckPowerState ( UInt32 maxPowerState ) with the max power state
	with which the class registered.
	*/
	virtual void		HandleCheckPowerState ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function TicklePowerManager
	@abstract Internal method. Do not use.
	@discussion Internal method. Do not use.
	*/
	virtual void		TicklePowerManager ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function HandlePowerOff
	@abstract Convenience method for a protocol service driver to handle a power off call (called
	on the way to sleep).
	@discussion Convenience method for a protocol service driver to handle a power off call (called
	on the way to sleep). This method is guaranteed to be called after application layer drivers
	have been put to sleep.
	@result A valid IOReturn code.
	*/
	virtual IOReturn	HandlePowerOff ( void );
	
	/*!
	@function HandlePowerOn
	@abstract Convenience method for a protocol service driver to handle a power on call (called
	on the way back up from sleep).
	@discussion Convenience method for a protocol service driver to handle a power on call (called
	on the way back up from sleep). Driver should perform any bus specific activity necessary to
	recover from power-on/wake from sleep (e.g. bus reset on ATAPI). This method is guaranteed to
	be called before application layer drivers have been awakened.
	@result A valid IOReturn code.
	*/
	virtual IOReturn	HandlePowerOn ( void );
	
	
public:
	

	/*!
	@function init
    @abstract Standard init method for all IORegistryEntry subclasses.
    @discussion A registry entry must be initialized with this method before it can be used. A property dictionary may passed and will be retained by this method for use as the registry entry's property table, or an empty one will be created.
    @param A dictionary that will become the registry entry's property table (retaining it), or zero which will cause an empty property table to be created.
    @result true on success, or false on a resource failure.
    */
	virtual bool	init ( OSDictionary * propTable = 0 ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function start
	@abstract During an IOService object's instantiation, starts the IOService object that has been selected to run on the provider.
	@discussion The <code>start</code> method of an IOService instance is called by its provider when it has been selected (due to its probe score and match category) as the winning client. The client is already attached to the provider when <code>start</code> is called.<br>Implementations of <code>start</code> must call <code>start</code> on their superclass at an appropriate point. If an implementation of <code>start</code> has already called <code>super::start</code> but subsequently determines that it will fail, it must call <code>super::stop</code> to balance the prior call to <code>super::start</code> and prevent reference leaks.
	@result <code>true</code> if the start was successful; <code>false</code> otherwise (which will cause the instance to be detached and usually freed).
	*/
	virtual bool	start ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	
	
	/*!
	@function free
	@abstract Frees data structures that were allocated during start().
	@discussion Frees data structures that were allocated during start().
	*/
	virtual void	free ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function RegisterSCSITaskCompletionRoutine
	@abstract Used by IOSCSITargetDevice to register a completion routine.
	@discussion Used by IOSCSITargetDevice to register a completion routine. Internal use only.
	@param completion A SCSITaskCompletion routine.
	*/
	void	RegisterSCSITaskCompletionRoutine ( SCSITaskCompletion completion );
	
	// ------- SCSI Architecture Model Task Management Functions ------
	
	/*!
	@function ExecuteCommand
	@abstract ExecuteCommand method will take a SCSI Task and transport it across the physical
	wire(s) to the device.
	@discussion ExecuteCommand method will take a SCSI Task and transport it across the physical
	wire(s) to the device.
	@param request A valid SCSITaskIdentifier.
	*/
	void	ExecuteCommand ( SCSITaskIdentifier	request ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function AbortTask
	@abstract The Task Management function to allow the SCSI Application Layer client to request
	that a specific task be aborted.
	@discussion The Task Management function to allow the SCSI Application Layer client to request
	that a specific task be aborted.
	@param theLogicalUnit A logical unit for which to abort a task.
	@param theTag A valid SCSITaggedTaskIdentifier used to identify which task to abort.
	@result A valid SCSIServiceResponse.
	*/
	SCSIServiceResponse		AbortTask ( UInt8 theLogicalUnit, SCSITaggedTaskIdentifier theTag ) APPLE_KEXT_OVERRIDE;

	/*!
	@function AbortTaskSet
	@abstract The Task Management function to allow the SCSI Application Layer client to request
	that a complete task set be aborted.
	@discussion The Task Management function to allow the SCSI Application Layer client to request
	that a complete task set be aborted.
	@param theLogicalUnit A logical unit for which to abort the task set.
	@result A valid SCSIServiceResponse.
	*/
	SCSIServiceResponse		AbortTaskSet ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;

	/*!
	@function ClearACA
	@abstract The Task Management function to clear an Auto-Contingent Allegiance condition.
	@discussion The Task Management function to clear an Auto-Contingent Allegiance condition.
	@param theLogicalUnit A logical unit for which to clear the ACA.
	@result A valid SCSIServiceResponse.
	*/
	SCSIServiceResponse		ClearACA ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;

	/*!
	@function ClearTaskSet
	@abstract The Task Management function to clear a task set.
	@discussion The Task Management function to clear a task set.
	@param theLogicalUnit A logical unit for which to clear a task set.
	@result A valid SCSIServiceResponse.
	*/
	SCSIServiceResponse		ClearTaskSet ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;
    
	/*!
	@function LogicalUnitReset
	@abstract The Task Management function to reset a logical unit.
	@discussion The Task Management function to reset a logical unit.
	@param theLogicalUnit A logical unit for which to clear a task set.
	@result A valid SCSIServiceResponse.
	*/
	SCSIServiceResponse		LogicalUnitReset ( UInt8 theLogicalUnit ) APPLE_KEXT_OVERRIDE;

	/*!
	@function TargetReset
	@abstract The Task Management function to reset a target device.
	@discussion The Task Management function to reset a target device.
	@result A valid SCSIServiceResponse.
	*/
	SCSIServiceResponse		TargetReset ( void ) APPLE_KEXT_OVERRIDE;
	
    // ************* Obsoleted Member Routine ****************
	/*!
	@function AbortCommand
	@abstract Deprecated. Do not use.
	@discussion Deprecated. Do not use.
	*/
	virtual SCSIServiceResponse		AbortCommand ( SCSITaskIdentifier	request ) APPLE_KEXT_OVERRIDE __attribute__ ((deprecated));

	
	// ---- Method used for determining protocol or physical interconnect characteristics. ----
	/*!
	@function IsProtocolServiceSupported
	@abstract IsProtocolServiceSupported will return true if the specified
	feature is supported by the protocol layer.
	@discussion IsProtocolServiceSupported will return true if the specified
	feature is supported by the protocol layer.
	@param feature A valid SCSIProtocolFeature.
	@param serviceValue A pointer to a value for the protocol feature.
	@result True if the requested service is supported, otherwise false.
	*/
	virtual bool	IsProtocolServiceSupported ( SCSIProtocolFeature feature, void * serviceValue ) APPLE_KEXT_OVERRIDE = 0;

	/*!
	@function HandleProtocolServiceFeature
	@abstract HandleProtocolServiceFeature instructs the Protocol Services driver to perform the
	necessary tasks for the indicated feature.
	@discussion HandleProtocolServiceFeature instructs the Protocol Services driver to perform the
	necessary tasks for the indicated feature.
	@param feature A valid SCSIProtocolFeature.
	@param serviceValue A pointer to a value for the protocol feature.
	@result True if successful, otherwise false.
	*/
	virtual bool	HandleProtocolServiceFeature ( SCSIProtocolFeature feature, void * serviceValue ) APPLE_KEXT_OVERRIDE = 0;
	
	
protected:
	
	// ----- Protocol Services Driver request handlers for Task Management functions -----

	/*!
	@function HandleAbortTask
	@abstract HandleAbortTask instructs the Protocol Services driver to abort the task.
	@discussion HandleAbortTask instructs the Protocol Services driver to abort the task.
	@param theLogicalUnit A valid logical unit number.
	@param theTag The tag of the command to abort.
	@result A valid SCSIServiceResponse.
	*/
    OSMetaClassDeclareReservedUsed ( IOSCSIProtocolServices, 1 );
	virtual SCSIServiceResponse		HandleAbortTask ( 
											UInt8 						theLogicalUnit, 
											SCSITaggedTaskIdentifier 	theTag );
    
	/*!
	@function HandleAbortTaskSet
	@abstract HandleAbortTaskSet instructs the Protocol Services driver to abort the task set.
	@discussion HandleAbortTaskSet instructs the Protocol Services driver to abort the task set.
	@param theLogicalUnit A valid logical unit number.
	@result A valid SCSIServiceResponse.
	*/
    OSMetaClassDeclareReservedUsed ( IOSCSIProtocolServices, 2 );
	virtual SCSIServiceResponse		HandleAbortTaskSet ( 
											UInt8 						theLogicalUnit );
	
	/*!
	@function HandleClearACA
	@abstract HandleClearACA instructs the Protocol Services driver to clear an auto-contingent allegiance.
	@discussion HandleClearACA instructs the Protocol Services driver to clear an auto-contingent allegiance.
	@param theLogicalUnit A valid logical unit number.
	@result A valid SCSIServiceResponse.
	*/
    OSMetaClassDeclareReservedUsed ( IOSCSIProtocolServices, 3 );
	virtual SCSIServiceResponse		HandleClearACA ( 
											UInt8 						theLogicalUnit );
	
	/*!
	@function HandleClearTaskSet
	@abstract HandleClearTaskSet instructs the Protocol Services driver to clear the task set.
	@discussion HandleClearTaskSet instructs the Protocol Services driver to clear the task set.
	@param theLogicalUnit A valid logical unit number.
	@result A valid SCSIServiceResponse.
	*/
    OSMetaClassDeclareReservedUsed ( IOSCSIProtocolServices, 4 );
	virtual SCSIServiceResponse		HandleClearTaskSet (
											UInt8 						theLogicalUnit );
	
	/*!
	@function HandleLogicalUnitReset
	@abstract HandleLogicalUnitReset instructs the Protocol Services driver to reset the logical unit.
	@discussion HandleLogicalUnitReset instructs the Protocol Services driver to reset the logical unit.
	@param theLogicalUnit A valid logical unit number.
	@result A valid SCSIServiceResponse.
	*/
    OSMetaClassDeclareReservedUsed ( IOSCSIProtocolServices, 5 );
	virtual SCSIServiceResponse		HandleLogicalUnitReset (
											UInt8 						theLogicalUnit );
											
	/*!
	@function HandleTargetReset
	@abstract HandleTargetReset instructs the Protocol Services driver to reset the target.
	@discussion HandleTargetReset instructs the Protocol Services driver to reset the target.
	@result A valid SCSIServiceResponse.
	*/
    OSMetaClassDeclareReservedUsed ( IOSCSIProtocolServices, 6 );
	virtual SCSIServiceResponse		HandleTargetReset ( void );

	/*!
	@function CreateSCSITargetDevice
	@abstract Used to create a SCSITargetDevice which will manage logical units.
	@discussion The CreateSCSITargetDevice member routine will create the appropriate object
	to represent the Target portion of a SCSI Device. This object is responsible
	for managing the Target functions of the SCSI Device including the Task Manager and
	Logical Units.
	@result True if successful, otherwise false.
	*/
    OSMetaClassDeclareReservedUsed ( IOSCSIProtocolServices,  7 );
	virtual bool	CreateSCSITargetDevice ( void );
	
private:
	
	
#if !TARGET_OS_IPHONE
	// Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices,  8 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 	9 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 10 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 11 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 12 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 13 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 14 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 15 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolServices, 16 );
#endif /* !TARGET_OS_IPHONE */
    
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif	/* _IOKIT_IO_SCSI_PROTOCOL_SERVICES_H_ */
