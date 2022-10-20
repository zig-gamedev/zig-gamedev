/*
	NSPrintPanel.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSSet.h>
#import <AppKit/NSHelpManager.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSPrintInfo, NSSet, NSView, NSViewController, NSWindow, NSWindowController;

typedef NS_OPTIONS(NSUInteger, NSPrintPanelOptions) {

    /* Whether the print panel has separate controls (not in any accessory view) that allow the user to change the number of copies to print, which pages to print, paper size, orientation, and scaling, respectively.
    */
    NSPrintPanelShowsCopies = 1 << 0,
    NSPrintPanelShowsPageRange = 1 << 1,
    NSPrintPanelShowsPaperSize = 1 << 2,
    NSPrintPanelShowsOrientation = 1 << 3,
    NSPrintPanelShowsScaling = 1 << 4,
    
    /* Whether the print panel has an additional "Selection" option for the paper range.
    */
    NSPrintPanelShowsPrintSelection API_AVAILABLE(macos(10.6)) = 1 << 5,

    /* Whether the print panel has a page setup accessory view with controls that allow the user to change paper size, orientation, and scaling. Any control that appear in the main part of the panel because one of the previous options is used does not also appear in the page setup accessory view.
    */
    NSPrintPanelShowsPageSetupAccessory = 1 << 8,

    /* Whether the print panel has a built-in preview. Setting this option in a print panel that's not being presented by an NSPrintOperation is not useful. Two things you need to be aware of when this option is set:
     1) the NSPrintInfo passed into -beginSheetWithPrintInfo:modalForWindow:delegate:didEndSelector:contextInfo: or -runModalWithPrintInfo: will be retained instead of copied. This is so that the NSPrintOperation that is presenting the panel can respond to -printInfo messages by returning the NSPrintInfo that the user is actually looking at and manipulating, which is the most useful thing for it to return. The result is that the passed-in NSPrintInfo can be mutated even when the user cancels the print panel, but that's OK; the factory methods that you use to create NSPrintOperations do the copying that's appropriate in that case.
     2) The presenting NSPrintOperation will send the printing view more messages that it would otherwise, so that it can do pagination right away, draw the preview on screen, etc.
    */
    NSPrintPanelShowsPreview = 1 << 17

} API_AVAILABLE(macos(10.5));

/* Valid values for passing into -[NSPrintPanel setJobStyleHint:].
*/
typedef NSString * NSPrintPanelJobStyleHint NS_TYPED_ENUM;
APPKIT_EXTERN NSPrintPanelJobStyleHint const NSPrintPhotoJobStyleHint;
APPKIT_EXTERN NSPrintPanelJobStyleHint const NSPrintAllPresetsJobStyleHint API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSPrintPanelJobStyleHint const NSPrintNoPresetsJobStyleHint API_AVAILABLE(macos(10.6));

/* The keys of the entries that must be in the dictionaries returned by NSPrintPanelAccessorizing's -localizedSummaryItems method.
*/
typedef NSString * NSPrintPanelAccessorySummaryKey NS_TYPED_ENUM;
APPKIT_EXTERN NSPrintPanelAccessorySummaryKey const NSPrintPanelAccessorySummaryItemNameKey API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSPrintPanelAccessorySummaryKey const NSPrintPanelAccessorySummaryItemDescriptionKey API_AVAILABLE(macos(10.5));

@protocol NSPrintPanelAccessorizing

/* Return the text that summarizes the settings that the user has chosen using this print panel accessory view and that should appear in the summary pane of the print panel. It must be an array of dictionaries (not nil), each of which has an NSPrintPanelAccessorySummaryItemNameKey entry and an NSPrintPanelAccessorySummaryItemDescriptionKey entry whose values are strings. A print panel accessory view must be KVO-compliant for "localizedSummaryItems" because NSPrintPanel observes it to keep what it displays in its Summary view up to date. (In Mac OS 10.5 there is no way for the user to see your accessory view and the Summary view at the same time, but that might not always be true in the future.)
*/
- (NSArray<NSDictionary<NSPrintPanelAccessorySummaryKey, NSString *> *> *)localizedSummaryItems;

@optional

/* Return the key paths for properties whose values affect what is drawn in the print panel's built-in preview. NSPrintPanel observes these key paths and redraws the preview when the values for any of them change. For example, if you write an accessory view that lets the user change document margins in the print panel you might provide an implementation of this method that returns a set that includes strings like @"representedObject.leftMargin", @"representedObject.rightMargin", etc. (because the representedObject would be an NSPrintInfo, which is KVO-compliant for "leftMargin," "rightMargin," etc.). This protocol method is optional because it's not necessary if you're not using NSPrintPanel's built-in preview, but if you use preview you almost certainly have to implement this method properly too.
*/
- (NSSet<NSString *> *)keyPathsForValuesAffectingPreview;

@end

NS_SWIFT_UI_ACTOR
@interface NSPrintPanel : NSObject

/* Create a new NSPrintPanel.
*/
+ (NSPrintPanel *)printPanel;


/* Controllers for the accessory views that will be presented in the print panel by the methods below. When the print panel is presented to the user each controller is automatically sent a -setRepresentedObject: message with this object's NSPrintInfo. Because NSViewControllers are KVC and KVO compliant for "representedObject," you can use one as the file's owner of an accessory view's nib and bind controls to the file's owner using key paths that start with "representedObject." to take advantage of NSPrintInfo's KVC and KVO compliance. Each controller is also automatically sent a -title message. If that returns nil the application's short name is used in the popup menu that lets the user choose an accessory view.
*/
- (void)addAccessoryController:(NSViewController<NSPrintPanelAccessorizing> *)accessoryController API_AVAILABLE(macos(10.5));
- (void)removeAccessoryController:(NSViewController<NSPrintPanelAccessorizing> *)accessoryController API_AVAILABLE(macos(10.5));
@property (readonly, copy) NSArray<__kindof NSViewController *> *accessoryControllers API_AVAILABLE(macos(10.5));

/* The options described above. In Mac OS 10.5 an -options message sent to a freshly-created NSPrintPanel will return (NSPrintPanelShowsCopies | NSPrintPanelShowsPageRange) unless it was created by an NSPrintOperation, in which case it will also return NSPrintPanelShowsPreview. (See the release notes for backward binary compatibility information though.) To allow your application to take advantage of controls that may be added by default in future versions of Mac OS X, get the options from the print panel you've just created, turn on and off the flags you care about, and then set the options.
*/
@property NSPrintPanelOptions options API_AVAILABLE(macos(10.5));

/* The title of the default button in the print panel. You can override the standard button title, "Print," when you're using an NSPrintPanel in such a way that printing isn't actually going to happen when the user presses that button.
*/
- (void)setDefaultButtonTitle:(nullable NSString *)defaultButtonTitle API_AVAILABLE(macos(10.5));
- (nullable NSString *)defaultButtonTitle API_AVAILABLE(macos(10.5));

/* The HTML help anchor for the print panel. You can override the standard anchor of the print panel's help button.
*/
@property (nullable, copy) NSHelpAnchorName helpAnchor API_AVAILABLE(macos(10.5));



// Set or get a string that provides a hint about the type of print job in which this print panel is being used. This controls the set of items that appear in the Presets menu. The string must be one of the job style hint strings declared above, or nil to show general presets.
@property (nullable, copy) NSPrintPanelJobStyleHint jobStyleHint;


/* Present a print panel to the user, document-modally. When the user has dismissed it, send the message selected by didEndSelector to the delegate, with the contextInfo as the last argument. The method selected by didEndSelector must have the same signature as:

    - (void)printPanelDidEnd:(NSPrintPanel *)printPanel returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo;
*/
- (void)beginSheetWithPrintInfo:(NSPrintInfo *)printInfo modalForWindow:(NSWindow *)docWindow delegate:(nullable id)delegate didEndSelector:(nullable SEL)didEndSelector contextInfo:(nullable void *)contextInfo;

/* Present a print panel to the user, application-modally, and return either NSOKButton or NSCancelButton. The default implementation of -runModal just invokes [self runModalWithPrintInfo:[[NSPrintOperation currentOperation] printInfo]].
*/
- (NSInteger)runModalWithPrintInfo:(NSPrintInfo *)printInfo API_AVAILABLE(macos(10.5));
- (NSInteger)runModal;


/* A simple accessor. Your -beginSheetWithPrintInfo:... delegate can use this so it doesn't have to keep a pointer to the NSPrintInfo elsewhere while waiting for the user to dismiss the print panel.
*/
@property (readonly, strong) NSPrintInfo *printInfo API_AVAILABLE(macos(10.5));


@end

@interface NSPrintPanel(NSDeprecated)

- (void)setAccessoryView:(nullable NSView *)accessoryView API_DEPRECATED("Use -addAccessoryController instead", macos(10.0,10.5));
- (nullable NSView *)accessoryView API_DEPRECATED("Use -accessoryControllers instead. For compatibility this returns the view of the first accessory controller, or nil", macos(10.0,10.5));

- (void)updateFromPrintInfo API_DEPRECATED("", macos(10.0,10.5));
- (void)finalWritePrintInfo API_DEPRECATED("", macos(10.0,10.5));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
