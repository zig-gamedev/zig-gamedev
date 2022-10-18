/*
	NSInputManager.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSRange.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSArray, NSAttributedString, NSEvent, NSInputServer, NSImage;

/* NSTextInput is deprecated in Mac OS X 10.6. Use NSTextInputClient instead.
 */
@protocol NSTextInput

- (void) insertText:(null_unspecified id)string API_DEPRECATED("", macos(10.0,10.6)); // instead of keyDown: string can be NSString or NSAttributedString
- (void) doCommandBySelector:(null_unspecified SEL)selector API_DEPRECATED("", macos(10.0,10.6));
	// setMarkedText: cannot take a nil first argument. string can be NSString or NSAttributedString
- (void) setMarkedText:(null_unspecified id)string selectedRange:(NSRange)selRange API_DEPRECATED("", macos(10.0,10.6));

- (void) unmarkText API_DEPRECATED("", macos(10.0,10.6));
- (BOOL) hasMarkedText API_DEPRECATED("", macos(10.0,10.6));
- (NSInteger) conversationIdentifier API_DEPRECATED("", macos(10.0,10.6));

/* Returns attributed string at the range.  This allows input mangers to query any range in backing-store.  May return nil.
*/
- (null_unspecified NSAttributedString *) attributedSubstringFromRange:(NSRange)range API_DEPRECATED("", macos(10.0,10.6));

/* This method returns the range for marked region.  If hasMarkedText == false, it'll return NSNotFound location & 0 length range.
*/
- (NSRange) markedRange API_DEPRECATED("", macos(10.0,10.6));

/* This method returns the range for selected region.  Just like markedRange method, its location field contains char index from the text beginning.
*/
- (NSRange) selectedRange API_DEPRECATED("", macos(10.0,10.6));

/* This method returns the first frame of rects for range in screen coordindate system.
*/
- (NSRect) firstRectForCharacterRange:(NSRange)range API_DEPRECATED("", macos(10.0,10.6));

/* This method returns the index for character that is nearest to point.  thPoint is in screen coordinate system.
*/
- (NSUInteger)characterIndexForPoint:(NSPoint)point API_DEPRECATED("", macos(10.0,10.6));

/* This method is the key to attribute extension.  We could add new attributes through this method. NSInputServer examines the return value of this method & constructs appropriate attributed string.
*/
- (null_unspecified NSArray*) validAttributesForMarkedText API_DEPRECATED("", macos(10.0,10.6));

@end

/* NSInputManager is deprecated in Mac OS X 10.6. Use NSTextInputContext instead. 
 */
API_DEPRECATED("Use NSTextInputContext instead", macos(10.0,10.6))
@interface NSInputManager : NSObject <NSTextInput>

/* The "current input manager" is the one that is receiving input events at the time this method is called.  It may change out from under you, so don't cache the return value.
*/
+ (null_unspecified NSInputManager *) currentInputManager API_DEPRECATED("", macos(10.0,10.6));

/* Cycle through list of input managers. Called from NSKeyBindingManager via doCommandBySelector:.
*/
+ (void)cycleToNextInputLanguage:(nullable id)sender API_DEPRECATED("", macos(10.0,10.6));
+ (void)cycleToNextInputServerInLanguage:(nullable id)sender API_DEPRECATED("", macos(10.0,10.6));

- (null_unspecified NSInputManager *)initWithName:(null_unspecified NSString *)inputServerName host:(null_unspecified NSString *)hostName API_DEPRECATED("", macos(10.0,10.6));

- (null_unspecified NSString *)localizedInputManagerName API_DEPRECATED("", macos(10.0,10.6));

/* These messages are sent by Views that conform to the NSTextInput protocol TO the Current Input Manager when things happen via user or programmatic action.  E.g., when the mouse moves outside the marked range, send markedTextWillBeAbandoned:.  If the user selects some new text or moves the mouse within the marked region, send markedTextSelectionChanged:.  Not all input manager/server combinations will allow all changes, but abandoning of the marked region cannot be aborted.
*/ 

- (void)markedTextAbandoned:(null_unspecified id)cli API_DEPRECATED("", macos(10.0,10.6)); /* send after abandoning */
- (void)markedTextSelectionChanged:(NSRange)newSel client:(null_unspecified id)cli API_DEPRECATED("", macos(10.0,10.6)); /* send after changing */

/* This corresponds to a server method for input managers that demand to do their own interepretation of command keys as long as they're active.  This will typically be called by a key binder to find out whether it shouldn't just pass along strings.
*/
- (BOOL)wantsToInterpretAllKeystrokes API_DEPRECATED("", macos(10.0,10.6));

- (null_unspecified NSString*)language API_DEPRECATED("", macos(10.0,10.6));

- (null_unspecified NSImage *)image API_DEPRECATED("", macos(10.0,10.6));

- (null_unspecified NSInputServer *) server API_DEPRECATED("", macos(10.0,10.6));

/* If corresponding input server wants to handle mouse events within marked region, this should return YES.  In that case, handleMouseEvent is sent. Otherwiese, mouse events are handled by first responder.
*/
- (BOOL)wantsToHandleMouseEvents API_DEPRECATED("", macos(10.0,10.6));

- (BOOL)handleMouseEvent:(null_unspecified NSEvent*)mouseEvent API_DEPRECATED("", macos(10.0,10.6));

/* This should return YES when the input method (language) prefers to delay text change notification 'till the input is actually committed.
*/
- (BOOL)wantsToDelayTextChangeNotifications API_DEPRECATED("", macos(10.0,10.6));
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
