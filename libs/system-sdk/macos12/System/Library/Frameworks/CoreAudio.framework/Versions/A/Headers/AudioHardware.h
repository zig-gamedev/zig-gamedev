/*==================================================================================================
     File:       CoreAudio/AudioHardware.h

     Contains:   API for communicating with audio hardware.

     Copyright:  (c) 1985-2011 by Apple, Inc., all rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

==================================================================================================*/
#if !defined(CoreAudio_AudioHardware_h)
#define CoreAudio_AudioHardware_h

//==================================================================================================
#pragma mark -
#pragma mark Overview
/*!
    @header AudioHardware
    The audio HAL provides an abstraction through which applications can access audio hardware. To
    do this, the HAL provides a small set of AudioObjects that provide access to the various pieces
    of the system.
    
    AudioObjects all have a set of properties that describe and manipulate their state. A property
    is accessed via an ordered triple. The first ordinate is the selector which describes the
    property. The other two ordinates are the scope and element that identify the particular part of
    the object in which to look for the selector. The AudioObjectPropertyAddress structure
    encapsulates the property address. The value of a property is an untyped block of data whose
    content depends on the specifics of the selector. Some selectors also require the use of a
    qualifier when querying. The qualifier allows for additional information to be provided to be
    used in the manipulation of the property. Changing the value of a property is always considered
    asynchronous.
    
    Applications use the routines AudioObjectHasProperty(), AudioObjectIsPropertySettable() and
    AudioObjectGetPropertyDataSize() to find useful meta-information about the property. Apps use
    AudioObjectGetPropertyData() and AudioObjectSetPropertyData() to manipulate the value of the
    property. Apps use AudioObjectAddPropertyListener() and AudioObjectRemovePropertyListener() to
    register/unregister a function that is to be called when a given property's value changes.
    
    The class of an AudioObject determines the basic functionality of the object in terms of what
    functions will operate on it as well as the set of properties that can be expected to be
    implemented by the object. The set of available classes for objects is limited to those defined
    here. There are no other classes. The set of classes is arranged in a hierarchy such that one
    class inherits the properties/routines of its super class.
    
    The base class for all AudioObjects is the class AudioObject. As such, each AudioObject will 
    provide basic properties such as its class, its human readable name, and the other
    AudioObjects it contains. Other important classes include AudioSystemObject, AudioDevice, and
    AudioStream.
    
    The AudioObjects in the HAL are arranged in a containment hierarchy. The root of the hierarchy
    is the one and only instance of the AudioSystemObject class. The properties of the
    AudioSystemObject describe the process global settings such as the various default devices and
    the notification run loop. The AudioSystemObject also contains all the AudioDevices that are
    available.
    
    Instances of the AudioDevice class encapsulate individual audio devices. An AudioDevice serves
    as the basic unit of IO. It provides a single IO cycle, a timing source based on it, and all the
    buffers synchronized to it. The IO cycle presents all the synchronized buffers to the client in
    the same call out along with time stamps that specify the current time, when the input data was
    acquired and when the output data will be presented.
    
    AudioDevices contain instances of the AudioStream class. An AudioStream represents a single
    buffer of data for transferring across the user/kernel boundary. As such, AudioStreams are the
    gatekeepers of format information. Each has its own format and list of available formats.
    AudioStreams can provide data in any format, including encoded formats and non-audio formats. If
    the format is a linear PCM format, the data will always be presented as 32 bit, native endian
    floating point. All conversions to and from the true physical format of the hardware is handled
    by the device's driver.
    
    Both AudioDevices and AudioStreams can contain instances of the AudioControl class or its many
    subclasses. An AudioControl provides properties that describe/manipulate a particular aspect of
    the object such as gain, mute, data source selection, etc. Many common controls are also
    also available as properties on the AudioDevice or AudioStream.
*/

//==================================================================================================
#pragma mark -
#pragma mark Includes

#include <Availability.h>
#include <CoreAudioTypes/CoreAudioTypes.h>
#include <CoreAudio/AudioHardwareBase.h>
#include <CoreFoundation/CoreFoundation.h>
#include <dispatch/dispatch.h>

//==================================================================================================

CF_ASSUME_NONNULL_BEGIN

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
#pragma mark Basic Constants

/*!
    @enum           Predefined AudioObjectID values 
    @abstract       ObjectIDs that are always the same
    @constant       kAudioObjectSystemObject
                        The AudioObjectID that always refers to the one and only instance of the
                        AudioSystemObject class.
*/
CF_ENUM(int)
{
    kAudioObjectSystemObject    = 1
};

//==================================================================================================
#pragma mark -
#pragma mark AudioObject Types

/*!
    @typedef        AudioObjectPropertyListenerProc
    @abstract       Clients register an AudioObjectPropertyListenerProc with an AudioObject in order
                    to receive notifications when the properties of the object change.
    @discussion     Listeners will be called when possibly many properties have changed.
                    Consequently, the implementation of a listener must go through the array of
                    addresses to see what exactly has changed. Note that the array of addresses will
                    always have at least one address in it for which the listener is signed up to
                    receive notifications about but may contain addresses for properties for which
                    the listener is not signed up to receive notifications.
    @param          inObjectID
                        The AudioObject whose properties have changed.
    @param          inNumberAddresses
                        The number of elements in the inAddresses array.
    @param          inAddresses
                        An array of AudioObjectPropertyAddresses indicating which properties
                        changed.
    @param          inClientData
                        A pointer to client data established when the listener proc was registered
                        with the AudioObject.
    @result         The return value is currently unused and should always be 0.
*/
typedef OSStatus
(*AudioObjectPropertyListenerProc)( AudioObjectID                       inObjectID,
                                    UInt32                              inNumberAddresses,
                                    const AudioObjectPropertyAddress*   inAddresses,
                                    void* __nullable                    inClientData);

/*!
    @typedef        AudioObjectPropertyListenerBlock
    @abstract       Clients register an AudioObjectPropertyListenerBlock with an AudioObject in
                    order to receive notifications when the properties of the object change.
    @discussion     Listeners will be called when possibly many properties have changed.
                    Consequently, the implementation of a listener must go through the array of
                    addresses to see what exactly has changed. Note that the array of addresses will
                    always have at least one address in it for which the listener is signed up to
                    receive notifications about but may contain addresses for properties for which
                    the listener is not signed up to receive notifications.
    @param          inNumberAddresses
                        The number of elements in the inAddresses array.
    @param          inAddresses
                        An array of AudioObjectPropertyAddresses indicating which properties
                        changed.
*/
typedef void
(^AudioObjectPropertyListenerBlock)(    UInt32                              inNumberAddresses,
                                        const AudioObjectPropertyAddress*   inAddresses);

//==================================================================================================
#pragma mark AudioObject Properties

/*!
    @enum           AudioObject Property Selectors
    @abstract       AudioObjectPropertySelector values provided by objects of the AudioObject class.
    @discussion     The AudioObject class is the base class for all classes. As such, all classes
                    inherit this set of properties.
    @constant       kAudioObjectPropertyCreator
                        A CFString that contains the bundle ID of the plug-in that instantiated the
                        object. The caller is responsible for releasing the returned CFObject.
    @constant       kAudioObjectPropertyListenerAdded
                        An AudioObjectPropertyAddress indicating the address to which a new listener
                        was added. Note that this property is not for applications to use. Rather,
                        this property is for the HAL shell to notify AudioObjects implemented by an
                        AudioPlugIn when a listener is added.
    @constant       kAudioObjectPropertyListenerRemoved
                        An AudioObjectPropertyAddress indicating the address to which a listener was
                        removed. Note that this property is not for applications to use. Rather,
                        this property is for the HAL shell to notify AudioObjects implemented by an
                        AudioPlugIn when a listener is removed.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioObjectPropertyCreator             = 'oplg',
    kAudioObjectPropertyListenerAdded       = 'lisa',
    kAudioObjectPropertyListenerRemoved     = 'lisr'
};

//==================================================================================================
#pragma mark AudioObject Functions

/*!
    @functiongroup  AudioObject
*/

/*!
    @function       AudioObjectShow
    @abstract       Prints to standard out a textural description of the AudioObject.
    @param          inObjectID
                        The AudioObject to show.
*/
extern void
AudioObjectShow(    AudioObjectID   inObjectID)                                                     __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectHasProperty
    @abstract       Queries an AudioObject about whether or not it has the given property.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @result         A Boolean indicating whether or not the AudioObject has the given property.
*/
extern Boolean
AudioObjectHasProperty( AudioObjectID                       inObjectID,
                        const AudioObjectPropertyAddress*   inAddress)                              __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectIsPropertySettable
    @abstract       Queries an AudioObject about whether or not the given property can be set using
                    AudioObjectSetPropertyData.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @param          outIsSettable
                        A Boolean indicating whether or not the property can be set.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectIsPropertySettable(  AudioObjectID                       inObjectID,
                                const AudioObjectPropertyAddress*   inAddress,
                                Boolean*                            outIsSettable)                  __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectGetPropertyDataSize
    @abstract       Queries an AudioObject to find the size of the data for the given property.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @param          inQualifierDataSize
                        A UInt32 indicating the size of the buffer pointed to by inQualifierData.
                        Note that not all properties require qualification, in which case this
                        value will be 0.
    @param          inQualifierData
                        A buffer of data to be used in determining the data of the property being
                        queried. Note that not all properties require qualification, in which case
                        this value will be NULL.
    @param          outDataSize
                        A UInt32 indicating how many bytes the data for the given property occupies.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectGetPropertyDataSize( AudioObjectID                       inObjectID,
                                const AudioObjectPropertyAddress*   inAddress,
                                UInt32                              inQualifierDataSize,
                                const void* __nullable              inQualifierData,
                                UInt32*                             outDataSize)                    __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectGetPropertyData
    @abstract       Queries an AudioObject to get the data of the given property and places it in
                    the provided buffer.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @param          inQualifierDataSize
                        A UInt32 indicating the size of the buffer pointed to by inQualifierData.
                        Note that not all properties require qualification, in which case this
                        value will be 0.
    @param          inQualifierData
                        A buffer of data to be used in determining the data of the property being
                        queried. Note that not all properties require qualification, in which case
                        this value will be NULL.
    @param          ioDataSize
                        A UInt32 which on entry indicates the size of the buffer pointed to by
                        outData and on exit indicates how much of the buffer was used.
    @param          outData
                        The buffer into which the AudioObject will put the data for the given
                        property.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectGetPropertyData( AudioObjectID                       inObjectID,
                            const AudioObjectPropertyAddress*   inAddress,
                            UInt32                              inQualifierDataSize,
                            const void* __nullable              inQualifierData,
                            UInt32*                             ioDataSize,
                            void*                               outData)                            __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectSetPropertyData
    @abstract       Tells an AudioObject to change the value of the given property using the
                    provided data.
    @discussion     Note that the value of the property should not be considered changed until the
                    HAL has called the listeners as many properties values are changed
                    asynchronously.
    @param          inObjectID
                        The AudioObject to change.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being changed.
    @param          inQualifierDataSize
                        A UInt32 indicating the size of the buffer pointed to by inQualifierData.
                        Note that not all properties require qualification, in which case this
                        value will be 0.
    @param          inQualifierData
                        A buffer of data to be used in determining the data of the property being
                        queried. Note that not all properties require qualification, in which case
                        this value will be NULL.
    @param          inDataSize
                        A UInt32 indicating the size of the buffer pointed to by inData.
    @param          inData
                        The buffer containing the data to be used to change the property's value.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectSetPropertyData( AudioObjectID                       inObjectID,
                            const AudioObjectPropertyAddress*   inAddress,
                            UInt32                              inQualifierDataSize,
                            const void* __nullable              inQualifierData,
                            UInt32                              inDataSize,
                            const void*                         inData)                             __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectAddPropertyListener
    @abstract       Registers the given AudioObjectPropertyListenerProc to receive notifications
                    when the given properties change.
    @param          inObjectID
                        The AudioObject to register the listener with.
    @param          inAddress
                        The AudioObjectPropertyAddresses indicating which property the listener
                        should be notified about.
    @param          inListener
                        The AudioObjectPropertyListenerProc to call.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectAddPropertyListener( AudioObjectID                       inObjectID,
                                const AudioObjectPropertyAddress*   inAddress,
                                AudioObjectPropertyListenerProc     inListener,
                                void* __nullable                    inClientData)                   __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectRemovePropertyListener
    @abstract       Unregisters the given AudioObjectPropertyListenerProc from receiving
                    notifications when the given properties change.
    @param          inObjectID
                        The AudioObject to unregister the listener from.
    @param          inAddress
                        The AudioObjectPropertyAddress indicating from which property the listener
                        should be removed.
    @param          inListener
                        The AudioObjectPropertyListenerProc being removed.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectRemovePropertyListener(  AudioObjectID                       inObjectID,
                                    const AudioObjectPropertyAddress*   inAddress,
                                    AudioObjectPropertyListenerProc     inListener,
                                    void* __nullable                    inClientData)               __OSX_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/*!
    @function       AudioObjectAddPropertyListenerBlock
    @abstract       Registers the given AudioObjectPropertyListenerBlock to receive notifications
                    when the given properties change.
    @param          inObjectID
                        The AudioObject to register the listener with.
    @param          inAddress
                        The AudioObjectPropertyAddresses indicating which property the listener
                        should be notified about.
    @param          inDispatchQueue
                        The dispatch queue on which the listener block will be dispatched. All
                        listener blocks will be dispatched asynchronously save for those dispatched
                        from the IO context (of which kAudioDevicePropertyDeviceIsRunning and
                        kAudioDeviceProcessorOverload are the only examples) which will be
                        dispatched synchronously. Note that this dispatch queue will be retained
                        until a matching call to AudioObjectRemovePropertyListenerBlock is made. If
                        this value is NULL, then the block will be directly invoked.
    @param          inListener
                        The AudioObjectPropertyListenerBlock to call. Note that this block will be
                        Block_copy'd and the reference maintained until a matching call to
                        AudioObjectRemovePropertyListenerBlock is made.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectAddPropertyListenerBlock(    AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        dispatch_queue_t __nullable         inDispatchQueue,
                                        AudioObjectPropertyListenerBlock    inListener)             __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_7_0);

/*!
    @function       AudioObjectRemovePropertyListenerBlock
    @abstract       Unregisters the given AudioObjectPropertyListenerBlock from receiving
                    notifications when the given properties change.
    @param          inObjectID
                        The AudioObject to unregister the listener from.
    @param          inAddress
                        The AudioObjectPropertyAddress indicating from which property the listener
                        should be removed.
    @param          inDispatchQueue
                        The dispatch queue on which the listener block was being dispatched to. 
    @param          inListener
                        The AudioObjectPropertyListenerBlock being removed.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioObjectRemovePropertyListenerBlock( AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        dispatch_queue_t __nullable         inDispatchQueue,
                                        AudioObjectPropertyListenerBlock    inListener)             __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_7_0);

//==================================================================================================
#pragma mark -
#pragma mark AudioSystemObject Constants

/*!
    @enum           AudioSystemObject Class Constants
    @abstract       Various constants related to the AudioSystemObject class.
    @discussion     Note that there is only ever one instance of the AudioSystemObject class and it
                    is available via the AudioObjectID, kAudioObjectSystemObject.
    @constant       kAudioSystemObjectClassID
                        The AudioClassID that identifies the AudioSystemObject class.
*/
CF_ENUM(AudioClassID)
{
    kAudioSystemObjectClassID   = 'asys'
};

/*!
    @enum           Power Hints
    @abstract       The values for kAudioHardwarePropertyPowerHint
    @discussion     The system object property, kAudioHardwarePropertyPowerHint, allows a process to
                    to indicate how aggressive the system can be with optimizations that save power.
                    Note that the value of this property can be set in an application's info.plist
                    using the key, "AudioHardwarePowerHint". The values for this key are the strings
                    that correspond to the values in the enum.
    @constant       kAudioHardwarePowerHintNone
                        This is the default value and it indicates that the system will not make any
                        power optimizations that compromise latency or quality in order to save
                        power. The info.plist value is "None" or the "AudioHardwarePowerHint" entry
                        can be omitted entirely.
    @constant       kAudioHardwarePowerHintFavorSavingPower
                        The system will choose to save power even at the expense of latency. The
                        info.plist value is "Favor Saving Power"
*/
typedef CF_ENUM(UInt32, AudioHardwarePowerHint)
{
    kAudioHardwarePowerHintNone             = 0,
    kAudioHardwarePowerHintFavorSavingPower = 1
};

//==================================================================================================
#pragma mark AudioSystemObject Properties

/*!
    @enum           AudioSystemObject Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioSystemObject class.
    @discussion     The AudioSystemObject class is a subclass of the AudioObject class. the class
                    has just the global scope, kAudioObjectPropertyScopeGlobal, and only a main element.
    @constant       kAudioHardwarePropertyDevices
                        An array of the AudioObjectIDs that represent all the devices currently
                        available to the system.
    @constant       kAudioHardwarePropertyDefaultInputDevice
                        The AudioObjectID of the default input AudioDevice.
    @constant       kAudioHardwarePropertyDefaultOutputDevice
                        The AudioObjectID of the default output AudioDevice.
    @constant       kAudioHardwarePropertyDefaultSystemOutputDevice
                        The AudioObjectID of the output AudioDevice to use for system related sound
                        from the alert sound to digital call progress.
    @constant       kAudioHardwarePropertyTranslateUIDToDevice
                        This property fetches the AudioObjectID that corresponds to the AudioDevice
                        that has the given UID. The UID is passed in via the qualifier as a CFString
                        while the AudioObjectID for the AudioDevice is returned to the caller as the
                        property's data. Note that an error is not returned if the UID doesn't refer
                        to any AudioDevices. Rather, this property will return kAudioObjectUnknown
                        as the value of the property.
    @constant       kAudioHardwarePropertyMixStereoToMono
                        A UInt32 where a value other than 0 indicates that AudioDevices should mix
                        stereo signals down to mono. Note that the two channels on the device that
                        comprise the stereo signal are defined on the device by
                        kAudioDevicePropertyPreferredChannelsForStereo.
    @constant       kAudioHardwarePropertyPlugInList
                        An array of AudioObjectIDs that represent all the AudioPlugIn objects
                        currently provided by the system
    @constant       kAudioHardwarePropertyTranslateBundleIDToPlugIn
                        This property fetches the AudioObjectID that corresponds to the AudioPlugIn
                        that has the given bundle ID. The bundle ID is passed in via the qualifier
                        as a CFString while the AudioObjectID for the AudioPlugIn is returned to the
                        caller as the property's data. Note that an error is not returned if the UID
                        doesn't refer to any AudioPlugIns. Rather, this property will return
                        kAudioObjectUnknown as the value of the property.
    @constant       kAudioHardwarePropertyTransportManagerList
                        An array of the AudioObjectIDs for all the AudioTransportManager objects.
    @constant       kAudioHardwarePropertyTranslateBundleIDToTransportManager
                        This property fetches the AudioObjectID that corresponds to the
                        AudioTransportManager whose bundle has the given bundle ID. The bundle ID is
                        passed in via the qualifier as a CFString while the AudioObjectID for the
                        AudioTransportManager is returned to the caller as the property's data. Note
                        that an error is not returned if the bundle ID doesn't refer to any
                        AudioTransportManagers. Rather, this property will return
                        kAudioObjectUnknown as the value of the property.
    @constant       kAudioHardwarePropertyBoxList
                        An array of AudioObjectIDs that represent all the AudioBox objects currently
                        provided by the system.
    @constant       kAudioHardwarePropertyTranslateUIDToBox
                        This property fetches the AudioObjectID that corresponds to the AudioBox
                        that has the given UID. The UID is passed in via the qualifier as a CFString
                        while the AudioObjectID for the AudioBox is returned to the caller as the
                        property's data. Note that an error is not returned if the UID doesn't refer
                        to any AudioBoxes. Rather, this property will return kAudioObjectUnknown
                        as the value of the property.
    @constant       kAudioHardwarePropertyClockDeviceList
                        An array of AudioObjectIDs that represent all the AudioClockDevice objects 
                        currently provided by the system.
    @constant       kAudioHardwarePropertyTranslateUIDToClockDevice
                        This property fetches the AudioObjectID that corresponds to the AudioClockDevice
                        that has the given UID. The UID is passed in via the qualifier as a CFString
                        while the AudioObjectID for the AudioClockDevice is returned to the caller
                        as the property's data. Note that an error is not returned if the UID doesn't
                        refer to any AudioClockDevice. Rather, this property will return 
                        kAudioObjectUnknown as the value of the property.
    @constant       kAudioHardwarePropertyProcessIsMain
                        A UInt32 where 1 means that the current process contains the main instance
                        of the HAL. The main instance of the HAL is the only instance in which
                        plug-ins should save/restore their devices' settings.
    @constant       kAudioHardwarePropertyIsInitingOrExiting
                        A UInt32 whose value will be non-zero if the HAL is either in the midst of
                        initializing or in the midst of exiting the process.
    @constant       kAudioHardwarePropertyUserIDChanged
                        This property exists so that clients can tell the HAL when they are changing
                        the effective user ID of the process. The way it works is that a client will
                        set the value of this property and the HAL will flush all its cached per-
                        user preferences such as the default devices. The value of this property is
                        a UInt32, but its value has no currently defined meaning and clients may
                        pass any value when setting it to trigger the cache flush.
    @constant       kAudioHardwarePropertyProcessIsAudible
                        A UInt32 where a non-zero value indicates that the audio of the process will
                        be heard. A value of 0 indicates that all audio in the process will not be
                        heard.
    @constant       kAudioHardwarePropertySleepingIsAllowed
                        A UInt32 where 1 means that the process will allow the CPU to idle sleep
                        even if there is audio IO in progress. A 0 means that the CPU will not be
                        allowed to idle sleep. Note that this property won't affect when the CPU is
                        forced to sleep.
    @constant       kAudioHardwarePropertyUnloadingIsAllowed
                        A UInt32 where 1 means that this process wants the HAL to unload itself
                        after a period of inactivity where there are no IOProcs and no listeners
                        registered with any AudioObject.
    @constant       kAudioHardwarePropertyHogModeIsAllowed
                        A UInt32 where 1 means that this process wants the HAL to automatically take
                        hog mode and 0 means that the HAL should not automatically take hog mode on
                        behalf of the process. Processes that only ever use the default device are
                        the sort of that should set this property's value to 0.
    @constant       kAudioHardwarePropertyUserSessionIsActiveOrHeadless
                        A UInt32 where a value other than 0 indicates that the login session of the
                        user of the process is either an active console session or a headless
                        session.
    @constant       kAudioHardwarePropertyServiceRestarted
                        A UInt32 whose value has no meaning. Rather, this property exists so that
                        clients can be informed when the service has been reset for some reason.
                        When a reset happens, any state the client has, such as cached data or
                        added listeners, must be re-established by the client.
    @constant       kAudioHardwarePropertyPowerHint
                        A UInt32 whose values are drawn from the AudioHardwarePowerHint enum above.
                        Only those values are allowed. This property allows a process to indicate how
                        aggressive the system can be with optimizations that save power. The default
                        value is kAudioHardwarePowerHintNone. Note that the value of this
                        property can be set in an application's info.plist using the key,
                        "AudioHardwarePowerHint". The values for this key are the strings that
                        correspond to the values in the Power Hints enum.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioHardwarePropertyDevices                               = 'dev#',
    kAudioHardwarePropertyDefaultInputDevice                    = 'dIn ',
    kAudioHardwarePropertyDefaultOutputDevice                   = 'dOut',
    kAudioHardwarePropertyDefaultSystemOutputDevice             = 'sOut',
    kAudioHardwarePropertyTranslateUIDToDevice                  = 'uidd',
    kAudioHardwarePropertyMixStereoToMono                       = 'stmo',
    kAudioHardwarePropertyPlugInList                            = 'plg#',
    kAudioHardwarePropertyTranslateBundleIDToPlugIn             = 'bidp',
    kAudioHardwarePropertyTransportManagerList                  = 'tmg#',
    kAudioHardwarePropertyTranslateBundleIDToTransportManager   = 'tmbi',
    kAudioHardwarePropertyBoxList                               = 'box#',
    kAudioHardwarePropertyTranslateUIDToBox                     = 'uidb',
    kAudioHardwarePropertyClockDeviceList                       = 'clk#',
    kAudioHardwarePropertyTranslateUIDToClockDevice             = 'uidc',
    kAudioHardwarePropertyProcessIsMain							= 'main',
    kAudioHardwarePropertyIsInitingOrExiting                    = 'inot',
    kAudioHardwarePropertyUserIDChanged                         = 'euid',
    kAudioHardwarePropertyProcessIsAudible                      = 'pmut',
    kAudioHardwarePropertySleepingIsAllowed                     = 'slep',
    kAudioHardwarePropertyUnloadingIsAllowed                    = 'unld',
    kAudioHardwarePropertyHogModeIsAllowed                      = 'hogr',
    kAudioHardwarePropertyUserSessionIsActiveOrHeadless         = 'user',
    kAudioHardwarePropertyServiceRestarted                      = 'srst',
    kAudioHardwarePropertyPowerHint                             = 'powh'
};

//==================================================================================================
#pragma mark AudioSystemObject Functions

/*!
    @functiongroup  AudioSystemObject
*/

/*!
    @function       AudioHardwareUnload
    @abstract       When this routine is called, all IO on all devices within a process will be
                    terminated and all resources capable of being released will be released. This
                    routine essentially returns the HAL to its uninitialized state.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareUnload(void)                                                                           __OSX_AVAILABLE_STARTING(__MAC_10_1, __IPHONE_2_0);

/*!
    @function       AudioHardwareCreateAggregateDevice
    @abstract       This routine creates a new AudioAggregateDevice using the provided description.
    @param          inDescription
                        The CFDictionary that specifies how to build the AudioAggregateDevice. The
                        supported keys are described in the AudioAggregateDevice Constants section.
    @param          outDeviceID
                        The AudioObjectID of the newly created AudioAggregateDevice.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareCreateAggregateDevice( CFDictionaryRef inDescription,
                                    AudioObjectID*  outDeviceID)                                    __OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_7_0);

/*!
    @function       AudioHardwareDestroyAggregateDevice
    @abstract       This routine destroys the given AudioAggregateDevice.
    @discussion     The actual destruction of the device is asynchronous and may take place after
                    the call to this routine has returned.
    @param          inDeviceID
                        The AudioObjectID of the AudioAggregateDevice to destroy.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareDestroyAggregateDevice(AudioObjectID inDeviceID)                                       __OSX_AVAILABLE_STARTING(__MAC_10_9, __IPHONE_7_0);

//==================================================================================================
#pragma mark -
#pragma mark AudioPlugIn Properties

/*!
    @enum           AudioPlugIn Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioPlugIn class.
    @discussion     The AudioPlugIn class is a subclass of the AudioObject class. the class has just
                    the global scope, kAudioObjectPropertyScopeGlobal, and only a main element.
    @constant       kAudioPlugInCreateAggregateDevice
                        This property is used to tell a plug-in to create a new
                        AudioAggregateDevice. Its value is only read. The qualifier data for this
                        property is a CFDictionary containing a description of the
                        AudioAggregateDevice to create. The keys for the CFDictionary are defined in
                        the AudioAggregateDevice Constants section. The value of the property that
                        gets returned is the AudioObjectID of the newly created device.
    @constant       kAudioPlugInDestroyAggregateDevice
                        This property is used to tell a plug-in to destroy an AudioAggregateDevice.
                        Like kAudioPlugInCreateAggregateDevice, this property is read only. The
                        value of the property is the AudioObjectID of the AudioAggregateDevice to
                        destroy.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioPlugInCreateAggregateDevice   = 'cagg',
    kAudioPlugInDestroyAggregateDevice  = 'dagg'
};

//==================================================================================================
#pragma mark -
#pragma mark AudioTransportManager Properties

/*!
    @enum           AudioTransportManager Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioTransportManager class.
    @discussion     The AudioTransportManager class is a subclass of the AudioPlugIn class. The
                    class has just the global scope, kAudioObjectPropertyScopeGlobal, and only a
                    main element.
    @constant       kAudioTransportManagerCreateEndPointDevice
                        This property is used to tell a transport manager to create a new
                        AudioDevice. Its value is only read. The qualifier data for this
                        property is a CFDictionary containing a description of the
                        AudioDevice to create. The standard keys for the CFDictionary are defined in
                        the AudioEndPointDevice Constants section. The value of the property that
                        gets returned is the AudioObjectID of the newly created device.
    @constant       kAudioTransportManagerDestroyEndPointDevice
                        This property is used to tell a transport manager to destroy an AudioDevice.
                        Like kAudioTransportManagerCreateDevice, this property is read only. The
                        value of the property is the AudioObjectID of the AudioAggregateDevice to
                        destroy.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioTransportManagerCreateEndPointDevice  = 'cdev',
    kAudioTransportManagerDestroyEndPointDevice = 'ddev'
    
};

//==================================================================================================
#pragma mark -
#pragma mark AudioDevice Types

/*!
    @typedef        AudioDeviceIOProc
    @abstract       An AudioDeviceIOProc is called by an AudioDevice to provide input data read from
                    the device and collect output data to be written to the device for the current
                    IO cycle.
    @param          inDevice
                        The AudioDevice doing the IO.
    @param          inNow
                        An AudioTimeStamp that indicates the IO cycle started. Note that this time
                        includes any scheduling latency that may have been incurred waking the
                        thread on which IO is being done.
    @param          inInputData
                        An AudioBufferList containing the input data for the current IO cycle. For
                        streams that are disabled, the AudioBuffer's mData field will be NULL but
                        the mDataByteSize field will still say how much data would have been there
                        if it was enabled. Note that the contents of this structure should never be
                        modified.
    @param          inInputTime
                        An AudioTimeStamp that indicates the time at which the first frame in the
                        data was acquired from the hardware. If the device has no input streams, the
                        time stamp will be zeroed out.
    @param          outOutputData
                        An AudioBufferList in which the output data for the current IO cycle is to
                        be placed. On entry, each AudioBuffer's mDataByteSize field indicates the
                        maximum amount of data that can be placed in the buffer and the buffer's
                        memory has been zeroed out. For formats where the number of bytes per packet
                        can vary (as with AC-3, for example), the client has to fill out on exit
                        each mDataByteSize field in each AudioBuffer with the amount of data that
                        was put in the buffer. Otherwise, the mDataByteSize field should not be
                        changed. For streams that are disabled, the AudioBuffer's mData field will
                        be NULL but the mDataByteSize field will still say how much data would have
                        been there if it was enabled. Except as noted above, the contents of this
                        structure should not other wise be modified.
    @param          inOutputTime
                        An AudioTimeStamp that indicates the time at which the first frame in the
                        data will be passed to the hardware. If the device has no output streams,
                        the time stamp will be zeroed out.
    @param          inClientData
                        A pointer to client data established when the AudioDeviceIOProc was
                        registered with the AudioDevice.
    @result         The return value is currently unused and should always be 0.
*/
typedef OSStatus
(*AudioDeviceIOProc)(   AudioObjectID           inDevice,
                        const AudioTimeStamp*   inNow,
                        const AudioBufferList*  inInputData,
                        const AudioTimeStamp*   inInputTime,
                        AudioBufferList*        outOutputData,
                        const AudioTimeStamp*   inOutputTime,
                        void* __nullable        inClientData);

/*!
    @typedef        AudioDeviceIOBlock
    @abstract       An AudioDeviceIOBlock is called by an AudioDevice to provide input data read 
                    from the device and collect output data to be written to the device for the
                    current IO cycle.
    @param          inNow
                        An AudioTimeStamp that indicates the IO cycle started. Note that this time
                        includes any scheduling latency that may have been incurred waking the
                        thread on which IO is being done.
    @param          inInputData
                        An AudioBufferList containing the input data for the current IO cycle. For
                        streams that are disabled, the AudioBuffer's mData field will be NULL but
                        the mDataByteSize field will still say how much data would have been there
                        if it was enabled. Note that the contents of this structure should never be
                        modified.
    @param          inInputTime
                        An AudioTimeStamp that indicates the time at which the first frame in the
                        data was acquired from the hardware. If the device has no input streams, the
                        time stamp will be zeroed out.
    @param          outOutputData
                        An AudioBufferList in which the output data for the current IO cycle is to
                        be placed. On entry, each AudioBuffer's mDataByteSize field indicates the
                        maximum amount of data that can be placed in the buffer and the buffer's
                        memory has been zeroed out. For formats where the number of bytes per packet
                        can vary (as with AC-3, for example), the client has to fill out on exit
                        each mDataByteSize field in each AudioBuffer with the amount of data that
                        was put in the buffer. Otherwise, the mDataByteSize field should not be
                        changed. For streams that are disabled, the AudioBuffer's mData field will
                        be NULL but the mDataByteSize field will still say how much data would have
                        been there if it was enabled. Except as noted above, the contents of this
                        structure should not other wise be modified.
    @param          inOutputTime
                        An AudioTimeStamp that indicates the time at which the first frame in the
                        data will be passed to the hardware. If the device has no output streams,
                        the time stamp will be zeroed out.
*/
typedef void
(^AudioDeviceIOBlock)(  const AudioTimeStamp*   inNow,
                        const AudioBufferList*  inInputData,
                        const AudioTimeStamp*   inInputTime,
                        AudioBufferList*        outOutputData,
                        const AudioTimeStamp*   inOutputTime);

/*!
    @typedef        AudioDeviceIOProcID
    @abstract       An AudioDeviceIOProcID represents both an IOProc and the client data that goes
                    with it. Once created, an AudioDeviceIOProcID can be used everywhere one would
                    use a regular IOProc. The purpose for an AudioDeviceIOProcID is to allow a
                    client to register the same function pointer as an IOProc with a device multiple
                    times provided
*/
typedef AudioDeviceIOProc   AudioDeviceIOProcID;

/*!
    @struct         AudioHardwareIOProcStreamUsage
    @abstract       This structure describes which streams a given AudioDeviceIOProc will use. It is
                    used in conjunction with kAudioDevicePropertyIOProcStreamUsage.
    @field          mIOProc
                        The IOProc whose stream usage is being specified.
    @field          mNumberStreams
                        The number of streams being specified.
    @field          mStreamIsOn
                        An array of UInt32's whose length is specified by mNumberStreams. Each
                        element of the array corresponds to a stream. A value of 0 means the stream
                        is not to be enabled. Any other value means the stream is to be used.
*/
struct  AudioHardwareIOProcStreamUsage
{
    void*   mIOProc;
    UInt32  mNumberStreams;
    UInt32  mStreamIsOn[1];
};
typedef struct AudioHardwareIOProcStreamUsage   AudioHardwareIOProcStreamUsage;

//==================================================================================================
#pragma mark AudioDevice Constants

/*!
    @enum           StartAtTime/GetNearestStartTime Flags
    @abstract       The flags that can be passed to control the behavior of AudioDeviceStartAtTime()
                    andAudioDeviceGetNearestStartTime().
    @constant       kAudioDeviceStartTimeIsInputFlag
                        Set to indicate that the requested time refers to an input time. Clear to
                        indicate that it is an output time.
    @constant       kAudioDeviceStartTimeDontConsultDeviceFlag
                        Set to indicate that the device should not be consulted when determining the
                        start time. Clear to indicate that the device should be consulted. This flag
                        cannot be set if kAudioDeviceStartTimeDontConsultHALFlag is set.
    @constant       kAudioDeviceStartTimeDontConsultHALFlag
                        Set to indicate that the HAL should not be consulted when determining the
                        start time. Clear to indicate that the HAL should be consulted. This flag
                        cannot be set if kAudioDeviceStartTimeDontConsultDeviceFlag is set.
*/
CF_ENUM(UInt32)
{
    kAudioDeviceStartTimeIsInputFlag            = (1 << 0),
    kAudioDeviceStartTimeDontConsultDeviceFlag  = (1 << 1),
    kAudioDeviceStartTimeDontConsultHALFlag     = (1 << 2)
};

//==================================================================================================
#pragma mark AudioDevice Properties

/*!
    @enum           AudioDevice Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioDevice class.
    @discussion     The AudioDevice class is a subclass of the AudioObjectClass. The class has four
                    scopes, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyScopeInput,
                    kAudioObjectPropertyScopeOutput, and kAudioObjectPropertyScopePlayThrough. The
                    class has a main element and an element for each channel in each stream
                    numbered according to the starting channel number of each stream.
    @constant       kAudioDevicePropertyPlugIn
                        An OSStatus that contains any error codes generated by loading the IOAudio
                        driver plug-in for the AudioDevice or kAudioHardwareNoError if the plug-in
                        loaded successfully. This property only exists for IOAudio-based
                        AudioDevices whose driver has specified a plug-in to load.
    @constant       kAudioDevicePropertyDeviceHasChanged
                        The type of this property is a UInt32, but its value has no meaning. This
                        property exists so that clients can listen to it and be told when the
                        configuration of the AudioDevice has changed in ways that cannot otherwise
                        be conveyed through other notifications. In response to this notification,
                        clients should re-evaluate everything they need to know about the device,
                        particularly the layout and values of the controls.
    @constant       kAudioDevicePropertyDeviceIsRunningSomewhere
                        A UInt32 where 1 means that the AudioDevice is running in at least one
                        process on the system and 0 means that it isn't running at all.
    @constant       kAudioDeviceProcessorOverload
                        A UInt32 where the value has no meaning. This property exists so that
                        clients can be notified when the AudioDevice detects that an IO cycle has
                        run past its deadline. Note that the notification for this property is
                        usually sent from the AudioDevice's IO thread.
    @constant       kAudioDevicePropertyIOStoppedAbnormally
                        A UInt32 where the value has no meaning. This property exists so that
                        clients can be notified when IO on the device has stopped outside of the
                        normal mechanisms. This typically comes up when IO is stopped after
                        AudioDeviceStart has returned successfully but prior to the notification for
                        kAudioDevicePropertyIsRunning being sent.
    @constant       kAudioDevicePropertyHogMode
                        A pid_t indicating the process that currently owns exclusive access to the
                        AudioDevice or a value of -1 indicating that the device is currently
                        available to all processes. If the AudioDevice is in a non-mixable mode,
                        the HAL will automatically take hog mode on behalf of the first process to
                        start an IOProc.
                        Note that when setting this property, the value passed in is ignored. If
                        another process owns exclusive access, that remains unchanged. If the
                        current process owns exclusive access, it is released and made available to
                        all processes again. If no process has exclusive access (meaning the current
                        value is -1), this process gains ownership of exclusive access.  On return,
                        the pid_t pointed to by inPropertyData will contain the new value of the
                        property.
    @constant       kAudioDevicePropertyBufferFrameSize
                        A UInt32 whose value indicates the number of frames in the IO buffers.
    @constant       kAudioDevicePropertyBufferFrameSizeRange
                        An AudioValueRange indicating the minimum and maximum values, inclusive, for
                        kAudioDevicePropertyBufferFrameSize.
    @constant       kAudioDevicePropertyUsesVariableBufferFrameSizes
                        A UInt32 that, if implemented by a device, indicates that the sizes of the
                        buffers passed to an IOProc will vary by a small amount. The value of this
                        property will indicate the largest buffer that will be passed and
                        kAudioDevicePropertyBufferFrameSize will indicate the smallest buffer that
                        will get passed to the IOProc. The usage of this property is narrowed to
                        only allow for devices whose buffer sizes vary by small amounts greater than
                        kAudioDevicePropertyBufferFrameSize. It is not intended to be a license for
                        devices to be able to send buffers however they please. Rather, it is
                        intended to allow for hardware whose natural rhythms lead to this necessity.
    @constant       kAudioDevicePropertyIOCycleUsage
                        A Float32 whose range is from 0 to 1. This value indicates how much of the
                        client portion of the IO cycle the process will use. The client portion of
                        the IO cycle is the portion of the cycle in which the device calls the
                        IOProcs so this property does not the apply to the duration of the entire
                        cycle.
    @constant       kAudioDevicePropertyStreamConfiguration
                        This property returns the stream configuration of the device in an
                        AudioBufferList (with the buffer pointers set to NULL) which describes the
                        list of streams and the number of channels in each stream. This corresponds
                        to what will be passed into the IOProc.
    @constant       kAudioDevicePropertyIOProcStreamUsage
                        An AudioHardwareIOProcStreamUsage structure which details the stream usage
                        of a given IO proc. If a stream is marked as not being used, the given
                        IOProc will see a corresponding NULL buffer pointer in the AudioBufferList
                        passed to its IO proc. Note that the number of streams detailed in the
                        AudioHardwareIOProcStreamUsage must include all the streams of that
                        direction on the device. Also, when getting the value of the property, one
                        must fill out the mIOProc field of the AudioHardwareIOProcStreamUsage with
                        the address of the of the IOProc whose stream usage is to be retrieved.
    @constant       kAudioDevicePropertyActualSampleRate
                        A Float64 that indicates the current actual sample rate of the AudioDevice
                        as measured by its time stamps.
    @constant       kAudioDevicePropertyClockDevice
                        A CFString that contains the UID for the AudioClockDevice that is currently
                        serving as the main time base of the device. The caller is responsible
                        for releasing the returned CFObject.
    @constant       kAudioDevicePropertyIOThreadOSWorkgroup
                        An os_workgroup_t that represents the thread workgroup the AudioDevice's
                        IO thread belongs to. The caller is responsible for releasing the returned
                        object.
	@constant		kAudioDevicePropertyProcessMute
						A UInt32 where a non-zero value indicates that the current process's audio
						will be zeroed out by the system. Note that this property does not apply to
						aggregate devices, just real, physical devices.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioDevicePropertyPlugIn                          = 'plug',
    kAudioDevicePropertyDeviceHasChanged                = 'diff',
    kAudioDevicePropertyDeviceIsRunningSomewhere        = 'gone',
    kAudioDeviceProcessorOverload                       = 'over',
    kAudioDevicePropertyIOStoppedAbnormally             = 'stpd',
    kAudioDevicePropertyHogMode                         = 'oink',
    kAudioDevicePropertyBufferFrameSize                 = 'fsiz',
    kAudioDevicePropertyBufferFrameSizeRange            = 'fsz#',
    kAudioDevicePropertyUsesVariableBufferFrameSizes    = 'vfsz',
    kAudioDevicePropertyIOCycleUsage                    = 'ncyc',
    kAudioDevicePropertyStreamConfiguration             = 'slay',
    kAudioDevicePropertyIOProcStreamUsage               = 'suse',
    kAudioDevicePropertyActualSampleRate                = 'asrt',
    kAudioDevicePropertyClockDevice                     = 'apcd',
    kAudioDevicePropertyIOThreadOSWorkgroup             = 'oswg',
    kAudioDevicePropertyProcessMute						= 'appm'
};

/*!
    @enum           AudioDevice Properties Implemented via AudioControl objects
    @abstract       AudioObjectPropertySelector values for AudioDevice properties that are
                    implemented by AudioControl objects.
    @discussion     These properties are also accessible by locating the AudioControl object
                    attached to the AudioDevice and using that object to access the properties of
                    the control.
    @constant       kAudioDevicePropertyJackIsConnected
                        A UInt32 where a value of 0 means that there isn't anything plugged into the
                        jack associated withe given element and scope. This property is implemented
                        by an AudioJackControl, a subclass of AudioBooleanControl.
    @constant       kAudioDevicePropertyVolumeScalar
                        A Float32 that represents the value of the volume control. The range is
                        between 0.0 and 1.0 (inclusive). Note that the set of all Float32 values
                        between 0.0 and 1.0 inclusive is much larger than the set of actual values
                        that the hardware can select. This means that the Float32 range has a many
                        to one mapping with the underlying hardware values. As such, setting a
                        scalar value will result in the control taking on the value nearest to what
                        was set. This property is implemented by an AudioControl object that is a
                        subclass of AudioVolumeControl.
    @constant       kAudioDevicePropertyVolumeDecibels
                        A Float32 that represents the value of the volume control in dB. Note that
                        the set of all Float32 values in the dB range for the control is much larger
                        than the set of actual values that the hardware can select. This means that
                        the Float32 range has a many to one mapping with the underlying hardware
                        values. As such, setting a dB value will result in the control taking on the
                        value nearest to what was set. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl.
    @constant       kAudioDevicePropertyVolumeRangeDecibels
                        An AudioValueRange that contains the minimum and maximum dB values the
                        control can have. This property is implemented by an AudioControl object
                        that is a subclass of AudioVolumeControl.
    @constant       kAudioDevicePropertyVolumeScalarToDecibels
                        A Float32 that on input contains a scalar volume value for the and on exit
                        contains the equivalent dB value. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl.
    @constant       kAudioDevicePropertyVolumeDecibelsToScalar
                        A Float32 that on input contains a dB volume value for the and on exit
                        contains the equivalent scalar value. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl.
    @constant       kAudioDevicePropertyStereoPan
                        A Float32 where 0.0 is full left, 1.0 is full right, and 0.5 is center. This
                        property is implemented by an AudioControl object that is a subclass of
                        AudioStereoPanControl.
    @constant       kAudioDevicePropertyStereoPanChannels
                        An array of two UInt32s that indicate which elements of the owning object
                        the signal is being panned between. This property is implemented by an
                        AudioControl object that is a subclass of AudioStereoPanControl.
    @constant       kAudioDevicePropertyMute
                        A UInt32 where a value of 1 means that mute is enabled making that element
                        inaudible. The property is implemented by an AudioControl object that is a
                        subclass of AudioMuteControl.
    @constant       kAudioDevicePropertySolo
                        A UInt32 where a value of 1 means that just that element is audible and the
                        other elements are inaudible. The property is implemented by an AudioControl
                        object that is a subclass of AudioSoloControl.
    @constant       kAudioDevicePropertyPhantomPower
                        A UInt32 where a value of 1 means that the AudioDevice has enabled phantom
                        power for the given element. The property is implemented by an AudioControl
                        object that is a subclass of AudioPhantomPowerControl.
    @constant       kAudioDevicePropertyPhaseInvert
                        A UInt32 where a value of 1 means that phase of the signal for the given
                        element has been flipped 180 degrees. The property is implemented by an
                        AudioControl object that is a subclass of AudioPhaseInvertControl.
    @constant       kAudioDevicePropertyClipLight
                        A UInt32 where a value of 1 means that the signal for the element has
                        exceeded the sample range. Once a clip light is turned on, it is to stay on
                        until either the value of the control is set to false or the current IO
                        session stops and a new IO session starts. The property is implemented by an
                        AudioControl object that is a subclass of AudioClipLightControl.
    @constant       kAudioDevicePropertyTalkback
                        A UInt32 where a value of 1 means that the talkback channel is enabled. The
                        property is implemented by an AudioControl object that is a subclass of
                        AudioTalkbackControl.
    @constant       kAudioDevicePropertyListenback
                        A UInt32 where a value of 1 means that the listenback channel is enabled.
                        The property is implemented by an AudioControl object that is a subclass of
                        AudioListenbackControl.
    @constant       kAudioDevicePropertyDataSource
                        An array of UInt32s whose values are the item IDs for the currently selected
                        data sources. This property is implemented by an AudioControl object that is
                        a subclass of AudioDataSourceControl.
    @constant       kAudioDevicePropertyDataSources
                        An array of UInt32s that are represent all the IDs of all the data sources
                        currently available. This property is implemented by an AudioControl object
                        that is a subclass of AudioDataSourceControl.
    @constant       kAudioDevicePropertyDataSourceNameForIDCFString
                        This property translates the given data source item ID into a human readable
                        name using an AudioValueTranslation structure. The input data is the UInt32
                        containing the item ID to translated and the output data is a CFString. The
                        caller is responsible for releasing the returned CFObject. This property is
                        implemented by an AudioControl object that is a subclass of
                        AudioDataSourceControl.
    @constant       kAudioDevicePropertyDataSourceKindForID
                        This property returns a UInt32 that identifies the kind of data source
                        the item ID refers to using an AudioValueTranslation structure. The input
                        data is the UInt32 containing the item ID and the output data is the UInt32.
    @constant       kAudioDevicePropertyClockSource
                        An array of UInt32s whose values are the item IDs for the currently selected
                        clock sources. This property is implemented by an AudioControl object that
                        is a subclass of AudioClockControl.
    @constant       kAudioDevicePropertyClockSources
                        An array of UInt32s that are represent all the IDs of all the clock sources
                        currently available. This property is implemented by an AudioControl object
                        that is a subclass of AudioClockControl.
    @constant       kAudioDevicePropertyClockSourceNameForIDCFString
                        This property translates the given clock source item ID into a human
                        readable name using an AudioValueTranslation structure. The input data is
                        the UInt32 containing the item ID to translated and the output data is a
                        CFString. The caller is responsible for releasing the returned CFObject.
                        This property is implemented by an AudioControl object that is a subclass of
                        AudioClockControl.
    @constant       kAudioDevicePropertyClockSourceKindForID
                        This property returns a UInt32 that identifies the kind of clock source
                        the item ID refers to using an AudioValueTranslation structure. The input
                        data is the UInt32 containing the item ID and the output data is the UInt32.
    @constant       kAudioDevicePropertyPlayThru
                        A UInt32 where a value of 0 means that play through is off and a value of 1
                        means that it is on. This property is implemented by an AudioControl object
                        that is a subclass of AudioMuteControl. Further, the control that implements
                        this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruSolo
                        A UInt32 where a value of 1 means that just that play through element is
                        audible and the other elements are inaudible. The property is implemented by
                        an AudioControl object that is a subclass of AudioSoloControl. Further, the
                        control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruVolumeScalar
                        A Float32 that represents the value of the volume control. The range is
                        between 0.0 and 1.0 (inclusive). Note that the set of all Float32 values
                        between 0.0 and 1.0 inclusive is much larger than the set of actual values
                        that the hardware can select. This means that the Float32 range has a many
                        to one mapping with the underlying hardware values. As such, setting a
                        scalar value will result in the control taking on the value nearest to what
                        was set. This property is implemented by an AudioControl object that is a
                        subclass of AudioVolumeControl.Further, the control that implements this
                        property is only available through kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruVolumeDecibels
                        A Float32 that represents the value of the volume control in dB. Note that
                        the set of all Float32 values in the dB range for the control is much larger
                        than the set of actual values that the hardware can select. This means that
                        the Float32 range has a many to one mapping with the underlying hardware
                        values. As such, setting a dB value will result in the control taking on the
                        value nearest to what was set. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl. Further, the
                        control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruVolumeRangeDecibels
                        An AudioValueRange that contains the minimum and maximum dB values the
                        control can have. This property is implemented by an AudioControl object
                        that is a subclass of AudioVolumeControl. Further, the control that
                        implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruVolumeScalarToDecibels
                        A Float32 that on input contains a scalar volume value for the and on exit
                        contains the equivalent dB value. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl. Further, the
                        control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruVolumeDecibelsToScalar
                        A Float32 that on input contains a dB volume value for the and on exit
                        contains the equivalent scalar value. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl. Further, the
                        control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruStereoPan
                        A Float32 where 0.0 is full left, 1.0 is full right, and 0.5 is center. This
                        property is implemented by an AudioControl object that is a subclass of
                        AudioStereoPanControl. Further, the control that implements this property is
                        only available through kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruStereoPanChannels
                        An array of two UInt32s that indicate which elements of the owning object
                        the signal is being panned between. This property is implemented by an
                        AudioControl object that is a subclass of AudioStereoPanControl. Further,
                        the control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruDestination
                        An array of UInt32s whose values are the item IDs for the currently selected
                        play through data destinations. This property is implemented by an
                        AudioControl object that is a subclass of AudioDataDestinationControl.
                        Further, the control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruDestinations
                        An array of UInt32s that are represent all the IDs of all the play through
                        data destinations currently available. This property is implemented by an
                        AudioControl object that is a subclass of AudioDataDestinationControl.
                        Further, the control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyPlayThruDestinationNameForIDCFString
                        This property translates the given play through data destination item ID
                        into a human readable name using an AudioValueTranslation structure. The
                        input data is the UInt32 containing the item ID to translated and the output
                        data is a CFString. The caller is responsible for releasing the returned
                        CFObject. This property is implemented by an AudioControl object that is a
                        subclass of AudioDataDestinationControl. Further, the control that
                        implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyChannelNominalLineLevel
                        An array of UInt32s whose values are the item IDs for the currently selected
                        nominal line levels. This property is implemented by an AudioControl object
                        that is a subclass of AudioLineLevelControl.
    @constant       kAudioDevicePropertyChannelNominalLineLevels
                        An array of UInt32s that represent all the IDs of all the nominal line
                        levels currently available. This property is implemented by an AudioControl
                        object that is a subclass of AudioLineLevelControl.
    @constant       kAudioDevicePropertyChannelNominalLineLevelNameForIDCFString
                        This property translates the given nominal line level item ID into a human
                        readable name using an AudioValueTranslation structure. The input data is
                        the UInt32 containing the item ID to be translated and the output data is a
                        CFString. The caller is responsible for releasing the returned CFObject.
                        This property is implemented by an AudioControl object that is a subclass of
                        AudioLineLevelControl.
    @constant       kAudioDevicePropertyHighPassFilterSetting
                        An array of UInt32s whose values are the item IDs for the currently selected
                        high pass filter setting. This property is implemented by an AudioControl
                        object that is a subclass of AudioHighPassFilterControl.
    @constant       kAudioDevicePropertyHighPassFilterSettings
                        An array of UInt32s that represent all the IDs of all the high pass filter
                        settings currently available. This property is implemented by an
                        AudioControl object that is a subclass of AudioHighPassFilterControl.
    @constant       kAudioDevicePropertyHighPassFilterSettingNameForIDCFString
                        This property translates the given high pass filter setting item ID into a
                        human readable name using an AudioValueTranslation structure. The input data
                        is the UInt32 containing the item ID to be translated and the output data is
                        a CFString. The caller is responsible for releasing the returned CFObject.
                        This property is implemented by an AudioControl object that is a subclass of
                        AudioHighPassFilterControl.
    @constant       kAudioDevicePropertySubVolumeScalar
                        A Float32 that represents the value of the LFE volume control. The range is
                        between 0.0 and 1.0 (inclusive). Note that the set of all Float32 values
                        between 0.0 and 1.0 inclusive is much larger than the set of actual values
                        that the hardware can select. This means that the Float32 range has a many
                        to one mapping with the underlying hardware values. As such, setting a
                        scalar value will result in the control taking on the value nearest to what
                        was set. This property is implemented by an AudioControl object that is a
                        subclass of AudioLFEVolumeControl.
    @constant       kAudioDevicePropertySubVolumeDecibels
                        A Float32 that represents the value of the LFE volume control in dB. Note
                        that the set of all Float32 values in the dB range for the control is much
                        larger than the set of actual values that the hardware can select. This
                        means that the Float32 range has a many to one mapping with the underlying
                        hardware values. As such, setting a dB value will result in the control
                        taking on the value nearest to what was set. This property is implemented by
                        an AudioControl object that is a subclass of AudioLFE VolumeControl.
    @constant       kAudioDevicePropertySubVolumeRangeDecibels
                        An AudioValueRange that contains the minimum and maximum dB values the
                        control can have. This property is implemented by an AudioControl object
                        that is a subclass of AudioLFEVolumeControl.
    @constant       kAudioDevicePropertySubVolumeScalarToDecibels
                        A Float32 that on input contains a scalar volume value for the and on exit
                        contains the equivalent dB value. This property is implemented by an
                        AudioControl object that is a subclass of AudioLFEVolumeControl.
    @constant       kAudioDevicePropertySubVolumeDecibelsToScalar
                        A Float32 that on input contains a dB volume value for the and on exit
                        contains the equivalent scalar value. This property is implemented by an
                        AudioControl object that is a subclass of AudioLFEVolumeControl.
    @constant       kAudioDevicePropertySubMute
                        A UInt32 where a value of 1 means that mute is enabled making the LFE on
                        that element inaudible. The property is implemented by an AudioControl
                        object that is a subclass of AudioLFEMuteControl.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioDevicePropertyJackIsConnected                                 = 'jack',
    kAudioDevicePropertyVolumeScalar                                    = 'volm',
    kAudioDevicePropertyVolumeDecibels                                  = 'vold',
    kAudioDevicePropertyVolumeRangeDecibels                             = 'vdb#',
    kAudioDevicePropertyVolumeScalarToDecibels                          = 'v2db',
    kAudioDevicePropertyVolumeDecibelsToScalar                          = 'db2v',
    kAudioDevicePropertyStereoPan                                       = 'span',
    kAudioDevicePropertyStereoPanChannels                               = 'spn#',
    kAudioDevicePropertyMute                                            = 'mute',
    kAudioDevicePropertySolo                                            = 'solo',
    kAudioDevicePropertyPhantomPower                                    = 'phan',
    kAudioDevicePropertyPhaseInvert                                     = 'phsi',
    kAudioDevicePropertyClipLight                                       = 'clip',
    kAudioDevicePropertyTalkback                                        = 'talb',
    kAudioDevicePropertyListenback                                      = 'lsnb',
    kAudioDevicePropertyDataSource                                      = 'ssrc',
    kAudioDevicePropertyDataSources                                     = 'ssc#',
    kAudioDevicePropertyDataSourceNameForIDCFString                     = 'lscn',
    kAudioDevicePropertyDataSourceKindForID                             = 'ssck',
    kAudioDevicePropertyClockSource                                     = 'csrc',
    kAudioDevicePropertyClockSources                                    = 'csc#',
    kAudioDevicePropertyClockSourceNameForIDCFString                    = 'lcsn',
    kAudioDevicePropertyClockSourceKindForID                            = 'csck',
    kAudioDevicePropertyPlayThru                                        = 'thru',
    kAudioDevicePropertyPlayThruSolo                                    = 'thrs',
    kAudioDevicePropertyPlayThruVolumeScalar                            = 'mvsc',
    kAudioDevicePropertyPlayThruVolumeDecibels                          = 'mvdb',
    kAudioDevicePropertyPlayThruVolumeRangeDecibels                     = 'mvd#',
    kAudioDevicePropertyPlayThruVolumeScalarToDecibels                  = 'mv2d',
    kAudioDevicePropertyPlayThruVolumeDecibelsToScalar                  = 'mv2s',
    kAudioDevicePropertyPlayThruStereoPan                               = 'mspn',
    kAudioDevicePropertyPlayThruStereoPanChannels                       = 'msp#',
    kAudioDevicePropertyPlayThruDestination                             = 'mdds',
    kAudioDevicePropertyPlayThruDestinations                            = 'mdd#',
    kAudioDevicePropertyPlayThruDestinationNameForIDCFString            = 'mddc',
    kAudioDevicePropertyChannelNominalLineLevel                         = 'nlvl',
    kAudioDevicePropertyChannelNominalLineLevels                        = 'nlv#',
    kAudioDevicePropertyChannelNominalLineLevelNameForIDCFString        = 'lcnl',
    kAudioDevicePropertyHighPassFilterSetting                           = 'hipf',
    kAudioDevicePropertyHighPassFilterSettings                          = 'hip#',
    kAudioDevicePropertyHighPassFilterSettingNameForIDCFString          = 'hipl',
    kAudioDevicePropertySubVolumeScalar                                 = 'svlm',
    kAudioDevicePropertySubVolumeDecibels                               = 'svld',
    kAudioDevicePropertySubVolumeRangeDecibels                          = 'svd#',
    kAudioDevicePropertySubVolumeScalarToDecibels                       = 'sv2d',
    kAudioDevicePropertySubVolumeDecibelsToScalar                       = 'sd2v',
    kAudioDevicePropertySubMute                                         = 'smut'
};

//==================================================================================================
#pragma mark AudioDevice Functions

/*!
    @functiongroup  AudioDevice
*/

/*!
    @function       AudioDeviceCreateIOProcID
    @abstract       Creates an AudioDeviceIOProcID from an AudioDeviceIOProc and a client data
                    pointer.
    @discussion     AudioDeviceIOProcIDs allow for the client to register the same function pointer
                    with a device multiple times
    @param          inDevice
                        The AudioDevice to register the IOProc with.
    @param          inProc
                        The AudioDeviceIOProc to register.
    @param          inClientData
                        A pointer to client data that is passed back to the IOProc when it is
                        called.
    @param          outIOProcID
                        The newly created AudioDeviceIOProcID.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceCreateIOProcID(  AudioObjectID                               inDevice,
                            AudioDeviceIOProc                           inProc,
                            void* __nullable                            inClientData,
                            AudioDeviceIOProcID __nullable * __nonnull  outIOProcID)                                    __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/*!
    @function       AudioDeviceCreateIOProcIDWithBlock
    @abstract       Creates an AudioDeviceIOProcID from an AudioDeviceIOBlock
    @param          outIOProcID
                        The newly created AudioDeviceIOProcID.
    @param          inDevice
                        The AudioDevice to register the Block with.
    @param          inDispatchQueue
                        The dispatch queue on which the IOBlock will be dispatched. All
                        IOBlocks are dispatched synchronously. Note that this dispatch queue will be
                        retained until a matching call to AudioDeviceDestroyIOProcID is made. If
                        this value is NULL, then the IOBlock will be directly invoked.
    @param          inIOBlock
                        The AudioDeviceIOBlock to register.  Note that this block will be
                        Block_copy'd and the reference maintained until a matching call to
                        AudioDeviceDestroyIOProcID is made.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceCreateIOProcIDWithBlock( AudioDeviceIOProcID __nullable * __nonnull  outIOProcID,
                                    AudioObjectID                               inDevice,
                                    dispatch_queue_t __nullable                 inDispatchQueue,
                                    AudioDeviceIOBlock                          inIOBlock)                              __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_7_0);

/*!
    @function       AudioDeviceDestroyIOProcID
    @abstract       Destroys an AudioDeviceIOProcID.
    @discussion     AudioDeviceIOProcIDs allow for the client to register the same function pointer
                    with a device multiple times
    @param          inDevice
                        The AudioDevice from which the ID came.
    @param          inIOProcID
                        The AudioDeviceIOProcID to get rid of.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceDestroyIOProcID( AudioObjectID       inDevice,
                            AudioDeviceIOProcID inIOProcID)                                     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/*!
    @function       AudioDeviceStart
    @abstract       Starts IO for the given AudioDeviceIOProcID.
    @param          inDevice
                        The AudioDevice to start the IOProc on.
    @param          inProcID
                        The AudioDeviceIOProcID to start. Note that this can be NULL, which starts
                        the hardware regardless of whether or not there are any IOProcs registered.
                        This is necessary if any of the AudioDevice's timing services are to be
                        used. A balancing call to AudioDeviceStop with a NULL IOProc is required to
                        stop the hardware.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceStart(   AudioObjectID                   inDevice,
                    AudioDeviceIOProcID __nullable  inProcID)                                                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceStartAtTime
    @abstract       Starts IO for the given AudioDeviceIOProcID and aligns the IO cycle of the
                    AudioDevice with the given time.
    @param          inDevice
                        The AudioDevice to start the IOProc on.
    @param          inProcID
                        The AudioDeviceIOProcID to start. Note that this can be NULL, which starts
                        the hardware regardless of whether or not there are any IOProcs registered.
    @param          ioRequestedStartTime
                        A pointer to an AudioTimeStamp that, on entry, is the requested time to
                        start the IOProc. On exit, it will be the actual time the IOProc will start.
    @param          inFlags
                        A UInt32 containing flags that modify how this function behaves.
    @result         An OSStatus indicating success or failure.
                    kAudioHardwareUnsupportedOperationError will be returned if the AudioDevice does
                    not support starting at a specific time and inProc and ioRequestedStartTime are
                    not NULL.
*/
extern OSStatus
AudioDeviceStartAtTime( AudioObjectID                   inDevice,
                        AudioDeviceIOProcID __nullable  inProcID,
                        AudioTimeStamp*                 ioRequestedStartTime,
                        UInt32                          inFlags)                                                __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_2_0);

/*!
    @function       AudioDeviceStop
    @abstract       Stops IO for the given AudioDeviceIOProcID.
    @param          inDevice
                        The AudioDevice to stop the IOProc on.
    @param          inProcID
                        The AudioDeviceIOProcID to stop.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceStop(    AudioObjectID                   inDevice,
                    AudioDeviceIOProcID __nullable  inProcID)                                                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceGetCurrentTime
    @abstract       Retrieves the current time from an AudioDevice. Note that the device has to be
                    running.
    @param          inDevice
                        The AudioDevice to from which to get the time.
    @param          outTime
                        An AudioTimeStamp into which the current time is put. On entry, the
                        mFlags field specifies which representations to provide. Because not every
                        device supports all time representations, on exit, the mFlags field will
                        indicate what values are actually valid.
    @result         An OSStatus indicating success or failure. kAudioHardwareNotRunningError will be
                    returned if the AudioDevice isn't running.
*/
extern OSStatus
AudioDeviceGetCurrentTime(  AudioObjectID   inDevice,
                            AudioTimeStamp* outTime)                                                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceTranslateTime
    @abstract       Translates the time in the AudioDevice's time base from one representation to
                    another. Note that the device has to be running
    @param          inDevice
                        The AudioDevice whose time base governs the translation.
    @param          inTime
                        An AudioTimeStamp containing the time to be translated.
    @param          outTime
                        An AudioTimeStamp into which the translated time is put. On entry, the
                        mFlags field specifies which representations to translate the input time
                        into. Because not every device supports all time representations, on exit,
                        the mFlags field will indicate which translations were actually done.
    @result         An OSStatus indicating success or failure. kAudioHardwareNotRunningError will be
                    returned if the AudioDevice isn't running.
*/
extern OSStatus
AudioDeviceTranslateTime(   AudioObjectID           inDevice,
                            const AudioTimeStamp*   inTime,
                            AudioTimeStamp*         outTime)                                        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceGetNearestStartTime
    @abstract       Query an AudioDevice to get a time equal to or later than the given time that is
                    the best time to start IO.
    @discussion     The time that is returned is dictated by the constraints of the device and the
                    system. For instance, the driver of a device that provides both audio and video
                    data may only allow start times that coincide with the edge of a video frame.
                    Also, if the device already has one or more active IOProcs, the start time will
                    be shifted to the beginning of the next IO cycle so as not to cause
                    discontinuities in the existing IOProcs. Another reason the start time may shift
                    is to allow for aligning the buffer accesses in an optimal fashion. Note that
                    the device must be running to use this function.
    @param          inDevice
                        The AudioDevice to query.
    @param          ioRequestedStartTime
                        A pointer to an AudioTimeStamp that, on entry, is the requested start time.
                        On exit, it will have the a time equal to or later than the requested time,
                        as dictated by the device's constraints.
    @param          inFlags
                        A UInt32 containing flags that modify how this function behaves.
    @result         An OSStatus indicating success or failure. kAudioHardwareNotRunningError will be
                    returned if the AudioDevice isn't running.
                    kAudioHardwareUnsupportedOperationError will be returned if the AudioDevice does
                    not support starting at a specific time.
*/
extern OSStatus
AudioDeviceGetNearestStartTime( AudioObjectID   inDevice,
                                AudioTimeStamp* ioRequestedStartTime,
                                UInt32          inFlags)                                            __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_2_0);

//==================================================================================================
#pragma mark -
#pragma mark AudioAggregateDevice Constants

/*!
    @enum           AudioAggregateDevice Class Constants
    @abstract       Various constants related to the AudioAggregateDevice class.
    @constant       kAudioAggregateDeviceClassID
                        The AudioClassID that identifies the AudioAggregateDevice class.
*/
CF_ENUM(AudioClassID)
{
    kAudioAggregateDeviceClassID            = 'aagg'
};

/*!
    @defined        kAudioAggregateDeviceUIDKey
    @discussion     The key used in a CFDictionary that describes the composition of an
                    AudioAggregateDevice. The value for this key is a CFString that contains the UID
                    of the AudioAggregateDevice.
*/
#define kAudioAggregateDeviceUIDKey             "uid"

/*!
    @defined        kAudioAggregateDeviceNameKey
    @discussion     The key used in a CFDictionary that describes the composition of an
                    AudioAggregateDevice. The value for this key is a CFString that contains the
                    human readable name of the AudioAggregateDevice.
*/
#define kAudioAggregateDeviceNameKey            "name"

/*!
    @defined        kAudioAggregateDeviceSubDeviceListKey
    @discussion     The key used in a CFDictionary that describes the composition of an
                    AudioAggregateDevice. The value for this key is a CFArray of CFDictionaries that
                    describe each sub-device in the AudioAggregateDevice. The keys for this
                    CFDictionary are defined in the AudioSubDevice section.
*/
#define kAudioAggregateDeviceSubDeviceListKey   "subdevices"

/*!
    @defined        kAudioAggregateDeviceMainSubDeviceKey
    @discussion     The key used in a CFDictionary that describes the composition of an
                    AudioAggregateDevice. The value for this key is a CFString that contains the
                    UID for the sub-device that is the time source for the
                    AudioAggregateDevice.
*/
#define kAudioAggregateDeviceMainSubDeviceKey "master"

/*!
    @defined        kAudioAggregateDeviceClockDeviceKey
    @discussion     The key used in a CFDictionary that describes the composition of an
                    AudioAggregateDevice. The value for this key is a CFString that contains the
                    UID for the clock device that is the time source for the
                    AudioAggregateDevice. If the aggregate device includes both a main audio
                    device and a clock device, the clock device will control the time base.
*/
#define kAudioAggregateDeviceClockDeviceKey     "clock"

/*!
    @defined        kAudioAggregateDeviceIsPrivateKey
    @discussion     The key used in a CFDictionary that describes the composition of an
                    AudioAggregateDevice. The value for this key is a CFNumber where a value of 0
                    means that the AudioAggregateDevice is to be published to the entire system and
                    a value of 1 means that the AudioAggregateDevice is private to the process that
                    created it. Note that a private AudioAggregateDevice is not persistent across
                    launches of the process that created it. Note that if this key is not present,
                    it implies that the AudioAggregateDevice is published to the entire system.
*/
#define kAudioAggregateDeviceIsPrivateKey       "private"

/*!
    @defined        kAudioAggregateDeviceIsStackedKey
    @discussion     The key used in a CFDictionary that describes the composition of an
                    AudioAggregateDevice. The value for this key is a CFNumber where a value of 0
                    means that the sub-devices of the AudioAggregateDevice are arranged such that
                    the output streams are all fed the same data.
 */
#define kAudioAggregateDeviceIsStackedKey       "stacked"
    
//==================================================================================================
#pragma mark AudioAggregateDevice Properties

/*!
    @enum           AudioAggregateDevice Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioAggregateDevice class.
    @discussion     AudioAggregateDevice is a subclass of AudioDevice and has the same scope and
                    element structure.
    @constant       kAudioAggregateDevicePropertyFullSubDeviceList
                        A CFArray of CFStrings that contain the UIDs of all the devices, active or
                        inactive, contained in the AudioAggregateDevice. The order of the items in
                        the array is significant and is used to determine the order of the streams
                        of the AudioAggregateDevice. The caller is responsible for releasing the
                        returned CFObject.
    @constant       kAudioAggregateDevicePropertyActiveSubDeviceList
                        An array of AudioObjectIDs for all the active sub-devices in the aggregate
                        device.
    @constant       kAudioAggregateDevicePropertyComposition
                        A CFDictionary that describes the composition of the AudioAggregateDevice.
                        The keys for this CFDicitionary are defined in the AudioAggregateDevice
                        Constants section. The caller is responsible for releasing the returned
                        CFObject.
    @constant       kAudioAggregateDevicePropertyMainSubDevice
                        A CFString that contains the UID for the AudioDevice that is currently
                        serving as the time base of the aggregate device. The caller is
                        responsible for releasing the returned CFObject.
    @constant       kAudioAggregateDevicePropertyClockDevice
                        A CFString that contains the UID for the AudioClockDevice that is currently
                        serving as the time base of the aggregate device. If the aggregate
                        device includes both a main audio device and a clock device, the clock
                        device will control the time base. Setting this property will enable
                        drift correction for all subdevices in the aggregate device. The caller is
                        responsible for releasing the returned CFObject.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioAggregateDevicePropertyFullSubDeviceList      = 'grup',
    kAudioAggregateDevicePropertyActiveSubDeviceList    = 'agrp',
    kAudioAggregateDevicePropertyComposition            = 'acom',
    kAudioAggregateDevicePropertyMainSubDevice          = 'amst',
    kAudioAggregateDevicePropertyClockDevice            = 'apcd'
};

//==================================================================================================
#pragma mark -
#pragma mark AudioSubDevice Constants

/*!
    @enum           AudioSubDevice Class Constants
    @abstract       Various constants related to the AudioSubDevice class.
    @constant       kAudioSubDeviceClassID
                        The AudioClassID that identifies the AudioSubDevice class.
*/
CF_ENUM(AudioClassID)
{
    kAudioSubDeviceClassID     = 'asub'
};

/*!
    @enum           AudioSubDevice Clock Drift Compensation Methods
    @abstract       Constants that describe the range of values the property
                    kAudioSubDevicePropertyDriftCompensation. It is a continuous range from
                    kAudioSubDeviceDriftCompensationMinQuality to
                    kAudioSubDeviceDriftCompensationMaxQuality, with some commonly used settings
                    called out.
*/
CF_ENUM(UInt32)
{
    kAudioSubDeviceDriftCompensationMinQuality      = 0,
    kAudioSubDeviceDriftCompensationLowQuality      = 0x20,
    kAudioSubDeviceDriftCompensationMediumQuality   = 0x40,
    kAudioSubDeviceDriftCompensationHighQuality     = 0x60,
    kAudioSubDeviceDriftCompensationMaxQuality      = 0x7F
};

/*!
    @defined        kAudioSubDeviceUIDKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFString that contains the UID for the
                    AudioSubDevice.
*/
#define kAudioSubDeviceUIDKey   "uid"

/*!
    @defined        kAudioAggregateDeviceSubDeviceNameKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFString that contains the human readable
                    name of the AudioSubDevice.
*/
#define kAudioSubDeviceNameKey                      "name"

/*!
    @defined        kAudioSubDeviceInputChannelsKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFNumber that indicates the total number of input
                    channels for the AudioSubDevice.
*/
#define kAudioSubDeviceInputChannelsKey             "channels-in"

/*!
    @defined        kAudioSubDeviceOutputChannelsKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFNumber that indicates the total number of output
                    channels for the AudioSubDevice.
*/
#define kAudioSubDeviceOutputChannelsKey            "channels-out"

/*!
    @defined        kAudioSubDeviceExtraInputLatencyKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFNumber that indicates the total number of frames
                    of additional latency that will be added to the input side of the
                    AudioSubDevice.
*/
#define kAudioSubDeviceExtraInputLatencyKey         "latency-in"

/*!
    @defined        kAudioSubDeviceExtraOutputLatencyKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFNumber that indicates the total number of frames
                    of additional latency that will be added to the output side of the
                    AudioSubDevice.
*/
#define kAudioSubDeviceExtraOutputLatencyKey        "latency-out"

/*!
    @defined        kAudioSubDeviceDriftCompensationKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFNumber where a non-zero value indicates that drift
                    compensation is enabled for the AudioSubDevice
*/
#define kAudioSubDeviceDriftCompensationKey         "drift"

/*!
    @defined        kAudioSubDeviceDriftCompensationQualityKey
    @discussion     The key used in a CFDictionary that describes the state of an AudioSubDevice.
                    The value for this key is a CFNumber that indicates the quality of the drifty
                    compensation for the AudioSubDevice
*/
#define kAudioSubDeviceDriftCompensationQualityKey  "drift quality"

//==================================================================================================
#pragma mark AudioSubDevice Properties

/*!
    @enum           AudioSubDevice Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioSubDevice class.
    @discussion     The AudioSubDevice class is a subclass of AudioDevice class and has the same
                    scope and element structure. However, AudioSubDevice objects do not implement an
                    IO path of their own and as such do not implement any  AudioDevice properties
                    associated with the IO path. They also don't have any streams.
    @constant       kAudioSubDevicePropertyExtraLatency
                        A Float64 indicating the number of sample frames to add to or subtract from
                        the latency compensation used for this AudioSubDevice.
    @constant       kAudioSubDevicePropertyDriftCompensation
                        A UInt32 where a value of 0 indicates that no drift compensation should be
                        done for this AudioSubDevice and a value of 1 means that it should.
    @constant       kAudioSubDevicePropertyDriftCompensationQuality
                        A UInt32 that controls the trade-off between quality and CPU load in the
                        drift compensation. The range of values is from 0 to 128, where the lower
                        the number, the worse the quality but also the less CPU is used to do the
                        compensation.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioSubDevicePropertyExtraLatency             = 'xltc',
    kAudioSubDevicePropertyDriftCompensation        = 'drft',
    kAudioSubDevicePropertyDriftCompensationQuality = 'drfq'
};

//==================================================================================================

#if defined(__cplusplus)
}
#endif

CF_ASSUME_NONNULL_END

//==================================================================================================

#include <CoreAudio/AudioHardwareDeprecated.h>

#endif  //  CoreAudio_AudioHardware_h
