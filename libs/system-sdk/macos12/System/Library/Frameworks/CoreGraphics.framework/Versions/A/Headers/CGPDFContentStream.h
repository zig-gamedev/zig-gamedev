/* CoreGraphics - CGPDFContentStream.h
   Copyright (c) 2004-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPDFCONTENTSTREAM_H_
#define CGPDFCONTENTSTREAM_H_

typedef struct CGPDFContentStream *CGPDFContentStreamRef;

#include <CoreGraphics/CGPDFPage.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Create a content stream from `page'. */

CG_EXTERN CGPDFContentStreamRef CGPDFContentStreamCreateWithPage(
  CGPDFPageRef page) CG_AVAILABLE_STARTING(10.4, 2.0);

/* Create a content stream from `stream'. */

CG_EXTERN CGPDFContentStreamRef CGPDFContentStreamCreateWithStream(
  CGPDFStreamRef stream, CGPDFDictionaryRef streamResources,
  CGPDFContentStreamRef cg_nullable parent)
  CG_AVAILABLE_STARTING(10.4, 2.0);

/* Increment the retain count of `cs'. */

CG_EXTERN CGPDFContentStreamRef CGPDFContentStreamRetain(
  CGPDFContentStreamRef cs) CG_AVAILABLE_STARTING(10.4, 2.0);

/* Decrement the retain count of `cs'. */

CG_EXTERN void CGPDFContentStreamRelease(CGPDFContentStreamRef cs)
  CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the array of CGPDFStreamRefs comprising the entire content stream
   of `cs'. */

CG_EXTERN CFArrayRef __nullable CGPDFContentStreamGetStreams(CGPDFContentStreamRef cs)
  CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the resource named `name' in category `category' of the resource
   dictionaries of `cs'. */

CG_EXTERN CGPDFObjectRef __nullable CGPDFContentStreamGetResource(
  CGPDFContentStreamRef cs, const char *category, const char *name)
  CG_AVAILABLE_STARTING(10.4, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPDFCONTENTSTREAM_H_ */
