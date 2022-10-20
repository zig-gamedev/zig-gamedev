/*
	NSScriptObjectSpecifiers.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSArray<ObjectType>, NSAppleEventDescriptor, NSNumber, NSScriptClassDescription, NSScriptWhoseTest, NSString;

NS_ASSUME_NONNULL_BEGIN

// Error codes for specific problems evaluating specifiers
NS_ENUM(NSInteger) {
    NSNoSpecifierError = 0,
    NSNoTopLevelContainersSpecifierError, // Someone called evaluate with nil.
    NSContainerSpecifierError, // Error evaluating container specifier.
    NSUnknownKeySpecifierError, // Receivers do not understand the key.
    NSInvalidIndexSpecifierError, // Index out of bounds
    NSInternalSpecifierError, // Other internal error
    NSOperationNotSupportedForKeySpecifierError // Attempt made to perform an unsuppported opweration on some key
};


typedef NS_ENUM(NSUInteger, NSInsertionPosition) {
    NSPositionAfter,
    NSPositionBefore,
    NSPositionBeginning,
    NSPositionEnd,
    NSPositionReplace
};

typedef NS_ENUM(NSUInteger, NSRelativePosition) {
    NSRelativeAfter = 0,
    NSRelativeBefore
};

typedef NS_ENUM(NSUInteger, NSWhoseSubelementIdentifier) {
    NSIndexSubelement = 0,
    NSEverySubelement = 1,
    NSMiddleSubelement = 2,
    NSRandomSubelement = 3,
    NSNoSubelement = 4 // Only valid for the end subelement
};

// This class represents a specifier to a set of objects.  It can be evaluated to return the objects it specifiers.  This abstract superclass is subclassed for each type of specifier.
// A specifier always accesses a specific property of an object or array of objects.  The object accessed is called the container (or container).  When object specifiers are nested the container[s] are described by the container specifier.  When an object specifier has no container specifier, the container objects must be supplied explicitly.
// Object specifiers can be nested.  The child retains its container specifier.

@interface NSScriptObjectSpecifier : NSObject <NSCoding> {
    @private
    NSScriptObjectSpecifier *_container;
    NSScriptObjectSpecifier *_child;
    NSString *_key;
    NSScriptClassDescription *_containerClassDescription;
    BOOL _containerIsObjectBeingTested;
    BOOL _containerIsRangeContainerObject;
    char _padding[2];
    NSAppleEventDescriptor *_descriptor;
    NSInteger _error;
}

/* Given a typeObjectSpecifier Apple event descriptor, create and return an object specifier, or nil for failure. If this is invoked and fails during the execution of a script command, information about the error that caused the failure is recorded in [NSScriptCommand currentCommand].
*/
+ (nullable NSScriptObjectSpecifier *)objectSpecifierWithDescriptor:(NSAppleEventDescriptor *)descriptor API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

- (instancetype)initWithContainerSpecifier:(NSScriptObjectSpecifier *)container key:(NSString *)property;
    // This figures out the container class desc from the container specifier.

- (instancetype)initWithContainerClassDescription:(NSScriptClassDescription *)classDesc containerSpecifier:(nullable NSScriptObjectSpecifier *)container key:(NSString *)property NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;

@property (nullable, assign) NSScriptObjectSpecifier *childSpecifier;
    // You generally should not call the set method.  It is called automatically by setContainerSpecifier:.

@property (nullable, retain) NSScriptObjectSpecifier *containerSpecifier;
     // setContainerSpecifier: calls [child setChildSpecifier:self] as well.

@property BOOL containerIsObjectBeingTested;
    // If our containerSpecifier is nil, this flag determines whether the top-level container is the default top-level object or the object currently being tested by an NSWhoseSpecifier.
@property BOOL containerIsRangeContainerObject;
    // If our containerSpecifier is nil, this flag determines whether the top-level container is the default top-level object or the object that is the container for the current range specifier being evaluated.
    // One or neither of -containerIsObjectBeingTested and -containerIsRangeContainerObject should be set to YES.  Setting both of these to YES makes no sense.

@property (copy) NSString *key;
    // The name of the key in the container object to be accessed by this specifier.

@property (nullable, retain) NSScriptClassDescription *containerClassDescription;
@property (nullable, readonly, retain) NSScriptClassDescription *keyClassDescription;

- (nullable NSInteger *)indicesOfObjectsByEvaluatingWithContainer:(id)container count:(NSInteger *)count NS_RETURNS_INNER_POINTER;
    // Returning with count == -1 is shorthand for all indices.
    // count == 0 means no objects match.
- (nullable id)objectsByEvaluatingWithContainers:(id)containers;
@property (nullable, readonly, retain) id objectsByEvaluatingSpecifier;

@property NSInteger evaluationErrorNumber;
@property (nullable, readonly, retain) NSScriptObjectSpecifier *evaluationErrorSpecifier;

/* Return an Apple event descriptor that represents the receiver. If the receiver was created with +objectSpecifierWithDescriptor: that passed-in descriptor is returned. Otherwise a new one is created and returned (autoreleased, of course).
*/ 
@property (nullable, readonly, copy) NSAppleEventDescriptor *descriptor API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

@end


@interface NSObject (NSScriptObjectSpecifiers)

@property (nullable, readonly, retain) NSScriptObjectSpecifier *objectSpecifier;
    // Overridden by objects that can provide a fully specified object specifier to themselves within an app.

- (nullable NSArray<NSNumber *> *)indicesOfObjectsByEvaluatingObjectSpecifier:(NSScriptObjectSpecifier *)specifier;
    // Containers that want to evaluate some specifiers on their own should implement this method.  The result array should be full of NSNumber objects which identify the indices of the matching objects.  If this method returns nil, the object specifier will go on to do its own evaluation.  If this method returns an array, the object specifier will use the NSNumbers in it as the indices.  So, if you evaluate the specifier and there are no objects which match, you should return an empty array, not nil.  If you find only one object you should still return its index in an array.  Returning an array with a single index where the index is -1 is interpretted to mean all the objects.

@end


// An Index specifiers return the object at the specified index for the specifier's property.  A negative index counts from the end of the array.

@interface NSIndexSpecifier : NSScriptObjectSpecifier {
    @private
    NSInteger _index;
}

- (instancetype)initWithContainerClassDescription:(NSScriptClassDescription *)classDesc containerSpecifier:(nullable NSScriptObjectSpecifier *)container key:(NSString *)property index:(NSInteger)index NS_DESIGNATED_INITIALIZER;

@property NSInteger index;

@end


// A Middle specifier returns the middle object from the objects for the specifier's property.  If there are an even number of objects it returns the object before the midpoint.

@interface NSMiddleSpecifier : NSScriptObjectSpecifier {}

@end


// A Name specifier returns the object with the specified name.

@interface NSNameSpecifier : NSScriptObjectSpecifier {
    @private
    NSString *_name;
}

- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithContainerClassDescription:(NSScriptClassDescription *)classDesc containerSpecifier:(nullable NSScriptObjectSpecifier *)container key:(NSString *)property name:(NSString *)name NS_DESIGNATED_INITIALIZER;
@property (copy) NSString *name;

@end


@interface NSPositionalSpecifier : NSObject {
    @private
    NSScriptObjectSpecifier *_specifier;
    NSInsertionPosition _unadjustedPosition;
    NSScriptClassDescription *_insertionClassDescription;
    id _moreVars;
    void *_reserved0;
}

// Given an object specifier and an insertion position relative to the specified object, initialize.
- (instancetype)initWithPosition:(NSInsertionPosition)position objectSpecifier:(NSScriptObjectSpecifier *)specifier NS_DESIGNATED_INITIALIZER;

// Return the position or object specifier that was specified at initialization time.
@property (readonly) NSInsertionPosition position API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);
@property (readonly, retain) NSScriptObjectSpecifier *objectSpecifier API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

// Set the class description for the object or objects to be inserted.  This message can be sent at any time after object initialization, but must be sent before evaluation to have any effect.
- (void)setInsertionClassDescription:(NSScriptClassDescription *)classDescription;

// Evaluate this positional specifier.  If evaluation is successful, subsequent -insertionContainer, -insertionKey, -insertionIndex, and -insertionReplaces messages sent to this object will return the results of the evaluation.
- (void)evaluate;

// Return the container into which insertion should be done, if evaluation has been successful, or nil otherwise.  If this object has never been evaluated, evaluation is attempted.
@property (nullable, readonly, retain) id insertionContainer;

// Return the key for the to-many relationship for which insertion should be done, if evaluation has been successful, or nil otherwise.  If this object has never been evaluated, evaluation is attempted.
@property (nullable, readonly, copy) NSString *insertionKey;

// Return an index into the set of keyed to-many relationship objects before which insertion should be done in the insertion container, if evaluation has been successful, or -1 otherwise.  If this object has never been evaluated, evaluation is attempted.
@property (readonly) NSInteger insertionIndex;

// Return YES if evaluation has been successful and the object to be inserted should actually replace the keyed, indexed object in the insertion container, instead of being inserted before it, or NO otherwise.  If this object has never been evaluated, evaluation is attempted.
@property (readonly) BOOL insertionReplaces;

@end


// This returns all the objects for the specifier's property.  This is used for accessing singular properties as well as for the "Every" specifier type for plural properties.

@interface NSPropertySpecifier : NSScriptObjectSpecifier {}

@end


// A Random specifier returns an object chosen at random from the objects for the specifier's property.

@interface NSRandomSpecifier : NSScriptObjectSpecifier {}

@end


// A Range specifier returns a contiguous subset of the objects for the specifier's property.

@interface NSRangeSpecifier : NSScriptObjectSpecifier {
    @private
    NSScriptObjectSpecifier *_startSpec;
    NSScriptObjectSpecifier *_endSpec;
}
- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithContainerClassDescription:(NSScriptClassDescription *)classDesc containerSpecifier:(nullable NSScriptObjectSpecifier *)container key:(NSString *)property startSpecifier:(nullable NSScriptObjectSpecifier *)startSpec endSpecifier:(nullable NSScriptObjectSpecifier *)endSpec NS_DESIGNATED_INITIALIZER;

@property (nullable, retain) NSScriptObjectSpecifier *startSpecifier;

@property (nullable, retain) NSScriptObjectSpecifier *endSpecifier;

@end


@interface NSRelativeSpecifier : NSScriptObjectSpecifier {
    @private
    NSRelativePosition _relativePosition;
    NSScriptObjectSpecifier *_baseSpecifier;
}
- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithContainerClassDescription:(NSScriptClassDescription *)classDesc containerSpecifier:(nullable NSScriptObjectSpecifier *)container key:(NSString *)property relativePosition:(NSRelativePosition)relPos baseSpecifier:(nullable NSScriptObjectSpecifier *)baseSpecifier NS_DESIGNATED_INITIALIZER;

@property NSRelativePosition relativePosition;

@property (nullable, retain) NSScriptObjectSpecifier *baseSpecifier;
    // This is another object specifier (which will be evaluated within the same container objects that this specifier is evalutated in).  To find the indices for this specifier we will evaluate the base specifier and take the index before or after the indices returned.

@end


// A Unique ID specifier returns the object with the specified ID.

@interface NSUniqueIDSpecifier : NSScriptObjectSpecifier {
    @private
    id _uniqueID;
}
- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithContainerClassDescription:(NSScriptClassDescription *)classDesc containerSpecifier:(nullable NSScriptObjectSpecifier *)container key:(NSString *)property uniqueID:(id)uniqueID NS_DESIGNATED_INITIALIZER;

@property (copy) id uniqueID;

@end


// A Qualified specifier uses a qualifier and another object specifier to get a subset of the objects for the specifier's property.  The other object specifier is evaluated for each object using that object as the container and the objects that result are tested with the qualifier.  An example makes this easier to understand.
// Take the specifier "paragraphs where color of third word is blue".
// This would result in an NSWhoseSpecifier where:
//     property name = "paragraphs"
//     other specifier = Index specifier with property "words" and index 3
//     qualifier = key value qualifier for key "color" and value [NSColor blueColor]

// The "subelement" stuff is to support stuff like "the first word whose..."

@interface NSWhoseSpecifier : NSScriptObjectSpecifier {
    @private
    NSScriptWhoseTest *_test;
    NSWhoseSubelementIdentifier _startSubelementIdentifier;
    NSInteger _startSubelementIndex;
    NSWhoseSubelementIdentifier _endSubelementIdentifier;
    NSInteger _endSubelementIndex;
}

- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithContainerClassDescription:(NSScriptClassDescription *)classDesc containerSpecifier:(nullable NSScriptObjectSpecifier *)container key:(NSString *)property test:(NSScriptWhoseTest *)test NS_DESIGNATED_INITIALIZER;

@property (retain) NSScriptWhoseTest *test;
@property NSWhoseSubelementIdentifier startSubelementIdentifier;

@property NSInteger startSubelementIndex;
    // Only used if the startSubelementIdentifier == NSIndexSubelement

@property NSWhoseSubelementIdentifier endSubelementIdentifier;

@property NSInteger endSubelementIndex;
    // Only used if the endSubelementIdentifier == NSIndexSubelement

@end

NS_ASSUME_NONNULL_END

