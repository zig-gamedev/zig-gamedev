/*
	NSAccessibility.h
	Application Kit
	Copyright (c) 2001-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <AppKit/NSErrors.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSObject.h>
#import <AppKit/NSAccessibilityConstants.h>
#import <AppKit/NSAccessibilityProtocols.h>
#import <AppKit/NSAccessibilityElement.h>
#import <AppKit/NSAccessibilityCustomRotor.h>
// #import <AppKit/NSAccessibilityColor.h> - this caused a build failure (60459719), and needs to be disabled until 60467092 is addressed
#import <AppKit/NSWorkspace.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString, NSArray, NSView;

/*** Accessibility Informal Protocol ***/

@interface NSObject (NSAccessibility)

/* Attribute methods
*/
- (NSArray<NSAccessibilityAttributeName> *)accessibilityAttributeNames API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));
- (nullable id)accessibilityAttributeValue:(NSAccessibilityAttributeName)attribute API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));
- (BOOL)accessibilityIsAttributeSettable:(NSAccessibilityAttributeName)attribute API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));
- (void)accessibilitySetValue:(nullable id)value forAttribute:(NSAccessibilityAttributeName)attribute API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));

/* Parameterized Attribute methods
*/
- (NSArray<NSAccessibilityParameterizedAttributeName> *)accessibilityParameterizedAttributeNames API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));
- (nullable id)accessibilityAttributeValue:(NSAccessibilityParameterizedAttributeName)attribute forParameter:(nullable id)parameter API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));

/* Accessibility action methods
*/
- (NSArray<NSAccessibilityActionName> *)accessibilityActionNames API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));
- (nullable NSString *)accessibilityActionDescription:(NSAccessibilityActionName)action API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));
- (void)accessibilityPerformAction:(NSAccessibilityActionName)action API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));

/* Return YES if the UIElement doesn't show up to the outside world - i.e. its parent should return the UIElement's children as its own - cutting the UIElement out. E.g. NSControls are ignored when they are single-celled.
*/
- (BOOL)accessibilityIsIgnored API_DEPRECATED("Use isAccessibilityElement instead", macos(10.1,10.10));

/* Returns the deepest descendant of the UIElement hierarchy that contains the point. You can assume the point has already been determined to lie within the receiver. Override this method to do deeper hit testing within a UIElement - e.g. a NSMatrix would test its cells. The point is bottom-left relative screen coordinates.
*/
- (nullable id)accessibilityHitTest:(NSPoint)point;

/* Returns the UI Element that has the focus. You can assume that the search for the focus has already been narrowed down to the receiver. Override this method to do a deeper search with a UIElement - e.g. a NSMatrix would determine if one of its cells has the focus.
*/
@property (nullable, readonly, strong) id accessibilityFocusedUIElement;


/* The following are optional methods to improve performance of accessible objects with large numbers of children or large numbers of UI Elements returned from attributes that return an array (selected children, or visible children, for instance).  The default implementation for these operations will call -accessibilityAttributeValue: to retrieve the entire array of values, and then perform the appropriate operation.  If these methods are implemented, they will be used instead.  For accessibility objects with many children, the results to these methods can sometimes be calculated without generating the entire array of children which can improve performance.
*/

/* Given an accessibility child of an object, return the index of that child in the parent.  If the provided object is not a child of the receiver, this method should return NSNotFound.
*/
- (NSUInteger)accessibilityIndexOfChild:(id)child;

/* Return the count of an accessibility array attribute.
*/
- (NSUInteger)accessibilityArrayAttributeCount:(NSAccessibilityAttributeName)attribute;

/* Return a subarray of values of an accessibility array attribute.  Note this method does not take a range.  The max count is the maximum desired number of items requested by an accessibility client.  This number may be beyond the bounds of your array.
*/
- (NSArray *)accessibilityArrayAttributeValues:(NSAccessibilityAttributeName)attribute index:(NSUInteger)index maxCount:(NSUInteger)maxCount;

/*
 Prior to OS X 10.9, the only accessible objects that could post accessibility notifications were those that inherited from NSView, NSWindow, or NSCell. An application's custom accessible object, subclassed from NSObject, could not post notifications.
 
 In 10.9 and later, an application's custom accessible object may post accessibility notifications if it follows the following guidelines:
 
 - the lifetime of the NSObject must match the lifetime of the corresponding element in the application's visual interface. It is common for a custom accessible object that acts as a proxy for an onscreen UI element to be autoreleased and deallocated very quickly, immediately after the application responds to a single accessibility request. This is not sufficient to support posting notifications, because any notification observers that are registered on the object will be removed as soon as the object is deallocated. Instead, the application must arrange for an accessible object that refers to a specific UI element to remain allocated as long as that UI element is visible.
 
 - the object must post the NSAccessibilityUIElementDestroyed notification at appropriate times, typically when the corresponding UI element in the application's visual interface is removed from the screen, and certainly when the accessible object is deallocated.
 
 - the object must implement -accessibilityNotifiesWhenDestroyed to return YES.
 */
@property (readonly) BOOL accessibilityNotifiesWhenDestroyed API_AVAILABLE(macos(10.9));

@end


@interface NSWorkspace (NSWorkspaceAccessibilityDisplay)

/* Get the current accessibility display option for high-contrast UI. If this is true, UI should be presented with high contrast such as utilizing a less subtle color palette or bolder lines. You may listen for NSWorkspaceAccessibilityDisplayOptionsDidChangeNotification to be notified when this changes. */
@property (readonly) BOOL accessibilityDisplayShouldIncreaseContrast API_AVAILABLE(macos(10.10));

/* Get the current accessibility display option for differentiate without color. If this is true, UI should not convey information using color alone and instead should use shapes or glyphs to convey information. You may listen for NSWorkspaceAccessibilityDisplayOptionsDidChangeNotification to be notified when this changes. */
@property (readonly) BOOL accessibilityDisplayShouldDifferentiateWithoutColor API_AVAILABLE(macos(10.10));

/* Get the current accessibility display option for reduce transparency. If this property's value is true, UI (mainly window) backgrounds should not be semi-transparent; they should be opaque. You may listen for NSWorkspaceAccessibilityDisplayOptionsDidChangeNotification to be notified when this changes. */
@property (readonly) BOOL accessibilityDisplayShouldReduceTransparency API_AVAILABLE(macos(10.10));

/* Get the current accessibility display option for reduce motion. If this property's value is true, UI should avoid large animations, especially those that simulate the third dimension. You may listen for NSWorkspaceAccessibilityDisplayOptionsDidChangeNotification to be notified when this changes. */
@property (readonly) BOOL accessibilityDisplayShouldReduceMotion API_AVAILABLE(macos(10.12));

/* Get the current accessibility display option for invert colors. If this property's value is true then the display will be inverted. In these cases it may be needed for UI drawing to be adjusted to in order to display optimally when inverted.  You may listen for NSWorkspaceAccessibilityDisplayOptionsDidChangeNotification to be notified when this changes. */
@property (readonly) BOOL accessibilityDisplayShouldInvertColors API_AVAILABLE(macos(10.12));

@end

@interface NSWorkspace (NSWorkspaceAccessibility)

/* Indicates if VoiceOver is currently running. This is observable through KVO. */
@property (readonly, getter=isVoiceOverEnabled) BOOL voiceOverEnabled API_AVAILABLE(macos(10.13));

/* Indicates if Switch Control is currently running. This is observable through KVO. */
@property (readonly, getter=isSwitchControlEnabled) BOOL switchControlEnabled API_AVAILABLE(macos(10.13));

@end

/* Notification posted to the NSWorkspace notification center when accessibility display options have changed. */
APPKIT_EXTERN NSNotificationName const NSWorkspaceAccessibilityDisplayOptionsDidChangeNotification API_AVAILABLE(macos(10.10));

/*** Accessibility Related Methods ***/

@interface NSObject (NSAccessibilityAdditions)

/* For changing the set of attributes on an instance - as an alternative to sub-classing.
*/
- (BOOL)accessibilitySetOverrideValue:(nullable id)value forAttribute:(NSAccessibilityAttributeName)attribute API_DEPRECATED("Use the NSAccessibility protocol methods instead (see NSAccessibilityProtocols.h)", macos(10.1,10.10));

@end

// With a NSView and a relative frame, return the frame in screen coordinates for accessibility
APPKIT_EXTERN NSRect NSAccessibilityFrameInView(NSView *parentView, NSRect frame) API_AVAILABLE(macos(10.10));

// With a NSView and a relative point, return the frame in screen coordinates for accessibility
APPKIT_EXTERN NSPoint NSAccessibilityPointInView(NSView *parentView, NSPoint point) API_AVAILABLE(macos(10.10));


/* Setting whether the application may have protected content. Protected content is identified by a value of TRUE for the accessibility attribute NSAccessibilityContainsProtectedContentAttribute. If NSAccessibilityMayContainProtectedContent returns NO, then NSAccessibilityContainsProtectedContentAttribute is completely ignored.

    A return value of TRUE indicates success.
 */
APPKIT_EXTERN BOOL NSAccessibilitySetMayContainProtectedContent(BOOL flag);

/* Getting descriptions for standard roles and actions.
*/
APPKIT_EXTERN NSString * _Nullable NSAccessibilityRoleDescription(NSAccessibilityRole role, NSAccessibilitySubrole _Nullable subrole);
APPKIT_EXTERN NSString * _Nullable NSAccessibilityRoleDescriptionForUIElement(id element);
APPKIT_EXTERN NSString * _Nullable NSAccessibilityActionDescription(NSAccessibilityActionName action);


/* Error signaling for bad setter value or bad parameter.
*/
APPKIT_EXTERN void NSAccessibilityRaiseBadArgumentException(_Null_unspecified id element, NSAccessibilityAttributeName _Null_unspecified attribute, _Null_unspecified id value) API_DEPRECATED("Exceptions are no longer appropriate for indicating errors in accessibility API. Unexpected values should be handled through appropriate type checking.", macos(10.1,10.11));


/*** Ignored UIElements Utilities ***/

APPKIT_EXTERN _Nullable id NSAccessibilityUnignoredAncestor(id element);
APPKIT_EXTERN _Nullable id NSAccessibilityUnignoredDescendant(id element);
APPKIT_EXTERN NSArray *NSAccessibilityUnignoredChildren(NSArray *originalChildren);
APPKIT_EXTERN NSArray *NSAccessibilityUnignoredChildrenForOnlyChild(id originalChild);


/*** Posting Notifications ***/

/* Posts a notification to accessibility client observers.  Note that accessibility notifications are not NSNotifications and do not use the NSNotificationCenter mechanism.  These notifications are received by client processes using the AX API defined in <HIServices/Accessibility.h> including AXUIElement.h.

For all notifications, the observer receives the provided notification string and the AX API representation of the provided element. 

For most notifications, the provided element is checked for observers of the provided notification.

For some notifications, the accessibility parent of the provided element is checked for observers instead.  An example is NSAccessibilityCreatedNotification.  It is impossible for a client to register to observe this notification on a new element, since the element does not exist yet.  So, the function would be called passing in the new element, and the accessibility parent of the element is automatically checked for registered observers.

The following notifications check the accessibility parent of the provided element for observers: NSAccessibilityCreatedNotification, NSAccessibilityWindowCreatedNotification, NSAccessibilityDrawerCreatedNotification, NSAccessibilitySheetCreatedNotification, NSAccessibilityHelpTagCreatedNotification, NSAccessibilityRowExpandedNotification, NSAccessibilityRowCollapsedNotification.

For another set of notifications, the NSApp instance is always the observed element.  An example is NSAccessibilityFocusedUIElementChangedNotification.  The provided element would be the element that now has focus, and the function will automatically check NSApp for registered observers.

The following notifications always check for accessibility observers of NSApp: NSAccessibilityFocusedUIElementChangedNotification, NSAccessibilityFocusedWindowChangedNotification, NSAccessibilityMainWindowChangedNotification.

The rule of thumb is that the affected element should be passed into the function (the newly created element, the newly focused element, the row that was expanded, etc.), and the function will check for observer registrations on the correct element.

*/
APPKIT_EXTERN void NSAccessibilityPostNotification(id element, NSAccessibilityNotificationName notification);

#ifdef __swift__
// This struct is not available in Objective-C. Its only purpose is to create a namespace for accessibility symbols in Swift.
typedef struct NSAccessibility { void *_reserved; } NSAccessibility;
#endif

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

