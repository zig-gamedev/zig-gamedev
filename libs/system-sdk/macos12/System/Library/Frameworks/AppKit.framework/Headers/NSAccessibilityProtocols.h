/*
 NSAccessibilityProtocols.h
 Application Kit
 Copyright (c) 2013-2021, Apple Inc.
 All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <AppKit/NSAccessibilityConstants.h>
#import <AppKit/NSAccessibilityCustomAction.h>
#import <AppKit/AppKitDefines.h>

@class NSAccessibilityCustomRotor;

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityElement <NSObject>
@required
- (NSRect)accessibilityFrame;
- (nullable id)accessibilityParent;

@optional
- (BOOL)isAccessibilityFocused;
- (NSString *)accessibilityIdentifier;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityGroup <NSAccessibilityElement>
@end

#pragma mark Buttons

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityButton <NSAccessibilityElement>
@required
- (nullable NSString *)accessibilityLabel;
- (BOOL)accessibilityPerformPress;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilitySwitch <NSAccessibilityButton>
// Post NSAccessibilityValueChangedNotification when the value changes

@required
- (nullable NSString *)accessibilityValue;

@optional
- (BOOL)accessibilityPerformIncrement;
- (BOOL)accessibilityPerformDecrement;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityRadioButton <NSAccessibilityButton>
// NSAccessibilityValueChangedNotification should be posted whenever accessibilityValue changes

@required
// Return @YES if selected, or @NO if not selected
- (nullable NSNumber *)accessibilityValue;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityCheckBox <NSAccessibilityButton>
// NSAccessibilityValueChangedNotification should be posted whenever accessibilityValue changes

@required
// Return @YES if checked, or @NO if unchecked
- (nullable NSNumber *)accessibilityValue;
@end

#pragma mark Text
NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityStaticText <NSAccessibilityElement>
@required
- (nullable NSString *)accessibilityValue;

@optional
- (nullable NSAttributedString *)accessibilityAttributedStringForRange:(NSRange)range;
- (NSRange)accessibilityVisibleCharacterRange;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityNavigableStaticText <NSAccessibilityStaticText>
@required
- (nullable NSString *)accessibilityStringForRange:(NSRange)range;
- (NSInteger)accessibilityLineForIndex:(NSInteger)index;
- (NSRange)accessibilityRangeForLine:(NSInteger)lineNumber;

// Frame is in screen coordinates. See NSAccessibilityFrameInView()
- (NSRect)accessibilityFrameForRange:(NSRange)range;
@end

#pragma mark Miscellaneous
NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityProgressIndicator <NSAccessibilityGroup>
@required
- (nullable NSNumber *)accessibilityValue;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityStepper <NSAccessibilityElement>
@required
- (nullable NSString *)accessibilityLabel;
- (BOOL)accessibilityPerformIncrement;
- (BOOL)accessibilityPerformDecrement;
@optional
// Value is a NSString or a NSNumber
- (nullable id)accessibilityValue;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilitySlider <NSAccessibilityElement>
@required
- (nullable NSString *)accessibilityLabel;
// Value is a NSString or a NSNumber
- (nullable id)accessibilityValue;
- (BOOL)accessibilityPerformIncrement;
- (BOOL)accessibilityPerformDecrement;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityImage <NSAccessibilityElement>
@required
- (nullable NSString *)accessibilityLabel;
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityContainsTransientUI <NSAccessibilityElement>
@required
- (BOOL)accessibilityPerformShowAlternateUI;
- (BOOL)accessibilityPerformShowDefaultUI;
- (BOOL)isAccessibilityAlternateUIVisible;
@end

@protocol NSAccessibilityRow;

#pragma mark Tables, Lists, Outlines
NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityTable <NSAccessibilityGroup>
@required
- (nullable NSString *)accessibilityLabel;

// An array of accessibility row elements
- (nullable NSArray<id<NSAccessibilityRow>> *)accessibilityRows;

@optional
// For tables that have a selection
// Make sure to post a selected-rows-changed notification when appropriate
// NSAccessibilityPostNotification(table, NSAccessibilitySelectedRowsChangedNotification);
- (nullable NSArray<id<NSAccessibilityRow>> *)accessibilitySelectedRows;
- (void)setAccessibilitySelectedRows:(NSArray<id<NSAccessibilityRow>> *)selectedRows;

// Advanced table accessibility
- (nullable NSArray<id<NSAccessibilityRow>> *)accessibilityVisibleRows;
- (nullable NSArray *)accessibilityColumns;
- (nullable NSArray *)accessibilityVisibleColumns;
- (nullable NSArray *)accessibilitySelectedColumns;

- (nullable NSString *)accessibilityHeaderGroup API_DEPRECATED_WITH_REPLACEMENT("accessibilityHeader", macos(10.9,10.14));

- (nullable NSArray *)accessibilitySelectedCells;
- (nullable NSArray *)accessibilityVisibleCells;
- (nullable NSArray *)accessibilityRowHeaderUIElements;
- (nullable NSArray *)accessibilityColumnHeaderUIElements;

@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityOutline <NSAccessibilityTable>
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityList <NSAccessibilityTable>
@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityRow <NSAccessibilityGroup>
@required
- (NSInteger)accessibilityIndex;

@optional
- (NSInteger)accessibilityDisclosureLevel;
@end

#pragma mark Layout Area (Canvas)
NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityLayoutArea <NSAccessibilityGroup>
@required
- (NSString *)accessibilityLabel;

// All of the layoutItem children
- (nullable NSArray *)accessibilityChildren;

// Selected layoutItem children
- (nullable NSArray *)accessibilitySelectedChildren;

// The focused layoutItem
@property (readonly, strong) id accessibilityFocusedUIElement;

@end

NS_PROTOCOL_REQUIRES_EXPLICIT_IMPLEMENTATION
@protocol NSAccessibilityLayoutItem <NSAccessibilityGroup>

@optional
// For layout items that can be moved
- (void)setAccessibilityFrame:(NSRect)frame;

@end


#pragma mark  Element Loading

API_AVAILABLE(macos(10.13)) @protocol NSAccessibilityElementLoading <NSObject>
@required
/*!
 * @brief Loads the target element with the given load token.
 * @returns An element that will be messaged for other accessibility
 * properties. Assistive technologies may try to set accessibility
 * focus on the returned element.
 */
- (nullable id<NSAccessibilityElement>)accessibilityElementWithToken:(NSAccessibilityLoadingToken)token;

@optional
/*!
 * @brief For text-based elements returned from accessibilityElementWithToken,
 * such as an NSTextView, the range specifies an area of interest. Assistive
 * technologies will try to bring focus to the specified text range.
 * @remark Either do not implement this method or return NSNotFound for the
 * location if there is no range of interest for a given load token.
 */
- (NSRange)accessibilityRangeInTargetElementWithToken:(NSAccessibilityLoadingToken)token;
@end

// The complete accessibility protocol
@protocol NSAccessibility <NSObject>
@required

#pragma mark General
// Return YES if the UIElement should be exposed
@property (getter = isAccessibilityElement) BOOL accessibilityElement API_AVAILABLE(macos(10.10));

// The frame of the UIElement in screen coordinates. See NSAccessibilityFrameInView()
// Invokes when clients request NSAccessibilitySizeAttribute or NSAccessibilityPositionAttribute
@property NSRect accessibilityFrame API_AVAILABLE(macos(10.10));

// Returns YES if the element is focused (generally, accessibilityFocused is equivilent to the
// UIElement with the focus ring or selection)
// Invokes when clients request NSAccessibilityFocusedAttribute
@property (getter = isAccessibilityFocused) BOOL accessibilityFocused API_AVAILABLE(macos(10.10));

// Activation point for the UIElement, in screen coordinates. See NSAccessibilityPointInView()
@property NSPoint accessibilityActivationPoint API_AVAILABLE(macos(10.10));

// UIElement for the containing top level element
// Invokes when clients request NSAccessibilityTopLevelUIElementAttribute
@property (nullable, weak) id accessibilityTopLevelUIElement API_AVAILABLE(macos(10.10));

// URL of the UIElement
// Invokes when clients request NSAccessibilityURLAttribute
@property (nullable, copy) NSURL *accessibilityURL API_AVAILABLE(macos(10.10));

// Value of the UIElement
// Invokes when clients request NSAccessibilityValueAttribute
@property (nullable, strong) id accessibilityValue API_AVAILABLE(macos(10.10));

// Human-readable description of value
// Invokes when clients request NSAccessibilityValueDescriptionAttribute
@property (nullable, copy) NSString *accessibilityValueDescription API_AVAILABLE(macos(10.10));

// Array of child UIElement which are visible
// Invokes when clients request NSAccessibilityVisibleChildrenAttribute
@property (nullable, copy) NSArray *accessibilityVisibleChildren API_AVAILABLE(macos(10.10));

// Sub-role, non - localized (e.g. NSAccessibilityCloseButtonSubrole)
// Invokes when clients request NSAccessibilitySubroleAttribute
@property (nullable, copy) NSAccessibilitySubrole accessibilitySubrole API_AVAILABLE(macos(10.10));

// Visible text on the UIElement
// Invokes when clients request NSAccessibilityTitleAttribute
@property (nullable, copy) NSString *accessibilityTitle API_AVAILABLE(macos(10.10));

// UIElement for the title
// Invokes when clients request NSAccessibilityTitleUIElementAttribute
@property (nullable, weak) id accessibilityTitleUIElement API_AVAILABLE(macos(10.10));

// Next content UIElement
// Invokes when clients request NSAccessibilityNextContentsAttribute
@property (nullable, copy) NSArray *accessibilityNextContents API_AVAILABLE(macos(10.10));

// UIElement orientation
// Invokes when clients request NSAccessibilityOrientationAttribute
@property NSAccessibilityOrientation accessibilityOrientation API_AVAILABLE(macos(10.10));

// UIElement for overflow
// Invokes when clients request NSAccessibilityOverflowButtonAttribute
@property (nullable, strong) id accessibilityOverflowButton API_AVAILABLE(macos(10.10));

// Element containing this UIElement
// Invokes when clients request NSAccessibilityParentAttribute
@property (nullable, weak) id accessibilityParent API_AVAILABLE(macos(10.10));

// Placeholder value of a control such as a text field
// Invokes when clients request NSAccessibilityPlaceholderValueAttribute
@property (nullable, copy) NSString *accessibilityPlaceholderValue API_AVAILABLE(macos(10.10));

// Previous content UIElement.
// Invokes when clients request NSAccessibilityPreviousContentsAttribute
@property (nullable, copy) NSArray *accessibilityPreviousContents API_AVAILABLE(macos(10.10));

// Role, non - localized (e.g. NSAccessibilityRadioButtonRole)
// Invokes when clients request NSAccessibilityRoleAttribute
@property (nullable, copy) NSAccessibilityRole accessibilityRole API_AVAILABLE(macos(10.10));

// Human readable role description (e.g. "radio button");
// Invokes when clients request NSAccessibilityRoleDescriptionAttribute
@property (nullable, copy) NSString *accessibilityRoleDescription API_AVAILABLE(macos(10.10));

// UIElement for search field search button
// Invokes when clients request NSAccessibilitySearchButtonAttribute
@property (nullable, strong) id accessibilitySearchButton API_AVAILABLE(macos(10.10));

// UIElement for search field menu
// Invokes when clients request NSAccessibilitySearchMenuAttribute
@property (nullable, strong) id accessibilitySearchMenu API_AVAILABLE(macos(10.10));

// Returns YES if the UIElement is selected
// Invokes when clients request NSAccessibilitySelectedAttribute
@property (getter = isAccessibilitySelected) BOOL accessibilitySelected API_AVAILABLE(macos(10.10));

// Array of selected child UIElements
// Invokes when clients request NSAccessibilitySelectedChildrenAttribute
@property (nullable, copy) NSArray *accessibilitySelectedChildren API_AVAILABLE(macos(10.10));

// Array of UIElements that this titles
// Invokes when clients request NSAccessibilityServesAsTitleForUIElementsAttribute
@property (nullable, copy) NSArray *accessibilityServesAsTitleForUIElements API_AVAILABLE(macos(10.10));

// Menu being displayed for the UIElement
// Invokes when clients request NSAccessibilityShownMenuAttribute
@property (nullable, strong) id accessibilityShownMenu API_AVAILABLE(macos(10.10));

// The UIElement's minimum value
// Invokes when clients request NSAccessibilityMinValueAttribute
@property (nullable, strong) id accessibilityMinValue API_AVAILABLE(macos(10.10));

// The UIElement's maximum value
// Invokes when clients request NSAccessibilityMaxValueAttribute
@property (nullable, strong) id accessibilityMaxValue API_AVAILABLE(macos(10.10));

// Corresponding UIElements
// Invokes when clients request NSAccessibilityLinkedUIElementsAttribute
@property (nullable, copy) NSArray *accessibilityLinkedUIElements API_AVAILABLE(macos(10.10));

// UIElement for the containing window
// Invokes when clients request NSAccessibilityWindowAttribute
@property (nullable, weak) id accessibilityWindow API_AVAILABLE(macos(10.10));

// Unique identifier for the UIElement
// Invokes when clients request NSAccessibilityIdentifierAttribute
@property (nullable, copy) NSString *accessibilityIdentifier API_AVAILABLE(macos(10.10));

// Instance description (e.g. a tool tip)
// Invokes when clients request NSAccessibilityHelpAttribute
@property (nullable, copy) NSString *accessibilityHelp API_AVAILABLE(macos(10.10));

// Filename of the underlying asset
// Invokes when clients request NSAccessibilityFilenameAttribute
@property (nullable, copy) NSString *accessibilityFilename API_AVAILABLE(macos(10.10));

// Returns YES if the UIElement is expanded
// Invokes when clients request NSAccessibilityExpandedAttribute
@property (getter = isAccessibilityExpanded) BOOL accessibilityExpanded API_AVAILABLE(macos(10.10));

// Returns YES if the UIElement has been edited
// Invokes when clients request NSAccessibilityEditedAttribute
@property (getter = isAccessibilityEdited) BOOL accessibilityEdited API_AVAILABLE(macos(10.10));

// Returns YES if the element responds to user events
// Invokes when clients request NSAccessibilityEnabledAttribute
@property (getter = isAccessibilityEnabled) BOOL accessibilityEnabled API_AVAILABLE(macos(10.10));

// Array of UIElements that represent children of the current UIElement
// Invokes when clients request NSAccessibilityChildrenAttribute
@property (nullable, copy) NSArray *accessibilityChildren API_AVAILABLE(macos(10.10));

// Returns an array of children UIElements ordered for linear navigation.
// This array should match all UIElements found in accessibilityChildren, but in an order that's more suitable for navigation
@property (nullable, copy) NSArray <id<NSAccessibilityElement>> *accessibilityChildrenInNavigationOrder API_AVAILABLE(macos(10.13));

// UIElement for search field clear button
// Invokes when clients request NSAccessibilityClearButtonAttribute
@property (nullable, strong) id accessibilityClearButton API_AVAILABLE(macos(10.10));

// UIElement for cancel button
// Invokes when clients request NSAccessibilityCancelButtonAttribute
@property (nullable, strong) id accessibilityCancelButton API_AVAILABLE(macos(10.10));

// Returns YES if the UIElement contains protected content
// Invokes when clients request NSAccessibilityContainsProtectedContentAttribute
@property (getter = isAccessibilityProtectedContent) BOOL accessibilityProtectedContent API_AVAILABLE(macos(10.10));

// Array of primary content elements
// Invokes when clients request NSAccessibilityContentsAttribute
@property (nullable, copy) NSArray *accessibilityContents API_AVAILABLE(macos(10.10));

// Description of UIElement
// Invokes when clients request NSAccessibilityDescriptionAttribute
@property (nullable, copy) NSString *accessibilityLabel API_AVAILABLE(macos(10.10));

// Returns YES if the element is has been triggered to display alternate UIs
// Invokes when clients request NSAccessibilityAlternateUIVisibleAttribute
@property (getter = isAccessibilityAlternateUIVisible) BOOL accessibilityAlternateUIVisible API_AVAILABLE(macos(10.10));

// Array of elements with which this element shares keyboard focus
// Invokes when clients request NSAccessibilitySharedFocusElementsAttribute
@property (nullable, copy) NSArray *accessibilitySharedFocusElements API_AVAILABLE(macos(10.10));

// Returns YES if the UIElement is required to have content for successful submission of a form
// Invokes when clients request NSAccessibilityRequiredAttribute
@property (getter=isAccessibilityRequired) BOOL accessibilityRequired API_AVAILABLE(macos(10.12));

/*
 * @brief Returns an array of custom rotors. Custom rotors are lists of
 * items of a specific category. For example, a "Headings" rotor would
 * return a list of headings a given document.
 */
@property (copy) NSArray<NSAccessibilityCustomRotor *> *accessibilityCustomRotors API_AVAILABLE(macos(10.13));

#pragma mark Application

// Returns YES if the element is focused (generally, accessibilityFocused is equivalent to the
// to the UIElement with the focus ring or selection)
// Invokes when clients request NSAccessibilityFocusedUIElementAttribute
@property (nullable, strong) id accessibilityApplicationFocusedUIElement API_AVAILABLE(macos(10.10));

// UIElement for the main window.
// Invokes when clients request NSAccessibilityMainWindowAttribute
@property (nullable, strong) id accessibilityMainWindow API_AVAILABLE(macos(10.10));

// Returns YES if the app hidden
// Invokes when clients request NSAccessibilityHiddenAttribute
@property (getter = isAccessibilityHidden) BOOL accessibilityHidden API_AVAILABLE(macos(10.10));

// Returns YES if the app is active
// Invokes when clients request NSAccessibilityFrontmostAttribute
@property (getter = isAccessibilityFrontmost) BOOL accessibilityFrontmost API_AVAILABLE(macos(10.10));

// UIElement for the key window.
// Invokes when clients request NSAccessibilityFocusedWindowAttribute
@property (nullable, strong) id accessibilityFocusedWindow API_AVAILABLE(macos(10.10));

// Array of UIElements for the windows
// Invokes when clients request NSAccessibilityWindowsAttribute
@property (nullable, copy) NSArray *accessibilityWindows API_AVAILABLE(macos(10.10));

// UIElement for the application extras menu bar.
// Invokes when clients request NSAccessibilityExtrasMenuBarAttribute
@property (nullable, weak) id accessibilityExtrasMenuBar API_AVAILABLE(macos(10.10));

// UIElement for the menu bar
// Invokes when clients request NSAccessibilityMenuBarAttribute
@property (nullable, weak) id accessibilityMenuBar API_AVAILABLE(macos(10.10));

#pragma mark Browser
// Array of UIElements for titles
// Invokes when clients request NSAccessibilityColumnTitlesAttribute
@property (nullable, copy) NSArray *accessibilityColumnTitles API_AVAILABLE(macos(10.10));

#pragma mark Grid
// Returns a BOOL indicating whether the children are ordered row major, or column major.
// Invokes when clients request NSAccessibilityOrderedByRowAttribute
@property (getter = isAccessibilityOrderedByRow) BOOL accessibilityOrderedByRow API_AVAILABLE(macos(10.10));

#pragma mark Layout area
// Horizontal units
// Invokes when clients request NSAccessibilityHorizontalUnitsAttribute
@property NSAccessibilityUnits accessibilityHorizontalUnits API_AVAILABLE(macos(10.10));

// Vertical units
// Invokes when clients request NSAccessibilityVerticalUnitsAttribute
@property NSAccessibilityUnits accessibilityVerticalUnits API_AVAILABLE(macos(10.10));

// Human-readable description of the horizontal units
// Invokes when clients request NSAccessibilityHorizontalUnitDescriptionAttribute
@property (nullable, copy) NSString *accessibilityHorizontalUnitDescription API_AVAILABLE(macos(10.10));

// Human-readable description of the vertical units
// Invokes when clients request NSAccessibilityVerticalUnitDescriptionAttribute
@property (nullable, copy) NSString *accessibilityVerticalUnitDescription API_AVAILABLE(macos(10.10));

// Layout point from screen point
// Invokes when clients request NSAccessibilityLayoutPointForScreenPointParameterizedAttribute
- (NSPoint)accessibilityLayoutPointForScreenPoint:(NSPoint)point API_AVAILABLE(macos(10.10));

// Layout size from screen size
// Invokes when clients request NSAccessibilityLayoutSizeForScreenSizeParameterizedAttribute
- (NSSize)accessibilityLayoutSizeForScreenSize:(NSSize)size API_AVAILABLE(macos(10.10));

// Screen point from layout point
// Invokes when clients request NSAccessibilityScreenPointForLayoutPointParameterizedAttribute
- (NSPoint)accessibilityScreenPointForLayoutPoint:(NSPoint)point API_AVAILABLE(macos(10.10));

// Screen size from layout size
// Invokes when clients request NSAccessibilityScreenSizeForLayoutSizeParameterizedAttribute
- (NSSize)accessibilityScreenSizeForLayoutSize:(NSSize)size API_AVAILABLE(macos(10.10));

#pragma mark Layout item
// UIElements for handles
// Invokes when clients request NSAccessibilityHandlesAttribute
@property (nullable, copy) NSArray *accessibilityHandles API_AVAILABLE(macos(10.10));

#pragma mark Level indicator
// Warning value of a level indicator, typically a number
// Invokes when clients request NSAccessibilityWarningValueAttribute
@property (nullable, strong) id accessibilityWarningValue API_AVAILABLE(macos(10.10));

// Critical value of a level indicator, typically a number
// Invokes when clients request NSAccessibilityCriticalValueAttribute
@property (nullable, strong) id accessibilityCriticalValue API_AVAILABLE(macos(10.10));

#pragma mark Outline
// Returns true if sub-rows are visible
// Invokes when clients request NSAccessibilityDisclosingAttribute
@property (getter = isAccessibilityDisclosed) BOOL accessibilityDisclosed API_AVAILABLE(macos(10.10));

// UIElement for disclosing row
// Invokes when clients request NSAccessibilityDisclosedByRowAttribute
@property (nullable, weak) id accessibilityDisclosedByRow API_AVAILABLE(macos(10.10));

// Array of UIElements for disclosed rows of the current ruw (ie, sub-rows)
// Invokes when clients request NSAccessibilityDisclosedRowsAttribute
@property (nullable, strong) id accessibilityDisclosedRows API_AVAILABLE(macos(10.10));

// Indentation level
// Invokes when clients request NSAccessibilityDisclosureLevelAttribute
@property NSInteger accessibilityDisclosureLevel API_AVAILABLE(macos(10.10));

#pragma mark Ruler
// Invokes when clients request NSAccessibilityMarkerUIElementsAttribute
@property (nullable, copy) NSArray *accessibilityMarkerUIElements API_AVAILABLE(macos(10.10));

// Invokes when clients request NSAccessibilityMarkerValuesAttribute
@property (nullable, strong) id accessibilityMarkerValues API_AVAILABLE(macos(10.10));

// Invokes when clients request NSAccessibilityMarkerGroupUIElementAttribute
@property (nullable, strong) id accessibilityMarkerGroupUIElement API_AVAILABLE(macos(10.10));

// Ruler units
// Invokes when clients request NSAccessibilityUnitsAttribute
@property NSAccessibilityUnits accessibilityUnits API_AVAILABLE(macos(10.10));

// Human-readable description of the ruler units
// Invokes when clients request NSAccessibilityUnitDescriptionAttribute
@property (nullable, copy) NSString *accessibilityUnitDescription API_AVAILABLE(macos(10.10));

// Marker type
// Invokes when clients request NSAccessibilityMarkerTypeAttribute
@property NSAccessibilityRulerMarkerType accessibilityRulerMarkerType API_AVAILABLE(macos(10.10));

// Human-readable description of the marker type
// Invokes when clients request NSAccessibilityMarkerTypeDescriptionAttribute
@property (nullable, copy) NSString *accessibilityMarkerTypeDescription API_AVAILABLE(macos(10.10));

#pragma mark Scrollable region
// UIElement for the horizontal scroller
// Invokes when clients request NSAccessibilityHorizontalScrollBarAttribute
@property (nullable, strong) id accessibilityHorizontalScrollBar API_AVAILABLE(macos(10.10));

// UIElement for the vertical scroller
// Invokes when clients request NSAccessibilityVerticalScrollBarAttribute
@property (nullable, strong) id accessibilityVerticalScrollBar API_AVAILABLE(macos(10.10));

#pragma mark Slider
// Array of allowed values
// Invokes when clients request NSAccessibilityAllowedValuesAttribute
@property (nullable, copy) NSArray<NSNumber *> *accessibilityAllowedValues API_AVAILABLE(macos(10.10));

// Array of label UIElements
// Invokes when clients request NSAccessibilityLabelUIElementsAttribute
@property (nullable, copy) NSArray *accessibilityLabelUIElements API_AVAILABLE(macos(10.10));

// Value of a label UIElement
// Invokes when clients request NSAccessibilityLabelValueAttribute
@property float accessibilityLabelValue API_AVAILABLE(macos(10.10));

#pragma mark Splitter
// UIElements for splitters
// Invokes when clients request NSAccessibilitySplittersAttribute
@property (nullable, copy) NSArray *accessibilitySplitters API_AVAILABLE(macos(10.10));

#pragma mark Stepper
// UIElement for the decrement button
// Invokes when clients request NSAccessibilityDecrementButtonAttribute
@property (nullable, strong) id accessibilityDecrementButton API_AVAILABLE(macos(10.10));

// UIElement for the increment button
// Invokes when clients request NSAccessibilityIncrementButtonAttribute
@property (nullable, strong) id accessibilityIncrementButton API_AVAILABLE(macos(10.10));

#pragma mark Tab view
// UIElements for tabs
// Invokes when clients request NSAccessibilityTabsAttribute
@property (nullable, copy) NSArray *accessibilityTabs API_AVAILABLE(macos(10.10));

#pragma mark Table/Outline
// UIElement for header
// Invokes when clients request NSAccessibilityHeaderAttribute
@property (nullable, strong) id accessibilityHeader API_AVAILABLE(macos(10.10));

// Number of columns
// Invokes when clients request NSAccessibilityColumnCountAttribute
@property NSInteger accessibilityColumnCount API_AVAILABLE(macos(10.10));

// Number of rows
// Invokes when clients request NSAccessibilityRowCountAttribute
@property NSInteger accessibilityRowCount API_AVAILABLE(macos(10.10));

// Index of the current UIElement (row index for a row, column index for a column)
// Invokes when clients request NSAccessibilityIndexAttribute
@property NSInteger accessibilityIndex API_AVAILABLE(macos(10.10));

// Array of UIElements for columns
// Invokes when clients request NSAccessibilityColumnsAttribute
@property (nullable, copy) NSArray *accessibilityColumns API_AVAILABLE(macos(10.10));

// Array of UIElements for rows
// Invokes when clients request NSAccessibilityRowsAttribute
@property (nullable, copy) NSArray *accessibilityRows API_AVAILABLE(macos(10.10));

// Array of UIElements for visible rows
// Invokes when clients request NSAccessibilityVisibleRowsAttribute
@property (nullable, copy) NSArray *accessibilityVisibleRows API_AVAILABLE(macos(10.10));

// Array of UIElements for selected rows
// Invokes when clients request NSAccessibilitySelectedRowsAttribute
@property (nullable, copy) NSArray *accessibilitySelectedRows API_AVAILABLE(macos(10.10));

// Array of UIElements for visible columns
// Invokes when clients request NSAccessibilityVisibleColumnsAttribute
@property (nullable, copy) NSArray *accessibilityVisibleColumns API_AVAILABLE(macos(10.10));

// Array of UIElements for selected columns
// Invokes when clients request NSAccessibilitySelectedColumnsAttribute
@property (nullable, copy) NSArray *accessibilitySelectedColumns API_AVAILABLE(macos(10.10));

// Sort direction
// Invokes when clients request NSAccessibilitySortDirectionAttribute
@property NSAccessibilitySortDirection accessibilitySortDirection API_AVAILABLE(macos(10.10));

#pragma mark Table (cell based)
// Array of UIElements for row headers
// Invokes when clients request NSAccessibilityRowHeaderUIElementsAttribute
@property (nullable, copy) NSArray *accessibilityRowHeaderUIElements API_AVAILABLE(macos(10.10));

// Array of UIElements for selected cells
// Invokes when clients request NSAccessibilitySelectedCellsAttribute
@property (nullable, copy) NSArray *accessibilitySelectedCells API_AVAILABLE(macos(10.10));

// Array of UIElements for visible cells
// Invokes when clients request NSAccessibilityVisibleCellsAttribute
@property (nullable, copy) NSArray *accessibilityVisibleCells API_AVAILABLE(macos(10.10));

// Array of UIElements for column headers
// Invokes when clients request NSAccessibilityColumnHeaderUIElementsAttribute
@property (nullable, copy) NSArray *accessibilityColumnHeaderUIElements API_AVAILABLE(macos(10.10));

// UIElement for cell at specified row and column
// Invokes when clients request NSAccessibilityCellForColumnAndRowParameterizedAttribute
- (nullable id)accessibilityCellForColumn:(NSInteger)column row:(NSInteger)row API_AVAILABLE(macos(10.10));

#pragma mark Table cell
// Cell location and row span
// Invokes when clients request NSAccessibilityRowIndexRangeAttribute
@property NSRange accessibilityRowIndexRange API_AVAILABLE(macos(10.10));

// Cell location and column span
// Invokes when clients request NSAccessibilityColumnIndexRangeAttribute
@property NSRange accessibilityColumnIndexRange API_AVAILABLE(macos(10.10));

#pragma mark Text
// Line number containing caret
// Invokes when clients request NSAccessibilityInsertionPointLineNumberAttribute
@property NSInteger accessibilityInsertionPointLineNumber API_AVAILABLE(macos(10.10));

// Part of shared text in this view
// Invokes when clients request NSAccessibilitySharedCharacterRangeAttribute
@property NSRange accessibilitySharedCharacterRange API_AVAILABLE(macos(10.10));

// Text views sharing text
// Invokes when clients request NSAccessibilitySharedTextUIElementsAttribute
@property (nullable, copy) NSArray *accessibilitySharedTextUIElements API_AVAILABLE(macos(10.10));

// Range of visible text
// Invokes when clients request NSAccessibilityVisibleCharacterRangeAttribute
@property NSRange accessibilityVisibleCharacterRange API_AVAILABLE(macos(10.10));

// Number of characters
// Invokes when clients request NSAccessibilityNumberOfCharactersAttribute
@property NSInteger accessibilityNumberOfCharacters API_AVAILABLE(macos(10.10));

// String of selected text
// Invokes when clients request NSAccessibilitySelectedTextAttribute
@property (nullable, copy) NSString *accessibilitySelectedText API_AVAILABLE(macos(10.10));

// Range of selected text
// Invokes when clients request NSAccessibilitySelectedTextRangeAttribute
@property NSRange accessibilitySelectedTextRange API_AVAILABLE(macos(10.10));

// Array of NSValue (rangeValue) ranges of selected text
// Invokes when clients request NSAccessibilitySelectedTextRangesAttribute
@property (nullable, copy) NSArray<NSValue *> *accessibilitySelectedTextRanges API_AVAILABLE(macos(10.10));

// Attributed string for a provided range.
// does _not_ use attributes from Appkit/AttributedString.h
// Invokes when clients request NSAccessibilityAttributedStringForRangeParameterizedAttribute
- (nullable NSAttributedString *)accessibilityAttributedStringForRange:(NSRange)range API_AVAILABLE(macos(10.10));

// Range for the provided line number
// Invokes when clients request NSAccessibilityRangeForLineParameterizedAttribute
- (NSRange)accessibilityRangeForLine:(NSInteger)line API_AVAILABLE(macos(10.10));

// String for the provided range
// Invokes when clients request NSAccessibilityStringForRangeParameterizedAttribute
- (nullable NSString *)accessibilityStringForRange:(NSRange)range API_AVAILABLE(macos(10.10));

// Composed character range for the character at the provided point
// Invokes when clients request NSAccessibilityRangeForPositionParameterizedAttribute
- (NSRange)accessibilityRangeForPosition:(NSPoint)point API_AVAILABLE(macos(10.10));

// Composed character range for the provided index
// Invokes when clients request NSAccessibilityRangeForIndexParameterizedAttribute
- (NSRange)accessibilityRangeForIndex:(NSInteger)index API_AVAILABLE(macos(10.10));

// Frame of text in the provided range (in screen coordinates.) See NSAccessibilityFrameInView()
// Invokes when clients request NSAccessibilityBoundsForRangeParameterizedAttribute
- (NSRect)accessibilityFrameForRange:(NSRange)range API_AVAILABLE(macos(10.10));

// RTF data for text in the provided range
// Invokes when clients request NSAccessibilityRTFForRangeParameterizedAttribute
- (nullable NSData *)accessibilityRTFForRange:(NSRange)range API_AVAILABLE(macos(10.10));

// Extent of style run at the provided index
// Invokes when clients request NSAccessibilityStyleRangeForIndexParameterizedAttribute
- (NSRange)accessibilityStyleRangeForIndex:(NSInteger)index API_AVAILABLE(macos(10.10));

// Line number for character index
// Invokes when clients request NSAccessibilityLineForIndexParameterizedAttribute
- (NSInteger)accessibilityLineForIndex:(NSInteger)index API_AVAILABLE(macos(10.10));

#pragma mark Window
// UIElement for toolbar box (or nil)
// Invokes when clients request NSAccessibilityToolbarButtonAttribute
@property (nullable, strong) id accessibilityToolbarButton API_AVAILABLE(macos(10.10));

// Is the window modal
// Invokes when clients request NSAccessibilityModalAttribute
@property (getter = isAccessibilityModal) BOOL accessibilityModal API_AVAILABLE(macos(10.10));

// UIElement for title's icon (or nil)
// Invokes when clients request NSAccessibilityProxyAttribute
@property (nullable, strong) id accessibilityProxy API_AVAILABLE(macos(10.10));

// Returns YES if this is it the main window
// Invokes when clients request NSAccessibilityMainAttribute
@property (getter = isAccessibilityMain) BOOL accessibilityMain API_AVAILABLE(macos(10.10));

// UIElement for full screen button
// Invokes when clients request NSAccessibilityFullScreenButtonAttribute
@property (nullable, strong) id accessibilityFullScreenButton API_AVAILABLE(macos(10.10));

// UIElement for grow box
// Invokes when clients request NSAccessibilityGrowAreaAttribute
@property (nullable, strong) id accessibilityGrowArea API_AVAILABLE(macos(10.10));

// URL for open document
// Invokes when clients request NSAccessibilityDocumentAttribute
@property (nullable, copy) NSString *accessibilityDocument API_AVAILABLE(macos(10.10));

// UIElement for default button
// Invokes when clients request NSAccessibilityDefaultButtonAttribute
@property (nullable, strong) id accessibilityDefaultButton API_AVAILABLE(macos(10.10));

// UIElement for close button
// Invokes when clients request NSAccessibilityCloseButtonAttribute
@property (nullable, strong) id accessibilityCloseButton API_AVAILABLE(macos(10.10));

// UIElement for zoom button
// Invokes when clients request NSAccessibilityZoomButtonAttribute
@property (nullable, strong) id accessibilityZoomButton API_AVAILABLE(macos(10.10));

// UIElement for minimize button
// Invokes when clients request NSAccessibilityMinimizeButtonAttribute
@property (nullable, strong) id accessibilityMinimizeButton API_AVAILABLE(macos(10.10));

// Returns YES if the window minimized
// Invokes when clients request NSAccessibilityMinimizedAttribute
@property (getter = isAccessibilityMinimized) BOOL accessibilityMinimized API_AVAILABLE(macos(10.10));

#pragma mark Actions

@property (nullable, copy) NSArray<NSAccessibilityCustomAction *> *accessibilityCustomActions API_AVAILABLE(macos(10.13));

// Invokes when clients perform NSAccessibilityCancelAction
- (BOOL)accessibilityPerformCancel API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityConfirmAction
- (BOOL)accessibilityPerformConfirm API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityDecrementAction
- (BOOL)accessibilityPerformDecrement API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityDeleteAction
- (BOOL)accessibilityPerformDelete API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityIncrementAction
- (BOOL)accessibilityPerformIncrement API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityPickAction
- (BOOL)accessibilityPerformPick API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityPressAction
- (BOOL)accessibilityPerformPress API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityRaiseAction
- (BOOL)accessibilityPerformRaise API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityShowAlternateUIAction
- (BOOL)accessibilityPerformShowAlternateUI API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityShowDefaultUIAction
- (BOOL)accessibilityPerformShowDefaultUI API_AVAILABLE(macos(10.10));

// Invokes when clients perform NSAccessibilityShowMenuAction
- (BOOL)accessibilityPerformShowMenu API_AVAILABLE(macos(10.10));

#pragma mark Allowed Selectors
// Returns YES if accessibility clients should be able to invoke the provided selector
- (BOOL)isAccessibilitySelectorAllowed:(SEL)selector API_AVAILABLE(macos(10.10));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
