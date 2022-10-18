//
//  MTLAccelerationStructure.h
//  Metal
//
//  Copyright (c) 2020 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/MTLDefines.h>
#import <Metal/MTLTypes.h>
#import <Metal/MTLArgument.h>
#import <Metal/MTLStageInputOutputDescriptor.h>
#import <Metal/MTLRenderCommandEncoder.h>
#import <Metal/MTLAccelerationStructureTypes.h>
#import <Metal/MTLResource.h>

NS_ASSUME_NONNULL_BEGIN

@protocol MTLBuffer;
@protocol MTLAccelerationStructure;

typedef NS_OPTIONS(NSUInteger, MTLAccelerationStructureUsage) {
    /**
     * @brief Default usage
     */
    MTLAccelerationStructureUsageNone = 0,
    
    /**
     * @brief Enable refitting for this acceleration structure. Note that this may reduce
     * acceleration structure quality.
     */
    MTLAccelerationStructureUsageRefit = (1 << 0),
   
    /**
     * @brief Prefer building this acceleration structure quickly at the cost of reduced ray
     * tracing performance.
     */
    MTLAccelerationStructureUsagePreferFastBuild = (1 << 1),

    /**
     * @brief Enable extended limits for this acceleration structure, possibly at the cost of
     * reduced ray tracing performance.
     */
    MTLAccelerationStructureUsageExtendedLimits API_AVAILABLE(macos(12.0), ios(15.0)) = (1 << 2),
} API_AVAILABLE(macos(11.0), ios(14.0));

typedef NS_OPTIONS(uint32_t, MTLAccelerationStructureInstanceOptions) {
    /**
     * @brief No options
     */
    MTLAccelerationStructureInstanceOptionNone = 0,

    /**
     * @brief Disable triangle back or front face culling
     */
    MTLAccelerationStructureInstanceOptionDisableTriangleCulling = (1 << 0),

    /**
     * @brief Disable triangle back or front face culling
     */
    MTLAccelerationStructureInstanceOptionTriangleFrontFacingWindingCounterClockwise = (1 << 1),

    /**
     * @brief Geometry is opaque
     */
    MTLAccelerationStructureInstanceOptionOpaque = (1 << 2),

    /**
     * @brief Geometry is non-opaque
     */
    MTLAccelerationStructureInstanceOptionNonOpaque = (1 << 3),
} API_AVAILABLE(macos(11.0), ios(14.0));

/**
 * @brief Base class for acceleration structure descriptors. Do not use this class directly. Use
 * one of the derived classes instead.
 */
MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLAccelerationStructureDescriptor : NSObject <NSCopying>

@property (nonatomic) MTLAccelerationStructureUsage usage;

@end

/**
 * @brief Base class for all geometry descriptors. Do not use this class directly. Use one of the derived
 * classes instead.
 */
MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLAccelerationStructureGeometryDescriptor : NSObject <NSCopying>

@property (nonatomic) NSUInteger intersectionFunctionTableOffset;

/**
 * @brief Whether the geometry is opaque
 */
@property (nonatomic) BOOL opaque;

/**
 * @brief Whether intersection functions may be invoked more than once per ray/primitive
 * intersection. Defaults to YES.
 */
@property (nonatomic) BOOL allowDuplicateIntersectionFunctionInvocation;

/**
 * @brief Label
 */
@property (nonatomic, copy, nullable) NSString* label API_AVAILABLE(macos(12.0), ios(15.0));

@end

/**
 * @brief Describes what happens to the object before the first motion key and after the last
 * motion key.
 */
typedef NS_ENUM(uint32_t, MTLMotionBorderMode){

    /**
     * @brief Motion is stopped. (default)
     */
    MTLMotionBorderModeClamp = 0,

    /**
     * @brief Object disappears
     */
    MTLMotionBorderModeVanish = 1
} API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Descriptor for a primitive acceleration structure
 */
MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLPrimitiveAccelerationStructureDescriptor : MTLAccelerationStructureDescriptor

/**
 * @brief Array of geometry descriptors. If motionKeyframeCount is greater than one all geometryDescriptors
 * must be motion versions and have motionKeyframeCount of primitive buffers.
 */
@property (nonatomic, retain, nullable) NSArray <MTLAccelerationStructureGeometryDescriptor *> * geometryDescriptors;

/**
 * @brief Motion border mode describing what happens if acceleration structure is sampled before
 * motionStartTime. If not set defaults to MTLMotionBorderModeClamp.
 */
@property (nonatomic) MTLMotionBorderMode motionStartBorderMode API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Motion border mode describing what happens if acceleration structure is sampled after
 * motionEndTime. If not set defaults to MTLMotionBorderModeClamp.
 */
@property (nonatomic) MTLMotionBorderMode motionEndBorderMode API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Motion start time of this geometry. If not set defaults to 0.0f.
 */
@property (nonatomic) float motionStartTime API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Motion end time of this geometry. If not set defaults to 1.0f.
 */
@property (nonatomic) float motionEndTime API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Motion keyframe count. Is 1 by default which means no motion.
 */
@property (nonatomic) NSUInteger motionKeyframeCount API_AVAILABLE(macos(12.0), ios(15.0));

+ (instancetype)descriptor;

@end

/**
 * @brief Descriptor for triangle geometry
 */
MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLAccelerationStructureTriangleGeometryDescriptor : MTLAccelerationStructureGeometryDescriptor

/**
 * @brief Vertex buffer containing triangle vertices. Each vertex must consist of three 32-bit floats
 * encoding X, Y, and Z position. Must not be nil.
 */
@property (nonatomic, retain, nullable) id <MTLBuffer> vertexBuffer;

/**
 * @brief Vertex buffer offset. Must be a multiple of the vertex stride and must be aligned to the
 * platform's buffer offset alignment.
 */
@property (nonatomic) NSUInteger vertexBufferOffset;

/**
 * @brief Stride, in bytes, between vertices in the vertex buffer. Must be at least 12 bytes and must be a
 * multiple of 4 bytes. Defaults to 12 bytes.
 */
@property (nonatomic) NSUInteger vertexStride;

/**
 * Optional index buffer containing references to vertices in the vertex buffer. May be nil.
 */
@property (nonatomic, retain, nullable) id <MTLBuffer> indexBuffer;

/**
 * @brief Index buffer offset. Must be a multiple of the index data type size and must be aligned to both
 * the index data type's alignment and the platform's buffer offset alignment.
 */
@property (nonatomic) NSUInteger indexBufferOffset;

/**
 * @brief Index type
 */
@property (nonatomic) MTLIndexType indexType;

/**
 * @brief Number of triangles
 */
@property (nonatomic) NSUInteger triangleCount;

+ (instancetype)descriptor;

@end

/**
 * @brief Descriptor for bounding box geometry
 */
MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLAccelerationStructureBoundingBoxGeometryDescriptor : MTLAccelerationStructureGeometryDescriptor

/**
 * @brief Bounding box buffer containing MTLAxisAlignedBoundingBoxes. Must not be nil.
 */
@property (nonatomic, retain, nullable) id <MTLBuffer> boundingBoxBuffer;

/**
 * @brief Bounding box buffer offset. Must be a multiple of the bounding box stride and must be
 * aligned to the platform's buffer offset alignment.
 */
@property (nonatomic) NSUInteger boundingBoxBufferOffset;

/**
 * @brief Stride, in bytes, between bounding boxes in the bounding box buffer. Must be at least 24
 * bytes and must be a multiple of 4 bytes. Defaults to 24 bytes.
 */
@property (nonatomic) NSUInteger boundingBoxStride;

/**
 * @brief Number of bounding boxes
 */
@property (nonatomic) NSUInteger boundingBoxCount;

+ (instancetype)descriptor;

@end

/**
 * @brief MTLbuffer and description how the data is stored in it.
 */
MTL_EXPORT API_AVAILABLE(macos(12.0), ios(15.0))
@interface MTLMotionKeyframeData : NSObject

/**
 * @brief Buffer containing the data of a single keyframe. Multiple keyframes can be interleaved in one MTLBuffer.
 */
@property (nonatomic, retain, nullable) id <MTLBuffer> buffer;

/**
 * @brief Buffer offset. Must be a multiple of 4 bytes.
 */
@property (nonatomic) NSUInteger offset;


+ (instancetype)data;

@end

/**
 * @brief Descriptor for motion triangle geometry
 */
MTL_EXPORT API_AVAILABLE(macos(12.0), ios(15.0))
@interface MTLAccelerationStructureMotionTriangleGeometryDescriptor : MTLAccelerationStructureGeometryDescriptor

/**
 * @brief Vertex buffer containing triangle vertices similar to what MTLAccelerationStructureTriangleGeometryDescriptor has but array of the values.
 */
@property (nonatomic, copy) NSArray <MTLMotionKeyframeData *> * vertexBuffers;

/**
 * @brief Stride, in bytes, between vertices in the vertex buffer. Must be at least 12 bytes and must be a
 * multiple of 4 bytes. Defaults to 12 bytes.
 */
@property (nonatomic) NSUInteger vertexStride;

/**
 * Optional index buffer containing references to vertices in the vertex buffer. May be nil.
 */
@property (nonatomic, retain, nullable) id <MTLBuffer> indexBuffer;

/**
 * @brief Index buffer offset. Must be a multiple of the index data type size and must be aligned to both
 * the index data type's alignment and the platform's buffer offset alignment.
 */
@property (nonatomic) NSUInteger indexBufferOffset;

/**
 * @brief Index type
 */
@property (nonatomic) MTLIndexType indexType;

/**
 * @brief Number of triangles
 */
@property (nonatomic) NSUInteger triangleCount;

+ (instancetype)descriptor;

@end

/**
 * @brief Descriptor for motion bounding box geometry
 */
MTL_EXPORT API_AVAILABLE(macos(12.0), ios(15.0))
@interface MTLAccelerationStructureMotionBoundingBoxGeometryDescriptor : MTLAccelerationStructureGeometryDescriptor

/**
 * @brief Bounding box buffer containing MTLAxisAlignedBoundingBoxes similar to what MTLAccelerationStructureBoundingBoxGeometryDescriptor has but array of the values.
 */
@property (nonatomic, copy) NSArray <MTLMotionKeyframeData *> * boundingBoxBuffers;

/**
 * @brief Stride, in bytes, between bounding boxes in the bounding box buffer. Must be at least 24
 * bytes and must be a multiple of 4 bytes. Defaults to 24 bytes.
 */
@property (nonatomic) NSUInteger boundingBoxStride;

/**
 * @brief Number of bounding boxes
 */
@property (nonatomic) NSUInteger boundingBoxCount;

+ (instancetype)descriptor;

@end


typedef struct {
    /**
     * @brief Transformation matrix describing how to transform the bottom-level acceleration structure.
     */
    MTLPackedFloat4x3 transformationMatrix;

    /**
     * @brief Instance options
     */
    MTLAccelerationStructureInstanceOptions options;
    
    /**
     * @brief Instance mask used to ignore geometry during ray tracing
     */
    uint32_t mask;

    /**
     * @brief Used to index into intersection function tables
     */
    uint32_t intersectionFunctionTableOffset;
    
    /**
     * @brief Acceleration structure index to use for this instance
     */
    uint32_t accelerationStructureIndex;
} MTLAccelerationStructureInstanceDescriptor API_AVAILABLE(macos(11.0), ios(14.0));

typedef struct {
    /**
     * @brief Transformation matrix describing how to transform the bottom-level acceleration structure.
     */
    MTLPackedFloat4x3 transformationMatrix;

    /**
     * @brief Instance options
     */
    MTLAccelerationStructureInstanceOptions options;

    /**
     * @brief Instance mask used to ignore geometry during ray tracing
     */
    uint32_t mask;

    /**
     * @brief Used to index into intersection function tables
     */
    uint32_t intersectionFunctionTableOffset;

    /**
     * @brief Acceleration structure index to use for this instance
     */
    uint32_t accelerationStructureIndex;

    /**
     * @brief User-assigned instance ID to help identify this instance in an
     * application-defined way
     */
    uint32_t userID;
} MTLAccelerationStructureUserIDInstanceDescriptor API_AVAILABLE(macos(12.0), ios(15.0));

typedef NS_ENUM(NSUInteger, MTLAccelerationStructureInstanceDescriptorType) {
    /**
     * @brief Default instance descriptor: MTLAccelerationStructureInstanceDescriptor
     */
    MTLAccelerationStructureInstanceDescriptorTypeDefault = 0,

    /**
     * @brief Instance descriptor with an added user-ID
     */
    MTLAccelerationStructureInstanceDescriptorTypeUserID = 1,

    /**
     * @brief Instance descriptor with support for motion
     */
    MTLAccelerationStructureInstanceDescriptorTypeMotion = 2,
} API_AVAILABLE(macos(12.0), ios(15.0));

typedef struct {
    /**
     * @brief Instance options
     */
    MTLAccelerationStructureInstanceOptions options;

    /**
     * @brief Instance mask used to ignore geometry during ray tracing
     */
    uint32_t mask;

    /**
     * @brief Used to index into intersection function tables
     */
    uint32_t intersectionFunctionTableOffset;

    /**
     * @brief Acceleration structure index to use for this instance
     */
    uint32_t accelerationStructureIndex;

    /**
     * @brief User-assigned instance ID to help identify this instance in an
     * application-defined way
     */
    uint32_t userID;

    /**
     * @brief The index of the first set of transforms describing one keyframe of the animation.
     * These transforms are stored in a separate buffer and they are uniformly distributed over
     * time time span of the motion.
     */
    uint32_t motionTransformsStartIndex;

    /**
     * @brief The count of motion transforms belonging to this motion which are stored in consecutive
     * memory addresses at the separate motionTransforms buffer.
     */
    uint32_t motionTransformsCount;
    /**
     * @brief Motion border mode describing what happens if acceleration structure is sampled
     * before motionStartTime
     */
    MTLMotionBorderMode motionStartBorderMode;

    /**
     * @brief Motion border mode describing what happens if acceleration structure is sampled
     * after motionEndTime
     */
    MTLMotionBorderMode motionEndBorderMode;

    /**
     * @brief Motion start time of this instance
     */
    float motionStartTime;

    /**
     * @brief Motion end time of this instance
     */
    float motionEndTime;
} MTLAccelerationStructureMotionInstanceDescriptor API_AVAILABLE(macos(12.0), ios(15.0));


/**
 * @brief Descriptor for an instance acceleration structure
 */
MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLInstanceAccelerationStructureDescriptor : MTLAccelerationStructureDescriptor

/**
 * @brief Buffer containing instance descriptors of the type specified by the instanceDescriptorType property
 */
@property (nonatomic, retain, nullable) id <MTLBuffer> instanceDescriptorBuffer;

/**
 * @brief Offset into the instance descriptor buffer. Must be a multiple of 64 bytes and must be
 * aligned to the platform's buffer offset alignment.
 */
@property (nonatomic) NSUInteger instanceDescriptorBufferOffset;

/**
 * @brief Stride, in bytes, between instance descriptors in the instance descriptor buffer. Must
 * be at least the size of the instance descriptor type and must be a multiple of 4 bytes.
 * Defaults to the size of the instance descriptor type.
 */
@property (nonatomic) NSUInteger instanceDescriptorStride;

/**
 * @brief Number of instance descriptors
 */
@property (nonatomic) NSUInteger instanceCount;

/**
 * @brief Acceleration structures to be instanced
 */
@property (nonatomic, retain, nullable) NSArray <id <MTLAccelerationStructure>> * instancedAccelerationStructures;

/**
 * @brief Type of instance descriptor in the instance descriptor buffer. Defaults to
 * MTLAccelerationStructureInstanceDescriptorTypeDefault.
 */
@property (nonatomic) MTLAccelerationStructureInstanceDescriptorType instanceDescriptorType API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Buffer containing transformation information for motion
 */
@property (nonatomic, retain, nullable) id <MTLBuffer> motionTransformBuffer API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Offset into the instance motion descriptor buffer. Must be a multiple of 64 bytes and
 * must be aligned to the platform's buffer offset alignment.
 */
@property (nonatomic) NSUInteger motionTransformBufferOffset API_AVAILABLE(macos(12.0), ios(15.0));

/**
 * @brief Number of motion transforms
 */
@property (nonatomic) NSUInteger motionTransformCount API_AVAILABLE(macos(12.0), ios(15.0));

+ (instancetype)descriptor;

@end

API_AVAILABLE(macos(11.0), ios(14.0))
@protocol MTLAccelerationStructure <MTLResource>

@property (nonatomic, readonly) NSUInteger size;

@end


NS_ASSUME_NONNULL_END
