/*
 *  AXAttributeConstants.h
 *  HIServices
 *
 *  Copyright (c) 2004, 2006, 2012 Apple Inc. All rights reserved.
 *
 */

#ifndef __AXATTRIBUTECONSTANTS__
#define __AXATTRIBUTECONSTANTS__

#include <CoreFoundation/CoreFoundation.h> /* CF_OPTIONS */

#ifndef __MACTYPES__
#include <MacTypes.h> /* UInt32 */
#endif

#include <CoreFoundation/CFAvailability.h> /* CF_OPTIONS */

/*! @header AXAttributeConstants.h
	@discussion Each UIElement has a set of attributes that assistive applications use to get information about the UIElement.
	The list of attributes vary depending on the type of UIElement. The value of some attributes can be changed, while others cannot.
	For example, changing the "value" attribute of a slider changes the slider's setting.

	Attribute values are stored as Core Foundation types, CFTypeRefs, and are reference counted (CFRetain/CFRelease). Some attributes
	have a particular type associated with them. For example, the "title" attribute, if defined, always has a string value, regardless
	of the type of UIElement from which it is obtained. A UIElement's "value" attribute, however, varies with the UIElement. For
	example, a text field's value is a string whereas a checkbox's value is a boolean. You need to explictly test the returned objects,
	using the CFGetTypeID function, for what type they really are.

	Finally, some attribute values hold simple structures, such as CGPoint and CGRect, instead of regular CFTypes. These are still passed
	between the target and assistive application as CFTypeRefs, but they merely wrap an encoded version of the structure. You need to use
	the functions AXValueCreate and AXValueGetValue to convert between the structures and CFTypeRefs. Each supported structure has an
	AXValueType associated with it. The AXValueGetType function returns the AXValueType of the structure contained within a CFTypeRef.
*/

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Attributes                                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
	Quick reference:
	
	// informational attributes
	kAXRoleAttribute
	kAXSubroleAttribute
	kAXRoleDescriptionAttribute
	kAXTitleAttribute
	kAXDescriptionAttribute
	kAXHelpAttribute
	
	// hierarchy or relationship attributes
	kAXParentAttribute
	kAXChildrenAttribute
	kAXSelectedChildrenAttribute
	kAXVisibleChildrenAttribute
	kAXWindowAttribute
	kAXTopLevelUIElementAttribute
	kAXTitleUIElementAttribute
	kAXServesAsTitleForUIElementsAttribute
	kAXLinkedUIElementsAttribute
    kAXSharedFocusElementsAttribute
	
	// visual state attributes
	kAXEnabledAttribute
	kAXFocusedAttribute
	kAXPositionAttribute
	kAXSizeAttribute
	
	// value attributes
	kAXValueAttribute
    kAXValueDescriptionAttribute
	kAXMinValueAttribute
	kAXMaxValueAttribute
	kAXValueIncrementAttribute
	kAXValueWrapsAttribute
	kAXAllowedValuesAttribute
	
	// text-specific attributes
	kAXSelectedTextAttribute
	kAXSelectedTextRangeAttribute
    kAXSelectedTextRangesAttribute
	kAXVisibleCharacterRangeAttribute
	kAXNumberOfCharactersAttribute
	kAXSharedTextUIElementsAttribute
	kAXSharedCharacterRangeAttribute
	
	// window, sheet, or drawer-specific attributes
	kAXMainAttribute
	kAXMinimizedAttribute
	kAXCloseButtonAttribute
	kAXZoomButtonAttribute
	kAXMinimizeButtonAttribute
	kAXToolbarButtonAttribute
	kAXProxyAttribute
	kAXGrowAreaAttribute
	kAXModalAttribute
	kAXDefaultButtonAttribute
	kAXCancelButtonAttribute
	
	// menu or menu item-specific attributes
	kAXMenuItemCmdCharAttribute
	kAXMenuItemCmdVirtualKeyAttribute
	kAXMenuItemCmdGlyphAttribute
	kAXMenuItemCmdModifiersAttribute
	kAXMenuItemMarkCharAttribute
	kAXMenuItemPrimaryUIElementAttribute
	
	// application element-specific attributes
	kAXMenuBarAttribute
	kAXWindowsAttribute
	kAXFrontmostAttribute
	kAXHiddenAttribute
	kAXMainWindowAttribute
	kAXFocusedWindowAttribute
	kAXFocusedUIElementAttribute
	kAXExtrasMenuBarAttribute
 
	// date/time-specific attributes
	kAXHourFieldAttribute
	kAXMinuteFieldAttribute
	kAXSecondFieldAttribute
	kAXAMPMFieldAttribute
	kAXDayFieldAttribute
	kAXMonthFieldAttribute
	kAXYearFieldAttribute
	
	// table, outline, or browser-specific attributes
	kAXRowsAttribute
	kAXVisibleRowsAttribute
	kAXSelectedRowsAttribute
	kAXColumnsAttribute
	kAXVisibleColumnsAttribute
	kAXSelectedColumnsAttribute
	kAXSortDirectionAttribute
	kAXColumnHeaderUIElementsAttribute
	kAXIndexAttribute
	kAXDisclosingAttribute
	kAXDisclosedRowsAttribute
	kAXDisclosedByRowAttribute
	
	// matte-specific attributes
	kAXMatteHoleAttribute
	kAXMatteContentUIElementAttribute
	
	// ruler-specific attributes
	kAXMarkerUIElementsAttribute
	kAXUnitsAttribute
	kAXUnitDescriptionAttribute
	kAXMarkerTypeAttribute
	kAXMarkerTypeDescriptionAttribute
	
	// miscellaneous or role-specific attributes
	kAXHorizontalScrollBarAttribute
	kAXVerticalScrollBarAttribute
	kAXOrientationAttribute
	kAXHeaderAttribute
	kAXEditedAttribute
	kAXTabsAttribute
	kAXOverflowButtonAttribute
	kAXFilenameAttribute
	kAXExpandedAttribute
	kAXSelectedAttribute
	kAXSplittersAttribute
	kAXContentsAttribute
	kAXNextContentsAttribute
	kAXPreviousContentsAttribute
	kAXDocumentAttribute
	kAXIncrementorAttribute
	kAXDecrementButtonAttribute
	kAXIncrementButtonAttribute
	kAXColumnTitleAttribute
	kAXURLAttribute
	kAXLabelUIElementsAttribute
	kAXLabelValueAttribute
	kAXShownMenuUIElementAttribute
	kAXIsApplicationRunningAttribute
	kAXFocusedApplicationAttribute
 	kAXElementBusyAttribute
	kAXAlternateUIVisibleAttribute
*/

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*! @group Informational Attributes                                                     */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*!	
	@defined kAXRoleAttribute

	@abstract
	Identifies the basic type of an element.
	
	@attributeblock Value
	A CFStringRef of one of the role strings defined in this header, or a new
	role string of your own invention. The string should not be localized, and it does
	not need to be human-readable. Instead of inventing new role strings, see if a
	custom element can be identified by an existing role string and a new subrole. See
	kAXSubroleAttribute.
	
	@attributeblock Writable
	No
	
	@discussion
	Required for all elements. Even in the worst case scenario where an element cannot
	figure out what its basic type is, it can still supply the value kAXUnknownRole.
	
	@attributeblock Carbon Accessorization Notes
	If your HIObjectClass or Carbon Event handler provides
	the kAXRoleAttribute, it must also provide the kAXRoleDescriptionAttribute.
*/
#define kAXRoleAttribute				CFSTR("AXRole")


/*!
	@defined kAXSubroleAttribute
	
	@abstract
	More specifically identifies the type of an element beyond the basic type provided
	by kAXRoleAttribute.
	
	@attributeblock Value
	A CFStringRef of one of the subrole strings defined in this header, or a new
	subrole string of your own invention. The string should not be localized, and it does
	not need to be human-readable.
	
	@attributeblock Writable
	No
	
	@discussion
	Required only when an element's kAXRoleAttribute alone doesn't provide an assistive
	application with enough information to convey the meaning of this element to the user.
	
	An example element that offers the kAXSubroleAttribute is a window's close box. Its
	kAXRoleAttribute value is kAXButtonRole and its kAXSubroleAttribute is
	kAXCloseButtonSubrole. It is of role kAXButtonRole because it offers no additional
	actions or attributes above and beyond what other kAXButtonRole elements provide; it
	was given a subrole in order to allow an assistive app to communicate the close box's
	semantic difference to the user.
	
	@attributeblock Carbon Accessorization Notes
	If your HIObjectClass or Carbon Event handler provides
	the kAXSubroleAttribute, it must also provide the kAXRoleDescriptionAttribute.
*/
#define kAXSubroleAttribute				CFSTR("AXSubrole")


/*!
	@defined kAXRoleDescriptionAttribute
	
	@discussion
	A localized, human-readable string that an assistive application can present to the user
	as an explanation of an element's basic type or purpose. Examples would be "push button"
	or "secure text field". The string's language should match the language of the app that
	the element lives within. The string should be all lower-case and contain no punctuation.
	
	Two elements with the same kAXRoleAttribute and kAXSubroleAttribute should have the
	same kAXRoleDescriptionAttribute.
	
	@attributeblock Value
	A localized, human-readable CFStringRef
	
	@attributeblock Writable
	No
	
	@abstract Required for all elements. Even in the worst case scenario where an element cannot
	figure out what its basic type is, it can still supply the value "unknown".
	
	@attributeblock Carbon Accessorization Notes
	The HIObjectClass or Carbon Event handler that provides
	the AXRole and/or AXSubrole for an element is the one that must also handle the
	AXRoleDescription attribute. If an HIObjectClass or Carbon Event handler does not
	provide either the AXRole or AXSubrole, it must not provide the AXRoleDescription.
*/
#define kAXRoleDescriptionAttribute			CFSTR("AXRoleDescription")

/*!
	@define kAXHelpAttribute
	@abstract A localized, human-readable CFStringRef that offers help content for an element.
	@discussion
	This is often the same information that would be provided in a help tag for the element.
	
	Recommended for any element that has help data available.
	
	@attributeblock Value
	A localized, human-readable CFStringRef.
	
	@attributeblock Writable
	No.
*/
#define kAXHelpAttribute				CFSTR("AXHelp")

/*!
	@defined kAXTitleAttribute

	@discussion
	The localized, human-readable string that is displayed as part of the element's
	normal visual interface. For example, an OK button's kAXTitleElement is the string
	"OK", and a menu item's kAXTitleElement is the text of the menu item.
	
	Required if the element draws a string as part of its normal visual interface.
	
	@attributeblock Value
	A localized, human-readable CFStringRef
	
	@attributeblock Writable
	No
	
*/
#define kAXTitleAttribute				CFSTR("AXTitle")

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*! @group Value Attributes                                                             */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*!
	@defined kAXValueAttribute

	@discussion
	A catch-all attribute that represents a user modifiable setting of an element. For
	example, the contents of an editable text field, the position of a scroll bar thumb,
	and whether a check box is checked are all communicated by the kAXValueAttribute of
	their respective elements.
	
	Required for many user manipulatable elements, or those whose value state conveys
	important information.
	
	@attributeblock Value
	Varies, but will always be the same type for a given kind of element. Each
	role that offers kAXValueAttribute will specify the type of data that will be used
	for its value.
	
	@attributeblock Writable
	Generally yes. However, it does not need to be writable if some other form
	of direct manipulation is more appropriate for causing a value change. For example,
	a kAXScrollBar's kAXValueAttribute is writable because it allows an efficient way
	for the user to get to a specific position in the element being scrolled. By
	contrast, a kAXCheckBox's kAXValueAttribute is not settable because underlying
	functionality of the check box widget relies on it being clicked on; therefore, it
	changes its own kAXValueAttribute appropriately in response to the kAXPressAction.
	
	Required for many user manipulatable elements, or those whose value state conveys
	important information.
*/
#define kAXValueAttribute				CFSTR("AXValue")


/*!
    @define kAXValueDescriptionAttribute
    
    Used to supplement kAXValueAttribute.  This attribute returns a string description that best 
    describes the current value stored in kAXValueAttribute.  This is useful for things like
    slider where the numeric value in kAXValueAttribute does not always convey enough information
    about the adjustment made on the slider.  As an example, a color slider that adjusts thru various  
    colors cannot be well-described by the numeric value in existing AXValueAttribute.  This is where 
    the kAXValueDescriptionAttribute comes in handy.  In this example, the developer can provide the  
    color information using this attribute.       
    
    Value: A localized, human-readable CFStringRef.
	
	Writable? No.
    
    Recommended for elements that support kAXValueAttribute.

*/
#define kAXValueDescriptionAttribute    CFSTR("AXValueDescription")


/*!
	@define kAXMinValueAttribute
	
	Only used in conjunction with kAXValueAttribute and kAXMaxValueAttribute, this
	attribute represents the minimum value that an element can display. This is useful
	for things like sliders and scroll bars, where the user needs to have an understanding
	of how much the kAXValueAttribute can vary.
	
	Value: Same data type as the element's kAXValueAttribute.
	
	Writable? No.
	
	Required for many user maniipulatable elements. See kAXValueAttribute for more
	details.
*/
#define kAXMinValueAttribute				CFSTR("AXMinValue")


/*!
	@define kAXMaxValueAttribute
	
	Only used in conjunction with kAXValueAttribute and kAXMinValueAttribute, this
	attribute represents the maximum value that an element can display. This is useful
	for things like sliders and scroll bars, where the user needs to have an understanding
	of how much the kAXValueAttribute can vary.
	
	Value: Same data type as the element's kAXValueAttribute.
	
	Writable? No.
	
	Required for many user maniipulatable elements. See kAXValueAttribute for more
	details.
*/
#define kAXMaxValueAttribute				CFSTR("AXMaxValue")


/*!
	@define kAXValueIncrementAttribute
	
	Only used in conjunction with kAXValueAttribute, this attribute represents the amount
	a value will change in one action on the given element. In particular, it is used on
	elements of role kAXIncrementorRole in order to give the user an idea of how much its
	value will change with a single click on the up or down arrow.
	
	Value: Same data type as the element's kAXValueAttribute.
	
	Writable? No.
	
	Recommended for kAXIncrementorRole and other similar elements.
*/
#define kAXValueIncrementAttribute			CFSTR("AXValueIncrement")


/*!
	An array of the allowed values for a slider or other widget that displays
	a large value range, but which can only be set to a small subset of values
	within that range.
	
	Value: A CFArrayRef of whatever type the element uses for its kAXValueAttribute.
	
	Writable? No.
	
	Recommended for sliders or other elements that can only be set to a small
	set of values.
*/
#define kAXAllowedValuesAttribute			CFSTR("AXAllowedValues")


/*
	kAXPlaceholderValueAttribute
	
	The value of placeholder text as found in a text field.
	
	Value: A CFStringRef.
	
	Writable? No.
	
	Recommended for text fields and other elements that have a placeholder value.
*/
#define kAXPlaceholderValueAttribute			CFSTR("AXPlaceholderValue")


/*!
	@define kAXEnabledAttribute
	
	Indicates whether the element can be interacted with by the user. For example,
	a disabled push button's kAXEnabledAttribute will be false.
	
	Value: A CFBooleanRef. True means enabled, false means disabled.
	
	Writable? No.
	
	Required for all views, menus, and menu items. Not required for windows.
*/
#define kAXEnabledAttribute				CFSTR("AXEnabled")


/*!
    @define kAXElementBusyAttribute
 
    Indicates that an element is busy. This status conveys
    that an element is in the process of updating or loading and that
    new information will be available when the busy state completes.
 
    Value: A CFBooleanRef. True means busy, false means not busy.
 
    Writable? Yes.
 */
#define kAXElementBusyAttribute				CFSTR("AXElementBusy")


/*!
	@define kAXFocusedAttribute
	
	Indicates whether the element is the current keyboard focus. It should be writable
	for any element that can accept keyboard focus, though you can only set the value
	of kAXFocusedAttribute to true. You cannot unfocus an element by setting the value
	to false. Only one element in a window's entire accessibility hierarchy should be
	marked as focused.
	
	Value: A CFBooleanRef. True means focused, false means not focused.
	
	Writable? Yes, for any focusable element. No in all other cases.
	
	Required for any focusable element. Not required for other elements, though it is
	often offered for non-focusable elements in a read-only fashion.
*/
#define kAXFocusedAttribute				CFSTR("AXFocused")


/*!
	@define kAXParentAttribute
	
	Indicates the element's container element in the visual element hierarchy. A push
	button's kAXParentElement might be a window element or a group. A sheet's
	kAXParentElement will be a window element. A window's kAXParentElement will be the
	application element. A menu item's kAXParentElement will be a menu element.
	
	Value: An AXUIElementRef.
	
	Writable? No.
	
	Required for every element except the application. Everything else in the visual
	element hierarchy must have a parent.
*/
#define kAXParentAttribute				CFSTR("AXParent")


/*!
	@define kAXChildrenAttribute
	
	Indicates the sub elements of a given element in the visual element hierarchy. A tab
	group's kAXChildrenAttribute is an array of tab radio button elements. A window's
	kAXChildrenAttribute is an array of the first-order views elements within the window.
	A menu's kAXChildrenAttribute is an array of the menu item elements.
	
	A given element may only be in the child array of one other element. If an element is
	in the child array of some other element, the element's kAXParentAttribute must be
	the other element.
	
	Value: A CFArrayRef of AXUIElementRefs.
	
	Writable? No.
	
	Required for elements that contain sub elements.
*/
#define kAXChildrenAttribute				CFSTR("AXChildren")


/*!
	@define kAXSelectedChildrenAttribute
	
	Indicates the selected sub elements of a given element in the visual element hierarchy.
	This is a the subset of the element's kAXChildrenAttribute that are selected. This is
	commonly used in lists so an assistive app can know which list item are selected.
	
	Value: A CFArrayRef of AXUIElementRefs.
	
	Writable? Only if there is no other way to manipulate the set of selected elements via
	accessibilty attributes or actions. Even if other ways exist, this attribute can be
	writable as a convenience to assistive applications and their users. If
	kAXSelectedChildrenAttribute is writable, a write request with a value of an empty
	array should deselect all selected children.
	
	Required for elements that contain selectable sub elements.
*/
#define kAXSelectedChildrenAttribute			CFSTR("AXSelectedChildren")


/*!
	@define kAXVisibleChildrenAttribute
	
	Indicates the visible sub elements of a given element in the visual element hierarchy.
	This is a the subset of the element's kAXChildrenAttribute that a sighted user can
	see on the screen. In a list element, kAXVisibleChildrenAttribute would be an array
	of child elements that are currently scrolled into view.
	
	Value: A CFArrayRef of AXUIElementRefs.
	
	Writable? No.
	
	Recommended for elements whose child elements can be occluded or scrolled out of view.
*/
#define kAXVisibleChildrenAttribute			CFSTR("AXVisibleChildren")


/*!
	@define kAXWindowAttribute
	
	A short cut for traversing an element's parent hierarchy until an element of role
	kAXWindowRole is found. Note that the value for kAXWindowAttribute should not be
	an element of role kAXSheetRole or kAXDrawerRole; instead, the value should be the
	element of kAXWindowRole that the sheet or drawer is attached to.
	
	Value: an AXUIElementRef of role kAXWindowRole.
	
	Writable? No.
	
	Required for any element that has an element of role kAXWindowRole somewhere
	in its parent chain.
*/
#define kAXWindowAttribute				CFSTR("AXWindow")


/*!
	@define kAXTopLevelUIElementAttribute
	
	This is very much like the kAXWindowAttribute, except that the value of this
	attribute can be an element with role kAXSheetRole or kAXDrawerRole. It is
	a short cut for traversing an element's parent hierarchy until an element of
	role kAXWindowRole, kAXSheetRole, or kAXDrawerRole is found.
	
	Value: An AXUIElementRef of role kAXWindowRole, kAXSheetRole, or kAXDrawerRole.
	
	Writable? No.
	
	Required for any element that has an appropriate element somewhere in its
	parent chain.
*/
#define kAXTopLevelUIElementAttribute			CFSTR("AXTopLevelUIElement")


/*!
	@define kAXPositionAttribute
	
	The global screen position of the top-left corner of an element.
	
	Value: An AXValueRef with type kAXValueCGPointType. 0,0 is the top-left
	corner of the screen that displays the menu bar. The value of the horizontal
	axis increases to the right. The value of the vertical axis increases
	downward. Units are points.
	
	Writable? Generally no. However, some elements that can be moved by the user
	through direct manipulation (like windows) should offer a writable position
	attribute.
	
	Required for all elements that are visible on the screen, which is virtually
	all elements.
*/
#define kAXPositionAttribute				CFSTR("AXPosition")


/*!
	@define kAXSizeAttribute
	
	The vertical and horizontal dimensions of the element.
	
	Value: An AXValueRef with type kAXValueCGSizeType. Units are points.
	
	Writable? Generally no. However, some elements that can be resized by the user
	through direct manipulation (like windows) should offer a writable size attribute.
	
	Required for all elements that are visible on the screen, which is virtually
	all elements.
*/
#define kAXSizeAttribute				CFSTR("AXSize")


/*!
	@define kAXOrientationAttribute
	
	An indication of whether an element is drawn and/or interacted with in a
	vertical or horizontal manner. Elements such as scroll bars and sliders offer
	the kAXOrientationAttribute.
	
	Value: kAXHorizontalOrientationValue, kAXVerticalOrientationValue, or rarely
	kAXUnknownOrientationValue.
	
	Writable? No.
	
	Required for scroll bars, sliders, or other elements whose semantic or
	associative meaning changes based on their orientation.
*/
#define kAXOrientationAttribute				CFSTR("AXOrientation")


/*!
	@define kAXDescriptionAttribute
	
	A localized, human-readable string that indicates an element's purpose in a way
	that is slightly more specific than the kAXRoleDescriptionAttribute, but which
	is less wordy than the kAXHelpAttribute. Typically, the description should be
	an adjective or short phrase that describes the element's usage. For example,
	the description of a slider in a font panel might be "font size". The string
	should be all lower-case and contain no punctuation.
	
	Value: A localized, human-readable CFStringRef.
	
	Writable? No.
	
	Recommended for all elements because it gives the user a concise indication of
	an element's purpose.
*/
#define kAXDescriptionAttribute			CFSTR("AXDescription")
#define kAXDescription					CFSTR("AXDescription") // old name


/*!
	@define kAXSelectedTextAttribute
	
	The selected text of an editable text element.
	
	Value: A CFStringRef with the currently selected text of the element.
	
	Writable? No.
	
	Required for all editable text elements.
*/
#define kAXSelectedTextAttribute			CFSTR("AXSelectedText")


/*!
	@define kAXSelectedTextRangeAttribute
	
	The range of characters (not bytes) that defines the current selection of an
	editable text element.
	
	Value: An AXValueRef of type kAXValueCFRange.
	
	Writable? Yes.
	
	Required for all editable text elements.
*/
#define kAXSelectedTextRangeAttribute			CFSTR("AXSelectedTextRange")

/*!
	@define kAXSelectedTextRangesAttribute
	
	An array of noncontiguous ranges of characters (not bytes) that defines the current selections of an
	editable text element.  
	
	Value: A CFArrayRef of kAXValueCFRanges.
	
	Writable? Yes.
	
	Recommended for text elements that support noncontiguous selections.
*/
#define kAXSelectedTextRangesAttribute			CFSTR("AXSelectedTextRanges")


/*!
	@define kAXVisibleCharacterRangeAttribute
	
	The range of characters (not bytes) that are scrolled into view in the editable
	text element.
	
	Value: An AXValueRef of type kAXValueCFRange.
	
	Writable? No.
	
	Required for elements of role kAXTextAreaRole. Not required for any other
	elements, including those of role kAXTextFieldRole.
*/

#define kAXVisibleCharacterRangeAttribute		CFSTR("AXVisibleCharacterRange")


/*!
	@define kAXNumberOfCharactersAttribute
	
	The total number of characters (not bytes) in an editable text element.
	
	Value: CFNumberRef
	
	Writable? No.
	
	Required for editable text elements.
*/
#define kAXNumberOfCharactersAttribute			CFSTR("AXNumberOfCharacters")


/*!
	@define kAXSharedTextUIElementsAttribute
	
	Value: CFArrayRef of AXUIElementRefs
	
	Writable? No.
	
	Optional?
*/
#define kAXSharedTextUIElementsAttribute		CFSTR("AXSharedTextUIElements")


/*!
	@define kAXSharedCharacterRangeAttribute
	
	Value: AXValueRef of type kAXValueCFRangeType
	
	Writable? No.
	
	Optional?
*/
#define kAXSharedCharacterRangeAttribute		CFSTR("AXSharedCharacterRange")


/*!
    @define kAXSharedFocusElementsAttribute
 
    Returns an array of elements that also have keyboard focus when a given element has
    keyboard focus. A common usage of this attribute is to report that both a search
    text field and a list of resulting suggestions share keyboard focus because keyboard
    events can be handled by either UI element. In this example, the text field would be
    the first responder and it would report the list of suggestions as an element in the
    array returned for kAXSharedFocusElementsAttribute.
 
    Value: A CFArrayRef of AXUIElementsRefs.
 
    Writable? No.
 */
#define kAXSharedFocusElementsAttribute         CFSTR("AXSharedFocusElements")


/*!
	@define kAXInsertionPointLineNumberAttribute
 */
#define kAXInsertionPointLineNumberAttribute		CFSTR("AXInsertionPointLineNumber")


/*!
	@define kAXMainAttribute
	
	Whether a window is the main document window of an application. For an active
	app, the main window is the single active document window. For an inactive app,
	the main window is the single document window which would be active if the app
	were active. Main does not necessarily imply that the window has key focus.
	
	Value: A CFBooleanRef. True means the window is main. False means it is not.
	
	Writable? Yes.
	
	Required for all window elements.
*/
#define kAXMainAttribute				CFSTR("AXMain")


/*!
	@define kAXMinimizedAttribute
	
	Whether a window is currently minimized to the dock.
	
	Value: A CFBooleanRef. True means minimized.
	
	Writable? Yes.
	
	Required for all window elements that can be minimized.
*/
#define kAXMinimizedAttribute				CFSTR("AXMinimized")


/*!
	@define kAXCloseButtonAttribute
	
	A convenience attribute so assistive apps can quickly access a window's close
	button element.
	
	Value: An AXUIElementRef of the window's close button element.
	
	Writable? No.
	
	Required for all window elements that have a close button.
*/
#define kAXCloseButtonAttribute				CFSTR("AXCloseButton")


/*!
	@define kAXZoomButtonAttribute
	
	A convenience attribute so assistive apps can quickly access a window's zoom
	button element.
	
	Value: An AXUIElementRef of the window's zoom button element.
	
	Writable? No.
	
	Required for all window elements that have a zoom button.
*/
#define kAXZoomButtonAttribute				CFSTR("AXZoomButton")


/*!
	@define kAXMinimizeButtonAttribute
	
	A convenience attribute so assistive apps can quickly access a window's minimize
	button element.
	
	Value: An AXUIElementRef of the window's minimize button element.
	
	Writable? No.
	
	Required for all window elements that have a minimize button.
*/
#define kAXMinimizeButtonAttribute			CFSTR("AXMinimizeButton")


/*!
	@define kAXToolbarButtonAttribute
	
	A convenience attribute so assistive apps can quickly access a window's toolbar
	button element.
	
	Value: An AXUIElementRef of the window's toolbar button element.
	
	Writable? No.
	
	Required for all window elements that have a toolbar button.
*/
#define kAXToolbarButtonAttribute			CFSTR("AXToolbarButton")


/*
 kAXFullScreenButtonAttribute
 
 A convenience attribute so assistive apps can quickly access a window's full screen
 button element.
 
 Value: An AXUIElementRef of the window's full screen button element.
 
 Writable? No.
 
 Required for all window elements that have a full screen button.
 */
#define kAXFullScreenButtonAttribute			CFSTR("AXFullScreenButton")


/*!
	@define kAXProxyAttribute
	
	A convenience attribute so assistive apps can quickly access a window's document
	proxy element.
	
	Value: An AXUIElementRef of the window's document proxy element.
	
	Writable? No.
	
	Required for all window elements that have a document proxy.
*/
#define kAXProxyAttribute				CFSTR("AXProxy")


/*!
	@define kAXGrowAreaAttribute
	
	A convenience attribute so assistive apps can quickly access a window's grow
	area element.
	
	Value: An AXUIElementRef of the window's grow area element.
	
	Writable? No.
	
	Required for all window elements that have a grow area.
*/
#define kAXGrowAreaAttribute				CFSTR("AXGrowArea")


/*!
	@define kAXModalAttribute
	
	Whether a window is modal.
	
	Value: A CFBooleanRef. True means the window is modal.
	
	Writable? No.
	
	Required for all window elements.
*/
#define kAXModalAttribute				CFSTR("AXModal")


/*!
	@define kAXDefaultButtonAttribute
	
	A convenience attribute so assistive apps can quickly access a window's default
	button element, if any.
	
	Value: An AXUIElementRef of the window's default button element.
	
	Writable? No.
	
	Required for all window elements that have a default button.
*/
#define kAXDefaultButtonAttribute			CFSTR("AXDefaultButton")


/*!
	@define kAXCancelButtonAttribute
	
	A convenience attribute so assistive apps can quickly access a window's cancel
	button element, if any.
	
	Value: An AXUIElementRef of the window's cancel button element.
	
	Writable? No.
	
	Required for all window elements that have a cancel button.
*/
#define kAXCancelButtonAttribute			CFSTR("AXCancelButton")

// menu-specific attributes
#define kAXMenuItemCmdCharAttribute			CFSTR("AXMenuItemCmdChar")
#define kAXMenuItemCmdVirtualKeyAttribute		CFSTR("AXMenuItemCmdVirtualKey")
#define kAXMenuItemCmdGlyphAttribute			CFSTR("AXMenuItemCmdGlyph")
#define kAXMenuItemCmdModifiersAttribute		CFSTR("AXMenuItemCmdModifiers")
#define kAXMenuItemMarkCharAttribute			CFSTR("AXMenuItemMarkChar")
#define kAXMenuItemPrimaryUIElementAttribute		CFSTR("AXMenuItemPrimaryUIElement")

/*! @typedef AXMenuItemModifiers
      @abstract Values that indicate the keyboard shortcut modifiers for a menu item (used with the {@link kAXMenuItemCmdModifiersAttribute} attribute).
 */
typedef CF_OPTIONS(UInt32, AXMenuItemModifiers) {
    kAXMenuItemModifierNone         = 0,        /* Mask for no modifiers other than the command key (which is used by default) */
    kAXMenuItemModifierShift        = (1 << 0), /* Mask for shift key modifier */
    kAXMenuItemModifierOption       = (1 << 1), /* Mask for option key modifier */
    kAXMenuItemModifierControl      = (1 << 2), /* Mask for control key modifier */
    kAXMenuItemModifierNoCommand    = (1 << 3)  /* Mask for no modifiers at all, not even the command key */
};

// application-specific attributes
#define kAXMenuBarAttribute				CFSTR("AXMenuBar")
#define kAXWindowsAttribute				CFSTR("AXWindows")
#define kAXFrontmostAttribute				CFSTR("AXFrontmost")
#define kAXHiddenAttribute				CFSTR("AXHidden")
#define kAXMainWindowAttribute				CFSTR("AXMainWindow")
#define kAXFocusedWindowAttribute			CFSTR("AXFocusedWindow")
#define kAXFocusedUIElementAttribute			CFSTR("AXFocusedUIElement") 
#define kAXExtrasMenuBarAttribute			CFSTR("AXExtrasMenuBar")

/*!
	@define kAXHeaderAttribute
	
	A convenience attribute whose value is an element that is a header for another
	element. For example, an outline element has a header attribute whose value is
	a element of role AXGroup that contains the header buttons for each column.
	Used for things like tables, outlines, columns, etc.
	
	Value: An AXUIElementRef whose role varies.
	
	Writable? No.
	
	Recommended for elements that have header elements contained within them that an
	assistive application might want convenient access to.
*/
#define kAXHeaderAttribute				CFSTR("AXHeader")
#define kAXEditedAttribute				CFSTR("AXEdited")
#define kAXValueWrapsAttribute				CFSTR("AXValueWraps")
#define kAXTabsAttribute				CFSTR("AXTabs")
#define kAXTitleUIElementAttribute			CFSTR("AXTitleUIElement")
#define kAXHorizontalScrollBarAttribute			CFSTR("AXHorizontalScrollBar")
#define kAXVerticalScrollBarAttribute			CFSTR("AXVerticalScrollBar")
#define kAXOverflowButtonAttribute			CFSTR("AXOverflowButton")
#define kAXFilenameAttribute				CFSTR("AXFilename")
#define kAXExpandedAttribute				CFSTR("AXExpanded")
#define kAXSelectedAttribute				CFSTR("AXSelected")
#define kAXSplittersAttribute				CFSTR("AXSplitters")
#define kAXNextContentsAttribute			CFSTR("AXNextContents")
#define kAXDocumentAttribute				CFSTR("AXDocument")
#define kAXDecrementButtonAttribute			CFSTR("AXDecrementButton")
#define kAXIncrementButtonAttribute			CFSTR("AXIncrementButton")
#define kAXPreviousContentsAttribute			CFSTR("AXPreviousContents")


/*!
	@define kAXContentsAttribute
	
	A convenience attribute so assistive apps can find interesting child elements
	of a given element, while at the same time avoiding non-interesting child
	elements. For example, the contents of a scroll area are the children that get
	scrolled, and not the horizontal and/or vertical scroll bars. The contents of
	a tab group does not include the tabs themselves.
	
	Value: A CFArrayRef of AXUIElementRefs.
	
	Writable? No.
	
	Recommended for elements that have children that act upon or are separate from
	other children.
*/
#define kAXContentsAttribute				CFSTR("AXContents")


/*!
	@define kAXIncrementorAttribute
	
	Convenience attribute that yields the incrementor of a time field or date
	field element.
	
	Value: A AXUIElementRef of role kAXIncrementorRole.
	
	Writable? No.
	
	Required for time field and date field elements that display an incrementor.
*/
#define kAXIncrementorAttribute				CFSTR("AXIncrementor")


/*!
	@define kAXHourFieldAttribute
	
	Convenience attribute that yields the hour field of a time field element.
	
	Value: A AXUIElementRef of role kAXTextFieldRole that is used to edit the
	hours in a time field element.
	
	Writable? No.
	
	Required for time field elements that display hours.
*/
#define kAXHourFieldAttribute				CFSTR("AXHourField")


/*!
	@define kAXMinuteFieldAttribute
	
	Convenience attribute that yields the minute field of a time field element.
	
	Value: A AXUIElementRef of role kAXTextFieldRole that is used to edit the
	minutes in a time field element.
	
	Writable? No.
	
	Required for time field elements that display minutes.
*/
#define kAXMinuteFieldAttribute				CFSTR("AXMinuteField")


/*!
	@define kAXSecondFieldAttribute
	
	Convenience attribute that yields the seconds field of a time field element.
	
	Value: A AXUIElementRef of role kAXTextFieldRole that is used to edit the
	seconds in a time field element.
	
	Writable? No.
	
	Required for time field elements that display seconds.
*/
#define kAXSecondFieldAttribute				CFSTR("AXSecondField")


/*!
	@define kAXAMPMFieldAttribute
	
	Convenience attribute that yields the AM/PM field of a time field element.
	
	Value: A AXUIElementRef of role kAXTextFieldRole that is used to edit the
	AM/PM setting in a time field element.
	
	Writable? No.
	
	Required for time field elements that displays an AM/PM setting.
*/
#define kAXAMPMFieldAttribute				CFSTR("AXAMPMField")


/*!
	@define kAXDayFieldAttribute
	
	Convenience attribute that yields the day field of a date field element.
	
	Value: A AXUIElementRef of role kAXTextFieldRole that is used to edit the
	day in a date field element.
	
	Writable? No.
	
	Required for date field elements that display days.
*/
#define kAXDayFieldAttribute				CFSTR("AXDayField")


/*!
	@define kAXMonthFieldAttribute
	
	Convenience attribute that yields the month field of a date field element.
	
	Value: A AXUIElementRef of role kAXTextFieldRole that is used to edit the
	month in a date field element.
	
	Writable? No.
	
	Required for date field elements that display months.
*/
#define kAXMonthFieldAttribute				CFSTR("AXMonthField")


/*!
	@define kAXYearFieldAttribute
	
	Convenience attribute that yields the year field of a date field element.
	
	Value: A AXUIElementRef of role kAXTextFieldRole that is used to edit the
	year in a date field element.
	
	Writable? No.
	
	Required for date field elements that display years.
*/
#define kAXYearFieldAttribute				CFSTR("AXYearField")


/*!
	@define kAXColumnTitleAttribute
*/
#define kAXColumnTitleAttribute				CFSTR("AXColumnTitles")


/*!
	@define kAXURLAttribute
	
	Value: A CFURLRef.
	
	Writable? No.
	
	Required for elements that represent a disk or network item.
*/
#define kAXURLAttribute					CFSTR("AXURL")
#define kAXLabelUIElementsAttribute			CFSTR("AXLabelUIElements")
#define kAXLabelValueAttribute				CFSTR("AXLabelValue")
#define kAXShownMenuUIElementAttribute                  CFSTR("AXShownMenuUIElement")
#define kAXServesAsTitleForUIElementsAttribute          CFSTR("AXServesAsTitleForUIElements")
#define kAXLinkedUIElementsAttribute 			CFSTR("AXLinkedUIElements")

// table/outline view attributes
#define kAXRowsAttribute				CFSTR("AXRows")
#define kAXVisibleRowsAttribute				CFSTR("AXVisibleRows")
#define kAXSelectedRowsAttribute			CFSTR("AXSelectedRows")
#define kAXColumnsAttribute				CFSTR("AXColumns")


/*!
	@define kAXVisibleColumnsAttribute
	
	Indicates the visible column sub-elements of a kAXBrowserRole element.
	This is the subset of a browser's kAXColumnsAttribute where each column in the
	array is one that is currently scrolled into view within the browser. It does
	not include any columns that are currently scrolled out of view.
	
	Value: A CFArrayRef of AXUIElementRefs representing the columns of a browser.
	The columns will be grandchild elements of the browser, and will generally be
	of role kAXScrollArea.
	
	Writable? No.
	
	Required for all browser elements.
*/
#define kAXVisibleColumnsAttribute			CFSTR("AXVisibleColumns")
#define kAXSelectedColumnsAttribute			CFSTR("AXSelectedColumns")
#define kAXSortDirectionAttribute			CFSTR("AXSortDirection")

// row/column attributes
#define kAXIndexAttribute				CFSTR("AXIndex")

// outline attributes
#define kAXDisclosingAttribute				CFSTR("AXDisclosing")
#define kAXDisclosedRowsAttribute			CFSTR("AXDisclosedRows")
#define kAXDisclosedByRowAttribute			CFSTR("AXDisclosedByRow")
#define kAXDisclosureLevelAttribute			CFSTR("AXDisclosureLevel")

// matte attributes
#define kAXMatteHoleAttribute				CFSTR("AXMatteHole")
#define kAXMatteContentUIElementAttribute		CFSTR("AXMatteContentUIElement")

// ruler attributes
#define kAXMarkerUIElementsAttribute			CFSTR("AXMarkerUIElements")
#define kAXUnitsAttribute				CFSTR("AXUnits")
#define kAXUnitDescriptionAttribute			CFSTR("AXUnitDescription")
#define kAXMarkerTypeAttribute				CFSTR("AXMarkerType")
#define kAXMarkerTypeDescriptionAttribute		CFSTR("AXMarkerTypeDescription")

// Dock attributes
#define kAXIsApplicationRunningAttribute		CFSTR("AXIsApplicationRunning")

// search field attributes
#define kAXSearchButtonAttribute			CFSTR("AXSearchButton")
#define kAXClearButtonAttribute				CFSTR("AXClearButton")

// system-wide attributes
#define kAXFocusedApplicationAttribute			CFSTR("AXFocusedApplication")

// grid attributes
#define kAXRowCountAttribute				CFSTR("AXRowCount")
#define kAXColumnCountAttribute				CFSTR("AXColumnCount")
#define kAXOrderedByRowAttribute			CFSTR("AXOrderedByRow")

// level indicator attributes
#define kAXWarningValueAttribute			CFSTR("AXWarningValue")
#define kAXCriticalValueAttribute			CFSTR("AXCriticalValue")

// cell-based table attributes
#define kAXSelectedCellsAttribute			CFSTR("AXSelectedCells")
#define kAXVisibleCellsAttribute			CFSTR("AXVisibleCells")
#define kAXRowHeaderUIElementsAttribute			CFSTR("AXRowHeaderUIElements")
#define kAXColumnHeaderUIElementsAttribute		CFSTR("AXColumnHeaderUIElements")

// cell attributes
#define kAXRowIndexRangeAttribute			CFSTR("AXRowIndexRange")
#define kAXColumnIndexRangeAttribute			CFSTR("AXColumnIndexRange")

// layout area attributes
#define kAXHorizontalUnitsAttribute			CFSTR("AXHorizontalUnits")
#define kAXVerticalUnitsAttribute			CFSTR("AXVerticalUnits")
#define kAXHorizontalUnitDescriptionAttribute		CFSTR("AXHorizontalUnitDescription")
#define kAXVerticalUnitDescriptionAttribute		CFSTR("AXVerticalUnitDescription")
#define kAXHandlesAttribute				CFSTR("AXHandles")

// obsolete/unknown attributes
#define kAXTextAttribute				CFSTR("AXText")
#define kAXVisibleTextAttribute				CFSTR("AXVisibleText")
#define kAXIsEditableAttribute				CFSTR("AXIsEditable")
#define kAXColumnTitlesAttribute			CFSTR("AXColumnTitles")

// UI element identification attributes
#define kAXIdentifierAttribute				CFSTR("AXIdentifier")

// UI supporting transient state attributes
#define kAXAlternateUIVisibleAttribute      CFSTR("AXAlternateUIVisible")

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*! @group Parameterized Attributes                                                     */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

// Text Suite Parameterized Attributes
#define kAXLineForIndexParameterizedAttribute		CFSTR("AXLineForIndex")
#define kAXRangeForLineParameterizedAttribute          	CFSTR("AXRangeForLine")
#define kAXStringForRangeParameterizedAttribute        	CFSTR("AXStringForRange")
#define kAXRangeForPositionParameterizedAttribute	CFSTR("AXRangeForPosition")
#define kAXRangeForIndexParameterizedAttribute 		CFSTR("AXRangeForIndex")
#define kAXBoundsForRangeParameterizedAttribute		CFSTR("AXBoundsForRange")
#define kAXRTFForRangeParameterizedAttribute		CFSTR("AXRTFForRange")
#define kAXAttributedStringForRangeParameterizedAttribute CFSTR("AXAttributedStringForRange")
#define kAXStyleRangeForIndexParameterizedAttribute	CFSTR("AXStyleRangeForIndex")

// cell-based table parameterized attributes
#define kAXCellForColumnAndRowParameterizedAttribute		CFSTR("AXCellForColumnAndRow")

// layout area parameterized attributes
#define kAXLayoutPointForScreenPointParameterizedAttribute	CFSTR("AXLayoutPointForScreenPoint")
#define kAXLayoutSizeForScreenSizeParameterizedAttribute	CFSTR("AXLayoutSizeForScreenSize")
#define kAXScreenPointForLayoutPointParameterizedAttribute	CFSTR("AXScreenPointForLayoutPoint")
#define kAXScreenSizeForLayoutSizeParameterizedAttribute	CFSTR("AXScreenSizeForLayoutSize")

#endif // __AXATTRIBUTECONSTANTS__

