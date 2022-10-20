/*!
	@file		CoreAudioBaseTypes.h
	@framework	CoreAudioTypes.framework
	@copyright	(c) 1985-2021 by Apple, Inc., all rights reserved.
    @abstract   Definition of types common to the Core Audio APIs.
*/

#ifndef CoreAudioTypes_CoreAudioBaseTypes_h
#define CoreAudioTypes_CoreAudioBaseTypes_h

//==================================================================================================
#pragma mark -
#pragma mark Includes

#define COREAUDIOTYPES_VERSION 20210401

#include <TargetConditionals.h>
#include <CoreFoundation/CFBase.h>

// CA_PREFER_FIXED_POINT is true on iOS, but the concept is no longer true. This symbol WILL
// be removed in a future release. (Only deprecated symbols depend on it.)
#if !defined(CA_PREFER_FIXED_POINT)
	#if TARGET_OS_IPHONE && !TARGET_OS_MACCATALYST
		#define CA_PREFER_FIXED_POINT 1
	#endif
#endif

#if defined(__has_feature) && __has_feature(attribute_deprecated_with_message)
    #define CA_CANONICAL_DEPRECATED __attribute__((deprecated("The concept of canonical formats is deprecated")))
#else
    #define CA_CANONICAL_DEPRECATED
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"

#if defined(__cplusplus)
    #include <string.h>
#endif

//==================================================================================================

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
#pragma mark -
#pragma mark General Error Codes

/*!
    @enum           General Audio error codes
    @abstract       These are the error codes returned from the APIs found through Core Audio related frameworks.
    @constant       kAudio_UnimplementedError
                        Unimplemented core routine.
    @constant       kAudio_FileNotFoundError
                        File not found.
    @constant       kAudio_FilePermissionError
                        File cannot be opened due to either file, directory, or sandbox permissions.
    @constant       kAudio_TooManyFilesOpenError
                        File cannot be opened because too many files are already open.
    @constant       kAudio_BadFilePathError
                        File cannot be opened because the specified path is malformed.
    @constant       kAudio_ParamError
                        Error in user parameter list.
    @constant       kAudio_MemFullError
                        Not enough room in heap zone.
*/

CF_ENUM(OSStatus)
{
    kAudio_UnimplementedError     = -4,
    kAudio_FileNotFoundError      = -43,
    kAudio_FilePermissionError    = -54,
    kAudio_TooManyFilesOpenError  = -42,
    kAudio_BadFilePathError       = '!pth', // 0x21707468, 561017960
    kAudio_ParamError             = -50,
    kAudio_MemFullError           = -108
};
	
//==================================================================================================
#pragma mark -
#pragma mark AudioValueRange

/*!
    @struct         AudioValueRange
    @abstract       This structure holds a pair of numbers that represent a continuous range of
                    values.
    @var            mMinimum
                        The minimum value.
    @var            mMaximum
                        The maximum value.
*/
struct AudioValueRange
{
    Float64 mMinimum;
    Float64 mMaximum;
};
typedef struct AudioValueRange  AudioValueRange;

//==================================================================================================
#pragma mark -
#pragma mark AudioValueTranslation

/*!
    @struct         AudioValueTranslation
    @abstract       This stucture holds the buffers necessary for translation operations.
    @var            mInputData
                        The buffer containing the data to be translated.
    @var            mInputDataSize
                        The number of bytes in the buffer pointed at by mInputData.
    @var            mOutputData
                        The buffer to hold the result of the translation.
    @var            mOutputDataSize
                        The number of bytes in the buffer pointed at by mOutputData.
*/
struct AudioValueTranslation
{
    void* __nonnull mInputData;
    UInt32          mInputDataSize;
    void* __nonnull mOutputData;
    UInt32          mOutputDataSize;
};
typedef struct AudioValueTranslation    AudioValueTranslation;

//==================================================================================================
#pragma mark -
#pragma mark AudioBuffer/AudioBufferList

/*!
    @struct         AudioBuffer
    @abstract       A structure to hold a buffer of audio data.
    @var            mNumberChannels
                        The number of interleaved channels in the buffer.
    @var            mDataByteSize
                        The number of bytes in the buffer pointed at by mData.
    @var            mData
                        A pointer to the buffer of audio data.
*/
struct AudioBuffer
{
    UInt32              mNumberChannels;
    UInt32              mDataByteSize;
    void* __nullable    mData;
};
typedef struct AudioBuffer  AudioBuffer;

/*!
    @struct         AudioBufferList
    @abstract       A variable length array of AudioBuffer structures.
    @var            mNumberBuffers
                        The number of AudioBuffers in the mBuffers array.
    @var            mBuffers
                        A variable length array of AudioBuffers.
*/
struct AudioBufferList
{
    UInt32      mNumberBuffers;
    AudioBuffer mBuffers[1]; // this is a variable length array of mNumberBuffers elements
	
#if defined(__cplusplus) && defined(CA_STRICT) && CA_STRICT
public:
    AudioBufferList() {}
private:
    //  Copying and assigning a variable length struct is problematic; generate a compile error.
    AudioBufferList(const AudioBufferList&);
    AudioBufferList&    operator=(const AudioBufferList&);
#endif

};
typedef struct AudioBufferList  AudioBufferList;

//==================================================================================================
#pragma mark -
#pragma mark Audio Formats

/*!
    @typedef        AudioSampleType
    @abstract       The canonical audio sample type used by the various CoreAudio APIs
    @discussion
        These types are deprecated. Code performing signal processing should use concrete types
        (e.g. float, Float32, SInt16, SInt32). Format-agnostic code, instead of relying on the sizes
        of these types, should calculate the size of a sample from an AudioStreamBasicDescription's
        mBytesPerChannel, mChannelsPerFrame, and (mFlags & kLinearPCMFormatFlagIsNonInterleaved).
        For interleaved formats, the size of a sample is mBytesPerFrame / mChannelsPerFrame.
        For non-interleaved formats, it is simply mBytesPerFrame.
*/
#if !defined(CA_PREFER_FIXED_POINT) || !CA_PREFER_FIXED_POINT
CA_CANONICAL_DEPRECATED typedef Float32     AudioSampleType;
CA_CANONICAL_DEPRECATED typedef Float32     AudioUnitSampleType;
#else
CA_CANONICAL_DEPRECATED typedef SInt16      AudioSampleType;
CA_CANONICAL_DEPRECATED typedef SInt32      AudioUnitSampleType;
#define kAudioUnitSampleFractionBits 24
#endif

/*!
    @typedef        AudioFormatID
    @abstract       A four char code indicating the general kind of data in the stream.
*/
typedef UInt32  AudioFormatID;
	
/*!
    @typedef        AudioFormatFlags
    @abstract       Flags that are specific to each format.
*/
typedef UInt32  AudioFormatFlags;
	
/*!
    @struct         AudioStreamBasicDescription
    @abstract       This structure encapsulates all the information for describing the basic
                    format properties of a stream of audio data.
    @discussion     This structure is sufficient to describe any constant bit rate format that  has
                    channels that are the same size. Extensions are required for variable bit rate
                    data and for constant bit rate data where the channels have unequal sizes.
                    However, where applicable, the appropriate fields will be filled out correctly
                    for these kinds of formats (the extra data is provided via separate properties).
                    In all fields, a value of 0 indicates that the field is either unknown, not
                    applicable or otherwise is inapproprate for the format and should be ignored.
                    Note that 0 is still a valid value for most formats in the mFormatFlags field.

                    In audio data a frame is one sample across all channels. In non-interleaved
                    audio, the per frame fields identify one channel. In interleaved audio, the per
                    frame fields identify the set of n channels. In uncompressed audio, a Packet is
                    one frame, (mFramesPerPacket == 1). In compressed audio, a Packet is an
                    indivisible chunk of compressed data, for example an AAC packet will contain
                    1024 sample frames.
 
    @var            mSampleRate
                        The number of sample frames per second of the data in the stream.
    @var            mFormatID
                        The AudioFormatID indicating the general kind of data in the stream.
    @var            mFormatFlags
                        The AudioFormatFlags for the format indicated by mFormatID.
    @var            mBytesPerPacket
                        The number of bytes in a packet of data.
    @var            mFramesPerPacket
                        The number of sample frames in each packet of data.
    @var            mBytesPerFrame
                        The number of bytes in a single sample frame of data.
    @var            mChannelsPerFrame
                        The number of channels in each frame of data.
    @var            mBitsPerChannel
                        The number of bits of sample data for each channel in a frame of data.
    @var            mReserved
                        Pads the structure out to force an even 8 byte alignment.
*/
struct AudioStreamBasicDescription
{
    Float64             mSampleRate;
    AudioFormatID       mFormatID;
    AudioFormatFlags    mFormatFlags;
    UInt32              mBytesPerPacket;
    UInt32              mFramesPerPacket;
    UInt32              mBytesPerFrame;
    UInt32              mChannelsPerFrame;
    UInt32              mBitsPerChannel;
    UInt32              mReserved;
};
typedef struct AudioStreamBasicDescription  AudioStreamBasicDescription;

/*!
    @constant       kAudioStreamAnyRate
                        The format can use any sample rate. Note that this constant can only appear
                        in listings of supported formats. It will never appear in a current format.
*/
static const Float64    kAudioStreamAnyRate = 0.0;

/*!
    @enum           Format IDs
    @abstract       The AudioFormatIDs used to identify individual formats of audio data.
    @constant       kAudioFormatLinearPCM
                        Linear PCM, uses the standard flags.
    @constant       kAudioFormatAC3
                        AC-3, has no flags.
    @constant       kAudioFormat60958AC3
                        AC-3 packaged for transport over an IEC 60958 compliant digital audio
                        interface. Uses the standard flags.
    @constant       kAudioFormatAppleIMA4
                        Apples implementation of IMA 4:1 ADPCM, has no flags.
    @constant       kAudioFormatMPEG4AAC
                        MPEG-4 Low Complexity AAC audio object, has no flags.
    @constant       kAudioFormatMPEG4CELP
                        MPEG-4 CELP audio object, has no flags.
    @constant       kAudioFormatMPEG4HVXC
                        MPEG-4 HVXC audio object, has no flags.
    @constant       kAudioFormatMPEG4TwinVQ
                        MPEG-4 TwinVQ audio object type, has no flags.
    @constant       kAudioFormatMACE3
                        MACE 3:1, has no flags.
    @constant       kAudioFormatMACE6
                        MACE 6:1, has no flags.
    @constant       kAudioFormatULaw
                        µLaw 2:1, has no flags.
    @constant       kAudioFormatALaw
                        aLaw 2:1, has no flags.
    @constant       kAudioFormatQDesign
                        QDesign music, has no flags
    @constant       kAudioFormatQDesign2
                        QDesign2 music, has no flags
    @constant       kAudioFormatQUALCOMM
                        QUALCOMM PureVoice, has no flags
    @constant       kAudioFormatMPEGLayer1
                        MPEG-1/2, Layer 1 audio, has no flags
    @constant       kAudioFormatMPEGLayer2
                        MPEG-1/2, Layer 2 audio, has no flags
    @constant       kAudioFormatMPEGLayer3
                        MPEG-1/2, Layer 3 audio, has no flags
    @constant       kAudioFormatTimeCode
                        A stream of IOAudioTimeStamps, uses the IOAudioTimeStamp flags (see
                        IOKit/audio/IOAudioTypes.h).
    @constant       kAudioFormatMIDIStream
                        A stream of MIDIPacketLists where the time stamps in the MIDIPacketList are
                        sample offsets in the stream. The mSampleRate field is used to describe how
                        time is passed in this kind of stream and an AudioUnit that receives or
                        generates this stream can use this sample rate, the number of frames it is
                        rendering and the sample offsets within the MIDIPacketList to define the
                        time for any MIDI event within this list. It has no flags.
    @constant       kAudioFormatParameterValueStream
                        A "side-chain" of Float32 data that can be fed or generated by an AudioUnit
                        and is used to send a high density of parameter value control information.
                        An AU will typically run a ParameterValueStream at either the sample rate of
                        the AudioUnit's audio data, or some integer divisor of this (say a half or a
                        third of the sample rate of the audio). The Sample Rate of the ASBD
                        describes this relationship. It has no flags.
    @constant       kAudioFormatAppleLossless
                        Apple Lossless, the flags indicate the bit depth of the source material.
    @constant       kAudioFormatMPEG4AAC_HE
                        MPEG-4 High Efficiency AAC audio object, has no flags.
    @constant       kAudioFormatMPEG4AAC_LD
                        MPEG-4 AAC Low Delay audio object, has no flags.
    @constant       kAudioFormatMPEG4AAC_ELD
                        MPEG-4 AAC Enhanced Low Delay audio object, has no flags. This is the formatID of
                        the base layer without the SBR extension. See also kAudioFormatMPEG4AAC_ELD_SBR
    @constant       kAudioFormatMPEG4AAC_ELD_SBR
                        MPEG-4 AAC Enhanced Low Delay audio object with SBR extension layer, has no flags.
    @constant       kAudioFormatMPEG4AAC_HE_V2
                        MPEG-4 High Efficiency AAC Version 2 audio object, has no flags.
    @constant       kAudioFormatMPEG4AAC_Spatial
                        MPEG-4 Spatial Audio audio object, has no flags.
    @constant       kAudioFormatMPEGD_USAC
                        MPEG-D Unified Speech and Audio Coding, has no flags.
    @constant       kAudioFormatAMR
                        The AMR Narrow Band speech codec.
    @constant       kAudioFormatAMR_WB
                        The AMR Wide Band speech codec.
    @constant       kAudioFormatAudible
                        The format used for Audible audio books. It has no flags.
    @constant       kAudioFormatiLBC
                        The iLBC narrow band speech codec. It has no flags.
    @constant       kAudioFormatDVIIntelIMA
                        DVI/Intel IMA ADPCM - ACM code 17.
    @constant       kAudioFormatMicrosoftGSM
                        Microsoft GSM 6.10 - ACM code 49.
    @constant       kAudioFormatAES3
                        This format is defined by AES3-2003, and adopted into MXF and MPEG-2
                        containers and SDTI transport streams with SMPTE specs 302M-2002 and
                        331M-2000. It has no flags.
    @constant       kAudioFormatEnhancedAC3
                        Enhanced AC-3, has no flags.
    @constant       kAudioFormatFLAC
                        Free Lossless Audio Codec, the flags indicate the bit depth of the source material.
    @constant       kAudioFormatOpus
                        Opus codec, has no flags.
*/
CF_ENUM(AudioFormatID)
{
    kAudioFormatLinearPCM               = 'lpcm',
    kAudioFormatAC3                     = 'ac-3',
    kAudioFormat60958AC3                = 'cac3',
    kAudioFormatAppleIMA4               = 'ima4',
    kAudioFormatMPEG4AAC                = 'aac ',
    kAudioFormatMPEG4CELP               = 'celp',
    kAudioFormatMPEG4HVXC               = 'hvxc',
    kAudioFormatMPEG4TwinVQ             = 'twvq',
    kAudioFormatMACE3                   = 'MAC3',
    kAudioFormatMACE6                   = 'MAC6',
    kAudioFormatULaw                    = 'ulaw',
    kAudioFormatALaw                    = 'alaw',
    kAudioFormatQDesign                 = 'QDMC',
    kAudioFormatQDesign2                = 'QDM2',
    kAudioFormatQUALCOMM                = 'Qclp',
    kAudioFormatMPEGLayer1              = '.mp1',
    kAudioFormatMPEGLayer2              = '.mp2',
    kAudioFormatMPEGLayer3              = '.mp3',
    kAudioFormatTimeCode                = 'time',
    kAudioFormatMIDIStream              = 'midi',
    kAudioFormatParameterValueStream    = 'apvs',
    kAudioFormatAppleLossless           = 'alac',
    kAudioFormatMPEG4AAC_HE             = 'aach',
    kAudioFormatMPEG4AAC_LD             = 'aacl',
    kAudioFormatMPEG4AAC_ELD            = 'aace',
    kAudioFormatMPEG4AAC_ELD_SBR        = 'aacf',
    kAudioFormatMPEG4AAC_ELD_V2         = 'aacg',
    kAudioFormatMPEG4AAC_HE_V2          = 'aacp',
    kAudioFormatMPEG4AAC_Spatial        = 'aacs',
    kAudioFormatMPEGD_USAC              = 'usac',
    kAudioFormatAMR                     = 'samr',
    kAudioFormatAMR_WB                  = 'sawb',
    kAudioFormatAudible                 = 'AUDB',
    kAudioFormatiLBC                    = 'ilbc',
    kAudioFormatDVIIntelIMA             = 0x6D730011,
    kAudioFormatMicrosoftGSM            = 0x6D730031,
    kAudioFormatAES3                    = 'aes3',
    kAudioFormatEnhancedAC3             = 'ec-3',
    kAudioFormatFLAC                    = 'flac',
    kAudioFormatOpus                    = 'opus'
};

/*!
    @enum           Standard AudioFormatFlags Values for AudioStreamBasicDescription
    @abstract       These are the standard AudioFormatFlags for use in the mFormatFlags field of the
                    AudioStreamBasicDescription structure.
    @discussion     Typically, when an ASBD is being used, the fields describe the complete layout
                    of the sample data in the buffers that are represented by this description -
                    where typically those buffers are represented by an AudioBuffer that is
                    contained in an AudioBufferList.

                    However, when an ASBD has the kAudioFormatFlagIsNonInterleaved flag, the
                    AudioBufferList has a different structure and semantic. In this case, the ASBD
                    fields will describe the format of ONE of the AudioBuffers that are contained in
                    the list, AND each AudioBuffer in the list is determined to have a single (mono)
                    channel of audio data. Then, the ASBD's mChannelsPerFrame will indicate the
                    total number of AudioBuffers that are contained within the AudioBufferList -
                    where each buffer contains one channel. This is used primarily with the
                    AudioUnit (and AudioConverter) representation of this list - and won't be found
                    in the AudioHardware usage of this structure.
    @constant       kAudioFormatFlagIsFloat
                        Set for floating point, clear for integer.
    @constant       kAudioFormatFlagIsBigEndian
                        Set for big endian, clear for little endian.
    @constant       kAudioFormatFlagIsSignedInteger
                        Set for signed integer, clear for unsigned integer. This is only valid if
                        kAudioFormatFlagIsFloat is clear.
    @constant       kAudioFormatFlagIsPacked
                        Set if the sample bits occupy the entire available bits for the channel,
                        clear if they are high or low aligned within the channel. Note that even if
                        this flag is clear, it is implied that this flag is set if the
                        AudioStreamBasicDescription is filled out such that the fields have the
                        following relationship:
                           ((mBitsPerSample / 8) * mChannelsPerFrame) == mBytesPerFrame
    @constant       kAudioFormatFlagIsAlignedHigh
                        Set if the sample bits are placed into the high bits of the channel, clear
                        for low bit placement. This is only valid if kAudioFormatFlagIsPacked is
                        clear.
    @constant       kAudioFormatFlagIsNonInterleaved
                        Set if the samples for each channel are located contiguously and the
                        channels are layed out end to end, clear if the samples for each frame are
                        layed out contiguously and the frames layed out end to end.
    @constant       kAudioFormatFlagIsNonMixable
                        Set to indicate when a format is non-mixable. Note that this flag is only
                        used when interacting with the HAL's stream format information. It is not a
                        valid flag for any other uses.
    @constant       kAudioFormatFlagsAreAllClear
                        Set if all the flags would be clear in order to preserve 0 as the wild card
                        value.
    @constant       kLinearPCMFormatFlagIsFloat
                        Synonym for kAudioFormatFlagIsFloat.
    @constant       kLinearPCMFormatFlagIsBigEndian
                        Synonym for kAudioFormatFlagIsBigEndian.
    @constant       kLinearPCMFormatFlagIsSignedInteger
                        Synonym for kAudioFormatFlagIsSignedInteger.
    @constant       kLinearPCMFormatFlagIsPacked
                        Synonym for kAudioFormatFlagIsPacked.
    @constant       kLinearPCMFormatFlagIsAlignedHigh
                        Synonym for kAudioFormatFlagIsAlignedHigh.
    @constant       kLinearPCMFormatFlagIsNonInterleaved
                        Synonym for kAudioFormatFlagIsNonInterleaved.
    @constant       kLinearPCMFormatFlagIsNonMixable
                        Synonym for kAudioFormatFlagIsNonMixable.
    @constant       kLinearPCMFormatFlagsAreAllClear
                        Synonym for kAudioFormatFlagsAreAllClear.
    @constant       kLinearPCMFormatFlagsSampleFractionShift
                        The linear PCM flags contain a 6-bit bitfield indicating that an integer
                        format is to be interpreted as fixed point. The value indicates the number
                        of bits are used to represent the fractional portion of each sample value.
                        This constant indicates the bit position (counting from the right) of the
                        bitfield in mFormatFlags.
    @constant       kLinearPCMFormatFlagsSampleFractionMask
                        number_fractional_bits = (mFormatFlags &
                        kLinearPCMFormatFlagsSampleFractionMask) >>
                        kLinearPCMFormatFlagsSampleFractionShift
    @constant       kAppleLosslessFormatFlag_16BitSourceData
                        This flag is set for Apple Lossless data that was sourced from 16 bit native
                        endian signed integer data.
    @constant       kAppleLosslessFormatFlag_20BitSourceData
                        This flag is set for Apple Lossless data that was sourced from 20 bit native
                        endian signed integer data aligned high in 24 bits.
    @constant       kAppleLosslessFormatFlag_24BitSourceData
                        This flag is set for Apple Lossless data that was sourced from 24 bit native
                        endian signed integer data.
    @constant       kAppleLosslessFormatFlag_32BitSourceData
                        This flag is set for Apple Lossless data that was sourced from 32 bit native
                        endian signed integer data.
*/
CF_ENUM(AudioFormatFlags)
{
    kAudioFormatFlagIsFloat                     = (1U << 0),     // 0x1
    kAudioFormatFlagIsBigEndian                 = (1U << 1),     // 0x2
    kAudioFormatFlagIsSignedInteger             = (1U << 2),     // 0x4
    kAudioFormatFlagIsPacked                    = (1U << 3),     // 0x8
    kAudioFormatFlagIsAlignedHigh               = (1U << 4),     // 0x10
    kAudioFormatFlagIsNonInterleaved            = (1U << 5),     // 0x20
    kAudioFormatFlagIsNonMixable                = (1U << 6),     // 0x40
    kAudioFormatFlagsAreAllClear                = 0x80000000,
	
    kLinearPCMFormatFlagIsFloat                 = kAudioFormatFlagIsFloat,
    kLinearPCMFormatFlagIsBigEndian             = kAudioFormatFlagIsBigEndian,
    kLinearPCMFormatFlagIsSignedInteger         = kAudioFormatFlagIsSignedInteger,
    kLinearPCMFormatFlagIsPacked                = kAudioFormatFlagIsPacked,
    kLinearPCMFormatFlagIsAlignedHigh           = kAudioFormatFlagIsAlignedHigh,
    kLinearPCMFormatFlagIsNonInterleaved        = kAudioFormatFlagIsNonInterleaved,
    kLinearPCMFormatFlagIsNonMixable            = kAudioFormatFlagIsNonMixable,
    kLinearPCMFormatFlagsSampleFractionShift    = 7,
    kLinearPCMFormatFlagsSampleFractionMask     = (0x3F << kLinearPCMFormatFlagsSampleFractionShift),
    kLinearPCMFormatFlagsAreAllClear            = kAudioFormatFlagsAreAllClear,
	
    kAppleLosslessFormatFlag_16BitSourceData    = 1,
    kAppleLosslessFormatFlag_20BitSourceData    = 2,
    kAppleLosslessFormatFlag_24BitSourceData    = 3,
    kAppleLosslessFormatFlag_32BitSourceData    = 4
};

/*!
    @enum           Commonly Used Combinations of AudioFormatFlags
    @abstract       Some commonly used combinations of flags for AudioStreamBasicDescriptions.
    @constant       kAudioFormatFlagsNativeEndian
                        Defined to set or clear kAudioFormatFlagIsBigEndian depending on the
                        endianness of the processor at build time.
    @constant       kAudioFormatFlagsCanonical
                        The flags for the canonical audio sample type. This matches AudioSampleType.
    @constant       kAudioFormatFlagsAudioUnitCanonical
                        The flags for the canonical audio unit sample type. This matches
                        AudioUnitSampleType.
    @constant       kAudioFormatFlagsNativeFloatPacked
                        The flags for fully packed, native endian floating point data.
 
    @discussion
        The "canonical" flags are deprecated. CA_PREFER_FIXED_POINT is discouraged because floating-
        point performance on iOS is such that fixed point is no longer truly preferred. All Apple-
        supplied AudioUnits support floating point. Replacement should be done with careful
        consideration of the format being specified or expected, but often
        kAudioFormatFlagsCanonical can be replaced with kAudioFormatFlagsNativeFloatPacked, and
        kAudioFormatFlagsAudioUnitCanonical with kAudioFormatFlagsNativeFloatPacked |
        kAudioFormatFlagIsNonInterleaved.
*/
CF_ENUM(AudioFormatFlags)
{
#if TARGET_RT_BIG_ENDIAN
    kAudioFormatFlagsNativeEndian       = kAudioFormatFlagIsBigEndian,
#else
    kAudioFormatFlagsNativeEndian       = 0,
#endif
#if !defined(CA_PREFER_FIXED_POINT) || !CA_PREFER_FIXED_POINT
    kAudioFormatFlagsCanonical CA_CANONICAL_DEPRECATED          = kAudioFormatFlagIsFloat | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked,
    kAudioFormatFlagsAudioUnitCanonical CA_CANONICAL_DEPRECATED = kAudioFormatFlagIsFloat | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved,
#else
    kAudioFormatFlagsCanonical CA_CANONICAL_DEPRECATED          = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked,
    kAudioFormatFlagsAudioUnitCanonical CA_CANONICAL_DEPRECATED = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved | (kAudioUnitSampleFractionBits << kLinearPCMFormatFlagsSampleFractionShift),
#endif
    kAudioFormatFlagsNativeFloatPacked  = kAudioFormatFlagIsFloat | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked
};

/*!
    @defined    TestAudioFormatNativeEndian
    @abstract   A macro for checking if an ASBD indicates native endian linear PCM data.
*/
#define TestAudioFormatNativeEndian(f)  ((f.mFormatID == kAudioFormatLinearPCM) && ((f.mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagsNativeEndian))

/*!
    @function   IsAudioFormatNativeEndian
    @abstract   A C++ inline function for checking if an ASBD indicates native endian linear PCM
                data.
    @param      f
                    The AudioStreamBasicDescription to examine.
    @result     Whether or not the ASBD indicates native endian linear PCM data.
*/
#if defined(__cplusplus)
static inline bool IsAudioFormatNativeEndian(const AudioStreamBasicDescription& f) { return (f.mFormatID == kAudioFormatLinearPCM) && ((f.mFormatFlags & kAudioFormatFlagIsBigEndian) == kAudioFormatFlagsNativeEndian); }
#endif

/*!
    @function   CalculateLPCMFlags
    @abstract   A C++ inline function for calculating the mFormatFlags for linear PCM data. Note
                that this function does not support specifying sample formats that are either
                unsigned integer or low-aligned.
    @param      inSampleRate
 
    @param      inValidBitsPerChannel
                    The number of valid bits in each sample.
    @param      inTotalBitsPerChannel
                    The total number of bits in each sample.
    @param      inIsFloat
                    Whether or not the samples are represented with floating point numbers.
    @param      isIsBigEndian
                    Whether the samples are big endian or little endian.
    @result     A UInt32 containing the format flags.
*/
#if defined(__cplusplus)
static inline AudioFormatFlags    CalculateLPCMFlags(UInt32 inValidBitsPerChannel, UInt32 inTotalBitsPerChannel, bool inIsFloat, bool inIsBigEndian, bool inIsNonInterleaved = false) { return (inIsFloat ? kAudioFormatFlagIsFloat : kAudioFormatFlagIsSignedInteger) | (inIsBigEndian ? ((UInt32)kAudioFormatFlagIsBigEndian) : 0) | ((inValidBitsPerChannel == inTotalBitsPerChannel) ? kAudioFormatFlagIsPacked : kAudioFormatFlagIsAlignedHigh) | (inIsNonInterleaved ? ((UInt32)kAudioFormatFlagIsNonInterleaved) : 0); }
#endif

/*!
    @function   FillOutASBDForLPCM
    @abstract   A C++ inline function for filling out an AudioStreamBasicDescription to describe
                linear PCM data. Note that this function does not support specifying sample formats
                that are either unsigned integer or low-aligned.
    @param      outASBD
                    The AudioStreamBasicDescription to fill out.
    @param      inSampleRate
                    The number of sample frames per second of the data in the stream.
    @param      inChannelsPerFrame
                    The number of channels in each frame of data.
    @param      inValidBitsPerChannel
                    The number of valid bits in each sample.
    @param      inTotalBitsPerChannel
                    The total number of bits in each sample.
    @param      inIsFloat
                    Whether or not the samples are represented with floating point numbers.
    @param      isIsBigEndian
                    Whether the samples are big endian or little endian.
*/
#if defined(__cplusplus)
static inline void    FillOutASBDForLPCM(AudioStreamBasicDescription& outASBD, Float64 inSampleRate, UInt32 inChannelsPerFrame, UInt32 inValidBitsPerChannel, UInt32 inTotalBitsPerChannel, bool inIsFloat, bool inIsBigEndian, bool inIsNonInterleaved = false)    { outASBD.mSampleRate = inSampleRate; outASBD.mFormatID = kAudioFormatLinearPCM; outASBD.mFormatFlags = CalculateLPCMFlags(inValidBitsPerChannel, inTotalBitsPerChannel, inIsFloat, inIsBigEndian, inIsNonInterleaved); outASBD.mBytesPerPacket = (inIsNonInterleaved ? 1 : inChannelsPerFrame) * (inTotalBitsPerChannel / 8); outASBD.mFramesPerPacket = 1; outASBD.mBytesPerFrame = (inIsNonInterleaved ? 1 : inChannelsPerFrame) * (inTotalBitsPerChannel / 8); outASBD.mChannelsPerFrame = inChannelsPerFrame; outASBD.mBitsPerChannel = inValidBitsPerChannel; }
#endif


/*!
    @struct         AudioStreamPacketDescription
    @abstract       This structure describes the packet layout of a buffer of data where the size of
                    each packet may not be the same or where there is extraneous data between
                    packets.
    @var            mStartOffset
                        The number of bytes from the start of the buffer to the beginning of the
                        packet.
    @var            mVariableFramesInPacket
                        The number of sample frames of data in the packet. For formats with a
                        constant number of frames per packet, this field is set to 0.
    @var            mDataByteSize
                        The number of bytes in the packet.
*/
struct  AudioStreamPacketDescription
{
    SInt64  mStartOffset;
    UInt32  mVariableFramesInPacket;
    UInt32  mDataByteSize;
};
typedef struct AudioStreamPacketDescription AudioStreamPacketDescription;

//==================================================================================================
#pragma mark -
#pragma mark Audio Time Stamps

//  SMPTETime is also defined in the CoreVideo headers.
#if !defined(__SMPTETime__)
#define __SMPTETime__

/*!
    @enum           SMPTE Time Types
    @abstract       Constants that describe the type of SMPTE time.
    @constant       kSMPTETimeType24
                        24 Frame
    @constant       kSMPTETimeType25
                        25 Frame
    @constant       kSMPTETimeType30Drop
                        30 Drop Frame
    @constant       kSMPTETimeType30
                        30 Frame
    @constant       kSMPTETimeType2997
                        29.97 Frame
    @constant       kSMPTETimeType2997Drop
                        29.97 Drop Frame
    @constant       kSMPTETimeType60
                        60 Frame
    @constant       kSMPTETimeType5994
                        59.94 Frame
    @constant       kSMPTETimeType60Drop
                        60 Drop Frame
    @constant       kSMPTETimeType5994Drop
                        59.94 Drop Frame
    @constant       kSMPTETimeType50
                        50 Frame
    @constant       kSMPTETimeType2398
                        23.98 Frame
*/
typedef CF_ENUM(UInt32, SMPTETimeType)
{
    kSMPTETimeType24        = 0,
    kSMPTETimeType25        = 1,
    kSMPTETimeType30Drop    = 2,
    kSMPTETimeType30        = 3,
    kSMPTETimeType2997      = 4,
    kSMPTETimeType2997Drop  = 5,
    kSMPTETimeType60        = 6,
    kSMPTETimeType5994      = 7,
    kSMPTETimeType60Drop    = 8,
    kSMPTETimeType5994Drop  = 9,
    kSMPTETimeType50        = 10,
    kSMPTETimeType2398      = 11
};

/*!
    @enum           SMPTETimeFlags
    @abstract       Flags that describe the SMPTE time state.
    @constant       kSMPTETimeValid
                        The full time is valid.
    @constant       kSMPTETimeRunning
                        Time is running.
*/
typedef CF_OPTIONS(UInt32, SMPTETimeFlags)
{
    kSMPTETimeUnknown   = 0,
    kSMPTETimeValid     = (1U << 0),
    kSMPTETimeRunning   = (1U << 1)
};

/*!
    @struct         SMPTETime
    @abstract       A structure for holding a SMPTE time.
    @var            mSubframes
                        The number of subframes in the full message.
    @var            mSubframeDivisor
                        The number of subframes per frame (typically 80).
    @var            mCounter
                        The total number of messages received.
    @var            mType
                        The kind of SMPTE time using the SMPTE time type constants.
    @var            mFlags
                        A set of flags that indicate the SMPTE state.
    @var            mHours
                        The number of hours in the full message.
    @var            mMinutes
                        The number of minutes in the full message.
    @var            mSeconds
                        The number of seconds in the full message.
    @var            mFrames
                        The number of frames in the full message.
*/
struct SMPTETime
{
    SInt16          mSubframes;
    SInt16          mSubframeDivisor;
    UInt32          mCounter;
    SMPTETimeType   mType;
    SMPTETimeFlags  mFlags;
    SInt16          mHours;
    SInt16          mMinutes;
    SInt16          mSeconds;
    SInt16          mFrames;
};
typedef struct SMPTETime    SMPTETime;

#endif

/*!
    @enum           AudioTimeStampFlags
    @abstract       The flags that indicate which fields in an AudioTimeStamp structure are valid.
    @constant       kAudioTimeStampSampleTimeValid
                        The sample frame time is valid.
    @constant       kAudioTimeStampHostTimeValid
                        The host time is valid.
    @constant       kAudioTimeStampRateScalarValid
                        The rate scalar is valid.
    @constant       kAudioTimeStampWordClockTimeValid
                        The word clock time is valid.
    @constant       kAudioTimeStampSMPTETimeValid
                        The SMPTE time is valid.
    @constant       kAudioTimeStampSampleHostTimeValid
                        The sample frame time and the host time are valid.
*/
typedef CF_OPTIONS(UInt32, AudioTimeStampFlags)
{
    kAudioTimeStampNothingValid         = 0,
    kAudioTimeStampSampleTimeValid      = (1U << 0),
    kAudioTimeStampHostTimeValid        = (1U << 1),
    kAudioTimeStampRateScalarValid      = (1U << 2),
    kAudioTimeStampWordClockTimeValid   = (1U << 3),
    kAudioTimeStampSMPTETimeValid       = (1U << 4),
    kAudioTimeStampSampleHostTimeValid  = (kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid)
};

/*!
    @struct         AudioTimeStamp
    @abstract       A structure that holds different representations of the same point in time.
    @var            mSampleTime
                        The absolute sample frame time.
    @var            mHostTime
                        The host machine's time base, mach_absolute_time.
    @var            mRateScalar
                        The ratio of actual host ticks per sample frame to the nominal host ticks
                        per sample frame.
    @var            mWordClockTime
                        The word clock time.
    @var            mSMPTETime
                        The SMPTE time.
    @var            mFlags
                        A set of flags indicating which representations of the time are valid.
    @var            mReserved
                        Pads the structure out to force an even 8 byte alignment.
*/
struct AudioTimeStamp
{
    Float64             mSampleTime;
    UInt64              mHostTime;
    Float64             mRateScalar;
    UInt64              mWordClockTime;
    SMPTETime           mSMPTETime;
    AudioTimeStampFlags mFlags;
    UInt32              mReserved;
};
typedef struct AudioTimeStamp   AudioTimeStamp;

/*!
    @function   FillOutAudioTimeStampWithSampleTime
    @abstract   A C++ inline function for filling out an AudioTimeStamp with a sample time
    @param      outATS
                    The AudioTimeStamp to fill out.
    @param      inSampleTime
                    The sample time to put in the AudioTimeStamp.
*/
#if defined(__cplusplus)
static inline void    FillOutAudioTimeStampWithSampleTime(AudioTimeStamp& outATS, Float64 inSampleTime)    { outATS.mSampleTime = inSampleTime; outATS.mHostTime = 0; outATS.mRateScalar = 0; outATS.mWordClockTime = 0; memset(&outATS.mSMPTETime, 0, sizeof(SMPTETime)); outATS.mFlags = kAudioTimeStampSampleTimeValid; }
#endif

/*!
    @function   FillOutAudioTimeStampWithHostTime
    @abstract   A C++ inline function for filling out an AudioTimeStamp with a host time
    @param      outATS
                    The AudioTimeStamp to fill out.
    @param      inHostTime
                    The host time to put in the AudioTimeStamp.
*/
#if defined(__cplusplus)
static inline void    FillOutAudioTimeStampWithHostTime(AudioTimeStamp& outATS, UInt64 inHostTime) { outATS.mSampleTime = 0; outATS.mHostTime = inHostTime; outATS.mRateScalar = 0; outATS.mWordClockTime = 0; memset(&outATS.mSMPTETime, 0, sizeof(SMPTETime)); outATS.mFlags = kAudioTimeStampHostTimeValid; }
#endif

/*!
    @function   FillOutAudioTimeStampWithSampleAndHostTime
    @abstract   A C++ inline function for filling out an AudioTimeStamp with a sample time and a
                host time
    @param      outATS
                    The AudioTimeStamp to fill out.
    @param      inSampleTime
                    The sample time to put in the AudioTimeStamp.
    @param      inHostTime
                    The host time to put in the AudioTimeStamp.
*/
#if defined(__cplusplus)
static inline void    FillOutAudioTimeStampWithSampleAndHostTime(AudioTimeStamp& outATS, Float64 inSampleTime, UInt64 inHostTime) { outATS.mSampleTime = inSampleTime; outATS.mHostTime = inHostTime; outATS.mRateScalar = 0; outATS.mWordClockTime = 0; memset(&outATS.mSMPTETime, 0, sizeof(SMPTETime)); outATS.mFlags = kAudioTimeStampSampleTimeValid | kAudioTimeStampHostTimeValid; }
#endif

//==================================================================================================
#pragma mark -
#pragma mark AudioClassDescription

/*!
    @struct         AudioClassDescription
    @abstract       This structure is used to describe codecs installed on the system.
    @var            mType
                        The four char code codec type.
    @var            mSubType
                        The four char code codec subtype.
    @var            mManufacturer
                        The four char code codec manufacturer.
*/
struct AudioClassDescription {
    OSType  mType;
    OSType  mSubType;
    OSType  mManufacturer;
};
typedef struct AudioClassDescription    AudioClassDescription;


//==================================================================================================
#pragma mark -
#pragma mark Audio Channel Layout

/*!
    @typedef        AudioChannelLabel
    @abstract       A tag identifying how the channel is to be used.
*/
typedef UInt32 AudioChannelLabel;

/*!
    @typedef        AudioChannelLayoutTag
    @abstract       A tag identifying a particular pre-defined channel layout.
*/
typedef UInt32 AudioChannelLayoutTag;

/*!
    @enum           AudioChannelLabel Constants
    @abstract       These constants are for use in the mChannelLabel field of an
                    AudioChannelDescription structure.
    @discussion     These channel labels attempt to list all labels in common use. Due to the
                    ambiguities in channel labeling by various groups, there may be some overlap or
                    duplication in the labels below. Use the label which most clearly describes what
                    you mean.

                    WAVE files seem to follow the USB spec for the channel flags. A channel map lets
                    you put these channels in any order, however a WAVE file only supports labels
                    1-18 and if present, they must be in the order given below. The integer values
                    for the labels below match the bit position of the label in the USB bitmap and
                    thus also the WAVE file bitmap.
*/
CF_ENUM(AudioChannelLabel)
{
    kAudioChannelLabel_Unknown                  = 0xFFFFFFFF,   ///< unknown or unspecified other use
    kAudioChannelLabel_Unused                   = 0,            ///< channel is present, but has no intended use or destination
    kAudioChannelLabel_UseCoordinates           = 100,          ///< channel is described by the mCoordinates fields.

    kAudioChannelLabel_Left                     = 1,
    kAudioChannelLabel_Right                    = 2,
    kAudioChannelLabel_Center                   = 3,
    kAudioChannelLabel_LFEScreen                = 4,
    kAudioChannelLabel_LeftSurround             = 5,
    kAudioChannelLabel_RightSurround            = 6,
    kAudioChannelLabel_LeftCenter               = 7,
    kAudioChannelLabel_RightCenter              = 8,
    kAudioChannelLabel_CenterSurround           = 9,            ///< WAVE: "Back Center" or plain "Rear Surround"
    kAudioChannelLabel_LeftSurroundDirect       = 10,
    kAudioChannelLabel_RightSurroundDirect      = 11,
    kAudioChannelLabel_TopCenterSurround        = 12,
    kAudioChannelLabel_VerticalHeightLeft       = 13,           ///< WAVE: "Top Front Left"
    kAudioChannelLabel_VerticalHeightCenter     = 14,           ///< WAVE: "Top Front Center"
    kAudioChannelLabel_VerticalHeightRight      = 15,           ///< WAVE: "Top Front Right"

    kAudioChannelLabel_TopBackLeft              = 16,
    kAudioChannelLabel_TopBackCenter            = 17,
    kAudioChannelLabel_TopBackRight             = 18,

    kAudioChannelLabel_RearSurroundLeft         = 33,
    kAudioChannelLabel_RearSurroundRight        = 34,
    kAudioChannelLabel_LeftWide                 = 35,
    kAudioChannelLabel_RightWide                = 36,
    kAudioChannelLabel_LFE2                     = 37,
    kAudioChannelLabel_LeftTotal                = 38,           ///< matrix encoded 4 channels
    kAudioChannelLabel_RightTotal               = 39,           ///< matrix encoded 4 channels
    kAudioChannelLabel_HearingImpaired          = 40,
    kAudioChannelLabel_Narration                = 41,
    kAudioChannelLabel_Mono                     = 42,
    kAudioChannelLabel_DialogCentricMix         = 43,

    kAudioChannelLabel_CenterSurroundDirect     = 44,           ///< back center, non diffuse
	
    kAudioChannelLabel_Haptic                   = 45,
	
	kAudioChannelLabel_LeftTopFront             = kAudioChannelLabel_VerticalHeightLeft,
	kAudioChannelLabel_CenterTopFront           = kAudioChannelLabel_VerticalHeightCenter,
	kAudioChannelLabel_RightTopFront            = kAudioChannelLabel_VerticalHeightRight,
	kAudioChannelLabel_LeftTopMiddle            = 49,
	kAudioChannelLabel_CenterTopMiddle          = kAudioChannelLabel_TopCenterSurround,
	kAudioChannelLabel_RightTopMiddle           = 51,
	kAudioChannelLabel_LeftTopRear              = 52,
	kAudioChannelLabel_CenterTopRear            = 53,
	kAudioChannelLabel_RightTopRear             = 54,
	
    // first order ambisonic channels
    kAudioChannelLabel_Ambisonic_W              = 200,
    kAudioChannelLabel_Ambisonic_X              = 201,
    kAudioChannelLabel_Ambisonic_Y              = 202,
    kAudioChannelLabel_Ambisonic_Z              = 203,

    // Mid/Side Recording
    kAudioChannelLabel_MS_Mid                   = 204,
    kAudioChannelLabel_MS_Side                  = 205,

    // X-Y Recording
    kAudioChannelLabel_XY_X                     = 206,
    kAudioChannelLabel_XY_Y                     = 207,
	
    // Binaural Recording
    kAudioChannelLabel_BinauralLeft             = 208,
    kAudioChannelLabel_BinauralRight            = 209,

    // other
    kAudioChannelLabel_HeadphonesLeft           = 301,
    kAudioChannelLabel_HeadphonesRight          = 302,
    kAudioChannelLabel_ClickTrack               = 304,
    kAudioChannelLabel_ForeignLanguage          = 305,

    // generic discrete channel
    kAudioChannelLabel_Discrete                 = 400,

    // numbered discrete channel
    kAudioChannelLabel_Discrete_0               = (1U<<16) | 0,
    kAudioChannelLabel_Discrete_1               = (1U<<16) | 1,
    kAudioChannelLabel_Discrete_2               = (1U<<16) | 2,
    kAudioChannelLabel_Discrete_3               = (1U<<16) | 3,
    kAudioChannelLabel_Discrete_4               = (1U<<16) | 4,
    kAudioChannelLabel_Discrete_5               = (1U<<16) | 5,
    kAudioChannelLabel_Discrete_6               = (1U<<16) | 6,
    kAudioChannelLabel_Discrete_7               = (1U<<16) | 7,
    kAudioChannelLabel_Discrete_8               = (1U<<16) | 8,
    kAudioChannelLabel_Discrete_9               = (1U<<16) | 9,
    kAudioChannelLabel_Discrete_10              = (1U<<16) | 10,
    kAudioChannelLabel_Discrete_11              = (1U<<16) | 11,
    kAudioChannelLabel_Discrete_12              = (1U<<16) | 12,
    kAudioChannelLabel_Discrete_13              = (1U<<16) | 13,
    kAudioChannelLabel_Discrete_14              = (1U<<16) | 14,
    kAudioChannelLabel_Discrete_15              = (1U<<16) | 15,
    kAudioChannelLabel_Discrete_65535           = (1U<<16) | 65535,
	
    // generic HOA ACN channel
    kAudioChannelLabel_HOA_ACN                  = 500,
	
    // numbered HOA ACN channels, SN3D normalization
    kAudioChannelLabel_HOA_ACN_0                = (2U << 16) | 0,
    kAudioChannelLabel_HOA_ACN_1                = (2U << 16) | 1,
    kAudioChannelLabel_HOA_ACN_2                = (2U << 16) | 2,
    kAudioChannelLabel_HOA_ACN_3                = (2U << 16) | 3,
    kAudioChannelLabel_HOA_ACN_4                = (2U << 16) | 4,
    kAudioChannelLabel_HOA_ACN_5                = (2U << 16) | 5,
    kAudioChannelLabel_HOA_ACN_6                = (2U << 16) | 6,
    kAudioChannelLabel_HOA_ACN_7                = (2U << 16) | 7,
    kAudioChannelLabel_HOA_ACN_8                = (2U << 16) | 8,
    kAudioChannelLabel_HOA_ACN_9                = (2U << 16) | 9,
    kAudioChannelLabel_HOA_ACN_10               = (2U << 16) | 10,
    kAudioChannelLabel_HOA_ACN_11               = (2U << 16) | 11,
    kAudioChannelLabel_HOA_ACN_12               = (2U << 16) | 12,
    kAudioChannelLabel_HOA_ACN_13               = (2U << 16) | 13,
    kAudioChannelLabel_HOA_ACN_14               = (2U << 16) | 14,
    kAudioChannelLabel_HOA_ACN_15               = (2U << 16) | 15,
    kAudioChannelLabel_HOA_ACN_65024            = (2U << 16) | 65024,    // 254th order uses 65025 channels

	// Specific SN3D alias
    kAudioChannelLabel_HOA_SN3D	                = kAudioChannelLabel_HOA_ACN_0, // Needs to be ORed with the channel index, not HOA order

    // HOA N3D
    kAudioChannelLabel_HOA_N3D                  = (3U << 16), // Needs to be ORed with the channel index, not HOA order

    kAudioChannelLabel_BeginReserved            = 0xF0000000,           // Channel label values in this range are reserved for internal use
    kAudioChannelLabel_EndReserved              = 0xFFFFFFFE
};

/*!
    @enum           AudioChannelBitmap
    @abstract       These constants are for use in the mChannelBitmap field of an
                    AudioChannelLayout structure.
*/
typedef CF_OPTIONS(UInt32, AudioChannelBitmap)
{
    kAudioChannelBit_Left                       = (1U<<0),
    kAudioChannelBit_Right                      = (1U<<1),
    kAudioChannelBit_Center                     = (1U<<2),
    kAudioChannelBit_LFEScreen                  = (1U<<3),
    kAudioChannelBit_LeftSurround               = (1U<<4),
    kAudioChannelBit_RightSurround              = (1U<<5),
    kAudioChannelBit_LeftCenter                 = (1U<<6),
    kAudioChannelBit_RightCenter                = (1U<<7),
    kAudioChannelBit_CenterSurround             = (1U<<8),      // WAVE: "Back Center"
    kAudioChannelBit_LeftSurroundDirect         = (1U<<9),
    kAudioChannelBit_RightSurroundDirect        = (1U<<10),
    kAudioChannelBit_TopCenterSurround          = (1U<<11),
    kAudioChannelBit_VerticalHeightLeft         = (1U<<12),     // WAVE: "Top Front Left"
    kAudioChannelBit_VerticalHeightCenter       = (1U<<13),     // WAVE: "Top Front Center"
    kAudioChannelBit_VerticalHeightRight        = (1U<<14),     // WAVE: "Top Front Right"
    kAudioChannelBit_TopBackLeft                = (1U<<15),
    kAudioChannelBit_TopBackCenter              = (1U<<16),
    kAudioChannelBit_TopBackRight               = (1U<<17),
	kAudioChannelBit_LeftTopFront             	= kAudioChannelBit_VerticalHeightLeft,
	kAudioChannelBit_CenterTopFront           	= kAudioChannelBit_VerticalHeightCenter,
	kAudioChannelBit_RightTopFront            	= kAudioChannelBit_VerticalHeightRight,
	kAudioChannelBit_LeftTopMiddle              = (1U<<21),
	kAudioChannelBit_CenterTopMiddle            = kAudioChannelBit_TopCenterSurround,
	kAudioChannelBit_RightTopMiddle             = (1U<<23),
	kAudioChannelBit_LeftTopRear                = (1U<<24),
	kAudioChannelBit_CenterTopRear              = (1U<<25),
	kAudioChannelBit_RightTopRear               = (1U<<26),
};

/*!
    @enum           AudioChannelFlags
    @abstract       These constants are used in the mChannelFlags field of an
                    AudioChannelDescription structure.
    @constant       kAudioChannelFlags_RectangularCoordinates
                        The channel is specified by the cartesian coordinates of the speaker
                        position. This flag is mutally exclusive with
                        kAudioChannelFlags_SphericalCoordinates.
    @constant       kAudioChannelFlags_SphericalCoordinates
                        The channel is specified by the spherical coordinates of the speaker
                        position. This flag is mutally exclusive with
                        kAudioChannelFlags_RectangularCoordinates.
    @constant       kAudioChannelFlags_Meters
                        Set to indicate the units are in meters, clear to indicate the units are
                        relative to the unit cube or unit sphere.
*/
typedef CF_OPTIONS(UInt32, AudioChannelFlags)
{
    kAudioChannelFlags_AllOff                   = 0,
    kAudioChannelFlags_RectangularCoordinates   = (1U<<0),
    kAudioChannelFlags_SphericalCoordinates     = (1U<<1),
    kAudioChannelFlags_Meters                   = (1U<<2)
};

// these are indices for accessing the mCoordinates array in struct AudioChannelDescription
/*!
    @enum           AudioChannelCoordinateIndex
    @abstract       Constants for indexing the mCoordinates array in an AudioChannelDescription
                    structure.
    @constant       kAudioChannelCoordinates_LeftRight
                        For rectangular coordinates, negative is left and positive is right.
    @constant       kAudioChannelCoordinates_BackFront
                        For rectangular coordinates, negative is back and positive is front.
    @constant       kAudioChannelCoordinates_DownUp
                        For rectangular coordinates, negative is below ground level, 0 is ground
                        level, and positive is above ground level.
    @constant       kAudioChannelCoordinates_Azimuth
                        For spherical coordinates, 0 is front center, positive is right, negative is
                        left. This is measured in degrees.
    @constant       kAudioChannelCoordinates_Elevation
                        For spherical coordinates, +90 is zenith, 0 is horizontal, -90 is nadir.
                        This is measured in degrees.
    @constant       kAudioChannelCoordinates_Distance
                        For spherical coordinates, the units are described by flags.
*/
typedef CF_ENUM(UInt32, AudioChannelCoordinateIndex)
{
    kAudioChannelCoordinates_LeftRight  = 0,
    kAudioChannelCoordinates_BackFront  = 1,
    kAudioChannelCoordinates_DownUp     = 2,
    kAudioChannelCoordinates_Azimuth    = 0,
    kAudioChannelCoordinates_Elevation  = 1,
    kAudioChannelCoordinates_Distance   = 2
};

/*!
    @enum           AudioChannelLayoutTag Constants
    @abstract       These constants are used in the mChannelLayoutTag field of an AudioChannelLayout
                    structure.
*/
CF_ENUM(AudioChannelLayoutTag)
{
    // Some channel abbreviations used below:
    // L - left
    // R - right
    // C - center
    // Ls - left surround
    // Rs - right surround
    // Cs - center surround
    // Rls - rear left surround
    // Rrs - rear right surround
    // Lw - left wide
    // Rw - right wide
    // Lsd - left surround direct
    // Rsd - right surround direct
    // Lc - left center
    // Rc - right center
    // Ts - top surround
    // Vhl - vertical height left
    // Vhc - vertical height center
    // Vhr - vertical height right
	// Ltm - left top middle
	// Rtm - right top middle
	// Ltr - left top rear
	// Ctr - center top rear
	// Rtr - right top rear
    // Lt - left matrix total. for matrix encoded stereo.
    // Rt - right matrix total. for matrix encoded stereo.

    //  General layouts
    kAudioChannelLayoutTag_UseChannelDescriptions   = (0U<<16) | 0,     ///< use the array of AudioChannelDescriptions to define the mapping.
    kAudioChannelLayoutTag_UseChannelBitmap         = (1U<<16) | 0,     ///< use the bitmap to define the mapping.

    kAudioChannelLayoutTag_Mono                     = (100U<<16) | 1,   ///< a standard mono stream
    kAudioChannelLayoutTag_Stereo                   = (101U<<16) | 2,   ///< a standard stereo stream (L R) - implied playback
    kAudioChannelLayoutTag_StereoHeadphones         = (102U<<16) | 2,   ///< a standard stereo stream (L R) - implied headphone playback
    kAudioChannelLayoutTag_MatrixStereo             = (103U<<16) | 2,   ///< a matrix encoded stereo stream (Lt, Rt)
    kAudioChannelLayoutTag_MidSide                  = (104U<<16) | 2,   ///< mid/side recording
    kAudioChannelLayoutTag_XY                       = (105U<<16) | 2,   ///< coincident mic pair (often 2 figure 8's)
    kAudioChannelLayoutTag_Binaural                 = (106U<<16) | 2,   ///< binaural stereo (left, right)
    kAudioChannelLayoutTag_Ambisonic_B_Format       = (107U<<16) | 4,   ///< W, X, Y, Z

    kAudioChannelLayoutTag_Quadraphonic             = (108U<<16) | 4,   ///< L R Ls Rs  -- 90 degree speaker separation
    kAudioChannelLayoutTag_Pentagonal               = (109U<<16) | 5,   ///< L R Ls Rs C  -- 72 degree speaker separation
    kAudioChannelLayoutTag_Hexagonal                = (110U<<16) | 6,   ///< L R Ls Rs C Cs  -- 60 degree speaker separation
    kAudioChannelLayoutTag_Octagonal                = (111U<<16) | 8,   ///< L R Ls Rs C Cs Lw Rw  -- 45 degree speaker separation
    kAudioChannelLayoutTag_Cube                     = (112U<<16) | 8,   ///< left, right, rear left, rear right
																		///< top left, top right, top rear left, top rear right

    //  MPEG defined layouts
    kAudioChannelLayoutTag_MPEG_1_0                 = kAudioChannelLayoutTag_Mono,          ///<  C
    kAudioChannelLayoutTag_MPEG_2_0                 = kAudioChannelLayoutTag_Stereo,        ///<  L R
    kAudioChannelLayoutTag_MPEG_3_0_A               = (113U<<16) | 3,                       ///<  L R C
    kAudioChannelLayoutTag_MPEG_3_0_B               = (114U<<16) | 3,                       ///<  C L R
    kAudioChannelLayoutTag_MPEG_4_0_A               = (115U<<16) | 4,                       ///<  L R C Cs
    kAudioChannelLayoutTag_MPEG_4_0_B               = (116U<<16) | 4,                       ///<  C L R Cs
    kAudioChannelLayoutTag_MPEG_5_0_A               = (117U<<16) | 5,                       ///<  L R C Ls Rs
    kAudioChannelLayoutTag_MPEG_5_0_B               = (118U<<16) | 5,                       ///<  L R Ls Rs C
    kAudioChannelLayoutTag_MPEG_5_0_C               = (119U<<16) | 5,                       ///<  L C R Ls Rs
    kAudioChannelLayoutTag_MPEG_5_0_D               = (120U<<16) | 5,                       ///<  C L R Ls Rs
    kAudioChannelLayoutTag_MPEG_5_1_A               = (121U<<16) | 6,                       ///<  L R C LFE Ls Rs
    kAudioChannelLayoutTag_MPEG_5_1_B               = (122U<<16) | 6,                       ///<  L R Ls Rs C LFE
    kAudioChannelLayoutTag_MPEG_5_1_C               = (123U<<16) | 6,                       ///<  L C R Ls Rs LFE
    kAudioChannelLayoutTag_MPEG_5_1_D               = (124U<<16) | 6,                       ///<  C L R Ls Rs LFE
    kAudioChannelLayoutTag_MPEG_6_1_A               = (125U<<16) | 7,                       ///<  L R C LFE Ls Rs Cs
    kAudioChannelLayoutTag_MPEG_7_1_A               = (126U<<16) | 8,                       ///<  L R C LFE Ls Rs Lc Rc
    kAudioChannelLayoutTag_MPEG_7_1_B               = (127U<<16) | 8,                       ///<  C Lc Rc L R Ls Rs LFE    (doc: IS-13818-7 MPEG2-AAC Table 3.1)
    kAudioChannelLayoutTag_MPEG_7_1_C               = (128U<<16) | 8,                       ///<  L R C LFE Ls Rs Rls Rrs
    kAudioChannelLayoutTag_Emagic_Default_7_1       = (129U<<16) | 8,                       ///<  L R Ls Rs C LFE Lc Rc
    kAudioChannelLayoutTag_SMPTE_DTV                = (130U<<16) | 8,                       ///<  L R C LFE Ls Rs Lt Rt
                                                                                            ///<      (kAudioChannelLayoutTag_ITU_5_1 plus a matrix encoded stereo mix)

    //  ITU defined layouts
    kAudioChannelLayoutTag_ITU_1_0                  = kAudioChannelLayoutTag_Mono,          ///<  C
    kAudioChannelLayoutTag_ITU_2_0                  = kAudioChannelLayoutTag_Stereo,        ///<  L R

    kAudioChannelLayoutTag_ITU_2_1                  = (131U<<16) | 3,                       ///<  L R Cs
    kAudioChannelLayoutTag_ITU_2_2                  = (132U<<16) | 4,                       ///<  L R Ls Rs
    kAudioChannelLayoutTag_ITU_3_0                  = kAudioChannelLayoutTag_MPEG_3_0_A,    ///<  L R C
    kAudioChannelLayoutTag_ITU_3_1                  = kAudioChannelLayoutTag_MPEG_4_0_A,    ///<  L R C Cs

    kAudioChannelLayoutTag_ITU_3_2                  = kAudioChannelLayoutTag_MPEG_5_0_A,    ///<  L R C Ls Rs
    kAudioChannelLayoutTag_ITU_3_2_1                = kAudioChannelLayoutTag_MPEG_5_1_A,    ///<  L R C LFE Ls Rs
    kAudioChannelLayoutTag_ITU_3_4_1                = kAudioChannelLayoutTag_MPEG_7_1_C,    ///<  L R C LFE Ls Rs Rls Rrs

    // DVD defined layouts
    kAudioChannelLayoutTag_DVD_0                    = kAudioChannelLayoutTag_Mono,          ///< C (mono)
    kAudioChannelLayoutTag_DVD_1                    = kAudioChannelLayoutTag_Stereo,        ///< L R
    kAudioChannelLayoutTag_DVD_2                    = kAudioChannelLayoutTag_ITU_2_1,       ///< L R Cs
    kAudioChannelLayoutTag_DVD_3                    = kAudioChannelLayoutTag_ITU_2_2,       ///< L R Ls Rs
    kAudioChannelLayoutTag_DVD_4                    = (133U<<16) | 3,                       ///< L R LFE
    kAudioChannelLayoutTag_DVD_5                    = (134U<<16) | 4,                       ///< L R LFE Cs
    kAudioChannelLayoutTag_DVD_6                    = (135U<<16) | 5,                       ///< L R LFE Ls Rs
    kAudioChannelLayoutTag_DVD_7                    = kAudioChannelLayoutTag_MPEG_3_0_A,    ///< L R C
    kAudioChannelLayoutTag_DVD_8                    = kAudioChannelLayoutTag_MPEG_4_0_A,    ///< L R C Cs
    kAudioChannelLayoutTag_DVD_9                    = kAudioChannelLayoutTag_MPEG_5_0_A,    ///< L R C Ls Rs
    kAudioChannelLayoutTag_DVD_10                   = (136U<<16) | 4,                       ///< L R C LFE
    kAudioChannelLayoutTag_DVD_11                   = (137U<<16) | 5,                       ///< L R C LFE Cs
    kAudioChannelLayoutTag_DVD_12                   = kAudioChannelLayoutTag_MPEG_5_1_A,    ///< L R C LFE Ls Rs
    // 13 through 17 are duplicates of 8 through 12.
    kAudioChannelLayoutTag_DVD_13                   = kAudioChannelLayoutTag_DVD_8,         ///< L R C Cs
    kAudioChannelLayoutTag_DVD_14                   = kAudioChannelLayoutTag_DVD_9,         ///< L R C Ls Rs
    kAudioChannelLayoutTag_DVD_15                   = kAudioChannelLayoutTag_DVD_10,        ///< L R C LFE
    kAudioChannelLayoutTag_DVD_16                   = kAudioChannelLayoutTag_DVD_11,        ///< L R C LFE Cs
    kAudioChannelLayoutTag_DVD_17                   = kAudioChannelLayoutTag_DVD_12,        ///< L R C LFE Ls Rs
    kAudioChannelLayoutTag_DVD_18                   = (138U<<16) | 5,                       ///< L R Ls Rs LFE
    kAudioChannelLayoutTag_DVD_19                   = kAudioChannelLayoutTag_MPEG_5_0_B,    ///< L R Ls Rs C
    kAudioChannelLayoutTag_DVD_20                   = kAudioChannelLayoutTag_MPEG_5_1_B,    ///< L R Ls Rs C LFE

    // These layouts are recommended for AudioUnit usage
        // These are the symmetrical layouts
    kAudioChannelLayoutTag_AudioUnit_4              = kAudioChannelLayoutTag_Quadraphonic,
    kAudioChannelLayoutTag_AudioUnit_5              = kAudioChannelLayoutTag_Pentagonal,
    kAudioChannelLayoutTag_AudioUnit_6              = kAudioChannelLayoutTag_Hexagonal,
    kAudioChannelLayoutTag_AudioUnit_8              = kAudioChannelLayoutTag_Octagonal,
        // These are the surround-based layouts
    kAudioChannelLayoutTag_AudioUnit_5_0            = kAudioChannelLayoutTag_MPEG_5_0_B,    ///< L R Ls Rs C
    kAudioChannelLayoutTag_AudioUnit_6_0            = (139U<<16) | 6,                       ///< L R Ls Rs C Cs
    kAudioChannelLayoutTag_AudioUnit_7_0            = (140U<<16) | 7,                       ///< L R Ls Rs C Rls Rrs
    kAudioChannelLayoutTag_AudioUnit_7_0_Front      = (148U<<16) | 7,                       ///< L R Ls Rs C Lc Rc
    kAudioChannelLayoutTag_AudioUnit_5_1            = kAudioChannelLayoutTag_MPEG_5_1_A,    ///< L R C LFE Ls Rs
    kAudioChannelLayoutTag_AudioUnit_6_1            = kAudioChannelLayoutTag_MPEG_6_1_A,    ///< L R C LFE Ls Rs Cs
    kAudioChannelLayoutTag_AudioUnit_7_1            = kAudioChannelLayoutTag_MPEG_7_1_C,    ///< L R C LFE Ls Rs Rls Rrs
    kAudioChannelLayoutTag_AudioUnit_7_1_Front      = kAudioChannelLayoutTag_MPEG_7_1_A,    ///< L R C LFE Ls Rs Lc Rc

    kAudioChannelLayoutTag_AAC_3_0                  = kAudioChannelLayoutTag_MPEG_3_0_B,    ///< C L R
    kAudioChannelLayoutTag_AAC_Quadraphonic         = kAudioChannelLayoutTag_Quadraphonic,  ///< L R Ls Rs
    kAudioChannelLayoutTag_AAC_4_0                  = kAudioChannelLayoutTag_MPEG_4_0_B,    ///< C L R Cs
    kAudioChannelLayoutTag_AAC_5_0                  = kAudioChannelLayoutTag_MPEG_5_0_D,    ///< C L R Ls Rs
    kAudioChannelLayoutTag_AAC_5_1                  = kAudioChannelLayoutTag_MPEG_5_1_D,    ///< C L R Ls Rs Lfe
    kAudioChannelLayoutTag_AAC_6_0                  = (141U<<16) | 6,                       ///< C L R Ls Rs Cs
    kAudioChannelLayoutTag_AAC_6_1                  = (142U<<16) | 7,                       ///< C L R Ls Rs Cs Lfe
    kAudioChannelLayoutTag_AAC_7_0                  = (143U<<16) | 7,                       ///< C L R Ls Rs Rls Rrs
    kAudioChannelLayoutTag_AAC_7_1                  = kAudioChannelLayoutTag_MPEG_7_1_B,    ///< C Lc Rc L R Ls Rs Lfe
    kAudioChannelLayoutTag_AAC_7_1_B                = (183U<<16) | 8,                       ///< C L R Ls Rs Rls Rrs LFE
    kAudioChannelLayoutTag_AAC_7_1_C                = (184U<<16) | 8,                       ///< C L R Ls Rs LFE Vhl Vhr
    kAudioChannelLayoutTag_AAC_Octagonal            = (144U<<16) | 8,                       ///< C L R Ls Rs Rls Rrs Cs

    kAudioChannelLayoutTag_TMH_10_2_std             = (145U<<16) | 16,                      ///< L R C Vhc Lsd Rsd Ls Rs Vhl Vhr Lw Rw Csd Cs LFE1 LFE2
    kAudioChannelLayoutTag_TMH_10_2_full            = (146U<<16) | 21,                      ///< TMH_10_2_std plus: Lc Rc HI VI Haptic

    kAudioChannelLayoutTag_AC3_1_0_1                = (149U<<16) | 2,                       ///< C LFE
    kAudioChannelLayoutTag_AC3_3_0                  = (150U<<16) | 3,                       ///< L C R
    kAudioChannelLayoutTag_AC3_3_1                  = (151U<<16) | 4,                       ///< L C R Cs
    kAudioChannelLayoutTag_AC3_3_0_1                = (152U<<16) | 4,                       ///< L C R LFE
    kAudioChannelLayoutTag_AC3_2_1_1                = (153U<<16) | 4,                       ///< L R Cs LFE
    kAudioChannelLayoutTag_AC3_3_1_1                = (154U<<16) | 5,                       ///< L C R Cs LFE

    kAudioChannelLayoutTag_EAC_6_0_A                = (155U<<16) | 6,                       ///< L C R Ls Rs Cs
    kAudioChannelLayoutTag_EAC_7_0_A                = (156U<<16) | 7,                       ///< L C R Ls Rs Rls Rrs

    kAudioChannelLayoutTag_EAC3_6_1_A               = (157U<<16) | 7,                       ///< L C R Ls Rs LFE Cs
    kAudioChannelLayoutTag_EAC3_6_1_B               = (158U<<16) | 7,                       ///< L C R Ls Rs LFE Ts
    kAudioChannelLayoutTag_EAC3_6_1_C               = (159U<<16) | 7,                       ///< L C R Ls Rs LFE Vhc
    kAudioChannelLayoutTag_EAC3_7_1_A               = (160U<<16) | 8,                       ///< L C R Ls Rs LFE Rls Rrs
    kAudioChannelLayoutTag_EAC3_7_1_B               = (161U<<16) | 8,                       ///< L C R Ls Rs LFE Lc Rc
    kAudioChannelLayoutTag_EAC3_7_1_C               = (162U<<16) | 8,                       ///< L C R Ls Rs LFE Lsd Rsd
    kAudioChannelLayoutTag_EAC3_7_1_D               = (163U<<16) | 8,                       ///< L C R Ls Rs LFE Lw Rw
    kAudioChannelLayoutTag_EAC3_7_1_E               = (164U<<16) | 8,                       ///< L C R Ls Rs LFE Vhl Vhr

    kAudioChannelLayoutTag_EAC3_7_1_F               = (165U<<16) | 8,                       ///< L C R Ls Rs LFE Cs Ts
    kAudioChannelLayoutTag_EAC3_7_1_G               = (166U<<16) | 8,                       ///< L C R Ls Rs LFE Cs Vhc
    kAudioChannelLayoutTag_EAC3_7_1_H               = (167U<<16) | 8,                       ///< L C R Ls Rs LFE Ts Vhc

    kAudioChannelLayoutTag_DTS_3_1                  = (168U<<16) | 4,                       ///< C L R LFE
    kAudioChannelLayoutTag_DTS_4_1                  = (169U<<16) | 5,                       ///< C L R Cs LFE
    kAudioChannelLayoutTag_DTS_6_0_A                = (170U<<16) | 6,                       ///< Lc Rc L R Ls Rs
    kAudioChannelLayoutTag_DTS_6_0_B                = (171U<<16) | 6,                       ///< C L R Rls Rrs Ts
    kAudioChannelLayoutTag_DTS_6_0_C                = (172U<<16) | 6,                       ///< C Cs L R Rls Rrs
    kAudioChannelLayoutTag_DTS_6_1_A                = (173U<<16) | 7,                       ///< Lc Rc L R Ls Rs LFE
    kAudioChannelLayoutTag_DTS_6_1_B                = (174U<<16) | 7,                       ///< C L R Rls Rrs Ts LFE
    kAudioChannelLayoutTag_DTS_6_1_C                = (175U<<16) | 7,                       ///< C Cs L R Rls Rrs LFE
    kAudioChannelLayoutTag_DTS_7_0                  = (176U<<16) | 7,                       ///< Lc C Rc L R Ls Rs
    kAudioChannelLayoutTag_DTS_7_1                  = (177U<<16) | 8,                       ///< Lc C Rc L R Ls Rs LFE
    kAudioChannelLayoutTag_DTS_8_0_A                = (178U<<16) | 8,                       ///< Lc Rc L R Ls Rs Rls Rrs
    kAudioChannelLayoutTag_DTS_8_0_B                = (179U<<16) | 8,                       ///< Lc C Rc L R Ls Cs Rs
    kAudioChannelLayoutTag_DTS_8_1_A                = (180U<<16) | 9,                       ///< Lc Rc L R Ls Rs Rls Rrs LFE
    kAudioChannelLayoutTag_DTS_8_1_B                = (181U<<16) | 9,                       ///< Lc C Rc L R Ls Cs Rs LFE
    kAudioChannelLayoutTag_DTS_6_1_D                = (182U<<16) | 7,                       ///< C L R Ls Rs LFE Cs

    kAudioChannelLayoutTag_WAVE_2_1                 = kAudioChannelLayoutTag_DVD_4,         ///< 3 channels, L R LFE
    kAudioChannelLayoutTag_WAVE_3_0                 = kAudioChannelLayoutTag_MPEG_3_0_A,    ///< 3 channels, L R C
    kAudioChannelLayoutTag_WAVE_4_0_A               = kAudioChannelLayoutTag_ITU_2_2,       ///< 4 channels, L R Ls Rs
    kAudioChannelLayoutTag_WAVE_4_0_B               = (185U<<16) | 4,                       ///< 4 channels, L R Rls Rrs
    kAudioChannelLayoutTag_WAVE_5_0_A               = kAudioChannelLayoutTag_MPEG_5_0_A,    ///< 5 channels, L R C Ls Rs
    kAudioChannelLayoutTag_WAVE_5_0_B               = (186U<<16) | 5,                       ///< 5 channels, L R C Rls Rrs
    kAudioChannelLayoutTag_WAVE_5_1_A               = kAudioChannelLayoutTag_MPEG_5_1_A,    ///< 6 channels, L R C LFE Ls Rs
    kAudioChannelLayoutTag_WAVE_5_1_B               = (187U<<16) | 6,                       ///< 6 channels, L R C LFE Rls Rrs
    kAudioChannelLayoutTag_WAVE_6_1                 = (188U<<16) | 7,                       ///< 7 channels, L R C LFE Cs Ls Rs
    kAudioChannelLayoutTag_WAVE_7_1                 = (189U<<16) | 8,                       ///< 8 channels, L R C LFE Rls Rrs Ls Rs

    kAudioChannelLayoutTag_HOA_ACN_SN3D             = (190U<<16) | 0,                       ///< Higher Order Ambisonics, Ambisonics Channel Number, SN3D normalization
                                                                                            ///< needs to be ORed with the actual number of channels (not the HOA order)
    kAudioChannelLayoutTag_HOA_ACN_N3D              = (191U<<16) | 0,                       ///< Higher Order Ambisonics, Ambisonics Channel Number, N3D normalization
                                                                                            ///< needs to be ORed with the actual number of channels (not the HOA order)
	
	kAudioChannelLayoutTag_Atmos_5_1_2              = (194U<<16) | 8,                       ///< L R C LFE Ls Rs Ltm Rtm
	kAudioChannelLayoutTag_Atmos_5_1_4              = (195U<<16) | 10,                      ///< L R C LFE Ls Rs Vhl Vhr Ltr Rtr
	kAudioChannelLayoutTag_Atmos_7_1_2              = (196U<<16) | 10,                      ///< L R C LFE Ls Rs Rls Rrs Ltm Rtm
	kAudioChannelLayoutTag_Atmos_7_1_4              = (192U<<16) | 12,                      ///< L R C LFE Ls Rs Rls Rrs Vhl Vhr Ltr Rtr
	kAudioChannelLayoutTag_Atmos_9_1_6              = (193U<<16) | 16,                      ///< L R C LFE Ls Rs Rls Rrs Lw Rw Vhl Vhr Ltm Rtm Ltr Rtr

    kAudioChannelLayoutTag_Logic_Mono               = kAudioChannelLayoutTag_Mono,          ///< C
    kAudioChannelLayoutTag_Logic_Stereo             = kAudioChannelLayoutTag_Stereo,        ///< L R
    kAudioChannelLayoutTag_Logic_Quadraphonic       = kAudioChannelLayoutTag_Quadraphonic,  ///< L R Ls Rs
    kAudioChannelLayoutTag_Logic_4_0_A              = kAudioChannelLayoutTag_MPEG_4_0_A,    ///< L R C Cs
    kAudioChannelLayoutTag_Logic_4_0_B              = kAudioChannelLayoutTag_MPEG_4_0_B,    ///< C L R Cs
    kAudioChannelLayoutTag_Logic_4_0_C              = (197U<<16) | 4,                       ///< L R Cs C
    kAudioChannelLayoutTag_Logic_5_0_A              = kAudioChannelLayoutTag_MPEG_5_0_A,    ///< L R C Ls Rs
    kAudioChannelLayoutTag_Logic_5_0_B              = kAudioChannelLayoutTag_MPEG_5_0_B,    ///< L R Ls Rs C
    kAudioChannelLayoutTag_Logic_5_0_C              = kAudioChannelLayoutTag_MPEG_5_0_C,    ///< L C R Ls Rs
    kAudioChannelLayoutTag_Logic_5_0_D              = kAudioChannelLayoutTag_MPEG_5_0_D,    ///< C L R Ls Rs
    kAudioChannelLayoutTag_Logic_5_1_A              = kAudioChannelLayoutTag_MPEG_5_1_A,    ///< L R C LFE Ls Rs
    kAudioChannelLayoutTag_Logic_5_1_B              = kAudioChannelLayoutTag_MPEG_5_1_B,    ///< L R Ls Rs C LFE
    kAudioChannelLayoutTag_Logic_5_1_C              = kAudioChannelLayoutTag_MPEG_5_1_C,    ///< L C R Ls Rs LFE
    kAudioChannelLayoutTag_Logic_5_1_D              = kAudioChannelLayoutTag_MPEG_5_1_D,    ///< C L R Ls Rs LFE
    kAudioChannelLayoutTag_Logic_6_0_A              = kAudioChannelLayoutTag_AAC_6_0,       ///< C L R Ls Rs Cs
    kAudioChannelLayoutTag_Logic_6_0_B              = (198U<<16) | 6,                       ///< L R Ls Rs Cs C
    kAudioChannelLayoutTag_Logic_6_0_C              = kAudioChannelLayoutTag_AudioUnit_6_0, ///< L R Ls Rs C Cs
    kAudioChannelLayoutTag_Logic_6_1_A              = kAudioChannelLayoutTag_AAC_6_1,       ///< C L R Ls Rs Cs LFE
    kAudioChannelLayoutTag_Logic_6_1_B              = (199U<<16) | 7,                       ///< L R Ls Rs Cs C LFE
    kAudioChannelLayoutTag_Logic_6_1_C              = kAudioChannelLayoutTag_MPEG_6_1_A,    ///< L R C LFE Ls Rs Cs
    kAudioChannelLayoutTag_Logic_6_1_D              = (200U<<16) | 7,                       ///< L C R Ls Cs Rs LFE
    kAudioChannelLayoutTag_Logic_7_1_A              = kAudioChannelLayoutTag_AudioUnit_7_1, ///< L R C LFE Ls Rs Rls Rrs
    kAudioChannelLayoutTag_Logic_7_1_B              = (201U<<16) | 8,                       ///< L R Ls Rs Rls Rrs C LFE
    kAudioChannelLayoutTag_Logic_7_1_C              = kAudioChannelLayoutTag_MPEG_7_1_C,    ///< L R C LFE Ls Rs Rls Rrs
    kAudioChannelLayoutTag_Logic_7_1_SDDS_A         = kAudioChannelLayoutTag_MPEG_7_1_A,    ///< L R C LFE Ls Rs Lc Rc
    kAudioChannelLayoutTag_Logic_7_1_SDDS_B         = kAudioChannelLayoutTag_MPEG_7_1_B,    ///< C Lc Rc L R Ls Rs LFE
    kAudioChannelLayoutTag_Logic_7_1_SDDS_C         = kAudioChannelLayoutTag_Emagic_Default_7_1, ///< L R Ls Rs C LFE Lc Rc
    kAudioChannelLayoutTag_Logic_Atmos_5_1_2        = kAudioChannelLayoutTag_Atmos_5_1_2,   ///< L R C LFE Ls Rs Ltm Rtm
    kAudioChannelLayoutTag_Logic_Atmos_5_1_4        = kAudioChannelLayoutTag_Atmos_5_1_4,   ///< L R C LFE Ls Rs Vhl Vhr Ltr Rtr
    kAudioChannelLayoutTag_Logic_Atmos_7_1_2        = kAudioChannelLayoutTag_Atmos_7_1_2,   ///< L R C LFE Ls Rs Rls Rrs Ltm Rtm
    kAudioChannelLayoutTag_Logic_Atmos_7_1_4_A      = kAudioChannelLayoutTag_Atmos_7_1_4,   ///< L R C LFE Ls Rs Rls Rrs Vhl Vhr Ltr Rtr
    kAudioChannelLayoutTag_Logic_Atmos_7_1_4_B      = (202U<<16) | 12,                      ///< L R Rls Rrs Ls Rs C LFE Vhl Vhr Ltr Rtr
    kAudioChannelLayoutTag_Logic_Atmos_7_1_6        = (203U<<16) | 14,                      ///< L R Rls Rrs Ls Rs C LFE Vhl Vhr Ltm Rtm Ltr Rtr

    kAudioChannelLayoutTag_DiscreteInOrder          = (147U<<16) | 0,                       ///< needs to be ORed with the actual number of channels
	
    kAudioChannelLayoutTag_BeginReserved            = 0xF0000000,                           ///< Channel layout tag values in this range are reserved for internal use
    kAudioChannelLayoutTag_EndReserved              = 0xFFFEFFFF,
	
    kAudioChannelLayoutTag_Unknown                  = 0xFFFF0000                            ///< needs to be ORed with the actual number of channels
};

/*!
    @struct         AudioChannelDescription
    @abstract       This structure describes a single channel.
    @var            mChannelLabel
                        The AudioChannelLabel that describes the channel.
    @var            mChannelFlags
                        Flags that control the interpretation of mCoordinates.
    @var            mCoordinates
                        An ordered triple that specifies a precise speaker location.
*/
struct AudioChannelDescription
{
    AudioChannelLabel   mChannelLabel;
    AudioChannelFlags   mChannelFlags;
    Float32             mCoordinates[3];
};
typedef struct AudioChannelDescription AudioChannelDescription;

/*!
    @struct         AudioChannelLayout
    @abstract       This structure is used to specify channel layouts in files and hardware.
    @var            mChannelLayoutTag
                        The AudioChannelLayoutTag that indicates the layout.
    @var            mChannelBitmap
                        If mChannelLayoutTag is set to kAudioChannelLayoutTag_UseChannelBitmap, this
                        field is the channel usage bitmap.
    @var            mNumberChannelDescriptions
                        The number of items in the mChannelDescriptions array.
    @var            mChannelDescriptions
                        A variable length array of AudioChannelDescriptions that describe the
                        layout.
*/
struct AudioChannelLayout
{
    AudioChannelLayoutTag       mChannelLayoutTag;
    AudioChannelBitmap          mChannelBitmap;
    UInt32                      mNumberChannelDescriptions;
    AudioChannelDescription     mChannelDescriptions[1]; // this is a variable length array of mNumberChannelDescriptions elements
	
#if defined(__cplusplus) && defined(CA_STRICT) && CA_STRICT
public:
    AudioChannelLayout() {}
private:
    //  Copying and assigning a variable length struct is problematic so turn their use into a
    //  compile time error for easy spotting.
    AudioChannelLayout(const AudioChannelLayout&);
    AudioChannelLayout&         operator=(const AudioChannelLayout&);
#endif

};
typedef struct AudioChannelLayout AudioChannelLayout;

/*!
    @function       AudioChannelLayoutTag_GetNumberOfChannels
    @abstract       A macro to get the number of channels out of an AudioChannelLayoutTag
    @discussion     The low 16 bits of an AudioChannelLayoutTag gives the number of channels except
                    for kAudioChannelLayoutTag_UseChannelDescriptions and
                    kAudioChannelLayoutTag_UseChannelBitmap.
    @param          inLayoutTag
                        The AudioChannelLayoutTag to examine.
    @result         The number of channels the tag indicates.
*/
#ifdef CF_INLINE
    CF_INLINE UInt32    AudioChannelLayoutTag_GetNumberOfChannels(AudioChannelLayoutTag inLayoutTag)    { return (UInt32)(inLayoutTag & 0x0000FFFF); }
#else
    #define AudioChannelLayoutTag_GetNumberOfChannels(inLayoutTag) ((UInt32)((inLayoutTag) & 0x0000FFFF))
#endif

/*!
    @struct     AudioFormatListItem
    @abstract   this struct is used as output from the kAudioFormatProperty_FormatList property
    @var        mASBD
                    an AudioStreamBasicDescription
    @var        mChannelLayoutTag
                    an AudioChannelLayoutTag
*/
struct AudioFormatListItem
{
    AudioStreamBasicDescription      mASBD;
    AudioChannelLayoutTag            mChannelLayoutTag;
};
typedef struct AudioFormatListItem AudioFormatListItem;

// Deprecated constants

/*! @enum           MPEG4ObjectID
    @ deprecated    in version 10.5

    @abstract       Constants that describe the various kinds of MPEG-4 audio data.
    @discussion     These constants are used in the flags field of an AudioStreamBasicDescription
                    that describes an MPEG-4 audio stream.
*/
typedef CF_ENUM(long, MPEG4ObjectID)
{
    kMPEG4Object_AAC_Main       = 1,
    kMPEG4Object_AAC_LC         = 2,
    kMPEG4Object_AAC_SSR        = 3,
    kMPEG4Object_AAC_LTP        = 4,
    kMPEG4Object_AAC_SBR        = 5,
    kMPEG4Object_AAC_Scalable   = 6,
    kMPEG4Object_TwinVQ         = 7,
    kMPEG4Object_CELP           = 8,
    kMPEG4Object_HVXC           = 9
};

//==================================================================================================

#if defined(__cplusplus)
}
#endif

#pragma clang diagnostic pop

#endif // CoreAudioTypes_CoreAudioBaseTypes_h
