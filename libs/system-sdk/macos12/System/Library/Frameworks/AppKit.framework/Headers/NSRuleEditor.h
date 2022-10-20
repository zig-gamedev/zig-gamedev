/*
        NSRuleEditor.h
	Application Kit
	Copyright (c) 2006-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSControl.h>

/* An NSRuleEditor is a class that allows the user to create and configure a list of options.  The view has a delegate which offers a tree of choices to the view.  The choices are presented by the view to the user as a row of popup buttons, static text fields, and custom views.  Each row in the list represents a particular path down the tree of choices.  An example of part of a tree of choices and a path through it:

                 ----| contains        -------> [CUSTOM TEXT FIELD]
Name-------------|   | starts with     |
Kind                 | ends with-------|
Creation date        | is
Modification date

Each node in the tree is represented by a "criterion," which can be any object.  As the user chooses from the popup menus, the rule editor view will query its delegate for the child criteria of the chosen criterion.  The only restriction on the criteria is that two criterions should be considered equal by isEqual: if they represent the same node, and must not be equal if they represent different nodes.  The path through the tree for a given row is represented by the array of criteria for the row.  For example, the criteria for the above selection would represent "Name", "ends with", and the custom text field; the manner in which the criteria represent those values is up to the developer.

There are two types of rows - standard rows and header rows (NSRuleEditorRowTypeSimple and NSRuleEditorRowTypeCompound).  A header row can contain other rows, but a standard row cannot.  Header rows and standard rows can have different trees of criteria.  The amount and style of row nesting is configurable.

Every time a row is created or modified, the rule editor view will query its delegate view for a "display value" for each new criteriion in the row.  The display value is what is presented to the user, and must be a NSString, NSView, or NSMenuItem.  When a criterion is selected in the row, the rule editor will query the criterion for its child criteria and then each of the children for their display value.  If there are multiple children, a popup button is formed from the string values and menu items.  If there is only one child, its display value is displayed as a static text field (if it is a string) or as a view (if the value is a view).

NSRuleEditor has some optional support for predicates.  For each row, it will ask its delegate for various parts of a predicate, which are returned in a dictionary.  Assuming the delegate returns enough parts for each row, the parts are then combined to form a predicate for the entire view.  This is expressed in the  "predicate" property for the view.  Subclassers can modify the predicate for each row as it is constructed.  Setting a predicate back on the view is not supported.

NSRuleEditor exposes one binding, "rows."  The "rows" binding may be bound to an ordered to-many relation (e.g. NSMutableArray).  Each object in the relation should have the following properties:

    @"rowType" -> integer representing the type of the row (NSRuleEditorRowType)
    @"subrows" -> ordered to-many relation (e.g. NSMutableArray) containing the directly nested subrows for the given row
    @"displayValues" -> ordered to-many relation containing the display values for the row
    @"criteria" -> ordered to-many relation containing the criteria for the row
    
    These key paths can be set using the set*KeyPath: methods below
*/

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSIndexSet, NSView, NSPredicate, NSString, NSViewAnimation;
@protocol NSRuleEditorDelegate;

/* The following strings are to be used as keys to the dictionary returned from the optional method ruleEditor: predicatePartsForCriterion: withDisplayValue: inRow: .  In order to construct a valid predicate, the union of the dictionaries for each item in the row must contain the required parts as described below: */
typedef NSString * NSRuleEditorPredicatePartKey NS_TYPED_ENUM;
APPKIT_EXTERN NSRuleEditorPredicatePartKey const NSRuleEditorPredicateLeftExpression; /* The value of this key should be an NSExpression representing the left expression in the predicate; this value is required for a non-nil comparison predicate */
APPKIT_EXTERN NSRuleEditorPredicatePartKey const NSRuleEditorPredicateRightExpression; /* The value of this key should be an NSExpression representing the right expression in the predicate; this value is required for a non-nil comparison predicate */
APPKIT_EXTERN NSRuleEditorPredicatePartKey const NSRuleEditorPredicateComparisonModifier; /* NSNumber representing the NSComparisonPredicateModifier of the predicate; this value is optional and, if not specified, NSDirectPredicateModifier is assumed */
APPKIT_EXTERN NSRuleEditorPredicatePartKey const NSRuleEditorPredicateOptions; /* NSNumber representing NSComparisonPredicateOptions; these are bitwise ORed together when constructing a predicate.  If none is specified, 0 (no options) is assumed. */
APPKIT_EXTERN NSRuleEditorPredicatePartKey const NSRuleEditorPredicateOperatorType; /* NSNumber representing NSPredicateOperatorType; this value is required for a non-nil comparison predicate */
APPKIT_EXTERN NSRuleEditorPredicatePartKey const NSRuleEditorPredicateCustomSelector; /* NSString representing a custom selector; if specified, this will override the operator type, options, and comparison modifier */
APPKIT_EXTERN NSRuleEditorPredicatePartKey const NSRuleEditorPredicateCompoundType; /* NSNumber representing a NSCompoundPredicateType; if specified, the other keys are ignored and the predicate for the row will be an NSCompoundPredicate predicate, whose subpredicates are the predicates of the subrows of the given row. */

typedef NS_ENUM(NSUInteger, NSRuleEditorNestingMode) {
    NSRuleEditorNestingModeSingle,	    /* Only a single row is allowed.  Plus/minus buttons will not be shown */
    NSRuleEditorNestingModeList,	    /* Allows a single list, with no nesting and no compound rows */
    NSRuleEditorNestingModeCompound,	    /* Unlimited nesting and compound rows; this is the default */
    NSRuleEditorNestingModeSimple	    /* One compound row at the top with subrows beneath it, and no further nesting allowed */
};


typedef NS_ENUM(NSUInteger, NSRuleEditorRowType) {
    NSRuleEditorRowTypeSimple,
    NSRuleEditorRowTypeCompound
};


@interface NSRuleEditor : NSControl

/* -- Configuring NSRuleEditor -- */

/* Clients can call this method to set and get the delegate for the NSRuleEditor.  NSRuleEditor requires a delegate that implements the required NSRuleEditorDelegateMethods methods to function. */
@property (nullable, weak) id<NSRuleEditorDelegate> delegate;

/* Clients can call this to automatically set a formatting dictionary based on the strings file with the given name.  Setting a formatting strings file searches the main bundle, and the bundle containing this nib, for a (possibly localized) strings file resource with the given name, loads it, and sets it as the formatting dictionary.  The resulting dictionary can be obtained with -[NSRuleEditor formattingDictionary].  If you set the formatting dictionary explicitly with -[NSRuleEditor setFormattingDictionary:], then it sets the current formattingStringsFilename to nil */
@property (nullable, copy) NSString *formattingStringsFilename;

/* Clients can call this to set (and get) a formatting dictionary on the NSRuleEditor.  The formatting dictionary should have NSString keys and NSString values.  The syntax of the keys and values is the same as the syntax for strings files. */
@property (nullable, copy) NSDictionary<NSString *, NSString *> *formattingDictionary;

/* Clients can call this to indicate that the available criteria may have changed and should be refetched from the delegate, and the popups recalculated.  If any item in a given row is "orphaned" (no longer reported as a child of its previous parent), those rows have their critieria and display values set to valid choices. */
- (void)reloadCriteria;

/* Clients call this to set and get the nesting mode for the NSRuleEditor.  This is generally set at view creation time and not modified after.  The default is NSRuleEditorNestingModeCompound. */
@property NSRuleEditorNestingMode nestingMode;

/* Clients call this to set and get the height of each row.  This method changes the receiver's frame and marks it for redisplay.  */
@property CGFloat rowHeight;

/* Clients call this to set the editable property of the control.  Users can only interact with editable NSRuleEditors.  The default is YES. */
@property (getter=isEditable) BOOL editable;

/* Rule editors that have the canRemoveAllRows property set to NO prevent the user from emptying the rule editor by deleting all the rows.  The rule editor can still be emptied programatically.  The default is YES. */
@property BOOL canRemoveAllRows;

/* -- Predicate support -- */

/* Clients can call this to obtain the predicate for the view if the delegate implements - ruleEditor: predicatePartsForItem: withValue: inRow:row: .  If the delegate does not, or if the delegate does not return enough parts to construct a full predicate, this method returns nil. */
@property (nullable, readonly, strong) NSPredicate *predicate;

/* Clients can call this to indicates that the predicate should be generated again from the delegate (by invoking the corresponding delegate method), typically because something has changed (for example, a view's value). */
- (void)reloadPredicate;

/* This method returns the predicate for a given row.  Clients should rarely have a need to call this directly, but it can be overridden to perform specialized predicate handling for certain criteria or display values. */
- (nullable NSPredicate *)predicateForRow:(NSInteger)row;

/* -- Obtaining row information -- */

/* Clients can call this to determine the number of rows */
@property (readonly) NSInteger numberOfRows;

/* Clients can call this to determine the immediate subrows of the given row.  Pass -1 to get the top-level rows.  Rows are numbered starting at 0.  If rowIndex is less than -1 or greater than or equal to the number of rows, this raises an NSRangeException.  */
- (NSIndexSet *)subrowIndexesForRow:(NSInteger)rowIndex;

/* Clients call this to obtain all of the currently chosen items for the given row.  These are the same items that are returned from the delegate method - ruleEditor: child: forCriterion: withRowType: */
- (NSArray *)criteriaForRow:(NSInteger)row;

/* Clients call this to obtain all of the chosen values (strings, views, or menu items) for row.  These are the same values that are returned from the delegate method - ruleEditor: valueForItem: inRow: */
- (NSArray *)displayValuesForRow:(NSInteger)row;

/* Clients call this to map from a display value (string, view, or menu item) back to a row.  This method searches each row via pointer equality for the given display value, which may be present as an alternative in a popup menu for that row.  It returns the index of the row containing the given value, or NSNotFound.  Raises NSInvalidArgumentException if displayValue is nil. */
- (NSInteger)rowForDisplayValue:(id)displayValue;

/* Clients call this to determine the type of the row.  If rowIndex is less than 0 or greater than or equal to the number of rows, this raises an NSRangeException */
- (NSRuleEditorRowType)rowTypeForRow:(NSInteger)rowIndex;

/* Clients call this to determine the parent row of rowIndex.  If rowIndex is a root row, this returns -1.  If rowIndex is less than 0 or greater than or equal to the number of rows, this raises an NSRangeException */
- (NSInteger)parentRowForRow:(NSInteger)rowIndex;

/* -- Manipulating rows -- */

/* Adds "initial" rows, or a row to the end - useful as the target for the outside + button */
- (void)addRow:(nullable id)sender;

/* Clients call this to add a new row at the given index with the given type as a subrow of the parent row.  Pass -1 to indicate that it should be a root row.  If parentRow >= rowIndex, or if rowIndex would fall amongst the children of some other parent, or if the nesting mode forbids this configuration, an NSInvalidArgumentException is raised. */
- (void)insertRowAtIndex:(NSInteger)rowIndex withType:(NSRuleEditorRowType)rowType asSubrowOfRow:(NSInteger)parentRow animate:(BOOL)shouldAnimate;

/* Clients call this to modify the row at a given index to contain the passed-in items and values.  It is your responsibility to ensure that each item in the array is a child of the previous item, and that the first item is a root item for the row type.  If the last item has child items, then the items array will be extended by querying the delegate for child items until a childless item is reached.  If the values array contains fewer objects than the (possibly extended) criteria array, then the delegate will be queried to construct the remaining display values. If you want the delegate to be queried for all the criteria or all the display values, pass empty arrays; do not pass nil.  Raises NSRangeException if rowIndex is equal to or larger than the number of rows, or less than 0. Raises an NSInvalidArgumentException if criteria or values is nil. */
- (void)setCriteria:(NSArray *)criteria andDisplayValues:(NSArray *)values forRowAtIndex:(NSInteger)rowIndex;

/* Clients call this to remove the row at the given index.  Any subrows of the deleted row are adopted by the parent of the deleted row, or are made root rows. Raises NSRangeException if rowIndex is equal to or larger than the number of rows, or less than 0. */
- (void)removeRowAtIndex:(NSInteger)rowIndex;

/* Clients call this to delete rows at the given indexes.  If includeSubrows is YES, then the subrows of the deleted rows will be deleted as well; if includeSubrows is NO, then each subrow will be adopted by its first non-deleted ancestor, or will become a root row. Raises NSRangeException if any index in rowIndexes is equal to or larger than the number of rows, or less than 0. */
- (void)removeRowsAtIndexes:(NSIndexSet *)rowIndexes includeSubrows:(BOOL)includeSubrows;

/* Clients call this to determine the indexes of the selected rows.  */
@property (readonly, copy) NSIndexSet *selectedRowIndexes;

/* Clients call this to modify the selected row indexes. Raises NSRangeException if any index in rowIndexes is equal to or larger than the number of rows, or less than 0. */
- (void)selectRowIndexes:(NSIndexSet *)indexes byExtendingSelection:(BOOL)extend;

/* -- Bindings support -- */

/* Sets the class used when creating a new row in the "rows" binding; this class should be KVC and KVO compliant for the key paths listed below.  By default this is NSMutableDictionary */
@property (assign) Class rowClass;

/* Set and get the key path for the row type, which is used to get the row type in the "rows" binding.  The row type is a value property of type NSRuleEditorRowType.  The default is @"rowType". */
@property (copy) NSString *rowTypeKeyPath;

/* Set and get the key path for the subrows, which is used to determined nested rows in the "rows" binding.  The subrows property is an ordered to-many relationship containing additional bound row objects. The default is @"subrows". */
@property (copy) NSString *subrowsKeyPath;

/* Set and get the criteria key path, which determines the criteria for a row in the "rows" binding.  (The criteria objects are what the delegate returns from - ruleEditor: child: forCriterion: withRowType:).  The criteria property is an ordered to-many relationship. The default is @"criteria". */
@property (copy) NSString *criteriaKeyPath;

/* Set and get the display values key path, which determines the display values for a row (the display values are what the delegate returns from - ruleEditor: displayValueForCriterion: inRow:).  The criteria property is an ordered to-many relationship. The default is @"displayValues". */
@property (copy) NSString *displayValuesKeyPath;

@end


@protocol NSRuleEditorDelegate <NSObject>

@required

/* -- Required delegate methods -- */

/* When called, you should return the number of child items of the given criterion.  If criterion is nil, you should return the number of root criteria for the given row type. Implementation of this method is required. */
- (NSInteger)ruleEditor:(NSRuleEditor *)editor numberOfChildrenForCriterion:(nullable id)criterion withRowType:(NSRuleEditorRowType)rowType;

/* When called, you should return the child of the given item at the given index.  If criterion is nil, return the root criterion for the given row type at the given index. Implementation of this method is required. */
- (id)ruleEditor:(NSRuleEditor *)editor child:(NSInteger)index forCriterion:(nullable id)criterion withRowType:(NSRuleEditorRowType)rowType;

/* When called, you should return a value for the given criterion.  The value should be an instance of NSString, NSView, or NSMenuItem.  If the value is an NSView or NSMenuItem, you must ensure it is unique for every invocation of this method; that is, do not return a particular instance of NSView or NSMenuItem more than once.  Implementation of this method is required. */
- (id)ruleEditor:(NSRuleEditor *)editor displayValueForCriterion:(id)criterion inRow:(NSInteger)row;

@optional

/* -- Optional delegate methods -- */


/* When called, you should return an NSDictionary representing the parts of the predicate determined by the given criterion and value.  The keys of the dictionary should be the strings shown above that begin with NSRuleEditorPredicate..., and the values should be as described in the comments adjacent to the keys.  Implementation of this method is optional. */
- (nullable NSDictionary<NSRuleEditorPredicatePartKey, id> *)ruleEditor:(NSRuleEditor *)editor predicatePartsForCriterion:(id)criterion withDisplayValue:(id)value inRow:(NSInteger)row;

/* If ruleEditorRowsDidChange: is implemented, NSRuleEditor will automatically register its delegate to receive NSRuleEditorRowsDidChangeNotification notifications to this method. Implementation of this method is optional. */
- (void)ruleEditorRowsDidChange:(NSNotification *)notification;

@end


/* Posted to the default notification center whenever the view's rows change.
 * The object is the NSRuleEditor; there is no userInfo */
APPKIT_EXTERN NSNotificationName const NSRuleEditorRowsDidChangeNotification;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

