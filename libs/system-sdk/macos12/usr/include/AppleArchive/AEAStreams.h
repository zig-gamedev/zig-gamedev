// AppleEncryptedArchive Streams

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

#pragma mark - Encryption/decryption stream open

/**
  @abstract Create a new encryption output stream

  @discussion The stream receiving the data \p encrypted_stream must implement random access \p pwrite.
  The returned stream provides sequential \p write only.

  If the signature mode is not \b NONE, the archive must eventually be signed, either on close if the
  private signing key is provided in \p context, or after closing, if only the public signing key is
  available at this point.  To sign the archive after the stream is closed, it is necessary to
  query the signature encryption key, and pass it with the context when calling \p AEAStreamSign.

  @param encrypted_stream receives encrypted data, only \p pwrite is called
  @param context provides parameters, keys, and associated data
  @param flags stream flags
  @param n_threads is the number of worker threads, 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AEAEncryptionOutputStreamOpen(
  AAByteStream encrypted_stream,
  AEAContext context,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Create a new encryption output stream appending data to an existing archive

  @discussion The stream receiving the data \p encrypted_stream must implement random access \p pread,pwrite.
  The returned stream provides sequential \p write only.  No signature verification is done when the archive
  is opened with this function.

  \p context must have been created from the same \p encrypted_stream.  If the call is successful, \p context
  is updated with the compression algorithm and block size.

  If the signature mode is not \b NONE, the archive must be signed again, either on close if the private signing
  key is provided, or offline if only the public signing key is provided.

  @param encrypted_stream is the encrypted data stream, \p pread,pwrite will be called
  @param context provides parameters, keys, and associated data
  @param flags stream flags
  @param n_threads is the number of worker threads, 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AEAEncryptionOutputStreamOpenExisting(
  AAByteStream encrypted_stream,
  AEAContext context,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Close encryption output stream and collect archive attributes

  @discussion \p stream must be an encryption stream. This function will close the stream,
  as if AAByteStreamClose(stream) were called, and collect the archive raw size,
  archive container size, and archive identifier in \p context. Other fields
  in \p context are not modified.

  @param stream is the encryption output stream
  @param context receives attributes

  @return 0 on success, a negative error code on failure
 */
APPLE_ARCHIVE_API int AEAEncryptionOutputStreamCloseAndUpdateContext(
  AAByteStream stream,
  AEAContext context
)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/**
  @abstract Create a new sequential decryption input stream

  @discussion The stream providing the data \p encrypted_stream must implement sequential \p read.
  The returned stream provides sequential \p read only.
  When this function is called, \p encrypted_stream is expected to be pointing the first byte after
  the prologue loaded in \p context.  This will be the case when \p context is created with
  \p AEAContextCreateWithEncryptedStream

  \p context must have been created from the same \p encrypted_stream.  If the call is successful, \p context
  is updated with the archive attributes.

  @param encrypted_stream provides encrypted data, only \p read is called
  @param context provides parameters, keys, and associated data
  @param flags stream flags
  @param n_threads is the number of worker threads, 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AEADecryptionInputStreamOpen(
  AAByteStream encrypted_stream,
  AEAContext context,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Create a new random access decryption input stream

  @discussion The stream providing the data \p encrypted_stream must implement \p read, \p pread and \p seek.
  The returned stream is read-only and implements \p read, \p pread and \p seek.

  The size specified in \p alloc_limit is the requested limit for memory allocation. Depending on the
  block size and compressor algorithm, we may have to allocate more memory in some cases.  Increasing this value
  allows more blocks to be cached, and increases performance.  Set it to 0 to request minimal allocation, and to
  SIZE_MAX to request best performance.
  The returned stream MUST be closed before \p encrypted_stream

  \p context must have been created from the same \p encrypted_stream.  If the call is successful,
  \p context is updated with the archive attributes.

  @param encrypted_stream provides encrypted data, only \p read is called
  @param context provides parameters, keys, and associated data
  @param alloc_limit is the requested max memory allocation size
  @param flags stream flags
  @param n_threads is the number of worker threads, 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AEADecryptionRandomAccessInputStreamOpen(
  AAByteStream encrypted_stream,
  AEAContext context,
  size_t alloc_limit,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Stream signing

/**
  @abstract Sign an encrypted_stream

  @discussion
  Must be called on a ByteStream with random read-write access to an encrypted stream, _after_ the
  stream has been written and closed. \p context needs to be created from the same stream, and populated
  with the required keys before this call.

  The function signs the archive prologue (replacing the signature segment with 0x00),
  using the \p signing_private_key and \p signing_encryption_key in \p context
  then stores the encrypted signature back in the archive.  Other data in the archive is left unchanged.

  The private key used to sign the archive must match the signing public key used to create the archive,
  or unlocking the archive will fail.

  @param encrypted_stream is the stream to sign
  @param context provides parameters, keys, and associated data

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AEAStreamSign(
  AAByteStream encrypted_stream,
  AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
