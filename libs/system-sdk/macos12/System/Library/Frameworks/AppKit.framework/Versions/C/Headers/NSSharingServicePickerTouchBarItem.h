/*
    NSSharingServicePickerTouchBarItem.h
    Application Kit
    Copyright (c) 2016-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSTouchBarItem.h>
#import <AppKit/NSSharingService.h>
#import <AppKit/AppKitDefines.h>
#import <TargetConditionals.h>

#if !TARGET_OS_IPHONE
@class NSImage;
#else
@class UIImage;
#endif

@protocol NSSharingServicePickerTouchBarItemDelegate;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(10.12.2), ios(10.13))
@interface NSSharingServicePickerTouchBarItem : NSTouchBarItem

#if !TARGET_OS_IPHONE
@property (weak) id<NSSharingServicePickerTouchBarItemDelegate> delegate;
#endif

/* Enables or disabled the sharing button; if the popover is shown, it will first be closed.
 */
@property (getter=isEnabled) BOOL enabled;

/* Get/set the button title and image. By default, the button title is an empty string, and the button image is a share picker image. */
@property (copy) NSString *buttonTitle;

#if !TARGET_OS_IPHONE
@property (nullable, strong) NSImage *buttonImage;
#else
@property (nullable, strong) UIImage *buttonImage;
#endif

@end

#if !TARGET_OS_IPHONE
@protocol NSSharingServicePickerTouchBarItemDelegate <NSSharingServicePickerDelegate>
@required
/* Return the items that represent the objects to be shared. They must conform to the <NSPasteboardWriting> protocol or be an NSItemProvider. (e.g. NSString, NSImage, NSURL, etc.). */
- (NSArray *)itemsForSharingServicePickerTouchBarItem:(NSSharingServicePickerTouchBarItem *)pickerTouchBarItem;

@end
#endif

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
