/**
 * @file b2p_dict_internal.h
 * @brief B2P Internal Dictionary
 */

#ifndef B2P_DICT_INTERNAL_H_
#define B2P_DICT_INTERNAL_H_

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
 * @defgroup B2P_INT_CMD_RSP B2P Command and Response codes internal to system.
 * @brief Opcode 0xC00 and on are reserved.
 *
 * @{
 */

#define B2P_CMD_METRIC_SEND                             0x300 /**< See #B2P_CMD_METRIC_SEND_s. */
#define B2P_RSP_METRIC_SEND                             0x301
#define B2P_CMD_METRIC_AVAILABLE                        0x302 /**< See #B2P_CMD_METRIC_AVAILABLE_s. */
#define B2P_RSP_METRIC_AVAILABLE                        0x303 /**< See #B2P_RSP_METRIC_AVAILABLE_s. */
#define B2P_CMD_POWER_OFF                               0x304 /**< See #B2P_CMD_POWER_OFF_s. */
#define B2P_RSP_POWER_OFF                               0x305
#define B2P_CMD_POWER_OFF_REQUEST                       0x306 /**< See #B2P_CMD_POWER_OFF_REQUEST_s. */
#define B2P_RSP_POWER_OFF_REQUEST                       0x307
#define B2P_CMD_POWER_ON_COMPLETE                       0x308 /**< See #B2P_CMD_POWER_ON_COMPLETE_s. */
#define B2P_RSP_POWER_ON_COMPLETE                       0x309 /**< See #B2P_RSP_POWER_ON_COMPLETE_s. Defined per product */
#define B2P_CMD_PROX_STATUS_SHOW                        0x30A
#define B2P_RSP_PROX_STATUS_SHOW                        0x30B
#define B2P_CMD_STORED_LOG_SEND                         0x30C /**< See #B2P_CMD_STORED_LOG_SEND_s. */
#define B2P_RSP_STORED_LOG_SEND                         0x30D

/** @} */


/*********
 * ENUMs *
 *********/

/***********
 * STRUCTs *
 ***********/
/**
 * @brief The command format for sending stored metric data.
 */
typedef struct ATTR_PACKED
{
  uint8_t metric_id; /**< See #B2P_METRIC_ID_e. */
  uint8_t payload[B2P_PACKET_MAX_CMD_DATA_SIZE - sizeof(uint8_t)];
} B2P_CMD_METRIC_SEND_s;

/**
 * @brief The response format for notifying case of bud battery metrics (max/min voltage, temp, current)
 * response to B2P_CMD_METRIC_SEND (0x800) with payload B2P_METRIC_ID_REL_BATT_HISTORY (0x11)
 */
typedef struct ATTR_PACKED
{
  uint16_t minVoltage;
  uint16_t maxVoltage;
  int16_t minCurrent;
  int16_t maxCurrent;
  int8_t minTemp;
  int8_t maxTemp;
  uint8_t isValid;
} B2P_RSP_METRIC_ID_REL_BATT_HISTORY_s;

/**
 * @brief The command format for notifying that case/master has data available.
 */
typedef struct ATTR_PACKED
{
  uint8_t bm_metric_id; /**< Bit mask with bit #B2P_METRIC_ID_e indicating which metrics are available. */
} B2P_CMD_METRIC_AVAILABLE_s;

/**
 * @brief The response format for notifying the case/master which metrics to send.
 */
typedef struct ATTR_PACKED
{
  uint8_t bm_metric_id; /**< Bit mask with bit #B2P_METRIC_ID_e indicating which metrics can be sent now. */
} B2P_RSP_METRIC_AVAILABLE_s;

/**
 * @brief The command format for notifying other MCU sub systems to power off.
 */
typedef struct ATTR_PACKED
{
  uint8_t reason; /**< See #B2P_POWEROFF_REASON_e. */
} B2P_CMD_POWER_OFF_s;

/**
 * @brief The command format for requesting the master sub-system to start power off.
 */
typedef struct ATTR_PACKED
{
  uint8_t reason;
} B2P_CMD_POWER_OFF_REQUEST_s;

/**
 * @brief The command format for notifying the sub-system that master has completed booting up.
 */
typedef struct ATTR_PACKED
{
  uint8_t reason;
  uint16_t rsvd;
  uint8_t  mcu_pending_ver_major; /**< 16 bits for build version, 8 bits minor and major rev . */
  uint8_t  mcu_pending_ver_minor;
  uint16_t mcu_pending_ver_build;
} B2P_CMD_POWER_ON_COMPLETE_s;

/**
 * @brief The command format for sending case/master stored logs.
 */
typedef struct ATTR_PACKED
{
  uint8_t comms_enabled; /**< Tracks if comms was enabled when logs were collected. */
  uint8_t data[]; /**< Arbitrary length data. */
} B2P_CMD_STORED_LOG_SEND_s;

#endif /* B2P_DICT_INTERNAL_H_ */
