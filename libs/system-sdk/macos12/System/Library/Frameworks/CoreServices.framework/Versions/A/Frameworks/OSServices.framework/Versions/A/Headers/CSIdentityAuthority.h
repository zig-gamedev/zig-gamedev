/*
     File:       OSServices/CSIdentityAuthority.h
 
     Contains:   CSIdentityAuthority APIs
 
     Copyright:  (c) 2006-2011 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __CSIDENTITYAUTHORITY__
#define __CSIDENTITYAUTHORITY__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  CSIdentityAuthorityRef
 *  
 *  Discussion:
 *    A reference to an identity authority object. An identity
 *    authority is a logical repository for identities.
 */
typedef struct __CSIdentityAuthority*   CSIdentityAuthorityRef;
/*
 *  CSIdentityAuthorityGetTypeID()
 *  
 *  Summary:
 *    Returns the CSIdentityAuthority type identifier
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    The CFTypeID of the CSIdentityAuthority Core Foundation type
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFTypeID 
CSIdentityAuthorityGetTypeID(void)                            API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );



/*
 *  CSGetDefaultIdentityAuthority()
 *  
 *  Summary:
 *    Returns the system's default identity authority
 *  
 *  Discussion:
 *    The default identity authority is a pseudo-authority representing
 *    the union of the local identity authority and the managed
 *    identity authority. The function CSIdentityGetAuthority will
 *    never return the default authority instance.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    The CSIdentityAuthorityRef of the default authority
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityAuthorityRef 
CSGetDefaultIdentityAuthority(void)                           API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSGetLocalIdentityAuthority()
 *  
 *  Summary:
 *    Returns the identity authority for identities defined on the
 *    local host
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    The CSIdentityAuthorityRef of the local authority
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityAuthorityRef 
CSGetLocalIdentityAuthority(void)                             API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSGetManagedIdentityAuthority()
 *  
 *  Summary:
 *    Returns the identity authority for identities defined in the
 *    system's managed directory server(s)
 *  
 *  Discussion:
 *    There is always a valid managed identity authority instance, but
 *    if the system is not bound to any managed directory servers, the
 *    managed identity authority will contain no identities.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Result:
 *    The CSIdentityAuthorityRef of the managed authority
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CSIdentityAuthorityRef 
CSGetManagedIdentityAuthority(void)                           API_AVAILABLE( macos(10.5) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  CSIdentityAuthorityCopyLocalizedName()
 *  
 *  Summary:
 *    Returns the localized name of an identity authority
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    authority:
 *      The identity authority to access
 *  
 *  Result:
 *    A CFStringRef containing the localized authority name
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
CSIdentityAuthorityCopyLocalizedName(CSIdentityAuthorityRef authority) API_AVAILABLE( ios(5.0), macos(10.5), tvos(9.0), watchos(1.0) );




#ifdef __cplusplus
}
#endif

#endif /* __CSIDENTITYAUTHORITY__ */

