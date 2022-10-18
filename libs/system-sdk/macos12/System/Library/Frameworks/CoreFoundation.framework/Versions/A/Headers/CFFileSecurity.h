/*
 *  CFFileSecurity.h
 *  NSFileSecurity and CFFileSecurity are toll-free bridged.
 *
 *  Copyright (c) 2010-2019, Apple Inc. All rights reserved.
 */

#include <CoreFoundation/CFBase.h>

#if TARGET_OS_MAC || CF_BUILDING_CF || NSBUILDINGFOUNDATION

#if !defined(__COREFOUNDATION_CFFILESECURITY__)
#define __COREFOUNDATION_CFFILESECURITY__ 1

#include <sys/types.h>
#include <sys/acl.h>
#include <sys/fcntl.h>
#include <CoreFoundation/CFUUID.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN

/*
 *	A CFFileSecurity object encapulates a file system object's security information
 *	in a CF object.
 */
typedef struct CF_BRIDGED_MUTABLE_TYPE(NSFileSecurity) __CFFileSecurity* CFFileSecurityRef;

/*
 *	Returns the type identifier for the CFFileSecurity opaque type.
 *
 *	Return Value
 *		The type identifier for the CFFileSecurity opaque type.
 */
CF_EXPORT
CFTypeID CFFileSecurityGetTypeID(void) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	Creates an CFFileSecurity object.
 *
 *	Parameters
 *		allocator
 *			The allocator to use to allocate memory for the new object. Pass
 *			NULL or kCFAllocatorDefault to use the current default allocator.
 *	Return Value
 *		A new CFFileSecurity object, or NULL if there was a problem creating the
 *		object. Ownership follows the Create Rule.
 */
CF_EXPORT
CFFileSecurityRef CFFileSecurityCreate(CFAllocatorRef allocator) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *  Creates a copy of a CFFileSecurity object.
 *
 *  Parameters
 *		allocator
 *			The allocator to use to allocate memory for the new object. Pass
 *			NULL or kCFAllocatorDefault to use the current default allocator.
 *		fileSec
 *			The CFFileSecurity object to copy.
 *	Return Value
 *		A copy of fileSec, or NULL if there was a problem creating the object.
 *		Ownership follows the Create Rule.
 */
CF_EXPORT
CFFileSecurityRef CFFileSecurityCreateCopy(CFAllocatorRef allocator, CFFileSecurityRef fileSec) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine copies the owner UUID associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		ownerUUID
 *			A pointer to storage for the owner UUID.
 *	Return Value
 *		true if ownerUUID is successfully returned; false if there is no owner
 *		UUID property associated with an CFFileSecurity object.
 */
CF_EXPORT
Boolean CFFileSecurityCopyOwnerUUID(CFFileSecurityRef fileSec, CFUUIDRef *ownerUUID) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/*
 *	This routine sets the owner UUID associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		ownerUUID
 *			The owner UUID.
 *	Return Value
 *		true if the owner UUID was successfully set; otherwise, false.
 */
CF_EXPORT
Boolean CFFileSecuritySetOwnerUUID(CFFileSecurityRef fileSec, CFUUIDRef ownerUUID) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/*
 *	This routine copies the group UUID associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		groupUUID
 *			A pointer to storage for the group UUID.
 *	Return Value
 *		true if groupUUID is successfully returned; false if there is no group
 *		UUID property associated with an CFFileSecurity object.
 */
CF_EXPORT
Boolean CFFileSecurityCopyGroupUUID(CFFileSecurityRef fileSec, CFUUIDRef *groupUUID) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine sets the group UUID associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		groupUUID
 *			The group UUID.
 *	Return Value
 *		true if the group UUID was successfully set; otherwise, false.
 */
CF_EXPORT
Boolean CFFileSecuritySetGroupUUID(CFFileSecurityRef fileSec, CFUUIDRef groupUUID) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine copies the access control list (acl_t) associated with an
 *	CFFileSecurity object. The acl_t returned by this routine is a copy and must
 *	be released using acl_free(3). The acl_t is meant to be manipulated using
 *	the acl calls defined in <sys/acl.h>.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		accessControlList
 *			A pointer to storage for an acl_t. The acl_t be released using
 *			acl_free(3)
 *	Return Value
 *		true if the access control list is successfully copied; false if there is
 *		no access control list property associated with the CFFileSecurity object.
 */
CF_EXPORT
Boolean CFFileSecurityCopyAccessControlList(CFFileSecurityRef fileSec, acl_t *accessControlList) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

#define kCFFileSecurityRemoveACL (acl_t) _FILESEC_REMOVE_ACL

/*
 *	This routine will set the access control list (acl_t) associated with an
 *	CFFileSecurityRef. To request removal of an access control list from a
 *	filesystem object, pass in kCFFileSecurityRemoveACL as the acl_t and set
 *	the fileSec on the target object using CFURLSetResourcePropertyForKey and
 *	the kCFURLFileSecurityKey. Setting the accessControlList to NULL will result
 *	in the property being unset.
 *
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		accessControlList
 *			The acl_t to set, or kCFFileSecurityRemoveACL to remove the access
 *			control list, or NULL to unset the accessControlList.
 *	Return Value
 *		true if the access control list is successfully set; otherwise, false.
 */
CF_EXPORT
Boolean CFFileSecuritySetAccessControlList(CFFileSecurityRef fileSec, acl_t accessControlList) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine gets the owner uid_t associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		owner
 *			A pointer to where the owner uid_t will be put.
 *	Return Value
 *		true if owner uid_t is successfully obtained; false if there is no owner
 *		property associated with an CFFileSecurity object.
 */
CF_EXPORT
Boolean CFFileSecurityGetOwner(CFFileSecurityRef fileSec, uid_t *owner) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine sets the owner uid_t associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		owner
 *			The owner uid_t.
 *	Return Value
 *		true if the owner uid_t was successfully set; otherwise, false.
 */
CF_EXPORT
Boolean CFFileSecuritySetOwner(CFFileSecurityRef fileSec, uid_t owner) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine gets the group gid_t associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		owner
 *			A pointer to where the group gid_t will be put.
 *	Return Value
 *		true if group gid_t is successfully obtained; false if there is no group
 *		property associated with an CFFileSecurity object.
 */
CF_EXPORT
Boolean CFFileSecurityGetGroup(CFFileSecurityRef fileSec, gid_t *group) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine sets the group gid_t associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		owner
 *			The group gid_t.
 *	Return Value
 *		true if the group gid_t was successfully set; otherwise, false.
 */
CF_EXPORT
Boolean CFFileSecuritySetGroup(CFFileSecurityRef fileSec, gid_t group) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine gets the mode_t associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		owner
 *			A pointer to where the mode_t will be put.
 *	Return Value
 *		true if mode_t is successfully obtained; false if there is no mode
 *		property associated with an CFFileSecurity object.
 */
CF_EXPORT
Boolean CFFileSecurityGetMode(CFFileSecurityRef fileSec, mode_t *mode) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*
 *	This routine sets the mode_t associated with an CFFileSecurity object.
 *  
 *	Parameters
 *		fileSec
 *			The CFFileSecurity object.
 *		owner
 *			The mode_t.
 *	Return Value
 *		true if the mode_t was successfully set; otherwise, false.
 */
CF_EXPORT
Boolean CFFileSecuritySetMode(CFFileSecurityRef fileSec, mode_t mode) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/* values to pass in the clearPropertyMask to CFFileSecurityClearProperties */
typedef CF_OPTIONS(CFOptionFlags, CFFileSecurityClearOptions) {
    kCFFileSecurityClearOwner               = 1UL << 0,
    kCFFileSecurityClearGroup               = 1UL << 1,
    kCFFileSecurityClearMode                = 1UL << 2,
    kCFFileSecurityClearOwnerUUID           = 1UL << 3,
    kCFFileSecurityClearGroupUUID           = 1UL << 4,
    kCFFileSecurityClearAccessControlList   = 1UL << 5
} API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/*
 *	This routine clears file security properties in the CFFileSecurity object.
 *  
 *	Parameters
 *		clearPropertyMask
 *			The file security properties to clear.
 *	Return Value
 *		true if the file security properties were successfully cleared; otherwise, false.
 */
CF_EXPORT
Boolean CFFileSecurityClearProperties(CFFileSecurityRef fileSec, CFFileSecurityClearOptions clearPropertyMask) API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));


CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif /* ! __COREFOUNDATION_CFFILESECURITY__ */
#endif

