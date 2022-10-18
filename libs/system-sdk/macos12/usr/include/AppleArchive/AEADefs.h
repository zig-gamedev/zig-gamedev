// AppleEncryptedArchive Types and Constants

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

#pragma mark - Profiles

// Context profile: <ciphersuite>__<encryption_mode>__<signature_mode>
// Ciphersuite: <key derivation function (KDF)>_<authenticated encryption with additional data (AEAD) function>
typedef uint32_t AEAProfile APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AEAProfiles, uint32_t) {

  AEA_PROFILE__HKDF_SHA256_HMAC__NONE__ECDSA_P256              = 0,
  AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__NONE        = 1,
  AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SYMMETRIC__ECDSA_P256  = 2,
  AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__NONE       = 3,
  AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__ECDHE_P256__ECDSA_P256 = 4,
  AEA_PROFILE__HKDF_SHA256_AESCTR_HMAC__SCRYPT__NONE           = 5,

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// Context fields
// Each field is stored as either uint64_t FieldValue, or as a blob of bytes
typedef uint32_t AEAContextField APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AEAContextFields, uint32_t) {

  AEA_CONTEXT_FIELD_PROFILE                       =  0, // uint, profile
  AEA_CONTEXT_FIELD_PADDING_SIZE                  =  1, // uint, padding size, in bytes, clamped to a restricted range
  AEA_CONTEXT_FIELD_CHECKSUM_MODE                 =  2, // uint, checksum mode, one of AEA_PROFILE_CHECKSUM_*
  AEA_CONTEXT_FIELD_COMPRESSION_ALGORITHM         =  3, // uint, compression algorithm, one of AA_COMPRESSION_ALGORITHM_*
  AEA_CONTEXT_FIELD_COMPRESSION_BLOCK_SIZE        =  4, // uint, compression block size, in bytes, clamped to a restricted range
  AEA_CONTEXT_FIELD_AUTH_DATA                     =  5, // blob, authentication data
  AEA_CONTEXT_FIELD_MAIN_KEY                      =  6, // blob, key used to append to an existing archive for encryption mode \b ECDHE_P256
  AEA_CONTEXT_FIELD_SIGNING_PUBLIC_KEY            =  7, // blob, signing public key for signature mode \b ECDSA_P256
  AEA_CONTEXT_FIELD_SIGNING_PRIVATE_KEY           =  8, // blob, signing private key for signature mode \b ECDSA_P256
  AEA_CONTEXT_FIELD_SYMMETRIC_KEY                 =  9, // blob, symmetric key for encryption mode \b SYMMETRIC
  AEA_CONTEXT_FIELD_RECIPIENT_PUBLIC_KEY          = 10, // blob, recipient public key for encryption mode \b ECDHE_P256 (encryption)
  AEA_CONTEXT_FIELD_RECIPIENT_PRIVATE_KEY         = 11, // blob, recipient private key for encryption mode \b ECDHE_P256 (decryption)
  AEA_CONTEXT_FIELD_SIGNATURE_ENCRYPTION_KEY      = 12, // blob, key used to encrypt signatures
  AEA_CONTEXT_FIELD_RAW_SIZE                      = 13, // uint, raw data size
  AEA_CONTEXT_FIELD_CONTAINER_SIZE                = 14, // uint, container size
  // 15, 16 reserved
  AEA_CONTEXT_FIELD_BLOCKS_PER_CLUSTER            = 17, // uint, number of blocks per AEA cluster
  AEA_CONTEXT_FIELD_ARCHIVE_IDENTIFIER            = 18, // blob, archive unique identifier
  AEA_CONTEXT_FIELD_PASSWORD                      = 19, // blob, password for encryption mode \b SCRYPT (macOS 11.3, iOS 14.5)

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// Context blob field data representation
typedef uint32_t AEAContextFieldRepresentation APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AEAContextFieldRepresentations, uint32_t) {

  AEA_CONTEXT_FIELD_REPRESENTATION_RAW            = 0, // raw binary data
  AEA_CONTEXT_FIELD_REPRESENTATION_X963           = 1, // ANSI x9.63 encoding
  AEA_CONTEXT_FIELD_REPRESENTATION_GENERATE       = 2, // generate a new random password/key

} APPLE_ARCHIVE_SWIFT_PRIVATE;

APPLE_ARCHIVE_ENUM(AEAContextFieldValues, uint32_t) {

  // Context ciphersuite
  // How keys are derived, and data is encrypted
  AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_HMAC        = 0, // RFC5869 HKDF with SHA256 hash key derivation, HMAC SHA256 AEAD (no encryption)
  AEA_CONTEXT_CIPHERSUITE_HKDF_SHA256_AESCTR_HMAC = 1, // RFC5869 HKDF with SHA256 hash key derivation, AES-CTR HMAC SHA256 AEAD encryption

  // Context signature mode
  // How the archive is signed
  AEA_CONTEXT_SIGNATURE_NONE                      = 0, // no signature
  AEA_CONTEXT_SIGNATURE_ECDSA_P256                = 1, // Elliptic Curve DSA with P-256

  // Context encryption mode
  AEA_CONTEXT_ENCRYPTION_NONE                     = 0, // no encryption
  AEA_CONTEXT_ENCRYPTION_SYMMETRIC                = 1, // symmetric key encryption
  AEA_CONTEXT_ENCRYPTION_ECDHE_P256               = 2, // ephemeral Diffie-Hellman
  AEA_CONTEXT_ENCRYPTION_SCRYPT                   = 3, // password (macOS 11.3, iOS 14.5)

  // Context checksum mode
  // How the checksum of uncompressed data is computed
  AEA_CONTEXT_CHECKSUM_NONE                       = 0, // no checksum
  AEA_CONTEXT_CHECKSUM_MURMURHASH64               = 1, // MurMurHash2 64 bits
  AEA_CONTEXT_CHECKSUM_SHA256                     = 2, // SHA-256 256 bits

  // Context padding size
  // Random byte padding is added at the end of the container until container size is multiple of padding size
  // Values < MIN_SIZE are reserved for special padding schemes
  AEA_CONTEXT_PADDING_NONE                        = 0, // no padding
  AEA_CONTEXT_PADDING_ADAPTIVE                    = 1, // Padmé adaptive padding algorithm (https://bford.info/pub/sec/purb.pdf)
  AEA_CONTEXT_PADDING_MIN_SIZE                    = 16, // First non-reserved value

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// Opaque types

typedef struct AEAContext_impl * AEAContext APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef struct AEAAuthData_impl * AEAAuthData APPLE_ARCHIVE_SWIFT_PRIVATE; // macOS 11.3, iOS 14.5

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
