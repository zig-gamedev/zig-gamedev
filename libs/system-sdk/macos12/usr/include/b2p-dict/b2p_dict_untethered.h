/**
 * @file b2p_dict_untethered.h
 * @brief B2P Dictionary for Untethered
 */

#ifndef B2P_DICT_UNTETHERED_H_
#define B2P_DICT_UNTETHERED_H_

#include <stdint.h>
#include "b2p_dict.h"

#ifndef _WIN32
#ifndef ATTR_PACKED
#define ATTR_PACKED   __attribute__((packed))
#endif
#else
#define ATTR_PACKED
#endif

#define B2P_DS_TAG_FWUP_HOST_TO_BUD_L  "h2b.l"
#define B2P_DS_TAG_FWUP_HOST_TO_BUD_R  "h2b.r"
#define B2P_DS_TAG_FWUP_HOST_TO_CASE   "h2c"
#define B2P_DS_TAG_DFU_BUD_L           "dfu.l"
#define B2P_DS_TAG_DFU_BUD_R           "dfu.r"
#define B2P_DS_TAG_CASE_LOGDUMP        "log.c"
#define B2P_DS_TAG_BUD_LOGDUMP_L       "log.l"
#define B2P_DS_TAG_BUD_LOGDUMP_R       "log.r"
#define B2P_DS_TAG_FWUP_BUD_TO_BUD_L   "b2b.l"
#define B2P_DS_TAG_FWUP_BUD_TO_BUD_R   "b2b.r"
#define B2P_DS_TAG_FWUP_BUD_TO_CASE_L  "b2c.l"
#define B2P_DS_TAG_FWUP_BUD_TO_CASE_R  "b2c.r"

  /*! Number of temperature ranges for battery health */
#define BATT_HEALTH_NUM_TEMP_RANGES    6
  /*! Number of voltage ranges for battery health */
#define BATT_HEALTH_NUM_VOLTAGE_RANGES 5
  /*! Number of milestones for battery health */
#define BATT_HEALTH_MILESTONES         6

/**
 * @defgroup B2P_CMD_RSP_UNTETHERED B2P Command and Response codes for Untethered
 * @brief Opcode 0x400.
 *
 * @{
 */
#define B2P_CMD_SWEAT_GET               0x0400
#define B2P_RSP_SWEAT_GET               0x0401 /**< See #B2P_RSP_SWEAT_GET_s. */
#define B2P_CMD_PAIRING_STATE_SET       0x0402 /**< See #B2P_CMD_PAIRING_STATE_SET_s. */
#define B2P_RSP_PAIRING_STATE_SET       0x0403
#define B2P_CMD_RADIO_STATE_SET         0x0404 /**< See #B2P_CMD_RADIO_STATE_SET_s. */
#define B2P_RSP_RADIO_STATE_SET         0x0405
#define B2P_CMD_BATTERY_STATUS_SYNC     0x0406 /**< See #B2P_CMD_BATTERY_STATUS_SYNC_s. */
#define B2P_RSP_BATTERY_STATUS_SYNC     0x0407 /**< See #B2P_RSP_BATTERY_STATUS_SYNC_s. */
#define B2P_CMD_BUD_SYSTEM_INFO_SYNC    0x0408 /**< See #B2P_CMD_BUD_SYSTEM_INFO_SYNC_s. */
#define B2P_RSP_BUD_SYSTEM_INFO_SYNC    0x0409 /**< See #B2P_RSP_BUD_SYSTEM_INFO_SYNC_s. */
#define B2P_CMD_COUPLE_SET              0x040A /**< See #B2P_CMD_COUPLE_SET_s. */
#define B2P_RSP_COUPLE_SET              0x040B
#define B2P_CMD_ROLE_SET                0x040C /**< See #B2P_CMD_ROLE_SET_s. */
#define B2P_RSP_ROLE_SET                0x040D
#define B2P_CMD_ROLE_GET                0x040E
#define B2P_RSP_ROLE_GET                0x040F /**< See #B2P_RSP_ROLE_GET_s. */
#define B2P_CMD_SYSTEM_VERSION_SYNC     0x0410 /**< See #B2P_CMD_SYSTEM_VERSION_SYNC_s. */
#define B2P_RSP_SYSTEM_VERSION_SYNC     0x0411 /**< See #B2P_RSP_SYSTEM_VERSION_SYNC_s. */
#define B2P_CMD_TEST_CCLR               0x0412
#define B2P_RSP_TEST_CCLR               0x0413
#define B2P_CMD_TEST_PARAM              0x0414 /**< See #B2P_CMD_TEST_PARAM_s. */
#define B2P_RSP_TEST_PARAM              0x0415
#define B2P_CMD_SET_FWUP_SIZE           0x0416 /**< See #B2P_CMD_SET_FWUP_SIZE_s. */
#define B2P_RSP_SET_FWUP_SIZE           0x0417
#define B2P_CMD_AVRCP                   0x0418 /**< See #B2P_CMD_AVRCP_s. */
#define B2P_RSP_AVRCP                   0x0419
#define B2P_CMD_HFP                     0x041A /**< See #B2P_CMD_HFP_s. */
#define B2P_RSP_HFP                     0x041B
#define B2P_CMD_TEST_ENTER              0x041C /**< See #B2P_CMD_TEST_ENTER_s. */
#define B2P_RSP_TEST_ENTER              0x041D
#define B2P_CMD_LOG_READ                0x041E
#define B2P_RSP_LOG_READ                0x041F
#define B2P_CMD_FWUP_APPLY              0x0420 /**< See #B2P_CMD_FWUP_APPLY_s. */
#define B2P_RSP_FWUP_APPLY              0x0421
#define B2P_CMD_SSIDE                   0x0422 /**< See #B2P_CMD_SSIDE_s. */
#define B2P_RSP_SSIDE                   0x0423
#define B2P_CMD_NVM_COMMIT              0x0424
#define B2P_RSP_NVM_COMMIT              0x0425
#define B2P_CMD_BL_ENTER                0x0426
#define B2P_RSP_BL_ENTER                0x0427
#define B2P_CMD_BUD_RESET               0x0428 /**< See #B2P_CMD_BUD_RESET_s. */
#define B2P_RSP_BUD_RESET               0x0429
#define B2P_CMD_SYS_STATUS              0x042A
#define B2P_RSP_SYS_STATUS              0x042B /**< See #B2P_RSP_SYS_STATUS_s. */
#define B2P_CMD_TFL_SET                 0x042C /**< See #B2P_CMD_TFL_SET_s. */
#define B2P_RSP_TFL_SET                 0x042D
#define B2P_CMD_TFL_GET                 0x042E
#define B2P_RSP_TFL_GET                 0x042F /**< See #B2P_RSP_TFL_GET_s. */
#define B2P_CMD_CASE_INFO_GET           0x0430
#define B2P_RSP_CASE_INFO_GET           0x0431 /**< See #B2P_RSP_CASE_INFO_GET_s. */
#define B2P_CMD_BATTERY_INFO_GET        0x0432
#define B2P_RSP_BATTERY_INFO_GET        0x0433 /**< See #B2P_RSP_BATTERY_INFO_GET_s. */
#define B2P_CMD_CASE_INFO_SEND          0x0434 /**< See #B2P_CMD_CASE_INFO_SEND_s. */
#define B2P_RSP_CASE_INFO_SEND          0x0435
#define B2P_CMD_SWEAT_RESET             0x0436
#define B2P_RSP_SWEAT_RESET             0x0437
#define B2P_CMD_BATMAN_GET              0x0438
#define B2P_RSP_BATMAN_GET              0x0439 /**< See #B2P_RSP_BATMAN_GET_s. */
#define B2P_CMD_CHARGER_REG_GET         0x0440
#define B2P_RSP_CHARGER_REG_GET         0x0441 /**< See #B2P_RSP_CHARGER_REG_GET_s. */
#define B2P_CMD_CASE_SN_SEND            0x0442 /**< See #B2P_CMD_CASE_SN_SEND_s. */
#define B2P_RSP_CASE_SN_SEND            0x0443
#define B2P_CMD_BOOTMODE_SET            0x0444 /**< See #B2P_CMD_BOOTMODE_SET_s. */
#define B2P_RSP_BOOTMODE_SET            0x0445
#define B2P_CMD_BOOTMODE_GET            0x0446
#define B2P_RSP_BOOTMODE_GET            0x0447 /**< See #B2P_RSP_BOOTMODE_GET_s. */
#define B2P_CMD_COUPLE_START            0x0448 /**< See #B2P_CMD_COUPLE_START_s. */
#define B2P_RSP_COUPLE_START            0x0449 /**< See #B2P_RSP_COUPLE_START_s. */
#define B2P_CMD_PASSKEY_GET             0x044A
#define B2P_RSP_PASSKEY_GET             0x044B /**< See #B2P_RSP_PASSKEY_GET_s. */
#define B2P_CMD_PASSKEY_SET             0x044C /**< See #B2P_CMD_PASSKEY_SET_s. */
#define B2P_RSP_PASSKEY_SET             0x044D
#define B2P_CMD_BUD_COMMS_LVL_SET       0x044E /**< See #B2P_CMD_BUD_COMMS_LVL_SET_s. */
#define B2P_RSP_BUD_COMMS_LVL_SET       0x044F
#define B2P_CMD_BUD_COMMS_LVL_GET       0x0450
#define B2P_RSP_BUD_COMMS_LVL_GET       0x0451 /**< See #B2P_RSP_BUD_COMMS_LVL_GET_s. */
#define B2P_CMD_BUD_SWAP_TRIGGER        0x0452
#define B2P_RSP_BUD_SWAP_TRIGGER        0x0453
#define B2P_CMD_BT_CADDR_GET            0x0454
#define B2P_RSP_BT_CADDR_GET            0x0455 /**< See #B2P_RSP_BT_CADDR_GET_s. */
#define B2P_CMD_FWUP_INSTALL            0x0456
#define B2P_RSP_FWUP_INSTALL            0x0457
#define B2P_CMD_FWUP_CLEAR              0x0458
#define B2P_RSP_FWUP_CLEAR              0x0459
#define B2P_CMD_BUD_SIDE_GET            0x045A
#define B2P_RSP_BUD_SIDE_GET            0x045B /**< See #B2P_RSP_BUD_SIDE_GET_s. */
#define B2P_CMD_BUD_UNCOUPLE            0x045C
#define B2P_RSP_BUD_UNCOUPLE            0x045D
#define B2P_CMD_SYS_DATA_FETCH          0x045E
#define B2P_RSP_SYS_DATA_FETCH          0x045F /**< See #B2P_RSP_SYS_DATA_FETCH_s. */
#define B2P_CMD_SYS_PING                0x0460
#define B2P_RSP_SYS_PING                0x0461 /**< See #B2P_RSP_SYS_PING_s. */
#define B2P_CMD_SYS_FWUP_STATE_GET      0x0462
#define B2P_RSP_SYS_FWUP_STATE_GET      0x0463 /**< See #B2P_RSP_SYS_FWUP_STATE_GET_s. */
#define B2P_CMD_BT_CADDR_SET            0x0464 /**< See #B2P_CMD_BT_CADDR_SET_s. */
#define B2P_RSP_BT_CADDR_SET            0x0465
#define B2P_CMD_BT_UTP_STATE_GET        0x0466
#define B2P_RSP_BT_UTP_STATE_GET        0x0467 /**< See #B2P_RSP_BT_UTP_STATE_GET_s. */
#define B2P_CMD_BUTTON_MODE_SET         0x0468 /**< See #B2P_CMD_BUTTON_MODE_SET_s. */
#define B2P_RSP_BUTTON_MODE_SET         0x0469
#define B2P_CMD_BUTTON_MODE_GET         0x046A
#define B2P_RSP_BUTTON_MODE_GET         0x046B /**< See #B2P_RSP_BUTTON_MODE_GET_s. */
#define B2P_CMD_SEAL_SETUP_SEND         0x046C /**< See #B2P_CMD_SEAL_SETUP_SEND_PAYLOAD_s. */
#define B2P_RSP_SEAL_SETUP_SEND         0x046D
#define B2P_CMD_SEAL_SETUP_SET          0x046E /**< See #B2P_CMD_SEAL_SETUP_SET_s. */
#define B2P_RSP_SEAL_SETUP_SET          0x046F
#define B2P_CMD_BT_CINFO_GET            0x0470
#define B2P_RSP_BT_CINFO_GET            0x0471 /**< See #B2P_RSP_BT_CINFO_GET_s. */
#define B2P_CMD_BT_CINFO_SET            0x0472 /**< See #B2P_CMD_BT_CINFO_SET_s. */
#define B2P_RSP_BT_CINFO_SET            0x0473
#define B2P_CMD_PAIRING_STATE_GET       0x0474
#define B2P_RSP_PAIRING_STATE_GET       0x0475 /**< See #B2P_RSP_PAIRING_STATE_GET_s. */
#define B2P_CMD_RADIO_STATE_GET         0x0476
#define B2P_RSP_RADIO_STATE_GET         0x0477 /**< See #B2P_RSP_RADIO_STATE_GET_s. */
#define B2P_CMD_BUD_LOCATION_GET        0x0478
#define B2P_RSP_BUD_LOCATION_GET        0x0479 /**< See #B2P_RSP_BUD_LOCATION_GET_s. */
#define B2P_CMD_BUD_LOCATION_NOTIFY     0x047A /**< See #B2P_CMD_BUD_LOCATION_NOTIFY_s. */
#define B2P_RSP_BUD_LOCATION_NOTIFY     0x047B
#define B2P_CMD_ONE_BUD_ANC_MODE_GET    0x047C
#define B2P_RSP_ONE_BUD_ANC_MODE_GET    0x047D /**< See #B2P_RSP_ONE_BUD_ANC_MODE_GET_s. */
#define B2P_CMD_ONE_BUD_ANC_MODE_SET    0x047E /**< See #B2P_CMD_ONE_BUD_ANC_MODE_SET_s. */
#define B2P_RSP_ONE_BUD_ANC_MODE_SET    0x047F
#define B2P_CMD_BUD_DOCK_STATE_SET      0x0480 /**< See #B2P_CMD_BUD_DOCK_STATE_SET_s. */
#define B2P_RSP_BUD_DOCK_STATE_SET      0x0481
#define B2P_CMD_BUD_DOCK_STATE_GET      0x0482
#define B2P_RSP_BUD_DOCK_STATE_GET      0x0483 /**< See #B2P_RSP_BUD_DOCK_STATE_GET_s. */
#define B2P_CMD_BUD_LID_STATE_SET       0x0484 /**< See #B2P_CMD_BUD_LID_STATE_SET_s. */
#define B2P_RSP_BUD_LID_STATE_SET       0x0485
#define B2P_CMD_BUD_LID_STATE_GET       0x0486
#define B2P_RSP_BUD_LID_STATE_GET       0x0487 /**< See #B2P_RSP_BUD_LID_STATE_GET_s. */
#define B2P_CMD_STEM_CLICK_GET          0x0488
#define B2P_RSP_STEM_CLICK_GET          0x0489
#define B2P_CMD_STEM_CLICK_SET          0x048A
#define B2P_RSP_STEM_CLICK_SET          0x048B
#define B2P_CMD_CASE_MGR_STATE_LOCK_SET 0x048C /**< See #B2P_CMD_CASE_MGR_STATE_LOCK_SET_s. */
#define B2P_RSP_CASE_MGR_STATE_LOCK_SET 0x048D
#define B2P_CMD_CASE_MGR_STATE_LOCK_GET 0x048E
#define B2P_RSP_CASE_MGR_STATE_LOCK_GET 0x048F /**< See #B2P_RSP_CASE_MGR_STATE_LOCK_GET_s. */
#define B2P_CMD_IED_STATE_SET           0x0490 /**< See #B2P_CMD_IED_STATE_SET_s. */
#define B2P_RSP_IED_STATE_SET           0x0491
#define B2P_CMD_STILL_STATE_SET         0x0492 /**< See #B2P_CMD_STILL_STATE_SET_s. */
#define B2P_RSP_STILL_STATE_SET         0x0493
#define B2P_CMD_LOCATION_MGR_STATE_GET  0x0494
#define B2P_RSP_LOCATION_MGR_STATE_GET  0x0495 /**< See #B2P_RSP_LOCATION_MGR_STATE_GET_s. */


/** @} */
/*********
 * ENUMs *
 *********/

/**
 * @brief Pairing State Enable.
 */
typedef enum
{
  B2P_PAIRING_STATE_disable,
  B2P_PAIRING_STATE_enable,
} B2P_PAIRING_STATE_e;

/**
 * @brief Radio State Enable.
 */
typedef enum
{
  B2P_RADIO_STATE_disable,
  B2P_RADIO_STATE_enable,
} B2P_RADIO_STATE_e;

/**
 * @brief Radio State Enable.
 */
typedef enum
{
  B2P_ROLE_STATE_secondary,
  B2P_ROLE_STATE_primary,
} B2P_ROLE_STATE_e;

/**
 * @brief bud status.
 */
typedef enum
{
  B2P_BUD_STATUS_not_present,
  B2P_BUD_STATUS_connected,
  B2P_BUD_STATUS_retrying,
  B2P_BUD_STATUS_cap_detect,
  B2P_BUD_STATUS_deep_sleep,
  B2P_BUD_STATUS_uvp
} B2P_BUD_STATUS_e;

/**
 * @brief bud coupling status.
 */
typedef enum
{
  B2P_COUPLE_START_STATUS_success,
  B2P_COUPLE_START_STATUS_abort
} B2P_COUPLE_START_STATUS_e;

/**
 * @brief bud communication levels.
 */
typedef enum
{
  B2P_BUD_COMMS_LVL_off,
  B2P_BUD_COMMS_LVL_comms_only,
  B2P_BUD_COMMS_LVL_charging,
} B2P_BUD_COMMS_LVL_e;

/**
 * @brief Data Session IDs.
 */
typedef enum
{
  B2P_DS_ID_platform_start = B2P_DS_SESSION_ID_common_end,
  B2P_DS_ID_fwup_host_to_bud_left = B2P_DS_ID_platform_start,
  B2P_DS_ID_fwup_host_to_bud_right,
  B2P_DS_ID_fwup_host_to_case,
  B2P_DS_ID_dfu_bud_left,
  B2P_DS_ID_dfu_bud_right,
  B2P_DS_ID_case_logdump,
  B2P_DS_ID_bud_logdump_left,
  B2P_DS_ID_bud_logdump_right,
  B2P_DS_ID_fwup_bud_to_bud_srv_left,
  B2P_DS_ID_fwup_bud_to_bud_srv_right,
  B2P_DS_ID_fwup_bud_to_case_srv_left,
  B2P_DS_ID_fwup_bud_to_case_srv_right,
  B2P_DS_ID_case_log_upload,
} B2P_DS_ID_e;

/**
 * @brief Bud side.
 */
typedef enum
{
  B2P_BUD_SIDE_unknown,
  B2P_BUD_SIDE_right,
  B2P_BUD_SIDE_left
} B2P_BUD_SIDE_e;

/**
 * @brief System FWUP state.
 */
typedef enum
{
  B2P_SYS_FWUP_STATE_idle,
  B2P_SYS_FWUP_STATE_bud_reacquiring,

  // Bud to Bud
  B2P_SYS_FWUP_STATE_b2b_downloading_l,
  B2P_SYS_FWUP_STATE_b2b_downloading_r,
  B2P_SYS_FWUP_STATE_b2b_installing,

  // Bud to Case
  B2P_SYS_FWUP_STATE_b2c_downloading_l,
  B2P_SYS_FWUP_STATE_b2c_downloading_r,
  B2P_SYS_FWUP_STATE_b2c_installing,

  // Host to bud
  B2P_SYS_FWUP_STATE_h2b_downloading_l,
  B2P_SYS_FWUP_STATE_h2b_downloading_r,

  // Host to case
  B2P_SYS_FWUP_STATE_h2c_downloading,
} B2P_SYS_FWUP_STATE_e;

/**
 * @brief BT UTP state
 */
typedef enum
{
  B2P_BT_UTP_STATE_not_connected = 0,
  B2P_BT_UTP_STATE_connected,
} B2P_BT_UTP_STATE_e;

/**
 * @brief Button modes. Aligned with AppleAPHostApp eButtonMode
 */
typedef enum
{
  B2P_BUTTON_MODE_UNKNOWN         = 0x00,
  B2P_BUTTON_MODE_SIRI            = 0x01,
  B2P_BUTTON_MODE_PLAY_PAUSE      = 0x02,
  B2P_BUTTON_MODE_NEXT            = 0x03,
  B2P_BUTTON_MODE_PREV            = 0x04,
  B2P_BUTTON_MODE_ANC             = 0x05,
  B2P_BUTTON_MODE_VOL_UP          = 0x06,
  B2P_BUTTON_MODE_VOL_DOWN        = 0x07,
  B2P_BUTTON_MODE_OFF             = 0x7F,
} B2P_BUTTON_MODE_e;

/**
 * @brief Bud location.
 */
typedef enum
{
  B2P_BUD_LOCATION_INEAR,
  B2P_BUD_LOCATION_OUT,
  B2P_BUD_LOCATION_INCASE,
  B2P_BUD_LOCATION_UNKNOWN
} B2P_BUD_LOCATION_e;

/**
 * @brief One Bud ANC Mode values
 */
typedef enum
{
  B2P_ONE_BUD_ANC_MODE_disable,
  B2P_ONE_BUD_ANC_MODE_enable
} B2P_ONE_BUD_ANC_MODE_e;

/**
 * @brief Bud dock States.
 */
typedef enum
{
  B2P_BUD_DOCK_STATE_unknown,
  B2P_BUD_DOCK_STATE_undocked,
  B2P_BUD_DOCK_STATE_docked,
  B2P_BUD_DOCK_STATE_docked_dead
} B2P_BUD_DOCK_STATE_e;

/**
 * @brief Bud lid States.
 */
typedef enum
{
  B2P_BUD_LID_STATE_unknown,
  B2P_BUD_LID_STATE_closed,
  B2P_BUD_LID_STATE_open
} B2P_BUD_LID_STATE_e;

/**
 * @brief Bud case manager state lock.
 */
typedef enum
{
  B2P_CASE_MGR_STATE_LOCK_disable,
  B2P_CASE_MGR_STATE_LOCK_enable
} B2P_CASE_MGR_STATE_LOCK_e;


/**
* @brief Bud stem click actions
*/
typedef enum
{
 B2P_STEM_ACTION_UNKNOWN,
 B2P_STEM_ACTION_CLICK_DOWN,
 B2P_STEM_ACTION_LIFT_UP,
} B2P_STEM_ACTION_e;

/**
* @brief Bud stem click gestures
*/
typedef enum
{
 B2P_STEM_GESTURE_UNKNOWN,
 B2P_STEM_GESTURE_SINGLE,
 B2P_STEM_GESTURE_DOUBLE,
 B2P_STEM_GESTURE_TRIPPLE,
 B2P_STEM_GESTURE_CLICK_HOLD,
 B2P_STEM_GESTURE_SWIPE_UP,
 B2P_STEM_GESTURE_SWIPE_DOWN,
}B2P_STEM_GESTURE_e;

/**
 * @brief IED States.
 */
typedef enum
{
  B2P_IED_STATE_in_ear,
  B2P_IED_STATE_out_of_ear
} B2P_IED_STATE_e;

/**
 * @brief Stillness service states.
 */
typedef enum
{
  B2P_STILL_STATE_moving,
  B2P_STILL_STATE_still
} B2P_STILL_STATE_e;

/**
 * @brief Location manager states, correspond to location_manager_state_e
 * in location_manager_public.h
 */
typedef enum
{
  B2P_LOCATION_MGR_STATE_undocked,
  B2P_LOCATION_MGR_STATE_undocked_tabletop,
  B2P_LOCATION_MGR_STATE_undocked_out_of_ear,
  B2P_LOCATION_MGR_STATE_undocked_in_ear,
  B2P_LOCATION_MGR_STATE_docked,
  B2P_LOCATION_MGR_STATE_docked_lid_open,
  B2P_LOCATION_MGR_STATE_docked_lid_closed
} B2P_LOCATION_MGR_STATE_e;

/***********
 * STRUCTs *
 ***********/

/**
 * @brief The response format for sweat detect info.
 */
typedef struct ATTR_PACKED
{
  // Last us counters
  uint16_t lastUsCounter1[2];
  uint16_t lastUsCounter2[2];

  // Min maxes
  uint16_t budDetectMinUsCounter_1[2];
  uint16_t budDetectMaxUsCounter_1[2];
  uint16_t budDetectMinUsCounter_2[2];
  uint16_t budDetectMaxUsCounter_2[2];

  uint16_t noCapMinUsCounter_1[2];
  uint16_t noCapMaxUsCounter_1[2];
  uint16_t noCapMinUsCounter_2[2];
  uint16_t noCapMaxUsCounter_2[2];

  uint16_t sweatMinUsCounter_1[2];
  uint16_t sweatMaxUsCounter_1[2];
  uint16_t sweatMinUsCounter_2[2];
  uint16_t sweatMaxUsCounter_2[2];

  // Number of detections
  uint32_t numSweatDetections[2];
  uint32_t sweatToBudDetectCount[2];

} B2P_RSP_SWEAT_GET_s;

/**
 * @brief The response format for bud/case BATMAN inquiry
 */
typedef struct ATTR_PACKED
{
  // BATMAN
  uint16_t budBatteryVoltage[2];
  int16_t  budBatteryCurrent[2];
  int8_t   budBatteryTemp[2];

  uint32_t caseBatteryVoltage;
  int16_t  caseBatteryCurrent;

  // TORCH
  int32_t tMLB;
  int32_t tBatt;
  int32_t tBattDerived;
  int32_t tMCU;

} B2P_RSP_BATMAN_GET_s;

/**
 * @brief The response format for bud/case CRAP inquiry. Structure is same as struct chargerRegType
 */
typedef struct ATTR_PACKED
{
  uint8_t statusAndShipModeControl;
  uint8_t faultsAndFaultsMask;
  uint8_t TSControlAndFaultsMasks;
  uint8_t fastChargeControl;
  uint8_t terminationAndPreCharge;
  uint8_t batteryVoltageControl;
  uint8_t sysVoutControl;
  uint8_t loadSwitchAndLDOControl;
  uint8_t pushButtonControl;
  uint8_t iLimAndBatteryUvloControl;
  uint8_t voltageBasedBatteryMonitor;
  uint8_t VinDpmAndTimers;
} B2P_RSP_CHARGER_REG_GET_s;


/**
 * @brief The identification response format for info id 2 (DEV).
 */
typedef struct ATTR_PACKED
{
  uint8_t hw_ver[3];
  uint8_t prod_fused;
  uint8_t eu_row_sel;

  uint8_t bud_status_l;                /**< See #B2P_BUD_STATUS_e. */
  uint8_t bdaddr_l[6];                 // [6:6]
  uint8_t hw_ver_l[3];

  uint8_t bud_status_r;                /**< See #B2P_BUD_STATUS_e. */
  uint8_t bdaddr_r[6];                 // [6:6]
  uint8_t hw_ver_r[3];
} B2P_RSP_IDENTIFICATION_DEV_s;

/**
 * @brief The command format for pairing state set
 */
typedef struct ATTR_PACKED
{
  uint8_t discoverable; /**< See #B2P_PAIRING_STATE_e. */
} B2P_CMD_PAIRING_STATE_SET_s;

/**
 * @brief The response format for pairing state get
 */
typedef struct ATTR_PACKED
{
  uint8_t discoverable; /**< See #B2P_PAIRING_STATE_e. */
} B2P_RSP_PAIRING_STATE_GET_s;

/**
 * @brief The command format for radio state set
 */
typedef struct ATTR_PACKED
{
  uint8_t radioState; /**< See #B2P_RADIO_STATE_e. */
} B2P_CMD_RADIO_STATE_SET_s;

/**
 * @brief The response format for radio state get
 */
typedef struct ATTR_PACKED
{
  uint8_t radioState; /**< See #B2P_RADIO_STATE_e. */
} B2P_RSP_RADIO_STATE_GET_s;

/**
 * @brief The command format for battery status sync
 */
typedef struct ATTR_PACKED
{
  uint8_t caseBatteryLevel;
  uint8_t caseChargeState;
  uint8_t caseLedStatus;
} B2P_CMD_BATTERY_STATUS_SYNC_s;

/**
 * @brief The response format for battery status sync
 */
typedef struct ATTR_PACKED
{
  uint8_t  budBatteryState;
  uint8_t  budBatteryLevel;
  uint16_t budBatteryVoltage;
  int16_t  budBatteryCurrent;
  int8_t   budBatteryTemperature;
} B2P_RSP_BATTERY_STATUS_SYNC_s;

/**
 * @brief The response format for bud system info get
 */
typedef struct ATTR_PACKED
{
  uint8_t caseStatus;                   // [1:1]
  uint8_t bdaddr[6];                    // [6:7]
  uint8_t coupled_bdaddr[6];            // [6:13]
  uint8_t fw_ver_active[3];             // [3:16]
  uint8_t fw_ver_pending[3];            // [3:19]
  uint8_t hw_ver[3];                    // [3:22]
  uint8_t status1;                      // [1:23]
  uint8_t status2;                      // [1:24]
  uint8_t status3;                      // [1:25]
  uint8_t case_fw_ver_fwup[3];          // [3:28]
  uint8_t active_ver[12];					      // [12:40]
  uint8_t pending_ver[12]; 				      // [12:52]
  uint8_t color_id;                     // [13:13]
  uint8_t rsvd[12];											// [14:65]
  uint8_t budBatteryLevelReceived;      // [1:66]
  uint8_t chargingState;                // [1:67]
} B2P_CMD_BUD_SYSTEM_INFO_SYNC_s;

/**
 * @brief The response format for bud system info get
 */
typedef struct ATTR_PACKED
{
  uint8_t my_bdaddr[6];                 // [6:6]
  uint8_t coupled_bdaddr[6];            // [6:12]
  uint8_t fw_ver_active[3];             // [3:15]
  uint8_t fw_ver_pending[3];            // [3:18]
  uint8_t hw_ver[3];                    // [3:21]
  uint8_t status1;                      // [1:22]
  uint8_t status2;                      // [1:23]
  uint8_t status3;                      // [1:24]
  uint8_t case_fw_ver_fwup[3];          // [3:27]
  uint8_t active_ver[12];               // [12:39]
  uint8_t pending_ver[12];              // [12:51]
  uint8_t color_id;                     // [13:13]
  uint8_t rsvd[12];                     // [14:64]
} B2P_RSP_BUD_SYSTEM_INFO_SYNC_s;

/**
 * @brief The response format for bud couple set
 */
typedef struct ATTR_PACKED
{
  uint8_t my_bdaddr[6];                 // [6:6]
  uint8_t coupled_bdaddr[6];            // [6:12]
  uint8_t fw_ver_active[3];             // [3:15]
  uint8_t fw_ver_pending[3];            // [3:18]
  uint8_t hw_ver[3];                    // [3:21]
  uint8_t status1;                      // [1:22]
  uint8_t status2;                      // [1:23]
  uint8_t status3;                      // [1:24]
  uint8_t case_fw_ver_fwup[3];          // [3:27]
  uint8_t active_ver[12];               // [12:39]
  uint8_t pending_ver[12];              // [12:51]
  uint8_t color_id;                     // [13:13]
  uint8_t rsvd[12];                     // [14:64]
  uint8_t charge_level;
  uint8_t charge_state;
} B2P_CMD_COUPLE_SET_s;

/**
 * @brief The command format for advanced battery status get
 */
typedef struct ATTR_PACKED
{
  uint8_t role; /**< See #B2P_ROLE_STATE_e. */
} B2P_CMD_ROLE_SET_s;

/**
 * @brief The response format for advanced battery status get
 */
typedef struct ATTR_PACKED
{
  uint8_t role; /**< See #B2P_ROLE_STATE_e. */
} B2P_RSP_ROLE_GET_s;

/**
 * @brief The response format for advanced battery status get
 */
typedef struct ATTR_PACKED
{
  uint8_t fw_version[3];
  uint8_t hw_ver[3];
  uint8_t color_id;
} B2P_CMD_SYSTEM_VERSION_SYNC_s;

/**
 * @brief The command format for setting the address.
 */
typedef struct ATTR_PACKED
{
  uint8_t write;
  uint8_t data[B2P_PACKET_MAX_DATA_SIZE-1];
} B2P_CMD_TEST_PARAM_s;

/**
 * @brief The command format for setting the firmware update size.
 */
typedef struct ATTR_PACKED
{
  uint8_t write;
  uint8_t startID;
  uint8_t middleID;
  uint8_t endID;
} B2P_CMD_SET_FWUP_SIZE_s;

/**
 * @brief The command format unknown.
 */
typedef struct ATTR_PACKED
{
  uint8_t data;
} B2P_CMD_AVRCP_s;

/**
 * @brief The command format for unknown.
 */
typedef struct ATTR_PACKED
{
  uint8_t dataStart;
  uint8_t dataEnd;
} B2P_CMD_HFP_s;

/**
 * @brief The command format for entering the test mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t enable;
} B2P_CMD_TEST_ENTER_s;

/**
 * @brief The command format for applying the firmware update.
 */
typedef struct ATTR_PACKED
{
  uint8_t my_bdaddr[6];                 // [6:6]
  uint8_t coupled_bdaddr[6];            // [6:12]
  uint8_t fw_ver_active[3];             // [3:15]
  uint8_t fw_ver_pending[3];            // [3:18]
  uint8_t hw_ver[3];                    // [3:21]
  uint8_t status1;                      // [1:22]
  uint8_t status2;                      // [1:23]
  uint8_t status3;                      // [1:24]
  uint8_t case_fw_ver_fwup[3];          // [3:27]
  uint8_t charge_level;
  uint8_t charge_state;
} B2P_CMD_FWUP_APPLY_s;

/**
 * @brief The command format for unknown.
 */
typedef struct ATTR_PACKED
{
  uint8_t side; /**< See #B2P_BUD_SIDE_e. */
} B2P_CMD_SSIDE_s;

/**
 * @brief The command format for performing a bud reset.
 */
typedef struct ATTR_PACKED
{
  uint8_t  dest; /**< See routing nibble definition. */
  uint32_t delay_ms;   /**< Delay in milliseconds. */
} B2P_CMD_BUD_RESET_s;

/**
 * @brief The response format for system status (polled) info.
 */
typedef struct ATTR_PACKED
{
  uint8_t case_lid_status;
  uint8_t case_soc;
  uint8_t case_charge_state;

  uint8_t bud_status_l; /**< See #B2P_BUD_STATUS_e. */
  uint8_t coupled_bdaddr_l[6];
  uint8_t status1_l;
  uint8_t status2_l;
  uint8_t status3_l;
  uint8_t case_fw_ver_fwup_l[3];
  uint8_t active_ver_l[12];
  uint8_t pending_ver_l[12];
  uint8_t color_id_l;
  uint8_t rsvd_l[12];
  uint8_t bud_soc_l;
  uint8_t bud_charge_state_l;

  uint8_t bud_status_r; /**< See #B2P_BUD_STATUS_e. */
  uint8_t coupled_bdaddr_r[6];
  uint8_t status1_r;
  uint8_t status2_r;
  uint8_t status3_r;
  uint8_t case_fw_ver_fwup_r[3];
  uint8_t active_ver_r[12];
  uint8_t pending_ver_r[12];
  uint8_t color_id_r;
  uint8_t rsvd_r[12];
  uint8_t bud_soc_r;
  uint8_t bud_charge_state_r;
} B2P_RSP_SYS_STATUS_s;

/**
 * @brief The command format for setting test flag.
 */
typedef struct ATTR_PACKED
{
  uint32_t flag;
} B2P_CMD_TFL_SET_s;

/**
 * @brief The response format for getting test flag.
 */
typedef struct ATTR_PACKED
{
  uint32_t flag;
} B2P_RSP_TFL_GET_s;

/**
 * @brief Structure to get the battery info.
 *        should be same as "AACP_BATTERY_INFO"
 */
typedef struct ATTR_PACKED
{
  uint8_t id;      /**< BATT_ID_XXX  */
  uint8_t type;    /**< BATT_TYPE_XXX  */
  uint8_t level;   /**< BATT_LEVEL_XXX or 0-100 value */
  uint8_t state;   /**< BATT_STATE_XXX  */
  uint8_t status;  /**< BATT_STATUS_XXX  */
} B2P_BATTERY_INFO_s;

/**
 * @brief The Response format for getting Battery Info
 *        CNFG_BATTERY_INFO_NUM
 */
typedef struct ATTR_PACKED
{
  B2P_BATTERY_INFO_s battery_info[3];  /**< See #B2P_BATTERY_INFO_s. */
} B2P_RSP_BATTERY_INFO_GET_s;

/**
 * @brief The Response format for getting Case Info
 */
typedef struct ATTR_PACKED
{
  B2P_BATTERY_INFO_s battery_info[3]; /**< See #B2P_BATTERY_INFO_s. */
  uint8_t lid_state;
  uint8_t led_state;
  uint8_t led_color;
  uint8_t is_left_bud_docked;
  uint8_t is_right_bud_docked;
} B2P_RSP_CASE_INFO_GET_s;

/**
 * @brief The notification format  for sending Case Info
 */
typedef struct ATTR_PACKED
{
  B2P_BATTERY_INFO_s battery_info[3]; /**< See #B2P_BATTERY_INFO_s. */
  uint8_t lid_state;
  uint8_t led_state;
  uint8_t led_color;
  uint8_t is_left_bud_docked;
  uint8_t is_right_bud_docked;
} B2P_CMD_CASE_INFO_SEND_s;

/**
 * @brief The command format for sending case serial number.
 */
typedef struct ATTR_PACKED
{
  uint8_t serial_number[B2P_PACKET_MAX_CMD_DATA_SIZE];
} B2P_CMD_CASE_SN_SEND_s;

/**
 * @brief The command format for setting boot mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t bootmode;
} B2P_CMD_BOOTMODE_SET_s;

/**
 * @brief The response format for getting boot mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t bootmode;
} B2P_RSP_BOOTMODE_GET_s;

/**
 * @brief The command format for triggering coupling process.
 */
typedef struct ATTR_PACKED
{
  uint8_t other_bud_bdaddr[6]; /**< BDADDR of other bud */
} B2P_CMD_COUPLE_START_s;

/**
 * @brief The response format for triggering coupling process.
 */
typedef struct ATTR_PACKED
{
  uint8_t status;         /**< See #B2P_COUPLE_START_STATUS_e. */
} B2P_RSP_COUPLE_START_s;

/**
 * @brief The response format for receiving coupling passkey.
 */
typedef struct ATTR_PACKED
{
  uint8_t passkey[1];     /**< Platform-specific passkey length.  */
} B2P_RSP_PASSKEY_GET_s;

/**
 * @brief The command format for sending coupling passkey.
 */
typedef struct ATTR_PACKED
{
  uint8_t passkey[1];     /**< Platform-specific passkey length.  */
} B2P_CMD_PASSKEY_SET_s;

/**
 * @brief The command format for setting bud communication voltage.
 */
typedef struct ATTR_PACKED
{
  uint8_t bud; /**< See routing nibble definition. */
  uint8_t lvl; /**< See #B2P_BUD_COMMS_LVL_e. */
} B2P_CMD_BUD_COMMS_LVL_SET_s;

/**
 * @brief The command format for getting bud communication voltage.
 */
typedef struct ATTR_PACKED
{
  uint8_t bud; /**< See routing nibble definition. */
} B2P_CMD_BUD_COMMS_LVL_GET_s;

/**
 * @brief The response format for getting bud communication voltage.
 */
typedef struct ATTR_PACKED
{
  uint8_t lvl; /**< See #B2P_BUD_COMMS_LVL_e. */
} B2P_RSP_BUD_COMMS_LVL_GET_s;

/**
 * @brief The response format for retrieving the Coupled Bud bluetooth address.
 */
typedef struct ATTR_PACKED
{
  uint8_t bt_addr[B2P_BT_ADDR_LEN];
} B2P_RSP_BT_CADDR_GET_s;

/**
 * @brief The command format for setting the Coupled Bud bluetooth address.
 */
typedef struct ATTR_PACKED
{
  uint8_t bt_addr[B2P_BT_ADDR_LEN];
} B2P_CMD_BT_CADDR_SET_s;

/**
 * @brief The response format for retrieving the Coupled Bud bluetooth info.
 */
typedef struct ATTR_PACKED
{
  uint8_t bt_addr[B2P_BT_ADDR_LEN];
  uint8_t bt_linkkey[B2P_BT_LINKKEY_LEN];
  uint32_t bt_linkkey_type; /**< See #B2P_BT_LINKKEY_TYPE_e. */
} B2P_RSP_BT_CINFO_GET_s;

/**
 * @brief The command format for setting the Coupled Bud bluetooth info.
 */
typedef struct ATTR_PACKED
{
  uint8_t bt_addr[B2P_BT_ADDR_LEN];
  uint8_t bt_linkkey[B2P_BT_LINKKEY_LEN];
  uint32_t bt_linkkey_type; /**< See #B2P_BT_LINKKEY_TYPE_e. */
} B2P_CMD_BT_CINFO_SET_s;

/**
 * @brief The response format for retrieving bud's side.
 */
typedef struct ATTR_PACKED
{
  uint8_t side; /**< See #B2P_BUD_SIDE_e. */
} B2P_RSP_BUD_SIDE_GET_s;

/**
 * @brief The sys data fetch response format.
 */
typedef struct ATTR_PACKED
{
  uint8_t hw_ver_c[3];
  uint8_t prod_fused_c;

  uint8_t mode_l;                      /**< See #B2P_PROTOCOL_MODE_e. */
  uint8_t data_fetched_l;              /**< Boolean value. */
  uint8_t serial_num_l[12];
  uint8_t ap_ecid_l[8];
  uint8_t bdaddr_l[6];                 // [6:6]
  uint8_t hw_ver_l[3];
  uint8_t fuse_l;                      /**< See #B2P_FUSE_STATE_e. */
  uint8_t side_l;                      /**< See #B2P_BUD_SIDE_e. */

  uint8_t mode_r;                      /**< See #B2P_PROTOCOL_MODE_e. */
  uint8_t data_fetched_r;              /**< Boolean value. */
  uint8_t serial_num_r[12];
  uint8_t ap_ecid_r[8];
  uint8_t bdaddr_r[6];                 // [6:6]
  uint8_t hw_ver_r[3];
  uint8_t fuse_r;                      /**< See #B2P_FUSE_STATE_e. */
  uint8_t side_r;                      /**< See #B2P_BUD_SIDE_e. */
} B2P_RSP_SYS_DATA_FETCH_s;

/**
 * @brief The sys ping response format.
 */
typedef struct ATTR_PACKED
{
  uint8_t bud_status_l;                /**< See #B2P_BUD_STATUS_e. */
  uint8_t mode_l;                      /**< See #B2P_PROTOCOL_MODE_e. */
  uint8_t data_fetched_l;              /**< Boolean value. */

  uint8_t bud_status_r;                /**< See #B2P_BUD_STATUS_e. */
  uint8_t mode_r;                      /**< See #B2P_PROTOCOL_MODE_e. */
  uint8_t data_fetched_r;              /**< Boolean value. */
} B2P_RSP_SYS_PING_s;

/**
 * @brief The sys fwup state get response format.
 */
typedef struct ATTR_PACKED
{
  uint8_t state; /**< See #B2P_SYS_FWUP_STATE_e. */
  uint8_t progress; /**< percentage. 0xFF means N/A. */
} B2P_RSP_SYS_FWUP_STATE_GET_s;

/**
 * @brief The bt utp state get response format.
 */
typedef struct ATTR_PACKED
{
  uint8_t state; /**< See #B2P_BT_UTP_STATE_e. */
} B2P_RSP_BT_UTP_STATE_GET_s;

/**
 * @brief The command format for setting button mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t mode_r;     /**< mode for right side. See #B2P_BUTTON_MODE_e. */
  uint8_t mode_l;     /**< mode for left side. See #B2P_BUTTON_MODE_e. */
} B2P_CMD_BUTTON_MODE_SET_s;

/**
 * @brief The response format for getting button mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t mode_r;     /**< mode for right side. See #B2P_BUTTON_MODE_e. */
  uint8_t mode_l;     /**< mode for left side. See #B2P_BUTTON_MODE_e. */
} B2P_RSP_BUTTON_MODE_GET_s;

/**
 * @brief The cmd payload to send to source for seal-setup test
 */
typedef struct ATTR_PACKED
{
  uint8_t opType;   /**< opcode type */
  uint8_t data[];   /**< Arbitrary length payload. */
} B2P_CMD_SEAL_SETUP_SEND_PAYLOAD_s;

/**
 * @brief The cmd payload for seal-setup test
 */
typedef struct ATTR_PACKED
{
  uint8_t opType;   /**< opcode type */
} B2P_CMD_SEAL_SETUP_SET_s;

/**
 * @brief The response format for retrieving bud's location.
 */
typedef struct ATTR_PACKED
{
  uint8_t budLocationStatePrimary;   /**< Location state for primary bud. See #B2P_BUD_LOCATION_e. */
  uint8_t budLocationStateSecondary; /**< Location state for secondary bud. See #B2P_BUD_LOCATION_e. */
} B2P_RSP_BUD_LOCATION_GET_s;

/**
 * @brief The response format for notifying bud's location.
 */
typedef struct ATTR_PACKED
{
  uint8_t budLocationStatePrimary;   /**< Location state for primary bud. See #B2P_BUD_LOCATION_e. */
  uint8_t budLocationStateSecondary; /**< Location state for secondary bud. See #B2P_BUD_LOCATION_e. */
} B2P_CMD_BUD_LOCATION_NOTIFY_s;

/**
 * @brief The response format for getting One Bud ANC Mode
 */
typedef struct ATTR_PACKED
{
  uint8_t anc_mode;     /**< See #B2P_ONE_BUD_ANC_MODE_e. */
} B2P_RSP_ONE_BUD_ANC_MODE_GET_s;

/**
 * @brief The command format for setting One Bud ANC Mode
 */
typedef struct ATTR_PACKED
{
  uint8_t anc_mode;     /**< See #B2P_ONE_BUD_ANC_MODE_e. */
} B2P_CMD_ONE_BUD_ANC_MODE_SET_s;

/**
 * @brief The command format for bud dock state set
 */
typedef struct ATTR_PACKED
{
  uint8_t my_state; /**< See #B2P_BUD_DOCK_STATE_e. */
} B2P_CMD_BUD_DOCK_STATE_SET_s;

/**
 * @brief The response format for bud dock state get
 */
typedef struct ATTR_PACKED
{
  uint8_t my_state; /**< See #B2P_BUD_DOCK_STATE_e. */
} B2P_RSP_BUD_DOCK_STATE_GET_s;

/**
 * @brief The command format for bud lid state set
 */
typedef struct ATTR_PACKED
{
  uint8_t state; /**< See #B2P_BUD_LID_STATE_e. */
} B2P_CMD_BUD_LID_STATE_SET_s;

/**
 * @brief The response format for bud lid state get
 */
typedef struct ATTR_PACKED
{
  uint8_t state; /**< See #B2P_BUD_LID_STATE_e. */
} B2P_RSP_BUD_LID_STATE_GET_s;

/**
 * @brief The command format for bud case manager state lock set
 */
typedef struct ATTR_PACKED
{
  uint8_t lock; /**< See #B2P_CASE_MGR_STATE_LOCK_e. */
} B2P_CMD_CASE_MGR_STATE_LOCK_SET_s;

/**
 * @brief The command format for bud case manager state lock get
 */
typedef struct ATTR_PACKED
{
  uint8_t lock; /**< See #B2P_CASE_MGR_STATE_LOCK_e. */
} B2P_RSP_CASE_MGR_STATE_LOCK_GET_s;

/**
 * @brief The command format for previous stem click action and gesture being set
 */
typedef struct ATTR_PACKED
{
  uint8_t action;  /**< B2P_STEM_ACTION_e.*/
  uint8_t gesture; /**< B2P_STEM_GESTURE_e.*/
} B2P_CMD_STEM_CLICK_SET_s;

/**
 * @brief The command format for previous stem click action and gesture being set
 */
typedef struct ATTR_PACKED
{
  uint8_t action;    /**< B2P_STEM_ACTION_e.*/
  uint8_t gesture;   /**< B2P_STEM_GESTURE_e.*/
} B2P_RSP_STEM_CLICK_GET_s;

/**
 * @brief AWD battery health data (format per batteryAWD_health_logging_t in AudioProductsDrivers)
 */
typedef struct
{
  uint16_t cycle_count;                                                                               // unit = count
  uint16_t time_at_voltage_tempMatrix_h[BATT_HEALTH_NUM_TEMP_RANGES][BATT_HEALTH_NUM_VOLTAGE_RANGES]; // unit = hours
  uint16_t time_charging_total_h;                                                                     // unit = hours
  uint16_t time_charging_wireless_total_h;                                                            // unit = hours
  uint16_t time_charging_topOff_h;                                                                    // unit = hours
  uint16_t time_charging_topOffWireless_h;                                                            // unit = hours
  uint16_t time_charging_topOffWired_h;                                                               // unit = hours
  int16_t  temp_batt_max_dC;                                          // unit = .1 degrees Celsius
  int16_t  temp_batt_avg_dC;                                          // unit = .1 degrees Celsius
  int16_t  temp_batt_min_dC;                                          // unit = .1 degrees Celsius
  int16_t  tempMlb_max_dC;                                            // unit = .1 degrees Celsius
  uint16_t voltage_max_mV;                                            // unit = mV
  uint16_t voltage_min_mV;                                            // unit = mV
  uint16_t capacity_mAh;                                              // unit = microAh
  uint16_t cycle_at_capacity;                                         // unit = count
  uint16_t resistance;                                                // unit = 0.01 Ohms
  int16_t  temp_sip_max;
  int16_t  current_discharge_max_mA;                                  // unit = mA
  uint16_t battery_vendor;
  uint8_t  capacity_milestone_mAh[BATT_HEALTH_MILESTONES];            // unit = mAh
  uint8_t  cycle_count_at_capacity_milestone[BATT_HEALTH_MILESTONES]; // unit = cycles at mA, resolution 6 cycles
  uint8_t  time_in_field_milestone[BATT_HEALTH_MILESTONES];
} B2P_CMD_METRIC_SEND_BATTERY_HEALTH_s;

/**
 * @brief The command format for IED state set
 */
typedef struct ATTR_PACKED
{
  uint8_t state; /**< See #B2P_IED_STATE_e. */
} B2P_CMD_IED_STATE_SET_s;

/**
 * @brief The command format for stillness service state set
 */
typedef struct ATTR_PACKED
{
  uint8_t state; /**< See #B2P_STILL_STATE_e. */
} B2P_CMD_STILL_STATE_SET_s;

/**
 * @brief The response format for getting location manager state
 */
typedef struct ATTR_PACKED
{
  uint8_t state;     /**< See #B2P_LOCATION_MGR_STATE_e. */
} B2P_RSP_LOCATION_MGR_STATE_GET_s;

#endif /* B2P_DICT_UNTETHERED_H_ */
