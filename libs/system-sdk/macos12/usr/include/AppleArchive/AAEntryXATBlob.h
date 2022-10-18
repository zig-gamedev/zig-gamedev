// AppleArchive header field: XAT blob

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

// XAT blob storage:
//
// XAT blob ::= (XAT entry)*
// XAT entry ::= { uint32_t attr_size; char key[]; uint8_t value[]; }
// attr_size is the total size of the XAT entry. KEY is 0-terminated.

#pragma mark - AAEntryXATBlob

/*!
  @abstract Create an empty XAT (extended attributes blob)

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AAEntryXATBlob _Nullable AAEntryXATBlobCreate(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Destroy XAT

  @param xat target object
*/
APPLE_ARCHIVE_API void AAEntryXATBlobDestroy(
  AAEntryXATBlob _Nullable xat)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create from encoded data representing an XAT field

  @param data encoded data to parse
  @param data_size number of bytes in \p data

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AAEntryXATBlob _Nullable AAEntryXATBlobCreateWithEncodedData(
  const uint8_t * _Nullable data,
  size_t data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstact Create XAT blob from filesystem object DIR+"/"+PATH

  @param dir reference directory
  @param path additional path under \p dir, may be empty
  @param flags operation flags

  @return a non-zero instance on success, and 0 on failure
*/
APPLE_ARCHIVE_API AAEntryXATBlob _Nullable AAEntryXATBlobCreateWithPath(
  const char * dir,
  const char * path,
  AAFlagSet flags)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Apply XAT contents to filesystem object DIR+"/"+PATH

  @discussion
  If \p AA_FLAG_IGNORE_EPERM is set, the call won't fail on permission errors (EPERM)
  If \p AA_FLAG_REPLACE_ATTRIBUTES is set, the call will clear extended attributes first

  @param xat target object
  @param dir reference directory
  @param path additional path under \p dir, may be empty
  @param flags operation flags

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryXATBlobApplyToPath(
  AAEntryXATBlob xat,
  const char * dir,
  const char * path,
  AAFlagSet flags)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get number of entries in XAT

  @param xat target object

  @return the number of entries
*/
APPLE_ARCHIVE_API uint32_t AAEntryXATBlobGetEntryCount(
  AAEntryXATBlob xat)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get a XAT entry

  @discussion
  If \p key_capacity is 0, \p key_length receives strlen(attribute_key), and \p key is not accessed.
  If \p key_capacity > strlen(attribute_key), \p key_length receives strlen(attribute_key), and \p key receives a 0-terminated copy of attribute_key.
  Otherwise the call fails.
  If \p data_capacity is 0, \p data_size receives the attribute_data size, and \p data is not accessed.
  If \p data_capacity >= attribute_data size, \p data_size receives the attribute_data size, and \p data receives a copy of attribute_data.
  Otherwise the call fails.

  @param xat target object
  @param i index of entry to get
  @param key_capacity number of bytes allocated in \p key, or 0 for key length query
  @param key receives 0-terminated key value if \p key_capacity allows it
  @param key_length receives key length (strlen, does not include final 0), if not NULL
  @param data_capacity number of bytes allocated in \p data, or 0 for data size query
  @param data receives data value if \p data_capacity allows it
  @param data_size receives data size, if not NULL

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryXATBlobGetEntry(
  AAEntryXATBlob xat,
  uint32_t i,
  size_t key_capacity,
  char * _Nullable key,
  size_t * _Nullable key_length,
  size_t data_capacity,
  uint8_t * _Nullable data,
  size_t * _Nullable data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Append a XAT entry

  @discussion
  \p key must be 0-terminated, and \p data must point to \p data_size bytes

  @param xat target object
  @param key 0-terminated attribute key
  @param data_size attribute value size in bytes
  @param data attribute value, \p data_size bytes, can be NULL if data_size is 0

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryXATBlobAppendEntry(
  AAEntryXATBlob xat,
  const char * key,
  const uint8_t * _Nullable data,
  size_t data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Update a XAT entry

  @discussion
  \p key must be 0-terminated, and \p data must point to \p data_size bytes

  @param xat target object
  @param i index of entry to update
  @param key 0-terminated attribute key
  @param data_size attribute value size in bytes
  @param data attribute value, \p data_size bytes

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryXATBlobSetEntry(
  AAEntryXATBlob xat,
  uint32_t i,
  const char * key,
  const uint8_t * _Nullable data,
  size_t data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove all XAT entries

  @param xat target object

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryXATBlobClear(
  AAEntryXATBlob xat)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove an entry from XAT

  @param xat target object
  @param i index of entry to remove

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAEntryXATBlobRemoveEntry(
  AAEntryXATBlob xat,
  uint32_t i)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get blob size, size of the data blob encoding the XAT field

  @param xat target object

  @return blob size >= 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API size_t AAEntryXATBlobGetEncodedSize(AAEntryXATBlob xat)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get blob data, encoding the header

  @discussion
  This pointer becomes invalid when XAT is modified, or destroyed.

  @param xat target object

  @return a pointer to the XAT blob data on success, and 0 on failure
*/
APPLE_ARCHIVE_API const uint8_t * _Nullable AAEntryXATBlobGetEncodedData(AAEntryXATBlob xat)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
