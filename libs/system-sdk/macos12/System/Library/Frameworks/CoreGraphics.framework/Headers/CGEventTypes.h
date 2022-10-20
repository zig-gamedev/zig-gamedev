/* CoreGraphics - CGEventTypes.h
   Copyright (c) 2004-2008 Apple Inc.
   All rights reserved. */

#ifndef CGEVENTTYPES_H_
#define CGEVENTTYPES_H_


#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGRemoteOperation.h>
#include <IOKit/hidsystem/IOLLEvent.h>

CF_ASSUME_NONNULL_BEGIN

/* An opaque type that represents a low-level hardware event.

   Low-level hardware events of this type are referred to as Quartz events.
   A typical event in Mac OS X originates when the user manipulates an input
   device such as a mouse or a keyboard. The device driver associated with
   that device, through the I/O Kit, creates a low-level event, puts it in
   the window server’s event queue, and notifies the window server. The
   window server creates a Quartz event, annotates the event, and dispatches
   the event to the appropriate run-loop port of the target process. There
   the event is picked up by the Carbon Event Manager and forwarded to the
   event-handling mechanism appropriate to the application environment. You
   can use event taps to gain access to Quartz events at several different
   steps in this process.

   This opaque type is derived from `CFType' and inherits the properties
   that all Core Foundation types have in common. */

typedef struct CF_BRIDGED_TYPE(id) __CGEvent *CGEventRef;

/* Constants that specify buttons on a one, two, or three-button mouse. */
typedef CF_ENUM(uint32_t, CGMouseButton) {
  kCGMouseButtonLeft = 0,
  kCGMouseButtonRight = 1,
  kCGMouseButtonCenter = 2
};

/* Constants that specify the unit of measurement for a scrolling event. */
typedef CF_ENUM(uint32_t, CGScrollEventUnit) {
  kCGScrollEventUnitPixel = 0,
  kCGScrollEventUnitLine = 1,
};

/* Constants that specify momentum scroll phases. */
typedef CF_ENUM(uint32_t, CGMomentumScrollPhase) {
    kCGMomentumScrollPhaseNone = 0,
    kCGMomentumScrollPhaseBegin = 1,
    kCGMomentumScrollPhaseContinue = 2,
    kCGMomentumScrollPhaseEnd = 3
};

/* Constants that specify scroll phases. */
typedef CF_ENUM(uint32_t, CGScrollPhase) {
    kCGScrollPhaseBegan = 1,
    kCGScrollPhaseChanged = 2,
    kCGScrollPhaseEnded = 4,
    kCGScrollPhaseCancelled = 8,
    kCGScrollPhaseMayBegin = 128
};

/* Constants that specify gesture phases. */
typedef CF_ENUM(uint32_t, CGGesturePhase) {
    kCGGesturePhaseNone = 0,
    kCGGesturePhaseBegan = 1,
    kCGGesturePhaseChanged = 2,
    kCGGesturePhaseEnded = 4,
    kCGGesturePhaseCancelled = 8,
    kCGGesturePhaseMayBegin = 128
};

/* Constants that indicate the modifier key state at the time an event is
   created, as well as other event-related states.

   Any bits not specified are reserved for future use. */
typedef CF_OPTIONS(uint64_t, CGEventFlags) { /* Flags for events */
  /* Device-independent modifier key bits. */
  kCGEventFlagMaskAlphaShift =          NX_ALPHASHIFTMASK,
  kCGEventFlagMaskShift =               NX_SHIFTMASK,
  kCGEventFlagMaskControl =             NX_CONTROLMASK,
  kCGEventFlagMaskAlternate =           NX_ALTERNATEMASK,
  kCGEventFlagMaskCommand =             NX_COMMANDMASK,

  /* Special key identifiers. */
  kCGEventFlagMaskHelp =                NX_HELPMASK,
  kCGEventFlagMaskSecondaryFn =         NX_SECONDARYFNMASK,

  /* Identifies key events from numeric keypad area on extended keyboards. */
  kCGEventFlagMaskNumericPad =          NX_NUMERICPADMASK,

  /* Indicates if mouse/pen movement events are not being coalesced */
  kCGEventFlagMaskNonCoalesced =        NX_NONCOALSESCEDMASK
};

/* Constants that specify the different types of input events. */
typedef CF_ENUM(uint32_t, CGEventType) {
  /* The null event. */
  kCGEventNull = NX_NULLEVENT,

  /* Mouse events. */
  kCGEventLeftMouseDown = NX_LMOUSEDOWN,
  kCGEventLeftMouseUp = NX_LMOUSEUP,
  kCGEventRightMouseDown = NX_RMOUSEDOWN,
  kCGEventRightMouseUp = NX_RMOUSEUP,
  kCGEventMouseMoved = NX_MOUSEMOVED,
  kCGEventLeftMouseDragged = NX_LMOUSEDRAGGED,
  kCGEventRightMouseDragged = NX_RMOUSEDRAGGED,

  /* Keyboard events. */
  kCGEventKeyDown = NX_KEYDOWN,
  kCGEventKeyUp = NX_KEYUP,
  kCGEventFlagsChanged = NX_FLAGSCHANGED,

  /* Specialized control devices. */
  kCGEventScrollWheel = NX_SCROLLWHEELMOVED,
  kCGEventTabletPointer = NX_TABLETPOINTER,
  kCGEventTabletProximity = NX_TABLETPROXIMITY,
  kCGEventOtherMouseDown = NX_OMOUSEDOWN,
  kCGEventOtherMouseUp = NX_OMOUSEUP,
  kCGEventOtherMouseDragged = NX_OMOUSEDRAGGED,

  /* Out of band event types. These are delivered to the event tap callback
     to notify it of unusual conditions that disable the event tap. */
  kCGEventTapDisabledByTimeout = 0xFFFFFFFE,
  kCGEventTapDisabledByUserInput = 0xFFFFFFFF
};

/* Event timestamp; roughly, nanoseconds since startup. */
typedef uint64_t CGEventTimestamp;

/* Constants used as keys to access specialized fields in low-level events. */
typedef CF_ENUM(uint32_t, CGEventField) {
  /* Key to access an integer field that contains the mouse button event
     number. Matching mouse-down and mouse-up events will have the same
     event number. */
  kCGMouseEventNumber = 0,

  /* Key to access an integer field that contains the mouse button click
  state. A click state of 1 represents a single click. A click state of 2
  represents a double-click. A click state of 3 represents a
  triple-click. */
  kCGMouseEventClickState = 1,

  /* Key to access a double field that contains the mouse button pressure.
     The pressure value may range from 0 to 1, with 0 representing the mouse
     being up. This value is commonly set by tablet pens mimicking a
     mouse. */
  kCGMouseEventPressure = 2,

  /* Key to access an integer field that contains the mouse button
     number. */
  kCGMouseEventButtonNumber = 3,

  /* Key to access an integer field that contains the horizontal mouse delta
     since the last mouse movement event. */
  kCGMouseEventDeltaX = 4,

  /* Key to access an integer field that contains the vertical mouse delta
     since the last mouse movement event. */
  kCGMouseEventDeltaY = 5,

  /* Key to access an integer field. The value is non-zero if the event
     should be ignored by the Inkwell subsystem. */
  kCGMouseEventInstantMouser = 6,

  /* Key to access an integer field that encodes the mouse event subtype as
     a `kCFNumberIntType'. */
  kCGMouseEventSubtype = 7,

  /* Key to access an integer field, non-zero when this is an autorepeat of
     a key-down, and zero otherwise. */
  kCGKeyboardEventAutorepeat = 8,

  /* Key to access an integer field that contains the virtual keycode of the
     key-down or key-up event. */
  kCGKeyboardEventKeycode = 9,

  /* Key to access an integer field that contains the keyboard type
     identifier. */
  kCGKeyboardEventKeyboardType = 10,

  /* Key to access an integer field that contains scrolling data. This field
     typically contains the change in vertical position since the last
     scrolling event from a Mighty Mouse scroller or a single-wheel mouse
     scroller. */
  kCGScrollWheelEventDeltaAxis1 = 11,

  /* Key to access an integer field that contains scrolling data. This field
     typically contains the change in horizontal position since the last
     scrolling event from a Mighty Mouse scroller. */
  kCGScrollWheelEventDeltaAxis2 = 12,

  /* This field is not used. */
  kCGScrollWheelEventDeltaAxis3 = 13,

  /* Key to access a field that contains scrolling data. The scrolling data
     represents a line-based or pixel-based change in vertical position
     since the last scrolling event from a Mighty Mouse scroller or a
     single-wheel mouse scroller. The scrolling data uses a fixed-point
     16.16 signed integer format. If this key is passed to
     `CGEventGetDoubleValueField', the fixed-point value is converted to a
     double value. */
  kCGScrollWheelEventFixedPtDeltaAxis1 = 93,

  /* Key to access a field that contains scrolling data. The scrolling data
     represents a line-based or pixel-based change in horizontal position
     since the last scrolling event from a Mighty Mouse scroller. The
     scrolling data uses a fixed-point 16.16 signed integer format. If this
     key is passed to `CGEventGetDoubleValueField', the fixed-point value is
     converted to a double value. */
  kCGScrollWheelEventFixedPtDeltaAxis2 = 94,

  /* This field is not used. */
  kCGScrollWheelEventFixedPtDeltaAxis3 = 95,

  /* Key to access an integer field that contains pixel-based scrolling
     data. The scrolling data represents the change in vertical position
     since the last scrolling event from a Mighty Mouse scroller or a
     single-wheel mouse scroller. */
  kCGScrollWheelEventPointDeltaAxis1 = 96,

  /* Key to access an integer field that contains pixel-based scrolling
     data. The scrolling data represents the change in horizontal position
     since the last scrolling event from a Mighty Mouse scroller. */
  kCGScrollWheelEventPointDeltaAxis2 = 97,

  /* This field is not used. */
  kCGScrollWheelEventPointDeltaAxis3 = 98,
    
  /*  */
  kCGScrollWheelEventScrollPhase = 99,
    
  /* rdar://11259169 */
  kCGScrollWheelEventScrollCount = 100,
    
  kCGScrollWheelEventMomentumPhase = 123,
    
  /* Key to access an integer field that indicates whether the event should
     be ignored by the Inkwell subsystem. If the value is non-zero, the
     event should be ignored. */
  kCGScrollWheelEventInstantMouser = 14,

  /* Key to access an integer field that contains the absolute X coordinate
     in tablet space at full tablet resolution. */
  kCGTabletEventPointX = 15,

  /* Key to access an integer field that contains the absolute Y coordinate
     in tablet space at full tablet resolution. */
  kCGTabletEventPointY = 16,

  /* Key to access an integer field that contains the absolute Z coordinate
     in tablet space at full tablet resolution. */
  kCGTabletEventPointZ = 17,

  /* Key to access an integer field that contains the tablet button state.
     Bit 0 is the first button, and a set bit represents a closed or pressed
     button. Up to 16 buttons are supported. */
  kCGTabletEventPointButtons = 18,

  /* Key to access a double field that contains the tablet pen pressure. A
     value of 0.0 represents no pressure, and 1.0 represents maximum
     pressure. */
  kCGTabletEventPointPressure = 19,

  /* Key to access a double field that contains the horizontal tablet pen
     tilt. A value of 0 represents no tilt, and 1 represents maximum tilt. */
  kCGTabletEventTiltX = 20,

  /* Key to access a double field that contains the vertical tablet pen
     tilt. A value of 0 represents no tilt, and 1 represents maximum tilt. */
  kCGTabletEventTiltY = 21,

  /* Key to access a double field that contains the tablet pen rotation. */
  kCGTabletEventRotation = 22,

  /* Key to access a double field that contains the tangential pressure on
     the device. A value of 0.0 represents no pressure, and 1.0 represents
     maximum pressure. */
  kCGTabletEventTangentialPressure = 23,

  /* Key to access an integer field that contains the system-assigned unique
     device ID. */
  kCGTabletEventDeviceID = 24,

  /* Key to access an integer field that contains a vendor-specified value. */
  kCGTabletEventVendor1 = 25,

  /* Key to access an integer field that contains a vendor-specified value. */
  kCGTabletEventVendor2 = 26,

  /* Key to access an integer field that contains a vendor-specified value. */
  kCGTabletEventVendor3 = 27,

  /* Key to access an integer field that contains the vendor-defined ID,
     typically the USB vendor ID. */
  kCGTabletProximityEventVendorID = 28,

  /* Key to access an integer field that contains the vendor-defined tablet
     ID, typically the USB product ID. */
  kCGTabletProximityEventTabletID = 29,

  /* Key to access an integer field that contains the vendor-defined ID of
     the pointing device. */
  kCGTabletProximityEventPointerID = 30,

  /* Key to access an integer field that contains the system-assigned device
     ID. */
  kCGTabletProximityEventDeviceID = 31,

  /* Key to access an integer field that contains the system-assigned unique
     tablet ID. */
  kCGTabletProximityEventSystemTabletID = 32,

  /* Key to access an integer field that contains the vendor-assigned
     pointer type. */
  kCGTabletProximityEventVendorPointerType = 33,

  /* Key to access an integer field that contains the vendor-defined pointer
     serial number. */
  kCGTabletProximityEventVendorPointerSerialNumber = 34,

  /* Key to access an integer field that contains the vendor-defined unique
     ID. */
  kCGTabletProximityEventVendorUniqueID = 35,

  /* Key to access an integer field that contains the device capabilities
     mask. */
  kCGTabletProximityEventCapabilityMask = 36,

  /* Key to access an integer field that contains the pointer type. */
  kCGTabletProximityEventPointerType = 37,

  /* Key to access an integer field that indicates whether the pen is in
     proximity to the tablet. The value is non-zero if the pen is in
     proximity to the tablet and zero when leaving the tablet. */
  kCGTabletProximityEventEnterProximity = 38,

  /* Key to access a field that contains the event target process serial
     number. The value is a 64-bit value. */
  kCGEventTargetProcessSerialNumber = 39,

  /* Key to access a field that contains the event target Unix process ID. */
  kCGEventTargetUnixProcessID = 40,

  /* Key to access a field that contains the event source Unix process ID. */
  kCGEventSourceUnixProcessID = 41,

  /* Key to access a field that contains the event source user-supplied
     data, up to 64 bits. */
  kCGEventSourceUserData = 42,

  /* Key to access a field that contains the event source Unix effective
     UID. */
  kCGEventSourceUserID = 43,

  /* Key to access a field that contains the event source Unix effective
     GID. */
  kCGEventSourceGroupID = 44,

  /* Key to access a field that contains the event source state ID used to
     create this event. */
  kCGEventSourceStateID = 45,
    
  /* Key to access an integer field that indicates whether a scrolling event
     contains continuous, pixel-based scrolling data. The value is non-zero
     when the scrolling data is pixel-based and zero when the scrolling data
     is line-based. */
  kCGScrollWheelEventIsContinuous = 88,
  
  /* Added in 10.5; made public in 10.7 */
  kCGMouseEventWindowUnderMousePointer = 91,
  kCGMouseEventWindowUnderMousePointerThatCanHandleThisEvent = 92,

  /* Unaccelerated pointer movement */
  kCGEventUnacceleratedPointerMovementX = 170,
  kCGEventUnacceleratedPointerMovementY = 171
};

/* Constants used with the `kCGMouseEventSubtype' event field. */
typedef CF_ENUM(uint32_t, CGEventMouseSubtype) {
  kCGEventMouseSubtypeDefault           = 0,
  kCGEventMouseSubtypeTabletPoint       = 1,
  kCGEventMouseSubtypeTabletProximity   = 2
};

/* Constants that specify possible tapping points for events. */
typedef CF_ENUM(uint32_t, CGEventTapLocation) {
  kCGHIDEventTap = 0,
  kCGSessionEventTap,
  kCGAnnotatedSessionEventTap
};

/* Constants that specify where a new event tap is inserted into the list of
   active event taps. */
typedef CF_ENUM(uint32_t, CGEventTapPlacement) {
  kCGHeadInsertEventTap = 0,
  kCGTailAppendEventTap
};

/* Constants that specify whether a new event tap is an active filter or a
   passive listener. */
typedef CF_ENUM(uint32_t, CGEventTapOptions) {
  kCGEventTapOptionDefault = 0x00000000,
  kCGEventTapOptionListenOnly = 0x00000001
};

/* A mask that identifies the set of Quartz events to be observed in an
   event tap. */
typedef uint64_t CGEventMask;

/* Generate an event mask for a single type of event. */
#define CGEventMaskBit(eventType)       ((CGEventMask)1 << (eventType))

/* An event mask that represents all event types. */
#define kCGEventMaskForAllEvents        (~(CGEventMask)0)

/* An opaque type that represents state within the client application that’s
   associated with an event tap. */
typedef struct __CGEventTapProxy *CGEventTapProxy;

/* A client-supplied callback function that’s invoked whenever an associated
   event tap receives a Quartz event.

   The callback is passed a proxy for the tap, the event type, the incoming
   event, and the user-defined data specified when the event tap was
   created. The function should return the (possibly modified) passed-in
   event, a newly constructed event, or NULL if the event is to be deleted.

   The event passed to the callback is retained by the calling code, and is
   released after the callback returns and the data is passed back to the
   event system. If a different event is returned by the callback function,
   then that event will be released by the calling code along with the
   original event, after the event data has been passed back to the event
   system. */

typedef CGEventRef __nullable (*CGEventTapCallBack)(CGEventTapProxy  proxy,
  CGEventType type, CGEventRef  event, void * __nullable userInfo);

/* When an event tap is installed or released, a notification is posted. See
   notify(3) and notify.h for details. */

#define kCGNotifyEventTapAdded   "com.apple.coregraphics.eventTapAdded"
#define kCGNotifyEventTapRemoved "com.apple.coregraphics.eventTapRemoved"

/* The structure used to report information about event taps. */

struct __CGEventTapInformation {
  uint32_t eventTapID;
  CGEventTapLocation tapPoint;          /* HID, session, annotated session */
  CGEventTapOptions options;            /* Listener, filter */
  CGEventMask eventsOfInterest;         /* Mask of events being tapped */
  pid_t tappingProcess;                 /* Process that is tapping events */
  pid_t processBeingTapped;             /* Zero if not a per-process tap */
  bool enabled;                         /* True if tap is enabled */
  float minUsecLatency;                 /* Minimum latency in microseconds */
  float avgUsecLatency;                 /* Average latency in microseconds */
  float maxUsecLatency;                 /* Maximum latency in microseconds */
};
typedef struct __CGEventTapInformation CGEventTapInformation;

/* An opaque type that represents the source of a Quartz event. */
typedef struct CF_BRIDGED_TYPE(id) __CGEventSource *CGEventSourceRef;

/* Constants that specify the possible source states of an event source. */
typedef CF_ENUM(int32_t, CGEventSourceStateID) {
  kCGEventSourceStatePrivate = -1,
  kCGEventSourceStateCombinedSessionState = 0,
  kCGEventSourceStateHIDSystemState = 1
};

/* A code that represents the type of keyboard used with a specified event
   source. */
typedef uint32_t CGEventSourceKeyboardType;

/* A constant specifying any input event type */
#define kCGAnyInputEventType ((CGEventType)(~0))

CF_ASSUME_NONNULL_END


#endif /* CGEVENTTYPES_H_ */
