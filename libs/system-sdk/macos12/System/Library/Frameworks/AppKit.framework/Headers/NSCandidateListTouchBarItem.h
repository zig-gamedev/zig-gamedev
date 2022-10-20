/*
    NSCandidateListTouchBarItem.h
    Application Kit
    Copyright (c) 2016-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSTouchBar.h>
#import <AppKit/NSTouchBarItem.h>
#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>

@protocol NSCandidateListTouchBarItemDelegate;
@protocol NSTextInputClient;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/* NSCandidateListTouchBarItem is an NSTouchBarItem that shows the word completion candidates for assisting users typing with text input capable views. In addition to candidates supplied by its client, the NSTouchBarItem can show candidates coming from the active input method. NSCandidateListTouchBarItem has two states: expanded and collapsed. When expanded, the item replaces any NSTouchBarItems occupying its location on the bar.
*/

#pragma mark NSCandidateListTouchBarItem
API_AVAILABLE(macos(10.12.2)) API_UNAVAILABLE(macCatalyst)
@interface NSCandidateListTouchBarItem<CandidateType> : NSTouchBarItem

#pragma mark Client
// The client object for the receiver.
@property (nullable, weak) NSView <NSTextInputClient> *client;

@property (nullable, weak) id <NSCandidateListTouchBarItemDelegate> delegate API_UNAVAILABLE(macCatalyst);

#pragma mark Appearance State
// Controls the visible state of the item. The default is YES.
@property (getter=isCollapsed) BOOL collapsed API_UNAVAILABLE(macCatalyst);

// When YES, the item is allowed to be collapsed. YES by default.
@property BOOL allowsCollapsing API_UNAVAILABLE(macCatalyst);

// Returns the state of its native candidate list visibility. When -collapsed=NO and not obscured by UI from the text input system, this property returns YES. KVO compliant. Clients should set candidates when YES.
@property (readonly, getter=isCandidateListVisible) BOOL candidateListVisible API_UNAVAILABLE(macCatalyst);

// Updates the candidate list visibility configuration based on the client's insertion point state. The client is expected to update the candidate list visibility configuration whenever its selection changes. When the selection length is 0 and the insertion cursor is visible, isVisible should be YES; otherwise, it should be NO. When -hasMarkedText is YES, it should be considered the cursor is visible.
- (void)updateWithInsertionPointVisibility:(BOOL)isVisible API_UNAVAILABLE(macCatalyst);

#pragma mark Input Method support
// When YES, the item displays candidates from input methods when available instead of -candidates. YES by default.
@property BOOL allowsTextInputContextCandidates;

#pragma mark Candidates
// A block function for converting a candidate object into an NSAttributedString that will be displayed in the candidate bar. nil by default. Not required for displaying NSString, NSAttributedString, and NSTextCheckingResult candidates. In absence of NSFontAttributeName and NSForegroundColorAttributeName in the returned string, the standard bar appearance font and color are used instead of Helvetica 12.0 and +[NSColor blackColor].
@property (nullable, copy) NSAttributedString * (^attributedStringForCandidate)(CandidateType candidate, NSInteger index) API_UNAVAILABLE(macCatalyst);

// Returns an array of candidate objects previously set via -setCandidates:forSelectedRange:inString:view:
@property (readonly, copy) NSArray<CandidateType> *candidates API_UNAVAILABLE(macCatalyst);
 
// Sets an array of candidate objects to be displayed in the candidate list bar item. The candidate objects are interpreted by -attributedStringForCandidate if the property is non-nil. NSCandidateListTouchBarItem can format NSString, NSAttributedString, and NSTextCheckingResult without the formatting block specified. The NSTouchBarItem will decide which candidates to place in the candidates array and in which order. The selectedRange and originalString arguments tell the context the list of candidates was originally derived from. For NSTextCheckingResult candidates, the selectedRange and originalString arguments should match those passed in to NSSpellChecker. The NSTouchBarItem will also arrange to display candidates near the selection in the view when appropriate.
- (void)setCandidates:(NSArray<CandidateType> *)candidates forSelectedRange:(NSRange)selectedRange inString:(nullable NSString *)originalString API_UNAVAILABLE(macCatalyst);

// The label displayed in the customization panel. The default is an empty string.
@property (readwrite, copy, null_resettable) NSString *customizationLabel API_UNAVAILABLE(macCatalyst);
@end

#pragma mark NSCandidateListTouchBarItemDelegate
API_UNAVAILABLE(macCatalyst)
@protocol NSCandidateListTouchBarItemDelegate <NSObject>
@optional

// Invoked when user touches down on a candidate in the bar.
- (void)candidateListTouchBarItem:(NSCandidateListTouchBarItem *)anItem beginSelectingCandidateAtIndex:(NSInteger)index API_AVAILABLE(macos(10.12.2)) API_UNAVAILABLE(macCatalyst);

// Invoked when user moves from touching one candidate in the bar to another.
- (void)candidateListTouchBarItem:(NSCandidateListTouchBarItem *)anItem changeSelectionFromCandidateAtIndex:(NSInteger)previousIndex toIndex:(NSInteger)index API_AVAILABLE(macos(10.12.2)) API_UNAVAILABLE(macCatalyst);

// Invoked when user stops touching candidates in the bar. If index==NSNotFound, user didn't select any candidate.
- (void)candidateListTouchBarItem:(NSCandidateListTouchBarItem *)anItem endSelectingCandidateAtIndex:(NSInteger)index API_AVAILABLE(macos(10.12.2)) API_UNAVAILABLE(macCatalyst);

// Invoked when -candidateListVisible changed the visibility state.
- (void)candidateListTouchBarItem:(NSCandidateListTouchBarItem *)anItem changedCandidateListVisibility:(BOOL)isVisible API_AVAILABLE(macos(10.12.2)) API_UNAVAILABLE(macCatalyst);
@end

@interface NSView (NSCandidateListTouchBarItem)
// Returns NSCandidateListTouchBarItem used by the receiver when the first responder. The default implementation just returns nil. NSTextInputContext uses the item returned from this method for showing the candidates from input methods.
@property (nullable, readonly, strong) NSCandidateListTouchBarItem *candidateListTouchBarItem API_AVAILABLE(macos(10.12.2));
@end

// The standard identifier for NSCandidateListTouchBarItem. -[NSView candidateListTouchBarItem] concrete overrides should be using this identifier for instantiating the NSTouchBarItem.
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierCandidateList API_AVAILABLE(macos(10.12.2)) API_UNAVAILABLE(macCatalyst);

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
