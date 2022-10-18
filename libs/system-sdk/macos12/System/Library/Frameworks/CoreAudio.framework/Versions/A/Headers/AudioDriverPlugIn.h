/*==================================================================================================
     File:       CoreAudio/AudioDriverPlugIn.h

     Contains:   API for the CFBundle an IOAudio driver can specify for the HAL to use
                 to provide implemenation for device specific properties.

     Copyright:  (c) 1985-2010 by Apple, Inc., all rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

==================================================================================================*/
#if !defined(CoreAudio_AudioDriverPlugIn_h)
#define CoreAudio_AudioDriverPlugIn_h

//==================================================================================================
#pragma mark    Overview

/*!
    @header AudioDriverPlugIn
    IOAudio family drivers can specify a CFBundle in order to implement device specific properties
    on behalf of the HAL. The CFBundle provides routines for opening and closing the device as well
    as the property management routines. The mechanism by which the driver specifies which CFBundle
    to load is defined by the IOAudio driver family in IOKit. The routines described in this header
    are loaded by name from the CFBundle. Note that the HAL will create an instance of a plug-in for
    each device the plug-in supports that is attached to the system in each process that is using
    the HAL.
*/

//==================================================================================================
#pragma mark    Includes

#include <CoreAudio/AudioHardware.h>
#include <IOKit/IOKitLib.h>

//==================================================================================================

CF_ASSUME_NONNULL_BEGIN

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
#pragma mark    Types

/*!
    @typedef        AudioDriverPlugInDevicePropertyChangedProc
    @abstract       A plug-in calls this routine to notify the HAL that one of its device's
                    properties has changed.
    @discussion     The HAL will notify any listeners on behalf of the plug-in. The pointer to this
                    routine is supplied to the plug-in when it is opened.
    @param          inDevice
                        The device whose property has changed.
    @param          inChannel
                        The channel of the device the property that has changed belongs to.
    @param          isInput
                        The section of the device the property that has changed belongs to.
    @param          inPropertyID
                        The selector of the property that has changed.
    @result         The return value is currently unused and should always be 0.
*/
typedef OSStatus
(*AudioDriverPlugInDevicePropertyChangedProc)(  AudioDeviceID           inDevice,
                                                UInt32                  inChannel,
                                                Boolean                 isInput,
                                                AudioDevicePropertyID   inPropertyID);

/*!
    @typedef        AudioDriverPlugInStreamPropertyChangedProc
    @abstract       A plug-in calls this routine to notify the HAL that a property of one of the
                    streams of one of its devices has changed.
    @discussion     The HAL will notify any listeners on behalf of the plug-in. The pointer to this
                    routine is supplied to the plug-in when it is opened.
    @param          inDevice
                        The device whose property has changed.
    @param          inIOAudioStream
                        The stream whose property has changed.
    @param          inChannel
                        The channel of the stream the property that has changed belongs to.
    @param          inPropertyID
                        The selector of the property that has changed.
    @result         The return value is currently unused and should always be 0.
*/
typedef OSStatus
(*AudioDriverPlugInStreamPropertyChangedProc)(  AudioDeviceID           inDevice,
                                                io_object_t             inIOAudioStream,
                                                UInt32                  inChannel,
                                                AudioDevicePropertyID   inPropertyID);

/*!
    @struct         AudioDriverPlugInHostInfo
    @abstract       This structure provides the plug-in with all the info it needs with respect to
                    communicating with the HAL.
    @field          mDeviceID
                        The AudioDeviceID of the audio device.
    @field          mIOAudioDevice
                        The io_object_t that points to the IOAudioDevice object for the device in
                        the driver.
    @field          mIOAudioEngine
                        The io_object_t that points to the IOAudioEngine object for the device in
                        the driver.
    @field          mDevicePropertyChangedProc
                        The AudioDriverPlugInDevicePropertyChangedProc the plug-in should call when
                        a device property changes.
    @field          mStreamPropertyChangedProc
                        The AudioDriverPlugInStreamPropertyChangedProc the plug-in should call when
                        a stream property changes.
*/
struct AudioDriverPlugInHostInfo
{
    AudioDeviceID                               mDeviceID;
    io_object_t                                 mIOAudioDevice;
    io_object_t                                 mIOAudioEngine;
    AudioDriverPlugInDevicePropertyChangedProc  mDevicePropertyChangedProc;
    AudioDriverPlugInStreamPropertyChangedProc  mStreamPropertyChangedProc;
};
typedef struct AudioDriverPlugInHostInfo    AudioDriverPlugInHostInfo;

//==================================================================================================
#pragma mark    Functions

/*!
    @functiongroup  AudioDriver Plug-In
*/

/*!
    @function       AudioDriverPlugInOpen
    @abstract       Instantiates a new instance of the driver plug-in.
    @param          inHostInfo
                        An AudioDriverPlugInHostInfo struct that tells the plug-in what device it is
                        being instantiated for.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDriverPlugInOpen(AudioDriverPlugInHostInfo*    inHostInfo);

/*!
    @function       AudioDriverPlugInClose
    @abstract       Closes an instance of the driver plug-in.
    @param          inDevice
                        The AudioDeviceID that describes the device who's plug-in instance is being
                        closed.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDriverPlugInClose(AudioDeviceID inDevice);

/*!
    @function       AudioDriverPlugInDeviceGetPropertyInfo
    @abstract       Retrieve information about the given property of an AudioDevice.
    @param          inDevice
                        The AudioDevice to query.
    @param          inChannel
                        The channel of the property to query where 0 is the main channel.
    @param          isInput
                        Which section of the AudioDevice to query.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to query.
    @param          outSize
                        A pointer to a UInt32 that receives the size of the property data in bytes
                        on exit. This can be NULL if the size information is not being requested.
    @param          outWritable
                        A pointer to a Boolean that receives indication of whether or not the given
                        property can be set. This can be NULL if the writability is not being
                        requested.
    @result         An OSStatus indicating success or failure. Note that if the plug-in does not
                    implement the given property, it should return
                    kAudioHardwareUnknownPropertyError to ensure that the HAL handles the property
                    normally.
*/
extern OSStatus
AudioDriverPlugInDeviceGetPropertyInfo( AudioDeviceID           inDevice,
                                        UInt32                  inChannel,
                                        Boolean                 isInput,
                                        AudioDevicePropertyID   inPropertyID,
                                        UInt32* __nullable      outSize,
                                        Boolean* __nullable     outWritable);

/*!
    @function       AudioDriverPlugInDeviceGetProperty
    @abstract       Queries an the AudioDevice object to get the data of the given property and
                    places it in the provided buffer.
    @param          inDevice
                        The AudioDevice to query.
    @param          inChannel
                        The channel of the property to query where 0 is the main channel.
    @param          isInput
                        Which section of the AudioDevice to query.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to query.
    @param          ioPropertyDataSize
                        A UInt32 which on entry indicates the size of the buffer pointed to by
                        outData and on exit indicates how much of the buffer was used.
    @param          outPropertyData
                        The buffer into which the object will put the data for the given property.
    @result         An OSStatus indicating success or failure. Note that if the plug-in does not
                    implement the given property, it should return
                    kAudioHardwareUnknownPropertyError to ensure that the HAL handles the property
                    normally.
*/
extern OSStatus
AudioDriverPlugInDeviceGetProperty( AudioDeviceID           inDevice,
                                    UInt32                  inChannel,
                                    Boolean                 isInput,
                                    AudioDevicePropertyID   inPropertyID,
                                    UInt32*                 ioPropertyDataSize,
                                    void*                   outPropertyData);

/*!
    @function       AudioDriverPlugInDeviceSetProperty
    @abstract       Tells the AudioDevice object to change the value of the given property using the
                    provided data.
    @discussion     Note that the value of the property should not be considered changed until the
                    HAL has called the listeners as many properties values are changed
                    asynchronously.
    @param          inDevice
                        The AudioDevice to change.
    @param          inWhen
                        A pointer to an AudioTimeStamp that says when to change the property's value
                        relative to the device's time base. NULL means execute the change
                        immediately.
    @param          inChannel
                        The channel of the property to change where 0 is the main channel.
    @param          isInput
                        Which section of the AudioDevice to change.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to change.
    @param          inPropertyDataSize
                        A UInt32 indicating the size of the buffer pointed to by inData.
    @param          inPropertyData
                        The buffer containing the data to be used to change the property's value.
    @result         An OSStatus indicating success or failure. Note that if the plug-in does not
                    implement the given property, it should return
                    kAudioHardwareUnknownPropertyError to ensure that the HAL handles the property
                    normally.
*/
extern OSStatus
AudioDriverPlugInDeviceSetProperty( AudioDeviceID                       inDevice,
                                    const AudioTimeStamp* __nullable    inWhen,
                                    UInt32                              inChannel,
                                    Boolean                             isInput,
                                    AudioDevicePropertyID               inPropertyID,
                                    UInt32                              inPropertyDataSize,
                                    const void*                         inPropertyData);

/*!
    @function       AudioDriverPlugInStreamGetPropertyInfo
    @abstract       Retrieve information about the given property of an AudioStream.
    @param          inDevice
                        The AudioDevice who's stream is being queried.
    @param          inIOAudioStream
                        The IOAudioStream to query.
    @param          inChannel
                        The channel of the property to query where 0 is the main channel.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to query.
    @param          outSize
                        A pointer to a UInt32 that receives the size of the property data in bytes
                        on exit. This can be NULL if the size information is not being requested.
    @param          outWritable
                        A pointer to a Boolean that receives indication of whether or not the given
                        property can be set. This can be NULL if the writability is not being
                        requested.
    @result         An OSStatus indicating success or failure. Note that if the plug-in does not
                    implement the given property, it should return
                    kAudioHardwareUnknownPropertyError to ensure that the HAL handles the property
                    normally.
*/
extern OSStatus
AudioDriverPlugInStreamGetPropertyInfo( AudioDeviceID           inDevice,
                                        io_object_t             inIOAudioStream,
                                        UInt32                  inChannel,
                                        AudioDevicePropertyID   inPropertyID,
                                        UInt32* __nullable      outSize,
                                        Boolean* __nullable     outWritable);

/*!
    @function       AudioDriverPlugInStreamGetProperty
    @abstract       Queries an the AudioStream object to get the data of the given property and
                    places it in the provided buffer.
    @param          inDevice
                        The AudioDevice who's stream is being queried.
    @param          inIOAudioStream
                        The IOAudioStream to query.
    @param          inChannel
                        The channel of the property to query where 0 is the main channel.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to query.
    @param          ioPropertyDataSize
                        A UInt32 which on entry indicates the size of the buffer pointed to by
                        outData and on exit indicates how much of the buffer was used.
    @param          outPropertyData
                        The buffer into which the object will put the data for the given property.
    @result         An OSStatus indicating success or failure. Note that if the plug-in does not
                    implement the given property, it should return
                    kAudioHardwareUnknownPropertyError to ensure that the HAL handles the property
                    normally.
*/
extern OSStatus
AudioDriverPlugInStreamGetProperty( AudioDeviceID           inDevice,
                                    io_object_t             inIOAudioStream,
                                    UInt32                  inChannel,
                                    AudioDevicePropertyID   inPropertyID,
                                    UInt32*                 ioPropertyDataSize,
                                    void*                   outPropertyData);

/*!
    @function       AudioDriverPlugInStreamSetProperty
    @abstract       Tells the AudioStream object to change the value of the given property using the
                    provided data.
    @discussion     Note that the value of the property should not be considered changed until the
                    HAL has called the listeners as many properties values are changed
                    asynchronously.
    @param          inDevice
                        The AudioDevice who's stream is being changed.
    @param          inIOAudioStream
                        The IOAudioStream to change.
    @param          inWhen
                        A pointer to an AudioTimeStamp that says when to change the property's value
                        relative to the device's time base. NULL means execute the change
                        immediately.
    @param          inChannel
                        The channel of the property to change where 0 is the main channel.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to change.
    @param          inPropertyDataSize
                        A UInt32 indicating the size of the buffer pointed to by inData.
    @param          inPropertyData
                        The buffer containing the data to be used to change the property's value.
    @result         An OSStatus indicating success or failure. Note that if the plug-in does not
                    implement the given property, it should return
                    kAudioHardwareUnknownPropertyError to ensure that the HAL handles the property
                    normally.
*/
extern OSStatus
AudioDriverPlugInStreamSetProperty( AudioDeviceID                       inDevice,
                                    io_object_t                         inIOAudioStream,
                                    const AudioTimeStamp* __nullable    inWhen,
                                    UInt32                              inChannel,
                                    AudioDevicePropertyID               inPropertyID,
                                    UInt32                              inPropertyDataSize,
                                    const void*                         inPropertyData);

//==================================================================================================

#if defined(__cplusplus)
}
#endif

CF_ASSUME_NONNULL_END

#endif  //  CoreAudio_AudioDriverPlugIn_h
