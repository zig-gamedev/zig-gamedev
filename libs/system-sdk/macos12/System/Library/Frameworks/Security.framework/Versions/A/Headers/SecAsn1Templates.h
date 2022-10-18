/*
 * Copyright (c) 2003-2006,2008,2010-2012 Apple Inc. All Rights Reserved.
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
 * SecAsn1Templates.h - Common ASN1 primitive templates for use with SecAsn1Coder.
 */

#ifndef	_SEC_ASN1_TEMPLATES_H_
#define _SEC_ASN1_TEMPLATES_H_

#include <Security/SecAsn1Types.h>

#ifdef  __cplusplus
extern "C" {
#endif

CF_ASSUME_NONNULL_BEGIN

/************************************************************************/

/*
 * Generic Templates
 * One for each of the simple types, plus a special one for ANY, plus:
 *	- a pointer to each one of those
 *	- a set of each one of those
 *	- a sequence of each one of those
 */
extern const SecAsn1Template kSecAsn1AnyTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1BitStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1BMPStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1BooleanTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1EnumeratedTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1GeneralizedTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1IA5StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1IntegerTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1UnsignedIntegerTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1NullTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1ObjectIDTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1OctetStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PrintableStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1T61StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1UniversalStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1UTCTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1UTF8StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1VisibleStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1TeletexStringTemplate[] SEC_ASN1_API_DEPRECATED;

extern const SecAsn1Template kSecAsn1PointerToAnyTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToBitStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToBMPStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToBooleanTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToEnumeratedTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToGeneralizedTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToIA5StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToIntegerTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToNullTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToObjectIDTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToOctetStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToPrintableStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToT61StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToUniversalStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToUTCTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToUTF8StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToVisibleStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1PointerToTeletexStringTemplate[] SEC_ASN1_API_DEPRECATED;

extern const SecAsn1Template kSecAsn1SequenceOfAnyTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfBitStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfBMPStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfBooleanTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfEnumeratedTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfGeneralizedTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfIA5StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfIntegerTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfNullTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfObjectIDTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfOctetStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfPrintableStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfT61StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfUniversalStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfUTCTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfUTF8StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfVisibleStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SequenceOfTeletexStringTemplate[] SEC_ASN1_API_DEPRECATED;

extern const SecAsn1Template kSecAsn1SetOfAnyTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfBitStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfBMPStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfBooleanTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfEnumeratedTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfGeneralizedTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfIA5StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfIntegerTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfNullTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfObjectIDTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfOctetStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfPrintableStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfT61StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfUniversalStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfUTCTimeTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfUTF8StringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfVisibleStringTemplate[] SEC_ASN1_API_DEPRECATED;
extern const SecAsn1Template kSecAsn1SetOfTeletexStringTemplate[] SEC_ASN1_API_DEPRECATED;

/*
 * Template for skipping a subitem; only used when decoding.
 */
extern const SecAsn1Template kSecAsn1SkipTemplate[] SEC_ASN1_API_DEPRECATED;

CF_ASSUME_NONNULL_END

#ifdef  __cplusplus
}
#endif

#endif	/* _SEC_ASN1_TEMPLATES_H_ */
