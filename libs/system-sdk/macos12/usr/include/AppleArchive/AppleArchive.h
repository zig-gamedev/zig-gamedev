// AppleArchive main header

#ifndef __APPLE_ARCHIVE_H
#define __APPLE_ARCHIVE_H

// No other headers are included in the AA API headers
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/acl.h>

// All the API headers will use these macros

// Availability
#if __has_include( <Availability.h> ) && !__BUILDING_APPLE_ARCHIVE
#include <Availability.h>
#define APPLE_ARCHIVE_AVAILABLE __API_AVAILABLE
#else
#define APPLE_ARCHIVE_AVAILABLE(...)
#endif

// Swift availability
#if __has_attribute(swift_private)
#define APPLE_ARCHIVE_SWIFT_PRIVATE __attribute__((swift_private))
#else
#define APPLE_ARCHIVE_SWIFT_PRIVATE
#endif

// API functions
#define APPLE_ARCHIVE_API __attribute__((visibility("default"))) APPLE_ARCHIVE_SWIFT_PRIVATE
#define APPLE_ARCHIVE_INLINE static inline __attribute__((always_inline,overloadable)) APPLE_ARCHIVE_SWIFT_PRIVATE

// Fixed size enums
#if __has_extension(cxx_fixed_enum)
#define APPLE_ARCHIVE_ENUM(name,type) enum name : type
#else
#define APPLE_ARCHIVE_ENUM(name,type) enum name
#endif

#include "AADefs.h"
#include "AAEntryMessage.h"
#include "AAFlagSet.h"
#include "AAByteStream.h"
#include "AACustomByteStream.h"
#include "AAFieldKeys.h"
#include "AAHeader.h"
#include "AAEntryAttributes.h"
#include "AAEntryACLBlob.h"
#include "AAEntryXATBlob.h"
#include "AAPathList.h"
#include "AAArchiveStream.h"
#include "AACustomArchiveStream.h"

#include "AppleEncryptedArchive.h"

// Deprecated calls

#ifdef __cplusplus
extern "C" {
#endif

APPLE_ARCHIVE_INLINE void AAByteStreamAbort(AAByteStream s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
__attribute__((availability(macos,deprecated=11.0, replacement="AAByteStreamCancel")))
{ AAByteStreamCancel(s); }

APPLE_ARCHIVE_INLINE void AACustomByteStreamSetAbortProc(AAByteStream s, AAByteStreamCancelProc proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
__attribute__((availability(macos,deprecated=11.0, replacement="AACustomByteStreamSetCancelProc")))
{ AACustomByteStreamSetCancelProc(s, proc); }

APPLE_ARCHIVE_INLINE void AAArchiveStreamAbort(AAArchiveStream s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
__attribute__((availability(macos,deprecated=11.0, replacement="AAArchiveStreamCancel")))
{ AAArchiveStreamCancel(s); }

APPLE_ARCHIVE_INLINE void AACustomArchiveStreamSetAbortProc(AAArchiveStream s, AAArchiveStreamCancelProc proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0))
__attribute__((availability(macos,deprecated=11.0, replacement="AACustomArchiveStreamSetCancelProc")))
{ AACustomArchiveStreamSetCancelProc(s, proc); }

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef __APPLE_ARCHIVE_H
