/*
	NSHelpManager.h
	Application Kit
	Copyright (c) 1995-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSMapTable.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSBundle.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSApplication.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSAttributedString, NSWindow;

typedef NSString * NSHelpBookName NS_SWIFT_BRIDGED_TYPEDEF;
typedef NSString * NSHelpAnchorName NS_SWIFT_BRIDGED_TYPEDEF;
typedef NSString * NSHelpManagerContextHelpKey NS_SWIFT_BRIDGED_TYPEDEF;

NS_SWIFT_UI_ACTOR
@interface NSHelpManager : NSObject

@property (class, readonly, strong) NSHelpManager *sharedHelpManager;
@property (class, getter=isContextHelpModeActive) BOOL contextHelpModeActive;

- (void)setContextHelp:(NSAttributedString *)attrString forObject:(id)object;
- (void)removeContextHelpForObject:(id)object;
- (nullable NSAttributedString *)contextHelpForObject:(id)object;

- (BOOL)showContextHelpForObject:(id)object locationHint:(NSPoint)pt;

- (void)openHelpAnchor:(NSHelpAnchorName)anchor inBook:(nullable NSHelpBookName)book;
- (void)findString:(NSString *)query inBook:(nullable NSHelpBookName)book;

/* Register one or more help books in the given bundle.  The main bundle is automatically registered by -openHelpAnchor:inBook: and -findString:inBook:.  You can use -registerBooksInBundle: to register help books in a plugin bundle, for example.  The Info.plist in the bundle should contain a help book directory path, which specifies one or more folders containing help books.  Returns NO if the bundle doesn't contain any help books or if registration fails.  Returns YES on successful registration. */
- (BOOL)registerBooksInBundle:(NSBundle *)bundle API_AVAILABLE(macos(10.6));

@end

//
// Notifications for the activation/deactivation of the context help mode
//

APPKIT_EXTERN NSNotificationName NSContextHelpModeDidActivateNotification;
APPKIT_EXTERN NSNotificationName NSContextHelpModeDidDeactivateNotification;

//
//  Conveniences for accessing Help.plist
//

@interface NSBundle(NSBundleHelpExtension)
- (nullable NSAttributedString *)contextHelpForKey:(NSHelpManagerContextHelpKey)key;	/* return nil if not found */
@end

//
//  Convenience methods on NSApplication
//

@interface NSApplication(NSApplicationHelpExtension)
- (void)activateContextHelpMode:(nullable id)sender;
  // Target method cover for [NSHelpManager setContextHelpModeActive:YES];
- (void)showHelp:(nullable id)sender;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
