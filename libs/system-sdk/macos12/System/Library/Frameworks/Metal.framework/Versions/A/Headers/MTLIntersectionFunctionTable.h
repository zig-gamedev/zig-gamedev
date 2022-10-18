//
//  MTLIntersectionFunctionTable.h
//  Framework
//
//  Copyright © 2020 Apple, Inc. All rights reserved.
//

#import <Metal/MTLDefines.h>


#import <Metal/MTLResource.h>
#import <Metal/MTLFunctionHandle.h>

/**
 * @brief Signature defining what data is provided to an intersection function. The signature
 * must match across the shading language declaration of the intersection function table,
 * intersection functions in the table, and the intersector using the table.
 */
typedef NS_OPTIONS(NSUInteger, MTLIntersectionFunctionSignature) {
	/**
	 * @Brief No signature
	 */
	MTLIntersectionFunctionSignatureNone = 0,

	/**
	 * @brief The intersection functions are entitled to read the built-in instance_id as described in
	 * the Metal Shading Language Guide.
	 */
	MTLIntersectionFunctionSignatureInstancing = (1 << 0),

	/**
	 * @brief The triangle intersection functions are entitled to to read the built-in barycentric_coord
	 * and front_facing as described in the Metal Shading Language Guide.
	 */
	MTLIntersectionFunctionSignatureTriangleData = (1 << 1),

	/**
	 * @brief The intersection functions are entitled to query world_space_origin and
	 * world_space_direction as described in the Metal Shading Language Guide.
	 */
    MTLIntersectionFunctionSignatureWorldSpaceData = (1 << 2),
    
    
    /**
     * @brief The intersection functions may be called from intersectors using the
     * instance_motion intersection tag as described in the Metal Shading Language Guide.
     */
    MTLIntersectionFunctionSignatureInstanceMotion API_AVAILABLE(macos(12.0), ios(15.0)) = (1 << 3),
    
    /**
     * @brief The intersection functions are entitled to query time, motion_start_time,
     * motion_end_time and key_frame_count as described in the Metal Shading Language Guide.
     */
    MTLIntersectionFunctionSignaturePrimitiveMotion API_AVAILABLE(macos(12.0), ios(15.0)) = (1 << 4),
    
    /**
     * @brief The intersection functions may be called from intersectors using the
     * extended_limits intersection tag as described in the Metal Shading Language Guide.
     */
    MTLIntersectionFunctionSignatureExtendedLimits API_AVAILABLE(macos(12.0), ios(15.0)) = (1 << 5),
    
    
} MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0));

MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLIntersectionFunctionTableDescriptor : NSObject <NSCopying>

/*!
 @method intersectionFunctionTableDescriptor
 @abstract Create an autoreleased intersection function table descriptor
 */
+ (nonnull MTLIntersectionFunctionTableDescriptor *)intersectionFunctionTableDescriptor;

/*!
* @property functionCount
* @abstract The number of functions in the table.
*/
@property (nonatomic) NSUInteger functionCount;

@end

API_AVAILABLE(macos(11.0), ios(14.0))
@protocol MTLIntersectionFunctionTable <MTLResource>


- (void)setBuffer:(nullable id <MTLBuffer>)buffer offset:(NSUInteger)offset atIndex:(NSUInteger)index;
- (void)setBuffers:(const id <MTLBuffer> __nullable [__nonnull])buffers offsets:(const NSUInteger [__nonnull])offsets withRange:(NSRange)range;


- (void)setFunction:(nullable id <MTLFunctionHandle>)function atIndex:(NSUInteger)index;
- (void)setFunctions:(const id <MTLFunctionHandle> __nullable [__nonnull])functions withRange:(NSRange)range;

/*
 * @brief Initialize the function at the given index with a triangle intersection function
 * with the given signature which always accepts ray/triangle intersections. If this method is
 * not called and an intersection function is not otherwise set at the given index,
 * ray/triangle intersections will be ignored if a call to the function at the given index
 * would be required. Ray/triangle intersections are always accepted if an intersection
 * function table is not provided.
 */
- (void)setOpaqueTriangleIntersectionFunctionWithSignature:(MTLIntersectionFunctionSignature)signature atIndex:(NSUInteger)index;

/*
 * @brief Initialize the function at the given range with a triangle intersection function
 * with the given signature which always accepts ray/triangle intersections. If this method is
 * not called and an intersection function is not otherwise set at an index in the given range,
 * ray/triangle intersections will be ignored if a call to the function at that index
 * would be required. Ray/triangle intersections are always accepted if an intersection
 * function table is not provided.
 */
- (void)setOpaqueTriangleIntersectionFunctionWithSignature:(MTLIntersectionFunctionSignature)signature withRange:(NSRange)range;

- (void)setVisibleFunctionTable:(nullable id <MTLVisibleFunctionTable>)functionTable atBufferIndex:(NSUInteger)bufferIndex;
- (void)setVisibleFunctionTables:(const id <MTLVisibleFunctionTable> __nullable [__nonnull])functionTables withBufferRange:(NSRange)bufferRange;

@end

