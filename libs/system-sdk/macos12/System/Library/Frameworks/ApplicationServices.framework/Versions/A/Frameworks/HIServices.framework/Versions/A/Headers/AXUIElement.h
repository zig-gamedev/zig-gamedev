/*
 *  AXUIElement.h
 *
 *  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 */

#ifndef __AXUIELEMENT__
#define __AXUIELEMENT__

#include <AvailabilityMacros.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGRemoteOperation.h>
#include <ApplicationServices/ApplicationServices.h>
CF_ASSUME_NONNULL_BEGIN
CF_IMPLICIT_BRIDGING_ENABLED
    
/*! @header AXUIElement.h
 @discussion
 Assistive applications use the functions defined in this header file to communicate with and control accessible applications running in Mac OS X.
 
 Each accessible user interface element in an application is represented by an AXUIElementRef, which is a CFTypeRef. AXUIElementRefs (like all
 CFTypeRefs) can be used with all the Core Foundation polymorphic functions, such as @link //apple_ref/c/func/CFRetain CFRetain@/link, @link //apple_ref/c/func/CFRelease CFRelease@/link, and @link //apple_ref/c/func/CFEqual CFEqual@/link.
 
 All functions defined in this header file will return <code>kAXErrorSuccess</code> on success if the return type is AXError.
 If there is some sort of system memory failure, such as the failure to allocate
 an object, all functions of return type AXError can return <code>kAXErrorFailure</code>. In the unlikely event that some process does not fully support the accessibility API, a function of return type AXError can
 return <code>kAXErrorNotImplemented</code>.
 
 In addition, some functions of return type AXError can return the following error codes:
 <dl><dt><code>kAXErrorInvalidUIElement</code></dt><dd>The passed-in AXUIElementRef is invalid. All functions that include an AXUIElementRef parameter can return this error code.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>At least one of the arguments is illegal (for example, NIL passed in for a pointer).</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>There is a problem with messaging (such as when messaging to the server fails or when the accessible application is unresponsive
 or waiting for user input). All functions that perform messaging can return this error code.</dd>
 <dt><code>kAXErrorAPIDisabled</code></dt><dd>The accessibility API is disabled. All functions that perform messaging can return this error code.</dd>
 </dl>
 
 For more information on the definition and use of accessibility objects and on Mac OS X accessibility support in general, see {@linkdoc //apple_ref/doc/uid/TP40001078 Accessibility Overview}.
 */

/*!
 @function AXAPIEnabled
 @abstract Returns whether the accessibility API is enabled.
 @deprecated  In OS X 10.9 and later. Use AXIsProcessTrustedWithOptions instead.
 @result Returns TRUE if the accessibility API is currently enabled, otherwise FALSE.
 */
extern Boolean AXAPIEnabled (void) CF_DEPRECATED_MAC(10_0, 10_9);

/*!
 @function AXIsProcessTrustedWithOptions
 @abstract Returns whether the current process is a trusted accessibility client.
 @param options A dictionary of options, or NULL to specify no options. The following options are available:
     
 KEY: kAXTrustedCheckOptionPrompt
 VALUE: ACFBooleanRef indicating whether the user will be informed if the current process is untrusted. This could be used, for example, on application startup to always warn a user if accessibility is not enabled for the current process. Prompting occurs asynchronously and does not affect the return value.
     
 @result Returns TRUE if the current process is a trusted accessibility client, FALSE if it is not.
 */
extern Boolean AXIsProcessTrustedWithOptions (CFDictionaryRef __nullable options) CF_AVAILABLE_MAC(10_9);
    
extern CFStringRef kAXTrustedCheckOptionPrompt CF_AVAILABLE_MAC(10_9);

/*!
 @function AXIsProcessTrusted
 @abstract Returns whether the current process is a trusted accessibility client.
 
 @result Returns TRUE if the current process is a trusted accessibility client, FALSE if it is not.
 */
extern Boolean AXIsProcessTrusted (void) CF_AVAILABLE_MAC(10_4);

/*!
 @function AXMakeProcessTrusted
 @abstract Attempts to make the process represented by the specified path a trusted accessibility client.
 
 @discussion Use this function to make a process a trusted accessibility client.  Note: The caller must be running as <code>root</code> to successfully call this function. In addition, the caller should relaunch the process after this function returns successfully for the trusted status to take effect.
 
 @param executablePath	The path to the executable of the process to make trusted.
 
 @result An AXError that indicates success or failure.
 */
extern AXError AXMakeProcessTrusted (CFStringRef executablePath) CF_DEPRECATED_MAC(10_4, 10_9);

/*!
 @typedef AXUIElementRef
 @abstract A structure used to refer to an accessibility object.
 
 @discussion An accessibility object provides information about the user interface object it represents. This information includes the object's
 position in the accessibility hierarchy, its position on the display, details about what it is, and what actions it can perform. Accessibility objects
 respond to messages sent by assistive applications and send notifications that describe state changes.
 */
typedef const struct CF_BRIDGED_TYPE(id) __AXUIElement *AXUIElementRef;

/*!
 @enum kAXCopyMultipleAttributeOptionStopOnError
 @discussion Pass this option to @link AXUIElementCopyMultipleAttributeValues AXUIElementCopyMultipleAttributeValues@/link to force the function
 to stop when it gets an error.
 */
typedef CF_OPTIONS(UInt32, AXCopyMultipleAttributeOptions)  {
    kAXCopyMultipleAttributeOptionStopOnError = 0x1
};

/*!
 @function AXUIElementGetTypeID
 @abstract Returns the unique type identifier for the AXUIElementRef type.
 
 @result Returns a CFTypeID representing the AXUIElementRef type.
 */
extern CFTypeID	AXUIElementGetTypeID (void);

/*!
 @function AXUIElementCopyAttributeNames
 @abstract Returns a list of all the attributes supported by the specified accessibility object.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param names On return, an array containing the accessibility object's attribute names.
 
 @result If unsuccessful, <code>AXUIElementCopyAttributeNames</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorAttributeUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified attribute.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or both of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorFailure</code></dt><dd>There was a system memory failure.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError 	AXUIElementCopyAttributeNames (AXUIElementRef element, CFArrayRef __nullable * __nonnull CF_RETURNS_RETAINED names);

/*!
 @function AXUIElementCopyAttributeValue
 @abstract Returns the value of an accessibility object's attribute.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param attribute The attribute name.
 @param value On return, the value associated with the specified attribute.
 
 @result If unsuccessful, <code>AXUIElementCopyAttributeValue</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorAttributeUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified attribute.</dd>
 <dt><code>kAXErrorNoValue</code></dt><dd>The specified attribute does not have a value.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError	AXUIElementCopyAttributeValue (AXUIElementRef element, CFStringRef attribute, CFTypeRef __nullable * __nonnull CF_RETURNS_RETAINED value);

/*!
 @function AXUIElementGetAttributeValueCount
 @abstract Returns the count of the array of an accessibility object's attribute value.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param attribute The attribute name.
 @param count On return, the size of the array that is the attribute's value.
 
 @result If unsuccessful, <code>AXUIElementGetAttributeValueCount</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>The attribute's value is not an array or one of the other arguments is an illegal value.</dd>
 <dt><code>kAXErrorAttributeUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified attribute.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError  AXUIElementGetAttributeValueCount (AXUIElementRef element, CFStringRef attribute, CFIndex *count);

/*!
 @function AXUIElementCopyAttributeValues
 @abstract Returns an array of attribute values for the accessibility object's attribute, starting at the specified index.
 @discussion This function is useful for dealing with large arrays, for example, a table view with a large number of children.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param attribute The attribute name.
 @param index The index into the array.
 @param maxValues The maximum number of values you want (this may be more or less than the number of values associated with the attribute).
 @param values On return, the attribute values you requested. If <code>maxValues</code> is greater than the number of values associated with the attribute, the <code>values</code> array will contain values found between <code>index</code> and the end of the attribute's array, inclusive.
 
 @result If unsuccessful, <code>AXUIElementCopyAttributeValues</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>The attribute's value is not array, the <code>index</code> or <code>maxValues</code> arguments are outside the array's range, or one of the other arguments is an illegal value.</dd>
 <dt><code>kAXErrorNoValue</code></dt><dd>The specified attribute does not have a value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError  AXUIElementCopyAttributeValues (AXUIElementRef element, CFStringRef attribute, CFIndex index, CFIndex maxValues, CFArrayRef __nullable * __nonnull CF_RETURNS_RETAINED values);

/*!
 @function AXUIElementIsAttributeSettable
 @abstract Returns whether the specified accessibility object's attribute can be modified.
 @discussion If you receive a <code>kAXErrorCannotComplete</code> error from this function, you might want to repeat the request or change the timeout value.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param attribute The attribute name.
 @param settable On return, a Boolean value indicating whether the attribute is settable.
 
 @result If unsuccessful, <code>AXUIElementIsAttributeSettable</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way (often due to a timeout).</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorAttributeUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified attribute.</dd>
 <dt><code>kAXErrorNoValue</code></dt><dd>The specified attribute does not have a value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError 	AXUIElementIsAttributeSettable (AXUIElementRef element, CFStringRef attribute, Boolean *settable);

/*!
 @function AXUIElementSetAttributeValue
 @abstract Sets the accessibility object's attribute to the specified value.
 @discussion You can send and receive many different CFTypeRefs using the accessibility API.
 These include all CFPropertyListRef types, AXUIElementRef, AXValueRef, AXTextMarkerRef, AXTextMarkerRangeRef,
 CFNullRef, CFAttributedStringRef, and CRURLRef.
 @param element The AXUIElementRef representing the accessibility object.
 @param attribute The attribute name.
 @param value The new value for the attribute.
 
 @result If unsuccessful, <code>AXUIElementSetAttributeValue</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>The value is not recognized by the accessible application or one of the other arguments is an illegal value.</dd>
 <dt><code>kAXErrorAttributeUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified attribute.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError 	AXUIElementSetAttributeValue (AXUIElementRef element, CFStringRef attribute, CFTypeRef value);

/*!
 @function AXUIElementCopyMultipleAttributeValues
 @abstract Returns the values of multiple attributes in the accessibility object.
 @discussion If the specified AXUIElementRef does not support an attribute passed in the <code>attributes</code> array, the returned array
 can contain an error or CFNull at the corresponding position.
 @param element The AXUIElementRef representing the accessibility object.
 @param attributes An array of attribute names.
 @param options A value that tells <code>AXUIElementCopyMultipleAttributeValues</code> how to handle errors.
 @param values On return, an array in which each position contains the value of the
 attribute that is in the corresponding position in the passed-in <code>attributes</code> array (or CFNull). If <code>options</code> = 0,
 the <code>values</code> array can contain an AXValueRef of type <code>kAXValueAXErrorType</code> in the corresponding position. If <code>options</code> = <code>kAXCopyMultipleAttributeOptionStopOnError</code>,
 this function will return immediately when it gets an error.
 
 @result If unsuccessful, <code>AXUIElementCopyMultipleAttributeValues</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>One of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError  AXUIElementCopyMultipleAttributeValues (AXUIElementRef element, CFArrayRef attributes, AXCopyMultipleAttributeOptions options, CFArrayRef __nullable * __nonnull CF_RETURNS_RETAINED values) CF_AVAILABLE_MAC(10_4);

/*!
 @function AXUIElementCopyParameterizedAttributeNames
 @abstract Returns a list of all the parameterized attributes supported by the specified accessibility object.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param names On return, an array containing the accessibility object's parameterized attribute names.
 
 @result If unsuccessful, <code>AXUIElementCopyParameterizedAttributeNames</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorAttributeUnsupported</code> or <code>kAXErrorParameterizedAttributeUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified parameterized attribute.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or both of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorFailure</code></dt><dd>There was some sort of system memory failure.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError	AXUIElementCopyParameterizedAttributeNames (AXUIElementRef element, CFArrayRef __nullable * __nonnull CF_RETURNS_RETAINED names) CF_AVAILABLE_MAC(10_3);

/*!
 @function AXUIElementCopyParameterizedAttributeValue
 @abstract Returns the value of an accessibility object's parameterized attribute.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param parameterizedAttribute The parameterized attribute.
 @param parameter The parameter.
 @param result On return, the value of the parameterized attribute.
 
 @result If unsuccessful, <code>AXUIElementCopyParameterizedAttributeValue</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorAttributeUnsupported</code> or <code>kAXErrorParameterizedAttributeUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified parameterized attribute.</dd>
 <dt><code>kAXErrorNoValue</code></dt><dd>The specified parameterized attribute does not have a value.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError	AXUIElementCopyParameterizedAttributeValue (AXUIElementRef element, CFStringRef parameterizedAttribute, CFTypeRef parameter, CFTypeRef __nullable * __nonnull CF_RETURNS_RETAINED result) CF_AVAILABLE_MAC(10_3);

/*!
 @function AXUIElementCopyActionNames
 @abstract Returns a list of all the actions the specified accessibility object can perform.
 @param element The AXUIElementRef representing the accessibility object.
 @param names On return, an array of actions the accessibility object can perform (empty if the accessibility object supports no actions).
 
 @result If unsuccessful, <code>AXUIElementCopyActionNames</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>One or both of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorFailure</code></dt><dd>There was some sort of system memory failure.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError 	AXUIElementCopyActionNames (AXUIElementRef element, CFArrayRef __nullable * __nonnull CF_RETURNS_RETAINED names);

/*!
 @function AXUIElementCopyActionDescription
 @abstract Returns a localized description of the specified accessibility object's action.
 
 @param element The AXUIElementRef representing the accessibility object.
 @param action The action to be described.
 @param description On return, a string containing the description of the action.
 
 @result If unsuccessful, <code>AXUIElementCopyActionDescription</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorActionUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified action (you will also receive this error if you pass in the system-wide accessibility object).</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError	AXUIElementCopyActionDescription (AXUIElementRef element, CFStringRef action, CFStringRef __nullable * __nonnull CF_RETURNS_RETAINED description);

/*!
 @function AXUIElementPerformAction
 @abstract Requests that the specified accessibility object perform the specified action.
 @discussion It is possible to receive the <code>kAXErrorCannotComplete</code> error code from this function because accessible applications often need to
 perform some sort of modal processing inside their action callbacks and they may not return within the timeout value set by the accessibility API.
 This does not necessarily mean that the function has failed, however. If appropriate, your assistive application
 can try to call this function again. Also, you may be able to increase the timeout value (see @link AXUIElementSetMessagingTimeout AXUIElementSetMessagingTimeout@/link).
 @param element The AXUIElementRef representing the accessibility object.
 @param action The action to be performed.
 
 @result If unsuccessful, <code>AXUIElementPerformAction</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorActionUnsupported</code></dt><dd>The specified AXUIElementRef does not support the specified action (you will also receive this error if you pass in the system-wide accessibility object).</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way or the application has not yet responded.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError 	AXUIElementPerformAction (AXUIElementRef element, CFStringRef action);

/*!
 @function AXUIElementCopyElementAtPosition
 @abstract Returns the accessibility object at the specified position in top-left relative screen coordinates.
 
 @discussion This function does hit-testing based on window z-order (that is, layering). If one window is on top of another window, the returned accessibility object comes from whichever window is topmost at the specified
 location. Note that if the system-wide accessibility object is passed in the <code>application</code> parameter, the position test is not restricted to a
 particular application.
 
 @param application The AXUIElementRef representing the application that contains the screen coordinates (or the system-wide accessibility object).
 @param x The horizontal position.
 @param y The vertical position.
 @param element On return, the accessibility object at the position specified by x and y.
 
 @result If unsuccessful, <code>AXUIElementCopyElementAtPosition</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorNoValue</code></dt><dd>There is no accessibility object at the specified position.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError	AXUIElementCopyElementAtPosition (AXUIElementRef application, float x,float y, AXUIElementRef __nullable * __nonnull CF_RETURNS_RETAINED element);

/*!
 @function AXUIElementCreateApplication
 @abstract Creates and returns the top-level accessibility object for the application with the specified process ID.
 
 @param pid The process ID of an application.
 @result The AXUIElementRef representing the top-level accessibility object for the application with the specified process ID.
 */
extern AXUIElementRef  AXUIElementCreateApplication (pid_t pid);

/*!
 @function AXUIElementCreateSystemWide
 @abstract Returns an accessibility object that provides access to system attributes.
 @discussion This is useful for things like finding the focused accessibility object regardless of which application is currently active.
 
 @result The AXUIElementRef representing the system-wide accessibility object.
 */
extern AXUIElementRef  AXUIElementCreateSystemWide (void);

/*!
 @function AXUIElementGetPid
 @abstract Returns the process ID associated with the specified accessibility object.
 
 @param element The AXUIElementRef representing an accessibility object.
 @param pid On return, the process ID associated with the specified accessibility object.
 
 @result If unsuccessful, <code>AXUIElementGetPid</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd></dl>
 */
extern AXError  AXUIElementGetPid (AXUIElementRef element, pid_t *pid);

/*!
 @function AXUIElementSetMessagingTimeout
 @abstract Sets the timeout value used in the accessibility API.
 @discussion Pass the system-wide accessibility object (see @link AXUIElementCreateSystemWide AXUIElementCreateSystemWide@/link) if you want to set the timeout globally for this process.
 Setting the timeout on another accessibility object sets it only for that object, not for other accessibility objects that are equal to it.
 
 Setting <code>timeoutInSeconds</code> to 0 for the system-wide accessibility object resets the global timeout to its default value. Setting <code>timeoutInSeconds</code>
 to 0 for any other accessibility object makes that element use the current global timeout value.
 
 @param element The AXUIElementRef representing an accessibility object.
 @param timeoutInSeconds The number of seconds for the new timeout value.
 
 @result If unsuccessful, <code>AXUIElementSetMessagingTimeout</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value (timeout values must be positive).</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd></dl>
 */
extern AXError AXUIElementSetMessagingTimeout (AXUIElementRef element, float timeoutInSeconds) CF_AVAILABLE_MAC(10_4);

/*!
 @function AXUIElementPostKeyboardEvent
 @abstract Posts keys to the specified application.
 
 @discussion This is similar to @link //apple_ref/c/func/CGPostKeyboardEvent CGPostKeyboardEvent@/link (which synthesizes a low-level keyboard event on the
 local machine), but it allows you to specify the target application as opposed to always sending the events to the active application. If the
 system-wide accessibility object is passed in the <code>application</code> parameter, the event is sent to the active application.
 
 You can only pass in the system-wide or application AXUIElementRef.
 
 @param application The AXUIElementRef representing the application (or the system-wide accessibility object).
 @param keyChar
 @param virtualKey
 @param keyDown
 
 @result If unsuccessful, <code>AXUIElementPostKeyboardEvent</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorInvalidUIElement</code></dt><dd>The AXUIElementRef is invalid.</dd>
 <dt><code>kAXErrorFailure</code></dt><dd>There is some sort of system memory failure.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorNotImplemented</code></dt><dd>The process does not fully support the accessibility API.</dd></dl>
 */
extern AXError AXUIElementPostKeyboardEvent (AXUIElementRef application, CGCharCode keyChar, CGKeyCode virtualKey, Boolean keyDown) CF_DEPRECATED_MAC(10_0, 10_9);;

/*! @group Text Marker API                   */

/*!
 @typedef AXTextMarkerRef
 
 @discussion
 An AXTextMarkerRef is a CFType. Like all CFTypes, they are reference counted (@link //apple_ref/c/func/CFRetain CFRetain@/link, @link //apple_ref/c/func/CFRelease CFRelease@/link).
 */
typedef const struct CF_BRIDGED_TYPE(id) __AXTextMarker *AXTextMarkerRef;

/*!
 @function AXTextMarkerGetTypeID
 @abstract Returns the unique type identifier for the AXTextMarkerRef type.
 
 @result Returns the CFTypeID of the AXTextMarkerRef type.
 */
extern CFTypeID AXTextMarkerGetTypeID (void);

/*!
 @function AXTextMarkerCreate
 @abstract Creates a new text marker.
 
 @param bytes The data used to identify this location in text
 @param length The length of the data
 */
extern AXTextMarkerRef AXTextMarkerCreate (CFAllocatorRef __nullable allocator, const UInt8 *bytes, CFIndex length);

/*!
 @function AXTextMarkerGetLength
 @abstract Get the length of bytes stored in a text marker
 
 @param marker The text marker
  
 @result The length of the data
 */
extern CFIndex AXTextMarkerGetLength (AXTextMarkerRef marker);

/*!
 @function AXTextMarkerGetBytePtr
 @abstract Get the byte data used to identify this location in text.
 
 @param marker The text marker
  
 @result a pointer to the byte data used to identify this location in text.
 */
extern const UInt8 *AXTextMarkerGetBytePtr (AXTextMarkerRef theTextMarker);

/*!
 @typedef AXTextMarkerRangeRef
 
 @discussion
 An AXTextMarkerRangeRef is a CFType. Like all CFTypes, they are reference counted (@link //apple_ref/c/func/CFRetain CFRetain@/link, @link //apple_ref/c/func/CFRelease CFRelease@/link).
 */
typedef const struct CF_BRIDGED_TYPE(id) __AXTextMarkerRange *AXTextMarkerRangeRef;

/*!
 @function AXTextMarkerRangeGetTypeID
 @abstract Returns the unique type identifier for the AXTextMarkerRangeRef type.
 
 @result Returns the CFTypeID of the AXTextMarkerRangeRef type.
 */
extern CFTypeID AXTextMarkerRangeGetTypeID (void);

/*!
 @function AXTextMarkerRangeGetTypeID
 @abstract Creates a new text marker given a start and end marker
 
 @param startMarker The text marker for the beginning of the range
 @param endMarker The text marker for the end of the range

 @result The text marker range object
 */
extern AXTextMarkerRangeRef AXTextMarkerRangeCreate (CFAllocatorRef __nullable allocator, AXTextMarkerRef startMarker, AXTextMarkerRef endMarker);


/*!
 @function AXTextMarkerRangeGetTypeID
 @abstract Creates a new text marker given the bytes for a start marker and an end marker
 
 @param startMarkerBytes The data used to identify the start marker location in text
 @param startMarkerLength The length of the start marker data
 @param endMarkeBytes The data used to identify the end marker location in text
 @param endMarkerLength The length of the end marker data
 
 @result The text marker range object
 */
extern AXTextMarkerRangeRef AXTextMarkerRangeCreateWithBytes (CFAllocatorRef __nullable allocator, const UInt8 *startMarkerBytes, CFIndex startMarkerLength, const UInt8 *endMarkerBytes, CFIndex endMarkerLength);

/*!
 @function AXTextMarkerRangeCopyStartMarker
 @abstract Copies the start marker from a text marker range
 
 @param textMarkerRange The text marker range
 
 @result The start text marker object
 */
extern AXTextMarkerRef AXTextMarkerRangeCopyStartMarker (AXTextMarkerRangeRef textMarkerRange);

/*!
 @function AXTextMarkerRangeCopyEndMarker
 @abstract Copies the end marker from a text marker range
 
 @param textMarkerRange The text marker range
 
 @result The end text marker object
 */
extern AXTextMarkerRef AXTextMarkerRangeCopyEndMarker (AXTextMarkerRangeRef textMarkerRange);


/*! @group Notification API                   */

/*!
 @typedef AXObserverRef
 
 @discussion
 An AXObserverRef is a CFType. Like all CFTypes, they are reference counted (@link //apple_ref/c/func/CFRetain CFRetain@/link, @link //apple_ref/c/func/CFRelease CFRelease@/link).
 */
typedef struct CF_BRIDGED_TYPE(id) __AXObserver *AXObserverRef;

/*!
 @typedef AXObserverCallback
 @abstract
 
 @param observer An AXObserverRef object to observe the notifications.
 @param element The accessibility object.
 @param notification The name of the notification to observe.
 @param refcon Application-defined data specified when registering the observer for notification
 */
typedef void (*AXObserverCallback)(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void * __nullable refcon);

/*!
 @typedef AXObserverCallbackWithInfo
 @abstract
     
 @param observer An AXObserverRef object to observe the notifications.
 @param element The accessibility object.
 @param notification The name of the notification to observe.
 @param info The coresponding notification information.
 @param refcon Application-defined data specified when registering the observer for notification
 */
typedef void (*AXObserverCallbackWithInfo)(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, CFDictionaryRef info, void * __nullable refcon);

/*!
 @function AXObserverGetTypeID
 @abstract Returns the unique type identifier for the AXObserverRef type.
 
 @result Returns the CFTypeID of the AXObserverRef type.
 */
CFTypeID AXObserverGetTypeID (void);

/*!
 @function AXObserverCreate
 @abstract Creates a new observer that can receive notifications from the specified application.
 @discussion When an observed notification is received, it is passed to @link AXObserverCallback AXObserverCallback@/link.
 
 @param application The process ID of the application.
 @param callback The callback function.
 @param outObserver On return, an AXObserverRef representing the observer object.
 
 @result If unsuccessful, <code>AXObserverCreate</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
 <dt><code>kAXErrorFailure</code></dt><dd>There is some sort of system memory failure.</dd></dl>
 */
extern AXError AXObserverCreate (pid_t application, AXObserverCallback callback, AXObserverRef __nullable * __nonnull CF_RETURNS_RETAINED outObserver);

/*!
 @function AXObserverCreateWithInfoCallback
 @abstract Creates a new observer that can receive notifications with an information dictionary from the specified application.
 @discussion When an observed notification is received, it is passed to @link AXObserverCallbackWithInfo AXObserverCallbackWithInfo@/link.
     
 @param application The process ID of the application.
 @param callback The callback function.
 @param outObserver On return, an AXObserverRef representing the observer object.
     
 @result If unsuccessful, <code>AXObserverCreateWithInfoCallback</code> may return one of the following error codes, among others:
     <dl><dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value.</dd>
     <dt><code>kAXErrorFailure</code></dt><dd>There is some sort of system memory failure.</dd></dl>
*/
extern AXError AXObserverCreateWithInfoCallback (pid_t application, AXObserverCallbackWithInfo callback, AXObserverRef __nullable * __nonnull CF_RETURNS_RETAINED outObserver);
    
/*!
 @function AXObserverAddNotification
 @abstract Registers the specified observer to receive notifications from the specified accessibility object.
 
 @param observer The observer object created from a call to @link AXObserverCreate AXObserverCreate@/link.
 @param element The accessibility object for which to observe notifications.
 @param notification The name of the notification to observe.
 @param refcon Application-defined data passed to the callback when it is called.
 
 @result If unsuccessful, <code>AXObserverAddNotification</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorInvalidUIElementObserver</code></dt><dd>The observer is not a valid AXObserverRef type.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value or the length of the notification name is greater than 1024.</dd>
 <dt><code>kAXErrorNotificationUnsupported</code></dt><dd>The accessibility object does not support notifications (note that the system-wide accessibility object does not support notifications).</dd>
 <dt><code>kAXErrorNotificationAlreadyRegistered</code></dt><dd>The notification has already been registered.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorFailure</code></dt><dd>There is some sort of system memory failure.</dd></dl>
 */
extern AXError AXObserverAddNotification (AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void * __nullable refcon);

/*!
 @function AXObserverRemoveNotification
 @abstract Removes the specified notification from the list of notifications the observer wants to receive from the accessibility object.
 
 @param observer The observer object created from a call to @link AXObserverCreate AXObserverCreate@/link.
 @param element The accessibility object for which this observer observes notifications.
 @param notification The name of the notification to remove from the list of observed notifications.
 
 @result If unsuccessful, <code>AXObserverRemoveNotification</code> may return one of the following error codes, among others:
 <dl><dt><code>kAXErrorInvalidUIElementObserver</code></dt><dd>The observer is not a valid AXObserverRef type.</dd>
 <dt><code>kAXErrorIllegalArgument</code></dt><dd>One or more of the arguments is an illegal value or the length of the notification name is greater than 1024.</dd>
 <dt><code>kAXErrorNotificationUnsupported</code></dt><dd>The accessibility object does not support notifications (note that the system-wide accessibility object does not support notifications).</dd>
 <dt><code>kAXErrorNotificationNotRegistered</code></dt><dd>This observer has not registered for any notifications.</dd>
 <dt><code>kAXErrorCannotComplete</code></dt><dd>The function cannot complete because messaging has failed in some way.</dd>
 <dt><code>kAXErrorFailure</code></dt><dd>There is some sort of system memory failure.</dd></dl>
 */
extern AXError AXObserverRemoveNotification (AXObserverRef observer, AXUIElementRef element, CFStringRef notification);

/*!
 @function AXObserverGetRunLoopSource
 @abstract Returns the observer's run loop source.
 @discussion The observer must be added to a run loop before it can receive notifications. Note that releasing the AXObserverRef automatically
 removes the run loop source from the run loop (you can also do this explicitly by calling @link //apple_ref/c/func/CFRunLoopRemoveSource CFRunLoopRemoveSource@/link).
 
 <code>AXObserverGetRunLoopSource</code> might be used in code in this way:
 
 <pre>
 CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(observer), kCFRunLoopDefaultMode);
 </pre>
 
 @param observer The observer object (created from a call to @link AXObserverCreate AXObserverCreate@/link) for which to get the run loop source.
 @result Returns the CFRunLoopSourceRef of the observer; NIL if you pass NIL in <code>observer</code>.
 */
extern CFRunLoopSourceRef AXObserverGetRunLoopSource (AXObserverRef observer);

CF_IMPLICIT_BRIDGING_DISABLED
CF_ASSUME_NONNULL_END
    
#ifdef __cplusplus
}
#endif

#endif // __AXUIELEMENT__
