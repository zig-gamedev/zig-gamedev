/*
 * Copyright (c) 1999-2000, 2009 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*
 This API has been fully replaced by API in sysdir.h.
 
 This API was deprecated because its enumerated types and many of the same identifiers
 for those enumerated types were identical to the types and identifiers found
 in Foundation/NSPathUtilities.h and including both headers caused compile errors.
*/

#ifndef __NS_SYSTEM_DIRECTORIES_H__
#define __NS_SYSTEM_DIRECTORIES_H__

#include <sys/cdefs.h>
#include <Availability.h>

typedef enum {
    NSApplicationDirectory          = 1,    // deprecated -- use SYSDIR_DIRECTORY_APPLICATION
    NSDemoApplicationDirectory      = 2,    // deprecated -- use SYSDIR_DIRECTORY_DEMO_APPLICATION
    NSDeveloperApplicationDirectory = 3,    // deprecated -- use SYSDIR_DIRECTORY_DEVELOPER_APPLICATION
    NSAdminApplicationDirectory     = 4,    // deprecated -- use SYSDIR_DIRECTORY_ADMIN_APPLICATION
    NSLibraryDirectory              = 5,    // deprecated -- use SYSDIR_DIRECTORY_LIBRARY
    NSDeveloperDirectory            = 6,    // deprecated -- use SYSDIR_DIRECTORY_DEVELOPER
    NSUserDirectory                 = 7,    // deprecated -- use SYSDIR_DIRECTORY_USER
    NSDocumentationDirectory        = 8,    // deprecated -- use SYSDIR_DIRECTORY_DOCUMENTATION
    NSDocumentDirectory             = 9,    // deprecated -- use SYSDIR_DIRECTORY_DOCUMENT
    NSCoreServiceDirectory          = 10,   // deprecated -- use SYSDIR_DIRECTORY_CORESERVICE
    NSAutosavedInformationDirectory = 11,   // deprecated -- use SYSDIR_DIRECTORY_AUTOSAVED_INFORMATION
    NSDesktopDirectory              = 12,   // deprecated -- use SYSDIR_DIRECTORY_DESKTOP
    NSCachesDirectory               = 13,   // deprecated -- use SYSDIR_DIRECTORY_CACHES
    NSApplicationSupportDirectory   = 14,   // deprecated -- use SYSDIR_DIRECTORY_APPLICATION_SUPPORT
    NSDownloadsDirectory            = 15,   // deprecated -- use SYSDIR_DIRECTORY_DOWNLOADS
    NSInputMethodsDirectory         = 16,   // deprecated -- use SYSDIR_DIRECTORY_INPUT_METHODS
    NSMoviesDirectory               = 17,   // deprecated -- use SYSDIR_DIRECTORY_MOVIES
    NSMusicDirectory                = 18,   // deprecated -- use SYSDIR_DIRECTORY_MUSIC
    NSPicturesDirectory             = 19,   // deprecated -- use SYSDIR_DIRECTORY_PICTURES
    NSPrinterDescriptionDirectory   = 20,   // deprecated -- use SYSDIR_DIRECTORY_PRINTER_DESCRIPTION
    NSSharedPublicDirectory         = 21,   // deprecated -- use SYSDIR_DIRECTORY_SHARED_PUBLIC
    NSPreferencePanesDirectory      = 22,   // deprecated -- use SYSDIR_DIRECTORY_PREFERENCE_PANES
    NSAllApplicationsDirectory      = 100,  // deprecated -- use SYSDIR_DIRECTORY_ALL_APPLICATIONS
    NSAllLibrariesDirectory         = 101,  // deprecated -- use SYSDIR_DIRECTORY_ALL_LIBRARIES
} NSSearchPathDirectory; // deprecated -- use sysdir_search_path_directory_t


typedef enum {
    NSUserDomainMask            = 1,        // deprecated -- use SYSDIR_DOMAIN_MASK_USER
    NSLocalDomainMask           = 2,        // deprecated -- use SYSDIR_DOMAIN_MASK_LOCAL
    NSNetworkDomainMask         = 4,        // deprecated -- use SYSDIR_DOMAIN_MASK_NETWORK
    NSSystemDomainMask          = 8,        // deprecated -- use SYSDIR_DOMAIN_MASK_SYSTEM
    NSAllDomainsMask            = 0x0ffff,  // deprecated -- use SYSDIR_DOMAIN_MASK_ALL
} NSSearchPathDomainMask; // deprecated -- use sysdir_search_path_domain_mask_t


typedef unsigned int NSSearchPathEnumerationState; // deprecated -- use sysdir_search_path_enumeration_state

__BEGIN_DECLS

extern NSSearchPathEnumerationState
NSStartSearchPathEnumeration(NSSearchPathDirectory dir, NSSearchPathDomainMask domainMask) __API_DEPRECATED("Use sysdir_start_search_path_enumeration() in sysdir.h", macosx(10.0, 10.12), ios(2.0, 10.0), watchos(1.0, 3.0), tvos(9.0, 10.0));

extern NSSearchPathEnumerationState
NSGetNextSearchPathEnumeration(NSSearchPathEnumerationState state, char *path) __API_DEPRECATED("Use sysdir_get_next_search_path_enumeration() in sysdir.h", macosx(10.0, 10.12), ios(2.0, 10.0), watchos(1.0, 3.0), tvos(9.0, 10.0));

__END_DECLS

#endif /* __NS_SYSTEM_DIRECTORIES_H__ */
