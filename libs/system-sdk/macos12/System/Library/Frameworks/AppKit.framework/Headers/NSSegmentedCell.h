/*
    NSSegmentedCell.h
    Application Kit
    Copyright (c) 2003-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSGeometry.h>
#import <AppKit/NSActionCell.h>
#import <AppKit/NSSegmentedControl.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSMutableArray;

@interface NSSegmentedCell : NSActionCell

/* Number of segments
*/
@property NSInteger segmentCount;

/* Which button is active. May turn off other segments depending on mode.
*/
@property NSInteger selectedSegment;

- (BOOL)selectSegmentWithTag:(NSInteger)tag;

/* For keyboard UI. Wraps.
*/
- (void)makeNextSegmentKey;
- (void)makePreviousSegmentKey;

@property NSSegmentSwitchTracking trackingMode;

// per segment info

/* Width of 0 means autosize to fit
*/
- (void)setWidth:(CGFloat)width forSegment:(NSInteger)segment;
- (CGFloat)widthForSegment:(NSInteger)segment;

- (void)setImage:(nullable NSImage *)image forSegment:(NSInteger)segment;
- (nullable NSImage *)imageForSegment:(NSInteger)segment;


- (void)setImageScaling:(NSImageScaling)scaling forSegment:(NSInteger)segment API_AVAILABLE(macos(10.5));
- (NSImageScaling)imageScalingForSegment:(NSInteger)segment API_AVAILABLE(macos(10.5));


- (void)setLabel:(NSString *)label forSegment:(NSInteger)segment;
- (nullable NSString *)labelForSegment:(NSInteger)segment;

- (void)setSelected:(BOOL)selected forSegment:(NSInteger)segment;
- (BOOL)isSelectedForSegment:(NSInteger)segment;

- (void)setEnabled:(BOOL)enabled forSegment:(NSInteger)segment;
- (BOOL)isEnabledForSegment:(NSInteger)segment;

- (void)setMenu:(nullable NSMenu *)menu forSegment:(NSInteger)segment;
- (nullable NSMenu *)menuForSegment:(NSInteger)segment;

- (void)setToolTip:(nullable NSString *)toolTip forSegment:(NSInteger)segment;
- (nullable NSString *)toolTipForSegment:(NSInteger)segment;

- (void)setTag:(NSInteger)tag forSegment:(NSInteger)segment;
- (NSInteger)tagForSegment:(NSInteger)segment;

/* see NSSegmentedControl.h for segment style names and values */
@property NSSegmentStyle segmentStyle API_AVAILABLE(macos(10.5));

/* For custom content drawing. frame has been adjusted to content area
*/
- (void)drawSegment:(NSInteger)segment inFrame:(NSRect)frame withView:(NSView *)controlView;

@end

@interface NSSegmentedCell (NSSegmentBackgroundStyle)
/* Describes the surface drawn onto in -[NSCell drawSegment:inFrame:withView:]. That method draws a segment interior, not the segment bezel.  This is both an override point and a useful method to call. A segmented cell that draws a custom bezel would override this to describe that surface. A cell that has custom segment drawing might query this method to help pick an image that looks good on the cell. Calling this method gives you some independence from changes in framework art style.
*/
- (NSBackgroundStyle)interiorBackgroundStyleForSegment:(NSInteger)segment API_AVAILABLE(macos(10.5));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

