/*	NSXMLParser.h
        Copyright (c) 2003-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSError.h>

@class NSData, NSDictionary<KeyType, ObjectType>, NSError, NSString, NSURL, NSInputStream, NSSet<ObjectType>;
@protocol NSXMLParserDelegate;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0))
typedef NS_ENUM(NSUInteger, NSXMLParserExternalEntityResolvingPolicy) {
    NSXMLParserResolveExternalEntitiesNever = 0, // default
    NSXMLParserResolveExternalEntitiesNoNetwork,
    NSXMLParserResolveExternalEntitiesSameOriginOnly, //only applies to NSXMLParser instances initialized with -initWithContentsOfURL:
    NSXMLParserResolveExternalEntitiesAlways
};

@interface NSXMLParser : NSObject {
@private
    id _reserved0;
    id _delegate;
    id _reserved1;
    id _reserved2;
    id _reserved3;
}
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url;  // initializes the parser with the specified URL.
- (instancetype)initWithData:(NSData *)data NS_DESIGNATED_INITIALIZER; // create the parser from data
- (instancetype)initWithStream:(NSInputStream *)stream API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); //create a parser that incrementally pulls data from the specified stream and parses it.

// delegate management. The delegate is not retained.
@property (nullable, assign) id <NSXMLParserDelegate> delegate;

@property BOOL shouldProcessNamespaces;
@property BOOL shouldReportNamespacePrefixes;
    
// The next two properties are really only available in OS X 10.9.5 or later
@property NSXMLParserExternalEntityResolvingPolicy externalEntityResolvingPolicy API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0)); //defaults to NSXMLNodeLoadExternalEntitiesNever

@property (nullable, copy) NSSet<NSURL *> *allowedExternalEntityURLs API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0));

- (BOOL)parse;	// called to start the event-driven parse. Returns YES in the event of a successful parse, and NO in case of error.
- (void)abortParsing;	// called by the delegate to stop the parse. The delegate will get an error message sent to it.

@property (nullable, readonly, copy) NSError *parserError;	// can be called after a parse is over to determine parser state.

//Toggles between disabling external entities entirely, and the current setting of the 'externalEntityResolvingPolicy'.
//The 'externalEntityResolvingPolicy' property should be used instead of this, unless targeting 10.9/7.0 or earlier
@property BOOL shouldResolveExternalEntities;

@end

// Once a parse has begun, the delegate may be interested in certain parser state. These methods will only return meaningful information during parsing, or after an error has occurred.
@interface NSXMLParser (NSXMLParserLocatorAdditions)
@property (nullable, readonly, copy) NSString *publicID;
@property (nullable, readonly, copy) NSString *systemID;
@property (readonly) NSInteger lineNumber;
@property (readonly) NSInteger columnNumber;

@end

/*
 
 For the discussion of event methods, assume the following XML:

 <?xml version="1.0" encoding="UTF-8"?>
 <?xml-stylesheet type='text/css' href='cvslog.css'?>
 <!DOCTYPE cvslog SYSTEM "cvslog.dtd">
 <cvslog xmlns="http://xml.apple.com/cvslog">
   <radar:radar xmlns:radar="http://xml.apple.com/radar">
     <radar:bugID>2920186</radar:bugID>
     <radar:title>API/NSXMLParser: there ought to be an NSXMLParser</radar:title>
   </radar:radar>
 </cvslog>
 
 */

// The parser's delegate is informed of events through the methods in the NSXMLParserDelegateEventAdditions category.
@protocol NSXMLParserDelegate <NSObject>
@optional

// Document handling methods
- (void)parserDidStartDocument:(NSXMLParser *)parser;
    // sent when the parser begins parsing of the document.
- (void)parserDidEndDocument:(NSXMLParser *)parser;
    // sent when the parser has completed parsing. If this is encountered, the parse was successful.

// DTD handling methods for various declarations.
- (void)parser:(NSXMLParser *)parser foundNotationDeclarationWithName:(NSString *)name publicID:(nullable NSString *)publicID systemID:(nullable NSString *)systemID;

- (void)parser:(NSXMLParser *)parser foundUnparsedEntityDeclarationWithName:(NSString *)name publicID:(nullable NSString *)publicID systemID:(nullable NSString *)systemID notationName:(nullable NSString *)notationName;

- (void)parser:(NSXMLParser *)parser foundAttributeDeclarationWithName:(NSString *)attributeName forElement:(NSString *)elementName type:(nullable NSString *)type defaultValue:(nullable NSString *)defaultValue;

- (void)parser:(NSXMLParser *)parser foundElementDeclarationWithName:(NSString *)elementName model:(NSString *)model;

- (void)parser:(NSXMLParser *)parser foundInternalEntityDeclarationWithName:(NSString *)name value:(nullable NSString *)value;

- (void)parser:(NSXMLParser *)parser foundExternalEntityDeclarationWithName:(NSString *)name publicID:(nullable NSString *)publicID systemID:(nullable NSString *)systemID;

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(nullable NSString *)namespaceURI qualifiedName:(nullable NSString *)qName attributes:(NSDictionary<NSString *, NSString *> *)attributeDict;
    // sent when the parser finds an element start tag.
    // In the case of the cvslog tag, the following is what the delegate receives:
    //   elementName == cvslog, namespaceURI == http://xml.apple.com/cvslog, qualifiedName == cvslog
    // In the case of the radar tag, the following is what's passed in:
    //    elementName == radar, namespaceURI == http://xml.apple.com/radar, qualifiedName == radar:radar
    // If namespace processing >isn't< on, the xmlns:radar="http://xml.apple.com/radar" is returned as an attribute pair, the elementName is 'radar:radar' and there is no qualifiedName.

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(nullable NSString *)namespaceURI qualifiedName:(nullable NSString *)qName;
    // sent when an end tag is encountered. The various parameters are supplied as above.

- (void)parser:(NSXMLParser *)parser didStartMappingPrefix:(NSString *)prefix toURI:(NSString *)namespaceURI;
    // sent when the parser first sees a namespace attribute.
    // In the case of the cvslog tag, before the didStartElement:, you'd get one of these with prefix == @"" and namespaceURI == @"http://xml.apple.com/cvslog" (i.e. the default namespace)
    // In the case of the radar:radar tag, before the didStartElement: you'd get one of these with prefix == @"radar" and namespaceURI == @"http://xml.apple.com/radar"

- (void)parser:(NSXMLParser *)parser didEndMappingPrefix:(NSString *)prefix;
    // sent when the namespace prefix in question goes out of scope.

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string;
    // This returns the string of the characters encountered thus far. You may not necessarily get the longest character run. The parser reserves the right to hand these to the delegate as potentially many calls in a row to -parser:foundCharacters:

- (void)parser:(NSXMLParser *)parser foundIgnorableWhitespace:(NSString *)whitespaceString;
    // The parser reports ignorable whitespace in the same way as characters it's found.

- (void)parser:(NSXMLParser *)parser foundProcessingInstructionWithTarget:(NSString *)target data:(nullable NSString *)data;
    // The parser reports a processing instruction to you using this method. In the case above, target == @"xml-stylesheet" and data == @"type='text/css' href='cvslog.css'"

- (void)parser:(NSXMLParser *)parser foundComment:(NSString *)comment;
    // A comment (Text in a <!-- --> block) is reported to the delegate as a single string

- (void)parser:(NSXMLParser *)parser foundCDATA:(NSData *)CDATABlock;
    // this reports a CDATA block to the delegate as an NSData.

- (nullable NSData *)parser:(NSXMLParser *)parser resolveExternalEntityName:(NSString *)name systemID:(nullable NSString *)systemID;
    // this gives the delegate an opportunity to resolve an external entity itself and reply with the resulting data.

- (void)parser:(NSXMLParser *)parser parseErrorOccurred:(NSError *)parseError;
    // ...and this reports a fatal error to the delegate. The parser will stop parsing.

- (void)parser:(NSXMLParser *)parser validationErrorOccurred:(NSError *)validationError;
    // If validation is on, this will report a fatal validation error to the delegate. The parser will stop parsing.
@end

FOUNDATION_EXPORT NSErrorDomain const NSXMLParserErrorDomain	API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));  // for use with NSError.

// Error reporting
typedef NS_ENUM(NSInteger, NSXMLParserError) {
    NSXMLParserInternalError = 1,
    NSXMLParserOutOfMemoryError = 2,
    NSXMLParserDocumentStartError = 3,
    NSXMLParserEmptyDocumentError = 4,
    NSXMLParserPrematureDocumentEndError = 5,
    NSXMLParserInvalidHexCharacterRefError = 6,
    NSXMLParserInvalidDecimalCharacterRefError = 7,
    NSXMLParserInvalidCharacterRefError = 8,
    NSXMLParserInvalidCharacterError = 9,
    NSXMLParserCharacterRefAtEOFError = 10,
    NSXMLParserCharacterRefInPrologError = 11,
    NSXMLParserCharacterRefInEpilogError = 12,
    NSXMLParserCharacterRefInDTDError = 13,
    NSXMLParserEntityRefAtEOFError = 14,
    NSXMLParserEntityRefInPrologError = 15,
    NSXMLParserEntityRefInEpilogError = 16,
    NSXMLParserEntityRefInDTDError = 17,
    NSXMLParserParsedEntityRefAtEOFError = 18,
    NSXMLParserParsedEntityRefInPrologError = 19,
    NSXMLParserParsedEntityRefInEpilogError = 20,
    NSXMLParserParsedEntityRefInInternalSubsetError = 21,
    NSXMLParserEntityReferenceWithoutNameError = 22,
    NSXMLParserEntityReferenceMissingSemiError = 23,
    NSXMLParserParsedEntityRefNoNameError = 24,
    NSXMLParserParsedEntityRefMissingSemiError = 25,
    NSXMLParserUndeclaredEntityError = 26,
    NSXMLParserUnparsedEntityError = 28,
    NSXMLParserEntityIsExternalError = 29,
    NSXMLParserEntityIsParameterError = 30,
    NSXMLParserUnknownEncodingError = 31,
    NSXMLParserEncodingNotSupportedError = 32,
    NSXMLParserStringNotStartedError = 33,
    NSXMLParserStringNotClosedError = 34,
    NSXMLParserNamespaceDeclarationError = 35,
    NSXMLParserEntityNotStartedError = 36,
    NSXMLParserEntityNotFinishedError = 37,
    NSXMLParserLessThanSymbolInAttributeError = 38,
    NSXMLParserAttributeNotStartedError = 39,
    NSXMLParserAttributeNotFinishedError = 40,
    NSXMLParserAttributeHasNoValueError = 41,
    NSXMLParserAttributeRedefinedError = 42,
    NSXMLParserLiteralNotStartedError = 43,
    NSXMLParserLiteralNotFinishedError = 44,
    NSXMLParserCommentNotFinishedError = 45,
    NSXMLParserProcessingInstructionNotStartedError = 46,
    NSXMLParserProcessingInstructionNotFinishedError = 47,
    NSXMLParserNotationNotStartedError = 48,
    NSXMLParserNotationNotFinishedError = 49,
    NSXMLParserAttributeListNotStartedError = 50,
    NSXMLParserAttributeListNotFinishedError = 51,
    NSXMLParserMixedContentDeclNotStartedError = 52,
    NSXMLParserMixedContentDeclNotFinishedError = 53,
    NSXMLParserElementContentDeclNotStartedError = 54,
    NSXMLParserElementContentDeclNotFinishedError = 55,
    NSXMLParserXMLDeclNotStartedError = 56,
    NSXMLParserXMLDeclNotFinishedError = 57,
    NSXMLParserConditionalSectionNotStartedError = 58,
    NSXMLParserConditionalSectionNotFinishedError = 59,
    NSXMLParserExternalSubsetNotFinishedError = 60,
    NSXMLParserDOCTYPEDeclNotFinishedError = 61,
    NSXMLParserMisplacedCDATAEndStringError = 62,
    NSXMLParserCDATANotFinishedError = 63,
    NSXMLParserMisplacedXMLDeclarationError = 64,
    NSXMLParserSpaceRequiredError = 65,
    NSXMLParserSeparatorRequiredError = 66,
    NSXMLParserNMTOKENRequiredError = 67,
    NSXMLParserNAMERequiredError = 68,
    NSXMLParserPCDATARequiredError = 69,
    NSXMLParserURIRequiredError = 70,
    NSXMLParserPublicIdentifierRequiredError = 71,
    NSXMLParserLTRequiredError = 72,
    NSXMLParserGTRequiredError = 73,
    NSXMLParserLTSlashRequiredError = 74,
    NSXMLParserEqualExpectedError = 75,
    NSXMLParserTagNameMismatchError = 76,
    NSXMLParserUnfinishedTagError = 77,
    NSXMLParserStandaloneValueError = 78,
    NSXMLParserInvalidEncodingNameError = 79,
    NSXMLParserCommentContainsDoubleHyphenError = 80,
    NSXMLParserInvalidEncodingError = 81,
    NSXMLParserExternalStandaloneEntityError = 82,
    NSXMLParserInvalidConditionalSectionError = 83,
    NSXMLParserEntityValueRequiredError = 84,
    NSXMLParserNotWellBalancedError = 85,
    NSXMLParserExtraContentError = 86,
    NSXMLParserInvalidCharacterInEntityError = 87,
    NSXMLParserParsedEntityRefInInternalError = 88,
    NSXMLParserEntityRefLoopError = 89,
    NSXMLParserEntityBoundaryError = 90,
    NSXMLParserInvalidURIError = 91,
    NSXMLParserURIFragmentError = 92,
    NSXMLParserNoDTDError = 94,
    NSXMLParserDelegateAbortedParseError = 512
};

NS_ASSUME_NONNULL_END
