/*	NSByteCountFormatter.h
	Copyright (c) 2012-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSFormatter.h>
#import <Foundation/NSMeasurement.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSByteCountFormatterUnits) {
    // This causes default units appropriate for the platform to be used. Specifying any units explicitly causes just those units to be used in showing the number.
    NSByteCountFormatterUseDefault      = 0, 
    //  Specifying any of the following causes the specified units to be used in showing the number.
    NSByteCountFormatterUseBytes        = 1UL << 0, 
    NSByteCountFormatterUseKB           = 1UL << 1,
    NSByteCountFormatterUseMB           = 1UL << 2,
    NSByteCountFormatterUseGB           = 1UL << 3, 
    NSByteCountFormatterUseTB           = 1UL << 4, 
    NSByteCountFormatterUsePB           = 1UL << 5, 
    NSByteCountFormatterUseEB           = 1UL << 6, 
    NSByteCountFormatterUseZB           = 1UL << 7, 
    NSByteCountFormatterUseYBOrHigher   = 0x0FFUL << 8, 
    // Can use any unit in showing the number.
    NSByteCountFormatterUseAll          = 0x0FFFFUL 
};

typedef NS_ENUM(NSInteger, NSByteCountFormatterCountStyle) {
    // Specifies display of file or storage byte counts. The actual behavior for this is platform-specific; on OS X 10.8, this uses the decimal style, but that may change over time.
    NSByteCountFormatterCountStyleFile   = 0,        
    // Specifies display of memory byte counts. The actual behavior for this is platform-specific; on OS X 10.8, this uses the binary style, but that may change over time.
    NSByteCountFormatterCountStyleMemory = 1,
    // The following two allow specifying the number of bytes for KB explicitly. It's better to use one of the above values in most cases.
    NSByteCountFormatterCountStyleDecimal = 2,    // 1000 bytes are shown as 1 KB
    NSByteCountFormatterCountStyleBinary  = 3     // 1024 bytes are shown as 1 KB
};


API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0))
@interface NSByteCountFormatter : NSFormatter {
    @private
    unsigned int _allowedUnits;
    char _countStyle;
    BOOL _allowsNonnumericFormatting, _includesUnit, _includesCount, _includesActualByteCount, _adaptive, _zeroPadsFractionDigits;
    int _formattingContext;
    int _reserved[5];
}

/* Shortcut for converting a byte count into a string without creating an NSByteCountFormatter and an NSNumber. If you need to specify options other than countStyle, create an instance of NSByteCountFormatter first.
 */
+ (NSString *)stringFromByteCount:(long long)byteCount countStyle:(NSByteCountFormatterCountStyle)countStyle;

/* Convenience method on stringForObjectValue:. Convert a byte count into a string without creating an NSNumber.
 */
- (NSString *)stringFromByteCount:(long long)byteCount;

/* Formats the value of the given measurement using the given `countStyle`.

   Throws an exception if the given measurement's unit does not belong to the `NSUnitInformationStorage` dimension.
 */
+ (NSString *)stringFromMeasurement:(NSMeasurement<NSUnitInformationStorage *> *)measurement countStyle:(NSByteCountFormatterCountStyle)countStyle API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));

/* Formats the value of the given measurement using the receiver's `countStyle`.
   Converts the measurement to the units allowed by the receiver's `allowedUnits` before formatting; depending on the value of the measurement, this may result in a string which implies an approximate value (e.g. if the measurement is too large to represent in `allowedUnits`, like `1e20 YB` expressed in `NSByteCountFormatterUseBytes`).

   Throws an exception if the given measurement's unit does not belong to the `NSUnitInformationStorage` dimension.
 */
- (NSString *)stringFromMeasurement:(NSMeasurement<NSUnitInformationStorage *> *)measurement API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));

/* Formats `obj` as a byte count (if `obj` is an `NSNumber`) or specific byte measurement (if `obj` is an `NSMeasurement`) using the receiver's settings.

   Returns `nil` if `obj` is not of the correct class (`NSNumber` or `NSMeasurement`).
   Throws an exception if `obj` is an `NSMeasurement` whose unit does not belong to the `NSUnitInformationStorage` dimension.
 */
- (nullable NSString *)stringForObjectValue:(nullable id)obj;
    
/* Specify the units that can be used in the output. If NSByteCountFormatterUseDefault, uses platform-appropriate settings; otherwise will only use the specified units. This is the default value. Note that ZB and YB cannot be covered by the range of possible values, but you can still choose to use these units to get fractional display ("0.0035 ZB" for instance).
 */
@property NSByteCountFormatterUnits allowedUnits;

/* Specify how the count is displayed by indicating the number of bytes to be used for kilobyte. The default setting is NSByteCountFormatterFileCount, which is the system specific value for file and storage sizes.
 */ 
@property NSByteCountFormatterCountStyle countStyle;

/* Choose whether to allow more natural display of some values, such as zero, where it may be displayed as "Zero KB," ignoring all other flags or options (with the exception of NSByteCountFormatterUseBytes, which would generate "Zero bytes"). The result is appropriate for standalone output. Default value is YES. Special handling of certain values such as zero is especially important in some languages, so it's highly recommended that this property be left in its default state.
 */
@property BOOL allowsNonnumericFormatting;

/* Choose whether to include the number or the units in the resulting formatted string. (For example, instead of 723 KB, returns "723" or "KB".) You can call the API twice to get both parts, separately. But note that putting them together yourself via string concatenation may be wrong for some locales; so use this functionality with care.  Both of these values are YES by default.  Setting both to NO will unsurprisingly result in an empty string.
 */
@property BOOL includesUnit;
@property BOOL includesCount;

/* Choose whether to parenthetically (localized as appropriate) display the actual number of bytes as well, for instance "723 KB (722,842 bytes)".  This will happen only if needed, that is, the first part is already not showing the exact byte count.  If includesUnit or includesCount are NO, then this setting has no effect.  Default value is NO.
*/
@property BOOL includesActualByteCount;

/* Choose the display style. The "adaptive" algorithm is platform specific and uses a different number of fraction digits based on the magnitude (in 10.8: 0 fraction digits for bytes and KB; 1 fraction digits for MB; 2 for GB and above). Otherwise the result always tries to show at least three significant digits, introducing fraction digits as necessary. Default is YES.
 */
@property (getter=isAdaptive) BOOL adaptive;

/* Choose whether to zero pad fraction digits so a consistent number of fraction digits are displayed, causing updating displays to remain more stable. For instance, if the adaptive algorithm is used, this option formats 1.19 and 1.2 GB as "1.19 GB" and "1.20 GB" respectively, while without the option the latter would be displayed as "1.2 GB". Default value is NO.
*/
@property BOOL zeroPadsFractionDigits;

/* Specify the formatting context for the formatted string. Default is NSFormattingContextUnknown.
*/
@property NSFormattingContext formattingContext API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@end

NS_ASSUME_NONNULL_END
