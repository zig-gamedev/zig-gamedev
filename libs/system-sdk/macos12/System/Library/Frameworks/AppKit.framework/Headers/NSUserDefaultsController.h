/*
	NSUserDefaultsController.h
	Application Kit
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
 */

#import <Foundation/NSDictionary.h>
#import <AppKit/NSController.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSUserDefaults;

@interface NSUserDefaultsController : NSController

@property (class, readonly, strong) NSUserDefaultsController *sharedUserDefaultsController;

- (instancetype)initWithDefaults:(nullable NSUserDefaults *)defaults initialValues:(nullable NSDictionary<NSString *, id> *)initialValues NS_DESIGNATED_INITIALIZER;    // if defaults is nil, the controller uses [NSUserDefaults standardUserDefaults]; if initialValues is nil, revertToInitialValues: has no effect
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (readonly, strong) NSUserDefaults *defaults;
@property (nullable, copy) NSDictionary<NSString *, id> *initialValues;

@property BOOL appliesImmediately;   // default: YES

@property (readonly) BOOL hasUnappliedChanges;

@property (readonly, strong) id values;    // accessor object for default values (independent of whether they are derived directly from the NSUserDefaults or from the initial values)

- (IBAction)revert:(nullable id)sender;
- (IBAction)save:(nullable id)sender;    // no effect if applies immediately
- (IBAction)revertToInitialValues:(nullable id)sender;    // no effect if no initial values are specified

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

