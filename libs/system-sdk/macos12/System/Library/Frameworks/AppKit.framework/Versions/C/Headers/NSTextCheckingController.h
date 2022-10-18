/*
        NSTextCheckingController.h
        Application Kit
        Copyright (c) 2018-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSValue.h>
#import <Foundation/NSString.h>
#import <Foundation/NSAttributedString.h>
#import <Foundation/NSTextCheckingResult.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSSpellChecker.h>
#import <AppKit/NSTextCheckingClient.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(10.15))
@interface NSTextCheckingController : NSObject

- (instancetype)initWithClient:(id <NSTextCheckingClient>)client NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE; // Use the designated initializer initWithClient:

// The client, typically an NSView, retains its controller instance; the controller doesn't retain its client.
@property (readonly) id <NSTextCheckingClient> client;

// Disconnects the controller from its client and renders the controller incapable of further action.
- (void)invalidate;

// To be called after text is changed.
- (void)didChangeTextInRange:(NSRange)range;

// To be called when user input has inserted text.
- (void)insertedTextInRange:(NSRange)range;

// To be called when the selection changes.
- (void)didChangeSelectedRange;

// To be called when the client wants the controller to ensure that a particular range of text should be scheduled to undergo any needed automatic checking.
- (void)considerTextCheckingForRange:(NSRange)range;

// Manually causes checking to be started immediately for a specified range.
- (void)checkTextInRange:(NSRange)range types:(NSTextCheckingTypes)checkingTypes options:(NSDictionary<NSTextCheckingOptionKey, id> *)options;

// Methods to be used to implement various menu actions.
- (void)checkTextInSelection:(nullable id)sender;
- (void)checkTextInDocument:(nullable id)sender;
- (void)orderFrontSubstitutionsPanel:(nullable id)sender;
- (void)checkSpelling:(nullable id)sender;
- (void)showGuessPanel:(nullable id)sender;
- (void)changeSpelling:(nullable id)sender;
- (void)ignoreSpelling:(nullable id)sender;

// To be called if Touch Bar candidates need to be updated for some reason other than change in text or selection.
- (void)updateCandidates;

// Gives a list of the annotation keys that the controller uses.
- (NSArray<NSAttributedStringKey> *)validAnnotations;

// Gives contextual menu items related to text checking that are specific to the given index in the text. The client should add these before any standard contextual menu items. If the click was not on the selection, this method may also provide a new effective range for the contextual menu to operate on.
- (nullable NSMenu *)menuAtIndex:(NSUInteger)location clickedOnSelection:(BOOL)clickedOnSelection effectiveRange:(NSRangePointer)effectiveRange;

// The controller will automatically generate a spellchecker document tag for the client. With this property the client can obtain the tag to use as needed, or set its own choice of tag.
@property NSInteger spellCheckerDocumentTag;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

