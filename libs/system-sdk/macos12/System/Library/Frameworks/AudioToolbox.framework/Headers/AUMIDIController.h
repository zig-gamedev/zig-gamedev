/*!
	@file		AUMIDIController.h
	@framework	AudioToolbox.framework
	@copyright	(c) 2002-2015 by Apple, Inc., all rights reserved.
	@abstract	Deprecated.
*/

#ifndef AudioToolbox_AUMIDIController_h
#define AudioToolbox_AUMIDIController_h

#include <TargetConditionals.h>

#if !TARGET_RT_64_BIT
// This API set is not available for 64-bit

#include <AudioToolbox/AudioUnit.h>
#include <CoreMIDI/CoreMIDI.h>
#include <AudioToolbox/AudioUnitUtilities.h>

typedef struct OpaqueAUMIDIController *AUMIDIControllerRef;


#ifdef __cplusplus
extern "C" {
#endif


/*
	inVirtualDestinationName is null to create no virtual destination
*/
extern OSStatus
AUMIDIControllerCreate(			CFStringRef				inVirtualDestinationName,
								AUMIDIControllerRef *	outController)				
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);

extern OSStatus
AUMIDIControllerDispose(		AUMIDIControllerRef		inController)			
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);

extern OSStatus
AUMIDIControllerMapChannelToAU(	AUMIDIControllerRef		inController,
								SInt32					inSourceMIDIChannel,
								AudioUnit				inAudioUnit,
								SInt32					inDestMIDIChannel,
								Boolean					inCreateDefaultControlMappings)
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);


extern OSStatus
AUMIDIControllerMapEventToParameter(	AUMIDIControllerRef		inController,
										UInt8					inMIDIStatusByte,
										UInt16					inMIDIControl,
										const AudioUnitParameter *inParameter)	
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);

// this will remove any mapping held by this controller
// to the specified audio unit - whether those are:
// (1) default mappings (AUMIDIControllerMapChannelToAU) 
// (2) custom mappings (AUMIDIControllerMapEventToParameter)

// Typically, this is done when (and should be done) when an AU no longer
// should receive MIDI events for its parameters (or the AU is being disposed)
extern OSStatus
AUMIDIControllerUnmapAudioUnit(		AUMIDIControllerRef		inController,
									AudioUnit				inAudioUnit)		
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
										
/*
	$$$ need description of timestamps in the packets (if any) are treated -- needs
	to factor in the AU's latency $$$
*/
extern OSStatus
AUMIDIControllerHandleMIDI(			AUMIDIControllerRef		inController,
									const MIDIPacketList *	inMIDIPacketList)	
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);


extern OSStatus
AUMIDIControllerConnectSource(		AUMIDIControllerRef		inController,
									MIDIEndpointRef			inSource)			
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);

extern OSStatus
AUMIDIControllerDisconnectSource(	AUMIDIControllerRef		inController,
									MIDIEndpointRef			inSource)			
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);

/*
	Tells an AUMIDIController to generate an XML description of the control/NRPN 
	mapping.  Returns a (local file) URL to the file written.  $$$ If the AUMIDIController
	has a virtual destination associated with it, the AUMIDIController will
	call MIDIObjectSetNameConfiguration to publish those names as the current
	ones for that destination.
*/
extern OSStatus
AUMIDIControllerExportXMLNames(		AUMIDIControllerRef		inController,
									CFURLRef *				outXMLFileURL)		
	API_DEPRECATED("no longer supported", macos(10.2, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);


#ifdef __cplusplus
}
#endif

#endif // !TARGET_RT_64_BIT

#endif // AudioToolbox_AUMIDIController_h

