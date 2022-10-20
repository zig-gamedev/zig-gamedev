/*	NSPredicate.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSOrderedSet.h>

@class NSDictionary <KeyType, ObjectType>, NSString;

NS_ASSUME_NONNULL_BEGIN

// Predicates wrap some combination of expressions and operators and when evaluated return a BOOL.

API_AVAILABLE(macos(10.4), ios(3.0), watchos(2.0), tvos(9.0))
@interface NSPredicate : NSObject <NSSecureCoding, NSCopying> {
    struct _predicateFlags {
        unsigned int _evaluationBlocked:1;
        unsigned int _reservedPredicateFlags:31;
    } _predicateFlags;
#ifdef __LP64__
    uint32_t reserved;
#endif
}

// Parse predicateFormat and return an appropriate predicate
+ (NSPredicate *)predicateWithFormat:(NSString *)predicateFormat argumentArray:(nullable NSArray *)arguments;
+ (NSPredicate *)predicateWithFormat:(NSString *)predicateFormat, ...;
+ (NSPredicate *)predicateWithFormat:(NSString *)predicateFormat arguments:(va_list)argList;

+ (nullable NSPredicate *)predicateFromMetadataQueryString:(NSString *)queryString API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos);

+ (NSPredicate *)predicateWithValue:(BOOL)value;    // return predicates that always evaluate to true/false

+ (NSPredicate*)predicateWithBlock:(BOOL (^)(id _Nullable evaluatedObject, NSDictionary<NSString *, id> * _Nullable bindings))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@property (readonly, copy) NSString *predicateFormat;    // returns the format string of the predicate

- (instancetype)predicateWithSubstitutionVariables:(NSDictionary<NSString *, id> *)variables;    // substitute constant values for variables

- (BOOL)evaluateWithObject:(nullable id)object;    // evaluate a predicate against a single object

- (BOOL)evaluateWithObject:(nullable id)object substitutionVariables:(nullable NSDictionary<NSString *, id> *)bindings API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0)); // single pass evaluation substituting variables from the bindings dictionary for any variable expressions encountered

- (void)allowEvaluation API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // Force a predicate which was securely decoded to allow evaluation

@end

@interface NSArray<ObjectType> (NSPredicateSupport)
- (NSArray<ObjectType> *)filteredArrayUsingPredicate:(NSPredicate *)predicate;    // evaluate a predicate against an array of objects and return a filtered array
@end

@interface NSMutableArray<ObjectType> (NSPredicateSupport)
- (void)filterUsingPredicate:(NSPredicate *)predicate;    // evaluate a predicate against an array of objects and filter the mutable array directly
@end


@interface NSSet<ObjectType> (NSPredicateSupport)
- (NSSet<ObjectType> *)filteredSetUsingPredicate:(NSPredicate *)predicate API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));    // evaluate a predicate against a set of objects and return a filtered set
@end

@interface NSMutableSet<ObjectType> (NSPredicateSupport)
- (void)filterUsingPredicate:(NSPredicate *)predicate API_AVAILABLE(macos(10.5), ios(3.0), watchos(2.0), tvos(9.0));    // evaluate a predicate against a set of objects and filter the mutable set directly
@end

@interface NSOrderedSet<ObjectType> (NSPredicateSupport)

- (NSOrderedSet<ObjectType> *)filteredOrderedSetUsingPredicate:(NSPredicate *)p API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));    // evaluate a predicate against an ordered set of objects and return a filtered ordered set

@end

@interface NSMutableOrderedSet<ObjectType> (NSPredicateSupport)

- (void)filterUsingPredicate:(NSPredicate *)p API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));  // evaluate a predicate against an ordered set of objects and filter the mutable ordered set directly

@end

NS_ASSUME_NONNULL_END
