/**
 * @file b2p_dict.h
 * @brief B2P Dictionary Version 0.0.140
 */

#ifndef B2P_DICT_H_
#define B2P_DICT_H_

#include <stdint.h>

#ifndef _WIN32
#ifndef ATTR_PACKED
#define ATTR_PACKED   __attribute__((packed))
#endif
#else
#define ATTR_PACKED
#endif

#define B2P_DICTIONARY_VERSION_MAJOR        0
#define B2P_DICTIONARY_VERSION_MINOR        0
#define B2P_DICTIONARY_VERSION_BUILD        140


    /*! B2P packet header size: SoF (2bytes) + Length (2bytes) + SeqNum (1byte) + Opcode (2bytes) + CRC32 (4bytes) */
#define B2P_PACKET_HEADER_SIZE              (11)
    /*! Maximum data size in a B2P packet */
#define B2P_PACKET_MAX_DATA_SIZE            ((1UL << 9) - 1 - B2P_PACKET_HEADER_SIZE)
    /*! Maximum data size in a B2P command/notification packet */
#define B2P_PACKET_MAX_CMD_DATA_SIZE        (B2P_PACKET_MAX_DATA_SIZE)
    /*! Maximum data size in a B2P response packet */
#define B2P_PACKET_MAX_RSP_DATA_SIZE        (B2P_PACKET_MAX_DATA_SIZE - 1)
    /*! Max size for Magic Key */
#define B2P_MAGIC_KEY_BYTE_SIZE             (16)
    /*! Max size for PDL List */
#define B2P_MAX_PAIR_NUM                    (10)
    /*! Length of MAC address. */
#define B2P_BT_ADDR_LEN                     (6)
    /*! 128 bits for security key for a BT pairing. */
#define B2P_BT_LINKKEY_LEN                  (16)
    /*! Max number of supported connections. */
#define B2P_BT_MAX_CONNECTIONS                  (2)
    /*! Macro to get the automation command data size */
#define B2P_AUTOMATION_CMD_DATA_LEN(cmd)    (cmd->size - sizeof(*cmd))
    /*! Macro to get the automation response data size */
#define B2P_AUTOMATION_RSP_DATA_LEN(rsp)    (rsp->size - sizeof(*rsp))


/**
 * @brief The routing nibble options for packet forwarding.
 */
enum
{
/* 0x0 */ B2P_ROUTE_NONE,
/* 0x1 */ B2P_ROUTE_BUD_R,
/* 0x2 */ B2P_ROUTE_BUD_L,
/* 0x3 */ B2P_ROUTE_CASE,
/* 0x4 */ B2P_ROUTE_BT,
/* 0x5 */ B2P_ROUTE_MCU,
/* 0x6    AVAILABLE */
/* 0x7    AVAILABLE */
/* 0x8 */ B2P_ROUTE_LOCUST_HOST = 0x8,
/* 0x9 */ B2P_ROUTE_LOCUST_DEVICE = 0x9,
/* 0xA    AVAILABLE */
/* 0xB    AVAILABLE */
/* 0xC    AVAILABLE */
/* 0xD    AVAILABLE */
/* 0xE */ B2P_ROUTE_AUTOMATION_HOST = 0xE,
/* 0xF */ B2P_ROUTE_HOST = 0xF,
};

/**
 * @defgroup B2P_CMD_RSP B2P Command and Response codes
 *
 * @{
 */
#define B2P_CMD_PING                        0x0000 /**< See #B2P_CMD_PING_s. */
#define B2P_RSP_PING                        0x0001 /**< See #B2P_RSP_PING_s. */
#define B2P_CMD_IDENTIFICATION              0x0002 /**< See #B2P_CMD_IDENTIFICATION_s. */
#define B2P_RSP_IDENTIFICATION              0x0003 /**< See #B2P_RSP_IDENTIFICATION_s. */
#define B2P_CMD_RESET                       0x0008 /**< See #B2P_CMD_RESET_s. */
#define B2P_RSP_RESET                       0x0009 /**< See #B2P_RSP_RESET_s. */
#define B2P_CMD_DFU_VALIDATE                0x000C
#define B2P_RSP_DFU_VALIDATE                0x000D
#define B2P_CMD_DFU_BOOT                    0x000E
#define B2P_RSP_DFU_BOOT                    0x000F
#define B2P_CMD_DS_CONNECT                  0x0010 /**< See #B2P_CMD_DS_CONNECT_s. */
#define B2P_RSP_DS_CONNECT                  0x0011 /**< See #B2P_RSP_DS_CONNECT_s. */
#define B2P_CMD_DS_DATA                     0x0012 /**< See #B2P_CMD_DS_DATA_s. */
#define B2P_RSP_DS_DATA                     0x0013 /**< See #B2P_RSP_DS_DATA_s. */
#define B2P_CMD_I2C_WRITE                   0x0014 /**< See #B2P_CMD_I2C_WRITE_s. */
#define B2P_RSP_I2C_WRITE                   0x0015
#define B2P_CMD_I2C_WRITE_REG8              0x0016 /**< See #B2P_CMD_I2C_WRITE_REG8_s. */
#define B2P_RSP_I2C_WRITE_REG8              0x0017
#define B2P_CMD_I2C_WRITE_REG16             0x0018 /**< See #B2P_CMD_I2C_WRITE_REG16_s. */
#define B2P_RSP_I2C_WRITE_REG16             0x0019
#define B2P_CMD_I2C_READ                    0x001A /**< See #B2P_CMD_I2C_READ_s. */
#define B2P_RSP_I2C_READ                    0x001B /**< See #B2P_RSP_I2C_READ_s. */
#define B2P_CMD_I2C_READ_REG8               0x001C /**< See #B2P_CMD_I2C_READ_REG8_s. */
#define B2P_RSP_I2C_READ_REG8               0x001D /**< See #B2P_RSP_I2C_READ_REG8_s. */
#define B2P_CMD_I2C_READ_REG16              0x001E /**< See #B2P_CMD_I2C_READ_REG16_s. */
#define B2P_RSP_I2C_READ_REG16              0x001F /**< See #B2P_RSP_I2C_READ_REG16_s. */
#define B2P_CMD_CRASH                       0x0020
#define B2P_RSP_CRASH                       0x0021
#define B2P_CMD_DS_CTRL                     0x0022 /**< See #B2P_CMD_DS_CTRL_s. */
#define B2P_RSP_DS_CTRL                     0x0023 /**< See #B2P_RSP_DS_CTRL_s. */
#define B2P_CMD_DFU_STATUS                  0x0024
#define B2P_RSP_DFU_STATUS                  0x0025 /**< See #B2P_RSP_DFU_STATUS_s. */
#define B2P_CMD_DS_DISCOVERY                0x0026
#define B2P_RSP_DS_DISCOVERY                0x0027 /**< See #B2P_RSP_DS_DISCOVERY_s. */
#define B2P_CMD_CBITS_READ                  0x0028 /**< See #B2P_CMD_CBITS_READ_s. */
#define B2P_RSP_CBITS_READ                  0x0029 /**< See #B2P_RSP_CBITS_READ_s. */
#define B2P_CMD_SYNC_TIMESTAMP              0x0098
#define B2P_RSP_SYNC_TIMESTAMP              0x0099 /**< See #B2P_RSP_SYNC_TIMESTAMP_s. */
#define B2P_CMD_LOOPBACK                    0x0100 /**< See #B2P_CMD_LOOPBACK_s. */
#define B2P_RSP_LOOPBACK                    0x0101 /**< See #B2P_CMD_LOOPBACK_s. */
#define B2P_CMD_BERYL_RESETCODE             0x0102 /**< See #B2P_CMD_BERYL_RESETCODE_s. */
#define B2P_RSP_BERYL_RESETCODE             0x0103
#define B2P_CMD_FW_VER_GET                  0x0104
#define B2P_RSP_FW_VER_GET                  0x0105 /**< See #B2P_RSP_FW_VER_GET_s. */
#define B2P_CMD_GPIO_GET                    0x0106 /**< See #B2P_CMD_GPIO_GET_s. */
#define B2P_RSP_GPIO_GET                    0x0107 /**< See #B2P_RSP_GPIO_GET_s. */
#define B2P_CMD_GPIO_SET                    0x0108 /**< See #B2P_CMD_GPIO_SET_s. */
#define B2P_RSP_GPIO_SET                    0x0109
#define B2P_CMD_SYSCFG_PRINT                0x010A /**< See #B2P_CMD_SYSCFG_PRINT_s. */
#define B2P_RSP_SYSCFG_PRINT                0x010B /**< See #B2P_RSP_SYSCFG_PRINT_s. */
#define B2P_CMD_METRIC_GET                  0x010C /**< See #B2P_CMD_METRIC_GET_s. */
#define B2P_RSP_METRIC_GET                  0x010D
#define B2P_CMD_DFU_ENTER                   0x010E /**< Command to put chip in DFU */
#define B2P_RSP_DFU_ENTER                   0x010F
#define B2P_CMD_SHIPPING_ENTER              0x0110
#define B2P_RSP_SHIPPING_ENTER              0x0111
#define B2P_CMD_HW_VER_GET                  0x0112
#define B2P_RSP_HW_VER_GET                  0x0113 /**< See #B2P_RSP_HW_VER_GET_s. */
#define B2P_CMD_SERIAL_NUM_GET              0x0114 /**< See #B2P_CMD_SERIAL_NUM_GET_s. */
#define B2P_RSP_SERIAL_NUM_GET              0x0115 /**< See #B2P_RSP_SERIAL_NUM_GET_s. */
#define B2P_CMD_COLOR_GET                   0x0116
#define B2P_RSP_COLOR_GET                   0x0117 /**< See #B2P_RSP_COLOR_GET_s. */
#define B2P_CMD_PROD_FUSE_GET               0x0118
#define B2P_RSP_PROD_FUSE_GET               0x0119 /**< See #B2P_RSP_PROD_FUSE_GET_s. */
#define B2P_CMD_FWUP_DATA                   0x0120 /**< See #B2P_CMD_FWUP_DATA_s. */
#define B2P_RSP_FWUP_DATA                   0x0121 /**< See #B2P_RSP_FWUP_DATA_s. */
#define B2P_CMD_FWUP_CTRL                   0x0122 /**< See #B2P_CMD_FWUP_CTRL_s. */
#define B2P_RSP_FWUP_CTRL                   0x0123 /**< See #B2P_RSP_FWUP_CTRL_s. */
#define B2P_CMD_RESET_CODE                  0x0124 /**< See #B2P_CMD_RESET_CODE_s. */
#define B2P_RSP_RESET_CODE                  0x0125
#define B2P_CMD_DFU_FORCE                   0x0126 /**< See #B2P_CMD_DFU_FORCE_s. */
#define B2P_RSP_DFU_FORCE                   0x0127
#define B2P_CMD_BVER_GET                    0x0128
#define B2P_RSP_BVER_GET                    0x0129 /**< See #B2P_RSP_BVER_GET_s. */
#define B2P_CMD_FLASH_FLUSH                 0x012A
#define B2P_RSP_FLASH_FLUSH                 0x012B
#define B2P_CMD_BT_NAME_GET                 0x012C
#define B2P_RSP_BT_NAME_GET                 0x012D /**< See #B2P_RSP_BT_NAME_GET_s. */
#define B2P_CMD_FWUP_INFO                   0x012E
#define B2P_RSP_FWUP_INFO                   0x012F /**< See #B2P_RSP_FWUP_INFO_s. */
#define B2P_CMD_WHOLE_BVER_READ             0x0130 /**< See #B2P_CMD_WHOLE_BVER_READ_s. */
#define B2P_RSP_WHOLE_BVER_READ             0x0131 /**< See #B2P_RSP_WHOLE_BVER_READ_s. */
#define B2P_CMD_SOUND_PLAY                  0x0132 /**< See #B2P_CMD_SOUND_PLAY_s. */
#define B2P_RSP_SOUND_PLAY                  0x0133
#define B2P_CMD_BT_NAME_SET                 0x0134 /**< See #B2P_CMD_BT_NAME_SET_s. */
#define B2P_RSP_BT_NAME_SET                 0x0135
#define B2P_CMD_VOLUME_GET                  0x0136
#define B2P_RSP_VOLUME_GET                  0x0137 /**< See #B2P_RSP_VOLUME_GET_s. */
#define B2P_CMD_PROD_CATEGORY_GET           0x0138
#define B2P_RSP_PROD_CATEGORY_GET           0x0139 /**< See #B2P_RSP_PROD_CATEGORY_GET_s. */
#define B2P_CMD_PRODUCT_ID_GET              0x013A
#define B2P_RSP_PRODUCT_ID_GET              0x013B /**< See #B2P_RSP_PRODUCT_ID_GET_s. */
#define B2P_CMD_FWUP_ORIGIN_GET             0x013C
#define B2P_RSP_FWUP_ORIGIN_GET             0x013D /**< SEE #B2P_RSP_FWUP_ORIGIN_GET_s. */
#define B2P_CMD_FLASH_ERASE                 0x013E /**< See #B2P_CMD_FLASH_ERASE_s. */
#define B2P_RSP_FLASH_ERASE                 0x013F
#define B2P_CMD_FLASH_WRITE                 0x0140 /**< See #B2P_CMD_FLASH_WRITE_s. */
#define B2P_RSP_FLASH_WRITE                 0x0141
#define B2P_CMD_FLASH_READ                  0x0142 /**< See #B2P_CMD_FLASH_READ_s. */
#define B2P_RSP_FLASH_READ                  0x0143 /**< See #B2P_RSP_FLASH_READ_s. */
#define B2P_CMD_DEVICE_CAPABILITIES_GET     0x0144
#define B2P_RSP_DEVICE_CAPABILITIES_GET     0x0145 /**< SEE #B2P_RSP_DEVICE_CAPABILITIES_GET_s. */
#define B2P_CMD_POP_SET                     0x0146 /**< See #B2P_CMD_POP_SET_s. */
#define B2P_RSP_POP_SET                     0x0147
#define B2P_CMD_POP_GET                     0x0148
#define B2P_RSP_POP_GET                     0x0149 /**< See #B2P_RSP_POP_GET_s. */
#define B2P_CMD_AUTO_INEAR_DETECTION_SET    0x014A /**< SEE #B2P_CMD_AUTO_INEAR_DETECTION_SET_s. */
#define B2P_RSP_AUTO_INEAR_DETECTION_SET    0x014B
#define B2P_CMD_AUTO_INEAR_DETECTION_GET    0x014C
#define B2P_RSP_AUTO_INEAR_DETECTION_GET    0x014D /**< SEE #B2P_RSP_AUTO_INEAR_DETECTION_GET_s. */
#define B2P_CMD_MIC_MODE_SET                0x014E /**< SEE #B2P_CMD_MIC_MODE_SET_s. */
#define B2P_RSP_MIC_MODE_SET                0x014F
#define B2P_CMD_MIC_MODE_GET                0x0150
#define B2P_RSP_MIC_MODE_GET                0x0151 /**< SEE #B2P_RSP_MIC_MODE_GET_s. */
#define B2P_CMD_BATT_GET                    0x0152
#define B2P_RSP_BATT_GET                    0x0153 /**< See #B2P_RSP_BATT_GET_s. */
#define B2P_CMD_FACTORY_RESET               0x0154
#define B2P_RSP_FACTORY_RESET               0x0155
#define B2P_CMD_HS_GET_HASH                 0x0156 /**< See #B2P_CMD_HS_GET_HASH_s. */
#define B2P_RSP_HS_GET_HASH                 0x0157 /**< See #B2P_RSP_HS_GET_HASH_s. */
#define B2P_CMD_LISTENING_MODE_LEVEL_GET    0x0158 /**< See #B2P_CMD_LISTENING_MODE_LEVEL_GET_s. */
#define B2P_RSP_LISTENING_MODE_LEVEL_GET    0x0159 /**< See #B2P_RSP_LISTENING_MODE_LEVEL_GET_s. */
#define B2P_CMD_LISTENING_MODE_LEVEL_SET    0x015A /**< See #B2P_CMD_LISTENING_MODE_LEVEL_SET_s. */
#define B2P_RSP_LISTENING_MODE_LEVEL_SET    0x015B
#define B2P_CMD_AUTH                        0x015C /**< See #B2P_CMD_AUTH_s. */
#define B2P_RSP_AUTH                        0x015D
#define B2P_CMD_LISTENING_MODE_LEVEL_NOTIFY 0x015E /**< See #B2P_CMD_LISTENING_MODE_LEVEL_NOTIFY_s. */
#define B2P_RSP_LISTENING_MODE_LEVEL_NOTIFY 0x015F
#define B2P_CMD_MAGIC_KEY_GET               0x0160 /**< See #B2P_CMD_MAGIC_KEY_GET_s. */
#define B2P_RSP_MAGIC_KEY_GET               0x0161 /**< See #B2P_RSP_MAGIC_KEY_GET_s. */
#define B2P_CMD_BATT_INFO_NOTIFY            0x0162 /**< See #B2P_CMD_BATT_INFO_NOTIFY_s. */
#define B2P_RSP_BATT_INFO_NOTIFY            0x0163
#define B2P_CMD_BT_ADDR_GET                 0x0164
#define B2P_RSP_BT_ADDR_GET                 0x0165 /**< See #B2P_RSP_BT_ADDR_GET_s. */
#define B2P_CMD_PMU_INFO_GET                0x0166
#define B2P_RSP_PMU_INFO_GET                0x0167 /**< See #B2P_RSP_PMU_INFO_GET_s. */
#define B2P_CMD_NV_GET                      0x0168 /**< See #B2P_CMD_NV_GET_s. */
#define B2P_RSP_NV_GET                      0x0169
#define B2P_CMD_NV_SET                      0x016A /**< See #B2P_CMD_NV_SET_s. */
#define B2P_RSP_NV_SET                      0x016B
#define B2P_CMD_LOG_DATA_SEND               0x016C /**< See #B2P_CMD_LOG_DATA_SEND_s. */
#define B2P_RSP_LOG_DATA_SEND               0x016D /**< See #B2P_RSP_LOG_DATA_SEND_s. */
#define B2P_CMD_SLEEP_COUNTS_GET            0x016E
#define B2P_RSP_SLEEP_COUNTS_GET            0x016F /**< See #B2P_RSP_SLEEP_COUNTS_GET_s. */
#define B2P_CMD_FLASH_ACTIVE_BANK_GET       0x0170
#define B2P_RSP_FLASH_ACTIVE_BANK_GET       0x0171 /**< See #B2P_RSP_FLASH_ACTIVE_BANK_GET_s. */
#define B2P_CMD_FLASH_BANK_SWITCH           0x0172 /**< See #B2P_CMD_FLASH_BANK_SWITCH_s. */
#define B2P_RSP_FLASH_BANK_SWITCH           0x0173
#define B2P_CMD_AUTO_HEAL_SET               0x0174 /**< See #B2P_CMD_AUTO_HEAL_SET_s. */
#define B2P_RSP_AUTO_HEAL_SET               0x0175
#define B2P_CMD_AUTO_HEAL_GET               0x0176
#define B2P_RSP_AUTO_HEAL_GET               0x0177 /**< See #B2P_RSP_AUTO_HEAL_GET_s. */
#define B2P_CMD_LINKKEYS_GET_ALL            0x0178
#define B2P_RSP_LINKKEYS_GET_ALL            0x0179 /**< See #B2P_RSP_LINKKEYS_GET_ALL_s. */
#define B2P_CMD_AACP_PROFILE_STATE_GET      0x017A
#define B2P_RSP_AACP_PROFILE_STATE_GET      0x017B /**< See #B2P_RSP_AACP_PROFILE_STATE_GET_s. */
#define B2P_CMD_LINKKEY_DELETE              0x017C /**< See #B2P_CMD_LINKKEY_DELETE_s. */
#define B2P_RSP_LINKKEY_DELETE              0x017D
#define B2P_CMD_LINKKEYS_DELETE_ALL         0x017E
#define B2P_RSP_LINKKEYS_DELETE_ALL         0x017F
#define B2P_CMD_LINKKEY_GET                 0x0180 /**< See #B2P_CMD_LINKKEY_GET_s. */
#define B2P_RSP_LINKKEY_GET                 0x0181 /**< See #B2P_RSP_LINKKEY_GET_s. */
#define B2P_CMD_UPTIME_GET                  0x0182
#define B2P_RSP_UPTIME_GET                  0x0183 /**< See #B2P_RSP_UPTIME_GET_s. */
#define B2P_CMD_AUTOMATION                  0x0184 /**< See #B2P_AUTOMATION_CMD_PAYLOAD_s. */
#define B2P_RSP_AUTOMATION                  0x0185 /**< See #B2P_AUTOMATION_RSP_PAYLOAD_s. */
#define B2P_CMD_GG_CFG_VER_GET              0x0186
#define B2P_RSP_GG_CFG_VER_GET              0x0187 /**< See #B2P_RSP_GG_CFG_VER_GET_s. */
#define B2P_CMD_USB_MODE_SET                0x0188 /**< See #B2P_CMD_USB_MODE_SET_s. */
#define B2P_RSP_USB_MODE_SET                0x0189
#define B2P_CMD_LISTENING_MODE_ROTATION_GET 0x018A
#define B2P_RSP_LISTENING_MODE_ROTATION_GET 0x018B /**< See #B2P_RSP_LISTENING_MODE_ROTATION_GET_s. */
#define B2P_CMD_LISTENING_MODE_ROTATION_SET 0x018C /**< See #B2P_CMD_LISTENING_MODE_ROTATION_SET_s. */
#define B2P_RSP_LISTENING_MODE_ROTATION_SET 0x018D
#define B2P_CMD_TRAIN_BUILD_GET             0x018E
#define B2P_RSP_TRAIN_BUILD_GET             0x018F /**< See #B2P_RSP_TRAIN_BUILD_GET_s. */
#define B2P_CMD_BLUE_AVENGERS_CONFIG        0x0190 /**< See #B2P_CMD_BLUE_AVENGERS_CONFIG_s. */
#define B2P_RSP_BLUE_AVENGERS_CONFIG        0x0191 /**< See #B2P_RSP_BLUE_AVENGERS_CONFIG_s. */
#define B2P_CMD_BT_SNIFF_INFO_GET           0x0192
#define B2P_RSP_BT_SNIFF_INFO_GET           0x0193 /**< See #B2P_RSP_BT_SNIFF_INFO_GET_s. */
#define B2P_CMD_AUDIO_STATUS_GET            0x0194
#define B2P_RSP_AUDIO_STATUS_GET            0x0195 /**< See #B2P_RSP_AUDIO_STATUS_GET_s. */
#define B2P_CMD_BT_SNOOP_INFO_GET           0x0196
#define B2P_RSP_BT_SNOOP_INFO_GET           0x0197 /**< See #B2P_RSP_BT_SNOOP_INFO_GET_s. */

// reserve 0xF00-0xFFD for platform/silicon specific B2P
#define B2P_CMD_LOCUST_SET_LOW_RDSON        0x0F00 /**< See #B2P_CMD_LOCUST_SET_LOW_RDSON. */
#define B2P_RSP_LOCUST_SET_LOW_RDSON        0x0F01 /**< See #B2P_RSP_LOCUST_SET_LOW_RDSON. */

// reserved opcodes for B2P routing
#define B2P_CMD_ROUTED_RESERVED             0x0FFE /* See B2P routing spec */
#define B2P_RSP_ROUTED_RESERVED             0x0FFF /* See B2P routing spec */

/** @} */


/*********
 * ENUMs *
 *********/

/**
 * @brief Protocol version.
 */
typedef enum
{
  B2P_PROTOCOL_VERSION_reserved,
  B2P_PROTOCOL_VERSION_buddy,
  B2P_PROTOCOL_VERSION_b2p,
} B2P_PROTOCOL_VERSION_e;

/**
 * @brief Protocol mode.
 */
typedef enum
{
  B2P_PROTOCOL_MODE_secureboot,
  B2P_PROTOCOL_MODE_yboot,
  B2P_PROTOCOL_MODE_app,
  B2P_PROTOCOL_MODE_diag, // reserved for factory diag image
  B2P_PROTOCOL_MODE_unknown = 255,
} B2P_PROTOCOL_MODE_e;

/**
 * @brief Reset mode.
 */
typedef enum
{
  B2P_RESET_MODE_sw,
  B2P_RESET_MODE_chip,
  B2P_RESET_MODE_system
} B2P_RESET_MODE_e;

/**
 * @brief Info ID.
 */
typedef enum
{
  B2P_INFO_ID_basic,
  B2P_INFO_ID_sn,
  B2P_INFO_ID_dev,
  B2P_INFO_ID_silicon
} B2P_INFO_ID_e;

/**
 * @brief DataSessions session ID.
 */
typedef enum
{
  B2P_DS_SESSION_ID_test_session,
  B2P_DS_SESSION_ID_airpods_fwup_iaup_server_session,
  B2P_DS_SESSION_ID_airpods_fwup_iaup_client_session,
  B2P_DS_SESSION_ID_airpodscase_fwup_iaup_server_session,
  B2P_DS_SESSION_ID_dfu_image,
  B2P_DS_SESSION_ID_dfu_manifest,
  B2P_DS_SESSION_ID_common_end = 0x10
} B2P_DS_SESSION_ID_e;

/**
 * @brief DataSessions session ID.
 */
typedef enum
{
  B2P_DS_STATE_close,
  B2P_DS_STATE_open,
} B2P_DS_STATE_e;

/**
 * @brief Metric ID.
 */
typedef enum
{
  B2P_METRIC_ID_AWD_LINK_INFO,
  B2P_METRIC_ID_AWD_DAILY_USAGE,
  B2P_METRIC_ID_AWD_USER_GESTURE,
  B2P_METRIC_ID_AWD_CRASH,
  B2P_METRIC_ID_AWD_CASE,
  B2P_METRIC_ID_AWD_BATT_HEALTH,
  B2P_METRIC_ID_REL_CONNECT_TIME = 0x10,
  B2P_METRIC_ID_REL_BATT_HISTORY,
} B2P_METRIC_ID_e;

/**
 * @brief B2P serial number components.
 */
typedef enum
{
  B2P_SN_MLB,
  B2P_SN_SYSTEM,
  B2P_SN_BATTERY,
  B2P_SN_POWER,
  B2P_SN_USB,
  B2P_SN_SYSTEM_CONFIG,
  B2P_SN_DRIVER_00 = 0x10,
  B2P_SN_END
} B2P_SN_e;

/**
 * @brief B2P FWUP control Commands.
 */
typedef enum
{
  B2P_FWUP_STATUS,
  B2P_FWUP_START,
  B2P_FWUP_STOP,
  B2P_FWUP_MANIFEST,
  B2P_FWUP_START_EX,
  B2P_FWUP_REQ,
  B2P_FWUP_READY
} B2P_FWUP_CTRL_CMD_e;

/**
 * @brief B2P FWUP Control Start session types.
 */
typedef enum
{
  B2P_FWUP_CTRL_START_SESSION_TYPE_BUD_TO_BUD_SERVER,
  B2P_FWUP_CTRL_START_SESSION_TYPE_BUD_TO_CASE_SERVER,
  B2P_FWUP_CTRL_START_SESSION_TYPE_BUD_TO_BUD_CLIENT,
  B2P_FWUP_CTRL_START_SESSION_TYPE_BUD_TO_CASE_CLIENT,
  B2P_FWUP_CTRL_START_SESSION_TYPE_CASE_TO_BUD_SERVER,
  B2P_FWUP_CTRL_START_SESSION_TYPE_CASE_TO_BUD_CLIENT,
  B2P_FWUP_CTRL_START_SESSION_TYPE_HOST_TO_BUD_SERVER,
  B2P_FWUP_CTRL_START_SESSION_TYPE_HOST_TO_CASE_SERVER,
  B2P_FWUP_CTRL_START_SESSION_TYPE_HOST_TO_BUD_CLIENT,
  B2P_FWUP_CTRL_START_SESSION_TYPE_HOST_TO_CASE_CLIENT
} B2P_FWUP_CTRL_START_SESSION_TYPE_e;

/**
 * @brief B2P FWUP Control Start EX partition.
 */
typedef enum
{
  B2P_FWUP_CTRL_START_EX_PARTITION_AUTO,
  B2P_FWUP_CTRL_START_EX_PARTITION_1,
  B2P_FWUP_CTRL_START_EX_PARTITION_2,
} B2P_FWUP_CTRL_START_EX_PARTITION_e;

/**
 * @brief B2P FWUP possible transports.
 */
typedef enum
{
  B2P_FWUP_TRANSPORT_BUDDY,
  B2P_FWUP_TRANSPORT_AACP,
  B2P_FWUP_TRANSPORT_UTP,
  B2P_FWUP_TRANSPORT_B2P_HID,
  B2P_FWUP_TRANSPORT_B2P_SPP,
} B2P_FWUP_TRANSPORT_e;

/**
 * @brief FW Partition.
 */
typedef enum
{
  B2P_FW_PARTITION_ACTIVE,
  B2P_FW_PARTITION_INACTIVE,
} B2P_FW_PARTITION_e;

/**
 * @brief Product category.
 */
typedef enum
{
  B2P_PROD_CATEGORY_untethered,
  B2P_PROD_CATEGORY_tethered,
} B2P_PROD_CATEGORY_e;

/**
 * @brief Firmware bundle origin.
 */
typedef enum
{
  B2P_FWUP_ORIGIN_none,
  B2P_FWUP_ORIGIN_hid,
  B2P_FWUP_ORIGIN_ota
} B2P_FWUP_ORIGIN_e;

/**
 * @brief FWUP stages.
 */
typedef enum
{
  B2P_FWUP_STAGE_none,
  B2P_FWUP_STAGE_host_to_bt,
  B2P_FWUP_STAGE_bt_to_bt,
  B2P_FWUP_STAGE_bt_to_mcu
} B2P_FWUP_STAGE_e;

/**
 * @brief FWUP error codes.
 */
typedef enum
{
  B2P_FWUP_ERROR_none,
  B2P_FWUP_ERROR_transport_error,
  B2P_FWUP_ERROR_timeout
} B2P_FWUP_ERROR_e;

/**
 * @brief Device Cap information.
 */
typedef enum
{
  B2P_BM_DEVICE_CAP_DEVICE_RENAME,            //Bit 0
  B2P_BM_DEVICE_CAP_DEVICE_USER_FW_UPDATE,    //Bit 1
  B2P_BM_DEVICE_CAP_DEVICE_SILIENT_FW_UPDATE, //Bit 2
  B2P_BM_DEVICE_CAP_DEVICE_BATT_INFO,         //Bit 3
  B2P_BM_DEVICE_CAP_DEVICE_CASE_INFO,         //Bit 4
  B2P_BM_DEVICE_CAP_DEVICE_LISTENING_MODE,    //Bit 5
  B2P_BM_DEVICE_CAP_DEVICE_AWARENESS,         //Bit 6
  B2P_BM_DEVICE_CAP_DEVICE_TURN_ON_OF_SOUNDS, //Bit 7
  B2P_BM_DEVICE_CAP_DEVICE_AMPLIFY_MODE,      //Bit 8
  B2P_BM_DEVICE_CAP_DEVICE_STEREO_MODE,       //Bit 9
  B2P_BM_DEVICE_CAP_DEVICE_DJ_MODE            //bit 10
}B2P_BM_DEVICE_CAP_e;

/**
 * @brief POP Mode enum.
 */
typedef enum
{
  B2P_POP_MODE_unknown,
  B2P_POP_MODE_DISABLED,
  B2P_POP_MODE_ENABLED,
  B2P_POP_MODE_LAST,
} B2P_POP_MODE_e;

/**
 * @brief Automatic InEar Detection information.
 */
typedef enum
{
  B2P_AUTO_INEAR_DETECTION_UNKNOWN,
  B2P_AUTO_INEAR_DETECTION_DISABLE,
  B2P_AUTO_INEAR_DETECTION_ENABLE,
  B2P_AUTO_INEAR_DETECTION_LAST
} B2P_AUTO_INEAR_DETECTION_e;

/**
 * @brief Automatic Mic Mode Information.
 */
typedef enum
{
  B2P_MIC_MODE_AUTO,
  B2P_MIC_MODE_FIXED_RIGHT,
  B2P_MIC_MODE_FIXED_LEFT
} B2P_MIC_MODE_e;

/**
 * @brief BatteryState.
 */
typedef enum
{
  B2P_BATTERY_STATE_unknown,
  B2P_BATTERY_STATE_discharging,
  B2P_BATTERY_STATE_charging,
  B2P_BATTERY_STATE_charged
} B2P_BATTERY_STATE_e;

/**
 * @brief Listening mode.
 */
typedef enum
{
  B2P_LISTENING_MODE_DISABLE_ALL,
  B2P_LISTENING_MODE_ANC,
  B2P_LISTENING_MODE_TRANSPARENCY
} B2P_LISTENING_MODE_LEVEL_e;

/**
 * @brief Listening mode: ANC enable
 */
typedef enum
{
  B2P_ANC_OFF = 0,
  B2P_ANC_ON = 100,
} B2P_ANC_ENABLE_e;

/**
 * @brief Listening mode: AWARE enable
 */
typedef enum
{
  B2P_AWARE_OFF = 0,
  B2P_AWARE_ON = 100,
} B2P_AWARE_ENABLE_e;

/**
 * @brief Listening mode rotation (aka configs on aacp side) values
 */
typedef enum
{
  B2P_LISTENING_MODE_CONFIGS_BITMASK_NONE            = 0x00000000,
  B2P_LISTENING_MODE_CONFIGS_BITMASK_NORMAL          = 0x00000001,
  B2P_LISTENING_MODE_CONFIGS_BITMASK_ANC             = 0x00000002,
  B2P_LISTENING_MODE_CONFIGS_BITMASK_TRANSPARENCY    = 0x00000004
} B2P_LISTENING_MODE_ROTATION_e;

/**
 * @brief NV type.
 */
typedef enum
{
  B2P_NV_TYPE_SCALAR,
  B2P_NV_TYPE_STRING,
  B2P_NV_TYPE_BINARY_BLOB
} B2P_NV_TYPE_e;

/**
 * @brief USB Mode.
 */
typedef enum
{
  B2P_USB_MODE_DFU,
  B2P_USB_MODE_APP,
  B2P_USB_MODE_APP_RESET, /* Enters app and resets devie. */
} B2P_USB_MODE_e;

/**
 * @brief LOG Message types,
 * should be same as AACP message types.
 */
typedef enum
{
  B2P_LOG_TYPE_CRASH           = 0x01,
  B2P_LOG_TYPE_STORED          = 0x02,
  B2P_LOG_TYPE_HS              = 0x03,
  B2P_LOG_TYPE_AWD             = 0x04,
  B2P_LOG_TYPE_HCI             = 0x05,
  B2P_LOG_TYPE_CRASH_AVAILABLE = 0x06,
  B2P_LOG_TYPE_FORCE_SEND      = 0xFE,
  B2P_LOG_TYPE_COMPLETE        = 0xFF
} B2P_LOG_MSG_TYPE_e;


/**
 * @brief LOG Message Status types,
 */
typedef enum
{
  B2P_LOG_SUCCESS,
  B2P_LOG_FAIL
} B2P_LOG_MSG_STATUS_e;

/**
 * @brief Charging states.
 */
typedef enum
{
  B2P_CHARGING_STATE_unknown,
  B2P_CHARGING_STATE_discharging,
  B2P_CHARGING_STATE_charging,
  B2P_CHARGING_STATE_charged,
  B2P_CHARGING_STATE_fault,
} B2P_CHARGING_STATE_e;

/**
 * @brief Data Session status codes.
 */
typedef enum
{
  B2P_DS_STATUS_idle =      0,
  B2P_DS_STATUS_ok =        1,
  B2P_DS_STATUS_success =   2,
  B2P_DS_STATUS_error =     3,
  B2P_DS_STATUS_unknown = 255
} B2P_DS_STATUS_e;

/**
 * @brief Data Session control commands.
 */
typedef enum
{
  B2P_DS_CTRL_get_mtu =           0, /**< Get MTU size. */
  B2P_DS_CTRL_discovery =         1, /**< Get look-up table of DS IDs. */
  B2P_DS_CTRL_set_destination =   2, /**< Set destination of data transferred. */
} B2P_DS_CTRL_e;

/**
 * @brief DFU status codes.
 */
typedef enum
{
  B2P_DFU_STATUS_idle,
  B2P_DFU_STATUS_done,
  B2P_DFU_STATUS_installing,
  B2P_DFU_STATUS_error
} B2P_DFU_STATUS_e;

/**
 * @brief Fuse states.
 */
typedef enum
{
  B2P_FUSE_STATE_unfused,
  B2P_FUSE_STATE_dev,
  B2P_FUSE_STATE_prod
} B2P_FUSE_STATE_e;

/**
 * @brief bluetooth linkkey type (same as OI_BT_LINK_KEY_TYPE).
 */
typedef enum
{
  B2P_BT_LINKKEY_TYPE_COMBO                 = 0,
  B2P_BT_LINKKEY_TYPE_LOCAL_UNIT            = 1,
  B2P_BT_LINKKEY_TYPE_REMOTE_UNIT           = 2,
  B2P_BT_LINKKEY_TYPE_DEBUG_COMBO           = 3,
  B2P_BT_LINKKEY_TYPE_UNAUTHENTICATED       = 4,
  B2P_BT_LINKKEY_TYPE_AUTHENTICATED         = 5,
  B2P_BT_LINKKEY_TYPE_CHANGED_COMBO         = 6,
  B2P_BT_LINKKEY_TYPE_MAGIC                 = 7,
  B2P_BT_LINKKEY_TYPE_SHARING               = 8,

  // W3 Only
  B2P_BT_LINKKEY_TYPE_UNAUTH_COMBO_P256     = 7,
  B2P_BT_LINKKEY_TYPE_AUTH_COMBO_P256       = 8,
  B2P_BT_LINKKEY_TYPE_MAGIC_W3              = 0xF0,
  B2P_BT_LINKKEY_TYPE_SHARING_W3            = 0xF1,
} B2P_BT_LINKKEY_TYPE_e;

/**
 * @brief Audio states
 */
typedef enum
{
  B2P_AUDIO_STATE_disconnected,
  B2P_AUDIO_STATE_a2dp,
  B2P_AUDIO_STATE_hfp_incall,
  B2P_AUDIO_STATE_hfp_insiri,
  B2P_AUDIO_STATE_hfp_other,
  B2P_AUDIO_STATE_doap,
  B2P_AUDIO_STATE_doap_insiri,
  B2P_AUDIO_STATE_a2dp_doap,
  B2P_AUDIO_STATE_a2dp_insiri
} B2P_AUDIO_STATE_e;

/**
 * @brief Audio call setup status types
 */
typedef enum
{
  B2P_CALL_SETUP_STATUS_unknown,
  B2P_CALL_SETUP_STATUS_idle,
  B2P_CALL_SETUP_STATUS_incoming,
  B2P_CALL_SETUP_STATUS_outgoing
} B2P_CALL_SETUP_STATUS_e;

/**
 * @brief Audio call hold status types
 */
typedef enum
{
  B2P_CALL_HOLD_STATUS_idle,
  B2P_CALL_HOLD_STATUS_withactive,
  B2P_CALL_HOLD_STATUS_noactive
} B2P_CALL_HOLD_STATUS_e;

/***********
 * STRUCTs *
 ***********/

/**
 * @brief The command format for ping.
 */
typedef struct ATTR_PACKED
{
  uint8_t resv;
  uint8_t protocol_ver; /**< See #B2P_PROTOCOL_VERSION_e. */
} B2P_CMD_PING_s;

/**
 * @brief The response format for ping.
 */
typedef struct ATTR_PACKED
{
  uint8_t protocol_ver; /**< See #B2P_PROTOCOL_VERSION_e. */
  uint8_t mode; /**< See #B2P_PROTOCOL_MODE_e. */
} B2P_RSP_PING_s;

/**
 * @brief The command format for identification.
 */
typedef struct ATTR_PACKED
{
  uint8_t info_id; /**< See #B2P_INFO_ID_e. */
} B2P_CMD_IDENTIFICATION_s;

/**
 * @brief The response format for identification.
 */
typedef struct ATTR_PACKED
{
  uint8_t info_id; /**< See #B2P_INFO_ID_e. */
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE-1]; /**< See #B2P_RSP_IDENTIFICATION_BASIC_s if info_id is B2P_INFO_ID_BASIC. See #B2P_RSP_IDENTIFICATION_SN_s if info_id is B2P_INFO_ID_SN. */
} B2P_RSP_IDENTIFICATION_s;

/**
 * @brief The identification response format for info id 0 (BASIC).
 */
typedef struct ATTR_PACKED
{
  uint16_t chip_id;
  uint16_t board_id;
  uint8_t security_epoch;
  uint8_t production_status;
  uint8_t security_mode;
  uint8_t security_domain;
  uint8_t ap_ecid[8];
  uint8_t nonce[32];
  uint8_t ap_chiprev;
} B2P_RSP_IDENTIFICATION_BASIC_s;

/**
 * @brief The identification response format for info id 1 (SN).
 *
 * @note Max size -1 because this is being used as a B2P_RSP_IDENTIFICATION_s
 *       payload which has "info_id" as a first byte.
 */
typedef struct ATTR_PACKED
{
  uint8_t serial_number[B2P_PACKET_MAX_RSP_DATA_SIZE-1];
} B2P_RSP_IDENTIFICATION_SN_s;

/**
 * @brief The identification response format for info id 2 (Silicon).
 */
typedef struct ATTR_PACKED
{
  uint8_t siliconID;
  uint8_t chipRevision;
  uint8_t customFields[2];
} B2P_RSP_IDENTIFICATION_SILICON_s;

/**
 * @brief The command format for reset.
 */
typedef struct ATTR_PACKED
{
  uint8_t mode; /**< See #B2P_RESET_MODE_e. */
  uint16_t time_ms;
} B2P_CMD_RESET_s;

/**
 * @brief The response format for reset.
 */
typedef struct ATTR_PACKED
{
  uint8_t mode; /**< See #B2P_RESET_MODE_e. */
  uint16_t time_ms;
} B2P_RSP_RESET_s;

/**
 * @brief The command format for DataSessions connection.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_id; /**< See #B2P_DS_SESSION_ID_e. */
  uint8_t state; /**< See #B2P_DS_STATE_e. */
} B2P_CMD_DS_CONNECT_s;

/**
 * @brief The response format for DataSessions connection.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_id; /**< See #B2P_DS_SESSION_ID_e. */
  uint8_t state; /**< See #B2P_DS_STATE_e. */
} B2P_RSP_DS_CONNECT_s;

/**
 * @brief The command format for DataSessions data transfer.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_id; /**< See #B2P_DS_SESSION_ID_e. */
  uint8_t status; /**< See #B2P_DS_STATUS_e. */
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE-2];
} B2P_CMD_DS_DATA_s;

/**
 * @brief The response format for DataSessions data transfer.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_id; /**< See #B2P_DS_SESSION_ID_e. */
  uint8_t status; /**< See #B2P_DS_STATUS_e. */
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE-2];
} B2P_RSP_DS_DATA_s;

/**
 * @brief The command format for an I2C write transaction.
 */
typedef struct ATTR_PACKED
{
  uint8_t bus;
  uint8_t address;
  uint8_t bytes[B2P_PACKET_MAX_CMD_DATA_SIZE-2];
} B2P_CMD_I2C_WRITE_s;

/**
 * @brief The command format for an I2C write transaction to an 8-bit register.
 */
typedef struct ATTR_PACKED
{
  uint8_t bus;
  uint8_t address;
  uint8_t reg;
  uint8_t bytes[B2P_PACKET_MAX_CMD_DATA_SIZE-3];
} B2P_CMD_I2C_WRITE_REG8_s;

/**
 * @brief The command format for an I2C write transaction to a 16-bit register.
 */
typedef struct ATTR_PACKED
{
  uint8_t  bus;
  uint8_t  address;
  uint16_t reg;
  uint8_t  bytes[B2P_PACKET_MAX_CMD_DATA_SIZE-4];
} B2P_CMD_I2C_WRITE_REG16_s;

/**
 * @brief The command format for an I2C read transaction.
 */
typedef struct ATTR_PACKED
{
  uint8_t bus;
  uint8_t address;
  uint8_t length;
} B2P_CMD_I2C_READ_s;

/**
 * @brief The response format for an I2C read transaction.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_RSP_I2C_READ_s;

/**
 * @brief The command format for an I2C read transaction to an 8-bit register.
 */
typedef struct ATTR_PACKED
{
  uint8_t bus;
  uint8_t address;
  uint8_t reg;
  uint8_t length;
} B2P_CMD_I2C_READ_REG8_s;

/**
 * @brief The response format for an I2C read transaction of an 8-bit register.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_RSP_I2C_READ_REG8_s;

/**
 * @brief The command format for an I2C read transaction to a 16-bit register.
 */
typedef struct ATTR_PACKED
{
  uint8_t  bus;
  uint8_t  address;
  uint16_t reg;
  uint8_t  length;
} B2P_CMD_I2C_READ_REG16_s;

/**
 * @brief The response format for an I2C read transaction of an 16-bit register.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_RSP_I2C_READ_REG16_s;

/**
 * @brief The command format for data session control.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_id; /**< See #B2P_DS_SESSION_ID_e. */
  uint8_t ctrl_cmd; /**< See #B2P_DS_CTRL_e. */
  uint8_t data[]; /**< optional payload. */
} B2P_CMD_DS_CTRL_s;

/**
 * @brief The response format for data session control.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_id; /**< See #B2P_DS_SESSION_ID_e. */
  uint8_t ctrl_cmd; /**< See #B2P_DS_CTRL_e. */
  uint8_t data[]; /**< optional payload. */
} B2P_RSP_DS_CTRL_s;

/**
 * @brief The response format for DS_CTRL_GET_MTU.
 */
typedef struct ATTR_PACKED
{
  uint16_t size;
} B2P_RSP_DS_CTRL_GET_MTU_s;

/**
 * @brief The command format for DS_CTRL_SET_DESTINATION.
 */
typedef struct ATTR_PACKED
{
  uint8_t destination_id;
} B2P_CMD_DS_CTRL_SET_DESTINATION_s;


typedef struct ATTR_PACKED
{
  uint8_t session_id;
  uint8_t tag_len;
  char    tag[]; /**< human-readable tag for this data session. NOTE: may not be NULL-terminated. */
} B2P_DS_DISCOVERY_ENTRY_s;

/**
 * @brief The response format for fetching a look-up table of data session IDs.
 */
typedef struct ATTR_PACKED
{
  B2P_DS_DISCOVERY_ENTRY_s entry[1]; /**< Arbitrary number of entries. See #B2P_DS_DISCOVERY_ENTRY_s. */
} B2P_RSP_DS_DISCOVERY_s;

/**
 * @brief The response format for getting DFU status.
 */
typedef struct ATTR_PACKED
{
  uint8_t status; /**< See #B2P_DFU_STATUS_e. */
} B2P_RSP_DFU_STATUS_s;

/**
 * @brief The command format for reading control bits.
 */
typedef struct ATTR_PACKED
{
  uint8_t station; /**< Station ID */
} B2P_CMD_CBITS_READ_s;

/**
 * @brief The response format for reading control bits.
 */
typedef struct ATTR_PACKED
{
  uint32_t timestamp;             /**< Unix timestamp: number of seconds since the epoch (1/1/1970 00:00 GMT) */
  uint32_t overlayVersion;        /**< station overlay version */
  uint8_t  absEraseCounter;       /**< total number of relFailCounter erases; never reset */
  uint8_t  absFailCounter;        /**< total number of failed tests; never reset */
  uint8_t  relFailCounter;        /**< number of failed tests since last erase */
  uint8_t  state;                 /**< test state */
} B2P_RSP_CBITS_READ_s;

/**
 * @brief The command format for loopback.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_CMD_LOOPBACK_s;

/**
 * @brief The response format for loopback.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_RSP_LOOPBACK_s;

/**
 * @brief The command format for sending the reset code to Beryl.
 */
typedef struct ATTR_PACKED
{
  uint8_t resetcode;
} B2P_CMD_BERYL_RESETCODE_s;

/**
 * @brief The response format for getting firmware version.
 */
typedef struct ATTR_PACKED
{
  uint16_t major;
  uint16_t minor;
  uint16_t build;
} B2P_RSP_FW_VER_GET_s;

/**
 * @brief The command format for getting GPIO/pin state.
 */
typedef struct ATTR_PACKED
{
  uint8_t port;
  uint8_t pin;
} B2P_CMD_GPIO_GET_s;

/**
 * @brief The response format for getting GPIO/pin state.
 */
typedef struct ATTR_PACKED
{
  uint8_t state;
} B2P_RSP_GPIO_GET_s;

/**
 * @brief The command format for setting GPIO/pin state.
 */
typedef struct ATTR_PACKED
{
  uint8_t port;
  uint8_t pin;
  uint8_t state;
} B2P_CMD_GPIO_SET_s;

/**
 * @brief The command format for printing syscfg variable.
 */
typedef struct ATTR_PACKED
{
  uint8_t key[B2P_PACKET_MAX_CMD_DATA_SIZE]; /**< ASCII string of key name. */
} B2P_CMD_SYSCFG_PRINT_s;

/**
 * @brief The response format for printing syscfg variable.
 */
typedef struct ATTR_PACKED
{
  uint8_t value[B2P_PACKET_MAX_RSP_DATA_SIZE]; /**< ASCII string representation of value. */
} B2P_RSP_SYSCFG_PRINT_s;

/**
 * @brief The command format for retrieving metric data.
 */
typedef struct ATTR_PACKED
{
  uint8_t metric_id; /**< See #B2P_METRIC_ID_e. */
} B2P_CMD_METRIC_GET_s;

/**
 * @brief The response format for getting hardware version.
 */
typedef struct ATTR_PACKED
{
  uint16_t major;
  uint16_t minor;
  uint16_t build;
} B2P_RSP_HW_VER_GET_s;

/**
 * @brief The command format for retrieving metric data.
 */
typedef struct ATTR_PACKED
{
  uint8_t component; /**< See #B2P_SN_e. */
} B2P_CMD_SERIAL_NUM_GET_s;

/**
 * @brief The command format for retrieving metric data.
 */
typedef struct ATTR_PACKED
{
  uint8_t serial_number[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_RSP_SERIAL_NUM_GET_s;

/**
 * @brief The response format for retrieving color ID.
 */
typedef struct ATTR_PACKED
{
  uint16_t color_id;
} B2P_RSP_COLOR_GET_s;

/**
 * @brief The response format for getting prod fuse.
 */
typedef struct ATTR_PACKED
{
  uint8_t is_prod_fused; /**< 0 = Prod fuse not set; 1 = Prod fuse set */
} B2P_RSP_PROD_FUSE_GET_s;

/**
 * @brief The command format for B2P FWUP DATA.
 */
typedef struct ATTR_PACKED
{
  uint8_t end_point;
  uint8_t data[B2P_PACKET_MAX_CMD_DATA_SIZE - 1];
} B2P_CMD_FWUP_DATA_s;

/**
 * @brief The response format for B2P FWUP DATA.
 */
typedef struct ATTR_PACKED
{
  uint8_t end_point;
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE - 1];
} B2P_RSP_FWUP_DATA_s;

/**
 * @brief The command format for B2P FWUP CTRL.
 */
typedef struct ATTR_PACKED
{
  uint8_t end_point;
  uint8_t ctrl_cmd; /**< See #B2P_FWUP_CTRL_CMD_e. */
  uint8_t data[B2P_PACKET_MAX_CMD_DATA_SIZE - 2];
} B2P_CMD_FWUP_CTRL_s;

/**
 * @brief The response format for B2P FWUP CTRL.
 */
typedef struct ATTR_PACKED
{
  uint8_t end_point;
  uint8_t data[B2P_PACKET_MAX_RSP_DATA_SIZE - 1];
} B2P_RSP_FWUP_CTRL_s;

/**
 * @brief The command format for B2P FWUP CTRL STATUS.
 */
typedef struct ATTR_PACKED
{
  uint8_t FWUP_DS_connection_state;
  uint8_t FWUP_transport;             /**<See #B2P_FWUP_TRANSPORT_e. */
  uint8_t iAUP_protocol_state;
  uint8_t iAUP_status_code;
  uint8_t request_pending;
  uint8_t command;
  uint32_t imageLength;
  uint16_t iAUPClientBlockSize;
  uint16_t rx_last_block_index;
  uint16_t tx_last_block_index;
  uint8_t B2P_end_point;
  uint8_t B2P_session_type;
  uint8_t sessionID;
  uint8_t partition_override;
  uint8_t fw_file_index;
  uint8_t ios_ota_xfer_pending;
  uint8_t fwup_mode_active;
  uint8_t fwup_transfer_active;
} B2P_RSP_FWUP_CTRL_STATUS_s;

/**
 * @brief The command format for B2P FWUP CTRL START.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_type; /**< See #B2P_FWUP_CTRL_START_SESSION_TYPE_e. */
} B2P_CMD_FWUP_CTRL_START_s;

/**
 * @brief The command format for B2P FWUP CTRL START EX.
 */
typedef struct ATTR_PACKED
{
  uint8_t session_type; /**< See #B2P_FWUP_CTRL_START_SESSION_TYPE_e. */
  uint8_t partition; /**< See #B2P_FWUP_CTRL_START_EX_PARTITION_e. */
} B2P_CMD_FWUP_CTRL_START_EX_s;

/**
 * @brief The command format for sending reset code.
 */
typedef struct ATTR_PACKED
{
  uint8_t  dest; /**< See routing nibble definition. */
  uint16_t reset_code;
  uint32_t baudrate;
  uint32_t val_low;
  uint32_t val_high;
} B2P_CMD_RESET_CODE_s;

/**
 * @brief The command format for forcing DFU.
 */
typedef struct ATTR_PACKED
{
  uint8_t dest; /**< See routing nibble definition. */
  uint8_t enable;
} B2P_CMD_DFU_FORCE_s;

/**
 * @brief The response format for getting the BVER.
 */
typedef struct ATTR_PACKED
{
  uint8_t active_major[2];
  uint8_t active_minor[8];
  uint8_t active_rev[2];
  uint8_t pending_major[2];
  uint8_t pending_minor[8];
  uint8_t pending_rev[2];
  uint8_t active_file;
  uint8_t pending_file;
} B2P_RSP_BVER_GET_s;

/**
 * @brief The response format for retrieving device friendly name.
 */
typedef struct ATTR_PACKED
{
  char name[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_RSP_BT_NAME_GET_s;

/**
 * @brief The command format for setting device friendly name.
 */
typedef struct ATTR_PACKED
{
  char name[B2P_PACKET_MAX_CMD_DATA_SIZE];
} B2P_CMD_BT_NAME_SET_s;

/**
 * @brief The response format for FWUP info.
 */
typedef struct ATTR_PACKED
{
  uint8_t stage; /**< See #B2P_FWUP_STAGE_e. */
  uint8_t error; /**< See #B2P_FWUP_ERROR_e. */
  uint8_t last_command;
  uint8_t last_response;
  uint16_t last_block_requested;
  uint16_t last_block_returned;
  uint32_t total_size;
  uint16_t block_size;
  uint8_t server_id; /**< See routing nibbles. */
  uint8_t client_id; /**< See routing nibbles. */
  uint8_t substate;
} B2P_RSP_FWUP_INFO_s;

/**
 * @brief The command format for WHOLE BVER READ.
 */
typedef struct ATTR_PACKED
{
  uint8_t fw_partition; /**< See #B2P_FW_PARTITION_e. */
} B2P_CMD_WHOLE_BVER_READ_s;

/**
 * @brief The response format for WHOLE BVER READ.
 */
typedef struct ATTR_PACKED
{
  uint8_t data[B2P_PACKET_MAX_CMD_DATA_SIZE];
} B2P_RSP_WHOLE_BVER_READ_s;

/**
 * @brief The command format for SOUND PLAY.
 */
typedef struct ATTR_PACKED
{
  uint8_t sid; /**< Sound id. */
  uint8_t force; /**< Bool to force playing sound. */
  uint8_t count; /**< Number of repetitions. */
} B2P_CMD_SOUND_PLAY_s;

/**
 * @brief The response format for getting the volume
 */
typedef struct ATTR_PACKED
{
  uint16_t volume;
} B2P_RSP_VOLUME_GET_s;

/**
 * @brief The response format for getting the product category.
 */
typedef struct ATTR_PACKED
{
  uint8_t category; /**< See #B2P_PROD_CATEGORY_e. */
} B2P_RSP_PROD_CATEGORY_GET_s;

/**
 * @brief The response format for retrieving the product ID.
 */
typedef struct ATTR_PACKED
{
  uint16_t product_id;
} B2P_RSP_PRODUCT_ID_GET_s;

/**
 * @brief The response format for retriving firmware update origin.
 */
typedef struct ATTR_PACKED
{
  uint8_t origin; /**< See #B2P_FWUP_ORIGIN_e. */
} B2P_RSP_FWUP_ORIGIN_GET_s;

/**
 * @brief The command format for erasing flash.
 */
typedef struct ATTR_PACKED
{
  uint32_t start_block;
  uint32_t end_block;
} B2P_CMD_FLASH_ERASE_s;

/**
 * @brief The command format for writing flash.
 */
typedef struct ATTR_PACKED
{
  uint32_t start_addr;
  uint8_t  data[B2P_PACKET_MAX_CMD_DATA_SIZE - sizeof(uint32_t)];
} B2P_CMD_FLASH_WRITE_s;

/**
 * @brief The command format for reading flash.
 */
typedef struct ATTR_PACKED
{
  uint32_t start_addr;
  uint32_t sz;
} B2P_CMD_FLASH_READ_s;

/**
 * @brief The response format for reading flash.
 */
typedef struct ATTR_PACKED
{
  uint8_t  data[B2P_PACKET_MAX_RSP_DATA_SIZE];
} B2P_RSP_FLASH_READ_s;

/**
 * @brief The response format for retriving Device Capabilities.
 */
typedef struct ATTR_PACKED
{
  uint32_t dev_cap; /**< See #B2P_DEVICE_CAP_e. */
}B2P_RSP_DEVICE_CAPABILITIES_GET_s;

/**
 * @brief The command format for setting POP mode.
 *  pop_mode + checksum must equal 0
 */
typedef struct ATTR_PACKED
{
  uint8_t pop_mode; /**< See #B2P_POP_MODE_e */
  uint8_t checksum; /**< 8 bit checksum for pop_mode */
} B2P_CMD_POP_SET_s;

/**
 * @brief The response format for getting POP mode.
 */
typedef struct ATTR_PACKED
{
  uint8_t pop_mode; /**< See #B2P_POP_MODE_e */
} B2P_RSP_POP_GET_s;

/**
 * @brief The command format for Automatic InEar Detection.
 */
typedef struct ATTR_PACKED
{
  uint8_t set_val; /**< See #B2P_AUTO_INEAR_DETECTION_e. */
}B2P_CMD_AUTO_INEAR_DETECTION_SET_s;

/**
 * @brief The response format for Automatic InEar Detection.
 */
typedef struct ATTR_PACKED
{
  uint8_t inear_status;  /**< See #B2P_AUTO_INEAR_DETECTION_e. */
}B2P_RSP_AUTO_INEAR_DETECTION_GET_s;

/**
 * @brief The command format for Mic mode set.
 */
typedef struct ATTR_PACKED
{
  uint8_t mic_mode_set; /**< See #B2P_MIC_MODE_e. */
}B2P_CMD_MIC_MODE_SET_s;

/**
 * @brief The response format for Mic Mode Get.
 */
typedef struct ATTR_PACKED
{
  uint8_t mic_mode_get;  /**< See #B2P_MIC_MODE_e. */
}B2P_RSP_MIC_MODE_GET_s;

/**
 * @brief The command format for getting battery info.
 */
typedef struct ATTR_PACKED
{
  uint8_t battLevel; /**< 0-100. */
  uint8_t battState; /**< See #B2P_BATTERY_STATE_e. */
} B2P_RSP_BATT_GET_s;

/**
 * @brief The command format for getting HS Asset hash.
 */
typedef struct ATTR_PACKED
{
  uint8_t index;
} B2P_CMD_HS_GET_HASH_s;

/**
 * @brief The response format for getting HS Asset hash.
 */
typedef struct ATTR_PACKED
{
  uint8_t hash[25];
} B2P_RSP_HS_GET_HASH_s;

/**
 * @brief The command format for getting Listening state
 */
typedef struct ATTR_PACKED
{
  uint8_t req_mode; /**< See #B2P_LISTENING_MODE_LEVEL_e. */
} B2P_CMD_LISTENING_MODE_LEVEL_GET_s;

/**
 * @brief The response format for getting Listening state
 */
typedef struct ATTR_PACKED
{
  uint8_t listening_mode_level; /**< ANC_ON/ANC_OFF or AWARE_ON/AWARE_OFF based on req_mode selected. */
} B2P_RSP_LISTENING_MODE_LEVEL_GET_s;

/**
 * @brief The command format for setting Listening Mode
 */
typedef struct ATTR_PACKED
{
  uint8_t req_mode; /**< See #B2P_LISTENING_MODE_LEVEL_e. */
  uint8_t level;
} B2P_CMD_LISTENING_MODE_LEVEL_SET_s;

/**
 * @brief The command format for Notifying Listening Mode
 */
typedef struct ATTR_PACKED
{
  uint8_t num_modes;
  uint8_t level[B2P_PACKET_MAX_CMD_DATA_SIZE - sizeof(uint8_t)]; /**< level[0] for ANC mode and level[1] for transparency mode. */
} B2P_CMD_LISTENING_MODE_LEVEL_NOTIFY_s;

/**
 * @brief The response format for getting Listening mode rotation list
 */
typedef struct ATTR_PACKED
{
  uint8_t listening_mode_rotation; /**< See #B2P_LISTENING_MODE_ROTATION_e. */
} B2P_RSP_LISTENING_MODE_ROTATION_GET_s;

/**
 * @brief The command format for setting Listening mode rotation list
 */
typedef struct ATTR_PACKED
{
  uint8_t listening_mode_rotation; /**< See #B2P_LISTENING_MODE_ROTATION_e. */
} B2P_CMD_LISTENING_MODE_ROTATION_SET_s;

/**
 * @brief The command format for exchanging Auth Data.
 */
typedef struct ATTR_PACKED
{
  uint8_t auth_pdu[B2P_PACKET_MAX_CMD_DATA_SIZE];
} B2P_CMD_AUTH_s;

/**
 * @brief The command format for Magic Key Get.
 */
typedef struct ATTR_PACKED
{
  uint32_t key_mask;
} B2P_CMD_MAGIC_KEY_GET_s;

/**
 * @brief The command format for Magic pairing key defintion.
 */
typedef struct ATTR_PACKED
{
  uint16_t  keyType;   /**< Type of the key. */
  uint16_t  keyLength; /**< Key length in bytes. */
  uint8_t   key[B2P_MAGIC_KEY_BYTE_SIZE];    /**< key  of 16 bytes. */
} B2P_MAGIC_PAIRING_KEY;

/**
 * @brief The command format for Notifying Battery Info
 */
typedef struct ATTR_PACKED
{
  uint8_t id;      /**< BATT_ID_XXX  */
  uint8_t type;    /**< BATT_TYPE_XXX  */
  uint8_t level;   /**< BATT_LEVEL_XXX or 0-100 value */
  uint8_t state;   /**< BATT_STATE_XXX  */
  uint8_t status;  /**< BATT_STATUS_XXX  */
} B2P_CMD_BATT_INFO_NOTIFY_s;

/**
 * @brief The response format for Magic Key Get.
 */
typedef struct ATTR_PACKED
{
  B2P_MAGIC_PAIRING_KEY magic_keys[2];
} B2P_RSP_MAGIC_KEY_GET_s;

/**
 * @brief The response format for retrieving the bluetooth address.
 */
typedef struct ATTR_PACKED
{
  uint8_t bt_addr[6];
} B2P_RSP_BT_ADDR_GET_s;

/**
 * @brief The response format for retrieving PMU info.
 */
typedef struct ATTR_PACKED
{
  uint8_t  uisoc;       /**< as a percentage. */
  uint8_t  raw_soc;     /**< as a percentage. */
  uint16_t voltage;     /**< in mV. */
  int16_t  current;     /**< in mA. */
  int16_t  temperature; /**< in 0.1 degrees Celsius. */
} B2P_RSP_PMU_INFO_GET_s;

/**
 * @brief The command format for getting NV variable.
 */
typedef struct ATTR_PACKED
{
  uint8_t type;     /**< See #B2P_NV_TYPE_e. */
  char name[9];     /** Must include null terminator. */
  uint8_t length;   /** Length of arbitrary length data. */
  uint8_t data[];   /**< Arbitrary length data.  */
} B2P_CMD_NV_GET_s;

/**
 * @brief The command format for setting NV variable.
 */
typedef struct ATTR_PACKED
{
  uint8_t type;     /**< See #B2P_NV_TYPE_e. */
  char name[9];     /** Must include null terminator. */
  uint8_t length;   /** Length of arbitrary length data. */
  uint8_t data[];   /**< Arbitrary length data.  */
} B2P_CMD_NV_SET_s;

/**
 * @brief The command format for Send data from Log manager.
 */

typedef struct ATTR_PACKED
{
  uint32_t  client_type;
  uint8_t  msg_type;     /**< See #B2P_LOG_MSG_TYPE_e. */
  uint8_t  data[];       /**< 512 bytes length(max) data, this value should less than B2P MTU.  */
} B2P_CMD_LOG_DATA_SEND_s;

/**
 * @brief The command format for RSP to send data from Log manager.
 */

typedef struct ATTR_PACKED
{
  uint32_t  client_type;
  uint8_t  msg_type;       /**< See #B2P_LOG_MSG_TYPE_e. */
  uint8_t  status;         /**< See B2P_LOG_MSG_STATUS_e. */
} B2P_RSP_LOG_DATA_SEND_s;

/**
 * @brief The command format for sending sleep counts.
 */
typedef struct ATTR_PACKED
{
  uint32_t sleep_count;
  uint32_t deep_sleep_count;
  uint32_t hibernate_count;
  uint32_t aop_deep_sleep_count;
} B2P_RSP_SLEEP_COUNTS_GET_s;

/**
 * @brief The format for B2P_CMD_FLASH_BANK_SWITCH command. Forces switch to $bank
 *
 */
typedef struct ATTR_PACKED
{
  uint8_t bank;
} B2P_CMD_FLASH_BANK_SWITCH_s;

/**
 * @brief The format for FLASH_BANK_INFO_GET response.
 * Will return either 1 or 2 for bank1 and bank2 respectively
 */
typedef struct ATTR_PACKED
{
  uint8_t bank;
} B2P_RSP_FLASH_ACTIVE_BANK_GET_s;

/**
 * @brief The format for B2P_CMD_AUTO_HEAL_SET command.
 */
typedef struct ATTR_PACKED
{
  uint8_t enable;       /**< 0 = disable, 1 = enable */
  uint8_t persistent;   /**< 0 = cleared on reset, 1 = persistent across reset */
} B2P_CMD_AUTO_HEAL_SET_s;

/**
 * @brief The format for B2P_RSP_AUTO_HEAL_GET response.
 */
typedef struct ATTR_PACKED
{
  uint8_t enable;       /**< 0 = disable, 1 = enable */
  uint8_t persistent;   /**< 0 = cleared on reset, 1 = persistent across reset */
} B2P_RSP_AUTO_HEAL_GET_s;

/**
 * @brief format for Device information bluetooth address & Keys.
 */
typedef struct ATTR_PACKED
{
  uint8_t bt_addr[B2P_BT_ADDR_LEN];
  uint8_t linkkey[B2P_BT_LINKKEY_LEN]; /**< The linkkey for a given MAC address. */
} B2P_PDL_INFO_PER_DEVICE;

/**
 * @brief The response format for retrieving the List of PDL  bluetooth address , name and Keys.
 */
typedef struct ATTR_PACKED
{
  B2P_PDL_INFO_PER_DEVICE pdl_info[B2P_MAX_PAIR_NUM];
} B2P_RSP_LINKKEYS_GET_ALL_s;

/**
 * @brief The response format for retrieving the AACP profile status.
 */
typedef struct ATTR_PACKED
{
  uint8_t profile_info;
} B2P_RSP_AACP_PROFILE_STATE_GET_s;

/**
 * @brief The command format for deleting LinkKey of given MAC address.
 */
typedef struct ATTR_PACKED
{
  uint8_t bdaddr[B2P_BT_ADDR_LEN];
} B2P_CMD_LINKKEY_DELETE_s;

/**
 * @brief The command format for retrieving the linkkey for a given MAC address.
 */
typedef struct ATTR_PACKED
{
  uint8_t bdaddr[B2P_BT_ADDR_LEN]; /**< MAC address of concern. */
} B2P_CMD_LINKKEY_GET_s;

/**
 * @brief The response format for retrieving the linkkey for a given MAC address.
 */
typedef struct ATTR_PACKED
{
  uint8_t linkkey[B2P_BT_LINKKEY_LEN]; /**< The linkkey for a given MAC address. */
} B2P_RSP_LINKKEY_GET_s;

/**
 */
typedef struct ATTR_PACKED
{
  uint32_t ms; /**< uptime in milliseconds. */
} B2P_RSP_UPTIME_GET_s;

/**
 * @brief The cmd payload for an automation packet
 */
typedef struct ATTR_PACKED
{
  uint8_t service_id;   /**< Id for the service being tested */
  uint8_t packet_type;  /**< Get/Set/Notify packet */
  uint16_t size;        /**< Size of Automation command payload + header */
  uint16_t report_id;   /**< ID of the report for the Service to act on  */
  uint8_t data[];       /**< Arbitrary length data.  */
} B2P_AUTOMATION_CMD_PAYLOAD_s;

/**
 * @brief The rsp payload for an automation packet
 */
typedef struct ATTR_PACKED
{
  uint8_t service_id;     /**< Id for the service being tested */
  uint8_t packet_type;    /**< Get/Set/Notify packet */
  uint16_t size;          /**< Size of Automation response payload + header  */
  uint8_t report_status;  /**< Report status returned by the service */
  uint16_t report_id;     /**< ID of the report the Service acted on */
  uint8_t data[];         /**< Arbitrary length data. */
} B2P_AUTOMATION_RSP_PAYLOAD_s;

/**
 * @brief The response payload for gasgauge config version get.
 */
typedef struct ATTR_PACKED
{
  uint8_t hw_type;
  uint8_t version;
} B2P_RSP_GG_CFG_VER_GET_s;

/**
 * @brief The command to configure USB paths (muxes, and custom silicon (e.g.: Locust) states).
 */
typedef struct ATTR_PACKED
{
  uint8_t destination; /**< Destination USB path: Routing nibble ID */
  uint8_t enable;      /**< Enable USB connection: No=0, Yes=1 */
  uint8_t mode;        /**< See #B2P_USB_MODE_e. */
} B2P_CMD_USB_MODE_SET_s;


/**
 * @brief The response payload for retrieving the train build from the device.
 *  Exposes RTKOS_version.h.
 */
typedef struct ATTR_PACKED
{
  char train_name[20]; // Will truncate name i > 20.
  char bni_major_letter;
  char bni_minor_letter;
  uint32_t bni_major_version;
  uint32_t bni_minor_version;
} B2P_RSP_TRAIN_BUILD_GET_s;


/**
 * @brief The command from case for BlueAvengers.
 */
typedef struct ATTR_PACKED
{
  uint64_t utc_time;
  uint8_t state : 1;
  uint8_t advertise : 1;
  uint8_t sound : 1;
  uint8_t couple_docked : 1;
  uint8_t unused : 4;
} B2P_CMD_BLUE_AVENGERS_CONFIG_s;

/**
 * @brief The response to BlueAvengers command from case.
 */
typedef struct ATTR_PACKED
{
  uint64_t utc_time;
  uint8_t  state : 1;
  uint8_t  defer : 1;
  uint8_t  sound : 1;
  uint8_t  unused : 5;
} B2P_RSP_BLUE_AVENGERS_CONFIG_s;

/**
 * @brief Sniff info per connection for the response payload
 *        for retrieving the BT sniff from the device
 *
 */
typedef struct ATTR_PACKED
{
  uint8_t           bdaddr[B2P_BT_ADDR_LEN];
  uint8_t           currentMode;
  uint16_t          currentInterval;
} B2P_RSP_BT_SNIFF_INFO_PER_CONN_s;

/**
 * @brief The response payload for retrieving the BT sniff from
 *        the device
 *
 */
typedef struct ATTR_PACKED
{
  B2P_RSP_BT_SNIFF_INFO_PER_CONN_s info[B2P_BT_MAX_CONNECTIONS];
} B2P_RSP_BT_SNIFF_INFO_GET_s;

/**
 * @brief Audio status per connection for the response payload
 *        for retrieving the audio status from the device
 *
 */
typedef struct ATTR_PACKED
{
  uint8_t           audio_state;       /**< See #B2P_AUDIO_STATE_e. */
  uint8_t           call_setup_status; /**< See #B2P_CALL_SETUP_STATUS_e. */
  uint8_t           call_hold_status;  /**< See #B2P_CALL_HOLD_STATUS_e. */
} B2P_RSP_AUDIO_STATUS_PER_CONN_s;

/**
 * @brief The response payload for retrieving the audio status from
 *        the device
 *
 */
typedef struct ATTR_PACKED
{
  B2P_RSP_AUDIO_STATUS_PER_CONN_s info[B2P_BT_MAX_CONNECTIONS];
} B2P_RSP_AUDIO_STATUS_GET_s;

/**
 * @brief Snoop info for the response payload for
 *        retrieving the BT snoop from the device
 *
 */
typedef struct ATTR_PACKED
{
  uint16_t          state;
  uint8_t           current_addr[B2P_BT_ADDR_LEN];
  uint8_t           target_addr[B2P_BT_ADDR_LEN];
} B2P_RSP_BT_SNOOP_INFO_GET_s;

/**
 * @brief The response payload for timestamp sync
 *
 */
typedef struct ATTR_PACKED
{
  uint32_t          timestamp_s;
} B2P_RSP_SYNC_TIMESTAMP_s;

#endif /* B2P_DICT_H_ */
