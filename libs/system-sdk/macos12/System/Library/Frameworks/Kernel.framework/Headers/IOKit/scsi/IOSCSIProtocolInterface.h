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

#ifndef _IOKIT_IO_SCSI_PROTOCOL_INTERFACE_H_
#define _IOKIT_IO_SCSI_PROTOCOL_INTERFACE_H_


/*! @header SCSI Protocol Interface
	@discussion
	This file contains definitions for the IOSCSIProtocolInterface class,
	SCSI Protocol Features used by this interface, and additional constants
	used by this interface.
*/


//-----------------------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------------------

#include <IOKit/storage/IOStorageDeviceCharacteristics.h>
#include <TargetConditionals.h>

//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------

/*
SCSI Device Characteristics - Defined between SCSI Application Layer and
							  SCSI Protocol Layer only.
*/

/*!
@constant kIOPropertySCSIDeviceCharacteristicsKey
@discussion
This key is used to define SCSI Device Characteristics for a particular device.
It is the key for the dictionary containing the keys of characteristics. These keys
are only defined between the SCSI Protocol Layer and the SCSI Applicaiton Layer. Some
properties may be copied from this dictionary to the more generic Device Characteristics
or Protocol Characteristics dictionaries.
*/
#define kIOPropertySCSIDeviceCharacteristicsKey		"SCSI Device Characteristics"

/*!
@constant kIOPropertySCSIInquiryLengthKey
@discussion
This key is used to define a default INQUIRY length to issue to the device. The
value is a UInt32 corresponding to the number of bytes to request in the INQUIRY
command.
*/
#define kIOPropertySCSIInquiryLengthKey				"Inquiry Length"

/*!
@constant kIOPropertySCSIManualEjectKey
@discussion
This key is used to indicate that the device is known to be a manual ejectable media
device.  This property overrides all of the driver checks for determining this capability.
This property is a string, although if it exists it should always be true.
*/
#define kIOPropertySCSIManualEjectKey				"Manual Eject"

/*!
@constant kIOPropertyReadTimeOutDurationKey
@discussion
This key is used to define the Read Time Out for a particular device.
This property overrides all of the protocol defaults.
This property is a value, in milliseconds.
*/
#define kIOPropertyReadTimeOutDurationKey			"Read Time Out Duration"

/*!
@constant kIOPropertyWriteTimeOutDurationKey
@discussion
This key is used to define the Write Time Out for a particular device.
This property overrides all of the protocol defaults.
This property is a value, in milliseconds.
*/
#define kIOPropertyWriteTimeOutDurationKey			"Write Time Out Duration"

/*!
@constant kIOPropertyRetryCountKey
@discussion
This key is used to define the number of Read/Write retries for a particular device.
This property overrides all of the protocol defaults.
The value is a UInt32 corresponding to the number of retries.
*/
#define kIOPropertyRetryCountKey					"Retry Count"

/*!
 @constant kIOPropertyAutonomousSpinDownKey
 @discussion
 This key is used to indicate that the device is known to have its own internal logic
 for idle disk spin down. This key is used to mark device which respond poorly to our
 efforts to manually spin down or spin up the device when it is already in the desired 
 state. 
 */
#define kIOPropertyAutonomousSpinDownKey			"Autonomous Spin Down"

/*!
 @constant kIOPropertyDoNotPreventMediumRemovalKey
 @discussion
 This key is used to indicate that the device either does not require or respond
 well to PREVENT_ALLOW_MEDIUM_REMOVAL. This property indicates that the 
 PREVENT_ALLOW_MEDIUM_REMOVAL should not be used with the specified device.
 */
#define kIOPropertyDoNotPreventMediumRemovalKey     "Do Not Prevent Medium Removal"

/*!
 @constant kIOPropertyEjectRequireStartStopUnitKey
 @discussion
 This key is used to indicate that while the device may have failed PREVENT_ALLOW_MEDIUM
 REMOVAL it still requires a START_STOP_UNIT to eject/unload media. 
 */
#define kIOPropertyEjectRequireStartStopUnitKey		"Eject Requires START_STOP_UNIT"

/*!
 @constant kIOPropertyRequiresRestartEjectKey
 @discussion
 This key is used to indicate that the device requires a START_STOP_UNIT to eject
 the device's media prior to restarting to ensuring proper function at next boot cycle.
 */
#define kIOPropertyRequiresRestartEjectKey          "Require Restart Eject"

/*!
 @constant kIOPropertyNoSenseDebounceRetries
 @discussion
 This key is used to indicate how many retries a device may require when it reports
 NO_SENSE/NO_ADITIONAL_SENSE_INFORMATION as a response to a REQUEST_SENSE.
 */
#define kIOPropertyNoSenseDebounceRetries           "NO_SENSE Debounce Retries"

#if defined(KERNEL) && defined(__cplusplus)


/*!
@constant kCFBundleIdentifierKey
@discussion
Property key for CFBundleIdentifier.
*/
#define kCFBundleIdentifierKey                      "CFBundleIdentifier"

/*!
@constant kIOSCSIArchitectureBundleIdentifierKey
@discussion
IOSCSIArchitectureModelFamily's CFBundle identifier.
*/
#define kIOSCSIArchitectureBundleIdentifierKey	"com.apple.iokit.IOSCSIArchitectureModelFamily"


// General kernel headers
#include <kern/thread.h>

// General IOKit headers
#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOWorkLoop.h>

// SCSI Architecture Model Family includes
#include <IOKit/scsi/SCSITask.h>


/*!
@enum SCSI Protocol Interface Device Notification values
@discussion
Message values for SCSI Protocol Interface Device Notifications.
@constant kSCSIProtocolNotification_DeviceRemoved
Private message sent between a SCSI protocol service provider and
SCSI application layer driver to indicate device removal.
@constant kSCSIProtocolNotification_VerifyDeviceState
Private message sent between a SCSI protocol service provider and
SCSI application layer driver to indicate device state may have
changed and the device state should be re-verified by the SCSI
Application Layer driver. An example would be a bus reset which clears
the tray locking state of an ATAPI device.
@constant kSCSIServicesNotification_ExclusivityChanged
Message sent when a change in exclusivity state occurs. Usually in
response to acquiring/releasing exclusive access to a device via a user client.
@constant kSCSIProtocolNotification_ForcePowerDown
Message sent between a SCSI protocol service provider and
SCSI application layer driver to initiate transition to the lowest power state.
@constant kSCSIProtocolNotification_ForcePowerUp
Message sent between a SCSI protocol service provider and
SCSI application layer driver to initiate transition to the highest power state.
*/
enum
{
	kSCSIProtocolNotification_DeviceRemoved			= 0x69000010,
	kSCSIProtocolNotification_VerifyDeviceState		= 0x69000020,
	kSCSIServicesNotification_ExclusivityChanged	= 0x69000030,
    kSCSIProtocolNotification_ForcePowerDown        = 0x69000040,
    kSCSIProtocolNotification_ForcePowerUp          = 0x69000050
};


/*!
@typedef SCSIProtocolFeature
@discussion
Typedef for SCSIProtocolFeature, a 32-bit quantity.
*/
typedef UInt32 SCSIProtocolFeature;

/*!
@enum SCSI Protocol Features
@discussion
The list of SCSI Protocol Features currently supported.
*/
enum
{
	
	/*!
	@constant kSCSIProtocolFeature_ACA Not yet used.
	*/
	kSCSIProtocolFeature_ACA								= 1,
	
	/*!
	@constant kSCSIProtocolFeature_CPUInDiskMode Used to determine
	if the SCSI Protocol Services Driver supports a CPU which is in
	target disk mode.
	*/
	kSCSIProtocolFeature_CPUInDiskMode						= 2,
	
	/*!
	@constant kSCSIProtocolFeature_ProtocolSpecificPolling Used
	to determine if the SCSI Protocol Services Driver supports
	protocol specific polling for media. This is used for low-power
	polling specifically for ATAPI devices on ATA buses
	*/
	kSCSIProtocolFeature_ProtocolSpecificPolling			= 3,
	
	/*!
	@constant kSCSIProtocolFeature_ProtocolSpecificSleepCommand Used
	to determine if the SCSI Protocol Services Driver supports
	protocol specific sleep commands to a drive. This is used for
	sleeping drives specifically ATAPI devices on ATA buses.
	*/
	kSCSIProtocolFeature_ProtocolSpecificSleepCommand		= 4,
	
	/*!
	@constant kSCSIProtocolFeature_GetMaximumLogicalUnitNumber If
	the SCSI Protocol Services Driver supports logical units, it will
	report the maximum addressable ID that it supports in the UInt32 pointer
	that is passed in as the serviceValue. If only one unit is supported,
	the driver should return false for this query.
	*/
	kSCSIProtocolFeature_GetMaximumLogicalUnitNumber		= 5,
	
	/*!
	@constant kSCSIProtocolFeature_MaximumReadBlockTransferCount If
	the SCSI Protocol Services Driver has a maximum number of
	blocks that can be transfered in a read request, it will return
	true to this query and return the block count in the UInt32 pointer
	that is passed in as the serviceValue.
	*/
	kSCSIProtocolFeature_MaximumReadBlockTransferCount		= 6,

	/*!
	@constant kSCSIProtocolFeature_MaximumWriteBlockTransferCount If
	the SCSI Protocol Services Driver has a maximum number of
	blocks that can be transferred in a write request, it will return
	true to this query and return the block count in the UInt32 pointer
	that is passed in as the serviceValue.
	*/
	kSCSIProtocolFeature_MaximumWriteBlockTransferCount		= 7,

	/*!
	@constant kSCSIProtocolFeature_MaximumReadTransferByteCount If
	the SCSI Protocol Services Driver has a maximum byte count
	that can be transferred in a read request, it will return
	true to this query and return the byte count in the UInt64 pointer
	that is passed in as the serviceValue.
	*/
	kSCSIProtocolFeature_MaximumReadTransferByteCount		= 8,

	/*!
	@constant kSCSIProtocolFeature_MaximumWriteTransferByteCount If
	the SCSI Protocol Services Driver has a maximum byte count
	that can be transferred in a write request, it will return
	true to this query and return the byte count in the UInt64 pointer
	that is passed in as the serviceValue.
	*/
	kSCSIProtocolFeature_MaximumWriteTransferByteCount		= 9,
	
	/*!
	@constant kSCSIProtocolFeature_SubmitDefaultInquiryData If
	the SCSI Protocol Services Driver needs any extra information to
	make any negotiation settings from the standard INQUIRY data, this 
	will be called to set that appropriately. The serviceValue will
	point to a SCSICmd_INQUIRY_StandardData buffer. The size
	of the buffer depends on the SCSI Device Characteristics
	dictionary for the device or bus. If there is no
	kIOPropertySCSIInquiryLengthKey value set in the dictionary
	or if it doesn't exist, then the size of the data will be
	the size of the full amount of Inquiry retrieved from the device.
	*/
	kSCSIProtocolFeature_SubmitDefaultInquiryData			= 10,
	
	/*!
	@constant kSCSIProtocolFeature_ProtocolAlwaysReportsAutosenseData If
	the SCSI Protocol Services Driver always reports available
	autosense data when a kSCSITaskStatus_CHECK_CONDITION is set,
	then the protocol layer should return true. E.g. FireWire
	transport drivers should respond true to this.
	*/
	kSCSIProtocolFeature_ProtocolAlwaysReportsAutosenseData	= 11,
	
	/*!
	@constant kSCSIProtocolFeature_ProtocolSpecificPowerOff If
	the SCSI Protocol Services Driver supports removing the power
	to the drive, then the protocol layer should return true. This is
	used for aggressive power management, specifically for ATAPI
	devices on ATA buses.
	*/
	kSCSIProtocolFeature_ProtocolSpecificPowerOff			= 12,
	
	/*!
	@constant kSCSIProtocolFeature_ProtocolSpecificPowerControl
	Used to determine if the SCSI Protocol Services Driver supports
	switching the power to the drive on and off. This is used for aggressive
	power management, specifically for SATAPI devices on AHCI buses.
	*/
	kSCSIProtocolFeature_ProtocolSpecificPowerControl		= 13,
	
	/*!
	@constant kSCSIProtocolFeature_ProtocolSpecificAsyncNotification
	Used to determine if the SCSI Protocol Services Driver supports
	asynchronous notifications from the drive. This is used to prevent
	polling for media, specifically for SATAPI devices on AHCI buses.
	*/
	kSCSIProtocolFeature_ProtocolSpecificAsyncNotification	= 14,
	
	/*!
	kSCSIProtocolFeature_HierarchicalLogicalUnits:
	If the SCSI Protocol Services layer supports hierarchical
	logical units, then the protocol services layer should report true
	and use IOSCSIProtocolServices::GetLogicalUnitBytes() to retrieve
	the full 8 bytes of LUN information.
	*/
	kSCSIProtocolFeature_HierarchicalLogicalUnits			= 15,
	
	/*!
	kSCSIProtocolFeature_MultiPathing:
	If the SCSI Protocol Services layer supports multi-pathing,
	then the protocol services layer should report true. 
	This is used to support multiple paths to a logical unit
	by creating a IOSCSIMultipathedLogicalUnit object.
	*/
	kSCSIProtocolFeature_MultiPathing						= 16,

    /*!
    kSCSIProtocolFeature_ProtocolSpecificLinkRetrain:
    If the SCSI Protocol Services layer supports link retrain, then the protocol
    services layer should report true.
    */
    kSCSIProtocolFeature_ProtocolSpecificLinkRetrain        = 17

};


/*!
@typedef SCSIProtocolPowerState
@discussion
Typedef for SCSIProtocolPowerState, a 32-bit quantity.
*/
typedef UInt32 SCSIProtocolPowerState;

/*!
@enum SCSI Protocol Power States
@discussion
The list of SCSI Protocol Power States.
*/
enum
{
	/*!
	@constant kSCSIProtocolPowerStateOff
	Off power state.
	*/
	kSCSIProtocolPowerStateOff			= 0,

	/*!
	@constant kSCSIProtocolPowerStateOn
	On power state.
	*/
	kSCSIProtocolPowerStateOn			= 1
};


//-----------------------------------------------------------------------------
//	Class Declaration
//-----------------------------------------------------------------------------

/*!
@class IOSCSIProtocolInterface
@superclass IOService
@discussion
This class defines the public SCSI Protocol Layer API for any class that
provides Protocol services or needs to provide the Protocol Service API
for passing service requests to a Protocol Service driver.
*/
class __exported IOSCSIProtocolInterface : public IOService
{
	
	OSDeclareAbstractStructors ( IOSCSIProtocolInterface )
	
public:
	
	/*! @function init
	@abstract Initializes generic IOService data structures (expansion data, etc).
	*/
	virtual bool init( OSDictionary * dictionary = 0 ) APPLE_KEXT_OVERRIDE;

	/*!
	@function start
	@abstract During an IOService object's instantiation, starts the IOService object that has been selected to run on the provider.
	@discussion See IOService.h for details.
	@result <code>true</code> if the start was successful; <code>false</code> otherwise (which will cause the instance to be detached and usually freed).
	*/
	virtual bool	start ( IOService * provider ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function free
	@abstract Called to release all resources held by the object.
	@discussion Release all resources held by the object, then call super::free().  
	*/	
	virtual void	free ( void ) APPLE_KEXT_OVERRIDE;

	/*!
	@function willTerminate
	@abstract Passes a termination up the stack.
	@discussion Notification that a provider has been terminated, sent before recursing up the stack, in root-to-leaf order.
	@param provider The terminated provider of this object.
	@param options Options originally passed to terminate().
	@result <code>true</code>.
	*/
	virtual bool	willTerminate ( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;

	/*!
	@function GetUserClientExclusivityState
	@abstract Gets the current exclusivity state of the user client.
	@discussion The GetUserClientExclusivityState() method is called by the SCSITaskUserClient
	to determine if any user client is holding exclusive access at the current time. This is simply
	a preflight check and a return value of <code>false</code> does not guarantee that a subsequent
	call to SetUserClientExclusivityState() will return successfully.
	@result <code>true</code> if a user client is in exclusive control of the device, <code>false</code> otherwise.
	*/
	virtual bool 		GetUserClientExclusivityState ( void );
	
	/*!
	@function SetUserClientExclusivityState
	@abstract Sets the current exclusivity state of the user client.
	@discussion The SetUserClientExclusivityState() method is called by the SCSITaskUserClient
	to set the exclusive access mode.
	@param userClient The instance of SCSITaskUserClient for which to change exclusivity state.
	@param state Exclusivity state. <code>true</code> means exclusive access is desired, <code>false</code>
	means exclusive access is being released.
	@result A valid IOReturn code indicating success or the type of failure.
	*/
	virtual IOReturn	SetUserClientExclusivityState ( IOService * userClient, bool state );
	
	
	/*!
	@function initialPowerStateForDomainState
	@abstract Determines which power state a device is in, given the current power domain state.
	@discussion Power management calls this method once, when the driver is initializing power management.
	Subclasses should not need to override this method.
	@param flags Flags that describe the character of "domain power"; they represent the <code>outputPowerCharacter</code> field of a state in the power domain's power state array. 
	@result A state number. 
	*/
	virtual unsigned long	initialPowerStateForDomainState ( IOPMPowerFlags flags ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function setPowerState
	@abstract Requests a power managed driver to change the power state of its device.
	@discussion Requests a power managed driver to change the power state of its device. Most subclasses
	of IOSCSIProtocolInterface have class-specific mechanisms and should not override this routine.
	See IOSCSIProtocolServices.h, IOSCSIBlockCommandsDevice.h, IOSCSIReducedBlockCommandsDevice.h, and
	IOSCSIMultimediaCommandsDevice.h for more information about power management changes.
	Subclasses should not need to override this method.
	@param powerStateOrdinal The number in the power state array to which the drive is being instructed to change.
	@param whichDevice A pointer to the power management object which registered to manage power for this device.
	The whichDevice field is not pertinent to us since the driver is both the "policy maker" for the device,
	and the "policy implementor" for the device.
	@result See IOService.h for details.
	*/
	virtual IOReturn 	setPowerState ( unsigned long powerStateOrdinal, IOService * whichDevice ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function IsPowerManagementIntialized
	@abstract Called to determine if power management is initialized.
	@discussion Called to determine if power management is initialized.
	Subclasses should not need to override this method.
	@result <code>true</code> if power management has been initialized, <code>false</code> otherwise.
	*/
	virtual bool		IsPowerManagementIntialized ( void );
	
	/*!
	@function CheckPowerState
	@abstract Called by clients to ensure device is in correct power state before issuing I/O.
	@discussion Called by clients to ensure device is in correct power state before issuing I/O.
	If the device is not ready to handle such requests, it gives the driver a chance to block the
	thread until the device is ready. Subclasses should not need to override this method.
	*/
	virtual void 		CheckPowerState ( void );
	
	/*!
	@function ExecuteCommand
	@abstract Called to send a SCSITask and transport it across the physical wire(s) to the device.
	@discussion Called to send a SCSITask and transport it across the physical wire(s) to the device.
	Subclasses internal to IOSCSIArchitectureModelFamily will need to override this method. Third
	party subclasses should not need to override this method.
	@param request A valid SCSITaskIdentifier representing the task to transport across the wire(s).
	*/
	virtual void		ExecuteCommand ( SCSITaskIdentifier request ) = 0;
	
	/*!
	@function AbortCommand
	@abstract Obsolete. Do not use this method.
	@discussion Obsolete. Do not use this method.
	*/
	virtual SCSIServiceResponse		AbortCommand ( SCSITaskIdentifier request ) = 0;
	
	/*!
	@function IsProtocolServiceSupported
	@abstract This method is called to query for support of a protocol specific service feature.
	@discussion This method is called to query for support of a protocol specific service feature.
	Subclasses of IOSCSIProtocolServices should override this method.
	@param feature A valid SCSIProtocolFeature. See enums for SCSIProtocolFeature.
	@param serviceValue A pointer to a structure/value that is used in conjunction with the feature
	requested. See enums for SCSIProtocolFeature. NB: This parameter may be NULL for certain
	feature requests.
	@result <code>true</code> if the feature is supported, <code>false</code> otherwise.
	*/
	virtual bool		IsProtocolServiceSupported ( 
								SCSIProtocolFeature 	feature, 
								void * 					serviceValue ) = 0;
	
	/*!
	@function HandleProtocolServiceFeature
	@abstract This method is called to enact support of a protocol specific service feature.
	@discussion This method is called to enact support of a protocol specific service feature.
	Subclasses of IOSCSIProtocolServices should override this method.
	@param feature A valid SCSIProtocolFeature. See enums for SCSIProtocolFeature.
	@param serviceValue A pointer to a structure/value that is used in conjunction with the feature
	requested. See enums for SCSIProtocolFeature. NB: This parameter may be NULL for certain
	feature requests.
	@result <code>true</code> if the service feature request succeeded, <code>false</code> otherwise.
	*/
	virtual bool		HandleProtocolServiceFeature ( 
								SCSIProtocolFeature 	feature, 
								void * 					serviceValue ) = 0;
	
protected:
	
	// Reserve space for future expansion.
	struct IOSCSIProtocolInterfaceExpansionData
	{
		IOWorkLoop *	fWorkLoop;
        UInt64          fPowerStateChangeStartTime;
        UInt64          fLastCommandTime;
	};
	IOSCSIProtocolInterfaceExpansionData * fIOSCSIProtocolInterfaceReserved;
	
	// ------ Power Management Support ------
	
	thread_call_t		fPowerManagementThread;
	IOCommandGate *		fCommandGate;
	UInt32				fCurrentPowerState;
	UInt32				fProposedPowerState;
	bool				fPowerTransitionInProgress;
	bool				fPowerAckInProgress;
	bool				fPowerManagementInitialized;
	
	/*!
	@function GetCommandGate
	@abstract Accessor method to obtain the IOCommandGate.
	@discussion Accessor method to obtain the IOCommandGate.
	@result The IOCommandGate for this instance. May return NULL.
	*/
	IOCommandGate *		GetCommandGate ( void );
	
	/*!
	@function InitializePowerManagement
	@abstract This method is called to initialize power management.
	@discussion This method is called to initialize power management. It will call PMinit(), joinPMTree(),
	and makeUsable(). This method does not call registerPowerDriver().
	Subclasses may override this method to change the behavior (such as the number of power states).
	@param provider The power management provider (i.e. the provider to attach to in the PowerManagement
	tree). This may be a device that is not in the PM Tree itself, in which case, the IOService plane
	is traversed towards the root node in an effort to find a node in the PM Tree.
	*/
	virtual void		InitializePowerManagement ( IOService * provider );
	
	/*!
	@function GetInitialPowerState
	@abstract This method is called to obtain the initial power state of the device (usually the highest).
	@discussion This method is called to obtain the initial power state of the device (usually the highest).
	Subclasses must override this method.
	@result A power state ordinal.
	*/
	virtual UInt32		GetInitialPowerState ( void ) = 0;
	
	/*!
	@function finalize
	@abstract Finalizes the destruction of an IOService object.
	@discussion See IOService.h
	Subclasses may override this method, but should call super::finalize().
    @result <code>true</code>.
	*/
	virtual bool		finalize ( IOOptionBits options ) APPLE_KEXT_OVERRIDE;
	
	/*!
	@function sHandleSetPowerState
	@abstract The sHandleSetPowerState method is a static function used as C->C++ glue
	for going behind the command gate.
	@discussion The sHandleSetPowerState method is a static function used as C->C++ glue
	for going behind the command gate.
	@param self The 'this' pointer for the class.
	@param powerStateOrdinal The power state to which device shall be changed.
    @result A valid IOReturn code indicating success or failure.
	*/
	static IOReturn 	sHandleSetPowerState ( IOSCSIProtocolInterface * self, UInt32 powerStateOrdinal );
	
	/*!
	@function sGetPowerTransistionInProgress
	@abstract The sGetPowerTransistionInProgress method is a static function used as C->C++ glue
	for going behind the command gate.
	@discussion The sGetPowerTransistionInProgress method is a static function used as C->C++ glue
	for going behind the command gate.
	@param self The 'this' pointer for the class.
    @result <code>true</code> if a power state is in progress, otherwise <code>false</code>.
	*/
	static bool sGetPowerTransistionInProgress ( IOSCSIProtocolInterface * 	self );
	
	/*!
	@function HandleSetPowerState
	@abstract The HandleSetPowerState method is called by the glue code and is on the
	serialized side of the command gate.
	@discussion The HandleSetPowerState method is called by the glue code and is on the
	serialized side of the command gate. This allows us to touch any member
	variables as necessary without any multi-threading issues.
	Subclasses may override this method to change behavior. Third party subclasses
	should not need to override this method, but may.
	@param powerStateOrdinal The power state to which device shall be changed.
	*/
	virtual void		HandleSetPowerState ( UInt32 powerStateOrdinal );
	
	/*!
	@function sPowerManagement
	@abstract The sPowerManagement method is a static C-function which is called using
	mach's thread_call API. It guarantees us a thread of execution which is
	different than the power management thread and the workloop thread on which
	we can issue commands to the device synchronously or asynchronously without
	worrying about deadlocks. It calls through to HandlePowerChange, which is
	a state machine used to direct power management.
	@discussion The sPowerManagement method is a static C-function which is called using
	mach's thread_call API. It guarantees us a thread of execution which is
	different than the power management thread and the workloop thread on which
	we can issue commands to the device synchronously or asynchronously without
	worrying about deadlocks. It calls through to HandlePowerChange, which is
	a state machine used to direct power management.
	@param whichDevice The 'this' pointer.
	*/
	static void			sPowerManagement ( thread_call_param_t whichDevice );
	
	/*!
	@function HandlePowerChange
	@abstract The HandlePowerChange method is pure virtual and is left to each protocol or
	application layer driver to implement. It is guaranteed to be called on its
	own thread of execution and can make synchronous or asynchronous calls.
	@discussion The HandlePowerChange method is pure virtual and is left to each protocol or
	application layer driver to implement. It is guaranteed to be called on its
	own thread of execution and can make synchronous or asynchronous calls.
	Subclasses must override this method. Third party subclasses shouldn't need to override
	this method but can to alter the default behavior.
	*/
	virtual void		HandlePowerChange ( void ) = 0;
		
	/*!
	@function sHandleCheckPowerState
	@abstract The sHandleCheckPowerState method is a static function used as C->C++ glue
	for going behind the command gate.
	@discussion The sHandleCheckPowerState method is a static function used as C->C++ glue
	for going behind the command gate.
	@param self The 'this' pointer for the class.
	*/
	static void			sHandleCheckPowerState ( IOSCSIProtocolInterface * self );
	
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
	this method but can to alter the default behavior.
	*/
	virtual void		HandleCheckPowerState ( void ) = 0;
	
	/*!
	@function HandleCheckPowerState(UInt32 maxPowerState)
	@abstract The HandleCheckPowerState(UInt32 maxPowerState) method is called by
	the subclasses and is passed the maxPowerState number given to the power
	manager at initialization time. This guarantees the threads block until that
	power state has been achieved.
	@discussion The HandleCheckPowerState(UInt32 maxPowerState) method is called by
	the subclasses and is passed the maxPowerState number given to the power
	manager at initialization time. This guarantees the threads block until that
	power state has been achieved.
	@param maxPowerState The maximum power state in the power state array.
	*/
	void 				HandleCheckPowerState ( UInt32 maxPowerState );
	
	/*!
	@function TicklePowerManager(void)
	@abstract The TicklePowerManager(void) method is called by CheckPowerState and
	sends an activity tickle to the power manager so that the idle timer is
	reset.
	@discussion The TicklePowerManager(void) method is called by CheckPowerState and
	sends an activity tickle to the power manager so that the idle timer is
	reset.
	Subclasses must override this method. Third party subclasses shouldn't need to override
	this method but can to alter the default behavior.
	*/
	virtual void		TicklePowerManager ( void ) = 0;
	
	/*!
	@function TicklePowerManager(UInt32 maxPowerState)
	@abstract The TicklePowerManager(UInt32 maxPowerState) method is a convenience
	function which can be called by subclasses in TicklePowerManager (void)
	in order to tell the power manager to reset idle timer or bring the device
	into the requested state. It returns whatever is returned by activityTickle
	(true if device is in the requested state, false if it is not).
	@discussion The TicklePowerManager(UInt32 maxPowerState) method is a convenience
	function which can be called by subclasses in TicklePowerManager(void)
	in order to tell the power manager to reset idle timer or bring the device
	into the requested state. It returns whatever is returned by activityTickle
	(true if device is in the requested state, false if it is not).
	@param maxPowerState The maximum power state in the power state array.
	@result The result of the call to activityTickle(). See IOService.h for details.
	*/
	bool				TicklePowerManager ( UInt32 maxPowerState );
	
	// ------ User Client Support ------
	
	bool				fUserClientExclusiveControlled;
	IOService *			fUserClient;
	
	/*!
	@function sGetUserClientExclusivityState
	@abstract The sGetUserClientExclusivityState method is a static function used as C->C++ glue
	for going behind the command gate.
	@discussion The sGetUserClientExclusivityState method is a static function used as C->C++ glue
	for going behind the command gate.
	@param self The 'this' pointer for the class.
	@param state A pointer to a bool in which the state should be set.
	*/
	static void 	sGetUserClientExclusivityState ( IOSCSIProtocolInterface * self, bool * state );
	
	/*!
	@function sSetUserClientExclusivityState
	@abstract The sSetUserClientExclusivityState method is a static function used as C->C++ glue
	for going behind the command gate.
	@discussion The sSetUserClientExclusivityState method is a static function used as C->C++ glue
	for going behind the command gate.
	@param self The 'this' pointer for the class.
	@param result A pointer to an IOReturn for the resulting status.
	@param userClient The instance of SCSITaskUserClient for which to change exclusivity state.
	@param state A bool indicating the desired state to set.
	*/
	static void		sSetUserClientExclusivityState ( IOSCSIProtocolInterface * self, IOReturn * result, IOService * userClient, bool state );

	/*!
	@function HandleGetUserClientExclusivityState
	@abstract Gets the current exclusivity state of the user client.
	@discussion The HandleGetUserClientExclusivityState() method is called on the serialized side
	of the command gate to determine if any user client is holding exclusive access at the current
	time. See discussion for GetUserClientExclusivityState().
	Subclasses may override this method to alter default behavior. Third party subclasses should
	not need to override this method.
	@result <code>true</code> if a user client is in exclusive control of the device, <code>false</code> otherwise.
	*/
	virtual bool	HandleGetUserClientExclusivityState ( void );

	/*!
	@function HandleSetUserClientExclusivityState
	@abstract Sets the current exclusivity state of the user client.
	@discussion The HandleSetUserClientExclusivityState() method is called on the serialized side
	of the command gate to set the exclusive access mode.
	@param userClient The instance of SCSITaskUserClient for which to change exclusivity state.
	@param state Exclusivity state. <code>true</code> means exclusive access is desired, <code>false</code>
	means exclusive access is being released.
	@result A valid IOReturn code indicating success or the type of failure.
	*/
	virtual IOReturn	HandleSetUserClientExclusivityState ( IOService * userClient, bool state );

    /*!
    @function ResetPowerStateChangeTimer
    @abstract Resets the power state change timer counter.
    @discussion The ResetPowerStateChangeTimer() method is called every time a new power state transition begins.
    */
    void    ResetPowerStateChangeTimer ( void );

    /*!
    @function CheckForSufficientTimeForPMCommand
    @abstract Checks if there's time remaining before sending a command.
    @discussion The CheckForSufficientTimeForPMCommand() method is called whenever a command with a timeout
    is about to be sent. It checks if the time remaining in what was requested to the PM subsystem, whether it can accommodate
    the command's maximum round trip time i.e., the timeout specified.
    @param timeoutDuration The timeout specified for the command.
    @result <code>true</code> if the command's timeout can be accommodated before the PM request times out.
    */
    bool    CheckForSufficientTimeForPMCommand ( UInt32 timeoutDuration );

public:
	
	// ------- SCSI Architecture Model Task Management Functions ------

	OSMetaClassDeclareReservedUsed ( IOSCSIProtocolInterface, 1 );
	/*!
	@function AbortTask
	@abstract Aborts a task based on the Logical Unit and tagged task identifier.
	@discussion Aborts a task based on the Logical Unit and tagged task identifier.
	Subclasses must override this method. Third party subclasses should not need to override
	this method.
	@param theLogicalUnit This value should be zero unless the device driver is
	more complex and managing multiple Logical Units.
	@param theTag A valid SCSITaggedTaskIdentifier representing an outstanding SCSITask.
	@result A valid SCSIServiceResponse code.
	*/
	virtual SCSIServiceResponse		AbortTask ( UInt8 theLogicalUnit, SCSITaggedTaskIdentifier theTag ) = 0;
	
	OSMetaClassDeclareReservedUsed ( IOSCSIProtocolInterface, 2 );
	/*!
	@function AbortTaskSet
	@abstract Aborts a task set based on the Logical Unit.
	@discussion Aborts a task set based on the Logical Unit.
	Subclasses must override this method. Third party subclasses should not need to override
	this method.
	@param theLogicalUnit This value should be zero unless the device driver is
	more complex and managing multiple Logical Units.
	@result A valid SCSIServiceResponse code.
	*/
	virtual SCSIServiceResponse		AbortTaskSet ( UInt8 theLogicalUnit ) = 0;
	
	OSMetaClassDeclareReservedUsed ( IOSCSIProtocolInterface, 3 );
	/*!
	@function ClearACA
	@abstract Clears an Auto-Contingent Allegiance (ACA) for the specified Logical Unit.
	@discussion Clears an Auto-Contingent Allegiance (ACA) for the specified Logical Unit.
	Subclasses must override this method. Third party subclasses should not need to override
	this method.
	@param theLogicalUnit This value should be zero unless the device driver is
	more complex and managing multiple Logical Units.
	@result A valid SCSIServiceResponse code.
	*/
	virtual SCSIServiceResponse		ClearACA ( UInt8 theLogicalUnit ) = 0;
	
	OSMetaClassDeclareReservedUsed ( IOSCSIProtocolInterface, 4 );
	/*!
	@function ClearTaskSet
	@abstract Clears a task set for the specified Logical Unit.
	@discussion Clears a task set for the specified Logical Unit.
	Subclasses must override this method. Third party subclasses should not need to override
	this method.
	@param theLogicalUnit This value should be zero unless the device driver is
	more complex and managing multiple Logical Units.
	@result A valid SCSIServiceResponse code.
	*/
	virtual SCSIServiceResponse		ClearTaskSet ( UInt8 theLogicalUnit ) = 0;
	
	OSMetaClassDeclareReservedUsed ( IOSCSIProtocolInterface, 5 );
	/*!
	@function LogicalUnitReset
	@abstract Resets the specified Logical Unit.
	@discussion Resets the specified Logical Unit.
	Subclasses must override this method. Third party subclasses should not need to override
	this method.
	@param theLogicalUnit This value should be zero unless the device driver is
	more complex and managing multiple Logical Units.
	@result A valid SCSIServiceResponse code.
	*/
	virtual SCSIServiceResponse		LogicalUnitReset ( UInt8 theLogicalUnit ) = 0;
	
	OSMetaClassDeclareReservedUsed ( IOSCSIProtocolInterface, 6 );
	
	/*!
	@function TargetReset
	@abstract Resets the target device.
	@discussion Resets the target device.
	Subclasses must override this method. Third party subclasses should not need to override
	this method.
	@result A valid SCSIServiceResponse code.
	*/
	virtual SCSIServiceResponse		TargetReset ( void ) = 0;
	
private:
	
	// Method to show that the disk spinning up in spindump stacks
	void							__DISK_IS_ASLEEP__ ( void ) __attribute__((noinline));

#if !TARGET_OS_IPHONE
	// Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 7 );
    OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 8 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 9 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 10 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 11 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 12 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 13 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 14 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 15 );
	OSMetaClassDeclareReservedUnused ( IOSCSIProtocolInterface, 16 );
#endif /* !TARGET_OS_IPHONE */
	
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif	/* _IOKIT_IO_SCSI_PROTOCOL_INTERFACE_H_ */
