/*	NSComparisonPredicate.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSPredicate.h>

NS_ASSUME_NONNULL_BEGIN

// Flags(s) that can be passed to the factory to indicate that a operator operating on strings should do so in a case insensitive fashion.
typedef NS_OPTIONS(NSUInteger, NSComparisonPredicateOptions) {
    NSCaseInsensitivePredicateOption = 0x01,
    NSDiacriticInsensitivePredicateOption = 0x02,
    NSNormalizedPredicateOption API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 0x04, /* Indicate that the strings to be compared have been preprocessed; this supersedes other options and is intended as a performance optimization option */
};

// Describes how the operator is modified: can be direct, ALL, or ANY
typedef NS_ENUM(NSUInteger, NSComparisonPredicateModifier) {
    NSDirectPredicateModifier = 0, // Do a direct comparison
    NSAllPredicateModifier, // ALL toMany.x = y
    NSAnyPredicateModifier // ANY toMany.x = y
};


// Type basic set of operators defined. Most are obvious; NSCustomSelectorPredicateOperatorType allows a developer to create an operator which uses the custom selector specified in the constructor to do the evaluation.
typedef NS_ENUM(NSUInteger, NSPredicateOperatorType) {
    NSLessThanPredicateOperatorType = 0, // compare: returns NSOrderedAscending
    NSLessThanOrEqualToPredicateOperatorType, // compare: returns NSOrderedAscending || NSOrderedSame
    NSGreaterThanPredicateOperatorType, // compare: returns NSOrderedDescending
    NSGreaterThanOrEqualToPredicateOperatorType, // compare: returns NSOrderedDescending || NSOrderedSame
    NSEqualToPredicateOperatorType, // isEqual: returns true
    NSNotEqualToPredicateOperatorType, // isEqual: returns false
    NSMatchesPredicateOperatorType,
    NSLikePredicateOperatorType,
    NSBeginsWithPredicateOperatorType,
    NSEndsWithPredicateOperatorType,
    NSInPredicateOperatorType, // rhs contains lhs returns true
    NSCustomSelectorPredicateOperatorType,
    NSContainsPredicateOperatorType API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)) = 99, // lhs contains rhs returns true
    NSBetweenPredicateOperatorType API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0))
};

@class NSPredicateOperator;
@class NSExpression;

// Comparison predicates are predicates which do some form of comparison between the results of two expressions and return a BOOL. They take an operator, a left expression, and a right expression, and return the result of invoking the operator with the results of evaluating the expressions.

API_AVAILABLE(macos(10.4), ios(3.0), watchos(2.0), tvos(9.0))
@interface NSComparisonPredicate : NSPredicate {
    @private
    void *_reserved2;
    NSPredicateOperator *_predicateOperator;
    NSExpression *_lhs;
    NSExpression *_rhs;
}

+ (NSComparisonPredicate *)predicateWithLeftExpression:(NSExpression *)lhs rightExpression:(NSExpression *)rhs modifier:(NSComparisonPredicateModifier)modifier type:(NSPredicateOperatorType)type options:(NSComparisonPredicateOptions)options;
+ (NSComparisonPredicate *)predicateWithLeftExpression:(NSExpression *)lhs rightExpression:(NSExpression *)rhs customSelector:(SEL)selector;

- (instancetype)initWithLeftExpression:(NSExpression *)lhs rightExpression:(NSExpression *)rhs modifier:(NSComparisonPredicateModifier)modifier type:(NSPredicateOperatorType)type options:(NSComparisonPredicateOptions)options NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithLeftExpression:(NSExpression *)lhs rightExpression:(NSExpression *)rhs customSelector:(SEL)selector NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (readonly) NSPredicateOperatorType predicateOperatorType;
@property (readonly) NSComparisonPredicateModifier comparisonPredicateModifier;
@property (readonly, retain) NSExpression *leftExpression;
@property (readonly, retain) NSExpression *rightExpression;
@property (nullable, readonly) SEL customSelector;
@property (readonly) NSComparisonPredicateOptions options;

@end

NS_ASSUME_NONNULL_END
