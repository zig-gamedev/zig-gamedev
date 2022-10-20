/* CoreImage - CIDetector.h

   Copyright (c) 2011 Apple, Inc.
   All rights reserved. */


#import <CoreImage/CoreImageDefines.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class CIImage;
@class CIContext;
@class CIFeature;

/** Detects features in images.

 This class potentially holds onto a lot of state. Hence it may be beneficial from a performance perspective to re-use the same CIDetector instance. Specifying a CIContext when creating a detector may have an impact on performance since this context may be used when analyzing an image.
 */
NS_CLASS_AVAILABLE(10_7, 5_0)
@interface CIDetector : NSObject {}

/** Returns a new detector instance of the given type.

 The type is used to specify the detection intent.
 This will return value if the detector type is not supported.

 The context argument specifies the CIContext to be used to operate on the image. May be nil.

 If the input image to -featuresInImage: is the output of a CoreImage operation, it may improve performance to specify the same context that was used to operate on that image.

 The detector may do image processing in this context and if the image is on the GPU and the specified context is a GPU context this may avoid additional upload to / download from the GPU. If the input image is on the CPU (or the output from a CPU based context) specifying a GPU based context (or vice versa) may reduce performance.

 The options parameter lets you optinally specify a accuracy / performance tradeoff. Can be nil or an empty dictionary. */
+ (nullable CIDetector *)detectorOfType:(NSString*)type
                                context:(nullable CIContext *)context
                                options:(nullable NSDictionary<NSString *,id> *)options
    NS_AVAILABLE(10_7, 5_0);

/** Returns an array of CIFeature instances in the given image.
 The array is sorted by confidence, highest confidence first. */
- (NSArray<CIFeature *> *)featuresInImage:(CIImage *)image
    NS_AVAILABLE(10_7, 5_0);

/** Returns an array of CIFeature instances in the given image.
 The array is sorted by confidence, highest confidence first.
 The options dictionary can contain a CIDetectorImageOrientation key value. */
- (NSArray<CIFeature *> *)featuresInImage:(CIImage *)image
                                  options:(nullable NSDictionary<NSString *,id> *)options
    NS_AVAILABLE(10_8, 5_0);

@end


// Types to be used for +[CIDetector detectorOfType:context:options:]

/* Specifies a detector type for face recognition. */
CORE_IMAGE_EXPORT NSString* const CIDetectorTypeFace NS_AVAILABLE(10_7, 5_0);

/* Specifies a detector type for rectangle detection. */
CORE_IMAGE_EXPORT NSString* const CIDetectorTypeRectangle NS_AVAILABLE(10_10, 8_0);

/* Specifies a detector type for barcode detection. */
CORE_IMAGE_EXPORT NSString* const CIDetectorTypeQRCode NS_AVAILABLE(10_10, 8_0);

/* Specifies a detector type for text detection. */
#if __OBJC2__
CORE_IMAGE_EXPORT NSString* const CIDetectorTypeText NS_AVAILABLE(10_11, 9_0);
#endif

// Options that can be used with +[CIDetector detectorOfType:context:options:]

/* The key in the options dictionary used to specify a accuracy / performance tradeoff to be used. */
CORE_IMAGE_EXPORT NSString* const CIDetectorAccuracy NS_AVAILABLE(10_7, 5_0);

/* These are values to be used with the CIDetectorAccuracy key when created a CIDetector.
 There is a performance / accuracy tradeoff to be made. The default value will work well for most
 situations, but using these the detector will favour performance over accuracy or
 accuracy over performance. */
CORE_IMAGE_EXPORT NSString* const CIDetectorAccuracyLow NS_AVAILABLE(10_7, 5_0); /* Lower accuracy, higher performance */
CORE_IMAGE_EXPORT NSString* const CIDetectorAccuracyHigh NS_AVAILABLE(10_7, 5_0); /* Lower performance, higher accuracy */

/*The key in the options dictionary used to specify that feature tracking should be used. */
CORE_IMAGE_EXPORT NSString* const CIDetectorTracking NS_AVAILABLE(10_8, 6_0);

/* The key in the options dictionary used to specify the minimum size that the
 detector will recognize as a feature. */

/* For face detector, the value for this key is an float NSNumber
 from 0.0 ... 1.0 that represents a percentage of shorter edge of an input image.
 valid values range: 0.01 <= CIDetectorMinFeatureSize <= 0.5.
 Setting a higher value for this parameter is used for performance gain only. The default value is 0.15. */

/* For rectangle detector, the value for this key is an float NSNumber
 from 0.0 ... 1.0 that represents a percentage of shorter edge of an input image.
 valid values range: 0.2 <= CIDetectorMinFeatureSize <= 1.0 The default value is 0.2. */

/* For text detector, the value for this key is an float NSNumber
 from 0.0 ... 1.0 that represents a percentage of height of an input image.
 valid values range: 0.0 <= CIDetectorMinFeatureSize <= 1.0. The default value is 10/(height of input image). */
CORE_IMAGE_EXPORT NSString* const CIDetectorMinFeatureSize NS_AVAILABLE(10_8, 6_0);

/* For rectangle detector, the value for this key is an integer NSNumber
 from 1 ... 256 that represents the maximum number of features to return. 
 valid value range: 1 <= CIDetectorMaxFeatureCount <= 256. The default value is 1.
 */
CORE_IMAGE_EXPORT NSString* const CIDetectorMaxFeatureCount NS_AVAILABLE(10_12, 10_0);

/* The key in the options dictionary used to specify number of angles, the value for this key is one of 1, 3, 5, 7, 9, 11.*/
CORE_IMAGE_EXPORT NSString* const CIDetectorNumberOfAngles NS_AVAILABLE(10_11, 9_0);

// Options that can be used with -[CIDetector featuresInImage:options:]

/* The value for this key is an integer NSNumber from 1..8 such as that
 found in kCGImagePropertyOrientation.  If present, the detection will be done
 based on that orientation but the coordinates in the returned features will
 still be based on those of the image. */
CORE_IMAGE_EXPORT NSString *const CIDetectorImageOrientation NS_AVAILABLE(10_8, 5_0);

/* The value for this key is a bool NSNumber. If true, facial expressions, such as blinking and closed eyes are extracted */
CORE_IMAGE_EXPORT NSString *const CIDetectorEyeBlink NS_AVAILABLE(10_9, 7_0);


/* The value for this key is a bool NSNumber. If true, facial expressions, such as smile are extracted */
CORE_IMAGE_EXPORT NSString *const CIDetectorSmile NS_AVAILABLE(10_9, 7_0);

/* The value for this key is a float NSNumber. Specifies the per frame focal length.  */
CORE_IMAGE_EXPORT NSString* const CIDetectorFocalLength NS_AVAILABLE(10_10, 8_0);

/* The value for this key is a float NSNumber. Specifies the aspect ratio of the rectangle detected.  */
CORE_IMAGE_EXPORT NSString* const CIDetectorAspectRatio NS_AVAILABLE(10_10, 8_0);

#if __OBJC2__
/* The value for this key is a bool NSNumber. Controls whether the text detector should detect subfeatures or not. The default value is NO */
CORE_IMAGE_EXPORT NSString* const CIDetectorReturnSubFeatures __OSX_AVAILABLE_STARTING(__MAC_10_11, __IPHONE_9_0);
#endif

NS_ASSUME_NONNULL_END
