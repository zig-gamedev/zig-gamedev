#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextElement.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextElement.h
//  Text Kit
//
//  Copyright (c) 2018-2021, Apple Inc. All rights reserved.
//


#import <Foundation/NSObject.h>

@class NSTextRange;
@class NSTextContentManager;

NS_ASSUME_NONNULL_BEGIN

// NSTextElement is an abstract base class for representing the smallest text layout unit typically paragraphs, tables, or attachments. A text element is associated with an NSTextContentManager.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextElement : NSObject
#pragma mark Initialization
- (instancetype)initWithTextContentManager:(nullable NSTextContentManager *)textContentManager NS_DESIGNATED_INITIALIZER;

#pragma mark Properties
// Returns the owner NSTextContentManager. A text element can migrate between text content managers.
@property (nullable, weak) NSTextContentManager *textContentManager;

// Represents the range of the element inside the document.
@property (nullable, strong) NSTextRange *elementRange;
@end

#pragma mark NSTextParagraph
// NSTextParagraph represents a single paragraph backed by NSAttributedString as the contents.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextParagraph : NSTextElement
#pragma mark Initialization
- (instancetype)initWithAttributedString:(nullable NSAttributedString *)attributedString NS_DESIGNATED_INITIALIZER;

#pragma mark Contents
@property (strong, readonly) NSAttributedString *attributedString;

// The paragraph ranges are derived from elementRange and attributedString.
@property (nullable, strong, readonly) NSTextRange *paragraphContentRange;
@property (nullable, strong, readonly) NSTextRange *paragraphSeparatorRange;
@end
NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextElement.h>
#endif
