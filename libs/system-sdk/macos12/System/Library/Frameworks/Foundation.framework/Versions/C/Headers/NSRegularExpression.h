/*	NSRegularExpression.h
	Copyright (c) 2009-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>
#import <Foundation/NSTextCheckingResult.h>

@class NSArray<ObjectType>;

NS_ASSUME_NONNULL_BEGIN

/* NSRegularExpression is a class used to represent and apply regular expressions.  An instance of this class is an immutable representation of a compiled regular expression pattern and various option flags.
*/

typedef NS_OPTIONS(NSUInteger, NSRegularExpressionOptions) {
   NSRegularExpressionCaseInsensitive             = 1 << 0,     /* Match letters in the pattern independent of case. */
   NSRegularExpressionAllowCommentsAndWhitespace  = 1 << 1,     /* Ignore whitespace and #-prefixed comments in the pattern. */
   NSRegularExpressionIgnoreMetacharacters        = 1 << 2,     /* Treat the entire pattern as a literal string. */
   NSRegularExpressionDotMatchesLineSeparators    = 1 << 3,     /* Allow . to match any character, including line separators. */
   NSRegularExpressionAnchorsMatchLines           = 1 << 4,     /* Allow ^ and $ to match the start and end of lines. */
   NSRegularExpressionUseUnixLineSeparators       = 1 << 5,     /* Treat only \n as a line separator (otherwise, all standard line separators are used). */
   NSRegularExpressionUseUnicodeWordBoundaries    = 1 << 6      /* Use Unicode TR#29 to specify word boundaries (otherwise, traditional regular expression word boundaries are used). */
};

API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSRegularExpression : NSObject <NSCopying, NSSecureCoding> {
    @protected   // all instance variables are private
    NSString *_pattern;
    NSUInteger _options;
    void *_internal;
    id _reserved1;
    int32_t _checkout;
    int32_t _reserved2;
}

/* An instance of NSRegularExpression is created from a regular expression pattern and a set of options.  If the pattern is invalid, nil will be returned and an NSError will be returned by reference.  The pattern syntax currently supported is that specified by ICU.
*/
+ (nullable NSRegularExpression *)regularExpressionWithPattern:(NSString *)pattern options:(NSRegularExpressionOptions)options error:(NSError **)error;
- (nullable instancetype)initWithPattern:(NSString *)pattern options:(NSRegularExpressionOptions)options error:(NSError **)error NS_DESIGNATED_INITIALIZER;

@property (readonly, copy) NSString *pattern;
@property (readonly) NSRegularExpressionOptions options;
@property (readonly) NSUInteger numberOfCaptureGroups;

/* This class method will produce a string by adding backslash escapes as necessary to the given string, to escape any characters that would otherwise be treated as pattern metacharacters.
*/
+ (NSString *)escapedPatternForString:(NSString *)string;

@end


typedef NS_OPTIONS(NSUInteger, NSMatchingOptions) {
   NSMatchingReportProgress         = 1 << 0,       /* Call the block periodically during long-running match operations. */
   NSMatchingReportCompletion       = 1 << 1,       /* Call the block once after the completion of any matching. */
   NSMatchingAnchored               = 1 << 2,       /* Limit matches to those at the start of the search range. */
   NSMatchingWithTransparentBounds  = 1 << 3,       /* Allow matching to look beyond the bounds of the search range. */
   NSMatchingWithoutAnchoringBounds = 1 << 4        /* Prevent ^ and $ from automatically matching the beginning and end of the search range. */
};

typedef NS_OPTIONS(NSUInteger, NSMatchingFlags) {
   NSMatchingProgress               = 1 << 0,       /* Set when the block is called to report progress during a long-running match operation. */
   NSMatchingCompleted              = 1 << 1,       /* Set when the block is called after completion of any matching. */
   NSMatchingHitEnd                 = 1 << 2,       /* Set when the current match operation reached the end of the search range. */
   NSMatchingRequiredEnd            = 1 << 3,       /* Set when the current match depended on the location of the end of the search range. */
   NSMatchingInternalError          = 1 << 4        /* Set when matching failed due to an internal error. */
};

@interface NSRegularExpression (NSMatching)

/* The fundamental matching method on NSRegularExpression is a block iterator.  There are several additional convenience methods, for returning all matches at once, the number of matches, the first match, or the range of the first match.  Each match is specified by an instance of NSTextCheckingResult (of type NSTextCheckingTypeRegularExpression) in which the overall match range is given by the range property (equivalent to rangeAtIndex:0) and any capture group ranges are given by rangeAtIndex: for indexes from 1 to numberOfCaptureGroups.  {NSNotFound, 0} is used if a particular capture group does not participate in the match.
*/

- (void)enumerateMatchesInString:(NSString *)string options:(NSMatchingOptions)options range:(NSRange)range usingBlock:(void (NS_NOESCAPE ^)(NSTextCheckingResult * _Nullable result, NSMatchingFlags flags, BOOL *stop))block;

- (NSArray<NSTextCheckingResult *> *)matchesInString:(NSString *)string options:(NSMatchingOptions)options range:(NSRange)range;
- (NSUInteger)numberOfMatchesInString:(NSString *)string options:(NSMatchingOptions)options range:(NSRange)range;
- (nullable NSTextCheckingResult *)firstMatchInString:(NSString *)string options:(NSMatchingOptions)options range:(NSRange)range;
- (NSRange)rangeOfFirstMatchInString:(NSString *)string options:(NSMatchingOptions)options range:(NSRange)range;

/* By default, the block iterator method calls the block precisely once for each match, with a non-nil result and appropriate flags.  The client may then stop the operation by setting the contents of stop to YES.  If the NSMatchingReportProgress option is specified, the block will also be called periodically during long-running match operations, with nil result and NSMatchingProgress set in the flags, at which point the client may again stop the operation by setting the contents of stop to YES.  If the NSMatchingReportCompletion option is specified, the block will be called once after matching is complete, with nil result and NSMatchingCompleted set in the flags, plus any additional relevant flags from among NSMatchingHitEnd, NSMatchingRequiredEnd, or NSMatchingInternalError.  NSMatchingReportProgress and NSMatchingReportCompletion have no effect for methods other than the block iterator.

NSMatchingHitEnd is set in the flags passed to the block if the current match operation reached the end of the search range.  NSMatchingRequiredEnd is set in the flags passed to the block if the current match depended on the location of the end of the search range.  NSMatchingInternalError is set in the flags passed to the block if matching failed due to an internal error (such as an expression requiring exponential memory allocations) without examining the entire search range.

NSMatchingAnchored, NSMatchingWithTransparentBounds, and NSMatchingWithoutAnchoringBounds can apply to any match or replace method.  If NSMatchingAnchored is specified, matches are limited to those at the start of the search range.  If NSMatchingWithTransparentBounds is specified, matching may examine parts of the string beyond the bounds of the search range, for purposes such as word boundary detection, lookahead, etc.  If NSMatchingWithoutAnchoringBounds is specified, ^ and $ will not automatically match the beginning and end of the search range (but will still match the beginning and end of the entire string).  NSMatchingWithTransparentBounds and NSMatchingWithoutAnchoringBounds have no effect if the search range covers the entire string.

NSRegularExpression is designed to be immutable and threadsafe, so that a single instance can be used in matching operations on multiple threads at once.  However, the string on which it is operating should not be mutated during the course of a matching operation (whether from another thread or from within the block used in the iteration).
*/

@end


@interface NSRegularExpression (NSReplacement)

/* NSRegularExpression also provides find-and-replace methods for both immutable and mutable strings.  The replacement is treated as a template, with $0 being replaced by the contents of the matched range, $1 by the contents of the first capture group, and so on.  Additional digits beyond the maximum required to represent the number of capture groups will be treated as ordinary characters, as will a $ not followed by digits.  Backslash will escape both $ and itself.
*/
- (NSString *)stringByReplacingMatchesInString:(NSString *)string options:(NSMatchingOptions)options range:(NSRange)range withTemplate:(NSString *)templ;
- (NSUInteger)replaceMatchesInString:(NSMutableString *)string options:(NSMatchingOptions)options range:(NSRange)range withTemplate:(NSString *)templ;

/* For clients implementing their own replace functionality, this is a method to perform the template substitution for a single result, given the string from which the result was matched, an offset to be added to the location of the result in the string (for example, in case modifications to the string moved the result since it was matched), and a replacement template.
*/
- (NSString *)replacementStringForResult:(NSTextCheckingResult *)result inString:(NSString *)string offset:(NSInteger)offset template:(NSString *)templ;

/* This class method will produce a string by adding backslash escapes as necessary to the given string, to escape any characters that would otherwise be treated as template metacharacters. 
*/
+ (NSString *)escapedTemplateForString:(NSString *)string;

@end

API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSDataDetector : NSRegularExpression {
    @protected   // all instance variables are private
    NSTextCheckingTypes _types;
}

/* NSDataDetector is a specialized subclass of NSRegularExpression.  Instead of finding matches to regular expression patterns, it matches items identified by Data Detectors, such as dates, addresses, and URLs.  The checkingTypes argument should contain one or more of the types NSTextCheckingTypeDate, NSTextCheckingTypeAddress, NSTextCheckingTypeLink, NSTextCheckingTypePhoneNumber, and NSTextCheckingTypeTransitInformation.  The NSTextCheckingResult instances returned will be of the appropriate types from that list.
*/
+ (nullable NSDataDetector *)dataDetectorWithTypes:(NSTextCheckingTypes)checkingTypes error:(NSError **)error;
- (nullable instancetype)initWithTypes:(NSTextCheckingTypes)checkingTypes error:(NSError **)error NS_DESIGNATED_INITIALIZER;

@property (readonly) NSTextCheckingTypes checkingTypes;

@end

NS_ASSUME_NONNULL_END
