/*
 * Copyright (c) 2005-2020 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */
/*
 * @APPLE_FREE_COPYRIGHT@
 */

#ifndef _CONSOLE_SERIAL_PROTOS_H_
#define _CONSOLE_SERIAL_PROTOS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void serial_keyboard_init(void);
void serial_keyboard_start(void) __dead2;
void serial_keyboard_poll(void) __dead2;

extern uint32_t serialmode;

#define SERIALMODE_OUTPUT    0x01
#define SERIALMODE_INPUT     0x02
#define SERIALMODE_SYNCDRAIN 0x04
#define SERIALMODE_BASE_TTY  0x08 /* Load Base/Recovery/FVUnlock TTY */
#define SERIALMODE_NO_IOLOG  0x10 /* prevent IOLogs writing to serial */

extern uint32_t cons_ops_index;
extern const uint32_t nconsops;
extern bool disable_serial_output;
extern bool disable_iolog_serial_output;

void console_init(void);

int _serial_getc(bool wait);
int _vcgetc(bool wait);

struct console_ops {
	void (*putc)(char, bool);
	int (*getc)(bool);
};

boolean_t console_is_serial(void);
int switch_to_serial_console(void);
int switch_to_video_console(void);
void switch_to_old_console(int old_console);

#define SERIAL_CONS_OPS 0
#define VC_CONS_OPS 1


#ifdef __cplusplus
}
#endif


#endif /* _CONSOLE_SERIAL_PROTOS_H_ */
