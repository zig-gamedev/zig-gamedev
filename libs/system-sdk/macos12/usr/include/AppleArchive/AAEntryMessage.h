// AppleArchive entry messages

#pragma once

#ifndef __APPLE_ARCHIVE_H
#error Include AppleArchive.h instead of this file
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark - Entry messages

/*!
  @abstract Messages sent to \p AAEntryMessageProc.
*/
typedef uint32_t AAEntryMessage APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AAEntryMessages, uint32_t) {

  /*!
    @abstract Skip directory \p path in search?
    @discussion If the callback returns a >0 value, the directory is not searched. \p data is not used.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_SEARCH_PRUNE_DIR = 10,

  /*!
    @abstract Exclude entry \p path in search?
    @discussion If the callback returns a >0 value, the entry is skipped. \p data is not used.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_SEARCH_EXCLUDE   = 11,

  /*!
    @abstract Error on entry \p path in search
    @discussion If the callback returns a >0 value, signals an error when searching \p path. \p data is not used.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_SEARCH_FAIL      = 12,

  /*!
    @abstract Begin entry \p path, skip?
    @discussion If the callback returns a >0 value, the entry is skipped. \p data is a AAHeader instance.
    If the entry is skipped, this will be the last message relative to this entry.
    Otherwise, more messages may be received, with a final END or FAIL.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_EXTRACT_BEGIN    = 20,

  /*!
    @abstract End entry \p path
    @discussion Signals we are done processing entry \p path.
    If not NULL, \p data is a pointer to two uint64_t values containing the entry IDX,IDZ.
    This will be the last message relative to this entry.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_EXTRACT_END      = 21,

  /*!
    @abstract Processing entry \p path failed, abort?
    @discussion Signals an error processing entry \p path. If the callback returns a >0 value, the current action is interrupted.
    If not NULL, \p data is a pointer to two uint64_t values containing the entry IDX,IDZ.
    This will be the last message relative to this entry.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_EXTRACT_FAIL     = 22,

  /*!
    @abstract Modify entry attributes
    @discussion \p data is a pointer to a AAEntryAttributes structure, which can be modified by the callback before being applied.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_EXTRACT_ATTRIBUTES = 23,

  /*!
    @abstract Modify entry XAT blob
    @discussion \p data is a AAEntryXATBlob instance, which can be modified by the callback before being applied.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_EXTRACT_XAT      = 24,

  /*!
    @abstract Modify entry ACL blob
    @discussion \p data is a AAEntryACLBlob instance, which can be modified by the callback before being applied.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_EXTRACT_ACL      = 25,

  /*!
    @abstract Report entry scanning (hashing data, collecting attributes) from the archive encoder
    @discussion \p data is not used
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_ENCODE_SCANNING  = 30,

  /*!
    @abstract Report entry writing from the archive encoder
    @discussion If not NULL, \p data is a pointer to two uint64_t values containing the total DAT blob size,
    and the currently processed DAT blob size.  For files with large data blobs, this callback will be received
    several times to allow progress tracking and cancellation.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_ENCODE_WRITING   = 31,

  /*!
    @abstract Exclude entry \p path in conversion?
    @discussion If the callback returns a >0 value, the entry is skipped. \p data is a AAHeader instance.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_CONVERT_EXCLUDE  = 40,

  /*!
    @abstract Exclude entry \p path in processing?
    @discussion If the callback returns a >0 value, the entry is skipped. \p data is a AAHeader instance.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_PROCESS_EXCLUDE  = 50,

  /*!
    @abstract Report entry reading from the archive decoder
    @discussion If not NULL, \p data is a pointer to two uint64_t values containing the total DAT blob size,
    and the currently processed DAT blob size.  For files with large data blobs, this callback will be received
    several times to allow progress tracking and cancellation.
    If the callback returns a negative value, the current operation will be aborted as soon as possible.
  */
  AA_ENTRY_MESSAGE_DECODE_READING   = 60,

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// Entry message callback
typedef int (*AAEntryMessageProc)(void * _Nullable arg, AAEntryMessage message, const char * _Nonnull path, void * _Nullable data) APPLE_ARCHIVE_SWIFT_PRIVATE;

#ifdef __cplusplus
}
#endif
