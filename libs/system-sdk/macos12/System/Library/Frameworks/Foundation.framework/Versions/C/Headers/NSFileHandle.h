/*	NSFileHandle.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSException.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSRunLoop.h>

@class NSString, NSData, NSError;

NS_ASSUME_NONNULL_BEGIN

@interface NSFileHandle : NSObject <NSSecureCoding>

@property (readonly, copy) NSData *availableData;

- (instancetype)initWithFileDescriptor:(int)fd closeOnDealloc:(BOOL)closeopt NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (nullable NSData *)readDataToEndOfFileAndReturnError:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) NS_REFINED_FOR_SWIFT;

- (nullable NSData *)readDataUpToLength:(NSUInteger)length error:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) NS_REFINED_FOR_SWIFT;

- (BOOL)writeData:(NSData *)data error:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) NS_REFINED_FOR_SWIFT;

- (BOOL)getOffset:(out unsigned long long *)offsetInFile error:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) NS_REFINED_FOR_SWIFT;

- (BOOL)seekToEndReturningOffset:(out unsigned long long *_Nullable)offsetInFile error:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) NS_REFINED_FOR_SWIFT;

- (BOOL)seekToOffset:(unsigned long long)offset error:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

- (BOOL)truncateAtOffset:(unsigned long long)offset error:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

- (BOOL)synchronizeAndReturnError:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

- (BOOL)closeAndReturnError:(out NSError **)error
    API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

@end

@interface NSFileHandle (NSFileHandleCreation)

@property (class, readonly, strong) NSFileHandle *fileHandleWithStandardInput;
@property (class, readonly, strong) NSFileHandle *fileHandleWithStandardOutput;
@property (class, readonly, strong) NSFileHandle *fileHandleWithStandardError;
@property (class, readonly, strong) NSFileHandle *fileHandleWithNullDevice;

+ (nullable instancetype)fileHandleForReadingAtPath:(NSString *)path;
+ (nullable instancetype)fileHandleForWritingAtPath:(NSString *)path;
+ (nullable instancetype)fileHandleForUpdatingAtPath:(NSString *)path;

+ (nullable instancetype)fileHandleForReadingFromURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
+ (nullable instancetype)fileHandleForWritingToURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
+ (nullable instancetype)fileHandleForUpdatingURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

FOUNDATION_EXPORT NSExceptionName const NSFileHandleOperationException;

FOUNDATION_EXPORT NSNotificationName const NSFileHandleReadCompletionNotification;
FOUNDATION_EXPORT NSNotificationName const NSFileHandleReadToEndOfFileCompletionNotification;
FOUNDATION_EXPORT NSNotificationName const NSFileHandleConnectionAcceptedNotification;
FOUNDATION_EXPORT NSNotificationName const NSFileHandleDataAvailableNotification;

FOUNDATION_EXPORT NSString * const NSFileHandleNotificationDataItem;
FOUNDATION_EXPORT NSString * const NSFileHandleNotificationFileHandleItem;
FOUNDATION_EXPORT NSString * const NSFileHandleNotificationMonitorModes API_DEPRECATED("Not supported", macos(10.0,10.7), ios(2.0,5.0), watchos(2.0,2.0), tvos(9.0,9.0));

@interface NSFileHandle (NSFileHandleAsynchronousAccess)

- (void)readInBackgroundAndNotifyForModes:(nullable NSArray<NSRunLoopMode> *)modes;
- (void)readInBackgroundAndNotify;

- (void)readToEndOfFileInBackgroundAndNotifyForModes:(nullable NSArray<NSRunLoopMode> *)modes;
- (void)readToEndOfFileInBackgroundAndNotify;

- (void)acceptConnectionInBackgroundAndNotifyForModes:(nullable NSArray<NSRunLoopMode> *)modes;
- (void)acceptConnectionInBackgroundAndNotify;

- (void)waitForDataInBackgroundAndNotifyForModes:(nullable NSArray<NSRunLoopMode> *)modes;
- (void)waitForDataInBackgroundAndNotify;

#ifdef __BLOCKS__
@property (nullable, copy) void (^readabilityHandler)(NSFileHandle *)  API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
@property (nullable, copy) void (^writeabilityHandler)(NSFileHandle *) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
#endif

@end

@interface NSFileHandle (NSFileHandlePlatformSpecific)

- (instancetype)initWithFileDescriptor:(int)fd;

@property (readonly) int fileDescriptor;

@end

@interface NSFileHandle (/* Deprecations */)

/* The API below may throw exceptions and will be deprecated in a future version of the OS.
 Use their replacements instead. */

- (NSData *)readDataToEndOfFile
    API_DEPRECATED_WITH_REPLACEMENT("readDataToEndOfFileAndReturnError:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

- (NSData *)readDataOfLength:(NSUInteger)length
    API_DEPRECATED_WITH_REPLACEMENT("readDataUpToLength:error:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


- (void)writeData:(NSData *)data
    API_DEPRECATED_WITH_REPLACEMENT("writeData:error:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


@property (readonly) unsigned long long offsetInFile
    API_DEPRECATED_WITH_REPLACEMENT("getOffset:error:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

- (unsigned long long)seekToEndOfFile
    API_DEPRECATED_WITH_REPLACEMENT("seekToEndReturningOffset:error:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

- (void)seekToFileOffset:(unsigned long long)offset
    API_DEPRECATED_WITH_REPLACEMENT("seekToOffset:error:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


- (void)truncateFileAtOffset:(unsigned long long)offset
    API_DEPRECATED_WITH_REPLACEMENT("truncateAtOffset:error:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

- (void)synchronizeFile
    API_DEPRECATED_WITH_REPLACEMENT("synchronizeAndReturnError:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

- (void)closeFile
    API_DEPRECATED_WITH_REPLACEMENT("closeAndReturnError:",
                                    macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED),
                                    watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));


@end

@interface NSPipe : NSObject

@property (readonly, retain) NSFileHandle *fileHandleForReading;
@property (readonly, retain) NSFileHandle *fileHandleForWriting;

+ (NSPipe *)pipe;

@end

NS_ASSUME_NONNULL_END
