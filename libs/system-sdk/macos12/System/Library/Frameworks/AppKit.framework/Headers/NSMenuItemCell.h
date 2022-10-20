/*
        NSMenuItemCell.h
        Application Kit
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSButtonCell.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSMenuItem.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSMenuItemCell : NSButtonCell

- (instancetype)initTextCell:(NSString *)string NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (nullable, strong) NSMenuItem *menuItem;


@property BOOL needsSizing;
- (void)calcSize;

@property BOOL needsDisplay;

@property (readonly) CGFloat stateImageWidth;
@property (readonly) CGFloat imageWidth;
@property (readonly) CGFloat titleWidth;
@property (readonly) CGFloat keyEquivalentWidth;

- (NSRect)stateImageRectForBounds:(NSRect)cellFrame;
- (NSRect)titleRectForBounds:(NSRect)cellFrame;
- (NSRect)keyEquivalentRectForBounds:(NSRect)cellFrame;

- (void)drawSeparatorItemWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)drawStateImageWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)drawImageWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)drawTitleWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)drawKeyEquivalentWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)drawBorderAndBackgroundWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;

@property NSInteger tag;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
