/*	NSDecimal.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#include <limits.h>
#import <Foundation/NSObjCRuntime.h>

@class NSDictionary;

NS_ASSUME_NONNULL_BEGIN

/***************	Type definitions		***********/

// Rounding policies :
// Original
//    value 1.2  1.21  1.25  1.35  1.27
// Plain    1.2  1.2   1.3   1.4   1.3
// Down     1.2  1.2   1.2   1.3   1.2
// Up       1.2  1.3   1.3   1.4   1.3
// Bankers  1.2  1.2   1.2   1.4   1.3

typedef NS_ENUM(NSUInteger, NSRoundingMode) {
    NSRoundPlain,   // Round up on a tie
    NSRoundDown,    // Always down == truncate
    NSRoundUp,      // Always up
    NSRoundBankers  // on a tie round so last digit is even
};

typedef NS_ENUM(NSUInteger, NSCalculationError) {
    NSCalculationNoError = 0,
    NSCalculationLossOfPrecision, // Result lost precision
    NSCalculationUnderflow,       // Result became 0
    NSCalculationOverflow,        // Result exceeds possible representation
    NSCalculationDivideByZero
};

#define NSDecimalMaxSize (8)
    // Give a precision of at least 38 decimal digits, 128 binary positions.

#define NSDecimalNoScale SHRT_MAX

typedef struct {
    signed   int _exponent:8;
    unsigned int _length:4;     // length == 0 && isNegative -> NaN
    unsigned int _isNegative:1;
    unsigned int _isCompact:1;
    unsigned int _reserved:18;
    unsigned short _mantissa[NSDecimalMaxSize];
} NSDecimal;

NS_INLINE BOOL NSDecimalIsNotANumber(const NSDecimal *dcm)
  { return ((dcm->_length == 0) && dcm->_isNegative); }

/***************	Operations		***********/

FOUNDATION_EXPORT void NSDecimalCopy(NSDecimal *destination, const NSDecimal *source);

FOUNDATION_EXPORT void NSDecimalCompact(NSDecimal *number);

FOUNDATION_EXPORT NSComparisonResult NSDecimalCompare(const NSDecimal *leftOperand, const NSDecimal *rightOperand);
    // NSDecimalCompare:Compares leftOperand and rightOperand.

FOUNDATION_EXPORT void NSDecimalRound(NSDecimal *result, const NSDecimal *number, NSInteger scale, NSRoundingMode roundingMode);
    // Rounds num to the given scale using the given mode.
    // result may be a pointer to same space as num.
    // scale indicates number of significant digits after the decimal point

FOUNDATION_EXPORT NSCalculationError NSDecimalNormalize(NSDecimal *number1, NSDecimal *number2, NSRoundingMode roundingMode);

FOUNDATION_EXPORT NSCalculationError NSDecimalAdd(NSDecimal *result, const NSDecimal *leftOperand, const NSDecimal *rightOperand, NSRoundingMode roundingMode);
    // Exact operations. result may be a pointer to same space as leftOperand or rightOperand

FOUNDATION_EXPORT NSCalculationError NSDecimalSubtract(NSDecimal *result, const NSDecimal *leftOperand, const NSDecimal *rightOperand, NSRoundingMode roundingMode);
    // Exact operations. result may be a pointer to same space as leftOperand or rightOperand

FOUNDATION_EXPORT NSCalculationError NSDecimalMultiply(NSDecimal *result, const NSDecimal *leftOperand, const NSDecimal *rightOperand, NSRoundingMode roundingMode);
    // Exact operations. result may be a pointer to same space as leftOperand or rightOperand

FOUNDATION_EXPORT NSCalculationError NSDecimalDivide(NSDecimal *result, const NSDecimal *leftOperand, const NSDecimal *rightOperand, NSRoundingMode roundingMode);
    // Division could be silently inexact;
    // Exact operations. result may be a pointer to same space as leftOperand or rightOperand
    
FOUNDATION_EXPORT NSCalculationError NSDecimalPower(NSDecimal *result, const NSDecimal *number, NSUInteger power, NSRoundingMode roundingMode);

FOUNDATION_EXPORT NSCalculationError NSDecimalMultiplyByPowerOf10(NSDecimal *result, const NSDecimal *number, short power, NSRoundingMode roundingMode);

FOUNDATION_EXPORT NSString *NSDecimalString(const NSDecimal *dcm, id _Nullable locale);

NS_ASSUME_NONNULL_END
