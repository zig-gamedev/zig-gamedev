/* CoreGraphics - CGEvent.h
   Copyright (c) 2004-2013 Apple Inc.
   All rights reserved. */

#ifndef CGEVENT_H_
#define CGEVENT_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#include <CoreGraphics/CGEventTypes.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Return the type identifier for the opaque type `CGEventRef'. */

CG_EXTERN CFTypeID CGEventGetTypeID(void)
    CG_AVAILABLE_STARTING(10.4);

/* Return a new event using the event source `source'. If `source' is NULL,
   the default source is used. */

CG_EXTERN CGEventRef __nullable CGEventCreate(CGEventSourceRef __nullable source)
    CG_AVAILABLE_STARTING(10.4);

/* Return a "flattened" data representation of an event. */

CG_EXTERN CFDataRef __nullable CGEventCreateData(
    CFAllocatorRef __nullable allocator,
    CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Return an event created from a "flattened" data representation of the
   event. */

CG_EXTERN CGEventRef __nullable CGEventCreateFromData(
    CFAllocatorRef __nullable allocator, CFDataRef __nullable data)
    CG_AVAILABLE_STARTING(10.4);

/* Return a new mouse event.

   The event source may be taken from another event, or may be NULL.
   `mouseType' should be one of the mouse event types. `mouseCursorPosition'
   should be the position of the mouse cursor in global coordinates.
   `mouseButton' should be the button that's changing state; `mouseButton'
   is ignored unless `mouseType' is one of `kCGEventOtherMouseDown',
   `kCGEventOtherMouseDragged', or `kCGEventOtherMouseUp'.

   The current implemementation of the event system supports a maximum of
   thirty-two buttons. Mouse button 0 is the primary button on the mouse.
   Mouse button 1 is the secondary mouse button (right). Mouse button 2 is
   the center button, and the remaining buttons are in USB device order. */

CG_EXTERN CGEventRef __nullable CGEventCreateMouseEvent(
    CGEventSourceRef __nullable source,
    CGEventType mouseType, CGPoint mouseCursorPosition,
    CGMouseButton mouseButton)
    CG_AVAILABLE_STARTING(10.4);

/* Return a new keyboard event.

   The event source may be taken from another event, or may be NULL. Based
   on the virtual key code values entered, the appropriate key down, key up,
   or flags changed events are generated.

   All keystrokes needed to generate a character must be entered, including
   SHIFT, CONTROL, OPTION, and COMMAND keys. For example, to produce a 'Z',
   the SHIFT key must be down, the 'z' key must go down, and then the SHIFT
   and 'z' key must be released:

     CGEventCreateKeyboardEvent(source, (CGKeyCode)56, true);  // shift down
     CGEventCreateKeyboardEvent(source, (CGKeyCode) 6, true);  // 'z' down
     CGEventCreateKeyboardEvent(source, (CGKeyCode) 6, false); // 'z' up
     CGEventCreateKeyboardEvent(source, (CGKeyCode)56, false); // 'shift up */

CG_EXTERN CGEventRef __nullable CGEventCreateKeyboardEvent(
    CGEventSourceRef __nullable source,
    CGKeyCode virtualKey, bool keyDown)
    CG_AVAILABLE_STARTING(10.4);
                                                 
/* Return a new scrollwheel event.

   The event source may be taken from another event, or may be NULL. The
   scrolling units may be specified in lines using `kCGScrollEventUnitLine'
   or in pixels using `kCGScrollEventUnitPixel'. `kCGScrollEventUnitPixel'
   will produce an event that most applications interpret as a smooth
   scrolling event.

   One or more wheels must be specified. The current implementation supports
   up to three wheels.

   Every scrollwheel event can be interpreted to be scrolling by pixel or by
   line. The scale between the two is about 10 pixels per line by default.
   The scale can be altered by setting a custom value for the event source,
   using `CGEventSourceSetPixelsPerLine'. */
 
CG_EXTERN CGEventRef __nullable CGEventCreateScrollWheelEvent(
    CGEventSourceRef __nullable source,
    CGScrollEventUnit units, uint32_t wheelCount, int32_t wheel1, ...)
    CG_AVAILABLE_STARTING(10.5);

/* A non-variadic variant version of SLEventCreateScrollWheelEvent. */

CG_EXTERN CGEventRef __nullable CGEventCreateScrollWheelEvent2(
    CGEventSourceRef __nullable source,
    CGScrollEventUnit units, uint32_t wheelCount, int32_t wheel1, int32_t wheel2, int32_t wheel3)
    CG_AVAILABLE_STARTING(10.13);

/* Return a copy of `event'. */

CG_EXTERN CGEventRef __nullable CGEventCreateCopy(CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Return an event source created from an existing event.

   Event filters may use the event source to generate events that are
   compatible with an event being filtered.

   Note that `CGEventCreateSourceFromEvent' may return NULL if the event
   was generated with a private CGEventSourceStateID owned by another
   process.  Such events should be filtered based on the public state. */

CG_EXTERN CGEventSourceRef __nullable CGEventCreateSourceFromEvent(
    CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Set the event source of an event. */

CG_EXTERN void CGEventSetSource(CGEventRef __nullable event,
    CGEventSourceRef __nullable source)
    CG_AVAILABLE_STARTING(10.4);

/* Return the event type of an event (left mouse down, for example). */

CG_EXTERN CGEventType CGEventGetType(CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Set the event type of an event. */

CG_EXTERN void CGEventSetType(CGEventRef __nullable event, CGEventType type)
    CG_AVAILABLE_STARTING(10.4);

/* Return the timestamp of an event. */

CG_EXTERN CGEventTimestamp CGEventGetTimestamp(CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Set the timestamp of an event. */

CG_EXTERN void CGEventSetTimestamp(CGEventRef __nullable event,
    CGEventTimestamp timestamp)
    CG_AVAILABLE_STARTING(10.4);

/* Return the location of an event in global display coordinates.
   CGPointZero is returned if event is not a valid CGEventRef. */

CG_EXTERN CGPoint CGEventGetLocation(CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Return the location of an event relative to the lower-left corner of the
   main display. CGPointZero is returned if event is not a valid CGEventRef. */

CG_EXTERN CGPoint CGEventGetUnflippedLocation(CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.5);

/* Set the location of an event in global display coordinates. */

CG_EXTERN void CGEventSetLocation(CGEventRef __nullable event, CGPoint location)
    CG_AVAILABLE_STARTING(10.4);

/* Return the event flags of an event. (CGEventFlags)0 if event is not a valid 
   CGEventRef. */

CG_EXTERN CGEventFlags CGEventGetFlags(CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Set the event flags of an event. */

CG_EXTERN void CGEventSetFlags(CGEventRef __nullable event, CGEventFlags flags)
    CG_AVAILABLE_STARTING(10.4);

/* Return the Unicode string associated with a keyboard event.

   When you call this function with a NULL string or a maximum string length
   of 0, the function still returns the actual count of Unicode characters
   in the event. */

CG_EXTERN void CGEventKeyboardGetUnicodeString(CGEventRef __nullable event,
    UniCharCount maxStringLength, UniCharCount *__nullable actualStringLength,
    UniChar * __nullable unicodeString)
    CG_AVAILABLE_STARTING(10.4);

/* Set the Unicode string associated with a keyboard event.

   By default, the system translates the virtual key code in a keyboard
   event into a Unicode string based on the keyboard ID in the event source.
   This function allows you to manually override this string. Note that
   application frameworks may ignore the Unicode string in a keyboard event
   and do their own translation based on the virtual keycode and perceived
   event state. */

CG_EXTERN void CGEventKeyboardSetUnicodeString(CGEventRef __nullable event,
    UniCharCount stringLength, const UniChar * __nullable unicodeString)
    CG_AVAILABLE_STARTING(10.4);

/* Return the integer value of a field in an event. */

CG_EXTERN int64_t CGEventGetIntegerValueField(CGEventRef __nullable event,
    CGEventField field)
    CG_AVAILABLE_STARTING(10.4);

/* Set the integer value of a field in an event.

   Before calling this function, the event type must be set using a typed
   event creation function such as `CGEventCreateMouseEvent', or by calling
   `CGEventSetType'.

   If you are creating a mouse event generated by a tablet, call this
   function and specify the field `kCGMouseEventSubtype' with a value of
   `kCGEventMouseSubtypeTabletPoint' or `kCGEventMouseSubtypeTabletProximity'
   before setting other parameters. */

CG_EXTERN void CGEventSetIntegerValueField(CGEventRef __nullable event,
    CGEventField field, int64_t value)
    CG_AVAILABLE_STARTING(10.4);

/* Return the floating-point value of a field in an event.

   In cases where the field value is represented within the event by a fixed
   point number or an integer, the result is scaled to the appropriate range
   as part of creating the floating-point representation. */

CG_EXTERN double CGEventGetDoubleValueField(CGEventRef __nullable event,
    CGEventField field)
    CG_AVAILABLE_STARTING(10.4);

/* Set the floating-point value of a field in an event.

   Before calling this function, the event type must be set using a typed
   event creation function such as `CGEventCreateMouseEvent', or by calling
   `CGEventSetType'.

   In cases where the field’s value is represented within the event by a
   fixed point number or integer, the value parameter is scaled as needed
   and converted to the appropriate type. */

CG_EXTERN void CGEventSetDoubleValueField(CGEventRef __nullable event,
    CGEventField field, double value)
    CG_AVAILABLE_STARTING(10.4);

/* Event taps.

   Taps may be placed at the point where HIDSystem events enter the server,
   at the point where HIDSystem and remote control events enter a session,
   at the point where events have been annotated to flow to a specific
   application, or at the point where events are delivered to the
   application. Taps may be inserted at a specified point at the head of
   pre-existing filters, or appended after any pre-existing filters.

   Taps may be passive event listeners, or active filters. An active filter
   may pass an event through unmodified, modify an event, or discard an
   event. When a tap is registered, it identifies the set of events to be
   observed with a mask, and indicates if it is a passive or active event
   filter. Multiple event type bitmasks may be ORed together.

   Taps may only be placed at `kCGHIDEventTap' by a process running as the
   root user. NULL is returned for other users.

   Taps placed at `kCGHIDEventTap', `kCGSessionEventTap',
   `kCGAnnotatedSessionEventTap', or on a specific process may only receive
   key up and down events if access for assistive devices is enabled
   (Preferences Accessibility panel, Keyboard view) or the caller is enabled
   for assistive device access, as by `AXMakeProcessTrusted'. If the tap is
   not permitted to monitor these events when the tap is created, then the
   appropriate bits in the mask are cleared. If that results in an empty
   mask, then NULL is returned.

   An event tap is represented as a `CFMachPortRef'. The event tap may be
   added to an appropriate run loop by calling `CFRunLoopAddSource'.

   Releasing the CFMachPortRef will release the tap.

   A `CGEventTapProxy' is an opaque reference to state within the client
   application associated with the tap. The tap function may pass this
   reference to other functions, such as the event-posting routines.

   The event tap callback runs from the CFRunLoop to which the tap
   CFMachPort is added as a source. Thread safety is defined by the
   CFRunLoop and its environment. */

/* Create an event tap. */

CG_EXTERN CFMachPortRef __nullable CGEventTapCreate(CGEventTapLocation tap,
    CGEventTapPlacement place, CGEventTapOptions options,
    CGEventMask eventsOfInterest, CGEventTapCallBack callback,
    void * __nullable userInfo)
    CG_AVAILABLE_STARTING(10.4);

/* Create an event tap for a specified process.

   Events routed to individual applications may be tapped using another
   function. `CGEventTapCreateForPSN' will report all events routed to the
   specified application. */

CG_EXTERN CFMachPortRef __nullable CGEventTapCreateForPSN(
    void *  processSerialNumber,
    CGEventTapPlacement place, CGEventTapOptions options,
    CGEventMask eventsOfInterest, CGEventTapCallBack callback,
    void *__nullable userInfo)
    CG_AVAILABLE_STARTING(10.4);

CG_EXTERN CFMachPortRef __nullable CGEventTapCreateForPid(pid_t pid,
  CGEventTapPlacement place, CGEventTapOptions options,
  CGEventMask eventsOfInterest, CGEventTapCallBack callback,
  void * __nullable userInfo) CG_AVAILABLE_STARTING(10.11);

/* Enable or disable an event tap.

   Taps are normally enabled when created. If a tap becomes unresponsive or
   a user requests taps be disabled, an appropriate `kCGEventTapDisabled...'
   event is passed to the registered CGEventTapCallBack function. An event
   tap may be re-enabled by calling this function. */

CG_EXTERN void CGEventTapEnable(CFMachPortRef  tap, bool enable)
    CG_AVAILABLE_STARTING(10.4);

/* Return true if `tap' is enabled; false otherwise. */

CG_EXTERN bool CGEventTapIsEnabled(CFMachPortRef  tap)
    CG_AVAILABLE_STARTING(10.4);

/* Post an event from an event tap into the event stream.

   You can use this function to post a new event at the same point to which
   an event returned from an event tap callback function would be posted.
   The new event enters the system before the event returned by the callback
   enters the system. Events posted into the system will be seen by all taps
   placed after the tap posting the event. */

CG_EXTERN void CGEventTapPostEvent(CGEventTapProxy __nullable proxy,
    CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Post an event into the event stream at a specified location.

   This function posts the specified event immediately before any event taps
   instantiated for that location, and the event passes through any such
   taps. */

CG_EXTERN void CGEventPost(CGEventTapLocation tap, CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

/* Post an event into the event stream for a specific application.

   This function makes it possible for an application to establish
   an event routing policy, for example, by tapping events at the
   `kCGAnnotatedSessionEventTap' location and then posting the events
   to another desired process.

   This function posts the specified event immediately before any event taps
   instantiated for the specified process, and the event passes through any
   such taps. */

/* DEPRECATED; use CGEventPostToPid instead. */
CG_EXTERN void CGEventPostToPSN(void * __nullable processSerialNumber,
    CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.4);

CG_EXTERN void CGEventPostToPid( pid_t pid,
    CGEventRef __nullable event)
    CG_AVAILABLE_STARTING(10.11);

/* Gets a list of currently installed event taps.

   `tapList' is an array of event tap information structures of length
   `maxNumberOfTaps'. You are responsible for allocating storage for this
   array. On return, your array contains a list of currently installed event
   taps. On return, the number of event taps that are currently installed is
   stored in `eventTapCount'. If you pass NULL in this parameter, the
   `maxNumberOfTaps' parameter is ignored, and the number of event taps that
   are currently installed is stored in `eventTapCount'.

   Each call to this function has the side effect of resetting the minimum
   and maximum latencies in the `tapList' parameter to the corresponding
   average values. Values reported in these fields reflect the minimum and
   maximum values seen since the preceding call, or the instantiation of the
   tap. This allows a monitoring tool to evaluate the best and worst case
   latency over time and under various operating conditions. */

CG_EXTERN CGError CGGetEventTapList(uint32_t maxNumberOfTaps,
    CGEventTapInformation * __nullable tapList,
    uint32_t * __nullable eventTapCount)
    CG_AVAILABLE_STARTING(10.4);

/* Checks whether the current process already has event listening access */
CG_EXTERN bool CGPreflightListenEventAccess(void) CG_AVAILABLE_STARTING(10.15);

/* Requests event listening access if absent, potentially prompting */
CG_EXTERN bool CGRequestListenEventAccess(void) CG_AVAILABLE_STARTING(10.15);

/* Checks whether the current process already has event synthesizing access */
CG_EXTERN bool CGPreflightPostEventAccess(void) CG_AVAILABLE_STARTING(10.15);

/* Requests event synthesizing access if absent, potentially prompting */
CG_EXTERN bool CGRequestPostEventAccess(void) CG_AVAILABLE_STARTING(10.15);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGEVENT_H_ */
