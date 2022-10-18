/*
	NSDictionaryController.h
	Application Kit
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
 */

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSArrayController.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

// NSDictionaryController transforms a dictionary into an array of key-value pairs that is displayed like any other array in an NSArrayController. For each key-value pair, the controller distinguishes between the actual "key" and the "localizedKey" (a user readable version of the key - see setLocalizedKeyDictionary:/-localizedKeyDictionary). If no localizedKeyDictionary is specified, the localized keys default to the keys.
// In general, the dictionary controller's validation logic will prevent duplicate keys (but developers need to turn on "Validates Immediately" for the value bindings of the controls editing keys to run that validation). For insertions of new entries, the controller will enumerate the key by default (so if the initialKey is "key", the inserted keys will be "key", "key1", "key2", and so on). To customize that behavior, override the -newObject method.

// This key-value pair object allows controls to be bound, for example, to arrangedObjects.localizedKey, arrangedObjects.key, arrangedObjects.value, and arrangedObjects.explicitlyIncluded of the controller. Mutating a key-value-pair object immediately results in the corresponding change in the content dictionary of the controller.
API_AVAILABLE(macos(10.11))
@interface NSDictionaryControllerKeyValuePair : NSObject

- (instancetype)init NS_UNAVAILABLE; // NSDictionaryControllerKeyValuePair instances must be created via -newObject on the NSDictionaryController.

@property (nullable, copy) NSString *key;
@property (nullable, strong) id value;

@property (nullable, copy) NSString *localizedKey; // The localized key from the NSDictionaryController’s localizedKeyDictionary
@property (readonly, getter=isExplicitlyIncluded) BOOL explicitlyIncluded; // YES if the key is in the NSDictionaryController’s includedKeys, otherwise NO

@end

API_AVAILABLE(macos(10.5))
@interface NSDictionaryController : NSArrayController

- (NSDictionaryControllerKeyValuePair *)newObject;    // overridden from the superclass to create a key-value pair to represent an entry in the content dictionary of the controller; method is invoked for insertions of new key-value pairs as well as transforming existing dictionary entries into key-value pairs for display

// Initial key and value are assigned to newly inserted entries. The initial key will be copied, the initial value will be retained when inserted into the dictionary (not copied), and must implement NSCoding if the dictionary controller gets archived.
@property (copy) NSString *initialKey;
@property (strong) id initialValue;

// Included keys are always represented by a key-value pair in the display array, whether present in the underlying dictionary or not. Excluded keys are always suppressed in the display array.
@property (copy) NSArray<NSString *> *includedKeys;
@property (copy) NSArray<NSString *> *excludedKeys;

// Localized key dictionary allows to specify a (typically localized) string for each key in the dictionary (the dictionary needs to contain the keys as keys and the localized keys as values).
@property (copy) NSDictionary<NSString *, NSString *> *localizedKeyDictionary;
@property (nullable, copy) NSString *localizedKeyTable;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

