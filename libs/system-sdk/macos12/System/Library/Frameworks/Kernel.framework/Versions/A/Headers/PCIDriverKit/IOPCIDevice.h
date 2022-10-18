/* iig(DriverKit-191.30.2) generated from IOPCIDevice.iig */

/* IOPCIDevice.iig:1-71 */
/*
 * Copyright (c) 2019-2019 Apple Inc. All rights reserved.
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

#ifndef _IOKIT_UIOPCIDEVICE_H
#define _IOKIT_UIOPCIDEVICE_H

#include <DriverKit/IOTypes.h>
#include <DriverKit/IOService.h>  /* .iig include */
#include <DriverKit/IOMemoryDescriptor.h>  /* .iig include */

/*!
 * @brief Options for <code>SaveDeviceState</code>
 *
 * @constant kPCIConfigShadowPermanent - Re-use the saved device state in all future restores (until the next permanent copy is made)
 */
enum IOPCISaveDeviceStateOptions
{
    kPCIConfigShadowPermanent = 0x80000000,
};

/*!
 * @brief BAR indexes for <code>GetBARInfo</code>
 */
enum IOPCIMemoryRange
{
    kPCIMemoryRangeBAR0         = 0,
    kPCIMemoryRangeBAR1         = 1,
    kPCIMemoryRangeBAR2         = 2,
    kPCIMemoryRangeBAR3         = 3,
    kPCIMemoryRangeBAR4         = 4,
    kPCIMemoryRangeBAR5         = 5,
    kPCIMemoryRangeExpansionROM = 6,
};

/*!
 * @brief BAR types for <code>GetBARInfo</code>
 */
enum IOPCIBARType
{
    kPCIBARTypeM32   = 0x00, /* 32-bit (Non-prefetchable) Memory */
    kPCIBARTypeIO    = 0x01, /* I/O Space */
    kPCIBARTypeM64   = 0x04, /* 64-bit (Non-prefetchable) Memory */
    kPCIBARTypeM32PF = 0x08, /* 32-bit Prefetchable Memory */
    kPCIBARTypeM64PF = 0x0c, /* 64-bit Prefetchable Memory */
};

/* source class IOPCIDevice IOPCIDevice.iig:72-369 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
   @iig implementation
   #if KERNEL
   #include <IOKit/pci/IOPCIDevice.h>
   #endif
   @iig end
 */

class KERNEL IOPCIDevice : public IOService
{
public:
#pragma mark IOService Overrides
    virtual bool
    init() override;

    virtual void
    free() override;

    virtual kern_return_t
    ClientCrashed(IOService * client, uint64_t options) override;

    /*!
     * @brief       Send a dictionary of properties to an IOService.
     * @discussion  Can set only properties defined in IOPCIFamilyDefinitions.h
     * @param       properties Dictionary of properties.
     * @return      kIOReturnSuccess if at least one key/value pair is successful. See IOReturn.h for error codes.
     */
    virtual kern_return_t
    SetProperties(OSDictionary * properties) override;

#pragma mark Session Management
    /*!
     * @brief       Open a session to the IOPCIDevice
     * @discussion  This method opens a session to an IOPCIDevice for exclusive access and
     *              allow access to the PCI Device's configuration and aperture space.
     * @param       forClient The IOService that is opening a session.
     * @param       options See IOService.h
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    kern_return_t
    Open(IOService*   forClient,
         IOOptionBits options) LOCALONLY;


    /*!
     * @brief       Close a session to the IOPCIDevice
     * @discussion  This method closes an open session to an IOPCIDevice. Closing the open session will also turn off the Bus Master Enable and
     *              Memory Space Enable bits defined in the command register in the PCI spec
     * @param       forClient The IOService that is closing its session.
     * @param       options See IOService.h
     * @return      KERN_SUCCESS is successful see IOReturn.h for error codes.
     */
    void
    Close(IOService*   forClient,
          IOOptionBits options) LOCALONLY;

#pragma mark Memory Accessors
    /*!
     * @brief       Reads a 32-bit value from the PCI device's configuration space.
     * @discussion  This method reads a 32-bit configuration space register on the device and returns its value. This is a blocking call.
     * @param       offset An offset into configuration space, of which bits 0-1 are ignored.
     * @param       readData An out parameter containing the 32-bit value in host byte order. -1 is written to readData on error.
     */
    void
    ConfigurationRead32(uint64_t offset, uint32_t* readData) LOCALONLY;

    /*!
     * @brief       Reads a 16-bit value from the PCI device's configuration space.
     * @discussion  This method reads a 16-bit configuration space register on the device and returns its value. This is a blocking call.
     * @param       offset An offset into configuration space, of which bits 0-1 are ignored.
     * @param       readData An out parameter containing the 16-bit value in host byte order. -1 is written to readData on error.
     */
    void
    ConfigurationRead16(uint64_t offset, uint16_t* readData) LOCALONLY;

    /*!
     * @brief       Reads an 8-bit value from the PCI device's configuration space.
     * @discussion  This method reads a 8-bit configuration space register on the device and returns its value. This is a blocking call.
     * @param       offset An offset into configuration space, of which bits 0-1 are ignored.
     * @param       readData An out parameter containing the 8-bit value in host byte order. -1 is written to readData on error.
     */
    void
    ConfigurationRead8(uint64_t offset, uint8_t* readData) LOCALONLY;

    /*!
     * @brief       Writes a 32-bit value to the PCI device's configuration space.
     * @discussion  This method writes a 32-bit value to a configuration space register on the device.
     * @param       offset An offset into configuration space, of which bits 0-1 are ignored.
     * @param       data A 32-bit value to be written in host byte order.
     */
    void
    ConfigurationWrite32(uint64_t offset,
                         uint32_t data) LOCALONLY;

    /*!
     * @brief       Writes a 16-bit value to the PCI device's configuration space.
     * @discussion  This method writes a 16-bit value to a configuration space register on the device.
     * @param       offset An offset into configuration space, of which bits 0-1 are ignored.
     * @param       data A 16-bit value to be written in host byte order.
     */
    void
    ConfigurationWrite16(uint64_t offset,
                         uint16_t data) LOCALONLY;

    /*!
     * @brief       Writes an 8-bit value to the PCI device's configuration space.
     * @discussion  This method writes an 8-bit value to a configuration space register on the device.
     * @param       offset An offset into configuration space, of which bits 0-1 are ignored.
     * @param       data An 8-bit value to be written in host byte order.
     */
    void
    ConfigurationWrite8(uint64_t offset,
                        uint8_t  data) LOCALONLY;

    /*!
     * @brief       Reads a 64-bit value from the PCI device's aperture at a given memory index.
     * @discussion  This method reads a 64-bit register on the device and returns its value. This is a blocking call.
     * @param       memoryIndex An index into the array of ranges assigned to the device.
     * @param       offset An offset into the device's memory specified by the index.
     * @param       readData An out parameter containing the 64-bit value in host byte order. -1 is written to readData on error.
     */
    void
    MemoryRead64(uint8_t   memoryIndex,
                 uint64_t  offset,
                 uint64_t* readData) LOCALONLY;

    /*!
     * @brief       Reads a 32-bit value from the PCI device's aperture at a given memory index.
     * @discussion  This method reads a 32-bit register on the device and returns its value. This is a blocking call.
     * @param       memoryIndex An index into the array of ranges assigned to the device.
     * @param       offset An offset into the device's memory specified by the index.
     * @param       readData An out parameter containing the 32-bit value in host byte order. -1 is written to readData on error.
     */
    void
    MemoryRead32(uint8_t   memoryIndex,
                 uint64_t  offset,
                 uint32_t* readData) LOCALONLY;

    /*!
     * @brief       Reads a 16-bit value from the PCI device's aperture at a given memory index.
     * @discussion  This method reads a 16-bit register on the device and returns its value. This is a blocking call.
     * @param       memoryIndex An index into the array of ranges assigned to the device.
     * @param       offset An offset into the device's memory specified by the index.
     * @param       readData An out parameter containing the 16-bit value in host byte order. -1 is written to readData on error.
     */
    void
    MemoryRead16(uint8_t   memoryIndex,
                 uint64_t  offset,
                 uint16_t* readData) LOCALONLY;

    /*!
     * @brief       Reads an 8-bit value from the PCI device's aperture at a given memory index.
     * @discussion  This method reads an 8-bit register on the device and returns its value. This is a blocking call.
     * @param       memoryIndex An index into the array of ranges assigned to the device.
     * @param       offset An offset into the device's memory specified by the index.
     * @param       readData An out parameter containing the 8-bit. -1 is written to readData on error.
     */
    void
    MemoryRead8(uint8_t  memoryIndex,
                uint64_t offset,
                uint8_t* readData) LOCALONLY;

    /*!
     * @brief       Writes a 64-bit value to the PCI device's aperture at a given memory index.
     * @discussion  This method writes a 64-bit register on the device and returns its value.
     * @param       memoryIndex An index into the array of ranges assigned to the device
     * @param       offset An offset into the device's memory specified by the index.
     * @param       data A 64-bit value to be written in host byte order.
     */
    void
    MemoryWrite64(uint8_t  memoryIndex,
                  uint64_t offset,
                  uint64_t data) LOCALONLY;

    /*!
     * @brief       Writes a 32-bit value to the PCI device's aperture at a given memory index.
     * @discussion  This method writes a 32-bit register on the device and returns its value.
     * @param       memoryIndex An index into the array of ranges assigned to the device.
     * @param       offset An offset into the device's memory specified by the index.
     * @param       data A 32-bit value to be written in host byte order.
     */
    void
    MemoryWrite32(uint8_t  memoryIndex,
                  uint64_t offset,
                  uint32_t data) LOCALONLY;

    /*!
     * @brief       Writes a 16-bit value to the PCI device's aperture at a given memory index.
     * @discussion  This method writes a 16-bit register on the device and returns its value.
     * @param       memoryIndex An index into the array of ranges assigned to the device.
     * @param       offset An offset into the device's memory specified by the index.
     * @param       data A 16-bit value to be written in host byte order.
     */
    void
    MemoryWrite16(uint8_t  memoryIndex,
                  uint64_t offset,
                  uint16_t data) LOCALONLY;

    /*!
     * @brief       Writes an 8-bit value to the PCI device's aperture at a given memory index.
     * @discussion  This method writes an 8-bit register on the device and returns its value.
     * @param       memoryIndex An index into the array of ranges assigned to the device
     * @param       offset An offset into the device's memory specified by the index.
     * @param       data An 8-bit value.
     */
    void
    MemoryWrite8(uint8_t  memoryIndex,
                 uint64_t offset,
                 uint8_t  data) LOCALONLY;

#pragma mark Configuration Space helpers

    /*!
     * @brief       Search configuration space for a PCI capability register.
     * @discussion  This method searches the device's configuration space for a PCI capability register matching the passed capability ID, if the device supports PCI capabilities.
     * @param       capabilityID A PCI capability ID. PCI Express devices may support extended capabilities in configuration space starting at offset 0x100.
     *              To search this space, the ID passed should be the negated value of the PCI-SIG assigned ID for the extended capability.
     * @param       searchOffset The offset into configuration space to start the next capability search.
     * @param       foundCapabilityOffset An optional out parameter to return the offset into configuration space where the capability was found. The offset can be used
     *              in the searchOffset parameter to find all capability blocks that may exist on the device with the same ID.
     * @result      kIOReturnSuccess if the capabilityID is found.
     */
    virtual kern_return_t
    FindPCICapability(uint32_t  capabilityID,
                      uint64_t  searchOffset,
                      uint64_t* foundCapabilityOffset);

    /*!
     * @brief       Accessor to return the PCI device's bus, device, and function number.
     * @discussion  This method is an accessor to return the PCI device's  bus, device and function number.
     * @param       returnBusNumber An optional out parameter to be filled with the device's bus number.
     * @param       returnDeviceNumber An optional out parameter to be filled with the device's device number.
     * @param       returnFunctionNumber An optional out parameter to be filled with the device's function number.
     * @result      kIOReturnSuccess if there were no errors.
     */
    virtual kern_return_t
    GetBusDeviceFunction(uint8_t* returnBusNumber,
                         uint8_t* returnDeviceNumber,
                         uint8_t* returnFunctionNumber);

#pragma mark Power Management

    /*!
     * @brief       Determine whether or not the device supports PCI Bus Power Management.
     * @discussion  This method will look at the device's capabilties registers and determine whether or not the device supports the PCI BUS Power Management Specification.
     * @param       state Check for support of a specific state (e.g. kPCIPMCPMESupportFromD3Cold). If state is 0, then check for a property in the registry
     *              which tells which state the hardware expects the device to go to during sleep.
     * @result      kIOReturnSuccess if the specified state is supported.
     */
    virtual kern_return_t
    HasPCIPowerManagement(uint64_t state);

    /*!
     * @brief       Enable PCI power management for sleep state
     * @discussion  This method will enable PCI Bus Powermanagement when going to sleep mode.
     * @param       state Enables PCI Power Management by placing the function in the given state (e.g. kPCIPMCSPowerStateD3). If state is 0xffffffff, then
     *              the IOPCIDevice determines the desired state. If state is kPCIPMCSPowerStateD0 (0) then PCI Power Management is disabled.
     * @result      kIOReturnSuccess if there were no errors.
     */
    virtual kern_return_t
    EnablePCIPowerManagement(uint64_t state);

#pragma mark State Management

    /*!
     * @brief       Save the PCIe state of the device.
     * @discussion  This method will save the device's Configuration Space into memory, to be restored at a later point (for example, after a Function Level Reset) with RestoreDeviceState().
     * @param       options See IOPCISaveDeviceStateOptions
     * @result      kIOReturnSuccess if the save was succesful.
     */
    virtual kern_return_t
    SaveDeviceState(IOOptionBits options = 0);

    /*!
     * @brief       Restore the PCIe state of the device.
     * @discussion  This method will restore the device's Configuration Space from memory, using the last saved state from SaveDeviceState().
     * @param       options Reserved for future use
     * @result      kIOReturnSuccess if there were no errors.
     */
    virtual kern_return_t
    RestoreDeviceState(IOOptionBits options = 0);

#pragma mark Memory Accessor helpers

    /*!
     * @brief       Lookup information related to the device BAR.
     * @discussion  This method returns the device memory index (used by the MemoryReadXX/MemoryWriteXX functions), type, and size of the user-specified BAR.
     * @param       barIndex See enum IOPCIMemoryRange.
     * @param       memoryIndex An out parameter to return the memory index, if the function is successful.
     * @param       barSize An out parameter to return the BAR size (bytes), if the function is successful. Not applicable for Expansion ROM.
     * @param       barType An out parameter to return the BAR type, if the function is successful. See enum IOPCIBARType. Not applicable for Expansion ROM.
     * @result      kIOReturnSuccess if the requested index is found.
     */
    virtual kern_return_t
    GetBARInfo(uint8_t   barIndex,
               uint8_t*  memoryIndex,
               uint64_t* barSize = 0,
               uint8_t*  barType = 0);
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOPCIDevice IOPCIDevice.iig:72-369 */

#define IOPCIDevice__ManageSession_ID            0xd395e45429887c65ULL
#define IOPCIDevice__CopyDeviceMemoryWithIndex_ID            0x8fbfd4a80b3ed3f1ULL
#define IOPCIDevice__MemoryAccess_ID            0x8d1327073fe3df0bULL
#define IOPCIDevice_FindPCICapability_ID            0xa41ed348e89e5c78ULL
#define IOPCIDevice_GetBusDeviceFunction_ID            0xee989f3b87e12497ULL
#define IOPCIDevice_HasPCIPowerManagement_ID            0xbad854791f46fcc0ULL
#define IOPCIDevice_EnablePCIPowerManagement_ID            0xfb030cc994f3e061ULL
#define IOPCIDevice_SaveDeviceState_ID            0xf6f91d3296a44c78ULL
#define IOPCIDevice_RestoreDeviceState_ID            0xa3652c7a818b5b57ULL
#define IOPCIDevice_GetBARInfo_ID            0x921a80175d4aa69dULL

#define IOPCIDevice__ManageSession_Args \
        IOService * forClient, \
        bool openClient, \
        IOOptionBits openOptions

#define IOPCIDevice__CopyDeviceMemoryWithIndex_Args \
        uint64_t memoryIndex, \
        IOMemoryDescriptor ** returnMemory, \
        IOService * forClient

#define IOPCIDevice__MemoryAccess_Args \
        uint64_t operation, \
        uint64_t offset, \
        uint64_t data, \
        uint64_t * readData, \
        IOService * forClient, \
        IOOptionBits options

#define IOPCIDevice_ClientCrashed_Args \
        IOService * client, \
        uint64_t options

#define IOPCIDevice_SetProperties_Args \
        OSDictionary * properties

#define IOPCIDevice_FindPCICapability_Args \
        uint32_t capabilityID, \
        uint64_t searchOffset, \
        uint64_t * foundCapabilityOffset

#define IOPCIDevice_GetBusDeviceFunction_Args \
        uint8_t * returnBusNumber, \
        uint8_t * returnDeviceNumber, \
        uint8_t * returnFunctionNumber

#define IOPCIDevice_HasPCIPowerManagement_Args \
        uint64_t state

#define IOPCIDevice_EnablePCIPowerManagement_Args \
        uint64_t state

#define IOPCIDevice_SaveDeviceState_Args \
        IOOptionBits options

#define IOPCIDevice_RestoreDeviceState_Args \
        IOOptionBits options

#define IOPCIDevice_GetBARInfo_Args \
        uint8_t barIndex, \
        uint8_t * memoryIndex, \
        uint64_t * barSize, \
        uint8_t * barType

#define IOPCIDevice_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOPCIDevice * self, const IORPC rpc);\
\
    kern_return_t\
    _ManageSession(\
        IOService * forClient,\
        bool openClient,\
        IOOptionBits openOptions,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _CopyDeviceMemoryWithIndex(\
        uint64_t memoryIndex,\
        IOMemoryDescriptor ** returnMemory,\
        IOService * forClient,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _MemoryAccess(\
        uint64_t operation,\
        uint64_t offset,\
        uint64_t data,\
        uint64_t * readData,\
        IOService * forClient,\
        IOOptionBits options,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Open(\
        IOService * forClient,\
        IOOptionBits options);\
\
    void\
    Close(\
        IOService * forClient,\
        IOOptionBits options);\
\
    void\
    ConfigurationRead32(\
        uint64_t offset,\
        uint32_t * readData);\
\
    void\
    ConfigurationRead16(\
        uint64_t offset,\
        uint16_t * readData);\
\
    void\
    ConfigurationRead8(\
        uint64_t offset,\
        uint8_t * readData);\
\
    void\
    ConfigurationWrite32(\
        uint64_t offset,\
        uint32_t data);\
\
    void\
    ConfigurationWrite16(\
        uint64_t offset,\
        uint16_t data);\
\
    void\
    ConfigurationWrite8(\
        uint64_t offset,\
        uint8_t data);\
\
    void\
    MemoryRead64(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint64_t * readData);\
\
    void\
    MemoryRead32(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint32_t * readData);\
\
    void\
    MemoryRead16(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint16_t * readData);\
\
    void\
    MemoryRead8(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint8_t * readData);\
\
    void\
    MemoryWrite64(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint64_t data);\
\
    void\
    MemoryWrite32(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint32_t data);\
\
    void\
    MemoryWrite16(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint16_t data);\
\
    void\
    MemoryWrite8(\
        uint8_t memoryIndex,\
        uint64_t offset,\
        uint8_t data);\
\
    kern_return_t\
    FindPCICapability(\
        uint32_t capabilityID,\
        uint64_t searchOffset,\
        uint64_t * foundCapabilityOffset,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetBusDeviceFunction(\
        uint8_t * returnBusNumber,\
        uint8_t * returnDeviceNumber,\
        uint8_t * returnFunctionNumber,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    HasPCIPowerManagement(\
        uint64_t state,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    EnablePCIPowerManagement(\
        uint64_t state,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SaveDeviceState(\
        IOOptionBits options = 0,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    RestoreDeviceState(\
        IOOptionBits options = 0,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetBARInfo(\
        uint8_t barIndex,\
        uint8_t * memoryIndex,\
        uint64_t * barSize = 0,\
        uint8_t * barType = 0,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*_ManageSession_Handler)(OSMetaClassBase * target, IOPCIDevice__ManageSession_Args);\
    static kern_return_t\
    _ManageSession_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _ManageSession_Handler func);\
\
    typedef kern_return_t (*_CopyDeviceMemoryWithIndex_Handler)(OSMetaClassBase * target, IOPCIDevice__CopyDeviceMemoryWithIndex_Args);\
    static kern_return_t\
    _CopyDeviceMemoryWithIndex_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _CopyDeviceMemoryWithIndex_Handler func);\
\
    typedef kern_return_t (*_MemoryAccess_Handler)(OSMetaClassBase * target, IOPCIDevice__MemoryAccess_Args);\
    static kern_return_t\
    _MemoryAccess_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _MemoryAccess_Handler func);\
\
    typedef kern_return_t (*FindPCICapability_Handler)(OSMetaClassBase * target, IOPCIDevice_FindPCICapability_Args);\
    static kern_return_t\
    FindPCICapability_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        FindPCICapability_Handler func);\
\
    typedef kern_return_t (*GetBusDeviceFunction_Handler)(OSMetaClassBase * target, IOPCIDevice_GetBusDeviceFunction_Args);\
    static kern_return_t\
    GetBusDeviceFunction_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetBusDeviceFunction_Handler func);\
\
    typedef kern_return_t (*HasPCIPowerManagement_Handler)(OSMetaClassBase * target, IOPCIDevice_HasPCIPowerManagement_Args);\
    static kern_return_t\
    HasPCIPowerManagement_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        HasPCIPowerManagement_Handler func);\
\
    typedef kern_return_t (*EnablePCIPowerManagement_Handler)(OSMetaClassBase * target, IOPCIDevice_EnablePCIPowerManagement_Args);\
    static kern_return_t\
    EnablePCIPowerManagement_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        EnablePCIPowerManagement_Handler func);\
\
    typedef kern_return_t (*SaveDeviceState_Handler)(OSMetaClassBase * target, IOPCIDevice_SaveDeviceState_Args);\
    static kern_return_t\
    SaveDeviceState_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SaveDeviceState_Handler func);\
\
    typedef kern_return_t (*RestoreDeviceState_Handler)(OSMetaClassBase * target, IOPCIDevice_RestoreDeviceState_Args);\
    static kern_return_t\
    RestoreDeviceState_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        RestoreDeviceState_Handler func);\
\
    typedef kern_return_t (*GetBARInfo_Handler)(OSMetaClassBase * target, IOPCIDevice_GetBARInfo_Args);\
    static kern_return_t\
    GetBARInfo_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetBARInfo_Handler func);\
\


#define IOPCIDevice_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    _ManageSession_Impl(IOPCIDevice__ManageSession_Args);\
\
    kern_return_t\
    _CopyDeviceMemoryWithIndex_Impl(IOPCIDevice__CopyDeviceMemoryWithIndex_Args);\
\
    kern_return_t\
    _MemoryAccess_Impl(IOPCIDevice__MemoryAccess_Args);\
\
    kern_return_t\
    ClientCrashed_Impl(IOService_ClientCrashed_Args);\
\
    kern_return_t\
    SetProperties_Impl(IOService_SetProperties_Args);\
\
    kern_return_t\
    FindPCICapability_Impl(IOPCIDevice_FindPCICapability_Args);\
\
    kern_return_t\
    GetBusDeviceFunction_Impl(IOPCIDevice_GetBusDeviceFunction_Args);\
\
    kern_return_t\
    HasPCIPowerManagement_Impl(IOPCIDevice_HasPCIPowerManagement_Args);\
\
    kern_return_t\
    EnablePCIPowerManagement_Impl(IOPCIDevice_EnablePCIPowerManagement_Args);\
\
    kern_return_t\
    SaveDeviceState_Impl(IOPCIDevice_SaveDeviceState_Args);\
\
    kern_return_t\
    RestoreDeviceState_Impl(IOPCIDevice_RestoreDeviceState_Args);\
\
    kern_return_t\
    GetBARInfo_Impl(IOPCIDevice_GetBARInfo_Args);\
\


#define IOPCIDevice_VirtualMethods \
\
public:\
\
    virtual bool\
    init(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    free(\
) APPLE_KEXT_OVERRIDE;\
\


#if !KERNEL

extern OSMetaClass          * gIOPCIDeviceMetaClass;
extern const OSClassLoadInformation IOPCIDevice_Class;

class IOPCIDeviceMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class IOPCIDeviceInterface : public OSInterface
{
public:
};

struct IOPCIDevice_IVars;
struct IOPCIDevice_LocalIVars;

class IOPCIDevice : public IOService, public IOPCIDeviceInterface
{
#if !KERNEL
    friend class IOPCIDeviceMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
#ifdef IOPCIDevice_DECLARE_IVARS
IOPCIDevice_DECLARE_IVARS
#else /* IOPCIDevice_DECLARE_IVARS */
    union
    {
        IOPCIDevice_IVars * ivars;
        IOPCIDevice_LocalIVars * lvars;
    };
#endif /* IOPCIDevice_DECLARE_IVARS */
#endif /* !KERNEL */

#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gIOPCIDeviceMetaClass; };
#endif /* KERNEL */

    using super = IOService;

#if !KERNEL
    IOPCIDevice_Methods
    IOPCIDevice_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#endif /* !__DOCUMENTATION__ */

/* IOPCIDevice.iig:371-372 */

#pragma mark Private Class Extension
/* IOPCIDevice.iig:395- */

#endif /* ! _IOKIT_UIOPCIDEVICE_H */
