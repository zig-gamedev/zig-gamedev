/*
	NSControl.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSView.h>
#import <AppKit/NSText.h>
#import <AppKit/NSCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSCell, NSFont, NSTextView, NSNotification, NSAttributedString, NSFormatter;

@interface NSControl : NSView

- (instancetype)initWithFrame:(NSRect)frameRect NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (nullable, weak) id target; // Target is weak for zeroing-weak compatible objects in apps linked on 10.10 or later. Otherwise the behavior of this property is 'assign’.
@property (nullable) SEL action;
@property NSInteger tag;
@property BOOL ignoresMultiClick;
@property (getter=isContinuous) BOOL continuous;
@property (getter=isEnabled) BOOL enabled;
@property BOOL refusesFirstResponder;
@property (getter=isHighlighted) BOOL highlighted API_AVAILABLE(macos(10.10));
@property NSControlSize controlSize API_AVAILABLE(macos(10.10));
@property (nullable, strong) __kindof NSFormatter *formatter;

@property (nullable, copy) id  /* id<NSCopying> */ objectValue;
@property (copy) NSString *stringValue;
@property (copy) NSAttributedString *attributedStringValue;
@property int intValue;
@property NSInteger integerValue;
@property float floatValue;
@property double doubleValue;

- (NSSize)sizeThatFits:(NSSize)size API_AVAILABLE(macos(10.10));
- (void)sizeToFit;

- (NSInteger)sendActionOn:(NSEventMask)mask;

- (BOOL)sendAction:(nullable SEL)action to:(nullable id)target;
- (void)takeIntValueFrom:(nullable id)sender;
- (void)takeFloatValueFrom:(nullable id)sender;
- (void)takeDoubleValueFrom:(nullable id)sender;
- (void)takeStringValueFrom:(nullable id)sender;
- (void)takeObjectValueFrom:(nullable id)sender;
- (void)takeIntegerValueFrom:(nullable id)sender API_AVAILABLE(macos(10.5));

- (void)mouseDown:(NSEvent *)event;
- (void)performClick:(nullable id)sender;

/* The following methods apply only to controls with apparent textual content of some sort (buttons with labels, textfields, etc.).
 */
@property (nullable, copy) NSFont *font;

@property BOOL usesSingleLineMode API_AVAILABLE(macos(10.10));

#if !TARGET_OS_IPHONE
@property NSLineBreakMode lineBreakMode API_AVAILABLE(macos(10.10));
@property NSTextAlignment alignment;
@property NSWritingDirection baseWritingDirection;
#endif

/* Gets and sets the ability for expansion tool tips to be shown or not. Expansion tooltips are automatically shown when the cell can not show the full content. This is controlled by the NSCell API expansionFrameWithFrame:inView: and is drawn by drawWithExpansionFrame:inView:. The default value is NO.
    This value is encoded along with the control. In general, it is recommended to turn this on for NSTextFields in a View Based NSTableView.
 */
@property BOOL allowsExpansionToolTips API_AVAILABLE(macos(10.8));

/*  Allows the control to return an expansion tool tip frame if contentFrame is too small for the entire contents in the view. When the mouse is hovered over the text in certain controls, the full contents will be shown in a special floating tool tip view. If the frame is not too small, return an empty rect, and no expansion tool tip view will be shown. By default, NSControl returns NSZeroRect, while some subclasses (such as NSTextField) will return the proper frame when required.
 */
- (NSRect)expansionFrameWithFrame:(NSRect)contentFrame API_AVAILABLE(macos(10.10));

/* Allows the control to perform custom expansion tool tip drawing. Note that the view may be different from the original view that the text appeared in.
 */
- (void)drawWithExpansionFrame:(NSRect)contentFrame inView:(NSView *)view API_AVAILABLE(macos(10.10));

@end

/* The following category applies only to controls with editable text, like NSTextField.
 */
@interface NSControl(NSControlEditableTextMethods)
- (nullable NSText *)currentEditor;
- (BOOL)abortEditing;
- (void)validateEditing;

- (void)editWithFrame:(NSRect)rect editor:(NSText *)textObj delegate:(nullable id)delegate event:(NSEvent *)event API_AVAILABLE(macos(10.10));
- (void)selectWithFrame:(NSRect)rect editor:(NSText *)textObj delegate:(nullable id)delegate start:(NSInteger)selStart length:(NSInteger)selLength API_AVAILABLE(macos(10.10));
- (void)endEditing:(NSText *)textObj API_AVAILABLE(macos(10.10));
@end

@protocol NSControlTextEditingDelegate <NSObject>
@optional

- (void)controlTextDidBeginEditing:(NSNotification *)obj;
- (void)controlTextDidEndEditing:(NSNotification *)obj;
- (void)controlTextDidChange:(NSNotification *)obj;

// These delegate and notification methods are sent from NSControl subclasses that allow text editing such as NSTextField and NSMatrix.  The classes that need to send these have delegates.  NSControl does not.
- (BOOL)control:(NSControl *)control textShouldBeginEditing:(NSText *)fieldEditor;
- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor;
- (BOOL)control:(NSControl *)control didFailToFormatString:(NSString *)string errorDescription:(nullable NSString *)error;
- (void)control:(NSControl *)control didFailToValidatePartialString:(NSString *)string errorDescription:(nullable NSString *)error;
- (BOOL)control:(NSControl *)control isValidObject:(nullable id)obj;

- (BOOL)control:(NSControl *)control textView:(NSTextView *)textView doCommandBySelector:(SEL)commandSelector;
- (NSArray<NSString *> *)control:(NSControl *)control textView:(NSTextView *)textView completions:(NSArray<NSString *> *)words forPartialWordRange:(NSRange)charRange indexOfSelectedItem:(NSInteger *)index;
@end

                                                                    // userInfo keys:
APPKIT_EXTERN NSNotificationName NSControlTextDidBeginEditingNotification;	//	@"NSFieldEditor"
APPKIT_EXTERN NSNotificationName NSControlTextDidEndEditingNotification;	//	@"NSFieldEditor"
APPKIT_EXTERN NSNotificationName NSControlTextDidChangeNotification;		//	@"NSFieldEditor"


@interface NSControl (NSDeprecated)

// Use formatters instead.  See -[NSControl formatter] and -[NSControl setFormatter:].
- (void)setFloatingPointFormat:(BOOL)autoRange left:(NSUInteger)leftDigits right:(NSUInteger)rightDigits API_DEPRECATED("", macos(10.0,10.0));

@property (class, nullable) Class cellClass;

@property (nullable, strong) __kindof NSCell *cell;

- (nullable __kindof NSCell *)selectedCell;
- (NSInteger)selectedTag;

- (void)setNeedsDisplay API_DEPRECATED("Set the needsDisplay property to YES instead", macos(10.0,10.14));
- (void)calcSize API_DEPRECATED("Override -layout instead. This method should never be called", macos(10.0,10.14));

- (void)updateCell:(NSCell *)cell;
- (void)updateCellInside:(NSCell *)cell;
- (void)drawCellInside:(NSCell *)cell;
- (void)drawCell:(NSCell *)cell;
- (void)selectCell:(NSCell *)cell;

@end

#if __swift__ < 40200
@interface NSObject(NSControlSubclassNotifications)
- (void)controlTextDidBeginEditing:(NSNotification *)obj API_DEPRECATED("This is now an optional method of the NSControlTextEditingDelegate protocol.", macos(10.0, 11.0));
- (void)controlTextDidEndEditing:(NSNotification *)obj API_DEPRECATED("This is now an optional method of the NSControlTextEditingDelegate protocol.", macos(10.0, 11.0));
- (void)controlTextDidChange:(NSNotification *)obj API_DEPRECATED("This is now an optional method of the NSControlTextEditingDelegate protocol.", macos(10.0, 11.0));
@end
#endif

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
