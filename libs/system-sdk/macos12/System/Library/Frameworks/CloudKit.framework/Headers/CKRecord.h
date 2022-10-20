//
//  CKRecord.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CoreLocation/CLLocation.h>
#import <Foundation/Foundation.h>

#import <CloudKit/CKAsset.h>
#import <CloudKit/CKDefines.h>
#import <CloudKit/CKReference.h>

@class CKRecordID, CKRecordZoneID;

NS_ASSUME_NONNULL_BEGIN

typedef NSString *CKRecordType;
typedef NSString *CKRecordFieldKey;

/*! Use this constant for the recordType parameter when fetching User Records. */
CK_EXTERN CKRecordType const CKRecordTypeUserRecord API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

/*! Use these keys in queries to match on the record's parent or share reference */
CK_EXTERN CKRecordFieldKey const CKRecordParentKey API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));
CK_EXTERN CKRecordFieldKey const CKRecordShareKey API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

@protocol CKRecordValue <NSObject>
@end

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKRecord : NSObject <NSSecureCoding, NSCopying>

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/*! This creates the record in the default zone. */
- (instancetype)initWithRecordType:(CKRecordType)recordType;

- (instancetype)initWithRecordType:(CKRecordType)recordType recordID:(CKRecordID *)recordID;
- (instancetype)initWithRecordType:(CKRecordType)recordType zoneID:(CKRecordZoneID *)zoneID;

@property (nonatomic, readonly, copy) CKRecordType recordType;
@property (nonatomic, readonly, copy) CKRecordID *recordID;

/*! Change tags are updated by the server to a unique value every time a record is modified.  A different change tag necessarily means that the contents of the record are different. */
@property (nonatomic, readonly, copy, nullable) NSString *recordChangeTag;

/*! This is a User Record recordID, identifying the user that created this record. */
@property (nonatomic, readonly, copy, nullable) CKRecordID *creatorUserRecordID;
@property (nonatomic, readonly, copy, nullable) NSDate *creationDate;

/*! This is a User Record recordID, identifying the user that last modified this record. */
@property (nonatomic, readonly, copy, nullable) CKRecordID *lastModifiedUserRecordID;
@property (nonatomic, readonly, copy, nullable) NSDate *modificationDate;

/*! @discussion In addition to @c objectForKey: and @c setObject:forKey:, dictionary-style subscripting (@c record[key] and @code record[key] = value @endcode) can be used to get and set values.
 *  Acceptable value object classes are:
 *  - CKReference
 *  - CKAsset
 *  - CLLocation
 *  - NSData
 *  - NSDate
 *  - NSNumber
 *  - NSString
 *  - NSArray containing objects of any of the types above
 *
 *  Any other classes will result in an exception with name @c NSInvalidArgumentException.
 *
 *  Field keys starting with '_' are reserved. Attempting to set a key prefixed with a '_' will result in an error.
 *
 *  Key names roughly match C variable name restrictions. They must begin with an ASCII letter and can contain ASCII letters and numbers and the underscore character.
 *  The maximum key length is 255 characters.
 */
- (nullable __kindof id<CKRecordValue>)objectForKey:(CKRecordFieldKey)key;
- (void)setObject:(nullable __kindof id<CKRecordValue>)object forKey:(CKRecordFieldKey)key;
- (NSArray<CKRecordFieldKey> *)allKeys;

/*! @abstract A special property that returns an array of token generated from all the string field values in the record.
 *
 *  @discussion These tokens have been normalized for the current locale, so they are suitable for performing full-text searches.
 */
- (NSArray<NSString *> *)allTokens;

- (nullable __kindof id<CKRecordValue>)objectForKeyedSubscript:(CKRecordFieldKey)key;
- (void)setObject:(nullable __kindof id<CKRecordValue>)object forKeyedSubscript:(CKRecordFieldKey)key;

/*! A list of keys that have been modified on the local CKRecord instance */
- (NSArray<CKRecordFieldKey> *)changedKeys;

/*! @discussion @c CKRecord supports @c NSSecureCoding.  When you invoke @c encodeWithCoder: on a @c CKRecord, it encodes all its values.  Including the record values you've set.
 *  If you want to store a @c CKRecord instance locally, AND you're already storing the record values locally, that's overkill.  In that case, you can use @c encodeSystemFieldsWithCoder:.  This will encode all parts of a @c CKRecord except the record keys / values you have access to via the @c changedKeys and @c objectForKey: methods.
 *  If you use @c initWithCoder: to reconstitute a @c CKRecord you encoded via @c encodeSystemFieldsWithCoder:, then be aware that
 *  - any record values you had set on the original instance, but had not saved, will be lost
 *  - the reconstituted CKRecord's @c changedKeys will be empty
 */
- (void)encodeSystemFieldsWithCoder:(NSCoder *)coder;

/*! @discussion The share property on a record can be set by creating a share using @code -[CKShare initWithRootRecord:] @endcode.
 *
 *  The share property on a record will be removed when the corresponding CKShare is deleted from the server. Send this record in the same batch as the share delete and this record's share property will be updated.
 *
 *  Sharing is only supported in zones with the @c CKRecordZoneCapabilitySharing capability. The default zone does not support sharing.
 *
 *  If any records have a parent reference to this record, they are implicitly shared alongside this record.
 *
 *  Note that records in a parent chain must only exist within one share. If a child record already has a share reference set then you will get a @c CKErrorAlreadyShared error if you try to share any of that record's parents.
 *
 *  Child records can be shared independently, even if they have a common parent.  For example:
 *  Record A has two child records, Record B and Record C.
 *      A
 *     / \
 *    B   C
 *
 *  These configurations are supported:
 *  - Record A part of Share 1, or
 *  - Record B part of Share 1, or
 *  - Record C part of Share 1, or
 *  - Record B part of Share 1, Record C part of Share 2
 *
 *  These configurations are not supported:
 *  Record A part of Share 1, Record B part of Share 2, or
 *    -- This is not allowed because Record B would then be in two shares; Share 1 by being Record A's child, and Share 2
 *  Record A part of Share 1, Record C part of Share 2, or
 *    -- This is not allowed because Record C would then be in two shares; Share 1 by being Record A's child, and Share 2
 *  Record A part of Share 1, Record B part of Share 2, Record C part of Share 3
 *    -- This is not allowed because both Record B and Record C would then each be in two shares.
 *
 *  Whenever possible, it is suggested that you construct your parent hierarchies such that you will only need to share the topmost record of that hierarchy.
 */
@property (nonatomic, readonly, copy, nullable) CKReference *share API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

/*! @abstract Use a parent reference to teach CloudKit about the hierarchy of your records.
 *
 *  @discussion When a record is shared, all children of that record are also shared.
 *
 *  A parent record reference must have @c CKReferenceActionNone set. You can create a separate reference with @c CKReferenceActionDeleteSelf if you would like your hierarchy cleaned up when the parent record is deleted.
 *
 *  The target of a parent reference must exist at save time - either already on the server, or part of the same @c CKModifyRecordsOperation batch.
 *
 *  You are encouraged to set up the @c parent relationships as part of normal record saves, even if you're not planning on sharing records at this time.
 *  This allows you to share and unshare a hierarchy of records at a later date by only modifying the "top level" record, setting or clearing its @c share reference.
 */
@property (nonatomic, copy, nullable) CKReference *parent API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

/*! Convenience wrappers around creating a @c CKReference to a parent record. The resulting @c CKReference will have @code referenceAction = CKReferenceActionNone @endcode */
- (void)setParentReferenceFromRecord:(nullable CKRecord *)parentRecord API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));
- (void)setParentReferenceFromRecordID:(nullable CKRecordID *)parentRecordID API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

@end

@interface NSString (CKRecordValue) <CKRecordValue>
@end

@interface NSNumber (CKRecordValue) <CKRecordValue>
@end

@interface NSArray (CKRecordValue) <CKRecordValue>
@end

@interface NSDate (CKRecordValue) <CKRecordValue>
@end

@interface NSData (CKRecordValue) <CKRecordValue>
@end

@interface CKReference (CKRecordValue) <CKRecordValue>
@end

@interface CKAsset (CKRecordValue) <CKRecordValue>
@end

@interface CLLocation (CKRecordValue) <CKRecordValue>
@end

/*! Formalizes a protocol for getting and setting keys on a CKRecord.  Not intended to be used directly by client code */
API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0))
@protocol CKRecordKeyValueSetting <NSObject>
- (nullable __kindof id<CKRecordValue>)objectForKey:(CKRecordFieldKey)key;
- (void)setObject:(nullable __kindof id<CKRecordValue>)object forKey:(CKRecordFieldKey)key;
- (nullable __kindof id<CKRecordValue>)objectForKeyedSubscript:(CKRecordFieldKey)key;
- (void)setObject:(nullable __kindof id<CKRecordValue>)object forKeyedSubscript:(CKRecordFieldKey)key;
- (NSArray<CKRecordFieldKey> *)allKeys;
- (NSArray<CKRecordFieldKey> *)changedKeys;
@end

API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0))
@interface CKRecord(CKRecordKeyValueSettingConformance) <CKRecordKeyValueSetting>

/*! Any values set here will be locally encrypted before being saved to the server and locally decrypted when fetched from the server. Encryption and decryption is handled by the CloudKit framework.
 * Key material necessary for decryption are available to the owner of the record, as well as any users that can access this record via a CKShare.
 * All CKRecordValue types can be set here except CKAsset and CKReference.
 */
@property (nonatomic, readonly, copy) id<CKRecordKeyValueSetting> encryptedValues API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));
                                                                                                     
@end

NS_ASSUME_NONNULL_END
