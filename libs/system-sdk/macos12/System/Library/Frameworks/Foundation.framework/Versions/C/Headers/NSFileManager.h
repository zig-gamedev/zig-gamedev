/*	NSFileManager.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSEnumerator.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSPathUtilities.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSError.h>
#import <Foundation/NSURL.h>
#import <CoreFoundation/CFBase.h>
#import <dispatch/dispatch.h>

@class NSArray<ObjectType>, NSData, NSDate, NSDirectoryEnumerator<ObjectType>, NSError, NSNumber, NSFileProviderService, NSXPCConnection, NSLock;
@protocol NSFileManagerDelegate;

typedef NSString * NSFileAttributeKey NS_TYPED_EXTENSIBLE_ENUM;
typedef NSString * NSFileAttributeType NS_TYPED_ENUM;
typedef NSString * NSFileProtectionType NS_TYPED_ENUM;

typedef NSString * NSFileProviderServiceName NS_TYPED_EXTENSIBLE_ENUM;

NS_ASSUME_NONNULL_BEGIN

/* Version number where NSFileManager can copy/move/enumerate resources forks correctly. 
*/
#define NSFoundationVersionWithFileManagerResourceForkSupport 412

typedef NS_OPTIONS(NSUInteger, NSVolumeEnumerationOptions) {
    /* The mounted volume enumeration will skip hidden volumes.
     */
    NSVolumeEnumerationSkipHiddenVolumes = 1UL << 1,
    
    /* The mounted volume enumeration will produce file reference URLs rather than path-based URLs.
     */
    NSVolumeEnumerationProduceFileReferenceURLs = 1UL << 2
} API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

typedef NS_OPTIONS(NSUInteger, NSDirectoryEnumerationOptions) {
    /* NSDirectoryEnumerationSkipsSubdirectoryDescendants causes the NSDirectoryEnumerator to perform a shallow enumeration and not descend into directories it encounters.
     */
    NSDirectoryEnumerationSkipsSubdirectoryDescendants = 1UL << 0,
    
    /* NSDirectoryEnumerationSkipsPackageDescendants will cause the NSDirectoryEnumerator to not descend into packages.
     */
    NSDirectoryEnumerationSkipsPackageDescendants      = 1UL << 1,
    
    /* NSDirectoryEnumerationSkipsHiddenFiles causes the NSDirectoryEnumerator to not enumerate hidden files.
     */
    NSDirectoryEnumerationSkipsHiddenFiles             = 1UL << 2,

    /* NSDirectoryEnumerationIncludesDirectoriesPostOrder causes the NSDirectoryEnumerator to enumerate each directory a second time after all of its contained files have been enumerated. Use NSDirectoryEnumerator.isEnumeratingDirectoryPostOrder to differentiate a post-order enumerated directory from a pre-order one.
     */
    NSDirectoryEnumerationIncludesDirectoriesPostOrder API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) = 1UL << 3,
    
    /* NSDirectoryEnumerationProducesRelativePathURLs causes the NSDirectoryEnumerator to always produce file path URLs relative to the directoryURL. This can reduce the size of each URL object returned during enumeration.
     */
    NSDirectoryEnumerationProducesRelativePathURLs API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) = 1UL << 4,
} API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

typedef NS_OPTIONS(NSUInteger, NSFileManagerItemReplacementOptions) {
    /* NSFileManagerItemReplacementUsingNewMetadataOnly causes -replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error: to use metadata from the new item only and not to attempt to preserve metadata from the original item.
     */
    NSFileManagerItemReplacementUsingNewMetadataOnly = 1UL << 0,
    
    /* NSFileManagerItemReplacementWithoutDeletingBackupItem causes -replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error: to leave the backup item in place after a successful replacement. The default behavior is to remove the item.
     */
     NSFileManagerItemReplacementWithoutDeletingBackupItem = 1UL << 1
} API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

typedef NS_ENUM(NSInteger, NSURLRelationship) {
    NSURLRelationshipContains,
    NSURLRelationshipSame,
    NSURLRelationshipOther
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* NSFileManagerUnmountOptions to pass to unmountVolumeAtURL:options:completionHandler: */
typedef NS_OPTIONS(NSUInteger, NSFileManagerUnmountOptions) {
    /* If the volume is on a partitioned disk, unmount all volumes on that disk. Then, eject the disk (if it is ejectable).
     */
    NSFileManagerUnmountAllPartitionsAndEjectDisk = 1UL << 0,
    
    /* Specifies that no UI should accompany the unmount operation. (Otherwise, the unmount UI, if needed, would delay completion of the completionHandler.)
     */
    NSFileManagerUnmountWithoutUI = 1UL << 1,
} API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/* If unmountVolumeAtURL:options:completionHandler: fails, the process identifier of the dissenter can be found in the  NSError's userInfo dictionary with this key */
FOUNDATION_EXPORT NSString *const NSFileManagerUnmountDissentingProcessIdentifierErrorKey API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos); // value is NSNumber containing the process identifier of the dissenter

/* Notification sent after the current ubiquity identity has changed.
*/
extern NSNotificationName const NSUbiquityIdentityDidChangeNotification API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

@interface NSFileManager : NSObject

/* Returns the default singleton instance.
*/
@property (class, readonly, strong) NSFileManager *defaultManager;

/* -mountedVolumeURLsIncludingResourceValuesForKeys:options: returns an NSArray of NSURLs locating the mounted volumes available on the computer. The property keys that can be requested are available in <Foundation/NSURL.h>.
 */
- (nullable NSArray<NSURL *> *)mountedVolumeURLsIncludingResourceValuesForKeys:(nullable NSArray<NSURLResourceKey> *)propertyKeys options:(NSVolumeEnumerationOptions)options API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* This method starts the process of unmounting the volume specified by url. If the volume is encrypted, it is re-locked after being unmounted. The completionHandler will be executed when the operation is complete. If the operation was successful, the block’s errorOrNil argument will be nil; otherwise, errorOrNil will be an error object indicating why the unmount operation failed.
 */
- (void)unmountVolumeAtURL:(NSURL *)url options:(NSFileManagerUnmountOptions)mask completionHandler:(void (^)(NSError * _Nullable errorOrNil))completionHandler API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

/* -contentsOfDirectoryAtURL:includingPropertiesForKeys:options:error: returns an NSArray of NSURLs identifying the the directory entries. If this method returns nil, an NSError will be returned by reference in the 'error' parameter. If the directory contains no entries, this method will return the empty array. When an array is specified for the 'keys' parameter, the specified property values will be pre-fetched and cached with each enumerated URL.
 
    This method always does a shallow enumeration of the specified directory (i.e. it always acts as if NSDirectoryEnumerationSkipsSubdirectoryDescendants has been specified). If you need to perform a deep enumeration, use -[NSFileManager enumeratorAtURL:includingPropertiesForKeys:options:errorHandler:].
 
    If you wish to only receive the URLs and no other attributes, then pass '0' for 'options' and an empty NSArray ('[NSArray array]') for 'keys'. If you wish to have the property caches of the vended URLs pre-populated with a default set of attributes, then pass '0' for 'options' and 'nil' for 'keys'.
 */
- (nullable NSArray<NSURL *> *)contentsOfDirectoryAtURL:(NSURL *)url includingPropertiesForKeys:(nullable NSArray<NSURLResourceKey> *)keys options:(NSDirectoryEnumerationOptions)mask error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));


/* -URLsForDirectory:inDomains: is analogous to NSSearchPathForDirectoriesInDomains(), but returns an array of NSURL instances for use with URL-taking APIs. This API is suitable when you need to search for a file or files which may live in one of a variety of locations in the domains specified.
 */
- (NSArray<NSURL *> *)URLsForDirectory:(NSSearchPathDirectory)directory inDomains:(NSSearchPathDomainMask)domainMask API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* -URLForDirectory:inDomain:appropriateForURL:create:error: is a URL-based replacement for FSFindFolder(). It allows for the specification and (optional) creation of a specific directory for a particular purpose (e.g. the replacement of a particular item on disk, or a particular Library directory.
 
    You may pass only one of the values from the NSSearchPathDomainMask enumeration, and you may not pass NSAllDomainsMask.
 */
- (nullable NSURL *)URLForDirectory:(NSSearchPathDirectory)directory inDomain:(NSSearchPathDomainMask)domain appropriateForURL:(nullable NSURL *)url create:(BOOL)shouldCreate error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* Sets 'outRelationship' to NSURLRelationshipContains if the directory at 'directoryURL' directly or indirectly contains the item at 'otherURL', meaning 'directoryURL' is found while enumerating parent URLs starting from 'otherURL'. Sets 'outRelationship' to NSURLRelationshipSame if 'directoryURL' and 'otherURL' locate the same item, meaning they have the same NSURLFileResourceIdentifierKey value. If 'directoryURL' is not a directory, or does not contain 'otherURL' and they do not locate the same file, then sets 'outRelationship' to NSURLRelationshipOther. If an error occurs, returns NO and sets 'error'.
 */
- (BOOL)getRelationship:(NSURLRelationship *)outRelationship ofDirectoryAtURL:(NSURL *)directoryURL toItemAtURL:(NSURL *)otherURL error:(NSError **)error API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* Similar to -[NSFileManager getRelationship:ofDirectoryAtURL:toItemAtURL:error:], except that the directory is instead defined by an NSSearchPathDirectory and NSSearchPathDomainMask. Pass 0 for domainMask to instruct the method to automatically choose the domain appropriate for 'url'. For example, to discover if a file is contained by a Trash directory, call [fileManager getRelationship:&result ofDirectory:NSTrashDirectory inDomain:0 toItemAtURL:url error:&error].
 */
- (BOOL)getRelationship:(NSURLRelationship *)outRelationship ofDirectory:(NSSearchPathDirectory)directory inDomain:(NSSearchPathDomainMask)domainMask toItemAtURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* createDirectoryAtURL:withIntermediateDirectories:attributes:error: creates a directory at the specified URL. If you pass 'NO' for withIntermediateDirectories, the directory must not exist at the time this call is made. Passing 'YES' for withIntermediateDirectories will create any necessary intermediate directories. This method returns YES if all directories specified in 'url' were created and attributes were set. Directories are created with attributes specified by the dictionary passed to 'attributes'. If no dictionary is supplied, directories are created according to the umask of the process. This method returns NO if a failure occurs at any stage of the operation. If an error parameter was provided, a presentable NSError will be returned by reference.
 */
- (BOOL)createDirectoryAtURL:(NSURL *)url withIntermediateDirectories:(BOOL)createIntermediates attributes:(nullable NSDictionary<NSFileAttributeKey, id> *)attributes error:(NSError **)error API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* createSymbolicLinkAtURL:withDestinationURL:error: returns YES if the symbolic link that point at 'destURL' was able to be created at the location specified by 'url'. 'destURL' is always resolved against its base URL, if it has one. If 'destURL' has no base URL and it's 'relativePath' is indeed a relative path, then a relative symlink will be created. If this method returns NO, the link was unable to be created and an NSError will be returned by reference in the 'error' parameter. This method does not traverse a terminal symlink.
 */
- (BOOL)createSymbolicLinkAtURL:(NSURL *)url withDestinationURL:(NSURL *)destURL error:(NSError **)error API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Instances of NSFileManager may now have delegates. Each instance has one delegate, and the delegate is not retained. In versions of Mac OS X prior to 10.5, the behavior of calling [[NSFileManager alloc] init] was undefined. In Mac OS X 10.5 "Leopard" and later, calling [[NSFileManager alloc] init] returns a new instance of an NSFileManager.
 */
@property (nullable, assign) id <NSFileManagerDelegate> delegate API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* setAttributes:ofItemAtPath:error: returns YES when the attributes specified in the 'attributes' dictionary are set successfully on the item specified by 'path'. If this method returns NO, a presentable NSError will be provided by-reference in the 'error' parameter. If no error is required, you may pass 'nil' for the error.
 
    This method replaces changeFileAttributes:atPath:.
 */
- (BOOL)setAttributes:(NSDictionary<NSFileAttributeKey, id> *)attributes ofItemAtPath:(NSString *)path error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* createDirectoryAtPath:withIntermediateDirectories:attributes:error: creates a directory at the specified path. If you pass 'NO' for createIntermediates, the directory must not exist at the time this call is made. Passing 'YES' for 'createIntermediates' will create any necessary intermediate directories. This method returns YES if all directories specified in 'path' were created and attributes were set. Directories are created with attributes specified by the dictionary passed to 'attributes'. If no dictionary is supplied, directories are created according to the umask of the process. This method returns NO if a failure occurs at any stage of the operation. If an error parameter was provided, a presentable NSError will be returned by reference.
 
    This method replaces createDirectoryAtPath:attributes:
 */
- (BOOL)createDirectoryAtPath:(NSString *)path withIntermediateDirectories:(BOOL)createIntermediates attributes:(nullable NSDictionary<NSFileAttributeKey, id> *)attributes error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* contentsOfDirectoryAtPath:error: returns an NSArray of NSStrings representing the filenames of the items in the directory. If this method returns 'nil', an NSError will be returned by reference in the 'error' parameter. If the directory contains no items, this method will return the empty array.
 
    This method replaces directoryContentsAtPath:
 */
- (nullable NSArray<NSString *> *)contentsOfDirectoryAtPath:(NSString *)path error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* subpathsOfDirectoryAtPath:error: returns an NSArray of NSStrings representing the filenames of the items in the specified directory and all its subdirectories recursively. If this method returns 'nil', an NSError will be returned by reference in the 'error' parameter. If the directory contains no items, this method will return the empty array.
 
    This method replaces subpathsAtPath:
 */
- (nullable NSArray<NSString *> *)subpathsOfDirectoryAtPath:(NSString *)path error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* attributesOfItemAtPath:error: returns an NSDictionary of key/value pairs containing the attributes of the item (file, directory, symlink, etc.) at the path in question. If this method returns 'nil', an NSError will be returned by reference in the 'error' parameter. This method does not traverse a terminal symlink.
 
    This method replaces fileAttributesAtPath:traverseLink:.
 */
- (nullable NSDictionary<NSFileAttributeKey, id> *)attributesOfItemAtPath:(NSString *)path error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* attributesOfFileSystemForPath:error: returns an NSDictionary of key/value pairs containing the attributes of the filesystem containing the provided path. If this method returns 'nil', an NSError will be returned by reference in the 'error' parameter. This method does not traverse a terminal symlink.
 
    This method replaces fileSystemAttributesAtPath:.
 */
- (nullable NSDictionary<NSFileAttributeKey, id> *)attributesOfFileSystemForPath:(NSString *)path error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* createSymbolicLinkAtPath:withDestination:error: returns YES if the symbolic link that point at 'destPath' was able to be created at the location specified by 'path'. If this method returns NO, the link was unable to be created and an NSError will be returned by reference in the 'error' parameter. This method does not traverse a terminal symlink.
 
    This method replaces createSymbolicLinkAtPath:pathContent:
 */
- (BOOL)createSymbolicLinkAtPath:(NSString *)path withDestinationPath:(NSString *)destPath error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* destinationOfSymbolicLinkAtPath:error: returns an NSString containing the path of the item pointed at by the symlink specified by 'path'. If this method returns 'nil', an NSError will be returned by reference in the 'error' parameter.
 
    This method replaces pathContentOfSymbolicLinkAtPath:
 */
- (nullable NSString *)destinationOfSymbolicLinkAtPath:(NSString *)path error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* These methods replace their non-error returning counterparts below. See the NSFileManagerDelegate protocol below for methods that are dispatched to the NSFileManager instance's delegate.
 */
- (BOOL)copyItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (BOOL)moveItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (BOOL)linkItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (BOOL)removeItemAtPath:(NSString *)path error:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* These methods are URL-taking equivalents of the four methods above. Their delegate methods are defined in the NSFileManagerDelegate protocol below.
 */
- (BOOL)copyItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (BOOL)moveItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (BOOL)linkItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (BOOL)removeItemAtURL:(NSURL *)URL error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* trashItemAtURL:resultingItemURL:error: returns YES if the item at 'url' was successfully moved to a Trash. Since the operation may require renaming the file to avoid collisions, it also returns by reference the resulting URL that the item was moved to. If this method returns NO, the item was not moved and an NSError will be returned by reference in the 'error' parameter.

    To easily discover if an item is in the Trash, you may use [fileManager getRelationship:&result ofDirectory:NSTrashDirectory inDomain:0 toItemAtURL:url error:&error] && result == NSURLRelationshipContains.
 */
- (BOOL)trashItemAtURL:(NSURL *)url resultingItemURL:(NSURL * _Nullable * _Nullable)outResultingURL error:(NSError **)error API_AVAILABLE(macos(10.8), ios(11.0)) API_UNAVAILABLE(watchos, tvos);

/* The following methods are deprecated on Mac OS X 10.5. Their URL-based and/or error-returning replacements are listed above.
 */
- (nullable NSDictionary *)fileAttributesAtPath:(NSString *)path traverseLink:(BOOL)yorn API_DEPRECATED("Use -attributesOfItemAtPath:error: instead", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (BOOL)changeFileAttributes:(NSDictionary *)attributes atPath:(NSString *)path API_DEPRECATED("Use -setAttributes:ofItemAtPath:error: instead", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (nullable NSArray *)directoryContentsAtPath:(NSString *)path API_DEPRECATED("Use -contentsOfDirectoryAtPath:error: instead", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (nullable NSDictionary *)fileSystemAttributesAtPath:(NSString *)path API_DEPRECATED("Use -attributesOfFileSystemForPath:error: instead", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (nullable NSString *)pathContentOfSymbolicLinkAtPath:(NSString *)path API_DEPRECATED("Use -destinationOfSymbolicLinkAtPath:error:", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (BOOL)createSymbolicLinkAtPath:(NSString *)path pathContent:(NSString *)otherpath API_DEPRECATED("Use -createSymbolicLinkAtPath:error: instead", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (BOOL)createDirectoryAtPath:(NSString *)path attributes:(NSDictionary *)attributes API_DEPRECATED("Use -createDirectoryAtPath:withIntermediateDirectories:attributes:error: instead", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));

#if TARGET_OS_OSX || TARGET_OS_MACCATALYST
- (BOOL)linkPath:(NSString *)src toPath:(NSString *)dest handler:(nullable id)handler API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (BOOL)copyPath:(NSString *)src toPath:(NSString *)dest handler:(nullable id)handler API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (BOOL)movePath:(NSString *)src toPath:(NSString *)dest handler:(nullable id)handler API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (BOOL)removeFileAtPath:(NSString *)path handler:(nullable id)handler API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
#endif

/* Process working directory management. Despite the fact that these are instance methods on NSFileManager, these methods report and change (respectively) the working directory for the entire process. Developers are cautioned that doing so is fraught with peril.
 */
@property (readonly, copy) NSString *currentDirectoryPath;
- (BOOL)changeCurrentDirectoryPath:(NSString *)path;

/* The following methods are of limited utility. Attempting to predicate behavior based on the current state of the filesystem or a particular file on the filesystem is encouraging odd behavior in the face of filesystem race conditions. It's far better to attempt an operation (like loading a file or creating a directory) and handle the error gracefully than it is to try to figure out ahead of time whether the operation will succeed.
 */
- (BOOL)fileExistsAtPath:(NSString *)path;
- (BOOL)fileExistsAtPath:(NSString *)path isDirectory:(nullable BOOL *)isDirectory;
- (BOOL)isReadableFileAtPath:(NSString *)path;
- (BOOL)isWritableFileAtPath:(NSString *)path;
- (BOOL)isExecutableFileAtPath:(NSString *)path;
- (BOOL)isDeletableFileAtPath:(NSString *)path;

/* -contentsEqualAtPath:andPath: does not take into account data stored in the resource fork or filesystem extended attributes.
 */
- (BOOL)contentsEqualAtPath:(NSString *)path1 andPath:(NSString *)path2;

/* displayNameAtPath: returns an NSString suitable for presentation to the user. For directories which have localization information, this will return the appropriate localized string. This string is not suitable for passing to anything that must interact with the filesystem.
 */
- (NSString *)displayNameAtPath:(NSString *)path;

/* componentsToDisplayForPath: returns an NSArray of display names for the path provided. Localization will occur as in displayNameAtPath: above. This array cannot and should not be reassembled into an usable filesystem path for any kind of access.
 */
- (nullable NSArray<NSString *> *)componentsToDisplayForPath:(NSString *)path;

/* enumeratorAtPath: returns an NSDirectoryEnumerator rooted at the provided path. If the enumerator cannot be created, this returns NULL. Because NSDirectoryEnumerator is a subclass of NSEnumerator, the returned object can be used in the for...in construct.
 */
- (nullable NSDirectoryEnumerator<NSString *> *)enumeratorAtPath:(NSString *)path;

/* enumeratorAtURL:includingPropertiesForKeys:options:errorHandler: returns an NSDirectoryEnumerator rooted at the provided directory URL. The NSDirectoryEnumerator returns NSURLs from the -nextObject method. The optional 'includingPropertiesForKeys' parameter indicates which resource properties should be pre-fetched and cached with each enumerated URL. The optional 'errorHandler' block argument is invoked when an error occurs. Parameters to the block are the URL on which an error occurred and the error. When the error handler returns YES, enumeration continues if possible. Enumeration stops immediately when the error handler returns NO.

    If you wish to only receive the URLs and no other attributes, then pass '0' for 'options' and an empty NSArray ('[NSArray array]') for 'keys'. If you wish to have the property caches of the vended URLs pre-populated with a default set of attributes, then pass '0' for 'options' and 'nil' for 'keys'.
 */
- (nullable NSDirectoryEnumerator<NSURL *> *)enumeratorAtURL:(NSURL *)url includingPropertiesForKeys:(nullable NSArray<NSURLResourceKey> *)keys options:(NSDirectoryEnumerationOptions)mask errorHandler:(nullable BOOL (^)(NSURL *url, NSError *error))handler API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* subpathsAtPath: returns an NSArray of all contents and subpaths recursively from the provided path. This may be very expensive to compute for deep filesystem hierarchies, and should probably be avoided.
 */
- (nullable NSArray<NSString *> *)subpathsAtPath:(NSString *)path;

/* These methods are provided here for compatibility. The corresponding methods on NSData which return NSErrors should be regarded as the primary method of creating a file from an NSData or retrieving the contents of a file as an NSData.
 */
- (nullable NSData *)contentsAtPath:(NSString *)path;
- (BOOL)createFileAtPath:(NSString *)path contents:(nullable NSData *)data attributes:(nullable NSDictionary<NSFileAttributeKey, id> *)attr;

/* fileSystemRepresentationWithPath: returns an array of characters suitable for passing to lower-level POSIX style APIs. The string is provided in the representation most appropriate for the filesystem in question.
 */
- (const char *)fileSystemRepresentationWithPath:(NSString *)path NS_RETURNS_INNER_POINTER;

/* stringWithFileSystemRepresentation:length: returns an NSString created from an array of bytes that are in the filesystem representation.
 */
- (NSString *)stringWithFileSystemRepresentation:(const char *)str length:(NSUInteger)len;

/* -replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error: is for developers who wish to perform a safe-save without using the full NSDocument machinery that is available in the AppKit.
 
    The `originalItemURL` is the item being replaced.
    `newItemURL` is the item which will replace the original item. This item should be placed in a temporary directory as provided by the OS, or in a uniquely named directory placed in the same directory as the original item if the temporary directory is not available.
    If `backupItemName` is provided, that name will be used to create a backup of the original item. The backup is placed in the same directory as the original item. If an error occurs during the creation of the backup item, the operation will fail. If there is already an item with the same name as the backup item, that item will be removed. The backup item will be removed in the event of success unless the `NSFileManagerItemReplacementWithoutDeletingBackupItem` option is provided in `options`.
    For `options`, pass `0` to get the default behavior, which uses only the metadata from the new item while adjusting some properties using values from the original item. Pass `NSFileManagerItemReplacementUsingNewMetadataOnly` in order to use all possible metadata from the new item.
 */
- (BOOL)replaceItemAtURL:(NSURL *)originalItemURL withItemAtURL:(NSURL *)newItemURL backupItemName:(nullable NSString *)backupItemName options:(NSFileManagerItemReplacementOptions)options resultingItemURL:(NSURL * _Nullable * _Nullable)resultingURL error:(NSError **)error API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));


/* Changes whether the item for the specified URL is ubiquitous and moves the item to the destination URL. When making an item ubiquitous, the destination URL must be prefixed with a URL from -URLForUbiquityContainerIdentifier:. Returns YES if the change is successful, NO otherwise.
 */
- (BOOL)setUbiquitous:(BOOL)flag itemAtURL:(NSURL *)url destinationURL:(NSURL *)destinationURL error:(NSError **)error API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Returns YES if the item for the specified URL is ubiquitous, NO otherwise.
 */
- (BOOL)isUbiquitousItemAtURL:(NSURL *)url API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Start downloading a local instance of the specified ubiquitous item, if necessary. Returns YES if the download started successfully or wasn't necessary, NO otherwise.
 */
- (BOOL)startDownloadingUbiquitousItemAtURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Removes the local instance of the ubiquitous item at the given URL. Returns YES if removal was successful, NO otherwise.
 */
- (BOOL)evictUbiquitousItemAtURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Returns a file URL for the root of the ubiquity container directory corresponding to the supplied container ID. Returns nil if the mobile container does not exist or could not be determined.
 */
- (nullable NSURL *)URLForUbiquityContainerIdentifier:(nullable NSString *)containerIdentifier API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Returns a URL that can be shared with other users to allow them download a copy of the specified ubiquitous item. Also returns the date after which the item will no longer be accessible at the returned URL. The URL must be prefixed with a URL from -URLForUbiquityContainerIdentifier:.
 */
- (nullable NSURL *)URLForPublishingUbiquitousItemAtURL:(NSURL *)url expirationDate:(NSDate * _Nullable * _Nullable)outDate error:(NSError **)error API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Returns an opaque token that represents the current ubiquity identity. This object can be copied, encoded, or compared with isEqual:. When ubiquity containers are unavailable because the user has disabled them, or when the user is simply not logged in, this method will return nil. The NSUbiquityIdentityDidChangeNotification notification is posted after this value changes.

    If you don't need the container URL and just want to check if ubiquity containers are available you should use this method instead of checking -URLForUbiquityContainerIdentifier:.
*/
@property (nullable, readonly, copy) id<NSObject,NSCopying,NSCoding> ubiquityIdentityToken API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/* Asynchronously returns a dictionary of zero or more NSFileProviderService instances, which enable your application to instruct the file's provider to take various actions on or with regards to the given URL. To do this, first identify an NSFileProviderService object whose name matches the service you wish to use. Then get an NSXPCConnection from it and set up its NSXPCInterface with the protocol that matches the service's name. You'll need to refer to external documentation or an SDK supplied by the provider to get this information. Once an NSXPCConnection is obtained, you must finish configuring it and send it -resume. Failure to do so will result in leaking system resources.
 */
- (void)getFileProviderServicesForItemAtURL:(NSURL *)url completionHandler:(void (^)(NSDictionary <NSFileProviderServiceName, NSFileProviderService *> * _Nullable services, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.13), ios(11.0)) API_UNAVAILABLE(watchos, tvos);

/* Returns the container directory associated with the specified security application group ID.
 */
- (nullable NSURL *)containerURLForSecurityApplicationGroupIdentifier:(NSString *)groupIdentifier API_AVAILABLE(macos(10.8), ios(7.0), watchos(2.0), tvos(9.0)); // Available for OS X in 10.8.3.


@end

@interface NSFileManager (NSUserInformation)

@property (readonly, copy) NSURL *homeDirectoryForCurrentUser API_AVAILABLE(macosx(10.12)) API_UNAVAILABLE(ios, watchos, tvos);
@property (readonly, copy) NSURL *temporaryDirectory API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

- (nullable NSURL *)homeDirectoryForUser:(NSString *)userName API_AVAILABLE(macosx(10.12)) API_UNAVAILABLE(ios, watchos, tvos);

@end

/* These delegate methods are for the use of the deprecated handler-taking methods on NSFileManager for copying, moving, linking or deleting files.
 */
@interface NSObject (NSCopyLinkMoveHandler)
- (BOOL)fileManager:(NSFileManager *)fm shouldProceedAfterError:(NSDictionary *)errorInfo API_DEPRECATED(" Handler API no longer supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)fileManager:(NSFileManager *)fm willProcessPath:(NSString *)path API_DEPRECATED("Handler API no longer supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
@end


@protocol NSFileManagerDelegate <NSObject>
@optional

/* fileManager:shouldCopyItemAtPath:toPath: gives the delegate an opportunity to filter the resulting copy. Returning YES from this method will allow the copy to happen. Returning NO from this method causes the item in question to be skipped. If the item skipped was a directory, no children of that directory will be copied, nor will the delegate be notified of those children.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned YES.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldCopyItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldCopyItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* fileManager:shouldProceedAfterError:copyingItemAtPath:toPath: gives the delegate an opportunity to recover from or continue copying after an error. If an error occurs, the error object will contain an NSError indicating the problem. The source path and destination paths are also provided. If this method returns YES, the NSFileManager instance will continue as if the error had not occurred. If this method returns NO, the NSFileManager instance will stop copying, return NO from copyItemAtPath:toPath:error: and the error will be provied there.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned NO.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error copyingItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error copyingItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* fileManager:shouldMoveItemAtPath:toPath: gives the delegate an opportunity to not move the item at the specified path. If the source path and the destination path are not on the same device, a copy is performed to the destination path and the original is removed. If the copy does not succeed, an error is returned and the incomplete copy is removed, leaving the original in place.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned YES.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldMoveItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldMoveItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* fileManager:shouldProceedAfterError:movingItemAtPath:toPath: functions much like fileManager:shouldProceedAfterError:copyingItemAtPath:toPath: above. The delegate has the opportunity to remedy the error condition and allow the move to continue.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned NO.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error movingItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error movingItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* fileManager:shouldLinkItemAtPath:toPath: acts as the other "should" methods, but this applies to the file manager creating hard links to the files in question.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned YES.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldLinkItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldLinkItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* fileManager:shouldProceedAfterError:linkingItemAtPath:toPath: allows the delegate an opportunity to remedy the error which occurred in linking srcPath to dstPath. If the delegate returns YES from this method, the linking will continue. If the delegate returns NO from this method, the linking operation will stop and the error will be returned via linkItemAtPath:toPath:error:.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned NO.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error linkingItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error linkingItemAtURL:(NSURL *)srcURL toURL:(NSURL *)dstURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* fileManager:shouldRemoveItemAtPath: allows the delegate the opportunity to not remove the item at path. If the delegate returns YES from this method, the NSFileManager instance will attempt to remove the item. If the delegate returns NO from this method, the remove skips the item. If the item is a directory, no children of that item will be visited.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned YES.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldRemoveItemAtPath:(NSString *)path;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldRemoveItemAtURL:(NSURL *)URL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* fileManager:shouldProceedAfterError:removingItemAtPath: allows the delegate an opportunity to remedy the error which occurred in removing the item at the path provided. If the delegate returns YES from this method, the removal operation will continue. If the delegate returns NO from this method, the removal operation will stop and the error will be returned via linkItemAtPath:toPath:error:.

    If the delegate does not implement this method, the NSFileManager instance acts as if this method returned NO.
 */
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error removingItemAtPath:(NSString *)path;
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error removingItemAtURL:(NSURL *)URL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end


@interface NSDirectoryEnumerator<ObjectType> : NSEnumerator<ObjectType>

/* For NSDirectoryEnumerators created with -enumeratorAtPath:, the -fileAttributes and -directoryAttributes methods return an NSDictionary containing the keys listed below. For NSDirectoryEnumerators created with -enumeratorAtURL:includingPropertiesForKeys:options:errorHandler:, these two methods return nil.
 */
@property (nullable, readonly, copy) NSDictionary<NSFileAttributeKey, id> *fileAttributes;
@property (nullable, readonly, copy) NSDictionary<NSFileAttributeKey, id> *directoryAttributes;

/* For NSDirectoryEnumerators created with -enumeratorAtURL:includingPropertiesForKeys:options:errorHandler: and the NSDirectoryEnumerationIncludesDirectoriesPostOrder option, this property is YES when the current object is a directory that is being enumerated after all of its contents have been enumerated.
 */
@property (readonly) BOOL isEnumeratingDirectoryPostOrder API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

- (void)skipDescendents;

/* This method returns the number of levels deep the current object is in the directory hierarchy being enumerated. The directory passed to -enumeratorAtURL:includingPropertiesForKeys:options:errorHandler: is considered to be level 0.
 */
@property (readonly) NSUInteger level API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* This method is spelled correctly.
 */
- (void)skipDescendants API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end


/* In an application that has received a URL to a file owned by a file provider, instances of NSFileProviderService can by obtained by calling -[NSFileManager getFileProviderServicesForItemAtURL:completionHandler:]. Each NSFileProviderService instance can only be used to operate on the URL originally passed to that method.
 */
API_AVAILABLE(macos(10.13), ios(11.0)) API_UNAVAILABLE(watchos, tvos)
@interface NSFileProviderService : NSObject {
@private
    NSFileProviderServiceName _name;
    id _endpointCreatingProxy;
    dispatch_group_t _requestFinishedGroup;
}
- (void)getFileProviderConnectionWithCompletionHandler:(void (^)(NSXPCConnection * _Nullable connection, NSError * _Nullable error))completionHandler;
@property (readonly, copy) NSFileProviderServiceName name;
@end

FOUNDATION_EXPORT NSFileAttributeKey const NSFileType;
FOUNDATION_EXPORT NSFileAttributeType const NSFileTypeDirectory;
FOUNDATION_EXPORT NSFileAttributeType const NSFileTypeRegular;
FOUNDATION_EXPORT NSFileAttributeType const NSFileTypeSymbolicLink;
FOUNDATION_EXPORT NSFileAttributeType const NSFileTypeSocket;
FOUNDATION_EXPORT NSFileAttributeType const NSFileTypeCharacterSpecial;
FOUNDATION_EXPORT NSFileAttributeType const NSFileTypeBlockSpecial;
FOUNDATION_EXPORT NSFileAttributeType const NSFileTypeUnknown;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileSize;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileModificationDate;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileReferenceCount;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileDeviceIdentifier;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileOwnerAccountName;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileGroupOwnerAccountName;
FOUNDATION_EXPORT NSFileAttributeKey const NSFilePosixPermissions;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileSystemNumber;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileSystemFileNumber;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileExtensionHidden;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileHFSCreatorCode;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileHFSTypeCode;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileImmutable;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileAppendOnly;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileCreationDate;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileOwnerAccountID;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileGroupOwnerAccountID;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileBusy;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileProtectionKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSFileProtectionType const NSFileProtectionNone API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSFileProtectionType const NSFileProtectionComplete API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSFileProtectionType const NSFileProtectionCompleteUnlessOpen API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSFileProtectionType const NSFileProtectionCompleteUntilFirstUserAuthentication API_AVAILABLE(macos(10.6), ios(5.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT NSFileAttributeKey const NSFileSystemSize;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileSystemFreeSize;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileSystemNodes;
FOUNDATION_EXPORT NSFileAttributeKey const NSFileSystemFreeNodes;

@interface NSDictionary<KeyType, ObjectType> (NSFileAttributes)

- (unsigned long long)fileSize;
- (nullable NSDate *)fileModificationDate;
- (nullable NSString *)fileType;
- (NSUInteger)filePosixPermissions;
- (nullable NSString *)fileOwnerAccountName;
- (nullable NSString *)fileGroupOwnerAccountName;
- (NSInteger)fileSystemNumber;
- (NSUInteger)fileSystemFileNumber;
- (BOOL)fileExtensionHidden;
- (OSType)fileHFSCreatorCode;
- (OSType)fileHFSTypeCode;
- (BOOL)fileIsImmutable;
- (BOOL)fileIsAppendOnly;
- (nullable NSDate *)fileCreationDate;
- (nullable NSNumber *)fileOwnerAccountID;
- (nullable NSNumber *)fileGroupOwnerAccountID;
@end

NS_ASSUME_NONNULL_END
