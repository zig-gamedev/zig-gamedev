/*	NSInvocation.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#include <stdbool.h>

@class NSMethodSignature;

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_UNAVAILABLE("NSInvocation and related APIs not available")
@interface NSInvocation : NSObject

+ (NSInvocation *)invocationWithMethodSignature:(NSMethodSignature *)sig;

@property (readonly, retain) NSMethodSignature *methodSignature;

- (void)retainArguments;
@property (readonly) BOOL argumentsRetained;

@property (nullable, assign) id target;
@property SEL selector;

- (void)getReturnValue:(void *)retLoc;
- (void)setReturnValue:(void *)retLoc;

- (void)getArgument:(void *)argumentLocation atIndex:(NSInteger)idx;
- (void)setArgument:(void *)argumentLocation atIndex:(NSInteger)idx;

- (void)invoke;
- (void)invokeWithTarget:(id)target;

@end


#if TARGET_OS_OSX
#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5

enum _NSObjCValueType {
    NSObjCNoType = 0,
    NSObjCVoidType = 'v',
    NSObjCCharType = 'c',
    NSObjCShortType = 's',
    NSObjCLongType = 'l',
    NSObjCLonglongType = 'q',
    NSObjCFloatType = 'f',
    NSObjCDoubleType = 'd',
    NSObjCBoolType = 'B',
    NSObjCSelectorType = ':',
    NSObjCObjectType = '@',
    NSObjCStructType = '{',
    NSObjCPointerType = '^',
    NSObjCStringType = '*',
    NSObjCArrayType = '[',
    NSObjCUnionType = '(',
    NSObjCBitfield = 'b'
} API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));

typedef struct {
    NSInteger type API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
    union {
    	char charValue;
	short shortValue;
	long longValue;
	long long longlongValue;
	float floatValue;
	double doubleValue;
	bool boolValue;
	SEL selectorValue;
	id objectValue;
	void *pointerValue;
	void *structLocation;
	char *cStringLocation;
    } value API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
} NSObjCValue API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));

#endif
#endif

NS_ASSUME_NONNULL_END
