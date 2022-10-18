/*
	NSColorSpace.h
	Application Kit
	Copyright (c) 2004-2021, Apple Inc.
	All rights reserved.
*/


#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>
#import <ApplicationServices/ApplicationServices.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSData;


typedef NS_ENUM(NSInteger, NSColorSpaceModel) {
    NSColorSpaceModelUnknown = -1,
    NSColorSpaceModelGray,
    NSColorSpaceModelRGB,
    NSColorSpaceModelCMYK,
    NSColorSpaceModelLAB,
    NSColorSpaceModelDeviceN,
    NSColorSpaceModelIndexed,
    NSColorSpaceModelPatterned
};

@interface NSColorSpace : NSObject <NSSecureCoding>

- (nullable instancetype)initWithICCProfileData:(NSData *)iccData;
@property (nullable, readonly, strong) NSData *ICCProfileData;

- (nullable instancetype)initWithColorSyncProfile:(void * /* ColorSyncProfileRef */)prof;
@property (nullable, readonly) void * /* ColorSyncProfileRef */colorSyncProfile NS_RETURNS_INNER_POINTER;

/* Create NSColorSpace from a CGColorSpace.  Might return nil if the CGColorSpace is one not supported by NSColorSpace. Internally the NSColorSpace might retain or create a new copy of the provided CGColorSpace; do not rely on pointer equality of the CGColorSpace provided to this function, when later queried. Archiving, for instance, might not persist the pointer equality/identity of the CGColorSpace.
*/
- (nullable instancetype)initWithCGColorSpace:(CGColorSpaceRef)cgColorSpace  API_AVAILABLE(macos(10.5));

/* Might return NULL if the NSColorSpace space cannot be represented as a CGColorSpace.
*/
@property (nullable, readonly) CGColorSpaceRef CGColorSpace  API_AVAILABLE(macos(10.5));

@property (readonly) NSInteger numberOfColorComponents;		// Does not include alpha
@property (readonly) NSColorSpaceModel colorSpaceModel;
@property (nullable, readonly, copy) NSString *localizedName;	// Will return nil if no localized name

@property (class, strong, readonly) NSColorSpace *sRGBColorSpace API_AVAILABLE(macos(10.5));
@property (class, strong, readonly) NSColorSpace *genericGamma22GrayColorSpace API_AVAILABLE(macos(10.6));             // The grayscale color space with gamma 2.2, compatible with sRGB

@property (class, strong, readonly) NSColorSpace *extendedSRGBColorSpace API_AVAILABLE(macos(10.12));                  // sRGB compatible color space that allows specifying components beyond the range of [0.0, 1.0]
@property (class, strong, readonly) NSColorSpace *extendedGenericGamma22GrayColorSpace API_AVAILABLE(macos(10.12));    // sRGB compatible gray color space that allows specifying components beyond the range of [0.0, 1.0]

@property (class, strong, readonly) NSColorSpace *displayP3ColorSpace API_AVAILABLE(macos(10.12));     // Standard DCI-P3 primaries, a D65 white point, and the same gamma curve as the sRGB IEC61966-2.1 color space

@property (class, strong, readonly) NSColorSpace *adobeRGB1998ColorSpace API_AVAILABLE(macos(10.5));

@property (class, strong, readonly) NSColorSpace *genericRGBColorSpace;		// NSColorSpace corresponding to Cocoa color space name NSCalibratedRGBColorSpace
@property (class, strong, readonly) NSColorSpace *genericGrayColorSpace;	// NSColorSpace corresponding to Cocoa color space name NSCalibratedWhiteColorSpace
@property (class, strong, readonly) NSColorSpace *genericCMYKColorSpace;
@property (class, strong, readonly) NSColorSpace *deviceRGBColorSpace;		// NSColorSpace corresponding to Cocoa color space name NSDeviceRGBColorSpace
@property (class, strong, readonly) NSColorSpace *deviceGrayColorSpace;		// NSColorSpace corresponding to Cocoa color space name NSDeviceWhiteColorSpace
@property (class, strong, readonly) NSColorSpace *deviceCMYKColorSpace;		// NSColorSpace corresponding to Cocoa color space name NSDeviceCMYKColorSpace

/* Return the list of color spaces available on the system that are displayed by the color panel, in the order they are displayed in the color panel. Doesn't return arbitrary color spaces which may have been created on the fly, or spaces without user displayable names. Pass model==NSUnknownColorSpaceModel to get all color spaces. Empty array is returned if no color spaces are available for the specified model.
*/
+ (NSArray<NSColorSpace *> *)availableColorSpacesWithModel:(NSColorSpaceModel)model  API_AVAILABLE(macos(10.6));

@end


static const NSColorSpaceModel NSUnknownColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelUnknown", macos(10.0,10.14)) = NSColorSpaceModelUnknown;
static const NSColorSpaceModel NSGrayColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelGray", macos(10.0,10.14)) = NSColorSpaceModelGray;
static const NSColorSpaceModel NSRGBColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelRGB", macos(10.0,10.14)) = NSColorSpaceModelRGB;
static const NSColorSpaceModel NSCMYKColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelCMYK", macos(10.0,10.14)) = NSColorSpaceModelCMYK;
static const NSColorSpaceModel NSLABColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelLAB", macos(10.0,10.14)) = NSColorSpaceModelLAB;
static const NSColorSpaceModel NSDeviceNColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelDeviceN", macos(10.0,10.14)) = NSColorSpaceModelDeviceN;
static const NSColorSpaceModel NSIndexedColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelIndexed", macos(10.0,10.14)) = NSColorSpaceModelIndexed;
static const NSColorSpaceModel NSPatternColorSpaceModel API_DEPRECATED_WITH_REPLACEMENT("NSColorSpaceModelPatterned", macos(10.0,10.14)) = NSColorSpaceModelPatterned;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END



