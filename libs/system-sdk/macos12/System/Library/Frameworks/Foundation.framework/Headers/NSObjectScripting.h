/*
	NSObjectScripting.h
	Copyright (c) 2002-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSDictionary<KeyType, ObjectType>, NSScriptObjectSpecifier, NSString;

NS_ASSUME_NONNULL_BEGIN

@interface NSObject(NSScripting)

/* Given an object specifier return the specified object or objects in the receiving container. This might successfully return an object, an array of objects, or nil, depending on the kind of object specifier. Because nil is a valid value, failure is signaled by sending the object specifier -setEvaluationError: before returning. Your override doesn't also have to invoke any of the NSScriptCommand error signaling methods, though it can, to record very specific error information. The NSUnknownKeySpecifierError and NSInvalidIndexSpecifierError numbers are special, in that Cocoa may continue evaluating an outer specifier if they're encountered, for the convenience of scripters.
*/
- (nullable id)scriptingValueForSpecifier:(NSScriptObjectSpecifier *)objectSpecifier API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);


/* Getter: */
/* Return an NSString-keyed dictionary of the object's scriptable properties, including all of those that are declared as properties or contents in the .sdef declaration (or Attributes and ToOneRelationships in the .scriptSuite declaration) of the class and its scripting superclasses, with the exception of ones keyed by "scriptingProperties." Each key in the dictionary must be identical to the key for an attribute or to-one relationship. The values of the dictionary must be Objective-C objects that are convertible to NSAppleEventDescriptors.
*/
/* Setter: */
/* Given an NSString-keyed dictionary, set one or more scriptable properties of the object. The valid keys for the dictionary include the keys for writable properties and contents in the .sdef declaration (or non-read-only Attributes and ToOneRelationships in the .scriptSuite declaration) of the object's class and its scripting superclasses, and no others. In other words, invokers of this method must have already done any necessary validation to ensure that the properties dictionary includes nothing but entries for declared, settable attributes and to-one relationships. Implementations of this method are not expected to check the validity of keys in the passed-in dictionary, but must be able accept dictionaries that do not contain entries for every scriptable property. The values of the dictionary are Objective-C objects.
 */
@property (nullable, copy) NSDictionary<NSString *, id> *scriptingProperties;

/* Create one or more scripting objects to be inserted into the relationship identified by the key by copying the passed-in value, set the properties in the copied object or objects, and return it or them. The value is, for example, derived from the receivers of a Duplicate command. Its type must match the type of the property identified by the key. For example, if the property is an ordered to-many relationship, the value will always be an array of objects to be copied, and an array of objects must therefore be returned. The properties are derived from the "with properties" parameter of a Duplicate command. When this method is invoked by Cocoa neither the value nor the properties will have yet been coerced using scripting key-value coding's -coerceValue:forKey: method. In .sdef-declared scriptability the types of the passed-in objects reliably match the relevant .sdef declarations however.
*/
- (nullable id)copyScriptingValue:(id)value forKey:(NSString *)key withProperties:(NSDictionary<NSString *, id> *)properties API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

/* Create a new instance of a scriptable class to be inserted into the relationship identified by the key, set the contentsValue and properties of it, and return it. Or return nil for failure. The contentsValue and properties are derived from the "with contents" and "with properties" parameters of a Make command. The contentsValue may be nil. When this method is invoked by Cocoa neither the contentsValue nor the properties will have yet been coerced using scripting key-value coding's -coerceValue:forKey: method. In .sdef-declared scriptability the types of the passed-in objects reliably match the relevant .sdef declarations however.
*/
- (nullable id)newScriptingObjectOfClass:(Class)objectClass forValueForKey:(NSString *)key withContentsValue:(nullable id)contentsValue properties:(NSDictionary<NSString *, id> *)properties API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);


@end

NS_ASSUME_NONNULL_END
