//
//  CKShareParticipant.h
//  CloudKit
//
//  Copyright (c) 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CKUserIdentity;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, CKShareParticipantAcceptanceStatus) {
    CKShareParticipantAcceptanceStatusUnknown,
    CKShareParticipantAcceptanceStatusPending,
    CKShareParticipantAcceptanceStatusAccepted,
    CKShareParticipantAcceptanceStatusRemoved,
} API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

/*! These permissions determine what share participants can do with records inside that share */
typedef NS_ENUM(NSInteger, CKShareParticipantPermission) {
    CKShareParticipantPermissionUnknown,
    CKShareParticipantPermissionNone,
    CKShareParticipantPermissionReadOnly,
    CKShareParticipantPermissionReadWrite,
} API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

/*! @abstract The participant type determines whether a participant can modify the list of participants on a share.
 *
 *  @discussion
 *  - Owners can add private users
 *  - Private users can access the share
 *  - Public users are "self-added" when the participant accesses the shareURL.  Owners cannot add public users.
 */
typedef NS_ENUM(NSInteger, CKShareParticipantRole) {
    CKShareParticipantRoleUnknown = 0,
    CKShareParticipantRoleOwner = 1,
    CKShareParticipantRolePrivateUser = 3,
    CKShareParticipantRolePublicUser = 4,
} API_AVAILABLE(macos(10.14), ios(12.0), tvos(12.0), watchos(5.0));

typedef NS_ENUM(NSInteger, CKShareParticipantType) {
    CKShareParticipantTypeUnknown = 0,
    CKShareParticipantTypeOwner = 1,
    CKShareParticipantTypePrivateUser = 3,
    CKShareParticipantTypePublicUser = 4,
} API_DEPRECATED_WITH_REPLACEMENT("CKShareParticipantRole", macos(10.12, 10.14), ios(10.0, 12.0), tvos(10.0, 12.0), watchos(3.0, 5.0));

API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKShareParticipant : NSObject <NSSecureCoding, NSCopying>

/*! Use @c CKFetchShareParticipantsOperation to create a @c CKShareParticipant object */
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property (nonatomic, readonly, copy) CKUserIdentity *userIdentity;

/*! The default participant role is @c CKShareParticipantRolePrivateUser. */
@property (nonatomic, assign) CKShareParticipantRole role API_AVAILABLE(macos(10.14), ios(12.0), tvos(12.0), watchos(5.0));

/*! The default participant type is @c CKShareParticipantTypePrivateUser. */
@property (nonatomic, assign) CKShareParticipantType type API_DEPRECATED_WITH_REPLACEMENT("role", macos(10.12, 10.14), ios(10.0, 12.0), tvos(10.0, 12.0), watchos(3.0, 5.0));

@property (nonatomic, readonly, assign) CKShareParticipantAcceptanceStatus acceptanceStatus;

/*! The default permission for a new participant is @c CKShareParticipantPermissionReadOnly. */
@property (nonatomic, assign) CKShareParticipantPermission permission;

@end

NS_ASSUME_NONNULL_END
