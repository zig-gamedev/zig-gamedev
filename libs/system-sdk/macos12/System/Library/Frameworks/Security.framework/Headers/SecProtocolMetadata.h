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

#ifndef SecProtocolMetadata_h
#define SecProtocolMetadata_h

#include <Security/SecProtocolObject.h>
#include <Security/SecProtocolTypes.h>
#include <Security/SecProtocolOptions.h>

#include <dispatch/dispatch.h>
#include <os/object.h>

/*!
 * The following diagram shows how clients interact with sec_protocol_options
 * and sec_protocol_metadata when configuring and using network security protocols.
 *
 *                    +--------+
 *                    | Client |
 *                    +-+---/ \+
 *                      |    |
 *        +-------------+    +-------------+
 *        | (1) set             (2) get    |
 *        | options             metadata   |
 * +-----\ /---------------+  +------------+----------+
 * | sec_protocol_options  |  | sec_protocol_metadata |
 * +-----------------------+  +-----------------------+
 *
 * Clients configure security protocols with `sec_protocol_options` instances.
 * And they inspect protocol instances using `sec_protocol_metadata` instances.
 */

#ifndef SEC_OBJECT_IMPL
/*!
 * A `sec_protocol_metadata` instance conatins read-only properties of a connected and configured
 * security protocol. Clients use this object to read information about a protocol instance. Properties
 * include, for example, the negotiated TLS version, ciphersuite, and peer certificates.
 */
SEC_OBJECT_DECL(sec_protocol_metadata);
#endif // !SEC_OBJECT_IMPL

__BEGIN_DECLS

SEC_ASSUME_NONNULL_BEGIN

/*!
 * @function sec_protocol_metadata_get_negotiated_protocol
 *
 * @abstract
 *      Get the application protocol negotiated, e.g., via the TLS ALPN extension.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return A NULL-terminated string carrying the negotiated protocol.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
const char * _Nullable
sec_protocol_metadata_get_negotiated_protocol(sec_protocol_metadata_t metadata);

/*!
 * @function sec_protocol_metadata_copy_peer_public_key
 *
 * @abstract
 *      Get the protocol instance peer's public key.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return A `dispatch_data_t` containing the peer's raw public key.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SEC_RETURNS_RETAINED _Nullable dispatch_data_t
sec_protocol_metadata_copy_peer_public_key(sec_protocol_metadata_t metadata);

/*!
 * @function sec_protocol_metadata_get_negotiated_tls_protocol_version
 *
 * @abstract
 *      Get the negotiated TLS version.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return A `tls_protocol_version_t` value.
 */
API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
tls_protocol_version_t
sec_protocol_metadata_get_negotiated_tls_protocol_version(sec_protocol_metadata_t metadata);

/*!
 * @function sec_protocol_metadata_get_negotiated_protocol_version
 *
 * @abstract
 *      Get the negotiated TLS version.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return A SSLProtocol enum of the TLS version.
 */
API_DEPRECATED_WITH_REPLACEMENT("sec_protocol_metadata_get_negotiated_tls_protocol_version",
                                macos(10.14, 10.15), ios(12.0, 13.0), watchos(5.0, 6.0), tvos(12.0, 13.0))
SSLProtocol
sec_protocol_metadata_get_negotiated_protocol_version(sec_protocol_metadata_t metadata);

/*!
 * @function sec_protocol_metadata_get_negotiated_tls_ciphersuite
 *
 * @abstract
 *      Get the negotiated TLS ciphersuite.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return A `tls_ciphersuite_t`.
 */
API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
tls_ciphersuite_t
sec_protocol_metadata_get_negotiated_tls_ciphersuite(sec_protocol_metadata_t metadata);

/*!
 * @function sec_protocol_metadata_get_negotiated_ciphersuite
 *
 * @abstract
 *      Get the negotiated TLS ciphersuite.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return A SSLCipherSuite.
 */
API_DEPRECATED_WITH_REPLACEMENT("sec_protocol_metadata_get_negotiated_tls_ciphersuite",
                                macos(10.14, 10.15), ios(12.0, 13.0), watchos(5.0, 6.0), tvos(12.0, 13.0), macCatalyst(13.0, 13.0))
SSLCipherSuite
sec_protocol_metadata_get_negotiated_ciphersuite(sec_protocol_metadata_t metadata);

/*!
 * @function sec_protocol_metadata_get_early_data_accepted
 *
 * @abstract
 *      Determine if early data was accepted by the peer.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return A bool indicating if early data was accepted.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
bool
sec_protocol_metadata_get_early_data_accepted(sec_protocol_metadata_t metadata);

#ifdef __BLOCKS__
/*!
 * @function sec_protocol_metadata_access_peer_certificate_chain
 *
 * @abstract
 *      Get the certificate chain of the protocol instance peer.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param handler
 *      A block to invoke one or more times with sec_certificate_t objects
 *
 * @return Returns true if the peer certificates were accessible, false otherwise.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
bool
sec_protocol_metadata_access_peer_certificate_chain(sec_protocol_metadata_t metadata,
                                                    void (^handler)(sec_certificate_t certificate));

/*!
 * @function sec_protocol_metadata_copy_ocsp_response
 *
 * @abstract
 *      Get the OCSP response from the protocol instance peer.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param handler
 *      A block to invoke one or more times with OCSP data
 *
 * @return Returns true if the OSCP response was accessible, false otherwise.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
bool
sec_protocol_metadata_access_ocsp_response(sec_protocol_metadata_t metadata,
                                           void (^handler)(dispatch_data_t ocsp_data));

/*!
 * @function sec_protocol_metadata_access_supported_signature_algorithms
 *
 * @abstract
 *      Get the signature algorithms supported by the peer. Clients may call this
 *      in response to a challenge block.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param handler
 *      A block to invoke one or more times with OCSP data
 *
 * @return Returns true if the supported signature list was accessible, false otherwise.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
bool
sec_protocol_metadata_access_supported_signature_algorithms(sec_protocol_metadata_t metadata,
                                                            void (^handler)(uint16_t signature_algorithm));

/*!
 * @function sec_protocol_metadata_access_distinguished_names
 *
 * @abstract
 *      Get the X.509 Distinguished Names from the protocol instance peer.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param handler
 *      A block to invoke one or more times with distinguished_name data
 *
 * @return Returns true if the distinguished names were accessible, false otherwise.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
bool
sec_protocol_metadata_access_distinguished_names(sec_protocol_metadata_t metadata,
                                                 void (^handler)(dispatch_data_t distinguished_name));

/*!
 * @function sec_protocol_metadata_access_pre_shared_keys
 *
 * @abstract
 *      Get the PSKs supported by the local instance.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param handler
 *      A block to invoke one or more times with tuples of dispatch_data_t objects carrying PSKs and their corresponding identities.
 *
 * @return Returns true if the PSKs were accessible, false otherwise.
 */
API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
bool
sec_protocol_metadata_access_pre_shared_keys(sec_protocol_metadata_t metadata, void (^handler)(dispatch_data_t psk, dispatch_data_t psk_identity));

#endif // __BLOCKS__

/*!
 * @function sec_protocol_metadata_get_server_name
 *
 * @abstract
 *      Obtain the server name offered by a client or server during
 *      connection establishmet. This is the value commonly carried
 *      in the TLS SNI extesion.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return Returns A NULL-terminated string carrying the server name, or NULL
 *      if none was provided.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
const char * _Nullable
sec_protocol_metadata_get_server_name(sec_protocol_metadata_t metadata);

/*!
 * @function sec_protocol_metadata_peers_are_equal
 *
 * @abstract
 *      Compare peer information for two `sec_protocol_metadata` instances.
 *      This comparison does not include protocol configuration options, e.g., ciphersuites.
 *
 * @param metadataA
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param metadataB
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return Returns true if both metadata values refer to the same peer, and false otherwise.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
bool
sec_protocol_metadata_peers_are_equal(sec_protocol_metadata_t metadataA, sec_protocol_metadata_t metadataB);

/*!
 * @function sec_protocol_metadata_challenge_parameters_are_equal
 *
 * @abstract
 *      Compare challenge-relevant information for two `sec_protocol_metadata` instances.
 *
 *      This comparison includes all information relevant to a challenge request, including:
 *      distinguished names, signature algorithms, and supported certificate types.
 *      See Section 7.4.4 of RFC5246 for more details.
 *
 * @param metadataA
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param metadataB
 *      A `sec_protocol_metadata_t` instance.
 *
 * @return Returns true if both metadata values have the same challenge parameters.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
bool
sec_protocol_metadata_challenge_parameters_are_equal(sec_protocol_metadata_t metadataA, sec_protocol_metadata_t metadataB);

/*!
 * @function sec_protocol_metadata_create_secret
 *
 * @abstract
 *      Export a secret, e.g., a cryptographic key, derived from the protocol metadata using a label string.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param label_len
 *      Length of the KDF label string.
 *
 * @param label
 *      KDF label string.
 *
 * @param exporter_length
 *      Length of the secret to be exported.
 *
 * @return Returns a dispatch_data_t object carrying the exported secret.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SEC_RETURNS_RETAINED _Nullable dispatch_data_t
sec_protocol_metadata_create_secret(sec_protocol_metadata_t metadata, size_t label_len,
                                    const char *label, size_t exporter_length);

/*!
 * @function sec_protocol_metadata_create_secret_with_context
 *
 * @abstract
 *      Export a secret, e.g., a cryptographic key, derived from the protocol metadata using a label and context string.
 *
 * @param metadata
 *      A `sec_protocol_metadata_t` instance.
 *
 * @param label_len
 *      Length of the KDF label string.
 *
 * @param label
 *      KDF label string.
 *
 * @param context_len
 *      Length of the KDF context string.
 *
 * @param context
 *      Constant opaque context value
 *
 * @param exporter_length
 *      Length of the secret to be exported.
 *
 * @return Returns a dispatch_data_t object carrying the exported secret.
 */
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
SEC_RETURNS_RETAINED _Nullable dispatch_data_t
sec_protocol_metadata_create_secret_with_context(sec_protocol_metadata_t metadata, size_t label_len,
                                                 const char *label, size_t context_len,
                                                 const uint8_t *context, size_t exporter_length);

SEC_ASSUME_NONNULL_END

__END_DECLS

#endif // SecProtocolMetadata_h
