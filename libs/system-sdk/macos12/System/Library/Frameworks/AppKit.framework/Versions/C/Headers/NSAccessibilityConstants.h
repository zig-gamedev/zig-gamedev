/*
	NSAccessibilityConstants.h
	Application Kit
	Copyright (c) 2001-2021, Apple Inc.
	All rights reserved.
*/
#import <AppKit/AppKitDefines.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

/*** Exception Constants ***/

// name for accessibility exception - declared in NSErrors.h
// APPKIT_EXTERN NSExceptionName NSAccessibilityException;

/* userInfo key for error codes in accessibility exceptions
 */
APPKIT_EXTERN NSString *const NSAccessibilityErrorCodeExceptionInfo;


/*** Accessibility Constants ***/

/* Standard attributes
 */
typedef NSString * NSAccessibilityAttributeName NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityRoleAttribute;		//(NSAccessibilityRole) - type, non-localized (e.g. radioButton)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityRoleDescriptionAttribute;	//(NSString *) - user readable role (e.g. "radio button")
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySubroleAttribute;		//(NSAccessibilitySubrole) - type, non-localized (e.g. closeButton)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityHelpAttribute;		//(NSString *) - instance description (e.g. a tool tip)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityValueAttribute;		//(id)         - element's value
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMinValueAttribute;		//(id)         - element's min value
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMaxValueAttribute;		//(id)         - element's max value
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityEnabledAttribute;		//(NSNumber *) - (boolValue) responds to user?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityFocusedAttribute;		//(NSNumber *) - (boolValue) has keyboard focus?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityParentAttribute;		//(id)         - element containing you
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityChildrenAttribute;		//(NSArray *)  - elements you contain
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityWindowAttribute;		//(id)         - UIElement for the containing window
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityTopLevelUIElementAttribute;	//(id)         - UIElement for the containing top level element
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedChildrenAttribute;	//(NSArray *)  - child elements which are selected
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVisibleChildrenAttribute;	//(NSArray *)  - child elements which are visible
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityPositionAttribute;		//(NSValue *)  - (pointValue) position in screen coords
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySizeAttribute;		//(NSValue *)  - (sizeValue) size
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityContentsAttribute;		//(NSArray *)  - main elements
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityTitleAttribute;		//(NSString *) - visible text (e.g. of a push button)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDescriptionAttribute;	//(NSString *) - instance description
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityShownMenuAttribute;		//(id)         - menu being displayed
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityValueDescriptionAttribute	//(NSString *)  - text description of value
API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySharedFocusElementsAttribute API_AVAILABLE(macos(10.10));  //(NSArray *)  - elements that share focus


/* Misc attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityPreviousContentsAttribute;	//(NSArray *)  - main elements
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityNextContentsAttribute;	//(NSArray *)  - main elements
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityHeaderAttribute;		//(id)         - UIElement for header.
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityEditedAttribute;		//(NSNumber *) - (boolValue) is it dirty?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityTabsAttribute;		//(NSArray *)  - UIElements for tabs
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityHorizontalScrollBarAttribute;//(id)       - UIElement for the horizontal scroller
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVerticalScrollBarAttribute;//(id)         - UIElement for the vertical scroller
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityOverflowButtonAttribute;	//(id)         - UIElement for overflow
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityIncrementButtonAttribute;	//(id)         - UIElement for increment
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDecrementButtonAttribute;	//(id)         - UIElement for decrement
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityFilenameAttribute;		//(NSString *) - filename
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityExpandedAttribute;		//(NSNumber *) - (boolValue) is expanded?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedAttribute;		//(NSNumber *) - (boolValue) is selected?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySplittersAttribute;	//(NSArray *)  - UIElements for splitters
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDocumentAttribute;		//(NSString *) - url as string - for open document
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityActivationPointAttribute;           //(NSValue *)  - (pointValue)

APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityURLAttribute;		//(NSURL *)    - url
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityIndexAttribute;		//(NSNumber *)  - (intValue)

APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityRowCountAttribute		//(NSNumber *)  - (intValue) number of rows
API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityColumnCountAttribute	//(NSNumber *)  - (intValue) number of columns
API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityOrderedByRowAttribute	//(NSNumber *)  - (boolValue) is ordered by row?
API_AVAILABLE(macos(10.5));

APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityWarningValueAttribute	//(id)  - warning value of a level indicator, typically a number
API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityCriticalValueAttribute	//(id)  - critical value of a level indicator, typically a number
API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityPlaceholderValueAttribute	//(NSString *)  - placeholder value of a control such as a text field
API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityContainsProtectedContentAttribute API_AVAILABLE(macos(10.9));   // (NSNumber *) - (boolValue) contains protected content?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityAlternateUIVisibleAttribute API_AVAILABLE(macos(10.10));  //(NSNumber *) - (boolValue)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityRequiredAttribute API_AVAILABLE(macos(10.12));  //(NSNumber *) - (boolValue) whether a form field is required to have content for successful submission of the form


/* Linkage attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityTitleUIElementAttribute;		//(id)	      - UIElement for the title
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityServesAsTitleForUIElementsAttribute; //(NSArray *) - UIElements this titles
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityLinkedUIElementsAttribute;		//(NSArray *) - corresponding UIElements


/* Text-specific attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedTextAttribute;		//(NSString *) - selected text
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedTextRangeAttribute;	//(NSValue *)  - (rangeValue) range of selected text
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityNumberOfCharactersAttribute;	//(NSNumber *) - number of characters
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVisibleCharacterRangeAttribute;	//(NSValue *)  - (rangeValue) range of visible text
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySharedTextUIElementsAttribute;	//(NSArray *)  - text views sharing text
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySharedCharacterRangeAttribute;	//(NSValue *)  - (rangeValue) part of shared text in this view
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityInsertionPointLineNumberAttribute;	//(NSNumber *) - line# containing caret
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedTextRangesAttribute	//(NSArray<NSValue *> *) - array of NSValue (rangeValue) ranges of selected text
API_AVAILABLE(macos(10.5));


/* Parameterized text-specific attributes
 */
typedef NSString * NSAccessibilityParameterizedAttributeName NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityLineForIndexParameterizedAttribute;		//(NSNumber *) - line# for char index; param:(NSNumber *)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityRangeForLineParameterizedAttribute;		//(NSValue *)  - (rangeValue) range of line; param:(NSNumber *)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityStringForRangeParameterizedAttribute;	//(NSString *) - substring; param:(NSValue * - rangeValue)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityRangeForPositionParameterizedAttribute;	//(NSValue *)  - (rangeValue) composed char range; param:(NSValue * - pointValue)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityRangeForIndexParameterizedAttribute;	//(NSValue *)  - (rangeValue) composed char range; param:(NSNumber *)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityBoundsForRangeParameterizedAttribute;	//(NSValue *)  - (rectValue) bounds of text; param:(NSValue * - rangeValue)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityRTFForRangeParameterizedAttribute;		//(NSData *)   - rtf for text; param:(NSValue * - rangeValue)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityStyleRangeForIndexParameterizedAttribute;	//(NSValue *)  - (rangeValue) extent of style run; param:(NSNumber *)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityAttributedStringForRangeParameterizedAttribute; //(NSAttributedString *) - does _not_ use attributes from Appkit/AttributedString.h


/* Text attributed string attributes and constants
 */
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityFontTextAttribute;			//(NSDictionary<NSAccessibilityFontAttributeKey, id> *)
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityForegroundColorTextAttribute;	//CGColorRef
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityBackgroundColorTextAttribute;	//CGColorRef
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityUnderlineColorTextAttribute;	//CGColorRef
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityStrikethroughColorTextAttribute;	//CGColorRef
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityUnderlineTextAttribute;		//(NSNumber *)	    - underline style
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilitySuperscriptTextAttribute;		//(NSNumber *)	    - superscript>0, subscript<0
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityStrikethroughTextAttribute;		//(NSNumber *)	    - (boolValue)
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityShadowTextAttribute;		//(NSNumber *)	    - (boolValue)
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityAttachmentTextAttribute;		//id - corresponding element
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityLinkTextAttribute;			//id - corresponding element
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityAutocorrectedTextAttribute API_AVAILABLE(macos(10.7));		//(NSNumber *)	    - (boolValue)
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityTextAlignmentAttribute API_AVAILABLE(macos(10.12));		//(NSNumber *) - (NSTextAlignment)

/* Textual list attributes and constants. Examples: unordered or ordered lists in a document.
 */
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityListItemPrefixTextAttribute    API_AVAILABLE(macos(10.11));    // NSAttributedString, the prepended string of the list item. If the string is a common unicode character (e.g. a bullet •), return that unicode character. For lists with images before the text, return a reasonable label of the image.
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityListItemIndexTextAttribute     API_AVAILABLE(macos(10.11));    // NSNumber, integerValue of the line index. Each list item increments the index, even for unordered lists. The first item should have index 0.
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityListItemLevelTextAttribute     API_AVAILABLE(macos(10.11));    // NSNumber, integerValue of the indent level. Each sublist increments the level. The first item should have level 0.

/*
 About MisspelledText attributes for attribute strings:
 
 NSAccessibilityMisspelledTextAttribute was the original attribute to indicate misspelled text. In OS X 10.4, the Cocoa text system added support for NSAccessibilityMarkedMisspelledTextAttribute, which was used to indicate a word that was visibly marked as misspelled (for example, with a red squiggle underneath); the original MisspelledText attribute could also be used on text that was not visibly marked as mispelled (for example, a misspelled word that was currently being edited by the user).
 
 Typically, a screen reader only wants to vocalize what a sighted user could see, and so the MarkedMisspelledText attribute was adopted by VoiceOver to provide feedback to the user about misspelled text. In OS X 10.9, VoiceOver has entirely stopped using the original MisspelledText attribute, and now only checks for MarkedMisspelledText.
 
 When implementing accessibility for a custom text-editing engine, you should generally provide the MarkedMisspelledText attribute in order to support VoiceOver, especially in OS X 10.9 and later. You may optionally also support the MisspelledText attribute for compatibility with other accessibility clients.
 */
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityMisspelledTextAttribute;		//(NSNumber *)	    - (boolValue)
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityMarkedMisspelledTextAttribute API_AVAILABLE(macos(10.4));	//(NSNumber *) - (boolValue)


APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityLanguageTextAttribute API_AVAILABLE(macos(10.13)); //(NSString *) - BCP-47 langugage code to identify the language of a segment of string

// This key in attributed string allows developers to return an array of localized strings to describe custom text attributes
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityCustomTextAttribute API_AVAILABLE(macos(10.13));   //(NSArray *) - NSString per custom text attribute
/*
 NSAccessibilityAnnotationTextAttribute allows annotation information to be conveyed.  It returns an array to allow overlapping annotations.  Each annotation attribute dictionary can describe itself via the set of NSAccessibilityAnnotationAttributeKeys.
    NSAccessibilityAnnotationLabel: Describe the annotation
    NSAccessibilityAnnotationElement: Annotation UIElement.  Should return this if the element is interactive.
    NSAccessibilityAnnotationLocation: The position where the annotation applies.  Generally, this is the entire range.  But in the case where the annotation, like a comment arrow, points to a position between two characters, the range is zero.  Since NSAttributedString requires adding an attribute to string of length greater than zero, the string nearest the annotation with at least length of one needs to be returned.  So NSAccessibilityAnnotationLocation is a way to indicate exactly which end of that range the annotation is intended.
 */
APPKIT_EXTERN NSAttributedStringKey const NSAccessibilityAnnotationTextAttribute API_AVAILABLE(macos(10.13));   //(NSArray *) - (NSDictionary<NSAccessibilityAnnotationAttributeKey, id> *)

typedef NSString * NSAccessibilityAnnotationAttributeKey NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityAnnotationAttributeKey const NSAccessibilityAnnotationLabel API_AVAILABLE(macos(10.13)); // required (NSString *)
APPKIT_EXTERN NSAccessibilityAnnotationAttributeKey const NSAccessibilityAnnotationElement API_AVAILABLE(macos(10.13)); // optional (id) - UIElement for the annotation object
APPKIT_EXTERN NSAccessibilityAnnotationAttributeKey const NSAccessibilityAnnotationLocation API_AVAILABLE(macos(10.13)); // optional (NSNumber *) - (NSAccessibilityAnnotationPosition)

typedef NS_ENUM(NSInteger, NSAccessibilityAnnotationPosition) {
    NSAccessibilityAnnotationPositionFullRange,
    NSAccessibilityAnnotationPositionStart,
    NSAccessibilityAnnotationPositionEnd,
} API_AVAILABLE(macos(10.13));

typedef NSString * NSAccessibilityFontAttributeKey NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityFontAttributeKey const NSAccessibilityFontNameKey;	// required
APPKIT_EXTERN NSAccessibilityFontAttributeKey const NSAccessibilityFontFamilyKey;	// optional
APPKIT_EXTERN NSAccessibilityFontAttributeKey const NSAccessibilityVisibleNameKey;	// optional
APPKIT_EXTERN NSAccessibilityFontAttributeKey const NSAccessibilityFontSizeKey;	// required


/* Window-specific attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMainAttribute;		//(NSNumber *) - (boolValue) is it the main window?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMinimizedAttribute;	//(NSNumber *) - (boolValue) is window minimized?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityCloseButtonAttribute;	//(id) - UIElement for close box (or nil)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityZoomButtonAttribute;	//(id) - UIElement for zoom box (or nil)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMinimizeButtonAttribute;	//(id) - UIElement for miniaturize box (or nil)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityToolbarButtonAttribute;	//(id) - UIElement for toolbar box (or nil)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityProxyAttribute;		//(id) - UIElement for title's icon (or nil)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityGrowAreaAttribute;		//(id) - UIElement for grow box (or nil)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityModalAttribute;		//(NSNumber *) - (boolValue) is the window modal
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDefaultButtonAttribute;	//(id) - UIElement for default button
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityCancelButtonAttribute;	//(id) - UIElement for cancel button
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityFullScreenButtonAttribute API_AVAILABLE(macos(10.7)); //(id) - UIElement for full screen button (or nil)

/* Application-specific attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMenuBarAttribute;		//(id)         - UIElement for the menu bar
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityWindowsAttribute;		//(NSArray *)  - UIElements for the windows
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityFrontmostAttribute;	//(NSNumber *) - (boolValue) is the app active?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityHiddenAttribute;		//(NSNumber *) - (boolValue) is the app hidden?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMainWindowAttribute;	//(id)         - UIElement for the main window.
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityFocusedWindowAttribute;	//(id)         - UIElement for the key window.
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityFocusedUIElementAttribute;	//(id)         - Currently focused UIElement.
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityExtrasMenuBarAttribute API_AVAILABLE(macos(10.8));    //(id)         - UIElement for the application extras menu bar.

typedef NS_ENUM(NSInteger, NSAccessibilityOrientation) {
    NSAccessibilityOrientationUnknown = 0,
    NSAccessibilityOrientationVertical = 1,
    NSAccessibilityOrientationHorizontal = 2,
} API_AVAILABLE(macos(10.10));

APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityOrientationAttribute;	//(NSAccessibilityOrientationValue)
typedef NSString * NSAccessibilityOrientationValue NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityOrientationValue const NSAccessibilityVerticalOrientationValue;
APPKIT_EXTERN NSAccessibilityOrientationValue const NSAccessibilityHorizontalOrientationValue;
APPKIT_EXTERN NSAccessibilityOrientationValue const NSAccessibilityUnknownOrientationValue;


APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityColumnTitlesAttribute;	//(NSArray *)  - UIElements for titles

APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySearchButtonAttribute;	//(id)         - UIElement for search field search btn
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySearchMenuAttribute;	//(id)         - UIElement for search field menu
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityClearButtonAttribute;	//(id)         - UIElement for search field clear btn


/* Table/outline view attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityRowsAttribute;		//(NSArray *)  - UIElements for rows
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVisibleRowsAttribute;	//(NSArray *)  - UIElements for visible rows
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedRowsAttribute;	//(NSArray *)  - UIElements for selected rows
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityColumnsAttribute;		//(NSArray *)  - UIElements for columns
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVisibleColumnsAttribute;	//(NSArray *)  - UIElements for visible columns
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedColumnsAttribute;	//(NSArray *)  - UIElements for selected columns
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySortDirectionAttribute;	//(NSAccessibilitySortDirectionValue)

/* Cell-based table attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilitySelectedCellsAttribute API_AVAILABLE(macos(10.6));	//(NSArray *)  - UIElements for selected cells
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVisibleCellsAttribute API_AVAILABLE(macos(10.6));	//(NSArray *)  - UIElements for visible cells
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityRowHeaderUIElementsAttribute API_AVAILABLE(macos(10.6));	//(NSArray *)  - UIElements for row headers
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityColumnHeaderUIElementsAttribute API_AVAILABLE(macos(10.6));	//(NSArray *)  - UIElements for column headers

/* Cell-based table parameterized attributes.  The parameter for this attribute is an NSArray containing two NSNumbers, the first NSNumber specifies the column index, the second NSNumber specifies the row index.
 */
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityCellForColumnAndRowParameterizedAttribute API_AVAILABLE(macos(10.6));  // (id) - UIElement for cell at specified row and column

/* Cell attributes.  The index range contains both the starting index, and the index span in a table.
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityRowIndexRangeAttribute API_AVAILABLE(macos(10.6));	//(NSValue *)  - (rangeValue) location and row span
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityColumnIndexRangeAttribute API_AVAILABLE(macos(10.6));   //(NSValue *)  - (rangeValue) location and column span

/* Layout area attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityHorizontalUnitsAttribute API_AVAILABLE(macos(10.6));   //(NSAccessibilityRulerUnitValue)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVerticalUnitsAttribute API_AVAILABLE(macos(10.6));	    //(NSAccessibilityRulerUnitValue)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityHorizontalUnitDescriptionAttribute API_AVAILABLE(macos(10.6)); //(NSString *)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityVerticalUnitDescriptionAttribute API_AVAILABLE(macos(10.6));    //(NSString *)

/* Layout area parameterized attributes
 */
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityLayoutPointForScreenPointParameterizedAttribute API_AVAILABLE(macos(10.6)); //(NSValue *)  - (pointValue); param:(NSValue * - pointValue)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityLayoutSizeForScreenSizeParameterizedAttribute API_AVAILABLE(macos(10.6)); //(NSValue *)  - (sizeValue); param:(NSValue * - sizeValue)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityScreenPointForLayoutPointParameterizedAttribute API_AVAILABLE(macos(10.6)); //(NSValue *)  - (pointValue); param:(NSValue * - pointValue)
APPKIT_EXTERN NSAccessibilityParameterizedAttributeName const NSAccessibilityScreenSizeForLayoutSizeParameterizedAttribute API_AVAILABLE(macos(10.6)); //(NSValue *)  - (sizeValue); param:(NSValue * - sizeValue)

/* Layout item attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityHandlesAttribute API_AVAILABLE(macos(10.6)); //(NSArray *)  - UIElements for handles

/* Sort direction values
 */
typedef NSString * NSAccessibilitySortDirectionValue NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilitySortDirectionValue const NSAccessibilityAscendingSortDirectionValue;
APPKIT_EXTERN NSAccessibilitySortDirectionValue const NSAccessibilityDescendingSortDirectionValue;
APPKIT_EXTERN NSAccessibilitySortDirectionValue const NSAccessibilityUnknownSortDirectionValue;

typedef NS_ENUM(NSInteger, NSAccessibilitySortDirection) {
    NSAccessibilitySortDirectionUnknown = 0,
    NSAccessibilitySortDirectionAscending = 1,
    NSAccessibilitySortDirectionDescending = 2,
} API_AVAILABLE(macos(10.10));

/* Outline attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDisclosingAttribute;	//(NSNumber *) - (boolValue) is disclosing rows?
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDisclosedRowsAttribute;	//(NSArray *)  - UIElements for disclosed rows
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDisclosedByRowAttribute;	//(id)         - UIElement for disclosing row
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityDisclosureLevelAttribute;	//(NSNumber *) - indentation level

/* Slider attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityAllowedValuesAttribute;	//(NSArray<NSNumber *> *) - array of allowed values
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityLabelUIElementsAttribute;	//(NSArray *) - array of label UIElements
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityLabelValueAttribute;	//(NSNumber *) - value of a label UIElement

/* Matte attributes
 */
// Attributes no longer supported
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMatteHoleAttribute API_DEPRECATED("", macos(10.1,10.10));		//(NSValue *) - (rect value) bounds of matte hole in screen coords
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMatteContentUIElementAttribute API_DEPRECATED("", macos(10.1,10.10)); //(id) - UIElement clipped by the matte

/* Ruler view attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMarkerUIElementsAttribute;	//(NSArray *)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMarkerValuesAttribute;		//
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMarkerGroupUIElementAttribute;	//(id)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityUnitsAttribute;			//(NSAccessibilityRulerUnitValue)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityUnitDescriptionAttribute;	//(NSString *)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMarkerTypeAttribute;		//(NSAccessibilityRulerMarkerTypeValue)
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityMarkerTypeDescriptionAttribute;	//(NSString *)

/* UI element identification attributes
 */
APPKIT_EXTERN NSAccessibilityAttributeName const NSAccessibilityIdentifierAttribute		//(NSString *)
API_AVAILABLE(macos(10.7));

/* Ruler marker type values
 */
typedef NSString * NSAccessibilityRulerMarkerTypeValue NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityLeftTabStopMarkerTypeValue;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityRightTabStopMarkerTypeValue;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityCenterTabStopMarkerTypeValue;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityDecimalTabStopMarkerTypeValue;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityHeadIndentMarkerTypeValue;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityTailIndentMarkerTypeValue;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityFirstLineIndentMarkerTypeValue;
APPKIT_EXTERN NSAccessibilityRulerMarkerTypeValue const NSAccessibilityUnknownMarkerTypeValue;

typedef NS_ENUM(NSInteger, NSAccessibilityRulerMarkerType) {
    NSAccessibilityRulerMarkerTypeUnknown = 0,
    NSAccessibilityRulerMarkerTypeTabStopLeft = 1,
    NSAccessibilityRulerMarkerTypeTabStopRight = 2,
    NSAccessibilityRulerMarkerTypeTabStopCenter = 3,
    NSAccessibilityRulerMarkerTypeTabStopDecimal = 4,
    NSAccessibilityRulerMarkerTypeIndentHead = 5,
    NSAccessibilityRulerMarkerTypeIndentTail = 6,
    NSAccessibilityRulerMarkerTypeIndentFirstLine = 7
} API_AVAILABLE(macos(10.10));

/* Ruler unit values
 */
typedef NSString * NSAccessibilityRulerUnitValue NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityRulerUnitValue const NSAccessibilityInchesUnitValue;
APPKIT_EXTERN NSAccessibilityRulerUnitValue const NSAccessibilityCentimetersUnitValue;
APPKIT_EXTERN NSAccessibilityRulerUnitValue const NSAccessibilityPointsUnitValue;
APPKIT_EXTERN NSAccessibilityRulerUnitValue const NSAccessibilityPicasUnitValue;
APPKIT_EXTERN NSAccessibilityRulerUnitValue const NSAccessibilityUnknownUnitValue;

typedef NS_ENUM(NSInteger, NSAccessibilityUnits) {
    NSAccessibilityUnitsUnknown = 0,
    NSAccessibilityUnitsInches = 1,
    NSAccessibilityUnitsCentimeters = 2,
    NSAccessibilityUnitsPoints = 3,
    NSAccessibilityUnitsPicas = 4
} API_AVAILABLE(macos(10.10));

/* Actions
 */
typedef NSString * NSAccessibilityActionName NS_TYPED_EXTENSIBLE_ENUM;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityPressAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityIncrementAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityDecrementAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityConfirmAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityPickAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityCancelAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityRaiseAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityShowMenuAction;
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityDeleteAction;

/* Actions that allow the developer to present either alternative or original UI. There may be new UI elements that appear. There may be UI elements that disappear. There may be changes to existing UI elements. Or a combination of them. Currently this is typically seen during a mouse hovering event.
 */
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityShowAlternateUIAction API_AVAILABLE(macos(10.9));
APPKIT_EXTERN NSAccessibilityActionName const NSAccessibilityShowDefaultUIAction API_AVAILABLE(macos(10.9));

/* Notifications
 */
typedef NSString * NSAccessibilityNotificationName NS_TYPED_ENUM;

/* Focus notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityMainWindowChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityFocusedWindowChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityFocusedUIElementChangedNotification;

/* Application notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityApplicationActivatedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityApplicationDeactivatedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityApplicationHiddenNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityApplicationShownNotification;

/* Window notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityWindowCreatedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityWindowMovedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityWindowResizedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityWindowMiniaturizedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityWindowDeminiaturizedNotification;

/* Drawer & sheet notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityDrawerCreatedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilitySheetCreatedNotification;

/* Element notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityUIElementDestroyedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityValueChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityTitleChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityResizedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityMovedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityCreatedNotification;

/* Used when UI changes require the attention of assistive application.  Pass along a user info dictionary with the key NSAccessibilityUIElementsKey and an array of elements that have been added or changed as a result of this layout change.
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityLayoutChangedNotification API_AVAILABLE(macos(10.9));

/* Misc notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityHelpTagCreatedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilitySelectedTextChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityRowCountChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilitySelectedChildrenChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilitySelectedRowsChangedNotification;
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilitySelectedColumnsChangedNotification;

APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityRowExpandedNotification API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityRowCollapsedNotification API_AVAILABLE(macos(10.6));

/* Cell-table notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilitySelectedCellsChangedNotification API_AVAILABLE(macos(10.6));

/* Layout area notifications
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityUnitsChangedNotification API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilitySelectedChildrenMovedNotification API_AVAILABLE(macos(10.6));

/* This notification allows an application to request that an announcement be made to the user by an assistive application such as VoiceOver.  The notification requires a user info dictionary with the key NSAccessibilityAnnouncementKey and the announcement as a localized string.  In addition, the key NSAccessibilityAnnouncementPriorityKey should also be used to help an assistive application determine the importance of this announcement.  This notification should be posted for the application element.
 */
APPKIT_EXTERN NSAccessibilityNotificationName const NSAccessibilityAnnouncementRequestedNotification API_AVAILABLE(macos(10.7));


/* Roles
 */
typedef NSString * NSAccessibilityRole NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityUnknownRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityButtonRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityRadioButtonRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityCheckBoxRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilitySliderRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityTabGroupRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityTextFieldRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityStaticTextRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityTextAreaRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityScrollAreaRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityPopUpButtonRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityMenuButtonRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityTableRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityApplicationRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityGroupRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityRadioGroupRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityListRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityScrollBarRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityValueIndicatorRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityImageRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityMenuBarRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityMenuBarItemRole API_AVAILABLE(macos(10.12));
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityMenuRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityMenuItemRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityColumnRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityRowRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityToolbarRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityBusyIndicatorRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityProgressIndicatorRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityWindowRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityDrawerRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilitySystemWideRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityOutlineRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityIncrementorRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityBrowserRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityComboBoxRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilitySplitGroupRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilitySplitterRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityColorWellRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityGrowAreaRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilitySheetRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityHelpTagRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityMatteRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityRulerRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityRulerMarkerRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityLinkRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityDisclosureTriangleRole API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityGridRole API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityRelevanceIndicatorRole;
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityLevelIndicatorRole API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityCellRole API_AVAILABLE(macos(10.6)); // As found in a cell-based table
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityPopoverRole API_AVAILABLE(macos(10.7));
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityPageRole API_AVAILABLE(macos(10.13));

/* Layout-area roles
 */
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityLayoutAreaRole API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityLayoutItemRole API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilityHandleRole API_AVAILABLE(macos(10.6));

/* Subroles
 */
typedef NSString * NSAccessibilitySubrole NS_TYPED_ENUM;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityUnknownSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityCloseButtonSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityZoomButtonSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityMinimizeButtonSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityToolbarButtonSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityTableRowSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityOutlineRowSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilitySecureTextFieldSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityStandardWindowSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityDialogSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilitySystemDialogSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityFloatingWindowSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilitySystemFloatingWindowSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityIncrementArrowSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityDecrementArrowSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityIncrementPageSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityDecrementPageSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilitySearchFieldSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityTextAttachmentSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityTextLinkSubrole;
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityTimelineSubrole API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilitySortButtonSubrole API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityRatingIndicatorSubrole API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityContentListSubrole API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityDefinitionListSubrole API_AVAILABLE(macos(10.6));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityFullScreenButtonSubrole API_AVAILABLE(macos(10.7));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityToggleSubrole API_AVAILABLE(macos(10.9));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilitySwitchSubrole API_AVAILABLE(macos(10.9));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityDescriptionListSubrole API_AVAILABLE(macos(10.9));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityTabButtonSubrole API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilityCollectionListSubrole API_AVAILABLE(macos(10.13));
APPKIT_EXTERN NSAccessibilitySubrole const NSAccessibilitySectionListSubrole API_AVAILABLE(macos(10.13));

/* Below are keys used for the user info dictionary of the NSAccessibilityPostNotificationWithUserInfo API */
typedef NSString * NSAccessibilityNotificationUserInfoKey NS_TYPED_ENUM;
/* Key used in the user info dictionary for notifications.  The value is an array of elements that is associated with the notification.  For example, a NSAccessibilityLayoutChangedNotification should include an array of elements that have been added/changed using this key.
 */
APPKIT_EXTERN NSAccessibilityNotificationUserInfoKey const NSAccessibilityUIElementsKey API_AVAILABLE(macos(10.9));

/* Key used in the user info dictionary for any relevant notification.  This gives the assistive application client an opportunity to determine how to handle the corresponding notification based on the priority.  For example, a developer should pass the priority key for NSAccessibilityAnnouncementRequestedNotification.  Then clients such as VoiceOver can decide to speak the announcement immediately or after the current speech is completed based on the priority level.  This key is required for NSAccessibilityAnnouncementRequestedNotification.  The NSAccessibilityLayoutChangedNotification is another example where priority can help assistive applications determine how to handle the UI change.
 */
APPKIT_EXTERN NSAccessibilityNotificationUserInfoKey const NSAccessibilityPriorityKey API_AVAILABLE(macos(10.9));

/* Key used in the user info dictionary sent with the NSAccessibilityAnnouncementRequestedNotification.  The value for the announcement should be a localized string.  This should generally be used in conjunction with the NSAccessibilityPriorityKey to help assistive applications determine the importance of this announcement.
 */
APPKIT_EXTERN NSAccessibilityNotificationUserInfoKey const NSAccessibilityAnnouncementKey API_AVAILABLE(macos(10.7));


/* This function allows an accessibility notification to be posted with a user info dictionary.  The user info dictionary can be nil.  Valid contents of the user info dictionary are limited to classes which can be returned to an accessibility client.  That list currently includes NSString, NSNumber, NSArray, NSValues of points, ranges, sizes, rects, and valid NSAccessibility objects.  Most accessibility notifications do not require a user info dictionary.
 */
APPKIT_EXTERN void NSAccessibilityPostNotificationWithUserInfo(id element, NSAccessibilityNotificationName notification, NSDictionary<NSAccessibilityNotificationUserInfoKey, id> * _Nullable userInfo) API_AVAILABLE(macos(10.7));



/* System defined priority levels.  This priority level should be included for every NSAccessibilityAnnouncementRequestedNotification.
 */
typedef NS_ENUM(NSInteger, NSAccessibilityPriorityLevel) {
    NSAccessibilityPriorityLow = 10,
    NSAccessibilityPriorityMedium = 50,
    NSAccessibilityPriorityHigh = 90
} API_AVAILABLE(macos(10.9));


// token type for loading search element
typedef id<NSSecureCoding, NSObject> NSAccessibilityLoadingToken;

/* Deprecated
 */
APPKIT_EXTERN NSAccessibilityRole const NSAccessibilitySortButtonRole API_DEPRECATED("", macos(10.4,10.6));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
