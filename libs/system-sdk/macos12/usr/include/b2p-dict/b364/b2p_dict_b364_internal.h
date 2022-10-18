/**
 * @file b2p_dict_b364_internal.h
 * @brief B2P Internal Dictionary
 */

#ifndef B2P_DICT_B364_INTERNAL_H_
#define B2P_DICT_B364_INTERNAL_H_

#include <stdint.h>
#include "b2p_dict_b364.h"

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

#define B2P_CMD_SYS_USB_PWR_STATUS                                  0x380 /**< See #B2P_CMD_SYS_USB_PWR_STATUS_s. */
#define B2P_RSP_SYS_USB_PWR_STATUS                                  0x381
#define B2P_CMD_PWR_BTN_EVENT                                       0x382 /**< See #B2P_CMD_PWR_BTN_EVENT_s. */
#define B2P_RSP_PWR_BTN_EVENT                                       0x383

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
  B2P_POWERON_REASON_userpower_on_button,
  B2P_POWERON_REASON_userpower_on_charger,
  B2P_POWERON_REASON_fw_update,
  B2P_POWERON_REASON_hard_reset,
  B2P_POWERON_REASON_factory_reset,
  B2P_POWERON_REASON_dev_core_reset,
  B2P_POWERON_REASON_dev_core_crash,
  B2P_POWERON_REASON_LAST
} B2P_POWERON_REASON_e;
 
typedef enum
{
  B2P_POWER_STATE_unknown,
  B2P_POWER_STATE_shutdown,
  B2P_POWER_STATE_idle,
  B2P_POWER_STATE_powered_on
} B2P_POWER_STATE_e;

typedef enum
{
  B2P_PWR_BTN_EVT_invalid,
  B2P_PWR_BTN_EVT_short_press,
  B2P_PWR_BTN_EVT_short_hold,
  B2P_PWR_BTN_EVT_short_release,
  B2P_PWR_BTN_EVT_long_press,
  B2P_PWR_BTN_EVT_long_hold,
  B2P_PWR_BTN_EVT_long_release,
  B2P_PWR_BTN_EVT_vlong_press,
  B2P_PWR_BTN_EVT_vlong_hold,
  B2P_PWR_BTN_EVT_vlong_release,
  B2P_PWR_BTN_EVT_vvlong_press,
  B2P_PWR_BTN_EVT_vvlong_hold,
  B2P_PWR_BTN_EVT_vvlong_release
} B2P_PWR_BTN_EVT_e;

/***********
 * STRUCTs *
 ***********/
/**
 * @brief The command format for ST to notify W2 when there is any change in USB and/or power state
 */
typedef struct ATTR_PACKED
{
  uint8_t  usb_status;    /**< 0 if not connected, 1 if connected. */
  uint8_t  powerMode;     /**< See #B2P_POWER_STATE_e. */
  uint32_t powerOnCount;  /**< # of times the device has been powered on */
  uint8_t  mcu_fw_transfer_pending; /**< 1 if MCU fwup transfer is pending, 0 if not. */
} B2P_CMD_SYS_USB_PWR_STATUS_s;

typedef struct ATTR_PACKED
{
  uint8_t  pwr_on_reason; /**< See #B2P_POWERON_REASON_e. */
  uint8_t  usb_status;    /**< 0 if not connected, 1 if connected. */
  uint8_t  powerMode;     /**< See #B2P_POWER_STATE_e. */
  uint32_t powerOnCount;  /**< # of times the device has been powered on */
  uint8_t  mcu_fw_transfer_pending; /**< 1 if MCU fwup transfer is pending, 0 if not. */
  uint8_t  mcu_cur_ver_major; /**< 16 bits for build version, 8 bits minor and major rev . */
  uint8_t  mcu_cur_ver_minor;
  uint16_t mcu_cur_ver_build;
} B2P_RSP_POWER_ON_COMPLETE_s;

typedef struct ATTR_PACKED
{
  uint8_t event;          /**< See #B2P_PWR_BTN_EVT_e . */
} B2P_CMD_PWR_BTN_EVENT_s;

#endif /* B2P_DICT_B364_INTERNAL_H_ */
