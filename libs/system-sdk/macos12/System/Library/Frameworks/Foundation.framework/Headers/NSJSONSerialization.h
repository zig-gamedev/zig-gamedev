/*	
    NSJSONSerialization.h
    Copyright (c) 2009-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSError, NSOutputStream, NSInputStream, NSData;

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSJSONReadingOptions) {
    NSJSONReadingMutableContainers = (1UL << 0),
    NSJSONReadingMutableLeaves = (1UL << 1),
    NSJSONReadingFragmentsAllowed = (1UL << 2),

    NSJSONReadingJSON5Allowed API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) = (1UL << 3),
    NSJSONReadingTopLevelDictionaryAssumed API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0)) = (1UL << 4),

    NSJSONReadingAllowFragments API_DEPRECATED_WITH_REPLACEMENT("NSJSONReadingFragmentsAllowed", macos(10.7, API_TO_BE_DEPRECATED), ios(5.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) = NSJSONReadingFragmentsAllowed,
} API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

typedef NS_OPTIONS(NSUInteger, NSJSONWritingOptions) {
    NSJSONWritingPrettyPrinted = (1UL << 0),

    /* Sorts dictionary keys for output using [NSLocale systemLocale]. Keys are compared using NSNumericSearch. The specific sorting method used is subject to change.
     */
    NSJSONWritingSortedKeys API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0)) = (1UL << 1),
    NSJSONWritingFragmentsAllowed = (1UL << 2),
    NSJSONWritingWithoutEscapingSlashes API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0)) = (1UL << 3),
} API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* A class for converting JSON to Foundation objects and converting Foundation objects to JSON.
   
   An object that may be converted to JSON must have the following properties:
    - Top level object is an NSArray or NSDictionary
    - All objects are NSString, NSNumber, NSArray, NSDictionary, or NSNull
    - All dictionary keys are NSStrings
    - NSNumbers are not NaN or infinity
*/
API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0))
@interface NSJSONSerialization : NSObject {
@private
    void *reserved[6];
}

/* Returns YES if the given object can be converted to JSON data, NO otherwise. The object must have the following properties:
    - Top level object is an NSArray or NSDictionary
    - All objects are NSString, NSNumber, NSArray, NSDictionary, or NSNull
    - All dictionary keys are NSStrings
    - NSNumbers are not NaN or infinity
 Other rules may apply. Calling this method or attempting a conversion are the definitive ways to tell if a given object can be converted to JSON data.
 */
+ (BOOL)isValidJSONObject:(id)obj;

/* Generate JSON data from a Foundation object. If the object will not produce valid JSON then an exception will be thrown. Setting the NSJSONWritingPrettyPrinted option will generate JSON with whitespace designed to make the output more readable. If that option is not set, the most compact possible JSON will be generated. If an error occurs, the error parameter will be set and the return value will be nil. The resulting data is a encoded in UTF-8.
 */
+ (nullable NSData *)dataWithJSONObject:(id)obj options:(NSJSONWritingOptions)opt error:(NSError **)error;

/* Create a Foundation object from JSON data. Set the NSJSONReadingAllowFragments option if the parser should allow top-level objects that are not an NSArray or NSDictionary. Setting the NSJSONReadingMutableContainers option will make the parser generate mutable NSArrays and NSDictionaries. Setting the NSJSONReadingMutableLeaves option will make the parser generate mutable NSString objects. If an error occurs during the parse, then the error parameter will be set and the result will be nil.
   The data must be in one of the 5 supported encodings listed in the JSON specification: UTF-8, UTF-16LE, UTF-16BE, UTF-32LE, UTF-32BE. The data may or may not have a BOM. The most efficient encoding to use for parsing is UTF-8, so if you have a choice in encoding the data passed to this method, use UTF-8.
 */
+ (nullable id)JSONObjectWithData:(NSData *)data options:(NSJSONReadingOptions)opt error:(NSError **)error;

/* Write JSON data into a stream. The stream should be opened and configured. The return value is the number of bytes written to the stream, or 0 on error. All other behavior of this method is the same as the dataWithJSONObject:options:error: method.
 */
+ (NSInteger)writeJSONObject:(id)obj toStream:(NSOutputStream *)stream options:(NSJSONWritingOptions)opt error:(NSError **)error;

/* Create a JSON object from JSON data stream. The stream should be opened and configured. All other behavior of this method is the same as the JSONObjectWithData:options:error: method.
 */
+ (nullable id)JSONObjectWithStream:(NSInputStream *)stream options:(NSJSONReadingOptions)opt error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
