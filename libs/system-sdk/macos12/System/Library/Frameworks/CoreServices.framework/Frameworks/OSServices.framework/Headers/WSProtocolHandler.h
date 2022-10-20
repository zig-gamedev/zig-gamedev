/*
     File:       OSServices/WSProtocolHandler.h
 
     Contains:   *** DEPRECATED *** WebServicesCore Method implementation API
 
     Copyright:  (c) 2002-2011 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __WSPROTOCOLHANDLER__
#define __WSPROTOCOLHANDLER__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __WSTYPES__
#include <OSServices/WSTypes.h>
#endif

/*
    WSProtocolHandler
*/

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
 *  WSProtocolHandlerRef
 *  
 *  Discussion:
 *    a WSProtocolHandlerRef represents an object that translates
 *    dictionaries into web services requests.  Typically, it is used
 *    to implement the server side of a web service by converting XML
 *    into CoreFoundation types, but it can also be used to produced
 *    serialzed web services requests without going through a
 *    WSMethodInvocation. It is created with a string specifying the
 *    web services protocol (XML-RPC or SOAP) and can also be modified
 *    through a set of external properties.
 */
typedef struct OpaqueWSProtocolHandlerRef*  WSProtocolHandlerRef;


/*
 *  WSProtocolHandlerGetTypeID()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
WSProtocolHandlerGetTypeID(void)                              API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  WSProtocolHandlerCreate()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Creates a WSProtocolHandlerRef for use in translating an XML
 *    document.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    allocator:
 *      a CFAllocatorRef used to allocate the protocol handler.
 *    
 *    protocol:
 *      a constant string, defiend in WSMethodInvocation.h, that
 *      determines the type of implementation to create (XML-RPC vs.
 *      SOAP).
 *  
 *  Result:
 *    a WSProtocolHandlerRef, NULL if a parse error occured.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern WSProtocolHandlerRef 
WSProtocolHandlerCreate(
  CFAllocatorRef   allocator,
  CFStringRef      protocol)                                  API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );




/*
 *  WSProtocolHandlerCopyRequestDictionary()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Parse an incoming XML document for the method name and
 *    parameters.  The results are in a dictionory as kWSMethodName (a
 *    CFString), kWSMethodParameters (a CFDict) and
 *    kWSMethodParameterOrder (a CFArray). If there was a parse error,
 *    NULL is returned. Protocol specific additions (eg,
 *    kWSSOAPMessageHeaders) may also be present in the dictionary. 
 *    The dictionary returned also represents the context with which
 *    XML reply documents are created (see
 *    WSProtocolHandlerCreateReply).  The caller must release the
 *    resulting dictionary.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      the protocol handler to use
 *    
 *    data:
 *      the XML document to parse
 *  
 *  Result:
 *    a CFDictionary
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDictionaryRef 
WSProtocolHandlerCopyRequestDictionary(
  WSProtocolHandlerRef   ref,
  CFDataRef              data)                                API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSProtocolHandlerCopyReplyDictionary()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Parse an incoming XML document as if it were the reply of a
 *    method.  The results are the same as the WSMethodInvocationInvoke
 *    response; that is, the reply could be a fault.  If there was a
 *    parse error, NULL is returned.  Protocol specific additions (eg,
 *    kWSSOAPMessageHeaders) may also be present in the dictionary. 
 *    The caller must release the resulting dictionary.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      the protocol handler to use
 *    
 *    methodName:
 *      the method name corrosponding to this result
 *    
 *    data:
 *      the XML document to parse
 *  
 *  Result:
 *    a CFDictionary
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDictionaryRef 
WSProtocolHandlerCopyReplyDictionary(
  WSProtocolHandlerRef   ref,
  CFStringRef            methodName,
  CFDataRef              data)                                API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  WSProtocolHandlerCopyReplyDocument()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Creates a Reply XML document for a given WSProtocolHandler and
 *    context dictionary.  Protocol specific addtions (eg,
 *    kWSSOAPMessageHeaders) may also be present in the dictionary.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      the WSProtocolHandler to respond
 *    
 *    methodContext:
 *      the CFDictionary containing the context for this method call,
 *      as returned by WSProtocolHandlerParseRequest
 *    
 *    resultValue:
 *      a CFTypeRef representing the reply data to be serialized
 *  
 *  Result:
 *    a CFDataRef containing the XML response
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDataRef 
WSProtocolHandlerCopyReplyDocument(
  WSProtocolHandlerRef   ref,
  CFDictionaryRef        methodContext,
  CFTypeRef              resultValue)                         API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  WSProtocolHandlerCopyFaultDocument()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Creates a Fault XML response for a given WSProtocolHandler and
 *    fault details dictionary
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      the WSProtocolHandler
 *    
 *    methodContext:
 *      the CFDictionary containing the context for this method call,
 *      as returned by WSProtocolHandlerParseRequest
 *    
 *    faultDict:
 *      a CFDictionary containing the fault information.  See
 *      WSMethodInvocation.h for valid keys.
 *  
 *  Result:
 *    a CFDataRef containing the XML fault
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDataRef 
WSProtocolHandlerCopyFaultDocument(
  WSProtocolHandlerRef   ref,
  CFDictionaryRef        methodContext,
  CFDictionaryRef        faultDict)                           API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  WSProtocolHandlerCopyRequestDocument()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Creates an XML request for a given WSProtocolHandler and
 *    parameter list.  This is the request sent to a server.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      the WSProtocolHandler
 *    
 *    methodName:
 *      a CFString of the method name to call
 *    
 *    methodParams:
 *      a CFDictionary containing the parameters to send
 *    
 *    methodParamOrder:
 *      a CFArray, which, if not NULL, specifies the order of the
 *      parameters in the CFDictionary
 *    
 *    methodExtras:
 *      a CFDictionary, which, if not NULL, contains additional
 *      information for the protocol (eg, kWSSoapMessageHeaders)
 *  
 *  Result:
 *    a CFDataRef
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDataRef 
WSProtocolHandlerCopyRequestDocument(
  WSProtocolHandlerRef   ref,
  CFStringRef            methodName,
  CFDictionaryRef        methodParams,
  CFArrayRef             methodParamOrder,
  CFDictionaryRef        methodExtras)                        API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  WSProtocolHandlerCopyProperty()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    returns a property from a protocol handler.  If the result is
 *    NULL, the property doesn't exist.  Being a "Copy" call, you must
 *    release the result.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      the implementation
 *    
 *    propertyName:
 *      the name of the property to retreive
 *  
 *  Result:
 *    the CFTypeRef value of the property, or NULL if the property was
 *    not specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeRef 
WSProtocolHandlerCopyProperty(
  WSProtocolHandlerRef   ref,
  CFStringRef            propertyName)                        API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  WSProtocolHandlerSetProperty()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Sets a property to a method implementation.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    ref:
 *      the implementation
 *    
 *    propertyName:
 *      the name of the property to retreive
 *    
 *    propertyValue:
 *      the value to set in the method implementation
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSProtocolHandlerSetProperty(
  WSProtocolHandlerRef   ref,
  CFStringRef            propertyName,
  CFTypeRef              propertyValue)                       API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
    Serialization / Deserialization override support.
    
    You can add serialization and deserialization callbacks for custom
    types, or types not otherwise handled by the framework. 
 */


/*
 *  WSProtocolHandlerSerializationProcPtr
 *  
 *  Discussion:
 *    Prototypes the callback function for a custom serialization proc.
 *     This callback is called whenever a type has the given CFTypeID. 
 *    The callback should return an XML snippet that will be understood
 *    by the server as a correct serialization for a given type.  If
 *    the callback returns NULL, the default serializer will be used.
 *    Note for for SOAP serializations, the parameter key (element
 *    name) is not part of the callback; it will be substituded for all
 *    occurances of "%@" in the returned string.
 *  
 *  Parameters:
 *    
 *    protocol:
 *      the protocol currently being serialized
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
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
typedef CFStringRef(*WSProtocolHandlerSerializationProcPtr)(WSProtocolHandlerRef protocol, CFTypeRef obj, void *info)API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );

	
	
/*
 *  WSProtocolHandlerSetSerializationOverride()   *** DEPRECATED ***
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
 *    protocol:
 *      the protocol
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
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSProtocolHandlerSetSerializationOverride(
  WSProtocolHandlerRef                    protocol,
  CFTypeID                                objType,
  WSProtocolHandlerSerializationProcPtr   serializationProc,
  WSClientContext *                       context)            API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );




/*
 *  WSProtocolHandlerDeserializationProcPtr
 *  
 *  Discussion:
 *    Prototypes the callback function for a custom deserializer.  This
 *    callback is passed a reference to the protocol element currently
 *    being executed, the root of the response parse tree, the current
 *    node being deserialized, and a pointer to private data. The
 *    return result should be a valid CFTypeRef object (which will be
 *    released by the caller) or NULL to allow the default deserializer
 *    to act.
 *  
 *  Parameters:
 *    
 *    protocol:
 *      the protocol executing
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
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
typedef CFTypeRef(*WSProtocolHandlerDeserializationProcPtr)(WSProtocolHandlerRef protocol, CFXMLTreeRef msgRoot, CFXMLTreeRef deserializeRoot, void *info)API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );

	
	
/*
 *  WSProtocolHandlerSetDeserializationOverride()   *** DEPRECATED ***
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
 *    protocol:
 *      the protocol
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
 *    Mac OS X:         in version 10.3 and later but deprecated in 10.8
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
WSProtocolHandlerSetDeserializationOverride(
  WSProtocolHandlerRef                      protocol,
  CFStringRef                               typeNamespace,
  CFStringRef                               typeName,
  WSProtocolHandlerDeserializationProcPtr   deserializationProc,
  WSClientContext *                         context)          API_DEPRECATED("No longer supported", macos(10.3,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );




/*
 * Properties in the Request Dictionary
 */
#define kWSMethodName                   CFSTR("/WSMethodName")
#define kWSMethodParameters             CFSTR("/WSMethodParameters")
#define kWSMethodParameterOrder         CFSTR("/WSMethodParameterOrder")

#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __WSPROTOCOLHANDLER__ */

