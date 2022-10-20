
/*
 *  AXTextAttributedString.h
 *
 *  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 */

#ifndef AXTEXTATTRIBUTEDSTRING_H
#define AXTEXTATTRIBUTEDSTRING_H

/*! @header AXTextAttributedString.h
      @discussion This header file contains definitions of constants used with accessibility objects that represent attributed strings.
      An attributed string is an association of a range of characters and their attributes, such as color and font. If an accessibility object
      represents an attributed string, the value of its <code>kAXParameterizedAttributeStringAttribute</code> attribute is an attributed string object
      (a <code>CFAttributedStringRef</code> or an <code>NSAttributedString</code>) that uses the constants defined in this header file to define its attributes.
 */


#ifdef __cplusplus
extern "C" {
#endif

#include <CoreFoundation/CoreFoundation.h>
CF_ASSUME_NONNULL_BEGIN
    
/*! @constant kAXFontTextAttribute
      @abstract A dictionary (a <code>CFDictionaryRef</code>) of two or more font keys.
      @discussion The dictionary associated with this attribute must contain the {@link kAXFontNameKey} and
      {@link kAXFontSizeKey} font keys. It may also contain the {@link kAXFontFamilyKey} and
      {@link kAXVisibleNameKey} font keys.
 */
extern CFStringRef kAXFontTextAttribute			AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFDictionaryRef - see kAXFontTextAttribute keys below

/*! @constant kAXForegroundColorTextAttribute
      @abstract A <code>CGColorRef</code> value that describes the foreground color.
 */
extern CFStringRef kAXForegroundColorTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CGColorRef

/*! @constant kAXBackgroundColorTextAttribute
      @abstract A <code>CGColorRef</code> value that describes the background color.
 */
extern CFStringRef kAXBackgroundColorTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CGColorRef

/*! @constant kAXUnderlineColorTextAttribute
      @abstract A <code>CGColorRef</code> value that describes the color of the underlining.
 */
extern CFStringRef kAXUnderlineColorTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CGColorRef

/*! @constant kAXStrikethroughColorTextAttribute
      @abstract A <code>CGColorRef</code> value that describes the color of the strikethrough.
 */
extern CFStringRef kAXStrikethroughColorTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CGColorRef

/*! @constant kAXUnderlineTextAttribute
      @abstract A <code>CFNumberRef</code> value that describes the underline style (see {@link //apple_ref/c/tdef/AXUnderlineStyle AXUnderlineStyle} for possible values).
 */
extern CFStringRef kAXUnderlineTextAttribute		AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFNumberRef - AXUnderlineStyle

/*! @constant kAXSuperscriptTextAttribute
      @abstract A <code>CFNumberRef</code> value that indicates whether the text is superscript (value > 0) or subscript (value < 0).
 */
extern CFStringRef kAXSuperscriptTextAttribute		AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFNumberRef = + number for superscript - for subscript

/*! @constant kAXStrikethroughTextAttribute
      @abstract A <code>CFBooleanRef</code> value that indicates whether the text displays strikethrough.
 */
extern CFStringRef kAXStrikethroughTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFBooleanRef

/*! @constant kAXShadowTextAttribute
      @abstract A <code>CFBooleanRef</code> value that indicates whether the text displays shadow.
 */
extern CFStringRef kAXShadowTextAttribute		AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFBooleanRef



/*! @constant kAXAttachmentTextAttribute
      @abstract An <code>AXUIElementRef</code> value for the accessibility object representing the text attachment.
 */
extern CFStringRef kAXAttachmentTextAttribute		AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// AXUIElementRef

/*! @constant kAXLinkTextAttribute
      @abstract An <code>AXUIElementRef</code> value used to access the object to which the specified range of text links.
 */
extern CFStringRef kAXLinkTextAttribute			AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// AXUIElementRef

/*! @constant kAXNaturalLanguageTextAttribute
      @abstract A <code>CFStringRef</code> value that indicates the language in which the text will be spoken aloud by the system.
 */
extern CFStringRef kAXNaturalLanguageTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFStringRef - the spoken language of the text

/*! @constant kAXReplacementStringTextAttribute
      @abstract A <code>CFStringRef</code> value that indicates the string that will replace the text.
 */
extern CFStringRef kAXReplacementStringTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFStringRef



/*! @constant kAXMisspelledTextAttribute
      @abstract A <code>CFBooleanRef</code> value that indicates whether the text is misspelled.
 */
extern CFStringRef kAXMisspelledTextAttribute		AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// AXUIElementRef

/*! @constant kAXMarkedMisspelledTextAttribute
	  @abstract A <code>CFBooleanRef</code> value that indicates whether the text is visibly shown as misspelled
		(for example, with a squiggle underneath the text).
 */
extern CFStringRef kAXMarkedMisspelledTextAttribute		AVAILABLE_MAC_OS_X_VERSION_10_9_AND_LATER;	// AXUIElementRef



/*! @constant kAXAutocorrectedTextAttribute
      @abstract A <code>CFBooleanRef</code> value that indicates whether the text was autocorrected.
 */
extern CFStringRef kAXAutocorrectedTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER;	// CFBooleanRef



/*! @constant kAXListItemPrefixTextAttribute
      @abstract A <code>CFAttributedStringRef</code> value that indicates the prepended string of the list item. If the string is a common unicode character (e.g. a bullet ¥), return that unicode character. For lists with images before the text, return a reasonable label of the image.
 */
extern CFStringRef kAXListItemPrefixTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

/*! @constant kAXListItemIndexTextAttribute
      @abstract A <code>CFNumberRef</code> value that indicates the line index. Each list item increments the index, even for unordered lists. The first item should have index 0.
 */
extern CFStringRef kAXListItemIndexTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;

/*! @constant kAXListItemLevelTextAttribute
      @abstract A <code>CFNumberRef</code> value that indicates the indent level. Each sublist increments the level. The first item should have level 0.
 */
extern CFStringRef kAXListItemLevelTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER;



/*! @group Font Text Attribute Keys
    @discussion
        Used with {@link kAXFontTextAttribute}.
 */

/*! @constant kAXFontNameKey
      @abstract Required key of the {@link kAXFontTextAttribute} dictionary for the font name in a <code>CFStringRef</code>.
 */
extern CFStringRef kAXFontNameKey			AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFStringRef - required

/*! @constant kAXFontFamilyKey
      @abstract Optional key of the {@link kAXFontTextAttribute} dictionary for the font family name in a <code>CFStringRef</code>.
 */
extern CFStringRef kAXFontFamilyKey			AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFStringRef - not required

/*! @constant kAXVisibleNameKey
      @abstract Optional key of the {@link kAXFontTextAttribute} dictionary for the font display name in a <code>CFStringRef</code>.
 */    
extern CFStringRef kAXVisibleNameKey			AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFStringRef - not required

/*! @constant kAXFontSizeKey
      @abstract Required key of the {@link kAXFontTextAttribute} dictionary for the font size in a <code>CFNumberRef</code>.
 */    
extern CFStringRef kAXFontSizeKey			AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CFNumberRef - required


/*! @typedef AXUnderlineStyle
      @abstract Values that describe the style of underlining (used with the {@link kAXUnderlineTextAttribute} attribute).
 */
typedef CF_ENUM(UInt32, AXUnderlineStyle) {
    kAXUnderlineStyleNone	= 0x0,
    kAXUnderlineStyleSingle	= 0x1,
    kAXUnderlineStyleThick	= 0x2,
    kAXUnderlineStyleDouble	= 0x9
};


// DO NOT USE. This is an old, misspelled version of one of the above constants.
extern CFStringRef kAXForegoundColorTextAttribute	AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;	// CGColorRef

CF_ASSUME_NONNULL_END
#ifdef __cplusplus
}
#endif

#endif // AXTEXTATTRIBUTEDSTRING_H
