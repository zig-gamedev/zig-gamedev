// AppleArchive path list

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

/*!
  @abstract Create a new path list with validated entries from a directory

  @discussion Starting from directory \p dir/path, search sub-directories and then
  all other filesystem objects under \p dir/path. The search and entry selection
  is controlled by calls to the \p msg_proc callback.  Paths passed to the callback
  are relative to \p dir, the empty string referring to \p dir itself.
  Paths stored in the result list are relative to \p dir.
  \p path can be NULL or empty. If not NULL or empty, it should be of the form
  "some/path/to/a/directory" and not include ".", "..", or symbolic links.

  @param dir input directory to search
  @param path optional sub-directory to restrict search, may be NULL to search the entire \p dir
  @param msg_data is passed as first argument to \p msg_proc
  @param msg_proc is called with queries to the caller if not NULL
  @param flags search flags
  @param n_threads is the number of worker threads, or 0 for default

  @return a new non-NULL instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAPathList _Nullable AAPathListCreateWithDirectoryContents(
  const char * dir,
  const char * _Nullable path,
  void * _Nullable msg_data,
  AAEntryMessageProc _Nullable msg_proc,
  AAFlagSet flags,
  int n_threads)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Create a new path list with a single path

  @discussion The path list only contains \p path, referring to existing filesystem object \p dir/path

  @param dir base directory
  @param path entry path under \p dir

  @return a new non-NULL instance on success, and NULL on failure
*/
APPLE_ARCHIVE_API AAPathList _Nullable AAPathListCreateWithPath(
  const char * dir,
  const char * path)
APPLE_ARCHIVE_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/*!
  @abstract Destroy a path list

  @discussion Do nothing if \p path_list is NULL

  @param path_list target object
*/
APPLE_ARCHIVE_API void AAPathListDestroy(
  AAPathList _Nullable path_list)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Get node path

  @discussion
  Let node_path be the path string corresponding to the given node.
  If \p capacity is 0, \p length receives strlen(node_path), and \p path is not accessed.
  if \p capacity > strlen(node_path), store a 0-terminated copy of node_path in path, and \p length receives strlen(node_path).
  Otherwise, return an error.

  @param path_list target object
  @param node index of node to retrieve
  @param path_capacity bytes available in \p path
  @param path receives a 0-terminated copy of node_path on success
  @param path_length receives strlen(node_path) on success if not NULL

  @return 0 on success, and a negative error code on failure
*/
APPLE_ARCHIVE_API int AAPathListNodeGetPath(
  AAPathList path_list,
  uint64_t node,
  size_t path_capacity,
  char * _Nullable path,
  size_t * _Nullable path_length)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstract Iterator on non-excluded entries, first node

  @param path_list target object

  @return index of first node, or UINT64_MAX if none
*/
APPLE_ARCHIVE_API uint64_t AAPathListNodeFirst(
  AAPathList path_list)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*!
  @abstrsct Iterator on non-excluded entries, next node

  @param path_list target object
  @paran node current node

  @return index of next node, or UINT64_MAX if none
*/
APPLE_ARCHIVE_API uint64_t AAPathListNodeNext(
  AAPathList path_list,
  uint64_t node)
APPLE_ARCHIVE_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
