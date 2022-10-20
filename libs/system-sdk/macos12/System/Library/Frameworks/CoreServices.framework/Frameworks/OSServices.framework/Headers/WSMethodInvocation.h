/*
     File:       OSServices/WSMethodInvocation.h
 
     Contains:   *** DEPRECATED *** WebServicesCore Method Invocation API
 
     Copyright:  (c) 2002-2011 Apple Inc. All rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __WSMETHODINVOCATION__
#define __WSMETHODINVOCATION__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __WSTYPES__
#include <OSServices/WSTypes.h>
#endif


#include <Availability.h>

/*
    WSMethodInvocation
 */
#ifdef __cplusplus
extern "C" {
#endif
/*
    Dictionary entry if the invocation result is not a fault.  This is
    always available in method responses, although for SOAP messages,
    it may be more correctly to query the result dictionary for the
    specific field you're interested in.  What this really means is
    that the dictionary returned by the invocation may contain more
    than one value, wherein the result data is duplicated several
    times.  If you don't know what to ask for to dump the reply, you
    can ask for this key.  If you do know what you want, you should
    request that field expliclty.

    You can also specify the name of the reply parameter in the
    invocation using kWSMethodInvocationResultParameterName.  This will
    add an alias for the given name to the result dictionary such that
    kWSMethodInvocationResult will always return the correct
    parameter.  (This won't work for multi-value returns, however.)
*/
extern CFStringRef kWSMethodInvocationResult                         API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
/*
    Dictionary entries if the result is a fault
*/
extern CFStringRef kWSFaultString  /* a CFString */                  API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSFaultCode    /* a CFNumber */                  API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSFaultExtra   /* a CFString or CFDictionary, or NULL */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
    If the result is a fault, and if the value of kWSFaultString in
    the reply dictionary is kWSNetworkStreamFaultString, then
    kWSFaultExtra will be a dictionary indicating the network error
    and kWSFaultCode is ignored in this case.  See
    <CoreFoundation/CFStream.h> for details on what the domain and
    error numbers mean.
*/
extern CFStringRef kWSNetworkStreamFaultString                       API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSStreamErrorMessage   /* A CFString (for debug purposes only) */ API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSStreamErrorDomain    /* A CFNumberRef */       API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSStreamErrorError     /* A CFNumberRef */       API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

/* 
    For HTTP[S] based invocations, you can specify a CFHTTPMessageRef
    as a property which will be used instead of creating a new
    outgoing message.  The CFHTTPMessageRef can contain header, proxy
    and authentication information.  The body of the message will be
    ignored and replaced with the outgoing, serialized invocation.

    After the invocation has executed, you can retrieve a copy of the
    actual CFHTTPMessageRef, containing the details of the invocation
    using kWSHTTPResponseMessage.  Attempting to retrieve the response
    message property before the invocation completes will result
    return NULL.
   
    See: <CFNetwork/CFHTTPMessage.h> for more information.
*/
extern CFStringRef kWSHTTPMessage           /* CFHTTPMessageRef */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSHTTPResponseMessage   /* CFHTTPMessageRef */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
/*
    To avoid having to create an entire CFHTTPMessageRef, these properties are
    individually settable.  If they are set, they will override any CFHTTPMessageRef
    previously specified.
*/
extern CFStringRef kWSHTTPExtraHeaders       /* a CFDictionary of { key (CFString), val (CFString) } pairs */ API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSHTTPVersion            /* "http/1.1" */        API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSHTTPProxy              /* CFURLRef */          API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSHTTPFollowsRedirects   /* kCFBooleanFalse */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
/* 
    SOCKS proxy support.  WSMethodInvocation uses the same flags as
    CFSocketStream.h in configuring SOCKS proxy support.  You can set
    the kCFStreamPropertySOCKSProxy property on the invocation and the
    value will be applied to the underlying stream.  See CFSocketStream.h
    for more information and valid keys.
*/
/*
    These debugging flags will populate the WSInvocationResultRef
    with some potentially useful debugging output.  The property
    name of the flag is the same as the the field in the result
    dictionary.
*/
extern CFStringRef kWSDebugOutgoingHeaders   /* kCFBooleanFalse */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSDebugOutgoingBody      /* kCFBooleanFalse */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSDebugIncomingHeaders   /* kCFBooleanFalse */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSDebugIncomingBody      /* kCFBooleanFalse */   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
/*
    Extra properties for SOAP messages.  These apply to the message
    namespace and format itself.  Individual message elements can
    be modified using the kWSRecord constants below.
*/
extern CFStringRef kWSSOAPBodyEncodingStyle   /* CFStringRef { kWSSOAPStyleDoc, kWSSOAPStyleRPC } */ API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSSOAPMethodNamespaceURI  /* CFStringRef */      API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSSOAPStyleDoc                                   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSSOAPStyleRPC                                   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
    For SOAP messages, this is an array of CFStringRefs which
    contain valid XML header elements that are sent with the
    message.  These are only applicable to the Header of a SOAP
    message.
*/
extern CFStringRef kWSSOAPMessageHeaders        /* CFArrayRef */     API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

/* 
    When serializing a record (dictionary) these keys present in
    the dictionary can modify the behavior of the serialization.
*/
extern CFStringRef kWSRecordParameterOrder      /* CFArrayRef of CFStringRef */ API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSRecordNamespaceURI        /* CFStringRef */    API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
extern CFStringRef kWSRecordType                /* CFStringRef */    API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
    Specifies that the result parameter will be found as this name.  This
    forces the deserializer to alias the named output parameter to kWSMethodInvocationResult
*/
extern CFStringRef kWSMethodInvocationResultParameterName            API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
/* 
    Specifies a timeout (as CFNumber) which specifies in seconds the
    amount of time to wait for the invocation to complete.  If the
    invocation times out before the server results are returned, 
    a fault will be returned with the error code errWSTimeoutError.  
*/
extern CFStringRef kWSMethodInvocationTimeoutValue                   API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );
#ifdef __cplusplus
}
#endif

#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint on
#endif


/*
 *  WSMethodInvocationRef
 *  
 *  Discussion:
 *    a WSMethodInvocationRef represents an object that can be executed
 *    to obtain a rsult from a web service. This is CFType and is
 *    therefore reference counted and and should be managed via
 *    CFRetain and CFRelease.
 */
typedef struct OpaqueWSMethodInvocationRef*  WSMethodInvocationRef;



/*
 *  WSMethodInvocationGetTypeID()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
WSMethodInvocationGetTypeID(void)                             API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationCreate()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Creates a web services method invocation object.  This object may
 *    be executed synchronously or scheduled on a run loop for
 *    asynchronous execution.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    url:
 *      the endpoint of the service
 *    
 *    methodName:
 *      the name of the method to be called
 *    
 *    protocol:
 *      a string, defined above, that determines the type of invocation
 *      object to create (XML-RPC vs. SOAP)
 *  
 *  Result:
 *    A WSMethodInvocationRef object that can be passed to
 *    WSMethodInvocationInvoke or scheduled with a run loop.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern WSMethodInvocationRef 
WSMethodInvocationCreate(
  CFURLRef      url,
  CFStringRef   methodName,
  CFStringRef   protocol)                                     API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationCreateFromSerialization()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Creates a web services method invocation object from a previously
 *    serialized contract.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    contract:
 *      the result of a previously serialized WSMethodInvocationRef
 *  
 *  Result:
 *    A WSMethodInvocationRef object that can be passed to
 *    WSMethodInvocationInvoke or scheduled with a run loop.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern WSMethodInvocationRef 
WSMethodInvocationCreateFromSerialization(CFDataRef contract) API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationCopySerialization()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Create a serialized version of the Method Invocation which can be
 *    reconstituted at a later time.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation to serialize
 *  
 *  Result:
 *    a CFDataRef
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDataRef 
WSMethodInvocationCopySerialization(WSMethodInvocationRef invocation) API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationSetParameters()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Set the parameters for a method invocation.  The parameterOrder
 *    may be NULL, in which case the order of th parameters is
 *    undefined.  If it is not NULL and the parameters dictionary
 *    contains more parameters than are specified by the order, the
 *    behavior is undefined.  If the parameterOrder specifies more
 *    parameters than are present in the dictionary, the result is
 *    undefined.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation object
 *    
 *    parameters:
 *      a CFDictionaryRef of CFString keys and CFTypeRef values.
 *    
 *    parameterOrder:
 *      a CFArrayRef of CFString parameter names.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSMethodInvocationSetParameters(
  WSMethodInvocationRef   invocation,
  CFDictionaryRef         parameters,
  CFArrayRef              parameterOrder)  /* can be NULL */  API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  WSMethodInvocationCopyParameters()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Copies the parameters from the invocation.  The resulting
 *    dictionary contains the parameter dictionary.  The parameterOrder
 *    output parameter, if not NULL, will contain the order used to
 *    serialize the parameters.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation
 *    
 *    parameterOrder:
 *      a pointer to a CFArray which will will receive the names, in
 *      their specified order, of the input parameter values.  This
 *      parameter may be NULL.
 *  
 *  Result:
 *    a CFDictionaryRef
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDictionaryRef 
WSMethodInvocationCopyParameters(
  WSMethodInvocationRef   invocation,
  CFArrayRef *            parameterOrder)  /* can be NULL */  API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  WSMethodInvocationSetProperty()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Add "properties" to a method invocation.  These properties can be
 *    user defined or one of the WebServicesCore declared properties
 *    (which may modify the behavior of the invocation.)  All
 *    WebServicesCore declared properties will start with the string
 *    "kWS", eg, kWSHTTPFollowsRedirects.  Properties are serialized
 *    along with the contract, so you may want to avoid sticking raw
 *    pointers in a CFNumber (for example).
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation
 *    
 *    propertyName:
 *      a CFStringRef name of the property to modify
 *    
 *    propertyValue:
 *      a CFTypeRef containing the new property value
 *  
 *  Result:
 *    none
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSMethodInvocationSetProperty(
  WSMethodInvocationRef   invocation,
  CFStringRef             propertyName,
  CFTypeRef               propertyValue)                      API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationCopyProperty()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Return a property from a invocation.  If the result is NULL, the
 *    property doesn't exist.  Being a "Copy" call, you must release
 *    the result.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation
 *    
 *    propertyName:
 *      the name of the property to retreive
 *  
 *  Result:
 *    the CFTypeRef value of the property, or NULL if the property was
 *    not specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeRef 
WSMethodInvocationCopyProperty(
  WSMethodInvocationRef   invocation,
  CFStringRef             propertyName)                       API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationInvoke()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Execute the invocation.  If the call was successful, the result
 *    will contain the result of the invocation. If for some reason the
 *    invocation failed, including out of memory or invalid parameter
 *    errors, then the result will contain a fault structure.  You must
 *    release the result when you're done with it.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation
 *  
 *  Result:
 *    a CFDictionaryRef containing the result of the execution or a
 *    fault, and optional debug information.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CF_RETURNS_RETAINED CFDictionaryRef 
WSMethodInvocationInvoke(WSMethodInvocationRef invocation)    API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
    These calls implemented the asynchronous variant of the WSMethodInvocationInvoke.
    
    The strategy is to schedule the invocation on a given runloop.
    When the invocation completes, it calls the specified callback with
    the result of the execution.  The callback is responsible for
    releasing the result ref.  Your code is responsible for
    unscheduling the invocation from the run loop, whether it completes
    or not.
    
    You can re-schedule an invocation after it completes.
    
    When you unschedule with the run loop, the CallBack is not called.
    
    If a network error occurs, the kWSFaultString entry of the result
    will contain some textual description of the error, kWSFaultCode
    will contain kWSNetworkingFault and kWSFaultExtra will be a
    dictionary containing two CFNumber values called kWSStreamErrorDomain
    and kWSStreamErrorError.
*/


/*
 *  WSMethodInvocationCallBackProcPtr
 *  
 *  Discussion:
 *    Prototypes the callback made when an asynchronous invocation
 *    completes.  This callback is passed a reference to the invocation
 *    just completed, a pointer to private data, and a dictionary that
 *    contains the return value or falut for this invocation.  The
 *    callback is responsible for releasing the dictionary when it is
 *    no longer used.
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation just completed
 *    
 *    info:
 *      private callback data
 *    
 *    outRef:
 *      a CFDictionaryRef containing the result of the execution or a
 *      fault, and optional debug information.
 *
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
typedef void(*WSMethodInvocationCallBackProcPtr)(WSMethodInvocationRef invocation, void *info, CFDictionaryRef outRef) API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

	
	
/*
 *  WSMethodInvocationSetCallBack()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    sets the callback for an asynchronous method invocation.  Call
 *    with a clientCB and context of NULL to clear the invocation
 *    callback.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation
 *    
 *    clientCB:
 *      a ProcPtr to be called when the invocation completes.
 *    
 *    context:
 *      a pointer to a WSClientContext.  The structure will be copied.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSMethodInvocationSetCallBack(
  WSMethodInvocationRef               invocation,
  WSMethodInvocationCallBackProcPtr   clientCB,
  WSClientContext *                   context)                API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationScheduleWithRunLoop()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Schedules the invocation to execute on the run loop.
 *
 *  Mac OS X threading:
 *    Thread safe
 *   
 *  Parameters:
 *    
 *    invocation:
 *      the invocation.
 *    
 *    runLoop:
 *      the run loop upon which to scheduile the invocation.
 *    
 *    runLoopMode:
 *      the run loop mode.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSMethodInvocationScheduleWithRunLoop(
  WSMethodInvocationRef   invocation,
  CFRunLoopRef            runLoop,
  CFStringRef             runLoopMode)                        API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSMethodInvocationUnscheduleFromRunLoop()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Unschedules the invocation from a given run loop and
 *    mode.  If the invocation has not yet completed,
 *    its callback will not be called.
 *
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation.
 *    
 *    runLoop:
 *      the run loop upon which to scheduile the invocation.
 *    
 *    runLoopMode:
 *      the run loop mode.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSMethodInvocationUnscheduleFromRunLoop(
  WSMethodInvocationRef   invocation,
  CFRunLoopRef            runLoop,
  CFStringRef             runLoopMode)                        API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
    Result interrogation.  

    If the result is a fault, look in the kWSFaultCode, kWSFaultString
    and kWSFaultExtra fields of the resulting dictionary.  If not a
    fault, kWSMethodInvocationResult will contain the result of the
    execution.  If debugging information was requested, it will be
    available in the dictionary as well.
*/
/*
 *  WSMethodResultIsFault()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    returns TRUE if the method invocation result contains a fault.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    methodResult:
 *      the result ref
 *  
 *  Result:
 *    TRUE if the result contains a fault condition
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
WSMethodResultIsFault(CFDictionaryRef methodResult)           API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
    Serialization / Deserialization override support.
    
    You can add serialization and deserialization callbacks for custom
    types, or types not otherwise handled by the framework.  Note that these
    properties are *not* serialized if the invocation is serialized.
 */


/*
 *  WSMethodInvocationSerializationProcPtr
 *  
 *  Discussion:
 *    Prototypes the callback function for a custom serialization proc.
 *     This callback is called whenever a type has the given CFTypeID. 
 *    The callback should return an XML snippet that will be understood
 *    by the server as a correct serialization for a given type.  If
 *    the callback returns NULL, the default serializer will be used.
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation currently being serialized
 *    
 *    obj:
 *      the CFTypeRef to be serialized
 *    
 *    info:
 *      private callback data
 *  
 *  Result:
 *    a CFStringRef containing valid XML.  The caller of this callback
 *    will release the string.
 *
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
typedef CFStringRef(*WSMethodInvocationSerializationProcPtr)(WSMethodInvocationRef invocation, CFTypeRef obj, void *info) API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

	
	
/*
 *  WSMethodInvocationAddSerializationOverride()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Specifies a callback which will be called to produce the XML that
 *    represents the serialization of a given type ref.  See
 *    WSDescription.h for a list of CFTypes for which there currently
 *    exist serializers.  If your callback returns NULL, the default
 *    serializer will be used.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation
 *    
 *    objType:
 *      the CFTypeID of the object
 *    
 *    serializationProc:
 *      the callback called
 *    
 *    context:
 *      a pointer to a WSClientContext.  The structure will be copied.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSMethodInvocationAddSerializationOverride(
  WSMethodInvocationRef                    invocation,
  CFTypeID                                 objType,
  WSMethodInvocationSerializationProcPtr   serializationProc,
  WSClientContext *                        context)           API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );




/*
 *  WSMethodInvocationDeserializationProcPtr
 *  
 *  Discussion:
 *    Prototypes the callback function for a custom deserializer.  This
 *    callback is passed a reference to the invocation currently being
 *    executed, the root of the response parse tree, the current node
 *    being deserialized, and a pointer to private data. The return
 *    result should be a valid CFTypeRef object (which will be released
 *    by the caller) or NULL to allow the default deserializer to act.
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation executing
 *    
 *    msgRoot:
 *      the root tree element
 *    
 *    deserializeRoot:
 *      the tree element that needs to be deserialied
 *    
 *    info:
 *      private callback data
 *  
 *  Result:
 *    a CFTypeRef representing the deserialized data, or NULL to allow
 *    the default deserializers to act.
 *
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
typedef CFTypeRef(*WSMethodInvocationDeserializationProcPtr)(WSMethodInvocationRef invocation, CFXMLTreeRef msgRoot, CFXMLTreeRef deserializeRoot, void *info) API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );

	
	
/*
 *  WSMethodInvocationAddDeserializationOverride()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Specifies a callback to be made when parsing an XML method
 *    response.  The callback should return a CFTypeRef containing the
 *    deserialized object value.  If the callback returns NULL, the
 *    default deserializer will be used.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    invocation:
 *      the invocation
 *    
 *    typeNamespace:
 *      the fully resolved namespace for a specific type.  If NULL, the
 *      default namespace will be used.  For example, this field could
 *      be: CFSTR("http://www.w3.org/2001/XMLSchema-instance").
 *    
 *    typeName:
 *      the non-qualified type name.  This parameter must not be NULL.
 *    
 *    deserializationProc:
 *      a ProcPtr to be called to perform the deserialization
 *    
 *    context:
 *      a pointer to a WSClientContext.  The structure will be copied.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSMethodInvocationAddDeserializationOverride(
  WSMethodInvocationRef                      invocation,
  CFStringRef                                typeNamespace,
  CFStringRef                                typeName,
  WSMethodInvocationDeserializationProcPtr   deserializationProc,
  WSClientContext *                          context)         API_DEPRECATED("No longer supported", macos(10.2,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );




#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __WSMETHODINVOCATION__ */

