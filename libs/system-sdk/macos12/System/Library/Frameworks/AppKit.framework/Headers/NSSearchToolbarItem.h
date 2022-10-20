/*
	NSSearchToolbarItem.h
	Application Kit
	Copyright (c) 2000-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <Availability.h>
#import <AppKit/NSToolbarItem.h>

@class NSSearchField;
@class NSView;

NS_ASSUME_NONNULL_BEGIN

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

// NSSearchToolbarItem provides the standard UI behavior for integrating a search field into the toolbar.
API_AVAILABLE(macos(11.0)) API_UNAVAILABLE(macCatalyst)
@interface NSSearchToolbarItem : NSToolbarItem

// An NSSearchField displayed in the toolbar item. While inside the toolbar item, the field properties and layout constraints are managed by the item. The field should be configured before assigned. The width constraint for the field could be updated after assigned. When set to nil, will reset to a search field with the default configuration.
@property (strong) NSSearchField *searchField API_UNAVAILABLE(macCatalyst);

// The base view property is owned by the toolbar item and not available for customization.
@property (nullable, strong) NSView *view NS_UNAVAILABLE;

// When YES, the cancel button in the field resigns the first responder status of the search field as clearing the contents. The default is YES.
@property BOOL resignsFirstResponderWithCancel;

// The preferred width for the search field. This value is used to configure the search field width whenever it gets the keyboard focus.If specifying custom width constraints to the search field, they should not conflict with this value.
@property CGFloat preferredWidthForSearchField;

// With the following two methods, clients of this toolbar item API can explicitly control the search interaction session programatically.
// Starts a search interaction. If necessary, expands to the preferred width and moves the keyboard focus to the search field.
- (void)beginSearchInteraction;

// Ends a search interaction. Gives up the first responder by calling -endEditing: to the search field. Adjusts to the natural available width for the toolbar item if necessary.
- (void)endSearchInteraction;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
