/*	NSXMLNode.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSXMLNodeOptions.h>

@class NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSError, NSString, NSURL;
@class NSXMLElement, NSXMLDocument;

NS_ASSUME_NONNULL_BEGIN

/*!
    @typedef NSXMLNodeKind
*/
typedef NS_ENUM(NSUInteger, NSXMLNodeKind) {
	NSXMLInvalidKind = 0,
	NSXMLDocumentKind,
	NSXMLElementKind,
	NSXMLAttributeKind,
	NSXMLNamespaceKind,
	NSXMLProcessingInstructionKind,
	NSXMLCommentKind,
	NSXMLTextKind,
	NSXMLDTDKind NS_SWIFT_NAME(DTDKind),
	NSXMLEntityDeclarationKind,
	NSXMLAttributeDeclarationKind,
	NSXMLElementDeclarationKind,
	NSXMLNotationDeclarationKind
};

// initWithKind options
//  NSXMLNodeOptionsNone
//  NSXMLNodePreserveAll
//  NSXMLNodePreserveNamespaceOrder
//  NSXMLNodePreserveAttributeOrder
//  NSXMLNodePreserveEntities
//  NSXMLNodePreservePrefixes
//  NSXMLNodeIsCDATA
//  NSXMLNodeExpandEmptyElement
//  NSXMLNodeCompactEmptyElement
//  NSXMLNodeUseSingleQuotes
//  NSXMLNodeUseDoubleQuotes

// Output options
//  NSXMLNodePrettyPrint

/*!
    @class NSXMLNode
    @abstract The basic unit of an XML document.
*/

@interface NSXMLNode : NSObject <NSCopying> {
@protected
#if __LP64__
    NSXMLNode *_parent;
    id _objectValue;
    NSXMLNodeKind _kind:4;
    uint32_t     _index:28;
@private
    int32_t _private;
#else
    NSXMLNodeKind _kind;
    NSXMLNode *_parent;
    uint32_t _index;
    id _objectValue;
#endif
}

- (instancetype)init NS_DESIGNATED_INITIALIZER;

/*!
    @method initWithKind:
    @abstract Invokes @link initWithKind:options: @/link with options set to NSXMLNodeOptionsNone
*/
- (instancetype)initWithKind:(NSXMLNodeKind)kind;

/*!
    @method initWithKind:options:
    @abstract Inits a node with fidelity options as description NSXMLNodeOptions.h
*/
- (instancetype)initWithKind:(NSXMLNodeKind)kind options:(NSXMLNodeOptions)options NS_DESIGNATED_INITIALIZER; //primitive

/*!
    @method document:
    @abstract Returns an empty document.
*/
+ (id)document;

/*!
    @method documentWithRootElement:
    @abstract Returns a document
    @param element The document's root node.
*/
+ (id)documentWithRootElement:(NSXMLElement *)element;

/*!
    @method elementWithName:
    @abstract Returns an element <tt>&lt;name>&lt;/name></tt>.
*/
+ (id)elementWithName:(NSString *)name;

/*!
    @method elementWithName:URI:
    @abstract Returns an element whose full QName is specified.
*/
+ (id)elementWithName:(NSString *)name URI:(NSString *)URI;

/*!
    @method elementWithName:stringValue:
    @abstract Returns an element with a single text node child <tt>&lt;name>string&lt;/name></tt>.
*/
+ (id)elementWithName:(NSString *)name stringValue:(NSString *)string;

/*!
    @method elementWithName:children:attributes:
    @abstract Returns an element children and attributes <tt>&lt;name attr1="foo" attr2="bar">&lt;-- child1 -->child2&lt;/name></tt>.
*/
+ (id)elementWithName:(NSString *)name children:(nullable NSArray<NSXMLNode *> *)children attributes:(nullable NSArray<NSXMLNode *> *)attributes;

/*!
    @method attributeWithName:stringValue:
    @abstract Returns an attribute <tt>name="stringValue"</tt>.
*/
+ (id)attributeWithName:(NSString *)name stringValue:(NSString *)stringValue;

/*!
    @method attributeWithLocalName:URI:stringValue:
    @abstract Returns an attribute whose full QName is specified.
*/
+ (id)attributeWithName:(NSString *)name URI:(NSString *)URI stringValue:(NSString *)stringValue;

/*!
    @method namespaceWithName:stringValue:
    @abstract Returns a namespace <tt>xmlns:name="stringValue"</tt>.
*/
+ (id)namespaceWithName:(NSString *)name stringValue:(NSString *)stringValue;

/*!
    @method processingInstructionWithName:stringValue:
    @abstract Returns a processing instruction <tt>&lt;?name stringValue></tt>.
*/
+ (id)processingInstructionWithName:(NSString *)name stringValue:(NSString *)stringValue;

/*!
    @method commentWithStringValue:
    @abstract Returns a comment <tt>&lt;--stringValue--></tt>.
*/
+ (id)commentWithStringValue:(NSString *)stringValue;

/*!
    @method textWithStringValue:
    @abstract Returns a text node.
*/
+ (id)textWithStringValue:(NSString *)stringValue;

/*!
    @method DTDNodeWithXMLString:
    @abstract Returns an element, attribute, entity, or notation DTD node based on the full XML string.
*/
+ (nullable id)DTDNodeWithXMLString:(NSString *)string;

#if 0
#pragma mark --- Properties ---
#endif

/*!
    @abstract Returns an element, attribute, entity, or notation DTD node based on the full XML string.
*/
@property (readonly) NSXMLNodeKind kind; //primitive

/*!
    @abstract Sets the nodes name. Applicable for element, attribute, namespace, processing-instruction, document type declaration, element declaration, attribute declaration, entity declaration, and notation declaration.
*/
@property (nullable, copy) NSString *name; //primitive

/*!
    @abstract Sets the content of the node. Setting the objectValue removes all existing children including processing instructions and comments. Setting the object value on an element creates a single text node child.
*/
@property (nullable, retain) id objectValue; //primitive

/*!
    @abstract Sets the content of the node. Setting the stringValue removes all existing children including processing instructions and comments. Setting the string value on an element creates a single text node child. The getter returns the string value of the node, which may be either its content or child text nodes, depending on the type of node. Elements are recursed and text nodes concatenated in document order with no intervening spaces.
*/
@property (nullable, copy) NSString *stringValue; //primitive

/*!
    @method setStringValue:resolvingEntities:
    @abstract Sets the content as with @link setStringValue: @/link, but when "resolve" is true, character references, predefined entities and user entities available in the document's dtd are resolved. Entities not available in the dtd remain in their entity form.
*/
- (void)setStringValue:(NSString *)string resolvingEntities:(BOOL)resolve; //primitive

#if 0
#pragma mark --- Tree Navigation ---
#endif

/*!
    @abstract A node's index amongst its siblings.
*/
@property (readonly) NSUInteger index; //primitive

/*!
    @abstract The depth of the node within the tree. Documents and standalone nodes are level 0.
*/
@property (readonly) NSUInteger level;

/*!
    @abstract The encompassing document or nil.
*/
@property (nullable, readonly, retain) NSXMLDocument *rootDocument;

/*!
    @abstract The parent of this node. Documents and standalone Nodes have a nil parent; there is not a 1-to-1 relationship between parent and children, eg a namespace cannot be a child but has a parent element.
*/
@property (nullable, readonly, copy) NSXMLNode *parent; //primitive

/*!
    @abstract The amount of children, relevant for documents, elements, and document type declarations. Use this instead of [[self children] count].
*/
@property (readonly) NSUInteger childCount; //primitive

/*!
    @abstract An immutable array of child nodes. Relevant for documents, elements, and document type declarations.
*/
@property (nullable, readonly, copy) NSArray<NSXMLNode *> *children; //primitive

/*!
    @method childAtIndex:
    @abstract Returns the child node at a particular index.
*/
- (nullable NSXMLNode *)childAtIndex:(NSUInteger)index; //primitive

/*!
    @abstract Returns the previous sibling, or nil if there isn't one.
*/
@property (nullable, readonly, copy) NSXMLNode *previousSibling;

/*!
    @abstract Returns the next sibling, or nil if there isn't one.
*/
@property (nullable, readonly, copy) NSXMLNode *nextSibling;

/*!
    @abstract Returns the previous node in document order. This can be used to walk the tree backwards.
*/
@property (nullable, readonly, copy) NSXMLNode *previousNode;

/*!
    @abstract Returns the next node in document order. This can be used to walk the tree forwards.
*/
@property (nullable, readonly, copy) NSXMLNode *nextNode;

/*!
    @method detach:
    @abstract Detaches this node from its parent.
*/
- (void)detach; //primitive

/*!
    @abstract Returns the XPath to this node, for example foo/bar[2]/baz.
*/
@property (nullable, readonly, copy) NSString *XPath;

#if 0
#pragma mark --- QNames ---
#endif

/*!
	@abstract Returns the local name bar if this attribute or element's name is foo:bar
*/
@property (nullable, readonly, copy) NSString *localName; //primitive

/*!
	@abstract Returns the prefix foo if this attribute or element's name if foo:bar
*/
@property (nullable, readonly, copy) NSString *prefix; //primitive

/*!
	@abstract Set the URI of this element, attribute, or document. For documents it is the URI of document origin. Getter returns the URI of this element, attribute, or document. For documents it is the URI of document origin and is automatically set when using initWithContentsOfURL.
*/
@property (nullable, copy) NSString *URI; //primitive

/*!
    @method localNameForName:
    @abstract Returns the local name bar in foo:bar.
*/
+ (NSString *)localNameForName:(NSString *)name;

/*!
    @method localNameForName:
    @abstract Returns the prefix foo in the name foo:bar.
*/
+ (nullable NSString *)prefixForName:(NSString *)name;


/*!
    @method predefinedNamespaceForPrefix:
    @abstract Returns the namespace belonging to one of the predefined namespaces xml, xs, or xsi
*/
+ (nullable NSXMLNode *)predefinedNamespaceForPrefix:(NSString *)name;

#if 0
#pragma mark --- Output ---
#endif

/*!
    @abstract Used for debugging. May give more information than XMLString.
*/
@property (readonly, copy) NSString *description;

/*!
    @abstract The representation of this node as it would appear in an XML document.
*/
@property (readonly, copy) NSString *XMLString;

/*!
    @method XMLStringWithOptions:
    @abstract The representation of this node as it would appear in an XML document, with various output options available.
*/
- (NSString *)XMLStringWithOptions:(NSXMLNodeOptions)options;

/*!
    @method canonicalXMLStringPreservingComments:
    @abstract W3 canonical form (http://www.w3.org/TR/xml-c14n). The input option NSXMLNodePreserveWhitespace should be set for true canonical form.
*/
- (NSString *)canonicalXMLStringPreservingComments:(BOOL)comments;

#if 0
#pragma mark --- XPath/XQuery ---
#endif

/*!
    @method nodesForXPath:error:
    @abstract Returns the nodes resulting from applying an XPath to this node using the node as the context item ("."). normalizeAdjacentTextNodesPreservingCDATA:NO should be called if there are adjacent text nodes since they are not allowed under the XPath/XQuery Data Model.
	@returns An array whose elements are a kind of NSXMLNode.
*/
- (nullable NSArray<__kindof NSXMLNode *> *)nodesForXPath:(NSString *)xpath error:(NSError **)error;

/*!
    @method objectsForXQuery:constants:error:
    @abstract Returns the objects resulting from applying an XQuery to this node using the node as the context item ("."). Constants are a name-value dictionary for constants declared "external" in the query. normalizeAdjacentTextNodesPreservingCDATA:NO should be called if there are adjacent text nodes since they are not allowed under the XPath/XQuery Data Model.
	@returns An array whose elements are kinds of NSArray, NSData, NSDate, NSNumber, NSString, NSURL, or NSXMLNode.
*/
- (nullable NSArray *)objectsForXQuery:(NSString *)xquery constants:(nullable NSDictionary<NSString *, id> *)constants error:(NSError **)error;

- (nullable NSArray *)objectsForXQuery:(NSString *)xquery error:(NSError **)error;
@end

NS_ASSUME_NONNULL_END
