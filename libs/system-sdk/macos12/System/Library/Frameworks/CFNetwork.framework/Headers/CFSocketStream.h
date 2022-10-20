/*
	 File:	   CFNetwork/CFSocketStream.h
 
	 Contains:   CoreFoundation Network socket streams header
 
	 Copyright:  Copyright (c) 2001-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFSOCKETSTREAM__
#define __CFSOCKETSTREAM__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __CFSTREAM__
#include <CoreFoundation/CFStream.h>
#endif

#ifndef __CFHOST__
#include <CFNetwork/CFHost.h>
#endif

#ifndef __CFNETSERVICES__
#include <CFNetwork/CFNetServices.h>
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
 *  kCFStreamPropertySSLContext 
 *
 * The SSLContextRef used for both read and write operations on a
 * CFSocketStream.
 *
 * CFReadStreamCopyProperty or CFWriteStreamCopyProperty return an
 * appropriately reference counted SSLContextRef.  If the stream has
 * not yet been opened, this SSLContext may be configured directly
 * using the appropriate SecureTransport APIs.
 *
 * CFReadStreamSetProperty or CFWriteStreamSetProperty will allow you
 * to specify an SSLContextRef for a stream.  If the stream has not
 * been opened, the SSLContextRef will replace any existing
 * SSLContextRef and be used in the initial stream handshake.  If the
 * stream has been opened without SSL enabled, setting this property
 * will initiate an SSL handshake over the existing socket.
 *
 * If an SSL settings dictionary was set via
 * kCFStreamPropertySSLSettings, a SSLContextRef is created internally
 * and configured as per the dictionary.  However, if an SSLContextRef
 * is set after this, its configuration will take precedence over the
 * previously configured context.
 *
 * Reconfiguring an SSLContext after the stream it is bound to has
 * opened is unsupported.
 *
 */
CFN_EXPORT const CFStringRef kCFStreamPropertySSLContext CF_AVAILABLE(10_9, 5_0);

/*
 *  kCFStreamPropertySSLPeerTrust
 *  
 *  Discussion:
 *	Stream property value for copy operations.  Returns a SecTrustRef
 *	which was a result of the SSL handshake.  This property is not valid before
 *  a stream is opened.  See Security/SecTrust.h for more information.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertySSLPeerTrust CF_AVAILABLE(10_5, 2_0);

/*
 *  kCFStreamSSLValidatesCertificateChain
 *  
 *  Discussion:
 *	Security property key for kCFStreamPropertySSLSettings. 
 *	CFBooleanRef indicating whether the certificate chain should be
 *	validated or not.  The value is kCFBooleanTrue by default (not
 *	set).
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamSSLValidatesCertificateChain CF_AVAILABLE(10_4, 2_0);
	
/*
 *  kCFStreamPropertySSLSettings
 *  
 *  Discussion:
 *	Stream property key for set operations.  CFDictionaryRef filled
 *	with different security settings.  By default, there are no
 *	security settings.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertySSLSettings CF_AVAILABLE(10_4, 2_0);

/*
 *  kCFStreamSSLLevel
 *  
 *  Discussion:
 *	Security property key for kCFStreamPropertySSLSettings. 
 *	CFStringRef being one of the security levels.  The value is
 *	kCFStreamSocketSecurityLevelNegotiatedSSL by default (not set).
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamSSLLevel CF_AVAILABLE(10_4, 2_0);

/*
 *  kCFStreamSSLPeerName
 *  
 *  Discussion:
 *	Security property key for kCFStreamPropertySSLSettings. 
 *	CFStringRef overriding the name used for certificate
 *	verification.  Set to kCFNull to prevent name verification. 
 *	Default is the host name with which the streams were created.  If
 *	no host name was used, no peer name will be used.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamSSLPeerName CF_AVAILABLE(10_4, 2_0);

/*
 *  kCFStreamSSLCertificates
 *  
 *  Discussion:
 *	Security property key for kCFStreamPropertySSLSettings. 
 *	CFArrayRef of SecCertificateRefs, except for index [0], which is
 *	a SecIdentityRef.  See SSLSetCertificate in
 *	Security/SecureTransport.h for more information.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamSSLCertificates CF_AVAILABLE(10_4, 2_0);

/*
 *  kCFStreamSSLIsServer
 *  
 *  Discussion:
 *	Security property key for kCFStreamPropertySSLSettings. 
 *	CFBooleanRef indicating whether the connection is to act as a
 *	server in the SSL process or not.  The value is kCFBooleanFalse
 *	by default (not set).  If set to kCFBooleanTrue, there must be a
 *	valid value for the kCFStreamSSLCertificates key too.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamSSLIsServer CF_AVAILABLE(10_4, 2_0);

	
/*	kCFStreamNetworkServiceType
 *
 * Discussion:
 * Property key to specify the type of service for the stream.  This
 * allows the system to properly handle the request with respect to
 * routing, suspension behavior and other networking related attributes
 * appropriate for the given service type.  The service types supported
 * are documented below.  Most streams should not need to set this
 * property.
 */
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceType CF_AVAILABLE(10_7, 4_0);

/* supported network service types: */
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeVideo      CF_AVAILABLE(10_7, 5_0);   // interactive video
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeVoice      CF_AVAILABLE(10_7, 5_0);   // interactive voice data
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeBackground CF_AVAILABLE(10_7, 5_0);   // background
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeResponsiveData __OSX_AVAILABLE_STARTING(__MAC_10_8, __IPHONE_6_0); // responsive (time sensitive) data
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeCallSignaling	   CF_AVAILABLE(10_12, 10_0); //Call Signaling
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeAVStreaming __OSX_AVAILABLE_STARTING(__MAC_10_8, __IPHONE_6_0);					// multimedia audio/video streaming
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeResponsiveAV __OSX_AVAILABLE_STARTING(__MAC_10_8, __IPHONE_6_0);				// responsive multimedia audio/video

/* deprecated network service type: */
CFN_EXPORT const CFStringRef kCFStreamNetworkServiceTypeVoIP       CF_DEPRECATED(10_7, 10_11, 4_0, 9_0, "use PushKit for VoIP control purposes");   // voice over IP control - this service type is deprecated in favor of using PushKit for VoIP control

/*
 *  kCFStreamPropertyNoCellular
 *  
 *  Discussion:
 *  Stream property value, for both set and copy operations.
 *  The value is a CFBooleanRef which indicates whether the connection
 *  is allowed to use the built-in celluar radios.  A value of kCFBooleanTrue 
 *  disallows use of cellular interfaces.  kCFBooleanFalse (the default)
 *  allows use of cellular interfaces.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyNoCellular CF_AVAILABLE(10_8, 5_0);

/*
 *  kCFStreamPropertyConnectionIsCellular
 *
 *  Discussion:
 *  Stream property key for copy operations.  Returns a CFBooleanRef value
 *  of kCFBooleanTrue if the stream has connected using the built in cellular radios.
 *  It returns kCFBooleanFalse if the stream is conneceted over a non-cellular
 *  interface or has not yet established a connection.
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyConnectionIsCellular CF_AVAILABLE(10_8, 6_0);

/*
 *  kCFStreamPropertyAllowExpensiveNetworkAccess
 *
 *  Discussion:
 *  Stream property value, for both set and copy operations.
 *  The value is a CFBooleanRef which indicates whether the connection
 *  is allowed to use network interfaces that are marked expensive.  A value of
 *  kCFBooleanTrue (the default) allows use of expensive interfaces.  kCFBooleanFalse
 *  disallows use of expensive interfaces.
 *
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyAllowExpensiveNetworkAccess API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 *  kCFStreamPropertyConnectionIsExpensive
 *
 *  Discussion:
 *  Stream property key for copy operations.  Returns a CFBooleanRef value
 *  of kCFBooleanTrue if the stream has connected using an "expensive" interface.
 *  It returns kCFBooleanFalse if the stream is conneceted over an "inexpensive"
 *  interface.  If the connection has not been established yet NULL will be returned.
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyConnectionIsExpensive API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 *  kCFStreamPropertyAllowConstrainedNetworkAccess
 *
 *  Discussion:
 *  Stream property value, for both set and copy operations.
 *  The value is a CFBooleanRef which indicates whether the connection
 *  is allowed to use "constrained" networks.  A value of kCFBooleanTrue (the default)
 *  allows the  use of constrained interfaces.  kCFBooleanFalse
 *  disallows use of constrained interfaces.
 *
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyAllowConstrainedNetworkAccess API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 *  kCFStreamErrorDomainWinSock
 *  
 *  Discussion:
 *  WinSock error domain.  On Win32 platforms, networking errors will
 *  come in this domain.  See <winsock2.h> for values.  Note that
 *  non-networking errors, like ENOMEM, will continue to come in the
 *  POSIX domain as on OS X.
 *  
 */
CFN_EXPORT const CFIndex kCFStreamErrorDomainWinSock CF_AVAILABLE(10_5, 2_0);

CF_INLINE
SInt32 CFSocketStreamSOCKSGetErrorSubdomain(const CFStreamError* error) {
	return ((error->error >> 16) & 0x0000FFFF);
}

CF_INLINE
SInt32 CFSocketStreamSOCKSGetError(const CFStreamError* error) {
	return (error->error & 0x0000FFFF);
}

enum {
  kCFStreamErrorSOCKSSubDomainNone = 0,        /* Error code is a general SOCKS error*/
  kCFStreamErrorSOCKSSubDomainVersionCode = 1, /* Error code is the version of SOCKS which the server wishes to use*/
  kCFStreamErrorSOCKS4SubDomainResponse = 2,   /* Error code is the status code returned by the server*/
  kCFStreamErrorSOCKS5SubDomainUserPass = 3,   /* Error code is the status code that the server returned*/
  kCFStreamErrorSOCKS5SubDomainMethod = 4,     /* Error code is the server's desired negotiation method*/
  kCFStreamErrorSOCKS5SubDomainResponse = 5    /* Error code is the response code that the server returned in reply to the connection request*/
};


/* kCFStreamErrorSOCKSSubDomainNone*/
enum {
  kCFStreamErrorSOCKS5BadResponseAddr = 1,
  kCFStreamErrorSOCKS5BadState  = 2,
  kCFStreamErrorSOCKSUnknownClientVersion = 3
};

/* kCFStreamErrorSOCKS4SubDomainResponse*/
enum {
  kCFStreamErrorSOCKS4RequestFailed = 91, /* request rejected or failed */
  kCFStreamErrorSOCKS4IdentdFailed = 92,  /* request rejected because SOCKS server cannot connect to identd on the client */
  kCFStreamErrorSOCKS4IdConflict = 93     /* request rejected because the client program and identd report different user-ids */
};

/* kCFStreamErrorSOCKS5SubDomainMethod*/
enum {
  kSOCKS5NoAcceptableMethod	 = 0xFF  /* other values indicate the server's desired method */
};

/*
 *  kCFStreamPropertyProxyLocalBypass
 *
 *  Discussion:
 *	CFDictionary key for proxy information.  It matches
 *	kSCPropNetProxiesExcludeSimpleHostnames defined in
 *	SCSchemaDefinitions.h.  CFNumber (0 or 1) indicating to bypass
 *	the proxies for simple hostnames (names without dots).
 *
 */
CFN_EXPORT const CFStringRef kCFStreamPropertyProxyLocalBypass CF_AVAILABLE(10_4, 2_0);

/*
 *  kCFStreamPropertySocketRemoteHost
 *  
 *  Discussion:
 *	Stream property key for copy operations. Returns a CFHostRef if
 *	known, otherwise NULL.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertySocketRemoteHost CF_AVAILABLE(10_3, 2_0);


/*
 *  kCFStreamPropertySocketRemoteNetService
 *  
 *  Discussion:
 *	Stream property key for copy operations. Returns a
 *	CFNetServiceRef if known, otherwise NULL.
 *  
 */
CFN_EXPORT const CFStringRef kCFStreamPropertySocketRemoteNetService CF_AVAILABLE(10_3, 2_0);

/*
 *  kCFStreamPropertySocketExtendedBackgroundIdleMode
 *
 *  Discussion:
 *	Set this to kCFBooleanTrue to enable extended background idle
 *	mode.  Enabling this mode asks the system to keep the socket open
 *  and delay reclaiming it when the process moves to the background (see
 *  https://developer.apple.com/library/ios/technotes/tn2277/_index.html)
 *  This property must be set before the stream is opened.
 *
 */
CFN_EXPORT const CFStringRef kCFStreamPropertySocketExtendedBackgroundIdleMode CF_AVAILABLE(10_11, 9_0);

/*
 *  CFStreamCreatePairWithSocketToCFHost()
 *  
 *  Discussion:
 *	Given a CFHostRef, this function will create a pair of streams
 *	suitable for connecting to the host.  If there is a failure
 *	during creation, the stream references will be set to NULL.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CFAllocator which should be used to allocate memory for the
 *	  streams. If this reference is not a valid CFAllocator, the
 *	  behavior is undefined.
 *	
 *	host:
 *	  A reference to a CFHost to which the streams are desired.  If
 *	  unresolved, the host will be resolved prior to connecting.
 *	
 *	port:
 *	  The port to which the connection should be established.
 *	
 *	readStream:
 *	  A pointer to a CFReadStreamRef which will be set to the new
 *	  read stream instance.  Can be set to NULL if not desired.
 *	
 *	writeStream:
 *	  A pointer to a CFWriteStreamRef which will be set to the new
 *	  write stream instance.  Can be set to NULL if not desired.
 *  
 */
CFN_EXPORT void 
CFStreamCreatePairWithSocketToCFHost(
  CFAllocatorRef      __nullable alloc,
  CFHostRef           host,
  SInt32              port,
  CFReadStreamRef __nullable *   __nullable readStream,
  CFWriteStreamRef __nullable *  __nullable writeStream)            API_DEPRECATED("Use Network framework instead", macos(10.3, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


/*
 *  CFStreamCreatePairWithSocketToNetService()
 *  
 *  Discussion:
 *	Given a CFNetService, this function will create a pair of streams
 *	suitable for connecting to the service.  If there is a failure
 *	during creation, the stream references will be set to NULL.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CFAllocator which should be used to allocate memory for the
 *	  streams. If this reference is not a valid CFAllocator, the
 *	  behavior is undefined.
 *	
 *	service:
 *	  A reference to a CFNetService to which the streams are desired.
 *	   If unresolved, the service will be resolved prior to
 *	  connecting.
 *	
 *	readStream:
 *	  A pointer to a CFReadStreamRef which will be set to the new
 *	  read stream instance.  Can be set to NULL if not desired.
 *	
 *	writeStream:
 *	  A pointer to a CFWriteStreamRef which will be set to the new
 *	  write stream instance.  Can be set to NULL if not desired.
 *  
 */
CFN_EXPORT void 
CFStreamCreatePairWithSocketToNetService(
  CFAllocatorRef      __nullable alloc,
  CFNetServiceRef     service,
  CFReadStreamRef   __nullable *  __nullable readStream,
  CFWriteStreamRef __nullable *  __nullable writeStream)           API_DEPRECATED("Use Network framework instead", macos(10.3, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 * The following properties are DEPRECATED starting in OS X 10.6 and iOS 4.0.
 *
 * kCFStreamPropertySSLPeerCertificates:
 *         The peer certificates are available as part of the SecTrustRef object.  See <Security/SecTrust.h>
 *
 * kCFStreamSSLAllowsExpiredCertificates:
 * kCFStreamSSLAllowsExpiredRoots:
 * kCFStreamSSLAllowsAnyRoot:
 *        The SSL handshake flags which affect untrusted certificate chain evaluation are deprecated.
 *        Instead, use the single property kCFStreamSSLValidatesCertificateChain to disable certificate
 *        chain checking if the user has decided that it is appropriate to do so.
 */

CFN_EXPORT const CFStringRef kCFStreamPropertySSLPeerCertificates   CF_DEPRECATED(10_4, 10_6, 2_0, 4_0);
CFN_EXPORT const CFStringRef kCFStreamSSLAllowsExpiredCertificates  CF_DEPRECATED(10_4, 10_6, 2_0, 4_0);
CFN_EXPORT const CFStringRef kCFStreamSSLAllowsExpiredRoots         CF_DEPRECATED(10_4, 10_6, 2_0, 4_0);
CFN_EXPORT const CFStringRef kCFStreamSSLAllowsAnyRoot              CF_DEPRECATED(10_4, 10_6, 2_0, 4_0);


#if PRAGMA_ENUM_ALWAYSINT
	#pragma enumsalwaysint reset
#endif

CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFSOCKETSTREAM__ */

