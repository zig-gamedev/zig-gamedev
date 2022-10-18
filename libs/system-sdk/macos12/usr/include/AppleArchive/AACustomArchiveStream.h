// AppleArchive custom archive output streams

#pragma once

#ifndef __APPLE_ARCHIVE_H
#error Include AppleArchive.h instead of this file
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull begin")
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#pragma mark - Callbacks for custom ArchiveStream implementations

typedef int (*AAArchiveStreamWriteHeaderProc)(
  void * _Nullable arg,
  AAHeader header) APPLE_ARCHIVE_SWIFT_PRIVATE;

typedef int (*AAArchiveStreamWriteBlobProc)(
  void * _Nullable arg,
  AAFieldKey key,
  const void * buf,
  size_t nbyte) APPLE_ARCHIVE_SWIFT_PRIVATE;

typedef int (*AAArchiveStreamReadHeaderProc)(
  void * _Nullable arg,
  AAHeader _Nullable * _Nonnull header) APPLE_ARCHIVE_SWIFT_PRIVATE;

typedef int (*AAArchiveStreamReadBlobProc)(
  void * _Nullable arg,
  AAFieldKey key,
  void * buf,
  size_t nbyte) APPLE_ARCHIVE_SWIFT_PRIVATE;

typedef void (*AAArchiveStreamCancelProc)(
  void * _Nullable arg) APPLE_ARCHIVE_SWIFT_PRIVATE;

typedef int (*AAArchiveStreamCloseProc)(
  void * _Nullable arg) APPLE_ARCHIVE_SWIFT_PRIVATE;

/**
  @abstract Create a new custom archive output stream

  @discussion
  All callbacks are initially empty
 */
APPLE_ARCHIVE_API AAArchiveStream _Nullable AACustomArchiveStreamOpen(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set custom archive stream data

  @discussion
  This value is passed as first argument to all callbacks

  @param s target object
  @param data stream data
 */
APPLE_ARCHIVE_API void AACustomArchiveStreamSetData(AAArchiveStream s, void * _Nullable data)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set custom archive stream close callback

  @discussion
  If the stream data is non NULL, a non NULL destructor callback is mandatory

  @param s target object
  @param proc callback
 */
APPLE_ARCHIVE_API void AACustomArchiveStreamSetCloseProc(AAArchiveStream s, AAArchiveStreamCloseProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set custom archive stream cancel callback

  @param s target object
  @param proc callback
 */
APPLE_ARCHIVE_API void AACustomArchiveStreamSetCancelProc(AAArchiveStream s, AAArchiveStreamCancelProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set custom archive stream write header callback

  @param s target object
  @param proc callback
 */
APPLE_ARCHIVE_API void AACustomArchiveStreamSetWriteHeaderProc(AAArchiveStream s, AAArchiveStreamWriteHeaderProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set custom archive stream write blob callback

  @param s target object
  @param proc callback
 */
APPLE_ARCHIVE_API void AACustomArchiveStreamSetWriteBlobProc(AAArchiveStream s, AAArchiveStreamWriteBlobProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set custom archive stream read header callback

  @param s target object
  @param proc callback
 */
APPLE_ARCHIVE_API void AACustomArchiveStreamSetReadHeaderProc(AAArchiveStream s, AAArchiveStreamReadHeaderProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Set custom archive stream read blob callback

  @param s target object
  @param proc callback
 */
APPLE_ARCHIVE_API void AACustomArchiveStreamSetReadBlobProc(AAArchiveStream s, AAArchiveStreamReadBlobProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif // __cplusplus

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
