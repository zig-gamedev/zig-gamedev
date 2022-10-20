/*
     File:       OSServices/CSIdentityQuery.h
 
     Contains:   Identity Query APIs
 
     Copyright:  (c) 2006-2011 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __CSIDENTITYQUERY__
#define __CSIDENTITYQUERY__

#ifndef __CSIDENTITY__
#include <OSServices/CSIdentity.h>
#endif




#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)


/*
 *  CSIdentityQueryGetTypeID()
 *  
 *  Summary:
 *    Retrieve the CFTypeID of the CSIdentityQuery class
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
CSIdentityQueryGetTypeID(void)                                API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );




/*
 *  CSIdentityQueryFlags
 *  
 *  Summary:
 *    Execution options for an identity query
 *  
 *  Discussion:
 *    A bit mask for setting execution options on a query
 */
enum {

  /*
   * After the intial query phase is complete, monitor the result set
   * for live updates
   */
  kCSIdentityQueryGenerateUpdateEvents = 0x0001,

  /*
   * Include all matching identities in the result set, including
   * hidden "system" users and groups (root, www, etc.)
   */
  kCSIdentityQueryIncludeHiddenIdentities = 0x0002
};

typedef CFOptionFlags                   CSIdentityQueryFlags;

/*
 *  CSIdentityQueryStringComparisonMethod
 *  
 *  Summary:
 *    Options for querying the database by name
 *  
 *  Discussion:
 *    When searching for identities by name, this value specifies the
 *    string comparison function
 */
enum {

  /*
   * The identity name must equal the search string
   */
  kCSIdentityQueryStringEquals  = 1,

  /*
   * The identity name must begin with the search string
   */
  kCSIdentityQueryStringBeginsWith = 2
};

typedef CFIndex                         CSIdentityQueryStringComparisonMethod;
/*
 *  CSIdentityQueryCreate()
 *  
 *  Summary:
 *    Creates an identity query object for all identities in the
 *    specified authority
 *  
 *  Discussion:
 *    The results of this query include all of the identities in the
 *    specified authority's database.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for this instance
 *    
 *    identityClass:
 *      The class of identity to find
 *    
 *    authority:
 *      The identity authority to query
 *  
 *  Result:
 *    A new CSIdentityQuery object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityQueryRef 
CSIdentityQueryCreate(
  CFAllocatorRef           allocator,
  CSIdentityClass          identityClass,
  CSIdentityAuthorityRef   authority)                         API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityQueryCreateForName()
 *  
 *  Summary:
 *    Creates an identity query object based on a name
 *  
 *  Discussion:
 *    The query finds identities by name. It searches the full names,
 *    posix names and aliases for matches.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for this instance
 *    
 *    name:
 *      The name criteria for the query.
 *    
 *    comparisonMethod:
 *      The comparision function (equal or begins with)
 *    
 *    identityClass:
 *      The class of identity to find
 *    
 *    authority:
 *      The identity authority to query
 *  
 *  Result:
 *    A new CSIdentityQuery object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityQueryRef 
CSIdentityQueryCreateForName(
  CFAllocatorRef                          allocator,
  CFStringRef                             name,
  CSIdentityQueryStringComparisonMethod   comparisonMethod,
  CSIdentityClass                         identityClass,
  CSIdentityAuthorityRef                  authority)          API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityQueryCreateForUUID()
 *  
 *  Summary:
 *    Creates an identity query object based on a UUID
 *  
 *  Discussion:
 *    Finds an identity by its UUID
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for this instance
 *    
 *    uuid:
 *      The UUID of the identity to find
 *    
 *    authority:
 *      The identity authority to query
 *  
 *  Result:
 *    A new CSIdentityQuery object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityQueryRef 
CSIdentityQueryCreateForUUID(
  CFAllocatorRef           allocator,
  CFUUIDRef                uuid,
  CSIdentityAuthorityRef   authority)                         API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityQueryCreateForPosixID()
 *  
 *  Summary:
 *    Creates an identity query object based on a POSIX ID
 *  
 *  Discussion:
 *    Finds an identity by its UID or GID
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for this instance
 *    
 *    posixID:
 *      The UID or GID of the identity to find
 *    
 *    identityClass:
 *      The class of identity to find
 *    
 *    authority:
 *      The identity authority to query
 *  
 *  Result:
 *    A new CSIdentityQuery object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityQueryRef 
CSIdentityQueryCreateForPosixID(
  CFAllocatorRef           allocator,
  id_t                     posixID,
  CSIdentityClass          identityClass,
  CSIdentityAuthorityRef   authority)                         API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityQueryCreateForPersistentReference()
 *  
 *  Summary:
 *    Creates an identity query object based on an identity reference
 *    data object
 *  
 *  Discussion:
 *    Finds an identity by reference data obtained from
 *    CSIdentityCreateReferenceData
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for this instance
 *    
 *    referenceData:
 *      The reference data that fully describes an identity
 *  
 *  Result:
 *    A new CSIdentityQuery object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityQueryRef 
CSIdentityQueryCreateForPersistentReference(
  CFAllocatorRef   allocator,
  CFDataRef        referenceData)                             API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityQueryCreateForCurrentUser()
 *  
 *  Summary:
 *    Creates a query for the current session user's identity
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for this instance
 *  
 *  Result:
 *    A new CSIdentityQuery object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityQueryRef 
CSIdentityQueryCreateForCurrentUser(CFAllocatorRef allocator) API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityQueryCopyResults()
 *  
 *  Summary:
 *    Retrieve the results of executing an identity query
 *  
 *  Discussion:
 *    Returns an immutable array of CSIdentityRefs, reflecting the
 *    current results of the query's execution.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    query:
 *      The query object to access
 *  
 *  Result:
 *    An array of zero or more CSIdentityRefs
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
CSIdentityQueryCopyResults(CSIdentityQueryRef query)          API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityQueryExecute()
 *  
 *  Summary:
 *    Execute an identity query synchronously
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    query:
 *      The query object to execute
 *    
 *    flags:
 *      Execution options
 *    
 *    error:
 *      Optional pointer to a CFError object which must be released by
 *      the caller if CSIdentityQueryExecute returns false
 *  
 *  Result:
 *    Returns true if the query executed successfully, false if an
 *    error occurred.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityQueryExecute(
  CSIdentityQueryRef     query,
  CSIdentityQueryFlags   flags,
  CFErrorRef *           error)       /* can be NULL */       API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );




/*
 *  CSIdentityQueryEvent
 *  
 *  Summary:
 *    Results from executing an asynchronous query
 *  
 *  Discussion:
 *    Events generated during asynchronous query execution
 */
enum {

  /*
   * Event generated when the initial lookup of identities has
   * finished. Live update events will follow if caller requests the
   * kCSIdentityQueryGenerateUpdateEvents option.
   */
  kCSIdentityQueryEventSearchPhaseFinished = 1,

  /*
   * Event generated when identities are added to the query results
   */
  kCSIdentityQueryEventResultsAdded = 2,

  /*
   * Event generated when identities already in the query results have
   * been modified
   */
  kCSIdentityQueryEventResultsChanged = 3,

  /*
   * Event generated when identities are removed from the query results
   */
  kCSIdentityQueryEventResultsRemoved = 4,

  /*
   * Used to report an error. Query execution stops (permanently) if
   * this event is sent.
   */
  kCSIdentityQueryEventErrorOccurred = 5
};

typedef CFIndex                         CSIdentityQueryEvent;

/*
 *  CSIdentityQueryReceiveEventCallback
 *  
 *  Summary:
 *    The client event callback function for receiving asynchronous
 *    query events
 *  
 *  Parameters:
 *    
 *    query:
 *      The identity query object that has completed an event
 *    
 *    event:
 *      The event the identity query object has completed
 *    
 *    identities:
 *      a CFArray containing identities resulting from the query
 *    
 *    error:
 *      A CFError object if there was an error from the query
 *    
 *    info:
 *      Any other information you want passed to the callback function
 */
typedef void (*CSIdentityQueryReceiveEventCallback)(CSIdentityQueryRef query, CSIdentityQueryEvent event, CFArrayRef identities, CFErrorRef error, void *info);

/*
 *  CSIdentityQueryClientContext
 *  
 *  Summary:
 *    Client structure specifying callbacks and private context data
 */
struct CSIdentityQueryClientContext {
  CFIndex             version;
  void *              info;
  CFAllocatorRetainCallBack  retainInfo;
  CFAllocatorReleaseCallBack  releaseInfo;
  CFAllocatorCopyDescriptionCallBack  copyInfoDescription;
  CSIdentityQueryReceiveEventCallback  receiveEvent;
};
typedef struct CSIdentityQueryClientContext CSIdentityQueryClientContext;
/*
 *  CSIdentityQueryExecuteAsynchronously()
 *  
 *  Summary:
 *    Execute an identity query asynchronously
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    query:
 *      The query object to execute
 *    
 *    flags:
 *      Execution options
 *    
 *    clientContext:
 *      The client context and callbacks to be used during execution
 *    
 *    runLoop:
 *      The run loop on which to schedule callbacks
 *    
 *    runLoopMode:
 *      The run loop mode in which callbacks may be scheduled
 *  
 *  Result:
 *    Returns true if query execution started, false if the query has
 *    already been executed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityQueryExecuteAsynchronously(
  CSIdentityQueryRef                    query,
  CSIdentityQueryFlags                  flags,
  const CSIdentityQueryClientContext *  clientContext,
  CFRunLoopRef                          runLoop,
  CFStringRef                           runLoopMode)          API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityQueryStop()
 *  
 *  Summary:
 *    Invalidate an identity query client
 *  
 *  Discussion:
 *    Invalidate a query client so that its callback will never be
 *    called in the future. Clients should call CSIdentityQueryStop
 *    when an query will no longer be used, prior to releasing the
 *    final query reference.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    query:
 *      The query to access
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentityQueryStop(CSIdentityQueryRef query)                 API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __CSIDENTITYQUERY__ */

