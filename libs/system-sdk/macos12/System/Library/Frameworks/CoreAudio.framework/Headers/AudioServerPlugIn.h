/*==================================================================================================
     File:       CoreAudio/AudioServerPlugIn.h

     Copyright:  (c) 1985-2011 by Apple Inc., all rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

==================================================================================================*/
#if !defined(CoreAudio_AudioServerPlugIn_h)
#define CoreAudio_AudioServerPlugIn_h

//==================================================================================================
#pragma mark -
#pragma mark Overview
/*!
    @header AudioServerPlugIn
    
    An AudioServerPlugIn is a CFPlugIn that is loaded by the host process as a driver. The plug-in
    bundle is installed in /Library/Audio/Plug-Ins/HAL. The bundle's name has the suffix ".driver".
    When loading the plug-in, the host looks for factories with the plug-in type,
    kAudioServerPlugInTypeUUID. The plug-in provides an object that conforms to the interface,
    kAudioServerPlugInDriverInterfaceUUID.

    An AudioServerPlugIn can provide the host with information that describes the conditions that
    must be met to load the plug-in. This is done through plug-in bundle's info.plist in a key named
    "AudioServerPlugIn_LoadingConditions". The value of this key is a dictionary whose keys describe
    the loading conditions for the plug-in. Currently, the only defined key is named
    "IOService Matching" whose value is an array of IOService matching dictionaries. The host will
    load the plug-in if any of these matching dictionaries match an IOService.

    An AudioServerPlugIn operates in its own process separate from the system daemon. First and
    foremost, an AudioServerPlugIn may not make any calls to the client HAL API in the
    CoreAudio.framework. This will result in undefined (but generally bad) behavior.

    Further, the host process is sandboxed. As such, an AudioServerPlugIn may only read files in its
    bundle in addition to the system libraries and frameworks. It may not access user documents or
    write to any filesystem locations other than the system's cache and temporary directories as
    derived through Apple API. The host provides a means for the plug-in to store and retrieve data
    from persistent storage.

    An AudioServerPlugIn may communicate with other processes on the system. However, the plug-in
    must list the name of the mach services to be accessed in the plug-in bundle's info.plist in a
    key named "AudioServerPlugIn_MachServices". The value of this key is an array of the names of
    the mach services that need to be accessed.

    An AudioServerPlugIn may create user-clients via IOServiceOpen() for standard IOKit objects
    without restriction. However, if a plug-in needs to create a custom user-client, it must list
    the name of the class of the user-client in the plug-in bundle's info.plist in a key named
    "AudioServerPlugIn_IOKitUserClients". The value of this key is an array of the names of the
    classes.

    An AudioServerPlugIn may also use network resources. However, the plug-in must declare this in
    its bundle's info.plist with the key named, "AudioServerPlugIn_Network". The value of this key
    is a boolean and must be set to true if the key is present.

    An AudioServerPlugIn provides the same property-based object model as the HAL's client
    framework. The basic objects and properties are defined in <CoreAudio/AudioHardwareBase.h> and
    are supplemented with properties declared here. The plug-in is responsible for defining the
    AudioObjectIDs to be used as handles for the AudioObjects the plug-in provides. However, the
    AudioObjectID for the one and only AudioPlugIn object must be kAudioObjectPlugInObject.

    When the state of an AudioObject implemented by the plug-in changes, it notifies the host using
    the host routine, PropertiesChanged(). The only exception to this is for AudioDevice objects.
    AudioDevices may call the host's PropertiesChanged() routine only for state changes that don't
    have any effect on IO or on the structure of the AudioDevice, such as a change to the value of a
    volume control.

    For changes to an AudioDevice's state that will affect IO or its structure, the change may not
    be made without first making a call to the host's RequestDeviceConfigurationChange() routine.
    This allows the host an opportunity to stop any outstanding IO and otherwise return the device
    to its ground state. The host will inform the plug-in that it is safe to make the change by
    calling the plug-in routine, PerformDeviceConfigurationChange(). It is only at this point that
    the device can make the state change. When PerformDeviceConfigurationChange() returns, the host
    will figure out what changed and restart any outstanding IO.

    The host is in control of IO. It tells the plug-in's AudioDevice when to start and when to stop
    the hardware. The host drives its timing using the timestamps provided by the AudioDevice's
    implementation of GetZeroTimeStamp(). The series of timestamps provides a mapping between the
    device's sample time and mach_absolute_time().

    The host provides the plug-in's device access to several tap points into the system's mix engine
    to allow for a variety of features, including adding processing to the signal. The host breaks
    these tap points down into IO operations that the host asks the plug-in to perform at the
    appropriate time. Prior to starting IO, the host will ask the plug-in which operations are to be
    performed. Note that the IO operations are performed on a real time thread on a deadline. As
    such the plug-in must avoid avoid blocking and return as quickly as possible.

    An AudioServerPlugIn can talk to a DriverKit-based Driver Extension. To do so requires adjusting
    the packaging of the CFPlugIn bundle slightly in order to carry the required entitlements. This
    requires turning the AudioServerPlugIn bundle into an executable. Coding the AudioServerPlugIn
    is the same as described above but there are some additional packaging changes to make:
        - Add the "Load As Application" key with the value of the number 1 to the
            AudioServerPlugIn's info.plist.
        - Export the symbol for the plug-in's factory function using a .exp file or other mechanism.
        - Set the "Mach-O Type" build setting for the target to Executable.
        - Add "-e _AudioServerPlugInMain" to the "Other Linker Flags" build setting and link
        	against CoreAudio.framework.
        - Code sign the binary such that it includes the
            "com.apple.developer.driverkit.userclient-access" entitlements necessary for talking to
            the Driver Extension.
*/

//==================================================================================================
#pragma mark -
#pragma mark Includes

#include <CoreAudioTypes/CoreAudioTypes.h>
#include <CoreAudio/AudioHardwareBase.h>
#include <CoreFoundation/CoreFoundation.h>
#if COREFOUNDATION_CFPLUGINCOM_SEPARATE
    #include <CoreFoundation/CFPlugInCOM.h>
#endif

//==================================================================================================

CF_ASSUME_NONNULL_BEGIN

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
#pragma mark -
#pragma mark Types

/*!
    @typedef        AudioServerPlugInHostInterface
    @abstract       Forward declaration of AudioServerPlugInHostInterface.
*/
typedef struct AudioServerPlugInHostInterface   AudioServerPlugInHostInterface;

/*!
    @typedef        AudioServerPlugInHostRef
    @abstract       A reference to an AudioServerPlugInHostInterface.
*/
typedef const AudioServerPlugInHostInterface*   AudioServerPlugInHostRef;

/*!
    @typedef        AudioServerPlugInDriverInterface
    @abstract       Forward declaration of AudioServerPlugInDriverInterface.
*/
typedef struct AudioServerPlugInDriverInterface    AudioServerPlugInDriverInterface;

/*!
    @typedef        AudioServerPlugInDriverRef
    @abstract       A reference to an AudioServerPlugInDriverInterface.
*/
typedef AudioServerPlugInDriverInterface* __nullable *  AudioServerPlugInDriverRef;

/*!
    @struct         AudioServerPlugInCustomPropertyInfo
    @abstract       The AudioServerPlugInCustomPropertyInfo struct is used to describe enough about
                    a custom property to allow the Host to marshal the data between the Host and
                    its clients.
    @field          mSelector
                        The AudioObjectPropertySelector of the custom property.
    @field          mPropertyDataType
                        A UInt32 whose value indicates the data type of the data of the custom
                        property. Constants for this value are defined in the Basic Constants
                        section.
    @field          mQualifierDataType
                        A UInt32 whose value indicates the data type of the data of the custom
                        property. Constants for this value are defined in the Basic Constants
                        section.
*/
struct  AudioServerPlugInCustomPropertyInfo
{
    AudioObjectPropertySelector mSelector;
    UInt32                      mPropertyDataType;
    UInt32                      mQualifierDataType;
};
typedef struct AudioServerPlugInCustomPropertyInfo  AudioServerPlugInCustomPropertyInfo;

/*!
    @struct         AudioServerPlugInClientInfo
    @abstract       The AudioServerPlugInClientInfo struct is used to identify a client of the Host
                    to the plug-in.
    @field          mClientID
                        An ID that allows for differentiating multiple clients in the same process.
                        This ID is passed to the plug-in during IO so that the plug-in can associate
                        the IO with the client easily.
    @field          mProcessID
                        The pid_t of the process that contains the client.
    @field          mIsNativeEndian
                        A Boolean indicating whether or not the client has the same endianness as
                        the server.
    @field          mBundleID
                        A CFStringRef that contains the bundle ID of the main bundle of the process
                        that contains the client. Note that the plug-in is expected to retain this
                        string itself if the plug-in wishes to keep it around.
*/
struct  AudioServerPlugInClientInfo
{
    UInt32                  mClientID;
    pid_t                   mProcessID;
    Boolean                 mIsNativeEndian;
    CFStringRef __nullable  mBundleID;
};
typedef struct AudioServerPlugInClientInfo  AudioServerPlugInClientInfo;

/*!
    @struct         AudioServerPlugInIOCycleInfo
    @abstract       This structure holds the common items passed to all the plug-in's IO routines to
                    describe an IO cycle
    @field          mCycleCounter
                        The cycle ordinal number. This number starts at 1 for the first IO cycle and
                        increments for each subsequent IO cycle. The count will recycle back to 1
                        whenever the IO thread resynchronizes with the hardware.
    @field          mNominalIOBufferFrameSize
                        The number of sample frames that will nominally be read/written in the new
                        IO cycle. Note that the actual IO buffers that get read/written might be a
                        different size depending on whether or not any drift compensation is being
                        applied by the Host.
    @field          mInputTime
                        The time stamp that indicates from where in the device's time line the input
                        data for the new IO cycle will start at.
    @field          mOutputTime
                        The time stamp that indicates from where in the device's time line the output
                        data for the new IO cycle will start at.
    @field          mMainHostTicksPerFrame
                        The number of host ticks per frame that the Host's clock is measuring for
                        the main device.
    @field          mDeviceHostTicksPerFrame
                        The number of host ticks per frame that the Host's clock is measuring for
                        this device. Note that this value will be equal to the main value for
                        clockless devices or when the device is the main device in an aggregate.
*/
struct AudioServerPlugInIOCycleInfo
{
    UInt64          mIOCycleCounter;
    UInt32          mNominalIOBufferFrameSize;
    AudioTimeStamp  mCurrentTime;
    AudioTimeStamp  mInputTime;
    AudioTimeStamp  mOutputTime;
    union
    {
		Float64		mMainHostTicksPerFrame;
		Float64		mMasterHostTicksPerFrame API_DEPRECATED_WITH_REPLACEMENT("mMainHostTicksPerFrame", macos(10.0, 12.0), ios(2.0, 15.0), watchos(1.0, 8.0), tvos(9.0, 15.0));
	};
    Float64         mDeviceHostTicksPerFrame;
};
typedef struct AudioServerPlugInIOCycleInfo AudioServerPlugInIOCycleInfo;

//==================================================================================================
#pragma mark -
#pragma mark Basic Constants

/*!
    @defined        kAudioServerPlugInTypeUUID
    @abstract       The UUID of the plug-in type (443ABAB8-E7B3-491A-B985-BEB9187030DB).
*/
#define kAudioServerPlugInTypeUUID  CFUUIDGetConstantUUIDWithBytes(NULL, 0x44, 0x3A, 0xBA, 0xB8, 0xE7, 0xB3, 0x49, 0x1A, 0xB9, 0x85, 0xBE, 0xB9, 0x18, 0x70, 0x30, 0xDB)

/*!
    @enum           Predefined AudioObjectID values 
    @abstract       ObjectIDs that are always the same
    @constant       kAudioObjectPlugInObject
                        The AudioObjectID that always refers to the one and only instance of the
                        AudioPlugIn for the plug-in.
*/
CF_ENUM(AudioObjectID)
{
    kAudioObjectPlugInObject    = 1
};

/*!
    @enum           Predefined client ID values 
    @constant       kAudioServerPlugInHostClientID
                        No actual client will have this value as its ID. It is only used when the
                        Host is making a request on its own behalf.
*/
CF_ENUM(AudioObjectID)
{
    kAudioServerPlugInHostClientID  = 0
};

/*!
    @enum           Custom Property Data Types
    @abstract       The set of data types the Host knows how to marshal between the server and the
                    client. These are the only types supported for custom properties. See
                    AudioServerPlugInCustomPropertyInfo for more information.
    @constant       kAudioServerPlugInCustomPropertyDataTypeNone
                        This is used to declare that the property doesn't have any property or
                        qualifier data.
    @constant       kAudioServerPlugInCustomPropertyDataTypeCFString
                        The property/qualifier data is a CFStringRef.
    @constant       kAudioServerPlugInCustomPropertyDataTypeCFPropertyList
                        The property/qualifier data is a CFPropertyListRef.
*/
typedef UInt32  AudioServerPlugInCustomPropertyDataType;
CF_ENUM(AudioServerPlugInCustomPropertyDataType)
{
    kAudioServerPlugInCustomPropertyDataTypeNone            = 0,
    kAudioServerPlugInCustomPropertyDataTypeCFString        = 'cfst',
    kAudioServerPlugInCustomPropertyDataTypeCFPropertyList  = 'plst'
};

/*!
    @enum           IO Operation IDs
    @abstract       The valid values for the inOperationID argument to the various IO methods.
    @constant       kAudioServerPlugInIOOperationThread
                        This operation marks the beginning and the ending of the IO thread. Note
                        that DoIOOperation() will never be called with this ID.
    @constant       kAudioServerPlugInIOOperationCycle
                        This operation marks the beginning and ending of each IO cycle. Note that
                        DoIOOperation() will never be called with this ID.
    @constant       kAudioServerPlugInIOOperationReadInput
                        This operation transfers the input data from the device's ring buffer to the
                        provided buffer in the stream's native format. Note that this operation
                        always happens in-place in the main buffer passed to DoIOOperation(). It is
                        required that this operation be implemented if the AudioDevice has input
                        streams.
    @constant       kAudioServerPlugInIOOperationConvertInput
                        This operation converts the input data from its native format to the
                        canonical format.
    @constant       kAudioServerPlugInIOOperationProcessInput
                        This operation performs arbitrary signal processing on the input data in the
                        canonical format.
    @constant       kAudioServerPlugInIOOperationProcessOutput
                        This operation performs arbitrary signal processing on the output data in
                        the canonical format.
    @constant       kAudioServerPlugInIOOperationMixOutput
                        This operation mixes the output data into the device's ring buffer. Note
                        that if a plug-in implements this operation, no further output operations
                        will occur for that cycle. It is assumed that the device handles everything
                        from there down including preparing the data for consumption by the
                        hardware. Note also that this operation always happens in-place in the main
                        buffer passed to DoIOOperation().
    @constant       kAudioServerPlugInIOOperationProcessMix
                        This operation processes the full mix of all clients' data in the canonical
                        format.
    @constant       kAudioServerPlugInIOOperationConvertMix
                        This operation converts the fully mixed data from the canonical format to
                        the device's native format.
    @constant       kAudioServerPlugInIOOperationWriteMix
                        This operation puts the data into the device's ring buffer for consumption
                        of the hardware. Note that this operation always happens in-place in the
                        main buffer passed to DoIOOperation(). It is required that this operation be
                        implemented if the AudioDevice has output streams.
*/
typedef CF_ENUM(UInt32, AudioServerPlugInIOOperation)
{
    kAudioServerPlugInIOOperationThread         = 'thrd',
    kAudioServerPlugInIOOperationCycle          = 'cycl',
    kAudioServerPlugInIOOperationReadInput      = 'read',
    kAudioServerPlugInIOOperationConvertInput   = 'cinp',
    kAudioServerPlugInIOOperationProcessInput   = 'pinp',
    kAudioServerPlugInIOOperationProcessOutput  = 'pout',
    kAudioServerPlugInIOOperationMixOutput      = 'mixo',
    kAudioServerPlugInIOOperationProcessMix     = 'pmix',
    kAudioServerPlugInIOOperationConvertMix     = 'cmix',
    kAudioServerPlugInIOOperationWriteMix       = 'rite'
};

//==================================================================================================
#pragma mark -
#pragma mark AudioObject Properties

/*!
    @enum           AudioObject Property Selectors
    @abstract       AudioObjectPropertySelector values provided by objects of the AudioObject class.
    @discussion     The AudioObject class is the base class for all classes. As such, all classes
                    inherit this set of properties.
    @constant       kAudioObjectPropertyCustomPropertyInfoList
                        An array of AudioServerPlugInCustomPropertyInfo that describe the custom
                        properties the object has.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioObjectPropertyCustomPropertyInfoList  = 'cust'
};

//==================================================================================================
#pragma mark -
#pragma mark AudioPlugIn Properties

/*!
    @enum           AudioPlugIn Class Constants
    @abstract       Various constants related to the AudioPlugIn class.
    @constant       kAudioPlugInPropertyResourceBundle
                        A CFString that contains a path to a resource bundle to use for localizing
                        strings and fetching resources like icons from the client process. The path
                        is relative to the path of the plug-in's bundle. The caller is responsible
                        for releasing the returned CFObject.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioPlugInPropertyResourceBundle  = 'rsrc'
};

//==================================================================================================
#pragma mark -
#pragma mark Device Constants

/*!
    @enum           Clock Smoothing Algorithm Selectors
    @abstract       The valid values for kAudioDevicePropertyClockAlgorithm
    @constant       kAudioDeviceClockAlgorithmRaw
                        When this value for the clock algorithm is specified, the Host will not
                        apply any filtering to the time stamps returned from GetZeroTimeStamp(). The
                        values will be used as-is.
    @constant       kAudioDeviceClockAlgorithmSimpleIIR
                        When this value for the clock algorithm is specified, the Host applies a
                        simple IIR filter to the time stamp stream. This is the default algorithm
                        used for devices that don't implement kAudioDevicePropertyClockAlgorithm.
    @constant       kAudioDeviceClockAlgorithm12PtMovingWindowAverage
                        This clock algorithm uses a 12 point moving window average to filter the time
                        stamps returned from GetZeroTimeStamp().
*/
typedef CF_ENUM(UInt32, AudioDeviceClockAlgorithmSelector)
{
    kAudioDeviceClockAlgorithmRaw                       = 'raww',
    kAudioDeviceClockAlgorithmSimpleIIR                 = 'iirf',
    kAudioDeviceClockAlgorithm12PtMovingWindowAverage   = 'mavg'
};

//==================================================================================================
#pragma mark Device Properties

/*!
    @enum           AudioDevice Properties
    @abstract       AudioObjectPropertySelector values provided by the AudioDevice class.
    @discussion     The AudioDevice class is a subclass of the AudioObjectClass. The class has four
                    scopes, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyScopeInput,
                    kAudioObjectPropertyScopeOutput, and kAudioObjectPropertyScopePlayThrough. The
                    class has a main element and an element for each channel in each stream
                    numbered according to the starting channel number of each stream.
    @constant       kAudioDevicePropertyZeroTimeStampPeriod
                        A UInt32 whose value indicates the number of sample frames the host can
                        expect between successive time stamps returned from GetZeroTimeStamp(). In
                        other words, if GetZeroTimeStamp() returned a sample time of X, the host can
                        expect that the next valid time stamp that will be returned will be X plus
                        the value of this property.
    @constant       kAudioDevicePropertyClockAlgorithm
                        A UInt32 whose value indicates to the Host what smoothing algorithm to use
                        for a device's clock. The only legal values for this property are specified
                        in the enum below.
    @constant       kAudioDevicePropertyClockIsStable
                        A UInt32 where a non-zero value indicates that the device's clock runs at or
                        very near the nominal rate with only small variations. If this property is
                        not implemented, it is assumed that the device's clock is stable.
*/
CF_ENUM(AudioObjectPropertySelector)
{
    kAudioDevicePropertyZeroTimeStampPeriod = 'ring',
    kAudioDevicePropertyClockAlgorithm      = 'clok',
    kAudioDevicePropertyClockIsStable       = 'cstb'
};

//==================================================================================================
#pragma mark -
#pragma mark AudioServerPlugInHostInterface

/*!
    @interface      AudioServerPlugInHostInterface
    @abstract       The interface that audio server plug-ins use to communicate with the Host.
*/
struct  AudioServerPlugInHostInterface
{

#pragma mark Property Operations

    /*!
        @method         PropertiesChanged
        @abstract       This method informs the Host when the state of an plug-in's object changes.
        @discussion     Note that for Device objects, this method is only used for state changes
                        that don't affect IO or the structure of the device.
        @param          inHost
                            The AudioServerPlugInHostRef passed to the plug-in at initialization.
        @param          inObjectID
                            The AudioObjectID of the object whose state has changed.
        @param          inNumberAddresses
                            The number of elements in the inAddresses array.
        @param          inAddresses
                            An array of AudioObjectPropertyAddresses for the changed properties.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (*PropertiesChanged)(   AudioServerPlugInHostRef            inHost,
                            AudioObjectID                       inObjectID,
                            UInt32                              inNumberAddresses,
                            const AudioObjectPropertyAddress*   inAddresses);

#pragma mark Storage Operations

    /*!
        @method         CopyFromStorage
        @abstract       This method will fetch the data associated with the named storage key.
        @param          inHost
                            The AudioServerPlugInHostRef passed to the plug-in at initialization.
        @param          inKey
                            A CFStringRef that contains the name of the key whose data is to be
                            fetched. Note that the Host will make sure that the keys for one plug-in
                            do not collide with the keys for other plug-ins.
        @param          outData
                            The data associated with the named storage key in the form of
                            CFPropertyList. The caller is responsible for releasing the returned
                            CFObject.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (*CopyFromStorage)( AudioServerPlugInHostRef                    inHost,
                        CFStringRef                                 inKey,
                        CFPropertyListRef __nullable * __nonnull    outData);

    /*!
        @method         WriteToStorage
        @abstract       This method will associate the given data with the named storage key,
                        replacing any existing data.
        @discussion     Note that any data stored this way is persists beyond the life span of the
                        Host including across rebooting.
        @param          inHost
                            The AudioServerPlugInHostRef passed to the plug-in at initialization.
        @param          inKey
                            A CFStringRef that contains the name of the key whose data is to be
                            written. Note that the Host will make sure that the keys for one plug-in
                            do not collide with the keys for other plug-ins.
        @param          inData
                            A CFPropertyListRef containing the data to associate with the key.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (*WriteToStorage)(  AudioServerPlugInHostRef    inHost,
                        CFStringRef                 inKey,
                        CFPropertyListRef           inData);

    /*!
        @method         DeleteFromStorage
        @abstract       This method will remove the given key and any associated data from storage.
        @param          inHost
                            The AudioServerPlugInHostRef passed to the plug-in at initialization.
        @param          inKey
                            A CFStringRef that contains the name of the key to be deleted. Note that
                            the Host will make sure that the keys for one plug-in do not collide
                            with the keys for other plug-ins.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (*DeleteFromStorage)(   AudioServerPlugInHostRef    inHost,
                            CFStringRef                 inKey);

#pragma mark Device Operations

    /*!
        @typedef        RequestDeviceConfigurationChange
        @abstract       Plug-ins invoke this routine to tell the Host to initiate a configuration
                        change operation.
        @discussion     When a plug-in's device object needs to change its structure or change any
                        state related to IO for any reason, it must begin this operation by invoking
                        this Host method. The device object may not perform the state change until
                        the Host gives the device clearance to do so by invoking the plug-in's
                        PerformDeviceConfigurationChange() routine. Note that the call to
                        PerformDeviceConfigurationChange() may be deferred to another thread at the
                        discretion of the host.
                        The sorts of changes that must go through this mechanism are anything that
                        affects either the structure of the device or IO. This includes, but is not
                        limited to, changing stream layout, adding/removing controls, changing the
                        nominal sample rate of the device, changing any sample formats on any stream
                        on the device, changing the size of the ring buffer, changing presentation
                        latency, and changing the safety offset.
        @param          inHost
                            The AudioServerPlugInHostRef passed to the plug-in at initialization.
        @param          inDeviceObjectID
                            The AudioObjectID of the device making the request.
        @param          inChangeAction
                            A UInt64 indicating the action the device object wants to take. It will
                            be passed back to the device in the invocation of
                            PerformDeviceConfigurationChange(). Note that this value is purely for
                            the plug-in's usage. The Host does not look at this value.
        @param          inChangeInfo
                            A pointer to information about the configuration change. It will be
                            passed back to the device in the invocation of
                            PerformDeviceConfigurationChange(). Note that this value is purely for
                            the plug-in's usage. The Host does not look at this value.
        @result         An OSStatus indicating success or failure. Note that even if this method
                        indicates success, the Host may still abort the change later.
    */
    OSStatus
    (*RequestDeviceConfigurationChange)(    AudioServerPlugInHostRef    inHost,
                                            AudioObjectID               inDeviceObjectID,
                                            UInt64                      inChangeAction,
                                            void* __nullable            inChangeInfo);
};

//==================================================================================================
#pragma mark -
#pragma mark AudioServerPlugInDriverInterface

/*!
    @interface      AudioServerPlugInDriverInterface
    @abstract       The CFPlugIn interface for a plug-in for the Audio Server that implements a
                    device driver.
    @discussion     In CFPlugIn terms, AudioServerPlugInDriverInterface is a subclass of IUnknown.
*/
struct  AudioServerPlugInDriverInterface
{

#pragma mark Inheritence
    /*!
        @field          _reserved
        @abstract       A standard part of the IUnknown interface.
    */
    void* __nullable    _reserved;

    /*!
        @method         QueryInterface
        @abstract       The IUnknown method for interface discovery.
        @param          inDriver
                            The CFPlugIn type to query.
        @param          inUUID
                            The UUID of the interface to find.
        @param          outInterface
                            The returned interface or NULL if none was found.
        @result         An error code indicating success of failure.
    */
    HRESULT
    (STDMETHODCALLTYPE *QueryInterface)(    void* __nullable                inDriver,
                                            REFIID                          inUUID,
                                            LPVOID __nullable * __nullable  outInterface);

    /*!
        @method         AddRef
        @abstract       The IUnknown method for retaining a reference to a CFPlugIn type.
        @param          inDriver
                            The CFPlugIn type to retain.
        @result         The resulting reference count after the new reference is added.
    */
    ULONG
    (STDMETHODCALLTYPE *AddRef)(    void* __nullable    inDriver);

    /*!
        @method         Release
        @abstract       The IUnknown method for releasing a reference to a CFPlugIn type.
        @param          inDriver
                            The CFPlugIn type to release.
        @result         The resulting reference count after the reference has been removed.
    */
    ULONG
    (STDMETHODCALLTYPE *Release)(   void* __nullable    inDriver);

#pragma mark Basic Operations

    /*!
        @method         Initialize
        @abstract       This method is called to initialize the instance of the plug-in.
        @discussion     As part of initialization, the plug-in should publish all the objects it
                        knows about at the time.
        @param          inDriver
                            The plug-in to initialize.
        @param          inHost
                            An AudioServerPlugInHostInterface struct that the plug-in is to use for
                            communication with the Host. The Host guarantees that the storage
                            pointed to by inHost will remain valid for the lifetime of the plug-in.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *Initialize)(    AudioServerPlugInDriverRef __nonnull    inDriver,
                                        AudioServerPlugInHostRef                inHost);

    /*!
        @method         CreateDevice
        @abstract       Tells the plug-in to create a new device based on the given description.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDescription
                            A CFDictionaryRef that contains a description of the device to create.
        @param          inClientInfo
                            A pointer to an AudioServerPlugInClientInfo structure describing the
                            client creating the device. Note that the mClientID field will be set to
                            kAudioServerPlugInHostClientID since the device hasn't been created yet.
                            The client info is passed here to give enough context to allow for the
                            creation of private devices.
        @param          outDeviceObjectID
                            On successful exit, this will contain the object ID of the newly created
                            device.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *CreateDevice)(  AudioServerPlugInDriverRef __nonnull    inDriver,
                                        CFDictionaryRef                         inDescription,
                                        const AudioServerPlugInClientInfo*      inClientInfo,
                                        AudioObjectID*                          outDeviceObjectID);

    /*!
        @method         DestroyDevice
        @abstract       Called to tell the plug-in about to destroy the given device.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                        The ID of the device to destroy.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *DestroyDevice)( AudioServerPlugInDriverRef __nonnull    inDriver,
                                        AudioObjectID                           inDeviceObjectID);

    /*!
        @method         AddDeviceClient
        @abstract       Called to tell the plug-in about a new client of the Host for a particular
                        device.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device that the client is using.
        @param          inClientInfo
                            A pointer to an AudioServerPlugInClientInfo structure describing the new
                            client. The plug-in should cache this data (including taking a retain on
                            the bundle ID) as the memory pointed is only valid for the duration of
                            the call.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *AddDeviceClient)(   AudioServerPlugInDriverRef __nonnull    inDriver,
                                            AudioObjectID                           inDeviceObjectID,
                                            const AudioServerPlugInClientInfo*      inClientInfo);

    /*!
        @method         RemoveDeviceClient
        @abstract       Called to tell the plug-in about a client that is no longer using the device.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device that the client is no longer using.
        @param          inClientInfo
                            A pointer to an AudioServerPlugInClientInfo structure describing the
                            client.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *RemoveDeviceClient)(    AudioServerPlugInDriverRef __nonnull    inDriver,
                                                AudioObjectID                           inDeviceObjectID,
                                                const AudioServerPlugInClientInfo*      inClientInfo);

    /*!
        @method         PerformDeviceConfigurationChange
        @abstract       This is called by the Host to allow the device to perform a configuration
                        change that had been previously requested via a call to the Host method,
                        RequestDeviceConfigurationChange().
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device that wants to change its configuration.
        @param          inChangeAction
                            A UInt64 indicating the action the device object wants to take. This is
                            the same value that was passed to RequestDeviceConfigurationChange().
                            Note that this value is purely for the plug-in's usage. The Host does
                            not look at this value.
        @param          inChangeInfo
                            A pointer to information about the configuration change. This is the
                            same value that was passed to RequestDeviceConfigurationChange(). Note
                            that this value is purely for the plug-in's usage. The Host does not
                            look at this value.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *PerformDeviceConfigurationChange)(  AudioServerPlugInDriverRef __nonnull    inDriver,
                                                            AudioObjectID                           inDeviceObjectID,
                                                            UInt64                                  inChangeAction,
                                                            void* __nullable                        inChangeInfo);

    /*!
        @method         AbortDeviceConfigurationChange
        @abstract       This is called by the Host to tell the plug-in not to perform a
                        configuration change that had been requested via a call to the Host method,
                        RequestDeviceConfigurationChange().
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device that wants to change its configuration.
        @param          inChangeAction
                            A UInt64 indicating the action the device object wants to take. This is
                            the same value that was passed to RequestDeviceConfigurationChange().
                            Note that this value is purely for the plug-in's usage. The Host does
                            not look at this value.
        @param          inChangeInfo
                            A pointer to information about the configuration change. This is the
                            same value that was passed to RequestDeviceConfigurationChange(). Note
                            that this value is purely for the plug-in's usage. The Host does not
                            look at this value.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *AbortDeviceConfigurationChange)(    AudioServerPlugInDriverRef __nonnull    inDriver,
                                                            AudioObjectID                           inDeviceObjectID,
                                                            UInt64                                  inChangeAction,
                                                            void* __nullable                        inChangeInfo);

#pragma mark Property Operations

    /*!
        @method         HasProperty
        @abstract       Queries an object about whether or not it has the given property.
        @param          inDriver
                            The plug-in that owns the object.
        @param          inObjectID
                            The object to query.
        @param          inClientProcessID
                            A pid_t indicating the process of the client. Note that this will often
                            be zero indicating that the Host is making the request.
        @param          inAddress
                            An AudioObjectPropertyAddress indicating the property being queried.
        @result         A Boolean indicating whether or not the object has the given property.
    */
    Boolean
    (STDMETHODCALLTYPE *HasProperty)(   AudioServerPlugInDriverRef __nonnull    inDriver,
                                        AudioObjectID                           inObjectID,
                                        pid_t                                   inClientProcessID,
                                        const AudioObjectPropertyAddress*       inAddress);

    /*!
        @method         IsPropertySettable
        @abstract       Queries an object about whether or not the given property can be set.
        @param          inDriver
                            The plug-in that owns the object.
        @param          inObjectID
                            The object to query.
        @param          inClientProcessID
                            A pid_t indicating the process of the client. Note that this will often
                            be zero indicating that the Host is making the request.
        @param          inAddress
                            An AudioObjectPropertyAddress indicating the property being queried.
        @param          outIsSettable
                            A Boolean indicating whether or not the property can be set.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *IsPropertySettable)(    AudioServerPlugInDriverRef __nonnull    inDriver,
                                                AudioObjectID                           inObjectID,
                                                pid_t                                   inClientProcessID,
                                                const AudioObjectPropertyAddress*       inAddress,
                                                Boolean*                                outIsSettable);

    /*!
        @method         GetPropertyDataSize
        @abstract       Queries an object to find the size of the data for the given property.
        @param          inDriver
                            The plug-in that owns the object.
        @param          inObjectID
                            The object to query.
        @param          inAddress
                            An AudioObjectPropertyAddress indicating the property being queried.
        @param          inClientProcessID
                            A pid_t indicating the process of the client. Note that this will often
                            be zero indicating that the Host is making the request.
        @param          inQualifierDataSize
                            A UInt32 indicating the size of the buffer pointed to by
                            inQualifierData. Note that not all properties require qualification, in
                            which case this value will be 0.
        @param          inQualifierData,
                            A buffer of data to be used in determining the data of the property
                            being queried. Note that not all properties require qualification, in
                            which case this value will be NULL.
        @param          outDataSize
                            A UInt32 indicating how many bytes the data for the given property
                            occupies.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *GetPropertyDataSize)(   AudioServerPlugInDriverRef __nonnull    inDriver,
                                                AudioObjectID                           inObjectID,
                                                pid_t                                   inClientProcessID,
                                                const AudioObjectPropertyAddress*       inAddress,
                                                UInt32                                  inQualifierDataSize,
                                                const void* __nullable                  inQualifierData,
                                                UInt32*                                 outDataSize);

    /*!
        @method         GetPropertyData
        @abstract       Fetches the data of the given property and places it in the provided buffer.
        @param          inDriver
                            The plug-in that owns the object.
        @param          inObjectID
                            The object to query.
        @param          inClientProcessID
                            A pid_t indicating the process of the client. Note that this will often
                            be zero indicating that the Host is making the request.
        @param          inAddress
                            An AudioObjectPropertyAddress indicating the property being queried.
        @param          inQualifierDataSize
                            A UInt32 indicating the size of the buffer pointed to by
                            inQualifierData. Note that not all properties require qualification, in
                            which case this value will be 0.
        @param          inQualifierData,
                            A buffer of data to be used in determining the data of the property
                            being queried. Note that not all properties require qualification, in
                            which case this value will be NULL.
        @param          inDataSize
                            A UInt32 that indicates the size of the buffer pointed to by outData
        @param          outDataSize
                            A UInt32 which on exit indicates how much of the buffer pointed to by
                            outData was used.
        @param          outData
                            The buffer into which the data for the given property will be put.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *GetPropertyData)(   AudioServerPlugInDriverRef __nonnull    inDriver,
                                            AudioObjectID                           inObjectID,
                                            pid_t                                   inClientProcessID,
                                            const AudioObjectPropertyAddress*       inAddress,
                                            UInt32                                  inQualifierDataSize,
                                            const void* __nullable                  inQualifierData,
                                            UInt32                                  inDataSize,
                                            UInt32*                                 outDataSize,
                                            void*                                   outData);

    /*!
        @method         SetPropertyData
        @abstract       Tells an object to change the value of the given property.
        @discussion     Note that the value of the property cannot be considered set until the
                        appropriate notification has been received by the Host.
        @param          inDriver
                            The plug-in that owns the object.
        @param          inObjectID
                            The object to change.
        @param          inClientProcessID
                            A pid_t indicating the process of the client. Note that this will often
                            be zero indicating that the Host is making the request.
        @param          inAddress
                            An AudioObjectPropertyAddress indicating the property being changed.
        @param          inQualifierDataSize
                            A UInt32 indicating the size of the buffer pointed to by
                            inQualifierData. Note that not all properties require qualification, in
                            which case this value will be 0.
        @param          inQualifierData,
                            A buffer of data to be used in determining the data of the property
                            being queried. Note that not all properties require qualification, in
                            which case this value will be NULL.
        @param          inDataSize
                        A UInt32 indicating the size of the buffer pointed to by inData.
        @param          inData
                            The buffer containing the data for the property's value.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *SetPropertyData)(   AudioServerPlugInDriverRef __nonnull    inDriver,
                                            AudioObjectID                           inObjectID,
                                            pid_t                                   inClientProcessID,
                                            const AudioObjectPropertyAddress*       inAddress,
                                            UInt32                                  inQualifierDataSize,
                                            const void* __nullable                  inQualifierData,
                                            UInt32                                  inDataSize,
                                            const void*                             inData);

#pragma mark IO Operations

    /*!
        @method         StartIO
        @abstract       Tells the device to start IO.
        @discussion     This call is expect to always succeed or fail. The hardware can take as long
                        as necessary in this call such that it always either succeeds (and returns
                        0) or fails.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The device to start.
        @param          inClientID
                            The ID of the client making the request. This will have been established
                            with the device by a previous call to AddDeviceClient(). Note that the
                            device should consider IO running for as long as at least one client has
                            started IO.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *StartIO)(   AudioServerPlugInDriverRef __nonnull    inDriver,
                                    AudioObjectID                           inDeviceObjectID,
                                    UInt32                                  inClientID);

    /*!
        @method         StopIO
        @abstract       Tells the device to stop IO.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The device to stop.
        @param          inClientID
                            The ID of the client making the request. This will have been established
                            with the device by a previous call to AddDeviceClient().
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *StopIO)(    AudioServerPlugInDriverRef __nonnull    inDriver,
                                    AudioObjectID                           inDeviceObjectID,
                                    UInt32                                  inClientID);

    /*!
        @method         GetZeroTimeStamp
        @abstract       Retrieves the most recent zero time stamp for the device.
        @discussion     Devices that do not provide their own clock do not need to implement this call.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The device whose zero time stamp is being requested.
        @param          inClientID
                            The ID of the client making the request. This will have been established
                            with the device by a previous call to AddDeviceClient().
        @param          outSampleTime
                            Upon return, the sample time portion of the zero time stamp.
        @param          outHostTime
                            Upon return, the host time portion of the zero time stamp.
        @param          outSeed
                            Upon return, the current seed value for the device's clock. If
                            successive calls return different seed values, the caller will assume
                            that the device's clock has established a new time line and that the
                            caller should resynchronize to the new state.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *GetZeroTimeStamp)(  AudioServerPlugInDriverRef __nonnull    inDriver,
                                            AudioObjectID                           inDeviceObjectID,
                                            UInt32                                  inClientID,
                                            Float64*                                outSampleTime,
                                            UInt64*                                 outHostTime,
                                            UInt64*                                 outSeed);

    /*!
        @method         WillDoIOOperation
        @abstract       Asks the plug-in whether or not the device will perform the given phase of
                        the IO cycle for a particular device.
        @discussion     As part of starting IO, the Host will ask the plug-in whether or not the
                        device in question will perform the given IO operation. This method is not
                        called during the IO cycle. A device is allowed to do different sets of
                        operations for different clients.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device.
        @param          inClientID
                            The ID of the client doing the operation. This will have been
                            established with the device by a previous call to AddDeviceClient().
        @param          inOperationID
                            A UInt32 that identifies the operation being asked about. Constants for
                            the valid values of this argument are listed in the Constants section.
        @param          outWillDo
                            Upon return, a Boolean indicating whether or not the device will perform
                            the given operation.
        @param          outWillDoInPlace
                            Upon return, a Boolean where true indicates that the device will perform
                            the requested operation entirely within the main buffer passed to the
                            DoIOOperation routine. If this value is false, it indicates that the
                            device requires that the secondary buffer be passed.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *WillDoIOOperation)( AudioServerPlugInDriverRef __nonnull    inDriver,
                                            AudioObjectID                           inDeviceObjectID,
                                            UInt32                                  inClientID,
                                            UInt32                                  inOperationID,
                                            Boolean*                                outWillDo,
                                            Boolean*                                outWillDoInPlace);

    /*!
        @method         BeginIOOperation
        @abstract       Tells the plug-in that the Host is about to begin a phase of the IO cycle for a
                        particular device.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device.
        @param          inClientID
                            The ID of the client doing the operation. This will have been
                            established with the device by a previous call to AddDeviceClient().
        @param          inOperationID
                            A UInt32 that identifies the operation being performed. Constants for
                            the valid values of this argument are listed in the Constants section.
        @param          inIOBufferFrameSize
                            The number of sample frames that will be processed in this operation.
                            Note that for some operations, this will be different than the nominal
                            buffer frame size.
        @param          inIOCycleInfo
                            The basic information about the current IO cycle.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *BeginIOOperation)(  AudioServerPlugInDriverRef __nonnull    inDriver,
                                            AudioObjectID                           inDeviceObjectID,
                                            UInt32                                  inClientID,
                                            UInt32                                  inOperationID,
                                            UInt32                                  inIOBufferFrameSize,
                                            const AudioServerPlugInIOCycleInfo*     inIOCycleInfo);

    /*!
        @method         DoIOOperation
        @abstract       Tells the device to perform an IO operation for a particular stream.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device.
        @param          inStreamObjectID
                            The ID of the stream whose data is being processed.
        @param          inClientID
                            The ID of the client doing the operation. This will have been
                            established with the device by a previous call to AddDeviceClient().
        @param          inOperationID
                            A UInt32 that identifies the operation being performed. Constants for
                            the valid values of this argument are listed in the Constants section.
        @param          inIOBufferFrameSize
                            The number of sample frames that will be processed in this operation.
                            Note that for some operations, this will be different than the nominal
                            buffer frame size.
        @param          inIOCycleInfo
                            The basic information about the current IO cycle.
        @param          ioMainBuffer
                            The primary buffer for the data for the operation. If the device
                            signaled through WillDoIOOperation() that the operation will be handled
                            in-place, this will be the only buffer passed in.
        @param          ioSecondaryBuffer
                            The secondary buffer for performing the operation. If the device
                            signaled through WillDoIOOperation() that the operation will not be
                            handled in place, the results of the operation must end up in this
                            buffer.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *DoIOOperation)( AudioServerPlugInDriverRef __nonnull    inDriver,
                                        AudioObjectID                           inDeviceObjectID,
                                        AudioObjectID                           inStreamObjectID,
                                        UInt32                                  inClientID,
                                        UInt32                                  inOperationID,
                                        UInt32                                  inIOBufferFrameSize,
                                        const AudioServerPlugInIOCycleInfo*     inIOCycleInfo,
                                        void* __nullable                        ioMainBuffer,
                                        void* __nullable                        ioSecondaryBuffer);

    /*!
        @method         EndIOOperation
        @abstract       Tells the plug-in that the Host is about to end a phase of the IO cycle for
                        a particular device.
        @param          inDriver
                            The plug-in that owns the device.
        @param          inDeviceObjectID
                            The ID of the device.
        @param          inClientID
                            The ID of the client doing the operation. This will have been
                            established with the device by a previous call to AddDeviceClient().
        @param          inOperationID
                            A UInt32 that identifies the operation being performed. Constants for
                            the valid values of this argument are listed in the Constants section.
        @param          inIOBufferFrameSize
                            The number of sample frames that will be processed in this operation.
                            Note that for some operations, this will be different than the nominal
                            buffer frame size.
        @param          inIOCycleInfo
                            The basic information about the current IO cycle.
        @result         An OSStatus indicating success or failure.
    */
    OSStatus
    (STDMETHODCALLTYPE *EndIOOperation)(    AudioServerPlugInDriverRef __nonnull    inDriver,
                                            AudioObjectID                           inDeviceObjectID,
                                            UInt32                                  inClientID,
                                            UInt32                                  inOperationID,
                                            UInt32                                  inIOBufferFrameSize,
                                            const AudioServerPlugInIOCycleInfo*     inIOCycleInfo);

};

/*!
    @defined        kAudioServerPlugInDriverInterfaceUUID
    @abstract       The UUID of the driver interface (EEA5773D-CC43-49F1-8E00-8F96E7D23B17).
*/
#define kAudioServerPlugInDriverInterfaceUUID   CFUUIDGetConstantUUIDWithBytes(NULL, 0xEE, 0xA5, 0x77, 0x3D, 0xCC, 0x43, 0x49, 0xF1, 0x8E, 0x00, 0x8F, 0x96, 0xE7, 0xD2, 0x3B, 0x17)

//==================================================================================================

#if defined(__cplusplus)
}
#endif

CF_ASSUME_NONNULL_END

#endif  //  CoreAudio_AudioServerPlugIn_h
