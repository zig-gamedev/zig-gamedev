#ifndef __HITOOLBOXDEBUGGING__
#define __HITOOLBOXDEBUGGING__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif


#ifndef __CARBONEVENTS__
#include <HIToolbox/CarbonEvents.h>
#endif

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif

#ifndef __MACWINDOWS__
#include <HIToolbox/MacWindows.h>
#endif

#ifndef __DIALOGS__
#include <HIToolbox/Dialogs.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* CarbonEvents */
#if !__LP64__
/*
 *  DebugSetEventTraceEnabled()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugSetEventTraceEnabled(Boolean inEnabled)                  AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugTraceEvent()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugTraceEvent(
  OSType    inEventClass,
  UInt32    inEventKind,
  Boolean   inTrace)                                          AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugTraceEventByName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugTraceEventByName(
  const char *  eventName,
  Boolean       inTrace)                                      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugPrintTracedEvents()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintTracedEvents(void)                                  AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugPrintEventQueue()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintEventQueue(EventQueueRef inQueue)                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


#endif  /* !__LP64__ */

/*
 *  DebugPrintMainEventQueue()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintMainEventQueue(void)                                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugPrintEvent()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintEvent(EventRef inEvent)                             AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/* Control specific debugging routines */
#if !__LP64__
/*
 *  DebugPrintControl()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintControl(ControlRef inControl)                       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugPrintControlHierarchy()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintControlHierarchy(WindowRef inWindow)                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DumpControlHierarchy()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
DumpControlHierarchy(
  WindowRef       inWindow,
  const FSSpec *  inDumpFile)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Dialogs */
/*
 *  DebugPrintDialogInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintDialogInfo(DialogRef inDialog)                      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/* HIView */
/*
 *  HIViewFlashDirtyArea()
 *  
 *  Discussion:
 *    Debugging aid. Flashes the region which would be redrawn at the
 *    next draw time for an entire window.
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to flash the dirty region for.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIViewFlashDirtyArea(WindowRef inWindow)                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;



/* Menus */
/*
 *  DebugPrintMenuList()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintMenuList(void)                                      AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugPrintMenu()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintMenu(MenuRef inMenu)                                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugPrintMenuItem()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintMenuItem(
  MenuRef         inMenu,
  MenuItemIndex   inItem)                                     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/* Windows */
/*
 *  DebugPrintWindow()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintWindow(WindowRef windowRef)                         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugPrintWindowGroup()
 *  
 *  Summary:
 *    Prints the contents of a window group to stdout.
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose contents to print.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintWindowGroup(WindowGroupRef inGroup)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DebugPrintAllWindowGroups()
 *  
 *  Summary:
 *    Prints the full window group hierarchy, starting at the root
 *    group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintAllWindowGroups(void)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DebugPrintWindowList()
 *  
 *  Summary:
 *    Prints the window list to stdout.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugPrintWindowList(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  DebugFlashWindowVisRgn()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugFlashWindowVisRgn(WindowRef windowRef)                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER_BUT_DEPRECATED;


/*
 *  DebugFlashWindowUpdateRgn()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
DebugFlashWindowUpdateRgn(WindowRef inWindow)                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER_BUT_DEPRECATED;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HITOOLBOXDEBUGGING__ */

