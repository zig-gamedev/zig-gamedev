#if !__has_include(<UIFoundation/NSText.h>)
/*
	NSText.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSView.h>
#import <AppKit/NSSpellProtocol.h>
#import <AppKit/AppKitDefines.h>

@class NSColor, NSFont, NSNotification;
@protocol NSTextDelegate;

NS_ASSUME_NONNULL_BEGIN

#if !TARGET_OS_IPHONE

#if !__NSTEXT_SHARED_SECTION__
#define __NSTEXT_SHARED_SECTION__ 1
#pragma mark NSTextAlignment
typedef NS_ENUM(NSInteger, NSTextAlignment) {
    NSTextAlignmentLeft      = 0,    // Visually left aligned
#if TARGET_ABI_USES_IOS_VALUES
    NSTextAlignmentCenter    = 1,    // Visually centered
    NSTextAlignmentRight     = 2,    // Visually right aligned
#else /* !TARGET_ABI_USES_IOS_VALUES */
    NSTextAlignmentRight     = 1,    // Visually right aligned
    NSTextAlignmentCenter    = 2,    // Visually centered
#endif
    NSTextAlignmentJustified = 3,    // Fully-justified. The last line in a paragraph is natural-aligned.
    NSTextAlignmentNatural   = 4     // Indicates the default alignment for script
} API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0));

#pragma mark NSWritingDirection
typedef NS_ENUM(NSInteger, NSWritingDirection) {
    NSWritingDirectionNatural       = -1,   // Determines direction using the Unicode Bidi Algorithm rules P2 and P3
    NSWritingDirectionLeftToRight   = 0,    // Left to right writing direction
    NSWritingDirectionRightToLeft   = 1     // Right to left writing direction
} API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0));
#endif // __NSTEXT_SHARED_SECTION__

#endif // !TARGET_OS_IPHONE

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST
@interface NSText : NSView <NSChangeSpelling, NSIgnoreMisspelledWords>

- (instancetype)initWithFrame:(NSRect)frameRect NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (copy) NSString *string;

- (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)string;
- (void)replaceCharactersInRange:(NSRange)range withRTF:(NSData *)rtfData;
- (void)replaceCharactersInRange:(NSRange)range withRTFD:(NSData *)rtfdData;

- (nullable NSData *)RTFFromRange:(NSRange)range;
- (nullable NSData *)RTFDFromRange:(NSRange)range;

- (BOOL)writeRTFDToFile:(NSString *)path atomically:(BOOL)flag;
- (BOOL)readRTFDFromFile:(NSString *)path;

@property (nullable, assign) id<NSTextDelegate> delegate;

@property (getter=isEditable) BOOL editable;
@property (getter=isSelectable) BOOL selectable;
@property (getter=isRichText) BOOL richText;        //If NO, also clears setImportsGraphics:
@property BOOL importsGraphics; // If YES, also sets setRichText:
@property (getter=isFieldEditor) BOOL fieldEditor;     // Indicates whether to end editing on CR, TAB, etc.
@property BOOL usesFontPanel;
@property BOOL drawsBackground;
@property (nullable, copy) NSColor *backgroundColor; // Default is nil. If nil, it implies -drawsBackground=NO

@property (getter=isRulerVisible, readonly) BOOL rulerVisible;

@property NSRange selectedRange;

- (void)scrollRangeToVisible:(NSRange)range;

@property (nullable, strong) NSFont *font;
@property (nullable, copy) NSColor *textColor; // Default is nil. If nil, draws with blackColor

#if !TARGET_OS_IPHONE
@property NSTextAlignment alignment;
@property NSWritingDirection baseWritingDirection;
#endif // !TARGET_OS_IPHONE

- (void)setTextColor:(nullable NSColor *)color range:(NSRange)range; // Passing nil removes NSForegroundColorAttributeName from range
- (void)setFont:(NSFont *)font range:(NSRange)range;

@property NSSize maxSize;
@property NSSize minSize;

@property (getter=isHorizontallyResizable) BOOL horizontallyResizable;
@property (getter=isVerticallyResizable) BOOL verticallyResizable;

- (void)sizeToFit;

- (void)copy:(nullable id)sender;
- (void)copyFont:(nullable id)sender;
- (void)copyRuler:(nullable id)sender;
- (void)cut:(nullable id)sender;
- (void)delete:(nullable id)sender;
- (void)paste:(nullable id)sender;
- (void)pasteFont:(nullable id)sender;
- (void)pasteRuler:(nullable id)sender;
- (void)selectAll:(nullable id)sender;
- (void)changeFont:(nullable id)sender;
- (void)alignLeft:(nullable id)sender;
- (void)alignRight:(nullable id)sender;
- (void)alignCenter:(nullable id)sender;
- (void)subscript:(nullable id)sender;
- (void)superscript:(nullable id)sender;
- (void)underline:(nullable id)sender;
- (void)unscript:(nullable id)sender;
- (void)showGuessPanel:(nullable id)sender;
- (void)checkSpelling:(nullable id)sender;
- (void)toggleRuler:(nullable id)sender;

@end

API_UNAVAILABLE_END

#if !TARGET_OS_IPHONE
/* Various important Unicode code points */
enum {
    NSEnterCharacter                = 0x0003,
    NSBackspaceCharacter            = 0x0008,
    NSTabCharacter                  = 0x0009,
    NSNewlineCharacter              = 0x000a,
    NSFormFeedCharacter             = 0x000c,
    NSCarriageReturnCharacter       = 0x000d,
    NSBackTabCharacter              = 0x0019,
    NSDeleteCharacter               = 0x007f,
    NSLineSeparatorCharacter        = 0x2028,
    NSParagraphSeparatorCharacter   = 0x2029
};

/* Movement codes for movement between fields; these codes are the integer values of the NSTextMovement key in NSTextDidEndEditing notifications, and are used when completions change in the NSTextView method insertCompletion:forPartialWordRange:movement:isFinal:.  Note that the value 0 is used for movements that do not fall under any of the other values, hence NSOtherTextMovement is a more appropriate name than the previous NSIllegalTextMovement.
*/
typedef NS_ENUM(NSInteger, NSTextMovement) {
    NSTextMovementReturn        = 0x10,
    NSTextMovementTab           = 0x11,
    NSTextMovementBacktab       = 0x12,
    NSTextMovementLeft          = 0x13,
    NSTextMovementRight         = 0x14,
    NSTextMovementUp            = 0x15,
    NSTextMovementDown          = 0x16,
    NSTextMovementCancel        = 0x17,
    NSTextMovementOther         = 0
};

/* Notifications */
APPKIT_EXTERN NSNotificationName NSTextDidBeginEditingNotification;
APPKIT_EXTERN NSNotificationName NSTextDidEndEditingNotification;    // userInfo key:  NSTextMovementUserInfoKey
APPKIT_EXTERN NSNotificationName NSTextDidChangeNotification;

// The user info dictionary key for NSTextDidEndEditingNotification
APPKIT_EXTERN NSString * const NSTextMovementUserInfoKey API_AVAILABLE(macos(10.13));

/* Deprecated */
// The following enum items are deprecated. Use NSTextMovement instead
enum {
    NSIllegalTextMovement = 0,
    NSReturnTextMovement = 0x10,
    NSTabTextMovement = 0x11,
    NSBacktabTextMovement = 0x12,
    NSLeftTextMovement = 0x13,
    NSRightTextMovement = 0x14,
    NSUpTextMovement = 0x15,
    NSDownTextMovement = 0x16,
    NSCancelTextMovement = 0x17,
    NSOtherTextMovement = 0
};

@protocol NSTextDelegate <NSObject>
@optional
- (BOOL)textShouldBeginEditing:(NSText *)textObject;        // YES means do it
- (BOOL)textShouldEndEditing:(NSText *)textObject;          // YES means do it
- (void)textDidBeginEditing:(NSNotification *)notification;
- (void)textDidEndEditing:(NSNotification *)notification;
- (void)textDidChange:(NSNotification *)notification;       // Any keyDown or paste which changes the contents causes this
@end

/* Additional values to be added to NSWritingDirectionLeftToRight or NSWritingDirectionRightToLeft, when used with NSWritingDirectionAttributeName */
enum {
    NSTextWritingDirectionEmbedding API_DEPRECATED("Use NSWritingDirectionEmbedding instead", macos(10.0,10.11))     = (0 << 1),
    NSTextWritingDirectionOverride API_DEPRECATED("Use NSWritingDirectionOverride instead", macos(10.0,10.11))      = (1 << 1)
};

static const NSTextAlignment NSLeftTextAlignment API_DEPRECATED_WITH_REPLACEMENT("NSTextAlignmentLeft", macos(10.0,10.12))  = NSTextAlignmentLeft;
static const NSTextAlignment NSRightTextAlignment API_DEPRECATED_WITH_REPLACEMENT("NSTextAlignmentRight", macos(10.0,10.12))  = NSTextAlignmentRight;
static const NSTextAlignment NSCenterTextAlignment API_DEPRECATED_WITH_REPLACEMENT("NSTextAlignmentCenter", macos(10.0,10.12))  = NSTextAlignmentCenter;
static const NSTextAlignment NSJustifiedTextAlignment API_DEPRECATED_WITH_REPLACEMENT("NSTextAlignmentJustified", macos(10.0,10.12))  = NSTextAlignmentJustified;
static const NSTextAlignment NSNaturalTextAlignment API_DEPRECATED_WITH_REPLACEMENT("NSTextAlignmentNatural", macos(10.0,10.12))  = NSTextAlignmentNatural;
#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSText.h>
#endif
