#if !__has_include(<UIFoundation/NSTextTable.h>)
/*
        NSTextTable.h
        Copyright (c) 2004-2021, Apple Inc.
        All rights reserved.

        Classes to represent text tables and other text blocks.
        NSTextBlock represents a single block of text.
        NSTextTableBlock is a subclass used to represent a block that is a cell in a table.
        NSTextTable represents a whole table.
*/

#import <AppKit/NSText.h>

@class NSTextTable;
@class NSLayoutManager;
@class NSTextContainer;

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

/* Values for NSTextBlockValueType */
typedef NS_ENUM(NSUInteger, NSTextBlockValueType) {
    NSTextBlockAbsoluteValueType    = 0,    // Absolute value in points
    NSTextBlockPercentageValueType  = 1     // Percentage value (out of 100)
};

/* Values for NSTextBlockDimension */
typedef NS_ENUM(NSUInteger, NSTextBlockDimension) {
    NSTextBlockWidth            = 0,
    NSTextBlockMinimumWidth     = 1,
    NSTextBlockMaximumWidth     = 2,
    NSTextBlockHeight           = 4,
    NSTextBlockMinimumHeight    = 5,
    NSTextBlockMaximumHeight    = 6
};

/* Values for NSTextBlockLayer */
typedef NS_ENUM(NSInteger, NSTextBlockLayer) {
    NSTextBlockPadding  = -1,
    NSTextBlockBorder   =  0,
    NSTextBlockMargin   =  1
};

/* Values for NSTextBlockVerticalAlignment */
typedef NS_ENUM(NSUInteger, NSTextBlockVerticalAlignment) {
    NSTextBlockTopAlignment         = 0,
    NSTextBlockMiddleAlignment      = 1,
    NSTextBlockBottomAlignment      = 2,
    NSTextBlockBaselineAlignment    = 3
};

/* Values for NSTextTableLayoutAlgorithm */
typedef NS_ENUM(NSUInteger, NSTextTableLayoutAlgorithm) {
    NSTextTableAutomaticLayoutAlgorithm = 0,
    NSTextTableFixedLayoutAlgorithm     = 1
};

/* NSTextBlock is the basic object for text block layout, and the superclass of the other classes. */
@interface NSTextBlock : NSObject <NSSecureCoding, NSCopying>

- (instancetype)init NS_DESIGNATED_INITIALIZER;

/* Content size */
- (void)setValue:(CGFloat)val type:(NSTextBlockValueType)type forDimension:(NSTextBlockDimension)dimension;
- (CGFloat)valueForDimension:(NSTextBlockDimension)dimension;
- (NSTextBlockValueType)valueTypeForDimension:(NSTextBlockDimension)dimension;

/* Convenience methods for content width in particular, using the above methods with dimension == NSTextBlockWidth */
- (void)setContentWidth:(CGFloat)val type:(NSTextBlockValueType)type;
@property (readonly) CGFloat contentWidth;
@property (readonly) NSTextBlockValueType contentWidthValueType;

/* Margin, border, and padding */
- (void)setWidth:(CGFloat)val type:(NSTextBlockValueType)type forLayer:(NSTextBlockLayer)layer edge:(NSRectEdge)edge;
- (void)setWidth:(CGFloat)val type:(NSTextBlockValueType)type forLayer:(NSTextBlockLayer)layer;     // Convenience method sets all edges at once
- (CGFloat)widthForLayer:(NSTextBlockLayer)layer edge:(NSRectEdge)edge;
- (NSTextBlockValueType)widthValueTypeForLayer:(NSTextBlockLayer)layer edge:(NSRectEdge)edge;

/* Alignment */
@property NSTextBlockVerticalAlignment verticalAlignment;

/* Colors */
@property (nullable, copy) NSColor *backgroundColor;
- (void)setBorderColor:(nullable NSColor *)color forEdge:(NSRectEdge)edge;
- (void)setBorderColor:(nullable NSColor *)color;        // Convenience method sets all edges at once
- (nullable NSColor *)borderColorForEdge:(NSRectEdge)edge;

/* These methods will be called during layout to determine the size and position of the block.  The first will be called before the block is laid out to determine the rect within which glyphs should be laid.  The second will be called after the block is laid out to determine the rect the block occupies with borders and margins. */
- (NSRect)rectForLayoutAtPoint:(NSPoint)startingPoint inRect:(NSRect)rect textContainer:(NSTextContainer *)textContainer characterRange:(NSRange)charRange;
- (NSRect)boundsRectForContentRect:(NSRect)contentRect inRect:(NSRect)rect textContainer:(NSTextContainer *)textContainer characterRange:(NSRange)charRange;

/* This method will be called during drawing to draw any colors and other decorations before the text is drawn. */
- (void)drawBackgroundWithFrame:(NSRect)frameRect inView:(NSView *)controlView characterRange:(NSRange)charRange layoutManager:(NSLayoutManager *)layoutManager;

@end

/* NSTextTableBlock is a subclass of NSTextBlock used for a block that appears as a cell in a text table. */
@interface NSTextTableBlock : NSTextBlock

- (instancetype)initWithTable:(NSTextTable *)table startingRow:(NSInteger)row rowSpan:(NSInteger)rowSpan startingColumn:(NSInteger)col columnSpan:(NSInteger)colSpan NS_DESIGNATED_INITIALIZER;     // Designated initializer

/* These methods determine the block's role in its enclosing table. */
@property (readonly, strong) NSTextTable *table;
@property (readonly) NSInteger startingRow;
@property (readonly) NSInteger rowSpan;
@property (readonly) NSInteger startingColumn;
@property (readonly) NSInteger columnSpan;

@end

/* NSTextTable represents a table as a whole. */
@interface NSTextTable : NSTextBlock

/* These methods control the basic parameters of the table. */
@property NSUInteger numberOfColumns;
@property NSTextTableLayoutAlgorithm layoutAlgorithm;
@property BOOL collapsesBorders;
@property BOOL hidesEmptyCells;

/* An NSTextTableBlock delegates its layout and drawing operations to its enclosing table, using the following NSTextTable methods. */
- (NSRect)rectForBlock:(NSTextTableBlock *)block layoutAtPoint:(NSPoint)startingPoint inRect:(NSRect)rect textContainer:(NSTextContainer *)textContainer characterRange:(NSRange)charRange;
- (NSRect)boundsRectForBlock:(NSTextTableBlock *)block contentRect:(NSRect)contentRect inRect:(NSRect)rect textContainer:(NSTextContainer *)textContainer characterRange:(NSRange)charRange;
- (void)drawBackgroundForBlock:(NSTextTableBlock *)block withFrame:(NSRect)frameRect inView:(NSView *)controlView characterRange:(NSRange)charRange layoutManager:(NSLayoutManager *)layoutManager;

@end



#endif // !TARGET_OS_IPHONE

NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSTextTable.h>
#endif
