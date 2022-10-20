/*
	NSTreeController.h
	Application Kit
	Copyright (c) 2003-2021, Apple Inc.
	All rights reserved.
 */

#import <AppKit/NSObjectController.h>
#import <AppKit/AppKitDefines.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSIndexPath.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSSortDescriptor, NSTreeNode;


@interface NSTreeController : NSObjectController

- (void)rearrangeObjects; // triggers rearranging the content objects for the user interface, including sorting (and filtering if provided by subclasses); subclasses can invoke this method if any parameter that affects the arranged objects changes

@property (readonly, strong) NSTreeNode *arrangedObjects; // proxy for the root tree node
@property (nullable, copy) NSString *childrenKeyPath; // key used to find the children of a model object.
@property (nullable, copy) NSString *countKeyPath; // optional for performance
@property (nullable, copy) NSString *leafKeyPath; // optional. inserting/adding children disabled for leaf nodes

@property (copy) NSArray<NSSortDescriptor *> *sortDescriptors;

@property (nullable, strong) id content;

- (IBAction)add:(nullable id)sender;    // adds a new sibling node to the end of the selected objects
- (IBAction)remove:(nullable id)sender; 	//removes the currently selected objects from the tree
- (IBAction)addChild:(nullable id)sender;    // adds a new child node to the end of the selected objects
- (IBAction)insert:(nullable id)sender;    // inserts a peer in front of first selected node
- (IBAction)insertChild:(nullable id)sender;    // inserts a new first child into the children array of the first selected node

@property (readonly) BOOL canInsert;
@property (readonly) BOOL canInsertChild;
@property (readonly) BOOL canAddChild;

- (void)insertObject:(nullable id)object atArrangedObjectIndexPath:(NSIndexPath *)indexPath;
- (void)insertObjects:(NSArray *)objects atArrangedObjectIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;
- (void)removeObjectAtArrangedObjectIndexPath:(NSIndexPath *)indexPath;
- (void)removeObjectsAtArrangedObjectIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;

	// functionality here is parallel to what is in array controller
@property BOOL avoidsEmptySelection;   // default: YES
@property BOOL preservesSelection;   // default: YES
@property BOOL selectsInsertedObjects;   // default: YES
@property BOOL alwaysUsesMultipleValuesMarker;

	/* All selection modification methods returning a BOOL indicate through that flag whether changing the selection was successful (changing the selection might trigger a commitEditing call which fails and thus deny's the selection change).
	*/
@property (readonly, copy) NSArray *selectedObjects;

- (BOOL)setSelectionIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;
@property (readonly, copy) NSArray<NSIndexPath *> *selectionIndexPaths;
- (BOOL)setSelectionIndexPath:(nullable NSIndexPath *)indexPath;
@property (nullable, readonly, copy) NSIndexPath *selectionIndexPath;
- (BOOL)addSelectionIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;
- (BOOL)removeSelectionIndexPaths:(NSArray<NSIndexPath *> *)indexPaths;

@property (readonly, copy) NSArray<NSTreeNode *> *selectedNodes API_AVAILABLE(macos(10.5));

- (void)moveNode:(NSTreeNode *)node toIndexPath:(NSIndexPath *)indexPath API_AVAILABLE(macos(10.5));
- (void)moveNodes:(NSArray<NSTreeNode *> *)nodes toIndexPath:(NSIndexPath *)startingIndexPath API_AVAILABLE(macos(10.5));

- (nullable NSString *)childrenKeyPathForNode:(NSTreeNode *)node API_AVAILABLE(macos(10.5));
- (nullable NSString *)countKeyPathForNode:(NSTreeNode *)node API_AVAILABLE(macos(10.5));
- (nullable NSString *)leafKeyPathForNode:(NSTreeNode *)node API_AVAILABLE(macos(10.5));


@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

