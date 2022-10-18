/**
 * @file b2p_dict_b515.h
 * @brief B2P Dictionary for B515
 */
#pragma once

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
 * @defgroup B2P_CMD_RSP_B515 B2P Command and Response codes for B515
 *
 * @brief Command namespacing
 *        0x400 - ST to ST
 *        0x500 - ST to Hearst
 *        0x600 - Hearst to ST
 *        0x700 - Hearst to Hearst (AOP-AOP)
 *        0x800 - Push Commands
 *        0x900 - Tap to Radar
 */

#define B2P_CMD_LINK_PULSE_CHECK            0x0400
#define B2P_RSP_LINK_PULSE_CHECK            0x0401
#define B2P_CMD_SET_LINK_STATE              0x0402
#define B2P_RSP_SET_LINK_STATE              0x0403
#define B2P_CMD_SET_PRIMARY_STATE           0x0404
#define B2P_RSP_SET_PRIMARY_STATE           0x0405
#define B2P_CMD_RESET_HEARST                0x040A
#define B2P_RSP_RESET_HEARST                0x040B
#define B2P_CMD_RESET_SYSTEM                0x040C
#define B2P_RSP_RESET_SYSTEM                0x040D
#define B2P_CMD_HEARST_STATE_SYNC           0x040E
#define B2P_RSP_HEARST_STATE_SYNC           0x040F
#define B2P_CMD_SET_CHUNNEL                 0x0410
#define B2P_RSP_SET_CHUNNEL                 0x0411
#define B2P_CMD_SYNC_TIME_BYPASS            0x0414
#define B2P_RSP_SYNC_TIME_BYPASS            0x0415
#define B2P_CMD_SYNC_TIME_ACTIVE            0x0416
#define B2P_RSP_SYNC_TIME_ACTIVE            0x0417
#define B2P_CMD_SYNC_VERSION                0x041A
#define B2P_RSP_SYNC_VERSION                0x041B
#define B2P_CMD_PRIMARY_SEND_STATE          0x041C
#define B2P_RSP_PRIMARY_SEND_STATE          0x041D
#define B2P_CMD_FWUP_SCP                    0x041E
#define B2P_RSP_FWUP_SCP                    0x041F
#define B2P_CMD_FWUP_T553                   0x0420
#define B2P_RSP_FWUP_T553                   0x0421

// TODO: Move to tethered dictionary
// <rdar://problem/58849533> Move B515 over to using Tethered B2P Dictionary
#define B2P_CMD_GG_INFO_SET                 0x0436 /**< See #B2P_CMD_GG_INFO_SET_s. */
#define B2P_RSP_GG_INFO_SET                 0x0437
#define B2P_CMD_BATT_HEALTH_SET             0x0438
#define B2P_RSP_BATT_HEALTH_SET             0x0439
#define B2P_CMD_BATT_HEALTH_GET             0x043A
#define B2P_RSP_BATT_HEALTH_GET             0x043B

#define B2P_CMD_FACT_CONNECT                0x0500
#define B2P_RSP_FACT_CONNECT                0x0501
#define B2P_CMD_SET_LEAP_MODE               0x0502
#define B2P_RSP_SET_LEAP_MODE               0x0503
#define B2P_CMD_BELFIELD_ENABLE             0x0504
#define B2P_RSP_BELFIELD_ENABLE             0x0505
#define B2P_CMD_BELFIELD_MIKEY_ID           0x0506
#define B2P_RSP_BELFIELD_MIKEY_ID           0x0507
#define B2P_CMD_ABBOT_ENABLE                0x0508
#define B2P_RSP_ABBOT_ENABLE                0x0509
#define B2P_CMD_UPDATE_STATE_OF_CHARGE      0x050A
#define B2P_RSP_UPDATE_STATE_OF_CHARGE      0x050B
#define B2P_CMD_TOGGLE_PAIRING_MODE         0x050C
#define B2P_RSP_TOGGLE_PAIRING_MODE         0x050D
#define B2P_CMD_SEND_BUTTON_GESTURE         0x050E
#define B2P_RSP_SEND_BUTTON_GESTURE         0x050F
#define B2P_CMD_SEND_CROWN_TICK             0x0510
#define B2P_RSP_SEND_CROWN_TICK             0x0511
#define B2P_CMD_UPDATE_BATTERY_STATE        0x0512
#define B2P_RSP_UPDATE_BATTERY_STATE        0x0513
#define B2P_CMD_SYNC_SYSTEM_STATE           0x0514
#define B2P_RSP_SYNC_SYSTEM_STATE           0x0515
#define B2P_CMD_SEND_ONHEAD_STATE           0x0516
#define B2P_RSP_SEND_ONHEAD_STATE           0x0517
#define B2P_CMD_FWUP_APPLY                  0x0518
#define B2P_RSP_FWUP_APPLY                  0x0519
//  available: 0x051A - 0x051F
#define B2P_CMD_POWER_OFF_PRIMARY_HEARST    0x0520
#define B2P_RSP_POWER_OFF_PRIMARY_HEARST    0x0521
#define B2P_CMD_CLEAR_PAIRING_HISTORY       0x0522
#define B2P_RSP_CLEAR_PAIRING_HISTORY       0x0523
#define B2P_CMD_PREPARE_POWER_CHANGE        0x0524
#define B2P_RSP_PREPARE_POWER_CHANGE        0x0525
#define B2P_CMD_PLAY_UI_TONE                0x0526
#define B2P_RSP_PLAY_UI_TONE                0x0527
#define B2P_CMD_SEND_UI_TONE_TUNING         0x0528
#define B2P_RSP_SEND_UI_TONE_TUNING         0x0529
//  available: 0x052A - 0x052F
#define B2P_CMD_HEARST_ENTER_SLEEP_EVENT    0x0530
#define B2P_RSP_HEARST_ENTER_SLEEP_EVENT    0x0531
#define B2P_CMD_UPDATE_CUSHION_ID           0x0532
#define B2P_RSP_UPDATE_CUSHION_ID           0x0533
#define B2P_CMD_INCASE_STATE_CHANGED        0x0534
#define B2P_RSP_INCASE_STATE_CHANGED        0x0535

#define B2P_CMD_BT_STATE_CHANGED            0x0600
#define B2P_RSP_BT_STATE_CHANGED            0x0601
#define B2P_CMD_PAIRING_MODE_CHANGED        0x0602
#define B2P_RSP_PAIRING_MODE_CHANGED        0x0603
#define B2P_CMD_POWER_OFF_CONFIRMED         0x0604
#define B2P_RSP_POWER_OFF_CONFIRMED         0x0605
#define B2P_CMD_POWER_CHANGE_READY          0x0606
#define B2P_RSP_POWER_CHANGE_READY          0x0607

#define B2P_CMD_PUSH_COMMAND                0x0800
#define B2P_RSP_PUSH_COMMAND                0x0801
#define B2P_CMD_PUSH_DUMP                   0x0802 // TODO: Make these once we need responses from console pushes
#define B2P_RSP_PUSH_DUMP                   0x0803

#define B2P_CMD_AWD_REQUEST_DAILIES         0x0880
#define B2P_RSP_AWD_REQUEST_DAILIES         0x0881
#define B2P_CMD_AWD_DAILY_REPORT            0x0882
#define B2P_RSP_AWD_DAILY_REPORT            0x0883

#define B2P_CMD_TTR_TRIGGER                 0x0900
#define B2P_RSP_TTR_TRIGGER                 0x0901
#define B2P_CMD_TTR_FETCH_RECORD            0x0902
#define B2P_RSP_TTR_FETCH_RECORD            0x0903
#define B2P_CMD_TTR_RECORD                  0x0904
#define B2P_RSP_TTR_RECORD                  0x0905
#define B2P_CMD_TTR_INIT                    0x0906
#define B2P_RSP_TTR_INIT                    0x0907

#define B2P_CMD_OTA_START                   0x0980
#define B2P_RSP_OTA_START                   0x0981
#define B2P_CMD_OTA_STOP                    0x0982
#define B2P_RSP_OTA_STOP                    0x0983

#define B2P_RSP_GENERIC_STATUS              0x0999

/**
 * @brief Describes the type of firmware version being requested
 */
typedef enum {
  B515_VERSION_SCP = 0x0,
  B515_VERSION_HEARST,
  B515_VERSION_T553,
  B515_VERSION_HAMM,
  B515_VERSION_INVALID
} B2P_B515_VERSION_TYPE_e;

// 0x0400 - 0x04FF
typedef struct ATTR_PACKED {
  uint32_t cmdTimestamp;
} B2P_CMD_LINK_PULSE_CHECK_s;

typedef struct ATTR_PACKED {
  uint32_t cmdTimestamp;
  uint32_t rspTimestamp;
} B2P_RSP_LINK_PULSE_CHECK_s;

typedef struct ATTR_PACKED {
  uint8_t state;
} B2P_CMD_SET_LINK_STATE_s;

typedef struct ATTR_PACKED {
  uint8_t state;
} B2P_RSP_SET_LINK_STATE_s;

typedef struct ATTR_PACKED {
  uint8_t state;
} B2P_CMD_SET_PRIMARY_STATE_s;

typedef struct ATTR_PACKED {
  uint8_t state;
} B2P_RSP_SET_PRIMARY_STATE_s;

typedef struct ATTR_PACKED {
  uint32_t key;
} B2P_CMD_RESET_SYSTEM_s;

typedef struct ATTR_PACKED {
  uint8_t chunnelMode;
} B2P_CMD_SET_CHUNNEL_s;

typedef struct ATTR_PACKED {
  uint32_t cmdTimestamp;
} B2P_CMD_SYNC_TIME_BYPASS_s;

typedef struct ATTR_PACKED {
  uint32_t cmdTimestamp;
  uint32_t rspTimestamp;
} B2P_RSP_SYNC_TIME_BYPASS_s;

typedef struct ATTR_PACKED {
  uint32_t cmdTimestamp;
} B2P_CMD_SYNC_TIME_ACTIVE_s;

typedef struct ATTR_PACKED {
  uint8_t state;
} B2P_CMD_PRIMARY_SEND_STATE_s;

typedef struct ATTR_PACKED {
  uint8_t fwType; /**< See #B2P_B515_VERSION_TYPE_e */
} B2P_CMD_SYNC_VERSION_s;

typedef struct ATTR_PACKED {
  uint8_t fwType; /**< See #B2P_B515_VERSION_TYPE_e */
  uint8_t data[B2P_PACKET_MAX_CMD_DATA_SIZE-1];
} B2P_RSP_SYNC_VERSION_s;

/**
 * @brief The command format for setting the tethered power manager's info.
 */
typedef struct ATTR_PACKED
{
  uint16_t voltage;       /**< battery voltage (in mV). */
  uint8_t  soc;           /**< smoothed state-of-charge. */
  uint8_t  raw_soc;       /**< raw state-of-charge. */
  int16_t  current;       /**< charge current (in mA). */
  int16_t  temperature;   /**< temperature of the battery (in 0.1 deg C). */
  uint8_t  fully_charged; /**< boolean indicating whether fully charged. */
} B2P_CMD_GG_INFO_SET_s;

// 0x0500 - 0x05FF
typedef struct ATTR_PACKED {
  uint8_t factConnectStatus;
} B2P_CMD_FACT_CONNECT_s;

typedef struct ATTR_PACKED {
  uint8_t status;
} B2P_RSP_FACT_CONNECT_s;

typedef struct ATTR_PACKED {
  uint8_t factMode;
} B2P_CMD_SET_LEAP_MODE_s;

typedef struct ATTR_PACKED {
  uint8_t status;
} B2P_RSP_SET_LEAP_MODE_s;

typedef struct ATTR_PACKED {
  uint8_t mikeyConnected;
} B2P_CMD_BELFIELD_EVENT_s;

typedef struct ATTR_PACKED {
  uint8_t status;
} B2P_RSP_BELFIELD_EVENT_s;

typedef struct ATTR_PACKED {
  uint16_t mikeyID;
} B2P_CMD_BELFIELD_MIKEY_ID_s;

typedef struct ATTR_PACKED {
  uint8_t status;
} B2P_RSP_BELFIELD_MIKEY_ID_s;

typedef struct ATTR_PACKED {
  uint8_t enable;
} B2P_CMD_ABBOT_ENABLE_s;

typedef struct ATTR_PACKED {
  uint8_t stateOfCharge;
} B2P_CMD_UPDATE_STATE_OF_CHARGE_s;

typedef struct ATTR_PACKED {
  uint8_t pairing;
} B2P_CMD_TOGGLE_PAIRING_MODE_s;

typedef struct ATTR_PACKED {
  uint8_t gesture;
} B2P_CMD_SEND_BUTTON_GESTURE_s;

typedef struct ATTR_PACKED {
  int16_t ticks;
} B2P_CMD_SEND_CROWN_TICK_s;

typedef struct ATTR_PACKED {
  uint8_t tone;
} B2P_CMD_PLAY_UI_TONE_s;

typedef struct ATTR_PACKED {
  uint8_t tuningParam;   //  0: UI tone panning value, 1: UI tone gain
  uint8_t tuningVal;
} B2P_CMD_SEND_UI_TONE_TUNING_s;

typedef struct ATTR_PACKED {
  uint8_t batteryState;
} B2P_CMD_UPDATE_BATTERY_STATE_s;

typedef struct ATTR_PACKED {
  uint8_t systemState;
} B2P_CMD_SYNC_SYSTEM_STATE_s;

typedef struct ATTR_PACKED {
  uint8_t onHead;
} B2P_CMD_SEND_ONHEAD_STATE_s;

typedef struct ATTR_PACKED {
  uint8_t powerOff;
} B2P_CMD_POWER_OFF_PRIMARY_HEARST_s;

typedef struct ATTR_PACKED {
  uint8_t powerState;
} B2P_CMD_PREPARE_POWER_CHANGE_s;

typedef struct ATTR_PACKED {
  uint8_t cushionId;
} B2P_CMD_UPDATE_CUSHION_ID_s;

// 0x0600 - 0x06FF
typedef struct ATTR_PACKED {
  uint8_t btState;
} B2P_CMD_BT_STATE_CHANGED_s;

typedef struct ATTR_PACKED {
  uint8_t pairingMode;
} B2P_CMD_PAIRING_MODE_CHANGED_s;

typedef struct ATTR_PACKED {
  uint8_t powerOffConfirmed;
} B2P_CMD_POWER_OFF_CONFIRMED_s;

typedef struct ATTR_PACKED {
  uint8_t inCase;
} B2P_CMD_INCASE_STATE_CHANGED_s;

// 0x0800 - 0x09FF
typedef struct ATTR_PACKED {
  uint8_t command[100];
  uint8_t length;
} B2P_CMD_PUSH_COMMAND_s;

typedef struct ATTR_PACKED {
  uint8_t status;
} B2P_RSP_PUSH_COMMAND_s;

// 0x900 - 0x090F
typedef struct ATTR_PACKED {
  uint8_t state;
} B2P_CMD_TTR_TRIGGER_s;

typedef struct ATTR_PACKED {
  uint8_t id;  //  0: fetch from primary, 1: fetch from secondary, 2: fetch done
} B2P_CMD_TTR_FETCH_RECORD_s;

typedef struct ATTR_PACKED {
  uint16_t moduleId;
  uint16_t dataLength;
  uint8_t  *data;
} B2P_CMD_TTR_RECORD_s;

typedef struct ATTR_PACKED {
  uint8_t state;
} B2P_CMD_TTR_INIT_s;

typedef struct ATTR_PACKED {
  uint8_t status;
} B2P_RSP_GENERIC_STATUS_s;
