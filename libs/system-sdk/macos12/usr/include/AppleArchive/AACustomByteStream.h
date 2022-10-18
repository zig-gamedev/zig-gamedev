// AppleArchive custom byte streams

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

#pragma mark - Callbacks for custom ByteStream implementations

/*!
  @abstract Sequential write proc

  @discussion
  Should behave like write(2), returning the number of written bytes, and a negative value in case of error.
  The internal stream position is incremented by the number of bytes written.

  @param arg stream object
  @param buf provides the bytes to write
  @param nbyte number of bytes to write

  @return number of bytes written on success, and a negative error code on failure
*/
typedef ssize_t (*AAByteStreamWriteProc)(
  void * _Nullable arg,
  const void * buf,
  size_t nbyte) APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Random access write proc

  @discussion
  Should behave like pwrite(2), returning the number of written bytes, and a negative value in case of error.

  @param arg stream object
  @param buf provides the bytes to write
  @param nbyte number of bytes to write
  @param offset write location in stream

  @return number of bytes written on success, and a negative error code on failure
*/
typedef ssize_t (*AAByteStreamPWriteProc)(
  void * _Nullable arg,
  const void * buf,
  size_t nbyte,
  off_t offset) APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Sequential read proc

  @discussion
  Should behave like read(2), returning the number of read bytes, and a negative value in case of error.
  If 0 is returned, we reached the end of stream, and subsequent read calls are expected to return 0.
  The internal stream position is incremented by the number of bytes read.

  @param arg stream object
  @param buf receives the bytes to read
  @param nbyte number of bytes to read

  @return number of bytes read on success, and a negative error code on failure
*/
typedef ssize_t (*AAByteStreamReadProc)(
  void * _Nullable arg,
  void * buf,
  size_t nbyte) APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Random access read proc

  @discussion
  Should behave like pread(2), returning a strictly positive number of read bytes, or 0 if EOF was reached, and a negative value in case of error.

  @param arg stream object
  @param buf receives the bytes to read
  @param nbyte number of bytes to read
  @param offset read location in stream

  @return number of bytes read on success, and a negative error code on failure
*/
typedef ssize_t (*AAByteStreamPReadProc)(
  void * _Nullable arg,
  void * buf,
  size_t nbyte,
  off_t offset) APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Seek proc

  @discussion
  Should behave like lseek(2). Set internal stream position to \p offset, relative to \p whence, one of SEEK_SET, SEEK_CUR, SEEK_END

  @param arg stream object
  @param offset new location relative to origin
  @param whence origin

  @return the new stream position on success, relative to the beginning of the stream, and a negative value on failure
*/
typedef off_t (*AAByteStreamSeekProc)(
  void * _Nullable arg,
  off_t offset,
  int whence) APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Cancel proc

  @discussion Subsequent read/write/seek calls to the stream are expected to fail. The stream still needs to be closed.

  @param arg stream object
*/
typedef void (*AAByteStreamCancelProc)(
  void * _Nullable arg) APPLE_ARCHIVE_SWIFT_PRIVATE;

/*!
  @abstract Close and destroy the stream, releasing all resources. Do nothing if \p arg is 0.

  @discussion
  Close can fail, for example when there are still buffered bytes in the stream, and an error
  occurred when trying to flush them.
  After \p close is called, subsequent operations on the stream are expected to fail.

  @param arg stream object

  @return 0 on success, a negative value on failure
*/
typedef int (*AAByteStreamCloseProc)(
  void * _Nullable arg) APPLE_ARCHIVE_SWIFT_PRIVATE;

/**
@abstract Create a new custom byte stream

@discussion
All callbacks are initially empty
 */
APPLE_ARCHIVE_API AAByteStream _Nullable AACustomByteStreamOpen(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream data

@discussion
This value is passed as first argument to all callbacks

@param s target object
@param data stream data
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetData(AAByteStream s, void * _Nullable data)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream close callback

@discussion
If the stream data is non NULL, a non NULL close callback is mandatory

@param s target object
@param proc callback
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetCloseProc(AAByteStream s, AAByteStreamCloseProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream read callback

@param s target object
@param proc callback
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetReadProc(AAByteStream s, AAByteStreamReadProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream pread callback

@param s target object
@param proc callback
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetPReadProc(AAByteStream s, AAByteStreamPReadProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream write callback

@param s target object
@param proc callback
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetWriteProc(AAByteStream s, AAByteStreamWriteProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream pwrite callback

@param s target object
@param proc callback
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetPWriteProc(AAByteStream s, AAByteStreamPWriteProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream seek callback

@param s target object
@param proc callback
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetSeekProc(AAByteStream s, AAByteStreamSeekProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
@abstract Set custom byte stream cancel callback

@param s target object
@param proc callback
 */
APPLE_ARCHIVE_API void AACustomByteStreamSetCancelProc(AAByteStream s, AAByteStreamCancelProc _Nullable proc)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif // __cplusplus

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
