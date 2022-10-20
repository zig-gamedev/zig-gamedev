/*
 *  AXActionConstants.h
 *  HIServices
 *
 *  Copyright (c) 2004, 2012 Apple Inc. All rights reserved.
 *
 */


#ifndef __AXACTIONCONSTANTS__
#define __AXACTIONCONSTANTS__


/*!
	@header
	
	@discussion
	Many UIElements have a set of actions that they can perform. Actions are designed to be
	simple. Actions roughly correspond to things you could do with a single click of the mouse
	on the UIElement. Buttons and menu items, for example, have a single action: push or pick,
	respectively. A scroll bar has several actions: page up, page down, up one line, down one line.
*/

/*
	TBD: Explain general philosophy on whether to expose an action or not.
	Our initial philopsophy is to not to have an action where there is a menu item
	or button that does the same thing.
*/

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*! @group Standard Actions             												*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*!
	@defined kAXPressAction
	
	@discussion
	Simulate clicking the UIElement, such as a button.
	
*/
#define kAXPressAction					CFSTR("AXPress")


/*!
	@defined kAXIncrementAction
	
	@discussion
	Increment the value of the UIElement.
*/
#define kAXIncrementAction				CFSTR("AXIncrement")

/*!
	@defined kAXDecrementAction
	
	@discussion
	Decrement the value of the UIElement.
*/
#define kAXDecrementAction				CFSTR("AXDecrement")

/* 	--Don't know if this is still correct. Is this what used to be kAXAcceptAction?-- */
/*!
	@defined kAXConfirmAction
	
	@discussion
	Simulate pressing Return in the UIElement, such as a text field.
*/
#define kAXConfirmAction				CFSTR("AXConfirm")

/*!
	@defined kAXCancelAction
	
	@discussion
	Simulate a Cancel action, such as hitting the Cancel button.
*/
#define kAXCancelAction					CFSTR("AXCancel")

/*!
    @defined kAXShowAlternateUIAction
 
    @discussion
    Show alternate or hidden UI.
    This is often used to trigger the same change that would occur on a mouse hover.
 */
#define kAXShowAlternateUIAction	CFSTR("AXShowAlternateUI")

/*!
    @defined kAXShowDefaultUIAction
    @discussion
    Show default UI.
    This is often used to trigger the same change that would occur when a mouse hover ends.
 */
#define kAXShowDefaultUIAction          CFSTR("AXShowDefaultUI")

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*! @group New Actions             										        		*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/* Need discussion for following */

/*!
	@defined kAXRaiseAction
        
        @discussion
	
*/
#define kAXRaiseAction					CFSTR("AXRaise")

/* Need discussion for following */

/*!
	@defined kAXShowMenuAction
        @discussion

*/
#define kAXShowMenuAction				CFSTR("AXShowMenu")

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*! @group Obsolete Actions																*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*!
	@defined kAXPickAction
	
	@discussion
	Select the UIElement, such as a menu item.
*/
#define kAXPickAction					CFSTR("AXPick")


#endif // __AXACTIONCONSTANTS__

