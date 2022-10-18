//
//  CKUserIdentity.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CKRecordID, CKUserIdentityLookupInfo;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKUserIdentity : NSObject <NSSecureCoding, NSCopying>
/*! Use @c CKDiscoverUserIdentitiesOperation or @c CKFetchShareParticipantsOperation to create a @c CKUserIdentity */
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/*! This is the @c lookupInfo you passed in to @c CKDiscoverUserIdentitiesOperation or @c CKFetchShareParticipantsOperation */
@property (nonatomic, readonly, copy, nullable) CKUserIdentityLookupInfo *lookupInfo;

@property (nonatomic, readonly, copy, nullable) NSPersonNameComponents *nameComponents;
@property (nonatomic, readonly, copy, nullable) CKRecordID *userRecordID;

/*! @abstract Link to the Contacts database.
 *
 *  @discussion Identities discovered via @c CKDiscoverAllUserIdentitiesOperation correspond to entries in the local Contacts database.  These identities will have @c contactIdentifiers filled out, which your app may use to get additional information about the contacts that were discovered.  Multiple @c contactIdentifiers may exist for a single discovered user, as multiple contacts may contain the same email addresses or phone numbers.
 *
 *  @return individual, non-unified contacts.
 *
 *  @discussion To transform these identifiers into an array of unified contact identifiers, pass a @c CNContact.predicateForContacts(withIdentifiers:) predicate into @c CNContactStore.unifiedContacts(matching:keysToFetch:)
 *
 *  @see Contacts.framework and CNContact.identifier
 */
@property (nonatomic, readonly, copy) NSArray<NSString *> *contactIdentifiers API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) API_UNAVAILABLE(tvos);


@property (nonatomic, readonly, assign) BOOL hasiCloudAccount;

@end

NS_ASSUME_NONNULL_END
