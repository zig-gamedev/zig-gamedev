#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextContentManager.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextContentManager.h
//  Text Kit
//
//  Copyright (c) 2018-2021, Apple Inc. All rights reserved.
//

#import <Foundation/NSArray.h>
#import <Foundation/NSNotification.h>
#import <AppKit/AppKitDefines.h>

@class NSTextRange;
@class NSTextElement;
@class NSTextParagraph;
@class NSTextLayoutManager;
@class NSTextStorage;
@class NSTextContentStorage;
@protocol NSTextLocation;
@protocol NSTextContentManagerDelegate;
@protocol NSTextStorageObserving;

NS_ASSUME_NONNULL_BEGIN

#pragma mark NSTextElementProvider
typedef NS_OPTIONS(NSUInteger, NSTextContentManagerEnumerationOptions) {
    NSTextContentManagerEnumerationOptionsNone = 0,
    NSTextContentManagerEnumerationOptionsReverse = (1 << 0)
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

// NSTextElementProvider is a protocol conformed by NSTextContentManager and its concrete subclasses. It defines the base interface for interacting with a custom text document content type.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@protocol NSTextElementProvider <NSObject>
// Declares the starting and ending locations for the document. The subclass could use its own implementation of a location object conforming to NSTextRange.
@property (strong, readonly) NSTextRange *documentRange;

// Enumerates the text elements starting at textLocation. If textLocation=nil, it assumes self.documentRange.location for forward enumeration; uses self.documentRange.endLocation for reverse enumeration. When enumerating backward, it starts with the element preceding the one containing textLocation. If enumerated at least one element, it returns the edge of the enumerated range. Note that the enumerated range might not match the range of the last element returned. It enumerates the elements in the sequence, but it can skip a range (it can limit the maximum number of text elements enumerated for a single invocation or hide some elements from the layout). Returning NO from block breaks out of the enumeration.
- (nullable id <NSTextLocation>)enumerateTextElementsFromLocation:(nullable id <NSTextLocation>)textLocation options:(NSTextContentManagerEnumerationOptions)options usingBlock:(BOOL (NS_NOESCAPE ^)(NSTextElement *element))block;

// Replaces the characters specified by range with textElements. If the edges of range is not at existing element range boundaries, it either split the element if it allows the operation (i.e. NSTextParagraph) or adjust the replacement range. This method is intended to be used only by NSTextLayoutManager.
- (void)replaceContentsInRange:(NSTextRange *)range withTextElements:(nullable NSArray<NSTextElement *> *)textElements;

//  Synchronizes changes to the backing store. If completionHandler=nil, performs the operation synchronously. completionHandler gets passed error if failed. It should block (or fail if synchronous) when there is an active transaction.
- (void)synchronizeToBackingStore:(nullable void (^)(NSError * _Nullable error))completionHandler;

@optional
// Returns a new location from location with offset. The offset value could be positive or negative indicating the logical direction. Could return nil when the inputs don't produce any legal location (i.e. out of bounds index).
- (nullable id <NSTextLocation>)locationFromLocation:(id <NSTextLocation>)location withOffset:(NSInteger)offset;

// Returns the offset between from and to. The return value could be positive or negative. Could return NSNotFound when the offset cannot be represented in an integer value (i.e. locations are not in the same document).
- (NSInteger)offsetFromLocation:(id <NSTextLocation>)from toLocation:(id <NSTextLocation>)to;

// Should be implemented if the location backing store requires manual adjustment after editing. When textRange is intersecting or following the current edited range, the method returns the range adjusted for the modification in the editing session. Returns nil, when no adjustment necessary. forEditingTextSelection indicates if textRange is for the text selection associated with the edit session.
- (nullable NSTextRange *)adjustedRangeFromRange:(NSTextRange *)textRange forEditingTextSelection:(BOOL)forEditingTextSelection;
@end

#pragma mark NSTextContentManager
// NSTextContentManager is an abstract class defining the interface for managing the text document contents and the default implementation. The concrete subclass overrides NSTextElementProvider for managing the content backing store. It is the root object strongly referencing the rest of objects in the TextKit network via an array of NSTextLayoutManager. Also, it manages the editing transaction by tracking the active NSTextLayoutManager focused to be editing.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextContentManager : NSObject <NSTextElementProvider, NSSecureCoding>
#pragma mark Initialization
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

#pragma mark Basic properties
@property (weak, nullable) id <NSTextContentManagerDelegate> delegate;

#pragma mark NSTextLayoutManager management
// An array of NSTextLayoutManager. KVO-compliant
@property (copy, readonly) NSArray<NSTextLayoutManager *> *textLayoutManagers;

- (void)addTextLayoutManager:(NSTextLayoutManager *)textLayoutManager;
- (void)removeTextLayoutManager:(NSTextLayoutManager *)textLayoutManager;

// The primary NSTextLayoutManager interacting with the user allowing to edit. Setting an NSTextLayoutManager not in textLayoutManagers will reset to nil. It automatically synchronizes pending edits before switching to a new primary object. The operation is synchronous. KVO-compliant
@property (nullable, strong) NSTextLayoutManager *primaryTextLayoutManager;

//  Synchronizes changes to all non-primary text layout managers. If completionHandler=nil, performs the operation synchronously. completionHandler gets passed error if failed. It should block (or fail if synchronous) when there is an active transaction.
- (void)synchronizeTextLayoutManagers:(nullable void (^)(NSError * _Nullable error))completionHandler;

#pragma mark Element access
// Returns an array of NSTextElement intersecting the specified range in sequence. It can return a set of elements not filling the entire range specified if the entire range is not synchronously available. Uses -enumerateTextElementsFromLocation:options:usingBlocK: to fill the array.
- (NSArray<NSTextElement *> *)textElementsForRange:(NSTextRange *)range;

#pragma mark Transaction
// When YES, there is an active editing transaction from primaryTextLayoutManager. the synchronization operations to non-primary text layout managers and the backing store block (or fails when synchronous) while this property is YES. Non-primary text layout managers should avoid accessing the elements while this is YES. KVO-compliant
@property (readonly) BOOL hasEditingTransaction;

// Invoked by primaryTextLayoutManager controlling the active editing transaction. Can be nested. The outer most transaction toggles hasEditingTransaction and sends synchronization messages if enabled after invoking transaction.
- (void)performEditingTransactionUsingBlock:(void (NS_NOESCAPE ^) (void))transaction NS_SWIFT_DISABLE_ASYNC;

// Records information about an edit action to the transaction. originalTextRange is the range edited before the action, and newTextRange is the corresponding range after the action. The concrete subclass should invoke this method for each edit action.
- (void)recordEditActionInRange:(NSTextRange *)originalTextRange newTextRange:(NSTextRange *)newTextRange;

// Automatically synchronizes all text layout managers when hasEditingTransaction becoming NO. YES by default.
@property BOOL automaticallySynchronizesTextLayoutManagers;

// Automatically synchronizes the backing store when hasEditingTransaction becoming NO. NO by default.
@property BOOL automaticallySynchronizesToBackingStore;
@end

#pragma mark NSTextContentManagerDelegate
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@protocol NSTextContentManagerDelegate <NSObject>
@optional
// Returns a custom element for location. When non-nil, textContentManager uses the element instead of creating based on its standard mapping logic.
- (nullable NSTextElement *)textContentManager:(NSTextContentManager *)textContentManager textElementAtLocation:(id <NSTextLocation>)location;

// Gives delegate a chance to validate textElement right before block() invocation with -enumerateTextElementsFromLocation:options:usingBlock:. Returning NO indicates textElement to be skipped from the enumeration.
- (BOOL)textContentManager:(NSTextContentManager *)textContentManager shouldEnumerateTextElement:(NSTextElement *)textElement options:(NSTextContentManagerEnumerationOptions)options;
@end

#pragma mark NSTextContentStorageDelegate
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@protocol NSTextContentStorageDelegate <NSTextContentManagerDelegate>
@optional
// Returns a custom NSTextParagraph for range in NSTextContentStorage.attributedString. When non-nil, textContentStorage uses the text paragraph instead of creating the standard NSTextParagraph with the attributed substring in range. The attributed string for a custom text paragraph must have range.length.
- (nullable NSTextParagraph *)textContentStorage:(NSTextContentStorage *)textContentStorage textParagraphWithRange:(NSRange)range;
@end

#pragma mark NSTextContentStorage
// NSTextContentStorage is a concrete subclass of NSTextContentManager providing support for NSAttributedString backing-store. It also implements NSTextStorageObserving participating as a client of NSTextStorage. The facility only supports a single NSTextContentStorage associated with a text storage. When -textStorage!=nil, -attributedString is ignored. By default, NSTextContentStorage is initialized with NSTextStorage as the backing-store.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextContentStorage : NSTextContentManager <NSTextStorageObserving>
#pragma mark Basic properties
@property (weak, nullable) id <NSTextContentStorageDelegate> delegate;

#pragma mark Document contents
// The document contents. KVO-compliant
@property (nullable, copy) NSAttributedString *attributedString;

#pragma mark NSAttributedString utils
// Returns a new attributed string for textElement. Returns if textElement cannot be mapped to NSAttributedString.
- (nullable NSAttributedString *)attributedStringForTextElement:(NSTextElement *)textElement;

// Returns NSTextElement corresponding to attributedString. Returns nil when attributedString contains attributes not mappable to NSTextElement.
- (nullable NSTextElement *)textElementForAttributedString:(NSAttributedString *)attributedString;

#pragma mark Optional NSTextElementProvider methods
// Returns a new location from location with offset. The offset value could be positive or negative indicating the logical direction. Could return nil when the inputs don't produce any legal location (i.e. out of bounds index).
- (nullable id <NSTextLocation>)locationFromLocation:(id <NSTextLocation>)location withOffset:(NSInteger)offset;

// Returns the offset between from and to. The return value could be positive or negative. Could return NSNotFound when the offset cannot be represented in an integer value (i.e. locations are not in the same document).
- (NSInteger)offsetFromLocation:(id <NSTextLocation>)from toLocation:(id <NSTextLocation>)to;

// Should be implemented if the location backing store requires manual adjustment after editing. When textRange is intersecting or following the current edited range, the method returns the range adjusted for the modification in the editing session. Returns nil, when no adjustment necessary. forEditingTextSelection indicates if textRange is for the text selection associated with the edit session.
- (nullable NSTextRange *)adjustedRangeFromRange:(NSTextRange *)textRange forEditingTextSelection:(BOOL)forEditingTextSelection;
@end

// Posted by NSTextContentStorage when a text attribute unsupported by NSTextContentStorage is added to the underlying text storage.
APPKIT_EXTERN NSNotificationName NSTextContentStorageUnsupportedAttributeAddedNotification API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextContentManager.h>
#endif
