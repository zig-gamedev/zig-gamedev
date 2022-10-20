/*
	NSSpeechRecognizer.h
	Application Kit
	Copyright (c) 2003-2021, Apple Inc.
	All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString;
@protocol NSSpeechRecognizerDelegate;


@interface NSSpeechRecognizer : NSObject

- (nullable instancetype)init;

- (void)startListening;
- (void)stopListening;

@property (nullable, weak) id<NSSpeechRecognizerDelegate> delegate;

@property (nullable, copy) NSArray<NSString *> *commands;

@property (nullable, copy) NSString *displayedCommandsTitle;

@property BOOL listensInForegroundOnly;

@property BOOL blocksOtherRecognizers;

@end

@protocol NSSpeechRecognizerDelegate <NSObject>
@optional
- (void)speechRecognizer:(NSSpeechRecognizer *)sender didRecognizeCommand:(NSString *)command;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

