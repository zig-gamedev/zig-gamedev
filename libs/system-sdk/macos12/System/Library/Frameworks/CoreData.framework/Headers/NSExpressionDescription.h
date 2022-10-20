/*
    NSExpressionDescription.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <CoreData/NSPropertyDescription.h>
#import <CoreData/NSAttributeDescription.h>

NS_ASSUME_NONNULL_BEGIN

@class NSExpression;

/* Special property description type intended for use with the NSFetchRequest -propertiesToFetch method.
   An NSExpressionDescription describes a column to be returned from a fetch that may not appear 
   directly as an attribute or relationship on an entity. Examples would be: upper(attribute) or
   max(attribute). NSExpressionDescriptions cannot be set as properties on NSEntityDescription. */
API_AVAILABLE(macosx(10.6),ios(3.0))
@interface NSExpressionDescription : NSPropertyDescription {
}

@property (nullable, strong) NSExpression *expression;

@property () NSAttributeType expressionResultType;

@end

NS_ASSUME_NONNULL_END

