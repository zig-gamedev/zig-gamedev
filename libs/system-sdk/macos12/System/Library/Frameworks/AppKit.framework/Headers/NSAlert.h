/*
	NSAlert.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSApplication.h>
#import <AppKit/NSHelpManager.h>
#import <AppKit/NSGraphics.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTextField, NSPanel, NSWindow, NSImage, NSButton, NSError;
@protocol NSAlertDelegate;


/* The default alert style is NSAlertStyleWarning.  NSAlertStyleCritical should be reserved for critical alerts and will cause the icon to be badged with a caution icon.
*/
typedef NS_ENUM(NSUInteger, NSAlertStyle) {
    NSAlertStyleWarning = 0,
    NSAlertStyleInformational = 1,
    NSAlertStyleCritical = 2
};

/* These are additional NSModalResponse values used by NSAlert's -runModal and -beginSheetModalForWindow:completionHandler:.

 By default, NSAlert return values are position dependent, with this mapping:
 first (rightmost) button = NSAlertFirstButtonReturn
 second button = NSAlertSecondButtonReturn
 third button = NSAlertThirdButtonReturn
 buttonPosition 3+x = NSAlertThirdButtonReturn + x

 Note that these return values do not apply to an NSAlert created via +alertWithMessageText:defaultButton:alternateButton:otherButton:informativeTextWithFormat:, which instead uses the same return values as NSRunAlertPanel.  See NSAlertDefaultReturn, etc. in NSPanel.h
 */
static const NSModalResponse NSAlertFirstButtonReturn = 1000;
static const NSModalResponse NSAlertSecondButtonReturn = 1001;
static const NSModalResponse NSAlertThirdButtonReturn = 1002;

NS_SWIFT_UI_ACTOR
@interface NSAlert : NSObject

/* Given an NSError, create an NSAlert that can be used to present the error to the user. The error's localized description, recovery suggestion, and recovery options will be used to set the alert's message text, informative text, and button titles, respectively.
*/
+ (NSAlert *)alertWithError:(NSError *)error;

@property (copy) NSString *messageText;
@property (copy) NSString *informativeText;

/* customize the icon.  By default uses the image named NSApplicationIcon.
*/
@property (null_resettable, strong) NSImage *icon;

/* customize the buttons in the alert panel.  Buttons are added from right to left (for left to right languages).
*/
- (NSButton *)addButtonWithTitle:(NSString *)title;
/* get the buttons, where the rightmost button is at index 0.
*/
@property (readonly, copy) NSArray<NSButton *> *buttons;

/* In order to customize a return value for a button:
   setTag:(NSInteger)tag;	setting a tag on a button will cause that tag to be the button's return value
   
   Note that we reserve the use of the tag for this purpose.  We also reserve the use of the target and the action.
   
   By default, the first button has a key equivalent of return which implies a pulsing default button, the button named "Cancel", if any, has a key equivalent of escape, and the button named "Don't Save", if any, has a key equivalent of cmd-d.  The following methods can be used to customize key equivalents:
   setKeyEquivalent:(NSString *)charCode:
   setKeyEquivalentModifierMask:(NSUInt)mask;
*/


/* -setShowsHelp:YES adds a help button to the alert panel. When the help button is pressed, the delegate is first consulted.  If the delegate does not implement alertShowHelp: or returns NO, then -[NSHelpManager openHelpAnchor:inBook:] is called with a nil book and the anchor specified by -setHelpAnchor:, if any.  An exception will be raised if the delegate returns NO and there is no help anchor set.
*/
@property BOOL showsHelp;

@property (nullable, copy) NSHelpAnchorName helpAnchor;

@property NSAlertStyle alertStyle;

/* The delegate of the receiver, currently only allows for custom help behavior of the alert.
   For apps linked against 10.12, this property has zeroing weak memory semantics. When linked against an older SDK this back to having `retain` semantics, matching legacy behavior.
 */
@property (nullable, weak) id<NSAlertDelegate> delegate;

/* -setShowsSuppressionButton: indicates whether or not the alert should contain a suppression checkbox.  The default is NO.  This checkbox is typically used to give the user an option to not show this alert again.  If shown, the suppression button will have a default localized title similar to @"Do not show this message again" (single-button alert) or @"Don't ask again" (multi-button alert).  You can customize this title using [[alert suppressionButton] setTitle:].  When the alert is dismissed, you can get the state of the suppression button, using [[alert suppressionButton] state] and store the result in user defaults, for example.  This setting can then be checked before showing the alert again.  By default, the suppression button is positioned below the informative text, and above the accessory view (if any) and the alert buttons, and left-aligned with the informative text.  However do not count on the placement of this button, since it might be moved if the alert panel user interface is changed in the future. If you need a checkbox for purposes other than suppression text, it is recommended you create your own using an accessory view.
*/
@property BOOL showsSuppressionButton API_AVAILABLE(macos(10.5));

/* -suppressionButton returns a suppression button which may be customized, including the title and the initial state.  You can also use this method to get the state of the button after the alert is dismissed, which may be stored in user defaults and checked before showing the alert again.  In order to show the suppression button in the alert panel, you must call -setShowsSuppressionButton:YES.
*/
@property (nullable, readonly, strong) NSButton *suppressionButton API_AVAILABLE(macos(10.5));


/* -setAccessoryView: sets the accessory view displayed in the alert panel.  By default, the accessory view is positioned below the informative text and the suppression button (if any) and above the alert buttons, left-aligned with the informative text.  If you want to customize the location of the accessory view, you must first call -layout.  See the discussion of -layout for more information.
*/
@property (nullable, strong) NSView *accessoryView API_AVAILABLE(macos(10.5));

/* -layout can be used to indicate that the alert panel should do immediate layout, overriding the default behavior of laying out lazily just before showing panel.  You should only call this method if you want to do your own custom layout after it returns.  You should call this method only after you have finished with NSAlert customization, including setting message and informative text, and adding buttons and an accessory view if needed.  You can make layout changes after this method returns, in particular to adjust the frame of an accessory view.  Note that the standard layout of the alert may change in the future, so layout customization should be done with caution.
*/
- (void)layout API_AVAILABLE(macos(10.5));


/* Run the alert as an application-modal panel and return the result.
*/
- (NSModalResponse)runModal;

/* Begins a sheet on the doc window using NSWindow's sheet API.
   If the alert has an alertStyle of NSAlertStyleCritical, it will be shown as a "critical" sheet; it will otherwise be presented as a normal sheet.
 */
- (void)beginSheetModalForWindow:(NSWindow *)sheetWindow completionHandler:(void (^ _Nullable)(NSModalResponse returnCode))handler API_AVAILABLE(macos(10.9));

/* return the application-modal panel or the document-modal sheet corresponding to this alert.
*/
@property (readonly, strong) NSWindow *window;

@end

@protocol NSAlertDelegate <NSObject>
@optional
/* the delegate should implement this if custom help behavior is required.
*/
- (BOOL)alertShowHelp:(NSAlert *)alert;
@end


@interface NSAlert (NSAlertDeprecated)

/* This was intended for use by apps migrating from the C-based API.  This uses alternate return codes that were compatible with this C-based API, but not with modern alerts, see NSAlertDefaultReturn, etc. in NSPanel.h
 Alerts should be created with the -init method and setting properties.
 */
+ (NSAlert *)alertWithMessageText:(nullable NSString *)message defaultButton:(nullable NSString *)defaultButton alternateButton:(nullable NSString *)alternateButton otherButton:(nullable NSString *)otherButton informativeTextWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(5,6) API_DEPRECATED("Use -init instead", macos(10.3,10.10));

- (void)beginSheetModalForWindow:(NSWindow *)window modalDelegate:(nullable id)delegate didEndSelector:(nullable SEL)didEndSelector contextInfo:(nullable void *)contextInfo API_DEPRECATED("Use -beginSheetModalForWindow:completionHandler: instead", macos(10.3,10.10));

@end

static const NSAlertStyle NSWarningAlertStyle API_DEPRECATED_WITH_REPLACEMENT("NSAlertStyleWarning", macos(10.3,10.12)) = NSAlertStyleWarning;
static const NSAlertStyle NSInformationalAlertStyle API_DEPRECATED_WITH_REPLACEMENT("NSAlertStyleInformational", macos(10.3,10.12)) = NSAlertStyleInformational;
static const NSAlertStyle NSCriticalAlertStyle API_DEPRECATED_WITH_REPLACEMENT("NSAlertStyleCritical", macos(10.3,10.12)) = NSAlertStyleCritical;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

