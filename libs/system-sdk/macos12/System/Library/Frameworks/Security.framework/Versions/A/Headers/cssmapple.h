/*
 * Copyright (c) 2000-2016 Apple Inc. All Rights Reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 *
 * cssmapple.h -- CSSM features specific to Apple's Implementation
 */

#ifndef _CSSMAPPLE_H_
#define _CSSMAPPLE_H_  1

#include <Security/SecBase.h>

#if SEC_OS_OSX_INCLUDES
#include <Security/cssmerr.h>
#include <Security/cssmtype.h>
#include <Security/x509defs.h>			/* for CSSM_APPLE_TP_CERT_REQUEST fields */
#include <Security/certextensions.h>	/* ditto */
#include <sys/types.h>					/* for the BSD *_t types */
#include <stdbool.h>
#endif /* SEC_OS_OSX */

#ifdef __cplusplus
extern "C" {
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#if SEC_OS_OSX
/* Guids for standard Apple addin modules. */

/* CSSM itself: {87191ca0-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidCssm;

/* File based DL (aka "Keychain DL"): {87191ca1-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleFileDL;

/* Core CSP (local space): {87191ca2-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleCSP;

/* Secure CSP/DL (aka "Keychain CSPDL): {87191ca3-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleCSPDL;

/* X509 Certificate CL: {87191ca4-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleX509CL;

/* X509 Certificate TP: {87191ca5-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleX509TP;

/* DLAP/OpenDirectory access DL: {87191ca6-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleLDAPDL;

/* TP for ".mac" related policies: {87191ca7-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleDotMacTP;

/* Smartcard CSP/DL: {87191ca8-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleSdCSPDL;

/* DL for ".mac" certificate access: {87191ca9-0fc9-11d4-849a-000502b52122} */
extern const CSSM_GUID gGuidAppleDotMacDL;


/* Apple defined WORDID values */
enum
{
	CSSM_WORDID_KEYCHAIN_PROMPT = CSSM_WORDID_VENDOR_START,
    CSSM_WORDID_KEYCHAIN_LOCK,
    CSSM_WORDID_KEYCHAIN_CHANGE_LOCK,
	CSSM_WORDID_PROCESS,
	CSSM_WORDID__RESERVED_1,		/* was used in 10.2 test seeds; no longer in use */
	CSSM_WORDID_SYMMETRIC_KEY,
	CSSM_WORDID_SYSTEM,
	CSSM_WORDID_KEY,
	CSSM_WORDID_PIN,
	CSSM_WORDID_PREAUTH,
	CSSM_WORDID_PREAUTH_SOURCE,
	CSSM_WORDID_ASYMMETRIC_KEY,
	CSSM_WORDID_PARTITION,
	CSSM_WORDID_KEYBAG_KEY,
	CSSM_WORDID__FIRST_UNUSED
};

/* Apple defined ACL subject and credential types */
enum
{
	CSSM_ACL_SUBJECT_TYPE_KEYCHAIN_PROMPT = CSSM_WORDID_KEYCHAIN_PROMPT,
	CSSM_ACL_SUBJECT_TYPE_PROCESS = CSSM_WORDID_PROCESS,
	CSSM_ACL_SUBJECT_TYPE_CODE_SIGNATURE = CSSM_WORDID_SIGNATURE,
	CSSM_ACL_SUBJECT_TYPE_COMMENT = CSSM_WORDID_COMMENT,
	CSSM_ACL_SUBJECT_TYPE_SYMMETRIC_KEY = CSSM_WORDID_SYMMETRIC_KEY,
	CSSM_ACL_SUBJECT_TYPE_PREAUTH = CSSM_WORDID_PREAUTH,
	CSSM_ACL_SUBJECT_TYPE_PREAUTH_SOURCE = CSSM_WORDID_PREAUTH_SOURCE,
	CSSM_ACL_SUBJECT_TYPE_ASYMMETRIC_KEY = CSSM_WORDID_ASYMMETRIC_KEY,
	CSSM_ACL_SUBJECT_TYPE_PARTITION = CSSM_WORDID_PARTITION,
};

enum
{
	CSSM_SAMPLE_TYPE_KEYCHAIN_PROMPT = CSSM_WORDID_KEYCHAIN_PROMPT,
    CSSM_SAMPLE_TYPE_KEYCHAIN_LOCK = CSSM_WORDID_KEYCHAIN_LOCK,
    CSSM_SAMPLE_TYPE_KEYCHAIN_CHANGE_LOCK = CSSM_WORDID_KEYCHAIN_CHANGE_LOCK,
	CSSM_SAMPLE_TYPE_PROCESS = CSSM_WORDID_PROCESS,
	CSSM_SAMPLE_TYPE_COMMENT = CSSM_WORDID_COMMENT,
	CSSM_SAMPLE_TYPE_RETRY_ID = CSSM_WORDID_PROPAGATE,
	CSSM_SAMPLE_TYPE_SYMMETRIC_KEY = CSSM_WORDID_SYMMETRIC_KEY,
	CSSM_SAMPLE_TYPE_PREAUTH = CSSM_WORDID_PREAUTH,
	CSSM_SAMPLE_TYPE_ASYMMETRIC_KEY = CSSM_WORDID_ASYMMETRIC_KEY,
	CSSM_SAMPLE_TYPE_KEYBAG_KEY = CSSM_WORDID_KEYBAG_KEY,
	// there is no CSSM_SAMPLE_TYPE_PREAUTH_SOURCE
};


/* Apple-defined ACL authorization tags */
enum {
	CSSM_ACL_AUTHORIZATION_CHANGE_ACL = CSSM_ACL_AUTHORIZATION_TAG_VENDOR_DEFINED_START,
	CSSM_ACL_AUTHORIZATION_CHANGE_OWNER,
	CSSM_ACL_AUTHORIZATION_PARTITION_ID,
	CSSM_ACL_AUTHORIZATION_INTEGRITY,

	// the "pre-auth" tags form a contiguous range of (up to) 64K pre-authorizations
	CSSM_ACL_AUTHORIZATION_PREAUTH_BASE =
		CSSM_ACL_AUTHORIZATION_TAG_VENDOR_DEFINED_START + 0x1000000,
	CSSM_ACL_AUTHORIZATION_PREAUTH_END = CSSM_ACL_AUTHORIZATION_PREAUTH_BASE + 0x10000
};

/* pre-authorization conversions (auth-tag to slot and back) */
#define CSSM_ACL_AUTHORIZATION_PREAUTH(slot) \
		(CSSM_ACL_AUTHORIZATION_PREAUTH_BASE + (slot))
#define CSSM_ACL_AUTHORIZATION_PREAUTH_SLOT(auth) \
		((auth) - CSSM_ACL_AUTHORIZATION_PREAUTH_BASE)
#define CSSM_ACL_AUTHORIZATION_IS_PREAUTH(auth) \
		((auth) >= CSSM_ACL_AUTHORIZATION_PREAUTH_BASE && \
		 (auth) < CSSM_ACL_AUTHORIZATION_PREAUTH_END)


/* Parameters and structures for Apple-defined ACL subjects and samples */

enum {	/* types of code signatures - item 1 of CSSM_ACL_SUBJECT_TYPE_CODE_SIGNATURE subjects */
    CSSM_ACL_CODE_SIGNATURE_INVALID = 0, /* standard OS X code signature */
    CSSM_ACL_CODE_SIGNATURE_OSX = 1		/* standard OS X code signature */
};

/* ACL subjects of type PROCESS */

enum {	/* PROCESS_SUBJECT mask fields */
    CSSM_ACL_MATCH_UID = 0x01,			/* match userid against uid field */
    CSSM_ACL_MATCH_GID = 0x02,			/* match groupid against gid field */
    CSSM_ACL_MATCH_HONOR_ROOT = 0x100,	/* let root (uid 0) match any userid */
    CSSM_ACL_MATCH_BITS = CSSM_ACL_MATCH_UID | CSSM_ACL_MATCH_GID
};

enum {	/* PROCESS_SUBJECT structure version field */
    CSSM_ACL_PROCESS_SELECTOR_CURRENT_VERSION = 0x101
};

typedef struct cssm_acl_process_subject_selector {	/* PROCESS_SUBJECT selector */
    uint16 version;			/* version of this selector */
    uint16 mask;			/* active fields mask */
    uint32 uid;				/* effective user id match */
    uint32 gid;				/* effective group id match */
} CSSM_ACL_PROCESS_SUBJECT_SELECTOR;

/* ACL subjects of type KEYCHAIN_PROMPT */

enum {	/* KEYCHAIN_PROMPT structure version field */
	CSSM_ACL_KEYCHAIN_PROMPT_CURRENT_VERSION = 0x101
};

enum {	/* KEYCHAIN_PROMPT operational flags */
	CSSM_ACL_KEYCHAIN_PROMPT_REQUIRE_PASSPHRASE = 0x0001, /* require re-entering of passphrase */
	/* the following bits are ignored by 10.4 and earlier */
	CSSM_ACL_KEYCHAIN_PROMPT_UNSIGNED = 0x0010,			/* prompt for unsigned clients */
	CSSM_ACL_KEYCHAIN_PROMPT_UNSIGNED_ACT = 0x0020,		/* UNSIGNED bit overrides system default */
	CSSM_ACL_KEYCHAIN_PROMPT_INVALID = 0x0040,			/* prompt for invalid signed clients */
	CSSM_ACL_KEYCHAIN_PROMPT_INVALID_ACT = 0x0080,		/* INVALID bit overrides system default */
};

typedef struct cssm_acl_keychain_prompt_selector { /* KEYCHAIN_PROMPT selector */
	uint16 version;			/* version of this selector */
	uint16 flags;			/* flag bits */
} CSSM_ACL_KEYCHAIN_PROMPT_SELECTOR;

/* ACL subjects of type CSSM_ACL_SUBJECT_TYPE_PREAUTH_SOURCE */
typedef uint32 CSSM_ACL_PREAUTH_TRACKING_STATE;
enum {	/* preauth tracking state */
	CSSM_ACL_PREAUTH_TRACKING_COUNT_MASK	= 0xff,		/* mask for count status */
	CSSM_ACL_PREAUTH_TRACKING_BLOCKED		= 0,		/* retries exhausted; the slot is blocked */
	/* 0 .. 255 is a count of (re)tries remaining */

	/* bits or'ed into any count given */
	CSSM_ACL_PREAUTH_TRACKING_UNKNOWN		= 0x40000000, /* status of slot is unknown (ignore count) */
	CSSM_ACL_PREAUTH_TRACKING_AUTHORIZED	= 0x80000000 /* the slot is currently authorized (or'ed in) */
};


/* Apple defined values of a CSSM_DB_ACCESS_TYPE */
enum {
	CSSM_DB_ACCESS_RESET			= 0x10000	/* clear pre-authentications (or'ed bit) */
};


/* Apple defined algorithm IDs */
enum
{
    CSSM_ALGID_APPLE_YARROW = CSSM_ALGID_VENDOR_DEFINED,
	CSSM_ALGID_AES,				/* RijnDael */
	CSSM_ALGID_FEE,				/* FEE Key Generation */
	CSSM_ALGID_FEE_MD5,			/* FEE/ElGamal signature w/ MD5 hash */
	CSSM_ALGID_FEE_SHA1,		/* FEE/ElGamal signature w/ SHA1 hash */
	CSSM_ALGID_FEED,			/* 1:1 FEE asymmetric encryption */
	CSSM_ALGID_FEEDEXP,			/* 2:1 FEE asymmetric encryption */
	CSSM_ALGID_ASC,				/* Apple Secure Compression */
	CSSM_ALGID_SHA1HMAC_LEGACY,	/* HMAC/SHA1, legacy compatible */
	CSSM_ALGID_KEYCHAIN_KEY,	/* derive or manipulate keychain master keys */
	CSSM_ALGID_PKCS12_PBE_ENCR,	/* PKCS12, encrypt/decrypt key */
	CSSM_ALGID_PKCS12_PBE_MAC,	/* PKCS12, MAC key */
	CSSM_ALGID_SECURE_PASSPHRASE,   /* passphrase acquired by SecurityServer */
	CSSM_ALGID_PBE_OPENSSL_MD5, /* traditional openssl key derivation */
	CSSM_ALGID_SHA256,			/* 256-bit SHA2 */
	CSSM_ALGID_SHA384,			/* 384-bit SHA2 */
	CSSM_ALGID_SHA512,			/* 512-bit SHA2 */
	CSSM_ALGID_ENTROPY_DEFAULT,	/* default entropy source of (CSP) device, if any */
	CSSM_ALGID_SHA224,			/* SHA2, 224 bit */
	CSSM_ALGID_SHA224WithRSA,	/* RSA signature on SHA224 digest */
	CSSM_ALGID_SHA256WithRSA,	/* RSA signature on SHA256 digest */
	CSSM_ALGID_SHA384WithRSA,	/* RSA signature on SHA384 digest */
	CSSM_ALGID_SHA512WithRSA,	/* RSA signature on SHA512 digest */
	CSSM_ALGID_OPENSSH1,		/* OpenSSH v1 RSA key wrapping */
	CSSM_ALGID_SHA224WithECDSA,	/* ECDSA signature on SHA224 digest */
	CSSM_ALGID_SHA256WithECDSA,	/* ECDSA signature on SHA256 digest */
	CSSM_ALGID_SHA384WithECDSA,	/* ECDSA signature on SHA384 digest */
	CSSM_ALGID_SHA512WithECDSA,	/* ECDSA signature on SHA512 digest */
	CSSM_ALGID_ECDSA_SPECIFIED,	/* ECDSA with separate digest algorithm specifier */
	CSSM_ALGID_ECDH_X963_KDF,	/* ECDH with X9.63 key derivation */
    CSSM_ALGID__FIRST_UNUSED
};

/* Apple defined padding */
enum
{
	/* RFC 2246 section E.2 for SSLv2 rollback detection */
    CSSM_PADDING_APPLE_SSLv2 = CSSM_PADDING_VENDOR_DEFINED
};


/* Apple defined keyblob formats */
enum {
	CSSM_KEYBLOB_RAW_FORMAT_VENDOR_DEFINED	= 0x80000000
};
enum {
	/* X509 SubjectPublicKeyInfo */
	CSSM_KEYBLOB_RAW_FORMAT_X509 = CSSM_KEYBLOB_RAW_FORMAT_VENDOR_DEFINED,
	/* OpenSSH v1 */
	CSSM_KEYBLOB_RAW_FORMAT_OPENSSH,
	/* openssl-style DSA private key */
	CSSM_KEYBLOB_RAW_FORMAT_OPENSSL,
	/* OpenSSH v2 */
	CSSM_KEYBLOB_RAW_FORMAT_OPENSSH2
};

/* Apple adds some "common" error codes. CDSA does not define an official start value for this. */
enum
{
    CSSM_CUSTOM_COMMON_ERROR_EXTENT = 0x00e0,

    CSSM_ERRCODE_NO_USER_INTERACTION =				0x00e0,
    CSSM_ERRCODE_USER_CANCELED =					0x00e1,
	CSSM_ERRCODE_SERVICE_NOT_AVAILABLE =			0x00e2,
	CSSM_ERRCODE_INSUFFICIENT_CLIENT_IDENTIFICATION = 0x00e3,
	CSSM_ERRCODE_DEVICE_RESET =						0x00e4,
	CSSM_ERRCODE_DEVICE_FAILED =					0x00e5,
	CSSM_ERRCODE_IN_DARK_WAKE =				0x00e6
};

enum {
	CSSMERR_CSSM_NO_USER_INTERACTION =  -2147417888,
	CSSMERR_AC_NO_USER_INTERACTION =    -2147405600,
	CSSMERR_CSP_NO_USER_INTERACTION =   -2147415840,
	CSSMERR_CL_NO_USER_INTERACTION =    -2147411744,
	CSSMERR_DL_NO_USER_INTERACTION =    -2147413792,
	CSSMERR_TP_NO_USER_INTERACTION =    -2147409696,

	CSSMERR_CSSM_USER_CANCELED =        -2147417887,
	CSSMERR_AC_USER_CANCELED =          -2147405599,
	CSSMERR_CSP_USER_CANCELED =         -2147415839,
	CSSMERR_CL_USER_CANCELED =          -2147411743,
	CSSMERR_DL_USER_CANCELED =          -2147413791,
	CSSMERR_TP_USER_CANCELED =          -2147409695,

	CSSMERR_CSSM_SERVICE_NOT_AVAILABLE =    -2147417886,
	CSSMERR_AC_SERVICE_NOT_AVAILABLE =      -2147405598,
	CSSMERR_CSP_SERVICE_NOT_AVAILABLE =     -2147415838,
	CSSMERR_CL_SERVICE_NOT_AVAILABLE =      -2147411742,
	CSSMERR_DL_SERVICE_NOT_AVAILABLE =      -2147413790,
	CSSMERR_TP_SERVICE_NOT_AVAILABLE =      -2147409694,

	CSSMERR_CSSM_INSUFFICIENT_CLIENT_IDENTIFICATION =   -2147417885,
	CSSMERR_AC_INSUFFICIENT_CLIENT_IDENTIFICATION =     -2147405597,
	CSSMERR_CSP_INSUFFICIENT_CLIENT_IDENTIFICATION =    -2147415837,
	CSSMERR_CL_INSUFFICIENT_CLIENT_IDENTIFICATION =     -2147411741,
	CSSMERR_DL_INSUFFICIENT_CLIENT_IDENTIFICATION =     -2147413789,
	CSSMERR_TP_INSUFFICIENT_CLIENT_IDENTIFICATION =     -2147409693,

	CSSMERR_CSSM_DEVICE_RESET =     -2147417884,
	CSSMERR_AC_DEVICE_RESET =       -2147405596,
	CSSMERR_CSP_DEVICE_RESET =      -2147415836,
	CSSMERR_CL_DEVICE_RESET =       -2147411740,
	CSSMERR_DL_DEVICE_RESET =       -2147413788,
	CSSMERR_TP_DEVICE_RESET =       -2147409692,

	CSSMERR_CSSM_DEVICE_FAILED =    -2147417883,
	CSSMERR_AC_DEVICE_FAILED =      -2147405595,
	CSSMERR_CSP_DEVICE_FAILED =     -2147415835,
	CSSMERR_CL_DEVICE_FAILED =      -2147411739,
	CSSMERR_DL_DEVICE_FAILED =      -2147413787,
	CSSMERR_TP_DEVICE_FAILED =      -2147409691,

	CSSMERR_CSSM_IN_DARK_WAKE =     -2147417882,
	CSSMERR_AC_IN_DARK_WAKE =       -2147405594,
	CSSMERR_CSP_IN_DARK_WAKE =      -2147415834,
	CSSMERR_CL_IN_DARK_WAKE =       -2147411738,
	CSSMERR_DL_IN_DARK_WAKE =       -2147413786,
	CSSMERR_TP_IN_DARK_WAKE =       -2147409690,
};

/* AppleCSPDL, AppleCSP private error codes. */
enum {
	CSSMERR_CSP_APPLE_ADD_APPLICATION_ACL_SUBJECT = -2147415040,
	/* An attempt was made to use a public key which is incomplete due to
       the lack of algorithm-specific parameters. */
	CSSMERR_CSP_APPLE_PUBLIC_KEY_INCOMPLETE = -2147415039,

	/* a code signature match failed */
	CSSMERR_CSP_APPLE_SIGNATURE_MISMATCH = -2147415038,

	/* Key start date invalid */
	CSSMERR_CSP_APPLE_INVALID_KEY_START_DATE = -2147415037,
	/* Key end date invalid */
	CSSMERR_CSP_APPLE_INVALID_KEY_END_DATE = -2147415036,

	/* Keychain Syncing error codes */
	CSSMERR_CSPDL_APPLE_DL_CONVERSION_ERROR = -2147415035,

	/* SSLv2 padding check: rollback attack detected */
	CSSMERR_CSP_APPLE_SSLv2_ROLLBACK = -2147415034,
};


/* AppleFileDL record types. */
enum
{
    CSSM_DL_DB_RECORD_GENERIC_PASSWORD = CSSM_DB_RECORDTYPE_APP_DEFINED_START + 0,
    CSSM_DL_DB_RECORD_INTERNET_PASSWORD = CSSM_DB_RECORDTYPE_APP_DEFINED_START + 1,
    CSSM_DL_DB_RECORD_APPLESHARE_PASSWORD = CSSM_DB_RECORDTYPE_APP_DEFINED_START + 2,

    CSSM_DL_DB_RECORD_X509_CERTIFICATE = CSSM_DB_RECORDTYPE_APP_DEFINED_START + 0x1000,
	CSSM_DL_DB_RECORD_USER_TRUST,
	CSSM_DL_DB_RECORD_X509_CRL,
	CSSM_DL_DB_RECORD_UNLOCK_REFERRAL,
	CSSM_DL_DB_RECORD_EXTENDED_ATTRIBUTE,
    CSSM_DL_DB_RECORD_METADATA = CSSM_DB_RECORDTYPE_APP_DEFINED_START + 0x8000
};

/* AppleFileDL extentions: passthrough ids */
enum {
	// Toggle whether or not to autocommit after modifying the database.
	// The input parameter is a CSSM_BOOL, where TRUE turns autocommit on
	// and FALSE turns it off.
	CSSM_APPLEFILEDL_TOGGLE_AUTOCOMMIT,

	// Commit any pending changes to the database.
	CSSM_APPLEFILEDL_COMMIT,

	// Rollback and discard any pending changes to the database.
	CSSM_APPLEFILEDL_ROLLBACK,

    // Try to take the file lock on the underlying database
    // Calling commit or rollback will release the lock
    CSSM_APPLEFILEDL_TAKE_FILE_LOCK,

    // Make a backup of this database in a new file
    CSSM_APPLEFILEDL_MAKE_BACKUP,

    // Make a copy of this database
    CSSM_APPLEFILEDL_MAKE_COPY,

    // Delete this database
    CSSM_APPLEFILEDL_DELETE_FILE,
};

/* UNLOCK_REFERRAL "type" attribute values */
enum {
	CSSM_APPLE_UNLOCK_TYPE_KEY_DIRECT			= 1,	// master secret key stored directly
	CSSM_APPLE_UNLOCK_TYPE_WRAPPED_PRIVATE		= 2,	// master key wrapped by public key
	CSSM_APPLE_UNLOCK_TYPE_KEYBAG				= 3		// master key wrapped via keybag
};

/* Apple DL private error codes. */
enum
{
	/* The OpenParameters argument passed to CSSM_DL_DbCreate or CSSM_DL_DbOpen
	   was neither NULL nor a pointer to a valid CSSM_APPLEDL_OPEN_PARAMETERS
	   structure. */
	CSSMERR_APPLEDL_INVALID_OPEN_PARAMETERS =		-2147412992,

	/* an operation failed because the disk was full */
	CSSMERR_APPLEDL_DISK_FULL = 					-2147412991,

	/* an operation failed because a disk quota was exceeded */
	CSSMERR_APPLEDL_QUOTA_EXCEEDED =				-2147412990,

	/* an operation failed because a file was too large */
	CSSMERR_APPLEDL_FILE_TOO_BIG = 					-2147412989,

    /* a keychain database's internal information ("blob") is invalid */
    CSSMERR_APPLEDL_INVALID_DATABASE_BLOB =			-2147412988,

    /* a keychain database's internal information ("blob") is invalid */
    CSSMERR_APPLEDL_INVALID_KEY_BLOB =				-2147412987,

    /* the internal data format version for a database's internal information ("blob") is invalid */
    CSSMERR_APPLEDL_INCOMPATIBLE_DATABASE_BLOB =	-2147412986,

    /* the internal data format version for a database's internal information ("blob") is invalid */
    CSSMERR_APPLEDL_INCOMPATIBLE_KEY_BLOB =			-2147412985,
};

/* Apple X509TP private error codes. */
enum
{
	/* Host name mismatch */
	CSSMERR_APPLETP_HOSTNAME_MISMATCH =				-2147408896,
	/* Non-understood extension with Critical flag true */
	CSSMERR_APPLETP_UNKNOWN_CRITICAL_EXTEN =		-2147408895,
	/* Basic Constraints extension required per policy, but not present */
	CSSMERR_APPLETP_NO_BASIC_CONSTRAINTS =			-2147408894,
	/* Invalid BasicConstraints.CA */
	CSSMERR_APPLETP_INVALID_CA =					-2147408893,
	/* Invalid Authority Key ID */
	CSSMERR_APPLETP_INVALID_AUTHORITY_ID =			-2147408892,
	/* Invalid Subject Key ID */
	CSSMERR_APPLETP_INVALID_SUBJECT_ID =			-2147408891,
	/* Invalid Key Usage for policy */
	CSSMERR_APPLETP_INVALID_KEY_USAGE =				-2147408890,
	/* Invalid Extended Key Usage for policy */
	CSSMERR_APPLETP_INVALID_EXTENDED_KEY_USAGE =	-2147408889,
	/* Invalid Subject/Authority Key ID Linkage */
	CSSMERR_APPLETP_INVALID_ID_LINKAGE =			-2147408888,
	/* PathLengthConstraint exceeded */
	CSSMERR_APPLETP_PATH_LEN_CONSTRAINT =			-2147408887,
	/* Cert group terminated at a root cert which did not self-verify */
	CSSMERR_APPLETP_INVALID_ROOT =					-2147408886,
	/* CRL expired */
	CSSMERR_APPLETP_CRL_EXPIRED =					-2147408885,
    /* CRL not valid yet */
	CSSMERR_APPLETP_CRL_NOT_VALID_YET =				-2147408884,
	/* Cannot find appropriate CRL */
	CSSMERR_APPLETP_CRL_NOT_FOUND =					-2147408883,
	/* specified CRL server down */
	CSSMERR_APPLETP_CRL_SERVER_DOWN =				-2147408882,
	/* illegible CRL distribution point URL */
	CSSMERR_APPLETP_CRL_BAD_URI =					-2147408881,
	/* Unknown critical cert extension */
	CSSMERR_APPLETP_UNKNOWN_CERT_EXTEN =			-2147408880,
    /* Unknown critical CRL extension */
	CSSMERR_APPLETP_UNKNOWN_CRL_EXTEN =				-2147408879,
	/* CRL not verifiable to anchor or root */
	CSSMERR_APPLETP_CRL_NOT_TRUSTED =				-2147408878,
	/* CRL verified to untrusted root */
	CSSMERR_APPLETP_CRL_INVALID_ANCHOR_CERT =		-2147408877,
	/* CRL failed policy verification */
	CSSMERR_APPLETP_CRL_POLICY_FAIL =				-2147408876,
	/* IssuingDistributionPoint extension violation */
	CSSMERR_APPLETP_IDP_FAIL =						-2147408875,
	/* Cert not found at specified issuerAltName */
	CSSMERR_APPLETP_CERT_NOT_FOUND_FROM_ISSUER =	-2147408874,
	/* Bad cert obtained from specified issuerAltName */
	CSSMERR_APPLETP_BAD_CERT_FROM_ISSUER =			-2147408873,
	/* S/MIME Email address mismatch */
	CSSMERR_APPLETP_SMIME_EMAIL_ADDRS_NOT_FOUND =	-2147408872,
	/* Appropriate S/MIME ExtendedKeyUsage not found */
	CSSMERR_APPLETP_SMIME_BAD_EXT_KEY_USE =			-2147408871,
	/* S/MIME KeyUsage incompatibility */
	CSSMERR_APPLETP_SMIME_BAD_KEY_USE =	  			-2147408870,
	/* S/MIME, cert with KeyUsage flagged !critical */
	CSSMERR_APPLETP_SMIME_KEYUSAGE_NOT_CRITICAL =	-2147408869,
	/* S/MIME, leaf with empty subject name and no email addrs
	   in SubjectAltName */
	CSSMERR_APPLETP_SMIME_NO_EMAIL_ADDRS =			-2147408868,
	/* S/MIME, leaf with empty subject name, SubjectAltName
	   not critical */
	CSSMERR_APPLETP_SMIME_SUBJ_ALT_NAME_NOT_CRIT =	-2147408867,
	/* Appropriate SSL ExtendedKeyUsage not found */
	CSSMERR_APPLETP_SSL_BAD_EXT_KEY_USE =			-2147408866,
	/* unparseable OCSP response */
	CSSMERR_APPLETP_OCSP_BAD_RESPONSE =				-2147408865,
	/* unparseable OCSP request */
	CSSMERR_APPLETP_OCSP_BAD_REQUEST =				-2147408864,
	/* OCSP service unavailable */
	CSSMERR_APPLETP_OCSP_UNAVAILABLE =				-2147408863,
	/* OCSP status: cert unrecognized */
	CSSMERR_APPLETP_OCSP_STATUS_UNRECOGNIZED =		-2147408862,
	/* revocation check not successful for each cert */
	CSSMERR_APPLETP_INCOMPLETE_REVOCATION_CHECK =	-2147408861,
	/* general network error */
	CSSMERR_APPLETP_NETWORK_FAILURE =				-2147408860,
	/* OCSP response not verifiable to anchor or root */
	CSSMERR_APPLETP_OCSP_NOT_TRUSTED =				-2147408859,
	/* OCSP response verified to untrusted root */
	CSSMERR_APPLETP_OCSP_INVALID_ANCHOR_CERT =		-2147408858,
	/* OCSP response signature error */
	CSSMERR_APPLETP_OCSP_SIG_ERROR =				-2147408857,
	/* No signer for OCSP response found */
	CSSMERR_APPLETP_OCSP_NO_SIGNER =				-2147408856,
	/* OCSP responder status: malformed request */
	CSSMERR_APPLETP_OCSP_RESP_MALFORMED_REQ =		-2147408855,
	/* OCSP responder status: internal error */
	CSSMERR_APPLETP_OCSP_RESP_INTERNAL_ERR =		-2147408854,
	/* OCSP responder status: try later */
	CSSMERR_APPLETP_OCSP_RESP_TRY_LATER =			-2147408853,
	/* OCSP responder status: signature required */
	CSSMERR_APPLETP_OCSP_RESP_SIG_REQUIRED =		-2147408852,
	/* OCSP responder status: unauthorized */
	CSSMERR_APPLETP_OCSP_RESP_UNAUTHORIZED =		-2147408851,
	/* OCSP response nonce did not match request */
	CSSMERR_APPLETP_OCSP_NONCE_MISMATCH =			-2147408850,
	/* Illegal cert chain length for Code Signing  */
	CSSMERR_APPLETP_CS_BAD_CERT_CHAIN_LENGTH =		-2147408849,
	/* Missing Basic Constraints for Code Signing */
	CSSMERR_APPLETP_CS_NO_BASIC_CONSTRAINTS =		-2147408848,
	/* Bad PathLengthConstraint for Code Signing */
	CSSMERR_APPLETP_CS_BAD_PATH_LENGTH =			-2147408847,
	/* Missing ExtendedKeyUsage for Code Signing */
	CSSMERR_APPLETP_CS_NO_EXTENDED_KEY_USAGE =		-2147408846,
	/* Development style Code Signing Cert Detected */
	CSSMERR_APPLETP_CODE_SIGN_DEVELOPMENT =			-2147408845,
	/* Illegal cert chain length for Resource Signing  */
	CSSMERR_APPLETP_RS_BAD_CERT_CHAIN_LENGTH =		-2147408844,
	/* Bad extended key usage for Resource Signing */
	CSSMERR_APPLETP_RS_BAD_EXTENDED_KEY_USAGE =		-2147408843,
	/* Trust Setting: deny */
	CSSMERR_APPLETP_TRUST_SETTING_DENY =			-2147408842,
	/* Invalid empty SubjectName */
	CSSMERR_APPLETP_INVALID_EMPTY_SUBJECT = 		-2147408841,
	/* Unknown critical Qualified Cert Statement ID */
	CSSMERR_APPLETP_UNKNOWN_QUAL_CERT_STATEMENT = 	-2147408840,
	/* Missing required extension */
	CSSMERR_APPLETP_MISSING_REQUIRED_EXTENSION =    -2147408839,
	/* Extended key usage not marked critical */
	CSSMERR_APPLETP_EXT_KEYUSAGE_NOT_CRITICAL =     -2147408838,
	/* Required name or identifier not present */
	CSSMERR_APPLETP_IDENTIFIER_MISSING =            -2147408837,
	/* Certificate authority pinning mismatch */
	CSSMERR_APPLETP_CA_PIN_MISMATCH =               -2147408836,
	/* Leaf pinning mismatch */
	CSSMERR_APPLETP_LEAF_PIN_MISMATCH =             -2147408835,
};

/* Apple .mac TP private error codes. */
enum
{
	/* cert request queued */
	CSSMERR_APPLE_DOTMAC_REQ_QUEUED =				-2147408796,
	/* cert request redirected */
	CSSMERR_APPLE_DOTMAC_REQ_REDIRECT =				-2147408795,
	/* general server-reported error */
	CSSMERR_APPLE_DOTMAC_REQ_SERVER_ERR =			-2147408794,
	/* server-reported parameter error */
	CSSMERR_APPLE_DOTMAC_REQ_SERVER_PARAM =			-2147408793,
	/* server-reported authorization error */
	CSSMERR_APPLE_DOTMAC_REQ_SERVER_AUTH =			-2147408792,
	/* server-reported unimplemented */
	CSSMERR_APPLE_DOTMAC_REQ_SERVER_UNIMPL =		-2147408791,
	/* server-reported not available */
	CSSMERR_APPLE_DOTMAC_REQ_SERVER_NOT_AVAIL =		-2147408790,
	/* server-reported already exists */
	CSSMERR_APPLE_DOTMAC_REQ_SERVER_ALREADY_EXIST = -2147408789,
	/* server-reported service error */
	CSSMERR_APPLE_DOTMAC_REQ_SERVER_SERVICE_ERROR = -2147408788,
	/* request already pending for specified user */
	CSSMERR_APPLE_DOTMAC_REQ_IS_PENDING	=			-2147408787,
	/* no request pending for specified user */
	CSSMERR_APPLE_DOTMAC_NO_REQ_PENDING	=			-2147408786,
	/* CSR failed to verify */
	CSSMERR_APPLE_DOTMAC_CSR_VERIFY_FAIL =			-2147408785,
	/* server reported failed consistency check */
	CSSMERR_APPLE_DOTMAC_FAILED_CONSISTENCY_CHECK =	-2147408784,
};

enum
{
	CSSM_APPLEDL_OPEN_PARAMETERS_VERSION =			1u
};

enum cssm_appledl_open_parameters_mask
{
	kCSSM_APPLEDL_MASK_MODE =			(1 << 0)
};

/* Pass a CSSM_APPLEDL_OPEN_PARAMETERS_PTR as the OpenParameters argument to
   CSSM_DL_DbCreate or CSSM_DL_DbOpen.  When using this struct, you must zero
   out the entire struct before setting any additional parameters to ensure
   forward compatibility.  */
typedef struct cssm_appledl_open_parameters
{
	uint32 length;	/* Should be sizeof(CSSM_APPLEDL_OPEN_PARAMETERS). */
	uint32 version;	/* Should be CSSM_APPLEDL_OPEN_PARAMETERS_VERSION. */

	/* If no OpenParameters are specified, autoCommit is on (!CSSM_FALSE) by default.
	   When autoCommit is on (!CSSM_FALSE), changes made to the Db are written to disk
	   before returning from each function.
	   When autoCommit is off (CSSM_FALSE), changes made to the database are not guaranteed
	   to be written to disk until the Db is closed.  This is useful for bulk writes.
	   Be aware that if autoCommit is off, changes made in previous calls to the DL might
	   get rolled back if a new modification operation fails. */
	CSSM_BOOL autoCommit;

	/* Mask marking which of the following fields are to be used. */
	uint32 mask;

	/* When calling DbCreate, the initial mode to create the database file with; ignored on DbOpen.  You must set the kCSSM_APPLEDL_MASK_MODE bit in mask or mode is ignored.  */
	mode_t mode;
} CSSM_APPLEDL_OPEN_PARAMETERS, *CSSM_APPLEDL_OPEN_PARAMETERS_PTR;


/* AppleCSPDL passthough ids */
enum
{
	/* Tell the SecurityServer to lock the database specified by the DLDBHandle argument.
	   The InputParams and OutputParams arguments are ignored. */
	CSSM_APPLECSPDL_DB_LOCK =			0,

	/* Tell the SecurityServer to unlock the database specified by the DLDBHandle argument.
	   The InputParameters argument is a CSSM_DATA_PTR containing the password, or NULL if
	   the SecurityServer should prompt for the password.
	   The OutputParams argument is ignored.
	   The SecurityServer will put up UI (though the SecurityAgent) when this function is called
	   iff InputParameters is NULL.  */
	CSSM_APPLECSPDL_DB_UNLOCK =			1,

	/* Ask the SecurityServer to get the db settings specified for the database
	   specified by the DLDBHandle argument.  The settings are returned in the OutputParameters argument.
	   The OutputParameters argument is a pointer to a CSSM_APPLECSPDL_DB_SETTINGS_PARAMETERS_PTR.
	   Upon successful completion, the AppleCSPDL will have allocated a
	   CSSM_APPLECSPDL_DB_SETTINGS_PARAMETERS structure using the application-specified
	   allocators for the DL attachment specified by the DLDBHandle argument.  The structure will contain
	   the current database settings for the specified database.  The client should free the
	   CSSM_APPLECSPDL_DB_SETTINGS_PARAMETERS_PTR after it has finished using it.
	   The InputParameters argument is ignored.
	   The SecurityServer might put up UI (though the SecurityAgent) when this function is called.  */
	CSSM_APPLECSPDL_DB_GET_SETTINGS =	2,

	/* Tell the SecurityServer to set the db settings specified in InputParameters on the database
	   specified by the DLDBHandle argument.
	   The InputParameters argument is a const CSSM_APPLECSPDL_DB_SETTINGS_PARAMETERS * containing
	   the new settings for the specified database.
	   The OutputParams argument is ignored.
	   The SecurityServer might put up UI (though the SecurityAgent) when this function is called.  */
	CSSM_APPLECSPDL_DB_SET_SETTINGS =	3,

	/* Ask the SecurityServer whether the database specified by the DLDBHandle argument is locked.
	   The InputParameters argument is ignored.
	   The OutputParameters argument is a pointer to a CSSM_APPLECSPDL_DB_IS_LOCKED_PARAMETERS_PTR.
	   Upon successful completion, the AppleCSPDL will have allocated a
	   CSSM_APPLECSPDL_DB_IS_LOCKED_PARAMETERS structure using the application-specified
	   allocators for the DL attachment specified by the DLDBHandle argument.  The structure will contain
	   the current lock status for the specified database.  The client should free the
	   CSSM_APPLECSPDL_DB_IS_LOCKED_PARAMETERS_PTR after it has finished using it.
	   The SecurityServer will put up UI (though the SecurityAgent) when this function is called. */
	CSSM_APPLECSPDL_DB_IS_LOCKED =		4,

	/* Tell the SecurityServer to change the password for the database specified by
	   the DLDBHandle.

	   The InputParameters argument is a const CSSM_APPLECSPDL_DB_CHANGE_PASSWORD_PARAMETERS * containing
	   a CSSM_ACCESS_CREDENTIALS * which determines how the password will be changed.  If the
	   accessCredentials are NULL, the SecurityAgent will prompt for the old and the new password for the
	   specified database.  If credentials are specified, there should be 2 entries:
	   1. a 3-element list containing:
	   CSSM_WORDID_KEYCHAIN_LOCK, CSSM_SAMPLE_TYPE_PASSWORD, and the old password.
	   2. a 3-element list containing:
	   CSSM_WORDID_KEYCHAIN_CHANGE_LOCK, CSSM_SAMPLE_TYPE_PASSWORD, and the new password.

	   The OutputParams argument is ignored.
	   The SecurityServer might put up UI (though the SecurityAgent) when this function is called.  */
	CSSM_APPLECSPDL_DB_CHANGE_PASSWORD =5,

	/* Return the SecurityServer database handle for the database specified by the DLDBHandle */
	CSSM_APPLECSPDL_DB_GET_HANDLE =		6,

	/* Given a CSSM_KEY for the CSPDL, return the SecurityServer key handle */
	CSSM_APPLESCPDL_CSP_GET_KEYHANDLE =	7,

	CSSM_APPLE_PRIVATE_CSPDL_CODE_8 = 8,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_9 = 9,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_10 = 10,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_11 = 11,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_12 = 12,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_13 = 13,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_14 = 14,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_15 = 15,
	CSSM_APPLE_PRIVATE_CSPDL_CODE_16 = 16,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_17 = 17,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_18 = 18,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_19 = 19,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_20 = 20,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_21 = 21,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_22 = 22,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_23 = 23,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_24 = 24,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_25 = 25,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_26 = 26,
    CSSM_APPLE_PRIVATE_CSPDL_CODE_27 = 27,

	/* Given a CSSM_KEY_PTR in any format, obtain the SHA-1 hash of the
	 * associated key blob.
	 * Key is specified in CSSM_CSP_CreatePassThroughContext.
	 * Hash is allocated bythe CSP, in the App's memory, and returned
	 * in *outData. */
	CSSM_APPLECSP_KEYDIGEST = 			0x100
};



/* AppleCSPDL passthough parameters */
typedef struct cssm_applecspdl_db_settings_parameters
{
	uint32 idleTimeout;				// seconds idle timeout lock
	uint8 lockOnSleep;				// lock database when system sleeps
} CSSM_APPLECSPDL_DB_SETTINGS_PARAMETERS, *CSSM_APPLECSPDL_DB_SETTINGS_PARAMETERS_PTR;

/* AppleCSPDL passthough parameters */
typedef struct cssm_applecspdl_db_is_locked_parameters
{
	uint8 isLocked;				// True iff the database is locked
} CSSM_APPLECSPDL_DB_IS_LOCKED_PARAMETERS, *CSSM_APPLECSPDL_DB_IS_LOCKED_PARAMETERS_PTR;

/* AppleCSPDL passthough parameters */
typedef struct cssm_applecspdl_db_change_password_parameters
{
	CSSM_ACCESS_CREDENTIALS *accessCredentials;
} CSSM_APPLECSPDL_DB_CHANGE_PASSWORD_PARAMETERS, *CSSM_APPLECSPDL_DB_CHANGE_PASSWORD_PARAMETERS_PTR;

/* Custom wrapped key formats */
enum {
	CSSM_KEYBLOB_WRAPPED_FORMAT_APPLE_CUSTOM = 100,
	CSSM_KEYBLOB_WRAPPED_FORMAT_OPENSSL,			// traditional openssl
	CSSM_KEYBLOB_WRAPPED_FORMAT_OPENSSH1			// OpenSSH v1
};

/*
 * Custom context attributes for AppleCSP.
 */
enum {
	CSSM_ATTRIBUTE_VENDOR_DEFINED	= 0x800000
};

enum {
	/*
	 * Public Key attribute for use with CSSM_ALGID_FEED.
	 */
    CSSM_ATTRIBUTE_PUBLIC_KEY =
			(CSSM_ATTRIBUTE_DATA_KEY | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 0)),

	/*
	 * FEE key attributes.
	 * See CSSM_FEE_PRIME_TYPE_xxx, CSSM_FEE_CURVE_TYPE_xxx enums, below.
	 */
	CSSM_ATTRIBUTE_FEE_PRIME_TYPE =
			(CSSM_ATTRIBUTE_DATA_UINT32 | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 1)),
	CSSM_ATTRIBUTE_FEE_CURVE_TYPE =
			(CSSM_ATTRIBUTE_DATA_UINT32 | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 2)),

	/*
	 * Apple Secure Compression (ComCryption) optimization.
	 * See CSSM_ASC_OPTIMIZE_xxx, enums, below.
	 */
	CSSM_ATTRIBUTE_ASC_OPTIMIZATION =
			(CSSM_ATTRIBUTE_DATA_UINT32 | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 3)),

	/*
	 * RSA blinding. Value is integer, nonzero (blinding on) or zero.
	 */
	CSSM_ATTRIBUTE_RSA_BLINDING =
			(CSSM_ATTRIBUTE_DATA_UINT32 | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 4)),

	/*
	 * Additional public key from which to obtain algorithm-specific
	 * parameters.
	 */
	CSSM_ATTRIBUTE_PARAM_KEY =
			(CSSM_ATTRIBUTE_DATA_KEY | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 5)),

	/*
	 * Prompt string for CSSM_ALGID_SECURE_PASSPHRASE key acquisition.
	 * Data is a UTF8-encoded external representation of a CFString.
	 */
	CSSM_ATTRIBUTE_PROMPT =
			(CSSM_ATTRIBUTE_DATA_CSSM_DATA | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 6)),

	/*
	 * Alert panel title for CSSM_ALGID_SECURE_PASSPHRASE key acquisition.
	 * Data is a UTF8-encoded external representation of a CFString.
	 */
	CSSM_ATTRIBUTE_ALERT_TITLE =
			(CSSM_ATTRIBUTE_DATA_CSSM_DATA | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 7)),

	/*
	 * Boolean to specify whether secure passphrase is being used to encrypt or to
	 * recover data. In the former case the user will be prompted to enter the
	 * passphrase twice. Value is integer, nonzero (verify passphrase) or zero.
	 */
	CSSM_ATTRIBUTE_VERIFY_PASSPHRASE =
			(CSSM_ATTRIBUTE_DATA_UINT32 | (CSSM_ATTRIBUTE_VENDOR_DEFINED + 8))

};

/*
 * FEE key pair prime modulus types.
 */
enum {
	CSSM_FEE_PRIME_TYPE_DEFAULT = 0,	/* default per key size */
	CSSM_FEE_PRIME_TYPE_MERSENNE,		/* (2 ** q) - 1Ê*/
	CSSM_FEE_PRIME_TYPE_FEE,			/* (2 ** q) - k */
	CSSM_FEE_PRIME_TYPE_GENERAL			/* random prime */
};

/*
 * FEE curve types. Comments refer to equation
 *
 *    y**2 = x**3 + c(x**2) + ax + b
 */
enum {
	CSSM_FEE_CURVE_TYPE_DEFAULT = 0,	/* default per key size */
	CSSM_FEE_CURVE_TYPE_MONTGOMERY,		/* a==1, b==0 */
	CSSM_FEE_CURVE_TYPE_WEIERSTRASS,	/* c==0. IEEE P1363 compliant. */
	CSSM_FEE_CURVE_TYPE_ANSI_X9_62		/* ANSI X9.62 compatible */
};

/*
 * Apple Secure Compression (ComCryption) optimization attributes.
 */
enum {
	CSSM_ASC_OPTIMIZE_DEFAULT = 0,
	CSSM_ASC_OPTIMIZE_SIZE,				/* max compression (currently the default) */
	CSSM_ASC_OPTIMIZE_SECURITY,			/* currently not implemented */
	CSSM_ASC_OPTIMIZE_TIME,				/* min runtime */
	CSSM_ASC_OPTIMIZE_TIME_SIZE,		/* implies loss of security */
	CSSM_ASC_OPTIMIZE_ASCII,			/* optimized for ASCC text, not implemented */
};

/*
 * Apple custom CSSM_KEYATTR_FLAGS.
 */
enum {
	/*
	 * When set, indicates a public key which is incomplete (though
	 * still valid) due to the lack of algorithm-specific parameters.
	 */
	CSSM_KEYATTR_PARTIAL			= 0x00010000,

	/*
	 * When set, public keys are stored encrypted. Default is to store
	 * public keys in the clear. AppleCSPDL only.
	 */
	CSSM_KEYATTR_PUBLIC_KEY_ENCRYPT = 0x00020000
};

/*
 * Name/OID pair used in CSSM_APPLE_TP_CERT_REQUEST
 */
typedef struct {
	const char 			*string;
	const CSSM_OID 		*oid;
} CSSM_APPLE_TP_NAME_OID;

/*
 * Certificate request passed to CSSM_TP_SubmitCredRequest() in the
 * CSSM_TP_AUTHORITY_REQUEST_TYPE.Requests field. Used for requesting
 * both locally-generated certs (CSSMOID_APPLE_TP_LOCAL_CERT_GEN) and
 * cert signing requests (CSSMOID_APPLE_TP_CSR_GEN).
 */
typedef struct {
	CSSM_CSP_HANDLE			cspHand;		// sign with this CSP
	CSSM_CL_HANDLE			clHand;			// and this CL
	uint32					serialNumber;
	uint32 					numSubjectNames;// size subjectNames[]
	CSSM_APPLE_TP_NAME_OID	*subjectNames;

	/*
	 * Issuer name can be expressed in the simplified CSSM_APPLE_TP_NAME_OID
	 * array, as is the subject name, or as an CSSM_X509_NAME, which is
	 * typically obtained from a signing cert.
	 * Exactly one of {issuerNames, issuerNameX509} must be non-NULL.
	 */
	uint32 					numIssuerNames;	// size issuerNames[]
	CSSM_APPLE_TP_NAME_OID	*issuerNames;   // optional; NULL implies root
											//    (signer == subject)
	CSSM_X509_NAME_PTR		issuerNameX509;
	const CSSM_KEY  		*certPublicKey;
	const CSSM_KEY  		*issuerPrivateKey;

	/* Unfortunately there is no practical way to map any algorithm
	 * to its appropriate OID, and we need both.... */
	CSSM_ALGORITHMS 		signatureAlg;   // e.g., CSSM_ALGID_SHA1WithRSA
	CSSM_OID				signatureOid;	// e.g., CSSMOID_SHA1WithRSA
	uint32					notBefore;		// relative to "now"
	uint32					notAfter;
	uint32					numExtensions;
	CE_DataAndType  		*extensions;	// optional

	/*
	 * Optional challenge string for CSSMOID_APPLE_TP_CSR_GEN.
	 */
	const char				*challengeString;
} CSSM_APPLE_TP_CERT_REQUEST;

/*
 * Options for X509TP's CSSM_TP_CertGroupVerify for policy CSSMOID_APPLE_TP_SSL.
 * A pointer to, and length of, one of these is optionally placed in
 * CSSM_TP_VERIFY_CONTEXT.Cred->Policy.PolicyIds[n].FieldValue.
 */
#define CSSM_APPLE_TP_SSL_OPTS_VERSION		1

/*
 * Values for CSSM_APPLE_TP_SSL_OPTIONS.flags.
 *
 * Set this flag when evaluating a client cert.
 */
#define CSSM_APPLE_TP_SSL_CLIENT		0x00000001

typedef struct {
	uint32      Version;        // CSSM_APPLE_TP_SSL_OPTS_VERSION

	/*
	 * The domain name of the server (e.g., "store.apple.com".) In the
	 * SSL and TLS protocols, this must match the common name of the
	 * subject cert. Expressed as a C string, optionally NULL terminated
	 * if it is NULL terminated, the length field should include the NULL).
	 */
	uint32      ServerNameLen;
	const char  *ServerName;    // optional

	/* new fields for struct version 1 */
	uint32		Flags;
} CSSM_APPLE_TP_SSL_OPTIONS;

/*
 * Options for X509TP's CSSM_TP_CertGroupVerify for policy
 * CSSMOID_APPLE_TP_REVOCATION_CRL. A pointer to, and length of, one
 * of these is optionally placed in
 * CSSM_TP_VERIFY_CONTEXT.Cred->Policy.PolicyIds[n].FieldValue.
 */
#define CSSM_APPLE_TP_CRL_OPTS_VERSION		0

typedef uint32 CSSM_APPLE_TP_CRL_OPT_FLAGS;
enum {
	// require CRL verification for each cert; default is "try"
	CSSM_TP_ACTION_REQUIRE_CRL_PER_CERT 	= 0x00000001,
	// enable fetch from network
	CSSM_TP_ACTION_FETCH_CRL_FROM_NET 		= 0x00000002,
	// if set and positive CRL verify for given cert, no further revocation
	// checking need be done on that cert
	CSSM_TP_ACTION_CRL_SUFFICIENT			= 0x00000004,
	// require CRL verification for certs which claim a CRL provider
	CSSM_TP_ACTION_REQUIRE_CRL_IF_PRESENT 	= 0x00000008
};

typedef struct {
	uint32      				Version;        // CSSM_APPLE_TP_CRL_OPTS_VERSION
	CSSM_APPLE_TP_CRL_OPT_FLAGS	CrlFlags;

	/*
	 * When non-NULL, store CRLs fetched from net here.
	 * This is most likely a pointer to one of the
	 * CSSM_TP_CALLERAUTH_CONTEXT.DBList entries but that
	 * is not a strict requirement.
	 */
	CSSM_DL_DB_HANDLE_PTR		crlStore;
} CSSM_APPLE_TP_CRL_OPTIONS;

/*
 * Options for X509TP's CSSM_TP_CertGroupVerify for policy
 * CSSMOID_APPLE_TP_SMIME. A pointer to, and length of, one
 * of these is optionally placed in
 * CSSM_TP_VERIFY_CONTEXT.Cred->Policy.PolicyIds[n].FieldValue.
 */
#define CSSM_APPLE_TP_SMIME_OPTS_VERSION		0
typedef struct {
	uint32      Version;        // CSSM_APPLE_TP_SMIME_OPTS_VERSION

	/*
	 * Intended usage of the leaf cert. The cert's KeyUsage extension,
	 * if present, must be a superset of this.
	 */
	CE_KeyUsage	IntendedUsage;

	/*
	 * The email address of the sender. If there is an email address
	 * in the sender's cert, that email address must match this one.
	 * Both (email address in the cert, and this one) are optional.
	 * Expressed as a C string, optionally NULL terminated (i.e.,
	 * SenderEmail[SenderEmailLen - 1] may or may not be NULL).
	 */
	uint32      SenderEmailLen;
	const char  *SenderEmail;    // optional
} CSSM_APPLE_TP_SMIME_OPTIONS;


/*
 * Optional ActionData for all X509TP CertGroupVerify policies.
 * A pointer to, and length of, one of these is optionally placed in
 * CSSM_TP_VERIFY_CONTEXT.ActionData.
 */
typedef uint32 CSSM_APPLE_TP_ACTION_FLAGS;
enum {
	CSSM_TP_ACTION_ALLOW_EXPIRED  		= 0x00000001,	// allow expired certs
	CSSM_TP_ACTION_LEAF_IS_CA 	 		= 0x00000002,	// first cert is a CA
	CSSM_TP_ACTION_FETCH_CERT_FROM_NET 	= 0x00000004,	// enable net fetch of CA cert
	CSSM_TP_ACTION_ALLOW_EXPIRED_ROOT 	= 0x00000008, 	// allow expired roots
	CSSM_TP_ACTION_REQUIRE_REV_PER_CERT	= 0x00000010, 	// require positive revocation
														//   check per cert
	CSSM_TP_ACTION_TRUST_SETTINGS		= 0x00000020,	// use TrustSettings instead of
														//   anchors
	CSSM_TP_ACTION_IMPLICIT_ANCHORS		= 0x00000040	// properly self-signed certs are
														//   treated as anchors implicitly
};

#define CSSM_APPLE_TP_ACTION_VERSION		0
typedef struct {
	uint32						Version; 		// CSSM_APPLE_TP_ACTION_VERSION
	CSSM_APPLE_TP_ACTION_FLAGS	ActionFlags;	// CSSM_TP_ACTION_ALLOW_EXPIRED, etc.
} CSSM_APPLE_TP_ACTION_DATA;

/*
 * Per-cert evidence returned from CSSM_TP_CertGroupVerify.
 * An array of these is presented in CSSM_TP_VERIFY_CONTEXT_RESULT.Evidence[2].
 * Same number of these as in the cert group in Evidence[1].
 */

#endif /* SEC_OS_OSX */

/* First, an array of bits indicating various status of the cert. */
typedef uint32 CSSM_TP_APPLE_CERT_STATUS;
enum
{
	CSSM_CERT_STATUS_EXPIRED			= 0x00000001,
	CSSM_CERT_STATUS_NOT_VALID_YET		= 0x00000002,
	CSSM_CERT_STATUS_IS_IN_INPUT_CERTS	= 0x00000004,
	CSSM_CERT_STATUS_IS_IN_ANCHORS		= 0x00000008,
	CSSM_CERT_STATUS_IS_ROOT			= 0x00000010,
	CSSM_CERT_STATUS_IS_FROM_NET		= 0x00000020,
	/* settings found in per-user Trust Settings */
	CSSM_CERT_STATUS_TRUST_SETTINGS_FOUND_USER		= 0x00000040,
	/* settings found in Admin Trust Settings */
	CSSM_CERT_STATUS_TRUST_SETTINGS_FOUND_ADMIN		= 0x00000080,
	/* settings found in System Trust Settings */
	CSSM_CERT_STATUS_TRUST_SETTINGS_FOUND_SYSTEM	= 0x00000100,
	/* Trust Settings result = Trust */
	CSSM_CERT_STATUS_TRUST_SETTINGS_TRUST			= 0x00000200,
	/* Trust Settings result = Deny */
	CSSM_CERT_STATUS_TRUST_SETTINGS_DENY			= 0x00000400,
	/* Per-cert error ignored due to Trust Settings */
	CSSM_CERT_STATUS_TRUST_SETTINGS_IGNORED_ERROR	= 0x00000800
};

typedef struct DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER {
	CSSM_TP_APPLE_CERT_STATUS	StatusBits;
	uint32 						NumStatusCodes;
	CSSM_RETURN 				*StatusCodes;

	/* index into raw cert group or AnchorCerts depending on IS_IN_ANCHORS */
	uint32 						Index;

	/* nonzero if cert came from a DLDB */
	CSSM_DL_DB_HANDLE			DlDbHandle;
	CSSM_DB_UNIQUE_RECORD_PTR	UniqueRecord;

#if SEC_OS_IPHONE
    /* CRLReason code if cert is revoked */
    sint32                      CrlReason;
#endif /* SEC_OS_IPHONE */
} CSSM_TP_APPLE_EVIDENCE_INFO DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;

#if SEC_OS_OSX

/*
 * CSSM_TP_VERIFY_CONTEXT_RESULT.Evidence[0], basically defines which version/flavor
 * of remaining evidence is.
 */
#define CSSM_TP_APPLE_EVIDENCE_VERSION		0
typedef struct
{
	uint32		Version;
} CSSM_TP_APPLE_EVIDENCE_HEADER;


/*
 * Apple-specific CSSM_EVIDENCE_FORM values
 *
 * The form of the evidence returns from CSSM_TP_CertGroupVerify is:
 *
 * EvidenceForm							contents of *Evidence
 * ------------  						---------------------
 * CSSM_EVIDENCE_FORM_APPLE_HEADER		CSSM_TP_APPLE_EVIDENCE_HEADER
 * CSSM_EVIDENCE_FORM_APPLE_CERTGROUP	CSSM_CERTGROUP
 * CSSM_EVIDENCE_FORM_APPLE_CERT_INFO	array of CSSM_TP_APPLE_EVIDENCE_INFO, size
 *											CSSM_CERTGROUP.NumCerts
 */

#define CSSM_EVIDENCE_FORM_APPLE_CUSTOM		0x80000000
enum
{
	CSSM_EVIDENCE_FORM_APPLE_HEADER 	= CSSM_EVIDENCE_FORM_APPLE_CUSTOM + 0,
	CSSM_EVIDENCE_FORM_APPLE_CERTGROUP 	= CSSM_EVIDENCE_FORM_APPLE_CUSTOM + 1,
	CSSM_EVIDENCE_FORM_APPLE_CERT_INFO  = CSSM_EVIDENCE_FORM_APPLE_CUSTOM + 2
};

/* AppleX509CL extensions: passthrough ids */
enum {
	/*
	 * Obtain a signed Certificate Signing Request.
	 * Input = CSSM_APPLE_CL_CSR_REQUEST
	 * Output = allocated CSSM_DATA which points to a DER-encoded CSR.
	 */
	CSSM_APPLEX509CL_OBTAIN_CSR,

	/*
	 * Perform signature verify of a CSR.
	 * Input:  CSSM_DATA referring to a DER-encoded CSR.
	 * Output: Nothing, returns CSSMERR_CL_VERIFICATION_FAILURE on
	 *         on failure.
	 */
	CSSM_APPLEX509CL_VERIFY_CSR
};

/*
 * Used in CL's CSSM_APPLEX509_OBTAIN_CSR Passthrough. This is the
 * input; the output is a CSSM_DATA * containing the signed and
 * DER-encoded CSR.
 */
typedef struct {
	CSSM_X509_NAME_PTR		subjectNameX509;

	/* Unfortunately there is no practical way to map any algorithm
	 * to its appropriate OID, and we need both.... */
	CSSM_ALGORITHMS 		signatureAlg;   // e.g., CSSM_ALGID_SHA1WithRSA
	CSSM_OID				signatureOid;	// e.g., CSSMOID_SHA1WithRSA

	CSSM_CSP_HANDLE			cspHand;		// sign with this CSP
	const CSSM_KEY  		*subjectPublicKey;
	const CSSM_KEY  		*subjectPrivateKey;

	/*
	 * Optional challenge string.
	 */
	const char				*challengeString;
} CSSM_APPLE_CL_CSR_REQUEST;

/*
 * When a CRL with no NextUpdate field is encountered, we use this time
 * as the NextUpdate attribute when storing in a DB. It represents the
 * virtual end of time in CSSM_TIMESTRING form.
 */
#define CSSM_APPLE_CRL_END_OF_TIME		"99991231235959"

/*
 * Default filesystem names and locations for SecurityServer features
 * (included here for lack of a better place)
 */
#define kKeychainSuffix			".keychain"
#define kKeychainDbSuffix       ".keychain-db"
#define kSystemKeychainName		"System.keychain"
#define kSystemKeychainDir		"/Library/Keychains/"
#define kSystemUnlockFile		"/var/db/SystemKey"

#define kSystemKeychainPath    kSystemKeychainDir kSystemKeychainName

/*
 * CSSM ACL tags used to store partition/integrity data in ACLs
 */
#define CSSM_APPLE_ACL_TAG_PARTITION_ID		"___PARTITION___"
#define CSSM_APPLE_ACL_TAG_INTEGRITY		"___INTEGRITY___"


void cssmPerror(const char *how, CSSM_RETURN error);

/* Convert between CSSM_OID and CSSM_ALGORITHMS */
bool cssmOidToAlg(const CSSM_OID *oid, CSSM_ALGORITHMS *alg);
const CSSM_OID *cssmAlgToOid(CSSM_ALGORITHMS algId);

/*
 * The MacOS OSStatus space has an embedding for UNIX errno values, similar to
 * the way we embed CSSM_RETURN values in OSStatus. These are the base and limit
 * values for this embedding.
 */
#define errSecErrnoBase			100000
#define errSecErrnoLimit		100255

#endif /* SEC_OS_OSX */

#pragma clang diagnostic pop

#ifdef	__cplusplus
}
#endif	// __cplusplus

#endif /* _CSSMAPPLE_H_ */
