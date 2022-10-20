/*
    NSLevelIndicator.h
    Application Kit
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSLevelIndicatorCell.h>
#import <AppKit/AppKitDefines.h>

APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSInteger, NSLevelIndicatorPlaceholderVisibility) {
    /* Placeholders are always displayed for standalone indicators. For level indicators inside a table view, placeholders draw only in the selected row and while the control is being edited. */
    NSLevelIndicatorPlaceholderVisibilityAutomatic = 0,
    
    /* Always display placeholders */
    NSLevelIndicatorPlaceholderVisibilityAlways = 1,
    
    /* Only display placeholders while the control is being edited */
    NSLevelIndicatorPlaceholderVisibilityWhileEditing = 2
};

@interface NSLevelIndicator : NSControl {
}

@property NSLevelIndicatorStyle levelIndicatorStyle API_AVAILABLE(macos(10.10));

@property (getter=isEditable) BOOL editable API_AVAILABLE(macos(10.13));

#pragma mark Value Thresholds

@property double minValue;

@property double maxValue;

@property double warningValue;

@property double criticalValue;

#pragma mark Tick Marks

@property NSTickMarkPosition tickMarkPosition;

@property NSInteger numberOfTickMarks;

@property NSInteger numberOfMajorTickMarks;

- (double)tickMarkValueAtIndex:(NSInteger)index;
- (NSRect)rectOfTickMarkAtIndex:(NSInteger)index;

#pragma mark Custom Colors

/*! Sets the fill color used by Continuous and Discrete Capacity indicators when drawing the "normal" state, and by the Rating indicator when drawing stars. The default value is a system-defined color which may vary between level indicator styles and OS releases. */
@property (copy, null_resettable) NSColor *fillColor API_AVAILABLE(macos(10.13));

/*! Sets the fill color used by Continuous and Discrete Capacity indicators when drawing values above the "warning" threshold. The default value is a system-defined color which may vary between level indicator styles and OS releases. */
@property (copy, null_resettable) NSColor *warningFillColor API_AVAILABLE(macos(10.13));

/*! Sets the fill color used by Continuous and Discrete Capacity indicators when drawing values above the "critical" threshold. The default value is a system-defined color which may vary between level indicator styles and OS releases. */
@property (copy, null_resettable) NSColor *criticalFillColor API_AVAILABLE(macos(10.13));

#pragma mark Capacity Indicator Options

@property BOOL drawsTieredCapacityLevels API_AVAILABLE(macos(10.13));

#pragma mark Rating Indicator Options

/*! For a Rating-style indicator, sets the conditions under which Rating placeholders are displayed. This property currently has no effect for other indicator styles. The default value is \c NSLevelIndicatorPlaceholderVisibilityAutomatic. */
@property NSLevelIndicatorPlaceholderVisibility placeholderVisibility API_AVAILABLE(macos(10.13));

/*! If non-nil, sets the image used by the Rating indicator style in place of the default star image. The default value is nil. */
@property (strong, nullable) NSImage *ratingImage API_AVAILABLE(macos(10.13));

/*! If non-nil, sets the image used by the Rating indicator style in place of the default faded placeholder image. The default value is nil.
 
 If the custom placeholder is a template image, its fill opacity can be adjusted by modifying the opacity of the template image.
 
 If both a ratingImage and ratingPlaceholderImage are set, each rating position is sized such that either image will fit without scaling (i.e. sized to the maximum width and height of both images). */
@property (strong, nullable) NSImage *ratingPlaceholderImage API_AVAILABLE(macos(10.13));

@end

API_UNAVAILABLE_END
