/* CoreGraphics - CGEventSource.h
   Copyright (c) 2004-2008 Apple Inc.
   All rights reserved. */

#ifndef CGEVENTSOURCE_H_
#define CGEVENTSOURCE_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#include <CoreGraphics/CGRemoteOperation.h>
#include <CoreGraphics/CGEventTypes.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* An event source contains accumulated state related to event generation
   and event posting, allowing for customized event generation and
   processing.

   A source state, represented by a `CGEventSourceStateID', refers to a
   global event state table. These tables contain accumulated information on
   modifier flag state, keyboard key state, mouse button state, and related
   internal parameters placed in effect by posting events with associated
   sources.

   Two pre-existing event state tables are defined:

     The `kCGEventSourceStateCombinedSessionState' table reflects the
     combined state of all event sources posting to the current user login
     session. If your program is posting events from within a login session,
     you should use this source state when you create an event source.

     The `kCGEventSourceStateHIDSystemState' table reflects the combined
     state of all hardware event sources posting from the HID system. If
     your program is a daemon or a user space device driver interpreting
     hardware state and generating events, you should use this source state
     when you create an event source.

   Very specialized applications such as remote control programs may want to
   generate and track event source state independent of other processes.
   These programs should use the `kCGEventSourceStatePrivate' value in
   creating their event source. An independent state table and unique source
   state ID (`CGEventSourceStateID') are created to track the event source's
   state. This independent state table is owned by the creating event source
   and released with it. */

/* Return the CFTypeID for CGEventSourceRefs. */

CG_EXTERN CFTypeID CGEventSourceGetTypeID(void)
    CG_AVAILABLE_STARTING(10.4);

/* Return a Quartz event source created with a specified source state. */

CG_EXTERN CGEventSourceRef __nullable CGEventSourceCreate(
    CGEventSourceStateID stateID)
    CG_AVAILABLE_STARTING(10.4);

/* Return the keyboard type to be used with a Quartz event source.
   Result of CGSDefaultKeyboardID() is returned if source is not a valid
   CGEventSourceRef. */

CG_EXTERN CGEventSourceKeyboardType CGEventSourceGetKeyboardType(
    CGEventSourceRef __nullable source)
    CG_AVAILABLE_STARTING(10.4);

/* Set the keyboard type to be used with a Quartz event source. */

CG_EXTERN void CGEventSourceSetKeyboardType(CGEventSourceRef __nullable source,
    CGEventSourceKeyboardType keyboardType)
    CG_AVAILABLE_STARTING(10.4);

/* Return the scale of pixels per line in a scrolling event source.

   This function returns the scale of pixels per line in the specified event
   source. For example, if the scale in the event source is 10.5 pixels per
   line, this function would return 10.5. Every scrolling event can be
   interpreted to be scrolling by pixel or by line. By default, the scale is
   about ten pixels per line. kCGSDefaultScrollwheelScale is returned in case 
   of invalid source */

CG_EXTERN double CGEventSourceGetPixelsPerLine(
    CGEventSourceRef __nullable source)
    CG_AVAILABLE_STARTING(10.5);

/* Set the scale of pixels per line in a scrolling event source.

   This function sets the scale of pixels per line in the specified event
   source. For example, if you pass the value 12 as the `pixelsPerLine'
   parameter, the scale of pixels per line in the event source would be
   changed to 12. Every scrolling event can be interpreted to be scrolling
   by pixel or by line. By default, the scale is about ten pixels per
   line. */

CG_EXTERN void CGEventSourceSetPixelsPerLine(CGEventSourceRef __nullable source,
    double pixelsPerLine)
    CG_AVAILABLE_STARTING(10.5);

/* Return the source state associated with a Quartz event source.

   For event sources created with the `kCGEventSourceStatePrivate' source
   state, this function returns the ID of the private source state table
   created for the event source. This unique ID may be passed to the
   `CGEventSourceCreate' function to create a second event source sharing
   the same state table. This may be useful, for example, in creating
   separate mouse and keyboard sources which share a common private state.
   kCGEventSourceStateCombinedSessionState is returned in case of invalid
   source. */

CG_EXTERN CGEventSourceStateID CGEventSourceGetSourceStateID(
    CGEventSourceRef __nullable source)
    CG_AVAILABLE_STARTING(10.4);

/* Return a Boolean value indicating the current button state of a Quartz
   event source. If true, the button is down; if false, the button is up. */

CG_EXTERN bool CGEventSourceButtonState(CGEventSourceStateID stateID,
    CGMouseButton button)
    CG_AVAILABLE_STARTING(10.4);

/* Return a Boolean value indicating the current keyboard state of a Quartz
   event source. If true, the key is down; if false, the key is up. */

CG_EXTERN bool CGEventSourceKeyState(CGEventSourceStateID stateID,
    CGKeyCode key)
    CG_AVAILABLE_STARTING(10.4);

/* Return the current flags of a Quartz event source. If true, the key is
   down; if false, the key is up. */

CG_EXTERN CGEventFlags CGEventSourceFlagsState(CGEventSourceStateID stateID)
    CG_AVAILABLE_STARTING(10.4);

/* Return the elapsed time since the last event for a Quartz event source.

   To get the elapsed time since the previous input event --- keyboard,
   mouse, or tablet --- specify `kCGAnyInputEventType'. */

CG_EXTERN CFTimeInterval CGEventSourceSecondsSinceLastEventType(
    CGEventSourceStateID stateID, CGEventType eventType)
    CG_AVAILABLE_STARTING(10.4);

/* Return a count of events of a given type seen since the Window Server
   started.

   Modifier keys produce `kCGEventFlagsChanged' events, not `kCGEventKeyDown'
   events, and do so both on press and release. The volume, brightness, and
   CD eject keys on some keyboards (both desktop and laptop) do not generate
   key up or key down events.

   For various reasons, the number of key up and key down events may not be
   the same when all keyboard keys are up. As a result, a mismatch does not
   necessarily indicate that some keys are down.

   Key autorepeat events are not counted. */

CG_EXTERN uint32_t CGEventSourceCounterForEventType(CGEventSourceStateID
    stateID, CGEventType eventType)
    CG_AVAILABLE_STARTING(10.4);

/* Set the 64-bit user-specified data for a Quartz event source.

   Each input event includes 64 bits of user-specified data. This function
   sets the user-specified data for all events created by the specified
   event source. This data may also be set per event using the
   `CGEventGetIntegerValueField' function. */

CG_EXTERN void CGEventSourceSetUserData(CGEventSourceRef __nullable source,
    int64_t userData)
    CG_AVAILABLE_STARTING(10.4);

/* Return the 64-bit user-specified data for a Quartz event source.

   Each input event includes 64 bits of user-specified data. This function
   gets the user-specified data for all events created by the specified
   event source. This data may also be obtained per event using the
   `CGEventGetIntegerValueField' function. 0 is returned in case of invalid
   source. */

CG_EXTERN int64_t CGEventSourceGetUserData(CGEventSourceRef __nullable source)
    CG_AVAILABLE_STARTING(10.4);

/* Set the mask that indicates which classes of local hardware events are
   enabled during event suppression.

   By default, the system does not suppress local hardware events from the
   keyboard or mouse during a short interval after a Quartz event is posted
   --- see `CGEventSourceSetLocalEventsSuppressionInterval' --- and during a
   synthetic mouse drag (mouse movement with the left or only mouse button
   down).

   Some applications may want to disable events from some of the local
   hardware during this interval. For example, if you post mouse events
   only, you may wish to suppress local mouse events and permit local
   keyboard events to pass through. This function lets you specify an event
   source, a suppression state (event suppression interval or mouse drag),
   and a filter mask of event classes to be passed through. The new local
   events filter takes effect with the next Quartz event you post using this
   event source. */

CG_EXTERN void CGEventSourceSetLocalEventsFilterDuringSuppressionState(
    CGEventSourceRef __nullable source, CGEventFilterMask filter,
    CGEventSuppressionState state)
    CG_AVAILABLE_STARTING(10.4);


/* Return the mask that indicates which classes of local hardware events are
   enabled during event suppression.

   You can configure the system to suppress local hardware events from the
   keyboard or mouse during a short interval after a Quartz event is posted
   or during a synthetic mouse drag (mouse movement with the left or only
   mouse button down). For information about setting this local events
   filter, see `CGEventSourceSetLocalEventsFilterDuringSuppressionState'.

   This function lets you specify an event source and a suppression state
   (event suppression interval or mouse drag), and returns a filter mask of
   event categories to be passed through during suppression.
   (CGEventFilterMask)0 is retuned in case of invalid source. */

CG_EXTERN CGEventFilterMask
    CGEventSourceGetLocalEventsFilterDuringSuppressionState(
    CGEventSourceRef __nullable source, CGEventSuppressionState state)
    CG_AVAILABLE_STARTING(10.4);

/* Set the interval that local hardware events may be suppressed following
   the posting of a Quartz event.

   This function sets the period of time in seconds that local hardware
   events may be suppressed after posting a Quartz event created with the
   specified event source. The default suppression interval is 0.25
   seconds. */

CG_EXTERN void CGEventSourceSetLocalEventsSuppressionInterval(
    CGEventSourceRef __nullable source, CFTimeInterval seconds)
    CG_AVAILABLE_STARTING(10.4);

/* Return the interval that local hardware events may be suppressed
   following the posting of a Quartz event.
   kCGSDefaultLocalEventSuppressionInterval is retuned in case of invalid
   source.*/

CG_EXTERN CFTimeInterval CGEventSourceGetLocalEventsSuppressionInterval(
    CGEventSourceRef __nullable source)
    CG_AVAILABLE_STARTING(10.4);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGEVENTSOURCE_H_ */
