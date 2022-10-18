/*
	NSPrintInfo.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSGeometry.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSDictionary.h>
#import <AppKit/AppKitDefines.h>
#import <AppKit/NSPrinter.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSPDFInfo, NSPrinter;

/* Valid values for the NSPrintOrientation attribute.
*/
typedef NS_ENUM(NSInteger, NSPaperOrientation) {
    NSPaperOrientationPortrait = 0,
    NSPaperOrientationLandscape = 1
} API_AVAILABLE(macos(10.9));

/* Valid values for the NSPrintHorizontalPagination and NSPrintVerticalPagination attributes.
*/
typedef NS_ENUM(NSUInteger, NSPrintingPaginationMode) {
    NSPrintingPaginationModeAutomatic = 0,
    NSPrintingPaginationModeFit = 1,
    NSPrintingPaginationModeClip = 2
};


typedef NSString * NSPrintInfoAttributeKey NS_TYPED_EXTENSIBLE_ENUM;

/* Keys for page setup attributes that are recognized by NSPrintInfo.
*/
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintPaperName; // an NSString
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintPaperSize; // an NSValue containing an NSSize, in points
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintOrientation; // an NSNumber containing NSPaperOrientationPortrait or NSPaperOrientationLandscape
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintScalingFactor; // an NSNumber containing a floating-point percentage

/* Keys for pagination attributes that are recognized by NSPrintInfo.
*/
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintLeftMargin; // an NSNumber containing a measurement in floating-point points
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintRightMargin; // an NSNumber containing a measurement in floating-point points
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintTopMargin; // an NSNumber containing a measurement in floating-point points
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintBottomMargin; // an NSNumber containing a measurement in floating-point points
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintHorizontallyCentered; // an NSNumber containing a boolean value
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintVerticallyCentered; // an NSNumber containing a boolean value
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintHorizontalPagination; // an NSNumber containing NSPrintingPaginationModeAutomatic, NSPrintingPaginationModeFit, or NSPrintingPaginationModeClip
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintVerticalPagination; // an NSNumber containing NSPrintingPaginationModeAutomatic, NSPrintingPaginationModeFit, or NSPrintingPaginationModeClip

/* Keys for print job attributes that are recognized by NSPrintInfo.
*/
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintPrinter;	// an NSPrinter
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintCopies; // an NSNumber containing the number of copies of the print job to be printed
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintAllPages; // an NSNumber containing a boolean value
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintFirstPage; // an NSNumber containing the one-based index of the first job in the page to print
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintLastPage; // an NSNumber containing the one-based index of the last job in the page to print
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintMustCollate; // an NSNumber containing a boolean value
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintReversePageOrder; // an NSNumber containing a boolean value
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintJobDisposition; // an NSPrintJobDispositionValue equal to NSPrintSpoolJob, NSPrintPreviewJob, NSPrintSaveJob, or NSPrintCancelJob
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintPagesAcross; // an NSNumber containing the number of logical pages to be placed across a physical sheet
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintPagesDown; // an NSNumber containing the number of logical pages to be placed down a physical sheet
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintTime; // an NSDate containing the time at which printing should begin
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintDetailedErrorReporting; // an NSNumber containing a boolean value
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintFaxNumber; // an NSString containing a fax number
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintPrinterName; // an NSString containing the name of a printer
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintSelectionOnly API_AVAILABLE(macos(10.6)); // an NSNumber containing a boolean value
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintJobSavingURL API_AVAILABLE(macos(10.6)); // an NSURL containing the location to which the job file will be saved, for NSPrintSaveJob
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintJobSavingFileNameExtensionHidden API_AVAILABLE(macos(10.6)); // A boolean NSNumber for whether the job file's name extension should be hidden, for NSPrintSaveJob

/* Keys for page adornment attributes that are recognized by NSPrintInfo.
*/
APPKIT_EXTERN NSPrintInfoAttributeKey const NSPrintHeaderAndFooter; // a boolean NSNumber for whether the results of -[NSView pageHeader] and -[NSView pageFooter] should be drawn on pages


/* Valid values for the NSPrintJobDisposition attribute.
 */
typedef NSString * NSPrintJobDispositionValue NS_TYPED_ENUM;
APPKIT_EXTERN NSPrintJobDispositionValue const NSPrintSpoolJob;
APPKIT_EXTERN NSPrintJobDispositionValue const NSPrintPreviewJob;
APPKIT_EXTERN NSPrintJobDispositionValue const NSPrintSaveJob;
APPKIT_EXTERN NSPrintJobDispositionValue const NSPrintCancelJob;


typedef NSString *NSPrintInfoSettingKey NS_SWIFT_BRIDGED_TYPEDEF;

@interface NSPrintInfo : NSObject<NSCopying, NSCoding>

/* Set or get the "shared" instance of NSPrintInfo. The shared print info object is the one that is used automatically by -[NSPageLayout runModal] and +[NSPrintOperation printOperationWithView:].
*/
@property (class, strong) NSPrintInfo *sharedPrintInfo;

/* Given a dictionary that contains attribute entries, initialize. Attributes that are recognized by the NSPrintInfo class will be silently validated in the context of the printer selected by the attributes dictionary, or the default printer if the attributes dictionary selects no printer. Attributes that are not recognized by the NSPrintInfo class will be preserved, and returned in the dictionary returned by the -dictionary method, but otherwise ignored. This is the designated initializer for this class.
*/
- (instancetype)initWithDictionary:(NSDictionary<NSPrintInfoAttributeKey, id> *)attributes NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)init; // Convience method that calls through to initWithDictionary:nil

/* Return a dictionary that contains attribute entries. This dictionary may contain attributes that were not specified in the dictionary originally passed to this object by -initWithDictionary. Changes to this dictionary will be reflected in the values returned by subsequent invocations of other of this class' methods.
*/
- (NSMutableDictionary<NSPrintInfoAttributeKey, id> *)dictionary;

/* Set or get the values of the paper attributes.  Because an NSPrintInfo's paper name, paper size, and orientation attributes must be kept consistent, invocation of any of the setting methods in this group may affect the values returned by subsequent invocations of any of the getting methods in this group.  For example, paper name and paper size must always agree, and the value returned by -paperSize always takes orientation into account.
*/
@property (nullable, copy) NSPrinterPaperName paperName;
@property NSSize paperSize;
@property NSPaperOrientation orientation;
@property CGFloat scalingFactor API_AVAILABLE(macos(10.6));

/* Set or get the values of the pagination attributes.
*/
@property CGFloat leftMargin;
@property CGFloat rightMargin;
@property CGFloat topMargin;
@property CGFloat bottomMargin;
@property (getter=isHorizontallyCentered) BOOL horizontallyCentered;
@property (getter=isVerticallyCentered) BOOL verticallyCentered;
@property NSPrintingPaginationMode horizontalPagination;
@property NSPrintingPaginationMode verticalPagination;

/* Set or get the value of the job disposition attribute.
*/
@property (copy) NSPrintJobDispositionValue jobDisposition;

/* Set or get the destination printer specified by this object.
*/
@property (copy) NSPrinter *printer;

/* Validate all of the attributes encapsulated by this object. This method is invoked automatically before the object is used by an NSPrintOperation. This method may be overridden to perform validation of attributes that are not recognized by the NSPrintInfo class.
*/
- (void)setUpPrintOperationDefaultValues;


/* Return the imageable area of a page specified by this object, taking into account the current printer, paper, and orientation settings, but not scaling. "Imageable area" is the maximum area that can possibly be marked on by the printer hardware, not the area defined by the current margin settings. The rectangle is in a coordinate space measured by points, with (0, 0) being the lower-left corner of the oriented page and (paperWidth, paperHeight) being the upper-right corner of the oriented page. The imageable bounds may extend past the edges of the page when, for example, a printer driver specifies it so that borderless printing can be done reliably.
*/
@property (readonly) NSRect imageablePageBounds;



/* Return the human-readable name of the currently selected paper size, suitable for presentation in user interfaces.  This will typically be different from the name returned by -paperName, which is almost never suitable for presentation to to the user.
*/
@property (nullable, readonly, copy) NSString *localizedPaperName;


/* Return the default printer, if one has been selected by the user, nil otherwise.
*/
@property (class, readonly, nullable, strong) NSPrinter *defaultPrinter;


/* The print info's print settings. You can put values in this dictionary to store them in any preset that the user creates while editing this print info with a print panel. Such values must be property list objects. This class is key-value coding (KVC) and key-value observing (KVO) compliant for "printSettings" so you can often bind controls in print panel accessory views directly to entries in this dictionary. You can also use this dictionary to get values that have been set by other parts of the printing system, like a printer driver's print dialog extension (the same sort of values that are returned by the Carbon Printing Manager's PMPrintSettingsGetValue() function). Other parts of the printing system often use key strings like "com.apple.print.PrintSettings.PMColorSyncProfileID" but dots like those in key strings wouldn't work well with KVC, so those dots are replaced with underscores in keys that appear in this dictionary, as in "com_apple_print_PrintSettings_PMColorSyncProfileID". You should use the same convention when adding entries to this dictionary.
*/
@property (readonly, strong) NSMutableDictionary<NSPrintInfoSettingKey, id> *printSettings API_AVAILABLE(macos(10.5));

/* Return a Printing Manager PMPrintSession, PMPageFormat, or PMPrintSettings object, respectively. The returned object is always consistent with the state of the NSPrintInfo at the moment the method is invoked, but isn't necessarily updated immediately if other NSPrintInfo methods like -setPaperSize: and -setPaperOrientation: are invoked. The returned object will always be valid (in the Printing Manager sense). If you set any values in the returned PMPageFormat or PMPrintSettings you should afterward invoke -updateFromPMPageFormat or -updateFromPMPrintSettings, respectively. You don't also have to call PMSessionValidatePageFormat() or PMSessionValidatePrintSettings() if you do that. You should not call PMRelease() for the returned object, except of course to balance any calls of PMRetain() you do.
*/
- (void *)PMPrintSession NS_RETURNS_INNER_POINTER API_AVAILABLE(macos(10.5)); // the result should be cast to PMPrintSession
- (void *)PMPageFormat NS_RETURNS_INNER_POINTER API_AVAILABLE(macos(10.5)); // the result should be cast to PMPageFormat
- (void *)PMPrintSettings NS_RETURNS_INNER_POINTER API_AVAILABLE(macos(10.5)); // the result should be cast to PMPrintSettings

/* Given that the NSPrintInfo's PMPageFormat or PMPrintSettings has been changed by something other than the NSPrintInfo itself, update the NSPrintInfo to be consistent.
*/
- (void)updateFromPMPageFormat API_AVAILABLE(macos(10.5));
- (void)updateFromPMPrintSettings API_AVAILABLE(macos(10.5));



/* Set or get whether only the selected content should be printed.
*/
@property (getter=isSelectionOnly) BOOL selectionOnly API_AVAILABLE(macos(10.6));

/* Update the receiver with all settings and attributes present in the given NSPDFInfo. Entries in the given NSPDFInfo's attributes dictionary will be copied into the receiver's attributes dictionary ([self dictionary]). If inPDFInfo is the result of an NSPDFPanel with the NSPDFPanelRequestsParentDirectory option enabled, you must first modify inPDFInfo.URL by appending a file name. If inPDFInfo.URL points to a directory, this method will throw an exception.
*/
- (void)takeSettingsFromPDFInfo:(NSPDFInfo *)inPDFInfo API_AVAILABLE(macos(10.9));

@end

@interface NSPrintInfo(NSDeprecated)

+ (void)setDefaultPrinter:(nullable NSPrinter *)printer API_DEPRECATED("NSPrintInfo's implementation has no effect", macos(10.0,10.2));

/* NSPrintInfo's implementation of this method recognizes only a small fixed set of paper names, and does not take the details of any particular printer into account.
*/
+ (NSSize)sizeForPaperName:(nullable NSPrinterPaperName)name API_DEPRECATED("Use -[NSPrinter pageSizeForPaper:] instead", macos(10.0,10.2));

@end

APPKIT_EXTERN NSString * const NSPrintFormName API_DEPRECATED("NSPrintInfo does not recognize this attribute", macos(10.0,10.2));
APPKIT_EXTERN NSString * const NSPrintJobFeatures API_DEPRECATED("NSPrintInfo does not recognize this attribute. -[NSPrintInfo setUpPrintOperationDefaultValues] sets a default value of an empty dictionary", macos(10.0,10.2));
APPKIT_EXTERN NSString * const NSPrintManualFeed API_DEPRECATED("NSPrintInfo does not recognize this attribute", macos(10.0,10.2));
APPKIT_EXTERN NSString * const NSPrintPagesPerSheet API_DEPRECATED("NSPrintInfo does not recognize this attribute. -[NSPrintInfo setUpPrintOperationDefaultValues] sets a default value of 1", macos(10.0,10.2));
APPKIT_EXTERN NSString * const NSPrintPaperFeed API_DEPRECATED("NSPrintInfo does not recognize this attribute", macos(10.0,10.2));

APPKIT_EXTERN NSString * const NSPrintSavePath API_DEPRECATED("Use NSPrintJobSavingURL instead", macos(10.0,10.6));

typedef NS_ENUM(NSUInteger, NSPrintingOrientation) {
    NSPortraitOrientation API_DEPRECATED_WITH_REPLACEMENT("NSPaperOrientationPortrait", macos(10.0,10.14)) = 0,
    NSLandscapeOrientation API_DEPRECATED_WITH_REPLACEMENT("NSPaperOrientationLandscape", macos(10.0,10.14)) = 1
} API_DEPRECATED_WITH_REPLACEMENT("NSPaperOrientation", macos(10.0,10.14));

/* Deprecated legacy printing pagination mode constants. Prefer to use NSPrintingPaginationMode values instead.
*/
static const NSPrintingPaginationMode NSAutoPagination API_DEPRECATED_WITH_REPLACEMENT("NSPrintingPaginationModeAutomatic", macos(10.0, 11.0)) = NSPrintingPaginationModeAutomatic;
static const NSPrintingPaginationMode NSFitPagination API_DEPRECATED_WITH_REPLACEMENT("NSPrintingPaginationModeFit", macos(10.0, 11.0)) = NSPrintingPaginationModeFit;
static const NSPrintingPaginationMode NSClipPagination API_DEPRECATED_WITH_REPLACEMENT("NSPrintingPaginationModeClip", macos(10.0, 11.0)) = NSPrintingPaginationModeClip;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
