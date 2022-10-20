/*
	 File:	   CFNetwork/CFHTTPMessage.h
 
	 Contains:   CoreFoundation Network socket streams header
 
	 Copyright:  Copyright (c) 2001-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFHTTPMESSAGE__
#define __CFHTTPMESSAGE__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __CFSTRING__
#include <CoreFoundation/CFString.h>
#endif

#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif



#if PRAGMA_ONCE
#pragma once
#endif

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*
 *  kCFHTTPVersion1_0
 *  
 *  Discussion:
 *	Version string for HTTP 1.0.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPVersion1_0 CF_AVAILABLE(10_1, 2_0);

/*
 *  kCFHTTPVersion1_1
 *  
 *  Discussion:
 *	Version string for HTTP 1.1.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPVersion1_1 CF_AVAILABLE(10_1, 2_0);

/*
 *  kCFHTTPVersion2_0
 *
 *  Discussion:
 *	Version string for HTTP 2.0.
 *
 */
CFN_EXPORT const CFStringRef kCFHTTPVersion2_0 CF_AVAILABLE(10_10, 8_0);

/*
 *  kCFHTTPVersion3_0
 *
 *  Discussion:
 *	Version string for HTTP 3.0.
 *
 */
CFN_EXPORT const CFStringRef kCFHTTPVersion3_0 CF_AVAILABLE(10_15, 13_0);

/*
 *  kCFHTTPAuthenticationSchemeBasic
 *  
 *  Discussion:
 *	HTTP Basic authentication scheme.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationSchemeBasic CF_AVAILABLE(10_2, 2_0);

/*
 *  kCFHTTPAuthenticationSchemeDigest
 *  
 *  Discussion:
 *	HTTP Digest Access authentication scheme.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationSchemeDigest CF_AVAILABLE(10_2, 2_0);

/*
 *  kCFHTTPAuthenticationSchemeNTLM
 *  
 *  Discussion:
 *	HTTP NTLM authentication scheme.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationSchemeNTLM CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFHTTPAuthenticationSchemeKerberos
 *
 *  Discussion:
 *	HTTP Negotiate authentication scheme.
 *
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationSchemeKerberos CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFHTTPAuthenticationSchemeNegotiate
 *  
 *  Discussion:
 *	HTTP Negotiate authentication scheme.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationSchemeNegotiate CF_AVAILABLE(10_5, 2_0);
	
/*
 *  kCFHTTPAuthenticationSchemeNegotiate2
 *  
 *  Discussion:
 *	HTTP Negotiate v2 authentication scheme.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationSchemeNegotiate2 CF_AVAILABLE(10_6, 3_0);
	
/*
 *  kCFHTTPAuthenticationSchemeXMobileMeAuthToken
 *  
 *  Discussion:
 *	HTTP XMobileMeAuthToken authentication scheme.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationSchemeXMobileMeAuthToken CF_AVAILABLE(10_6, 4_3);

/*
 *  CFHTTPMessageRef
 *  
 *  Discussion:
 *	This is the type of a reference to an HTTP message. An HTTP
 *	message can be a request or a response.
 */
typedef struct __CFHTTPMessage*		 CFHTTPMessageRef;

/*
 *  CFHTTPMessageGetTypeID()
 *  
 *  Discussion:
 *	Return the unique type for this class.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Result:
 *	A unique CFType for CFHTTPMessage.
 *  
 */
CFN_EXPORT CFTypeID 
CFHTTPMessageGetTypeID(void) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCreateRequest()
 *  
 *  Discussion:
 *	Create an HTTPMessage from an HTTP method, url and version.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	requestMethod:
 *	  A pointer to a CFString indicating the method of request. For a
 *	  "GET" request, for example, the value would be CFSTR("GET").
 *	
 *	url:
 *	  A pointer to a CFURL structure created any of the several
 *	  CFURLCreate... functions.  If this parameter is not a pointer
 *	  to a valid CFURL structure, the behavior is undefined.
 *	
 *	httpVersion:
 *	  A pointer to a CFString indicating the version of request.
 *  
 *  Result:
 *	A pointer to the CFHTTPMessage created, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	message.
 *  
 */
CFN_EXPORT CFHTTPMessageRef 
CFHTTPMessageCreateRequest(CFAllocatorRef __nullable alloc, CFStringRef requestMethod, CFURLRef url, CFStringRef httpVersion) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCreateResponse()
 *  
 *  Discussion:
 *	Create an HTTPMessage from an HTTP status code, description and
 *	version.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	statusCode:
 *	  An integer status code for the response.
 *	
 *	statusDescription:
 *	  A pointer to a CFString for the status. Pass NULL to use the
 *	  standard description for the given status code, as found in RFC
 *	  2616.
 *	
 *	httpVersion:
 *	  A pointer to a CFString for the HTTP version.
 *  
 *  Result:
 *	A pointer to the CFHTTPMessage created, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	message.
 *  
 */
CFN_EXPORT CFHTTPMessageRef 
CFHTTPMessageCreateResponse(
  CFAllocatorRef  __nullable alloc,
  CFIndex         statusCode,
  CFStringRef     __nullable statusDescription,
  CFStringRef     httpVersion)              CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCreateEmpty()
 *  
 *  Discussion:
 *	Creates an empty request or response, which you can then append
 *	bytes to via CFHTTPMessageAppendBytes().
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	isRequest:
 *	  A boolean. Pass kCFBooleanTrue if the message should be a
 *	  request.
 *  
 *  Result:
 *	A pointer to the CFHTTPMessage created, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	message.
 *  
 */
CFN_EXPORT CFHTTPMessageRef 
CFHTTPMessageCreateEmpty(CFAllocatorRef __nullable alloc, Boolean isRequest) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCreateCopy()
 *  
 *  Discussion:
 *	Creates a copy of a CFHTTPMessage.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A pointer to the CFHTTPMessage created, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	message.
 *  
 */
CFN_EXPORT CFHTTPMessageRef 
CFHTTPMessageCreateCopy(CFAllocatorRef __nullable alloc, CFHTTPMessageRef message) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageIsRequest()
 *  
 *  Discussion:
 *	Returns whether the CFHTTPMessage is a request or a response.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A Boolean. A value of kCFBooleanTrue indicates the message is a
 *	request. A value of kCFBooleanFalse indicates the message is a
 *	response.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPMessageIsRequest(CFHTTPMessageRef message) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCopyVersion()
 *  
 *  Discussion:
 *	Returns the HTTP version.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A pointer to a CFString, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the string.
 *  
 */
CFN_EXPORT CFStringRef 
CFHTTPMessageCopyVersion(CFHTTPMessageRef message) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCopyBody()
 *  
 *  Discussion:
 *	Returns the body of the message.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A pointer to a CFData, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the data.
 *  
 */
CFN_EXPORT __nullable CFDataRef
CFHTTPMessageCopyBody(CFHTTPMessageRef message) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageSetBody()
 *  
 *  Discussion:
 *	Sets the body of the message from a CFData.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *	
 *	bodyData:
 *	  A pointer to a CFData containing the body to be set. If the
 *	  bodyData is NULL, the behavior is undefined.
 *  
 */
CFN_EXPORT void 
CFHTTPMessageSetBody(CFHTTPMessageRef message, CFDataRef bodyData) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCopyHeaderFieldValue()
 *  
 *  Discussion:
 *	Returns the specified header field.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *	
 *	headerField:
 *	  A pointer to the CFString. If the headerField is NULL, the
 *	  behavior is undefined.
 *  
 *  Result:
 *	A pointer to a CFString, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the string.
 *  
 */
CFN_EXPORT __nullable CFStringRef
CFHTTPMessageCopyHeaderFieldValue(CFHTTPMessageRef message, CFStringRef headerField) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCopyAllHeaderFields()
 *  
 *  Discussion:
 *	Returns a CFDictionary containing all of the header fields.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A pointer to a CFDictionary, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the dictionary.
 *  
 */
CFN_EXPORT __nullable CFDictionaryRef
CFHTTPMessageCopyAllHeaderFields(CFHTTPMessageRef message) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageSetHeaderFieldValue()
 *  
 *  Discussion:
 *	Sets the value of the specified header field.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *	
 *	headerField:
 *	  A pointer to the CFString. If headerField is NULL, the behavior
 *	  is undefined.
 *	
 *	value:
 *	  A pointer to the CFString containing the value to set. Set the
 *	  value to NULL to remove the header field.
 *  
 */
CFN_EXPORT void 
CFHTTPMessageSetHeaderFieldValue(CFHTTPMessageRef message, CFStringRef headerField, CFStringRef __nullable value) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageAppendBytes()
 *  
 *  Discussion:
 *	Appends the given bytes to the message given (parsing out any
 *	control information if appropriate).  Returns kCFBooleanFalse if
 *	a parsing error occurs while processing the new data.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *	
 *	newBytes:
 *	  A pointer to the bytes. If newBytes is NULL, the behavior is
 *	  undefined.
 *	
 *	numBytes:
 *	  A CFIndex of the number of bytes to append.
 *  
 *  Result:
 *	A Boolean indicating success or failure.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPMessageAppendBytes(CFHTTPMessageRef message, const UInt8 *newBytes, CFIndex numBytes) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageIsHeaderComplete()
 *  
 *  Discussion:
 *	Returns whether further header data is expected by the message.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	message:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A Boolean. A value of kCFBooleanTrue indicates the header is
 *	complete and no further data is expected.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPMessageIsHeaderComplete(CFHTTPMessageRef message) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCopySerializedMessage()
 *  
 *  Discussion:
 *	Creates a self-contained copy of a CFHTTPMessage. This would be
 *	suitable for persistant storage or for transmitting over the
 *	network independently.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	request:
 *	  A pointer to the CFHTTPMessage to be seralized.
 *  
 *  Result:
 *	A pointer to a CFData, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the data.
 *  
 */
CFN_EXPORT __nullable CFDataRef
CFHTTPMessageCopySerializedMessage(CFHTTPMessageRef message) CF_AVAILABLE(10_1, 2_0);


/*********************/
/* Request functions */
/*********************/

/*
 *  CFHTTPMessageCopyRequestURL()
 *  
 *  Discussion:
 *	Creates a copy of the request URL.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	request:
 *	  A pointer to the CFHTTPMessage.
 *  
 *  Result:
 *	A pointer to a CFURL, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the url.
 *  
 */
CFN_EXPORT __nullable CFURLRef
CFHTTPMessageCopyRequestURL(CFHTTPMessageRef request) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCopyRequestMethod()
 *  
 *  Discussion:
 *	Creates a copy of the request method.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	request:
 *	  A pointer to the CFHTTPMessage.
 *  
 *  Result:
 *	A pointer to a CFString, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the string.
 *  
 */
CFN_EXPORT __nullable CFStringRef
CFHTTPMessageCopyRequestMethod(CFHTTPMessageRef request) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageAddAuthentication()
 *  
 *  Discussion:
 *	Adds authentication to the request. Tries to modify request to
 *	contain the authentication information requested by the failed
 *	response (which presumably is a 401 or 407 response).
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	request:
 *	  A pointer to a CFHTTPMessage request.
 *	
 *	authenticationFailureResponse:
 *	  A pointer to a CFHTTPMessage of the failed response.
 *	
 *	username:
 *	  A pointer to a CFString containing the user name to
 *	  authenticate.
 *	
 *	password:
 *	  A pointer to a CFString containing the password of the user.
 *	
 *	authenticationScheme:
 *	  A pointer to a CFString containing the authentication scheme to
 *	  use to authenticate. If authenticationScheme is NULL, strongest
 *	  supported scheme listed authenticationFailureResponse will be
 *	  used.
 *	
 *	forProxy:
 *	  A boolean indicating whether the authentication applies to a
 *	  proxy or not.
 *  
 *  Result:
 *	A pointer to a CFString, or NULL if failed. It is caller's
 *	responsibilty to release the memory allocated for the string.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPMessageAddAuthentication(
  CFHTTPMessageRef   request,
  CFHTTPMessageRef   __nullable authenticationFailureResponse,
  CFStringRef        username,
  CFStringRef        password,
  CFStringRef        __nullable authenticationScheme,
  Boolean            forProxy)              CF_AVAILABLE(10_1, 2_0);


/**********************/
/* Response functions */
/**********************/

/*
 *  CFHTTPMessageGetResponseStatusCode()
 *  
 *  Discussion:
 *	Returns the status code for the response.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	response:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A UInt32 indicating the status code.
 *  
 */
CFN_EXPORT CFIndex 
CFHTTPMessageGetResponseStatusCode(CFHTTPMessageRef response) CF_AVAILABLE(10_1, 2_0);


/*
 *  CFHTTPMessageCopyResponseStatusLine()
 *  
 *  Discussion:
 *	Returns the status line for the response.
 *  
 *  Mac OS X threading:
 *	Not thread safe
 *  
 *  Parameters:
 *	
 *	response:
 *	  A pointer to the CFHTTPMessage to be copied. If the message is
 *	  NULL, the behavior is undefined.
 *  
 *  Result:
 *	A CFString indicating the status code, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	string.
 *  
 */
CFN_EXPORT __nullable CFStringRef
CFHTTPMessageCopyResponseStatusLine(CFHTTPMessageRef response) CF_AVAILABLE(10_1, 2_0);



CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFHTTPMESSAGE__ */

