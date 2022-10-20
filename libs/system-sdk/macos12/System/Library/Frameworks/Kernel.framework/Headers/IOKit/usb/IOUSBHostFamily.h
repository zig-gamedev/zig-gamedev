/*
 * Copyright (c) 1998-2016 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

/*! 
 @header     IOUSBHostFamily.h
 @brief      IOUSBHostFamily provides support for discovering, enumerating, and communicating with USB devices.
 @discussion Starting with Mac OS X El Capitan, IOUSBHostFamily serves as a replacement for IOUSBFamily.
 
 <h3>Architecture</h3>

 USB host controller hardware is managed by AppleUSBHostController subclasses such as AppleUSBXHCI and AppleUSBEHCI.  Each AppleUSBHostController is the parent to one or more AppleUSBHostPort services, and each AppleUSBHostPort is able to enumerate a single IOUSBHostDevice service.  IOUSBHostDevice is the parent to zero or more IOUSBHostInterface services, and IOUSBHostInterface provides access to zero or more IOUSBHostPipe objects.  USB 3.0 and newer streaming-capable IOUSBHostPipe objects can generate one or more IOUSBHostStream objects.
 
 AppleUSBHostPort services provide a workloop that is dedicated for use by the port, its enumerated device, and any attached drivers.  As such, drivers should not create or manage their own workloops.
 
 IOUSBHostDevice and IOUSBHostInterface services can be matched on and controlled by third-party drivers, and are documented in IOUSBHostDevice.h and IOUSBHostInterface.h.  IOUSBHostPipe and IOUSBHostStream services share a common IOUSBHostIOSource base class, and are documented in IOUSBHostIOSource.h, IOUSBHostPipe.h, and IOUSBHostStream.h.  AppleUSBHostController and AppleUSBHostPort should be considered opaque classes.
 
 <h3>Matching</h3>
 
 IOUSBHostDevice and IOUSBHostInterface matching uses the same properties as the deprecated IOUSBDevice and IOUSBInterface classes.  See <a href="https://developer.apple.com/library/mac/qa/qa1076/_index.html">QA1076: Tips on USB driver matching for Mac OS X</a> for additional information.

 <h3>Power management</h3>

 IOUSBHostFamily supports more advanced power management capabilities, including pausing IO, and automatic suspend of idle devices.  All of this functionality is guided by idle policies set on IOUSBHostInterface services and IOUSBHostPipe objects.  The idle policy functionality allows most drivers to ignore suspend or resume events on the bus, and makes it unnecessary for them to actively participate in power management.

 <h4>Idle suspend</h4>

 By default, interfaces and pipes do not enable an idle policy, and the device will remain in the active (not suspended) state as long as the host machine is awake.  To enable idle suspend, the driver controlling an IOUSBHostInterface must enable an idle policy with the <code>IOUSBHostInterface::setIdlePolicy</code> API.  In the simple case of a device with only a single interface, when the interface's endpoints have no IO requests pending, the system will wait the time specified in the setIdlePolicy call, and then electrically suspend the device.  A new IO request during the wait period will cancel the suspend, and a new IO request after the device has suspended will immediately resume the device and initiate the IO.
 
 In the more complex case of a device with multiple interfaces, a consensus idle policy must be calculated by finding the largest idle policy for all interfaces, excluding those which have not been opened by a driver.
 
 Selection of an idle policy depends on the individual device, and should factor in resume latency tolerance.  The recommended idle policy for an interface is 50ms or larger.
 
 Idle suspend is disabled if a driver is the power child of the IOUSBHostDevice and resides in a power state with an input power requirement that does not include <code>kIOPMLowPower</code>.

 <h4>Pausing IO</h4>
 
 A more advanced approach to idle suspend allows the device to suspend while transfers are still pending.  This is particularly useful for interfaces with interrupt IN endpoints that always have an IO pending, and otherwise would not idle suspend.  To enable IO pause, endpoints that would otherwise keep the device in the active state must enable an idle policy with the <code>IOUSBHostPipe::setIdlePolicy</code> API.  When an IOUSBHostPipe with an idle policy begins servicing a new IO, but does not complete it within the wait time specified in the idle policy, the IOUSBHostPipe is now considered idle and will not keep the device in the active state.  Idle endpoints are still serviced on the bus, and the IOUSBHostPipe is capable of moving data even when idle.  When all endpoints in the device have no pending IO requests or are idle, the idle suspend wait period begins as described above.

 If the idle suspend period elapses without any new IO requests that would cancel the suspend, IOUSBHostFamily will pause the transfers on the bus and suspend the device.  IO requests are not aborted or returned to the driver when paused in this manner.
 
 After resuming the device due to a remote wake or a new IO request, the pending IO requests are resumed on the bus, and data movement can continue.  If a large transfer is interrupted by a suspend, IOUSBHostFamily will combine the packets from before and after the suspend and return them as a single transfer result.  Note that not all firmware supports a mid-transfer suspend/resume, and pausing IO should only be enabled for devices that have been tested for this use case.
 
 Control and isochronous IOUSBHostPipe objects never enable an idle policy, and pending IO on these endpoints will always prevent idle suspend.
 
 Selection of an idle policy depends on the individual endpoint, and should factor in resume latency tolerance and expected data transfer frequency.  The recommended idle policy for IN-direction bulk and interrupt endpoints is 50ms or larger.  Firmware must be capable of handling a mid-transfer suspend/resume, and should issue a remote wake when data is available to be read.  Enabling an idle policy for OUT-direction bulk or interrupt endpoints is not generally recommended, but if enabled, firmware should have the ability to issue a remote wake when there is space in its endpoint buffer for additional data.

 <h4>System power state changes</h4>
 
 For devices that idle suspend, it may be necessary to perform IO as the system enters the sleep state, for example to enable wake-on-LAN for an Ethernet controller.  Drivers that wish to perform this type of IO should call <code>IOService::registerInterestedDriver</code> on the PM root to be notified of system-level changes.  During the <code>powerStateWillChangeTo</code> notification with a capability flag including <code>kIOPMSleepCapability</code>, the driver can initiate an IO request and resume the device.  Once the driver acknowledges this <code>powerStateWillChangeTo</code> notification, the device may not be able to raise its power state until the system wakes.  Attempting an IO request from other PM root <code>powerStateWillChangeTo</code> and <code>powerStateDidChangeTo</code> notifications may result in a deadlock waiting for a power state change.

 <h3>Comparison to IOUSBFamily</h3>
 
 Developers already familiar with IOUSBFamily can take advantage of new capabilities in IOUSBHostFamily that can simplify driver development and maintenace.
 
 <h4>Workloops</h4>
 
 IOUSBFamily provided a workloop for each controller, and all downstream devices shared that workloop.  By contrast, each AppleUSBHostPort creates a workloop dedicated for its attached device, which significantly reduces lock contention and improves responsiveness for drivers attached to IOUSBHostDevice or IOUSBHostInterface services.  Because the workloop is not shared across many unrelated services, driver developers are discouraged from creating an additional workloop.
 
 <h4>Synchronous IO from completion callbacks</h4>
 
 IOUSBFamily did not permit synchronous IO to be executed from a completion callback, but this restriction has been removed in IOUSBHostFamily.  Driver developers can now issue a sequence of synchronous commands in a completion routine without chaining thread calls or using complex state machines.
 
 <h4>IO bookkeeping</h4>
 
 IOUSBHostFamily now tracks all outstanding IOs for an endpoint, and offers more predictable behavior when aborting transfers or terminating services.  In most cases, driver developers no longer need to track the number of outstanding IOs, and can terminate their services with more determinism.

 <h3>Driver Recommendations</h3>
 
 <ul>
    <li>Do not create a separate workloop</li>
    <li>If joining the power plane, reside in a <code>kIOPMLowPower</code> state to enable idle suspend</li>
    <li>If supporting pausing IO, only enable an idle policy on bulk or interrupt IN endpoints</li>
    <li>If interested in system power state changes, call <code>registerInterestedDriver</code> on the PM root</li>
 </ul>
*/

#ifndef IOUSBHostFamily_IOUSBHostFamily_h
#define IOUSBHostFamily_IOUSBHostFamily_h

#include <IOKit/usb/StandardUSB.h>
#include <IOKit/usb/IOUSBHostFamilyDefinitions.h>

#define __IOUSBHOSTFAMILY_DEPRECATED __kpi_deprecated("Use USBDriverKit")

#pragma mark Typedefs

typedef uint16_t tUSBHostDeviceAddress;

#pragma mark General enumerations

#ifdef __cplusplus
using namespace StandardUSB;
#endif

enum tEndpointDirection
{
    kEndpointDirectionOut       = (kEndpointDescriptorDirectionOut >> kEndpointDescriptorDirectionPhase),
    kEndpointDirectionIn        = (kEndpointDescriptorDirectionIn >> kEndpointDescriptorDirectionPhase),
    kEndpointDirectionUnknown   = 2
};

typedef enum tEndpointDirection tEndpointDirection;

enum tEndpointType
{
    kEndpointTypeControl        = (kEndpointDescriptorTransferTypeControl >> kEndpointDescriptorTransferTypePhase),
    kEndpointTypeIsochronous    = (kEndpointDescriptorTransferTypeIsochronous >> kEndpointDescriptorTransferTypePhase),
    kEndpointTypeBulk           = (kEndpointDescriptorTransferTypeBulk >> kEndpointDescriptorTransferTypePhase),
    kEndpointTypeInterrupt      = (kEndpointDescriptorTransferTypeInterrupt >> kEndpointDescriptorTransferTypePhase)
};

typedef enum tEndpointType tEndpointType;

enum tDeviceRequestDirection
{
    kRequestDirectionOut            = (kDeviceRequestDirectionOut >> kDeviceRequestDirectionPhase),
    kRequestDirectionIn             = (kDeviceRequestDirectionIn >> kDeviceRequestDirectionPhase)
};

typedef enum tDeviceRequestDirection tDeviceRequestDirection;

enum tDeviceRequestType
{
    kRequestTypeStandard            = (kDeviceRequestTypeStandard >> kDeviceRequestTypePhase),
    kRequestTypeClass               = (kDeviceRequestTypeClass >> kDeviceRequestTypePhase),
    kRequestTypeVendor              = (kDeviceRequestTypeVendor >> kDeviceRequestTypePhase)
};

typedef enum tDeviceRequestType tDeviceRequestType;

enum tDeviceRequestRecipient
{
    kRequestRecipientDevice         = (kDeviceRequestRecipientDevice >> kDeviceRequestRecipientPhase),
    kRequestRecipientInterface      = (kDeviceRequestRecipientInterface >> kDeviceRequestRecipientPhase),
    kRequestRecipientEndpoint       = (kDeviceRequestRecipientEndpoint >> kDeviceRequestRecipientPhase),
    kRequestRecipientOther          = (kDeviceRequestRecipientOther >> kDeviceRequestRecipientPhase)
};

typedef enum tDeviceRequestRecipient tDeviceRequestRecipient;

static inline uint8_t makeDeviceRequestbmRequestType(tDeviceRequestDirection direction, tDeviceRequestType type, tDeviceRequestRecipient recipient)
{
    return   ((direction << kDeviceRequestDirectionPhase) & kDeviceRequestDirectionMask)
            | ((type << kDeviceRequestTypePhase) & kDeviceRequestTypeMask)
            | ((recipient << kDeviceRequestRecipientPhase) & kDeviceRequestRecipientMask);
}

enum
{
    kUSBHostVendorSpecificClass             = 255,
    kUSBHostHubClass                        = 9,
    kUSBHostMaxDevices                      = 128,           // The largest number of devices permitted on USB 2.0 buses
    kUSBHostMaxPipes                        = 32,            // The largest number of endpoints permitted on USB devices
    kUSBHostMaxCountFullSpeedIsochronous    = 1023,       // max size (bytes) of any one Isoc frame for 1 FS endpoint
    kUSBHostVendorIDAppleComputer           = 0x05AC
};

/*!
 * @enum tInternalUSBHostConnectionSpeed
 * @brief Connection speeds used internally by IOUSBHostFamily
 */
enum tInternalUSBHostConnectionSpeed
{
    kUSBHostConnectionSpeedLow          = 0,
    kUSBHostConnectionSpeedFull         = 1,
    kUSBHostConnectionSpeedHigh         = 2,
    kUSBHostConnectionSpeedSuper        = 3,
    kUSBHostConnectionSpeedSuperPlus    = 4,
    kUSBHostConnectionSpeedSuperPlusBy2 = 5,
    kUSBHostConnectionSpeedCount        = 6
};


/*!
 @definedblock  IOUSBHostFamily message codes
 @discussion    Messages passed between USB services using the <code>IOService::message</code> API.
 */
#define kUSBHostMessageUpdateIdlePolicy               iokit_usbhost_msg(0x100)      // 0xe0005100  Apple Internal use only.  IOUSBHostInterface -> IOUSBHostDevice to update its idle policy.
#define kUSBHostMessageRemoteWake                     iokit_usbhost_msg(0x101)      // 0xe0005101  Apple Internal use only.  AppleUSBHostController -> AppleUSBHostPort -> IOUSBHostDevice upon a remote wake event.  Argument is locationID of remote wake.
#define kUSBHostMessageDeviceSuspend                  iokit_usbhost_msg(0x102)      // 0xe0005102  Apple Internal use only.  IOUSBHostDevice -> clients upon a suspend event.
#define kUSBHostMessageDeviceResume                   iokit_usbhost_msg(0x103)      // 0xe0005103  Apple Internal use only.  IOUSBHostDevice -> clients upon a resume event.
#define kUSBHostMessagePortsCreated                   iokit_usbhost_msg(0x104)      // 0xe0005104  Apple Internal use only.  AppleUSBHostController and AppleUSBHub -> clients after all ports have been created.
#define kUSBHostMessageDeviceConnected                iokit_usbhost_msg(0x105)      // 0xe0005105  Apple Internal use only.  AppleUSBRemovablePort -> clients after a connect.
#define kUSBHostMessageDeviceDisconnected             iokit_usbhost_msg(0x106)      // 0xe0005106  Apple Internal use only.  AppleUSBRemovablePort -> clients after a disconnect.
#define kUSBHostMessageControllerPoweredOn            iokit_usbhost_msg(0x107)      // 0xe0005107  Apple Internal use only.  AppleEmbeddedUSBXHCIFL1100 -> FL1100Boot after a stable power state is reached.
#define kUSBHostMessageNonInterruptIsochFrame         iokit_usbhost_msg(0x108)      // 0xe0005108  Apple Internal use only. 
#define kUSBHostMessageInterfaceAlternateSetting      iokit_usbhost_msg(0x109)      // 0xe0005109  Apple Internal use only.  IOUSBHostInterface -> IOUSBInterface to update interface properties after an alternate setting is selected
#define kUSBHostMessageDeviceLegacyCapture            iokit_usbhost_msg(0x10A)      // 0xe000510A  Apple Internal use only.  IOUSBHostDevice -> IOUSBDevice to relay user space re-enumeration for capturing/releasing devices
#define kUSBHostMessageControllerInterrupt            iokit_usbhost_msg(0x10B)      // 0xe000510B  Apple Internal use only.  Source -> AppleUSBHostController to indicate an interrupt is ready for consumption

// User Message Support

#define kUSBLegacyMessagePortHasBeenSuspended         iokit_usblegacy_err_msg(0x0d) // 0xe000400d  Apple Internal use only. Message sent by an IOUSBDevice indicating the completion of a power change on the compatibility service
#define kUSBLegacyMessagePortHasBeenResumed           iokit_usblegacy_err_msg(0x0b) // 0xe000400b  Apple Internal use only. Message sent by an IOUSBDevice indicating the completion of a power change on the compatibility service
#define kUSBHostMessageOvercurrentCondition           iokit_usblegacy_err_msg(0x13) // 0xe0004013  Apple Internal use only.  Message sent to the clients of the device's hub parent, when a device causes an overcurrent condition.  The message argument contains the locationID of the device
#define kUSBHostMessageNotEnoughPower                 iokit_usblegacy_err_msg(0x14) // 0xe0004014  Apple Internal use only.  Message sent to the clients of the device's hub parent, when a device causes an low power notice to be displayed.  The message argument contains the locationID of the device
#define kIOUSBHostMessageReallocateExtraCurrent       iokit_usblegacy_err_msg(0x18) // 0xe0004018  Message to ask any clients using extra current to attempt to allocate it some more
#define kUSBHostMessageEndpointCountExceeded          iokit_usblegacy_err_msg(0x19) // 0xe0004019  Apple Internal use only.  Message sent to a device when endpoints cannot be created because the USB controller ran out of resources
#define kUSBHostMessageDeviceCountExceeded            iokit_usblegacy_err_msg(0x1a) // 0xe000401a  Apple Internal use only.  Message sent by a hub when a device cannot be enumerated because the USB controller ran out of resources
#define kUSBHostMessageUnsupportedConfiguration       iokit_usblegacy_err_msg(0x1c) // 0xe000401c  Apple Internal use only.  Message sent to the clients of the device when a device is not supported in the current configuration.  The message argument contains the locationID of the device
#define kUSBHostMessageHubCountExceeded               iokit_usblegacy_err_msg(0x1d) // 0xe000401d  Apple Internal use only.  Message sent when a 6th hub was plugged in and was not enumerated, as the USB spec only support 5 hubs in a chain
#define kUSBHostMessageTDMLowBattery                  iokit_usblegacy_err_msg(0x1e) // 0xe000401e  Apple Internal use only.  Message sent when when an attached TDM system battery is running low.
/*! @/definedblock */


/*!
 * @brief Port types returned by IOUSBHostDevice::getPortStatus
 *
 * @constant kUSBHostPortTypeStandard A general-purpose USB port.
 * @constant kUSBHostPortTypeCaptive The attached device cannot be physically disconnected from the port.
 * @constant kUSBHostPortTypeInternal The attached device cannot be physically disconnected from the host machine.
 * @constant kUSBHostPortTypeAccessory The attached device may require authentication before function drivers can access it.
 * @constant kUSBHostPortTypeCount The number of entries in this enum.
 */
enum tUSBHostPortType
{
    kUSBHostPortTypeStandard    = kIOUSBHostPortTypeStandard,
    kUSBHostPortTypeCaptive     = kIOUSBHostPortTypeCaptive,
    kUSBHostPortTypeInternal    = kIOUSBHostPortTypeInternal,
    kUSBHostPortTypeAccessory   = kIOUSBHostPortTypeAccessory,
    kUSBHostPortTypeExpressCard = kIOUSBHostPortTypeExpressCard,
    kUSBHostPortTypeCount       = kIOUSBHostPortTypeCount
};

/*!
 * @brief Connection speeds returned by IOUSBHostDevice::getPortStatus
 * @discussion This enumeration matches the default speed ID mappings defined in XHCI 1.0 Table 147.
 *
 * @constant kUSBHostPortConnectionSpeedNone No device is connected
 * @constant kUSBHostPortConnectionSpeedFull A full-speed (12 Mb/s) device is connected
 * @constant kUSBHostPortConnectionSpeedLow A low-speed (1.5 Mb/s) device is connected
 * @constant kUSBHostPortConnectionSpeedHigh A high-speed (480 Mb/s) device is connected)
 * @constant kUSBHostPortConnectionSpeedSuper A superspeed (5 Gb/s) device is connected)
 * @constant kUSBHostPortConnectionSpeedSuperPlus A superspeed (10 Gb/s) device is connected)
 * @constant kUSBHostPortConnectionSpeedUSB3Gen2By2 A superspeed (20 Gb/s) device is connected)
 */
enum tUSBHostConnectionSpeed
{
    kUSBHostPortConnectionSpeedNone         = kIOUSBHostConnectionSpeedNone,
    kUSBHostPortConnectionSpeedFull         = kIOUSBHostConnectionSpeedFull,
    kUSBHostPortConnectionSpeedLow          = kIOUSBHostConnectionSpeedLow,
    kUSBHostPortConnectionSpeedHigh         = kIOUSBHostConnectionSpeedHigh,
    kUSBHostPortConnectionSpeedSuper        = kIOUSBHostConnectionSpeedSuper,
    kUSBHostPortConnectionSpeedSuperPlus    = kIOUSBHostConnectionSpeedSuperPlus,
    kUSBHostPortConnectionSpeedSuperPlusBy2 = kIOUSBHostConnectionSpeedSuperPlusBy2,
    kUSBHostPortConnectionSpeedCount        = kIOUSBHostConnectionSpeedCount
};

/*!
 * @enum tOpenOptions
 * @brief Option bits used when calling ::open on IOUSBHostFamily services
 */
enum
{
    kUSBHostOpenOptionSelectAlternateSetting = StandardUSBBit(16),  // IOUSBHostInterface
    kUSBHostOpenOptionUserClientSession      = StandardUSBBit(17)   // IOUSBHostDevice: Reserved for AppleUSBHostUserClient objects
};

/*!
 * @brief Values returned by IOUSBHostDevice::getPortStatus
 *
 * @constant kUSBHostPortStatusPortTypeMask The mask for bits representing the port type.
 * @constant kUSBHostPortStatusPortTypeStandard A general-purpose USB port.
 * @constant kUSBHostPortStatusPortTypeCaptive The attached device cannot be physically disconnected from the port.
 * @constant kUSBHostPortStatusPortTypeInternal The attached device cannot be physically disconnected from the host machine.
 * @constant kUSBHostPortStatusPortTypeAccessory The attached device may require authentication before function drivers can access it.
 * @constant kUSBHostPortStatusConnectedSpeedMask The mask for bits representing the connection state.
 * @constant kUSBHostPortStatusConnectedSpeedNone The port does not have a connected device.
 * @constant kUSBHostPortStatusConnectedSpeedFull The port has a full-speed device connected.
 * @constant kUSBHostPortStatusConnectedSpeedLow The port has a low-speed device connected.
 * @constant kUSBHostPortStatusConnectedSpeedHigh The port has a high-speed device connected.
 * @constant kUSBHostPortStatusConnectedSpeedSuper The port has a superspeed device connected.
 * @constant kUSBHostPortStatusResetting The port is currently resetting the link.
 * @constant kUSBHostPortStatusEnabled The port is enabled and packets are permitted to reach the device.  Not valid unless kUSBHostPortStatusConnectedSpeedMask is nonzero.
 * @constant kUSBHostPortStatusSuspended The port is suspended.  Not valid unless kUSBHostPortStatusConnectedSpeedMask is nonzero.
 * @constant kUSBHostPortStatusOvercurrent The port is in the overcurrent condition.
 * @constant kUSBHostPortStatusTestMode The port is in test mode.
 */
enum tUSBHostPortStatus
{
    kUSBHostPortStatusPortTypeMask               = kIOUSBHostPortStatusPortTypeMask,
    kUSBHostPortStatusPortTypePhase              = kIOUSBHostPortStatusPortTypePhase,
    kUSBHostPortStatusPortTypeStandard           = kIOUSBHostPortStatusPortTypeStandard,
    kUSBHostPortStatusPortTypeCaptive            = kIOUSBHostPortStatusPortTypeCaptive,
    kUSBHostPortStatusPortTypeInternal           = kIOUSBHostPortStatusPortTypeInternal,
    kUSBHostPortStatusPortTypeAccessory          = kIOUSBHostPortStatusPortTypeAccessory,
    kUSBHostPortStatusPortTypeReserved           = kIOUSBHostPortStatusPortTypeReserved,
    kUSBHostPortStatusConnectedSpeedMask         = kIOUSBHostPortStatusConnectedSpeedMask,
    kUSBHostPortStatusConnectedSpeedPhase        = kIOUSBHostPortStatusConnectedSpeedPhase,
    kUSBHostPortStatusConnectedSpeedNone         = kIOUSBHostPortStatusConnectedSpeedNone,
    kUSBHostPortStatusConnectedSpeedFull         = kIOUSBHostPortStatusConnectedSpeedFull,
    kUSBHostPortStatusConnectedSpeedLow          = kIOUSBHostPortStatusConnectedSpeedLow,
    kUSBHostPortStatusConnectedSpeedHigh         = kIOUSBHostPortStatusConnectedSpeedHigh,
    kUSBHostPortStatusConnectedSpeedSuper        = kIOUSBHostPortStatusConnectedSpeedSuper,
    kUSBHostPortStatusConnectedSpeedSuperPlus    = kIOUSBHostPortStatusConnectedSpeedSuperPlus,
    kUSBHostPortStatusConnectedSpeedSuperPlusBy2 = kIOUSBHostPortStatusConnectedSpeedSuperPlusBy2,
    kUSBHostPortStatusResetting                  = kIOUSBHostPortStatusResetting,
    kUSBHostPortStatusEnabled                    = kIOUSBHostPortStatusEnabled,
    kUSBHostPortStatusSuspended                  = kIOUSBHostPortStatusSuspended,
    kUSBHostPortStatusOvercurrent                = kIOUSBHostPortStatusOvercurrent,
    kUSBHostPortStatusTestMode                   = kIOUSBHostPortStatusTestMode
};

/*!
 @enum tUSBHostDefaultControlRequestTimeout
 @brief Default control request timeout values in milliseconds
 */
enum
{
#if USB_COMPLIANCE_MODE
    kUSBHostSetAddressTimeout                      = 50,    // USB 2.0 9.2.6.3
    kUSBHostStandardRequestNoDataTimeout           = 500,   // USB 2.0 9.2.6.4
    kUSBHostStandardRequestCompletionTimeout       = 5000,  // USB 2.0 9.2.6.4
    kUSBHostStandardRequestSimpleCompletionTimeout = 50,    // USB 2.0 9.2.6.4 Standard request with no data stage
    kUSBHostClassRequestNoDataTimeout              = 500,   // USB 2.0 9.2.6.5
    kUSBHostClassRequestCompletionTimeout          = 5000,  // USB 2.0 9.2.6.5
    kUSBHostVendorRequestNoDataTimeout             = 0,
    kUSBHostVendorRequestCompletionTimeout         = 5000,
#else
    // More tolerant timeout values for noncompliant devices
    kUSBHostSetAddressTimeout                      = 1000,
    kUSBHostStandardRequestNoDataTimeout           = 0,
    kUSBHostStandardRequestCompletionTimeout       = 5000,
    kUSBHostStandardRequestSimpleCompletionTimeout = 1000,
    kUSBHostClassRequestNoDataTimeout              = 0,
    kUSBHostClassRequestCompletionTimeout          = 5000,
    kUSBHostVendorRequestNoDataTimeout             = 0,
    kUSBHostVendorRequestCompletionTimeout         = 5000,
#endif
    kUSBHostDefaultControlNoDataTimeoutMS          = 0,
    kUSBHostDefaultControlCompletionTimeoutMS      = 5000
};

#pragma mark APCI enumerations

// ACPI 5.0a Table 9-227: UPC Return Package Values
typedef enum
{
    kUSBHostPortNotConnectable = 0,                 // Port is not connectable
    kUSBHostPortConnectable    = 1                  // Port is connectable either user visible or invisible
} tUSBHostPortConnectable;

// ACPI 5.0a Table 9-227: UPC Return Package Values
typedef enum
{
    kUSBHostConnectorTypeA              = 0x00,
    kUSBHostConnectorTypeMiniAB         = 0x01,
    kUSBHostConnectorTypeExpressCard    = 0x02,
    kUSBHostConnectorTypeUSB3A          = 0x03,
    kUSBHostConnectorTypeUSB3B          = 0x04,
    kUSBHostConnectorTypeUSB3MicroB     = 0x05,
    kUSBHostConnectorTypeUSB3MicroAB    = 0x06,
    kUSBHostConnectorTypeUSB3PowerB     = 0x07,
    kUSBHostConnectorTypeUSBTypeC       = 0x09,
    kUSBHostConnectorTypeUnknown        = 0xFE,
    kUSBHostConnectorTypeProprietary    = 0xFF
} tUSBHostConnectorType;

/*!
 * @enum tUSBHostPowerSourceType
 * @brief Power sources used internally by IOUSBHostFamily
 * @constant kUSBHostPowerSourceTypeStaticPool The power source is a statically allocated pool for software to manage.
 * @constant kUSBHostPowerSourceTypeSMC        The power source is managed by the SMC.
 * @constant kUSBHostPowerSourceTypeHardware   The power source is guaranteed by the hardware and can always allocate the port current limit.
 */
typedef enum
{
    kUSBHostPowerSourceTypeStaticPool   = 0,
    kUSBHostPowerSourceTypeSMC,
    kUSBHostPowerSourceTypeHardware
}tUSBHostPowerSourceType;

#if TARGET_CPU_X86 || TARGET_CPU_X86_64

#ifndef kACPIDevicePathKey
#define kACPIDevicePathKey          "acpi-path"
#endif

#ifndef kACPIInterruptTypeValid
#define kACPIInterruptTypeValid     (1 << 1)
#endif

// ACPI methods
#define kUSBHostACPIPropertyXHCICompanion   "XHCN"      // The number of the companion XHCI controller, used for multiplexed ports
#define kUSBHostACPIPropertyMultiplexor     "MUXS"      // The ACPI method name for the port multiplexor

// _UPC
// _EJD

#define kSDControllerGPIOResetACPIMethod                "SRST"
#define kSDControllerGPIOPowerACPIMethod                "SPWR"
#define kSDPortConnectionBehaviorACPIMethod             "SBHV"
#define kSDControllerGPIOResetPropertyKey               "GPIO_RESET"
#define kSDControllerCaptiveUSB3ReaderKey               "U3SD"
#define kGetBehaviorACPIMethod                          "GBHV"
#define kGPEACPIString                                  "_GPE"
#define kRDYForGPIOTest                                 "RDYG"
#define kReconfiguredCount                              "RCFG"
#define kUSBPlatformProperties                          "USBX"
#define kUSBTypeCCableDetectACPIMethod                  "MODU"
#define kUSBTypeCCableDetectACPIMethodSupported         "RTPC"
#define kGPEOCACPIString                                "GPOC"
#define kACPIPortTimingOverride                         "TMG"

// connection types returned by MODU method
typedef enum
{
    kUSBTypeCCableTypeNone              = 0,
    kUSBTypeCCableTypeUSB               = 1,
} tUSBCTypeCableType;

#endif

typedef enum
{
    kLinkStateU0                = 0,                // USB3 Link State U0 (On)
    kLinkStateU1                = 1,                // USB3 Link State U1 (Standby, fast exit)
    kLinkStateU2                = 2,                // USB3 Link State U2 (Standby, slower exit)
    kLinkStateU3                = 3,                // USB3 Link State U3 (Suspend)
    
    kLinkStateL0                = 0,                // USB2 Link State L0 (On)
    kLinkStateL1                = 1,                // USB2 Link State L1 (Sleep)
    kLinkStateL2                = 2,                // USB2 Link State L2 (Suspend)
    kLinkStateL3                = 3,                // USB2 Link State L3 (Off)
    
} tUSBLinkState;

typedef enum
{
    kDescriptorExitLatency,                         // Descriptor Exit Latencies (DEL).
    kDeviceExitLatency,                             // Device Initiated Exit Latencies (PEL)
    kSystemExitLatency,                             // System Exit Latency (SEL)
    kMaxExitLatency,                                // Max Exit Latency (MEL)

    // USB2 LPM specific latencies
    kBestEffortServiceLatency,                      // Best Effort Service Latency
    kBestEffortServiceLatencyDeep,                  // Best Effort Service Latency Deep
    kDefaultBestEffortServiceLatency,               // Default Best Effort Service Latency
    kDefaultBestEffortServiceLatencyDeep,           // Default Best Effort Service Latency Deep
    kMaxL1BaseExitLatency,                          // Max Exit Latency for L1 while using BESL base value
    kMaxL1DeepExitLatency,                          // Max Exit Latency for L1 while using BESL Deep

} tUSBLPMExitLatency;

enum
{
    kUSB3LPMMaxU1SEL             = 0xFF,
    kUSB3LPMMaxU1PEL             = 0xFF,
    
    kUSB3LPMMaxU2SEL             = 0xFFFF,
    kUSB3LPMMaxU2PEL             = 0xFFFF,
    
    kUSB3LPMMaxU1Timeout         = 0x7F,
    kUSB3LPMU1Disabled           = 0,
    kUSB3LPMU1AcceptOnly         = 0xFF,
    
    kUSB3LPMMaxU2Timeout         = 0xFE00,          // In Micro Seconds
    kUSB3LPMU2Disabled           = 0,               // In Micro Seconds
    kUSB3LPMU2AcceptOnly         = 0xFF00,          // In Micro Seconds
    
    kUSB3LPMMaxT3SEL             = 100,             // SEL:T3 value as per USB 3.0 Section C.1.5.1 in Nano Seconds
    kUSB3LPMMaxHostSchDelay      = 1000,            // Maximum Host Scheduling Delay for Max Exit Latency Calculations
    
    kUSB3LPMExtraDeviceEL        = 4000,            // An additional delay(buffer) in Nano Seconds added to Exit Latency reported by the device in its BOS descriptor
    
    kUSB2LPMMaxL1Timeout         = 0xFF00,          // In Micro seconds
};

typedef enum
{
    kUSBDeviceLPMStatusDefault   = 0,
    kUSBDeviceLPMStatusDisabled  = 1,
    kUSBDeviceLPMStatusEnabled   = 2
    
} tUSBDeviceLPMStatus;

#endif
