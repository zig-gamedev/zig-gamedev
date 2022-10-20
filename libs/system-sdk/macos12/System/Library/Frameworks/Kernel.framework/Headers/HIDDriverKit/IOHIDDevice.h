/* iig(DriverKit-191.30.2) generated from IOHIDDevice.iig */

/* IOHIDDevice.iig:1-44 */
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
#include <IOKit/hid/IOHIDDevice.h>
#endif
#endif

#ifndef _HIDDRIVERKIT_IOHIDDEVICE_H
#define _HIDDRIVERKIT_IOHIDDEVICE_H

#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */
#include <HIDDriverKit/IOHIDDeviceTypes.h>
#include <DriverKit/IOTypes.h>

class IOMemoryDescriptor;

/* source class IOHIDDevice IOHIDDevice.iig:45-131 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

class KERNEL IOHIDDevice : public IOService
{

public:

    /*!
     * @function handleReport
     * @abstract Handle an asynchronous report received from the HID device.
     * @param report A memory descriptor that describes the report.
     * @param reportType The type of report.
     * @param options Options to specify the request. No options are
     * currently defined, and the default value is 0.
     * @result kIOReturnSuccess on success, or an error return otherwise.
     */

    virtual kern_return_t handleReport(uint64_t                  timestamp,
                                       IOMemoryDescriptor        *report,
                                       uint32_t                  reportLength,
                                       IOHIDReportType           reportType = kIOHIDReportTypeInput,
                                       IOOptionBits              options    = 0) LOCALONLY;


    
    /*!
     * @function getReport
     * @abstract Get a report from the HID device.
     * @param report A memory descriptor that describes the memory to store
     * the report read from the HID device.
     * @param reportType The report type.
     * @param options The lower 8 bits will represent the Report ID.  The
     * other 24 bits are options to specify the request.
     * @param completionTimeout Specifies an amount of time (in ms) after which
     * the command will be aborted if the entire command has not been completed.
     * @param action OSAction to call CompleteReport when request completes.
     * @result kIOReturnSuccess on success, or an error return otherwise.
     */
    
    virtual kern_return_t getReport(IOMemoryDescriptor      * report,
                                    IOHIDReportType         reportType,
                                    IOOptionBits            options,
                                    uint32_t                completionTimeout,
                                    OSAction                * action) LOCALONLY;
    
    
    /*!
     * @function setReport
     * @abstract Send a report to the HID device.
     * @param report A memory descriptor that describes the report to send
     * to the HID device.
     * @param reportType The report type.
     * @param options The lower 8 bits will represent the Report ID.  The
     * other 24 bits are options to specify the request.
     * @param completionTimeout Specifies an amount of time (in ms) after which
     * the command will be aborted if the entire command has not been completed.
     * @param action OSAction to call CompleteReport when request completes.
     * @result kIOReturnSuccess on success, or an error return otherwise.
     */
    
    virtual kern_return_t setReport(IOMemoryDescriptor      * report,
                                    IOHIDReportType         reportType,
                                    IOOptionBits            options,
                                    uint32_t                completionTimeout,
                                    OSAction                * action) LOCALONLY;

    
protected:

    /*!
     * @brief       Complete async requests made with GetReport/SetReport
     * @discussion  method should be called upon completion of GetReport/SetReport.
     * @param       action   passed to GetReport/SetReport.
     * @param       status   status of the completion.
     * @param       actualByteCount   used buffer size of the buffer submitted with SetReport/GetReport.
     * @return      none
     */

    virtual void  CompleteReport (OSAction            * action TARGET,
                                  IOReturn            status,
                                  uint32_t            actualByteCount) LOCAL = 0;

    

    virtual void setProperty (OSObject * key, OSObject * value) LOCALONLY;
    

public:
  
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOHIDDevice IOHIDDevice.iig:45-131 */

#define IOHIDDevice__Start_ID            0xd844e4e02f7103bbULL
#define IOHIDDevice__CompleteReport_ID            0xdc4ef5129e3a9d08ULL
#define IOHIDDevice__ProcessReport_ID            0xd539b835ea14880fULL
#define IOHIDDevice__HandleReport_ID            0xad138a788e1f3247ULL
#define IOHIDDevice__SetProperty_ID            0xa6ee936a6065f6baULL
#define IOHIDDevice_CompleteReport_ID            0xf2d80ecf6d8c6cb5ULL

#define IOHIDDevice__Start_Args \
        IOService * provider

#define IOHIDDevice__CompleteReport_Args \
        OSAction * action, \
        IOReturn status, \
        uint32_t actualByteCount

#define IOHIDDevice__ProcessReport_Args \
        HIDReportCommandType command, \
        IOMemoryDescriptor * report, \
        IOHIDReportType reportType, \
        IOOptionBits options, \
        uint32_t completionTimeout, \
        OSAction * action

#define IOHIDDevice__HandleReport_Args \
        uint64_t timestamp, \
        IOMemoryDescriptor * report, \
        uint32_t reportLength, \
        IOHIDReportType reportType, \
        IOOptionBits options

#define IOHIDDevice__SetProperty_Args \
        IOBufferMemoryDescriptor * serialization

#define IOHIDDevice_CompleteReport_Args \
        OSAction * action, \
        IOReturn status, \
        uint32_t actualByteCount

#define IOHIDDevice_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOHIDDevice * self, const IORPC rpc);\
\
    kern_return_t\
    _Start(\
        IOService * provider,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateAction_CompleteReport(size_t referenceSize, OSAction ** action);\
\
    void\
    _ProcessReport(\
        HIDReportCommandType command,\
        IOMemoryDescriptor * report,\
        IOHIDReportType reportType,\
        IOOptionBits options,\
        uint32_t completionTimeout,\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _HandleReport(\
        uint64_t timestamp,\
        IOMemoryDescriptor * report,\
        uint32_t reportLength,\
        IOHIDReportType reportType = kIOHIDReportTypeInput,\
        IOOptionBits options = 0,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    _SetProperty(\
        IOBufferMemoryDescriptor * serialization,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    CompleteReport(\
        OSAction * action,\
        IOReturn status,\
        uint32_t actualByteCount,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    void\
    _ProcessReport_Impl(IOHIDDevice__ProcessReport_Args);\
\
    void\
    _SetProperty_Impl(IOHIDDevice__SetProperty_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef kern_return_t (*_Start_Handler)(OSMetaClassBase * target, IOHIDDevice__Start_Args);\
    static kern_return_t\
    _Start_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _Start_Handler func);\
\
    typedef void (*_ProcessReport_Handler)(OSMetaClassBase * target, IOHIDDevice__ProcessReport_Args);\
    static kern_return_t\
    _ProcessReport_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _ProcessReport_Handler func);\
\
    typedef kern_return_t (*_HandleReport_Handler)(OSMetaClassBase * target, IOHIDDevice__HandleReport_Args);\
    static kern_return_t\
    _HandleReport_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _HandleReport_Handler func);\
\
    typedef void (*_SetProperty_Handler)(OSMetaClassBase * target, IOHIDDevice__SetProperty_Args);\
    static kern_return_t\
    _SetProperty_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _SetProperty_Handler func);\
\
    typedef void (*CompleteReport_Handler)(OSMetaClassBase * target, IOHIDDevice_CompleteReport_Args);\
    static kern_return_t\
    CompleteReport_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CompleteReport_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    CompleteReport_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CompleteReport_Handler func);\
\


#define IOHIDDevice_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    _Start_Impl(IOHIDDevice__Start_Args);\
\
    void\
    _CompleteReport_Impl(IOHIDDevice__CompleteReport_Args);\
\
    kern_return_t\
    _HandleReport_Impl(IOHIDDevice__HandleReport_Args);\
\


#define IOHIDDevice_VirtualMethods \
\
public:\
\
    virtual kern_return_t\
    handleReport(\
        uint64_t timestamp,\
        IOMemoryDescriptor * report,\
        uint32_t reportLength,\
        IOHIDReportType reportType = kIOHIDReportTypeInput,\
        IOOptionBits options = 0) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    getReport(\
        IOMemoryDescriptor * report,\
        IOHIDReportType reportType,\
        IOOptionBits options,\
        uint32_t completionTimeout,\
        OSAction * action) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    setReport(\
        IOMemoryDescriptor * report,\
        IOHIDReportType reportType,\
        IOOptionBits options,\
        uint32_t completionTimeout,\
        OSAction * action) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    setProperty(\
        OSObject * key,\
        OSObject * value) APPLE_KEXT_OVERRIDE;\
\


#if !KERNEL

extern OSMetaClass          * gIOHIDDeviceMetaClass;
extern const OSClassLoadInformation IOHIDDevice_Class;

class IOHIDDeviceMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class IOHIDDeviceInterface : public OSInterface
{
public:
    virtual kern_return_t
    handleReport(uint64_t timestamp,
        IOMemoryDescriptor * report,
        uint32_t reportLength,
        IOHIDReportType reportType,
        IOOptionBits options) = 0;

    virtual kern_return_t
    getReport(IOMemoryDescriptor * report,
        IOHIDReportType reportType,
        IOOptionBits options,
        uint32_t completionTimeout,
        OSAction * action) = 0;

    virtual kern_return_t
    setReport(IOMemoryDescriptor * report,
        IOHIDReportType reportType,
        IOOptionBits options,
        uint32_t completionTimeout,
        OSAction * action) = 0;

    virtual void
    setProperty(OSObject * key,
        OSObject * value) = 0;

    kern_return_t
    handleReport_Call(uint64_t timestamp,
        IOMemoryDescriptor * report,
        uint32_t reportLength,
        IOHIDReportType reportType,
        IOOptionBits options)  { return handleReport(timestamp, report, reportLength, reportType, options); };\

    kern_return_t
    getReport_Call(IOMemoryDescriptor * report,
        IOHIDReportType reportType,
        IOOptionBits options,
        uint32_t completionTimeout,
        OSAction * action)  { return getReport(report, reportType, options, completionTimeout, action); };\

    kern_return_t
    setReport_Call(IOMemoryDescriptor * report,
        IOHIDReportType reportType,
        IOOptionBits options,
        uint32_t completionTimeout,
        OSAction * action)  { return setReport(report, reportType, options, completionTimeout, action); };\

    void
    setProperty_Call(OSObject * key,
        OSObject * value)  { return setProperty(key, value); };\

};

struct IOHIDDevice_IVars;
struct IOHIDDevice_LocalIVars;

class IOHIDDevice : public IOService, public IOHIDDeviceInterface
{
#if !KERNEL
    friend class IOHIDDeviceMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
#ifdef IOHIDDevice_DECLARE_IVARS
IOHIDDevice_DECLARE_IVARS
#else /* IOHIDDevice_DECLARE_IVARS */
    union
    {
        IOHIDDevice_IVars * ivars;
        IOHIDDevice_LocalIVars * lvars;
    };
#endif /* IOHIDDevice_DECLARE_IVARS */
#endif /* !KERNEL */

#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gIOHIDDeviceMetaClass; };
#endif /* KERNEL */

    using super = IOService;

#if !KERNEL
    IOHIDDevice_Methods
    IOHIDDevice_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#define OSAction_IOHIDDevice__CompleteReport_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(OSAction_IOHIDDevice__CompleteReport * self, const IORPC rpc);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\


#define OSAction_IOHIDDevice__CompleteReport_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define OSAction_IOHIDDevice__CompleteReport_VirtualMethods \
\
public:\
\


#if !KERNEL

extern OSMetaClass          * gOSAction_IOHIDDevice__CompleteReportMetaClass;
extern const OSClassLoadInformation OSAction_IOHIDDevice__CompleteReport_Class;

class OSAction_IOHIDDevice__CompleteReportMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

class OSAction_IOHIDDevice__CompleteReportInterface : public OSInterface
{
public:
};

struct OSAction_IOHIDDevice__CompleteReport_IVars;
struct OSAction_IOHIDDevice__CompleteReport_LocalIVars;

class __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer"))) OSAction_IOHIDDevice__CompleteReport : public OSAction, public OSAction_IOHIDDevice__CompleteReportInterface
{
#if KERNEL
    OSDeclareDefaultStructorsWithDispatch(OSAction_IOHIDDevice__CompleteReport);
#endif /* KERNEL */

#if !KERNEL
    friend class OSAction_IOHIDDevice__CompleteReportMetaClass;
#endif /* !KERNEL */

public:
#ifdef OSAction_IOHIDDevice__CompleteReport_DECLARE_IVARS
OSAction_IOHIDDevice__CompleteReport_DECLARE_IVARS
#else /* OSAction_IOHIDDevice__CompleteReport_DECLARE_IVARS */
    union
    {
        OSAction_IOHIDDevice__CompleteReport_IVars * ivars;
        OSAction_IOHIDDevice__CompleteReport_LocalIVars * lvars;
    };
#endif /* OSAction_IOHIDDevice__CompleteReport_DECLARE_IVARS */
#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gOSAction_IOHIDDevice__CompleteReportMetaClass; };
    virtual const OSMetaClass *
    getMetaClass() const APPLE_KEXT_OVERRIDE { return gOSAction_IOHIDDevice__CompleteReportMetaClass; };
#endif /* KERNEL */

    using super = OSAction;

#if !KERNEL
    OSAction_IOHIDDevice__CompleteReport_Methods
#endif /* !KERNEL */

    OSAction_IOHIDDevice__CompleteReport_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */


/* IOHIDDevice.iig:188- */


#endif /* ! _HIDDRIVERKIT_IOHIDDEVICE_H */
