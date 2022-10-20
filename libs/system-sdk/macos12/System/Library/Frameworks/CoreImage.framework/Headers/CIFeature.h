/* CoreImage - CIFeature.h

   Copyright (c) 2011 Apple, Inc.
   All rights reserved. */

#import <CoreImage/CoreImageDefines.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/** Generic feature found by a CIDetector. */
NS_CLASS_AVAILABLE(10_7, 5_0)
@interface CIFeature : NSObject {}

/** The type of the feature. */
@property (readonly, retain) NSString *type;
/** The bounds of the feature in the image it was detected in. */
@property (readonly, assign) CGRect bounds;

@end

/** Specifies the type of a feature that is a face. */
CORE_IMAGE_EXPORT NSString* const CIFeatureTypeFace;
/** Specifies the type of a feature that is a rectangle. */
CORE_IMAGE_EXPORT NSString* const CIFeatureTypeRectangle;
/** Specifies the type of a feature that is a QR code. */
CORE_IMAGE_EXPORT NSString* const CIFeatureTypeQRCode;
/** Specifies the type of a feature that is a text. */
CORE_IMAGE_EXPORT NSString* const CIFeatureTypeText;


/** A face feature found by a CIDetector.
 All positions are relative to the original image. */
NS_CLASS_AVAILABLE(10_7, 5_0)
@interface CIFaceFeature : CIFeature
{
    CGRect bounds;
    BOOL hasLeftEyePosition;
    CGPoint leftEyePosition;
    BOOL hasRightEyePosition;
    CGPoint rightEyePosition;
    BOOL hasMouthPosition;
    CGPoint mouthPosition;
    
    
    BOOL hasTrackingID;
    int trackingID;
    BOOL hasTrackingFrameCount;
    int trackingFrameCount;
    
    BOOL hasFaceAngle;
    float faceAngle;
    
    BOOL hasSmile;
    BOOL leftEyeClosed;
    BOOL rightEyeClosed;
}

/** coordinates of various cardinal points within a face.
 
 Note that the left eye is the eye on the left side of the face
 from the observer's perspective. It is not the left eye from
 the subject's perspective. */

@property (readonly, assign) CGRect bounds;
@property (readonly, assign) BOOL hasLeftEyePosition;
@property (readonly, assign) CGPoint leftEyePosition;
@property (readonly, assign) BOOL hasRightEyePosition;
@property (readonly, assign) CGPoint rightEyePosition;
@property (readonly, assign) BOOL hasMouthPosition;
@property (readonly, assign) CGPoint mouthPosition;

@property (readonly, assign) BOOL hasTrackingID;
@property (readonly, assign) int trackingID;
@property (readonly, assign) BOOL hasTrackingFrameCount;
@property (readonly, assign) int trackingFrameCount;

@property (readonly, assign) BOOL hasFaceAngle;
@property (readonly, assign) float faceAngle;

@property (readonly, assign) BOOL hasSmile;
@property (readonly, assign) BOOL leftEyeClosed;
@property (readonly, assign) BOOL rightEyeClosed;

@end

/** A rectangle feature found by a CIDetector
 All positions are relative to the original image. */

NS_CLASS_AVAILABLE(10_10, 8_0)
@interface CIRectangleFeature : CIFeature
{
    CGRect bounds;
    CGPoint topLeft;
    CGPoint topRight;
    CGPoint bottomLeft;
    CGPoint bottomRight;
}

@property (readonly) CGRect bounds;
@property (readonly) CGPoint topLeft;
@property (readonly) CGPoint topRight;
@property (readonly) CGPoint bottomLeft;
@property (readonly) CGPoint bottomRight;

@end

/** A QR code feature found by a CIDetector
 All positions are relative to the original image. */

@class CIQRCodeDescriptor;

NS_CLASS_AVAILABLE(10_10, 8_0)
@interface CIQRCodeFeature : CIFeature <NSSecureCoding, NSCopying>
{
    CGRect bounds;
    CGPoint topLeft;
    CGPoint topRight;
    CGPoint bottomLeft;
    CGPoint bottomRight;
    CIQRCodeDescriptor *symbolDescriptor;
}

@property (readonly) CGRect bounds;
@property (readonly) CGPoint topLeft;
@property (readonly) CGPoint topRight;
@property (readonly) CGPoint bottomLeft;
@property (readonly) CGPoint bottomRight;

@property (nullable, readonly) NSString* messageString;

@property (nullable, readonly) CIQRCodeDescriptor *symbolDescriptor NS_AVAILABLE(10_13, 11_0);

@end

/** A text feature found by a CIDetector
 All positions are relative to the original image. */

#if __OBJC2__
NS_CLASS_AVAILABLE(10_11, 9_0)
@interface CITextFeature : CIFeature
{
}

@property (readonly) CGRect bounds;
@property (readonly) CGPoint topLeft;
@property (readonly) CGPoint topRight;
@property (readonly) CGPoint bottomLeft;
@property (readonly) CGPoint bottomRight;
@property (nullable, readonly) NSArray *subFeatures;


@end
#endif // __OBJC2__

NS_ASSUME_NONNULL_END
