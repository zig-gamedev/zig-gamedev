/*	NSXMLElement.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSXMLNode.h>

@class NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSMutableArray, NSEnumerator, NSString;

NS_ASSUME_NONNULL_BEGIN

/*!
    @class NSXMLElement
    @abstract An XML element
    @discussion Note: Trying to add a document, namespace, attribute, or node with a parent throws an exception. To add a node with a parent first detach or create a copy of it.
*/
@interface NSXMLElement : NSXMLNode {
@protected
	NSString *_name;
	id _attributes;
	id _namespaces;
	NSArray *_children;
	BOOL _childrenHaveMutated;
	BOOL _zeroOrOneAttributes;
	BOOL _zeroOrOneNamespaces;
	uint8_t _padding;
	NSString *_URI;	
	NSInteger _prefixIndex;
}

/*!
    @method initWithName:
    @abstract Returns an element <tt>&lt;name>&lt;/name></tt>.
*/
- (instancetype)initWithName:(NSString *)name;

/*!
    @method initWithName:URI:
    @abstract Returns an element whose full QName is specified.
*/
- (instancetype)initWithName:(NSString *)name URI:(nullable NSString *)URI NS_DESIGNATED_INITIALIZER; //primitive

/*!
    @method initWithName:stringValue:
    @abstract Returns an element with a single text node child <tt>&lt;name>string&lt;/name></tt>.
*/
- (instancetype)initWithName:(NSString *)name stringValue:(nullable NSString *)string;

/*!
    @method initWithXMLString:error:
    @abstract Returns an element created from a string. Parse errors are collected in <tt>error</tt>.
*/
- (nullable instancetype)initWithXMLString:(NSString *)string error:(NSError **)error NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithKind:(NSXMLNodeKind)kind options:(NSXMLNodeOptions)options;

#if 0
#pragma mark --- Elements by name ---
#endif

/*!
    @method elementsForName:
    @abstract Returns all of the child elements that match this name.
*/
- (NSArray<NSXMLElement *> *)elementsForName:(NSString *)name;

/*!
    @method elementsForLocalName:URI
    @abstract Returns all of the child elements that match this localname URI pair.
*/
- (NSArray<NSXMLElement *> *)elementsForLocalName:(NSString *)localName URI:(nullable NSString *)URI;

#if 0
#pragma mark --- Attributes ---
#endif

/*!
    @method addAttribute:
    @abstract Adds an attribute. Attributes with duplicate names are not added.
*/
- (void)addAttribute:(NSXMLNode *)attribute; //primitive

/*!
    @method removeAttributeForName:
    @abstract Removes an attribute based on its name.
*/
- (void)removeAttributeForName:(NSString *)name; //primitive

/*!
    @abstract Set the attributes. In the case of duplicate names, the first attribute with the name is used.
*/
@property (nullable, copy) NSArray<NSXMLNode *> *attributes;

/*!
 @method setAttributesWithDictionary:
 @abstract Set the attributes based on a name-value dictionary.
 */
- (void)setAttributesWithDictionary:(NSDictionary<NSString *, NSString *> *)attributes;

/*!
    @method attributeForName:
    @abstract Returns an attribute matching this name.
*/
- (nullable NSXMLNode *)attributeForName:(NSString *)name;

/*!
    @method attributeForLocalName:URI:
    @abstract Returns an attribute matching this localname URI pair.
*/
- (nullable NSXMLNode *)attributeForLocalName:(NSString *)localName URI:(nullable NSString *)URI; //primitive

#if 0
#pragma mark --- Namespaces ---
#endif

/*!
    @method addNamespace:URI:
    @abstract Adds a namespace. Namespaces with duplicate names are not added.
*/
- (void)addNamespace:(NSXMLNode *)aNamespace; //primitive

/*!
    @method addNamespace:URI:
    @abstract Removes a namespace with a particular name.
*/
- (void)removeNamespaceForPrefix:(NSString *)name; //primitive

/*!
    @abstract Set the namespaces. In the case of duplicate names, the first namespace with the name is used.
*/
@property (nullable, copy) NSArray<NSXMLNode *> *namespaces; //primitive

/*!
    @method namespaceForPrefix:
    @abstract Returns the namespace matching this prefix.
*/
- (nullable NSXMLNode *)namespaceForPrefix:(NSString *)name;

/*!
    @method resolveNamespaceForName:
    @abstract Returns the namespace who matches the prefix of the name given. Looks in the entire namespace chain.
*/
- (nullable NSXMLNode *)resolveNamespaceForName:(NSString *)name;

/*!
    @method resolvePrefixForNamespaceURI:
    @abstract Returns the URI of this prefix. Looks in the entire namespace chain.
*/
- (nullable NSString *)resolvePrefixForNamespaceURI:(NSString *)namespaceURI;

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
    @method removeChildAtIndex:atIndex:
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

/*!
    @method normalizeAdjacentTextNodesPreservingCDATA:
    @abstract Adjacent text nodes are coalesced. If the node's value is the empty string, it is removed. This should be called with a value of NO before using XQuery or XPath.
*/
- (void)normalizeAdjacentTextNodesPreservingCDATA:(BOOL)preserve;

@end

@interface NSXMLElement (NSDeprecated)
/*!
    @method setAttributesAsDictionary:
    @abstract Set the attributes base on a name-value dictionary.
    @discussion This method is deprecated and does not function correctly. Use -setAttributesWithDictionary: instead.
 */
- (void)setAttributesAsDictionary:(NSDictionary *)attributes API_DEPRECATED_WITH_REPLACEMENT("setAttributesWithDictionary:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
@end

NS_ASSUME_NONNULL_END
