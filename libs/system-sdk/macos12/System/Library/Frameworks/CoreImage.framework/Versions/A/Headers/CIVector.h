/* 
   CoreImage - CIVector.h

   Copyright (c) 2015 Apple, Inc.
   All rights reserved.
*/

#import <CoreImage/CoreImageDefines.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

NS_CLASS_AVAILABLE(10_4, 5_0)
@interface CIVector : NSObject <NSCopying, NSSecureCoding>
{
    size_t _count;
    
    union {
        CGFloat vec[4];
        CGFloat *ptr;
    }
    _u;
}

/* Create a new vector object. */

+ (instancetype)vectorWithValues:(const CGFloat *)values count:(size_t)count;

+ (instancetype)vectorWithX:(CGFloat)x;
+ (instancetype)vectorWithX:(CGFloat)x Y:(CGFloat)y;
+ (instancetype)vectorWithX:(CGFloat)x Y:(CGFloat)y Z:(CGFloat)z;
+ (instancetype)vectorWithX:(CGFloat)x Y:(CGFloat)y Z:(CGFloat)z W:(CGFloat)w;

/* the CGPoint x and y values are stored in the first X and Y values of the CIVector. */
+ (instancetype)vectorWithCGPoint:(CGPoint)p NS_AVAILABLE(10_9, 5_0);

/* the CGRect x, y, width, height values are stored in the first X, Y, Z, W values of the CIVector. */
+ (instancetype)vectorWithCGRect:(CGRect)r NS_AVAILABLE(10_9, 5_0);

/* the CGAffineTransform's six values are stored in the first six values of the CIVector. */
+ (instancetype)vectorWithCGAffineTransform:(CGAffineTransform)t NS_AVAILABLE(10_9, 5_0);

+ (instancetype)vectorWithString:(NSString *)representation;

/* Initializers. */

- (instancetype)initWithValues:(const CGFloat *)values count:(size_t)count NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithX:(CGFloat)x;
- (instancetype)initWithX:(CGFloat)x Y:(CGFloat)y;
- (instancetype)initWithX:(CGFloat)x Y:(CGFloat)y Z:(CGFloat)z;
- (instancetype)initWithX:(CGFloat)x Y:(CGFloat)y Z:(CGFloat)z W:(CGFloat)w;
- (instancetype)initWithCGPoint:(CGPoint)p NS_AVAILABLE(10_9, 5_0);
- (instancetype)initWithCGRect:(CGRect)r NS_AVAILABLE(10_9, 5_0);
- (instancetype)initWithCGAffineTransform:(CGAffineTransform)r NS_AVAILABLE(10_9, 5_0);

- (instancetype)initWithString:(NSString *)representation;

/* Return the value from the vector at position 'index' (zero-based).
 * Any 'index' value is valid, if the component would otherwise be
 * undefined, zero is returned. */
- (CGFloat)valueAtIndex:(size_t)index;

/* Return the number of values stored in the vector. */
@property (readonly) size_t count;

/* Properties. */

@property (readonly) CGFloat X;
@property (readonly) CGFloat Y;
@property (readonly) CGFloat Z;
@property (readonly) CGFloat W;
@property (readonly) CGPoint CGPointValue NS_AVAILABLE(10_9, 5_0);
@property (readonly) CGRect CGRectValue NS_AVAILABLE(10_9, 5_0);
@property (readonly) CGAffineTransform CGAffineTransformValue NS_AVAILABLE(10_9, 5_0);

/* Returns a formatted string with the components of the vector.
 * The string is suitable for passing to [CIVector vectorWithString:].
 * This property is not KVO-safe because it returns a new NSString each time.
 * The value of the NSString will be the same each time it is called. */
@property (readonly) NSString *stringRepresentation;

@end

NS_ASSUME_NONNULL_END
