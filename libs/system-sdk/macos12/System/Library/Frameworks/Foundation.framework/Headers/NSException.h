/*	NSException.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>
#import <stdarg.h>
#import <setjmp.h>

@class NSString, NSDictionary, NSArray<ObjectType>, NSNumber;

NS_ASSUME_NONNULL_BEGIN

/***************	Generic Exception names		***************/

FOUNDATION_EXPORT NSExceptionName const NSGenericException;
FOUNDATION_EXPORT NSExceptionName const NSRangeException;
FOUNDATION_EXPORT NSExceptionName const NSInvalidArgumentException;
FOUNDATION_EXPORT NSExceptionName const NSInternalInconsistencyException;

FOUNDATION_EXPORT NSExceptionName const NSMallocException;

FOUNDATION_EXPORT NSExceptionName const NSObjectInaccessibleException;
FOUNDATION_EXPORT NSExceptionName const NSObjectNotAvailableException;
FOUNDATION_EXPORT NSExceptionName const NSDestinationInvalidException;
    
FOUNDATION_EXPORT NSExceptionName const NSPortTimeoutException;
FOUNDATION_EXPORT NSExceptionName const NSInvalidSendPortException;
FOUNDATION_EXPORT NSExceptionName const NSInvalidReceivePortException;
FOUNDATION_EXPORT NSExceptionName const NSPortSendException;
FOUNDATION_EXPORT NSExceptionName const NSPortReceiveException;

FOUNDATION_EXPORT NSExceptionName const NSOldStyleException;

FOUNDATION_EXPORT NSExceptionName const NSInconsistentArchiveException;

/***************	Exception object	***************/

#if __OBJC2__
__attribute__((__objc_exception__))
#endif
@interface NSException : NSObject <NSCopying, NSSecureCoding> {
    @private
    NSString		*name;
    NSString		*reason;
    NSDictionary	*userInfo;
    id			reserved;
}

+ (NSException *)exceptionWithName:(NSExceptionName)name reason:(nullable NSString *)reason userInfo:(nullable NSDictionary *)userInfo;
- (instancetype)initWithName:(NSExceptionName)aName reason:(nullable NSString *)aReason userInfo:(nullable NSDictionary *)aUserInfo NS_DESIGNATED_INITIALIZER;

@property (readonly, copy) NSExceptionName name;
@property (nullable, readonly, copy) NSString *reason;
@property (nullable, readonly, copy) NSDictionary *userInfo;

@property (readonly, copy) NSArray<NSNumber *> *callStackReturnAddresses API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (readonly, copy) NSArray<NSString *> *callStackSymbols API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (void)raise;

@end

@interface NSException (NSExceptionRaisingConveniences)

+ (void)raise:(NSExceptionName)name format:(NSString *)format, ... NS_FORMAT_FUNCTION(2,3);
+ (void)raise:(NSExceptionName)name format:(NSString *)format arguments:(va_list)argList NS_FORMAT_FUNCTION(2,0);

@end


#define NS_DURING		@try {
#define NS_HANDLER		} @catch (NSException *localException) {
#define NS_ENDHANDLER		}
#define NS_VALUERETURN(v,t)	return (v)
#define NS_VOIDRETURN		return


typedef void NSUncaughtExceptionHandler(NSException *exception);

FOUNDATION_EXPORT NSUncaughtExceptionHandler * _Nullable NSGetUncaughtExceptionHandler(void);
FOUNDATION_EXPORT void NSSetUncaughtExceptionHandler(NSUncaughtExceptionHandler * _Nullable);


#if __clang__
#define __PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wformat-extra-args\"")

#define __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS _Pragma("clang diagnostic pop")
#else
#define __PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS
#define __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS
#endif

@class NSAssertionHandler;

#if (defined(__STDC_VERSION__) && (199901L <= __STDC_VERSION__)) || (defined(__cplusplus) && (201103L <= __cplusplus))

#if !defined(NS_BLOCK_ASSERTIONS)

#if !defined(_NSAssertBody)
#define NSAssert(condition, desc, ...)	\
    do {				\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
	if (__builtin_expect(!(condition), 0)) {		\
	    [[NSAssertionHandler currentHandler] handleFailureInMethod:_cmd \
		object:self file:@(__FILE_NAME__) \
	    	lineNumber:__LINE__ description:(desc), ##__VA_ARGS__]; \
	}				\
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS \
    } while(0)
#endif

#if !defined(_NSCAssertBody)
#define NSCAssert(condition, desc, ...) \
    do {				\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
	if (__builtin_expect(!(condition), 0)) {		\
	    [[NSAssertionHandler currentHandler] handleFailureInFunction:(NSString * _Nonnull)@(__PRETTY_FUNCTION__) \
		file:@(__FILE_NAME__) \
	    	lineNumber:__LINE__ description:(desc), ##__VA_ARGS__]; \
	}				\
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS \
    } while(0)
#endif

#else // NS_BLOCK_ASSERTIONS defined

#if !defined(_NSAssertBody)
#define NSAssert(condition, desc, ...) do {} while (0)
#endif

#if !defined(_NSCAssertBody)
#define NSCAssert(condition, desc, ...) do {} while (0)
#endif

#endif

#if !defined(_NSAssertBody)
#define NSAssert1(condition, desc, arg1) NSAssert((condition), (desc), (arg1))
#define NSAssert2(condition, desc, arg1, arg2) NSAssert((condition), (desc), (arg1), (arg2))
#define NSAssert3(condition, desc, arg1, arg2, arg3) NSAssert((condition), (desc), (arg1), (arg2), (arg3))
#define NSAssert4(condition, desc, arg1, arg2, arg3, arg4) NSAssert((condition), (desc), (arg1), (arg2), (arg3), (arg4))
#define NSAssert5(condition, desc, arg1, arg2, arg3, arg4, arg5) NSAssert((condition), (desc), (arg1), (arg2), (arg3), (arg4), (arg5))
#define NSParameterAssert(condition) NSAssert((condition), @"Invalid parameter not satisfying: %@", @#condition)
#endif

#if !defined(_NSCAssertBody)
#define NSCAssert1(condition, desc, arg1) NSCAssert((condition), (desc), (arg1))
#define NSCAssert2(condition, desc, arg1, arg2) NSCAssert((condition), (desc), (arg1), (arg2))
#define NSCAssert3(condition, desc, arg1, arg2, arg3) NSCAssert((condition), (desc), (arg1), (arg2), (arg3))
#define NSCAssert4(condition, desc, arg1, arg2, arg3, arg4) NSCAssert((condition), (desc), (arg1), (arg2), (arg3), (arg4))
#define NSCAssert5(condition, desc, arg1, arg2, arg3, arg4, arg5) NSCAssert((condition), (desc), (arg1), (arg2), (arg3), (arg4), (arg5))
#define NSCParameterAssert(condition) NSCAssert((condition), @"Invalid parameter not satisfying: %@", @#condition)
#endif

#endif


/* Non-vararg implementation of asserts (ignore) */
#if !defined(NS_BLOCK_ASSERTIONS)
#if !defined(_NSAssertBody)
#define _NSAssertBody(condition, desc, arg1, arg2, arg3, arg4, arg5)	\
    do {						\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
	if (!(condition)) {				\
            NSString *__assert_file__ = [NSString stringWithUTF8String:__FILE__]; \
            __assert_file__ = __assert_file__ ? __assert_file__ : @"<Unknown File>"; \
	    [[NSAssertionHandler currentHandler] handleFailureInMethod:_cmd object:self file:__assert_file__ \
	    	lineNumber:__LINE__ description:(desc), (arg1), (arg2), (arg3), (arg4), (arg5)];	\
	}						\
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS \
    } while(0)
#endif
#if !defined(_NSCAssertBody)
#define _NSCAssertBody(condition, desc, arg1, arg2, arg3, arg4, arg5)	\
    do {						\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
	if (!(condition)) {				\
            NSString *__assert_fn__ = [NSString stringWithUTF8String:__PRETTY_FUNCTION__]; \
            __assert_fn__ = __assert_fn__ ? __assert_fn__ : @"<Unknown Function>"; \
            NSString *__assert_file__ = [NSString stringWithUTF8String:__FILE__]; \
            __assert_file__ = __assert_file__ ? __assert_file__ : @"<Unknown File>"; \
	    [[NSAssertionHandler currentHandler] handleFailureInFunction:__assert_fn__ file:__assert_file__ \
	    	lineNumber:__LINE__ description:(desc), (arg1), (arg2), (arg3), (arg4), (arg5)];	\
	}						\
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS \
    } while(0)
#endif
#else
#if !defined(_NSAssertBody)
#define _NSAssertBody(condition, desc, arg1, arg2, arg3, arg4, arg5)
#endif
#if !defined(_NSCAssertBody)
#define _NSCAssertBody(condition, desc, arg1, arg2, arg3, arg4, arg5)
#endif
#endif


/*
 * Asserts to use in Objective-C method bodies
 */
 
#if !defined(NSAssert)
#define NSAssert5(condition, desc, arg1, arg2, arg3, arg4, arg5)	\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSAssertBody((condition), (desc), (arg1), (arg2), (arg3), (arg4), (arg5)) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSAssert4(condition, desc, arg1, arg2, arg3, arg4)	\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSAssertBody((condition), (desc), (arg1), (arg2), (arg3), (arg4), 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSAssert3(condition, desc, arg1, arg2, arg3)	\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSAssertBody((condition), (desc), (arg1), (arg2), (arg3), 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSAssert2(condition, desc, arg1, arg2)		\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSAssertBody((condition), (desc), (arg1), (arg2), 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSAssert1(condition, desc, arg1)		\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSAssertBody((condition), (desc), (arg1), 0, 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSAssert(condition, desc)			\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSAssertBody((condition), (desc), 0, 0, 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS
#endif

#if !defined(NSParameterAssert)
#define NSParameterAssert(condition)			\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSAssertBody((condition), @"Invalid parameter not satisfying: %s", #condition, 0, 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS
#endif


#if !defined(NSCAssert)
#define NSCAssert5(condition, desc, arg1, arg2, arg3, arg4, arg5)	\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSCAssertBody((condition), (desc), (arg1), (arg2), (arg3), (arg4), (arg5)) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSCAssert4(condition, desc, arg1, arg2, arg3, arg4)	\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSCAssertBody((condition), (desc), (arg1), (arg2), (arg3), (arg4), 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSCAssert3(condition, desc, arg1, arg2, arg3)	\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSCAssertBody((condition), (desc), (arg1), (arg2), (arg3), 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSCAssert2(condition, desc, arg1, arg2)	\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSCAssertBody((condition), (desc), (arg1), (arg2), 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSCAssert1(condition, desc, arg1)		\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSCAssertBody((condition), (desc), (arg1), 0, 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS

#define NSCAssert(condition, desc)			\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSCAssertBody((condition), (desc), 0, 0, 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS
#endif

#if !defined(NSCParameterAssert)
#define NSCParameterAssert(condition)			\
	__PRAGMA_PUSH_NO_EXTRA_ARG_WARNINGS \
    _NSCAssertBody((condition), @"Invalid parameter not satisfying: %s", #condition, 0, 0, 0, 0) \
        __PRAGMA_POP_NO_EXTRA_ARG_WARNINGS
#endif


FOUNDATION_EXPORT NSString * const NSAssertionHandlerKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@interface NSAssertionHandler : NSObject {
    @private
    void *_reserved;
}

@property (class, readonly, strong) NSAssertionHandler *currentHandler;

- (void)handleFailureInMethod:(SEL)selector object:(id)object file:(NSString *)fileName lineNumber:(NSInteger)line description:(nullable NSString *)format,... NS_FORMAT_FUNCTION(5,6);

- (void)handleFailureInFunction:(NSString *)functionName file:(NSString *)fileName lineNumber:(NSInteger)line description:(nullable NSString *)format,... NS_FORMAT_FUNCTION(4,5);

@end

NS_ASSUME_NONNULL_END
