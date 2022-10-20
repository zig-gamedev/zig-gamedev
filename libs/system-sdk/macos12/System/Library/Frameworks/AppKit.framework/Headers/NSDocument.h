/*
	NSDocument.h
	Application Kit
	Copyright (c) 1997-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSNib.h>
#import <AppKit/NSNibDeclarations.h>
#import <AppKit/NSUserInterfaceValidation.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSFilePresenter.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSPrintInfo.h>
#import <AppKit/NSKeyValueBinding.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSData, NSDate, NSError, NSFileWrapper, NSMenuItem, NSPageLayout, NSPrintInfo, NSPrintOperation, NSSavePanel, NSSharingService, NSSharingServicePicker, NSUndoManager, NSURL, NSView, NSWindow, NSWindowController;

typedef NS_ENUM(NSUInteger, NSDocumentChangeType) {

/* The value to pass to -updateChangeCount: to indicate that a single change has been done. For example, NSDocument's built-in undo support uses this whenever a document receives an NSUndoManagerWillCloseUndoGroupNotification from its own undo manager.
*/
    NSChangeDone = 0,

/* The value to pass to indicate that a single change has been undone. For example, NSDocument's built-in undo support uses this whenever a document receives an NSUndoManagerDidUndoChangeNotification from its own undo manager.
*/
    NSChangeUndone = 1,

/* The value to pass to indicate that a single change has been redone. For example, NSDocument's built-in undo support uses this whenever a document receives an NSUndoManagerDidRedoChangeNotification from its own undo manager.
*/
    NSChangeRedone API_AVAILABLE(macos(10.5)) = 5,

/* The value to pass to indicate that the document has been synchronized with its file or file package. For example, both -revertToContentsOfURL:ofType:error: and -saveToURL:ofType:forSaveOperation:completionHandler: may use this when they invoke -updateChangeCount:.
*/
    NSChangeCleared = 2,

/* The value to pass to indicate that the document has been initialized with the contents of a file or file package other than the one whose location would be returned by -fileURL, and therefore can't possibly be synchronized with its persistent representation. For example, -initForURL:withContentsOfURL:ofType:error: uses this when the two passed-in URLs are not equal to indicate that an autosaved document is being reopened.
*/
    NSChangeReadOtherContents = 3,

/* The value to pass to indicate that the document's contents have been autosaved. For example, -saveToURL:ofType:forSaveOperation:completionHandler: may use this when it invokes -updateChangeCount:.
*/
    NSChangeAutosaved = 4,
    
/* Use this mask to indicate that a discardable change has been done. Discardable changes cause the document to be edited. In a locked document, for example, discardable changes may be thrown away instead of prompting the user to save them. Bitwise-or this value with the appropriate kind of change, NSChangeDone, NSChangeUndone, or NSChangeRedone. For example, a discardable change is "NSChangeDone | NSChangeDiscardable".
*/
    NSChangeDiscardable API_AVAILABLE(macos(10.7)) = 256
    
};

typedef NS_ENUM(NSUInteger, NSSaveOperationType) {

/* The overwriting of a document's file or file package with the document's current contents because the user explicitly commanded it.
*/
    NSSaveOperation = 0,

/* The writing of a document's current contents to a new file or file package, and then making the just-written file or file package the document's current one.
*/
    NSSaveAsOperation = 1,

/* The writing of a document's current contents to a new file or file package without changing the document's current one.
*/
    NSSaveToOperation = 2,

/* The overwriting of a document's file or file package with the document's current contents even though the user has not explicitly commanded it.
*/
    NSAutosaveInPlaceOperation API_AVAILABLE(macos(10.7)) = 4,

/* The writing of a document's current contents to a file or file package that is separate from the document's current one, without changing the document's current one. For an NSDocument subclass that overrides Mac OS 10.7's +autosavesInPlaces to return YES this is only used during autosaving of documents that have never been saved and therefore do not yet have a document file that can be overwritten during autosaving.
*/
    NSAutosaveElsewhereOperation API_AVAILABLE(macos(10.7)) = 3,
    
/* The writing of a document's current contents to new file or file package even though the user has not explicitly commanded it, and then making the just-written file or file package the document's current one.
*/
    NSAutosaveAsOperation API_AVAILABLE(macos(10.8)) = 5,
    
/* An old name for NSAutosaveElsewhereOperation that was deprecated in Mac OS 10.7.
*/
    NSAutosaveOperation API_DEPRECATED("Use NSAutosaveElsewhereOperation instead", macos(10.4,10.7))= 3
    
};

NS_SWIFT_UI_ACTOR
@interface NSDocument : NSObject<NSEditorRegistration, NSFilePresenter, NSMenuItemValidation, NSUserInterfaceValidations>

#pragma mark *** Initialization ***

/* The designated initializer. It is invoked by each of NSDocument's other initialization methods.

You can override this method to perform initialization that must be done both when creating new empty documents and when opening existing documents. Your override must invoke super to initialize private NSDocument instance variables. It must never return nil. If an error can occur during object initialization, check for the error in an override of -initWithType:error:, -initWithContentsOfURL:ofType:error:, or -initForURL:withContentsOfURL:ofType:error:, because those methods can return NSErrors.
*/
- (instancetype)init NS_DESIGNATED_INITIALIZER;

#pragma mark *** Creation ***

/* Initialize a new empty document of a specified type, and return it if successful. If not successful, return nil after setting *outError to an NSError that encapsulates the reason why the document could not be initialized. The default implementation of this method just invokes [self init] and [self setFileType:typeName].

You can override this method to perform initialization that must be done when creating new documents but should not be done when opening existing documents. Your override should typically invoke super, or at least it must invoke -init, NSDocument's designated initializer, to initialize NSDocument's private instance variables.
*/
- (nullable instancetype)initWithType:(NSString *)typeName error:(NSError **)outError;

#pragma mark *** Opening ***

/* Return whether instances of the receiving class can concurrently read documents of the specified type. The default implementation of this method returns NO. You can override it to return YES to enable concurrent opening of documents but you must make sure your document reading code can be safely executed concurrently, in non-main threads.
*/
+ (BOOL)canConcurrentlyReadDocumentsOfType:(NSString *)typeName API_AVAILABLE(macos(10.6));

/* Initialize a document located by a URL, of a specified type, and return it if successful. If not successful, return nil after setting *outError to an NSError that encapsulates the reason why the document could not be initialized. The default implementation of this method invokes [self init], [self readFromURL:url ofType:typeName error:outError], [self setFileURL:url], [self setFileType:typeName], and [self setFileModificationDate:theModificationDate].

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self initWithContentsOfFile:url ofType:typeName] if -initWithContentsOfFile:ofType: is overridden and the URL uses the "file:" scheme. It still invokes [self setFileModificationDate:theModificationDate] in this situation.
*/
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url ofType:(NSString *)typeName error:(NSError **)outError;

#pragma mark *** Reopening after Autosaving ***

/* Initialize a document located by a URL, of a specified type, but by reading the contents for the document from another URL, and return it if successful. If not successful, return nil after setting *outError to an NSError that encapsulates the reason why the document could not be initialized. The default implementation of this method invokes [self readFromURL:contentsURL ofType:typeName error:outError], [self setFileURL:urlOrNil], [self setAutosavedContentsFileURL:contentsURL], [self setFileType:typeName], and [self setFileModificationDate:theModificationDate]. It also invokes [self updateChangeCount:NSChangeReadOtherContents] if the two URLs aren't identical, so that -isDocumentEdited will always return YES until the user saves or reverts the document.

To ease the adoption of the autosaving feature introduced in Mac OS 10.4, the default implementation of this method invokes [self initWithContentsOfFile:[contentsURL path] ofType:typeName] if -initWithContentsOfFile:ofType: is overridden and the URL uses the "file:" scheme. It still invokes [self setFileModificationDate:theModificationDate] and [self updateChangeCount:NSChangeReadOtherContents] in this situation. It still also invokes [self setFileURL:urlOrNil], to overwrite the incorrect invocation of -setFileName: that the override of -initWithContentsOfFile:ofType: likely did.

urlOrNil will be nil if the initializing is being done as part of the reopening of an autosaved document when the autosaved document had never been saved in the first place.
*/
- (nullable instancetype)initForURL:(nullable NSURL *)urlOrNil withContentsOfURL:(NSURL *)contentsURL ofType:(NSString *)typeName error:(NSError **)outError;

#pragma mark *** Attributes Applicable to Every Kind of Document ***

/* These should all be set at initialization time, but not as part of reading the document, and during NSSaveOperations, NSSaveAsOperations, NSAutosaveInPlaceOperations, and NSAutosaveAsOperations, but not as part of writing the document. Specifically, there should be no need to set these from within an override of a -read... or -write... method.
*/

/* The name of the document's format. The set method isn't for _changing_ the document's format, it's just for initially recording the document's format during opening or saving.
*/
@property (nullable, copy) NSString *fileType;

/* The location of the document's on-disk representation. The set method doesn't actually rename the document, it's just for recording the document's location during initial opening or saving. The default implementation of -setFileURL: just records the URL so that the default implementation of -fileURL can return it. The default implementation of -fileURL returns whatever was stored by a previous invocation of the default implementation of -setFileURL:.

Starting in Mac OS 10.7 the default implementations of these methods are thread safe enough that -setFileURL: being invoked on the main thread while -fileURL is being invoked on a different thread won't cause a crash. If you override one of these methods you must maintain that thread safety because AppKit itself may invoke -fileURL on a non-main thread. (It's still not a good idea to invoke -setFileURL: on a non-main thread though.)

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of -setFileURL: instead invokes [self setFileName:[url path]] if -setFileName: is overridden and the URL is nil or uses the "file:" scheme. Likewise, the default implementation of -fileURL instead invokes -[self fileName] and returns the result as a URL if -fileName is overridden.
*/
@property (nullable, copy) NSURL *fileURL;

/* The last known modification date of the document's on-disk representation.
*/
@property (nullable, copy) NSDate *fileModificationDate;

/* Whether the document is a draft that the user has not expressed an interest in keeping around. A save panel will be presented when the user closes a draft document. Only documents with non-nil values for [self fileURL] should be considered drafts.
*/
@property (getter=isDraft) BOOL draft API_AVAILABLE(macos(10.8));

#pragma mark *** UI and File Access Serialization ***

/* Given a block that will perform work that might result in the presentation of modal UI, wait for any work scheduled by previous invocations of this method to be completed, and then invoke the block. The block is passed another block, the activity completion handler, which must be invoked when the activity is complete. There are two kinds of waiting possible, synchronous and asynchronous. Which one is used is controlled by the waitSynchronously parameter. If the value for that parameter is YES then this method does not return until previous activities are complete and the passed-in block has been invoked. If it is NO then the passed-in block might not be invoked before this method has returned. It might instead be invoked later, on the main thread, after previous activities are complete.

This method's primary use is to wait for asynchronous saving, a new feature that was introduced in Mac OS 10.7. With asynchronous saving it is possible for the user to instigate a user interface action that might present modal UI, a sheet for example, when asynchronous saving is about to fail and present an error alert sheet of its own, which would not work. This method solves that problem. If your NSDocument subclass supports asynchronous saving you should invoke this method around the performance of any work that might cause the presentation of modal UI, regardless of whether that work is performed synchronously or asynchronously. Here is a list of NSDocument methods whose default implementations invoke this method because they might present sheets, either to ask the user what to do as they begin their work or because they may fail and present errors to user:
-revertDocumentToSaved:
-saveDocumentWithDelegate:didSaveSelector:contextInfo:
-runModalSavePanelForSaveOperation:delegate:didSaveSelector:contextInfo:
-saveToURL:ofType:forSaveOperation:delegate:didSaveSelector:contextInfo:
-canCloseDocumentWithDelegate:shouldCloseSelector:contextInfo:
-duplicateDocumentWithDelegate:didDuplicateSelector:contextInfo:
-runModalPageLayoutWithPrintInfo:delegate:didRunSelector:contextInfo:
-printDocumentWithSettings:showPrintPanel:delegate:didPrintSelector:contextInfo:
-runModalPrintOperation:delegate:didRunSelector:contextInfo:
More uses of this method may be added to NSDocument in the future.
 
This method must be invoked on the main thread. If it is passed YES for waitSynchronously it waits on the main thread, blocking further user interaction with the document. The purpose of blocking the main thread is so the user cannot continue to change the document while an activity is pending. This prevents for example the strange situation where the user chooses to revert the document, reverting does not happen immediately because asynchronous saving is still in progress, the user is able to continue to make changes to the document, and then those changes are immediately discarded when the asynchronous saving is complete and the document is reverted. All of the NSDocument methods listed above pass YES for waitSynchronously.

You pass NO for waitSynchronously when the work that is to be done is instigated by the user so indirectly that the work might begin when modal UI is already being presented. For example, here is another method whose default implementation invokes this method, this time passing NO for waitSynchronously:
-autosaveDocumentWithDelegate:didAutosaveSelector:contextInfo:
This method might present an error alert but it is typically invoked by a timer. If it passed YES for waitSynchronously, and the timer fired while the user was looking at a sheet presented by a previous activity, blocking of the main thread would prevent the handling of the user interface events necessary to dismiss that sheet and complete that previous activity. Deadlock would result.

Whether you make this method wait synchronously or asynchronously to do your work is separate from whether your work is done synchronously or asynchronously. For example, as mentioned above -saveToURL:ofType:forSaveOperation:delegate:didSaveSelector:contextInfo: passes YES for waitSynchronously when it uses this method, even though the majority of the work it does may be done asynchronously.

You should not invoke this method during the invocation of the block passed to -performSynchronousFileAccessUsingBlock:, mentioned down below, or in between the time -performAsynchronousFileAccessUsingBlock:, also mentioned down below, invokes the block passed to it and the time at which the corresponding file access completion handler is invoked. If you do then deadlock can result. In other words, you cannot begin a new activity as part of file access. You can on the other hand invoke -performSynchronousFileAccessUsingBlock: or -performAsynchronousFileAccessUsingBlock: as part of an activity.

Some asynchronous activities, like saving, need to do work on the main thread as they are completing. A deadlock would be inevitable if there were no way to interrupt this method's blocking of the main thread. See -continueAsynchronousWorkOnMainThreadUsingBlock: to find out how to interrupt this method's blocking of the main thread.
*/
- (void)performActivityWithSynchronousWaiting:(BOOL)waitSynchronously usingBlock:(void (^)(void (^activityCompletionHandler)(void)))block API_AVAILABLE(macos(10.7));

/* If a block that was passed to -performActivityWithSynchronousWaiting:usingBlock: is being invoked, invoke the passed-in block, having recorded state that makes inner invocations of -performActivityWithSynchronousWaiting:usingBlock: not wait. If this method is invoked outside of an invocation of a block passed to -performActivityWithSynchronousWaiting:usingBlock:, simply invoke the passed-in block.

This method is useful when code executed in a block passed to -performActivityWithSynchronousWaiting:usingBlock: may also invoke that method. For example, -saveDocumentWithDelegate:didSaveSelector:contextInfo:, which uses -performActivityWithSynchronousWaiting:usingBlock:, uses this around its invocation of -runModalSavePanelForSaveOperation:delegate:didSaveSelector: or -saveToURL:ofType:forSaveOperation:delegate:didSaveSelector:contextInfo: because both of those methods also use -performActivityWithSynchronousWaiting:usingBlock:. Without the use of this method that inner invocation of -performActivityWithSynchronousWaiting:usingBlock: would wait forever.
*/
- (void)continueActivityUsingBlock:(void (NS_NOESCAPE ^)(void))block API_AVAILABLE(macos(10.7));

/* Invoke the block on the main thread. If the main thread is blocked by an invocation of -performActivityWithSynchronousWaiting:usingBlock: or -performSynchronousFileAccessUsingBlock:, interrupt that blocking to invoke the block, and then resume blocking when the invocation of the block has returned. Invocations of this method always return before the passed-in block is invoked.

You can invoke this method when work is being done on a non-main thread and part of the work must be continued on the main thread. For example, -saveToURL:ofType:forSaveOperation:completionHandler: uses this method when it has just completed the actual writing of the file during asynchronous saving and, to finish the saving operation, must invoke -updateChangeCountWithToken:forSaveOperation: and other methods on the main thread.

This method can of course be invoked on any thread.
*/
- (void)continueAsynchronousWorkOnMainThreadUsingBlock:(void (^)(void))block API_AVAILABLE(macos(10.7));

/* Given a block that will perform file access, wait for any file access scheduled by previous invocations of this method or -performAsynchronousFileAccessUsingBlock: to be complete, and then invoke the block. When the block invocation returns allow the next scheduled file access to to be performed, if any.

Like -performActivityWithSynchronousWaiting:usingBlock: this method's primary use is to wait for asynchronous saving, but in contrast with that method it is only for use to wait for the part of an asynchronous saving operation that actually touches the document's file or values in memory that are in some way relative to the document's file. See the comment for -performAsynchronousFileAccessUsingBlock: for an explanation of why this distinction is important.

In general you should use this method or -performAsynchronousFileAccessUsingBlock: around code that gets or sets values in memory that only make sense in the context of the document file's current state. For example, NSDocument itself consistently uses this mechanism around invocations of these methods:
- -fileType, -fileURL, -fileModificationDate, -autosavedContentsFileURL:, and -isDraft, because you can't reliably make decisions based on a file's location, type, modification date, or draft status when it might be being asynchronously moved, renamed, or changed at that moment.
- -setFileType:, -setFileURL:, -setFileModificationDate:, -setAutosavedContentsFileURL:, and -setDraft:, to make using this mechanism when invoking the previous list of methods meaningful.
- -isDocumentEdited and -hasUnautosavedChanges, because you can't reliably make decisions based on whether the document's contents in memory have been saved to a file when it might be being asynchronously saved at that moment.
- -updateChangeCountWithToken:forSaveOperation: and, sometimes, updateChangeCount:, to make using this mechanism when invoking -isDocumentEdited and -hasUnautosavedChanges meaningful.
- -backupFileURL, since it depends on -fileURL.
 */
- (void)performSynchronousFileAccessUsingBlock:(void (NS_NOESCAPE ^)(void))block API_AVAILABLE(macos(10.7));

/* Do the same sort of thing as -performSynchronousFileAccessUsingBlock:, but without ever blocking the main thread, and perhaps not invoking the block until after the method invocation has returned, though still always on the same thread as the method invocation. The block is passed another block, the file access completion handler, which must be invoked when the file access is complete, though it can be invoked from any thread. This method is for use with file access that might begin on one thread but continue on another before it is complete. saveToURL:ofType:forSaveOperation:completionHandler: for for example uses this method instead of -performSynchronousFileAccessUsingBlock: because if it does asynchronous saving then there is no way for it to actually complete all of its file access before returning from the file access block.

The distinction between entire activities and the file accessing part of activities established by having both activity performing methods and file access performing methods is worthwhile because sometimes it is valuable to perform file access without any risk of waiting for the user to dismiss a modal panel. For example, NSDocument's implementation of -[NSFilePresenter relinquishPresentedItemToWriter:] uses -performAsynchronousFileAccessUsingBlock: to ensure that the uses of -performSynchronousFileAccessUsingBlock: described above wait while another process is moving, renaming, or changing the file. Using -performActivityWithSynchronousWaiting:usingBlock: instead would not be appropriate because that would introduce the possibility of the other process' writing being blocked until the user has dismissed a sheet that is being presented as part of a previously scheduled activity.
*/
- (void)performAsynchronousFileAccessUsingBlock:(void (^)(void (^fileAccessCompletionHandler)(void)))block API_AVAILABLE(macos(10.7));

#pragma mark *** Reverting ***

/* The action of the File menu's Revert to Saved item in a document-based application. The default implementation of this method presents a panel giving the user the opportunity to cancel the operation and, and if the user chooses to continue, makes sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has discarded its changes and then invokes [self revertToContentsOfURL:[self url] ofType:[self fileType] error:outError]. If that returns NO, it presents the error to the user in an document-modal alert panel.

If +autosavesInPlace returns YES and +preservesVersions returns NO, this method throws an exception.
*/
- (IBAction)revertDocumentToSaved:(nullable id)sender;

/* Discard all unsaved document modifications and replace the document's contents by reading a file or file package located by a URL, of a specified type, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the document could not be reverted. The default implementation of this method invokes [self readFromURL:url ofType:typeName error:outError], [self setFileModificationDate:theModificationDate], [self updateChangeCount:NSChangeCleared], and, if the document has an undo manager, [[self undoManager] removeAllActions]. It also deletes autosaved contents files when they have become obsolete.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self revertToSavedFromFile:[url path] ofType:typeName] if -revertToSavedFromFile:ofType: is overridden and the URL uses the "file:" scheme. It still invokes [self updateChangeCount:NSChangeCleared] and [[self undoManager] removeAllActions] in this situation (because the default implementation of -revertToSavedFromFile:ofType: does not; -revertDocumentToSaved: used to, but doesn't any longer).
*/
- (BOOL)revertToContentsOfURL:(NSURL *)url ofType:(NSString *)typeName error:(NSError **)outError;

#pragma mark *** Simple Reading and Writing ***

/* Typically you would override one of the next three methods... */

/* Set the contents of this document by reading from a file or file package located by a URL, of a specified type, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the document could not be read. The default implementation of this method just creates an NSFileWrapper and invokes [self readFromFileWrapper:theFileWrapper ofType:typeName error:outError].

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self readFromFile:[url path] ofType:typeName] if -readFromFile:ofType: is overridden and the URL uses the "file:" scheme.
*/
- (BOOL)readFromURL:(NSURL *)url ofType:(NSString *)typeName error:(NSError **)outError;

/* Set the contents of this document by reading from a file wrapper of a specified type, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the document could not be read. The default implementation of this method invokes [self readFromData:[fileWrapper regularFileContents] ofType:typeName error:outError].

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self loadFileWrapperRepresentation:fileWrapper ofType:typeName] if -loadFileWrapperRepresentation:ofType: is overridden.
*/
- (BOOL)readFromFileWrapper:(NSFileWrapper *)fileWrapper ofType:(NSString *)typeName error:(NSError **)outError;

/* Set the contents of this document by reading from data of a specified type, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the document could not be read. The default implementation of this method throws an exception because at least one of these three reading methods, or every method that may invoke -readFromURL:ofType:error: (!), must be overridden. 

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self loadDataRepresentation:data ofType:typeName] if -loadDataRepresentation:ofType: is overridden.
*/
- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError;

/* Return YES if the document's entire file is loaded into memory, NO otherwise. The default implementation of this method returns YES. You can override this method to return NO if additional data may need to be read from the file. NSDocument may use this value to do things like prevent volume ejection or warn the user when a partially loaded file disappears from the file system.
 */
@property (getter=isEntireFileLoaded, readonly) BOOL entireFileLoaded API_AVAILABLE(macos(10.7));

/* ...and also one of these. */

/* Write the contents of this document to a file or file package located by a URL, formatted to a specified type, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the document could not be written. The default implementation of this method just invokes [self fileWrapperOfType:typeName error:outError], invokes [self unblockUserInteraction], and writes the returned file wrapper to disk.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self writeToFile:[url path] ofType:typeName] if -writeToFile:ofType: is overridden and the URL uses the "file:" scheme.
*/
- (BOOL)writeToURL:(NSURL *)url ofType:(NSString *)typeName error:(NSError **)outError;

/* Create and return a file wrapper that contains the contents of this document, formatted to a specified type, or return nil after setting *outError to an NSError that encapsulates the reason why the file wrapper could not be created. The default implementation of this method just invokes [self dataOfType:typeName], makes a file wrapper for that data, and returns the file wrapper.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self fileWrapperRepresentationOfType:typeName] if -fileWrapperRepresentationOfType: is overridden.
*/
- (nullable NSFileWrapper *)fileWrapperOfType:(NSString *)typeName error:(NSError **)outError;

/* Create and return a data object that contains the contents of this document, formatted to a specified type, or return nil after setting *outError to an NSError that encapsulates the reason why the data object could not be created. The default implementation of this method throws an exception because at least one of these three writing methods, or -writeToURL:ofType:saveOperation:originalContentsURL:error:, must be overridden.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self dataRepresentationOfType:typeName] if -dataRepresentationOfType: is overridden.
*/
- (nullable NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError;

/* If -saveToURL:ofType:forSaveOperation:completionHandler: is writing on a non-main thread because -canAsynchronouslyWriteToURL:ofType:forSaveOperation: has returned YES, but is still blocking the main thread, unblock the main thread. Otherwise, do nothing. For example, the default implementation of -fileWrapperOfType:error: invokes this when it has created the NSFileWrapper to return. Assuming that the NSFileWrapper will not be mutated by subsequent user actions, it is effectively a "snapshot" of the document's contents, and once it is created it is safe to resume handling user events on the main thread, even though some of those user events might change the document's contents before the NSFileWrapper has been safely written. You can invoke this method to make asynchronous saving actually asynchronous if you've overridden -writeSafelyToURL:ofType:forSaveOperation:error:, -writeToURL:forSaveOperation:originalContentsURL:error:, or -writeToURL:ofType:error: in such a way that the invocation of this method done by -writeToURL:ofType:error:'s default implementation won't happen during writing.
*/
- (void)unblockUserInteraction API_AVAILABLE(macos(10.7));

/* Return YES if autosaving is being done right now but nothing bad would happen if it were to be cancelled, NO otherwise. For example, when periodic autosaving is being done just for crash protection, which doesn't really have to be done all of the time, this method returns YES. When autosaving is being done because the document is being closed this method returns NO.

You can use this notion of implicitly cancellable autosaving when implementing a "stop-copying-on-write" model so that your document class' writing code can invoke -unblockUserInteraction relatively quickly during writing on a non-main thread regardless of how much work must actually be done to make the sort of snapshot mentioned in the comment for -unblockUserInteraction. When this method returns YES your writing code can invoke -unblockUserInteraction after having merely recorded the fact that changes to the document model made by the user should first cancel the rest of the writing. Your code that makes changes to the document model then has to actually always do that cancellation first. (This may be a significant amount of work but hopefully less than implementing a copy-on-write model, which is another option for invoking -unblockUserInteraction as quickly as possible.) If your writing code is implicitly cancelled like that it should signal NSCocoaErrorDomain/NSUserCancelledError, the one kind of error that is never actually presented to the user.
*/
@property (readonly) BOOL autosavingIsImplicitlyCancellable API_AVAILABLE(macos(10.7));

#pragma mark *** Advanced Writing ***

/* Write the contents of the document to a file or file package located by a URL, formatted to a specified type, for a particular kind of save operation, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the document could not be written. The default implementation of this method:
    1) Either creates a temporary directory in which the document writing should be done, or renames the old on-disk version of the document, depending on what sort of save operation is being done, whether or not there's already a copy of the document on disk, and the capabilities of the file system to which writing is being done. The details of this are subject to change.
    2) Invokes -writeToURL:ofType:forSaveOperation:originalContentsURL:error:.
    3) Invokes -fileAttributesToWriteToURL:ofType:forSaveOperation:originalContentsURL:error: and writes the returned attributes, if any, to the file. Some attributes from the old on-disk version of the document may be copied at the same time, if applicable.
    4) Moves the just-written file to its final location, or deletes the old on-disk version of the document, and deletes any temporary directories, depending on the same factors listed for step 1.

For documents that return NO from +preservesVersions, the default implementation of this method invokes -keepBackupFile for NSSaveOperation to determine whether or not the old on-disk version of the document, if there was one, should be preserved after being renamed. If -keepBackupFile returns YES, or for documents that return YES from +preservesVersions, the default implementation of this method invokes [self backupFileURL] to determine the location for the old on-disk version of the document; if it returns nil, no backup file will be kept.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self writeWithBackupToFile:[url path] ofType:typeName saveOperation:aSaveOperation] if -writeWithBackupToFile:ofType:saveOperation: is overridden and the URL uses the "file:" scheme. The save operation used in this case will never be one of the autosaving ones; NSSaveToOperation will be used instead.

This method is responsible for doing document writing in a way that minimizes the danger of leaving the disk to which writing is being done in an inconsistent state in the event of an application crash, system crash, hardware failure, power outage, etc. Because it does several different things, and because the things are likely to change in future releases of Mac OS X, it's probably not a good idea to override this method without invoking super (the same was true of -writeWithBackupToFile:ofType:saveOperation:).
*/
- (BOOL)writeSafelyToURL:(NSURL *)url ofType:(NSString *)typeName forSaveOperation:(NSSaveOperationType)saveOperation error:(NSError **)outError;

/* Write the contents of the document to a file or file package located by a URL, formatted to a specified type, for a particular kind of save operation, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the document could not be written. The default implementation of this method merely invokes [self writeToURL:url ofType:typeName error:outError]. You can override this method instead of one of the methods in the "Simple Reading and Writing" section above if your document writing machinery needs access to the on-disk representation of the document version that is about to be overwritten. url will often not be the same value as [self fileURL]. Other times it will not be the same as the URL for the final save destination. Likewise, absoluteOriginalContentsURL will often not be the same value as [self fileURL], though it will be nil for documents that have never been saved. It will point to nothing if the document's on-disk representation has been deleted.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self writeToFile:[url path] ofType:typeName originalFile:[absoluteOriginalContentsURL path] saveOperation:aSaveOperation] if -writeToFile:ofType:originalFile:saveOperation: is overridden and both URLs use the "file:" scheme. The save operation used in this case will never be one of the autosaving ones; NSSaveToOperation will be used instead.
*/
- (BOOL)writeToURL:(NSURL *)url ofType:(NSString *)typeName forSaveOperation:(NSSaveOperationType)saveOperation originalContentsURL:(nullable NSURL *)absoluteOriginalContentsURL error:(NSError **)outError;

/* Given that a file is being saved, return the attributes that should be written to a file or file package located by a URL, formatted to a specified type, for a particular kind of save operation. If not successful, return nil after setting *outError to an NSError that encapsulates the reason why attributes could not be returned. The set of valid file attributes is a subset of those understood by the NSFileManager class. The default implementation of this method returns an empty dictionary for an NSSaveOperation or NSAutosaveInPlaceOperation, or a dictionary with an appropriate NSFileExtensionHidden entry for any other kind of save operation. You can override this method to customize the attributes that are written to document files.

For backward binary compatibility with Mac OS 10.5 and earlier the default implementation of this method instead returns a dictionary with NSFileHFSCreatorCode and NSFileHFSTypeCode entries that have a value of 0 for NSSaveOperation, in applications linked against Mac OS 10.5 or earlier.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self fileAttributesToWriteToFile:[url path] ofType:typeName saveOperation:aSaveOperation] if -fileAttributesToWriteToFile:ofType:saveOperation: is overridden and the URL uses the "file:" scheme. The save operation used in this case will never be one of the autosaving ones; NSSaveToOperation will be used instead.

The default implementation of -[NSDocument writeSafelyToURL:ofType:forSaveOperation:error:] automatically copies important attributes like file permissions, creation date, and Finder info from the old on-disk version of a document to the new one during an NSSaveOperation or NSAutosaveInPlaceOperation. This method is meant to be used just for attributes that need to be written for the first time, for NSSaveAsOperations, NSSaveToOperations, and NSAutosaveAsOperations. Actually, url and absoluteOriginalContentsURL are passed in just for completeness; NSDocument's default implementation for instance doesn't even need to use them.
*/
- (nullable NSDictionary<NSString *, id> *)fileAttributesToWriteToURL:(NSURL *)url ofType:(NSString *)typeName forSaveOperation:(NSSaveOperationType)saveOperation originalContentsURL:(nullable NSURL *)absoluteOriginalContentsURL error:(NSError **)outError;

/* Return YES if the old on-disk version of a document that is being overwritten should be preserved during an NSSaveOperation, NO otherwise. The default implementation of this method returns NO. For applications that return YES from +preservesVersions, this method has no effect.
*/
@property (readonly) BOOL keepBackupFile;

/* Return the URL that will be used when preserving a backup file during an NSSaveOperation or NSAutosaveInPlaceOperation, or nil if the backup file can't be created or isn't needed.

On Mac OS 10.8, document versions can be preserved using a backup file that is stored at the returned URL during safe-saving. Using this backup file for preserving versions is much more efficient because NSDocument is able to use NSFileVersionReplacingByMoving. NSDocument calls this method twice during saving: once before -writeSafelyToURL:ofType:forSaveOperation:error: to check whether NSFileVersionReplacingByMoving will be possible (and to preserve by copying if it's not), and once within that method to discover where to put the backup file.

Implementations of -writeSafelyToURL:ofType:forSaveOperation:error: must check the value returned by this method during NSSaveOperations and NSAutosaveInPlaceOperations and, if the URL is non-nil, move the previous contents of the file that would be overwritten to that location. (The default implementation of -writeSafelyToURL:ofType:forSaveOperation:error: does this.) Custom implementations of -writeSafelyToURL:ofType:forSaveOperation:error: can use -[NSFileManager replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error:] with a backup item name of [[self backupFileURL] lastPathComponent] and the NSFileManagerItemReplacementWithoutDeletingBackupItem option to easily create the backup file. If your custom implementation is unable to keep the backup file, you must override this method and return nil to ensure that the document's file gets correctly preserved before it gets overwritten.

The default implementation of this method returns a non-nil value based off the value of [self fileURL] only if the document's file needs to be preserved prior to saving, or if +preservesVersions returns NO. Otherwise, it returns nil.
*/
@property (nullable, readonly, copy) NSURL *backupFileURL API_AVAILABLE(macos(10.8));

#pragma mark *** Saving ***

/* The action of the File menu's Save item in a document-based application. The default implementation of this method merely invokes [self saveDocumentWithDelegate:nil didSaveSelector:NULL contextInfo:NULL].
*/
- (IBAction)saveDocument:(nullable id)sender;

/* The action of the File menu's Save As... item in a document-based application. The default implementation of this method merely invokes [self runModalSavePanelForSaveOperation:NSSaveAsOperation delegate:nil didSaveSelector:NULL contextInfo:NULL].
*/
- (IBAction)saveDocumentAs:(nullable id)sender;

/* The action of the File menu's Export... item in a document-based application (actually, the menu item goes by different names in different applications; check the Human Interface Guidelines). The default implementation of this method merely invokes [self runModalSavePanelForSaveOperation:NSSaveToOperation delegate:nil didSaveSelector:NULL contextInfo:NULL].
*/
- (IBAction)saveDocumentTo:(nullable id)sender;

/* If a save operation can be performed without further user intervention (at the very least, neither -fileURL nor -fileType return nil), then save the document. Otherwise, present a save panel to the user and then save the document if the user OKs the panel. When saving is completed or has been cancelled, send the message selected by didSaveSelector to the delegate, with the contextInfo as the last argument. The method selected by didSaveSelector must have the same signature as:

    - (void)document:(NSDocument *)document didSave:(BOOL)didSaveSuccessfully contextInfo:(void *)contextInfo;

The default implementation of this method first makes sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has committed its changes, then invokes [self runModalSavePanelForSaveOperation:NSSaveAsOperation delegate:delegate didSaveSelector:didSaveSelector contextInfo:inContextInfo] right away if a save panel must be presented. Otherwise, it may present a panel asking the user to make a decision for any of the following situations:
    1) The document's file or file package was modified by something other than the current application since it was opened or most recently saved.
    2) The document's file or file package was moved or renamed since it was opened or most recently saved.
    3) The document's file or file package was deleted or trashed since it was opened or most recently saved.
The list of conditions for which NSDocument checks is subject to change. Regardless, the default implementation of this method always invokes either -runModalSavePanelForSaveOperation:delegate:didSaveSelector:contextInfo: or -saveToURL:ofType:forSaveOperation:delegate:didSaveSelector:contextInfo: unless the user cancels one of the panels it presents.
*/
- (void)saveDocumentWithDelegate:(nullable id)delegate didSaveSelector:(nullable SEL)didSaveSelector contextInfo:(nullable void *)contextInfo;

/* Present a save panel to the user, then try to save the document if the user OKs the panel. When saving is completed, regardless of success or failure, or has been cancelled, send the message selected by didSaveSelector to the delegate, with the contextInfo as the last argument. The method selected by didSaveSelector must have the same signature as:

    - (void)document:(NSDocument *)document didSave:(BOOL)didSaveSuccessfully contextInfo:(void *)contextInfo;

The default implementation of this method first makes sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has committed its changes, then creates a save panel, adds a standard "file format" accessory view if there is more than one file type for the user to choose from and [self shouldRunSavePanelWithAccessoryView] returns YES, sets various attributes of the panel, invokes [self prepareSavePanel:theSavePanel] to provide an opportunity for customization, then presents the panel. If the user OKs the panel -saveToURL:ofType:forSaveOperation:delegate:didSaveSelector:contextInfo: is invoked.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method instead invokes [self saveToFile:nil saveOperation:saveOperation delegate:delegate didSaveSelector:contextInfo:] if -saveToFile:saveOperation:delegate:didSaveSelector:contextInfo: is overridden, even if the user cancels the panel (because that's what 10.3 did).
*/
- (void)runModalSavePanelForSaveOperation:(NSSaveOperationType)saveOperation delegate:(nullable id)delegate didSaveSelector:(nullable SEL)didSaveSelector contextInfo:(nullable void *)contextInfo;

/* Return YES if -runModalSavePanelForSaveOperation:delegate:didSaveSelector:contextInfo: should add NSDocument's standard file format accessory view to the save panel, NO otherwise. The default implementation of this method returns YES. You can override this method to prevent NSDocument from adding an accessory view to the save panel so that your application can add its own when -prepareSavePanel is invoked.
*/
@property (readonly) BOOL shouldRunSavePanelWithAccessoryView;

/* Given the save panel that -runModalSavePanelForSaveOperation:delegate:didSaveSelector:contextInfo: is about to present to the user, make any final changes before it is presented, and return YES for success. Return NO for failure, to cancel the save operation. The default implementation of this method just returns YES.
*/
- (BOOL)prepareSavePanel:(NSSavePanel *)savePanel;

/* Return YES if a save panel has been presented by this document and the user chose to hide the name extension of the file that was selected in that save panel, NO otherwise.
*/
@property (readonly) BOOL fileNameExtensionWasHiddenInLastRunSavePanel;

/* If a save panel has been presented by this document, and a choice of file type was presented in that panel, return the name of the file type that the user chose.
*/
@property (nullable, readonly, copy) NSString *fileTypeFromLastRunSavePanel;

/* Save the contents of the document to a file or file package located by a URL, formatted to a specified type, for a particular kind of save operation. When saving is completed, regardless of success or failure, send the message selected by didSaveSelector to the delegate, with the contextInfo as the last argument. The method selected by didSaveSelector must have the same signature as:

    - (void)document:(NSDocument *)document didSave:(BOOL)didSaveSuccessfully contextInfo:(void *)contextInfo;

The default implementation of this method first makes sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has committed its changes (except for autosave operations), then invokes [self saveToURL:url ofType:typeName forSaveOperation:saveOperation completionHandler:aCompletionHandler] and, if that completion handler is passed an NSError when it is invoked, presents the error to the user in a document-modal panel before messaging the delegate.
*/
- (void)saveToURL:(NSURL *)url ofType:(NSString *)typeName forSaveOperation:(NSSaveOperationType)saveOperation delegate:(nullable id)delegate didSaveSelector:(nullable SEL)didSaveSelector contextInfo:(nullable void *)contextInfo;

/* Save the contents of the document to a file or file package located by a URL, formatted to a specified type, for a particular kind of save operation, and invoke the passed-in completion handler at some point in the future, perhaps after the method invocation has returned. The completion handler must be invoked on the main thread. If successful, pass a nil error. If not successful, pass an NSError that encapsulates the reason why the document could not be saved.
 
 The default implementation of this method invokes [self canAsynchronouslyWriteToURL:url ofType:typeName forSaveOperation:saveOperation]. If writing can't be done concurrently, it invokes [self writeSafelyToURL:url ofType:typeName forSaveOperation:saveOperation error:&anError] on the main thread thread. If writing can be done concurrently, it invokes that method on a different thread, but blocking the main thread until something invokes -unblockUserInteraction. Either way, if -writeSafelyToURL:ofType:forSaveOperation:error: returns YES, it also invokes some combination of -setFileModificationDate:, -setFileType:, -setFileURL:, -updateChangeCount:, and -setAutosavedContentsFileURL:, as appropriate for the kind of save operation, on the main thread. It also updates information that -saveDocumentWithDelegate:didSaveSelector:contextInfo: uses to check for modification, renaming, moving, deleting, and trashing of open documents, and deletes autosaved contents files when they have become obsolete. You can override this method to do things that need to be done before or after any save operation but, because this method does several different things, and because the things are likely to change in future releases of Mac OS X, it's probably not a good idea to override this method without invoking super.

For backward binary compatibility with Mac OS 10.6 and earlier, the default implementation of this method instead invokes [self saveToURL:url ofType:typeName forSaveOperation:saveOperation error:&anError] if that method is overridden and this one is not, and passes any error to the completion handler.
*/
- (void)saveToURL:(NSURL *)url ofType:(NSString *)typeName forSaveOperation:(NSSaveOperationType)saveOperation completionHandler:(void (^)(NSError * _Nullable errorOrNil))completionHandler API_AVAILABLE(macos(10.7));

/* Return whether the receiver can concurrently write to a file or file package located by a URL, formatted for a specific type, for a specific kind of save operation. The default implementation of this method returns NO. You are strongly encouraged to override it and make it return YES, after making sure your overrides of -write... methods can be safely invoked on a non-main thread, and making sure that -unblockUserInteraction will be invoked at some appropriate time during writing.
*/
- (BOOL)canAsynchronouslyWriteToURL:(NSURL *)url ofType:(NSString *)typeName forSaveOperation:(NSSaveOperationType)saveOperation API_AVAILABLE(macos(10.7));

#pragma mark *** Autosaving ***

/* Return YES if saving the user's changes to the document file is probably what they want, or NO after setting *outError to an NSError that encapsulates the reason why it might not be. The default implementation of this method checks for documents that have not been changed in a while ("a while" is subject to change) or are saved in folders where the user typically does not edit documents (the ~/Downloads folder, for example; also subject to change). When it senses one of those cases it returns NO with an NSError that has recovery options like "Duplicate," "Cancel", and "Unlock."

In an application that has adopted Mac OS 10.7's new document model by overriding +autosavesInPlace and making it return YES, you can override this method to customize the autosaving safety checking that NSDocument does by default. You can remove NSDocument's default checking by overriding this method and not invoking super. You can add to NSDocument's default checking by invoking super and then doing your own checking if [super checkAutosavingSafetyAndReturnError:] did not signal an error. For example, TextEdit overrides this method to ask the user what to do when opening a document file has been lossy and overwriting that file might therefore be lossy. Other combinations of your override invoking super are possible.

When autosaving in place is turned on an NSDocument may invoke this method when it receives notification from its NSUndoManager that the user changed the document, or undid or redid a change. (Or it may not. NSDocument does not invoke this method for scripted changes or after some kinds of reverting.) If an error is returned it presents the error to the user, allowing the user to choose a recovery option. If the error is recovered from then it repeats this until no error is signalled, to make sure that all checks have been done. This means that when you signal an error and the user's choice of recovery option indicates that they have seen and disregarded a safety concern you must record that fact, and not do that particular safety check again. Once all errors are recovered from NSDocument continues on by invoking -updateChangeCount:, which it has done in response to NSUndoManager notifications since Mac OS 10.0. If an error is not recovered from then NSDocument invokes -[NSUndoManager undo] or -[NSUndoManager redo] to roll back the change. So, some of the NSError recovery options you let the user select, like NSDocument's own "Duplicate and "Cancel," should indicate failed recovery and that will cause the document to be unchanged afterward.
*/
- (BOOL)checkAutosavingSafetyAndReturnError:(NSError **)outError API_AVAILABLE(macos(10.7));

/* Schedule periodic autosaving for the purpose of crash protection. The default implementation of this method generally checks to see if autosaving is turned on and if so, and if [self hasUnautosavedChanges] returns YES, schedules an NSTimer to invoke -autosaveDocumentWithDelegate:didAutosaveSelector:contextInfo: in the future. If [self hasUnautosavedChanges] returns NO it will unschedule any previously scheduled timer. It takes care not to cause -autosaveDocumentWithDelegate:didAutosaveSelector:contextInfo: to be invoked before a previous invocation caused by it has finished. The exact timings it uses are complicated and subject to change in future releases of Mac OS X. You can override this method to control when exactly periodic autosaving happens. It is invoked by -updateChangeCount: and -updateChangeCountWithToken:forSaveOperation:.
*/
- (void)scheduleAutosaving API_AVAILABLE(macos(10.7));

/* Return YES if the document has changes that have not been autosaved, NO otherwise, as determined by the history of previous invocations of -updateChangeCount:. The default implementation of this method returns NO immediately after invocation of -updateChangeCount:NSChangeCleared or -updateChangeCount:NSChangeAutosaved. It will then return YES if subsequent invocations of -updateChangeCount: have recorded a situation in which the document has changes that have not been autosaved. (-updateChangeCount:NSChangeReadOtherContents has no effect on what the default implementation of this method returns.)
*/
@property (readonly) BOOL hasUnautosavedChanges;

/* Autosave the document's contents if it needs autosaving, and then send the message selected by didAutosaveSelector to the delegate, with the contextInfo as the last argument. The method selected by didAutosaveSelector must have the same signature as:

    - (void)document:(NSDocument *)document didAutosave:(BOOL)didAutosaveSuccessfully contextInfo:(void *)contextInfo;

If any error occurs while autosaving, it must be reported to the user, typically in a document-modal alert panel, before the delegate is messaged with succeeded:NO.

The default implementation of this method simply invokes [self autosaveWithImplicitCancellability:YES completionHandler:aCompletionHandler] and then presents to the user any error that occurs.

This method is only invoked by AppKit itself as a result of -scheduleAutosaving being invoked. It is not invoked for any of the purposes of autosaving introduced by Mac OS 10.7's new document model that is triggered by +autosavesInPlace returning YES.
*/
- (void)autosaveDocumentWithDelegate:(nullable id)delegate didAutosaveSelector:(nullable SEL)didAutosaveSelector contextInfo:(nullable void *)contextInfo;

/* Autosave the document's contents at an appropriate location if it needs autosaving, and invoke the passed-in completion handler at some point in the future, perhaps after the method invocation has returned. The completion handler must be invoked on the main thread. If successful, pass a nil error. If not successful, pass an NSError that encapsulates the reason why the document could not be autosaved. autosavingIsImplicitlyCancellable becomes what -autosavingIsImplicitlyCancellable returns while the autosaving is happening.

The default implementation of this method invokes [self hasUnautosavedChanges] and, if that returns YES, figures out where the autosaved document contents should go and invokes [self saveToURL:autosavedDocumentContentsURL ofType:[self autosavingFileType] forSaveOperation:oneOfTheAutosaveOperations completionHandler:aCompletionHandler]. oneOfTheAutosaveOperations will be NSAutosaveAsOperation if the document has been changed with NSChangeDone and the current document should be turned into a draft, NSAutosaveInPlaceOperation if [[self class] autosavesInPlace] returns YES and [self fileURL] returns non-nil, or NSAutosaveElsewhereOperation otherwise. The check of [self fileURL] is because there's no such thing as autosaving "in place" when the document does not yet have a permanent place.
*/
- (void)autosaveWithImplicitCancellability:(BOOL)autosavingIsImplicitlyCancellable completionHandler:(void (^)(NSError * _Nullable errorOrNil))completionHandler API_AVAILABLE(macos(10.7));

/* Return YES if the receiving subclass of NSDocument supports Mac OS 10.7 autosaving in place, NO otherwise. The default implementation of this method returns NO. You can override it and return YES to declare your NSDocument subclass' ability to do Mac OS 10.7 autosaving in place. You should not invoke this method to find out whether autosaving in place is actually being done at any particular moment. You should instead use the NSSaveOperationType parameter passed to your overrides of -save... and -write... methods.
 
AppKit invokes this method at a variety of times, and not always on the main thread. For example, -autosaveWithImplicitCancellability:completionHandler: invokes this as part of determining whether the autosaving will be an NSAutosaveInPlaceOperation instead of an NSAutosaveElsewhereOperation. For another example, -canCloseDocumentWithDelegate:shouldCloseSelector:contextInfo: and NSDocumentController's machinery for handling unsaved changes at application termination time both invoke this as part of determining whether alerts about unsaved changes should be presented to the user.
*/
@property(class, readonly) BOOL autosavesInPlace API_AVAILABLE(macos(10.7));

/* Return YES if the receiving subclass of NSDocument supports Mac OS 10.7 version preservation, NO otherwise. The default implementation of this method returns [self autosavesInPlace]. You can override it and return NO to declare that NSDocument should not preserve old document versions.

Returning NO from this method will disable version browsing and -revertDocumentToSaved:, which rely on version preservation when autosaving in place. Returning YES from this method when +autosavesInPlace returns NO will result in undefined behavior.
 */
@property(class, readonly) BOOL preservesVersions API_AVAILABLE(macos(10.7));

/* The action of the Browse Saved Versions menu item in a document-based application. The default implementation causes the document's main window, specified by [self windowForSheet], to enter the Versions browser.
*/
- (IBAction)browseDocumentVersions:(nullable id)sender API_AVAILABLE(macos(10.8));

/* Whether the receiver is currently displaying the Versions browser. KVO-compliant.
*/
@property (getter=isBrowsingVersions, readonly) BOOL browsingVersions API_AVAILABLE(macos(10.12));

/* If the receiver is currently displaying the Versions browser, cleanly stop browsing versions (which includes waiting for any animations to complete). Then invoke the completion handler on the main thread.
*/
- (void)stopBrowsingVersionsWithCompletionHandler:(void (^ _Nullable)(void))completionHandler API_AVAILABLE(macos(10.12));

/* Return YES if the receiving subclass of NSDocument supports Mac OS 10.8 autosaving of drafts, NO otherwise. The default implementation of this method returns YES for applications linked on or after Mac OS 10.8. You can override it and return YES to declare your NSDocument subclass' ability to do Mac OS 10.8 autosaving of drafts. You can also override it and return NO to opt out of this behavior after linking with 10.8. You should not invoke this method to find out whether autosaving of a draft will be done. Instances of subclasses that return YES from this method should be ready to properly handle save operations with NSAutosaveAsOperation.

AppKit invokes this method at a variety of times. For example, when -updateChangeCount is called with NSChangeDone (without NSChangeDiscardable), NSDocument will the next autosave to use NSAutosaveAsOperation and return the document into a draft.
*/
@property(class, readonly) BOOL autosavesDrafts API_AVAILABLE(macos(10.8));

/* Return the document type that should be used for an autosave operation. The default implementation just returns [self fileType].
*/
@property (nullable, readonly, copy) NSString *autosavingFileType;

/* The location of the most recently autosaved document contents. The default implementation of -setAutosavedContentsFileURL: records the URL and notifies the shared document controller that this document should be autoreopened if the application is quit or crashes before the document is saved. The default implementation of -autosavedContentsFileURL just returns whatever was stored by a previous invocation of the default implementation of -setAutosavedContentsFileURL:.

Starting in Mac OS 10.7 the default implementations of these methods are thread safe enough that -setAutosavedContentsFileURL: being invoked on the main thread while -autosavedContentsFileURL is being invoked on a different thread won't cause a crash. If you override one of these methods you must maintain that thread safety because AppKit itself may invoke -autosavedContentsFileURL on a background thread. (It's still not a good idea to invoke -setAutosavedContentsFileURL: on a non-main thread though.)
*/
@property (nullable, copy) NSURL *autosavedContentsFileURL;

#pragma mark *** Closing ***

/* If there are changes that have not yet been saved to the document's file and saving cannot be done without asking the user first, present a panel to the user informing them that the document is modified and asking if it should be saved. If the user indicates that it should be, then try to save it. When saving is completed, regardless of success or failure, or has been rejected one way or another by the user, send the message selected by shouldCloseSelector to the delegate, with the contextInfo as the last argument. If the document is not modified then just send the message selected by shouldCloseSelector right away. The method selected by shouldCloseSelector must have the same signature as:

    - (void)document:(NSDocument *)document shouldClose:(BOOL)shouldClose contextInfo:(void *)contextInfo;

The default implementation of this method has two rather different behaviors. If [[self class] autosavesInPlace] returns YES and [self fileURL] returns non-nil then it simply invokes [self autosaveWithImplicitCancellability:NO completionHandler:aCompletionHandler] if [self hasUnautosavedChanges] returns YES after making sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has committed its changes. Otherwise it presents a panel giving the user the choice of canceling, discarding changes, or saving. In that case the shouldClose value passed to the delegate will be YES if the document was not modified in the first place, or the user chose to discard modifications, or chose to save and the saving was successful. NO will be passed if the user chose to cancel the operation or saving was unsuccessful. Because -saveDocumentWithDelegate:didSaveSelector:contextInfo: is used, an alert panel will be presented before the delegate is messaged if saving is attempted but does not succeed.
*/
- (void)canCloseDocumentWithDelegate:(id)delegate shouldCloseSelector:(nullable SEL)shouldCloseSelector contextInfo:(nullable void *)contextInfo;

/* Close the document, discarding any as-yet-unsaved modifications. The default implementation of this method first makes sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has discarded its changes, sends each of the document's window controllers a -close message, invokes [[NSDocumentController sharedDocumentController] removeDocument:self], and then invokes [NSFileCoordinator removeFilePresenter:self]. This is typically the method to use to close open documents (instead of just causing them to be deallocated by some other means). It's usually correct to use -canCloseDocumentWithDelegate:shouldCloseSelector:contextInfo: first to make sure that the user's changes have been saved if appropriate.
*/
- (void)close;

#pragma mark *** Duplicating ***

/* The action of the File menu's Duplicate item in a document-based application. The default implementation of this method merely invokes [self duplicateDocumentWithDelegate:nil didDuplicateSelector:NULL contextInfo:NULL].
*/
- (IBAction)duplicateDocument:(nullable id)sender API_AVAILABLE(macos(10.7));

/* Create a new document whose contents are the same as the receiver's and that doesn't yet have a fileURL. When duplicating is completed, regardless of success or failure, or has been rejected one way or another by the user, send the message selected by didDuplicateSelector to the delegate, with the contextInfo as the last argument. The method selected by didDuplicateSelector must have the same signature as:
 
 - (void)document:(NSDocument *)document didDuplicate:(BOOL)didDuplicate contextInfo:(void *)contextInfo;

The default implementation of this method first makes sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has committed its changes, then checks to see if there are recent changes that might have been inadvertent and, if so, presents a panel giving the user the choice of canceling, duplicating, or duplicating then also discarding recent changes. Unless the user cancels duplicating, or if no panel was presented, it then invokes -duplicateAndReturnError:. If the user chose duplicating and discarding it also discards recent changes after duplicating.
*/
- (void)duplicateDocumentWithDelegate:(nullable id)delegate didDuplicateSelector:(nullable SEL)didDuplicateSelector contextInfo:(nullable void *)contextInfo API_AVAILABLE(macos(10.7));

/* Create and return a new document whose contents are the same as the receiver and that doesn't yet have a fileURL, and return it if successful. If not successful, return nil after setting *outError to an NSError that encapsulates the reason why the document could not be duplicated.

The default implementation of this method first uses -writeSafelyToURL:ofType:forSaveOperation:error: to write the document's current contents to a file located in the same directory that is used for the autosaved contents of untitled documents and with the same sort of name, then invokes [[NSDocumentController sharedDocumentController] duplicateDocumentWithContentsOfURL:newContentsURL copying:NO displayName:aDisplayName error:outError]. The display name parameter is either [self displayName] if self has its own name, the display name of the original document if self is a duplicate, or nil if self is untitled.

You can override this method to customize what is done during document duplication, but if your override does not invoke -[NSDocumentController duplicateDocumentWithContentsOfURL:copying:displayName:error:] you must take care to do things that that method does, especially invoking -[NSDocumentController addDocument:] and +[NSFileCoordinator addFilePresenter:].
*/
- (nullable __kindof NSDocument *)duplicateAndReturnError:(NSError **)outError API_AVAILABLE(macos(10.7));

#pragma mark *** Renaming ***

/* The action of the Rename menu item in a document-based application. The default implementation of this method initiates a renaming session in the window returned by [self windowForSheet].
*/
- (IBAction)renameDocument:(nullable id)sender API_AVAILABLE(macos(10.8));

#pragma mark *** Moving ***

/* The action of the Move to iCloud… menu item in a document-based application. The default implementation of this method presents an alert, asking the user to confirm the move, then invokes -moveToURL:completionHandler: with a URL in the application's default ubiquity container.
*/
- (IBAction)moveDocumentToUbiquityContainer:(nullable id)sender API_AVAILABLE(macos(10.8));

/* The action of the Move To… menu item in a document-based application. The default implementation of this method merely invokes [self moveDocumentWithCompletionHandler:nil].
*/
- (IBAction)moveDocument:(nullable id)sender API_AVAILABLE(macos(10.8));

/* Present a move panel to the user, then try to save the document if the user OKs the panel. When moving is completed, regardless of success, failure, or cancellation, invoke the given block.

The default implementation of this method first makes sure that any editor registered using Cocoa Binding's NSEditorRegistration protocol has committed its changes if necessary. Then, if [self fileURL] is non-nil, it creates and presents a move panel. If the user OKs the panel, -moveToURL:completionHandler: is invoked. If a file already exists at the location the user chooses, the user will be asked to choose between replacing that file, renaming the current document, or canceling. If [self fileURL] is nil, then this method will instead invoke [self runModalSavePanelForSaveOperation:NSSaveAsOperation delegate:didSaveSelector:contextInfo:].
*/
- (void)moveDocumentWithCompletionHandler:(void (^ _Nullable)(BOOL didMove))completionHandler API_AVAILABLE(macos(10.8));

/* Move the document's file to the given URL and invoke the passed-in completion handler at some point in the future, perhaps after the method invocation has returned. The completion handler must be invoked on the main thread. If successful, pass a nil error. If not successful, pass an NSError that encapsulates the reason why the document could not be moved.

The default implementation of this method does a coordinated move of the file at [self fileURL] to the given URL, replacing any file that may currently exist at that URL, and invokes -setFileURL: if the operation is successful. If [self fileURL] is nil, then this method will instead invoke [self saveToURL:url ofType:[self fileType] forSaveOperation:NSSaveAsOperation completionHandler:aCompletionHandler].
*/
- (void)moveToURL:(NSURL *)url completionHandler:(void (^ _Nullable)(NSError * _Nullable))completionHandler API_AVAILABLE(macos(10.8));

#pragma mark *** Locking ***

/* The actions of the Lock and Unlock menu items in a document-based application. The default implementations of these method invoke -lockDocumentWithCompletionHandler: and -unlockDocumentWithCompletionHandler: respectively.
*/
- (IBAction)lockDocument:(nullable id)sender API_AVAILABLE(macos(10.8));
- (IBAction)unlockDocument:(nullable id)sender API_AVAILABLE(macos(10.8));

/* Lock the document to prevent the user from making further modifications. When locking is completed, regardless of success or failure, invoke the given block.

The default implementation of this method first makes sure that any editor registered using Cocoa Binding's NSEditorRegistration protocol has committed its changes and immediately autosaves the document. If the autosave completes successfully or isn't necessary, this method invokes [self lockWithCompletionHandler:]. When locking succeeds, -isLocked will begin returning YES. Documents that return nil from [self fileURL] cannot be locked.
*/
- (void)lockDocumentWithCompletionHandler:(void (^ _Nullable)(BOOL didLock))completionHandler API_AVAILABLE(macos(10.8));

/* Lock the file at [self fileURL] to prevent further modifications. When locking is completed regardless of success or failure, invoke the given block.

The default implementation of this method enables the "user immutable" flag on the file at [self fileURL].
*/
- (void)lockWithCompletionHandler:(void (^ _Nullable)(NSError * _Nullable))completionHandler API_AVAILABLE(macos(10.8));

/* Unlock the document and allow the user to make modifications to the document. When unlocking is completed, regardless of success or failure, invoke the given block.

The default implementation of this method invokes [self unlockWithCompletionHandler:]. This method also disables autosaving safety checking, meaning that -checkAutosavingSafetyAndReturnError: will no longer be invoked on this document. When unlocking succeeds, -isLocked will being returning NO.
*/
- (void)unlockDocumentWithCompletionHandler:(void (^ _Nullable)(BOOL didUnlock))completionHandler API_AVAILABLE(macos(10.8));

/* Unlock the file at [self fileURL] to allow modifications. When unlocking is completed, regardless of success of failure, invoke the given block.

The default implementation of this method tries to clear the "user immutable" flag and add write permissions (if necessary) to the file at [self fileURL].
*/
- (void)unlockWithCompletionHandler:(void (^ _Nullable)(NSError * _Nullable))completionHandler API_AVAILABLE(macos(10.8));

/* Returns YES when it appears the file at [self fileURL] cannot be written to. The conditions that cause this to return YES are subject to change, but may include the lack of write permissions, the "user immutable" flag, a read-only parent directory or volume, a return value of NO from -checkAutosavingSafetyAndReturnError:. You should not override this method.
*/
@property (getter=isLocked, readonly) BOOL locked API_AVAILABLE(macos(10.8));

#pragma mark *** Printing ***

/* The action of the File menu's Page Setup... item in a document-based application. The default implementation of this method invokes [self printInfo], makes a copy of the print info object, and invokes [self runModalPageLayoutWithPrintInfo:printInfoCopy delegate:aPrivateDelegate didRunSelector:aSelectorForAPrivateMethod contextInfo:privateContextInfo]. If the user OKs the page layout panel, [self shouldChangePrintInfo:printInfoCopy] is invoked. If that returns YES, [self setPrintInfo:printInfoCopy] is invoked.
*/
- (IBAction)runPageLayout:(nullable id)sender;

/* Present a page layout panel to the user. When the page layout panel has been dismissed, regardless of whether the user cancelled or OK'ed it, send the message selected by didRunSelector to the delegate, with the contextInfo as the last argument. The method selected by didRunSelector must have the same signature as:

    - (void)document:(NSDocument *)document didRunPageLayoutAndUserAccepted:(BOOL)userAccepted contextInfo:(void *)contextInfo;

The default implementation of this method creates a page layout panel, invokes [self preparePageLayout:thePageLayoutPanel] to provide an opportunity for customization, then presents the panel.
*/
- (void)runModalPageLayoutWithPrintInfo:(NSPrintInfo *)printInfo delegate:(nullable id)delegate didRunSelector:(nullable SEL)didRunSelector contextInfo:(nullable void *)contextInfo;

/* Given the page layout panel that -runModalPageLayoutWithPrintInfo:delegate:didRunSelector:contextInfo: is about to present to the user, make any final changes before it is presented, and return YES for success. Return NO for failure, to cancel the presentation of the panel. The default implementation of this method just returns YES.
*/
- (BOOL)preparePageLayout:(NSPageLayout *)pageLayout;

/* Given a print info object that is the result of the user OK'ing the page layout panel presented by -runPageLayout:, return YES if the print info should be set in the document, NO otherwise.
*/
- (BOOL)shouldChangePrintInfo:(NSPrintInfo *)newPrintInfo;

/* The print info for the document. The default implementation of -setPrintInfo: records the change as an undoable change, if the document has an undo manager. If the document has no undo manager, it invokes [self updateChangeCount:NSChangeDone].
*/
@property (copy) NSPrintInfo *printInfo;

/* The action of the File menu's Print... item in a document-based application. The default implementation of this method merely invokes [self printDocumentWithSettings:[NSDictionary dictionary] showPrintPanel:YES delegate:nil didPrintSelector:NULL contextInfo:NULL].
*/
- (IBAction)printDocument:(nullable id)sender;

/* Print the document. If showing of the print panel is specified, present it first, and print only if the user OKs the panel. The NSPrintInfo attributes in the passed-in printSettings dictionary should be added to a copy of the document's print info, and the resulting print info should be used for the operation. When printing is completed or has been cancelled, send the message selected by didPrintSelector to the delegate, with the contextInfo as the last argument. The method selected by didPrintSelector must have the same signature as:

    - (void)document:(NSDocument *)document didPrint:(BOOL)didPrintSuccessfully contextInfo:(void *)contextInfo;

The default implementation of this method first makes sure that any editor registered using Cocoa Bindings' NSEditorRegistration protocol has committed its changes, then invokes [self printOperationWithSettings:printSettings error:&anError]. If nil is returned it presents the error to the user in a document-modal panel before messaging the delegate. Otherwise it invokes [thePrintOperation setShowsPrintPanel:showPrintPanel] then [self runModalPrintOperation:thePrintOperation delegate:delegate didRunSelector:didPrintSelector contextInfo:contextInfo].

Starting in OS X 10.6, if the printSettings dictionary has an NSPrintJobDisposition entry whose value is NSPrintSaveJob, while lacking an NSPrintJobSavingURL or NSPrintSavePath entry indicating where the PDF file should be written, then the default implementation of this method will present a save panel asking the user where the PDF file should be saved. Additionally, starting in OS X 10.9, the default implementation of this method will invoke -PDFPrintOperation instead of -printOperationWithSettings:error: in this scenario.

For backward binary compatibility with Mac OS 10.3 and earlier, the default implementation of this method invokes [self printShowingPrintPanel:showPrintPanel] if -printShowingPrintPanel: is overridden. When doing this it uses private functionality to arrange for 1) the print settings to take effect despite the fact that the override of -printShowingPrintPanel: can't possibly know about them, and 2) getting notified when the print operation has been completed, so it can message the delegate at the correct time. Correct messaging of the delegate is necessary for correct handling of the Print Apple event.
*/
- (void)printDocumentWithSettings:(NSDictionary<NSPrintInfoAttributeKey, id> *)printSettings showPrintPanel:(BOOL)showPrintPanel delegate:(nullable id)delegate didPrintSelector:(nullable SEL)didPrintSelector contextInfo:(nullable void *)contextInfo;

/* Create a print operation that can be run to print the document's current contents, and return it if successful. If not successful, return nil after setting *outError to an NSError that encapsulates the reason why the print operation could not be created. The NSPrintInfo attributes in the passed-in printSettings dictionary should be added to a copy of the document's print info, and the resulting print info should be used for the operation. The default implementation of this method does nothing. You must override it to enable printing in your application.
*/
- (nullable NSPrintOperation *)printOperationWithSettings:(NSDictionary<NSPrintInfoAttributeKey, id> *)printSettings error:(NSError **)outError;

/* Run a print operation, possibly with printing UI presented document-modally. When printing is completed, regardless of success or failure, or has been cancelled, send the message selected by didRunSelector to the delegate, with the contextInfo as the last argument. The method selected by didRunSelector must have the same signature as:

    - (void)document:(NSDocument *)document didRunPrintOperation:(BOOL)didPrintSuccessfully contextInfo:(void *)contextInfo;
*/
- (void)runModalPrintOperation:(NSPrintOperation *)printOperation delegate:(nullable id)delegate didRunSelector:(nullable SEL)didRunSelector contextInfo:(nullable void *)contextInfo;

#pragma mark *** PDF Creation ***

/* The action of the File menu's Export As PDF... item in a document-based application. The default implementation of this method merely invokes [self printDocumentWithSettings:@{ NSPrintJobDisposition : NSPrintSaveJob } showPrintPanel:NO delegate:nil didPrintSelector:NULL contextInfo:NULL].
 */
- (IBAction)saveDocumentToPDF:(nullable id)sender API_AVAILABLE(macos(10.9));

/* Create a print operation that can be run to create a PDF representation of the document's current contents, and return it if successful. You typically should not use [self printInfo] when creating this print operation, but you should instead maintain a separate NSPrintInfo instance specifically for creating PDFs. The default implementation of this method simply invokes [self printOperationWithSettings:@{ NSPrintJobDisposition : NSPrintSaveJob } error:NULL], but you are highly encouraged to override it if your document subclass supports creating PDF representations.
 */
@property (readonly, strong) NSPrintOperation *PDFPrintOperation API_AVAILABLE(macos(10.9));

#pragma mark *** Sharing ***

/* If YES, allows this instance to be shared via NSDocumentController's standard Share menu. If NO, the standard Share menu will be disabled when this document is targeted.
 
 By default, this returns the same value as [[self class] autosavesInPlace].
 */
@property (readonly) BOOL allowsDocumentSharing API_AVAILABLE(macos(10.13));

/* Share the document's file using the given NSSharingService. First, ensure the document is in a state where it can be properly shared by the given service by saving or relocating the receiver's file, if necessary. Then perform the given sharing service with 'self.fileURL' as the only item. When sharing is complete or fails, invoke the completion handler indicating whether the operation was successful.
 
 The default implementation of this method first ensures the document is saved by autosaving the document if possible, or else presenting the user with a save panel. If the given sharing service has the name NSSharingServiceNameCloudSharing, then it also ensures that file is located in an appropriate container for sharing (e.g. iCloud Drive). When the sharing service finishes, it displays an errors returned by the NSSharingService, then invokes the completion handler, passing a boolean indicating whether sharing succeeded or not.
 
 If the given sharing service had a delegate already set, the default implementation replaces it with its own delegate in order to get completion notifications. However, all NSSharingServiceDelegate methods will be forwarded to the original delegate as well.
 
 When an NSDocument is the only item being used with NSSharingServicePicker or NSSharingServicePickerTouchBarItem, it will invoke this method instead of calling -[NSSharingService performWithItems:] directly.
 */
- (void)shareDocumentWithSharingService:(NSSharingService *)sharingService completionHandler:(void (^ _Nullable)(BOOL success))completionHandler API_AVAILABLE(macos(10.13));

/* Given the NSSharingServicePicker used by an NSDocumentController-owned Share menu (either an automatically inserted one, or one returned by +[NSDocumentController standardShareMenuItem]), make any final changes before that menu is presented. Specifically, you may choose to set a custom NSSharingServicePickerDelegate to customize the construction of the resulting menu, or provide a custom NSSharingServiceDelegate for the chosen service.
 */
- (void)prepareSharingServicePicker:(NSSharingServicePicker *)sharingServicePicker API_AVAILABLE(macos(10.13));

#pragma mark *** Change Management ***

/* Return YES if the document has changes that have not been saved, NO otherwise, primarily determined by the history of previous invocations of -updateChangeCount:. The default implementation of this method returns NO immediately after invocation of -updateChangeCount:NSChangeCleared. It will then return YES if subsequent invocations of -updateChangeCount: have recorded a situation in which the document has changes that have not been saved. Also, it will always return YES after invocation of -updateChangeCount:NSChangeReadOtherContents, until the next invocation of -updateChangeCount:NSChangeCleared. (-updateChangeCount:NSChangeAutosaved has no effect on what the default implementation of this method returns.) Lastly, because NSDocument implements Cocoa Bindings' NSEditorRegistration protocol, the default implementation will return YES whenever there are registered key-value binding editors.
*/
@property (getter=isDocumentEdited, readonly) BOOL documentEdited;

/* Return YES if the document is in read-only "viewing mode". You may use this information to prevent certain kinds of user actions or changes when the user is viewing an old document version.
*/
@property (getter=isInViewingMode, readonly) BOOL inViewingMode API_AVAILABLE(macos(10.7));

/* Record the fact that a change affecting the values returned by -isDocumentEdited and -hasUnautosavedChanges has occurred. See the comments for NSDocumentChangeType for examples of when this method is invoked by NSDocument's own method implementations. Your application only needs to invoke this method explicitly if it is not taking advantage of NSDocument's built-in undo support. The default implementation of this method also sends all of the document's window controllers -setDocumentEdited: messages when appropriate.
*/
- (void)updateChangeCount:(NSDocumentChangeType)change;

/* Return an object that encapsulates the current record of document change at the beginning of a save operation, for passing to -updateChangeCountWithToken:forSaveOperation: at the end of the save operation. For example, -saveToURL:ofType:forSaveOperation:completionHandler: invokes this method, on the main thread, before it does any actual saving.
*/
- (id)changeCountTokenForSaveOperation:(NSSaveOperationType)saveOperation API_AVAILABLE(macos(10.7));

/* Record the fact that saving has succeeded and update the values returned by methods like -isDocumentEdited and -hasUnautosavedChanges. For example, -saveToURL:ofType:forSaveOperation:completionHandler: invokes this method, on the main thread, when it is done saving. The default implementation of this method also sends all of the document's window controllers -setDocumentEdited: messages when appropriate.

This was method was introduced with support for asynchronous saving in Mac OS 10.7. Because the goal of asynchronous saving is to let the user continue changing the document during save operations, the old mechanism of merely invoke [self updateChangeCount:NSChangeCleared] when saving has succeeded is no longer sufficient.
*/
- (void)updateChangeCountWithToken:(id)changeCountToken forSaveOperation:(NSSaveOperationType)saveOperation API_AVAILABLE(macos(10.7));

/* The document's undo manager. The default implementation of -setUndoManager:, in addition to recording the undo manager, registers the document as an observer of various NSUndoManager notifications so that -updateChangeCount: is invoked as undoable changes are made to the document. The default implementation of -undoManager creates an undo manager if the document does not already have one and -hasUndoManager would return YES.
*/
@property (nullable, strong) NSUndoManager *undoManager;

/* Whether or not the document has an undo manager. The default implementation of -setHasUndoManager: releases the document's current undo manager if it has one before the invocation but is not to have one afterward.
*/
@property BOOL hasUndoManager;

#pragma mark *** Error Presentation ***

/* Methods that are declared by NSResponder and also implemented by NSDocument, even though for historical reasons NSDocument is not a subclass of NSResponder. NSDocument's default implementations are equivalent to NSResponder's while treating the shared NSDocumentController as the "next responder" and forwarding error presentation messages to it, despite the fact that for historical reasons NSDocumentController is not a subclass of NSResponder either.

NSWindowController has overrides of -presentError:modalForWindow:delegate:didPresentSelector:contextInfo: and -presentError: that treat the window controller's document as the "next responder" and forward these messages to it, if the window controller has a document (some do, some don't).

The default implementations of several NSDocument methods invoke -presentError:modalForWindow:delegate:didPresentSelector:contextInfo:.

You can customize the presentation of errors on a per-NSDocument-subclass basis by overriding -willPresentError:. Your override should follow the advice given for overriding of -[NSResponder willPresentError:].
*/
- (void)presentError:(NSError *)error modalForWindow:(NSWindow *)window delegate:(nullable id)delegate didPresentSelector:(nullable SEL)didPresentSelector contextInfo:(nullable void *)contextInfo;
- (BOOL)presentError:(NSError *)error;
- (NSError *)willPresentError:(NSError *)error;

/* Given an NSError that has been returned by an NSDocument method, accommodate the fact that the NSError is not going to be presented to the user and the error is definitely not going to be recovered from. Some NSDocument methods, like those involved in writing, sometimes don't immediately delete temporary files if there is a chance that the error can be recovered from and the operation can continue. To make sure that cleanup is always done you should invoke this method with NSDocument errors that are not going to be passed to one of the presentError:... methods. For example, when NSDocument's implementation of -[NSFilePresenter savePresentedItemChangesWithCompletionHandler:] invokes this method when it invokes -autosaveWithImplicitCancellability:completionHandler: and the completion handler is passed an NSError, because it does not present the error to the user.
*/
- (void)willNotPresentError:(NSError *)error API_AVAILABLE(macos(10.7));

#pragma mark *** Windows and Window Controllers ***

/* Create the user interface for this document, but don't show it yet. The default implementation of this method invokes [self windowNibName], creates a new window controller using the resulting nib name (if it is not nil), specifying this document as the nib file's owner, and then invokes [self addWindowController:theNewWindowController] to attach it. You can override this method to use a custom subclass of NSWindowController or to create more than one window controller right away. NSDocumentController invokes this method when creating or opening new documents.
*/
- (void)makeWindowControllers;

/* Return the name of the nib to be used by -makeWindowControllers. The default implementation returns nil. You can override this method to return the name of a nib in your application's resources; the class of the file's owner in that nib must match the class of this object, and the window outlet of the file's owner should be connected to a window. Virtually every subclass of NSDocument has to override either -makeWindowControllers or -windowNibName.
*/
@property (nullable, readonly, copy) NSNibName windowNibName;

/* Be notified that a window controller will or did load a nib with this document as the nib file's owner. The default implementations of these methods do nothing. You can override these methods to do additional setup during the creation of the document's user interface, especially when there's no other reason to create a custom subclass of NSWindowController, so you're not, and you're simply overriding -windowNibName instead of -makeWindowControllers.
*/
- (void)windowControllerWillLoadNib:(NSWindowController *)windowController;
- (void)windowControllerDidLoadNib:(NSWindowController *)windowController;

/* The setter for the "window" IB outlet of this class. It is invoked automatically during the loading of any nib for which this document is the file's owner, if the file's owner window outlet is connected in the nib. You should not invoke this method directly, and typically you would not override it either.
*/
- (void)setWindow:(nullable NSWindow *)window;

/* Add or remove a window controller from this document's list of attached window controllers, and set the document of the passed-in window controller. You cannot attach a window controller to more than one document at a time. The default implementation of -addWindowController: removes the passed-in window controller from the document to which it is attached, if it is already attached to one. It also ignores redundant invocations. The default implementations of both of these methods send -setDocument: messages to the passed-in window controller (with a nil argument, in the latter case). You wouldn't typically override either of these methods. You typically invoke -addWindowController: once or more from an override of -makeWindowControllers, if you override -makeWindowControllers, and can invoke it at other times in applications that present multiple windows per document to the user.
*/
- (void)addWindowController:(NSWindowController *)windowController;
- (void)removeWindowController:(NSWindowController *)windowController;

/* Present the user interface for this document. The default implementation of this method merely sends -showWindow: messages to each of the attached window controllers. You typically would not invoke or override this method. NSDocumentController invokes this method when creating or opening new documents.
*/
- (void)showWindows;

/* Return an array of all window controllers that have been added to this document with -addWindowController: but not yet removed with -removeWindowController:.
*/
@property (readonly, copy) NSArray<__kindof NSWindowController *> *windowControllers;

/* If the window controller is one whose closing results in the document being closed too, or there is only one window controller attached to the document, do the exact same thing that -canCloseDocumentWithDelegate:shouldCloseSelector:contextInfo: would do. Otherwise send the message selected by shouldCloseSelector to the delegate, with the contextInfo as the last argument, specifying that the window controller should be closed. The method selected by shouldCloseSelector must have the same signature as:

    - (void)document:(NSDocument *)document shouldClose:(BOOL)shouldClose contextInfo:(void *)contextInfo;

The default implementation of this method sends the window controller a -shouldCloseDocument message and, if the result is YES or the current count of window controllers is one, merely invokes [self canCloseDocumentWithDelegate:delegate shouldCloseSelector:shouldCloseSelector contextInfo:contextInfo]. Otherwise it messages the delegate as described above. You can override this method, but typically you would not invoke it. NSWindow invokes this method for any window that has a window controller and a document associated with it, before sending the window delegate any -windowShouldClose: message.
*/
- (void)shouldCloseWindowController:(NSWindowController *)windowController delegate:(nullable id)delegate shouldCloseSelector:(nullable SEL)shouldCloseSelector contextInfo:(nullable void *)contextInfo;

/* Set the name for this document that is fit for presentation to the user. If the value is nil then a name based on -defaultDraftName is used.
*/
- (void)setDisplayName:(nullable NSString *)displayNameOrNil API_AVAILABLE(macos(10.7));

/* Returns the name for this document that is fit for presentation to the user. You can override this method, but overriding -[NSWindowController windowTitleForDocumentDisplayName:] is usually better, because a document's display name is used in error alerts, alerts presented during document saving, the alert that's presented when the user attempts to close a document that has unsaved changes, and save panels (as the default value of the "Save As:" field). In those places the document file's actual name really is what should be used.
*/
@property (copy, null_resettable) NSString *displayName;

/* Return the default draft name for the receiver. The default implementation returns the "Untitled" string for the user's current locale.
 
 NSDocument invokes this in -displayName when the receiver hasn't yet been assigned one and also prior to autosaving with NSAutosaveAsOperation. NSDocument will append a number to this string if there is a already another document or file with the same name. Applications will typically return a name that describes the kind of document. For example, a spreadsheet application may return "Spreadsheet". Alternatively, a document created from a template may choose to return the name of that template, like "Résumé".
*/
- (NSString *)defaultDraftName API_AVAILABLE(macos(10.8));

/* Of the windows associated with this document, return the one most appropriate to use as the parent window of a document-modal sheet. This method may return nil, in which case the invoker should present an application-modal panel. NSDocument's implementation of this method returns the window of the first window controller, or nil if there are no window controllers or if the first window controller has no window.
*/
@property (nullable, readonly, strong) NSWindow *windowForSheet;

#pragma mark *** Types ***

/* Return the names of the types for which this class can be instantiated for the application to play the Editor or Viewer role. The default implementation of this method returns information derived from the application's Info.plist. You must typically override it in document classes that are dynamically loaded from plugins. NSDocumentController uses this method when presenting an open panel and when trying to figure the NSDocument subclass to instantiate when opening a particular type of document.
*/
@property (class, readonly, copy) NSArray<NSString *> *readableTypes;

/* Return the names of the types which this class can save. Typically this includes types for which the application can play the Editor role, plus types than can be merely exported by the application. The default implementation of this method returns information derived from the application's Info.plist. You must typically override it in document classes that are dynamically loaded from plugins.
*/
@property (class, readonly, copy) NSArray<NSString *> *writableTypes;

/* Return YES if instances of this class can be instantiated for the application to play the Editor role, NO otherwise. The default implementation of this method returns information derived from the application's Info.plist. You must typically override it in document classes that are dynamically loaded from plugins. NSDocument uses this method when presenting a save panel; in that situation all types returned by -writableTypes are presented in the standard file format popup, except the ones for which -isNativeType returns NO.
*/
+ (BOOL)isNativeType:(NSString *)type;

/* Return the names of the types to which this document can be saved for a kind of save operation. For every kind of save operation except NSSaveToOperation the returned array must only include types for which the the application can play the Editor role. For NSSaveToOperation the returned array may include types for which the application can only play the Viewer role, and other types that the application can merely export. The default implementation of this method returns [[self class] writableTypes] with, except during NSSaveToOperations, types for which +isNativeType returns NO filtered out.

You can override this method to limit the set of writable types when the document currently contains data that is not representable in all types. For example, you can disallow saving to .rtf files when the document contains an attachment and can only be saved properly to .rtfd files. NSDocument uses this this method during save operations that present save panels, and during scripted save operations that do not. It may be called at additional times in future releases of Mac OS X. 

You can invoke this method when creating a custom save panel accessory view to easily present the same set of types that NSDocument would in its standard file format popup menu.
*/
- (NSArray<NSString *> *)writableTypesForSaveOperation:(NSSaveOperationType)saveOperation;

/* For a specified type, and a particular kind of save operation, return a file name extension that can be appended to a base file name. The default implementation of this method invokes [[NSWorkspace sharedWorkspace] preferredFilenameExtensionForType:typeName] if the type is a UTI or, for backward binary compatibility with Mac OS 10.4 and earlier, invokes [[NSDocumentController sharedDocumentController] fileExtensionsFromType:typeName] and chooses the first file name extension in the returned array if not.

You can override this method to customize the appending of extensions to file names by NSDocument. It's invoked from a variety of places within AppKit itself.
*/
- (nullable NSString *)fileNameExtensionForType:(NSString *)typeName saveOperation:(NSSaveOperationType)saveOperation API_AVAILABLE(macos(10.5));

#pragma mark *** Menu Item Validation ***

/* Conformance to the NSUserInterfaceValidations protocol. NSDocument's implementation of this method conditionally enables menu items for all of the action methods listed in this header file.
*/
- (BOOL)validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)item;

#pragma mark *** Ubiquitous Storage ***

/* Return YES if instances of this class should allow the use of ubiquitous document storage. The default implementation of this method returns YES if the application has a valid ubiquity container entitlement. When this method returns YES, NSDocument may do things like add new menu items and other UI for ubiquitous documents and allow documents to be saved or moved into the default ubiquity container. You can override this method to return NO for document classes that should not include these features. */
@property (class, readonly) BOOL usesUbiquitousStorage API_AVAILABLE(macos(10.8));

@end

@interface NSDocument(NSDeprecated)

#pragma mark *** Backward Compatibility ***

/* Methods that were deprecated in Mac OS 10.6. See the comments above for information about when your overrides of them are still invoked, for backward binary compatibility.
*/
- (BOOL)saveToURL:(NSURL *)url ofType:(NSString *)typeName forSaveOperation:(NSSaveOperationType)saveOperation error:(NSError **)outError API_DEPRECATED("Use -saveToURL:ofType:forSaveOperation:completionHandler: instead", macos(10.4,10.6));

/* Methods that were deprecated in Mac OS 10.4. See the comments above for information about when your overrides of them are still invoked, for backward binary compatibility.
*/
- (nullable NSData *)dataRepresentationOfType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (nullable NSDictionary *)fileAttributesToWriteToFile:(NSString *)fullDocumentPath ofType:(NSString *)documentTypeName saveOperation:(NSSaveOperationType)saveOperationType API_DEPRECATED("", macos(10.1,10.4));
- (nullable NSString *)fileName API_DEPRECATED("", macos(10.0,10.4));
- (nullable NSFileWrapper *)fileWrapperRepresentationOfType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (nullable id)initWithContentsOfFile:(NSString *)absolutePath ofType:(NSString *)typeName API_DEPRECATED("", macos(10.0,10.4));
- (nullable id)initWithContentsOfURL:(NSURL *)url ofType:(NSString *)typeName API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)loadDataRepresentation:(NSData *)data ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)loadFileWrapperRepresentation:(NSFileWrapper *)wrapper ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (void)printShowingPrintPanel:(BOOL)flag API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)readFromFile:(NSString *)fileName ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)readFromURL:(NSURL *)url ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)revertToSavedFromFile:(NSString *)fileName ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)revertToSavedFromURL:(NSURL *)url ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (NSInteger)runModalPageLayoutWithPrintInfo:(NSPrintInfo *)printInfo API_DEPRECATED("", macos(10.0,10.4));
- (void)saveToFile:(NSString *)fileName saveOperation:(NSSaveOperationType)saveOperation delegate:(nullable id)delegate didSaveSelector:(nullable SEL)didSaveSelector contextInfo:(nullable void *)contextInfo API_DEPRECATED("", macos(10.0,10.4));
- (void)setFileName:(nullable NSString *)fileName API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)writeToFile:(NSString *)fileName ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)writeToFile:(NSString *)fullDocumentPath ofType:(NSString *)documentTypeName originalFile:(nullable NSString *)fullOriginalDocumentPath saveOperation:(NSSaveOperationType)saveOperationType API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)writeToURL:(NSURL *)url ofType:(NSString *)type API_DEPRECATED("", macos(10.0,10.4));
- (BOOL)writeWithBackupToFile:(NSString *)fullDocumentPath ofType:(NSString *)documentTypeName saveOperation:(NSSaveOperationType)saveOperationType API_DEPRECATED("", macos(10.0,10.4));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
