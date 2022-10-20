/*!
	@file		DefaultAudioOutput.h
	@framework	AudioToolbox.framework
	@copyright	(c) 2000-2015 by Apple, Inc., all rights reserved.
	@deprecated	Contains deprecated API's.
*/

#ifndef AudioToolbox_DefaultAudioOutput_h
#define AudioToolbox_DefaultAudioOutput_h

#if !__LP64__

#include <Availability.h>
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <AudioToolbox/AudioUnit.h>
#else
	#include <AudioUnit.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Open an instance of the default audio output unit (it can be closed with CloseComponent).
extern OSStatus	OpenDefaultAudioOutput(AudioUnit *outUnit)		API_DEPRECATED("no longer supported", macos(10.0, 10.3)) API_UNAVAILABLE(ios, watchos, tvos);

extern OSStatus	OpenSystemSoundAudioOutput(AudioUnit *outUnit)	API_DEPRECATED("no longer supported", macos(10.1, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
				// for system sounds like alerts, modems, etc.

#ifdef __cplusplus
}
#endif

#endif // !__LP64__

#endif // AudioToolbox_DefaultAudioOutput_h
