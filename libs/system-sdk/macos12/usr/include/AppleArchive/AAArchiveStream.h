// AppleArchive archive input/output streams
//
// An archive output stream receives archive elements through the WriteHeader and WriteBlob functions.
// An archive input stream provides archive elements through the ReadHeader and ReadBlob functions.
//
// The library provides common implementations of archive output streams, to:
// - encode archive contents to a byte stream,
// - extract archive contents to a directory,
// - convert to another archive with different fields.
//
// And the library provides the following input stream:
// - decode archive contents from a byte stream
//
// Where other operations on the archive elements are needed, the API allows to create a
// custom archive stream, where all the processing functions can be redefined.

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

#pragma mark - Write functions (output stream)

/**
  @abstract Begin a new entry

  @discussion All data blobs for the previous entry must have been written before this call.

  @param s target stream
  @param header is the header for the new entry

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAArchiveStreamWriteHeader(AAArchiveStream s, AAHeader header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abatract Write data for current blob

  @discussion Several write calls are allowed for the same blob. The total number of written bytes
  for a given blob MUST match the blob size stored in the header. Data for all blobs defined in the
  current header must be written.

  @param s target stream
  @param key is the blob key, MUST correspond to blob keys in the entry header, in the same order
  @param buf data buffer
  @param nbyte number of bytes in \p buf

  @return 0 on success, a negative error code on failure
*/
APPLE_ARCHIVE_API int AAArchiveStreamWriteBlob(AAArchiveStream s, AAFieldKey key, const void * buf, size_t nbyte)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Read functions (input stream)

/**
  @abstract Get next archive entry header

  @discussion If \p *header is initially NULL and there is an entry to return, a new \p AAHeader instance
  is returned, the caller gains ownership of the object, and should destroy it after use.
  \p *header is allowed to be non NULL when this function is called. In that case, the contents
  of this existing object will be replaced with the new header contents.

  @param s the target stream
  @param header receives the next entry AAHeader object if an entry is found, and is left unchanged otherwise

  @return 1 and set/update the parameters if an entry is found, 0 if we reached the end of the archive, and a negative error code on failure
 */
APPLE_ARCHIVE_API int AAArchiveStreamReadHeader(
  AAArchiveStream s,
  AAHeader _Nullable * _Nonnull header)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Read archive entry blob data

  @discussion This function reads data stored in the first unread blob matching \p key.  Blob data can only be
  read in the order they are stored in the header.  Reading more data than the blob size stored in the header
  is an error.

  @param s the target stream
  @param key field key of blob to read
  @param buf receives blob data
  @param nbyte the number of bytes to read

  @return 0 on success, and a negative error code on failure
 */
APPLE_ARCHIVE_API int AAArchiveStreamReadBlob(
  AAArchiveStream s,
  AAFieldKey key,
  void * buf,
  size_t nbyte)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Common functions

/**
  @abstract Cancel stream

  @discussion After this call, other stream operations are expected to fail

  @param s target stream
*/
APPLE_ARCHIVE_API void AAArchiveStreamCancel(AAArchiveStream s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Close an archive output stream

  @param s target stream, do nothing if NULL
*/
APPLE_ARCHIVE_API int AAArchiveStreamClose(AAArchiveStream _Nullable s)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - High level write

/*!
  @abstract Write the fields defined in \p key_set for all filesystem objects dir.'/'.path where \p path in \p path_list

  @discussion
  In addition to the fields accepted by AAHeaderInitWithPath, the following fields are accepted:

    XAT,ACL,DAT,HLC,CLC,SLC

  Hard link (HLC) and clone (CLC) information relating files in \p path_list is obtained from the filesystem, and stored in the archive.
  If same data (SLC) information is required, a hash will be computed for each file, and then contents compared. When extracting
  entries with a SLC field, the auto-clone feature can be used, creating filesystem clones (when supported) instead of copies.

  The entry message callback for the encoder will receive a AA_ENTRY_MESSAGE_SCANNING for each entry (in a non predictable order)
  when first scanned and possibly hashed. Then it will receive a AA_ENTRY_MESSAGE_ENCODING for each entry (in archive order)
  when the AppleArchive elements are be written for the entry. If the client returns a negative value to any of these callbacks, the
  operation will be aborted.

  @param s target stream
  @param path_list path list providing all the paths of the entries to write
  @param key_set set of keys to include for each entry
  @param dir 0-terminated directory path, not stored in the PAT field of the archive entry
  @param msg_data is passed as first argument to \p msg_proc
  @param msg_proc is called with queries to the caller if not NULL
  @param flags convert flags
  @param n_threads is the number of worker threads, or 0 for default

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAArchiveStreamWritePathList(
  AAArchiveStream s,
  AAPathList path_list,
  AAFieldKeySet key_set,
  const char * dir,
  void * _Nullable msg_data,
  AAEntryMessageProc _Nullable msg_proc,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Output archive stream instances

/*!
  @abstract Open an extraction archive output stream

  @discussion
  For each entry received by the stream, \p msg_proc is first called with the BEGIN message.
  If the client returns a non 0 value, the entry is discarded. Otherwise, the entry will be extracted,
  and the callback will be called with the ATTRIBUTES,XAT,ACL messages to allow the client to
  make modifications to the extracted fields before writing to the filesystem.
  When the entry is fully extracted, the callback is called with END. When the entry
  extraction fails, the callback is called with FAIL, and if it returns non 0, processing
  is aborted. Otherwise, processing continues.  BEGIN messages are called in archive order and
  will always be the first call for a given entry.  END of FAIL will always be the last
  call for a given entry, and will not necessarily be called in the same order.  In particular,
  for directories, END will only be called after all directory contents are extracted.

  @param dir output directory, must exist
  @param msg_data is passed as first argument to \p msg_proc
  @param msg_proc is called with queries to the caller if not NULL
  @param flags extract flags
  @param n_threads is the number of worker threads, or 0 for default

  @return a new stream instance on success, and NULL on failure
 */
APPLE_ARCHIVE_API AAArchiveStream _Nullable AAExtractArchiveOutputStreamOpen(
  const char * dir,
  void * _Nullable msg_data,
  AAEntryMessageProc _Nullable msg_proc,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a conversion archive output stream

  @discussion
  For each entry received by the stream, the \p msg_proc is first called with the CONVERT_EXCLUDE message.
  If the client returns a non 0 value, the entry is discarded. Otherwise, the entry will be converted.
  For each incoming entry header, fields in \p remove_key_set are removed from the entry.  Fields in
  \p insert_key_set are inserted if the data present in the entry allows to compute them.  This is the
  case for example for digest fields SH1, SH2, etc. when the input archive contains DAT blobs.
  Mandatory fields are never removed.
  The returned stream MUST be closed before \p stream.

  @param stream archive output stream receiving the produced archive
  @param insert_key_set fields to insert in the converted archive
  @param remove_key_set fields to remove in the converted archive
  @param msg_data is passed as first argument to \p msg_proc
  @param msg_proc is called with queries to the caller if not NULL
  @param flags convert flags
  @param n_threads is the number of worker threads, or 0 for default

  @return a new stream instance on success, and NULL on failure
 */
APPLE_ARCHIVE_API AAArchiveStream _Nullable AAConvertArchiveOutputStreamOpen(
  AAArchiveStream stream,
  AAFieldKeySet insert_key_set,
  AAFieldKeySet remove_key_set,
  void * _Nullable msg_data,
  AAEntryMessageProc _Nullable msg_proc,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/**
  @abstract Encode archive to a byte stream

  @discussion
  Received archive data is encoded and sent to \p stream.
  The returned stream MUST be closed before \p stream.

  @param stream byte stream receiving the encoded archive
  @param msg_data is passed as first argument to \p msg_proc
  @param msg_proc is called with queries to the caller if not NULL
  @param flags encoder flags
  @param n_threads is the number of worker threads, or 0 for default

  @return a new stream instance on success, and NULL on failure
 */
APPLE_ARCHIVE_API AAArchiveStream _Nullable AAEncodeArchiveOutputStreamOpen(
  AAByteStream stream,
  void * _Nullable msg_data,
  AAEntryMessageProc _Nullable msg_proc,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Input archive stream instances

/**
  @abstract Decode archive from a byte stream

  @discussion Raw archive data is read from \p stream and decoded.  The stream functions
  will then allow iteration through the decoded entries in archive order.
  The returned stream MUST be closed before \p stream.

  @param stream input stream providing raw archive data
  @param msg_data is passed as first argument to \p msg_proc
  @param msg_proc is called with queries to the caller if not NULL
  @param flags decoder flags
  @param n_threads is the number of worker threads, or 0 for default

  @return a new stream instance on success, and NULL on failure
 */
APPLE_ARCHIVE_API AAArchiveStream _Nullable AADecodeArchiveInputStreamOpen(
  AAByteStream stream,
  void * _Nullable msg_data,
  AAEntryMessageProc _Nullable msg_proc,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#pragma mark - Process an archive stream

/**
  @abstract Process all entries of an archive

  @discussion Read all entries from \p istream, and write them to \p ostream.

  @param istream input archive stream
  @param ostream output archive stream
  @param msg_data is passed as first argument to \p msg_proc
  @param msg_proc is called with queries to the caller if not NULL
  @param flags encoder flags
  @param n_threads is the number of worker threads, or 0 for default

  @return the number of archive entries transferred on success, and a negative error code on failure
 */
APPLE_ARCHIVE_API ssize_t AAArchiveStreamProcess(
  AAArchiveStream istream,
  AAArchiveStream ostream,
  void * _Nullable msg_data,
  AAEntryMessageProc _Nullable msg_proc,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif // __cplusplus

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
