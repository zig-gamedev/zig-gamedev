/*    NSListFormatter.h
      Copyright (c) 2018-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSFormatter.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>

NS_ASSUME_NONNULL_BEGIN

/* NSListFormatter provides locale-correct formatting of a list of items using the appropriate separator and conjunction. Note that the list formatter is unaware of the context where the joined string will be used, e.g., in the beginning of the sentence or used as a standalone string in the UI, so it will not provide any sort of capitalization customization on the given items, but merely join them as-is. The string joined this way may not be grammatically correct when placed in a sentence, and it should only be used in a standalone manner.
*/
API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
@interface NSListFormatter : NSFormatter
#if !__OBJC2__
{
@private
    void * _listFormatter;
    NSLocale *_locale;
    NSFormatter *_itemFormatter;
}
#endif // !__OBJC2__

/* Specifies the locale to format the items. Defaults to autoupdatingCurrentLocale. Also resets to autoupdatingCurrentLocale on assignment of nil.
 */
@property (null_resettable, copy) NSLocale *locale;

/* Specifies how each object should be formatted. If not set, the object is formatted using its instance method in the following order: -descriptionWithLocale:, -localizedDescription, and -description.
 */
@property (nullable, copy) NSFormatter *itemFormatter;

/* Convenience method to return a string constructed from an array of strings using the list format specific to the current locale. It is recommended to join only disjointed strings that are ready to display in a bullet-point list. Sentences, phrases with punctuations, and appositions may not work well when joined together.
 */
+ (NSString *)localizedStringByJoiningStrings:(NSArray<NSString *> *)strings;

/* Convenience method for -stringForObjectValue:. Returns a string constructed from an array in the locale-aware format. Each item is formatted using the itemFormatter. If the itemFormatter does not apply to a particular item, the method will fall back to the item's -descriptionWithLocale: or -localizedDescription if implemented, or -description if not.

 Returns nil if `items` is nil or if the list formatter cannot generate a string representation for all items in the array.
 */
- (nullable NSString *)stringFromItems:(NSArray *)items;

/* Inherited from NSFormatter. `obj` must be an instance of NSArray. Returns nil if `obj` is nil, not an instance of NSArray, or if the list formatter cannot generate a string representation for all objects in the array.
 */
- (nullable NSString *)stringForObjectValue:(nullable id)obj;

@end

NS_ASSUME_NONNULL_END
