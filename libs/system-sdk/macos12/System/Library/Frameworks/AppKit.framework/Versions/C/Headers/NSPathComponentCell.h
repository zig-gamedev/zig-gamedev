/*
    NSPathComponentCell.h
    Application Kit
    Copyright (c) 2006-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <AppKit/NSTextFieldCell.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSImage, NSString, NSURL;

/* Use the 'stringValue' or 'attributedStringValue' to set the display title for this component cell. Use 'image' and 'setImage:' to control the icon displayed for this component cell.
*/

API_AVAILABLE(macos(10.5))
@interface NSPathComponentCell : NSTextFieldCell

/* See NSPathComponent.h for details on the image & URL properties.
*/
@property (nullable, strong) NSImage *image;

@property (nullable, copy) NSURL *URL;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

