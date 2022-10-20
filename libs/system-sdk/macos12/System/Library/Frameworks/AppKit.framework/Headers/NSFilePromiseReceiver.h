/*
	NSFilePromiseReceiver.h
	Application Kit
	Copyright (c) 2015-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSPasteboard.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSOperationQueue;

API_AVAILABLE(macos(10.12))
@interface NSFilePromiseReceiver : NSObject <NSPasteboardReading>
/* A view must register what types it accepts via -registerForDraggedTypes:. Use this class method to get the file promise drag types that NSFilePromiseReceiver can accept, in order to register a view to accept promised files.
 */
@property (class, readonly, copy) NSArray<NSString *> *readableDraggedTypes;

/* The UTI of the file types promised (Note: The count of fileTypes should tell you the number of promised files, however, that is not guaranteed. Historically, some legacy file promisers only list each unique fileType once and write one or more files per type.
 */
@property(copy, readonly) NSArray<NSString*> *fileTypes;

/* The file names of the promised files that are being written to the destination location. Note: This property returns an empty array until the file promise is called in via receivePromisedFilesAtDestination.
 Note: This is an array, because legacy promises are an array of files on one pasteboard item.
 */
@property(copy, readonly) NSArray<NSString*> *fileNames;

/* This effectively calls in the promises. Therefore, only call once you are accepting the file promise.
 All file promisesReceiver's in a drag must specify the same destination location.
 This is an array, because legacy promises are an array of files on one pasteboard item.
 The reader block is called on the supplied operationQueue when the promised file is ready to be read. When the source is an NSFilePromiseProvider, the readerBlock call is wrapped in an NSFileCoordination read. Otherwise, a heuristic is used to determine when the promised file is ready to be read.
 The options dictionary is ignored for now.
 Note: Writing of the promised file may be cancelled or fail. When either occurs, the readerBlock is still called, but with a non-nil NSError. When NSError is non-nil, fileURL should be ignored.
 */
- (void)receivePromisedFilesAtDestination:(NSURL *)destinationDir options:(NSDictionary *)options operationQueue:(NSOperationQueue *)operationQueue reader:(void(^)(NSURL *fileURL, NSError * _Nullable errorOrNil))reader;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
