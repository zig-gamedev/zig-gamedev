/*
    File:       IOVideoDeviceLib.h
    
    Contains:   IOCFPlugin library for using IOVideoDevice objects.
                The IOVideoDevice plugin provides a convenient set of functions for accessing and manipulating IOVideoDevice objects from user programs.

    Copyright:  © 2006-2012 by Apple Inc., all rights reserved.
*/

#if !defined(__IOKIT_IOVIDEODEVICELIB_H)
#define __IOKIT_IOVIDEODEVICELIB_H

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/stream/IOStreamLib.h>
#include <IOKit/stream/IOStreamShared.h>
#include <IOKit/video/IOVideoDeviceShared.h>
#include <IOKit/video/IOVideoTypes.h>

__BEGIN_DECLS    
    
#pragma mark IOVideo UUIDs

/*!
    @defined        kIOVideoDeviceLibTypeID
    @discussion     This is the UUID of the plug-in type (5339633C-F903-4212-9C90-9B18AF01862D).
*/
#define kIOVideoDeviceLibTypeID CFUUIDGetConstantUUIDWithBytes(kCFAllocatorDefault, 0x53, 0x39, 0x63, 0x3C, 0xF9, 0x03, 0x42, 0x12, 0x9C, 0x90, 0x9B, 0x18, 0xAF, 0x01, 0x86, 0x2D)
    
/*!
    @defined        kIOVideoDeviceInterfaceID_v1
    @discussion     This is the UUID of version 1 of the plug-in interface (080E3-5106-4D16-B70C-B3216F13CDB9A).
*/
#define kIOVideoDeviceInterfaceID_v1 CFUUIDGetConstantUUIDWithBytes(kCFAllocatorDefault, 0x0D, 0xE0, 0x80, 0xE3, 0x51, 0x06, 0x4D, 0x16, 0xB7, 0x0C, 0xB3, 0x21, 0x6F, 0x13, 0xCD, 0xB9)
#define kIOVideoDeviceInterfaceID kIOVideoDeviceInterfaceID_v1

typedef struct IOVideoDeviceInterface_v1_t IOVideoDeviceInterface_v1_t;

/*!
    @typedef    IOVideoDeviceRef
*/
typedef IOVideoDeviceInterface_v1_t** IOVideoDeviceRef;

/*!
    @typedef    IOVideoDeviceOutputCallback
*/
typedef void (*IOVideoDeviceOutputCallback)(    IOVideoDeviceRef    stream,
                                                void*               context);

/*!
    @typedef    IOVideoDeviceNotificationCallback
*/
typedef void (*IOVideoDeviceNotificationCallback)(  IOVideoDeviceRef    device,
                                                    void*               context,
                                                    void*               message);

/*!
    @interface IOVideoDeviceInterface
    @abstract Interface for accessing kernel-space video drivers from user space.
 */
typedef struct IOVideoDeviceInterface_v1_t
{
    IUNKNOWN_C_GUTS;
    UInt32 Version;
    UInt32 Revision;

    /*!
        @functiongroup  Opening and closing VideoDevices
    */
    
    /*!
        @function       Open
        @abstract       Open an IOVideoDevice from user space.
        @discussion     The Open function opens the device.
        @param          device
                            The IOVideoDeviceRef to the device returned by QueryInterface.
        @param          options
                            Open options.  Currently unused.
        @result         If the device could not be opened an error will be returned.
    */
    IOReturn
    (*Open)(IOVideoDeviceRef device, IOOptionBits options);

    /*!
        @function       Close
        @abstract       Closes an IOVideoDevice.
        @discussion     Calling Close frees all streams belonging to the device and frees all user resources used by the device.
        @param          device
                            The IOVideoDeviceRef of the device to close.
        @result         Returns kIOReturnSuccess if the device was successfully closed.
    */
    IOReturn
    (*Close)(IOVideoDeviceRef device);

    /*!
        @function       GetNotificationPort
        @abstract       Get the notification port for device state changes sent to user space.
        @param          device
                            The IOVideoDeviceRef of the stream to operate on.
        @result         A CFMachPortRef of the output notification port.
    */
    CFMachPortRef
    (*GetNotificationPort)(IOVideoDeviceRef device);

    /*!
        @function       SetNotificationCallback
        @abstract       Set the callback function to be called when certain device state changes happen.
        @param          device
                            The IOVideoDeviceRef of the device to operate on. Pass NULL to remove the callback.
        @result         Returns kIOReturnSuccess if the callback was successfully set or removed.
    */
    IOReturn
    (*SetNotificationCallback)( IOVideoDeviceRef                    device,
                                IOVideoDeviceNotificationCallback   callback,
                                void*                               context);

    /*!
        @function       SetControlValue
        @param          device
        @param          controlID
        @param          newValue
        @result         Returns kIOReturnSuccess if the call was successfully.
    */
    IOReturn
    (*SetControlValue)( IOVideoDeviceRef    device,
                        UInt32              controlID,
                        UInt32              value,
                        UInt32*             newValue);
 
    /*!
        @function       SetStreamFormat
        @param          device
        @param          streamID
        @param          streamFormat
        @result         Returns kIOReturnSuccess if the call was successfully.
    */
    IOReturn
    (*SetStreamFormat)( IOVideoDeviceRef            device,
                        UInt32                      streamID,
                        IOVideoStreamDescription*   streamFormat);
    
    /*!
        @function       GetRunLoopSource
        @abstract       Gets a CFRunLoopSource for the CFMachPort used for notifications from the kernel that data is ready.
        @param          device
                            The IOVideoDeviceRef of the stream to operate on.
        @result         The CFRunLoopSourceRef for the run loop source, or NULL if there was an error creating the source.
    */
    CFRunLoopSourceRef
    (*GetRunLoopSource)(IOVideoDeviceRef device);
    
    /*!
        @function       AddToRunLoop
        @abstract       Add the CFRunLoopSource for the notification port to a run loop.
        @param          device
                            The IOVideoDeviceRef of the device to operate on.
        @param          runLoop
                            The run loop to which to add the notification source.
        @result         Returns kIOReturnSuccess if the source was successfully added to the run loop.
    */
    
    IOReturn
    (*AddToRunLoop)(    IOVideoDeviceRef    device,
                        CFRunLoopRef        runLoop);
    
    /*!
        @function       RemoveFromRunLoop
        @abstract       Remove the CFRunLoopSource for the notification port from a run loop.
        @param          device
                            The IOVideoDeviceRef of the device to operate on.
        @param          runLoop
                            The run loop from which to remove the notification source.
        @result         Returns kIOReturnSuccess if the source was successfully removed from the run loop.
    */
    IOReturn
    (*RemoveFromRunLoop)(   IOVideoDeviceRef    device,
                            CFRunLoopRef        runLoop);

    /*!
        @function       CreateStreamInterface
        @param          device
                            The IOVideoDeviceRef of the device to operate on.
        @param          streamDictionary
        @param          streamIndex
        @param          isInput
        @result         Returns kIOReturnSuccess if the stream was successfully created.
    */
    IOReturn
    (*CreateStreamInterface)(   IOVideoDeviceRef    device,
                                CFDictionaryRef     streamDictionary,
                                UInt32              streamIndex,
                                bool                isInput,
                                IOStreamRef*        streamRef);

    /*!
        @function       ReleaseStreamInterface
        @param          device
                            The IOVideoDeviceRef of the device to operate on.
        @param          isInput
        @param          streamRef
        @result         Returns kIOReturnSuccess if the stream was successfully released.
    */
    IOReturn
    (*ReleaseStreamInterface)(  IOVideoDeviceRef    device,
                                bool                isInput,
                                IOStreamRef*        streamRef);
} IOVideoDeviceInterface;


__END_DECLS

#endif
