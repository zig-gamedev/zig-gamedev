/*
 * xcselect.h
 *
 * xcode-select public API
 *
 * Copyright 2019 Apple Inc. All rights reserved.
 *
 */

#ifndef __XCSELECT_H__
#define __XCSELECT_H__

#include <os/availability.h>
#include <os/base.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/_types/_errno_t.h>
#include <CoreFoundation/CFBase.h>

#if __has_feature(assume_nonnull)
#define XCSELECT_ASSUME_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#define XCSELECT_ASSUME_NONNULL_END   _Pragma("clang assume_nonnull end")
#else
#define XCSELECT_ASSUME_NONNULL_BEGIN
#define XCSELECT_ASSUME_NONNULL_END
#endif

#define XCSELECT_BEGIN_DECLS \
    __BEGIN_DECLS \
    XCSELECT_ASSUME_NONNULL_BEGIN

#define XCSELECT_END_DECLS \
    XCSELECT_ASSUME_NONNULL_END \
    __END_DECLS

/**
 * \brief describe preferred policy for locating an SDK appropriate for building host content.
 */
typedef CF_ENUM(uint32_t, xcselect_host_sdk_policy_t) {
    /** Return an SDK that matches the host's major OS version if it is available, otherwise return the latest macOS SDK available. */
    XCSELECT_HOST_SDK_POLICY_MATCHING_PREFERRED CF_SWIFT_NAME(matchingPreferred) = 1,

    /** Return an SDK that matches the host's major OS version. */
    XCSELECT_HOST_SDK_POLICY_MATCHING_ONLY CF_SWIFT_NAME(matchingOnly),

    /** Return the latest macOS SDK available. */
    XCSELECT_HOST_SDK_POLICY_LATEST CF_SWIFT_NAME(latest),
} CF_SWIFT_NAME(HostSDKPolicy);

XCSELECT_BEGIN_DECLS

/**
 * \brief Get the path to an SDK appropriate for building content for the current host.
 *
 * \param sdk_policy The policy to use to determine which SDK to return.
 *
 * \param path Upon success, this handle is adjusted to point to heap allocated
 *             memory containing the appropriate SDK path as a C string. The
 *             caller is responsible for calling free(3) on this string when it
 *             is no longer needed.  On failure, it is not changed.
 *
 * \returns 0 on success, or an errno_t on failure, eg:
 *          EINVAL - Invalid input (invalid sdk_policy or NULL path)
 *          ENOENT - Unable to find an SDK per the requested policy.
 */
OS_EXPORT
OS_SWIFT_UNAVAILABLE("Unavailable in Swift")
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
errno_t xcselect_host_sdk_path(xcselect_host_sdk_policy_t sdk_policy,
                               char * __nullable * __nonnull path);

XCSELECT_END_DECLS

/* TODO: Remove this once dependents have either transitioned to public API or
 *       moved to including xcselect_private.h directly.
 */
#ifdef __has_include
#if __has_include(<xcselect_private.h>)
#include <xcselect_private.h>
#endif
#endif

#endif /* __XCSELECT_H__ */
