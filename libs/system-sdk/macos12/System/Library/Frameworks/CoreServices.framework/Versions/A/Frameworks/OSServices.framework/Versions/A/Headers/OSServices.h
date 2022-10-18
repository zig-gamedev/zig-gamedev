/*
     File:       OSServices/OSServices.h

     Contains:   Master include for OSServices private framework

     Copyright:  (c) 2000-2012 Apple Inc. All rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

*/

#ifndef __OSSERVICES__
#define __OSSERVICES__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#if TARGET_OS_OSX
#ifndef __CARBONCORE__
#include <CarbonCore/CarbonCore.h>
#endif
#endif

#ifndef __CSIDENTITY__
#include <OSServices/CSIdentity.h>
#endif

#ifndef __CSIDENTITYAUTHORITY__
#include <OSServices/CSIdentityAuthority.h>
#endif

#ifndef __CSIDENTITYBASE__
#include <OSServices/CSIdentityBase.h>
#endif

#ifndef __CSIDENTITYQUERY__
#include <OSServices/CSIdentityQuery.h>
#endif

#if TARGET_OS_OSX
#ifndef __ICONSTORAGE__
#include <OSServices/IconStorage.h>
#endif
#endif

#if TARGET_OS_OSX
#ifndef __POWER__
#include <OSServices/Power.h>
#endif
#endif

#if TARGET_OS_OSX
#ifndef __SECURITYCORE__
#include <OSServices/SecurityCore.h>
#endif
#endif

#if TARGET_OS_OSX
#ifndef __WSMETHODINVOCATION__
#include <OSServices/WSMethodInvocation.h>
#endif
#endif

#if TARGET_OS_OSX
#ifndef __WSPROTOCOLHANDLER__
#include <OSServices/WSProtocolHandler.h>
#endif
#endif

#if TARGET_OS_OSX
#ifndef __WSTYPES__
#include <OSServices/WSTypes.h>
#endif
#endif


#endif /* __OSSERVICES__ */
