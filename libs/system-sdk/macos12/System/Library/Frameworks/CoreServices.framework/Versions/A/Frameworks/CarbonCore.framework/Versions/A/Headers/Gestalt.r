/*
     File:       Gestalt.r
 
     Contains:   Gestalt Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1988-2011 by Apple Inc. All rights reserved.
*/

#ifndef __GESTALT_R__
#define __GESTALT_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define gestaltAddressingModeAttr 		'addr'				/*  addressing mode attributes  */
#define gestalt32BitAddressing 			0					/*  using 32-bit addressing mode  */
#define gestalt32BitSysZone 			1					/*  32-bit compatible system zone  */
#define gestalt32BitCapable 			2					/*  Machine is 32-bit capable  */

#define gestaltAFPClient 				'afps'
#define gestaltAFPClientVersionMask 	0x0000FFFF			/*  low word of SInt32 is the  */
															/*  client version 0x0001 -> 0x0007 */
#define gestaltAFPClient3_5 			0x0001
#define gestaltAFPClient3_6 			0x0002
#define gestaltAFPClient3_6_1 			0x0003
#define gestaltAFPClient3_6_2 			0x0004
#define gestaltAFPClient3_6_3 			0x0005				/*  including 3.6.4, 3.6.5 */
#define gestaltAFPClient3_7 			0x0006				/*  including 3.7.1 */
#define gestaltAFPClient3_7_2 			0x0007				/*  including 3.7.3, 3.7.4 */
#define gestaltAFPClient3_8 			0x0008
#define gestaltAFPClient3_8_1 			0x0009				/*  including 3.8.2  */
#define gestaltAFPClient3_8_3 			0x000A
#define gestaltAFPClient3_8_4 			0x000B				/*  including 3.8.5, 3.8.6  */
#define gestaltAFPClientAttributeMask 	0xFFFF0000			/*  high word of response is a  */
															/*  set of attribute bits */
#define gestaltAFPClientCfgRsrc 		16					/*  Client uses config resources */
#define gestaltAFPClientSupportsIP 		29					/*  Client supports AFP over TCP/IP */
#define gestaltAFPClientVMUI 			30					/*  Client can put up UI from the PBVolMount trap */
#define gestaltAFPClientMultiReq 		31					/*  Client supports multiple outstanding requests */

#define gestaltAliasMgrAttr 			'alis'				/*  Alias Mgr Attributes  */
#define gestaltAliasMgrPresent 			0					/*  True if the Alias Mgr is present  */
#define gestaltAliasMgrSupportsRemoteAppletalk  1			/*  True if the Alias Mgr knows about Remote Appletalk  */
#define gestaltAliasMgrSupportsAOCEKeychain  2				/*  True if the Alias Mgr knows about the AOCE Keychain  */
#define gestaltAliasMgrResolveAliasFileWithMountOptions  3	/*  True if the Alias Mgr implements gestaltAliasMgrResolveAliasFileWithMountOptions() and IsAliasFile()  */
#define gestaltAliasMgrFollowsAliasesWhenResolving  4
#define gestaltAliasMgrSupportsExtendedCalls  5
#define gestaltAliasMgrSupportsFSCalls 	6					/*  true if Alias Mgr supports HFS+ Calls  */
#define gestaltAliasMgrPrefersPath 		7					/*  True if the Alias Mgr prioritizes the path over file id during resolution by default  */
#define gestaltAliasMgrRequiresAccessors  8					/*  Set if Alias Manager requires accessors for size and usertype  */

#define gestaltAppearanceAttr 			'appr'
#define gestaltAppearanceExists 		0
#define gestaltAppearanceCompatMode 	1

#define gestaltAppearanceVersion 		'apvr'
#define gestaltArbitorAttr 				'arb '
#define gestaltSerialArbitrationExists 	0					/*  this bit if the serial port arbitrator exists */

#define gestaltAppleScriptVersion 		'ascv'				/*  AppleScript version */
#define gestaltAppleScriptAttr 			'ascr'				/*  AppleScript attributes */
#define gestaltAppleScriptPresent 		0
#define gestaltAppleScriptPowerPCSupport  1

#define gestaltATAAttr 					'ata '				/*  ATA is the driver to support IDE hard disks  */
#define gestaltATAPresent 				0					/*  if set, ATA Manager is present  */

#define gestaltATalkVersion 			'atkv'				/*  Detailed AppleTalk version; see comment above for format  */
#define gestaltAppleTalkVersion 		'atlk'				/*  appletalk version  */
#define gestaltAUXVersion 				'a/ux'				/*  a/ux version, if present  */
#define gestaltMacOSCompatibilityBoxAttr  'bbox'			/*  Classic presence and features  */
#define gestaltMacOSCompatibilityBoxPresent  0				/*  True if running under the Classic  */
#define gestaltMacOSCompatibilityBoxHasSerial  1			/*  True if Classic serial support is implemented.  */
#define gestaltMacOSCompatibilityBoxless  2					/*  True if we're Boxless (screen shared with Carbon/Cocoa)  */

#define gestaltBusClkSpeed 				'bclk'				/*  main I/O bus clock speed in hertz  */
#define gestaltBusClkSpeedMHz 			'bclm'				/*  main I/O bus clock speed in megahertz ( a UInt32 )  */
#define gestaltCloseViewAttr 			'BSDa'				/*  CloseView attributes  */
#define gestaltCloseViewEnabled 		0					/*  Closeview enabled (dynamic bit - returns current state)  */
#define gestaltCloseViewDisplayMgrFriendly  1				/*  Closeview compatible with Display Manager (FUTURE)  */

#define gestaltCarbonVersion 			'cbon'				/*  version of Carbon API present in system  */
#define gestaltCFMAttr 					'cfrg'				/*  Selector for information about the Code Fragment Manager  */
#define gestaltCFMPresent 				0					/*  True if the Code Fragment Manager is present  */
#define gestaltCFMPresentMask 			0x0001
#define gestaltCFM99Present 			2					/*  True if the CFM-99 features are present.  */
#define gestaltCFM99PresentMask 		0x0004

#define gestaltProcessorCacheLineSize 	'csiz'				/*  The size, in bytes, of the processor cache line.  */
#define gestaltCollectionMgrVersion 	'cltn'				/*  Collection Manager version  */
#define gestaltColorMatchingAttr 		'cmta'				/*  ColorSync attributes  */
#define gestaltHighLevelMatching 		0
#define gestaltColorMatchingLibLoaded 	1

#define gestaltColorMatchingVersion 	'cmtc'
#define gestaltColorSync10 				0x0100				/*  0x0100 & 0x0110 _Gestalt versions for 1.0-1.0.3 product  */
#define gestaltColorSync11 				0x0110				/*    0x0100 == low-level matching only  */
#define gestaltColorSync104 			0x0104				/*  Real version, by popular demand  */
#define gestaltColorSync105 			0x0105
#define gestaltColorSync20 				0x0200				/*  ColorSync 2.0  */
#define gestaltColorSync21 				0x0210
#define gestaltColorSync211 			0x0211
#define gestaltColorSync212 			0x0212
#define gestaltColorSync213 			0x0213
#define gestaltColorSync25 				0x0250
#define gestaltColorSync26 				0x0260
#define gestaltColorSync261 			0x0261
#define gestaltColorSync30 				0x0300

#define gestaltControlMgrVersion 		'cmvr'				/*  NOTE: The first version we return is 3.0.1, on Mac OS X plus update 2 */
#define gestaltControlMgrAttr 			'cntl'				/*  Control Mgr */
#define gestaltControlMgrPresent 		0x00000001			/*  NOTE: this is a bit mask, whereas all other Gestalt constants of */
															/*  this type are bit index values.   Universal Interfaces 3.2 slipped */
															/*  out the door with this mistake. */
#define gestaltControlMgrPresentBit 	0					/*  bit number */
#define gestaltControlMsgPresentMask 	0x00000001

#define gestaltConnMgrAttr 				'conn'				/*  connection mgr attributes     */
#define gestaltConnMgrPresent 			0
#define gestaltConnMgrCMSearchFix 		1					/*  Fix to CMAddSearch?      */
#define gestaltConnMgrErrorString 		2					/*  has CMGetErrorString()  */
#define gestaltConnMgrMultiAsyncIO 		3					/*  CMNewIOPB, CMDisposeIOPB, CMPBRead, CMPBWrite, CMPBIOKill  */

#define gestaltColorPickerVersion 		'cpkr'				/*  returns version of ColorPicker  */
#define gestaltColorPicker 				'cpkr'				/*  gestaltColorPicker is old name for gestaltColorPickerVersion  */

#define gestaltComponentMgr 			'cpnt'				/*  Component Mgr version  */
#define gestaltComponentPlatform 		'copl'				/*  Component Platform number  */

#define gestaltNativeCPUtype 			'cput'				/*  Native CPU type                           */
#define gestaltNativeCPUfamily 			'cpuf'				/*  Native CPU family                       */
#define gestaltCPU68000 				0					/*  Various 68k CPUs...     */
#define gestaltCPU68010 				1
#define gestaltCPU68020 				2
#define gestaltCPU68030 				3
#define gestaltCPU68040 				4
#define gestaltCPU601 					0x0101				/*  IBM 601                                */
#define gestaltCPU603 					0x0103
#define gestaltCPU604 					0x0104
#define gestaltCPU603e 					0x0106
#define gestaltCPU603ev 				0x0107
#define gestaltCPU750 					0x0108				/*  Also 740 - "G3"  */
#define gestaltCPU604e 					0x0109
#define gestaltCPU604ev 				0x010A				/*  Mach 5, 250Mhz and up  */
#define gestaltCPUG4 					0x010C				/*  Max  */
#define gestaltCPUG47450 				0x0110				/*  Vger , Altivec  */

#define gestaltCPUApollo 				0x0111				/*  Apollo , Altivec, G4 7455  */
#define gestaltCPUG47447 				0x0112
#define gestaltCPU750FX 				0x0120				/*  Sahara,G3 like thing  */
#define gestaltCPU970 					0x0139				/*  G5  */
#define gestaltCPU970FX 				0x013C				/*  another G5  */
#define gestaltCPU970MP 				0x0144

															/*  x86 CPUs all start with 'i' in the high nybble  */
#define gestaltCPU486 					'i486'
#define gestaltCPUPentium 				'i586'
#define gestaltCPUPentiumPro 			'i5pr'
#define gestaltCPUPentiumII 			'i5ii'
#define gestaltCPUX86 					'ixxx'
#define gestaltCPUPentium4 				'i5iv'

#define gestaltCRMAttr 					'crm '				/*  comm resource mgr attributes  */
#define gestaltCRMPresent 				0
#define gestaltCRMPersistentFix 		1					/*  fix for persistent tools  */
#define gestaltCRMToolRsrcCalls 		2					/*  has CRMGetToolResource/ReleaseToolResource  */

#define gestaltControlStripVersion 		'csvr'				/*  Control Strip version (was 'sdvr')  */
#define gestaltCountOfCPUs 				'cpus'				/*  the number of CPUs on the computer, Mac OS X 10.4 and later  */
#define gestaltCTBVersion 				'ctbv'				/*  CommToolbox version  */
#define gestaltDBAccessMgrAttr 			'dbac'				/*  Database Access Mgr attributes  */
#define gestaltDBAccessMgrPresent 		0					/*  True if Database Access Mgr present  */

#define gestaltDiskCacheSize 			'dcsz'				/*  Size of disk cache's buffers, in bytes  */
#define gestaltSDPFindVersion 			'dfnd'				/*  OCE Standard Directory Panel */
#define gestaltDictionaryMgrAttr 		'dict'				/*  Dictionary Manager attributes  */
#define gestaltDictionaryMgrPresent 	0					/*  Dictionary Manager attributes  */

#define gestaltDITLExtAttr 				'ditl'				/*  AppenDITL, etc. calls from CTB  */
#define gestaltDITLExtPresent 			0					/*  True if calls are present  */
#define gestaltDITLExtSupportsIctb 		1					/*  True if AppendDITL, ShortenDITL support 'ictb's  */

#define gestaltDialogMgrAttr 			'dlog'				/*  Dialog Mgr */
#define gestaltDialogMgrPresent 		0x00000001			/*  NOTE: this is a bit mask, whereas all other Gestalt constants of */
															/*  this type are bit index values.   Universal Interfaces 3.2 slipped */
															/*  out the door with this mistake. */
#define gestaltDialogMgrPresentBit 		0					/*  bit number */
#define gestaltDialogMgrHasAquaAlertBit  2					/*  bit number */
#define gestaltDialogMgrPresentMask 	0x00000001
#define gestaltDialogMgrHasAquaAlertMask  0x00000004
#define gestaltDialogMsgPresentMask 	0x00000001			/*  compatibility mask */

#define gestaltDesktopPicturesAttr 		'dkpx'				/*  Desktop Pictures attributes  */
#define gestaltDesktopPicturesInstalled  0					/*  True if control panel is installed  */
#define gestaltDesktopPicturesDisplayed  1					/*  True if a picture is currently displayed  */

#define gestaltDisplayMgrVers 			'dplv'				/*  Display Manager version  */
#define gestaltDisplayMgrAttr 			'dply'				/*  Display Manager attributes  */
#define gestaltDisplayMgrPresent 		0					/*  True if Display Mgr is present  */
#define gestaltDisplayMgrCanSwitchMirrored  2				/*  True if Display Mgr can switch modes on mirrored displays  */
#define gestaltDisplayMgrSetDepthNotifies  3				/*  True SetDepth generates displays mgr notification  */
#define gestaltDisplayMgrCanConfirm 	4					/*  True Display Manager supports DMConfirmConfiguration  */
#define gestaltDisplayMgrColorSyncAware  5					/*  True if Display Manager supports profiles for displays  */
#define gestaltDisplayMgrGeneratesProfiles  6				/*  True if Display Manager will automatically generate profiles for displays  */
#define gestaltDisplayMgrSleepNotifies 	7					/*  True if Display Mgr generates "displayWillSleep", "displayDidWake" notifications  */

#define gestaltDragMgrAttr 				'drag'				/*  Drag Manager attributes  */
#define gestaltDragMgrPresent 			0					/*  Drag Manager is present  */
#define gestaltDragMgrFloatingWind 		1					/*  Drag Manager supports floating windows  */
#define gestaltPPCDragLibPresent 		2					/*  Drag Manager PPC DragLib is present  */
#define gestaltDragMgrHasImageSupport 	3					/*  Drag Manager allows SetDragImage call  */
#define gestaltCanStartDragInFloatWindow  4					/*  Drag Manager supports starting a drag in a floating window  */
#define gestaltSetDragImageUpdates 		5					/*  Drag Manager supports drag image updating via SetDragImage  */

#define gestaltDrawSprocketVersion 		'dspv'				/*  Draw Sprocket version (as a NumVersion)  */
#define gestaltDigitalSignatureVersion 	'dsig'				/*  returns Digital Signature Toolbox version in low-order word */
#define gestaltDTPFeatures 				'dtpf'
#define kDTPThirdPartySupported 		0x00000004			/*  mask for checking if third-party drivers are supported */

#define gestaltDTPInfo 					'dtpx'				/*  returns GestaltDTPInfoHdle */
#define gestaltEasyAccessAttr 			'easy'				/*  Easy Access attributes  */
#define gestaltEasyAccessOff 			0					/*  if Easy Access present, but off (no icon)  */
#define gestaltEasyAccessOn 			1					/*  if Easy Access "On"  */
#define gestaltEasyAccessSticky 		2					/*  if Easy Access "Sticky"  */
#define gestaltEasyAccessLocked 		3					/*  if Easy Access "Locked"  */

#define gestaltEditionMgrAttr 			'edtn'				/*  Edition Mgr attributes  */
#define gestaltEditionMgrPresent 		0					/*  True if Edition Mgr present  */
#define gestaltEditionMgrTranslationAware  1				/*  True if edition manager is translation manager aware  */

#define gestaltAppleEventsAttr 			'evnt'				/*  Apple Events attributes  */
#define gestaltAppleEventsPresent 		0					/*  True if Apple Events present  */
#define gestaltScriptingSupport 		1
#define gestaltOSLInSystem 				2					/*  OSL is in system so donÕt use the one linked in to app  */
#define gestaltSupportsApplicationURL 	4					/*  Supports the typeApplicationURL addressing mode  */

#define gestaltExtensionTableVersion 	'etbl'				/*  ExtensionTable version  */
#define gestaltFBCIndexingState 		'fbci'				/*  Find By Content indexing state */
#define gestaltFBCindexingSafe 			0					/*  any search will result in synchronous wait */
#define gestaltFBCindexingCritical 		1					/*  any search will execute immediately */

#define gestaltFBCVersion 				'fbcv'				/*  Find By Content version */
#define gestaltFBCCurrentVersion 		0x0011				/*  First release for OS 8/9 */
#define gestaltOSXFBCCurrentVersion 	0x0100				/*  First release for OS X */

#define gestaltFileMappingAttr 			'flmp'				/*  File mapping attributes */
#define gestaltFileMappingPresent 		0					/*  bit is set if file mapping APIs are present */
#define gestaltFileMappingMultipleFilesFix  1				/*  bit is set if multiple files per volume can be mapped */

#define gestaltFloppyAttr 				'flpy'				/*  Floppy disk drive/driver attributes  */
#define gestaltFloppyIsMFMOnly 			0					/*  Floppy driver only supports MFM disk formats  */
#define gestaltFloppyIsManualEject 		1					/*  Floppy drive, driver, and file system are in manual-eject mode  */
#define gestaltFloppyUsesDiskInPlace 	2					/*  Floppy drive must have special DISK-IN-PLACE output; standard DISK-CHANGED not used  */

#define gestaltFinderAttr 				'fndr'				/*  Finder attributes  */
#define gestaltFinderDropEvent 			0					/*  Finder recognizes drop event  */
#define gestaltFinderMagicPlacement 	1					/*  Finder supports magic icon placement  */
#define gestaltFinderCallsAEProcess 	2					/*  Finder calls AEProcessAppleEvent  */
#define gestaltOSLCompliantFinder 		3					/*  Finder is scriptable and recordable  */
#define gestaltFinderSupports4GBVolumes  4					/*  Finder correctly handles 4GB volumes  */
#define gestaltFinderHasClippings 		6					/*  Finder supports Drag Manager clipping files  */
#define gestaltFinderFullDragManagerSupport  7				/*  Finder accepts 'hfs ' flavors properly  */
#define gestaltFinderFloppyRootComments  8					/*  in MacOS 8 and later, will be set if Finder ever supports comments on Floppy icons  */
#define gestaltFinderLargeAndNotSavedFlavorsOK  9			/*  in MacOS 8 and later, this bit is set if drags with >1024-byte flavors and flavorNotSaved flavors work reliably  */
#define gestaltFinderUsesExtensibleFolderManager  10		/*  Finder uses Extensible Folder Manager (for example, for Magic Routing)  */
#define gestaltFinderUnderstandsRedirectedDesktopFolder  11	/*  Finder deals with startup disk's desktop folder residing on another disk  */

#define gestaltFindFolderAttr 			'fold'				/*  Folder Mgr attributes  */
#define gestaltFindFolderPresent 		0					/*  True if Folder Mgr present  */
#define gestaltFolderDescSupport 		1					/*  True if Folder Mgr has FolderDesc calls  */
#define gestaltFolderMgrFollowsAliasesWhenResolving  2		/*  True if Folder Mgr follows folder aliases  */
#define gestaltFolderMgrSupportsExtendedCalls  3			/*  True if Folder Mgr supports the Extended calls  */
#define gestaltFolderMgrSupportsDomains  4					/*  True if Folder Mgr supports domains for the first parameter to FindFolder  */
#define gestaltFolderMgrSupportsFSCalls  5					/*  True if FOlder manager supports __FindFolderFSRef & __FindFolderExtendedFSRef  */

#define gestaltFindFolderRedirectionAttr  'fole'
#define gestaltFontMgrAttr 				'font'				/*  Font Mgr attributes  */
#define gestaltOutlineFonts 			0					/*  True if Outline Fonts supported  */

#define gestaltFPUType 					'fpu '				/*  fpu type  */
#define gestaltNoFPU 					0					/*  no FPU  */
#define gestalt68881 					1					/*  68881 FPU  */
#define gestalt68882 					2					/*  68882 FPU  */
#define gestalt68040FPU 				3					/*  68040 built-in FPU  */

#define gestaltFSAttr 					'fs  '				/*  file system attributes  */
#define gestaltFullExtFSDispatching 	0					/*  has really cool new HFSDispatch dispatcher  */
#define gestaltHasFSSpecCalls 			1					/*  has FSSpec calls  */
#define gestaltHasFileSystemManager 	2					/*  has a file system manager  */
#define gestaltFSMDoesDynamicLoad 		3					/*  file system manager supports dynamic loading  */
#define gestaltFSSupports4GBVols 		4					/*  file system supports 4 gigabyte volumes  */
#define gestaltFSSupports2TBVols 		5					/*  file system supports 2 terabyte volumes  */
#define gestaltHasExtendedDiskInit 		6					/*  has extended Disk Initialization calls  */
#define gestaltDTMgrSupportsFSM 		7					/*  Desktop Manager support FSM-based foreign file systems  */
#define gestaltFSNoMFSVols 				8					/*  file system doesn't supports MFS volumes  */
#define gestaltFSSupportsHFSPlusVols 	9					/*  file system supports HFS Plus volumes  */
#define gestaltFSIncompatibleDFA82 		10					/*  VCB and FCB structures changed; DFA 8.2 is incompatible  */

#define gestaltFSSupportsDirectIO 		11					/*  file system supports DirectIO  */
#define gestaltHasHFSPlusAPIs 			12					/*  file system supports HFS Plus APIs  */
#define gestaltMustUseFCBAccessors 		13					/*  FCBSPtr and FSFCBLen are invalid - must use FSM FCB accessor functions */
#define gestaltFSUsesPOSIXPathsForConversion  14			/*  The path interchange routines operate on POSIX paths instead of HFS paths  */
#define gestaltFSSupportsExclusiveLocks  15					/*  File system uses POSIX O_EXLOCK for opens  */
#define gestaltFSSupportsHardLinkDetection  16				/*  File system returns if an item is a hard link  */
#define gestaltFSAllowsConcurrentAsyncIO  17				/*  File Manager supports concurrent async reads and writes  */

#define gestaltAdminFeaturesFlagsAttr 	'fred'				/*  a set of admin flags, mostly useful internally.  */
#define gestaltFinderUsesSpecialOpenFoldersFile  0			/*  the Finder uses a special file to store the list of open folders  */

#define gestaltFSMVersion 				'fsm '				/*  returns version of HFS External File Systems Manager (FSM)  */
#define gestaltFXfrMgrAttr 				'fxfr'				/*  file transfer manager attributes  */
#define gestaltFXfrMgrPresent 			0
#define gestaltFXfrMgrMultiFile 		1					/*  supports FTSend and FTReceive  */
#define gestaltFXfrMgrErrorString 		2					/*  supports FTGetErrorString  */
#define gestaltFXfrMgrAsync 			3					/* supports FTSendAsync, FTReceiveAsync, FTCompletionAsync */

#define gestaltGraphicsAttr 			'gfxa'				/*  Quickdraw GX attributes selector  */
#define gestaltGraphicsIsDebugging 		0x00000001
#define gestaltGraphicsIsLoaded 		0x00000002
#define gestaltGraphicsIsPowerPC 		0x00000004

#define gestaltGraphicsVersion 			'grfx'				/*  Quickdraw GX version selector  */
#define gestaltCurrentGraphicsVersion 	0x00010200			/*  the version described in this set of headers  */

#define gestaltHardwareAttr 			'hdwr'				/*  hardware attributes  */
#define gestaltHasVIA1 					0					/*  VIA1 exists  */
#define gestaltHasVIA2 					1					/*  VIA2 exists  */
#define gestaltHasASC 					3					/*  Apple Sound Chip exists  */
#define gestaltHasSCC 					4					/*  SCC exists  */
#define gestaltHasSCSI 					7					/*  SCSI exists  */
#define gestaltHasSoftPowerOff 			19					/*  Capable of software power off  */
#define gestaltHasSCSI961 				21					/*  53C96 SCSI controller on internal bus  */
#define gestaltHasSCSI962 				22					/*  53C96 SCSI controller on external bus  */
#define gestaltHasUniversalROM 			24					/*  Do we have a Universal ROM?  */
#define gestaltHasEnhancedLtalk 		30					/*  Do we have Enhanced LocalTalk?  */

#define gestaltHelpMgrAttr 				'help'				/*  Help Mgr Attributes  */
#define gestaltHelpMgrPresent 			0					/*  true if help mgr is present  */
#define gestaltHelpMgrExtensions 		1					/*  true if help mgr extensions are installed  */
#define gestaltAppleGuideIsDebug 		30
#define gestaltAppleGuidePresent 		31					/*  true if AppleGuide is installed  */

#define gestaltHardwareVendorCode 		'hrad'				/*  Returns hardware vendor information  */
#define gestaltHardwareVendorApple 		'Appl'				/*  Hardware built by Apple  */

#define gestaltCompressionMgr 			'icmp'				/*  returns version of the Image Compression Manager  */
#define gestaltIconUtilitiesAttr 		'icon'				/*  Icon Utilities attributes  (Note: available in System 7.0, despite gestalt)  */
#define gestaltIconUtilitiesPresent 	0					/*  true if icon utilities are present  */
#define gestaltIconUtilitiesHas48PixelIcons  1				/*  true if 48x48 icons are supported by IconUtilities  */
#define gestaltIconUtilitiesHas32BitIcons  2				/*  true if 32-bit deep icons are supported  */
#define gestaltIconUtilitiesHas8BitDeepMasks  3				/*  true if 8-bit deep masks are supported  */
#define gestaltIconUtilitiesHasIconServices  4				/*  true if IconServices is present  */

#define gestaltInternalDisplay 			'idsp'				/*  slot number of internal display location  */
#define gestaltKeyboardType 			'kbd '				/*  keyboard type  */
#define gestaltMacKbd 					1
#define gestaltMacAndPad 				2
#define gestaltMacPlusKbd 				3					/*  OBSOLETE: This pre-ADB keyboard is not supported by any Mac OS X hardware and this value now means gestaltUnknownThirdPartyKbd  */
#define gestaltUnknownThirdPartyKbd 	3					/*  Unknown 3rd party keyboard.  */
#define gestaltExtADBKbd 				4
#define gestaltStdADBKbd 				5
#define gestaltPrtblADBKbd 				6
#define gestaltPrtblISOKbd 				7
#define gestaltStdISOADBKbd 			8
#define gestaltExtISOADBKbd 			9
#define gestaltADBKbdII 				10
#define gestaltADBISOKbdII 				11
#define gestaltPwrBookADBKbd 			12
#define gestaltPwrBookISOADBKbd 		13
#define gestaltAppleAdjustKeypad 		14
#define gestaltAppleAdjustADBKbd 		15
#define gestaltAppleAdjustISOKbd 		16
#define gestaltJapanAdjustADBKbd 		17					/*  Japan Adjustable Keyboard  */
#define gestaltPwrBkExtISOKbd 			20					/*  PowerBook Extended International Keyboard with function keys  */
#define gestaltPwrBkExtJISKbd 			21					/*  PowerBook Extended Japanese Keyboard with function keys       */
#define gestaltPwrBkExtADBKbd 			24					/*  PowerBook Extended Domestic Keyboard with function keys       */
#define gestaltPS2Keyboard 				27					/*  PS2 keyboard  */
#define gestaltPwrBkSubDomKbd 			28					/*  PowerBook Subnote Domestic Keyboard with function keys w/  inverted T   */
#define gestaltPwrBkSubISOKbd 			29					/*  PowerBook Subnote International Keyboard with function keys w/  inverted T      */
#define gestaltPwrBkSubJISKbd 			30					/*  PowerBook Subnote Japanese Keyboard with function keys w/ inverted T     */
#define gestaltPortableUSBANSIKbd 		37					/*  Powerbook USB-based internal keyboard, ANSI layout  */
#define gestaltPortableUSBISOKbd 		38					/*  Powerbook USB-based internal keyboard, ISO layout  */
#define gestaltPortableUSBJISKbd 		39					/*  Powerbook USB-based internal keyboard, JIS layout  */
#define gestaltThirdPartyANSIKbd 		40					/*  Third party keyboard, ANSI layout.  Returned in Mac OS X Tiger and later.  */
#define gestaltThirdPartyISOKbd 		41					/*  Third party keyboard, ISO layout. Returned in Mac OS X Tiger and later.  */
#define gestaltThirdPartyJISKbd 		42					/*  Third party keyboard, JIS layout. Returned in Mac OS X Tiger and later.  */
#define gestaltPwrBkEKDomKbd 			195					/*  (0xC3) PowerBook Domestic Keyboard with Embedded Keypad, function keys & inverted T     */
#define gestaltPwrBkEKISOKbd 			196					/*  (0xC4) PowerBook International Keyboard with Embedded Keypad, function keys & inverted T    */
#define gestaltPwrBkEKJISKbd 			197					/*  (0xC5) PowerBook Japanese Keyboard with Embedded Keypad, function keys & inverted T       */
#define gestaltUSBCosmoANSIKbd 			198					/*  (0xC6) original USB Domestic (ANSI) Keyboard  */
#define gestaltUSBCosmoISOKbd 			199					/*  (0xC7) original USB International (ISO) Keyboard  */
#define gestaltUSBCosmoJISKbd 			200					/*  (0xC8) original USB Japanese (JIS) Keyboard  */
#define gestaltPwrBk99JISKbd 			201					/*  (0xC9) '99 PowerBook JIS Keyboard with Embedded Keypad, function keys & inverted T                */
#define gestaltUSBAndyANSIKbd 			204					/*  (0xCC) USB Pro Keyboard Domestic (ANSI) Keyboard                                  */
#define gestaltUSBAndyISOKbd 			205					/*  (0xCD) USB Pro Keyboard International (ISO) Keyboard                                */
#define gestaltUSBAndyJISKbd 			206					/*  (0xCE) USB Pro Keyboard Japanese (JIS) Keyboard                                     */

#define gestaltPortable2001ANSIKbd 		202					/*  (0xCA) PowerBook and iBook Domestic (ANSI) Keyboard with 2nd cmd key right & function key moves.      */
#define gestaltPortable2001ISOKbd 		203					/*  (0xCB) PowerBook and iBook International (ISO) Keyboard with 2nd cmd key right & function key moves.    */
#define gestaltPortable2001JISKbd 		207					/*  (0xCF) PowerBook and iBook Japanese (JIS) Keyboard with function key moves.                    */

#define gestaltUSBProF16ANSIKbd 		34					/*  (0x22) USB Pro Keyboard w/ F16 key Domestic (ANSI) Keyboard  */
#define gestaltUSBProF16ISOKbd 			35					/*  (0x23) USB Pro Keyboard w/ F16 key International (ISO) Keyboard  */
#define gestaltUSBProF16JISKbd 			36					/*  (0x24) USB Pro Keyboard w/ F16 key Japanese (JIS) Keyboard  */
#define gestaltProF16ANSIKbd 			31					/*  (0x1F) Pro Keyboard w/F16 key Domestic (ANSI) Keyboard  */
#define gestaltProF16ISOKbd 			32					/*  (0x20) Pro Keyboard w/F16 key International (ISO) Keyboard  */
#define gestaltProF16JISKbd 			33					/*  (0x21) Pro Keyboard w/F16 key Japanese (JIS) Keyboard  */

#define gestaltUDFSupport 				'kudf'				/*     Used for communication between UDF implementations */
#define gestaltLowMemorySize 			'lmem'				/*  size of low memory area  */
#define gestaltLogicalRAMSize 			'lram'				/*  logical ram size  */
#define gestaltMachineType 				'mach'				/*  machine type  */
#define gestaltClassic 					1
#define gestaltMacXL 					2
#define gestaltMac512KE 				3
#define gestaltMacPlus 					4
#define gestaltMacSE 					5
#define gestaltMacII 					6
#define gestaltMacIIx 					7
#define gestaltMacIIcx 					8
#define gestaltMacSE030 				9
#define gestaltPortable 				10
#define gestaltMacIIci 					11
#define gestaltPowerMac8100_120 		12
#define gestaltMacIIfx 					13
#define gestaltMacClassic 				17
#define gestaltMacIIsi 					18
#define gestaltMacLC 					19
#define gestaltMacQuadra900 			20
#define gestaltPowerBook170 			21
#define gestaltMacQuadra700 			22
#define gestaltClassicII 				23
#define gestaltPowerBook100 			24
#define gestaltPowerBook140 			25
#define gestaltMacQuadra950 			26
#define gestaltMacLCIII 				27
#define gestaltPerforma450 				27
#define gestaltPowerBookDuo210 			29
#define gestaltMacCentris650 			30
#define gestaltPowerBookDuo230 			32
#define gestaltPowerBook180 			33
#define gestaltPowerBook160 			34
#define gestaltMacQuadra800 			35
#define gestaltMacQuadra650 			36
#define gestaltMacLCII 					37
#define gestaltPowerBookDuo250 			38
#define gestaltAWS9150_80 				39
#define gestaltPowerMac8100_110 		40
#define gestaltAWS8150_110 				40
#define gestaltPowerMac5200 			41
#define gestaltPowerMac5260 			41
#define gestaltPerforma5300 			41
#define gestaltPowerMac6200 			42
#define gestaltPerforma6300 			42
#define gestaltMacIIvi 					44
#define gestaltMacIIvm 					45
#define gestaltPerforma600 				45
#define gestaltPowerMac7100_80 			47
#define gestaltMacIIvx 					48
#define gestaltMacColorClassic 			49
#define gestaltPerforma250 				49
#define gestaltPowerBook165c 			50
#define gestaltMacCentris610 			52
#define gestaltMacQuadra610 			53
#define gestaltPowerBook145 			54
#define gestaltPowerMac8100_100 		55
#define gestaltMacLC520 				56
#define gestaltAWS9150_120 				57
#define gestaltPowerMac6400 			58
#define gestaltPerforma6400 			58
#define gestaltPerforma6360 			58
#define gestaltMacCentris660AV 			60
#define gestaltMacQuadra660AV 			60
#define gestaltPerforma46x 				62
#define gestaltPowerMac8100_80 			65
#define gestaltAWS8150_80 				65
#define gestaltPowerMac9500 			67
#define gestaltPowerMac9600 			67
#define gestaltPowerMac7500 			68
#define gestaltPowerMac7600 			68
#define gestaltPowerMac8500 			69
#define gestaltPowerMac8600 			69
#define gestaltAWS8550 					68
#define gestaltPowerBook180c 			71
#define gestaltPowerBook520 			72
#define gestaltPowerBook520c 			72
#define gestaltPowerBook540 			72
#define gestaltPowerBook540c 			72
#define gestaltPowerMac5400 			74
#define gestaltPowerMac6100_60 			75
#define gestaltAWS6150_60 				75
#define gestaltPowerBookDuo270c 		77
#define gestaltMacQuadra840AV 			78
#define gestaltPerforma550 				80
#define gestaltPowerBook165 			84
#define gestaltPowerBook190 			85
#define gestaltMacTV 					88
#define gestaltMacLC475 				89
#define gestaltPerforma47x 				89
#define gestaltMacLC575 				92
#define gestaltMacQuadra605 			94
#define gestaltMacQuadra630 			98
#define gestaltMacLC580 				99
#define gestaltPerforma580 				99
#define gestaltPowerMac6100_66 			100
#define gestaltAWS6150_66 				100
#define gestaltPowerBookDuo280 			102
#define gestaltPowerBookDuo280c 		103
#define gestaltPowerMacLC475 			104					/*  Mac LC 475 & PPC Processor Upgrade Card */
#define gestaltPowerMacPerforma47x 		104
#define gestaltPowerMacLC575 			105					/*  Mac LC 575 & PPC Processor Upgrade Card  */
#define gestaltPowerMacPerforma57x 		105
#define gestaltPowerMacQuadra630 		106					/*  Quadra 630 & PPC Processor Upgrade Card */
#define gestaltPowerMacLC630 			106					/*  Mac LC 630 & PPC Processor Upgrade Card */
#define gestaltPowerMacPerforma63x 		106					/*  Performa 63x & PPC Processor Upgrade Card */
#define gestaltPowerMac7200 			108
#define gestaltPowerMac7300 			109
#define gestaltPowerMac7100_66 			112
#define gestaltPowerBook150 			115
#define gestaltPowerMacQuadra700 		116					/*  Quadra 700 & Power PC Upgrade Card */
#define gestaltPowerMacQuadra900 		117					/*  Quadra 900 & Power PC Upgrade Card  */
#define gestaltPowerMacQuadra950 		118					/*  Quadra 950 & Power PC Upgrade Card  */
#define gestaltPowerMacCentris610 		119					/*  Centris 610 & Power PC Upgrade Card  */
#define gestaltPowerMacCentris650 		120					/*  Centris 650 & Power PC Upgrade Card  */
#define gestaltPowerMacQuadra610 		121					/*  Quadra 610 & Power PC Upgrade Card  */
#define gestaltPowerMacQuadra650 		122					/*  Quadra 650 & Power PC Upgrade Card  */
#define gestaltPowerMacQuadra800 		123					/*  Quadra 800 & Power PC Upgrade Card  */
#define gestaltPowerBookDuo2300 		124
#define gestaltPowerBook500PPCUpgrade 	126
#define gestaltPowerBook5300 			128
#define gestaltPowerBook1400 			310
#define gestaltPowerBook3400 			306
#define gestaltPowerBook2400 			307
#define gestaltPowerBookG3Series 		312
#define gestaltPowerBookG3 				313
#define gestaltPowerBookG3Series2 		314
#define gestaltPowerMacNewWorld 		406					/*  All NewWorld architecture Macs (iMac, blue G3, etc.) */
#define gestaltPowerMacG3 				510
#define gestaltPowerMac5500 			512
#define gestalt20thAnniversary 			512
#define gestaltPowerMac6500 			513
#define gestaltPowerMac4400_160 		514					/*  slower machine has different machine ID */
#define gestaltPowerMac4400 			515
#define gestaltMacOSCompatibility 		1206				/*     Mac OS Compatibility on Mac OS X (Classic) */

#define gestaltQuadra605 				94
#define gestaltQuadra610 				53
#define gestaltQuadra630 				98
#define gestaltQuadra650 				36
#define gestaltQuadra660AV 				60
#define gestaltQuadra700 				22
#define gestaltQuadra800 				35
#define gestaltQuadra840AV 				78
#define gestaltQuadra900 				20
#define gestaltQuadra950 				26

#define kMachineNameStrID 				(-16395)
#define gestaltSMPMailerVersion 		'malr'				/*  OCE StandardMail */
#define gestaltMediaBay 				'mbeh'				/*  media bay driver type  */
#define gestaltMBLegacy 				0					/*  media bay support in PCCard 2.0  */
#define gestaltMBSingleBay 				1					/*  single bay media bay driver  */
#define gestaltMBMultipleBays 			2					/*  multi-bay media bay driver  */

#define gestaltMessageMgrVersion 		'mess'				/*  GX Printing Message Manager Gestalt Selector  */
#define gestaltMenuMgrAttr 				'menu'				/*  If this Gestalt exists, the Mac OS 8.5 Menu Manager is installed  */
#define gestaltMenuMgrPresent 			0x00000001			/*  NOTE: this is a bit mask, whereas all other Gestalt constants of this nature  */
															/*  are bit index values. 3.2 interfaces slipped out with this mistake unnoticed.  */
															/*  Sincere apologies for any inconvenience. */
#define gestaltMenuMgrPresentBit 		0					/*  bit number  */
#define gestaltMenuMgrAquaLayoutBit 	1					/*  menus have the Aqua 1.0 layout */
#define gestaltMenuMgrMultipleItemsWithCommandIDBit  2		/*  CountMenuItemsWithCommandID/GetIndMenuItemWithCommandID support multiple items with the same command ID */
#define gestaltMenuMgrRetainsIconRefBit  3					/*  SetMenuItemIconHandle, when passed an IconRef, calls AcquireIconRef */
#define gestaltMenuMgrSendsMenuBoundsToDefProcBit  4		/*  kMenuSizeMsg and kMenuPopUpMsg have menu bounding rect information */
#define gestaltMenuMgrMoreThanFiveMenusDeepBit  5			/*  the Menu Manager supports hierarchical menus more than five deep */
#define gestaltMenuMgrCGImageMenuTitleBit  6				/*  SetMenuTitleIcon supports CGImageRefs */
															/*  masks for the above bits */
#define gestaltMenuMgrPresentMask 		0x00000001
#define gestaltMenuMgrAquaLayoutMask 	0x00000002
#define gestaltMenuMgrMultipleItemsWithCommandIDMask  0x00000004
#define gestaltMenuMgrRetainsIconRefMask  0x00000008
#define gestaltMenuMgrSendsMenuBoundsToDefProcMask  0x00000010
#define gestaltMenuMgrMoreThanFiveMenusDeepMask  0x00000020
#define gestaltMenuMgrCGImageMenuTitleMask  0x00000040

#define gestaltMultipleUsersState 		'mfdr'				/*  Gestalt selector returns MultiUserGestaltHandle (in Folders.h) */
#define gestaltMachineIcon 				'micn'				/*  machine icon  */
#define gestaltMiscAttr 				'misc'				/*  miscellaneous attributes  */
#define gestaltScrollingThrottle 		0					/*  true if scrolling throttle on  */
#define gestaltSquareMenuBar 			2					/*  true if menu bar is square  */

#define gestaltMixedModeVersion 		'mixd'				/*  returns version of Mixed Mode  */
#define gestaltMixedModeAttr 			'mixd'				/*  returns Mixed Mode attributes  */
#define gestaltMixedModePowerPC 		0					/*  true if Mixed Mode supports PowerPC ABI calling conventions  */
#define gestaltPowerPCAware 			0					/*  old name for gestaltMixedModePowerPC  */
#define gestaltMixedModeCFM68K 			1					/*  true if Mixed Mode supports CFM-68K calling conventions  */
#define gestaltMixedModeCFM68KHasTrap 	2					/*  true if CFM-68K Mixed Mode implements _MixedModeDispatch (versions 1.0.1 and prior did not)  */
#define gestaltMixedModeCFM68KHasState 	3					/*  true if CFM-68K Mixed Mode exports Save/RestoreMixedModeState  */

#define gestaltQuickTimeConferencing 	'mtlk'				/*  returns QuickTime Conferencing version  */
#define gestaltMemoryMapAttr 			'mmap'				/*  Memory map type  */
#define gestaltMemoryMapSparse 			0					/*  Sparse memory is on  */

#define gestaltMMUType 					'mmu '				/*  mmu type  */
#define gestaltNoMMU 					0					/*  no MMU  */
#define gestaltAMU 						1					/*  address management unit  */
#define gestalt68851 					2					/*  68851 PMMU  */
#define gestalt68030MMU 				3					/*  68030 built-in MMU  */
#define gestalt68040MMU 				4					/*  68040 built-in MMU  */
#define gestaltEMMU1 					5					/*  Emulated MMU type 1   */

															/*     On Mac OS X, the user visible machine name may something like "PowerMac3,4", which is */
															/*     a unique string for each signifigant Macintosh computer which Apple creates, but is */
															/*     not terribly useful as a user visible string. */
#define gestaltUserVisibleMachineName 	'mnam'				/*  Coerce response into a StringPtr to get a user visible machine name  */
#define gestaltMPCallableAPIsAttr 		'mpsc'				/*  Bitmap of toolbox/OS managers that can be called from MPLibrary MPTasks  */
#define gestaltMPFileManager 			0					/*  True if File Manager calls can be made from MPTasks  */
#define gestaltMPDeviceManager 			1					/*  True if synchronous Device Manager calls can be made from MPTasks  */
#define gestaltMPTrapCalls 				2					/*  True if most trap-based calls can be made from MPTasks  */

#define gestaltStdNBPAttr 				'nlup'				/*  standard nbp attributes  */
#define gestaltStdNBPPresent 			0
#define gestaltStdNBPSupportsAutoPosition  1				/*  StandardNBP takes (-1,-1) to mean alert position main screen  */

#define gestaltNotificationMgrAttr 		'nmgr'				/*  notification manager attributes  */
#define gestaltNotificationPresent 		0					/*  notification manager exists  */

#define gestaltNameRegistryVersion 		'nreg'				/*  NameRegistryLib version number, for System 7.5.2+ usage  */
#define gestaltNuBusSlotCount 			'nubs'				/*  count of logical NuBus slots present  */
#define gestaltOCEToolboxVersion 		'ocet'				/*  OCE Toolbox version  */
#define gestaltOCETB 					0x0102				/*  OCE Toolbox version 1.02  */
#define gestaltSFServer 				0x0100				/*  S&F Server version 1.0  */

#define gestaltOCEToolboxAttr 			'oceu'				/*  OCE Toolbox attributes  */
#define gestaltOCETBPresent 			0x01				/*  OCE toolbox is present, not running  */
#define gestaltOCETBAvailable 			0x02				/*  OCE toolbox is running and available  */
#define gestaltOCESFServerAvailable 	0x04				/*  S&F Server is running and available  */
#define gestaltOCETBNativeGlueAvailable  0x10				/*  Native PowerPC Glue routines are availible  */

#define gestaltOpenFirmwareInfo 		'opfw'				/*  Open Firmware info  */
#define gestaltOSAttr 					'os  '				/*  o/s attributes  */
#define gestaltSysZoneGrowable 			0					/*  system heap is growable  */
#define gestaltLaunchCanReturn 			1					/*  can return from launch  */
#define gestaltLaunchFullFileSpec 		2					/*  can launch from full file spec  */
#define gestaltLaunchControl 			3					/*  launch control support available  */
#define gestaltTempMemSupport 			4					/*  temp memory support  */
#define gestaltRealTempMemory 			5					/*  temp memory handles are real  */
#define gestaltTempMemTracked 			6					/*  temporary memory handles are tracked  */
#define gestaltIPCSupport 				7					/*  IPC support is present  */
#define gestaltSysDebuggerSupport 		8					/*  system debugger support is present  */
#define gestaltNativeProcessMgrBit 		19					/*  the process manager itself is native  */
#define gestaltAltivecRegistersSwappedCorrectlyBit  20		/*  Altivec registers are saved correctly on process switches  */

#define gestaltOSTable 					'ostt'				/*   OS trap table base   */
#define gestaltOpenTptNetworkSetup 		'otcf'
#define gestaltOpenTptNetworkSetupLegacyImport  0
#define gestaltOpenTptNetworkSetupLegacyExport  1
#define gestaltOpenTptNetworkSetupSupportsMultihoming  2

#define gestaltOpenTptNetworkSetupVersion  'otcv'
#define gestaltOpenTptRemoteAccess 		'otra'
#define gestaltOpenTptRemoteAccessPresent  0
#define gestaltOpenTptRemoteAccessLoaded  1
#define gestaltOpenTptRemoteAccessClientOnly  2
#define gestaltOpenTptRemoteAccessPServer  3
#define gestaltOpenTptRemoteAccessMPServer  4
#define gestaltOpenTptPPPPresent 		5
#define gestaltOpenTptARAPPresent 		6

#define gestaltOpenTptRemoteAccessVersion  'otrv'
#define gestaltOpenTptVersions 			'otvr'				/*  Defined by OT 1.1 and higher, response is NumVersion. */
#define gestaltOpenTpt 					'otan'				/*  Defined by all versions, response is defined below. */
#define gestaltOpenTptPresentMask 		0x00000001
#define gestaltOpenTptLoadedMask 		0x00000002
#define gestaltOpenTptAppleTalkPresentMask  0x00000004
#define gestaltOpenTptAppleTalkLoadedMask  0x00000008
#define gestaltOpenTptTCPPresentMask 	0x00000010
#define gestaltOpenTptTCPLoadedMask 	0x00000020
#define gestaltOpenTptIPXSPXPresentMask  0x00000040
#define gestaltOpenTptIPXSPXLoadedMask 	0x00000080
#define gestaltOpenTptPresentBit 		0
#define gestaltOpenTptLoadedBit 		1
#define gestaltOpenTptAppleTalkPresentBit  2
#define gestaltOpenTptAppleTalkLoadedBit  3
#define gestaltOpenTptTCPPresentBit 	4
#define gestaltOpenTptTCPLoadedBit 		5
#define gestaltOpenTptIPXSPXPresentBit 	6
#define gestaltOpenTptIPXSPXLoadedBit 	7

#define gestaltPCCard 					'pccd'				/*     PC Card attributes */
#define gestaltCardServicesPresent 		0					/*     PC Card 2.0 (68K) API is present */
#define gestaltPCCardFamilyPresent 		1					/*     PC Card 3.x (PowerPC) API is present */
#define gestaltPCCardHasPowerControl 	2					/*     PCCardSetPowerLevel is supported */
#define gestaltPCCardSupportsCardBus 	3					/*     CardBus is supported */

#define gestaltProcClkSpeed 			'pclk'				/*  processor clock speed in hertz (a UInt32)  */
#define gestaltProcClkSpeedMHz 			'mclk'				/*  processor clock speed in megahertz (a UInt32)  */
#define gestaltPCXAttr 					'pcxg'				/*  PC Exchange attributes  */
#define gestaltPCXHas8and16BitFAT 		0					/*  PC Exchange supports both 8 and 16 bit FATs  */
#define gestaltPCXHasProDOS 			1					/*  PC Exchange supports ProDOS  */
#define gestaltPCXNewUI 				2
#define gestaltPCXUseICMapping 			3					/*  PC Exchange uses InternetConfig for file mappings  */

#define gestaltLogicalPageSize 			'pgsz'				/*  logical page size  */
#define gestaltScreenCaptureMain 		'pic1'				/*  Zero, or vRefNum of disk to hold picture  */
#define gestaltScreenCaptureDir 		'pic2'				/*  Directory ID of folder to hold picture  */

#define gestaltGXPrintingMgrVersion 	'pmgr'				/*  QuickDraw GX Printing Manager Version */
#define gestaltPopupAttr 				'pop!'				/*  popup cdef attributes  */
#define gestaltPopupPresent 			0

#define gestaltPowerMgrAttr 			'powr'				/*  power manager attributes  */
#define gestaltPMgrExists 				0
#define gestaltPMgrCPUIdle 				1
#define gestaltPMgrSCC 					2
#define gestaltPMgrSound 				3
#define gestaltPMgrDispatchExists 		4
#define gestaltPMgrSupportsAVPowerStateAtSleepWake  5

#define gestaltPowerMgrVers 			'pwrv'				/*  power manager version  */
#define gestaltPPCToolboxAttr 			'ppc '				/*  PPC toolbox attributes  */
#define gestaltPPCToolboxPresent 		0x0000				/*  PPC Toolbox is present  Requires PPCInit to be called  */
#define gestaltPPCSupportsRealTime 		0x1000				/*  PPC Supports real-time delivery  */
#define gestaltPPCSupportsIncoming 		0x0001				/*  PPC will allow incoming network requests  */
#define gestaltPPCSupportsOutGoing 		0x0002				/*  PPC will allow outgoing network requests  */
#define gestaltPPCSupportsTCP_IP 		0x0004				/*  PPC supports TCP/IP transport   */
#define gestaltPPCSupportsIncomingAppleTalk  0x0010
#define gestaltPPCSupportsIncomingTCP_IP  0x0020
#define gestaltPPCSupportsOutgoingAppleTalk  0x0100
#define gestaltPPCSupportsOutgoingTCP_IP  0x0200

#define gestaltPowerPCProcessorFeatures  'ppcf'				/*  Optional PowerPC processor features  */
#define gestaltPowerPCHasGraphicsInstructions  0			/*  has fres, frsqrte, and fsel instructions  */
#define gestaltPowerPCHasSTFIWXInstruction  1				/*  has stfiwx instruction  */
#define gestaltPowerPCHasSquareRootInstructions  2			/*  has fsqrt and fsqrts instructions  */
#define gestaltPowerPCHasDCBAInstruction  3					/*  has dcba instruction  */
#define gestaltPowerPCHasVectorInstructions  4				/*  has vector instructions  */
#define gestaltPowerPCHasDataStreams 	5					/*  has dst, dstt, dstst, dss, and dssall instructions  */
#define gestaltPowerPCHas64BitSupport 	6					/*  double word LSU/ALU, etc.  */
#define gestaltPowerPCHasDCBTStreams 	7					/*  TH field of DCBT recognized  */
#define gestaltPowerPCASArchitecture 	8					/*  chip uses new 'A/S' architecture  */
#define gestaltPowerPCIgnoresDCBST 		9					/*   */

#define gestaltProcessorType 			'proc'				/*  processor type  */
#define gestalt68000 					1
#define gestalt68010 					2
#define gestalt68020 					3
#define gestalt68030 					4
#define gestalt68040 					5

#define gestaltSDPPromptVersion 		'prpv'				/*  OCE Standard Directory Panel */
#define gestaltParityAttr 				'prty'				/*  parity attributes  */
#define gestaltHasParityCapability 		0					/*  has ability to check parity  */
#define gestaltParityEnabled 			1					/*  parity checking enabled  */

#define gestaltQD3DVersion 				'q3v '				/*  Quickdraw 3D version in pack BCD */
#define gestaltQD3DViewer 				'q3vc'				/*  Quickdraw 3D viewer attributes */
#define gestaltQD3DViewerPresent 		0					/*  bit 0 set if QD3D Viewer is available */

#if OLDROUTINENAMES
#define gestaltQD3DViewerNotPresent 	0x00
#define gestaltQD3DViewerAvailable 		0x01

#endif  /* OLDROUTINENAMES */

#define gestaltQuickdrawVersion 		'qd  '				/*  quickdraw version  */
#define gestaltOriginalQD 				0x0000				/*  original 1-bit QD  */
#define gestalt8BitQD 					0x0100				/*  8-bit color QD  */
#define gestalt32BitQD 					0x0200				/*  32-bit color QD  */
#define gestalt32BitQD11 				0x0201				/*  32-bit color QDv1.1  */
#define gestalt32BitQD12 				0x0220				/*  32-bit color QDv1.2  */
#define gestalt32BitQD13 				0x0230				/*  32-bit color QDv1.3  */
#define gestaltAllegroQD 				0x0250				/*  Allegro QD OS 8.5  */
#define gestaltMacOSXQD 				0x0300				/*  0x310, 0x320 etc. for 10.x.y  */

#define gestaltQD3D 					'qd3d'				/*  Quickdraw 3D attributes */
#define gestaltQD3DPresent 				0					/*  bit 0 set if QD3D available */

#if OLDROUTINENAMES
#define gestaltQD3DNotPresent 			0x00
#define gestaltQD3DAvailable 			0x01

#endif  /* OLDROUTINENAMES */

#define gestaltGXVersion 				'qdgx'				/*  Overall QuickDraw GX Version */
#define gestaltQuickdrawFeatures 		'qdrw'				/*  quickdraw features  */
#define gestaltHasColor 				0					/*  color quickdraw present  */
#define gestaltHasDeepGWorlds 			1					/*  GWorlds can be deeper than 1-bit  */
#define gestaltHasDirectPixMaps 		2					/*  PixMaps can be direct (16 or 32 bit)  */
#define gestaltHasGrayishTextOr 		3					/*  supports text mode grayishTextOr  */
#define gestaltSupportsMirroring 		4					/*  Supports video mirroring via the Display Manager.  */
#define gestaltQDHasLongRowBytes 		5					/*  Long rowBytes supported in GWorlds  */

#define gestaltQDTextVersion 			'qdtx'				/*  QuickdrawText version  */
#define gestaltOriginalQDText 			0x0000				/*  up to and including 8.1  */
#define gestaltAllegroQDText 			0x0100				/*  starting with 8.5  */
#define gestaltMacOSXQDText 			0x0200				/*  we are in Mac OS X  */

#define gestaltQDTextFeatures 			'qdtf'				/*  QuickdrawText features  */
#define gestaltWSIISupport 				0					/*  bit 0: WSII support included  */
#define gestaltSbitFontSupport 			1					/*  sbit-only fonts supported  */
#define gestaltAntiAliasedTextAvailable  2					/*  capable of antialiased text  */
#define gestaltOFA2available 			3					/*  OFA2 available  */
#define gestaltCreatesAliasFontRsrc 	4					/*  "real" datafork font support  */
#define gestaltNativeType1FontSupport 	5					/*  we have scaler for Type1 fonts  */
#define gestaltCanUseCGTextRendering 	6

#define gestaltQuickTimeConferencingInfo  'qtci'			/*  returns pointer to QuickTime Conferencing information  */
#define gestaltQuickTimeVersion 		'qtim'				/*  returns version of QuickTime  */
#define gestaltQuickTime 				'qtim'				/*  gestaltQuickTime is old name for gestaltQuickTimeVersion  */

#define gestaltQuickTimeFeatures 		'qtrs'
#define gestaltPPCQuickTimeLibPresent 	0					/*  PowerPC QuickTime glue library is present  */

#define gestaltQuickTimeStreamingFeatures  'qtsf'
#define gestaltQuickTimeStreamingVersion  'qtst'
#define gestaltQuickTimeThreadSafeFeaturesAttr  'qtth'		/*  Quicktime thread safety attributes  */
#define gestaltQuickTimeThreadSafeICM 	0
#define gestaltQuickTimeThreadSafeMovieToolbox  1
#define gestaltQuickTimeThreadSafeMovieImport  2
#define gestaltQuickTimeThreadSafeMovieExport  3
#define gestaltQuickTimeThreadSafeGraphicsImport  4
#define gestaltQuickTimeThreadSafeGraphicsExport  5
#define gestaltQuickTimeThreadSafeMoviePlayback  6

#define gestaltQTVRMgrAttr 				'qtvr'				/*  QuickTime VR attributes                                */
#define gestaltQTVRMgrPresent 			0					/*  QTVR API is present                                    */
#define gestaltQTVRObjMoviesPresent 	1					/*  QTVR runtime knows about object movies                 */
#define gestaltQTVRCylinderPanosPresent  2					/*  QTVR runtime knows about cylindrical panoramic movies  */
#define gestaltQTVRCubicPanosPresent 	3					/*  QTVR runtime knows about cubic panoramic movies        */

#define gestaltQTVRMgrVers 				'qtvv'				/*  QuickTime VR version                                   */
#define gestaltPhysicalRAMSize 			'ram '				/*  physical RAM size, in bytes  */
#define gestaltPhysicalRAMSizeInMegabytes  'ramm'			/*  physical RAM size, scaled in megabytes  */
#define gestaltRBVAddr 					'rbv '				/*  RBV base address   */
#define gestaltROMSize 					'rom '				/*  rom size  */
#define gestaltROMVersion 				'romv'				/*  rom version  */
#define gestaltResourceMgrAttr 			'rsrc'				/*  Resource Mgr attributes  */
#define gestaltPartialRsrcs 			0					/*  True if partial resources exist  */
#define gestaltHasResourceOverrides 	1					/*  Appears in the ROM; so put it here.  */

#define gestaltResourceMgrBugFixesAttrs  'rmbg'				/*  Resource Mgr bug fixes  */
#define gestaltRMForceSysHeapRolledIn 	0
#define gestaltRMFakeAppleMenuItemsRolledIn  1
#define gestaltSanityCheckResourceFiles  2					/*  Resource manager does sanity checking on resource files before opening them  */
#define gestaltSupportsFSpResourceFileAlreadyOpenBit  3		/*  The resource manager supports GetResFileRefNum and FSpGetResFileRefNum and FSpResourceFileAlreadyOpen  */
#define gestaltRMSupportsFSCalls 		4					/*  The resource manager supports OpenResFileFSRef, CreateResFileFSRef and  ResourceFileAlreadyOpenFSRef  */
#define gestaltRMTypeIndexOrderingReverse  8				/*  GetIndType() calls return resource types in opposite order to original 68k resource manager  */

#define gestaltRealtimeMgrAttr 			'rtmr'				/*  Realtime manager attributes          */
#define gestaltRealtimeMgrPresent 		0					/*  true if the Realtime manager is present     */

#define gestaltSafeOFAttr 				'safe'
#define gestaltVMZerosPagesBit 			0
#define gestaltInitHeapZerosOutHeapsBit  1
#define gestaltNewHandleReturnsZeroedMemoryBit  2
#define gestaltNewPtrReturnsZeroedMemoryBit  3
#define gestaltFileAllocationZeroedBlocksBit  4

#define gestaltSCCReadAddr 				'sccr'				/*  scc read base address   */
#define gestaltSCCWriteAddr 			'sccw'				/*  scc read base address   */
#define gestaltScrapMgrAttr 			'scra'				/*  Scrap Manager attributes  */
#define gestaltScrapMgrTranslationAware  0					/*  True if scrap manager is translation aware  */

#define gestaltScriptMgrVersion 		'scri'				/*  Script Manager version number      */
#define gestaltScriptCount 				'scr#'				/*  number of active script systems    */
#define gestaltSCSI 					'scsi'				/*  SCSI Manager attributes  */
#define gestaltAsyncSCSI 				0					/*  Supports Asynchronous SCSI  */
#define gestaltAsyncSCSIINROM 			1					/*  Async scsi is in ROM (available for booting)  */
#define gestaltSCSISlotBoot 			2					/*  ROM supports Slot-style PRAM for SCSI boots (PDM and later)  */
#define gestaltSCSIPollSIH 				3					/*  SCSI Manager will poll for interrupts if Secondary Interrupts are busy.  */

#define gestaltControlStripAttr 		'sdev'				/*  Control Strip attributes  */
#define gestaltControlStripExists 		0					/*  Control Strip is installed  */
#define gestaltControlStripVersionFixed  1					/*  Control Strip version Gestalt selector was fixed  */
#define gestaltControlStripUserFont 	2					/*  supports user-selectable font/size  */
#define gestaltControlStripUserHotKey 	3					/*  support user-selectable hot key to show/hide the window  */

#define gestaltSDPStandardDirectoryVersion  'sdvr'			/*  OCE Standard Directory Panel */
#define gestaltSerialAttr 				'ser '				/*  Serial attributes  */
#define gestaltHasGPIaToDCDa 			0					/*  GPIa connected to DCDa */
#define gestaltHasGPIaToRTxCa 			1					/*  GPIa connected to RTxCa clock input */
#define gestaltHasGPIbToDCDb 			2					/*  GPIb connected to DCDb  */
#define gestaltHidePortA 				3					/*  Modem port (A) should be hidden from users  */
#define gestaltHidePortB 				4					/*  Printer port (B) should be hidden from users  */
#define gestaltPortADisabled 			5					/*  Modem port (A) disabled and should not be used by SW  */
#define gestaltPortBDisabled 			6					/*  Printer port (B) disabled and should not be used by SW  */

#define gestaltShutdownAttributes 		'shut'				/*  ShutDown Manager Attributes  */
#define gestaltShutdownHassdOnBootVolUnmount  0				/*  True if ShutDown Manager unmounts boot & VM volume at shutdown time.  */

#define gestaltNuBusConnectors 			'sltc'				/*  bitmap of NuBus connectors */
#define gestaltSlotAttr 				'slot'				/*  slot attributes   */
#define gestaltSlotMgrExists 			0					/*  true is slot mgr exists   */
#define gestaltNuBusPresent 			1					/*  NuBus slots are present   */
#define gestaltSESlotPresent 			2					/*  SE PDS slot present   */
#define gestaltSE30SlotPresent 			3					/*  SE/30 slot present   */
#define gestaltPortableSlotPresent 		4					/*  PortableÕs slot present   */

#define gestaltFirstSlotNumber 			'slt1'				/*  returns first physical slot  */
#define gestaltSoundAttr 				'snd '				/*  sound attributes  */
#define gestaltStereoCapability 		0					/*  sound hardware has stereo capability  */
#define gestaltStereoMixing 			1					/*  stereo mixing on external speaker  */
#define gestaltSoundIOMgrPresent 		3					/*  The Sound I/O Manager is present  */
#define gestaltBuiltInSoundInput 		4					/*  built-in Sound Input hardware is present  */
#define gestaltHasSoundInputDevice 		5					/*  Sound Input device available  */
#define gestaltPlayAndRecord 			6					/*  built-in hardware can play and record simultaneously  */
#define gestalt16BitSoundIO 			7					/*  sound hardware can play and record 16-bit samples  */
#define gestaltStereoInput 				8					/*  sound hardware can record stereo  */
#define gestaltLineLevelInput 			9					/*  sound input port requires line level  */
															/*  the following bits are not defined prior to Sound Mgr 3.0  */
#define gestaltSndPlayDoubleBuffer 		10					/*  SndPlayDoubleBuffer available, set by Sound Mgr 3.0 and later  */
#define gestaltMultiChannels 			11					/*  multiple channel support, set by Sound Mgr 3.0 and later  */
#define gestalt16BitAudioSupport 		12					/*  16 bit audio data supported, set by Sound Mgr 3.0 and later  */

#define gestaltSplitOSAttr 				'spos'
#define gestaltSplitOSBootDriveIsNetworkVolume  0			/*  the boot disk is a network 'disk', from the .LANDisk drive.  */
#define gestaltSplitOSAware 			1					/*  the system includes the code to deal with a split os situation.  */
#define gestaltSplitOSEnablerVolumeIsDifferentFromBootVolume  2 /*  the active enabler is on a different volume than the system file.  */
#define gestaltSplitOSMachineNameSetToNetworkNameTemp  3	/*  The machine name was set to the value given us from the BootP server  */
#define gestaltSplitOSMachineNameStartupDiskIsNonPersistent  5 /*  The startup disk ( vRefNum == -1 ) is non-persistent, meaning changes won't persist across a restart.  */

#define gestaltSMPSPSendLetterVersion 	'spsl'				/*  OCE StandardMail */
#define gestaltSpeechRecognitionAttr 	'srta'				/*  speech recognition attributes  */
#define gestaltDesktopSpeechRecognition  1					/*  recognition thru the desktop microphone is available  */
#define gestaltTelephoneSpeechRecognition  2				/*  recognition thru the telephone is available  */

#define gestaltSpeechRecognitionVersion  'srtb'				/*  speech recognition version (0x0150 is the first version that fully supports the API)  */
#define gestaltSoftwareVendorCode 		'srad'				/*  Returns system software vendor information  */
#define gestaltSoftwareVendorApple 		'Appl'				/*  System software sold by Apple  */
#define gestaltSoftwareVendorLicensee 	'Lcns'				/*  System software sold by licensee  */

#define gestaltStandardFileAttr 		'stdf'				/*  Standard File attributes  */
#define gestaltStandardFile58 			0					/*  True if selectors 5-8 (StandardPutFile-CustomGetFile) are supported  */
#define gestaltStandardFileTranslationAware  1				/*  True if standard file is translation manager aware  */
#define gestaltStandardFileHasColorIcons  2					/*  True if standard file has 16x16 color icons  */
#define gestaltStandardFileUseGenericIcons  3				/*  Standard file LDEF to use only the system generic icons if true  */
#define gestaltStandardFileHasDynamicVolumeAllocation  4	/*  True if standard file supports more than 20 volumes  */

#define gestaltSysArchitecture 			'sysa'				/*  Native System Architecture  */
#define gestalt68k 						1					/*  Motorola MC68k architecture  */
#define gestaltPowerPC 					2					/*  IBM PowerPC architecture  */
#define gestaltIntel 					10					/*  Intel x86 architecture  */

#define gestaltSystemUpdateVersion 		'sysu'				/*  System Update version  */
#define gestaltSystemVersion 			'sysv'				/*  system version */
#define gestaltSystemVersionMajor 		'sys1'				/*  The major system version number; in 10.4.17 this would be the decimal value 10  */
#define gestaltSystemVersionMinor 		'sys2'				/*  The minor system version number; in 10.4.17 this would be the decimal value 4  */
#define gestaltSystemVersionBugFix 		'sys3'				/*  The bug fix system version number; in 10.4.17 this would be the decimal value 17  */

#define gestaltToolboxTable 			'tbtt'				/*   OS trap table base   */
#define gestaltTextEditVersion 			'te  '				/*  TextEdit version number  */
#define gestaltTE1 						1					/*  TextEdit in MacIIci ROM  */
#define gestaltTE2 						2					/*  TextEdit with 6.0.4 Script Systems on MacIIci (Script bug fixes for MacIIci)  */
#define gestaltTE3 						3					/*  TextEdit with 6.0.4 Script Systems all but MacIIci  */
#define gestaltTE4 						4					/*  TextEdit in System 7.0  */
#define gestaltTE5 						5					/*  TextWidthHook available in TextEdit  */

#define gestaltTE6 						6					/*  TextEdit with integrated TSMTE and improved UI  */
#define gestaltTEAttr 					'teat'				/*  TextEdit attributes  */
#define gestaltTEHasGetHiliteRgn 		0					/*  TextEdit has TEGetHiliteRgn  */
#define gestaltTESupportsInlineInput 	1					/*  TextEdit does Inline Input  */
#define gestaltTESupportsTextObjects 	2					/*  TextEdit does Text Objects  */
#define gestaltTEHasWhiteBackground 	3					/*  TextEdit supports overriding the TERec's background to white  */

#define gestaltTeleMgrAttr 				'tele'				/*  Telephone manager attributes  */
#define gestaltTeleMgrPresent 			0
#define gestaltTeleMgrPowerPCSupport 	1
#define gestaltTeleMgrSoundStreams 		2
#define gestaltTeleMgrAutoAnswer 		3
#define gestaltTeleMgrIndHandset 		4
#define gestaltTeleMgrSilenceDetect 	5
#define gestaltTeleMgrNewTELNewSupport 	6

#define gestaltTermMgrAttr 				'term'				/*  terminal mgr attributes  */
#define gestaltTermMgrPresent 			0
#define gestaltTermMgrErrorString 		2

#define gestaltThreadMgrAttr 			'thds'				/*  Thread Manager attributes  */
#define gestaltThreadMgrPresent 		0					/*  bit true if Thread Mgr is present  */
#define gestaltSpecificMatchSupport 	1					/*  bit true if Thread Mgr supports exact match creation option  */
#define gestaltThreadsLibraryPresent 	2					/*  bit true if Thread Mgr shared library is present  */

#define gestaltTimeMgrVersion 			'tmgr'				/*  time mgr version  */
#define gestaltStandardTimeMgr 			1					/*  standard time mgr is present  */
#define gestaltRevisedTimeMgr 			2					/*  revised time mgr is present  */
#define gestaltExtendedTimeMgr 			3					/*  extended time mgr is present  */
#define gestaltNativeTimeMgr 			4					/*  PowerPC native TimeMgr is present  */

#define gestaltTSMTEVersion 			'tmTV'
#define gestaltTSMTE1 					0x0100				/*  Original version of TSMTE  */
#define gestaltTSMTE15 					0x0150				/*  System 8.0  */
#define gestaltTSMTE152 				0x0152				/*  System 8.2  */

#define gestaltTSMTEAttr 				'tmTE'
#define gestaltTSMTEPresent 			0
#define gestaltTSMTE 					0					/*  gestaltTSMTE is old name for gestaltTSMTEPresent  */

#define gestaltAVLTreeAttr 				'tree'				/*  AVLTree utility routines attributes.  */
#define gestaltAVLTreePresentBit 		0					/*  if set, then the AVL Tree routines are available.  */
#define gestaltAVLTreeSupportsHandleBasedTreeBit  1			/*  if set, then the AVL Tree routines can store tree data in a single handle  */
#define gestaltAVLTreeSupportsTreeLockingBit  2				/*  if set, the AVLLockTree() and AVLUnlockTree() routines are available.  */

#define gestaltALMAttr 					'trip'				/*  Settings Manager attributes (see also gestaltALMVers)  */
#define gestaltALMPresent 				0					/*  bit true if ALM is available  */
#define gestaltALMHasSFGroup 			1					/*  bit true if Put/Get/Merge Group calls are implmented  */
#define gestaltALMHasCFMSupport 		2					/*  bit true if CFM-based modules are supported  */
#define gestaltALMHasRescanNotifiers 	3					/*  bit true if Rescan notifications/events will be sent to clients  */

#define gestaltALMHasSFLocation 		1
#define gestaltTSMgrVersion 			'tsmv'				/*  Text Services Mgr version, if present  */
#define gestaltTSMgr15 					0x0150
#define gestaltTSMgr20 					0x0200				/*  Version 2.0 as of MacOSX 10.0  */
#define gestaltTSMgr22 					0x0220				/*  Version 2.2 as of MacOSX 10.3  */
#define gestaltTSMgr23 					0x0230				/*  Version 2.3 as of MacOSX 10.4  */

#define gestaltTSMgrAttr 				'tsma'				/*  Text Services Mgr attributes, if present  */
#define gestaltTSMDisplayMgrAwareBit 	0					/*  TSM knows about display manager  */
#define gestaltTSMdoesTSMTEBit 			1					/*  TSM has integrated TSMTE  */

#define gestaltSpeechAttr 				'ttsc'				/*  Speech Manager attributes  */
#define gestaltSpeechMgrPresent 		0					/*  bit set indicates that Speech Manager exists  */
#define gestaltSpeechHasPPCGlue 		1					/*  bit set indicates that native PPC glue for Speech Manager API exists  */

#define gestaltTVAttr 					'tv  '				/*  TV version  */
#define gestaltHasTVTuner 				0					/*  supports Philips FL1236F video tuner  */
#define gestaltHasSoundFader 			1					/*  supports Philips TEA6330 Sound Fader chip  */
#define gestaltHasHWClosedCaptioning 	2					/*  supports Philips SAA5252 Closed Captioning  */
#define gestaltHasIRRemote 				3					/*  supports CyclopsII Infra Red Remote control  */
#define gestaltHasVidDecoderScaler 		4					/*  supports Philips SAA7194 Video Decoder/Scaler  */
#define gestaltHasStereoDecoder 		5					/*  supports Sony SBX1637A-01 stereo decoder  */
#define gestaltHasSerialFader 			6					/*  has fader audio in serial with system audio  */
#define gestaltHasFMTuner 				7					/*  has FM Tuner from donnybrook card  */
#define gestaltHasSystemIRFunction 		8					/*  Infra Red button function is set up by system and not by Video Startup  */
#define gestaltIRDisabled 				9					/*  Infra Red remote is not disabled.  */
#define gestaltINeedIRPowerOffConfirm 	10					/*  Need IR power off confirm dialog.  */
#define gestaltHasZoomedVideo 			11					/*  Has Zoomed Video PC Card video input.  */

#define gestaltATSUVersion 				'uisv'
#define gestaltOriginalATSUVersion 		0x00010000			/*  ATSUI version 1.0  */
#define gestaltATSUUpdate1 				0x00020000			/*  ATSUI version 1.1  */
#define gestaltATSUUpdate2 				0x00030000			/*  ATSUI version 1.2  */
#define gestaltATSUUpdate3 				0x00040000			/*  ATSUI version 2.0  */
#define gestaltATSUUpdate4 				0x00050000			/*  ATSUI version in Mac OS X - SoftwareUpdate 1-4 for Mac OS 10.0.1 - 10.0.4  */
#define gestaltATSUUpdate5 				0x00060000			/*  ATSUI version 2.3 in MacOS 10.1  */
#define gestaltATSUUpdate6 				0x00070000			/*  ATSUI version 2.4 in MacOS 10.2  */
#define gestaltATSUUpdate7 				0x00080000			/*  ATSUI version 2.5 in MacOS 10.3  */

#define gestaltATSUFeatures 			'uisf'
#define gestaltATSUTrackingFeature 		0x00000001			/*  feature introduced in ATSUI version 1.1  */
#define gestaltATSUMemoryFeature 		0x00000001			/*  feature introduced in ATSUI version 1.1  */
#define gestaltATSUFallbacksFeature 	0x00000001			/*  feature introduced in ATSUI version 1.1  */
#define gestaltATSUGlyphBoundsFeature 	0x00000001			/*  feature introduced in ATSUI version 1.1  */
#define gestaltATSULineControlFeature 	0x00000001			/*  feature introduced in ATSUI version 1.1  */
#define gestaltATSULayoutCreateAndCopyFeature  0x00000001	/*  feature introduced in ATSUI version 1.1  */
#define gestaltATSULayoutCacheClearFeature  0x00000001		/*  feature introduced in ATSUI version 1.1  */
#define gestaltATSUTextLocatorUsageFeature  0x00000002		/*  feature introduced in ATSUI version 1.2  */
#define gestaltATSULowLevelOrigFeatures  0x00000004			/*  first low-level features introduced in ATSUI version 2.0  */
#define gestaltATSUFallbacksObjFeatures  0x00000008			/*  feature introduced - ATSUFontFallbacks objects introduced in ATSUI version 2.3  */
#define gestaltATSUIgnoreLeadingFeature  0x00000008			/*  feature introduced - kATSLineIgnoreFontLeading LineLayoutOption introduced in ATSUI version 2.3  */
#define gestaltATSUByCharacterClusterFeature  0x00000010	/*  ATSUCursorMovementTypes introduced in ATSUI version 2.4  */
#define gestaltATSUAscentDescentControlsFeature  0x00000010	/*  attributes introduced in ATSUI version 2.4  */
#define gestaltATSUHighlightInactiveTextFeature  0x00000010	/*  feature introduced in ATSUI version 2.4  */
#define gestaltATSUPositionToCursorFeature  0x00000010		/*  features introduced in ATSUI version 2.4  */
#define gestaltATSUBatchBreakLinesFeature  0x00000010		/*  feature introduced in ATSUI version 2.4  */
#define gestaltATSUTabSupportFeature 	0x00000010			/*  features introduced in ATSUI version 2.4  */
#define gestaltATSUDirectAccess 		0x00000010			/*  features introduced in ATSUI version 2.4  */
#define gestaltATSUDecimalTabFeature 	0x00000020			/*  feature introduced in ATSUI version 2.5  */
#define gestaltATSUBiDiCursorPositionFeature  0x00000020	/*  feature introduced in ATSUI version 2.5  */
#define gestaltATSUNearestCharLineBreakFeature  0x00000020	/*  feature introduced in ATSUI version 2.5  */
#define gestaltATSUHighlightColorControlFeature  0x00000020	/*  feature introduced in ATSUI version 2.5  */
#define gestaltATSUUnderlineOptionsStyleFeature  0x00000020	/*  feature introduced in ATSUI version 2.5  */
#define gestaltATSUStrikeThroughStyleFeature  0x00000020	/*  feature introduced in ATSUI version 2.5  */
#define gestaltATSUDropShadowStyleFeature  0x00000020		/*  feature introduced in ATSUI version 2.5  */

#define gestaltUSBAttr 					'usb '				/*  USB Attributes  */
#define gestaltUSBPresent 				0					/*  USB Support available  */
#define gestaltUSBHasIsoch 				1					/*  USB Isochronous features available  */

#define gestaltUSBVersion 				'usbv'				/*  USB version  */
#define gestaltVersion 					'vers'				/*  gestalt version  */
#define gestaltValueImplementedVers 	5					/*  version of gestalt where gestaltValue is implemented.  */

#define gestaltVIA1Addr 				'via1'				/*  via 1 base address   */
#define gestaltVIA2Addr 				'via2'				/*  via 2 base address   */
#define gestaltVMAttr 					'vm  '				/*  virtual memory attributes  */
#define gestaltVMPresent 				0					/*  true if virtual memory is present  */
#define gestaltVMHasLockMemoryForOutput  1					/*  true if LockMemoryForOutput is available  */
#define gestaltVMFilemappingOn 			3					/*  true if filemapping is available  */
#define gestaltVMHasPagingControl 		4					/*  true if MakeMemoryResident, MakeMemoryNonResident, FlushMemory, and ReleaseMemoryData are available  */

#define gestaltVMInfoType 				'vmin'				/*  Indicates how the Finder should display information about VM in  */
															/*  the Finder about box.  */
#define gestaltVMInfoSizeStorageType 	0					/*  Display VM on/off, backing store size and name  */
#define gestaltVMInfoSizeType 			1					/*  Display whether VM is on or off and the size of the backing store  */
#define gestaltVMInfoSimpleType 		2					/*  Display whether VM is on or off  */
#define gestaltVMInfoNoneType 			3					/*  Display no VM information  */

#define gestaltVMBackingStoreFileRefNum  'vmbs'				/*  file refNum of virtual memory's main backing store file (returned in low word of result)  */
#define gestaltALMVers 					'walk'				/*  Settings Manager version (see also gestaltALMAttr)  */
#define gestaltWindowMgrAttr 			'wind'				/*  If this Gestalt exists, the Mac OS 8.5 Window Manager is installed */
#define gestaltWindowMgrPresent 		0x00000001			/*  NOTE: this is a bit mask, whereas all other Gestalt constants of */
															/*  this type are bit index values.   Universal Interfaces 3.2 slipped */
															/*  out the door with this mistake. */
#define gestaltWindowMgrPresentBit 		0					/*  bit number */
#define gestaltExtendedWindowAttributes  1					/*  Has ChangeWindowAttributes; GetWindowAttributes works for all windows */
#define gestaltExtendedWindowAttributesBit  1				/*  Has ChangeWindowAttributes; GetWindowAttributes works for all windows */
#define gestaltHasFloatingWindows 		2					/*  Floating window APIs work */
#define gestaltHasFloatingWindowsBit 	2					/*  Floating window APIs work */
#define gestaltHasWindowBuffering 		3					/*  This system has buffering available */
#define gestaltHasWindowBufferingBit 	3					/*  This system has buffering available */
#define gestaltWindowLiveResizeBit 		4					/*  live resizing of windows is available */
#define gestaltWindowMinimizeToDockBit 	5					/*  windows minimize to the dock and do not windowshade (Mac OS X) */
#define gestaltHasWindowShadowsBit 		6					/*  windows have shadows */
#define gestaltSheetsAreWindowModalBit 	7					/*  sheet windows are modal only to their parent window */
#define gestaltFrontWindowMayBeHiddenBit  8					/*  FrontWindow and related APIs will return the front window even when the app is hidden */
															/*  masks for the above bits */
#define gestaltWindowMgrPresentMask 	0x00000001
#define gestaltExtendedWindowAttributesMask  0x00000002
#define gestaltHasFloatingWindowsMask 	0x00000004
#define gestaltHasWindowBufferingMask 	0x00000008
#define gestaltWindowLiveResizeMask 	0x00000010
#define gestaltWindowMinimizeToDockMask  0x00000020
#define gestaltHasWindowShadowsMask 	0x00000040
#define gestaltSheetsAreWindowModalMask  0x00000080
#define gestaltFrontWindowMayBeHiddenMask  0x00000100

#define gestaltHasSingleWindowModeBit 	8					/*  This system supports single window mode */
#define gestaltHasSingleWindowModeMask 	0x00000100

#define gestaltX86Features 				'x86f'
#define gestaltX86HasFPU 				0					/*  has an FPU that supports the 387 instructions */
#define gestaltX86HasVME 				1					/*  supports Virtual-8086 Mode Extensions */
#define gestaltX86HasDE 				2					/*  supports I/O breakpoints (Debug Extensions) */
#define gestaltX86HasPSE 				3					/*  supports 4-Mbyte pages (Page Size Extension) */
#define gestaltX86HasTSC 				4					/*  supports RTDSC instruction (Time Stamp Counter) */
#define gestaltX86HasMSR 				5					/*  supports Model Specific Registers */
#define gestaltX86HasPAE 				6					/*  supports physical addresses > 32 bits (Physical Address Extension) */
#define gestaltX86HasMCE 				7					/*  supports Machine Check Exception */
#define gestaltX86HasCX8 				8					/*  supports CMPXCHG8 instructions (Compare Exchange 8 bytes) */
#define gestaltX86HasAPIC 				9					/*  contains local APIC */
#define gestaltX86HasSEP 				11					/*  supports fast system call (SysEnter Present) */
#define gestaltX86HasMTRR 				12					/*  supports Memory Type Range Registers */
#define gestaltX86HasPGE 				13					/*  supports Page Global Enable */
#define gestaltX86HasMCA 				14					/*  supports Machine Check Architecture */
#define gestaltX86HasCMOV 				15					/*  supports CMOVcc instruction (Conditional Move). */
															/*  If FPU bit is also set, supports FCMOVcc and FCOMI, too */
#define gestaltX86HasPAT 				16					/*  supports Page Attribute Table */
#define gestaltX86HasPSE36 				17					/*  supports 36-bit Page Size Extension */
#define gestaltX86HasPSN 				18					/*  Processor Serial Number */
#define gestaltX86HasCLFSH 				19					/*  CLFLUSH Instruction supported */
#define gestaltX86Serviced20 			20					/*  do not count on this bit value */
#define gestaltX86HasDS 				21					/*  Debug Store */
#define gestaltX86ResACPI 				22					/*  Thermal Monitor, SW-controlled clock */
#define gestaltX86HasMMX 				23					/*  supports MMX instructions */
#define gestaltX86HasFXSR 				24					/*  Supports FXSAVE and FXRSTOR instructions (fast FP save/restore) */
#define gestaltX86HasSSE 				25					/*  Streaming SIMD extensions */
#define gestaltX86HasSSE2 				26					/*  Streaming SIMD extensions 2 */
#define gestaltX86HasSS 				27					/*  Self-Snoop */
#define gestaltX86HasHTT 				28					/*  Hyper-Threading Technology */
#define gestaltX86HasTM 				29					/*  Thermal Monitor */

#define gestaltX86AdditionalFeatures 	'x86a'
#define gestaltX86HasSSE3 				0					/*  Prescott New Inst. */
#define gestaltX86HasMONITOR 			3					/*  Monitor/mwait */
#define gestaltX86HasDSCPL 				4					/*  Debug Store CPL */
#define gestaltX86HasVMX 				5					/*  VMX */
#define gestaltX86HasSMX 				6					/*  SMX */
#define gestaltX86HasEST 				7					/*  Enhanced SpeedsTep (GV3) */
#define gestaltX86HasTM2 				8					/*  Thermal Monitor 2 */
#define gestaltX86HasSupplementalSSE3 	9					/*  Supplemental SSE3 instructions */
#define gestaltX86HasCID 				10					/*  L1 Context ID */
#define gestaltX86HasCX16 				13					/*  CmpXchg16b instruction */
#define gestaltX86HasxTPR 				14					/*  Send Task PRiority msgs */

#define gestaltTranslationAttr 			'xlat'				/*  Translation Manager attributes  */
#define gestaltTranslationMgrExists 	0					/*  True if translation manager exists  */
#define gestaltTranslationMgrHintOrder 	1					/*  True if hint order reversal in effect  */
#define gestaltTranslationPPCAvail 		2
#define gestaltTranslationGetPathAPIAvail  3

#define gestaltExtToolboxTable 			'xttt'				/*  Extended Toolbox trap table base  */
#define gestaltUSBPrinterSharingVersion  'zak '				/*  USB Printer Sharing Version */
#define gestaltUSBPrinterSharingVersionMask  0x0000FFFF		/*  mask for bits in version */
#define gestaltUSBPrinterSharingAttr 	'zak '				/*  USB Printer Sharing Attributes */
#define gestaltUSBPrinterSharingAttrMask  0xFFFF0000		/*   mask for attribute bits */
#define gestaltUSBPrinterSharingAttrRunning  0x80000000		/*  printer sharing is running */
#define gestaltUSBPrinterSharingAttrBooted  0x40000000		/*  printer sharing was installed at boot time */

#define gestaltWorldScriptIIVersion 	'doub'
#define gestaltWorldScriptIIAttr 		'wsat'
#define gestaltWSIICanPrintWithoutPrGeneralBit  0			/*  bit 0 is on if WS II knows about PrinterStatus callback  */


#endif /* __GESTALT_R__ */

