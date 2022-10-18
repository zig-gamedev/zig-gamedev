/*
 * Copyright (c) 2018-2019 Apple Inc. All rights reserved.
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

#ifndef AppleUSBDescriptorParsing_h
#define AppleUSBDescriptorParsing_h

#include <TargetConditionals.h>
#if TARGET_OS_DRIVERKIT
#include <USBDriverKit/AppleUSBDefinitions.h>
#include <USBDriverKit/IOUSBHostFamilyDefinitions.h>
#else
#include <IOKit/usb/AppleUSBDefinitions.h>
#include <IOKit/usb/IOUSBHostFamilyDefinitions.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

    /*!
 * @brief       Get the next descriptor in a configuration descriptor
 * @discussion  This method will advance currentDescriptor by its bLength, and validate that the new descriptor fits withing the bounds of configurationDescriptor.  Using NULL for currentDescriptor will return the first descriptor after the configuration descriptor.
 * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
 * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
 * @return      Descriptor pointer, or NULL if no descriptor can be returned
 */
const IOUSBDescriptorHeader* IOUSBGetNextDescriptor(const IOUSBConfigurationDescriptor* configurationDescriptor, const IOUSBDescriptorHeader* currentDescriptor);

/*!
 * @brief       Find the next descriptor matching a given type within a configuration descriptor
 * @discussion  This method uses getNextDescriptor, and further validates that the returned descriptor's bDescriptorType field matches the type parameter.
 * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
 * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
 * @param       type tDescriptorType representing the descriptor type to find
 * @return      Descriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDescriptorHeader* IOUSBGetNextDescriptorWithType(const IOUSBConfigurationDescriptor* configurationDescriptor, const IOUSBDescriptorHeader* currentDescriptor, const uint8_t type);

/*!
 * @brief       Get the next descriptor in a configuration descriptor that belongs to another container descriptor
 * @discussion  This method uses getNextDescriptor, but will return NULL if another descriptor is found whose bDescriptorType field matches the value used for parentDescriptor's bDescriptorType.  Using NULL for currentDescriptor will return the first descriptor after parentDescriptor.
 * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
 * @param       parentDescriptor A descriptor pointer within the bounds of configurationDescriptor
 * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
 * @return      Descriptor pointer, or NULL if no descriptor can be returned
 */
const IOUSBDescriptorHeader* IOUSBGetNextAssociatedDescriptor(const IOUSBConfigurationDescriptor* configurationDescriptor, const IOUSBDescriptorHeader* parentDescriptor, const IOUSBDescriptorHeader* currentDescriptor);

/*!
 * @brief       Find the next descriptor matching a given type within a configuration descriptor that belongs to another container descriptor
 * @discussion  This method uses getNextAssociatedDescriptor, and further validates that the returned descriptor's bDescriptorType field matches the type passed parameter.
 * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
 * @param       parentDescriptor A descriptor pointer within the bounds of configurationDescriptor
 * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
 * @param       type tDescriptorType representing the descriptor type to find
 * @return      Descriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDescriptorHeader* IOUSBGetNextAssociatedDescriptorWithType(const IOUSBConfigurationDescriptor* configurationDescriptor, const IOUSBDescriptorHeader* parentDescriptor, const IOUSBDescriptorHeader* currentDescriptor, const uint8_t type);

/*!
 * @brief       Find the next interface association descriptor in a configuration descriptor
 * @discussion  This method uses getNextDescriptorWithType to fetch the next interface association descriptor
 * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
 * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
 * @return      InterfaceAssociationDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBInterfaceAssociationDescriptor* IOUSBGetNextInterfaceAssociationDescriptor(const IOUSBConfigurationDescriptor* configurationDescriptor, const IOUSBDescriptorHeader* currentDescriptor);

/*!
 * @brief       Find the next interface descriptor in a configuration descriptor
 * @discussion  This method uses getNextDescriptorWithType to fetch the next interface descriptor
 * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
 * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
 * @return      InterfaceDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBInterfaceDescriptor* IOUSBGetNextInterfaceDescriptor(const IOUSBConfigurationDescriptor* configurationDescriptor, const IOUSBDescriptorHeader* currentDescriptor);

/*!
 * @brief       Find the next endpoint descriptor associated with an interface descriptor
 * @discussion  This method uses getNextAssociatedDescriptorWithType to fetch the next endpoint descriptor associated with a specific interface descriptor
 * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
 * @param       interfaceDescriptor An interface descriptor within the bounds of configurationDescriptor
 * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
 * @return      EndpointDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBEndpointDescriptor* IOUSBGetNextEndpointDescriptor(const IOUSBConfigurationDescriptor* configurationDescriptor, const IOUSBInterfaceDescriptor* interfaceDescriptor, const IOUSBDescriptorHeader* currentDescriptor);

/*!
 * @brief       Get the next device capability descriptor in a BOS descriptor
 * @discussion  This method will advance currentDescriptor by its bLength, and validate that the new descriptor fits withing the bounds of bosDescriptor.  Using NULL for currentDescriptor will return the first descriptor after the BOS descriptor.
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @param       currentDescriptor A descriptor pointer within the bounds of bosDescriptor, or NULL
 * @return      DeviceCapabilityDescriptor pointer, or NULL if no descriptor can be returned
 */
const IOUSBDeviceCapabilityDescriptorHeader* IOUSBGetNextCapabilityDescriptor(const IOUSBBOSDescriptor* bosDescriptor, const IOUSBDeviceCapabilityDescriptorHeader* currentDescriptor);

/*!
 * @brief       Find the next descriptor matching a given type within a BOS descriptor
 * @discussion  This method uses getNextCapabilityDescriptor, and further validates that the returned descriptor's bDevCapabilityType field matches the type parameter.
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @param       currentDescriptor A descriptor pointer within the bounds of bosDescriptor, or NULL
 * @param       type tDeviceCapabilityType representing the descriptor type to find
 * @return      DeviceCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDeviceCapabilityDescriptorHeader* IOUSBGetNextCapabilityDescriptorWithType(const IOUSBBOSDescriptor* bosDescriptor, const IOUSBDeviceCapabilityDescriptorHeader* currentDescriptor, const uint8_t type);

/*!
 * @brief       Find the first USB20ExtensionCapabilityDescriptor in a BOS descriptor
 * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first USB20ExtensionCapabilityDescriptor
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @return      USB20ExtensionCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDeviceCapabilityUSB2Extension* IOUSBGetUSB20ExtensionDeviceCapabilityDescriptor(const IOUSBBOSDescriptor* bosDescriptor);

/*!
 * @brief       Find the first SuperSpeedUSBDeviceCapabilityDescriptor in a BOS descriptor
 * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first SuperSpeedUSBDeviceCapabilityDescriptor
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @return      SuperSpeedUSBDeviceCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDeviceCapabilitySuperSpeedUSB* IOUSBGetSuperSpeedDeviceCapabilityDescriptor(const IOUSBBOSDescriptor* bosDescriptor);

/*!
 * @brief       Find the first SuperSpeedPlusUSBDeviceCapabilityDescriptor in a BOS descriptor
 * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first SuperSpeedPlusUSBDeviceCapabilityDescriptor
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @return      SuperSpeedPlusUSBDeviceCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDeviceCapabilitySuperSpeedPlusUSB* IOUSBGetSuperSpeedPlusDeviceCapabilityDescriptor(const IOUSBBOSDescriptor* bosDescriptor);

/*!
 * @brief       Find the first ContainerIDCapabilityDescriptor in a BOS descriptor
 * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first ContainerIDCapabilityDescriptor
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @return      ContainerIDCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDeviceCapabilityContainerID* IOUSBGetContainerIDDescriptor(const IOUSBBOSDescriptor* bosDescriptor);

/*!
 * @brief       Find the first PlatformCapabilityDescriptor in a BOS descriptor
 * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first PlatformCapabilityDescriptor
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @return      PlatformCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBPlatformCapabilityDescriptor* IOUSBGetPlatformCapabilityDescriptor(const IOUSBBOSDescriptor* bosDescriptor);

/*!
 * @brief       Find the first BillboardCapabilityDescriptor in a BOS descriptor
 * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first BillboardCapabilityDescriptor
 * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
 * @return      BillboardCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
 */
const IOUSBDeviceCapabilityBillboard* IOUSBGetBillboardDescriptor(const IOUSBBOSDescriptor* bosDescriptor);


#pragma mark Endpoint descriptor parsing
/*!
 * @brief       Extract the direction of an endpoint from an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor to determine its transfer direction
 * @param       descriptor The descriptor to parse
 * @return      tEndpointDirection indicating the direction found.  Control endpoints return tEndpointDirection.
 */
uint8_t IOUSBGetEndpointDirection(const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the direction and number of an endpoint from an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor to determine its address
 * @param       descriptor The descriptor to parse
 * @return      uint8_t representing direction and endpoint number
 */
uint8_t IOUSBGetEndpointAddress(const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the number of an endpoint from an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor to determine its number, excluding direction
 * @param       descriptor The descriptor to parse
 * @return      uint8_t representing endpoint number
 */
uint8_t IOUSBGetEndpointNumber(const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the type of an endpoint from an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor to determine its type
 * @param       descriptor The descriptor to parse
 * @return      tEndpointType indicating the type found.
 */
uint8_t IOUSBGetEndpointType(const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the max packet size from an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor to determine its max packet size, which does not take into account mult or burst factors.
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The descriptor to parse
 * @return      uint16_t The max packet size in bytes
 */
uint16_t IOUSBGetEndpointMaxPacketSize(uint32_t usbDeviceSpeed, const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the burst size from endpoint descriptors
 * @discussion  This method parses endpoint descriptors to determine burst size, which includes mult and burst factors as applicable.  SuperSpeed Plus isochronous endpoints will return the dwBytesPerInterval field from the SuperSpeedPlusIsochronousEndpointCompanionDescriptor parameter.
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The EndpointDescriptor to parse
 * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse, or NULL
 * @param       sspCompanionDescriptor The SuperSpeedPlusIsochronousEndpointCompanionDescriptor to parse, or NULL
 * @return      uint32_t The burst size in bytes
 */
uint32_t IOUSBGetEndpointBurstSize(uint32_t usbDeviceSpeed,
                                   const IOUSBEndpointDescriptor* descriptor,
                                   const IOUSBSuperSpeedEndpointCompanionDescriptor* companionDescriptor,
                                   const IOUSBSuperSpeedPlusIsochronousEndpointCompanionDescriptor* sspCompanionDescriptor);

/*!
 * @brief       Extract the mult count from endpoint descriptors
 * @discussion  This method parses endpoint descriptors to determine mult
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The EndpointDescriptor to parse
 * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse, or NULL
 * @param       sspCompanionDescriptor The SuperSpeedPlusIsochronousEndpointCompanionDescriptor to parse, or NULL
 * @return      uint8_t The mult count
 */
uint8_t IOUSBGetEndpointMult(uint32_t usbDeviceSpeed,
                             const IOUSBEndpointDescriptor* descriptor,
                             const IOUSBSuperSpeedEndpointCompanionDescriptor* companionDescriptor,
                             const IOUSBSuperSpeedPlusIsochronousEndpointCompanionDescriptor* sspCompanionDescriptor);

/*!
 * @brief       Extract the interval of an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor and returns the service interval as n in (2^(n-1)) microframes
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The EndpointDescriptor to parse
 * @return      uint32_t Encoded endpoint interval
 */
uint32_t IOUSBGetEndpointIntervalEncodedMicroframes(uint32_t usbDeviceSpeed, const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the interval of an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor and returns the service interval in microframes
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The EndpointDescriptor to parse
 * @return      uint32_t Endpoint interval in microframes
 */
uint32_t IOUSBGetEndpointIntervalMicroframes(uint32_t usbDeviceSpeed, const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the interval of an endpoint descriptor
 * @discussion  This method parses an endpoint descriptor and returns the service interval in frames
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The EndpointDescriptor to parse
 * @return      uint32_t Endpoint interval in frames
 */
uint32_t IOUSBGetEndpointIntervalFrames(uint32_t usbDeviceSpeed, const IOUSBEndpointDescriptor* descriptor);

/*!
 * @brief       Extract the number of streams supported by an endpoint
 * @discussion  This method parses endpoint descriptors and returns the number of streams supported as n in (2^n)
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The EndpointDescriptor to parse
 * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse
 * @return      uint32_t Encoded number of streams
 */
uint32_t IOUSBGetEndpointMaxStreamsEncoded(uint32_t usbDeviceSpeed, const IOUSBEndpointDescriptor* descriptor, const IOUSBSuperSpeedEndpointCompanionDescriptor* companionDescriptor);

/*!
 * @brief       Extract the number of streams supported by an endpoint
 * @discussion  This method parses endpoint descriptors and returns the number of streams supported
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The EndpointDescriptor to parse
 * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse
 * @return      uint32_t Number of streams
 */
uint32_t IOUSBGetEndpointMaxStreams(uint32_t usbDeviceSpeed, const IOUSBEndpointDescriptor* descriptor, const IOUSBSuperSpeedEndpointCompanionDescriptor* companionDescriptor);

/*!
 * @brief       Extract the maximum bus current required by a configuration descriptor
 * @discussion  This method parses a configuration descriptor and returns the number of milliamps required to power the device
 * @param       usbDeviceSpeed The operational speed of the device
 * @param       descriptor The ConfigurationDescriptor to parse
 * @return      uint32_t milliamps required
 */
uint32_t IOUSBGetConfigurationMaxPowerMilliAmps(uint32_t usbDeviceSpeed, const IOUSBConfigurationDescriptor* descriptor);

#ifdef  __cplusplus
}
#endif

#endif /* AppleUSBDescriptorParsing_h */
