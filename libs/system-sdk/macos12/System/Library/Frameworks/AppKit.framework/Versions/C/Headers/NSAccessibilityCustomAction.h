/*
     NSAccessibilityCustomAction.h
     Application Kit
     Copyright (c) 2017-2021, Apple Inc. All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(10.13)) @interface NSAccessibilityCustomAction : NSObject

/* Creates a custom action that performs the action via a block handler.
    The block should return whether the action was executed successfully or not.
    The return status will help accessibility clients know what to communicate back to the user.
*/
- (instancetype)initWithName:(NSString *)name handler:(BOOL(^ _Nullable)(void))handler;

/* Creates a custom action that performs the action via selector/target.
    The selector should have a signature that returns a BOOL for the success status of the execution.
*/
- (instancetype)initWithName:(NSString *)name target:(id<NSObject>)target selector:(SEL)selector;

// A localized name that describes the action.  This is a string that may be displayed to the user.
@property (copy) NSString *name;

// The block that handles the execution of the action
@property (nullable, copy) BOOL (^handler)(void);

// The object that will perform the action via selector
@property (nullable, weak) id<NSObject> target;

/*
 The method that will be called on the target to perform the action.
 It must conform to one of the following signatures:
 - (BOOL)myPerformActionMethod;
 - (BOOL)myPerformActionMethod:(NSAccessibilityCustomAction *)action;
 */
@property (nullable) SEL selector;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
