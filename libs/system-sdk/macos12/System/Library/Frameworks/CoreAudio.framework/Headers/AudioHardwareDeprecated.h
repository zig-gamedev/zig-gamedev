/*==================================================================================================
     File:       CoreAudio/AudioHardwareDeprecated.h

     Copyright:  (c) 1985-2011 by Apple, Inc., all rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

==================================================================================================*/
#if !defined(CoreAudio_AudioHardwareDeprecated_h)
#define CoreAudio_AudioHardwareDeprecated_h

//==================================================================================================
//  Includes

#include <Availability.h>
#include <CoreAudioTypes/CoreAudioTypes.h>
#include <CoreFoundation/CoreFoundation.h>

//==================================================================================================

CF_ASSUME_NONNULL_BEGIN

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
#pragma mark -
#pragma mark Property Support Constants

/*!
    @enum           Property Address Constants
    @abstract       The valid values for the scope in a property address.
    @constant       kAudioDevicePropertyScopeInput
                        The AudioObjectPropertyScope for properties that apply to the input side of
                        an object.
    @constant       kAudioDevicePropertyScopeOutput
                        The AudioObjectPropertyScope for properties that apply to the output side of
                        an object.
    @constant       kAudioDevicePropertyScopePlayThrough
                        The AudioObjectPropertyScope for properties that apply to the play through
                        side of an object.
*/
CF_ENUM(AudioObjectPropertyScope)
{
    kAudioDevicePropertyScopeInput          = kAudioObjectPropertyScopeInput,
    kAudioDevicePropertyScopeOutput         = kAudioObjectPropertyScopeOutput,
    kAudioDevicePropertyScopePlayThrough    = kAudioObjectPropertyScopePlayThrough
};

/*!
    @enum           Property Wildcard Constants
    @abstract       Constants that are used as wildcards in an AudioObjectPropertyAddress.
    @discussion     Wildcards match any and all values for there associated type. They are
                    especially useful for registering listener procs to receive notifications and
                    for querying an AudioObject's list of AudioControls.
    @constant       kAudioPropertyWildcardPropertyID
                        A synonym for kAudioObjectPropertySelectorWildcard.
    @constant       kAudioPropertyWildcardSection
                        The wildcard value for the isInput argument of AudioDeviceGetPropertyInfo(),
                        AudioDeviceGetProperty(), and AudioDeviceSetProperty().
    @constant       kAudioPropertyWildcardChannel
                        A synonym for kAudioObjectPropertyElementWildcard.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioPropertyWildcardPropertyID        = kAudioObjectPropertySelectorWildcard
};

CF_ENUM(unsigned char)
{
    kAudioPropertyWildcardSection           = 0xFF
};

CF_ENUM(AudioObjectPropertyElement)
{
    kAudioPropertyWildcardChannel           = kAudioObjectPropertyElementWildcard
};

//==================================================================================================
#pragma mark -
#pragma mark AudioControl Constants

/*!
    @enum           AudioBooleanControl Subclass IDs
    @abstract       The four char codes that identify the various standard subclasses of
                    AudioBooleanControl.
    @constant       kAudioISubOwnerClassID
                        An AudioBooleanControl where true means that the AudioDevice that
                        ultimately owns the control also owns any iSub attached to the CPU.
*/
CF_ENUM(AudioClassID)
{
    kAudioISubOwnerControlClassID       = 'atch'
};

//==================================================================================================
#pragma mark AudioControl Properties

/*!
    @enum           AudioLevelControl Properties
    @abstract       AudioObjectPropertySelector values that apply to all AudioLevelControls.
    @discussion     AudioLevelControl is a subclass of AudioControl and has only the single scope,
                    kAudioObjectPropertyScopeGlobal, and only a main element.
    @constant       kAudioLevelControlPropertyDecibelsToScalarTransferFunction
                        A UInt32 whose value indicates the transfer function the HAL uses to convert
                        between decibel values and scalar values.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioLevelControlPropertyDecibelsToScalarTransferFunction  = 'lctf'
};

/*!
    @enum           Values for kAudioLevelControlPropertyDecibelsToScalarTransferFunction
    @abstract       The following constants are the only supported values for a volume control's
                    transfer function.
    @discussion     The transfer function implemented in the volume control works by raising the
                    scalar value to an exponent to map it into the decibel range. The constants
                    in this enum express the exponent used in the name as a quotient. For example,
                    kAudioLevelControlTranferFunction3Over4 represents the exponent 0.75.
*/
typedef CF_ENUM(UInt32, AudioLevelControlTransferFunction)
{
    kAudioLevelControlTranferFunctionLinear     = 0,
    kAudioLevelControlTranferFunction1Over3     = 1,
    kAudioLevelControlTranferFunction1Over2     = 2,
    kAudioLevelControlTranferFunction3Over4     = 3,
    kAudioLevelControlTranferFunction3Over2     = 4,
    kAudioLevelControlTranferFunction2Over1     = 5,
    kAudioLevelControlTranferFunction3Over1     = 6,
    kAudioLevelControlTranferFunction4Over1     = 7,
    kAudioLevelControlTranferFunction5Over1     = 8,
    kAudioLevelControlTranferFunction6Over1     = 9,
    kAudioLevelControlTranferFunction7Over1     = 10,
    kAudioLevelControlTranferFunction8Over1     = 11,
    kAudioLevelControlTranferFunction9Over1     = 12,
    kAudioLevelControlTranferFunction10Over1    = 13,
    kAudioLevelControlTranferFunction11Over1    = 14,
    kAudioLevelControlTranferFunction12Over1    = 15
};

//==================================================================================================
#pragma mark -
#pragma mark AudioSystemObject Types

/*!
    @typedef        AudioHardwarePropertyID
    @abstract       An AudioHardwarePropertyID is a integer that identifies a specific piece of
                    information about the AudioSystemObject.
*/
typedef AudioObjectPropertySelector AudioHardwarePropertyID;

/*!
    @typedef        AudioHardwarePropertyListenerProc
    @abstract       Clients register an AudioHardwarePropertyListenerProc with the AudioSystemObject
                    in order to receive notifications when the properties of the object change.
    @discussion     Note that the same functionality is provided by AudioObjectPropertyListenerProc.
    @param          inPropertyID
                        The AudioHardwarePropertyID of the property that changed.
    @param          inClientData
                        A pointer to client data established when the listener proc was registered
                        with the AudioSystemObject.
    @result         The return value is currently unused and should always be 0.
*/
typedef OSStatus
(*AudioHardwarePropertyListenerProc)(   AudioHardwarePropertyID inPropertyID,
                                        void* __nullable        inClientData);

//==================================================================================================
#pragma mark AudioSystemObject Constants

/*!
    @defined        kAudioHardwareRunLoopMode
    @discussion     The name of the run loop mode to which only HAL run loop sources and sources
                    added via AudioHardwareAddRunLoopSource() belong. This is the mode in which to
                    task a run loop in order to ensure that just HAL related events are handled.
*/
#define kAudioHardwareRunLoopMode   "com.apple.audio.CoreAudio"

//==================================================================================================
#pragma mark AudioSystemObject Properties

/*!
    @enum           AudioSystemObject Properties
    @abstract       AudioObjectPropertySelector values that apply to the AudioSystemObject.
    @discussion     The AudioSystemObject has one scope, kAudioObjectPropertyScopeGlobal, and only a
                    main element.
    @constant       kAudioHardwarePropertyRunLoop
                        The CFRunLoopRef the HAL is currently attaching all of its system
                        notification handlers to. In 10.6 and later, the HAL will use the process's
                        run loop (as defined by CFRunLoopGetMain()) for this task. Whereas in
                        previous releases, the HAL created and managed its own thread for the task.
                        Clients can set this property to tell the HAL to use a thread of the
                        client's choosing. If the value for this property is set to NULL, the HAL
                        will return to its pre-10.6 behavior of creating and managing its own
                        thread for notifications. The caller is responsible for releasing the
                        returned CFObject.
    @constant       kAudioHardwarePropertyDeviceForUID
                        Using an AudioValueTranslation structure, this property translates the input
                        CFStringRef containing a UID into the AudioDeviceID that refers to the
                        AudioDevice with that UID. This property will return kAudioDeviceUnknown if
                        the given UID does not match any currently available AudioDevice.
    @constant       kAudioHardwarePropertyPlugInForBundleID
                        Using an AudioValueTranslation structure, this property translates the input
                        CFString containing a bundle ID into the AudioObjectID of the AudioPlugIn
                        that corresponds to it. This property will return kAudioObjectUnkown if the
                        given bundle ID doesn't match any AudioPlugIns.
    @constant       kAudioHardwarePropertyProcessIsMaster
						The deprecated synonym for kAudioHardwarePropertyProcessIsMain
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioHardwarePropertyRunLoop                           = 'rnlp',
    kAudioHardwarePropertyDeviceForUID                      = 'duid',
    kAudioHardwarePropertyPlugInForBundleID                 = 'pibi',
    kAudioHardwarePropertyProcessIsMaster API_DEPRECATED_WITH_REPLACEMENT("kAudioHardwarePropertyProcessIsMain", macos(10.0, 12.0), ios(2.0, 15.0), watchos(1.0, 8.0), tvos(9.0, 15.0)) = 'mast'
};

/*!
    @enum           AudioSystemObject Properties Implemented via AudioControl objects
    @abstract       AudioObjectPropertySelector values for AudioSystemObject properties that are
                    implemented by AudioControl objects.
    @discussion     These properties are also accessible by locating the AudioControl object
                    attached to the AudioSystemObject and using that object to access the properties
                    of the control.
    @constant       kAudioHardwarePropertyBootChimeVolumeScalar
                        A Float32 that represents the value of the boot chime volume control. The
                        range is between 0.0 and 1.0 (inclusive). This property is implemented by an
                        AudioControl object that is a subclass of AudioBootChimeVolumeControl.
    @constant       kAudioHardwarePropertyBootChimeVolumeDecibels
                        A Float32 that represents the value of the boot chime volume control in dB.
                        This property is implemented by an AudioControl object that is a subclass
                        of AudioBootChimeVolumeControl.
    @constant       kAudioHardwarePropertyBootChimeVolumeRangeDecibels
                        An AudioValueRange that contains the minimum and maximum dB values the
                        boot chime control can have. This property is implemented by an AudioControl
                        object that is a subclass of AudioBootChimeVolumeControl.
    @constant       kAudioHardwarePropertyBootChimeVolumeScalarToDecibels
                        A Float32 that on input contains a scalar volume value for the boot chime
                        and on exit contains the equivalent dB value. This property is implemented
                        by an AudioControl object that is a subclass of AudioBootChimeVolumeControl.
    @constant       kAudioHardwarePropertyBootChimeVolumeDecibelsToScalar
                        A Float32 that on input contains a dB volume value for the boot chime and on
                        exit contains the equivalent scalar value. This property is implemented by
                        an AudioControl object that is a subclass of AudioBootChimeVolumeControl.
    @constant       kAudioHardwarePropertyBootChimeVolumeDecibelsToScalarTransferFunction
                        A UInt32 whose value indicates the transfer function the HAL uses to convert
                        between decibel values and scalar values. This property is implemented by an
                        AudioControl object that is a subclass of AudioBootChimeVolumeControl.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioHardwarePropertyBootChimeVolumeScalar                             = 'bbvs',
    kAudioHardwarePropertyBootChimeVolumeDecibels                           = 'bbvd',
    kAudioHardwarePropertyBootChimeVolumeRangeDecibels                      = 'bbd#',
    kAudioHardwarePropertyBootChimeVolumeScalarToDecibels                   = 'bv2d',
    kAudioHardwarePropertyBootChimeVolumeDecibelsToScalar                   = 'bd2v',
    kAudioHardwarePropertyBootChimeVolumeDecibelsToScalarTransferFunction   = 'bvtf'
};

//==================================================================================================
#pragma mark AudioSystemObject Functions

/*!
    @functiongroup  AudioSystemObject
*/

/*!
    @function       AudioHardwareAddRunLoopSource
    @abstract       Add the given CFRunLoopSource to the the HAL's notification CFRunLoop.
    @discussion     The CFRunLoop the HAL uses for notifications is specified by
                    kAudioHardwarePropertyRunLoop. If kAudioHardwarePropertyRunLoop changes,
                    CFRunLoopSources added with this function will automatically be transferred to
                    the new CFRunLoop.
                    Usage of the HAL's notification run loop is deprecated. Please use libdispatch
                    instead.
    @param          inRunLoopSource
                        The CFRunLoopSource to add.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareAddRunLoopSource(CFRunLoopSourceRef    inRunLoopSource)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_3, __MAC_10_7, __IPHONE_2_0, __IPHONE_4_1);

/*!
    @function       AudioHardwareRemoveRunLoopSource
    @abstract       Remove the given CFRunLoopSource from the the HAL's notification CFRunLoop.
    @discussion     The CFRunLoop the HAL uses for notifications is specified by
                    kAudioHardwarePropertyRunLoop.
                    Usage of the HAL's notification run loop is deprecated. Please use libdispatch
                    instead.
    @param          inRunLoopSource
                        The CFRunLoopSource to remove.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareRemoveRunLoopSource(CFRunLoopSourceRef inRunLoopSource)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_3, __MAC_10_7, __IPHONE_2_0, __IPHONE_4_1);

/*!
    @function       AudioHardwareGetPropertyInfo
    @abstract       Retrieve information about the given property.
    @discussion     Note that the same functionality is provided by the functions
                    AudioObjectHasProperty(), AudioObjectIsPropertySettable(), and
                    AudioObjectGetPropertyDataSize().
    @param          inPropertyID
                        The AudioHardwarePropertyID of the property to query.
    @param          outSize
                        A pointer to a UInt32 that receives the size of the property data in bytes
                        on exit. This can be NULL if the size information is not being requested.
    @param          outWritable
                        A pointer to a Boolean that receives indication of whether or not the given
                        property can be set. This can be NULL if the writability is not being
                        requested.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareGetPropertyInfo(   AudioHardwarePropertyID inPropertyID,
                                UInt32* __nullable      outSize,
                                Boolean* __nullable     outWritable)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioHardwareGetProperty
    @abstract       Queries an the AudioSystemObject to get the data of the given property and
                    places it in the provided buffer.
    @discussion     Note that the same functionality is provided by the function
                    AudioObjectGetPropertyData().
    @param          inPropertyID
                        The AudioHardwarePropertyID of the property to query.
    @param          ioPropertyDataSize
                        A UInt32 which on entry indicates the size of the buffer pointed to by
                        outData and on exit indicates how much of the buffer was used.
    @param          outPropertyData
                        The buffer into which the AudioSystemObject will put the data for the given
                        property.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareGetProperty(   AudioHardwarePropertyID inPropertyID,
                            UInt32*                 ioPropertyDataSize,
                            void*                   outPropertyData)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioHardwareSetProperty
    @abstract       Tells the AudioSystemObject to change the value of the given property using the
                    provided data.
    @discussion     Note that the value of the property should not be considered changed until the
                    HAL has called the listeners as many properties values are changed
                    asynchronously. Also note that the same functionality is provided by the
                    function AudioObjectGetPropertyData().
    @param          inPropertyID
                        The AudioHardwarePropertyID of the property to change.
    @param          inPropertyDataSize
                        A UInt32 indicating the size of the buffer pointed to by inData.
    @param          inPropertyData
                        The buffer containing the data to be used to change the property's value.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareSetProperty(   AudioHardwarePropertyID inPropertyID,
                            UInt32                  inPropertyDataSize,
                            const void*             inPropertyData)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioHardwareAddPropertyListener
    @abstract       Registers the given AudioHardwarePropertyListenerProc to receive notifications
                    when the given property changes.
    @discussion     Note that the same functionality is provided by AudioObjectAddPropertyListener
                    in conjunction with AudioObjectPropertyListenerProc.
    @param          inPropertyID
                        The AudioHardwarePropertyID of the property to listen to.
    @param          inProc
                        AudioHardwarePropertyListenerProc to call.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareAddPropertyListener(   AudioHardwarePropertyID             inPropertyID,
                                    AudioHardwarePropertyListenerProc   inProc,
                                    void* __nullable                    inClientData)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioHardwareRemovePropertyListener
    @abstract       Unregisters the given AudioHardwarePropertyListenerProc from receive
                    notifications when the given property changes.
    @discussion     Note that the same functionality is provided by
                    AudioObjectRemovePropertyListener in conjunction with
                    AudioObjectPropertyListenerProc.
    @param          inPropertyID
                        The AudioHardwarePropertyID of the property to stop listening to.
    @param          inProc
                        AudioHardwarePropertyListenerProc to unregister.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareRemovePropertyListener(    AudioHardwarePropertyID             inPropertyID,
                                        AudioHardwarePropertyListenerProc   inProc)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

//==================================================================================================
#pragma mark -
#pragma mark AudioDevice Types

/*!
    @typedef        AudioDeviceID
    @abstract       AudioDevice is the base class for all objects that represent an audio device.
    @discussion     AudioDevice is a subclass of AudioObject. AudioDevices normally contain
                    AudioStreams and AudioControls, but may contain other things depending on the
                    kind of AudioDevice (e.g. aggregate devices contain other AudioDevices).
*/
typedef AudioObjectID   AudioDeviceID;

/*!
    @typedef        AudioDevicePropertyID
    @abstract       An AudioDevicePropertyID is an integer that identifies a specific piece of
                    information about the object.
*/
typedef AudioObjectPropertySelector AudioDevicePropertyID;

/*!
    @typedef        AudioDevicePropertyListenerProc
    @abstract       Clients register an AudioDevicePropertyListenerProc with the AudioDevice object
                    in order to receive notifications when the properties of the object change.
    @discussion     Note that the same functionality is provided by AudioObjectPropertyListenerProc.
    @param          inDevice
                        The AudioDevice whose property has changed.
    @param          inChannel
                        The channel of the property that changed where 0 is the main channel.
    @param          isInput
                        Which section of the AudioDevice changed.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property that changed.
    @param          inClientData
                        A pointer to client data established when the listener proc was registered
                        with the object.
    @result         The return value is currently unused and should always be 0.
*/
typedef OSStatus
(*AudioDevicePropertyListenerProc)( AudioDeviceID           inDevice,
                                    UInt32                  inChannel,
                                    Boolean                 isInput,
                                    AudioDevicePropertyID   inPropertyID,
                                    void* __nullable        inClientData);

//==================================================================================================
#pragma mark AudioDevice Constants

/*!
    @enum           AudioDevice Class Constants
    @abstract       Various constants related to AudioDevices.
    @constant       kAudioDeviceUnknown
                        The AudioObjectID for a nonexistent AudioObject.
*/
CF_ENUM(AudioObjectID)
{
    kAudioDeviceUnknown                     = kAudioObjectUnknown
};

/*!
    @enum           Transport Type IDs
    @abstract       Commonly used values for kAudioDevicePropertyTransportType and
                    kAudioTransportManagerPropertyTransportType
    @constant       kAudioDeviceTransportTypeAutoAggregate
                        The transport type ID for automatically generated aggregate devices.
*/
CF_ENUM(UInt32)
{
    kAudioDeviceTransportTypeAutoAggregate  = 'fgrp'
};

//==================================================================================================
#pragma mark AudioDevice Properties

/*!
    @enum           AudioDevice Properties Implemented via AudioControl objects
    @abstract       AudioObjectPropertySelector values for AudioDevice properties that are
                    implemented by AudioControl objects.
    @discussion     These properties are also accessible by locating the AudioControl object
                    attached to the AudioDevice and using that object to access the properties of
                    the control.
    @constant       kAudioDevicePropertyVolumeDecibelsToScalarTransferFunction
                        A UInt32 whose value indicates the transfer function the HAL uses to convert
                        between decibel values and scalar values. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl.
    @constant       kAudioDevicePropertyPlayThruVolumeDecibelsToScalarTransferFunction
                        A UInt32 whose value indicates the transfer function the HAL uses to convert
                        between decibel values and scalar values. This property is implemented by an
                        AudioControl object that is a subclass of AudioVolumeControl. Further, the
                        control that implements this property is only available through
                        kAudioDevicePropertyScopePlayThrough.
    @constant       kAudioDevicePropertyDriverShouldOwniSub
                        A UInt32 where a value of 0 means that the AudioDevice should not claim
                        ownership of any attached iSub and a value of 1 means that it should. Note
                        that this property is only available for built-in devices and for USB Audio
                        devices that use the standard class compliant driver. This property is
                        implemented by an AudioControl object that is a subclass of
                        AudioISubOwnerControl.
    @constant       kAudioDevicePropertySubVolumeDecibelsToScalarTransferFunction
                        A UInt32 whose value indicates the transfer function the HAL uses to convert
                        between decibel values and scalar values. This property is implemented by an
                        AudioControl object that is a subclass of AudioLFEVolumeControl.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioDevicePropertyVolumeDecibelsToScalarTransferFunction          = 'vctf',
    kAudioDevicePropertyPlayThruVolumeDecibelsToScalarTransferFunction  = 'mvtf',
    kAudioDevicePropertyDriverShouldOwniSub                             = 'isub',
    kAudioDevicePropertySubVolumeDecibelsToScalarTransferFunction       = 'svtf'
};

/*!
    @enum           AudioDevice Properties That Ought To Some Day Be Deprecated
    @abstract       AudioObjectPropertySelector values whose functionality is better provided by
                    other selectors.
    @discussion     These selectors are still provided for backward compatibility. The description
                    of the property will indicate in parentheses the better selectors to use and
                    why.
    @constant       kAudioDevicePropertyDeviceName
                        A C-string that contains the human readable name of the AudioDevice.
                        (kAudioObjectPropertyName: CFStrings are better for localization.)
    @constant       kAudioDevicePropertyDeviceNameCFString
                        A CFStringRef that contains the human readable name of the AudioDevice. The
                        caller is responsible for releasing the returned CFObject.
                        (kAudioObjectPropertyName: This is just another name for the inherited
                        selector.)
    @constant       kAudioDevicePropertyDeviceManufacturer
                        A C-string that contains the human readable name of the manufacturer of the
                        AudioDevice.
                        (kAudioObjectPropertyManufacturer: CFStrings are better for localization.)
    @constant       kAudioDevicePropertyDeviceManufacturerCFString
                        A CFString that contains the human readable name of the manufacturer of the
                        AudioDevice. The caller is responsible for releasing the returned CFObject.
                        (kAudioObjectPropertyManufacturer: This is just another name for the
                        inherited selector.)
    @constant       kAudioDevicePropertyRegisterBufferList
                        This property allows clients to register a fully populated AudioBufferList
                        that matches the topology described by
                        kAudioDevicePropertyStreamConfiguration for doing input using
                        AudioDeviceRead(). The AudioBufferList will be registered with the call the
                        AudioDeviceSetProperty() and will be unregistered with the call to
                        AudioDeviceGetProperty(). If this property isn't implemented by the
                        AudioDevice, it implies that the AudioDevice also doesn't support
                        AudioDeviceRead().
                        (Aggregate devices make AudioDeviceRead() obsolete for the most part.)
    @constant       kAudioDevicePropertyBufferSize
                        A UInt32 containing the size in bytes of the IO buffer for the AudioStream
                        containing the element.
                        (kAudioDevicePropertyBufferFrameSize: with multiple AudioStreams and the
                        requirement that all streams' buffers represent the same amount of time, it
                        doesn't make sense to set the buffer size in bytes since it will be
                        different for each stream.)
    @constant       kAudioDevicePropertyBufferSizeRange
                        An AudioValueRange specifying the minimum and maximum bytes size for the
                        IO buffer for the AudioStream containing the given element.
                        (kAudioDevicePropertyBufferFrameSizeRange: see
                        kAudioDevicePropertyBufferSize.)
    @constant       kAudioDevicePropertyChannelName
                        A C-string that contains a human readable name for the given element in the
                        given scope. The caller is responsible for releasing the returned CFObject.
                        (kAudioObjectPropertyElementName: CFStrings are better for
                        localization.)
    @constant       kAudioDevicePropertyChannelNameCFString
                        A CFString that contains a human readable name for the given element in the
                        given scope. The caller is responsible for releasing the returned CFObject.
                        (kAudioObjectPropertyElementName: This is just another name for the
                        inherited selector.)
    @constant       kAudioDevicePropertyChannelCategoryName
                        A C-string that contains a human readable name for the category of the given
                        element in the given scope. The caller is responsible for releasing the
                        returned CFObject.
                        (kAudioObjectPropertyElementCategoryName: CFStrings are better for
                        localization.)
    @constant       kAudioDevicePropertyChannelCategoryNameCFString
                        A CFString that contains a human readable name for the category of the given
                        element in the given scope. The caller is responsible for releasing the
                        returned CFObject.
                        (kAudioObjectPropertyElementCategoryName: This is just another name for the
                        inherited selector.)
    @constant       kAudioDevicePropertyChannelNumberName
                        A C-string that contains a human readable name for the number of the given
                        element in the given scope. The caller is responsible for releasing the
                        returned CFObject.
                        (kAudioObjectPropertyElementNumberName: CFStrings are better for
                        localization.)
    @constant       kAudioDevicePropertyChannelNumberNameCFString
                        A CFString that contains a human readable name for the number of the given
                        element in the given scope. The caller is responsible for releasing the
                        returned CFObject.
                        (kAudioObjectPropertyElementNumberName: This is just another name for the
                        inherited selector.)
    @constant       kAudioDevicePropertySupportsMixing
                        A UInt32 where a value of 1 means the AudioDevice supports mixing and a
                        value of 0 means that it doesn't and that all IO is performed in each
                        AudioStream's current physical format. This property is changed indirectly
                        by changing to a format that doesn't support mixing, such as AC-3. (The HAL
                        now vends its format information with a flag indicating the mixability in
                        order to better support devices with streams that are both mixable and non-
                        mixable.)
    @constant       kAudioDevicePropertyStreamFormat
                        An AudioStreamBasicDescription that describes the current data format for
                        the AudioStream that contains the channel referred to by the element number.
                        (kAudioStreamPropertyVirtualFormat: Managing format information is
                        inherently an operation on AudioStreams, rather than AudioDevices. It is
                        confusing for the client to work with formats at the AudioDevice level and
                        has been shown to lead to programming mistakes by clients when working with
                        devices that have multiple streams.)
    @constant       kAudioDevicePropertyStreamFormats
                        An array of AudioStreamBasicDescriptions that describe the available data
                        formats for the AudioStream that contains the channel referred to by the
                        element number.
                        (kAudioStreamPropertyAvailableVirtualFormats: Managing format information is
                        inherently an operation on AudioStreams, rather than AudioDevices. It is
                        confusing for the client to work with formats at the AudioDevice level and
                        has been shown to lead to programming mistakes by clients when working with
                        devices that have multiple streams.)
    @constant       kAudioDevicePropertyStreamFormatSupported
                        An AudioStreamBasicDescription is passed in to query whether or not the
                        format is supported. A kAudioDeviceUnsupportedFormatError will be returned
                        if the format is not supported and kAudioHardwareNoError will be returned if
                        it is supported. AudioStreamBasicDescription fields set to 0 will be ignored
                        in the query, but otherwise values must match exactly.
                        (kAudioStreamPropertyAvailableVirtualFormats: The proper and most robust way
                        to find a format that the AudioStream can support is to get the list of
                        available formats and look through that rather than using this property.)
    @constant       kAudioDevicePropertyStreamFormatMatch
                        An AudioStreamBasicDescription is passed in and the AudioStream will modify
                        it to describe the best match, in the AudioDevice's opinion, for the given
                        format.
                        (kAudioStreamPropertyAvailableVirtualFormats: The proper and most robust way
                        to find a format that the AudioStream can support is to get the list of
                        available formats and look through that rather than using this property.)
    @constant       kAudioDevicePropertyDataSourceNameForID
                        This property translates the given data source item ID into a human readable
                        name using an AudioValueTranslation structure. The input data is the UInt32
                        holding the item ID to be translated and the output data is a buffer to hold
                        the name as a null terminated C-string.
                        (kAudioDevicePropertyDataSourceNameForIDCFString: CFStrings are better for
                        localization.)
    @constant       kAudioDevicePropertyClockSourceNameForID
                        This property translates the given clock source item ID into a human
                        readable name using an AudioValueTranslation structure. The input data is
                        the UInt32 holding the item ID to be translated and the output data is a
                        buffer to hold the name as a null terminated C-string.
                        (kAudioDevicePropertyClockSourceNameForIDCFString: CFStrings are better for
                        localization.)
    @constant       kAudioDevicePropertyPlayThruDestinationNameForID
                        This property translates the given play through destination item ID into a
                        human readable name using an AudioValueTranslation structure. The input data
                        is the UInt32 holding the item ID to be translated and the output data is a
                        buffer to hold the name as a null terminated C-string.
                        (kAudioDevicePropertyPlayThruDestinationNameForIDCFString: CFStrings are
                        better for localization.)
    @constant       kAudioDevicePropertyChannelNominalLineLevelNameForID
                        This property translates the given nominal line level item ID into a human
                        readable name using an AudioValueTranslation structure. The input data is
                        the UInt32 holding the item ID to be translated and the output data is a
                        buffer to hold the name as a null terminated C-string.
                        (kAudioDevicePropertyChannelNominalLineLevelNameForIDCFString: CFStrings are
                        better for localization.)
    @constant       kAudioDevicePropertyHighPassFilterSettingNameForID
                        This property translates the given high pass filter setting item ID into a
                        human readable name using an AudioValueTranslation structure. The input data
                        is the UInt32 holding the item ID to be translated and the output data is a
                        buffer to hold the name as a null terminated C-string.
                        (kAudioDevicePropertyHighPassFilterSettingNameForIDCFString: CFStrings are
                        better for localization.)
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioDevicePropertyDeviceName                          = 'name',
    kAudioDevicePropertyDeviceNameCFString                  = kAudioObjectPropertyName,
    kAudioDevicePropertyDeviceManufacturer                  = 'makr',
    kAudioDevicePropertyDeviceManufacturerCFString          = kAudioObjectPropertyManufacturer,
    kAudioDevicePropertyRegisterBufferList                  = 'rbuf',
    kAudioDevicePropertyBufferSize                          = 'bsiz',
    kAudioDevicePropertyBufferSizeRange                     = 'bsz#',
    kAudioDevicePropertyChannelName                         = 'chnm',
    kAudioDevicePropertyChannelNameCFString                 = kAudioObjectPropertyElementName,
    kAudioDevicePropertyChannelCategoryName                 = 'ccnm',
    kAudioDevicePropertyChannelCategoryNameCFString         = kAudioObjectPropertyElementCategoryName,
    kAudioDevicePropertyChannelNumberName                   = 'cnnm',
    kAudioDevicePropertyChannelNumberNameCFString           = kAudioObjectPropertyElementNumberName,
    kAudioDevicePropertySupportsMixing                      = 'mix?',
    kAudioDevicePropertyStreamFormat                        = 'sfmt',
    kAudioDevicePropertyStreamFormats                       = 'sfm#',
    kAudioDevicePropertyStreamFormatSupported               = 'sfm?',
    kAudioDevicePropertyStreamFormatMatch                   = 'sfmm',
    kAudioDevicePropertyDataSourceNameForID                 = 'sscn',
    kAudioDevicePropertyClockSourceNameForID                = 'cscn',
    kAudioDevicePropertyPlayThruDestinationNameForID        = 'mddn',
    kAudioDevicePropertyChannelNominalLineLevelNameForID    = 'cnlv',
    kAudioDevicePropertyHighPassFilterSettingNameForID      = 'chip'
};

//==================================================================================================
#pragma mark AudioDevice Functions

/*!
    @functiongroup  AudioDevice
*/

/*!
    @function       AudioDeviceAddIOProc
    @abstract       Registers the given AudioDeviceIOProc with the AudioDevice.
    @discussion     A client may have multiple IOProcs for a given device, but the device is free to
                    only accept as many as it can handle. Note that it is not recommended for
                    clients to have more than a single IOProc registered at a time as this can be
                    wasteful of system resources. Rather, it is recommended that the client do any
                    necessary mixing itself so that only one IOProc is necessary.
                    This routine has been deprecated in favor of AudioDeviceCreateIOProcID().
    @param          inDevice
                        The AudioDevice to register the IOProc with.
    @param          inProc
                        The AudioDeviceIOProc to register.
    @param          inClientData
                        A pointer to client data that is passed back to the IOProc when it is
                        called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceAddIOProc(   AudioDeviceID       inDevice,
                        AudioDeviceIOProc   inProc,
                        void* __nullable    inClientData)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceRemoveIOProc
    @abstract       Unregisters the given AudioDeviceIOProc from the AudioDevice.
                    This routine has been deprecated in favor of AudioDeviceDestroyIOProcID().
    @param          inDevice
                        The AudioDevice to unregister the IOProc from.
    @param          inProc
                        The AudioDeviceIOProc to unregister.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceRemoveIOProc(    AudioDeviceID       inDevice,
                            AudioDeviceIOProc   inProc)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceRead
    @abstract       Read some data from an AudioDevice starting at the given time.
    @discussion     With the advent of aggregate devices, the need for AudioDeviceRead has gone
                    away. Consequently, this function is now deprecated.
    @param          inDevice
                        The AudioDevice to read from.
    @param          inStartTime
                        An AudioTimeStamp indicating the time from which to read the data. In
                        general, the valid range of time (in frames) is from the current time minus
                        the maximum IO buffer size to the current time minus the safety offset.
    @param          outData
                        An AudioBufferList that must be the same size and shape as that returned by
                        kAudioDevicePropertyStreamConfiguration. Further, the AudioBufferList must
                        have been previously registered with the device via
                        kAudioDevicePropertyRegisterBufferList. On exit, the mDataSize fields will
                        be updated with the amount of data read.
    @result         An OSStatus indicating success or failure.
                    kAudioHardwareUnsupportedOperationError will be returned if the AudioDevice does
                    not support direct reading.
*/
extern OSStatus
AudioDeviceRead(    AudioDeviceID           inDevice,
                    const AudioTimeStamp*   inStartTime,
                    AudioBufferList*        outData)                                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_5, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceGetPropertyInfo
    @abstract       Retrieve information about the given property of an AudioDevice.
    @discussion     Note that the same functionality is provided by the functions
                    AudioObjectHasProperty(), AudioObjectIsPropertySettable(), and
                    AudioObjectGetPropertyDataSize().
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
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceGetPropertyInfo( AudioDeviceID           inDevice,
                            UInt32                  inChannel,
                            Boolean                 isInput,
                            AudioDevicePropertyID   inPropertyID,
                            UInt32* __nullable      outSize,
                            Boolean* __nullable     outWritable)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceGetProperty
    @abstract       Queries an the AudioDevice object to get the data of the given property and
                    places it in the provided buffer.
    @discussion     Note that the same functionality is provided by the function
                    AudioObjectGetPropertyData().
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
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceGetProperty( AudioDeviceID           inDevice,
                        UInt32                  inChannel,
                        Boolean                 isInput,
                        AudioDevicePropertyID   inPropertyID,
                        UInt32*                 ioPropertyDataSize,
                        void*                   outPropertyData)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceSetProperty
    @abstract       Tells the AudioDevice object to change the value of the given property using the
                    provided data.
    @discussion     Note that the value of the property should not be considered changed until the
                    HAL has called the listeners as many properties values are changed
                    asynchronously. Also note that the same functionality is provided by the
                    function AudioObjectSetPropertyData().
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
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceSetProperty( AudioDeviceID                       inDevice,
                        const AudioTimeStamp* __nullable    inWhen,
                        UInt32                              inChannel,
                        Boolean                             isInput,
                        AudioDevicePropertyID               inPropertyID,
                        UInt32                              inPropertyDataSize,
                        const void*                         inPropertyData)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceAddPropertyListener
    @abstract       Registers the given AudioDevicePropertyListenerProc to receive notifications
                    when the given property changes.
    @discussion     Note that the same functionality is provided by AudioObjectAddPropertyListener
                    in conjunction with AudioObjectPropertyListenerProc.
    @param          inDevice
                        The AudioDevice with whom to register the listener.
    @param          inChannel
                        The channel of the property to listen to.
    @param          isInput
                        Which section of the AudioDevice to listen to.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to listen to.
    @param          inProc
                        AudioDevicePropertyListenerProc to call.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceAddPropertyListener( AudioDeviceID                   inDevice,
                                UInt32                          inChannel,
                                Boolean                         isInput,
                                AudioDevicePropertyID           inPropertyID,
                                AudioDevicePropertyListenerProc inProc,
                                void* __nullable                inClientData)                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioDeviceRemovePropertyListener
    @abstract       Unregisters the given AudioDevicePropertyListenerProc from receiving
                    notifications when the given property changes.
    @discussion     Note that the same functionality is provided by
                    AudioObjectRemovePropertyListener in conjunction with
                    AudioObjectPropertyListenerProc.
    @param          inDevice
                        The AudioDevice with whom to unregister the listener.
    @param          inChannel
                        The channel of the property to unregister from.
    @param          isInput
                        Which section of the AudioDevice to unregister from.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to stop listening to.
    @param          inProc
                        AudioDevicePropertyListenerProc to unregister.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioDeviceRemovePropertyListener(  AudioDeviceID                   inDevice,
                                    UInt32                          inChannel,
                                    Boolean                         isInput,
                                    AudioDevicePropertyID           inPropertyID,
                                    AudioDevicePropertyListenerProc inProc)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

//==================================================================================================
#pragma mark -
#pragma mark AudioAggregateDevice Constants
/*!
    @defined        kAudioAggregateDeviceMasterSubDeviceKey
    @discussion     The deprecated synonym for kAudioAggregateDeviceMainSubDeviceKey
*/
#define kAudioAggregateDeviceMasterSubDeviceKey "master"

//==================================================================================================
#pragma mark AudioAggregateDevice Properties

/*!
    @enum           AudioAggregateDevice Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioAggregateDevice class.
    @discussion     AudioAggregateDevice is a subclass of AudioDevice and has the same scope and
                    element structure.
    @constant       kAudioAggregateDevicePropertyMasterSubDevice
                        The deprecated synonym for kAudioAggregateDevicePropertyMainSubDevice
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioAggregateDevicePropertyMasterSubDevice API_DEPRECATED_WITH_REPLACEMENT("kAudioAggregateDevicePropertyMainSubDevice", macos(10.0, 12.0), ios(2.0, 15.0), watchos(1.0, 8.0), tvos(9.0, 15.0))	= kAudioAggregateDevicePropertyMainSubDevice
};

//==================================================================================================
#pragma mark -
#pragma mark AudioStream Types

/*!
    @typedef        AudioStreamID
    @abstract       AudioStream is the base class for all objects that represent a stream of data on
                    an audio device.
    @discussion     AudioStream is a subclass of AudioObject and can contain AudioControls.
*/
typedef AudioObjectID   AudioStreamID;

/*!
    @typedef        AudioStreamPropertyListenerProc
    @abstract       Clients register an AudioStreamPropertyListenerProc with the AudioStream object
                    in order to receive notifications when the properties of the object change.
    @discussion     Note that the same functionality is provided by AudioObjectPropertyListenerProc.
    @param          inStream
                        The AudioStream whose property has changed.
    @param          inChannel
                        The channel of the property that changed where 0 is the main channel.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property that changed.
    @param          inClientData
                        A pointer to client data established when the listener proc was registered
                        with the object.
    @result         The return value is currently unused and should always be 0.
*/
typedef OSStatus
(*AudioStreamPropertyListenerProc)( AudioStreamID           inStream,
                                    UInt32                  inChannel,
                                    AudioDevicePropertyID   inPropertyID,
                                    void* __nullable        inClientData);

//==================================================================================================
#pragma mark AudioStream Constants

/*!
    @enum           AudioStream Class Constants
    @abstract       Various constants related to AudioStreams.
    @constant       kAudioStreamUnknown
                        The AudioObjectID for a nonexistent AudioObject.
*/
CF_ENUM(AudioObjectID)
{
    kAudioStreamUnknown = kAudioObjectUnknown
};

//==================================================================================================
#pragma mark AudioStream Properties

/*!
    @enum           AudioStream Properties That Ought To Some Day Be Deprecated
    @abstract       AudioObjectPropertySelector values whose functionality is better provided by
                    other selectors.
    @discussion     These selectors are still provided for backward compatibility. The description
                    of the property will indicate in parentheses the better selectors to use and
                    why.
    @constant       kAudioStreamPropertyOwningDevice
                        The AudioObjectID of the AudioDevice of which this AudioStream is a part.
                        (kAudioObjectPropertyOwner: This is just another name for the inherited
                        selector.)
    @constant       kAudioStreamPropertyPhysicalFormats
                        An array of AudioStreamBasicDescriptions that describe the available data
                        formats for the AudioStream. The physical format refers to the data format
                        in which the hardware for the owning AudioDevice performs its IO
                        transactions.
                        (kAudioStreamPropertyAvailablePhysicalFormats: The new name for this
                        property is much clearer for readers of the API to see what is meant and the
                        AudioStreamRangedDescription structure provides better information.)
    @constant       kAudioStreamPropertyPhysicalFormatSupported
                        An AudioStreamBasicDescription is passed in to query whether or not the
                        format is supported. A kAudioDeviceUnsupportedFormatError will be returned
                        if the format is not supported and kAudioHardwareNoError will be returned if
                        it is supported. AudioStreamBasicDescription fields set to 0 will be ignored
                        in the query, but otherwise values must match exactly. The physical format
                        refers to the data format in which the hardware for the owning AudioDevice
                        performs its IO transactions.
                        (kAudioStreamPropertyAvailablePhysicalFormats: The proper and most robust
                        way to find a format that the AudioStream can support is to get the list of
                        available formats and look through that rather than using this property.)
    @constant       kAudioStreamPropertyPhysicalFormatMatch
                        An AudioStreamBasicDescription is passed in and the AudioStream will modify
                        it to describe the best match, in the AudioDevice's opinion, for the given
                        format. The physical format refers to the data format in which the hardware
                        for the owning AudioDevice performs its IO transactions.
                        (kAudioStreamPropertyAvailablePhysicalFormats: The proper and most robust
                        way to find a format that the AudioStream can support is to get the list of
                        available formats and look through that rather than using this property.)
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioStreamPropertyOwningDevice                = kAudioObjectPropertyOwner,
    kAudioStreamPropertyPhysicalFormats             = 'pft#',
    kAudioStreamPropertyPhysicalFormatSupported     = 'pft?',
    kAudioStreamPropertyPhysicalFormatMatch         = 'pftm'
};

//==================================================================================================
#pragma mark AudioStream Functions

/*!
    @functiongroup  AudioStream
*/

/*!
    @function       AudioStreamGetPropertyInfo
    @abstract       Retrieve information about the given property of an AudioStream.
    @param          inStream
                        The AudioStream to query.
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
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioStreamGetPropertyInfo( AudioStreamID           inStream,
                            UInt32                  inChannel,
                            AudioDevicePropertyID   inPropertyID,
                            UInt32* __nullable      outSize,
                            Boolean* __nullable     outWritable)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioStreamGetProperty
    @abstract       Queries an the AudioStream object to get the data of the given property and
                    places it in the provided buffer.
    @discussion     Note that the same functionality is provided by the function
                    AudioObjectGetPropertyData().
    @param          inStream
                        The AudioStream to query.
    @param          inChannel
                        The channel of the property to query where 0 is the main channel.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to query.
    @param          ioPropertyDataSize
                        A UInt32 which on entry indicates the size of the buffer pointed to by
                        outData and on exit indicates how much of the buffer was used.
    @param          outPropertyData
                        The buffer into which the object will put the data for the given property.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioStreamGetProperty( AudioStreamID           inStream,
                        UInt32                  inChannel,
                        AudioDevicePropertyID   inPropertyID,
                        UInt32*                 ioPropertyDataSize,
                        void*                   outPropertyData)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioStreamSetProperty
    @abstract       Tells the AudioStream object to change the value of the given property using the
                    provided data.
    @discussion     Note that the value of the property should not be considered changed until the
                    HAL has called the listeners as many properties values are changed
                    asynchronously. Also note that the same functionality is provided by the
                    function AudioObjectSetPropertyData().
    @param          inStream
                        The AudioStream to change.
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
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioStreamSetProperty( AudioStreamID                       inStream,
                        const AudioTimeStamp* __nullable    inWhen,
                        UInt32                              inChannel,
                        AudioDevicePropertyID               inPropertyID,
                        UInt32                              inPropertyDataSize,
                        const void*                         inPropertyData)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioStreamAddPropertyListener
    @abstract       Registers the given AudioStreamPropertyListenerProc to receive notifications
                    when the given property changes.
    @discussion     Note that the same functionality is provided by AudioObjectAddPropertyListener
                    in conjunction with AudioObjectPropertyListenerProc.
    @param          inStream
                        The AudioStream with whom to register the listener.
    @param          inChannel
                        The channel of the property to listen to.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to listen to.
    @param          inProc
                        AudioStreamPropertyListenerProc to call.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioStreamAddPropertyListener( AudioStreamID                   inStream,
                                UInt32                          inChannel,
                                AudioDevicePropertyID           inPropertyID,
                                AudioStreamPropertyListenerProc inProc,
                                void* __nullable                inClientData)                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);

/*!
    @function       AudioStreamRemovePropertyListener
    @abstract       Unregisters the given AudioStreamPropertyListenerProc from receiving
                    notifications when the given property changes.
    @discussion     Note that the same functionality is provided by
                    AudioObjectRemovePropertyListener in conjunction with
                    AudioObjectPropertyListenerProc.
    @param          inStream
                        The AudioStream with whom to unregister the listener.
    @param          inChannel
                        The channel of the property to unregister from.
    @param          inPropertyID
                        The AudioDevicePropertyID of the property to stop listening to.
    @param          inProc
                        AudioStreamPropertyListenerProc to unregister.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioStreamRemovePropertyListener(  AudioStreamID                   inStream,
                                    UInt32                          inChannel,
                                    AudioDevicePropertyID           inPropertyID,
                                    AudioStreamPropertyListenerProc inProc)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_1, __MAC_10_6, __IPHONE_2_0, __IPHONE_2_0);


//==================================================================================================
#pragma mark -
#pragma mark AudioControl Constants

/*!
    @enum           AudioControl Base Class IDs
    @abstract       The AudioClassIDs that identify the various AudioControl base classes.
    @constant       kAudioBootChimeVolumeControlClassID
                        An AudioLevelControl for the boot chime of the CPU.
*/
CF_ENUM(AudioClassID)
{
    kAudioBootChimeVolumeControlClassID = 'pram'
};

//==================================================================================================
#pragma mark AudioControl Properties

/*!
    @enum           AudioControl Properties
    @abstract       AudioObjectPropertySelector values that apply to all AudioControls.
    @discussion     AudioControl is a subclass of AudioObject and has only the single scope,
                    kAudioObjectPropertyScopeGlobal, and only a main element.
    @constant       kAudioControlPropertyVariant
                        A UInt32 that identifies the specific variant of an AudioControl. This
                        allows the owning AudioObject to support controls that are of the same basic
                        class (that is, the values of  kAudioObjectPropertyClass are the same) but
                        may control a part of the object for which the standard controls do not
                        control.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioControlPropertyVariant    = 'cvar'
};

/*!
    @enum           AudioClockSourceControl Properties
    @abstract       AudioObjectPropertySelector values that apply only to AudioClockSourceControls.
    @discussion     These properties supplement the regular AudioSelectorControl Properties.
    @constant       kAudioClockSourceControlPropertyItemKind
                        This property returns a UInt32 that identifies the kind of clock source
                        the item ID refers to. The qualifier contains the ID of the item. Note that
                        this property is a synonym for kAudioSelectorControlPropertyItemKind.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioClockSourceControlPropertyItemKind     = kAudioSelectorControlPropertyItemKind
};

//==================================================================================================

#if defined(__cplusplus)
}
#endif

CF_ASSUME_NONNULL_END

#endif  //  CoreAudio_AudioHardwareDeprecated_h
