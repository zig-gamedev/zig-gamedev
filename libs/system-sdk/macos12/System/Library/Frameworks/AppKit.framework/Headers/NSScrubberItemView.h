/*
 NSScrubberItemView.h
 Application Kit
 Copyright (c) 2016-2021, Apple Inc.
 All rights reserved.
 */

#import <AppKit/NSImageCell.h>
#import <AppKit/NSView.h>
#import <AppKit/AppKitDefines.h>
#import <os/lock.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSScrubberLayoutAttributes, NSTextField, NSImageView;

#pragma mark - Arranged View

API_AVAILABLE(macos(10.12.2))
@interface NSScrubberArrangedView : NSView

@property (getter=isSelected)    BOOL selected;
@property (getter=isHighlighted) BOOL highlighted;

- (void)applyLayoutAttributes:(__kindof NSScrubberLayoutAttributes *)layoutAttributes NS_REQUIRES_SUPER;

@end

#pragma mark - Selection View

/*!
 @class NSScrubberSelectionView
 @abstract The base view class for all selection decorations used by the @c NSScrubber control.
 */
API_AVAILABLE(macos(10.12.2))
@interface NSScrubberSelectionView : NSScrubberArrangedView
@end

#pragma mark - Item Views

/*!
 @class NSScrubberItemView
 @abstract The base view class that is arranged by a @c NSScrubber control.
 */
API_AVAILABLE(macos(10.12.2))
@interface NSScrubberItemView : NSScrubberArrangedView

@end

#pragma mark Text Item

/*!
 @class NSScrubberTextItemView
 @abstract A simple @c NSScrubberItemView for displaying text. The -fittingSize method can be used to measure the smallest size for the view which fits the title without truncating.
 */
API_AVAILABLE(macos(10.12.2))
@interface NSScrubberTextItemView : NSScrubberItemView

@property (strong, readonly) NSTextField *textField;
@property (copy) NSString *title;

@end

#pragma mark Image Item

/*!
 @class NSScrubberTextItemView
 @abstract A simple @c NSScrubberItemView for displaying an image. 
 @discussion If the provided image is larger than the view's frame, it is scaled proportionally to fill the entire frame. The cropped portion of the image is determined by the @c imageAlignment property.
 */
API_AVAILABLE(macos(10.12.2))
@interface NSScrubberImageItemView : NSScrubberItemView

@property (strong, readonly) NSImageView *imageView;
@property (copy) NSImage *image;
@property NSImageAlignment imageAlignment;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
