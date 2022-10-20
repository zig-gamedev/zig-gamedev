/* CoreGraphics - CGPDFOperatorTable.h
 * Copyright (c) 2004-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGPDFOPERATORTABLE_H_
#define CGPDFOPERATORTABLE_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CGPDFOperatorTable *CGPDFOperatorTableRef;

#include <CoreGraphics/CGPDFScanner.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

typedef void (*CGPDFOperatorCallback)(CGPDFScannerRef  scanner,
                                      void * __nullable info);

/* Return an empty operator table. */

CG_EXTERN CGPDFOperatorTableRef __nullable CGPDFOperatorTableCreate(void)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Increment the retain count of `table'. */

CG_EXTERN CGPDFOperatorTableRef cg_nullable CGPDFOperatorTableRetain(
    CGPDFOperatorTableRef cg_nullable table)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Decrement the retain count of `table'. */

CG_EXTERN void CGPDFOperatorTableRelease(
    CGPDFOperatorTableRef cg_nullable table)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Set the callback for the operator named `name' to `callback' */

CG_EXTERN void CGPDFOperatorTableSetCallback(
    CGPDFOperatorTableRef cg_nullable table,
    const char * cg_nullable name, CGPDFOperatorCallback cg_nullable callback)
    CG_AVAILABLE_STARTING(10.4, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif  /* CGPDFOPERATORTABLE_H_ */
