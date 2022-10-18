/*
        NSStoryboardSegue.h
        Application Kit
        Copyright (c) 2013-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NSString * NSStoryboardSegueIdentifier NS_SWIFT_BRIDGED_TYPEDEF;

API_AVAILABLE(macos(10.10))
@interface NSStoryboardSegue : NSObject

// Convenience constructor for returning a segue that performs a handler block in its -perform method.
+ (instancetype)segueWithIdentifier:(NSStoryboardSegueIdentifier)identifier source:(id)sourceController destination:(id)destinationController performHandler:(void (^)(void))performHandler;

/* This is the designated initializer for NSStoryboardSegue. If a developer implements a NSStoryboardSegue subclass, they can override this method to perform init-time logic. When a segue starts, a new instance of NSStoryboardSegue will be created with this method, and then perform will subsequently be invoked. */
- (instancetype)initWithIdentifier:(NSStoryboardSegueIdentifier)identifier source:(id)sourceController destination:(id)destinationController NS_DESIGNATED_INITIALIZER;

/* NSStoryboardSegue instances have optional identifiers that can be assigned in Interface Builder. These identifiers can be used in overrides of -[NSViewController prepareForSegue:sender:] to differentiate segues. */
@property (nullable, readonly, copy) NSStoryboardSegueIdentifier identifier;

/* Subclasses of NSStoryboardSegue can use this property to access the source view or window controller that is being segued away from. */
@property (readonly, strong) id sourceController;

/* Subclasses of NSStoryboardSegue can use this property to access the destination view or window controller that's being segued to. This property is also essential for overrides of -[NSViewController prepareForSegue:sender:], which is passed to the source view controller. This property allows the receiver of -[NSViewController prepareForSegue:sender:] to access and pass configuration data to the destination controller. */
@property (readonly, strong) id destinationController;

/* This method is implemented by NSStoryboardSegue subclasses to present a custom transition between two controllers. */
- (void)perform;

@end

@protocol NSSeguePerforming <NSObject>

@optional

/* Subclasses should override this to do any necessary work preparing for the destination ViewController or WindowController.
 */
- (void)prepareForSegue:(NSStoryboardSegue *)segue sender:(nullable id)sender API_AVAILABLE(macos(10.10));

/* Performs the identified Segue created from this WindowController. Throws exception if identifier is nil or not associated with the receiver.
 */
- (void)performSegueWithIdentifier:(NSStoryboardSegueIdentifier)identifier sender:(nullable id)sender API_AVAILABLE(macos(10.10));

/* Invoked immediately prior to initiating a segue. Return NO to prevent the segue from firing. The default implementations in NSViewController and NSWindowController return YES. This method is invoked when -performSegueWithIdentifier:sender: is used.
 */
- (BOOL)shouldPerformSegueWithIdentifier:(NSStoryboardSegueIdentifier)identifier sender:(nullable id)sender API_AVAILABLE(macos(10.10));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
