/*
	File:		Bluetooth.h
	Contains:	Public interfaces for Bluetooth technology.
	Copyright:	(c) 2010 by Apple, Inc. All rights reserved.
*/

#pragma once

#ifdef KERNEL
#import	<IOKit/bluetooth/BluetoothAssignedNumbers.h>
#else
#import	<CoreFoundation/CFBase.h>
#import	<IOBluetooth/BluetoothAssignedNumbers.h>
#endif

#import <IOKit/IOTypes.h>
#import <libkern/OSByteOrder.h>

//---------------------------------------------------------------------------------------------------------------------------
/*!	@header		Bluetooth
	Bluetooth wireless technology
*/

#if defined(__cplusplus)
	#define IOBLUETOOTH_EXPORT extern "C"
#else
	#define IOBLUETOOTH_EXPORT extern
#endif

#ifdef	__cplusplus
	extern "C" {
#endif


#if 0

#pragma mark === Baseband ===
#endif

//===========================================================================================================================
//	Baseband
//===========================================================================================================================

typedef uint16_t		BluetoothConnectionHandle;		// Upper 4 bits are reserved.
typedef uint8_t		BluetoothLMPHandle;
enum
{
    kBluetoothConnectionHandleNone	= 0xffff
};

typedef uint8_t		BluetoothReasonCode;
typedef uint8_t		BluetoothEncryptionEnable;
enum
{
	kBluetoothEncryptionEnableOff           = 0x00,
	kBluetoothEncryptionEnableOn            = 0x01,
    
    kBluetoothEncryptionEnableBREDRE0       = 0x01,
    kBluetoothEncryptionEnableLEAESCCM      = 0x01,
    kBluetoothEncryptionEnableBREDRAESCCM   = 0x02
};

typedef uint8_t		BluetoothKeyFlag;
enum
{
	kBluetoothKeyFlagSemiPermanent 	= 0x00, 
	kBluetoothKeyFlagTemporary 		= 0x01
};

typedef uint8_t		BluetoothKeyType;
enum
{
	kBluetoothKeyTypeCombination					= 0x00, 
	kBluetoothKeyTypeLocalUnit						= 0x01,
	kBluetoothKeyTypeRemoteUnit						= 0x02,
	kBluetoothKeyTypeDebugCombination				= 0x03,
	kBluetoothKeyTypeUnauthenticatedCombination		= 0x04,
	kBluetoothKeyTypeAuthenticatedCombination		= 0x05,
	kBluetoothKeyTypeChangedCombination				= 0x06,
    kBluetoothKeyTypeUnauthenticatedCombinationP256 = 0x07,
    kBluetoothKeyTypeAuthenticatedCombinationP256   = 0x08
};

// Packet types (Bluetooth spec section 7.1.5 - Create Connection)

typedef uint16_t		BluetoothPacketType;
enum
{
	kBluetoothPacketTypeReserved1	= 0x0001, 
	kBluetoothPacketType2DH1Omit	= 0x0002,	// Masks OUT this packet type
	kBluetoothPacketType3DH1Omit	= 0x0004,	// Masks OUT this packet type

	kBluetoothPacketTypeDM1			= 0x0008, 
	kBluetoothPacketTypeDH1			= 0x0010, 
	kBluetoothPacketTypeHV1			= 0x0020, 	// Reserved
	kBluetoothPacketTypeHV2			= 0x0040, 	// Reserved
	kBluetoothPacketTypeHV3			= 0x0080, 	// Reserved
	kBluetoothPacketTypeDV			= 0x0100, 	// Reserved
	kBluetoothPacketType2DH3Omit	= 0x0100,	// Masks OUT this packet type
	kBluetoothPacketType3DH3Omit	= 0x0200,	// Masks OUT this packet type
	kBluetoothPacketTypeAUX			= 0x0200,	// Deprecated
	
	kBluetoothPacketTypeDM3			= 0x0400, 
	kBluetoothPacketTypeDH3			= 0x0800,
	
	kBluetoothPacketType2DH5Omit	= 0x1000,	// Masks OUT this packet type
	kBluetoothPacketType3DM5Omit	= 0x2000,	// Masks OUT this packet type 
	
	kBluetoothPacketTypeDM5			= 0x4000, 
	kBluetoothPacketTypeDH5			= 0x8000, 
	
	kBluetoothPacketTypeEnd
};

// Setup Synchronous Packet types (Bluetooth 2.1 spec section 7.1.26 - Setup Synchronous Command)

enum
{
	kBluetoothSynchronousConnectionPacketTypeNone			= 0x0000,
	kBluetoothSynchronousConnectionPacketTypeHV1			= 0x0001,
	kBluetoothSynchronousConnectionPacketTypeHV2			= 0x0002,
	kBluetoothSynchronousConnectionPacketTypeHV3			= 0x0004,
	kBluetoothSynchronousConnectionPacketTypeEV3			= 0x0008,
	kBluetoothSynchronousConnectionPacketTypeEV4			= 0x0010,
	kBluetoothSynchronousConnectionPacketTypeEV5			= 0x0020,

	// masking out certain types:
	
	kBluetoothSynchronousConnectionPacketType2EV3Omit		= 0x0040,
	kBluetoothSynchronousConnectionPacketType3EV3Omit		= 0x0080,
	kBluetoothSynchronousConnectionPacketType2EV5Omit		= 0x0100,
	kBluetoothSynchronousConnectionPacketType3EV5Omit		= 0x0200,
	
	kBluetoothSynchronousConnectionPacketTypeFutureUse		= 0xFC00,
	
	kBluetoothSynchronousConnectionPacketTypeAll			= 0xFFFF,

	kBluetoothSynchronousConnectionPacketTypeEnd
};


// LAP/Inquiry Access Codes

typedef uint32_t		BluetoothLAP;
enum
{
	kBluetoothGeneralInquiryAccessCodeIndex		= 0, 			// General/Unlimited Inquiry Access Code (GIAC)
	kBluetoothGeneralInquiryAccessCodeLAPValue 	= 0x9E8B33, 	// General/Unlimited Inquiry Access Code (GIAC)
	
	kBluetoothLimitedInquiryAccessCodeIndex		= 1, 			// Limited Dedicated Inquiry Access Code (LIAC)
	kBluetoothLimitedInquiryAccessCodeLAPValue 	= 0x9E8B00, 	// Limited Dedicated Inquiry Access Code (LIAC)
	
	// All other access codes are reserved for future use (indices 2-63, LAP values 0x9E8B01-0x9E8B32 and 0x9E8B34-0x9E8B3F).
	
	kBluetoothLimitedInquiryAccessCodeEnd
};

// PageScanRepetitionMode

typedef	uint8_t	BluetoothPageScanRepetitionMode;
enum
{
	kBluetoothPageScanRepetitionModeR0 			= 0x00, 
	kBluetoothPageScanRepetitionModeR1 			= 0x01, 
	kBluetoothPageScanRepetitionModeR2 			= 0x02
	
	// All other values are reserved for future use.
};

// PageScanPeriodMode

typedef uint8_t	BluetoothPageScanPeriodMode;
enum
{
	kBluetoothPageScanPeriodModeP0 				= 0x00, 
	kBluetoothPageScanPeriodModeP1 				= 0x01, 
	kBluetoothPageScanPeriodModeP2 				= 0x02
	
	// All other values are reserved for future use.
};

// PageScanMode

typedef uint8_t	BluetoothPageScanMode;
enum
{
	kBluetoothPageScanModeMandatory 			= 0x00, 
	kBluetoothPageScanModeOptional1 			= 0x01, 
	kBluetoothPageScanModeOptional2 			= 0x02, 
	kBluetoothPageScanModeOptional3 			= 0x03
	
	// All other values are reserved for future use.
};
		
// PageScanType
		
typedef uint8_t	BluetoothHCIPageScanType;
enum BluetoothHCIPageScanTypes
{
	kBluetoothHCIPageScanTypeStandard		= 0x00,
	kBluetoothHCIPageScanTypeInterlaced		= 0x01,
			
	kBluetoothHCIPageScanTypeReservedStart	= 0x02,
	kBluetoothHCIPageScanTypeReservedEnd	= 0xFF
};	
		
// Erroneous Data Reporting
typedef uint8_t	BluetoothHCIErroneousDataReporting;
enum
{
	kBluetoothHCIErroneousDataReportingDisabled			= 0x00,
	kBluetoothHCIErroneousDataReportingEnabled			= 0x01,
			
	kBluetoothHCIErroneousDataReportingReservedStart	= 0x02,
	kBluetoothHCIErroneousDataReportingReservedEnd		= 0xFF
};		

#if 0
#pragma mark -
#pragma mark === Devices ===
#endif

typedef struct	BluetoothDeviceAddress		BluetoothDeviceAddress;
struct	BluetoothDeviceAddress
{
	uint8_t		data[ 6 ];
};

typedef struct	BluetoothKey				BluetoothKey;
struct	BluetoothKey
{
	uint8_t		data[ 16 ];
};

typedef struct  BluetoothIRK                BluetoothIRK;
struct BluetoothIRK
{
    uint8_t     data[ 16 ];
};
        
typedef struct	BluetoothPINCode			BluetoothPINCode;
struct	BluetoothPINCode
{
	uint8_t		data[ 16 ];		// PIN codes may be up to 128 bits.
};


//	Physical layout of the "class of device/service" field (see Bluetooth Assigned Numbers section 1.2):
//
//	 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//	 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  <- Bit Transmission Order
//	+---------------+---------------+---------------+
//	|    octet 3    |    octet 2    |    octet 1    | <- Octet Transmission Order
//	+---------------+---------------+---------------+
//	<------ 11 bits ----->< 5 bits ><- 6 bits ->
//	+---------------------+---------+-----------+-+-+
//	|   Service Classes   | Major   |  Minor    | | |
//	+-+-+-+-+-+-+-+-+-+-+-+  Device |   Device  |0|0|
//	| | | | | | | |*|*|*| |   Class |    Class  | | |
//	+-+-+-+-+-+-+-+-+-+-+-+---------+-----------+-+-+
//	 | | | | | | |       |                        |
//   | | | | | | |       + Limited Discoverable   +- Format Type
//	 | | | | | | +- Networking
//	 | | | | | +- Rendering
//	 | | | | +- Capturing
//	 | | | +- Object Transfer
//	 | | +- Audio
//	 | +- Telephony
//	 +- Information

typedef uint32_t		BluetoothClassOfDevice;

#define		BluetoothGetDeviceClassMajor( inCOD )		( ((inCOD) & 0x00001F00) >> 8 )
#define		BluetoothGetDeviceClassMinor( inCOD )		( ((inCOD) & 0x000000FC) >> 2 )
#define		BluetoothGetServiceClassMajor( inCOD )		( ((inCOD) & 0x00FFE000) >> 13 )
#define		BluetoothMakeClassOfDevice( inServiceClassMajor, inDeviceClassMajor, inDeviceClassMinor )		\
										((((inServiceClassMajor) << 13) & 0x00FFE000) | (((inDeviceClassMajor) << 8) & 0x00001F00) | (((inDeviceClassMinor) << 2) & 0x000000FC))

///
/// Major Service Classes (11-bit value - bits 13-23 of Device/Service field)
///

typedef uint32_t		BluetoothServiceClassMajor;
// Service Class Major enum in BluetoothAssignedNumbers.h

///
/// Major Device Classes (5-bit value - bits 8-12 of Device/Service field)
///

typedef uint32_t		BluetoothDeviceClassMajor;
// Device Class Major enum in BluetoothAssignedNumbers.h

///
/// Minor Device Classes (6-bit value - bits 2-7 of Device/Service field)
///

typedef uint32_t		BluetoothDeviceClassMinor;
// Device Class Minor enum in BluetoothAssignedNumbers.h

// Misc Device Types

enum
{
	kBluetoothDeviceNameMaxLength	= 248
};

typedef uint8_t		BluetoothDeviceName[ kBluetoothDeviceNameMaxLength ];		// Max 248 bytes of UTF-8 encoded Unicode.
typedef uint16_t	BluetoothClockOffset;										// Bits 14-0 come from bits 16-2 of CLKslav-CLKmaster.
typedef uint8_t		BluetoothRole;												//
typedef uint8_t		BluetoothAllowRoleSwitch;									// 0x00-0x01 valid, 0x02-0xFF reserved.
enum
{
	kBluetoothDontAllowRoleSwitch 	= 0x00, 
	kBluetoothAllowRoleSwitch 		= 0x01
};

enum
{
	kBluetoothRoleBecomeCentral		= 0x00,
	kBluetoothRoleRemainPeripheral 	= 0x01,
	kBluetoothRoleBecomeMaster __attribute__ ((deprecated)) = kBluetoothRoleBecomeCentral,
	kBluetoothRoleRemainSlave __attribute__ ((deprecated)) = kBluetoothRoleRemainPeripheral
};

typedef struct BluetoothSetEventMask	BluetoothSetEventMask;
struct	BluetoothSetEventMask
{
	uint8_t		data[ 8 ];
};

typedef uint8_t	BluetoothPINType;


#if 0
#pragma mark -
#pragma mark === L2CAP ===
#endif

//===========================================================================================================================
//	Logical Link Control and Adaptation Protocol (L2CAP)
//===========================================================================================================================


// ACL Packet values (Bluetooth L2CAP spec section 1).

enum
{
	kBluetoothL2CAPMaxPacketSize					= 65535,	// Max number of data bytes in an L2CAP packet.

	kBluetoothACLLogicalChannelReserved 			= 0, 		// [00] Reserved for future use
	kBluetoothACLLogicalChannelL2CAPContinue	 	= 1, 		// [01] Continuation of L2CAP packet.
	kBluetoothACLLogicalChannelL2CAPStart 			= 2, 		// [10] Start of L2CAP packet.
	kBluetoothACLLogicalChannelLMP	 				= 3			// [11] Link Manager Protocol packet.
};

// Channel Identifiers (Bluetooth L2CAP spec section 2.1).

typedef uint16_t		BluetoothL2CAPChannelID;
enum
{
	kBluetoothL2CAPChannelNull					= 0x0000, 	// Illegal, should not be used
	kBluetoothL2CAPChannelSignalling	 		= 0x0001, 	// L2CAP signalling channel
	kBluetoothL2CAPChannelConnectionLessData	= 0x0002, 	// L2CAP connection less data
	kBluetoothL2CAPChannelAMPManagerProtocol	= 0x0003,	// AMP Manager Protocol
	kBluetoothL2CAPChannelAttributeProtocol		= 0x0004,
	kBluetoothL2CAPChannelLESignalling			= 0x0005,
	kBluetoothL2CAPChannelSecurityManager		= 0x0006,
    kBluetoothL2CAPChannelBREDRSecurityManager  = 0x0007,
	// Range 0x0008 to 0x003E reserved for future use.
	kBluetoothL2CAPChannelReservedStart			= 0x0008,
    kBluetoothL2CAPChannelLEAP					= 0x002A,
    kBluetoothL2CAPChannelLEAS					= 0x002B,
    kBluetoothL2CAPChannelMagicPairing			= 0x0030,
	kBluetoothL2CAPChannelMagnet				= 0x003A,
    kBluetoothL2CAPChannelReservedEnd			= 0x003E,
    
    kBluetoothL2CAPChannelAMPTestManager        = 0x003F,
    
    // Range 0x0040 to 0xFFFF are dynamically allocated.
    kBluetoothL2CAPChannelDynamicStart			= 0x0040,
    kBluetoothL2CAPChannelDynamicEnd			= 0xffff,
	kBluetoothL2CAPChannelEnd					= 0xffff
};

typedef BluetoothL2CAPChannelID		BluetoothL2CAPGroupID;

// Protocol/Service Multiplexor (PSM) values (Bluetooth L2CAP spec section 5.2).

typedef uint16_t		BluetoothL2CAPPSM;
// PSM enum in BluetoothAssignedNumbers.h

// Command Codes

typedef enum
{
	kBluetoothL2CAPCommandCodeReserved							= 0x00, 
	kBluetoothL2CAPCommandCodeCommandReject						= 0x01, 
	kBluetoothL2CAPCommandCodeConnectionRequest					= 0x02, 
	kBluetoothL2CAPCommandCodeConnectionResponse				= 0x03, 
	kBluetoothL2CAPCommandCodeConfigureRequest					= 0x04, 
	kBluetoothL2CAPCommandCodeConfigureResponse					= 0x05, 
	kBluetoothL2CAPCommandCodeDisconnectionRequest				= 0x06, 
	kBluetoothL2CAPCommandCodeDisconnectionResponse				= 0x07, 
	kBluetoothL2CAPCommandCodeEchoRequest						= 0x08, 
	kBluetoothL2CAPCommandCodeEchoResponse						= 0x09, 
	kBluetoothL2CAPCommandCodeInformationRequest				= 0x0A, 
	kBluetoothL2CAPCommandCodeInformationResponse				= 0x0B,
	kBluetoothL2CAPCommandCodeCreateChannelRequest				= 0x0C,
	kBluetoothL2CAPCommandCodeCreateChannelResponse				= 0x0D,
	kBluetoothL2CAPCommandCodeMoveChannelRequest				= 0x0E,
	kBluetoothL2CAPCommandCodeMoveChannelResponse				= 0x0F,
	kBluetoothL2CAPCommandCodeMoveChannelConfirmation			= 0x10,
	kBluetoothL2CAPCommandCodeMoveChannelConfirmationResponse	= 0x11,
	kBluetoothL2CAPCommandCodeConnectionParameterUpdateRequest	= 0x12,
	kBluetoothL2CAPCommandCodeConnectionParameterUpdateResponse	= 0x13,
	kBluetoothL2CAPCommandCodeLECreditBasedConnectionRequest    = 0x14,
	kBluetoothL2CAPCommandCodeLECreditBasedConnectionResponse	= 0x15,
	kBluetoothL2CAPCommandCodeLEFlowControlCredit               = 0x16,
} BluetoothL2CAPCommandCode;

// Command Reject

typedef enum
{
	kBluetoothL2CAPCommandRejectReasonCommandNotUnderstood 	= 0x0000, 
	kBluetoothL2CAPCommandRejectReasonSignallingMTUExceeded = 0x0001, 
	kBluetoothL2CAPCommandRejectReasonInvalidCIDInRequest 	= 0x0002, 
} BluetoothL2CAPCommandRejectReason;

typedef uint16_t		BluetoothL2CAPMTU;
typedef	uint16_t		BluetoothL2CAPLinkTimeout;
typedef uint16_t		BluetoothL2CAPFlushTimeout;
enum
{
	kBluetoothL2CAPFlushTimeoutUseExisting 	= 0x0000, 
	kBluetoothL2CAPFlushTimeoutImmediate 	= 0x0001, 
	kBluetoothL2CAPFlushTimeoutForever 		= 0xFFFF, 
	
	kBluetoothL2CAPFlushTimeoutEnd
};

typedef struct BluetoothL2CAPQualityOfServiceOptions	BluetoothL2CAPQualityOfServiceOptions;
struct BluetoothL2CAPQualityOfServiceOptions
{
	uint8_t		flags;
	uint8_t		serviceType;
	uint32_t	tokenRate;
	uint32_t	tokenBucketSize;
	uint32_t	peakBandwidth;
	uint32_t	latency;
	uint32_t	delayVariation;
};

typedef struct BluetoothL2CAPRetransmissionAndFlowControlOptions BluetoothL2CAPRetransmissionAndFlowControlOptions;
struct BluetoothL2CAPRetransmissionAndFlowControlOptions
{
	uint8_t		flags;
	uint8_t		txWindowSize;
	uint8_t		maxTransmit;
	uint16_t	retransmissionTimeout;
	uint16_t	monitorTimeout;
	uint16_t	maxPDUPayloadSize;
};

typedef enum BluetoothL2CAPSegmentationAndReassembly {
    kBluetoothL2CAPSegmentationAndReassemblyUnsegmentedSDU    =   0x00,
    kBluetoothL2CAPSegmentationAndReassemblyStartOfSDU        =   0x01,
    kBluetoothL2CAPSegmentationAndReassemblyEndOfSDU          =   0x02,
    kBluetoothL2CAPSegmentationAndReassemblyContinuationOfSDU =   0x03
} BluetoothL2CAPSegmentationAndReassembly;
        
enum
{
	kBluetoothL2CAPInfoTypeMaxConnectionlessMTUSize = 0x0001
};

// Packets

enum
{
	kBluetoothL2CAPPacketHeaderSize = 4
};

typedef uint16_t	BluetoothL2CAPByteCount;
typedef uint8_t		BluetoothL2CAPCommandID;
typedef uint16_t	BluetoothL2CAPCommandByteCount;

typedef enum
{
    kBluetoothL2CAPConnectionResultSuccessful						= 0x0000,
    kBluetoothL2CAPConnectionResultPending							= 0x0001,
    kBluetoothL2CAPConnectionResultRefusedPSMNotSupported			= 0x0002,
    kBluetoothL2CAPConnectionResultRefusedSecurityBlock				= 0x0003,
    kBluetoothL2CAPConnectionResultRefusedNoResources				= 0x0004,
    kBluetoothL2CAPConnectionResultRefusedReserved					= 0x0005,
    kBluetoothL2CAPConnectionResultRefusedInvalidSourceCID			= 0x0006,
    kBluetoothL2CAPConnectionResultRefusedSourceCIDAlreadyAllocated	= 0x0007
} BluetoothL2CAPConnectionResult;

typedef enum 
{
    kBluetoothL2CAPConnectionStatusNoInfoAvailable			= 0x0000,
    kBluetoothL2CAPConnectionStatusAuthenticationPending	= 0x0001,
    kBluetoothL2CAPConnectionStatusAuthorizationPending		= 0x0002,
} BluetoothL2CAPConnectionStatus;

typedef enum
{
    kBluetoothL2CAPConfigurationResultSuccess					= 0x0000,
    kBluetoothL2CAPConfigurationResultUnacceptableParams		= 0x0001,
    kBluetoothL2CAPConfigurationResultRejected					= 0x0002,
    kBluetoothL2CAPConfigurationResultUnknownOptions			= 0x0003,
} BluetoothL2CAPConfigurationResult;

typedef enum
{
    kBluetoothL2CAPConfigurationOptionMTU                           = 0x01,
    kBluetoothL2CAPConfigurationOptionFlushTimeout                  = 0x02,
    kBluetoothL2CAPConfigurationOptionQoS                           = 0x03,
    kBluetoothL2CAPConfigurationOptionRetransmissionAndFlowControl  = 0x04,
    kBluetoothL2CAPConfigurationOptionFrameCheckSequence            = 0x05,
    kBluetoothL2CAPConfigurationOptionExtendedFlowSpecification     = 0x06,
    kBluetoothL2CAPConfigurationOptionExtendedWindowSize            = 0x07
} BluetoothL2CAPConfigurationOption;

enum
{
    kBluetoothL2CAPConfigurationOptionMTULength                             = 2,
    kBluetoothL2CAPConfigurationOptionFlushTimeoutLength                    = 2,
    kBluetoothL2CAPConfigurationOptionQoSLength                             = 22,
    kBluetoothL2CAPConfigurationOptionRetransmissionAndFlowControlLength    = 9
};

typedef enum
{
    kBluetoothL2CAPConfigurationBasicL2CAPModeFlag          = 0x00,
    kBluetoothL2CAPConfigurationRetransmissionModeFlag      = 0x01,
    kBluetoothL2CAPConfigurationFlowControlModeFlag         = 0x02,
    kBluetoothL2CAPConfigurationEnhancedRetransmissionMode  = 0x03,
    kBluetoothL2CAPConfigurationStreamingMode               = 0x04
} BluetoothL2CAPConfigurationRetransmissionAndFlowControlFlags;


typedef enum
{
    kBluetoothL2CAPInformationTypeConnectionlessMTU			= 0x0001,
    kBluetoothL2CAPInformationTypeExtendedFeatures			= 0x0002,
	kBluetoothL2CAPInformationTypeFixedChannelsSupported	= 0x0003,
} BluetoothL2CAPInformationType;

typedef enum
{
    kBluetoothL2CAPInformationResultSuccess			= 0x0000,
    kBluetoothL2CAPInformationResultNotSupported	= 0x0001,
} BluetoothL2CAPInformationResult;

typedef enum
{
    kBluetoothL2CAPInformationNoExtendedFeatures			= 0x00000000,
    kBluetoothL2CAPInformationFlowControlMode				= 0x00000001,
    kBluetoothL2CAPInformationRetransmissionMode			= 0x00000002,
    kBluetoothL2CAPInformationBidirectionalQoS				= 0x00000004,
	kBluetoothL2CAPInformationEnhancedRetransmissionMode	= 0x00000008,
	kBluetoothL2CAPInformationStreamingMode					= 0x00000010,
	kBluetoothL2CAPInformationFCSOption						= 0x00000020,
	kBluetoothL2CAPInformationExtendedFlowSpecification		= 0x00000040,
	kBluetoothL2CAPInformationFixedChannels					= 0x00000080,
	kBluetoothL2CAPInformationExtendedWindowSize			= 0x00000100,
	kBluetoothL2CAPUnicastConnectionlessDataReception		= 0x00000200,
} BluetoothL2CAPInformationExtendedFeaturesMask;

typedef enum
{
    kBluetoothL2CAPQoSTypeNoTraffic     = 0x00,
    kBluetoothL2CAPQoSTypeBestEffort	= 0x01,
    kBluetoothL2CAPQoSTypeGuaranteed	= 0x02,
} BluetoothL2CAPQoSType;

typedef enum {
    kBluetoothL2CAPSupervisoryFuctionTypeReceiverReady			= 0x0,
    kBluetoothL2CAPSupervisoryFuctionTypeReject					= 0x1,
    kBluetoothL2CAPSupervisoryFuctionTypeReceiverNotReady		= 0x2,
    kBluetoothL2CAPSupervisoryFuctionTypeSelectiveReject		= 0x3
} BluetoothL2CAPSupervisoryFuctionType;
        
enum {
	kBluetoothLETXTimeMin			= 0x0148,   // 328 us / 41 octets / 27 (0x1B) byte PDU
	kBluetoothLETXTimeDefault		= 0x0148,   // 328 us
	kBluetoothLETXTimeMax			= 0x0848,   // 2120 us / 265 octets / 251 (0xFB) byte PDU
	kBluetoothLETXOctetsMin			= 0x001b,   // 27 bytes pre-LE DPLE size
	kBluetoothLETXOctetsDefault		= 0x001b,   // 27 bytes
	kBluetoothLETXOctetsMax			= 0x00fb,   // 251 bytes 4.2 LE DPLE feature
};

enum
{
//	kBluetoothL2CAPMTULowEnergyDefault			= 0x0080,								// 128 bytes - DPLE feature
	kBluetoothL2CAPMTULowEnergyDefault			= kBluetoothLETXOctetsMin,				// 27 bytes
	kBluetoothL2CAPMTULowEnergyMax              = kBluetoothLETXOctetsMax,				// 251 bytes
	kBluetoothL2CAPMTUMinimum					= 0x0030,								// 48 bytes
	kBluetoothL2CAPMTUDefault					= 0x03F9,								// 11.10.08 - dropped back to 1017 from 1021 (don't aggravate the 3DH5 problem between CSR<->BRCM just yet)
	kBluetoothL2CAPMTUMaximum					= 0xffff,	
	kBluetoothL2CAPMTUStart						= 0x7fff,
	kBluetoothL2CAPMTUSIG						= 0x0030,								// 48 bytes
	kBluetoothL2CAPFlushTimeoutDefault			= kBluetoothL2CAPFlushTimeoutForever,	// 0xffff
	kBluetoothL2CAPQoSFlagsDefault				= 0,
	kBluetoothL2CAPQoSTypeDefault				= kBluetoothL2CAPQoSTypeBestEffort,		// 0x01
	kBluetoothL2CAPQoSTokenRateDefault			= 0x00000000,
	kBluetoothL2CAPQoSTokenBucketSizeDefault	= 0x00000000,
	kBluetoothL2CAPQoSPeakBandwidthDefault		= 0x00000000,
	kBluetoothL2CAPQoSLatencyDefault			= 0xffffffff,
	kBluetoothL2CAPQoSDelayVariationDefault		= 0xffffffff
};

enum {
    kBluetoothLEMaxTXTimeMin = 0x0148,
    kBluetoothLEMaxTXTimeDefault = kBluetoothL2CAPMTULowEnergyDefault,
    kBluetoothLEMaxTXTimeMax = 0x0848,
    kBluetoothLEMaxTXOctetsMin              = 0x001b,	// 27 bytes
    kBluetoothLEMaxTXOctetsDefault			= 0x0080,	// 128 bytes - DPLE feature
    kBluetoothLEMaxTXOctetsMax              = 0x00fb,	// 251 bytes - DPLE feature
};
        

#pragma mark === LE Security Manager ===

#define kBluetoothLESMPTimeout 30		
		
#define kBluetoothLESMPMinEncryptionKeySize 7
#define kBluetoothLESMPMaxEncryptionKeySize	16
		
enum BluetoothLESecurityManagerKeyDistributionFormat
{
	kBluetoothLESecurityManagerEncryptionKey			= (1 << 0L), // LTK, EDIV, RAND
	kBluetoothLESecurityManagerIDKey					= (1 << 1L), // IRK 
	kBluetoothLESecurityManagerSignKey					= (1 << 2L), // CSRK
    kBluetoothLESecurityManagerLinkKey					= (1 << 3L), // LinkKey
};				
		
typedef enum
{
	kBluetoothLESecurityManagerCommandCodeReserved                      = 0x00,
	kBluetoothLESecurityManagerCommandCodePairingRequest                = 0x01,
	kBluetoothLESecurityManagerCommandCodePairingResponse               = 0x02,
	kBluetoothLESecurityManagerCommandCodePairingConfirm                = 0x03,
	kBluetoothLESecurityManagerCommandCodePairingRandom                 = 0x04,
	kBluetoothLESecurityManagerCommandCodePairingFailed                 = 0x05,
	kBluetoothLESecurityManagerCommandCodeEncryptionInfo                = 0x06,
	kBluetoothLESecurityManagerCommandCodeMasterIdentification          = 0x07,
	kBluetoothLESecurityManagerCommandCodeIdentityInfo                  = 0x08,
	kBluetoothLESecurityManagerCommandCodeIdentityAddressInfo           = 0x09,
	kBluetoothLESecurityManagerCommandCodeSigningInfo                   = 0x0A,
    kBluetoothLESecurityManagerCommandCodeSecurityRequest               = 0x0B,
    kBluetoothLESecurityManagerCommandCodePairingPublicKey              = 0x0C,
    kBluetoothLESecurityManagerCommandCodePairingDHKeyCheck             = 0x0D,
    kBluetoothLESecurityManagerCommandCodePairingKeypressNotification	= 0x0E,
    
	// 0x0C - 0xFF reserved
	kBluetoothLESecurityManagerCommandCodeReservedStart                 = 0x0F,
	kBluetoothLESecurityManagerCommandCodeReservedEnd                   = 0xFF
} BluetoothLESecurityManagerCommandCode;

typedef enum
{
	kBluetoothLESecurityManagerUserInputCapabilityNoInput			= 0x01,
	kBluetoothLESecurityManagerUserInputCapabilityYesNo				= 0x02,
	kBluetoothLESecurityManagerUserInputCapabilityKeyboard			= 0x03
} BluetoothLESecurityManagerUserInputCapability;

typedef enum
{
	kBluetoothLESecurityManagerUserOutputCapabilityNoOutput			= 0x01,
	kBluetoothLESecurityManagerUserOutputCapabilityNumericOutput	= 0x02
} BluetoothLESecurityManagerUserOutputCapability;

typedef enum
{
	kBluetoothLESecurityManagerIOCapabilityDisplayOnly				= 0x00,
	kBluetoothLESecurityManagerIOCapabilityDisplayYesNo				= 0x01,
	kBluetoothLESecurityManagerIOCapabilityKeyboardOnly				= 0x02,
	kBluetoothLESecurityManagerIOCapabilityNoInputNoOutput          = 0x03,
	kBluetoothLESecurityManagerIOCapabilityKeyboardDisplay          = 0x04,
	
	// 0x05 - 0xFF reserved
	kBluetoothLESecurityManagerIOCapabilityReservedStart            = 0x05,
	kBluetoothLESecurityManagerIOCapabilityReservedEnd				= 0xFF
} BluetoothLESecurityManagerIOCapability;

typedef enum
{
	kBluetoothLESecurityManagerOOBAuthenticationDataNotPresent      = 0x00,
	kBluetoothLESecurityManagerOOBAuthenticationDataPresent         = 0x01,
	
	// 0x02 - 0xFF reserved
	kBluetoothLESecurityManagerOOBDataReservedStart					= 0x02,
	kBluetoothLESecurityManagerOOBDataReservedEnd					= 0xFF,
} BluetoothLESecurityManagerOOBData;

enum
{
	kBluetoothLESecurityManagerNoBonding							= 0,    // [00]
	kBluetoothLESecurityManagerBonding								= 1,    // [01]
	kBluetoothLESecurityManagerReservedStart						= 2,    // [10]
	kBluetoothLESecurityManagerReservedEnd							= 3     // [11]
};

typedef enum
{
	kBluetoothLESecurityManagerReasonCodeReserved                                           = 0x00,
	kBluetoothLESecurityManagerReasonCodePasskeyEntryFailed                                 = 0x01,
	kBluetoothLESecurityManagerReasonCodeOOBNotAvailbale                                    = 0x02,
	kBluetoothLESecurityManagerReasonCodeAuthenticationRequirements                         = 0x03,
	kBluetoothLESecurityManagerReasonCodeConfirmValueFailed                                 = 0x04,
	kBluetoothLESecurityManagerReasonCodePairingNotSupported                                = 0x05,
	kBluetoothLESecurityManagerReasonCodeEncryptionKeySize                                  = 0x06,
	kBluetoothLESecurityManagerReasonCodeCommandNotSupported                                = 0x07,
	kBluetoothLESecurityManagerReasonCodeUnspecifiedReason                                  = 0x08,
	kBluetoothLESecurityManagerReasonCodeRepeatedAttempts                                   = 0x09,
    kBluetoothLESecurityManagerReasonCodeInvalidParameters                                  = 0x0A,
    kBluetoothLESecurityManagerReasonCodeDHKeyCheckFailed                                   = 0x0B,
    kBluetoothLESecurityManagerReasonCodeNumericComparisonFailed                            = 0x0C,
    kBluetoothLESecurityManagerReasonCodeBREDRPairingInProgress                             = 0x0D,
    kBluetoothLESecurityManagerReasonCodeCrossTransportKeyDerivationGenerationNotAllowed    = 0x0E,
    
	kBluetoothLESecurityManagerReasonCodeReservedStart                                      = 0x0F,
	kBluetoothLESecurityManagerReasonCodeReservedEnd                                        = 0xFF
} BluetoothLESecurityManagerPairingFailedReasonCode;
        
typedef enum
{
    kBluetoothLESecurityManagerNotificationTypePasskeyEntryStarted      = 0,
    kBluetoothLESecurityManagerNotificationTypePasskeyDigitEntered      = 1,
    kBluetoothLESecurityManagerNotificationTypePasskeyDigitErased       = 2,
    kBluetoothLESecurityManagerNotificationTypePasskeyCleared           = 3,
    kBluetoothLESecurityManagerNotificationTypePasskeyEntryCompleted    = 4,
    
    kBluetoothLESecurityManagerNotificationTypeReservedStart            = 5,
    kBluetoothLESecurityManagerNotificationTypeReservedEnd              = 255,
}  BluetoothLESecurityManagerKeypressNotificationType;

		
		

#pragma mark === AMP Manager ===
typedef enum {
	kBluetoothAMPManagerCodeReserved							= 0x00,
	kBluetoothAMPManagerCodeAMPCommandReject					= 0x01,
	kBluetoothAMPManagerCodeAMPDiscoverRequest					= 0x02,
	kBluetoothAMPManagerCodeAMPDiscoverResponse					= 0x03,
	kBluetoothAMPManagerCodeAMPChangeNotify						= 0x04,
	kBluetoothAMPManagerCodeAMPChangeResponse					= 0x05,
	kBluetoothAMPManagerCodeAMPGetInfoRequest					= 0x06,
	kBluetoothAMPManagerCodeAMPGetInfoResponse					= 0x07,
	kBluetoothAMPManagerCodeAMPGetAssocRequest					= 0x08,
	kBluetoothAMPManagerCodeAMPGetAssocResponse					= 0x09,
	kBluetoothAMPManagerCodeAMPCreatePhysicalLinkRequest		= 0x0A,
	kBluetoothAMPManagerCodeAMPCreatePhysicalLinkResponse		= 0x0B,
	kBluetoothAMPManagerCodeAMPDisconnectPhysicalLinkRequest	= 0x0C,
	kBluetoothAMPManagerCodeAMPDisconnectPhysicalLinkResponse	= 0x0D,
} BluetoothAMPManagerCode;
		
typedef enum {
	kBluetoothAMPManagerCommandRejectReasonCommandNotRecognized	= 0x0000,
} BluetoothAMPCommandRejectReason;
		
typedef enum {
	kBluetoothAMPManagerDiscoverResponseControllerStatusPoweredDown		= 0x00,
	kBluetoothAMPManagerDiscoverResponseControllerStatusBluetoothOnly	= 0x01,
	kBluetoothAMPManagerDiscoverResponseControllerStatusNoCapacity		= 0x02,
	kBluetoothAMPManagerDiscoverResponseControllerStatusLowCapacity		= 0x03,
	kBluetoothAMPManagerDiscoverResponseControllerStatusMediumCapacity	= 0x04,
	kBluetoothAMPManagerDiscoverResponseControllerStatusHighCapacity	= 0x05,
	kBluetoothAMPManagerDiscoverResponseControllerStatusFullCapacity	= 0x06,
} BluetoothAMPDiscoverResponseControllerStatus;
		
typedef enum {
	kBluetoothAMPManagerGetInfoResponseSuccess				= 0x00,
	kBluetoothAMPManagerGetInfoResponseInvalidControllerID	= 0x01,
} BluetoothAMPGetInfoResponseStatus;
		
typedef enum {
	kBluetoothAMPManagerGetAssocResponseSuccess				= 0x00,
	kBluetoothAMPManagerGetAssocResponseInvalidControllerID	= 0x01,
} BluetoothAMPGetAssocResponseStatus;
		
typedef enum {
	kBluetoothAMPManagerCreatePhysicalLinkResponseSuccess										= 0x00,
	kBluetoothAMPManagerCreatePhysicalLinkResponseInvalidControllerID							= 0x01,
	kBluetoothAMPManagerCreatePhysicalLinkResponseUnableToStartLinkCreation						= 0x02,
	kBluetoothAMPManagerCreatePhysicalLinkResponseCollisionOccurred								= 0x03,
	kBluetoothAMPManagerCreatePhysicalLinkResponseAMPDisconnectedPhysicalLinkRequestReceived	= 0x04,
	kBluetoothAMPManagerCreatePhysicalLinkResponsePhysicalLinkAlreadyExists						= 0x05,
	kBluetoothAMPManagerCreatePhysicalLinkResponseSecurityViolation								= 0x06,
} BluetoothAMPCreatePhysicalLinkResponseStatus;
		
typedef enum {
	kBluetoothAMPManagerDisconnectPhysicalLinkResponseSuccess				= 0x00,
	kBluetoothAMPManagerDisconnectPhysicalLinkResponseInvalidControllerID	= 0x01,
	kBluetoothAMPManagerDisconnectPhysicalLinkResponseNoPhysicalLink		= 0x02,
} BluetoothAMPDisconnectPhysicalLinkResponseStatus;

#if 0
#pragma mark -
#pragma mark === HCI ===
#endif

//===========================================================================================================================
//	Host Controller Interface (HCI)
//===========================================================================================================================

//	HCI Command Packet
//	------------------
//	
//	                     1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
//	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//	+--------------------------------+---------------+--------------+
//	|            OpCode              |               |              |
//	+--------------------+-----------| Param Length  |  Params...   |
//	|         OCF        |    OGF    |               |              |
//	+--------------------+-----------+---------------+--------------+
//	|                                                               |
//	|                         Params...                             |
//	|                                                               |
//	+---------------------------------------------------------------+

// Commands
		
typedef uint8_t         BluetoothHCICommandOpCodeGroup;
typedef uint16_t		BluetoothHCICommandOpCodeCommand;
typedef uint16_t		BluetoothHCICommandOpCode;
typedef	uint32_t		BluetoothHCIVendorCommandSelector;

#define	BluetoothHCIMakeCommandOpCode( GROUP, CMD )				( ( ( ( GROUP ) & 0x003F ) << 10 ) | ( ( CMD ) & 0x03FF ) )
#define	BluetoothHCIMakeCommandOpCodeEndianSwap( GROUP, CMD )	( CFSwapInt16HostToLittle ( BluetoothHCIMakeCommandOpCode( GROUP, CMD ) ) )
#define	BluetoothHCIExtractCommandOpCodeGroup( OPCODE )			( ( ( OPCODE ) >> 10 ) & 0x003F )
#define	BluetoothHCIExtractCommandOpCodeCommand( OPCODE )		( ( OPCODE ) & 0x03FF )

#define BluetoothHCIMakeCommandOpCodeHostOrder(GROUP, CMD )	OSSwapLittleToHostConstInt16( ( ( ( GROUP ) & 0x003F ) << 10 ) | ( ( CMD ) & 0x03FF ) )


enum
{
	// Command Group: NoOp
	
	kBluetoothHCIOpCodeNoOp												= 0, 
	kBluetoothHCICommandGroupNoOp										= 0x00, 
		kBluetoothHCICommandNoOp										= 0x0000,
	
	// Command Group: Link Control
	
	kBluetoothHCICommandGroupLinkControl								= 0x01, 
		kBluetoothHCICommandInquiry										= 0x0001, 
		kBluetoothHCICommandInquiryCancel								= 0x0002, 
		kBluetoothHCICommandPeriodicInquiryMode							= 0x0003, 
		kBluetoothHCICommandExitPeriodicInquiryMode						= 0x0004, 
		kBluetoothHCICommandCreateConnection							= 0x0005, 
		kBluetoothHCICommandDisconnect									= 0x0006, 
		kBluetoothHCICommandAddSCOConnection							= 0x0007,	// DEPRECATED
		kBluetoothHCICommandCreateConnectionCancel						= 0x0008,
		kBluetoothHCICommandAcceptConnectionRequest						= 0x0009, 
		kBluetoothHCICommandRejectConnectionRequest						= 0x000A, 
		kBluetoothHCICommandLinkKeyRequestReply							= 0x000B, 
		kBluetoothHCICommandLinkKeyRequestNegativeReply					= 0x000C, 
		kBluetoothHCICommandPINCodeRequestReply							= 0x000D, 
		kBluetoothHCICommandPINCodeRequestNegativeReply					= 0x000E, 
		kBluetoothHCICommandChangeConnectionPacketType					= 0x000F, 
		kBluetoothHCICommandAuthenticationRequested						= 0x0011, 
		kBluetoothHCICommandSetConnectionEncryption						= 0x0013, 
		kBluetoothHCICommandChangeConnectionLinkKey						= 0x0015, 
		kBluetoothHCICommandMasterLinkKey								= 0x0017, 
		kBluetoothHCICommandRemoteNameRequest							= 0x0019, 
		kBluetoothHCICommandReadRemoteSupportedFeatures					= 0x001B, 
		kBluetoothHCICommandReadRemoteExtendedFeatures					= 0x001C, 
		kBluetoothHCICommandReadRemoteVersionInformation				= 0x001D, 
		kBluetoothHCICommandReadClockOffset								= 0x001F, 
		kBluetoothHCICommandRemoteNameRequestCancel						= 0x001A,
		kBluetoothHCICommandReadLMPHandle								= 0x0020,
		kBluetoothHCICommandSetupSynchronousConnection					= 0x0028,
		kBluetoothHCICommandAcceptSynchronousConnectionRequest			= 0x0029,
		kBluetoothHCICommandRejectSynchronousConnectionRequest			= 0x002A,
		kBluetoothHCICommandIOCapabilityRequestReply					= 0x002B,
		kBluetoothHCICommandUserConfirmationRequestReply				= 0x002C,
		kBluetoothHCICommandUserConfirmationRequestNegativeReply		= 0x002D,
		kBluetoothHCICommandUserPasskeyRequestReply						= 0x002E,
		kBluetoothHCICommandUserPasskeyRequestNegativeReply				= 0x002F,
		kBluetoothHCICommandRemoteOOBDataRequestReply					= 0x0030,
		kBluetoothHCICommandRemoteOOBDataRequestNegativeReply			= 0x0033,
		kBluetoothHCICommandIOCapabilityRequestNegativeReply			= 0x0034,
		kBluetoothHCICommandEnhancedSetupSynchronousConnection			= 0x003D,
		kBluetoothHCICommandEnhancedAcceptSynchronousConnectionRequest	= 0x003E,
		kBluetoothHCICommandTruncatedPage								= 0x003F,
		kBluetoothHCICommandTruncatedPageCancel							= 0x0040,
		kBluetoothHCICommandSetConnectionlessPeripheralBroadcast		= 0x0041,
		kBluetoothHCICommandSetConnectionlessPeripheralBroadcastReceive	= 0x0042,
		kBluetoothHCICommandStartSynchronizationTrain					= 0x0043,
		kBluetoothHCICommandReceiveSynchronizationTrain					= 0x0044,
		kBluetoothHCICommandRemoteOOBExtendedDataRequestReply			= 0x0045,
		kBluetoothHCICommandSetConnectionlessSlaveBroadcast __attribute__ ((deprecated)) = kBluetoothHCICommandSetConnectionlessPeripheralBroadcast,
		kBluetoothHCICommandSetConnectionlessSlaveBroadcastReceive __attribute__ ((deprecated))	= kBluetoothHCICommandSetConnectionlessPeripheralBroadcastReceive,

	// Command Group: Link Policy
	
	kBluetoothHCICommandGroupLinkPolicy									= 0x02, 
		kBluetoothHCICommandHoldMode									= 0x0001, 
		kBluetoothHCICommandSniffMode									= 0x0003, 
		kBluetoothHCICommandExitSniffMode								= 0x0004, 
		kBluetoothHCICommandParkMode									= 0x0005, 
		kBluetoothHCICommandExitParkMode								= 0x0006, 
		kBluetoothHCICommandQoSSetup									= 0x0007, 
		kBluetoothHCICommandRoleDiscovery								= 0x0009, 
		kBluetoothHCICommandSwitchRole									= 0x000B, 
		kBluetoothHCICommandReadLinkPolicySettings						= 0x000C, 
		kBluetoothHCICommandWriteLinkPolicySettings						= 0x000D, 
		kBluetoothHCICommandReadDefaultLinkPolicySettings				= 0x000E,
		kBluetoothHCICommandWriteDefaultLinkPolicySettings				= 0x000F,
		kBluetoothHCICommandFlowSpecification							= 0x0010,
		kBluetoothHCICommandSniffSubrating								= 0x0011,
		kBluetoothHCICommandAcceptSniffRequest							= 0x0031,
		kBluetoothHCICommandRejectSniffRequest							= 0x0032,
	
	// Command Group: Host Controller & Baseband
	
	kBluetoothHCICommandGroupHostController								= 0x03, 
		kBluetoothHCICommandSetEventMask								= 0x0001, 
		kBluetoothHCICommandReset										= 0x0003, 
		kBluetoothHCICommandSetEventFilter								= 0x0005, 
		kBluetoothHCICommandFlush										= 0x0008, 
		kBluetoothHCICommandReadPINType									= 0x0009, 
		kBluetoothHCICommandWritePINType								= 0x000A, 
		kBluetoothHCICommandCreateNewUnitKey							= 0x000B, 
		kBluetoothHCICommandReadStoredLinkKey							= 0x000D, 
		kBluetoothHCICommandWriteStoredLinkKey							= 0x0011, 
		kBluetoothHCICommandDeleteStoredLinkKey							= 0x0012, 
		kBluetoothHCICommandChangeLocalName								= 0x0013, 
		kBluetoothHCICommandReadLocalName								= 0x0014, 
		kBluetoothHCICommandReadConnectionAcceptTimeout					= 0x0015, 
		kBluetoothHCICommandWriteConnectionAcceptTimeout				= 0x0016, 
		kBluetoothHCICommandReadPageTimeout								= 0x0017, 
		kBluetoothHCICommandWritePageTimeout							= 0x0018, 
		kBluetoothHCICommandReadScanEnable								= 0x0019, 
		kBluetoothHCICommandWriteScanEnable								= 0x001A, 
		kBluetoothHCICommandReadPageScanActivity						= 0x001B, 
		kBluetoothHCICommandWritePageScanActivity						= 0x001C, 
		kBluetoothHCICommandReadInquiryScanActivity						= 0x001D, 
		kBluetoothHCICommandWriteInquiryScanActivity					= 0x001E, 
		kBluetoothHCICommandReadAuthenticationEnable					= 0x001F, 
		kBluetoothHCICommandWriteAuthenticationEnable					= 0x0020, 
		kBluetoothHCICommandReadEncryptionMode							= 0x0021,	// DEPRECATED
		kBluetoothHCICommandWriteEncryptionMode							= 0x0022,	// DEPRECATED
		kBluetoothHCICommandReadClassOfDevice							= 0x0023,
		kBluetoothHCICommandWriteClassOfDevice							= 0x0024, 
		kBluetoothHCICommandReadVoiceSetting							= 0x0025,
		kBluetoothHCICommandWriteVoiceSetting							= 0x0026, 
		kBluetoothHCICommandReadAutomaticFlushTimeout					= 0x0027, 
		kBluetoothHCICommandWriteAutomaticFlushTimeout					= 0x0028, 
		kBluetoothHCICommandReadNumberOfBroadcastRetransmissions		= 0x0029, 
		kBluetoothHCICommandWriteNumberOfBroadcastRetransmissions		= 0x002A, 
		kBluetoothHCICommandReadHoldModeActivity						= 0x002B, 
		kBluetoothHCICommandWriteHoldModeActivity						= 0x002C, 
		kBluetoothHCICommandReadTransmitPowerLevel						= 0x002D, 
		kBluetoothHCICommandReadSCOFlowControlEnable					= 0x002E, 
		kBluetoothHCICommandWriteSCOFlowControlEnable					= 0x002F, 
		kBluetoothHCICommandSetHostControllerToHostFlowControl			= 0x0031, 
		kBluetoothHCICommandHostBufferSize								= 0x0033, 
		kBluetoothHCICommandHostNumberOfCompletedPackets				= 0x0035, 
		kBluetoothHCICommandReadLinkSupervisionTimeout					= 0x0036, 
		kBluetoothHCICommandWriteLinkSupervisionTimeout					= 0x0037, 
		kBluetoothHCICommandReadNumberOfSupportedIAC					= 0x0038, 
		kBluetoothHCICommandReadCurrentIACLAP							= 0x0039, 
		kBluetoothHCICommandWriteCurrentIACLAP							= 0x003A, 
		kBluetoothHCICommandReadPageScanPeriodMode						= 0x003B,	// DEPRECATED 
		kBluetoothHCICommandWritePageScanPeriodMode						= 0x003C,	// DEPRECATED 
		kBluetoothHCICommandReadPageScanMode							= 0x003D,	// DEPRECATED 
		kBluetoothHCICommandWritePageScanMode							= 0x003E,	// DEPRECATED 
		kBluetoothHCICommandSetAFHClassification						= 0x003F, 		
		kBluetoothHCICommandReadInquiryScanType							= 0x0042, 		
		kBluetoothHCICommandWriteInquiryScanType						= 0x0043, 		
		kBluetoothHCICommandReadInquiryMode								= 0x0044, 		
		kBluetoothHCICommandWriteInquiryMode							= 0x0045,
		kBluetoothHCICommandReadPageScanType							= 0x0046,
		kBluetoothHCICommandWritePageScanType							= 0x0047,
		kBluetoothHCICommandReadAFHChannelAssessmentMode				= 0x0048,
		kBluetoothHCICommandWriteAFHChannelAssessmentMode				= 0x0049,
		kBluetoothHCICommandReadExtendedInquiryResponse					= 0x0051, 		
		kBluetoothHCICommandWriteExtendedInquiryResponse				= 0x0052,
		kBluetoothHCICommandRefreshEncryptionKey						= 0x0053,
		kBluetoothHCICommandReadSimplePairingMode						= 0x0055,
		kBluetoothHCICommandWriteSimplePairingMode						= 0x0056,
		kBluetoothHCICommandReadLocalOOBData							= 0x0057,
		kBluetoothHCICommandReadInquiryResponseTransmitPower			= 0x0058,
		kBluetoothHCICommandWriteInquiryResponseTransmitPower			= 0x0059,
		kBluetoothHCICommandSendKeypressNotification					= 0x0060,
		kBluetoothHCICommandReadDefaultErroneousDataReporting			= 0x005A,
		kBluetoothHCICommandWriteDefaultErroneousDataReporting			= 0x005B,
		kBluetoothHCICommandEnhancedFlush								= 0x005F,
		kBluetoothHCICommandReadLogicalLinkAcceptTimeout				= 0x0061,
		kBluetoothHCICommandWriteLogicalLinkAcceptTimeout				= 0x0062,
		kBluetoothHCICommandSetEventMaskPageTwo							= 0x0063,
		kBluetoothHCICommandReadLocationData							= 0x0064,
		kBluetoothHCICommandWriteLocationData							= 0x0065,
		kBluetoothHCICommandReadFlowControlMode							= 0x0066,
		kBluetoothHCICommandWriteFlowControlMode						= 0x0067,
		kBluetoothHCICommandReadEnhancedTransmitPowerLevel				= 0x0068,
		kBluetoothHCICommandReadBestEffortFlushTimeout					= 0x0069,
		kBluetoothHCICommandWriteBestEffortFlushTimeout					= 0x006A,
		kBluetoothHCICommandShortRangeMode								= 0x006B,
		kBluetoothHCICommandReadLEHostSupported							= 0x006C,
		kBluetoothHCICommandWriteLEHostSupported						= 0x006D,
		kBluetoothHCICommandSetMWSChannelParameters						= 0x006E,
		kBluetoothHCICommandSetExternalFrameConfiguration				= 0x006F,
		kBluetoothHCICommandSetMWSSignaling								= 0x0070,
		kBluetoothHCICommandSetMWSTransportLayer						= 0x0071,
		kBluetoothHCICommandSetMWSScanFrequencyTable					= 0x0072,
		kBluetoothHCICommandSetMWSPATTERNConfiguration					= 0x0073,
		kBluetoothHCICommandSetReservedLTADDR							= 0x0074,
		kBluetoothHCICommandDeleteReservedLTADDR						= 0x0075,
		kBluetoothHCICommandSetConnectionlessPeripheralBroadcastData	= 0x0076,
		kBluetoothHCICommandReadSynchronizationTrainParameters			= 0x0077,
		kBluetoothHCICommandWriteSynchronizationTrainParameters			= 0x0078,
		kBluetoothHCICommandReadSecureConnectionsHostSupport			= 0x0079,
		kBluetoothHCICommandWriteSecureConnectionsHostSupport			= 0x007A,
		kBluetoothHCICommandReadAuthenticatedPayloadTimeout				= 0x007B,
		kBluetoothHCICommandWriteAuthenticatedPayloadTimeout			= 0x007C,
		kBluetoothHCICommandReadLocalOOBExtendedData					= 0x007D,
		kBluetoothHCICommandReadExtendedPageTimeout						= 0x007E,
		kBluetoothHCICommandWriteExtendedPageTimeout					= 0x007F,
		kBluetoothHCICommandReadExtendedInquiryLength					= 0x0080,
		kBluetoothHCICommandWriteExtendedInquiryLength					= 0x0081,
		kBluetoothHCICommandSetConnectionlessSlaveBroadcastData __attribute__ ((deprecated)) = kBluetoothHCICommandSetConnectionlessPeripheralBroadcastData,

	// Command Group: Informational
	
	kBluetoothHCICommandGroupInformational								= 0x04, 
		kBluetoothHCICommandReadLocalVersionInformation					= 0x0001, 
		kBluetoothHCICommandReadLocalSupportedCommands					= 0x0002, 
		kBluetoothHCICommandReadLocalSupportedFeatures					= 0x0003, 
		kBluetoothHCICommandReadLocalExtendedFeatures					= 0x0004,
		kBluetoothHCICommandReadBufferSize								= 0x0005, 
		kBluetoothHCICommandReadCountryCode								= 0x0007,	// DEPRECATED 
		kBluetoothHCICommandReadDeviceAddress							= 0x0009,
		kBluetoothHCICommandReadDataBlockSize							= 0x000A,
		kBluetoothHCICommandReadLocalSupportedCodecs					= 0x000B,
	
	// Command Group: Status
	
	kBluetoothHCICommandGroupStatus										= 0x05, 
		kBluetoothHCICommandReadFailedContactCounter					= 0x0001, 
		kBluetoothHCICommandResetFailedContactCounter					= 0x0002, 
		kBluetoothHCICommandGetLinkQuality								= 0x0003, 
		kBluetoothHCICommandReadRSSI									= 0x0005, 
		kBluetoothHCICommandReadAFHMappings								= 0x0006,
		kBluetoothHCICommandReadClock									= 0x0007,
		kBluetoothHCICommandReadEncryptionKeySize						= 0x0008,
		kBluetoothHCICommandReadLocalAMPInfo							= 0x0009,
		kBluetoothHCICommandReadLocalAMPASSOC							= 0x000A,
		kBluetoothHCICommandWriteRemoteAMPASSOC							= 0x000B,
		kBluetoothHCICommandGetMWSTransportLayerConfiguration			= 0x000C,
		kBluetoothHCICommandSetTriggeredClockCapture					= 0x000D,

	// Command Group: Testing
	
	kBluetoothHCICommandGroupTesting									= 0x06, 
		kBluetoothHCICommandReadLoopbackMode							= 0x0001, 
		kBluetoothHCICommandWriteLoopbackMode							= 0x0002, 
		kBluetoothHCICommandEnableDeviceUnderTestMode					= 0x0003, 
		kBluetoothHCICommandWriteSimplePairingDebugMode					= 0x0004,
		kBluetoothHCICommandEnableAMPReceiverReports					= 0x0007,
		kBluetoothHCICommandAMPTestEnd									= 0x0008,
		kBluetoothHCICommandAMPTest										= 0x0009,

	// Command Group: Low Energy
	
	kBluetoothHCICommandGroupLowEnergy										= 0x08,
		kBluetoothHCICommandLESetEventMask									= 0x0001,
		kBluetoothHCICommandLEReadBufferSize								= 0x0002,
		kBluetoothHCICommandLEReadLocalSupportedFeatures					= 0x0003,
		kBluetoothHCICommandLESetRandomAddress								= 0x0005,
		kBluetoothHCICommandLESetAdvertisingParameters						= 0x0006,
		kBluetoothHCICommandLEReadAdvertisingChannelTxPower					= 0x0007,
		kBluetoothHCICommandLESetAdvertisingData							= 0x0008,
		kBluetoothHCICommandLESetScanResponseData							= 0x0009,
		kBluetoothHCICommandLESetAdvertiseEnable							= 0x000A,
		kBluetoothHCICommandLESetScanParameters								= 0x000B,
		kBluetoothHCICommandLESetScanEnable									= 0x000C,
		kBluetoothHCICommandLECreateConnection								= 0x000D,
		kBluetoothHCICommandLECreateConnectionCancel						= 0x000E,
		kBluetoothHCICommandLEReadWhiteListSize								= 0x000F,
		kBluetoothHCICommandLEClearWhiteList								= 0x0010,
		kBluetoothHCICommandLEAddDeviceToWhiteList							= 0x0011,
		kBluetoothHCICommandLERemoveDeviceFromWhiteList						= 0x0012,
		kBluetoothHCICommandLEConnectionUpdate								= 0x0013,
		kBluetoothHCICommandLESetHostChannelClassification					= 0x0014,
		kBluetoothHCICommandLEReadChannelMap								= 0x0015,
		kBluetoothHCICommandLEReadRemoteUsedFeatures						= 0x0016,
		kBluetoothHCICommandLEEncrypt										= 0x0017,
		kBluetoothHCICommandLERand											= 0x0018,
		kBluetoothHCICommandLEStartEncryption								= 0x0019,
		kBluetoothHCICommandLELongTermKeyRequestReply						= 0x001A,
		kBluetoothHCICommandLELongTermKeyRequestNegativeReply				= 0x001B,
		kBluetoothHCICommandLEReadSupportedStates							= 0x001C,
		kBluetoothHCICommandLEReceiverTest									= 0x001D,
		kBluetoothHCICommandLETransmitterTest								= 0x001E,
		kBluetoothHCICommandLETestEnd										= 0x001F,
		kBluetoothHCICommandLERemoteConnectionParameterRequestReply			= 0x0020,
		kBluetoothHCICommandLERemoteConnectionParameterRequestNegativeReply	= 0x0021,
		kBluetoothHCICommandLESetDataLength									= 0x0022,
		kBluetoothHCICommandLEReadSuggestedDefaultDataLength				= 0x0023,
		kBluetoothHCICommandLEWriteSuggestedDefaultDataLength				= 0x0024,
		kBluetoothHCICommandLEReadLocalP256PublicKey						= 0x0025,
		kBluetoothHCICommandLEGenerateDHKey									= 0x0026,
		kBluetoothHCICommandLEAddDeviceToResolvingList						= 0x0027,
		kBluetoothHCICommandLERemoveDeviceFromResolvingList					= 0x0028,
		kBluetoothHCICommandLEClearResolvingList							= 0x0029,
		kBluetoothHCICommandLEReadResolvingListSize							= 0x002A,
		kBluetoothHCICommandLEReadPeerResolvableAddress						= 0x002B,
		kBluetoothHCICommandLEReadLocalResolvableAddress					= 0x002C,
		kBluetoothHCICommandLESetAddressResolutionEnable					= 0x002D,
		kBluetoothHCICommandLESetResolvablePrivateAddressTimeout			= 0x002E,
		kBluetoothHCICommandLEReadMaximumDataLength							= 0x002F,
        kBluetoothHCICommandLEReadPhy                                       = 0x0030,
        kBluetoothHCICommandLESetDefaultPhy                                 = 0x0031,
        kBluetoothHCICommandLESetPhy                                        = 0x0032,
		kBluetoothHCICommandLEEnhancedReceiverTest							= 0x0033,
		kBluetoothHCICommandLEEnhancedTransmitterTest						= 0x0034,
		kBluetoothHCICommandLESetAdvertisingSetRandomAddress				= 0x0035,
		kBluetoothHCICommandLESetExtendedAdvertisingParameters				= 0x0036,
		kBluetoothHCICommandLESetExtendedAdvertisingData					= 0x0037,
		kBluetoothHCICommandLESetExtendedScanResponseData					= 0x0038,
		kBluetoothHCICommandLESetExtendedAdvertisingEnableCommand			= 0x0039,
		kBluetoothHCICommandLEReadMaximumAdvertisingDataLength				= 0x003A,
		kBluetoothHCICommandLEReadNumberofSupportedAdvertisingSets			= 0x003B,
		kBluetoothHCICommandLERemoveAdvertisingSet							= 0x003C,
		kBluetoothHCICommandLEClearAdvertisingSets							= 0x003D,
		kBluetoothHCICommandLESetPeriodicAdvertisingParameters				= 0x003E,
		kBluetoothHCICommandLESetPeriodicAdvertisingData					= 0x003F,
		kBluetoothHCICommandLESetPeriodicAdvertisingEnable					= 0x0040,
		kBluetoothHCICommandLESetExtendedScanParameters						= 0x0041,
		kBluetoothHCICommandLESetExtendedScanEnable							= 0x0042,
		kBluetoothHCICommandLEExtendedCreateConnection						= 0x0043,
		kBluetoothHCICommandLEPeriodicAdvertisingCreateSync					= 0x0044,
		kBluetoothHCICommandLEPeriodicAdvertisingCreateSyncCancel			= 0x0045,
		kBluetoothHCICommandLEPeriodicAdvertisingTerminateSync				= 0x0046,
		kBluetoothHCICommandLEAddDeviceToPeriodicAdvertiserList				= 0x0047,
		kBluetoothHCICommandLERemoveDeviceFromPeriodicAdvertiserList		= 0x0048,
		kBluetoothHCICommandLEClearPeriodicAdvertiserList					= 0x0049,
		kBluetoothHCICommandLEReadPeriodicAdvertiserListSize				= 0x004A,
		kBluetoothHCICommandLEReadTransmitPower								= 0x004B,
		kBluetoothHCICommandLEReadRFPathCompensation						= 0x004C,
		kBluetoothHCICommandLEWriteRFPathCompensation						= 0x004D,
		kBluetoothHCICommandLESetPrivacyMode								= 0x004E,

	// Command Group: Logo Testing (no commands yet)
	
	kBluetoothHCICommandGroupLogoTesting 								= 0x3E, 
	
	// Command Group: Vendor Specific
	kBluetoothHCICommandGroupVendorSpecific								= 0x3F, 
	
	kBluetoothHCICommandGroupMax										= 0x40, 
	kBluetoothHCICommandMax												= 0x03FF
};

// HCI Data Types

typedef uint8_t										BluetoothHCIQoSFlags;
typedef uint8_t										BluetoothHCIParamByteCount;
typedef uint16_t									BluetoothHCIACLDataByteCount;
typedef uint8_t										BluetoothHCISCODataByteCount;
typedef uint8_t										BluetoothHCIInquiryLength;
typedef uint8_t										BluetoothHCIResponseCount;
typedef uint8_t										BluetoothHCICountryCode;
typedef uint16_t									BluetoothHCIModeInterval;
typedef uint16_t									BluetoothHCISniffAttemptCount;
typedef uint16_t									BluetoothHCISniffTimeout;
typedef uint16_t									BluetoothHCIParkModeBeaconInterval;

typedef uint8_t										BluetoothMaxSlots;
typedef uint16_t									BluetoothManufacturerName;
typedef uint8_t										BluetoothLMPVersion;
typedef uint16_t									BluetoothLMPSubversion;

typedef uint8_t										BluetoothHCIConnectionMode;
enum BluetoothHCIConnectionModes
{
	kConnectionActiveMode					= 0,
	kConnectionHoldMode						= 1,
	kConnectionSniffMode					= 2,
	kConnectionParkMode						= 3,
	kConnectionModeReservedForFutureUse		= 4,
};
				
typedef struct	BluetoothHCISupportedCommands		BluetoothHCISupportedCommands;
struct BluetoothHCISupportedCommands
{
	uint8_t	data[64];
};		
		
typedef struct	BluetoothHCISupportedFeatures		BluetoothHCISupportedFeatures;
struct BluetoothHCISupportedFeatures
{
	uint8_t	data[8];
};

typedef struct BluetoothHCISupportedFeatures        BluetoothHCILESupportedFeatures;

typedef struct BluetoothHCISupportedFeatures        BluetoothHCILEUsedFeatures;

typedef uint8_t										BluetoothHCIPageNumber;
typedef struct	BluetoothHCIExtendedFeaturesInfo	BluetoothHCIExtendedFeaturesInfo;
struct BluetoothHCIExtendedFeaturesInfo
{
	BluetoothHCIPageNumber	page;
	BluetoothHCIPageNumber	maxPage;
	uint8_t					data[8];
};

        
enum BluetoothLEFeatureBits
{
    kBluetoothLEFeatureLEEncryption                         = (1 << 0L),
    kBluetoothLEFeatureConnectionParamsRequestProcedure     = (1 << 1L),
    kBluetoothLEFeatureExtendedRejectIndication             = (1 << 2L),
    kBluetoothLEFeaturePeripheralInitiatedFeaturesExchange  = (1 << 3L),
    kBluetoothLEFeatureLEPing                               = (1 << 4L),
    kBluetoothLEFeatureLEDataPacketLengthExtension          = (1 << 5L),
    kBluetoothLEFeatureLLPrivacy                            = (1 << 6L),
    kBluetoothLEFeatureExtendedScannerFilterPolicies        = (1 << 7L),
	kBluetoothLEFeatureSlaveInitiatedFeaturesExchange __attribute__ ((deprecated)) = kBluetoothLEFeaturePeripheralInitiatedFeaturesExchange,
    
    // Three other bytes are RFU
};

enum BluetoothFeatureBits
{
	// Byte 0 of the support features data structure.

	kBluetoothFeatureThreeSlotPackets					= (1 << 0L),
	kBluetoothFeatureFiveSlotPackets					= (1 << 1L),
	kBluetoothFeatureEncryption							= (1 << 2L),
	kBluetoothFeatureSlotOffset							= (1 << 3L),
	kBluetoothFeatureTimingAccuracy						= (1 << 4L),
	kBluetoothFeatureSwitchRoles						= (1 << 5L),
	kBluetoothFeatureHoldMode							= (1 << 6L),
	kBluetoothFeatureSniffMode							= (1 << 7L),
	
	// Byte 1 of the support features data structure.

	kBluetoothFeatureParkMode							= (1 << 0L),
	kBluetoothFeatureRSSI								= (1 << 1L),
	kBluetoothFeaturePowerControlRequests				= (1 << 1L),
	kBluetoothFeatureChannelQuality						= (1 << 2L),
	kBluetoothFeatureSCOLink							= (1 << 3L),
	kBluetoothFeatureHV2Packets							= (1 << 4L),
	kBluetoothFeatureHV3Packets							= (1 << 5L),
	kBluetoothFeatureULawLog							= (1 << 6L),
	kBluetoothFeatureALawLog							= (1 << 7L),

	// Byte 2 of the support features data structure.

	kBluetoothFeatureCVSD								= (1 << 0L),
	kBluetoothFeaturePagingScheme						= (1 << 1L),
	kBluetoothFeaturePowerControl						= (1 << 2L),
	kBluetoothFeatureTransparentSCOData					= (1 << 3L),
	kBluetoothFeatureFlowControlLagBit0					= (1 << 4L),
	kBluetoothFeatureFlowControlLagBit1					= (1 << 5L),
	kBluetoothFeatureFlowControlLagBit2					= (1 << 6L),
	kBluetoothFeatureBroadcastEncryption				= (1 << 7L),	
	
	// Byte 3 of the support features data structure.	
	
	kBluetoothFeatureScatterMode						= (1 << 0L),
	kBluetoothFeatureEnhancedDataRateACL2MbpsMode		= (1 << 1L),	
	kBluetoothFeatureEnhancedDataRateACL3MbpsMode		= (1 << 2L),	
	kBluetoothFeatureEnhancedInquiryScan				= (1 << 3L),
	kBluetoothFeatureInterlacedInquiryScan				= (1 << 4L),	
	kBluetoothFeatureInterlacedPageScan					= (1 << 5L),		
	kBluetoothFeatureRSSIWithInquiryResult				= (1 << 6L),	
	kBluetoothFeatureExtendedSCOLink					= (1 << 7L),

	// Byte 4 of the support features data structure.	

	kBluetoothFeatureEV4Packets												= (1 << 0L),
	kBluetoothFeatureEV5Packets												= (1 << 1L),
	kBluetoothFeatureAbsenceMasks											= (1 << 2L),
	kBluetoothFeatureAFHCapablePeripheral									= (1 << 3L),
	kBluetoothFeatureAFHClassificationPeripheral							= (1 << 4L),
	kBluetoothFeatureAliasAuhentication										= (1 << 5L),
	kBluetoothFeatureLESupportedController									= (1 << 6L),
	kBluetoothFeature3SlotEnhancedDataRateACLPackets						= (1 << 7L),
	kBluetoothFeatureAFHCapableSlave __attribute__ ((deprecated)) 			= kBluetoothFeatureAFHCapablePeripheral,
	kBluetoothFeatureAFHClassificationSlave __attribute__ ((deprecated)) 	= kBluetoothFeatureAFHClassificationPeripheral,
	
	// Byte 5 of the support features data structure.

	kBluetoothFeature5SlotEnhancedDataRateACLPackets	= (1 << 0L), // 2.0 version of this header had this at the wrong bit location
	kBluetoothFeatureSniffSubrating						= (1 << 1L),
	kBluetoothFeaturePauseEncryption					= (1 << 2L),
	kBluetoothFeatureAFHCapableMaster					= (1 << 3L),
	kBluetoothFeatureAFHClassificationMaster			= (1 << 4L),
	kBluetoothFeatureEnhancedDataRateeSCO2MbpsMode		= (1 << 5L),
	kBluetoothFeatureEnhancedDataRateeSCO3MbpsMode		= (1 << 6L),
	kBluetoothFeature3SlotEnhancedDataRateeSCOPackets	= (1 << 7L),
	
	// Byte 6 of the support features data structure.	

	kBluetoothFeatureExtendedInquiryResponse			= (1 << 0L),
	kBluetoothFeatureSecureSimplePairing				= (1 << 3L),
	kBluetoothFeatureEncapsulatedPDU					= (1 << 4L),
	kBluetoothFeatureErroneousDataReporting				= (1 << 5L),
	kBluetoothFeatureNonFlushablePacketBoundaryFlag		= (1 << 6L),
	
	// Byte 7 of the support features data structure.

	kBluetoothFeatureLinkSupervisionTimeoutChangedEvent	= (1 << 0L),
	kBluetoothFeatureInquiryTransmissionPowerLevel		= (1 << 1L),
	kBluetoothFeatureExtendedFeatures					= (1 << 7L),
	
	// Byte 8 of the support features data structure (extended) page 1

	kBluetoothExtendedFeatureSimpleSecurePairingHostMode		= (1 << 0L),
    kBluetoothExtendedFeatureLESupportedHostMode                = (1 << 1L),
    kBluetoothExtendedFeatureLEAndBREDRToSameDeviceHostMode     = (1 << 2L),
    KBluetoothExtendedFeatureSecureConnectionsHostMode          = (1 << 3L),
    
    // Byte 16 of supported features data structure (extended) page 2
    
    kBluetoothExtendedFeatureSecureConnectionsControllerSupport  = (1 << 0L),
    kBluetoothExtendedFeaturePing                                = (1 << 1L),
    kBluetoothExtendedFeatureReserved                            = (1 << 2L),
    kBluetoothExtendedFeatureTrainNudging                        = (1 << 3L),
    kBluetoothExtendedFeatureSlotAvailabilityMask                = (1 << 4L),

};

typedef struct BluetoothEventFilterCondition	BluetoothEventFilterCondition;
struct BluetoothEventFilterCondition
{
    uint8_t		data[ 7 ];
};        
        
typedef uint16_t										BluetoothHCIFailedContactCount;
typedef struct BluetoothHCIFailedContactInfo		BluetoothHCIFailedContactInfo;
struct BluetoothHCIFailedContactInfo
{
	BluetoothHCIFailedContactCount	count;
	BluetoothConnectionHandle		handle;
};

typedef SInt8										BluetoothHCIRSSIValue;	/* Valid Range: -127 to +20 */
typedef struct BluetoothHCIRSSIInfo					BluetoothHCIRSSIInfo;
struct BluetoothHCIRSSIInfo
{
	BluetoothConnectionHandle		handle;
	BluetoothHCIRSSIValue			RSSIValue;
};

typedef	uint8_t										BluetoothHCILinkQuality;
typedef struct BluetoothHCILinkQualityInfo			BluetoothHCILinkQualityInfo;
struct BluetoothHCILinkQualityInfo
{
	BluetoothConnectionHandle		handle;
	BluetoothHCILinkQuality			qualityValue;
};

typedef	uint8_t										        BluetoothHCIEncryptionKeySize;
typedef struct BluetoothHCIEncryptionKeySizeInfo			BluetoothHCIEncryptionKeySizeInfo;
struct BluetoothHCIEncryptionKeySizeInfo
{
	BluetoothConnectionHandle		        handle;
	BluetoothHCIEncryptionKeySize			keySize;
};
        
typedef uint8_t	BluetoothHCIRole;
typedef struct	BluetoothHCIRoleInfo					BluetoothHCIRoleInfo;
struct BluetoothHCIRoleInfo
{
	uint8_t						role;
	BluetoothConnectionHandle	handle;
};

enum BluetoothHCIRoles
{
	kBluetoothHCICentralRole			= 0x00,
	kBluetoothHCIPeripheralRole			= 0x01,
	kBluetoothHCIMasterRole __attribute__ ((deprecated)) = kBluetoothHCICentralRole,
	kBluetoothHCISlaveRole __attribute__ ((deprecated)) = kBluetoothHCIPeripheralRole
};

typedef uint16_t										BluetoothHCILinkPolicySettings;
enum BluetoothHCILinkPolicySettingsValues
{
	kDisableAllLMModes					= 0x0000,
	kEnableCentralPeripheralSwitch		= 0x0001,
	kEnableHoldMode						= 0x0002,
	kEnableSniffMode					= 0x0004,
	kEnableParkMode						= 0x0008,
	kReservedForFutureUse				= 0x0010,
	kEnableMasterSlaveSwitch __attribute__ ((deprecated))	= kEnableCentralPeripheralSwitch
};

typedef struct	BluetoothHCILinkPolicySettingsInfo		BluetoothHCILinkPolicySettingsInfo;
struct BluetoothHCILinkPolicySettingsInfo
{
	BluetoothHCILinkPolicySettings		settings;
	BluetoothConnectionHandle			handle;
};


typedef struct BluetoothHCIQualityOfServiceSetupParams	BluetoothHCIQualityOfServiceSetupParams;
struct BluetoothHCIQualityOfServiceSetupParams
{
	uint8_t			flags;
	uint8_t			serviceType;
	uint32_t		tokenRate;
	uint32_t		peakBandwidth;
	uint32_t		latency;
	uint32_t		delayVariation;
};

typedef struct BluetoothHCISetupSynchronousConnectionParams	BluetoothHCISetupSynchronousConnectionParams;
struct BluetoothHCISetupSynchronousConnectionParams
{
	uint32_t		transmitBandwidth;
	uint32_t		receiveBandwidth;
	uint16_t		maxLatency;
	uint16_t		voiceSetting;
	uint8_t			retransmissionEffort;
	uint16_t		packetType;
};
		
typedef struct BluetoothHCIAcceptSynchronousConnectionRequestParams	BluetoothHCIAcceptSynchronousConnectionRequestParams;
struct BluetoothHCIAcceptSynchronousConnectionRequestParams
{
	uint32_t		transmitBandwidth;
	uint32_t		receiveBandwidth;
	uint16_t		maxLatency;
	uint16_t		contentFormat;
	uint8_t			retransmissionEffort;
	uint16_t		packetType;
};
// Add for EnhancedSetup
typedef struct BluetoothHCIEnhancedSetupSynchronousConnectionParams	BluetoothHCIEnhancedSetupSynchronousConnectionParams;
struct BluetoothHCIEnhancedSetupSynchronousConnectionParams
{
    uint32_t		transmitBandwidth;
    uint32_t		receiveBandwidth;
    uint64_t        transmitCodingFormat;
    uint64_t        receiveCodingFormat;
    uint16_t        transmitCodecFrameSize;
    uint16_t        receiveCodecFrameSize;
    uint32_t        inputBandwidth;
    uint32_t        outputBandwidth;
    uint64_t        inputCodingFormat;
    uint64_t        outputCodingFormat;
    uint16_t        inputCodedDataSize;
    uint16_t        outputCodedDataSize;
    uint8_t         inputPCMDataFormat;
    uint8_t         outputPCMDataFormat;
    uint8_t         inputPCMSamplePayloadMSBPosition;
    uint8_t         outputPCMSamplePayloadMSBPosition;
    uint8_t         inputDataPath;
    uint8_t         outputDataPath;
    uint8_t         inputTransportUnitSize;
    uint8_t         outputTransportUnitSize;
    uint16_t		maxLatency;
	uint16_t		packetType;
    uint8_t			retransmissionEffort;
};
// Add for EnhancedAccept
typedef struct BluetoothHCIEnhancedAcceptSynchronousConnectionRequestParams	BluetoothHCIEnhancedAcceptSynchronousConnectionRequestParams;
struct BluetoothHCIEnhancedAcceptSynchronousConnectionRequestParams
{
    uint32_t		transmitBandwidth;
    uint32_t		receiveBandwidth;
    uint64_t        transmitCodingFormat;
    uint64_t        receiveCodingFormat;
    uint16_t        transmitCodecFrameSize;
    uint16_t        receiveCodecFrameSize;
    uint32_t        inputBandwidth;
    uint32_t        outputBandwidth;
    uint64_t        inputCodingFormat;
    uint64_t        outputCodingFormat;
    uint16_t        inputCodedDataSize;
    uint16_t        outputCodedDataSize;
    uint8_t         inputPCMDataFormat;
    uint8_t         outputPCMDataFormat;
    uint8_t         inputPCMSamplePayloadMSBPosition;
    uint8_t         outputPCMSamplePayloadMSBPosition;
    uint8_t         inputDataPath;
    uint8_t         outputDataPath;
    uint8_t         inputTransportUnitSize;
    uint8_t         outputTransportUnitSize;
    uint16_t		maxLatency;
	uint16_t		packetType;
    uint8_t			retransmissionEffort;
};

typedef uint8_t	BluetoothHCILoopbackMode;
enum
{
	kBluetoothHCILoopbackModeOff		= 0x00,
	kBluetoothHCILoopbackModeLocal		= 0x01,
	kBluetoothHCILoopbackModeRemote		= 0x02
};
		
typedef struct BluetoothReadClockInfo BluetoothReadClockInfo;
struct BluetoothReadClockInfo
{
	BluetoothConnectionHandle	handle;
	uint32_t					clock;
	uint16_t					accuracy;			
};
        
typedef struct BluetoothHCIEventFlowSpecificationData BluetoothHCIEventFlowSpecificationData;
struct BluetoothHCIEventFlowSpecificationData
{
    BluetoothConnectionHandle					connectionHandle;
    uint8_t										flags;
    uint8_t										flowDirection;
    uint8_t										serviceType;
    uint32_t									tokenRate;
    uint32_t									tokenBucketSize;
    uint32_t									peakBandwidth;
    uint32_t									accessLatency;
};        
                
typedef uint32_t BluetoothHCIOperationID;
typedef uint32_t BluetoothHCIEventID;
typedef uint32_t BluetoothHCIDataID;
typedef uint32_t BluetoothHCISignalID;
typedef uint32_t BluetoothHCITransportID;
typedef uint32_t BluetoothHCITransportCommandID;
typedef uint32_t BluetoothHCIRequestID;
		

// Version Information

typedef struct	BluetoothHCIVersionInfo		BluetoothHCIVersionInfo;
struct	BluetoothHCIVersionInfo
{
	// Local & Remote information
	
	BluetoothManufacturerName	manufacturerName;
	BluetoothLMPVersion			lmpVersion;
	BluetoothLMPSubversion		lmpSubVersion;
	
	// Local information only
	
	uint8_t						hciVersion;
	uint16_t					hciRevision;
};

// HCI buffer sizes.

typedef struct	BluetoothHCIBufferSize		BluetoothHCIBufferSize;
struct	BluetoothHCIBufferSize
{
	uint16_t	ACLDataPacketLength;
	uint8_t		SCODataPacketLength;
	uint16_t	totalNumACLDataPackets;
	uint16_t	totalNumSCODataPackets;
};

typedef struct	BluetoothHCILEBufferSize	BluetoothHCILEBufferSize;
struct	BluetoothHCILEBufferSize
{
	uint16_t	ACLDataPacketLength;
	uint8_t		totalNumACLDataPackets;
};

// Timeouts
typedef uint16_t	BluetoothHCIConnectionAcceptTimeout;
typedef uint16_t	BluetoothHCIPageTimeout;
enum BluetoothHCITimeoutValues
{
	kDefaultPageTimeout			= 0x2710,
};

#define		BluetoothGetSlotsFromSeconds( inSeconds )		( (inSeconds/.000625 ) )
#define		BluetoothGetSecondsFromSlots( inSlots )			( (inSlots*.000625 ) )


// Link Keys
typedef uint16_t	BluetoothHCINumLinkKeysDeleted;
typedef uint8_t	BluetoothHCINumLinkKeysToWrite;
typedef uint8_t	BluetoothHCIDeleteStoredLinkKeyFlag;
enum BluetoothHCIDeleteStoredLinkKeyFlags
{
	kDeleteKeyForSpecifiedDeviceOnly			= 0x00,
	kDeleteAllStoredLinkKeys					= 0x01,
};

typedef uint8_t	BluetoothHCIReadStoredLinkKeysFlag;
enum BluetoothHCIReadStoredLinkKeysFlags
{
	kReturnLinkKeyForSpecifiedDeviceOnly		= 0x00,
	kReadAllStoredLinkKeys						= 0x01,
};

typedef struct	BluetoothHCIStoredLinkKeysInfo	BluetoothHCIStoredLinkKeysInfo;
struct BluetoothHCIStoredLinkKeysInfo
{
	uint16_t	numLinkKeysRead;
	uint16_t	maxNumLinkKeysAllowedInDevice;
};


// Page Scan

typedef uint8_t	BluetoothHCIPageScanMode;
enum BluetoothHCIPageScanModes
{
	kMandatoryPageScanMode		= 0x00,
	kOptionalPageScanMode1		= 0x01,
	kOptionalPageScanMode2		= 0x02,
	kOptionalPageScanMode3		= 0x03,
};

typedef uint8_t	BluetoothHCIPageScanPeriodMode;
enum BluetoothHCIPageScanPeriodModes
{
	kP0Mode						= 0x00,
	kP1Mode						= 0x01,
	kP2Mode						= 0x02,
};

typedef uint8_t	BluetoothHCIPageScanEnableState;
enum BluetoothHCIPageScanEnableStates
{
	kNoScansEnabled							= 0x00,
	kInquiryScanEnabledPageScanDisabled		= 0x01,
	kInquiryScanDisabledPageScanEnabled		= 0x02,
	kInquiryScanEnabledPageScanEnabled		= 0x03,
};

typedef struct	BluetoothHCIScanActivity	BluetoothHCIScanActivity;
struct BluetoothHCIScanActivity
{
	uint16_t 	scanInterval;
	uint16_t	scanWindow;
};

typedef struct	BluetoothHCIInquiryAccessCode	BluetoothHCIInquiryAccessCode;
struct BluetoothHCIInquiryAccessCode
{
	uint8_t 	data[3];
};

typedef uint8_t BluetoothHCIInquiryAccessCodeCount;
typedef struct	BluetoothHCICurrentInquiryAccessCodes	BluetoothHCICurrentInquiryAccessCodes;
struct BluetoothHCICurrentInquiryAccessCodes
{
	BluetoothHCIInquiryAccessCodeCount	count;	// Number of codes in array.
	BluetoothHCIInquiryAccessCode *		codes;	// Ptr to array of codes.
};

enum
{
	kMaximumNumberOfInquiryAccessCodes	= 0x40
};
		
typedef struct	BluetoothHCICurrentInquiryAccessCodesForWrite	BluetoothHCICurrentInquiryAccessCodesForWrite;
struct BluetoothHCICurrentInquiryAccessCodesForWrite
{
	BluetoothHCIInquiryAccessCodeCount	count;			// Number of codes in array (Range 0x01 to 0x40)
	uint8_t 							codes [kMaximumNumberOfInquiryAccessCodes * sizeof (BluetoothHCIInquiryAccessCode)];	// Array of bytes (maximum 192 bytes -- 64 * 3).
};
		
typedef	struct BluetoothHCILinkSupervisionTimeout	BluetoothHCILinkSupervisionTimeout;
struct BluetoothHCILinkSupervisionTimeout
{
	BluetoothConnectionHandle 	handle;
	uint16_t					timeout;
};

typedef uint8_t BluetoothHCIFlowControlState;
enum BluetoothHCISCOFlowControlStates
{
	kSCOFlowControlDisabled		= 0x00,
	kSCOFlowControlEnabled		= 0x01
};

enum BluetoothHCIGeneralFlowControlStates
{
	kHostControllerToHostFlowControlOff			= 0x00,
	kHCIACLDataPacketsOnHCISCODataPacketsOff	= 0x01,
	kHCIACLDataPacketsOffHCISCODataPacketsOn	= 0x02,
	kHCIACLDataPacketsOnHCISCODataPacketsOn		= 0x03,
};		
		
typedef SInt8 BluetoothHCITransmitPowerLevel;
typedef uint8_t BluetoothHCITransmitPowerLevelType;
enum BluetoothHCITransmitReadPowerLevelTypes
{
	kReadCurrentTransmitPowerLevel	= 0x00,
	kReadMaximumTransmitPowerLevel	= 0x01,
};

typedef uint8_t	BluetoothHCIAFHChannelAssessmentMode;	
enum BluetoothHCIAFHChannelAssessmentModes
{
	kAFHChannelAssessmentModeDisabled		= 0x00,
	kAFHChannelAssessmentModeEnabled		= 0x01
};


typedef struct BluetoothHCITransmitPowerLevelInfo	BluetoothHCITransmitPowerLevelInfo;
struct BluetoothHCITransmitPowerLevelInfo
{
	BluetoothConnectionHandle		handle;
	BluetoothHCITransmitPowerLevel	level; // Range: -70 <= N <= 20 (units are dBm)
};

typedef uint8_t	BluetoothHCINumBroadcastRetransmissions;
typedef uint8_t	BluetoothHCIHoldModeActivity;
enum BluetoothHCIHoldModeActivityStates
{
	kMaintainCurrentPowerState		= 0x00,
	kSuspendPageScan				= 0x01,
	kSuspendInquiryScan				= 0x02,
	kSuspendPeriodicInquiries		= 0x03,
};

typedef uint8_t BluetoothHCIAuthenticationEnable;
enum BluetoothHCIAuthentionEnableModes
{
	kAuthenticationDisabled			= 0x00,
	kAuthenticationEnabled			= 0x01,
};

typedef uint8_t	BluetoothHCIEncryptionMode;
enum BluetoothHCIEncryptionModes
{
	kEncryptionDisabled									= 0x00,		// Default.
	kEncryptionOnlyForPointToPointPackets				= 0x01,
	kEncryptionForBothPointToPointAndBroadcastPackets	= 0x02,
};

typedef uint16_t	BluetoothHCIAutomaticFlushTimeout;
typedef struct	BluetoothHCIAutomaticFlushTimeoutInfo	BluetoothHCIAutomaticFlushTimeoutInfo;
struct BluetoothHCIAutomaticFlushTimeoutInfo
{
	BluetoothConnectionHandle			handle;
	BluetoothHCIAutomaticFlushTimeout	timeout;
};

#define	kInfoStringMaxLength		35
typedef struct	BluetoothTransportInfo 		BluetoothTransportInfo;
typedef 		BluetoothTransportInfo*		BluetoothTransportInfoPtr;
struct			BluetoothTransportInfo
{
	uint32_t	productID;
	uint32_t	vendorID;
	uint32_t	type;
	char		productName[kInfoStringMaxLength];
	char		vendorName[kInfoStringMaxLength];
	
	uint64_t	totalDataBytesSent;
	uint64_t	totalSCOBytesSent;
	uint64_t	totalDataBytesReceived;
	uint64_t	totalSCOBytesReceived;
};

enum BluetoothTransportTypes
{
	kBluetoothTransportTypeUSB		= 0x01,
	kBluetoothTransportTypePCCard	= 0x02,
	kBluetoothTransportTypePCICard	= 0x03,
	kBluetoothTransportTypeUART		= 0x04,
    kBluetoothTransportTypePCIe     = 0x05
};

// Inquiries

typedef struct	BluetoothHCIInquiryResult	BluetoothHCIInquiryResult;
struct	BluetoothHCIInquiryResult
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothPageScanRepetitionMode		pageScanRepetitionMode;
	BluetoothHCIPageScanPeriodMode		pageScanPeriodMode;
	BluetoothHCIPageScanMode			pageScanMode;
	BluetoothClassOfDevice				classOfDevice;
	BluetoothClockOffset				clockOffset;
};

#define kBluetoothHCIInquiryResultsMaxResults 50
typedef struct	BluetoothHCIInquiryResults	BluetoothHCIInquiryResults;
struct	BluetoothHCIInquiryResults
{
	BluetoothHCIInquiryResult		results[kBluetoothHCIInquiryResultsMaxResults];
	IOItemCount						count;
};


//Inquiries with RSSI (v1.2 specification)

typedef struct	BluetoothHCIInquiryWithRSSIResult	BluetoothHCIInquiryWithRSSIResult;
struct	BluetoothHCIInquiryWithRSSIResult
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothPageScanRepetitionMode		pageScanRepetitionMode;
	uint8_t								reserved;
	BluetoothClassOfDevice				classOfDevice;
	BluetoothClockOffset				clockOffset;
	BluetoothHCIRSSIValue				RSSIValue;
};

typedef struct	BluetoothHCIInquiryWithRSSIResults	BluetoothHCIInquiryWithRSSIResults;
struct	BluetoothHCIInquiryWithRSSIResults
{
	BluetoothHCIInquiryWithRSSIResult	results[50];
	IOItemCount							count;
};

//Inquiries with 'Extended Inquiry Response' (v2.1 specification)

typedef uint8_t	BluetoothHCIFECRequired;
enum BluetoothHCIFECRequiredValues
{
	kBluetoothHCIFECRequired		= 0x00,
	kBluetoothHCIFECNotRequired		= 0x01
};

typedef	uint8_t	BluetoothHCIInquiryMode;
enum BluetoothHCIInquiryModes
{
	kBluetoothHCIInquiryModeResultFormatStandard								=	0x00,
	kBluetoothHCIInquiryModeResultFormatWithRSSI								=	0x01,
	kBluetoothHCIInquiryModeResultFormatWithRSSIOrExtendedInquiryResultFormat	=	0x02
};

typedef uint8_t	BluetoothHCIInquiryScanType;
enum BluetoothHCIInquiryScanTypes
{
	kBluetoothHCIInquiryScanTypeStandard		= 0x00,
	kBluetoothHCIInquiryScanTypeInterlaced		= 0x01,
	
	kBluetoothHCIInquiryScanTypeReservedStart	= 0x02,
	kBluetoothHCIInquiryScanTypeReservedEnd		= 0xFF
};						

typedef	uint8_t	BluetoothHCIExtendedInquiryResponseDataType;

typedef struct	BluetoothHCIExtendedInquiryResponse		BluetoothHCIExtendedInquiryResponse;	/* Extended Inquiry Response [EIR] data, consisting of a sequence of data structures in this format: [length(1byte)][data type(1byte)][data(e.g. device name)] */
struct	BluetoothHCIExtendedInquiryResponse
{
	uint8_t		data[ 240 ];
};

typedef struct	BluetoothHCIReadExtendedInquiryResponseResults	BluetoothHCIReadExtendedInquiryResponseResults;
struct	BluetoothHCIReadExtendedInquiryResponseResults
{
	BluetoothHCIFECRequired					outFECRequired;
	BluetoothHCIExtendedInquiryResponse		extendedInquiryResponse;
};

typedef struct	BluetoothHCIExtendedInquiryResult	BluetoothHCIExtendedInquiryResult;
struct	BluetoothHCIExtendedInquiryResult
{
	uint8_t								numberOfReponses;			/* always a value of 1 */
	BluetoothDeviceAddress				deviceAddress;
	BluetoothPageScanRepetitionMode		pageScanRepetitionMode;
	uint8_t								reserved;
	BluetoothClassOfDevice				classOfDevice;
	BluetoothClockOffset				clockOffset;
	BluetoothHCIRSSIValue				RSSIValue;
	BluetoothHCIExtendedInquiryResponse	extendedInquiryResponse;
};

typedef struct BluetoothHCIReadLMPHandleResults	BluetoothHCIReadLMPHandleResults;
struct BluetoothHCIReadLMPHandleResults
{
	BluetoothConnectionHandle		handle;
	BluetoothLMPHandle				lmp_handle;
	uint32_t						reserved;
};		
						
// 'Simple Pairing' (v2.1 specification)

typedef	uint8_t	BluetoothHCISimplePairingMode;
enum BluetoothHCISimplePairingModes
{
	kBluetoothHCISimplePairingModeNotSet									=	0x00,
	kBluetoothHCISimplePairingModeEnabled									=	0x01
};

typedef	uint8_t	BluetoothSimplePairingDebugMode;
enum BluetoothSimplePairingDebugModes
{
	kBluetoothHCISimplePairingDebugModeDisabled								=	0x00,
	kBluetoothHCISimplePairingDebugModeEnabled								=	0x01
};

typedef struct	BluetoothHCISimplePairingOOBData		BluetoothHCISimplePairingOOBData;
struct BluetoothHCISimplePairingOOBData
{
	uint8_t		data[ 16 ];
};

typedef struct BluetoothHCIReadLocalOOBDataResults BluetoothHCIReadLocalOOBDataResults;
struct	BluetoothHCIReadLocalOOBDataResults
{
	BluetoothHCISimplePairingOOBData		hash;
	BluetoothHCISimplePairingOOBData		randomizer;
};

typedef uint8_t	BluetoothIOCapability;
enum BluetoothIOCapabilities
{
	kBluetoothCapabilityTypeDisplayOnly			= 0x00,
	kBluetoothCapabilityTypeDisplayYesNo		= 0x01,
	kBluetoothCapabilityTypeKeyboardOnly		= 0x02,
	kBluetoothCapabilityTypeNoInputNoOutput		= 0x03
};

typedef uint8_t	BluetoothOOBDataPresence;
enum BluetoothOOBDataPresenceValues
{
	kBluetoothOOBAuthenticationDataNotPresent				= 0x00,
	kBluetoothOOBAuthenticationDataFromRemoteDevicePresent	= 0x01
};

typedef uint8_t	BluetoothAuthenticationRequirements;
enum BluetoothAuthenticationRequirementsValues
{
	kBluetoothAuthenticationRequirementsMITMProtectionNotRequired					= 0x00,		/* Numeric comparison with automatic accept allowed */
	kBluetoothAuthenticationRequirementsMITMProtectionRequired						= 0x01,		/* Refer to BluetoothIOCapabilities to determine authentication procedure */
	kBluetoothAuthenticationRequirementsMITMProtectionNotRequiredNoBonding			= 0x00,
	kBluetoothAuthenticationRequirementsMITMProtectionRequiredNoBonding				= 0x01,
	kBluetoothAuthenticationRequirementsMITMProtectionNotRequiredDedicatedBonding	= 0x02,
	kBluetoothAuthenticationRequirementsMITMProtectionRequiredDedicatedBonding		= 0x03,
	kBluetoothAuthenticationRequirementsMITMProtectionNotRequiredGeneralBonding		= 0x04,
	kBluetoothAuthenticationRequirementsMITMProtectionRequiredGeneralBonding		= 0x05
};

typedef struct BluetoothIOCapabilityResponse	BluetoothIOCapabilityResponse;
struct BluetoothIOCapabilityResponse
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothIOCapability				ioCapability;				/* possible values from BluetoothIOCapabilities above */
	BluetoothOOBDataPresence			OOBDataPresence;				
	BluetoothAuthenticationRequirements	authenticationRequirements;
};

typedef uint32_t	BluetoothPasskey;

typedef struct BluetoothUserPasskeyNotification	BluetoothUserPasskeyNotification;
struct BluetoothUserPasskeyNotification
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothPasskey					passkey;				/* passkey for display. valid values are 000000 - 999999 */
};

typedef uint8_t	BluetoothKeypressNotificationType;
enum BluetoothKeypressNotificationTypes
{
	kBluetoothKeypressNotificationTypePasskeyEntryStarted		= 0,
	kBluetoothKeypressNotificationTypePasskeyDigitEntered		= 1,
	kBluetoothKeypressNotificationTypePasskeyDigitErased		= 2,
	kBluetoothKeypressNotificationTypePasskeyCleared			= 3,
	kBluetoothKeypressNotificationTypePasskeyEntryCompleted		= 4
};
		
typedef struct BluetoothKeypressNotification	BluetoothKeypressNotification;
struct BluetoothKeypressNotification
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothKeypressNotificationType	notificationType;
};
		
typedef struct BluetoothRemoteHostSupportedFeaturesNotification	BluetoothRemoteHostSupportedFeaturesNotification;
struct BluetoothRemoteHostSupportedFeaturesNotification
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothHCISupportedFeatures		hostSupportedFeatures;
};		
		
typedef SInt8	TransmissionPower;

typedef struct BluetoothAFHHostChannelClassification	BluetoothAFHHostChannelClassification;
struct BluetoothAFHHostChannelClassification
{
	uint8_t		data[ 10 ];			/* 79 bits meaningful */
};

typedef uint8_t	BluetoothAFHMode;
typedef struct	BluetoothAFHResults					BluetoothAFHResults;
struct BluetoothAFHResults
{
	BluetoothConnectionHandle	handle;
	BluetoothAFHMode			mode;
	uint8_t						afhMap[10];
};

typedef uint32_t	BluetoothNumericValue;

typedef struct BluetoothUserConfirmationRequest	BluetoothUserConfirmationRequest;
struct BluetoothUserConfirmationRequest
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothNumericValue				numericValue;				/* numeric value for display. valid values are 000000 - 999999 */
};

typedef struct	BluetoothHCIEventSimplePairingCompleteResults 		BluetoothHCIEventSimplePairingCompleteResults;
struct			BluetoothHCIEventSimplePairingCompleteResults
{
	BluetoothDeviceAddress						deviceAddress;
};


// Packet Sizes

enum
{
	kBluetoothHCICommandPacketHeaderSize 	= 3, 
	kBluetoothHCICommandPacketMaxDataSize 	= 255, 
	kBluetoothHCIMaxCommandPacketSize		= kBluetoothHCICommandPacketHeaderSize + kBluetoothHCICommandPacketMaxDataSize, 
	
	kBluetoothHCIEventPacketHeaderSize	 	= 2, 
	kBluetoothHCIEventPacketMaxDataSize 	= 255, 
	kBluetoothHCIMaxEventPacketSize			= kBluetoothHCIEventPacketHeaderSize + kBluetoothHCIEventPacketMaxDataSize, 
	
	kBluetoothHCIDataPacketHeaderSize 		= 4, 
	kBluetoothHCIDataPacketMaxDataSize 		= 65535, 
	kBluetoothHCIMaxDataPacketSize			= kBluetoothHCIDataPacketHeaderSize + kBluetoothHCIDataPacketMaxDataSize
};

typedef uint8_t		BluetoothHCIEventCode;
typedef uint8_t		BluetoothLinkType;
enum BluetoothLinkTypes
{
	kBluetoothSCOConnection		= 0,
	kBluetoothACLConnection		= 1,
	kBluetoothESCOConnection	= 2,
    kBluetoothLinkTypeNone		= 0xff
};

typedef uint16_t		BluetoothHCIContentFormat; // 10 bits meaningful

typedef uint16_t		BluetoothHCIVoiceSetting; // 10 bits meaningful
enum
{
	// Input Coding
	kBluetoothVoiceSettingInputCodingMask					= 0x300,
	kBluetoothVoiceSettingInputCodingLinearInputCoding		= 0x000,
	kBluetoothVoiceSettingInputCodingULawInputCoding		= 0x100,
	kBluetoothVoiceSettingInputCodingALawInputCoding		= 0x200,
};

enum
{
	// Input Data Format
	kBluetoothVoiceSettingInputDataFormatMask				= 0x0C0,
	kBluetoothVoiceSettingInputDataFormat1sComplement		= 0x000,
	kBluetoothVoiceSettingInputDataFormat2sComplement		= 0x040,
	kBluetoothVoiceSettingInputDataFormatSignMagnitude		= 0x080,
	kBluetoothVoiceSettingInputDataFormatUnsigned			= 0x0C0,
};

enum
{
	// Input Sample Size
	kBluetoothVoiceSettingInputSampleSizeMask				= 0x020,
	kBluetoothVoiceSettingInputSampleSize8Bit				= 0x000,
	kBluetoothVoiceSettingInputSampleSize16Bit				= 0x020,
};

enum
{
	// Linear PCM Bit Position
	kBluetoothVoiceSettingPCMBitPositionMask				= 0x01C,
};

enum
{
	// Air Coding Format
	kBluetoothVoiceSettingAirCodingFormatMask				= 0x003,
	kBluetoothVoiceSettingAirCodingFormatCVSD				= 0x000,
	kBluetoothVoiceSettingAirCodingFormatULaw				= 0x001,
	kBluetoothVoiceSettingAirCodingFormatALaw				= 0x002,
	kBluetoothVoiceSettingAirCodingFormatTransparentData	= 0x003,
};

typedef uint8_t		BluetoothHCISupportedIAC;
typedef uint32_t	BluetoothHCITransmitBandwidth;
typedef uint32_t	BluetoothHCIReceiveBandwidth;
typedef uint64_t    BluetoothHCITransmitCodingFormat;
typedef uint64_t    BluetoothHCIReceiveCodingFormat;
typedef uint16_t    BluetoothHCITransmitCodecFrameSize;
typedef uint16_t    BluetoothHCIReceiveCodecFrameSize;
typedef uint32_t    BluetoothHCIInputBandwidth;
typedef uint32_t    BluetoothHCIOutputBandwidth;
typedef uint64_t    BluetoothHCIInputCodingFormat;
typedef uint64_t    BluetoothHCIOutputCodingFormat;
typedef uint16_t    BluetoothHCIInputCodedDataSize;
typedef uint16_t    BluetoothHCIOutputCodedDataSize;
typedef uint8_t     BluetoothHCIInputPCMDataFormat;
typedef uint8_t     BluetoothHCIOutputPCMDataFormat;
typedef uint8_t     BluetoothHCIInputPCMSamplePayloadMSBPosition;
typedef uint8_t     BluetoothHCIOutputPCMSamplePayloadMSBPosition;
typedef uint8_t     BluetoothHCIInputDataPath;
typedef uint8_t     BluetoothHCIOutputDataPath;
typedef uint8_t     BluetoothHCIInputTransportUnitSize;
typedef uint8_t     BluetoothHCIOutputTransportUnitSize;
typedef uint16_t	BluetoothHCIMaxLatency;
typedef uint8_t		BluetoothHCIRetransmissionEffort;		
enum BluetoothHCIRetransmissionEffortTypes
{
	kHCIRetransmissionEffortTypeNone								= 0x00,
	kHCIRetransmissionEffortTypeAtLeastOneAndOptimizeForPower		= 0x01,
	kHCIRetransmissionEffortTypeAtLeastOneAndOptimizeLinkQuality	= 0x02,
	kHCIRetransmissionEffortTypeDontCare							= 0xFF,
};
		

// Setup Synchronous Packet types (Bluetooth 2.1 spec section 7.7.35  - Setup Synchronous Command Complete Event)

typedef uint8_t		BluetoothAirMode;
enum
{
	kBluetoothAirModeULawLog				= 0x00,
	kBluetoothAirModeALawLog				= 0x01,
	kBluetoothAirModeCVSD					= 0x02,
	kBluetoothAirModeTransparentData		= 0x03
};

typedef struct	BluetoothSynchronousConnectionInfo		BluetoothSynchronousConnectionInfo;
struct	BluetoothSynchronousConnectionInfo
{
	BluetoothHCITransmitBandwidth		transmitBandWidth;
	BluetoothHCIReceiveBandwidth		receiveBandWidth;
	BluetoothHCIMaxLatency				maxLatency;
	BluetoothHCIVoiceSetting			voiceSetting;
	BluetoothHCIRetransmissionEffort	retransmissionEffort;
	BluetoothPacketType					packetType;
};

typedef struct	BluetoothEnhancedSynchronousConnectionInfo		BluetoothEnhancedSynchronousConnectionInfo;
struct	BluetoothEnhancedSynchronousConnectionInfo
{
    BluetoothHCITransmitBandwidth                   transmitBandWidth;
    BluetoothHCIReceiveBandwidth                    receiveBandWidth;
    BluetoothHCITransmitCodingFormat                transmitCodingFormat;
    BluetoothHCIReceiveCodingFormat                 receiveCodingFormat;
    BluetoothHCITransmitCodecFrameSize              transmitCodecFrameSize;
    BluetoothHCIReceiveCodecFrameSize               receiveCodecFrameSize;
    BluetoothHCIInputBandwidth                      inputBandwidth;
    BluetoothHCIOutputBandwidth                     outputBandwidth;
    BluetoothHCIInputCodingFormat                   inputCodingFormat;
    BluetoothHCIOutputCodingFormat                  outputCodingFormat;
    BluetoothHCIInputCodedDataSize                  inputCodedDataSize;
    BluetoothHCIOutputCodedDataSize                 outputCodedDataSize;
    BluetoothHCIInputPCMDataFormat                  inputPCMDataFormat;
    BluetoothHCIOutputPCMDataFormat                 outputPCMDataFormat;
    BluetoothHCIInputPCMSamplePayloadMSBPosition    inputPCMSampelPayloadMSBPosition;
    BluetoothHCIOutputPCMSamplePayloadMSBPosition   outputPCMSampelPayloadMSBPosition;
    BluetoothHCIInputDataPath                       inputDataPath;
    BluetoothHCIOutputDataPath                      outputDataPath;
    BluetoothHCIInputTransportUnitSize              inputTransportUnitSize;
    BluetoothHCIOutputTransportUnitSize             outputTransportUnitSize;
    BluetoothHCIMaxLatency                          maxLatency;
    BluetoothHCIVoiceSetting                        voiceSetting;
    BluetoothHCIRetransmissionEffort                retransmissionEffort;
    BluetoothPacketType                             packetType;
};
        
typedef struct	BluetoothHCIEventSynchronousConnectionCompleteResults 	BluetoothHCIEventSynchronousConnectionCompleteResults;
struct			BluetoothHCIEventSynchronousConnectionCompleteResults
{
	BluetoothConnectionHandle			connectionHandle;
	BluetoothDeviceAddress				deviceAddress;
	BluetoothLinkType					linkType;
	uint8_t								transmissionInterval;
	uint8_t								retransmissionWindow;
	uint16_t							receivePacketLength;
	uint16_t							transmitPacketLength;
	BluetoothAirMode					airMode;
};

typedef struct	BluetoothHCIEventSynchronousConnectionChangedResults 	BluetoothHCIEventSynchronousConnectionChangedResults;
struct			BluetoothHCIEventSynchronousConnectionChangedResults
{
	BluetoothConnectionHandle			connectionHandle;
	uint8_t								transmissionInterval;
	uint8_t								retransmissionWindow;
	uint16_t							receivePacketLength;
	uint16_t							transmitPacketLength;
};

typedef uint8_t	BluetoothHCIStatus;
typedef uint8_t	BluetoothHCIEventStatus;

// Events.

enum
{
	kBluetoothHCIEventInquiryComplete									= 0x01,
	kBluetoothHCIEventInquiryResult										= 0x02,
	kBluetoothHCIEventConnectionComplete								= 0x03,
	kBluetoothHCIEventConnectionRequest									= 0x04,
	kBluetoothHCIEventDisconnectionComplete								= 0x05,
	kBluetoothHCIEventAuthenticationComplete							= 0x06,
	kBluetoothHCIEventRemoteNameRequestComplete							= 0x07,
	kBluetoothHCIEventEncryptionChange									= 0x08,
	kBluetoothHCIEventChangeConnectionLinkKeyComplete					= 0x09,
	kBluetoothHCIEventMasterLinkKeyComplete								= 0x0A,
	kBluetoothHCIEventReadRemoteSupportedFeaturesComplete				= 0x0B,
	kBluetoothHCIEventReadRemoteVersionInformationComplete				= 0x0C,
	kBluetoothHCIEventQoSSetupComplete									= 0x0D,
	kBluetoothHCIEventCommandComplete									= 0x0E,
	kBluetoothHCIEventCommandStatus										= 0x0F,
	kBluetoothHCIEventHardwareError										= 0x10,
	kBluetoothHCIEventFlushOccurred										= 0x11,
	kBluetoothHCIEventRoleChange										= 0x12,
	kBluetoothHCIEventNumberOfCompletedPackets							= 0x13,
	kBluetoothHCIEventModeChange										= 0x14,
	kBluetoothHCIEventReturnLinkKeys									= 0x15,
	kBluetoothHCIEventPINCodeRequest									= 0x16,
	kBluetoothHCIEventLinkKeyRequest									= 0x17,
	kBluetoothHCIEventLinkKeyNotification								= 0x18,
	kBluetoothHCIEventLoopbackCommand									= 0x19,
	kBluetoothHCIEventDataBufferOverflow								= 0x1A,
	kBluetoothHCIEventMaxSlotsChange									= 0x1B,
	kBluetoothHCIEventReadClockOffsetComplete							= 0x1C,
	kBluetoothHCIEventConnectionPacketType								= 0x1D,
	kBluetoothHCIEventQoSViolation										= 0x1E,
	kBluetoothHCIEventPageScanModeChange								= 0x1F,	// DEPRECATED
	kBluetoothHCIEventPageScanRepetitionModeChange						= 0x20,
	
	// [v1.2]

	kBluetoothHCIEventFlowSpecificationComplete							= 0x21,
	kBluetoothHCIEventInquiryResultWithRSSI								= 0x22,
	kBluetoothHCIEventReadRemoteExtendedFeaturesComplete				= 0x23,
	kBluetoothHCIEventSynchronousConnectionComplete						= 0x2C,
	kBluetoothHCIEventSynchronousConnectionChanged						= 0x2D,

	// [v2.1]

	kBluetoothHCIEventSniffSubrating									= 0x2E,
	kBluetoothHCIEventExtendedInquiryResult								= 0x2F,
	kBluetoothHCIEventEncryptionKeyRefreshComplete						= 0x30,
	kBluetoothHCIEventIOCapabilityRequest								= 0x31,
	kBluetoothHCIEventIOCapabilityResponse								= 0x32,
	kBluetoothHCIEventUserConfirmationRequest							= 0x33,
	kBluetoothHCIEventUserPasskeyRequest								= 0x34,
	kBluetoothHCIEventRemoteOOBDataRequest								= 0x35,
	kBluetoothHCIEventSimplePairingComplete								= 0x36,
	kBluetoothHCIEventLinkSupervisionTimeoutChanged						= 0x38,
	kBluetoothHCIEventEnhancedFlushComplete								= 0x39,
	kBluetoothHCIEventUserPasskeyNotification							= 0x3B,
	kBluetoothHCIEventKeypressNotification								= 0x3C,
	kBluetoothHCIEventRemoteHostSupportedFeaturesNotification			= 0x3D,
	
	// [v4.0] (All LE Events come through this event, and the first following byte is the subevent
	
	kBluetoothHCIEventLEMetaEvent										= 0x3E,
		kBluetoothHCISubEventLEConnectionComplete						= 0x01,
		kBluetoothHCISubEventLEAdvertisingReport						= 0x02,
		kBluetoothHCISubEventLEConnectionUpdateComplete					= 0x03,
		kBluetoothHCISubEventLEReadRemoteUsedFeaturesComplete			= 0x04,
		kBluetoothHCISubEventLELongTermKeyRequest						= 0x05,
    	kBluetoothHCISubEventLERemoteConnectionParameterRequest			= 0x06,
    	kBluetoothHCISubEventLEDataLengthChange							= 0x07,
    	kBluetoothHCISubEventLEReadLocalP256PublicKeyComplete			= 0x08,
    	kBluetoothHCISubEventLEGenerateDHKeyComplete					= 0x09,
    	kBluetoothHCISubEventLEEnhancedConnectionComplete				= 0x0A,
    	kBluetoothHCISubEventLEDirectAdvertisingReport					= 0x0B,
        kBluetoothHCISubEventLEPhyUpdateComplete                        = 0x0C,
        kBluetoothHCISubEventLEExtendedAdvertising                      = 0x0D,
        kBluetoothHCISubEventLEPeriodicAdvertisingSyncEstablished       = 0x0E,
        kBluetoothHCISubEventLEPeriodicAdvertisingReport                = 0x0F,
        kBluetoothHCISubEventLEPeriodicAdvertisingSyncLost              = 0x10,
        kBluetoothHCISubEventLEScanTimeout                              = 0x11,
        kBluetoothHCISubEventLEAdvertisingSetTerminated                 = 0x12,
        kBluetoothHCISubEventLEScanRequestReceived                      = 0x13,
        kBluetoothHCISubEventLEChannelSelectionAlgorithm                = 0x14,
    
    
	// [v3.0]
	
	kBluetoothHCIEventPhysicalLinkComplete								= 0x40,
	kBluetoothHCIEventChannelSelected									= 0x41,
	kBluetoothHCIEventDisconnectionPhysicalLinkComplete					= 0x42,
	kBluetoothHCIEventPhysicalLinkLossEarlyWarning						= 0x43,
	kBluetoothHCIEventPhysicalLinkRecovery								= 0x44,
	kBluetoothHCIEventLogicalLinkComplete								= 0x45,
	kBluetoothHCIEventDisconnectionLogicalLinkComplete					= 0x46,
	kBluetoothHCIEventFlowSpecModifyComplete							= 0x47,
	kBluetoothHCIEventNumberOfCompletedDataBlocks						= 0x48,
	kBluetoothHCIEventShortRangeModeChangeComplete						= 0x4C,
	kBluetoothHCIEventAMPStatusChange									= 0x4D,
	kBluetoothHCIEventAMPStartTest										= 0x49,
	kBluetoothHCIEventAMPTestEnd										= 0x4A,
	kBluetoothHCIEventAMPReceiverReport									= 0x4B,
	
	
	kBluetoothHCIEventLogoTesting										= 0xFE,
	kBluetoothHCIEventVendorSpecific									= 0xFF
};

// HCI Event Masks

// Event masks are 8 octets according to the spec. v2.1 introduces some event masks that
// actually exceed 32 bits so the 4 byte enum we had before Bluetooth 2.0 will still work for old
// the masks, but the new masks need to be defined as 64 bits.

typedef uint64_t	BluetoothHCIEventMask;

#define kBluetoothHCIEventMaskLEDefault64Bit									0x000000000000001FLL
#define	kBluetoothHCIEventMaskDefault64Bit										0x00001FFFFFFFFFFFLL
#define	kBluetoothHCIEventMaskAll64Bit											0xFFFFFFFFFFFFFFFFLL

	// [v1.2]

#define	kBluetoothHCIEventMaskFlowSpecificationCompleteEvent					0x0000000100000000LL
#define	kBluetoothHCIEventMaskInquiryResultWithRSSIEvent						0x0000000200000000LL
#define	kBluetoothHCIEventMaskReadRemoteExtendedFeaturesCompleteEvent			0x0000000400000000LL
#define	kBluetoothHCIEventMaskSynchronousConnectionCompleteEvent				0x0000080000000000LL
#define	kBluetoothHCIEventMaskSynchronousConnectionChangedEvent					0x0000100000000000LL

	// [v2.1]

#define	kBluetoothHCIEventMaskSniffSubratingEvent								0x0000200000000000LL
#define	kBluetoothHCIEventMaskExtendedInquiryResultEvent						0x0000400000000000LL
#define kBluetoothHCIEventMaskEncryptionChangeEvent								0x0000000000000080LL
#define kBluetoothHCIEventMaskEncryptionKeyRefreshCompleteEvent					0x0000800000000000LL
#define	kBluetoothHCIEventMaskLinkSupervisionTimeoutChangedEvent				0x0080000000000000LL
#define	kBluetoothHCIEventMaskEnhancedFlushCompleteEvent						0x0100000000000000LL
	
	// [v2.1 Secure Simple Pairing]

#define	kBluetoothHCIEventMaskIOCapabilityRequestEvent							0x0001000000000000LL
#define	kBluetoothHCIEventMaskIOCapabilityRequestReplyEvent						0x0002000000000000LL
#define	kBluetoothHCIEventMaskUserConfirmationRequestEvent						0x0004000000000000LL
#define	kBluetoothHCIEventMaskUserPasskeyRequestEvent							0x0008000000000000LL
#define	kBluetoothHCIEventMaskRemoteOOBDataRequestEvent							0x0010000000000000LL
#define	kBluetoothHCIEventMaskSimplePairingCompleteEvent						0x0020000000000000LL
#define kBluetoothHCIEvnetMaskLinkSupervisionTimeoutChangedEvent				0x0080000000000000LL
#define kBluetoothHCIEvnetMaskEnhancedFlushCompleteEvent						0x0100000000000000LL
#define	kBluetoothHCIEventMaskUserPasskeyNotificationEvent						0x0400000000000000LL
#define	kBluetoothHCIEventMaskKeypressNotificationEvent							0x0800000000000000LL
#define	kBluetoothHCIEventMaskRemoteHostSupportedFeaturesNotificationEvent		0x1000000000000000LL
#define	kBluetoothHCIEventMaskLEMetaEvent										0x2000000000000000LL
	
enum
{
	kBluetoothHCIEventMaskNone											= 0x00000000, 
	kBluetoothHCIEventMaskInquiryComplete								= 0x00000001, 
	kBluetoothHCIEventMaskInquiryResult									= 0x00000002, 
	kBluetoothHCIEventMaskConnectionComplete							= 0x00000004, 
	kBluetoothHCIEventMaskConnectionRequest								= 0x00000008, 
	kBluetoothHCIEventMaskDisconnectionComplete							= 0x00000010, 
	kBluetoothHCIEventMaskAuthenticationComplete						= 0x00000020, 
	kBluetoothHCIEventMaskRemoteNameRequestComplete						= 0x00000040, 
	kBluetoothHCIEventMaskEncryptionChange								= 0x00000080, 
	kBluetoothHCIEventMaskChangeConnectionLinkKeyComplete				= 0x00000100, 
	kBluetoothHCIEventMaskMasterLinkKeyComplete							= 0x00000200, 
	kBluetoothHCIEventMaskReadRemoteSupportedFeaturesComplete			= 0x00000400, 
	kBluetoothHCIEventMaskReadRemoteVersionInformationComplete			= 0x00000800, 
	kBluetoothHCIEventMaskQoSSetupComplete								= 0x00001000, 
	kBluetoothHCIEventMaskCommandComplete								= 0x00002000, 
	kBluetoothHCIEventMaskCommandStatus									= 0x00004000, 
	kBluetoothHCIEventMaskHardwareError									= 0x00008000, 
	kBluetoothHCIEventMaskFlushOccurred									= 0x00010000, 
	kBluetoothHCIEventMaskRoleChange									= 0x00020000, 
	
	kBluetoothHCIEventMaskNumberOfCompletedPackets						= 0x00040000, 
	kBluetoothHCIEventMaskModeChange									= 0x00080000, 
	kBluetoothHCIEventMaskReturnLinkKeys								= 0x00100000, 
	kBluetoothHCIEventMaskPINCodeRequest								= 0x00200000, 
	kBluetoothHCIEventMaskLinkKeyRequest								= 0x00400000, 
	kBluetoothHCIEventMaskLinkKeyNotification							= 0x00800000, 
	kBluetoothHCIEventMaskLoopbackCommand								= 0x01000000, 
	kBluetoothHCIEventMaskDataBufferOverflow							= 0x02000000, 
	kBluetoothHCIEventMaskMaxSlotsChange								= 0x04000000, 
	kBluetoothHCIEventMaskReadClockOffsetComplete						= 0x08000000, 
	kBluetoothHCIEventMaskConnectionPacketTypeChanged					= 0x10000000, 
	kBluetoothHCIEventMaskQoSViolation									= 0x20000000, 
	kBluetoothHCIEventMaskPageScanModeChange							= 0x40000000, 
	kBluetoothHCIEventMaskPageScanRepetitionModeChange					= 0x80000000, 
	
	kBluetoothHCIEventMaskAll											= 0xFFFFFFFF, 
	kBluetoothHCIEventMaskDefault										= kBluetoothHCIEventMaskAll		/* Use kBluetoothHCIEventMaskDefault64Bit above! */
};

// Event results structures.

typedef struct	BluetoothHCIEventConnectionCompleteResults		BluetoothHCIEventConnectionCompleteResults;
struct			BluetoothHCIEventConnectionCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothDeviceAddress						deviceAddress;
	BluetoothLinkType							linkType;
	BluetoothHCIEncryptionMode					encryptionMode;
};

typedef struct	BluetoothHCIEventLEConnectionCompleteResults		BluetoothHCIEventLEConnectionCompleteResults;
struct			BluetoothHCIEventLEConnectionCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
	uint8_t										role;
	uint8_t										peerAddressType;
	BluetoothDeviceAddress						peerAddress;
	uint16_t									connInterval;
	uint16_t									connLatency;
	uint16_t									supervisionTimeout;
	uint8_t										masterClockAccuracy;
} __attribute__((packed));
        
typedef struct    BluetoothHCIEventLEEnhancedConnectionCompleteResults        BluetoothHCIEventLEEnhancedConnectionCompleteResults;
struct            BluetoothHCIEventLEEnhancedConnectionCompleteResults
{
    BluetoothConnectionHandle                  connectionHandle;
    uint8_t                                    role;
    uint8_t                                    peerAddressType;
    BluetoothDeviceAddress                     peerAddress;
    BluetoothDeviceAddress                     localResolvablePrivateAddress;
    BluetoothDeviceAddress                     peerResolvablePrivateAddress;
    uint16_t                                   connInterval;
    uint16_t                                   connLatency;
    uint16_t                                   supervisionTimeout;
    uint8_t                                    masterClockAccuracy;
} __attribute__((packed));
	
typedef struct	BluetoothHCIEventLEConnectionUpdateCompleteResults		BluetoothHCIEventLEConnectionUpdateCompleteResults;
struct			BluetoothHCIEventLEConnectionUpdateCompleteResults
{
    BluetoothConnectionHandle					connectionHandle;
    uint16_t									connInterval;
    uint16_t									connLatency;
    uint16_t									supervisionTimeout;
} __attribute__((packed));
        
typedef struct  BluetoothHCIEventLEReadRemoteUsedFeaturesCompleteResults     BluetoothHCIEventLEReadRemoteUsedFeaturesCompleteResults;
struct          BluetoothHCIEventLEReadRemoteUsedFeaturesCompleteResults
{
    BluetoothConnectionHandle					connectionHandle;
    BluetoothHCISupportedFeatures				usedFeatures;
} __attribute__((packed));

typedef struct	BluetoothHCIEventDisconnectionCompleteResults		BluetoothHCIEventDisconnectionCompleteResults;
struct			BluetoothHCIEventDisconnectionCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothReasonCode							reason;
};

typedef struct	BluetoothHCIEventReadSupportedFeaturesResults 	BluetoothHCIEventReadSupportedFeaturesResults;
struct 			BluetoothHCIEventReadSupportedFeaturesResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothHCISupportedFeatures				supportedFeatures;
};

typedef struct	BluetoothHCIEventReadExtendedFeaturesResults 	BluetoothHCIEventReadExtendedFeaturesResults;
struct 			BluetoothHCIEventReadExtendedFeaturesResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothHCIExtendedFeaturesInfo			supportedFeaturesInfo;
};
		
typedef struct	BluetoothHCIEventReadRemoteVersionInfoResults 	BluetoothHCIEventReadRemoteVersionInfoResults;
struct 			BluetoothHCIEventReadRemoteVersionInfoResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothLMPVersion							lmpVersion;
	BluetoothManufacturerName					manufacturerName;
	BluetoothLMPSubversion						lmpSubversion;
};

typedef struct	BluetoothHCIEventRemoteNameRequestResults 		BluetoothHCIEventRemoteNameRequestResults;
struct 			BluetoothHCIEventRemoteNameRequestResults
{
	BluetoothDeviceAddress						deviceAddress;
	BluetoothDeviceName							deviceName;
};

typedef struct	BluetoothHCIEventReadClockOffsetResults 		BluetoothHCIEventReadClockOffsetResults;
struct			BluetoothHCIEventReadClockOffsetResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothClockOffset						clockOffset;
};

typedef struct	BluetoothHCIEventConnectionRequestResults 		BluetoothHCIEventConnectionRequestResults;
struct			BluetoothHCIEventConnectionRequestResults
{
	BluetoothDeviceAddress						deviceAddress;
	BluetoothClassOfDevice						classOfDevice;
	BluetoothLinkType							linkType;
};

typedef struct	BluetoothHCIEventLinkKeyNotificationResults 		BluetoothHCIEventLinkKeyNotificationResults;
struct			BluetoothHCIEventLinkKeyNotificationResults
{
	BluetoothDeviceAddress						deviceAddress;
	BluetoothKey								linkKey;
	BluetoothKeyType							keyType;
};

typedef struct	BluetoothHCIEventMaxSlotsChangeResults 		BluetoothHCIEventMaxSlotsChangeResults;
struct			BluetoothHCIEventMaxSlotsChangeResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothMaxSlots							maxSlots;
};

typedef struct	BluetoothHCIEventModeChangeResults 		BluetoothHCIEventModeChangeResults;
struct			BluetoothHCIEventModeChangeResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothHCIConnectionMode					mode;
	BluetoothHCIModeInterval					modeInterval;
};

typedef struct	BluetoothHCIEventReturnLinkKeysResults	BluetoothHCIEventReturnLinkKeysResults;
struct			BluetoothHCIEventReturnLinkKeysResults
{
	uint8_t										numLinkKeys;
	struct {
		BluetoothDeviceAddress					deviceAddress;
		BluetoothKey							linkKey;
	} linkKeys[1];
};

typedef struct	BluetoothHCIEventAuthenticationCompleteResults 		BluetoothHCIEventAuthenticationCompleteResults;
struct			BluetoothHCIEventAuthenticationCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
};

typedef struct	BluetoothHCIEventEncryptionChangeResults 		BluetoothHCIEventEncryptionChangeResults;
struct			BluetoothHCIEventEncryptionChangeResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothEncryptionEnable					enable;
};

typedef struct	BluetoothHCIEventChangeConnectionLinkKeyCompleteResults 	BluetoothHCIEventChangeConnectionLinkKeyCompleteResults;
struct			BluetoothHCIEventChangeConnectionLinkKeyCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
};

typedef struct	BluetoothHCIEventMasterLinkKeyCompleteResults 	BluetoothHCIEventMasterLinkKeyCompleteResults;
struct			BluetoothHCIEventMasterLinkKeyCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothKeyFlag							keyFlag;
};

typedef struct	BluetoothHCIEventQoSSetupCompleteResults 	BluetoothHCIEventQoSSetupCompleteResults;
struct			BluetoothHCIEventQoSSetupCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothHCIQualityOfServiceSetupParams		setupParams;
};

typedef struct	BluetoothHCIEventHardwareErrorResults 	BluetoothHCIEventHardwareErrorResults;
struct			BluetoothHCIEventHardwareErrorResults
{
	BluetoothHCIStatus							error;
};

typedef struct	BluetoothHCIEventFlushOccurredResults 	BluetoothHCIEventFlushOccurredResults;
struct			BluetoothHCIEventFlushOccurredResults
{
	BluetoothConnectionHandle					connectionHandle;
};

typedef struct	BluetoothHCIEventRoleChangeResults 	BluetoothHCIEventRoleChangeResults;
struct			BluetoothHCIEventRoleChangeResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothDeviceAddress						deviceAddress;
	BluetoothRole								role;
};

typedef struct	BluetoothHCIEventDataBufferOverflowResults 	BluetoothHCIEventDataBufferOverflowResults;
struct			BluetoothHCIEventDataBufferOverflowResults
{
	BluetoothLinkType							linkType;
};

typedef struct	BluetoothHCIEventConnectionPacketTypeResults 	BluetoothHCIEventConnectionPacketTypeResults;
struct			BluetoothHCIEventConnectionPacketTypeResults
{
	BluetoothConnectionHandle					connectionHandle;
	BluetoothPacketType							packetType;
};

typedef struct	BluetoothHCIEventReadRemoteSupportedFeaturesResults 	BluetoothHCIEventReadRemoteSupportedFeaturesResults;
struct			BluetoothHCIEventReadRemoteSupportedFeaturesResults
{
	BluetoothHCIStatus							error;
	BluetoothConnectionHandle					connectionHandle;
	BluetoothHCISupportedFeatures				lmpFeatures;
};

typedef struct	BluetoothHCIEventReadRemoteExtendedFeaturesResults 	BluetoothHCIEventReadRemoteExtendedFeaturesResults;
struct			BluetoothHCIEventReadRemoteExtendedFeaturesResults
{
	BluetoothHCIStatus							error;
	BluetoothConnectionHandle					connectionHandle;
	BluetoothHCIPageNumber						page;
	BluetoothHCIPageNumber						maxPage;
	BluetoothHCISupportedFeatures				lmpFeatures;
};

typedef struct	BluetoothHCIEventQoSViolationResults 	BluetoothHCIEventQoSViolationResults;
struct			BluetoothHCIEventQoSViolationResults
{
	BluetoothConnectionHandle					connectionHandle;
};

typedef struct	BluetoothHCIEventPageScanModeChangeResults 	BluetoothHCIEventPageScanModeChangeResults;
struct			BluetoothHCIEventPageScanModeChangeResults
{
	BluetoothDeviceAddress			deviceAddress;
	BluetoothPageScanMode			pageScanMode;
};

typedef struct	BluetoothHCIEventPageScanRepetitionModeChangeResults 	BluetoothHCIEventPageScanRepetitionModeChangeResults;
struct			BluetoothHCIEventPageScanRepetitionModeChangeResults
{
	BluetoothDeviceAddress				deviceAddress;
	BluetoothPageScanRepetitionMode		pageScanRepetitionMode;
};

typedef struct	BluetoothHCIEventVendorSpecificResults 	BluetoothHCIEventVendorSpecificResults;
struct			BluetoothHCIEventVendorSpecificResults
{
	uint8_t								length;
	uint8_t								data[255];
};

typedef struct	BluetoothHCIEventEncryptionKeyRefreshCompleteResults		BluetoothHCIEventEncryptionKeyRefreshCompleteResults;
struct			BluetoothHCIEventEncryptionKeyRefreshCompleteResults
{
	BluetoothConnectionHandle					connectionHandle;
};

typedef struct BluetoothHCIEventSniffSubratingResults BluetoothHCIEventSniffSubratingResults;
struct BluetoothHCIEventSniffSubratingResults
{
	BluetoothConnectionHandle			connectionHandle;
	uint16_t							maxTransmitLatency;
	uint16_t							maxReceiveLatency;
	uint16_t							minRemoteTimeout;
	uint16_t							minLocalTimeout;			
};
	
// LE Meta Events	
typedef struct BluetoothHCIEventLEMetaResults BluetoothHCIEventLEMetaResults;
struct BluetoothHCIEventLEMetaResults
{
	uint8_t		length;
	uint8_t		data[255];
};
		
typedef struct BluetoothHCIEventLELongTermKeyRequestResults BluetoothHCIEventLELongTermKeyRequestResults;
struct BluetoothHCIEventLELongTermKeyRequestResults
{
	BluetoothConnectionHandle	connectionHandle;
	uint8_t						randomNumber[8];
	uint16_t					ediv;	
};

#define kNoNotifyProc	NULL
#define kNoUserRefCon	NULL

typedef struct BluetoothHCIRequestCallbackInfo BluetoothHCIRequestCallbackInfo;
struct BluetoothHCIRequestCallbackInfo
{
	mach_vm_address_t					userCallback;			// Proc to call when async handler is called.
	mach_vm_address_t					userRefCon;				// For user's info.
	mach_vm_address_t					internalRefCon;			// For our purposes.
	mach_vm_address_t 					asyncIDRefCon;			// For our aync calls.
	mach_vm_address_t					reserved;				// For the future. Currently Unused.
};

// Error codes

enum
{
	kBluetoothHCIErrorSuccess											= 0x00, 
	kBluetoothHCIErrorUnknownHCICommand									= 0x01, 
	kBluetoothHCIErrorNoConnection										= 0x02, 
	kBluetoothHCIErrorHardwareFailure									= 0x03, 
	kBluetoothHCIErrorPageTimeout										= 0x04, 
	kBluetoothHCIErrorAuthenticationFailure								= 0x05, 
	kBluetoothHCIErrorKeyMissing										= 0x06, 
	kBluetoothHCIErrorMemoryFull										= 0x07, 
	kBluetoothHCIErrorConnectionTimeout									= 0x08, 
	kBluetoothHCIErrorMaxNumberOfConnections							= 0x09, 
	kBluetoothHCIErrorMaxNumberOfSCOConnectionsToADevice				= 0x0A, 
	kBluetoothHCIErrorACLConnectionAlreadyExists						= 0x0B, 
	kBluetoothHCIErrorCommandDisallowed									= 0x0C, 
	kBluetoothHCIErrorHostRejectedLimitedResources						= 0x0D, 
	kBluetoothHCIErrorHostRejectedSecurityReasons						= 0x0E, 
	kBluetoothHCIErrorHostRejectedRemoteDeviceIsPersonal				= 0x0F, 
	kBluetoothHCIErrorHostTimeout										= 0x10, 
	kBluetoothHCIErrorUnsupportedFeatureOrParameterValue				= 0x11, 
	kBluetoothHCIErrorInvalidHCICommandParameters						= 0x12, 
	kBluetoothHCIErrorOtherEndTerminatedConnectionUserEnded				= 0x13, 
	kBluetoothHCIErrorOtherEndTerminatedConnectionLowResources			= 0x14, 
	kBluetoothHCIErrorOtherEndTerminatedConnectionAboutToPowerOff		= 0x15, 
	kBluetoothHCIErrorConnectionTerminatedByLocalHost					= 0x16, 
	kBluetoothHCIErrorRepeatedAttempts									= 0x17, 
	kBluetoothHCIErrorPairingNotAllowed									= 0x18, 
	kBluetoothHCIErrorUnknownLMPPDU										= 0x19, 
	kBluetoothHCIErrorUnsupportedRemoteFeature							= 0x1A, 
	kBluetoothHCIErrorSCOOffsetRejected									= 0x1B, 
	kBluetoothHCIErrorSCOIntervalRejected								= 0x1C, 
	kBluetoothHCIErrorSCOAirModeRejected								= 0x1D, 
	kBluetoothHCIErrorInvalidLMPParameters								= 0x1E, 
	kBluetoothHCIErrorUnspecifiedError									= 0x1F, 
	kBluetoothHCIErrorUnsupportedLMPParameterValue						= 0x20, 
	kBluetoothHCIErrorRoleChangeNotAllowed								= 0x21, 
	kBluetoothHCIErrorLMPResponseTimeout								= 0x22, 
	kBluetoothHCIErrorLMPErrorTransactionCollision						= 0x23, 
	kBluetoothHCIErrorLMPPDUNotAllowed									= 0x24, 
	kBluetoothHCIErrorEncryptionModeNotAcceptable						= 0x25, 	// Added Core Spec, v1.1
	kBluetoothHCIErrorUnitKeyUsed										= 0x26, 	// 1.1
	kBluetoothHCIErrorQoSNotSupported									= 0x27, 	// 1.1
	kBluetoothHCIErrorInstantPassed										= 0x28, 	// 1.1
	kBluetoothHCIErrorPairingWithUnitKeyNotSupported					= 0x29, 	// 1.1
	kBluetoothHCIErrorHostRejectedUnacceptableDeviceAddress				= 0x0F,		// 2.0+
	kBluetoothHCIErrorDifferentTransactionCollision						= 0x2A, 	// 1.2
	kBluetoothHCIErrorQoSUnacceptableParameter							= 0x2C, 	// 1.2
	kBluetoothHCIErrorQoSRejected										= 0x2D, 	// 1.2
	kBluetoothHCIErrorChannelClassificationNotSupported					= 0x2E, 	// 1.2
	kBluetoothHCIErrorInsufficientSecurity								= 0x2F, 	// 1.2
	kBluetoothHCIErrorParameterOutOfMandatoryRange						= 0x30,		// 1.2
	kBluetoothHCIErrorRoleSwitchPending									= 0x31,		// 1.2
	kBluetoothHCIErrorReservedSlotViolation								= 0x34,		// 1.2
	kBluetoothHCIErrorRoleSwitchFailed									= 0x35,		// 1.2
	kBluetoothHCIErrorExtendedInquiryResponseTooLarge					= 0x36, 	// 2.1
	kBluetoothHCIErrorSecureSimplePairingNotSupportedByHost				= 0x37, 	// 2.1
	kBluetoothHCIErrorHostBusyPairing                                   = 0x38,
	kBluetoothHCIErrorConnectionRejectedDueToNoSuitableChannelFound		= 0x39,
	kBluetoothHCIErrorControllerBusy                                    = 0x3A,
	kBluetoothHCIErrorUnacceptableConnectionInterval                    = 0x3B,
	kBluetoothHCIErrorDirectedAdvertisingTimeout                        = 0x3C,
	kBluetoothHCIErrorConnectionTerminatedDueToMICFailure				= 0x3D,
	kBluetoothHCIErrorConnectionFailedToBeEstablished                   = 0x3E,
	kBluetoothHCIErrorMACConnectionFailed                               = 0x3F,
    kBluetoothHCIErrorCoarseClockAdjustmentRejected                     = 0x40,
    
    kBluetoothHCIErrorMax												= 0x40
};

#if 0
#pragma mark === HCI Power Mode ===
#endif

//===========================================================================================================================
//	HCI Power Mode 
//===========================================================================================================================

typedef enum
{
	kBluetoothHCIPowerStateON				= 0x01,
	kBluetoothHCIPowerStateOFF				= 0x00,
	kBluetoothHCIPowerStateUnintialized		= 0xFF,	
} BluetoothHCIPowerState;

enum
{
	kBluetoothHCIErrorPowerIsOFF			= (kBluetoothHCIErrorMax + 1)
};

#if 0
#pragma mark === HCI USB Transport ===
#endif

//===========================================================================================================================
//	HCI USB Transport
//===========================================================================================================================

//---------------------------------------------------------------------------------------------------------------------------
/*!	@enum		BluetoothHCIUSBDeviceMatchingConstants
	@abstract	Bluetooth USB device matching constants
	@constant	kBluetoothHCITransportUSBClassCode			Wireless Controller
	@constant	kBluetoothHCITransportUSBSubClassCode		RF Controller
	@constant	kBluetoothHCITransportUSBProtocolCode		Bluetooth Programming
*/

enum
{
	kBluetoothHCITransportUSBClassCode		= 0xE0,
	kBluetoothHCITransportUSBSubClassCode	= 0x01,
	kBluetoothHCITransportUSBProtocolCode	= 0x01
};

#if 0
#pragma mark === TCI - L2CAP ===
#endif

//===========================================================================================================================
//	TCI - L2CAP
//===========================================================================================================================

enum
{
	kBluetoothL2CAPTCIEventIDReserved							= 0x00,
	kBluetoothL2CAPTCIEventIDL2CA_ConnectInd					= 0x01,
	kBluetoothL2CAPTCIEventIDL2CA_ConfigInd						= 0x02,
	kBluetoothL2CAPTCIEventIDL2CA_DisconnectInd					= 0x03,
	kBluetoothL2CAPTCIEventIDL2CA_QoSViolationInd				= 0x04,
	kBluetoothL2CAPTCIEventIDL2CA_TimeOutInd					= 0x05
};

enum
{
	kBluetoothL2CAPTCICommandReserved							= 0x0000,
	kBluetoothL2CAPTCICommandL2CA_ConnectReq					= 0x0001,
	kBluetoothL2CAPTCICommandL2CA_DisconnectReq					= 0x0002,
	kBluetoothL2CAPTCICommandL2CA_ConfigReq						= 0x0003,
	kBluetoothL2CAPTCICommandL2CA_DisableCLT					= 0x0004,
	kBluetoothL2CAPTCICommandL2CA_EnableCLT						= 0x0005,
	kBluetoothL2CAPTCICommandL2CA_GroupCreate					= 0x0006,
	kBluetoothL2CAPTCICommandL2CA_GroupClose					= 0x0007,
	kBluetoothL2CAPTCICommandL2CA_GroupAddMember				= 0x0008,
	kBluetoothL2CAPTCICommandL2CA_GroupRemoveMember				= 0x0009,
	kBluetoothL2CAPTCICommandL2CA_GroupMembership				= 0x000A,
	kBluetoothL2CAPTCICommandL2CA_WriteData						= 0x000B,
	kBluetoothL2CAPTCICommandL2CA_ReadData						= 0x000C,
	kBluetoothL2CAPTCICommandL2CA_Ping							= 0x000D,
	kBluetoothL2CAPTCICommandL2CA_GetInfo						= 0x000E,
	kBluetoothL2CAPTCICommandL2CA_Reserved1						= 0x000F,
	kBluetoothL2CAPTCICommandL2CA_Reserved2						= 0x0010,
	kBluetoothL2CAPTCICommandL2CA_ConnectResp					= 0x0011,
	kBluetoothL2CAPTCICommandL2CA_DisconnectResp				= 0x0012,
	kBluetoothL2CAPTCICommandL2CA_ConfigResp					= 0x0013
};

#if 0
#pragma mark -
#pragma mark === RFCOMM ===
#endif

//===========================================================================================================================
//	RFCOMM
//===========================================================================================================================
#define kMaxChannelIDPerSide	31

typedef uint8_t	BluetoothRFCOMMChannelID;

#define	RFCOMM_CHANNEL_ID_IS_VALID( CHANNEL ) (( CHANNEL >= 1 ) && ( CHANNEL <= 30 ))

typedef uint16_t	BluetoothRFCOMMMTU;

typedef enum BluetoothRFCOMMParityType
{
    kBluetoothRFCOMMParityTypeNoParity = 0, 
    kBluetoothRFCOMMParityTypeOddParity, 
    kBluetoothRFCOMMParityTypeEvenParity, 
    kBluetoothRFCOMMParityTypeMaxParity
} BluetoothRFCOMMParityType;

typedef enum BluetoothRFCOMMLineStatus
{
    BluetoothRFCOMMLineStatusNoError = 0, 
    BluetoothRFCOMMLineStatusOverrunError, 
    BluetoothRFCOMMLineStatusParityError, 
    BluetoothRFCOMMLineStatusFramingError
} BluetoothRFCOMMLineStatus;

#if 0
#pragma mark -
#pragma mark === SDP ===
#endif

//===========================================================================================================================
//	SDP
//===========================================================================================================================

typedef uint8_t	BluetoothSDPPDUID;
enum {
    kBluetoothSDPPDUIDReserved							= 0,
    kBluetoothSDPPDUIDErrorResponse						= 1,
    kBluetoothSDPPDUIDServiceSearchRequest				= 2,
    kBluetoothSDPPDUIDServiceSearchResponse				= 3,
    kBluetoothSDPPDUIDServiceAttributeRequest			= 4,
    kBluetoothSDPPDUIDServiceAttributeResponse			= 5,
    kBluetoothSDPPDUIDServiceSearchAttributeRequest		= 6,
    kBluetoothSDPPDUIDServiceSearchAttributeResponse	= 7
};

#define IS_REQUEST_PDU( _pduID ) (	( _pduID == kBluetoothSDPPDUIDServiceSearchRequest ) || \
                                    ( _pduID == kBluetoothSDPPDUIDServiceAttributeRequest ) || \
                                    ( _pduID == kBluetoothSDPPDUIDServiceSearchAttributeRequest ) )
                                    
#define IS_RESPONSE_PDU( _pduID ) (	( _pduID == kBluetoothSDPPDUIDErrorResponse ) || \
                                    ( _pduID == kBluetoothSDPPDUIDServiceSearchResponse ) || \
                                    ( _pduID == kBluetoothSDPPDUIDServiceAttributeResponse ) || \
                                    ( _pduID == kBluetoothSDPPDUIDServiceSearchAttributeResponse ) )

typedef uint16_t	BluetoothSDPErrorCode;
enum {
    kBluetoothSDPErrorCodeSuccess						= 0x0000,
    kBluetoothSDPErrorCodeReserved						= 0x0000,
    kBluetoothSDPErrorCodeInvalidSDPVersion				= 0x0001,
    kBluetoothSDPErrorCodeInvalidServiceRecordHandle	= 0x0002,
    kBluetoothSDPErrorCodeInvalidRequestSyntax			= 0x0003,
    kBluetoothSDPErrorCodeInvalidPDUSize				= 0x0004,
    kBluetoothSDPErrorCodeInvalidContinuationState		= 0x0005,
    kBluetoothSDPErrorCodeInsufficientResources			= 0x0006,
    
    kBluetoothSDPErrorCodeReservedStart					= 0x0007,
    kBluetoothSDPErrorCodeReservedEnd					= 0xFFFF
};

typedef uint16_t	BluetoothSDPTransactionID;

typedef uint32_t	BluetoothSDPServiceRecordHandle;

enum {
    kBluetoothSDPDataElementTypeNil						= 0,
    kBluetoothSDPDataElementTypeUnsignedInt				= 1,
    kBluetoothSDPDataElementTypeSignedInt				= 2,
    kBluetoothSDPDataElementTypeUUID					= 3,
    kBluetoothSDPDataElementTypeString					= 4,
    kBluetoothSDPDataElementTypeBoolean					= 5,
    kBluetoothSDPDataElementTypeDataElementSequence		= 6,
    kBluetoothSDPDataElementTypeDataElementAlternative	= 7,
    kBluetoothSDPDataElementTypeURL						= 8,
    kBluetoothSDPDataElementTypeReservedStart			= 9,
    kBluetoothSDPDataElementTypeReservedEnd				= 31
};

typedef uint16_t	BluetoothSDPUUID16;
typedef uint32_t	BluetoothSDPUUID32;

typedef uint8_t		BluetoothSDPDataElementTypeDescriptor;
typedef uint8_t		BluetoothSDPDataElementSizeDescriptor;

typedef uint16_t	BluetoothSDPServiceAttributeID;

#if 0
#pragma mark -
#pragma mark === LE ===
#endif
        
typedef enum {
    BluetoothLEScanTypePassive	= 0x00,
    BluetoothLEScanTypeActive	= 0x01
} BluetoothLEScanType;

typedef enum {
    BluetoothLEAddressTypePublic	= 0x00,
    BluetoothLEAddressTypeRandom	= 0x01
} BluetoothLEAddressType;
        
typedef enum {
    BluetoothLEScanFilterNone		= 0x00,
    BluetoothLEScanFilterSafelist	= 0x01,
	BluetoothLEScanFilterWhitelist __attribute__ ((deprecated)) = BluetoothLEScanFilterSafelist
} BluetoothLEScanFilter;
        
typedef enum {
    BluetoothLEScanDisable	= 0x00,
    BluetoothLEScanEnable	= 0x01
} BluetoothLEScan;
		
typedef enum {
	BluetoothLEConnectionIntervalMin	= 0x06,
	BluetoothLEConnectionIntervalMax	= 0x0C80
} BluetoothLEConnectionInterval;
		
typedef enum {
    BluetoothLEScanDuplicateFilterDisable	= 0x00,
    BluetoothLEScanDuplicateFilterEnable	= 0x01,
} BluetoothLEScanDuplicateFilter;
        
typedef enum {
    BluetoothLEAdvertisingTypeConnectableUndirected		= 0x00,
    BluetoothLEAdvertisingTypeConnectableDirected		= 0x01,
    BluetoothLEAdvertisingTypeDiscoverableUndirected	= 0x02,
    BluetoothLEAdvertisingTypeNonConnectableUndirected	= 0x03,
    BluetoothLEAdvertisingTypeScanResponse				= 0x04
} BluetoothLEAdvertisingType;

#ifdef	__cplusplus
	}
#endif
