/*
 * ImageIO - CGImageAnimation.h
 * Copyright (c) 2018 Apple Inc. All rights reserved.
 *
 */

#ifndef CGIMAGEANIMATION_H_
#define CGIMAGEANIMATION_H_

#include <ImageIO/ImageIOBase.h>

#include <CoreGraphics/CoreGraphics.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_ASSUME_NONNULL_BEGIN


typedef CF_ENUM(OSStatus, CGImageAnimationStatus) {
    kCGImageAnimationStatus_ParameterError          = -22140,      /* NULL or invalid parameter passed to API */
    kCGImageAnimationStatus_CorruptInputImage       = -22141,      /* An image cannot be read from the given source */
    kCGImageAnimationStatus_UnsupportedFormat       = -22142,      /* The image format is not applicable to animation */
    kCGImageAnimationStatus_IncompleteInputImage    = -22143,      /* An image can be read from the given source, but it is incomplete */
    kCGImageAnimationStatus_AllocationFailure       = -22144       /* A required resource could not be created */
};

/* Starts the animation at the given index. Defaults to 0
 * Value is a CFNumber
 */
IMAGEIO_EXTERN const CFStringRef kCGImageAnimationStartIndex  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);

/* The value of this key overrides the `delay time' specified by the image
 * Value is a CFNumber of kCFNumberDoubleType.
 */
IMAGEIO_EXTERN const CFStringRef kCGImageAnimationDelayTime  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);

/* The value of this key overrides the `loop count' specified by the image
 * Value is a CFNumber; kCFNumberPositiveInfinity may be used.
 */
IMAGEIO_EXTERN const CFStringRef kCGImageAnimationLoopCount  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);


typedef void (^CGImageSourceAnimationBlock)(size_t index, CGImageRef image, bool* stop);

/** Animate the sequence of images contained in the file at `url'. Currently supported image
 * formats are GIF and APNG. The `options' dictionary may be used to request additional playback
 * options; see the list of keys above for more information. The block is called on the main queue
 * at time intervals specified by the `delay time' of the image. The animation can be stopped by
 * setting the boolean parameter of the block to true.
 */
IMAGEIO_EXTERN OSStatus CGAnimateImageAtURLWithBlock(CFURLRef url, CFDictionaryRef _iio_Nullable options, CGImageSourceAnimationBlock block) IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);

/** Animate the sequence of images contained in `data'. Currently supported image
 * formats are GIF and APNG. The `options' dictionary may be used to request additional playback
 * options; see the list of keys above for more information. The block is called on the main queue
 * at time intervals specified by the `delay time' of the image. The animation can be stopped by
 * setting the boolean parameter of the block to true.
 */
IMAGEIO_EXTERN OSStatus CGAnimateImageDataWithBlock(CFDataRef data, CFDictionaryRef _iio_Nullable options, CGImageSourceAnimationBlock block) IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGIMAGEANIMATION_H_ */
