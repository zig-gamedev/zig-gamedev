/**
 * @file b2p_dict_b419_internal.h
 * @brief B2P Internal Dictionary
 */

#ifndef B2P_DICT_B419_INTERNAL_H_
#define B2P_DICT_B419_INTERNAL_H_

#include <stdint.h>
#include "b2p_dict_b419.h"

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

#define B2P_CMD_SYS_USB_PWR_STATUS                      0x380 /**< See #B2P_CMD_SYS_USB_PWR_STATUS_s. */
#define B2P_RSP_SYS_USB_PWR_STATUS                      0x381
#define B2P_CMD_USB_CONN_STATUS                         0x382 /**< See #B2P_CMD_USB_CONN_STATUS_s. (Deprecated**)*/
#define B2P_RSP_USB_CONN_STATUS                         0x383


/** @} */


/*********
 * ENUMs *
 *********/

/**
 * @brief Different reasons for powering on system
 */
typedef enum
{
  B2P_POWERON_REASON_unknown,
  B2P_POWERON_REASON_userpower_on_hall,
  B2P_POWERON_REASON_userpower_on_charger,
  B2P_POWERON_REASON_fw_update,
  B2P_POWERON_REASON_hard_reset,
  B2P_POWERON_REASON_factory_reset,
  B2P_POWERON_REASON_dev_core_reset,
  B2P_POWERON_REASON_dev_core_crash,
  B2P_POWERON_REASON_LAST
} B2P_POWERON_REASON_e;

/***********
 * STRUCTs *
 ***********/
/**
 * @brief The command format for ST to notify W2 when there is any change in hinge/USB
 */
typedef struct ATTR_PACKED
{
  uint8_t  hinge_status; /**< 0 if hinge is folded/closed, Non-zero if open. */
  uint8_t  usb_status;   /**< 0 if not connected, 1 if connected. */
  uint16_t usb_acc_type; /**< 0 if not an audio device, 1 if it's B442. Potentially extensible */
  uint32_t hinge_open_count; /**< # of times both hinges have been opened */
  uint8_t  mcu_fw_transfer_pending; /**< 1 if MCU fwup transfer is pending, 0 if not. */
} B2P_CMD_SYS_USB_PWR_STATUS_s;

/**
 * @brief The command format for notifying USB insertion/removal.
 */
typedef struct ATTR_PACKED
{
  uint8_t connected; /**< 0 if not connected, 1 if connected. */
  uint8_t acc_type;  /**< 0 if not an audio device, 1 if it's B442. Potentially extensible */
} B2P_CMD_USB_CONN_STATUS_s;

/**
 * @brief The response format from the sub-system to master upon completing booting up.
 */
typedef struct ATTR_PACKED
{
  uint8_t  pwr_on_reason; /**< See B2P_POWERON_REASON_e. */
  uint8_t  audio_ready;  /**< 1 if audio codec has been booted up, 0 if not. */
  uint8_t  hinge_status; /**< 0 if hinge is folded/closed, Non-zero if open. */
  uint8_t  usb_status;   /**< 0 if not connected, 1 if connected. */
  uint16_t usb_acc_type; /**< 0 if not an audio device, 1 if it's B442. Potentially extensible */
  uint32_t hinge_open_count; /**< # of times both hinges have been opened */
  uint8_t  mcu_fw_transfer_pending; /**< 1 if MCU fwup transfer is pending, 0 if not. */
  uint8_t  mcu_cur_ver_major; /**< 16 bits for build version, 8 bits minor and major rev . */
  uint8_t  mcu_cur_ver_minor;
  uint16_t mcu_cur_ver_build;
} B2P_RSP_POWER_ON_COMPLETE_s;


 #endif /* B2P_DICT_B419_INTERNAL_H_ */
