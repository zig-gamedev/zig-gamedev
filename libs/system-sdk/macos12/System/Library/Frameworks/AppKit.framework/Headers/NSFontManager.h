/*
	NSFontManager.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSMenu.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSFont, NSFontPanel, NSMenu, NSFontDescriptor;

/*
 * Font Traits
 *
 * This list should be kept small since the more traits that are assigned
 * to a given font, the harder it will be to map it to some other family.
 * Some traits are mutually exclusive such as NSExpanded and NSCondensed.
 */
typedef NS_OPTIONS(NSUInteger, NSFontTraitMask) {
    NSItalicFontMask			= 0x00000001,
    NSBoldFontMask			= 0x00000002,
    NSUnboldFontMask			= 0x00000004,
    NSNonStandardCharacterSetFontMask	= 0x00000008,
    NSNarrowFontMask			= 0x00000010,
    NSExpandedFontMask			= 0x00000020,
    NSCondensedFontMask			= 0x00000040,
    NSSmallCapsFontMask			= 0x00000080,
    NSPosterFontMask			= 0x00000100,
    NSCompressedFontMask		= 0x00000200,
    NSFixedPitchFontMask		= 0x00000400,
    NSUnitalicFontMask			= 0x01000000
};


typedef NS_OPTIONS(NSUInteger, NSFontCollectionOptions) {
   NSFontCollectionApplicationOnlyMask = 1 << 0
};


/* And these "actions" are really tag values in Font Menu cells which send any of the action messages listed above.  Normally, they're pre-defined in the font panel.
 */
typedef NS_ENUM(NSUInteger, NSFontAction) {
    NSNoFontChangeAction		= 0,
    NSViaPanelFontAction		= 1,
    NSAddTraitFontAction		= 2,
    NSSizeUpFontAction			= 3,
    NSSizeDownFontAction		= 4,
    NSHeavierFontAction			= 5,
    NSLighterFontAction			= 6,
    NSRemoveTraitFontAction		= 7
};

@interface NSFontManager : NSObject <NSMenuItemValidation>

+ (void)setFontPanelFactory:(nullable Class)factoryId;
+ (void)setFontManagerFactory:(nullable Class)factoryId;
@property (class, readonly, strong) NSFontManager *sharedFontManager;

@property (getter=isMultiple, readonly) BOOL multiple;
@property (nullable, readonly, strong) NSFont *selectedFont;
- (void)setSelectedFont:(NSFont *)fontObj isMultiple:(BOOL)flag;
- (void)setFontMenu:(NSMenu *)newMenu;
- (nullable NSMenu *)fontMenu:(BOOL)create;
- (nullable NSFontPanel *)fontPanel:(BOOL)create;
- (nullable NSFont *)fontWithFamily:(NSString *)family traits:(NSFontTraitMask)traits weight:(NSInteger)weight size:(CGFloat)size;
- (NSFontTraitMask)traitsOfFont:(NSFont *)fontObj;
- (NSInteger)weightOfFont:(NSFont *)fontObj;

/* Three methods for supporting app font management.  The fonts and names used/returned by these functions are the same as those used by the NSFontPanel.  The third method below takes as input a name as returned by "availableFontFamilies" and returns an NSArray of NSArrays.  The elements of the "inner" arrays are: (0) the font's name, (1) non-family part of the name as used by NSFontPanel, (2) an NSNumber indicating the weight, and (3) an NSNumber indicating the traits.
*/
@property (readonly, copy) NSArray<NSString *> *availableFonts;
@property (readonly, copy) NSArray<NSString *> *availableFontFamilies;
- (nullable NSArray<NSArray *> *)availableMembersOfFontFamily:(NSString *)fam;

- (NSFont *)convertFont:(NSFont *)fontObj;
- (NSFont *)convertFont:(NSFont *)fontObj toSize:(CGFloat)size;
- (nullable NSFont *)convertFont:(NSFont *)fontObj toFace:(NSString *)typeface;
- (NSFont *)convertFont:(NSFont *)fontObj toFamily:(NSString *)family;
- (NSFont *)convertFont:(NSFont *)fontObj toHaveTrait:(NSFontTraitMask)trait;
- (NSFont *)convertFont:(NSFont *)fontObj toNotHaveTrait:(NSFontTraitMask)trait;
- (NSFont *)convertWeight:(BOOL)upFlag ofFont:(NSFont *)fontObj;
@property (getter=isEnabled) BOOL enabled;
@property SEL action;
@property (nullable, assign) id delegate API_DEPRECATED("NSFontManager doesn't have any delegate method. This property should not be used.", macos(10.0,10.11));


- (BOOL)sendAction;

- (NSString *) localizedNameForFamily:(NSString *)family face:(nullable NSString *)faceKey;

- (void)setSelectedAttributes:(NSDictionary<NSString *, id> *)attributes isMultiple:(BOOL)flag;
- (NSDictionary<NSString *, id> *)convertAttributes:(NSDictionary<NSString *, id> *)attributes;

- (nullable NSArray *)availableFontNamesMatchingFontDescriptor:(NSFontDescriptor *)descriptor API_DEPRECATED("Use -[NSFontDescriptor matchingFontDescriptorsWithMandatoryKeys:] instead", macos(10.0,10.11));

@property (readonly, copy) NSArray *collectionNames API_DEPRECATED("Use +[NSFontCollection allFontCollectionNames] instead", macos(10.0,10.11));
- (nullable NSArray *)fontDescriptorsInCollection:(NSString *)collectionNames API_DEPRECATED("Use -[NSFontCollection matchingDescriptors] instead", macos(10.0,10.11));
- (BOOL)addCollection:(NSString *)collectionName options:(NSFontCollectionOptions)collectionOptions API_DEPRECATED("Use +[NSFontCollection showFontCollection:withName:visibility:name:] instead", macos(10.0,10.11));
- (BOOL)removeCollection:(NSString *)collectionName API_DEPRECATED("Use +[NSFontCollection hideFontCollectionWithName:visibility:error:] instead", macos(10.0,10.11));

- (void)addFontDescriptors:(NSArray *)descriptors  toCollection:(NSString *)collectionName API_DEPRECATED("Use -[NSMutableFontCollection addQueryForDescriptors:] instead", macos(10.0,10.11));
- (void)removeFontDescriptor:(NSFontDescriptor *)descriptor fromCollection:(NSString *)collection API_DEPRECATED("Use -[NSMutableFontCollection removeQueryForDescriptors:] instead", macos(10.0,10.11));

/* Returns the current font action used by -convertFont:. This method is intended to be invoked to query the font conversion action while the action message (usually -changeFont:) is being invoked.
 */
@property (readonly) NSFontAction currentFontAction API_AVAILABLE(macos(10.5));

/* Converts fontTraits to a new traits mask value just as -convertFont:. This method is intended to be invoked to query the font traits while the action message (usually -changeFont:) is being invoked when the current font action is either NSAddTraitFontAction or NSRemoveTraitFontAction.
 */
- (NSFontTraitMask)convertFontTraits:(NSFontTraitMask)traits API_AVAILABLE(macos(10.5));

@property (nullable, weak) id target API_AVAILABLE(macos(10.5));

@end

@interface NSFontManager(NSFontManagerMenuActionMethods)

- (BOOL)fontNamed:(NSString *)fName hasTraits:(NSFontTraitMask)someTraits;

/* The following is like "availableFonts", but checks traits.  Returns an auto-released instance.  To get fonts with "non-italic" or "non-bold" combine the flags NSUnitalicFontMask and/or NSUnboldFontMask as the argument "someTraits".
*/
- (nullable NSArray<NSString *> *)availableFontNamesWithTraits:(NSFontTraitMask)someTraits;

/* These methods are action messages sent from Font Menu items:
*/
- (void)addFontTrait:(nullable id)sender;
- (void)removeFontTrait:(nullable id)sender;
- (void)modifyFontViaPanel:(nullable id)sender;
- (void)modifyFont:(nullable id)sender;
- (void)orderFrontFontPanel:(nullable id)sender;
- (void)orderFrontStylesPanel:(nullable id)sender;

@end

/* This method is perhaps inappropriately named; it is sent to the delegate of the shared NSFontPanel instance, not to the delegate of the NSFontManager.
*/
@interface NSObject(NSFontManagerDelegate)
- (BOOL)fontManager:(id)sender willIncludeFont:(NSString *)fontName API_DEPRECATED("Use NSFontCollection for providing filtered font lists", macos(10.0,10.11));
@end

#if __swift__ < 40200
/* This is the message that's propagated up the responder chain.
*/
@interface NSObject(NSFontManagerResponderMethod)
- (void)changeFont:(nullable id)sender API_DEPRECATED("This is now an optional method of the NSFontChanging protocol.", macos(10.0, 11.0));
@end
#endif

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

