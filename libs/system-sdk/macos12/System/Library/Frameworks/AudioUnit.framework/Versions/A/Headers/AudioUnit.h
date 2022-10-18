// Umbrella header for AudioUnit.framework
// Deprecated: Use AudioToolbox

#ifndef AudioUnit_AudioUnit_h
#define AudioUnit_AudioUnit_h

#ifndef AUDIO_UNIT_VERSION
#define AUDIO_UNIT_VERSION 1070
#endif

#include <AudioUnit/AUComponent.h>
#include <AudioUnit/AudioCodec.h>
#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioOutputUnit.h>
#include <AudioUnit/AudioUnitParameters.h>
#include <AudioUnit/AudioUnitProperties.h>
#include <AudioUnit/MusicDevice.h>

#ifdef __OBJC2__
#include <AudioUnit/AUAudioUnit.h>
#include <AudioUnit/AUAudioUnitImplementation.h>
#include <AudioUnit/AUParameters.h>
#endif // __OBJC2__

#endif // AudioUnit_AudioUnit_h
