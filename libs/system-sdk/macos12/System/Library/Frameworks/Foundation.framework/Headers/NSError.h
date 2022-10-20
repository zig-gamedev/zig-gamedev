/*	NSError.h
	Copyright (c) 2003-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSDictionary<KeyType, ObjectType>, NSArray<ObjectType>, NSString;

typedef NSString *NSErrorDomain;

NS_ASSUME_NONNULL_BEGIN

// Predefined domain for errors from most AppKit and Foundation APIs.
FOUNDATION_EXPORT NSErrorDomain const NSCocoaErrorDomain;

// Other predefined domains; value of "code" will correspond to preexisting values in these domains.
FOUNDATION_EXPORT NSErrorDomain const NSPOSIXErrorDomain;
FOUNDATION_EXPORT NSErrorDomain const NSOSStatusErrorDomain;
FOUNDATION_EXPORT NSErrorDomain const NSMachErrorDomain;

typedef NSString *NSErrorUserInfoKey;

// Key in userInfo. A recommended standard way to embed NSErrors from underlying calls. The value of this key should be an NSError.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSUnderlyingErrorKey;

// Key in userInfo. A recommended standard way to embed a list of several NSErrors from underlying calls. The value of this key should be an NSArray of NSError. This value is independent from the value of `NSUnderlyingErrorKey` - neither, one, or both may be set.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSMultipleUnderlyingErrorsKey API_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

// Keys in userInfo, for subsystems wishing to provide their error messages up-front. Note that NSError will also consult the userInfoValueProvider for the domain when these values are not present in the userInfo dictionary.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSLocalizedDescriptionKey;             // NSString, a complete sentence (or more) describing ideally both what failed and why it failed.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSLocalizedFailureReasonErrorKey;      // NSString, a complete sentence (or more) describing why the operation failed.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSLocalizedRecoverySuggestionErrorKey; // NSString, a complete sentence (or more) describing what the user can do to fix the problem.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSLocalizedRecoveryOptionsErrorKey;    // NSArray of NSStrings corresponding to button titles.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSRecoveryAttempterErrorKey;           // Instance of a subclass of NSObject that conforms to the NSErrorRecoveryAttempting informal protocol
FOUNDATION_EXPORT NSErrorUserInfoKey const NSHelpAnchorErrorKey;                  // NSString containing a help anchor
FOUNDATION_EXPORT NSErrorUserInfoKey const NSDebugDescriptionErrorKey API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));     // NSString. This provides a string which will be shown when constructing the debugDescription of the NSError, to be used when debugging or when formatting the error with %@.  This string will never be used in localizedDescription, so will not be shown to the user.
FOUNDATION_EXPORT NSErrorUserInfoKey const NSLocalizedFailureErrorKey API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));  // NSString, a complete sentence (or more) describing what failed.  Setting a value for this key in userInfo dictionary of errors received from framework APIs is a good way to customize and fine tune the localizedDescription of an NSError.  As an example, for Foundation error code NSFileWriteOutOfSpaceError, setting the value of this key to "The image library could not be saved." will allow the localizedDescription of the error to come out as "The image library could not be saved. The volume Macintosh HD is out of space." rather than the default (say) “You can't save the file ImgDatabaseV2 because the volume Macintosh HD is out of space."

// Other standard keys in userInfo, for various error codes
FOUNDATION_EXPORT NSErrorUserInfoKey const NSStringEncodingErrorKey ;  // NSNumber containing NSStringEncoding
FOUNDATION_EXPORT NSErrorUserInfoKey const NSURLErrorKey;              // NSURL
FOUNDATION_EXPORT NSErrorUserInfoKey const NSFilePathErrorKey;         // NSString

@interface NSError : NSObject <NSCopying, NSSecureCoding> {
    @private
    void *_reserved;
    NSInteger _code;
    NSString *_domain;
    NSDictionary *_userInfo;
}

/* Domain cannot be nil; dict may be nil if no userInfo desired.
*/
- (instancetype)initWithDomain:(NSErrorDomain)domain code:(NSInteger)code userInfo:(nullable NSDictionary<NSErrorUserInfoKey, id> *)dict NS_DESIGNATED_INITIALIZER;
+ (instancetype)errorWithDomain:(NSErrorDomain)domain code:(NSInteger)code userInfo:(nullable NSDictionary<NSErrorUserInfoKey, id> *)dict;

/* These define the error. Domains are described by names that are arbitrary strings used to differentiate groups of codes; for custom domain using reverse-DNS naming will help avoid conflicts. Codes are domain-specific.
*/
@property (readonly, copy) NSErrorDomain domain;
@property (readonly) NSInteger code;

/* Additional info which may be used to describe the error further. Examples of keys that might be included in here are "Line Number", "Failed URL", etc. Embedding other errors in here can also be used as a way to communicate underlying reasons for failures; for instance "File System Error" embedded in the userInfo of an NSError returned from a higher level document object. If the embedded error information is itself NSError, the standard key NSUnderlyingErrorKey can be used.
*/
@property (readonly, copy) NSDictionary<NSErrorUserInfoKey, id> *userInfo;

/* The primary user-presentable message for the error, for instance for NSFileReadNoPermissionError: "The file "File Name" couldn't be opened because you don't have permission to view it.". This message should ideally indicate what failed and why it failed. This value either comes from NSLocalizedDescriptionKey, or NSLocalizedFailureErrorKey+NSLocalizedFailureReasonErrorKey, or NSLocalizedFailureErrorKey. The steps this takes to construct the description include:
 1. Look for NSLocalizedDescriptionKey in userInfo, use value as-is if present.
 2. Look for NSLocalizedFailureErrorKey in userInfo. If present, use, combining with value for NSLocalizedFailureReasonErrorKey if available.
 3. Fetch NSLocalizedDescriptionKey from userInfoValueProvider, use value as-is if present.
 4. Fetch NSLocalizedFailureErrorKey from userInfoValueProvider. If present, use, combining with value for NSLocalizedFailureReasonErrorKey if available.
 5. Look for NSLocalizedFailureReasonErrorKey in userInfo or from userInfoValueProvider; combine with generic "Operation failed" message.
 6. Last resort localized but barely-presentable string manufactured from domain and code. The result is never nil.
*/
@property (readonly, copy) NSString *localizedDescription;

/* Return a complete sentence which describes why the operation failed. For instance, for NSFileReadNoPermissionError: "You don't have permission.". In many cases this will be just the "because" part of the error message (but as a complete sentence, which makes localization easier).  Default implementation of this picks up the value of NSLocalizedFailureReasonErrorKey from the userInfo dictionary. If not present, it consults the userInfoValueProvider for the domain, and if that returns nil, this also returns nil.
*/
@property (nullable, readonly, copy) NSString *localizedFailureReason;

/* Return the string that can be displayed as the "informative" (aka "secondary") message on an alert panel. For instance, for NSFileReadNoPermissionError: "To view or change permissions, select the item in the Finder and choose File > Get Info.". Default implementation of this picks up the value of NSLocalizedRecoverySuggestionErrorKey from the userInfo dictionary. If not present, it consults the userInfoValueProvider for the domain, and if that returns nil, this also returns nil.
*/
@property (nullable, readonly, copy) NSString *localizedRecoverySuggestion;

/* Return titles of buttons that are appropriate for displaying in an alert. These should match the string provided as a part of localizedRecoverySuggestion.  The first string would be the title of the right-most and default button, the second one next to it, and so on. If used in an alert the corresponding default return values are NSAlertFirstButtonReturn + n. Default implementation of this picks up the value of NSLocalizedRecoveryOptionsErrorKey from the userInfo dictionary. If not present, it consults the userInfoValueProvider for the domain, and if that returns nil, this also returns nil. nil return usually implies no special suggestion, which would imply a single "OK" button.
*/
@property (nullable, readonly, copy) NSArray<NSString *> *localizedRecoveryOptions;

/* Return an object that conforms to the NSErrorRecoveryAttempting informal protocol. The recovery attempter must be an object that can correctly interpret an index into the array returned by localizedRecoveryOptions. The default implementation of this picks up the value of NSRecoveryAttempterErrorKey from the userInfo dictionary. If not present, it consults the userInfoValueProvider for the domain. If that returns nil, this also returns nil.
*/
@property (nullable, readonly, strong) id recoveryAttempter;

/* Return the help anchor that can be used to create a help button to accompany the error when it's displayed to the user.  This is done automatically by +[NSAlert alertWithError:], which the presentError: variants in NSApplication go through. The default implementation of this picks up the value of the NSHelpAnchorErrorKey from the userInfo dictionary. If not present, it consults the userInfoValueProvider for the domain.  If that returns nil, this also returns nil.
*/
@property (nullable, readonly, copy) NSString *helpAnchor;

/* Return a list of underlying errors, if any. It includes the values of both NSUnderlyingErrorKey and NSMultipleUnderlyingErrorsKey. If there are no underlying errors, returns an empty array.
*/
@property (readonly, copy) NSArray<NSError *> *underlyingErrors API_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));

/* Specify a block which will be called from the implementations of localizedDescription, localizedFailureReason, localizedRecoverySuggestion, localizedRecoveryOptions, recoveryAttempter, helpAnchor, and debugDescription when the underlying value for these is not present in the userInfo dictionary of NSError instances with the specified domain.  The provider will be called with the userInfo key corresponding to the queried property: For instance, NSLocalizedDescriptionKey for localizedDescription.  The provider should return nil for any keys it is not able to provide and, very importantly, any keys it does not recognize (since we may extend the list of keys in future releases).

The specified block will be called synchronously at the time when the above properties are queried. The results are not cached.

This provider is optional. It enables localization and formatting of error messages to be done lazily; rather than populating the userInfo at NSError creation time, these keys will be fetched on-demand when asked for.
 
It is expected that only the “owner” of an NSError domain specifies the provider for the domain, and this is done once. This facility is not meant for consumers of errors to customize the userInfo entries.  This facility should not be used to customize the behaviors of error domains provided by the system.
 
If an appropriate result for the requested key cannot be provided, return nil rather than choosing to manufacture a generic fallback response such as "Operation could not be completed, error 42." NSError will take care of the fallback cases.
*/
+ (void)setUserInfoValueProviderForDomain:(NSErrorDomain)errorDomain provider:(id _Nullable (^ _Nullable)(NSError *err, NSErrorUserInfoKey userInfoKey))provider API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));
+ (id _Nullable (^ _Nullable)(NSError *err, NSErrorUserInfoKey userInfoKey))userInfoValueProviderForDomain:(NSErrorDomain)errorDomain API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

@end


@interface NSObject(NSErrorRecoveryAttempting)

/* Given that an error alert has been presented document-modally to the user, and the user has chosen one of the error's recovery options, attempt recovery from the error, and send the selected message to the specified delegate. The option index is an index into the error's array of localized recovery options. The method selected by didRecoverSelector must have the same signature as:

    - (void)didPresentErrorWithRecovery:(BOOL)didRecover contextInfo:(void *)contextInfo;

The value passed for didRecover must be YES if error recovery was completely successful, NO otherwise.
*/
- (void)attemptRecoveryFromError:(NSError *)error optionIndex:(NSUInteger)recoveryOptionIndex delegate:(nullable id)delegate didRecoverSelector:(nullable SEL)didRecoverSelector contextInfo:(nullable void *)contextInfo;

/* Given that an error alert has been presented applicaton-modally to the user, and the user has chosen one of the error's recovery options, attempt recovery from the error, and return YES if error recovery was completely successful, NO otherwise. The recovery option index is an index into the error's array of localized recovery options.
*/
- (BOOL)attemptRecoveryFromError:(NSError *)error optionIndex:(NSUInteger)recoveryOptionIndex;

@end

NS_ASSUME_NONNULL_END

