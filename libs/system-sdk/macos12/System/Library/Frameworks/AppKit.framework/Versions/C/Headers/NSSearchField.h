/*
    NSSearchField.h
    Application Kit
    Copyright (c) 2003-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSTextField.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NSString * NSSearchFieldRecentsAutosaveName NS_SWIFT_BRIDGED_TYPEDEF;

@class NSSearchField;

@protocol NSSearchFieldDelegate <NSTextFieldDelegate>
@optional
- (void)searchFieldDidStartSearching:(NSSearchField *)sender API_AVAILABLE(macos(10.11));
- (void)searchFieldDidEndSearching:(NSSearchField *)sender API_AVAILABLE(macos(10.11));
@end

@interface NSSearchField : NSTextField

// The rectangle for the search text within the bounds of the search field. Subclasses can override this method for custom layout purposes.
@property (readonly) NSRect searchTextBounds API_AVAILABLE(macos(11.0));

// The rectangle for the search button within the bounds of the search field. Subclasses can override this method for custom layout purposes.
@property (readonly) NSRect searchButtonBounds API_AVAILABLE(macos(11.0));

// The rectangle for the cancel button within the bounds of the search field. Subclasses can override this method for custom layout purposes.
@property (readonly) NSRect cancelButtonBounds API_AVAILABLE(macos(11.0));

/* if app wants to do custom search lists. will return empty array if no searches */
@property (copy) NSArray<NSString *> *recentSearches;

/* must be set to use. default is nil which means no autosave. */
@property (nullable, copy) NSSearchFieldRecentsAutosaveName recentsAutosaveName;

/* set/get search menu template. Menu can use custom tagged items to indicate special items. this menu isn't actually set but used to construct the dynamic search menu. if cleared, then we don't track recents. */
@property (nullable, strong) NSMenu *searchMenuTemplate API_AVAILABLE(macos(10.10));

/* if clear, send action on each key stroke (after sufficient amount of time so we don't interfere with typing). if set, send only on return/enter or clicking magnifying search button */
@property BOOL sendsWholeSearchString API_AVAILABLE(macos(10.10));

/* set/get limit max recents. allowable between 0 and 254. setting -1 will use default. */
@property NSInteger maximumRecents API_AVAILABLE(macos(10.10));

@property BOOL sendsSearchStringImmediately API_AVAILABLE(macos(10.10));

@property (nullable, weak) id<NSSearchFieldDelegate> delegate API_AVAILABLE(macos(10.11));
@end

@interface NSSearchField (NSSearchField_Deprecated)
- (NSRect)rectForSearchTextWhenCentered:(BOOL)isCentered API_DEPRECATED_WITH_REPLACEMENT("searchTextBounds", macos(10.11, 12.0));

- (NSRect)rectForSearchButtonWhenCentered:(BOOL)isCentered API_DEPRECATED_WITH_REPLACEMENT("searchButtonBounds", macos(10.11, 12.0));

- (NSRect)rectForCancelButtonWhenCentered:(BOOL)isCentered API_DEPRECATED_WITH_REPLACEMENT("cancelButtonBounds", macos(10.11, 12.0));

@property BOOL centersPlaceholder API_DEPRECATED("The placeholder centering UI design is no longer available. Setting this property is no-op.", macos(10.11, 12.0));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
