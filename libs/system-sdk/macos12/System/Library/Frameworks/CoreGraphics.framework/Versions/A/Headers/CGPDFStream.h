/* CoreGraphics - CGPDFStream.h
 * Copyright (c) 2002-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGPDFSTREAM_H_
#define CGPDFSTREAM_H_

typedef struct CGPDFStream *CGPDFStreamRef;

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef CF_ENUM (int32_t, CGPDFDataFormat) {
    CGPDFDataFormatRaw, CGPDFDataFormatJPEGEncoded, CGPDFDataFormatJPEG2000
};

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGPDFDictionary.h>
#include <CoreGraphics/CGPDFStream.h>
#include <CoreFoundation/CFData.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Return the dictionary of `stream'. */

CG_EXTERN CGPDFDictionaryRef __nullable CGPDFStreamGetDictionary(
    CGPDFStreamRef cg_nullable stream)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Return the data of `stream'. */

CG_EXTERN CFDataRef __nullable CGPDFStreamCopyData(
    CGPDFStreamRef cg_nullable stream,
    CGPDFDataFormat * cg_nullable format)
    CG_AVAILABLE_STARTING(10.3, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFSTREAM_H_ */
