/*
     File:       SecurityHI/SecurityHI.h
 
     Contains:   Master include for SecurityHI private framework
 
     Copyright:  © 2000-2008 by Apple Inc., all rights reserved.
 
     Warning:    *** APPLE INTERNAL USE ONLY ***
                 This file may contain unreleased API's
 
     BuildInfo:  Built by:            khenrickson
                 On:                  Fri Aug 10 12:01:34 2012
                 With Interfacer:     3.0d46   (Mac OS X for PowerPC)
                 From:                SecurityHI.i
                     Revision:        1.2
                     Dated:           2003/05/22 21:07:47
                     Last change by:  mortay
                     Last comment:    merged #3195145
 
     Bugs:       Report bugs to Radar component "System Interfaces", "Latest"
                 List the version information (from above) in the Problem Description.
 
*/
#ifndef __SECURITYHI__
#define __SECURITYHI__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif


#ifndef __KEYCHAINHI__
#include <SecurityHI/KeychainHI.h>
#endif

#ifndef __URLACCESS__
#include <SecurityHI/URLAccess.h>
#endif

#ifndef __SECCERTIFICATESUPPORT__
#include <SecurityHI/SecCertificateSupport.h>
#endif

#endif /* __SECURITYHI__ */

