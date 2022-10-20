/*
 * Copyright (c) 1998-2014 Apple Inc. All rights reserved.
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


#ifndef _IOKIT_IO_SCSI_REDUCED_BLOCK_COMMANDS_DEVICE_H_
#define _IOKIT_IO_SCSI_REDUCED_BLOCK_COMMANDS_DEVICE_H_

#if defined(KERNEL) && defined(__cplusplus)


/*! @header SCSI Reduced Block Commands Device
 @discussion
 This file contains definitions for the IOSCSIReducedBlockCommandsDevice class,
 commands required for RBC devices, power management, and additional constants
 used by this class.
 */


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// General IOKit headers
#include <IOKit/IOLib.h>
#include <IOKit/IOMemoryDescriptor.h>

// Generic IOKit storage related headers
#include <IOKit/storage/IOStorage.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi/IOSCSIPrimaryCommandsDevice.h>

// Build includes
#include <TargetConditionals.h>

//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------

// RBC power states as defined in T10:1240D SCSI Reduced Block Commands (RBC)
// Revision 10a, August 18, 1999, page 13.
enum
{
	kRBCPowerStateSystemSleep	= 0,
	kRBCPowerStateSleep 		= 1,
	kRBCPowerStateStandby 		= 2,
	kRBCPowerStateIdle			= 3,
	kRBCPowerStateActive		= 4,
	kRBCNumPowerStates			= 5
};

/*!
@enum Media lock state values
@discussion
Values used to set media lock state.
@constant kMediaStateUnlocked
Value to be passed in order to unlock media and make it availabe for writing.
@constant kMediaStateLocked
Value to be passed when the media is to be locked and made read-only. 
*/

enum
{
	kMediaStateUnlocked	= 0,
	kMediaStateLocked 	= 1
};

#define kCapacityDataBufferSize				8

// Forward declaration for the SCSIReducedBlockCommands that is used internally by the
// IOSCSIReducedBlockCommandsDevice class.
class SCSIReducedBlockCommands;

//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------

/*!
@class IOSCSIReducedBlockCommandsDevice
@superclass IOSCSIPrimaryCommandsDevice
@discussion
Implementation of the T10 device Reduced Block Commands device. This is the base
driver for all subclasses implementing peripheral device type 0xE.
*/
class __exported IOSCSIReducedBlockCommandsDevice : public IOSCSIPrimaryCommandsDevice
{
	
	OSDeclareAbstractStructors ( IOSCSIReducedBlockCommandsDevice );
	
private:
	
	static void			AsyncReadWriteComplete ( SCSITaskIdentifier completedTask );
	
protected:

	// Reserve space for future expansion.
	struct IOSCSIReducedBlockCommandsDeviceExpansionData
	{
		IONotifier *	fPowerDownNotifier;
		bool			fMediumRemovalPrevented;
		bool			fKnownManualEject;
		UInt32			fPollingMode;
		bool			fProtocolSpecificPowerControl;
	};
	IOSCSIReducedBlockCommandsDeviceExpansionData * fIOSCSIReducedBlockCommandsDeviceReserved;
	
	#define fPowerDownNotifier				fIOSCSIReducedBlockCommandsDeviceReserved->fPowerDownNotifier
	#define fMediumRemovalPrevented			fIOSCSIReducedBlockCommandsDeviceReserved->fMediumRemovalPrevented
	#define fKnownManualEject				fIOSCSIReducedBlockCommandsDeviceReserved->fKnownManualEject
	#define fPollingMode					fIOSCSIReducedBlockCommandsDeviceReserved->fPollingMode
	#define fProtocolSpecificPowerControl	fIOSCSIReducedBlockCommandsDeviceReserved->fProtocolSpecificPowerControl
	
	bool				fMediaChanged;
	bool				fMediaPresent;
	
	// The byte count of each physical block on the media.
	UInt32				fMediaBlockSize;
	
	// The total number of blocks of mediaBlockSize on the media.
	UInt32				fMediaBlockCount;
	
	// Flags used to indicate device features.
	bool				fMediaIsRemovable;
	bool				fMediaIsWriteProtected;
	
	thread_call_t		fPollingThread;
	
	enum
	{
		kPollingMode_Suspended			= 0,
		kPollingMode_NewMedia 			= 1,
		kPollingMode_MediaRemoval		= 2
	};
	
	/*!
	@function CreateStorageServiceNub
	@abstract Creates a nub which links the IOSCSIReducedBlockCommandsDevice to the IOStorageFamily.
	@discussion Creates a nub which links the IOSCSIReducedBlockCommandsDevice to the IOStorageFamily.
	*/
	virtual void 		CreateStorageServiceNub ( void );
	
	/*!
	@function DetermineDeviceCharacteristics
	@abstract Determine the characteristics of the device.
	@discussion Determine the characteristics, ANSI version, removability, etc., of the device via its INQUIRY data.
	@result <code>true</code> on success and <code>false</code> on failure.
	 */
	virtual bool		DetermineDeviceCharacteristics ( void );
	
	/*!
	@function PollForMedia
	@abstract Poll device for media insertion or removal depending on selected polling mode.
	@discussion Polls device for media insertion or removal depending on selected polling mode. 
	If a change in media state is detected the correct media inserted or removed API is called from within PollForMedia.
	*/
	virtual void		PollForMedia ( void );
	
	/*!
	@function EnablePolling
	@abstract Enables media polling.
	@discussion Schedules a thread to poll the device for media at the prescribed interval.
	*/
	virtual void		EnablePolling ( void );
	
	/*!
	@function DisablePolling
	@abstract Disables media polling.
	@discussion Cancels ( if running ) the media polling thread.
	*/
	virtual void		DisablePolling ( void );
	
	/*!
	@function CheckWriteProtection
	@abstract Ascertains whether or not media is locked.
	@discussion Determines the current write protection state of the media.
	Result stored in fMediaIsWriteProtected.
	*/
	virtual void		CheckWriteProtection ( void );
	
	/*!
	@function SetMediaCharacteristics
	@abstract Sets the block size and block count for the current media.
	@discussion Sets the block size and block count for the current media.
	@param blockSize The media's block size in bytes.
	@param blockCount The count of the media's blocks.
	*/
	virtual void		SetMediaCharacteristics ( UInt32 blockSize, UInt32 blockCount );
	
	/*!
	@function ResetMediaCharacteristics
	@abstract Resets the block size and block count.
	@discussion Resets the block size and block count.
	*/    
	virtual void		ResetMediaCharacteristics ( void );
	
	/*!
	@function ClearNotReadyStatus
	@abstract Clears any NOT_READY status for the device.
	@discussion Polls device with TEST_UNIT_READY until the device returns <code>kSCSITaskStatus_GOOD</code> status.
	Exits if device has become disconnected.
	@result <code>true</code> if device is now active (isInactive == false) and <code>false</code> otherwise.
	*/
	virtual bool		ClearNotReadyStatus ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function IssueRead
	@abstract DEPRECATED. Please do not use this method.
	@discussion DEPRECATED. Please do not use this method.
	@param buffer DEPRECATED. Please do not use this method.
	@param startBlock DEPRECATED. Please do not use this method.
	@param blockCount DEPRECATED. Please do not use this method.
	@result DEPRECATED. Please do not use this method.
	*/
	virtual IOReturn	IssueRead ( IOMemoryDescriptor *	buffer,
									UInt64					startBlock,
									UInt64					blockCount );
	
	/*!
	@function IssueWrite
	@abstract DEPRECATED. Please do not use this method.
	@discussion DEPRECATED. Please do not use this method.
	@param buffer DEPRECATED. Please do not use this method.
	@param startBlock DEPRECATED. Please do not use this method.
	@param blockCount DEPRECATED. Please do not use this method.
	@result DEPRECATED. Please do not use this method.
	*/
	virtual IOReturn	IssueWrite ( IOMemoryDescriptor*	buffer,
									 UInt64					startBlock,
									 UInt64					blockCount );
	
	/*!
	@function IssueRead
	@abstract Issues an asynchronous read command.
	@discussion Issues an asynchronous read command.
	@param buffer A pointer to a valid IOMemoryDescriptor to which the data can be copied to.
	@param startBlock Starting logical block address for the read operation.
	@param blockCount The number of blocks to be read.
	@param clientData A valid pointer to the client data to be used for callback completion. 
	@result A valid IOReturn value.
	*/
	virtual IOReturn	IssueRead ( IOMemoryDescriptor *	buffer,
									UInt64					startBlock,
									UInt64					blockCount,
									void *					clientData );
	
	/*!
	@function IssueWrite
	@abstract Issues an asynchronous write command.
	@discussion Issues an asynchronous write command.
	@param buffer A pointer to a valid IOMemoryDescriptor to which the data can be copied from.
	@param startBlock Starting logical block address for the write operation.
	@param blockCount The number of blocks to be written.
	@param clientData A valid pointer to the client data to be used for callback completion. 
	@result A valid IOReturn value.
	*/
	virtual IOReturn	IssueWrite ( IOMemoryDescriptor *	buffer,
									 UInt64					startBlock,
									 UInt64					blockCount,
									 void *					clientData );

	// ----- Power Management Support ------
		
	/*!
	@function InitializePowerManagement
	@abstract Called to initialize power management.
	@discussion Called by InitializeDeviceSupport() to initialize power management by, 
	joining the power management tree, registering power states, and setting the lowest idle 
	power state.
	@param A valid pointer to this object's provider.
	*/
	
	// We override this method to set our power states and register ourselves
	// as a power policy maker.
	virtual void 		InitializePowerManagement ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function GetInitialPowerState
	@abstract Called by power management to determine initial power state.
	@discussion Called by power management to determine which power state is desired when 
	power management is initialized.
	@result Returns the initial power state desired when power management is initialized.
	*/
	
	// We override this method so that when we register for power management,
	// we go to our active power state (which the drive is definitely in
	// at startup time).
	virtual UInt32		GetInitialPowerState ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function GetNumberOfPowerStateTransitions
	@abstract Called by power management to determine this class's total 
	number of power state transitions.
	@discussion Called by power management to determine this class's total 
	number of power state transitions.
	@result Returns the number of power state transitions for this class.
	*/
	
	// We override this method in order to provide the number of transitions
	// from Fully active to Sleep state so that the idle timer can be adjusted
	// to the appropriate time period based on the disk spin down time set in
	// the Energy Saver prefs panel.
	virtual UInt32		GetNumberOfPowerStateTransitions ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function TicklePowerManager
	@abstract The TicklePowerManager() is called by CheckPowerState() and
	sends an activity tickle ( via activityTickle() ) to the power manager 
	so that the idle timer is reset.
	@discussion The TicklePowerManager() is called by CheckPowerState() and
	sends an activity tickle ( via activityTickle() ) to the power manager
	so that the idle timer is reset.
	Subclasses must override this method. Third party subclasses shouldn't need to override
	this method but can to alter the default behavior.
	*/

	// The TicklePowerManager method is called to tell the power manager that the
	// device needs to be in a certain power state to handle requests.
	virtual void		TicklePowerManager ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function HandlePowerChange
	@abstract Handles the state machine for power management state changes.
	@discussion Handles the state machine for power management state changes.
	*/
	
	// The HandlePowerChange method is the state machine for power management.
	// It is guaranteed to be on its own thread of execution (different from
	// the power manager thread AND the workloop thread. This routine can
	// send sync or async calls to the drive without worrying about threading
	// issues.
	virtual void		HandlePowerChange ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function HandleCheckPowerState(void)
	@abstract The HandleCheckPowerState (void) method is on the serialized side of the command
	gate and can change member variables safely without multi-threading issues.
	It's main purpose is to call the superclass' HandleCheckPowerState ( UInt32 maxPowerState )
	with the max power state the class registered with.
	@discussion The HandleCheckPowerState (void) method is on the serialized side of the command
	gate and can change member variables safely without multi-threading issues.
	It's main purpose is to call the superclass' HandleCheckPowerState ( UInt32 maxPowerState )
	with the max power state the class registered with.
	Subclasses must override this method. Third party subclasses shouldn't need to override
	this method but can to alter the default behavior. The default behavior is to block until the 
	object transistions to its maximum power state.
	*/
	
	virtual void		HandleCheckPowerState ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function CheckMediaPresence
	@abstract Called to ascertain the current presence of media.
	@discussion Called to ascertain the current presence of media.
	@result <code>true</code> if media is present and <code>false</code> if media is not present.
	*/
	
	// The CheckMediaPresence method is called to see if the media which we
	// anticipated being there is still there.
	virtual bool		CheckMediaPresence ( void );
	
	/*!
	@function InitializeDeviceSupport
	@abstract Called in super class's start() to initialize device support.
	@discussion Called in super class's start() to initialize device support. The underlying infrastructure
	for supporting a device must be implemented here.
	@result <code>true</code> if device could be successfully configured <code>false</code> if not.
	*/    
	virtual bool		InitializeDeviceSupport ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function StartDeviceSupport
	@abstract Called in super class's start() to start device support.
	@discussion Called in super class's start() to start device support. Support for starting
	devices and making them usable must be implemented here. 
	*/  
	virtual void		StartDeviceSupport ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function SuspendDeviceSupport
	@abstract Called to temporarily suspend device support.
	@discussion Called to temporarily suspend device support. Support for suspending
	devices must be implemented here.
	*/ 
	virtual void		SuspendDeviceSupport ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function ResumeDeviceSupport
	@abstract Called to temporarily resume device support.
	@discussion Called to temporarily resume device support. Support for resuming
	devices must be implemented here.
	*/ 
	virtual void		ResumeDeviceSupport ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function StopDeviceSupport
	@abstract Called to stop device support.
	@discussion Called to stop device support. Support for stopping
	devices must be implemented here.
	*/ 
	virtual void		StopDeviceSupport ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function TerminateDeviceSupport
	@abstract Called to terminate device support.
	@discussion Called to terminate device support. The underlying infrastructure for
	support a device should be cleaned up here.
	*/ 
	virtual void		TerminateDeviceSupport ( void ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function free
	@abstract Called to release all resources held by the object.
	@discussion Release all resources held by the object, then call super::free().  
	*/	
	virtual void 		free ( void ) APPLE_KEXT_OVERRIDE;
	
public:
	
	/*!
	@function SyncReadWrite
	@abstract Translates a synchronous I/O request into a read or a write.
	@discussion Translates a synchronous I/O request into a read or a write.
	Default implementation always returns kIOReturnUnsupported.
	@param buffer A pointer to a valid IOMemoryDescriptor to which the 
	data can be copied to or from.
	@param startBlock Starting logical block address for the write operation.
	@param blockCount The number of block to be write.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	SyncReadWrite ( 	IOMemoryDescriptor *	buffer,
											UInt64					startBlock,
											UInt64					blockCount );
	
	/*!
	@function AsyncReadWrite
	@abstract Translates an asynchronous I/O request into a read or a write.
	@discussion Translates an asynchronous I/O request into a read or a write.
	@param buffer A pointer to a valid IOMemoryDescriptor to which the 
	data can be copied to or from.
	@param startBlock Starting logical block address for the write operation.
	@param blockCount The number of block to be write.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	AsyncReadWrite (	IOMemoryDescriptor *	buffer,
											UInt64					block,
											UInt64					nblks,
											void *					clientData );
	
	/*!
	@function EjectTheMedia
	@abstract Called to eject the inserted media from the drive.
	@discussion Called by IOBlockStorageDevice subclasses to eject the inserted 
	media from the drive.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	EjectTheMedia ( void );	
	
	/*!
	@function FormatMedia
	@abstract Called to perform a low level format of the inserted media.
	@discussion Called IOBlockStorageDevice subclasses to perform a low level 
	of the inserted media. Default implementation simply returns kIOReturnUnsupported 
	if not overridden.
	@param byteCapacity
	@result A valid IOReturn value.
	*/
	virtual IOReturn	FormatMedia ( UInt64 byteCapacity );
	
	/*!
	@function GetFormatCapacities
	@abstract Called to determine the formatted capacity of the inserted media.
	@discussion Called IOBlockStorageDevice subclasses to determine the formatted 
	capacity of the inserted media. Presently unsupported by default implementation. 
	Returns 0 unless overridden.
	@param capacities a pointer to a valid UInt64 array.
	@param capacitiesMaxCount the number of elements addressable within capacities.
	@result The number of elements assigned in capacities ( count is 1 based ).
	*/
	virtual UInt32		GetFormatCapacities ( 	UInt64 * capacities,
												UInt32   capacitiesMaxCount ) const;
	
	/*!
	@function LockUnlockMedia
	@abstract Called to lock or unlock the inserted media.
	@discussion Called by IOBlockStorageDevice subclasses to lock or unlock the inserted media. 
	@param doLock if true the media will be locked, if false the media will be unlocked.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	LockUnlockMedia ( bool doLock );
	
	/*!
	@function SynchronizeCache
	@abstract Called to synchronize the devices cache and thereby force its contents to be
	written out to the media.
	@discussion Called by IOBlockStorageDevice subclasses to synchronize the devices cache 
	and thereby force the cache's contents to be written out to the media.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	SynchronizeCache ( void );
	
	/*!
	@function ReportBlockSize
	@abstract Reports the logical block size of the media.
	@discussion Called by IOBlockStorageDevice subclasses to get the logical block size of the media.
	@param blockSize a valid pointer to a UInt64 in which the logical block size is to be stored.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	ReportBlockSize ( UInt64 * blockSize );
	
	/*!
	@function ReportEjectability
	@abstract Reports whether or not the media is ejectable.
	@discussion Called by IOBlockStorageDevice subclasses to determine whether or not the media is ejectable.
	@param isEjectable a valid pointer to a bool in which the ejectability of the media is to be stored.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	ReportEjectability ( bool * isEjectable );
	
	/*!
	@function ReportLockability
	@abstract Reports whether or not the media is lockable ( can be write protected ).
	@discussion Called by IOBlockStorageDevice subclasses to determine whether or not the media is lockable
	( can be write protected ).
	@param isLockable a valid pointer to a bool in which the lockability of the media is to be stored.
	@result A valid IOReturn value.
	*/  
	virtual IOReturn	ReportLockability ( bool * isLockable );
	
	/*!
	@function ReportPollRequirements
	@abstract Reports polling requirements to caller.
	@discussion Called by IOBlockStorageDevice subclasses to determine polling requirements. 
	Used to determine if polling for media is required and if polling is expensive. 
	@param pollIsRequired a pointer to a bool in which to store whether or not polling is required.
	@param pollIsExpensive a pointer to a bool in which to store whether or not polling is expensive.
	@result A valid IOReturn value.
	*/ 
	virtual IOReturn	ReportPollRequirements ( bool * pollIsRequired,
												 bool * pollIsExpensive );
	
	/*!
	@function ReportMaxReadTransfer
	@abstract Reports maximum read transfer size to caller.
	@discussion Called by IOBlockStorageDevice subclasses to determine the maximum size
	of a read transfer in bytes. 
	@param blockSize used to pass in the block size to be used for a follow up transfer.
	@param max a pointer to a UInt64 in which to store the maximum transfer size in bytes.
	@result A valid IOReturn value.
	*/ 
	virtual IOReturn	ReportMaxReadTransfer (  UInt64 	blockSize,
												 UInt64 * 	max );
	
	/*!
	@function ReportMaxValidBlock
	@abstract Reports the maximum valid block on the media.
	@discussion Called by IOBlockStorageDevice subclasses to determine the maximum valid 
	block of the media.
	@param maxBlock a pointer to a UInt64 in which the maximum valid block is to be stored.
	@result A valid IOReturn value.
	*/ 
	virtual IOReturn	ReportMaxValidBlock ( UInt64 * maxBlock );
	
	/*!
	@function ReportMaxWriteTransfer
	@abstract Reports maximum write transfer size to caller.
	@discussion Called by IOBlockStorageDevice subclasses to determine the maximum size
	of a write transfer in bytes.
	@param blockSize used to pass in the block size of the media.
	@param max a pointer to a UInt64 in which to store the maximum transfer size in bytes.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	ReportMaxWriteTransfer ( UInt64	blockSize,
												UInt64 * max );
	
	/*!
	@function ReportMediaState
	@abstract Reports the state of media in the device.
	@discussion Called by IOBlockStorageDevice subclasses to determine whether or not media is 
	present and if that media has changed.
	@param mediaPresent a pointer to a bool in which media presence will be stored.
	@param changed a pointer to a bool in which whether or not the media has changed will be stored.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	ReportMediaState ( 	bool * mediaPresent,
											bool * changed );
	
	/*!
	@function ReportRemovability
	@abstract Reports whether or not the media may be removed.
	@discussion Called by IOBlockStorageDevice subclasses to determine whether or not the media may be removed.
	@param isRemovable a pointer to a bool in which whether or not the media can be me removed 
	will be stored.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	ReportRemovability ( bool * isRemovable );
	
	/*!
	@function ReportWriteProtection
	@abstract Reports whether or not the media is write protected.
	@discussion Called by IOBlockStorageDevice subclasses to determine whether or not the media is write protected.
	@param isWriteProtected a pointer to a bool in which whether or not the media is write protected will be stored.
	@result A valid IOReturn value.
	*/
	virtual IOReturn	ReportWriteProtection ( bool * isWriteProtected );
	
	/*!
	@function sPollForMedia
	@abstract Static method called to poll for media.
	@discussion Static method called to poll for media.
	@param pdtDriver a pointer to a valid IOSCSIReducedBlockCommandsDevice ( or subclass ) which is to
	be polled.
	@param refCon a pointer to an additional object which can be determined by the implementer.
	*/
	static 	void		sPollForMedia ( void * pdtDriver, void * refCon );
	
	
protected:
	
	
	/*!
	@function FORMAT_UNIT
	@abstract Builds a FORMAT UNIT command.
	@discussion Builds a FORMAT UNIT command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param IMMED used to specify whether or not the immediate bit for the command needs to be set.
	@param PROGRESS used to specify whether or not the progress bit for the commands needs to be set. 
	@param PERCENT_TIME used to specify whether or not the percent time bit for the command needs to be set.
	@param INCREMENT used to specify whether or not the increment bit for the command needs to be set.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The FORMAT_UNIT command as defined in section 5.1
	virtual bool FORMAT_UNIT (
							SCSITaskIdentifier			request,
							SCSICmdField1Bit			IMMED,
							SCSICmdField1Bit			PROGRESS,
							SCSICmdField1Bit			PERCENT_TIME,
							SCSICmdField1Bit			INCREMENT );
	
	/*!
	@function INQUIRY
	@abstract Builds a INQUIRY command.
	@discussion Builds a INQUIRY command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the received INQUIRY data is to be stored.
	@param CMDDT used to specify whether or not the command support data ( CmdDT ) bit for the commands needs to be set. 
	@param EVPD used to specify whether or not the vital product data ( EVPD ) bit for the command needs to be set.
	@param PAGE_OR_OPERATION_CODE used to specify which INQUIRY data page is being requested of the device.
	@param ALLOCATION_LENGTH used to specify the maximum requested amount of INQUIRY data.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The INQUIRY command as defined in SPC-2 w/o CONTROL byte
	virtual bool INQUIRY (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit			CMDDT,
							SCSICmdField1Bit			EVPD,
							SCSICmdField1Byte			PAGE_OR_OPERATION_CODE,
							SCSICmdField1Byte			ALLOCATION_LENGTH );
	
	/*!
	@function MODE_SELECT_6
	@abstract Builds a MODE SELECT(6) command.
	@discussion Builds a MODE SELECT(6) command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the MODE SELECT data is stored.
	@param PF used to specify whether or not the page format ( PF ) bit for the commands needs to be set. 
	@param SP used to specify whether or not the saves pages ( SP ) bit for the command needs to be set.
	@param PARAMETER_LIST_LENGTH used to specify the length of the supplied data.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The MODE_SELECT(6) command as defined in SPC-2 w/o CONTROL byte
	virtual bool MODE_SELECT_6 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			PF,
							SCSICmdField1Bit 			SP,
							SCSICmdField1Byte 			PARAMETER_LIST_LENGTH );
	
	/*!
	@function MODE_SENSE_6
	@abstract Builds a MODE SENSE(6) command.
	@discussion Builds a MODE SENSE(6) command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the MODE SENSE data is to be stored.
	@param DBD used to specify whether or not the disable block descriptors ( DBD ) bit for the commands needs to be set. 
	@param PC used to specify the desired page control ( PC ) value.
	@param PAGE_CODE used to specify the desired page code.
	@param ALLOCATION_LENGTH used to specify the maximum requested amount of MODE_SENSE data.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The MODE_SENSE(6) command as defined in SPC-2 w/o CONTROL byte
	virtual bool MODE_SENSE_6 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Bit 			DBD,
							SCSICmdField2Bit 			PC,
							SCSICmdField6Bit 			PAGE_CODE,
							SCSICmdField1Byte 			ALLOCATION_LENGTH );
	
	/*!
	@function PERSISTENT_RESERVE_IN
	@abstract Builds a PERSISTENT RESERVE IN command.
	@discussion Builds a PERSISTENT RESERVE IN command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the reservation data is to be stored.
	@param SERVICE_ACTION used to specify the SERVICE ACTION code. 
	@param ALLOCATION_LENGTH used to specify the number of parameter data bytes available to be returned.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The PERSISTENT_RESERVE_IN command as defined in SPC-2 w/o CONTROL byte
	virtual bool PERSISTENT_RESERVE_IN (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField5Bit 			SERVICE_ACTION,
							SCSICmdField2Byte 			ALLOCATION_LENGTH );
	
	/*!
	@function PERSISTENT_RESERVE_OUT
	@abstract Builds a PERSISTENT RESERVE OUT command.
	@discussion Builds a PERSISTENT RESERVE OUT command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the reservation data is stored.
	@param SERVICE_ACTION used to specify the SERVICE ACTION code. 
	@param SCOPE used to specify the desired scope.
	@param TYPE used to specify the desired type.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The PERSISTENT_RESERVE_OUT command as defined in SPC-2 w/o CONTROL byte
	virtual bool PERSISTENT_RESERVE_OUT (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField5Bit			SERVICE_ACTION,
							SCSICmdField4Bit			SCOPE,
							SCSICmdField4Bit			TYPE );

	/*!
	@function PREVENT_ALLOW_MEDIUM_REMOVAL
	@abstract Builds a PREVENT ALLOW MEDIUM REMOVAL command.
	@discussion Builds a PREVENT ALLOW MEDIUM REMOVAL command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param PREVENT used to specify whether or not media removal shall be prevented or allowed. The caller
	uses kMediaStateUnlocked or kMediaStateLocked constant to set the state.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The PREVENT_ALLOW_MEDIUM_REMOVAL command as defined in SPC-2 w/o CONTROL byte
	virtual bool PREVENT_ALLOW_MEDIUM_REMOVAL ( 
							SCSITaskIdentifier			request,
							SCSICmdField2Bit			PREVENT );
	
	/*!
	@function READ_10
	@abstract Builds a READ(10) command.
	@discussion Builds a READ(10) command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the read data is to be stored.
	@param blockSize used to specify the block size to be used for the read.
	@param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the read is to be initiated.
	@param TRANSFER_LENGTH the desired transfer length in blocks.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The READ_10 command as defined in section 5.2
	virtual bool READ_10 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							UInt32						blockSize,
							SCSICmdField4Byte			LOGICAL_BLOCK_ADDRESS,
							SCSICmdField2Byte			TRANSFER_LENGTH );
	
	/*!
	@function READ_CAPACITY
	@abstract Builds a READ CAPACITY command.
	@discussion Builds a READ CAPACITY command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the read capacity data is to be stored.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The READ_CAPACITY command as defined in section 5.3
	virtual bool READ_CAPACITY (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer );
	
	/*!
	@function RELEASE_6
	@abstract Builds a RELEASE(6) command.
	@discussion Builds a RELEASE(6) command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The RELEASE(6) command as defined in SPC-2 w/o CONTROL byte
	virtual bool RELEASE_6 (								
							SCSITaskIdentifier			request );
	
	/*!
	@function REQUEST_SENSE
	@abstract Builds a REQUEST SENSE command.
	@discussion Builds a REQUEST SENSE command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the sense data is to be stored.
	@param ALLOCATION_LENGTH the maximum size of sense data to be supplied.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The REQUEST_SENSE command as defined in SPC-2 w/o CONTROL byte
	virtual bool REQUEST_SENSE (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField1Byte 			ALLOCATION_LENGTH );
	
	/*!
	@function RESERVE_6
	@abstract Builds a RESERVE(6) command.
	@discussion Builds a RESERVE(6) command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The RESERVE(6) command as defined in SPC-2 w/o CONTROL byte
	virtual bool RESERVE_6 (	
							SCSITaskIdentifier			request );
	
	/*!
	@function START_STOP_UNIT
	@abstract Builds a START STOP UNIT command.
	@discussion Builds a START STOP UNIT command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param IMMED used to specify whether or not the immediate bit for the command needs to be set.
	@param POWER_CONDITIONS used to specify the desired POWER CONDITIONS code.
	@param LEOJ used to specify whether or not the LOAD/EJECT ( LEOJ ) bit for the command needs to be set.
	@param START used to specify whether or not the START bit for the command needs to be set.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The START_STOP_UNIT command as defined in section 5.4
	virtual bool START_STOP_UNIT (
							SCSITaskIdentifier			request,
							SCSICmdField1Bit			IMMED,
							SCSICmdField4Bit			POWER_CONDITIONS,
							SCSICmdField1Bit			LEOJ,
							SCSICmdField1Bit			START );
	
	/*!
	@function SYNCHRONIZE_CACHE
	@abstract Builds a SYNCHRONIZE CACHE command.
	@discussion Builds a SYNCHRONIZE CACHE command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The SYNCRONIZE_CACHE command as defined in section 5.5
	virtual bool SYNCHRONIZE_CACHE (
							SCSITaskIdentifier			request );
	
	/*!
	@function TEST_UNIT_READY
	@abstract Builds a TEST UNIT READY command.
	@discussion Builds a TEST UNIT READY command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The TEST_UNIT_READY command as defined in SPC-2 w/o CONTROL byte
	virtual bool TEST_UNIT_READY (
							SCSITaskIdentifier			request );
	
	/*!
	@function VERIFY
	@abstract Builds a VERIFY command.
	@discussion Builds a VERIFY command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the verify should be initiated.
	@param VERIFICATION_LENGTH the desired length in blocks.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The VERIFY command as defined in section 5.7
	virtual bool VERIFY (
							SCSITaskIdentifier			request,
							SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
							SCSICmdField2Byte 			VERIFICATION_LENGTH );
	
	/*!
	@function WRITE_10
	@abstract Builds a WRITE(10) command.
	@discussion Builds a WRITE(10) command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the data to be written is stored.
	@param blockSize used to specify the block size to be used for the write.
	@param FUA used specify whether or not the force unit access ( FUA ) bit should be set.
	@param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the write should be initiated.
	@param TRANSFER_LENGTH the desired transfer length in blocks.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The WRITE_10 command	as defined in section 5.6
	virtual bool WRITE_10 (
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							UInt32						blockSize,
							SCSICmdField1Bit			FUA,
							SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
							SCSICmdField2Byte 			TRANSFER_LENGTH );
	
	/*!
	@function WRITE_BUFFER
	@abstract Builds a WRITE BUFFER command.
	@discussion Builds a WRITE BUFFER command.
	@param request a SCSITaskIdentifier in which the built command will be stored.
	@param dataBuffer a point to a valid IOMemoryDescriptor in which the data to be written is stored.
	@param MODE used to specify which WRITE BUFFER mode is to be used.
	@param BUFFER_ID used to be specificy the desired buffer ID.
	@param BUFFER_OFFSET used to specify the desired buffer offset.
	@param PARAMETER_LIST_LENGTH the length of the buffer to be transferred.
	@result <code>true</code> if the command was successfully built and <code>false</code> if not.
	*/
	// The WRITE_BUFFER command as defined in SPC-2 w/o CONTROL byte
	virtual bool WRITE_BUFFER ( 
							SCSITaskIdentifier			request,
							IOMemoryDescriptor *		dataBuffer,
							SCSICmdField4Bit 			MODE,
							SCSICmdField1Byte 			BUFFER_ID,
							SCSICmdField3Byte 			BUFFER_OFFSET,
							SCSICmdField3Byte 			PARAMETER_LIST_LENGTH );
	
	

	/* Added with 10.2 */
	OSMetaClassDeclareReservedUsed ( IOSCSIReducedBlockCommandsDevice, 1 );
	
public:
	
	/*!
	@function PowerDownHandler
	@abstract Method called at sleep, restart, or shutdown time.
	@discussion Notification handler registered with registerPrioritySleepWakeInterest().
	@param refCon a pointer to an object which can be determined by the implementer when
	registerPrioritySleepWakeInterest() is called.
	@param messageType a constant defined in IOMessage.h ( kIOMessageSystemWillPowerOff, kIOMessageSystemWillRestart, etc. ).
	@param provider a pointer to the provider of the notification.
	@param messageArgument a pointer to the notification's argument.
	@param argSize the size of the argument.
	@result A valid IOReturn value.
    */
	virtual IOReturn	PowerDownHandler (	void * 			refCon,
											UInt32 			messageType,
											IOService * 	provider,
											void * 			messageArgument,
											vm_size_t 		argSize );
	
	
	/* Added with 10.2 */	
	OSMetaClassDeclareReservedUsed ( IOSCSIReducedBlockCommandsDevice, 2 );
	
protected:

	/*!
	@function SetMediaIcon
	@abstract Sets graphical icon for device's media objects.
	@discussion Provides an opportunity for the driver to select a more fitting icon then default icon. 
	*/
	virtual	void		SetMediaIcon ( void );
	

	/* Added with 10.3.3 */		
	OSMetaClassDeclareReservedUsed ( IOSCSIReducedBlockCommandsDevice, 3 );
	
protected:

	/*!
	@function AsyncReadWriteCompletion
	@abstract Completion routine for asynchronous read and write requests.
	@discussion Completion routine for asynchronous read and write requests.
	*/
	virtual	void AsyncReadWriteCompletion ( SCSITaskIdentifier completedTask );
	
	
private:
	
	// Space reserved for future expansion.
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice,  4 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice,  5 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice,  6 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice,  7 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice,  8 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice,  9 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice, 10 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice, 11 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice, 12 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice, 13 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice, 14 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice, 15 );
	OSMetaClassDeclareReservedUnused ( IOSCSIReducedBlockCommandsDevice, 16 );
	
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif /* _IOKIT_IO_SCSI_REDUCED_BLOCK_COMMANDS_DEVICE_H_ */
