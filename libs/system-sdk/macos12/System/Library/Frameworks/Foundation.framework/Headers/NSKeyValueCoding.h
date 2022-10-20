/*
	NSKeyValueCoding.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSOrderedSet.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSException.h>

@class NSError, NSString;

NS_ASSUME_NONNULL_BEGIN

/* The exception that is thrown when a key value coding operation fails. The exception's user info dictionary will contain at least two entries:
    - @"NSTargetObjectUserInfoKey": the receiver of the failed KVC message.
    - @"NSUnknownUserInfoKey": the key that was used in the failed KVC message.

The actual value of this constant string is "NSUnknownKeyException," to match the exceptions that are thrown by KVC methods that were deprecated in Mac OS 10.3.
*/
FOUNDATION_EXPORT NSExceptionName const NSUndefinedKeyException;

typedef NSString * NSKeyValueOperator NS_TYPED_ENUM;

/* Strings for the names of array operators supported by key-value coding. Only these string declarations are new in Mac OS 10.4. The actual support for array operators appeared in Mac OS 10.3. The values of these do not include "@" prefixes.
*/
FOUNDATION_EXPORT NSKeyValueOperator const NSAverageKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSCountKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSDistinctUnionOfArraysKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSDistinctUnionOfObjectsKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSDistinctUnionOfSetsKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSMaximumKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSMinimumKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSSumKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSUnionOfArraysKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSUnionOfObjectsKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSUnionOfSetsKeyValueOperator;

@interface NSObject(NSKeyValueCoding)

/* Return YES if -valueForKey:, -setValue:forKey:, -mutableArrayValueForKey:, -storedValueForKey:, -takeStoredValue:forKey:, and -takeValue:forKey: may directly manipulate instance variables when sent to instances of the receiving class, NO otherwise. The default implementation of this property returns YES.
*/
@property (class, readonly) BOOL accessInstanceVariablesDirectly;

/* Given a key that identifies an attribute or to-one relationship, return the attribute value or the related object. Given a key that identifies a to-many relationship, return an immutable array or an immutable set that contains all of the related objects.
    
The default implementation of this method does the following:
    1. Searches the class of the receiver for an accessor method whose name matches the pattern -get<Key>, -<key>, or -is<Key>, in that order. If such a method is found it is invoked. If the type of the method's result is an object pointer type the result is simply returned. If the type of the result is one of the scalar types supported by NSNumber conversion is done and an NSNumber is returned. Otherwise, conversion is done and an NSValue is returned (new in Mac OS 10.5: results of arbitrary type are converted to NSValues, not just NSPoint, NRange, NSRect, and NSSize).
    2 (introduced in Mac OS 10.7). Otherwise (no simple accessor method is found), searches the class of the receiver for methods whose names match the patterns -countOf<Key> and -indexIn<Key>OfObject: and -objectIn<Key>AtIndex: (corresponding to the primitive methods defined by the NSOrderedSet class) and also -<key>AtIndexes: (corresponding to -[NSOrderedSet objectsAtIndexes:]). If a count method and an indexOf method and at least one of the other two possible methods are found, a collection proxy object that responds to all NSOrderedSet methods is returned. Each NSOrderedSet message sent to the collection proxy object will result in some combination of -countOf<Key>, -indexIn<Key>OfObject:, -objectIn<Key>AtIndex:, and -<key>AtIndexes: messages being sent to the original receiver of -valueForKey:. If the class of the receiver also implements an optional method whose name matches the pattern -get<Key>:range: that method will be used when appropriate for best performance.
    3. Otherwise (no simple accessor method or set of ordered set access methods is found), searches the class of the receiver for methods whose names match the patterns -countOf<Key> and -objectIn<Key>AtIndex: (corresponding to the primitive methods defined by the NSArray class) and (introduced in Mac OS 10.4) also -<key>AtIndexes: (corresponding to -[NSArray objectsAtIndexes:]). If a count method and at least one of the other two possible methods are found, a collection proxy object that responds to all NSArray methods is returned. Each NSArray message sent to the collection proxy object will result in some combination of -countOf<Key>, -objectIn<Key>AtIndex:, and -<key>AtIndexes: messages being sent to the original receiver of -valueForKey:. If the class of the receiver also implements an optional method whose name matches the pattern -get<Key>:range: that method will be used when appropriate for best performance.
    4 (introduced in Mac OS 10.4). Otherwise (no simple accessor method or set of ordered set or array access methods is found), searches the class of the receiver for a threesome of methods whose names match the patterns -countOf<Key>, -enumeratorOf<Key>, and -memberOf<Key>: (corresponding to the primitive methods defined by the NSSet class). If all three such methods are found a collection proxy object that responds to all NSSet methods is returned. Each NSSet message sent to the collection proxy object will result in some combination of -countOf<Key>, -enumeratorOf<Key>, and -memberOf<Key>: messages being sent to the original receiver of -valueForKey:.
    5. Otherwise (no simple accessor method or set of collection access methods is found), if the receiver's class' +accessInstanceVariablesDirectly property returns YES, searches the class of the receiver for an instance variable whose name matches the pattern _<key>, _is<Key>, <key>, or is<Key>, in that order. If such an instance variable is found, the value of the instance variable in the receiver is returned, with the same sort of conversion to NSNumber or NSValue as in step 1.
    6. Otherwise (no simple accessor method, set of collection access methods, or instance variable is found), invokes -valueForUndefinedKey: and returns the result. The default implementation of -valueForUndefinedKey: raises an NSUndefinedKeyException, but you can override it in your application.

Compatibility notes:
    - For backward binary compatibility, an accessor method whose name matches the pattern -_get<Key>, or -_<key> is searched for between steps 1 and 3. If such a method is found it is invoked, with the same sort of conversion to NSNumber or NSValue as in step 1. KVC accessor methods whose names start with underscores were deprecated as of Mac OS 10.3 though.
    - The behavior described in step 5 is a change from Mac OS 10.2, in which the instance variable search order was <key>, _<key>.
    - For backward binary compatibility, -handleQueryWithUnboundKey: will be invoked instead of -valueForUndefinedKey: in step 6, if the implementation of -handleQueryWithUnboundKey: in the receiver's class is not NSObject's.
*/
- (nullable id)valueForKey:(NSString *)key;

/* Given a value and a key that identifies an attribute, set the value of the attribute. Given an object and a key that identifies a to-one relationship, relate the object to the receiver, unrelating the previously related object if there was one. Given a collection object and a key that identifies a to-many relationship, relate the objects contained in the collection to the receiver, unrelating previously related objects if there were any.

The default implementation of this method does the following:
    1. Searches the class of the receiver for an accessor method whose name matches the pattern -set<Key>:. If such a method is found the type of its parameter is checked. If the parameter type is not an object pointer type but the value is nil -setNilValueForKey: is invoked. The default implementation of -setNilValueForKey: raises an NSInvalidArgumentException, but you can override it in your application. Otherwise, if the type of the method's parameter is an object pointer type the method is simply invoked with the value as the argument. If the type of the method's parameter is some other type the inverse of the NSNumber/NSValue conversion done by -valueForKey: is performed before the method is invoked.
    2. Otherwise (no accessor method is found), if the receiver's class' +accessInstanceVariablesDirectly property returns YES, searches the class of the receiver for an instance variable whose name matches the pattern _<key>, _is<Key>, <key>, or is<Key>, in that order. If such an instance variable is found and its type is an object pointer type the value is retained and the result is set in the instance variable, after the instance variable's old value is first released. If the instance variable's type is some other type its value is set after the same sort of conversion from NSNumber or NSValue as in step 1.
    3. Otherwise (no accessor method or instance variable is found), invokes -setValue:forUndefinedKey:. The default implementation of -setValue:forUndefinedKey: raises an NSUndefinedKeyException, but you can override it in your application.

Compatibility notes:
    - For backward binary compatibility with -takeValue:forKey:'s behavior, a method whose name matches the pattern -_set<Key>: is also recognized in step 1. KVC accessor methods whose names start with underscores were deprecated as of Mac OS 10.3 though.
    - For backward binary compatibility, -unableToSetNilForKey: will be invoked instead of -setNilValueForKey: in step 1, if the implementation of -unableToSetNilForKey: in the receiver's class is not NSObject's.
    - The behavior described in step 2 is different from -takeValue:forKey:'s, in which the instance variable search order is <key>, _<key>.
    - For backward binary compatibility with -takeValue:forKey:'s behavior, -handleTakeValue:forUnboundKey: will be invoked instead of -setValue:forUndefinedKey: in step 3, if the implementation of -handleTakeValue:forUnboundKey: in the receiver's class is not NSObject's.
*/
- (void)setValue:(nullable id)value forKey:(NSString *)key;

/* Given a pointer to a value pointer, a key that identifies an attribute or to-one relationship, and a pointer to an NSError pointer, return a value that is suitable for use in subsequent -setValue:forKey: messages sent to the same receiver. If no validation is necessary, return YES without altering *ioValue or *outError. If validation is necessary and possible, return YES after setting *ioValue to an object that is the validated version of the original value, but without altering *outError. If validation is necessary but not possible, return NO after setting *outError to an NSError that encapsulates the reason that validation was not possible, but without altering *ioValue. The sender of the message is never given responsibility for releasing ioValue or outError.

The default implementation of this method searches the class of the receiver for a validator method whose name matches the pattern -validate<Key>:error:. If such a method is found it is invoked and the result is returned. If no such method is found, YES is returned.
*/
- (BOOL)validateValue:(inout id _Nullable * _Nonnull)ioValue forKey:(NSString *)inKey error:(out NSError **)outError;

/* Given a key that identifies an _ordered_ to-many relationship, return a mutable array that provides read-write access to the related objects. Objects added to the mutable array will become related to the receiver, and objects removed from the mutable array will become unrelated.

The default implementation of this method recognizes the same simple accessor methods and array accessor methods as -valueForKey:'s, and follows the same direct instance variable access policies, but always returns a mutable collection proxy object instead of the immutable collection that -valueForKey: would return. It also:
    1. Searches the class of the receiver for methods whose names match the patterns -insertObject:in<Key>AtIndex: and -removeObjectFrom<Key>AtIndex: (corresponding to the two most primitive methods defined by the NSMutableArray class), and (introduced in Mac OS 10.4) also -insert<Key>:atIndexes: and -remove<Key>AtIndexes: (corresponding to -[NSMutableArray insertObjects:atIndexes:] and -[NSMutableArray removeObjectsAtIndexes:). If at least one insertion method and at least one removal method are found each NSMutableArray message sent to the collection proxy object will result in some combination of -insertObject:in<Key>AtIndex:, -removeObjectFrom<Key>AtIndex:, -insert<Key>:atIndexes:, and -remove<Key>AtIndexes: messages being sent to the original receiver of -mutableArrayValueForKey:. If the class of the receiver also implements an optional method whose name matches the pattern -replaceObjectIn<Key>AtIndex:withObject: or (introduced in Mac OS 10.4) -replace<Key>AtIndexes:with<Key>: that method will be used when appropriate for best performance.
    2. Otherwise (no set of array mutation methods is found), searches the class of the receiver for an accessor method whose name matches the pattern -set<Key>:. If such a method is found each NSMutableArray message sent to the collection proxy object will result in a -set<Key>: message being sent to the original receiver of -mutableArrayValueForKey:.
    3. Otherwise (no set of array mutation methods or simple accessor method is found), if the receiver's class' +accessInstanceVariablesDirectly property returns YES, searches the class of the receiver for an instance variable whose name matches the pattern _<key> or <key>, in that order. If such an instance variable is found, each NSMutableArray message sent to the collection proxy object will be forwarded to the instance variable's value, which therefore must typically be an instance of NSMutableArray or a subclass of NSMutableArray.
    4. Otherwise (no set of array mutation methods, simple accessor method, or instance variable is found), returns a mutable collection proxy object anyway. Each NSMutableArray message sent to the collection proxy object will result in a -setValue:forUndefinedKey: message being sent to the original receiver of -mutableArrayValueForKey:. The default implementation of -setValue:forUndefinedKey: raises an NSUndefinedKeyException, but you can override it in your application.

Performance note: the repetitive -set<Key>: messages implied by step 2's description are a potential performance problem. For better performance implement insertion and removal methods that fulfill the requirements for step 1 in your KVC-compliant class. For best performance implement a replacement method too.
*/
- (NSMutableArray *)mutableArrayValueForKey:(NSString *)key;

/* Given a key that identifies an _ordered_ and uniquing to-many relationship, return a mutable ordered set that provides read-write access to the related objects. Objects added to the mutable ordered set will become related to the receiver, and objects removed from the mutable ordered set will become unrelated.

The default implementation of this method recognizes the same simple accessor methods and ordered set accessor methods as -valueForKey:'s, and follows the same direct instance variable access policies, but always returns a mutable collection proxy object instead of the immutable collection that -valueForKey: would return. It also:
    1. Searches the class of the receiver for methods whose names match the patterns -insertObject:in<Key>AtIndex: and -removeObjectFrom<Key>AtIndex: (corresponding to the two most primitive methods defined by the NSMutableOrderedSet class), and also -insert<Key>:atIndexes: and -remove<Key>AtIndexes: (corresponding to -[NSMutableOrderedSet insertObjects:atIndexes:] and -[NSMutableOrderedSet removeObjectsAtIndexes:). If at least one insertion method and at least one removal method are found each NSMutableOrderedSet message sent to the collection proxy object will result in some combination of -insertObject:in<Key>AtIndex:, -removeObjectFrom<Key>AtIndex:, -insert<Key>:atIndexes:, and -remove<Key>AtIndexes: messages being sent to the original receiver of -mutableOrderedSetValueForKey:. If the class of the receiver also implements an optional method whose name matches the pattern -replaceObjectIn<Key>AtIndex:withObject: or -replace<Key>AtIndexes:with<Key>: that method will be used when appropriate for best performance.
    2. Otherwise (no set of ordered set mutation methods is found), searches the class of the receiver for an accessor method whose name matches the pattern -set<Key>:. If such a method is found each NSMutableOrderedSet message sent to the collection proxy object will result in a -set<Key>: message being sent to the original receiver of -mutableOrderedSetValueForKey:.
    3. Otherwise (no set of ordered set mutation methods or simple accessor method is found), if the receiver's class' +accessInstanceVariablesDirectly property returns YES, searches the class of the receiver for an instance variable whose name matches the pattern _<key> or <key>, in that order. If such an instance variable is found, each NSMutableOrderedSet message sent to the collection proxy object will be forwarded to the instance variable's value, which therefore must typically be an instance of NSMutableOrderedSet or a subclass of NSMutableOrderedSet.
    4. Otherwise (no set of ordered set mutation methods, simple accessor method, or instance variable is found), returns a mutable collection proxy object anyway. Each NSMutableOrderedSet message sent to the collection proxy object will result in a -setValue:forUndefinedKey: message being sent to the original receiver of -mutableOrderedSetValueForKey:. The default implementation of -setValue:forUndefinedKey: raises an NSUndefinedKeyException, but you can override it in your application.

Performance note: the repetitive -set<Key>: messages implied by step 2's description are a potential performance problem. For better performance implement insertion and removal methods that fulfill the requirements for step 1 in your KVC-compliant class. For best performance implement a replacement method too.
*/
- (NSMutableOrderedSet *)mutableOrderedSetValueForKey:(NSString *)key API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Given a key that identifies an _unordered_ and uniquing to-many relationship, return a mutable set that provides read-write access to the related objects. Objects added to the mutable set will become related to the receiver, and objects removed from the mutable set will become unrelated.

The default implementation of this method recognizes the same simple accessor methods and set accessor methods as -valueForKey:'s, and follows the same direct instance variable access policies, but always returns a mutable collection proxy object instead of the immutable collection that -valueForKey: would return. It also:
    1. Searches the class of the receiver for methods whose names match the patterns -add<Key>Object: and -remove<Key>Object: (corresponding to the two primitive methods defined by the NSMutableSet class) and also -add<Key>: and -remove<Key>: (corresponding to -[NSMutableSet unionSet:] and -[NSMutableSet minusSet:]). If at least one addition method and at least one removal method are found each NSMutableSet message sent to the collection proxy object will result in some combination of -add<Key>Object:, -remove<Key>Object:, -add<Key>:, and -remove<Key>: messages being sent to the original receiver of -mutableSetValueForKey:. If the class of the receiver also implements an optional method whose name matches the pattern -intersect<Key>: or -set<Key>: that method will be used when appropriate for best performance.
    2. Otherwise (no set of set mutation methods is found), searches the class of the receiver for an accessor method whose name matches the pattern -set<Key>:. If such a method is found each NSMutableSet message sent to the collection proxy object will result in a -set<Key>: message being sent to the original receiver of -mutableSetValueForKey:.
    3. Otherwise (no set of set mutation methods or simple accessor method is found), if the receiver's class' +accessInstanceVariablesDirectly property returns YES, searches the class of the receiver for an instance variable whose name matches the pattern _<key> or <key>, in that order. If such an instance variable is found, each NSMutableSet message sent to the collection proxy object will be forwarded to the instance variable's value, which therefore must typically be an instance of NSMutableSet or a subclass of NSMutableSet.
    4. Otherwise (no set of set mutation methods, simple accessor method, or instance variable is found), returns a mutable collection proxy object anyway. Each NSMutableSet message sent to the collection proxy object will result in a -setValue:forUndefinedKey: message being sent to the original receiver of -mutableSetValueForKey:. The default implementation of -setValue:forUndefinedKey: raises an NSUndefinedKeyException, but you can override it in your application.

Performance note: the repetitive -set<Key>: messages implied by step 2's description are a potential performance problem. For better performance implement methods that fulfill the requirements for step 1 in your KVC-compliant class.
*/
- (NSMutableSet *)mutableSetValueForKey:(NSString *)key;

/* Key-path-taking variants of like-named methods. The default implementation of each parses the key path enough to determine whether or not it has more than one component (key path components are separated by periods). If so, -valueForKey: is invoked with the first key path component as the argument, and the method being invoked is invoked recursively on the result, with the remainder of the key path passed as an argument. If not, the like-named non-key-path-taking method is invoked.
*/
- (nullable id)valueForKeyPath:(NSString *)keyPath;
- (void)setValue:(nullable id)value forKeyPath:(NSString *)keyPath;
- (BOOL)validateValue:(inout id _Nullable * _Nonnull)ioValue forKeyPath:(NSString *)inKeyPath error:(out NSError **)outError;
- (NSMutableArray *)mutableArrayValueForKeyPath:(NSString *)keyPath;
- (NSMutableOrderedSet *)mutableOrderedSetValueForKeyPath:(NSString *)keyPath API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (NSMutableSet *)mutableSetValueForKeyPath:(NSString *)keyPath;

/* Given that an invocation of -valueForKey: would be unable to get a keyed value using its default access mechanism, return the keyed value using some other mechanism. The default implementation of this method raises an NSUndefinedKeyException. You can override it to handle properties that are dynamically defined at run-time.
*/
- (nullable id)valueForUndefinedKey:(NSString *)key;

/* Given that an invocation of -setValue:forKey: would be unable to set the keyed value using its default mechanism, set the keyed value using some other mechanism. The default implementation of this method raises an NSUndefinedKeyException. You can override it to handle properties that are dynamically defined at run-time.
*/
- (void)setValue:(nullable id)value forUndefinedKey:(NSString *)key;

/* Given that an invocation of -setValue:forKey: would be unable to set the keyed value because the type of the parameter of the corresponding accessor method is an NSNumber scalar type or NSValue structure type but the value is nil, set the keyed value using some other mechanism. The default implementation of this method raises an NSInvalidArgumentException. You can override it to map nil values to something meaningful in the context of your application.
*/
- (void)setNilValueForKey:(NSString *)key;

/* Given an array of keys, return a dictionary containing the keyed attribute values, to-one-related objects, and/or collections of to-many-related objects. Entries for which -valueForKey: returns nil have NSNull as their value in the returned dictionary.
*/
- (NSDictionary<NSString *, id> *)dictionaryWithValuesForKeys:(NSArray<NSString *> *)keys;

/* Given a dictionary containing keyed attribute values, to-one-related objects, and/or collections of to-many-related objects, set the keyed values. Dictionary entries whose values are NSNull result in -setValue:nil forKey:key messages being sent to the receiver.
*/
- (void)setValuesForKeysWithDictionary:(NSDictionary<NSString *, id> *)keyedValues;

@end

@interface NSArray<ObjectType>(NSKeyValueCoding)

/* Return an array containing the results of invoking -valueForKey: on each of the receiver's elements. The returned array will contain NSNull elements for each instance of -valueForKey: returning nil.
*/
- (id)valueForKey:(NSString *)key;

/* Invoke -setValue:forKey: on each of the receiver's elements.
*/
- (void)setValue:(nullable id)value forKey:(NSString *)key;

@end

@interface NSDictionary<KeyType, ObjectType>(NSKeyValueCoding)

/* Return the result of sending -objectForKey: to the receiver.
*/
- (nullable ObjectType)valueForKey:(NSString *)key;

@end

@interface NSMutableDictionary<KeyType, ObjectType>(NSKeyValueCoding)

/* Send -setObject:forKey: to the receiver, unless the value is nil, in which case send -removeObjectForKey:.
*/
- (void)setValue:(nullable ObjectType)value forKey:(NSString *)key;

@end

@interface NSOrderedSet<ObjectType>(NSKeyValueCoding)

/* Return an ordered set containing the results of invoking -valueForKey: on each of the receiver's members. The returned ordered set might not have the same number of members as the receiver. The returned ordered set will not contain any elements corresponding to instances of -valueForKey: returning nil, nor will it contain duplicates.
*/
- (id)valueForKey:(NSString *)key API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/* Invoke -setValue:forKey: on each of the receiver's members.
*/
- (void)setValue:(nullable id)value forKey:(NSString *)key API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

@end

@interface NSSet<ObjectType>(NSKeyValueCoding)

/* Return a set containing the results of invoking -valueForKey: on each of the receiver's members. The returned set might not have the same number of members as the receiver. The returned set will not contain any elements corresponding to instances of -valueForKey: returning nil (in contrast with -[NSArray(NSKeyValueCoding) valueForKey:], which may put NSNulls in the arrays it returns).
*/
- (id)valueForKey:(NSString *)key;

/* Invoke -setValue:forKey: on each of the receiver's members.
*/
- (void)setValue:(nullable id)value forKey:(NSString *)key;

@end

#if TARGET_OS_OSX

@interface NSObject(NSDeprecatedKeyValueCoding)

/* Methods that were deprecated in Mac OS 10.4.
*/
+ (BOOL)useStoredAccessor API_DEPRECATED("Legacy KVC API", macos(10.0,10.4), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (nullable id)storedValueForKey:(NSString *)key API_DEPRECATED("Legacy KVC API", macos(10.0,10.4), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)takeStoredValue:(nullable id)value forKey:(NSString *)key API_DEPRECATED("Legacy KVC API", macos(10.0,10.4), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));

/* Methods that were deprecated in Mac OS 10.3. Use the new, more consistently named, methods declared above instead.
*/
- (void)takeValue:(nullable id)value forKey:(NSString *)key API_DEPRECATED("Legacy KVC API", macos(10.0,10.3), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)takeValue:(nullable id)value forKeyPath:(NSString *)keyPath API_DEPRECATED("Legacy KVC API", macos(10.0,10.3), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (nullable id)handleQueryWithUnboundKey:(NSString *)key API_DEPRECATED("Legacy KVC API", macos(10.0,10.3), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)handleTakeValue:(nullable id)value forUnboundKey:(NSString *)key API_DEPRECATED("Legacy KVC API", macos(10.0,10.3), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)unableToSetNilForKey:(NSString *)key API_DEPRECATED("Legacy KVC API", macos(10.0,10.3), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (NSDictionary *)valuesForKeys:(NSArray *)keys API_DEPRECATED("Legacy KVC API", macos(10.0,10.3), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)takeValuesFromDictionary:(NSDictionary *)properties API_DEPRECATED("Legacy KVC API", macos(10.0,10.3), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));

@end

#endif

NS_ASSUME_NONNULL_END
