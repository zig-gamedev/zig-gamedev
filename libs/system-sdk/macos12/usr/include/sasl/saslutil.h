/* saslutil.h -- various utility functions in SASL library
 */

#ifndef SASLUTIL_H
#define SASLUTIL_H 1

#ifndef SASL_H
#include "sasl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* base64 decode
 *  in     -- input data
 *  inlen  -- length of input data
 *  out    -- output data (may be same as in, must have enough space)
 *  outmax  -- max size of output buffer
 * result:
 *  outlen -- actual output length
 *
 * returns SASL_BADPROT on bad base64,
 *  SASL_BUFOVER if result won't fit
 *  SASL_OK on success
 */
LIBSASL_API int sasl_decode64(const char *in, unsigned inlen,
			      char *out, unsigned outmax, unsigned *outlen) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* base64 encode
 *  in      -- input data
 *  inlen   -- input data length
 *  out     -- output buffer (will be NUL terminated)
 *  outmax  -- max size of output buffer
 * result:
 *  outlen  -- gets actual length of output buffer (optional)
 * 
 * Returns SASL_OK on success, SASL_BUFOVER if result won't fit
 */
LIBSASL_API int sasl_encode64(const char *in, unsigned inlen,
			      char *out, unsigned outmax, unsigned *outlen) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* make a challenge string (NUL terminated)
 *  buf      -- buffer for result
 *  maxlen   -- max length of result
 *  hostflag -- 0 = don't include hostname, 1 = include hostname
 * returns final length or 0 if not enough space
 */
LIBSASL_API int sasl_mkchal(sasl_conn_t *conn, char *buf, 
			    unsigned maxlen, unsigned hostflag) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* verify a string is valid UTF-8
 * if len == 0, strlen(str) will be used.
 * returns SASL_BADPROT on error, SASL_OK on success
 */
LIBSASL_API int sasl_utf8verify(const char *str, unsigned len) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* create random pool seeded with OS-based params */
LIBSASL_API int sasl_randcreate(sasl_rand_t **rpool) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* free random pool from randcreate */
LIBSASL_API void sasl_randfree(sasl_rand_t **rpool) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* seed random number generator */
LIBSASL_API void sasl_randseed(sasl_rand_t *rpool, const char *seed,
			       unsigned len) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* generate random octets */
LIBSASL_API void sasl_rand(sasl_rand_t *rpool, char *buf, unsigned len) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* churn data into random number generator */
LIBSASL_API void sasl_churn(sasl_rand_t *rpool, const char *data,
			    unsigned len) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* erase a security sensitive buffer or password.
 *   Implementation may use recovery-resistant erase logic.  
 */
LIBSASL_API void sasl_erasebuffer(char *pass, unsigned len) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

/* Lowercase string in place */
LIBSASL_API char *sasl_strlower (char *val) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

LIBSASL_API int sasl_config_init(const char *filename) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

LIBSASL_API void sasl_config_done(void) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);

#ifdef WIN32
/* Just in case a different DLL defines this as well */
#if defined(NEED_GETOPT)
LIBSASL_API int getopt(int argc, char **argv, char *optstring) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);
#endif
LIBSASL_API char * getpass(const char *prompt) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_11,__IPHONE_NA,__IPHONE_NA);
#endif /* WIN32 */

#ifdef __cplusplus
}
#endif

#endif /* SASLUTIL_H */
