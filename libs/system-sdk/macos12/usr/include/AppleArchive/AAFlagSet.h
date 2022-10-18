// AppleArchive flags

#pragma once

#ifndef __APPLE_ARCHIVE_H
#error Include AppleArchive.h instead of this file
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark - Common flags (64 bits)

typedef uint64_t AAFlagSet APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AAFlags, uint64_t) {

  /*!
    @abstract Ignore EPERM errors

    @discussion
    Ignore permission errors (EPERM) in system calls when archiving and extracting files.
  */
  AA_FLAG_IGNORE_EPERM = (UINT64_C(1) << 0),

  /*!
    @abstract Skip duplicate DAT fields

    @discussion
    The default for regular files is to archive a DAT blob for each entry, even if it means duplicating the blob
    for each instance of a hard link cluster (same HLC field) / clone cluster (same CLC field) / files with same
    data cluster (same SLC field).
    If this flag is set, it lowers the requirement to only have at least one entry with DAT in each HLC/CLC/SLC cluster.
    There may still be some duplicate DAT in the archive, in case a file belongs to different HLC/CLC/SLC clusters.

    Setting this flag will make the archive smaller, but will prevent some uses of the archive. For example when extracting
    a subset of the archive, if the file carrying the DAT blob in a cluster is excluded, extracting other files of the same
    cluster will fail.
  */
  AA_FLAG_ARCHIVE_DEDUPLICATE_DAT = (UINT64_C(1) << 1),

  /*!
    @abstract Don't resolve ACL entry qualifiers

    @discussion
    By default, when archiving, ACL entry qualifiers are resolved to user name, group name, or SID whenever possible.
    If this flag is set, do not resolve the qualifiers, and store GUID.
  */
  AA_FLAG_ARCHIVE_NO_RESOLVE_ACL_QUALIFIERS = (UINT64_C(1) << 2),

  /*!
    @abstract Replace XAT/ACL when applying

    @discussion Clear extended attributes / access control lists before setting them with the *ApplyToPath functions
  */
  AA_FLAG_REPLACE_ATTRIBUTES = (UINT64_C(1) << 3),

  /*!
    @abstract Don't extract duplicates as clones/hard links

    @discussion
    By default, files with identical data (archived with the same SLC cluster ID) are extracted as clones if the filesystem supports it.
    If this flag is set, extract them as copies.
  */
  AA_FLAG_EXTRACT_NO_AUTO_DEDUP = (UINT64_C(1) << 4),

  /*!
    @abstract Don't extract zero blocks as holes

    @discussion
    By default, holes are created for aligned blocks of 0 in file data if the filesystem supports it.
    If this flag is set, do not use holes, and store all 0.
  */
  AA_FLAG_EXTRACT_NO_AUTO_SPARSE = (UINT64_C(1) << 5),

  /*!
    @abstract Allow crossing volume boundaries

    @discussion
    By default, when iterating directory contents, search is restricted to the initial volume.
    If this flag is set, allow descent into different volumes.
  */
  AA_FLAG_CROSS_VOLUME_BOUNDARIES = (UINT64_C(1) << 6),

  /*!
    @abstract Extract duplicates as hard links

    @discussion
    By default, when auto-deduplication is enabled, files with identical data (archived with the same SLC cluster ID)
    are extracted as clones if the filesystem supports it.
    If this flag is set, extract them as hard links instead.
    WARNING: this should be used with caution, because hard links are not copy on write: any change made to one
    file in the SLC cluster will be reflected in all other members of the cluster.
  */
  AA_FLAG_EXTRACT_AUTO_DEDUP_AS_HARD_LINKS = (UINT64_C(1) << 7),

  /*!
    @abstract Insert IDX,IDZ field when decoding archive

    @discussion
    If this bit is set, the archive decoder will insert IDX,IDZ fields in the header before
    returning from readHeader.  These are the offset and size of the entry in the uncompressed
    archive.
  */
  AA_FLAG_DECODE_INSERT_IDX = (UINT64_C(1) << 8),

  /*!
    @abstract Exclude metadata entries

    @discussion
    If this bit is set, metadata entries will be excluded from processing.
  */
  AA_FLAG_EXCLUDE_METADATA_ENTRIES = (UINT64_C(1) << 9),

  /*!
    @abstract Use \p pwrite on the output stream in RandomAccessByteStreamProcess
  */
  AA_FLAG_PROCESS_RANDOM_ACCESS_OUTPUT = (UINT64_C(1) << 10),

  /*!
    @abstract Verbosity level

    @discussion A verbosity level can be passed to most functions, logging to stderr:
    0 = silent, this is the default
    1 = info, log call args and global stats
    2 = debug, log some debug details
    3 = heavy debug, log all debug details
  */
  AA_FLAG_VERBOSITY_0 = (UINT64_C(0) << 62),
  AA_FLAG_VERBOSITY_1 = (UINT64_C(1) << 62),
  AA_FLAG_VERBOSITY_2 = (UINT64_C(2) << 62),
  AA_FLAG_VERBOSITY_3 = (UINT64_C(3) << 62),

} APPLE_ARCHIVE_SWIFT_PRIVATE;

#define AA_FLAG_VERBOSITY(level) ((((uint64_t)(level) > UINT64_C(3))?UINT64_C(3):(uint64_t)(level)) << 62)

#ifdef __cplusplus
}
#endif
