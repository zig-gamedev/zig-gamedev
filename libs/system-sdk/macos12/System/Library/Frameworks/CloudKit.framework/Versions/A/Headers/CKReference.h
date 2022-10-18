//
//  CKReference.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CKRecord, CKRecordID, CKAsset;

NS_ASSUME_NONNULL_BEGIN

/*! @enum CKReferenceAction
 *  @constant CKReferenceActionNone When the referred record is deleted, this record is unchanged, and has a dangling pointer
 *  @constant CKReferenceActionDeleteSelf When the referred record is deleted then this record is also deleted
 */
typedef NS_ENUM(NSUInteger, CKReferenceAction) {
    CKReferenceActionNone       = 0,
    CKReferenceActionDeleteSelf = 1,
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));


API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKReference : NSObject <NSSecureCoding, NSCopying>

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/*! @discussion It is acceptable to relate two records that have not yet been uploaded to the server, but those records must be uploaded to the server in the same operation.
 *
 *  If a record references a record that does not exist on the server and is not in the current save operation it will result in an error.
 */
- (instancetype)initWithRecordID:(CKRecordID *)recordID action:(CKReferenceAction)action NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithRecord:(CKRecord *)record action:(CKReferenceAction)action;

@property (nonatomic, readonly, assign) CKReferenceAction referenceAction;

@property (nonatomic, readonly, copy) CKRecordID *recordID;

@end

NS_ASSUME_NONNULL_END
