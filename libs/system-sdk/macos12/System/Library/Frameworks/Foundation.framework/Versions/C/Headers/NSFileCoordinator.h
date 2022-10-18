/*
	NSFileCoordinator.h
	Copyright (c) 2010-2019, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSURL.h>

@class NSArray<ObjectType>, NSError, NSMutableDictionary, NSOperationQueue, NSSet<ObjectType>;

@protocol NSFilePresenter;

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSFileCoordinatorReadingOptions) {

    /* Whether reading does _not_ trigger sending of -savePresentedItemChangesWithCompletionHandler: to certain NSFilePresenters in the system and waiting for those NSFilePresenters to respond. The default behavior during coordinated reading is to send -savePresentedItemChangesWithCompletionHandler: to NSFilePresenters.
    */
    NSFileCoordinatorReadingWithoutChanges = 1 << 0,

    /* Whether reading of an item that might be a symbolic link file causes the resolution of the link if it is. This affects the URL passed to the block passed to an invocation of one of the -coordinateReadingItemAtURL:... methods. This is not a valid option to use with -prepareForReadingItemsAtURLs:options:writingItemsAtURLs:options:error:byAccessor:.
    */
    NSFileCoordinatorReadingResolvesSymbolicLink = 1 << 1,

    /* Whether the reading to be done will only attempt to get an item's metadata that is immediately available (name, modification date, tags, and other attributes), and not its contents. For ubiquitous items, specifying this option will cause coordinated reads to be granted immediately (barring other coordinated readers or writers or file presenters on the same system on the same system preventing this) instead of waiting for any downloading of contents or additional metadata like conflicting versions or thumbnails. Attempting to read the item's contents during such a coordinated read may give you unexpected results or fail.
     */
    NSFileCoordinatorReadingImmediatelyAvailableMetadataOnly API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 1 << 2,

    /* Whether reading of an item is being done for the purpose of uploading. When using this option, NSFileCoordinator will create a temporary snapshot of the item being read and will relinquish its claim on the file once that snapshot is made to avoid blocking other coordinated writes during a potentially long upload. If the item at the URL being read is a directory (such as a document package), then the snapshot will be a new file that contains the zipped contents of that directory, and the URL passed to the accessor block will locate that file.
     
    When using this option, you may upload the document outside of the accessor block. However, you should open a file descriptor to the file or relocate the file within the accessor block before you do so, because NSFileCoordinator will unlink the file after the block returns, rendering it inaccessible via the URL.
    */
    NSFileCoordinatorReadingForUploading API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 1 << 3,

};

typedef NS_OPTIONS(NSUInteger, NSFileCoordinatorWritingOptions) {

    /* You can use only one of these writing options at a time. Using none of them indicates that the writing will simply update the item.
    */

    /* Whether the writing to be done is actually the deletion of the item. This affects how the writing waits for previously scheduled coordinated reading and writing, how the writing causes subsequently scheduled reading and writing to wait, and what NSFilePresenter messaging is done. See the comments in the Single File Coordination section below. This option is how you trigger sending of -accommodatePresentedItemDeletionWithCompletionHandler: or -accommodatePresentedSubitemDeletionAtURL:completionHandler: messages to NSFilePresenters.

    For example, Finder uses this when it's emptying the trash to give NSFilePresenters a chance to close documents before their files disappear, or would disappear if the files weren't still open.
    */
    NSFileCoordinatorWritingForDeleting = 1 << 0,

    /* Whether the writing to be done is actually the moving or renaming of the item. This affects how the writing waits for previously scheduled coordinated reading and writing, how the writing causes subsequently scheduled reading and writing to wait, and what NSFilePresenter messaging is done. See the comments in the Single File Coordination section below. This option has no effect when what's being moved is a plain file so you can use it in code that moves file system items without checking whether the items are files or directories. Any such check would invite a race condition anyway.

    For example, Finder uses this when it's moving items that the user has dragged and dropped so as not to yank files contained by moved folders out from underneath applications that are reading or writing those files.
    */
    NSFileCoordinatorWritingForMoving = 1 << 1,

    /* Whether coordinated writing triggers sending of -savePresentedItemChangesWithCompletionHandler: to certain NSFilePresenters in the system and waiting for those NSFilePresenters to respond.
    */
    NSFileCoordinatorWritingForMerging = 1 << 2,

    /* Whether the writing to be done is actually the replacement of the item with a different item. It causes the same behavior as NSFileCoordinatorWritingForDeleting except that when the item being written to is renamed or moved while the writer is being made to wait the item is considered to have been a different item, so the writer is not passed an updated URL to reflect the renaming or moving. Use this when the moving or creation of an item will replace any item that gets in its way. To avoid a race condition use it regardless of whether there is actually an item in the way before the writing begins. Don't use this when simply updating the contents of a file, even if the way you do that is writing the contents to another file and renaming it into place. This is not a valid option to use with -prepareForReadingItemsAtURLs:options:writingItemsAtURLs:options:error:byAccessor:.

    For example, NSDocument uses this for NSSaveAsOperation and NSSaveToOperation to announce that it is possibly overwriting an item with a brand new file or file package. This gives any NSFilePresenter of the overwritten item, including perhaps a different instance of NSDocument, perhaps in the same application, a chance to close itself before the item is overwritten.

    For another example, the most accurate and safe way to coordinate a move is to invoke -coordinateWritingItemAtURL:options:writingItemAtURL:options:error:byAccessor: using the NSFileCoordinatorWritingForMoving option with the source URL and NSFileCoordinatorWritingForReplacing with the destination URL.
    */
    NSFileCoordinatorWritingForReplacing = 1 << 3,

    /* Whether the writing to be done will change the item's metadata only and not its contents. If the item being written to is ubiquitous, then changes to the item's contents during this coordinated write may not be preserved or fail. When using this option, changing metadata that is related to the item's contents is not supported for ubiquitous items and such changes may not be preserved. For example, changing the value of NSURLTagNamesKey is supported, but changing the value of NSURLContentModificationDateKey is not.
     */
    NSFileCoordinatorWritingContentIndependentMetadataOnly API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 1 << 4

};


API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
@interface NSFileAccessIntent : NSObject {
@private
    NSURL *_url;
    BOOL _isRead;
    NSInteger _options;
}
+ (instancetype)readingIntentWithURL:(NSURL *)url options:(NSFileCoordinatorReadingOptions)options;
+ (instancetype)writingIntentWithURL:(NSURL *)url options:(NSFileCoordinatorWritingOptions)options;
@property (readonly, copy) NSURL *URL; // Use this URL within the accessor block. This property may change from its original value in response to actions from other writers.
@end


API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0))
@interface NSFileCoordinator : NSObject {
    @private
    id _accessArbiter;
    id _fileReactor;
    id _purposeID;
    NSURL *_recentFilePresenterURL;
    id _accessClaimIDOrIDs;
    BOOL _isCancelled;
    NSMutableDictionary *_movedItems;
}

#pragma mark *** File Presenters ***

/* The process' file presenters. If you invoke +addFilePresenter: you have to do a balancing invocation of +removeFilePresenter: before the file presenter is deallocated, even in a garbage-collected application.

If your application reads an item and then registers a file presenter for it there is a possible race condition in which between those two steps another process does coordinated reading or writing of the item, without any messages sent to your not-quite-registered file presenter. This can leave your file presenter ignorant of the fact that what it knows about the item it just read is already out of date, or under the misconception that just because it hasn't received a -relinquish... method it owns the item. To avoid that race condition you can invoke +addFilePresenter: in the same block that you pass to -coordinateReadingItemAtURL:options:error:byAccessor: to read what the file presenter will present.
*/
+ (void)addFilePresenter:(id<NSFilePresenter>)filePresenter;
+ (void)removeFilePresenter:(id<NSFilePresenter>)filePresenter;
@property (class, readonly, copy) NSArray<id<NSFilePresenter>> *filePresenters;

/* The designated initializer. If an NSFilePresenter is provided then the receiver is considered to have been created by that NSFilePresenter, or on its behalf.

NSFileCoordinator is meant to be instantiated on a per-file-operation basis, where a file operation is something like the opening or saving of a document, or the copying or moving of a batch of folders and files. There is no benefit to keeping an instance of it alive in your application for much more time than it takes to actually perform the file operation. Doing so can be harmful, or at least wasteful of memory, because NSFileCoordinators may retain NSFilePresenters.

You pass an NSFilePresenter to this initializer when the operation whose file access is to be coordinated is being performed by that NSFilePresenter. Associating an NSFileCoordinator with an NSFilePresenter accomplishes a few important things:
- It prevents the NSFileCoordinator from sending messages to that NSFilePresenter, so the NSFilePresenter does not have to somehow filter out messages about its own file operations. The exception to this rule is that messages about versions of the presented item being added, remove, or resolved during coordinated writing are sent to every relevant NSFilePresenter, even the one passed to -initWithFilePresenter:.
- It allows the file coordination mechanism to determine when coordinated writing is being done in response to an NSFilePresenter receiving a -savePresentedItemChangesWithCompletionHandler: message, and not deadlock. Usually coordinated writing done by one NSFileCoordinator must wait for coordinated reading of the same file or directory done by another NSFileCoordinator. But, for example, when coordinated reading is begun with one NSFileCoordinator, and that causes an NSFilePresenter to do coordinated writing using another NSFileCoordinator, the writing done with the second NSFileCoordinator should not wait for the completion of the first NSFileCoordinator's reading, it should instead happen while the first NSFileCoordinator is waiting to read.
- It allows the file coordination mechanism to handle a race condition that can occur when it has sent an NSFilePresenter a -presentedItemDidMoveToURL: message in the NSFilePresenter's operation queue but before that message is dequeued the NSFilePresenter enqueues, on a different queue, an operation using the old URL. For this to be effective however the NSFileCoordinator must be initialized in the same operation queue in which NSFilePresenter messages are received.
- It allows the file coordination mechanism to gracefully handle your application's registration of an NSFilePresenter that at first returns nil when sent -presentedItemURL but can later return non-nil at the end of doing a coordinated write that creates the presented item in the file system for the very first time. AppKit for example takes advantage of this by registering brand new untitled NSDocuments as NSFilePresenters immediately, instead of waiting until after the first time the user causes the document to be saved to a file, which would be more complicated.

For example, NSDocument creates a single NSFileCoordinator for all of the coordinated reading and writing it does during the saving of a document. It always creates the NSFileCoordinator in the main queue even when it is doing the actual coordinated reading and writing in a background queue to implement asynchronous saving.
*/
- (instancetype)initWithFilePresenter:(nullable id<NSFilePresenter>)filePresenterOrNil NS_DESIGNATED_INITIALIZER;

#pragma mark *** Purpose Identifier ***

/* A string that uniquely identifies the file access that will be done by this NSFileCoordinator. Every NSFileCoordinator has a unique purpose identifier that is created during initialization. Coordinated reads and writes performed by NSFileCoordinators with the same purpose identifier never block each other, even if they exist in different processes. In addition to some of the reasons explained in the comments of -initWithFilePresenter:, you may want to set a custom purpose identifier for the following reasons:
- Your application has an NSFileProviderExtension. Any file coordination done on behalf of the NSFileProviderExtension needs to be done using the same purpose identifier reported by your NSFileProviderExtension.
- To avoid deadlocking when two separate subsystems need to work together to perform one high-level operation, and both subsystems perform their own coordinated reads or writes.
 
If you are coordinating file access on behalf of an NSFilePresenter, you should use -initWithFilePresenter: and should not attempt to set a custom purpose identifier. Every NSFileCoordinator instance initialized with the same NSFilePresenter will have the same purpose identifier.
 
When creating custom purpose identifiers, you can use a reverse DNS style string, such as "com.mycompany.myapplication.mypurpose", or a UUID string. Nil and zero-length strings are not allowed.
 
Purpose identifiers can be set only once. If you attempt to set the purpose identifier of an NSFileCoordinator that you initialized with -initWithFilePresenter: or that you already assigned a purpose identifier, an exception will be thrown.
*/
@property (copy) NSString *purposeIdentifier API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

#pragma mark *** Asynchronous File Coordination ***

/* You can consider "item" in method names and comments in this header file to be an abbreviation of "fileOrDirectory." As always, a directory might actually be a file package.

The term "reader" refers to an invocation of -coordinateAccessWithIntents:queue:byAccessor: with at least one NSFileAccessIntent created with +readingIntentWithURL:options:. Similarly, the term "writer" refers to an invocation of -coordinateAccessWithIntents:queue:byAccessor: with at least one NSFileAccessIntent created with +writingIntentWithURL:optiosn:.
*/

/* Given an array of one or more NSFileAccessIntent objects that specify reading and/or writing items located by the corresponding URLs, wait asynchronously for certain other readers and writers and then invoke the passed-in block on the given queue, which must not be nil. If an error occurs, file access is not granted and a non-nil error will be passed to the accessor block. If file access is successfully granted, then 'error' will be nil and you may perform the intended file access inside the accessor block.
 
You must use the URL property on the NSFileAccessIntent objects when performing file access in the accessor block. Within the block, the NSFileAccessIntent objects' URLs may differ from their original values to account for items that have been moved or renamed while waiting for access to be granted. When access to the intended files is granted, certain other readers and writers are made to wait until the given block returns, which defines the end of that file access. Do not allow file access to continue after the accessor block returns by dispatching work to other threads or queues.
 
You can invoke this method to serialize your process's access of files and directories with other processes' access of the same files and directories so that inconsistencies due to overlapping reading and writing don't occur. It also causes messages to be sent to NSFilePresenters, and wait for NSFilePresenters to react, as described below. The fact that file system items can be moved or renamed while this method is waiting to invoke the block you passed when invoking it is why it's critical to use the URL property on the NSFileAccessIntent objects, not the URLs you used when initializing them.
 
In general a coordinated reader waits for a coordinated writer of the same item, and a coordinated writer waits for coordinated readers and other coordinated writers of the same item. Coordinated readers do not wait for each other. Coordinated reading or writing of items in a file package is treated as coordinated reading or writing of the file package as a whole. A coordinated reader of a directory that is not a file package does not wait for coordinated writers of contained items, or cause such writers to wait. With one exception, a coordinated writer of a directory that is not a file package does not wait for coordinated readers and writers of contained items, or cause such readers and writers to wait. The exception is when you use NSFileCoordinatorWritingForDeleting, NSFileCoordinatorWritingForMoving, or NSFileCoordinatorWritingForReplacing. They make your coordinated writer wait for previously scheduled coordinated readers and writers of contained items, and causes subsequently scheduled coordinated readers and writers of contained items to wait.
 
None of those rules apply to coordinated readers and writers that are using the exact same instance of NSFileCoordinator, including arrays of multiple NSFileAccessIntent objects. Instances of NSFileCoordinator never block themselves. You can take advantage of that in a couple of ways when invoking -coordinateAccessWithIntents:queue:byAccessor: multiple times on the same NSFileCoordinator instance, but take care because doing so raises the possibility of deadlocking with other processes that are doing the same sort of thing. If you can, you should invoke -coordinateAccessWithIntents:queue:byAccessor: a single time with multiple NSFileAccessIntent objects instead of invoking it multiple times with a single NSFileAccssIntent object.
 
In addition to waiting for writers, readers wait for NSFilePresenters that are messaged as part of the coordinated reading.
 
Coordinated reading of an item triggers the sending of messages to NSFilePresenters that implement the corresponding optional methods, even those in other processes, except the one specified when -initWithFilePresenter: was invoked:
- -relinquishPresentedItemToReader: is sent to NSFilePresenters of the item and, if the item is in a file package, NSFilePresenters of the file package. If there are nested file packages then the message is sent to NSFilePresenters of all of them.
- If NSFileCoordinatorReadingWithoutChanges is not used then -savePresentedItemChangesWithCompletionHandler: is also sent to the same NSFilePresenters.

In addition to waiting for readers and other writers, writers wait for NSFilePresenters that are messaged as part of the coordinated writing.
 
Coordinated writing of an item triggers the sending of messages to NSFilePresenters that implement the corresponding optional methods, even those in other processes, except the one specified when -initWithFilePresenter: was invoked:
- -relinquishPresentedItemToWriter: is sent to NSFilePresenters of the item and, if the item is in a file package, NSFilePresenters of the file package. If there are nested file packages then the message is sent to NSFilePresenters of all of them.
- If NSFileCoordinatorWritingForDeleting, NSFileCoordinatorWritingForMoving, or NSFileCoordinatorWritingForReplacing is used and the item is a directory then -relinquishPresentedItemToWriter: is also sent to NSFilePresenters of each item contained by it.
- If NSFileCoordinatorWritingForDeleting or NSFileCoordinatorWritingForReplacing is used then -accommodatePresentedItemDeletionWithCompletionHandler: is sent to NSFilePresenters of the item and, if the item is a directory, NSFilePresenters of each item contained by it. -accommodatePresentedSubitemDeletionAtURL:completionHandler: is sent to NSFilePresenters of each file package that contains the item.
- When NSFileCoordinatorWritingForReplacing is used the the definition of "the item" depends on what happened while waiting for other writers. See the description of it above.
- If NSFileCoordinatorWritingForMerging is used then -savePresentedItemChangesWithCompletionHandler: is sent to NSFilePresenters of the item and, if the item is in a file package, NSFilePresenters of the file package. If there are nested file packages then the message is sent to NSFilePresenters of all of them.
 
For both coordinated reading and writing, if there are multiple NSFilePresenters involved then the order in which they are messaged is undefined. If an NSFilePresenter signals failure then waiting will fail and *outError will be set to an NSError describing the failure.
*/
- (void)coordinateAccessWithIntents:(NSArray<NSFileAccessIntent *> *)intents queue:(NSOperationQueue *)queue byAccessor:(void (^)(NSError * _Nullable error))accessor API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

#pragma mark *** Synchronous File Coordination ***

/* The next four methods behave similarly to -coordinateAccessWithIntents:queue:byAccessor: with one or two NSFileAccessIntent objects with the following exceptions:

Each of these methods wait synchronously on the same thread they were invoked on before invoking the passed-in accessor block on the same thread, instead of waiting asynchronously and scheduling invocation of the block on a specific queue.

The accessor block of each of these methods is passed one or more URLs that locate the intended items, perhaps changed from the original URLs to take into account the fact that the item might have been moved or renamed during the waiting.
 
Each of these methods returns an NSError by reference instead of passing it to the accessory block. However, these methods are uncommon among Cocoa framework methods in that they don't also return a result indicating success or failure. The success of the waiting that they do is typically not interesting to invokers. Only the success of file system access done by the passed-in block is interesting. (The failure of either is of course interesting.) When invoking these methods it's cleanest to just declare a __block variable outside of the block and initialize it to a value that signals failure, and then inside the block set it to a value that signals success. If the waiting fails then the invoked method sets the error reference to an NSError that describes what went wrong, your block will not be invoked, your __block variable will not be set to a value that signals success, and all will be as it should be, with failure signaled and an NSError that describes the failure.
*/
- (void)coordinateReadingItemAtURL:(NSURL *)url options:(NSFileCoordinatorReadingOptions)options error:(NSError **)outError byAccessor:(void (NS_NOESCAPE ^)(NSURL *newURL))reader;
- (void)coordinateWritingItemAtURL:(NSURL *)url options:(NSFileCoordinatorWritingOptions)options error:(NSError **)outError byAccessor:(void (NS_NOESCAPE ^)(NSURL *newURL))writer;
- (void)coordinateReadingItemAtURL:(NSURL *)readingURL options:(NSFileCoordinatorReadingOptions)readingOptions writingItemAtURL:(NSURL *)writingURL options:(NSFileCoordinatorWritingOptions)writingOptions error:(NSError **)outError byAccessor:(void (NS_NOESCAPE ^)(NSURL *newReadingURL, NSURL *newWritingURL))readerWriter;
- (void)coordinateWritingItemAtURL:(NSURL *)url1 options:(NSFileCoordinatorWritingOptions)options1 writingItemAtURL:(NSURL *)url2 options:(NSFileCoordinatorWritingOptions)options2 error:(NSError **)outError byAccessor:(void (NS_NOESCAPE ^)(NSURL *newURL1, NSURL *newURL2))writer;

#pragma mark *** Batched File Coordination ***

/* Prepare to more efficiently do a large number of invocations of -coordinate... methods, first synchronously messaging and waiting for NSFilePresenters in a variation of what individual invocations of the -coordinate... methods would do, and then, if no error occurs, invoke the passed-in block. The passed-in block must invoke the completion handler passed to it when all of the coordinated reading and writing it does is done. The completion handler block can be invoked on any thread (or from any dispatch queue, if that's how you think of it). This method returns errors in the same manner as the -coordinate... methods.

The -coordinate... methods must use interprocess communication to message instances of NSFileCoordinator and NSFilePresenter in other processes in the system. That is an expense best avoided when reading or writing many files in one operation. Using this method can greatly reduce the amount of interprocess communication required by, for example, a large batched copying or moving of files. You use it by moving all of the invocations of the -coordinate... methods your application will do during a batch operation, or the scheduling of them if the operation's work is done in a multithreaded fashion, into a block and passing that block to an invocation of this method, remembering that the completion handler passed to that block must be invoked when the operation is done. You don't simply pass all URLs that will be passed into invocations of the -coordinate... methods when invoking this method. Instead you pass the top-level files and directories involved in the operation. This method triggers messages to not just NSFilePresenters of those items, but also NSFilePresenters of items contained by those items. For example, when Finder uses this method during a copy operation readingURLs is an array of the URLs of the exact files and folders that the user has selected, even though those folders may contain many files and subfolders for which Finder is going to do coordinated reading, and writingURLs is an array that contains just the URL of the destination folder.

In most cases it is redundant to pass the same reading or writing options in an invocation of this method as are passed to individual invocations of the -coordinate... methods invoked by the block passed to an invocation of this method. For example, when Finder invokes this method during a copy operation it does not pass NSFileCoordinatorReadingWithoutChanges because it is appropriate to trigger the saving of document changes right away, but it does pass it when doing the nested invocations of -coordinate... methods because it is not necessary to trigger saving again, even if the user changes the document before the Finder proceeds far enough to actually copy that document's file.
*/
- (void)prepareForReadingItemsAtURLs:(NSArray<NSURL *> *)readingURLs options:(NSFileCoordinatorReadingOptions)readingOptions writingItemsAtURLs:(NSArray<NSURL *> *)writingURLs options:(NSFileCoordinatorWritingOptions)writingOptions error:(NSError **)outError byAccessor:(void (NS_NOESCAPE ^)(void (^completionHandler)(void)))batchAccessor;

#pragma mark *** Renaming and Moving Notification ***

/*Announce that the item located by a URL is going to be located by another URL.

Support for App Sandbox on OS X. Some applications can rename files while saving them. For example, when a user adds attachments to a rich text document, TextEdit changes the document's extension from .rtf to .rtfd. A sandboxed application like TextEdit must ordinarily prompt the user for approval before renaming a document. You can invoke this method to make your process declare its intent to rename a document without user approval. After the renaming succeeds you must invoke -itemAtURL:didMoveToURL:, with the same arguments, for the process to keep access to the file with its new name and to give up access to any file that appears with the old name. If the renaming fails you should probably not invoke -itemAtURL:didMoveToURL:.

There is no reason to invoke this method from applications that do not use App Sandbox. Invoking it does nothing on iOS.
*/
- (void)itemAtURL:(NSURL *)oldURL willMoveToURL:(NSURL *)newURL API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* Announce that the item located by a URL is now located by another URL.

This triggers the sending of messages to NSFilePresenters that implement the corresponding optional methods, even those in other processes, except the one specified when -initWithFilePresenter: was invoked:
- -presentedItemDidMoveToURL: is sent to NSFilePresenters of the item.
- If the item is a directory then -presentedItemDidMoveToURL: is sent to NSFilePresenters of each item contained by it.
- -presentedSubitemAtURL:didMoveToURL: is sent to NSFilePresenters of each directory that contains the item, unless that method is not implemented but -presentedItemDidChange is, and the directory is actually a file package, in which case -presentedItemDidChange is sent instead.

This also balances invocations of -itemAtURL:willMoveToURL:, as described above.

Useless invocations of this method are harmless, so you don't have to write code that compares NSURLs for equality, which is not straightforward. This method must be invoked from within the block passed to an invocation of -coordinateAccessWithIntents:queue:byAccessory:, -coordinateWritingItemAtURL:options:error:byAccessor:, or -coordinateReadingItemAtURL:options:writingItemAtURL:options:error:byAccessor:.
*/
- (void)itemAtURL:(NSURL *)oldURL didMoveToURL:(NSURL *)newURL;

#pragma mark *** Ubiquity Attribute Change Notification ***

/* Announce that the item located by a URL has changed one or more ubiquity attributes. See NSFilePresenter.observedPresentedItemUbiquityAttributes for an explanation of valid attributes.
 
This triggers the sending of messages to NSFilePresenters that implement -presentedItemDidChangeUbiquityAttibutes:, even those in other processes.
*/
- (void)itemAtURL:(NSURL *)url didChangeUbiquityAttributes:(NSSet <NSURLResourceKey> *)attributes API_AVAILABLE(macos(10.13), ios(11.0)) API_UNAVAILABLE(watchos, tvos);

#pragma mark *** Cancellation ***

/* Cancel all invocations of -coordinate... and -prepare... methods for the receiver. Any current invocation of one of those methods will stop waiting and return immediately, unless it has already invoked the passed-in block, in which case it will return when the passed-in block returns. Subsequent invocations of those methods will not invoke the blocks passed into them at all. When an invocation of -coordinate... or -prepare... returns without invoking the passed-in block because this method was invoked it instead returns an error whose domain is NSCocoaErrorDomain and whose code is NSUserCancelledError. Messages that have already been sent to NSFilePresenters will not be cancelled but the file coordination machinery will stop waiting for the replies.

This method can be invoked from any thread. It always returns immediately, without waiting for anything. Cancellation is racy; you usually cannot assume that no block passed into a -coordinate... or -prepare... method is already being invoked, so the code inside those blocks typically still has to check for cancellation, whatever that means in your application.
*/
- (void)cancel;

@end

NS_ASSUME_NONNULL_END

