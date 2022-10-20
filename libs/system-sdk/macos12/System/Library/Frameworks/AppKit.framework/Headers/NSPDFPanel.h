/*
        NSPDFPanel.h
        Application Kit
        Copyright (c) 2013-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSMutableArray, NSString;
@class NSPDFInfo, NSViewController, NSWindow;

typedef NS_OPTIONS(NSInteger, NSPDFPanelOptions) {
    NSPDFPanelShowsPaperSize = 1 << 2,
    NSPDFPanelShowsOrientation = 1 << 3,
    NSPDFPanelRequestsParentDirectory = 1 << 24
} API_AVAILABLE(macos(10.9));

API_AVAILABLE(macos(10.9)) NS_SWIFT_UI_ACTOR
@interface NSPDFPanel : NSObject

/* Create a new NSPDFPanel.
*/
+ (NSPDFPanel *)panel;

/* Controller for the accessory views that will be presented in the panel by the methods below. When the panel is presented to the user the controller is automatically sent a -setRepresentedObject: message with an NSPDFInfo equivalent to those one passed to -beginSheetWithPDFInfo:modalForWindow:completionHandler:.
*/
@property (nullable, strong) NSViewController *accessoryController;

/* The options described above. The default value is 0. To allow your application to take advantage of controls that may be added by default in future versions OS X, get the options from the panel you've just created, turn on and off the flags you care about, and then set the options.
*/
@property NSPDFPanelOptions options;

/*  The user-editable file name shown in the name field. The default value is the equivalent of 'Untitled' for the current locale. Note that this string should never include a file extension.
*/
@property (copy) NSString *defaultFileName;

/* Present a print panel to the user, document-modally. When the user has dismissed it, invoke the completionHandler block. If the user didn't click the 'Cancel' button, the given NSPDFInfo will be modified to reflect the user's chosen settings, if any.
 */
- (void)beginSheetWithPDFInfo:(NSPDFInfo *)pdfInfo modalForWindow:(nullable NSWindow *)docWindow completionHandler:(void (^)(NSInteger))completionHandler;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
