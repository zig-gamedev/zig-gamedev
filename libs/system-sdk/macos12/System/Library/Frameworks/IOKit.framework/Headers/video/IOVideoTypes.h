/*
    File:       IOVideoTypes.h
    
    Contains:   

    Copyright:  © 2006-2014 by Apple Inc., all rights reserved.
*/

#if !defined(__IOVideoTypes_h__)
#define __IOVideoTypes_h__

//  System Includes
#include <IOKit/IOTypes.h>
#include <mach/message.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/*!
    @struct         IOVideoDeviceNotification
    @abstract       This structure contains an individual notification from the driver.
    @field          mObjectID
                        The ID of the object to which the notification pertains.
    @field          mNotificationID
                        A UInt32 that identifies the kind of the notification.
    @field          mNotificationArgument1
                        A UInt32 whose usage depends on the the specific kind of notification.
    @field          mNotificationArgument2
                        A UInt32 whose usage depends on the the specific kind of notification.
    @field          mNotificationArgument3
                        A UInt64 whose usage depends on the the specific kind of notification.
    @field          mNotificationArgument4
                        A UInt64 whose usage depends on the the specific kind of notification.
*/
struct IOVideoDeviceNotification
{
    UInt32  mObjectID;
    UInt32  mNotificationID;
    UInt32  mNotificationArgument1;
    UInt32  mNotificationArgument2;
    UInt64  mNotificationArgument3;
    UInt64  mNotificationArgument4;
};
typedef struct IOVideoDeviceNotification IOVideoDeviceNotification;

/*!
    @struct         IOVideoDeviceNotificationMessage
    @abstract       This structure describes a notification from the driver. Note that the message can contain multiple notifications.
    @field          mMessageHeader
                        The mach message header.
    @field          mClientData
                        The client data that was registered with the mach port.
    @field          mNumberNotifications
                        The number of IOVideoDeviceNotifications in the mNotifications array.
    @field          mNotifications
                        A variable length array of IOVideoDeviceNotification structures that carry the actual notification data. The number of elements in this array is denoted by
                        mNumberNotifications, but can also be inferred from the message size in the mach message header.
*/
struct IOVideoDeviceNotificationMessage
{
    mach_msg_header_t           mMessageHeader;
    UInt32                      mClientData;
    UInt32                      mNumberNotifications;
    IOVideoDeviceNotification   mNotifications[1];
};
typedef struct IOVideoDeviceNotificationMessage IOVideoDeviceNotificationMessage;

#define CalculateIOVideoDeviceNotificationMessageSize(numberNotifications) (sizeof(IOVideoDeviceNotificationMessage) + (((numberNotifications) - 1) * sizeof(IOVideoDeviceNotification)))

#pragma mark Notification IDs
/*!
    @enum           Notification IDs
    @discussion
    @abstract       The four char codes used to identify the kind of the notification.
    @discussion     All device-level notifications will have an object ID of 0.
    @constant       kIOVideoDeviceNotificationID_ControlValueChanged
                        Indicates that the value of the control with the given ID has changed. The first argument is the new value.
    @constant       kIOVideoDeviceNotificationID_ControlRangeChanged
                        Indicates that the range of the control with the given ID has changed.
*/
enum
{
    kIOVideoDeviceNotificationID_ControlValueChanged = 'cval',
    kIOVideoDeviceNotificationID_ControlRangeChanged = 'crng'
};

struct IOVideoStreamDescription
{
    UInt32          mVideoCodecType;
    UInt32          mVideoCodecFlags;
    UInt32          mWidth;
    UInt32          mHeight;
    UInt32          mReserved1;
    UInt32          mReserved2;
};
typedef struct IOVideoStreamDescription  IOVideoStreamDescription;

//==================================================================================================
#pragma mark Control Constants

/*!
    @enum           Control Constants
    @discussion
    @abstract       Various constants related to controls.
    @constant       kIOVideoControlScopeGlobal
                        The scope for controls that apply to the device as a whole.
    @constant       kIOVideoControlScopeInput
                        The scope for controls that apply to the input section of the device.
    @constant       kIOVideoControlScopeOutput
                        The scope for controls that apply to the output section of the device.
    @constant       kIOVideoControlScopePlayThrough
                        The scope for controls that apply to the play through section of the device.
    @constant       kIOVideoControlElementMaster
                        The element value for controls that apply to the master element or to the entire scope. Note that other elements are numbered consecutively starting from 1.
*/
enum
{
    kIOVideoControlScopeGlobal      = 'glob',
    kIOVideoControlScopeInput       = 'inpt',
    kIOVideoControlScopeOutput      = 'outp',
    kIOVideoControlScopePlayThrough = 'ptru',
    kIOVideoControlElementMaster    = 0
};
    
/*!
    @enum           Control Base Class IDs
    @discussion
    @abstract       The class IDs that identify the various control base classes.
    @constant       kIOVideoControlBaseClassIDBoolean
                        The class ID that identifies the boolean control class which is a subclass of the base control class. Boolean controls manipulate on/off switches in the hardware.
    @constant       kIOVideoControlBaseClassIDSelector
                        The class ID that identifies the selector control class which is a subclass of the base control class. Selector controls manipulate controls that have
                        multiple, but discreet values.
    @constant       kIOVideoControlBaseClassIDFeature
                        The class ID that identifies the feature control class which is a subclass of the base control class. Feature controls manipulate various features that might be
                        present on a device, such as hue, saturation, zoom, etc.
*/
enum
{
    kIOVideoControlBaseClassIDBoolean   = 'togl',
    kIOVideoControlBaseClassIDSelector  = 'slct',   
    kIOVideoControlBaseClassIDFeature   = 'ftct'
    
};

/*!
    @enum           IOVideoBooleanControl Subclass IDs
    @discussion
    @abstract       The four char codes that identify the various standard subclasses of IOVideoBooleanControl.
    @constant       kIOVideoBooleanControlClassIDJack 
                        A IOVideoBooleanControl where a true value means something is plugged into that element.
    @constant       kIOVideoBooleanControlClassIDDirection
                        A IOVideoBooleanControl where a true value means the element is operating in input mode, and false means the element is operating in output mode.
                        This control is only needed for devices which can do input and output, but not at the same time.
*/
enum
{
    kIOVideoBooleanControlClassIDJack       = 'jack',
    kIOVideoBooleanControlClassIDDirection  = 'dire'
};

/*!
    @enum           IOVideoSelectorControl Subclass IDs
    @discussion
    @abstract       The four char codes that identify the various standard subclasses of IOVideoSelectorControl.
    @constant       kIOVideoSelectorControlClassIDDataSource
                        A IOVideoSelectorControl that identifies where the data for the element is coming from.
    @constant       kIOVideoSelectorControlClassIDDataDestination
                        A IOVideoSelectorControl that identifies where the data for the element is going.
*/
enum
{
    kIOVideoSelectorControlClassIDDataSource        = 'dsrc',
    kIOVideoSelectorControlClassIDDataDestination   = 'dest'
};

/*!
    @enum           IOVideoFeatureControl Subclass IDs
    @discussion
    @abstract       The four char codes that identify the various standard subclasses of IOVideoFeatureControl.
    @constant       kIOVideoFeatureControlClassIDBlackLevel
                        A IOVideoFeatureControl that controls the black level offset. The units for the control's absolute value are percetage (%).
    @constant       kIOVideoFeatureControlClassIDWhiteLevel
                        A IOVideoFeatureControl that controls the white level offset. The units for the control's absolute value are percentage (%).
    @constant       kIOVideoFeatureControlClassIDHue
                        A IOVideoFeatureControl that controls the hue offset. Positive values mean counterclockwise, negative values means clockwise on a vector scope. The units for the
                        control's absolute value are degrees (°).
    @constant       kIOVideoFeatureControlClassIDSaturation
                        A IOVideoFeatureControl that controls color intensity. For example, at high saturation levels, red appears to be red; at low saturation, red appears as pink. The
                        unit for the control's absolute value is a percentage (%).
    @constant       kIOVideoFeatureControlClassIDContrast
                        A IOVideoFeatureControl that controls a the distance bewtween the whitest whites and blackest blacks. The units for the control's absolute value are percentage (%).
    @constant       kIOVideoFeatureControlClassIDSharpness
                        A IOVideoFeatureControl that controls the sharpness of the picture. The units for the control's absolute value are undefined.
    @constant       kIOVideoFeatureControlClassIDBrightness
                        A IOVideoFeatureControl that controls the intensity of the video level. The units for the control's absolute value are percetage (%).
    @constant       kIOVideoFeatureControlClassIDGain
                        A IOVideoFeatureControl that controls the amplification of the signal. The units for the control's absolute value are decibels (dB).
    @constant       kIOVideoFeatureControlClassIDIris
                        A IOVideoFeatureControl that controls a mechanical lens iris. The units for the control's absolute value are an F number (F).
    @constant       kIOVideoFeatureControlClassIDShutter
                        A IOVideoFeatureControl that controls the integration time of the incoming light. The units for the control's absolute value are seconds (s).
    @constant       kIOVideoFeatureControlClassIDExposure
                        A IOVideoFeatureControl that controls a the total amount of light accumulated. The units for the control's absolute value are exposure value (EV).
    @constant       kIOVideoFeatureControlClassIDWhiteBalanceU
                        A IOVideoFeatureControl that controls the adjustment of the white color of the picture. The units for the control's absolute value are kelvin (K).
    @constant       kIOVideoFeatureControlClassIDWhiteBalanceV
                        A IOVideoFeatureControl that controls a adjustment of the white color of the picture. The units for the control's absolute value are kelvin (K).
    @constant       kIOVideoFeatureControlClassIDGamma
                        A IOVideoFeatureControl that defines the function between incoming light level and output picture level. The units for the control's absolute value are undefined.
    @constant       kIOVideoFeatureControlClassIDTemperature
                        A IOVideoFeatureControl that controls the temperature inside of the device and/or controlling temperature. The units for the control's absolute value are undefined.
    @constant       kIOVideoFeatureControlClassIDZoom
                        A IOVideoFeatureControl that controls the zoom. The units for the control's absolute value are power where 1 is the wide end.
    @constant       kIOVideoFeatureControlClassIDFocus
                        A IOVideoFeatureControl that controls a focus mechanism. The units for the control's absolute value are meters (m).
    @constant       kIOVideoFeatureControlClassIDPan
                        A IOVideoFeatureControl that controls a panning mechanism. Positive values mean clockwise, negative values means counterclockwise. The units for the control's
                        absolute value are degrees (°).
    @constant       kIOVideoFeatureControlClassIDTilt
                        A IOVideoFeatureControl that controls a tilt mechanism. Positive values mean upwards, negative values means downwards. The units for the control's absolute value are
                        degrees (°).
    @constant       kIOVideoFeatureControlClassIDOpticalFilter
                        A IOVideoFeatureControl that controls changing the optical filter of camera lens function. The units for the control's absolute value are are undefined.
    @constant       kIOVideoFeatureControlClassIDBacklightCompensation
                        A IOVideoFeatureControl that controls the amount of backlight compensation to apply. A low number indicates the least amount of backlight compensation. The units for the
                        control's absolute value are are undefined.
    @constant       kIOVideoFeatureControlClassIDPowerLineFrequency
                        A IOVideoFeatureControl to specify the power line frequency to properly implement anti-flicker processing. The units for the contorl's absolute value are hertz (Hz).
*/
enum
{
    kIOVideoFeatureControlClassIDBlackLevel             = 'bklv',
    kIOVideoFeatureControlClassIDWhiteLevel             = 'whlv',
    kIOVideoFeatureControlClassIDHue                    = 'hue ',
    kIOVideoFeatureControlClassIDSaturation             = 'satu',
    kIOVideoFeatureControlClassIDContrast               = 'ctst',
    kIOVideoFeatureControlClassIDSharpness              = 'shrp',
    kIOVideoFeatureControlClassIDBrightness             = 'brit',
    kIOVideoFeatureControlClassIDGain                   = 'gain',
    kIOVideoFeatureControlClassIDIris                   = 'iris',
    kIOVideoFeatureControlClassIDShutter                = 'shtr',
    kIOVideoFeatureControlClassIDExposure               = 'xpsr',
    kIOVideoFeatureControlClassIDWhiteBalanceU          = 'whbu',
    kIOVideoFeatureControlClassIDWhiteBalanceV          = 'whbv',
    kIOVideoFeatureControlClassIDGamma                  = 'gmma',
    kIOVideoFeatureControlClassIDTemperature            = 'temp',
    kIOVideoFeatureControlClassIDZoom                   = 'zoom',
    kIOVideoFeatureControlClassIDFocus                  = 'fcus',
    kIOVideoFeatureControlClassIDPan                    = 'pan ',
    kIOVideoFeatureControlClassIDTilt                   = 'tilt',
    kIOVideoFeatureControlClassIDOpticalFilter          = 'opft',
    kIOVideoFeatureControlClassIDBacklightCompensation  = 'bklt',
    kIOVideoFeatureControlClassIDPowerLineFrequency     = 'pwfq'
};

//==================================================================================================
#pragma mark    IORegistry Keys

// IOVideo Class Names
#define kIOVideoDevice_ClassName                                "IOVideoDevice"

// IOVideoDevice IORegistry Keys
#pragma mark IOVideoDevice IORegistry Keys
#define kIOVideoDeviceKey_DeviceName                            "device name"
#define kIOVideoDeviceKey_DeviceManufacturer                    "device manufacturer"
#define kIOVideoDeviceKey_DeviceUID                             "device UID"
#define kIOVideoDeviceKey_IOEngineIsRunning                     "is running"
#define kIOVideoDeviceKey_InputLatency                          "input latency"
#define kIOVideoDeviceKey_OutputLatency                         "output latency"
#define kIOVideoDeviceKey_InputStreamList                       "input streams"
#define kIOVideoDeviceKey_OutputStreamList                      "output streams"
#define kIOVideoDeviceKey_ControlList                           "controls"

// IOVideoDevice Stream Dictionary Keys
#pragma mark IOVideoDevice Stream Dictionary Keys
#define kIOVideoStreamKey_StreamID                              "stream ID"
#define kIOVideoStreamKey_StartingDeviceChannelNumber           "starting channel"
#define kIOVideoStreamKey_BufferMappingOptions                  "buffer mapping options"
#define kIOVideoStreamKey_CurrentFormat                         "current format"
#define kIOVideoStreamKey_AvailableFormats                      "available formats"

// IOVideoDevice Control Dictionary Keys
#pragma mark IOVideoDevice Control Dictionary Keys
#define kIOVideoControlKey_ControlID                            "control ID"
#define kIOVideoControlKey_BaseClass                            "base class"
#define kIOVideoControlKey_Class                                "class"
#define kIOVideoControlKey_Scope                                "scope"
#define kIOVideoControlKey_Element                              "element"
#define kIOVideoControlKey_IsReadOnly                           "read only"
#define kIOVideoControlKey_Variant                              "variant"
#define kIOVideoControlKey_Name                                 "name"
#define kIOVideoControlKey_Value                                "value"

// IOVideoDevice Selector Control Dictionary Keys
#pragma mark IOVideoDevice Selector Control Dictionary Keys
#define kIOVideoSelectorControlKey_SelectorMap                  "selectors"

// IOVideoDevice Selector Control Selector Map Item Dictionary Keys
#pragma mark IOVideoDevice Selector Control Selector Map Item Dictionary Keys
#define kIOVideoSelectorControlSelectorMapItemKey_Value         "value"
#define kIOVideoSelectorControlSelectorMapItemKey_Name          "name"
#define kIOVideoSelectorControlSelectorMapItemKey_Kind          "kind"

// Stream Format Dictionary Keys
#pragma mark Stream Format Dictionary Keys
#define kIOVideoStreamFormatKey_CodecType                       "codec type"
#define kIOVideoStreamFormatKey_CodecFlags                      "codec flags"
#define kIOVideoStreamFormatKey_Width                           "width"
#define kIOVideoStreamFormatKey_Height                          "height"


//==================================================================================================

#if defined(__cplusplus)
}
#endif

#endif
