/*
        NSTreeNode.h
	AppKit
	Copyright (c) 2007-2021, Apple Inc.
	All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSIndexPath, NSSortDescriptor, NSTreeController;

API_AVAILABLE(macos(10.5))
@interface NSTreeNode : NSObject

+ (instancetype)treeNodeWithRepresentedObject:(nullable id)modelObject;
- (instancetype)initWithRepresentedObject:(nullable id)modelObject;

@property (nullable, readonly, strong) id representedObject;

@property (readonly, strong) NSIndexPath *indexPath; // represents the receiver's location in the entire tree
@property (getter=isLeaf, readonly) BOOL leaf; // determined by (count of child nodes == 0)
    
@property (nullable, readonly, copy) NSArray<NSTreeNode *> *childNodes; // traversal
@property (readonly, strong) NSMutableArray<NSTreeNode *> *mutableChildNodes; // returns a mutable proxy - parentNode of inserted/removed child nodes is automatically updated
- (nullable NSTreeNode *)descendantNodeAtIndexPath:(NSIndexPath *)indexPath; // traversal begins with receiver

@property (nullable, readonly, weak) NSTreeNode *parentNode;

// sorts the entire subtree
- (void)sortWithSortDescriptors:(NSArray<NSSortDescriptor *> *)sortDescriptors recursively:(BOOL)recursively;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

