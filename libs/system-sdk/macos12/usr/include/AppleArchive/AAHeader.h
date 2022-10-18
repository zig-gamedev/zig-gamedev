// AppleArchive header

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

#pragma mark - AppleArchive header

/*!
  @abstract Create an empty Header

  @return a new non zero instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAHeader _Nullable AAHeaderCreate(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Destroy a Header

  @param header is the header to destroy, do nothing if NULL
*/
APPLE_ARCHIVE_API void AAHeaderDestroy(AAHeader _Nullable header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a Header and initialize with encoded data

  @param data_size number of bytes in \p data
  @param data encoded data to parse

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AAHeader _Nullable AAHeaderCreateWithEncodedData(
  size_t data_size,
  const uint8_t * data)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*
  @abstract Clone a Header

  @param header is the header to clone

  @return a non-zero instance on success, and 0 on failure
 */
APPLE_ARCHIVE_API AAHeader _Nullable AAHeaderClone(
  AAHeader header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Assign header values

  @discussion Assign \p from_header contents to \p header

  @param header is the target header
  @param from_header is the source header

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderAssign(
  AAHeader header,
  AAHeader from_header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create header with fields \p key_set from filesystem object DIR+"/"+PATH

  @discussion
  Always includes the following fields:

    TYP,PAT,LNK (symlinks only),DEV (devices only)          -- entry type, path, definition

  Includes the following fields if present in \p key_set:

    UID,GID,MOD,FLG,MTM,CTM,BTM,INO,SIZ,DUZ                 -- entry metadata
    CKS,SH1,SH2,SH3,SH5                                     -- from entry data (expensive, regular files only)

  Other keys in \p key_set are ignored.
  Previous contents of \p header are discarded by this call, and \p header will be left empty on failure.

  @param key_set fields to include
  @param dir root path
  @param path relative path of filesystem object
  @param flags operation flags

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AAHeader _Nullable AAHeaderCreateWithPath(
  AAFieldKeySet key_set,
  const char * dir,
  const char * path,
  AAFlagSet flags)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Manipulating fields

/*!
  @abstract Get number of fields in \p header

  @param header target object

 @return the number of fields >= 0
*/
APPLE_ARCHIVE_API uint32_t AAHeaderGetFieldCount(AAHeader header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get one index for \p key in \p header

  @discussion
  If more than one field has \p key, the returned index will be any one of these fields

  @param header target object
  @param key query key

  @return index >= 0 if KEY is in \p header, and -1 if not (this is not an error)
*/
APPLE_ARCHIVE_API int AAHeaderGetKeyIndex(AAHeader header, AAFieldKey key)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get type for field \p i in \p header

  @param header target object
  @param i field index

  @return one of AA_FIELD_TYPE_... >= 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderGetFieldType(AAHeader header, uint32_t i)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get key for field \p i in \p header

  @param header target object
  @param i field index

  @return a valid field on success, and a field with ikey = 0 on failure
*/
APPLE_ARCHIVE_API AAFieldKey AAHeaderGetFieldKey(AAHeader header, uint32_t i)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get payload size, total size of all BLOB fields (64-bit)

  @param header target object

  @return payload size >= 0
*/
APPLE_ARCHIVE_API uint64_t AAHeaderGetPayloadSize(AAHeader header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove field \p from \p header

  @param header target object
  @param i field index

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderRemoveField(AAHeader header, uint32_t i)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove all fields from \p header

  @param header target object

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderClear(AAHeader header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Set/Append header field

/*!
  @abstract Set Flag field \p i in \p header

  @param header target object
  @param i field index, or UINT32_MAX to append a new field
  @param key field key

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderSetFieldFlag(AAHeader header, uint32_t i, AAFieldKey key)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Set UInt field \p i in \p header

  @param header target object
  @param i field index, or UINT32_MAX to append a new field
  @param key field key
  @param value new field value

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderSetFieldUInt(AAHeader header, uint32_t i, AAFieldKey key, uint64_t value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Set String field \p i in \p header

  @param header target object
  @param i field index, or UINT32_MAX to append a new field
  @param key field key
  @param value new field value
  @param length new field length

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderSetFieldString(AAHeader header, uint32_t i, AAFieldKey key, const char * value, size_t length)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Set Hash field \p i in \p header

  @param header target object
  @param i field index, or UINT32_MAX to append a new field
  @param key field key
  @param hash_function new field hash function, one of AA_HASH_FUNCTION_*
  @param value new field value

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderSetFieldHash(AAHeader header, uint32_t i, AAFieldKey key, AAHashFunction hash_function, const uint8_t * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Set Timespec field \p i in \p header

  @param header target object
  @param i field index, or UINT32_MAX to append a new field
  @param key field key
  @param value new field value (seconds, nanoseconds)

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderSetFieldTimespec(AAHeader header, uint32_t i, AAFieldKey key, const struct timespec * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Set Blob field \p i in \p header

  @param header target object
  @param i field index, or UINT32_MAX to append a new field
  @param key field key
  @param size new field blob size

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderSetFieldBlob(AAHeader header, uint32_t i, AAFieldKey key, uint64_t size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Append field (inline helpers)

/*!
  @abstract Append flag field \p key to \p header
  @param header target object
  @param key field key
  @return the index >= 0 of the new field on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AAHeaderAppendFieldFlag(AAHeader header, AAFieldKey key)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AAHeaderSetFieldFlag(header, UINT32_MAX, key); }

/*!
  @abstract Append uint field \p key to \p header
  @param header target object
  @param key field key
  @param value field value
  @return the index >= 0 of the new field on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AAHeaderAppendFieldUInt(AAHeader header, AAFieldKey key, uint64_t value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AAHeaderSetFieldUInt(header, UINT32_MAX, key, value); }

/*!
  @abstract Append string field \p key to \p header
  @param header target object
  @param key field key
  @param value pointer to \p length characters, the string doesn't need to be 0-terminated
  @param length string length
  @return the index >= 0 of the new field on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AAHeaderAppendFieldString(AAHeader header, AAFieldKey key, const char * value, size_t length)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AAHeaderSetFieldString(header, UINT32_MAX, key, value, length); }

/*!
  @abstract Append C string field \p key to header
  @param header target object
  @param key field key
  @param value pointer to a 0-terminated string
  @return the index >= 0 of the new field on success, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AAHeaderAppendFieldCString(AAHeader header, AAFieldKey key, const char * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AAHeaderSetFieldString(header, UINT32_MAX, key, value, strlen(value)); }

/*!
  @abstract Append hash field \p key to \p header
  @param header target object
  @param key field key
  @param hash_function hash function, one of AA_HASH_FUNCTION_*
  @param value pointer to hash value (size determined by hash type)
  @return the index >= 0 of the new field on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AAHeaderAppendFieldHash(AAHeader header, AAFieldKey key, AAHashFunction hash_function, const uint8_t * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AAHeaderSetFieldHash(header, UINT32_MAX, key, hash_function, value); }

/*!
  @abstract Append timespec field \p key to \p header
  @param header target object
  @param key field key
  @param value value seconds, nanoseconds
  @return the index >= 0 of the new field on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AAHeaderAppendFieldTimespec(AAHeader header, AAFieldKey key, const struct timespec * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AAHeaderSetFieldTimespec(header, UINT32_MAX, key, value); }

/*!
  @abstract Append blob field \p key to \p header
  @param header target object
  @param key field key
  @param size blob size
  @return the index >= 0 of the new field on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AAHeaderAppendFieldBlob(AAHeader header, AAFieldKey key, uint64_t size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AAHeaderSetFieldBlob(header, UINT32_MAX, key, size); }

#pragma mark - Get field value

/*!
  @abstract Get value for UInt field \p i in \p header

  @param header target object
  @param i field index, field type must match the type expected by this function
  @param value receives the field value

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderGetFieldUInt(AAHeader header, uint32_t i, uint64_t * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get value for String field \p i in \p header

  @discussion
  If \p capacity is 0, \p length receives the length of the string, and \p value not accessed.
  If \p capacity > strlen(string), \p length receives the length of the string, and \p value receives a 0-terminated copy of the string.
  Otherwise, the call fails.

  @param header target object
  @param i field index, field type must match the type expected by this function
  @param capacity number of bytes available in \p value
  @param value receives a 0-terminated copy of the field string on success, can be NULL if \p capacity is 0
  @param length receives the length of the field string on success, if not NULL

  @return 0 and set return value on success, and a negative error code on failure (includes insufficient capacity)
*/
APPLE_ARCHIVE_API int AAHeaderGetFieldString(AAHeader header, uint32_t i, size_t capacity, char * _Nullable value, size_t * _Nullable length)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get value for Hash field \p i in \p header
  If \p capacity is 0, \p hash_function receives the hash function, and \p value is not accessed

  @param header target object
  @param i field index, field type must match the type expected by this function
  @param capacity number of bytes available in \p value
  @param hash_function receives the hash function, if not NULL, one of AA_HASH_FUNCTION_*
  @param value receives the hash value on success, can be NULL if \p capacity is 0

  @return 0 and set return value on success, and a negative error code on failure (includes insufficient capacity)
*/
APPLE_ARCHIVE_API int AAHeaderGetFieldHash(AAHeader header, uint32_t i, size_t capacity, AAHashFunction * _Nullable hash_function, uint8_t * _Nullable value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get value for Timespec field \p i in \p header

  @param header target object
  @param i field index, field type must match the type expected by this function
  @param value receives seconds, nanoseconds

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderGetFieldTimespec(AAHeader header, uint32_t i, struct timespec * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get value for Blob field \p i in \p header

  @param header target object
  @param i field index, field type must match the type expected by this function
  @param size receives the blob size
  @param offset receives the blob offset in the blob section of the entry. Offset 0 is the first byte after the header

  @return 0 and set return value on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAHeaderGetFieldBlob(AAHeader header, uint32_t i, uint64_t * size, uint64_t * offset)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Get field value with key (inline helpers)

/*!
  @abstract Lookup \p key and get UInt field value

  @param header target object
  @param key query key
  @param value receives the field value
  @return 1 if key exists and GetFieldUInt succeeded, 0 if key doesn't exist, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AAHeaderGetFieldUIntWithKey(AAHeader header, AAFieldKey key, uint64_t * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  int i = AAHeaderGetKeyIndex(header, key);
  if (i < 0) return 0; // key not found
  int s = AAHeaderGetFieldUInt(header, (uint32_t)i, value);
  return (s == 0)?1:s;
}

/*!
  @abstract Lookup \p key and get String field value

  @param header target object
  @param key query key
  @param capacity number of bytes available in \p value
  @param length receives the length of the field string on success, if not NULL
  @param value receives a 0-terminated copy of the field string on success, can be NULL if \p capacity is 0

  @return 1 if key exists and GetFieldString succeeded, 0 if key doesn't exist, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AAHeaderGetFieldStringWithKey(AAHeader header, AAFieldKey key, size_t capacity, char * _Nullable value, size_t * _Nullable length)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  int i = AAHeaderGetKeyIndex(header, key);
  if (i < 0) return 0; // key not found
  int s = AAHeaderGetFieldString(header, (uint32_t)i, capacity, value, length);
  return (s == 0)?1:s;
}

/*!
  @abstract Lookup \p key and get Hash field value

  @param header target object
  @param key query key
  @param capacity number of bytes available in \p value
  @param hash_function receives the hash function, if not NULL, one of AA_HASH_FUNCTION_*
  @param value receives the hash value on success, can be NULL if \p capacity is 0

  @return 1 if key exists and GetFieldHash succeeded, 0 if key doesn't exist, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AAHeaderGetFieldHashWithKey(AAHeader header, AAFieldKey key, size_t capacity, AAHashFunction * _Nullable hash_function, uint8_t * _Nullable value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  int i = AAHeaderGetKeyIndex(header, key);
  if (i < 0) return 0; // key not found
  int s = AAHeaderGetFieldHash(header, (uint32_t)i, capacity, hash_function, value);
  return (s == 0)?1:s;
}

/*!
  @abstract Lookup \p key and get Timespec field value

  @param header target object
  @param key query key
  @param value receives seconds, nanoseconds

  @return 1 if key exists and GetFieldTimespec succeeded, 0 if key doesn't exist, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AAHeaderGetFieldTimespecWithKey(AAHeader header, AAFieldKey key, struct timespec * value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  int i = AAHeaderGetKeyIndex(header, key);
  if (i < 0) return 0; // key not found
  int s = AAHeaderGetFieldTimespec(header, (uint32_t)i, value);
  return (s == 0)?1:s;
}

/*!
  @abstract Lookup \p key and get Blob field value

  @param header target object
  @param key query key
  @param size receives the blob size
  @param offset receives the blob offset in the blob section of the entry. Offset 0 is the first byte after the header

  @return 1 if key exists and GetFieldBlob succeeded, 0 if key doesn't exist, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AAHeaderGetFieldBlobWithKey(AAHeader header, AAFieldKey key, uint64_t * size, uint64_t * offset)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  int i = AAHeaderGetKeyIndex(header, key);
  if (i < 0) return 0; // key not found
  int s = AAHeaderGetFieldBlob(header, (uint32_t)i, size, offset);
  return (s == 0)?1:s;
}

#pragma mark - Encoded AppleArchive header

/*!
  @abstract Get header blob size, size of the data blob encoding the header

  @return header blob size >= 0
*/
APPLE_ARCHIVE_API size_t AAHeaderGetEncodedSize(AAHeader header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get header blob data, encoding the header. This pointer becomes invalid when HEADER is modified, or destroyed.

  @return a pointer to the read only header blob data on success, and 0 on failure
*/
APPLE_ARCHIVE_API const uint8_t * _Nullable AAHeaderGetEncodedData(AAHeader header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
