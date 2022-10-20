/*
        NSTextFinder.h
        Application Kit
        Copyright (c) 2003-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSRange.h>
#import <AppKit/NSNibDeclarations.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSView, NSOperationQueue;
@protocol NSTextFinderClient, NSTextFinderBarContainer;

typedef NS_ENUM(NSInteger, NSTextFinderAction) {
    NSTextFinderActionShowFindInterface = 1,
    NSTextFinderActionNextMatch = 2,
    NSTextFinderActionPreviousMatch = 3,
    NSTextFinderActionReplaceAll = 4,
    NSTextFinderActionReplace = 5,
    NSTextFinderActionReplaceAndFind = 6,
    NSTextFinderActionSetSearchString = 7,
    NSTextFinderActionReplaceAllInSelection = 8,
    NSTextFinderActionSelectAll = 9,
    NSTextFinderActionSelectAllInSelection = 10,
    NSTextFinderActionHideFindInterface = 11,
    NSTextFinderActionShowReplaceInterface = 12,
    NSTextFinderActionHideReplaceInterface = 13
} API_AVAILABLE(macos(10.7));


/* Values for communicating NSTextFinder search options via pasteboard. Use the NSPasteboardTypeTextFinderOptions type.
*/
typedef NSString * NSPasteboardTypeTextFinderOptionKey NS_TYPED_ENUM;
APPKIT_EXTERN NSPasteboardTypeTextFinderOptionKey const NSTextFinderCaseInsensitiveKey API_AVAILABLE(macos(10.7));       // BOOL
APPKIT_EXTERN NSPasteboardTypeTextFinderOptionKey const NSTextFinderMatchingTypeKey API_AVAILABLE(macos(10.7));          // NSNumber containing NSTextFinderMatchingType

typedef NS_ENUM(NSInteger, NSTextFinderMatchingType) {
    NSTextFinderMatchingTypeContains = 0,
    NSTextFinderMatchingTypeStartsWith = 1,
    NSTextFinderMatchingTypeFullWord = 2,
    NSTextFinderMatchingTypeEndsWith = 3
} API_AVAILABLE(macos(10.7));


API_AVAILABLE(macos(10.7))
@interface NSTextFinder : NSObject <NSCoding>

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* A text finder must be associated with an object which implements the NSTextFinderClient protocol for it to function. The client is responsible for providing the string to be searched, the location for the find bar, and methods which control feedback to the user about the search results. */
@property (nullable, assign) IBOutlet id <NSTextFinderClient> client;

/* Responders to -performTextFinderAction: should call these methods. The -validateAction: method should be called by an implementation of -validateUserInterfaceItem: to properly validate items with an action of -performTextFinderAction:. The -performAction: method should be called by the implementation of -performTextFinderAction:. Both methods should pass the item's or sender's tag as the action parameter to an instance of NSTextFinder that it creates and initializes with the above method. */
- (void)performAction:(NSTextFinderAction)op;
- (BOOL)validateAction:(NSTextFinderAction)op;

/* This property must be set to support the find bar. When the find bar is requested to be shown, NSTextFinder will call -showFindBarView: on the container, passing the view for the find bar, which it should display somewhere that is easily associated with the content being searched. NSScrollView already implements NSTextFinderBarContainer and is an excellent place to display the find bar, in most circumstances. The container may freely modify the find bar view's width and origin, but not its height. If this property is not set, then the find bar cannot be shown. */
@property (nullable, assign) IBOutlet id <NSTextFinderBarContainer> findBarContainer;


/* Find indicator control - NSTextFinder and NSView will handle the find indicator correctly when a content view is resized, moved, or removed from the view hierarchy. If your content view's scrolling is done by an NSScrollView, the find indicator will also be handled for you during scrolling. The following methods allow a client to deal with situations outside of these. */

/* There may be some circumstances where the find indicator should be immediately cancelled or hidden, such as when the view's content or selection is changed without the knowledge of the NSTextFinder. This method will immediately cancel the current find indicator. */
- (void)cancelFindIndicator;

/* If your client's document is not scrolled by NSScrollView, then you should set this property to YES when scrolling occurs to cause the find indicator to be updated appropriately. */
@property BOOL findIndicatorNeedsUpdate;


/* Incremental Search */

/* An NSTextFinder uses this property's value to determine if it should perform regular or incremental searches. If YES, then the find bar will do incremental searches. If it returns NO, then the find bar will behave regularly. The default value is NO. */
@property (getter=isIncrementalSearchingEnabled) BOOL incrementalSearchingEnabled;

/* NSTextFinder uses this property's value to determine what kind of incremental search feedback should be presented. If YES, then when an incremental search begins, the findBarContainer's contentView will be dimmed, except for the locations of the incremental matches. If NO, then the incremental matches will not be highlighted automatically, but you can use incrementalMatchRanges to highlight the matches yourself. The default value is YES. */
@property BOOL incrementalSearchingShouldDimContentView;

/* This array is updated periodically on the main queue as the incremental search operation on a background queue finds matches. You can use this property when incrementalSearchingShouldDimContentView is NO to know where to draw highlights for incremental matches. This array is KVO compliant and can be observed to know when to update your highlights. When NSKeyValueObservingOptionNew and NSKeyValueObservingOptionOld are used, the KVO change dictionary provides the ranges (and their indexes) that are added or removed so you can invalidate the minimal region needed to bring your highlights into sync with the NSTextFinder's results. If no incremental search is active, or there are no matches found, this array will be empty. If an incremental search is currently in progress, but not yet complete, this will return all the ranges found so far. */
@property (readonly, copy) NSArray<NSValue *> *incrementalMatchRanges;

/* If you set incrementalSearchingShouldDimContentView is NO to highlight incremental matches in your own view, you are encouraged to use this method to draw the highlight. */
+ (void)drawIncrementalMatchHighlightInRect:(NSRect)rect;

/* When incremental search is enabled, this method must be called when it is known that the client's string will be modified, but before the modification takes place. */
- (void)noteClientStringWillChange;

@end


@protocol NSTextFinderClient <NSObject>

/*
 All user interface items that control text finding ("Find...", "Find Next", etc.) have -performTextFinderAction: as their action and a tag that corresponds to one of the values in the NSTextFinderAction enum. A responder may implement this method by calling +performAction:forClient: on an NSTextFinder instance. The responder passes both the sender's tag, and the client that the NSTextFinder will interact with.
 
 The responder to -performTextFinderAction: should also implement -validateUserInterfaceItem:. When the parameter to this method has an action equal to -performTextFinderAction:, the responder should call +[NSTextFinder validateAction:forClient:], passing the item's tag and the client in a similar manner to its implementation of -performTextFinderAction:.
 */

@optional

/* NSTextFinder uses the following properties to validate individual actions. If these properties are not implemented, NSTextFinder will act as if they returned YES. */
@property (getter=isSelectable, readonly) BOOL selectable;
@property (readonly) BOOL allowsMultipleSelection;
@property (getter=isEditable, readonly) BOOL editable;


/* The client must implement at least one of the following two sets of API to provide NSTextFinder access to the content to be searched. If both sets are implemented by the client, the last one will be preferred over the first. */

/* If the client contains a single string, then the client can implement this property to return that string to be searched. */
@property (readonly, strong) NSString *string;

/*
 If the client cannot logically or efficiently flatten itself into a single string, then the following two methods should be used instead. These methods require the client to conceptually map its content to a single string, which is composed of a concatenation of all its substrings.
 
 The first method asks for the string that contains the given character index, which the client should return. This client should also return, by reference, the "effective range" of that substring in the full conceptually concatenated string. Finally, the client should return whether the substring ends with a "search boundary", meaning that NSTextFinder should not attempt to find any matches that overlap this boundary.
 
 The client should report the full length of the conceptually concatenated string in the second model -- the sum of the lengths of all of its substrings.
*/
- (NSString *)stringAtIndex:(NSUInteger)characterIndex effectiveRange:(NSRangePointer)outRange endsWithSearchBoundary:(BOOL *)outFlag;
- (NSUInteger)stringLength;


/* When an action is sent via +performAction:withClient:, NSTextFinder will perform that action, but it may need additional information from the client, or it may require the client to perform some actions on its behalf. The following methods and properties provide the hooks NSTextFinder needs for each of the actions it supports. If the client does not implement one of these methods or properties, then the action that requires it will be disabled. */

/* This property is required for the NextMatch, PreviousMatch, Replace, ReplaceAndFind, and SetSearchString actions. The client just needs to return its first selected range, or {index, 0} to indicate the location of the insertion point if there is no selection. */
@property (readonly) NSRange firstSelectedRange;

/* This property is required for the ReplaceAllInSelection, SelectAll, and SelectAllInSelection actions. The NSArray should contain NSRanges wrapped by NSValues. */
@property (copy) NSArray<NSValue *> *selectedRanges;

/* This method is used by all actions, but is not strictly required by any. This method instructs the client to scroll its view to make the given range visible. */
- (void)scrollRangeToVisible:(NSRange)range;

/* These methods are used by the ShowReplaceInterface, Replace, ReplaceAll(InSelection) and ReplaceAndFind actions. Since NSTextFinder can only read the content from the client, the client is responsible for performing replace operations itself, which are done via these methods. Before a replace operation is performed, NSTextFinder calls the first method to determine if a replacement should take place. If it returns NO, then the characters in the given ranges will not be replaced. If the method returns YES, or is not implemented, then it will call the second method, instructing the client to carry out the replacement. Finally, the third method will be called, if implemented, to indicate that the replacement was completed. For ReplaceAll actions, these methods will be called multiple times, starting from the last match and moving toward the first, in order to preserve the indexes of the matches which precede the current one. */
- (BOOL)shouldReplaceCharactersInRanges:(NSArray<NSValue *> *)ranges withStrings:(NSArray<NSString *> *)strings;
- (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)string;
- (void)didReplaceCharacters;


/* This method is used when displaying feedback about find operations to the user. The client should return the view the contains the character at the given index. It should also return by reference the entire range of the string displayed by the view. */
- (NSView *)contentViewAtIndex:(NSUInteger)index effectiveCharacterRange:(NSRangePointer)outRange;

/* NSTextFinder uses this method to determine the location to display the find indicator. The client should convert the given content range to an array of rectangles in the contentView's coordinate system and return that array. The given range is guaranteed not to overlap multiple views. */
- (nullable NSArray<NSValue *> *)rectsForCharacterRange:(NSRange)range;

/* NSTextFinder uses this property's value to determine which ranges it should search to show all of the incremental matches that are currently visible. If this property is not implemented, then the incremental matches cannot be shown. */
@property (readonly, copy) NSArray<NSValue *> *visibleCharacterRanges;

/* Draw the glyphs for the requested character range as they are drawn in the given content view. If the character range partially intersects a glyph range, then the full glyph is can be drawn to avoid additional layout. The given range is guaranteed to be completely contained by the given view. When this method is called, a drawing context effectively identical to the one provided to the view’s -drawRect: will be set up. This method is mainly used to draw find indicator contents, so implementations should check -isDrawingFindIndicator to ensure that the text will be easily readable against the background of the find indicator when it returns YES. If this method is not implemented, then the find indicator will be drawn using the content view’s -drawRect: method instead. */
- (void)drawCharactersInRange:(NSRange)range forContentView:(NSView *)view;

@end


@protocol NSTextFinderBarContainer <NSObject>

@required

/* This property is used by NSTextFinder to assign a find bar to a container. The container may freely modify the view's width, but should not modify its height. This property is managed by NSTextFinder. You should not set this property. */
@property (nullable, strong) NSView *findBarView;

/* This property controls whether the receiver should display its find bar or not. When this property is YES and the findBarView property is set, then the find bar should be displayed by the container. Otherwise, the find bar should not be displayed. The default value should be NO. */
@property (getter=isFindBarVisible) BOOL findBarVisible;

/* A notification that the find bar changed height, signaling the container to re-tile, if appropriate. */
- (void)findBarViewDidChangeHeight;

@optional

/* This contentView should contain all the views which display the contents being searched. This content view defines the area to be dimmed during incremental search, if incrementalSearchingShouldDimContentView is YES. If this method is not implemented or returns nil, then NSTextFinder will act as if incrementalSearchingShouldDimContentView is NO. */
- (nullable NSView *)contentView;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
