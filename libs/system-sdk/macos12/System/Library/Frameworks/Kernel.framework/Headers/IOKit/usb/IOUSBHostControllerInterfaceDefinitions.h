//
//  IOUSBHostControllerInterfaceDefinitions.h
//  IOUSBHostFamily
//
//  Copyright © 2020 Apple. All rights reserved.
//

#ifndef IOUSBHostControllerInterfaceDefinitions_h
#define IOUSBHostControllerInterfaceDefinitions_h

#include <IOKit/usb/AppleUSBDefinitions.h>
#include <IOKit/usb/IOUSBHostFamilyDefinitions.h>

#pragma pack(push, 1)

#pragma mark enumerations

typedef enum
{
    IOUSBHostCIUserClientVersion100 = 0
} IOUSBHostCIUserClientVersion;

/*!
 * @enum        IOUSBHostCIExceptionType
 * @brief       Exception specifier included as message argument to <code>kUSBHostMessageControllerException</code>
 * @discussion  The kernel driver sends a <code>kUSBHostMessageControllerException</code> message to its clients when a fatal problem has occurred, with a refCon of the IOUSBHostController instance and a <code>IOUSBHostCIExceptionType</code> in the message arguments.
 *              When any <code>kUSBHostMessageControllerException</code> message is received, the specified IOUSBHostControllerInterface must be destroyed.
 */
typedef enum
{
    IOUSBHostCIExceptionTypeUnknown = 0,                // Unknown failure
    IOUSBHostCIExceptionTypeCapabilitiesInvalid,        // Client provided an invalid IOUSBHostCIMessageTypeControllerCapabilities or IOUSBHostCIMessageTypePortCapabilities structure
    IOUSBHostCIExceptionTypeTerminated,                 // Kernel service is terminating
    IOUSBHostCIExceptionTypeCommandReadCollision,       // Client has issued too many concurrent asynchronous command reads
    IOUSBHostCIExceptionTypeCommandWriteFailed,         // Kernel was unable to send a command message to the client
    IOUSBHostCIExceptionTypeCommandTimeout,             // Command did not complete in the command timeout threshold specified in IOUSBHostCIMessageTypeControllerCapabilities
    IOUSBHostCIExceptionTypeCommandFailure,             // Critical command failed
    IOUSBHostCIExceptionTypeInterruptInvalid,           // Client sent an invalid interrupt message
    IOUSBHostCIExceptionTypeInterruptOverflow,          // Kernel was unable to keep up with interrupt messages sent by the client
    IOUSBHostCIExceptionTypeDoorbellReadCollision,      // Client has issued too many concurrent asynchronous doorbell reads
    IOUSBHostCIExceptionTypeDoorbellOverflow,           // Client was unable to keep up with doorbell messages sent by the kernel
    IOUSBHostCIExceptionTypeProtocolError,              // Client sent an invalid message in response to a command or transfer
    IOUSBHostCIExceptionTypeFrameUpdateError,           // Client sent a IOUSBHostCIMessageTypeFrameNumberUpdate and IOUSBHostCIMessageTypeFrameTimestampUpdate pair with invalid contents or an out-of-spec frame duration
} IOUSBHostCIExceptionType;

/*!
 * @enum        IOUSBHostCIMessageType
 * @brief       Values to be populated in IOUSBHostCIMessageControlType of an IOUSBHostCIMessage structure
 */
typedef enum
{
    IOUSBHostCIMessageTypeControllerCapabilities    = 0x0,
    IOUSBHostCIMessageTypePortCapabilities,
    
    IOUSBHostCIMessageTypePortEvent                 = 0x8,
    IOUSBHostCIMessageTypeFrameNumberUpdate,
    IOUSBHostCIMessageTypeFrameTimestampUpdate,

    IOUSBHostCIMessageTypeCommandMin                = 0x10,
    
    IOUSBHostCIMessageTypeControllerPowerOn         = 0x10,
    IOUSBHostCIMessageTypeControllerPowerOff,
    IOUSBHostCIMessageTypeControllerStart,
    IOUSBHostCIMessageTypeControllerPause,
    IOUSBHostCIMessageTypeControllerFrameNumber,
    
    IOUSBHostCIMessageTypePortPowerOn               = 0x18,
    IOUSBHostCIMessageTypePortPowerOff,
    IOUSBHostCIMessageTypePortResume,
    IOUSBHostCIMessageTypePortSuspend,
    IOUSBHostCIMessageTypePortReset,
    IOUSBHostCIMessageTypePortDisable,
    IOUSBHostCIMessageTypePortStatus,
    
    IOUSBHostCIMessageTypeDeviceCreate              = 0x20,
    IOUSBHostCIMessageTypeDeviceDestroy,
    IOUSBHostCIMessageTypeDeviceStart,
    IOUSBHostCIMessageTypeDevicePause,
    IOUSBHostCIMessageTypeDeviceUpdate,
    
    IOUSBHostCIMessageTypeEndpointCreate            = 0x28,
    IOUSBHostCIMessageTypeEndpointDestroy,
    IOUSBHostCIMessageTypeEndpoint_reserved_,
    IOUSBHostCIMessageTypeEndpointPause,
    IOUSBHostCIMessageTypeEndpointUpdate,
    IOUSBHostCIMessageTypeEndpointReset,
    IOUSBHostCIMessageTypeEndpointSetNextTransfer,

    IOUSBHostCIMessageTypeCommandMax                = 0x37,
    
    IOUSBHostCIMessageTypeSetupTransfer             = 0x38,
    IOUSBHostCIMessageTypeNormalTransfer,
    IOUSBHostCIMessageTypeStatusTransfer,
    IOUSBHostCIMessageTypeIsochronousTransfer,
    IOUSBHostCIMessageTypeLink,
    IOUSBHostCIMessageTypeTransferComplete,
} IOUSBHostCIMessageType;

/*!
 * @enum        IOUSBHostCIMessageStatus
 * @brief       Values to be populated in IOUSBHostCIMessageControlStatus of an IOUSBHostCIMessage structure
 */
typedef enum
{
    IOUSBHostCIMessageStatusReserved               = 0, // Reserved, do not use this value
    IOUSBHostCIMessageStatusSuccess,                    // The operation was successful
    IOUSBHostCIMessageStatusOffline,                    // The target is unreachable
    IOUSBHostCIMessageStatusNotPermitted,               // The request cannot be performed in the current state
    IOUSBHostCIMessageStatusBadArgument,                // The request did not pass validation checks
    IOUSBHostCIMessageStatusTimeout,                    // The request did not complete in the time expected
    IOUSBHostCIMessageStatusNoResources,                // The request cannot be completed due to resource limitations
    IOUSBHostCIMessageStatusEndpointStopped,            // The transfer generated an event due to an IOUSBHostCIMessageTypeEndpointPause command, not due to a completed transfer
    IOUSBHostCIMessageStatusProtocolError,              // The transfer completed due to a protocol error
    IOUSBHostCIMessageStatusTransactionError,           // The transfer completed due to a USB transaction error
    IOUSBHostCIMessageStatusOverrunError,               // The transfer completed due to a USB babble (overrun) error
    IOUSBHostCIMessageStatusStallError,                 // The transfer completed due to a STALL handshake from the device
    IOUSBHostCIMessageStatusMissedServiceError,         // The isochronous transfer was not able to be serviced in the specified frame number
    IOUSBHostCIMessageStatusError                       // Unspecified error
} IOUSBHostCIMessageStatus;

/*!
 * @enum        IOUSBHostCIDeviceSpeed
 * @brief       Values to represent a connected device speed, to be used in IOUSBHostCIMessages and IOUSBHostsCIPortStatus
 */
typedef enum
{
     IOUSBHostCIDeviceSpeedNone         = kIOUSBHostConnectionSpeedNone,
     IOUSBHostCIDeviceSpeedFull         = kIOUSBHostConnectionSpeedFull,
     IOUSBHostCIDeviceSpeedLow          = kIOUSBHostConnectionSpeedLow,
     IOUSBHostCIDeviceSpeedHigh         = kIOUSBHostConnectionSpeedHigh,
     IOUSBHostCIDeviceSpeedSuper        = kIOUSBHostConnectionSpeedSuper,
     IOUSBHostCIDeviceSpeedSuperPlus    = kIOUSBHostConnectionSpeedSuperPlus,
     IOUSBHostCIDeviceSpeedSuperPlusBy2 = kIOUSBHostConnectionSpeedSuperPlusBy2
} IOUSBHostCIDeviceSpeed;

/*!
 * @enum        IOUSBHostCILinkState
 * @brief       Values to represent a port's link state, to be used in IOUSBHostCIMessages and IOUSBHostCIPortStatus
 */
typedef enum
{
    IOUSBHostCILinkStateU0              = 0,
    IOUSBHostCILinkStateU1              = 1,
    IOUSBHostCILinkStateU2              = 2,
    IOUSBHostCILinkStateU3              = 3,
    IOUSBHostCILinkStateDisabled        = 4,
    IOUSBHostCILinkStateRxDetect        = 5,
    IOUSBHostCILinkStateInactive        = 6,
    IOUSBHostCILinkStatePolling         = 7,
    IOUSBHostCILinkStateRecovery        = 8,
    IOUSBHostCILinkStateReset           = 9,
    IOUSBHostCILinkStateCompliance      = 10,
    IOUSBHostCILinkStateTest            = 11,
    IOUSBHostCILinkStateResume          = 15,
    
} IOUSBHostCILinkState;

#pragma mark port status

typedef uint32_t IOUSBHostCIPortStatus;

/*!
 * @enum
 * @brief       Fields in IOUSBHostCIPortStatus
 * @discussion  IOUSBHostCIPortStatusPowered is set if the port is currently powered
 *              IOUSBHostCIPortStatusOvercurrent is set if the port is currently observing an overcurrent condition
 *              IOUSBHostCIPortStatusConnected is set if the port is currently observing a connected device
 *              IOUSBHostCIPortStatusLinkState is populated with a IOUSBHostCILinkState value
 *              IOUSBHostCIPortStatusSpeed is populated with a IOUSBHostCIDeviceSpeed value
 *              IOUSBHostCIPortStatusOvercurrentChange is set if a change to IOUSBHostCIPortStatusOvercurrent has been observed
 *              IOUSBHostCIPortStatusConnectChange is set if a change to IOUSBHostCIPortStatusConnected has been observed
 *              IOUSBHostCIPortStatusLinkStateChange is set if the link has independently transitioned from an enabled to disabled state.  See IOUSBHostCILinkStateEnabled(...) to determine if a particular link state is considered to be enabled or disabled.
 *              IOUSBHostCIPortStatusChangeMask is a mask combining all change bits
 */
enum
{
    IOUSBHostCIPortStatusPowered                    = IOUSBBit(0),
    IOUSBHostCIPortStatusOvercurrent                = IOUSBBit(1),
    IOUSBHostCIPortStatusConnected                  = IOUSBBit(2),
    IOUSBHostCIPortStatusLinkState                  = IOUSBBitRange64(4, 7),
    IOUSBHostCIPortStatusLinkStatePhase             = IOUSBBitRangePhase(4, 7),
    IOUSBHostCIPortStatusSpeed                      = IOUSBBitRange64(8, 10),
    IOUSBHostCIPortStatusSpeedPhase                 = IOUSBBitRangePhase(8, 10),
    IOUSBHostCIPortStatusOvercurrentChange          = IOUSBBit(17),
    IOUSBHostCIPortStatusConnectChange              = IOUSBBit(18),
    IOUSBHostCIPortStatusLinkStateChange            = IOUSBBit(20),
    
    IOUSBHostCIPortStatusChangeMask = IOUSBHostCIPortStatusLinkStateChange | IOUSBHostCIPortStatusConnectChange | IOUSBHostCIPortStatusOvercurrentChange,
};

#pragma mark doorbell messges

/*!
 * @typedef     IOUSBHostCIDoorbell
 * @brief       Structured message used to notify the client that the specified endpoint or stream has updated IOUSBHostCIMessage messages to process
 * @discussion  The kernel driver uses IOUSBHostCIDoorbell messages to transition an endpoint to the IOUSBHostCIEndpointStateActive state (if eligible), or to notify the client that additional valid IOUSBHostCIMessage structures have been appended to an already active queue.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIEndpointStateMachine associated with the endpoint selector in IOUSBHostCIDoorbellDeviceAddress and IOUSBHostCIDoorbellEndpointAddress of the command
 *              2. Utilize the IOUSBHostCIEndpointStateMachine processDoorbell interface to determine if the doorbell can be processed
 *              3. If successful and the endpoint is not already processing transfer messages, use the IOUSBHostCIEndpointStateMachine's currentTransferMessage property to begin processing transfer messages
 */
typedef uint32_t IOUSBHostCIDoorbell;

/*!
 * @enum
 * @brief       Fields in IOUSBHostCIDoorbell
 * @discussion  IOUSBHostCIDoorbellDeviceAddress is populated with the device address targeted by the IOUSBHostCIDoorbell
 *              IOUSBHostCIDoorbellEndpointAddress is populated with the endpoint address targeted by the IOUSBHostCIDoorbell
 *              IOUSBHostCIDoorbellStreamID is populated with the stream ID targeted by the IOUSBHostCIDoorbell
 */
enum
{
    IOUSBHostCIDoorbellDeviceAddress               = IOUSBBitRange(0, 7),
    IOUSBHostCIDoorbellDeviceAddressPhase          = IOUSBBitRangePhase(0, 7),
    IOUSBHostCIDoorbellEndpointAddress             = IOUSBBitRange(8, 15),
    IOUSBHostCIDoorbellEndpointAddressPhase        = IOUSBBitRangePhase(8, 15),
    IOUSBHostCIDoorbellStreamID                    = IOUSBBitRange(16, 31),
    IOUSBHostCIDoorbellStreamIDPhase               = IOUSBBitRangePhase(16, 31)
};

#pragma mark IOUSBHostCIMessage structures

/*!
 * @typedef     IOUSBHostCIMessage
 * @brief       Foundational IOUSBHostControllerInterface message structure used for commands, transfers, and interrupts
 * @discussion  IOUSBHostCIMessage structures are control structures passed between the IOUSBHostControllerInterface client and the kernel driver.  They are used to represent capabilities during initialization, commands and transfers sent from the kernel, and interrupt events sent to the kernel.
 */
typedef struct IOUSBHostCIMessage
{
    uint32_t control;
    uint32_t data0;
    uint64_t data1;
} IOUSBHostCIMessage;

/*!
 * @enum
 * @brief       Fields used in all IOUSBHostCIMessage structures
 * @discussion  IOUSBHostCIMessageControlType is populated in the control field with IOUSBHostCIMessageType.
 *              IOUSBHostCIMessageControlStatus is populated in the control field with IOUSBHostCIMessageStatus.
 *              IOUSBHostCIMessageControlNoResponse is set in the control field to incidate the recipient should not send a reply message.
 *              IOUSBHostCIMessageControlValid is set in the control field to indicate the message is fully initialized and ready for consumption by the recipient.
 */
enum
{
    IOUSBHostCIMessageControlType                  = IOUSBBitRange(0, 5),
    IOUSBHostCIMessageControlTypePhase             = IOUSBBitRangePhase(0, 5),
    IOUSBHostCIMessageControlStatus                = IOUSBBitRange(8, 11),
    IOUSBHostCIMessageControlStatusPhase           = IOUSBBitRangePhase(8, 11),
    IOUSBHostCIMessageControlNoResponse            = IOUSBBit(14),
    IOUSBHostCIMessageControlValid                 = IOUSBBit(15),
};

#pragma mark capability IOUSBHostCIMessages

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeControllerCapabilities
 * @discussion  IOUSBHostCICapabilitiesMessageControlPortCount is populated in the control field with the number of ports supported by this controller.
 *              IOUSBHostCICapabilitiesMessageData0CommandTimeoutThreshold is populated in the data0 field to indicate a timeout threshold for commands from the kernel.  Format is 2^n seconds.  A command timeout is a fatal error and will generate a IOUSBHostCIExceptionTypeCommandTimeout exception.
 *              IOUSBHostCICapabilitiesMessageData0ConnectionLatency is populated in the data0 field to indicate the time needed to pass a transfer structure and its associated data payload between the IOUSBHostControllerInterface and its target.  Format is 2^n milliseconds.
 */
enum
{
    IOUSBHostCICapabilitiesMessageControlPortCount                     = IOUSBBitRange(16, 19),
    IOUSBHostCICapabilitiesMessageControlPortCountPhase                = IOUSBBitRangePhase(16, 19),
    
    IOUSBHostCICapabilitiesMessageData0CommandTimeoutThreshold         = IOUSBBitRange(0, 1),
    IOUSBHostCICapabilitiesMessageData0CommandTimeoutThresholdPhase    = IOUSBBitRangePhase(0, 1),
    IOUSBHostCICapabilitiesMessageData0ConnectionLatency               = IOUSBBitRange(4, 7),
    IOUSBHostCICapabilitiesMessageData0ConnectionLatencyPhase          = IOUSBBitRangePhase(4, 7)
};

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypePortCapabilities
 * @discussion  IOUSBHostCIPortCapabilitiesMessageControlPortNumber is populated in the control field with the nonzero port number described by the structure.
 *              IOUSBHostCIPortCapabilitiesMessageControlInternalConnector is set in the control field to indicate the port is not user-accessible, such as for a captive device.
 *              IOUSBHostCIPortCapabilitiesMessageControlConnectorType is populated in the control field with the ACPI Connector Type
 *              IOUSBHostCIPortCapabilitiesMessageData0MaxPower is populated in the data0 field with the maximum power consumption allowed by the port, in 8mA @ 5V units.
 */
enum
{
    IOUSBHostCIPortCapabilitiesMessageControlPortNumber                 = IOUSBBitRange(16, 19),
    IOUSBHostCIPortCapabilitiesMessageControlPortNumberPhase            = IOUSBBitRangePhase(16, 19),
    IOUSBHostCIPortCapabilitiesMessageControlInternalConnector          = IOUSBBit(23),
    IOUSBHostCIPortCapabilitiesMessageControlConnectorType              = IOUSBBitRange(24, 31),
    IOUSBHostCIPortCapabilitiesMessageControlConnectorTypePhase         = IOUSBBitRangePhase(24, 31),

    IOUSBHostCIPortCapabilitiesMessageData0MaxPower                     = IOUSBBitRange(0, 7),
    IOUSBHostCIPortCapabilitiesMessageData0MaxPowerPhase                = IOUSBBitRangePhase(0, 7),
};

#pragma mark command IOUSBHostCIMessages

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType value in the range of IOUSBHostCIMessageTypeCommandMin to IOUSBHostCIMessageTypeCommandMax
 * @discussion  IOUSBHostCICommandMessageControlStatus is populated in command response messages, see IOUSBHostCIMessageControlStatus
 *              IOUSBHostCICommandMessageData0RootPort is populated in the data0 field with the nonzero port number if the command targets a port
 *              IOUSBHostCICommandMessageData0DeviceAddress is populated in the data0 field with the device address if the command targets a device, endpoint, or stream
 *              IOUSBHostCICommandMessageData0EndpointAddress is populated in the data0 field with the endpoint address if the command targets an endpoint or stream
 *              IOUSBHostCICommandMessageData0StreamID is populated in the data0 field with the stream ID if the command targets a stream
 */
enum
{
    IOUSBHostCICommandMessageControlStatus             = IOUSBHostCIMessageControlStatus,
    IOUSBHostCICommandMessageControlStatusPhase        = IOUSBHostCIMessageControlStatusPhase,
    
    IOUSBHostCICommandMessageData0RootPort             = IOUSBBitRange(0, 3),
    IOUSBHostCICommandMessageData0RootPortPhase        = IOUSBBitRangePhase(0, 3),
    
    IOUSBHostCICommandMessageData0DeviceAddress        = IOUSBBitRange(0, 7),
    IOUSBHostCICommandMessageData0DeviceAddressPhase   = IOUSBBitRangePhase(0, 7),
    IOUSBHostCICommandMessageData0EndpointAddress      = IOUSBBitRange(8, 15),
    IOUSBHostCICommandMessageData0EndpointAddressPhase = IOUSBBitRangePhase(8, 15),
    IOUSBHostCICommandMessageData0StreamID             = IOUSBBitRange(16, 31),
    IOUSBHostCICommandMessageData0StreamIDPhase        = IOUSBBitRangePhase(16, 31)
};

#pragma mark controller states and IOUSBHostCIMessages

/*!
 * @enum        IOUSBHostCIControllerState
 * @brief       Host controller state managed by IOUSBHostCIControllerStateMachine
 * @discussion  IOUSBHostCIControllerStateOff is the initial state of a newly created IOUSBHostCIControllerStateMachine instance, and represents a completely quiesced controller.  All root ports must be in the IOUSBHostCIPortStateOff state.
 *              IOUSBHostCIControllerStatePaused represents a controller that is idle.  In this state the controller is not processing IO requests, but can detect port changes such as connect events or remote wakes.  Root ports must not be in the IOUSBHostCIPortStateActive state.
 *              IOUSBHostCIControllerStateActive represents a controller that is fully functional and able to service IO requests.  Root ports may be in the IOUSBHostCIPortStateActive state.
 */
typedef enum
{
    IOUSBHostCIControllerStateOff       = 0,
    IOUSBHostCIControllerStatePaused,
    IOUSBHostCIControllerStateActive
} IOUSBHostCIControllerState;

/*!
 * @brief       IOUSBHostCIMessageTypeControllerPowerOn command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeControllerPowerOn command is valid for an IOUSBHostCIControllerStateMachine in the IOUSBHostCIControllerStateOff state.
 *              When received, client should perform the following actions:
 *              1. Utilize the IOUSBHostCIControllerStateMachine inspectCommand interface to determine if the command should be handled
 *              2. If successful, initialize the USB controller target and enable detection of port events
 *              3. Utilize the IOUSBHostCIControllerStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeControllerPowerOff command handling exepctations
 * @discussion  An IOUSBHostCIMessageTypeControllerPowerOff command is valid for an IOUSBHostCIControllerStateMachine in the IOUSBHostCIControllerStatePaused or IOUSBHostCIControllerStateActive states.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostCIControllerStateMachine inspectCommand interface to determine if the command should be handled
 *              2. If successful, confirm that all downstream IOUSBHostCIPortStateMachine instances are in the IOUSBHostCIPortStateOff state
 *              3. If port state machine requirements are satisfied, disable detection of port events and power down the USB controller target
 *              4. Utilize the IOUSBHostCIControllerStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeControllerStart command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeControllerStart command is valid for an IOUSBHostCIControllerStateMachine in the IOUSBHostCIControllerStatePaused state.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostCIControllerStateMachine inspectCommand interface to determine if the command should be handled
 *              2. If successful, enable IO transfers on the USB controller target
 *              3. Utilize the IOUSBHostCIControllerStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeControllerPause command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeControllerPause command is valid for an IOUSBHostCIControllerStateMachine in the IOUSBHostCIMessageTypeControllerActive state.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostCIControllerStateMachine inspectCommand interface to determine if the command should be handled
 *              2. If successful, disable IO transfers on the USB controller target
 *              3. Utilize the IOUSBHostCIControllerStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeControllerFrameNumber command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeControllerFrameNumber command is valid for an IOUSBHostCIControllerStateMachine in the IOUSBHostCIControllerStateActive state.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostCIControllerStateMachine inspectCommand interface to determine if the command should be handled
 *              2. If successful, utilize the IOUSBHostCIControllerStateMachine enqueueUpdatedFrame interface to post frame update messages to the kernel driver
 *              3. Utilize the IOUSBHostCIControllerStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

#pragma mark port states and IOUSBHostCIMessages

/*!
 * @enum        IOUSBHostCIPortState
 * @brief       Port state managed by IOUSBHostCIPortStateMachine
 * @discussion  IOUSBHostCIPortStateOff is the initial state of a newly created IOUSBHostCIPortStateMachine and represents an unpowered port which is unable to detect events such as connections.  Downstream devices, if any, must be in the IOUSBHostCIDeviceStateDestroyed state.
 *              IOUSBHostCIPortStatePowered represents a powered port which is able to detect events such as connections.  Downstream devices, if any, must be in the IOUSBHostCIDeviceStateDestroyed state.
 *              IOUSBHostCIPortStateSuspended represents a port with a downstream device that is in a low-power state such as U3 or L2.  The port is able to detect events such as remote wakes, and is not actively transmitting data.  Downstream devices must be in the IOUSBHostCIDeviceStatePaused state.
 *              IOUSBHostCIPortStateActive represents a port with a downstream device that is able to immediately transfer data.  The downstream device may be in the IOUSBHostCIDeviceStateActive state.
 */
typedef enum
{
    IOUSBHostCIPortStateOff             = 0,
    IOUSBHostCIPortStatePowered,
    IOUSBHostCIPortStateSuspended,
    IOUSBHostCIPortStateActive
} IOUSBHostCIPortState;

/*!
 * @brief       IOUSBHostCIMessageTypePortEvent message expectations
 * @discussion  An IOUSBHostCIMessageTypePortEvent message is sent by the client to the kernel driver to notify it that a change has occurred on the specified port.
 *              The kernel driver will respond with port commands to interrogate the port for its current state and handle any new conditions.
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypePortEvent
 * @discussion  IOUSBHostCIPortEventMessageData0PortNumber is populated in the data0 field with the nonzero port number of the port that needs to process events.
 */
enum
{
    IOUSBHostCIPortEventMessageData0PortNumber         = IOUSBBitRange(0, 3),
    IOUSBHostCIPortEventMessageData0PortNumberPhase    = IOUSBBitRangePhase(0, 3)
};

/*!
 * @enum
 * @brief       Fields used in some IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType in the range of IOUSBHostCIMessageTypePortPowerOn to IOUSBHostCIMessageTypePortStatus
 * @discussion  IOUSBHostCIPortStatusCommandData1Powered is set in the data1 field of command responses if the port is currently powered
 *              IOUSBHostCIPortStatusCommandData1Overcurrent is set in the data1 field of command responses if the port is currently observing an overcurrent condition
 *              IOUSBHostCIPortStatusCommandData1Connected is set in the data1 field of command responses if the port is currently observing a connected device
 *              IOUSBHostCIPortStatusCommandData1LinkState is populated in the data1 field of command responses with the currently observed IOUSBHostCILinkState
 *              IOUSBHostCIPortStatusCommandData1Speed is populated in the data1 field of command responses with the currently observed IOUSBHostCIDeviceSpeed
 *              IOUSBHostCIPortStatusCommandData1OvercurrentChange is set in the data1 field of commands if the bit should be cleared on subsequent command responses.  It is set in the data1 field of command responses if IOUSBHostCIPortStatusCommandData1Overcurrent has changed.
 *              IOUSBHostCIPortStatusCommandData1ConnectChange is set in the data1 field of commands if the bit should be cleared on subsequent command responses.  It is set in the data1 field of command responses if IOUSBHostCIPortStatusCommandData1Connected has changed.
 *              IOUSBHostCIPortStatusCommandData1LinkStateChange is set in the data1 field of commands if the bit should be cleared on subsequent command responses.  It is set in the data1 field of command responses if the link has independently transitioned from an enabled to disabled state.  See IOUSBHostCILinkStateEnabled(...) to determine if a particular link state is considered to be enabled or disabled.
 *              IOUSBHostCIPortStatusCommandData1ChangeMask is a mask combining all change bits from the data1 field.
 */
enum
{
    IOUSBHostCIPortStatusCommandData1Powered                               = IOUSBHostCIPortStatusPowered,
    IOUSBHostCIPortStatusCommandData1Overcurrent                           = IOUSBHostCIPortStatusOvercurrent,
    IOUSBHostCIPortStatusCommandData1Connected                             = IOUSBHostCIPortStatusConnected,
    IOUSBHostCIPortStatusCommandData1LinkState                             = IOUSBHostCIPortStatusLinkState,
    IOUSBHostCIPortStatusCommandData1LinkStatePhase                        = IOUSBHostCIPortStatusLinkStatePhase,
    IOUSBHostCIPortStatusCommandData1Speed                                 = IOUSBHostCIPortStatusSpeed,
    IOUSBHostCIPortStatusCommandData1SpeedPhase                            = IOUSBHostCIPortStatusSpeedPhase,
    IOUSBHostCIPortStatusCommandData1OvercurrentChange                     = IOUSBHostCIPortStatusOvercurrentChange,
    IOUSBHostCIPortStatusCommandData1ConnectChange                         = IOUSBHostCIPortStatusConnectChange,
    IOUSBHostCIPortStatusCommandData1LinkStateChange                       = IOUSBHostCIPortStatusLinkStateChange,
    
    IOUSBHostCIPortStatusCommandData1ChangeMask = IOUSBHostCIPortStatusChangeMask,
};

/*!
 * @brief       IOUSBHostCIMessageTypePortPowerOn command handling expectations
 * @discussion  An IOUSBHostCIMessageTypePortPowerOn command is valid for an IOUSBHostCIPortStateMachine in the IOUSBHostCIPortStateOff state.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostControllerInterface getPortStateMachineForCommand interface to locate an IOUSBHostCIPortStateMachine instance for the command
 *              2. Utilize the IOUSBHostCIPortStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, power on the specified port hardware and enable it to accept connections
 *              4. Utilize the IOUSBHostCIPortStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypePortPowerOff command handling expectations
 * @discussion  An IOUSBHostCIMessageTypePortPowerOff command is valid for an IOUSBHostCIPortStateMachine in any state except IOUSBHostCIPortStateOff.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostControllerInterface getPortStateMachineForCommand interface to locate an IOUSBHostCIPortStateMachine instance for the command
 *              2. Utilize the IOUSBHostCIPortStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, confirm that all downstream IOUSBHostCIDeviceStateMachine instances are in the IOUSBHostCIDeviceStateDestroyed state
 *              4. If device state machine requirements are satisfied, power off the specified port hardware and disable connection detection for that port
 *              5. Utilize the IOUSBHostCIPortStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypePortResume command handling expectations
 * @discussion  An IOUSBHostCIMessageTypePortResume command is valid for an IOUSBHostCIPortStateMachine in the IOUSBHostCIPortStateSuspended state.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostControllerInterface getPortStateMachineForCommand interface to locate an IOUSBHostCIPortStateMachine instance for the command
 *              2. Utilize the IOUSBHostCIPortStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, reenable traffic to the downstream device (e.g. U0 or L0) and call IOUSBHostCIPortStateMachine updateLinkState interface to report current link conditions
 *              5. Utilize the IOUSBHostCIPortStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypePortSuspend command handling expectations
 * @discussion  An IOUSBHostCIMessageTypePortSuspend command is valid for an IOUSBHostCIPortStateMachine in the IOUSBHostCIPortStateActive state.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostControllerInterface getPortStateMachineForCommand interface to locate an IOUSBHostCIPortStateMachine instance for the command
 *              2. Utilize the IOUSBHostCIPortStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, put the link into a low-power state (e.g. U3 or L2) and call IOUSBHostCIPortStateMachine updateLinkState interface to report current link conditions
 *              4. Utilize the IOUSBHostCIPortStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypePortReset command handling expectations
 * @discussion  An IOUSBHostCIMessageTypePortReset command is valid for an IOUSBHostCIPortStateMachine in any state except IOUSBHostCIPortStateOff.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostControllerInterface getPortStateMachineForCommand interface to locate an IOUSBHostCIPortStateMachine instance for the command
 *              2. Utilize the IOUSBHostCIPortStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, issue a bus-level reset on the port and call IOUSBHostCIPortStateMachine updateLinkState interface to report current link conditions
 *              4. Utilize the IOUSBHostCIPortStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypePortDisable command handling expectations
 * @discussion  An IOUSBHostCIMessageTypePortDisable command is valid for an IOUSBHostCIPortStateMachine in any state except IOUSBHostCIPortStateOff.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostControllerInterface getPortStateMachineForCommand interface to locate an IOUSBHostCIPortStateMachine instance for the command
 *              2. Utilize the IOUSBHostCIPortStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, disable traffic propogation to downstream devices and call IOUSBHostCIPortStateMachine updateLinkState interface to report current link conditions
 *              4. Utilize the IOUSBHostCIPortStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypePortStatus command handling expectations
 * @discussion  An IOUSBHostCIMessageTypePortStatus command is valid for an IOUSBHostCIPortStateMachine in any state.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostControllerInterface getPortStateMachineForCommand interface to locate an IOUSBHostCIPortStateMachine instance for the command
 *              2. Utilize the IOUSBHostCIPortStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, ensure current link conditions have been captured by callin IOUSBHostCIPortStateMachine updateLinkState
 *              4. Utilize the IOUSBHostCIPortStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

#pragma mark device states and IOUSBHostCIMessages

/*!
 * @enum        IOUSBHostCIDeviceState
 * @brief       Device state managed by IOUSBHostCIDeviceStateMachine
 * @discussion  IOUSBHostCIDeviceStateDestroyed represents a device that is no longer usable, and whose resources will be destroyed and freed
 *              IOUSBHostCIDeviceStatePaused represents a device that may have IO requests enqueued, but they are not active on the bus.  The state is closely correlated to IOUSBHostCIPortStateSuspended, and endpoints associated with this device may not be in the IOUSBHostCIEndpointStateActive state.
 *              IOUSBHostCIDeviceStateActive is the initial sate of a newly created IOUSBHsotCIDeviceStateMachine, and represents a device that can be processing IO requests.  Endpoints associated with this device may be in the IOUSBHostCIEndpointStateActive state.
 */
typedef enum
{
    IOUSBHostCIDeviceStateDestroyed = 0,
    IOUSBHostCIDeviceStatePaused,
    IOUSBHostCIDeviceStateActive
} IOUSBHostCIDeviceState;

/*!
 * @brief       IOUSBHostCIMessageTypeDeviceCreate command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeDeviceCreate command is used to prompt creation of an IOUSBHostCIDeviceStateMachine instance, to be tracked by the client.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostCIDeviceStateMachine initWithInterface interface, using the command as a parameter
 *              2. If successful, the client must attempt to allocate and assign a USB device address to the device at the specified route
 *              3. Utilize the IOUSBHostCIDeviceStateMachine respondToCommand interface to submit a command response to the kernel driver, or the IOUSBHostControllerInterface enqueueInterrupt interface if the IOUSBHostCIDeviceStateMachine was not created
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeDeviceCreate
 * @discussion  IOUSBHostCIDeviceCreateCommandData0RootPort is populated in the data0 field of the command with the nonzero root port number for which to create a device
 *              IOUSBHostCIDeviceCreateCommandData0Route is populated in the data0 field of the command with the USB route string (see USB 3.2 § 8.9)
 *              IOUSBHostCIDeviceCreateCommandData1DeviceAddress is populated in the data1 field of the command response with the USB address assigned to the newly created device
 */
enum
{
    IOUSBHostCIDeviceCreateCommandData0RootPort                            = IOUSBBitRange(0, 3),
    IOUSBHostCIDeviceCreateCommandData0RootPortPhase                       = IOUSBBitRangePhase(0, 3),
    IOUSBHostCIDeviceCreateCommandData0Route                               = IOUSBBitRange(4, 23),
    IOUSBHostCIDeviceCreateCommandData0RoutePhase                          = IOUSBBitRangePhase(4, 23),
    IOUSBHostCIDeviceCreateCommandData1DeviceAddress                       = IOUSBBitRange64(0, 7),
    IOUSBHostCIDeviceCreateCommandData1DeviceAddressPhase                  = IOUSBBitRangePhase(0, 7)
};

/*!
 * @brief       IOUSBHostCIMessageTypeDeviceDestroy command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeDeviceDestroy command is valid for an IOUSBHostCIDeviceStateMachine in any state except IOUSBHostCIDeviceStateDestroyed.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIDeviceStateMachine associated with the device address selector in IOUSBHostCICommandMessageData0DeviceAddress of the command
 *              2. Utilize the IOUSBHostCIDeviceStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, confirm all endpoints encapsulated by the device are in the IOUSBHostCIEndpointStateDestroyed state.
 *              4. If endpoint state requirements have been met, release resources associated with the device instance
 *              5. Utilize the IOUSBHostCIDeviceStateMachine respondToCommand interface to submit a command response to the kernel driver
 *              6. Destroy the IOUSBHostCIDeviceStateMachine instance
 */

/*!
 * @brief       IOUSBHostCIMessageTypeDeviceStart command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeDeviceStart command is valid for an IOUSBHostCIDeviceStateMachine in the IOUSBHostCIDeviceStatePaused state.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIDeviceStateMachine associated with the device address selector in IOUSBHostCICommandMessageData0DeviceAddress of the command
 *              2. Utilize the IOUSBHostCIDeviceStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, adjust necessary internal state to permit IO transfers to the device
 *              4. Utilize the IOUSBHostCIDeviceStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeDevicePause command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeDevicePause command is valid for an IOUSBHostCIDeviceStateMachine in the IOUSBHostCIDeviceStateActive state.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIDeviceStateMachine associated with the device address selector in IOUSBHostCICommandMessageData0DeviceAddress of the command
 *              2. Utilize the IOUSBHostCIDeviceStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, adjust necessary internal state to quiesce IO transfers to the device
 *              4. Utilize the IOUSBHostCIDeviceStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeDeviceUpdate command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeDeviceUpdate command is valid for an IOUSBHostCIDeviceStateMachine in any state except IOUSBHostCIDeviceStateDestroyed.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIDeviceStateMachine associated with the device address selector in IOUSBHostCICommandMessageData0DeviceAddress of the command
 *              2. Utilize the IOUSBHostCIDeviceStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, inspect the USB descriptor at the address provided in IOUSBHostCIDeviceUpdateCommandData1DescriptorAddress and make appropriate adjustments to the controller configuration
 *              4. Utilize the IOUSBHostCIDeviceStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeDeviceUpdate
 * @discussion  IOUSBHostCIDeviceUpdateCommandData1DescriptorAddress is populated in the data1 field of the command with the virtual address of a USB descriptor (see USB 3.2 § 9.5) applicable to the device.
 */
enum
{
    IOUSBHostCIDeviceUpdateCommandData1DescriptorAddress                    = IOUSBBitRange(0, 63),
    IOUSBHostCIDeviceUpdateCommandData1DescriptorAddressPhase               = IOUSBBitRangePhase(0, 63)
};

#pragma mark endpoint states and IOUSBHostCIMessages

/*!
 * @enum        IOUSBHostCIEndpointState
 * @brief       Endpoint state managed by IOUSBHostCIEndpointStateMachine
 * @discussion  IOUSBHostCIEndpointStateDestroyed represents an endpoint that is no longer usable, and whose resources will be destroyed and freed.
 *              IOUSBHostCIEndpointStateHalted represents an endpoint that has encountered an IO error.  The client must not access or modify transfer structures or IO buffers for an endpoint in this state.
 *              IOUSBHostCIEndpointStatePaused is the initial state of a newly created IOUSBHostCIEndpointStateMachine, and represents an endpoint that is not currently servicing IO requests.  The client must not access or modify transfer structures or IO buffers for an endpoint in this state.
 *              IOUSBHostCIEndpointStateActive represents an endpoint that is currently servicing an IO request, or is idle after successfully servicing its queue of IO requests.  In this state the client may access transfer structures and IO buffers.
 */
typedef enum
{
    IOUSBHostCIEndpointStateDestroyed = 0,
    IOUSBHostCIEndpointStateHalted,
    IOUSBHostCIEndpointStatePaused,
    IOUSBHostCIEndpointStateActive,
} IOUSBHostCIEndpointState;

/*!
 * @brief       IOUSBHostCIMessageTypeEndpointCreate command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeEndpointCreate command is used to prompt creation of an IOUSBHostCIEndpointStateMachine instance, to be tracked by the client.
 *              When received, the client should perform the following actions:
 *              1. Utilize the IOUSBHostCIEndpointStateMachine initWithInterface interface, using the command as a parameter
 *              2. If successful, inspect the USB descriptor at the address provided in IOUSBHostCIEndpointCreateCommandData1Descriptor and allocate appropriate resources for the specified endpoint
 *              3. Utilize the IOUSBHostCIEndpointStateMachine respondToCommand interface to submit a command response to the kernel driver, or the IOUSBHostControllerInterface enqueueInterrupt interface if the IOUSBHostCIEndpointStateMachine was not created
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeEndpointCreate
 * @discussion  IOUSBHostCIEndpointCreateCommandData1Descriptor is populated in the data1 field of the command with the virtual address of the USB endpoint descriptor(s) (see USB 3.2 § 9.5) describing the endpoint.
 */
enum
{
    IOUSBHostCIEndpointCreateCommandData1Descriptor                        = IOUSBBitRange64(0, 63),
    IOUSBHostCIEndpointCreateCommandData1DescriptorPhase                   = IOUSBBitRangePhase(0, 63)
};

/*!
 * @brief       IOUSBHostCIMessageTypeEndpointDestroy command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeEndpointDestroy command is valid for an IOUSBHostCIEndpointStateMachine in any state except IOUSBHostCIEndpointStateDestroyed.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIEndpointStateMachine associated with the endpoint selector in IOUSBHostCICommandMessageData0DeviceAddress and IOUSBHostCICommandMessageData0EndpointAddress of the command
 *              2. Utilize the IOUSBHostCIEndpointStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, quiesce activity on the specified endpoint and abort any remaining transfers (including submitting transfer completion messages to the kernel driver)
 *              4. Utilize the IOUSBHostCIEndpointStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeEndpointPause command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeEndpointPause command is valid for an IOUSBHostCIEndpointStateMachine in the IOUSBHostCIEndpointStateActive state.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIEndpointStateMachine associated with the endpoint selector in IOUSBHostCICommandMessageData0DeviceAddress and IOUSBHostCICommandMessageData0EndpointAddress of the command
 *              2. Utilize the IOUSBHostCIEndpointStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, quiesce activity on the specified endpoint, submit any generated transfer completion messages to the kernel driver, and stop accessing transfer structures or data buffers for the endpoint
 *              4. Utilize the IOUSBHostCIEndpointStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @brief       IOUSBHostCIMessageTypeEndpointUpdate command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeEndpointUpdate command is valid for an IOUSBHostCIEndpointStateMachine in the IOUSBHostCIEndpointStatePaused state.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIEndpointStateMachine associated with the endpoint selector in IOUSBHostCICommandMessageData0DeviceAddress and IOUSBHostCICommandMessageData0EndpointAddress of the command
 *              2. Utilize the IOUSBHostCIEndpointStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, inspect the USB descriptor at the address provided in IOUSBHostCIEndpointUpdateCommandData1Descriptor and make appropriate adjustments to the endpoint configuration
 *              4. Utilize the IOUSBHostCIEndpoointStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeEndpointUpdate
 * @discussion  IOUSBHostCIEndpointUpdateCommandData1Descriptor is populated in the data1 field of the command with the virtual address of the USB endpoint descriptors (see USB 3.2 § 9.5) describing updated endpoint capabilities.
 */
enum
{
    IOUSBHostCIEndpointUpdateCommandData1Descriptor                        = IOUSBBitRange64(0, 63),
    IOUSBHostCIEndpointUpdateCommandData1DescriptorPhase                   = IOUSBBitRangePhase(0, 63)
};

/*!
 * @brief       IOUSBHostCIMessageTypeEndpointReset command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeEndpointReset command is valid for an IOUSBHostCIEndpointStateMachine in the IOUSBHostCIEndpointStateHalted state.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIEndpointStateMachine associated with the endpoint selector in IOUSBHostCICommandMessageData0DeviceAddress and IOUSBHostCICommandMessageData0EndpointAddress of the command
 *              2. Utilize the IOUSBHostCIEndpointStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, perform necessary actions to clear a Halted condition on the endpoint and return it to the IOUSBHostCIEndpointStatePaused state
 *              4. Utilize the IOUSBHostCIEndpointStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeEndpointReset
 * @discussion  IOUSBHostCIEndpointResetCommandData1ClearState is set in the data1 field of the command to indicate the endpoint state (e.g. data toggle) should be reset as part of processing the command.
 */
enum
{
    IOUSBHostCIEndpointResetCommandData1ClearState                         = IOUSBBit(0)
};

/*!
 * @brief       IOUSBHostCIMessageTypeEndpointSetNextTransfer command handling expectations
 * @discussion  An IOUSBHostCIMessageTypeEndpointSetNextTransfer command is valid for an IOUSBHostCIEndpointStateMachine in the IOUSBHostCIEndpointStatePaused state.
 *              When received, the client should perform the following actions:
 *              1. Locate the iOUSBHostCIEndpointStateMachine associated with the endpoint selector in IOUSBHostCICommandMessageData0DeviceAddress and IOUSBHostCICommandMessageData0EndpointAddress of the command
 *              2. Utilize the IOUSBHostCIEndpointStateMachine inspectCommand interface to determine if the command should be processed
 *              3. If successful, update internal state as needed to force a refresh of the transfer structure pointer on the next doorbell for the endpoint
 *              4. Utilize the IOUSBHostCIEndpointStateMachine respondToCommand interface to submit a command response to the kernel driver
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeEndpointSetNextTransfer
 * @discussion  IOUSBHostCIEndpointSetNextTransferCommandData1Address is populated in the data1 field of the command with the virtual address of the next IOUSBHostCIMessage transfer message to process.
 */
enum
{
    IOUSBHostCIEndpointSetNextTransferCommandData1Address                  = IOUSBBitRange64(0, 63),
    IOUSBHostCIEndpointSetNextTransferCommandData1AddressPhase             = IOUSBBitRangePhase(0, 63)
};

#pragma mark transfer IOUSBHostCIMessages

/*!
 * @brief       Transfer IOUSBHostCIMessage expectations
 * @discussion  Data movement is accomplished via arrays of IOUSBHostCIMessage structures with a IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeSetupTransfer, IOUSBHostCIMessageTypeNormalTransfer,
 *              IOUSBHostCIMessageTypeStatusTransfer, IOUSBHostCIMessageTypeIsochronousTransfer, and IOUSBHostCIMessageTypeLink.  Control endpoints support IOUSBHostCIMessageTypeSetupTransfer and
 *              IOUSBHostCIMessageTypeStatusTransfer messages, and use IOUSBHostCIMessageTypeNormalTransfer to represent the data phase of transfers.  Isochronous endpoints use
 *              IOUSBHostCIMessageTypeIsochronousTransfer messages.  Bulk and interrupt endpoints use IOUSBHostCIMessageTypeNormalTransfer messages.
 *
 *              The last message in an array has the type IOUSBHostCIMessageTypeLink, which when valid points to the first entry in the next array of messages.  IOUSBHostCIEndpointStateMachine aids in parsing these structures
 *              and following valid links.
 *
 *              When a doorbell message arrives for the endpoint, clients should use the IOUSBHostCIEndpoointStateMachine processDoorbell interface and currentTransferMessage property to check for valid transfer messages to
 *              process.  While parsing transfer messages, those with the IOUSBHostCIMessageControlNoResponse bit cleared should use the IOUSBHostCIEndpointStateMachine enqueueTransferCompletionForMessage
 *              interface to deliver a IOUSBHostCIMessageTypeTransferComplete message to the kernel driver.  If a control, bulk, or interrupt endpoint transfer message is being completed with an unsuccessful status, the endpoint
 *              will transition to the IOUSBHostCIEndpointStateHalted state, and the client must first ensure it has quiesced activity on the endpoint and is no longer accessing transfer structures or data buffers for that endpoint.
 */

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeSetupTransfer
 * @discussion  See USB 3.2 § 9.3 for a description of the control request fields.
 *              IOUSBHostCISetupTransferData1bmRequestType is populated in the data1 field with the control request's bmRequestType
 *              IOUSBHostCISetupTransferData1bRequest is populated in the data1 field with the control request's bRequest
 *              IOUSBHostCISetupTransferData1wValue is populated in the data1 field with the control request's wValue
 *              IOUSBHostCISetupTransferData1wIndex is populated in the data1 field with the control request's wIndex
 *              IOUSBHostCISetupTransferData1wLength is populated in the data1 field with the control request's wLength
 */
enum
{
    IOUSBHostCISetupTransferData1bmRequestType                              = IOUSBBitRange64(0, 7),
    IOUSBHostCISetupTransferData1bmRequestTypePhase                         = IOUSBBitRangePhase(0, 7),
    IOUSBHostCISetupTransferData1bRequest                                   = IOUSBBitRange64(8, 15),
    IOUSBHostCISetupTransferData1bRequestPhase                              = IOUSBBitRangePhase(8, 15),
    IOUSBHostCISetupTransferData1wValue                                     = IOUSBBitRange64(16, 31),
    IOUSBHostCISetupTransferData1wValuePhase                                = IOUSBBitRangePhase(16, 31),
    IOUSBHostCISetupTransferData1wIndex                                     = IOUSBBitRange64(32, 47),
    IOUSBHostCISetupTransferData1wIndexPhase                                = IOUSBBitRangePhase(32, 47),
    IOUSBHostCISetupTransferData1wLength                                    = IOUSBBitRange64(48, 63),
    IOUSBHostCISetupTransferData1wLengthPhase                               = IOUSBBitRangePhase(48, 63)
};

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeNormalTransfer
 * @discussion  IOUSBHostCINormalTransferData0Length is populated in the data0 field with the length of the transfer, in bytes
 *              IOUSBHostCINormalTransferData1Buffer is populated in the data1 field with the virtual address of the buffer to use for the transfer
 */
enum
{
    IOUSBHostCINormalTransferData0Length                                    = IOUSBBitRange(0, 27),
    IOUSBHostCINormalTransferData0LengthPhase                               = IOUSBBitRangePhase(0, 27),
    IOUSBHostCINormalTransferData1Buffer                                    = IOUSBBitRange(0, 63),
    IOUSBHostCINormalTransferData1BufferPhase                               = IOUSBBitRangePhase(0, 63)
};

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeIsochronousTransfer
 * @discussion  IOUSBHostCIIsochronousTransferControlFrameNumber is populated in the control field with the least significant bits of the frame number for this transfer
 *              IOUSBHostCIIsochronousTransferControlASAP is set in the control field to indicate IOUSBHostCIIsochronousTransferControlFrameNumber should be ignored and the transfer should be sent at the endpoint's next service opportunity
 *              IOUSBHostCIIsochronousTransferData0Length is populated in the data0 field with the length of the transfer, in bytes
 *              IOUSBHostCIIsochronousTransferData1BufferPhase is populated in the data1 field with the virtual address of the buffer to use for the transfer
 */
enum
{
    IOUSBHostCIIsochronousTransferControlFrameNumber                        = IOUSBBitRange(16, 23),
    IOUSBHostCIIsochronousTransferControlFrameNumberPhase                   = IOUSBBitRangePhase(16, 23),
    IOUSBHostCIIsochronousTransferControlASAP                               = IOUSBBit(24),
    IOUSBHostCIIsochronousTransferData0Length                               = IOUSBHostCINormalTransferData0Length,
    IOUSBHostCIIsochronousTransferData0LengthPhase                          = IOUSBHostCINormalTransferData0LengthPhase,
    IOUSBHostCIIsochronousTransferData1Buffer                               = IOUSBHostCINormalTransferData1Buffer,
    IOUSBHostCIIsochronousTransferData1BufferPhase                          = IOUSBHostCINormalTransferData1BufferPhase
};

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeLink
 * @discussion  IOUSBHostCILinkData1TransferStructureAddress is populated with the virtual address of the next IOUSBHostCIMessage structure to consume.
 */
enum
{
    IOUSBHostCILinkData1TransferStructureAddress                            = IOUSBBitRange(0, 63),
    IOUSBHostCILinkData1TransferStructureAddressPhase                       = IOUSBBitRangePhase(0, 63)
};

/*!
 * @enum
 * @brief       Fields used in IOUSBHostCIMessage structures with an IOUSBHostCIMessageControlType of IOUSBHostCIMessageTypeTransferComplete
 * @discussion  IOUSBHostCITransferCompletionMessageControlStatus is populated in the control field with a IOUSBHostCIMessageStatus describing the result of the transfer
 *              IOUSBHostCITransferCompletionMessageControlDeviceAddress is populated in the control field with the device address associated with the transfer
 *              IOUSBHostCITransferCompletionMessageControlEndpointAddress is populated in the control field with the endpoint address associated with the transfer
 *              IOUSBHostCITransferCompletionMessageData0TransferLength is populated in the data0 field with the actual number of bytes transferred for the transfer
 *              IOUSBHostCITransferCompletionMessageData1TransferStructure is populated in the data1 field with the virtual address of the IOUSBHostCIMessage transfer structure that generated this event
 */
enum
{
    IOUSBHostCITransferCompletionMessageControlStatus                      = IOUSBHostCIMessageControlStatus,
    IOUSBHostCITransferCompletionMessageControlStatusPhase                 = IOUSBHostCIMessageControlStatusPhase,
    IOUSBHostCITransferCompletionMessageControlDeviceAddress               = IOUSBBitRange(16, 23),
    IOUSBHostCITransferCompletionMessageControlDeviceAddressPhase          = IOUSBBitRangePhase(16, 23),
    IOUSBHostCITransferCompletionMessageControlEndpointAddress             = IOUSBBitRange(24, 31),
    IOUSBHostCITransferCompletionMessageControlEndpointAddressPhase        = IOUSBBitRangePhase(24, 31),
    
    IOUSBHostCITransferCompletionMessageData0TransferLength                = IOUSBBitRange(0, 27),
    IOUSBHostCITransferCompletionMessageData0TransferLengthPhase           = IOUSBBitRangePhase(0, 27),
    
    IOUSBHostCITransferCompletionMessageData1TransferStructure             = IOUSBBitRange64(0, 63),
    IOUSBHostCITransferCompletionMessageData1TransferStructurePhase        = IOUSBBitRangePhase(0, 63)
};

#pragma pack(pop)

#endif /* IOUSBHostControllerInterfaceDefinitions_h */
