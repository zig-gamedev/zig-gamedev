/*
	File:		BluetoothAssignedNumbers.h
	Copyright:	(c) 2001-2017 by Apple, Inc. All rights reserved.
*/

#pragma once

#ifdef	__cplusplus
	extern "C" {
#endif

#pragma mark -
#pragma mark === General ===

//==================================================================================================================
//	General
//==================================================================================================================

/*
 BluetoothCompanyIdentifers
 
 The "living document" can be found on the Bluetooth SIG website:
 https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers
 */

enum BluetoothCompanyIdentifers
{
	kBluetoothCompanyIdentiferEricssonTechnologyLicensing						=	0,
	kBluetoothCompanyIdentiferNokiaMobilePhones									=	1,
	kBluetoothCompanyIdentiferIntel												=	2,
	kBluetoothCompanyIdentiferIBM												=	3,
	kBluetoothCompanyIdentiferToshiba											=	4,
	kBluetoothCompanyIdentifer3Com												=	5,
	kBluetoothCompanyIdentiferMicrosoft											=	6,
	kBluetoothCompanyIdentiferLucent											=	7,
	kBluetoothCompanyIdentiferMotorola											=	8,
	kBluetoothCompanyIdentiferInfineonTechnologiesAG							=	9,
	kBluetoothCompanyIdentiferCambridgeSiliconRadio								=	10,
	kBluetoothCompanyIdentiferSiliconWave										=	11,
	kBluetoothCompanyIdentiferDigianswerAS										=	12,
	kBluetoothCompanyIdentiferTexasInstruments									=	13,
	kBluetoothCompanyIdentiferParthusTechnologies								=	14,
	kBluetoothCompanyIdentiferBroadcom											=	15,
	kBluetoothCompanyIdentiferMitelSemiconductor								=	16,
	kBluetoothCompanyIdentiferWidcomm											=	17,
	kBluetoothCompanyIdentiferZeevo												=	18,
	kBluetoothCompanyIdentiferAtmel												=	19,
	kBluetoothCompanyIdentiferMistubishiElectric								=	20,
	kBluetoothCompanyIdentiferRTXTelecom										=	21,
	kBluetoothCompanyIdentiferKCTechnology										=	22,
	kBluetoothCompanyIdentiferNewlogic											=	23,
	kBluetoothCompanyIdentiferTransilica										=	24,
	kBluetoothCompanyIdentiferRohdeandSchwarz									=	25,
	kBluetoothCompanyIdentiferTTPCom											=	26,
	kBluetoothCompanyIdentiferSigniaTechnologies								=	27,
	kBluetoothCompanyIdentiferConexantSystems									=	28,
	kBluetoothCompanyIdentiferQualcomm											=	29,
	kBluetoothCompanyIdentiferInventel											=	30,
	kBluetoothCompanyIdentiferAVMBerlin											=	31,
	kBluetoothCompanyIdentiferBandspeed											=	32,
	kBluetoothCompanyIdentiferMansella											=	33,
	kBluetoothCompanyIdentiferNEC												=	34,
	kBluetoothCompanyIdentiferWavePlusTechnology								=	35,
	kBluetoothCompanyIdentiferAlcatel											=	36,
	kBluetoothCompanyIdentiferPhilipsSemiconductor								=	37,
	kBluetoothCompanyIdentiferCTechnologies										=	38,
	kBluetoothCompanyIdentiferOpenInterface										=	39,
	kBluetoothCompanyIdentiferRFCMicroDevices									=	40,
	kBluetoothCompanyIdentiferHitachi											=	41,
	kBluetoothCompanyIdentiferSymbolTechnologies								=	42,
	kBluetoothCompanyIdentiferTenovis											=	43,
	kBluetoothCompanyIdentiferMacronixInternational								=	44,
	kBluetoothCompanyIdentiferGCTSemiconductor									=	45,
	kBluetoothCompanyIdentiferNorwoodSystems									=	46,
	kBluetoothCompanyIdentiferMewTelTechnology									=	47,
	kBluetoothCompanyIdentiferSTMicroelectronics								=	48,
	kBluetoothCompanyIdentiferSynopsys											=	49,
	kBluetoothCompanyIdentiferRedMCommunications								=	50,
	kBluetoothCompanyIdentiferCommil											=	51,
	kBluetoothCompanyIdentiferCATC												=	52,
	kBluetoothCompanyIdentiferEclipse											=	53,
	kBluetoothCompanyIdentiferRenesasTechnology									=	54,
	kBluetoothCompanyIdentiferMobilian											=	55,
	kBluetoothCompanyIdentiferTerax												=	56,
	kBluetoothCompanyIdentiferIntegratedSystemSolution							=	57,
	kBluetoothCompanyIdentiferMatsushitaElectricIndustrial						=	58,
	kBluetoothCompanyIdentiferGennum											=	59,
	kBluetoothCompanyIdentiferResearchInMotion									=	60,
	kBluetoothCompanyIdentiferIPextreme											=	61,
	kBluetoothCompanyIdentiferSystemsAndChips									=	62,
	kBluetoothCompanyIdentiferBluetoothSIG										=	63,
	kBluetoothCompanyIdentiferSeikoEpson										=	64,
	kBluetoothCompanyIdentiferIntegratedSiliconSolution							=	65,
	kBluetoothCompanyIdentiferCONWISETechnology									=	66,
	kBluetoothCompanyIdentiferParrotSA											=	67,
	kBluetoothCompanyIdentiferSocketCommunications								=	68,
	kBluetoothCompanyIdentiferAtherosCommunications								=	69,
	kBluetoothCompanyIdentiferMediaTek											=	70,
	kBluetoothCompanyIdentiferBluegiga											=	71,
	kBluetoothCompanyIdentiferMarvellTechnologyGroup							=	72,
	kBluetoothCompanyIdentifer3DSP												=	73,
	kBluetoothCompanyIdentiferAccelSemiconductor								=	74,
	kBluetoothCompanyIdentiferContinentialAutomotiveSystems						=	75,
	kBluetoothCompanyIdentiferApple												=	76,
	kBluetoothCompanyIdentiferStaccatoCommunications							=	77,
	kBluetoothCompanyIdentiferAvagoTechnologies									=	78,
	kBluetoothCompanyIdentiferAPT												=	79,
	kBluetoothCompanyIdentiferSiRFTechnology									=	80,
	kBluetoothCompanyIdentiferTZeroTechnologies									=	81,
	kBluetoothCompanyIdentiferJandM												=	82,
	kBluetoothCompanyIdentiferFree2Move											=	83,
	kBluetoothCompanyIdentifer3DiJoy											=	84,
	kBluetoothCompanyIdentiferPlantronics										=	85,
	kBluetoothCompanyIdentiferSonyEricssonMobileCommunications					=	86,
	kBluetoothCompanyIdentiferHarmonInternational								=	87,
	kBluetoothCompanyIdentiferVisio												=	88,
	kBluetoothCompanyIdentiferNordicSemiconductor								=	89,
	kBluetoothCompanyIdentiferEMMicroElectronicMarin							=	90,
	kBluetoothCompanyIdentiferRalinkTechnology									=	91,
	kBluetoothCompanyIdentiferBelkinInternational								=	92,
	kBluetoothCompanyIdentiferRealtekSemiconductor								=	93,
	kBluetoothCompanyIdentiferStonestreetOne									=	94,
	kBluetoothCompanyIdentiferWicentric											=	95,
	kBluetoothCompanyIdentiferRivieraWaves										=	96,
	kBluetoothCompanyIdentiferRDAMicroelectronics								=	97,
	kBluetoothCompanyIdentiferGibsonGuitars										=	98,
	kBluetoothCompanyIdentiferMiCommand											=	99,
	kBluetoothCompanyIdentiferBandXIInternational								=	100,
	kBluetoothCompanyIdentiferHewlettPackard									=	101,
	kBluetoothCompanyIdentifer9SolutionsOy										=	102,
	kBluetoothCompanyIdentiferGNNetcom											=	103,
	kBluetoothCompanyIdentiferGeneralMotors										=	104,
	kBluetoothCompanyIdentiferAAndDEngineering									=	105,
	kBluetoothCompanyIdentiferMindTree											=	106,
	kBluetoothCompanyIdentiferPolarElectroOY									=	107,
	kBluetoothCompanyIdentiferBeautifulEnterprise								=	108,
	kBluetoothCompanyIdentiferBriarTek											=	109,
	kBluetoothCompanyIdentiferSummitDataCommunications							=	110,
	kBluetoothCompanyIdentiferSoundID											=	111,
	kBluetoothCompanyIdentiferMonster											=	112,
	kBluetoothCompanyIdentiferConnectBlueAB										=	113,
	kBluetoothCompanyIdentiferShangHaiSuperSmartElectronics						=	114,
	kBluetoothCompanyIdentiferGroupSense										=	115,
	kBluetoothCompanyIdentiferZomm												=	116,
	kBluetoothCompanyIdentiferSamsungElectronics								=	117,
	kBluetoothCompanyIdentiferCreativeTechnology								=	118,
	kBluetoothCompanyIdentiferLairdTechnologies									=	119,
	kBluetoothCompanyIdentiferNike												=	120,
	kBluetoothCompanyIdentiferLessWire											=	121,
	kBluetoothCompanyIdentiferMStarTechnologies									=	122,
	kBluetoothCompanyIdentiferHanlynnTechnologies								=	123,
	kBluetoothCompanyIdentiferAAndRCambridge									=	124,
	kBluetoothCompanyIdentiferSeersTechnology									=	125,
	kBluetoothCompanyIdentiferSportsTrackingTechnologies						=	126,
	kBluetoothCompanyIdentiferAutonetMobile										=	127,
	kBluetoothCompanyIdentiferDeLormePublishingCompany							=	128,
	kBluetoothCompanyIdentiferWuXiVimicro										=	129,
	kBluetoothCompanyIdentiferSennheiserCommunications							=	130,
	kBluetoothCompanyIdentiferTimeKeepingSystems								=	131,
	kBluetoothCompanyIdentiferLudusHelsinki										=	132,
	kBluetoothCompanyIdentiferBlueRadios										=	133,
	kBluetoothCompanyIdentiferEquinux											=	134,
	kBluetoothCompanyIdentiferGarminInternational								=	135,
	kBluetoothCompanyIdentiferEcotest											=	136,
	kBluetoothCompanyIdentiferGNResound											=	137,
	kBluetoothCompanyIdentiferJawbone											=	138,
	kBluetoothCompanyIdentiferTopconPositioningSystems							=	139,
	kBluetoothCompanyIdentiferGimbal											=	140,
	kBluetoothCompanyIdentiferZscanSoftware										=	141,
	kBluetoothCompanyIdentiferQuintic											=	142,
	kBluetoothCompanyIdentiferTelitWirelessSolutions							=	143,
	kBluetoothCompanyIdentiferFunaiElectric										=	144,
	kBluetoothCompanyIdentiferAdvancedPANMOBILSystems							=	145,
	kBluetoothCompanyIdentiferThinkOptics										=	146,
	kBluetoothCompanyIdentiferUniversalElectriconics							=	147,
	kBluetoothCompanyIdentiferAirohaTechnology									=	148,
	kBluetoothCompanyIdentiferNECLightning										=	149,
	kBluetoothCompanyIdentiferODMTechnology										=	150,
	kBluetoothCompanyIdentiferConnecteDevice									=	151,
	kBluetoothCompanyIdentiferZero1TV											=	152,
	kBluetoothCompanyIdentiferITechDynamicGlobalDistribution					=	153,
	kBluetoothCompanyIdentiferAlpwise											=	154,
	kBluetoothCompanyIdentiferJiangsuToppowerAutomotiveElectronics				=	155,
	kBluetoothCompanyIdentiferColorfy											=	156,
	kBluetoothCompanyIdentiferGeoforce											=	157,
	kBluetoothCompanyIdentiferBose												=	158,
	kBluetoothCompanyIdentiferSuuntoOy											=	159,
	kBluetoothCompanyIdentiferKensingtonComputerProductsGroup					=	160,
	kBluetoothCompanyIdentiferSRMedizinelektronik								=	161,
	kBluetoothCompanyIdentiferVertu												=	162,
	kBluetoothCompanyIdentiferMetaWatch											=	163,
	kBluetoothCompanyIdentiferLinak												=	164,
	kBluetoothCompanyIdentiferOTLDynamics										=	165,
	kBluetoothCompanyIdentiferPandaOcean										=	166,
	kBluetoothCompanyIdentiferVisteon											=	167,
	kBluetoothCompanyIdentiferARPDevicesUnlimited								=	168,
	kBluetoothCompanyIdentiferMagnetiMarelli									=	169,
	kBluetoothCompanyIdentiferCaenRFID											=	170,
	kBluetoothCompanyIdentiferIngenieurSystemgruppeZahn							=	171,
	kBluetoothCompanyIdentiferGreenThrottleGames								=	172,
	kBluetoothCompanyIdentiferPeterSystemtechnik								=	173,
	kBluetoothCompanyIdentiferOmegawave											=	174,
	kBluetoothCompanyIdentiferCinetix											=	175,
	kBluetoothCompanyIdentiferPassifSemiconductor								=	176,
	kBluetoothCompanyIdentiferSarisCyclingGroup									=	177,
	kBluetoothCompanyIdentiferBekey												=	178,
	kBluetoothCompanyIdentiferClarinoxTechnologies								=	179,
	kBluetoothCompanyIdentiferBDETechnology										=	180,
	kBluetoothCompanyIdentiferSwirlNetworks										=	181,
	kBluetoothCompanyIdentiferMesoInternational									=	182,
	kBluetoothCompanyIdentiferTreLab											=	183,
	kBluetoothCompanyIdentiferQualcommInnovationCenter							=	184,
	kBluetoothCompanyIdentiferJohnsonControls									=	185,
	kBluetoothCompanyIdentiferStarkeyLaboratories								=	186,
	kBluetoothCompanyIdentiferSPowerElectronics									=	187,
	kBluetoothCompanyIdentiferAceSensor											=	188,
	kBluetoothCompanyIdentiferAplix												=	189,
	kBluetoothCompanyIdentiferAAMPofAmerica										=	190,
	kBluetoothCompanyIdentiferStalmartTechnology								=	191,
	kBluetoothCompanyIdentiferAMICCOMElectronics								=	192,
	kBluetoothCompanyIdentiferShenzhenExcelsecuDataTechnology					=	193,
	kBluetoothCompanyIdentiferGeneq												=	194,
	kBluetoothCompanyIdentiferAdidas											=	195,
	kBluetoothCompanyIdentiferLGElectronics										=	196,
	kBluetoothCompanyIdentiferOnsetComputer										=	197,
	kBluetoothCompanyIdentiferSelflyBV											=	198,
	kBluetoothCompanyIdentiferQuupa												=	199,
	kBluetoothCompanyIdentiferGeLo												=	200,
	kBluetoothCompanyIdentiferEvluma											=	201,
	kBluetoothCompanyIdentiferMC10												=	202,
	kBluetoothCompanyIdentiferBinauricSE										=	203,
	kBluetoothCompanyIdentiferBeatsElectronics									=	204,
	kBluetoothCompanyIdentiferMicrochipTechnology								=	205,
	kBluetoothCompanyIdentiferElgatoSystems										=	206,
	kBluetoothCompanyIdentiferARCHOS											=	207,
	kBluetoothCompanyIdentiferDexcom											=	208,
	kBluetoothCompanyIdentiferPolarElectroEurope								=	209,
	kBluetoothCompanyIdentiferDialogSemiconductor								=	210,
	kBluetoothCompanyIdentiferTaixingbangTechnology								=	211,
	kBluetoothCompanyIdentiferKawantech											=	212,
	kBluetoothCompanyIdentiferAustcoCommunicationsSystems						=	213,
	kBluetoothCompanyIdentiferTimexGroup										=	214,
	kBluetoothCompanyIdentiferQualcommTechnologies								=	215,
	kBluetoothCompanyIdentiferQualcommConnectedExperiences						=	216,
	kBluetoothCompanyIdentiferVoyetraTurtleBeach								=	217,
	kBluetoothCompanyIdentifertxtrGMBH											=	218,
	kBluetoothCompanyIdentiferBiosentronics										=	219,
	kBluetoothCompanyIdentiferProctorAndGamble									=	220,
	kBluetoothCompanyIdentiferHosiden											=	221,
	kBluetoothCompanyIdentiferMusik												=	222,
	kBluetoothCompanyIdentiferMisfitWearables									=	223,
	kBluetoothCompanyIdentiferGoogle											=	224,
	kBluetoothCompanyIdentiferDanlers											=	225,
	kBluetoothCompanyIdentiferSemilink											=	226,
	kBluetoothCompanyIdentiferInMusicBrands										=	227,
	kBluetoothCompanyIdentiferLSResearch										=	228,
	kBluetoothCompanyIdentiferEdenSoftwareConsultants							=	229,
	kBluetoothCompanyIdentiferFreshtemp											=	230,
	kBluetoothCompanyIdentiferKSTechnologies									=	231,
	kBluetoothCompanyIdentiferACTSTechnologies									=	232,
	kBluetoothCompanyIdentiferVtrackSystems										=	233,
	kBluetoothCompanyIdentiferNielsenKellerman									=	234,
	kBluetoothCompanyIdentiferServerTechnology									=	235,
	kBluetoothCompanyIdentiferBioResearchAssociates								=	236,
	kBluetoothCompanyIdentiferJollyLogic										=	237,
	kBluetoothCompanyIdentiferAboveAverageOutcomes								=	238,
	kBluetoothCompanyIdentiferBitsplitters										=	239,
	kBluetoothCompanyIdentiferPayPal											=	240,
	kBluetoothCompanyIdentiferWitronTechnology									=	241,
	kBluetoothCompanyIdentiferMorseProject										=	242,
	kBluetoothCompanyIdentiferKentDisplays										=	243,
	kBluetoothCompanyIdentiferNautilus											=	244,
	kBluetoothCompanyIdentiferSmartifier										=	245,
	kBluetoothCompanyIdentiferElcometer											=	246,
	kBluetoothCompanyIdentiferVSNTechnologies									=	247,
	kBluetoothCompanyIdentiferAceUni											=	248,
	kBluetoothCompanyIdentiferStickNFind										=	249,
	kBluetoothCompanyIdentiferCrystalCode										=	250,
	kBluetoothCompanyIdentiferKOUKAMM											=	251,
	kBluetoothCompanyIdentiferDelphi											=	252,
	kBluetoothCompanyIdentiferValenceTech										=	253,
	kBluetoothCompanyIdentiferStanleyBlackAndDecker								=	254,
	kBluetoothCompanyIdentiferTypeProducts										=	255,
	kBluetoothCompanyIdentiferTomTomInternational								=	256,
	kBluetoothCompanyIdentiferFuGoo												=	257,
	kBluetoothCompanyIdentiferKeiser											=	258,
	kBluetoothCompanyIdentiferBangAndOlufson									=	259,
	kBluetoothCompanyIdentiferPLUSLocationSystems								=	260,
	kBluetoothCompanyIdentiferUbiquitousComputingTechnology						=	261,
	kBluetoothCompanyIdentiferInnovativeYachtterSolutions						=	262,
	kBluetoothCompanyIdentiferWilliamDemantHolding								=	263,
	
	kBluetoothCompanyIdentiferInteropIdentifier									=	65535
};


#pragma mark -
#pragma mark === Baseband ===

//==================================================================================================================
//	Baseband
//==================================================================================================================

//
// Service Class Major
//

enum
{
	kBluetoothServiceClassMajorLimitedDiscoverableMode		= 0x001,	// Bit 13 - Limited Discoverable Mode
	kBluetoothServiceClassMajorReserved1					= 0x002, 	// Bit 14 - Reserved for future use.
	kBluetoothServiceClassMajorReserved2					= 0x004, 	// Bit 15 - Reserved for future use.
	kBluetoothServiceClassMajorPositioning					= 0x008, 	// Bit 16 - Positioning (Location ID)
	kBluetoothServiceClassMajorNetworking					= 0x010, 	// Bit 17 - LAN, Ad hoc, etc...
	kBluetoothServiceClassMajorRendering					= 0x020, 	// Bit 18 - Printing, Speaker, etc...
	kBluetoothServiceClassMajorCapturing					= 0x040,	// Bit 19 - Scanner, Microphone, etc...
	kBluetoothServiceClassMajorObjectTransfer				= 0x080,	// Bit 20 - v-Inbox, v-Folder, etc...
	kBluetoothServiceClassMajorAudio						= 0x100,	// Bit 21 - Speaker, Microphone, Headset, etc...
	kBluetoothServiceClassMajorTelephony					= 0x200,	// Bit 22 - Cordless telephony, Modem, Headset, etc...
	kBluetoothServiceClassMajorInformation					= 0x400,	// Bit 23 - Web server, WAP server, etc...
	
	kBluetoothServiceClassMajorAny							= '****',	// Pseudo-class - means anything acceptable.
	kBluetoothServiceClassMajorNone							= 'none',	// Pseudo-class - means no matching.
	
	kBluetoothServiceClassMajorEnd
};

//
// Device Class Major
//

enum
{
	kBluetoothDeviceClassMajorMiscellaneous					= 0x00, 	// [00000] Miscellaneous
	kBluetoothDeviceClassMajorComputer						= 0x01, 	// [00001] Desktop, Notebook, PDA, Organizers, etc...
	kBluetoothDeviceClassMajorPhone							= 0x02, 	// [00010] Cellular, Cordless, Payphone, Modem, etc...
	kBluetoothDeviceClassMajorLANAccessPoint				= 0x03, 	// [00011] LAN Access Point
    kBluetoothDeviceClassMajorAudio							= 0x04, 	// [00100] Headset, Speaker, Stereo, etc...
	kBluetoothDeviceClassMajorPeripheral					= 0x05, 	// [00101] Mouse, Joystick, Keyboards, etc...
    kBluetoothDeviceClassMajorImaging						= 0x06,		// [00110] Printing, scanner, camera, display, etc...
	kBluetoothDeviceClassMajorWearable						= 0x07,		// [00111] Wearable
	kBluetoothDeviceClassMajorToy							= 0x08,		// [01000] Toy
	kBluetoothDeviceClassMajorHealth						= 0x09,		// [01001] Health devices
	kBluetoothDeviceClassMajorUnclassified					= 0x1F, 	// [11111] Specific device code not assigned
	
	// Range 0x06 to 0x1E Reserved for future use.
	
	kBluetoothDeviceClassMajorAny							= '****',	// Pseudo-class - means anything acceptable.
	kBluetoothDeviceClassMajorNone							= 'none',	// Pseudo-class - means no matching.
	
	kBluetoothDeviceClassMajorEnd
};

//
// Device Class Minor
//

enum
{
	///
	/// Computer Minor Class
	///
	
	kBluetoothDeviceClassMinorComputerUnclassified			= 0x00, 	// [000000] Specific device code not assigned
	kBluetoothDeviceClassMinorComputerDesktopWorkstation	= 0x01, 	// [000001] Desktop workstation
	kBluetoothDeviceClassMinorComputerServer				= 0x02, 	// [000010] Server-class computer
	kBluetoothDeviceClassMinorComputerLaptop				= 0x03, 	// [000011] Laptop
	kBluetoothDeviceClassMinorComputerHandheld				= 0x04, 	// [000100] Handheld PC/PDA (clam shell)
	kBluetoothDeviceClassMinorComputerPalmSized				= 0x05, 	// [000101] Palm-sized PC/PDA
    kBluetoothDeviceClassMinorComputerWearable				= 0x06,		// [000110] Wearable computer (watch sized)
	
	// Range 0x06 to 0x7F Reserved for future use.
	
    
	///
	/// Phone Minor Class
	///
	
	kBluetoothDeviceClassMinorPhoneUnclassified				= 0x00, 	// [000000] Specific device code not assigned
	kBluetoothDeviceClassMinorPhoneCellular					= 0x01, 	// [000001] Cellular
	kBluetoothDeviceClassMinorPhoneCordless					= 0x02, 	// [000010] Cordless
	kBluetoothDeviceClassMinorPhoneSmartPhone				= 0x03, 	// [000011] Smart phone
	kBluetoothDeviceClassMinorPhoneWiredModemOrVoiceGateway	= 0x04, 	// [000100] Wired modem or voice gateway
    kBluetoothDeviceClassMinorPhoneCommonISDNAccess			= 0x05,		// [000101] Common ISDN Access
	
	// Range 0x05 to 0x7F Reserved for future use.
	
    
	///
	/// LAN Access Point Minor Class
	///
	
	// See the Bluetooth specification for LAN Access Point minor classes, which are broken into bits 5-7 for utilization and bits 2-4 for class.
	
    
	///
	/// Audio Minor Class
	///
	
	kBluetoothDeviceClassMinorAudioUnclassified					= 0x00, 	// [000000] Specific device code not assigned
	kBluetoothDeviceClassMinorAudioHeadset						= 0x01, 	// [000001] Device conforms to the Headset profile
    kBluetoothDeviceClassMinorAudioHandsFree					= 0x02,		// [000010] Hands-free
    kBluetoothDeviceClassMinorAudioReserved1					= 0x03,		// [000011] Reserved
    kBluetoothDeviceClassMinorAudioMicrophone					= 0x04,		// [000100] Microphone
    kBluetoothDeviceClassMinorAudioLoudspeaker					= 0x05,		// [000101] Loudspeaker
    kBluetoothDeviceClassMinorAudioHeadphones					= 0x06,		// [000110] Headphones
    kBluetoothDeviceClassMinorAudioPortable						= 0x07,		// [000111] Portable Audio
    kBluetoothDeviceClassMinorAudioCar							= 0x08,		// [001000] Car Audio
    kBluetoothDeviceClassMinorAudioSetTopBox					= 0x09,		// [001001] Set-top box
    kBluetoothDeviceClassMinorAudioHiFi							= 0x0a,		// [001010] HiFi Audio Device
    kBluetoothDeviceClassMinorAudioVCR							= 0x0b,		// [001011] VCR
    kBluetoothDeviceClassMinorAudioVideoCamera					= 0x0c,		// [001100] Video Camera
    kBluetoothDeviceClassMinorAudioCamcorder					= 0x0d,		// [001101] Camcorder
    kBluetoothDeviceClassMinorAudioVideoMonitor					= 0x0e,		// [001110] Video Monitor
    kBluetoothDeviceClassMinorAudioVideoDisplayAndLoudspeaker	= 0x0f,		// [001111] Video Display and Loudspeaker
    kBluetoothDeviceClassMinorAudioVideoConferencing			= 0x10,		// [010000] Video Conferencing
    kBluetoothDeviceClassMinorAudioReserved2					= 0x11,		// [010001] Reserved
    kBluetoothDeviceClassMinorAudioGamingToy					= 0x12,		// [010010] Gaming/Toy
    
	
	// Range 0x13 to 0x7F Reserved for future use.
	
    
	///
	/// Peripheral Minor Class
	///
    
    // Peripheral1 subclass is bits 7 & 6
    
    kBluetoothDeviceClassMinorPeripheral1Keyboard			= 0x10,		// [01XXXX] Keyboard
    kBluetoothDeviceClassMinorPeripheral1Pointing			= 0x20,		// [10XXXX] Pointing device
    kBluetoothDeviceClassMinorPeripheral1Combo				= 0x30,		// [11XXXX] Combo keyboard/pointing device
    
    // Peripheral2 subclass is bits 5-2
    
    kBluetoothDeviceClassMinorPeripheral2Unclassified		= 0x00,		// [XX0000] Uncategorized device
    kBluetoothDeviceClassMinorPeripheral2Joystick			= 0x01,		// [XX0001] Joystick
    kBluetoothDeviceClassMinorPeripheral2Gamepad			= 0x02,		// [XX0010] Gamepad
    kBluetoothDeviceClassMinorPeripheral2RemoteControl		= 0x03,		// [XX0011] Remote control
    kBluetoothDeviceClassMinorPeripheral2SensingDevice		= 0x04,		// [XX0100] Sensing device
    kBluetoothDeviceClassMinorPeripheral2DigitizerTablet	= 0x05,		// [XX0101] Digitizer Tablet	
    kBluetoothDeviceClassMinorPeripheral2CardReader			= 0x06,		// [XX0110] Card Reader
    kBluetoothDeviceClassMinorPeripheral2DigitalPen         = 0x07,     // [XX0111] Digital Pen
    kBluetoothDeviceClassMinorPeripheral2HandheldScanner    = 0x08,     // [XX1000] Handheld scanner for bar-codes, RFID, etc.
    kBluetoothDeviceClassMinorPeripheral2GesturalInputDevice = 0x09,    // [XX1001] Handheld gestural input device (e.g., "wand" form factor)
    
	kBluetoothDeviceClassMinorPeripheral2AnyPointing		= 'poin',	// Anything under MinorPeripheral1Pointing
	
    // Range 0x05 to 0x0f reserved for future use
	
    
	///
	/// Imaging Minor Class
	///
    
    // Imaging1 subclass is bits 7 - 4
    
    kBluetoothDeviceClassMinorImaging1Display				= 0x04,		// [XXX1XX] Display
    kBluetoothDeviceClassMinorImaging1Camera				= 0x08,		// [XX1XXX] Camera
    kBluetoothDeviceClassMinorImaging1Scanner				= 0x10,		// [X1XXXX] Scanner
    kBluetoothDeviceClassMinorImaging1Printer				= 0x20,		// [1XXXXX] Printer
    
    // Imaging2 subclass is bits 3 - 2
    
    kBluetoothDeviceClassMinorImaging2Unclassified			= 0x00,		// [XXXX00] Uncategorized, default
    
    // Range 0x01 - 0x03 reserved for future use
    
	///
	/// Wearable Minor Class
	///
        
    kBluetoothDeviceClassMinorWearableWristWatch			= 0x01,		// [000001] Watch
    kBluetoothDeviceClassMinorWearablePager					= 0x02,		// [000010] Pager
    kBluetoothDeviceClassMinorWearableJacket				= 0x03,		// [000011] Jacket
    kBluetoothDeviceClassMinorWearableHelmet				= 0x04,		// [000100] Helmet
    kBluetoothDeviceClassMinorWearableGlasses				= 0x05,		// [000101] Glasses
    
	///
	/// Toy Minor Class
	///
        
    kBluetoothDeviceClassMinorToyRobot						= 0x01,		// [000001] Robot
    kBluetoothDeviceClassMinorToyVehicle					= 0x02,		// [000010] Vehicle
    kBluetoothDeviceClassMinorToyDollActionFigure			= 0x03,		// [000011] Doll / Action Figure
    kBluetoothDeviceClassMinorToyController					= 0x04,		// [000100] Controller
    kBluetoothDeviceClassMinorToyGame						= 0x05,		// [000101] Game
    
	///
	/// Health Minor Class
	///
        
    kBluetoothDeviceClassMinorHealthUndefined				= 0x00,		// [000000] Undefined
    kBluetoothDeviceClassMinorHealthBloodPressureMonitor	= 0x01,		// [000001] Blood Pressure Monitor
    kBluetoothDeviceClassMinorHealthThermometer				= 0x02,		// [000010] Thermometer
    kBluetoothDeviceClassMinorHealthScale					= 0x03,		// [000011] Scale
    kBluetoothDeviceClassMinorHealthGlucoseMeter			= 0x04,		// [000100] Glucose Meter
    kBluetoothDeviceClassMinorHealthPulseOximeter			= 0x05,		// [000101] Pulse Oximeter
    kBluetoothDeviceClassMinorHealthHeartRateMonitor		= 0x06,		// [000111] Heart Rate Monitor
    kBluetoothDeviceClassMinorHealthDataDisplay				= 0x07,		// [001000] Display
    
	///
	///	Misc
	///
	
	kBluetoothDeviceClassMinorAny							= '****',	// Pseudo-class - means anything acceptable.
	kBluetoothDeviceClassMinorNone							= 'none',	// Pseudo-class - means no matching.
	
	kBluetoothDeviceClassMinorEnd
};
		
		
// Peripheral devices bits 7 & 6
#define BluetoothCoDMinorPeripheral1(minorClass) (minorClass & 0x30)
		
// Peripheral devices bits 5 - 2
#define BluetoothCoDMinorPeripheral2(minorClass) (minorClass & 0x0f)	
		
#pragma mark - LE Appearance
enum {
    kBluetoothGAPAppearanceUnknown                                  = 0,
    kBluetoothGAPAppearanceGenericPhone                             = 64,
    kBluetoothGAPAppearanceGenericComputer                          = 128,
    kBluetoothGAPAppearanceGenericWatch                             = 192,
    kBluetoothGAPAppearanceGenericClock                             = 256,
    kBluetoothGAPAppearanceGenericDisplay                           = 320,
    kBluetoothGAPAppearanceGenericRemoteControl                     = 384,
    kBluetoothGAPAppearanceGenericEyeGlasses                        = 448,
    kBluetoothGAPAppearanceGenericTag                               = 512,
    kBluetoothGAPAppearanceGenericKeyring                           = 576,
    kBluetoothGAPAppearanceGenericMediaPlayer                       = 640,
    kBluetoothGAPAppearanceGenericBarcodeScanner                    = 704,
    kBluetoothGAPAppearanceGenericThermometer                       = 768,
    kBluetoothGAPAppearanceGenericHeartrateSensor                   = 832,
    kBluetoothGAPAppearanceGenericBloodPressure                     = 896,
    kBluetoothGAPAppearanceGenericHumanInterfaceDevice              = 960,
    kBluetoothGAPAppearanceHumanInterfaceDeviceKeyboard             = 961,
    kBluetoothGAPAppearanceHumanInterfaceDeviceMouse                = 962,
    kBluetoothGAPAppearanceHumanInterfaceDeviceJoystick             = 963,
    kBluetoothGAPAppearanceHumanInterfaceDeviceGamepad              = 964,
    kBluetoothGAPAppearanceHumanInterfaceDeviceDigitizerTablet      = 965,
    kBluetoothGAPAppearanceHumanInterfaceDeviceCardReader           = 966,
    kBluetoothGAPAppearanceHumanInterfaceDeviceDigitalPen           = 967,
    kBluetoothGAPAppearanceHumanInterfaceDeviceBarcodeScanner       = 968,
    kBluetoothGAPAppearanceGenericGlucoseMeter                      = 1024,
    kBluetoothGAPAppearanceGenericRunningWalkingSensor              = 1088,
    kBluetoothGAPAppearanceGenericCycling                           = 1152,
};
        
#pragma mark -
#pragma mark === L2CAP ===

//===========================================================================================================================
//	L2CAP
//===========================================================================================================================

enum
{
    kBluetoothL2CAPPSMSDP						= 0x0001, 
    kBluetoothL2CAPPSMRFCOMM					= 0x0003, 
    kBluetoothL2CAPPSMTCS_BIN					= 0x0005,	// Telephony Control Specifictation / TCS Binary
    kBluetoothL2CAPPSMTCS_BIN_Cordless			= 0x0007,	// Telephony Control Specifictation / TCS Binary
    kBluetoothL2CAPPSMBNEP						= 0x000F,	// Bluetooth Network Encapsulation Protocol
    kBluetoothL2CAPPSMHIDControl				= 0x0011,	// HID profile - control interface
    kBluetoothL2CAPPSMHIDInterrupt				= 0x0013,	// HID profile - interrupt interface
    kBluetoothL2CAPPSMAVCTP						= 0x0017,	// Audio/Video Control Transport Protocol
    kBluetoothL2CAPPSMAVDTP						= 0x0019,	// Audio/Video Distribution Transport Protocol
    kBluetoothL2CAPPSMAVCTP_Browsing			= 0x001B,	// AVTCP (AVRCP) Browsing
    kBluetoothL2CAPPSMUID_C_Plane				= 0x001D,	// Unrestricted Digital Information Profile (UDI)
    kBluetoothL2CAPPSMATT						= 0x001F,	// Attribute Protocol
	
	// Range < 0x1000 reserved.
    kBluetoothL2CAPPSMReservedStart				= 0x0001,
    kBluetoothL2CAPPSMReservedEnd				= 0x1000,
    
	// Range 0x1001-0xFFFF dynamically assigned.
    kBluetoothL2CAPPSMDynamicStart				= 0x1001,
	// AACP
	kBluetoothL2CAPPSMAACP						= 0x1001,
	kBluetoothL2CAPPSMD2D						= 0x100F,
    kBluetoothL2CAPPSMDynamicEnd				= 0xFFFF,
    
    kBluetoothL2CAPPSMNone						= 0x0000
};


#pragma mark -
#pragma mark === SDP ===

//===========================================================================================================================
//	Service Discovery Protocol
//===========================================================================================================================

enum
{
	// General
	
	kBluetoothSDPUUID16Base									= 0x0000, 	// 00000000-0000-1000-8000-00805f9b34fb
	
	// Protocols
	
	kBluetoothSDPUUID16SDP				 					= 0x0001, 	// 00000001-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16UDP 									= 0x0002, 	// 00000002-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16RFCOMM 								= 0x0003, 	// 00000003-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16TCP 									= 0x0004, 	// 00000004-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16TCSBIN 								= 0x0005, 	// 00000005-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16TCSAT 								= 0x0006, 	// 00000006-0000-1000-8000-00805f9b34fb
    kBluetoothSDPUUID16ATT                                  = 0x0007,
	kBluetoothSDPUUID16OBEX 								= 0x0008, 	// 00000008-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16IP 									= 0x0009, 	// 00000009-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16FTP 									= 0x000A, 	// 0000000A-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16HTTP 								= 0x000C, 	// 0000000C-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16WSP	 								= 0x000E, 	// 0000000E-0000-1000-8000-00805f9b34fb 
	kBluetoothSDPUUID16BNEP									= 0x000F,
	kBluetoothSDPUUID16UPNP									= 0x0010,
	kBluetoothSDPUUID16HIDP									= 0x0011,
	kBluetoothSDPUUID16HardcopyControlChannel				= 0x0012,
	kBluetoothSDPUUID16HardcopyDataChannel					= 0x0014,
	kBluetoothSDPUUID16HardcopyNotification					= 0x0016,
	kBluetoothSDPUUID16AVCTP								= 0x0017,
	kBluetoothSDPUUID16AVDTP								= 0x0019,
	kBluetoothSDPUUID16CMPT									= 0x001B,
	kBluetoothSDPUUID16UDI_C_Plane							= 0x001D,
	kBluetoothSDPUUID16MCAPControlChannel					= 0x001E,
	kBluetoothSDPUUID16MCAPDataChannel						= 0x001F,
	kBluetoothSDPUUID16L2CAP 								= 0x0100, 	// 00000100-0000-1000-8000-00805f9b34fb
};

enum SDPServiceClasses
{
	kBluetoothSDPUUID16ServiceClassServiceDiscoveryServer					= 0x1000,	// 00001000-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassBrowseGroupDescriptor					= 0x1001,	// 00001001-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassPublicBrowseGroup						= 0x1002,	// 00001002-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassSerialPort								= 0x1101,	// 00001101-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassLANAccessUsingPPP						= 0x1102,	// 00001102-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassDialupNetworking							= 0x1103,	// 00001103-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassIrMCSync									= 0x1104,	// 00001104-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassOBEXObjectPush							= 0x1105,	// 00001105-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassOBEXFileTransfer							= 0x1106,	// 00001106-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassIrMCSyncCommand							= 0x1107,	// 00001107-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassHeadset									= 0x1108,	// 00001108-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassCordlessTelephony						= 0x1109,	// 00001109-0000-1000-8000-00805f9b34fb
    kBluetoothSDPUUID16ServiceClassAudioSource								= 0x110A,
    kBluetoothSDPUUID16ServiceClassAudioSink								= 0x110B,
    kBluetoothSDPUUID16ServiceClassAVRemoteControlTarget					= 0x110C,
    kBluetoothSDPUUID16ServiceClassAdvancedAudioDistribution				= 0x110D,
    kBluetoothSDPUUID16ServiceClassAVRemoteControl							= 0x110E,
    kBluetoothSDPUUID16ServiceClassAVRemoteControlController				= 0x110F,
	kBluetoothSDPUUID16ServiceClassIntercom									= 0x1110,	// 00001110-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassFax										= 0x1111,	// 00001111-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassHeadsetAudioGateway						= 0x1112,	// 00001112-0000-1000-8000-00805f9b34fb
    kBluetoothSDPUUID16ServiceClassWAP										= 0x1113,
    kBluetoothSDPUUID16ServiceClassWAPClient								= 0x1114,
    kBluetoothSDPUUID16ServiceClassPANU										= 0x1115,
    kBluetoothSDPUUID16ServiceClassNAP										= 0x1116,
    kBluetoothSDPUUID16ServiceClassGN										= 0x1117,
    kBluetoothSDPUUID16ServiceClassDirectPrinting							= 0x1118,
    kBluetoothSDPUUID16ServiceClassReferencePrinting						= 0x1119,
    kBluetoothSDPUUID16ServiceClassImaging									= 0x111A,
    kBluetoothSDPUUID16ServiceClassImagingResponder							= 0x111B,
    kBluetoothSDPUUID16ServiceClassImagingAutomaticArchive					= 0x111C,
    kBluetoothSDPUUID16ServiceClassImagingReferencedObjects					= 0x111D,
    kBluetoothSDPUUID16ServiceClassHandsFree								= 0x111E,
    kBluetoothSDPUUID16ServiceClassHandsFreeAudioGateway					= 0x111F,
    kBluetoothSDPUUID16ServiceClassDirectPrintingReferenceObjectsService	= 0x1120,
    kBluetoothSDPUUID16ServiceClassReflectedUI								= 0x1121,
    kBluetoothSDPUUID16ServiceClassBasicPrinting							= 0x1122,
    kBluetoothSDPUUID16ServiceClassPrintingStatus							= 0x1123,
    kBluetoothSDPUUID16ServiceClassHumanInterfaceDeviceService				= 0x1124,
    kBluetoothSDPUUID16ServiceClassHardcopyCableReplacement					= 0x1125,
    kBluetoothSDPUUID16ServiceClassHCR_Print								= 0x1126,
    kBluetoothSDPUUID16ServiceClassHCR_Scan									= 0x1127,
    kBluetoothSDPUUID16ServiceClassCommonISDNAccess							= 0x1128,
    kBluetoothSDPUUID16ServiceClassVideoConferencingGW						= 0x1129,
    kBluetoothSDPUUID16ServiceClassUDI_MT									= 0x112A,
    kBluetoothSDPUUID16ServiceClassUDI_TA									= 0x112B,
    kBluetoothSDPUUID16ServiceClassAudioVideo								= 0x112C,
    kBluetoothSDPUUID16ServiceClassSIM_Access								= 0x112D,
    kBluetoothSDPUUID16ServiceClassPhonebookAccess_PCE						= 0x112E,
    kBluetoothSDPUUID16ServiceClassPhonebookAccess_PSE						= 0x112F,
    kBluetoothSDPUUID16ServiceClassPhonebookAccess							= 0x1130,
    kBluetoothSDPUUID16ServiceClassHeadset_HS								= 0x1131,
    kBluetoothSDPUUID16ServiceClassMessageAccessServer						= 0x1132,
    kBluetoothSDPUUID16ServiceClassMessageNotificationServer				= 0x1133,
    kBluetoothSDPUUID16ServiceClassMessageAccessProfile						= 0x1134,
    kBluetoothSDPUUID16ServiceClassGlobalNavigationSatelliteSystem			= 0x1135,
    kBluetoothSDPUUID16ServiceClassGlobalNavigationSatelliteSystemServer	= 0x1136,
	kBluetoothSDPUUID16ServiceClassPnPInformation							= 0x1200,	// 00001200-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassGenericNetworking						= 0x1201,	// 00001201-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassGenericFileTransfer						= 0x1202,	// 00001202-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassGenericAudio								= 0x1203,	// 00001203-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassGenericTelephony							= 0x1204,	// 00001204-0000-1000-8000-00805f9b34fb
	kBluetoothSDPUUID16ServiceClassVideoSource								= 0x1303,
	kBluetoothSDPUUID16ServiceClassVideoSink								= 0x1304,
	kBluetoothSDPUUID16ServiceClassVideoDistribution						= 0x1305,
	kBluetoothSDPUUID16ServiceClassHealthDevice								= 0x1400,
	kBluetoothSDPUUID16ServiceClassHealthDeviceSource						= 0x1401,
	kBluetoothSDPUUID16ServiceClassHealthDeviceSink							= 0x1402,
    kBluetoothSDPUUID16ServiceClassGATT                                     = 0x1801,
};

enum SDPAttributeIdentifierCodes
{
	kBluetoothSDPAttributeIdentifierServiceRecordHandle					= 0x0000,
	kBluetoothSDPAttributeIdentifierServiceClassIDList					= 0x0001,
	kBluetoothSDPAttributeIdentifierServiceRecordState					= 0x0002,
	kBluetoothSDPAttributeIdentifierServiceID							= 0x0003,
	kBluetoothSDPAttributeIdentifierProtocolDescriptorList				= 0x0004,
	kBluetoothSDPAttributeIdentifierBrowseGroupList						= 0x0005,
	kBluetoothSDPAttributeIdentifierLanguageBaseAttributeIDList			= 0x0006,
	kBluetoothSDPAttributeIdentifierServiceInfoTimeToLive				= 0x0007,
	kBluetoothSDPAttributeIdentifierServiceAvailability					= 0x0008,
	kBluetoothSDPAttributeIdentifierBluetoothProfileDescriptorList		= 0x0009,
	kBluetoothSDPAttributeIdentifierDocumentationURL					= 0x000A,
	kBluetoothSDPAttributeIdentifierClientExecutableURL					= 0x000B,
	kBluetoothSDPAttributeIdentifierIconURL								= 0x000C,
	kBluetoothSDPAttributeIdentifierAdditionalProtocolsDescriptorList	= 0x000D,

    // Service Discovery Server
	kBluetoothSDPAttributeIdentifierVersionNumberList					= 0x0200,
	kBluetoothSDPAttributeIdentifierServiceDatabaseState				= 0x0201,
    
    // Browse Group Descriptor
    kBluetoothSDPAttributeIdentifierGroupID								= 0x0200,
    
    // PAN
    kBluetoothSDPAttributeIdentifierIPSubnet							= 0x0200,

    // HID																			Required		Type		Notes
    kBluetoothSDPAttributeIdentifierHIDReleaseNumber					= 0x0200,	// O			uint16
    kBluetoothSDPAttributeIdentifierHIDParserVersion					= 0x0201,	// M			uint16
    kBluetoothSDPAttributeIdentifierHIDDeviceSubclass					= 0x0202,	// M			uint8       Should match the low order 8 bits of CoD unless a combo device
    kBluetoothSDPAttributeIdentifierHIDCountryCode						= 0x0203,	// M			uint8
    kBluetoothSDPAttributeIdentifierHIDVirtualCable						= 0x0204,	// M			bool
    kBluetoothSDPAttributeIdentifierHIDReconnectInitiate				= 0x0205,	// M			bool
    kBluetoothSDPAttributeIdentifierHIDDescriptorList					= 0x0206,	// M			sequence    Layout is defined in Section 6.2 of the HID Specification
    kBluetoothSDPAttributeIdentifierHIDLangIDBaseList					= 0x0207,	// M			sequence
    kBluetoothSDPAttributeIdentifierHIDSDPDisable						= 0x0208,	// O			bool
    kBluetoothSDPAttributeIdentifierHIDBatteryPower						= 0x0209,	// O			bool
    kBluetoothSDPAttributeIdentifierHIDRemoteWake						= 0x020A,	// O			bool
    kBluetoothSDPAttributeIdentifierHIDProfileVersion					= 0x020B,	// M			uint16
    kBluetoothSDPAttributeIdentifierHIDSupervisionTimeout				= 0x020C,	// O			uint16		Default to 5 seconds or longer if not defined
    kBluetoothSDPAttributeIdentifierHIDNormallyConnectable				= 0x020D,	// O			bool
    kBluetoothSDPAttributeIdentifierHIDBootDevice						= 0x020E,	// M			bool		Only defined for mice and keyboards as of HID 1.0
    kBluetoothSDPAttributeIdentifierHIDSSRHostMaxLatency				= 0x020F,	// O			uint16
    kBluetoothSDPAttributeIdentifierHIDSSRHostMinTimeout				= 0x0210,	// O			uint16
    
	kBluetoothSDPAttributeIdentifierServiceVersion						= 0x0300,
	kBluetoothSDPAttributeIdentifierExternalNetwork						= 0x0301,	// Cordless telephony
    kBluetoothSDPAttributeIdentifierNetwork								= 0x0301,	// Handsfree Profile (HFP)
	kBluetoothSDPAttributeIdentifierSupportedDataStoresList				= 0x0301,	// Sync Profile
	kBluetoothSDPAttributeIdentifierFaxClass1Support					= 0x0302,	// Fax Profile
	kBluetoothSDPAttributeIdentifierRemoteAudioVolumeControl			= 0x0302,	// GAP???
	kBluetoothSDPAttributeIdentifierFaxClass2_0Support					= 0x0303,
	kBluetoothSDPAttributeIdentifierSupporterFormatsList				= 0x0303,
	kBluetoothSDPAttributeIdentifierFaxClass2Support					= 0x0304,
	kBluetoothSDPAttributeIdentifierAudioFeedbackSupport				= 0x0305,
    kBluetoothSDPAttributeIdentifierNetworkAddress						= 0x0306,	// WAP
    kBluetoothSDPAttributeIdentifierWAPGateway							= 0x0307,	// WAP
    kBluetoothSDPAttributeIdentifierHomepageURL							= 0x0308,	// WAP
    kBluetoothSDPAttributeIdentifierWAPStackType						= 0x0309,	// WAP
    kBluetoothSDPAttributeIdentifierSecurityDescription					= 0x030A,	// PAN
    kBluetoothSDPAttributeIdentifierNetAccessType						= 0x030B,	// PAN
    kBluetoothSDPAttributeIdentifierMaxNetAccessRate					= 0x030C,	// PAN
    kBluetoothSDPAttributeIdentifierSupportedCapabilities				= 0x0310,	// Imaging
    kBluetoothSDPAttributeIdentifierSupportedFeatures					= 0x0311,	// Imaging & HFP
    kBluetoothSDPAttributeIdentifierSupportedFunctions					= 0x0312,	// Imaging
    kBluetoothSDPAttributeIdentifierTotalImagingDataCapacity			= 0x0313,	// Imaging

	kBluetoothSDPAttributeIdentifierServiceName							= 0x0000, /* +language base offset*/
	kBluetoothSDPAttributeIdentifierServiceDescription					= 0x0001, /* +language base offset*/
	kBluetoothSDPAttributeIdentifierProviderName						= 0x0002  /* +language base offset*/
};

enum SDPAttributeDeviceIdentificationRecord
{
	// DeviceID v1.3																Required		Type		Section		Notes
	kBluetoothSDPAttributeDeviceIdentifierServiceDescription			= 0x0001,	// O			string		-
	kBluetoothSDPAttributeDeviceIdentifierDocumentationURL				= 0x000A,	// O			url			-
	kBluetoothSDPAttributeDeviceIdentifierClientExecutableURL			= 0x000B,	// O			url			-
	kBluetoothSDPAttributeDeviceIdentifierSpecificationID				= 0x0200,	// M			uint16		5.1
	kBluetoothSDPAttributeDeviceIdentifierVendorID						= 0x0201,	// M			uint16		5.2
	kBluetoothSDPAttributeDeviceIdentifierProductID						= 0x0202,	// M			uint16		5.3
	kBluetoothSDPAttributeDeviceIdentifierVersion						= 0x0203,	// M			uint16		5.4
	kBluetoothSDPAttributeDeviceIdentifierPrimaryRecord					= 0x0204,	// M			bool		5.5
	kBluetoothSDPAttributeDeviceIdentifierVendorIDSource				= 0x0205,	// M			uint16		5.6
	kBluetoothSDPAttributeDeviceIdentifierReservedRangeStart			= 0x0206,
	kBluetoothSDPAttributeDeviceIdentifierReservedRangeEnd				= 0x02FF
	
};

enum ProtocolParameters
{
    kBluetoothSDPProtocolParameterL2CAPPSM								= 1,
    kBluetoothSDPProtocolParameterRFCOMMChannel							= 1,
    kBluetoothSDPProtocolParameterTCPPort								= 1,
    kBluetoothSDPProtocolParameterUDPPort								= 1,
    kBluetoothSDPProtocolParameterBNEPVersion							= 1,
    kBluetoothSDPProtocolParameterBNEPSupportedNetworkPacketTypeList	= 2
};

//Inquiries with 'Extended Inquiry Response' (v2.1 specification)

enum BluetoothHCIExtendedInquiryResponseDataTypes
{
	kBluetoothHCIExtendedInquiryResponseDataTypeFlags										=	0x01,
	kBluetoothHCIExtendedInquiryResponseDataType16BitServiceClassUUIDsWithMoreAvailable		=	0x02,
	kBluetoothHCIExtendedInquiryResponseDataType16BitServiceClassUUIDsCompleteList			=	0x03,
	kBluetoothHCIExtendedInquiryResponseDataType32BitServiceClassUUIDsWithMoreAvailable		=	0x04,
	kBluetoothHCIExtendedInquiryResponseDataType32BitServiceClassUUIDsCompleteList			=	0x05,
	kBluetoothHCIExtendedInquiryResponseDataType128BitServiceClassUUIDsWithMoreAvailable	=	0x06,
	kBluetoothHCIExtendedInquiryResponseDataType128BitServiceClassUUIDsCompleteList			=	0x07,
	kBluetoothHCIExtendedInquiryResponseDataTypeShortenedLocalName							=	0x08,
	kBluetoothHCIExtendedInquiryResponseDataTypeCompleteLocalName							=	0x09,
    kBluetoothHCIExtendedInquiryResponseDataTypeTransmitPowerLevel                          =   0x0A,
    kBluetoothHCIExtendedInquiryResponseDataTypeSSPOOBClassOfDevice                         =   0x0D,
    kBluetoothHCIExtendedInquiryResponseDataTypeSSPOOBSimplePairingHashC                    =   0x0E,
    kBluetoothHCIExtendedInquiryResponseDataTypeSSPOOBSimplePairingRandomizerR              =   0x0F,
	kBluetoothHCIExtendedInquiryResponseDataTypeDeviceID									=	0x10,
	kBluetoothHCIExtendedInquiryResponseDataTypeSecurityManagerTKValue						=	0x10,
    kBluetoothHCIExtendedInquiryResponseDataTypeSecurityManagerOOBFlags                     =   0x11,
	kBluetoothHCIExtendedInquiryResponseDataTypePeripheralConnectionIntervalRange           =   0x12,
    kBluetoothHCIExtendedInquiryResponseDataTypeServiceSolicitation16BitUUIDs               =   0x14,
    kBluetoothHCIExtendedInquiryResponseDataTypeServiceSolicitation128BitUUIDs              =   0x15,
    kBluetoothHCIExtendedInquiryResponseDataTypeServiceData                                 =   0x16,
    kBluetoothHCIExtendedInquiryResponseDataTypePublicTargetAddress                         =   0x17,
    kBluetoothHCIExtendedInquiryResponseDataTypeRandomTargetAddress                         =   0x18,
    kBluetoothHCIExtendedInquiryResponseDataTypeAppearance                                  =   0x19,
    kBluetoothHCIExtendedInquiryResponseDataTypeAdvertisingInterval							=	0x1A,
    kBluetoothHCIExtendedInquiryResponseDataTypeLEBluetoothDeviceAddress					=	0x1B,
    kBluetoothHCIExtendedInquiryResponseDataTypeLERole										=	0x1C,
    kBluetoothHCIExtendedInquiryResponseDataTypeSimplePairingHash							=	0x1D,
    kBluetoothHCIExtendedInquiryResponseDataTypeSimplePairingRandomizer						=	0x1E,
    kBluetoothHCIExtendedInquiryResponseDataTypeServiceSolicitation32BitUUIDs				=	0x1F,
    kBluetoothHCIExtendedInquiryResponseDataTypeServiceData32BitUUID						=	0x20,
    kBluetoothHCIExtendedInquiryResponseDataTypeServiceData128BitUUID						=	0x21,
    kBluetoothHCIExtendedInquiryResponseDataTypeSecureConnectionsConfirmationValue			=	0x22,
    kBluetoothHCIExtendedInquiryResponseDataTypeSecureConnectionsRandomValue				=	0x23,
    kBluetoothHCIExtendedInquiryResponseDataTypeURI											=	0x24,
    kBluetoothHCIExtendedInquiryResponseDataTypeIndoorPositioning							=	0x25,
    kBluetoothHCIExtendedInquiryResponseDataTypeTransportDiscoveryData						=	0x26,
    kBluetoothHCIExtendedInquiryResponseDataType3DInformationData							=	0x3D,
	kBluetoothHCIExtendedInquiryResponseDataTypeManufacturerSpecificData					=	0xFF,
	kBluetoothHCIExtendedInquiryResponseDataTypeSlaveConnectionIntervalRange __attribute__ ((deprecated)) = kBluetoothHCIExtendedInquiryResponseDataTypePeripheralConnectionIntervalRange,
};


// HCI Versions
// Living Document URL: https://www.bluetooth.com/specifications/assigned-numbers/host-controller-interface

typedef enum BluetoothHCIVersions
{
	kBluetoothHCIVersionCoreSpecification1_0b												=	0x00,
	kBluetoothHCIVersionCoreSpecification1_1												=	0x01,
	kBluetoothHCIVersionCoreSpecification1_2												=	0x02,
	kBluetoothHCIVersionCoreSpecification2_0EDR												=	0x03,
	kBluetoothHCIVersionCoreSpecification2_1EDR												=	0x04,
	kBluetoothHCIVersionCoreSpecification3_0HS												=	0x05,
	kBluetoothHCIVersionCoreSpecification4_0												=	0x06,
    kBluetoothHCIVersionCoreSpecification4_1												=	0x07,
	kBluetoothHCIVersionCoreSpecification4_2												=	0x08,
	kBluetoothHCIVersionCoreSpecification5_0												=	0x09,
	kBluetoothHCIVersionCoreSpecification5_1												=   0x0a,
	kBluetoothHCIVersionCoreSpecification5_2												=   0x0b
} BluetoothHCIVersions;


// LMP Versions
// Living Document URL: https://www.bluetooth.com/specifications/assigned-numbers/link-manager

typedef enum BluetoothLMPVersions
{
	kBluetoothLMPVersionCoreSpecification1_0b												=	0x00,
	kBluetoothLMPVersionCoreSpecification1_1												=	0x01,
	kBluetoothLMPVersionCoreSpecification1_2												=	0x02,
	kBluetoothLMPVersionCoreSpecification2_0EDR												=	0x03,
	kBluetoothLMPVersionCoreSpecification2_1EDR												=	0x04,
	kBluetoothLMPVersionCoreSpecification3_0HS												=	0x05,
	kBluetoothLMPVersionCoreSpecification4_0												=	0x06,
	kBluetoothLMPVersionCoreSpecification4_1												=	0x07,
	kBluetoothLMPVersionCoreSpecification4_2												=	0x08,
	kBluetoothLMPVersionCoreSpecification5_0												=	0x09,
	kBluetoothLMPVersionCoreSpecification5_1												=   0x0a,
	kBluetoothLMPVersionCoreSpecification5_2												=   0x0b
} BluetoothLMPVersions;

#ifdef	__cplusplus
	}
#endif
