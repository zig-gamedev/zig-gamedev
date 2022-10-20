/*
	NSKeyValueObserving.h
	Copyright (c) 2003-2019, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSOrderedSet.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSDictionary.h>

@class NSIndexSet, NSString;

NS_ASSUME_NONNULL_BEGIN

/* Options for use with -addObserver:forKeyPath:options:context: and -addObserver:toObjectsAtIndexes:forKeyPath:options:context:.
*/
typedef NS_OPTIONS(NSUInteger, NSKeyValueObservingOptions) {

    /* Whether the change dictionaries sent in notifications should contain NSKeyValueChangeNewKey and NSKeyValueChangeOldKey entries, respectively.
    */
    NSKeyValueObservingOptionNew = 0x01,
    NSKeyValueObservingOptionOld = 0x02,

    /* Whether a notification should be sent to the observer immediately, before the observer registration method even returns. The change dictionary in the notification will always contain an NSKeyValueChangeNewKey entry if NSKeyValueObservingOptionNew is also specified but will never contain an NSKeyValueChangeOldKey entry. (In an initial notification the current value of the observed property may be old, but it's new to the observer.) You can use this option instead of explicitly invoking, at the same time, code that is also invoked by the observer's -observeValueForKeyPath:ofObject:change:context: method. When this option is used with -addObserver:toObjectsAtIndexes:forKeyPath:options:context: a notification will be sent for each indexed object to which the observer is being added.
    */
    NSKeyValueObservingOptionInitial API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 0x04,

    /* Whether separate notifications should be sent to the observer before and after each change, instead of a single notification after the change. The change dictionary in a notification sent before a change always contains an NSKeyValueChangeNotificationIsPriorKey entry whose value is [NSNumber numberWithBool:YES], but never contains an NSKeyValueChangeNewKey entry. You can use this option when the observer's own KVO-compliance requires it to invoke one of the -willChange... methods for one of its own properties, and the value of that property depends on the value of the observed object's property. (In that situation it's too late to easily invoke -willChange... properly in response to receiving an -observeValueForKeyPath:ofObject:change:context: message after the change.)

When this option is specified, the change dictionary in a notification sent after a change contains the same entries that it would contain if this option were not specified, except for ordered unique to-many relationships represented by NSOrderedSets.  For those, for NSKeyValueChangeInsertion and NSKeyValueChangeReplacement changes, the change dictionary for a will-change notification contains an NSKeyValueChangeIndexesKey (and NSKeyValueChangeOldKey in the case of Replacement where the NSKeyValueObservingOptionOld option was specified at registration time) which give the indexes (and objects) which *may* be changed by the operation.  The second notification, after the change, contains entries reporting what did actually change.  For NSKeyValueChangeRemoval changes, removals by index are precise.
    */
    NSKeyValueObservingOptionPrior API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) = 0x08

};

/* Possible values in the NSKeyValueChangeKindKey entry in change dictionaries. See the comments for -observeValueForKeyPath:ofObject:change:context: for more information.
*/
typedef NS_ENUM(NSUInteger, NSKeyValueChange) {
    NSKeyValueChangeSetting = 1,
    NSKeyValueChangeInsertion = 2,
    NSKeyValueChangeRemoval = 3,
    NSKeyValueChangeReplacement = 4,
};

/* Possible kinds of set mutation for use with -willChangeValueForKey:withSetMutation:usingObjects: and -didChangeValueForKey:withSetMutation:usingObjects:. Their semantics correspond exactly to NSMutableSet's -unionSet:, -minusSet:, -intersectSet:, and -setSet: method, respectively.
*/
typedef NS_ENUM(NSUInteger, NSKeyValueSetMutationKind) {
    NSKeyValueUnionSetMutation = 1,
    NSKeyValueMinusSetMutation = 2,
    NSKeyValueIntersectSetMutation = 3,
    NSKeyValueSetSetMutation = 4
};

typedef NSString * NSKeyValueChangeKey NS_TYPED_ENUM;
/* Keys for entries in change dictionaries. See the comments for -observeValueForKeyPath:ofObject:change:context: for more information.
*/
FOUNDATION_EXPORT NSKeyValueChangeKey const NSKeyValueChangeKindKey;
FOUNDATION_EXPORT NSKeyValueChangeKey const NSKeyValueChangeNewKey;
FOUNDATION_EXPORT NSKeyValueChangeKey const NSKeyValueChangeOldKey;
FOUNDATION_EXPORT NSKeyValueChangeKey const NSKeyValueChangeIndexesKey;
FOUNDATION_EXPORT NSKeyValueChangeKey const NSKeyValueChangeNotificationIsPriorKey API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@interface NSObject(NSKeyValueObserving)

/* Given that the receiver has been registered as an observer of the value at a key path relative to an object, be notified of a change to that value.

The change dictionary always contains an NSKeyValueChangeKindKey entry whose value is an NSNumber wrapping an NSKeyValueChange (use -[NSNumber unsignedIntegerValue]). The meaning of NSKeyValueChange depends on what sort of property is identified by the key path:
    - For any sort of property (attribute, to-one relationship, or ordered or unordered to-many relationship) NSKeyValueChangeSetting indicates that the observed object has received a -setValue:forKey: message, or that the key-value coding-compliant set method for the key has been invoked, or that a -willChangeValueForKey:/-didChangeValueForKey: pair has otherwise been invoked.
    - For an _ordered_ to-many relationship, NSKeyValueChangeInsertion, NSKeyValueChangeRemoval, and NSKeyValueChangeReplacement indicate that a mutating message has been sent to the array returned by a -mutableArrayValueForKey: message sent to the object, or sent to the ordered set returned by a -mutableOrderedSetValueForKey: message sent to the object, or that one of the key-value coding-compliant array or ordered set mutation methods for the key has been invoked, or that a -willChange:valuesAtIndexes:forKey:/-didChange:valuesAtIndexes:forKey: pair has otherwise been invoked.
    - For an _unordered_ to-many relationship (introduced in Mac OS 10.4), NSKeyValueChangeInsertion, NSKeyValueChangeRemoval, and NSKeyValueChangeReplacement indicate that a mutating message has been sent to the set returned by a -mutableSetValueForKey: message sent to the object, or that one of the key-value coding-compliant set mutation methods for the key has been invoked, or that a -willChangeValueForKey:withSetMutation:usingObjects:/-didChangeValueForKey:withSetMutation:usingObjects: pair has otherwise been invoked.

For any sort of property, the change dictionary contains an NSKeyValueChangeNewKey entry if NSKeyValueObservingOptionNew was specified at observer registration time, it's the right kind of change, and this isn't a prior notification. The change dictionary contains an NSKeyValueChangeOldKey if NSKeyValueObservingOptionOld was specified and it's the right kind of change. See the comments for the NSKeyValueObserverNotification informal protocol methods for what the values of those entries can be.

For an _ordered_ to-many relationship, the change dictionary always contains an NSKeyValueChangeIndexesKey entry whose value is an NSIndexSet containing the indexes of the inserted, removed, or replaced objects, unless the change is an NSKeyValueChangeSetting.

If NSKeyValueObservingOptionPrior (introduced in Mac OS 10.5) was specified at observer registration time, and this notification is one being sent prior to a change as a result, the change dictionary contains an NSKeyValueChangeNotificationIsPriorKey entry whose value is an NSNumber wrapping YES (use -[NSNumber boolValue]).

context is always the same pointer that was passed in at observer registration time.
*/
- (void)observeValueForKeyPath:(nullable NSString *)keyPath ofObject:(nullable id)object change:(nullable NSDictionary<NSKeyValueChangeKey, id> *)change context:(nullable void *)context;

@end

@interface NSObject(NSKeyValueObserverRegistration)

/* Register or deregister as an observer of the value at a key path relative to the receiver. The options determine what is included in observer notifications and when they're sent, as described above, and the context is passed in observer notifications as described above. You should use -removeObserver:forKeyPath:context: instead of -removeObserver:forKeyPath: whenever possible because it allows you to more precisely specify your intent. When the same observer is registered for the same key path multiple times, but with different context pointers each time, -removeObserver:forKeyPath: has to guess at the context pointer when deciding what exactly to remove, and it can guess wrong.
*/
- (void)addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context;
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath context:(nullable void *)context API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath;

@end

@interface NSArray<ObjectType>(NSKeyValueObserverRegistration)

/* Register or deregister as an observer of the values at a key path relative to each indexed element of the array. The options determine what is included in observer notifications and when they're sent, as described above, and the context is passed in observer notifications as described above. These are not merely convenience methods; invoking them is potentially much faster than repeatedly invoking NSObject(NSKeyValueObserverRegistration) methods. You should use -removeObserver:fromObjectsAtIndexes:forKeyPath:context: instead of -removeObserver:fromObjectsAtIndexes:forKeyPath: whenever possible for the same reason described in the NSObject(NSKeyValueObserverRegistration) comment.
*/
- (void)addObserver:(NSObject *)observer toObjectsAtIndexes:(NSIndexSet *)indexes forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context;
- (void)removeObserver:(NSObject *)observer fromObjectsAtIndexes:(NSIndexSet *)indexes forKeyPath:(NSString *)keyPath context:(nullable void *)context API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (void)removeObserver:(NSObject *)observer fromObjectsAtIndexes:(NSIndexSet *)indexes forKeyPath:(NSString *)keyPath;

/* NSArrays are not observable, so these methods raise exceptions when invoked on NSArrays. Instead of observing an array, observe the ordered to-many relationship for which the array is the collection of related objects.
*/
- (void)addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context;
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath context:(nullable void *)context API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath;

@end

@interface NSOrderedSet<ObjectType>(NSKeyValueObserverRegistration)

/* NSOrderedSets are not observable, so these methods raise exceptions when invoked on NSOrderedSets. Instead of observing an ordered set, observe the ordered to-many relationship for which the ordered set is the collection of related objects.
*/
- (void)addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context;
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath context:(nullable void *)context API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath;

@end

@interface NSSet<ObjectType>(NSKeyValueObserverRegistration)

/* NSSets are not observable, so these methods raise exceptions when invoked on NSSets. Instead of observing a set, observe the unordered to-many relationship for which the set is the collection of related objects.
*/
- (void)addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(nullable void *)context;
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath context:(nullable void *)context API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath;

@end

@interface NSObject(NSKeyValueObserverNotification)

/* Given a key that identifies a property (attribute, to-one relationship, or ordered or unordered to-many relationship), send -observeValueForKeyPath:ofObject:change:context: notification messages of kind NSKeyValueChangeSetting to each observer registered for the key, including those that are registered with other objects using key paths that locate the keyed value in this object. Invocations of these methods must always be paired.

The change dictionaries in notifications resulting from use of these methods contain optional entries if requested at observer registration time:
    - The NSKeyValueChangeOldKey entry, if present, contains the value returned by -valueForKey: at the instant that -willChangeValueForKey: is invoked (or an NSNull if -valueForKey: returns nil).
    - The NSKeyValueChangeNewKey entry, if present, contains the value returned by -valueForKey: at the instant that -didChangeValueForKey: is invoked (or an NSNull if -valueForKey: returns nil).
*/
- (void)willChangeValueForKey:(NSString *)key;
- (void)didChangeValueForKey:(NSString *)key;

/* Given a key that identifies an _ordered_ to-many relationship, send -observeValueForKeyPath:ofObject:change:context: notification messages of the passed-in change kind to each observer registered for the key, including those that are registered with other objects using key paths that locate the keyed value in this object. The passed-in kind must be NSKeyValueChangeInsertion, NSKeyValueChangeRemoval, or NSKeyValueChangeReplacement. The passed-in index set must be the indexes of the objects being inserted, removed, or replaced. Invocations of these methods must always be paired, with identical arguments.

The change dictionaries in notifications resulting from use of these methods contain optional entries if requested at observer registration time:
    - The NSKeyValueChangeOldKey entry, if present (only for NSKeyValueChangeRemoval and NSKeyValueChangeReplacement), contains an array of the indexed objects from the array returned by -valueForKey: at the instant that -willChangeValueForKey:valuesAtIndexes:forKey: is invoked.
    - The NSKeyValueChangeNewKey entry, if present (only for NSKeyValueChangeInsertion and NSKeyValueChangeReplacement), contains an array of the indexed objects from the array returned by -valueForKey: at the instant that -didChangeValueForKey:valuesAtIndexes:forKey: is invoked.
*/
- (void)willChange:(NSKeyValueChange)changeKind valuesAtIndexes:(NSIndexSet *)indexes forKey:(NSString *)key;
- (void)didChange:(NSKeyValueChange)changeKind valuesAtIndexes:(NSIndexSet *)indexes forKey:(NSString *)key;

/* Given a key that identifies an _unordered_ to-many relationship, send -observeValueForKeyPath:ofObject:change:context: notification messages to each observer registered for the key, including those that are registered with other objects using key paths that locate the keyed value in this object. The passed-in mutation kind corresponds to an NSMutableSet method. The passed-in set must contain the set that would be passed to the corresponding NSMutableSet method. Invocations of these methods must always be paired, with identical arguments.

The value of the NSKeyValueChangeKindKey entry in change dictionaries in notifications resulting from use of these methods depends on the passed-in mutationKind value:
    - NSKeyValueUnionSetMutation -> NSKeyValueChangeInsertion
    - NSKeyValueMinusSetMutation -> NSKeyValueChangeRemoval
    - NSKeyValueIntersectSetMutation -> NSKeyValueChangeRemoval
    - NSKeyValueSetSetMutation -> NSKeyValueChangeReplacement

The change dictionaries may also contain optional entries:
    - The NSKeyValueChangeOldKey entry, if present (only for for NSKeyValueChangeRemoval and NSKeyValueChangeReplacement), contains the set of objects that were removed.
    - The NSKeyValueChangeNewKey entry, if present (only for NSKeyValueChangeInsertion and NSKeyValueChangeReplacement), contains the set of objects that were added.
*/
- (void)willChangeValueForKey:(NSString *)key withSetMutation:(NSKeyValueSetMutationKind)mutationKind usingObjects:(NSSet *)objects;
- (void)didChangeValueForKey:(NSString *)key withSetMutation:(NSKeyValueSetMutationKind)mutationKind usingObjects:(NSSet *)objects;

@end

@interface NSObject(NSKeyValueObservingCustomization)

/* Return a set of key paths for properties whose values affect the value of the keyed property. When an observer for the key is registered with an instance of the receiving class, KVO itself automatically observes all of the key paths for the same instance, and sends change notifications for the key to the observer when the value for any of those key paths changes. The default implementation of this method searches the receiving class for a method whose name matches the pattern +keyPathsForValuesAffecting<Key>, and returns the result of invoking that method if it is found. So, any such method must return an NSSet too. If no such method is found, an NSSet that is computed from information provided by previous invocations of the now-deprecated +setKeys:triggerChangeNotificationsForDependentKey: method is returned, for backward binary compatibility.

This method and KVO's automatic use of it comprise a dependency mechanism that you can use instead of sending -willChangeValueForKey:/-didChangeValueForKey: messages for dependent, computed, properties.
 
You can override this method when the getter method of one of your properties computes a value to return using the values of other properties, including those that are located by key paths. Your override should typically invoke super and return a set that includes any members in the set that result from doing that (so as not to interfere with overrides of this method in superclasses).

You can't really override this method when you add a computed property to an existing class using a category, because you're not supposed to override methods in categories. In that case, implement a matching +keyPathsForValuesAffecting<Key> to take advantage of this mechanism.
*/
+ (NSSet<NSString *> *)keyPathsForValuesAffectingValueForKey:(NSString *)key API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* Return YES if the key-value observing machinery should automatically invoke -willChangeValueForKey:/-didChangeValueForKey:, -willChange:valuesAtIndexes:forKey:/-didChange:valuesAtIndexes:forKey:, or -willChangeValueForKey:withSetMutation:usingObjects:/-didChangeValueForKey:withSetMutation:usingObjects: whenever instances of the class receive key-value coding messages for the key, or mutating key-value coding-compliant methods for the key are invoked. Return NO otherwise. Starting in Mac OS 10.5, the default implementation of this method searches the receiving class for a method whose name matches the pattern +automaticallyNotifiesObserversOf<Key>, and returns the result of invoking that method if it is found. So, any such method must return BOOL too. If no such method is found YES is returned.
*/
+ (BOOL)automaticallyNotifiesObserversForKey:(NSString *)key;

/* Take or return a pointer that identifies information about all of the observers that are registered with the receiver, the options that were used at registration-time, etc. The default implementation of these methods store observation info in a global dictionary keyed by the receivers' pointers. For improved performance, you can override these methods to store the opaque data pointer in an instance variable. Overrides of these methods must not attempt to send Objective-C messages to the passed-in observation info, including -retain and -release.
*/
@property (nullable) void *observationInfo NS_RETURNS_INNER_POINTER;

@end

#if TARGET_OS_OSX

@interface NSObject(NSDeprecatedKeyValueObservingCustomization)

/* A method that was deprecated in Mac OS 10.5, in favor of using +keyPathsForValuesAffectingValueForKey:. Registers the fact that invocations of -willChangeValueForKey:/-didChangeValueForKey:, -willChange:valuesAtIndexes:forKey:/-didChange:valuesAtIndexes:forKey:, and -willChangeValueForKey:withSetMutation:usingObjects:/-didChangeValueForKey:withSetMutation:usingObjects: for any key in the passed-in array should also send notifications for the dependent key.
*/
+ (void)setKeys:(NSArray *)keys triggerChangeNotificationsForDependentKey:(NSString *)dependentKey API_DEPRECATED("Use +keyPathsForValuesAffectingValueForKey instead", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));

@end 

#endif

NS_ASSUME_NONNULL_END
