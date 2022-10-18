/*
     File:       CarbonCore/MultiprocessingInfo.h
 
     Contains:   Multiprocessing Information interfaces
                 The contents of this header file are deprecated.
 
     Copyright:  © 1995-2011 DayStar Digital, Inc.
*/

/*********************************************************************************************
 
 The Multiprocessing Utilites are deprecated.  Callers should use blocks, libDispatch, or pthreads.
  
*********************************************************************************************/
/*
   ==========================================================================================================================
   *** WARNING: You must properly check the availability of MP services before calling them!
   See the section titled "Checking API Availability".
   ==========================================================================================================================
*/


#ifndef __MULTIPROCESSINGINFO__
#define __MULTIPROCESSINGINFO__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MULTIPROCESSING__
#include <CarbonCore/Multiprocessing.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma options align=power

/*
   ======================================= NOTICE ============================================
   As of Mac OS X v10.6, the APIs in this header file are discouraged. These APIs are slated
   for deprecation in the next major release of OS X. The new dispatch APIs (see dispatch(3))
   replace the Multiprocessing APIs and the pthread threading APIs.
   ===========================================================================================
*/

/*
   ==========================================================================================================================
   This is the header file for version 2.3 of the Mac OS multiprocessing information support. 
   ==========================================================================================================================
*/


/*
   ==========================================================================================================================
   The following services are new in version 2.1:
    MPGetNextTaskID
    MPGetNextCpuID
   ==========================================================================================================================
*/

/*
   ==========================================================================================================================
   The following services are new in version 2.2:
    MPGetPageSizeClasses
    MPGetPageSize
    MPGetNextAreaID
   ==========================================================================================================================
*/

/*
   ==========================================================================================================================
   The following services are new in version 2.3:
    MPGetNextCoherenceID
    MPGetNextProcessID
    MPGetNextAddressSpaceID
    MPGetNextQueueID
    MPGetNextSemaphoreID
    MPGetNextCriticalRegionID
    MPGetNextTimerID
    MPGetNextEventID
    MPGetNextNotificationID
    MPGetNextConsoleID
   ==========================================================================================================================
*/



/*
   ¤
   ==========================================================================================================================
   Page size Services
   ==================
*/

/*
 *  MPGetPageSizeClasses()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/* The number of page size classes, 1 to n.*/
/* -------------------------------------------------------------------------------------------*/
/*
 *  MPGetPageSize()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/* The page size in bytes.*/

/*
   ¤
   ==========================================================================================================================
   ID Iterator Services
   ==========================
*/

/*
 *  MPGetNextCoherenceID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextCpuID()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework but deprecated in 10.7
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */
extern OSStatus 
MPGetNextCpuID(
  MPCoherenceID   owningCoherenceID,
  MPCpuID *       cpuID)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_7, __IPHONE_NA, __IPHONE_NA);


/*
 *  MPGetNextProcessID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextAddressSpaceID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextTaskID()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in CoreServices.framework but deprecated in 10.7
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */
extern OSStatus 
MPGetNextTaskID(
  MPProcessID   owningProcessID,
  MPTaskID *    taskID)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_4, __MAC_10_7, __IPHONE_NA, __IPHONE_NA);


/*
 *  MPGetNextQueueID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextSemaphoreID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextCriticalRegionID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextTimerID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextEventID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextNotificationID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextAreaID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNextConsoleID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */




/* -------------------------------------------------------------------------------------------*/


/*
 *  MPGetNextID()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */




/*
   ¤
   ==========================================================================================================================
   Object Information Services
   ===========================
*/


/*
   ----------------------------------------------------------------------------------------------
   ! The implementation of MPGetObjectInfo assumes that all info records are in 4 byte multiples.
*/


enum {
                                        /* The version of the MPAreaInfo structure requested.*/
  kMPQueueInfoVersion           = 1L | (kOpaqueQueueID << 16),
  kMPSemaphoreInfoVersion       = 1L | (kOpaqueSemaphoreID << 16),
  kMPEventInfoVersion           = 1L | (kOpaqueEventID << 16),
  kMPCriticalRegionInfoVersion  = 1L | (kOpaqueCriticalRegionID << 16),
  kMPNotificationInfoVersion    = 1L | (kOpaqueNotificationID << 16),
  kMPAddressSpaceInfoVersion    = 1L | (kOpaqueAddressSpaceID << 16)
};


struct MPQueueInfo {
  PBVersion           version;                /* Version of the data structure requested*/

  MPProcessID         processID;              /* Owning process ID*/
  OSType              queueName;              /* Queue name*/

  ItemCount           nWaiting;
  MPTaskID            waitingTaskID;          /* First waiting task.*/

  ItemCount           nMessages;
  ItemCount           nReserved;

  void *              p1;                     /* First message parameters...*/
  void *              p2;
  void *              p3;
};
typedef struct MPQueueInfo              MPQueueInfo;
struct MPSemaphoreInfo {
  PBVersion           version;                /* Version of the data structure requested*/

  MPProcessID         processID;              /* Owning process ID*/
  OSType              semaphoreName;          /* Semaphore name*/

  ItemCount           nWaiting;
  MPTaskID            waitingTaskID;          /* First waiting task.*/

  ItemCount           maximum;
  ItemCount           count;
};
typedef struct MPSemaphoreInfo          MPSemaphoreInfo;
struct MPEventInfo {
  PBVersion           version;                /* Version of the data structure requested*/

  MPProcessID         processID;              /* Owning process ID*/
  OSType              eventName;              /* Event name*/

  ItemCount           nWaiting;
  MPTaskID            waitingTaskID;          /* First waiting task.*/

  MPEventFlags        events;
};
typedef struct MPEventInfo              MPEventInfo;
struct MPCriticalRegionInfo {
  PBVersion           version;                /* Version of the data structure requested*/

  MPProcessID         processID;              /* Owning process ID*/
  OSType              regionName;             /* Critical region name*/

  ItemCount           nWaiting;
  MPTaskID            waitingTaskID;          /* First waiting task.*/

  MPTaskID            owningTask;
  ItemCount           count;
};
typedef struct MPCriticalRegionInfo     MPCriticalRegionInfo;
struct MPNotificationInfo {
  PBVersion           version;                /* Version of the data structure requested*/

  MPProcessID         processID;              /* Owning process ID*/
  OSType              notificationName;       /* Notification name*/

  MPQueueID           queueID;                /* Queue to notify.*/
  void *              p1;
  void *              p2;
  void *              p3;

  MPEventID           eventID;                /* Event to set.*/
  MPEventFlags        events;

  MPSemaphoreID       semaphoreID;            /* Sempahore to signal.   */
};
typedef struct MPNotificationInfo       MPNotificationInfo;
struct MPAddressSpaceInfo {
  PBVersion           version;                /* Version of the data structure requested*/

  MPProcessID         processID;              /* Owning process ID*/
  MPCoherenceID       groupID;                /* Related coherence group.*/
  ItemCount           nTasks;                 /* Number of tasks in this space.*/
  UInt32              vsid[16];               /* Segment register VSIDs.*/
};
typedef struct MPAddressSpaceInfo       MPAddressSpaceInfo;
/* *** We should put the task info call here instead of in MPExtractTaskState.*/


/*
 *  MPGetQueueInfo()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetSemaphoreInfo()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetEventInfo()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetCriticalRegionInfo()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetNotificationInfo()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */


/*
 *  MPGetAddressSpaceInfo()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in MPDiagnostics 2.3 and later
 */



/* ==========================================================================================================================*/




#pragma options align=reset

#ifdef __cplusplus
}
#endif

#endif /* __MULTIPROCESSINGINFO__ */

