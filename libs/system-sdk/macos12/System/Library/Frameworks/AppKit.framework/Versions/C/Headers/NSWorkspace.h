/*
	NSWorkspace.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSFileManager.h>
#import <AppKit/AppKitDefines.h>
#import <mach/machine.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor, NSError, NSImage, NSView, NSNotificationCenter, NSURL, NSScreen, NSRunningApplication, NSWorkspaceOpenConfiguration, NSAppleEventDescriptor;
@class UTType;

typedef NS_OPTIONS(NSUInteger, NSWorkspaceIconCreationOptions) {
    NSExcludeQuickDrawElementsIconCreationOption    = 1 << 1,
    NSExclude10_4ElementsIconCreationOption	    = 1 << 2
};


@interface NSWorkspace : NSObject

/* Get the shared instance of NSWorkspace.  This method will create an instance of NSWorkspace if it has not been created yet.  You should not attempt to instantiate instances of NSWorkspace yourself, and you should not attempt to subclass NSWorkspace. */
@property (class, readonly, strong) NSWorkspace *sharedWorkspace;

/* Returns the NSNotificationCenter for this NSWorkspace.  All notifications in this header file must be registered on this notification center.  If you register on other notification centers, you will not receive the notifications. */
@property (readonly, strong) NSNotificationCenter *notificationCenter;

/* Open a URL, using the default handler for the URL's scheme. */
- (BOOL)openURL:(NSURL *)url;

/* Open a URL asynchronously with the given configuration. The completion handler is called on a concurrent queue with either the NSRunningApplication on success, or an NSError on failure. */
- (void)openURL:(NSURL *)url configuration:(NSWorkspaceOpenConfiguration *)configuration completionHandler:(void (^ _Nullable)(NSRunningApplication *_Nullable app, NSError *_Nullable error))completionHandler API_AVAILABLE(macos(10.15));

/* Open URLs in a specified application with the given configuration. The completion handler is called on a concurrent queue with either the NSRunningApplication on success, or an NSError on failure. */
- (void)openURLs:(NSArray<NSURL *> *)urls withApplicationAtURL:(NSURL *)applicationURL configuration:(NSWorkspaceOpenConfiguration *)configuration completionHandler:(void (^ _Nullable)(NSRunningApplication *_Nullable app, NSError *_Nullable error))completionHandler API_AVAILABLE(macos(10.15));

/* Launch the application at the specified URL with the given configuration. The completion handler is called on a concurrent queue with either the NSRunningApplication on success, or an NSError on failure. */
- (void)openApplicationAtURL:(NSURL *)applicationURL configuration:(NSWorkspaceOpenConfiguration *)configuration completionHandler:(void (^ _Nullable)(NSRunningApplication *_Nullable app, NSError *_Nullable error))completionHandler API_AVAILABLE(macos(10.15));

/* Activate the Finder and open a window selecting the file at the given path.  If fullPath is nil, this will instead open the directory specified by rootFullPath, and not select any file. If rootFullPath is the empty string (@""), the file is selected in the main viewer. Otherwise, a new file viewer is opened.
 */
- (BOOL)selectFile:(nullable NSString *)fullPath inFileViewerRootedAtPath:(NSString *)rootFullPath;

/* Activate the Finder, and open one or more windows selecting the files at the given fileURLs. */
- (void)activateFileViewerSelectingURLs:(NSArray<NSURL *> *)fileURLs API_AVAILABLE(macos(10.6));

/* Displays a Spotlight search results window in Finder for the specified query string. Finder becomes the active application, if possible. The user can further refine the search via the Finder UI. Returns YES if the communication with Finder was successful.
 */
- (BOOL)showSearchResultsForQueryString:(NSString *)queryString API_AVAILABLE(macos(10.6));

/* noteFileSystemChanged: informs listeners of a filesystem change, using the FNNotifyByPath() API.  As the FNNotify API has been supplanted by the FSEvents API, the use of this method is discouraged. */
- (void)noteFileSystemChanged:(NSString *)path;

/* Indicates whether a given directory is a package.  Returns YES if the file is a package, NO if not, or if the file does not exist or the operation otherwise failed.  A file may be a package because its filename extension indicates so (for example, .framework) or because it has the package bit set. */
- (BOOL)isFilePackageAtPath:(NSString *)fullPath;

/* Returns the icon for a file at a given path, or a generic icon if the operation fails. */
- (NSImage *)iconForFile:(NSString *)fullPath;

/* Returns the icon for a group of files at the given paths. */
- (nullable NSImage *)iconForFiles:(NSArray<NSString *> *)fullPaths;

/* Get the icon for a given type object. Returns a default icon if the operation fails.  */
- (NSImage *)iconForContentType:(UTType *)contentType API_AVAILABLE(macos(11.0));

/* If image is not nil, this sets a custom icon for the file at the given path.  If image is nil, this removes any custom icon at the given path.  Returns YES if successful, NO if not. */
- (BOOL)setIcon:(nullable NSImage *)image forFile:(NSString *)fullPath options:(NSWorkspaceIconCreationOptions)options;

/* Get the array of file labels as NSStrings.  The file label index for a particular file is available as a property on the URL.  You may listen for NSWorkspaceDidChangeFileLabelsNotification to be notified when these change. */
@property (readonly, copy) NSArray<NSString *> *fileLabels API_AVAILABLE(macos(10.6));

/* Get the corresponding array of file label colors.  This array has the same number of elements as fileLabels, and the color at a given index corresponds to the label at the same index . You may listen for NSWorkspaceDidChangeFileLabelsNotification to be notified when these change. */
@property (readonly, copy) NSArray<NSColor *> *fileLabelColors API_AVAILABLE(macos(10.6));

/* Asynchronous file operations.  recycleURLs moves the given files to the trash, and duplicateURLs duplicates them in the same manner as the Finder.  If completionHandler is not nil, it will be called when the operation is complete, on the same dispatch queue that was used for the recycleURLs: call.  Within the completion handler, the newURLs dictionary parameter maps the given URLs to their new URLs in the Trash.  Files that could not be moved to the Trash will not be present in the dictionary.
 
If the operation succeeded for every file, the error parameter will be nil.  If it failed for one or more files, the error parameter will describe the overall result of the operation in a manner suitable for presentation to the user.  The completionHandler may be nil if you are not interested in the results.
 
 These methods may show a progress indicator, or other user interface elements, at AppKit's discretion.  In Mac OS X 10.6, these methods require that the main run loop be run in a common mode.
*/
- (void)recycleURLs:(NSArray<NSURL *> *)URLs completionHandler:(void (^ _Nullable)(NSDictionary<NSURL *, NSURL *> * newURLs, NSError * _Nullable error))handler API_AVAILABLE(macos(10.6));
- (void)duplicateURLs:(NSArray<NSURL *> *)URLs completionHandler:(void (^ _Nullable)(NSDictionary<NSURL *, NSURL *> * newURLs, NSError * _Nullable error))handler API_AVAILABLE(macos(10.6));

/* Gets information about the filesystem.  fullPath is the path to any file or directory on the filesystem, including the filesystem's mount point.  The returned values have the following significance:
   - isRemovable: whether the filesystem is backed by removable media, such as a CD or floppy disk.
   - isWritable: whether the filesystem can be written to
   - isUnmountable: whether the filesystem can be unmounted.
   - description: typically the type of the volume or the name of the filesystem
   - the file system type, for example, 'hfs'
*/
- (BOOL)getFileSystemInfoForPath:(NSString *)fullPath isRemovable:(nullable BOOL *)removableFlag isWritable:(nullable BOOL *)writableFlag isUnmountable:(nullable BOOL *)unmountableFlag description:(NSString * _Nullable * _Nullable)description type:(NSString * _Nullable * _Nullable)fileSystemType;

/* Attempt to eject the volume mounted at the given path.  Returns YES if successful, NO if not, for example, if the volume is not ejectable. */
- (BOOL)unmountAndEjectDeviceAtPath:(NSString *)path;

/* Same as unmountAndEjectDeviceAtPath, except it takes an NSURL and returns an NSError. */
- (BOOL)unmountAndEjectDeviceAtURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.6));

/* extendPowerOffBy: is currently not implemented.  Do not use it. */
- (NSInteger)extendPowerOffBy:(NSInteger)requested;

/* Attempt to hide all other applications. */
- (void)hideOtherApplications;

/* Get the URL for the application with the given identifier.  This uses various heuristics in case multiple apps have the same bundle ID.  This returns nil if no app has the bundle identifier.*/
- (nullable NSURL *)URLForApplicationWithBundleIdentifier:(NSString *)bundleIdentifier API_AVAILABLE(macos(10.6));

/* Get the URL for all copies of an application with the given identifier. The system sorts the resulting array based on each application's suitability to launch. The first application is considered the best available match. This returns the empty array if no app has the bundle identifier. */
- (NSArray<NSURL *> *)URLsForApplicationsWithBundleIdentifier:(NSString *)bundleIdentifier API_AVAILABLE(macos(12.0));

/* Returns the URL to the default application that would be used to open the given URL, as if the file were double clicked in the Finder (for file URLs).  This returns nil if no app can open it, or if the file does not exist. */
- (nullable NSURL *)URLForApplicationToOpenURL:(NSURL *)url API_AVAILABLE(macos(10.6));

/* Returns URLs to all available applications that can open the given URL. The system sorts the resulting array based on each application's suitability to open the given URL. The first application is considered the best available match. This returns the empty array if no app can open it, or if the file does not exist. */
- (NSArray<NSURL *> *)URLsForApplicationsToOpenURL:(NSURL *)url API_AVAILABLE(macos(12.0));

/* Sets the default handler for files with the same UTType as the one specified. Does not apply to non-file URLs. Some UTTypes require user consent before you can change their handlers. If a change requires user consent, the system will ask the user asynchronously before invoking the completion handler. */
- (void)setDefaultApplicationAtURL:(NSURL *)applicationURL toOpenContentTypeOfFileAtURL:(NSURL *)url completionHandler:(void (^_Nullable)(NSError *_Nullable error))completionHandler API_AVAILABLE(macos(12.0));

/* Sets the default handler for URLs with the same scheme as the one specified. Some URL schemes require user consent before you can change their handlers. If a change requires user consent, the system will ask the user asynchronously before invoking the completion handler. */
- (void)setDefaultApplicationAtURL:(NSURL *)applicationURL toOpenURLsWithScheme:(NSString *)urlScheme completionHandler:(void (^_Nullable)(NSError *_Nullable error))completionHandler API_AVAILABLE(macos(12.0));

/* Sets the default handler for the specified file (rather than all files like it.) Does not apply to non-file URLs. Some files require user consent before you can change their handlers. Write access to the specified URL is required. If a change requires user consent, the system will ask the user asynchronously before invoking the completion handler. */
- (void)setDefaultApplicationAtURL:(NSURL *)applicationURL toOpenFileAtURL:(NSURL *)url completionHandler:(void (^_Nullable)(NSError *_Nullable error))completionHandler API_AVAILABLE(macos(12.0));

/* Returns the URL to the default application that would be used to open the given URL, as if the file were double clicked in the Finder.  This returns nil if no app can open it. */
- (nullable NSURL *)URLForApplicationToOpenContentType:(UTType *)contentType API_AVAILABLE(macos(12.0));

/* Returns URLs to all available applications that can open the given UTType. The system sorts the resulting array based on each application's suitability to open the given UTType. The first application is considered the best available match. This returns the empty array if no app can open it. */
- (NSArray<NSURL *> *)URLsForApplicationsToOpenContentType:(UTType *)contentType API_AVAILABLE(macos(12.0));

/* Sets the default handler for the specified UTType. Some types require user consent before you can change their handlers. If a change requires user consent, the system will ask the user asynchronously before invoking the completion handler. */
- (void)setDefaultApplicationAtURL:(NSURL *)applicationURL toOpenContentType:(UTType *)contentType completionHandler:(void (^_Nullable)(NSError *_Nullable error))completionHandler API_AVAILABLE(macos(12.0));

/* Gets the frontmost application, which is the application that will receive key events.  This is observable through KVO. */
@property (nullable, readonly, strong) NSRunningApplication *frontmostApplication API_AVAILABLE(macos(10.7));

/* Gets the menu bar owning application, which is the application that currently owns and draws the menu bar. This is observable through KVO. */
@property (nullable, readonly, strong) NSRunningApplication *menuBarOwningApplication API_AVAILABLE(macos(10.7));
@end

API_AVAILABLE(macos(10.15))
@interface NSWorkspaceOpenConfiguration : NSObject <NSCopying>
/* Instantiates a configuration object with default values. Equivalent to [[[NSWorkspaceOpenConfiguration alloc] init] autorelease]. */
+ (instancetype)configuration;

/* Display user interface elements if needed, including errors and authentication. Defaults to YES.
 The completion handler will not be invoked until the user dismisses any such UI.
 Gatekeeper UI is not affected and will always be presented if needed. */
@property BOOL promptsUserIfNeeded;

/* Add the application instance or documents to the Recent Items menu. Defaults to YES. */
@property BOOL addsToRecentItems;

/* Bring the application instance to the foreground. Defaults to YES. */
@property BOOL activates;

/* Hide the application instance. Defaults to NO. */
@property BOOL hides;

/* Hide other application instances. Defaults to NO. */
@property BOOL hidesOthers;

/* Print documents and URLs instead of opening them. Defaults to NO. */
@property (getter=isForPrinting) BOOL forPrinting;

/* Ignore any running instance of the application and launch a new one. Defaults to NO (i.e. prefers to reuse a running instance). When createsNewApplicationInstance is YES, the value of allowsRunningApplicationSubstitution is not consulted. */
@property BOOL createsNewApplicationInstance;

/* If an instance of an application is already running, but the running instance is at a different URL (and is capable of opening the provided URLs), use the running application. Defaults to YES. Set this to NO if you are letting the user select between specific versions of an application or letting them choose a particular installation. */
@property BOOL allowsRunningApplicationSubstitution;

/* Command-line arguments to pass to a new application instance. Defaults to @[].
 Only applies when a new application instance is created.
 The path to the application instance is always automatically inserted as the first argument.
 If the calling process is sandboxed, the value of this property is ignored. */
@property (copy) NSArray<NSString *> *arguments;

/* Environment variables to set in a new application instance. Defaults to @{}.
 Only applies when a new application instance is created.
 Additional environment variables may be included by the system. */
@property (copy) NSDictionary<NSString *, NSString *> *environment;

/* The first NSAppleEventDescriptor to send to the new app. If an instance of the app is already running, this is sent to that app. Defaults to nil. When nil, a default apple event will be sent, if required. */
@property (strong, nullable) NSAppleEventDescriptor *appleEvent;

/* A cpu_type_t (from <mach/machine.h>) that specifies the architecture to launch. Ignored if a new instance is not launched. Defaults to CPU_TYPE_ANY. When CPU_TYPE_ANY, the system will decide the preferred architecture based on those present in the executable. */
@property cpu_type_t architecture;

/* Only open the provided URL if it is a valid universal link with an application configured to open it. If there is no application configured, or the user disabled using it to open the link, then the open will fail with an error. Defaults to NO. */
@property BOOL requiresUniversalLinks;

@end


/* The following keys may be specified or returned in the options dictionary for setDesktopImageURL:forScreen:options:error: and desktopImageOptionsForScreen:. */
typedef NSString * NSWorkspaceDesktopImageOptionKey NS_TYPED_ENUM;

/* The value is an NSNumber containing an NSImageScaling.  If this is not specified, NSImageScaleProportionallyUpOrDown is used.  Note: NSImageScaleProportionallyDown is not currently supported.
 */
APPKIT_EXTERN NSWorkspaceDesktopImageOptionKey const NSWorkspaceDesktopImageScalingKey API_AVAILABLE(macos(10.6));

/* The value is an NSNumber containing a BOOL, which affects the interpretation of Proportional scaling types.  A NO value will make the image fully visible, but there may be empty space on the sides or top and bottom.  A YES value will cause the image to fill the entire screen, but the image may be clipped.  If this is not specified, NO is assumed.  Non-proportional scaling types ignore this value.
 */
APPKIT_EXTERN NSWorkspaceDesktopImageOptionKey const NSWorkspaceDesktopImageAllowClippingKey API_AVAILABLE(macos(10.6));

/* The value is an NSColor, which is used to fill any empty space around the image.  If not specified, a default value is used.  Currently, only colors that use or can be converted to use NSCalibratedRGBColorSpace are supported, and any alpha value is ignored.
 */
APPKIT_EXTERN NSWorkspaceDesktopImageOptionKey const NSWorkspaceDesktopImageFillColorKey API_AVAILABLE(macos(10.6));


/* Desktop images */
@interface NSWorkspace (NSDesktopImages)

/* Sets the desktop image for the given screen to the image at the given URL.  The URL must be a file URL and may not be nil.  The options dictionary may contain any of the NSWorkspaceDesktopImage keys, which control how the image is scaled on the screen.  This returns YES if the image was successfully set; otherwise, NO is returned and an error is returned by reference.

  You should normally NOT present a user interface for picking the options.  Instead, just choose appropriate defaults and allow the user to adjust them in the System Preference Pane.
 */
- (BOOL)setDesktopImageURL:(NSURL *)url forScreen:(NSScreen *)screen options:(NSDictionary<NSWorkspaceDesktopImageOptionKey, id> *)options error:(NSError **)error API_AVAILABLE(macos(10.6));

/* Returns the URL for the desktop image for the given screen.
 */
- (nullable NSURL *)desktopImageURLForScreen:(NSScreen *)screen API_AVAILABLE(macos(10.6));

/* Returns the options dictionary for the desktop image for the given screen.
 */
- (nullable NSDictionary<NSWorkspaceDesktopImageOptionKey, id> *)desktopImageOptionsForScreen:(NSScreen *)screen API_AVAILABLE(macos(10.6));

@end


typedef NS_ENUM(NSInteger, NSWorkspaceAuthorizationType) {
    NSWorkspaceAuthorizationTypeCreateSymbolicLink,
    NSWorkspaceAuthorizationTypeSetAttributes,
    NSWorkspaceAuthorizationTypeReplaceFile,
} API_AVAILABLE(macos(10.14));

API_AVAILABLE(macos(10.14))
@interface NSWorkspaceAuthorization : NSObject
@end

@interface NSWorkspace (NSWorkspaceAuthorization)
- (void)requestAuthorizationOfType:(NSWorkspaceAuthorizationType)type completionHandler:(void (^)(NSWorkspaceAuthorization * _Nullable authorization, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.14));
@end

@interface NSFileManager (NSWorkspaceAuthorization)

/* The following method returns an NSFileManager instance that can perform file system operations previously allowed by the user via -[NSWorkspace requestAuthorizationOfType:completionHandler:]. Each NSWorkspaceAuthorization you receive requires creating a new NSFileManager instance using this method.
 
 Only the following NSFileManager methods currently take advantage of an authorization:
 
 -createSymbolicLinkAtURL:withDestinationURL:error:  (NSWorkspaceAuthorizationTypeCreateSymbolicLink)
 -setAttributes:ofItemAtPath:error:  (NSWorkspaceAuthorizationTypeSetAttributes)
 -replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error:  (NSWorkspaceAuthorizationTypeReplaceFile)
 
 Note that an NSWorkspaceAuthorizationTypeSetAttributes authorization only enables -setAttributes:ofItemAtPath:error: to modify the following attributes:
 
 - NSFileOwnerAccountID
 - NSFileGroupOwnerAccountID
 - NSFilePosixPermissions
 
 Also note that for -replaceItemAtURL:withItemAtURL:backupItemName:options:resultingItemURL:error:, the backupItemName and options parameters will be ignored.
 
 These methods may also fail with any of the following errors:
 - NSWorkspaceAuthorizationInvalidError: The provided NSWorkspaceAuthorization expired or is invalid.
 - NSFileWriteUnknownError: The application failed to communicate with a helper process, or a file system error occurred.
 - NSFileWriteNoPermissionError: The operation failed for any other reason, including the user denying access to the resource, or access to a resource is denied by system policy.
 
 All other NSFileManager methods invoked on this instance will behave normally.
 */
+ (instancetype)fileManagerWithAuthorization:(NSWorkspaceAuthorization *)authorization API_AVAILABLE(macos(10.14));

@end

/* Application notifications */

/* In Mac OS X 10.6 and later, all application notifications have the following key in their userInfo.  Its value is an instance of NSRunningApplication, representing the affected app.
 */
APPKIT_EXTERN NSString * const NSWorkspaceApplicationKey API_AVAILABLE(macos(10.6));

APPKIT_EXTERN NSNotificationName NSWorkspaceWillLaunchApplicationNotification;	//	see above
APPKIT_EXTERN NSNotificationName NSWorkspaceDidLaunchApplicationNotification;	//	see above
APPKIT_EXTERN NSNotificationName NSWorkspaceDidTerminateApplicationNotification;	//	see above
APPKIT_EXTERN NSNotificationName const NSWorkspaceDidHideApplicationNotification API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSNotificationName const NSWorkspaceDidUnhideApplicationNotification API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSNotificationName const NSWorkspaceDidActivateApplicationNotification API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSNotificationName const NSWorkspaceDidDeactivateApplicationNotification API_AVAILABLE(macos(10.6));


/* Volume notifications */

/* In Mac OS X 10.6 and later, the following keys are available in the userInfo of NSWorkspaceDidMountNotification, NSWorkspaceWillUnmountNotification, NSWorkspaceDidUnmountNotification, and NSWorkspaceDidRenameVolumeNotification.
*/
APPKIT_EXTERN NSString * const NSWorkspaceVolumeLocalizedNameKey API_AVAILABLE(macos(10.6)); //NSString containing the user-visible name of the volume
APPKIT_EXTERN NSString * const NSWorkspaceVolumeURLKey API_AVAILABLE(macos(10.6));  //NSURL containing the mount path of the volume

/* The following additional keys are provided in the userInfo of NSWorkspaceDidRenameVolumeNotification.
 */
APPKIT_EXTERN NSString * const NSWorkspaceVolumeOldLocalizedNameKey API_AVAILABLE(macos(10.6)); //NSString containing the old user-visible name of the volume
APPKIT_EXTERN NSString * const NSWorkspaceVolumeOldURLKey API_AVAILABLE(macos(10.6));  //NSURL containing the old mount path of the volume

APPKIT_EXTERN NSNotificationName NSWorkspaceDidMountNotification;		//	@"NSDevicePath"
APPKIT_EXTERN NSNotificationName NSWorkspaceDidUnmountNotification;		//	@"NSDevicePath"
APPKIT_EXTERN NSNotificationName NSWorkspaceWillUnmountNotification;		//	@"NSDevicePath"

/* NSWorkspaceDidRenameVolumeNotification is posted when a volume changes its name and/or mount path.  These typically change simultaneously, in which case only one notification is posted.
 */
APPKIT_EXTERN NSNotificationName const NSWorkspaceDidRenameVolumeNotification API_AVAILABLE(macos(10.6));


/* Power notifications */
APPKIT_EXTERN NSNotificationName const NSWorkspaceWillPowerOffNotification;

APPKIT_EXTERN NSNotificationName NSWorkspaceWillSleepNotification;
APPKIT_EXTERN NSNotificationName NSWorkspaceDidWakeNotification;

APPKIT_EXTERN NSNotificationName const NSWorkspaceScreensDidSleepNotification	API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSNotificationName const NSWorkspaceScreensDidWakeNotification	API_AVAILABLE(macos(10.6));

/* Session notifications */
APPKIT_EXTERN NSNotificationName NSWorkspaceSessionDidBecomeActiveNotification;
APPKIT_EXTERN NSNotificationName NSWorkspaceSessionDidResignActiveNotification;


/* Miscellaneous notifications */

/* NSWorkspaceDidChangeFileLabelsNotification is posted when the user changes a file label color name or the color itself.  The notification object is always NSWorkspace, and there is no user info.
 */
APPKIT_EXTERN NSNotificationName const NSWorkspaceDidChangeFileLabelsNotification API_AVAILABLE(macos(10.6));

APPKIT_EXTERN NSNotificationName const NSWorkspaceActiveSpaceDidChangeNotification API_AVAILABLE(macos(10.6));


/* Everything remaining in this header is deprecated and should not be used. */
typedef NSString * NSWorkspaceFileOperationName API_DEPRECATED("", macos(10.0,10.11)) NS_TYPED_ENUM;

typedef NS_OPTIONS(NSUInteger, NSWorkspaceLaunchOptions) {
    NSWorkspaceLaunchAndPrint                 API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setForPrinting:YES] instead.", macos(10.3, 11.0)) = 0x00000002,
    NSWorkspaceLaunchWithErrorPresentation    API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setPromptsUserIfNeeded:YES] instead.", macos(10.3, 11.0)) = 0x00000040,
    NSWorkspaceLaunchInhibitingBackgroundOnly API_DEPRECATED("This option does nothing.", macos(10.3, 11.0)) = 0x00000080,
    NSWorkspaceLaunchWithoutAddingToRecents   API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setAddsToRecentItems:YES] instead.", macos(10.3, 11.0)) = 0x00000100,
    NSWorkspaceLaunchWithoutActivation        API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setActivates:NO] instead.", macos(10.3, 11.0)) = 0x00000200,
    NSWorkspaceLaunchAsync                    API_DEPRECATED("When using NSWorkspaceOpenConfiguration, all launches are asynchronous.", macos(10.3, 11.0)) = 0x00010000,
    NSWorkspaceLaunchNewInstance              API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setCreatesNewApplicationInstance:YES] instead.", macos(10.3, 11.0)) = 0x00080000,
    NSWorkspaceLaunchAndHide                  API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setHides:YES] instead.", macos(10.3, 11.0)) = 0x00100000,
    NSWorkspaceLaunchAndHideOthers            API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setHidesOthers:YES] instead.", macos(10.3, 11.0)) = 0x00200000,
    NSWorkspaceLaunchDefault                  API_DEPRECATED("Use NSWorkspaceOpenConfiguration instead.", macos(10.3, 11.0)) = NSWorkspaceLaunchAsync,

    // Deprecated classic options
    NSWorkspaceLaunchAllowingClassicStartup   API_DEPRECATED("The Classic environment is no longer supported.", macos(10.3,10.11)) = 0x00020000,
    NSWorkspaceLaunchPreferringClassic        API_DEPRECATED("The Classic environment is no longer supported.", macos(10.3,10.11)) = 0x00040000,
};

/* The following keys can be used in the configuration dictionary of the launchApplicationAtURL:, openURL:, and openURL:withApplicationAtURL: methods. Each key is optional, and if omitted, default behavior is applied. */
typedef NSString * NSWorkspaceLaunchConfigurationKey API_DEPRECATED("Use NSWorkspaceOpenConfiguration instead.", macos(10.6, 11.0)) NS_TYPED_ENUM;
APPKIT_EXTERN NSWorkspaceLaunchConfigurationKey const NSWorkspaceLaunchConfigurationAppleEvent API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setAppleEvent:] instead.", macos(10.6, 11.0)); //the first NSAppleEventDescriptor to send to the new app. If an instance of the app is already running, this is sent to that app.
APPKIT_EXTERN NSWorkspaceLaunchConfigurationKey const NSWorkspaceLaunchConfigurationArguments API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setArguments:] instead.", macos(10.6, 11.0)); //an NSArray of NSStrings, passed to the new app in the argv parameter. Ignored if a new instance is not launched.
APPKIT_EXTERN NSWorkspaceLaunchConfigurationKey const NSWorkspaceLaunchConfigurationEnvironment API_DEPRECATED("Use -[NSWorkspaceOpenConfiguration setEnvironment:] instead.", macos(10.6, 11.0)); //an NSDictionary, mapping NSStrings to NSStrings, containing environment variables to set for the new app. Ignored if a new instance is not launched.
APPKIT_EXTERN NSWorkspaceLaunchConfigurationKey const NSWorkspaceLaunchConfigurationArchitecture API_DEPRECATED("Do not specify an architecutre. When unspecified, the architecture for a new application instance will be determined based on the available architectures in its executable.", macos(10.6, 11.0)); //an NSNumber containing an NSBundleExecutableArchitecture (from NSBundle.h). Ignored if a new instance is not launched.


@interface NSWorkspace (NSDeprecated)

/* Open a file at some path. If you use the variant without the withApplication: parameter, or if you pass nil for this parameter, the default application is used. The appName parameter may be a full path to an application, or just the application's name, with or without the .app extension. If you pass YES for andDeactivate:, or call a variant without this parameter, the calling app is deactivated before the new app is launched, so that the new app may come to the foreground unless the user switches to another application in the interim. Passing YES for andDeactivate: is generally recommended.
 */
- (BOOL)openFile:(NSString *)fullPath API_DEPRECATED("Use -[NSWorkspace openURL:] instead.", macos(10.0, 11.0));
- (BOOL)openFile:(NSString *)fullPath withApplication:(nullable NSString *)appName API_DEPRECATED("Use -[NSWorkspace openURLs:withApplicationAtURL:configuration:completionHandler:] instead.", macos(10.0, 11.0));
- (BOOL)openFile:(NSString *)fullPath withApplication:(nullable NSString *)appName andDeactivate:(BOOL)flag API_DEPRECATED("Use -[NSWorkspace openURLs:withApplicationAtURL:configuration:completionHandler:] instead.", macos(10.0, 11.0));

/* Launches an application. The appName may be a full path to the app, or the name alone, with or without the .app extension. */
- (BOOL)launchApplication:(NSString *)appName API_DEPRECATED("Use -[NSWorkspace openApplicationAtURL:configuration:completionHandler:] instead.", macos(10.0, 11.0));

/* Launches the app at the given URL. If the app is successfully launched, a reference to the new running app is returned. If the app is already running, and NSWorkspaceLaunchNewInstance is not specified, then a reference to the existing app is returned. If the app could not be launched, nil is returned and an NSError is returned by reference.
 
 The configuration dictionary can be used to pass additional options to the app. The configuration dictionary may be empty, in which case default behavior applies.
 */
- (nullable NSRunningApplication *)launchApplicationAtURL:(NSURL *)url options:(NSWorkspaceLaunchOptions)options configuration:(NSDictionary<NSWorkspaceLaunchConfigurationKey, id> *)configuration error:(NSError **)error API_DEPRECATED("Use -[NSWorkspace openApplicationAtURL:configuration:completionHandler:] instead.", macos(10.6, 11.0));

/* Opens the given URL in an the application that claims it. An NSRunningApplication instance representing the app that the URL was opened in is returned. If the app could not be launched or no app claims the URL, nil is returned and an NSError is returned by reference. The options and configuration parameters are the same as those in launchApplicationAtURL:options:configuration:error:. */
- (nullable NSRunningApplication *)openURL:(NSURL *)url options:(NSWorkspaceLaunchOptions)options configuration:(NSDictionary<NSWorkspaceLaunchConfigurationKey, id> *)configuration error:(NSError **)error API_DEPRECATED("Use -[NSWorkspace openURL:configuration:completionHandler:] instead.", macos(10.10, 11.0));

/* Opens the given URLs in the application at applicationURL. Returns the NSRunningApplication for the app the URLs were opened in. If the app could not be launched, nil is returned and an NSError is returned by reference. The options and configuration parameters are the same as those in launchApplicationAtURL:options:configuration:error:. */
- (nullable NSRunningApplication *)openURLs:(NSArray<NSURL *> *)urls withApplicationAtURL:(NSURL *)applicationURL options:(NSWorkspaceLaunchOptions)options configuration:(NSDictionary<NSWorkspaceLaunchConfigurationKey, id> *)configuration error:(NSError **)error API_DEPRECATED("Use -[NSWorkspace openURLs:withApplicationAtURL:configuration:completionHandler:] instead.", macos(10.10, 11.0));

/* This currently does the same thing as launchApplication:. Its use is discouraged. */
- (BOOL)launchApplication:(NSString *)appName showIcon:(BOOL)showIcon autolaunch:(BOOL)autolaunch API_DEPRECATED("Use -[NSWorkspace openApplicationAtURL:configuration:completionHandler:] instead.", macos(10.0, 11.0));

/* Get the full path for a given application name, which may or may not have the .app extension. This indicates the application that will be opened by methods such as launchApplication:. If the application could not be found, returns nil. */
- (nullable NSString *)fullPathForApplication:(NSString *)appName API_DEPRECATED("Use -[NSWorkspace URLForApplicationWithBundleIdentifier:] instead.", macos(10.0, 11.0));

/* Get the path for the application with the given identifier. This uses various heuristics in case multiple apps have the same bundle ID. */
- (nullable NSString *)absolutePathForAppBundleWithIdentifier:(NSString *)bundleIdentifier API_DEPRECATED("Use -[NSWorkspace URLForApplicationWithBundleIdentifier:] instead.", macos(10.0, 11.0));

/* The following methods launch an app with the given bundle identifier. The descriptor describes the first AppleEvent to be sent to the process. The launchIdentifier is currently of no significance.
 If the application is already running, and NSWorkspaceLaunchNewInstance is not specified in the options, then the descriptor is delivered to the currently running app, and YES is returned.
 */
- (BOOL)launchAppWithBundleIdentifier:(NSString *)bundleIdentifier options:(NSWorkspaceLaunchOptions)options additionalEventParamDescriptor:(nullable NSAppleEventDescriptor *)descriptor launchIdentifier:(NSNumber * _Nullable * _Nullable)identifier API_DEPRECATED("Use -[NSWorkspace openApplicationAtURL:configuration:completionHandler:] instead.", macos(10.0, 11.0));
- (BOOL)openURLs:(NSArray<NSURL *> *)urls withAppBundleIdentifier:(nullable NSString *)bundleIdentifier options:(NSWorkspaceLaunchOptions)options additionalEventParamDescriptor:(nullable NSAppleEventDescriptor *)descriptor launchIdentifiers:(NSArray<NSNumber *> * _Nullable * _Nullable)identifiers API_DEPRECATED("Use -[NSWorkspace openURLs:withApplicationAtURL:configuration:completionHandler:] instead.", macos(10.10, 11.0));

- (BOOL)openTempFile:(NSString *)fullPath API_DEPRECATED("", macos(10.0,10.6));
- (void)findApplications API_DEPRECATED("", macos(10.0,10.6));
- (void)noteUserDefaultsChanged API_DEPRECATED("", macos(10.0,10.6));
- (void)slideImage:(NSImage *)image from:(NSPoint)fromPoint to:(NSPoint)toPoint API_DEPRECATED("", macos(10.0,10.6));
- (void)checkForRemovableMedia API_DEPRECATED("", macos(10.0,10.6));
- (void)noteFileSystemChanged API_DEPRECATED("", macos(10.0,10.6));
- (BOOL)fileSystemChanged API_DEPRECATED("", macos(10.0,10.6));
- (BOOL)userDefaultsChanged API_DEPRECATED("", macos(10.0,10.6));
- (nullable NSArray *)mountNewRemovableMedia API_DEPRECATED("", macos(10.0,10.6));


/* The following methods return information about an application as a dictionary containing as many of the following keys as are available:
 NSApplicationPath (the full path to the application, as a string)
 NSApplicationName (the application's name, as a string)
 NSApplicationBundleIdentifier (the application's bundle identifier, as a string)
 NSApplicationProcessIdentifier (the application's process id, as an NSNumber)
 NSApplicationProcessSerialNumberHigh (the high long of the PSN, as an NSNumber)
 NSApplicationProcessSerialNumberLow (the low long of the PSN, as an NSNumber)
 The same information will now be provided in the userInfo of the NSWorkspace notifications for application launch and termination.
 */

/* Get an NSDictionary representing the currently frontmost app, with the above keys.  The NSWorkspaceApplicationKey is also provided.  On Mac OS X 10.7 and later, use -frontmostApplication. */
- (nullable NSDictionary *)activeApplication API_DEPRECATED("Use -[NSWorkspace frontmostApplication] instead.", macos(10.0,10.11));

/* Get the mount paths of all volumes. Despite its name, this method returns URLs for network volumes as well. On Mac OS X 10.6 and later, use -[NSFileManager mountedVolumeURLsIncludingResourceValuesForKeys:options:] instead. */
- (nullable NSArray *)mountedLocalVolumePaths API_DEPRECATED("Use -[NSFileManager mountedVolumeURLsIncludingResourceValuesForKeys:options:] instead.", macos(10.0,10.11));

/* Get the mount paths of all volumes backed by removable media, such as DVDs. On Mac OS X 10.6 and later, use -[NSFileManager mountedVolumeURLsIncludingResourceValuesForKeys:options:] instead. */
- (nullable NSArray *)mountedRemovableMedia API_DEPRECATED("Use -[NSFileManager mountedVolumeURLsIncludingResourceValuesForKeys:options:] instead.", macos(10.0,10.11));


/* Gets an array of NSDictionaries with the above keys.  In addition, the NSWorkspaceApplicationKey is provided, and vends an instance of NSRunningApplication.  This method does not return applications that are UIElement or BackgroundOnly.  To access the entire list of running applications, use the -[NSWorkspace runningApplications] method, declared in NSRunningApplication.h.
 */
- (nullable NSArray *)launchedApplications API_DEPRECATED("Use -[NSWorkspace runningApplications] instead.", macos(10.0,10.7));

/* Open a file with an animation.  This currently does the same thing as openFile: and its use is discouraged. */
- (BOOL)openFile:(NSString *)fullPath fromImage:(nullable NSImage *)image at:(NSPoint)point inView:(nullable NSView *)view API_DEPRECATED("Use -[NSWorkspace openURL:] instead.", macos(10.0,10.11));


/* Performs the given file operation, blocking until complete.  source should be the directory containing the file(s).  For operations that require a destination, such as Move and Copy, destination should be the destination directory; otherwise it should be nil.  files is an array of file names that are in the source directory.
 A value is returned by reference in the tag parameter, either 0 for success, or -1 for failure.  tag may be NULL.
 */
- (BOOL)performFileOperation:(NSWorkspaceFileOperationName)operation source:(NSString *)source destination:(NSString *)destination files:(NSArray *)files tag:(nullable NSInteger *)tag API_DEPRECATED("", macos(10.0,10.11));

/* Get, by reference, the name of the app used to open a file at the given path, and the type of the file.  The type of the file will either be a filename extension or an HFS type encoded with NSFileTypeForHFSTypeCode(). Both strings are returned autoreleased.  The method returns YES if successful, NO if not. */
- (BOOL)getInfoForFile:(NSString *)fullPath application:(NSString * _Nullable* _Nullable)appName type:(NSString * _Nullable* _Nullable)type API_DEPRECATED("Use -[NSWorkspace URLForApplicationToOpenURL:] to get the URL of an application that will open a given item, or -[NSURL getResourceValue:forKey:error:] with NSURLContentTypeKey to get the type of the given item.", macos(10.0, 12.0));

/* Get the icon for a given file type.  The file type may be a filename extension, or a HFS code encoded via NSFileTypeForHFSTypeCode, or a Universal Type Identifier (UTI).   Returns a default icon if the operation fails.  */
- (NSImage *)iconForFileType:(NSString *)fileType API_DEPRECATED("Use -[NSWorkspace iconForContentType:] instead.", macos(10.0, 12.0));

/* Given an absolute file path, return the uniform type identifier (UTI) of the file, if one can be determined. Otherwise, return nil after setting *outError to an NSError that encapsulates the reason why the file's type could not be determined. If the file at the end of the path is a symbolic link the type of the symbolic link will be returned.

 You can invoke this method to get the UTI of an existing file.  To get the UTI of a URL, use the NSURLTypeIdentifierKey file system resource key from NSURL.h.
 */
- (nullable NSString *)typeOfFile:(NSString *)absoluteFilePath error:(NSError **)outError API_DEPRECATED("Use -[NSURL getResourceValue:forKey:error:] with NSURLContentTypeKey instead.", macos(10.5, 12.0));

/* Given a UTI, return a string that describes the document type and is fit to present to the user, or nil for failure.

 You can invoke this method to get the name of a type that must be shown to the user, in an alert about your application's inability to handle the type, for instance.
 */
- (nullable NSString *)localizedDescriptionForType:(NSString *)typeName API_DEPRECATED("Use UTType.localizedDescription instead.", macos(10.5, 12.0));

/* Given a UTI, return the best file name extension to use when creating a file of that type, or nil for failure.

 You can invoke this method when your application has only the base name of a file that's being written and it has to append a file name extension so that the file's type can be reliably identified later on.
 */
- (nullable NSString *)preferredFilenameExtensionForType:(NSString *)typeName API_DEPRECATED("Use UTType.preferredFilenameExtension instead.", macos(10.5, 12.0));

/* Given a file name extension and a UTI, return YES if the file name extension is a valid tag for the identified type, NO otherwise.

 You can invoke this method when your application needs to check if a file name extension can be used to reliably identify the type later on. For example, NSSavePanel uses this method to validate any extension that the user types in the panel's file name field.
 */
- (BOOL)filenameExtension:(NSString *)filenameExtension isValidForType:(NSString *)typeName API_DEPRECATED("Use +[UTType typesWithTag:tagClass:conformingToType:] to get a list of candidate types, then check if the input type conforms to any of them.", macos(10.5, 12.0));

/* Given two UTIs, return YES if the first "conforms to" to the second in the uniform type identifier hierarchy, NO otherwise. This method will always return YES if the two strings are equal, so you can also use it with other kinds of type name, including those declared in CFBundleTypeName Info.plist entries in apps that don't take advantage of the support for UTIs that was added to Cocoa in Mac OS 10.5.

 You can invoke this method when your application must determine whether it can handle a file of a known type, returned by -typeOfFile:error: for instance.

 Use this method instead of merely comparing UTIs for equality.
 */
- (BOOL)type:(NSString *)firstTypeName conformsToType:(NSString *)secondTypeName API_DEPRECATED("Use -[UTType conformsToType:] instead.", macos(10.5, 12.0));

@end

/* Possible values for operation in performFileOperation:...
 */
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceMoveOperation API_DEPRECATED("Use -[NSFileManager moveItemAtURL:toURL:error:] instead.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceCopyOperation API_DEPRECATED("Use -[NSFileManager copyItemAtURL:toURL:error:] instead.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceLinkOperation API_DEPRECATED("Use -[NSFileManager linkItemAtURL:toURL:error:] instead.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceCompressOperation API_DEPRECATED("This operation is unimplemented.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceDecompressOperation API_DEPRECATED("This operation is unimplemented.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceEncryptOperation API_DEPRECATED("This operation is unimplemented.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceDecryptOperation API_DEPRECATED("This operation is unimplemented.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceDestroyOperation API_DEPRECATED("Use -[NSFileManager removeItemAtURL:error:] instead.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceRecycleOperation API_DEPRECATED("Use -[NSWorkspace recycleURLs:completionHandler:] instead.", macos(10.0,10.11));
APPKIT_EXTERN NSWorkspaceFileOperationName NSWorkspaceDuplicateOperation API_DEPRECATED("Use -[NSWorkspace duplicateURLs:completionHandler:] instead.", macos(10.0,10.11));

APPKIT_EXTERN NSNotificationName NSWorkspaceDidPerformFileOperationNotification API_DEPRECATED("", macos(10.0,10.11));	//	@"NSOperationNumber"

APPKIT_EXTERN NSString * NSPlainFileType API_DEPRECATED("", macos(10.0,10.6));
APPKIT_EXTERN NSString * NSDirectoryFileType API_DEPRECATED("", macos(10.0,10.6));
APPKIT_EXTERN NSString * NSApplicationFileType API_DEPRECATED("", macos(10.0,10.6));
APPKIT_EXTERN NSString * NSFilesystemFileType API_DEPRECATED("", macos(10.0,10.6));
APPKIT_EXTERN NSString * NSShellCommandFileType API_DEPRECATED("", macos(10.0,10.6));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
