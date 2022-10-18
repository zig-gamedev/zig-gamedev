/*	NSObjCRuntime.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#include <TargetConditionals.h>
#include <Availability.h>

#if TARGET_OS_OSX || TARGET_OS_IPHONE
#include <objc/NSObjCRuntime.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include <AvailabilityMacros.h>
#endif

#if defined(__cplusplus)
#define FOUNDATION_EXTERN extern "C"
#else
#define FOUNDATION_EXTERN extern
#endif

#define FOUNDATION_EXPORT FOUNDATION_EXTERN
#define FOUNDATION_IMPORT FOUNDATION_EXTERN

#if !defined(NS_INLINE)
    #if defined(__GNUC__)
        #define NS_INLINE static __inline__ __attribute__((always_inline))
    #elif defined(__MWERKS__) || defined(__cplusplus)
        #define NS_INLINE static inline
    #elif defined(_MSC_VER)
        #define NS_INLINE static __inline
    #endif
#endif

#if !defined(FOUNDATION_STATIC_INLINE)
#define FOUNDATION_STATIC_INLINE static __inline__
#endif

#if !defined(FOUNDATION_EXTERN_INLINE)
#define FOUNDATION_EXTERN_INLINE extern __inline__
#endif

#if !defined(NS_REQUIRES_NIL_TERMINATION)
    #if defined(__APPLE_CC__) && (__APPLE_CC__ >= 5549)
        #define NS_REQUIRES_NIL_TERMINATION __attribute__((sentinel(0,1)))
    #else
        #define NS_REQUIRES_NIL_TERMINATION __attribute__((sentinel))
    #endif
#endif

#if !defined(NS_BLOCKS_AVAILABLE)
    #if __BLOCKS__ && (MAC_OS_X_VERSION_10_6 <= MAC_OS_X_VERSION_MAX_ALLOWED || __IPHONE_4_0 <= __IPHONE_OS_VERSION_MAX_ALLOWED)
        #define NS_BLOCKS_AVAILABLE 1
    #else
        #define NS_BLOCKS_AVAILABLE 0
    #endif
#endif

// Marks APIs whose iOS versions are nonatomic, that is cannot be set/get from multiple threads safely without additional synchronization
#if __clang_tapi__
    #if !defined(NS_NONATOMIC_IOSONLY)
        #if TARGET_OS_OSX || TARGET_OS_MACCATALYST
            #if __has_feature(objc_property_explicit_atomic)
                #define NS_NONATOMIC_IOSONLY atomic
            #else
                #define NS_NONATOMIC_IOSONLY
            #endif
        #else
            #define NS_NONATOMIC_IOSONLY nonatomic
        #endif
    #endif
#else
    #if !defined(NS_NONATOMIC_IOSONLY)
        #if TARGET_OS_IPHONE
            #define NS_NONATOMIC_IOSONLY nonatomic
        #else
            #if __has_feature(objc_property_explicit_atomic)
                #define NS_NONATOMIC_IOSONLY atomic
            #else
                #define NS_NONATOMIC_IOSONLY
            #endif
        #endif
    #endif
#endif

// Use NS_NONATOMIC_IOSONLY instead of this older macro
#if !defined(NS_NONATOMIC_IPHONEONLY)
#define NS_NONATOMIC_IPHONEONLY NS_NONATOMIC_IOSONLY
#endif

// Marks APIs which format strings by taking a format string and optional varargs as arguments
#if !defined(NS_FORMAT_FUNCTION)
    #if (__GNUC__*10+__GNUC_MINOR__ >= 42)
	#define NS_FORMAT_FUNCTION(F,A) __attribute__((format(__NSString__, F, A)))
    #else
	#define NS_FORMAT_FUNCTION(F,A)
    #endif
#endif

// Marks APIs which are often used to process (take and return) format strings, so they can be used in place of a constant format string parameter in APIs
#if !defined(NS_FORMAT_ARGUMENT)
    #if defined(__clang__)
	#define NS_FORMAT_ARGUMENT(A) __attribute__ ((format_arg(A)))
    #else
	#define NS_FORMAT_ARGUMENT(A)
    #endif
#endif

// Some compilers provide the capability to test if certain features are available. This macro provides a compatibility path for other compilers.
#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef __has_extension
#define __has_extension(x) 0
#endif

// Some compilers provide the capability to test if certain attributes are available. This macro provides a compatibility path for other compilers.
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

// Marks methods and functions which return an object that needs to be released by the caller but whose names are not consistent with Cocoa naming rules. The recommended fix to this is to rename the methods or functions, but this macro can be used to let the clang static analyzer know of any exceptions that cannot be fixed.
// This macro is ONLY to be used in exceptional circumstances, not to annotate functions which conform to the Cocoa naming rules.
#if __has_feature(attribute_ns_returns_retained)
#define NS_RETURNS_RETAINED __attribute__((ns_returns_retained))
#else
#define NS_RETURNS_RETAINED
#endif

// Marks methods and functions which return an object that may need to be retained by the caller but whose names are not consistent with Cocoa naming rules. The recommended fix to this is to rename the methods or functions, but this macro can be used to let the clang static analyzer know of any exceptions that cannot be fixed.
// This macro is ONLY to be used in exceptional circumstances, not to annotate functions which conform to the Cocoa naming rules.
#if __has_feature(attribute_ns_returns_not_retained)
#define NS_RETURNS_NOT_RETAINED __attribute__((ns_returns_not_retained))
#else
#define NS_RETURNS_NOT_RETAINED
#endif

#ifndef NS_RETURNS_INNER_POINTER
#if __has_attribute(objc_returns_inner_pointer)
#define NS_RETURNS_INNER_POINTER __attribute__((objc_returns_inner_pointer))
#else
#define NS_RETURNS_INNER_POINTER
#endif
#endif

// Marks methods and functions which cannot be used when compiling in automatic reference counting mode.
#if __has_feature(objc_arc)
#define NS_AUTOMATED_REFCOUNT_UNAVAILABLE __attribute__((unavailable("not available in automatic reference counting mode")))
#else
#define NS_AUTOMATED_REFCOUNT_UNAVAILABLE
#endif

// Marks classes which cannot participate in the ARC weak reference feature.
#if __has_attribute(objc_arc_weak_reference_unavailable)
#define NS_AUTOMATED_REFCOUNT_WEAK_UNAVAILABLE __attribute__((objc_arc_weak_reference_unavailable))
#else
#define NS_AUTOMATED_REFCOUNT_WEAK_UNAVAILABLE
#endif

// Marks classes that must specify @dynamic or @synthesize for properties in their @implementation (property getters & setters will not be synthesized unless the @synthesize directive is used)
#if __has_attribute(objc_requires_property_definitions)
#define NS_REQUIRES_PROPERTY_DEFINITIONS __attribute__((objc_requires_property_definitions)) 
#else
#define NS_REQUIRES_PROPERTY_DEFINITIONS
#endif

// Decorates methods in which the receiver may be replaced with the result of the method. 
#if __has_feature(attribute_ns_consumes_self)
#define NS_REPLACES_RECEIVER __attribute__((ns_consumes_self)) NS_RETURNS_RETAINED
#else
#define NS_REPLACES_RECEIVER
#endif

#if __has_feature(attribute_ns_consumed)
#define NS_RELEASES_ARGUMENT __attribute__((ns_consumed))
#else
#define NS_RELEASES_ARGUMENT
#endif

// Mark local variables of type 'id' or pointer-to-ObjC-object-type so that values stored into that local variable are not aggressively released by the compiler during optimization, but are held until either the variable is assigned to again, or the end of the scope (such as a compound statement, or method definition) of the local variable.
#ifndef NS_VALID_UNTIL_END_OF_SCOPE
#if __has_attribute(objc_precise_lifetime)
#define NS_VALID_UNTIL_END_OF_SCOPE __attribute__((objc_precise_lifetime))
#else
#define NS_VALID_UNTIL_END_OF_SCOPE
#endif
#endif

// Annotate classes which are root classes as really being root classes
#ifndef NS_ROOT_CLASS
#if __has_attribute(objc_root_class)
#define NS_ROOT_CLASS __attribute__((objc_root_class))
#else
#define NS_ROOT_CLASS
#endif
#endif

#ifndef NS_REQUIRES_SUPER
#if __has_attribute(objc_requires_super)
#define NS_REQUIRES_SUPER __attribute__((objc_requires_super))
#else
#define NS_REQUIRES_SUPER
#endif
#endif

#ifndef NS_DESIGNATED_INITIALIZER
#if __has_attribute(objc_designated_initializer)
#define NS_DESIGNATED_INITIALIZER __attribute__((objc_designated_initializer))
#else
#define NS_DESIGNATED_INITIALIZER
#endif
#endif

#ifndef NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
#if __has_attribute(objc_protocol_requires_explicit_implementation)
#define NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION __attribute__((objc_protocol_requires_explicit_implementation))
#else
#define NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
#endif
#endif

#if __has_attribute(not_tail_called)
#define NS_NO_TAIL_CALL __attribute__((not_tail_called))
#else
#define NS_NO_TAIL_CALL
#endif

#if !__has_feature(objc_instancetype)
#undef instancetype
#define instancetype id
#endif

#if __has_attribute(swift_async)
#  define NS_SWIFT_DISABLE_ASYNC __attribute__((swift_async(none)))
#  define NS_SWIFT_ASYNC(COMPLETION_BLOCK_INDEX) __attribute__((swift_async(not_swift_private, COMPLETION_BLOCK_INDEX)))
#  define NS_REFINED_FOR_SWIFT_ASYNC(COMPLETION_BLOCK_INDEX) __attribute__((swift_async(swift_private, COMPLETION_BLOCK_INDEX)))
#else
#  define NS_SWIFT_DISABLE_ASYNC
#  define NS_SWIFT_ASYNC(COMPLETION_BLOCK_INDEX)
#  define NS_REFINED_FOR_SWIFT_ASYNC(COMPLETION_BLOCK_INDEX)
#endif
#if __has_attribute(swift_async_name)
#  define NS_SWIFT_ASYNC_NAME(NAME) __attribute__((swift_async_name(#NAME)))
#else
#  define NS_SWIFT_ASYNC_NAME(NAME)
#endif
#if __has_attribute(swift_attr)
#  define NS_SWIFT_UI_ACTOR __attribute__((swift_attr("@UIActor")))
#else
#  define NS_SWIFT_UI_ACTOR
#endif
#if __has_attribute(swift_async_error)
#  define NS_SWIFT_ASYNC_NOTHROW __attribute__((swift_async_error(none)))
#  define NS_SWIFT_ASYNC_THROWS_ON_TRUE(TRUE_PARAMETER_INDEX) __attribute__((swift_async_error(nonzero_argument, TRUE_PARAMETER_INDEX)))
#  define NS_SWIFT_ASYNC_THROWS_ON_FALSE(FALSE_PARAMETER_INDEX)  __attribute__((swift_async_error(zero_argument, FALSE_PARAMETER_INDEX)))
#else
#  define NS_SWIFT_ASYNC_NOTHROW
#  define NS_SWIFT_ASYNC_THROWS_ON_TRUE(TRUE_PARAMETER_INDEX)
#  define NS_SWIFT_ASYNC_THROWS_ON_FALSE(FALSE_PARAMETER_INDEX)
#endif

#if !defined(NS_UNAVAILABLE)
#define NS_UNAVAILABLE UNAVAILABLE_ATTRIBUTE
#endif

#if !defined(__unsafe_unretained)
#define __unsafe_unretained
#endif

#include <CoreFoundation/CFAvailability.h>

#define NS_AVAILABLE(_mac, _ios) CF_AVAILABLE(_mac, _ios)
#define NS_AVAILABLE_MAC(_mac) CF_AVAILABLE_MAC(_mac)
#define NS_AVAILABLE_IOS(_ios) CF_AVAILABLE_IOS(_ios)

#define NS_DEPRECATED(_macIntro, _macDep, _iosIntro, _iosDep, ...) CF_DEPRECATED(_macIntro, _macDep, _iosIntro, _iosDep, __VA_ARGS__)
#define NS_DEPRECATED_MAC(_macIntro, _macDep, ...) CF_DEPRECATED_MAC(_macIntro, _macDep, __VA_ARGS__)
#define NS_DEPRECATED_IOS(_iosIntro, _iosDep, ...) CF_DEPRECATED_IOS(_iosIntro, _iosDep, __VA_ARGS__)

#define NS_DEPRECATED_WITH_REPLACEMENT_MAC(_rep, _macIntroduced, _macDeprecated) API_DEPRECATED_WITH_REPLACEMENT(_rep, macosx(_macIntroduced, _macDeprecated)) API_UNAVAILABLE(ios, watchos, tvos)

#define NS_ENUM_AVAILABLE(_mac, _ios) CF_ENUM_AVAILABLE(_mac, _ios)
#define NS_ENUM_AVAILABLE_MAC(_mac) CF_ENUM_AVAILABLE_MAC(_mac)
#define NS_ENUM_AVAILABLE_IOS(_ios) CF_ENUM_AVAILABLE_IOS(_ios)

#define NS_ENUM_DEPRECATED(_macIntro, _macDep, _iosIntro, _iosDep, ...) CF_ENUM_DEPRECATED(_macIntro, _macDep, _iosIntro, _iosDep, __VA_ARGS__)
#define NS_ENUM_DEPRECATED_MAC(_macIntro, _macDep, ...) CF_ENUM_DEPRECATED_MAC(_macIntro, _macDep, __VA_ARGS__)
#define NS_ENUM_DEPRECATED_IOS(_iosIntro, _iosDep, ...) CF_ENUM_DEPRECATED_IOS(_iosIntro, _iosDep, __VA_ARGS__)

#define NS_AVAILABLE_IPHONE(_ios) CF_AVAILABLE_IOS(_ios)
#define NS_DEPRECATED_IPHONE(_iosIntro, _iosDep) CF_DEPRECATED_IOS(_iosIntro, _iosDep)

/* NS_ENUM supports the use of one or two arguments. The first argument is always the integer type used for the values of the enum. The second argument is an optional type name for the macro. When specifying a type name, you must precede the macro with 'typedef' like so:
 
typedef NS_ENUM(NSInteger, NSComparisonResult) {
    ...
};
 
If you do not specify a type name, do not use 'typedef'. For example:
 
NS_ENUM(NSInteger) {
    ...
};
*/
#define NS_ENUM(...) CF_ENUM(__VA_ARGS__)
#define NS_OPTIONS(_type, _name) CF_OPTIONS(_type, _name)

/* NS_CLOSED_ENUM is only for enums which are guaranteed to never gain an additional case.
 
Usually, this is because the enum represents a mathematically complete set. For example: NSComparisonResult. Once an enum is marked as closed, it is a binary and source incompatible change to add a new value. If there is any doubt about an enum gaining a private or additional public case in the future, use NS_ENUM instead.
*/
#define NS_CLOSED_ENUM(_type, _name) CF_CLOSED_ENUM(_type, _name)

/* */

#define _NS_TYPED_ENUM _CF_TYPED_ENUM
#define _NS_TYPED_EXTENSIBLE_ENUM _CF_TYPED_EXTENSIBLE_ENUM

// Note: NS_TYPED_ENUM is preferred to NS_STRING_ENUM
#define NS_STRING_ENUM _NS_TYPED_ENUM
// Note: NS_TYPED_EXTENSIBLE_ENUM is preferred to NS_EXTENSIBLE_STRING_ENUM
#define NS_EXTENSIBLE_STRING_ENUM _NS_TYPED_EXTENSIBLE_ENUM

#define NS_TYPED_ENUM _NS_TYPED_ENUM
#define NS_TYPED_EXTENSIBLE_ENUM _NS_TYPED_EXTENSIBLE_ENUM


#define __NS_ERROR_ENUM_GET_MACRO(_1, _2, NAME, ...) NAME
#if ((__cplusplus && __cplusplus >= 201103L && (__has_extension(cxx_strong_enums) || __has_feature(objc_fixed_enum))) || (!__cplusplus && __has_feature(objc_fixed_enum))) && __has_attribute(ns_error_domain)
#define __NS_NAMED_ERROR_ENUM(_domain, _name)     enum _name : NSInteger _name; enum __attribute__((ns_error_domain(_domain))) _name : NSInteger
#define __NS_ANON_ERROR_ENUM(_domain)             enum __attribute__((ns_error_domain(_domain))) : NSInteger
#else
#define __NS_NAMED_ERROR_ENUM(_domain, _name) NS_ENUM(NSInteger, _name)
#define __NS_ANON_ERROR_ENUM(_domain) NS_ENUM(NSInteger)
#endif

/* NS_ERROR_ENUM supports the use of one or two arguments. The first argument is always the domain specifier for the enum. The second argument is an optional name of the typedef for the macro. When specifying a name for of the typedef, you must precede the macro with 'typedef' like so:
 
 typedef NS_ERROR_ENUM(MyErrorDomain, SomeErrorCodes) {
 ...
 };
 
 If you do not specify a typedef name, do not use 'typedef', like so:
 
 NS_ERROR_ENUM(MyErrorDomain) {
 ...
 };
 */
#define NS_ERROR_ENUM(...) __NS_ERROR_ENUM_GET_MACRO(__VA_ARGS__, __NS_NAMED_ERROR_ENUM, __NS_ANON_ERROR_ENUM)(__VA_ARGS__)

#define NS_SWIFT_BRIDGED_TYPEDEF CF_SWIFT_BRIDGED_TYPEDEF

// This macro is to be used by system frameworks to support the weak linking of classes. Weak linking is supported on iOS 3.1 and Mac OS X 10.6.8 or later.
#if (__MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_6 || __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_3_1) && \
    ((__has_feature(objc_weak_class) || \
     (defined(__llvm__) && defined(__APPLE_CC__) && (__APPLE_CC__ >= 5658)) || \
     (defined(__APPLE_CC__) && (__APPLE_CC__ >= 5666))))
#define NS_CLASS_AVAILABLE(_mac, _ios) __attribute__((visibility("default"))) NS_AVAILABLE(_mac, _ios)
#define NS_CLASS_DEPRECATED(_mac, _macDep, _ios, _iosDep, ...) __attribute__((visibility("default"))) NS_DEPRECATED(_mac, _macDep, _ios, _iosDep, __VA_ARGS__)
#else
// class weak import is not supported
#define NS_CLASS_AVAILABLE(_mac, _ios)
#define NS_CLASS_DEPRECATED(_mac, _macDep, _ios, _iosDep, ...)
#endif

#define NS_CLASS_AVAILABLE_IOS(_ios) NS_CLASS_AVAILABLE(NA, _ios)
#define NS_CLASS_AVAILABLE_MAC(_mac) NS_CLASS_AVAILABLE(_mac, NA)
#define NS_CLASS_DEPRECATED_MAC(_macIntro, _macDep, ...) NS_CLASS_DEPRECATED(_macIntro, _macDep, NA, NA, __VA_ARGS__)
#define NS_CLASS_DEPRECATED_IOS(_iosIntro, _iosDep, ...) NS_CLASS_DEPRECATED(NA, NA, _iosIntro, _iosDep, __VA_ARGS__)

#define NS_EXTENSION_UNAVAILABLE(_msg)      __OS_EXTENSION_UNAVAILABLE(_msg)
#define NS_EXTENSION_UNAVAILABLE_MAC(_msg)  __OSX_EXTENSION_UNAVAILABLE(_msg)
#define NS_EXTENSION_UNAVAILABLE_IOS(_msg)  __IOS_EXTENSION_UNAVAILABLE(_msg)

#define NS_SWIFT_UNAVAILABLE(_msg) CF_SWIFT_UNAVAILABLE(_msg)

#ifndef NS_ASSUME_NONNULL_BEGIN
#define NS_ASSUME_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#endif
#ifndef NS_ASSUME_NONNULL_END
#define NS_ASSUME_NONNULL_END   _Pragma("clang assume_nonnull end")
#endif

#define NS_REFINED_FOR_SWIFT CF_REFINED_FOR_SWIFT

#define NS_SWIFT_NAME(_name) CF_SWIFT_NAME(_name)

#define NS_NOESCAPE CF_NOESCAPE

#if __has_attribute(swift_error)
#define NS_SWIFT_NOTHROW __attribute__((swift_error(none)))
#else
#define NS_SWIFT_NOTHROW
#endif

#define NS_WARN_UNUSED_RESULT CF_WARN_UNUSED_RESULT

// There is no need to use this macro any longer, the last Foundation epoch 
// was 8 and that is now assumed to be the final version. 
#define FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(__epoch__) (!defined(SWIFT_CLASS_EXTRA))

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT double NSFoundationVersionNumber;

#if TARGET_OS_MAC
#define NSFoundationVersionNumber10_0	397.40
#define NSFoundationVersionNumber10_1	425.00
#define NSFoundationVersionNumber10_1_1	425.00
#define NSFoundationVersionNumber10_1_2	425.00
#define NSFoundationVersionNumber10_1_3	425.00
#define NSFoundationVersionNumber10_1_4	425.00
#define NSFoundationVersionNumber10_2	462.00
#define NSFoundationVersionNumber10_2_1	462.00
#define NSFoundationVersionNumber10_2_2	462.00
#define NSFoundationVersionNumber10_2_3	462.00
#define NSFoundationVersionNumber10_2_4	462.00
#define NSFoundationVersionNumber10_2_5	462.00
#define NSFoundationVersionNumber10_2_6	462.00
#define NSFoundationVersionNumber10_2_7	462.70
#define NSFoundationVersionNumber10_2_8	462.70
#define NSFoundationVersionNumber10_3	500.00
#define NSFoundationVersionNumber10_3_1	500.00
#define NSFoundationVersionNumber10_3_2	500.30
#define NSFoundationVersionNumber10_3_3	500.54
#define NSFoundationVersionNumber10_3_4	500.56
#define NSFoundationVersionNumber10_3_5	500.56
#define NSFoundationVersionNumber10_3_6	500.56
#define NSFoundationVersionNumber10_3_7	500.56
#define NSFoundationVersionNumber10_3_8	500.56
#define NSFoundationVersionNumber10_3_9	500.58
#define NSFoundationVersionNumber10_4	567.00
#define NSFoundationVersionNumber10_4_1	567.00
#define NSFoundationVersionNumber10_4_2	567.12
#define NSFoundationVersionNumber10_4_3	567.21
#define NSFoundationVersionNumber10_4_4_Intel	567.23
#define NSFoundationVersionNumber10_4_4_PowerPC	567.21
#define NSFoundationVersionNumber10_4_5	567.25
#define NSFoundationVersionNumber10_4_6	567.26
#define NSFoundationVersionNumber10_4_7	567.27
#define NSFoundationVersionNumber10_4_8	567.28
#define NSFoundationVersionNumber10_4_9	567.29
#define NSFoundationVersionNumber10_4_10	567.29
#define NSFoundationVersionNumber10_4_11	567.36
#define NSFoundationVersionNumber10_5	677.00
#define NSFoundationVersionNumber10_5_1	677.10
#define NSFoundationVersionNumber10_5_2 677.15
#define NSFoundationVersionNumber10_5_3 677.19
#define NSFoundationVersionNumber10_5_4 677.19
#define NSFoundationVersionNumber10_5_5 677.21
#define NSFoundationVersionNumber10_5_6 677.22
#define NSFoundationVersionNumber10_5_7 677.24
#define NSFoundationVersionNumber10_5_8 677.26
#define NSFoundationVersionNumber10_6 751.00
#define NSFoundationVersionNumber10_6_1 751.00
#define NSFoundationVersionNumber10_6_2 751.14
#define NSFoundationVersionNumber10_6_3 751.21
#define NSFoundationVersionNumber10_6_4 751.29
#define NSFoundationVersionNumber10_6_5 751.42
#define NSFoundationVersionNumber10_6_6 751.53
#define NSFoundationVersionNumber10_6_7 751.53
#define NSFoundationVersionNumber10_6_8 751.62
#define NSFoundationVersionNumber10_7 833.10
#define NSFoundationVersionNumber10_7_1 833.10
#define NSFoundationVersionNumber10_7_2 833.20
#define NSFoundationVersionNumber10_7_3 833.24
#define NSFoundationVersionNumber10_7_4 833.25
#define NSFoundationVersionNumber10_8 945.00
#define NSFoundationVersionNumber10_8_1 945.00
#define NSFoundationVersionNumber10_8_2 945.11
#define NSFoundationVersionNumber10_8_3 945.16
#define NSFoundationVersionNumber10_8_4 945.18
#define NSFoundationVersionNumber10_9 1056
#define NSFoundationVersionNumber10_9_1 1056
#define NSFoundationVersionNumber10_9_2 1056.13
#define NSFoundationVersionNumber10_10 1151.16
#define NSFoundationVersionNumber10_10_1 1151.16
#define NSFoundationVersionNumber10_10_2 1152.14
#define NSFoundationVersionNumber10_10_3 1153.20
#define NSFoundationVersionNumber10_10_4 1153.20
#define NSFoundationVersionNumber10_10_5 1154
#define NSFoundationVersionNumber10_10_Max 1199
#define NSFoundationVersionNumber10_11   1252
#define NSFoundationVersionNumber10_11_1 1255.1
#define NSFoundationVersionNumber10_11_2 1256.1
#define NSFoundationVersionNumber10_11_3 1256.1
#define NSFoundationVersionNumber10_11_4 1258
#define NSFoundationVersionNumber10_11_Max 1299
#endif

#if TARGET_OS_IPHONE
#define NSFoundationVersionNumber_iPhoneOS_2_0	678.24
#define NSFoundationVersionNumber_iPhoneOS_2_1  678.26
#define NSFoundationVersionNumber_iPhoneOS_2_2  678.29
#define NSFoundationVersionNumber_iPhoneOS_3_0  678.47
#define NSFoundationVersionNumber_iPhoneOS_3_1  678.51
#define NSFoundationVersionNumber_iPhoneOS_3_2  678.60
#define NSFoundationVersionNumber_iOS_4_0  751.32
#define NSFoundationVersionNumber_iOS_4_1  751.37
#define NSFoundationVersionNumber_iOS_4_2  751.49
#define NSFoundationVersionNumber_iOS_4_3  751.49
#define NSFoundationVersionNumber_iOS_5_0  881.00
#define NSFoundationVersionNumber_iOS_5_1  890.10
#define NSFoundationVersionNumber_iOS_6_0  992.00
#define NSFoundationVersionNumber_iOS_6_1  993.00
#define NSFoundationVersionNumber_iOS_7_0 1047.20
#define NSFoundationVersionNumber_iOS_7_1 1047.25
#define NSFoundationVersionNumber_iOS_8_0 1140.11
#define NSFoundationVersionNumber_iOS_8_1 1141.1
#define NSFoundationVersionNumber_iOS_8_2 1142.14
#define NSFoundationVersionNumber_iOS_8_3 1144.17
#define NSFoundationVersionNumber_iOS_8_4 1144.17
#define NSFoundationVersionNumber_iOS_8_x_Max 1199
#define NSFoundationVersionNumber_iOS_9_0 1240.1
#define NSFoundationVersionNumber_iOS_9_1 1241.14
#define NSFoundationVersionNumber_iOS_9_2 1242.12
#define NSFoundationVersionNumber_iOS_9_3 1242.12
#define NSFoundationVersionNumber_iOS_9_4 1280.25
#define NSFoundationVersionNumber_iOS_9_x_Max 1299
#endif

@class NSString, Protocol;

typedef NSString * NSExceptionName NS_TYPED_EXTENSIBLE_ENUM;
typedef NSString * NSRunLoopMode NS_TYPED_EXTENSIBLE_ENUM;

FOUNDATION_EXPORT NSString *NSStringFromSelector(SEL aSelector);
FOUNDATION_EXPORT SEL NSSelectorFromString(NSString *aSelectorName);

FOUNDATION_EXPORT NSString *NSStringFromClass(Class aClass);
FOUNDATION_EXPORT Class _Nullable NSClassFromString(NSString *aClassName);

FOUNDATION_EXPORT NSString *NSStringFromProtocol(Protocol *proto) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT Protocol * _Nullable NSProtocolFromString(NSString *namestr) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT const char *NSGetSizeAndAlignment(const char *typePtr, NSUInteger * _Nullable sizep, NSUInteger * _Nullable alignp);

FOUNDATION_EXPORT void NSLog(NSString *format, ...) NS_FORMAT_FUNCTION(1,2) NS_NO_TAIL_CALL;
FOUNDATION_EXPORT void NSLogv(NSString *format, va_list args) NS_FORMAT_FUNCTION(1,0) NS_NO_TAIL_CALL;

/*
 These constants are used to indicate how items in a request are ordered, from the first one given in a method invocation or function call to the last (that is, left to right in code).
 
 Given the function:
   NSComparisonResult f(int a, int b)
 
 If:
    a < b   then return NSOrderedAscending. The left operand is smaller than the right operand.
    a > b   then return NSOrderedDescending. The left operand is greater than the right operand.
    a == b  then return NSOrderedSame. The operands are equal.
*/
typedef NS_CLOSED_ENUM(NSInteger, NSComparisonResult) {
    NSOrderedAscending = -1L,
    NSOrderedSame,
    NSOrderedDescending
};

typedef NSComparisonResult (^NSComparator)(id obj1, id obj2);

typedef NS_OPTIONS(NSUInteger, NSEnumerationOptions) {
    NSEnumerationConcurrent = (1UL << 0),
    NSEnumerationReverse = (1UL << 1),
};

typedef NS_OPTIONS(NSUInteger, NSSortOptions) {
    NSSortConcurrent = (1UL << 0),
    NSSortStable = (1UL << 4),
};

typedef NS_ENUM(NSInteger, NSQualityOfService) {
    NSQualityOfServiceUserInteractive = 0x21,
    NSQualityOfServiceUserInitiated = 0x19,
    NSQualityOfServiceUtility = 0x11,
    NSQualityOfServiceBackground = 0x09,
    NSQualityOfServiceDefault = -1
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

static const NSInteger NSNotFound = NSIntegerMax;

NS_ASSUME_NONNULL_END

#if !defined(YES)
    #define YES	(BOOL)1
#endif

#if !defined(NO)
    #define NO	(BOOL)0
#endif

#if !defined(NS_SUPPRESS_MIN_MAX_ABS)

#if (defined(__GNUC__) || defined(__clang__)) && !defined(__STRICT_ANSI__)

#if __clang__

#define __NSX_PASTE__(A,B) A##B

#if !defined(MIN)
    #define __NSMIN_IMPL__(A,B,L) ({ __typeof__(A) __NSX_PASTE__(__a,L) = (A); __typeof__(B) __NSX_PASTE__(__b,L) = (B); (__NSX_PASTE__(__a,L) < __NSX_PASTE__(__b,L)) ? __NSX_PASTE__(__a,L) : __NSX_PASTE__(__b,L); })
    #define MIN(A,B) __NSMIN_IMPL__(A,B,__COUNTER__)
#endif

#if !defined(MAX)
    #define __NSMAX_IMPL__(A,B,L) ({ __typeof__(A) __NSX_PASTE__(__a,L) = (A); __typeof__(B) __NSX_PASTE__(__b,L) = (B); (__NSX_PASTE__(__a,L) < __NSX_PASTE__(__b,L)) ? __NSX_PASTE__(__b,L) : __NSX_PASTE__(__a,L); })
    #define MAX(A,B) __NSMAX_IMPL__(A,B,__COUNTER__)
#endif

#if !defined(ABS)
    #define __NSABS_IMPL__(A,L) ({ __typeof__(A) __NSX_PASTE__(__a,L) = (A); (__NSX_PASTE__(__a,L) < 0) ? - __NSX_PASTE__(__a,L) : __NSX_PASTE__(__a,L); })
    #define ABS(A) __NSABS_IMPL__(A,__COUNTER__)
#endif

#else

#if !defined(MIN)
    #define MIN(A,B)	({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __a : __b; })
#endif

#if !defined(MAX)
    #define MAX(A,B)	({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __b : __a; })
#endif

#if !defined(ABS)
    #define ABS(A)	({ __typeof__(A) __a = (A); __a < 0 ? -__a : __a; })
#endif

#endif

#else

#if !defined(MIN)
    #define MIN(A,B)	((A) < (B) ? (A) : (B))
#endif

#if !defined(MAX)
    #define MAX(A,B)	((A) > (B) ? (A) : (B))
#endif

#if !defined(ABS)
    #define ABS(A)	((A) < 0 ? (-(A)) : (A))
#endif

#endif

#if __has_include(<ptrauth.h>)
#include <ptrauth.h>
#endif

#ifndef __ptrauth_objc_isa_pointer
#define __ptrauth_objc_isa_pointer
#endif

#endif

