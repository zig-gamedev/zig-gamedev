/*
	NSScriptStandardSuiteCommands.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSScriptCommand.h>

@class NSDictionary<KeyType, ObjectType>;
@class NSScriptObjectSpecifier;
@class NSScriptClassDescription;
@class NSString;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, NSSaveOptions) {
    NSSaveOptionsYes = 0,
    NSSaveOptionsNo,
    NSSaveOptionsAsk
};

@interface NSCloneCommand : NSScriptCommand {
    @private
    NSScriptObjectSpecifier *_keySpecifier;
}

- (void)setReceiversSpecifier:(nullable NSScriptObjectSpecifier *)receiversRef;
    // Splits off the inner-most child specifier.  The rest is the receiver specifier while the child is the key specifier.

@property (readonly, retain) NSScriptObjectSpecifier *keySpecifier;

@end


@interface NSCloseCommand : NSScriptCommand {}

@property (readonly) NSSaveOptions saveOptions;
    // This converts the "SaveOptions" argument into a constant that can be easily tested.  You should use this instead of accessing the SaveOptions argument directly.

@end


@interface NSCountCommand : NSScriptCommand {}

@end


@interface NSCreateCommand : NSScriptCommand {
    @private
    id _moreVars2;
}

@property (readonly, retain) NSScriptClassDescription *createClassDescription;
    // Returns the class description for the class that is to be created (using the "ObjectClass" argument to figure it out).

@property (readonly, copy) NSDictionary<NSString *, id> *resolvedKeyDictionary;
    // The actual "KeyDictionary" argument has appleEventCodes as keys.  This method returns a version that has those codes resolved to actual key names (using the -createClassDescription to resolve the keys).

@end


@interface NSDeleteCommand : NSScriptCommand {
    @private
    NSScriptObjectSpecifier *_keySpecifier;
}

- (void)setReceiversSpecifier:(nullable NSScriptObjectSpecifier *)receiversRef;
    // Splits off the inner-most child specifier.  The rest is the receiver specifier while the child is the key specifier.

@property (readonly, retain) NSScriptObjectSpecifier *keySpecifier;

@end


@interface NSExistsCommand : NSScriptCommand {}

@end


@interface NSGetCommand : NSScriptCommand {}

@end


@interface NSMoveCommand : NSScriptCommand {
    @private
    NSScriptObjectSpecifier *_keySpecifier;
}

- (void)setReceiversSpecifier:(nullable NSScriptObjectSpecifier *)receiversRef;
    // Splits off the inner-most child specifier.  The rest is the receiver specifier while the child is the key specifier.

@property (readonly, retain) NSScriptObjectSpecifier *keySpecifier;

@end


@interface NSQuitCommand : NSScriptCommand {}

@property (readonly) NSSaveOptions saveOptions;
    // This converts the "SaveOptions" argument into a constant that can be easily tested.  You should use this instead of accessing the SaveOptions argument directly.

@end


@interface NSSetCommand : NSScriptCommand {
    @private
    NSScriptObjectSpecifier *_keySpecifier;
}

- (void)setReceiversSpecifier:(nullable NSScriptObjectSpecifier *)receiversRef;
    // Splits off the inner-most child specifier.  The rest is the receiver specifier while the child is the key specifier.

@property (readonly, retain) NSScriptObjectSpecifier *keySpecifier;

@end

NS_ASSUME_NONNULL_END
