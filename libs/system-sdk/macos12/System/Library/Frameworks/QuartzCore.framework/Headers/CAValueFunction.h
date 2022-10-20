/* CoreAnimation - CAValueFunction.h

   Copyright (c) 2008-2021, Apple Inc.
   All rights reserved. */

#import <QuartzCore/CABase.h>
#import <Foundation/NSObject.h>

typedef NSString * CAValueFunctionName NS_TYPED_ENUM;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0))
@interface CAValueFunction : NSObject <NSSecureCoding>
{
@protected
  NSString *_string;
  void *_impl;
}

+ (nullable instancetype)functionWithName:(CAValueFunctionName)name;

@property(readonly) CAValueFunctionName name;

@end

/** Value function names. **/

/* The `rotateX', `rotateY', `rotateZ' functions take a single input
 * value in radians, and construct a 4x4 matrix representing the
 * corresponding rotation matrix. */

CA_EXTERN CAValueFunctionName const kCAValueFunctionRotateX
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAValueFunctionName const kCAValueFunctionRotateY
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAValueFunctionName const kCAValueFunctionRotateZ
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

/* The `scale' function takes three input values and constructs a
 * 4x4 matrix representing the corresponding scale matrix. */

CA_EXTERN CAValueFunctionName const kCAValueFunctionScale
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

/* The `scaleX', `scaleY', `scaleZ' functions take a single input value
 * and construct a 4x4 matrix representing the corresponding scaling
 * matrix. */

CA_EXTERN CAValueFunctionName const kCAValueFunctionScaleX
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAValueFunctionName const kCAValueFunctionScaleY
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAValueFunctionName const kCAValueFunctionScaleZ
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

/* The `translate' function takes three input values and constructs a
 * 4x4 matrix representing the corresponding scale matrix. */

CA_EXTERN CAValueFunctionName const kCAValueFunctionTranslate
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

/* The `translateX', `translateY', `translateZ' functions take a single
 * input value and construct a 4x4 matrix representing the corresponding
 * translation matrix. */

CA_EXTERN CAValueFunctionName const kCAValueFunctionTranslateX
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAValueFunctionName const kCAValueFunctionTranslateY
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));
CA_EXTERN CAValueFunctionName const kCAValueFunctionTranslateZ
    API_AVAILABLE(macos(10.6), ios(3.0), watchos(2.0), tvos(9.0));

NS_ASSUME_NONNULL_END
