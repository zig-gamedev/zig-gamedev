/*
 * Copyright (c) 2021 Apple Inc.
 * All rights reserved.
 */

#ifndef __NE_SOCKET_H__
#define __NE_SOCKET_H__

#include <Availability.h>
#include <os/availability.h>
#include <stdbool.h>

/*!
 * @typedef ne_socket_attribution_t
 * @discussion Attribution values can be used to indicate who determined what network resources should be used.
 */
typedef enum {
	/*! @const NE_SOCKET_ATTRIBUTION_DEVELOPER The developer chose */
	NE_SOCKET_ATTRIBUTION_DEVELOPER = 1,
	/*! @const NE_SOCKET_ATTRIBUTION_USER The user chose */
	NE_SOCKET_ATTRIBUTION_USER = 2,
} ne_socket_attribution_t
API_AVAILABLE(ios(15.0), macos(12.0), tvos(15.0), watchos(8.0));

/*!
 @function ne_socket_set_domains
 @discussion Associate a domain name (as well as any resolved CNAME if present) with a socket.
 @param socket The file descriptor of a socket.
 @param domain_names An array consisting of the original domain name by itself, or optionally, followed by a list of resolved CNAME(s) if present.  Each domain name is
 a null-terminated string and must consist of '.' separated labels formed from the set of Alphanumeric characters and hyphens (but not commence or end with a hyphen).
 Each domain name must comply to the maximum length of 253 ASCII characters, and each label must comply to the maximum length of 63 ASCII characters.  The labels are
 case-insensitive.  This API only supports domains consisting of the printable ASCII character range.  Domains with non-printable ASCII characters will be treated as invalid by
 this API, thus callers should punycode any such domain before calling this API.
 @param domain_count Number of domains in the domain_names array.
 @return returns 0 upon success or non-zero integer error code defined in errno.h upon failure.
 */
int ne_socket_set_domains(int socket, const char * const * const domain_names, int domain_count)
API_AVAILABLE(ios(15.0), macos(12.0), tvos(15.0), watchos(8.0));

/*!
 @function ne_socket_set_attribution
 @discussion  In order to help differentiate communication with domains requested by the user from those requested by the developer, attribution may be used. This value
 defaults to NE_SOCKET_ATTRIBUTION_DEVELOPER. Setting this to NE_SOCKET_ATTRIBUTION_USER indicates that the networking performed using these parameters is
 based on content specified by the user, not the developer.
 @param socket The file descriptor of a socket.
 @param attribution The entity to attribute the network operations to.
 @return returns 0 upon success or non-zero integer error code defined in errno.h upon failure.
 */
int ne_socket_set_attribution(int socket, ne_socket_attribution_t attribution)
API_AVAILABLE(ios(15.0), macos(12.0), tvos(15.0), watchos(8.0));

/*!
 @function ne_socket_set_website_attribution
 @discussion  In order to help differentiate communication with domains requested due to a user chosen website, website attribution may be used.  This function sets the
 attribution of the socket to NE_SOCKET_ATTRIBUTION_USER and indicates that the socket is used to load some part of the website at the specified name.  Calling this API
 indicates that the networking performed using these parameters is based on content specified by the user, not the developer.
 @param socket The file descriptor of a socket.
 @param hostname The website hostname to attribute the network operations to.
 @return returns 0 upon success or non-zero integer error code defined in errno.h upon failure.
 */
int ne_socket_set_website_attribution(int socket, const char *hostname)
API_AVAILABLE(ios(15.0), macos(12.0), tvos(15.0), watchos(8.0));

/*!
 @function ne_socket_set_is_app_initiated
 @discussion <This is for backward compatibility - to be obsoleted.  Please adopt ne_socket_set_attribution() instead.>
 @param socket The file descriptor of a socket.
 @param is_app_initiated Set this boolean to false to indicate that the networking performed using these parameters is based on content not requested by the application.
 This value defaults to true.
 @return returns 0 upon success or non-zero integer error code defined in errno.h upon failure.
 */
int ne_socket_set_is_app_initiated(int socket, bool is_app_initiated)
API_UNAVAILABLE(ios, macos, tvos, watchos);

#endif /* __NE_SOCKET_H__ */
