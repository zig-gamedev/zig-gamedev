/*
	 File:	   CFNetwork/CFHTTPAuthentication.h
 
	 Contains:   CoreFoundation Network HTTP authentication header
 
	 Copyright:  Copyright (c) 2001-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFHTTPAUTHENTICATION__
#define __CFHTTPAUTHENTICATION__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __CFHTTPMESSAGE__
#include <CFNetwork/CFHTTPMessage.h>
#endif



#if PRAGMA_ONCE
#pragma once
#endif

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*
 *  CFHTTPAuthenticationRef
 *  
 *  Discussion:
 *	This is the type of a reference to HTTP authentication
 *	information.
 */
typedef struct _CFHTTPAuthentication*   CFHTTPAuthenticationRef;

/*
 *  CFStreamErrorHTTPAuthentication
 *  
 *  Discussion:
 *	Authentication errors which may be returned as a result of trying
 *	to apply authentication to a request.  These errors are in the
 *	kCFStreamErrorDomainHTTP domain.
 */
typedef CF_ENUM(int, CFStreamErrorHTTPAuthentication) {

  /*
   * The type of authentication to be applied to a request is not
   * supported.
   */
  kCFStreamErrorHTTPAuthenticationTypeUnsupported = -1000,

  /*
   * The username was in a format not suitable for applying to the
   * request.
   */
  kCFStreamErrorHTTPAuthenticationBadUserName = -1001,

  /*
   * The password was in a format not suitable for applying to the
   * request.
   */
  kCFStreamErrorHTTPAuthenticationBadPassword = -1002
};


/*
 *  kCFHTTPAuthenticationUsername
 *  
 *  Discussion:
 *	CFDictionary key, for CFHTTPMessageApplyCredentialDictionary. The
 *	username for authentication as a CFString.  Needs to be added if
 *	CFHTTPAuthenticationRequiresUserNameAndPassword returns TRUE.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationUsername CF_AVAILABLE(10_4, 2_0);

/*
 *  kCFHTTPAuthenticationPassword
 *  
 *  Discussion:
 *	CFDictionary key, for CFHTTPMessageApplyCredentialDictionary. The
 *	password for authentication as a CFString.  Needs to be added if
 *	CFHTTPAuthenticationRequiresUserNameAndPassword returns TRUE.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationPassword CF_AVAILABLE(10_4, 2_0);

/*
 *  kCFHTTPAuthenticationAccountDomain
 *  
 *  Discussion:
 *	CFDictionary key, for CFHTTPMessageApplyCredentialDictionary. The
 *	domain for authentication as a CFString.  Needs to be added if
 *	CFHTTPAuthenticationRequiresAccountDomain returns TRUE.
 *  
 */
CFN_EXPORT const CFStringRef kCFHTTPAuthenticationAccountDomain CF_AVAILABLE(10_4, 2_0);

/*
 *  CFHTTPAuthenticationGetTypeID()
 *  
 *  Discussion:
 *	Returns the type identifier of all CFHTTPAuthentication instances.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 */
CFN_EXPORT CFTypeID 
CFHTTPAuthenticationGetTypeID(void) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPAuthenticationCreateFromResponse()
 *  
 *  Discussion:
 *	Based on a response of 401 or 407, this function will create a
 *	new authentication object which can be used for adding
 *	credentials to future requests.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  Allocator to use for creating authentication object
 *	
 *	response:
 *	  Failed response.
 *  
 *  Result:
 *	A freshly created authentication object useful for applying
 *	authentication credentials to new requests.
 *  
 */
CFN_EXPORT CFHTTPAuthenticationRef 
CFHTTPAuthenticationCreateFromResponse(CFAllocatorRef __nullable alloc, CFHTTPMessageRef response) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPAuthenticationIsValid()
 *  
 *  Discussion:
 *	Returns TRUE if the given authentication information was
 *	instantiated correctly and contains enough information in order
 *	to be applied to a request. If FALSE is returned, the object may
 *	still contain information which is useful to the user, e.g.
 *	unsupported method name.  An invalid object may be queried for
 *	information but may not be applied to a request.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *	
 *	error:
 *	  Reference to a CFStreamError which will be populated in the
 *	  case of an error in creation.  Pass NULL if not interested in
 *	  the failure reason.  The error domain will be
 *	  kCFStreamErrorDomainHTTP, and the error code will be one of
 *	  those defined in CFHTTPStream.h or one of those listed as
 *	  CFStreamErrorHTTPAuthentication.
 *  
 *  Result:
 *	TRUE or FALSE depending on whether the authentication object is
 *	good for applying credentials to further requests.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPAuthenticationIsValid(CFHTTPAuthenticationRef auth, CFStreamError * __nullable error) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPAuthenticationAppliesToRequest()
 *  
 *  Discussion:
 *	Returns TRUE if the given request requires credentials based upon
 *	the given authentication information.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *	
 *	request:
 *	  The request which is believed to need the given authentication.
 *  
 *  Result:
 *	TRUE if the given authentication information should be applied to
 *	the request, otherwise FALSE is returned.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPAuthenticationAppliesToRequest(CFHTTPAuthenticationRef auth, CFHTTPMessageRef request) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPAuthenticationRequiresOrderedRequests()
 *  
 *  Discussion:
 *	Some authentication methods require that future requests must be
 *	performed in an ordered manner, so that information from a
 *	response can be added to a following request.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *  
 *  Result:
 *	Returns TRUE if the given authentication method requires ordered
 *	requests.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPAuthenticationRequiresOrderedRequests(CFHTTPAuthenticationRef auth) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPMessageApplyCredentials()
 *  
 *  Discussion:
 *	Perform the authentication method required on the request using
 *	the given username and password.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	request:
 *	  The request which is to receive the credentials.
 *	
 *	auth:
 *	  The authentication information for the given request.
 *	
 *	username:
 *	  The username to use for performing the authentication.
 *	
 *	password:
 *	  The password to use for performing the authentication.
 *	
 *	error:
 *	  Reference to a CFStreamError which will be populated with the
 *	  error information should one occurr during the application of
 *	  the credentials. Pass NULL if not interested in the failure
 *	  reason.  The error domain will be kCFStreamErrorDomainHTTP, and
 *	  the error code will be one of those define in CFHTTPStream.h or
 *	  one of those listed as CFStreamErrorHTTPAuthentication.
 *  
 *  Result:
 *	TRUE will be returned if the application of the credentials to
 *	the request was successful, otherwise FALSE is returned.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPMessageApplyCredentials(
  CFHTTPMessageRef				request,
  CFHTTPAuthenticationRef		auth,
  CFStringRef       __nullable	username,
  CFStringRef       __nullable	password,
  CFStreamError *   __nullable	error)          CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPMessageApplyCredentialDictionary()
 *  
 *  Discussion:
 *	Perform the authentication method required on the request using
 *	the given credential information.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	request:
 *	  The request which is to receive the credentials.
 *	
 *	auth:
 *	  The authentication information for the given request.
 *	
 *	dict:
 *	  A dictionary containing credentials to be applied to the
 *	  request.  Valid keys are declared above.
 *	
 *	error:
 *	  Reference to a CFStreamError which will be populated with the
 *	  error information should one occurr during the application of
 *	  the credentials. Pass NULL if not interested in the failure
 *	  reason.  The error domain will be kCFStreamErrorDomainHTTP, and
 *	  the error code will be one of those define in CFHTTPStream.h or
 *	  one of those listed as CFStreamErrorHTTPAuthentication.
 *  
 *  Result:
 *	TRUE will be returned if the application of the credentials to
 *	the request was successful, otherwise FALSE is returned.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPMessageApplyCredentialDictionary(
  CFHTTPMessageRef          request,
  CFHTTPAuthenticationRef   auth,
  CFDictionaryRef           dict,
  CFStreamError *    __nullable  error)          CF_AVAILABLE(10_4, 2_0);



/*
 *  CFHTTPAuthenticationCopyRealm()
 *  
 *  Discussion:
 *	Some authentication techniques provide for namespaces on top of
 *	domains. This call will return the authentication information's
 *	namespace if there is one, otherwise it will return NULL.  This
 *	namespace is usually used for prompting the application user for
 *	a name and password.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *  
 *  Result:
 *	This call will return the authentication information's namespace
 *	if there is one, otherwise it will return NULL.
 *  
 */
CFN_EXPORT CFStringRef 
CFHTTPAuthenticationCopyRealm(CFHTTPAuthenticationRef auth) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPAuthenticationCopyDomains()
 *  
 *  Discussion:
 *	Returns a list of domain URL's on which the given authentication
 *	should be applied. This function is provided mostly for
 *	informational purposes. CFHTTPAuthenticationAppliesToRequest
 *	should be used in order to check whether a request requires the
 *	authentication.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *  
 *  Result:
 *	Returns a list of domain URL's on which the given authentication
 *	should be applied.
 *  
 */
CFN_EXPORT CFArrayRef 
CFHTTPAuthenticationCopyDomains(CFHTTPAuthenticationRef auth) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPAuthenticationCopyMethod()
 *  
 *  Discussion:
 *	Returns the method of authentication which will be performed when
 *	applying credentials.  The strongest method of authentication
 *	will be chosen in the case of multiple choices.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *  
 *  Result:
 *	Returns the method of authentication which will be performed when
 *	applying credentials.
 *  
 */
CFN_EXPORT CFStringRef 
CFHTTPAuthenticationCopyMethod(CFHTTPAuthenticationRef auth) CF_AVAILABLE(10_2, 2_0);



/*
 *  CFHTTPAuthenticationRequiresUserNameAndPassword()
 *  
 *  Discussion:
 *	Returns TRUE if the chosen authentication scheme requires a
 *	username and password.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *  
 *  Result:
 *	Returns TRUE if the chosen authentication scheme requires a
 *	username and password.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPAuthenticationRequiresUserNameAndPassword(CFHTTPAuthenticationRef auth) CF_AVAILABLE(10_3, 2_0);



/*
 *  CFHTTPAuthenticationRequiresAccountDomain()
 *  
 *  Discussion:
 *	Returns TRUE if the chosen authentication scheme requires a
 *	domain for authentication.  Currently, this will return TRUE for
 *	"NTLM" and FALSE for the other methods.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The API's to CFHTTPAuthenticationRef are thread-safe so long as
 *	multiple threads are not altering the same
 *	CFHTTPAuthenticationRef at the same time.
 *  
 *  Parameters:
 *	
 *	auth:
 *	  The authentication information being queried.
 *  
 *  Result:
 *	Returns TRUE if the chosen authentication scheme requires a
 *	domain for authentication.
 *  
 */
CFN_EXPORT Boolean 
CFHTTPAuthenticationRequiresAccountDomain(CFHTTPAuthenticationRef auth) CF_AVAILABLE(10_4, 2_0);



CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFHTTPAUTHENTICATION__ */

