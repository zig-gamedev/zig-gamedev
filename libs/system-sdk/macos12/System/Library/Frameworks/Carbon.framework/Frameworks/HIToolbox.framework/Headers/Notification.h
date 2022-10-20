/*
     File:       HIToolbox/Notification.h
 
     Contains:   Notification Manager interfaces
 
     Copyright:  © 1989-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __NOTIFICATION__
#define __NOTIFICATION__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

typedef struct NMRec                    NMRec;
typedef NMRec *                         NMRecPtr;
typedef CALLBACK_API( void , NMProcPtr )(NMRecPtr nmReqPtr);
typedef STACK_UPP_TYPE(NMProcPtr)                               NMUPP;
struct NMRec {
  QElemPtr            qLink;                  /* next queue entry*/
  short               qType;                  /* queue type -- ORD(nmType) = 8*/
  short               nmFlags;                /* reserved*/
  SRefCon             nmPrivate;              /* reserved*/
  short               nmReserved;             /* reserved*/
  short               nmMark;                 /* item to mark in Apple menu*/
  Handle              nmIcon;                 /* handle to small icon*/
  Handle              nmSound;                /* handle to sound record*/
  StringPtr           nmStr;                  /* string to appear in alert*/
  NMUPP               nmResp;                 /* pointer to response routine*/
  SRefCon             nmRefCon;               /* for application use*/
};

/*
 *  NewNMUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern NMUPP
NewNMUPP(NMProcPtr userRoutine)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeNMUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeNMUPP(NMUPP userUPP)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeNMUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeNMUPP(
  NMRecPtr  nmReqPtr,
  NMUPP     userUPP)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline NMUPP                                                NewNMUPP(NMProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeNMUPP(NMUPP) { }
    inline void                                                 InvokeNMUPP(NMRecPtr nmReqPtr, NMUPP userUPP) { (*userUPP)(nmReqPtr); }
  #else
    #define NewNMUPP(userRoutine)                               ((NMUPP)userRoutine)
    #define DisposeNMUPP(userUPP)
    #define InvokeNMUPP(nmReqPtr, userUPP)                      (*userUPP)(nmReqPtr)
  #endif
#endif

#if !__LP64__
/*
 *  NMInstall()
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
NMInstall(NMRecPtr nmReqPtr)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  NMRemove()
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
NMRemove(NMRecPtr nmReqPtr)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __NOTIFICATION__ */

