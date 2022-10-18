/*
	NSScriptSuiteRegistry.h
	Copyright (c) 1997-2019, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSArray<ObjectType>, NSBundle, NSData, NSDictionary<KeyType, ObjectType>, NSMutableArray, NSMutableDictionary, NSMutableSet, NSScriptClassDescription, NSScriptCommandDescription;

NS_ASSUME_NONNULL_BEGIN

@interface NSScriptSuiteRegistry : NSObject {
    @private
    BOOL _isLoadingSDEFFiles;
    BOOL _isLoadingSecurityOverride;
    BOOL _hasLoadedIntrinsics;
    char _reserved1[1];
    NSMutableSet *_seenBundles;
    NSMutableArray *_suiteDescriptionsBeingCollected;
    NSScriptClassDescription *_classDescriptionNeedingRegistration;
    NSMutableArray *_suiteDescriptions;
    NSScriptCommandDescription *_commandDescriptionNeedingRegistration;
    NSMutableDictionary *_cachedClassDescriptionsByAppleEventCode;
    NSMutableDictionary *_cachedCommandDescriptionsByAppleEventCodes;
    NSDictionary *_cachedSuiteDescriptionsByName;
    NSMutableDictionary *_complexTypeDescriptionsByName;
    NSMutableDictionary *_listTypeDescriptionsByName;
    unsigned int _nextComplexTypeAppleEventCode;
    void *_reserved2[4];
}

/* Get or set the program's single NSScriptSuiteRegistry.
*/
+ (NSScriptSuiteRegistry *)sharedScriptSuiteRegistry;
+ (void)setSharedScriptSuiteRegistry:(NSScriptSuiteRegistry *)registry;

/* Given a bundle, register class and command descriptions from any .scriptSuite/.scriptTerminology resource files in the bundle. Redundant invocations of this method are ignored.
*/
- (void)loadSuitesFromBundle:(NSBundle *)bundle;

/* Given a scripting suite declaration dictionary of the sort that is valid in .scriptSuite property list files, and a pointer to the bundle that contains the code that implements the suite, register class and command descriptions for the suite.
*/
- (void)loadSuiteWithDictionary:(NSDictionary *)suiteDeclaration fromBundle:(NSBundle *)bundle;

/* Register a scripting class or command description.
*/
- (void)registerClassDescription:(NSScriptClassDescription *)classDescription;
- (void)registerCommandDescription:(NSScriptCommandDescription *)commandDescription;

/* Return a list of all registered suite names.
*/
@property (readonly, copy) NSArray<NSString *> *suiteNames;

/* Return the four character code used to identify the named suite.
*/
- (FourCharCode)appleEventCodeForSuite:(NSString *)suiteName;

/* Return the bundle that contains the code that implements the named suite.
*/
- (nullable NSBundle *)bundleForSuite:(NSString *)suiteName;

/* Return a dictionary containing the descriptions of all of the classes or commands in the named suite, keyed by class or comand name.
*/
- (nullable NSDictionary<NSString *, NSScriptClassDescription *> *)classDescriptionsInSuite:(NSString *)suiteName;
- (nullable NSDictionary<NSString *, NSScriptCommandDescription *> *)commandDescriptionsInSuite:(NSString *)suiteName;

/* Given a four character code used to identify a scripting suite, return the name of the suite.
*/
- (nullable NSString *)suiteForAppleEventCode:(FourCharCode)appleEventCode;

/* Given a four character code used to identify a scriptable class in Apple events, return the class description.
*/
- (nullable NSScriptClassDescription *)classDescriptionWithAppleEventCode:(FourCharCode)appleEventCode;

/* Given the pair of four character codes used to identify a scripting command in Apple events, return the command description.
*/
- (nullable NSScriptCommandDescription *)commandDescriptionWithAppleEventClass:(FourCharCode)appleEventClassCode andAppleEventCode:(FourCharCode)appleEventIDCode;

/* Return suitable reply data for the standard Get AETE Apple event, given the set of scripting suites currently registered.
*/
- (nullable NSData *)aeteResource:(NSString *)languageName;

@end

NS_ASSUME_NONNULL_END
