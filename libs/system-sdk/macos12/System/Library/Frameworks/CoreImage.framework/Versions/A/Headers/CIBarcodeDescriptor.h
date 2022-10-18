/* CoreImage - CIBarcodeDescriptor.h

   Copyright (c) 2017 Apple, Inc.
   All rights reserved. */

#import <CoreImage/CoreImageDefines.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 @class CIBarcodeDescriptor
 @abstract
    CIBarcodeDescriptor is an abstract base class that defines an abstract representation of a machine readable code's symbol attributes.
 
 @discsussion
    Each subclass is sufficient to recreate the symbol exactly as seen or to be used with a custom parser. Subclasses of CIBarcodeDescriptor are defined for each code type to contain the formal specification of each symbology.
 
 */
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIBarcodeDescriptor : NSObject <NSSecureCoding, NSCopying>
@end

/*!
 @enum CIQRCodeErrorCorrectionLevel
 @abstract
    Constants indicating the percentage of the symbol that is dedicated to error correction.
 
 @constant CIQRCodeErrorCorrectionLevelL
    Indicates that approximately 20% of the symbol data is dedicated to error correction.
 @constant CIQRCodeErrorCorrectionLevelM
    Indicates that approximately 37% of the symbol data is dedicated to error correction.
 @constant CIQRCodeErrorCorrectionLevelQ
    Indicates that approximately 55% of the symbol data is dedicated to error correction.
 @constant CIQRCodeErrorCorrectionLevelH
    Indicates that approximately 65% of the symbol data is dedicated to error correction.
 */
typedef CF_ENUM(NSInteger, CIQRCodeErrorCorrectionLevel)
{
    CIQRCodeErrorCorrectionLevelL NS_SWIFT_NAME(levelL) = 'L',
    CIQRCodeErrorCorrectionLevelM NS_SWIFT_NAME(levelM) = 'M',
    CIQRCodeErrorCorrectionLevelQ NS_SWIFT_NAME(levelQ) = 'Q',
    CIQRCodeErrorCorrectionLevelH NS_SWIFT_NAME(levelH) = 'H',

} NS_SWIFT_NAME(CIQRCodeDescriptor.ErrorCorrectionLevel);

/*!
 @class CIQRCodeDescriptor
 @abstract
    CIQRCodeDescriptor is a concrete subclass of CIBarcodeDescriptor that defines an abstract representation of a QR code symbol. 
 */
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIQRCodeDescriptor : CIBarcodeDescriptor
{
    NSData *errorCorrectedPayload;

    NSInteger symbolVersion;
    uint8_t maskPattern;
    CIQRCodeErrorCorrectionLevel errorCorrectionLevel;
}

/*!
 @property errorCorrectedPayload
 @abstract
 The error-corrected codewords that comprise the QR code symbol.
 
 @discussion
 QR Codes are formally specified in ISO/IEC 18004:2006(E). Section 6.4.10 "Bitstream to codeword conversion" specifies the set of 8-bit codewords in the symbol immediately prior to splitting the message into blocks and applying error correction.
 
 During decode, error correction is applied and if successful, the message is re-ordered to the state immediately following "Bitstream to codeword coversion." The errorCorrectedPayload corresponds to this sequence of 8-bit codewords.
 */
@property (readonly) NSData *errorCorrectedPayload;

/*!
 @property symbolVersion
 @abstract
    The version property corresponds to the size of the QR Code.

 @discussion
    QR Codes are square. ISO/IEC 18004 defines versions from 1 to 40, where a higher symbol version indicates a larger data carrying capacity. This field is required in order to properly interpret the error corrected payload.
 */
@property (readonly) NSInteger symbolVersion;

/*!
 @property maskPattern
 @abstract
    The data mask pattern for the QR code symbol.
 
 @discussion
    QR Codes support eight data mask patterns, which are used to avoid large black or large white areas inside the symbol body. Valid values range from 0 to 7.
 */
@property (readonly) uint8_t maskPattern;

/*!
 @property errorCorrectionLevel
 @abstract
    The error correction level of the QR code.
 
 @discussion
    QR Codes support four levels of Reed-Solomon error correction, in increasing error correction capability: L, M, Q, and H.
 */
@property (readonly) CIQRCodeErrorCorrectionLevel errorCorrectionLevel;

// Initializes a descriptor that can be used as input to CIBarcodeGenerator
- (nullable instancetype)initWithPayload:(NSData *)errorCorrectedPayload
                           symbolVersion:(NSInteger)symbolVersion
                             maskPattern:(uint8_t)maskPattern
                    errorCorrectionLevel:(CIQRCodeErrorCorrectionLevel)errorCorrectionLevel;

// Construct an autoreleased descriptor that can be used as input to CIBarcodeGenerator
+ (nullable instancetype)descriptorWithPayload:(NSData *)errorCorrectedPayload
                                 symbolVersion:(NSInteger)symbolVersion
                                   maskPattern:(uint8_t)maskPattern
                          errorCorrectionLevel:(CIQRCodeErrorCorrectionLevel)errorCorrectionLevel;

@end


/*!
 @class CIAztecCodeDescriptor
 @abstract
    CIAztecCodeDescriptor is a concrete subclass of CIBarcodeDescriptor that defines an abstract representation of an Aztec Code symbol.
 
 @discussion
    CIAztecCodeDescriptor may not be instantiated directly.
 */
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIAztecCodeDescriptor : CIBarcodeDescriptor
{
    NSData *errorCorrectedPayload;

    BOOL isCompact;
    NSInteger layerCount;
    NSInteger dataCodewordCount;
}

/*!
 @property errorCorrectedPayload
 @abstract
 The error-corrected codewords that comprise the Aztec code symbol.
 
 @discussion
 Aztec Codes are formally specified in ISO/IEC 24778:2008(E).
 
 The error corrected payload consists of the 6-, 8-, 10-, or 12-bit message codewords produced at the end of the step described in section 7.3.1.2 "Formation of data codewords", which exists immediately prior to adding error correction. These codewords have dummy bits inserted to ensure that an entire codeword isn't all 0's or all 1's. Clients will need to remove these extra bits as part of interpreting the payload.
 */
@property (readonly) NSData *errorCorrectedPayload;

/*!
 @property isCompact
 @abstract
    A BOOL indicating whether the symbol is compact.
 
 @discussion
    Compact Aztec symbols use one-fewer ring in the central finder pattern than full-range Aztec symbols of the same number of data layers.
 */
@property (readonly) BOOL isCompact;

/*!
 @property layerCount
 @abstract
    The number of data layers in the Aztec code symbol.
 
 @discussion
    Combined with the isCompact property, the number of data layers determines the number of modules in the Aztec Code symbol. Valid values range from 1 to 32. Compact symbols can have up to 4 data layers.
 
    The number of data layers also determines the number of bits in each data codeword of the message carried by the Aztec Code symbol.
 */
@property (readonly) NSInteger layerCount;

/*!
 @property dataCodewordCount
 @abstract
    The number of non-error-correction codewords carried by the Aztec code symbol.
 
 @discussion
    Used to determine the level of error correction in conjunction with the number of data layers. Valid values are 1...2048. Compact symbols can have up to 64 message codewords.
 
    Note that this value can exceed the number of message codewords allowed by the number of data layers in this symbol. In this case, the actual number of message codewords is 1024 fewer than this value and the message payload is to be interpreted in an application-defined manner.
 */
@property (readonly) NSInteger dataCodewordCount;

// Initializes a descriptor that can be used as input to CIBarcodeGenerator
- (nullable instancetype)initWithPayload:(NSData *)errorCorrectedPayload
                               isCompact:(BOOL)isCompact
                              layerCount:(NSInteger)layerCount
                       dataCodewordCount:(NSInteger)dataCodewordCount;

// Construct an autoreleased descriptor that can be used as input to CIBarcodeGenerator
+ (nullable instancetype)descriptorWithPayload:(NSData *)errorCorrectedPayload
                                     isCompact:(BOOL)isCompact
                                    layerCount:(NSInteger)layerCount
                             dataCodewordCount:(NSInteger)dataCodewordCount;

@end

/*!
 @class CIPDF417CodeDescriptor
 @abstract
    CIPDF417CodeDescriptor is a concrete subclass of CIBarcodeDescriptor that defines an abstract representation of a PDF417 code symbol.
 
 @discussion
    Refer to the ISO/IEC 15438:2006(E) for the PDF417 symbol specification.
 
 */
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIPDF417CodeDescriptor : CIBarcodeDescriptor
{   
    NSData *errorCorrectedPayload;
    
    BOOL isCompact;
    NSInteger rowCount;
    NSInteger columnCount;
}

/*!
 @property errorCorrectedPayload
 @abstract
 The error-corrected codewords which comprise the PDF417 symbol. The first codeword indicates the number of data codewords in the errorCorrectedPayload.
 
 @discussion
 PDF417 codes are comprised of a start character on the left and a stop character on the right. Each row begins and ends with special characters indicating the current row as well as information about the dimensions of the PDF417 symbol. The errorCorrectedPayload represents the sequence of PDF417 codewords that make up the body of the message. The first codeword indicates the number of codewords in the message. This count includes the "count" codeword and any padding codewords, but does not include the error correction codewords. Each codeword is a 16-bit value in the range of 0...928. The sequence is to be interpreted as described in the PDF417 bar code symbology specification -- ISO/IEC 15438:2006(E).
 */
@property(readonly) NSData *errorCorrectedPayload;

/*!
 @property isCompact
 @abstract
    A BOOL indicating whether the symbol is compact.
 
 @discussion
    Compact PDF417 symbols have abbreviated right-side guard bars.
 */
@property (readonly) BOOL isCompact;

/*!
 @property rowCount
 @abstract
    Indicates the number of rows in the rectangular matrix.
 
 @discussion
    rowCount values range from 3 to 90.
 */
@property (readonly) NSInteger rowCount;

/*!
 @property columnCount
 @abstract
    Indicates the number of columns in the rectangular matrix, excluding the columns used to indicate the symbol structure.
 
 @discussion
    columnCount values range from 1 to 30.
 */
@property (readonly) NSInteger columnCount;

// Initializes a descriptor that can be used as input to CIBarcodeGenerator
- (nullable instancetype)initWithPayload:(NSData *)errorCorrectedPayload
                               isCompact:(BOOL)isCompact
                                rowCount:(NSInteger)rowCount
                             columnCount:(NSInteger)columnCount;

// Construct an autoreleased descriptor that can be used as input to CIBarcodeGenerator
+ (nullable instancetype)descriptorWithPayload:(NSData *)errorCorrectedPayload
                                     isCompact:(BOOL)isCompact
                                      rowCount:(NSInteger)rowCount
                                   columnCount:(NSInteger)columnCount;

@end


/*!
 @enum CIDataMatrixCodeECCVersion
 @abstract
    Constants indicating the Data Matrix code ECC version.
 
 @discussion
    ECC 000 - 140 symbols offer five levels of error correction using convolutional code error correction. Each successive level of error correction offers more protection for the message data and increases the size of the symbol required to carry a specific message. ECC 000 symbols offer no data protection. The other modes are described in ISO/IEC 16022:2006 and enumerated in this list only for completeness.
 
    ECC 200 symbols utilize Reed-Solomon error correction. The error correction capacity for any given Data Matrix symbol is fixed by the size (in rows and columns) of the symbol. See Table 7 of ISO/IEC 16022:2006(E) for more details.
 
 @constant CIDataMatrixCodeECCVersion000
    Indicates error correction using convolutional code error correction with no data protection.
 @constant CIDataMatrixCodeECCVersion050
    Indicates 1/4 of the symbol is dedicated to convolutional code error correction.
 @constant CIDataMatrixCodeECCVersion080
    Indicates 1/3 of the symbol is dedicated to convolutional code error correction.
 @constant CIDataMatrixCodeECCVersion100
    Indicates 1/2 of the symbol is dedicated to convolutional code error correction.
 @constant CIDataMatrixCodeECCVersion140
    Indicates 3/4 of the symbol is dedicated to convolutional code error correction.
 @constant CIDataMatrixCodeECCVersion200
    Indicates error correction using Reed-Solomon error correction. Data protection overhead varies based on symbol size.
 */
typedef CF_ENUM(NSInteger, CIDataMatrixCodeECCVersion)
{
    CIDataMatrixCodeECCVersion000 NS_SWIFT_NAME(v000) =   0,
    CIDataMatrixCodeECCVersion050 NS_SWIFT_NAME(v050) =  50,
    CIDataMatrixCodeECCVersion080 NS_SWIFT_NAME(v080) =  80,
    CIDataMatrixCodeECCVersion100 NS_SWIFT_NAME(v100) = 100,
    CIDataMatrixCodeECCVersion140 NS_SWIFT_NAME(v140) = 140,
    CIDataMatrixCodeECCVersion200 NS_SWIFT_NAME(v200) = 200,

} NS_SWIFT_NAME(CIDataMatrixCodeDescriptor.ECCVersion);

/*!
 @class CIDataMatrixCodeDescriptor
 @abstract
    CIDataMatrixCodeDescriptor is a concrete subclass of CIBarcodeDescriptor that defines an abstract representation of a Data Matrix code symbol.

 */
NS_CLASS_AVAILABLE(10_13, 11_0)
@interface CIDataMatrixCodeDescriptor : CIBarcodeDescriptor
{
    NSData *errorCorrectedPayload;

    NSInteger rowCount;
    NSInteger columnCount;
    CIDataMatrixCodeECCVersion eccVersion;
}

/*!
 @property errorCorrectedPayload
 @abstract
 The error corrected payload that comprise the Data Matrix code symbol.
 
 @discussion
 DataMatrix symbols are specified bn ISO/IEC 16022:2006(E). ECC 200-type symbols will always have an even number of rows and columns.
 
 For ECC 200-type symbols, the phases of encoding data into a symbol are described in section 5.1 -- Encode procedure overview. The error corrected payload comprises the de-interleaved bits of the message described at the end of Step 1: Data encodation.
 */
@property (readonly) NSData *errorCorrectedPayload;

/*!
 @property rowCount
 @abstract
    The number of module rows.
 
 @discussion
    Refer to ISO/IEC 16022:2006(E) for valid module row and column count combinations.
 */
@property (readonly) NSInteger rowCount;

/*!
 @property columnCount
 @abstract
    The number of module columns.
 
 @discussion
    Refer to ISO/IEC 16022:2006(E) for valid module row and column count combinations.
 */
@property (readonly) NSInteger columnCount;

/*!
 @property eccVersion
 @abstract
    The Data Matrix code ECC version.

 @discussion
    Valid values are 000, 050, 080, 100, 140, and 200. Any symbol with an even number of rows and columns will be ECC 200.
 */
@property (readonly) CIDataMatrixCodeECCVersion eccVersion;

// Initializes a descriptor that can be used as input to CIBarcodeGenerator
- (nullable instancetype)initWithPayload:(NSData *)errorCorrectedPayload
                                rowCount:(NSInteger)rowCount
                             columnCount:(NSInteger)columnCount
                              eccVersion:(CIDataMatrixCodeECCVersion)eccVersion;

// Construct an autoreleased descriptor that can be used as input to CIBarcodeGenerator
+ (nullable instancetype)descriptorWithPayload:(NSData *)errorCorrectedPayload
                                      rowCount:(NSInteger)rowCount
                                   columnCount:(NSInteger)columnCount
                                    eccVersion:(CIDataMatrixCodeECCVersion)eccVersion;

@end

/*!
 @extension NSUserActivity
 @abstract
    A NSUserActivity extension for passing detected barcodes.
 */

#if __OBJC2__

@class NSUserActivity;

@interface NSUserActivity (CIBarcodeDescriptor)

/*! 
 @abstract The scanned code in the user activity passed in by system scanner.
 */
@property (nonatomic, nullable, readonly, copy) CIBarcodeDescriptor *detectedBarcodeDescriptor API_AVAILABLE(macos(10.13.4), ios(11.3), tvos(11.3));

@end

#endif // __OBJC2__

NS_ASSUME_NONNULL_END
