/*
        NSTextView.h
        Application Kit
        Copyright (c) 1994-2021, Apple Inc.
        All rights reserved.
*/

// NSTextView is an NSText subclass that displays the glyphs laid out in one NSTextContainer.

#import <AppKit/NSText.h>
#import <AppKit/NSInputManager.h>
#import <AppKit/NSTextAttachment.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSDragging.h>
#import <AppKit/NSUserInterfaceValidation.h>
#import <AppKit/NSTextInputClient.h>
#import <AppKit/NSNibDeclarations.h>
#import <AppKit/NSTextFinder.h>
#import <AppKit/NSLayoutManager.h>
#import <AppKit/NSSpellChecker.h>
#import <AppKit/NSPasteboard.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSTextCheckingResult.h>
#import <AppKit/NSTouchBarItem.h>
#import <AppKit/NSCandidateListTouchBarItem.h>
#import <AppKit/NSColorPanel.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSTextContent.h>

@protocol NSTextViewDelegate;
@protocol NSTextLayoutOrientationProvider;
@protocol NSTextAttachmentCell;
@protocol QLPreviewItem;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSTextContainer;
@class NSTextStorage;
@class NSLayoutManager;
@class NSTextContentStorage;
@class NSTextLayoutManager;
@class NSRulerView;
@class NSRulerMarker;
@class NSUndoManager;
@class NSParagraphStyle;
@class NSOrthography;
@class NSSharingServicePicker;
@class NSValue;
@class NSTextAttachment;

/* Values for NSSelectionGranularity */
typedef NS_ENUM(NSUInteger, NSSelectionGranularity) {
    NSSelectByCharacter = 0,
    NSSelectByWord = 1,
    NSSelectByParagraph = 2
};

/* Values for NSSelectionAffinity */
typedef NS_ENUM(NSUInteger, NSSelectionAffinity) {
    NSSelectionAffinityUpstream = 0,
    NSSelectionAffinityDownstream = 1
};


/* A meta locale identifier representing the set of Roman input sources available.  You can specify [NSArray arrayWithObject: NSAllRomanInputSourcesLocaleIdentifier] to restrict allowed input sources to Roman only.
*/
APPKIT_EXTERN NSString * NSAllRomanInputSourcesLocaleIdentifier API_AVAILABLE(macos(10.5));

#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_12
NS_AUTOMATED_REFCOUNT_WEAK_UNAVAILABLE
#endif
@interface NSTextView : NSText <NSColorChanging, NSMenuItemValidation, NSUserInterfaceValidations, NSTextInputClient, NSTextLayoutOrientationProvider, NSDraggingSource, NSStandardKeyBindingResponding, NSTextInput, NSAccessibilityNavigableStaticText, NSTextContent>

/**************************** Initializing ****************************/

// Designated Initializer. container may be nil.
- (instancetype)initWithFrame:(NSRect)frameRect textContainer:(nullable NSTextContainer *)container NS_DESIGNATED_INITIALIZER;

- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

// This variant will create the text network (textStorage, layoutManager, and a container).
- (instancetype)initWithFrame:(NSRect)frameRect;

/***************** Get/Set the container and other stuff *****************/

// The set method should not be called directly, but you might want to override it.  Gets or sets the text container for this view.  Setting the text container marks the view as needing display.  The text container calls the set method from its setTextView: method.
@property (nullable, assign) NSTextContainer *textContainer;

// This method should be used instead of the primitive -setTextContainer: if you need to replace a view's text container with a new one leaving the rest of the web intact.  This method deals with all the work of making sure the view doesn't get deallocated and removing the old container from the layoutManager and replacing it with the new one.
- (void)replaceTextContainer:(NSTextContainer *)newContainer;

// The textContainerInset determines the padding that the view provides around the container.  The container's origin will be inset by this amount from the bounds point {0,0} and padding will be left to the right and below the container of the same amount.  This inset affects the view sizing in response to new layout and is used by the rectangular text containers when they track the view's frame dimensions.
@property NSSize textContainerInset;

// The container's origin in the view is determined from the current usage of the container, the container inset, and the view size.  textContainerOrigin returns this point.
@property (readonly) NSPoint textContainerOrigin;

// invalidateTextContainerOrigin is sent automatically whenever something changes that causes the origin to possibly move.  You usually do not need to call invalidate yourself.
- (void)invalidateTextContainerOrigin;

@property (nullable, readonly, assign) NSLayoutManager *layoutManager;

@property (nullable, readonly, assign) NSTextStorage *textStorage;

// The text views's text layout manager, if its text container is configured with one; null otherwise.
@property (nullable, readonly, weak) NSTextLayoutManager *textLayoutManager API_AVAILABLE(macos(12.0));

// The text view's text layout storage, if its text container is configured with one; null otherwise.
@property (nullable, readonly, weak) NSTextContentStorage *textContentStorage API_AVAILABLE(macos(12.0));

/************************* Key binding entry-point *************************/

- (void)insertText:(id)insertString API_DEPRECATED("Use -insertText:replacementRange: from NSTextInputClient instead. Since the method is designed to be used solely by the input system, the message should never be sent to a text view from applications. Any content modifications should be via either NSTextStorage or NSText methods.", macos(10.0,10.11));

/*************************** Sizing methods ***************************/

// Sets the frame size of the view to desiredSize constrained within min and max size.
- (void)setConstrainedFrameSize:(NSSize)desiredSize;

/***************** New miscellaneous API above and beyond NSText *****************/

// These two complete the set of range: type set methods. to be equivalent to the set of non-range taking varieties.
#if !TARGET_OS_IPHONE
- (void)setAlignment:(NSTextAlignment)alignment range:(NSRange)range;
- (void)setBaseWritingDirection:(NSWritingDirection)writingDirection range:(NSRange)range;
#endif

/*************************** New Font menu commands ***************************/

- (void)turnOffKerning:(nullable id)sender;
- (void)tightenKerning:(nullable id)sender;
- (void)loosenKerning:(nullable id)sender;
- (void)useStandardKerning:(nullable id)sender;
- (void)turnOffLigatures:(nullable id)sender;
- (void)useStandardLigatures:(nullable id)sender;
- (void)useAllLigatures:(nullable id)sender;
- (void)raiseBaseline:(nullable id)sender;
- (void)lowerBaseline:(nullable id)sender;
- (void)toggleTraditionalCharacterShape:(nullable id)sender API_DEPRECATED("Use the traditional shaped characters encoded in the Unicode standard. Access the characters via the character palette.", macos(10.0,10.11));
- (void)outline:(nullable id)sender;

/*************************** Find menu commands ***************************/

// See NSFindPanelAction for possible tags in sender
- (void)performFindPanelAction:(nullable id)sender;

/*************************** New Text commands ***************************/

- (void)alignJustified:(nullable id)sender;
- (void)changeColor:(nullable id)sender;
- (void)changeAttributes:(nullable id)sender;
- (void)changeDocumentBackgroundColor:(nullable id)sender;
- (void)orderFrontSpacingPanel:(nullable id)sender;
- (void)orderFrontLinkPanel:(nullable id)sender;
- (void)orderFrontListPanel:(nullable id)sender;
- (void)orderFrontTablePanel:(nullable id)sender;

/*************************** Ruler support ***************************/

- (void)rulerView:(NSRulerView *)ruler didMoveMarker:(NSRulerMarker *)marker;
- (void)rulerView:(NSRulerView *)ruler didRemoveMarker:(NSRulerMarker *)marker;
- (void)rulerView:(NSRulerView *)ruler didAddMarker:(NSRulerMarker *)marker;
- (BOOL)rulerView:(NSRulerView *)ruler shouldMoveMarker:(NSRulerMarker *)marker;
- (BOOL)rulerView:(NSRulerView *)ruler shouldAddMarker:(NSRulerMarker *)marker;
- (CGFloat)rulerView:(NSRulerView *)ruler willMoveMarker:(NSRulerMarker *)marker toLocation:(CGFloat)location;
- (BOOL)rulerView:(NSRulerView *)ruler shouldRemoveMarker:(NSRulerMarker *)marker;
- (CGFloat)rulerView:(NSRulerView *)ruler willAddMarker:(NSRulerMarker *)marker atLocation:(CGFloat)location;
- (void)rulerView:(NSRulerView *)ruler handleMouseDown:(NSEvent *)event;

/*************************** Fine display control ***************************/

// If flag is set, then this will attempt to avoid causing layout, if possible, when displaying the rect.  If flag is not set, this is equivalent to setNeedsDisplayInRect:.
- (void)setNeedsDisplayInRect:(NSRect)rect avoidAdditionalLayout:(BOOL)flag;

@property (readonly) BOOL shouldDrawInsertionPoint;
- (void)drawInsertionPointInRect:(NSRect)rect color:(NSColor *)color turnedOn:(BOOL)flag;

// This is the override point for view background drawing.
- (void)drawViewBackgroundInRect:(NSRect)rect;

/*************************** Especially for subclassers ***************************/

- (void)updateRuler;
- (void)updateFontPanel;

- (void)updateDragTypeRegistration;

- (NSRange)selectionRangeForProposedRange:(NSRange)proposedCharRange granularity:(NSSelectionGranularity)granularity;

/*************************** Especially for subclassers ***************************/

// Cause the text view to act as if someone clicked on a piece of text with link as the value of NSLinkAttributeName.  If, for instance, you have a special attachment cell that can end up following links, you can use this method to ask the text view to follow a link once you decide it should.  This method is invoked by the text view during mouse tracking if the user is clicking a link as well.  This sends the textView:clickedOnLink: delegation if the delegate responds.
- (void)clickedOnLink:(id)link atIndex:(NSUInteger)charIndex;

/************************* Speech support *************************/

- (void)startSpeaking:(nullable id)sender;
- (void)stopSpeaking:(nullable id)sender;

/************************* Vertical text support *************************/

#if !TARGET_OS_IPHONE
// Changes the receiver's layout orientation and invalidates the contents.  Unlike other NSTextView properties, this is not shared by sibling views.  It also rotates the bounds 90 degrees, swaps horizontal and vertical bits of the autoresizing mask, and reconfigures isHorizontallyResizable and isVerticallyResizable properties accordingly.  Also, if -enclosingScrollView returns non-nil, it reconfigures horizontal and vertical ruler views, horizontal and vertical scrollers, and the frame.
- (void)setLayoutOrientation:(NSTextLayoutOrientation)orientation API_AVAILABLE(macos(10.7));
#endif

// An action method that calls -setLayoutOrientation: with the sender's tag as the orientation.
- (void)changeLayoutOrientation:(nullable id)sender API_AVAILABLE(macos(10.7));

/************************* Helper for subclassers *************************/

// Here point is in view coordinates, and the return value is a character index appropriate for placing a zero-length selection for an insertion point associated with the mouse at the given point.  The NSTextInput method characterIndexForPoint: is not suitable for this role.
- (NSUInteger)characterIndexForInsertionAtPoint:(NSPoint)point API_AVAILABLE(macos(10.5));

/**************************** Ownership policy ****************************/
// Returns whether instances of the class operate in the object ownership policy introduced with macOS Sierra and later. When YES, the new object owner policy is used. Under the policy, each text view strongly retains its text storage and its text container weakly references the view. Also, the text views are compatible with __weak storage. The default is YES.
@property (readonly, class) BOOL stronglyReferencesTextStorage API_AVAILABLE(macos(10.12));

/*************************** Document Content Access ***************************/
#pragma mark Document Content Mutation
// Replaces the contents at the specified range with attributedString. In addition to invoking -[NSTextStorage replaceCharactersInRange:withAttributedString:], this method ensures that the change is validated with -shouldChangeTextInRange:replacementString:/-didChangeText. Returns YES if the change was validated and performed. Upon replacement, each attribute run in attributedString is complemented by the attributes at range.location.
- (BOOL)performValidatedReplacementInRange:(NSRange)range withAttributedString:(NSAttributedString *)attributedString API_AVAILABLE(macos(10.14));

/*************************** Dark Mode ***************************/
#pragma mark Dark Mode
// When YES, enables the adaptive color mapping mode. In this mode under the dark effective appearance, NSTextView maps all colors with NSColorTypeComponentBased by inverting the brightness whenever they are coming in and out of the model object, NSTextStorage. For example, when rendering, interacting with NSColorPanel and NSFontManager, and converting from/to the pasteboard and external formats, the color values are converted between the model and rendering contexts. Note that the color conversion algorithm compresses the brightness range and, therefore, does not retain the round-trip fidelity between the light and dark appearances. It may not be suitable for rich text authoring, so it is a good idea to provide a command or preference for your users to see and edit their docs without this option, or in light mode.
@property BOOL usesAdaptiveColorMappingForDarkAppearance API_AVAILABLE(macos(10.14));
@end

@interface NSTextView (NSCompletion)

/************************* Completion support *********************/

// Responder method for invoking completion.  May be invoked programmatically if autocompletion is desired.
- (void)complete:(nullable id)sender;

// Usually returns the partial range from the most recent beginning of a word up to the insertion point.  May be overridden by subclassers to alter the range to be completed.  Returning (NSNotFound, 0) suppresses completion.
@property (readonly) NSRange rangeForUserCompletion;

// Returns an array of potential completions, in the order to be presented, representing complete words that the user might be trying to type when starting by typing the partial word at the given range.  May be overridden by subclassers to modify or override this list.  Returning nil or a zero-length array suppresses completion.   The selected item index may optionally be set to indicate which completion should be initially selected; default is 0, and -1 indicates no selection.  This method should call the delegate method textView:completions:forPartialWordRange:indexOfSelectedItem: if implemented.
- (nullable NSArray<NSString *> *)completionsForPartialWordRange:(NSRange)charRange indexOfSelectedItem:(NSInteger *)index;

// Called with final == NO as the user moves through the potential completions, then with final == YES when a completion is definitively selected (or completion is cancelled and the original value is reinserted).  The default implementation inserts the completion into the text at the appropriate location.  The movement argument takes its values from the movement codes defined in NSText.h, and allows subclassers to distinguish between canceling completion and selection by arrow keys, by return, by tab, or by other means such as clicking.
- (void)insertCompletion:(NSString *)word forPartialWordRange:(NSRange)charRange movement:(NSInteger)movement isFinal:(BOOL)flag;

@end

@interface NSTextView (NSPasteboard)

/******************* Pasteboard support (mainly for subclassers) *******************/

// To implement support for writing a new type, override -writeSelectionToPasteboard:type: to support your new type (and call super if you're not going to handle it.)  Then you can override -writablePasteboardTypes and optionally the +registerForServices methods to enable the new type.

// To implement support for reading a new type, override -readSelectionFromPasteboard:type: to support your new type (and call super if you're not going to handle it.)  Then you can override -readablePasteboardTypes and optionally the +registerForServices and acceptableDragTypes methods to enable the new type.  Advanced drag acceptance behavior can be implemented with dragOperationForDraggingInfo:type: and cleanUpAfterDragOperation.

// To implement new mechanisms that cause data to be written to the pasteboard you can call writablePasteboardTypes and writeSelectionToPasteboard:types:.

// To implement new mechanisms that cause data to be read from the pasteboard you can call preferredPasteboardTypeFromArray:restrictedToTypesFromArray: and readSelectionFromPasteboard:type:.

// Returns an array of pasteboard types that can be provided from the current selection.  Overriders should copy the result from super and add their own new types.
@property (readonly, copy) NSArray<NSPasteboardType> *writablePasteboardTypes;

// Invoked automatically to write a single type to the pasteboard.  The type will already have been declared to the pasteboard so this should merely write the data using the appropriate set method on the pasteboard.
- (BOOL)writeSelectionToPasteboard:(NSPasteboard *)pboard type:(NSPasteboardType)type;

// Declares all the types to the pasteboard then calls writeSelectionToPasteboard:type: for each type in the array.
- (BOOL)writeSelectionToPasteboard:(NSPasteboard *)pboard types:(NSArray<NSPasteboardType> *)types;

// Returns an array of types that could be read currently in order of preference.  Subclassers should take care to consider the "preferred" part of the semantics of this method.  Figure out where your new type should fit into the preferred order.  The preferred order should usually be from the richest types down to the less rich.  The default array will start with RTFD, RTF, strings, files, images, colors and so on.  The ordering list really starts to lose significance after the first few elements.  If the new format you are supporting is richer than RTFD, put it at the head of the list, otherwise try to find the right place for it, but don't count on the actual contents of the list you get from super either.
@property (readonly, copy) NSArray<NSPasteboardType> *readablePasteboardTypes;

// Returns the most preferred type from the available types array that it is currently possible to read.  If allowedTypes is provided then only those types will be considered regardless of whether others could currently be read.  You should not have to override this to support new types.
- (nullable NSPasteboardType)preferredPasteboardTypeFromArray:(NSArray<NSPasteboardType> *)availableTypes restrictedToTypesFromArray:(nullable NSArray<NSPasteboardType> *)allowedTypes;

// Invoked automatically to read a specific type from the pasteboard.  The type will already have been by the preferredPasteboardTypeFromArray:restrictedToTypesFromArray: method so this should merely read the data using the appropriate accessor method on the pasteboard.
- (BOOL)readSelectionFromPasteboard:(NSPasteboard *)pboard type:(NSPasteboardType)type;

// Part of the services mechanism.  This is implemented such that you should not need to override it to support new types.  It should not be necessary to call this.
- (BOOL)readSelectionFromPasteboard:(NSPasteboard *)pboard;

// Automatically invoked when the first instance of NSTextView is created.  Subclassing this is necessary to add support for new service types (in addition to actually supporting writing or reading the types, of course).  Override it to call super and then register your own new types.  You should probably never need to call this except to message super in an override.
+ (void)registerForServices;

// This method is part of the services protocol and is implemented in such a way that if you extend the type support correctly you should not need to override it.  You should never need to call it either.
- (nullable id)validRequestorForSendType:(nullable NSPasteboardType)sendType returnType:(nullable NSPasteboardType)returnType;

// These methods are like paste: (from NSResponder) but they restrict the acceptable type of the pasted data.  They are suitable as menu actions for appropriate "Paste As" submenu commands.
- (void)pasteAsPlainText:(nullable id)sender;
- (void)pasteAsRichText:(nullable id)sender;

@end

@interface NSTextView (NSDragging)

// Causes textview to begin dragging current selected range, returning YES if it succeeds in initiating the drag.  Primarily for subclassers, who can override it to intervene at beginning of a drag.
- (BOOL)dragSelectionWithEvent:(NSEvent *)event offset:(NSSize)mouseOffset slideBack:(BOOL)slideBack;

// Used by dragSelectionWithEvent:offset:slideBack: to get an appropriate image.  Returns the lower-left point of the image in view coordinates as origin.  Can be called by others who need such an image, or can be overridden by subclassers to return a different image (if it returns nil, a default icon will be used).
- (nullable NSImage *)dragImageForSelectionWithEvent:(NSEvent *)event origin:(nullable NSPointPointer)origin;

// Must be overridden to support new drag types in addition to adding support for reading and writing the type.  Override it to call super, then make a copy of the result and add your own new types before returning the copy.  You should probably never need to call this except to message super in an override.
@property (readonly, copy) NSArray<NSPasteboardType> *acceptableDragTypes;

// This is called by draggingEntered:... and draggingUpdated:...  It should return the drag operation constant appropriate to the current situation or NSDragOperationNone.  It can also do any other auxiliary stuff like drawing a ghost to indicate where the dragged thing will go when it is dropped.  Be aware that this is called over and over so if you're going to draw a ghost or something avoid doing it over and over unless you need to.  Any state you set up in this method that needs to be cleared can be cleared by overriding the next method.  You should probably never need to call this except to message super in an override.  Clients should override this method rather than draggingEntered:... and draggingUpdated:... to control the drag operation.  Clients overriding this method must message super to get the standard drag insertion indicator.
- (NSDragOperation)dragOperationForDraggingInfo:(id <NSDraggingInfo>)dragInfo type:(NSPasteboardType)type;

// If you set up persistent state that should go away when the drag operation finishes, you can clean it up here.  Such state is usually set up in -dragOperationForDraggingInfo:type:.  You should probably never need to call this except to message super in an override.
- (void)cleanUpAfterDragOperation;

@end

// The methods in this category deal with settings that need to be shared by all the NSTextViews of a single NSLayoutManager.  Many of these methods are overrides of NSText or NSResponder methods.
@interface NSTextView (NSSharing)

/*************************** Selected/Marked range ***************************/

@property (copy) NSArray<NSValue *> *selectedRanges;

// These multiple-range methods supersede the corresponding single-range methods.  The ranges argument must be a non-nil, non-empty array of objects responding to rangeValue.  The return value of selectedRanges obeys the same restrictions, and in addition its elements are sorted, non-overlapping, non-contiguous, and (except for the case of a single range) have non-zero-length.
- (void)setSelectedRanges:(NSArray<NSValue *> *)ranges affinity:(NSSelectionAffinity)affinity stillSelecting:(BOOL)stillSelectingFlag;

- (void)setSelectedRange:(NSRange)charRange affinity:(NSSelectionAffinity)affinity stillSelecting:(BOOL)stillSelectingFlag;
@property (readonly) NSSelectionAffinity selectionAffinity;
@property NSSelectionGranularity selectionGranularity;

// Selected text attributes are applied as temporary attributes to selected text.  Candidates include those attributes that do not affect layout.
@property (copy) NSDictionary<NSAttributedStringKey, id> *selectedTextAttributes;

@property (copy) NSColor *insertionPointColor;

- (void)updateInsertionPointStateAndRestartTimer:(BOOL)restartFlag;

// Marked text attributes are applied as temporary attributes to selected text.  Candidates include those attributes that do not affect layout.
@property (nullable, copy) NSDictionary<NSAttributedStringKey, id> *markedTextAttributes;

// Link text attributes are applied as temporary attributes to any text with a link attribute.  Candidates include those attributes that do not affect layout.  Default attributes are blue color, single underline, and the pointing hand cursor.
@property (nullable, copy) NSDictionary<NSAttributedStringKey, id> *linkTextAttributes;

// If set, then text with a link attribute will automatically be treated as if it had an implicit tooltip attribute with the same value as the link attribute.  An explicit tooltip attribute will take precedence over this implicit one.  The textView:willDisplayToolTip:forCharacterAtIndex: delegate method affects these tooltips as it does any other.
@property BOOL displaysLinkToolTips API_AVAILABLE(macos(10.5));

/************************* Glyph info support *************************/

@property BOOL acceptsGlyphInfo;

/*************************** Other NSTextView methods ***************************/

@property BOOL usesRuler;

@property BOOL usesInspectorBar API_AVAILABLE(macos(10.7));

@property (getter=isContinuousSpellCheckingEnabled) BOOL continuousSpellCheckingEnabled;
- (void)toggleContinuousSpellChecking:(nullable id)sender;

@property (readonly) NSInteger spellCheckerDocumentTag;

// If grammar checking is enabled, then it is performed whenever spellchecking is performed, whether continuously or manually.
@property (getter=isGrammarCheckingEnabled) BOOL grammarCheckingEnabled API_AVAILABLE(macos(10.5));
- (void)toggleGrammarChecking:(nullable id)sender API_AVAILABLE(macos(10.5));

// May be called or overridden to control setting of spelling and grammar indicators.  Values are those listed for NSSpellingStateAttributeName.  Calls the delegate method textView:shouldSetSpellingState:range:.
- (void)setSpellingState:(NSInteger)value range:(NSRange)charRange API_AVAILABLE(macos(10.5));

@property (copy) NSDictionary<NSAttributedStringKey, id> *typingAttributes;

// These multiple-range methods supersede the corresponding single-range methods.  For the first method, the affectedRanges argument obeys the same restrictions as the argument to setSelectedRanges:, and the replacementStrings array should either be nil (for attribute-only changes) or have the same number of elements as affectedRanges.  For the remaining three methods, the return values obey the same restrictions as that for selectedRanges, except that they will be nil if the corresponding change is not permitted, where the corresponding single-range methods return (NSNotFound, 0).
- (BOOL)shouldChangeTextInRanges:(NSArray<NSValue *> *)affectedRanges replacementStrings:(nullable NSArray<NSString *> *)replacementStrings;
@property (nullable, readonly, copy) NSArray<NSValue *> *rangesForUserTextChange;
@property (nullable, readonly, copy) NSArray<NSValue *> *rangesForUserCharacterAttributeChange;
@property (nullable, readonly, copy) NSArray<NSValue *> *rangesForUserParagraphAttributeChange;

- (BOOL)shouldChangeTextInRange:(NSRange)affectedCharRange replacementString:(nullable NSString *)replacementString;
- (void)didChangeText;

@property (readonly) NSRange rangeForUserTextChange;
@property (readonly) NSRange rangeForUserCharacterAttributeChange;
@property (readonly) NSRange rangeForUserParagraphAttributeChange;

@property BOOL allowsDocumentBackgroundColorChange;

@property (nullable, copy) NSParagraphStyle *defaultParagraphStyle;

@property BOOL allowsUndo;

// May be called to introduce a break in the coalescing of undo actions for user typing, for example at a save point.
- (void)breakUndoCoalescing;

@property (getter=isCoalescingUndo, readonly) BOOL coalescingUndo API_AVAILABLE(macos(10.6));

// Specifies whether image attachments should permit editing of their images, if the text view is editable and the text attachment cell supports image editing.
@property BOOL allowsImageEditing API_AVAILABLE(macos(10.5));

// Applies a temporary highlighting effect, intended to indicate the result of a find operation.  Clients should perform any necessary scrolling before calling this method.  The effect will be removed after a certain time, or when other actions (such as scrolling) take place, but it can be removed immediately by calling this method again with a zero-length range.
- (void)showFindIndicatorForRange:(NSRange)charRange API_AVAILABLE(macos(10.5));

// Controls whether to show rollover button for extension service items inside text selection.  It's enabled by default.
@property BOOL usesRolloverButtonForSelection API_AVAILABLE(macos(10.10));


/*************************** NSText methods ***************************/

@property (nullable, weak) id<NSTextViewDelegate> delegate;
@property (getter=isEditable) BOOL editable;
@property (getter=isSelectable) BOOL selectable;
@property (getter=isRichText) BOOL richText;
@property BOOL importsGraphics;
@property BOOL drawsBackground;
@property (copy) NSColor *backgroundColor;
@property (getter=isFieldEditor) BOOL fieldEditor;
@property BOOL usesFontPanel;
@property (getter=isRulerVisible) BOOL rulerVisible;
- (void)setSelectedRange:(NSRange)charRange;
    // Other NSText methods are implemented in the base NSTextView implementation rather than in this category.  See NSText.h for declarations.

/*************************** Input Source support ***************************/
/* Returns an array of locale identifiers representing keyboard input sources allowed to be enabled when the receiver has the keyboard focus.
 */
@property (nullable, copy) NSArray<NSString *> *allowedInputSourceLocales API_AVAILABLE(macos(10.5));
@end

@interface NSTextView (NSTextChecking)

/*************************** Smart copy/paste/delete/substitution support ***************************/

@property BOOL smartInsertDeleteEnabled;
- (NSRange)smartDeleteRangeForProposedRange:(NSRange)proposedCharRange;
- (void)toggleSmartInsertDelete:(nullable id)sender;

- (void)smartInsertForString:(NSString *)pasteString replacingRange:(NSRange)charRangeToReplace beforeString:(NSString * _Nullable * _Nullable)beforeString afterString:(NSString * _Nullable * _Nullable)afterString;
- (nullable NSString *)smartInsertBeforeStringForString:(NSString *)pasteString replacingRange:(NSRange)charRangeToReplace;
- (nullable NSString *)smartInsertAfterStringForString:(NSString *)pasteString replacingRange:(NSRange)charRangeToReplace;

@property (getter=isAutomaticQuoteSubstitutionEnabled) BOOL automaticQuoteSubstitutionEnabled API_AVAILABLE(macos(10.5));
- (void)toggleAutomaticQuoteSubstitution:(nullable id)sender API_AVAILABLE(macos(10.5));
@property (getter=isAutomaticLinkDetectionEnabled) BOOL automaticLinkDetectionEnabled API_AVAILABLE(macos(10.5));
- (void)toggleAutomaticLinkDetection:(nullable id)sender API_AVAILABLE(macos(10.5));

@property (getter=isAutomaticDataDetectionEnabled) BOOL automaticDataDetectionEnabled API_AVAILABLE(macos(10.6));
- (void)toggleAutomaticDataDetection:(nullable id)sender API_AVAILABLE(macos(10.6));
@property (getter=isAutomaticDashSubstitutionEnabled) BOOL automaticDashSubstitutionEnabled API_AVAILABLE(macos(10.6));
- (void)toggleAutomaticDashSubstitution:(nullable id)sender API_AVAILABLE(macos(10.6));
@property (getter=isAutomaticTextReplacementEnabled) BOOL automaticTextReplacementEnabled API_AVAILABLE(macos(10.6));
- (void)toggleAutomaticTextReplacement:(nullable id)sender API_AVAILABLE(macos(10.6));
@property (getter=isAutomaticSpellingCorrectionEnabled) BOOL automaticSpellingCorrectionEnabled API_AVAILABLE(macos(10.6));
- (void)toggleAutomaticSpellingCorrection:(nullable id)sender API_AVAILABLE(macos(10.6));

// These two are bulk methods for setting and getting many checking type settings at once.  They will call the individual methods as necessary.
@property NSTextCheckingTypes enabledTextCheckingTypes API_AVAILABLE(macos(10.6));

// These two methods usually would not be called directly, since NSTextView itself will call them as needed, but they can be overridden.
- (void)checkTextInRange:(NSRange)range types:(NSTextCheckingTypes)checkingTypes options:(NSDictionary<NSTextCheckingOptionKey, id> *)options API_AVAILABLE(macos(10.6));
- (void)handleTextCheckingResults:(NSArray<NSTextCheckingResult *> *)results forRange:(NSRange)range types:(NSTextCheckingTypes)checkingTypes options:(NSDictionary<NSTextCheckingOptionKey, id> *)options orthography:(NSOrthography *)orthography wordCount:(NSInteger)wordCount API_AVAILABLE(macos(10.6));

- (void)orderFrontSubstitutionsPanel:(nullable id)sender API_AVAILABLE(macos(10.6));

// Ordinarily text checking will occur in the background, and results that replace text will be applied only for text that has been typed in by the user, but these last two methods cause the currently enabled text checking types to be applied immediately to the selection or the document, respectively, with results that replace text applied to all text whatever its origin.
- (void)checkTextInSelection:(nullable id)sender API_AVAILABLE(macos(10.6));
- (void)checkTextInDocument:(nullable id)sender API_AVAILABLE(macos(10.6));

@property BOOL usesFindPanel;

@property BOOL usesFindBar API_AVAILABLE(macos(10.7));

@property (getter=isIncrementalSearchingEnabled) BOOL incrementalSearchingEnabled API_AVAILABLE(macos(10.7));

@end

@interface NSTextView (NSQuickLookPreview)

/*************************** Quick Look support ***************************/
// This action message toggles the visibility state of the Quick Look preview panel if the receiver is the current Quick Look controller.
- (IBAction)toggleQuickLookPreviewPanel:(nullable id)sender API_AVAILABLE(macos(10.7));

// Returns an array of preview items within the specified character ranges.  Each preview item conforms to the QLPreviewItem protocol.  The NSTextView implementation returns an array of NSURL objects, each url referring to the document URL of a text attachment content if available.
- (NSArray<id<QLPreviewItem>> *)quickLookPreviewableItemsInRanges:(NSArray<NSValue *> *)ranges API_AVAILABLE(macos(10.7));

// Notifies QLPreviewPanel for possible status changes with the data source or controller.  Typically invoked from selection changes.
- (void)updateQuickLookPreviewPanel API_AVAILABLE(macos(10.7));

@end

@interface NSTextView (NSTextView_SharingService)
/*************************** NSSharingService support ***************************/

// Creates a new instance of NSSharingServicePicker based on the current selection & shows to the screen. The items passed to the NSSharingServicePicker initializer are determined using -itemsForSharingServiceInRanges:. When the current selection is 0 length, the whole document is passed to the method.
- (IBAction)orderFrontSharingServicePicker:(nullable id)sender API_AVAILABLE(macos(10.8));

@end


#pragma mark NSTouchBar support
/* NSTextView provides support for text formatting and inputting NSTouchBarItems. -[NSTextView makeTouchBar] instantiates an NSTouchBar configured based on settings such as -automaticTextCompletionEnabled and -richText. NSTextView conforms to NSTouchBarDelegate and supplies NSTouchBarItems via -touchBar:makeItemForIdentifier:.
 */
@interface NSTextView (NSTextView_TouchBar) <NSCandidateListTouchBarItemDelegate, NSTouchBarDelegate>

// Enables the automatic completion NSTouchBarItem. YES by default. When YES, NSTextView displays the candidates for the text selection in its NSCandidateListTouchBarItem returned from -candidateListTouchBarItem. Invokes -updateTouchBarItemIdentifiers.
@property (getter=isAutomaticTextCompletionEnabled) BOOL automaticTextCompletionEnabled API_AVAILABLE(macos(10.12.2));
- (IBAction)toggleAutomaticTextCompletion:(nullable id)sender API_AVAILABLE(macos(10.12.2));

// When Yes, NSTouchBarItemIdentifierCharacterPicker is included in -[NSTouchBar itemIdentifiers] for the receiver's NSTouchBar. Default is YES. Invokes -updateTouchBarItemIdentifiers.
@property BOOL allowsCharacterPickerTouchBarItem API_AVAILABLE(macos(10.12.2));

// This message should be sent whenever a property affecting NSTouchBarItem states is changed. It updates -itemIdentifiers for the receiver's NSTouchBar.
- (void)updateTouchBarItemIdentifiers API_AVAILABLE(macos(10.12.2));

// Updates state of text formatting NSTouchBarItems such as NSTouchBarItemIdentifierTextStyle and NSTouchBarItemIdentifierTextAlignment for the receiver based on the current selection.
- (void)updateTextTouchBarItems API_AVAILABLE(macos(10.12.2));

// Updates the candidates for -candidateListTouchBarItem.
- (void)updateCandidates API_AVAILABLE(macos(10.12.2));

// -[NSTextView candidateListTouchBarItem] returns an NSCandidateTouchBarItem instance owned by the receiver. The NSTouchBarItem is instantiated in -[NSTextView touchBar:makeItemForIdentifier:] with NSTouchBarItemIdentifierCandidateList.
@property (nullable, readonly, strong) NSCandidateListTouchBarItem *candidateListTouchBarItem API_AVAILABLE(macos(10.12.2));
@end

#pragma mark NSTextView Factory Methods
@interface NSTextView (NSTextView_Factory)
// Instantiates a new text view enclosed in a scroll view. As with -[NSTextView init*] methods, the objects created with this factory method is configured suitable for UI elements typically used in inspectors. Access the text view via -[NSScrollView documentView].
+ (NSScrollView *)scrollableTextView API_AVAILABLE(macos(10.14));

// Instantiates a new text view configured as a field editor.
+ (instancetype)fieldEditor API_AVAILABLE(macos(10.14));

// Instantiates a new text view configured for displaying the document contents enclosed in a scroll view. Access the text view via -[NSScrollView documentView].
+ (NSScrollView *)scrollableDocumentContentTextView API_AVAILABLE(macos(10.14));
+ (NSScrollView *)scrollablePlainDocumentContentTextView API_AVAILABLE(macos(10.14));
@end

@interface NSTextView (NSDeprecated)

// toggleBaseWritingDirection: will be deprecated in favor of the new NSResponder methods makeBaseWritingDirectionNatural:, makeBaseWritingDirectionLeftToRight:, and makeBaseWritingDirectionRightToLeft:, which NSTextView now implements.
- (void)toggleBaseWritingDirection:(nullable id)sender API_DEPRECATED("Use NSResponder's makeBaseWritingDirectionNatural:, makeBaseWritingDirectionLeftToRight:, and makeBaseWritingDirectionRightToLeft: instead", macos(10.3,10.6));

@end

// Note that all delegation messages come from the first textView

@protocol NSTextViewDelegate <NSTextDelegate>
@optional

// Delegate only.
- (BOOL)textView:(NSTextView *)textView clickedOnLink:(id)link atIndex:(NSUInteger)charIndex;

// Delegate only.
- (void)textView:(NSTextView *)textView clickedOnCell:(id <NSTextAttachmentCell>)cell inRect:(NSRect)cellFrame atIndex:(NSUInteger)charIndex;

// Delegate only.
- (void)textView:(NSTextView *)textView doubleClickedOnCell:(id <NSTextAttachmentCell>)cell inRect:(NSRect)cellFrame atIndex:(NSUInteger)charIndex;

// Delegate only.  Allows the delegate to take over attachment dragging altogether.
- (void)textView:(NSTextView *)view draggedCell:(id <NSTextAttachmentCell>)cell inRect:(NSRect)rect event:(NSEvent *)event atIndex:(NSUInteger)charIndex;

// Delegate only.  If the previous method is not used, this method and the next allow the textview to take care of attachment dragging and pasting, with the delegate responsible only for writing the attachment to the pasteboard.  In this method, the delegate should return an array of types that it can write to the pasteboard for the given attachment.
- (NSArray<NSPasteboardType> *)textView:(NSTextView *)view writablePasteboardTypesForCell:(id<NSTextAttachmentCell>)cell atIndex:(NSUInteger)charIndex;

// Delegate only.  In this method, the delegate should attempt to write the given attachment to the pasteboard with the given type, and return success or failure.
- (BOOL)textView:(NSTextView *)view writeCell:(id<NSTextAttachmentCell>)cell atIndex:(NSUInteger)charIndex toPasteboard:(NSPasteboard *)pboard type:(NSPasteboardType)type;

// Delegate only.  Will not be called if textView:willChangeSelectionFromCharacterRanges:toCharacterRanges: is implemented.  Effectively prevents multiple selection.
- (NSRange)textView:(NSTextView *)textView willChangeSelectionFromCharacterRange:(NSRange)oldSelectedCharRange toCharacterRange:(NSRange)newSelectedCharRange;

// Delegate only.  Supersedes textView:willChangeSelectionFromCharacterRange:toCharacterRange:.  Return value must be a non-nil, non-empty array of objects responding to rangeValue.
- (NSArray<NSValue *> *)textView:(NSTextView *)textView willChangeSelectionFromCharacterRanges:(NSArray<NSValue *> *)oldSelectedCharRanges toCharacterRanges:(NSArray<NSValue *> *)newSelectedCharRanges;

// Delegate only.  Supersedes textView:shouldChangeTextInRange:replacementString:.  The affectedRanges argument obeys the same restrictions as selectedRanges, and the replacementStrings argument will either be nil (for attribute-only changes) or have the same number of elements as affectedRanges.
- (BOOL)textView:(NSTextView *)textView shouldChangeTextInRanges:(NSArray<NSValue *> *)affectedRanges replacementStrings:(nullable NSArray<NSString *> *)replacementStrings;

// Delegate only.  The delegate should return newTypingAttributes to allow the change, oldTypingAttributes to prevent it, or some other dictionary to modify it.
- (NSDictionary<NSAttributedStringKey, id> *)textView:(NSTextView *)textView shouldChangeTypingAttributes:(NSDictionary<NSString *, id> *)oldTypingAttributes toAttributes:(NSDictionary<NSAttributedStringKey, id> *)newTypingAttributes;

- (void)textViewDidChangeSelection:(NSNotification *)notification;

- (void)textViewDidChangeTypingAttributes:(NSNotification *)notification;

// Delegate only.  Allows delegate to modify the tooltip that will be displayed from that specified by the NSToolTipAttributeName, or to suppress display of the tooltip (by returning nil).
- (nullable NSString *)textView:(NSTextView *)textView willDisplayToolTip:(NSString *)tooltip forCharacterAtIndex:(NSUInteger)characterIndex;

// Delegate only.  Allows delegate to modify the list of completions that will be presented for the partial word at the given range.  Returning nil or a zero-length array suppresses completion.  Optionally may specify the index of the initially selected completion; default is 0, and -1 indicates no selection.
- (NSArray<NSString *> *)textView:(NSTextView *)textView completions:(NSArray<NSString *> *)words forPartialWordRange:(NSRange)charRange indexOfSelectedItem:(nullable NSInteger *)index;

// Delegate only.  If characters are changing, replacementString is what will replace the affectedCharRange.  If attributes only are changing, replacementString will be nil.  Will not be called if textView:shouldChangeTextInRanges:replacementStrings: is implemented.
- (BOOL)textView:(NSTextView *)textView shouldChangeTextInRange:(NSRange)affectedCharRange replacementString:(nullable NSString *)replacementString;

- (BOOL)textView:(NSTextView *)textView doCommandBySelector:(SEL)commandSelector;

// Delegate only.  Allows delegate to control the setting of spelling and grammar indicators.  Values are those listed for NSSpellingStateAttributeName.
- (NSInteger)textView:(NSTextView *)textView shouldSetSpellingState:(NSInteger)value range:(NSRange)affectedCharRange API_AVAILABLE(macos(10.5));

// Delegate only.  Allows delegate to control the context menu returned by menuForEvent:.  The menu parameter is the context menu NSTextView would otherwise return; charIndex is the index of the character that was right-clicked.
- (nullable NSMenu *)textView:(NSTextView *)view menu:(NSMenu *)menu forEvent:(NSEvent *)event atIndex:(NSUInteger)charIndex API_AVAILABLE(macos(10.5));

// Delegate only.  Called by checkTextInRange:types:options:, this method allows control over text checking options (via the return value) or types (by modifying the flags pointed to by the inout parameter checkingTypes).
- (NSDictionary<NSTextCheckingOptionKey, id> *)textView:(NSTextView *)view willCheckTextInRange:(NSRange)range options:(NSDictionary<NSTextCheckingOptionKey, id> *)options types:(NSTextCheckingTypes *)checkingTypes API_AVAILABLE(macos(10.6));

// Delegate only.  Called by handleTextCheckingResults:forRange:orthography:wordCount:, this method allows observation of text checking, or modification of the results (via the return value).
- (NSArray<NSTextCheckingResult *> *)textView:(NSTextView *)view didCheckTextInRange:(NSRange)range types:(NSTextCheckingTypes)checkingTypes options:(NSDictionary<NSTextCheckingOptionKey, id> *)options results:(NSArray<NSTextCheckingResult *> *)results orthography:(NSOrthography *)orthography wordCount:(NSInteger)wordCount API_AVAILABLE(macos(10.6));

// Returns an URL representing the document contents for textAttachment.  The returned NSURL object is utilized by NSTextView for providing default behaviors involving text attachments such as Quick Look and double-clicking.  -[NSTextView quickLookPreviewableItemsInRanges:] uses this method for mapping text attachments to their corresponding document URLs.  NSTextView invokes -[NSWorkspace openURL:] with the URL returned from this method when the delegate has no -textView:doubleClickedOnCell:inRect:atPoint: implementation.
- (nullable NSURL *)textView:(NSTextView *)textView URLForContentsOfTextAttachment:(NSTextAttachment *)textAttachment atIndex:(NSUInteger)charIndex API_AVAILABLE(macos(10.7));

// Delegate only. Returns a sharing service picker created for items right before shown to the screen inside -orderFrontSharingServicePicker: method. The delegate specify a delegate for the NSSharingServicePicker instance. Also, it is allowed to return its own NSSharingServicePicker instance instead.
- (nullable NSSharingServicePicker *)textView:(NSTextView *)textView willShowSharingServicePicker:(NSSharingServicePicker *)servicePicker forItems:(NSArray *)items API_AVAILABLE(macos(10.8));

- (nullable NSUndoManager *)undoManagerForTextView:(NSTextView *)view;


// Delegate only. Invoked from -updateTouchBarItemIdentifiers before setting the item identifiers for textView's NSTouchBar.
- (NSArray<NSTouchBarItemIdentifier> *)textView:(NSTextView *)textView shouldUpdateTouchBarItemIdentifiers:(NSArray<NSTouchBarItemIdentifier> *)identifiers API_AVAILABLE(macos(10.12.2));

// Delegate only. Provides customized list of candidates to textView.candidateListTouchBarItem. Invoked from -updateCandidates. NSTextView uses the candidates returned from this method and suppress its built-in candidate generation. Returning nil from this delegate method allows NSTextView to query candidates from NSSpellChecker.
- (nullable NSArray *)textView:(NSTextView *)textView candidatesForSelectedRange:(NSRange)selectedRange API_AVAILABLE(macos(10.12.2));

// Delegate only. Allows customizing the candidate list queried from NSSpellChecker.
- (NSArray<NSTextCheckingResult *> *)textView:(NSTextView *)textView candidates:(NSArray<NSTextCheckingResult *> *)candidates forSelectedRange:(NSRange)selectedRange API_AVAILABLE(macos(10.12.2));

// Delegate only. Notifies the delegate that the user selected the candidate at index in -[NSCandidateListTouchBarItem candidates] for textView.candidateListTouchBarItem. When no candidate selected, index is NSNotFound. Returning YES allows textView to insert the candidate into the text storage if it's NSString, NSAttributedString, or NSTextCheckingResult.
- (BOOL)textView:(NSTextView *)textView shouldSelectCandidateAtIndex:(NSUInteger)index API_AVAILABLE(macos(10.12.2));


// The following delegate-only methods are deprecated in favor of the more verbose ones above.
- (BOOL)textView:(NSTextView *)textView clickedOnLink:(null_unspecified id)link API_DEPRECATED("Use -textView:clickedOnLink:atIndex: instead", macos(10.0,10.6));
- (void)textView:(NSTextView *)textView clickedOnCell:(null_unspecified id <NSTextAttachmentCell>)cell inRect:(NSRect)cellFrame API_DEPRECATED("Use -textView:clickedOnCell:inRect:atIndex: instead", macos(10.0,10.6));
- (void)textView:(NSTextView *)textView doubleClickedOnCell:(null_unspecified id <NSTextAttachmentCell>)cell inRect:(NSRect)cellFrame API_DEPRECATED("Use -textView:doubleClickedOnCell:inRect:atIndex: instead", macos(10.0,10.6));
- (void)textView:(NSTextView *)view draggedCell:(null_unspecified id <NSTextAttachmentCell>)cell inRect:(NSRect)rect event:(null_unspecified NSEvent *)event API_DEPRECATED("Use -textView:draggedCell:inRect:event:atIndex: instead", macos(10.0,10.6));

@end


#pragma mark NSTouchBarItemIdentifiers
/* Standard NSTouchBarItemIdentifiers */
// An NSTouchBarItemIdentifier for a control selecting special characters (i.e. Emoji). -[NSTouchBar itemForIdentifier:] recognizes the identifier.
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierCharacterPicker API_AVAILABLE(macos(10.12.2));

/* Identifiers recognized by -[NSTextView touchBar:makeItemForIdentifier:] */
// An NSTouchBarItemIdentifier for a control selecting the text color.
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierTextColorPicker API_AVAILABLE(macos(10.12.2));

// An NSTouchBarItemIdentifier for a control selecting the text style.
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierTextStyle API_AVAILABLE(macos(10.12.2));

// An NSTouchBarItemIdentifier for a control selecting the text alignment. -[NSTextView touchBarItemForIdentifier:] returns an NSPopoverTouchBarItem.
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierTextAlignment API_AVAILABLE(macos(10.12.2));

// An NSTouchBarItemIdentifier for a control inserting the text list style. -[NSTextView touchBarItemForIdentifier:] returns an NSPopoverTouchBarItem.
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierTextList API_AVAILABLE(macos(10.12.2));

// An NSTouchBarItemIdentifier for a group of text format controls.
APPKIT_EXTERN NSTouchBarItemIdentifier const NSTouchBarItemIdentifierTextFormat API_AVAILABLE(macos(10.12.2));


// NSOldNotifyingTextView -> the old view, NSNewNotifyingTextView -> the new view.  The text view delegate is not automatically registered to receive this notification because the text machinery will automatically switch over the delegate to observe the new first text view as the first text view changes.
APPKIT_EXTERN NSNotificationName NSTextViewWillChangeNotifyingTextViewNotification;

// NSOldSelectedCharacterRange -> NSValue with old range.
APPKIT_EXTERN NSNotificationName NSTextViewDidChangeSelectionNotification;

APPKIT_EXTERN NSNotificationName NSTextViewDidChangeTypingAttributesNotification;

APPKIT_EXTERN NSNotificationName NSTextViewWillSwitchToNSLayoutManagerNotification;
APPKIT_EXTERN NSNotificationName NSTextViewDidSwitchToNSLayoutManagerNotification;


/* These constants are deprecated in favor of their NSTextFinder equivalents. */

/* Values for NSFindPanelAction */
typedef NS_ENUM(NSUInteger, NSFindPanelAction) {
    NSFindPanelActionShowFindPanel = 1,
    NSFindPanelActionNext = 2,
    NSFindPanelActionPrevious = 3,
    NSFindPanelActionReplaceAll = 4,
    NSFindPanelActionReplace = 5,
    NSFindPanelActionReplaceAndFind = 6,
    NSFindPanelActionSetFindString = 7,
    NSFindPanelActionReplaceAllInSelection = 8,
    NSFindPanelActionSelectAll = 9,
    NSFindPanelActionSelectAllInSelection = 10
};

/* Values for NSFindPanel search metadata */

APPKIT_EXTERN NSPasteboardType NSFindPanelSearchOptionsPboardType  API_AVAILABLE(macos(10.5));

typedef NSString * NSPasteboardTypeFindPanelSearchOptionKey NS_TYPED_ENUM;
APPKIT_EXTERN NSPasteboardTypeFindPanelSearchOptionKey NSFindPanelCaseInsensitiveSearch API_AVAILABLE(macos(10.5));     // BOOL
APPKIT_EXTERN NSPasteboardTypeFindPanelSearchOptionKey NSFindPanelSubstringMatch API_AVAILABLE(macos(10.5));            // NSNumber containing NSFindPanelSubstringMatchType

typedef NS_ENUM(NSUInteger, NSFindPanelSubstringMatchType) {
    NSFindPanelSubstringMatchTypeContains = 0,
    NSFindPanelSubstringMatchTypeStartsWith = 1,
    NSFindPanelSubstringMatchTypeFullWord = 2,
    NSFindPanelSubstringMatchTypeEndsWith = 3
};

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
