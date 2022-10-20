/*
	 File:	   CFNetwork/CFProxySupport.h
 
	 Contains:   Support for computing which proxy applies when
 
	 Copyright:  Copyright (c) 2006-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFPROXYSUPPORT__
#define __CFPROXYSUPPORT__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __CFARRAY__
#include <CoreFoundation/CFArray.h>
#endif

#ifndef __CFSTRING__
#include <CoreFoundation/CFString.h>
#endif

#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif

#ifndef __CFERROR__
#include <CoreFoundation/CFError.h>
#endif

#ifndef __CFRUNLOOP__
#include <CoreFoundation/CFRunLoop.h>
#endif

#ifndef __CFSTREAM__
#include <CoreFoundation/CFStream.h>
#endif


/* 
	These APIs return arrays of dictionaries, where each dictionary describes a single proxy. 
	The arrays represent the order in which the proxies should be tried - try to download the URL
	using the first entry in the array, and if that fails, try using the second entry, and so on.

	The keys to the proxy dictionaries follow the function declarations; every proxy dictionary 
	will have an entry for kCFProxyTypeKey.  If the type is anything except 
	kCFProxyTypeAutoConfigurationURL, the dictionary will also have entries for the proxy's host
	and port (under kCFProxyHostNameKey and kCFProxyPortNumberKey respectively).  If the type is
	kCFProxyTypeAutoConfigurationURL, it will have an entry for kCFProxyAutoConfigurationURLKey.  
	
	The keys for username and password are optional and will only be present if the username 
	or password could be extracted from the information passed in (i.e. either the URL itself
	or the proxy dictionary supplied).  These APIs do not consult any external credential stores
	(such as the Keychain).
*/


#if PRAGMA_ONCE
#pragma once
#endif

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*!
	@function CFNetworkCopySystemProxySettings
	@discussion Returns a CFDictionary containing the current system internet proxy settings.
	@result Returns a dictionary containing key-value pairs that represent
		the current internet proxy settings.  See below for definitions of the keys and
		values.
		NULL if no proxy settings have been defined or if an error
		was encountered.
		The caller is responsible for releasing the returned dictionary.
*/
CFN_EXPORT __nullable CFDictionaryRef
CFNetworkCopySystemProxySettings(void) CF_AVAILABLE(10_6, 2_0);

	
/*
 *  CFNetworkCopyProxiesForURL()
 *  
 *  Discussion:
 *	Given a URL and a proxy dictionary, determines the ordered list
 *	of proxies that should be used to download the given URL.
 *  
 *  Parameters:
 *	
 *	url:
 *	  The URL to be accessed
 *	
 *	proxySettings:
 *	  A dictionary describing the available proxy settings; the
 *	  dictionary's format should match the dictionary returned
 *	  by CFNetworkCopySystemProxySettings described below.
 *  
 *  Result:
 *	An array of dictionaries; each dictionary describes a single
 *	proxy.  See the comment at the top of this file for how to
 *	interpret the returned dictionaries.
 *  
 */
CFN_EXPORT CFArrayRef 
CFNetworkCopyProxiesForURL(CFURLRef url, CFDictionaryRef proxySettings) CF_AVAILABLE(10_5, 2_0);




/*
 *  CFProxyAutoConfigurationResultCallback
 *  
 *  Discussion:
 *	Callback function to be called when a PAC file computation
 *	(initiated by either CFNetworkExecuteProxyAutoConfigurationScript
 *	or CFNetworkExecuteProxyAutoConfigurationURL) has completed.
 *  
 *  Parameters:
 *	
 *	client:
 *	  The client reference passed in to
 *	  CFNetworkExecuteProxyAutoConfigurationScript or
 *	  CFNetworkExecuteProxyAutoConfigurationURL
 *	
 *	proxyList:
 *	  Upon success, the list of proxies returned by the
 *	  autoconfiguration script.  The list has the same format as
 *	  returned by CFProxyCopyProxiesForURL, above, except that no
 *	  entry may be of type kCFProxyTypeAutoConfigurationURL.  Note
 *	  that if the client wishes to keep this list, they must retain
 *	  it when they receive this callback.
 *	
 *	error:
 *	  Upon failure, an error object explaining the failure.
 */
typedef CALLBACK_API_C( void , CFProxyAutoConfigurationResultCallback )(void *client, CFArrayRef proxyList, CFErrorRef __nullable error);

/*
 *  CFNetworkCopyProxiesForAutoConfigurationScript()
 *  
 *  Discussion:
 *	Synchronously executes the given proxy autoconfiguration script
 *	and returns a valid proxyList and NULL error upon success or a
 *	NULL proxyList and valid error on failure.
 *  
 *  Parameters:
 *	
 *	proxyAutoConfigurationScript:
 *	  A CFString containing the code of the script to be executed.
 *	
 *	targetURL:
 *	  The URL that should be input in to the autoconfiguration script.
 *	
 *	error:
 *	  A return argument that will contain a valid error in case of
 *	  failure.
 *  
 *  Result:
 *	An array of dictionaries describing the proxies returned by the
 *	script or NULL on failure.
 *  
 */
CFN_EXPORT __nullable CFArrayRef
CFNetworkCopyProxiesForAutoConfigurationScript(CFStringRef proxyAutoConfigurationScript, CFURLRef targetURL, CFErrorRef * __nullable error) CF_AVAILABLE(10_5, 2_0);


/*
 *  CFNetworkExecuteProxyAutoConfigurationScript()
 *  
 *  Discussion:
 *	Begins the process of executing proxyAutoConfigurationScript to
 *	determine the correct proxy to use to retrieve targetURL.  The
 *	caller should schedule the returned run loop source; when the
 *	results are found, the caller's callback will be called via the
 *	run loop, passing a valid proxyList and NULL error upon success,
 *	or a NULL proxyList and valid error on failure.  The caller
 *	should invalidate the returned run loop source if it wishes to
 *	terminate the request before completion. The returned
 *	RunLoopSource will be removed from all run loops and modes on
 *	which it was scheduled after the callback returns.
 *  
 *  Parameters:
 *	
 *	proxyAutoConfigurationScript:
 *	  A CFString containing the code of the script to be executed.
 *	
 *	targetURL:
 *	  The URL that should be passed to the autoconfiguration script.
 *	
 *	cb:
 *	  A client callback to notify the caller of completion.
 *	
 *	clientContext:
 *	  a stream context containing a client info object and optionally
 *	  retain / release callbacks for said info object.
 *  
 *  Result:
 *	A CFRunLoopSource which the client can use to schedule execution
 *	of the AutoConfiguration Script.
 *  
 */
CFN_EXPORT CFRunLoopSourceRef 
CFNetworkExecuteProxyAutoConfigurationScript(
  CFStringRef proxyAutoConfigurationScript,
  CFURLRef targetURL,
  CFProxyAutoConfigurationResultCallback cb,
  CFStreamClientContext * clientContext) CF_AVAILABLE(10_5, 2_0);



/*
 *  CFNetworkExecuteProxyAutoConfigurationURL()
 *  
 *  Discussion:
 *	As CFNetworkExecuteProxyAutoConfigurationScript(), above, except
 *	that CFNetworkExecuteProxyAutoConfigurationURL will additionally
 *	download the contents of proxyAutoConfigURL, convert it to a
 *	JavaScript string, and then execute that script.
 *  Ownership for the returned CFRunLoopSourceRef follows the copy rule, 
 *  the client is responsible for releasing the object.
 *  
 */
CFN_EXPORT CFRunLoopSourceRef 
CFNetworkExecuteProxyAutoConfigurationURL(
  CFURLRef proxyAutoConfigURL,
  CFURLRef targetURL,
  CFProxyAutoConfigurationResultCallback cb,
  CFStreamClientContext * clientContext) CF_AVAILABLE(10_5, 2_0);


/*
 *  kCFProxyTypeKey
 *  
 *  Discussion:
 *	Key for the type of proxy being represented; value will be one of
 *	the kCFProxyType constants listed below.
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyTypeKey CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyHostNameKey
 *  
 *  Discussion:
 *	Key for the proxy's hostname; value is a CFString.  Note that
 *	this may be an IPv4 or IPv6 dotted-IP string.
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyHostNameKey CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyPortNumberKey
 *  
 *  Discussion:
 *	Key for the proxy's port number; value is a CFNumber specifying
 *	the port on which to contact the proxy
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyPortNumberKey CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyAutoConfigurationURLKey
 *  
 *  Discussion:
 *	Key for the proxy's PAC file location; this key is only present
 *	if the proxy's type is kCFProxyTypeAutoConfigurationURL.  Value
 *	is a CFURL specifying the location of a proxy auto-configuration
 *	file
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyAutoConfigurationURLKey CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyAutoConfigurationJavaScriptKey
 *  
 *  Discussion:
 *	Key for the proxy's PAC script
 *	The value is a CFString that contains the full JavaScript soure text for the PAC file.
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyAutoConfigurationJavaScriptKey CF_AVAILABLE(10_7, 3_0);


/*
 *  kCFProxyUsernameKey
 *  
 *  Discussion:
 *	Key for the username to be used with the proxy; value is a
 *	CFString. Note that this key will only be present if the username
 *	could be extracted from the information passed in.  No external
 *	credential stores (like the Keychain) are consulted.
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyUsernameKey CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyPasswordKey
 *  
 *  Discussion:
 *	Key for the password to be used with the proxy; value is a
 *	CFString. Note that this key will only be present if the username
 *	could be extracted from the information passed in.  No external
 *	credential stores (like the Keychain) are consulted.
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyPasswordKey CF_AVAILABLE(10_5, 2_0);

/* 
	Possible values for kCFProxyTypeKey:
	kCFProxyTypeNone - no proxy should be used; contact the origin server directly
	kCFProxyTypeHTTP - the proxy is an HTTP proxy
	kCFProxyTypeHTTPS - the proxy is a tunneling proxy as used for HTTPS
	kCFProxyTypeSOCKS - the proxy is a SOCKS proxy
	kCFProxyTypeFTP - the proxy is an FTP proxy
	kCFProxyTypeAutoConfigurationURL - the proxy is specified by a proxy autoconfiguration (PAC) file
*/

/*
 *  kCFProxyTypeNone
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyTypeNone CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyTypeHTTP
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyTypeHTTP CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyTypeHTTPS
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyTypeHTTPS CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyTypeSOCKS
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyTypeSOCKS CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyTypeFTP
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyTypeFTP CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyTypeAutoConfigurationURL
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyTypeAutoConfigurationURL CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFProxyTypeAutoConfigurationJavaScript
 *
 */
CFN_EXPORT const CFStringRef kCFProxyTypeAutoConfigurationJavaScript CF_AVAILABLE(10_7, 3_0);
	
/*
 *  kCFProxyAutoConfigHTTPResponse
 *  
 */
CFN_EXPORT const CFStringRef kCFProxyAutoConfigurationHTTPResponseKey CF_AVAILABLE(10_5, 2_0);
	

#if TARGET_OS_MAC
/*
 *  kCFNetworkProxiesExceptionsList
 *  
 *  Discussion:
 *	Key for the list of host name patterns that should bypass the proxy; value is a
 *	CFArray of CFStrings.  
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesExceptionsList CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesExcludeSimpleHostnames
 *  
 *  Discussion:
 *	Key whose value indicates if simple hostnames will be excluded; value is a
 *	CFNumber.  Simple hostnames will be excluded if the key is present and has a 
 *	non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesExcludeSimpleHostnames CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesFTPEnable
 *  
 *  Discussion:
 *	Key for the enabled status of the ftp proxy; value is a
 *	CFNumber.  The proxy is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesFTPEnable CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesFTPPassive
 *  
 *  Discussion:
 *	Key for the state of passive mode for the ftp proxy; value is a
 *	CFNumber.  A value of one indicates that passive mode is enabled, a value
 *	of zero indicates that passive mode is not enabled.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesFTPPassive CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesFTPPort
 *  
 *  Discussion:
 *	Key for the port number associated with the ftp proxy; value is a
 *	CFNumber which is the port number.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesFTPPort CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesFTPProxy
 *  
 *  Discussion:
 *	Key for the host name associated with the ftp proxy; value is a
 *	CFString which is the proxy host name.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesFTPProxy CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesGopherEnable
 *  
 *  Discussion:
 *	Key for the enabled status of the gopher proxy; value is a
 *	CFNumber.  The proxy is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesGopherEnable CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesGopherPort
 *  
 *  Discussion:
 *	Key for the port number associated with the gopher proxy; value is a
 *	CFNumber which is the port number.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesGopherPort CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesGopherProxy
 *  
 *  Discussion:
 *	Key for the host name associated with the gopher proxy; value is a
 *	CFString which is the proxy host name.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesGopherProxy CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesHTTPEnable
 *  
 *  Discussion:
 *	Key for the enabled status of the HTTP proxy; value is a
 *	CFNumber.  The proxy is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesHTTPEnable CF_AVAILABLE(10_6, 2_0);

/*
 *  kCFNetworkProxiesHTTPPort
 *  
 *  Discussion:
 *	Key for the port number associated with the HTTP proxy; value is a
 *	CFNumber which is the port number.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesHTTPPort CF_AVAILABLE(10_6, 2_0);

/*
 *  kCFNetworkProxiesHTTPProxy
 *  
 *  Discussion:
 *	Key for the host name associated with the HTTP proxy; value is a
 *	CFString which is the proxy host name.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesHTTPProxy CF_AVAILABLE(10_6, 2_0);

/*
 *  kCFNetworkProxiesHTTPSEnable
 *  
 *  Discussion:
 *	Key for the enabled status of the HTTPS proxy; value is a
 *	CFNumber.  The proxy is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesHTTPSEnable CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesHTTPSPort
 *  
 *  Discussion:
 *	Key for the port number associated with the HTTPS proxy; value is a
 *	CFNumber which is the port number.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesHTTPSPort CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesHTTPSProxy
 *  
 *  Discussion:
 *	Key for the host name associated with the HTTPS proxy; value is a
 *	CFString which is the proxy host name.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesHTTPSProxy CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesRTSPEnable
 *  
 *  Discussion:
 *	Key for the enabled status of the RTSP proxy; value is a
 *	CFNumber.  The proxy is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesRTSPEnable CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesRTSPPort
 *  
 *  Discussion:
 *	Key for the port number associated with the RTSP proxy; value is a
 *	CFNumber which is the port number.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesRTSPPort CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesRTSPProxy
 *  
 *  Discussion:
 *	Key for the host name associated with the RTSP proxy; value is a
 *	CFString which is the proxy host name.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesRTSPProxy CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesSOCKSEnable
 *  
 *  Discussion:
 *	Key for the enabled status of the SOCKS proxy; value is a
 *	CFNumber.  The proxy is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesSOCKSEnable CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesSOCKSPort
 *  
 *  Discussion:
 *	Key for the port number associated with the SOCKS proxy; value is a
 *	CFNumber which is the port number.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesSOCKSPort CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesSOCKSProxy
 *  
 *  Discussion:
 *	Key for the host name associated with the SOCKS proxy; value is a
 *	CFString which is the proxy host name.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesSOCKSProxy CF_AVAILABLE(10_6, NA);

/*
 *  kCFNetworkProxiesProxyAutoConfigEnable
 *  
 *  Discussion:
 *	Key for the enabled status ProxyAutoConfig (PAC); value is a
 *	CFNumber.  ProxyAutoConfig is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesProxyAutoConfigEnable	CF_AVAILABLE(10_6, 2_0);

/*
 *  kCFNetworkProxiesProxyAutoConfigURLString
 *  
 *  Discussion:
 *	Key for the url which indicates the location of the ProxyAutoConfig (PAC) file; value is a
 *	CFString which is url for the PAC file.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesProxyAutoConfigURLString CF_AVAILABLE(10_6, 2_0);

/*
 * kCFNetworkProxiesProxyAutoConfigJavaScript
 *
 * Discussion:
 * Key for the string which is the full JavaScript source of the ProxyAutoConfig (PAC) script;  value is a
 * CFString with is the full text source of the PAC script.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesProxyAutoConfigJavaScript	CF_AVAILABLE(10_7, 3_0);
	
/*
 *  kCFNetworkProxiesProxyAutoDiscoveryEnable
 *  
 *  Discussion:
 *	Key for the enabled status of proxy auto discovery; value is a
 *	CFNumber.  Proxy auto discovery is enabled if the key is present and has a non-zero value.
 */
CFN_EXPORT const CFStringRef kCFNetworkProxiesProxyAutoDiscoveryEnable CF_AVAILABLE(10_6, NA);
#endif // TARGET_OS_MAC

CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFPROXYSUPPORT__ */

