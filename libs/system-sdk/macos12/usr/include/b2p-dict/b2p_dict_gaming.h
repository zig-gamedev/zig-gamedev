/**
 * @file b2p_dict_gaming.h
 * @brief B2P Dictionary for Gaming products
 */

#ifndef B2P_DICT_GAMING_H_
#define B2P_DICT_GAMING_H_

#include <stdint.h>
#include "b2p_dict.h"

#ifndef _WIN32
#ifndef ATTR_PACKED
#define ATTR_PACKED   __attribute__((packed))
#endif
#else
#define ATTR_PACKED
#endif

/**
 * @defgroup B2P_CMD_RSP_GAMING B2P Command and Response codes for Gaming products
 *
 * @{
 */
#define B2P_CMD_SYS_BOOTMODE_SET            0x0400 /**< See #B2P_CMD_SYS_BOOTMODE_SET_s. */
#define B2P_RSP_SYS_BOOTMODE_SET            0x0401
#define B2P_CMD_SYS_BOOTMODE_GET            0x0402
#define B2P_RSP_SYS_BOOTMODE_GET            0x0403 /**< See #B2P_RSP_SYS_BOOTMODE_GET_s. */
#define B2P_CMD_SYS_FACTORY_EXIT            0x0404
#define B2P_RSP_SYS_FACTORY_EXIT            0x0405
#define B2P_CMD_SYS_FACTORY_MODE_GET        0x0406
#define B2P_RSP_SYS_FACTORY_MODE_GET        0x0407 /**< See #B2P_RSP_SYS_FACTORY_MODE_GET_s. */
#define B2P_CMD_SYS_RESET                   0x0408
#define B2P_RSP_SYS_RESET                   0x0409
#define B2P_CMD_SYS_FUSE_GET                0x040A
#define B2P_RSP_SYS_FUSE_GET                0x040B /**< See #B2P_RSP_SYS_FUSE_GET_s. */
#define B2P_CMD_GAMING_HID_REPORT           0x040C /**< See #B2P_CMD_GAMING_HID_REPORT_s. */
#define B2P_RSP_GAMING_HID_REPORT           0x040D

/** @} */


/*********
 * ENUMs *
 *********/


/***********
 * STRUCTs *
 ***********/

/**
 * @brief The command format for setting boot mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t bootmode;
} B2P_CMD_SYS_BOOTMODE_SET_s;

/**
 * @brief The response format for getting boot mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t bootmode; /**< 0 <= bootmode < 8 */
} B2P_RSP_SYS_BOOTMODE_GET_s;

/**
 * @brief The response format for checking if device is in factory mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t mode; /**< 1 for factory mode, 0 for not. */
} B2P_RSP_SYS_FACTORY_MODE_GET_s;

/**
 * @brief The response format for determining whether device is dev-fused.
 */
typedef struct ATTR_PACKED
{
  uint8_t fuse; /**< 1 for prod fuse, 0 for dev fuse. */
} B2P_RSP_SYS_FUSE_GET_s;

/**
 * @brief The command format for sending HID report.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[];
} B2P_CMD_GAMING_HID_REPORT_s;


#endif /* B2P_DICT_GAMING_H_ */
