/*
 *  CoreText.h
 *  CoreText
 *
 *  Copyright (c) 2006-2020 Apple Inc. All rights reserved.
 *
 */

/*!
    @header

    Thread Safety Information

    All functions in this header are thread safe unless otherwise specified.
*/

#ifndef __CORETEXT__
#define __CORETEXT__

#include <CoreText/CTFont.h>
#include <CoreText/CTFontCollection.h>
#include <CoreText/CTFontDescriptor.h>
#include <CoreText/CTFontManager.h>
#include <CoreText/CTFontTraits.h>
#include <CoreText/CTFrame.h>
#include <CoreText/CTFramesetter.h>
#include <CoreText/CTGlyphInfo.h>
#include <CoreText/CTLine.h>
#include <CoreText/CTParagraphStyle.h>
#include <CoreText/CTRubyAnnotation.h>
#include <CoreText/CTRun.h>
#include <CoreText/CTRunDelegate.h>
#include <CoreText/CTStringAttributes.h>
#include <CoreText/CTTextTab.h>
#include <CoreText/CTTypesetter.h>
#include <CoreText/SFNTLayoutTypes.h>
#include <CoreText/SFNTTypes.h>

CF_EXTERN_C_BEGIN

/*!
    @function   CTGetCoreTextVersion
    @abstract   Returns the version of the CoreText framework.

    @discussion This function returns a number indicating the version of the
                CoreText framework. Note that framework version is not always
                an accurate indicator of feature availability. The recommended
                way to use this function is first to check that the function
                pointer is non-NULL, followed by calling it and comparing its
                result to a defined constant (or constants). For example, to
                determine whether the CoreText API is available:
                    if (&CTGetCoreTextVersion != NULL && CTGetCoreTextVersion() >= kCTVersionNumber10_5) {
                        // CoreText API is available
                    }

    @result     The version number. This value is for comparison with the
                constants beginning with kCTVersionNumber and will not exceed
                kCTVersionNumber11_0.
*/

uint32_t CTGetCoreTextVersion( void ) CT_DEPRECATED("Use -[NSProcessInfo operatingSystemVersion]", macos(10.5, 11.0), ios(3.2, 14.0), watchos(2.0, 7.0), tvos(9.0, 14.0));

#define kCTVersionNumber10_5 0x00020000
#define kCTVersionNumber10_5_2 0x00020001
#define kCTVersionNumber10_5_3 0x00020002
#define kCTVersionNumber10_5_5 0x00020003
#define kCTVersionNumber10_6 0x00030000
#define kCTVersionNumber10_7 0x00040000
#define kCTVersionNumber10_8 0x00050000
#define kCTVersionNumber10_9 0x00060000
#define kCTVersionNumber10_10 0x00070000
#define kCTVersionNumber10_11 0x00080000
#define kCTVersionNumber10_12 0x00090000
#define kCTVersionNumber10_13 0x000A0000
#define kCTVersionNumber10_14 0x000B0000
#define kCTVersionNumber10_15 0x000C0000
#define kCTVersionNumber11_0 0x000D0000

CF_EXTERN_C_END

#endif // __CORETEXT__
