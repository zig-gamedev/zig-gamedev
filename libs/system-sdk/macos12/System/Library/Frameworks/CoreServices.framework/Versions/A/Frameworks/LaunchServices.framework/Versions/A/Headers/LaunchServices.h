/*
     File:       LaunchServices.h
 
     Contains:   Public interfaces for LaunchServices.framework
 
     Copyright:  (c) 2003-2012 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/

*/

#ifndef __LAUNCHSERVICES__
#define __LAUNCHSERVICES__

#include <TargetConditionals.h>


#if TARGET_OS_OSX
#ifndef __ICONSCORE__
#include <LaunchServices/IconsCore.h>
#endif
#endif // TARGET_OS_OSX

#ifndef __LSINFO__
#include <LaunchServices/LSInfo.h>
#endif

#ifndef __LSOPEN__
#include <LaunchServices/LSOpen.h>
#endif

#if TARGET_OS_OSX
#ifndef __LSQUARANTINE__
#include <LaunchServices/LSQuarantine.h>
#endif
#endif // TARGET_OS_OSX


#ifndef __UTCORETYPES__
#include <LaunchServices/UTCoreTypes.h>
#endif

#ifndef __UTTYPE__
#include <LaunchServices/UTType.h>
#endif

#endif /* __LAUNCHSERVICES__ */
