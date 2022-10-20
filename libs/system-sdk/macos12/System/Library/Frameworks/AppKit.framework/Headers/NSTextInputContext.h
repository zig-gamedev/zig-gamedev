/*
	NSTextInputContext.h
	Application Kit
	Copyright (c) 2008-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSTextInputClient.h>
#import <Foundation/NSNotification.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSEvent;

typedef NSString * NSTextInputSourceIdentifier NS_SWIFT_BRIDGED_TYPEDEF;

API_AVAILABLE(macos(10.6))
@interface NSTextInputContext : NSObject

/* The current activated text input context object. The Cocoa Text Input system communicates primarily with the client of the activated input context via the NSTextInputClient protocol.
 */
@property (class, readonly, nullable, strong) NSTextInputContext *currentInputContext;

/* The designated initializer.
 */
- (instancetype)initWithClient:(id <NSTextInputClient>)client NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE; // Use the designated initializer initWithClient:

/**** Properties *****/
/* Returns the owner of this input context. The owner, typically an NSView, retains its NSTextInputContext instance. NSTextInputContext doesn't retain its client.
 */
@property(readonly) id <NSTextInputClient> client; 

/* Indicates whether the client handles NSGlyphInfoAttributeName or not. The default value is determined by examining the return value sending -validAttributesForMarkedText to client at initialization.
 */
@property BOOL acceptsGlyphInfo;

/* Specifies the set of keyboard input source locales allowed when this input context is active. NSAllRomanInputSourcesLocaleIdentifier can be specified as a valid locale.
 */
@property(nullable, copy) NSArray<NSString *> *allowedInputSourceLocales;


/**** Activation *****/
/* Invoked when the receiver gets activated/deactivated.
 */
- (void)activate;
- (void)deactivate;

/**** Input source interface ****/
/* Tells the Cocoa Text Input system to handle mouse/key events. Returns YES if the system consumed the event.
 */
- (BOOL)handleEvent:(NSEvent *)event;

/* Notifies the system to discard the current conversion session. The client should clear its marked range when sending this message.
 */
- (void)discardMarkedText;

/* Notifies the text input system that information related to character positions, including the document's visual coordinates, text selection, and document contents, has been modified. Text engines implementing the NSTextInputClient protocol should send this message whenever any of these changes occur, and -[NSTextInputContext handleEvent:] will not otherwise be called. -handleEvent: serves as an implicit notification that any of these changes could have occurred.
 */
- (void)invalidateCharacterCoordinates;


/**** Text Input sources handling ****/
/* Text Input source handling API identifies text input sources with text input source identifier strings (i.e. "com.apple.inputmethod.Kotoeri.Japanese") supplied by the underlying TextInputSources framework. The ID corresponds to the kTISPropertyInputSourceID attribute.
 */

/* The array of keyboard text input source identifier strings available to the receiver.
 */
@property(nullable, readonly) NSArray<NSTextInputSourceIdentifier> *keyboardInputSources;

/* The identifier string for the selected keyboard text input source.
 */
@property(nullable, copy) NSTextInputSourceIdentifier selectedKeyboardInputSource;


/**** Text Input source attributes ****/
/* Returns the display name for inputSourceIdentifier.
 */
+ (nullable NSString *)localizedNameForInputSource:(NSTextInputSourceIdentifier)inputSourceIdentifier;
@end

/**** Notifications ****/
/* Notified whenever the selected text input source changes.
 */
APPKIT_EXTERN NSNotificationName NSTextInputContextKeyboardSelectionDidChangeNotification API_AVAILABLE(macos(10.6));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

