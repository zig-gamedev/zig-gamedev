//
//  MTLFunctionHandle.h
//  Framework
//
//  Copyright © 2020 Apple, Inc. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <Metal/MTLDefines.h>
#import <Metal/MTLLibrary.h>

API_AVAILABLE(macos(11.0), ios(14.0))
@protocol MTLFunctionHandle <NSObject>
@property (readonly) MTLFunctionType functionType;
@property (readonly, nonnull) NSString* name;
@property (readonly, nonnull) id<MTLDevice> device;
@end

#import <Metal/MTLVisibleFunctionTable.h>

