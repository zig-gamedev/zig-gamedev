/*	NSArchiver.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSCoder.h>
#import <Foundation/NSException.h>

@class NSString, NSData, NSMutableData, NSMutableDictionary, NSMutableArray;

NS_ASSUME_NONNULL_BEGIN

/************		Archiving: Writing	****************/

API_DEPRECATED("Use NSKeyedArchiver instead", macos(10.0,10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
@interface NSArchiver : NSCoder {
@private
    void		*mdata;
    void		*pointerTable;
    void		*stringTable;
    void		*ids;
    void		*map;
    void		*replacementTable;
    void		*reserved;
}

- (instancetype)initForWritingWithMutableData:(NSMutableData *)mdata NS_DESIGNATED_INITIALIZER;
    
@property (readonly, retain) NSMutableData *archiverData;

- (void)encodeRootObject:(id)rootObject;
- (void)encodeConditionalObject:(nullable id)object;

+ (NSData *)archivedDataWithRootObject:(id)rootObject;

+ (BOOL)archiveRootObject:(id)rootObject toFile:(NSString *)path;

- (void)encodeClassName:(NSString *)trueName intoClassName:(NSString *)inArchiveName;
- (nullable NSString *)classNameEncodedForTrueClassName:(NSString *)trueName;

- (void)replaceObject:(id)object withObject:(id)newObject;

@end

/************		Archiving: Reading		****************/

API_DEPRECATED("Use NSKeyedUnarchiver instead", macos(10.0,10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
@interface NSUnarchiver : NSCoder {
@private
    void *		datax;
    NSUInteger		cursor;
    NSZone              *objectZone;
    NSUInteger		systemVersion;
    signed char		streamerVersion;
    char		swap;
    char		unused1;
    char		unused2;
    void		*pointerTable;
    void		*stringTable;
    id			classVersions;
    NSInteger			lastLabel;
     void		*map;
    void		*allUnarchivedObjects;
    id			reserved;
}

- (nullable instancetype)initForReadingWithData:(NSData *)data NS_DESIGNATED_INITIALIZER;

- (void)setObjectZone:(nullable NSZone *)zone NS_AUTOMATED_REFCOUNT_UNAVAILABLE;
- (nullable NSZone *)objectZone NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

@property (getter=isAtEnd, readonly) BOOL atEnd;

@property (readonly) unsigned int systemVersion;

+ (nullable id)unarchiveObjectWithData:(NSData *)data;
+ (nullable id)unarchiveObjectWithFile:(NSString *)path;

+ (void)decodeClassName:(NSString *)inArchiveName asClassName:(NSString *)trueName;

- (void)decodeClassName:(NSString *)inArchiveName asClassName:(NSString *)trueName;

+ (NSString *)classNameDecodedForArchiveClassName:(NSString *)inArchiveName;
- (NSString *)classNameDecodedForArchiveClassName:(NSString *)inArchiveName;

- (void)replaceObject:(id)object withObject:(id)newObject;

@end

/************		Object call back		****************/

@interface NSObject (NSArchiverCallback)

@property (nullable, readonly) Class classForArchiver;

- (nullable id)replacementObjectForArchiver:(NSArchiver *)archiver API_DEPRECATED_WITH_REPLACEMENT("replacementObjectForCoder:", macos(10.0,10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0));

@end

NS_ASSUME_NONNULL_END
