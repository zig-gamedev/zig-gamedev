/*	NSValueTransformer.h
        Copyright (c) 2002-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSArray<ObjectType>, NSString;

NS_ASSUME_NONNULL_BEGIN

typedef NSString *NSValueTransformerName NS_TYPED_EXTENSIBLE_ENUM;

FOUNDATION_EXPORT NSValueTransformerName const NSNegateBooleanTransformerName	API_AVAILABLE(macos(10.3), ios(3.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSValueTransformerName const NSIsNilTransformerName		API_AVAILABLE(macos(10.3), ios(3.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSValueTransformerName const NSIsNotNilTransformerName	API_AVAILABLE(macos(10.3), ios(3.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT NSValueTransformerName const NSUnarchiveFromDataTransformerName       API_DEPRECATED_WITH_REPLACEMENT("NSSecureUnarchiveFromDataTransformerName", macos(10.3, 10.14), ios(3.0, 12.0), watchos(2.0, 5.0), tvos(9.0, 12.0));
FOUNDATION_EXPORT NSValueTransformerName const NSKeyedUnarchiveFromDataTransformerName  API_DEPRECATED_WITH_REPLACEMENT("NSSecureUnarchiveFromDataTransformerName", macos(10.3, 10.14), ios(3.0, 12.0), watchos(2.0, 5.0), tvos(9.0, 12.0));
FOUNDATION_EXPORT NSValueTransformerName const NSSecureUnarchiveFromDataTransformerName API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0));

API_AVAILABLE(macos(10.3), ios(3.0), watchos(2.0), tvos(9.0))
@interface NSValueTransformer : NSObject {
}

// name-based registry for shared objects (especially used when loading nib files with transformers specified by name in Interface Builder) - also useful for localization (developers can register different kind of transformers or differently configured transformers at application startup and refer to them by name from within nib files or other code)
// if valueTransformerForName: does not find a registered transformer instance, it will fall back to looking up a class with the specified name - if one is found, it will instantiate a transformer with the default -init method and automatically register it
+ (void)setValueTransformer:(nullable NSValueTransformer *)transformer forName:(NSValueTransformerName)name;
+ (nullable NSValueTransformer *)valueTransformerForName:(NSValueTransformerName)name;
+ (NSArray<NSValueTransformerName> *)valueTransformerNames;

// information that can be used to analyze available transformer instances (especially used inside Interface Builder)
+ (Class)transformedValueClass;    // class of the "output" objects, as returned by transformedValue:
+ (BOOL)allowsReverseTransformation;    // flag indicating whether transformation is read-only or not

- (nullable id)transformedValue:(nullable id)value;           // by default returns value
- (nullable id)reverseTransformedValue:(nullable id)value;    // by default raises an exception if +allowsReverseTransformation returns NO and otherwise invokes transformedValue:

@end

/// A value transformer which transforms values to and from \c NSData by archiving and unarchiving using secure coding.
API_AVAILABLE(macos(10.14), ios(12.0), watchos(5.0), tvos(12.0))
@interface NSSecureUnarchiveFromDataTransformer : NSValueTransformer

/// The list of allowable classes which the top-level object in the archive must conform to on encoding and decoding.
///
/// Returns the result of \c +transformedValueClass if not \c Nil; otherwise, currently returns \c NSArray, \c NSDictionary, \c NSSet, \c NSString, \c NSNumber, \c NSDate, \c NSData, \c NSURL, \c NSUUID, and \c NSNull.
///
/// Can be overridden by subclasses to provide an expanded or different set of allowed transformation classes.
@property (class, readonly, copy) NSArray<Class> *allowedTopLevelClasses;

@end

NS_ASSUME_NONNULL_END
