/*
        NSPredicateEditorRowTemplate.h
	Application Kit
	Copyright (c) 2006-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSComparisonPredicate.h>
#import <CoreData/NSAttributeDescription.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSPredicate, NSEntityDescription, NSView;

API_AVAILABLE(macos(10.5))
@interface NSPredicateEditorRowTemplate : NSObject <NSCoding, NSCopying>

/* returns a positive number if the template can represent the predicate, and zero if it cannot.  The highest match determines which template is responsible for displaying the predicate.  Developers can override this to determine which predicates their custom template handles.  By default, this returns values in the range [0., 1.]
*/
- (double)matchForPredicate:(NSPredicate *)predicate;

/* returns the list of views that are placed in the row.  NSPopUpButtons are treated specially in that the items of multiple templates are merged together; other views are added as-is.  Developers can override this to return views in addition to or instead of the default views.
*/
@property (readonly, copy) NSArray<NSView *> *templateViews;

/* sets the value of the views according to the given predicate.  This is only called if matchForPredicate: returns a positive value for the given predicate.  Developers can override this to set the values of their custom views.
*/
- (void)setPredicate:(NSPredicate *)predicate;

/* returns the predicate represented by the template's views' values and the given subpredicates.  Developers can override this to return the predicate represented by their custom views. */
- (NSPredicate *)predicateWithSubpredicates:(nullable NSArray<NSPredicate *> *)subpredicates;

/* for a given predicate, returns the subpredicates that should be made subrows.  For NSCompoundPredicate, this returns the array of subpredicates; it returns nil for other types of predicates.  This is only called if matchForPredicate: returns a positive value for the given predicate.  Developers can override this to create custom templates that handle complicated compound predicates.
*/ 
- (nullable NSArray<NSPredicate *> *)displayableSubpredicatesOfPredicate:(NSPredicate *)predicate;

/* creates a template of the popup-popup-popup form, with the left and right popups representing the left and right expression arrays, and the center popup representing the operators.
*/
- (instancetype)initWithLeftExpressions:(NSArray<NSExpression *> *)leftExpressions rightExpressions:(NSArray<NSExpression *> *)rightExpressions modifier:(NSComparisonPredicateModifier)modifier operators:(NSArray<NSNumber *> *)operators options:(NSUInteger)options;

/* creates a template of the popup-popup-view form, with the left popups representing the left expressions, the right view representing an arbitrary value, and the center popup representing the operators.
*/
- (instancetype)initWithLeftExpressions:(NSArray<NSExpression *> *)leftExpressions rightExpressionAttributeType:(NSAttributeType)attributeType modifier:(NSComparisonPredicateModifier)modifier operators:(NSArray<NSNumber *> *)operators options:(NSUInteger)options;

/* creates a template suitable for displaying compound predicates.  NSPredicateEditor contains such a template by default.
*/
- (instancetype)initWithCompoundTypes:(NSArray<NSNumber *> *)compoundTypes;

/* returns the various values set in the initializers, or zero/nil if they do not apply
*/
@property (nullable, readonly, copy) NSArray<NSExpression *> *leftExpressions;
@property (nullable, readonly, copy) NSArray<NSExpression *> *rightExpressions;
@property (readonly) NSAttributeType rightExpressionAttributeType;
@property (readonly) NSComparisonPredicateModifier modifier;
@property (nullable, readonly, copy) NSArray<NSNumber *> *operators;
@property (readonly) NSUInteger options;
@property (nullable, readonly, copy) NSArray<NSNumber *> *compoundTypes;

/* CoreData convenience method: creates an array of templates of the popup-popup-view variety from the given key paths.  The key paths may cross relationships but must terminate in attributes.
*/
+ (NSArray<NSPredicateEditorRowTemplate *> *)templatesWithAttributeKeyPaths:(NSArray<NSString *> *)keyPaths inEntityDescription:(NSEntityDescription *)entityDescription;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

