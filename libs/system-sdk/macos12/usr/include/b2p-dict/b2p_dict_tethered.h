/**
 * @file b2p_dict_tethered.h
 * @brief B2P Dictionary for Tethered products
 */

#ifndef B2P_DICT_TETHERED_H_
#define B2P_DICT_TETHERED_H_

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
 * @defgroup B2P_CMD_RSP_TETHERED B2P Command and Response codes for Tethered products
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
#define B2P_CMD_SYS_POWEROFF_REASON         0x040C
#define B2P_RSP_SYS_POWEROFF_REASON         0x040D /**< See #B2P_RSP_SYS_POWEROFF_REASON_s. */
#define B2P_CMD_HALL_TEST_MODE_ENTER        0x040E /**< See #B2P_CMD_HALL_TEST_MODE_ENTER_s. */
#define B2P_RSP_HALL_TEST_MODE_ENTER        0x040F
#define B2P_CMD_HALL_TEST_MODE_EXIT         0x0410
#define B2P_RSP_HALL_TEST_MODE_EXIT         0x0411 /**< See #B2P_RSP_HALL_TEST_MODE_EXIT_s. */
#define B2P_CMD_SYS_RTCBKPREG_GET           0x0412
#define B2P_RSP_SYS_RTCBKPREG_GET           0x0413 /**< See #B2P_RSP_SYS_RTCBKPREG_GET_s. */
#define B2P_CMD_DBG_SET_DEBUG_OUTPUT        0x0414 /**< See #B2P_CMD_DBG_SET_DEBUG_OUTPUT_s. */
#define B2P_RSP_DBG_SET_DEBUG_OUTPUT        0x0415
#define B2P_CMD_DBG_SET_DEBUG_PRINT_LEVEL   0x0416 /**< See #B2P_CMD_DBG_SET_DEBUG_PRINT_LEVEL_s. */
#define B2P_RSP_DBG_SET_DEBUG_PRINT_LEVEL   0x0417
#define B2P_CMD_SYS_NVV_GET                 0x0418 /**< See #B2P_CMD_SYS_NVV_GET_s. */
#define B2P_RSP_SYS_NVV_GET                 0x0419 /**< See #B2P_RSP_SYS_NVV_GET_s. */
#define B2P_CMD_SYS_NVV_SET                 0x041A /**< See #B2P_CMD_SYS_NVV_SET_s. */
#define B2P_RSP_SYS_NVV_SET                 0x041B
#define B2P_CMD_SYS_NVV_BANK_CTRL           0x041C /**< See #B2P_CMD_SYS_NVV_BANK_CTRL_s. */
#define B2P_RSP_SYS_NVV_BANK_CTRL           0x041D
#define B2P_CMD_POWER_VOTES_GET             0x041E
#define B2P_RSP_POWER_VOTES_GET             0x041F /**< See #B2P_RSP_POWER_VOTES_GET_s. */
#define B2P_CMD_WHOAMI                      0x0420
#define B2P_RSP_WHOAMI                      0x0421 /**< See #B2P_RSP_WHOAMI_s. */
#define B2P_CMD_POWER_MODE_SET              0x0422 /**< See #B2P_CMD_POWER_MODE_SET_s. */
#define B2P_RSP_POWER_MODE_SET              0x0423
#define B2P_CMD_GET_LOGS                    0x0424 /**< See #B2P_CMD_GET_LOGS_s. */
#define B2P_RSP_GET_LOGS                    0x0425 /**< See #B2P_RSP_GET_LOGS_s. */
#define B2P_CMD_BL_ENTER                    0x0426
#define B2P_RSP_BL_ENTER                    0x0427
#define B2P_CMD_BT_PAIRING_STATE_SET        0x0428 /**< See #B2P_CMD_BT_PAIRING_STATE_SET_s. */
#define B2P_RSP_BT_PAIRING_STATE_SET        0x0429
#define B2P_CMD_BATT_INFO_GET               0x0430
#define B2P_RSP_BATT_INFO_GET               0x0431 /**< See #B2P_RSP_BATT_INFO_GET_s. */
#define B2P_CMD_HINGE_STATUS_GET            0x0432
#define B2P_RSP_HINGE_STATUS_GET            0x0433 /**< See #B2P_RSP_HINGE_STATUS_GET_s. */
#define B2P_CMD_ENABLE_DISCHARGING          0x0434 /**< See #B2P_CMD_ENABLE_DISCHARGING_s. */
#define B2P_RSP_ENABLE_DISCHARGING          0x0435
// #define B2P_CMD_GG_INFO_SET                 0x0436 // Reserved for B515 (for now)
// #define B2P_RSP_GG_INFO_SET                 0x0437
// #define B2P_CMD_BATT_HEALTH_SET             0x0438
// #define B2P_RSP_BATT_HEALTH_SET             0x0439
// #define B2P_CMD_BATT_HEALTH_GET             0x043A
// #define B2P_RSP_BATT_HEALTH_GET             0x043B



/** @} */


/*********
 * ENUMs *
 *********/

/**
 * @brief Different reasons for powering off system
 */
typedef enum
{
  B2P_POWEROFF_REASON_unknown,
  B2P_POWEROFF_REASON_userpower_off,
  B2P_POWEROFF_REASON_charger_disconnect,
  B2P_POWEROFF_REASON_crit_battery,
  B2P_POWEROFF_REASON_system_overtemp,
  B2P_POWEROFF_REASON_system_undertemp,
  B2P_POWEROFF_REASON_batt_overtemp,
  B2P_POWEROFF_REASON_batt_undertemp,
  B2P_POWEROFF_REASON_aux_connected,
  B2P_POWEROFF_REASON_pairingtimer_expiry,
  B2P_POWEROFF_REASON_inactivitytimer_expiry,
  B2P_POWEROFF_REASON_fw_update,
  B2P_POWEROFF_REASON_hard_reset,
  B2P_POWEROFF_REASON_factory_reset,
  B2P_POWEROFF_REASON_motion_timeout,
  B2P_POWEROFF_REASON_aux_plug_during_disabled_interrupts,
  B2P_POWEROFF_REASON_dev_core_reset,
  B2P_POWEROFF_REASON_LAST
} B2P_POWEROFF_REASON_e;

/**
 * @brief Pairing State Enable.
 */
typedef enum
{
  B2P_BT_PAIRING_STATE_disable,
  B2P_BT_PAIRING_STATE_enable,
} B2P_BT_PAIRING_STATE_e;

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
 * @brief The response format for getting last power off reason.
 */
typedef struct ATTR_PACKED
{
  uint32_t reason; /**< See #B2P_POWEROFF_REASON_e. */
} B2P_RSP_SYS_POWEROFF_REASON_s;

/**
 * @brief The command format for putting hall effect sensors in test mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t mode;
} B2P_CMD_HALL_TEST_MODE_ENTER_s;

/**
 * @brief The response format for ending hall effect sensor test.
 */
typedef struct ATTR_PACKED
{
  uint32_t record;
} B2P_RSP_HALL_TEST_MODE_EXIT_s;

/**
 * @brief The command format for setting debug print output port.
 */
typedef struct ATTR_PACKED
{
  uint8_t port; /**< Specified where to dump the output. */
} B2P_CMD_DBG_SET_DEBUG_OUTPUT_s;

/**
 * @brief The command format for setting debug print module level.
 */
typedef struct ATTR_PACKED
{
  uint16_t bmModule; /**< Bitmask of which modules to set levels for. */
  uint8_t bmLevel; /**< Bitmask for print levels. */
} B2P_CMD_DBG_SET_DEBUG_PRINT_LEVEL_s;

/**
 * @brief The command format for retrieving NVV variables.
 */
typedef struct ATTR_PACKED
{
  uint16_t var_id; /**< NVV Variable ID. */
} B2P_CMD_SYS_NVV_GET_s;

/**
 * @brief The response format for retrieving NVV variables.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[1];
} B2P_RSP_SYS_NVV_GET_s;

/**
 * @brief The command format for setting NVV variables.
 */
typedef struct ATTR_PACKED
{
  uint16_t var_id; /**< NVV Variable ID. */
  uint8_t data[1];
} B2P_CMD_SYS_NVV_SET_s;

/**
 * @brief The response format for getting NVV control data.
 */
typedef struct ATTR_PACKED
{
  uint32_t ctr;
  uint8_t active_bank_id;
} B2P_CMD_SYS_NVV_BANK_CTRL_s;

/**
 * @brief The response format for getting the power votes by different modules.
 */
typedef struct ATTR_PACKED
{
  uint8_t votes[1]; /**< Votes of each subsystem. */
} B2P_RSP_POWER_VOTES_GET_s;

/**
 * @brief The response format for getting RTC backup registers.
 */
typedef struct ATTR_PACKED
{
  uint32_t bk_regs[5];
} B2P_RSP_SYS_RTCBKPREG_GET_s;

/**
 * @brief The response format for WHO AM I command.
 */
typedef struct ATTR_PACKED
{
  uint8_t route;
} B2P_RSP_WHOAMI_s;

/**
 * @brief The command format for setting power mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t powermode; /**< Sleep mode can be a byte from 1 to 5 (lowest power) or so, with increasingly aggressive power savings. */
} B2P_CMD_POWER_MODE_SET_s;

/**
 * @brief The command format to get logs.
 */
typedef struct ATTR_PACKED
{
  uint8_t device; /**< Logs for which device. See #B2P_DEVICE_e. */
  uint8_t chunk_no; /**< Chunk number */
} B2P_CMD_GET_LOGS_s;

/**
 * @brief The response format to retrieve logs.
 */
typedef struct ATTR_PACKED
{
  uint8_t log_data[B2P_PACKET_MAX_DATA_SIZE];
} B2P_RSP_GET_LOGS_s;

/**
 * @brief The command format to get log size.
 */
typedef struct ATTR_PACKED
{
  uint8_t device; /**< Logs for which device. See #B2P_DEVICE_e. */
} B2P_CMD_GET_LOG_SIZE_s;

/**
 * @brief The command format to get size of last stored logs.
 */
typedef struct ATTR_PACKED
{
  uint16_t log_size;
} B2P_RSP_GET_LOG_SIZE_s;

/**
 * @brief The command format to clear logs.
 */
typedef struct ATTR_PACKED
{
  uint8_t device; /**< Logs for which device. See #B2P_DEVICE_e. */
} B2P_CMD_CLEAR_LOG_s;

/**
 * @brief The command format for pairing state set
 */
typedef struct ATTR_PACKED
{
  uint8_t discoverable; /**< See #B2P_BT_PAIRING_STATE_e. */
} B2P_CMD_BT_PAIRING_STATE_SET_s;

/**
 * @brief The Response format for getting Battery Info
 *        CNFG_BATTERY_INFO_NUM
 */
typedef struct ATTR_PACKED
{
  uint8_t id;      /**< BATT_ID_XXX  */
  uint8_t type;    /**< BATT_TYPE_XXX  */
  uint8_t level;   /**< BATT_LEVEL_XXX or 0-100 value */
  uint8_t state;   /**< BATT_STATE_XXX  */
  uint8_t status;  /**< BATT_STATUS_XXX  */
} B2P_RSP_BATT_INFO_GET_s;

/**
 * @brief The command gets the hinge status
 */
typedef struct ATTR_PACKED
{
  uint8_t hinge_status; /**< 0 means either or all hinges closed, 1 means all hinges open. */
} B2P_RSP_HINGE_STATUS_GET_s;

typedef struct ATTR_PACKED
{

} B2P_CMD_ENABLE_DISCHARGING_s;


#endif /* B2P_DICT_TETHERED_H_ */
