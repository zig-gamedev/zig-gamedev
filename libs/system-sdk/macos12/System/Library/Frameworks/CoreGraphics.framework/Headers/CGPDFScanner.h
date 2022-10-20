/* CoreGraphics - CGPDFScanner.h
   Copyright (c) 2004-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPDFSCANNER_H_
#define CGPDFSCANNER_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CGPDFScanner *CGPDFScannerRef;

#include <CoreGraphics/CGPDFContentStream.h>
#include <CoreGraphics/CGPDFOperatorTable.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Create a scanner. */

CG_EXTERN CGPDFScannerRef  CGPDFScannerCreate(
    CGPDFContentStreamRef  cs,
    CGPDFOperatorTableRef __nullable table, void * __nullable info)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Retain `scanner'. */

CG_EXTERN CGPDFScannerRef cg_nullable CGPDFScannerRetain(
    CGPDFScannerRef cg_nullable scanner)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Release `scanner'. */

CG_EXTERN void CGPDFScannerRelease(CGPDFScannerRef cg_nullable scanner)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Scan the content stream of `scanner'. Returns true if the entire stream
   was scanned successfully; false if scanning failed for some reason (for
   example, if the stream's data is corrupted). */

CG_EXTERN bool CGPDFScannerScan(CGPDFScannerRef cg_nullable scanner)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the content stream associated with `scanner'. */

CG_EXTERN CGPDFContentStreamRef  CGPDFScannerGetContentStream(
    CGPDFScannerRef  scanner)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and return it in `value'. */

CG_EXTERN bool CGPDFScannerPopObject(CGPDFScannerRef  scanner,
    CGPDFObjectRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's a boolean, return
   it in `value'. Return false if the top of the stack isn't a boolean. */

CG_EXTERN bool CGPDFScannerPopBoolean(CGPDFScannerRef  scanner,
    CGPDFBoolean * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's an integer, return
   it in `value'. Return false if the top of the stack isn't an integer. */

CG_EXTERN bool CGPDFScannerPopInteger(CGPDFScannerRef  scanner,
    CGPDFInteger * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's a number, return
   it in `value'. Return false if the top of the stack isn't a number. */

CG_EXTERN bool CGPDFScannerPopNumber(CGPDFScannerRef  scanner,
    CGPDFReal * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's a name, return it
   in `value'. Return false if the top of the stack isn't a name. */

CG_EXTERN bool CGPDFScannerPopName(CGPDFScannerRef  scanner,
    const char * __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's a string, return
   it in `value'. Return false if the top of the stack isn't a string. */

CG_EXTERN bool CGPDFScannerPopString(CGPDFScannerRef  scanner,
    CGPDFStringRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's an array, return
   it in `value'. Return false if the top of the stack isn't an array. */

CG_EXTERN bool CGPDFScannerPopArray(CGPDFScannerRef  scanner,
    CGPDFArrayRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's a dictionary,
   return it in `value'. Return false if the top of the stack isn't a
   dictionary. */

CG_EXTERN bool CGPDFScannerPopDictionary(CGPDFScannerRef  scanner,
    CGPDFDictionaryRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Pop an object from the stack of `scanner' and, if it's a stream, return
   it in `value'. Return false if the top of the stack isn't a stream. */

CG_EXTERN bool CGPDFScannerPopStream(CGPDFScannerRef  scanner,
    CGPDFStreamRef __nullable * __nullable value)
    CG_AVAILABLE_STARTING(10.4, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFSCANNER_H_ */
