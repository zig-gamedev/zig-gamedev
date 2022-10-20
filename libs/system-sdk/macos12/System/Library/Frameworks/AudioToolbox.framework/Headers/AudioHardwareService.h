/*!
	@file		AudioHardwareService.h
	@framework	AudioToolbox.framework
	@copyright	(c) 2006-2015 by Apple, Inc., all rights reserved.
    @abstract   API's for general high level audio services.
 
    @discussion
 
     The Audio Hardware Service (AHS) provides a way for applications to query and manipulate the
     aspects of an audio hardware device without incurring the overhead of loading the full audio
     HAL. AHS provides access to all the AudioObjects and their properties on the system. However,
     access is limited to only those properties that do not directly impact IO. For example, you can
     query the device's format but you can't query its IO buffer size. As such, the AHS API directly
     incorporates the various structures and constants in HAL's API, with the caveat that the
     AudioObjectIDs used in AHS cannot be used with the HAL.
*/

#ifndef AudioHardwareService_h
#define AudioHardwareService_h

#if !TARGET_OS_IPHONE
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
    #include <Availability.h>
    #include <CoreAudio/AudioHardware.h>
#else
    #include <AudioHardware.h>
#endif
#endif

#if !TARGET_OS_IPHONE

#if defined(__cplusplus)
extern "C"
{
#endif
    
//==================================================================================================
#pragma mark    Audio Hardware Service Properties

/*!
    @enum           Audio Hardware Service Properties
    @abstract       AudioObjectPropertySelector values that apply to various kinds of AudioObjects
                    only when accessed via the Audio Hardware Service API.
    @constant       kAudioHardwareServiceProperty_ServiceRestarted
                        A Float32 whose value has no meaning. Rather, this property exists so that
                        clients can be informed when the service has been reset for some reason.
                        When a reset happens, any state the client has with AHS, such as cached data
                        or added listeners, must be re-established by the client.
    @constant       kAudioHardwareServiceDeviceProperty_VirtualMainVolume
                        A Float32 that represents the value of the volume control. The range is
                        between 0.0 and 1.0 (inclusive). This actual volume controls this property
                        manipulates depends on what the device provides. If the device has a true
                        main volume control, this property directly controls that. If the device
                        has individual channel volume controls, this property will apply to those
                        identified by the device's preferred multi-channel layout (or preferred
                        stereo pair if the device is stereo only). Note that this control maintains
                        the relative balance between all the channels it affects.
    @constant       kAudioHardwareServiceDeviceProperty_VirtualMainBalance
                        A Float32 that represents the value of the stereo balance control. The range
                        is from 0.0 (all power to the left) to 1.0 (all power to the right) with
                        the value of 0.5 signifying that the channels have equal power. This control
                        is only present for devices that have individual channel volume controls. It
                        manipulates the relative balance between the volume controls on the channels
                        identified as the device's default stereo pair.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioHardwareServiceProperty_ServiceRestarted              = 'srst',
    
	kAudioHardwareServiceDeviceProperty_VirtualMainVolume     = 'vmvc',
	kAudioHardwareServiceDeviceProperty_VirtualMasterVolume   API_DEPRECATED_WITH_REPLACEMENT("kAudioHardwareServiceDeviceProperty_VirtualMainVolume", macos(10.5, 10.5)) API_UNAVAILABLE(ios, watchos, tvos) = kAudioHardwareServiceDeviceProperty_VirtualMainVolume,
    
	kAudioHardwareServiceDeviceProperty_VirtualMainBalance    = 'vmbc',
	kAudioHardwareServiceDeviceProperty_VirtualMasterBalance   API_DEPRECATED_WITH_REPLACEMENT("kAudioHardwareServiceDeviceProperty_VirtualMainBalance", macos(10.5, 10.5)) API_UNAVAILABLE(ios, watchos, tvos) = kAudioHardwareServiceDeviceProperty_VirtualMainBalance,
};

//==================================================================================================
#pragma mark    Audio Hardware Service Functions

/*!
    @functiongroup  Audio Hardware Service
*/

/*!
    @function       AudioHardwareServiceHasProperty
    @abstract       Queries an AudioObject about whether or not it has the given property.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @result         A Boolean indicating whether or not the AudioObject has the given property.
*/
extern Boolean
AudioHardwareServiceHasProperty(    AudioObjectID                       inObjectID,
                                    const AudioObjectPropertyAddress*   inAddress)                  
                                                            API_DEPRECATED("no longer supported", macos(10.5, 10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function       AudioHardwareServiceIsPropertySettable
    @abstract       Queries an AudioObject about whether or not the given property can be set using
                    AudioHardwareServiceSetPropertyData.
    @param          inObjectID
                        The AudioObject to query.
    @param          inAddress
                        An AudioObjectPropertyAddress indicating which property is being queried.
    @param          outIsSettable
                        A Boolean indicating whether or not the property can be set.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceIsPropertySettable( AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        Boolean*                            outIsSettable)          
                                                            API_DEPRECATED("no longer supported", macos(10.5, 10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function       AudioHardwareServiceGetPropertyDataSize
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
AudioHardwareServiceGetPropertyDataSize(    AudioObjectID                       inObjectID,
                                            const AudioObjectPropertyAddress*   inAddress,
                                            UInt32                              inQualifierDataSize,
                                            const void*                         inQualifierData,
                                            UInt32*                             outDataSize)        
                                                            API_DEPRECATED("no longer supported", macos(10.5, 10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function       AudioHardwareServiceGetPropertyData
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
AudioHardwareServiceGetPropertyData(    AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        UInt32                              inQualifierDataSize,
                                        const void*                         inQualifierData,
                                        UInt32*                             ioDataSize,
                                        void*                               outData)                
                                                        API_DEPRECATED("no longer supported", macos(10.5, 10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function       AudioHardwareServiceSetPropertyData
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
AudioHardwareServiceSetPropertyData(    AudioObjectID                       inObjectID,
                                        const AudioObjectPropertyAddress*   inAddress,
                                        UInt32                              inQualifierDataSize,
                                        const void*                         inQualifierData,
                                        UInt32                              inDataSize,
                                        const void*                         inData)                 
                                                        API_DEPRECATED("no longer supported", macos(10.5, 10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function       AudioHardwareServiceAddPropertyListener
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
AudioHardwareServiceAddPropertyListener(    AudioObjectID                       inObjectID,
                                            const AudioObjectPropertyAddress*   inAddress,
                                            AudioObjectPropertyListenerProc     inListener,
                                            void*                               inClientData)       
                                                        API_DEPRECATED("no longer supported", macos(10.5, 10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function       AudioHardwareServiceRemovePropertyListener
    @abstract       Unregisters the given AudioObjectPropertyListenerProc from receiving
                    notifications when the given properties change.
    @param          inObjectID
                        The AudioObject to unregister the listener from.
    @param          inAddress
                        The AudioObjectPropertyAddresses indicating which property the listener
                        will stop being notified about.
    @param          inListener
                        The AudioObjectPropertyListenerProc being removed.
    @param          inClientData
                        A pointer to client data that is passed to the listener when it is called.
    @result         An OSStatus indicating success or failure.
*/
extern OSStatus
AudioHardwareServiceRemovePropertyListener( AudioObjectID                       inObjectID,
                                            const AudioObjectPropertyAddress*   inAddress,
                                            AudioObjectPropertyListenerProc     inListener,
                                            void*                               inClientData)       
                                                        API_DEPRECATED("no longer supported", macos(10.5, 10.11)) API_UNAVAILABLE(ios, watchos, tvos);

#if defined(__cplusplus)
}
#endif
#endif //!TARGET_OS_IPHONE

#endif /* AudioHardwareService_h */
