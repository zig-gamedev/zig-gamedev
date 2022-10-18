/* iig(DriverKit-191.30.2) generated from IOHIDEventService.iig */

/* IOHIDEventService.iig:1-48 */
/*
 * Copyright (c) 2018-2020 Apple Inc. All rights reserved.
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
#include <IOKit/hidevent/IOHIDEventService.h>
#endif
#endif

#ifndef _HIDDRIVERKIT_IOHIDEVENTSERVICE_H
#define _HIDDRIVERKIT_IOHIDEVENTSERVICE_H

#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */
#include <DriverKit/IOTypes.h>

class IOBufferMemoryDescriptor;
class IOHIDEvent;

typedef struct IOHIDDigitizerStylusData IOHIDDigitizerStylusData;
typedef struct IOHIDDigitizerTouchData IOHIDDigitizerTouchData;


/* source class IOHIDEventService IOHIDEventService.iig:49-381 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

class KERNEL IOHIDEventService : public IOService
{

public:
    /*!
     * @function init
     *
     * @abstract
     * Initializes IOHIDEventService object.
     *
     * @return
     * true on success.
     */
    virtual bool init() override;

    /*!
     * @function free
     *
     * @abstract
     * frees the object.
     */
    virtual void free() override;

    /*!
     * @function Start
     *
     * @abstract
     * Function called after initialization to start the IOService.
     *
     * @discussion
     * Override this function to perform any needed driver initialization before
     * main functionality begins. Calling the parent's Start in a child's Start is required.
     * If the parent's Start returns an error or the child encounters an error in Start after
     * the parent's Start has been called, Stop must immediately be called.
     *
     * @param provider
     * The IOService that is provider for this object.
     *
     * @return
     * returns kIOReturnSuccess on success.
     */
    virtual kern_return_t Start(IOService * provider) override;

    /*!
     * @function Stop
     *
     * @abstract
     * Function called during termination to stop the IOService.
     *
     * @discussion
     * Override this function to perform any needed driver teardown before
     * the process exits. Calling the parent's Stop in a child's Stop is required.
     * Do not call Stop if the parent's Start has not yet been called.
     * Stop should only be called once.
     *
     * @param provider
     * The IOService that is provider for this object.
     *
     * @return
     * returns kIOReturnSuccess on success.
     */
    virtual kern_return_t Stop(IOService * provider) override;

    /*!
     * @function DispatchKeyboardEvent
     *
     * @abstract
     * Dispatches a keyboard event with the provided usage page, usage, and
     * value.
     *
     * @param timestamp
     * The timestamp of the event.
     *
     * @param usagePage
     * The usage page. Usage pages are defined in
     * <IOKit/hid/IOHIDUsageTables.h>.
     *
     * @param usage
     * The usage. Usages are defined in <IOKit/hid/IOHIDUsageTables.h>.
     *
     * @param value
     * The value.
     *
     * @param options
     * Optional option bits. Options are defined in the IOHIDKeyboardEventOptions
     * enumerator in <IOKit/hid/IOHIDKeys.h>
     *
     * @param repeat
     * Default behavior for keyboard events is to repeat keys if the key has been
     * held down for a certain amount of time defined in system preferences. Pass
     * in false to not apply key repeat logic to this event.
     */
    virtual kern_return_t dispatchKeyboardEvent(uint64_t timeStamp,
                                                uint32_t usagePage,
                                                uint32_t usage,
                                                uint32_t value,
                                                IOOptionBits options,
                                                bool repeat = true) LOCALONLY;

    /*!
     * @function DispatchRelativePointerEvent
     *
     * @abstract
     * Dispatches a relative pointer event.
     *
     * @param timestamp
     * The timestamp of the event.
     *
     * @param dx
     * The delta X value.
     *
     * @param dy
     * The delta Y value.
     *
     * @param buttonState
     * Current state of the buttons, if any.
     *
     * @param options
     * Optional option bits.
     *
     * @param accelerate
     * Pointer events are subject to an acceleration algorithm. Pass in false
     * if you do not wish to have acceleration logic applied to the pointer event.
     */
    virtual kern_return_t dispatchRelativePointerEvent(uint64_t timeStamp,
                                                       IOFixed dx,
                                                       IOFixed dy,
                                                       uint32_t buttonState,
                                                       IOOptionBits options,
                                                       bool accelerate = true) LOCALONLY;
    
    /*!
     * @function DispatchAbsolutePointerEvent
     *
     * @abstract
     * Dispatches an absolute pointer event.
     *
     * @param timeStamp
     * The timestamp of the event.
     *
     * @param x
     * An X value between 0 and 1.
     *
     * @param dy
     * A Y value between 0 and 1.
     *
     * @param buttonState
     * Current state of the buttons, if any.
     *
     * @param options
     * Optional option bits.
     *
     * @param accelerate
     * Pointer events are subject to an acceleration algorithm. Pass in false
     * if you do not wish to have acceleration logic applied to the pointer event.
     */
    virtual kern_return_t dispatchAbsolutePointerEvent(uint64_t timeStamp,
                                                       IOFixed x,
                                                       IOFixed y,
                                                       uint32_t buttonState,
                                                       IOOptionBits options,
                                                       bool accelerate = true) LOCALONLY;
    
    /*!
     * @function DispatchRelativeScrollWheelEvent
     *
     * @abstract
     * Dispatches a relative scroll wheel event.
     *
     * @param timestamp
     * The timestamp of the event.
     *
     * @param dx
     * The delta X value.
     *
     * @param dy
     * The delta Y value.
     *
     * @param dz
     * The delta Z value.
     *
     * @param options
     * Optional option bits.
     *
     * @param accelerate
     * Scroll events are subject to an acceleration algorithm. Pass in false
     * if you do not wish to have acceleration logic applied to the scroll event.
     */
    virtual kern_return_t dispatchRelativeScrollWheelEvent(uint64_t timeStamp,
                                                           IOFixed dx,
                                                           IOFixed dy,
                                                           IOFixed dz,
                                                           IOOptionBits options,
                                                           bool accelerate = true) LOCALONLY;
    
    /*!
     * @function dispatchDigitizerStylusEvent
     *
     * @abstract
     * Dispatches a digitizer stylus event.
     *
     * @param timestamp
     * The timestamp of the event.
     *
     * @param stylusData
     * A struct containing the stylus data. See IOHIDDigtizerStructs.h for more 
     * info.
     */
    virtual kern_return_t dispatchDigitizerStylusEvent(
                                        uint64_t timeStamp,
                                        IOHIDDigitizerStylusData *stylusData) LOCALONLY;
    
    /*!
     * @function dispatchDigitizerTouchEvent
     *
     * @abstract
     * Dispatches a digitizer touch pad event.
     *
     * @param timestamp
     * The timestamp of the event.
     *
     * @param touchData
     * An array of touch data, where each struct represents a surface contact. 
     * See IOHIDDigtizerStructs.h for more info.
     *
     * @param touchDataCount
     * The touchData array count.
     */
    virtual kern_return_t dispatchDigitizerTouchEvent(
                                            uint64_t timeStamp,
                                            IOHIDDigitizerTouchData *touchData,
                                            uint32_t touchDataCount) LOCALONLY;

protected:

    /*!
     * @function SetLED
     *
     * @abstract
     * DEPRECATED. Prefer SetLEDState instead.
     * Sets an LED on the service.
     *
     * @discussion
     * By default if no implementation of SetLEDState is provided by
     * the DriverKit Driver, SetLED will be called instead by a 
     * default implementation in IOUserHIDEventService. The
     * IOUserHIDEventService implementation always returns
     * kIOReturnSuccess if the usagePage for SetLEDState is @{kHIDPage_LEDs}.
     *
     * @param usage
     * The LED usage to set. LED usages can be found in
     * <IOKit/hid/IOHIDUsageTables.h>.
     *
     * @param on
     * Turn on or off the LED.
     */
    virtual void SetLED(uint32_t usage, bool on) LOCAL;

    /*!
     * @function SetLEDState
     *
     * @abstract
     * Sets an LED on the service.
     *
     * @discussion
     * If there is no matching LED, then kIOReturnUnsupported should be returned, so that the UserClient
     * can be signaled that the usage requested doesn't exist. If the LED exists it should return
     * kIOReturnSuccess.
     *
     * @param usagePage
     * The usage page of the LED requested to set.
     *
     * @param usage
     * The LED usage to set. LED usages can be found in
     * <IOKit/hid/IOHIDUsageTables.h>.
     *
     * @param on
     * Turn on or off the LED.
     */
    virtual kern_return_t SetLEDState(uint32_t usagePage, uint32_t usage, bool on) LOCAL;
    
    /*!
     * @function dispatchEvent
     *
     * @abstract
     * Dispatches an event.
     *
     * @param event
     * The IOHIDEvent to dispatch.
     */
    virtual void dispatchEvent(IOHIDEvent *event) LOCALONLY;

    /*!
     * @function handleCopyMatchingEvent
     *
     * @abstract
     * copy event that match parameters
     *
     * @param matching
     * Dictionary with matching key/values. Supported keys (see IOHIDEventServiceKeys_Private.h):
     *   kIOHIDEventTypeKey
     *   kIOHIDUsagePageKey
     *   kIOHIDUsageKey
     *
     * @param event
     * Event copy. Caller must release event.
     *
     * @return
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t handleCopyMatchingEvent(OSDictionary * matching, IOHIDEvent **event) LOCAL;

    /*!
     * @function SetProperties
     *
     * @abstract
     * set properties on the event service
     *
     * @discussion
     * This method should be overridden by the DriverKit class if it needs to respond to setProperties calls.
     * Calling SetProperties(properties, SUPERDISPATCH) from the DriverKit class will get the properties added to a dictionary in IOReg
     * that HID Event System Clients will be able to see.
     *
     * @param properties
     * Dictionary with property key/values. Supported keys (see IOHIDEventServiceKeys.h and IOHIDEventServiceKeys_Private.h):
     *   kIOHIDEventTypeKey
     *   kIOHIDUsagePageKey
     *   kIOHIDUsageKey
     *
     * @return
     * Returns kIOReturnSuccess on success.
     */
    virtual kern_return_t SetProperties(OSDictionary *properties) override;
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOHIDEventService IOHIDEventService.iig:49-381 */

#define IOHIDEventService__CompleteSetLED_ID            0x89830a8035210708ULL
#define IOHIDEventService_SetLEDAction_ID            0xa095dc2c466d441dULL
#define IOHIDEventService__SetLED_ID            0xecb4f77e69022d2dULL
#define IOHIDEventService__CompleteSetProperties_ID            0xe1490a1f685db486ULL
#define IOHIDEventService__SetUserProperties_ID            0xefd8a550b2092d2dULL
#define IOHIDEventService_SetUserProperties_ID            0xe86319b882a5ed35ULL
#define IOHIDEventService__CompleteCopyEvent_ID            0xfe4c1f0658fc9a16ULL
#define IOHIDEventService__CopyEvent_ID            0xc3450c2606d5f0e6ULL
#define IOHIDEventService_CopyEvent_ID            0xf17de3a9be12ffedULL
#define IOHIDEventService__Start_ID            0xb7aeaf00ae079615ULL
#define IOHIDEventService_EventAvailable_ID            0xb6d3212b1068148aULL
#define IOHIDEventService_SetEventMemory_ID            0xed62c58e8d2f64e7ULL
#define IOHIDEventService__DispatchRelativeScrollWheelEvent_ID            0xe5902a0b51ed22ffULL
#define IOHIDEventService__DispatchAbsolutePointerEvent_ID            0xa89e9153d54a8eb1ULL
#define IOHIDEventService__DispatchRelativePointerEvent_ID            0xdfb3b21fdb3ea3b4ULL
#define IOHIDEventService__DispatchKeyboardEvent_ID            0xd8dc8ce69e15f7d3ULL
#define IOHIDEventService_SetLED_ID            0xfee475ac1384bab8ULL
#define IOHIDEventService_SetLEDState_ID            0xa871aa31861269baULL
#define IOHIDEventService_handleCopyMatchingEvent_ID            0xc7c97c024faede2dULL

#define IOHIDEventService__CompleteSetLED_Args \
        OSAction * action, \
        IOReturn result, \
        uint64_t context

#define IOHIDEventService_SetLEDAction_Args \
        uint32_t usagePage, \
        uint32_t usage, \
        bool on, \
        uint64_t context, \
        OSAction * action

#define IOHIDEventService__SetLED_Args \
        uint32_t usagePage, \
        uint32_t usage, \
        bool on, \
        uint64_t context, \
        OSAction * action

#define IOHIDEventService__CompleteSetProperties_Args \
        OSAction * action, \
        IOReturn result, \
        uint64_t context

#define IOHIDEventService__SetUserProperties_Args \
        OSDictionary * properties, \
        uint64_t context, \
        OSAction * action

#define IOHIDEventService_SetUserProperties_Args \
        OSDictionary * properties, \
        uint64_t context, \
        OSAction * action

#define IOHIDEventService__CompleteCopyEvent_Args \
        OSAction * action, \
        IOBufferMemoryDescriptor * eventBuffer, \
        uint64_t context

#define IOHIDEventService__CopyEvent_Args \
        OSDictionary * matching, \
        uint64_t context, \
        OSAction * action

#define IOHIDEventService_CopyEvent_Args \
        OSDictionary * matching, \
        uint64_t context, \
        OSAction * action

#define IOHIDEventService__Start_Args \
        IOService * provider

#define IOHIDEventService_EventAvailable_Args \
        uint32_t length

#define IOHIDEventService_SetEventMemory_Args \
        IOBufferMemoryDescriptor * memory

#define IOHIDEventService__DispatchRelativeScrollWheelEvent_Args \
        uint64_t timeStamp, \
        IOFixed dx, \
        IOFixed dy, \
        IOFixed dz, \
        IOOptionBits options, \
        bool accelerate

#define IOHIDEventService__DispatchAbsolutePointerEvent_Args \
        uint64_t timeStamp, \
        IOFixed x, \
        IOFixed y, \
        uint32_t buttonState, \
        IOOptionBits options, \
        bool accelerate

#define IOHIDEventService__DispatchRelativePointerEvent_Args \
        uint64_t timeStamp, \
        IOFixed dx, \
        IOFixed dy, \
        uint32_t buttonState, \
        IOOptionBits options, \
        bool accelerate

#define IOHIDEventService__DispatchKeyboardEvent_Args \
        uint64_t timeStamp, \
        uint32_t usagePage, \
        uint32_t usage, \
        uint32_t value, \
        IOOptionBits options, \
        bool repeat

#define IOHIDEventService_Start_Args \
        IOService * provider

#define IOHIDEventService_Stop_Args \
        IOService * provider

#define IOHIDEventService_SetLED_Args \
        uint32_t usage, \
        bool on

#define IOHIDEventService_SetLEDState_Args \
        uint32_t usagePage, \
        uint32_t usage, \
        bool on

#define IOHIDEventService_handleCopyMatchingEvent_Args \
        OSDictionary * matching, \
        IOHIDEvent ** event

#define IOHIDEventService_SetProperties_Args \
        OSDictionary * properties

#define IOHIDEventService_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOHIDEventService * self, const IORPC rpc);\
\
    void\
    _CompleteSetLED(\
        OSAction * action,\
        IOReturn result,\
        uint64_t context,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    SetLEDAction(\
        uint32_t usagePage,\
        uint32_t usage,\
        bool on,\
        uint64_t context,\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateAction_SetLED(size_t referenceSize, OSAction ** action);\
\
    void\
    _CompleteSetProperties(\
        OSAction * action,\
        IOReturn result,\
        uint64_t context,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateAction_SetUserProperties(size_t referenceSize, OSAction ** action);\
\
    void\
    SetUserProperties(\
        OSDictionary * properties,\
        uint64_t context,\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    _CompleteCopyEvent(\
        OSAction * action,\
        IOBufferMemoryDescriptor * eventBuffer,\
        uint64_t context,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateAction_CopyEvent(size_t referenceSize, OSAction ** action);\
\
    void\
    CopyEvent(\
        OSDictionary * matching,\
        uint64_t context,\
        OSAction * action,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _Start(\
        IOService * provider,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    EventAvailable(\
        uint32_t length,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetEventMemory(\
        IOBufferMemoryDescriptor * memory,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _DispatchRelativeScrollWheelEvent(\
        uint64_t timeStamp,\
        IOFixed dx,\
        IOFixed dy,\
        IOFixed dz,\
        IOOptionBits options,\
        bool accelerate = true,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _DispatchAbsolutePointerEvent(\
        uint64_t timeStamp,\
        IOFixed x,\
        IOFixed y,\
        uint32_t buttonState,\
        IOOptionBits options,\
        bool accelerate = true,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _DispatchRelativePointerEvent(\
        uint64_t timeStamp,\
        IOFixed dx,\
        IOFixed dy,\
        uint32_t buttonState,\
        IOOptionBits options,\
        bool accelerate = true,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _DispatchKeyboardEvent(\
        uint64_t timeStamp,\
        uint32_t usagePage,\
        uint32_t usage,\
        uint32_t value,\
        IOOptionBits options,\
        bool repeat = true,\
        OSDispatchMethod supermethod = NULL);\
\
    void\
    SetLED(\
        uint32_t usage,\
        bool on,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    SetLEDState(\
        uint32_t usagePage,\
        uint32_t usage,\
        bool on,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    handleCopyMatchingEvent(\
        OSDictionary * matching,\
        IOHIDEvent ** event,\
        OSDispatchMethod supermethod = NULL);\
\
\
protected:\
    /* _Impl methods */\
\
    void\
    _SetLED_Impl(IOHIDEventService__SetLED_Args);\
\
    void\
    _SetUserProperties_Impl(IOHIDEventService__SetUserProperties_Args);\
\
    void\
    _CopyEvent_Impl(IOHIDEventService__CopyEvent_Args);\
\
    kern_return_t\
    Start_Impl(IOService_Start_Args);\
\
    kern_return_t\
    Stop_Impl(IOService_Stop_Args);\
\
    void\
    SetLED_Impl(IOHIDEventService_SetLED_Args);\
\
    kern_return_t\
    SetLEDState_Impl(IOHIDEventService_SetLEDState_Args);\
\
    kern_return_t\
    handleCopyMatchingEvent_Impl(IOHIDEventService_handleCopyMatchingEvent_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef void (*_CompleteSetLED_Handler)(OSMetaClassBase * target, IOHIDEventService__CompleteSetLED_Args);\
    static kern_return_t\
    _CompleteSetLED_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _CompleteSetLED_Handler func);\
\
    typedef void (*SetLEDAction_Handler)(OSMetaClassBase * target, IOHIDEventService_SetLEDAction_Args);\
    static kern_return_t\
    SetLEDAction_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetLEDAction_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    SetLEDAction_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetLEDAction_Handler func);\
\
    typedef void (*_CompleteSetProperties_Handler)(OSMetaClassBase * target, IOHIDEventService__CompleteSetProperties_Args);\
    static kern_return_t\
    _CompleteSetProperties_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _CompleteSetProperties_Handler func);\
\
    typedef void (*SetUserProperties_Handler)(OSMetaClassBase * target, IOHIDEventService_SetUserProperties_Args);\
    static kern_return_t\
    SetUserProperties_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetUserProperties_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    SetUserProperties_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetUserProperties_Handler func);\
\
    typedef void (*_CompleteCopyEvent_Handler)(OSMetaClassBase * target, IOHIDEventService__CompleteCopyEvent_Args);\
    static kern_return_t\
    _CompleteCopyEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _CompleteCopyEvent_Handler func);\
\
    typedef void (*CopyEvent_Handler)(OSMetaClassBase * target, IOHIDEventService_CopyEvent_Args);\
    static kern_return_t\
    CopyEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyEvent_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    CopyEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyEvent_Handler func);\
\
    typedef kern_return_t (*_Start_Handler)(OSMetaClassBase * target, IOHIDEventService__Start_Args);\
    static kern_return_t\
    _Start_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _Start_Handler func);\
\
    typedef kern_return_t (*EventAvailable_Handler)(OSMetaClassBase * target, IOHIDEventService_EventAvailable_Args);\
    static kern_return_t\
    EventAvailable_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        EventAvailable_Handler func);\
\
    typedef kern_return_t (*SetEventMemory_Handler)(OSMetaClassBase * target, IOHIDEventService_SetEventMemory_Args);\
    static kern_return_t\
    SetEventMemory_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetEventMemory_Handler func);\
\
    typedef kern_return_t (*_DispatchRelativeScrollWheelEvent_Handler)(OSMetaClassBase * target, IOHIDEventService__DispatchRelativeScrollWheelEvent_Args);\
    static kern_return_t\
    _DispatchRelativeScrollWheelEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _DispatchRelativeScrollWheelEvent_Handler func);\
\
    typedef kern_return_t (*_DispatchAbsolutePointerEvent_Handler)(OSMetaClassBase * target, IOHIDEventService__DispatchAbsolutePointerEvent_Args);\
    static kern_return_t\
    _DispatchAbsolutePointerEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _DispatchAbsolutePointerEvent_Handler func);\
\
    typedef kern_return_t (*_DispatchRelativePointerEvent_Handler)(OSMetaClassBase * target, IOHIDEventService__DispatchRelativePointerEvent_Args);\
    static kern_return_t\
    _DispatchRelativePointerEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _DispatchRelativePointerEvent_Handler func);\
\
    typedef kern_return_t (*_DispatchKeyboardEvent_Handler)(OSMetaClassBase * target, IOHIDEventService__DispatchKeyboardEvent_Args);\
    static kern_return_t\
    _DispatchKeyboardEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _DispatchKeyboardEvent_Handler func);\
\
    typedef void (*SetLED_Handler)(OSMetaClassBase * target, IOHIDEventService_SetLED_Args);\
    static kern_return_t\
    SetLED_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetLED_Handler func);\
\
    typedef kern_return_t (*SetLEDState_Handler)(OSMetaClassBase * target, IOHIDEventService_SetLEDState_Args);\
    static kern_return_t\
    SetLEDState_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        SetLEDState_Handler func);\
\
    typedef kern_return_t (*handleCopyMatchingEvent_Handler)(OSMetaClassBase * target, IOHIDEventService_handleCopyMatchingEvent_Args);\
    static kern_return_t\
    handleCopyMatchingEvent_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        handleCopyMatchingEvent_Handler func);\
\


#define IOHIDEventService_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    void\
    _CompleteSetLED_Impl(IOHIDEventService__CompleteSetLED_Args);\
\
    void\
    _CompleteSetProperties_Impl(IOHIDEventService__CompleteSetProperties_Args);\
\
    void\
    _CompleteCopyEvent_Impl(IOHIDEventService__CompleteCopyEvent_Args);\
\
    kern_return_t\
    _Start_Impl(IOHIDEventService__Start_Args);\
\
    kern_return_t\
    EventAvailable_Impl(IOHIDEventService_EventAvailable_Args);\
\
    kern_return_t\
    SetEventMemory_Impl(IOHIDEventService_SetEventMemory_Args);\
\
    kern_return_t\
    _DispatchRelativeScrollWheelEvent_Impl(IOHIDEventService__DispatchRelativeScrollWheelEvent_Args);\
\
    kern_return_t\
    _DispatchAbsolutePointerEvent_Impl(IOHIDEventService__DispatchAbsolutePointerEvent_Args);\
\
    kern_return_t\
    _DispatchRelativePointerEvent_Impl(IOHIDEventService__DispatchRelativePointerEvent_Args);\
\
    kern_return_t\
    _DispatchKeyboardEvent_Impl(IOHIDEventService__DispatchKeyboardEvent_Args);\
\
    kern_return_t\
    SetProperties_Impl(IOService_SetProperties_Args);\
\


#define IOHIDEventService_VirtualMethods \
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
    virtual kern_return_t\
    dispatchKeyboardEvent(\
        uint64_t timeStamp,\
        uint32_t usagePage,\
        uint32_t usage,\
        uint32_t value,\
        IOOptionBits options,\
        bool repeat = true) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    dispatchRelativePointerEvent(\
        uint64_t timeStamp,\
        IOFixed dx,\
        IOFixed dy,\
        uint32_t buttonState,\
        IOOptionBits options,\
        bool accelerate = true) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    dispatchAbsolutePointerEvent(\
        uint64_t timeStamp,\
        IOFixed x,\
        IOFixed y,\
        uint32_t buttonState,\
        IOOptionBits options,\
        bool accelerate = true) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    dispatchRelativeScrollWheelEvent(\
        uint64_t timeStamp,\
        IOFixed dx,\
        IOFixed dy,\
        IOFixed dz,\
        IOOptionBits options,\
        bool accelerate = true) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    dispatchDigitizerStylusEvent(\
        uint64_t timeStamp,\
        IOHIDDigitizerStylusData * stylusData) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    dispatchDigitizerTouchEvent(\
        uint64_t timeStamp,\
        IOHIDDigitizerTouchData * touchData,\
        uint32_t touchDataCount) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    dispatchEvent(\
        IOHIDEvent * event) APPLE_KEXT_OVERRIDE;\
\


#if !KERNEL

extern OSMetaClass          * gIOHIDEventServiceMetaClass;
extern const OSClassLoadInformation IOHIDEventService_Class;

class IOHIDEventServiceMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class IOHIDEventServiceInterface : public OSInterface
{
public:
    virtual kern_return_t
    dispatchKeyboardEvent(uint64_t timeStamp,
        uint32_t usagePage,
        uint32_t usage,
        uint32_t value,
        IOOptionBits options,
        bool repeat) = 0;

    virtual kern_return_t
    dispatchRelativePointerEvent(uint64_t timeStamp,
        IOFixed dx,
        IOFixed dy,
        uint32_t buttonState,
        IOOptionBits options,
        bool accelerate) = 0;

    virtual kern_return_t
    dispatchAbsolutePointerEvent(uint64_t timeStamp,
        IOFixed x,
        IOFixed y,
        uint32_t buttonState,
        IOOptionBits options,
        bool accelerate) = 0;

    virtual kern_return_t
    dispatchRelativeScrollWheelEvent(uint64_t timeStamp,
        IOFixed dx,
        IOFixed dy,
        IOFixed dz,
        IOOptionBits options,
        bool accelerate) = 0;

    virtual kern_return_t
    dispatchDigitizerStylusEvent(uint64_t timeStamp,
        IOHIDDigitizerStylusData * stylusData) = 0;

    virtual kern_return_t
    dispatchDigitizerTouchEvent(uint64_t timeStamp,
        IOHIDDigitizerTouchData * touchData,
        uint32_t touchDataCount) = 0;

    virtual void
    dispatchEvent(IOHIDEvent * event) = 0;

    kern_return_t
    dispatchKeyboardEvent_Call(uint64_t timeStamp,
        uint32_t usagePage,
        uint32_t usage,
        uint32_t value,
        IOOptionBits options,
        bool repeat)  { return dispatchKeyboardEvent(timeStamp, usagePage, usage, value, options, repeat); };\

    kern_return_t
    dispatchRelativePointerEvent_Call(uint64_t timeStamp,
        IOFixed dx,
        IOFixed dy,
        uint32_t buttonState,
        IOOptionBits options,
        bool accelerate)  { return dispatchRelativePointerEvent(timeStamp, dx, dy, buttonState, options, accelerate); };\

    kern_return_t
    dispatchAbsolutePointerEvent_Call(uint64_t timeStamp,
        IOFixed x,
        IOFixed y,
        uint32_t buttonState,
        IOOptionBits options,
        bool accelerate)  { return dispatchAbsolutePointerEvent(timeStamp, x, y, buttonState, options, accelerate); };\

    kern_return_t
    dispatchRelativeScrollWheelEvent_Call(uint64_t timeStamp,
        IOFixed dx,
        IOFixed dy,
        IOFixed dz,
        IOOptionBits options,
        bool accelerate)  { return dispatchRelativeScrollWheelEvent(timeStamp, dx, dy, dz, options, accelerate); };\

    kern_return_t
    dispatchDigitizerStylusEvent_Call(uint64_t timeStamp,
        IOHIDDigitizerStylusData * stylusData)  { return dispatchDigitizerStylusEvent(timeStamp, stylusData); };\

    kern_return_t
    dispatchDigitizerTouchEvent_Call(uint64_t timeStamp,
        IOHIDDigitizerTouchData * touchData,
        uint32_t touchDataCount)  { return dispatchDigitizerTouchEvent(timeStamp, touchData, touchDataCount); };\

    void
    dispatchEvent_Call(IOHIDEvent * event)  { return dispatchEvent(event); };\

};

struct IOHIDEventService_IVars;
struct IOHIDEventService_LocalIVars;

class IOHIDEventService : public IOService, public IOHIDEventServiceInterface
{
#if !KERNEL
    friend class IOHIDEventServiceMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
#ifdef IOHIDEventService_DECLARE_IVARS
IOHIDEventService_DECLARE_IVARS
#else /* IOHIDEventService_DECLARE_IVARS */
    union
    {
        IOHIDEventService_IVars * ivars;
        IOHIDEventService_LocalIVars * lvars;
    };
#endif /* IOHIDEventService_DECLARE_IVARS */
#endif /* !KERNEL */

#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gIOHIDEventServiceMetaClass; };
#endif /* KERNEL */

    using super = IOService;

#if !KERNEL
    IOHIDEventService_Methods
    IOHIDEventService_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#define OSAction_IOHIDEventService__SetLED_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(OSAction_IOHIDEventService__SetLED * self, const IORPC rpc);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\


#define OSAction_IOHIDEventService__SetLED_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define OSAction_IOHIDEventService__SetLED_VirtualMethods \
\
public:\
\


#if !KERNEL

extern OSMetaClass          * gOSAction_IOHIDEventService__SetLEDMetaClass;
extern const OSClassLoadInformation OSAction_IOHIDEventService__SetLED_Class;

class OSAction_IOHIDEventService__SetLEDMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

class OSAction_IOHIDEventService__SetLEDInterface : public OSInterface
{
public:
};

struct OSAction_IOHIDEventService__SetLED_IVars;
struct OSAction_IOHIDEventService__SetLED_LocalIVars;

class __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer"))) OSAction_IOHIDEventService__SetLED : public OSAction, public OSAction_IOHIDEventService__SetLEDInterface
{
#if KERNEL
    OSDeclareDefaultStructorsWithDispatch(OSAction_IOHIDEventService__SetLED);
#endif /* KERNEL */

#if !KERNEL
    friend class OSAction_IOHIDEventService__SetLEDMetaClass;
#endif /* !KERNEL */

public:
#ifdef OSAction_IOHIDEventService__SetLED_DECLARE_IVARS
OSAction_IOHIDEventService__SetLED_DECLARE_IVARS
#else /* OSAction_IOHIDEventService__SetLED_DECLARE_IVARS */
    union
    {
        OSAction_IOHIDEventService__SetLED_IVars * ivars;
        OSAction_IOHIDEventService__SetLED_LocalIVars * lvars;
    };
#endif /* OSAction_IOHIDEventService__SetLED_DECLARE_IVARS */
#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gOSAction_IOHIDEventService__SetLEDMetaClass; };
    virtual const OSMetaClass *
    getMetaClass() const APPLE_KEXT_OVERRIDE { return gOSAction_IOHIDEventService__SetLEDMetaClass; };
#endif /* KERNEL */

    using super = OSAction;

#if !KERNEL
    OSAction_IOHIDEventService__SetLED_Methods
#endif /* !KERNEL */

    OSAction_IOHIDEventService__SetLED_VirtualMethods
};

#define OSAction_IOHIDEventService__SetUserProperties_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(OSAction_IOHIDEventService__SetUserProperties * self, const IORPC rpc);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\


#define OSAction_IOHIDEventService__SetUserProperties_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define OSAction_IOHIDEventService__SetUserProperties_VirtualMethods \
\
public:\
\


#if !KERNEL

extern OSMetaClass          * gOSAction_IOHIDEventService__SetUserPropertiesMetaClass;
extern const OSClassLoadInformation OSAction_IOHIDEventService__SetUserProperties_Class;

class OSAction_IOHIDEventService__SetUserPropertiesMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

class OSAction_IOHIDEventService__SetUserPropertiesInterface : public OSInterface
{
public:
};

struct OSAction_IOHIDEventService__SetUserProperties_IVars;
struct OSAction_IOHIDEventService__SetUserProperties_LocalIVars;

class __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer"))) OSAction_IOHIDEventService__SetUserProperties : public OSAction, public OSAction_IOHIDEventService__SetUserPropertiesInterface
{
#if KERNEL
    OSDeclareDefaultStructorsWithDispatch(OSAction_IOHIDEventService__SetUserProperties);
#endif /* KERNEL */

#if !KERNEL
    friend class OSAction_IOHIDEventService__SetUserPropertiesMetaClass;
#endif /* !KERNEL */

public:
#ifdef OSAction_IOHIDEventService__SetUserProperties_DECLARE_IVARS
OSAction_IOHIDEventService__SetUserProperties_DECLARE_IVARS
#else /* OSAction_IOHIDEventService__SetUserProperties_DECLARE_IVARS */
    union
    {
        OSAction_IOHIDEventService__SetUserProperties_IVars * ivars;
        OSAction_IOHIDEventService__SetUserProperties_LocalIVars * lvars;
    };
#endif /* OSAction_IOHIDEventService__SetUserProperties_DECLARE_IVARS */
#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gOSAction_IOHIDEventService__SetUserPropertiesMetaClass; };
    virtual const OSMetaClass *
    getMetaClass() const APPLE_KEXT_OVERRIDE { return gOSAction_IOHIDEventService__SetUserPropertiesMetaClass; };
#endif /* KERNEL */

    using super = OSAction;

#if !KERNEL
    OSAction_IOHIDEventService__SetUserProperties_Methods
#endif /* !KERNEL */

    OSAction_IOHIDEventService__SetUserProperties_VirtualMethods
};

#define OSAction_IOHIDEventService__CopyEvent_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(OSAction_IOHIDEventService__CopyEvent * self, const IORPC rpc);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\


#define OSAction_IOHIDEventService__CopyEvent_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define OSAction_IOHIDEventService__CopyEvent_VirtualMethods \
\
public:\
\


#if !KERNEL

extern OSMetaClass          * gOSAction_IOHIDEventService__CopyEventMetaClass;
extern const OSClassLoadInformation OSAction_IOHIDEventService__CopyEvent_Class;

class OSAction_IOHIDEventService__CopyEventMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

class OSAction_IOHIDEventService__CopyEventInterface : public OSInterface
{
public:
};

struct OSAction_IOHIDEventService__CopyEvent_IVars;
struct OSAction_IOHIDEventService__CopyEvent_LocalIVars;

class __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer"))) OSAction_IOHIDEventService__CopyEvent : public OSAction, public OSAction_IOHIDEventService__CopyEventInterface
{
#if KERNEL
    OSDeclareDefaultStructorsWithDispatch(OSAction_IOHIDEventService__CopyEvent);
#endif /* KERNEL */

#if !KERNEL
    friend class OSAction_IOHIDEventService__CopyEventMetaClass;
#endif /* !KERNEL */

public:
#ifdef OSAction_IOHIDEventService__CopyEvent_DECLARE_IVARS
OSAction_IOHIDEventService__CopyEvent_DECLARE_IVARS
#else /* OSAction_IOHIDEventService__CopyEvent_DECLARE_IVARS */
    union
    {
        OSAction_IOHIDEventService__CopyEvent_IVars * ivars;
        OSAction_IOHIDEventService__CopyEvent_LocalIVars * lvars;
    };
#endif /* OSAction_IOHIDEventService__CopyEvent_DECLARE_IVARS */
#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gOSAction_IOHIDEventService__CopyEventMetaClass; };
    virtual const OSMetaClass *
    getMetaClass() const APPLE_KEXT_OVERRIDE { return gOSAction_IOHIDEventService__CopyEventMetaClass; };
#endif /* KERNEL */

    using super = OSAction;

#if !KERNEL
    OSAction_IOHIDEventService__CopyEvent_Methods
#endif /* !KERNEL */

    OSAction_IOHIDEventService__CopyEvent_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */


/* IOHIDEventService.iig:630- */

#endif /* ! _HIDDRIVERKIT_IOHIDEVENTSERVICE_H */
