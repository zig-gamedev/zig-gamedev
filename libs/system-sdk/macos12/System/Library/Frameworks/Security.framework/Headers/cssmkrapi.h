/*
 * Copyright (c) 1999-2001,2004,2011,2014 Apple Inc. All Rights Reserved.
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
 * cssmkrapi.h -- Application Programmers Interface for Key Recovery Modules
 */

#ifndef _CSSMKRAPI_H_
#define _CSSMKRAPI_H_  1

#include <Security/cssmtype.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

typedef uint32 CSSM_KRSP_HANDLE; /* Key Recovery Service Provider Handle */

typedef struct cssm_kr_name {
    uint8 Type; /* namespace type */
    uint8 Length; /* name string length */
    char *Name; /* name string */
} CSSM_KR_NAME DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;

typedef struct DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER cssm_kr_profile {
    CSSM_KR_NAME UserName; /* name of the user */
    CSSM_CERTGROUP_PTR UserCertificate; /* public key certificate of the user */
    CSSM_CERTGROUP_PTR KRSCertChain; /* cert chain for the KRSP coordinator */
    uint8 LE_KRANum; /* number of KRA cert chains in the following list */
    CSSM_CERTGROUP_PTR LE_KRACertChainList; /* list of Law enforcement KRA certificate chains */
    uint8 ENT_KRANum; /* number of KRA cert chains in the following list */
    CSSM_CERTGROUP_PTR ENT_KRACertChainList; /* list of Enterprise KRA certificate chains */
    uint8 INDIV_KRANum; /* number of KRA cert chains in the following list */
    CSSM_CERTGROUP_PTR INDIV_KRACertChainList; /* list of Individual KRA certificate chains */
    CSSM_DATA_PTR INDIV_AuthenticationInfo; /* authentication information for individual key recovery */
    uint32 KRSPFlags; /* flag values to be interpreted by KRSP */
    CSSM_DATA_PTR KRSPExtensions; /* reserved for extensions specific to KRSPs */
} CSSM_KR_PROFILE DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER, *CSSM_KR_PROFILE_PTR DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;

typedef struct DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER cssm_kr_wrappedproductinfo {
    CSSM_VERSION StandardVersion;
    CSSM_STRING StandardDescription;
    CSSM_VERSION ProductVersion;
    CSSM_STRING ProductDescription;
    CSSM_STRING ProductVendor;
    uint32 ProductFlags;
} CSSM_KR_WRAPPEDPRODUCT_INFO DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER, *CSSM_KR_WRAPPEDPRODUCT_INFO_PTR DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;

typedef struct DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER cssm_krsubservice {
    uint32 SubServiceId;
    char *Description; /* Description of this sub service */
    CSSM_KR_WRAPPEDPRODUCT_INFO WrappedProduct;
} CSSM_KRSUBSERVICE DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER, *CSSM_KRSUBSERVICE_PTR DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;

typedef uint32 CSSM_KR_POLICY_TYPE;
#define CSSM_KR_INDIV_POLICY			(0x00000001)
#define CSSM_KR_ENT_POLICY				(0x00000002)
#define CSSM_KR_LE_MAN_POLICY			(0x00000003)
#define CSSM_KR_LE_USE_POLICY			(0x00000004)

typedef uint32 CSSM_KR_POLICY_FLAGS;

#define CSSM_KR_INDIV					(0x00000001)
#define CSSM_KR_ENT						(0x00000002)
#define CSSM_KR_LE_MAN					(0x00000004)
#define CSSM_KR_LE_USE					(0x00000008)
#define CSSM_KR_LE						(CSSM_KR_LE_MAN | CSSM_KR_LE_USE)
#define CSSM_KR_OPTIMIZE				(0x00000010)
#define CSSM_KR_DROP_WORKFACTOR			(0x00000020)

typedef struct DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER cssm_kr_policy_list_item {
    struct kr_policy_list_item *next;
    CSSM_ALGORITHMS AlgorithmId;
    CSSM_ENCRYPT_MODE Mode;
    uint32 MaxKeyLength;
    uint32 MaxRounds;
    uint8 WorkFactor;
    CSSM_KR_POLICY_FLAGS PolicyFlags;
    CSSM_CONTEXT_TYPE AlgClass;
} CSSM_KR_POLICY_LIST_ITEM DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER, *CSSM_KR_POLICY_LIST_ITEM_PTR DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;

typedef struct DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER cssm_kr_policy_info {
    CSSM_BOOL krbNotAllowed;
    uint32 numberOfEntries;
    CSSM_KR_POLICY_LIST_ITEM *policyEntry;
} CSSM_KR_POLICY_INFO DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER, *CSSM_KR_POLICY_INFO_PTR DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;


#pragma clang diagnostic pop

#ifdef __cplusplus
}
#endif

#endif /* _CSSMKRAPI_H_ */
