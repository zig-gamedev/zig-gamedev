/*
	 File:	   CFNetwork/CFNetDiagnostics.h
 
	 Contains:   CFNetDiagnostics interface
  
	 Copyright:  Copyright (c) 2004-2017 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFNETDIAGNOSTICS__
#define __CFNETDIAGNOSTICS__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <stdint.h>


#if PRAGMA_ONCE
#pragma once
#endif

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

/*
 *  CFNetDiagnosticRef
 *  
 *  Discussion:
 *	This is the type used to describe the types of connection that
 *	clients may be querying about
 */
typedef struct __CFNetDiagnostic*	   CFNetDiagnosticRef;

/*
 *  CFNetDiagnosticStatusValues
 *  
 *  Discussion:
 *	Values for CFNetDiagnosticStatus
 */
typedef CF_ENUM(int, CFNetDiagnosticStatusValues) {

  /*
   * There is no status, but no error has occured
   */
  kCFNetDiagnosticNoErr	= 0,

  /*
   * An error occured that prevented the call from completing
   */
  kCFNetDiagnosticErr = -66560L,

  /*
   * The connection appears to be working
   */
  kCFNetDiagnosticConnectionUp = -66559L,
  kCFNetDiagnosticConnectionIndeterminate = -66558L,

  /*
   * The connection does not appear to be working
   */
  kCFNetDiagnosticConnectionDown = -66557L

} CF_DEPRECATED(10_4, 10_13, 2_0, 11_0);


/*
 *  CFNetDiagnosticStatus
 *  
 *  Discussion:
 *	Returned by the various status and diagnostic calls
 */
typedef CFIndex	CFNetDiagnosticStatus CF_DEPRECATED(10_4, 10_13, 2_0, 11_0);

/*
 *  CFNetDiagnosticCreateWithStreams()
 *  
 *  Discussion:
 *	Creates a CFNetDiagnosticRef from a pair of CFStreams. Either
 *	stream may be NULL. This is the preferred interface for creating
 *	a CFNetDiagnosticRef.
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CF allocator to use.
 *	
 *	readStream:
 *	  CFReadStreamRef referring to the failed connection. May be NULL.
 *	
 *	writeStream:
 *	  CFWriteStreamRef referring to the failed connection. May be
 *	  NULL.
 *  
 *  Result:
 *	A CFNetDiagnosticRef referring to the current networking issue.
 *  
 */
CFN_EXPORT CFNetDiagnosticRef 
CFNetDiagnosticCreateWithStreams(CFAllocatorRef __nullable alloc, CFReadStreamRef __nullable readStream, CFWriteStreamRef __nullable writeStream) CF_DEPRECATED(10_4, 10_13, 2_0, 11_0);


/*
 *  CFNetDiagnosticCreateWithURL()
 *  
 *  Discussion:
 *	Creates a CFNetDiagnosticRef based on a CFURLRef passed in by the
 *	application.
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CF allocator to use.
 *	
 *	url:
 *	  CFURLRef referring to the failed connection.
 *  
 *  Result:
 *	A CFNetDiagnosticRef referring to the current networking issue.
 *  
 */
CFN_EXPORT CFNetDiagnosticRef 
CFNetDiagnosticCreateWithURL(CFAllocatorRef alloc, CFURLRef url) CF_DEPRECATED(10_4, 10_13, 2_0, 11_0);



/*
 *  CFNetDiagnosticSetName()
 *  
 *  Discussion:
 *	If the framework requires an application name to be displayed to
 *	the user it will derive it from the bundle identifier of the
 *	currently running application, in that application's current
 *	localization. If you want to override that you may use
 *	CFNetDiagnosticSetName to specify a CFStringRef to be used.
 *  
 *  Parameters:
 *	
 *	details:
 *	  CFNetDiagnosticRef referring to the current problem.
 *	
 *	name:
 *	  The localized name that should appear to the user when
 *	  referring to the application.
 *  
 */
CFN_EXPORT void 
CFNetDiagnosticSetName(CFNetDiagnosticRef details, CFStringRef name) CF_DEPRECATED(10_4, 10_13, 2_0, 11_0);



/*
 *  CFNetDiagnosticDiagnoseProblemInteractively()
 *  
 *  Discussion:
 *	Opens the Network Diagnostics window and returns immediately once
 *	it is open. The client passes in a CFNetDiagnosticRef built with
 *	one of the creator functions.
 *  
 *  Parameters:
 *	
 *	details:
 *	  CFNetDiagnosticRef referring to the current problem.
 *  
 *  Result:
 *	A CFNetDiagnosticStatus. Will either be CFNetDiagnosticNoErr, or
 *	CFNetDiagnosticErr if there was an error attempting to run the
 *	diagnosis.
 *  
 */
CFN_EXPORT CFNetDiagnosticStatus 
CFNetDiagnosticDiagnoseProblemInteractively(CFNetDiagnosticRef details) CF_DEPRECATED(10_4, 10_13, 2_0, 11_0);



/*
 *  CFNetDiagnosticCopyNetworkStatusPassively()
 *  
 *  Discussion:
 *	Returns a status value that can be used to display basic
 *	information about the connection. If the caller wishes they may
 *	pass in a pointer to a CFStringRef that will be used to pass back
 *	a localized description of the problem. It is the caller's
 *	responsibility to release the CFStringRef. If the caller does not
 *	want a description they may pass in NULL.
 *	CFNetDiagnosticCopyNetworkStatusPassively() is guaranteed not to
 *	cause network activity.
 *  
 *  Parameters:
 *	
 *	details:
 *	  CFNetDiagnosticRef referring to the current problem.
 *	
 *	description:
 *	  A pointer to a CFStringRef that, upon return, will point to a
 *	  localized string containing a description of the current
 *	  network status. May be NULL. If it is not NULL, the client must
 *	  call CFRelease on the returned object.
 *  
 */
CFN_EXPORT CFNetDiagnosticStatus 
CFNetDiagnosticCopyNetworkStatusPassively(CFNetDiagnosticRef details, CFStringRef __nullable * __nullable description) CF_DEPRECATED(10_4, 10_13, 2_0, 11_0);



CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFNETDIAGNOSTICS__ */

