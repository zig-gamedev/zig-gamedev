/* CoreGraphics - CGDirectDisplay.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGDIRECTDISPLAY_H_
#define CGDIRECTDISPLAY_H_


#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGError.h>
#include <CoreGraphics/CGGeometry.h>
#include <CoreGraphics/CGWindow.h>
#include <CoreGraphics/CGWindowLevel.h>
#include <mach/boolean.h>

typedef uint32_t CGDirectDisplayID;
typedef uint32_t CGOpenGLDisplayMask;
typedef double CGRefreshRate;

typedef struct CF_BRIDGED_TYPE(id) CGDisplayMode *CGDisplayModeRef;

#define kCGNullDirectDisplay ((CGDirectDisplayID)0)
#define kCGDirectMainDisplay CGMainDisplayID()

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Return the display ID of the current main display. */

CG_EXTERN CGDirectDisplayID CGMainDisplayID(void)
    CG_AVAILABLE_STARTING(10.2);

/* Mechanisms used to find screen IDs.

   The following functions take an array length (`maxDisplays') and array of
   pointers to CGDirectDisplayIDs (`displays'). The array is filled in with
   the displays meeting the specified criteria; no more than `maxDisplays'.
   will be stored in `displays'. The number of displays meeting the criteria
   is returned in `matchingDisplayCount'.

   If the `displays' array is NULL, only the number of displays meeting the
   specified criteria is returned in `matchingDisplayCount'. */

CG_EXTERN CGError CGGetDisplaysWithPoint(CGPoint point, uint32_t maxDisplays,
    CGDirectDisplayID * __nullable displays,
    uint32_t * __nullable matchingDisplayCount)
    CG_AVAILABLE_STARTING(10.0);

CG_EXTERN CGError CGGetDisplaysWithRect(CGRect rect, uint32_t maxDisplays,
    CGDirectDisplayID * __nullable displays,
    uint32_t * __nullable matchingDisplayCount)
    CG_AVAILABLE_STARTING(10.0);

CG_EXTERN CGError CGGetDisplaysWithOpenGLDisplayMask(CGOpenGLDisplayMask mask,
    uint32_t maxDisplays, CGDirectDisplayID * __nullable displays,
    uint32_t * __nullable matchingDisplayCount)
    CG_AVAILABLE_STARTING(10.0);
                            
/* Return a list of active displays.

   If `activeDisplays' is NULL, then `maxDisplays' is ignored, and
   `displayCount' is set to the number of displays. Otherwise, the list of
   active displays is stored in `activeDisplays'; no more than `maxDisplays'
   will be stored in `activeDisplays'.

   The first display returned in the list is the main display (the one with
   the menu bar). When mirroring, this will be the largest drawable display
   in the mirror set, or, if all displays are the same size, the one with
   the deepest pixel depth. */

CG_EXTERN CGError CGGetActiveDisplayList(uint32_t maxDisplays,
    CGDirectDisplayID * __nullable activeDisplays,
    uint32_t * __nullable displayCount)
    CG_AVAILABLE_STARTING(10.0);

/* Return a list of online displays.

   If `onlineDisplays' is NULL, then `maxDisplays' is ignored, and
   `displayCount' is set to the number of displays. Otherwise, the list of
   online displays is stored in `onlineDisplays'; no more than `maxDisplays'
   will be stored in `onlineDisplays'.

   With hardware mirroring, a display may be online but not necessarily
   active or drawable. Programs which manipulate display settings such as
   the palette or gamma tables need access to all displays in use, including
   hardware mirrors which are not drawable. */

CG_EXTERN CGError CGGetOnlineDisplayList(uint32_t maxDisplays,
    CGDirectDisplayID * __nullable onlineDisplays,
    uint32_t * __nullable displayCount)
    CG_AVAILABLE_STARTING(10.2);

/* Return the OpenGL display mask for `display', or 0 if `display' is an
   invalid display. */

CG_EXTERN CGOpenGLDisplayMask CGDisplayIDToOpenGLDisplayMask(
    CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Return the display for the OpenGL display mask `mask', or
   `kCGNullDirectDisplay' if the bits set dont't match a display. A mask
   with multiple bits set returns an arbitrary match. */

CG_EXTERN CGDirectDisplayID CGOpenGLDisplayMaskToDisplayID(
    CGOpenGLDisplayMask mask)
    CG_AVAILABLE_STARTING(10.2);

/* Return the screen size and screen origin of `display' in global
   coordinates, or `CGRectZero' if `display' is invalid. */

CG_EXTERN CGRect CGDisplayBounds(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Return the width in pixels of `display'. */

CG_EXTERN size_t CGDisplayPixelsWide(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Return the height in pixels of `display'. */

CG_EXTERN size_t CGDisplayPixelsHigh(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Return an array of all modes for the specified display, or NULL if
   `display' is invalid. The "options" field is reserved for future
   expansion; pass NULL for now. */
  
CG_EXTERN CFArrayRef __nullable CGDisplayCopyAllDisplayModes(
    CGDirectDisplayID display, CFDictionaryRef __nullable options)
    CG_AVAILABLE_STARTING(10.6);

CG_EXTERN const CFStringRef  kCGDisplayShowDuplicateLowResolutionModes
    CG_AVAILABLE_STARTING(10.8);

/* Return the current mode of the specified display, or NULL if `display'
   is invalid. */
   
CG_EXTERN CGDisplayModeRef __nullable CGDisplayCopyDisplayMode(
    CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.6);

/* Switch the display mode of `display' to `mode'. The "options" field is
   reserved for future expansion; pass NULL for now.

   The selected display mode persists for the life of the program, and
   automatically reverts to the permanent setting when the program
   terminates.

   When changing display modes of displays in a mirroring set, other
   displays in the mirroring set will be set to a display mode capable of
   mirroring the bounds of the largest display being explicitly set.

   Note that after switching, display parameters and addresses may change. */

CG_EXTERN CGError CGDisplaySetDisplayMode(CGDirectDisplayID display,
    CGDisplayModeRef __nullable mode, CFDictionaryRef __nullable options)
    CG_AVAILABLE_STARTING(10.6);

/* Return the width in points of the specified display mode. */

CG_EXTERN size_t CGDisplayModeGetWidth(CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Return the height in points of the specified display mode. */

CG_EXTERN size_t CGDisplayModeGetHeight(CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Return a string representing the pixel encoding of the specified display
   mode, expressed as a CFString containing an IOKit graphics mode. */

CG_EXTERN CFStringRef __nullable CGDisplayModeCopyPixelEncoding(
    CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_BUT_DEPRECATED(10.6, 10.11);

/* Return the refresh rate of the specified display mode. */

CG_EXTERN double CGDisplayModeGetRefreshRate(CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Return the IOKit flags of the specified display mode. */

CG_EXTERN uint32_t CGDisplayModeGetIOFlags(CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Return the IOKit display mode ID of the specified display mode. */

CG_EXTERN int32_t CGDisplayModeGetIODisplayModeID(
    CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Return true if the specified mode is usable for displaying the
   desktop GUI; false otherwise. */

CG_EXTERN bool CGDisplayModeIsUsableForDesktopGUI(
    CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Return the CFTypeID for CGDisplayModeRefs. */

CG_EXTERN CFTypeID CGDisplayModeGetTypeID(void)
    CG_AVAILABLE_STARTING(10.6);

/* Equivalent to `CFRetain(mode)', except it doesn't crash (as CFRetain
   does) if `mode' is NULL. */

CG_EXTERN CGDisplayModeRef __nullable CGDisplayModeRetain(
    CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Equivalent to `CFRelease(mode)', except it doesn't crash (as CFRelease
   does) if `mode' is NULL. */

CG_EXTERN void CGDisplayModeRelease(CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.6);

/* Return the width in pixels of the specified display mode. */

CG_EXTERN size_t CGDisplayModeGetPixelWidth(CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.8);

/* Return the height in pixels of the specified display mode. */

CG_EXTERN size_t CGDisplayModeGetPixelHeight(CGDisplayModeRef __nullable mode)
    CG_AVAILABLE_STARTING(10.8);

/* Set the gamma function for `display' by specifying the coefficients of
   the gamma transfer function.

   Gamma values must be greater than 0. Minimum values must be in the
   interval [0, 1). Maximum values must be in the interval (0, 1]. Out of
   range values or maximum values greater than or equal to minimum values
   return `kCGErrorRangeCheck'.

   Values are computed by sampling a function for a range of indexes from 0
   to 1:
     value = Min + ((Max - Min) * pow(index, Gamma))
   The resulting values are converted to a machine-specific format and
   loaded into display hardware. */

typedef float CGGammaValue;
                                              
CG_EXTERN CGError CGSetDisplayTransferByFormula(CGDirectDisplayID display,
    CGGammaValue redMin, CGGammaValue redMax, CGGammaValue redGamma,
    CGGammaValue greenMin, CGGammaValue greenMax, CGGammaValue greenGamma,
    CGGammaValue blueMin, CGGammaValue blueMax, CGGammaValue blueGamma)
    CG_AVAILABLE_STARTING(10.0);

/* Return the coefficients of the gamma transfer function for `display'. */

CG_EXTERN CGError CGGetDisplayTransferByFormula(CGDirectDisplayID display,
    CGGammaValue * __nullable redMin, CGGammaValue * __nullable redMax,
    CGGammaValue * __nullable redGamma,
    CGGammaValue * __nullable greenMin, CGGammaValue * __nullable greenMax,
    CGGammaValue * __nullable greenGamma,
    CGGammaValue * __nullable blueMin, CGGammaValue * __nullable blueMax,
    CGGammaValue * __nullable blueGamma)
    CG_AVAILABLE_STARTING(10.0);

/* Return the capacity, or number of entries, in the gamma table for
   `display', or 0 if 'display' is invalid. */

CG_EXTERN uint32_t CGDisplayGammaTableCapacity(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.3);

/* Set the gamma function for `display' by specifying the values in the RGB
   gamma tables.

   Values within each table should be in the interval [0, 1] The same table
   may be passed in for red, green, and blue channels. The number of entries
   in the tables is specified by `tableSize'. The tables are interpolated as
   needed to generate the number of samples needed by the display hardware. */

CG_EXTERN CGError CGSetDisplayTransferByTable(CGDirectDisplayID display,
    uint32_t tableSize, const CGGammaValue * __nullable redTable,
    const CGGammaValue * __nullable greenTable,
    const CGGammaValue * __nullable blueTable)
    CG_AVAILABLE_STARTING(10.0);

/* Return the RGB gamma table values for `display'.

   The number of entries in each array is specified by `capacity'; no more
   than `capacity' entries will be written to each table. The number of
   entries written is stored in `sampleCount'. */

CG_EXTERN CGError CGGetDisplayTransferByTable(CGDirectDisplayID display,
    uint32_t capacity, CGGammaValue * __nullable redTable,
    CGGammaValue * __nullable greenTable,
    CGGammaValue * __nullable blueTable, uint32_t * __nullable sampleCount)
    CG_AVAILABLE_STARTING(10.0);

/* Set the gamma function for `display' by specifying the values in the RGB
   gamma tables as bytes.

   Values within each table should be in the interval [0, 255] The same
   table may be passed in for red, green, and blue channels. The number of
   entries in the tables is specified by `tableSize'. The tables are
   interpolated as needed to generate the number of samples needed by the
   display hardware. */

CG_EXTERN CGError CGSetDisplayTransferByByteTable(CGDirectDisplayID display,
    uint32_t tableSize, const uint8_t *  redTable,
    const uint8_t *  greenTable, const uint8_t *  blueTable)
    CG_AVAILABLE_STARTING(10.0);

/* Restore the gamma tables of all system displays to the values in the
   user's ColorSync display profile. */

CG_EXTERN void CGDisplayRestoreColorSyncSettings(void)
    CG_AVAILABLE_STARTING(10.0);

/* Options used with `CGDisplayCaptureWithOptions' and
   `CGCaptureAllDisplaysWithOptions'. */

typedef CF_OPTIONS(uint32_t, CGCaptureOptions) {
    kCGCaptureNoOptions = 0,	/* Default behavior. */
    kCGCaptureNoFill __CG_DEPRECATED_ENUMERATOR = (1 << 0)	/* Disables fill with black on capture. */
};

/* Return true if `display' is captured; false otherwise. */

CG_EXTERN boolean_t CGDisplayIsCaptured(CGDirectDisplayID display)
    CG_AVAILABLE_BUT_DEPRECATED(10.0,10.9);

/* Capture `display' for exclusive use by an application. */

CG_EXTERN CGError CGDisplayCapture(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Capture `display' for exclusive use by an application, using the options
   specified by `options'. */

CG_EXTERN CGError CGDisplayCaptureWithOptions(CGDirectDisplayID display,
    CGCaptureOptions options)
    CG_AVAILABLE_STARTING(10.3);

/* Release the captured display `display'. */

CG_EXTERN CGError CGDisplayRelease(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Capture all displays. This operation provides an immersive environment
   for an appplication, and prevents other applications from trying to
   adjust to display changes. */

CG_EXTERN CGError CGCaptureAllDisplays(void)
    CG_AVAILABLE_STARTING(10.0);

/* Capture all displays, using the options specified by `options'. This
   operation provides an immersive environment for an appplication, and
   prevents other applications from trying to adjust to display changes. */

CG_EXTERN CGError CGCaptureAllDisplaysWithOptions(CGCaptureOptions options)
    CG_AVAILABLE_STARTING(10.3);

/* Release all captured displays and restore the display modes to the user's
   preferences. May be used in conjunction with `CGDisplayCapture' or
   `CGCaptureAllDisplays'. */

CG_EXTERN CGError CGReleaseAllDisplays(void)
    CG_AVAILABLE_STARTING(10.0);

/* Returns window ID of the shield window for the captured display `display',
   or NULL if the display is not not shielded. */

CG_EXTERN CGWindowID CGShieldingWindowID(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Returns the window level of the shield window for the captured display
   `display'. */

CG_EXTERN CGWindowLevel CGShieldingWindowLevel(void)
    CG_AVAILABLE_STARTING(10.0);

/* Return an image containing the contents of the display identified by
   `displayID'. */

CG_EXTERN CGImageRef __nullable CGDisplayCreateImage(CGDirectDisplayID displayID)
    CG_AVAILABLE_STARTING(10.6);

/* Return an image containing the contents of the rectangle `rect',
   specified in display space, of the display identified by `displayID'. The
   actual rectangle used is the rectangle returned from
   `CGRectIntegral(rect)'. */

CG_EXTERN CGImageRef __nullable CGDisplayCreateImageForRect(
    CGDirectDisplayID display, CGRect rect)
    CG_AVAILABLE_STARTING(10.6);

/* Hide the mouse cursor and increment the hide cursor count. The `display'
   parameter is ignored. */

CG_EXTERN CGError CGDisplayHideCursor(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Decrement the hide cursor count; show the cursor if the hide cursor count
   is zero. The `display' parameter is ignored. */

CG_EXTERN CGError CGDisplayShowCursor(CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.0);

/* Move the mouse cursor to the specified point relative to the origin (the
   upper-left corner) of `display'. No events are generated as a result of
   the move. Points that lie outside the desktop are clipped to the
   desktop. */

CG_EXTERN CGError CGDisplayMoveCursorToPoint(CGDirectDisplayID display,
    CGPoint point)
    CG_AVAILABLE_STARTING(10.0);

/* Return the mouse position change since with the last mouse move event
   received by the application. */

CG_EXTERN void CGGetLastMouseDelta(int32_t *__nullable deltaX,
    int32_t * __nullable deltaY)
    CG_AVAILABLE_STARTING(10.0);

/* Return a CGContext suitable for drawing to the captured display
   `display', or NULL if `display' has not been captured. The context is
   owned by the device and should not be released by the caller.

   The context remains valid while the display is captured and while the
   display configuration is unchanged. Releasing the captured display or
   reconfiguring the display invalidates the drawing context.

   The determine when the display configuration is changing, use
   `CGDisplayRegisterReconfigurationCallback'. */

CG_EXTERN CGContextRef __nullable CGDisplayGetDrawingContext(
    CGDirectDisplayID display)
    CG_AVAILABLE_STARTING(10.3);

/* These are deprecated; don't use them. */

#define kCGDisplayWidth                     CFSTR("Width")
#define kCGDisplayHeight                    CFSTR("Height")
#define kCGDisplayMode                      CFSTR("Mode")
#define kCGDisplayBitsPerPixel              CFSTR("BitsPerPixel")
#define kCGDisplayBitsPerSample             CFSTR("BitsPerSample")
#define kCGDisplaySamplesPerPixel           CFSTR("SamplesPerPixel")
#define kCGDisplayRefreshRate               CFSTR("RefreshRate")
#define kCGDisplayModeUsableForDesktopGUI	CFSTR("UsableForDesktopGUI")
#define kCGDisplayIOFlags                   CFSTR("IOFlags")
#define kCGDisplayBytesPerRow               CFSTR("kCGDisplayBytesPerRow")
#define kCGIODisplayModeID                  CFSTR("IODisplayModeID")

#define kCGDisplayModeIsSafeForHardware             \
  CFSTR("kCGDisplayModeIsSafeForHardware")
#define kCGDisplayModeIsInterlaced					\
  CFSTR("kCGDisplayModeIsInterlaced") 
#define kCGDisplayModeIsStretched					\
  CFSTR("kCGDisplayModeIsStretched")
#define kCGDisplayModeIsTelevisionOutput            \
  CFSTR("kCGDisplayModeIsTelevisionOutput")

/* These types are deprecated; don't use them. */

typedef uint32_t CGDisplayCount;
typedef CGError CGDisplayErr;
#define CGDisplayNoErr kCGErrorSuccess

/* These functions are deprecated; do not use them. */

/* Use the CGDisplayMode APIs instead. */
CG_EXTERN CFArrayRef __nullable CGDisplayAvailableModes(CGDirectDisplayID dsp)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.6);

/* Use the CGDisplayMode APIs instead. */
CG_EXTERN CFDictionaryRef __nullable CGDisplayBestModeForParameters(
    CGDirectDisplayID display, size_t bitsPerPixel, size_t width, size_t height,
    boolean_t * __nullable exactMatch)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.6);

/* Use the CGDisplayMode APIs instead. */
CG_EXTERN CFDictionaryRef __nullable CGDisplayBestModeForParametersAndRefreshRate(
    CGDirectDisplayID display, size_t bitsPerPixel, size_t width, size_t height,
    CGRefreshRate refreshRate, boolean_t * __nullable exactMatch)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.6);

/* Use the CGDisplayMode APIs instead. */
CG_EXTERN CFDictionaryRef __nullable CGDisplayCurrentMode(
    CGDirectDisplayID display)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.6);

/* Use the CGDisplayMode APIs instead. */
CG_EXTERN CGError CGDisplaySwitchToMode(CGDirectDisplayID display,
    CFDictionaryRef __nullable mode)
    CG_AVAILABLE_BUT_DEPRECATED(10.0, 10.6);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED


#endif /* CGDIRECTDISPLAY_H_ */
