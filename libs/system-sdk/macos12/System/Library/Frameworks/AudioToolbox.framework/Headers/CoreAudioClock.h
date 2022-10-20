/*!
	@file		CoreAudioClock.h
	@framework	AudioToolbox.framework
	@copyright	(c) 2004-2015 by Apple, Inc., all rights reserved.
	@abstract	Services for audio and MIDI-related synchronization and
				time unit conversions.
*/

#ifndef AudioToolbox_CoreAudioClock_h
#define AudioToolbox_CoreAudioClock_h

//==================================================================================================

#include <TargetConditionals.h>
#if TARGET_OS_OSX
#include <Availability.h>
#include <CoreAudioTypes/CoreAudioTypes.h>
#include <AudioToolbox/MusicPlayer.h>

CF_ASSUME_NONNULL_BEGIN

//==================================================================================================

/*!
	@enum		CAClockPropertyID
	
	@abstract	The properties of a CoreAudioClock, accessible via CAClockGetProperty and
				CAClockSetProperty.

	@constant	kCAClockProperty_InternalTimebase
					Type: CAClockTimebase. Selects the internal time reference for the clock
					(currently, host time, an audio device, or audio output unit).
	@constant	kCAClockProperty_TimebaseSource
					Type: according to the internal timebase. If the timebase is
					kCAClockTimebase_AudioDevice, the value is an AudioDeviceID. For
					kCAClockTimebase_AudioOutputUnit, the value is an AudioUnit.
	@constant	kCAClockProperty_SyncMode
					Type: CAClockSyncMode. Selects between internal synchronization and multiple
					external synchronization modes such as MIDI Time Code and MIDI beat clock.
	@constant	kCAClockProperty_SyncSource
					Type: according to the sync mode. For kCAClockSyncMode_MIDIClockTransport or 
					kCAClockSyncMode_MTCTransport, the value is a MIDIEndpointRef.
	@constant	kCAClockProperty_SMPTEFormat
					Type: CAClockSMPTEFormat. Specifies the SMPTE format (fps, drop or non-drop)
					expected for received SMPTE messages, and used for transmitted SMPTE
					messages and SMPTE time representations.
	@constant	kCAClockProperty_SMPTEOffset
					Type: CAClockSeconds. The SMPTE time, represented in seconds since
					00:00:00:00, corresponding to a timeline position of 0 seconds.
	@constant	kCAClockProperty_MIDIClockDestinations
					Type: array of MIDIEndpointRef. When non-empty, the clock will transmit
					MIDI beat clock to the MIDI endpoints in this list. (As of macOS 10.6,
					the endpoints may be virtual sources. Previously, they had to be destinations.)
	@constant	kCAClockProperty_MTCDestinations
					Type: array of MIDIEndpointRef. When non-empty, the clock will transmit
					MIDI Time Code to the MIDI endpoints in this list. (As of macOS 10.6,
					the endpoints may be virtual sources. Previously, they had to be destinations.)
	@constant	kCAClockProperty_MTCFreewheelTime
					Type: CAClockSeconds. When the sync mode is MIDI Time Code, this controls
					how long the reader will keep running after the last MTC message is received
					before stopping (default: 1 sec). Should be at least ~4 SMPTE frames in
					duration.
	@constant	kCAClockProperty_TempoMap
					Type: array of CATempoMapEntry. Specifies a tempo map for the clock,
					defining the relationship between timeline positions in seconds and musical
					beats. Defaults to a constant tempo of 120 bpm.
	@constant	kCAClockProperty_MeterTrack
					Type: array of CAMeterTrackEntry. Specifies the positions of musical time 
					signature changes in the timeline. Used only for converting between beats
					and CABarBeatTime's (a display representation of a beat time).
	@constant	kCAClockProperty_Name
					Type: CFStringRef. Sets a name for the clock. When a client sets the
					property, the clock retains a reference to the string. When a client gets
					the property, it receives a borrowed reference (i.e. the client must not
					release the string).
	@constant	kCAClockProperty_SendMIDISPP
					Type: UInt32. Specifies whether MIDI Song Position Pointer messages are
					sent to the clock's MIDI clock destinations (if any). Available starting
					in macOS 10.6.
*/
typedef CF_ENUM(UInt32, CAClockPropertyID) {
	kCAClockProperty_InternalTimebase   = 'intb',
	kCAClockProperty_TimebaseSource		= 'itbs',
	kCAClockProperty_SyncMode			= 'synm',
	kCAClockProperty_SyncSource			= 'syns',
	kCAClockProperty_SMPTEFormat		= 'smpf',
	kCAClockProperty_SMPTEOffset		= 'smpo',
	kCAClockProperty_MIDIClockDestinations = 'mbcd',
	kCAClockProperty_MTCDestinations	= 'mtcd',
	kCAClockProperty_MTCFreewheelTime   = 'mtfw',
	kCAClockProperty_TempoMap			= 'tmpo',
	kCAClockProperty_MeterTrack			= 'metr',
	kCAClockProperty_Name				= 'name',
	kCAClockProperty_SendMIDISPP		= 'mspp'
};

/*!
	@enum		CAClockTimebase
	
	@abstract	The available internal hardware time references for a clock.
	
	@constant	kCAClockTimebase_HostTime
					The clock's reference time is host time (as returned
					by <code>mach_absolute_time()</code> or
					<code>HostTime()</code>).
	@constant	kCAClockTimebase_AudioDevice
					The clock's reference time is derived from an audio
					device.
	@constant	kCAClockTimebase_AudioOutputUnit
					The clock's reference time is derived from the audio
					device addressed by an output Audio Unit.
*/
typedef CF_ENUM(UInt32, CAClockTimebase) {
	kCAClockTimebase_HostTime			= 'host',
	kCAClockTimebase_AudioDevice		= 'audi',
	kCAClockTimebase_AudioOutputUnit	= 'auou'
};

/*!
	@enum		CAClockSyncMode
	
	@abstract	Specifies internal synchronization, or an external sync source type.
	
	@constant	kCAClockSyncMode_Internal
					The clock is not driven by an external sync source.
	@constant	kCAClockSyncMode_MIDIClockTransport
					The clock is driven by MIDI beat clock received from a CoreMIDI source
					endpoint.
	@constant	kCAClockSyncMode_MTCTransport
					The clock is driven by MIDI Time Code received from a CoreMIDI source
					endpoint.
*/
typedef CF_ENUM(UInt32, CAClockSyncMode) {
	kCAClockSyncMode_Internal			= 'intr',   // sync to the internal timebase
	kCAClockSyncMode_MIDIClockTransport = 'mclk',	// sync to MIDI beat clock, external transport
	kCAClockSyncMode_MTCTransport		= 'mmtc'	// sync to MIDI Time Code, external transport
};


/* From CoreAudioTypes.h:
	enum {
		kSMPTETimeType24		= 0,
		kSMPTETimeType25		= 1,
		kSMPTETimeType30Drop	= 2,
		kSMPTETimeType30		= 3,
		kSMPTETimeType2997		= 4,
		kSMPTETimeType2997Drop	= 5
	};
*/

// we can remove this after the SDK's have the real declaration.
#if !defined(COREAUDIOTYPES_VERSION) || COREAUDIOTYPES_VERSION <= 1051
#define SMPTETimeType UInt32
#endif

/*!
	@typedef		CAClockSMPTEFormat
	@abstract		A SMPTE format, specifying the frames per second (fps) and
					whether it is drop frame.

	The possible values of a CAClockSMPTEFormat are found in <CoreAudioTypes/CoreAudioTypes.h>.
	Values include kSMPTETimeType30, kSMPTETimeType30Drop, etc. Note that formats with more than 30
	fps are not usable with MIDI Time Code.
*/
typedef SMPTETimeType		CAClockSMPTEFormat;

/*!
	@enum		CAClockMessage
	@abstract	The messages sent to a CAClockListenerProc to notify the client of
				changes to the clock's state.
	
	@constant	kCAClockMessage_StartTimeSet
					A new start time was set or received from an external sync source.
	@constant	kCAClockMessage_Started
					The clock's time has started moving.
	@constant	kCAClockMessage_Stopped
					The clock's time has stopped moving.
	@constant	kCAClockMessage_Armed
					The client has called CAClockArm().
	@constant	kCAClockMessage_Disarmed
					The client has called CAClockDisarm().
	@constant	kCAClockMessage_PropertyChanged
					A clock property has been changed.
	@constant	kCAClockMessage_WrongSMPTEFormat
					The clock is receiving SMPTE (MTC) messages in a SMPTE format that does not
					match the clock's SMPTE format.
*/
typedef CF_ENUM(UInt32, CAClockMessage) {
	kCAClockMessage_StartTimeSet		= 'stim',
	kCAClockMessage_Started				= 'strt',
	kCAClockMessage_Stopped				= 'stop',
	kCAClockMessage_Armed				= 'armd',
	kCAClockMessage_Disarmed			= 'darm',
	kCAClockMessage_PropertyChanged		= 'pchg',
	kCAClockMessage_WrongSMPTEFormat	= '?smp'
};


/*!
	@enum		CAClockTimeFormat
	@abstract	The various units in which a clock can represent and report time.
	
	@constant	kCAClockTimeFormat_HostTime
					Absolute host time, as returned by <code>mach_absolute_time()</code>.
	@constant	kCAClockTimeFormat_Samples
					Absolute audio samples, as a Float64. Available when the internal timebase
					is an audio device (or audio output unit). The units are in arbitrary sample
					numbers, corresponding to the audio device's current time, and at the
					device's current sample rate.
	@constant	kCAClockTimeFormat_Beats
					Musical beats, as a Float64. This is a position on the clock's timeline.
	@constant	kCAClockTimeFormat_Seconds
					Seconds, as a Float64. This is a position on the clock's timeline.
	@constant	kCAClockTimeFormat_SMPTESeconds
					Seconds, as a Float64. This is the same as kCAClockTimeFormat_Seconds,
					except that the clock's SMPTE offset has been applied.
	@constant	kCAClockTimeFormat_SMPTETime
					SMPTETime structure.
*/
typedef CF_ENUM(UInt32, CAClockTimeFormat) {
	kCAClockTimeFormat_HostTime			= 'host',
	kCAClockTimeFormat_Samples			= 'samp',
	kCAClockTimeFormat_Beats			= 'beat',   // relative position on media timeline
	kCAClockTimeFormat_Seconds			= 'secs',   // relative position on media timeline
	kCAClockTimeFormat_SMPTESeconds		= 'smps',	// absolute SMPTE position in seconds
	kCAClockTimeFormat_SMPTETime		= 'smpt',	// SMPTETime structure
	kCAClockTimeFormat_AbsoluteSeconds	= 'asec'
};

CF_ENUM(OSStatus) {  // errors
	kCAClock_UnknownPropertyError		= -66816,
	kCAClock_InvalidPropertySizeError   = -66815,
	kCAClock_InvalidTimeFormatError		= -66814,
	kCAClock_InvalidSyncModeError		= -66813,
	kCAClock_InvalidSyncSourceError		= -66812,
	kCAClock_InvalidTimebaseError		= -66811,
	kCAClock_InvalidTimebaseSourceError = -66810,
	kCAClock_InvalidSMPTEFormatError	= -66809,
	kCAClock_InvalidSMPTEOffsetError	= -66808,
	kCAClock_InvalidUnitError			= -66807,	// e.g. asking for audio samples when sync source 
													// not associated with an audio device
	kCAClock_InvalidPlayRateError		= -66806,
	kCAClock_CannotSetTimeError			= -66805	// can't SetCurrentTime unless stopped
													// or can't SetCurrentTempo unless running
};

//==================================================================================================

/*!
	@typedef	CAClockRef
	@abstract	A reference to a Core Audio Clock object.
*/
typedef struct OpaqueCAClock *	CAClockRef;
/*!
	@typedef	CAClockBeats
	@abstract	MIDI quarter notes (see MIDI specs)
*/
typedef Float64					CAClockBeats;
/*!
	@typedef	CAClockTempo
	@abstract	A musical tempo in beats per minute.
*/
typedef Float64					CAClockTempo;	// beats per minute
/*
	@typedef	CAClockSamples
	@abstract	Audio samples.
*/
typedef Float64					CAClockSamples;
/*
	@typedef	CAClockSeconds
	@abstract	Seconds.
*/
typedef Float64					CAClockSeconds;

struct MIDIPacketList;

//==================================================================================================

/*!
	@typedef	CAClockListenerProc
	
	@abstract	A client-supplied function called when the clock's state changes.
	
	@param	userData
				The value passed to CAClockAddListener when the callback function
				was installed.
	@param	message
				Signifies the kind of event which occurred.
	@param	param
				This value is specific to the message (currently no messages have values).
*/
typedef void (*CAClockListenerProc)(void *userData, CAClockMessage message, const void *param);

/*!
	@struct 	CAClockTime
	
	@abstract	Represents a time value using one of several possible units.
	
	@var  	format
				Specifies the time's format and units.
	@var  	reserved
				Must be 0.
	@var  	time
				The time value. Use the member of the union appropriate to the format (see the
				description of CAClockTimeFormat).
*/
struct CAClockTime {
	CAClockTimeFormat	format;
	UInt32				reserved;
	union {
		UInt64			hostTime;
		CAClockSamples	samples;
		CAClockBeats	beats;
		CAClockSeconds	seconds;
		SMPTETime		smpte;
	}					time;
};
typedef struct CAClockTime CAClockTime;

/*!
	@struct 	CATempoMapEntry
	@abstract	A tempo change event.
	
	The clock's tempo map defines the correspondence between seconds and musical
	beats, and is used in conversions between the two.

	@var  	beats
				The beat time at which the tempo changes.
	@var  	tempoBPM
				The new tempo as of that time.
*/
struct CATempoMapEntry {
	CAClockBeats		beats;
	CAClockTempo		tempoBPM;			// beats per minute
};
typedef struct CATempoMapEntry CATempoMapEntry;

/*!
	@struct		CAMeterTrackEntry
	@abstract	A time signature change event.
	
	The meter track is used for converting between beats as floating-point
	numbers (CAClockBeats) and their display representations (CABarBeatTime).

	@var  	beats
				The beat time at which the time signature (meter) changes.
	@var  	meterNumer
				The numerator of the new time signature.
	@var  	meterDenom
				The denominator of the new time signature (1, 2, 4, 8, etc.).

*/
struct CAMeterTrackEntry {
	CAClockBeats		beats;
	UInt16				meterNumer;
	UInt16				meterDenom;			// 1,2,4,8 etc.
};
typedef struct CAMeterTrackEntry CAMeterTrackEntry;

//==================================================================================================

#ifdef __cplusplus
extern "C" {
#endif

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockNew
	
	@abstract		Create a new clock object.
	
	@param			inReservedFlags
						Must be 0.
	
	@param			outCAClock
						Must be non-null. On successful return, the new clock object.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockNew(	UInt32					 				inReservedFlags,
			CAClockRef __nullable * __nonnull		outCAClock)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockDispose
	
	@abstract		Dispose a clock object.
		
	@param			inCAClock
						The clock object to be disposed.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockDispose(				CAClockRef 			inCAClock)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockGetPropertyInfo
	
	@abstract		Gets information about a clock's property.
		
	@param			inCAClock
						The clock object.
	
	@param			inPropertyID
						The property being queried.
	
	@param			outSize
						If non-null, on exit, this is set to the size of the
						property's value.

	@param			outWritable
						If non-null, on exit, this indicates whether the
						property value is settable.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockGetPropertyInfo(		CAClockRef				inCAClock,
							CAClockPropertyID		inPropertyID,
							UInt32 * __nullable		outSize,
							Boolean * __nullable	outWritable)				API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockGetProperty
	
	@abstract		Gets the current value of a clock's property.
	
	@param			inCAClock
						The clock object.
	
	@param			inPropertyID
						The property being fetched.
	
	@param			ioPropertyDataSize
						On entry, the size (in bytes) of the memory pointed to
						by outPropertyData. On exit, the actual size of the
						property data returned.
	
	@param			outPropertyData
						The value of the property is copied to the memory
						this points to.
	
	@result			An OSStatus error code.
*/

extern OSStatus
CAClockGetProperty(			CAClockRef			inCAClock,
							CAClockPropertyID	inPropertyID,
							UInt32 *			ioPropertyDataSize,
							void *				outPropertyData)			API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockSetProperty
	
	@abstract		Changes the value of a clock's property.
	
	@param			inCAClock
						The clock object.
	
	@param			inPropertyID
						The property being set.
	
	@param			inPropertyDataSize
						The size of the property data, in bytes.
	
	@param			inPropertyData
						Points to the property's new value.

	@result			An OSStatus error code.
*/
extern OSStatus
CAClockSetProperty(			CAClockRef			inCAClock,
							CAClockPropertyID	inPropertyID,
							UInt32				inPropertyDataSize,
							const void *		inPropertyData)				API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockAddListener

	@abstract		Adds a callback function to receive notifications of changes to the clock's
					state.

					Note: The CAClockListenerProc may be called on a realtime thread internal to
					the clock object.
						
	@param			inCAClock
						The clock object.
	
	@param			inListenerProc
						The callback function.
	
	@param			inUserData
						This value is passed to the callback function, in the userData
						parameter.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockAddListener(			CAClockRef			inCAClock,
							CAClockListenerProc inListenerProc,
							void *				inUserData)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);
	
//  -----------------------------------------------------------------------------
/*!
	@function		CAClockRemoveListener
	
	@abstract		Removes a listener callback function.
	
	@param			inCAClock
						The clock object.
	
	@param			inListenerProc
						The callback function.
	
	@param			inUserData
						The same value as was passed for inUserData when this
						function was registered with CAClockAddListener. (This
						allows a single callback function to be registered more
						than once, with different userData arguments.)
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockRemoveListener(		CAClockRef			inCAClock,
							CAClockListenerProc inListenerProc,
							void *				inUserData)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockSetCurrentTime

	@abstract		Sets the clock's current position on the media timeline.

	Call this to specify where on the media timeline playback will begin. The
	supplied time must be in seconds, beats, or SMPTE (not host time or audio
	samples). Must only be called when stopped.					
	
	@param			inCAClock
						The clock object.
	
	@param			inTime
						The new time position.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockSetCurrentTime(		CAClockRef			inCAClock,
							const CAClockTime *	inTime)						API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockGetCurrentTime

	@abstract		Obtain the clock's current position on the media timeline.
	
	@param			inCAClock
						The clock object.
	
	@param			inTimeFormat
						Specifies the desired format for outTime.
	
	@param			outTime
						On exit, the clock's current time position.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockGetCurrentTime(		CAClockRef			inCAClock,
							CAClockTimeFormat	inTimeFormat,
							CAClockTime *		outTime)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockGetStartTime
	
	@abstract		Obtain the position on the media timeline where playback will start,
					or has already started.
	
	@param			inCAClock
						The clock object.

	@param			inTimeFormat
						Specifies the desired format for outTime.
	
	@param			outTime
						On exit, the clock's start time position.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockGetStartTime(		CAClockRef			inCAClock,
							CAClockTimeFormat	inTimeFormat,
							CAClockTime *		outTime)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockTranslateTime
	
	@abstract		Convert between time units.
	
	@param			inCAClock
						The clock object.
	
	@param			inTime
						The time to be converted.
	
	@param			inOutputTimeFormat
						Specifies the desired format for outTime
	
	@param			outTime
						On exit, a time corresponding to inTime, converted to the desired
						time format.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockTranslateTime(		CAClockRef			inCAClock,
							const CAClockTime *	inTime,
							CAClockTimeFormat	inOutputTimeFormat,
							CAClockTime *		outTime)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockStart
	
	@abstract		Begin advancing the clock on its media timeline.
	
	@param			inCAClock
						The clock object.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockStart(				CAClockRef			inCAClock)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockStop
	
	@abstract		Stop advancing the clock on its media timeline.
	
	@param			inCAClock
						The clock object.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockStop(				CAClockRef			inCAClock)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockArm

	@abstract		Allow received sync messages to start the clock.

	If a clock is following and being controlled by an external transport
	(e.g. MIDI Time Code), call this to indicate that the client is ready to
	start its transport in response to the external transport having started.

	The external time source will set the clock's start position and start
	the clock.
						
	@param			inCAClock
						The clock object.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockArm(					CAClockRef			inCAClock)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockDisarm
	
	@abstract		Disallow received sync messages from starting the clock.
	
	@param			inCAClock
						The clock object.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockDisarm(				CAClockRef			inCAClock)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockSetPlayRate

	@abstract		Alter the clock's playback rate.

	Adjusts the ratio between the timebase and media time; e.g. at 0.5, the 
	media time will move half as quickly as timebase time.
						
	@param			inCAClock
						The clock object.
	
	@param			inPlayRate
						The clock's desired play rate.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockSetPlayRate(			CAClockRef			inCAClock,
							Float64				inPlayRate)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockGetPlayRate

	@abstract		Obtain the clock's playback rate.

	Returns the clock's current play rate. If the clock is internally synced,
	this will be the last rate set by CAClockSetPlayRate. If the clock is
	externally synced, it will be the rate of the external sync source, where
	1.0 means that it is running at exactly the same rate as the clock's
	timebase. (2.0 means twice as fast).
	
	@param			inCAClock
						The clock object.
	
	@param			outPlayRate
						On exit, the clock's playback rate.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockGetPlayRate(			CAClockRef			inCAClock,
							Float64 *			outPlayRate)				API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockGetCurrentTempo
	
	@abstract		Obtain the clock's current musical tempo.

	Returns the current instantaneous tempo and a timestamp indicating where on the
	timeline the tempo most recently changed.

	If the clock is externally synced, the returned tempo will not reflect the
	effective tempo; this routine always reflects the client-specified tempo. To
	obtain the effective tempo, multiply the current tempo by the current play
	rate.
	
	@param			inCAClock
						The clock object.
	
	@param			outTempo
						On exit, the current tempo.
	
	@param			outTimestamp
						If non-null, on exit, the time at which the tempo last changed.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockGetCurrentTempo(		CAClockRef					inCAClock,
							CAClockTempo *				outTempo,
							CAClockTime * __nullable	outTimestamp)				API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockSetCurrentTempo
	
	@abstract		Manually override the clock's musical tempo during playback.

	Effects a manual override of the tempo map while running. After stopping and
	restarting, the original tempo map will be used again.
						
	@param			inCAClock
						The clock object.
	
	@param			inTempo
						The new desired tempo.
	
	@param			inTimestamp
						Specifies a precise point on the timeline where the tempo change is to
						take effect. If null, the tempo change takes effect immediately.
	
	@result
					An OSStatus error code. If this call is made while stopped,
					kCAClock_CannotSetTimeError is returned.
*/
extern OSStatus
CAClockSetCurrentTempo(		CAClockRef						inCAClock,
							CAClockTempo					inTempo,
							const CAClockTime * __nullable	inTimestamp)			API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockSecondsToSMPTETime
	
	@abstract		Converts seconds to a SMPTE time representation.

	Converts seconds on the media timeline to a SMPTE time. The clock's current
	SMPTE format and offset must be set appropriately.

	@param			inCAClock
						The clock object.
	
	@param			inSeconds
						The number of seconds to be converted (e.g. 3600 = 1 hour).
	
	@param			inSubframeDivisor
						The number of subframes per frame desired in outSMPTETime.
	
	@param			outSMPTETime
						On exit, the SMPTE time corresponding to inSeconds.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockSecondsToSMPTETime(	CAClockRef			inCAClock,
							CAClockSeconds		inSeconds,
							UInt16				inSubframeDivisor,
							SMPTETime *			outSMPTETime)				API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockSMPTETimeToSeconds
	
	@abstract		Converts a SMPTE time representation to seconds.

	Converts SMPTE time to seconds on the media timeline. The clock's current
	SMPTE format and offset must be set appropriately.

	@param			inCAClock
						The clock object.
	
	@param			inSMPTETime
						The SMPTE time to be converted to seconds.
	
	@param			outSeconds
						On exit, the number of seconds corresponding to inSMPTETime.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockSMPTETimeToSeconds(	CAClockRef			inCAClock,
							const SMPTETime *   inSMPTETime,
							CAClockSeconds *	outSeconds)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockBeatsToBarBeatTime
	
	@abstract		Converts a number of beats to a CABarBeatTime structure.

	Converts a beat position on the media timeline to a CABarBeatTime, using the
	clock's meter track. Examples using 4/4 time and a subbeat divisor of 480:

	inBeats | outBarBeatTime: bars . beats . units
	--------|-------------------------------------
	0		| 1.1.0
	1		| 1.2.0
	4		| 2.1.0
	4.5		| 2.1.240

	@param			inCAClock
						The clock object.
	
	@param			inBeats
						The absolute beat count to be converted.
	
	@param			inSubbeatDivisor
						The number of units per beat.
	
	@param			outBarBeatTime
						On exit, the bar/beat/subbeat time corresponding to inBeats.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockBeatsToBarBeatTime(	CAClockRef			inCAClock,
							CAClockBeats		inBeats,
							UInt16				inSubbeatDivisor,
							CABarBeatTime *		outBarBeatTime)				API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockBarBeatTimeToBeats
	
	@abstract		Converts a CABarBeatTime structure to a number of beats.

	Converts a CABarBeatTime structure (bars/beats/subbeats) to a beat
	position, using the clock's meter track.
	
	@param			inCAClock
						The clock object.
	
	@param			inBarBeatTime
						The bar/beat/subunit time to be converted to beats.
	
	@param			outBeats
						On exit, the number of absolute beats corresponding to inBarBeatTime.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockBarBeatTimeToBeats(	CAClockRef			inCAClock,
							const CABarBeatTime *inBarBeatTime,
							CAClockBeats *		outBeats)					API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//  -----------------------------------------------------------------------------
/*!
	@function		CAClockParseMIDI
	
	@abstract		Provides MIDI messages to a clock without using CoreMIDI

	In some situations, a client may wish to drive a clock using MIDI Time Code or
	beat clock obtained from a source other than Core MIDI. To do so,
	construct MIDIPacketLists containing the timecode or beat clock messages,
	and pass them to this function.
	
	@param			inCAClock
						The clock object.
	
	@param			inMIDIPacketList
						The MIDI events to be parsed.
	
	@result			An OSStatus error code.
*/
extern OSStatus
CAClockParseMIDI(			CAClockRef			inCAClock,
							const struct MIDIPacketList *inMIDIPacketList)	API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

#ifdef __cplusplus
}
#endif

CF_ASSUME_NONNULL_END

#endif // TARGET_OS_OSX

#endif // AudioToolbox_CoreAudioClock_h
