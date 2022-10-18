/*
    NSAttributeDescription.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <CoreData/NSPropertyDescription.h>

NS_ASSUME_NONNULL_BEGIN

@class NSEntityDescription;

// types explicitly distinguish between bit sizes to ensure data store independence of the underlying operating system


typedef NS_ENUM(NSUInteger, NSAttributeType) {
    NSUndefinedAttributeType = 0,
    NSInteger16AttributeType = 100,
    NSInteger32AttributeType = 200,
    NSInteger64AttributeType = 300,
    NSDecimalAttributeType = 400,
    NSDoubleAttributeType = 500,
    NSFloatAttributeType = 600,
    NSStringAttributeType = 700,
    NSBooleanAttributeType = 800,
    NSDateAttributeType = 900,
    NSBinaryDataAttributeType = 1000,
    NSUUIDAttributeType API_AVAILABLE(macosx(10.13), ios(11.0), tvos(11.0), watchos(4.0)) = 1100,
    NSURIAttributeType API_AVAILABLE(macosx(10.13), ios(11.0), tvos(11.0), watchos(4.0)) = 1200,
    NSTransformableAttributeType API_AVAILABLE(macosx(10.5), ios(3.0)) = 1800, // If your attribute is of NSTransformableAttributeType, the attributeValueClassName must be set or attribute value class must implement NSCopying.
    NSObjectIDAttributeType API_AVAILABLE(macosx(10.6), ios(3.0)) = 2000
 };

// Attributes represent individual values like strings, numbers, dates, etc.
API_AVAILABLE(macosx(10.4),ios(3.0))
@interface NSAttributeDescription : NSPropertyDescription {
}

// NSUndefinedAttributeType is valid for transient properties - Core Data will still track the property as an id value and register undo/redo actions, etc. NSUndefinedAttributeType is illegal for non-transient properties.
@property () NSAttributeType attributeType;

@property (nullable, copy) NSString *attributeValueClassName;

@property (nullable, retain) id defaultValue;   // value is retained and not copied

/* Returns the version hash for the attribute.  This value includes the versionHash information from the NSPropertyDescription superclass, and the attribute type.*/
@property (readonly, copy) NSData *versionHash API_AVAILABLE(macosx(10.5),ios(3.0));

/* The name of the transformer used to convert a NSTransformedAttributeType.  The transformer must output NSData from transformValue and allow reverse transformation.  If this value is not set, or set to nil, Core Data will default to using a transformer which uses NSCoding to archive/unarchive the attribute value.*/
@property (nullable, copy) NSString *valueTransformerName API_AVAILABLE(macosx(10.5),ios(3.0));

@property () BOOL allowsExternalBinaryDataStorage API_AVAILABLE(macosx(10.7),ios(5.0));

/* Indicates if the value of the attribute should be captured on delete when Persistent History is enabled */
@property () BOOL preservesValueInHistoryOnDeletion API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0));

/*
 * This property can be set to enable encryption-at-rest on data stored in CloudKit servers.
 *
 * There are several restrictions on how clients can use this property:
 *  1. Attributes to be encrypted must be new additions to the CloudKit schema. Attributes that already exist in the production schema cannot be changed to support encryption.
 *  2. Attributes cannot (ever) change their encryption state in the CloudKit schema. Once something is encrypted (or not) it will forever be so.
 *
 * Note: This property does not affect the data in the persistent store. Local file encryption should continue to be managed by using NSFileProtection and other standard platform security mechanisms.
 */
@property () BOOL allowsCloudEncryption API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));

@end

NS_ASSUME_NONNULL_END
