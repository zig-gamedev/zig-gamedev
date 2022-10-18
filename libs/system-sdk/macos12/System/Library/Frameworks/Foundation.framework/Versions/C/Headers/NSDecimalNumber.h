/*	NSDecimalNumber.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSValue.h>
#import <Foundation/NSScanner.h>
#import <Foundation/NSDecimal.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSException.h>

NS_ASSUME_NONNULL_BEGIN

/***************	Exceptions		***********/

FOUNDATION_EXPORT NSExceptionName const NSDecimalNumberExactnessException;
FOUNDATION_EXPORT NSExceptionName const NSDecimalNumberOverflowException;
FOUNDATION_EXPORT NSExceptionName const NSDecimalNumberUnderflowException;
FOUNDATION_EXPORT NSExceptionName const NSDecimalNumberDivideByZeroException;

/***************	Rounding and Exception behavior		***********/

@class NSDecimalNumber;

@protocol NSDecimalNumberBehaviors

- (NSRoundingMode)roundingMode;

- (short)scale;
    // The scale could return NO_SCALE for no defined scale.

- (nullable NSDecimalNumber *)exceptionDuringOperation:(SEL)operation error:(NSCalculationError)error leftOperand:(NSDecimalNumber *)leftOperand rightOperand:(nullable NSDecimalNumber *)rightOperand;
    // Receiver can raise, return a new value, or return nil to ignore the exception.

@end

/***************	NSDecimalNumber: the class		***********/

@interface NSDecimalNumber : NSNumber {
@private
    signed   int _exponent:8;
    unsigned int _length:4;
    unsigned int _isNegative:1;
    unsigned int _isCompact:1;
    unsigned int _reserved:1;
    unsigned int _hasExternalRefCount:1;
    unsigned int _refs:16;
    unsigned short _mantissa[0]; /* GCC */
}

- (instancetype)initWithMantissa:(unsigned long long)mantissa exponent:(short)exponent isNegative:(BOOL)flag;
- (instancetype)initWithDecimal:(NSDecimal)dcm NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithString:(nullable NSString *)numberValue;
- (instancetype)initWithString:(nullable NSString *)numberValue locale:(nullable id)locale;

- (NSString *)descriptionWithLocale:(nullable id)locale;

@property (readonly) NSDecimal decimalValue;

+ (NSDecimalNumber *)decimalNumberWithMantissa:(unsigned long long)mantissa exponent:(short)exponent isNegative:(BOOL)flag;
+ (NSDecimalNumber *)decimalNumberWithDecimal:(NSDecimal)dcm;
+ (NSDecimalNumber *)decimalNumberWithString:(nullable NSString *)numberValue;
+ (NSDecimalNumber *)decimalNumberWithString:(nullable NSString *)numberValue locale:(nullable id)locale;

@property (class, readonly, copy) NSDecimalNumber *zero;
@property (class, readonly, copy) NSDecimalNumber *one;
@property (class, readonly, copy) NSDecimalNumber *minimumDecimalNumber;
@property (class, readonly, copy) NSDecimalNumber *maximumDecimalNumber;
@property (class, readonly, copy) NSDecimalNumber *notANumber;

- (NSDecimalNumber *)decimalNumberByAdding:(NSDecimalNumber *)decimalNumber;
- (NSDecimalNumber *)decimalNumberByAdding:(NSDecimalNumber *)decimalNumber withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;

- (NSDecimalNumber *)decimalNumberBySubtracting:(NSDecimalNumber *)decimalNumber;
- (NSDecimalNumber *)decimalNumberBySubtracting:(NSDecimalNumber *)decimalNumber withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;

- (NSDecimalNumber *)decimalNumberByMultiplyingBy:(NSDecimalNumber *)decimalNumber;
- (NSDecimalNumber *)decimalNumberByMultiplyingBy:(NSDecimalNumber *)decimalNumber withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;

- (NSDecimalNumber *)decimalNumberByDividingBy:(NSDecimalNumber *)decimalNumber;
- (NSDecimalNumber *)decimalNumberByDividingBy:(NSDecimalNumber *)decimalNumber withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;

- (NSDecimalNumber *)decimalNumberByRaisingToPower:(NSUInteger)power;
- (NSDecimalNumber *)decimalNumberByRaisingToPower:(NSUInteger)power withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;

- (NSDecimalNumber *)decimalNumberByMultiplyingByPowerOf10:(short)power;
- (NSDecimalNumber *)decimalNumberByMultiplyingByPowerOf10:(short)power withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;


- (NSDecimalNumber *)decimalNumberByRoundingAccordingToBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;
    // Round to the scale of the behavior.

- (NSComparisonResult)compare:(NSNumber *)decimalNumber;
    // compare two NSDecimalNumbers

@property (class, strong) id <NSDecimalNumberBehaviors> defaultBehavior;
    // One behavior per thread - The default behavior is
    //   rounding mode: NSRoundPlain
    //   scale: No defined scale (full precision)
    //   ignore exactnessException
    //   raise on overflow, underflow and divide by zero.

@property (readonly) const char *objCType NS_RETURNS_INNER_POINTER;
    // return 'd' for double
    
@property (readonly) double doubleValue;
    // return an approximate double value
    
@end

/***********	A class for defining common behaviors		*******/

@interface NSDecimalNumberHandler : NSObject <NSDecimalNumberBehaviors, NSCoding> {
  @private
    signed int _scale:16;
    unsigned _roundingMode:3;
    unsigned _raiseOnExactness:1;
    unsigned _raiseOnOverflow:1;
    unsigned _raiseOnUnderflow:1;
    unsigned _raiseOnDivideByZero:1;
    unsigned _unused:9;
    void *_reserved2;
    void *_reserved;
}

@property (class, readonly, strong) NSDecimalNumberHandler *defaultDecimalNumberHandler;
    // rounding mode: NSRoundPlain
    // scale: No defined scale (full precision)
    // ignore exactnessException (return nil)
    // raise on overflow, underflow and divide by zero.

- (instancetype)initWithRoundingMode:(NSRoundingMode)roundingMode scale:(short)scale raiseOnExactness:(BOOL)exact raiseOnOverflow:(BOOL)overflow raiseOnUnderflow:(BOOL)underflow raiseOnDivideByZero:(BOOL)divideByZero NS_DESIGNATED_INITIALIZER;

+ (instancetype)decimalNumberHandlerWithRoundingMode:(NSRoundingMode)roundingMode scale:(short)scale raiseOnExactness:(BOOL)exact raiseOnOverflow:(BOOL)overflow raiseOnUnderflow:(BOOL)underflow raiseOnDivideByZero:(BOOL)divideByZero;

@end

/***********	Extensions to other classes		*******/

@interface NSNumber (NSDecimalNumberExtensions)

@property (readonly) NSDecimal decimalValue;
    // Could be silently inexact for float and double.

@end

@interface NSScanner (NSDecimalNumberScanning)

- (BOOL)scanDecimal:(nullable NSDecimal *)dcm
#if defined(__swift__) // Deprecated for Swift only:
    API_DEPRECATED_WITH_REPLACEMENT("scanDecimal()", macosx(10.0,10.15), ios(2.0,13.0), watchos(2.0,13.0), tvos(9.0,13.0))
#endif
    ;

@end

NS_ASSUME_NONNULL_END
