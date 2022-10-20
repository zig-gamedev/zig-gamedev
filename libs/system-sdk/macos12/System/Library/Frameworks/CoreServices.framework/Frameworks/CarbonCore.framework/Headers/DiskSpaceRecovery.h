/*
     File:       CarbonCore/DiskSpaceRecovery.h
 
     Contains:   Low level interfaces for recovering space
 
     Copyright:  © 2008-2011 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DISKSPACERECOVERY__
#define __DISKSPACERECOVERY__

#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif

#ifndef __CFERROR__
#include <CoreFoundation/CFError.h>
#endif

#ifndef __CFUUID__
#include <CoreFoundation/CFUUID.h>
#endif

#include <dispatch/dispatch.h>

#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


#if __BLOCKS__

/* options to pass to CSDiskSpaceStartRecovery() */
enum {
  kCSDiskSpaceRecoveryOptionNoUI = (1 << 0)
};

typedef int                             CSDiskSpaceRecoveryOptions;

/* callback block invoked by CSDiskSpaceStartRecovery() */
typedef void (^CSDiskSpaceRecoveryCallback)(Boolean succeeded, UInt64 bytesFree, CFErrorRef error);

/*
 *  CSDiskSpaceStartRecovery()
 *  
 *  Discussion:
 *
 *    Asynchronously attempts to recover free space on the specified
 *    volume and continues until at least the specified amount of space
 *    is available or until no more space can be recovered. The
 *    recovery attempt can take a significant amount of time - progress
 *    can be judged by the caller by monitoring the amount of free
 *    space on the volume. The provided callback block will be invoked
 *    once the recovery attempt is complete. Space may be recovered by
 *    removing Time Machine local snapshots, compacting the
 *    GenerationStore, or via other methods in the future such as
 *    removing unneeded cache files or temporary files. The actual
 *    clean up is performed by a helper process and any user interface
 *    or IO is managed by the helper process and not by the caller's
 *    process. The user will be prompted for permission before any
 *    potentially destructive operations are performed such as removing
 *    Time Machine local snapshots. Use the
 *    kCSDiskSpaceRecoveryOptionNoUI flag if you do not want the API to
 *    invoke any space recovery operations which could result in UI
 *    being presented.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.7
 *  
 *  Parameters:
 *    
 *    volumeURL:
 *      The url of the mount point where the space is needed.
 *    
 *    bytesNeeded:
 *      The minimum number of bytes of free space needed on the volume.
 *    
 *    options:
 *      Pass kCSDiskSpaceRecoveryOptionNoUI if user interface should
 *      not be presented (this may restrict the kinds and amount of
 *      space that can be recovered)
 *    
 *    outOperationUUID:
 *      If non NULL will be set to point to a CFUUID identifying the
 *      operation. This CFUUID can be passed to
 *      CSDiskSpaceCancelRecovery() to cancel the operation. The caller
 *      is responsible for releasing the returned CFUUID.
 *    
 *    callbackQueue:
 *      The dispatch queue used to invoke the callback.
 *    
 *    callback:
 *      A block which will be invoked once the free space recovery is
 *      complete. The arguments passed to the block will indicate
 *      whether recovery succeeded or not, how many bytes are now free
 *      on the volume, and any error that was encountered.
 *  
 *  Result:
 *    A CFUUID which uniquely identifies the recover space operation
 *    and can later be passed to CSDiskSpaceCancelRecovery() to cancel
 *    the operation if needed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.7 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSDiskSpaceStartRecovery(
  CFURLRef                      volumeURL,
  UInt64                        bytesNeeded,
  CSDiskSpaceRecoveryOptions    options,
  CFUUIDRef *                   outOperationUUID,
  dispatch_queue_t              callbackQueue,
  CSDiskSpaceRecoveryCallback   callback)                     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);



/*
 *  CSDiskSpaceCancelRecovery()
 *  
 *  Discussion:
 *
 *    Cancels the specified space recovery operation. Cancelation may
 *    take a significant amount of time but this function will return
 *    immediately. Once cancelation is successful the callback provided
 *    to CSDiskSpaceStartRecovery() will be invoked.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.7
 *  
 *  Parameters:
 *    
 *    operationUUID:
 *      The CFUUID for the operation to be canceled as returned by
 *      CSDiskSpaceStartRecovery()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.7 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
CSDiskSpaceCancelRecovery(CFUUIDRef operationUUID)            __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);


/*
 *  CSDiskSpaceGetRecoveryEstimate()
 *  
 *  Discussion:
 *
 *    Returns a quick, conservative estimate of the number of
 *    recoverable bytes on the specified volume. This estimated value
 *    is only intended for display in the user interface. The actual
 *    amount of recoverable space on a volume may be greater than the
 *    estimated space so this function should not be used to attempt to
 *    determine in advance if any particular call to
 *    CSDiskSpaceStartRecovery() would succeed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.7
 *  
 *  Parameters:
 *    
 *    volumeURL:
 *      The url of the mount point of the volume.
 *  
 *  Result:
 *    A UInt64 containing a quick, conservative, rough estimate of the
 *    number of bytes that might be recoverable on a volume.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.7 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern UInt64 
CSDiskSpaceGetRecoveryEstimate(CFURLRef volumeURL)            __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);




#endif


#ifdef __cplusplus
}
#endif

#endif /* __DISKSPACERECOVERY__ */

