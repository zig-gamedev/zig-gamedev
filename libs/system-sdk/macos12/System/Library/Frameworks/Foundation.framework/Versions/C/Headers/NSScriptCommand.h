/*
	NSScriptCommand.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSAppleEventDescriptor, NSDictionary<KeyType, ObjectType>, NSMutableDictionary, NSScriptObjectSpecifier, NSScriptCommandDescription, NSString;

NS_ASSUME_NONNULL_BEGIN

// Error codes for general command execution problems (not typedef'd to allow subclasses to define their own values)
NS_ENUM(NSInteger) {
    NSNoScriptError = 0,
    NSReceiverEvaluationScriptError,  // Problem evaluating receiver specifier
    NSKeySpecifierEvaluationScriptError,  // Problem evaluating key specifier (for commands that have them)
    NSArgumentEvaluationScriptError,  // Problem evaluating an argument specifier
    NSReceiversCantHandleCommandScriptError,  // The receivers dont support the command
    NSRequiredArgumentsMissingScriptError,  // One or more required arguments are missing
    NSArgumentsWrongScriptError,  // One or more arguments are of wrong type or are otherwise invalid
    NSUnknownKeyScriptError,  // Other miscellaneous internal error
    NSInternalScriptError,  // Other miscellaneous internal error
    NSOperationNotSupportedForKeyScriptError,
    NSCannotCreateScriptCommandError
};

@interface NSScriptCommand : NSObject <NSCoding> {

    // All instance variables are private.
    @private
    NSScriptCommandDescription *_commandDescription;
    id _directParameter;
    NSScriptObjectSpecifier *_receiversSpecifier;
    id _evaluatedReceivers;
    NSDictionary *_arguments;
    NSMutableDictionary *_evaluatedArguments;
    struct {
        unsigned int hasEvaluatedReceivers:1;
        unsigned int hasEvaluatedArguments:1;
        unsigned int RESERVED:30;
    } _flags;
    id _moreVars;
    void *_reserved;

}

- (instancetype)initWithCommandDescription:(NSScriptCommandDescription *)commandDef NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)inCoder;

@property (readonly, retain) NSScriptCommandDescription *commandDescription;
    // The command description is immutable once the command is created.

@property (nullable, retain) id directParameter;
    // Accessors for the object that corresponds to the keyDirectObject parameter of the Apple event from which this command derives.

@property (nullable, retain) NSScriptObjectSpecifier *receiversSpecifier;
    // Accessors for the specifier of the object(s) that will be given a chance to handle the command, if the direct parameter of the original event was an object specifier.  For backwards compatibility, -[NSScriptCommand setDirectParameter:] sends a [self setReceiversSpecifier:] message if the direct parameter is an object specifier.
    // Some types of commands may treat this somewhat specially.  For instance, the Set command backs off the innermost component of this specifier and uses the innermost component as the key to be set within that specifiers container specifier.

@property (nullable, readonly, retain) id evaluatedReceivers;
    // If the direct parameter of the original event was an object specifier, and that object specifier can be (or already has been) evaluated successfully, return the specified object(s).  Return nil otherwise.

@property (nullable, copy) NSDictionary<NSString *, id> *arguments;
@property (nullable, readonly, copy) NSDictionary<NSString *, id> *evaluatedArguments;
    // Arguments can be either normal objects or object specifiers.  If they are object specifiers they will be evaluated before being used.

@property (getter=isWellFormed, readonly) BOOL wellFormed;
    // This checks the command against the command's description.  It makes sure there is a defined command and that the number of arguments and the types of non-specifier arguments is well formed with respect to the command description.

// Given that the command has no receivers that explicitly handle the command, do some appropriate action.  -[NSScriptCommand executeCommand] invokes this method, and it is not meant to be invoked from anywhere else.  You can override this method when creating a new kind of command class that is not handled by the receivers of the command.
// If -suspend is invoked during the invocation of this method, its return value is ignored by Cocoa Scripting's built-in Apple event handling.
- (nullable id)performDefaultImplementation;

// Evaluate the receiver specifier and any argument specifiers there may be, and validate using -isWellFormed.  If the command is invalid set the error number and possibly the error string and return.  If the command is valid, and there are receivers, and if a handling method for this command is declared for the class of the receivers, invoke the handler method on each receiver.  If there are multiple receivers the results of invoking the receivers' handlers methods are placed in an array and the array is returned; if there is a single receiver the result of invoking its handler method for this command is returned.  If the command is valid but there are no receivers, or if there are but a handler method for this command is not declared for their class, invoke -performDefaultImplementation on self and return the result.  You should not have to override this method.
// If -suspend is invoked during the invocation of this method, its return value is ignored by Cocoa Scripting's built-in Apple event handling.
- (nullable id)executeCommand;

// Set the error number, offending object descriptor, type descriptor, or message, respectively, that will be put in the reply to the Apple event from which this command was constructed, when execution of the command is completed, if the sender of the event requested a reply. If -setScriptErrorNumber: is invoked during an invocation of -executeCommand, the invocation of -executeCommand may stop invoking command handling methods in the receiver and return early.
@property NSInteger scriptErrorNumber;
@property (nullable, retain) NSAppleEventDescriptor *scriptErrorOffendingObjectDescriptor API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);
@property (nullable, retain) NSAppleEventDescriptor *scriptErrorExpectedTypeDescriptor API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);
@property (nullable, copy) NSString *scriptErrorString;

// If a command is being executed in the current thread by Cocoa Scripting's built-in Apple event handling (i.e., an instance of NSScriptCommand is handling an -executeCommand message at this instant, as the result of the dispatch of an Apple event), return the command.  Return nil otherwise.  -setScriptErrorNumber: and -setScriptErrorString: messages sent to the returned command object will affect the reply event sent to the sender of the event from which the command was constructed, if the sender has requested a reply.
+ (nullable NSScriptCommand *)currentCommand;

// If the receiver was constructed by Cocoa Scripting's built-in Apple event handling, the Apple event descriptor from which it was constructed.  The effects of mutating or retaining this descriptor are undefined, though it may be copied.
@property (nullable, readonly, copy) NSAppleEventDescriptor *appleEvent;

// If the receiver is being executed in the current thread by Cocoa Scripting's built-in Apple event handling (i.e., would be returned by [NSScriptCommand currentCommand]), suspend the execution of the command.  Otherwise, do nothing.  A matching invocation of -resumeExecutionWithResult: must be made.
- (void)suspendExecution;

// If a successful, unmatched, invocation -suspendExecution has been made, resume the execution of the command.  Otherwise, do nothing.  The result is for the segment of command execution that was suspended:
// - If -suspendExecution was invoked from within a command handler of one the command's receivers, the result is considered to be the return value of the handler.  Unless the command has received a -setScriptErrorNumber: message with a nonzero error number, execution of the command will continue and the command handlers of other receivers will be invoked.
// - If -suspendExecution was invoked from within an override of -performDefaultImplementation the result is treated as if it were the return value of the invocation of -performDefaultImplementation.
// This method may be invoked in any thread, not just the one in which the corresponding invocation of -suspendExecution occurred.
- (void)resumeExecutionWithResult:(nullable id)result;

@end

NS_ASSUME_NONNULL_END
