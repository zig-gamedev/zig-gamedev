/*
	NSSegmentedControl.h
	Application Kit
	Copyright (c) 2003-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSCell.h>
#import <AppKit/NSUserInterfaceCompression.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSImage;

typedef NS_ENUM(NSUInteger, NSSegmentSwitchTracking) {
    NSSegmentSwitchTrackingSelectOne = 0,  // only one button can be selected
    NSSegmentSwitchTrackingSelectAny = 1,  // any button can be selected
    NSSegmentSwitchTrackingMomentary = 2,  // only selected while tracking
    NSSegmentSwitchTrackingMomentaryAccelerator API_AVAILABLE(macos(10.10.3)) = 3, // accelerator behavior, only selected while tracking
};

typedef NS_ENUM(NSInteger, NSSegmentStyle) {
    NSSegmentStyleAutomatic = 0, //Appearance is chosen depending on the type of window and the control's position inside it
    NSSegmentStyleRounded = 1,
    NSSegmentStyleRoundRect = 3,
    NSSegmentStyleTexturedSquare = 4,
    NSSegmentStyleSmallSquare = 6,
    NSSegmentStyleSeparated API_AVAILABLE(macos(10.10)) = 8,
    
    /* In Mac OS X 10.10 and later, the following styles have identical artwork */
    NSSegmentStyleTexturedRounded API_AVAILABLE(macos(10.5)) = 2,
    NSSegmentStyleCapsule API_AVAILABLE(macos(10.5)) = 5
} API_AVAILABLE(macos(10.5));

typedef NS_ENUM(NSInteger, NSSegmentDistribution) {
    /// Dynamically sized segments will be sized to fit their contents, any remaining space will be left blank. This style is equivalent to the way segments were distributed on older systems.
    NSSegmentDistributionFit = 0,
    
    /// Dynamically sized segments will be sized to fill the available space, with extra space being distributed equally among them. Default value.
    NSSegmentDistributionFill,
    
    /// Dynamically sized segments will be sized to fill the available space, and kept the same size as each other.
    NSSegmentDistributionFillEqually,
    
    /// Dynamically sized segments will be sized to fill the available space, and kept proportional to their fitting size.
    NSSegmentDistributionFillProportionally,
    
} API_AVAILABLE(macos(10.13));

@interface NSSegmentedControl : NSControl <NSUserInterfaceCompression>

@property NSInteger segmentCount;

@property NSInteger selectedSegment;

- (BOOL)selectSegmentWithTag:(NSInteger)tag;

- (void)setWidth:(CGFloat)width forSegment:(NSInteger)segment;
- (CGFloat)widthForSegment:(NSInteger)segment;

- (void)setImage:(nullable NSImage *)image forSegment:(NSInteger)segment;
- (nullable NSImage *)imageForSegment:(NSInteger)segment;

- (void)setImageScaling:(NSImageScaling)scaling forSegment:(NSInteger)segment API_AVAILABLE(macos(10.5));
- (NSImageScaling)imageScalingForSegment:(NSInteger)segment API_AVAILABLE(macos(10.5));

- (void)setLabel:(NSString *)label forSegment:(NSInteger)segment;
- (nullable NSString *)labelForSegment:(NSInteger)segment;

- (void)setMenu:(nullable NSMenu *)menu forSegment:(NSInteger)segment;
- (nullable NSMenu *)menuForSegment:(NSInteger)segment;

- (void)setSelected:(BOOL)selected forSegment:(NSInteger)segment;
- (BOOL)isSelectedForSegment:(NSInteger)segment;

- (void)setEnabled:(BOOL)enabled forSegment:(NSInteger)segment;
- (BOOL)isEnabledForSegment:(NSInteger)segment;

- (void)setToolTip:(nullable NSString *)toolTip forSegment:(NSInteger)segment API_AVAILABLE(macos(10.13));
- (nullable NSString *)toolTipForSegment:(NSInteger)segment API_AVAILABLE(macos(10.13));

- (void)setTag:(NSInteger)tag forSegment:(NSInteger)segment API_AVAILABLE(macos(10.13));
- (NSInteger)tagForSegment:(NSInteger)segment API_AVAILABLE(macos(10.13));

- (void)setShowsMenuIndicator:(BOOL)showsMenuIndicator forSegment:(NSInteger)segment API_AVAILABLE(macos(10.13));
- (BOOL)showsMenuIndicatorForSegment:(NSInteger)segment API_AVAILABLE(macos(10.13));

@property NSSegmentStyle segmentStyle API_AVAILABLE(macos(10.5));

@property (getter=isSpringLoaded) BOOL springLoaded API_AVAILABLE(macos(10.10.3)); // sends action on deep-press or extended hover while dragging. Defaults to NO.

@property NSSegmentSwitchTracking trackingMode API_AVAILABLE(macos(10.10.3));

/*  This message is valid only for trackingMode=NSSegmentSwitchTrackingMomentaryAccelerator and provides the double value for the selected segment.
 */
@property (readonly) double doubleValueForSelectedSegment API_AVAILABLE(macos(10.10.3));

@property (nullable, copy) NSColor *selectedSegmentBezelColor API_AVAILABLE(macos(10.12.2)); // The color of the selected segment's bevel, in appearances that support it

@property (readonly) NSInteger indexOfSelectedItem API_AVAILABLE(macos(10.4)); // same as selectedSegment, but useful for connecting a segmented control to -[NSTabView takeSelectedTabViewItemFromSender:]

#if !TARGET_OS_IPHONE
- (void)setAlignment:(NSTextAlignment)alignment forSegment:(NSInteger)segment API_AVAILABLE(macos(10.13)); // Controls the content alignment of the specified segment. Defaults to NSTextAlignmentCenter and results in the standard centered cluster of content
- (NSTextAlignment)alignmentForSegment:(NSInteger)segment API_AVAILABLE(macos(10.13));
#endif

@property NSSegmentDistribution segmentDistribution API_AVAILABLE(macos(10.13)); // Defaults to NSSegmentDistributionFill on 10.13, older systems will continue to behave similarly to NSSegmentDistributionFit

- (void)compressWithPrioritizedCompressionOptions:(NSArray<NSUserInterfaceCompressionOptions *> *)prioritizedOptions API_AVAILABLE(macos(10.13));
- (NSSize)minimumSizeWithPrioritizedCompressionOptions:(NSArray<NSUserInterfaceCompressionOptions *> *)prioritizedOptions API_AVAILABLE(macos(10.13));
@property (readonly, copy) NSUserInterfaceCompressionOptions *activeCompressionOptions API_AVAILABLE(macos(10.13));

@end

@interface NSSegmentedControl (NSSegmentedControlConvenience)

/*!
 Creates a standard segmented control containing one segment for each of the provided labels.
 @param labels An array of localized label strings to use for the control's segments.
 @param trackingMode The selection mode for the control. The NSSegmentSwitchTracking enum describes the possible values and their effects.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized segmented control.
 */
+ (instancetype)segmentedControlWithLabels:(NSArray<NSString *> *)labels trackingMode:(NSSegmentSwitchTracking)trackingMode target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

/*!
 Creates a standard segmented control containing one segment for each of the provided images. To ensure accessibility for this control, set the accessibilityDescription property on each of the provided images.
 @param images An array of image objects to use for the control's segments.
 @param trackingMode The selection mode for the control. The NSSegmentSwitchTracking enum describes the possible values and their effects.
 @param target The target object that receives action messages from the control.
 @param action The action message sent by the control.
 @return An initialized segmented control.
 */
+ (instancetype)segmentedControlWithImages:(NSArray<NSImage *> *)images trackingMode:(NSSegmentSwitchTracking)trackingMode target:(nullable id)target action:(nullable SEL)action API_AVAILABLE(macos(10.12));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
