/*	NSItemProvider.h
        Copyright (c) 2013-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSArray.h>


#if __OBJC2__


NS_ASSUME_NONNULL_BEGIN

@class NSItemProvider, NSProgress;


typedef NS_ENUM(NSInteger, NSItemProviderRepresentationVisibility) {
    NSItemProviderRepresentationVisibilityAll = 0,                                  // All processes can see this representation
    NSItemProviderRepresentationVisibilityTeam                                      // Only processes from the same dev team can see this representation
    API_AVAILABLE(ios(11.0), watchos(4.0), tvos(11.0)) API_UNAVAILABLE(macos) = 1,
    NSItemProviderRepresentationVisibilityGroup                                     // Only processes from the same group can see this representation
    API_AVAILABLE(macos(10.13)) API_UNAVAILABLE(ios, watchos, tvos) = 2 ,
    NSItemProviderRepresentationVisibilityOwnProcess = 3,                           // Ony the originator's process can see this representation
} API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


// The default behavior is to copy files.
typedef NS_OPTIONS(NSInteger, NSItemProviderFileOptions) {
    NSItemProviderFileOptionOpenInPlace = 1,
} API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


// This protocol allows a class to export its data to a variety of binary representations.
API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0))
@protocol NSItemProviderWriting <NSObject>

@property (class, NS_NONATOMIC_IOSONLY, readonly, copy) NSArray<NSString *> *writableTypeIdentifiersForItemProvider;

@optional

// If this method is not implemented, the class method will be consulted instead.
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSArray<NSString *> *writableTypeIdentifiersForItemProvider;

+ (NSItemProviderRepresentationVisibility)itemProviderVisibilityForRepresentationWithTypeIdentifier:(NSString *)typeIdentifier;

// If this method is not implemented, the class method will be consulted instead.
- (NSItemProviderRepresentationVisibility)itemProviderVisibilityForRepresentationWithTypeIdentifier:(NSString *)typeIdentifier;

@required

- (nullable NSProgress *)loadDataWithTypeIdentifier:(NSString *)typeIdentifier // One of writableTypeIdentifiersForItemProvider
                   forItemProviderCompletionHandler:(void (^)(NSData * _Nullable data, NSError * _Nullable error))completionHandler;

@end


// This protocol allows a class to be constructed from a variety of binary representations.
API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0))
@protocol NSItemProviderReading <NSObject>

@property (class, NS_NONATOMIC_IOSONLY, readonly, copy) NSArray<NSString *> *readableTypeIdentifiersForItemProvider;

+ (nullable instancetype)objectWithItemProviderData:(NSData *)data
                                     typeIdentifier:(NSString *)typeIdentifier
                                              error:(NSError **)outError;

@end


typedef void (^NSItemProviderCompletionHandler)(__nullable __kindof id <NSSecureCoding> item, NSError * __null_unspecified error);
typedef void (^NSItemProviderLoadHandler)(__null_unspecified NSItemProviderCompletionHandler completionHandler, __null_unspecified Class expectedValueClass, NSDictionary * __null_unspecified options);


// An NSItemProvider is a high level abstraction for an item supporting multiple representations.
API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
@interface NSItemProvider : NSObject <NSCopying>

#pragma mark - Binary interface


#pragma mark Provider


- (instancetype)init NS_DESIGNATED_INITIALIZER;

// Register higher-fidelity types first, followed by progressively lower-fidelity ones. This ordering helps consumers get the best representation they can handle.

// Registers a data-backed representation.
- (void)registerDataRepresentationForTypeIdentifier:(NSString *)typeIdentifier
                                         visibility:(NSItemProviderRepresentationVisibility)visibility
                                        loadHandler:(NSProgress * _Nullable (^)(void (^completionHandler)(NSData * _Nullable data, NSError * _Nullable error)))loadHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Registers a file-backed representation.
// Set `coordinated` to YES if the returned file must be accessed using NSFileCoordinator.
// If `NSItemProviderFileOptionOpenInPlace` is not provided, the file provided will be copied before the load handler returns.
- (void)registerFileRepresentationForTypeIdentifier:(NSString *)typeIdentifier
                                        fileOptions:(NSItemProviderFileOptions)fileOptions
                                         visibility:(NSItemProviderRepresentationVisibility)visibility
                                        loadHandler:(NSProgress * _Nullable (^)(void (^completionHandler)(NSURL * _Nullable url, BOOL coordinated, NSError * _Nullable error)))loadHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


#pragma mark Consumer


// Returns the list of registered type identifiers, in the order they were registered.
@property (copy, readonly, atomic) NSArray<NSString *> *registeredTypeIdentifiers;

- (NSArray<NSString *> *)registeredTypeIdentifiersWithFileOptions:(NSItemProviderFileOptions)fileOptions API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Returns YES if the item provider has at least one item that conforms to the supplied type identifier.
- (BOOL)hasItemConformingToTypeIdentifier:(NSString *)typeIdentifier;

- (BOOL)hasRepresentationConformingToTypeIdentifier:(NSString *)typeIdentifier
                                        fileOptions:(NSItemProviderFileOptions)fileOptions API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Copies the provided data into an NSData object.
- (NSProgress *)loadDataRepresentationForTypeIdentifier:(NSString *)typeIdentifier
                                      completionHandler:(void(^)(NSData * _Nullable data, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Writes a copy of the data to a temporary file. This file will be deleted when the completion handler returns. Your program should copy or move the file within the completion handler.
- (NSProgress *)loadFileRepresentationForTypeIdentifier:(NSString *)typeIdentifier
                                      completionHandler:(void(^)(NSURL * _Nullable url, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Open the original file in place, if possible.
// If a file is not available for opening in place, a copy of the file is written to a temporary location, and `isInPlace` is set to NO. Your program may then copy or move the file, or the system will delete this file at some point in the future.
- (NSProgress *)loadInPlaceFileRepresentationForTypeIdentifier:(NSString *)typeIdentifier
                                             completionHandler:(void (^)(NSURL * _Nullable url, BOOL isInPlace, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


#pragma mark Metadata


@property (atomic, copy, nullable) NSString *suggestedName API_AVAILABLE(macos(10.14), ios(11.0)) API_UNAVAILABLE(watchos, tvos);


#pragma mark - NSItemProviderReading and Writing interface


// Instantiate an NSItemProvider by querying an object for its eligible type identifiers via the NSItemProviderWriting protocol.
- (instancetype)initWithObject:(id<NSItemProviderWriting>)object API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Add representations from an object using the NSItemProviderWriting protocol. Duplicate representations are ignored.
- (void)registerObject:(id<NSItemProviderWriting>)object visibility:(NSItemProviderRepresentationVisibility)visibility API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Add representations from a class, but defer loading the object until needed.
- (void)registerObjectOfClass:(Class<NSItemProviderWriting>)aClass
                   visibility:(NSItemProviderRepresentationVisibility)visibility
                  loadHandler:(NSProgress * _Nullable (^)(void (^completionHandler)(__kindof id<NSItemProviderWriting> _Nullable object, NSError * _Nullable error)))loadHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

- (BOOL)canLoadObjectOfClass:(Class<NSItemProviderReading>)aClass API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

// Instantiate an object using the NSItemProviderReading protocol.
- (NSProgress *)loadObjectOfClass:(Class<NSItemProviderReading>)aClass
                completionHandler:(void (^)(__kindof id<NSItemProviderReading> _Nullable object, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


#pragma mark - Coercing interface

// These methods allow you to assign NSSecureCoding-compliant objects to certain UTIs, and retrieve either the original object, or a coerced variant
// based on the following rules.
//
// If the object is retrieved using loadItemForTypeIdentifier:options:completionHandler, and the completion block signature contains a paramater of
// the same class as the initial object (or a superclass), the original object is returned.
//
// If the completion block signature contains a parameter that is not the same class as `item`, some coercion may occur:
//    Original class       Requested class          Coercion action
//    -------------------  -----------------------  -------------------
//    NSURL                NSData                   The contents of the URL is read and returned as NSData
//    NSData               NSImage/UIImage          An NSImage (macOS) or UIImage (iOS) is constructed from the data
//    NSURL                UIImage                  A UIImage is constructed from the file at the URL (iOS)
//    NSImage              NSData                   A TIFF representation of the image is returned
//
// When providing or consuming data using this interface, a file may be opened in-place depending on the NSExtension context in which this object is used.
//
// If the item is retrieved using the binary interface described above, the original object will be retrieved and coerced to NSData.
//
// Items registered using the binary interface will appear as NSData with respect to the coercing interface.


#pragma mark Provider


// Initialize an NSItemProvider with an object assigned to a single UTI. `item` is retained.
- (instancetype)initWithItem:(nullable id<NSSecureCoding>)item typeIdentifier:(nullable NSString *)typeIdentifier NS_DESIGNATED_INITIALIZER;

// Initialize an NSItemProvider with load handlers for the given file URL, and the file content. A type identifier is inferred from the file extension.
- (nullable instancetype)initWithContentsOfURL:(null_unspecified NSURL *)fileURL;

// Registers a load handler that returns an object, assigned to a single UTI.
- (void)registerItemForTypeIdentifier:(NSString *)typeIdentifier loadHandler:(NSItemProviderLoadHandler)loadHandler;


#pragma mark Consumer


// Loads the best matching item for a type identifier. The returned object depends on the class specified for the completion handler's `item` parameter.
// See the table above for coercion rules.
- (void)loadItemForTypeIdentifier:(NSString *)typeIdentifier options:(nullable NSDictionary *)options completionHandler:(nullable NSItemProviderCompletionHandler)completionHandler;


@end


// Common keys for the item provider options dictionary.
FOUNDATION_EXTERN NSString * const NSItemProviderPreferredImageSizeKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)); // NSValue of CGSize or NSSize, specifies image size in pixels.


// Some uses of NSItemProvider support the use of optional preview images.
@interface NSItemProvider(NSPreviewSupport)

// Sets a custom preview image handler block for this item provider. The returned item should preferably be NSData or a file NSURL.
@property (nullable, copy, atomic) NSItemProviderLoadHandler previewImageHandler API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

// Loads the preview image for this item by either calling the supplied preview block or falling back to a QuickLook-based handler. This method, like loadItemForTypeIdentifier:options:completionHandler:, supports implicit type coercion for the item parameter of the completion block. Allowed value classes are: NSData, NSURL, UIImage/NSImage.
- (void)loadPreviewImageWithOptions:(null_unspecified NSDictionary *)options completionHandler:(null_unspecified NSItemProviderCompletionHandler)completionHandler API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@end


// Keys used in property list items received from or sent to JavaScript code

// If JavaScript code passes an object to its completionFunction, it will be placed into an item of type kUTTypePropertyList, containing an NSDictionary, under this key.
FOUNDATION_EXTERN NSString * _Null_unspecified const NSExtensionJavaScriptPreprocessingResultsKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

// Arguments to be passed to a JavaScript finalize method should be placed in an item of type kUTTypePropertyList, containing an NSDictionary, under this key.
FOUNDATION_EXTERN NSString * _Null_unspecified const NSExtensionJavaScriptFinalizeArgumentKey API_AVAILABLE(ios(8.0), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);


#pragma mark - Errors


// Constant used by NSError to distinguish errors belonging to the NSItemProvider domain
FOUNDATION_EXTERN NSString * const NSItemProviderErrorDomain API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

// NSItemProvider-related error codes
typedef NS_ENUM(NSInteger, NSItemProviderErrorCode) {
    NSItemProviderUnknownError                                      = -1,
    NSItemProviderItemUnavailableError                              = -1000,
    NSItemProviderUnexpectedValueClassError                         = -1100,
    NSItemProviderUnavailableCoercionError API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = -1200
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

NS_ASSUME_NONNULL_END


#else // __OBJC2__


@protocol NSItemProviderReading <NSObject>
@end

@protocol NSItemProviderWriting <NSObject>
@end


#endif
