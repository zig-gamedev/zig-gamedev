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


#ifndef _IOKIT_IO_SCSI_BLOCK_COMMANDS_DEVICE_H_
#define _IOKIT_IO_SCSI_BLOCK_COMMANDS_DEVICE_H_

#if defined(KERNEL) && defined(__cplusplus)

//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------

// SBC power states as defined in T10:996D SCSI Block Commands - 3 (SBC-3)
// Revision 8c, November 13, 1997, pages 10-11.
enum
{
	kSBCPowerStateSystemSleep 	= 0,
	kSBCPowerStateSleep 		= 1,
	kSBCPowerStateStandby 		= 2,
	kSBCPowerStateIdle			= 3,
	kSBCPowerStateActive		= 4,
	kSBCNumPowerStates			= 5
};

enum
{
	kMediaStateUnlocked	= 0,
	kMediaStateLocked 	= 1
};


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

// General IOKit headers
#include <IOKit/IOLib.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/IOService.h>
#include <IOKit/IOUserClient.h>

// Generic IOKit storage related headers
#include <IOKit/storage/IOStorage.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi/IOSCSIPrimaryCommandsDevice.h>
#include <IOKit/storage/IOBlockStorageDevice.h>

// Build includes
#include <TargetConditionals.h>

// Forward declaration for the SCSIBlockCommands that is used internally by the
// IOSCSIBlockCommandsDevice class.
class SCSIBlockCommands;

//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------

/*!
 @class IOSCSIBlockCommandsDevice
 @superclass IOSCSIPrimaryCommandsDevice
 @discussion Implementation of the T10 device Block Commands device.
 */
class __exported IOSCSIBlockCommandsDevice : public IOSCSIPrimaryCommandsDevice
{

	OSDeclareAbstractStructors ( IOSCSIBlockCommandsDevice )

private:

	IOReturn				GetWriteCacheState ( IOMemoryDescriptor * 	buffer,
												 UInt8					modePageControlValue );

	static void				AsyncReadWriteComplete ( SCSITaskIdentifier	completedTask );

protected:

	// Reserve space for future expansion.
    struct IOSCSIBlockCommandsDeviceExpansionData
	{
		IONotifier *		fPowerDownNotifier;
		bool				fWriteCacheEnabled;
		bool				fDeviceIsShared;
		UInt64				fMediumBlockCount64;
#if !TARGET_OS_IPHONE
		bool				fDeviceHasSATTranslation;
        bool                fDeviceHasNVMETranslation;
#endif /* !TARGET_OS_IPHONE */
		bool				fProtocolSpecificPowerControl;
		bool				fRequiresEjectWithStartStopUnit;
		UInt32				fMaximumUnmapLBACount;
		UInt32				fMaximumUnmapBlockDescriptorCount;
        UInt64              fMaximumWriteSameLength;
        UInt8               fLBPRZ;
		bool				fUnmapAllowed;
        bool                fUseWriteSame;
	};
    IOSCSIBlockCommandsDeviceExpansionData * fIOSCSIBlockCommandsDeviceReserved;

	#define fPowerDownNotifier					fIOSCSIBlockCommandsDeviceReserved->fPowerDownNotifier
	#define fWriteCacheEnabled					fIOSCSIBlockCommandsDeviceReserved->fWriteCacheEnabled
	#define fMaximumUnmapLBACount				fIOSCSIBlockCommandsDeviceReserved->fMaximumUnmapLBACount
	#define fMaximumUnmapBlockDescriptorCount	fIOSCSIBlockCommandsDeviceReserved->fMaximumUnmapBlockDescriptorCount
    #define fMaximumWriteSameLength             fIOSCSIBlockCommandsDeviceReserved->fMaximumWriteSameLength
    #define fLBPRZ                              fIOSCSIBlockCommandsDeviceReserved->fLBPRZ
	#define fUnmapAllowed						fIOSCSIBlockCommandsDeviceReserved->fUnmapAllowed
    #define fUseWriteSame						fIOSCSIBlockCommandsDeviceReserved->fUseWriteSame

	// The fDeviceIsShared is used to indicate whether this device exists on a Physical
	// Interconnect that allows multiple initiators to access it.  This is used mainly
	// by the power management code to not send power state related START_STOP_UNIT
	// commands to the device.
	// The fDeviceIsShared value is also used to prevent power state commands from being
	// sent to manual eject device since these devices behave better when allowed to
	// manage their own power.
	#define fDeviceIsShared	fIOSCSIBlockCommandsDeviceReserved->fDeviceIsShared

	// The fMediumBlockCount64 provides support for 64 bit LBAs and
	// replaces fMediumBlockCount which only supports 32 bits.
	// This value should not be directly accessed and instead the member routine 
	// ReportMediumTotalBlockCount() should be used to retrieve it and the member routine
	// SetMediumCharacteristics() should be used to set it.
	#define fMediumBlockCount64	fIOSCSIBlockCommandsDeviceReserved->fMediumBlockCount64

#if !TARGET_OS_IPHONE
	#define fDeviceHasSATTranslation fIOSCSIBlockCommandsDeviceReserved->fDeviceHasSATTranslation
#endif /* !TARGET_OS_IPHONE */

#if !TARGET_OS_IPHONE
    #define fDeviceHasNVMETranslation fIOSCSIBlockCommandsDeviceReserved->fDeviceHasNVMETranslation
#endif /* !TARGET_OS_IPHONE */

	// Device support protocol specific power off
	#define fProtocolSpecificPowerControl fIOSCSIBlockCommandsDeviceReserved->fProtocolSpecificPowerControl

	// Device requires START_STOP_UNIT for ejects, regardless if PREVENT_ALLOW_MEIDUMREMOVAL
	// failed. 
	#define fRequiresEjectWithStartStopUnit fIOSCSIBlockCommandsDeviceReserved->fRequiresEjectWithStartStopUnit

private:
	/* OBSOLETE. Use IOSCSIPrimaryCommandsDevice::Get/SetANSIVersion */
	UInt8				fANSIVersion;

protected:
	// ---- Device Characteristics ----
	bool				fMediaIsRemovable;

	// ---- Medium Characteristics ----
	bool				fMediumPresent;

private:
	// The byte count of each physical block on the medium.
	// This value should not be directly accessed and instead the member routine 
	// ReportMediumBlockSize() should be used to retrieve it and the member routine
	// SetMediumCharacteristics() should be used to set it.
	UInt32				fMediumBlockSize;

	// The total number of blocks of fMediumBlockSize on the medium.
	// OBSOLETE.  Use fMediumBlockCount64 instead which allows for support of
	// devices that have 33 through 64 bit LBA values.
	UInt32				fMediumBlockCount;

protected:
	// Whether the installed medium is protected from writes
	bool				fMediumIsWriteProtected;

	// Whether user removal of medium has been prevented	
	bool				fMediumRemovalPrevented;

	// Indicates whether this is a known manual eject device
	bool				fKnownManualEject;

	// Polling thread variables
	thread_call_t		fPollingThread;
	UInt32				fPollingMode;
	enum
	{
		kPollingMode_Suspended		= 0,
		kPollingMode_NewMedia 		= 1,
		kPollingMode_MediaRemoval	= 2
	};

	// ---- Methods for controlling the current state of device support ----
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

	/*!
	 @function ClearNotReadyStatus
	 @abstract Clears any NOT_READY status for the device.
	 @discussion Polls device with TEST_UNIT_READY until the device returns <code>kSCSITaskStatus_GOOD</code> status.
	 Exits if device has become disconnected.
	 @result <code>true</code> if device is now active (isInactive == false) and <code>false</code> otherwise.
	 */
	virtual bool		ClearNotReadyStatus ( void ) APPLE_KEXT_OVERRIDE;

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
	 @function GetMediumRotationRate
	 @abstract Get the rotation rate of the device.
	 @discussion Get the rotation rate of the device via its INQUIRY data. 
	 Sets the result as a property on the device's registry entry.
	 */
	void				GetMediumRotationRate ( void );

	/*!
	 @function LogicalBlockProvisioningUnmapSupport
	 @abstract Check if the device supports Logical Block Provisioning Unmap.
	 @discussion Check if the device supports Logical Block Provisioning Unmap via its INQUIRY data.
	 @return <code>true</code> if unmap is supported and <code>false</code> otherwise.
	 */
	bool				LogicalBlockProvisioningUnmapSupport ( void );

	/*!
	 @function GetDeviceUnmapCharacteristics
	 @abstract Get the unmap characteristics of the device.
	 @discussion Get the unmap characteristics of the device.
	 */
	void				GetDeviceUnmapCharacteristics ( );

	// ---- Methods used for controlling the polling thread ----

	/*!
	 @function ProcessPoll
	 @abstract Process a poll for media.
	 @discussion Process a poll for media based on the current polling mode of the driver.
	 */
	virtual void		ProcessPoll ( void );

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

	// Main and support methods for polling for new Media

	/*!
	 @function PollForNewMedia
	 @abstract Polls for new media insertion.
	 @discussion Polls for new media insertion.  If new media is inserted, it is initialized and
	 clients are notified via the <code>kIOMessageMediaStateHasChanged</code> message.
	 */
	virtual void		PollForNewMedia ( void );

	/*!
	 @function DetermineMediaPresence
	 @abstract Check if media has been inserted.
	 @discussion Check if media has been inserted by sending TEST_UNIT_READY
	 and checking the SENSE data.
	 @return <code>true</code> if medium was found and <code>false</code> otherwise.
	 */
	virtual bool		DetermineMediaPresence ( void );

	/*!
	 @function PreventMediumRemoval
	 @abstract Prevent Medium Removal.
	 @discussion Prevents medium removal by sending the PREVENT_ALLOW_MEDIUM_REMOVAL command
	 with the PREVENT field set to "Medium removal shall be prohibited".
	 @return <code>true</code> if medium was successfully locked and <code>false</code> otherwise.
	 */
	virtual bool		PreventMediumRemoval ( void );

	/*!
	 @function DetermineMediumCapacity
	 @abstract Determines capacity of the medium.
	 @discussion Determines capacity of the medium via the READ_CAPACITY or READ_CAPACITY_16 commands.
	 @param blockSize used to return the block size of the medium.
	 @param blockCount used to return the block count of the medium.
	 @return <code>true</code> if medium capacity was successfully queried and <code>false</code> otherwise.
	 */
	virtual bool		DetermineMediumCapacity (
							UInt64 * 				blockSize,
							UInt64 * 				blockCount );

	/*!
	 @function DetermineMediumWriteProtectState
	 @abstract Determine medium write protect state of the medium.
	 @discussion Determine medium write protect state of the medium via MODE_SENSE_10, MODE_SENSE_6,
	 or manual READ_10 commands.
	 @return <code>true</code> if medium is in write protect state and <code>false</code> otherwise.
	 */
	virtual bool		DetermineMediumWriteProtectState ( void );

    // Main and support methods for polling for Media removal

	/*!
	 @function PollForMediaRemoval
	 @abstract Check if media has been removed.
	 @discussion Check if media has been removed by sending TEST_UNIT_READY
	 and checking the SENSE data.  If so, message clients and resets the driver's
	 medium characteristics.
	 */
	virtual void		PollForMediaRemoval ( void );

	// ---- Methods used for handling medium characteristics ----

	/*!
	 @function SetMediumCharacteristics
	 @abstract Sets the block size and block count for the current media.
	 OBSOLETE - Use the version compatible with 64 bit LBAs instead.
	 @discussion Sets the block size and block count for the current media.
	 OBSOLETE - Use the version compatible with 64 bit LBAs instead.
	 @param blockSize The media's block size in bytes.
	 @param blockCount The count of the media's blocks.
	 */
	virtual void		SetMediumCharacteristics (
							UInt32 					blockSize,
							UInt32 					blockCount );

	/*!
	 @function SetMediumCharacteristics
	 @abstract Sets the block size and block count for the current media.
	 @discussion Sets the block size and block count for the current media.
	 @param blockSize The media's block size in bytes.
	 @param blockCount The count of the media's blocks.
	 */
	void				SetMediumCharacteristics (
							UInt64 					blockSize,
							UInt64 					blockCount );

	/*!
	 @function ResetMediumCharacteristics
	 @abstract Resets the block size and block count.
	 @discussion Resets the block size and block count.
	 */
	virtual void		ResetMediumCharacteristics ( void );

	/*!
	 @function IssueRead
	 @abstract DEPRECATED. Please do not use this method.
	 @discussion DEPRECATED. Please do not use this method.
	 @param buffer DEPRECATED. Please do not use this method.
	 @param startBlock DEPRECATED. Please do not use this method.
	 @param blockCount DEPRECATED. Please do not use this method.
	 @result DEPRECATED. Please do not use this method.
	 */
	virtual IOReturn	IssueRead (
							IOMemoryDescriptor *	buffer,
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
	virtual IOReturn	IssueRead (
							IOMemoryDescriptor *	buffer,
					  		UInt64					startBlock,
					 		UInt64					blockCount,
					   		void * 					clientData );

	/*!
	 @function IssueWrite
	 @abstract DEPRECATED. Please do not use this method.
	 @discussion DEPRECATED. Please do not use this method.
	 @param buffer DEPRECATED. Please do not use this method.
	 @param startBlock DEPRECATED. Please do not use this method.
	 @param blockCount DEPRECATED. Please do not use this method.
	 @result DEPRECATED. Please do not use this method.
	 */
	virtual IOReturn	IssueWrite (
							IOMemoryDescriptor *	buffer,
							UInt64					startBlock,
					   		UInt64					blockCount );

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
	virtual IOReturn	IssueWrite (
							IOMemoryDescriptor *	buffer,
						 	UInt64					startBlock,
					  		UInt64					blockCount,
							void * 					clientData );

	/*!
	 @function IssueUnmap
	 @abstract Issue a synchronous unmap command.
	 @discussion Issue a synchronous unmap command.
	 @param extentsList a list of extents to unmap.
	 @param blockDescriptorCount the count of extents in extentsList.
	 @return A valid IOReturn value.
	 */
	IOReturn			IssueUnmap (
							IOBlockStorageDeviceExtent *	extentsList,
							UInt32							blockDescriptorCount );

	// ----- Power Management Support ------

	/*!
	 @function InitializePowerManagement
	 @abstract Called to initialize power management.
	 @discussion Called by InitializeDeviceSupport() to initialize power management by,
	 joining the power management tree, registering power states, and setting the lowest idle
	 power state.
	 @param provider A valid pointer to this object's provider.
	 */
	virtual void 		InitializePowerManagement ( IOService * provider ) APPLE_KEXT_OVERRIDE;

	/*!
	 @function GetInitialPowerState
	 @abstract Called by power management to determine initial power state.
	 @discussion We override this method so that when we register for power management,
	 we go to our active power state (which the drive is definitely in
	 at startup time).
	 @result Returns the initial power state desired when power management is initialized.
	 */
	virtual UInt32		GetInitialPowerState ( void ) APPLE_KEXT_OVERRIDE;

	/*!
	 @function GetNumberOfPowerStateTransitions
	 @abstract Called by power management to determine this class's total
	 number of power state transitions.
	 @discussion We override this method in order to provide the number of transitions
	 from Fully active to Sleep state so that the idle timer can be adjusted
	 to the appropriate time period based on the disk spin down time set in
	 the Energy Saver prefs panel.
	 @result Returns the number of power state transitions for this class.
	 */
	virtual UInt32		GetNumberOfPowerStateTransitions ( void ) APPLE_KEXT_OVERRIDE;

	/*!
	 @function TicklePowerManager
	 @abstract The TicklePowerManager method is called to tell the power manager that the
	 device needs to be in a certain power state to handle requests.
	 @discussion The TicklePowerManager() is called by CheckPowerState() and
	 sends an activity tickle ( via activityTickle() ) to the power manager
	 so that the idle timer is reset.
	 Subclasses must override this method. Third party subclasses shouldn't need to override
	 this method but can to alter the default behavior.
	 */
	virtual void		TicklePowerManager ( void ) APPLE_KEXT_OVERRIDE;

	/*!
	 @function HandlePowerChange
	 @abstract Handles the state machine for power management state changes.
	 @discussion The HandlePowerChange method is the state machine for power management.
	 It is guaranteed to be on its own thread of execution (different from
	 the power manager thread AND the workloop thread. This routine can
	 send sync or async calls to the drive without worrying about threading
	 issues.
	 */
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
	 object transitions to its maximum power state.
	 */
	virtual void		HandleCheckPowerState ( void ) APPLE_KEXT_OVERRIDE;

	/*!
	 @function VerifyMediumPresence
	 @abstract Called to ascertain the current presence of media.
	 @discussion The VerifyMediumPresence method is called to see if the medium which we
	 anticipated being there is still there.
	 @result <code>true</code> if media is present and <code>false</code> if media is not present.
	 */
	virtual bool		VerifyMediumPresence ( void );

	/*!
	 @function CheckMediumCapacityData
	 @abstract Called to ascertain if the medium has changed capacity data.
	 If so, it messages the upper layers.
	 @discussion The CheckMediumCapacityData is called to see if the medium has indeed changed
	 capacity data. If it has, it messages the upper layers.
	 */
	void 				CheckMediumCapacityData ( void );

public:

	/*!
	 @function sProcessPoll
	 @abstract Static method called to poll for media.
	 @discussion Static method called to poll for media.  Called from polling thread.
	 Will reschedule itself via EnablePolling if polling is not suspended.
	 @param pdtDriver a pointer to a valid IOSCSIBlockCommandsDevice ( or subclass ) which is to
	 be polled.
	 @param refCon a pointer to an additional object which can be determined by the implementer.
	 */
	static 	void		sProcessPoll ( void * pdtDriver, void * refCon );

	// ---- Methods for controlling the device ----

	/*!
	 @function SyncReadWrite
	 @abstract DEPRECATED. Please do not use this method.
	 @discussion DEPRECATED. Please do not use this method.
	 @param buffer DEPRECATED. Please do not use this method.
	 @param startBlock DEPRECATED. Please do not use this method.
	 @param blockCount DEPRECATED. Please do not use this method.
	 @param blockSize DEPRECATED. Please do not use this method.
	 @result DEPRECATED. Please do not use this method.
	 */
	virtual IOReturn	SyncReadWrite (
							IOMemoryDescriptor *	buffer,
							UInt64					startBlock,
							UInt64					blockCount,
							UInt64					blockSize );

	/*!
	 @function AsyncReadWrite
	 @abstract Translates an asynchronous I/O request into a read or a write.
	 @discussion Translates an asynchronous I/O request into a read or a write.
	 @param buffer A pointer to a valid IOMemoryDescriptor to which the
	 data can be copied to or from.
	 @param startBlock Starting logical block address for the write operation.
	 @param blockCount The number of blocks to read or write.
	 @param blockSize used to specify the block size to be used for the read or write.
	 @param clientData A valid pointer to the client data to be used for callback completion.
	 @result A valid IOReturn value.
	 */
	virtual IOReturn	AsyncReadWrite (
							IOMemoryDescriptor *	buffer,
							UInt64					startBlock,
							UInt64					blockCount,
							UInt64					blockSize,
							void * 					clientData );

	/*!
	 @function GetWriteCacheState
	 @abstract Get the write cache state.
	 @discussion Get the write cache state.
	 @param enabled used to return the current write cache state.
	 @return A valid IOReturn value.
	 */
	IOReturn	GetWriteCacheState ( bool * enabled );	

	/*!
	 @function SetWriteCacheState
	 @abstract Sets the write cache state.
	 @discussion Sets the write cache state.
	 @param enabled the state of the write cache.
	 @return A valid IOReturn value.
	 */
	IOReturn	SetWriteCacheState ( bool enabled );

	/*!
	 @function DetermineMediumGeometry
	 @abstract Determines geometry of the medium.
	 @discussion Determines geometry of the medium via mode sense data and places
	 the results in the Device Characteristics Dictionary
	 */
	void		DetermineMediumGeometry ( void );

	// ---- Methods for controlling medium state ----
	/*!
	 @function EjectTheMedium
	 @abstract Called to eject the inserted media from the drive.
	 @discussion Called by IOBlockStorageDevice subclasses to eject the inserted
	 media from the drive.
	 @result A valid IOReturn value.
	 */
	virtual IOReturn	EjectTheMedium ( void );

	/*!
	 @function LockUnlockMedium
	 @abstract Called to lock or unlock the inserted media.
	 @discussion Called by IOBlockStorageDevice subclasses to lock or unlock the inserted media.
	 @param doLock if true the media will be locked, if false the media will be unlocked.
	 @result A valid IOReturn value.
	 */
	virtual IOReturn	LockUnlockMedium ( bool doLock );

	/*!
	 @function SynchronizeCache
	 @abstract Called to synchronize the devices cache and thereby force its contents to be
	 written out to the media.
	 @discussion Called by IOBlockStorageDevice subclasses to synchronize the devices cache
	 and thereby force the cache's contents to be written out to the media.
	 @result A valid IOReturn value.
	 */
	virtual IOReturn	SynchronizeCache ( void );

	// ---- Methods for controlling media format ----

	/*!
	 @function FormatMedium
	 @abstract Called to perform a low level format of the inserted media.
	 @discussion Called IOBlockStorageDevice subclasses to perform a low level
	 of the inserted media. Default implementation simply returns kIOReturnUnsupported
	 if not overridden.
	 @param blockCount The number of blocks to be formatted.
	 @param blockSize used to specify the block size to be used for the format.
	 @result A valid IOReturn value.
	 */
	virtual IOReturn	FormatMedium (
							UInt64 					blockCount,
							UInt64 					blockSize );

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
	virtual UInt32		GetFormatCapacities (
							UInt64 * 				capacities,
			 				UInt32   				capacitiesMaxCount ) const;

	// ---- Query methods to report device characteristics ----

	/*!
	 @function ReportDeviceMaxBlocksReadTransfer
	 @abstract Reports maximum read transfer blocks.
	 @discussion Reports the maximum number of blocks that the device can handle per
	 read.  A value of 0 (zero) indicates there is no limit aside from the size of 
	 the method's return parameter.
	 @return the maximum read transfer blocks.
	 */
	virtual UInt64		ReportDeviceMaxBlocksReadTransfer ( void );

	/*!
	 @function ReportDeviceMaxBlocksWriteTransfer
	 @abstract Reports maximum write transfer blocks.
	 @discussion Reports the maximum number of blocks that the device can handle per
	 write.  A value of 0 (zero) indicates there is no limit aside from the size of 
	 the method's return parameter.
	 @return the maximum write transfer blocks.
	 */
	virtual UInt64		ReportDeviceMaxBlocksWriteTransfer ( void );

	/*!
	 @function ReportDeviceMediaRemovability
	 @abstract Reports whether the device supports removal of the media.
	 @discussion Reports whether the device supports removal of the media.
	 @return <code>true</code> if media is removable and <code>false</code> otherwise.
	 */
	virtual bool		ReportDeviceMediaRemovability ( void );

	// ---- Query methods to report installed medium characteristics ----

	/*!
	 @function ReportMediumBlockSize
	 @abstract Reports the logical block size of the medium.
	 @discussion Reports the logical block size of the medium.
	 @return the logical block size.
	 */
	virtual UInt64		ReportMediumBlockSize ( void );

	/*!
	 @function ReportMediumTotalBlockCount
	 @abstract Reports the total logical block count of the medium.
	 @discussion Reports the total logical block count of the medium.
	 @return the total logical block count.
	 */
	virtual UInt64		ReportMediumTotalBlockCount ( void );

	/*!
	 @function ReportMediumWriteProtection
	 @abstract Reports whether or not the medium is write protected.
	 @discussion Reports whether or not the media is write protected.
	 @return <code>true</code> if media is write protected and <code>false</code> otherwise.
	 */
	virtual bool		ReportMediumWriteProtection ( void );

    // ---- Query method to report the device provision initialization pattern ----

	/*!
	 @function ReportProvisioningInitializationPattern
	 @abstract Reports the device provision initialization pattern.
	 @discussion Reports the device provision initialization pattern.
	 @param buffer the buffer into which the results will be copied.
	 @result A valid IOReturn value.
	 */
	IOReturn            ReportProvisioningInitializationPattern (
                            IOMemoryDescriptor *                    buffer );

	/*! @function message
	 @abstract Receives messages delivered from an attached provider.
	 @discussion Handles the <code>kIOMessageMediaParametersHaveChanged</code> message
	 by CheckMediumCapacityData on a separate thread.
	 @param type A type defined in <code>IOMessage.h</code>.
	 @param nub The provider from which the message originates.
	 @param arg An argument defined by the message type.
	 @result An IOReturn code defined by the message type.
	 */
	virtual IOReturn	message ( UInt32 type, IOService * nub, void * arg ) APPLE_KEXT_OVERRIDE;

	// ---- Called prior to restarting or shutting down ----

	/*! @function systemWillShutdown
	 @abstract Handles system shutdown and restart notifications.
	 @discussion Overrides <code>IOService::systemWillShutdown</code> in order
	 to execute any special handling as specified in the device characteristics dictionary.
	 As a result, drivers can expect their <code>disable</code> method to be called
	 before system shutdown or restart. This implementation is synchronous and can
	 block before calling <code>IOService::systemWillShutdown</code> and return.
	 @param specifier
	 <code>kIOMessageSystemWillPowerOff</code> or <code>kIOMessageSystemWillRestart</code>.
	 @see //apple_ref/cpp/instm/IOService/systemWillShutdown/void/(IOOptionBits) IOService::systemWillShutdown
	 */
	virtual void systemWillShutdown ( IOOptionBits specifier ) APPLE_KEXT_OVERRIDE;

    // ---- Unmap related methods ----
    // Check to see if we can coalesce the current and the next extent

	/*!
	 @function UnmapTryExtentCoalesce
	 @abstract Helper function to check and coalesce two extents.
	 @discussion Helper function to check and coalesce two extents.  In order to be
	 successfully merged, the extents must be sequential.
	 @param mergedExtent a pointer where the new merged extent will be placed.
	 @param currentExtent the first extent to merge.
	 @param nextExtent the second extent to merge.
	 @return <code>true</code> if the extents were merged and <code>false</code> otherwise.
	 */
	bool		UnmapTryExtentCoalesce (
							IOBlockStorageDeviceExtent *			mergedExtent,
							IOBlockStorageDeviceExtent *			currentExtent,
							IOBlockStorageDeviceExtent *			nextExtent );

	/*!
	 @function UnmapTruncateAndAccumulate
	 @abstract Helper function to truncate an extent if required.
	 @discussion Adds an extent to the end of the extent unmap dispatch list.
	 If there are not enough unmap LBAs to fit the whole extent, it is split and the remaining,
	 truncated extent is returned.
	 @param extentDispatchList the extent dispatch list to add the extent.
	 @param blockDescriptorCount the current count of block descriptors in the dispatch list.
	 @param unmapLBACount the current count of queued LBAs for the next unmap operation.
	 @param mergedExtent a pointer to the extent to be added.  The truncated extent is returned
	 using this pointer as well.
	 @return <code>true</code> if adding the extent used all remaining unmap LBAs
	 and <code>false</code> otherwise.
	 */
	bool		UnmapTruncateAndAccumulate (
							IOBlockStorageDeviceExtent *			extentDispatchList,
							UInt32									blockDescriptorCount,
							UInt32 *								unmapLBACount,
							IOBlockStorageDeviceExtent *			mergedExtent );

    // Use the write same command to do unmaps on the given extents.

	/*!
	 @function WriteSameUnmap
	 @abstract Uses the write same command to unmap the given extents.
	 @discussion Uses the write same command to unmap the given extents.
	 @param extents the array of extents to unmap.
	 @param extentsCount the count of extents in the provided array.
	 @param requestBlockSize used to specify the block size to be used for the write.
	 @return A valid IOReturn value.
	 */
	IOReturn	WriteSameUnmap (
							IOBlockStorageDeviceExtent *			extents,
							UInt32									extentsCount,
							UInt32									requestBlockSize );

	/*!
	 @function UpdateLBAProvisionStatus
	 @abstract Get the block provision status.
	 @discussion Update the extents with the provision status that we got from the device.
	 @param extentsCount the count of extents in <code>extents</code>.
	 @param extents a list of extents to unmap.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the received
	 status data is to be stored.
	 @param transferLength used to specify the maximum requested amount of status data.
	 @return A valid IOReturn value.
	 */
	IOReturn	UpdateLBAProvisionStatus (
							UInt32 *								extentsCount,
							IOBlockStorageProvisionDeviceExtent *	extents,
							IOMemoryDescriptor *					dataBuffer,
							UInt32									transferLength );

	/*!
	 @function IsUnmapAllowed
	 @abstract Return whether unmap is allowed.
	 @discussion Return whether unmap is allowed.
	 @return <code>true</code> if unmap is allowed and <code>false</code> otherwise.
	 */
	bool		IsUnmapAllowed ( );

	/*!
	 @function IsUseWriteSame
	 @abstract Return whether WRITE_SAME should be used for UNMAP commands.
	 @discussion Return whether WRITE_SAME should be used for UNMAP commands.
	 @return <code>true</code> if the device supports unmap operations with write same command
	 and <code>false</code> otherwise.
	 */
	bool		IsUseWriteSame ( );

protected:


	/*!
	 @function GET_LBA_STATUS
	 @abstract Builds a GET_LBA_STATUS command.
	 @discussion Builds a GET_LBA_STATUS command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the received
	 status data is to be stored.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address
	 from which the get status is to be initiated.
	 @param ALLOCATION_LENGTH used to specify the maximum requested amount of status data.
	 @param CONTROL the CONTROL byte to use for the command.
	 @result <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool GET_LBA_STATUS (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						SCSICmdField8Byte			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte			ALLOCATION_LENGTH,
						SCSICmdField1Byte			CONTROL );

	 /*!
	 @function READ_10
	 @abstract Builds a READ (10) command.
	 @discussion Builds a READ (10) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the read data is to be stored.
	 @param blockSize used to specify the block size to be used for the read.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param RELADR the value of the relative address bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the read is to be initiated.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param CONTROL the CONTROL byte to use for the command.
	 @result <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	virtual bool READ_10 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			RELADR,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField2Byte 			TRANSFER_LENGTH,
						SCSICmdField1Byte 			CONTROL );

	// Defined in SBC-2 section 5.10
	/*!
	 @function READ_10
	 @abstract Builds a READ (10) command.
	 @discussion Builds a READ (10) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the read data is to be stored.
	 @param blockSize used to specify the block size to be used for the read.
	 @param RDPROTECT the value of the read protect field to set in the command.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param FUA_NV the value of the force unit access bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the read is to be initiated.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param CONTROL the CONTROL byte to use for the command.
	 @result <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool READ_10 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField3Bit 			RDPROTECT,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			FUA_NV,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField2Byte 			TRANSFER_LENGTH,
						SCSICmdField1Byte 			CONTROL );

	/*!
	 @function READ_12
	 @abstract Builds a READ (12) command.
	 @discussion Builds a READ (12) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the read data is to be stored.
	 @param blockSize used to specify the block size to be used for the read.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param RELADR the value of the relative address bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the read is to be initiated.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	virtual bool READ_12 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			RELADR,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte 			TRANSFER_LENGTH,
						SCSICmdField1Byte 			CONTROL );

	// Defined in SBC-2 section 5.11
	/*!
	 @function READ_12
	 @abstract Builds a READ (12) command.
	 @discussion Builds a READ (12) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the read data is to be stored.
	 @param blockSize used to specify the block size to be used for the read.
	 @param RDPROTECT the value of the read protect field to set in the command.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param FUA_NV the value of the force unit access bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the read is to be initiated.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool READ_12 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField3Bit			RDPROTECT,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			FUA_NV,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte 			TRANSFER_LENGTH,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField1Byte 			CONTROL );

	/*!
	 @function READ_16
	 @abstract Builds a READ (16) command.
	 @discussion Builds a READ (16) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the read data is to be stored.
	 @param blockSize used to specify the block size to be used for the read.
	 @param RDPROTECT the value of the read protect field to set in the command.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param FUA_NV the value of the force unit access bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the read is to be initiated.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool READ_16 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField3Bit			RDPROTECT,
						SCSICmdField1Bit			DPO,
						SCSICmdField1Bit			FUA,
						SCSICmdField1Bit			FUA_NV,
						SCSICmdField8Byte			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte			TRANSFER_LENGTH,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField1Byte			CONTROL );

	/*!
	 @function READ_CAPACITY
	 @abstract Builds a READ CAPACITY command.
	 @discussion Builds a READ CAPACITY command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the read capacity data is to be stored.
	 @param RELADR the value of the relative address bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which
	 the read capacity is to be initiated.
	 @param PMI the value of the partial medium indicator bit to set in the command.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	virtual bool READ_CAPACITY (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						SCSICmdField1Bit 			RELADR,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField1Bit 			PMI,
						SCSICmdField1Byte 			CONTROL );

	/*!
	 @function READ_CAPACITY_16
	 @abstract Builds a READ CAPACITY (16) command.
	 @discussion Builds a READ CAPACITY (16) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the read capacity data is to be stored.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which
	 the read capacity is to be initiated.
	 @param ALLOCATION_LENGTH used to specify the maximum requested amount of read data.
	 @param PMI the value of the partial medium indicator bit to set in the command.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool READ_CAPACITY_16 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						SCSICmdField8Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte			ALLOCATION_LENGTH,
						SCSICmdField1Bit 			PMI,
						SCSICmdField1Byte 			CONTROL );

    /*!
	 @function REPORT_PROVISIONING_INITIALIZATION_PATTERN
	 @abstract Builds a REPORT_PROVISIONING_INITIALIZATION_PATTERN command.
	 @discussion Builds a REPORT_PROVISIONING_INITIALIZATION_PATTERN command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the
	 provisioning initialization pattern data is to be stored.
     @param ALLOCATION_LENGTH used to specify the maximum requested amount of provisioning data.
     @param CONTROL the CONTROL byte to use for the command.
     @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
     */
    bool REPORT_PROVISIONING_INITIALIZATION_PATTERN (
                        SCSITaskIdentifier          request,
                        IOMemoryDescriptor *        dataBuffer,
                        SCSICmdField4Byte           ALLOCATION_LENGTH,
                        SCSICmdField1Byte           CONTROL );

	/*!
	 @function START_STOP_UNIT
	 @abstract Builds a START STOP UNIT command.
	 @discussion Builds a START STOP UNIT command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param IMMED the value of the immediate bit to set in the command.
	 @param POWER_CONDITIONS used to specify the desired POWER CONDITIONS code.
	 @param LOEJ used to specify whether or not the LOAD/EJECT ( LEOJ ) bit for the command needs to be set.
	 @param START the value of the START bit to set in the command.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	virtual bool START_STOP_UNIT (
						SCSITaskIdentifier			request,
						SCSICmdField1Bit 			IMMED,
						SCSICmdField4Bit 			POWER_CONDITIONS,
						SCSICmdField1Bit 			LOEJ,
						SCSICmdField1Bit 			START,
						SCSICmdField1Byte 			CONTROL );

	/*!
	 @function SYNCHRONIZE_CACHE
	 @abstract Builds a SYNCHRONIZE CACHE command.
	 @discussion Builds a SYNCHRONIZE CACHE command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param IMMED the value of the immediate bit to set in the command.
	 @param RELADR the value of the relative address bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which
	 the synchronize cache is to be initiated.
	 @param NUMBER_OF_BLOCKS the number of blocks to synchronize.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	virtual bool SYNCHRONIZE_CACHE (
						SCSITaskIdentifier			request,
						SCSICmdField1Bit 			IMMED,
						SCSICmdField1Bit 			RELADR,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField2Byte 			NUMBER_OF_BLOCKS,
						SCSICmdField1Byte 			CONTROL );

	// Defined in SBC-2 section 5.22
	/*!
	 @function SYNCHRONIZE_CACHE
	 @abstract Builds a SYNCHRONIZE CACHE command.
	 @discussion Builds a SYNCHRONIZE CACHE command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param IMMED the value of the immediate bit to set in the command.
	 @param SYNC_NV used to specify whether or not the synchronize non-volatile bit for the command needs to be set.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which
	 the synchronize cache is to be initiated.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param NUMBER_OF_BLOCKS the number of blocks to synchronize.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool SYNCHRONIZE_CACHE (
						SCSITaskIdentifier			request,
						SCSICmdField1Bit 			IMMED,
						SCSICmdField1Bit 			SYNC_NV,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField2Byte 			NUMBER_OF_BLOCKS,
						SCSICmdField1Byte 			CONTROL );

	/*!
	 @function SYNCRONIZE_CACHE_16
	 @abstract Builds a SYNCHRONIZE CACHE (16) command.
	 @discussion Builds a SYNCHRONIZE CACHE (16) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param SYNC_NV used to specify whether or not the synchronize non-volatile bit for the command needs to be set.
	 @param IMMED the value of the immediate bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which
	 the synchronize cache is to be initiated.
	 @param NUMBER_OF_BLOCKS the number of blocks to synchronize.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool SYNCRONIZE_CACHE_16 (
						SCSITaskIdentifier			request,
						SCSICmdField1Bit			SYNC_NV,
						SCSICmdField1Bit			IMMED,
						SCSICmdField8Byte			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte			NUMBER_OF_BLOCKS,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField1Byte			CONTROL );

	/*!
	 @function UNMAP
	 @abstract Builds a UNMAP command.
	 @discussion Builds a UNMAP command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the
	 parameter list data is supplied.
	 @param ANCHOR the value of the anchor bit to set in the command.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param PARAMETER_LIST_LENGTH the length of the supplied parameter list data buffer in bytes.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool UNMAP (
						SCSITaskIdentifier          request,
						IOMemoryDescriptor *        dataBuffer,
						SCSICmdField1Bit            ANCHOR,
						SCSICmdField6Bit            GROUP_NUMBER,
						SCSICmdField2Byte           PARAMETER_LIST_LENGTH,
						SCSICmdField1Byte           CONTROL );

	/*!
	 @function WRITE_10
	 @abstract Builds a WRITE (10) command.
	 @discussion Builds a WRITE (10) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
	 @param blockSize used to specify the block size to be used for the write.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param EBP the value of the erase by pass bit to set in the command.
	 @param RELADR the value of the relative address bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the write is to be initiated.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	virtual bool WRITE_10 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			EBP,
						SCSICmdField1Bit 			RELADR,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField2Byte 			TRANSFER_LENGTH,
						SCSICmdField1Byte 			CONTROL );

	//  Defined in SBC-2 section 5.29
	/*!
	 @function WRITE_10
	 @abstract Builds a WRITE (10) command.
	 @discussion Builds a WRITE (10) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
	 @param blockSize used to specify the block size to be used for the write.
	 @param WRPROTECT the value of the write protect field to set in the command.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param FUA_NV the value of the force unit access bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the write is to be initiated.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool WRITE_10 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField3Bit			WRPROTECT,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			FUA_NV,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField2Byte 			TRANSFER_LENGTH,
						SCSICmdField1Byte 			CONTROL );

	/*!
	 @function WRITE_12
	 @abstract Builds a WRITE (12) command.
	 @discussion Builds a WRITE (12) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
	 @param blockSize used to specify the block size to be used for the write.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param EBP the value of the erase by pass bit to set in the command.
	 @param RELADR the value of the relative address bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the write is to be initiated.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	virtual bool WRITE_12 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			EBP,
						SCSICmdField1Bit 			RELADR,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte 			TRANSFER_LENGTH,
						SCSICmdField1Byte 			CONTROL );

	//  Defined in SBC-2 section 5.30
	/*!
	 @function WRITE_12
	 @abstract Builds a WRITE (12) command.
	 @discussion Builds a WRITE (12) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
	 @param blockSize used to specify the block size to be used for the write.
	 @param WRPROTECT the value of the write protect field to set in the command.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param FUA_NV the value of the force unit access bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the write is to be initiated.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool WRITE_12 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField3Bit			WRPROTECT,
						SCSICmdField1Bit 			DPO,
						SCSICmdField1Bit 			FUA,
						SCSICmdField1Bit 			FUA_NV,
						SCSICmdField4Byte 			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField4Byte 			TRANSFER_LENGTH,
						SCSICmdField1Byte 			CONTROL );

	/*!
	 @function WRITE_16
	 @abstract Builds a WRITE (16) command.
	 @discussion Builds a WRITE (16) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param dataBuffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
	 @param blockSize used to specify the block size to be used for the write.
	 @param WRPROTECT the value of the write protect field to set in the command.
	 @param DPO the value of the disable page out bit to set in the command.
	 @param FUA the value of the force unit access bit to set in the command.
	 @param FUA_NV the value of the force unit access bit to set in the command.
	 @param LOGICAL_BLOCK_ADDRESS used to specify the logical block address from which the write is to be initiated.
	 @param TRANSFER_LENGTH the desired transfer length in blocks.
	 @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param CONTROL the CONTROL byte to use for the command.
	 @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
	 */
	bool WRITE_16 (
						SCSITaskIdentifier			request,
						IOMemoryDescriptor *		dataBuffer,
						UInt32						blockSize,
						SCSICmdField3Bit			WRPROTECT,
						SCSICmdField1Bit			DPO,
						SCSICmdField1Bit			FUA,
						SCSICmdField1Bit			FUA_NV,
						SCSICmdField8Byte			LOGICAL_BLOCK_ADDRESS,
						SCSICmdField4Byte			TRANSFER_LENGTH,
						SCSICmdField6Bit			GROUP_NUMBER,
						SCSICmdField1Byte			CONTROL );	

    /*!
	 @function WRITE_SAME_10
	 @abstract Builds a WRITE SAME (10) command.
	 @discussion Builds a WRITE SAME (10) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param buffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
     @param requestBlockSize used to specify the block size to be used for the write.
	 @param WRPROTECT the value of the write protect field to set in the command.
	 @param ANCHOR the value of the anchor bit to set in the command.
     @param UNMAP the value of the unmap bit to set in the command.
	 @param startBlock Starting logical block address for the write or unmap operation.
     @param GROUP_NUMBER the value of the grouping function group number to set in the command.
	 @param blockCount The number of blocks to write or unmap.
     @param CONTROL the CONTROL byte to use for the command.
     @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
     */
    bool WRITE_SAME_10 (
                        SCSITaskIdentifier          request,
                        IOMemoryDescriptor *        buffer,
                        UInt32                      requestBlockSize,
                        SCSICmdField3Bit			WRPROTECT,
                        SCSICmdField1Bit            ANCHOR,
                        SCSICmdField1Bit            UNMAP,
                        SCSICmdField4Byte           startBlock,
                        SCSICmdField6Bit			GROUP_NUMBER,
                        SCSICmdField2Byte           blockCount,
                        SCSICmdField1Byte           CONTROL );

    /*!
	 @function WRITE_SAME_16
	 @abstract Builds a WRITE SAME (16) command.
	 @discussion Builds a WRITE SAME (16) command.
	 @param request a SCSITaskIdentifier in which the built command will be stored.
	 @param buffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
	 @param requestBlockSize used to specify the block size to be used for the write.
	 @param WRPROTECT the value of the write protect field to set in the command.
	 @param ANCHOR the value of the anchor bit to set in the command.
	 @param UNMAP the value of the unmap bit to set in the command.
     @param NDOB used to specify whether or not the no data-out buffer bit for the command needs to be set.
	 @param startBlock Starting logical block address for the write or unmap operation.
	 @param blockCount The number of blocks to write or unmap.
     @param GROUP_NUMBER the value of the grouping function group number to set in the command.
     @param CONTROL the CONTROL byte to use for the command.
     @return <code>true</code> if the command was successfully built and <code>false</code> otherwise.
     */
    bool WRITE_SAME_16 (
                        SCSITaskIdentifier          request,
                        IOMemoryDescriptor *        buffer,
                        UInt32                      requestBlockSize,
                        SCSICmdField3Bit			WRPROTECT,
                        SCSICmdField1Bit            ANCHOR,
                        SCSICmdField1Bit            UNMAP,
                        SCSICmdField1Bit            NDOB,
                        SCSICmdField8Byte           startBlock,
                        SCSICmdField4Byte           blockCount,
                        SCSICmdField6Bit            GROUP_NUMBER,
                        SCSICmdField1Byte           CONTROL );

	/* Added with 10.2 */
	OSMetaClassDeclareReservedUsed ( IOSCSIBlockCommandsDevice, 1 );

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
	OSMetaClassDeclareReservedUsed ( IOSCSIBlockCommandsDevice, 2 );

protected:

	/*!
	 @function SetMediumIcon
	 @abstract Sets graphical icon for device's medium objects.
	 @discussion Provides an opportunity for the driver to select a more fitting icon then default icon.
	 */
	virtual	void		SetMediumIcon ( void );


	/* Added with 10.3.3 */		
	OSMetaClassDeclareReservedUsed ( IOSCSIBlockCommandsDevice, 3 );

protected:

	/*!
	 @function AsyncReadWriteCompletion
	 @abstract Completion routine for asynchronous read and write requests.
	 @discussion Completion routine for asynchronous read and write requests.
	 @param completedTask the task from which to extract the client data to inspect.
	 */
	virtual	void AsyncReadWriteCompletion ( SCSITaskIdentifier completedTask );


	/* Added with 10.6.0 */
	OSMetaClassDeclareReservedUsed ( IOSCSIBlockCommandsDevice, 4 );

public:

	/*!
	 @function AsyncReadWrite
	 @abstract Translates an asynchronous I/O request into a read or a write.
	 @discussion Translates an asynchronous I/O request into a read or a write.
	 @param buffer A pointer to a valid IOMemoryDescriptor to which the
	 data can be copied to or from.
	 @param startBlock Starting logical block address for the write operation.
	 @param blockCount The number of block to be write.
	 @param blockSize used to specify the block size to be used for the read or write.
	 @param attributes used to set the attributes of the read or write.
	 Currently only used to set kIOStorageOptionForceUnitAccess.
	 @param clientData A valid pointer to the client data to be used for callback completion.
	 @result A valid IOReturn value.
	 */
	virtual	IOReturn AsyncReadWrite (
							IOMemoryDescriptor *	buffer,
							UInt64					startBlock,
							UInt64					blockCount,
							UInt64					blockSize,
							IOStorageAttributes *	attributes,
							void * 					clientData );

	OSMetaClassDeclareReservedUsed ( IOSCSIBlockCommandsDevice, 5 );

	/*!
	 @function Unmap
	 @abstract Unmap the given extents.
	 @discussion Unmap the given extents.
	 @param extents a list of extents to unmap.
	 @param ExtentsCount the count of extents in <code>extents</code>.
	 @result A valid IOReturn value.
	 */
	virtual IOReturn Unmap ( IOBlockStorageDeviceExtent *	extents,
							 UInt32							ExtentsCount );

    OSMetaClassDeclareReservedUsed ( IOSCSIBlockCommandsDevice, 6 );

    /*!
	 @function WriteSame
	 @abstract Write or unmap the given extent.
	 @discussion Write or unmap the given extent.
	 @param buffer a pointer to a valid IOMemoryDescriptor in which the data to be written is stored.
	 @param startBlock Starting logical block address for the write or unmap operation.
	 @param blockCount The number of blocks to write or unmap.
     @param writeSameOptions the command options to pass to WRITE_SAME.
	 @param requestBlockSize used to specify the block size to be used for the write.
	 @result A valid IOReturn value.
     */
    virtual IOReturn WriteSame (
                            IOMemoryDescriptor *	buffer,
                            UInt64					startBlock,
                            UInt64					blockCount,
                            UInt8                   writeSameOptions,
                            UInt32                  requestBlockSize );

	OSMetaClassDeclareReservedUsed ( IOSCSIBlockCommandsDevice, 7 );

	/*!
	 @function GetProvisionStatus
	 @abstract Get the block provision status.
	 @discussion Get the block provision status.
	 @param block Starting logical block address for the status operation.
	 @param nblks The number of blocks.
	 @param extentsCount the count of extents in <code>extents</code>.
	 @param extents a list of extents to unmap.
	 @result A valid IOReturn value.
	 */
	virtual IOReturn GetProvisionStatus ( UInt64									block,
										  UInt64									nblks,
										  UInt32 *									extentsCount,
										  IOBlockStorageProvisionDeviceExtent *		extents );


private:

#if !TARGET_OS_IPHONE
	// Space reserved for future expansion.
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 8 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 9 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 10 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 11 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 12 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 13 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 14 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 15 );
	OSMetaClassDeclareReservedUnused ( IOSCSIBlockCommandsDevice, 16 );
#endif /* !TARGET_OS_IPHONE */

};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif /* _IOKIT_IO_SCSI_BLOCK_COMMANDS_DEVICE_H_ */
