/*
    NSDerivedAttributeDescription.h
    Core Data
    Copyright (c) 2018-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <CoreData/NSAttributeDescription.h>

@class NSExpression;
@class NSPredicate;

NS_ASSUME_NONNULL_BEGIN

/* Class that describes an attribute whose value should be derived from one or more
 other properties and how that derivation should be done.
 This is primarily intended to optimize fetch performance. Some use cases:
 * creating a derived 'searchName' attribute that reflects a 'name' attribute with
 case and diacritics stripped for more efficient comparisons during fetching
 * creating a 'relationshipCount' attribute reflecting the number of objects in
 a relationship and so avoid having to do a join during fetching
 
 IMPORTANT: Derived attributes will be recomputed during save, recomputed values will not be reflected in a managed object's property until after a save.
 
 NOTE: Prior to macOS 10.16, iOS 14.0, tvOS 14.0, and watchOS 7.0 a refresh of the object is required after a save to reflect recomputed values
 */

API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0))
@interface NSDerivedAttributeDescription : NSAttributeDescription

/* Instance of NSExpression that will be used to generate the derived data.
 When using derived attributes in an SQL store, this expression should be
 * a keypath expression (including @operation components)
 * a function expression using one of the predefined functions defined
 in NSExpression.h
 Any keypaths used in the expression must be accessible from the entity on which
 the derived attribute is specified.
 If a store is added to a coordinator whose model contains derived attributes of
 a type not supported by the store, the add will fail and an NSError will be returned. */
@property (strong, nullable) NSExpression *derivationExpression;

@end

NS_ASSUME_NONNULL_END
