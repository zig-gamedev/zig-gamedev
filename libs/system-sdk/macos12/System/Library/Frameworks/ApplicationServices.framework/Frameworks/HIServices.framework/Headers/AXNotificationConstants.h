/*
 *  AXNotificationConstants.h
 *  HIServices
 *
 *  Copyright (c) 2004, 2012 Apple Inc. All rights reserved.
 *
 */

#ifndef __AXNOTIFICATIONCONSTANTS__
#define __AXNOTIFICATIONCONSTANTS__

#include <CoreFoundation/CFBase.h> /* CF_ENUM */

/*!
	@header AXNotificationConstants.h
	
	@discussion
	Assistive applications can register to be notified about certain events in a target application. For example, creation of a window or the destruction
	of a UIElement. To receive notifications you must first create an observer and specify a callback function; second, add the observer's run loop source
	to the run loop on which you want the callback executed; and third, register the observer for one or more notifications.

	When you create the observer, you specify the application being observed. An observer can receive notifications only from UIElements in that application.
	To handle multiple applications, you have to create at least one observer per application.

	When you register an observer for a notification, you specify the UIElement you are interested in observing. When you want to receive a notification from
	any element in an application, use the application UIElement; you then receive the notification regardless of which element in the application sends the
	notification. This is useful if the UIElement does not exist yet, such as when a new window is created, or if you care about state changes, such as the
	keyboard focus moving, without having to observe every element separately. When the callback function is executed it is passed the UIElement that was
	affected by the notification.

	Observers are represented by the AXObserverRef type, which is a CFType. Like all CFTypes they are reference counted (CFRetain/CFRelease).
 */

/*-----------------------------------------*/
/*! @group Focus Notifications             */
/*-----------------------------------------*/

/* Need discussion for following */

/*!
	@defined kAXMainWindowChangedNotification
	
	@abstract
	Notification of a change in the main window.

	@discussion
	Value is the new main window UIElement or the
	Application UIElement if there's no main window.
 */
#define kAXMainWindowChangedNotification		CFSTR("AXMainWindowChanged")

/*!
	@defined kAXFocusedWindowChangedNotification
	@abstract Notification that the focused window changed.
	
 */
#define kAXFocusedWindowChangedNotification		CFSTR("AXFocusedWindowChanged")

/*!
	@defined kAXFocusedUIElementChangedNotification
	@abstract Notification that the focused UI element has changed.
	@discussion
		Value is the new focused UIElement or
		the Application UIElement if there's no focus
 */
#define kAXFocusedUIElementChangedNotification		CFSTR("AXFocusedUIElementChanged")

/*-----------------------------------------*/
/*! @group Application Notifications       */
/*-----------------------------------------*/

/*!
	@defined kAXApplicationActivatedNotification
	@abstract Notification that an application was activated.
	@discussion Value is an application UIElement.
*/
#define kAXApplicationActivatedNotification		CFSTR("AXApplicationActivated")

/*!
	@defined kAXApplicationDeactivatedNotification
	@abstract Notification that an application was deactivated.
	@discussion
		Value is an application UIElement
 */
#define kAXApplicationDeactivatedNotification		CFSTR("AXApplicationDeactivated")

/*!
	@defined kAXApplicationHiddenNotification
	@abstract Notification that an application has been hidden.
	@discussion
		Value is an application UIElement
 */
#define kAXApplicationHiddenNotification		CFSTR("AXApplicationHidden")

/*!
	@defined kAXApplicationShownNotification
	@abstract Notification that an application is no longer hidden.
	@discussion
		Value is an application UIElement
 */
#define kAXApplicationShownNotification			CFSTR("AXApplicationShown")

/*-----------------------------------------*/
/*! @group Window Notifications            */
/*-----------------------------------------*/

/*!
	@defined kAXWindowCreatedNotification
	@abstract Notification that a window was created.
	@discussion
		Value is a new window UIElement
*/
#define kAXWindowCreatedNotification			CFSTR("AXWindowCreated")

/*!
	@defined kAXWindowMovedNotification
	@abstract Notification that a window moved.
	@discussion
		This notification is sent at the end of the window move, not continuously as the window is being moved.

		Value is the moved window UIElement
 */
#define kAXWindowMovedNotification			CFSTR("AXWindowMoved")

/*!
	@defined kAXWindowResizedNotification
	@abstract Notification that a window was resized.
	@discussion
		This notification is sent at the end of the window resize, not continuously as the window is being resized.

		Value is the resized window UIElement
 */
#define kAXWindowResizedNotification			CFSTR("AXWindowResized")

/*!
	@defined kAXWindowMiniaturizedNotification
	@abstract Notification that a window was minimized.

	@discussion
		Value is the minimized window UIElement
 */
#define kAXWindowMiniaturizedNotification		CFSTR("AXWindowMiniaturized")

/*!
	@defined kAXWindowDeminiaturizedNotification
	@abstract Notification that a window is no longer minimized.
	@discussion
		Value is the unminimized window UIElement
 */
#define kAXWindowDeminiaturizedNotification		CFSTR("AXWindowDeminiaturized")

/*---------------------------------------------------*/
/*! @group New Drawer, Sheet, and Help Notifications */
/*---------------------------------------------------*/

/*!
	@defined kAXDrawerCreatedNotification
	@abstract Notification that a drawer was created.
*/
#define kAXDrawerCreatedNotification			CFSTR("AXDrawerCreated")
/*!
	@defined kAXSheetCreatedNotification
	@abstract Notification that a sheet was created.
*/
#define kAXSheetCreatedNotification			CFSTR("AXSheetCreated")
/*!
	@defined kAXHelpTagCreatedNotification
	@abstract Notification that a help tag was created.
*/
#define kAXHelpTagCreatedNotification			CFSTR("AXHelpTagCreated")

/*-----------------------------------------*/
/*! @group Element Notifications           */
/*-----------------------------------------*/

/*!
	@defined kAXValueChangedNotification
	@discussion
		This notification is sent when the value of the UIElement's <b>value</b> attribute has changed, not when the value of any other attribute has changed.

		Value is the modified UIElement
*/
#define kAXValueChangedNotification			CFSTR("AXValueChanged")

/*!
	@defined kAXUIElementDestroyedNotification

	@discussion
		The returned UIElement is no longer valid in the target application. You can still use the local reference
		with calls like CFEqual (for example, to remove it from a list), but you should not pass it to the accessibility APIs.

		Value is the destroyed UIElement
 */
#define kAXUIElementDestroyedNotification		CFSTR("AXUIElementDestroyed")

/*!
 @defined kAXElementBusyChangedNotification
 @abstract Notification that an element's busy state has changed.
 @discussion
    Value is the (un)busy UIElement.
 */
#define kAXElementBusyChangedNotification		CFSTR("AXElementBusyChanged")

/*-----------------------------------------*/
/*! @group Menu Notifications              */
/*-----------------------------------------*/
/*!
	@defined kAXMenuOpenedNotification
	@abstract Notification that a menu has been opened.
	@discussion
		Value is the opened menu UIElement.
 */
#define kAXMenuOpenedNotification			CFSTR("AXMenuOpened")
/*!
	@defined kAXMenuClosedNotification
	@abstract Notification that a menu has been closed.
	@discussion
		Value is the closed menu UIElement.
 */
#define kAXMenuClosedNotification			CFSTR("AXMenuClosed")
/*!
	@defined kAXMenuItemSelectedNotification
	@abstract Notification that a menu item has been seleted.
	@discussion
		Value is the selected menu item UIElement.
 */
#define kAXMenuItemSelectedNotification			CFSTR("AXMenuItemSelected")

/*-----------------------------------------*/
/*! @group Table/outline notifications     */
/*-----------------------------------------*/

/*! @abstract Notification that the number of rows in this table has changed.
 */
#define kAXRowCountChangedNotification			CFSTR("AXRowCountChanged")

/*-----------------------------------------*/
/*! @group Outline notifications           */
/*-----------------------------------------*/

/*! @abstract Notification that a row in an outline has been expanded.
    @discussion
	The value is the collapsed row UIElement.
 */
#define kAXRowExpandedNotification			CFSTR("AXRowExpanded")

/*! @abstract Notification that a row in an outline has been collapsed.
    @discussion
	The value is the collapsed row UIElement.
 */
#define kAXRowCollapsedNotification			CFSTR("AXRowCollapsed")

/*-----------------------------------------*/
/*! @group Cell-based table notifications  */
/*-----------------------------------------*/

/*! @abstract Notification that the selected cells have changed.
 */
#define kAXSelectedCellsChangedNotification		CFSTR("AXSelectedCellsChanged")

/*-----------------------------------------*/
/*! @group Layout area notifications       */
/*-----------------------------------------*/

/*! @abstract Notification that the units have changed.
 */
#define kAXUnitsChangedNotification			CFSTR("AXUnitsChanged")  

/*! @abstract Notification that the selected children have moved.
 */
#define kAXSelectedChildrenMovedNotification		CFSTR("AXSelectedChildrenMoved")

/*-----------------------------------------*/
/*! @group Other notifications             */
/*-----------------------------------------*/

/*! @abstract Notification that a different subset of this element's children were selected.
 */
#define kAXSelectedChildrenChangedNotification		CFSTR("AXSelectedChildrenChanged")

/*! @abstract Notification that this element has been resized.
 */
#define kAXResizedNotification				CFSTR("AXResized")

/*! @abstract Notification that this element has moved.
 */
#define kAXMovedNotification				CFSTR("AXMoved")

/*! @abstract Notification that an element was created.
 */
#define kAXCreatedNotification				CFSTR("AXCreated")

/*! @abstract Notification that the set of selected rows changed.
 */
#define kAXSelectedRowsChangedNotification	CFSTR("AXSelectedRowsChanged")

/*! @abstract Notification that the set of selected columns changed.
 */
#define kAXSelectedColumnsChangedNotification	CFSTR("AXSelectedColumnsChanged")

/*! @abstract Notification that a different set of text was selected.
 */
#define kAXSelectedTextChangedNotification	CFSTR("AXSelectedTextChanged")

/*! @abstract Notification that the title changed.
 */
#define kAXTitleChangedNotification             CFSTR("AXTitleChanged")

/*! @abstract Notification that the layout changed.
 */
#define kAXLayoutChangedNotification			CFSTR("AXLayoutChanged")

/*! @abstract Notification to request an announcement to be spoken.
 */
#define kAXAnnouncementRequestedNotification	CFSTR("AXAnnouncementRequested")

/*! @abstract Notification info key used to specify an element.
 */
#define kAXUIElementsKey	CFSTR("AXUIElementsKey")

/*! @abstract Notification info key used to specify a priority for the notification. See AXPriority.
 */
#define kAXPriorityKey      CFSTR("AXPriorityKey")

/*! @abstract Notification info key used to specify an announcement to be spoken with a notification.
 */
#define kAXAnnouncementKey	CFSTR("AXAnnouncementKey")

/*! @abstract Notification info key used to specify the title of an element to be spoken with a notification.
 */
#define kAXUIElementTitleKey    CFSTR("AXUIElementTitleKey")

/*! @abstract Priority values used for kAXPriorityKey
 */
typedef CF_ENUM(CFIndex, AXPriority) {
    kAXPriorityLow      = 10,
    kAXPriorityMedium   = 50,
    kAXPriorityHigh     = 90
} CF_ENUM_AVAILABLE_MAC(10_9);
    
#endif // __AXNOTIFICATIONCONSTANTS__
