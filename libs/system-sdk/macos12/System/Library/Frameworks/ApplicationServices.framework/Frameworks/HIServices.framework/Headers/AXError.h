/*
 *  AXError.h
 *
 *  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 */

#ifndef __AXERROR__
#define __AXERROR__

#ifdef __cplusplus
extern "C" {
#endif

#include <CoreFoundation/CoreFoundation.h>

/*!
	@header AXError.h
	
	@discussion
	These  error codes can be returned from the accessibility functions defined in AXUIElement.h.
*/


/*!
	@enum Accessibility Error Codes
	@abstract Error codes returned by accessibility functions.
	@discussion
    
*/

typedef CF_ENUM(SInt32, AXError) {
    /*! No error occurred. */
    kAXErrorSuccess 				= 0,

    /*! A system error occurred, such as the failure to allocate an object. */
    kAXErrorFailure				= -25200,

    /*! An illegal argument was passed to the function. */
    kAXErrorIllegalArgument			= -25201,

    /*! The AXUIElementRef passed to the function is invalid. */
    kAXErrorInvalidUIElement			= -25202,

    /*! The AXObserverRef passed to the function is not a valid observer. */
    kAXErrorInvalidUIElementObserver		= -25203,

    /*! The function cannot complete because messaging failed in some way or because the application with which the function is communicating is busy or unresponsive. */
    kAXErrorCannotComplete			= -25204,

    /*! The attribute is not supported by the AXUIElementRef. */
    kAXErrorAttributeUnsupported		= -25205,

    /*! The action is not supported by the AXUIElementRef. */
    kAXErrorActionUnsupported			= -25206,

    /*! The notification is not supported by the AXUIElementRef. */
    kAXErrorNotificationUnsupported		= -25207,

    /*! Indicates that the function or method is not implemented (this can be returned if a process does not support the accessibility API). */
    kAXErrorNotImplemented			= -25208,

    /*! This notification has already been registered for. */
    kAXErrorNotificationAlreadyRegistered	= -25209,

    /*! Indicates that a notification is not registered yet. */
    kAXErrorNotificationNotRegistered		= -25210,

    /*! The accessibility API is disabled (as when, for example, the user deselects "Enable access for assistive devices" in Universal Access Preferences). */
    kAXErrorAPIDisabled				= -25211,

    /*! The requested value or AXUIElementRef does not exist. */
    kAXErrorNoValue				= -25212,

    /*! The parameterized attribute is not supported by the AXUIElementRef. */
    kAXErrorParameterizedAttributeUnsupported	= -25213,

    /*! Not enough precision. */
	kAXErrorNotEnoughPrecision	= -25214
};


#ifdef __cplusplus
}
#endif

#endif // __AXERROR__
