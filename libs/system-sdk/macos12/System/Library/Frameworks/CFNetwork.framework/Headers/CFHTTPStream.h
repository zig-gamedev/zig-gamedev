/*
	 File:	   CFNetwork/CFHTTPStream.h
 
	 Contains:   CoreFoundation Network HTTP streams header
 
	 Copyright:  Copyright (c) 2001-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFHTTPSTREAM__
#define __CFHTTPSTREAM__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __CFSTREAM__
#include <CoreFoundation/CFStream.h>
#endif

#ifndef __CFHTTPMESSAGE__
#include <CFNetwork/CFHTTPMessage.h>
#endif



#if PRAGMA_ONCE
#pragma once
#endif

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

#if PRAGMA_ENUM_ALWAYSINT
	#pragma enumsalwaysint on
#endif

/*
 *  kCFStreamErrorDomainHTTP
 *  
 *  Discussion:
 *	Result code returned by HTTP server
 *  
 */
CFN_EXPORT const SInt32 kCFStreamErrorDomainHTTP CF_AVAILABLE(10_1, 2_0);

/*
 *  CFStreamErrorHTTP
 *  
 *  Discussion:
 *	Errors from the kCFStreamErrorDomainHTTP domain.
 */
typedef CF_ENUM(int, CFStreamErrorHTTP) {

  /*
   * Could not parse the request/response.
   */
  kCFStreamErrorHTTPParseFailure = -1,

  /*
   * A loop was detected during redirection.
   */
  kCFStreamErrorHTTPRedirectionLoop = -2,

  /*
   * Could not retreive url for request/response.
   */
  kCFStreamErrorHTTPBadURL	  = -3
};

/*
 *  kCFStreamPropertyHTTPResponseHeader
 *  
 *  Discussion:
 *	Stream property key, for copy operations. Value is a
 *	CFHTTPMessage with 0 bytes data.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPResponseHeader CF_DEPRECATED(10_1, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");


/*
 *  kCFStreamPropertyHTTPFinalURL   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Stream property key, for copy operations. The response header
 *	value is the CFURL from the final request; will only differ from
 *	the URL in the original request if an autoredirection has
 *	occurred.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPFinalURL CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");

/*
 *  kCFStreamPropertyHTTPFinalRequest   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Stream property key, for copy operations. The value is the
 *	CFHTTPMessage transmitted by the stream, after all modifications
 *	(such as for authentication, connection policy, or redirection)
 *	have been made.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPFinalRequest CF_DEPRECATED(10_5, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");

/*
 *  kCFStreamPropertyHTTPProxy   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Stream property key, for both set and copy operations. The value
 *	is a CFDictionary. HTTP proxy information is set the same way as
 *	SOCKS proxies (see CFSocketStream.h). Call
 *	CFReadStreamSetProperty() passing an HTTP stream and the property
 *	kCFStreamPropertyHTTPProxy. The value should include at least one
 *	Host/Port pair from the keys below. Use the dictionary returned
 *	by SystemConfiguration.framework to set the default values for
 *	the system. HTTP proxies are not applied automatically.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPProxy CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");


/*
 *  kCFStreamPropertyHTTPProxyHost   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Proxy dictionary key. The hostname of an HTTP proxy. The value is
 *	a CFString. The key name matches kSCPropNetProxiesHTTPProxy.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPProxyHost CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");


/*
 *  kCFStreamPropertyHTTPProxyPort   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Proxy dictionary key. Value is a CFNumber.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPProxyPort CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");
/* matches kSCPropNetProxiesHTTPPort */


/*
 *  kCFStreamPropertyHTTPSProxyHost   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Proxy dictionary key. Value is a CFString.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPSProxyHost CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");
/* matches kSCPropNetProxiesHTTPSProxy */


/*
 *  kCFStreamPropertyHTTPSProxyPort   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Proxy dictionary key. Value is a CFNumber.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPSProxyPort CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");
/* matches kSCPropNetProxiesHTTPSPort */


/*
 *  kCFStreamPropertyHTTPShouldAutoredirect   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Stream property key. Value is a CFBoolean. Redirection is not
 *	performed by default.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPShouldAutoredirect CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");


/*
 *  kCFStreamPropertyHTTPAttemptPersistentConnection   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Stream property key. Value is a CFBoolean.  If this property is
 *	set to kCFBooleanTrue, an HTTP stream will look for an
 *	appropriate extant persistent connection to use, and if it finds
 *	none, will try to create one. Persistent connections are not used
 *	by default.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPAttemptPersistentConnection CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");


/*
 *  kCFStreamPropertyHTTPRequestBytesWrittenCount   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Stream property key. Value is a CFNumber. This property can only
 *	be retrieved, not set. The number returned is the number of bytes
 *	from the body of the request that have been written to the
 *	underlying socket
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyHTTPRequestBytesWrittenCount CF_DEPRECATED(10_3, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");


/*********************/
/* Creation routines */
/*********************/
/*
 *  CFReadStreamCreateForHTTPRequest()   *** DEPRECATED ***
 *  
 *  DEPRECATED: Use NSURLSession API for http requests
 *
 *  Discussion:
 *	Create an HTTP read stream for the response to the given request.
 *	When the stream is opened, it will begin transmitting the
 *	request. The bytes returned are the pure body bytes; the response
 *	header has been parsed off. To retrieve the response header, ask
 *	for kCFStreamPropertyHTTPResponseHeader, above, any time after
 *	the first bytes arrive on the stream (or when stream end is
 *	reported, if there are no data bytes). When an HTTP/1.1 server
 *	returns a chunked a response, the chunks will be formed into one
 *	continuous stream.
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	request:
 *	  A pointer to a CFHTTPMessage created by the
 *	  CFHTTPMessageCreateRequest function.
 *  
 *  Result:
 *	A pointer to the CF read stream created, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	read stream.
 *  
 */
CFN_EXPORT CFReadStreamRef 
CFReadStreamCreateForHTTPRequest(CFAllocatorRef __nullable alloc, CFHTTPMessageRef request) CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");


/*
 *  CFReadStreamCreateForStreamedHTTPRequest()   *** DEPRECATED ***
 *  
 *  DEPRECATED: Use NSURLSession API for http requests
 *
 *  Discussion:
 *	Creates a read stream for the response to the given
 *	requestHeaders plus requestBody. Use in preference to
 *	CFReadStreamCreateForHTTPRequest() when the body of the request
 *	is larger than you wish to be resident in memory.  Note that
 *	because streams cannot be reset, read streams created this way
 *	cannot have autoredirection enabled.  If the Content-Length
 *	header is set in requestHeaders, it is assumed that the caller
 *	got the length right and that requestBody will report
 *	end-of-stream after precisely Content-Length bytes have been read
 *	from it. If the Content-Length header is not set, the chunked
 *	transfer-encoding will be added to requestHeaders, and bytes read
 *	from requestBody will be transmitted chunked. The body of
 *	requestHeaders is ignored.
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  A pointer to the CFAllocator which should be used to allocate
 *	  memory for the CF read stream and its storage for values. If
 *	  this reference is not a valid CFAllocator, the behavior is
 *	  undefined.
 *	
 *	requestHeaders:
 *	  A pointer to a CFHTTPMessage created by the
 *	  CFHTTPMessageCreateRequest function. The body of requestHeaders
 *	  is ignored.
 *	
 *	requestBody:
 *	  A pointer to a CFReadStream.
 *  
 *  Result:
 *	A pointer to the CF read stream created, or NULL if failed. It is
 *	caller's responsibilty to release the memory allocated for the
 *	read stream.
 *  
 */
CFN_EXPORT CFReadStreamRef 
CFReadStreamCreateForStreamedHTTPRequest(CFAllocatorRef __nullable alloc, CFHTTPMessageRef requestHeaders, CFReadStreamRef requestBody) CF_DEPRECATED(10_2, 10_11, 2_0, 9_0, "Use NSURLSession API for http requests");

#if PRAGMA_ENUM_ALWAYSINT
	#pragma enumsalwaysint reset
#endif

CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFHTTPSTREAM__ */

