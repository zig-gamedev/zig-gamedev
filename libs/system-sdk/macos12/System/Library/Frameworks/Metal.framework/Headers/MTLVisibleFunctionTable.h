//
//  MTLVisibleFunctionTable.h
//  Framework
//
//  Copyright © 2020 Apple, Inc. All rights reserved.
//

#import <Metal/MTLDefines.h>


#import <Metal/MTLResource.h>
#import <Metal/MTLFunctionHandle.h>

MTL_EXPORT API_AVAILABLE(macos(11.0), ios(14.0))
@interface MTLVisibleFunctionTableDescriptor : NSObject <NSCopying>

/*!
 @method visibleFunctionTableDescriptor
 @abstract Create an autoreleased visible function table descriptor
 */
+ (nonnull MTLVisibleFunctionTableDescriptor *)visibleFunctionTableDescriptor;

/*!
* @property functionCount
* @abstract The number of functions in the table.
*/
@property (nonatomic) NSUInteger functionCount;

@end

API_AVAILABLE(macos(11.0), ios(14.0))
@protocol MTLVisibleFunctionTable <MTLResource>

- (void)setFunction:(nullable id <MTLFunctionHandle>)function atIndex:(NSUInteger)index;

- (void)setFunctions:(const id <MTLFunctionHandle> __nullable [__nonnull])functions withRange:(NSRange)range;

@end

