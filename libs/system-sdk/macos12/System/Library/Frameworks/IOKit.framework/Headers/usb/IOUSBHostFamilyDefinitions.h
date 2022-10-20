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
   @header     IOUSBHostFamilyDefinitions.h
   @brief      Definitions used for the IOUSBHostFamily
   !*/

#ifndef IOUSBHostFamily_IOUSBHostFamilyDefinitions_h
#define IOUSBHostFamily_IOUSBHostFamilyDefinitions_h

#include <TargetConditionals.h>
#if TARGET_OS_DRIVERKIT
#include <stdint.h>
#include <stddef.h>
#include <sys/_types/_uuid_t.h>
#include <DriverKit/IOTypes.h>
#include <DriverKit/IOLib.h>
#else
#include <IOKit/IOTypes.h>
#include <libkern/OSByteOrder.h>
#endif

#define IOUSBHostFamilyBit(bit)                     ((uint32_t)(1) << bit)
#define IOUSBHostFamilyBitRange(start, end)         (~(((uint32_t)(1) << start) - 1) & (((uint32_t)(1) << end) | (((uint32_t)(1) << end) - 1)))
#define IOUSBHostFamilyBitRange64(start, end)       (~(((uint64_t)(1) << start) - 1) & (((uint64_t)(1) << end) | (((uint64_t)(1) << end) - 1)))
#define IOUSBHostFamilyBitRangePhase(start, end)    (start)

#define iokit_usb_codemask                          IOUSBHostFamilyBitRange(0, 11)
#define iokit_usbhost_group                         (1 << IOUSBHostFamilyBitRangePhase(12, 13))
#define iokit_usblegacy_group                       (0 << IOUSBHostFamilyBitRangePhase(12, 13))
#define iokit_usbhost_err(message)                  ((IOReturn)(iokit_family_err(sub_iokit_usb, iokit_usbhost_group | (message & iokit_usb_codemask))))
#define iokit_usbhost_msg(message)                  ((uint32_t)(iokit_family_msg(sub_iokit_usb, iokit_usbhost_group | (message & iokit_usb_codemask))))
#define iokit_usblegacy_err_msg(message)            ((uint32_t)(sys_iokit | sub_iokit_usb | message))

/*!
   @definedblock  IOUSBHostFamily message codes
   @discussion    Messages passed between USB services using the <code>IOService::message</code> API.
 */
#define kUSBHostMessageConfigurationSet             iokit_usbhost_msg(0x00) // 0xe0005000  IOUSBHostDevice -> clients upon a setConfiguration call.
#define kUSBHostMessageRenegotiateCurrent           iokit_usbhost_msg(0x01) // 0xe0005001  Request clients to renegotiate bus current allocations
#define kUSBHostMessageControllerException          iokit_usbhost_msg(0x02) // 0xe0005002  A fatal problem has occurred with an AppleUSBUserHCI controller

#define kUSBHostReturnPipeStalled                   iokit_usbhost_err(0x0)  // 0xe0005000  Pipe has issued a STALL handshake.  Use clearStall to clear this condition.
#define kUSBHostReturnNoPower                       iokit_usbhost_err(0x1)  // 0xe0005001  A setConfiguration call was not able to succeed because all configurations require more power than is available.

/*!
 * @enum       tIOUSBHostConnectionSpeed
 * @brief      Connection speeds reported in kUSBHostMatchingPropertySpeed
 * @discussion This enumeration matches the default speed ID mappings defined in XHCI 1.0 Table 147.
 * @constant   kIOUSBHostConnectionSpeedNone No device is connected
 * @constant   kIOUSBHostConnectionSpeedFull A full-speed (12 Mb/s) device is connected
 * @constant   kIOUSBHostConnectionSpeedLow A low-speed (1.5 Mb/s) device is connected
 * @constant   kIOUSBHostConnectionSpeedHigh A high-speed (480 Mb/s) device is connected)
 * @constant   kIOUSBHostConnectionSpeedSuper A superspeed (5 Gb/s) device is connected)
 * @constant   kIOUSBHostConnectionSpeedSuperPlus A superspeed (10 Gb/s) device is connected)
 * @constant   kIOUSBHostConnectionSpeedSuperPlusBy2 A superspeed (20 Gb/s) device is connected)
 */
enum tIOUSBHostConnectionSpeed
{
    kIOUSBHostConnectionSpeedNone         = 0,
    kIOUSBHostConnectionSpeedFull         = 1,
    kIOUSBHostConnectionSpeedLow          = 2,
    kIOUSBHostConnectionSpeedHigh         = 3,
    kIOUSBHostConnectionSpeedSuper        = 4,
    kIOUSBHostConnectionSpeedSuperPlus    = 5,
    kIOUSBHostConnectionSpeedSuperPlusBy2 = 6,
    kIOUSBHostConnectionSpeedCount        = 7
};

/*!
 * @brief Port types returned by IOUSBHostDevice::getPortStatus and kUSBHostPortPropertyStatus
 *
 * @constant kIOUSBHostPortTypeStandard A general-purpose USB port.
 * @constant kIOUSBHostPortTypeCaptive The attached device cannot be physically disconnected from the port.
 * @constant kIOUSBHostPortTypeInternal The attached device cannot be physically disconnected from the host machine.
 * @constant kIOUSBHostPortTypeAccessory The attached device may require authentication before function drivers can access it.
 * @constant kIOUSBHostPortTypeCount The number of entries in this enum.
 */
enum tIOUSBHostPortType
{
    kIOUSBHostPortTypeStandard = 0,
    kIOUSBHostPortTypeCaptive,
    kIOUSBHostPortTypeInternal,
    kIOUSBHostPortTypeAccessory,
    kIOUSBHostPortTypeExpressCard,
    kIOUSBHostPortTypeCount
};

/*!
 * @brief Values returned by IOUSBHostDevice::getPortStatus  and kUSBHostPortPropertyStatus
 *
 * @constant kIOUSBHostPortStatusPortTypeMask The mask for bits representing the port type.
 * @constant kIOUSBHostPortStatusPortTypeStandard A general-purpose USB port.
 * @constant kIOUSBHostPortStatusPortTypeCaptive The attached device cannot be physically disconnected from the port.
 * @constant kIOUSBHostPortStatusPortTypeInternal The attached device cannot be physically disconnected from the host machine.
 * @constant kIOUSBHostPortStatusPortTypeAccessory The attached device may require authentication before function drivers can access it.
 * @constant kIOUSBHostPortStatusConnectedSpeedMask The mask for bits representing the connection state.
 * @constant kIOUSBHostPortStatusConnectedSpeedNone The port does not have a connected device.
 * @constant kIOUSBHostPortStatusConnectedSpeedFull The port has a full-speed device connected.
 * @constant kIOUSBHostPortStatusConnectedSpeedLow The port has a low-speed device connected.
 * @constant kIOUSBHostPortStatusConnectedSpeedHigh The port has a high-speed device connected.
 * @constant kIOUSBHostPortStatusConnectedSpeedSuper The port has a superspeed device connected.
 * @constant kIOUSBHostPortStatusResetting The port is currently resetting the link.
 * @constant kIOUSBHostPortStatusEnabled The port is enabled and packets are permitted to reach the device.  Not valid unless kIOUSBHostPortStatusConnectedSpeedMask is nonzero.
 * @constant kIOUSBHostPortStatusSuspended The port is suspended.  Not valid unless kIOUSBHostPortStatusConnectedSpeedMask is nonzero.
 * @constant kIOUSBHostPortStatusOvercurrent The port is in the overcurrent condition.
 * @constant kIOUSBHostPortStatusTestMode The port is in test mode.
 */
enum tIOUSBHostPortStatus
{
    kIOUSBHostPortStatusPortTypeMask               = IOUSBHostFamilyBitRange(0, 3),
    kIOUSBHostPortStatusPortTypePhase              = IOUSBHostFamilyBitRangePhase(0, 3),
    kIOUSBHostPortStatusPortTypeStandard           = (kIOUSBHostPortTypeStandard << IOUSBHostFamilyBitRangePhase(0, 3)),
    kIOUSBHostPortStatusPortTypeCaptive            = (kIOUSBHostPortTypeCaptive << IOUSBHostFamilyBitRangePhase(0, 3)),
    kIOUSBHostPortStatusPortTypeInternal           = (kIOUSBHostPortTypeInternal << IOUSBHostFamilyBitRangePhase(0, 3)),
    kIOUSBHostPortStatusPortTypeAccessory          = (kIOUSBHostPortTypeAccessory << IOUSBHostFamilyBitRangePhase(0, 3)),
    kIOUSBHostPortStatusPortTypeReserved           = IOUSBHostFamilyBitRange(4, 7),
    kIOUSBHostPortStatusConnectedSpeedMask         = IOUSBHostFamilyBitRange(8, 10),
    kIOUSBHostPortStatusConnectedSpeedPhase        = IOUSBHostFamilyBitRangePhase(8, 10),
    kIOUSBHostPortStatusConnectedSpeedNone         = (kIOUSBHostConnectionSpeedNone << IOUSBHostFamilyBitRangePhase(8, 10)),
    kIOUSBHostPortStatusConnectedSpeedFull         = (kIOUSBHostConnectionSpeedFull << IOUSBHostFamilyBitRangePhase(8, 10)),
    kIOUSBHostPortStatusConnectedSpeedLow          = (kIOUSBHostConnectionSpeedLow << IOUSBHostFamilyBitRangePhase(8, 10)),
    kIOUSBHostPortStatusConnectedSpeedHigh         = (kIOUSBHostConnectionSpeedHigh << IOUSBHostFamilyBitRangePhase(8, 10)),
    kIOUSBHostPortStatusConnectedSpeedSuper        = (kIOUSBHostConnectionSpeedSuper << IOUSBHostFamilyBitRangePhase(8, 10)),
    kIOUSBHostPortStatusConnectedSpeedSuperPlus    = (kIOUSBHostConnectionSpeedSuperPlus << IOUSBHostFamilyBitRangePhase(8, 10)),
    kIOUSBHostPortStatusConnectedSpeedSuperPlusBy2 = (kIOUSBHostConnectionSpeedSuperPlusBy2 << IOUSBHostFamilyBitRangePhase(8, 10)),
    kIOUSBHostPortStatusResetting                  = IOUSBHostFamilyBit(11),
    kIOUSBHostPortStatusEnabled                    = IOUSBHostFamilyBit(12),
    kIOUSBHostPortStatusSuspended                  = IOUSBHostFamilyBit(13),
    kIOUSBHostPortStatusOvercurrent                = IOUSBHostFamilyBit(14),
    kIOUSBHostPortStatusTestMode                   = IOUSBHostFamilyBit(15)
};

#pragma mark Entitlements
#define kIOUSBTransportDextEntitlement                          "com.apple.developer.driverkit.transport.usb"
#define kIOUSBHostVMEntitlement                                 "com.apple.vm.device-access"
#define kIOUSBHostControllerInterfaceEntitlement                "com.apple.developer.usb.host-controller-interface"

#pragma mark Registry property names

#define kUSBHostMatchingPropertySpeed                           "USBSpeed"
#define kUSBHostMatchingPropertyPortType                        "USBPortType"

#define kUSBHostMatchingPropertyVendorID                        "idVendor"
#define kUSBHostMatchingPropertyProductID                       "idProduct"
#define kUSBHostMatchingPropertyProductIDMask                   "idProductMask"
#define kUSBHostMatchingPropertyProductIDArray                  "idProductArray"
#define kUSBHostMatchingPropertyDeviceClass                     "bDeviceClass"
#define kUSBHostMatchingPropertyDeviceSubClass                  "bDeviceSubClass"
#define kUSBHostMatchingPropertyDeviceProtocol                  "bDeviceProtocol"
#define kUSBHostMatchingPropertyDeviceReleaseNumber             "bcdDevice"
#define kUSBHostMatchingPropertyConfigurationValue              "bConfigurationValue"
#define kUSBHostMatchingPropertyInterfaceClass                  "bInterfaceClass"
#define kUSBHostMatchingPropertyInterfaceSubClass               "bInterfaceSubClass"
#define kUSBHostMatchingPropertyInterfaceProtocol               "bInterfaceProtocol"
#define kUSBHostMatchingPropertyInterfaceNumber                 "bInterfaceNumber"

#define kUSBHostPropertyLocationID                              "locationID"
#define kUSBHostPropertyDebugOptions                            "kUSBDebugOptions"
#define kUSBHostPropertyWakePowerSupply                         "kUSBWakePowerSupply"
#define kUSBHostPropertySleepPowerSupply                        "kUSBSleepPowerSupply"
#define kUSBHostPropertyWakePortCurrentLimit                    "kUSBWakePortCurrentLimit"
#define kUSBHostPropertySleepPortCurrentLimit                   "kUSBSleepPortCurrentLimit"
#define kUSBHostPropertyFailedRemoteWake                        "kUSBFailedRemoteWake"
#define kUSBHostPropertyBusCurrentPoolID                        "UsbBusCurrentPoolID"
#define kUSBHostPropertySmcBusCurrentPoolID                     "UsbSmcBusCurrentPoolID"
#define kUSBHostPropertyForcePower                              "UsbForcePower"
#define kUSBHostPropertyForceLinkSpeed                          "UsbLinkSpeed"
#define kUSBHostPropertyForceHardwareException                  "UsbHardwareException"
#define kUSBHostPropertyAllowSoftRetry                          "UsbAllowSoftRetry"

#define kUSBHostUserClientPropertyOwningTaskName                "UsbUserClientOwningTaskName"
#define kUSBHostUserClientPropertyEntitlementRequired           "UsbUserClientEntitlementRequired"
#define kUSBHostUserClientPropertyEnableReset                   "UsbUserClientEnableReset"
#define kUSBHostUserClientPropertyEnableDataToggleReset         "UsbUserClientEnableDataToggleReset"

#define kUSBHostDevicePropertyVendorString                      "kUSBVendorString"
#define kUSBHostDevicePropertySerialNumberString                "kUSBSerialNumberString"
#define kUSBHostDevicePropertyContainerID                       "kUSBContainerID"
#define kUSBHostDevicePropertyFailedRequestedPower              "kUSBFailedRequestedPower"
#define kUSBHostDevicePropertyResumeRecoveryTime                "kUSBResumeRecoveryTime"
#define kUSBHostDevicePropertyPreferredConfiguration            "kUSBPreferredConfiguration"
#define kUSBHostDevicePropertyPreferredRecoveryConfiguration    "kUSBPreferredRecoveryConfiguration"
#define kUSBHostDevicePropertyCurrentConfiguration              "kUSBCurrentConfiguration"
#define kUSBHostDevicePropertyRemoteWakeOverride                "kUSBRemoteWakeOverride"
#define kUSBHostDevicePropertyConfigurationDescriptorOverride   "kUSBConfigurationDescriptorOverride"
#define kUSBHostDevicePropertyDeviceDescriptorOverride          "kUSBDeviceDescriptorOverride"
#define kUSBHostDevicePropertyConfigurationCurrentOverride      "kUSBConfigurationCurrentOverride"
#define kUSBHostDevicePropertyResetDurationOverride             "kUSBResetDurationOverride"
#define kUSBHostDevicePropertyDesiredChargingCurrent            "kUSBDesiredChargingCurrent"
#define kUSBHostDevicePropertyDescriptorOverride                "kUSBDescriptorOverride"
#define kUSBHostDescriptorOverrideVendorStringIndex             "UsbDescriptorOverrideVendorStringIndex"
#define kUSBHostDescriptorOverrideProductStringIndex            "UsbDescriptorOverrideProductStringIndex"
#define kUSBHostDescriptorOverrideSerialNumberStringIndex       "UsbDescriptorOverrideSerialNumberStringIndex"
#define kUSBHostDevicePropertyDeviceECID                        "kUSBDeviceECID"
#define kUSBHostDevicePropertyEnableLPM                         "kUSBHostDeviceEnableLPM"
#define kUSBHostDevicePropertyDisablePortLPM                    "kUSBHostDeviceDisablePortLPM"          // Disable port initiated LPM for this device
#define kUSBHostDevicePropertyStreamsSupported                  "UsbStreamsSupported"                   // Default kOSBooleanTrue.  OSBoolean indicating if streaming endpoints are supported

#define kUSBHostBillboardDevicePropertyNumberOfAlternateModes   "bNumberOfAlternateModes"
#define kUSBHostBillboardDevicePropertyPreferredAlternateMode   "bPreferredAlternateMode"
#define kUSBHostBillboardDevicePropertyVCONNPower               "VCONNPower"
#define kUSBHostBillboardDevicePropertyConfigured               "bmConfigured"
#define kUSBHostBillboardDevicePropertyAdditionalFailureInfo    "bAdditonalFailureInfo"
#define kUSBHostBillboardDevicePropertyBcdVersion               "BcdVersion"
#define kUSBHostBillboardDevicePropertySVID                     "wSVID"
#define kUSBHostBillboardDevicePropertyAlternateMode            "bAlternateMode"
#define kUSBHostBillboardDevicePropertyAlternateModeStringIndex "iAlternateModeString"
#define kUSBHostBillboardDevicePropertyAlternateModeString      "AlternateModeString"
#define kUSBHostBillboardDevicePropertyAddtionalInfoURLIndex    "iAddtionalInfoURL"
#define kUSBHostBillboardDevicePropertyAddtionalInfoURL         "AddtionalInfoURL"
#define kUSBHostBillboardDevicePropertydwAlternateModeVdo       "dwAlternateModeVdo"

#define kUSBHostInterfacePropertyAlternateSetting               "bAlternateSetting"

#define kUSBHostPortPropertyStatus                              "port-status"
#define kUSBHostPortPropertyOvercurrent                         "UsbHostPortOvercurrent"
#define kUSBHostPortPropertyPortNumber                          "port"
#define kUSBHostPortPropertyRemovable                           "removable"
#define kUSBHostPortPropertyTestMode                            "kUSBTestMode"
#define kUSBHostPortPropertyUsb3ComplianceMode                  "kUSBHostPortPropertyUsb3ComplianceMode"
#define kUSBHostPortPropertySimulateInterrupt                   "kUSBSimulateInterrupt"
#define kUSBHostPortPropertyBusCurrentAllocation                "kUSBBusCurrentAllocation"
#define kUSBHostPortPropertyBusCurrentSleepAllocation           "kUSBBusCurrentSleepAllocation"
#define kUSBHostPortPropertyConnectable                         "UsbConnectable"
#define kUSBHostPortPropertyConnectorType                       "UsbConnector"
#define kUSBHostPortPropertyMux                                 "UsbMux"
#define kUSBHostPortPropertyCompanionIndex                      "kUSBCompanionIndex"
#define kUSBHostPortPropertyDisconnectInterval                  "kUSBDisconnectInterval"
#define kUSBHostPortPropertyUsbCPortNumber                      "UsbCPortNumber"
#define kUSBHostPortPropertyCompanionPortNumber                 "UsbCompanionPortNumber"                // OSData  key to set/get the port number of the companion port
#define kUSBHostPortPropertyPowerSource                         "UsbPowerSource"
#define kUSBHostPortPropertyUSB3Mode                            "Usb3Mode"
#define kUSBHostPortPropertyExternalDeviceResetController       "kUSBHostPortExternalDeviceResetController"
#define kUSBHostPortPropertyExternalDevicePowerController       "kUSBHostPortExternalDevicePowerController"
#define kUSBHostPortPropertyCardReader                          "kUSBHostPortPropertyCardReader"
#define kUSBHostPortPropertyCardReaderValidateDescriptors       "kUSBHostPortPropertyCardReaderValidateDescriptors"

#define kUSBHostHubPropertyPowerSupply                          "kUSBHubPowerSupply"                    // OSNumber mA available for downstream ports, 0 for bus-powered
#define kUSBHostHubPropertyIdlePolicy                           "kUSBHubIdlePolicy"                     // OSNumber ms to be used as device idle policy
#define kUSBHostHubPropertyStartupDelay                         "kUSBHubStartupDelay"                   // OSNumber ms delay before creating downstream ports
#define kUSBHostHubPropertyPortSequenceDelay                    "kUSBHubPortSequenceDelay"              // OSNumber ms delay between port creation
#define kUSBHostHubPropertyHubPowerSupplyType                   "kUSBHubPowerSupplyType"                // OSNumber for tPowerSupply hub is, 2 for bus-powered, 1 for self
#define kUSBHostHubPropertyGlobalSuspendSupported               "UsbHubGlobalSuspendSupported"          // OSBoolean false to attempt selective suspend (legacy behavior)

#define kUSBHostControllerPropertyIsochronousRequiresContiguous "kUSBIsochronousRequiresContiguous"
#define kUSBHostControllerPropertySleepSupported                "kUSBSleepSupported"
#define kUSBHostControllerPropertyRTD3Supported                 "UsbRTD3Supported"
#define kUSBHostControllerPropertyMuxEnabled                    "kUSBMuxEnabled"
#define kUSBHostControllerPropertyCompanion                     "kUSBCompanion"                         // OSBoolean false to disable all companion controllers
#define kUSBHostControllerPropertyLowSpeedCompanion             "kUSBLowSpeedCompanion"                 // OSBoolean false to disable low-speed companion controller
#define kUSBHostControllerPropertyFullSpeedCompanion            "kUSBFullSpeedCompanion"                // OSBoolean false to disable full-speed companion controller
#define kUSBHostControllerPropertyHighSpeedCompanion            "kUSBHighSpeedCompanion"                // OSBoolean false to disable high-speed companion controller
#define kUSBHostControllerPropertySuperSpeedCompanion           "kUSBSuperSpeedCompanion"               // OSBoolean false to disable superspeed companion controller
#define kUSBHostControllerPropertyRevision                      "Revision"                              // OSData    Major/minor revision number of controller
#define kUSBHostControllerPropertyCompanionControllerName       "UsbCompanionControllerName"            // OSString  key to set/get the name of the service, i.e. companion controller dictionary.
#define kUSBHostControllerPropertyDisableUSB3LPM                "kUSBHostControllerDisableUSB3LPM"      // OSBoolean true to disable USB3 LPM on a given controller
#define kUSBHostControllerPropertyDisableUSB2LPM                "kUSBHostControllerDisableUSB2LPM"      // OSBoolean true to disable USB2 LPM on a given controller
#define kUSBHostControllerPropertyDisableWakeSources            "UsbHostControllerDisableWakeSources"   // OSBoolean true to disable connect/disconnect/overcurrent wake sources
#define kUSBHostControllerPropertyPersistFullSpeedIsochronous   "UsbHostControllerPersistFullSpeedIsochronous"  // OSBoolean true to reduce commands related to full-speed isochronous endpoints
#define kUSBHostControllerPropertyDeferRegisterService          "UsbHostControllerDeferRegisterService" // OSBoolean true to defer registerService call by base class during start
#define kUSBHostControllerPropertyStreamPolicy                  "UsbHostControllerStreamPolicy"         // OSNumber containing tUSBStreamPolicy
#define kUSBHostControllerPropertyTierLimit                     "UsbHostControllerTierLimit"            // OSNumber containing the number of tiers supported by this controller (See USB 2.0 § 4.1.1)

#define kIOUSBHostDeviceClassName                               "IOUSBHostDevice"
#define kIOUSBHostInterfaceClassName                            "IOUSBHostInterface"

// for IOUSBLib compatibility
#define kUSBHostDevicePropertyAddress                           "kUSBAddress"
#define kUSBHostDevicePropertyManufacturerStringIndex           "iManufacturer"
#define kUSBHostDevicePropertySerialNumberStringIndex           "iSerialNumber"
#define kUSBHostDevicePropertyProductStringIndex                "iProduct"
#define kUSBHostDevicePropertyProductString                     "kUSBProductString"
#define kUSBHostDevicePropertyNumConfigs                        "bNumConfigurations"
#define kUSBHostDevicePropertyMaxPacketSize                     "bMaxPacketSize0"
#define kUSBHostDevicePropertyStandardVersion                   "bcdUSB"

#define kUSBHostInterfacePropertyStringIndex                    "iInterface"
#define kUSBHostInterfacePropertyString                         "kUSBString"
#define kUSBHostInterfacePropertyNumEndpoints                   "bNumEndpoints"

// Legacy power properties
#define kAppleMaxPortCurrent                                    "AAPL,current-available"
#define kAppleCurrentExtra                                      "AAPL,current-extra"
#define kAppleMaxPortCurrentInSleep                             "AAPL,max-port-current-in-sleep"
#define kAppleCurrentExtraInSleep                               "AAPL,current-extra-in-sleep"
#define kAppleExternalConnectorBitmap                           "AAPL,ExternalConnectorBitmap"

#endif /* IOUSBHostFamily_IOUSBHostFamilyDefinitions_h */
