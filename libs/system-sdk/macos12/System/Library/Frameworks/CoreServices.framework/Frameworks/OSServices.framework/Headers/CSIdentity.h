/*
     File:       OSServices/CSIdentity.h
 
     Contains:   CSIdentity APIs
 
     Copyright:  (c) 2006-2011 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __CSIDENTITY__
#define __CSIDENTITY__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __CSIDENTITYBASE__
#if TARGET_OS_IPHONE && (TARGET_OS_EMBEDDED || TARGET_OS_SIMULATOR || !TARGET_OS_MACCATALYST)
#include <CoreServices/CSIdentityBase.h>
#else
#include <OSServices/CSIdentityBase.h>
#endif
#endif

#ifndef __CSIDENTITYAUTHORITY__
#if TARGET_OS_IPHONE && (TARGET_OS_EMBEDDED || TARGET_OS_SIMULATOR || !TARGET_OS_MACCATALYST)
#include <CoreServices/CSIdentityAuthority.h>
#else
#include <OSServices/CSIdentityAuthority.h>
#endif
#endif

#ifndef __SECBASE__
#include <Security/SecBase.h>
#endif

#ifndef __AUTHORIZATION__
#include <Security/Security.h>
#if TARGET_OS_OSX
#include <Security/Authorization.h>
#endif
#endif

#include <Availability.h>
#include <ConditionalMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

#include <sys/types.h>

/*
    CSIdentity

    A CSIdentity object represents a user or group entity known to the system. An
    identity object has the following required attributes: a class (user
    or group), a unique identitfier (UUID), a full name, a Posix ID
    (UID or GID), and a Posix name (a.k.a. "short" name). There are also a number
    of optional attributes such as email address, image data, etc.
    
    Group identities have a membership which may include both users as well as
    other groups. An identity can be tested for membership in a specific group.

    A CSIdentity object is a private copy of the identity information. It can be
    modified in memory, but requires authorization to commit changes back to
    the identity authority database. On Mac OS X version 10.5, only local identities
    can be created, modified or deleted, and only by users with Administrator 
    credentials.

    Changes may be committed synchronously or asynchronously. All data validation
    occurs at commit time.

    Two identities are CFEqual if they have the same class and UUID.
*/


/*
 *  CSIdentityRef
 *  
 *  Discussion:
 *    A reference to an identity object. Can be either a user or group.
 */
typedef struct __CSIdentity*            CSIdentityRef;

/*
 *  CSIdentityQueryRef
 *  
 *  Discussion:
 *    A reference to an identity query object, used to lookup
 *    identities in an identity authority's database.
 */
typedef struct __CSIdentityQuery*       CSIdentityQueryRef;
/*
    kCSIdentityGeneratePosixName
    Passing this constant as the Posix name when creating an indentity
    will generate a unique Posix name, based on the full name. The Posix 
    name is generated at commit time.
*/
/*
 *  kCSIdentityGeneratePosixName
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kCSIdentityGeneratePosixName                API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  CSIdentityClass
 *  
 *  Discussion:
 *    Enum specifying an identity class
 */
enum {

  /*
   * The class value for user identities
   */
  kCSIdentityClassUser          = 1,

  /*
   * The class value for group identities
   */
  kCSIdentityClassGroup         = 2
};

typedef CFIndex                         CSIdentityClass;

/*
 *  CSIdentityFlags
 *  
 *  Discussion:
 *    Flags used when creating new identities
 */
enum {

  /*
   * Use this flag to set no optional attributes for a new identity
   */
  kCSIdentityFlagNone           = 0,

  /*
   * This flag causes the identity to be "hidden," that is, excluded
   * from most user-visible identity lists. Hidden identities include
   * administrative users and groups such as root, www, and mysql.
   * System service access control groups should be created with the
   * hidden flag.
   */
  kCSIdentityFlagHidden         = 1
};

typedef CFOptionFlags                   CSIdentityFlags;
/*
 *  CSIdentityGetTypeID()
 *  
 *  Summary:
 *    Returns the CSIdentity type identifier
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    The CFTypeID of the CSIdentity Core Foundation type
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
CSIdentityGetTypeID(void)                                     API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *
 *  Creating Identities
 *
 */
/*
 *  CSIdentityCreate()
 *  
 *  Summary:
 *    Creates a new identity
 *  
 *  Discussion:
 *    The new identity is allocated but is not committed to the
 *    identity authority's database. It will become persistent and
 *    available to other clients after being committed using
 *    CSIdentityCommit or CSIdentityCommitAsynchronously.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use when creating the object. NULL is
 *      equivalent to specifying kCFAllocatorDefault.
 *    
 *    identityClass:
 *      The type of identity to be created. Specifying
 *      kCSIdentityClassUser creates a user, while
 *      kCSIdentityClassGroup creates a group.
 *    
 *    fullName:
 *      The primary name of the new identity.
 *    
 *    posixName:
 *      The POSIX name of the new identity. Specify
 *      kCSIdentityGeneratePosixName to have a name generated
 *      autmatically from the full name.
 *    
 *    flags:
 *      A CSIdentityFlags mask defining attributes of the new identity
 *    
 *    authority:
 *      The identity authority to host the identity. Caller must have
 *      write access to the identity authority or commit will fail.
 *      Currently, only local identities may be created, so callers
 *      must specify the local identity authority for this argument.
 *  
 *  Result:
 *    The CSIdentityRef of the newly created identity object. Returns
 *    NULL only if allocation fails.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityRef 
CSIdentityCreate(
  CFAllocatorRef           allocator,
  CSIdentityClass          identityClass,
  CFStringRef              fullName,
  CFStringRef              posixName,
  CSIdentityFlags          flags,
  CSIdentityAuthorityRef   authority)                         API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityCreateCopy()
 *  
 *  Summary:
 *    Creates a copy of an identity
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for the new identity. NULL is equivalent
 *      to specifying kCFAllocatorDefault.
 *    
 *    identity:
 *      The identity to copy
 *  
 *  Result:
 *    The CSIdentityRef of the newly created identity object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityRef 
CSIdentityCreateCopy(
  CFAllocatorRef   allocator,
  CSIdentityRef    identity)                                  API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *
 *  Getting Identity Attributes
 *
 */
/*
 *  CSIdentityGetClass()
 *  
 *  Summary:
 *    Returns an identity's class
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity object to access
 *  
 *  Result:
 *    The CSIdentityClass of an identity
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityClass 
CSIdentityGetClass(CSIdentityRef identity)                    API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityGetAuthority()
 *  
 *  Summary:
 *    Returns the identity authority of an identity
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity object to access
 *  
 *  Result:
 *    A CSIdentityAuthorityRef object
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityAuthorityRef 
CSIdentityGetAuthority(CSIdentityRef identity)                API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityGetUUID()
 *  
 *  Summary:
 *    Returns an identity's UUID.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity object to access
 *  
 *  Result:
 *    A CFUUID object containing identity's UUID. Will never return
 *    NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFUUIDRef 
CSIdentityGetUUID(CSIdentityRef identity)                     API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityGetFullName()
 *  
 *  Summary:
 *    Retrieve the full name of an identity
 *  
 *  Discussion:
 *    The full name is the name that is displayed in the user interface.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity object to access
 *  
 *  Result:
 *    Returns an identity's full name as a CFStringRef. This attribute
 *    is always non-NULL. The identity object may release its reference
 *    to the return value when the identity is modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
CSIdentityGetFullName(CSIdentityRef identity)                 API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityGetPosixID()
 *  
 *  Summary:
 *    Retrieve POSIX ID of an identity.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Result:
 *    Returns an identity's POSIX identifier (a UID or GID).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern id_t 
CSIdentityGetPosixID(CSIdentityRef identity)                  API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityGetPosixName()
 *  
 *  Summary:
 *    Retrieve the POSIX name (short name) of an identity.
 *  
 *  Discussion:
 *    The POSIX name cannot be changed after an identity has been
 *    created.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity object to access.
 *  
 *  Result:
 *    Returns an identity's POSIX name. This attribute is always
 *    non-NULL. The identity object may release its reference to the
 *    return value when the identity is modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
CSIdentityGetPosixName(CSIdentityRef identity)                API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSIdentityGetEmailAddress()
 *  
 *  Summary:
 *    Retrieve the email address of a user identity
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Result:
 *    Returns the email address of the identity or NULL if there is no
 *    email address. The identity object may release its reference to
 *    the return value when the identity is modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
CSIdentityGetEmailAddress(CSIdentityRef identity)             API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityGetImageURL()
 *  
 *  Summary:
 *    Retrieve the URL to an identity's image file
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Result:
 *    Returns a CFURLRef that contains the location of the user's image
 *    file, or NULL if there is no image URL. The identity object may
 *    release its reference to the return value when the identity is
 *    modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFURLRef 
CSIdentityGetImageURL(CSIdentityRef identity)                 API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityGetImageData()
 *  
 *  Summary:
 *    Retrieve the image associated with a user identity
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Result:
 *    Returns the identity's image data as a CFDataRef or NULL if there
 *    is no image data. The identity object may release its reference
 *    to the return value when the identity is modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDataRef 
CSIdentityGetImageData(CSIdentityRef identity)                API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityGetImageDataType()
 *  
 *  Summary:
 *    Retrieve the uniform type identifier (UTI) of an identity's image
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Result:
 *    Returns a UTI as a CFStringRef for this identity's image data or
 *    NULL if there is no image data. The identity object may release
 *    its reference to the return value when the identity is modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
CSIdentityGetImageDataType(CSIdentityRef identity)            API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityGetAliases()
 *  
 *  Summary:
 *    Retrieve the aliases of an identity.
 *  
 *  Discussion:
 *    Aliases are alternate names for identities. As with all identity
 *    names, aliases must be unique within the entire namespace of of
 *    the identity authority.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Result:
 *    Returns an array containing the identity's name aliases as
 *    CFStringRefs. The array may be empty. The identity object may
 *    release its reference to the return value when the identity is
 *    modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
CSIdentityGetAliases(CSIdentityRef identity)                  API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityIsMemberOfGroup()
 *  
 *  Summary:
 *    Check if an identity is a memeber of a group
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity whose membership is in question
 *    
 *    group:
 *      The group identity whose membership is to be checked
 *  
 *  Result:
 *    Returns true if the identity is a member (directly or indirectly)
 *    of the specified group
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityIsMemberOfGroup(
  CSIdentityRef   identity,
  CSIdentityRef   group)                                      API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityIsHidden()
 *  
 *  Summary:
 *    Determine if a identity's hidden attribute is enabled
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity object to access
 *  
 *  Result:
 *    Returns true if the identity was created with the hidden attribute
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityIsHidden(CSIdentityRef identity)                    API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityCreatePersistentReference()
 *  
 *  Summary:
 *    Create an opaque, persistent data reference to an identity
 *  
 *  Discussion:
 *    A persistent identity reference is an opaque data object from
 *    which an identity object may queried the future (see
 *    CSIdentityQueryCreateForPersistentReference). A persistent
 *    reference is suitable for storage in an external data store, for
 *    example, as an entry in an application-specific access control
 *    list associated with a shared resource. Use of a persistent
 *    identity reference is preferred over a pure UUID-based identity
 *    reference because the peristent reference contains additional
 *    information needed to optimize the identity query and to improve
 *    the user experience when working in a distributed identity
 *    environment (LDAP, Active Directory, etc.).
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator for the data
 *    
 *    identity:
 *      The identity to reference
 *  
 *  Result:
 *    Returns a new persistent reference for the identity
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFDataRef 
CSIdentityCreatePersistentReference(
  CFAllocatorRef   allocator,
  CSIdentityRef    identity)                                  API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *
 *  Methods which apply only to users
 *
 */

/*
 *  CSIdentityIsEnabled()
 *  
 *  Summary:
 *    Determine if a user is enabled
 *  
 *  Discussion:
 *    A user that is not enabled cannot authenticate. This setting may
 *    be used to temporarily allow a user's access to all services and
 *    resources.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    user:
 *      The user identity to access
 *  
 *  Result:
 *    Returns true if the user is enabled. A user that is not enabled
 *    cannot authenticate.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityIsEnabled(CSIdentityRef user)                       API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityAuthenticateUsingPassword()
 *  
 *  Summary:
 *    Attempt to autenticate a password for a user identity
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    user:
 *      The user identity to access
 *    
 *    password:
 *      The password to authenticate
 *  
 *  Result:
 *    Returns true if the passord is correct for the specified user
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityAuthenticateUsingPassword(
  CSIdentityRef   user,
  CFStringRef     password)                                   API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityGetCertificate()
 *  
 *  Summary:
 *    Get a user's authentication certificate
 *  
 *  Discussion:
 *    The authentication certificate can be used in PKI-based protocols
 *    to authenticate users.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    user:
 *      The user identity to access
 *  
 *  Result:
 *    The identity's certificate, or NULL if there is no certificate. 
 *    The identity object may release its reference to the return value
 *    when the identity is modified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern SecCertificateRef 
CSIdentityGetCertificate(CSIdentityRef user)                  API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *
 *  Methods which apply only to groups
 *
 */

/*
 *  CSIdentityCreateGroupMembershipQuery()
 *  
 *  Summary:
 *    Creates a query to find a group's members
 *  
 *  Discussion:
 *    Using a query to lookup group membership allows the caller to
 *    execute the query synchronously or asynchronously.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    allocator:
 *      The allocator to use for the query
 *    
 *    group:
 *      The group identity whose members are to be queried
 *  
 *  Result:
 *    The CSIdentityQueryRef of the newly created object. The query is
 *    ready to be executed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityQueryRef 
CSIdentityCreateGroupMembershipQuery(
  CFAllocatorRef   allocator,
  CSIdentityRef    group)                                     API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *
 *  Methods that modify identities
 *
 */

/*
 *  CSIdentitySetFullName()
 *  
 *  Summary:
 *    Sets an identity's full name.
 *  
 *  Discussion:
 *    This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity object to access
 *    
 *    fullName:
 *      The new full name of the identity
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentitySetFullName(
  CSIdentityRef   identity,
  CFStringRef     fullName)                                   API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentitySetEmailAddress()
 *  
 *  Summary:
 *    Set an identity's email address
 *  
 *  Discussion:
 *    This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The user identity to access
 *    
 *    emailAddress:
 *      The user's new email address value. Pass NULL to remove an
 *      email address.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentitySetEmailAddress(
  CSIdentityRef   identity,
  CFStringRef     emailAddress)       /* can be NULL */       API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentitySetImageURL()
 *  
 *  Summary:
 *    Set the URL of an identity's external image storage
 *  
 *  Discussion:
 *    This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *    
 *    url:
 *      The URL file of the image. For local identities, this must be a
 *      file URL. Pass NULL to remove the image URL from the identity.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentitySetImageURL(
  CSIdentityRef   identity,
  CFURLRef        url)            /* can be NULL */           API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentitySetImageData()
 *  
 *  Summary:
 *    Set the internally-stored image data and data type for an identity
 *  
 *  Discussion:
 *    This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *    
 *    imageData:
 *      The image data. Pass NULL to remove image data.
 *    
 *    imageDataType:
 *      The uniform type identitier (UTI) of the image data. Currently,
 *      kUTTypeJPEG ("public.jpeg") is the only type supported.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentitySetImageData(
  CSIdentityRef   identity,
  CFDataRef       imageData,           /* can be NULL */
  CFStringRef     imageDataType)       /* can be NULL */      API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityAddAlias()
 *  
 *  Summary:
 *    Add a name alias to an identity
 *  
 *  Discussion:
 *    This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *    
 *    alias:
 *      The alias to add
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentityAddAlias(
  CSIdentityRef   identity,
  CFStringRef     alias)                                      API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityRemoveAlias()
 *  
 *  Summary:
 *    Remove an alias name from an identity
 *  
 *  Discussion:
 *    This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *    
 *    alias:
 *      The alias name to remove
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentityRemoveAlias(
  CSIdentityRef   identity,
  CFStringRef     alias)                                      API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *
 *  Methods that modify group membership
 *
 */

/*
 *  CSIdentityAddMember()
 *  
 *  Summary:
 *    Add an identity to a group
 *  
 *  Discussion:
 *    This change to the group must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    group:
 *      The group identity to access
 *    
 *    member:
 *      The identity to add to the group. Can be a user or group
 *      identity.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentityAddMember(
  CSIdentityRef   group,
  CSIdentityRef   member)                                     API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityRemoveMember()
 *  
 *  Summary:
 *    Remove a member from a group
 *  
 *  Discussion:
 *    This change to the group must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    group:
 *      The group identity to access
 *    
 *    member:
 *      The member identity to remove
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentityRemoveMember(
  CSIdentityRef   group,
  CSIdentityRef   member)                                     API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/* 


    Methods that modfify user credentials


*/

/*
 *  CSIdentitySetIsEnabled()
 *  
 *  Summary:
 *    Enable or disable a user
 *  
 *  Discussion:
 *    A disabled user account cannot authenticate. Credentials
 *    (password and certificate) are not affected. This change must be
 *    committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    user:
 *      The identity object to access
 *    
 *    isEnabled:
 *      The new value of the isEnabled attribute
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentitySetIsEnabled(
  CSIdentityRef   user,
  Boolean         isEnabled)                                  API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentitySetPassword()
 *  
 *  Summary:
 *    Set a user password
 *  
 *  Discussion:
 *    Setting the password to NULL removes the current password and
 *    disables password authentication for the user. Setting the
 *    password to a zero-length string allows authentication with a
 *    blank password. This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    user:
 *      The user identity to access
 *    
 *    password:
 *      The new password, or NULL to remove the current password and
 *      disable password-based authentication
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentitySetPassword(
  CSIdentityRef   user,
  CFStringRef     password)       /* can be NULL */           API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentitySetCertificate()
 *  
 *  Summary:
 *    Set a user's authentication certificate
 *  
 *  Discussion:
 *    The subject name in the certificate will function as an alias for
 *    the identity. As with all identity names, the subject name must
 *    be unique within the entire name space of the identity authority.
 *    This change must be submitted.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    user:
 *      The user identity to access
 *    
 *    certificate:
 *      The user's certificate, or NULL to remove the current
 *      certificate
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentitySetCertificate(
  CSIdentityRef       user,
  SecCertificateRef   certificate)       /* can be NULL */    API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *
 *  Permanent Deletion
 *
 */
/*
 *  CSIdentityDelete()
 *  
 *  Summary:
 *    Permanently delete an identity from the identity database
 *  
 *  Discussion:
 *    Sets an identity to deleted state. This change must be committed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to delete
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentityDelete(CSIdentityRef identity)                      API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *
 *
 *  Committing changes
 *
 */


#if TARGET_OS_OSX
/*
 *  CSIdentityCommit()
 *  
 *  Summary:
 *    Synchronously commit all pending changes to the identity
 *    authority database
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to commit
 *    
 *    authorization:
 *      The authorization object holding credentials necessary to allow
 *      modification to the identity database. As a convenience,
 *      callers may pass NULL for the authorization, and the
 *      implmentation will attempt to acquire the necessary credentials
 *      from Authorization Services.
 *    
 *    error:
 *      Optional pointer to a CFErrorRef which will be set if this
 *      function returns false. When this occurs, the caller is
 *      responsible for releasing the error.
 *  
 *  Result:
 *    Returns true if successful, false if an error occurred
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityCommit(
  CSIdentityRef      identity,
  AuthorizationRef   authorization,       /* can be NULL */
  CFErrorRef *       error)               /* can be NULL */   API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );
	
#endif // TARGET_OS_OSX


/*
 *  Status values
 *  
 */
enum {

  /*
   * The identity has been committed to the authority database
   */
  kCSIdentityCommitCompleted    = 1
};


typedef void (*CSIdentityStatusUpdatedCallback)(CSIdentityRef identity, CFIndex status, CFErrorRef error, void *info);

/*
 *  CSIdentityClientContext
 *  
 *  Discussion:
 *    Structure containing the user-defined data and callbacks used
 *    during asynchronous commits
 */
struct CSIdentityClientContext {

  /*
   * The version number of the client structure type.  The current
   * version number is 0.
   */
  CFIndex             version;

  /*
   * An arbitrary pointer to client-defined data, which can be
   * associated with the client and is passed to the callbacks.
   */
  void *              info;

  /*
   * The callback used to add a retain for the on the client object for
   * the life of the asynchronous operation, and may be used for
   * temporary references the identity needs to take. This callback
   * returns the actual info pointer to be passed to the statusUpdated
   * callback. May be NULL.
   */
  CFAllocatorRetainCallBack  retain;

  /*
   * The callback used to remove a retain previously acquired for the
   * client object. May be NULL.
   */
  CFAllocatorReleaseCallBack  release;

  /*
   * The callback used to create a descriptive string representation of
   * the client object for debugging purposes. This is used by the
   * CFCopyDescription() function. May be NULL.
   */
  CFAllocatorCopyDescriptionCallBack  copyDescription;

  /*
   * The client callback invoked when the status of an asnchronous
   * operation changes
   */
  CSIdentityStatusUpdatedCallback  statusUpdated;
};
typedef struct CSIdentityClientContext  CSIdentityClientContext;


#if TARGET_OS_OSX
/*
 *  CSIdentityCommitAsynchronously()
 *  
 *  Summary:
 *    Asychronously commit all pending changes to the identity
 *    authority's database
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to commit
 *    
 *    clientContext:
 *      The client structure specifying context and callbacks for the
 *      asynchronous operation
 *    
 *    runLoop:
 *      The run loop on which to schedule the statusUpdated callback
 *    
 *    runLoopMode:
 *      The run loop mode in which the callback can be scheduled
 *    
 *    authorization:
 *      The authorization object holding credentials necessary to allow
 *      modification to the identity database. As a convenience,
 *      callers may pass NULL for the authorization, and the
 *      implmentation will attempt to acquire the necessary credentials
 *      from Authorization Services. Modifying the local system
 *      identity database requires Admin credentials.
 *  
 *  Result:
 *    Returns true if the commit operation is started, indicated that
 *    an statusUpdated callback will follow. Returns false if the
 *    identity has no uncommitted changes or a commit is already in
 *    progress
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityCommitAsynchronously(
  CSIdentityRef                    identity,
  const CSIdentityClientContext *  clientContext,
  CFRunLoopRef                     runLoop,
  CFStringRef                      runLoopMode,
  AuthorizationRef                 authorization)       /* can be NULL */ API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );
#endif // TARGET_OS_OSX


/*
 *  CSIdentityIsCommitting()
 *  
 *  Summary:
 *    Determine if a commit operation is in progress
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Result:
 *    Returns true if a commit operation is in progress
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSIdentityIsCommitting(CSIdentityRef identity)                API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityRemoveClient()
 *  
 *  Summary:
 *    Invalidate an identity's client structure to stop client callbacks
 *  
 *  Discussion:
 *    After returning, this function guarantees that client callbacks
 *    will never be invoked again. Use this function when releasing an
 *    identity which may have an outstanding asynchronous request. This
 *    function does not cancel an outstanding commit operation because
 *    a commit cannot be interrupted.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    identity:
 *      The identity to access
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSIdentityRemoveClient(CSIdentityRef identity)                API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __CSIDENTITY__ */

