/*
    NSPropertyMapping.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>


NS_ASSUME_NONNULL_BEGIN

@class NSExpression;

API_AVAILABLE(macosx(10.5),ios(3.0))
@interface NSPropertyMapping : NSObject {
}

/* Returns/sets the name of the property in the destination entity for the mapping.  
*/
@property (nullable, copy) NSString *name;

/* Returns/sets the value expression for the property mapping.  The expression is used to create the value for the destination property.
*/
@property (nullable, strong) NSExpression *valueExpression;

/* Returns/sets the user info for the property mapping.
*/
@property (nullable, strong) NSDictionary *userInfo;

@end

NS_ASSUME_NONNULL_END
