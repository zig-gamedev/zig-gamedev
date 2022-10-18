/*	NSUserDefaults.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSNotification.h>

@class NSArray<ObjectType>, NSData, NSDictionary<KeyValue, ObjectValue>, NSMutableDictionary, NSString, NSURL;

NS_ASSUME_NONNULL_BEGIN

/// NSGlobalDomain identifies a domain shared between all applications for a given user. NSGlobalDomain is automatically included in all search lists, after the entries for the search list's domain.
FOUNDATION_EXPORT NSString * const NSGlobalDomain;
/// NSArgumentDomain identifies a search list entry containing the commandline arguments the application was launched with, if any. Arguments must be formatted as '-key plistvalue'. NSArgumentDomain is automatically included in all search lists, after forced defaults, but before all other entries. This can be useful for testing purposes.
FOUNDATION_EXPORT NSString * const NSArgumentDomain;
/// NSRegistrationDomain identifies a search list entry containing all defaults set with -registerDefaults:, if any. NSRegistrationDomain is automatically included as the final entry of all search lists.
FOUNDATION_EXPORT NSString * const NSRegistrationDomain;

/*!
 NSUserDefaults is a hierarchical persistent interprocess (optionally distributed) key-value store, optimized for storing user settings.
 
 Hierarchical: NSUserDefaults has a list of places to look for data called the "search list". A search list is referred to by an arbitrary string called the "suite identifier" or "domain identifier". When queried, NSUserDefaults checks each entry of its search list until it finds one that contains the key in question, or has searched the whole list. The list is (note: "current host + current user" preferences are unimplemented on iOS, watchOS, and tvOS, and "any user" preferences are not generally useful for applications on those operating systems):
 - Managed ("forced") preferences, set by a configuration profile or via mcx from a network administrator
 - Commandline arguments
 - Preferences for the current domain, in the cloud
 - Preferences for the current domain, the current user, in the current host
 - Preferences for the current domain, the current user, in any host
 - Preferences added via -addSuiteNamed:
 - Preferences global to all apps for the current user, in the current host
 - Preferences global to all apps for the current user, in any host
 - Preferences for the current domain, for all users, in the current host
 - Preferences global to all apps for all users, in the current host
 - Preferences registered with -registerDefaults:
 
 Persistent: Preferences stored in NSUserDefaults persist across reboots and relaunches of apps unless otherwise specified.
 
 Interprocess: Preferences may be accessible to and modified from multiple processes simultaneously (for example between an application and an extension).
 
 Optionally distributed (Currently only supported in Shared iPad for Students mode):  Data stored in user defaults can be made "ubiqitous", i.e. synchronized between devices via the cloud.  Ubiquitous user defaults are automatically propagated to all devices logged into the same iCloud account. When reading defaults (via -*ForKey: methods on NSUserDefaults), ubiquitous defaults are searched before local defaults. All operations on ubiquitous defaults are asynchronous, so registered defaults may be returned in place of ubiquitous defaults if downloading from iCloud hasn't finished. Ubiquitous defaults are specified in the Defaults Configuration File for an application.
 
 Key-Value Store: NSUserDefaults stores Property List objects (NSString, NSData, NSNumber, NSDate, NSArray, and NSDictionary) identified by NSString keys, similar to an NSMutableDictionary.
 
 Optimized for storing user settings: NSUserDefaults is intended for relatively small amounts of data, queried very frequently, and modified occasionally. Using it in other ways may be slow or use more memory than solutions more suited to those uses.
 
 The 'App' CFPreferences functions in CoreFoundation act on the same search lists that NSUserDefaults does.
 
 NSUserDefaults can be observed using Key-Value Observing for any key stored in it. Using NSKeyValueObservingOptionPrior to observe changes from other processes or devices will behave as though NSKeyValueObservingOptionPrior was not specified.
 */
@interface NSUserDefaults : NSObject {
@private
    id _kvo_;
    CFStringRef _identifier_;
    CFStringRef _container_;
    void *_reserved[2];
}

/*!
 +standardUserDefaults returns a global instance of NSUserDefaults configured to search the current application's search list.
 */
@property (class, readonly, strong) NSUserDefaults *standardUserDefaults;

/// +resetStandardUserDefaults releases the standardUserDefaults and sets it to nil. A new standardUserDefaults will be created the next time it's accessed. The only visible effect this has is that all KVO observers of the previous standardUserDefaults will no longer be observing it.
+ (void)resetStandardUserDefaults;

/// -init is equivalent to -initWithSuiteName:nil
- (instancetype)init;

/// -initWithSuiteName: initializes an instance of NSUserDefaults that searches the shared preferences search list for the domain 'suitename'. For example, using the identifier of an application group will cause the receiver to search the preferences for that group. Passing the current application's bundle identifier, NSGlobalDomain, or the corresponding CFPreferences constants is an error. Passing nil will search the default search list.
- (nullable instancetype)initWithSuiteName:(nullable NSString *)suitename API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;

/// -initWithUser: is equivalent to -init
- (nullable id)initWithUser:(NSString *)username API_DEPRECATED("Use -init instead", macos(10.0,10.9), ios(2.0,7.0), watchos(2.0,2.0), tvos(9.0,9.0));

/*!
 -objectForKey: will search the receiver's search list for a default with the key 'defaultName' and return it. If another process has changed defaults in the search list, NSUserDefaults will automatically update to the latest values. If the key in question has been marked as ubiquitous via a Defaults Configuration File, the latest value may not be immediately available, and the registered value will be returned instead.
 */
- (nullable id)objectForKey:(NSString *)defaultName;

/*!
 -setObject:forKey: immediately stores a value (or removes the value if nil is passed as the value) for the provided key in the search list entry for the receiver's suite name in the current user and any host, then asynchronously stores the value persistently, where it is made available to other processes.
 */
- (void)setObject:(nullable id)value forKey:(NSString *)defaultName;

/// -removeObjectForKey: is equivalent to -[... setObject:nil forKey:defaultName]
- (void)removeObjectForKey:(NSString *)defaultName;

/// -stringForKey: is equivalent to -objectForKey:, except that it will convert NSNumber values to their NSString representation. If a non-string non-number value is found, nil will be returned.
- (nullable NSString *)stringForKey:(NSString *)defaultName;

/// -arrayForKey: is equivalent to -objectForKey:, except that it will return nil if the value is not an NSArray.
- (nullable NSArray *)arrayForKey:(NSString *)defaultName;
/// -dictionaryForKey: is equivalent to -objectForKey:, except that it will return nil if the value is not an NSDictionary.
- (nullable NSDictionary<NSString *, id> *)dictionaryForKey:(NSString *)defaultName;
/// -dataForKey: is equivalent to -objectForKey:, except that it will return nil if the value is not an NSData.
- (nullable NSData *)dataForKey:(NSString *)defaultName;
/// -stringForKey: is equivalent to -objectForKey:, except that it will return nil if the value is not an NSArray<NSString *>. Note that unlike -stringForKey:, NSNumbers are not converted to NSStrings.
- (nullable NSArray<NSString *> *)stringArrayForKey:(NSString *)defaultName;
/*!
 -integerForKey: is equivalent to -objectForKey:, except that it converts the returned value to an NSInteger. If the value is an NSNumber, the result of -integerValue will be returned. If the value is an NSString, it will be converted to NSInteger if possible. If the value is a boolean, it will be converted to either 1 for YES or 0 for NO. If the value is absent or can't be converted to an integer, 0 will be returned.
 */
- (NSInteger)integerForKey:(NSString *)defaultName;
/// -floatForKey: is similar to -integerForKey:, except that it returns a float, and boolean values will not be converted.
- (float)floatForKey:(NSString *)defaultName;
/// -doubleForKey: is similar to -integerForKey:, except that it returns a double, and boolean values will not be converted.
- (double)doubleForKey:(NSString *)defaultName;
/*!
 -boolForKey: is equivalent to -objectForKey:, except that it converts the returned value to a BOOL. If the value is an NSNumber, NO will be returned if the value is 0, YES otherwise. If the value is an NSString, values of "YES" or "1" will return YES, and values of "NO", "0", or any other string will return NO. If the value is absent or can't be converted to a BOOL, NO will be returned.
 
 */
- (BOOL)boolForKey:(NSString *)defaultName;
/*!
 -URLForKey: is equivalent to -objectForKey: except that it converts the returned value to an NSURL. If the value is an NSString path, then it will construct a file URL to that path. If the value is an archived URL from -setURL:forKey: it will be unarchived. If the value is absent or can't be converted to an NSURL, nil will be returned.
 */
- (nullable NSURL *)URLForKey:(NSString *)defaultName API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/// -setInteger:forKey: is equivalent to -setObject:forKey: except that the value is converted from an NSInteger to an NSNumber.
- (void)setInteger:(NSInteger)value forKey:(NSString *)defaultName;
/// -setFloat:forKey: is equivalent to -setObject:forKey: except that the value is converted from a float to an NSNumber.
- (void)setFloat:(float)value forKey:(NSString *)defaultName;
/// -setDouble:forKey: is equivalent to -setObject:forKey: except that the value is converted from a double to an NSNumber.
- (void)setDouble:(double)value forKey:(NSString *)defaultName;
/// -setBool:forKey: is equivalent to -setObject:forKey: except that the value is converted from a BOOL to an NSNumber.
- (void)setBool:(BOOL)value forKey:(NSString *)defaultName;
/// -setURL:forKey is equivalent to -setObject:forKey: except that the value is archived to an NSData. Use -URLForKey: to retrieve values set this way.
- (void)setURL:(nullable NSURL *)url forKey:(NSString *)defaultName API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/*!
 -registerDefaults: adds the registrationDictionary to the last item in every search list. This means that after NSUserDefaults has looked for a value in every other valid location, it will look in registered defaults, making them useful as a "fallback" value. Registered defaults are never stored between runs of an application, and are visible only to the application that registers them.
 
 Default values from Defaults Configuration Files will automatically be registered.
 */
- (void)registerDefaults:(NSDictionary<NSString *, id> *)registrationDictionary;

/*!
 -addSuiteNamed: adds the full search list for 'suiteName' as a sub-search-list of the receiver's. The additional search lists are searched after the current domain, but before global defaults. Passing NSGlobalDomain or the current application's bundle identifier is unsupported.
 */
- (void)addSuiteNamed:(NSString *)suiteName;
/*!
 -removeSuiteNamed: removes a sub-searchlist added via -addSuiteNamed:.
 */
- (void)removeSuiteNamed:(NSString *)suiteName;

/*!
 -dictionaryRepresentation returns a composite snapshot of the values in the receiver's search list, such that [[receiver dictionaryRepresentation] objectForKey:x] will return the same thing as [receiver objectForKey:x].
 */
- (NSDictionary<NSString *, id> *)dictionaryRepresentation;

/*
 Volatile domains are not added to any search list, are not persisted, and are not visible to other applications. Using them is not recommended.
 */
@property (readonly, copy) NSArray<NSString *> *volatileDomainNames;
- (NSDictionary<NSString *, id> *)volatileDomainForName:(NSString *)domainName;
- (void)setVolatileDomain:(NSDictionary<NSString *, id> *)domain forName:(NSString *)domainName;
- (void)removeVolatileDomainForName:(NSString *)domainName;

/// -persistentDomainNames returns an incomplete list of domains that have preferences stored in them.
- (NSArray *)persistentDomainNames API_DEPRECATED("Not recommended", macos(10.0,10.9), ios(2.0,7.0), watchos(2.0,2.0), tvos(9.0,9.0));

/// -persistentDomainForName: returns a dictionary representation of the search list entry specified by 'domainName', the current user, and any host.
- (nullable NSDictionary<NSString *, id> *)persistentDomainForName:(NSString *)domainName;
/// -setPersistentDomain:forName: replaces all values in the search list entry specified by 'domainName', the current user, and any host, with the values in 'domain'. The change will be persisted.
- (void)setPersistentDomain:(NSDictionary<NSString *, id> *)domain forName:(NSString *)domainName;
/// -removePersistentDomainForName: removes all values from the search list entry specified by 'domainName', the current user, and any host. The change is persistent.
- (void)removePersistentDomainForName:(NSString *)domainName;

/*!
 -synchronize is deprecated and will be marked with the API_DEPRECATED macro in a future release.
 
 -synchronize blocks the calling thread until all in-progress set operations have completed. This is no longer necessary. Replacements for previous uses of -synchronize depend on what the intent of calling synchronize was. If you synchronized...
 - ...before reading in order to fetch updated values: remove the synchronize call
 - ...after writing in order to notify another program to read: the other program can use KVO to observe the default without needing to notify
 - ...before exiting in a non-app (command line tool, agent, or daemon) process: call CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication)
 - ...for any other reason: remove the synchronize call
 */
- (BOOL)synchronize;

// -objectIsForcedForKey: returns YES if the value for 'key' is provided by managed preferences (a configuration profile or mcx)
- (BOOL)objectIsForcedForKey:(NSString *)key;

// -objectIsForcedForKey:inDomain: returns YES if the value for 'key' is provided by managed preferences (a configuration profile or mcx) for the search list named by 'domain'
- (BOOL)objectIsForcedForKey:(NSString *)key inDomain:(NSString *)domain;

@end

/*!
 NSUserDefaultsSizeLimitExceededNotification is posted on the main queue when more data is stored in user defaults than is allowed. Currently there is no limit for local user defaults except on tvOS, where a warning notification will be posted at 512kB, and the process terminated at 1MB. For ubiquitous defaults, the limit depends on the logged in iCloud user.
 */
FOUNDATION_EXPORT NSNotificationName const NSUserDefaultsSizeLimitExceededNotification API_AVAILABLE(ios(9.3), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

/*!
 NSUbiquitousUserDefaultsNoCloudAccountNotification is posted on the main queue to the default notification center when a cloud default is set, but no iCloud user is logged in.
 
 This is not necessarily an error: ubiquitous defaults set when no iCloud user is logged in will be uploaded the next time one is available if configured to do so.
 */
FOUNDATION_EXPORT NSNotificationName const NSUbiquitousUserDefaultsNoCloudAccountNotification API_AVAILABLE(ios(9.3), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

/*!
 NSUbiquitousUserDefaultsDidChangeAccountsNotification is posted on the main queue to the default notification center when the user changes the primary iCloud account. The keys and values in the local key-value store have been replaced with those from the new account, regardless of the relative timestamps.
 */
FOUNDATION_EXPORT NSNotificationName const NSUbiquitousUserDefaultsDidChangeAccountsNotification API_AVAILABLE(ios(9.3), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

/*!
 NSUbiquitousUserDefaultsCompletedInitialSyncNotification is posted on the main queue when ubiquitous defaults finish downloading the first time a device is connected to an iCloud account, and when a user switches their primary iCloud account.
 */
FOUNDATION_EXPORT NSNotificationName const NSUbiquitousUserDefaultsCompletedInitialSyncNotification API_AVAILABLE(ios(9.3), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

/*!
 NSUserDefaultsDidChangeNotification is posted whenever any user defaults changed within the current process, but is not posted when ubiquitous defaults change, or when an outside process changes defaults. Using key-value observing to register observers for the specific keys of interest will inform you of all updates, regardless of where they're from.
 */
FOUNDATION_EXPORT NSNotificationName const NSUserDefaultsDidChangeNotification;

#if TARGET_OS_OSX
/* The following keys and their values are deprecated in Mac OS X 10.5 "Leopard". Developers should use NSLocale, NSDateFormatter and NSNumberFormatter to retrieve the values formerly returned by these keys.
 */
FOUNDATION_EXPORT NSString * const NSWeekDayNameArray API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSShortWeekDayNameArray API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSMonthNameArray API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSShortMonthNameArray API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSTimeFormatString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSDateFormatString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSTimeDateFormatString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSShortTimeDateFormatString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSCurrencySymbol API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSDecimalSeparator API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSThousandsSeparator API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSDecimalDigits API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSAMPMDesignation API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSHourNameDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSYearMonthWeekDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSEarlierTimeDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSLaterTimeDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSThisDayDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSNextDayDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSNextNextDayDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSPriorDayDesignations API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSDateTimeOrdering API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSInternationalCurrencyString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSShortDateFormatString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSPositiveCurrencyFormatString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT NSString * const NSNegativeCurrencyFormatString API_DEPRECATED("", macos(10.0, 10.5)) API_UNAVAILABLE(ios, watchos, tvos);
#endif

NS_ASSUME_NONNULL_END
