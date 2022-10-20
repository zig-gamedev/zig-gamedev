/*
    NSTableViewRowAction.h
    Application Kit
    Copyright (c) 2014-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSColor, NSButton, NSImage;

typedef NS_ENUM(NSInteger, NSTableViewRowActionStyle) {
    NSTableViewRowActionStyleRegular,
    NSTableViewRowActionStyleDestructive, // Destructive buttons will do a special animation when being deleted
} API_AVAILABLE(macos(10.11));

/* An array of this class should be returned from the NSTableView delegate method tableView:rowActionsForRow:edge:.
 */
API_AVAILABLE(macos(10.11))
@interface NSTableViewRowAction : NSObject <NSCopying>

+ (instancetype)rowActionWithStyle:(NSTableViewRowActionStyle)style title:(NSString *)title handler:(void (^)(NSTableViewRowAction *action, NSInteger row))handler;

@property (readonly) NSTableViewRowActionStyle style;
@property (copy) NSString *title;
@property (null_resettable, copy) NSColor *backgroundColor; // The default background color is dependent on style. Generally this is red for destructive actions, and blue for others.

/* Prefer using an image over text for the row action button
 */
@property (nullable, strong) NSImage *image API_AVAILABLE(macos(10.12));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
