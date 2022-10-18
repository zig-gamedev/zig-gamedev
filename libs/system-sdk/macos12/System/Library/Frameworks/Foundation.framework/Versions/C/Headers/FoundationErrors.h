/*	FoundationErrors.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSError.h>

/* NSError codes in NSCocoaErrorDomain. Note that other frameworks (such as AppKit and CoreData) also provide additional NSCocoaErrorDomain error codes.
*/
NS_ERROR_ENUM(NSCocoaErrorDomain) {
    // File system and file I/O related errors, with NSFilePathErrorKey or NSURLErrorKey containing path or URL
    NSFileNoSuchFileError = 4,                                              // Attempt to do a file system operation on a non-existent file
    NSFileLockingError = 255,                                               // Couldn't get a lock on file
    NSFileReadUnknownError = 256,                                           // Read error (reason unknown)
    NSFileReadNoPermissionError = 257,                                      // Read error (permission problem)
    NSFileReadInvalidFileNameError = 258,                                   // Read error (invalid file name)
    NSFileReadCorruptFileError = 259,                                       // Read error (file corrupt, bad format, etc)
    NSFileReadNoSuchFileError = 260,                                        // Read error (no such file)
    NSFileReadInapplicableStringEncodingError = 261,                        // Read error (string encoding not applicable) also NSStringEncodingErrorKey
    NSFileReadUnsupportedSchemeError = 262,                                 // Read error (unsupported URL scheme)
    NSFileReadTooLargeError API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 263,             // Read error (file too large)
    NSFileReadUnknownStringEncodingError API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 264, // Read error (string encoding of file contents could not be determined)
    NSFileWriteUnknownError = 512,                                          // Write error (reason unknown)
    NSFileWriteNoPermissionError = 513,                                     // Write error (permission problem)
    NSFileWriteInvalidFileNameError = 514,                                  // Write error (invalid file name)
    NSFileWriteFileExistsError API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)) = 516,          // Write error (file exists)
    NSFileWriteInapplicableStringEncodingError = 517,                       // Write error (string encoding not applicable) also NSStringEncodingErrorKey
    NSFileWriteUnsupportedSchemeError = 518,                                // Write error (unsupported URL scheme)
    NSFileWriteOutOfSpaceError = 640,                                       // Write error (out of disk space)
    NSFileWriteVolumeReadOnlyError API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 642,      // Write error (readonly volume)

    // NSFileManager unmount errors
    NSFileManagerUnmountUnknownError API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos) = 768,    // The volume could not be unmounted (reason unknown)
    NSFileManagerUnmountBusyError API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos) = 769,       // The volume could not be unmounted because it is in use

    // Other errors
    NSKeyValueValidationError = 1024,                                       // KVC validation error
    NSFormattingError = 2048,                                               // Formatting error
    NSUserCancelledError = 3072,                                            // User cancelled operation (this one often doesn't deserve a panel and might be a good one to special case)
    NSFeatureUnsupportedError API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = 3328,          // Feature unsupported error
    
    // Executable loading errors
    NSExecutableNotLoadableError API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 3584,           // Executable is of a type that is not loadable in the current process
    NSExecutableArchitectureMismatchError API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 3585,  // Executable does not provide an architecture compatible with the current process
    NSExecutableRuntimeMismatchError API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 3586,       // Executable has Objective C runtime information incompatible with the current process
    NSExecutableLoadError API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 3587,                  // Executable cannot be loaded for some other reason, such as a problem with a library it depends on
    NSExecutableLinkError API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 3588,                  // Executable fails due to linking issues
    
    // Inclusive error range definitions, for checking future error codes
    NSFileErrorMinimum = 0,
    NSFileErrorMaximum = 1023,
    
    NSValidationErrorMinimum = 1024,
    NSValidationErrorMaximum = 2047,

    NSExecutableErrorMinimum API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 3584,
    NSExecutableErrorMaximum API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 3839,

    NSFormattingErrorMinimum = 2048,
    NSFormattingErrorMaximum = 2559,
    
    NSPropertyListReadCorruptError API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 3840,         // Error parsing a property list
    NSPropertyListReadUnknownVersionError API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 3841,  // The version number in the property list is unknown
    NSPropertyListReadStreamError API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 3842,          // Stream error reading a property list
    NSPropertyListWriteStreamError API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 3851,         // Stream error writing a property list
    NSPropertyListWriteInvalidError API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 3852,       // Invalid property list object or invalid property list type specified when writing

    NSPropertyListErrorMinimum API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 3840,
    NSPropertyListErrorMaximum API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) = 4095,

    NSXPCConnectionInterrupted API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = 4097,
    NSXPCConnectionInvalid API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = 4099,
    NSXPCConnectionReplyInvalid API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = 4101,
    
    NSXPCConnectionErrorMinimum API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = 4096,
    NSXPCConnectionErrorMaximum API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = 4224,

    NSUbiquitousFileUnavailableError API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) = 4353, // NSURLUbiquitousItemDownloadingErrorKey contains an error with this code when the item has not been uploaded to iCloud by the other devices yet
    NSUbiquitousFileNotUploadedDueToQuotaError API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) = 4354, // NSURLUbiquitousItemUploadingErrorKey contains an error with this code when the item has not been uploaded to iCloud because it would make the account go over-quota
    NSUbiquitousFileUbiquityServerNotAvailable API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) = 4355, // NSURLUbiquitousItemDownloadingErrorKey and NSURLUbiquitousItemUploadingErrorKey contain an error with this code when connecting to the iCloud servers failed

    NSUbiquitousFileErrorMinimum API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) = 4352,
    NSUbiquitousFileErrorMaximum API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) = 4607,

    NSUserActivityHandoffFailedError API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 4608,	// The data for the user activity was not available (e.g. if the remote device became unavailable.)
    NSUserActivityConnectionUnavailableError API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 4609,  // The user activity could not be continued because a required connection was not available
    NSUserActivityRemoteApplicationTimedOutError API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 4610, // The remote application failed to send data in time
    NSUserActivityHandoffUserInfoTooLargeError API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 4611, // The NSUserActivity userInfo dictionary was too large to receive

    NSUserActivityErrorMinimum API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 4608,
    NSUserActivityErrorMaximum API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 4863,
    
    NSCoderReadCorruptError API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4864, // Error parsing data during decode
    NSCoderValueNotFoundError API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4865, // Data requested was not found
    NSCoderInvalidValueError API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0)) = 4866, // Data was not valid to encode
    NSCoderErrorMinimum API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4864,
    NSCoderErrorMaximum API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4991,
    
    NSBundleErrorMinimum API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4992,
    NSBundleErrorMaximum API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 5119,
    
    NSBundleOnDemandResourceOutOfSpaceError API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4992, // There was not enough space available to download the requested On Demand Resources.
    NSBundleOnDemandResourceExceededMaximumSizeError API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4993, // The application exceeded the amount of On Demand Resources content in use at one time
    NSBundleOnDemandResourceInvalidTagError API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 4994, // The application specified a tag which the system could not find in the application tag manifest
    
    NSCloudSharingNetworkFailureError API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5120,        // Sharing failed due to a network failure.
    NSCloudSharingQuotaExceededError API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5121,         // The user doesn't have enough storage space available to share the requested items.
    NSCloudSharingTooManyParticipantsError API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5122,   // Additional participants could not be added to the share, because the limit was reached.
    NSCloudSharingConflictError API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5123,              // A conflict occurred while trying to save changes to the CKShare and/or root CKRecord. Respond to this error by first fetching the server's changes to the records, then either handle the conflict manually or present it, which will instruct the user to try the operation again.
    NSCloudSharingNoPermissionError API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5124,         // The current user doesn't have permission to perform the requested actions.
    NSCloudSharingOtherError API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5375,                 // These errors may require application-specific responses. For CloudKit sharing, use the NSUnderlyingErrorKey, which is a CKErrorDomain error, to discover the specific error and refer to the CloudKit documentation for the proper response to these errors.
    
    NSCloudSharingErrorMinimum API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5120,
    NSCloudSharingErrorMaximum API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos) = 5375,
    
    NSCompressionFailedError API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) = 5376,
    NSDecompressionFailedError API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) = 5377,
    
    NSCompressionErrorMinimum API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) = 5376,
    NSCompressionErrorMaximum API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) = 5503,

};

