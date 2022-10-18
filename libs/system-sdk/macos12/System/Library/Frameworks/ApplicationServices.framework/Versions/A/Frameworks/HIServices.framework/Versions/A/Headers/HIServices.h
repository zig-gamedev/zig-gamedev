/*
     File:       HIServices/HIServices.h
 
     Contains:   Master include for HIServices framework
 
     Copyright:  2002-2018 by Apple, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HISERVICES__
#define __HISERVICES__

#include <TargetConditionals.h>

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#if TARGET_OS_OSX

#ifndef __HISHAPE__
#include <HIServices/HIShape.h>
#endif

#ifndef __ICONS__
#include <HIServices/Icons.h>
#endif

#ifndef __INTERNETCONFIG__
#include <HIServices/InternetConfig.h>
#endif

#ifndef __PROCESSES__
#include <HIServices/Processes.h>
#endif

#ifndef __PASTEBOARD__
#include <HIServices/Pasteboard.h>
#endif

#ifndef __TRANSLATIONSERVICES__
#include <HIServices/TranslationServices.h>
#endif

#ifndef __ACCESSIBILITY__
#include <HIServices/Accessibility.h>
#endif

#ifndef __UNIVERSALACCESS__
#include <HIServices/UniversalAccess.h>
#endif

#endif

#endif /* __HISERVICES__ */

