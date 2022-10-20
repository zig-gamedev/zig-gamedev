/*
     File:       OpenScripting/DigitalHubRegistry.h
 
     Contains:   Digital Hub AppleEvents
 
     Version:    OpenScripting
 
     Copyright:  © 2002-2008 by Apple Computer, Inc.  All rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DIGITALHUBREGISTRY__
#define __DIGITALHUBREGISTRY__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif


#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

/* class */
enum {
  kDigiHubEventClass            = 'dhub'
};


/* events*/
enum {
  kDigiHubMusicCD               = 'aucd', /* 1635083108 0x61756364*/
  kDigiHubPictureCD             = 'picd', /* 1885954916 0x70696364*/
  kDigiHubVideoDVD              = 'vdvd', /* 1986295396 0x76647664*/
  kDigiHubBlankCD               = 'bcd ', /* 1650680864 0x62636420*/
  kDigiHubBlankDVD              = 'bdvd' /* 1650751076 0x62647664*/
};

/*
    Parameters for Digital Hub AppleEvents:
                    
        kDigiHubMusicCD
        Required parameters:
        -->     keyDirectObject         typeFSRef

        kDigiHubPictureCD
        Required parameters:
        -->     keyDirectObject         typeFSRef
                
        kDigiHubVideoDVD
        Required parameters:
        -->     keyDirectObject         typeFSRef
                
        kDigiHubBlankCD
        Required parameters:
        -->     keyDirectObject         typeUTF8Text

        kDigiHubBlankDVD
        Required parameters:
        -->     keyDirectObject         typeUTF8Text

*/


#endif /* __DIGITALHUBREGISTRY__ */

