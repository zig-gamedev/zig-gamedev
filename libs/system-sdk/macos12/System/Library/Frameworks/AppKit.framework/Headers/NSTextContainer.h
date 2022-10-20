#if !__has_include(<UIFoundation/NSTextContainer.h>)
/*
        NSTextContainer.h
        Application Kit
        Copyright (c) 1994-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <AppKit/NSParagraphStyle.h>
#import <AppKit/NSLayoutManager.h>

@class NSBezierPath;
@class NSTextLayoutManager;

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

API_AVAILABLE(macos(10.0), ios(7.0), tvos(9.0)) @interface NSTextContainer : NSObject <NSSecureCoding, NSTextLayoutOrientationProvider>

/**************************** Initialization ****************************/

- (instancetype)initWithSize:(NSSize)size NS_DESIGNATED_INITIALIZER API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;


/**************************** Layout ****************************/

// Accessor for the NSLayoutManager object owning the receiver.
// Avoid assigning a layout manager directly through this property.  Adding a text container to a layout manager through -[NSLayoutManager addTextContainer:] will use the property for assigning the new layout manager.
@property (nullable, assign) NSLayoutManager *layoutManager;

// This method should be used instead of the primitive -setLayoutManager: if you need to replace a container's layoutManager with a new one leaving the rest of the web intact.  All the NSTextContainers on the old NSLayoutManager get transferred to the new one.  This method deals with all the work of making sure the containers don't get deallocated and removing the old layoutManager from the text storage and replacing it with the new one.
- (void)replaceLayoutManager:(NSLayoutManager *)newLayoutManager API_AVAILABLE(macos(10.0), ios(9.0), tvos(9.0));

// Returns NSTextLayoutManager owning the text container. When non-nil, -layoutManager should be nil.
@property (weak, nullable, readonly) NSTextLayoutManager *textLayoutManager API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

/************************* Container shape properties *************************/

// Default value: CGSizeZero  Defines the maximum size for the layout area returned from -lineFragmentRectForProposedRect:writingDirection:remainingRect:.  0.0 and less means no limitation.
@property NSSize size API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Default value : empty array  An array of NSBezierPath representing the exclusion paths inside the receiver's bounding rect.
@property (copy) NSArray<NSBezierPath *> *exclusionPaths API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Default value: NSLineBreakByWordWrapping  The line break mode defines the behavior of the last line inside the text container.
@property NSLineBreakMode lineBreakMode API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

/************************* Layout constraint properties *************************/

// Default value: 5.0  The layout padding at the beginning and end of the line fragment rects insetting the layout width available for the contents.  This value is utilized by NSLayoutManager for determining the layout width.
@property CGFloat lineFragmentPadding;

// Default value: 0 (no limit)  The maximum number of lines that can be stored in the receiver.  This value is utilized by NSLayoutManager for determining the maximum number of lines associated with the text container.
@property NSUInteger maximumNumberOfLines API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

/**************************** Line fragments ****************************/

// Returns the bounds of a line fragment rect inside the receiver for proposedRect.  This is the intersection of proposedRect and the receiver's bounding rect defined by -size property.  The regions defined by -exclusionPaths property are excluded from the return value.  charIndex is the character location inside the text storage for the line fragment being processed.  It is possible that proposedRect can be divided into multiple line fragments due to exclusion paths.  In that case, remainingRect returns the remainder that can be passed in as the proposed rect for the next iteration.  baseWritingDirection determines the direction of advancement for line fragments inside a visual horizontal line.  The values passed into the method are either NSWritingDirectionLeftToRight or NSWritingDirectionRightToLeft.  This method can be overridden by subclasses for further text container region customization.
- (NSRect)lineFragmentRectForProposedRect:(NSRect)proposedRect atIndex:(NSUInteger)characterIndex writingDirection:(NSWritingDirection)baseWritingDirection remainingRect:(nullable NSRect *)remainingRect API_AVAILABLE(macos(10.11), ios(7.0), tvos(9.0));

// Returns YES if the receiver is a rectangular shape defined simply by -size. TextKit utilizes this information for enabling various layout optimizations. NSLayoutManager disables non-contiguous layout when this property is NO. The default implementation returns NO when -exclusionPaths has 1 or more items, -maximumNumberOfLines is not 0, or -lineFragmentRectForProposedRect:atIndex:writingDirection:remainingRect: is overridden. It's recommended to override this property when -lineFragmentRectForProposedRect:atIndex:writingDirection:remainingRect: is overridden.
@property (getter=isSimpleRectangularTextContainer, readonly) BOOL simpleRectangularTextContainer NS_AVAILABLE(10_0, 9_0);


/**************************** View synchronization ****************************/

// Default value: NO  Define whether the text container view bounds changes can affect the text container size.
@property BOOL widthTracksTextView;
@property BOOL heightTracksTextView;

// Set/get the view which the container is drawn in.  Having a view is optional.
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_12
@property (nullable, strong) NSTextView *textView;
#else
@property (nullable, weak) NSTextView *textView;
#endif
@end

/**************************** Deprecated ****************************/
// NSLineSweepDirection and NSLineMovementDirection are soft deprecated starting with OS X 10.11. It will be officially deprecated in a future release
typedef NS_ENUM(NSUInteger, NSLineSweepDirection) {
    NSLineSweepLeft     = 0,
    NSLineSweepRight    = 1,
    NSLineSweepDown     = 2,
    NSLineSweepUp       = 3
};

typedef NS_ENUM(NSUInteger, NSLineMovementDirection) {
    NSLineDoesntMove    = 0,
    NSLineMovesLeft     = 1,
    NSLineMovesRight    = 2,
    NSLineMovesDown     = 3,
    NSLineMovesUp       = 4
};

@interface NSTextContainer (NSTextContainerDeprecated)
// Methods names with "containerSize" are soft deprecated starting with OS X 10.11. It will be officially deprecated in a future release
- (instancetype)initWithContainerSize:(NSSize)aContainerSize; // Use -initWithSize: instead. For binary compatibility, this method now just calls [self initWithSize:]. Also, -init still calls -initWithContainerSize:.
@property NSSize containerSize; // Use -size instead

// This method is soft deprecated starting with OS X 10.11. It will be officially deprecated in a future release
- (NSRect)lineFragmentRectForProposedRect:(NSRect)proposedRect sweepDirection:(NSLineSweepDirection)sweepDirection movementDirection:(NSLineMovementDirection)movementDirection remainingRect:(nullable NSRectPointer)remainingRect; // Use -lineFragmentRectForProposedRect:atIndex:writingDirection:remainingRect: instead

- (BOOL)containsPoint:(NSPoint)point API_DEPRECATED("", macos(10.0,10.11));
@end



#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSTextContainer.h>
#endif
