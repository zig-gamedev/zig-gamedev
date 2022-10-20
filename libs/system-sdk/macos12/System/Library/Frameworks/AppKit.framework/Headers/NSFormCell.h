/*
	NSFormCell.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSActionCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSFormCell : NSActionCell

- (instancetype)initTextCell:(nullable NSString *)string NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)initImageCell:(nullable NSImage *)image NS_UNAVAILABLE;

- (CGFloat)titleWidth:(NSSize)size;
@property CGFloat titleWidth;
@property (copy) NSString *title;
@property (strong) NSFont *titleFont;
@property (getter=isOpaque, readonly) BOOL opaque;

@property (nullable, copy) NSString *placeholderString;
@property (nullable, copy) NSAttributedString *placeholderAttributedString;

#if !TARGET_OS_IPHONE
@property NSTextAlignment titleAlignment;
@property NSWritingDirection titleBaseWritingDirection;
#endif

/* Determine the preferred width of the text field portion of the receiver. The preferred width is reflected in the cell's cellSize, which will be large enough to accommodate the title, bezel, and a text field of width preferredTextWidth. It is also reflected in the intrinsicContentSize of the NSForm. That is, under autolayout, the NSForm will try to size itself so that the text field cell is the given width, according to the usual content size constraint priorities.
 
 If the width is negative, the cellSize matches the historic behavior, which is that it is large enough to accommodate the title, bezel, and the current text. The default is -1.
 
 This method can aid migration to autolayout, and is sufficient for simple cases. However, for new apps, prefer to use NSTextFields directly instead of NSForm.
 */
@property CGFloat preferredTextFieldWidth API_AVAILABLE(macos(10.8));

@end

@interface NSFormCell(NSKeyboardUI)
/* Deprecated in 10.8 and higher. Use setTitle: instead.
 */
- (void)setTitleWithMnemonic:(null_unspecified NSString *)stringWithAmpersand API_DEPRECATED("", macos(10.0,10.8));
@end

@interface NSFormCell(NSFormCellAttributedStringMethods)
@property (copy) NSAttributedString *attributedTitle;
@end



API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
