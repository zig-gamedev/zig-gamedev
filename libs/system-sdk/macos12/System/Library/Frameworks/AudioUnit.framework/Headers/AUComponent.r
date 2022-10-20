/*
	File:   AUComponent.r
 
	Contains:   AudioUnit Interfaces
  
	Copyright:  � 2002-2008 by Apple, Inc., all rights reserved.
 
	Bugs?:		For bug reports, consult the following page on
                the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __AUCOMPONENT_R__
#define __AUCOMPONENT_R__

#ifndef __CONDITIONALMACROS_R__
#include "ConditionalMacros.r"
#endif

#define kAudioUnitType_Output					'auou'
#define kAudioUnitSubType_HALOutput				'ahal'
#define kAudioUnitSubType_DefaultOutput			'def '
#define kAudioUnitSubType_SystemOutput			'sys '
#define kAudioUnitSubType_GenericOutput			'genr'
#define kAudioUnitSubType_VoiceProcessingIO		'vpio'

#define kAudioUnitType_MusicDevice				'aumu'
#define kAudioUnitSubType_DLSSynth				'dls '
#define kAudioUnitSubType_MIDISynth				'msyn'
#define kAudioUnitSubType_Sampler				'samp'

#define kAudioUnitType_MusicEffect				'aumf'

#define kAudioUnitType_FormatConverter			'aufc'
#define kAudioUnitSubType_AUConverter			'conv'
#define kAudioUnitSubType_Varispeed				'vari'
#define kAudioUnitSubType_DeferredRenderer		'defr'
#define kAudioUnitSubType_TimePitch				'tmpt'
#define kAudioUnitSubType_NewTimePitch			'nutp'
#define kAudioUnitSubType_MultiSplitter			'mspl'
#define kAudioUnitSubType_Splitter				'splt'
#define kAudioUnitSubType_Merger				'merg'
#define kAudioUnitSubType_RoundTripAAC			'raac'

#define kAudioUnitType_Effect					'aufx'
#define kAudioUnitSubType_Delay					'dely'
#define kAudioUnitSubType_LowPassFilter			'lpas'
#define kAudioUnitSubType_HighPassFilter		'hpas'
#define kAudioUnitSubType_BandPassFilter		'bpas'
#define kAudioUnitSubType_HighShelfFilter		'hshf'
#define kAudioUnitSubType_LowShelfFilter		'lshf'
#define kAudioUnitSubType_ParametricEQ			'pmeq'
#define kAudioUnitSubType_GraphicEQ				'greq'
#define kAudioUnitSubType_PeakLimiter			'lmtr'
#define kAudioUnitSubType_DynamicsProcessor		'dcmp'
#define kAudioUnitSubType_MultiBandCompressor	'mcmp'
#define kAudioUnitSubType_MatrixReverb			'mrev'
#define kAudioUnitSubType_SampleDelay			'sdly'
#define kAudioUnitSubType_Pitch					'pitc'
#define kAudioUnitSubType_AUFilter				'filt'
#define kAudioUnitSubType_NetSend				'nsnd'
#define kAudioUnitSubType_Distortion			'dist'
#define kAudioUnitSubType_RogerBeep				'rogr'
#define kAudioUnitSubType_NBandEQ               'nbeq'
#define kAudioUnitSubType_Reverb2               'rvb2'

#define kAudioUnitType_Mixer					'aumx'
#define kAudioUnitSubType_StereoMixer			'smxr'
#define kAudioUnitSubType_3DMixer				'3dmx'
#define kAudioUnitSubType_MatrixMixer			'mxmx'
#define kAudioUnitSubType_MultiChannelMixer		'mcmx'

#define kAudioUnitType_Panner					'aupn'
#define kAudioUnitSubType_SphericalHeadPanner	'sphr'
#define kAudioUnitSubType_VectorPanner			'vbas'
#define kAudioUnitSubType_SoundFieldPanner		'ambi'
#define kAudioUnitSubType_HRTFPanner			'hrtf'

#define kAudioUnitType_OfflineEffect			'auol'

#define kAudioUnitType_Generator				'augn'
#define kAudioUnitSubType_ScheduledSoundPlayer	'sspl'
#define kAudioUnitSubType_AudioFilePlayer		'afpl'
#define kAudioUnitSubType_NetReceive			'nrcv'

#define kAudioUnitManufacturer_Apple			'appl'


#endif /* __AUCOMPONENT_R__ */

