// AppleArchive definitions

#pragma once

#ifndef __APPLE_ARCHIVE_H
#error Include AppleArchive.h instead of this file
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark - Constants

#define APPLE_ARCHIVE_API_VERSION 1200

// Hash function
typedef uint32_t AAHashFunction APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AAHashFunctions,uint32_t) {

  AA_HASH_FUNCTION_CRC32       = 1,     ///< POSIX 1003.2-1992 32-bit CRC (as used in BOM)
  AA_HASH_FUNCTION_SHA1        = 2,     ///< SHA1 160 bits
  AA_HASH_FUNCTION_SHA256      = 3,     ///< SHA2 256 bits
  AA_HASH_FUNCTION_SHA384      = 4,     ///< SHA2 384 bits
  AA_HASH_FUNCTION_SHA512      = 5,     ///< SHA2 512 bits

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// Compression algorithms
// These constants match the values in compression.h
typedef uint32_t AACompressionAlgorithm APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AACompressionAlgorithms,uint32_t) {

  AA_COMPRESSION_ALGORITHM_NONE     = 0x000, // copy data, no compression
  AA_COMPRESSION_ALGORITHM_LZ4      = 0x100, // LZ4 with the frame format used in the Compression library
  AA_COMPRESSION_ALGORITHM_ZLIB     = 0x505, // ZLIB level 5 (RFC1950 zlib format, includes 2 bytes header before raw deflate)
  AA_COMPRESSION_ALGORITHM_LZMA     = 0x306, // LZMA preset 6
  AA_COMPRESSION_ALGORITHM_LZFSE    = 0x801, // LZFSE

} APPLE_ARCHIVE_SWIFT_PRIVATE;

#pragma mark - AppleArchive archive headers

// AppleArchive entry types, corresponding to the stat(2) st_mode values
// The char is the value stored in the AppleArchive stream the 'TYP' UINT header entry
typedef uint32_t AAEntryType APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AAEntryTypes, uint32_t) {

  AA_ENTRY_TYPE_REG            = 'F',   ///< regular file
  AA_ENTRY_TYPE_DIR            = 'D',   ///< directory
  AA_ENTRY_TYPE_LNK            = 'L',   ///< symbolic link
  AA_ENTRY_TYPE_FIFO           = 'P',   ///< fifo
  AA_ENTRY_TYPE_CHR            = 'C',   ///< character special
  AA_ENTRY_TYPE_BLK            = 'B',   ///< block special
  AA_ENTRY_TYPE_SOCK           = 'S',   ///< socket
  AA_ENTRY_TYPE_WHT            = 'W',   ///< whiteout
  AA_ENTRY_TYPE_DOOR           = 'R',   ///< door
  AA_ENTRY_TYPE_PORT           = 'T',   ///< port

  AA_ENTRY_TYPE_METADATA       = 'M',   ///< metadata (not a file system object)

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// AppleArchive header field types
typedef uint32_t AAFieldType APPLE_ARCHIVE_SWIFT_PRIVATE;
APPLE_ARCHIVE_ENUM(AAFieldTypes, uint32_t) {

  AA_FIELD_TYPE_FLAG           = 0,     ///< flag
  AA_FIELD_TYPE_UINT           = 1,     ///< unsigned integer
  AA_FIELD_TYPE_STRING         = 2,     ///< string
  AA_FIELD_TYPE_HASH           = 3,     ///< hash
  AA_FIELD_TYPE_TIMESPEC       = 4,     ///< time
  AA_FIELD_TYPE_BLOB           = 5,     ///< data blob

} APPLE_ARCHIVE_SWIFT_PRIVATE;

// Known AppleArchive field keys

// AAFieldKey constant from a 3-char constant string S
#define AA_FIELD_C(s) ((AAFieldKey){.skey=s})
//                                             type     description
//                                        ------------------------------------------------------------------------------------------
#define AA_FIELD_ACL AA_FIELD_C("ACL")    ///< BLOB     entry Access Control List (dirs and regular files only, see AAEntryACLBlob)
#define AA_FIELD_BTM AA_FIELD_C("BTM")    ///< TIMESPEC backup time
#define AA_FIELD_CKS AA_FIELD_C("CKS")    ///< HASH     entry data 32-bit CRC (regular files only)
#define AA_FIELD_CLC AA_FIELD_C("CLC")    ///< UINT     clone cluster id (regular files only)
#define AA_FIELD_CTM AA_FIELD_C("CTM")    ///< TIMESPEC creation time
#define AA_FIELD_DAT AA_FIELD_C("DAT")    ///< BLOB     entry data (regular files only)
#define AA_FIELD_DEV AA_FIELD_C("DEV")    ///< UINT     device id (st.st_rdev, char/block devices only)
#define AA_FIELD_DE2 AA_FIELD_C("DE2")    ///< UINT     device minor (from tar archives, char/block devices only)
#define AA_FIELD_DUZ AA_FIELD_C("DUZ")    ///< UINT     disk usage
#define AA_FIELD_FLG AA_FIELD_C("FLG")    ///< UINT     flags (st.st_flags)
#define AA_FIELD_GID AA_FIELD_C("GID")    ///< UINT     group id (st.st_gid)
#define AA_FIELD_GIN AA_FIELD_C("GIN")    ///< STRING   group name (from tar archives)
#define AA_FIELD_HLC AA_FIELD_C("HLC")    ///< UINT     hard link cluster id (regular files only)
#define AA_FIELD_IDX AA_FIELD_C("IDX")    ///< UINT     offset of entry in reference archive
#define AA_FIELD_IDZ AA_FIELD_C("IDZ")    ///< UINT     size of entry in reference archive
#define AA_FIELD_INO AA_FIELD_C("INO")    ///< UINT     inode number (st.st_ino)
#define AA_FIELD_LNK AA_FIELD_C("LNK")    ///< STRING   symbolic link path (symbolic links only)
#define AA_FIELD_MOD AA_FIELD_C("MOD")    ///< UINT     access modes (low 12 bits of st.st_mode)
#define AA_FIELD_MTM AA_FIELD_C("MTM")    ///< TIMESPEC modification time
#define AA_FIELD_NLK AA_FIELD_C("NLK")    ///< UINT     number of hard links (st.st_nlink from cpio archives)
#define AA_FIELD_PAT AA_FIELD_C("PAT")    ///< STRING   entry path
#define AA_FIELD_SH1 AA_FIELD_C("SH1")    ///< HASH     entry data SHA1 hash (regular files only)
#define AA_FIELD_SH2 AA_FIELD_C("SH2")    ///< HASH     entry data SHA2-256 hash (regular files only)
#define AA_FIELD_SH3 AA_FIELD_C("SH3")    ///< HASH     entry data SHA2-384 hash (regular files only)
#define AA_FIELD_SH5 AA_FIELD_C("SH5")    ///< HASH     entry data SHA2-512 hash (regular files only)
#define AA_FIELD_SIZ AA_FIELD_C("SIZ")    ///< UINT     uncompressed data size (regular files only)
#define AA_FIELD_SLC AA_FIELD_C("SLC")    ///< UINT     identical data cluster id (regular files only)
#define AA_FIELD_TYP AA_FIELD_C("TYP")    ///< UINT     entry type (from the high bits of st.st_mode, one of AA_ENTRY_TYPE_*)
#define AA_FIELD_UID AA_FIELD_C("UID")    ///< UINT     user id (st.st_uid)
#define AA_FIELD_UIN AA_FIELD_C("UIN")    ///< STRING   user name (from tar archives)
#define AA_FIELD_XAT AA_FIELD_C("XAT")    ///< BLOB     entry extended attributes (see AAEntryXATBlob)
#define AA_FIELD_YAF AA_FIELD_C("YAF")    ///< BLOB     string containing a list of archived fields, separated by ',' (metadata only)

#pragma mark - AppleArchive objects

typedef struct AAPathList_impl       * AAPathList      APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef struct AAHeader_impl         * AAHeader        APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef struct AAFieldKeySet_impl    * AAFieldKeySet   APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef struct AAEntryACLBlob_impl   * AAEntryACLBlob  APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef struct AAEntryXATBlob_impl   * AAEntryXATBlob  APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef struct AAByteStream_impl     * AAByteStream    APPLE_ARCHIVE_SWIFT_PRIVATE;
typedef struct AAArchiveStream_impl  * AAArchiveStream APPLE_ARCHIVE_SWIFT_PRIVATE;

#ifdef __cplusplus
}
#endif
