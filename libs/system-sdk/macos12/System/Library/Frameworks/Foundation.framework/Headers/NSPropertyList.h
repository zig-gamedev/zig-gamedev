/*	NSPropertyList.h
	Copyright (c) 2002-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#include <CoreFoundation/CFPropertyList.h>

@class NSData, NSString, NSError, NSInputStream, NSOutputStream;

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSPropertyListMutabilityOptions) {
    NSPropertyListImmutable = kCFPropertyListImmutable,
    NSPropertyListMutableContainers = kCFPropertyListMutableContainers,
    NSPropertyListMutableContainersAndLeaves = kCFPropertyListMutableContainersAndLeaves
};

typedef NS_ENUM(NSUInteger, NSPropertyListFormat) {
    NSPropertyListOpenStepFormat = kCFPropertyListOpenStepFormat,
    NSPropertyListXMLFormat_v1_0 = kCFPropertyListXMLFormat_v1_0,
    NSPropertyListBinaryFormat_v1_0 = kCFPropertyListBinaryFormat_v1_0
};

typedef NSPropertyListMutabilityOptions NSPropertyListReadOptions;
typedef NSUInteger NSPropertyListWriteOptions;

@interface NSPropertyListSerialization : NSObject {
    void *reserved[6];
}

/* Verify that a specified property list is valid for a given format. Returns YES if the property list is valid.
 */
+ (BOOL)propertyList:(id)plist isValidForFormat:(NSPropertyListFormat)format;

/* Create an NSData from a property list. The format can be either NSPropertyListXMLFormat_v1_0 or NSPropertyListBinaryFormat_v1_0. The options parameter is currently unused and should be set to 0. If an error occurs the return value will be nil and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem.
 */
+ (nullable NSData *)dataWithPropertyList:(id)plist format:(NSPropertyListFormat)format options:(NSPropertyListWriteOptions)opt error:(out NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* Write a property list to an output stream. The stream should be opened and configured. The format can be either NSPropertyListXMLFormat_v1_0 or NSPropertyListBinaryFormat_v1_0. The options parameter is currently unused and should be set to 0. If an error occurs the return value will be 0 and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem. In a successful case, the return value is the number of bytes written to the stream.
 */
+ (NSInteger)writePropertyList:(id)plist toStream:(NSOutputStream *)stream format:(NSPropertyListFormat)format options:(NSPropertyListWriteOptions)opt error:(out NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* Create a property list from an NSData. The options can be any of NSPropertyListMutabilityOptions. If the format parameter is non-NULL, it will be filled out with the format that the property list was stored in. If an error occurs the return value will be nil and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem.
 */
+ (nullable id)propertyListWithData:(NSData *)data options:(NSPropertyListReadOptions)opt format:(nullable NSPropertyListFormat *)format error:(out NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* Create a property list by reading from an NSInputStream. The options can be any of NSPropertyListMutabilityOptions. If the format parameter is non-NULL, it will be filled out with the format that the property list was stored in. If an error occurs the return value will be nil and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem.
 */
+ (nullable id)propertyListWithStream:(NSInputStream *)stream options:(NSPropertyListReadOptions)opt format:(nullable NSPropertyListFormat *)format error:(out NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));


/* This method is deprecated. Use dataWithPropertyList:format:options:error: instead.
 */
+ (nullable NSData *)dataFromPropertyList:(id)plist format:(NSPropertyListFormat)format errorDescription:(out __strong NSString * _Nullable * _Nullable)errorString API_DEPRECATED("Use dataWithPropertyList:format:options:error: instead.", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));

/* This method is deprecated. Use propertyListWithData:options:format:error: instead.
 */
+ (nullable id)propertyListFromData:(NSData *)data mutabilityOption:(NSPropertyListMutabilityOptions)opt format:(nullable NSPropertyListFormat *)format errorDescription:(out __strong NSString * _Nullable * _Nullable)errorString API_DEPRECATED("Use propertyListWithData:options:format:error: instead.", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));

@end

NS_ASSUME_NONNULL_END
