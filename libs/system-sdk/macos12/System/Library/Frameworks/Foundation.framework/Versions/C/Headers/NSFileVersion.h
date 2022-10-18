/*
	NSFileVersion.h
	Copyright (c) 2010-2019, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSArray<ObjectType>, NSDate, NSDictionary, NSError, NSString, NSURL, NSPersonNameComponents;

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSFileVersionAddingOptions) {

    /* Whether +addVersionOfItemAtURL:withContentsOfURL:options:error: can move the new version contents file into the version store instead of copying it. Moving is much faster. See the comment for -temporaryDirectoryURLForNewVersionOfItemAtURL: for an example of when this useful.
    */
    NSFileVersionAddingByMoving = 1 << 0

};

typedef NS_OPTIONS(NSUInteger, NSFileVersionReplacingOptions) {

    /* Whether -replaceItemAtURL:options:error: must move the version's contents out of the version store instead of copying it. This is useful when you want to promote a version's contents to a separate file. You wouldn't use this to restore a version of a file.
    */
    NSFileVersionReplacingByMoving = 1 << 0

};

/* Instances of NSFileVersion for the same version of the same file are equal, and instances of NSFileVersion for different versions of the same file are not equal, but the equality of NSFileVersions for different files is undefined. Repeated invocations of the methods that return NSFileVersions do not necessarily return the exact same instance of NSFileVersion.
*/
API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0))
@interface NSFileVersion : NSObject {
@private
    NSURL *_fileURL;
    id _addition;
    id _deadVersionIdentifier;
    id _nonLocalVersion;
    NSURL *_contentsURL;
    BOOL _isBackup;
    NSString *_localizedName;
    NSString *_localizedComputerName;
    NSDate *_modificationDate;
    BOOL _isResolved;
    BOOL _contentsURLIsAccessed;
    id _reserved;
    NSString *_name;
}

/* Return an NSFileVersion that represents the contents of the file located by a URL, or nil if there is no such file.
*/
+ (nullable NSFileVersion *)currentVersionOfItemAtURL:(NSURL *)url;

/* Return an array of NSFileVersions associated with the file located by a URL, or nil if there is no such file. The array never contains an NSFileVersion equal to what +currentVersionOfItemAtURL: would return.
*/
+ (nullable NSArray<NSFileVersion *> *)otherVersionsOfItemAtURL:(NSURL *)url;

/* Return an array of NSFileVersions that represent unresolved conflicts for the file located by a URL, or nil if there is no such file.
*/
+ (nullable NSArray<NSFileVersion *> *)unresolvedConflictVersionsOfItemAtURL:(NSURL *)url;

/* Asynchronously return an array of NSFileVersions associated with the file located by the given URL, or nil if there is no such file or another error occurs. Versions returned by this method do not initially have their contents stored locally on the device, so a download may be required before you are able to access them. File attributes are accessible via -[NSURL getPromisedItemResourceValue:forKey:error:]. You can request a download by performing a coordinated read with NSFileCoordinator on the URL property of the resulting NSFileVersions.
 
When a version is successfully downloaded, its contents are cached locally, and the version will no longer be returned by this method. The version will be returned by +[NSFileVersion otherVersionsOfItemAtURL:] instead, but will retain the same persistentIdentifier value. If the local version is later discarded, future invocations of this method may resume returning the version.
 
If you need to get all versions for a document, both local and non-local, you should use an NSFilePresenter that implements -presentedItemDidGainVersion: and -presentedItemDidLoseVersion: and invoke +[NSFileCoordinator addFilePresenter:], +[NSFileVersion otherVersionsOfItemAtURL:], and this method within a single coordinated read.
*/
+ (void)getNonlocalVersionsOfItemAtURL:(NSURL *)url completionHandler:(void (^)(NSArray<NSFileVersion *> * _Nullable nonlocalFileVersions, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* For a file located by a URL, return the NSFileVersion identified by a persistent identifier of the sort returned by -persistentIdentifier, or nil if the version no longer exists.
*/
+ (nullable NSFileVersion *)versionOfItemAtURL:(NSURL *)url forPersistentIdentifier:(id)persistentIdentifier;

/* Add a new version of the file located by a URL, with the contents coming from a file located by either the same or a different URL, and return a new instance that represents the version if successful. If not successful, return NO after setting *outError to an NSError that encapsulates why not.

You can add versions only on Mac OS X.

When adding or removing versions of a file you should do it as part of a "coordinated" write to the file. The NSFileCoordinator class that you use to do coordinated file access is declared in <Foundation/NSFileCoordinator.h>. Using it properly ensures that NSFilePresenters of the file, or directories that contain the file, receive accurate notifications about versions being added or removed. NSFilePresenter is declared in <Foundation/NSFilePresenter.h>. For example, use -[NSFileCoordinator coordinateWritingItemAtURL:options:error:byAccessor:] when the file URL and the contents url are the same. (NSFileVersion doesn't simply use NSFileCoordinator itself because that would be insufficient when the adding or removing of versions is part of a larger operation that should be treated as one coordinated file access.)
*/
+ (nullable NSFileVersion *)addVersionOfItemAtURL:(NSURL *)url withContentsOfURL:(NSURL *)contentsURL options:(NSFileVersionAddingOptions)options error:(NSError **)outError API_AVAILABLE(macos(10.7)) API_UNAVAILABLE(ios, watchos, tvos);

/* Given a URL, create a new directory that is suitable for using as the container of a new temporary file that you will create and use with NSFileVersionAddingByMoving. This is useful when you want to create a new version of a file out of something other than the file's current contents, for example, the contents in memory of a document that has not yet been saved to its file. You must remove this directory when you are done with it, using -[NSFileManager removeItemAtURL:error:] for example.
*/
+ (NSURL *)temporaryDirectoryURLForNewVersionOfItemAtURL:(NSURL *)url API_AVAILABLE(macos(10.7)) API_UNAVAILABLE(ios, watchos, tvos);

/* The location of the receiver's storage, or possibly nil if the receiver's storage has been deleted. The storage is read-only. The URL will have an arcane path. You must not derive user-presentable text from it.
*/
@property (readonly, copy) NSURL *URL;

/* The user-presentable name of the version, or possibly nil if the receiver's storage has been deleted. This will be different from the user-presentable name of the versioned file if, for example, the file has been renamed since the version was added.
*/
@property (nullable, readonly, copy) NSString *localizedName;

/* The user-presentable name of the computer on which the version was saved, or possibly nil if the receiver's storage has been deleted, or nil if no computer name was recorded. The computer name is guaranteed to have been recorded only if the version is a conflict version. This will be different from that computer's current name if the computer's name has been changed since the version was retrieved from that computer.
*/
@property (nullable, readonly, copy) NSString *localizedNameOfSavingComputer;

/* The name components of the user who created this version of the file. Is nil if the file is not shared or if the current user is the originator.
*/
@property (nullable, readonly, copy) NSPersonNameComponents *originatorNameComponents API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos);

/* The modification date of the version, or possibly nil if the receiver's storage has been deleted.
*/
@property (nullable, readonly, copy) NSDate *modificationDate;

/* An object that can be encoded and, after subsequent decoding, passed to -versionOfItemAtURL:forPersistentIdentifier: to create a new instance of NSFileVersion that is equal to the receiver.
*/
@property (readonly, retain) id<NSCoding> persistentIdentifier;

/* Whether the version was created as a result of the discovery of a conflict between two writers of the versioned file.
*/
@property (readonly, getter=isConflict) BOOL conflict;

/* If the version is a conflict version, whether the conflict has been resolved. If the version is not a conflict version, simply YES.

The operating system's reaction to your setting this to YES is complicated and subject to change in future releases. One result however is that the version won't appear in arrays returned by +unresolvedConflictVersionsOfItemAtURL: anymore, unless setting fails.

Once you have indicated that a conflict has been resolved you cannot make it unresolved again. Setting this to NO causes an exception to be thrown.
*/
@property (getter=isResolved) BOOL resolved;

/* Whether the system is allowed to automatically delete the receiver's storage in the future, at an unpredictable time.

Setting this to YES can fail so you must not depend on discarding for correct operation.

Once you have indicated that a version is discardable you cannot make it undiscardable again. Setting this to NO causes an exception to be thrown.

You cannot make the versioned file itself discardable. Setting the value of this property always throws an exception when sent to the result of invoking +currentVersionOfItemAtURL:.

Versions can be discardable only on Mac OS X.
*/
@property (getter=isDiscardable) BOOL discardable API_AVAILABLE(macos(10.7)) API_UNAVAILABLE(ios, watchos, tvos);

/* Whether the version has local contents. Versions that are returned by +getNonlocalVersionsOfItemAtURL:completionHandler: do not initially have local contents. You can only access their contents, either directly via the URL or by invoking -replaceItemAtURL:options:error:, from within a coordinated read on the NSFileVersion's URL.
*/
@property (readonly) BOOL hasLocalContents API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* Whether the version has a thumbnail image available. Thumbnails for versions from +getNonlocalVersionsOfItemAtURL:completionHandler: may not immediately be available. As soon as it becomes available, this property will change from NO to YES. You can use KVO to be notified of this change. If a thumbnail is available, you can access it using NSURLThumbnailKey or NSURLThumbnailDictionaryKey.
*/
@property (readonly) BOOL hasThumbnail API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* If the passed-in URL locates a file, replace the file with a file whose contents are taken from the version but whose display name is taken from the file. If the passed-in URL does not locate a file then simply write one. If successful, return a URL that locates the resulting file; it may be different from the passed-in URL. The one exception to taking the display name from an existing file is if the version is of a different type than the overwritten file. In that case the file name extension will be taken from the version. (When file name extensions are being hidden in a user-friendly way this is not actually an exception.) If not successful, return NO after setting *outError to an NSError that encapsulates why not.

When you use NSFileVersionReplacingByMoving you remove a version of the file, and should do it as part of a coordinated write to the file. The advice about this for +addVersionOfItemAtURL:withContentsOfURL:options:error: applies here too. When you use it to promote a version to a separate file you actually write to two files, and should do it as part of a coordinated write to two files, using -[NSFileCoordinator coordinateWritingItemAtURL:options:writingItemAtURL:options:error:byAccessor:], most likely using NSFileCoordinatorWritingForReplacing for the file you're promoting the version to.
*/
- (nullable NSURL *)replaceItemAtURL:(NSURL *)url options:(NSFileVersionReplacingOptions)options error:(NSError **)error;

/* Delete the receiver's storage, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates why not. Subsequent invocations of +versionsOfItemAtURL: won't include an NSFileVersion for a successfully removed version.

You cannot use this method to delete the versioned file itself. This method always throws an exception when sent to the result of invoking +currentVersionOfItemAtURL:. Use -[NSFileManager removeItemAtURL:error:] for that instead.

When removing versions of a file you should do it as part of a coordinated write to the file. The advice about this for +addVersionOfItemAtURL:withContentsOfURL:options:error: applies here too.
*/
- (BOOL)removeAndReturnError:(NSError **)outError;

/* Delete all of the versions associated with the file located by a URL, except for the current one, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates why not.

When removing versions of a file you should do it as part of a coordinated write to the file. The advice about this for +addVersionOfItemAtURL:withContentsOfURL:options:error: applies here too.
*/
+ (BOOL)removeOtherVersionsOfItemAtURL:(NSURL *)url error:(NSError **)outError;

@end

NS_ASSUME_NONNULL_END
