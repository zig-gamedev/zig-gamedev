/*
	NSCell.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/NSText.h>
#import <AppKit/NSParagraphStyle.h>
#import <AppKit/NSUserInterfaceItemIdentification.h>
#import <AppKit/NSAccessibilityProtocols.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSAttributedString, NSDraggingImageComponent, NSEvent, NSFont, NSFormatter, NSImage, NSMenu, NSText, NSTextView, NSView;

typedef NS_ENUM(NSUInteger, NSCellType) {
    NSNullCellType  = 0,
    NSTextCellType  = 1,
    NSImageCellType = 2
};

typedef NS_ENUM(NSUInteger, NSCellAttribute) {
    NSCellDisabled               = 0,
    NSCellState                  = 1,
    NSPushInCell                 = 2,
    NSCellEditable               = 3,
    NSChangeGrayCell             = 4,
    NSCellHighlighted            = 5,
    NSCellLightsByContents       = 6,
    NSCellLightsByGray           = 7,
    NSChangeBackgroundCell       = 8,
    NSCellLightsByBackground     = 9,
    NSCellIsBordered             = 10,
    NSCellHasOverlappingImage    = 11,
    NSCellHasImageHorizontal     = 12,
    NSCellHasImageOnLeftOrBottom = 13,
    NSCellChangesContents        = 14,
    NSCellIsInsetButton          = 15,
    NSCellAllowsMixedState       = 16
};

typedef NS_ENUM(NSUInteger, NSCellImagePosition) {
    NSNoImage       = 0,
    NSImageOnly     = 1,
    NSImageLeft     = 2,
    NSImageRight    = 3,
    NSImageBelow    = 4,
    NSImageAbove    = 5,
    NSImageOverlaps = 6,
    NSImageLeading  API_AVAILABLE(macos(10.12)) = 7,
    NSImageTrailing API_AVAILABLE(macos(10.12)) = 8
};

typedef NS_ENUM(NSUInteger, NSImageScaling) {
    NSImageScaleProportionallyDown = 0, // Scale image down if it is too large for destination. Preserve aspect ratio.
    NSImageScaleAxesIndependently,      // Scale each dimension to exactly fit destination. Do not preserve aspect ratio.
    NSImageScaleNone,                   // Do not scale.
    NSImageScaleProportionallyUpOrDown, // Scale image to maximum possible dimensions while (1) staying within destination area (2) preserving aspect ratio
    
    NSScaleProportionally API_DEPRECATED("Use NSImageScaleProportionallyDown instead", macos(10.0,10.10)) = 0,
    NSScaleToFit API_DEPRECATED("Use NSImageScaleAxesIndependently instead", macos(10.0,10.10)),
    NSScaleNone API_DEPRECATED("Use NSImageScaleNone instead", macos(10.0,10.10))
} API_AVAILABLE(macos(10.5));

typedef NSInteger NSControlStateValue NS_TYPED_EXTENSIBLE_ENUM;
static const NSControlStateValue NSControlStateValueMixed = -1;
static const NSControlStateValue NSControlStateValueOff = 0;
static const NSControlStateValue NSControlStateValueOn = 1;

typedef NS_OPTIONS(NSUInteger, NSCellStyleMask) {
    NSNoCellMask               = 0,
    NSContentsCellMask         = 1,
    NSPushInCellMask           = 2,
    NSChangeGrayCellMask       = 4,
    NSChangeBackgroundCellMask = 8
};

typedef NS_ENUM(NSUInteger, NSControlTint) {
    NSDefaultControlTint  = 0,
    NSBlueControlTint     = 1,
    NSGraphiteControlTint = 6,
    NSClearControlTint    = 7
};

typedef NS_ENUM(NSUInteger, NSControlSize) {
    NSControlSizeRegular = 0,
    NSControlSizeSmall = 1,
    NSControlSizeMini = 2,
    NSControlSizeLarge API_AVAILABLE(macos(11.0)) = 3
};

NS_SWIFT_UI_ACTOR
@interface NSCell : NSObject <NSCopying, NSCoding, NSUserInterfaceItemIdentification, NSAccessibilityElement, NSAccessibility>

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initTextCell:(NSString *)string NS_DESIGNATED_INITIALIZER;
- (instancetype)initImageCell:(nullable NSImage *)image NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (class, readonly) BOOL prefersTrackingUntilMouseUp;

@property (nullable, assign) NSView *controlView; // Must be an NSControl subclass
@property NSCellType type;
@property NSControlStateValue state;
@property (nullable, weak) id target; // Target is weak for zeroing-weak compatible objects in apps linked on 10.10 or later. Otherwise the behavior of this property is 'assign'.
@property (nullable) SEL action;
@property NSInteger tag;
@property (copy) NSString *title;
@property (getter=isOpaque, readonly) BOOL opaque;
@property (getter=isEnabled) BOOL enabled;

- (NSInteger)sendActionOn:(NSEventMask)mask;

@property (getter=isContinuous) BOOL continuous;
@property (getter=isEditable) BOOL editable;
@property (getter=isSelectable) BOOL selectable;
@property (getter=isBordered) BOOL bordered;
@property (getter=isBezeled) BOOL bezeled;
@property (getter=isScrollable) BOOL scrollable; /* If YES, sets wraps to NO */
@property (getter=isHighlighted) BOOL highlighted;
#if !TARGET_OS_IPHONE
@property NSTextAlignment alignment;
#endif
@property BOOL wraps;    /* If YES, sets scrollable to NO */
@property (nullable, strong) NSFont *font;
@property (readonly, copy) NSString *keyEquivalent;
@property (nullable, strong) __kindof NSFormatter *formatter;
@property (nullable, copy) id /* id <NSCopying> */ objectValue;
@property (readonly) BOOL hasValidObjectValue;
@property (copy) NSString *stringValue;
- (NSComparisonResult)compare:(id)otherCell;
@property int intValue;
@property float floatValue;
@property double doubleValue;
@property NSInteger integerValue API_AVAILABLE(macos(10.5));
- (void)takeIntValueFrom:(nullable id)sender;
- (void)takeFloatValueFrom:(nullable id)sender;
- (void)takeDoubleValueFrom:(nullable id)sender;
- (void)takeStringValueFrom:(nullable id)sender;
- (void)takeObjectValueFrom:(nullable id)sender;
- (void)takeIntegerValueFrom:(nullable id)sender API_AVAILABLE(macos(10.5));
@property (nullable, strong) NSImage *image;
@property NSControlSize controlSize;
@property (nullable, strong) id representedObject;
- (NSInteger)cellAttribute:(NSCellAttribute)parameter;
- (void)setCellAttribute:(NSCellAttribute)parameter to:(NSInteger)value;
- (NSRect)imageRectForBounds:(NSRect)rect;
- (NSRect)titleRectForBounds:(NSRect)rect;
- (NSRect)drawingRectForBounds:(NSRect)rect;
@property (readonly) NSSize cellSize;
- (NSSize)cellSizeForBounds:(NSRect)rect;
- (nullable NSColor *)highlightColorWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)calcDrawInfo:(NSRect)rect;
- (NSText *)setUpFieldEditorAttributes:(NSText *)textObj;
- (void)drawInteriorWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)drawWithFrame:(NSRect)cellFrame inView:(NSView *)controlView;
- (void)highlight:(BOOL)flag withFrame:(NSRect)cellFrame inView:(NSView *)controlView;
@property (readonly) NSInteger mouseDownFlags;
- (void)getPeriodicDelay:(float *)delay interval:(float *)interval;
- (BOOL)startTrackingAt:(NSPoint)startPoint inView:(NSView *)controlView;
- (BOOL)continueTracking:(NSPoint)lastPoint at:(NSPoint)currentPoint inView:(NSView *)controlView;
- (void)stopTracking:(NSPoint)lastPoint at:(NSPoint)stopPoint inView:(NSView *)controlView mouseIsUp:(BOOL)flag;
- (BOOL)trackMouse:(NSEvent *)event inRect:(NSRect)cellFrame ofView:(NSView *)controlView untilMouseUp:(BOOL)flag;
- (void)editWithFrame:(NSRect)rect inView:(NSView *)controlView editor:(NSText *)textObj delegate:(nullable id)delegate event:(nullable NSEvent *)event;
- (void)selectWithFrame:(NSRect)rect inView:(NSView *)controlView editor:(NSText *)textObj delegate:(nullable id)delegate start:(NSInteger)selStart length:(NSInteger)selLength;
- (void)endEditing:(NSText *)textObj;
- (void)resetCursorRect:(NSRect)cellFrame inView:(NSView *)controlView;

@property (nullable, strong) NSMenu *menu;
- (nullable NSMenu *)menuForEvent:(NSEvent *)event inRect:(NSRect)cellFrame ofView:(NSView *)view;
@property (class, readonly, nullable, strong) NSMenu *defaultMenu;

@property BOOL sendsActionOnEndEditing;

#if !TARGET_OS_IPHONE
@property NSWritingDirection baseWritingDirection;
#endif

#if !TARGET_OS_IPHONE
@property NSLineBreakMode lineBreakMode;
#endif

@property BOOL allowsUndo;

/* Truncates and adds the ellipsis character to the last visible line if the text doesn't fit into the cell bounds. The setting is ignored if -lineBreakMode is neither NSLineBreakByWordWrapping nor NSLineBreakByCharWrapping.
 */
@property BOOL truncatesLastVisibleLine API_AVAILABLE(macos(10.5));

/* Bi-directional User Interface. It specifies the general UI layout flow directions.
*/
@property NSUserInterfaceLayoutDirection userInterfaceLayoutDirection API_AVAILABLE(macos(10.6));

/* Returns a custom field editor for editing inside controlView. This is an override point for NSCell subclasses designed to work with its own custom field editor. This message is sent to the selected cell of controlView in -[NSWindow fieldEditor:forObject:]. Returning non-nil from this method indicates skipping the standard field editor querying processes including -windowWillReturnFieldEditor:toObject: delegation. The default NSCell implementation returns nil. The field editor returned from this method should have isFieldEditor == YES.
 */
- (nullable NSTextView *)fieldEditorForView:(NSView *)controlView API_AVAILABLE(macos(10.6));

/* Tells the text cell to layout/render its content in single-line. If YES, the cell ignores the return value from -wraps, interprets NSLineBreakByWordWrapping and NSLineBreakByCharWrapping from -lineBreakMode as NSLineBreakByClipping, and configures the field editor to ignore key binding commands that insert paragraph/line separators. Also, the field editor bound to a single line cell filters paragraph/line separator insertion from user actions. Cells in the single line mode use the fixed baseline layout. The text baseline position is determined solely by the control size regardless of content font style/size.
 */
@property BOOL usesSingleLineMode API_AVAILABLE(macos(10.6));

/* Multi-image Drag Support. The default implementation will return an array of up to two NSDraggingImageComponent instances -- one for the image portion and another for the text portion (if appropriate). This method can be subclassed and overridden to provide a custom set of NSDraggingImageComponents to create the drag image for the cell. This method is generally used by NSTableView/NSOutlineView.
 */
- (NSArray<NSDraggingImageComponent *> *)draggingImageComponentsWithFrame:(NSRect)frame inView:(NSView *)view API_AVAILABLE(macos(10.7));

@end

@interface NSCell(NSKeyboardUI)
@property BOOL refusesFirstResponder;
@property (readonly) BOOL acceptsFirstResponder;
@property BOOL showsFirstResponder;

- (void)performClick:(nullable id)sender;

@property NSFocusRingType focusRingType;
@property (class, readonly) NSFocusRingType defaultFocusRingType;

- (void)drawFocusRingMaskWithFrame:(NSRect)cellFrame inView:(NSView *)controlView API_AVAILABLE(macos(10.7));
- (NSRect)focusRingMaskBoundsForFrame:(NSRect)cellFrame inView:(NSView *)controlView API_AVAILABLE(macos(10.7));

@property (readonly) BOOL wantsNotificationForMarkedText; // If the receiver returns YES, the field editor initiated by it posts text change notifications (i.e. NSTextDidChangeNotification) while editing marked text; otherwise, they are delayed until the marked text confirmation. The NSCell's implementation returns NO.
@end

@interface NSCell(NSCellAttributedStringMethods)
@property (copy) NSAttributedString *attributedStringValue;
/* These methods determine whether the user can modify text attributes and import graphics in a rich cell.  Note that whatever these flags are, cells can still contain attributed text if programmatically set. */
@property BOOL allowsEditingTextAttributes;       /* If NO, also clears setImportsGraphics: */
@property BOOL importsGraphics;                   /* If YES, also sets setAllowsEditingTextAttributes: */
@end

@interface NSCell(NSCellMixedState)	/* allow button to have mixed state value*/
@property BOOL allowsMixedState;
@property (readonly) NSInteger nextState;			/* get next state state in cycle */
- (void)setNextState;			/* toggle/cycle through states */
@end

/* Cell Hit testing support */

typedef NS_OPTIONS(NSUInteger, NSCellHitResult) {
    // An empty area, or did not hit in the cell
    NSCellHitNone = 0,
    // A content area in the cell
    NSCellHitContentArea = 1 << 0,
    // An editable text area of the cell
    NSCellHitEditableTextArea = 1 << 1,
    // A trackable area in the cell
    NSCellHitTrackableArea = 1 << 2,
} API_AVAILABLE(macos(10.5));

@interface NSCell(NSCellHitTest)
/* Return hit testing information for the cell. Use a bit-wise mask to look for a specific value when calling the method. Generally, this should be overridden by custom NSCell subclasses to return the correct result. Currently, it is called by some multi-cell views, such as NSTableView.

    By default, NSCell will look at the cell type and do the following:

    NSImageCellType: 
        If the image exists, and the event point is in the image return NSCellHitContentArea, else NSCellHitNone.

    NSTextCellType (also applies to NSTextFieldCell): 
        If there is text:
            If the event point hits in the text, return NSCellHitContentArea. Additionally, if the cell is enabled return NSCellHitContentArea | NSCellHitEditableTextArea.
        If there is not text:
            Returns NSCellHitNone.

    NSNullCellType (this is the default that applies to non text or image cells who don't override hitTestForEvent:):
        Return NSCellHitContentArea by default.
        If the cell not disabled, and it would track, return NSCellHitContentArea | NSCellHitTrackableArea.
*/
- (NSCellHitResult)hitTestForEvent:(NSEvent *)event inRect:(NSRect)cellFrame ofView:(NSView *)controlView API_AVAILABLE(macos(10.5));
@end

@interface NSCell(NSCellExpansion)
/*  Allows the cell to return an expansion cell frame if cellFrame is too small for the entire contents in the view. When the mouse is hovered over the cell in certain controls, the full cell contents will be shown in a special floating tool tip view. If the frame is not too small, return an empty rect, and no expansion tool tip view will be shown. By default, NSCell returns NSZeroRect, while some subclasses (such as NSTextFieldCell) will return the proper frame when required. 
*/
- (NSRect)expansionFrameWithFrame:(NSRect)cellFrame inView:(NSView *)view API_AVAILABLE(macos(10.5));

/* Allows the cell to perform custom expansion tool tip drawing. Note that the view may be different from the original view that the cell appeared in. By default, NSCell simply calls drawWithFrame:inView:.
*/
- (void)drawWithExpansionFrame:(NSRect)cellFrame inView:(NSView *)view API_AVAILABLE(macos(10.5));
@end

typedef NS_ENUM(NSInteger, NSBackgroundStyle) {
    /* The background reflects the predominant color scheme of the view's appearance. */
    NSBackgroundStyleNormal = 0,

    /* The background is indicating emphasis (e.g. selection state) using an alternate color or visual effect. Content may alter its appearance to reflect this emphasis. */
    NSBackgroundStyleEmphasized,

    /* The background is intended to appear higher than the content drawn on it. Content might need to be inset. */
    NSBackgroundStyleRaised,

    /* The background is intended to appear lower than the content drawn on it. Content might need to be embossed. */
    NSBackgroundStyleLowered,
} API_AVAILABLE(macos(10.5));

@interface NSCell (NSCellBackgroundStyle)

/* Describes the surface the cell is drawn onto in -[NSCell drawWithFrame:inView:]. A control typically sets this before it asks the cell to draw. A cell may draw differently based on background characteristics. For example, a tableview drawing a cell in a selected row might call [cell setBackgroundStyle:NSBackgroundStyleEmphasized]. A text cell might decide to render its text using alternateSelectedControlTextColor as a result.
 */
@property NSBackgroundStyle backgroundStyle API_AVAILABLE(macos(10.5));

/* Describes the surface drawn onto in -[NSCell drawInteriorWithFrame:inView:]. This is often the same as the backgroundStyle, but a button that draws a bezel would have a different interiorBackgroundStyle.  
 
 This is both an override point and a useful method to call. A button that draws a custom bezel would override this to describe that surface. A cell that has custom interior drawing might query this method to help pick an image that looks good on the cell. Calling this method gives you some independence from changes in framework art style.
 */
@property (readonly) NSBackgroundStyle interiorBackgroundStyle API_AVAILABLE(macos(10.5));

@end

/* Draw an image from two end caps and a fill.  The end caps are scaled proportionally to match the thickness of the destination frame.  In the horizontal case, the startCap is drawn into the left part of the destination, the endCap is drawn into the right part of the destination, and the fill is tiled over the remaining area.  The caps and the fill should all be the same height.  The vertical case is similar.

 This is an appropriate way to draw the bezel of a button that can be resized in one dimension.
 */
APPKIT_EXTERN void NSDrawThreePartImage(NSRect frame, NSImage * _Nullable startCap, NSImage * _Nullable centerFill, NSImage * _Nullable endCap, BOOL vertical, NSCompositingOperation op, CGFloat alphaFraction, BOOL flipped) API_AVAILABLE(macos(10.5));

/* Draw an image from nine pieces.  When drawn, the destination rect is partitioned into nine rectangular regions: the corner pieces are the natural size of the corner images, the edge pieces are the natural size of the edge fill images in the direction perpendicular to the edge and flush with the corners.  The center rect fills the remaining space.  The supplied images and fills are drawn into the corresponding regions, with fill images tiled at their natural dimensions.  Images that share a border should have the same thickness in that dimension.

 This method is appropriate for the bezel of a control, like a box, that can be resized in both dimensions.
 */
APPKIT_EXTERN void NSDrawNinePartImage(NSRect frame, NSImage * _Nullable topLeftCorner, NSImage * _Nullable topEdgeFill, NSImage * _Nullable topRightCorner, NSImage * _Nullable leftEdgeFill, NSImage * _Nullable centerFill, NSImage * _Nullable rightEdgeFill, NSImage * _Nullable bottomLeftCorner, NSImage * _Nullable bottomEdgeFill, NSImage * _Nullable bottomRightCorner, NSCompositingOperation op, CGFloat alphaFraction, BOOL flipped) API_AVAILABLE(macos(10.5));

@interface NSCell (NSDeprecated)

@property NSControlTint controlTint API_DEPRECATED("The controlTint property is not respected on 10.14 and later. For custom cells, use +[NSColor controlAccentColor] to respect the user's preferred accent color when drawing.", macos(10.0, 11.0));

// Use formatters instead.  See -[NSCell formatter] and -[NSCell setFormatter:].
- (NSInteger)entryType API_DEPRECATED("", macos(10.0,10.0));
- (void)setEntryType:(NSInteger)type API_DEPRECATED("", macos(10.0,10.0));
- (BOOL)isEntryAcceptable:(NSString *)string API_DEPRECATED("", macos(10.0,10.0));
- (void)setFloatingPointFormat:(BOOL)autoRange left:(NSUInteger)leftDigits right:(NSUInteger)rightDigits API_DEPRECATED("", macos(10.0,10.0));

/* In 10.8 and higher, all the *Mnemonic* methods are deprecated. On MacOS they have typically not been used.
 */
- (void)setMnemonicLocation:(NSUInteger)location API_DEPRECATED("", macos(10.0,10.8));
- (NSUInteger)mnemonicLocation API_DEPRECATED("", macos(10.0,10.8));
- (NSString *)mnemonic API_DEPRECATED("", macos(10.0,10.8));
- (void)setTitleWithMnemonic:(NSString *)stringWithAmpersand API_DEPRECATED("", macos(10.0,10.8));

@end

/* In some appearances, NSBackgroundStyleLight may refer to a state where the background is actually a dark color. Use NSBackgroundStyleNormal instead. */
static const NSBackgroundStyle NSBackgroundStyleLight API_DEPRECATED_WITH_REPLACEMENT("NSBackgroundStyleNormal", macos(10.5, 11.0)) = NSBackgroundStyleNormal;
/* NSBackgroundStyleDark is not a reliable indicator of background states with visually dark or saturated colors. Use NSBackgroundStyleEmphasized instead. */
static const NSBackgroundStyle NSBackgroundStyleDark API_DEPRECATED_WITH_REPLACEMENT("NSBackgroundStyleEmphasized", macos(10.5, 11.0)) = NSBackgroundStyleEmphasized;

typedef NSControlStateValue NSCellStateValue API_DEPRECATED_WITH_REPLACEMENT("NSControlStateValue", macos(10.0,10.14));
static const NSControlStateValue NSMixedState API_DEPRECATED_WITH_REPLACEMENT("NSControlStateValueMixed", macos(10.0,10.14)) = NSControlStateValueMixed;
static const NSControlStateValue NSOffState API_DEPRECATED_WITH_REPLACEMENT("NSControlStateValueOff", macos(10.0,10.14)) = NSControlStateValueOff;
static const NSControlStateValue NSOnState API_DEPRECATED_WITH_REPLACEMENT("NSControlStateValueOn", macos(10.0,10.14)) = NSControlStateValueOn;

static const NSControlSize NSRegularControlSize API_DEPRECATED_WITH_REPLACEMENT("NSControlSizeRegular", macos(10.0,10.12)) = NSControlSizeRegular;
static const NSControlSize NSSmallControlSize API_DEPRECATED_WITH_REPLACEMENT("NSControlSizeSmall", macos(10.0,10.12)) = NSControlSizeSmall;
static const NSControlSize NSMiniControlSize API_DEPRECATED_WITH_REPLACEMENT("NSControlSizeMini", macos(10.0,10.12)) = NSControlSizeMini;

APPKIT_EXTERN NSNotificationName NSControlTintDidChangeNotification API_DEPRECATED("Changes to the accent color can be manually observed by implementing -viewDidChangeEffectiveAppearance in a NSView subclass, or by Key-Value Observing the -effectiveAppearance property on NSApplication. Views are automatically redisplayed when the accent color changes.", macos(10.0, 11.0));

enum {
    NSAnyType API_DEPRECATED("Use formatters instead", macos(10.0,10.0)) = 0,
    NSIntType API_DEPRECATED("Use formatters instead", macos(10.0,10.0)) = 1,
    NSPositiveIntType API_DEPRECATED("Use formatters instead", macos(10.0,10.0)) = 2,
    NSFloatType API_DEPRECATED("Use formatters instead", macos(10.0,10.0)) = 3,
    NSPositiveFloatType API_DEPRECATED("Use formatters instead", macos(10.0,10.0)) = 4,
    NSDoubleType API_DEPRECATED("Use formatters instead", macos(10.0,10.0)) = 6,
    NSPositiveDoubleType API_DEPRECATED("Use formatters instead", macos(10.0,10.0)) = 7
};



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
