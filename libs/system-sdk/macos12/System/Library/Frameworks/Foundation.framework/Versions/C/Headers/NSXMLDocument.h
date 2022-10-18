/*	NSXMLDocument.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSXMLNode.h>

@class NSData, NSXMLDTD, NSXMLDocument, NSDictionary<KeyType, ObjectType>, NSArray<ObjectType>;

NS_ASSUME_NONNULL_BEGIN

// Input options
//  NSXMLNodeOptionsNone
//  NSXMLNodePreserveAll
//  NSXMLNodePreserveNamespaceOrder
//  NSXMLNodePreserveAttributeOrder
//  NSXMLNodePreserveEntities
//  NSXMLNodePreservePrefixes
//  NSXMLNodePreserveCDATA
//  NSXMLNodePreserveEmptyElements
//  NSXMLNodePreserveQuotes
//  NSXMLNodePreserveWhitespace
//  NSXMLNodeLoadExternalEntities
//  NSXMLNodeLoadExternalEntitiesSameOriginOnly
	
//  NSXMLDocumentTidyHTML
//  NSXMLDocumentTidyXML

//  NSXMLDocumentValidate

// Output options
//  NSXMLNodePrettyPrint
//  NSXMLDocumentIncludeContentTypeDeclaration

/*!
    @typedef NSXMLDocumentContentKind
	@abstract Define what type of document this is.
	@constant NSXMLDocumentXMLKind The default document type
	@constant NSXMLDocumentXHTMLKind Set if NSXMLDocumentTidyHTML is set and HTML is detected
	@constant NSXMLDocumentHTMLKind Outputs empty tags without a close tag, eg <br>
	@constant NSXMLDocumentTextKind Output the string value of the document
*/
typedef NS_ENUM(NSUInteger, NSXMLDocumentContentKind) {
	NSXMLDocumentXMLKind = 0,
	NSXMLDocumentXHTMLKind,
	NSXMLDocumentHTMLKind,
	NSXMLDocumentTextKind
};

/*!
    @class NSXMLDocument
    @abstract An XML Document
	@discussion Note: if the application of a method would result in more than one element in the children array, an exception is thrown. Trying to add a document, namespace, attribute, or node with a parent also throws an exception. To add a node with a parent first detach or create a copy of it.
*/
@interface NSXMLDocument : NSXMLNode {
@protected
	NSString *_encoding;
	NSString *_version;
	NSXMLDTD *_docType;
	NSArray *_children;
	BOOL _childrenHaveMutated;
	BOOL _standalone;
	int8_t padding[2];
	NSXMLElement *_rootElement;
	NSString *_URI;
	id	 _extraIvars;
	NSUInteger _fidelityMask;
	NSXMLDocumentContentKind _contentKind;
}

- (instancetype)init NS_DESIGNATED_INITIALIZER;

/*!
    @method initWithXMLString:options:error:
    @abstract Returns a document created from either XML or HTML, if the HTMLTidy option is set. Parse errors are returned in <tt>error</tt>.
*/
- (nullable instancetype)initWithXMLString:(NSString *)string options:(NSXMLNodeOptions)mask error:(NSError **)error;

/*!
    @method initWithContentsOfURL:options:error:
    @abstract Returns a document created from the contents of an XML or HTML URL. Connection problems such as 404, parse errors are returned in <tt>error</tt>.
*/
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url options:(NSXMLNodeOptions)mask error:(NSError **)error;

/*!
    @method initWithData:options:error:
    @abstract Returns a document created from data. Parse errors are returned in <tt>error</tt>.
*/
- (nullable instancetype)initWithData:(NSData *)data options:(NSXMLNodeOptions)mask error:(NSError **)error NS_DESIGNATED_INITIALIZER; //primitive

/*!
    @method initWithRootElement:
    @abstract Returns a document with a single child, the root element.
*/
- (instancetype)initWithRootElement:(nullable NSXMLElement *)element NS_DESIGNATED_INITIALIZER;

#if 0
#pragma mark --- Properties ---
#endif

+ (Class)replacementClassForClass:(Class)cls;

/*!
    @abstract Sets the character encoding to an IANA type.
*/
@property (nullable, copy) NSString *characterEncoding; //primitive

/*!
    @abstract Sets the XML version. Should be 1.0 or 1.1.
*/
@property (nullable, copy) NSString *version; //primitive

/*!
    @abstract Set whether this document depends on an external DTD. If this option is set the standalone declaration will appear on output.
*/
@property (getter=isStandalone) BOOL standalone; //primitive

/*!
    @abstract The kind of document.
*/
@property NSXMLDocumentContentKind documentContentKind; //primitive

/*!
    @abstract Set the MIME type, eg text/xml.
*/
@property (nullable, copy) NSString *MIMEType; //primitive

/*!
    @abstract Set the associated DTD. This DTD will be output with the document.
*/
@property (nullable, copy) NSXMLDTD *DTD; //primitive

/*!
    @method setRootElement:
    @abstract Set the root element. Removes all other children including comments and processing-instructions.
*/
- (void)setRootElement:(NSXMLElement *)root;

/*!
    @method rootElement
    @abstract The root element.
*/
- (nullable NSXMLElement *)rootElement; //primitive

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

#if 0
#pragma mark --- Output ---
#endif

/*!
    @abstract Invokes XMLDataWithOptions with NSXMLNodeOptionsNone.
*/
@property (readonly, copy) NSData *XMLData;

/*!
    @method XMLDataWithOptions:
    @abstract The representation of this node as it would appear in an XML document, encoded based on characterEncoding.
*/
- (NSData *)XMLDataWithOptions:(NSXMLNodeOptions)options;

#if 0
#pragma mark --- XSLT ---
#endif

/*!
    @method objectByApplyingXSLT:arguments:error:
    @abstract Applies XSLT with arguments (NSString key/value pairs) to this document, returning a new document.
*/
- (nullable id)objectByApplyingXSLT:(NSData *)xslt arguments:(nullable NSDictionary<NSString *, NSString *> *)arguments error:(NSError **)error;

/*!
    @method objectByApplyingXSLTString:arguments:error:
    @abstract Applies XSLT as expressed by a string with arguments (NSString key/value pairs) to this document, returning a new document.
*/
- (nullable id)objectByApplyingXSLTString:(NSString *)xslt arguments:(nullable NSDictionary<NSString *, NSString *> *)arguments error:(NSError **)error;

/*!
    @method objectByApplyingXSLTAtURL:arguments:error:
    @abstract Applies the XSLT at a URL with arguments (NSString key/value pairs) to this document, returning a new document. Error may contain a connection error from the URL.
*/
- (nullable id)objectByApplyingXSLTAtURL:(NSURL *)xsltURL arguments:(nullable NSDictionary<NSString *, NSString *> *)argument error:(NSError **)error;

#if 0
#pragma mark --- Validation ---
#endif

- (BOOL)validateAndReturnError:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
