/*	NSExpression.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString;
@class NSArray<ObjectType>;
@class NSMutableDictionary;
@class NSPredicate;

NS_ASSUME_NONNULL_BEGIN

// Expressions are the core of the predicate implementation. When expressionValueWithObject: is called, the expression is evaluated, and a value returned which can then be handled by an operator. Expressions can be anything from constants to method invocations. Scalars should be wrapped in appropriate NSValue classes.

typedef NS_ENUM(NSUInteger, NSExpressionType) {
    NSConstantValueExpressionType = 0, // Expression that always returns the same value
    NSEvaluatedObjectExpressionType, // Expression that always returns the parameter object itself
    NSVariableExpressionType, // Expression that always returns whatever is stored at 'variable' in the bindings dictionary
    NSKeyPathExpressionType, // Expression that returns something that can be used as a key path
    NSFunctionExpressionType, // Expression that returns the result of evaluating a symbol
    NSUnionSetExpressionType API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)), // Expression that returns the result of doing a unionSet: on two expressions that evaluate to flat collections (arrays or sets)
    NSIntersectSetExpressionType API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)), // Expression that returns the result of doing an intersectSet: on two expressions that evaluate to flat collections (arrays or sets)
    NSMinusSetExpressionType API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)), // Expression that returns the result of doing a minusSet: on two expressions that evaluate to flat collections (arrays or sets)
    NSSubqueryExpressionType API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)) = 13,
    NSAggregateExpressionType API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)) = 14,
    NSAnyKeyExpressionType API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) = 15,
    NSBlockExpressionType = 19,
    NSConditionalExpressionType API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 20

};

API_AVAILABLE(macos(10.4), ios(3.0), watchos(2.0), tvos(9.0))
@interface NSExpression : NSObject <NSSecureCoding, NSCopying> {
    @package
    struct _expressionFlags {
        unsigned int _evaluationBlocked:1;
        unsigned int _reservedExpressionFlags:31;
    } _expressionFlags;
#ifdef __LP64__
    uint32_t reserved;
#endif
    NSExpressionType _expressionType;
}

+ (NSExpression *)expressionWithFormat:(NSString *)expressionFormat argumentArray:(NSArray *)arguments API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
+ (NSExpression *)expressionWithFormat:(NSString *)expressionFormat, ...  API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
+ (NSExpression *)expressionWithFormat:(NSString *)expressionFormat arguments:(va_list)argList API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

+ (NSExpression *)expressionForConstantValue:(nullable id)obj;    // Expression that returns a constant value
+ (NSExpression *)expressionForEvaluatedObject;    // Expression that returns the object being evaluated
+ (NSExpression *)expressionForVariable:(NSString *)string;    // Expression that pulls a value from the variable bindings dictionary
+ (NSExpression *)expressionForKeyPath:(NSString *)keyPath;    // Expression that invokes valueForKeyPath with keyPath
+ (NSExpression *)expressionForFunction:(NSString *)name arguments:(NSArray *)parameters;    // Expression that invokes one of the predefined functions. Will throw immediately if the selector is bad; will throw at runtime if the parameters are incorrect.
    // Predefined functions are:
    // name              parameter array contents				returns
	//-------------------------------------------------------------------------------------------------------------------------------------
    // sum:              NSExpression instances representing numbers		NSNumber 
    // count:            NSExpression instances representing numbers		NSNumber 
    // min:              NSExpression instances representing numbers		NSNumber  
    // max:              NSExpression instances representing numbers		NSNumber
    // average:          NSExpression instances representing numbers		NSNumber
    // median:           NSExpression instances representing numbers		NSNumber
    // mode:             NSExpression instances representing numbers		NSArray	    (returned array will contain all occurrences of the mode)
    // stddev:           NSExpression instances representing numbers		NSNumber
    // add:to:           NSExpression instances representing numbers		NSNumber
    // from:subtract:    two NSExpression instances representing numbers	NSNumber
    // multiply:by:      two NSExpression instances representing numbers	NSNumber
    // divide:by:        two NSExpression instances representing numbers	NSNumber
    // modulus:by:       two NSExpression instances representing numbers	NSNumber
    // sqrt:             one NSExpression instance representing numbers		NSNumber
    // log:              one NSExpression instance representing a number	NSNumber
    // ln:               one NSExpression instance representing a number	NSNumber
    // raise:toPower:    one NSExpression instance representing a number	NSNumber
    // exp:              one NSExpression instance representing a number	NSNumber
    // floor:            one NSExpression instance representing a number	NSNumber
    // ceiling:          one NSExpression instance representing a number	NSNumber
    // abs:              one NSExpression instance representing a number	NSNumber
    // trunc:            one NSExpression instance representing a number	NSNumber
    // uppercase:	 one NSExpression instance representing a string	NSString
    // lowercase:        one NSExpression instance representing a string    NSString
    // canonical:        one NSExpression instance representing a string    NSString
    // random            none							NSNumber (integer)
    // randomn:          one NSExpression instance representing a number	NSNumber (integer) such that 0 <= rand < param
    // now               none							[NSDate now]
    // bitwiseAnd:with:	 two NSExpression instances representing numbers	NSNumber    (numbers will be treated as NSInteger)
    // bitwiseOr:with:	 two NSExpression instances representing numbers	NSNumber    (numbers will be treated as NSInteger)
    // bitwiseXor:with:	 two NSExpression instances representing numbers	NSNumber    (numbers will be treated as NSInteger)
    // leftshift:by:	 two NSExpression instances representing numbers	NSNumber    (numbers will be treated as NSInteger)
    // rightshift:by:	 two NSExpression instances representing numbers	NSNumber    (numbers will be treated as NSInteger)
    // onesComplement:	 one NSExpression instance representing a numbers	NSNumber    (numbers will be treated as NSInteger)
    // noindex:		 an NSExpression					parameter   (used by CoreData to indicate that an index should be dropped)
    // distanceToLocation:fromLocation:
    //                   two NSExpression instances representing CLLocations    NSNumber
    // length:           an NSExpression instance representing a string         NSNumber

+ (NSExpression *)expressionForAggregate:(NSArray<NSExpression *> *)subexpressions API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // Expression that returns a collection containing the results of other expressions
+ (NSExpression *)expressionForUnionSet:(NSExpression *)left with:(NSExpression *)right API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // return an expression that will return the union of the collections expressed by left and right
+ (NSExpression *)expressionForIntersectSet:(NSExpression *)left with:(NSExpression *)right API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // return an expression that will return the intersection of the collections expressed by left and right
+ (NSExpression *)expressionForMinusSet:(NSExpression *)left with:(NSExpression *)right API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // return an expression that will return the disjunction of the collections expressed by left and right
+ (NSExpression *)expressionForSubquery:(NSExpression *)expression usingIteratorVariable:(NSString *)variable predicate:(NSPredicate *)predicate API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // Expression that filters a collection by storing elements in the collection in the variable variable and keeping the elements for which qualifer returns true; variable is used as a local variable, and will shadow any instances of variable in the bindings dictionary, the variable is removed or the old value replaced once evaluation completes
+ (NSExpression *)expressionForFunction:(NSExpression *)target selectorName:(NSString *)name arguments:(nullable NSArray *)parameters API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));    // Expression that invokes the selector on target with parameters. Will throw at runtime if target does not implement selector or if parameters are wrong.
+ (NSExpression *)expressionForAnyKey API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
+ (NSExpression *)expressionForBlock:(id (^)(id _Nullable evaluatedObject, NSArray<NSExpression *> *expressions, NSMutableDictionary * _Nullable context))block arguments:(nullable NSArray<NSExpression *> *)arguments API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)); // Expression that invokes the block with the parameters; note that block expressions are not encodable or representable as parseable strings.
+ (NSExpression *)expressionForConditional:(NSPredicate *)predicate trueExpression:(NSExpression *)trueExpression falseExpression:(NSExpression *)falseExpression  API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)); // Expression that will return the result of trueExpression or falseExpression depending on the value of predicate

- (instancetype)initWithExpressionType:(NSExpressionType)type NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

// accessors for individual parameters - raise if not applicable
@property (readonly) NSExpressionType expressionType;
@property (nullable, readonly, retain) id constantValue;
@property (readonly, copy) NSString *keyPath;
@property (readonly, copy) NSString *function;
@property (readonly, copy) NSString *variable;
@property (readonly, copy) NSExpression *operand;    // the object on which the selector will be invoked (the result of evaluating a key path or one of the defined functions)
@property (nullable, readonly, copy) NSArray<NSExpression *> *arguments;    // array of expressions which will be passed as parameters during invocation of the selector on the operand of a function expression

@property (readonly, retain) id collection API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));
@property (readonly, copy) NSPredicate *predicate API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));
@property (readonly, copy) NSExpression *leftExpression API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // expression which represents the left side of a set expression
@property (readonly, copy) NSExpression *rightExpression API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // expression which represents the right side of a set expression


@property (readonly, copy) NSExpression *trueExpression API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)); // expression which will be evaluated if a conditional expression's predicate evaluates to true
@property (readonly, copy) NSExpression *falseExpression API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)); // expression which will be evaluated if a conditional expression's predicate evaluates to false

@property (readonly, copy) id (^expressionBlock)(id _Nullable, NSArray<NSExpression *> *, NSMutableDictionary * _Nullable) API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

// evaluate the expression using the object and bindings- note that context is mutable here and can be used by expressions to store temporary state for one predicate evaluation
- (nullable id)expressionValueWithObject:(nullable id)object context:(nullable NSMutableDictionary *)context;

- (void)allowEvaluation API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // Force an expression which was securely decoded to allow evaluation

@end

NS_ASSUME_NONNULL_END
