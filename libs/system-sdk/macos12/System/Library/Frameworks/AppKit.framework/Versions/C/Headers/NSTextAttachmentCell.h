/*
        NSTextAttachmentCell.h
        Copyright (c) 2021-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSCell.h>

@class NSTextAttachment;
@class NSLayoutManager;
@class NSTextContainer;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

// These are the only methods required of cells in text attachments... The default NSCell class implements most of these; the NSTextAttachmentCell class is a subclass which implements all and provides some additional functionality.
API_UNAVAILABLE(macCatalyst)
@protocol NSTextAttachmentCell <NSObject>
- (void)drawWithFrame:(NSRect)cellFrame inView:(nullable NSView *)controlView;
- (BOOL)wantsToTrackMouse;
- (void)highlight:(BOOL)flag withFrame:(NSRect)cellFrame inView:(nullable NSView *)controlView;
- (BOOL)trackMouse:(NSEvent *)theEvent inRect:(NSRect)cellFrame ofView:(nullable NSView *)controlView untilMouseUp:(BOOL)flag;
- (NSSize)cellSize;
- (NSPoint)cellBaselineOffset;
@property (nullable, assign) NSTextAttachment *attachment;

// Sophisticated cells should implement these in addition to the simpler methods, above.  The class NSTextAttachmentCell implements them to simply call the simpler methods; more complex conformers should implement the simpler methods to call these.
- (void)drawWithFrame:(NSRect)cellFrame inView:(nullable NSView *)controlView characterIndex:(NSUInteger)charIndex;
- (void)drawWithFrame:(NSRect)cellFrame inView:(nullable NSView *)controlView characterIndex:(NSUInteger)charIndex layoutManager:(NSLayoutManager *)layoutManager;
- (BOOL)wantsToTrackMouseForEvent:(NSEvent *)theEvent inRect:(NSRect)cellFrame ofView:(nullable NSView *)controlView atCharacterIndex:(NSUInteger)charIndex;
- (BOOL)trackMouse:(NSEvent *)theEvent inRect:(NSRect)cellFrame ofView:(nullable NSView *)controlView atCharacterIndex:(NSUInteger)charIndex untilMouseUp:(BOOL)flag;
- (NSRect)cellFrameForTextContainer:(NSTextContainer *)textContainer proposedLineFragment:(NSRect)lineFrag glyphPosition:(NSPoint)position characterIndex:(NSUInteger)charIndex;
@end


// Simple class to provide basic attachment cell functionality. By default this class causes NSTextView to send out delegate messages when the attachment is clicked on or dragged.
@interface NSTextAttachmentCell : NSCell <NSTextAttachmentCell>
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
