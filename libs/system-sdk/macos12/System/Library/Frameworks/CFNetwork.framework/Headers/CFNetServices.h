/*
	 File:	   CFNetwork/CFNetServices.h
 
	 Contains:   CoreFoundation Network Net Services header
 
	 Copyright:  Copyright (c) 2001-2013 Apple Inc. All rights reserved.
 
	 Bugs?:	  For bug reports, consult the following page on
				 the World Wide Web:
 
					 http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFNETSERVICES__
#define __CFNETSERVICES__

#ifndef __CFNETWORKDEFS__
#include <CFNetwork/CFNetworkDefs.h>
#endif

#ifndef __CFBASE__
#include <CoreFoundation/CFBase.h>
#endif

#ifndef __CFSTREAM__
#include <CoreFoundation/CFStream.h>
#endif



#if PRAGMA_ONCE
#pragma once
#endif

CF_EXTERN_C_BEGIN
CF_ASSUME_NONNULL_BEGIN

#pragma pack(push, 2)


/*
 *  CFNetServiceRef
 *  
 *  Discussion:
 *	This is the type of a reference to a service.  It may be used for
 *	registering or for resolving.
 */
typedef struct __CFNetService*		  CFNetServiceRef;

/*
 *  CFNetServiceMonitorRef
 *  
 *  Discussion:
 *	This is the type of a reference to a service monitor.  It may be
 *	used for watching record changes on a CFNetServiceRef.
 */
typedef struct __CFNetServiceMonitor*   CFNetServiceMonitorRef;

/*
 *  CFNetServiceBrowserRef
 *  
 *  Discussion:
 *	This is the type of a reference to a service or domain browser.
 *	It may be used for discovering services or domains.
 */
typedef struct __CFNetServiceBrowser*   CFNetServiceBrowserRef;

#ifdef __MACH__
/*
 *  kCFStreamErrorDomainMach
 *  
 *  Discussion:
 *	Errors reported by mach.  See <mach/error.h>
 *  
 */
CFN_EXPORT const SInt32 kCFStreamErrorDomainMach CF_AVAILABLE(10_2, 2_0);
#endif  /* defined(__MACH__) */

/*
 *  kCFStreamErrorDomainNetServices
 *  
 *  Discussion:
 *	Errors listed below or reported by the Service Discovery API's.
 *	See <dns_sd.h>.  The Service Discovery errors will only be
 *	returned when using the new, Mac OS X 10.4-based API's or
 *	CFNetServiceBrowser.
 *  
 */
CFN_EXPORT const SInt32 kCFStreamErrorDomainNetServices CF_AVAILABLE(10_2, 2_0);



/*
 *  CFNetServicesError
 *  
 *  Discussion:
 *	Errors from the kCFStreamErrorDomainNetServices domain.
 */
typedef CF_ENUM(int, CFNetServicesError) {

  /*
   * An error of unknown type has occured.
   */
  kCFNetServicesErrorUnknown	= -72000L,

  /*
   * The given registration has had a name collision.  Registration
   * should be cancelled and should try again probably with a different
   * name.
   */
  kCFNetServicesErrorCollision  = -72001L,

  /*
   * Not used
   */
  kCFNetServicesErrorNotFound   = -72002L,

  /*
   * There is already a register, resolve, browse, or monitor in
   * progress on the given object.
   */
  kCFNetServicesErrorInProgress = -72003L,

  /*
   * Not used
   */
  kCFNetServicesErrorBadArgument = -72004L,

  /*
   * The register, resolve, or browse on the object has been cancelled.
   */
  kCFNetServicesErrorCancel	 = -72005L,

  /*
   * The given CFNetServiceBrowser or CFNetServiceMonitor has already
   * been invalidated and can no longer be used.
   */
  kCFNetServicesErrorInvalid	= -72006L,

  /*
   * The given CFNetServiceResolveWithTimeout has hit the timeout
   * before a successful resolve.
   */
  kCFNetServicesErrorTimeout	= -72007L,

  /*
   * Missing required configuration for local network access.
   */
  kCFNetServicesErrorMissingRequiredConfiguration API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0)) API_UNAVAILABLE(watchos) = -72008L,
};



/*
 *  CFNetServiceMonitorType
 *  
 *  Discussion:
 *	Record type specifier in order to inform CFNetServiceMonitor to
 *	watch for certain record changes.
 */
typedef CF_ENUM(int, CFNetServiceMonitorType) {

  /*
   * Watch for TXT record changes.
   */
  kCFNetServiceMonitorTXT = 1
};


/*
 *  CFNetService flags
 *
 *  Discussion:
 *  Bit flags to be used for registration of a service with CFNetServiceRegisterWithOptions.
 */
typedef CF_OPTIONS(CFOptionFlags, CFNetServiceRegisterFlags) {
	
  /*
   * Indicate that registration should not auto-rename the service to prevent name conflicts.
   */
  kCFNetServiceFlagNoAutoRename = 1
};


/*
 *  CFNetServiceBrowser flags
 *  
 *  Discussion:
 *	Result bit flags passed to CFNetServiceBrowserClientCallBack.
 */
typedef CF_OPTIONS(CFOptionFlags, CFNetServiceBrowserFlags) {

  /*
   * Client will get another callback briefly and shouldn't do costly screen updates (or such).
   */
  kCFNetServiceFlagMoreComing = 1,

  /*
   * If off, the result is a service.
   */
  kCFNetServiceFlagIsDomain = 2,

  /*
   * The result domain is the default domain for the given domain browse type (registration or browse).
   */
  kCFNetServiceFlagIsDefault = 4,

  /*
   * Same as the previous but incorrectly named. Kept for compatibility.
   */
  kCFNetServiceFlagIsRegistrationDomain CF_ENUM_DEPRECATED(10_2, 10_4, 2_0, 2_0) = 4,

  /*
   * The result item should be removed and not added.
   */
  kCFNetServiceFlagRemove = 8
};



/*
 *  CFNetServiceClientContext
 *  
 *  Discussion:
 *	Structure containing the user-defined data and callbacks for
 *	CFNetService and CFNetServiceBrowser objects.
 */
struct CFNetServiceClientContext {

  /*
   * The version number of the structure type being passed in as a
   * parameter to the CFNetService, Browser, or Monitor client
   * function.  The current version number is 0.
   */
  CFIndex			 version;

  /*
   * An arbitrary pointer to client-defined data, which can be
   * associated with the service/browser and is passed to the callbacks.
   */
  void *	__nullable	 info;

  /*
   * The callback used to add a retain for the service/browser on the
   * info pointer for the life of the service/browser, and may be used
   * for temporary references the service/browser needs to take. This
   * callback returns the actual info pointer to store in the
   * service/browser, almost always just the pointer passed as the
   * parameter.
   */
  CFAllocatorRetainCallBack  __nullable retain;

  /*
   * The callback used to remove a retain previously added for the
   * service/browser on the info pointer.
   */
  CFAllocatorReleaseCallBack  __nullable release;

  /*
   * The callback used to create a descriptive string representation of
   * the info pointer (or the data pointed to by the info pointer) for
   * debugging purposes. This is used by the CFCopyDescription()
   * function.
   */
  CFAllocatorCopyDescriptionCallBack  __nullable copyDescription;
};
typedef struct CFNetServiceClientContext CFNetServiceClientContext;

/*
 *  CFNetServiceClientCallBack
 *  
 *  Discussion:
 *	Callback function which is called upon error or completion of
 *	resolve or register.  If resolving with the deprecated API's, the
 *	callback may be called multiple times, once for each resolved
 *	address.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  Service receiving the event.
 *	
 *	error:
 *	  Reference to an error structure if the event is a failure.
 *	
 *	info:
 *	  Client's info reference which was passed into the client
 *	  context.
 */
typedef CALLBACK_API_C( void , CFNetServiceClientCallBack )(CFNetServiceRef theService, CFStreamError * __nullable error, void  * __nullable info);

/*
 *  CFNetServiceMonitorClientCallBack
 *  
 *  Discussion:
 *	Callback function which is called as the monitored record changes.
 *  
 *  Parameters:
 *	
 *	theMonitor:
 *	  CFNetServiceMonitor receiving the event.
 *	
 *	theService:
 *	  Service receiving the event.
 *	
 *	typeInfo:
 *	  The information type which changed.
 *	
 *	rdata:
 *	  The contents of the record that changed.
 *	
 *	error:
 *	  Reference to an error structure if the event is a failure.
 *	
 *	info:
 *	  Client's info reference which was passed into the client
 *	  context.
 */
typedef CALLBACK_API_C( void , CFNetServiceMonitorClientCallBack )(CFNetServiceMonitorRef theMonitor, CFNetServiceRef __nullable theService, CFNetServiceMonitorType typeInfo, CFDataRef __nullable rdata, CFStreamError * __nullable error, void * __nullable info);

/*
 *  CFNetServiceBrowserClientCallBack
 *  
 *  Discussion:
 *	Callback function which is called upon error or upon successful
 *	discovery of services or domains.
 *  
 *  Parameters:
 *	
 *	browser:
 *	  CFNetServiceBrowser receiving the event.
 *	
 *	flags:
 *	  Bitwise flags indicating the event or further information about
 *	  the event.
 *	
 *	domainOrService:
 *	  If searching for domains, a CFStringRef indicating the domain
 *	  which was found or is going away.  If searching for services,
 *	  the service which was found or is going away.
 *	
 *	error:
 *	  Reference to an error structure if the event is a failure.
 *	
 *	info:
 *	  Client's info reference which was passed into the client
 *	  context.
 */
typedef CALLBACK_API_C( void , CFNetServiceBrowserClientCallBack )(CFNetServiceBrowserRef browser, CFOptionFlags flags, CFTypeRef __nullable domainOrService, CFStreamError * __nullable error, void * __nullable info);

/*
 *  CFNetServiceGetTypeID()
 *  
 *  Discussion:
 *	Returns the type identifier of all CFNetService instances.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 */
CFN_EXPORT CFTypeID 
CFNetServiceGetTypeID(void) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


/*
 *  CFNetServiceMonitorGetTypeID()
 *  
 *  Discussion:
 *	Returns the type identifier of all CFNetServiceMonitor instances.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 */
CFN_EXPORT CFTypeID 
CFNetServiceMonitorGetTypeID(void) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


/*
 *  CFNetServiceBrowserGetTypeID()
 *  
 *  Discussion:
 *	Returns the type identifier of all CFNetServiceBrowser instances.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 */
CFN_EXPORT CFTypeID 
CFNetServiceBrowserGetTypeID(void) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceCreate()
 *  
 *  Discussion:
 *	Creates an instance of a Network Service.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CFAllocator which should be used to allocate memory for the
 *	  service and its storage for values. If this reference is not a
 *	  valid CFAllocator, the behavior is undefined.
 *	
 *	domain:
 *	  The network domain in which it is registered or will be
 *	  registered. This value must be non-NULL.
 *	
 *	serviceType:
 *	  The type of service being registered or resolved on the
 *	  network. The service type consists of the application protocol
 *	  name followed by the transport protocol name, separated by a
 *	  dot (e.g. "_ftp._tcp").  The application protocol name should
 *	  be 14 characters or less, and should only contain lower-case
 *	  letters, digits, and hyphens.  New service types should be
 *	  registered at <htp://www.dns-sd.org/ServiceTypes.html>.  This
 *	  value must be non-NULL.
 *	
 *	name:
 *	  The name of the machine or application advertising the service.
 *	   If this value is not unique, registering will eventually fail.
 *	   This value must be non-NULL.  This value is usually displayed
 *	  in a browser for the user.
 *	
 *	port:
 *	  The port on which the service is listening.  This must be
 *	  non-zero for services which are to be registered.
 *  
 *  Result:
 *	A valid CFNetService which may now be registered or resolved, or
 *	NULL if unsuccessful.
 *  
 */
CFN_EXPORT CFNetServiceRef 
CFNetServiceCreate(CFAllocatorRef __nullable alloc, CFStringRef domain, CFStringRef serviceType, CFStringRef name, SInt32 port) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceCreateCopy()
 *  
 *  Discussion:
 *	Creates a new CFNetService object as a copy of service argument.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CFAllocator which should be used to allocate memory for the
 *	  new service. If this reference is not a valid CFAllocator, the
 *	  behavior is undefined.
 *	
 *	service:
 *	  A CFNetServiceRef representing the original service. Must be
 *	  non-NULL.  If this If this reference is not a valid
 *	  CFNetServiceRef, the behavior is undefined.
 *  
 *  Result:
 *	Returns a valid CFNetServiceRef which contains a copy of all
 *	previously resolved data from the original.  NULL is returned in
 *	the case of failure.
 *  
 */
CFN_EXPORT CFNetServiceRef 
CFNetServiceCreateCopy(CFAllocatorRef __nullable alloc, CFNetServiceRef service) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.3, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceGetDomain()
 *  
 *  Discussion:
 *	Query a Network Service for its domain.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The function gets the data in a thread-safe manner, but the
 *	resulting data is not safe.  Since it is returned as a matter of
 *	a get opposed to a copy, the data is not safe if the service is
 *	being altered from another thread.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to be queried.  Must be non-NULL.
 *  
 *  Result:
 *	CFStringRef which is the service's domain.
 *  
 */
CFN_EXPORT CFStringRef 
CFNetServiceGetDomain(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceGetType()
 *  
 *  Discussion:
 *	Query a Network Service for its type.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The function gets the data in a thread-safe manner, but the
 *	resulting data is not safe.  Since it is returned as a matter of
 *	a get opposed to a copy, the data is not safe if the service is
 *	being altered from another thread.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to be queried.  Must be non-NULL.
 *  
 *  Result:
 *	CFStringRef which is the service's service type.
 *  
 */
CFN_EXPORT CFStringRef 
CFNetServiceGetType(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceGetName()
 *  
 *  Discussion:
 *	Query a Network Service for its name.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The function gets the data in a thread-safe manner, but the
 *	resulting data is not safe.  Since it is returned as a matter of
 *	a get opposed to a copy, the data is not safe if the service is
 *	being altered from another thread.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to be queried.  Must be non-NULL.
 *  
 *  Result:
 *	CFStringRef which is the service's name.
 *  
 */
CFN_EXPORT CFStringRef 
CFNetServiceGetName(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceRegisterWithOptions()
 *  
 *  Discussion:
 *	Registers the entity on the network.  This requires that the
 *	service has a domain, a type, a name, and a port.  The service is
 *	registered on the network until this function returns or is
 *	cancelled by calling CFNetServiceCancel.  In synchronous mode,
 *	this function will block until there is an error or it is
 *	cancelled from another thread.  In asynchronous mode, this
 *	function returns immediately and the underlying network
 *	registration process will start.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to register on the network.  Must be
 *	  non-NULL.
 *	
 *	options:
 *	  A set of bit options used to instruct the registration process.
 *	  Current supported option is kCFNetServiceFlagNoAutoRename.
 *	
 *	error:
 *	  A reference to an error struct which will be set to the error
 *	  and domain of the error should one occur.  If the value of
 *	  error is not desired, set to NULL.
 *  
 *  Result:
 *	Returns FALSE if domain, type, name or port is NULL.  In
 *	synchronous mode, it will always return FALSE as a result of the
 *	error or the cancellation.  In asynchronous mode, it will return
 *	TRUE if the registration process could start.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceRegisterWithOptions(CFNetServiceRef theService, CFOptionFlags options, CFStreamError * __nullable error) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceResolveWithTimeout()
 *  
 *  Discussion:
 *	Resolves the information related to this service.  It will
 *	resolve the target host, the addresses, and the first TXT record
 *	associated with the service.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The CFNetServiceRef which should be resolved. Must be non-NULL.
 *	  If this reference is not a valid CFNetServiceRef, the behavior
 *	  is undefined.
 *	
 *	timeout:
 *	  CFTimeInterval representing the maximum amount of time to take
 *	  to perform the resolve.  If the resolve can not be performed
 *	  within this timeout, the function or callback will recieve a
 *	  timeout error.  Values less than or equal to zero indicate an
 *	  infinite timeout.
 *	
 *	error:
 *	  A reference to a CFStreamError structure which will be filled
 *	  with any error information should an error occur.  May be set
 *	  to NULL if error information is not wanted.
 *  
 *  Result:
 *	Returns TRUE on success and FALSE on failure.  In asynchronous
 *	mode, this function will return immediately.  In synchronous
 *	mode, it will block until the resolve has completed or until the
 *	resolve is cancelled.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceResolveWithTimeout(CFNetServiceRef theService, CFTimeInterval timeout, CFStreamError * __nullable error) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceCancel()
 *  
 *  Discussion:
 *	Cancels an outstanding request for registration or resolution.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service which is active.  Must be non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceCancel(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceGetTargetHost()
 *  
 *  Discussion:
 *	Query a Network Service for its resolve target.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The function gets the data in a thread-safe manner, but the
 *	resulting data is not safe.  Since it is returned as a matter of
 *	a get opposed to a copy, the data is not safe if the service is
 *	being altered from another thread.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to be queried.  Must be non-NULL.
 *  
 *  Result:
 *	Returns The target hostname of the machine providing the service,
 *	or NULL if the entity's target is not known (has not been
 *	resolved).
 *  
 */
CFN_EXPORT __nullable CFStringRef
CFNetServiceGetTargetHost(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceGetPortNumber()
 *  
 *  Discussion:
 *	Query a Network Service for its port number.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The function gets the data in a thread-safe manner.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to be queried.  Must be non-NULL.
 *  
 *  Result:
 *	Returns a SInt32 containing the port number in host byte order.
 *	Returns -1 if the entity's port is not known (has not been
 *	resolved)
 *  
 */
CFN_EXPORT SInt32 
CFNetServiceGetPortNumber(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.5, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceGetAddressing()
 *  
 *  Discussion:
 *	Query a Network Service for its addressing.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The function gets the data in a thread-safe manner, but the
 *	resulting data is not safe.  Since it is returned as a matter of
 *	a get opposed to a copy, the data is not safe if the service is
 *	being altered from another thread.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to be queried.  Must be non-NULL.
 *  
 *  Result:
 *	Returns NULL if the entity's addressing is not known (has not
 *	been resolved).  The array will contain a CFDataRef for each
 *	address resolved.  Each CFDataRef contains a struct sockaddr
 *	representing the address of the service.
 *  
 */
CFN_EXPORT __nullable CFArrayRef
CFNetServiceGetAddressing(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceGetTXTData()
 *  
 *  Discussion:
 *	Query a Network Service for its TXT record contents.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *	The function gets the data in a thread-safe manner, but the
 *	resulting data is not safe.  Since it is returned as a matter of
 *	a get opposed to a copy, the data is not safe if the service is
 *	being altered from another thread.
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to be queried.  Must be non-NULL.
 *  
 *  Result:
 *	Returns NULL if the entity's TXT is not known (has not been
 *	resolved).  The result will contain the contents of the TXT
 *	record.  This is suitable to pass to
 *	CFNetServiceCreateDictionaryWithTXTData.
 *  
 */
CFN_EXPORT __nullable CFDataRef
CFNetServiceGetTXTData(CFNetServiceRef theService) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceSetTXTData()
 *  
 *  Discussion:
 *	Sets the TXT record for the service.  If the service is currently
 *	registered on the network, the record will be broadcast.  Setting
 *	the TXT record on a resolving service is not allowed.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to receive the new record.  Must be
 *	  non-NULL.
 *	
 *	txtRecord:
 *	  The contents of the TXT record.  This should not exceed a
 *	  length of 1450 bytes.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceSetTXTData(CFNetServiceRef theService, CFDataRef txtRecord) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceCreateDictionaryWithTXTData()
 *  
 *  Discussion:
 *	Parses the given TXT record data into a set of key/value pairs as
 *	a CFDictionary where keys are CFStringRefs and values are
 *	CFDataRefs.  If the given record can not be parsed, NULL will be
 *	returned.  READ THE COMMENTS FOR
 *	CFNetServiceCreateTXTDataWithDictionary TO FULLY UNDERSTAND THE
 *	USE AND RESULTS OF THIS FUNCTION.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  CFAllocatorRef to be used for the creation of the result.
 *	
 *	txtRecord:
 *	  The TXT record data as returned by CFNetServiceGetInfo.
 *  
 *  Result:
 *	CFDictionaryRef containing the key/value pairs parsed from the
 *	record. It will return NULL if the record could not be parsed. 
 *	Keys in the dictionary are CFStringRef's.  Values are CFDataRef's.
 *  
 */
CFN_EXPORT __nullable CFDictionaryRef
CFNetServiceCreateDictionaryWithTXTData(CFAllocatorRef __nullable alloc, CFDataRef txtRecord) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceCreateTXTDataWithDictionary()
 *  
 *  Discussion:
 *	Flattens a set of key/value pairs into a CFDataRef suitable to
 *	pass into CFNetServiceSetTXTData.  This function will properly
 *	format the data for TXT record usage.  THIS IS NOT A GENERAL
 *	CFDictionaryRef FLATTENING ROUTINE.  CFDictionaryRef keys should
 *	be CFStringRef's and values should be CFDataRef's.  For
 *	convenience, values that are CFStringRef's will be converted to
 *	CFDataRef's representing the flattened UTF-8 bytes of the string.
 *	 The types of the values are not encoded in the CFDataRef's,
 *	therefore CFStringRef's will be flattened into CFDataRef's, and
 *	they will come out of CFNetServiceCreateDictionaryWithTXTData as
 *	CFDataRef's.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  CFAllocatorRef to be used for the creation of the result.
 *	
 *	keyValuePairs:
 *	  CFDictionaryRef containing keys and values to be placed into
 *	  the TXT record.  The keys must be CFStringRef's.  The values
 *	  should be CFDataRef's (CFStringRef's are permitted for
 *	  convenience).  Any other types will cause a failure.  The
 *	  length of a key and its value should not exceed 255.
 *  
 *  Result:
 *	CFDataRef containing the flattened form of the keys and values. 
 *	If the dictionary could not be flattend, NULL will be returned.
 *  
 */
CFN_EXPORT __nullable CFDataRef
CFNetServiceCreateTXTDataWithDictionary(CFAllocatorRef __nullable alloc, CFDictionaryRef keyValuePairs) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceSetClient()
 *  
 *  Discussion:
 *	Sets up the service to be used in asynchronous mode. 
 *	Notification of registration failure or resolution completion
 *	will occur through the given callback.  Once the client is set,
 *	the service must be scheduled on a runloop. The client callback
 *	will be triggered via one of the scheduled run loops; It is the
 *	caller's responsibility to ensure that at least one of the
 *	scheduled run loops is being run.  This call must be performed
 *	before calling CFNetServiceRegister or CFNetServiceResolve,
 *	otherwise it will return FALSE.  TRUE will be returned if the
 *	client could be set.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The service to set up for asynchronous mode.  Must be non-NULL.
 *	
 *	clientCB:
 *	  Function pointer will be called upon registration failure or
 *	  upon resolution completion.  In the case of resolution, this
 *	  callback may be called multiple times if there is more than one
 *	  address for a service.  Passing NULL will remove the client
 *	  from the entity and cancel any outstanding activity.
 *	
 *	clientContext:
 *	  Client contextual information to be used when calling clientCB.
 *	  Passing NULL will remove the client from the entity and cancel
 *	  any outstanding activity.
 *  
 *  Result:
 *	Returns FALSE if the client could not be set, TRUE otherwise.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceSetClient(CFNetServiceRef theService, CFNetServiceClientCallBack __nullable clientCB, CFNetServiceClientContext * __nullable clientContext) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceScheduleWithRunLoop()
 *  
 *  Discussion:
 *	Schedule the given service on the given run loop and mode.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The service which is set up for asynchronous mode. Must be
 *	  non-NULL.
 *	
 *	runLoop:
 *	  A reference to a runloop on which the service should be
 *	  scheduled. Must be non-NULL.
 *	
 *	runLoopMode:
 *	  The mode on which to schedule the service.  Must be non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceScheduleWithRunLoop(CFNetServiceRef theService, CFRunLoopRef runLoop, CFStringRef runLoopMode) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceUnscheduleFromRunLoop()
 *  
 *  Discussion:
 *	Unschedule the given service from the given run loop and mode.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The service which is set up for asynchronous mode.  Must be
 *	  non-NULL.
 *	
 *	runLoop:
 *	  A reference to a runloop from which the service should be
 *	  unscheduled.  Must be non-NULL.
 *	
 *	runLoopMode:
 *	  The mode from which to unschedule the service.  Must be
 *	  non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceUnscheduleFromRunLoop(CFNetServiceRef theService, CFRunLoopRef runLoop, CFStringRef runLoopMode) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceMonitorCreate()
 *  
 *  Discussion:
 *	Creates an instance of an object suitable for watching for
 *	CFNetService record changes on the network.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CFAllocator which should be used to allocate memory for the
 *	  monitor and its storage for values. If this reference is not a
 *	  valid CFAllocator, the behavior is undefined.
 *	
 *	theService:
 *	  The CFNetService to be monitored for record changes.
 *	
 *	clientCB:
 *	  Function pointer that will be called as record changes occur. 
 *	  Must be non-NULL.
 *	
 *	clientContext:
 *	  Client contextual information to be used when calling clientCB.
 *	   Must be non-NULL.
 *  
 *  Result:
 *	Returns a new instance of a CFNetServiceMonitor, or NULL if the
 *	object could not be created.
 *  
 */
CFN_EXPORT CFNetServiceMonitorRef 
CFNetServiceMonitorCreate(
  CFAllocatorRef __nullable          alloc,
  CFNetServiceRef                    theService,
  CFNetServiceMonitorClientCallBack  clientCB,
  CFNetServiceClientContext *        clientContext) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceMonitorInvalidate()
 *  
 *  Discussion:
 *	Invalidates the given monitor object so that it may no longer be
 *	scheduled and callback never be called.  This will also stop any
 *	monitors currently in progress.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	monitor:
 *	  CFNetServiceMonitor to invalidate.  Must be non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceMonitorInvalidate(CFNetServiceMonitorRef monitor) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceMonitorStart()
 *  
 *  Discussion:
 *	Starts monitoring for record changes on a service.  It watches
 *	for changes of the given record type.  If there is already an
 *	outstanding monitor, it will return FALSE.  In synchronous mode,
 *	this call blocks until the monitor is stopped. It will return
 *	FALSE if there is an error performing the monitor or if there is
 *	some other error.  It will return TRUE otherwise.  In
 *	asynchronous mode, this call will return TRUE or FALSE depending
 *	if the underlying network query could be instantiated.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	monitor:
 *	  CFNetServiceMonitor to perform the watch.
 *	
 *	recordType:
 *	  CFNetServiceMonitorType indicating the record type to watch.
 *	
 *	error:
 *	  A reference to an error struct which will be set to the error
 *	  and domain of the error should one occur.  If the value of
 *	  error is not desired, set to NULL.
 *  
 *  Result:
 *	Returns FALSE if an error occurs during a synchronous monitor or
 *	if the monitor could not start.  It returns TRUE otherwise.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceMonitorStart(CFNetServiceMonitorRef monitor, CFNetServiceMonitorType recordType, CFStreamError * __nullable error) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceMonitorStop()
 *  
 *  Discussion:
 *	Stops an outstanding monitor.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	monitor:
 *	  CFNetServiceMonitor with an active monitor.  Must be non-NULL.
 *	
 *	error:
 *	  Error value to be returned in "error" in
 *	  CFNetServiceMonitorStart if monitor is being performed in
 *	  synchronous mode.  In this case, a non-zero of the error field
 *	  of the struct will cause CFNetServiceMonitorStart to return
 *	  FALSE.  In asynchronous mode, the client call back will be
 *	  called with this error. Can be NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceMonitorStop(CFNetServiceMonitorRef monitor, CFStreamError * __nullable error) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceMonitorScheduleWithRunLoop()
 *  
 *  Discussion:
 *	Schedules the monitor on a run loop and mode.  Use this to place
 *	the given monitor into asynchronous mode.  The client callback
 *	will be triggered via one of the scheduled run loops; It is the
 *	caller's responsibility to ensure that at least one of the
 *	scheduled run loops is being run.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	monitor:
 *	  CFNetServiceMonitor to schedule.  Must be non-NULL.
 *	
 *	runLoop:
 *	  A reference to a runloop on which the monitor should be
 *	  scheduled.  Must be non-NULL.
 *	
 *	runLoopMode:
 *	  The mode on which to schedule the monitor.  Must be non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceMonitorScheduleWithRunLoop(CFNetServiceMonitorRef monitor, CFRunLoopRef runLoop, CFStringRef runLoopMode) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceMonitorUnscheduleFromRunLoop()
 *  
 *  Discussion:
 *	Unschedules the browser from a run loop and mode.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	monitor:
 *	  CFNetServiceMonitor to unschedule.  Must be non-NULL.
 *	
 *	runLoop:
 *	  A reference to a runloop from which the monitor should be
 *	  unscheduled. Must be non-NULL.
 *	
 *	runLoopMode:
 *	  The mode from which to unschedule the monitor.  Must be
 *	  non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceMonitorUnscheduleFromRunLoop(CFNetServiceMonitorRef monitor, CFRunLoopRef runLoop, CFStringRef runLoopMode) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.4, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceBrowserCreate()
 *  
 *  Discussion:
 *	Creates an instance of a browser object.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	alloc:
 *	  The CFAllocator which should be used to allocate memory for the
 *	  browser and its storage for values. If this reference is not a
 *	  valid CFAllocator, the behavior is undefined.
 *	
 *	clientCB:
 *	  Function pointer that will be called as domains or services are
 *	  found on the network.  Must be non-NULL.
 *	
 *	clientContext:
 *	  Client contextual information to be used when calling clientCB.
 *	  Must be non-NULL.
 *  
 *  Result:
 *	Returns a new instance of a browser, or NULL if the instance
 *	could not be created.
 *  
 */
CFN_EXPORT CFNetServiceBrowserRef 
CFNetServiceBrowserCreate(CFAllocatorRef __nullable alloc, CFNetServiceBrowserClientCallBack clientCB, CFNetServiceClientContext *clientContext) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceBrowserInvalidate()
 *  
 *  Discussion:
 *	Invalidates the given browser object so that it may no longer be
 *	scheduled and callback never be called.  This will also stop any
 *	searches currently in progress.
 *  
 *  Parameters:
 *	
 *	browser:
 *	  Network Service Browser to invalidate.  Must be non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceBrowserInvalidate(CFNetServiceBrowserRef browser) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceBrowserSearchForDomains()
 *  
 *  Discussion:
 *	Starts a search for domains.  The browser will either try to find
 *	"Browse" domains or will search for "Registration" domains.  If
 *	there is already an outstanding search, it will return FALSE.  In
 *	syncronous mode, this call blocks until the search is stopped. 
 *	It will return FALSE if there is an error performing the search.
 *	It will return TRUE otherwise.  In asynchronous mode, this call
 *	will return TRUE or FALSE depending if the underlying network
 *	search could be started.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	browser:
 *	  Network Service Browser to perform the search.  Must be
 *	  non-NULL.
 *	
 *	registrationDomains:
 *	  FALSE if "Browse" domains are to be discovered. TRUE if only
 *	  "Registration" domains are to be discovered.
 *	
 *	error:
 *	  A reference to an error struct which will be set to the error
 *	  and domain of the error should one occur.  If the value of
 *	  error is not desired, set to NULL.
 *  
 *  Result:
 *	Returns FALSE if an error occurs during a synchronous search or
 *	if the search could not start.  It returns TRUE otherwise.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceBrowserSearchForDomains(CFNetServiceBrowserRef browser, Boolean registrationDomains, CFStreamError * __nullable error) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceBrowserSearchForServices()
 *  
 *  Discussion:
 *	Starts a search for a service type on the given domain.  If there
 *	is already an outstanding search, it will return FALSE.  In
 *	syncronous mode, this call blocks until the search is stopped. 
 *	It will return FALSE if there is an error performing the search
 *	or if there is some other error.  It will return TRUE otherwise.
 *	In asynchronous mode, this call will return TRUE or FALSE
 *	depending if the underlying network search could be instantiated.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	browser:
 *	  Network Service Browser to perform the search.  Must be
 *	  non-NULL.
 *	
 *	domain:
 *	  Network domain to search in order to find the service.  Must be
 *	  non-NULL.
 *	
 *	serviceType:
 *	  Service type for which to search.  Must be non-NULL.
 *	
 *	error:
 *	  A reference to an error struct which will be set to the error
 *	  and domain of the error should one occur.  If the value of
 *	  error is not desired, set to NULL.
 *  
 *  Result:
 *	Returns FALSE if an error occurs during a synchronous search or
 *	if the search could not start.  It returns TRUE otherwise.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceBrowserSearchForServices(CFNetServiceBrowserRef browser, CFStringRef domain, CFStringRef serviceType, CFStreamError * __nullable error) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceBrowserStopSearch()
 *  
 *  Discussion:
 *	Stops an outstanding browser search.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	browser:
 *	  Network Service Browser performing the search.  Must be
 *	  non-NULL.
 *	
 *	error:
 *	  Error value to be returned in "error" in
 *	  CFNetServiceBrowserStartServiceSearch if search is being
 *	  performed in synchronous mode.  In this case, a non-zero of the
 *	  error field of the struct will cause
 *	  CFNetServiceBrowserStartServiceSearch to return FALSE. In
 *	  asynchronous mode, the client call back will be called with
 *	  this error. Can be NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceBrowserStopSearch(CFNetServiceBrowserRef browser, CFStreamError * __nullable error) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceBrowserScheduleWithRunLoop()
 *  
 *  Discussion:
 *	Schedules the browser on a run loop and mode.  Use this to place
 *	the given browser into asynchronous mode.  The client callback
 *	will be triggered via one of the scheduled run loops; It is the
 *	caller's responsibility to ensure that at least one of the
 *	scheduled run loops is being run.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	browser:
 *	  Network Service Browser to schedule.  Must be non-NULL.
 *	
 *	runLoop:
 *	  A reference to a runloop on which the browser should be
 *	  scheduled.  Must be non-NULL.
 *	
 *	runLoopMode:
 *	  The mode on which to schedule the browser.  Must be non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceBrowserScheduleWithRunLoop(CFNetServiceBrowserRef browser, CFRunLoopRef runLoop, CFStringRef runLoopMode) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));



/*
 *  CFNetServiceBrowserUnscheduleFromRunLoop()
 *  
 *  Discussion:
 *	Unschedules the browser from a run loop and mode.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	browser:
 *	  Network Service Browser to unschedule.  Must be non-NULL.
 *	
 *	runLoop:
 *	  A reference to a runloop from which the browser should be
 *	  unscheduled. Must be non-NULL.
 *	
 *	runLoopMode:
 *	  The mode from which to unschedule the browser.  Must be
 *	  non-NULL.
 *  
 */
CFN_EXPORT void 
CFNetServiceBrowserUnscheduleFromRunLoop(CFNetServiceBrowserRef browser, CFRunLoopRef runLoop, CFStringRef runLoopMode) API_DEPRECATED("Use nw_browser_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


#ifdef __MACH__
/*
 *  CFNetServiceRegister()   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Registers the entity on the network.  This requires that the
 *	service has a domain, a type, a name, and a port.  The service is
 *	registered on the network until this function returns or is
 *	cancelled by calling CFNetServiceCancel.  In synchronous mode,
 *	this function will block until there is an error or it is
 *	cancelled from another thread.  In asynchronous mode, this
 *	function returns immediately and the underlying network
 *	registration process will start. 
 *	
 *	As a result of new, better performing API's in Service Discovery,
 *	users should now call CFNetServiceRegisterWithOptions.  Using the
 *	new calls will allow your application to perform better on the
 *	network.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to register on the network.  Must be
 *	  non-NULL.
 *	
 *	error:
 *	  A reference to an error struct which will be set to the error
 *	  and domain of the error should one occur.  If the value of
 *	  error is not desired, set to NULL.
 *  
 *  Result:
 *	Returns FALSE if domain, type, name or port is NULL.  In
 *	synchronous mode, it will always return FALSE as a result of the
 *	error or the cancellation.  In asynchronous mode, it will return
 *	TRUE if the registration process could start.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceRegister(CFNetServiceRef theService, CFStreamError * __nullable error) CF_DEPRECATED(10_2, 10_4, NA, NA);



/*
 *  CFNetServiceResolve()   *** DEPRECATED ***
 *  
 *  Discussion:
 *	Resolves the addressing for the given service.  This requires
 *	that the service has a domain, a type, and a name.  The service
 *	is  resolved on the network until this function returns or is
 *	cancelled by calling CFNetServiceCancel. In synchronous mode,
 *	this function will block until there is an error or it is
 *	cancelled from another thread.  In asynchronous mode, this
 *	function returns immediately and the underlying network
 *	resolution process will start. 
 *	
 *	As a result of new, better performing API's in Service Discovery,
 *	users should now call CFNetServiceResolveWithTimeout.  If needing
 *	to monitor TXT record changes, users should use the new
 *	CFNetServiceMonitor object. Using the new calls will allow your
 *	application to perform better on the network.
 *  
 *  Mac OS X threading:
 *	Thread safe
 *  
 *  Parameters:
 *	
 *	theService:
 *	  The Network Service to resolve on the network.  Must be
 *	  non-NULL.
 *	
 *	error:
 *	  A reference to an error struct which will be set to the error
 *	  and domain of the error should one occur.  If the value of
 *	  error is not desired, set to NULL.
 *  
 *  Result:
 *	Returns FALSE if domain, type, or name is NULL.  In synchronous
 *	mode, it will return FALSE as a result of an error or a
 *	cancellation.  It will return TRUE if the resolution does
 *	succeed.  In asynchronous mode, it will return TRUE if the
 *	resolution process could start.
 *  
 */
CFN_EXPORT Boolean 
CFNetServiceResolve(CFNetServiceRef theService, CFStreamError * __nullable error) CF_DEPRECATED(10_2, 10_4, NA, NA);


#endif  /* defined(__MACH__) */


#pragma pack(pop)

CF_ASSUME_NONNULL_END
CF_EXTERN_C_END

#endif /* __CFNETSERVICES__ */

