//
//  CKShare.h
//  CloudKit
//
//  Copyright (c) 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CloudKit/CKRecord.h>
#import <CloudKit/CKShareParticipant.h>

NS_ASSUME_NONNULL_BEGIN

CK_EXTERN CKRecordType const CKRecordTypeShare API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

/*! A zone-wide CKShare always uses the record name @c CKRecordNameZoneWideShare.
 *  You can use this to fetch the @c CKShare record for the zone with a @c CKFetchRecordsOperation.
 */
CK_EXTERN NSString * const CKRecordNameZoneWideShare API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

/*! Predefined keys in the @c CKRecordTypeShare schema.  They're used by the out of process UI flow to send a share, and as part of the share acceptance flow.  These are optional */

/*! Value is a string.  Example for a recipe sharing app: "Pot Roast" */
CK_EXTERN CKRecordFieldKey const CKShareTitleKey API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));
/*! Value is a data blob suitable to pass into @code -[NSImage imageWithData:] or -[UIImage imageWithData:] @endcode */
CK_EXTERN CKRecordFieldKey const CKShareThumbnailImageDataKey API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));
/*! Value is a string representing a UTI.  Example for a recipe sharing app: "com.mycompany.recipe" */
CK_EXTERN CKRecordFieldKey const CKShareTypeKey API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

/*! @class CKShare
 *
 *  @discussion Like CKRecords, CKShares can store arbitrary key-value pairs.  They are modified and fetched in the same manner.
 *  A share, its root record, and its root record's children records will only appear in a participant's CKFetchRecordChangesOperation's results after the share has been accepted by that participant.
 *  Clients have access to the share (and optionally the root record) before accepting a share, via the CKShareMetadata object.  Note that in order to access a root record before accepting a share, you must run a CKFetchShareMetadataOperation requesting the root record.
 *  A CKShare will appear in a CKFetchRecordChangesOperation's results set whenever the participant list is updated.  For that reason, you shouldn't place heavy key-value pairs in it.
 */

API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKShare : CKRecord <NSSecureCoding, NSCopying>

/*! When saving a newly created CKShare, you must save the share and its rootRecord in the same CKModifyRecordsOperation batch. */
- (instancetype)initWithRootRecord:(CKRecord *)rootRecord;
- (instancetype)initWithRootRecord:(CKRecord *)rootRecord shareID:(CKRecordID *)shareID NS_DESIGNATED_INITIALIZER;

/*! Creates a zone-wide @c CKShare.  A zone-wide @c CKShare can only exist in a zone with sharing capability @c CKRecordZoneCapabilityZoneWideSharing.
 * Only one such share can exist in a zone at a time.
 *
 * All records in this zone will appear in a participant's @c CKFetchRecordZoneChangesOperation results in the shared database after the
 * share has been accepted by the participant.
 *
 * Since these shares do not have an associated root record, @c shouldFetchRootRecord and @c rootRecordDesiredKeys are always ignored when
 * running a @c CKFetchShareMetadataOperation on a zone-wide share URL. Additionally, @c rootRecordID on the resulting @c CKShareMetadata is
 * always absent.
 */
- (instancetype)initWithRecordZoneID:(CKRecordZoneID *)recordZoneID NS_DESIGNATED_INITIALIZER API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

- (instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

/*! @abstract Defines what permission a user has when not explicitly added to the share.
 *
 *  @discussion Shares with @c publicPermission more permissive than @c CKShareParticipantPermissionNone can be joined by any user with access to the share's shareURL.
 *  By default, public permission is @c CKShareParticipantPermissionNone.
 *  Changing the public permission to @c CKShareParticipantPermissionReadOnly or @c CKShareParticipantPermissionReadWrite will result in all pending participants being removed.  Already-accepted participants will remain on the share.
 *  Changing the public permission to @c CKShareParticipantPermissionNone will result in all participants being removed from the share.  You may subsequently choose to call @c addParticipant: before saving the share, those participants will be added to the share.
 */
@property (nonatomic, assign) CKShareParticipantPermission publicPermission;

/*! @abstract A URL that can be used to invite participants to this share.
 *
 *  @discussion Only available after share record has been saved to the server.  This url is stable, and is tied to the rootRecord.  That is, if you share a rootRecord, delete the share, and re-share the same rootRecord via a newly created share, that newly created share's url will be identical to the prior share's url
 */
@property (nonatomic, readonly, copy, nullable) NSURL *URL;

/*! @abstract All participants on the share that the current user has permissions to see.
 *
 *  @discussion At the minimum that will include the owner and the current user.
 */
@property (nonatomic, readonly, copy) NSArray<CKShareParticipant *> *participants;

/*! Convenience methods for fetching special users from the participant array */
@property (nonatomic, readonly, copy) CKShareParticipant *owner;
@property (nonatomic, readonly, copy, nullable) CKShareParticipant *currentUserParticipant;

/*! @discussion If a participant with a matching userIdentity already exists, then that existing participant's properties will be updated; no new participant will be added.
 *  In order to modify the list of participants, a share must have publicPermission set to @c CKShareParticipantPermissionNone.  That is, you cannot mix-and-match private users and public users in the same share.
 *  Only certain participant types may be added via this API
 *  @see CKShareParticipantRole
 */
- (void)addParticipant:(CKShareParticipant *)participant;
- (void)removeParticipant:(CKShareParticipant *)participant;

/*! These superclass-provided initializers are not allowed for CKShare */
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)initWithRecordType:(CKRecordType)recordType NS_UNAVAILABLE;
- (instancetype)initWithRecordType:(CKRecordType)recordType recordID:(CKRecordID *)recordID NS_UNAVAILABLE;
- (instancetype)initWithRecordType:(CKRecordType)recordType zoneID:(CKRecordZoneID *)zoneID NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
