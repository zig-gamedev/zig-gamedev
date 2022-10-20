/*
        NSTabView.h
        Application Kit
        Copyright (c) 2000-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSView.h>
#import <AppKit/NSCell.h>
#import <AppKit/NSLayoutConstraint.h>
#import <AppKit/NSApplication.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSFont, NSTabViewItem;
@protocol NSTabViewDelegate;

static const NSAppKitVersion NSAppKitVersionNumberWithDirectionalTabs = 631.0;

// Use tabPosition and tabViewBorderType instead
typedef NS_ENUM(NSUInteger, NSTabViewType) {
    NSTopTabsBezelBorder	= 0,			// the default
    NSLeftTabsBezelBorder	= 1,
    NSBottomTabsBezelBorder	= 2,
    NSRightTabsBezelBorder	= 3,
    NSNoTabsBezelBorder		= 4,
    NSNoTabsLineBorder		= 5,
    NSNoTabsNoBorder		= 6
};

typedef NS_ENUM(NSUInteger, NSTabPosition) {
    NSTabPositionNone                  = 0,
    NSTabPositionTop                   = 1,
    NSTabPositionLeft                  = 2,
    NSTabPositionBottom                = 3,
    NSTabPositionRight                 = 4
} API_AVAILABLE(macos(10.12));

typedef NS_ENUM(NSUInteger, NSTabViewBorderType) {
    NSTabViewBorderTypeNone            = 0,
    NSTabViewBorderTypeLine            = 1,
    NSTabViewBorderTypeBezel           = 2
} API_AVAILABLE(macos(10.12));

@interface NSTabView : NSView

	/* Select */

- (void)selectTabViewItem:(nullable NSTabViewItem *)tabViewItem;
- (void)selectTabViewItemAtIndex:(NSInteger)index;				// May raise an NSRangeException
- (void)selectTabViewItemWithIdentifier:(id)identifier;                         // May raise an NSRangeException if identifier not found
- (void)takeSelectedTabViewItemFromSender:(nullable id)sender;			// May raise an NSRangeException

	/* Navigation */

- (void)selectFirstTabViewItem:(nullable id)sender;
- (void)selectLastTabViewItem:(nullable id)sender;
- (void)selectNextTabViewItem:(nullable id)sender;
- (void)selectPreviousTabViewItem:(nullable id)sender;

	/* Getters */

@property (nullable, readonly, strong) NSTabViewItem *selectedTabViewItem;	// return nil if none are selected
@property (strong) NSFont *font;						// returns font used for all tab labels.
@property NSTabViewType tabViewType;                                            // Use tabPosition and tabViewBorderType instead. Setting this will also set the tabPosition and tabViewBorderType. Setting tabPosition or tabViewBorderType will affect tabViewType
@property NSTabPosition tabPosition API_AVAILABLE(macos(10.12));
@property NSTabViewBorderType tabViewBorderType API_AVAILABLE(macos(10.12));        // This will only be respected if NSTabPosition is NSTabPositionNone.
@property (copy) NSArray<__kindof NSTabViewItem *> *tabViewItems;
@property BOOL allowsTruncatedLabels;
@property (readonly) NSSize minimumSize;					// returns the minimum size of the tab view
@property BOOL drawsBackground;  						// only relevant for borderless tab view type
@property NSControlSize controlSize;

	/* Add/Remove tabs */

- (void)addTabViewItem:(NSTabViewItem *)tabViewItem;				// Add tab at the end.
- (void)insertTabViewItem:(NSTabViewItem *)tabViewItem atIndex:(NSInteger)index;// May raise an NSRangeException
- (void)removeTabViewItem:(NSTabViewItem *)tabViewItem;				// tabViewItem must be an existing tabViewItem

	/* Delegate */

@property (nullable, weak) id<NSTabViewDelegate> delegate;

	/* Hit testing */

- (nullable NSTabViewItem *)tabViewItemAtPoint:(NSPoint)point;			// point in local coordinates. returns nil if none.

	/* Geometry */

@property (readonly) NSRect contentRect;					// Return the rect available for a "page".

	/* Query */

@property (readonly) NSInteger numberOfTabViewItems;
- (NSInteger)indexOfTabViewItem:(NSTabViewItem *)tabViewItem;			// NSNotFound if not found
- (NSTabViewItem *)tabViewItemAtIndex:(NSInteger)index;                         // May raise an NSRangeException	
- (NSInteger)indexOfTabViewItemWithIdentifier:(id)identifier;			// NSNotFound if not found

@property NSControlTint controlTint API_DEPRECATED("The controlTint property is not respected on 10.14 and later.", macos(10.0, 10.14));

@end

//================================================================================
//	NSTabViewDelegate protocol
//================================================================================

@protocol NSTabViewDelegate <NSObject>
@optional
- (BOOL)tabView:(NSTabView *)tabView shouldSelectTabViewItem:(nullable NSTabViewItem *)tabViewItem;
- (void)tabView:(NSTabView *)tabView willSelectTabViewItem:(nullable NSTabViewItem *)tabViewItem;
- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(nullable NSTabViewItem *)tabViewItem;
- (void)tabViewDidChangeNumberOfTabViewItems:(NSTabView *)tabView;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
