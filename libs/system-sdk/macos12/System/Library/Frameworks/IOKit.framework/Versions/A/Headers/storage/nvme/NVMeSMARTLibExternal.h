/*
 * Copyright (c) 1998-2020 Apple Inc. All rights reserved.
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

#ifndef __NVME_SMART_LIB_EXTERNAL_H__
#define __NVME_SMART_LIB_EXTERNAL_H__


/*! @defined kIOPropertySMARTCapableKey
 @discussion Property to search for in IORegistry to find NVMe SMART capable devices
 without hardcoding the search to a particular device class. */
#define kIOPropertyNVMeSMARTCapableKey  "NVMe SMART Capable"

#include <IOKit/IOReturn.h>
#include <IOKit/IOTypes.h>


#if !KERNEL
#include <CoreFoundation/CFPlugIn.h>
#if COREFOUNDATION_CFPLUGINCOM_SEPARATE
#include <CoreFoundation/CFPlugInCOM.h>
#endif

#include <IOKit/IOCFPlugIn.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    
/*! @header NVMeSMARTLib
 NVMeSMARTLib implements non-kernel task access to NVMe SMART data.
*/
    

/* 68413F59-268A-4787-BC48-7F9960235647 */
/*! @defined kIONVMeSMARTLibFactoryID
    @discussion UUID for the IONVMeSMARTInterface Factory. */
    
#define kIONVMeSMARTLibFactoryID			CFUUIDGetConstantUUIDWithBytes(NULL,				\
                                            0x68, 0x41, 0x3F, 0x59, 0x26, 0x8A, 0x47, 0x87,		\
                                            0xBC, 0x48, 0x7F, 0x99, 0x60, 0x23, 0x56, 0x47)
    
    
/* AA0FA6F9-C2D6-457F-B10B-59A13253292F */
/*! @defined kIONVMeSMARTUserClientTypeID
    @discussion Factory ID for creating an NVMe SMART user client. */
#define kIONVMeSMARTUserClientTypeID		CFUUIDGetConstantUUIDWithBytes(NULL,				\
                                            0xAA, 0x0F, 0xA6, 0xF9, 0xC2, 0xD6, 0x45, 0x7F,		\
                                            0xB1, 0x0B, 0x59, 0xA1, 0x32, 0x53, 0x29, 0x2F)
    
/* CCD1DB19-FD9A-4DAF-BF95-12454B230AB6 */
/*! @defined kIONVMeSMARTInterfaceID
    @discussion InterfaceID for IONVMeSMARTInterface. */
#define kIONVMeSMARTInterfaceID             CFUUIDGetConstantUUIDWithBytes(NULL,				\
                                            0xCC, 0xD1, 0xDB, 0x19, 0xFD, 0x9A, 0x4D, 0xAF,		\
                                            0xBF, 0x95, 0x12, 0x45, 0x4B, 0x23, 0x0A, 0xB6)
    
    
#endif	/* !KERNEL */


enum
{
    
    kNVMeIdentifyControllerSerialNumberLen          = 20,
    kNVMeIdentifyControllerModelNumberLen           = 40,
    kNVMeIdentifyControllerFirmwareRevisionLen      = 8,
    kNVMeIdentifyControllerIEEEOUIIDLen             = 3,
    
};

#pragma pack(1)
    
typedef struct NVMePowerStateDescriptor
{
    
    uint16_t    MAXIMUM_POWER;
    uint8_t     RESERVED1[2];
    uint32_t    ENTRY_LATENCY;
    uint32_t    EXIT_LATENCY;
    uint8_t     RELATIVE_READ_THROUGHPUT;
    uint8_t     RELATIVE_READ_LATENCY;
    uint8_t     RELATIVE_WRITE_THROUGHPUT;
    uint8_t     RELATIVE_WRITE_LATENCY;
    uint8_t     RESERVED2[16];
    
} NVMePowerStateDescriptor;

typedef struct NVMeIdentifyControllerStruct
{
    
    // Controller Capabilites and Features
    uint16_t    PCI_VID;
    uint16_t    PCI_SSVID;
    uint8_t     SERIAL_NUMBER[kNVMeIdentifyControllerSerialNumberLen];
    uint8_t     MODEL_NUMBER[kNVMeIdentifyControllerModelNumberLen];
    uint8_t     FW_REVISION[kNVMeIdentifyControllerFirmwareRevisionLen];
    uint8_t     RECOMMENDED_ARBITRATION_BURST;
    uint8_t     IEEE_OUI_ID[kNVMeIdentifyControllerIEEEOUIIDLen];
    uint8_t     MIC;
    uint8_t     MAX_DATA_TRANSFER_SIZE;
    uint16_t    CONTROLLER_ID;
    uint8_t     RESERVED1[176];
    
    // Admin Command Set Attributes
    uint16_t    OPTIONAL_ADMIN_COMMAND_SUPPORT;
    uint8_t     ABORT_COMMAND_LIMIT;
    uint8_t     ASYNC_EVENT_REQUEST_LIMIT;
    uint8_t     FW_UPDATES;
    uint8_t     LOG_PAGE_ATTR;
    uint8_t     ERROR_LOG_PAGE_ENTRIES;
    uint8_t     NUM_OF_POWER_STATE_SUPPORT;
    uint8_t     ADMIN_VENDOR_SPECIFIC_COMMAND_CONFIG;
    uint8_t     AUTONOMOUS_POWER_STATE_TRANSITION_ATTR;
    uint8_t     RESERVED2[246];
    
    // NVM Command Set Attributes
    uint8_t     SQ_ENTRY_SIZE;
    uint8_t     CQ_ENTRY_SIZE;
    uint8_t     RESERVED3[2];
    uint32_t    NUM_OF_NAMESPACES;
    uint16_t    OPTIONAL_NVM_COMMAND_SUPPORT;
    uint16_t    FUSE_OP_SUPPORT;
    uint8_t     FORMAT_NVM_ATTR;
    uint8_t     VOLATILE_WRITE_CACHE;
    uint16_t    ATOMIC_WRITE_UNIT_NORMAL;
    uint16_t    ATOMIC_WRITE_UNIT_POWER_FAIL;
    uint8_t     NVM_VENDOR_SPECIFIC_COMMAND_CONFIG;
    uint8_t     RESERVED4[1];
    uint16_t    ATOMIC_COMPARE_AND_WRITE_UNIT;
    uint8_t     RESERVED5[2];
    uint32_t    SGL_SUPPORT;
    uint8_t     RESERVED6[164];
    
    // I/O Command Set Attributes
    uint8_t     RESERVED7[1344];
    
    // Power State Descriptors
    NVMePowerStateDescriptor    POWER_STATE_DESCRIPTORS[32];
    
    uint8_t     RESERVED8[1024];
    
} NVMeIdentifyControllerStruct;

typedef struct NVMeLBAFormatDataStruct
{
    
    uint16_t    METADATA_SIZE;
    uint8_t     LBA_DATA_SIZE;
    uint8_t     RELATIVE_PERFORMANCE;
    
} NVMeLBAFormatDataStruct;

typedef struct NVMeIdentifyNamespaceStruct
{
    
    uint64_t    NAMESPACE_SIZE;
    uint64_t    NAMESPACE_CAPACITY;
    uint64_t    NAMESPACE_UTILIZATION;
    uint8_t     NAMESPACE_FEATURES;
    uint8_t     NUM_OF_LBA_FORMATS;
    uint8_t     FORMATTED_LBA_SIZE;
    uint8_t     METADATA_CAPS;
    uint8_t     E2E_DATA_PROTECTION_CAPS;
    uint8_t     E2E_DATA_PROTECTION_TYPE_SETTINGS;
    uint8_t     NS_MULTIPATH_IO_AND_NS_SHARING_CAPS;
    uint8_t     RESERVATION_CAPABILITIES;
    uint8_t     RESERVED1[88];

    uint64_t    IEEE_EXTENDED_UID;

    NVMeLBAFormatDataStruct LBA_FORMATS[16];
    uint8_t     RESERVED2[192];

    // Vendor Specific Stuff
    uint8_t     NSTYPE;
    uint8_t     VENDOR_SPECIFIC[3711];
    
} NVMeIdentifyNamespaceStruct;

typedef struct NVMeSMARTData
{
    
    uint8_t     CRITICAL_WARNING;
    uint16_t    TEMPERATURE;
    uint8_t     AVAILABLE_SPARE;
    uint8_t     AVAILABLE_SPARE_THRESHOLD;
    uint8_t     PERCENTAGE_USED;
    uint8_t     RESERVED1[26];
    uint64_t    DATA_UNITS_READ[2];
    uint64_t    DATA_UNITS_WRITTEN[2];
    uint64_t    HOST_READ_COMMANDS[2];
    uint64_t    HOST_WRITE_COMMANDS[2];
    uint64_t    CONTROLLER_BUSY_TIME[2];
    uint64_t    POWER_CYCLES[2];
    uint64_t    POWER_ON_HOURS[2];
    uint64_t    UNSAFE_SHUTDOWNS[2];
    uint64_t    MEDIA_ERRORS[2];
    uint64_t    NUM_ERROR_INFO_LOG_ENTRIES[2];
    uint8_t     RESERVED2[320];
    
} NVMeSMARTData;

#pragma options align=reset
    
#if !KERNEL

    
typedef struct IONVMeSMARTInterface
{

    IUNKNOWN_C_GUTS;
	
	UInt16		version;
    UInt16		revision;
	
    /*!
     @function      SMARTReadData
     @abstract      Retrieves 512 byte device SMART data structure.
     @discussion	See section 5.10.1.2 of NVM Express revision 1.0c.

     @param         data    Pointer to a SMART data buffer.

     @return        kIOReturnNoResources    If memory allocation failed.
     @return        kIOReturnBadArgument    Invalid argument passed.
     @return        kIOReturnOffline                Device is offline.
     @return        kIOReturnSuccess            Success.
     */
	
	IOReturn ( *SMARTReadData ) ( void * interface, NVMeSMARTData * data );
    
    /*!
     @function      GetIdentifyData
     @abstract      Retrieves a 4K device or namespace identify data buffer.
     @discussion    See section 5.11 of NVM Express revision 1.0c.

     @param         data                Pointer to either an controller or namespace identify strucutre.
     @param         inNamspace  Namespace ID or 0 for controller identify data.
     
     @return        kIOReturnNoResources    If memory allocation failed.
     @return        kIOReturnBadArgument    Invalid argument passed.
     @return        kIOReturnOffline                Device is offline.
     @return        kIOReturnSuccess            Success.
     */
    
	IOReturn ( *GetIdentifyData ) ( void * interface, void * data, uint32_t inNamespace );
  
    UInt64  reserved0;
    UInt64  reserved1;

    /*!
     @function      GetLogPage
     @abstract      Retrieves a log page.
     @discussion    See section 5.10 of NVM Express revision 1.0c.
     
     @param         data                Pointer to buffer that will contain log page data.
     @param         inLogPageId Log page ID for this get log page command.
     @param         inNumDWords Number of dwords for log page data, zero based.
     
     @return        kIOReturnNoResources    If memory allocation failed.
     @return        kIOReturnBadArgument    Invalid argument passed.
     @return        kIOReturnOffline                Device is offline.
     @return        kIOReturnSuccess            Success.
     */
    
    IOReturn ( *GetLogPage ) ( void * interface, void * data, uint32_t inLogPageId, uint32_t inNumDWords );

    UInt64  reserved2;
    UInt64  reserved3;
    UInt64  reserved4;
    UInt64  reserved5;
    UInt64  reserved6;
    UInt64  reserved7;
    UInt64  reserved8;
    UInt64  reserved9;
    UInt64  reserved10;
    UInt64  reserved11;
    UInt64  reserved12;
    UInt64  reserved13;
    UInt64  reserved14;
    UInt64  reserved15;
    UInt64  reserved16;
    UInt64  reserved17;
    UInt64  reserved18;
    UInt64  reserved19;
    UInt64  reserved20;
    UInt64  reserved21;
    UInt64  reserved22;
    UInt64  reserved23;
    
} IONVMeSMARTInterface;
    

#ifdef __cplusplus
    }
#endif

#endif	/* !KERNEL */

#endif  /* __NVME_SMART_LIB_EXTERNAL_H__ */
