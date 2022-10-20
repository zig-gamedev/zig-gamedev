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

/*! @header     StandardUSB.h
    @brief      StandardUSB defines structures and constants to reflect the USB specification
 */

#ifndef IOUSBHostFamily_StandardUSB_h
#define IOUSBHostFamily_StandardUSB_h

#include <IOKit/IOTypes.h>
#include <IOKit/IOReturn.h>
#include <libkern/OSByteOrder.h>
#include <IOKit/usb/AppleUSBDefinitions.h>


#define StandardUSBBit(bit)                     IOUSBBit(bit)
#define StandardUSBBitRange(start, end)         IOUSBBitRange(start, end)
#define StandardUSBBitRange64(start, end)       IOUSBBitRange64(start, end)
#define StandardUSBBitRangePhase(start, end)    IOUSBBitRangePhase(start, end)

#define kUSB30Bitrate5Gbps  kIOUSB30Bitrate5Gbps
#define kUSB30Bitrate10Gbps kIOUSB30Bitrate10Gbps
#define kUSB32Bitrate20Gbps kIOUSB32Bitrate20Gbps

#ifdef __cplusplus

#pragma mark StandardUSB namespace
namespace StandardUSB
{
    
#endif // __cplusplus
    
#pragma mark Descriptor definitions
    /*!
     * @enum        tDescriptorType
     * @namespace   StandardUSB
     * @brief       Descriptor types defined by USB 2.0 Table 9-5 and USB 3.0 Table 9-6
     */
    enum tDescriptorType
    {
        kDescriptorTypeDevice = kIOUSBDescriptorTypeDevice,
        kDescriptorTypeConfiguration = kIOUSBDescriptorTypeConfiguration,
        kDescriptorTypeString = kIOUSBDescriptorTypeString,
        kDescriptorTypeInterface = kIOUSBDescriptorTypeInterface,
        kDescriptorTypeEndpoint = kIOUSBDescriptorTypeEndpoint,
        kDescriptorTypeDeviceQualifier = kIOUSBDescriptorTypeDeviceQualifier,
        kDescriptorTypeOtherSpeedConfiguration = kIOUSBDescriptorTypeOtherSpeedConfiguration,
        kDescriptorTypeInterfacePower = kIOUSBDescriptorTypeInterfacePower,
        kDescriptorTypeOTG = kIOUSBDescriptorTypeOTG,
        kDescriptorTypeDebug = kIOUSBDescriptorTypeDebug,
        kDescriptorTypeInterfaceAssociation = kIOUSBDescriptorTypeInterfaceAssociation,
        kDescriptorTypeBOS = kIOUSBDescriptorTypeBOS,
        kDescriptorTypeDeviceCapability = kIOUSBDescriptorTypeDeviceCapability,
        kDescriptorTypeHub = kIOUSBDescriptorTypeHub,
        kDescriptorTypeSuperSpeedHub = kIOUSBDescriptorTypeSuperSpeedHub,
        kDescriptorTypeSuperSpeedUSBEndpointCompanion = kIOUSBDescriptorTypeSuperSpeedUSBEndpointCompanion,
        kDescriptorTypeSuperSpeedPlusIsochronousEndpointCompanion = kIOUSBDescriptorTypeSuperSpeedPlusIsochronousEndpointCompanion
    };
    
    typedef enum tDescriptorType tDescriptorType;

    /*!
     * @enum        tDescriptorSize
     * @namespace   StandardUSB
     * @brief       Size in bytes for descriptor structures
     */
    enum tDescriptorSize
    {
        kDescriptorSize = kIOUSBDescriptorHeaderSize,
        kDescriptorSizeDevice = kIOUSBDescriptorSizeDevice,
        kDescriptorSizeConfiguration = kIOUSBDescriptorSizeConfiguration,
        kDescriptorSizeInterface = kIOUSBDescriptorSizeInterface,
        kDescriptorSizeEndpoint = kIOUSBDescriptorSizeEndpoint,
        kDescriptorSizeStringMinimum = kDescriptorSize,
        kDescriptorSizeStringMaximum = kIOUSBDescriptorSizeStringMaximum,
        kDescriptorSizeDeviceQualifier = kIOUSBDescriptorSizeDeviceQualifier,
        kDescriptorSizeInterfaceAssociation = kIOUSBDescriptorSizeInterfaceAssociation,
        kDescriptorSizeBOS = kIOUSBDescriptorSizeBOS,
        kDescriptorSizeDeviceCapability = kIOUSBDescriptorSizeDeviceCapability,
        kDescriptorSizeUSB20ExtensionCapability = kIOUSBDescriptorSizeUSB20ExtensionCapability,
        kDescriptorSizeSuperSpeedUSBDeviceCapability = kIOUSBDescriptorSizeSuperSpeedUSBDeviceCapability,
        kDescriptorSizeContainerIDCapability = kIOUSBDescriptorSizeContainerIDCapability,
        kDescriptorSizeHubMinimum = kIOUSBDescriptorSizeHubMinimum,
        kDescriptorSizeHubMaximum = kIOUSBDescriptorSizeHubMaximum,
        kDescriptorSizeSuperSpeedHub = kIOUSBDescriptorSizeSuperSpeedHub,
        kDescriptorSizeSuperSpeedUSBEndpointCompanion = kIOUSBDescriptorSizeSuperSpeedUSBEndpointCompanion,
        kDescriptorSizeSuperSpeedPlusIsochronousEndpointCompanion = kIOUSBDescriptorSizeSuperSpeedPlusIsochronousEndpointCompanion,
        kDescriptorSizeBillboardDeviceMinimum = kIOUSBDescriptorSizeBillboardDeviceMinimum,
        kDescriptorSizeBillboardDeviceMaximum = kIOUSBDescriptorSizeBillboardDeviceMaximum,
        kDescriptorSizePlatformECIDCapability = kIOUSBDescriptorSizePlatformECIDCapability,
        kDescriptorSizePlatformCapability     = kIOUSBDescriptorSizePlatformCapability
    };
    
    typedef enum tDescriptorSize tDescriptorSize;

    /*!
     * @struct      Descriptor
     * @namespace   StandardUSB
     * @brief       Base descriptor defined by USB 2.0 9.5
     * @discussion  StandardUSB.h declares structs to represent a variety of USB standard descriptors.  Each of thes structs is derived from the Descriptor base struct, and can therefore be passed to methods accepting a Descriptor pointer without explicit casting.  For the C language, each struct is defined independently to include the bLength and bDescriptorType fields expected for all USB descriptors.
     */
    struct Descriptor
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
    } __attribute__((packed));

    typedef struct Descriptor Descriptor;
    
    
#ifdef __cplusplus
    // USB 2.0 9.6.1: Device
    struct DeviceDescriptor : public Descriptor
    {
#else
    struct DeviceDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint16_t    bcdUSB;
        uint8_t     bDeviceClass;
        uint8_t     bDeviceSubClass;
        uint8_t     bDeviceProtocol;
        uint8_t     bMaxPacketSize0;
        uint16_t    idVendor;
        uint16_t    idProduct;
        uint16_t    bcdDevice;
        uint8_t     iManufacturer;
        uint8_t     iProduct;
        uint8_t     iSerialNumber;
        uint8_t     bNumConfigurations;
    } __attribute__((packed));
        
    typedef struct DeviceDescriptor DeviceDescriptor;

    
#ifdef __cplusplus
    // USB 2.0 9.6.2: Device Qualifier
    struct DeviceQualifierDescriptor : public Descriptor
    {
#else
    struct DeviceQualifierDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint16_t    bcdUSB;
        uint8_t     bDeviceClass;
        uint8_t     bDeviceSubClass;
        uint8_t     bDeviceProtocol;
        uint8_t     bMaxPacketSize0;
        uint8_t     bNumConfigurations;
        uint8_t     bReserved;
    } __attribute__((packed));
    
    typedef struct DeviceQualifierDescriptor DeviceQualifierDescriptor;
        
        
#ifdef __cplusplus
    // USB 2.0 9.6.3: Configuration
    struct ConfigurationDescriptor : public Descriptor
    {
#else
    struct ConfigurationDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint16_t    wTotalLength;
        uint8_t     bNumInterfaces;
        uint8_t     bConfigurationValue;
        uint8_t     iConfiguration;
        uint8_t     bmAttributes;
        uint8_t     bMaxPower;
    } __attribute__((packed));
    
    typedef struct ConfigurationDescriptor ConfigurationDescriptor;
        
    
    enum
    {
        kConfigurationDescriptorAttributeRemoteWakeCapable  = kIOUSBConfigurationDescriptorAttributeRemoteWakeCapable,
        kConfigurationDescriptorAttributeSelfPowered        = kIOUSBConfigurationDescriptorAttributeSelfPowered
    };
    
        
#ifdef __cplusplus
    // USB 2.0 9.6.5: Interface
    struct InterfaceDescriptor : public Descriptor
    {
#else
    struct InterfaceDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint8_t     bInterfaceNumber;
        uint8_t     bAlternateSetting;
        uint8_t     bNumEndpoints;
        uint8_t     bInterfaceClass;
        uint8_t     bInterfaceSubClass;
        uint8_t     bInterfaceProtocol;
        uint8_t     iInterface;
    } __attribute__((packed));
    
    typedef struct InterfaceDescriptor InterfaceDescriptor;
        
    
#ifdef __cplusplus
    // USB 2.0 9.6.6: Endpoint
    struct EndpointDescriptor : public Descriptor
    {
#else
    struct EndpointDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint8_t     bEndpointAddress;
        uint8_t     bmAttributes;
        uint16_t    wMaxPacketSize;
        uint8_t     bInterval;
    } __attribute__((packed));
        
    typedef struct EndpointDescriptor EndpointDescriptor;
    
    enum
    {
        kEndpointDescriptorNumber                  = kIOUSBEndpointDescriptorNumber,
        kEndpointDescriptorEndpointAddressReserved = kIOUSBEndpointDescriptorEndpointAddressReserved,
        kEndpointDescriptorDirection               = kIOUSBEndpointDescriptorDirection,
        kEndpointDescriptorDirectionPhase          = kIOUSBEndpointDescriptorDirectionPhase,
        kEndpointDescriptorDirectionOut            = kIOUSBEndpointDescriptorDirectionOut,
        kEndpointDescriptorDirectionIn             = kIOUSBEndpointDescriptorDirectionIn,
        
        kEndpointDescriptorTransferType                    = kIOUSBEndpointDescriptorTransferType,
        kEndpointDescriptorTransferTypePhase               = kIOUSBEndpointDescriptorTransferTypePhase,
        kEndpointDescriptorTransferTypeControl             = kIOUSBEndpointDescriptorTransferTypeControl,
        kEndpointDescriptorTransferTypeIsochronous         = kIOUSBEndpointDescriptorTransferTypeIsochronous,
        kEndpointDescriptorTransferTypeBulk                = kIOUSBEndpointDescriptorTransferTypeBulk,
        kEndpointDescriptorTransferTypeInterrupt           = kIOUSBEndpointDescriptorTransferTypeInterrupt,
        kEndpointDescriptorSynchronizationType             = kIOUSBEndpointDescriptorSynchronizationType,
        kEndpointDescriptorSynchronizationTypePhase        = kIOUSBEndpointDescriptorSynchronizationTypePhase,
        kEndpointDescriptorSynchronizationTypeNone         = kIOUSBEndpointDescriptorSynchronizationTypeNone,
        kEndpointDescriptorSynchronizationTypeAsynchronous = kIOUSBEndpointDescriptorSynchronizationTypeAsynchronous,
        kEndpointDescriptorSynchronizationTypeAdaptive     = kIOUSBEndpointDescriptorSynchronizationTypeAdaptive,
        kEndpointDescriptorSynchronizationTypeSynchronous  = kIOUSBEndpointDescriptorSynchronizationTypeSynchronous,
        kEndpointDescriptorUsageType                       = kIOUSBEndpointDescriptorUsageType,
        kEndpointDescriptorUsageTypePhase                  = kIOUSBEndpointDescriptorUsageTypePhase,
        kEndpointDescriptorUsageTypeInterruptPeriodic      = kIOUSBEndpointDescriptorUsageTypeInterruptPeriodic,
        kEndpointDescriptorUsageTypeInterruptNotification  = kIOUSBEndpointDescriptorUsageTypeInterruptNotification,
        kEndpointDescriptorUsageTypeInterruptReserved1     = kIOUSBEndpointDescriptorUsageTypeInterruptReserved1,
        kEndpointDescriptorUsageTypeInterruptReserved2     = kIOUSBEndpointDescriptorUsageTypeInterruptReserved2,
        kEndpointDescriptorUsageTypeIsocData               = kIOUSBEndpointDescriptorUsageTypeIsocData,
        kEndpointDescriptorUsageTypeIsocFeedback           = kIOUSBEndpointDescriptorUsageTypeIsocFeedback,
        kEndpointDescriptorUsageTypeIsocImplicit           = kIOUSBEndpointDescriptorUsageTypeIsocImplicit,
        kEndpointDescriptorUsageTypeIsocReserved           = kIOUSBEndpointDescriptorUsageTypeIsocReserved,
        
        kEndpointDescriptorPacketSize          = kIOUSBEndpointDescriptorPacketSize,
        kEndpointDescriptorPacketSizePhase     = kIOUSBEndpointDescriptorPacketSizePhase,
        kEndpointDescriptorPacketSizeMult      = kIOUSBEndpointDescriptorPacketSizeMult,
        kEndpointDescriptorPacketSizeMultPhase = kIOUSBEndpointDescriptorPacketSizeMultPhase,
        kEndpointDescriptorReserved            = kIOUSBEndpointDescriptorReserved,
        kEndpointDescriptorReservedPhase       = kIOUSBEndpointDescriptorReservedPhase
    };
    
    // USB Language Identifiers 1.0
    enum tLanguageID
    {
        kLanguageIDEnglishUS = kIOUSBLanguageIDEnglishUS
    };
    
    typedef enum tLanguageID tLanguageID;
        
        
#ifdef __cplusplus
    // USB 2.0 9.6.7: String
    struct StringDescriptor : public Descriptor
    {
#else
    struct StringDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint8_t     bString[1];
    } __attribute__((packed));
        
    typedef struct StringDescriptor StringDescriptor;
    
    
    enum tDeviceCapabilityType
    {
        // USB 3.0 Table 9-13
        kDeviceCapabilityTypeWireless               = kIOUSBDeviceCapabilityTypeWireless,
        kDeviceCapabilityTypeUSB20Extension         = kIOUSBDeviceCapabilityTypeUSB20Extension,
        kDeviceCapabilityTypeSuperSpeed             = kIOUSBDeviceCapabilityTypeSuperSpeed,
        kDeviceCapabilityTypeContainerID            = kIOUSBDeviceCapabilityTypeContainerID,
        
        // USB 3.1 Table 9-14
        kDeviceCapabilityTypePlatform               = kIOUSBDeviceCapabilityTypePlatform,
        kDeviceCapabilityTypePowerDelivery          = kIOUSBDeviceCapabilityTypePowerDelivery,
        kDeviceCapabilityTypeBatteryInfo            = kIOUSBDeviceCapabilityTypeBatteryInfo,
        kDeviceCapabilityTypePdConsumerPort         = kIOUSBDeviceCapabilityTypePdConsumerPort,
        kDeviceCapabilityTypePdProviderPort         = kIOUSBDeviceCapabilityTypePdProviderPort,
        kDeviceCapabilityTypeSuperSpeedPlus         = kIOUSBDeviceCapabilityTypeSuperSpeedPlus,
        kDeviceCapabilityTypePrecisionMeasurement   = kIOUSBDeviceCapabilityTypePrecisionMeasurement,
        kDeviceCapabilityTypeWirelessExt            = kIOUSBDeviceCapabilityTypeWirelessExt,
        kDeviceCapabilityTypeBillboard              = kIOUSBDeviceCapabilityTypeBillboard,
        kDeviceCapabilityTypeBillboardAltMode       = kIOUSBDeviceCapabilityTypeBillboardAltMode
    };
    
    typedef enum tDeviceCapabilityType tDeviceCapabilityType;
        
    
#ifdef __cplusplus
    // USB 3.0 9.6.2: Binary Device Object Store (BOS)
    struct BOSDescriptor : public Descriptor
    {
#else
    struct BOSDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint16_t    wTotalLength;
        uint8_t     bNumDeviceCaps;
    } __attribute__((packed));
        
    typedef struct BOSDescriptor BOSDescriptor;


#ifdef __cplusplus
    // USB 3.0 9.6.2: Binary Device Object Store (BOS)
    struct DeviceCapabilityDescriptor : public Descriptor
    {
#else
    struct DeviceCapabilityDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint8_t     bDevCapabilityType;
    } __attribute__((packed));
        
    typedef struct DeviceCapabilityDescriptor DeviceCapabilityDescriptor;
        
    
#ifdef __cplusplus
    // USB 3.0 9.6.2.1: USB 2.0 Extension
    struct USB20ExtensionCapabilityDescriptor : public DeviceCapabilityDescriptor
    {
#else
    struct USB20ExtensionCapabilityDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
        uint8_t     bDevCapabilityType;
#endif
        uint32_t    bmAttributes;
    } __attribute__((packed));
        
    typedef struct USB20ExtensionCapabilityDescriptor USB20ExtensionCapabilityDescriptor;
        
    
    enum
    {
        kUSB20ExtensionCapabilityLPM = kIOUSBUSB20ExtensionCapabilityLPM,
        
        // From USB 2.0 ECN Errata for Link Power Management.
        kUSB20ExtensionCapabilityBESLSupport    = kIOUSBUSB20ExtensionCapabilityBESLSupport,
        kUSB20ExtensionCapabilityBESLValid      = kIOUSBUSB20ExtensionCapabilityBESLValid,
        kUSB20ExtensionCapabilityBESLDValid     = kIOUSBUSB20ExtensionCapabilityBESLDValid,
        kUSB20ExtensionCapabilityBESL           = kIOUSBUSB20ExtensionCapabilityBESL,
        kUSB20ExtensionCapabilityBESLPhase      = kIOUSBUSB20ExtensionCapabilityBESLPhase,
        kUSB20ExtensionCapabilityBESLD          = kIOUSBUSB20ExtensionCapabilityBESLD,
        kUSB20ExtensionCapabilityBESLDPhase     = kIOUSBUSB20ExtensionCapabilityBESLDPhase
    };
    
        
#ifdef __cplusplus
    // USB 3.0 9.6.2.2: SuperSpeed USB Device Capability
    struct SuperSpeedUSBDeviceCapabilityDescriptor : public DeviceCapabilityDescriptor
    {
#else
    struct SuperSpeedUSBDeviceCapabilityDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
        uint8_t     bDevCapabilityType;
#endif
        uint8_t     bmAttributes;
        uint16_t    wSpeedsSupported;
        uint8_t     bFunctionalitySupport;
        uint8_t     bU1DevExitLat;
        uint16_t    bU2DevExitLat;
    } __attribute__((packed));
        
    typedef struct SuperSpeedUSBDeviceCapabilityDescriptor SuperSpeedUSBDeviceCapabilityDescriptor;
        
    
    enum
    {
        kSuperSpeedDeviceCapabilityLTM = kIOUSBSuperSpeedDeviceCapabilityLTM,
        
        kSuperSpeedDeviceCapabilityLowSpeed  = kIOUSBSuperSpeedDeviceCapabilityLowSpeed,
        kSuperSpeedDeviceCapabilityFullSpeed = kIOUSBSuperSpeedDeviceCapabilityFullSpeed,
        kSuperSpeedDeviceCapabilityHighSpeed = kIOUSBSuperSpeedDeviceCapabilityHighSpeed,
        kSuperSpeedDeviceCapability5Gb       = kIOUSBSuperSpeedDeviceCapability5Gb,
        
        kSuperSpeedDeviceCapabilitySupportLowSpeed  = kIOUSBSuperSpeedDeviceCapabilitySupportLowSpeed,
        kSuperSpeedDeviceCapabilitySupportFullSpeed = kIOUSBSuperSpeedDeviceCapabilitySupportFullSpeed,
        kSuperSpeedDeviceCapabilitySupportHighSpeed = kIOUSBSuperSpeedDeviceCapabilitySupportHighSpeed,
        kSuperSpeedDeviceCapabilitySupport5Gb       = kIOUSBSuperSpeedDeviceCapabilitySupport5Gb,
        
        kSuperSpeedDeviceCapabilityU1DevExitLatMax = kIOUSBSuperSpeedDeviceCapabilityU1DevExitLatMax,
        kSuperSpeedDeviceCapabilityU2DevExitLatMax = kIOUSBSuperSpeedDeviceCapabilityU2DevExitLatMax
    };
        
#ifdef __cplusplus
    // USB 3.1 9.6.2.5: SuperSpeedPlus USB Device Capability
    struct SuperSpeedPlusUSBDeviceCapabilityDescriptor : public DeviceCapabilityDescriptor
    {
#else
    struct SuperSpeedPlusUSBDeviceCapabilityDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
        uint8_t     bDevCapabilityType;
#endif
        uint8_t     bReserved;
        uint32_t    bmAttributes;
        uint16_t    wFunctionalitySupport;
        uint16_t    wReserved;
        uint32_t    bmSublinkSpeedAttr[];
        
    } __attribute__((packed));

    typedef struct SuperSpeedPlusUSBDeviceCapabilityDescriptor SuperSpeedPlusUSBDeviceCapabilityDescriptor;
    
    enum
    {
        //bmAttributes
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedAttrCount = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedAttrCount,
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedAttrCountPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedAttrCountPhase,
        
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedIdCount = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedIdCount,
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedIdCountPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedIdCountPhase,
        
        //wFunctionalitySupport
        kSuperSpeedPlusDeviceCapabilitySublinkMinSpeedId = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkMinSpeedId,
        kSuperSpeedPlusDeviceCapabilitySublinkMinSpeedIdPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkMinSpeedIdPhase,
        
        kSuperSpeedPlusDeviceCapabilityReserved = kIOUSBSuperSpeedPlusDeviceCapabilityReserved,
        kSuperSpeedPlusDeviceCapabilityReservedPhase = kIOUSBSuperSpeedPlusDeviceCapabilityReservedPhase,
        
        kSuperSpeedPlusDeviceCapabilityMinRxLaneCount = kIOUSBSuperSpeedPlusDeviceCapabilityMinRxLaneCount,
        kSuperSpeedPlusDeviceCapabilityMinRxLaneCountPhase = kIOUSBSuperSpeedPlusDeviceCapabilityMinRxLaneCountPhase,
        
        kSuperSpeedPlusDeviceCapabilityMinTxLaneCount = kIOUSBSuperSpeedPlusDeviceCapabilityMinTxLaneCount,
        kSuperSpeedPlusDeviceCapabilityMinTxLaneCountPhase = kIOUSBSuperSpeedPlusDeviceCapabilityMinTxLaneCountPhase,
        
        //bmSublinkSpeedAttr
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedId = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedId,
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedIdPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedIdPhase,
        
        kSuperSpeedPlusDeviceCapabilitySublinkLSE      = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkLSE,
        kSuperSpeedPlusDeviceCapabilitySublinkLSEPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkLSEPhase,
        kSuperSpeedPlusDeviceCapabilitySublinkLSEBits  = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkLSEBits,
        kSuperSpeedPlusDeviceCapabilitySublinkLSEKbits = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkLSEKbits,
        kSuperSpeedPlusDeviceCapabilitySublinkLSEMbits = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkLSEMbits,
        kSuperSpeedPlusDeviceCapabilitySublinkLSEGbits = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkLSEGbits,
        
        kSuperSpeedPlusDeviceCapabilitySublinkType = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkType,
        kSuperSpeedPlusDeviceCapabilitySublinkTypePhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkTypePhase,
        
        kSuperSpeedPlusDeviceCapabilitySublinkSymmetry = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSymmetry,
        kSuperSpeedPlusDeviceCapabilitySublinkSymmetryPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSymmetryPhase,
        kSuperSpeedPlusDeviceCapabilitySublinkSymmetric  = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSymmetric,
        kSuperSpeedPlusDeviceCapabilitySublinkAsymmetric = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkAsymmetric,
        
        kSuperSpeedPlusDeviceCapabilitySublinkDirection = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkDirection,
        kSuperSpeedPlusDeviceCapabilitySublinkDirectionPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkDirectionPhase,
        kSuperSpeedPlusDeviceCapabilitySublinkDirectionRx = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkDirectionRx,
        kSuperSpeedPlusDeviceCapabilitySublinkDirectionTx = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkDirectionTx,
        
        kSuperSpeedPlusDeviceCapabilitySublinkReserved = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkReserved,
        kSuperSpeedPlusDeviceCapabilitySublinkReservedPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkReservedPhase,
        
        kSuperSpeedPlusDeviceCapabilitySublinkProtocol = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkProtocol,
        kSuperSpeedPlusDeviceCapabilitySublinkProtocolPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkProtocolPhase,
        
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedMantissa = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedMantissa,
        kSuperSpeedPlusDeviceCapabilitySublinkSpeedMantissaPhase = kIOUSBSuperSpeedPlusDeviceCapabilitySublinkSpeedMantissaPhase,
    };

        
#ifdef __cplusplus
    // USB 3.0 9.6.2.3: Container ID
    struct ContainerIDCapabilityDescriptor : public DeviceCapabilityDescriptor
    {
#else
    struct ContainerIDCapabilityDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
        uint8_t     bDevCapabilityType;
#endif
        uint8_t  bReserved;
        uint32_t containerID[4];
    } __attribute__((packed));
        
    typedef struct ContainerIDCapabilityDescriptor ContainerIDCapabilityDescriptor;

#ifdef __cplusplus
    // USB 3.1 9.6.2.4: Platform Descriptor
    struct PlatformCapabilityDescriptor : public DeviceCapabilityDescriptor
    {
#else
    struct PlatformCapabilityDescriptor
    {
        uint8_t   bLength;
        uint8_t   bDescriptorType;
        uint8_t   bDevCapabilityType;
#endif
        uint8_t   bReserved;
        uuid_t    uuidPlatformCapability;
    }__attribute__((packed));

    typedef struct PlatformCapabilityDescriptor PlatformCapabilityDescriptor;

    // USB Billboard 3.1.6.2: Billboard Capability Descriptor V1.2
    struct BillboardAltModeCapabilityCompatibility
    {
        uint16_t  wSVID;
        uint32_t  dwAlternateMode;
        uint8_t   iAlternateModeString;
    }__attribute__((packed));

    typedef struct BillboardAltModeCapabilityCompatibility BillboardAltModeCapabilityCompatibility;

    // USB Billboard 3.1.6.2: Billboard Capability Descriptor V1.1 and 1.21+
    struct BillboardAltModeCapability
    {
        uint16_t  wSVID;
        uint8_t   bAlternateMode;
        uint8_t   iAlternateModeString;
    }__attribute__((packed));

    typedef struct BillboardAltModeCapability BillboardAltModeCapability;

#ifdef __cplusplus
    // USB Billboard 3.1.6.2: Billboard Capability Descriptor
    struct BillboardCapabilityDescriptor : public DeviceCapabilityDescriptor
    {
#else
    struct BillboardCapabilityDescriptor
    {
        uint8_t  bLength;
        uint8_t  bDescriptorType;
        uint8_t  bDevCapabilityType;
#endif
        uint8_t  iAddtionalInfoURL;
        uint8_t  bNumberOfAlternateModes;
        uint8_t  bPreferredAlternateMode;
        uint16_t VCONNPower;
        uint8_t  bmConfigured[32];
        uint16_t bcdVersion;
        uint8_t  bAdditonalFailureInfo;
        uint8_t  bReserved;
        uint8_t  altModeCapabilities[];
    }__attribute__((packed));

    typedef struct BillboardCapabilityDescriptor BillboardCapabilityDescriptor;

#ifdef __cplusplus
    // USB Billboard 3.1.6.3: Billboard Capability Descriptor V1.21
    struct BillboardAltModeCapabilityDescriptor : public DeviceCapabilityDescriptor
    {
#else
    struct BillboardAltModeCapabilityDescriptor
    {
        uint8_t   bLength;
        uint8_t   bDescriptorType;
        uint8_t   bDevCapabilityType;
#endif
        uint8_t   bIndex;
        uint16_t  dwAlternateModeVdo;
    }__attribute__((packed));

    typedef struct BillboardAltModeCapabilityDescriptor BillboardAltModeCapabilityDescriptor;

#ifdef __cplusplus
    // USB 3.0 9.6.4: Interface Association
    struct InterfaceAssociationDescriptor: public Descriptor
    {
#else
    struct InterfaceAssociationDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint8_t     bFirstInterface;
        uint8_t     bInterfaceCount;
        uint8_t     bFunctionClass;
        uint8_t     bFunctionSubClass;
        uint8_t     bFunctionProtocol;
        uint8_t     iFunction;
    } __attribute__((packed));
        
    typedef struct InterfaceAssociationDescriptor InterfaceAssociationDescriptor;
        
    
#ifdef __cplusplus
    // USB 3.1 9.6.7: SuperSpeed Endpoint Companion
    struct SuperSpeedEndpointCompanionDescriptor : public Descriptor
    {
#else
    struct SuperSpeedEndpointCompanionDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint8_t     bMaxBurst;
        uint8_t     bmAttributes;
        uint16_t    wBytesPerInterval;
    } __attribute__((packed));
        
    typedef struct SuperSpeedEndpointCompanionDescriptor SuperSpeedEndpointCompanionDescriptor;
        
    
    enum
    {
        kSuperSpeedEndpointCompanionDescriptorMaxBurst      = kIOUSBSuperSpeedEndpointCompanionDescriptorMaxBurst,
        kSuperSpeedEndpointCompanionDescriptorMaxBurstPhase = kIOUSBSuperSpeedEndpointCompanionDescriptorMaxBurstPhase,
        
        kSuperSpeedEndpointCompanionDescriptorBulkMaxStreams      = kIOUSBSuperSpeedEndpointCompanionDescriptorBulkMaxStreams,
        kSuperSpeedEndpointCompanionDescriptorBulkMaxStreamsPhase = kIOUSBSuperSpeedEndpointCompanionDescriptorBulkMaxStreamsPhase,
        kSuperSpeedEndpointCompanionDescriptorBulkReserved        = kIOUSBSuperSpeedEndpointCompanionDescriptorBulkReserved,
        kSuperSpeedEndpointCompanionDescriptorBulkReservedPhase   = kIOUSBSuperSpeedEndpointCompanionDescriptorBulkReservedPhase,
        kSuperSpeedEndpointCompanionDescriptorIsocMult            = kIOUSBSuperSpeedEndpointCompanionDescriptorIsocMult,
        kSuperSpeedEndpointCompanionDescriptorIsocMultPhase       = kIOUSBSuperSpeedEndpointCompanionDescriptorIsocMultPhase,
        kSuperSpeedEndpointCompanionDescriptorIsocReserved        = kIOUSBSuperSpeedEndpointCompanionDescriptorIsocReserved,
        kSuperSpeedEndpointCompanionDescriptorIsocReservedPhase   = kIOUSBSuperSpeedEndpointCompanionDescriptorIsocReservedPhase,
        kSuperSpeedEndpointCompanionDescriptorSSPIsocCompanion    = kIOUSBSuperSpeedEndpointCompanionDescriptorSSPIsocCompanion
    };


#ifdef __cplusplus
    // USB 3.1 9.6.8: SuperSpeedPlus Isochronous Endpoint Companion
    struct SuperSpeedPlusIsochronousEndpointCompanionDescriptor : public Descriptor
    {
#else
    struct SuperSpeedPlusIsochronousEndpointCompanionDescriptor
    {
        uint8_t     bLength;
        uint8_t     bDescriptorType;
#endif
        uint16_t    wReserved;
        uint32_t    dwBytesPerInterval;
    } __attribute__((packed));

    typedef struct SuperSpeedPlusIsochronousEndpointCompanionDescriptor SuperSpeedPlusIsochronousEndpointCompanionDescriptor;

        
#ifdef __cplusplus
    // USB 2.0 11.23.2.1: Hub Descriptor
    struct HubDescriptor : public Descriptor
    {
#else
    struct HubDescriptor
    {
        uint8_t  bLength;
        uint8_t  bDescriptorType;
#endif
        uint8_t  bNumberPorts;
        uint16_t wHubCharacteristics;
        uint8_t  bPowerOnToPowerGood;
        uint8_t  bHubControllerCurrent;
        uint8_t  deviceRemovable[2];    // Technically variable size
        uint8_t  reserved[2];           // Unused
    } __attribute__((packed));
        
    typedef struct HubDescriptor HubDescriptor;
        
    
#ifdef __cplusplus
    // USB 3.0 10.13.2.1: SuperSpeed Hub Descriptor
    struct SuperSpeedHubDescriptor : public Descriptor
    {
#else
    struct SuperSpeedHubDescriptor
    {
        uint8_t  bLength;
        uint8_t  bDescriptorType;
#endif
        uint8_t  bNumberPorts;
        uint16_t wHubCharacteristics;
        uint8_t  bPowerOnToPowerGood;
        uint8_t  bHubControllerCurrent;
        uint8_t  bHubDecodeLatency;
        uint16_t wHubDelay;
        uint16_t deviceRemovable;
    } __attribute__((packed));
        
    typedef struct SuperSpeedHubDescriptor SuperSpeedHubDescriptor;
        
    
    enum
    {
        kSuperSpeedHubCharacteristicsPowerSwitchingMask       = kIOUSBSuperSpeedHubCharacteristicsPowerSwitchingMask,
        kSuperSpeedHubCharacteristicsPowerSwitchingGanged     = kIOUSBSuperSpeedHubCharacteristicsPowerSwitchingGanged,
        kSuperSpeedHubCharacteristicsPowerSwitchingIndividual = kIOUSBSuperSpeedHubCharacteristicsPowerSwitchingIndividual,
        kSuperSpeedHubCharacteristicsCompoundDevice           = kIOUSBSuperSpeedHubCharacteristicsCompoundDevice,
        kSuperSpeedHubCharacteristicsOverCurrentMask          = kIOUSBSuperSpeedHubCharacteristicsOverCurrentMask,
        kSuperSpeedHubCharacteristicsOverCurrentGlobal        = kIOUSBSuperSpeedHubCharacteristicsOverCurrentGlobal,
        kSuperSpeedHubCharacteristicsOverCurrentIndividual    = kIOUSBSuperSpeedHubCharacteristicsOverCurrentIndividual,
        kSuperSpeedHubCharacteristicsReserved                 = kIOUSBSuperSpeedHubCharacteristicsReserved,
        
        kSuperSpeedHubDecodeLatencyMax = kIOUSBSuperSpeedHubDecodeLatencyMax,
        kSuperSpeedHubDelayMax         = kIOUSBSuperSpeedHubDelayMax
    };
    
#if defined(__cplusplus) && KERNEL
#pragma mark Descriptor list parsing
    /*!
     * @brief       Get the next descriptor in a configuration descriptor
     * @discussion  This method will advance currentDescriptor by its bLength, and validate that the new descriptor fits withing the bounds of configurationDescriptor.  Using NULL for currentDescriptor will return the first descriptor after the configuration descriptor.
     * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
     * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
     * @return      Descriptor pointer, or NULL if no descriptor can be returned
     */
    const Descriptor* getNextDescriptor(const ConfigurationDescriptor* configurationDescriptor, const Descriptor* currentDescriptor);

    /*!
     * @brief       Find the next descriptor matching a given type within a configuration descriptor
     * @discussion  This method uses getNextDescriptor, and further validates that the returned descriptor's bDescriptorType field matches the type parameter.
     * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
     * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
     * @param       type tDescriptorType representing the descriptor type to find
     * @return      Descriptor pointer, or NULL if no matching descriptor can be found
     */
    const Descriptor* getNextDescriptorWithType(const ConfigurationDescriptor* configurationDescriptor, const Descriptor* currentDescriptor, const uint8_t type);

    /*!
     * @brief       Get the next descriptor in a configuration descriptor that belongs to another container descriptor
     * @discussion  This method uses getNextDescriptor, but will return NULL if another descriptor is found whose bDescriptorType field matches the value used for parentDescriptor's bDescriptorType.  Using NULL for currentDescriptor will return the first descriptor after parentDescriptor.
     * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
     * @param       parentDescriptor A descriptor pointer within the bounds of configurationDescriptor
     * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
     * @return      Descriptor pointer, or NULL if no descriptor can be returned
     */
    const Descriptor* getNextAssociatedDescriptor(const ConfigurationDescriptor* configurationDescriptor, const Descriptor* parentDescriptor, const Descriptor* currentDescriptor);

    /*!
     * @brief       Find the next descriptor matching a given type within a configuration descriptor that belongs to another container descriptor
     * @discussion  This method uses getNextAssociatedDescriptor, and further validates that the returned descriptor's bDescriptorType field matches the type passed parameter.
     * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
     * @param       parentDescriptor A descriptor pointer within the bounds of configurationDescriptor
     * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
     * @param       type tDescriptorType representing the descriptor type to find
     * @return      Descriptor pointer, or NULL if no matching descriptor can be found
     */
    const Descriptor* getNextAssociatedDescriptorWithType(const ConfigurationDescriptor* configurationDescriptor, const Descriptor* parentDescriptor, const Descriptor* currentDescriptor, const uint8_t type);

    /*!
     * @brief       Find the next interface association descriptor in a configuration descriptor
     * @discussion  This method uses getNextDescriptorWithType to fetch the next interface association descriptor
     * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
     * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
     * @return      InterfaceAssociationDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const InterfaceAssociationDescriptor* getNextInterfaceAssociationDescriptor(const ConfigurationDescriptor* configurationDescriptor, const Descriptor* currentDescriptor);
    
    /*!
     * @brief       Find the next interface descriptor in a configuration descriptor
     * @discussion  This method uses getNextDescriptorWithType to fetch the next interface descriptor
     * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
     * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
     * @return      InterfaceDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const InterfaceDescriptor* getNextInterfaceDescriptor(const ConfigurationDescriptor* configurationDescriptor, const Descriptor* currentDescriptor);
        
    /*!
     * @brief       Find the next endpoint descriptor associated with an interface descriptor
     * @discussion  This method uses getNextAssociatedDescriptorWithType to fetch the next endpoint descriptor associated with a specific interface descriptor
     * @param       configurationDescriptor Configuration descriptor that contains the descriptors to iterate through
     * @param       interfaceDescriptor An interface descriptor within the bounds of configurationDescriptor
     * @param       currentDescriptor A descriptor pointer within the bounds of configurationDescriptor, or NULL
     * @return      EndpointDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const EndpointDescriptor* getNextEndpointDescriptor(const ConfigurationDescriptor* configurationDescriptor, const InterfaceDescriptor* interfaceDescriptor, const Descriptor* currentDescriptor);
    
    /*!
     * @brief       Get the next device capability descriptor in a BOS descriptor
     * @discussion  This method will advance currentDescriptor by its bLength, and validate that the new descriptor fits withing the bounds of bosDescriptor.  Using NULL for currentDescriptor will return the first descriptor after the BOS descriptor.
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @param       currentDescriptor A descriptor pointer within the bounds of bosDescriptor, or NULL
     * @return      DeviceCapabilityDescriptor pointer, or NULL if no descriptor can be returned
     */
    const DeviceCapabilityDescriptor* getNextCapabilityDescriptor(const BOSDescriptor* bosDescriptor, const DeviceCapabilityDescriptor* currentDescriptor);
    
    /*!
     * @brief       Find the next descriptor matching a given type within a BOS descriptor
     * @discussion  This method uses getNextCapabilityDescriptor, and further validates that the returned descriptor's bDevCapabilityType field matches the type parameter.
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @param       currentDescriptor A descriptor pointer within the bounds of bosDescriptor, or NULL
     * @param       type tDeviceCapabilityType representing the descriptor type to find
     * @return      DeviceCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const DeviceCapabilityDescriptor* getNextCapabilityDescriptorWithType(const BOSDescriptor* bosDescriptor, const DeviceCapabilityDescriptor* currentDescriptor, const uint8_t type);
        
    /*!
     * @brief       Find the first USB20ExtensionCapabilityDescriptor in a BOS descriptor
     * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first USB20ExtensionCapabilityDescriptor
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @return      USB20ExtensionCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const USB20ExtensionCapabilityDescriptor* getUSB20ExtensionDeviceCapabilityDescriptor(const BOSDescriptor* bosDescriptor);

    /*!
     * @brief       Find the first SuperSpeedUSBDeviceCapabilityDescriptor in a BOS descriptor
     * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first SuperSpeedUSBDeviceCapabilityDescriptor
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @return      SuperSpeedUSBDeviceCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const SuperSpeedUSBDeviceCapabilityDescriptor* getSuperSpeedDeviceCapabilityDescriptor(const BOSDescriptor* bosDescriptor);

    /*!
     * @brief       Find the first SuperSpeedPlusUSBDeviceCapabilityDescriptor in a BOS descriptor
     * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first SuperSpeedPlusUSBDeviceCapabilityDescriptor
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @return      SuperSpeedPlusUSBDeviceCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const SuperSpeedPlusUSBDeviceCapabilityDescriptor* getSuperSpeedPlusDeviceCapabilityDescriptor(const BOSDescriptor* bosDescriptor);

    /*!
     * @brief       Find the first ContainerIDCapabilityDescriptor in a BOS descriptor
     * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first ContainerIDCapabilityDescriptor
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @return      ContainerIDCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const ContainerIDCapabilityDescriptor* getContainerIDDescriptor(const BOSDescriptor* bosDescriptor);

    /*!
     * @brief       Find the first PlatformCapabilityDescriptor in a BOS descriptor
     * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first PlatformCapabilityDescriptor
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @return      PlatformCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const PlatformCapabilityDescriptor* getPlatformCapabilityDescriptor(const BOSDescriptor* bosDescriptor);
        
    /*!
     * @brief       Find the first BillboardCapabilityDescriptor in a BOS descriptor
     * @discussion  This method uses getNextCapabilityDescriptorWithType to fetch the first BillboardCapabilityDescriptor
     * @param       bosDescriptor BOS descriptor that contains the descriptors to iterate through
     * @return      BillboardCapabilityDescriptor pointer, or NULL if no matching descriptor can be found
     */
    const BillboardCapabilityDescriptor* getBillboardDescriptor(const BOSDescriptor* bosDescriptor);


#pragma mark Device descriptor parsing
    /*!
     * @brief       Validate the contents of a device descriptor
     * @discussion  This method parses a device descriptor and validates its contents.  If repairable validation errors are encountered, the descriptor passed in may be modified.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The descriptor to validate
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateDeviceDescriptor(uint32_t usbDeviceSpeed, const DeviceDescriptor* descriptor);
    
#pragma mark Endpoint descriptor parsing
    /*!
     * @brief       Extract the direction of an endpoint from an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor to determine its transfer direction
     * @param       descriptor The descriptor to parse
     * @return      tEndpointDirection indicating the direction found.  Control endpoints return tEndpointDirection.
     */
    uint8_t getEndpointDirection(const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Extract the direction and number of an endpoint from an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor to determine its address
     * @param       descriptor The descriptor to parse
     * @return      uint8_t representing direction and endpoint number
     */
    uint8_t getEndpointAddress(const EndpointDescriptor* descriptor);
    
    /*!
     * @brief       Extract the number of an endpoint from an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor to determine its number, excluding direction
     * @param       descriptor The descriptor to parse
     * @return      uint8_t representing endpoint number
     */
    uint8_t getEndpointNumber(const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Extract the type of an endpoint from an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor to determine its type
     * @param       descriptor The descriptor to parse
     * @return      tEndpointType indicating the type found.
     */
    uint8_t getEndpointType(const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Validate the max packet size of an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor and validates the max packet size.  If repairable validation errors are encountered, the descriptor passed in may be modified.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The descriptor to validate
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateEndpointMaxPacketSize(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Extract the max packet size from an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor to determine its max packet size, which does not take into account mult or burst factors.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The descriptor to parse
     * @return      uint16_t The max packet size in bytes
     */
    uint16_t getEndpointMaxPacketSize(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Validate the burst size of endpoint descriptors
     * @discussion  This method parses endpoint descriptors and validates the burst size.  If repairable validation errors are encountered, the descriptors passed in may be modified.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to validate
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to validate, or NULL
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateEndpointBurstSize(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor, const SuperSpeedEndpointCompanionDescriptor* companionDescriptor);
        
    /*!
     * @brief       Validate the burst size of endpoint descriptors
     * @discussion  This method parses endpoint descriptors and validates the burst size.  If repairable validation errors are encountered, the descriptors passed in may be modified.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to validate
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to validate, or NULL
     * @param       sspCompanionDescriptor The SuperSpeedPlusIsochronousEndpointCompanionDescriptor to validate, or NULL
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateEndpointBurstSize(uint32_t usbDeviceSpeed,
                                   const EndpointDescriptor* descriptor,
                                   const SuperSpeedEndpointCompanionDescriptor* companionDescriptor,
                                   const SuperSpeedPlusIsochronousEndpointCompanionDescriptor* sspCompanionDescriptor);
    
    /*!
     * @brief       Extract the burst size from endpoint descriptors
     * @discussion  This method parses endpoint descriptors to determine burst size, which includes mult and burst factors as applicable.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse, or NULL
     * @return      uint16_t The burst size in bytes
     */
    uint16_t getEndpointBurstSize(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor, const SuperSpeedEndpointCompanionDescriptor* companionDescriptor);
        
    /*!
     * @brief       Extract the burst size from endpoint descriptors
     * @discussion  This method parses endpoint descriptors to determine burst size, which includes mult and burst factors as applicable.  SuperSpeed Plus isochronous endpoints will return the dwBytesPerInterval field from the SuperSpeedPlusIsochronousEndpointCompanionDescriptor parameter.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse, or NULL
     * @param       sspCompanionDescriptor The SuperSpeedPlusIsochronousEndpointCompanionDescriptor to parse, or NULL
     * @return      uint32_t The burst size in bytes
     */
    uint32_t getEndpointBurstSize32(uint32_t usbDeviceSpeed,
                                    const EndpointDescriptor* descriptor,
                                    const SuperSpeedEndpointCompanionDescriptor* companionDescriptor = NULL,
                                    const SuperSpeedPlusIsochronousEndpointCompanionDescriptor* sspCompanionDescriptor = NULL);
        
    /*!
     * @brief       Extract the mult count from endpoint descriptors
     * @discussion  This method parses endpoint descriptors to determine mult
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse, or NULL
     * @param       sspCompanionDescriptor The SuperSpeedPlusIsochronousEndpointCompanionDescriptor to parse, or NULL
     * @return      uint8_t The mult count
     */
    uint8_t getEndpointMult(uint32_t usbDeviceSpeed,
                            const EndpointDescriptor* descriptor,
                            const SuperSpeedEndpointCompanionDescriptor* companionDescriptor = NULL,
                            const SuperSpeedPlusIsochronousEndpointCompanionDescriptor* sspCompanionDescriptor = NULL);
        
    /*!
     * @brief       Validate the interval of an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor and validates the interval.  If repairable validation errors are encountered, the descriptor passed in may be modified.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to validate
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateEndpointInterval(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Extract the interval of an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor and returns the service interval as n in (2^(n-1)) microframes
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @return      uint32_t Encoded endpoint interval
     */
    uint32_t getEndpointIntervalEncodedMicroframes(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor);

    /*!
     * @brief       Extract the interval of an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor and returns the service interval in microframes
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @return      uint32_t Endpoint interval in microframes
     */
    uint32_t getEndpointIntervalMicroframes(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Extract the interval of an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor and returns the service interval in frames
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @return      uint32_t Endpoint interval in frames
     */
    uint32_t getEndpointIntervalFrames(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor);
        
    /*!
     * @brief       Extract the number of streams supported by an endpoint
     * @discussion  This method parses endpoint descriptors and returns the number of streams supported as n in (2^n)
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse
     * @return      uint32_t Encoded number of streams
     */
    uint32_t getEndpointMaxStreamsEncoded(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor, const SuperSpeedEndpointCompanionDescriptor* companionDescriptor);

    /*!
     * @brief       Extract the number of streams supported by an endpoint
     * @discussion  This method parses endpoint descriptors and returns the number of streams supported
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to parse
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to parse
     * @return      uint32_t Number of streams
     */
    uint32_t getEndpointMaxStreams(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor, const SuperSpeedEndpointCompanionDescriptor* companionDescriptor);
        
    /*!
     * @brief       Extract the maximum bus current required by a configuration descriptor
     * @discussion  This method parses a configuration descriptor and returns the number of milliamps required to power the device
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The ConfigurationDescriptor to parse
     * @return      uint32_t milliamps required
     */
    uint32_t getConfigurationMaxPowerMilliAmps(uint32_t usbDeviceSpeed, const ConfigurationDescriptor* descriptor);
        
    /*!
     * @brief       Validate the contents of an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor and validates its contents.  If repairable validation errors are encountered, the descriptor passed in may be modified.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to validate
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to validate, or NULL
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateEndpointDescriptor(uint32_t usbDeviceSpeed, const EndpointDescriptor* descriptor, const SuperSpeedEndpointCompanionDescriptor* companionDescriptor);
        
    /*!
     * @brief       Validate the contents of an endpoint descriptor
     * @discussion  This method parses an endpoint descriptor and validates its contents.  If repairable validation errors are encountered, the descriptor passed in may be modified.
     * @param       usbDeviceSpeed The operational speed of the device
     * @param       descriptor The EndpointDescriptor to validate
     * @param       companionDescriptor The SuperSpeedEndpointCompanionDescriptor to validate, or NULL
     * @param       sspCompanionDescriptor The SuperSpeedPlusIsochronousEndpointCompanionDescriptor to validate, or NULL
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateEndpointDescriptor(uint32_t usbDeviceSpeed,
                                    const EndpointDescriptor* descriptor,
                                    const SuperSpeedEndpointCompanionDescriptor* companionDescriptor,
                                    const SuperSpeedPlusIsochronousEndpointCompanionDescriptor* sspCompanionDescriptor);
    
#pragma mark String descriptor parsing
    /*!
     * @brief       Convert a USB string descriptor to a UTF8 character string
     * @discussion  This method uses utf8_encodestr with appropriate options to convert a USB string descriptor to a UTF8 string.
     * @param       stringDescriptor Descriptor to convert
     * @param       stringBuffer Buffer to write the UTF8 string to
     * @param       length Reference to size_t.  As input it is the size stringBuffer.  As output it is the number of character written to stringBuffer.
     * @return      IOReturn result code.  kIOReturnSuccess if any portion of the string could be converted and placed in stringBuffer.  kIOReturnError if the string descriptor cannot be converted to UTF8.
     */
    IOReturn stringDescriptorToUTF8(const StringDescriptor* stringDescriptor, char* stringBuffer, size_t& length);
    
#pragma mark Capability descriptor parsing
    /*!
     * @brief       Validate the contents of a BOS descriptor
     * @discussion  This method parses a BOS descriptor and validates its contents, as well as the contents of its associated device capability descriptors.  If repairable validation errors are encountered, the descriptor passed in may be modified.
     * @param       bosDescriptor The BOSDescriptor to validate
     * @return      bool true if the descriptor passed validation, false if it did not
     */
    bool validateDeviceCapabilityDescriptors(const BOSDescriptor* bosDescriptor);
        
#endif // __cplusplus
    
#pragma mark Device requests
    // USB 2.0 9.3: USB Device Requests
    struct DeviceRequest
    {
        uint8_t     bmRequestType;
        uint8_t     bRequest;
        uint16_t    wValue;
        uint16_t    wIndex;
        uint16_t    wLength;
    } __attribute__((packed));
        
    typedef struct DeviceRequest DeviceRequest;
        
    // USB 3.0 9.4.12: Set SEL Standard Device Request
    struct DeviceRequestSetSELData
    {
        uint8_t     u1Sel;              // Time in μs for U1 System Exit Latency
        uint8_t     u1Pel;              // Time in μs for U1 Device to Host Exit Latency
        uint16_t    u2Sel;              // Time in μs for U2 System Exit Latency
        uint16_t    u2Pel;              // Time in μs for U2 Device to Host Exit Latency
    } __attribute__((packed));
        
    typedef struct DeviceRequestSetSELData DeviceRequestSetSELData;
    
    enum
    {
        kDeviceRequestSize              = kIOUSBDeviceRequestSize,
        kDeviceRequestDirectionMask     = kIOUSBDeviceRequestDirectionMask,
        kDeviceRequestDirectionPhase    = kIOUSBDeviceRequestDirectionPhase,
        kDeviceRequestDirectionOut      = kIOUSBDeviceRequestDirectionOut,
        kDeviceRequestDirectionIn       = kIOUSBDeviceRequestDirectionIn,
        kDeviceRequestTypeMask          = kIOUSBDeviceRequestTypeMask,
        kDeviceRequestTypePhase         = kIOUSBDeviceRequestTypePhase,
        kDeviceRequestTypeStandard      = kIOUSBDeviceRequestTypeStandard,
        kDeviceRequestTypeClass         = kIOUSBDeviceRequestTypeClass,
        kDeviceRequestTypeVendor        = kIOUSBDeviceRequestTypeVendor,
        kDeviceRequestRecipientMask     = kIOUSBDeviceRequestRecipientMask,
        kDeviceRequestRecipientPhase    = kIOUSBDeviceRequestRecipientPhase,
        kDeviceRequestRecipientDevice   = kIOUSBDeviceRequestRecipientDevice,
        kDeviceRequestRecipientInterface= kIOUSBDeviceRequestRecipientInterface,
        kDeviceRequestRecipientEndpoint = kIOUSBDeviceRequestRecipientEndpoint,
        kDeviceRequestRecipientOther    = kIOUSBDeviceRequestRecipientOther,
    };
    
    // USB 2.0 9.4: Standard Device Requests
    // USB 3.0 9.4: Standard Device Requests
    enum
    {
        kDeviceRequestGetStatus             = kIOUSBDeviceRequestGetStatus,
        kDeviceRequestClearFeature          = kIOUSBDeviceRequestClearFeature,
        kDeviceRequestGetState              = kIOUSBDeviceRequestGetState,
        kDeviceRequestSetFeature            = kIOUSBDeviceRequestSetFeature,
        kDeviceRequestSetAddress            = kIOUSBDeviceRequestSetAddress,
        kDeviceRequestGetDescriptor         = kIOUSBDeviceRequestGetDescriptor,
        kDeviceRequestSetDescriptor         = kIOUSBDeviceRequestSetDescriptor,
        kDeviceRequestGetConfiguration      = kIOUSBDeviceRequestGetConfiguration,
        kDeviceRequestSetConfiguration      = kIOUSBDeviceRequestSetConfiguration,
        kDeviceRequestGetInterface          = kIOUSBDeviceRequestGetInterface,
        kDeviceRequestSetInterface          = kIOUSBDeviceRequestSetInterface,
        kDeviceRequestSynchFrame            = kIOUSBDeviceRequestSynchFrame,
        kDeviceRequestSetSel                = kIOUSBDeviceRequestSetSel,
        kDeviceRequestSetIsochronousDelay   = kIOUSBDeviceRequestSetIsochronousDelay
    };
    
    // USB 2.0 9.4.5: Get Status
    // USB 3.0 9.4.5: Get Status
    enum
    {
        kDeviceStatusSelfPowered            = kIOUSBDeviceStatusSelfPowered,
        kDeviceStatusRemoteWakeEnable       = kIOUSBDeviceStatusRemoteWakeEnable,
        kDeviceStatusU1Enable               = kIOUSBDeviceStatusU1Enable,
        kDeviceStatusU2Enable               = kIOUSBDeviceStatusU2Enable,
        kDeviceStatusLTMEnable              = kIOUSBDeviceStatusLTMEnable,
        
        kInterfaceStatusRemoteWakeCapable   = kIOUSBInterfaceStatusRemoteWakeCapable,
        kInterfaceStatusRemoteWakeEnable    = kIOUSBInterfaceStatusRemoteWakeEnable,
        
        kEndpointStatusHalt                 = IOUSBEndpointStatusHalt
    };
    
    // USB 2.0 Table 9-6: Standard Feature Selectors
    // USB 3.0 Table 9-7: Standard Feature Selectors
    enum
    {
        kDeviceFeatureSelectorRemoteWakeup  = kIOUSBDeviceFeatureSelectorRemoteWakeup,
        kDeviceFeatureSelectorTestMode      = kIOUSBDeviceFeatureSelectorTestMode,
        kDeviceFeatureSelectorU1Enable      = kIOUSBDeviceFeatureSelectorU1Enable,
        kDeviceFeatureSelectorU2Enable      = kIOUSBDeviceFeatureSelectorU2Enable,
        kDeviceFeatureSelectorLTMEnable     = kIOUSBDeviceFeatureSelectorLTMEnable,
        
        kInterfaceFeatureSelectorSuspend    = kIOUSBInterfaceFeatureSelectorSuspend,
        
        kEndpointFeatureSelectorStall       = IOUSBEndpointFeatureSelectorStall,
    };
    
    // USB 3.0 Table 9-8: Suspend Options
    enum
    {
        kInterfaceSuspendLowPower           = kIOUSBInterfaceSuspendLowPower,
        kInterfaceSuspendRemoteWakeEnable   = kIOUSBInterfaceSuspendRemoteWakeEnable
    };
        
    // USB 3.0 Table 10-16: Hub Parameters
    enum
    {
        kHubPort2PortExitLatencyNs          = kIOUSBHubPort2PortExitLatencyNs,
        kHubDelayNs                         = kIOUSBHubDelayNs
    };
        
    // USB 3.0 Table 8-33: Timing Parameters
    enum
    {
        kPingResponseTimeNs                 = kIOUSBPingResponseTimeNs
    };

    enum tBusVoltage
    {
        kBusVoltageDefault                  = kIOUSBBusVoltageDefault
    };
        
#ifdef __cplusplus
}


namespace StandardUSB20
{
#pragma mark BESL & HIRD Encodings

    /*!
     * @brief       BESL Encodings from Table X-X1 in USB 2.0 ECN Errata for Link Power Management
     * @discussion  This method converts the Best Effort Service Latency Index value used in the USB 2.0 Extension descriptor to Nanoseconds.
     * @param       beslIndex The BESL Index value used in the USB 2.0 Extension descriptor
     * @param       beslNs Reference to uint32_t.  As output, it is the converted BESL value in Nanoseconds
     * @return      result code.  kIOReturnSuccess if the conversion was successful. kIOReturnBadArgument if the input beslIndex is invalid.
     */
    IOReturn convertBESLIndexToNs(uint32_t beslIndex, uint32_t& beslNs);

    /*!
     * @brief       BESL Encodings from Table X-X1 in USB 2.0 ECN Errata for Link Power Management
     * @discussion  This method does the opposite of convertBESLIndexToUs. It converts the BESL value in Nanoseconds to Index.
     * @param       beslNs The converted BESL value in Nanoseconds
     * @param       beslIndex Reference to uint32_t. As output, it is the BESL Index value converted from Nanoseconds.
     * @return      result code.  kIOReturnSuccess if the conversion was successful. kIOReturnBadArgument if the input beslNs is invalid.
     */
    IOReturn convertBESLNsToIndex(uint32_t beslNs, uint32_t &beslIndex);

    enum tBusCurrent
    {
        kBusCurrentMinimum       = kIOUSB20BusCurrentMinimum,
        kBusCurrentDefault       = kIOUSB20BusCurrentDefault,
        kBusCurrentMaxPowerUnits = kIOUSB20BusCurrentMaxPowerUnits

    };
}

namespace StandardUSB30
{
    enum tBusCurrent
    {
        kBusCurrentMinimum       = kIOUSB30BusCurrentMinimum,
        kBusCurrentDefault       = kIOUSB30BusCurrentDefault,
        kBusCurrentMaxPowerUnits = kIOUSB30BusCurrentMaxPowerUnits
    };

    // USB 3.0 Table 6-21
    enum tResetTimeout
    {
        kResetMinimumTimeout                    = kIOUSB30ResetMinimumTimeout,
        kResetTypicalTimeout                    = kIOUSB30ResetTypicalTimeout,
        kResetMaximumTimeout                    = kIOUSB30ResetMaximumTimeout,
        kResetMaximumWithMarginTimeout          = kIOUSB30ResetMaximumWithMarginTimeout
    };

    // USB 3.0 Table 7-12
    enum tLinkStateTimeout
    {
        kLinkStateSSInactiveQuietTimeout        = kIOUSB30LinkStateSSInactiveQuietTimeout,
        kLinkStateRxDetectQuietTimeout          = kIOUSB30LinkStateRxDetectQuietTimeout,
        kLinkStatePollingLFPSTimeout            = kIOUSB30LinkStatePollingLFPSTimeout,
        kLinkStatePollingActiveTimeout          = kIOUSB30LinkStatePollingActiveTimeout,
        kLinkStatePollingConfigurationTimeout   = kIOUSB30LinkStatePollingConfigurationTimeout,
        kLinkStatePollingIdleTimeout            = kIOUSB30LinkStatePollingIdleTimeout,
        kLinkStateU0RecoveryTimeout             = kIOUSB30LinkStateU0RecoveryTimeout,
        kLinkStateU0LTimeout                    = kIOUSB30LinkStateU0LTimeout,        // 10 microseconds
        kLinkStateU1NoLFPSResponseTimeout       = kIOUSB30LinkStateU1NoLFPSResponseTimeout,
        kLinkStateU1PingTimeout                 = kIOUSB30LinkStateU1PingTimeout,
        kLinkStateU2NoLFPSResponseTimeout       = kIOUSB30LinkStateU2NoLFPSResponseTimeout,
        kLinKStateU2RxDetectDelay               = kIOUSB30LinKStateU2RxDetectDelay,
        kLinkStateU3NoLFPSResponseTimeout       = kIOUSB30LinkStateU3NoLFPSResponseTimeout,
        kLinkStateU3WakeupRetryDelay            = kIOUSB30LinkStateU3WakeupRetryDelay,
        kLinkStateU3RxDetectDelay               = kIOUSB30LinkStateU3RxDetectDelay,
        kLinkStateRecoveryActiveTimeout         = kIOUSB30LinkStateRecoveryActiveTimeout,
        kLinkStateRecoveryConfigurationTimeout  = kIOUSB30LinkStateRecoveryConfigurationTimeout,
        kLinkStateRecoveryIdleTimeout           = kIOUSB30LinkStateRecoveryIdleTimeout,
        kLinkStateLoopbackExitTimeout           = kIOUSB30LinkStateLoopbackExitTimeout,
        kLinkStateHotResetActiveTimeout         = kIOUSB30LinkStateHotResetActiveTimeout,
        kLinkStateHotResetExitTimeout           = kIOUSB30LinkStateHotResetExitTimeout,

        // USB 3.0 7.5.4
        kLinkStatePollingDeadline               = kIOUSB30LinkStatePollingDeadline,

        // USB 3.0 7.5.9 and 7.5.10
        kLinkStateSSResumeDeadline              = kIOUSB30LinkStateSSResumeDeadline,
    };
    
    // USB 3.1 Table 8-18
    enum tDeviceNotificationType
    {
        kDeviceNotificationTypeFunctionWake             = kIOUSB30DeviceNotificationTypeFunctionWake,
        kDeviceNotificationTypeLatencyTolerance         = kIOUSB30DeviceNotificationTypeLatencyTolerance,
        kDeviceNotificationTypeBusIntervalAdjustment    = kIOUSB30DeviceNotificationTypeBusIntervalAdjustment,
        kDeviceNotificationTypeHostRoleRequest          = kIOUSB30DeviceNotificationTypeHostRoleRequest,
        kDeviceNotificationTypeSublinkSpeed             = kIOUSB30DeviceNotificationTypeSublinkSpeed
    };
    
    // USB 3.1 Table 8-36
    enum tTimingParameters
    {
        kTimingParameterBELTDefaultNs = kIOUSB30TimingParameterBELTDefaultNs,
        kTimingParameterBELTMinNs     = kIOUSB30TimingParameterBELTMinNs
    };
    
    // USB 3.1 Table 10-12 Port Status Type Codes
    enum tHubPortStatusCode
    {
        tHubPortStatusCodeStandard = kIOUSB30HubPortStatusCodeStandard,
        tHubPortStatusCodePD       = kIOUSB30HubPortStatusCodePD,
        tHubPortStatusCodeExt      = kIOUSB30HubPortStatusCodeExt,
        tHubPortStatusCodeCount    = kIOUSB30HubPortStatusCodeCount
    };
    
    // USB 3.1 10.16.2.6 Get Port Status
    struct tHubPortStatusExt
    {
        uint16_t wPortStatus;
        uint16_t wPortChange;
        uint32_t dwExtPortStatus;
    } __attribute__((packed));
    
    // USB 3.1 Table 10-11.  Note, offsets are specific to dwExtPortStatus
    enum tHubExtStatus
    {
        kHubExtStatusRxSublinkSpeedID      = kIOUSB30HubExtStatusRxSublinkSpeedID,
        kHubExtStatusRxSublinkSpeedIDPhase = kIOUSB30HubExtStatusRxSublinkSpeedIDPhase,
        
        kHubExtStatusTxSublinkSpeedID      = kIOUSB30HubExtStatusTxSublinkSpeedID,
        kHubExtStatusTxSublinkSpeedIDPhase = kIOUSB30HubExtStatusTxSublinkSpeedIDPhase,
        
        kHubExtStatusRxLaneCount      = kIOUSB30HubExtStatusRxLaneCount,
        kHubExtStatusRxLaneCountPhase = kIOUSB30HubExtStatusRxLaneCountPhase,
        
        kHubExtStatusTxLaneCount      = kIOUSB30HubExtStatusTxLaneCount,
        kHubExtStatusTxLaneCountPhase = kIOUSB30HubExtStatusTxLaneCountPhase
    };
}
#endif // __cplusplus

#endif
