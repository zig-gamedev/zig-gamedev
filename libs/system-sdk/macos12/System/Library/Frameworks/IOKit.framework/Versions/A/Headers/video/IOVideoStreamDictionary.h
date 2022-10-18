/*
	File:		IOVideoStreamDictionary.h
	
	Contains:	

	Copyright:	© 2006-2012 by Apple Inc., all rights reserved.
*/

#if !defined(__IOVideoStreamDictionary_h__)
#define __IOVideoStreamDictionary_h__

// System Includes
#include <IOKit/video/IOVideoTypes.h>

class OSArray;
class OSDictionary;

class IOVideoStreamDictionary
{

// Construction/Destruction
public:
	static OSDictionary*	create(UInt32 streamID, UInt32 startingDeviceChannelNumber, const OSDictionary* currentFormat, OSArray* availableFormats = NULL);

// Attributes
public:
	static UInt32			getStreamID(const OSDictionary* dictionary);
	static void				setStreamID(OSDictionary* dictionary, UInt32 streamID);
	
	static UInt32			getStartingDeviceChannelNumber(const OSDictionary* dictionary);
	static void				setStartingDeviceChannelNumber(OSDictionary* dictionary, UInt32 startingDeviceChannelNumber);
	
	static IOOptionBits		getBufferMappingOptions(const OSDictionary* dictionary);
	static void				setBufferMappingOptions(OSDictionary* dictionary, IOOptionBits bufferMappingOptions);

	static bool				getCurrentFormat(const OSDictionary* dictionary, IOVideoStreamDescription& format);
	static void				setCurrentFormat(OSDictionary* dictionary, const IOVideoStreamDescription& format);
	static OSDictionary*	copyCurrentFormatDictionary(const OSDictionary* dictionary);
	static void				setCurrentFormatDictionary(OSDictionary* dictionary, const OSDictionary* format);
	
	static OSArray*			copyAvailableFormats(const OSDictionary* dictionary);
	static void				setAvailableFormats(OSDictionary* dictionary, OSArray* availableFormats);
	
	static void				printDictionary(const OSDictionary* dictionary);
};

#endif
