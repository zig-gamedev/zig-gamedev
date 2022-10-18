/*
    NSFetchRequestExpression.h
    Core Data
    Copyright (c) 2006-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSExpression.h>

NS_ASSUME_NONNULL_BEGIN

static const NSExpressionType NSFetchRequestExpressionType = (NSExpressionType)50;

API_AVAILABLE(macosx(10.5),ios(3.0))
@interface NSFetchRequestExpression : NSExpression {
}

/* Returns an expression which will evaluate to the result of executing a fetch request on a context.  The first argument must be an expression which evaluates to an NSFetchRequest *, and the second must be an expression which evaluates to an NSManagedObjectContext *.  If the desired result is simply the count for the request, the "countOnly" argument should be YES.
*/
+ (NSExpression*)expressionForFetch:(NSExpression*)fetch context:(NSExpression*)context countOnly:(BOOL)countFlag;


/* Returns the expression for the fetch request:  evaluating it must return an NSFetchRequest *.
*/
@property (readonly, strong) NSExpression *requestExpression;


/* Returns the expression for the managed object context:  evaluating it must return an NSManagedObjectContext *.
*/
@property (readonly, strong) NSExpression *contextExpression; 


/* Boolean indicating if the request will return object values or merely the count.  If the value returns NO, the managed object context (from the contextExpression) will perform executeFetchRequest:error: with the requestExpression;  if the value returns YES, the managed object context will perform countForFetchRequest:error with the requestExpression.
*/
@property (getter=isCountOnlyRequest, readonly) BOOL countOnlyRequest;

@end

NS_ASSUME_NONNULL_END
