/*
     File:       HIToolbox/TypeSelect.h
 
     Contains:   TypeSelect Utilties
 
     Copyright:  © 2000-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TYPESELECT__
#define __TYPESELECT__

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)


/*
 *  TypeSelect
 *  
 *  Discussion:
 *    The TypeSelection API is deprecated in Mac OS X 10.4 and later,
 *    and is not included in the 64-bit version of HIToolbox.
 *    Applications should use the UCTypeSelect API in
 *    UnicodeUtilities.h.
 */
typedef SInt16 TSCode;
enum {
  tsPreviousSelectMode          = -1,
  tsNormalSelectMode            = 0,
  tsNextSelectMode              = 1
};

struct TypeSelectRecord {
  UInt32              tsrLastKeyTime;
  ScriptCode          tsrScript;
  Str63               tsrKeyStrokes;
};
typedef struct TypeSelectRecord         TypeSelectRecord;
#if !__LP64__
typedef CALLBACK_API( Boolean , IndexToStringProcPtr )(short item, ScriptCode *itemsScript, StringPtr *itemsStringPtr, void *yourDataPtr);
typedef STACK_UPP_TYPE(IndexToStringProcPtr)                    IndexToStringUPP;
/*
 *  NewIndexToStringUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern IndexToStringUPP
NewIndexToStringUPP(IndexToStringProcPtr userRoutine)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  DisposeIndexToStringUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeIndexToStringUPP(IndexToStringUPP userUPP)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

/*
 *  InvokeIndexToStringUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeIndexToStringUPP(
  short             item,
  ScriptCode *      itemsScript,
  StringPtr *       itemsStringPtr,
  void *            yourDataPtr,
  IndexToStringUPP  userUPP)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;

#if __MACH__
  #ifdef __cplusplus
    inline IndexToStringUPP                                     NewIndexToStringUPP(IndexToStringProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeIndexToStringUPP(IndexToStringUPP) { }
    inline Boolean                                              InvokeIndexToStringUPP(short item, ScriptCode * itemsScript, StringPtr * itemsStringPtr, void * yourDataPtr, IndexToStringUPP userUPP) { return (*userUPP)(item, itemsScript, itemsStringPtr, yourDataPtr); }
  #else
    #define NewIndexToStringUPP(userRoutine)                    ((IndexToStringUPP)userRoutine)
    #define DisposeIndexToStringUPP(userUPP)
    #define InvokeIndexToStringUPP(item, itemsScript, itemsStringPtr, yourDataPtr, userUPP) (*userUPP)(item, itemsScript, itemsStringPtr, yourDataPtr)
  #endif
#endif

#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  TypeSelectClear()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use UCTypeSelectFlushSelectorData instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCTypeSelectFlushSelectorData instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
TypeSelectClear(TypeSelectRecord * tsr)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  TypeSelectNewKey()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use UCTypeSelectAddKeyToSelector instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCTypeSelectAddKeyToSelector instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
TypeSelectNewKey(
  const EventRecord *  theEvent,
  TypeSelectRecord *   tsr)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  TypeSelectFindItem()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use UCTypeSelectFindItem instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCTypeSelectFindItem instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
TypeSelectFindItem(
  const TypeSelectRecord *  tsr,
  short                     listSize,
  TSCode                    selectMode,
  IndexToStringUPP          getStringProc,
  void *                    yourDataPtr)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/*
 *  TypeSelectCompare()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use UCTypeSelectCompare instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCTypeSelectCompare instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern short 
TypeSelectCompare(
  const TypeSelectRecord *  tsr,
  ScriptCode                testStringScript,
  StringPtr                 testStringPtr)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;



#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TYPESELECT__ */

