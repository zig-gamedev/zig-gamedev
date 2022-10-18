// AppleArchive byte streams

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

#pragma mark - Stream functions

/*!
  @abstract Sequential write

  @param s ByteStream
  @param buf provides the bytes to write
  @param nbyte number of bytes to write

  @return number of bytes written on success, and a negative error code on failure or if \p write is not implemented
*/
APPLE_ARCHIVE_API ssize_t AAByteStreamWrite(
  AAByteStream s,
  const void * buf,
  size_t nbyte)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Random-access write

  @param s ByteStream
  @param buf provides the bytes to write
  @param nbyte number of bytes to write
  @param offset write location in stream

  @return number of bytes written on success, and a negative error code on failure or if \p pwrite is not implemented
*/
APPLE_ARCHIVE_API ssize_t AAByteStreamPWrite(
  AAByteStream s,
  const void * buf,
  size_t nbyte,
  off_t offset)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Sequential read

  @param s ByteStream
  @param buf receives the bytes to read
  @param nbyte number of bytes to read

  @return number of bytes read on success, and a negative error code on failure or if \p read is not implemented
*/
APPLE_ARCHIVE_API ssize_t AAByteStreamRead(
  AAByteStream s,
  void * buf,
  size_t nbyte)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Random-access read

  @param s ByteStream
  @param buf receives the bytes to read
  @param nbyte number of bytes to read
  @param offset read location in stream

  @return number of bytes read on success, and a negative error code on failure or if \p pread is not implemented
*/
APPLE_ARCHIVE_API ssize_t AAByteStreamPRead(
  AAByteStream s,
  void * buf,
  size_t nbyte,
  off_t offset)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Seek

  @discussion
  Set internal stream position to \p offset, relative to \p whence, one of SEEK_SET, SEEK_CUR, SEEK_END

  @param s ByteStream
  @param offset new location relative to origin
  @param whence origin

  @return the new stream position on success, relative to the beginning of the stream, and a negative value on failure
*/
APPLE_ARCHIVE_API off_t AAByteStreamSeek(
  AAByteStream s,
  off_t offset,
  int whence)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Cancel, the stream still needs to be closed

  @discussion Asynchronous cancellation. Subsequent calls to the stream are expected to fail.

  @param s ByteStream
*/
APPLE_ARCHIVE_API void AAByteStreamCancel(
  AAByteStream s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Close stream

  @discussion Destroy the stream and release all resources.  The stream handle becomes invalid after this call.

  @param s ByteStream, ignored if NULL

  @return 0 on success, a negative value on failure
*/
APPLE_ARCHIVE_API int AAByteStreamClose(
  AAByteStream _Nullable s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Stream objects

/*!
  @abstract Create file stream with an open file descriptor

  @discussion
  All calls are directly mapped to the read, write, etc. system calls.

  @param fd is the opened file descriptor
  @param automatic_close if not 0, we'll close(fd) when the stream is closed

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AAFileStreamOpenWithFD(
  int fd,
  int automatic_close)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Open a new file descriptor and create file stream

  @discussion
  The file is opened with open(path, open_flags, open_mode).
  All calls are directly mapped to the read, write, etc. system calls.
  We will call close(fd) when the stream is destroyed.

  @param path is the file to open
  @param open_flags are the flags passed to open(2)
  @param open_mode is the creation mode passed to open(2)

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AAFileStreamOpenWithPath(
  const char * path,
  int open_flags,
  mode_t open_mode)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a block compression (pbz*) sequential output stream

  @discussion This stream is write-only, and supports only sequential writes.
  pwrite, read, pread, seek are not implemented.
  \p compressed_stream must implement \p write.

  @param compressed_stream is the output stream receiving the compressed data
  @param compression_algorithm is the compression algorithm to use
  @param block_size is the compression block size (bytes)
  @param flags stream flags
  @param n_threads is the number of worker threads, 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AACompressionOutputStreamOpen(
  AAByteStream compressed_stream,
  AACompressionAlgorithm compression_algorithm,
  size_t block_size,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a block compression (pbz*) sequential output stream appending data to an
  existing block compression stream

  @discussion This stream is write-only, and supports only sequential writes.
  pwrite, read, pread, seek are not implemented. Data is appended to the existing stream.
  If smaller than block size, the last block in the existing stream will be modified to add incoming data.
  \p compressed_stream must implement \p pread, \p seek, and \p write.
  The returned stream MUST be closed before \p compressed_stream

  @param compressed_stream is the output stream providing/receiving the compressed data
  @param flags stream flags
  @param n_threads is the number of worker threads, 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AACompressionOutputStreamOpenExisting(
  AAByteStream compressed_stream,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a decompression sequential input stream

  @description
  A few bytes of the input will be read by this function, to identify the compression used.
  It recognizes block compression (pbz*), LZFSE and LZ4 frames as generated by the
  Compression library, zlib, gzip, bzip2, and xz.
  If none of these compression formats can be identified, a copy stream is created, directly relaying the input data.
  This stream is read-only, and supports only sequential reads.
  pread, write, pwrite, seek are not implemented.
  The returned stream MUST be closed before \p compressed_stream

  @param compressed_stream is the input stream providing the compressed data, only read is called
  @param flags stream flags
  @param n_threads is the number of worker threads (used only for block compression), 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AADecompressionInputStreamOpen(
  AAByteStream compressed_stream,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a decompression random access input stream

  @description
  The input stream MUST use the block compression format (pbz*).
  The size specified in \p alloc_limit is the requested limit for memory allocation. Depending on the
  block size and compressor algorithm, we may have to allocate more memory in some cases.  Increasing this value
  allows more blocks to be cached, and increases performance.  Set it to 0 to request minimal allocation, and to
  SIZE_MAX to request best performance.
  This stream is read-only and supports random access reads.
  write and pwrite are not implemented, but read, pread and seek are.
  The returned stream MUST be closed before \p compressed_stream

  @param compressed_stream is the input stream providing the compressed data, only pread and seek are called
  @param alloc_limit is the requested max memory allocation size
  @param flags stream flags
  @param n_threads is the number of worker threads (used only for block compression), 0 for default

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AADecompressionRandomAccessInputStreamOpen(
  AAByteStream compressed_stream,
  size_t alloc_limit,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Open a new temporary file descriptor and create a file stream

  @discussion
  All calls are directly mapped to the read, write, etc. system calls.
  AAByteStreamClose will remove the file.

  @return a new stream instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAByteStream _Nullable AATempFileStreamOpen(void)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a shared buffer and associated pair of stream streams

  @discussion
  This shared buffer allows data buffering between two threads. The writer thread writes to the buffer
  through \p ostream and blocks when full, and the reader thread reads from the buffer through \p istream
  and blocks when empty.
  If \p ostream receives a write call with nbyte=0, it is considered EOF, and after all data is
  read, further reads on \p istream will return 0.

  @param ostream is initialized with a new sequential write stream, writing data to the buffer
  @param istream is initialized with a new sequential read stream, reading data from the buffer
  @param buffer_capacity is the size to allocate for the buffer (bytes)

  @return 0 and set both streams to new instances on success, return a negative error code and set both streams to NULL on failure
*/
APPLE_ARCHIVE_API int AASharedBufferPipeOpen(
  AAByteStream _Nonnull * _Nullable ostream,
  AAByteStream _Nonnull * _Nullable istream,
  size_t buffer_capacity)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Process an byte stream

/**
  @abstract Process all data of a stream

  @discussion Read all bytes from \p istream, and write them to \p ostream.

  @param istream input byte stream
  @param ostream output byte stream

  @return number of bytes processed on success, and a negative error code on failure
 */
APPLE_ARCHIVE_API off_t AAByteStreamProcess(
  AAByteStream istream,
  AAByteStream ostream)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Process a random access stream

  @discussion Perform random access reads from \p istream with buffers of \p block_size bytes.
  If \p AA_FLAG_PROCESS_RANDOM_ACCESS_OUTPUT is set, write the data using \p pwrite to the output stream.
  Otherwise, write the blocks in order using \p write to the output stream.

  @param istream input byte stream, must implement \p pread
  @param ostream output byte stream
  @param max_offset max number of bytes to process, or -1 to process to EOF
  @param block_size is the buffer size for each \p pread call
  @param flags operation flags
  @param n_threads number of reader threads

  @return number of bytes processed on success, and a negative error code on failure
 */
APPLE_ARCHIVE_API off_t AARandomAccessByteStreamProcess(
  AAByteStream istream,
  AAByteStream ostream,
  off_t max_offset,
  size_t block_size,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

#ifdef __cplusplus
}
#endif // __cplusplus

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
