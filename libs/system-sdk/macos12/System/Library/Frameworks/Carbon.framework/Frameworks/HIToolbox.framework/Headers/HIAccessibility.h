/*
     File:       HIToolbox/HIAccessibility.h
 
     Contains:   Accessibility Carbon events and API for HIToolbox
 
     Copyright:  © 2005-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIACCESSIBILITY__
#define __HIACCESSIBILITY__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __CARBONEVENTS__
#include <HIToolbox/CarbonEvents.h>
#endif

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif

#ifndef __MENUS__
#include <HIToolbox/Menus.h>
#endif

#ifndef __CONTROLS__
#include <HIToolbox/Controls.h>
#endif

#ifndef __MACWINDOWS__
#include <HIToolbox/MacWindows.h>
#endif

#ifndef __HIOBJECT__
#include <HIToolbox/HIObject.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------*/
/*  Accessibility Events                                                                */
/*--------------------------------------------------------------------------------------*/
/*
    kEventClassAccessibility quick reference:
    
    kEventAccessibleGetChildAtPoint                     =   1,
    kEventAccessibleGetFocusedChild                     =   2,
    
    kEventAccessibleGetAllAttributeNames                =   21,
    kEventAccessibleGetAllParameterizedAttributeNames   =   25,
    kEventAccessibleGetNamedAttribute                   =   22,
    kEventAccessibleSetNamedAttribute                   =   23,
    kEventAccessibleIsNamedAttributeSettable            =   24,
    
    kEventAccessibleGetAllActionNames                   =   41,
    kEventAccessiblePerformNamedAction                  =   42,
    kEventAccessibleGetNamedActionDescription           =   44
*/
/*
 *  kEventClassAccessibility / kEventAccessibleGetChildAtPoint
 *  
 *  Summary:
 *    Finds the child of an accessible object at a given point.
 *  
 *  Discussion:
 *    The kEventParamMouseLocation parameter will contain a global
 *    point. Your handler for this event should find the child of
 *    yourself which is underneath that point and return it in the
 *    kEventParamAccessibleChild parameter.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    --> kEventParamMouseLocation (in, typeHIPoint)
 *          The location in global coordinates.
 *    
 *    <-- kEventParamAccessibleChild (out, typeCFTypeRef)
 *          On exit, contains the child of the accessible object at the
 *          specified point, in the form of an AXUIElementRef. If there
 *          is no child at the given point, you should still return
 *          noErr, but leave the parameter empty (do not call
 *          SetEventParameter). Only return immediate children; do not
 *          return grandchildren of yourself.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleGetChildAtPoint = 1
};

/*
 *  kEventClassAccessibility / kEventAccessibleGetFocusedChild
 *  
 *  Summary:
 *    Finds the focused child of an accessible object.
 *  
 *  Discussion:
 *    Your handler for this event should find the child of itself which
 *    is part of the focus chain and return it in the
 *    kEventParamAccessibleChild parameter.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    <-- kEventParamAccessibleChild (out, typeCFTypeRef)
 *          On exit, contains the focused child of the accessible
 *          object, in the form of an AXUIElementRef. If there is no
 *          child in the focus chain, you should still return noErr,
 *          but leave the parameter empty (do not call
 *          SetEventParameter). Only return immediate children; do not
 *          return grandchildren of yourself.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleGetFocusedChild = 2
};

/*
 *  kEventClassAccessibility / kEventAccessibleGetAllAttributeNames
 *  
 *  Summary:
 *    Returns the attributes supported by an accessible object. You
 *    must only return the names of your regular (non-parameterized)
 *    attributes via this event. If you support parameterized
 *    attributes, you must return them via the new
 *    kEventAccessibleGetAllParameterizedAttributeNames event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    <-> kEventParamAccessibleAttributeNames (in/out, typeCFMutableArrayRef)
 *          Add each of the regular (non-parameterized) attribute names
 *          supported by the accessible object to this array, as
 *          CFStringRefs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleGetAllAttributeNames = 21
};

/*
 *  kEventClassAccessibility / kEventAccessibleGetAllParameterizedAttributeNames
 *  
 *  Summary:
 *    Returns the parameterized attributes supported by an accessible
 *    object. You must not return the names of your regular
 *    (non-parameterized) attributes via this event. If you support
 *    regular attributes, you must return them via the original
 *    kEventAccessibleGetAllAttributeNames event. Parameterized
 *    attributes are introduced in Mac OS X 10.3.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    <-> kEventParamAccessibleAttributeNames (in/out, typeCFMutableArrayRef)
 *          Add each of the parameterized attribute names supported by
 *          the accessible object to this array, as CFStringRefs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleGetAllParameterizedAttributeNames = 25
};

/*
 *  kEventClassAccessibility / kEventAccessibleGetNamedAttribute
 *  
 *  Summary:
 *    Returns the value of an attribute of an accessible object.
 *  
 *  Discussion:
 *    The kEventParamAccessibleAttributeName parameter will contain an
 *    attribute name in the form of a CFStringRef. If you support the
 *    named attribute, return the attributeÕs value in the
 *    kEventParamAccessibleAttributeValue parameter.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    --> kEventParamAccessibleAttributeName (in, typeCFStringRef)
 *          The name of the requested attribute.
 *    
 *    --> kEventParamAccessibleAttributeParameter (in, typeCFTypeRef)
 *          This parameter is optional and will only exist if your
 *          accessible object is being asked for the value of a
 *          parameterized attribute. When present, this event parameter
 *          will contain a CFTypeRef describing the parameters of the
 *          request. Parameterized attributes are introduced in Mac OS
 *          X 10.3.
 *    
 *    <-- kEventParamAccessibleAttributeValue (out, typeCFTypeRef)
 *          On exit, contains the attribute's value. The type of this
 *          parameter varies according to the attribute; it might
 *          typically be typeCFStringRef (for a textual attribute),
 *          typeBoolean (for a boolean attribute), or typeSInt32 (for
 *          an integer-valued attribute).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleGetNamedAttribute = 22
};

/*
 *  kEventClassAccessibility / kEventAccessibleSetNamedAttribute
 *  
 *  Summary:
 *    Sets the value of an attribute of an accessible object.
 *  
 *  Discussion:
 *    The kEventParamAccessibleAttributeName parameter will contain an
 *    attribute name in the form of a CFStringRef. The
 *    kEventParamAccessibleAttributeValue parameter will contain data
 *    in an arbitrary format. If you support the named attribute, set
 *    the named attributeÕs value to the data provided in the event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    --> kEventParamAccessibleAttributeName (in, typeCFStringRef)
 *          The name of the requested attribute.
 *    
 *    --> kEventParamAccessibleAttributeValue (in, typeCFTypeRef)
 *          The new value of the attribute. The type of this parameter
 *          varies according to the attribute; it might typically be
 *          typeCFStringRef (for a textual attribute), typeBoolean (for
 *          a boolean attribute), or typeSInt32 (for an integer-valued
 *          attribute).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleSetNamedAttribute = 23
};

/*
 *  kEventClassAccessibility / kEventAccessibleIsNamedAttributeSettable
 *  
 *  Summary:
 *    Determines whether an attribute of an accessible object can be
 *    modified.
 *  
 *  Discussion:
 *    The kEventParamAccessibleAttributeName parameter will contain an
 *    attribute name in the form of a CFStringRef. If you support the
 *    named attribute, set the kEventParamAccessibleAttributeSettable
 *    parameter to a Boolean indicating whether the named attribute can
 *    have its value changed via the kEventAccessibleSetNamedAttribute
 *    event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    --> kEventParamAccessibleAttributeName (in, typeCFStringRef)
 *          The name of the requested attribute.
 *    
 *    <-- kEventParamAccessibleAttributeSettable (out, typeBoolean)
 *          On exit, indicates whether the attribute may be modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleIsNamedAttributeSettable = 24
};

/*
 *  kEventClassAccessibility / kEventAccessibleGetAllActionNames
 *  
 *  Summary:
 *    Returns the actions supported by an accessible object.
 *  
 *  Discussion:
 *    The kEventParamAccessibleActionNames parameter will contain a
 *    CFMutableArrayRef. Add each of the action names you support to
 *    this array in the form of a CFStringRef.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    <-> kEventParamAccessibleActionNames (in/out, typeCFMutableArrayRef)
 *          Add each of the actions supported by the accessible object
 *          to this array, as CFStringRefs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleGetAllActionNames = 41
};

/*
 *  kEventClassAccessibility / kEventAccessiblePerformNamedAction
 *  
 *  Summary:
 *    Requests that a specific action be performed by an accessible
 *    object.
 *  
 *  Discussion:
 *    The kEventParamAccessibleActionName parameter will contain an
 *    attribute name in the form of a CFStringRef. If you support the
 *    named action, perform the action. There are times, however, when
 *    performing an action causes an accessible object to call a
 *    routine which may not return immediately, such as StandardAlert
 *    or PopUpMenuSelect. You should only call such routines when you
 *    receive an action request that was queued; if you call such a
 *    routine when processing an event that was directly dispatched,
 *    you will probably cause the assistive app to receive a timeout
 *    error. On Mac OS X 10.3 and later, the
 *    kEventParamAccessibilityEventQueued parameter will indicate
 *    whether the event was queued. If so, process the request
 *    normally. If not, you can request that the event be posted to the
 *    queue and sent to you later by returning
 *    eventDeferAccessibilityEventErr from your handler. On Mac OS X
 *    10.2, the parameter will not exist, the event will always be
 *    directly dispatched, and there is no way to request that it be
 *    posted to the queue; in this case, you should perform the action
 *    even if it will cause the assistive app to receive a timeout
 *    error.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    --> kEventParamAccessibleActionName (in, typeCFStringRef)
 *          The name of the requested action.
 *    
 *    --> kEventParamAccessibilityEventQueued (in, typeBoolean)
 *          An indication of whether this event was delivered to you
 *          via the main event queue. This parameter only exists on Mac
 *          OS X 10.3 and later. See the discussion for more details.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessiblePerformNamedAction = 42
};

/*
 *  kEventClassAccessibility / kEventAccessibleGetNamedActionDescription
 *  
 *  Summary:
 *    Returns a human-language description of an action supported by an
 *    accessible object.
 *  
 *  Discussion:
 *    The kEventParamAccessibleActionName parameter will contain an
 *    attribute name in the form of a CFStringRef. The
 *    kEventParamAccessibleActionDescription parameter will contain a
 *    CFMutableStringRef. If you support the named action, alter the
 *    mutable string to contain a textual description of the actionÕs
 *    significance.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamAccessibleObject (in, typeCFTypeRef)
 *          The accessible object, in the form of an AXUIElementRef.
 *    
 *    --> kEventParamAccessibleActionName (in, typeCFStringRef)
 *          The name of the requested action.
 *    
 *    <-> kEventParamAccessibleActionDescription (in/out, typeCFMutableStringRef)
 *          If you support the action, extract this parameter from the
 *          event and set the contents of the mutable string to contain
 *          a description of the action. Do not set this event
 *          parameter to a CFStringRef of your own creation; you must
 *          modify the preexisting mutable string stored in the event
 *          parameter.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventAccessibleGetNamedActionDescription = 44
};

#if !__LP64__
/*
 *  AXUIElementCreateWithHIObjectAndIdentifier()
 *  
 *  Discussion:
 *    This routine creates an AXUIElementRef to represent an accessible
 *    object for a Carbon application. A Carbon accessible object is
 *    comprised of an HIObjectRef and a 64-bit identifier. The
 *    resulting AXUIElementRef is a CFTypeRef, and must be managed as
 *    such. You can create a new AXUIElementRef every time you need
 *    one. Even though the actual hex values of two AXUIElementRefs
 *    might be different, they may represent the same accessible
 *    object; because AXUIElementRefs are Core Foundation objects, you
 *    can use CFEqual to compare them.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inHIObject:
 *      The HIObjectRef of the accessible object.
 *    
 *    inIdentifier:
 *      The 64-bit identifier of the accessible object.
 *  
 *  Result:
 *    An AXUIElementRef that represents the Carbon accessible object
 *    identified by the given HIObjectRef and 64-bit identifier. This
 *    follows CoreFoundation semantics in that it will return NULL for
 *    failure, and because it is a "Create" function you will need to
 *    CFRelease() this AXUIElementRef when it is no longer needed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern AXUIElementRef 
AXUIElementCreateWithHIObjectAndIdentifier(
  HIObjectRef   inHIObject,
  UInt64        inIdentifier)                                 AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  AXUIElementGetHIObject()
 *  
 *  Discussion:
 *    If the incoming AXUIElementRef is a Carbon accessible object,
 *    this routine will return the HIObjectRef of the accessible object.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inUIElement:
 *      The AXUIElementRef of whom you'd like to get the HIObjectRef.
 *  
 *  Result:
 *    The HIObjectRef of the AXUIElementRef. If the incoming
 *    AXUIElementRef is not a Carbon accessible object, this routine
 *    will return NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern HIObjectRef 
AXUIElementGetHIObject(AXUIElementRef inUIElement)            AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  AXUIElementGetIdentifier()
 *  
 *  Discussion:
 *    If the incoming AXUIElementRef is a Carbon accessible object,
 *    this routine will pass back the 64-bit identifier of the
 *    accessible object.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inUIElement:
 *      The AXUIElementRef of whom you'd like to get the 64-bit
 *      identifier.
 *    
 *    outIdentifier:
 *      The 64-bit identifier of the AXUIElementRef. If the incoming
 *      AXUIElementRef is not a Carbon accessible object, this routine
 *      will pass back zero. Note that zero is often a legal value for
 *      Carbon accessible object, so do not assume that the accessible
 *      object is not a Carbon accessible object just because you get a
 *      result of zero.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
AXUIElementGetIdentifier(
  AXUIElementRef   inUIElement,
  UInt64 *         outIdentifier)                             AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  AXNotificationHIObjectNotify()
 *  
 *  Discussion:
 *    Posts a notification for the given pseudo-AXUIElementRef. Though
 *    an actual AXUIElementRef is not actually passed in to this
 *    function, its component parts are. This saves the implementation
 *    the hassle of dismantling the AXUIElementRef into its component
 *    parts.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inNotification:
 *      The notification name string.
 *    
 *    inHIObject:
 *      The HIObjectRef component of the AXUIElementRef to whom the
 *      notification applies.
 *    
 *    inIdentifier:
 *      The 64-bit identifier component of the AXUIElementRef to whom
 *      the notification applies.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
AXNotificationHIObjectNotify(
  CFStringRef   inNotification,
  HIObjectRef   inHIObject,
  UInt64        inIdentifier)                                 AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HICopyAccessibilityRoleDescription()
 *  
 *  Summary:
 *    Returns the role description string for a standard role or a
 *    standard role-subrole pair.
 *  
 *  Discussion:
 *    This routine is useful if you are implementing an accessible
 *    object that has a standard role or role-subrole pair and you want
 *    to make sure your object provides the same role description
 *    string that the equivalent system-supplied object provides. In
 *    other words, if you are implementing an accessible object of role
 *    kAXButtonRole, you can use this routine to make sure it provides
 *    the same role description that the standard push button provides.
 *    
 *    This routine can provide role description strings for all roles
 *    and role-subrole pairs that are used in the standard/system
 *    accessible objects on Mac OS X 10.4 and later. Once this routine
 *    is able to provide a role description string for a role or
 *    role-subrole pair, it will continue to do so on subsequent system
 *    releases, even if the system no longer produces a standard
 *    accessible object with that role or role-subrole pair.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inRole:
 *      The role CFStringRef for your accessible object. Callers
 *      typically pass one of the kAXFooRole constant strings from
 *      within the HIServices framework.
 *    
 *    inSubrole:
 *      The subrole CFStringRef for your accessible object. Callers
 *      typically pass one of the kAXFooSubrole constant strings from
 *      within the HIServices framework. Pass NULL if your accessible
 *      object does not have a subrole.
 *  
 *  Result:
 *    A CFStringRef with the standard role description for the role or
 *    role-subrole pair. You must release the role description when you
 *    are finished with it. If there is no standard role description
 *    for the role or role-subrole pair, this routine will return NULL.
 *    If you pass either an unknown role or an unknown subrole, this
 *    routine will return NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
HICopyAccessibilityRoleDescription(
  CFStringRef   inRole,
  CFStringRef   inSubrole)       /* can be NULL */            AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HICopyAccessibilityActionDescription()
 *  
 *  Summary:
 *    Returns the action description string for a standard
 *    accessibility action.
 *  
 *  Discussion:
 *    This routine is useful if you are implementing an accessible
 *    object that implements a standard action and you want to make
 *    sure your object provides the same role action string that the a
 *    system-supplied object provides. 
 *    This routine can provide action description strings for all
 *    actions that are used in the standard/system accessible objects
 *    on Mac OS X 10.4 and later. Once this routine is able to provide
 *    a description string for an action, it will continue to do so on
 *    subsequent system releases, even if the system no longer produces
 *    a standard accessible object that supports the action.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inAction:
 *      The action CFStringRef for which you'd like to generate a
 *      description. Callers must pass one of the kAXFooAction constant
 *      strings from within the HIServices framework.
 *  
 *  Result:
 *    A CFStringRef with the standard description for the action. You
 *    must release the description when you are finished with it. If
 *    you pass an unsupported action to this routine, the behavior is
 *    undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
HICopyAccessibilityActionDescription(CFStringRef inAction)    AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIObjectIsAccessibilityIgnored()
 *  
 *  Discussion:
 *    Reports whether or not the given HIObject is marked as ignored
 *    for accessibility. 
 *    
 *    See the discussion of HIObjectSetAccessibilityIgnored for details
 *    on what it means to be accessibility ignored.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inObject:
 *      The object whose accessibility ignored state you wish to query.
 *  
 *  Result:
 *    A Boolean value indicating whether or not the HIObject is ignored
 *    for accessibility.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIObjectIsAccessibilityIgnored(HIObjectRef inObject)          AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIObjectSetAccessibilityIgnored()
 *  
 *  Discussion:
 *    Marks an HIObject as ignored (or not) for the purposes of the
 *    accessibility APIs. 
 *    
 *    An HIObject that is ignored for accessibility will never be shown
 *    to an assistive application that uses the accessibility APIs to
 *    examine an interface. Your application's accessibility
 *    implementation can (and should) still report an ignored HIObject
 *    as usual. Carbon's accessibility engine will automatically prune
 *    any ignored HIObjects out of the data that is shown to an
 *    assistive application. 
 *    <BR>By default, an HIObject is *not* accessibility ignored.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inObject:
 *      The object whose accessibility ignored state you wish to change.
 *    
 *    inIgnored:
 *      A Boolean value indicating whether or not to ignore the object.
 *  
 *  Result:
 *    An OSStatus signifying success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIObjectSetAccessibilityIgnored(
  HIObjectRef   inObject,
  Boolean       inIgnored)                                    AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIObjectSetAuxiliaryAccessibilityAttribute()
 *  
 *  Summary:
 *    Associates an additional accessibility attribute with a UIElement
 *    that is used to represent a given HIObject or a part thereof.
 *  
 *  Discussion:
 *    This routine lets your application provide the name of and data
 *    for an accessibility attribute that you want to add to the
 *    UIElement used to represent a given HIObject-identifier pair.
 *    Normally, accessibility attributes can only be supplied
 *    dynamically via Carbon Events, but this routine allows you to
 *    supply them statically. 
 *    
 *    When an accessibility attribute Carbon Event is handled by the
 *    HIObject with a given identifier, the toolbox automatically
 *    supplies the names and/or values of any auxiliary attributes
 *    associated with that HIObject- identifier pair. 
 *    
 *    This routine is particularly useful for supplying values for the
 *    kAXDescriptionAttribute, kAXTitleUIElementAttribute,
 *    kAXServesAsTitleUIElementAttribute, kAXLinkedUIElementsAttribute
 *    and other attributes whose value is specific to the layout and
 *    usage of your application. 
 *    
 *    This routine only allows you to associate attributes whose values
 *    never change. If you need to supply attributes whose values are
 *    determined dynamically or whose values are settable, you must
 *    install the normal accessibility Carbon Event handlers. 
 *    
 *    The auxiliary attribute store is consulted during the HIObject's
 *    default handling of the accessibility attribute Carbon Events.
 *    This means that any programmatic handling of a given
 *    accessibility attribute will have a chance to override or block
 *    the consultation of the store. The general rule is that if the
 *    toolbox or a Carbon Event handler can provide the attribute value
 *    some other way, the store will not be consulted.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inHIObject:
 *      The HIObjectRef part of the object-identifier pair to which the
 *      attribute data is associated.
 *    
 *    inIdentifier:
 *      The 64-bit identifier part of the object-identifier pair to
 *      which the attribute data is associated. When you want to
 *      associate the attribute data to the HIObject as a whole -- such
 *      as when you want to give a description attribute to a push
 *      button -- you should pass zero in this parameter.
 *    
 *    inAttributeName:
 *      A CFStringRef of the name of the attribute you wish to
 *      associate with the object-identifier pair. This string will be
 *      retained before adding it to the auxiliary attribute store.
 *    
 *    inAttributeData:
 *      A CFTypeRef with the data to be supplied as the attribute's
 *      value. This data will be retained before adding it to the
 *      auxiliary attribute store; you may release inAttributeData
 *      after you have called this routine. The inAttributeData
 *      parameter may also be NULL, which indicates that the named
 *      auxiliary attribute should no longer be associated with the
 *      object-identifier pair; any named attribute data previously
 *      associated with the object-identifier pair will be released.
 *  
 *  Result:
 *    An OSStatus result code. The function will return noErr if it was
 *    able to associate the attribute data with the HIObjectRef. If the
 *    HIObjectRef is invalid, paramErr will be returned. Other results
 *    may be returned in other situations.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIObjectSetAuxiliaryAccessibilityAttribute(
  HIObjectRef   inHIObject,
  UInt64        inIdentifier,
  CFStringRef   inAttributeName,
  CFTypeRef     inAttributeData)       /* can be NULL */      AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIObjectOverrideAccessibilityContainment()
 *  
 *  Summary:
 *    Allows you to override the AXUIElementRefs that an HIObject would
 *    normally supply as the values of its AXParent, AXWindow, and
 *    AXTopLevelUIElement attributes.
 *  
 *  Discussion:
 *    This routine allows you to change the parent that an HIObject
 *    would normally supply in the accessibility hierarchy. For
 *    instance, a popup control could call this routine on its menu so
 *    that the menu returned the popup control as the menu's parent;
 *    normally the menu would return the application as its parent.
 *    Optionally, this routine also allows you to change the window and
 *    top-level element that an HIObject would normally supply. 
 *     
 *    If the input HIObject is a standard toolbox construct like an
 *    HIView or a Menu, the input HIObject will not be added as an
 *    accessibility child of its normal parent. In all other cases, it
 *    is the client's responsibility to ensure that the input HIObject
 *    is not added as an accessibility child of its normal parent. 
 *     
 *    If the desired AXUIElementRef parent represents an HIView, a
 *    Menu, or a Window, the input HIObject will be automatically added
 *    as an accessibility child of the specified parent. In all other
 *    cases, it is the client's responsibility to manually add the
 *    input HIObject as a child of the specified parent. To represent
 *    an HIView, a Menu, or a Window, an AXUIElementRef must contain
 *    the appropriate HIObjectRef as well as an identifier value of
 *    zero. 
 *    
 *    Similar rules don't have to apply for the handling of the window
 *    and top-level element attributes because those attributes don't
 *    represent two-way relationships. 
 *    
 *    A containment override is not necessarily supported by every type
 *    of HIObject. Currently, it is supported by HIViews, Menus, and
 *    Windows.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inHIObject:
 *      The HIObjectRef whose parent attribute you want to override.
 *    
 *    inDesiredParent:
 *      The AXUIElementRef that you wish the HIObject to return as the
 *      value of its AXParent attribute. This routine makes a copy of
 *      the AXUIElementRef; you must release inDesiredParent after you
 *      have called this routine. Passing NULL indicates that you want
 *      the HIObject to revert to its normal, un-overridden behavior.
 *    
 *    inDesiredWindow:
 *      The AXUIElementRef that you wish the HIObject to return as the
 *      value of its AXWindow attribute. This routine makes a copy of
 *      the AXUIElementRef; you must release inDesiredWindow after you
 *      have called this routine. Passing NULL indicates that you want
 *      the HIObject to report its normal window, if any.
 *    
 *    inDesiredTopLevelUIElement:
 *      The AXUIElementRef that you wish the HIObject to return as the
 *      value of its AXTopLevelUIElement attribute. This routine makes
 *      a copy of the AXUIElementRef; you must release
 *      inDesiredTopLevelUIElement after you have called this routine.
 *      Passing NULL indicates that you want the HIObject to report its
 *      normal top-level element, if any.
 *  
 *  Result:
 *    An OSStatus result code. If the HIObjectRef is invalid, this
 *    routine will return paramErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIObjectOverrideAccessibilityContainment(
  HIObjectRef      inHIObject,
  AXUIElementRef   inDesiredParent,
  AXUIElementRef   inDesiredWindow,
  AXUIElementRef   inDesiredTopLevelUIElement)                AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;




#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HIACCESSIBILITY__ */

