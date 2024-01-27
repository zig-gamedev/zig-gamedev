/*
 * This file generated automatically from xf86vidmode.xml by c_client.py.
 * Edit at your peril.
 */

/**
 * @defgroup XCB_XF86VidMode_API XCB XF86VidMode API
 * @brief XF86VidMode XCB Protocol Implementation.
 * @{
 **/

#ifndef __XF86VIDMODE_H
#define __XF86VIDMODE_H

#include "xcb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XCB_XF86VIDMODE_MAJOR_VERSION 2
#define XCB_XF86VIDMODE_MINOR_VERSION 2

extern xcb_extension_t xcb_xf86vidmode_id;

typedef uint32_t xcb_xf86vidmode_syncrange_t;

/**
 * @brief xcb_xf86vidmode_syncrange_iterator_t
 **/
typedef struct xcb_xf86vidmode_syncrange_iterator_t {
    xcb_xf86vidmode_syncrange_t *data;
    int                          rem;
    int                          index;
} xcb_xf86vidmode_syncrange_iterator_t;

typedef uint32_t xcb_xf86vidmode_dotclock_t;

/**
 * @brief xcb_xf86vidmode_dotclock_iterator_t
 **/
typedef struct xcb_xf86vidmode_dotclock_iterator_t {
    xcb_xf86vidmode_dotclock_t *data;
    int                         rem;
    int                         index;
} xcb_xf86vidmode_dotclock_iterator_t;

typedef enum xcb_xf86vidmode_mode_flag_t {
    XCB_XF86VIDMODE_MODE_FLAG_POSITIVE_H_SYNC = 1,
    XCB_XF86VIDMODE_MODE_FLAG_NEGATIVE_H_SYNC = 2,
    XCB_XF86VIDMODE_MODE_FLAG_POSITIVE_V_SYNC = 4,
    XCB_XF86VIDMODE_MODE_FLAG_NEGATIVE_V_SYNC = 8,
    XCB_XF86VIDMODE_MODE_FLAG_INTERLACE = 16,
    XCB_XF86VIDMODE_MODE_FLAG_COMPOSITE_SYNC = 32,
    XCB_XF86VIDMODE_MODE_FLAG_POSITIVE_C_SYNC = 64,
    XCB_XF86VIDMODE_MODE_FLAG_NEGATIVE_C_SYNC = 128,
    XCB_XF86VIDMODE_MODE_FLAG_H_SKEW = 256,
    XCB_XF86VIDMODE_MODE_FLAG_BROADCAST = 512,
    XCB_XF86VIDMODE_MODE_FLAG_PIXMUX = 1024,
    XCB_XF86VIDMODE_MODE_FLAG_DOUBLE_CLOCK = 2048,
    XCB_XF86VIDMODE_MODE_FLAG_HALF_CLOCK = 4096
} xcb_xf86vidmode_mode_flag_t;

typedef enum xcb_xf86vidmode_clock_flag_t {
    XCB_XF86VIDMODE_CLOCK_FLAG_PROGRAMABLE = 1
} xcb_xf86vidmode_clock_flag_t;

typedef enum xcb_xf86vidmode_permission_t {
    XCB_XF86VIDMODE_PERMISSION_READ = 1,
    XCB_XF86VIDMODE_PERMISSION_WRITE = 2
} xcb_xf86vidmode_permission_t;

/**
 * @brief xcb_xf86vidmode_mode_info_t
 **/
typedef struct xcb_xf86vidmode_mode_info_t {
    xcb_xf86vidmode_dotclock_t dotclock;
    uint16_t                   hdisplay;
    uint16_t                   hsyncstart;
    uint16_t                   hsyncend;
    uint16_t                   htotal;
    uint32_t                   hskew;
    uint16_t                   vdisplay;
    uint16_t                   vsyncstart;
    uint16_t                   vsyncend;
    uint16_t                   vtotal;
    uint8_t                    pad0[4];
    uint32_t                   flags;
    uint8_t                    pad1[12];
    uint32_t                   privsize;
} xcb_xf86vidmode_mode_info_t;

/**
 * @brief xcb_xf86vidmode_mode_info_iterator_t
 **/
typedef struct xcb_xf86vidmode_mode_info_iterator_t {
    xcb_xf86vidmode_mode_info_t *data;
    int                          rem;
    int                          index;
} xcb_xf86vidmode_mode_info_iterator_t;

/**
 * @brief xcb_xf86vidmode_query_version_cookie_t
 **/
typedef struct xcb_xf86vidmode_query_version_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_query_version_cookie_t;

/** Opcode for xcb_xf86vidmode_query_version. */
#define XCB_XF86VIDMODE_QUERY_VERSION 0

/**
 * @brief xcb_xf86vidmode_query_version_request_t
 **/
typedef struct xcb_xf86vidmode_query_version_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
} xcb_xf86vidmode_query_version_request_t;

/**
 * @brief xcb_xf86vidmode_query_version_reply_t
 **/
typedef struct xcb_xf86vidmode_query_version_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint16_t major_version;
    uint16_t minor_version;
} xcb_xf86vidmode_query_version_reply_t;

/**
 * @brief xcb_xf86vidmode_get_mode_line_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_mode_line_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_mode_line_cookie_t;

/** Opcode for xcb_xf86vidmode_get_mode_line. */
#define XCB_XF86VIDMODE_GET_MODE_LINE 1

/**
 * @brief xcb_xf86vidmode_get_mode_line_request_t
 **/
typedef struct xcb_xf86vidmode_get_mode_line_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
} xcb_xf86vidmode_get_mode_line_request_t;

/**
 * @brief xcb_xf86vidmode_get_mode_line_reply_t
 **/
typedef struct xcb_xf86vidmode_get_mode_line_reply_t {
    uint8_t                    response_type;
    uint8_t                    pad0;
    uint16_t                   sequence;
    uint32_t                   length;
    xcb_xf86vidmode_dotclock_t dotclock;
    uint16_t                   hdisplay;
    uint16_t                   hsyncstart;
    uint16_t                   hsyncend;
    uint16_t                   htotal;
    uint16_t                   hskew;
    uint16_t                   vdisplay;
    uint16_t                   vsyncstart;
    uint16_t                   vsyncend;
    uint16_t                   vtotal;
    uint8_t                    pad1[2];
    uint32_t                   flags;
    uint8_t                    pad2[12];
    uint32_t                   privsize;
} xcb_xf86vidmode_get_mode_line_reply_t;

/** Opcode for xcb_xf86vidmode_mod_mode_line. */
#define XCB_XF86VIDMODE_MOD_MODE_LINE 2

/**
 * @brief xcb_xf86vidmode_mod_mode_line_request_t
 **/
typedef struct xcb_xf86vidmode_mod_mode_line_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint32_t screen;
    uint16_t hdisplay;
    uint16_t hsyncstart;
    uint16_t hsyncend;
    uint16_t htotal;
    uint16_t hskew;
    uint16_t vdisplay;
    uint16_t vsyncstart;
    uint16_t vsyncend;
    uint16_t vtotal;
    uint8_t  pad0[2];
    uint32_t flags;
    uint8_t  pad1[12];
    uint32_t privsize;
} xcb_xf86vidmode_mod_mode_line_request_t;

/** Opcode for xcb_xf86vidmode_switch_mode. */
#define XCB_XF86VIDMODE_SWITCH_MODE 3

/**
 * @brief xcb_xf86vidmode_switch_mode_request_t
 **/
typedef struct xcb_xf86vidmode_switch_mode_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint16_t zoom;
} xcb_xf86vidmode_switch_mode_request_t;

/**
 * @brief xcb_xf86vidmode_get_monitor_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_monitor_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_monitor_cookie_t;

/** Opcode for xcb_xf86vidmode_get_monitor. */
#define XCB_XF86VIDMODE_GET_MONITOR 4

/**
 * @brief xcb_xf86vidmode_get_monitor_request_t
 **/
typedef struct xcb_xf86vidmode_get_monitor_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
} xcb_xf86vidmode_get_monitor_request_t;

/**
 * @brief xcb_xf86vidmode_get_monitor_reply_t
 **/
typedef struct xcb_xf86vidmode_get_monitor_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint8_t  vendor_length;
    uint8_t  model_length;
    uint8_t  num_hsync;
    uint8_t  num_vsync;
    uint8_t  pad1[20];
} xcb_xf86vidmode_get_monitor_reply_t;

/** Opcode for xcb_xf86vidmode_lock_mode_switch. */
#define XCB_XF86VIDMODE_LOCK_MODE_SWITCH 5

/**
 * @brief xcb_xf86vidmode_lock_mode_switch_request_t
 **/
typedef struct xcb_xf86vidmode_lock_mode_switch_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint16_t lock;
} xcb_xf86vidmode_lock_mode_switch_request_t;

/**
 * @brief xcb_xf86vidmode_get_all_mode_lines_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_all_mode_lines_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_all_mode_lines_cookie_t;

/** Opcode for xcb_xf86vidmode_get_all_mode_lines. */
#define XCB_XF86VIDMODE_GET_ALL_MODE_LINES 6

/**
 * @brief xcb_xf86vidmode_get_all_mode_lines_request_t
 **/
typedef struct xcb_xf86vidmode_get_all_mode_lines_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
} xcb_xf86vidmode_get_all_mode_lines_request_t;

/**
 * @brief xcb_xf86vidmode_get_all_mode_lines_reply_t
 **/
typedef struct xcb_xf86vidmode_get_all_mode_lines_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint32_t modecount;
    uint8_t  pad1[20];
} xcb_xf86vidmode_get_all_mode_lines_reply_t;

/** Opcode for xcb_xf86vidmode_add_mode_line. */
#define XCB_XF86VIDMODE_ADD_MODE_LINE 7

/**
 * @brief xcb_xf86vidmode_add_mode_line_request_t
 **/
typedef struct xcb_xf86vidmode_add_mode_line_request_t {
    uint8_t                    major_opcode;
    uint8_t                    minor_opcode;
    uint16_t                   length;
    uint32_t                   screen;
    xcb_xf86vidmode_dotclock_t dotclock;
    uint16_t                   hdisplay;
    uint16_t                   hsyncstart;
    uint16_t                   hsyncend;
    uint16_t                   htotal;
    uint16_t                   hskew;
    uint16_t                   vdisplay;
    uint16_t                   vsyncstart;
    uint16_t                   vsyncend;
    uint16_t                   vtotal;
    uint8_t                    pad0[2];
    uint32_t                   flags;
    uint8_t                    pad1[12];
    uint32_t                   privsize;
    xcb_xf86vidmode_dotclock_t after_dotclock;
    uint16_t                   after_hdisplay;
    uint16_t                   after_hsyncstart;
    uint16_t                   after_hsyncend;
    uint16_t                   after_htotal;
    uint16_t                   after_hskew;
    uint16_t                   after_vdisplay;
    uint16_t                   after_vsyncstart;
    uint16_t                   after_vsyncend;
    uint16_t                   after_vtotal;
    uint8_t                    pad2[2];
    uint32_t                   after_flags;
    uint8_t                    pad3[12];
} xcb_xf86vidmode_add_mode_line_request_t;

/** Opcode for xcb_xf86vidmode_delete_mode_line. */
#define XCB_XF86VIDMODE_DELETE_MODE_LINE 8

/**
 * @brief xcb_xf86vidmode_delete_mode_line_request_t
 **/
typedef struct xcb_xf86vidmode_delete_mode_line_request_t {
    uint8_t                    major_opcode;
    uint8_t                    minor_opcode;
    uint16_t                   length;
    uint32_t                   screen;
    xcb_xf86vidmode_dotclock_t dotclock;
    uint16_t                   hdisplay;
    uint16_t                   hsyncstart;
    uint16_t                   hsyncend;
    uint16_t                   htotal;
    uint16_t                   hskew;
    uint16_t                   vdisplay;
    uint16_t                   vsyncstart;
    uint16_t                   vsyncend;
    uint16_t                   vtotal;
    uint8_t                    pad0[2];
    uint32_t                   flags;
    uint8_t                    pad1[12];
    uint32_t                   privsize;
} xcb_xf86vidmode_delete_mode_line_request_t;

/**
 * @brief xcb_xf86vidmode_validate_mode_line_cookie_t
 **/
typedef struct xcb_xf86vidmode_validate_mode_line_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_validate_mode_line_cookie_t;

/** Opcode for xcb_xf86vidmode_validate_mode_line. */
#define XCB_XF86VIDMODE_VALIDATE_MODE_LINE 9

/**
 * @brief xcb_xf86vidmode_validate_mode_line_request_t
 **/
typedef struct xcb_xf86vidmode_validate_mode_line_request_t {
    uint8_t                    major_opcode;
    uint8_t                    minor_opcode;
    uint16_t                   length;
    uint32_t                   screen;
    xcb_xf86vidmode_dotclock_t dotclock;
    uint16_t                   hdisplay;
    uint16_t                   hsyncstart;
    uint16_t                   hsyncend;
    uint16_t                   htotal;
    uint16_t                   hskew;
    uint16_t                   vdisplay;
    uint16_t                   vsyncstart;
    uint16_t                   vsyncend;
    uint16_t                   vtotal;
    uint8_t                    pad0[2];
    uint32_t                   flags;
    uint8_t                    pad1[12];
    uint32_t                   privsize;
} xcb_xf86vidmode_validate_mode_line_request_t;

/**
 * @brief xcb_xf86vidmode_validate_mode_line_reply_t
 **/
typedef struct xcb_xf86vidmode_validate_mode_line_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint32_t status;
    uint8_t  pad1[20];
} xcb_xf86vidmode_validate_mode_line_reply_t;

/** Opcode for xcb_xf86vidmode_switch_to_mode. */
#define XCB_XF86VIDMODE_SWITCH_TO_MODE 10

/**
 * @brief xcb_xf86vidmode_switch_to_mode_request_t
 **/
typedef struct xcb_xf86vidmode_switch_to_mode_request_t {
    uint8_t                    major_opcode;
    uint8_t                    minor_opcode;
    uint16_t                   length;
    uint32_t                   screen;
    xcb_xf86vidmode_dotclock_t dotclock;
    uint16_t                   hdisplay;
    uint16_t                   hsyncstart;
    uint16_t                   hsyncend;
    uint16_t                   htotal;
    uint16_t                   hskew;
    uint16_t                   vdisplay;
    uint16_t                   vsyncstart;
    uint16_t                   vsyncend;
    uint16_t                   vtotal;
    uint8_t                    pad0[2];
    uint32_t                   flags;
    uint8_t                    pad1[12];
    uint32_t                   privsize;
} xcb_xf86vidmode_switch_to_mode_request_t;

/**
 * @brief xcb_xf86vidmode_get_view_port_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_view_port_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_view_port_cookie_t;

/** Opcode for xcb_xf86vidmode_get_view_port. */
#define XCB_XF86VIDMODE_GET_VIEW_PORT 11

/**
 * @brief xcb_xf86vidmode_get_view_port_request_t
 **/
typedef struct xcb_xf86vidmode_get_view_port_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
} xcb_xf86vidmode_get_view_port_request_t;

/**
 * @brief xcb_xf86vidmode_get_view_port_reply_t
 **/
typedef struct xcb_xf86vidmode_get_view_port_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint32_t x;
    uint32_t y;
    uint8_t  pad1[16];
} xcb_xf86vidmode_get_view_port_reply_t;

/** Opcode for xcb_xf86vidmode_set_view_port. */
#define XCB_XF86VIDMODE_SET_VIEW_PORT 12

/**
 * @brief xcb_xf86vidmode_set_view_port_request_t
 **/
typedef struct xcb_xf86vidmode_set_view_port_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
    uint32_t x;
    uint32_t y;
} xcb_xf86vidmode_set_view_port_request_t;

/**
 * @brief xcb_xf86vidmode_get_dot_clocks_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_dot_clocks_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_dot_clocks_cookie_t;

/** Opcode for xcb_xf86vidmode_get_dot_clocks. */
#define XCB_XF86VIDMODE_GET_DOT_CLOCKS 13

/**
 * @brief xcb_xf86vidmode_get_dot_clocks_request_t
 **/
typedef struct xcb_xf86vidmode_get_dot_clocks_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
} xcb_xf86vidmode_get_dot_clocks_request_t;

/**
 * @brief xcb_xf86vidmode_get_dot_clocks_reply_t
 **/
typedef struct xcb_xf86vidmode_get_dot_clocks_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint32_t flags;
    uint32_t clocks;
    uint32_t maxclocks;
    uint8_t  pad1[12];
} xcb_xf86vidmode_get_dot_clocks_reply_t;

/** Opcode for xcb_xf86vidmode_set_client_version. */
#define XCB_XF86VIDMODE_SET_CLIENT_VERSION 14

/**
 * @brief xcb_xf86vidmode_set_client_version_request_t
 **/
typedef struct xcb_xf86vidmode_set_client_version_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t major;
    uint16_t minor;
} xcb_xf86vidmode_set_client_version_request_t;

/** Opcode for xcb_xf86vidmode_set_gamma. */
#define XCB_XF86VIDMODE_SET_GAMMA 15

/**
 * @brief xcb_xf86vidmode_set_gamma_request_t
 **/
typedef struct xcb_xf86vidmode_set_gamma_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint8_t  pad1[12];
} xcb_xf86vidmode_set_gamma_request_t;

/**
 * @brief xcb_xf86vidmode_get_gamma_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_gamma_cookie_t;

/** Opcode for xcb_xf86vidmode_get_gamma. */
#define XCB_XF86VIDMODE_GET_GAMMA 16

/**
 * @brief xcb_xf86vidmode_get_gamma_request_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[26];
} xcb_xf86vidmode_get_gamma_request_t;

/**
 * @brief xcb_xf86vidmode_get_gamma_reply_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint8_t  pad1[12];
} xcb_xf86vidmode_get_gamma_reply_t;

/**
 * @brief xcb_xf86vidmode_get_gamma_ramp_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_ramp_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_gamma_ramp_cookie_t;

/** Opcode for xcb_xf86vidmode_get_gamma_ramp. */
#define XCB_XF86VIDMODE_GET_GAMMA_RAMP 17

/**
 * @brief xcb_xf86vidmode_get_gamma_ramp_request_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_ramp_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint16_t size;
} xcb_xf86vidmode_get_gamma_ramp_request_t;

/**
 * @brief xcb_xf86vidmode_get_gamma_ramp_reply_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_ramp_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint16_t size;
    uint8_t  pad1[22];
} xcb_xf86vidmode_get_gamma_ramp_reply_t;

/** Opcode for xcb_xf86vidmode_set_gamma_ramp. */
#define XCB_XF86VIDMODE_SET_GAMMA_RAMP 18

/**
 * @brief xcb_xf86vidmode_set_gamma_ramp_request_t
 **/
typedef struct xcb_xf86vidmode_set_gamma_ramp_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint16_t size;
} xcb_xf86vidmode_set_gamma_ramp_request_t;

/**
 * @brief xcb_xf86vidmode_get_gamma_ramp_size_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_ramp_size_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_gamma_ramp_size_cookie_t;

/** Opcode for xcb_xf86vidmode_get_gamma_ramp_size. */
#define XCB_XF86VIDMODE_GET_GAMMA_RAMP_SIZE 19

/**
 * @brief xcb_xf86vidmode_get_gamma_ramp_size_request_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_ramp_size_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
} xcb_xf86vidmode_get_gamma_ramp_size_request_t;

/**
 * @brief xcb_xf86vidmode_get_gamma_ramp_size_reply_t
 **/
typedef struct xcb_xf86vidmode_get_gamma_ramp_size_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint16_t size;
    uint8_t  pad1[22];
} xcb_xf86vidmode_get_gamma_ramp_size_reply_t;

/**
 * @brief xcb_xf86vidmode_get_permissions_cookie_t
 **/
typedef struct xcb_xf86vidmode_get_permissions_cookie_t {
    unsigned int sequence;
} xcb_xf86vidmode_get_permissions_cookie_t;

/** Opcode for xcb_xf86vidmode_get_permissions. */
#define XCB_XF86VIDMODE_GET_PERMISSIONS 20

/**
 * @brief xcb_xf86vidmode_get_permissions_request_t
 **/
typedef struct xcb_xf86vidmode_get_permissions_request_t {
    uint8_t  major_opcode;
    uint8_t  minor_opcode;
    uint16_t length;
    uint16_t screen;
    uint8_t  pad0[2];
} xcb_xf86vidmode_get_permissions_request_t;

/**
 * @brief xcb_xf86vidmode_get_permissions_reply_t
 **/
typedef struct xcb_xf86vidmode_get_permissions_reply_t {
    uint8_t  response_type;
    uint8_t  pad0;
    uint16_t sequence;
    uint32_t length;
    uint32_t permissions;
    uint8_t  pad1[20];
} xcb_xf86vidmode_get_permissions_reply_t;

/** Opcode for xcb_xf86vidmode_bad_clock. */
#define XCB_XF86VIDMODE_BAD_CLOCK 0

/**
 * @brief xcb_xf86vidmode_bad_clock_error_t
 **/
typedef struct xcb_xf86vidmode_bad_clock_error_t {
    uint8_t  response_type;
    uint8_t  error_code;
    uint16_t sequence;
    uint32_t bad_value;
    uint16_t minor_opcode;
    uint8_t  major_opcode;
} xcb_xf86vidmode_bad_clock_error_t;

/** Opcode for xcb_xf86vidmode_bad_h_timings. */
#define XCB_XF86VIDMODE_BAD_H_TIMINGS 1

/**
 * @brief xcb_xf86vidmode_bad_h_timings_error_t
 **/
typedef struct xcb_xf86vidmode_bad_h_timings_error_t {
    uint8_t  response_type;
    uint8_t  error_code;
    uint16_t sequence;
    uint32_t bad_value;
    uint16_t minor_opcode;
    uint8_t  major_opcode;
} xcb_xf86vidmode_bad_h_timings_error_t;

/** Opcode for xcb_xf86vidmode_bad_v_timings. */
#define XCB_XF86VIDMODE_BAD_V_TIMINGS 2

/**
 * @brief xcb_xf86vidmode_bad_v_timings_error_t
 **/
typedef struct xcb_xf86vidmode_bad_v_timings_error_t {
    uint8_t  response_type;
    uint8_t  error_code;
    uint16_t sequence;
    uint32_t bad_value;
    uint16_t minor_opcode;
    uint8_t  major_opcode;
} xcb_xf86vidmode_bad_v_timings_error_t;

/** Opcode for xcb_xf86vidmode_mode_unsuitable. */
#define XCB_XF86VIDMODE_MODE_UNSUITABLE 3

/**
 * @brief xcb_xf86vidmode_mode_unsuitable_error_t
 **/
typedef struct xcb_xf86vidmode_mode_unsuitable_error_t {
    uint8_t  response_type;
    uint8_t  error_code;
    uint16_t sequence;
    uint32_t bad_value;
    uint16_t minor_opcode;
    uint8_t  major_opcode;
} xcb_xf86vidmode_mode_unsuitable_error_t;

/** Opcode for xcb_xf86vidmode_extension_disabled. */
#define XCB_XF86VIDMODE_EXTENSION_DISABLED 4

/**
 * @brief xcb_xf86vidmode_extension_disabled_error_t
 **/
typedef struct xcb_xf86vidmode_extension_disabled_error_t {
    uint8_t  response_type;
    uint8_t  error_code;
    uint16_t sequence;
    uint32_t bad_value;
    uint16_t minor_opcode;
    uint8_t  major_opcode;
} xcb_xf86vidmode_extension_disabled_error_t;

/** Opcode for xcb_xf86vidmode_client_not_local. */
#define XCB_XF86VIDMODE_CLIENT_NOT_LOCAL 5

/**
 * @brief xcb_xf86vidmode_client_not_local_error_t
 **/
typedef struct xcb_xf86vidmode_client_not_local_error_t {
    uint8_t  response_type;
    uint8_t  error_code;
    uint16_t sequence;
    uint32_t bad_value;
    uint16_t minor_opcode;
    uint8_t  major_opcode;
} xcb_xf86vidmode_client_not_local_error_t;

/** Opcode for xcb_xf86vidmode_zoom_locked. */
#define XCB_XF86VIDMODE_ZOOM_LOCKED 6

/**
 * @brief xcb_xf86vidmode_zoom_locked_error_t
 **/
typedef struct xcb_xf86vidmode_zoom_locked_error_t {
    uint8_t  response_type;
    uint8_t  error_code;
    uint16_t sequence;
    uint32_t bad_value;
    uint16_t minor_opcode;
    uint8_t  major_opcode;
} xcb_xf86vidmode_zoom_locked_error_t;

/**
 * Get the next element of the iterator
 * @param i Pointer to a xcb_xf86vidmode_syncrange_iterator_t
 *
 * Get the next element in the iterator. The member rem is
 * decreased by one. The member data points to the next
 * element. The member index is increased by sizeof(xcb_xf86vidmode_syncrange_t)
 */
void
xcb_xf86vidmode_syncrange_next (xcb_xf86vidmode_syncrange_iterator_t *i);

/**
 * Return the iterator pointing to the last element
 * @param i An xcb_xf86vidmode_syncrange_iterator_t
 * @return  The iterator pointing to the last element
 *
 * Set the current element in the iterator to the last element.
 * The member rem is set to 0. The member data points to the
 * last element.
 */
xcb_generic_iterator_t
xcb_xf86vidmode_syncrange_end (xcb_xf86vidmode_syncrange_iterator_t i);

/**
 * Get the next element of the iterator
 * @param i Pointer to a xcb_xf86vidmode_dotclock_iterator_t
 *
 * Get the next element in the iterator. The member rem is
 * decreased by one. The member data points to the next
 * element. The member index is increased by sizeof(xcb_xf86vidmode_dotclock_t)
 */
void
xcb_xf86vidmode_dotclock_next (xcb_xf86vidmode_dotclock_iterator_t *i);

/**
 * Return the iterator pointing to the last element
 * @param i An xcb_xf86vidmode_dotclock_iterator_t
 * @return  The iterator pointing to the last element
 *
 * Set the current element in the iterator to the last element.
 * The member rem is set to 0. The member data points to the
 * last element.
 */
xcb_generic_iterator_t
xcb_xf86vidmode_dotclock_end (xcb_xf86vidmode_dotclock_iterator_t i);

/**
 * Get the next element of the iterator
 * @param i Pointer to a xcb_xf86vidmode_mode_info_iterator_t
 *
 * Get the next element in the iterator. The member rem is
 * decreased by one. The member data points to the next
 * element. The member index is increased by sizeof(xcb_xf86vidmode_mode_info_t)
 */
void
xcb_xf86vidmode_mode_info_next (xcb_xf86vidmode_mode_info_iterator_t *i);

/**
 * Return the iterator pointing to the last element
 * @param i An xcb_xf86vidmode_mode_info_iterator_t
 * @return  The iterator pointing to the last element
 *
 * Set the current element in the iterator to the last element.
 * The member rem is set to 0. The member data points to the
 * last element.
 */
xcb_generic_iterator_t
xcb_xf86vidmode_mode_info_end (xcb_xf86vidmode_mode_info_iterator_t i);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_query_version_cookie_t
xcb_xf86vidmode_query_version (xcb_connection_t *c);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_query_version_cookie_t
xcb_xf86vidmode_query_version_unchecked (xcb_connection_t *c);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_query_version_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_query_version_reply_t *
xcb_xf86vidmode_query_version_reply (xcb_connection_t                        *c,
                                     xcb_xf86vidmode_query_version_cookie_t   cookie  /**< */,
                                     xcb_generic_error_t                    **e);

int
xcb_xf86vidmode_get_mode_line_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_mode_line_cookie_t
xcb_xf86vidmode_get_mode_line (xcb_connection_t *c,
                               uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_mode_line_cookie_t
xcb_xf86vidmode_get_mode_line_unchecked (xcb_connection_t *c,
                                         uint16_t          screen);

uint8_t *
xcb_xf86vidmode_get_mode_line_private (const xcb_xf86vidmode_get_mode_line_reply_t *R);

int
xcb_xf86vidmode_get_mode_line_private_length (const xcb_xf86vidmode_get_mode_line_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_mode_line_private_end (const xcb_xf86vidmode_get_mode_line_reply_t *R);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_mode_line_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_mode_line_reply_t *
xcb_xf86vidmode_get_mode_line_reply (xcb_connection_t                        *c,
                                     xcb_xf86vidmode_get_mode_line_cookie_t   cookie  /**< */,
                                     xcb_generic_error_t                    **e);

int
xcb_xf86vidmode_mod_mode_line_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_mod_mode_line_checked (xcb_connection_t *c,
                                       uint32_t          screen,
                                       uint16_t          hdisplay,
                                       uint16_t          hsyncstart,
                                       uint16_t          hsyncend,
                                       uint16_t          htotal,
                                       uint16_t          hskew,
                                       uint16_t          vdisplay,
                                       uint16_t          vsyncstart,
                                       uint16_t          vsyncend,
                                       uint16_t          vtotal,
                                       uint32_t          flags,
                                       uint32_t          privsize,
                                       const uint8_t    *private);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_mod_mode_line (xcb_connection_t *c,
                               uint32_t          screen,
                               uint16_t          hdisplay,
                               uint16_t          hsyncstart,
                               uint16_t          hsyncend,
                               uint16_t          htotal,
                               uint16_t          hskew,
                               uint16_t          vdisplay,
                               uint16_t          vsyncstart,
                               uint16_t          vsyncend,
                               uint16_t          vtotal,
                               uint32_t          flags,
                               uint32_t          privsize,
                               const uint8_t    *private);

uint8_t *
xcb_xf86vidmode_mod_mode_line_private (const xcb_xf86vidmode_mod_mode_line_request_t *R);

int
xcb_xf86vidmode_mod_mode_line_private_length (const xcb_xf86vidmode_mod_mode_line_request_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_mod_mode_line_private_end (const xcb_xf86vidmode_mod_mode_line_request_t *R);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_switch_mode_checked (xcb_connection_t *c,
                                     uint16_t          screen,
                                     uint16_t          zoom);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_switch_mode (xcb_connection_t *c,
                             uint16_t          screen,
                             uint16_t          zoom);

int
xcb_xf86vidmode_get_monitor_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_monitor_cookie_t
xcb_xf86vidmode_get_monitor (xcb_connection_t *c,
                             uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_monitor_cookie_t
xcb_xf86vidmode_get_monitor_unchecked (xcb_connection_t *c,
                                       uint16_t          screen);

xcb_xf86vidmode_syncrange_t *
xcb_xf86vidmode_get_monitor_hsync (const xcb_xf86vidmode_get_monitor_reply_t *R);

int
xcb_xf86vidmode_get_monitor_hsync_length (const xcb_xf86vidmode_get_monitor_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_monitor_hsync_end (const xcb_xf86vidmode_get_monitor_reply_t *R);

xcb_xf86vidmode_syncrange_t *
xcb_xf86vidmode_get_monitor_vsync (const xcb_xf86vidmode_get_monitor_reply_t *R);

int
xcb_xf86vidmode_get_monitor_vsync_length (const xcb_xf86vidmode_get_monitor_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_monitor_vsync_end (const xcb_xf86vidmode_get_monitor_reply_t *R);

char *
xcb_xf86vidmode_get_monitor_vendor (const xcb_xf86vidmode_get_monitor_reply_t *R);

int
xcb_xf86vidmode_get_monitor_vendor_length (const xcb_xf86vidmode_get_monitor_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_monitor_vendor_end (const xcb_xf86vidmode_get_monitor_reply_t *R);

void *
xcb_xf86vidmode_get_monitor_alignment_pad (const xcb_xf86vidmode_get_monitor_reply_t *R);

int
xcb_xf86vidmode_get_monitor_alignment_pad_length (const xcb_xf86vidmode_get_monitor_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_monitor_alignment_pad_end (const xcb_xf86vidmode_get_monitor_reply_t *R);

char *
xcb_xf86vidmode_get_monitor_model (const xcb_xf86vidmode_get_monitor_reply_t *R);

int
xcb_xf86vidmode_get_monitor_model_length (const xcb_xf86vidmode_get_monitor_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_monitor_model_end (const xcb_xf86vidmode_get_monitor_reply_t *R);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_monitor_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_monitor_reply_t *
xcb_xf86vidmode_get_monitor_reply (xcb_connection_t                      *c,
                                   xcb_xf86vidmode_get_monitor_cookie_t   cookie  /**< */,
                                   xcb_generic_error_t                  **e);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_lock_mode_switch_checked (xcb_connection_t *c,
                                          uint16_t          screen,
                                          uint16_t          lock);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_lock_mode_switch (xcb_connection_t *c,
                                  uint16_t          screen,
                                  uint16_t          lock);

int
xcb_xf86vidmode_get_all_mode_lines_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_all_mode_lines_cookie_t
xcb_xf86vidmode_get_all_mode_lines (xcb_connection_t *c,
                                    uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_all_mode_lines_cookie_t
xcb_xf86vidmode_get_all_mode_lines_unchecked (xcb_connection_t *c,
                                              uint16_t          screen);

xcb_xf86vidmode_mode_info_t *
xcb_xf86vidmode_get_all_mode_lines_modeinfo (const xcb_xf86vidmode_get_all_mode_lines_reply_t *R);

int
xcb_xf86vidmode_get_all_mode_lines_modeinfo_length (const xcb_xf86vidmode_get_all_mode_lines_reply_t *R);

xcb_xf86vidmode_mode_info_iterator_t
xcb_xf86vidmode_get_all_mode_lines_modeinfo_iterator (const xcb_xf86vidmode_get_all_mode_lines_reply_t *R);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_all_mode_lines_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_all_mode_lines_reply_t *
xcb_xf86vidmode_get_all_mode_lines_reply (xcb_connection_t                             *c,
                                          xcb_xf86vidmode_get_all_mode_lines_cookie_t   cookie  /**< */,
                                          xcb_generic_error_t                         **e);

int
xcb_xf86vidmode_add_mode_line_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_add_mode_line_checked (xcb_connection_t           *c,
                                       uint32_t                    screen,
                                       xcb_xf86vidmode_dotclock_t  dotclock,
                                       uint16_t                    hdisplay,
                                       uint16_t                    hsyncstart,
                                       uint16_t                    hsyncend,
                                       uint16_t                    htotal,
                                       uint16_t                    hskew,
                                       uint16_t                    vdisplay,
                                       uint16_t                    vsyncstart,
                                       uint16_t                    vsyncend,
                                       uint16_t                    vtotal,
                                       uint32_t                    flags,
                                       uint32_t                    privsize,
                                       xcb_xf86vidmode_dotclock_t  after_dotclock,
                                       uint16_t                    after_hdisplay,
                                       uint16_t                    after_hsyncstart,
                                       uint16_t                    after_hsyncend,
                                       uint16_t                    after_htotal,
                                       uint16_t                    after_hskew,
                                       uint16_t                    after_vdisplay,
                                       uint16_t                    after_vsyncstart,
                                       uint16_t                    after_vsyncend,
                                       uint16_t                    after_vtotal,
                                       uint32_t                    after_flags,
                                       const uint8_t              *private);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_add_mode_line (xcb_connection_t           *c,
                               uint32_t                    screen,
                               xcb_xf86vidmode_dotclock_t  dotclock,
                               uint16_t                    hdisplay,
                               uint16_t                    hsyncstart,
                               uint16_t                    hsyncend,
                               uint16_t                    htotal,
                               uint16_t                    hskew,
                               uint16_t                    vdisplay,
                               uint16_t                    vsyncstart,
                               uint16_t                    vsyncend,
                               uint16_t                    vtotal,
                               uint32_t                    flags,
                               uint32_t                    privsize,
                               xcb_xf86vidmode_dotclock_t  after_dotclock,
                               uint16_t                    after_hdisplay,
                               uint16_t                    after_hsyncstart,
                               uint16_t                    after_hsyncend,
                               uint16_t                    after_htotal,
                               uint16_t                    after_hskew,
                               uint16_t                    after_vdisplay,
                               uint16_t                    after_vsyncstart,
                               uint16_t                    after_vsyncend,
                               uint16_t                    after_vtotal,
                               uint32_t                    after_flags,
                               const uint8_t              *private);

uint8_t *
xcb_xf86vidmode_add_mode_line_private (const xcb_xf86vidmode_add_mode_line_request_t *R);

int
xcb_xf86vidmode_add_mode_line_private_length (const xcb_xf86vidmode_add_mode_line_request_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_add_mode_line_private_end (const xcb_xf86vidmode_add_mode_line_request_t *R);

int
xcb_xf86vidmode_delete_mode_line_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_delete_mode_line_checked (xcb_connection_t           *c,
                                          uint32_t                    screen,
                                          xcb_xf86vidmode_dotclock_t  dotclock,
                                          uint16_t                    hdisplay,
                                          uint16_t                    hsyncstart,
                                          uint16_t                    hsyncend,
                                          uint16_t                    htotal,
                                          uint16_t                    hskew,
                                          uint16_t                    vdisplay,
                                          uint16_t                    vsyncstart,
                                          uint16_t                    vsyncend,
                                          uint16_t                    vtotal,
                                          uint32_t                    flags,
                                          uint32_t                    privsize,
                                          const uint8_t              *private);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_delete_mode_line (xcb_connection_t           *c,
                                  uint32_t                    screen,
                                  xcb_xf86vidmode_dotclock_t  dotclock,
                                  uint16_t                    hdisplay,
                                  uint16_t                    hsyncstart,
                                  uint16_t                    hsyncend,
                                  uint16_t                    htotal,
                                  uint16_t                    hskew,
                                  uint16_t                    vdisplay,
                                  uint16_t                    vsyncstart,
                                  uint16_t                    vsyncend,
                                  uint16_t                    vtotal,
                                  uint32_t                    flags,
                                  uint32_t                    privsize,
                                  const uint8_t              *private);

uint8_t *
xcb_xf86vidmode_delete_mode_line_private (const xcb_xf86vidmode_delete_mode_line_request_t *R);

int
xcb_xf86vidmode_delete_mode_line_private_length (const xcb_xf86vidmode_delete_mode_line_request_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_delete_mode_line_private_end (const xcb_xf86vidmode_delete_mode_line_request_t *R);

int
xcb_xf86vidmode_validate_mode_line_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_validate_mode_line_cookie_t
xcb_xf86vidmode_validate_mode_line (xcb_connection_t           *c,
                                    uint32_t                    screen,
                                    xcb_xf86vidmode_dotclock_t  dotclock,
                                    uint16_t                    hdisplay,
                                    uint16_t                    hsyncstart,
                                    uint16_t                    hsyncend,
                                    uint16_t                    htotal,
                                    uint16_t                    hskew,
                                    uint16_t                    vdisplay,
                                    uint16_t                    vsyncstart,
                                    uint16_t                    vsyncend,
                                    uint16_t                    vtotal,
                                    uint32_t                    flags,
                                    uint32_t                    privsize,
                                    const uint8_t              *private);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_validate_mode_line_cookie_t
xcb_xf86vidmode_validate_mode_line_unchecked (xcb_connection_t           *c,
                                              uint32_t                    screen,
                                              xcb_xf86vidmode_dotclock_t  dotclock,
                                              uint16_t                    hdisplay,
                                              uint16_t                    hsyncstart,
                                              uint16_t                    hsyncend,
                                              uint16_t                    htotal,
                                              uint16_t                    hskew,
                                              uint16_t                    vdisplay,
                                              uint16_t                    vsyncstart,
                                              uint16_t                    vsyncend,
                                              uint16_t                    vtotal,
                                              uint32_t                    flags,
                                              uint32_t                    privsize,
                                              const uint8_t              *private);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_validate_mode_line_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_validate_mode_line_reply_t *
xcb_xf86vidmode_validate_mode_line_reply (xcb_connection_t                             *c,
                                          xcb_xf86vidmode_validate_mode_line_cookie_t   cookie  /**< */,
                                          xcb_generic_error_t                         **e);

int
xcb_xf86vidmode_switch_to_mode_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_switch_to_mode_checked (xcb_connection_t           *c,
                                        uint32_t                    screen,
                                        xcb_xf86vidmode_dotclock_t  dotclock,
                                        uint16_t                    hdisplay,
                                        uint16_t                    hsyncstart,
                                        uint16_t                    hsyncend,
                                        uint16_t                    htotal,
                                        uint16_t                    hskew,
                                        uint16_t                    vdisplay,
                                        uint16_t                    vsyncstart,
                                        uint16_t                    vsyncend,
                                        uint16_t                    vtotal,
                                        uint32_t                    flags,
                                        uint32_t                    privsize,
                                        const uint8_t              *private);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_switch_to_mode (xcb_connection_t           *c,
                                uint32_t                    screen,
                                xcb_xf86vidmode_dotclock_t  dotclock,
                                uint16_t                    hdisplay,
                                uint16_t                    hsyncstart,
                                uint16_t                    hsyncend,
                                uint16_t                    htotal,
                                uint16_t                    hskew,
                                uint16_t                    vdisplay,
                                uint16_t                    vsyncstart,
                                uint16_t                    vsyncend,
                                uint16_t                    vtotal,
                                uint32_t                    flags,
                                uint32_t                    privsize,
                                const uint8_t              *private);

uint8_t *
xcb_xf86vidmode_switch_to_mode_private (const xcb_xf86vidmode_switch_to_mode_request_t *R);

int
xcb_xf86vidmode_switch_to_mode_private_length (const xcb_xf86vidmode_switch_to_mode_request_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_switch_to_mode_private_end (const xcb_xf86vidmode_switch_to_mode_request_t *R);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_view_port_cookie_t
xcb_xf86vidmode_get_view_port (xcb_connection_t *c,
                               uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_view_port_cookie_t
xcb_xf86vidmode_get_view_port_unchecked (xcb_connection_t *c,
                                         uint16_t          screen);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_view_port_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_view_port_reply_t *
xcb_xf86vidmode_get_view_port_reply (xcb_connection_t                        *c,
                                     xcb_xf86vidmode_get_view_port_cookie_t   cookie  /**< */,
                                     xcb_generic_error_t                    **e);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_view_port_checked (xcb_connection_t *c,
                                       uint16_t          screen,
                                       uint32_t          x,
                                       uint32_t          y);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_view_port (xcb_connection_t *c,
                               uint16_t          screen,
                               uint32_t          x,
                               uint32_t          y);

int
xcb_xf86vidmode_get_dot_clocks_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_dot_clocks_cookie_t
xcb_xf86vidmode_get_dot_clocks (xcb_connection_t *c,
                                uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_dot_clocks_cookie_t
xcb_xf86vidmode_get_dot_clocks_unchecked (xcb_connection_t *c,
                                          uint16_t          screen);

uint32_t *
xcb_xf86vidmode_get_dot_clocks_clock (const xcb_xf86vidmode_get_dot_clocks_reply_t *R);

int
xcb_xf86vidmode_get_dot_clocks_clock_length (const xcb_xf86vidmode_get_dot_clocks_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_dot_clocks_clock_end (const xcb_xf86vidmode_get_dot_clocks_reply_t *R);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_dot_clocks_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_dot_clocks_reply_t *
xcb_xf86vidmode_get_dot_clocks_reply (xcb_connection_t                         *c,
                                      xcb_xf86vidmode_get_dot_clocks_cookie_t   cookie  /**< */,
                                      xcb_generic_error_t                     **e);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_client_version_checked (xcb_connection_t *c,
                                            uint16_t          major,
                                            uint16_t          minor);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_client_version (xcb_connection_t *c,
                                    uint16_t          major,
                                    uint16_t          minor);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_gamma_checked (xcb_connection_t *c,
                                   uint16_t          screen,
                                   uint32_t          red,
                                   uint32_t          green,
                                   uint32_t          blue);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_gamma (xcb_connection_t *c,
                           uint16_t          screen,
                           uint32_t          red,
                           uint32_t          green,
                           uint32_t          blue);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_gamma_cookie_t
xcb_xf86vidmode_get_gamma (xcb_connection_t *c,
                           uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_gamma_cookie_t
xcb_xf86vidmode_get_gamma_unchecked (xcb_connection_t *c,
                                     uint16_t          screen);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_gamma_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_gamma_reply_t *
xcb_xf86vidmode_get_gamma_reply (xcb_connection_t                    *c,
                                 xcb_xf86vidmode_get_gamma_cookie_t   cookie  /**< */,
                                 xcb_generic_error_t                **e);

int
xcb_xf86vidmode_get_gamma_ramp_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_gamma_ramp_cookie_t
xcb_xf86vidmode_get_gamma_ramp (xcb_connection_t *c,
                                uint16_t          screen,
                                uint16_t          size);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_gamma_ramp_cookie_t
xcb_xf86vidmode_get_gamma_ramp_unchecked (xcb_connection_t *c,
                                          uint16_t          screen,
                                          uint16_t          size);

uint16_t *
xcb_xf86vidmode_get_gamma_ramp_red (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

int
xcb_xf86vidmode_get_gamma_ramp_red_length (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_gamma_ramp_red_end (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

uint16_t *
xcb_xf86vidmode_get_gamma_ramp_green (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

int
xcb_xf86vidmode_get_gamma_ramp_green_length (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_gamma_ramp_green_end (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

uint16_t *
xcb_xf86vidmode_get_gamma_ramp_blue (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

int
xcb_xf86vidmode_get_gamma_ramp_blue_length (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_get_gamma_ramp_blue_end (const xcb_xf86vidmode_get_gamma_ramp_reply_t *R);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_gamma_ramp_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_gamma_ramp_reply_t *
xcb_xf86vidmode_get_gamma_ramp_reply (xcb_connection_t                         *c,
                                      xcb_xf86vidmode_get_gamma_ramp_cookie_t   cookie  /**< */,
                                      xcb_generic_error_t                     **e);

int
xcb_xf86vidmode_set_gamma_ramp_sizeof (const void  *_buffer);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will not cause
 * a reply to be generated. Any returned error will be
 * saved for handling by xcb_request_check().
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_gamma_ramp_checked (xcb_connection_t *c,
                                        uint16_t          screen,
                                        uint16_t          size,
                                        const uint16_t   *red,
                                        const uint16_t   *green,
                                        const uint16_t   *blue);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_void_cookie_t
xcb_xf86vidmode_set_gamma_ramp (xcb_connection_t *c,
                                uint16_t          screen,
                                uint16_t          size,
                                const uint16_t   *red,
                                const uint16_t   *green,
                                const uint16_t   *blue);

uint16_t *
xcb_xf86vidmode_set_gamma_ramp_red (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

int
xcb_xf86vidmode_set_gamma_ramp_red_length (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_set_gamma_ramp_red_end (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

uint16_t *
xcb_xf86vidmode_set_gamma_ramp_green (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

int
xcb_xf86vidmode_set_gamma_ramp_green_length (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_set_gamma_ramp_green_end (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

uint16_t *
xcb_xf86vidmode_set_gamma_ramp_blue (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

int
xcb_xf86vidmode_set_gamma_ramp_blue_length (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

xcb_generic_iterator_t
xcb_xf86vidmode_set_gamma_ramp_blue_end (const xcb_xf86vidmode_set_gamma_ramp_request_t *R);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_gamma_ramp_size_cookie_t
xcb_xf86vidmode_get_gamma_ramp_size (xcb_connection_t *c,
                                     uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_gamma_ramp_size_cookie_t
xcb_xf86vidmode_get_gamma_ramp_size_unchecked (xcb_connection_t *c,
                                               uint16_t          screen);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_gamma_ramp_size_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_gamma_ramp_size_reply_t *
xcb_xf86vidmode_get_gamma_ramp_size_reply (xcb_connection_t                              *c,
                                           xcb_xf86vidmode_get_gamma_ramp_size_cookie_t   cookie  /**< */,
                                           xcb_generic_error_t                          **e);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 */
xcb_xf86vidmode_get_permissions_cookie_t
xcb_xf86vidmode_get_permissions (xcb_connection_t *c,
                                 uint16_t          screen);

/**
 *
 * @param c The connection
 * @return A cookie
 *
 * Delivers a request to the X server.
 *
 * This form can be used only if the request will cause
 * a reply to be generated. Any returned error will be
 * placed in the event queue.
 */
xcb_xf86vidmode_get_permissions_cookie_t
xcb_xf86vidmode_get_permissions_unchecked (xcb_connection_t *c,
                                           uint16_t          screen);

/**
 * Return the reply
 * @param c      The connection
 * @param cookie The cookie
 * @param e      The xcb_generic_error_t supplied
 *
 * Returns the reply of the request asked by
 *
 * The parameter @p e supplied to this function must be NULL if
 * xcb_xf86vidmode_get_permissions_unchecked(). is used.
 * Otherwise, it stores the error if any.
 *
 * The returned value must be freed by the caller using free().
 */
xcb_xf86vidmode_get_permissions_reply_t *
xcb_xf86vidmode_get_permissions_reply (xcb_connection_t                          *c,
                                       xcb_xf86vidmode_get_permissions_cookie_t   cookie  /**< */,
                                       xcb_generic_error_t                      **e);


#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
