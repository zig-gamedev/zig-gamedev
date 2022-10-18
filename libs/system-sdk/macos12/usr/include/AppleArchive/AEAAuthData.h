// AppleEncryptedArchive AuthData blob

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

// The auth data blob storage is identical to the AppleArchive XAT blob storage:
//
// AuthData blob ::= (AuthData entry)*
// AuthData entry ::= { uint32_t entry_size; char key[]; uint8_t value[]; }
// entry_size is the total size of the AuthData entry. KEY is 0-terminated.

/*!
  @abstract Create an empty auth data blob

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AEAAuthData _Nullable AEAAuthDataCreate(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Destroy auth data blob

  @param auth_data target object, can be NULL
*/
APPLE_ARCHIVE_API void AEAAuthDataDestroy(
  AEAAuthData _Nullable auth_data)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Create from auth data field in context

  @param context provides the auth data

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AEAAuthData _Nullable AEAAuthDataCreateWithContext(
  AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Get number of entries in auth data

  @param auth_data target object

  @return the number of entries
*/
APPLE_ARCHIVE_API uint32_t AEAAuthDataGetEntryCount(
  AEAAuthData auth_data)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Get a auth data entry

  @discussion
  If \p key_capacity is 0, \p key_length receives strlen(entry_key), and \p key is not accessed.
  If \p key_capacity > strlen(entry_key), \p key_length receives strlen(entry_key), and \p key receives a 0-terminated copy of entry_key.
  Otherwise the call fails.
  If \p data_capacity is 0, \p data_size receives the entry_data size, and \p data is not accessed.
  If \p data_capacity >= entry_data size, \p data_size receives the entry_data size, and \p data receives a copy of entry_data.
  Otherwise the call fails.

  @param auth_data target object
  @param i index of entry to get
  @param key_capacity number of bytes allocated in \p key, or 0 for key length query
  @param key receives 0-terminated key value if \p key_capacity allows it
  @param key_length receives key length (strlen, does not include final 0), if not NULL
  @param data_capacity number of bytes allocated in \p data, or 0 for data size query
  @param data receives data value if \p data_capacity allows it
  @param data_size receives data size, if not NULL

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AEAAuthDataGetEntry(
  AEAAuthData auth_data,
  uint32_t i,
  size_t key_capacity,
  char * _Nullable key,
  size_t * _Nullable key_length,
  size_t data_capacity,
  uint8_t * _Nullable data,
  size_t * _Nullable data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Append a auth data entry

  @discussion
  \p key must be 0-terminated, and \p data must point to \p data_size bytes

  @param auth_data target object
  @param key 0-terminated entry key
  @param data_size entry value size in bytes
  @param data entry value, \p data_size bytes, can be NULL if data_size is 0

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AEAAuthDataAppendEntry(
  AEAAuthData auth_data,
  const char * key,
  const uint8_t * _Nullable data,
  size_t data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Update a auth data entry

  @discussion
  \p key must be 0-terminated, and \p data must point to \p data_size bytes

  @param auth_data target object
  @param i index of entry to update
  @param key 0-terminated entry key
  @param data_size entry value size in bytes
  @param data entry value, \p data_size bytes

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AEAAuthDataSetEntry(
  AEAAuthData auth_data,
  uint32_t i,
  const char * key,
  const uint8_t * _Nullable data,
  size_t data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Remove all auth data entries

  @param auth_data target object

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AEAAuthDataClear(
  AEAAuthData auth_data)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Remove an entry from auth data

  @param auth_data target object
  @param i index of entry to remove

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AEAAuthDataRemoveEntry(
  AEAAuthData auth_data,
  uint32_t i)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Get blob size, size of the data blob encoding the auth data

  @param auth_data target object

  @return blob size >= 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API size_t AEAAuthDataGetEncodedSize(AEAAuthData auth_data)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/*!
  @abstract Get blob data, encoding the header

  @discussion
  This pointer becomes invalid when auth_data is modified, or destroyed.

  @param auth_data target object

  @return a pointer to the encoded blob data on success, and 0 on failure
*/
APPLE_ARCHIVE_API const uint8_t * _Nullable AEAAuthDataGetEncodedData(AEAAuthData auth_data)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
