//
//  ATSDefines.h
//  ATS
//
//  Copyright (c) 2011-2019 Apple Inc. All rights reserved.
//

#ifndef __ATSDEFINES__
#define __ATSDEFINES__

#if defined(BUILDING_ATS)
# define ATS_AVAILABLE_BUT_DEPRECATED_WITH_ADVICE(_mac,_macDep,_deprecationAdvice)
# define ATS_AVAILABLE_BUT_DEPRECATED(_mac,_macDep)
# define ATS_DEPRECATED_IN_MAC_OS_X_VERSION(_macDep) // For types
#elif defined(__has_extension) /* defined(BUILDING_ATS) */
# if __has_extension(attribute_deprecated_with_message)
#  define ATS_AVAILABLE_BUT_DEPRECATED_WITH_ADVICE(_mac,_macDep,_deprecationAdvice) __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_##_mac,__MAC_##_macDep, __IPHONE_NA, __IPHONE_NA, _deprecationAdvice)
# endif /*__has_extension(attribute_deprecated_with_message) */
#endif /* defined(__has_extension)*/ /* defined(BUILDING_ATS) */

#if !defined(ATS_AVAILABLE_BUT_DEPRECATED_WITH_ADVICE)
# include <Availability.h>
# if defined(__MAC_10_8)
#  define ATS_AVAILABLE_BUT_DEPRECATED_WITH_ADVICE(_mac,_macDep,_deprecationAdvice) AVAILABLE_MAC_OS_X_VERSION_##_mac##_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_##_macDep
# else
#  define ATS_AVAILABLE_BUT_DEPRECATED_WITH_ADVICE(_mac,_macDep,_deprecationAdvice) AVAILABLE_MAC_OS_X_VERSION_##_mac##_AND_LATER
# endif
#endif

#if !defined(ATS_AVAILABLE_BUT_DEPRECATED)
# include <Availability.h>
# if defined(__MAC_10_8)
#  define ATS_AVAILABLE_BUT_DEPRECATED(_mac,_macDep) AVAILABLE_MAC_OS_X_VERSION_##_mac##_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_##_macDep
# else
#  define ATS_AVAILABLE_BUT_DEPRECATED(_mac,_macDep) AVAILABLE_MAC_OS_X_VERSION_##_mac##_AND_LATER
# endif
#endif

#if !defined(ATS_DEPRECATED_IN_MAC_OS_X_VERSION)
# define ATS_DEPRECATED_IN_MAC_OS_X_VERSION(_macDep) DEPRECATED_IN_MAC_OS_X_VERSION_##_macDep##_AND_LATER
#endif

#if (TARGET_OS_MAC && !(TARGET_OS_EMBEDDED || TARGET_OS_IPHONE)) || (TARGET_OS_EMBEDDED || TARGET_OS_IPHONE)
# define ATS_LEGACY_API 1
#else
# define ATS_LEGACY_API 0
#endif // (TARGET_OS_MAC && !(TARGET_OS_EMBEDDED || TARGET_OS_IPHONE)) || (TARGET_OS_EMBEDDED || TARGET_OS_IPHONE)

#endif //__ATSDEFINES__
