//
//  CKDefines.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

#ifndef CK_EXTERN
#ifdef __cplusplus
#define CK_EXTERN   extern "C" __attribute__((visibility ("default")))
#else
#define CK_EXTERN   extern __attribute__((visibility ("default")))
#endif
#endif

#ifndef CK_HIDDEN
#define CK_HIDDEN   __attribute__((visibility("hidden")))
#endif

#ifndef CK_EXTERN_HIDDEN
#ifdef __cplusplus
#define CK_EXTERN_HIDDEN   extern "C" __attribute__((visibility ("hidden")))
#else
#define CK_EXTERN_HIDDEN   extern __attribute__((visibility ("hidden")))
#endif
#endif

#ifndef CK_SWIFT_AVAILABILITY
    #if defined(__swift__) && !defined(CK_BUILDING_CK)
        #define CK_SWIFT_AVAILABILITY(...) API_AVAILABLE(__VA_ARGS__)
        #define CK_SWIFT_DEPRECATED(...) API_DEPRECATED(__VA_ARGS__)
    #else
        #define CK_SWIFT_AVAILABILITY(...)
        #define CK_SWIFT_DEPRECATED(...)
    #endif
#endif

NS_ASSUME_NONNULL_END
