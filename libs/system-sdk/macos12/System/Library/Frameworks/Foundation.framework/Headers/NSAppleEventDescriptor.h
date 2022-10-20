/*
	NSAppleEventDescriptor.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <CoreServices/CoreServices.h>

@class NSData;

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSAppleEventSendOptions) {
    NSAppleEventSendNoReply        = kAENoReply, /* sender doesn't want a reply to event */
    NSAppleEventSendQueueReply     = kAEQueueReply, /* sender wants a reply but won't wait */
    NSAppleEventSendWaitForReply   = kAEWaitReply, /* sender wants a reply and will wait */
    NSAppleEventSendNeverInteract  = kAENeverInteract, /* server should not interact with user */
    NSAppleEventSendCanInteract    = kAECanInteract, /* server may try to interact with user */
    NSAppleEventSendAlwaysInteract = kAEAlwaysInteract, /* server should always interact with user where appropriate */
    NSAppleEventSendCanSwitchLayer = kAECanSwitchLayer, /* interaction may switch layer */
    NSAppleEventSendDontRecord     = kAEDontRecord, /* don't record this event */
    NSAppleEventSendDontExecute    = kAEDontExecute, /* don't execute this event; used for recording */
    NSAppleEventSendDontAnnotate   = kAEDoNotAutomaticallyAddAnnotationsToEvent, /* don't automatically add any sandbox or other annotations to the event */

    NSAppleEventSendDefaultOptions = NSAppleEventSendWaitForReply | NSAppleEventSendCanInteract
} API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

@interface NSAppleEventDescriptor : NSObject<NSCopying, NSSecureCoding> {
    @private
    AEDesc _desc;
    BOOL _hasValidDesc;
    char _padding[3];
}

// Create an autoreleased NSAppleEventDescriptor whose AEDesc type is typeNull.
+ (NSAppleEventDescriptor *)nullDescriptor;

// Given some data, and a four-character type code, create and return an autoreleased NSAppleEventDescriptor that contains that data, with that type.
+ (nullable NSAppleEventDescriptor *)descriptorWithDescriptorType:(DescType)descriptorType bytes:(nullable const void *)bytes length:(NSUInteger)byteCount;
+ (nullable NSAppleEventDescriptor *)descriptorWithDescriptorType:(DescType)descriptorType data:(nullable NSData *)data;

// Given a value, create and return an autoreleased NSAppleEventDescriptor that contains that value, with an appropriate type (typeBoolean, typeEnumerated, typeSInt32, typeIEEE64BitFloatingPoint, or typeType, respectively).
+ (NSAppleEventDescriptor *)descriptorWithBoolean:(Boolean)boolean;
+ (NSAppleEventDescriptor *)descriptorWithEnumCode:(OSType)enumerator;
+ (NSAppleEventDescriptor *)descriptorWithInt32:(SInt32)signedInt;
+ (NSAppleEventDescriptor *)descriptorWithDouble:(double)doubleValue API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);
+ (NSAppleEventDescriptor *)descriptorWithTypeCode:(OSType)typeCode;

// Given a string, date, or file URL object, respectively, create and return an autoreleased NSAppleEventDescriptor that contains that value.
+ (NSAppleEventDescriptor *)descriptorWithString:(NSString *)string;
+ (NSAppleEventDescriptor *)descriptorWithDate:(NSDate *)date API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);
+ (NSAppleEventDescriptor *)descriptorWithFileURL:(NSURL *)fileURL API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

// Create and return an autoreleased event, list, or record NSAppleEventDescriptor, respectively.
+ (NSAppleEventDescriptor *)appleEventWithEventClass:(AEEventClass)eventClass eventID:(AEEventID)eventID targetDescriptor:(nullable NSAppleEventDescriptor *)targetDescriptor returnID:(AEReturnID)returnID transactionID:(AETransactionID)transactionID;
+ (NSAppleEventDescriptor *)listDescriptor;
+ (NSAppleEventDescriptor *)recordDescriptor;

// Create and return an autoreleased application address descriptor using the current process, a pid, a url referring to an application, or an application bundle identifier, respectively.  The result is suitable for use as the "targetDescriptor" parameter of +appleEventWithEventClass:/initWithEventClass:.
+ (NSAppleEventDescriptor *)currentProcessDescriptor API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);
+ (NSAppleEventDescriptor *)descriptorWithProcessIdentifier:(pid_t)processIdentifier API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);
+ (NSAppleEventDescriptor *)descriptorWithBundleIdentifier:(NSString *)bundleIdentifier API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);
+ (NSAppleEventDescriptor *)descriptorWithApplicationURL:(NSURL *)applicationURL API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

// The designated initializer.  The initialized object takes ownership of the passed-in AEDesc, and will call AEDisposeDesc() on it at deallocation time.
- (instancetype)initWithAEDescNoCopy:(const AEDesc *)aeDesc NS_DESIGNATED_INITIALIZER;

// Other initializers.
- (nullable instancetype)initWithDescriptorType:(DescType)descriptorType bytes:(nullable const void *)bytes length:(NSUInteger)byteCount;
- (nullable instancetype)initWithDescriptorType:(DescType)descriptorType data:(nullable NSData *)data;
- (instancetype)initWithEventClass:(AEEventClass)eventClass eventID:(AEEventID)eventID targetDescriptor:(nullable NSAppleEventDescriptor *)targetDescriptor returnID:(AEReturnID)returnID transactionID:(AETransactionID)transactionID;
- (instancetype)initListDescriptor;
- (instancetype)initRecordDescriptor;

// Return a pointer to the AEDesc that is encapsulated by the object.
@property (nullable, readonly) const AEDesc *aeDesc NS_RETURNS_INNER_POINTER;

// Get the four-character type code or the data from a fully-initialized descriptor.
@property (readonly) DescType descriptorType;
@property (readonly, copy) NSData *data;

// Return the contents of a descriptor, after coercing the descriptor's contents to typeBoolean, typeEnumerated, typeSInt32, typeIEEE64BitFloatingPoint, or typeType, respectively.
@property (readonly) Boolean booleanValue;
@property (readonly) OSType enumCodeValue;
@property (readonly) SInt32 int32Value;
@property (readonly) double doubleValue API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);
@property (readonly) OSType typeCodeValue;

// Return the contents of a descriptor, after coercing the descriptor's contents to a string, date, or file URL, respectively.
@property (nullable, readonly, copy) NSString *stringValue;
@property (nullable, readonly, copy) NSDate *dateValue API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);
@property (nullable, readonly, copy) NSURL *fileURLValue API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

// Accessors for an event descriptor.
@property (readonly) AEEventClass eventClass;
@property (readonly) AEEventID eventID;
@property (readonly) AEReturnID returnID;
@property (readonly) AETransactionID transactionID;

// Set, retrieve, or remove parameter descriptors inside an event descriptor.
- (void)setParamDescriptor:(NSAppleEventDescriptor *)descriptor forKeyword:(AEKeyword)keyword;
- (nullable NSAppleEventDescriptor *)paramDescriptorForKeyword:(AEKeyword)keyword;
- (void)removeParamDescriptorWithKeyword:(AEKeyword)keyword;

// Set or retrieve attribute descriptors inside an event descriptor.
- (void)setAttributeDescriptor:(NSAppleEventDescriptor *)descriptor forKeyword:(AEKeyword)keyword;
- (nullable NSAppleEventDescriptor *)attributeDescriptorForKeyword:(AEKeyword)keyword;

// Send an Apple event.
- (nullable NSAppleEventDescriptor *)sendEventWithOptions:(NSAppleEventSendOptions)sendOptions timeout:(NSTimeInterval)timeoutInSeconds error:(NSError **)error API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

// Return whether or not a descriptor is a record-like descriptor.  Record-like descriptors function as records, but may have a descriptorType other than typeAERecord, such as typeObjectSpecifier.
@property (readonly) BOOL isRecordDescriptor API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios, watchos, tvos);

// Return the number of items inside a list or record descriptor.
@property (readonly) NSInteger numberOfItems;

// Set, retrieve, or remove indexed descriptors inside a list or record descriptor.
- (void)insertDescriptor:(NSAppleEventDescriptor *)descriptor atIndex:(NSInteger)index;
- (nullable NSAppleEventDescriptor *)descriptorAtIndex:(NSInteger)index;
- (void)removeDescriptorAtIndex:(NSInteger)index;

// Set, retrieve, or remove keyed descriptors inside a record descriptor.
- (void)setDescriptor:(NSAppleEventDescriptor *)descriptor forKeyword:(AEKeyword)keyword;
- (nullable NSAppleEventDescriptor *)descriptorForKeyword:(AEKeyword)keyword;
- (void)removeDescriptorWithKeyword:(AEKeyword)keyword;

// Return the keyword associated with an indexed descriptor inside a record descriptor.
- (AEKeyword)keywordForDescriptorAtIndex:(NSInteger)index;

// Create and return a descriptor of the requested type, doing a coercion if that's appropriate and possible.
- (nullable NSAppleEventDescriptor *)coerceToDescriptorType:(DescType)descriptorType;

@end

NS_ASSUME_NONNULL_END
