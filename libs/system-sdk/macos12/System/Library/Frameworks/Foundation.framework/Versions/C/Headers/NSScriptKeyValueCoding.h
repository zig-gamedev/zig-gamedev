/*
	NSScriptKeyValueCoding.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString;

NS_ASSUME_NONNULL_BEGIN

extern NSString *NSOperationNotSupportedForKeyException;

@interface NSObject(NSScriptKeyValueCoding)

/* Return the indexed object in the value of the to-many relationship identified by the key. The default implementation of this method searches the class of the receiver for a method whose name matches the pattern -valueIn<Key>AtIndex: and invokes it if one is found. If no such method is found an exception is thrown. (In general. Other things may also be done for backward binary compatibility.) If your application requires Mac OS 10.4 or newer you can ignore this method. Fulfilling the more modern requirements of KVC-compliance established by -[NSObject(NSKeyValueCoding) valueForKey:] is enough to make Cocoa's scripting support work for the keyed relationship.
*/
- (nullable id)valueAtIndex:(NSUInteger)index inPropertyWithKey:(NSString *)key;

/* Return the named object in the value of the to-many relationship identified by the key. The default implementation of this method searches the class of the receiver for a method whose name matches the pattern -valueIn<Key>WithName: and invokes it if one is found. The declared type of the method's parameter must be NSString *. If no such method is found an exception is thrown. Cocoa's scripting support uses this method during the evaluation of NSNameSpecifiers if it's overridden or a -valueIn<Key>WithName: method is implemented for the key in question. You can take advantage of this to optimize the evaluation of name specifiers. (Doing this is less frequently useful than doing the equivalent thing for unique IDs.) If you don't then Cocoa does a linear search of all of the related objects.
*/
- (nullable id)valueWithName:(NSString *)name inPropertyWithKey:(NSString *)key;

/* Return the uniquely dentified object in the value of the to-many relationship identified by the key. uniqueID must be either an NSNumber or an NSString. The default implementation of this method searches the class of the receiver for a method whose name matches the pattern -valueIn<Key>WithUniqueID: and invokes it if one is found. The declared type of the unique ID parameter must be id, NSNumber *, NSString *, or one of the scalar types that is supported by NSNumber. If no such method is found an exception is thrown. Cocoa's scripting support uses this method during the evaluation of NSUniqueIDSpecifiers if it's overridden or a -valueIn<Key>WithUniqueID: method is implemented for the key in question. You can take advantage of this to optimize the evaluation of unique ID specifiers. If you don't then Cocoa just does a linear search of all of the related objects.
*/
- (nullable id)valueWithUniqueID:(id)uniqueID inPropertyWithKey:(NSString *)key;

/* Insert, remove, or replace the indexed object in the value of the to-many relationship identified by the key. The default implementations of these methods search the class of the receiver for a method whose name matches the pattern -insertIn<Key>:atIndex:, -removeFrom<Key>AtIndex:, or -replaceIn<Key>:atIndex:, respectively, and invokes it if one is found. If no such method is found an exception is thrown. (In general. Other things may also be done for backward binary compatibility.) If your application requires Mac OS 10.4 or newer you can ignore these methods. Fulfilling the more modern requirements of KVC-compliance established by -[NSObject(NSKeyValueCoding) mutableArrayValueForKey:] is enough to make Cocoa's scripting support work for the keyed relationship.
*/
- (void)insertValue:(id)value atIndex:(NSUInteger)index inPropertyWithKey:(NSString *)key;
- (void)removeValueAtIndex:(NSUInteger)index fromPropertyWithKey:(NSString *)key;
- (void)replaceValueAtIndex:(NSUInteger)index inPropertyWithKey:(NSString *)key withValue:(id)value;

/* Insert the object into the value of the to-many relationship identified by the key, at a location that makes sense for a scripted Make command with no "at" parameter (either the beginning or the end, typically). The default implementation of this method searches the class of the receiver for a method whose name matches the pattern -insertIn<Key>: and invokes it if one is found. In an application with .scriptSuite/.scriptTerminology-declared scriptability, NSCreateCommand may invoke this method if the to-many relationship's .scriptSuite declaration has a LocationRequiredToCreate = NO entry, and its default implementation throws an exception if no -insertIn<Key>: method is found. In an application with .sdef-declared scriptability, NSCreateCommand may invoke this method for any to-many relationship, and (starting in Mac OS 10.5) its default implementation never throws an exception. It inserts the new object at either the beginning or the end of the relationship, depending on the value of the "insert-at-beginning" attribute of the <cocoa> subelement of the declaring <element> element.
*/
- (void)insertValue:(id)value inPropertyWithKey:(NSString *)key;

/* If the keyed property is an attribute or to-one relationship, coerce the object into an object suitable for setting as the value of the property and return that object. If the keyed property is a to-many relationship, coerce the object into an object suitable for inserting in the value of the property and return that object. The default implementation of this method searches the class of the receiver for a method whose name matches the pattern -coerceValueFor<Key>: and invokes it if one is found. If no such method is found and the application's scriptability is not .sdef-declared then it sends a -coerceValue:toClass: message to [NSScriptCoercionHandler sharedCoercionHandler].
*/
- (nullable id)coerceValue:(nullable id)value forKey:(NSString *)key;
    
@end

NS_ASSUME_NONNULL_END
