#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextAttachment.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
/*
        NSTextAttachment.h
        Copyright (c) 1994-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSAttributedString.h>
#import <AppKit/AppKitDefines.h>
#import <CoreGraphics/CGGeometry.h>

NS_ASSUME_NONNULL_BEGIN

enum {
    NSAttachmentCharacter API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos) = 0xFFFC // Replacement character is used for attachments
};

@class NSTextContainer;
@class NSLayoutManager;
@class NSImage;
@class NSView;
@class NSFileWrapper;
@class NSTextAttachmentViewProvider;
@class NSTextLayoutManager;
@protocol NSTextLocation;
@class NSTextAttachmentCell;
@protocol NSTextAttachmentCell;


// This protocol defines the interface to attachment objects from NSLayoutManager
@protocol NSTextAttachmentContainer <NSObject>
// This protocol is available only for Objective-C 2 or later architecture

// Returns the image object rendered by NSLayoutManager at imageBounds inside textContainer.  It should return an image appropriate for the target rendering context derived by arguments to this method.  The NSTextAttachment implementation returns -image when non-nil.  If -image==nil, it returns an image based on -contents and -fileType properties.
- (nullable NSImage *)imageForBounds:(CGRect)imageBounds textContainer:(nullable NSTextContainer *)textContainer characterIndex:(NSUInteger)charIndex  API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);


// Returns the layout bounds to the layout manager.  The bounds origin is interpreted to match position inside lineFrag.  The NSTextAttachment implementation returns -bounds if not CGRectZero; otherwise, it derives the bounds value from -[image size].  Conforming objects can implement more sophisticated logic for negotiating the frame size based on the available container space and proposed line fragment rect.
- (CGRect)attachmentBoundsForTextContainer:(nullable NSTextContainer *)textContainer proposedLineFragment:(CGRect)lineFrag glyphPosition:(CGPoint)position characterIndex:(NSUInteger)charIndex API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

@end

// This protocol defines the interface to attachment objects from NSTextLayoutManager
@protocol NSTextAttachmentLayout <NSObject>

// Returns the image object rendered at bounds inside textContainer.  It should return an image appropriate for the target rendering context derived by arguments to this method.  The NSTextAttachment implementation returns -image when non-nil.  If -image==nil, it returns an image based on -contents and -fileType properties.
- (nullable NSImage *)imageForBounds:(CGRect)bounds attributes:(NSDictionary<NSAttributedStringKey, id> *)attributes location:(id <NSTextLocation>)location textContainer:(nullable NSTextContainer *)textContainer API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

// Returns the layout bounds. The bounds origin is interpreted to match position inside proposedLineFragment. The NSTextAttachment implementation returns -bounds if not CGRectZero; otherwise, it derives the bounds value from image.size. Conforming objects can implement more sophisticated logic for negotiating the frame size based on the available container space and proposed line fragment rect.
- (CGRect)attachmentBoundsForAttributes:(NSDictionary<NSAttributedStringKey, id> *)attributes location:(id <NSTextLocation>)location textContainer:(nullable NSTextContainer *)textContainer proposedLineFragment:(CGRect)proposedLineFragment position:(CGPoint)position API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

// Returns the text attachment view provider corresponding to -fileType. The default implementation queries the text attachment view provider class via +[NSTextAttachment textAttachmentViewProviderClassForFileType:]. When non-nil, it instantiates a view, then, fills properties declared in NSTextAttachmentViewProvider if implemented
- (nullable NSTextAttachmentViewProvider *)viewProviderForParentView:(nullable NSView *)parentView location:(id <NSTextLocation>)location textContainer:(nullable NSTextContainer *)textContainer API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);
@end

API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos) @interface NSTextAttachment : NSObject <NSTextAttachmentLayout, NSTextAttachmentContainer, NSSecureCoding>

/**************************** Initialization ****************************/

// Designated initializer.  Both arguments can be nil.  When contentData==nil || uti==nil, the receiver is consider to be an attachment without document contents.  In this case, the NSAttributedString methods writing external file format tries to save the return value of -[NSTextAttachment image] instead.
- (instancetype)initWithData:(nullable NSData *)contentData ofType:(nullable NSString *)uti NS_DESIGNATED_INITIALIZER API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

// Previous designated initializer on OS X. This method invokes -initWithData:ofType: with nil arguments, then, fills -fileWrapper property.
- (instancetype)initWithFileWrapper:(nullable NSFileWrapper *)fileWrapper;


/**************************** Content properties ****************************/

// These two properties define the contents for the text attachment.  Modifying these properties have a side effect of invalidating -image and -fileWrapper properties. -fileType is an UTI describing the format for -contents.
@property (nullable, copy) NSData *contents API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);
@property (nullable, copy) NSString *fileType API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

/**************************** Rendering/layout properties ****************************/

// Image representing the text attachment contents. Modifying this property invalidates -contents, -fileType, and -FileWrapper properties.
@property (nullable, strong) NSImage *image API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

// Defines the layout bounds of the receiver's graphical representation in the text coordinate system.  The origin is at the glyph location on the text baseline.  The default value is CGRectZero.
@property CGRect bounds API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);

/**************************** Non-image contents properties ****************************/

// Optionally, NSTextAttachment can be associated with a file wrapper. Modifying this property has a side effect of invalidating -image, -contents, and fileType properties.
@property (nullable, strong) NSFileWrapper *fileWrapper;

// The cell which handles user interaction. By default an instance of NSTextAttachmentCell is used.
@property (nullable, strong) id <NSTextAttachmentCell> attachmentCell API_AVAILABLE(macos(10.0)) API_UNAVAILABLE(ios, watchos, tvos);

#pragma mark Advanced Layout Properties
// Layout padding before and after the text attachment bounds. The layout and rendering bounds X origin gets inset by the padding value. This affects the relationship between the text attachment bounds. 0.0 by default
@property CGFloat lineLayoutPadding API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);


#pragma mark View-based attachments
// Factory methods for defining the mapping between UTI and text attachment view provider class
+ (nullable Class)textAttachmentViewProviderClassForFileType:(NSString *)fileType API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);
+ (void)registerTextAttachmentViewProviderClass:(Class)textAttachmentViewProviderClass forFileType:(NSString *)fileType API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

// When YES, the text attachment tries to use a text attachment view returned by -viewProviderForParentView:location:textContainer:. YES by default
@property BOOL allowsTextAttachmentView API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

// Returns YES if the text attachment is configured to use text attachment views. By default, it checks -allowsTextAttachmentView, +textAttachmentViewClassForFileType:, and its contents
@property (readonly) BOOL usesTextAttachmentView API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);
@end

@interface NSAttributedString (NSAttributedStringAttachmentConveniences)
// A convenience method for creating an attributed string containing attachment using NSAttachmentCharacter as the base character.
+ (NSAttributedString *)attributedStringWithAttachment:(NSTextAttachment *)attachment API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0)) API_UNAVAILABLE(watchos);
@end

API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextAttachmentViewProvider : NSObject
- (instancetype)initWithTextAttachment:(NSTextAttachment *)textAttachment parentView:(nullable NSView *)parentView textLayoutManager:(nullable NSTextLayoutManager *)textLayoutManager location:(id <NSTextLocation>)location NS_DESIGNATED_INITIALIZER;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@property (readonly, weak) NSTextAttachment *textAttachment;
@property (readonly, weak, nullable) NSTextLayoutManager *textLayoutManager;

@property (readonly, strong) id <NSTextLocation> location;

// View instantiation
// The getter first invokes loadView if the view hasn't been set yet. Subclasses must call super if they override the setter or getter.
@property(nullable, strong) NSView *view;

// This is where subclasses should create their custom view hierarchy. Should never be called directly.
- (void)loadView;

// This property determines the text attachment bounds policy. If YES, -[NSTextAttachment attachmentBoundsForAttributes:location:textContainer:proposedLineFragment:position:] consults the text attachment view provider for determining the bounds instead of using -bounds. NO by default
@property BOOL tracksTextAttachmentViewBounds;

// Invoked from -[NSTextAttachment attachmentBoundsForAttributes:location:textContainer:proposedLineFragment:position:]
- (CGRect)attachmentBoundsForAttributes:(NSDictionary<NSAttributedStringKey, id> *)attributes location:(id <NSTextLocation>)location textContainer:(nullable NSTextContainer *)textContainer proposedLineFragment:(CGRect)proposedLineFragment position:(CGPoint)position;
@end


@interface NSMutableAttributedString (NSMutableAttributedStringAttachmentConveniences)

- (void)updateAttachmentsFromPath:(NSString *)path;

@end



NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextAttachment.h>
#endif

#import <AppKit/NSTextAttachmentCell.h>
