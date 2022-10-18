/* CoreGraphics - CGPDFString.h
 * Copyright (c) 2002-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGPDFSTRING_H_
#define CGPDFSTRING_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CGPDFString *CGPDFStringRef;

#include <CoreGraphics/CGBase.h>
#include <CoreFoundation/CFDate.h>
#include <CoreFoundation/CFString.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Return the length of `string'. */

CG_EXTERN size_t CGPDFStringGetLength(CGPDFStringRef cg_nullable string)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return a pointer to the bytes of `string'. */

CG_EXTERN const unsigned char * __nullable CGPDFStringGetBytePtr(
    CGPDFStringRef cg_nullable string)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return a CFString representing `string' as a "text string". See Section
   3.8.1 "Text Strings", PDF Reference: Adobe PDF version 1.6 (5th ed.) for
   more information. */

CG_EXTERN CFStringRef __nullable CGPDFStringCopyTextString(
    CGPDFStringRef cg_nullable string)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Convert `string' to a CFDate. See Section 3.8.3 "Dates", PDF Reference:
   Adobe PDF version 1.6 (5th ed.) for more information. */

CG_EXTERN CFDateRef __nullable CGPDFStringCopyDate(
    CGPDFStringRef cg_nullable string)
    CG_AVAILABLE_STARTING(10.4, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFSTRING_H_ */
