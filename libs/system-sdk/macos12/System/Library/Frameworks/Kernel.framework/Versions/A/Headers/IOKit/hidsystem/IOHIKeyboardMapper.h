/*
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#ifndef _IOHIKEYBOARDMAPPER_H
#define _IOHIKEYBOARDMAPPER_H

#include <IOKit/hidsystem/ev_keymap.h>
#include <IOKit/hidsystem/IOLLEvent.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOInterruptEventSource.h>

class IOHIKeyboard;
class IOHIDSystem;

/*
 * Key ip/down state is tracked in a bit list.  Bits are set
 * for key-down, and cleared for key-up.  The bit vector and macros
 * for it's manipulation are defined here.
 */

typedef	UInt32 * kbdBitVector;

#define EVK_BITS_PER_UNIT	32
#define EVK_BITS_MASK		31
#define EVK_BITS_SHIFT		5	// 1<<5 == 32, for cheap divide

#define EVK_KEYDOWN(n, bits) \
	(bits)[((n)>>EVK_BITS_SHIFT)] |= (1 << ((n) & EVK_BITS_MASK))

#define EVK_KEYUP(n, bits) \
	(bits)[((n)>>EVK_BITS_SHIFT)] &= ~(1 << ((n) & EVK_BITS_MASK))

#define EVK_IS_KEYDOWN(n, bits) \
	(((bits)[((n)>>EVK_BITS_SHIFT)] & (1 << ((n) & EVK_BITS_MASK))) != 0)

/* the maximum number of modifier keys sticky keys can hold at once */
#define kMAX_MODIFIERS					5

/* the number of shift keys in a row that must be depressed to toggle state */
#define kNUM_SHIFTS_TO_ACTIVATE			5

/* the number of milliseconds all the shifts must be pressed in - 30 seconds (30000 mS)*/
#define kDEFAULT_SHIFTEXPIREINTERVAL	30000


// sticky keys state flags
enum
{
    kState_Disabled_Flag        = 0x0001,	// disabled and will do nothing until this is changed
    kState_ShiftActivates_Flag	= 0x0002,	// the 'on' gesture (5 shifts) will activate
    kState_On                   = 0x0004,	// currently on, will hold down modifiers when pressed
    kState_On_ModifiersDown     = 0x0008,	// one or more modifiers being held down
    kState_Mask                 = 0x00FF,	// mask for all states
};

typedef struct _stickyKeys_ToggleInfo
{
	// size of this allocation
	IOByteCount		size;

	// which modifier key we are tracking (using NX_WHICHMODMASK)
	unsigned		toggleModifier;

	// the number of times the modifier must be pressed to toggle
	unsigned		repetitionsToToggle;

	// how long the user has to press the modifier repetitionsToToggle times
	// the default is 30 seconds
	AbsoluteTime	expireInterval;

	// the number of times the modifier used within the alloted time
	unsigned		currentCount;

	// the times that the last shift must occer for this one to be used
	// this array will actually be of size repetitionsToToggle
	AbsoluteTime	deadlines[1];
} StickyKeys_ToggleInfo;

// Flags for each sticky key modifier
// This will allow for chording of keys
// and for key locking
enum
{
    kModifier_DidPerformModifiy	= 0x01,
    kModifier_DidKeyUp		= 0x02,
    kModifier_Locked		= 0x04,
};
typedef struct _stickyKeys_ModifierInfo
{
	UInt8		key;		// Key code of the sticky modifier
        UInt8		state;		// The state of the sticky modifier
        UInt8		leftModBit;	// System Mod bit of the sticky modifier
} StickyKeys_ModifierInfo;

class IOHIDKeyboardDevice;

class __kpi_deprecated ("Use DriverKit") IOHIKeyboardMapper : public OSObject
{
  OSDeclareDefaultStructors(IOHIKeyboardMapper);

private:
	IOHIKeyboard *		_delegate;					// KeyMap delegate
	bool				_mappingShouldBeFreed;		// true if map can be IOFree'd
	NXParsedKeyMapping	_parsedMapping;				// current system-wide keymap
	
	// binary compatibility padding
    struct ExpansionData { 
    
        // This is for sticky keys
        kbdBitVector		cached_KeyBits;
        
        UInt32		specialKeyModifierFlags;
        
        SInt32      modifierSwap_Modifiers[NX_NUMMODIFIERS];
		
		unsigned char * cachedAlphaLockModDefs;
    };
    ExpansionData * _reserved;				    // Reserved for future use.  (Internal use only)
    
public:
	static IOHIKeyboardMapper * keyboardMapper(
										IOHIKeyboard * delegate,
										const UInt8 *  mapping,
										UInt32         mappingLength,
										bool           mappingShouldBeFreed );
	
	virtual bool init(IOHIKeyboard * delegate,
					const UInt8 *  mapping,
					  UInt32         mappingLength,
					  bool           mappingShouldBeFreed);
	virtual void free(void) APPLE_KEXT_OVERRIDE;
	
	virtual const UInt8 *   mapping();
	virtual UInt32          mappingLength();
	virtual bool 		  	serialize(OSSerialize *s) const APPLE_KEXT_OVERRIDE;
	
	virtual void 		translateKeyCode(UInt8 key, bool keyDown, kbdBitVector keyBits);
	virtual UInt8  		getParsedSpecialKey(UInt8 logical);   //retrieve a key from _parsedMapping

	virtual	void		setKeyboardTarget (IOService * keyboardTarget);
	
	virtual bool 	    updateProperties (void);
	virtual IOReturn  	setParamProperties (OSDictionary * dict);
	
	// keyEventPostProcess is called while a lock is not held, so a recursive
	// call back into HIKeyboard is possible
	virtual void 	    keyEventPostProcess (void);

private:
	static void makeNumberParamProperty( OSDictionary * dict, const char * key,
                            unsigned long long number, unsigned int bits );


	virtual bool parseKeyMapping(const UInt8 *        mapping,
								 UInt32               mappingLength,
							     NXParsedKeyMapping * parsedMapping) const;
	
	virtual void calcModBit(int bit, kbdBitVector keyBits);
	virtual void doModCalc(int key, kbdBitVector keyBits);
	virtual void doCharGen(int keyCode, bool down);

	/* sticky keys functionality */
private:
	// original translateKeyCode
	void rawTranslateKeyCode (UInt8 key, bool keyDown, kbdBitVector keyBits);
  
	// post special keyboard events thru the event system
	void postKeyboardSpecialEvent (unsigned subtype, unsigned eventType=NX_SYSDEFINED);

private:
        
private:
public:
    OSMetaClassDeclareReservedUsed(IOHIKeyboardMapper,  0);
    virtual IOReturn message( UInt32 type, IOService * provider, void * argument = 0 );

	// binary compatibility padding
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  1);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  2);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  3);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  4);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  5);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  6);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  7);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  8);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper,  9);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper, 10);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper, 11);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper, 12);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper, 13);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper, 14);
    OSMetaClassDeclareReservedUnused(IOHIKeyboardMapper, 15);
};

#endif // _IOHIKEYBOARDMAPPER_H

/*
 * HISTORICAL NOTE:
 *   The "delegate" object had to respond to the following protocol;
 *   this protocol has since been merged into the IOHIKeyboard class.
 *
 * @protocol KeyMapDelegate
 *
 * - keyboardEvent	:(unsigned)eventType
 * 	flags	:(unsigned)flags
 *	keyCode	:(unsigned)keyCode
 *	charCode:(unsigned)charCode
 *	charSet	:(unsigned)charSet
 *	originalCharCode:(unsigned)origCharCode
 *	originalCharSet:(unsigned)origCharSet;
 * 
 * - keyboardSpecialEvent:(unsigned)eventType
 *	flags	 :(unsigned)flags
 *	keyCode	:(unsigned)keyCode
 *	specialty:(unsigned)flavor;
 *
 * - updateEventFlags:(unsigned)flags;	// Does not generate events
 *
 * - (unsigned)eventFlags;		// Global event flags
 * - (unsigned)deviceFlags;		// per-device event flags
 * - setDeviceFlags:(unsigned)flags;	// Set device event flags
 * - (bool)alphaLock;			// current alpha-lock state
 * - setAlphaLock:(bool)val;		// Set current alpha-lock state
 * - (bool)charKeyActive;		// Is a character gen. key down?
 * - setCharKeyActive:(bool)val;	// Note that a char gen key is down.
 *
 * @end
 */
