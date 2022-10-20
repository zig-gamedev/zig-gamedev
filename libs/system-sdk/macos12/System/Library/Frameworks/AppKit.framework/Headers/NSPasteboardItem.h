/*
	NSPasteboardItem.h
	Application Kit
	Copyright (c) 2008-2021, Apple Inc.
	All rights reserved.
*/


#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSPasteboard.h>
#import <CoreFoundation/CFBase.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSPasteboard;
@protocol NSPasteboardItemDataProvider;

/* An NSPasteboard can contain multiple items.  Any object that implements the NSPasteboardWriting and NSPasteboardReading protocols can be written and read on the pasteboard directly.  This allows common pasteboard classes such as URLs, colors, images, strings, attributed strings, and sounds to be written and read without an intermediary object.  The custom classes of an application can also implement these protocols for use with the pasteboard.

Sometimes, however, an application needs more fine-grained access to the types and data of a particular pasteboard item, or in the case of a delegate or subclass, needs a way to inspect and change what has already been put on the pasteboard.  In these cases, it is appropriate to use pasteboard items.

There are three main uses for an NSPasteboardItem:
    1. Providing data on the pasteboard: create one or more pasteboard items, set data or data providers for types, and write to the pasteboard
    2. Customizing data already on the pasteboard: as a delegate or subclass, retrieve the pasteboard items currently on the pasteboard, Read the existing types and data and set new data and data providers for types as needed.
    3. Retrieving data from the pasteboard: Retrieve the pasteboard items from the pasteboard.  Read the data for types.

A pasteboard item can be associated with a single pasteboard.  When you create a pasteboard item, it can be written to any pasteboard.  When you pass in a pasteboard item to -writeObjects:, that pasteboard item becomes bound to the pasteboard it was written to.  When you retrieve pasteboard items using -pasteboardItems or -readObjectsForClasses:options:, the returned pasteboard items are associated with the messaged pasteboard.  Passing a pasteboard item that is aready associated with a pasteboard into -writeObjects: causes an exception to be raised.

Pasteboard items are intended to be used during a single pasteboard interaction, not held onto and used repeatedly.  A pasteboard item is only valid until the owner of the pasteboard changes.  If a pasteboard item is stale because the pasteboard owner has changed, it will return nil or NO values from its methods.

The pasteboard item API is very similar to the NSPasteboard API for a single item.  One important difference is that NSPasteboardItem expects strings which are valid UTI strings.  If a type is specified that is not a valid UTI string, the method call will fail.  Similarly, all reported types are UTIs.  As of 10.6, NSPasteboard.h declares a number of NSPasteboardType constants which can be used to provide the correct UTI for common pasteboard types.

*/

API_AVAILABLE(macos(10.6))
@interface NSPasteboardItem : NSObject <NSPasteboardWriting, NSPasteboardReading>
/* Returns an array of UTI strings of the data types supported by the receiver.
*/
@property (readonly, copy) NSArray<NSPasteboardType> *types;

/* Given an array of types, will return the first type contained in the pasteboard item, according to the sender's ordering of types.  It will check for UTI conformance of the requested types, preferring an exact match to conformance.
*/
- (nullable NSPasteboardType)availableTypeFromArray:(NSArray<NSPasteboardType> *)types;

/* Promises the types and registers the data provider to be messaged to provide the data for that type when requested.
*/
- (BOOL)setDataProvider:(id<NSPasteboardItemDataProvider>)dataProvider forTypes:(NSArray<NSPasteboardType> *)types;

/* Sets a value for the provided UTI type string.
*/
- (BOOL)setData:(NSData *)data forType:(NSPasteboardType)type;
- (BOOL)setString:(NSString *)string forType:(NSPasteboardType)type;
- (BOOL)setPropertyList:(id)propertyList forType:(NSPasteboardType)type;

/* Returns a value for the provided UTI type string.
*/
- (nullable NSData *)dataForType:(NSPasteboardType)type;
- (nullable NSString *)stringForType:(NSPasteboardType)type;
- (nullable id)propertyListForType:(NSPasteboardType)type;

@end



@protocol NSPasteboardItemDataProvider <NSObject>
/* Implemented by the data provider of a pasteboard item, to provide the data for a particular UTI type.
*/
@required
- (void)pasteboard:(nullable NSPasteboard *)pasteboard item:(NSPasteboardItem *)item provideDataForType:(NSPasteboardType)type;

/* One data provider can provide data for more than one pasteboard item.  This method is called when the pasteboard no longer needs the data provider for any of its pasteboard items.  This can be either because the data provider has fulfilled all promises, or because ownership of the pasteboard has changed.
*/
@optional
- (void)pasteboardFinishedWithDataProvider:(NSPasteboard *)pasteboard;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

