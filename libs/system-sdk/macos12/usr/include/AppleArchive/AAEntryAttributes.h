// Entry attributes

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

// Entry attributes for archive/extract, passed as arguments to the msg_proc to allow atttribute customization
typedef struct {

  // If the <FIELD> bit is set, then <field> is used
  union {
    uint32_t bits;
    struct {
    uint32_t UID : 1;
    uint32_t GID : 1;
    uint32_t FLG : 1;
    uint32_t MOD : 1;
    uint32_t BTM : 1;
    uint32_t CTM : 1;
    uint32_t MTM : 1;
    };
  };

  uint32_t        uid;   ///< user id
  uint32_t        gid;   ///< group id
  uint32_t        flg;   ///< BSD flags
  uint32_t        mod;   ///< access mode
  struct timespec btm;   ///< backup time
  struct timespec ctm;   ///< creation time
  struct timespec mtm;   ///< modification time

} AAEntryAttributes APPLE_ARCHIVE_SWIFT_PRIVATE;

#ifdef __cplusplus
}
#endif

#if __has_feature(assume_nonnull)
_Pragma("clang assume_nonnull end")
#endif
