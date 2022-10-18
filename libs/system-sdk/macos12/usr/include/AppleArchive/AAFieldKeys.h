// AAFieldKeys

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

// Field key
typedef union {

  char skey[4];                // skey[0,1,2] must be upper-case non-zero ASCII characters; skey[3] must be 0
  uint32_t ikey;               // ikey is the same 4 bytes as a 32-bit integer (the most significant byte is 0)

} AAFieldKey APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Create an empty field key set
  @return a new non zero instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAFieldKeySet _Nullable AAFieldKeySetCreate(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a field key set from a string representation

  @discussion
  Create a set from 0-terminated string \p s="AAA,BBB,CCC,DDD" (length must be exactly 4*N-1).
  The keys do not need to be ordered in \p s.  Characters do not need to be uppercase.

  @param s the string to parse

  @return a new non zero instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAFieldKeySet _Nullable AAFieldKeySetCreateWithString(const char * s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a clone of a field key set

  @param key_set object to copy

  @return a new non zero instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAFieldKeySet _Nullable AAFieldKeySetClone(AAFieldKeySet key_set)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Destroy a field key set

  @discussion Do nothing if \p key_set is NULL

  @param key_set target object
*/
APPLE_ARCHIVE_API void AAFieldKeySetDestroy(AAFieldKeySet _Nullable key_set)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove all entries in a field key set

  @param key_set target object

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetClear(AAFieldKeySet key_set)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Does \p key_set contain \p key?

  @param key_set target object
  @param key query key

  @return 1 if \p key is in \p key_set, 0 if not, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetContainsKey(AAFieldKeySet key_set, AAFieldKey key)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Insert \p key to \p key_set

  @discussion Do nothing if \p key is already in \p key_set

  @param key_set target object
  @param key value to add

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetInsertKey(AAFieldKeySet key_set, AAFieldKey key)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove \p key from \p key_set

  @discussion Do nothing if \p key is not in \p key_set

  @param key_set target object
  @param key value to remove

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetRemoveKey(AAFieldKeySet key_set, AAFieldKey key)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Insert all keys in \p s to \p key_set

  @discussion key_set = key_set UNION s

  @param key_set target object
  @param s set to add

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetInsertKeySet(AAFieldKeySet key_set, AAFieldKeySet s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Remove all keys in \p s from \p key_set

  @discussion key_set = key_set INTER (NOT s)

  @param key_set target object
  @param s set to remove

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetRemoveKeySet(AAFieldKeySet key_set, AAFieldKeySet s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
 @abstract Remove all keys not in \p s from \p key_set

 @discussion key_set = key_set INTER s

 @param key_set target object
 @param s set to remove

 @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetSelectKeySet(AAFieldKeySet key_set, AAFieldKeySet s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Field key count

  @param key_set target object

  @return number of keys in \p key_set
*/
APPLE_ARCHIVE_API uint32_t AAFieldKeySetGetKeyCount(AAFieldKeySet key_set)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abatract Get key at index \p i

  @param key_set target object
  @param i key index

  @return a valid key on success, and a key with ikey = 0 on failure
 */
APPLE_ARCHIVE_API AAFieldKey AAFieldKeySetGetKey(AAFieldKeySet key_set, uint32_t i)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Serialize \p key_set to string

  @param key_set target object
  @param capacity bytes allocated in \p s, must be at least 4 * NKeys
  @param s receives the string representation of \p key_set

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAFieldKeySetSerialize(AAFieldKeySet key_set, size_t capacity, char * s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
