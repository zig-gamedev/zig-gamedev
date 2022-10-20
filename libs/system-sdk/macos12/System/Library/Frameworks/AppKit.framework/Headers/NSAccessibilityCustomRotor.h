/*
	NSAccessibilityCustomRotor.h
	Application Kit
	Copyright (c) 2016-2021, Apple Inc.
	All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <AppKit/NSAccessibilityProtocols.h>
#import <AppKit/AppKitDefines.h>

/*!
 * @discussion NSAccessibilityCustomRotor:
 *
 * Assistive technologies, such as VoiceOver, provide interfaces to quickly
 * search applications for content of a given type. For example, in a web
 * browser, a list of navigational links or buttons can quickly be explored
 * using VoiceOver's content menus.
 *
 * The classes defined here provide a way for applications to vend their own
 * content menus. For example, Pages can create a "Headings" custom rotor
 * that allows assistive technologies to search the Pages document for all
 * headings.
 */

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSAccessibilityCustomRotor;
@class NSAccessibilityCustomRotorItemResult;
@class NSAccessibilityCustomRotorSearchParameters;

@protocol NSAccessibilityCustomRotorItemSearchDelegate;
@protocol NSAccessibilityCustomRotorItemLoadDelegate;


/*!
 * @brief Direction to search for an NSAccessibilityCustomRotorItemResult.
 */
typedef NS_ENUM(NSInteger, NSAccessibilityCustomRotorSearchDirection) {
    NSAccessibilityCustomRotorSearchDirectionPrevious,
    NSAccessibilityCustomRotorSearchDirectionNext,
} API_AVAILABLE(macos(10.13));

/*!
 * @brief Use NSAccessibilityCustomRotorType when providing results for the
 * following types. This allows assistive technologies to assign keyboard
 * commands and gestures for these common search types.
 */
typedef NS_ENUM(NSInteger, NSAccessibilityCustomRotorType) {
    // Default if using a custom labeled rotor
    NSAccessibilityCustomRotorTypeCustom = 0,
    
    // This type is used for searching the current
    // element for any type of item.
    NSAccessibilityCustomRotorTypeAny = 1,
    
    // Common types of rotors
    NSAccessibilityCustomRotorTypeAnnotation,
    NSAccessibilityCustomRotorTypeBoldText,
    NSAccessibilityCustomRotorTypeHeading,
    NSAccessibilityCustomRotorTypeHeadingLevel1,
    NSAccessibilityCustomRotorTypeHeadingLevel2,
    NSAccessibilityCustomRotorTypeHeadingLevel3,
    NSAccessibilityCustomRotorTypeHeadingLevel4,
    NSAccessibilityCustomRotorTypeHeadingLevel5,
    NSAccessibilityCustomRotorTypeHeadingLevel6,
    NSAccessibilityCustomRotorTypeImage,
    NSAccessibilityCustomRotorTypeItalicText,
    NSAccessibilityCustomRotorTypeLandmark,
    NSAccessibilityCustomRotorTypeLink,
    NSAccessibilityCustomRotorTypeList,
    NSAccessibilityCustomRotorTypeMisspelledWord,
    NSAccessibilityCustomRotorTypeTable,
    NSAccessibilityCustomRotorTypeTextField,
    NSAccessibilityCustomRotorTypeUnderlinedText,
    NSAccessibilityCustomRotorTypeVisitedLink,
    NSAccessibilityCustomRotorTypeAudiograph,
} API_AVAILABLE(macos(10.13));


/*!
 * @brief NSAccessibilityCustomRotors allow assistive technologies, like
 * VoiceOver, to search applications for content related to the given label.
 */
API_AVAILABLE(macos(10.13)) @interface NSAccessibilityCustomRotor : NSObject

/*!
 * @brief Convenience initializer that uses
 * NSAccessibilityCustomRotorTypeCustom as the default rotor type. Use this
 * initializer for custom rotors that are not one of the common types.
 */
- (instancetype)initWithLabel:(NSString *)label
           itemSearchDelegate:(id<NSAccessibilityCustomRotorItemSearchDelegate>)itemSearchDelegate;

/*!
 * @brief Convenience initializer for custom rotors that use a common type
 * such as links, headings, etc. A default label will be provided.
 */
- (instancetype)initWithRotorType:(NSAccessibilityCustomRotorType)rotorType
               itemSearchDelegate:(id<NSAccessibilityCustomRotorItemSearchDelegate>)itemSearchDelegate;

/*!
 * @brief The rotor type to provide results for.
 * @remark The default type is NSAccessibilityCustomRotorTypeCustom, unless
 * the rotor type was specified in the initializer.
 */
@property NSAccessibilityCustomRotorType type;

/*!
 * @brief The localized label assistive technologies will use to describe
 * the custom rotor.
 * @remark The label is only used when the rotor type is
 * NSAccessibilityCustomRotorTypeCustom since a default is provided
 * for all other types.
 */
@property (copy) NSString *label;

/*!
 * @brief The itemSearchDelegate will be asked to find the next item result
 * after performing a search with the given search parameters.
 */
@property (weak) id<NSAccessibilityCustomRotorItemSearchDelegate> itemSearchDelegate;

/*!
 * @brief Provide an item load delegate if the rotor vends item results that
 * do not have a backing UI element yet. The loader will be asked to load an
 * element via the accessibilityElementWithToken protocol method when the
 * item result is selected by an assistive client. Applications can use the
 * item result's token to determine which item to return.
 */
@property (nullable, weak) id<NSAccessibilityElementLoading> itemLoadingDelegate;

@end


/*!
 * @brief NSAccessibilityCustomRotorSearchParameters is a container for 
 * search parameters. It should be examined to determine the next matching
 * NSAccessibilityCustomRotorItemResult.
 */
API_AVAILABLE(macos(10.13)) @interface NSAccessibilityCustomRotorSearchParameters : NSObject

/*!
 * @brief The currentItem determines where the search will start from. If 
 * it is nil, the search should begin from, and include, the first or last
 * item, depending on which search direction is used (e.g. search direction
 * next will return the first item and previous will return the last item).
 */
@property (nullable, strong) NSAccessibilityCustomRotorItemResult *currentItem;

/*!
 * @brief Either NSAccessibilityCustomRotorSearchDirectionPrevious or
 * NSAccessibilityCustomRotorSearchDirectionNext.
 */
@property NSAccessibilityCustomRotorSearchDirection searchDirection;

/*!
 * @brief A string of text to filter the results against. This is used to get
 * type-ahead results. For example, given a list of primary colors and filter
 * text "Re", color item "Red" would be returned as a result.
 */
@property (copy) NSString *filterString;

@end


/*!
 * @brief NSAccessibilityCustomRotorItemResults are the objects returned
 * to assistive technologies that match a search parameter criteria.
 */
API_AVAILABLE(macos(10.13)) @interface NSAccessibilityCustomRotorItemResult : NSObject

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/*!
 * @brief Creates an item result with a given target element. Assistive
 * technologies may try to set accessibility focus on the element.
 */
- (instancetype)initWithTargetElement:(id<NSAccessibilityElement>)targetElement NS_DESIGNATED_INITIALIZER;

/*!
 * @brief Creates an item result with a given item load token and custom label.
 * Use this initializer if the application has not yet loaded the element
 * backing the item result.
 */
- (instancetype)initWithItemLoadingToken:(NSAccessibilityLoadingToken)itemLoadingToken
                             customLabel:(NSString *)customLabel NS_DESIGNATED_INITIALIZER;

/*!
 * @brief A target element references an element that will be messaged for
 * other accessibility properties. If it is not nil, assistive technologies
 * may try to set accessibility focus on it.
 */
@property (nullable, weak, readonly) id<NSAccessibilityElement> targetElement;

/*!
 * @brief Provide an item load token if the application has not yet
 * loaded the element backing the item result. Application can use the token
 * to determine which item to return.
 */
@property (nullable, strong, readonly) NSAccessibilityLoadingToken itemLoadingToken;

/*!
 * @brief For text-based elements such as an NSTextView, this is an NSRange
 * that specifies the area of interest. If the target range has NSNotFound
 * for the location, the search should begin from the first or last character
 * of the text element, depending on the search direction.
 */
@property NSRange targetRange;

/*!
 * @brief A localized label that can be used instead of the default item 
 * label to describe the item result.
 * @remark Required if using the loader-based initializer. Optional otherwise.
 */
@property (nullable, copy) NSString *customLabel;

@end


API_AVAILABLE(macos(10.13)) @protocol NSAccessibilityCustomRotorItemSearchDelegate <NSObject>

@required
/*!
 * @brief Returns the found NSAccessibilityCustomRotorItemResult after
 * performing a search with the given search parameters.
 */
- (nullable NSAccessibilityCustomRotorItemResult *)rotor:(NSAccessibilityCustomRotor *)rotor
                               resultForSearchParameters:(NSAccessibilityCustomRotorSearchParameters *)searchParameters;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
