/* CoreGraphics - CGWindow.h
   Copyright (c) 2006-2008 Apple Inc.
   All rights reserved. */

#ifndef CGWINDOW_H_
#define CGWINDOW_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

/* The CGWindowID contains a unique value within the user session
   representing a window */

typedef uint32_t CGWindowID;

/* Values used for `CGWindowSharingType'. */
typedef CF_ENUM(uint32_t, CGWindowSharingType) {
    kCGWindowSharingNone = 0,
    kCGWindowSharingReadOnly = 1,
    kCGWindowSharingReadWrite = 2
};

/* Values used for `CGWindowBackingType'. */
typedef CF_ENUM(uint32_t, CGWindowBackingType) {
    kCGBackingStoreRetained = 0,
    kCGBackingStoreNonretained = 1,
    kCGBackingStoreBuffered = 2
};

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGImage.h>
#include <CoreFoundation/CFDictionary.h>
#include <CoreFoundation/CFArray.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* A value which is never the window ID of any window. */

#define kCGNullWindowID ((CGWindowID)0)

/* Keys for window dictionaries. */

/* The window ID, a unique value within the user session representing the
   window. The value of this key is a CFNumber 32-bit signed integer
   value. */

CG_EXTERN const CFStringRef  kCGWindowNumber
    CG_AVAILABLE_STARTING(10.5);

/* The backing store type of the window, one of `kCGBackingStoreRetained',
   `kCGBackingStoreNonretained', or `kCGBackingStoreBuffered'. The value of
   this key is a CFNumber 32-bit signed integer value. */

CG_EXTERN const CFStringRef  kCGWindowStoreType
    CG_AVAILABLE_STARTING(10.5);

/* The window layer number of the window. The value of this key is a
   CFNumber 32-bit signed integer value. */

CG_EXTERN const CFStringRef  kCGWindowLayer
    CG_AVAILABLE_STARTING(10.5);

/* The bounds of the window in screen space, with the origin at the
   upper-left corner of the main display. The value of this key is a
   CFDictionary; use `CGRectMakeWithDictionaryRepresentation' to obtain the
   bounds as a CGRect value. */

CG_EXTERN const CFStringRef  kCGWindowBounds
    CG_AVAILABLE_STARTING(10.5);

/* The sharing state of the window, one of `kCGWindowSharingNone',
   `kCGWindowSharingReadOnly', or `kCGWindowSharingReadWrite'. The value of
   this key is a CFNumber 32-bit signed integer value. */

CG_EXTERN const CFStringRef  kCGWindowSharingState
    CG_AVAILABLE_STARTING(10.5);

/* The alpha fade of the window. The value of this key is a CFNumber
   floating-point value. The value 1.0 is normal (opaque); the value 0.0 is
   fully transparent (invisible). */

CG_EXTERN const CFStringRef  kCGWindowAlpha
    CG_AVAILABLE_STARTING(10.5);

/* The process ID of the process that owns the window. The value of this key
   is a CFNumber 32-bit signed integer value. */

CG_EXTERN const CFStringRef  kCGWindowOwnerPID
    CG_AVAILABLE_STARTING(10.5);

/* An estimate of the memory in bytes currently used by the window and its
   supporting data structures. The value of this key is a CFNumber 64-bit
   signed integer value. */

CG_EXTERN const CFStringRef  kCGWindowMemoryUsage
    CG_AVAILABLE_STARTING(10.5);

/* Optional keys for window dictionaries. */

/* If present, the workspace ID of the workspace associated with the window.
   The value of this key is a CFNumber 32-bit signed integer value. */

CG_EXTERN const CFStringRef  kCGWindowWorkspace
  CG_AVAILABLE_BUT_DEPRECATED(10.5, 10.8);

/* If present, the name of the application process which owns the window.
   The value of this key is a CFString. */

CG_EXTERN const CFStringRef  kCGWindowOwnerName
    CG_AVAILABLE_STARTING(10.5);

/* If present, the name of the window. The value of this key is a
   CFString. */

CG_EXTERN const CFStringRef  kCGWindowName
    CG_AVAILABLE_STARTING(10.5);

/* If present, true if the window is ordered on screen, false otherwise. If
   the key is not present, then the window is not ordered on screen. The
   value of this key is a CFBoolean. */

CG_EXTERN const CFStringRef  kCGWindowIsOnscreen
    CG_AVAILABLE_STARTING(10.5);

/* If present, true if the window backing store is in video memory, false
   otherwise. If the key is not present, then the window backing store is in
   main memory. The value of this key is a CFBoolean. */

CG_EXTERN const CFStringRef  kCGWindowBackingLocationVideoMemory
    CG_AVAILABLE_STARTING(10.5);

/* Flags for CGWindowListOption values.  These may be ORed together. */

typedef CF_OPTIONS(uint32_t, CGWindowListOption) {
    /* List all windows in this user session, including both on- and
     off-screen windows. The parameter `relativeToWindow' should be
     `kCGNullWindowID'. */
    kCGWindowListOptionAll = 0,

    /* List all on-screen windows in this user session, ordered from front to
     back. The parameter `relativeToWindow' should be `kCGNullWindowID'. */
    kCGWindowListOptionOnScreenOnly = (1 << 0),

    /* List all on-screen windows above the window specified by
     `relativeToWindow', ordered from front to back. */
    kCGWindowListOptionOnScreenAboveWindow = (1 << 1),

    /* List all on-screen windows below the window specified by
     `relativeToWindow', ordered from front to back. */
    kCGWindowListOptionOnScreenBelowWindow = (1 << 2),

    /* Include the window specified by `relativeToWindow' in any list,
     effectively creating `at-or-above' or `at-or-below' lists. */
    kCGWindowListOptionIncludingWindow = (1 << 3),

    /* Exclude any windows from the list that are elements of the desktop. */
    kCGWindowListExcludeDesktopElements = (1 << 4)
};

/* Return an array of window dictionaries for windows within the user
   session.

   This function returns NULL if the caller is not running within a Quartz
   GUI session or the window server is disabled. You should release the
   array when you are finished using it. */

CG_EXTERN CFArrayRef __nullable CGWindowListCopyWindowInfo(CGWindowListOption option,
    CGWindowID relativeToWindow)
    CG_AVAILABLE_STARTING(10.5);

/* Return an array of CGWindowID values for windows within the user session.

   This function returns NULL if the caller is not running within a Quartz
   GUI session or the window server is disabled. You should release the
   array when you are finished using it. */

CG_EXTERN CFArrayRef __nullable CGWindowListCreate(CGWindowListOption option,
    CGWindowID relativeToWindow)
    CG_AVAILABLE_STARTING(10.5)
    CF_SWIFT_UNAVAILABLE("No replacement at present");

/* Return an array of window dictionaries, each corresponding to a window ID
   specified in `windowArray'.

   This function returns NULL if the caller is not running within a Quartz
   GUI session or the window server is disabled. You should release the
   array when you are finished using it. */

CG_EXTERN CFArrayRef __nullable CGWindowListCreateDescriptionFromArray(
    CFArrayRef __nullable windowArray)
    CG_AVAILABLE_STARTING(10.5);

/* Flags for CGWindowImageOption values.  These may be ORed together. */

typedef CF_OPTIONS(uint32_t, CGWindowImageOption) {
    /* If `CGRectNull' is passed as the screen bounds, then then bounds
     computation includes window frame ornamentation, such as a shadow. */
    kCGWindowImageDefault = 0,

    /* If `CGRectNull' is passed as the screen bounds, then then bounds
     computation excludes window frame ornamentation, such as a shadow. */
    kCGWindowImageBoundsIgnoreFraming = (1 << 0),

    /* Force the created image to be opaque.  Empty areas are white */
    kCGWindowImageShouldBeOpaque = (1 << 1),

    /* Only draw the windows' shadows, not the windows themselves. */
    kCGWindowImageOnlyShadows = (1 << 2),

    /* Return the best image resolution. The screen size may be
     different than the returned image size. */
    kCGWindowImageBestResolution = (1 << 3),

    /* Return the nominal image resolution. The screen size
     equals the returned image size. */
    kCGWindowImageNominalResolution = (1 << 4)
};

/* Create an image containing a composite of the specified set of windows
   contained within a rectangular area. The set of windows is specified
   using options from `CGWindowListOption', along with an optional
   additional window ID.

   The windows list options are:

   --- kCGWindowListOptionAll, kCGWindowListOptionOnScreenOnly: Use all
   on-screen windows in this user session to construct the image. The
   parameter `windowID' should be `kCGNullWindowID'.

   --- kCGWindowListOptionOnScreenAboveWindow: Use all on-screen windows in
   this user session above the window specified by `windowID', ordered from
   front to back, to construct the image. To include the window specified by
   `windowID', add the flag `kCGWindowListOptionIncludingWindow'.

   --- kCGWindowListOptionOnScreenBelowWindow: Use all on-screen windows in
   this user session below the window specified by `windowID', ordered from
   front to back, to construct the image. To include the window specified by
   `windowID', add the flag `kCGWindowListOptionIncludingWindow'.

   --- kCGWindowListOptionIncludingWindow: Use only the window specified by
   `windowID' to construct the image.

   The parameter `screenBounds' specifies the rectangle in screen space
   (origin at the upper-left; y-value increasing downward). Setting
   `screenBounds' to `CGRectInfinite' will include all the windows on the
   entire desktop. Setting `screenBounds' to `CGRectNull' will use the
   bounding box of the specified windows as the screen space rectangle.

   The parameter `imageOptions' allows you to specify whether the window
   frame ornamentation, such as a shadow or similar effect, should be
   included or excluded in the bounds calculation when `CGRectNull' is
   specified for the window bounds.

   If no windows meet the specified criteria, or the windows can't be read,
   then a transparent black image will be returned.

   Any on-screen window with sharing type `kCGWindowSharingNone' will not
   be included in the image.

   This function returns NULL if the caller is not running within a Quartz
   GUI session or the window server is disabled. */

CG_EXTERN CGImageRef __nullable CGWindowListCreateImage(CGRect screenBounds,
    CGWindowListOption listOption, CGWindowID windowID,
    CGWindowImageOption imageOption)
    CG_AVAILABLE_STARTING(10.5);

/* Create an image containing a composite of the specified set of windows
   contained within a rectangular area à la `CGWindowListCreateImage'. The
   set of windows is specified by `windowArray', an array of window IDs. */

CG_EXTERN CGImageRef __nullable CGWindowListCreateImageFromArray(
    CGRect screenBounds, CFArrayRef  windowArray,
    CGWindowImageOption imageOption)
    CG_AVAILABLE_STARTING(10.5);

/* A CFNumberRef encoding appropriate for use with a CGWindowID. */
#define kCGWindowIDCFNumberType kCFNumberSInt32Type

/* CFNumberRef encoding appropriate for use with CGWindowSharingType */
#define kCGWindowSharingCFNumberType kCFNumberSInt32Type

/* CFNumberRef encoding appropriate for use with CGWindowBackingType */
#define kCGWindowBackingCFNumberType kCFNumberSInt32Type

/* Checks whether the current process already has screen capture access */
CG_EXTERN bool CGPreflightScreenCaptureAccess(void) CG_AVAILABLE_STARTING(10.15);

/* Requests event listening access if absent, potentially prompting */
CG_EXTERN bool CGRequestScreenCaptureAccess(void) CG_AVAILABLE_STARTING(10.15);


CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGWINDOW_H_ */
