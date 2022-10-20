/*	NSXMLDTD.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSXMLNode.h>

@class NSArray<ObjectType>, NSData, NSMutableDictionary;
@class NSXMLDTDNode;

NS_ASSUME_NONNULL_BEGIN

/*!
    @class NSXMLDTD
    @abstract Defines the order, repetition, and allowable values for a document
*/
@interface NSXMLDTD : NSXMLNode {
@private
	NSString *_name;
	NSString *_publicID;
	NSString *_systemID;
	NSArray *_children;
	BOOL _childrenHaveMutated;
	uint8_t _padding3[3];
	NSMutableDictionary *_entities;
	NSMutableDictionary *_elements;
	NSMutableDictionary *_notations;
	NSMutableDictionary *_attributes;
	NSString *_original;
	BOOL _modified;
	uint8_t _padding2[3];
}

#if 0
#pragma mark --- Properties ---
#endif

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithKind:(NSXMLNodeKind)kind options:(NSXMLNodeOptions)options API_UNAVAILABLE(macos, ios, watchos, tvos);
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url options:(NSXMLNodeOptions)mask error:(NSError **)error;
- (nullable instancetype)initWithData:(NSData *)data options:(NSXMLNodeOptions)mask error:(NSError **)error NS_DESIGNATED_INITIALIZER; //primitive

/*!
    @abstract Sets the public id. This identifier should be in the default catalog in /etc/xml/catalog or in a path specified by the environment variable XML_CATALOG_FILES. When the public id is set the system id must also be set.
*/
@property (nullable, copy) NSString *publicID; //primitive

/*!
    @abstract Sets the system id. This should be a URL that points to a valid DTD.
*/
@property (nullable, copy) NSString *systemID; //primitive

#if 0
#pragma mark --- Children ---
#endif

/*!
    @method insertChild:atIndex:
    @abstract Inserts a child at a particular index.
*/
- (void)insertChild:(NSXMLNode *)child atIndex:(NSUInteger)index; //primitive

/*!
    @method insertChildren:atIndex:
    @abstract Insert several children at a particular index.
*/
- (void)insertChildren:(NSArray<NSXMLNode *> *)children atIndex:(NSUInteger)index;

/*!
    @method removeChildAtIndex:
    @abstract Removes a child at a particular index.
*/
- (void)removeChildAtIndex:(NSUInteger)index; //primitive

/*!
    @method setChildren:
    @abstract Removes all existing children and replaces them with the new children. Set children to nil to simply remove all children.
*/
- (void)setChildren:(nullable NSArray<NSXMLNode *> *)children; //primitive

/*!
    @method addChild:
    @abstract Adds a child to the end of the existing children.
*/
- (void)addChild:(NSXMLNode *)child;

/*!
    @method replaceChildAtIndex:withNode:
    @abstract Replaces a child at a particular index with another child.
*/
- (void)replaceChildAtIndex:(NSUInteger)index withNode:(NSXMLNode *)node;

#if 0
#pragma mark --- Accessors ---
#endif

/*!
    @method entityDeclarationForName:
    @abstract Returns the entity declaration matching this name.
*/
- (nullable NSXMLDTDNode *)entityDeclarationForName:(NSString *)name; //primitive

/*!
    @method notationDeclarationForName:
    @abstract Returns the notation declaration matching this name.
*/
- (nullable NSXMLDTDNode *)notationDeclarationForName:(NSString *)name; //primitive

/*!
    @method elementDeclarationForName:
    @abstract Returns the element declaration matching this name.
*/
- (nullable NSXMLDTDNode *)elementDeclarationForName:(NSString *)name; //primitive

/*!
    @method attributeDeclarationForName:
    @abstract Returns the attribute declaration matching this name.
*/
- (nullable NSXMLDTDNode *)attributeDeclarationForName:(NSString *)name elementName:(NSString *)elementName; //primitive

/*!
    @method predefinedEntityDeclarationForName:
    @abstract Returns the predefined entity declaration matching this name.
	@discussion The five predefined entities are
	<ul><li>&amp;lt; - &lt;</li><li>&amp;gt; - &gt;</li><li>&amp;amp; - &amp;</li><li>&amp;quot; - &quot;</li><li>&amp;apos; - &amp;</li></ul>
*/
+ (nullable NSXMLDTDNode *)predefinedEntityDeclarationForName:(NSString *)name;
@end

NS_ASSUME_NONNULL_END
