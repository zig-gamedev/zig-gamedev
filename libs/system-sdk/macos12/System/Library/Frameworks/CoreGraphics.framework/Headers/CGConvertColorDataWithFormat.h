/* CoreGraphics - CGConvertColorDataWithFormat.h
   Copyright (c)2021 Apple Inc.
   All rights reserved. */

#ifndef CGCONVERTCOLORDATAWITHFORMAT_H
#define CGCONVERTCOLORDATAWITHFORMAT_H

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

typedef struct {
    uint32_t        		version;            // current version = 0
    CFTypeRef       		colorspace_info;    // one of:
                                                // - CGColorSpaceRef,
                                                // - CFStringRef (known name used by CGColorSpaceCreateWithName),
                                                // - CFPropertyListRef (flattened CGColorSpaceRef used by IOSurface)
    CGBitmapInfo    		bitmap_info;		// see CGImage.h
    size_t          		bits_per_component;
    size_t          		bytes_per_row;      // can be 0, if data is pixel aligned
    CGColorRenderingIntent	intent;				// set to kCGRenderingIntentDefault if unknown
    CGFloat*        		decode;             // pass NULL; reserved for future use
} CGColorDataFormat;


CG_EXTERN bool CGConvertColorDataWithFormat(size_t            width,
                                            size_t            height,
                                            void*             dst_data,
                                            CGColorDataFormat dst_format,
                                            void*             src_data,
                                            CGColorDataFormat src_format,
                                            CFDictionaryRef   options);

#endif /* CGCONVERTCOLORDATAWITHFORMAT_H */
