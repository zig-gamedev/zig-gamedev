/*
	NSScriptCommandDescription.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSArray<ObjectType>, NSDictionary, NSScriptCommand, NSString;

NS_ASSUME_NONNULL_BEGIN

@interface NSScriptCommandDescription : NSObject<NSCoding> {
    @private
    NSString *_suiteName;
    NSString *_plistCommandName;
    FourCharCode _classAppleEventCode;
    FourCharCode _idAppleEventCode;
    NSString *_objcClassName;
    NSObject *_resultTypeNameOrDescription;
    FourCharCode _plistResultTypeAppleEventCode;
    id _moreVars;
}

- (id)init API_UNAVAILABLE(macos, ios, watchos, tvos);

/* Initialize, given a scripting suite name, command name, and command declaration dictionary of the sort that is valid in .scriptSuite property list files.
*/
- (nullable instancetype)initWithSuiteName:(NSString *)suiteName commandName:(NSString *)commandName dictionary:(nullable NSDictionary *)commandDeclaration NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;

/* Return the suite name or command name provided at initialization time.
*/
@property (readonly, copy) NSString *suiteName;
@property (readonly, copy) NSString *commandName;

/* Return the four character codes that identify the command in Apple events.
*/
@property (readonly) FourCharCode appleEventClassCode;
@property (readonly) FourCharCode appleEventCode;

/* Return the Objective-C class name for instances of the described command.
*/
@property (readonly, copy) NSString *commandClassName;

/* Return the declared type name for the result of execution of the described command, or nil if the described command is not declared to return a result.
*/
@property (nullable, readonly, copy) NSString *returnType;

/* Return the four character code that identifies in Apple events the declared type of the result of execution of the described command.
*/
@property (readonly) FourCharCode appleEventCodeForReturnType;

/* Return the strings valid for use as keys into argument dictionaries in instances of the described command.
*/
@property (readonly, copy) NSArray<NSString *> *argumentNames;

/* Return the declared type of the named argument in instances of the described command.
*/
- (nullable NSString *)typeForArgumentWithName:(NSString *)argumentName;

/* Return the four character code that identifies in Apple events the declared type of the named argument in instances of the described command.
*/
- (FourCharCode)appleEventCodeForArgumentWithName:(NSString *)argumentName;

/* Return YES if the named argument is declared to be optional, NO otherwise.
*/
- (BOOL)isOptionalArgumentWithName:(NSString *)argumentName;

/* Create an instance of the described command.
*/
- (NSScriptCommand *)createCommandInstance;
- (NSScriptCommand *)createCommandInstanceWithZone:(nullable NSZone *)zone;

@end

NS_ASSUME_NONNULL_END
