/*
 *  CLAvailability.h
 *  CoreLocation
 *
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

/*
#ifndef __CL_INDIRECT__
#error "Please #include <CoreLocation/CoreLocation.h> instead of this file directly."
#endif // __CL_INDIRECT__
*/

#import <Availability.h>
#import <os/availability.h>

#if defined(__has_include)
#if __has_include(<AppleFeatures/AppleFeatures.h>)
#include <AppleFeatures/AppleFeatures.h>
#endif // AppleFeatures
#else // !defined(__has_include)
#error "__has_include not defined."
#endif // __has_include

#ifdef __cplusplus
#define CL_EXTERN extern "C" __attribute__((visibility ("default")))
#else
#define CL_EXTERN extern __attribute__((visibility ("default")))
#endif
