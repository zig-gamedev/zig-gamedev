/*
	NSController.h
	Application Kit
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
 */

#import <AppKit/NSKeyValueBinding.h>
#import <AppKit/AppKitDefines.h>
#import <CoreFoundation/CoreFoundation.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSMutableArray, NSMutableDictionary, NSMutableSet;

@interface NSController : NSObject <NSCoding, NSEditor, NSEditorRegistration>

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (void)objectDidBeginEditing:(id<NSEditor>)editor;
- (void)objectDidEndEditing:(id<NSEditor>)editor;
- (void)discardEditing;
- (BOOL)commitEditing;
- (void)commitEditingWithDelegate:(nullable id)delegate didCommitSelector:(nullable SEL)didCommitSelector contextInfo:(nullable void *)contextInfo;
@property (getter=isEditing, readonly) BOOL editing;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

