/*
   CoreImage - CIImageProvider.h

   Copyright (c) 2015 Apple, Inc.
   All rights reserved.
*/

#import <CoreImage/CIImage.h>

NS_ASSUME_NONNULL_BEGIN

@interface CIImage (CIImageProvider)

/* Create a new CIImage populated when rendered with  data provided by 'p'.
 * The provider object 'p' is retained until the image is deallocated. 
 * The 'options' dictionary supports kCIImageProviderTileSize as well as
 * other options defined in CIImage.h 
 */
+ (CIImage *)imageWithImageProvider:(id)p
							   size:(size_t)width
                                   :(size_t)height
							 format:(CIFormat)f
						 colorSpace:(nullable CGColorSpaceRef)cs
                            options:(nullable NSDictionary<CIImageOption,id> *)options
    NS_AVAILABLE(10_4, 9_0);

- (instancetype)initWithImageProvider:(id)p
                                 size:(size_t)width
                                     :(size_t)height
                               format:(CIFormat)f
                           colorSpace:(nullable CGColorSpaceRef)cs
                              options:(nullable NSDictionary<CIImageOption,id> *)options
    NS_AVAILABLE(10_4, 9_0);

@end

/** Informal protocol used to lazily supply image data. */
@interface NSObject (CIImageProvider)

/* Callee should initialize the given bitmap with the subregion x,y
 * width,height of the image. (this subregion is defined in the image's
 * local coordinate space, i.e. the origin is the top left corner of
 * the image).
 *
 * By default, this method will be called to requests the full image
 * data regardless of what subregion is needed for the current render.
 * All of the image is loaded or none of it is.
 *
 * If the kCIImageProviderTileSize option is specified, then only the
 * tiles that are needed are requested.
 *
 * Changing the virtual memory mapping of the supplied buffer (e.g. using
 * vm_copy () to modify it) will give undefined behavior.
 */
- (void)provideImageData:(void *)data
			 bytesPerRow:(size_t)rowbytes
				  origin:(size_t)x
                        :(size_t)y
					size:(size_t)width
                        :(size_t)height
				userInfo:(nullable id)info;

@end

/* Options that may be supplied to image providers through the dictionary. */

/* Specifies the the tile size that the provideImageData: method will be called for.
 * If the value is:
 *   An NSNumber, then the value specifies a square tile size.
 *
 *   An NSArray or CIVector with two values, then it specifies a rectangular tile width and height.
 *
 *   Not specified, then provideImageData: will be called for the entire image.
 *
 *   NSNull, then provideImageData: can be called for any possible origin and size.
 */
CORE_IMAGE_EXPORT CIImageOption const kCIImageProviderTileSize NS_AVAILABLE(10_4, 9_0);

/* The object passed when the provideImageData: method is called.
 * It is retained until the image is deallocated.
 */
CORE_IMAGE_EXPORT CIImageOption const kCIImageProviderUserInfo NS_AVAILABLE(10_4, 9_0);

NS_ASSUME_NONNULL_END
