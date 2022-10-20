/*
        NSTextCheckingClient.h
        Application Kit
        Copyright (c) 2018-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSAttributedString.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSTextInputClient.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSAttributedString, NSView, NSCandidateListTouchBarItem;

typedef NS_ENUM(NSInteger, NSTextInputTraitType) {
   NSTextInputTraitTypeDefault,
   NSTextInputTraitTypeNo,
   NSTextInputTraitTypeYes,
};

@protocol NSTextInputTraits
@optional
@property NSTextInputTraitType autocorrectionType;
@property NSTextInputTraitType spellCheckingType;
@property NSTextInputTraitType grammarCheckingType;
@property NSTextInputTraitType smartQuotesType;
@property NSTextInputTraitType smartDashesType;
@property NSTextInputTraitType smartInsertDeleteType;
@property NSTextInputTraitType textReplacementType;
@property NSTextInputTraitType dataDetectionType;
@property NSTextInputTraitType linkDetectionType;
@property NSTextInputTraitType textCompletionType;
@end

@protocol NSTextCheckingClient <NSTextInputClient, NSTextInputTraits>
@required

/* These methods suppose that ranges of text in the document may have attached to them certain annotations relevant for text checking, represented by dictionaries with various keys, such as NSSpellingStateAttributeName for ranges of text marked as misspelled. They allow an NSTextCheckingController instance to set and retrieve these annotations, and to perform other actions required for text checking.  The keys and values in these annotation dictionaries will always be strings.
*/

/* In all of these methods, the standard range adjustment policy is as follows:  if the specified range lies only partially within the bounds of the document, the receiver is responsible for adjusting the range so as to limit it to the bounds of the document. If the specified range is {NSNotFound, 0}, then the receiver should replace it with the entire range of the document. Otherwise, if none of the range lies within the bounds of the document, then these methods should have no effect, and return nil where appropriate. The beginning and end of the document are not considered as lying outside of the bounds of the document, and zero-length ranges are acceptable (although in some cases they may have no effect).
*/

/* Returns annotated string specified by range. The range should be adjusted according to the standard range adjustment policy, and in addition for this method alone it should be adjusted to begin and end on paragraph boundaries (with possible exceptions for paragraphs exceeding some maximum length). If the range lies within the bounds of the document but is of zero length, it should be adjusted to include the enclosing paragraph. This method should return nil if none of the range lies within the bounds of the document, but if only a zero-length portion of the adjusted range lies within the bounds of the document, as may happen with an empty document or at the end of the document, then an empty attributed string should be returned rather than nil. If the return value is non-nil and actualRange is non-NULL, then actualRange returns the actual adjusted range used.
*/
- (nullable NSAttributedString *)annotatedSubstringForProposedRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange;

/* The receiver replaces any existing annotations on the specified range with the provided annotations. The range should be adjusted according to the standard range adjustment policy. Has no effect if the adjusted range has zero length.
*/
- (void)setAnnotations:(NSDictionary<NSAttributedStringKey, NSString *> *)annotations range:(NSRange)range;

/* The receiver adds the specified annotation to the specified range. The range should be adjusted according to the standard range adjustment policy. Has no effect if the adjusted range has zero length.
*/
- (void)addAnnotations:(NSDictionary<NSAttributedStringKey, NSString *> *)annotations range:(NSRange)range;

/* The receiver removes the specified annotation from the specified range. The range should be adjusted according to the standard range adjustment policy. Has no effect if the adjusted range has zero length.
*/
- (void)removeAnnotation:(NSAttributedStringKey)annotationName range:(NSRange)range;

/* The receiver replaces the text in the specified range with the corrected text from annotatedString, or inserts it if the range has zero length, and replaces existing annotations (if any) with those in annotatedString. The range should be adjusted according to the standard range adjustment policy.
*/
- (void)replaceCharactersInRange:(NSRange)range withAnnotatedString:(NSAttributedString *)annotatedString;

/* The receiver selects the text in the specified range and where appropriate scrolls so that it is at least partially visible. A zero-length selection corresponds to an insertion point. The range should be adjusted according to the standard range adjustment policy.
*/
- (void)selectAndShowRange:(NSRange)range;

/* Returns the view displaying the first logical area for range, and the corresponding rect in view coordinates. The range should be adjusted according to the standard range adjustment policy. May return nil if the range is not being displayed, or if none of the range lies within the bounds of the document. A zero-length selection corresponds to an insertion point, and this should return an appropriate view and rect if the adjusted range is of zero length, provided it lies within the bounds of the document (including at the end of the document) and is being displayed. If the return value is non-nil and actualRange is non-NULL, then actualRange returns the range of text displayed in the returned rect.
*/
- (nullable NSView *)viewForRange:(NSRange)range firstRect:(nullable NSRectPointer)firstRect actualRange:(nullable NSRangePointer)actualRange;

/* Returns the appropriate candidate list touch bar item for displaying touch bar candidates, if any.
*/
- (nullable NSCandidateListTouchBarItem *)candidateListTouchBarItem;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
