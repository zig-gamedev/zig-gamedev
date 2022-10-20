/*
    NSLevelIndicatorCell.h
    Application Kit
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSActionCell.h>
#import <AppKit/NSSliderCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSUInteger, NSLevelIndicatorStyle) {
    NSLevelIndicatorStyleRelevancy,
    NSLevelIndicatorStyleContinuousCapacity,
    NSLevelIndicatorStyleDiscreteCapacity,
    NSLevelIndicatorStyleRating
};

@interface NSLevelIndicatorCell : NSActionCell

- (instancetype)initWithLevelIndicatorStyle:(NSLevelIndicatorStyle)levelIndicatorStyle;

@property NSLevelIndicatorStyle levelIndicatorStyle;

@property double minValue;

@property double maxValue;

@property double warningValue;

@property double criticalValue;

@property NSTickMarkPosition tickMarkPosition;

@property NSInteger numberOfTickMarks;

@property NSInteger numberOfMajorTickMarks;

- (NSRect)rectOfTickMarkAtIndex:(NSInteger)index;
- (double)tickMarkValueAtIndex:(NSInteger)index;

@end

static const NSLevelIndicatorStyle NSRelevancyLevelIndicatorStyle API_DEPRECATED_WITH_REPLACEMENT("NSLevelIndicatorStyleRelevancy", macos(10.0,10.14)) = NSLevelIndicatorStyleRelevancy;
static const NSLevelIndicatorStyle NSContinuousCapacityLevelIndicatorStyle API_DEPRECATED_WITH_REPLACEMENT("NSLevelIndicatorStyleContinuousCapacity", macos(10.0,10.14)) = NSLevelIndicatorStyleContinuousCapacity;
static const NSLevelIndicatorStyle NSDiscreteCapacityLevelIndicatorStyle API_DEPRECATED_WITH_REPLACEMENT("NSLevelIndicatorStyleDiscreteCapacity", macos(10.0,10.14)) = NSLevelIndicatorStyleDiscreteCapacity;
static const NSLevelIndicatorStyle NSRatingLevelIndicatorStyle API_DEPRECATED_WITH_REPLACEMENT("NSLevelIndicatorStyleRating", macos(10.0,10.14)) = NSLevelIndicatorStyleRating;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

