// AppleEncryptedArchive Context

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

// The encryption context is an object containing all parameters, keys, additional data needed to open
// an encrypted archive for both encryption and decryption streams.

/**
  @abstract Create a new encryption context from a profile

  @discussion The new context will be initialized with the given \p profile.
  The context must be destroyed after use by calling \p AEAContextDestroy.
  The profile defines the ciphersuite, encryption mode, and signature mode. Other
  context parameters are set to sensible default values.

  @param profile one of the pre-defined profiles

  @return a new context instance on success, NULL on failure
*/
APPLE_ARCHIVE_API AEAContext _Nullable AEAContextCreateWithProfile(AEAProfile profile)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Create an encryption context from an existing encrypted stream

  @discussion The new context will be initialized with the archive prologue read from \p encrypted_stream.
  The caller can then use this context to query the archive profile, unique identifier, and auth data; attach
  the keys required to open the archive, and eventually open a decryption stream for reading,
  or open an encryption stream for append.
  The context must be destroyed after use by calling \p AEAContextDestroy.

  \p read is called on \p encrypted_stream to read the prologue.  \p encrypted_stream is
  expected to be positionned on the first byte of the archive magic before this call, and will point
  to the first byte after the prologue after a successful call.

  @param encrypted_stream provides encrypted data, only \p read is called, and the entire archive prologue will be read.

  @return a new context instance on success, NULL on failure
*/
APPLE_ARCHIVE_API AEAContext _Nullable AEAContextCreateWithEncryptedStream(AAByteStream encrypted_stream)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Destroy an encryption context

  @param context object to destroy, do nothing if NULL
*/
APPLE_ARCHIVE_API void AEAContextDestroy(AEAContext _Nullable context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Get context uint field

  @param context target object
  @param field query field

  @return context field value on success, UINT64_MAX on failure
 */
APPLE_ARCHIVE_API uint64_t AEAContextGetFieldUInt(
  AEAContext context,
  AEAContextField field)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Get context blob field

  @param context target object
  @param field query field
  @param representation specifies the representation to use to store the data in \p buf
  @param buf_capacity bytes allocated in \p buf, can be 0 to query size
  @param buf receives the data, can be NULL if \p buf_capacity is 0
  @param buf_size receives the data size if not NULL (bytes)

  @return 0 on success, a negative error code on failure
 */
APPLE_ARCHIVE_API int AEAContextGetFieldBlob(
  AEAContext context,
  AEAContextField field,
  AEAContextFieldRepresentation representation,
  size_t buf_capacity,
  uint8_t * _Nullable buf,
  size_t * _Nullable buf_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set context uint field

  @param context target object
  @param field field to set
  @param value new field value

  @return 0 on success, a negative error code on failure
 */
APPLE_ARCHIVE_API int AEAContextSetFieldUInt(
  AEAContext context,
  AEAContextField field,
  uint64_t value)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set context blob field

  @param context target object
  @param field field to set
  @param representation specifies the representation used for \p buf data
  @param buf new field value, can be NULL if \p buf_size is 0
  @param buf_size bytes in \p buf

  @return 0 on success, a negative error code on failure
 */
APPLE_ARCHIVE_API int AEAContextSetFieldBlob(
  AEAContext context,
  AEAContextField field,
  AEAContextFieldRepresentation representation,
  const uint8_t * _Nullable buf,
  size_t buf_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Generate random context blob field

  @discussion Uses a cryptographic random number generator to set the field.
  SYMMETRIC_KEY and PASSWORD are the only valid fields that can be set using
  this function.

  @param context target object
  @param field field to set

  @return 0 on success, a negative error code on failure
 */
APPLE_ARCHIVE_API int AEAContextGenerateFieldBlob(
  AEAContext context,
  AEAContextField field)
APPLE_ARCHIVE_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

#pragma mark - Inline helpers

/**
  @abstract Get context profile
  @param context target object
  @return context profile
 */
APPLE_ARCHIVE_INLINE uint32_t AEAContextGetProfile(AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return (uint32_t)AEAContextGetFieldUInt(context, AEA_CONTEXT_FIELD_PROFILE); }

/**
  @abstract Get profile ciphersuite
  @param profile profile
  @return profile ciphersuite
 */
APPLE_ARCHIVE_INLINE uint32_t AEAProfileGetCiphersuite(uint32_t profile)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  switch (profile) {
  case AEA_PROFILE__HKDF_SHA256_HMAC__NONE__ECDSA_P256             : return AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_HMAC;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__NONE       : return AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_AESCTR_HMAC;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__ECDSA_P256 : return AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_AESCTR_HMAC;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__NONE      : return AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_AESCTR_HMAC;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__ECDSA_P256: return AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_AESCTR_HMAC;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SCRYPT__NONE          : return AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_AESCTR_HMAC;
  }
  return UINT32_MAX; // invalid
}

/**
  @abstract Get profile signature mode
  @param profile profile
  @return profile signature mode
 */
APPLE_ARCHIVE_INLINE uint32_t AEAProfileGetSignatureMode(uint32_t profile)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  switch (profile) {
  case AEA_PROFILE__HKDF_SHA256_HMAC__NONE__ECDSA_P256             : return AEA_CONTEXT_SIGNATURE_ECDSA_P256;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__NONE       : return AEA_CONTEXT_SIGNATURE_NONE;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__ECDSA_P256 : return AEA_CONTEXT_SIGNATURE_ECDSA_P256;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__NONE      : return AEA_CONTEXT_SIGNATURE_NONE;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__ECDSA_P256: return AEA_CONTEXT_SIGNATURE_ECDSA_P256;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SCRYPT__NONE          : return AEA_CONTEXT_SIGNATURE_NONE;
  }
  return UINT32_MAX; // invalid
}

/**
  @abstract Get profile encryption mode
  @param profile profile
  @return profile encryption mode
 */
APPLE_ARCHIVE_INLINE uint32_t AEAProfileGetEncryptionMode(uint32_t profile)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  switch (profile) {
  case AEA_PROFILE__HKDF_SHA256_HMAC__NONE__ECDSA_P256             : return AEA_CONTEXT_ENCRYPTION_NONE;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__NONE       : return AEA_CONTEXT_ENCRYPTION_SYMMETRIC;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__ECDSA_P256 : return AEA_CONTEXT_ENCRYPTION_SYMMETRIC;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__NONE      : return AEA_CONTEXT_ENCRYPTION_ECDHE_P256;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__ECDSA_P256: return AEA_CONTEXT_ENCRYPTION_ECDHE_P256;
  case AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SCRYPT__NONE          : return AEA_CONTEXT_ENCRYPTION_SCRYPT;
  }
  return UINT32_MAX; // invalid
}

#pragma mark - Context get

/**
  @abstract Get context padding size, or one of the AEA_CONTEXT_PADDING_* constants
  @discussion This is available only when encrypting.
  @param context target object
  @return context padding mode
 */
APPLE_ARCHIVE_INLINE uint64_t AEAContextGetPaddingSize(AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return (uint64_t)AEAContextGetFieldUInt(context, AEA_CONTEXT_FIELD_PADDING_SIZE); }

/**
  @abstract Get context checksum mode
  @param context target object
  @return context checksum mode
 */
APPLE_ARCHIVE_INLINE uint32_t AEAContextGetChecksumMode(AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return (uint32_t)AEAContextGetFieldUInt(context, AEA_CONTEXT_FIELD_CHECKSUM_MODE); }

/**
  @abstract Get context compression algorithm
  @param context target object
  @return context compression algorithm
*/
APPLE_ARCHIVE_INLINE AACompressionAlgorithm AEAContextGetCompressionAlgorithm(AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return (AACompressionAlgorithm)AEAContextGetFieldUInt(context, AEA_CONTEXT_FIELD_COMPRESSION_ALGORITHM); }

/**
  @abstract Get context compression block size
  @param context target object
  @return context compression block size
*/
APPLE_ARCHIVE_INLINE size_t AEAContextGetCompressionBlockSize(AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return (size_t)AEAContextGetFieldUInt(context, AEA_CONTEXT_FIELD_COMPRESSION_BLOCK_SIZE); }

/**
  @abstract Get archive raw data size
  @param context target object
  @return Uncompressed size of data stored in archive (as stored in root header)
*/
APPLE_ARCHIVE_INLINE uint64_t AEAContextGetRawSize(AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AEAContextGetFieldUInt(context, AEA_CONTEXT_FIELD_RAW_SIZE); }

/**
  @abstract Get archive container size
  @param context target object
  @return Archive container size (as stored in root header)
*/
APPLE_ARCHIVE_INLINE uint64_t AEAContextGetContainerSize(AEAContext context)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AEAContextGetFieldUInt(context, AEA_CONTEXT_FIELD_CONTAINER_SIZE); }

/**
  @abstract Get context auth data
  @param context target object
  @param auth_data_capacity is the number of bytes allocated in \p auth_data, can be 0 to query size
  @param auth_data receives a copy of the context auth data on success (raw bytes), can be NULL when \p auth_data_capacity is 0
  @param auth_data_size receives the auth data size if not NULL (bytes)
  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextGetAuthData(
  AEAContext context,
  size_t auth_data_capacity,
  uint8_t * _Nullable auth_data,
  size_t * _Nullable auth_data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextGetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_AUTH_DATA,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    auth_data_capacity,
    auth_data,
    auth_data_size);
}

/**
  @abstract Get context signature encryption key

  @discussion This key is needed to sign the archive after it has been closed, without requiring the encryption credentials.

  @param context target object
  @param key_capacity bytes allocated in \p key, can be 0 to query size
  @param key receives the key (raw bytes), can be NULL if \p key_capacity is 0
  @param key_size receives the key size if not NULL (bytes)
  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextGetSignatureEncryptionKey(
  AEAContext context,
  size_t key_capacity,
  uint8_t * _Nullable key,
  size_t * _Nullable key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextGetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_SIGNATURE_ENCRYPTION_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    key_capacity,
    key,
    key_size);
}

/**
  @abstract Get archive unique identifier

  @param context target object
  @param identifier_capacity bytes allocated in \p identifier, can be 0 to query size
  @param identifier receives the archive identifier (raw bytes), can be NULL if \p identifier_capacity is 0
  @param identifier_size receives the archive identifier size if not NULL (bytes)
  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextGetArchiveIdentifier(
  AEAContext context,
  size_t identifier_capacity,
  uint8_t * _Nullable identifier,
  size_t * _Nullable identifier_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5))
{
  return AEAContextGetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_ARCHIVE_IDENTIFIER,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    identifier_capacity,
    identifier,
    identifier_size);
}

/**
  @abstract Get archive main key

  @param context target object
  @param key_capacity bytes allocated in \p key, can be 0 to query size
  @param key receives the key (raw bytes), can be NULL if \p key_capacity is 0
  @param key_size receives the key size if not NULL (bytes)
  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextGetMainKey(
  AEAContext context,
  size_t key_capacity,
  uint8_t * _Nullable key,
  size_t * _Nullable key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5))
{
  return AEAContextGetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_MAIN_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    key_capacity,
    key,
    key_size);
}

#pragma mark - Context set

/**
  @abstract Set context compression algorithm
  @param context target object
  @param compression_algorithm algorithm
  @return 0 on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AEAContextSetCompressionAlgorithm(AEAContext context, AACompressionAlgorithm compression_algorithm)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AEAContextSetFieldUInt(context, AEA_CONTEXT_FIELD_COMPRESSION_ALGORITHM, (uint32_t)compression_algorithm); }

/**
  @abstract Set context compression block size
  @param context target object
  @param compression_block_size block size, will be internally clamped to a limited range
  @return 0 on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AEAContextSetCompressionBlockSize(AEAContext context, size_t compression_block_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  if (compression_block_size > UINT32_MAX) compression_block_size = UINT32_MAX;
  return AEAContextSetFieldUInt(context, AEA_CONTEXT_FIELD_COMPRESSION_BLOCK_SIZE, (uint64_t)compression_block_size);
}

/**
  @abstract Set context checksum mode
  @param context target object
  @param checksum_mode checksum mode
  @return 0 on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AEAContextSetChecksumMode(AEAContext context, uint32_t checksum_mode)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{ return AEAContextSetFieldUInt(context, AEA_CONTEXT_FIELD_CHECKSUM_MODE, (uint64_t)checksum_mode); }

/**
  @abstract Set context padding size

  @discussion When encrypting, the final archive size will be multiple of \p padding_size.
  Valid byte sizes are >= AEA_CONTEXT_PADDING_MIN_SIZE.
  Special padding schemes are one of AEA_CONTEXT_PADDING_*, including AEA_CONTEXT_PADDING_NONE (0) for no padding.

  @param context target object
  @param padding_size requested padding size in bytes if >= AEA_CONTEXT_PADDING_MIN_SIZE, or a special padding scheme AEA_CONTEXT_PADDING_*
  @return 0 on success, and a negative error code on failure
 */
APPLE_ARCHIVE_INLINE int AEAContextSetPaddingSize(AEAContext context, uint64_t padding_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldUInt(context, AEA_CONTEXT_FIELD_PADDING_SIZE, padding_size);
}

/**
  @abstract Set context auth data

  @discussion The auth data is an opaque data blob stored unencrypted in the archive prologue.

  @param context target object
  @param auth_data auth data (raw data)
  @param auth_data_size auth data size (bytes)

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetAuthData(
  AEAContext context,
  const uint8_t * auth_data,
  size_t auth_data_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_AUTH_DATA,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    auth_data,
    auth_data_size);
}

/**
  @abstract Set context auth data from encoded key->value blob

  @param context target object
  @param auth_data auth data blob

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetAuthData(
  AEAContext context,
  AEAAuthData auth_data)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_AUTH_DATA,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    AEAAuthDataGetEncodedData(auth_data),
    AEAAuthDataGetEncodedSize(auth_data));
}

/**
  @abstract Set context signature encryption key

  @discussion The signature encryption key can be obtained from a context after it has been used to
  open an encryption stream.  The signature encryption key allows signing the stream after it is closed,
  without requiring the encryption credentials. i.e. the stream can be signed offline by a process not
  able to decrypt it.

  @param context target object
  @param key key data (raw data)
  @param key_size key size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetSignatureEncryptionKey(
  AEAContext context,
  const uint8_t * key,
  size_t key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_SIGNATURE_ENCRYPTION_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    key,
    key_size);
}

/**
  @abstract Set context symmetric key

  @discussion Stores a copy of \p key in \p context.  Required to encrypt/decrypt a stream when
  encryption mode is \b SYMMETRIC.
  An internal size range is enforced for the key. The caller is expected to enforce key strength policies.

  @param context target object
  @param key key (raw data)
  @param key_size key size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetSymmetricKey(
  AEAContext context,
  const uint8_t * key,
  size_t key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_SYMMETRIC_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    key,
    key_size);
}

/**
  @abstract Set context password

  @discussion Stores a copy of \p password in \p context.  Required to encrypt/decrypt a stream when
  encryption mode is \b SCRYPT.
  An internal size range is enforced for the password. The caller is expected to enforce password strength policies.

  @param context target object
  @param password password (raw data)
  @param password_size password size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetPassword(
  AEAContext context,
  const uint8_t * password,
  size_t password_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_PASSWORD,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    password,
    password_size);
}

/**
  @abstract Set context signing public key (x9.63 representation)

  @discussion Stores a copy of \p key in \p context.  This is required to open the stream when
  signature mode is \b ECDSA_P256.  The corresponding private key must be used to sign the archive, and the
  same key must be passed when opening the archive for decryption.

  @param context target object
  @param key key
  @param key_size key size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetSigningPublicKey(
  AEAContext context,
  const uint8_t * key,
  size_t key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_SIGNING_PUBLIC_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_X963,
    key,
    key_size);
}

/**
  @abstract Set context signing private key (x9.63 representation)

  @discussion Stores a copy of \p key in \p context.  This can be passed instead of the signing public key
  to an encryption stream when signature mode is \b ECDSA_P256.  In that case, the encryption stream will
  be signed on close. The corresponding public key must be used to decrypt the stream and verify the signature.

  @param context target object
  @param key key
  @param key_size key size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetSigningPrivateKey(
  AEAContext context,
  const uint8_t * key,
  size_t key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_SIGNING_PRIVATE_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_X963,
    key,
    key_size);
}

/**
  @abstract Set context recipient public key (x9.63 representation)

  @discussion Stores a copy of \p key in \p context.  This is required to encrypt a stream when
  encryption mode is \b ECDHE_P256.  The corresponding private key must be used to decrypt the archive.

  @param context target object
  @param key key
  @param key_size key size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetRecipientPublicKey(
  AEAContext context,
  const uint8_t * key,
  size_t key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_RECIPIENT_PUBLIC_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_X963,
    key,
    key_size);
}

/**
  @abstract Set context recipient private key (x9.63 representation)

  @discussion Stores a copy of \p key in \p context.  This is required to decrypt a stream when
  encryption mode is \b ECDHE_P256.

  @param context target object
  @param key key
  @param key_size key size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetRecipientPrivateKey(
  AEAContext context,
  const uint8_t * key,
  size_t key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_RECIPIENT_PRIVATE_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_X963,
    key,
    key_size);
}

/**
  @abstract Set archive main key

  @discussion The main key can be obtained from a context after it has been used to
  open an encryption stream.  The main key allows re-opening the stream to append data
  to it, and is not intended to be communicated to the archive recipients, or third parties.

  @param context target object
  @param key key data (raw data)
  @param key_size key size (bytes)

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_INLINE int AEAContextSetMainKey(
  AEAContext context,
  const uint8_t * key,
  size_t key_size)
APPLE_ARCHIVE_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5))
{
  return AEAContextSetFieldBlob(
    context,
    AEA_CONTEXT_FIELD_MAIN_KEY,
    AEA_CONTEXT_FIELD_REPRESENTATION_RAW,
    key,
    key_size);
}

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
