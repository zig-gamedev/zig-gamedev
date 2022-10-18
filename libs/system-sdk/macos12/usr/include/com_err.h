/* $Copyright:
 *
 * Copyright 1998-2000 by the Massachusetts Institute of Technology.
 * 
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  Furthermore if you modify
 * this software you must label your software as modified software and not
 * distribute it in such a fashion that it might be confused with the
 * original MIT software. M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * Individual source code files are copyright MIT, Cygnus Support,
 * OpenVision, Oracle, Sun Soft, FundsXpress, and others.
 * 
 * Project Athena, Athena, Athena MUSE, Discuss, Hesiod, Kerberos, Moira,
 * and Zephyr are trademarks of the Massachusetts Institute of Technology
 * (MIT).  No commercial use of these trademarks may be made without prior
 * written permission of MIT.
 * 
 * "Commercial use" means use of a name in a product or other for-profit
 * manner.  It does NOT prevent a commercial firm from referring to the MIT
 * trademarks in order to convey information (although in doing so,
 * recognition of their trademark status should be given).
 * $
 */

/* $Header$ */

/* 
 *
 * KerberosComErr.h -- Functions to handle Kerberos errors.
 *
 */


#ifndef __KERBEROSCOMERR__
#define __KERBEROSCOMERR__

#if defined(macintosh) || (defined(__MACH__) && defined(__APPLE__))
#    include <TargetConditionals.h>
#    if TARGET_RT_MAC_CFM
#        error "Use KfM 4.0 SDK headers for CFM compilation."
#    endif
#endif

#include <sys/types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long errcode_t;
typedef void (*com_err_handler_t)
    (const char *whoami, errcode_t code, const char *format, va_list args);

struct error_table {
    const char * const * const messages;
    int32_t base;
    int32_t count;
};

/* ******************* */
/* Function prototypes */
/* ******************* */

void com_err    (const char *progname, errcode_t code, const char *format, ...);
void com_err_va (const char *progname, errcode_t code, const char *format, va_list args);

const char *error_message (errcode_t code);

com_err_handler_t set_com_err_hook(com_err_handler_t handler);
com_err_handler_t reset_com_err_hook(void);

errcode_t add_error_table    (const struct error_table *et);
errcode_t remove_error_table (const struct error_table *et);

#ifdef __cplusplus
}
#endif

#endif /* __KERBEROSCOMERR__ */
