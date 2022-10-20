/*
        NSTextStorageScripting.h
        AppKit Framework
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/NSTextStorage.h>

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

@interface NSTextStorage (Scripting)

@property (copy) NSArray<NSTextStorage *> *attributeRuns;

@property (copy) NSArray<NSTextStorage *> *paragraphs;

@property (copy) NSArray<NSTextStorage *> *words;

@property (copy) NSArray<NSTextStorage *> *characters;

@property (nullable, strong) NSFont *font;

@property (nullable, strong) NSColor *foregroundColor;

@end
#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
