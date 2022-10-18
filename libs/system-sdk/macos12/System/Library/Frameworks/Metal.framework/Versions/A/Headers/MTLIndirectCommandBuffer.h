//
//  MTLIndirectCommandBuffer.h
//  Metal
//
//  Copyright © 2017 Apple, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/MTLDefines.h>
#import <Metal/MTLIndirectCommandEncoder.h>

NS_ASSUME_NONNULL_BEGIN


/*!
 @abstract
 A bitfield of commands that may be performed indirectly.
 */
typedef NS_OPTIONS(NSUInteger, MTLIndirectCommandType) {
    MTLIndirectCommandTypeDraw                = (1 << 0),
    MTLIndirectCommandTypeDrawIndexed         = (1 << 1),
    MTLIndirectCommandTypeDrawPatches         API_AVAILABLE(tvos(14.5)) = (1 << 2),
    MTLIndirectCommandTypeDrawIndexedPatches  API_AVAILABLE(tvos(14.5)) = (1 << 3) ,

    MTLIndirectCommandTypeConcurrentDispatch  API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(13.0)) = (1 << 5), /* Dispatch threadgroups with concurrent execution */

    MTLIndirectCommandTypeConcurrentDispatchThreads  API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(13.0)) = (1 << 6), /* Dispatch threads with concurrent execution */
} API_AVAILABLE(macos(10.14), ios(12.0));


/*!
 @abstract The data layout required for specifying an indirect command buffer execution range.
 */
typedef struct
{
    uint32_t location;
    uint32_t length;
}  MTLIndirectCommandBufferExecutionRange API_AVAILABLE(macos(10.14), macCatalyst(13.0), ios(13.0));

MTL_INLINE MTLIndirectCommandBufferExecutionRange MTLIndirectCommandBufferExecutionRangeMake(uint32_t location, uint32_t length) API_AVAILABLE(macos(10.14), macCatalyst(13.0), ios(13.0))
{
    MTLIndirectCommandBufferExecutionRange icbRange = {location, length};
    return icbRange;
}

/*!
 @abstract
 Describes the limits and features that can be used in an indirect command
 */
MTL_EXPORT API_AVAILABLE(macos(10.14), ios(12.0))
@interface MTLIndirectCommandBufferDescriptor : NSObject <NSCopying>

/*!
 @abstract
 A bitfield of the command types that be encoded.
 @discussion
 MTLCommandTypeDispatch cannot be mixed with any other command type.
 */
@property (readwrite, nonatomic) MTLIndirectCommandType commandTypes;

/*!
 @abstract
 Whether the render or compute pipeline are inherited from the encoder
 */
@property (readwrite, nonatomic) BOOL inheritPipelineState API_AVAILABLE(macos(10.14), ios(13.0));

/*!
 @abstract
 Whether the render or compute pipeline can set arguments.
 */
@property (readwrite, nonatomic) BOOL inheritBuffers;

/*!
 @abstract
 The maximum bind index of vertex argument buffers that can be set per command.
 */
@property (readwrite, nonatomic) NSUInteger maxVertexBufferBindCount;

/*!
 @absract
 The maximum bind index of fragment argument buffers that can be set per command.
 */
@property (readwrite, nonatomic) NSUInteger maxFragmentBufferBindCount;

/*!
 @absract
 The maximum bind index of kernel (or tile) argument buffers that can be set per command.
 */
@property (readwrite, nonatomic) NSUInteger maxKernelBufferBindCount API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(13.0));

@end


API_AVAILABLE(macos(10.14), ios(12.0))
@protocol MTLIndirectCommandBuffer <MTLResource>

@property (readonly) NSUInteger size;

-(void)resetWithRange:(NSRange)range;

- (id <MTLIndirectRenderCommand>)indirectRenderCommandAtIndex:(NSUInteger)commandIndex;
- (id <MTLIndirectComputeCommand>)indirectComputeCommandAtIndex:(NSUInteger)commandIndex API_AVAILABLE(macos(11.0), macCatalyst(14.0), ios(13.0));

@end

NS_ASSUME_NONNULL_END
