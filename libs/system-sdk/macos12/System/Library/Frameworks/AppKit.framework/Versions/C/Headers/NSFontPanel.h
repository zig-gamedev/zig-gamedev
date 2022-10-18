/*
	NSFontPanel.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSPanel.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSFontPanel, NSMutableDictionary, NSFontManager, NSMutableArray, NSTableView, NSFontDescriptor, NSFont;


typedef NS_OPTIONS(NSUInteger, NSFontPanelModeMask) {
    NSFontPanelModeMaskFace = 1 << 0,
    NSFontPanelModeMaskSize = 1 << 1,
    NSFontPanelModeMaskCollection = 1 << 2,
    NSFontPanelModeMaskUnderlineEffect = 1<<8,
    NSFontPanelModeMaskStrikethroughEffect = 1<<9,
    NSFontPanelModeMaskTextColorEffect = 1<< 10,
    NSFontPanelModeMaskDocumentColorEffect = 1<<11,
    NSFontPanelModeMaskShadowEffect = 1<<12,
    NSFontPanelModeMaskAllEffects = 0XFFF00,
    NSFontPanelModesMaskStandardModes = 0xFFFF,
    NSFontPanelModesMaskAllModes = 0xFFFFFFFF
};

@protocol NSFontChanging <NSObject>
@optional

/* This is the message that's propagated up the responder chain.
 */
- (void)changeFont:(nullable NSFontManager *)sender;
- (NSFontPanelModeMask)validModesForFontPanel:(NSFontPanel *)fontPanel;
@end

#if __swift__ < 40200
@interface NSObject (NSFontPanelValidationAdditions)
- (NSFontPanelModeMask)validModesForFontPanel:(NSFontPanel *)fontPanel API_DEPRECATED("This is now an optional method of the NSFontChanging protocol.", macos(10.0, 11.0));
@end
#endif

@interface NSFontPanel : NSPanel

@property (class, readonly, strong) NSFontPanel *sharedFontPanel;
@property (class, readonly) BOOL sharedFontPanelExists;

@property (nullable, strong) NSView *accessoryView;
- (void)setPanelFont:(NSFont *)fontObj isMultiple:(BOOL)flag;
- (NSFont *)panelConvertFont:(NSFont *)fontObj;
@property BOOL worksWhenModal;
@property (getter=isEnabled) BOOL enabled;

/* This method triggers a re-load to the default state, so that the delegate will be called, and have an opportunity to scrutinize the default list of fonts to be displayed in the panel.
*/
- (void) reloadDefaultFontFamilies;

@end


// The following enum items are deprecated. Use NSFontPanelModeMask instead
enum {
    NSFontPanelFaceModeMask = 1 << 0,
    NSFontPanelSizeModeMask = 1 << 1,
    NSFontPanelCollectionModeMask = 1 << 2,
    NSFontPanelUnderlineEffectModeMask = 1<<8,
    NSFontPanelStrikethroughEffectModeMask = 1<<9,
    NSFontPanelTextColorEffectModeMask = 1<< 10,
    NSFontPanelDocumentColorEffectModeMask = 1<<11,
    NSFontPanelShadowEffectModeMask = 1<<12,
    NSFontPanelAllEffectsModeMask = 0XFFF00,
    NSFontPanelStandardModesMask = 0xFFFF,
    NSFontPanelAllModesMask = 0xFFFFFFFF
};

/* Tags of views in the FontPanel
 THESE ARE ALL OBSOLETE and should not be used.
 */
enum {
    NSFPPreviewButton			= 131,
    NSFPRevertButton			= 130,
    NSFPSetButton			= 132,
    NSFPPreviewField			= 128,
    NSFPSizeField			= 129,
    NSFPSizeTitle			= 133,
    NSFPCurrentField			= 134
} API_DEPRECATED("", macos(10.0,10.0));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
