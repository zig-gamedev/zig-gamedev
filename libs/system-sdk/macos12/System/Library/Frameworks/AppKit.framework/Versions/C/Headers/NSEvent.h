/*
    NSEvent.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSTouch.h>
#import <AppKit/AppKitDefines.h>
#import <ApplicationServices/ApplicationServices.h>
#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSSet.h>
#import <IOKit/hidsystem/IOLLEvent.h>
#import <limits.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSGraphicsContext, NSWindow, NSTrackingArea;

typedef NS_ENUM(NSUInteger, NSEventType) {        /* various types of events */
    NSEventTypeLeftMouseDown             = 1,
    NSEventTypeLeftMouseUp               = 2,
    NSEventTypeRightMouseDown            = 3,
    NSEventTypeRightMouseUp              = 4,
    NSEventTypeMouseMoved                = 5,
    NSEventTypeLeftMouseDragged          = 6,
    NSEventTypeRightMouseDragged         = 7,
    NSEventTypeMouseEntered              = 8,
    NSEventTypeMouseExited               = 9,
    NSEventTypeKeyDown                   = 10,
    NSEventTypeKeyUp                     = 11,
    NSEventTypeFlagsChanged              = 12,
    NSEventTypeAppKitDefined             = 13,
    NSEventTypeSystemDefined             = 14,
    NSEventTypeApplicationDefined        = 15,
    NSEventTypePeriodic                  = 16,
    NSEventTypeCursorUpdate              = 17,
    NSEventTypeScrollWheel               = 22,
    NSEventTypeTabletPoint               = 23,
    NSEventTypeTabletProximity           = 24,
    NSEventTypeOtherMouseDown            = 25,
    NSEventTypeOtherMouseUp              = 26,
    NSEventTypeOtherMouseDragged         = 27,
    /* The following event types are available on some hardware on 10.5.2 and later */
    NSEventTypeGesture API_AVAILABLE(macos(10.5))       = 29,
    NSEventTypeMagnify API_AVAILABLE(macos(10.5))       = 30,
    NSEventTypeSwipe   API_AVAILABLE(macos(10.5))       = 31,
    NSEventTypeRotate  API_AVAILABLE(macos(10.5))       = 18,
    NSEventTypeBeginGesture API_AVAILABLE(macos(10.5))  = 19,
    NSEventTypeEndGesture API_AVAILABLE(macos(10.5))    = 20,
    
    NSEventTypeSmartMagnify API_AVAILABLE(macos(10.8)) = 32,
    NSEventTypeQuickLook API_AVAILABLE(macos(10.8)) = 33,
    
    NSEventTypePressure API_AVAILABLE(macos(10.10.3)) = 34,
    NSEventTypeDirectTouch API_AVAILABLE(macos(10.10)) = 37,

    NSEventTypeChangeMode API_AVAILABLE(macos(10.15)) = 38,
};

static const NSEventType NSLeftMouseDown 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeLeftMouseDown", macos(10.0,10.12)) = NSEventTypeLeftMouseDown;
static const NSEventType NSLeftMouseUp 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeLeftMouseUp", macos(10.0,10.12)) = NSEventTypeLeftMouseUp;
static const NSEventType NSRightMouseDown 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeRightMouseDown", macos(10.0,10.12)) = NSEventTypeRightMouseDown;
static const NSEventType NSRightMouseUp		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeRightMouseUp", macos(10.0,10.12)) = NSEventTypeRightMouseUp;
static const NSEventType NSMouseMoved 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeMouseMoved", macos(10.0,10.12)) = NSEventTypeMouseMoved;
static const NSEventType NSLeftMouseDragged	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeLeftMouseDragged", macos(10.0,10.12)) = NSEventTypeLeftMouseDragged;
static const NSEventType NSRightMouseDragged 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeRightMouseDragged", macos(10.0,10.12)) = NSEventTypeRightMouseDragged;
static const NSEventType NSMouseEntered 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeMouseEntered", macos(10.0,10.12)) = NSEventTypeMouseEntered;
static const NSEventType NSMouseExited 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeMouseExited", macos(10.0,10.12)) = NSEventTypeMouseExited;
static const NSEventType NSKeyDown 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeKeyDown", macos(10.0,10.12)) = NSEventTypeKeyDown;
static const NSEventType NSKeyUp 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeKeyUp", macos(10.0,10.12)) = NSEventTypeKeyUp;
static const NSEventType NSFlagsChanged 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeFlagsChanged", macos(10.0,10.12)) = NSEventTypeFlagsChanged;
static const NSEventType NSAppKitDefined 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeAppKitDefined", macos(10.0,10.12)) = NSEventTypeAppKitDefined;
static const NSEventType NSSystemDefined 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeSystemDefined", macos(10.0,10.12)) = NSEventTypeSystemDefined;
static const NSEventType NSApplicationDefined	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeApplicationDefined", macos(10.0,10.12)) = NSEventTypeApplicationDefined;
static const NSEventType NSPeriodic 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypePeriodic", macos(10.0,10.12)) = NSEventTypePeriodic;
static const NSEventType NSCursorUpdate 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeCursorUpdate", macos(10.0,10.12)) = NSEventTypeCursorUpdate;
static const NSEventType NSScrollWheel 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeScrollWheel", macos(10.0,10.12)) = NSEventTypeScrollWheel;
static const NSEventType NSTabletPoint 		API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeTabletPoint", macos(10.0,10.12)) = NSEventTypeTabletPoint;
static const NSEventType NSTabletProximity 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeTabletProximity", macos(10.0,10.12)) = NSEventTypeTabletProximity;
static const NSEventType NSOtherMouseDown	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeOtherMouseDown", macos(10.0,10.12)) = NSEventTypeOtherMouseDown;
static const NSEventType NSOtherMouseUp 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeOtherMouseUp", macos(10.0,10.12)) = NSEventTypeOtherMouseUp;
static const NSEventType NSOtherMouseDragged 	API_DEPRECATED_WITH_REPLACEMENT("NSEventTypeOtherMouseDragged", macos(10.0,10.12)) = NSEventTypeOtherMouseDragged;


// For APIs introduced in Mac OS X 10.6 and later, this type is used with NS*Mask constants to indicate the events of interest.
typedef NS_OPTIONS(unsigned long long, NSEventMask) { /* masks for the types of events */
    NSEventMaskLeftMouseDown         = 1ULL << NSEventTypeLeftMouseDown,
    NSEventMaskLeftMouseUp           = 1ULL << NSEventTypeLeftMouseUp,
    NSEventMaskRightMouseDown        = 1ULL << NSEventTypeRightMouseDown,
    NSEventMaskRightMouseUp          = 1ULL << NSEventTypeRightMouseUp,
    NSEventMaskMouseMoved            = 1ULL << NSEventTypeMouseMoved,
    NSEventMaskLeftMouseDragged      = 1ULL << NSEventTypeLeftMouseDragged,
    NSEventMaskRightMouseDragged     = 1ULL << NSEventTypeRightMouseDragged,
    NSEventMaskMouseEntered          = 1ULL << NSEventTypeMouseEntered,
    NSEventMaskMouseExited           = 1ULL << NSEventTypeMouseExited,
    NSEventMaskKeyDown               = 1ULL << NSEventTypeKeyDown,
    NSEventMaskKeyUp                 = 1ULL << NSEventTypeKeyUp,
    NSEventMaskFlagsChanged          = 1ULL << NSEventTypeFlagsChanged,
    NSEventMaskAppKitDefined         = 1ULL << NSEventTypeAppKitDefined,
    NSEventMaskSystemDefined         = 1ULL << NSEventTypeSystemDefined,
    NSEventMaskApplicationDefined    = 1ULL << NSEventTypeApplicationDefined,
    NSEventMaskPeriodic              = 1ULL << NSEventTypePeriodic,
    NSEventMaskCursorUpdate          = 1ULL << NSEventTypeCursorUpdate,
    NSEventMaskScrollWheel           = 1ULL << NSEventTypeScrollWheel,
    NSEventMaskTabletPoint           = 1ULL << NSEventTypeTabletPoint,
    NSEventMaskTabletProximity       = 1ULL << NSEventTypeTabletProximity,
    NSEventMaskOtherMouseDown        = 1ULL << NSEventTypeOtherMouseDown,
    NSEventMaskOtherMouseUp          = 1ULL << NSEventTypeOtherMouseUp,
    NSEventMaskOtherMouseDragged     = 1ULL << NSEventTypeOtherMouseDragged,
    /* The following event masks are available on some hardware on 10.5.2 and later */
    NSEventMaskGesture API_AVAILABLE(macos(10.5))          = 1ULL << NSEventTypeGesture,
    NSEventMaskMagnify API_AVAILABLE(macos(10.5))          = 1ULL << NSEventTypeMagnify,
    NSEventMaskSwipe API_AVAILABLE(macos(10.5))            = 1ULL << NSEventTypeSwipe,
    NSEventMaskRotate API_AVAILABLE(macos(10.5))           = 1ULL << NSEventTypeRotate,
    NSEventMaskBeginGesture API_AVAILABLE(macos(10.5))     = 1ULL << NSEventTypeBeginGesture,
    NSEventMaskEndGesture API_AVAILABLE(macos(10.5))       = 1ULL << NSEventTypeEndGesture,
    
    /* Note: You can only use these event masks on 64 bit. In other words, you cannot setup a local, nor global, event monitor for these event types on 32 bit. Also, you cannot search the event queue for them (nextEventMatchingMask:...) on 32 bit.
     */
    NSEventMaskSmartMagnify API_AVAILABLE(macos(10.8)) = 1ULL << NSEventTypeSmartMagnify,
    NSEventMaskPressure API_AVAILABLE(macos(10.10.3)) = 1ULL << NSEventTypePressure,
    NSEventMaskDirectTouch API_AVAILABLE(macos(10.12.2)) = 1ULL << NSEventTypeDirectTouch,

    NSEventMaskChangeMode API_AVAILABLE(macos(10.15)) = 1ULL << NSEventTypeChangeMode,
    
    NSEventMaskAny              = NSUIntegerMax,
    
};

static const NSEventMask NSLeftMouseDownMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskLeftMouseDown", macos(10.0,10.12)) = NSEventMaskLeftMouseDown;
static const NSEventMask NSLeftMouseUpMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskLeftMouseUp", macos(10.0,10.12)) = NSEventMaskLeftMouseUp;
static const NSEventMask NSRightMouseDownMask	 	API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskRightMouseDown", macos(10.0,10.12)) = NSEventMaskRightMouseDown;
static const NSEventMask NSRightMouseUpMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskRightMouseUp", macos(10.0,10.12)) = NSEventMaskRightMouseUp;
static const NSEventMask NSMouseMovedMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskMouseMoved", macos(10.0,10.12)) = NSEventMaskMouseMoved;
static const NSEventMask NSLeftMouseDraggedMask 	API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskLeftMouseDragged", macos(10.0,10.12)) = NSEventMaskLeftMouseDragged;
static const NSEventMask NSRightMouseDraggedMask 	API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskRightMouseDragged", macos(10.0,10.12)) = NSEventMaskRightMouseDragged;
static const NSEventMask NSMouseEnteredMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskMouseEntered", macos(10.0,10.12)) = NSEventMaskMouseEntered;
static const NSEventMask NSMouseExitedMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskMouseExited", macos(10.0,10.12)) = NSEventMaskMouseExited;
static const NSEventMask NSKeyDownMask 			API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskKeyDown", macos(10.0,10.12)) = NSEventMaskKeyDown;
static const NSEventMask NSKeyUpMask 			API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskKeyUp", macos(10.0,10.12)) = NSEventMaskKeyUp;
static const NSEventMask NSFlagsChangedMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskFlagsChanged", macos(10.0,10.12)) = NSEventMaskFlagsChanged;
static const NSEventMask NSAppKitDefinedMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskAppKitDefined", macos(10.0,10.12)) = NSEventMaskAppKitDefined;
static const NSEventMask NSSystemDefinedMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskSystemDefined", macos(10.0,10.12)) = NSEventMaskSystemDefined;
static const NSEventMask NSApplicationDefinedMask 	API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskApplicationDefined", macos(10.0,10.12)) = NSEventMaskApplicationDefined;
static const NSEventMask NSPeriodicMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskPeriodic", macos(10.0,10.12)) = NSEventMaskPeriodic;
static const NSEventMask NSCursorUpdateMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskCursorUpdate", macos(10.0,10.12)) = NSEventMaskCursorUpdate;
static const NSEventMask NSScrollWheelMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskScrollWheel", macos(10.0,10.12)) = NSEventMaskScrollWheel;
static const NSEventMask NSTabletPointMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskTabletPoint", macos(10.0,10.12)) = NSEventMaskTabletPoint;
static const NSEventMask NSTabletProximityMask	 	API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskTabletProximity", macos(10.0,10.12)) = NSEventMaskTabletProximity;
static const NSEventMask NSOtherMouseDownMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskOtherMouseDown", macos(10.0,10.12)) = NSEventMaskOtherMouseDown;
static const NSEventMask NSOtherMouseUpMask 		API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskOtherMouseUp", macos(10.0,10.12)) = NSEventMaskOtherMouseUp;
static const NSEventMask NSOtherMouseDraggedMask 	API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskOtherMouseDragged", macos(10.0,10.12)) = NSEventMaskOtherMouseDragged;
static const NSEventMask NSAnyEventMask			API_DEPRECATED_WITH_REPLACEMENT("NSEventMaskAny", macos(10.0,10.12)) = NSUIntegerMax;

NS_INLINE NSEventMask NSEventMaskFromType(NSEventType type) { return (1UL << type); }

/* Device-independent bits found in event modifier flags */
typedef NS_OPTIONS(NSUInteger, NSEventModifierFlags) {
    NSEventModifierFlagCapsLock           = 1 << 16, // Set if Caps Lock key is pressed.
    NSEventModifierFlagShift              = 1 << 17, // Set if Shift key is pressed.
    NSEventModifierFlagControl            = 1 << 18, // Set if Control key is pressed.
    NSEventModifierFlagOption             = 1 << 19, // Set if Option or Alternate key is pressed.
    NSEventModifierFlagCommand            = 1 << 20, // Set if Command key is pressed.
    NSEventModifierFlagNumericPad         = 1 << 21, // Set if any key in the numeric keypad is pressed.
    NSEventModifierFlagHelp               = 1 << 22, // Set if the Help key is pressed.
    NSEventModifierFlagFunction           = 1 << 23, // Set if any function key is pressed.
    
    // Used to retrieve only the device-independent modifier flags, allowing applications to mask off the device-dependent modifier flags, including event coalescing information.
    NSEventModifierFlagDeviceIndependentFlagsMask    = 0xffff0000UL
};

static const NSEventModifierFlags NSAlphaShiftKeyMask         API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagCapsLock", macos(10.0,10.12)) = NSEventModifierFlagCapsLock;
static const NSEventModifierFlags NSShiftKeyMask              API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagShift", macos(10.0,10.12)) = NSEventModifierFlagShift;
static const NSEventModifierFlags NSControlKeyMask            API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagControl", macos(10.0,10.12)) = NSEventModifierFlagControl;
static const NSEventModifierFlags NSAlternateKeyMask          API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagOption", macos(10.0,10.12)) = NSEventModifierFlagOption;
static const NSEventModifierFlags NSCommandKeyMask            API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagCommand", macos(10.0,10.12)) = NSEventModifierFlagCommand;
static const NSEventModifierFlags NSNumericPadKeyMask         API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagNumericPad", macos(10.0,10.12)) = NSEventModifierFlagNumericPad;
static const NSEventModifierFlags NSHelpKeyMask               API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagHelp", macos(10.0,10.12)) = NSEventModifierFlagHelp;
static const NSEventModifierFlags NSFunctionKeyMask           API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagFunction", macos(10.0,10.12)) = NSEventModifierFlagFunction;
static const NSEventModifierFlags NSDeviceIndependentModifierFlagsMask    API_DEPRECATED_WITH_REPLACEMENT("NSEventModifierFlagDeviceIndependentFlagsMask", macos(10.0,10.12)) = NSEventModifierFlagDeviceIndependentFlagsMask;

/* pointer types for NSEventTypeTabletProximity events or mouse events with subtype NSEventSubtypeTabletProximity*/
typedef NS_ENUM(NSUInteger, NSPointingDeviceType) {        
    NSPointingDeviceTypeUnknown     = NX_TABLET_POINTER_UNKNOWN,
    NSPointingDeviceTypePen         = NX_TABLET_POINTER_PEN,
    NSPointingDeviceTypeCursor      = NX_TABLET_POINTER_CURSOR,
    NSPointingDeviceTypeEraser      = NX_TABLET_POINTER_ERASER
};

static const NSPointingDeviceType NSUnknownPointingDevice     API_DEPRECATED_WITH_REPLACEMENT("NSPointingDeviceTypeUnknown", macos(10.0,10.12)) = NSPointingDeviceTypeUnknown;
static const NSPointingDeviceType NSPenPointingDevice         API_DEPRECATED_WITH_REPLACEMENT("NSPointingDeviceTypePen", macos(10.0,10.12)) = NSPointingDeviceTypePen;
static const NSPointingDeviceType NSCursorPointingDevice      API_DEPRECATED_WITH_REPLACEMENT("NSPointingDeviceTypeCursor", macos(10.0,10.12)) = NSPointingDeviceTypeCursor;
static const NSPointingDeviceType NSEraserPointingDevice      API_DEPRECATED_WITH_REPLACEMENT("NSPointingDeviceTypeEraser", macos(10.0,10.12)) = NSPointingDeviceTypeEraser;

/* button masks for NSEventTypeTabletPoint events or mouse events with subtype NSEventSubtypeTabletPoint */
typedef NS_OPTIONS(NSUInteger, NSEventButtonMask) {
    NSEventButtonMaskPenTip                = NX_TABLET_BUTTON_PENTIPMASK,
    NSEventButtonMaskPenLowerSide          = NX_TABLET_BUTTON_PENLOWERSIDEMASK,
    NSEventButtonMaskPenUpperSide          = NX_TABLET_BUTTON_PENUPPERSIDEMASK
};

static const NSEventButtonMask NSPenTipMask                API_DEPRECATED_WITH_REPLACEMENT("NSEventButtonMaskPenTip", macos(10.0,10.12)) = NSEventButtonMaskPenTip;
static const NSEventButtonMask NSPenLowerSideMask          API_DEPRECATED_WITH_REPLACEMENT("NSEventButtonMaskPenLowerSide", macos(10.0,10.12)) = NSEventButtonMaskPenLowerSide;
static const NSEventButtonMask NSPenUpperSideMask          API_DEPRECATED_WITH_REPLACEMENT("NSEventButtonMaskPenUpperSide", macos(10.0,10.12)) = NSEventButtonMaskPenUpperSide;

typedef NS_OPTIONS(NSUInteger, NSEventPhase) {
    NSEventPhaseNone        = 0, // event not associated with a phase.
    NSEventPhaseBegan       = 0x1 << 0,
    NSEventPhaseStationary  = 0x1 << 1,
    NSEventPhaseChanged     = 0x1 << 2,
    NSEventPhaseEnded       = 0x1 << 3,
    NSEventPhaseCancelled   = 0x1 << 4,
    NSEventPhaseMayBegin    = 0x1 << 5,
} API_AVAILABLE(macos(10.7));

typedef NS_ENUM(NSInteger, NSEventGestureAxis) {
    NSEventGestureAxisNone = 0,
    NSEventGestureAxisHorizontal,
    NSEventGestureAxisVertical
} API_AVAILABLE(macos(10.7));

typedef NS_OPTIONS(NSUInteger, NSEventSwipeTrackingOptions) {
    NSEventSwipeTrackingLockDirection = 0x1 << 0, // Clamp gestureAmount to 0 if the user starts to swipe in the opposite direction than they started.
    NSEventSwipeTrackingClampGestureAmount = 0x1 << 1  // Don't allow gestureAmount to go beyond +/-1.0
} API_AVAILABLE(macos(10.7));

typedef NS_ENUM(short, NSEventSubtype) {
    /* event subtypes for NSEventTypeAppKitDefined events */
    NSEventSubtypeWindowExposed            = 0,
    NSEventSubtypeApplicationActivated     = 1,
    NSEventSubtypeApplicationDeactivated   = 2,
    NSEventSubtypeWindowMoved              = 4,
    NSEventSubtypeScreenChanged            = 8,
    
    /* event subtypes for NSEventTypeSystemDefined events */
    NSEventSubtypePowerOff             = 1,
    
    /* event subtypes for mouse events */
    NSEventSubtypeMouseEvent        = NX_SUBTYPE_DEFAULT,
    NSEventSubtypeTabletPoint       = NX_SUBTYPE_TABLET_POINT,
    NSEventSubtypeTabletProximity   = NX_SUBTYPE_TABLET_PROXIMITY,
    NSEventSubtypeTouch  API_AVAILABLE(macos(10.6))             = NX_SUBTYPE_MOUSE_TOUCH
};

static const NSEventSubtype NSWindowExposedEventType            API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeWindowExposed", macos(10.0,10.12)) = NSEventSubtypeWindowExposed;
static const NSEventSubtype NSApplicationActivatedEventType     API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeApplicationActivated", macos(10.0,10.12)) = NSEventSubtypeApplicationActivated;
static const NSEventSubtype NSApplicationDeactivatedEventType   API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeApplicationDeactivated", macos(10.0,10.12)) = NSEventSubtypeApplicationDeactivated;
static const NSEventSubtype NSWindowMovedEventType              API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeWindowMoved", macos(10.0,10.12)) = NSEventSubtypeWindowMoved;
static const NSEventSubtype NSScreenChangedEventType            API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeScreenChanged", macos(10.0,10.12)) = NSEventSubtypeScreenChanged;
static const NSEventSubtype NSAWTEventType                      API_DEPRECATED("This subtype no longer exists", macos(10.10,10.12)) = (NSEventSubtype)16;
static const NSEventSubtype NSPowerOffEventType             API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypePowerOff", macos(10.0,10.12)) = NSEventSubtypePowerOff;
static const NSEventSubtype NSMouseEventSubtype             API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeMouseEvent", macos(10.0,10.12)) = NSEventSubtypeMouseEvent;
static const NSEventSubtype NSTabletPointEventSubtype       API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeTabletPoint", macos(10.0,10.12)) = NSEventSubtypeTabletPoint;
static const NSEventSubtype NSTabletProximityEventSubtype   API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeTabletProximity", macos(10.0,10.12)) = NSEventSubtypeTabletProximity;
static const NSEventSubtype NSTouchEventSubtype             API_DEPRECATED_WITH_REPLACEMENT("NSEventSubtypeTouch", macos(10.0,10.12)) = NSEventSubtypeTouch;


// NSPressureBehavior - The pressure gesture behavior that describes how a pressure gesture behaves and progresses
// In general, pressure gestures begin when stage reaches 1 and end when stage reaches 0. This corresponds to the simultaneously generated mouse down/up events.
typedef NS_ENUM(NSInteger, NSPressureBehavior) {
    NSPressureBehaviorUnknown = -1,
    
    // The default primary behavior when otherwise not explicitly configured. In 10.10.3 the default is NSPressureBehaviorPrimaryDeepClick.
    NSPressureBehaviorPrimaryDefault = 0,
    
    // Variable pressure with a small dynamic range after mouseDown.
    // Mouse events: LeftMouse
    // # of Stages: 1
    // Allowed stage transitions: 1->0
    // Actuations: mouseDown, mouseUp
    // stageTransition: NO
    NSPressureBehaviorPrimaryClick = 1,
    
    // Variable pressure with a large dynamic range after mouseDown suitable for drawing and general pressure use.
    // Mouse events: LeftMouse
    // # of Stages: 1
    // Allowed stage transitions: 1->0
    // Actuations: mouseDown, mouseUp
    // stageTransition: NO
    NSPressureBehaviorPrimaryGeneric = 2,
    
    // Variable pressure after mouseDown suitable for controlling speed. Used by NSAcceleratorButton
    // Mouse events: LeftMouse
    // # of Stages: 1
    // Allowed stage transitions: 1->0
    // Actuations: mouseDown, mouseUp
    // stageTransition: NO
    NSPressureBehaviorPrimaryAccelerator = 3,
    
    // Toggle between stages 1 and 2 until drag or mouse up (e.g. Force Click)
    // Mouse events: LeftMouse
    // # of Stages: 2 (stage 2 is disabled once dragging starts)
    // Allowed stage transitions: 1->0, 1->2, 2->1, 2->0
    // Actuations: mouseDown, mouseUp, 1->2, 2->1
    // stageTransition: into and release from stage 2
    NSPressureBehaviorPrimaryDeepClick = 5,
    
    // Toggle between stages 1 and 2 until mouse up (e.g. Force Click)
    // Mouse events: LeftMouse
    // # of Stages: 2
    // Allowed stage transitions: 1->0, 1->2, 2->1, 2->0
    // Actuations: mouseDown, mouseUp, 1->2, 2->1
    // stageTransition: into and release from stage 2
    NSPressureBehaviorPrimaryDeepDrag = 6,
    
}API_AVAILABLE(macos(10.10.3));

@interface NSEvent : NSObject <NSCopying, NSCoding>


/* these messages are valid for all events */
@property (readonly) NSEventType type;
@property (readonly) NSEventModifierFlags modifierFlags;
@property (readonly) NSTimeInterval timestamp;
@property (readonly, nullable, weak) NSWindow *window;
@property (readonly) NSInteger windowNumber;
@property (nullable, readonly, strong) NSGraphicsContext *context API_DEPRECATED("This method always returns nil. If you need access to the current drawing context, use [NSGraphicsContext currentContext] inside of a draw operation.", macos(10.0,10.12));

/* these messages are valid for all mouse down/up/drag events */
@property (readonly) NSInteger clickCount;
@property (readonly) NSInteger buttonNumber;    // for NSOtherMouse events, but will return valid constants for NSLeftMouse and NSRightMouse
/* these messages are valid for all mouse down/up/drag and enter/exit events */
@property (readonly) NSInteger eventNumber;

/* -pressure is valid for all mouse down/up/drag events, and is also valid for NSEventTypeTabletPoint events on 10.4 or later and NSEventTypePressure on 10.10.3 or later */
@property (readonly) float pressure;
/* -locationInWindow is valid for all mouse-related events */
@property (readonly) NSPoint locationInWindow;

/* these messages are valid for scroll wheel events and mouse move/drag events.  As of 10.5.2, deltaX and deltaY are also valid for swipe events.  A non-0 deltaX will represent a horizontal swipe, -1 for swipe right and 1 for swipe left.  A non-0 deltaY will represent a vertical swipe, -1 for swipe down and 1 for swipe up. As of 10.7, the preferred methods for scroll wheel events are scrollingDeltaX and scrollingDeltaY defined below.
*/
@property (readonly) CGFloat deltaX;    
@property (readonly) CGFloat deltaY;    
@property (readonly) CGFloat deltaZ;    // 0 for most scroll wheel and mouse events

/* This message is valid for NSEventTypeScrollWheel events. A generic scroll wheel issues rather coarse scroll deltas. Some Apple mice and trackpads provide much more precise delta. This method determines the resolution of the scrollDeltaX and scrollDeltaY values.
*/
@property (readonly) BOOL hasPreciseScrollingDeltas API_AVAILABLE(macos(10.7));

/* The following two message are the preferred API for accessing NSEventTypeScrollWheel deltas. When -hasPreciseScrollingDeltas reutrns NO, multiply the returned value by line or row height. When -hasPreciseScrollingDeltas returns YES, scroll by the returned value (in points).
*/
@property (readonly) CGFloat scrollingDeltaX API_AVAILABLE(macos(10.7));
@property (readonly) CGFloat scrollingDeltaY API_AVAILABLE(macos(10.7));

/* This message is valid for NSEventTypeScrollWheel events. With the Magic Mouse and some trackpads, the user can flick thier finger resulting in a stream of scroll events that dissipate over time. The location of these scroll wheel events changes as the user moves the cursor. AppKit latches these scroll wheel events to the view that is under the cursor when the flick occurs. A custom view can use this method to recognize these momentum scroll events and further route the event to the appropriate sub component.
*/
@property (readonly) NSEventPhase momentumPhase API_AVAILABLE(macos(10.7));

/* valid for NSEventScrollWheel events. The user may choose to change the scrolling behavior such that it feels like they are moving the content instead of the scroll bar. To accomplish this, deltaX/Y and scrollingDeltaX/Y are automatically inverted for NSEventScrollWheel events according to the user's preferences. However, for some uses, the behavior should not respect the user preference. This method allows you to determine when the event has been inverted and compensate by multiplying -1 if needed. 
*/
@property (getter=isDirectionInvertedFromDevice, readonly) BOOL directionInvertedFromDevice API_AVAILABLE(macos(10.7));

/* these messages are valid for keyup and keydown events */
@property (nullable, readonly, copy) NSString *characters;
@property (nullable, readonly, copy) NSString *charactersIgnoringModifiers;

/* This message is valid keyup and keydown events.
    It returns the character(s) that would have been generated if a different modifier combination had been used.
    It uses [self keyCode], the new modifiers and the current keyboard input source's layout data for re-translation.  It entirely ignores the modifiers that are already present in the event and calling this method will not affect the dead key state for current text input.
    If there is invalid data in this event, -charactersByApplyingModifiers will return nil.
*/
- (nullable NSString *)charactersByApplyingModifiers:(NSEventModifierFlags)modifiers API_AVAILABLE(macos(10.15)); 

  /* the chars that would have been generated, regardless of modifier keys (except shift) */
@property (getter=isARepeat, readonly) BOOL ARepeat;
/* this message is valid for keyup, keydown and flagschanged events */
@property (readonly) unsigned short keyCode;        /* device-independent key number */

/* these messages are valid for enter and exit events */
@property (readonly) NSInteger trackingNumber;
@property (nullable, readonly) void *userData NS_RETURNS_INNER_POINTER;
/* -trackingArea returns the NSTrackingArea that generated this event.  It is possible for there to be no trackingArea associated with the event in some cases where the event corresponds to a trackingRect installed with -[NSView addTrackingRect:owner:userData:assumeInside:], in which case nil is returned. */
@property (nullable, readonly, strong) NSTrackingArea *trackingArea API_AVAILABLE(macos(10.5)); 

/* this message is valid for kit, system, and app-defined events */
/* this message is also valid for mouse events on 10.4 or later */
@property (readonly) NSEventSubtype subtype;

/* these messages are valid for kit, system, and app-defined events */
@property (readonly) NSInteger data1;
@property (readonly) NSInteger data2;


/* -eventRef and +eventWithEventRef:  are valid for all events */
/* -eventRef returns an EventRef corresponding to the NSEvent.  The EventRef is retained by the NSEvent, so will be valid as long as the NSEvent is valid, and will be released when the NSEvent is freed.  You can use RetainEvent to extend the lifetime of the EventRef, with a corresponding ReleaseEvent when you are done with it.  If there is no EventRef corresponding to the NSEvent, -eventRef will return NULL.
*/
@property (nullable, readonly) const void * /* EventRef */eventRef NS_RETURNS_INNER_POINTER API_AVAILABLE(macos(10.5));
/* +eventWithEventRef: returns an autoreleased NSEvent corresponding to the EventRef.  The EventRef is retained by the NSEvent and will be released when the NSEvent is freed.  If there is no NSEvent corresponding to the EventRef, +eventWithEventRef will return nil.
*/
+ (nullable NSEvent *)eventWithEventRef:(const void * /* EventRef */)eventRef API_AVAILABLE(macos(10.5));

/* -CGEvent returns an autoreleased CGEventRef corresponding to the NSEvent.  If there is no CGEventRef corresponding to the NSEvent, -CGEvent will return NULL.
*/
@property (nullable, readonly) CGEventRef CGEvent API_AVAILABLE(macos(10.5));

/* +eventWithCGEvent: returns an autoreleased NSEvent corresponding to the CGEventRef.  The CGEventRef is retained by the NSEvent and will be released when the NSEvent is freed.  If there is no NSEvent corresponding to the CGEventRef, +eventWithEventRef will return nil.
*/
+ (nullable NSEvent *)eventWithCGEvent:(CGEventRef)cgEvent API_AVAILABLE(macos(10.5));

/* Enable or disable coalescing of mouse movement events, including mouse moved, mouse dragged, and tablet events.  Coalescing is enabled by default.
*/
@property (class, getter=isMouseCoalescingEnabled) BOOL mouseCoalescingEnabled API_AVAILABLE(macos(10.5));


/* This message is valid for events of type NSEventTypeMagnify, on 10.5.2 or later */
@property (readonly) CGFloat magnification API_AVAILABLE(macos(10.5));       // change in magnification.   This value should be added to the current scaling of an item to get the new scale factor.

/* this message is valid for mouse events with subtype NSEventSubtypeTabletPoint or NSEventSubtypeTabletProximity, and for NSEventTypeTabletPoint and NSEventTypeTabletProximity events */
@property (readonly) NSUInteger deviceID;

/* this message is valid for valid for mouse events with subtype NSEventSubtypeTabletPoint, and for NSEventTypeTabletPoint events.  On 10.5.2 or later, it is also valid for NSEventTypeRotate events. */
@property (readonly) float rotation;       // In degrees.  For NSEventTypeTabletPoint, this is rotation of the pen.  For NSEventTypeRotate, it is rotation on the track pad.

/* these messages are valid for mouse events with subtype NSEventSubtypeTabletPoint, and for NSEventTypeTabletPoint events */
/* absolute x coordinate in tablet space at full tablet resolution */
@property (readonly) NSInteger absoluteX; 
/* absolute y coordinate in tablet space at full tablet resolution */
@property (readonly) NSInteger absoluteY;               
/* absolute z coordinate in tablet space at full tablet resolution */
@property (readonly) NSInteger absoluteZ;     
/* mask indicating which buttons are pressed.*/
@property (readonly) NSEventButtonMask buttonMask;
/* range is -1 to 1 for both axes */
@property (readonly) NSPoint tilt;     
/* tangential pressure on the device; range is -1 to 1 */
@property (readonly) float tangentialPressure;  
/* NSArray of 3 vendor defined shorts */
@property (readonly, strong) id vendorDefined;    

/* these messages are valid for mouse events with subtype NSEventSubtypeTabletProximity, and  for NSEventTypeTabletProximity events */
/* vendor defined, typically USB vendor ID */
@property (readonly) NSUInteger vendorID;
/* vendor defined tablet ID */
@property (readonly) NSUInteger tabletID;
/* index of the device on the tablet.  Usually 0, except for tablets that support multiple concurrent devices */
@property (readonly) NSUInteger pointingDeviceID;
/* system assigned unique tablet ID */
@property (readonly) NSUInteger systemTabletID;
/* vendor defined pointing device type */
@property (readonly) NSUInteger vendorPointingDeviceType; 
/* vendor defined serial number of pointing device */
@property (readonly) NSUInteger pointingDeviceSerialNumber; 
/* vendor defined unique ID */
@property (readonly) unsigned long long uniqueID;    
/* mask representing capabilities of device */
@property (readonly) NSUInteger capabilityMask;    
/* mask representing capabilities of device */
@property (readonly) NSPointingDeviceType pointingDeviceType;
/* YES - entering; NO - leaving */
@property (getter=isEnteringProximity, readonly) BOOL enteringProximity;    


- (NSSet<NSTouch *> *)touchesMatchingPhase:(NSTouchPhase)phase inView:(nullable NSView *)view API_AVAILABLE(macos(10.6));

/* Only valid for NSEventTypeGesture events. Equivalent to [event touchesMatchingPhase:NSTouchPhaseAny inView:nil] */
- (NSSet <NSTouch *> *)allTouches API_AVAILABLE(macos(10.12));

/* Only valid for NSEventTypeGesture events. Equivalent to [event touchesMatchingPhase:NSTouchPhaseAny inView:view] */
- (NSSet <NSTouch *> *)touchesForView:(NSView *)view API_AVAILABLE(macos(10.12));

/* An array of auxiliary NSTouch’s for the touch events that did not get delivered for a given main touch. This also includes an auxiliary version of the main touch itself. Only valid for NSEventTypeDirectTouch events.
*/
- (NSArray <NSTouch *> *)coalescedTouchesForTouch:(NSTouch *)touch API_AVAILABLE(macos(10.12.2));

/* The phase of a gesture scroll event. A gesture phrase are all the events that begin with a NSEventPhaseBegan and end with either a NSEventPhaseEnded or NSEventPhaseCancelled. All the gesture events are sent to the view under the cursor when the NSEventPhaseBegan occurred.  A gesture scroll event starts with a NSEventPhaseBegan phase and ends with a NSPhaseEnded. Legacy scroll wheel events (say from a Mighty Mouse) and momentum scroll wheel events have a phase of NSEventPhaseNone.
    Valid for NSEventTypeScrollWheel
*/
@property (readonly) NSEventPhase phase API_AVAILABLE(macos(10.7));

/* This message is valid for NSEventTypePressure events. Pressure gesture events go through multiple stages.
*/
@property (readonly) NSInteger stage API_AVAILABLE(macos(10.10.3));

/* This message is valid for NSEventTypePressure events. Positive stageTransition describes approaching the next stage of the pressure gesture. Negative stageTransition describes approaching release of the current stage.
*/
@property (readonly) CGFloat stageTransition API_AVAILABLE(macos(10.10.3));

/* This message is valid for Mouse events. The event mask describing the various events that you may also get during this event sequence. Useful for determining if the input device issuing this mouse event can also simultaneously issue NSEventTypePressure events.
*/
@property (readonly) NSEventMask associatedEventsMask API_AVAILABLE(macos(10.10.3));

/* this message is valid for NSEventTypePressure events */
@property (readonly) NSPressureBehavior pressureBehavior API_AVAILABLE(macos(10.11));

/* Returns the user's preference about using gesture scrolls as a way to track fluid swipes. This value is determined by the Mouse / Trackpad preference panel for the current user. Generally, NSScrollView will check this for you. However, if your app is not using an NSScrollView, or your NSResponder can receive scrollWheel messages without first being sent to an NSScrollView, then you should check this preference before calling -trackSwipeEventWithOptions:dampenAmountThresholdMin:max:usingHandler:
*/
@property (class, readonly, getter=isSwipeTrackingFromScrollEventsEnabled) BOOL swipeTrackingFromScrollEventsEnabled API_AVAILABLE(macos(10.7));

/* This method allows easy tracking and UI feedback of scroll events as fluid swipes. Fluid swipes are tracked not only to the end of the physical gesture phase by the user, but also to the completion of any UI animation that should be performed. Using this method allows your implementation to maintain a consistent fluid feel with other applications. Any gesture amount outside of the supplied minimum and maximum dampen amount is pre-dampened for you to provide an elastic feel.

   The trackingHandler has the following parameters:
       gestureAmount: the amount of gesture that you should display in the UI. This may be a fractional amount. Note: Upon completion, the gesture amount will animate to one of the following values: -1, 0, 1.
       phase: the phase of the physical gesture as performed by the user. When the phase is either NSPhaseEnded, or NSPhaseCancelled, the use has physically ended the gesture successfully or not, respectively. Your handler will continue to be called with updated progress values to complete the fluid swipe animation with a phase of NSPhaseNone.
       isComplete: Signifies the swipe and animation are complete and you should tear down any temporary animation objects. The trackingHandler is released and will not be called further.
      *stop: A pointer to a BOOL allowing you to cancel further use of this trackingHandler by setting its value to YES. The trackingHandler is released appropriately.

   Note: This method returns immediately and tracking is performed asynchronously.

   Valid for Scroll events with a phase of NSEventPhaseBegan or NSEventPhaseChanged
*/
- (void)trackSwipeEventWithOptions:(NSEventSwipeTrackingOptions)options dampenAmountThresholdMin:(CGFloat)minDampenThreshold max:(CGFloat)maxDampenThreshold usingHandler:(void (^)(CGFloat gestureAmount, NSEventPhase phase, BOOL isComplete, BOOL *stop))trackingHandler API_AVAILABLE(macos(10.7));

/* used for initial delay and periodic behavior in tracking loops */
+ (void)startPeriodicEventsAfterDelay:(NSTimeInterval)delay withPeriod:(NSTimeInterval)period;
+ (void)stopPeriodicEvents;

/* apps will rarely create these objects */
+ (nullable NSEvent *)mouseEventWithType:(NSEventType)type location:(NSPoint)location modifierFlags:(NSEventModifierFlags)flags timestamp:(NSTimeInterval)time windowNumber:(NSInteger)wNum context:(nullable NSGraphicsContext* __unused)unusedPassNil eventNumber:(NSInteger)eNum clickCount:(NSInteger)cNum pressure:(float)pressure;
+ (nullable NSEvent *)keyEventWithType:(NSEventType)type location:(NSPoint)location modifierFlags:(NSEventModifierFlags)flags timestamp:(NSTimeInterval)time windowNumber:(NSInteger)wNum context:(nullable NSGraphicsContext* __unused)unusedPassNil characters:(NSString *)keys charactersIgnoringModifiers:(NSString *)ukeys isARepeat:(BOOL)flag keyCode:(unsigned short)code;
+ (nullable NSEvent *)enterExitEventWithType:(NSEventType)type location:(NSPoint)location modifierFlags:(NSEventModifierFlags)flags timestamp:(NSTimeInterval)time windowNumber:(NSInteger)wNum context:(nullable NSGraphicsContext* __unused)unusedPassNil eventNumber:(NSInteger)eNum trackingNumber:(NSInteger)tNum userData:(nullable void *)data;
+ (nullable NSEvent *)otherEventWithType:(NSEventType)type location:(NSPoint)location modifierFlags:(NSEventModifierFlags)flags timestamp:(NSTimeInterval)time windowNumber:(NSInteger)wNum context:(nullable NSGraphicsContext* __unused)unusedPassNil subtype:(short)subtype data1:(NSInteger)d1 data2:(NSInteger)d2;

// global mouse coordinates
@property (class, readonly) NSPoint mouseLocation;

/* modifier keys currently down.  This returns the state of devices combined with synthesized events at the moment, independent of which events have been delivered via the event stream. */
@property (class, readonly) NSEventModifierFlags modifierFlags API_AVAILABLE(macos(10.6));

/* mouse buttons currently down.  Returns indices of the mouse buttons currently down.  1 << 0 corresponds to leftMouse, 1 << 1 to rightMouse, and 1 << n, n >= 2 to other mouse buttons.  This returns the state of devices combined with synthesized events at the moment, independent of which events have been delivered via the event stream, so this method is not suitable for tracking. */
@property (class, readonly) NSUInteger pressedMouseButtons API_AVAILABLE(macos(10.6));

/* the time in which a second click must occur in order to be considered a doubleClick.  This is a system value so overrides will have no effect. */
@property (class, readonly) NSTimeInterval doubleClickInterval   API_AVAILABLE(macos(10.6));
/* the time for which a key must be held down in order to generate the first key repeat event.  This is a system value so overrides will have no effect. */
@property (class, readonly) NSTimeInterval keyRepeatDelay        API_AVAILABLE(macos(10.6));
/* the time between subsequent key repeat events.  This is a system value so overrides will have no effect. */
@property (class, readonly) NSTimeInterval keyRepeatInterval     API_AVAILABLE(macos(10.6));

/*
   API for monitoring events in other processes, or in your own process. For either +addGlobal or +addLocal, pass an event mask specifying which events you wish to monitor, and a block that will be called with the event to monitor.

   Use +addGlobal to install an event monitor that receives copies of events posted to other applications. Events are delivered asynchronously to your app and you can only observe the event; you cannot modify or otherwise prevent the event from being delivered to its original target application. Key-related events may only be monitored if accessibility is enabled or if your application is trusted for accessibility access (see AXIsProcessTrusted in AXUIElement.h). Note that your handler will not be called for events that are sent to your own application.

   Use +addLocal to install an event monitor that receives events before they are dispatched by -[NSApplication sendEvent:]. In this case, your block should either return a valid NSEvent (which may be the same as the incoming NSEvent, or may be a newly created NSEvent) to cause the event to be dispatched, or it may return nil to stop dispatching of the event. Note that your handler will not be called for events that are consumed by nested event-tracking loops such as control tracking, menu tracking, or window dragging; only events that are dispatched through -[NSApplication sendEvent:] will be passed to your handler.

   In either case, the return value of the API is a retained NSObject. You typically do not need to retain and release the event monitor yourself, since the implementation will retain it while needed.
   
   To remove the event monitor, under both garbage collection and non-GC, pass the return value from the +add API to +removeMonitor.
*/
+ (nullable id)addGlobalMonitorForEventsMatchingMask:(NSEventMask)mask handler:(void (^)(NSEvent *event))block API_AVAILABLE(macos(10.6));
+ (nullable id)addLocalMonitorForEventsMatchingMask:(NSEventMask)mask handler:(NSEvent* _Nullable (^)(NSEvent *event))block API_AVAILABLE(macos(10.6));
+ (void)removeMonitor:(id)eventMonitor API_AVAILABLE(macos(10.6));

@end


/* Unicodes we reserve for function keys on the keyboard,  OpenStep reserves the range 0xF700-0xF8FF for this purpose.  The availability of various keys will be system dependent. */
enum {
    NSUpArrowFunctionKey        = 0xF700,
    NSDownArrowFunctionKey      = 0xF701,
    NSLeftArrowFunctionKey      = 0xF702,
    NSRightArrowFunctionKey     = 0xF703,
    NSF1FunctionKey             = 0xF704,
    NSF2FunctionKey             = 0xF705,
    NSF3FunctionKey             = 0xF706,
    NSF4FunctionKey             = 0xF707,
    NSF5FunctionKey             = 0xF708,
    NSF6FunctionKey             = 0xF709,
    NSF7FunctionKey             = 0xF70A,
    NSF8FunctionKey             = 0xF70B,
    NSF9FunctionKey             = 0xF70C,
    NSF10FunctionKey            = 0xF70D,
    NSF11FunctionKey            = 0xF70E,
    NSF12FunctionKey            = 0xF70F,
    NSF13FunctionKey            = 0xF710,
    NSF14FunctionKey            = 0xF711,
    NSF15FunctionKey            = 0xF712,
    NSF16FunctionKey            = 0xF713,
    NSF17FunctionKey            = 0xF714,
    NSF18FunctionKey            = 0xF715,
    NSF19FunctionKey            = 0xF716,
    NSF20FunctionKey            = 0xF717,
    NSF21FunctionKey            = 0xF718,
    NSF22FunctionKey            = 0xF719,
    NSF23FunctionKey            = 0xF71A,
    NSF24FunctionKey            = 0xF71B,
    NSF25FunctionKey            = 0xF71C,
    NSF26FunctionKey            = 0xF71D,
    NSF27FunctionKey            = 0xF71E,
    NSF28FunctionKey            = 0xF71F,
    NSF29FunctionKey            = 0xF720,
    NSF30FunctionKey            = 0xF721,
    NSF31FunctionKey            = 0xF722,
    NSF32FunctionKey            = 0xF723,
    NSF33FunctionKey            = 0xF724,
    NSF34FunctionKey            = 0xF725,
    NSF35FunctionKey            = 0xF726,
    NSInsertFunctionKey         = 0xF727,
    NSDeleteFunctionKey         = 0xF728,
    NSHomeFunctionKey           = 0xF729,
    NSBeginFunctionKey          = 0xF72A,
    NSEndFunctionKey            = 0xF72B,
    NSPageUpFunctionKey         = 0xF72C,
    NSPageDownFunctionKey       = 0xF72D,
    NSPrintScreenFunctionKey    = 0xF72E,
    NSScrollLockFunctionKey     = 0xF72F,
    NSPauseFunctionKey          = 0xF730,
    NSSysReqFunctionKey         = 0xF731,
    NSBreakFunctionKey          = 0xF732,
    NSResetFunctionKey          = 0xF733,
    NSStopFunctionKey           = 0xF734,
    NSMenuFunctionKey           = 0xF735,
    NSUserFunctionKey           = 0xF736,
    NSSystemFunctionKey         = 0xF737,
    NSPrintFunctionKey          = 0xF738,
    NSClearLineFunctionKey      = 0xF739,
    NSClearDisplayFunctionKey   = 0xF73A,
    NSInsertLineFunctionKey     = 0xF73B,
    NSDeleteLineFunctionKey     = 0xF73C,
    NSInsertCharFunctionKey     = 0xF73D,
    NSDeleteCharFunctionKey     = 0xF73E,
    NSPrevFunctionKey           = 0xF73F,
    NSNextFunctionKey           = 0xF740,
    NSSelectFunctionKey         = 0xF741,
    NSExecuteFunctionKey        = 0xF742,
    NSUndoFunctionKey           = 0xF743,
    NSRedoFunctionKey           = 0xF744,
    NSFindFunctionKey           = 0xF745,
    NSHelpFunctionKey           = 0xF746,
    NSModeSwitchFunctionKey     = 0xF747
};



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
