/*
    File:       IMKInputSession.h

    Contains:   IMKInputSession Interfaces.

    Copyright:  (c) 2006 by Apple Computer, Inc., all rights reserved.
    
*/

/*!
	@header
	@abstract	This header contains the protocol that is used to communicate with client input sessions.  An input method uses this protocol to send text or obtain information about a client's text.
*/

#ifndef __IMKINPUTSESSION__
#define __IMKINPUTSESSION__

//to use this header you must use Objective-C.  This will be part of HIToolbox/HIToolbox.h and many people who include that header
//do not use Objective-C.

#ifdef __OBJC__

#import <Foundation/Foundation.h>
#import <HIToolbox/TextServices.h>


/*!
    @enum     IMKLocationToOffsetMappingMode
    @abstract   List modes for mapping a screen location to a text offset.
    @discussion Often how a screen location should be mapped to a text offset is ambiguous.  When tracking a coordinate will not change the offset until the coordinate is completely past a given character.  When trying to place an insertion point a coordinate will be mapped to the nearest character boundary.
*/
enum {
 	kIMKMouseTrackingMode    = (1<<0),
  	kIMKNearestBoundaryMode  = (1<<1)
};
typedef NSInteger IMKLocationToOffsetMappingMode;

/*!
	@const		IMKTextOrientationName
	@abstract	The key used to find client's text orientation value which is part of the dictionary returned by attributesForCharacterIndex.

	@discussion The orientation is the orientation of the client's text.  Use this key to access the orientation value in the attributes dictionary returned by attributesForCharacterIndex. The value is an NSNumber which represents a BOOL value. A value of 1 means the client's text is laid out horizontally.  A value of 0 means that the client's text is laid out vertically.  
	
	
*/
extern const NSString* IMKTextOrientationName;

@class IMKClient;




/*!
    @protocol    IMKTextInput
    @abstract   Used to send and obtain data from the client side session.
    @discussion Input methods call one of the methods in this protocol to communicate with the client.  
*/
@protocol IMKTextInput

/*!
    @method     
    @abstract   Sends text that has been fully converted to an input session.
    @discussion When an input method finishes a conversion it calls this method and passes the finished text as an NSString or NSAttributedString. The replacementRange parameter is there to allow input methods to insert text at a location other than the current selection.
               
               If used the replacementRange should be relative to the beginning of the client document.
               
               If the string should be inserted at the current selection specify a replacementRange with a location and length of NSNotFound.

                If the client does not support the TSMDocumentAccess protocol the replacementRange string will be ignored.
*/
- (void)insertText:(id)string replacementRange:(NSRange)replacementRange;



/*!
    @method     
    @abstract   Insert the text and mark it to indicate that it is part of an active input session. 
    @discussion The string parameter must be an NSString or an NSAttributedString. Passing an NSString will produce default marking ( On a 72 dpi screen that is a 2 pixel black underline for the entire string).  SelectionRange indicates the area of the string that should be selected and therefore if possible the area that should be displayed. SelectionRange is relative to the string parameter.
                
               For example, if the string contains the these characters: "INPUT" and the range is (5,0) the selection will be set right after the "T."
                
               ReplacementRange specifies the location in the client document where the marked text should be placed.  If the marked text should be placed at the current cursor location replacementRange.location will equal NSNotFound.  If used (i.e. location is not NSNotFound) the replacementRange is relative to the client document (i.e. 0 is the beginning of the document) and NOT the string parameter.
             
               Currently, the replacement range is sent to the client via the kEventParamTextInputSendReplaceRange Carbon Event parameter.  As noted in the comment for this parameter for this parameter to be used the client must support the TSMDocumentAccess protocol.  If a client does not support the TSMDocumentAccess protocol the replacementRange will be ignored.
*/
- (void)setMarkedText:(id)string selectionRange:(NSRange)selectionRange replacementRange:(NSRange)replacementRange;



/*
    A Note on selectedRange:, attributedSubStringFromRange,  and length.
   
   These methods all depend on the client application supporting  TSMDocumentAccess Carbon events.  Many applications do not support these so input methods should be prepared for these methods to return nil or NSNotFound.  The return that can be expected is included in the  discussion of each of these methods.


*/
/*!
    @method     
    @abstract   Returns the current selection range.
                
   @discussion If the client does not support the TSMDocumentAccess protocol the returned range will have a location value of NSNotFound and a length of NSNotFound.
              
               If a valid range is returned it is relative to the client's document.
*/
- (NSRange)selectedRange;



/*!
    @method    
    @abstract   Returns the range in the document that is occupied by marked text, which in other terms is the current inline session.
    @discussion While an input method is converting text input and sending it to the client the client will maintain a range of text that is marked.  That is to say that there will be a range of text that is underlined in certain ways to indicate to the user that their keystrokes are in the process of being converted by an input method and the conversion has not yet been finalized.  The input method uses this method to request that range.  The returned range is relative to the clients document.  
    
             If this range is not available the range location and length will be NSNotFound.
*/
- (NSRange)markedRange;



/*!
    @method     
    @abstract   Called to request a string along with its attributes such as font and glyph info.
    @discussion Input methods call this method to obtain a range of text from the client document along with that text's font and glyphInfo.  The kind of attributes that can be included in the string are defined in CTStringAttributes.h.  The range should be document relative.
                                
               If the client does not support TSMDocumentAccess the returned string will be created from data obtained by sending the client application a kEventTextInputGetSelectedText Carbon event.

				The returned NSAttributedString is an autoreleased object.  Do not release this object unless you retain it first.
				
				Note it is possible that the returned string will be longer or shorter than the specified range.  This can be caused when there surrogate unicodes in a document, and the range's location or length splits a surrogate pair.
				You can test for this by comparing the string's length against the range you included as the range parameter.
*/
- (NSAttributedString*)attributedSubstringFromRange:(NSRange)range;

/*!
    @method     
    @abstract   Returns the length of the text document.
 @discussion If the client does not support the TSMDocumentAccess protocol the returned value will be NSNotFound.
   
               Please note that depending on how clients store text determing how long a text document is can  be a very expensive operation. Input methods should be aware of this problem and avoid frequently calling this method.
*/
- (NSInteger)length;



/*!
    @method     
    @abstract   Map a global point to a location in the text document.
    @discussion Given a global point (typically from a mousedown) map that to a location in a text document.  
 
               MappingMode is set to kIMKMouseTrackingMode if the input method is tracking the mouse. If the input method is tracking the mouse the application should pin highlighting to the active inline area.  If the input method simply wants to map a screen postion to an offset than mappingMode should be set to kIMKNearestBoundaryMode.
                
               If the point is not inside the text body the returned value will be NSNotFound.  If the point is inside the text body and inside the marked range inMarkedRange will be set to YES.  If it is outside the markedRange it will be set to NO.
*/
- (NSInteger)characterIndexForPoint:(NSPoint)point tracking:(IMKLocationToOffsetMappingMode)mappingMode inMarkedRange:(BOOL*)inMarkedRange;


/*!
    @method     - (NSDictionary*)attributesForCharacterIndex:(NSUInteger)index lineHeightRectangle:(NSRect*)lineRect;
    @abstract   Returns a dictionary of text attributes for the text at a given character index. The attributes include the CTFontRef for the text at that index and the text orientation.  The text orientation is indicated by an NSNumber whose value is 0 if the text is vertically oriented and 1 if the text is horizontally oriented. The key for this value is IMKTextOrientationKey. Additionally, a rectangle that would frame a one-pixel wide rectangle with the height of the line is returned in the frame parameter.  Note that rectangle will be oriented the same way the line is oriented.
    @discussion Input methods will call this method to place a candidate window on the screen. The index is relative to the inline session.  Note that if there is no inline session the value of index should be 0, which indicates that the information should be taken from the current selection.

				The returned NSDictionary is an autoreleased object.  Don't release it unless you have retained it.
    
*/
- (NSDictionary*)attributesForCharacterIndex:(NSUInteger)index lineHeightRectangle:(NSRect*)lineRect;



/*!
    @method     
    @abstract   Returns an array of NSString names for the attributes supported by the receiver. 
   @discussion Input methods should restrict the attributes used to create attributed strings to the attributes in this array.

				This is not a copy function.  The returned NSArray should not be released unless it is first retained.
*/
- (NSArray*)validAttributesForMarkedText;



/*!
    @method     
    @abstract   Override the current keyboard.
    @discussion Pass a keyboard unique name.  The client will try to locate a keyboard layout with that name in the input method's bundle.  If a layout is found it is passed to the client who then tells the text service manager to use that layout for keyboard events.  Input methods should call the method each time they are activated.
               
               If an input method wishes to use a system keyboard to override the current keyboard they are responsible for determining the keyboard's unique name.  Typically this name a DNS type name such as: com.apple.<some name>.
*/
-(void)overrideKeyboardWithKeyboardNamed:(NSString*)keyboardUniqueName;

/*!
    @method     
    @abstract   Select a new  input mode.
    @discussion The mode identifier is an NSString with a DNS format.  
               
               Typically the mode identifier is a DNS type name such as: com.<company name>.inputmethod.<some name>.  The identifier should match one of the keys in the  ComponentInputModeDict.  This method allows an input method to change its mode directly.
*/
-(void)selectInputMode:(NSString*)modeIdentifier;


/*!
    @method     
    @abstract   Test to see if the current input session supports Unicode text.
    @discussion Input methods that restrict the character codes sent to a client if that client does not support Unicode text should call this method to learn if a given input session supports Unicode text.
*/
-(BOOL)supportsUnicode;

/*!
    @method     
    @abstract   Returns the bundle identifier for the process that the input session is attached to.
    @discussion Many input methods need to be able to identify the process that input sessions belong to.  This method provides that service.

				The returned NSString is an autoreleased object.  Call retain to own it.
*/
-(NSString*)bundleIdentifier;

/*!
    @method     
    @abstract  Returns the window level for client window.
    @discussion Call this to determine the client's window level.  
    
    Internally, IMKCandidates uses this method to determine the correct level for candidate display.  Therefore, input methods that use IMKCandidates have no reason to call this method.  
    
    However, many input method developers have built there own candidate display system, and this method is here for their use.  In order to display a candidate window at the correct level use this method to obtain the client window level, increment the returned level, and then apply that level to any candidate windows.
*/
-(CGWindowLevel)windowLevel;

/*!
    @method     
    @abstract   Check if the current client input session supports a given TSM property.
    @discussion This method is available to provide compatibilty for input methods that need to check TSMDocument properties.
*/
-(BOOL)supportsProperty:(TSMDocumentPropertyTag)property;

/*!
    @method     
    @abstract   Return a unique ID for the client.
    @discussion This method is a wrapper around the NSProcessInfo method globallyUniqueString.
*/
-(NSString*)uniqueClientIdentifierString AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER;

/*!
    @method     
    @abstract   Called to request a string along.
    @discussion Input methods call this method to obtain a range of text from the client document The range should be document relative.
                                
               If the client does not support TSMDocumentAccess the returned string will be created from data obtained by sending the client application a kEventTextInputGetSelectedText Carbon event.
			   Note that clients that do not support TSMDocumentAccess are extremely rare.  Certainly the text system for Cocoa does support this functionality.

				The returned NSString is an autoreleased object.  Do not release this object unless you retain it first.
				
				Note it is possible that the returned string will be longer or shorter than the specified range.  This can be caused when there surrogate unicodes in a document, and the range's location or length splits a surrogate pair.
				The parameter actualRange will contain the real range of the string in this case.
*/
- (NSString*)stringFromRange:(NSRange)range actualRange:(NSRangePointer)actualRange AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER;


/* 
	@method     
    @abstract   Returns a rectangle for a range of text.
    @discussion Returns the first logical rectangular area for aRange. The return value is in the global  coordinate. The size value can be negative if the text flows to the left. If non-NULL, actuallRange contains the character range corresponding to the returned area.
*/
- (NSRect)firstRectForCharacterRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER;



@end

@protocol IMKUnicodeTextInput

/*!
    @method     
    @abstract   Sends unicode text to the client application.
    @discussion Use this method to send text that was not generated by user keyboard input.  This method should be used by palette input methods that send text to a client.  Typically the text is generated in response to user activity in the palette window. The string parameter can be either an NSString or an NSAttributedString.  Use an NSAttributed string if you need to include NSGlyphInfo objects with the text in the string. 
*/
-(void)insertText:(id)string;

@end

#endif
#endif

