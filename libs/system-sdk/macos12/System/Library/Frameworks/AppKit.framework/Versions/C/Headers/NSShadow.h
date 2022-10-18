#if !__has_include(<UIFoundation/NSShadow.h>)
/*
        NSShadow.h
        Application Kit
	Copyright (c) 2002-2021, Apple Inc.
        All rights reserved.
*/

/* NSShadow stores the properties of a drop shadow to be added to drawing.  */

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

@class NSColor;

API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0)) @interface NSShadow : NSObject <NSCopying, NSSecureCoding>

- (instancetype)init NS_DESIGNATED_INITIALIZER;     // designated initializer, returns a default shadow with zero offset, zero blur radius, and default color

@property NSSize shadowOffset;  // offset in user space of the shadow from the original drawing, in default user space units, where positive values are up and to the right

@property CGFloat shadowBlurRadius;      // blur radius of the shadow in default user space units

@property(nullable, copy) NSColor *shadowColor;   // color used for the shadow (default is black with an alpha value of 1/3)

- (void)set;

@end


#endif // !TARGET_OS_IPHONE

NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSShadow.h>
#endif
