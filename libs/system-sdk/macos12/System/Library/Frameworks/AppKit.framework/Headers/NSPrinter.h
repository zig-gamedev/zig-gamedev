/*
	NSPrinter.h
	Application Kit
	Copyright (c) 1994-2021, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSGeometry.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <AppKit/NSGraphics.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString;

/* Valid return values for -[NSPrinter statusForTable:].
*/
typedef NS_ENUM(NSUInteger, NSPrinterTableStatus) {
    NSPrinterTableOK = 0,
    NSPrinterTableNotFound = 1,
    NSPrinterTableError = 2
};

typedef NSString * NSPrinterTypeName NS_TYPED_EXTENSIBLE_ENUM;
typedef NSString * NSPrinterPaperName NS_TYPED_EXTENSIBLE_ENUM;

@interface NSPrinter: NSObject<NSCopying, NSCoding>

/* Return an array of strings that contain the human-readable names of all available printers.
*/
@property (class, readonly, copy) NSArray<NSString *> *printerNames;

/* Return an array of strings that contain the human-readable descriptions of the makes and models of all available printers. The array may contain fewer entries than the array that would be returned by an invocation of +printerNames.
*/
@property (class, readonly, copy) NSArray<NSPrinterTypeName> *printerTypes;

/* Given a human-readable printer name of the sort that appears in a Print panel's "Printer:" popup menu, return an instance of NSPrinter if the named printer is available, nil otherwise.
*/
+ (nullable NSPrinter *)printerWithName:(NSString *)name;

/* Given a string of the sort returned by +printerTypes:, return the first printer whose description matches, or nil if no available printer matches.
*/
+ (nullable NSPrinter *)printerWithType:(NSPrinterTypeName)type;

/* Return the printer's human-readable name, or an empty string for failure.
*/
@property (readonly, copy) NSString *name;

/* Return a human-readable description of the printer's make and model, or an empty string for failure.
*/
@property (readonly, copy) NSPrinterTypeName type;

/* Return the PostScript language level of the printer if it is a PostScript printer, or 0 if it is not a PostScript printer or for failure.
*/
@property (readonly) NSInteger languageLevel;

/* Given a valid paper name, return the corresponding paper size in points, or NSZeroSize for failure.
*/
- (NSSize)pageSizeForPaper:(NSPrinterPaperName)paperName;

/* Return a dictionary that describes the printing device using entries keyed by the NSDevice... strings declared in NSGraphics.h. The only entry that is guaranteed to exist in the returned dictionary is NSDeviceIsPrinter.
*/
@property (readonly, copy) NSDictionary<NSDeviceDescriptionKey, id> *deviceDescription;

@end

@interface NSPrinter(NSDeprecated)


/* PMPrinter API should be used instead. Return the status of the named table in the printer's PostScript Printer Description (PPD) if the PPD is available. NSPrinterTableNotFound is returned for every table name if no PPD is available.
 */
- (NSPrinterTableStatus)statusForTable:(NSString *)tableName API_DEPRECATED("", macos(10.0,10.9));

/* PMPrinter API should be used instead. Methods for accessing the information in the printer's PPD. If no PPD is available for the printer, values such as NO, 0, NSZeroRect, NSZeroSize, and nil are returned.
 */
- (BOOL)isKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));
- (BOOL)booleanForKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));
- (float)floatForKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));
- (int)intForKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));
- (NSRect)rectForKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));
- (NSSize)sizeForKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));
- (nullable NSString *)stringForKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));
- (nullable NSArray *)stringListForKey:(nullable NSString *)key inTable:(NSString *)table API_DEPRECATED("", macos(10.0,10.9));


/* -[NSPrinter imageRectForPaper:] will attempt to determine and return the bounds of the imageable area for a particular paper name, but querying such information by paper name alone is not reliable. Use -[NSPrintInfo imageablePageBounds], which was introduced in Mac OS 10.2, instead.
*/
- (NSRect)imageRectForPaper:(nullable NSString *)paperName API_DEPRECATED("", macos(10.0,10.2));

/* NSPrinter's implementations of these methods return values that are not reliable or meaningful for some printers.
*/
- (BOOL)acceptsBinary API_DEPRECATED("", macos(10.0,10.2));
- (BOOL)isColor API_DEPRECATED("", macos(10.0,10.2));
- (BOOL)isFontAvailable:(nullable NSString *)faceName API_DEPRECATED("", macos(10.0,10.2));
- (BOOL)isOutputStackInReverseOrder API_DEPRECATED("", macos(10.0,10.2));

/* NSPrinter's implementation of each of these methods returns nil.
*/
+ (nullable NSPrinter *)printerWithName:(NSString *)name domain:(nullable NSString *)domain includeUnavailable:(BOOL)flag API_DEPRECATED("", macos(10.0,10.2));
- (NSString *)domain API_DEPRECATED("", macos(10.0,10.2));
- (NSString *)host API_DEPRECATED("", macos(10.0,10.2));
- (NSString *)note API_DEPRECATED("", macos(10.0,10.2));

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
