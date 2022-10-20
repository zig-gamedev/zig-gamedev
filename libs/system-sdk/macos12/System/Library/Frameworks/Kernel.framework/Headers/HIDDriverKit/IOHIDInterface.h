/* iig(DriverKit-191.30.2) generated from IOHIDInterface.iig */

/* IOHIDInterface.iig:1-46 */
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

#if !__IIG
#if KERNEL
#include <IOKit/hid/IOHIDInterface.h>
#endif
#endif

#ifndef _HIDDRIVERKIT_IOHIDINTERFACE_H
#define _HIDDRIVERKIT_IOHIDINTERFACE_H

#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */
#include <DriverKit/IOMemoryDescriptor.h>  /* .iig include */
#include <DriverKit/IOBufferMemoryDescriptor.h>  /* .iig include */
#include <HIDDriverKit/IOHIDDeviceTypes.h>
#include <DriverKit/IOTypes.h>

class OSArray;

/* source class IOHIDInterface IOHIDInterface.iig:47-285 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
 *   @class IOHIDInterface
 *   @abstract Abstract interface to HID device functionality
 *   @discussion Following object properties can be used to identify/match interface and its characteristics (see  IOHIDDeviceKeys.h )
 *    kIOHIDReportIntervalKey
 *    kIOHIDVendorIDKey
 *    kIOHIDProductIDKey
 *    kIOHIDTransportKey
 *    kIOHIDVersionNumberKey
 *    kIOHIDCountryCodeKey
 *    kIOHIDLocationIDKey
 *    kIOHIDManufacturerKey
 *    kIOHIDProductKey
 *    kIOHIDSerialNumberKey
 *    kIOHIDRequestTimeoutKey
*/

class KERNEL IOHIDInterface : public IOService
{
public:
    /*!
     * @function init
     *
     * @abstract
     * Initializes IOHIDInterface object.
     *
     * @return
     * true on success.
     */
    virtual bool init() override;
    
    /*!
     * @function free
     *
     * @abstract
     * frees the IOHIDInterface object.
     */
    virtual void free() override;
    
    /*!
     * @function ReportAvailable
     *
     * @abstract
     * Callback invoked when an input report is received from the device.
     *
     * @param timestamp
     * The timestamp of the report.
     *
     * @param reportID
     * The report ID.
     *
     * @param reportLength
     * The length of the report.
     *
     * @param type
     * The report type.
     *
     * @param report
     * A memory descriptor that describes the report.
     *
     * @param action
     * The OSAction object that will handle the asynchronous report callback.
     */
    virtual void ReportAvailable(uint64_t timestamp,
                                 uint32_t reportID,
                                 uint32_t reportLength,
                                 IOHIDReportType type,
                                 IOMemoryDescriptor *report,
                                 OSAction *action TARGET) LOCAL = 0;
    
    /*!
     * @function AddReportToPool
     *
     * @abstract
     * Adds a memory descriptor to the report pool.
     *
     * @param report
     * A memory descriptor large enough to hold input reports.
     *
     * @return
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t AddReportToPool(IOBufferMemoryDescriptor *report);
    
    /*!
     * @function Open
     *
     * @abstract
     * Opens the interface in order to receive input reports.
     *
     * @param forClient
     * The client opening the IOHIDInterface.
     *
     * @param options
     * Options to pass to the interface.
     *
     * @param action
     * The OSAction object that will handle the asynchronous report callback.
     *
     * @return
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t Open(IOService *forClient,
                               IOOptionBits options,
                               OSAction *action TYPE(ReportAvailable));
    
    /*!
     * @function Close
     *
     * @abstract
     * Closes the IOHIDInterface, stopping invocation of report callbacks.
     *
     * @param forClient
     * The client closing the IOHIDInterface.
     *
     * @param options
     * Options to pass to the interface.
     *
     * @return
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t Close(IOService *forClient,
                                IOOptionBits options);
    
    /*!
     * @function SetReport
     *
     * @abstract
     * Send a report to the HID device.
     *
     * @param report
     * A memory descriptor that describes the report to send
     * to the HID device.
     *
     * @param reportType
     * The report type.
     *
     * @param reportID
     * the report ID.
     *
     * @param options
     * The lower 8 bits will represent the Report ID. The other 24 bits are
     * options to specify the request.
     *
     * @return
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t SetReport (IOMemoryDescriptor *        report,
                                     IOHIDReportType             reportType,
                                     uint32_t                    reportID            = 0,
                                     IOOptionBits                options             = 0);
    
    /*!
     * @function GetReport
     *
     * @abstract
     * Get a report from the HID device.
     *
     * @param report
     * A memory descriptor that describes the memory to store the report read
     * from the HID device.
     *
     * @param reportType
     * The report type.
     *
     * @param reportID
     * The report ID.
     *
     * @param options
     * The lower 8 bits will represent the Report ID. The other 24 bits are
     * options to specify the request.
     *
     * @return
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t GetReport (IOMemoryDescriptor *        report,
                                     IOHIDReportType             reportType,
                                     uint32_t                    reportID            = 0,
                                     IOOptionBits                options             = 0);
    
    /*!
     * @function processReport
     *
     * @abstract
     * Processes the report received from the HandleReportCallback function.
     * Will update all element values (returned from the getElements function)
     * from the new report data.
     *
     * @param timestamp
     * The timestamp of the report.
     *
     * @param report
     * Bytes that describes the report.
     *
     * @param reportLength
     * The length of the report.
     *
     * @param type
     * The report type.
     *
     * @param reportID
     * The report ID.
     */
    virtual void processReport(uint64_t timestamp,
                               uint8_t *report,
                               uint32_t reportLength,
                               IOHIDReportType type,
                               uint32_t reportID) LOCALONLY;
    
    /*!
     * @function getElements
     *
     * @abstract
     * Returns an array of IOHIDElement objects. The element values will be
     * updates on calls to the processReport() function.
     *
     * @return
     * Returns an array of IOHIDElement objects.
     */
    virtual OSArray *getElements() LOCALONLY;
    
    /*!
     * @function commitElements
     *
     * @abstract
     * Commits an array of IOHIDElements to/from the device.
     *
     * @param elements
     * An array of IOHIDElement objects. 
     *
     * @param direction
     * The direction to commit the elements. Directions are defined in the
     * IOHIDElementCommitDirection enumerator in <IOKit/hid/IOHIDKeys.h>.
     *
     * @result
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t commitElements(OSArray *elements,
                                         IOHIDElementCommitDirection direction) LOCALONLY;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOHIDInterface IOHIDInterface.iig:47-285 */

#define IOHIDInterface_SendDebugBuffer_ID            0x9cd93af0504ec45dULL
#define IOHIDInterface_GetElementValues_ID            0xc4a89d6fd6fd065cULL
#define IOHIDInterface_SetElementValues_ID            0xf964ed7a299eb3a5ULL
#define IOHIDInterface_GetSupportedCookies_ID            0xec13cf336ecceaccULL
#define IOHIDInterface_ReportAvailable_ID            0xd46e690536704302ULL
#define IOHIDInterface_AddReportToPool_ID            0xbe04e3d42572fd53ULL
#define IOHIDInterface_Open_ID            0x9f3263b463025b4fULL
#define IOHIDInterface_Close_ID            0x9bcf029fa80af1b5ULL
#define IOHIDInterface_SetReport_ID            0xca9744a1af98f7dfULL
#define IOHIDInterface_GetReport_ID            0xa8ccbd83dbbed4cdULL

#define IOHIDInterface_SendDebugBuffer_Args \
        IOMemoryDescriptor * debug

#define IOHIDInterface_GetElementValues_Args \
        uint32_t count, \
        IOMemoryDescriptor * elementValues

#define IOHIDInterface_SetElementValues_Args \
        uint32_t count, \
        IOMemoryDescriptor * elementValues

#define IOHIDInterface_GetSupportedCookies_Args \
        IOBufferMemoryDescriptor ** cookies

#define IOHIDInterface_ReportAvailable_Args \
        uint64_t timestamp, \
        uint32_t reportID, \
        uint32_t reportLength, \
        IOHIDReportType type, \
        IOMemoryDescriptor * report, \
        OSAction * action

#define IOHIDInterface_AddReportToPool_Args \
        IOBufferMemoryDescriptor * report

#define IOHIDInterface_Open_Args \
        IOService * forClient, \
        IOOptionBits options, \
        OSAction * action

#define IOHIDInterface_Close_Args \
        IOService * forClient, \
        IOOptionBits options

#define IOHIDInterface_SetReport_Args \
        IOMemoryDescriptor * report, \
        IOHIDReportType reportType, \
        uint32_t reportID, \
        IOOptionBits options

#define IOHIDInterface_GetReport_Args \
        IOMemoryDescriptor * report, \
        IOHIDReportType reportType, \
        uint32_t reportID, \
        IOOptionBits options

#define IOHIDInterface_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOHIDInterface * self, const IORPC rpc);\
\
    kern_return_t\
    SendDebugBuffer(\
        IOMemoryDescriptor * debug,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetElementValues(\
        uint32_t count,\
        IOMemoryDescriptor * elementValues,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetElementValues(\
        uint32_t count,\
        IOMemoryDescriptor * elementValues,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetSupportedCookies(\
        __attribute__((os_returns_retained)) IOBufferMemoryDescriptor ** cookies,\
        OSDispatchMethod supermethod = NULL);\
\
    bool\
    createElements(\
);\
\
    void\
    ReportAvailable(\
        uint64_t timestamp,\
        uint32_t reportID,\
        uint32_t reportLength,\
        IOHIDReportType type,\
        IOMemoryDescriptor * report,\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    AddReportToPool(\
        IOBufferMemoryDescriptor * report,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Open(\
        IOService * forClient,\
        IOOptionBits options,\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    Close(\
        IOService * forClient,\
        IOOptionBits options,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetReport(\
        IOMemoryDescriptor * report,\
        IOHIDReportType reportType,\
        uint32_t reportID = 0,\
        IOOptionBits options = 0,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    GetReport(\
        IOMemoryDescriptor * report,\
        IOHIDReportType reportType,\
        uint32_t reportID = 0,\
        IOOptionBits options = 0,\
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
    typedef kern_return_t (*SendDebugBuffer_Handler)(OSMetaClassBase * target, IOHIDInterface_SendDebugBuffer_Args);\
    static kern_return_t\
    SendDebugBuffer_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SendDebugBuffer_Handler func);\
\
    typedef kern_return_t (*GetElementValues_Handler)(OSMetaClassBase * target, IOHIDInterface_GetElementValues_Args);\
    static kern_return_t\
    GetElementValues_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetElementValues_Handler func);\
\
    typedef kern_return_t (*SetElementValues_Handler)(OSMetaClassBase * target, IOHIDInterface_SetElementValues_Args);\
    static kern_return_t\
    SetElementValues_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetElementValues_Handler func);\
\
    typedef kern_return_t (*GetSupportedCookies_Handler)(OSMetaClassBase * target, IOHIDInterface_GetSupportedCookies_Args);\
    static kern_return_t\
    GetSupportedCookies_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetSupportedCookies_Handler func);\
\
    typedef void (*ReportAvailable_Handler)(OSMetaClassBase * target, IOHIDInterface_ReportAvailable_Args);\
    static kern_return_t\
    ReportAvailable_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        ReportAvailable_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    ReportAvailable_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        ReportAvailable_Handler func);\
\
    typedef kern_return_t (*AddReportToPool_Handler)(OSMetaClassBase * target, IOHIDInterface_AddReportToPool_Args);\
    static kern_return_t\
    AddReportToPool_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        AddReportToPool_Handler func);\
\
    typedef kern_return_t (*Open_Handler)(OSMetaClassBase * target, IOHIDInterface_Open_Args);\
    static kern_return_t\
    Open_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Open_Handler func);\
\
    typedef kern_return_t (*Close_Handler)(OSMetaClassBase * target, IOHIDInterface_Close_Args);\
    static kern_return_t\
    Close_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        Close_Handler func);\
\
    typedef kern_return_t (*SetReport_Handler)(OSMetaClassBase * target, IOHIDInterface_SetReport_Args);\
    static kern_return_t\
    SetReport_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetReport_Handler func);\
\
    typedef kern_return_t (*GetReport_Handler)(OSMetaClassBase * target, IOHIDInterface_GetReport_Args);\
    static kern_return_t\
    GetReport_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        GetReport_Handler func);\
\


#define IOHIDInterface_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    SendDebugBuffer_Impl(IOHIDInterface_SendDebugBuffer_Args);\
\
    kern_return_t\
    GetElementValues_Impl(IOHIDInterface_GetElementValues_Args);\
\
    kern_return_t\
    SetElementValues_Impl(IOHIDInterface_SetElementValues_Args);\
\
    kern_return_t\
    GetSupportedCookies_Impl(IOHIDInterface_GetSupportedCookies_Args);\
\
    kern_return_t\
    AddReportToPool_Impl(IOHIDInterface_AddReportToPool_Args);\
\
    kern_return_t\
    Open_Impl(IOHIDInterface_Open_Args);\
\
    kern_return_t\
    Close_Impl(IOHIDInterface_Close_Args);\
\
    kern_return_t\
    SetReport_Impl(IOHIDInterface_SetReport_Args);\
\
    kern_return_t\
    GetReport_Impl(IOHIDInterface_GetReport_Args);\
\


#define IOHIDInterface_VirtualMethods \
\
public:\
\
    virtual kern_return_t\
    getElementValues(\
        OSArray * elements) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    setElementValues(\
        OSArray * elements) APPLE_KEXT_OVERRIDE;\
\
    virtual bool\
    init(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    free(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    processReport(\
        uint64_t timestamp,\
        uint8_t * report,\
        uint32_t reportLength,\
        IOHIDReportType type,\
        uint32_t reportID) APPLE_KEXT_OVERRIDE;\
\
    virtual OSArray *\
    getElements(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    commitElements(\
        OSArray * elements,\
        IOHIDElementCommitDirection direction) APPLE_KEXT_OVERRIDE;\
\


#if !KERNEL

extern OSMetaClass          * gIOHIDInterfaceMetaClass;
extern const OSClassLoadInformation IOHIDInterface_Class;

class IOHIDInterfaceMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class IOHIDInterfaceInterface : public OSInterface
{
public:
    virtual kern_return_t
    getElementValues(OSArray * elements) = 0;

    virtual kern_return_t
    setElementValues(OSArray * elements) = 0;

    virtual void
    processReport(uint64_t timestamp,
        uint8_t * report,
        uint32_t reportLength,
        IOHIDReportType type,
        uint32_t reportID) = 0;

    virtual OSArray *
    getElements() = 0;

    virtual kern_return_t
    commitElements(OSArray * elements,
        IOHIDElementCommitDirection direction) = 0;

    kern_return_t
    getElementValues_Call(OSArray * elements)  { return getElementValues(elements); };\

    kern_return_t
    setElementValues_Call(OSArray * elements)  { return setElementValues(elements); };\

    void
    processReport_Call(uint64_t timestamp,
        uint8_t * report,
        uint32_t reportLength,
        IOHIDReportType type,
        uint32_t reportID)  { return processReport(timestamp, report, reportLength, type, reportID); };\

    OSArray *
    getElements_Call()  { return getElements(); };\

    kern_return_t
    commitElements_Call(OSArray * elements,
        IOHIDElementCommitDirection direction)  { return commitElements(elements, direction); };\

};

struct IOHIDInterface_IVars;
struct IOHIDInterface_LocalIVars;

class IOHIDInterface : public IOService, public IOHIDInterfaceInterface
{
#if !KERNEL
    friend class IOHIDInterfaceMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
#ifdef IOHIDInterface_DECLARE_IVARS
IOHIDInterface_DECLARE_IVARS
#else /* IOHIDInterface_DECLARE_IVARS */
    union
    {
        IOHIDInterface_IVars * ivars;
        IOHIDInterface_LocalIVars * lvars;
    };
#endif /* IOHIDInterface_DECLARE_IVARS */
#endif /* !KERNEL */

#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gIOHIDInterfaceMetaClass; };
#endif /* KERNEL */

    using super = IOService;

#if !KERNEL
    IOHIDInterface_Methods
    IOHIDInterface_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#endif /* !__DOCUMENTATION__ */


/* IOHIDInterface.iig:304- */

#endif /* ! _HIDDRIVERKIT_IOHIDINTERFACE_H */
