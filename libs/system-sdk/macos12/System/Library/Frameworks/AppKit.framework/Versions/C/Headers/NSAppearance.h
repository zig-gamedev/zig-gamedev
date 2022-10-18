/*
        NSAppearance.h
        Application Kit
        Copyright (c) 2011-2021, Apple Inc.
        All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString, NSBundle;

typedef NSString * NSAppearanceName NS_TYPED_EXTENSIBLE_ENUM;

API_AVAILABLE(macos(10.9))
@interface NSAppearance : NSObject <NSSecureCoding> 

@property (readonly, copy) NSAppearanceName name API_AVAILABLE(macos(10.9));

// Sets the current thread's appearance, which is used for drawing, resolving colors/images, and laying out views.
// Automatically set by NSView before that view's drawRect:, updateLayer, and layout methods are invoked.
// At other times its return value is unreliable (depending on if the previous caller restored it to a previous value after setting it).
// This is not the correct way to determine the 'system' appearance. Use a view's, window's, or the app's effectiveAppearance.
@property (class, null_resettable, strong) NSAppearance *currentAppearance API_DEPRECATED("Use -performAsCurrentDrawingAppearance: to temporarily set the drawing appearance, or +currentDrawingAppearance to access the currently drawing appearance.", macos(10.9, 12.0));

// The appearance which has been made active for drawing (usually by locking focus on a view)
// and is used for color and asset resolution.
@property (class, readonly, strong) NSAppearance *currentDrawingAppearance API_AVAILABLE(macos(11.0));

// Make the receiver the active drawing appearance and perform the block.
// Saves and restores the previous current appearance.
- (void)performAsCurrentDrawingAppearance:(void (NS_NOESCAPE ^)(void))block API_AVAILABLE(macos(11.0));

/* Finds and returns an NSAppearance based on the name. 
 For standard appearances such as NSAppearanceNameAqua, a built-in appearance is returned.
 For other names, the main bundle is searched.
 */
+ (nullable NSAppearance *)appearanceNamed:(NSAppearanceName)name;

/* Creates an NSAppearance by searching the specified bundle for a file with the specified name (without path extension).
    If bundle is nil, the main bundle is assumed.
 */
- (nullable instancetype)initWithAppearanceNamed:(NSAppearanceName)name bundle:(nullable NSBundle *)bundle NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

/* Query allowsVibrancy to see if the given appearance actually needs vibrant drawing. You may want to draw differently if the current apperance is vibrant.
 */
@property (readonly) BOOL allowsVibrancy API_AVAILABLE(macos(10.10));

/* Given a list of appearance names, returns the one that best matches the receiver
 */
- (nullable NSAppearanceName)bestMatchFromAppearancesWithNames:(NSArray<NSAppearanceName> *)appearances API_AVAILABLE(macos(10.14));

@end

#pragma mark -
#pragma mark Standard Appearances

APPKIT_EXTERN NSAppearanceName const NSAppearanceNameAqua API_AVAILABLE(macos(10.9));
APPKIT_EXTERN NSAppearanceName const NSAppearanceNameDarkAqua API_AVAILABLE(macos(10.14));

APPKIT_EXTERN NSAppearanceName const NSAppearanceNameLightContent API_DEPRECATED("Light content should use the default Aqua apppearance.", macos(10.9,10.10));

/* The following two Vibrant appearances should only be set on an NSVisualEffectView, or one of its container subviews.
 */
APPKIT_EXTERN NSAppearanceName const NSAppearanceNameVibrantDark API_AVAILABLE(macos(10.10));
APPKIT_EXTERN NSAppearanceName const NSAppearanceNameVibrantLight API_AVAILABLE(macos(10.10));

/* The following appearance names are for matching using bestMatchFromAppearancesWithNames:
   Passing any of them to appearanceNamed: will return NULL
 */
APPKIT_EXTERN NSAppearanceName const NSAppearanceNameAccessibilityHighContrastAqua API_AVAILABLE(macos(10.14));
APPKIT_EXTERN NSAppearanceName const NSAppearanceNameAccessibilityHighContrastDarkAqua API_AVAILABLE(macos(10.14));
APPKIT_EXTERN NSAppearanceName const NSAppearanceNameAccessibilityHighContrastVibrantLight API_AVAILABLE(macos(10.14));
APPKIT_EXTERN NSAppearanceName const NSAppearanceNameAccessibilityHighContrastVibrantDark API_AVAILABLE(macos(10.14));

#pragma mark -

@protocol NSAppearanceCustomization <NSObject>
@required

@property (nullable, strong) NSAppearance *appearance API_AVAILABLE(macos(10.9));

/* This returns the appearance that would be used when drawing the receiver, taking inherited appearances into account.
 */
@property (readonly, strong) NSAppearance *effectiveAppearance API_AVAILABLE(macos(10.9));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
