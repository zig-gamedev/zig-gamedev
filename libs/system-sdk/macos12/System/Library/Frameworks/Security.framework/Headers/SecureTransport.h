/*
 * Copyright (c) 1999-2002,2005-2019 Apple Inc. All Rights Reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*
 * SecureTransport.h - Public API for Apple SSL/TLS Implementation
 */

#ifndef _SECURITY_SECURETRANSPORT_H_
#define _SECURITY_SECURETRANSPORT_H_

/*
 * This file describes the public API for an implementation of the
 * Secure Socket Layer, V. 3.0, Transport Layer Security, V. 1.0 to V. 1.2
 * and Datagram Transport Layer Security V. 1.0
 *
 * There are no transport layer dependencies in this library;
 * it can be used with sockets, Open Transport, etc. Applications using
 * this library provide callback functions which do the actual I/O
 * on underlying network connections. Applications are also responsible
 * for setting up raw network connections; the application passes in
 * an opaque reference to the underlying (connected) entity at the
 * start of an SSL session in the form of an SSLConnectionRef.
 *
 * Some terminology:
 *
 * A "client" is the initiator of an SSL Session. The canonical example
 * of a client is a web browser, when it's talking to an https URL.
 *
 * A "server" is an entity which accepts requests for SSL sessions made
 * by clients. E.g., a secure web server.

 * An "SSL Session", or "session", is bounded by calls to SSLHandshake()
 * and SSLClose(). An "Active session" is in some state between these
 * two calls, inclusive.
 *
 * An SSL Session Context, or SSLContextRef, is an opaque reference in this
 * library to the state associated with one session. A SSLContextRef cannot
 * be reused for multiple sessions.
 */

#include <CoreFoundation/CFArray.h>
#include <Security/SecProtocolOptions.h>
#include <Security/CipherSuite.h>
#include <Security/SecTrust.h>
#include <Security/SecBase.h>
#include <sys/types.h>
#include <Availability.h>

#ifdef __cplusplus
extern "C" {
#endif

CF_ASSUME_NONNULL_BEGIN
CF_IMPLICIT_BRIDGING_ENABLED

#define __SECURETRANSPORT_API_DEPRECATED(...) \
    __API_DEPRECATED("No longer supported. Use Network.framework.", __VA_ARGS__)

/***********************
 *** Common typedefs ***
 ***********************/

/* Opaque reference to an SSL session context */
struct SSLContext;
typedef struct CF_BRIDGED_TYPE(id) SSLContext *SSLContextRef;

/* Opaque reference to an I/O connection (socket, endpoint, etc.) */
typedef const void *SSLConnectionRef;

/* SSL session options */
typedef CF_ENUM(int, SSLSessionOption) {
	/*
	 * Set this option to enable returning from SSLHandshake (with a result of
	 * errSSLServerAuthCompleted) when the server authentication portion of the
	 * handshake is complete. This disable certificate verification and
	 * provides an opportunity to perform application-specific server
	 * verification before deciding to continue.
	 */
	kSSLSessionOptionBreakOnServerAuth CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 0,
	/*
	 * Set this option to enable returning from SSLHandshake (with a result of
	 * errSSLClientCertRequested) when the server requests a client certificate.
	 */
	kSSLSessionOptionBreakOnCertRequested CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 1,
    /*
     * This option is the same as kSSLSessionOptionBreakOnServerAuth but applies
     * to the case where SecureTransport is the server and the client has presented
     * its certificates allowing the server to verify whether these should be
     * allowed to authenticate.
     */
    kSSLSessionOptionBreakOnClientAuth CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 2,
    /*
     * Enable/Disable TLS False Start
     * When enabled, False Start will only be performed if a adequate cipher-suite is
     * negotiated.
     */
    kSSLSessionOptionFalseStart CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 3,
    /*
     * Enable/Disable 1/n-1 record splitting for BEAST attack mitigation.
     * When enabled, record splitting will only be performed for TLS 1.0 connections
     * using a block cipher.
     */
    kSSLSessionOptionSendOneByteRecord CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 4,
    /*
     * Allow/Disallow server identity change on renegotiation. Disallow by default
     * to avoid Triple Handshake attack.
     */
    kSSLSessionOptionAllowServerIdentityChange CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 5,
    /*
     * Enable fallback countermeasures. Use this option when retyring a SSL connection
     * with a lower protocol version because of failure to connect.
     */
    kSSLSessionOptionFallback CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 6,
    /*
     * Set this option to break from a client hello in order to check for SNI
     */
    kSSLSessionOptionBreakOnClientHello CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 7,
    /*
     * Set this option to Allow renegotations. False by default.
     */
    kSSLSessionOptionAllowRenegotiation CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 8,
    /*
     * Set this option to enable session tickets. False by default.
     */
    kSSLSessionOptionEnableSessionTickets CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0) = 9,
};

/* State of an SSLSession */
typedef CF_CLOSED_ENUM(int, SSLSessionState) {
	kSSLIdle CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),        /* no I/O performed yet */
	kSSLHandshake CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),	/* SSL handshake in progress */
	kSSLConnected CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),	/* Handshake complete, ready for normal I/O */
	kSSLClosed CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),		/* connection closed normally */
	kSSLAborted	CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0)		/* connection aborted */
};

/*
 * Status of client certificate exchange (which is optional
 * for both server and client).
 */
typedef CF_ENUM(int, SSLClientCertificateState) {
	/* Server hasn't asked for a cert. Client hasn't sent one. */
	kSSLClientCertNone CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),
	/* Server has asked for a cert, but client didn't send it. */
	kSSLClientCertRequested CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),
	/*
	 * Server side: We asked for a cert, client sent one, we validated
	 *				it OK. App can inspect the cert via
	 *				SSLCopyPeerCertificates().
	 * Client side: server asked for one, we sent it.
	 */
	kSSLClientCertSent CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),
	/*
	 * Client sent a cert but failed validation. Server side only.
	 * Server app can inspect the cert via SSLCopyPeerCertificates().
	 */
	kSSLClientCertRejected CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0)
};

/*
 * R/W functions. The application using this library provides
 * these functions via SSLSetIOFuncs().
 *
 * Data's memory is allocated by caller; on entry to these two functions
 * the *length argument indicates both the size of the available data and the
 * requested byte count. Number of bytes actually transferred is returned in
 * *length.
 *
 * The application may configure the underlying connection to operate
 * in a non-blocking manner; in such a case, a read operation may
 * well return errSSLWouldBlock, indicating "I transferred less data than
 * you requested (maybe even zero bytes), nothing is wrong, except
 * requested I/O hasn't completed". This will be returned back up to
 * the application as a return from SSLRead(), SSLWrite(), SSLHandshake(),
 * etc.
 */
typedef OSStatus
(*SSLReadFunc) 				(SSLConnectionRef 	connection,
							 void 				*data, 			/* owned by
							 									 * caller, data
							 									 * RETURNED */
							 size_t 			*dataLength);	/* IN/OUT */
typedef OSStatus
(*SSLWriteFunc) 			(SSLConnectionRef 	connection,
							 const void 		*data,
							 size_t 			*dataLength);	/* IN/OUT */

/* DEPRECATED aliases for errSSLPeerAuthCompleted */
#define errSSLServerAuthCompleted	errSSLPeerAuthCompleted
#define errSSLClientAuthCompleted	errSSLPeerAuthCompleted

/* DEPRECATED alias for the end of the error range */
#define errSSLLast errSSLUnexpectedRecord

typedef CF_CLOSED_ENUM(int, SSLProtocolSide)
{
    kSSLServerSide CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),
    kSSLClientSide CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0)
};

typedef CF_ENUM(int, SSLConnectionType)
{
    kSSLStreamType CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0),
    kSSLDatagramType CF_ENUM_DEPRECATED(10_2, 10_15, 2_0, 13_0)
};

/*
 * Predefined TLS configuration constants
 */

/* Default configuration (has 3DES, no RC4) */
extern const CFStringRef kSSLSessionConfig_default
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.13), ios(5.0, 11.0));

/* ATS v1 Config: TLS v1.2, only PFS ciphersuites */
extern const CFStringRef kSSLSessionConfig_ATSv1
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/* ATS v1 Config without PFS: TLS v1.2, include non PFS ciphersuites */
extern const CFStringRef kSSLSessionConfig_ATSv1_noPFS
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/* TLS v1.2 to TLS v1.0, with default ciphersuites (no 3DES, no RC4) */
extern const CFStringRef kSSLSessionConfig_standard
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/* TLS v1.2 to TLS v1.0, with default ciphersuites + RC4 + 3DES */
extern const CFStringRef kSSLSessionConfig_RC4_fallback
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.13), ios(5.0, 11.0));

/* TLS v1.0 only, with default ciphersuites + fallback SCSV */
extern const CFStringRef kSSLSessionConfig_TLSv1_fallback
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/* TLS v1.0, with default ciphersuites + RC4 + 3DES + fallback SCSV */
extern const CFStringRef kSSLSessionConfig_TLSv1_RC4_fallback
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.13), ios(5.0, 11.0));

/* TLS v1.2 to TLS v1.0, defaults + RC4 + DHE ciphersuites */
extern const CFStringRef kSSLSessionConfig_legacy
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/* TLS v1.2 to TLS v1.0, default + RC4 + DHE ciphersuites */
extern const CFStringRef kSSLSessionConfig_legacy_DHE
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/* TLS v1.2, anonymous ciphersuites only */
extern const CFStringRef kSSLSessionConfig_anonymous
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/* TLS v1.2 to TLS v1.0, has 3DES, no RC4 */
extern const CFStringRef kSSLSessionConfig_3DES_fallback
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.13), ios(5.0, 11.0));

/* TLS v1.0, with default ciphersuites + 3DES, no RC4 */
extern const CFStringRef kSSLSessionConfig_TLSv1_3DES_fallback
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.13), ios(5.0, 11.0));


/******************
 *** Public API ***
 ******************/

/*
 * Secure Transport APIs require a SSLContextRef, which is an opaque
 * reference to the SSL session and its parameters. On Mac OS X 10.7
 * and earlier versions, a new context is created using SSLNewContext,
 * and is disposed by calling SSLDisposeContext.
 *
 * On i0S 5.0 and later, as well as Mac OS X versions after 10.7, the
 * SSLContextRef is a true CFType object with retain-release semantics.
 * New code should create a new context using SSLCreateContext (instead
 * of SSLNewContext), and dispose the context by calling CFRelease
 * (instead of SSLDisposeContext) when finished with it.
 */

/*
 * @function SSLContextGetTypeID
 * @abstract Return the CFTypeID for SSLContext objects.
 * @return CFTypeId for SSLContext objects.
 */
CFTypeID
SSLContextGetTypeID(void)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLCreateContext
 * @abstract Create a new instance of an SSLContextRef using the specified allocator.
 * @param alloc Allocator to use for memory.
 * @param protooclSide Client or server indication.
 * @param connectionType Type of connection.
 * @return A newly allocated SSLContextRef, or NULL on error.
 */
__nullable
SSLContextRef
SSLCreateContext(CFAllocatorRef __nullable alloc, SSLProtocolSide protocolSide, SSLConnectionType connectionType)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));


#if TARGET_OS_OSX
/*
 * @function SSLNewContext
 * @abstract Create a new session context.
 * @note
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * NOTE: this function is not available on iOS, and should be considered
 * deprecated on Mac OS X. Your code should use SSLCreateContext instead.
 *
 * @param isServer Flag indicating if the context is for the server (true) or client (false).
 * @param contextPtr Pointer to SSLContextRef where result will be stored.
 * @return errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLNewContext				(Boolean 			isServer,
							 SSLContextRef 		* __nonnull CF_RETURNS_RETAINED contextPtr)	/* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLDisposeContext
 * @abstract Dispose of a session context.
 * @note
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * NOTE: this function is not available on iOS, and should be considered
 * deprecated on Mac OS X. Your code should use CFRelease to dispose a session
 * created with SSLCreateContext.
 *
 * @param context A SSLContextRef to deallocate and destroy.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLDisposeContext			(SSLContextRef		context)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

#endif /* MAC OS X */

/*
 * @function SSLGetSessionState
 * @abstract Determine the state of an SSL/DTLS session.
 * @param context A valid SSLContextRef.
 * @param state Output pointer to store the SSLSessionState.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetSessionState			(SSLContextRef		context,
							 SSLSessionState	*state)		/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetSessionOption
 * @abstract Set options for an SSL session. Must be called prior to SSLHandshake();
 *   subsequently cannot be called while session is active.
 * @param context A valid SSLContextRef.
 * @param option An option enumeration value.
 * @param value Value of the SSLSessionOption.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetSessionOption			(SSLContextRef		context,
							 SSLSessionOption	option,
							 Boolean			value)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.6, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetSessionOption
 * @abstract Determine current value for the specified option in a given SSL session.
 * @param context A valid SSLContextRef.
 * @param option An option enumeration value.
 * @param value Pointer to a Boolean where the SSLSessionOption value is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetSessionOption			(SSLContextRef		context,
							 SSLSessionOption	option,
							 Boolean			*value)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.6, 10.15), ios(5.0, 13.0));

/********************************************************************
 *** Session context configuration, common to client and servers. ***
 ********************************************************************/

/*
 * @function SSLSetIOFuncs
 * @abstract Specify functions which do the network I/O. Must be called prior
 *    to SSLHandshake(); subsequently cannot be called while a session is
 *    active.
 * @param context A valid SSLContextRef.
 * @param readFunc Pointer to a SSLReadFunc.
 * @param writeFunc Pointer to a SSLWriteFunc.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetIOFuncs				(SSLContextRef		context,
							 SSLReadFunc 		readFunc,
							 SSLWriteFunc		writeFunc)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));


/*
 * @function SSLSetSessionConfig
 * @absttact Set a predefined configuration for the SSL Session
 * @note This currently affect enabled protocol versions,
 *   enabled ciphersuites, and the kSSLSessionOptionFallback
 *   session option.
 * @param context A valid SSLContextRef.
 * @param config String name of constant TLS handshake configuration, e.g., kSSLSessionConfig_standard.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetSessionConfig(SSLContextRef context,
                    CFStringRef config)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.12, 10.15), ios(10.0, 13.0));

/*
 * @function SSLSetProtocolVersionMin
 * @abstract Set the minimum SSL protocol version allowed. Optional.
 *   The default is the lower supported protocol.
 * @note This can only be called when no session is active.
 *
 *   For TLS contexts, legal values for minVersion are :
 *	  	kSSLProtocol3
 * 		kTLSProtocol1
 * 		kTLSProtocol11
 * 		kTLSProtocol12
 *
 *   For DTLS contexts, legal values for minVersion are :
 *      kDTLSProtocol1
 * @param context A valid SSLContextRef.
 * @param minVersion Minimum TLS protocol version.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetProtocolVersionMin  (SSLContextRef      context,
                           SSLProtocol        minVersion)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetProtocolVersionMin
 * @abstract Get minimum protocol version allowed
 * @param context A valid SSLContextRef.
 * @param minVersion Pointer to SSLProtocol value where the minimum protocol version is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetProtocolVersionMin  (SSLContextRef      context,
                           SSLProtocol        *minVersion)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetProtocolVersionMax
 * @abstract Set the maximum SSL protocol version allowed. Optional.
 *   The default is the highest supported protocol.
 * @note
 *   This can only be called when no session is active.
 *
 *   For TLS contexts, legal values for maxVersion are :
 *		kSSLProtocol3
 * 		kTLSProtocol1
 * 		kTLSProtocol11
 * 		kTLSProtocol12
 *
 *   For DTLS contexts, legal values for maxVersion are :
 *      kDTLSProtocol1
 * @param context A valid SSLContextRef.
 * @param maxVersion Maximum TLS protocol version.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetProtocolVersionMax  (SSLContextRef      context,
                           SSLProtocol        maxVersion)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetProtocolVersionMax
 * @abstract Get maximum protocol version allowed
 * @param context A valid SSLContextRef.
 * @param maxVersion Pointer to SSLProtocol value where the maximum protocol version is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetProtocolVersionMax  (SSLContextRef      context,
                           SSLProtocol        *maxVersion)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));


#if TARGET_OS_OSX
/*
 * @function SSLSetProtocolVersionEnabled
 * @abstract Set allowed SSL protocol versions. Optional.
 * @discussion Specifying kSSLProtocolAll for SSLSetProtocolVersionEnabled results in
 *   specified 'enable' boolean to be applied to all supported protocols.
 *   The default is "all supported protocols are enabled".
 *   This can only be called when no session is active.
 *
 *   Legal values for protocol are :
 *		kSSLProtocol2
 *		kSSLProtocol3
 * 		kTLSProtocol1
 *		kSSLProtocolAll
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note this function is not available on iOS, and should be considered
 * deprecated on Mac OS X. You can use SSLSetProtocolVersionMin and/or
 * SSLSetProtocolVersionMax to specify which protocols are enabled.
 * @param context A valid SSLContextRef.
 * @param protocol A SSLProtocol enumerated value.
 * @param enable Boolean to enable or disable the designated protocol.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetProtocolVersionEnabled (SSLContextRef 	context,
							 SSLProtocol		protocol,
							 Boolean			enable)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * Obtain a value specified in SSLSetProtocolVersionEnabled.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * NOTE: this function is not available on iOS, and should be considered
 * deprecated on Mac OS X. You can use SSLGetProtocolVersionMin and/or
 * SSLGetProtocolVersionMax to check whether a protocol is enabled.
 */
OSStatus
SSLGetProtocolVersionEnabled(SSLContextRef 		context,
							 SSLProtocol		protocol,
							 Boolean			*enable)	/* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLSetProtocolVersion
 * @abstract Get/set SSL protocol version; optional. Default is kSSLProtocolUnknown,
 *   in which case the highest possible version is attempted, but a lower
 *   version is accepted if the peer requires it.
 * @discussion SSLSetProtocolVersion cannot be called when a session is active.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note this function is not available on iOS, and deprecated on Mac OS X 10.8.
 *   Use SSLSetProtocolVersionMin and/or SSLSetProtocolVersionMax to specify
 *   which protocols are enabled.
 * @param context A valid SSLContextRef.
 * @param protocol A SSLProtocol enumerated value.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetProtocolVersion		(SSLContextRef 		context,
							 SSLProtocol		version)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.8));

/*
 * @function SSLGetProtocolVersion
 * @abstract Obtain the protocol version specified in SSLSetProtocolVersion.
 * @discussion If SSLSetProtocolVersionEnabled() has been called for this session,
 *   SSLGetProtocolVersion() may return errSecParam if the protocol enable
 *   state can not be represented by the SSLProtocol enums (e.g.,
 *   SSL2 and TLS1 enabled, SSL3 disabled).
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note this function is not available on iOS, and deprecated on Mac OS X 10.8.
 *   Use SSLGetProtocolVersionMin and/or SSLGetProtocolVersionMax to check
 *   whether a protocol is enabled.
 * @param context A valid SSLContextRef.
 * @param protocol A SSLProtocol enumerated value pointer to store the protocol version.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetProtocolVersion		(SSLContextRef		context,
							 SSLProtocol		*protocol)	/* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.8));

#endif /* MAC OS X */

/*
 * @function SSLSetCertificate
 * @abstract Specify this connection's certificate(s).
 * @discussion This is mandatory for server connections,and optional for
 *   clients. Specifying a certificate for a client enables SSL client-side
 *   authentication. The end-entity cert is in certRefs[0]. Specifying a root
 *   cert is optional; if it's not specified, the root cert which verifies the
 *   cert chain specified here must be present in the system-wide set of trusted
 *   anchor certs.
 *
 *   The certRefs argument is a CFArray containing SecCertificateRefs,
 *   except for certRefs[0], which is a SecIdentityRef.
 *
 *   Must be called prior to SSLHandshake(), or immediately after
 *   SSLHandshake has returned errSSLClientCertRequested (i.e. before the
 *   handshake is resumed by calling SSLHandshake again.)
 *
 *   SecureTransport assumes the following:
 *
 *    -- The certRef references remain valid for the lifetime of the session.
 *    -- The certificate specified in certRefs[0] is capable of signing.
 *    -- The required capabilities of the certRef[0], and of the optional cert
 *       specified in SSLSetEncryptionCertificate (see below), are highly
 *       dependent on the application. For example, to work as a server with
 *       Netscape clients, the cert specified here must be capable of both
 *       signing and encrypting.
 * @param context A valid SSLContextRef.
 * @param certRefs An array of SecCertificateRef instances.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetCertificate			(SSLContextRef		context,
							 CFArrayRef			_Nullable certRefs)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetConnection
 * @abstract Specify I/O connection - a socket, endpoint, etc., which is
 * managed by caller.
 * @discussion On the client side, it's assumed that communication
 *   has been established with the desired server on this connection.
 *   On the server side, it's assumed that an incoming client request
 *   has been established.
 *
 *   Must be called prior to SSLHandshake(); subsequently can only be
 *   called when no session is active.
 * @param context A valid SSLContextRef.
 * @param connection A SSLConnectionRef.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetConnection			(SSLContextRef                  context,
							 SSLConnectionRef __nullable	connection)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetConnection
 * @abstract Retrieve the I/O connection managed managed by the caller.
 * @param context A valid SSLContextRef.
 * @param connection A SSLConnectionRef pointer.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetConnection			(SSLContextRef		context,
							 SSLConnectionRef	* __nonnull CF_RETURNS_NOT_RETAINED connection)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetPeerDomainName
 * @abstract Specify the fully qualified doman name of the peer, e.g., "store.apple.com."
 * @discussion Optional; used to verify the common name field in peer's certificate.
 *   Name is in the form of a C string; NULL termination optional, i.e.,
 *   peerName[peerNameLen+1] may or may not have a NULL. In any case peerNameLen
 *   is the number of bytes of the peer domain name.
 * @param context A valid SSLContextRef.
 * @param peerName A C string carrying the peer domain name.
 * @param peerNameLen Length of the peer domain name string.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetPeerDomainName		(SSLContextRef		context,
							 const char			* __nullable peerName,
							 size_t				peerNameLen)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetPeerDomainNameLength
 * @abstract Determine the buffer size needed for SSLGetPeerDomainName().
 * @param context A valid SSLContextRef.
 * @param peerNameLen Pointer to where the length of the peer domain name string is stored
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetPeerDomainNameLength	(SSLContextRef		context,
							 size_t				*peerNameLen)	// RETURNED
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetPeerDomainName
 * @abstract Obtain the value specified in SSLSetPeerDomainName().
 * @param context A valid SSLContextRef.
 * @param peerName Pointer to where the peer domain name is stored.
 * @param peerNameLen Pointer to where the length of the peer domain name string is stored,
 *        up to the length specified by peerNameLen (on input).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetPeerDomainName		(SSLContextRef		context,
							 char				*peerName,		// returned here
							 size_t				*peerNameLen)	// IN/OUT
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLCopyRequestedPeerNameLength
 * @abstract [Server Only] obtain the hostname specified by the client in the ServerName extension (SNI)
 * @param context A valid SSLContextRef.
 * @param peerNameLen Pointer to where the length of the requested peer domain name string
 *        is stored, up to the length specified by peerNameLen (on input).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyRequestedPeerNameLength  (SSLContextRef  ctx,
                                 size_t         *peerNameLen)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.11, 10.15), ios(9.0, 13.0));

/*
 * @function SSLCopyRequestedPeerName
 * @abstract Determine the buffer size needed for SSLCopyRequestedPeerNameLength().
 * @param context A valid SSLContextRef.
 * @param peerName Pointer to where the requested peer domain name is stored.
 * @param peerNameLen Pointer to where the length of the requested peer domain name string
 *        is stored, up to the length specified by peerNameLen (on input).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyRequestedPeerName    (SSLContextRef      context,
                             char               *peerName,
                             size_t             *peerNameLen)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.11, 10.15), ios(9.0, 13.0));


/*
 * @function SSLSetDatagramHelloCookie
 * @abstract Specify the Datagram TLS Hello Cookie.
 * @discussion This is to be called for server side only and is optional.
 *  The default is a zero len cookie. The maximum cookieLen is 32 bytes.
 * @param context A valid SSLContextRef.
 * @param cookie Pointer to opaque cookie data.
 * @param cookieLen Length of cookie data.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetDatagramHelloCookie	(SSLContextRef		dtlsContext,
                             const void         * __nullable cookie,
                             size_t             cookieLen)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetMaxDatagramRecordSize
 * @abstract Specify the maximum record size, including all DTLS record headers.
 * @discussion This should be set appropriately to avoid fragmentation
 *   of Datagrams during handshake, as fragmented datagrams may
 *   be dropped by some network.
 * @note This is for Datagram TLS only
 * @param context A valid SSLContextRef.
 * @param maxSize Maximum size of datagram record(s).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetMaxDatagramRecordSize (SSLContextRef		dtlsContext,
                             size_t             maxSize)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetMaxDatagramRecordSize
 * @abstract Get the maximum record size, including all Datagram TLS record headers.
 * @note This is for Datagram TLS only
 * @param context A valid SSLContextRef.
 * @param maxSize Pointer where maximum size of datagram record(s) is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetMaxDatagramRecordSize (SSLContextRef		dtlsContext,
                             size_t             *maxSize)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetNegotiatedProtocolVersion
 * @abstract Obtain the actual negotiated protocol version of the active
 *   session, which may be different that the value specified in
 *   SSLSetProtocolVersion(). Returns kSSLProtocolUnknown if no
 *   SSL session is in progress.
 * @param context A valid SSLContextRef.
 * @param protocol Pointer where negotiated SSLProtocol is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetNegotiatedProtocolVersion		(SSLContextRef		context,
									 SSLProtocol		*protocol)	/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetNumberSupportedCiphers
 * @abstract Determine number and values of all of the SSLCipherSuites we support.
 *    Caller allocates output buffer for SSLGetSupportedCiphers() and passes in
 *    its size in *numCiphers. If supplied buffer is too small, errSSLBufferOverflow
 *    will be returned.
 * @param context A valid SSLContextRef.
 * @param numCiphers Pointer where number of supported ciphers is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetNumberSupportedCiphers (SSLContextRef			context,
							  size_t				*numCiphers)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetSupportedCiphers
 * @abstract Get the supported ciphers.
 * @param context A valid SSLContextRef.
 * @param ciphers Pointer to array of SSLCipherSuite values where supported ciphersuites
 *                are stored. This array size is specified by the input value of numCiphers.
 * @param numCiphers Pointer where number of supported ciphers is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetSupportedCiphers		 (SSLContextRef			context,
							  SSLCipherSuite		*ciphers,		/* RETURNED */
							  size_t				*numCiphers)	/* IN/OUT */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetNumberEnabledCiphers
 * @abstract Determine number and values of all of the SSLCipherSuites currently enabled.
 *    Caller allocates output buffer for SSLGetEnabledCiphers() and passes in
 *    its size in *numCiphers. If supplied buffer is too small, errSSLBufferOverflow
 *    will be returned.
 * @param context A valid SSLContextRef.
 * @param numCiphers Pointer where number of enabled ciphers is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetNumberEnabledCiphers     (SSLContextRef       context,
                                size_t              *numCiphers)
__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetEnabledCiphers
 * @abstract Specify a (typically) restricted set of SSLCipherSuites to be enabled by
 *    the current SSLContext. Can only be called when no session is active. Default
 *    set of enabled SSLCipherSuites is the same as the complete set of supported
 *    SSLCipherSuites as obtained by SSLGetSupportedCiphers().
 * @param context A valid SSLContextRef.
 * @param ciphers Array of enabled SSLCipherSuite values. This array size is specified
 *                by the input value of numCiphers.
 * @param numCiphers Pointer where number of enabled ciphers is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetEnabledCiphers		(SSLContextRef			context,
							 const SSLCipherSuite	*ciphers,
							 size_t					numCiphers)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetEnabledCiphers
 * @abstract Get the set of supported ciphersuites.
 * @param context A valid SSLContextRef.
 * @param ciphers Pointer to array of SSLCipherSuite values where enabled ciphersuites
 *                are stored. This array size is specified by the input value of numCiphers.
 * @param numCiphers Pointer where number of enabled ciphers is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetEnabledCiphers		(SSLContextRef			context,
							 SSLCipherSuite			*ciphers,		/* RETURNED */
							 size_t					*numCiphers)	/* IN/OUT */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetSessionTicketsEnabled
 * @abstract Forcibly enable or disable session ticket resumption.
 * @note By default, session tickets are disabled.
 * @param context A valid SSLContextRef.
 * @param enabled Boolean indicating if ticket support is enabled (true) or not (false).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetSessionTicketsEnabled     (SSLContextRef          context,
                                 Boolean                enabled)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.13, 10.15), ios(11.0, 13.0));


#if TARGET_OS_OSX
/*
 * @function SSLSetEnableCertVerify
 * @abstract Enable/disable peer certificate chain validation. Default is enabled.
 * @discussion If caller disables, it is the caller's responsibility to call
 *    SSLCopyPeerCertificates() upon successful completion of the handshake
 *    and then to perform external validation of the peer certificate
 *    chain before proceeding with data transfer.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To disable peer certificate chain validation, you
 *    can instead use SSLSetSessionOption to set kSSLSessionOptionBreakOnServerAuth
 *    to true. This will disable verification and cause SSLHandshake to return with
 *    an errSSLServerAuthCompleted result when the peer certificates have been
 *    received; at that time, you can choose to evaluate peer trust yourself, or
 *    simply call SSLHandshake again to proceed with the handshake.
 * @param context A valid SSLContextRef.
 * @param enableVerify Boolean indicating if certificate verifiation is enabled (true) or disabled (false).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetEnableCertVerify		(SSLContextRef 			context,
							 Boolean				enableVerify)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLGetEnableCertVerify
 * @abstract Check whether peer certificate chain validation is enabled.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To check whether peer certificate chain validation
 *    is enabled in a context, call SSLGetSessionOption to obtain the value of
 *    the kSSLSessionOptionBreakOnServerAuth session option flag. If the value
 *    of this option flag is true, then verification is disabled.
 * @param context A valid SSLContextRef.
 * @param enableVerify Pointer to Boolean where enable bit is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetEnableCertVerify		(SSLContextRef 			context,
							 Boolean				*enableVerify)	/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLSetAllowsExpiredCerts
 * @abstract Specify the option of ignoring certificates' "expired" times.
 * @discussion This is a common failure in the real SSL world. Default setting for this
 *    flag is false, meaning expired certs result in an errSSLCertExpired error.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To ignore expired certificate errors, first disable
 *    Secure Transport's automatic verification of peer certificates by calling
 *    SSLSetSessionOption to set kSSLSessionOptionBreakOnServerAuth to true. When
 *    SSLHandshake subsequently returns an errSSLServerAuthCompleted result,
 *    your code should obtain the SecTrustRef for the peer's certificates and
 *    perform a custom trust evaluation with SecTrust APIs (see SecTrust.h).
 *    The SecTrustSetOptions function allows you to specify that the expiration
 *    status of certificates should be ignored for this evaluation.
 *
 * Example:
 *
 *	status = SSLSetSessionOption(ctx, kSSLSessionOptionBreakOnServerAuth, true);
 *	do {
 *		status = SSLHandshake(ctx);
 *
 *		if (status == errSSLServerAuthCompleted) {
 *			SecTrustRef peerTrust = NULL;
 *			status = SSLCopyPeerTrust(ctx, &peerTrust);
 *			if (status == errSecSuccess) {
 *				SecTrustResultType trustResult;
 *				// set flag to allow expired certificates
 *				SecTrustSetOptions(peerTrust, kSecTrustOptionAllowExpired);
 *				status = SecTrustEvaluate(peerTrust, &trustResult);
 *				if (status == errSecSuccess) {
 *					// A "proceed" result means the cert is explicitly trusted,
 *					// e.g. "Always Trust" was clicked;
 *					// "Unspecified" means the cert has no explicit trust settings,
 *					// but is implicitly OK since it chains back to a trusted root.
 *					// Any other result means the cert is not trusted.
 *					//
 *					if (trustResult == kSecTrustResultProceed ||
 *						trustResult == kSecTrustResultUnspecified) {
 *						// certificate is trusted
 *						status = errSSLWouldBlock; // so we call SSLHandshake again
 *					} else if (trustResult == kSecTrustResultRecoverableTrustFailure) {
 *						// not trusted, for some reason other than being expired;
 *						// could ask the user whether to allow the connection here
 *						//
 *						status = errSSLXCertChainInvalid;
 *					} else {
 *						// cannot use this certificate (fatal)
 *						status = errSSLBadCert;
 *					}
 *				}
 *				if (peerTrust) {
 *					CFRelease(peerTrust);
 *				}
 *			}
 *		} // errSSLServerAuthCompleted
 *
 *	} while (status == errSSLWouldBlock);
 * @param context A valid SSLContextRef.
 * @param allowsExpired Boolean indicating if expired certificates are allowed (true) or not (false).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetAllowsExpiredCerts	(SSLContextRef		context,
							 Boolean			allowsExpired)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLGetAllowsExpiredCerts
 * @abstract Obtain the current value of an SSLContext's "allowExpiredCerts" flag.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X.
 * @param context A valid SSLContextRef.
 * @param allowsExpired Pointer to Boolean where the expired certificate allowance Boolean is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetAllowsExpiredCerts	(SSLContextRef		context,
							 Boolean			*allowsExpired)	/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLSetAllowsExpiredRoots
 * @abstract Similar to SSLSetAllowsExpiredCerts, SSLSetAllowsExpiredRoots allows the
 *    option of ignoring "expired" status for root certificates only.
 * @discussion Default setting is false, i.e., expired root certs result in an
 *    errSSLCertExpired error.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To ignore expired certificate errors, first disable
 *    Secure Transport's automatic verification of peer certificates by calling
 *    SSLSetSessionOption to set kSSLSessionOptionBreakOnServerAuth to true. When
 *    SSLHandshake subsequently returns an errSSLServerAuthCompleted result,
 *    your code should obtain the SecTrustRef for the peer's certificates and
 *    perform a custom trust evaluation with SecTrust APIs (see SecTrust.h).
 *    The SecTrustSetOptions function allows you to specify that the expiration
 *    status of certificates should be ignored for this evaluation.
 *
 *    See the description of the SSLSetAllowsExpiredCerts function (above)
 *    for a code example. The kSecTrustOptionAllowExpiredRoot option can be used
 *    instead of kSecTrustOptionAllowExpired to allow expired roots only.
 * @param context A valid SSLContextRef.
 * @param allowsExpired Boolean indicating if expired roots are allowed.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetAllowsExpiredRoots	(SSLContextRef		context,
							 Boolean			allowsExpired)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLGetAllowsExpiredRoots
 * @abstract Obtain the current value of an SSLContext's "allow expired roots" flag.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X.
 * @param context A valid SSLContextRef.
 * @param allowsExpired Pointer to Boolean where the expired root certificate allowance
 *                      Boolean is stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetAllowsExpiredRoots	(SSLContextRef		context,
							 Boolean			*allowsExpired)	/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLSetAllowsAnyRoot
 * @abstract Specify option of allowing for an unknown root cert, i.e., one which
 * this software can not verify as one of a list of known good root certs.
 * @discussion Default for this flag is false, in which case one of the following two
 *    errors may occur:
 *       -- The peer returns a cert chain with a root cert, and the chain
 *          verifies to that root, but the root is not one of our trusted
 *          roots. This results in errSSLUnknownRootCert on handshake.
 *       -- The peer returns a cert chain which does not contain a root cert,
 *          and we can't verify the chain to one of our trusted roots. This
 *          results in errSSLNoRootCert on handshake.
 *
 *    Both of these error conditions are ignored when the AllowAnyRoot flag is
 *    true, allowing connection to a totally untrusted peer.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To ignore unknown root cert errors, first disable
 *    Secure Transport's automatic verification of peer certificates by calling
 *    SSLSetSessionOption to set kSSLSessionOptionBreakOnServerAuth to true. When
 *    SSLHandshake subsequently returns an errSSLServerAuthCompleted result,
 *    your code should obtain the SecTrustRef for the peer's certificates and
 *    perform a custom trust evaluation with SecTrust APIs (see SecTrust.h).
 *
 *    See the description of the SSLSetAllowsExpiredCerts function (above)
 *    for a code example. Note that an unknown root certificate will cause
 *    SecTrustEvaluate to report kSecTrustResultRecoverableTrustFailure as the
 *    trust result.
 * @param context A valid SSLContextRef.
 * @param anyRoot Boolean indicating if any root is allowed (true) or not (false).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetAllowsAnyRoot			(SSLContextRef		context,
							 Boolean			anyRoot)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLGetAllowsAnyRoot
 * @abstract Obtain the current value of an SSLContext's "allow any root" flag.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X.
 * @param context A valid SSLContextRef.
 * @param anyRoot Pointer to Boolean to store any root allowance Boolean.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetAllowsAnyRoot			(SSLContextRef		context,
							 Boolean			*anyRoot) /* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLSetTrustedRoots
 * @abstract Augment or replace the system's default trusted root certificate set
 * for this session.
 * @discussion If replaceExisting is true, the specified roots will
 *    be the only roots which are trusted during this session. If replaceExisting
 *    is false, the specified roots will be added to the current set of trusted
 *    root certs. If this function has never been called, the current trusted
 *    root set is the same as the system's default trusted root set.
 *    Successive calls with replaceExisting false result in accumulation
 *    of additional root certs.
 *
 *    The trustedRoots array contains SecCertificateRefs.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To trust specific roots in a session, first disable
 *    Secure Transport's automatic verification of peer certificates by calling
 *    SSLSetSessionOption to set kSSLSessionOptionBreakOnServerAuth to true. When
 *    SSLHandshake subsequently returns an errSSLServerAuthCompleted result,
 *    your code should obtain the SecTrustRef for the peer's certificates and
 *    perform a custom trust evaluation with SecTrust APIs (see SecTrust.h).
 *
 *    See the description of the SSLSetAllowsExpiredCerts function (above)
 *    for a code example. You can call SecTrustSetAnchorCertificates to
 *    augment the system's trusted root set, or SecTrustSetAnchorCertificatesOnly
 *    to make these the only trusted roots, prior to calling SecTrustEvaluate.
 * @param context A valid SSLContextRef.
 * @param trustedRoots Array of SecCertificateRef roots.
 * @param replaceExisting Boolean indicating if provided roots should override all others for this connection.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetTrustedRoots			(SSLContextRef 		context,
							 CFArrayRef 		trustedRoots,
							 Boolean 			replaceExisting)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLCopyTrustedRoots
 * @abstract Obtain an array of SecCertificateRefs representing the current
 *    set of trusted roots.
 * @discussion If SSLSetTrustedRoots() has never been called
 *    for this session, this returns the system's default root set.
 *    Caller must CFRelease the returned CFArray.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To get the current set of trusted roots, call the
 *    SSLCopyPeerTrust function to obtain the SecTrustRef for the peer certificate
 *    chain, then SecTrustCopyCustomAnchorCertificates (see SecTrust.h).
 * @param context A valid SSLContextRef.
 * @param trustedRoots Array of SecCertificateRef roots.
 * @param replaceExisting Boolean indicating if provided roots should override all others for this connection.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyTrustedRoots			(SSLContextRef 		context,
							 CFArrayRef 		* __nonnull CF_RETURNS_RETAINED trustedRoots)	/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.5, 10.9));

/*
 * @function SSLCopyPeerCertificates
 * @abstract Request peer certificates. Valid anytime, subsequent to a handshake attempt.
 * @discussion The certs argument is a CFArray containing SecCertificateRefs.
 *    Caller must CFRelease the returned array.
 *
 *    The cert at index 0 of the returned array is the subject (end
 *    entity) cert; the root cert (or the closest cert to it) is at
 *    the end of the returned array.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. To get peer certificates, call SSLCopyPeerTrust
 *    to obtain the SecTrustRef for the peer certificate chain, then use the
 *    SecTrustCopyCertificateChain to retrieve individual certificates in
 *    the chain (see SecTrust.h).
 * @param context A valid SSLContextRef.
 * @param certs Pointer to CFArrayRef that will store a reference to the peer's certificates.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyPeerCertificates		(SSLContextRef 		context,
							 CFArrayRef			* __nonnull CF_RETURNS_RETAINED certs)		/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.5, 10.9));

#endif /* MAC OS X */

/*
 * @function SSLCopyPeerTrust
 * @abstract Obtain a SecTrustRef representing peer certificates. Valid anytime,
 *    subsequent to a handshake attempt. Caller must CFRelease the returned
 *    trust reference.
 * @discussion The returned trust reference will have already been evaluated for
 *    you, unless one of the following is true:
 *    - Your code has disabled automatic certificate verification, by calling
 *      SSLSetSessionOption to set kSSLSessionOptionBreakOnServerAuth to true.
 *    - Your code has called SSLSetPeerID, and this session has been resumed
 *      from an earlier cached session.
 *
 *    In these cases, your code should call SecTrustEvaluate prior to
 *    examining the peer certificate chain or trust results (see SecTrust.h).
 *
 * @note If you have not called SSLHandshake at least once prior to
 *    calling this function, the returned trust reference will be NULL.
 * @param context A valid SSLContextRef.
 * @param trust Pointer to SecTrustRef where peer's SecTrustRef is copied (retained).
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyPeerTrust			(SSLContextRef 		context,
							 SecTrustRef		* __nonnull CF_RETURNS_RETAINED trust)		/* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.6, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetPeerID
 * @discussion Specify some data, opaque to this library, which is sufficient
 *    to uniquely identify the peer of the current session. An example
 *    would be IP address and port, stored in some caller-private manner.
 *    To be optionally called prior to SSLHandshake for the current
 *    session. This is mandatory if this session is to be resumable.
 *
 *    SecureTransport allocates its own copy of the incoming peerID. The
 *    data provided in *peerID, while opaque to SecureTransport, is used
 *    in a byte-for-byte compare to other previous peerID values set by the
 *    current application. Matching peerID blobs result in SecureTransport
 *    attempting to resume an SSL session with the same parameters as used
 *    in the previous session which specified the same peerID bytes.
 * @param context A valid SSLContextRef.
 * @param peerID Opaque peer ID.
 * @param peerIDLen Length of opaque peer ID.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetPeerID				(SSLContextRef 		context,
							 const void 		* __nullable peerID,
							 size_t				peerIDLen)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetPeerID
 * @abstract Obtain current PeerID. Returns NULL pointer, zero length if
 *    SSLSetPeerID has not been called for this context.
 * @param context A valid SSLContextRef.
 * @param peerID Pointer to storage for the peer ID.
 * @param peerIDLen Pointer to storage for the peer ID length.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetPeerID				(SSLContextRef 		context,
							 const void 		* __nullable * __nonnull peerID,
							 size_t				*peerIDLen)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetNegotiatedCipher
 * @abstract Obtain the SSLCipherSuite (e.g., SSL_RSA_WITH_DES_CBC_SHA) negotiated
 *    for this session. Only valid when a session is active.
 * @param context A valid SSLContextRef.
 * @param cipherSuite Pointer to storage for negotiated SSLCipherSuite.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetNegotiatedCipher		(SSLContextRef 		context,
							 SSLCipherSuite 	*cipherSuite)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetALPNProtocols
 * @abstract Set the ALPN protocols to be passed in the ALPN negotiation.
 * @discussion This is the list of supported application-layer protocols supported.
 *
 *    The protocols parameter must be an array of CFStringRef values
 *    with ASCII-encoded reprensetations of the supported protocols, e.g., "http/1.1".
 *
 * @note See RFC 7301 for more information.
 * @param context A valid SSLContextRef.
 * @param protocols Array of CFStringRefs carrying application protocols.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetALPNProtocols         (SSLContextRef      context,
                             CFArrayRef         protocols)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.13, 10.15), ios(11.0, 13.0));

/*
 * @function SSLCopyALPNProtocols
 * @abstract Get the ALPN protocols associated with this SSL context.
 * @discussion This is the list of supported application-layer protocols supported.
 *
 *    The resultant protocols array will contain CFStringRef values containing
 *    ASCII-encoded representations of the supported protocols, e.g., "http/1.1".
 *
 *    See RFC 7301 for more information.
 *
 * @note The `protocols` pointer must be NULL, otherwise the copy will fail.
 *    This function will allocate memory for the CFArrayRef container
 *    if there is data to provide. Otherwise, the pointer will remain NULL.
 * @param context A valid SSLContextRef.
 * @param protocols Pointer to a CFArrayRef where peer ALPN protocols are stored.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyALPNProtocols        (SSLContextRef      context,
                             CFArrayRef         __nullable * __nonnull protocols)           /* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.13, 10.15), ios(11.0, 13.0));

/*
 * @function SSLSetOCSPResponse
 * @abstract Set the OCSP response for the given SSL session.
 * @discussion The response parameter must be a non-NULL CFDataRef containing the
 *    bytes of the OCSP response.
 * @param context A valid SSLContextRef.
 * @param response CFDataRef carrying OCSP response.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetOCSPResponse          (SSLContextRef      context,
                             CFDataRef          __nonnull response)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.13, 10.15), ios(11.0, 13.0));

/********************************************************
 *** Session context configuration, server side only. ***
 ********************************************************/

/*
 * @function SSLSetEncryptionCertificate
 * @discussion This function is deprecated in OSX 10.11 and iOS 9.0 and
 *    has no effect on the TLS handshake since OSX 10.10 and
 *    iOS 8.0. Using separate RSA certificates for encryption
 *    and signing is no longer supported.
 * @param context A valid SSLContextRef.
 * @param certRefs Array of certificates.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetEncryptionCertificate	(SSLContextRef		context,
							 CFArrayRef			certRefs)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.11), ios(5.0, 9.0));

/*
 * @enum SSLAuthenticate
 * @discussion Optional; Default is kNeverAuthenticate.
 *    Can only be called when no session is active.
 */
typedef CF_ENUM(int, SSLAuthenticate) {
	kNeverAuthenticate,			/* skip client authentication */
	kAlwaysAuthenticate,		/* require it */
	kTryAuthenticate			/* try to authenticate, but not an error if client doesn't have a cert */
};

/*
 * @function SSLSetClientSideAuthenticate
 * @abstract Specify requirements for client-side authentication.
 * @param context A valid SSLContextRef.
 * @param auth A SSLAuthenticate enumeration value.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetClientSideAuthenticate 	(SSLContextRef		context,
								 SSLAuthenticate	auth)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLAddDistinguishedName
 * @abstract Add a DER-encoded distinguished name to list of acceptable names
 *    to be specified in requests for client certificates.
 * @param context A valid SSLContextRef.
 * @param derDN A DER-encoded Distinguished Name blob.
 * @param derDNLen Length of the Distinguished Name blob.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLAddDistinguishedName		(SSLContextRef 		context,
							 const void 		* __nullable derDN,
							 size_t 			derDNLen)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.4, 10.15), ios(5.0, 13.0));


#if TARGET_OS_OSX
/*
 * @function SSLSetCertificateAuthorities
 * @abstract Add a SecCertificateRef, or a CFArray of them, to a server's list
 *    of acceptable Certificate Authorities (CAs) to present to the client
 *    when client authentication is performed.
 * @discussion If replaceExisting is true, the specified certificate(s) will
 *    replace a possible existing list of acceptable CAs. If replaceExisting
 *    is false, the specified certificate(s) will be appended to the existing
 *    list of acceptable CAs, if any.
 *
 *    Returns errSecParam if this is called on a SSLContextRef which
 *    is configured as a client, or when a session is active.
 *
 * @note this function is currently not available on iOS.
 * @param context A valid SSLContextRef.
 * @param certificateOrARray Either a SecCertificateRef (or CFArrayRef of them).
 * @param replaceExisting Boolean indicating if existing CAs should be overruled for this connection.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetCertificateAuthorities(SSLContextRef		context,
							 CFTypeRef			certificateOrArray,
							 Boolean 			replaceExisting)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.5, 10.15));

/*
 * @function SSLCopyCertificateAuthorities
 * @abstract Obtain the certificates specified in SSLSetCertificateAuthorities(),
 *    if any. Returns a NULL array if SSLSetCertificateAuthorities() has not been called.
 * @discussion Caller must CFRelease the returned array.
 *
 * @note This function is currently not available on iOS.
 * @param context A valid SSLContextRef.
 * @param certificates Pointer to CFArrayRef storage for retained copy of CAs for this connection.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyCertificateAuthorities(SSLContextRef		context,
							  CFArrayRef		* __nonnull CF_RETURNS_RETAINED certificates)	/* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.5, 10.15));

#endif /* MAC OS X */

/*
 * @function SSLCopyDistinguishedNames
 * @abstract Obtain the list of acceptable distinguished names as provided by
 *    a server (if the SSLContextRef is configured as a client), or as
 *    specified by SSLSetCertificateAuthorities (if the SSLContextRef
 *    is configured as a server).
 * @discussion The returned array contains CFDataRefs, each of which represents
 *    one DER-encoded RDN. Caller must CFRelease the returned array.
 * @param context A valid SSLContextRef.
 * @param names Pointer to CFArrayRef storage for retained copy of Distinguished Names.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLCopyDistinguishedNames	(SSLContextRef		context,
							 CFArrayRef			* __nonnull CF_RETURNS_RETAINED names)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.5, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetClientCertificateState
 * @abstract Obtain client certificate exchange status.
 * @discussion Can be called any time.
 *    Reflects the *last* client certificate state change;
 *    subsequent to a renegotiation attempt by either peer, the state
 *    is reset to kSSLClientCertNone.
 * @param context A valid SSLContextRef.
 * @param clientState Pointer to SSLClientCertificateState storage.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetClientCertificateState	(SSLContextRef				context,
								 SSLClientCertificateState	*clientState)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.3, 10.15), ios(5.0, 13.0));


#if TARGET_OS_OSX
/*
 * @function SSLSetDiffieHellmanParams
 * @abstract Specify Diffie-Hellman parameters.
 * @discussion Optional; if we are configured to allow
 *    for D-H ciphers and a D-H cipher is negotiated, and this function has not
 *    been called, a set of process-wide parameters will be calculated. However
 *    that can take a long time (30 seconds).
 * @note This function is currently not available on iOS.
 * @param context A valid SSLContextRef.
 * @param clientState Pointer to SSLClientCertificateState storage.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetDiffieHellmanParams	(SSLContextRef			context,
                             const void 			* __nullable dhParams,
                             size_t					dhParamsLen)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15));

/*
 * @function SSLGetDiffieHellmanParams
 * @abstract Return parameter block specified in SSLSetDiffieHellmanParams.
 * @discussion Returned data is not copied and belongs to the SSLContextRef.
 * @note This function is currently not available on iOS.
 * @param context A valid SSLContextRef.
 * @param dhParams Pointer to storage for DH parameters (if set), of at length most |*dhParamsLen|.
 * @param dhParamsLen (Input and output) length of dhParams.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetDiffieHellmanParams	(SSLContextRef			context,
                             const void 			* __nullable * __nonnull dhParams,
                             size_t					*dhParamsLen)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15));

/*
 * @function SSLSetRsaBlinding
 * @abstract Enable/Disable RSA blinding.
 * @discussion This feature thwarts a known timing
 *    attack to which RSA keys are vulnerable; enabling it is a tradeoff
 *    between performance and security. The default for RSA blinding is
 *    enabled.
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X. RSA blinding is enabled unconditionally, as
 *    it prevents a known way for an attacker to recover the private key,
 *    and the performance gain of disabling it is negligible.
 * @param context A valid SSLContextRef.
 * @param blinding Boolean indicating if RSA blinding is enabled.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetRsaBlinding			(SSLContextRef			context,
                             Boolean				blinding)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

/*
 * @function SSLGetRsaBlinding
 * @abstract Get RSA blinding state.
 * @discussion See SSLSetRsaBlinding().
 *
 * ==========================
 * MAC OS X ONLY (DEPRECATED)
 * ==========================
 * @note This function is not available on iOS, and should be considered
 *    deprecated on Mac OS X.
 * @param context A valid SSLContextRef.
 * @param blinding Pointer to Boolean storage for RSA blinding state.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetRsaBlinding			(SSLContextRef			context,
                             Boolean				*blinding)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.9));

#endif /* MAC OS X */

/*******************************
 ******** I/O Functions ********
 *******************************/

/*
 * Note: depending on the configuration of the underlying I/O
 * connection, all SSL I/O functions can return errSSLWouldBlock,
 * indicating "not complete, nothing is wrong, except required
 * I/O hasn't completed". Caller may need to repeat I/Os as necessary
 * if the underlying connection has been configured to behave in
 * a non-blocking manner.
 */

/*
 * @function SSLHandshake
 * @abstract Perform the SSL handshake.
 * @discussion On successful return, session is ready for normal secure application
 *    I/O via SSLWrite and SSLRead.
 *
 *    Interesting error returns:
 *
 *     errSSLUnknownRootCert: Peer had a valid cert chain, but the root of
 *         the chain is unknown.
 *
 *     errSSLNoRootCert: Peer had a cert chain which did not end in a root.
 *
 *     errSSLCertExpired: Peer's cert chain had one or more expired certs.
 *
 *     errSSLXCertChainInvalid: Peer had an invalid cert chain (i.e.,
 *         signature verification within the chain failed, or no certs
 *         were found).
 *
 *     In all of the above errors, the handshake was aborted; the peer's
 *     cert chain is available via SSLCopyPeerTrust or SSLCopyPeerCertificates.
 *
 *     Other interesting result codes:
 *
 *     errSSLPeerAuthCompleted: Peer's cert chain is valid, or was ignored if
 *         cert verification was disabled via SSLSetEnableCertVerify. The application
 *         may decide to continue with the handshake (by calling SSLHandshake
 *         again), or close the connection at this point.
 *
 *     errSSLClientCertRequested: The server has requested a client certificate.
 *         The client may choose to examine the server's certificate and
 *         distinguished name list, then optionally call SSLSetCertificate prior
 *         to resuming the handshake by calling SSLHandshake again.
 *
 *    A return value of errSSLWouldBlock indicates that SSLHandshake has to be
 *    called again (and again and again until something else is returned).
 * @param context A valid SSLContextRef.
 * @result errSecSuccess on success, alternative error on failure or incomplete state.
 */
OSStatus
SSLHandshake				(SSLContextRef		context)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLReHandshake
 * @abstract Server Only: Request renegotation.
 * @discussion This will return an error if the server is already renegotiating, or if the session is closed.
 *    After this return without error, the application should call SSLHandshake() and/or SSLRead() as
 *    for the original handshake.
 * @param context A valid SSLContextRef.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLReHandshake				(SSLContextRef		context)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.12, 10.15), ios(10.0, 13.0));


/*
 * @function SSLWrite
 * @abstract Normal application-level write.
 * @discussion On both of these, a errSSLWouldBlock return and a partially completed
 *    transfer - or even zero bytes transferred - are NOT mutually exclusive.
 * @param context A valid SSLContextRef.
 * @param data Pointer to data to write.
 * @param dataLength Length of data to write.
 * @param processed Pointer to storage indicating how much data was written.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLWrite					(SSLContextRef		context,
							 const void *		__nullable data,
							 size_t				dataLength,
							 size_t 			*processed)		/* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLRead
 * @abstract * @abstract Normal application-level write.
 * @discussion Data is mallocd by caller; available size specified in
 *    dataLength; actual number of bytes read returned in
 *    *processed.
 * @param context A valid SSLContextRef.
 * @param data Pointer to storage where data can be read.
 * @param dataLength Length of data storage.
 * @param processed Pointer to storage indicating how much data was read.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLRead						(SSLContextRef		context,
							 void *				data,			/* RETURNED */
							 size_t				dataLength,
							 size_t 			*processed)		/* RETURNED */
    __SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetBufferedReadSize
 * @abstract Determine how much data the client can be guaranteed to
 *    obtain via SSLRead() without blocking or causing any low-level
 *    read operations to occur.
 * @param context A valid SSLContextRef.
 * @param bufferSize Pointer to store the amount of buffered data to be read.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetBufferedReadSize		(SSLContextRef context,
							 size_t *bufferSize)				/* RETURNED */
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLGetDatagramWriteSize
 * @abstract Determine how much data the application can be guaranteed to write
 *    with SSLWrite() without causing fragmentation. The value is based on
 *    the maximum Datagram Record size defined by the application with
 *    SSLSetMaxDatagramRecordSize(), minus the DTLS Record header size.
 * @param context A valid SSLContextRef (for DTLS).
 * @param bufferSize Pointer to store the amount of data that can be written.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLGetDatagramWriteSize		(SSLContextRef dtlsContext,
							 size_t *bufSize)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.8, 10.15), ios(5.0, 13.0));

/*
 * @function SSLClose
 * @abstract Terminate current SSL session.
 * @param context A valid SSLContextRef.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLClose					(SSLContextRef		context)
	__SECURETRANSPORT_API_DEPRECATED(macos(10.2, 10.15), ios(5.0, 13.0));

/*
 * @function SSLSetError
 * @abstract Set the status of a SSLContextRef.
 * @discussion This is to be done after handling steps of the SSL handshake such
 *    as server certificate validation.
 * @param context A valid SSLContextRef.
 * @param status Error status to set internally, which will be translated to an alert.
 * @result errSecSuccess on success, alternative error on failure.
 */
OSStatus
SSLSetError                 (SSLContextRef      context,
                             OSStatus           status)
    __SECURETRANSPORT_API_DEPRECATED(macos(10.13, 10.15), ios(11.0, 13.0));

#undef __SECURETRANSPORT_API_DEPRECATED

CF_IMPLICIT_BRIDGING_DISABLED
CF_ASSUME_NONNULL_END

#ifdef __cplusplus
}
#endif

#endif /* !_SECURITY_SECURETRANSPORT_H_ */
