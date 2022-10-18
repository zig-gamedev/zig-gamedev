/*
	NSColorPicker.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSColorPicking.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@interface NSColorPicker : NSObject <NSColorPickingDefault>
- (nullable instancetype)initWithPickerMask:(NSUInteger)mask colorPanel:(NSColorPanel *)owningColorPanel;
@property (readonly, strong) NSColorPanel *colorPanel;
@property (readonly, strong) NSImage *provideNewButtonImage;
- (void)insertNewButtonImage:(NSImage *)newButtonImage in:(NSButtonCell *)buttonCell;
- (void)viewSizeChanged:(nullable id)sender;
- (void)attachColorList:(NSColorList *)colorList;
- (void)detachColorList:(NSColorList *)colorList;
- (void)setMode:(NSColorPanelMode)mode;
// The default implementation of buttonToolTip returns the current classname
@property (readonly, copy) NSString *buttonToolTip;
// The default implementation of minContentSize will look at your view's Autosizing behavior and do the right thing based on that.
@property (readonly) NSSize minContentSize;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
