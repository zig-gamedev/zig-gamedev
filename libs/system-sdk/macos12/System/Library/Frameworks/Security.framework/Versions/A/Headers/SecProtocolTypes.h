/*
 * Copyright (c) 2018 Apple Inc. All Rights Reserved.
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

#ifndef SecProtocolTypes_h
#define SecProtocolTypes_h

#include <Security/SecProtocolObject.h>
#include <Security/SecTrust.h>
#include <Security/SecCertificate.h>
#include <Security/SecIdentity.h>
#include <Security/CipherSuite.h>
#include <Security/SecBase.h>

#ifndef SEC_OBJECT_IMPL
/*!
 * These are os_object compatible and ARC-able wrappers around existing CoreFoundation
 * Security types, including: SecTrustRef, SecIdentityRef, and SecCertificateRef. They allow
 * clients to use these types in os_object-type APIs and data structures. The underlying
 * CoreFoundation types may be extracted and used by clients as needed.
 */
SEC_OBJECT_DECL(sec_trust);
SEC_OBJECT_DECL(sec_identity);
SEC_OBJECT_DECL(sec_certificate);
#endif // !SEC_OBJECT_IMPL

/*!
 * @enum tls_protocol_version_t enumeration
 * @abstract Enumerations for the set of supported TLS and DTLS protocol versions.
 *
 * @constant tls_protocol_version_TLSv10 TLS 1.0 [https://tools.ietf.org/html/rfc4346]
 * @constant tls_protocol_version_TLSv11 TLS 1.1 [https://tools.ietf.org/html/rfc2246]
 * @constant tls_protocol_version_TLSv12 TLS 1.2 [https://tools.ietf.org/html/rfc5246]
 * @constant tls_protocol_version_TLSv13 TLS 1.3 [https://tools.ietf.org/html/rfc8446]
 * @constant tls_protocol_version_DTLSv10 DTLS 1.0 [https://tools.ietf.org/html/rfc4347]
 * @constant tls_protocol_version_DTLSv12 DTLS 1.2 [https://tools.ietf.org/html/rfc6347]
 */
typedef CF_ENUM(uint16_t, tls_protocol_version_t) {
    tls_protocol_version_TLSv10 CF_SWIFT_NAME(TLSv10) = 0x0301,
    tls_protocol_version_TLSv11 CF_SWIFT_NAME(TLSv11) = 0x0302,
    tls_protocol_version_TLSv12 CF_SWIFT_NAME(TLSv12) = 0x0303,
    tls_protocol_version_TLSv13 CF_SWIFT_NAME(TLSv13) = 0x0304,
    tls_protocol_version_DTLSv10 CF_SWIFT_NAME(DTLSv10) = 0xfeff,
    tls_protocol_version_DTLSv12 CF_SWIFT_NAME(DTLSv12) = 0xfefd,
};

/*!
 * @enum tls_ciphersuite_t enumeration
 * @abstract Enumerations for the set of supported TLS and DTLS ciphersuites.
 *
 * See https://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml#tls-parameters-4
 * for ciphersuite codepoint allocations and reference RFCs.
 *
 * @constant tls_ciphersuite_RSA_WITH_3DES_EDE_CBC_SHA
 * @constant tls_ciphersuite_RSA_WITH_AES_128_CBC_SHA
 * @constant tls_ciphersuite_RSA_WITH_AES_256_CBC_SHA
 * @constant tls_ciphersuite_RSA_WITH_AES_128_GCM_SHA256
 * @constant tls_ciphersuite_RSA_WITH_AES_256_GCM_SHA384
 * @constant tls_ciphersuite_RSA_WITH_AES_128_CBC_SHA256
 * @constant tls_ciphersuite_RSA_WITH_AES_256_CBC_SHA256
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_AES_256_CBC_SHA
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_AES_128_CBC_SHA
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_AES_256_CBC_SHA
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_AES_128_CBC_SHA256
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_AES_256_CBC_SHA384
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_AES_128_GCM_SHA256
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_AES_256_GCM_SHA384
 * @constant tls_ciphersuite_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256
 * @constant tls_ciphersuite_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256
 * @constant tls_ciphersuite_AES_128_GCM_SHA256
 * @constant tls_ciphersuite_AES_256_GCM_SHA384
 * @constant tls_ciphersuite_CHACHA20_POLY1305_SHA256
 */
typedef CF_ENUM(uint16_t, tls_ciphersuite_t) {
    tls_ciphersuite_RSA_WITH_3DES_EDE_CBC_SHA CF_SWIFT_NAME(RSA_WITH_3DES_EDE_CBC_SHA) = 0x000A,
    tls_ciphersuite_RSA_WITH_AES_128_CBC_SHA CF_SWIFT_NAME(RSA_WITH_AES_128_CBC_SHA) = 0x002F,
    tls_ciphersuite_RSA_WITH_AES_256_CBC_SHA CF_SWIFT_NAME(RSA_WITH_AES_256_CBC_SHA) = 0x0035,
    tls_ciphersuite_RSA_WITH_AES_128_GCM_SHA256 CF_SWIFT_NAME(RSA_WITH_AES_128_GCM_SHA256) = 0x009C,
    tls_ciphersuite_RSA_WITH_AES_256_GCM_SHA384 CF_SWIFT_NAME(RSA_WITH_AES_256_GCM_SHA384) = 0x009D,
    tls_ciphersuite_RSA_WITH_AES_128_CBC_SHA256 CF_SWIFT_NAME(RSA_WITH_AES_128_CBC_SHA256) = 0x003C,
    tls_ciphersuite_RSA_WITH_AES_256_CBC_SHA256 CF_SWIFT_NAME(RSA_WITH_AES_256_CBC_SHA256) = 0x003D,
    tls_ciphersuite_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA CF_SWIFT_NAME(ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA) = 0xC008,
    tls_ciphersuite_ECDHE_ECDSA_WITH_AES_128_CBC_SHA CF_SWIFT_NAME(ECDHE_ECDSA_WITH_AES_128_CBC_SHA) = 0xC009,
    tls_ciphersuite_ECDHE_ECDSA_WITH_AES_256_CBC_SHA CF_SWIFT_NAME(ECDHE_ECDSA_WITH_AES_256_CBC_SHA) = 0xC00A,
    tls_ciphersuite_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA CF_SWIFT_NAME(ECDHE_RSA_WITH_3DES_EDE_CBC_SHA) = 0xC012,
    tls_ciphersuite_ECDHE_RSA_WITH_AES_128_CBC_SHA CF_SWIFT_NAME(ECDHE_RSA_WITH_AES_128_CBC_SHA) = 0xC013,
    tls_ciphersuite_ECDHE_RSA_WITH_AES_256_CBC_SHA CF_SWIFT_NAME(ECDHE_RSA_WITH_AES_256_CBC_SHA) = 0xC014,
    tls_ciphersuite_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 CF_SWIFT_NAME(ECDHE_ECDSA_WITH_AES_128_CBC_SHA256) = 0xC023,
    tls_ciphersuite_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 CF_SWIFT_NAME(ECDHE_ECDSA_WITH_AES_256_CBC_SHA384) = 0xC024,
    tls_ciphersuite_ECDHE_RSA_WITH_AES_128_CBC_SHA256 CF_SWIFT_NAME(ECDHE_RSA_WITH_AES_128_CBC_SHA256) = 0xC027,
    tls_ciphersuite_ECDHE_RSA_WITH_AES_256_CBC_SHA384 CF_SWIFT_NAME(ECDHE_RSA_WITH_AES_256_CBC_SHA384) = 0xC028,
    tls_ciphersuite_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 CF_SWIFT_NAME(ECDHE_ECDSA_WITH_AES_128_GCM_SHA256) = 0xC02B,
    tls_ciphersuite_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 CF_SWIFT_NAME(ECDHE_ECDSA_WITH_AES_256_GCM_SHA384) = 0xC02C,
    tls_ciphersuite_ECDHE_RSA_WITH_AES_128_GCM_SHA256 CF_SWIFT_NAME(ECDHE_RSA_WITH_AES_128_GCM_SHA256) = 0xC02F,
    tls_ciphersuite_ECDHE_RSA_WITH_AES_256_GCM_SHA384 CF_SWIFT_NAME(ECDHE_RSA_WITH_AES_256_GCM_SHA384) = 0xC030,
    tls_ciphersuite_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256 CF_SWIFT_NAME(ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256) = 0xCCA8,
    tls_ciphersuite_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 CF_SWIFT_NAME(ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256) = 0xCCA9,
    tls_ciphersuite_AES_128_GCM_SHA256 CF_SWIFT_NAME(AES_128_GCM_SHA256) = 0x1301,
    tls_ciphersuite_AES_256_GCM_SHA384 CF_SWIFT_NAME(AES_256_GCM_SHA384) = 0x1302,
    tls_ciphersuite_CHACHA20_POLY1305_SHA256 CF_SWIFT_NAME(CHACHA20_POLY1305_SHA256) = 0x1303,
};

/*!
 * @enum tls_ciphersuite_group_t enumeration
 * @abstract Convenience ciphersuite groups that collate ciphersuites of comparable security
 *  properties into a single alias.
 *
 * @constant tls_ciphersuite_group_default
 * @constant tls_ciphersuite_group_compatibility
 * @constant tls_ciphersuite_group_legacy
 * @constant tls_ciphersuite_group_ats
 * @constant tls_ciphersuite_group_ats_compatibility
 */
typedef CF_ENUM(uint16_t, tls_ciphersuite_group_t) {
    tls_ciphersuite_group_default,
    tls_ciphersuite_group_compatibility,
    tls_ciphersuite_group_legacy,
    tls_ciphersuite_group_ats,
    tls_ciphersuite_group_ats_compatibility,
};

/*!
 * @enum SSLProtocol enumeration
 * @abstract Enumerations for the set of supported TLS and DTLS protocol versions.
 *
 * @note This enumeration is deprecated. Use `tls_protocol_version_t` instead.
 */
typedef CF_ENUM(int, SSLProtocol) {
    kSSLProtocolUnknown CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 0,
    kTLSProtocol1 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 4,
    kTLSProtocol11 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 7,
    kTLSProtocol12 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 8,
    kDTLSProtocol1 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 9,
    kTLSProtocol13 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 10,
    kDTLSProtocol12 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 11,
    kTLSProtocolMaxSupported CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 999,
    kSSLProtocol2 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 1,
    kSSLProtocol3 CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 2,
    kSSLProtocol3Only CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 3,
    kTLSProtocol1Only CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 5,
    kSSLProtocolAll CF_ENUM_DEPRECATED(10_2, 10_15, 5_0, 13_0) = 6,
};

__BEGIN_DECLS

SEC_ASSUME_NONNULL_BEGIN

/*!
 * @function sec_trust_create
 *
 * @abstract
 *      Create an ARC-able `sec_trust_t` instance from a `SecTrustRef`.
 *
 * @param trust
 *      A `SecTrustRef` instance.
 *
 * @return a `sec_trust_t` instance.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SEC_RETURNS_RETAINED _Nullable sec_trust_t
sec_trust_create(SecTrustRef trust);

/*!
 * @function sec_trust_copy_ref
 *
 * @abstract
 *      Copy a retained reference to the underlying `SecTrustRef` instance.
 *
 * @param trust
 *      A `sec_trust_t` instance.
 *
 * @return The underlying `SecTrustRef` instance.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SecTrustRef
sec_trust_copy_ref(sec_trust_t trust);

/*!
 * @function sec_identity_create
 *
 * @abstract
 *      Create an ARC-able `sec_identity_t` instance from a `SecIdentityRef`.
 *
 * @param identity
 *      A `SecIdentityRef` instance.
 *
 * @return a `sec_identity_t` instance.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SEC_RETURNS_RETAINED _Nullable sec_identity_t
sec_identity_create(SecIdentityRef identity);

/*!
 * @function sec_identity_create_with_certificates
 *
 * @abstract
 *      Create an ARC-able `sec_identity_t` instance from a `SecIdentityRef` and
 *      array of SecCertificateRef instances.
 *
 * @param identity
 *      A `SecIdentityRef` instance.
 *
 * @param certificates
 *      An array of `SecCertificateRef` instances.
 *
 * @return a `sec_identity_t` instance.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SEC_RETURNS_RETAINED _Nullable sec_identity_t
sec_identity_create_with_certificates(SecIdentityRef identity, CFArrayRef certificates);

#ifdef __BLOCKS__
/*!
 * @function sec_identity_access_certificates
 *
 * @abstract
 *      Access the certificates associated with the `sec_identity_t` instance.
 *
 * @param identity
 *      A `sec_identity_t` instance.
 *
 * @param handler
 *      A block to invoke one or more times with `sec_certificate_t` instances.
 *
 * @return Returns true if the peer certificates were accessible, false otherwise.
 */
API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
bool
sec_identity_access_certificates(sec_identity_t identity,
                                 void (^handler)(sec_certificate_t certificate));
#endif // __BLOCKS__

/*!
 * @function sec_identity_copy_ref
 *
 * @abstract
 *      Copy a retained reference to the underlying `SecIdentityRef` instance.
 *
 * @param identity
 *      A `sec_identity_t` instance.
 *
 * @return The underlying `SecIdentityRef` instance.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
_Nullable SecIdentityRef
sec_identity_copy_ref(sec_identity_t identity);

/*!
 * @function sec_identity_copy_certificates_ref
 *
 * @abstract
 *      Copy a retained reference to the underlying `CFArrayRef` container of `SecCertificateRef` types.
 *
 * @param identity
 *      A `sec_identity_t` instance.
 *
 * @return The underlying `CFArrayRef` container with `SecCertificateRef` instances.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
_Nullable CFArrayRef
sec_identity_copy_certificates_ref(sec_identity_t identity);

/*!
 * @function sec_certificate_create
 *
 * @abstract
 *      Create an ARC-able `sec_certificate_t` instance from a `SecCertificateRef`.
 *
 * @param certificate
 *      A `SecCertificateRef` instance.
 *
 * @return a `sec_certificate_t` instance.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SEC_RETURNS_RETAINED _Nullable sec_certificate_t
sec_certificate_create(SecCertificateRef certificate);

/*!
 * @function sec_certificate_copy_ref
 *
 * @abstract
 *      Copy a retained reference to the underlying `SecCertificateRef` instance.
 *
 * @param certificate
 *      A `sec_certificate_t` instance.
 *
 * @return The underlying `SecCertificateRef` instance.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SecCertificateRef
sec_certificate_copy_ref(sec_certificate_t certificate);

SEC_ASSUME_NONNULL_END

__END_DECLS

#endif // SecProtocolTypes_h
