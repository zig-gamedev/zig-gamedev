/*
	NSUserScriptTask.h
	Copyright (c) 2012-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSAppleEventDescriptor, NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSError, NSFileHandle, NSString, NSURL, NSXPCConnection;

NS_ASSUME_NONNULL_BEGIN

/*
    These classes are intended to execute user-supplied scripts, and will execute them outside of the application's sandbox, if any.  (They are *not* intended to execute scripts built into an application; for that, use NSTask, NSAppleScript, or AMWorkflow.)  If the application is sandboxed, then the script must be in the "application scripts" folder, which you can get using +[NSFileManager URLForDirectory:NSApplicationScriptsDirectory ...].  A sandboxed application may read from, but not write to, this folder.

    If you simply need to execute scripts without regard to input or output, use NSUserScriptTask, which can execute any of the specific types.  If you need specific control over the input to or output from the script, use one of the sub-classes, which have more detailed "execute" methods.
*/

/* NSUserScriptTask: An abstract "user script".
*/
API_AVAILABLE(macos(10.8)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSUserScriptTask : NSObject {
    @protected
    NSURL *_scriptURL;
    NSXPCConnection *_connection;
    BOOL _hasExeced;
    BOOL _hasTerminated;
    NSFileHandle *_stdin;
    NSFileHandle *_stdout;
    NSFileHandle *_stderr;
}

// Initialize given a URL for a script file.  The returned object will be of one of the specific sub-classes below, or nil if the file does not appear to match any of the known types.  (If used from a sub-class, the result will be of that class, or nil.)
- (nullable instancetype)initWithURL:(NSURL *)url error:(NSError **)error NS_DESIGNATED_INITIALIZER;

@property (readonly, copy) NSURL *scriptURL;

// Execute the script with no input and ignoring any result.  This and the other "execute" methods below may be called at most once on any given instance.  If the script completed normally, the completion handler's "error" parameter will be nil.
typedef void (^NSUserScriptTaskCompletionHandler)(NSError * _Nullable error);
- (void)executeWithCompletionHandler:(nullable NSUserScriptTaskCompletionHandler)handler;

@end


/* NSUserUnixTask: a Unix executable file, typically a shell script.
*/
API_AVAILABLE(macos(10.8)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSUserUnixTask : NSUserScriptTask

// Standard I/O streams.  Setting them to nil (the default) will bind them to /dev/null.
@property (nullable, retain) NSFileHandle *standardInput;
@property (nullable, retain) NSFileHandle *standardOutput;
@property (nullable, retain) NSFileHandle *standardError;

// Execute the file with the given arguments.  "arguments" is an array of NSStrings.  The arguments do not undergo shell expansion, so you do not need to do special quoting, and shell variables are not resolved.
typedef void (^NSUserUnixTaskCompletionHandler)(NSError *_Nullable error);
- (void)executeWithArguments:(nullable NSArray<NSString *> *)arguments completionHandler:(nullable NSUserUnixTaskCompletionHandler)handler;

@end


/* NSUserAppleScriptTask: an AppleScript script.
*/
API_AVAILABLE(macos(10.8)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSUserAppleScriptTask : NSUserScriptTask {
    @private
    BOOL _isParentDefaultTarget;
}

// Execute the AppleScript script by sending it the given Apple event.  Pass nil to execute the script's default "run" handler.
typedef void (^NSUserAppleScriptTaskCompletionHandler)(NSAppleEventDescriptor * _Nullable result, NSError * _Nullable error);
- (void)executeWithAppleEvent:(nullable NSAppleEventDescriptor *)event completionHandler:(nullable NSUserAppleScriptTaskCompletionHandler)handler;

@end


/* NSUserAutomatorTask: an Automator workflow.
*/
API_AVAILABLE(macos(10.8)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSUserAutomatorTask : NSUserScriptTask {
    @private
    NSDictionary *_variables;
}

// Workflow variables.
@property (nullable, copy) NSDictionary<NSString *, id> *variables;

// Execute the Automator workflow, passing it the given input.
typedef void (^NSUserAutomatorTaskCompletionHandler)(id _Nullable result, NSError * _Nullable error);
- (void)executeWithInput:(nullable id <NSSecureCoding>)input completionHandler:(nullable NSUserAutomatorTaskCompletionHandler)handler;

@end

NS_ASSUME_NONNULL_END
