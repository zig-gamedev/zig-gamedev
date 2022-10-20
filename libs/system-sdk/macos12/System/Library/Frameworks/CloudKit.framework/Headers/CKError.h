//
//  CKError.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CloudKit/CKDefines.h>

NS_ASSUME_NONNULL_BEGIN

CK_EXTERN NSString * const CKErrorDomain API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

/*! @abstract When a CKErrorPartialFailure happens this key will be set in the error's userInfo dictionary.
 *
 *  @discussion The value of this key will be a dictionary, and the values will be errors for individual items with the keys being the item IDs that failed.
 */
CK_EXTERN NSString * const CKPartialErrorsByItemIDKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

/*! If the server rejects a record save because it has been modified since the last time it was read, a @c CKErrorServerRecordChanged error will be returned and it will contain versions of the record in its userInfo dictionary. Apply your custom conflict resolution logic to the server record under @c CKServerRecordKey and attempt a save of that record. */
CK_EXTERN NSString * const CKRecordChangedErrorAncestorRecordKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));
CK_EXTERN NSString * const CKRecordChangedErrorServerRecordKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));
CK_EXTERN NSString * const CKRecordChangedErrorClientRecordKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

/* On error CKErrorZoneNotFound, the userInfo dictionary may contain a NSNumber instance that specifies a boolean value representing if the error is caused by the user having reset all encrypted data for their account */
CK_EXTERN NSString * const CKErrorUserDidResetEncryptedDataKey API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

/*! On some errors, the userInfo dictionary may contain a NSNumber instance that specifies the period of time in seconds after which the client may retry the request. For example, this key will be on @c CKErrorServiceUnavailable, @c CKErrorRequestRateLimited, and other errors for which the recommended resolution is to retry after a delay.
 */
CK_EXTERN NSString * const CKErrorRetryAfterKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

typedef NS_ENUM(NSInteger, CKErrorCode) {
    /*! CloudKit.framework encountered an error.  This is a non-recoverable error. */
    CKErrorInternalError                  = 1,
    
    /*! Some items failed, but the operation succeeded overall. Check CKPartialErrorsByItemIDKey in the userInfo dictionary for more details.
     *  This error is only returned from CKOperation completion blocks, which are deprecated in swift.
     *  It will not be returned from (swift-only) CKOperation result blocks, which are their replacements
     */
    CKErrorPartialFailure                 = 2,
    
    /*! Network not available */
    CKErrorNetworkUnavailable             = 3,
    
    /*! Network error (available but CFNetwork gave us an error) */
    CKErrorNetworkFailure                 = 4,
    
    /*! Un-provisioned or unauthorized container. Try provisioning the container before retrying the operation. */
    CKErrorBadContainer                   = 5,
    
    /*! Service unavailable */
    CKErrorServiceUnavailable             = 6,
    
    /*! Client is being rate limited */
    CKErrorRequestRateLimited             = 7,
    
    /*! Missing entitlement */
    CKErrorMissingEntitlement             = 8,
    
    /*! Not authenticated (writing without being logged in, no user record) */
    CKErrorNotAuthenticated               = 9,
    
    /*! Access failure (save, fetch, or shareAccept) */
    CKErrorPermissionFailure              = 10,
    
    /*! Record does not exist */
    CKErrorUnknownItem                    = 11,
    
    /*! Bad client request (bad record graph, malformed predicate) */
    CKErrorInvalidArguments               = 12,
    
    CKErrorResultsTruncated API_DEPRECATED("Will not be returned", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0), watchos(3.0, 3.0)) = 13,
    
    /*! The record was rejected because the version on the server was different */
    CKErrorServerRecordChanged            = 14,
    
    /*! The server rejected this request. This is a non-recoverable error */
    CKErrorServerRejectedRequest          = 15,
    
    /*! Asset file was not found */
    CKErrorAssetFileNotFound              = 16,
    
    /*! Asset file content was modified while being saved */
    CKErrorAssetFileModified              = 17,
    
    /*! App version is less than the minimum allowed version */
    CKErrorIncompatibleVersion            = 18,
    
    /*! The server rejected the request because there was a conflict with a unique field. */
    CKErrorConstraintViolation            = 19,
    
    /*! A CKOperation was explicitly cancelled */
    CKErrorOperationCancelled             = 20,
    
    /*! The previousServerChangeToken value is too old and the client must re-sync from scratch */
    CKErrorChangeTokenExpired             = 21,
    
    /*! One of the items in this batch operation failed in a zone with atomic updates, so the entire batch was rejected. */
    CKErrorBatchRequestFailed             = 22,
    
    /*! The server is too busy to handle this zone operation. Try the operation again in a few seconds. */
    CKErrorZoneBusy                       = 23,
    
    /*! Operation could not be completed on the given database. Likely caused by attempting to modify zones in the public database. */
    CKErrorBadDatabase                    = 24,
    
    /*! Saving a record would exceed quota */
    CKErrorQuotaExceeded                  = 25,
    
    /*! The specified zone does not exist on the server */
    CKErrorZoneNotFound                   = 26,
    
    /*! The request to the server was too large. Retry this request as a smaller batch. */
    CKErrorLimitExceeded                  = 27,
    
    /*! The user deleted this zone through the settings UI. Your client should either remove its local data or prompt the user before attempting to re-upload any data to this zone. */
    CKErrorUserDeletedZone                = 28,
    
    /*! A share cannot be saved because there are too many participants attached to the share */
    CKErrorTooManyParticipants            API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0)) = 29,
    
    /*! A record/share cannot be saved, doing so would cause a hierarchy of records to exist in multiple shares */
    CKErrorAlreadyShared                  API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0)) = 30,
    
    /*! The target of a record's parent or share reference was not found */
    CKErrorReferenceViolation             API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0)) = 31,
    
    /*! Request was rejected due to a managed account restriction */
    CKErrorManagedAccountRestricted       API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0)) = 32,
    
    /*! Share Metadata cannot be determined, because the user is not a member of the share.  There are invited participants on the share with email addresses or phone numbers not associated with any iCloud account. The user may be able to join the share if they can associate one of those email addresses or phone numbers with their iCloud account via the system Share Accept UI. Call UIApplication's openURL on this share URL to have the user attempt to verify their information. */
    CKErrorParticipantMayNeedVerification API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0)) = 33,
    
    /*! The server received and processed this request, but the response was lost due to a network failure.  There is no guarantee that this request succeeded.  Your client should re-issue the request (if it is idempotent), or fetch data from the server to determine if the request succeeded. */
    CKErrorServerResponseLost             API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0), watchos(4.0)) = 34,
    
    /*! The file for this asset could not be accessed. It is likely your application does not have permission to open the file, or the file may be temporarily unavailable due to its data protection class. This operation can be retried after it is able to be opened in your process. */
    CKErrorAssetNotAvailable              API_AVAILABLE(macos(10.13), ios(11.3), tvos(11.3), watchos(4.3)) = 35,

    /*! The current account is in a state that may need user intervention to recover from. The user should be directed to check the Settings app. Listen for CKAccountChangedNotifications to know when to re-check account status and retry. */
    CKErrorAccountTemporarilyUnavailable  API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) = 36
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

NS_ASSUME_NONNULL_END
