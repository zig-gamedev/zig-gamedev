/*
 *  CTDefines.h
 *  CoreText
 *
 *  Copyright (c) 2010-2020 Apple Inc. All rights reserved.
 *
 */

#ifndef __CTDEFINES__
#define __CTDEFINES__

#include <TargetConditionals.h>
#include <Availability.h>

#ifndef __has_feature
# define __has_feature(x) 0
#endif

#ifndef __has_attribute
# define __has_attribute(x) 0
#endif

#if defined(CT_BUILDING_CoreText) || TARGET_OS_WIN32
# define CT_AVAILABLE(...)
# define CT_UNAVAILABLE(...)
# define CT_DEPRECATED(...)
# define CT_DEPRECATED_WITH_REPLACEMENT(...)
#else /* defined(CT_BUILDING_CoreText) || TARGET_OS_WIN32 */
# define CT_AVAILABLE(...) API_AVAILABLE(__VA_ARGS__)
# define CT_UNAVAILABLE(...) API_UNAVAILABLE(__VA_ARGS__)
# define CT_DEPRECATED(...) API_DEPRECATED(__VA_ARGS__)
# define CT_DEPRECATED_WITH_REPLACEMENT(...) API_DEPRECATED_WITH_REPLACEMENT(__VA_ARGS__)
#endif /* defined(CT_BUILDING_CoreText) || TARGET_OS_WIN32 */

#if __has_feature(enumerator_attributes) && __has_attribute(availability)
# define CT_ENUM_DEPRECATED(...) CT_DEPRECATED(__VA_ARGS__)
# define CT_ENUM_AVAILABLE(...) CT_AVAILABLE(__VA_ARGS__)
# define CT_ENUM_UNAVAILABLE(...) CT_UNAVAILABLE(__VA_ARGS__)
#else
# define CT_ENUM_DEPRECATED(...)
# define CT_ENUM_AVAILABLE(...)
# define CT_ENUM_UNAVAILABLE(...)
#endif /* __has_feature(enumerator_attributes) && __has_attribute(availability) */

#if __has_attribute(objc_bridge)
# if defined(__OBJC__)
#  if TARGET_OS_IPHONE
@class UIFont;
@class UIFontDescriptor;
#  else
@class NSFont;
@class NSFontCollection;
@class NSFontDescriptor;
@class NSGlyphInfo;
@class NSMutableFontCollection;
#  endif /* TARGET_OS_IPHONE */
@class NSParagraphStyle;
@class NSTextTab;
# endif /* defined(__OBJC__) */
#endif /*  __has_attribute(objc_bridge) */

#if TARGET_OS_WIN32
#define _Nullable
#define _Nonnull

#define CF_BRIDGED_TYPE(T)
#define CF_BRIDGED_MUTABLE_TYPE(T)
#define CF_RELATED_TYPE(T,C,I)

#define CF_ASSUME_NONNULL_BEGIN
#define CF_ASSUME_NONNULL_END

# if defined(CT_BUILDING_CoreText) && defined(__cplusplus)
#  define CT_EXPORT extern "C" __declspec(dllexport)
# elif defined(CT_BUILDING_CoreText) && !defined(__cplusplus)
#  define CT_EXPORT extern __declspec(dllexport)
# elif defined(__cplusplus)
#  define CT_EXPORT extern "C" __declspec(dllimport)
# else
#  define CT_EXPORT extern __declspec(dllimport)
# endif
#else
# define CT_EXPORT extern
#endif

#endif
