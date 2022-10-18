/*
	NSFileWrapper.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSData, NSDictionary<KeyType, ObjectType>, NSError, NSString, NSURL;

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSFileWrapperReadingOptions) {
    
    /* Whether the contents are read immediately, applied recursively in the case of directory file wrappers. If reading with this option succeeds then subsequent invocations of -fileWrappers, -regularFileContents, -symbolicLinkDestinationURL:, -serializedRepresentation, and, on Mac OS X, -[NSFileWrapper(NSExtensions) icon] sent to the receiver and all its descendant file wrappers won't fail. For performance NSFileWrapper may or may not immediately read the contents of some file packages immediately even when this option is chosen. For example, the contents of bundles (not all file packages are bundles) are immutable to the user so on Mac OS X NSFileWrapper may read the children of such a directory lazily. You can use this option to take a reasonable snapshot of a file or folder for writing later. For example, a Mac OS X application like TextEdit can use this option when creating new file wrappers to represent attachments that the user creates by copying and pasting or dragging and dropping from the Finder to a TextEdit document. You wouldn't use this option when reading a document file package because that would cause unnecessarily bad perfomance. For example, an application wouldn't use this option when creating file wrappers to represent attachments as it's opening a document stored in a file package.
    */
    NSFileWrapperReadingImmediate = 1 << 0,

    /* Whether file mapping for regular file wrappers is disallowed. In Mac OS 10.6 and newer NSFileWrapper only ever memory maps files on internal drives, regardless of whether this option is used. It never memory maps files on external drives or network volumes. You can use this option to keep NSFileWrapper from memory mapping files at all, even ones on internal drives. This is useful if you want to make sure your application doesn't hold files open (mapped files are open files) and therefore prevent the user from ejecting DVDs, unmounting disk partitions, or unmounting disk images.
    */
    NSFileWrapperReadingWithoutMapping = 1 << 1

} API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

typedef NS_OPTIONS(NSUInteger, NSFileWrapperWritingOptions) {

    /* Whether writing is done atomically. You can use this option to ensure that when overwriting a file package the overwriting either completely succeeds or completely fails, with no possibility of leaving the file package in an inconsistent state. Because this option causes additional I/O you shouldn't use it unnecessarily. For example, on Mac OS X you wouldn't use this option in an override of -[NSDocument writeToURL:ofType:error:] because NSDocument's implementation of safe saving already does atomic writing.
    */
    NSFileWrapperWritingAtomic = 1 << 0,

    /* Whether descendant file wrappers are sent -setFilename: if the writing succeeds. This is necessary when your application passes original contents URLs to -writeToURL:options:originalContentsURL:error:. Without using this and reusing child file wrappers properly subsequent invocations of -writeToURL:options:originalContentsURL:error: wouldn't be able to reliably create hard links in a new file package for performance because the record of names in the old file package would be out of date.
    */
    NSFileWrapperWritingWithNameUpdating = 1 << 1

} API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

API_AVAILABLE(macos(10.0), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSFileWrapper : NSObject<NSSecureCoding> {
    @private
    NSDictionary *_fileAttributes;
    NSString *_preferredFileName;
    NSString *_fileName;
    id _contents;
    id _icon;
    id _moreVars;
}

#pragma mark *** Initialization ***

/* A designated initializer for creating an instance whose kind (directory, regular file, or symbolic link) is determined based on what the URL locates. If reading is not successful return nil after setting *outError to an NSError that encapsulates the reason why the file wrapper could not be read.
*/
- (nullable instancetype)initWithURL:(NSURL *)url options:(NSFileWrapperReadingOptions)options error:(NSError **)outError NS_DESIGNATED_INITIALIZER API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* A designated initializer for creating an instance for which -isDirectory returns YES. The passed-in dictionary must contain entries whose values are the file wrappers that are to become children and whose keys are file names. Each file wrapper that does not already have a preferred file name is sent -setPreferredFilename: with the corresponding key as the argument.
*/
- (instancetype)initDirectoryWithFileWrappers:(NSDictionary<NSString *, NSFileWrapper *> *)childrenByPreferredName NS_DESIGNATED_INITIALIZER;

/* A designated initializer for creating an instance for which -isRegularFile returns YES.
*/
- (instancetype)initRegularFileWithContents:(NSData *)contents NS_DESIGNATED_INITIALIZER;

/* A designated initializer for creating an instance for which -isSymbolicLink returns YES. -writeToURL:options:originalContentsURL:error: uses the result of sending -relativePath to this URL when creating a symbolic link. (An NSURL initialized by -initFileURLWithPath: returns that entire path when sent -relativePath though.)
*/
- (instancetype)initSymbolicLinkWithDestinationURL:(NSURL *)url NS_DESIGNATED_INITIALIZER API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* A designated initializer. The data must be in the same format as that returned by -serializedRepresentation.
*/
- (nullable instancetype)initWithSerializedRepresentation:(NSData *)serializeRepresentation NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;

#pragma mark *** Properties Applicable to Every Kind of File Wrapper ***

/* What kind of file wrapper it is. Invocations of -readFromURL:options:error: may change what subsequent invocations of these methods return.
*/
@property (readonly, getter=isDirectory) BOOL directory;
@property (readonly, getter=isRegularFile) BOOL regularFile;
@property (readonly, getter=isSymbolicLink) BOOL symbolicLink;

/* The file name that is "preferred." When the receiver is added to a parent directory file wrapper the parent will attempt to use this name as the key into its dictionary of children. Usually the preferred file name will actually get used in this situation but it won't be if that key is already in use. The default implementation of this method causes existing parents to remove and re-add the child to accommodate the change. Preferred file names of children are not effectively preserved when you write a file wrapper to disk and then later instantiate another file wrapper by reading. If your application needs to preserve the user-visible names of attachments it has to make its own arrangements for their storage.

Some instances of NSFileWrapper may be created without a preferredFilename (e.g. -initDirectoryWithFileWrappers: or -initRegularFileWithContents:), meaning preferredFilename may be nil. However, setting nil is never allowed and will result in an exception.
*/
@property (nullable, copy) NSString *preferredFilename;

/* The actual file name. Often it will be the same as the preferred file name but might instead be a name derived from the preferred file name. You can use this method to find out the name of a child that's just been read but you should not use it to find out the name of a child that's about to be written, because the name might be about to change. Send -keyForFileWrapper: to the parent instead.
*/
@property (nullable, copy) NSString *filename;

/* The file attributes, in a dictionary of the same sort as those returned by -[NSFileManager attributesOfItemAtPath:error:].
*/
@property (copy) NSDictionary<NSString *, id> *fileAttributes;

/* On Mac OS X methods for setting and getting the icon are in AppKit's NSFileWrapper(NSExtensions), declared in <AppKit/NSFileWrapperExtensions.h>.
*/

#pragma mark *** Reading ***

/* Whether the receiver matches the directory, regular file, or symbolic link that is located by the URL. For a directory, children are compared against the files in the directory, recursively. The contents of files are not compared; matching of regular files is based on file modification dates. Because children of directory file wrappers are not read immediately by -initWithURL:options:error: or -readFromURL:options:error: unless NSFileWrapperReadingImmediate is used, creating a file wrapper and then later sending it this message is not a reliable way to simply check whether anything in a directory has changed. You can use this method to determine whether the receiver's contents in memory are out of date relative to the file system.
*/
- (BOOL)matchesContentsOfURL:(NSURL *)url API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* Recursively reread the entire contents of the receiver from the specified location, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the file wrapper could not be reread. When reading a directory children are added and removed as necessary to match the file system.
*/
- (BOOL)readFromURL:(NSURL *)url options:(NSFileWrapperReadingOptions)options error:(NSError **)outError API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

#pragma mark *** Writing ***

/* Recursively write the entire contents of the receiver at the specified location, and return YES if successful. If not successful, return NO after setting *outError to an NSError that encapsulates the reason why the file wrapper could not be written. originalContentsURL may be nil or it may be the location of a previous revision of what is being written. If it is not nil the default implementation of this method attempts to avoid unnecessary I/O by merely writing hard links to regular files instead of actually writing out their contents. The descendant file wrappers must return accurate values when sent -filename for this to work (use NSFileWrapperWritingWithNameUpdating to increase the likelihood of that).
*/
- (BOOL)writeToURL:(NSURL *)url options:(NSFileWrapperWritingOptions)options originalContentsURL:(nullable NSURL *)originalContentsURL error:(NSError **)outError API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

#pragma mark *** Serialization ***

/* Return an NSData suitable for passing to -initWithSerializedRepresentation:. This method may return nil if the receiver is the result of reading from the file system (use NSFileWrapperReadingImmediately if appropriate to prevent that).
*/
@property (nullable, readonly, copy) NSData *serializedRepresentation;

#pragma mark *** Directories ***

/* Each of these methods throw an exception when [receiver isDirectory]==NO. */

/* Add a file wrapper to the receiver's children and return the file name that uniquely identifies it among the receiver's children. The unique file name will be the same as the passed-in file wrapper's preferred file name unless that name is already in use as a key into the receiver's dictionary of children. Beware of the fact that -initDirectoryWithFileWrappers:, -initRegularFileWithContents:, and -initSymbolicLinkWithDestinationURL: do not automatically set the preferred file name.
*/
- (NSString *)addFileWrapper:(NSFileWrapper *)child;

/* A convenience method. The default implementation merely allocates a new file wrapper, initializes it with -initRegularFileWithContents:, sends it -setPreferredFileName:fileName, adds it to the receiver with -addFileWrapper:, and returns what -addFileWrapper: returned.
*/
- (NSString *)addRegularFileWithContents:(NSData *)data preferredFilename:(NSString *)fileName;

/* Remove a file wrapper from the receiver's children.
*/
- (void)removeFileWrapper:(NSFileWrapper *)child;

/* Return a dictionary whose values are the receiver's children and whose keys are the unique file name that has been assigned to each one. This method may return nil if the receiver is the result of reading a parent from the file system (use NSFileWrapperReadingImmediately if appropriate to prevent that).
*/
@property (nullable, readonly, copy) NSDictionary<NSString *, NSFileWrapper *> *fileWrappers;

/* Return the unique file name that has been assigned to a child or nil if it is not a child of the receiver.
*/
- (nullable NSString *)keyForFileWrapper:(NSFileWrapper *)child;

#pragma mark *** Regular Files ***

/* This method throws an exception when [receiver isRegularFile]==NO. */

/* Return the receiver's contents. This may return nil if the receiver is the result of reading a parent from the file system (use NSFileWrapperReadingImmediately if appropriate to prevent that).
*/
@property (nullable, readonly, copy) NSData *regularFileContents;

#pragma mark *** Symbolic Links ***

/* This method throws an exception when [receiver isSymbolicLink]==NO. */

/* Return the destination link of the receiver. This may return nil if the receiver is the result of reading a parent from the file system (use NSFileWrapperReadingImmediately if appropriate to prevent that).
*/
@property (nullable, readonly, copy) NSURL *symbolicLinkDestinationURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

#if TARGET_OS_OSX

@interface NSFileWrapper(NSDeprecated)

#pragma mark *** Backward Compatibility ***

/* Methods that were deprecated in OS X 10.6 and never available on iOS. */

- (nullable id)initWithPath:(NSString *)path API_DEPRECATED("Use -initWithURL:options:error: instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
- (id)initSymbolicLinkWithDestination:(NSString *)path API_DEPRECATED("Use -initSymbolicLinkWithDestinationURL: and -setPreferredFileName:, if necessary, instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
- (BOOL)needsToBeUpdatedFromPath:(NSString *)path API_DEPRECATED("Use -matchesContentsOfURL: instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
- (BOOL)updateFromPath:(NSString *)path API_DEPRECATED("Use -readFromURL:options:error: instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)atomicFlag updateFilenames:(BOOL)updateFilenamesFlag API_DEPRECATED("Use -writeToURL:options:originalContentsURL:error: instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
- (NSString *)addFileWithPath:(NSString *)path API_DEPRECATED("Instantiate a new NSFileWrapper with -initWithURL:options:error:, send it -setPreferredFileName: if necessary, then use -addFileWrapper: instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
- (NSString *)addSymbolicLinkWithDestination:(NSString *)path preferredFilename:(NSString *)filename API_DEPRECATED("Instantiate a new NSFileWrapper with -initWithSymbolicLinkDestinationURL:, send it -setPreferredFileName: if necessary, then use -addFileWrapper: instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
- (NSString *)symbolicLinkDestination API_DEPRECATED("Use -symbolicLinkDestinationURL instead.", macos(10.0,10.10)) API_UNAVAILABLE(ios, watchos, tvos);

@end

#endif

NS_ASSUME_NONNULL_END
