/*
    NSTextFieldCell.h
    Application Kit
    Copyright (c) 1994-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSActionCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSUInteger, NSTextFieldBezelStyle) {
    NSTextFieldSquareBezel  = 0,
    NSTextFieldRoundedBezel = 1
};

@class NSColor;

@interface NSTextFieldCell : NSActionCell

- (instancetype)initTextCell:(NSString *)string NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)initImageCell:(nullable NSImage *)image NS_UNAVAILABLE; // Use the designated initializer initTextCell:

@property (nullable, copy) NSColor *backgroundColor;
@property BOOL drawsBackground;
@property (nullable, copy) NSColor *textColor;
- (NSText *)setUpFieldEditorAttributes:(NSText *)textObj;

@property NSTextFieldBezelStyle bezelStyle;

@property (nullable, copy) NSString *placeholderString;
@property (nullable, copy) NSAttributedString *placeholderAttributedString;

- (void)setWantsNotificationForMarkedText:(BOOL)flag API_AVAILABLE(macos(10.5));

/* Returns an array of locale identifiers representing keyboard input sources allowed to be enabled when the receiver has the keyboard focus.
 */
@property (nullable, copy) NSArray<NSString *> *allowedInputSourceLocales API_AVAILABLE(macos(10.5));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

