// AppleArchive header blob: entry ACL

#pragma once

#ifndef __APPLE_ARCHIVE_H
#error Include AppleArchive.h instead of this file
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull begin")
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Qualifier type
//
// Darwin returns a guid_t, which may be resolved to uid_t or gid_t (mbr_uuid_to_id + getpwuid/getgrgid), or to a SID (mbr_uuid_to_sid)
// Windows returns a SID, which may be resolved to host+account names (ConvertSidToStringSid)
// When creating an ACL entry, we'll try to resolve user/group or account, and fallback to store an SID, or an UUID.
// When extracting the archive, the client will have to translate the qualifiers to the target system using the provided callback.
// The constant values are the characters used in the ACL blob
typedef uint32_t AAACEQualifierType APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AAACEQualifierTypes, uint32_t) {

  AA_ACE_QUALIFIER_TYPE_USER  = 'U',         // user/account name string
  AA_ACE_QUALIFIER_TYPE_GROUP = 'G',         // group name string
  AA_ACE_QUALIFIER_TYPE_SID   = 'S',         // ascii SID string
  AA_ACE_QUALIFIER_TYPE_UUID  = 'I',         // ascii UUID string

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// tag, permissions, flags for each ACL entry (ACE)
typedef acl_tag_t AAACETag APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef uint64_t AAACEPermSet APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef uint64_t AAACEFlagSet APPLE_ARCHIVE_SWIFT_PRIVATE;

// ACL blob storage:
//
// ACL blob ::= (ACE blob)*
// ACE blob ::= { uint32_t ace_size; char type; uint64_t flags; uint64_t perms; char qualifier_type; uint8_t qualifier[]; }
// ace_size is the total size of the ACE blob, i.e. qualifier_size + 22 bytes
// The stored values for type, flags, perms, qualifier_type should _not_ be assumed to match the acl.h constants on each SDK

#pragma mark - AAEntryACLBlob

// Entry struct used for the API functions
// Each SDK may offer a different definition for this struct, and they are mapped to a common AppleArchive internal storage.
typedef struct {

  AAACETag tag;                        // one of ACL_EXTENDED_ALLOW, ACL_EXTENDED_DENY
  AAACEPermSet perms;                  // bitwise OR of zero or more acl_perm_t values
  AAACEFlagSet flags;                  // bitwise OR of zero or more acl_flag_t values
  AAACEQualifierType qualifier_type;   // one of AA_ACL_QUALIFIER_TYPE_*

} AAAccessControlEntry APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Create an empty ACL (Access Control List blob)

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API _Nullable AAEntryACLBlob AAEntryACLBlobCreate(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Destroy ACL
*/
APPLE_ARCHIVE_API void AAEntryACLBlobDestroy(
  AAEntryACLBlob _Nullable acl)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create from encoded data representing an ACL field

  @param data encoded data to parse
  @param data_size number of bytes in \p data

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AAEntryACLBlob _Nullable AAEntryACLBlobCreateWithEncodedData(
  const uint8_t * data,
  size_t data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstact Create ACL blob from filesystem object DIR+"/"+PATH

  @param dir reference directory
  @param path additional path under \p dir, may be empty
  @param flags operation flags

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AAEntryACLBlob _Nullable AAEntryACLBlobCreateWithPath(
  const char * dir,
  const char * path,
  AAFlagSet flags)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Apply ACL contents to filesystem object DIR+"/"+PATH

  @discussion
  If \p AA_FLAG_IGNORE_EPERM is set, the call won't fail on permission errors (EPERM)
  If \p AA_FLAG_REPLACE_ATTRIBUTES is set, the call will clear ACL first

  @param acl target object
  @param dir reference directory
  @param path additional path under \p dir, may be empty
  @param flags operation flags

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryACLBlobApplyToPath(
  AAEntryACLBlob acl,
  const char * dir,
  const char * path,
  AAFlagSet flags)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get number of entries in ACL

  @return the number of entries >= 0
*/
APPLE_ARCHIVE_API uint32_t AAEntryACLBlobGetEntryCount(
  AAEntryACLBlob acl)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get ACL entry contents

  @discussion
  Set all fields in \p entry to the contents of entry \p i in \p acl.
  \p qualifier_size is always set to the actual qualifier size in ACL.
  If \p qualifier_capacity is 0, \p qualifier_value is not accessed and can be 0.
  If \p qualifier_capacity >= qualifier_size, \p qualifier_value receives a copy of the qualifier.
  Otherwise, the call fails.

  @param acl target object
  @param i index of entry to get
  @param ace access control entry
  @param qualifier_capacity is the number of bytes allocated in \p qualifier_value, or 0 for size query only
  @param qualifier_value receives the ACE qualifier value if capacity is large enough, not accessed is capacity is 0
  @param qualifier_size receives the ACE qualifier size, if not NULL

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryACLBlobGetEntry(
  AAEntryACLBlob acl,
  uint32_t i,
  AAAccessControlEntry * ace,
  size_t qualifier_capacity,
  uint8_t * _Nullable qualifier_value,
  size_t * _Nullable qualifier_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Append an ACL entry

  @discussion
  Append \p entry, \p qualifier_value.
  \p qualifier_value must point to \p qualifier_size bytes of data

  @param acl target object
  @param ace access control entry
  @param qualifier_value ACE qualifier value, \p qualifier_size bytes
  @param qualifier_size ACE qualifier size

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryACLBlobAppendEntry(
  AAEntryACLBlob acl,
  const AAAccessControlEntry * ace,
  const uint8_t * qualifier_value,
  size_t qualifier_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Update an ACL entry

  @discussion
  Set entry \p i in ACL to \p entry, \p qualifier_value.
  \p qualifier_value must point to \p qualifier_size bytes of data

  @param acl target object
  @param i index of entry to update
  @param ace access control entry
  @param qualifier_size ACE qualifier size
  @param qualifier_value ACE qualifier value, \p qualifier_size bytes

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryACLBlobSetEntry(
  AAEntryACLBlob acl,
  uint32_t i,
  const AAAccessControlEntry * ace,
  const uint8_t * qualifier_value,
  size_t qualifier_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove all ACL entries

  @param acl target object

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryACLBlobClear(
  AAEntryACLBlob acl)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove one ACL entry

  @param acl target object
  @param i index of entry to remove

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryACLBlobRemoveEntry(
  AAEntryACLBlob acl,
  uint32_t i)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get size of the data blob encoding the ACL field

  @param acl target object

  @return blob size >= 0
*/
APPLE_ARCHIVE_API size_t AAEntryACLBlobGetEncodedSize(AAEntryACLBlob acl)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get data blob encoding the ACL field

  @discussion
  This pointer becomes invalid after ACL is modified, or destroyed.

  @param acl target object

  @return a pointer to the ACL blob data on success, and 0 on failure
*/
APPLE_ARCHIVE_API const uint8_t * _Nullable AAEntryACLBlobGetEncodedData(AAEntryACLBlob acl)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
