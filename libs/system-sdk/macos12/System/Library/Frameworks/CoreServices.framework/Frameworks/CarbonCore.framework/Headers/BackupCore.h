/*
     File:       CarbonCore/BackupCore.h
 
     Contains:   Backup low level Interfaces.
 
     Copyright:  © 2006-2011 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __BACKUPCORE__
#define __BACKUPCORE__

#ifndef __CFURL__
#include <CoreFoundation/CFURL.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  CSBackupSetItemExcluded()
 *  
 *  Discussion:
 *    Add or remove an item from the list of items excluded from
 *    backup. When backing up, the backup daemon skips items marked by
 *    this call. If a folder is marked for exclusion, it and its
 *    contents are excluded from backup. When specifying by path, it is
 *    OK to pass  a URL of an item/folder that does not exist yet.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    item:
 *      The URL of the file/folder to be added or removed from the
 *      exclusion list.
 *    
 *    exclude:
 *      true to exclude an item, false to stop excluding it.
 *    
 *    excludeByPath:
 *      false to make the exclusion bit 'sticky', i.e. follow the item
 *      if it gets moved, true to tie the exclusion to an absolute path.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CSBackupSetItemExcluded(
  CFURLRef   item,
  Boolean    exclude,
  Boolean    excludeByPath)                                   __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_NA);


/*
 *  CSBackupIsItemExcluded()
 *  
 *  Discussion:
 *    Report whether or not an item is being excluded from backup.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    item:
 *      A URL to the item of interest.
 *    
 *    excludeByPath:
 *      pass an optional return address to determine whether or not the
 *      given item is excluded as an absolute path or whether it is
 *      sticky to the item. Can be NULL.
 *  
 *  Result:
 *    true if the item or any of its ancestors are excluded from
 *    backup, false otherwise.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in CoreServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
CSBackupIsItemExcluded(
  CFURLRef   item,
  Boolean *  excludeByPath)                                   __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_NA);



#ifdef __cplusplus
}
#endif

#endif /* __BACKUPCORE__ */

