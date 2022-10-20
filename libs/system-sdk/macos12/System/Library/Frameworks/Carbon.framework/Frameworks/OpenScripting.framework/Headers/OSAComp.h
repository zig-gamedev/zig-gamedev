/*
     File:       OpenScripting/OSAComp.h
 
     Contains:   AppleScript Component Implementor's Interfaces.
 
     Version:    OpenScripting
 
     Copyright:  © 1992-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OSACOMP__
#define __OSACOMP__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif


#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
    Types and Constants
**************************************************************************/
/**************************************************************************
    Routines for Associating a Storage Type with a Script Data Handle 
**************************************************************************/
/*
 *  OSAGetStorageType()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSErr 
OSAGetStorageType(
  AEDataStorage   scriptData,
  DescType *      dscType)                                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSAAddStorageType()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSErr 
OSAAddStorageType(
  AEDataStorage   scriptData,
  DescType        dscType)                                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  OSARemoveStorageType()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSErr 
OSARemoveStorageType(AEDataStorage scriptData)                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



#ifdef __cplusplus
}
#endif

#endif /* __OSACOMP__ */

