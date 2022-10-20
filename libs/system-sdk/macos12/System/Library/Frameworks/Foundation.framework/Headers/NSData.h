/*	NSData.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSRange.h>

@class NSString, NSURL, NSError;

NS_ASSUME_NONNULL_BEGIN

/****************	Read/Write Options	****************/

typedef NS_OPTIONS(NSUInteger, NSDataReadingOptions) {
    NSDataReadingMappedIfSafe =   1UL << 0,	// Hint to map the file in if possible and safe
    NSDataReadingUncached = 1UL << 1,	// Hint to get the file not to be cached in the kernel
    NSDataReadingMappedAlways API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)) = 1UL << 3,	// Hint to map the file in if possible. This takes precedence over NSDataReadingMappedIfSafe if both are given.
    
    // Options with old names for NSData reading methods. Please stop using these old names.
    NSDataReadingMapped API_DEPRECATED_WITH_REPLACEMENT("NSDataReadingMappedIfSafe", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) = NSDataReadingMappedIfSafe,	// Deprecated name for NSDataReadingMappedIfSafe
    NSMappedRead API_DEPRECATED_WITH_REPLACEMENT("NSDataReadingMapped", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) = NSDataReadingMapped,			// Deprecated name for NSDataReadingMapped
    NSUncachedRead API_DEPRECATED_WITH_REPLACEMENT("NSDataReadingUncached", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) = NSDataReadingUncached		// Deprecated name for NSDataReadingUncached
};

typedef NS_OPTIONS(NSUInteger, NSDataWritingOptions) {
    NSDataWritingAtomic = 1UL << 0,	// Hint to use auxiliary file when saving; equivalent to atomically:YES
    NSDataWritingWithoutOverwriting API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = 1UL << 1, // Hint to  prevent overwriting an existing file. Cannot be combined with NSDataWritingAtomic.

    NSDataWritingFileProtectionNone API_AVAILABLE(macos(11.0), ios(4.0), watchos(2.0), tvos(9.0))                                  = 0x10000000,
    NSDataWritingFileProtectionComplete API_AVAILABLE(macos(11.0), ios(4.0), watchos(2.0), tvos(9.0))                              = 0x20000000,
    NSDataWritingFileProtectionCompleteUnlessOpen API_AVAILABLE(macos(11.0), ios(5.0), watchos(2.0), tvos(9.0))                    = 0x30000000,
    NSDataWritingFileProtectionCompleteUntilFirstUserAuthentication API_AVAILABLE(macos(11.0), ios(5.0), watchos(2.0), tvos(9.0))  = 0x40000000,
    NSDataWritingFileProtectionMask API_AVAILABLE(macos(11.0), ios(4.0), watchos(2.0), tvos(9.0))                                  = 0xf0000000,

    // Options with old names for NSData writing methods. Please stop using these old names.
    NSAtomicWrite API_DEPRECATED_WITH_REPLACEMENT("NSDataWritingAtomic", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) = NSDataWritingAtomic	    // Deprecated name for NSDataWritingAtomic
};


/****************	Data Search Options	****************/

typedef NS_OPTIONS(NSUInteger, NSDataSearchOptions) {
    NSDataSearchBackwards = 1UL << 0,
    NSDataSearchAnchored = 1UL << 1
} API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));


/****************        Base 64 Options	****************/

typedef NS_OPTIONS(NSUInteger, NSDataBase64EncodingOptions) {
    // Use zero or one of the following to control the maximum line length after which a line ending is inserted. No line endings are inserted by default.
    NSDataBase64Encoding64CharacterLineLength = 1UL << 0,
    NSDataBase64Encoding76CharacterLineLength = 1UL << 1,

    // Use zero or more of the following to specify which kind of line ending is inserted. The default line ending is CR LF.
    NSDataBase64EncodingEndLineWithCarriageReturn = 1UL << 4,
    NSDataBase64EncodingEndLineWithLineFeed = 1UL << 5,

} API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));


typedef NS_OPTIONS(NSUInteger, NSDataBase64DecodingOptions) {
    // Use the following option to modify the decoding algorithm so that it ignores unknown non-Base64 bytes, including line ending characters.
    NSDataBase64DecodingIgnoreUnknownCharacters = 1UL << 0
} API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));


/****************	Immutable Data		****************/

@interface NSData : NSObject <NSCopying, NSMutableCopying, NSSecureCoding>

@property (readonly) NSUInteger length;
/*
 The -bytes method returns a pointer to a contiguous region of memory managed by the receiver.
 If the regions of memory represented by the receiver are already contiguous, it does so in O(1) time, otherwise it may take longer
 Using -enumerateByteRangesUsingBlock: will be efficient for both contiguous and discontiguous data.
 */
@property (readonly) const void *bytes NS_RETURNS_INNER_POINTER;

@end

@interface NSData (NSExtendedData)

@property (readonly, copy) NSString *description;
- (void)getBytes:(void *)buffer length:(NSUInteger)length;
- (void)getBytes:(void *)buffer range:(NSRange)range;
- (BOOL)isEqualToData:(NSData *)other;
- (NSData *)subdataWithRange:(NSRange)range;
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile;
- (BOOL)writeToURL:(NSURL *)url atomically:(BOOL)atomically; // the atomically flag is ignored if the url is not of a type the supports atomic writes
- (BOOL)writeToFile:(NSString *)path options:(NSDataWritingOptions)writeOptionsMask error:(NSError **)errorPtr;
- (BOOL)writeToURL:(NSURL *)url options:(NSDataWritingOptions)writeOptionsMask error:(NSError **)errorPtr;
- (NSRange)rangeOfData:(NSData *)dataToFind options:(NSDataSearchOptions)mask range:(NSRange)searchRange API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/*
 'block' is called once for each contiguous region of memory in the receiver (once total for contiguous NSDatas), until either all bytes have been enumerated, or the 'stop' parameter is set to YES.
 */
- (void) enumerateByteRangesUsingBlock:(void (NS_NOESCAPE ^)(const void *bytes, NSRange byteRange, BOOL *stop))block API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

@end

@interface NSData (NSDataCreation)

+ (instancetype)data;
+ (instancetype)dataWithBytes:(nullable const void *)bytes length:(NSUInteger)length;
+ (instancetype)dataWithBytesNoCopy:(void *)bytes length:(NSUInteger)length;
+ (instancetype)dataWithBytesNoCopy:(void *)bytes length:(NSUInteger)length freeWhenDone:(BOOL)b;
+ (nullable instancetype)dataWithContentsOfFile:(NSString *)path options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr;
+ (nullable instancetype)dataWithContentsOfURL:(NSURL *)url options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr;
+ (nullable instancetype)dataWithContentsOfFile:(NSString *)path;
+ (nullable instancetype)dataWithContentsOfURL:(NSURL *)url;
- (instancetype)initWithBytes:(nullable const void *)bytes length:(NSUInteger)length;
- (instancetype)initWithBytesNoCopy:(void *)bytes length:(NSUInteger)length;
- (instancetype)initWithBytesNoCopy:(void *)bytes length:(NSUInteger)length freeWhenDone:(BOOL)b;
- (instancetype)initWithBytesNoCopy:(void *)bytes length:(NSUInteger)length deallocator:(nullable void (^)(void *bytes, NSUInteger length))deallocator API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
- (nullable instancetype)initWithContentsOfFile:(NSString *)path options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr;
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url options:(NSDataReadingOptions)readOptionsMask error:(NSError **)errorPtr;
- (nullable instancetype)initWithContentsOfFile:(NSString *)path;
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url;
- (instancetype)initWithData:(NSData *)data;
+ (instancetype)dataWithData:(NSData *)data;

@end

@interface NSData (NSDataBase64Encoding)

/* Create an NSData from a Base-64 encoded NSString using the given options. By default, returns nil when the input is not recognized as valid Base-64.
*/
- (nullable instancetype)initWithBase64EncodedString:(NSString *)base64String options:(NSDataBase64DecodingOptions)options API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/* Create a Base-64 encoded NSString from the receiver's contents using the given options.
*/
- (NSString *)base64EncodedStringWithOptions:(NSDataBase64EncodingOptions)options API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/* Create an NSData from a Base-64, UTF-8 encoded NSData. By default, returns nil when the input is not recognized as valid Base-64.
*/
- (nullable instancetype)initWithBase64EncodedData:(NSData *)base64Data options:(NSDataBase64DecodingOptions)options API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/* Create a Base-64, UTF-8 encoded NSData from the receiver's contents using the given options.
*/
- (NSData *)base64EncodedDataWithOptions:(NSDataBase64EncodingOptions)options API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

@end

/* Various algorithms provided for compression APIs. See NSData and NSMutableData.
 */
typedef NS_ENUM(NSInteger, NSDataCompressionAlgorithm) {
    
    // LZFSE is the recommended compression algorithm if you don't have a specific reason to use another algorithm. Note that LZFSE is intended for use with Apple devices only. This algorithm generally compresses better than Zlib, but not as well as LZMA. It is generally slower than LZ4.
    NSDataCompressionAlgorithmLZFSE = 0,
    
    // LZ4 is appropriate if compression speed is critical. LZ4 generally sacrifices compression ratio in order to achieve its greater speed.
    // This implementation of LZ4 makes a small modification to the standard format, which is described in greater detail in <compression.h>.
    NSDataCompressionAlgorithmLZ4,
    
    // LZMA is appropriate if compression ratio is critical and memory usage and compression speed are not a factor. LZMA is an order of magnitude slower for both compression and decompression than other algorithms. It can also use a very large amount of memory, so if you need to compress large amounts of data on embedded devices with limited memory you should probably avoid LZMA.
    // Encoding uses LZMA level 6 only, but decompression works with any compression level.
    NSDataCompressionAlgorithmLZMA,
    
    // Zlib is appropriate if you want a good balance between compression speed and compression ratio, but only if you need interoperability with non-Apple platforms. Otherwise, LZFSE is generally a better choice than Zlib.
    // Encoding uses Zlib level 5 only, but decompression works with any compression level. It uses the raw DEFLATE format as described in IETF RFC 1951.
    NSDataCompressionAlgorithmZlib,
    
} API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

@interface NSData (NSDataCompression)

/* These methods return a compressed or decompressed version of the receiver using the specified algorithm.
 */
- (nullable instancetype)decompressedDataUsingAlgorithm:(NSDataCompressionAlgorithm)algorithm error:(NSError **)error API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));
- (nullable instancetype)compressedDataUsingAlgorithm:(NSDataCompressionAlgorithm)algorithm error:(NSError **)error API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

@end


@interface NSData (NSDeprecated)

- (void)getBytes:(void *)buffer API_DEPRECATED("This method is unsafe because it could potentially cause buffer overruns. Use -getBytes:length: instead.", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));
+ (nullable id)dataWithContentsOfMappedFile:(NSString *)path API_DEPRECATED("Use +dataWithContentsOfURL:options:error: and NSDataReadingMappedIfSafe or NSDataReadingMappedAlways instead.", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (nullable id)initWithContentsOfMappedFile:(NSString *)path  API_DEPRECATED("Use -initWithContentsOfURL:options:error: and NSDataReadingMappedIfSafe or NSDataReadingMappedAlways instead.", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));

/* These methods first appeared in NSData.h on OS X 10.9 and iOS 7.0. They are deprecated in the same releases in favor of the methods in the NSDataBase64Encoding category. However, these methods have existed for several releases, so they may be used for applications targeting releases prior to OS X 10.9 and iOS 7.0.
*/
- (nullable id)initWithBase64Encoding:(NSString *)base64String API_DEPRECATED("Use initWithBase64EncodedString:options: instead", macos(10.6,10.9), ios(4.0,7.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (NSString *)base64Encoding API_DEPRECATED("Use base64EncodedStringWithOptions: instead", macos(10.6,10.9), ios(4.0,7.0), watchos(2.0,2.0), tvos(9.0,9.0));

@end

/****************	Mutable Data		****************/

@interface NSMutableData : NSData

@property (readonly) void *mutableBytes NS_RETURNS_INNER_POINTER;
@property NSUInteger length;

@end

@interface NSMutableData (NSExtendedMutableData)

- (void)appendBytes:(const void *)bytes length:(NSUInteger)length;
- (void)appendData:(NSData *)other;
- (void)increaseLengthBy:(NSUInteger)extraLength;
- (void)replaceBytesInRange:(NSRange)range withBytes:(const void *)bytes;
- (void)resetBytesInRange:(NSRange)range;
- (void)setData:(NSData *)data;
- (void)replaceBytesInRange:(NSRange)range withBytes:(nullable const void *)replacementBytes length:(NSUInteger)replacementLength;

@end

@interface NSMutableData (NSMutableDataCreation)

+ (nullable instancetype)dataWithCapacity:(NSUInteger)aNumItems;
+ (nullable instancetype)dataWithLength:(NSUInteger)length;
- (nullable instancetype)initWithCapacity:(NSUInteger)capacity;
- (nullable instancetype)initWithLength:(NSUInteger)length;

@end

@interface NSMutableData (NSMutableDataCompression)

/* These methods compress or decompress the receiver's contents in-place using the specified algorithm. If the operation is not successful, these methods leave the receiver unchanged..
 */
- (BOOL)decompressUsingAlgorithm:(NSDataCompressionAlgorithm)algorithm error:(NSError **)error API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));
- (BOOL)compressUsingAlgorithm:(NSDataCompressionAlgorithm)algorithm error:(NSError **)error API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

@end

/****************	    Purgeable Data	****************/

API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSPurgeableData : NSMutableData <NSDiscardableContent> {
@private
    NSUInteger _length;
    int32_t _accessCount;
    uint8_t _private[32];
    void *_reserved;
}

@end

NS_ASSUME_NONNULL_END
