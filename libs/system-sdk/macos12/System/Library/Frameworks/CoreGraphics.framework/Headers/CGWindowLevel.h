/* CoreGraphics - CGWindowLevel.h
   Copyright (c) 2000-2008 Apple Inc.
   All rights reserved. */

#ifndef CGWINDOWLEVEL_H_
#define CGWINDOWLEVEL_H_

#include <CoreFoundation/CFAvailability.h>
#include <CoreGraphics/CGBase.h>
#include <stdint.h>

/* Windows may be assigned to a particular level. When assigned to a level,
   the window is ordered relative to all other windows in that level.
   Windows with a higher level are sorted in front of windows with a lower
   level.

   A common set of window levels is defined here for use within higher level
   frameworks. The levels are accessed via a key and function, so that
   levels may be changed or adjusted in future releases without breaking
   binary compatability. */

typedef CF_ENUM (int32_t, CGWindowLevelKey) {
    kCGBaseWindowLevelKey = 0,
    kCGMinimumWindowLevelKey,
    kCGDesktopWindowLevelKey,
    kCGBackstopMenuLevelKey,
    kCGNormalWindowLevelKey,
    kCGFloatingWindowLevelKey,
    kCGTornOffMenuWindowLevelKey,
    kCGDockWindowLevelKey,
    kCGMainMenuWindowLevelKey,
    kCGStatusWindowLevelKey,
    kCGModalPanelWindowLevelKey,
    kCGPopUpMenuWindowLevelKey,
    kCGDraggingWindowLevelKey,
    kCGScreenSaverWindowLevelKey,
    kCGMaximumWindowLevelKey,
    kCGOverlayWindowLevelKey,
    kCGHelpWindowLevelKey,
    kCGUtilityWindowLevelKey,
    kCGDesktopIconWindowLevelKey,
    kCGCursorWindowLevelKey,
    kCGAssistiveTechHighWindowLevelKey,
    kCGNumberOfWindowLevelKeys	/* Must be last. */
};

typedef int32_t CGWindowLevel;

/* Return the window level that corresponds to one of the standard window
   types. */

CG_EXTERN CGWindowLevel CGWindowLevelForKey(CGWindowLevelKey key)
  CG_AVAILABLE_STARTING(10.0);

/* The number of window levels reserved by Apple for internal use. */
#define kCGNumReservedWindowLevels	(16)
#define kCGNumReservedBaseWindowLevels (5)

/* Definitions of older constant values. */
#define kCGBaseWindowLevel              ((CGWindowLevel)INT32_MIN)
#define kCGMinimumWindowLevel           ((CGWindowLevel)(kCGBaseWindowLevel + kCGNumReservedBaseWindowLevels))
#define kCGMaximumWindowLevel           ((CGWindowLevel)(INT32_MAX - kCGNumReservedWindowLevels))

#define kCGDesktopWindowLevel           ((CGWindowLevel)(kCGMinimumWindowLevel + 20))
#define kCGDesktopIconWindowLevel       ((CGWindowLevel)(kCGDesktopWindowLevel + 20))
#define kCGBackstopMenuLevel            ((CGWindowLevel)-20)
#define kCGNormalWindowLevel            ((CGWindowLevel)0)
#define kCGFloatingWindowLevel          ((CGWindowLevel)3)
#define kCGTornOffMenuWindowLevel       ((CGWindowLevel)3)
#define kCGModalPanelWindowLevel        ((CGWindowLevel)8)
#define kCGUtilityWindowLevel           ((CGWindowLevel)19)
#define kCGDockWindowLevel              ((CGWindowLevel)20)
#define kCGMainMenuWindowLevel          ((CGWindowLevel)24)
#define kCGStatusWindowLevel            ((CGWindowLevel)25)
#define kCGPopUpMenuWindowLevel         ((CGWindowLevel)101)
#define kCGOverlayWindowLevel           ((CGWindowLevel)102)
#define kCGHelpWindowLevel              ((CGWindowLevel)200)
#define kCGDraggingWindowLevel          ((CGWindowLevel)500)
#define kCGScreenSaverWindowLevel       ((CGWindowLevel)1000)
#define kCGAssistiveTechHighWindowLevel ((CGWindowLevel)1500)
#define kCGCursorWindowLevel            ((CGWindowLevel)(kCGMaximumWindowLevel - 1))

#endif /* CGWINDOWLEVEL_H_ */
