/*	
    NSURLError.h
    Copyright (c) 2003-2019, Apple Inc. All rights reserved.
    
    Public header file.
*/

#import <TargetConditionals.h>

#if TARGET_OS_IPHONE
#if __has_include(<CFNetwork/CFNetwork.h>)
#import <CFNetwork/CFNetwork.h>
#endif
#else
#import <CoreServices/CoreServices.h>
#endif

#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSError.h>

@class NSString;

NS_ASSUME_NONNULL_BEGIN

/*
    @discussion Constants used by NSError to differentiate between "domains" of error codes, serving as a discriminator for error codes that originate from different subsystems or sources.
    @constant NSURLErrorDomain Indicates an NSURL error.
*/
FOUNDATION_EXPORT NSErrorDomain const NSURLErrorDomain;

/*!
    @const NSURLErrorFailingURLErrorKey
    @abstract The NSError userInfo dictionary key used to store and retrieve the URL which caused a load to fail.
*/
FOUNDATION_EXPORT NSString * const NSURLErrorFailingURLErrorKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/*!
    @const NSURLErrorFailingURLStringErrorKey
    @abstract The NSError userInfo dictionary key used to store and retrieve the NSString object for the URL which caused a load to fail.
    @discussion This constant supersedes NSErrorFailingURLStringKey, which was deprecated in Mac OS X 10.6.  Both constants refer to the same value for backward-compatibility, but this symbol name has a better prefix.
*/
FOUNDATION_EXPORT NSString * const NSURLErrorFailingURLStringErrorKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/*!
    @const NSErrorFailingURLStringKey
    @abstract The NSError userInfo dictionary key used to store and retrieve the NSString object for the URL which caused a load to fail.
    @discussion This constant is deprecated in Mac OS X 10.6, and is superseded by NSURLErrorFailingURLStringErrorKey.  Both constants refer to the same value for backward-compatibility, but the new symbol name has a better prefix.
*/
FOUNDATION_EXPORT NSString * const NSErrorFailingURLStringKey API_DEPRECATED("Use NSURLErrorFailingURLStringErrorKey instead", macos(10.0,10.6), ios(2.0,4.0), watchos(2.0,2.0), tvos(9.0,9.0));

/*!
    @const NSURLErrorFailingURLPeerTrustErrorKey
    @abstract The NSError userInfo dictionary key used to store and retrieve the SecTrustRef object representing the state of a failed SSL handshake.
 */
FOUNDATION_EXPORT NSString * const NSURLErrorFailingURLPeerTrustErrorKey API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/*!
    @const NSURLErrorBackgroundTaskCancelledReasonKey
    @abstract The NSError userInfo dictionary key used to store and retrieve the NSNumber corresponding to the reason why a background
	NSURLSessionTask was cancelled
 */
FOUNDATION_EXPORT NSString * const NSURLErrorBackgroundTaskCancelledReasonKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/*!
    @enum Codes associated with NSURLErrorBackgroundTaskCancelledReasonKey
    @abstract Constants used by NSError to indicate why a background NSURLSessionTask was cancelled.
 */

NS_ENUM(NSInteger)
{
    NSURLErrorCancelledReasonUserForceQuitApplication =    0,
    NSURLErrorCancelledReasonBackgroundUpdatesDisabled =   1,
    NSURLErrorCancelledReasonInsufficientSystemResources API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = 2,
    
} API_AVAILABLE(macos(10.10), ios(7.0), watchos(2.0), tvos(9.0));

/*!
    @const NSURLErrorNetworkUnavailableReasonKey
    @abstract The NSErrorUserInfoKey used to store and retrieve the NSNumber object corresponding to the reason why the network is unavailable when the task failed due to unsatisfiable network constraints.  See the NSURLErrorNetworkUnavailableReason enum for details.
*/
FOUNDATION_EXPORT NSErrorUserInfoKey const NSURLErrorNetworkUnavailableReasonKey API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*!
    @enum Codes associated with NSURLErrorNetworkUnavailableReasonKey
    @abstract Constants used by NSError to indicate that a URLSessionTask failed because of unsatisfiable network constraints.
    @discussion For example if the URLSessionConfiguration property allowsExpensiveNetworkAccess was set to NO and the only interfaces available were marked as expensive then the task would fail with a NSURLErrorNotConnectedToInternet error and the userInfo dictionary would contain the value NSURLErrorNetworkUnavailableReasonExpensive for the key NSURLErrorNetworkUnavailableReason.
*/

typedef NS_ENUM(NSInteger, NSURLErrorNetworkUnavailableReason)
{
    NSURLErrorNetworkUnavailableReasonCellular =    0,
    NSURLErrorNetworkUnavailableReasonExpensive =   1,
    NSURLErrorNetworkUnavailableReasonConstrained = 2,
} API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*!
    @enum NSURL-related Error Codes
    @abstract Constants used by NSError to indicate errors in the NSURL domain
*/
NS_ERROR_ENUM(NSURLErrorDomain)
{
    NSURLErrorUnknown = 			-1,
    NSURLErrorCancelled = 			-999,
    NSURLErrorBadURL = 				-1000,
    NSURLErrorTimedOut = 			-1001,
    NSURLErrorUnsupportedURL = 			-1002,
    NSURLErrorCannotFindHost = 			-1003,
    NSURLErrorCannotConnectToHost = 		-1004,
    NSURLErrorNetworkConnectionLost = 		-1005,
    NSURLErrorDNSLookupFailed = 		-1006,
    NSURLErrorHTTPTooManyRedirects = 		-1007,
    NSURLErrorResourceUnavailable = 		-1008,
    NSURLErrorNotConnectedToInternet = 		-1009,
    NSURLErrorRedirectToNonExistentLocation = 	-1010,
    NSURLErrorBadServerResponse = 		-1011,
    NSURLErrorUserCancelledAuthentication = 	-1012,
    NSURLErrorUserAuthenticationRequired = 	-1013,
    NSURLErrorZeroByteResource = 		-1014,
    NSURLErrorCannotDecodeRawData =             -1015,
    NSURLErrorCannotDecodeContentData =         -1016,
    NSURLErrorCannotParseResponse =             -1017,
    NSURLErrorAppTransportSecurityRequiresSecureConnection API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = -1022,
    NSURLErrorFileDoesNotExist = 		-1100,
    NSURLErrorFileIsDirectory = 		-1101,
    NSURLErrorNoPermissionsToReadFile = 	-1102,
    NSURLErrorDataLengthExceedsMaximum API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) =	-1103,
    NSURLErrorFileOutsideSafeArea API_AVAILABLE(macos(10.12.4), ios(10.3), watchos(3.2), tvos(10.2)) = -1104,
    
    // SSL errors
    NSURLErrorSecureConnectionFailed = 		-1200,
    NSURLErrorServerCertificateHasBadDate = 	-1201,
    NSURLErrorServerCertificateUntrusted = 	-1202,
    NSURLErrorServerCertificateHasUnknownRoot = -1203,
    NSURLErrorServerCertificateNotYetValid = 	-1204,
    NSURLErrorClientCertificateRejected = 	-1205,
    NSURLErrorClientCertificateRequired =	-1206,
    NSURLErrorCannotLoadFromNetwork = 		-2000,
    
    // Download and file I/O errors
    NSURLErrorCannotCreateFile = 		-3000,
    NSURLErrorCannotOpenFile = 			-3001,
    NSURLErrorCannotCloseFile = 		-3002,
    NSURLErrorCannotWriteToFile = 		-3003,
    NSURLErrorCannotRemoveFile = 		-3004,
    NSURLErrorCannotMoveFile = 			-3005,
    NSURLErrorDownloadDecodingFailedMidStream = -3006,
    NSURLErrorDownloadDecodingFailedToComplete =-3007,

    NSURLErrorInternationalRoamingOff API_AVAILABLE(macos(10.7), ios(3.0), watchos(2.0), tvos(9.0)) =         -1018,
    NSURLErrorCallIsActive API_AVAILABLE(macos(10.7), ios(3.0), watchos(2.0), tvos(9.0)) =                    -1019,
    NSURLErrorDataNotAllowed API_AVAILABLE(macos(10.7), ios(3.0), watchos(2.0), tvos(9.0)) =                  -1020,
    NSURLErrorRequestBodyStreamExhausted API_AVAILABLE(macos(10.7), ios(3.0), watchos(2.0), tvos(9.0)) =      -1021,
    
    NSURLErrorBackgroundSessionRequiresSharedContainer API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = -995,
    NSURLErrorBackgroundSessionInUseByAnotherProcess API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)) = -996,
    NSURLErrorBackgroundSessionWasDisconnected API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))= -997,
};

NS_ASSUME_NONNULL_END

