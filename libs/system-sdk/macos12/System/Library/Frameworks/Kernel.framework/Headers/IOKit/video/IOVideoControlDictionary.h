/*
	File:		IOVideoControlDictionary.h
	
	Contains:	

	Copyright:	© 2006-2012 by Apple Inc., all rights reserved.
*/

#if !defined(__IOVideoControlDictionary_h__)
#define __IOVideoControlDictionary_h__

// System Includes
#include <IOKit/video/IOVideoTypes.h>

class	OSArray;
class	OSDictionary;
class	OSString;

class IOVideoControlDictionary
{
//	Construction/Destruction
public:
	static OSDictionary*	create(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, bool isReadOnly = false, UInt32 variant = 0, OSString* name = NULL);
	static OSDictionary*	createBooleanControl(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, bool value, bool isReadOnly = false, UInt32 variant = 0, OSString* name = NULL);
	static OSDictionary*	createSelectorControl(UInt32 controlID, UInt32 baseClass, UInt32 derivedClass, UInt32 scope, UInt32 element, UInt32 value, OSArray* selectorMap, bool isReadOnly = false, UInt32 variant = 0, OSString* name = NULL);

//	General Attributes
public:
	static OSDictionary*	getControlByID(OSArray* controlList, UInt32 controlID);

	static UInt32			getControlID(const OSDictionary* dictionary);
	static void				setControlID(OSDictionary* dictionary, UInt32 controlID);

	static UInt32			getBaseClass(const OSDictionary* dictionary);
	static void				setBaseClass(OSDictionary* dictionary, UInt32 baseClass);

	static UInt32			getClass(const OSDictionary* dictionary);
	static void				setClass(OSDictionary* dictionary, UInt32 derivedClass);

	static UInt32			getScope(const OSDictionary* dictionary);
	static void				setScope(OSDictionary* dictionary, UInt32 scope);

	static UInt32			getElement(const OSDictionary* dictionary);
	static void				setElement(OSDictionary* dictionary, UInt32 element);

	static bool				isReadOnly(const OSDictionary* dictionary);
	static void				setIsReadOnly(OSDictionary* dictionary, bool isReadOnly);

	static UInt32			getVariant(const OSDictionary* dictionary);
	static void				setVariant(OSDictionary* dictionary, UInt32 variant);

	static OSString*		copyName(const OSDictionary* dictionary);
	static void				setName(OSDictionary* dictionary, const OSString* name);

//	Boolean Control Attributes
public:
	static bool				getBooleanControlValue(const OSDictionary* dictionary);
	static void				setBooleanControlValue(OSDictionary* dictionary, bool value);

//	Selector Control Attributes
public:
	static UInt32			getSelectorControlValue(const OSDictionary* dictionary);
	static void				setSelectorControlValue(OSDictionary* dictionary, UInt32 value);

	static OSArray*			copySelectorControlSelectorMap(const OSDictionary* dictionary);
	static void				setSelectorControlSelectorMap(OSDictionary* dictionary, const OSArray* selectorMap);

//	Selector Control Selector Map Item Support
public:
	static OSDictionary*	createSelectorControlSelectorMapItem(UInt32 value, const OSString* name);
	static OSDictionary*	createSelectorControlSelectorMapItem(UInt32 value, const OSString* name, UInt32 kind);
};

#endif
