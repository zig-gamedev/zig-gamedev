/*
	AppKitDefines.h
	Application Kit
	Copyright (c) 1995-2021, Apple Inc.
	All rights reserved.
*/
#ifndef _APPKITDEFINES_H
#define _APPKITDEFINES_H

#import <AvailabilityMacros.h>
#import <Foundation/NSObjCRuntime.h>

#ifndef AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_0
#define AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_0 DEPRECATED_IN_MAC_OS_X_VERSION_10_0_AND_LATER
#define AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_1 DEPRECATED_IN_MAC_OS_X_VERSION_10_1_AND_LATER
#define AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_2 DEPRECATED_IN_MAC_OS_X_VERSION_10_2_AND_LATER
#define AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3 DEPRECATED_IN_MAC_OS_X_VERSION_10_3_AND_LATER
#define AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4 DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER
#define AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5 DEPRECATED_IN_MAC_OS_X_VERSION_10_5_AND_LATER
#define AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_6 DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER
#define AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7 DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER
#endif

#ifdef __cplusplus
#define APPKIT_EXTERN		extern "C"
#define APPKIT_PRIVATE_EXTERN	__attribute__((visibility("hidden"))) extern "C"
#define APPKIT_PRIVATE          __attribute__((visibility("hidden")))
#else
#define APPKIT_EXTERN		extern
#define APPKIT_PRIVATE_EXTERN	__attribute__((visibility("hidden"))) extern
#define APPKIT_PRIVATE          __attribute__((visibility("hidden")))
#endif

#ifndef NS_SWIFT_BRIDGED_TYPEDEF
#if __has_attribute(swift_bridged_typedef)
#define NS_SWIFT_BRIDGED_TYPEDEF __attribute__((swift_bridged_typedef))
#else
#define NS_SWIFT_BRIDGED_TYPEDEF
#endif
#endif

/* These macros are for AppKit's own use. They may be modified or removed in a future release. */
#ifndef APPKIT_IVAR
#define APPKIT_IVAR DEPRECATED_MSG_ATTRIBUTE("AppKit instance variables are private, and the ability to access them will be removed in a future release.")
#endif


#define APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST API_UNAVAILABLE_BEGIN(ios)


#endif // _APPKITDEFINES_H

