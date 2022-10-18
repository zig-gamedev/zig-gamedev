/*
    NSPersistentCloudKitContainerEvent.h
    Core Data
    Copyright (c) 2020-2021, Apple Inc.
    All rights reserved.
*/


#import <Foundation/NSUUID.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSString.h>
#import <Foundation/NSNotification.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSPersistentCloudKitContainerEventType) {
    NSPersistentCloudKitContainerEventTypeSetup,
    NSPersistentCloudKitContainerEventTypeImport,
    NSPersistentCloudKitContainerEventTypeExport
} API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0)) NS_SWIFT_NAME(NSPersistentCloudKitContainer.EventType);

COREDATA_EXTERN NSNotificationName const NSPersistentCloudKitContainerEventChangedNotification API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0)) NS_SWIFT_NAME(NSPersistentCloudKitContainer.eventChangedNotification);
COREDATA_EXTERN NSString * const NSPersistentCloudKitContainerEventUserInfoKey API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0)) NS_SWIFT_NAME(NSPersistentCloudKitContainer.eventNotificationUserInfoKey);

API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0))
NS_SWIFT_NAME(NSPersistentCloudKitContainer.Event)
@interface NSPersistentCloudKitContainerEvent : NSObject<NSCopying>
@property(nonatomic, strong, readonly) NSUUID *identifier;
@property(nonatomic, strong, readonly) NSString *storeIdentifier;
@property(nonatomic, readonly) NSPersistentCloudKitContainerEventType type;
@property(nonatomic, strong, readonly) NSDate *startDate;
@property(nonatomic, strong, nullable, readonly) NSDate *endDate;
@property(nonatomic, assign, readonly) BOOL succeeded;
@property(nonatomic, strong, nullable, readonly) NSError *error;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END

