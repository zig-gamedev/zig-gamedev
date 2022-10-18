/*
	File:		IOVideoStreamFormat.h
	
	Contains:	

	Copyright:	© 2006-2012 by Apple Inc., all rights reserved.
*/

#if !defined(__IOVideoStreamFormat_h__)
#define __IOVideoStreamFormat_h__

// System Includes
#include <IOKit/video/IOVideoTypes.h>

class OSDictionary;

class IOVideoStreamFormatDictionary
{
public:
	static OSDictionary*	create(UInt32 codecType, UInt32 codecFlags, UInt32 width, UInt32 height);
	static OSDictionary*	createWithDescription(const IOVideoStreamDescription& format);

// Attributes
public:
	static UInt32			getCodecType(const OSDictionary* dictionary);
	static void				setCodecType(OSDictionary* dictionary, UInt32 codecType);

	static UInt32			getCodecFlags(const OSDictionary* dictionary);
	static void				setCodecFlags(OSDictionary* dictionary, UInt32 codecFlags);

	static UInt32			getWidth(const OSDictionary* dictionary);
	static void				setWidth(OSDictionary* dictionary, UInt32 width);

	static UInt32			getHeight(const OSDictionary* dictionary);
	static void				setHeight(OSDictionary* dictionary, UInt32 height);

	static void				getDescription(const OSDictionary* dictionary, IOVideoStreamDescription& format);
	static void				printDescription(const IOVideoStreamDescription& format);
	static void				printDictionary(const OSDictionary* dictionary);
	
	static bool				isSameSampleFormat(const IOVideoStreamDescription& format1, const IOVideoStreamDescription& format2);
};

#endif
