/*
     File:       Folders.r
 
     Contains:   Folder Manager Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1995-2011 by Apple Inc. All rights reserved.
*/

#ifndef __FOLDERS_R__
#define __FOLDERS_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define kOnSystemDisk 					(-32768)			/*  previously was 0x8000 but that is an unsigned value whereas vRefNum is signed */
#define kOnAppropriateDisk 				(-32767)			/*  Generally, the same as kOnSystemDisk, but it's clearer that this isn't always the 'boot' disk. */
															/*  Folder Domains - Carbon only.  The constants above can continue to be used, but the folder/volume returned will */
															/*  be from one of the domains below. */
#define kSystemDomain 					(-32766)			/*  Read-only system hierarchy. */
#define kLocalDomain 					(-32765)			/*  All users of a single machine have access to these resources. */
#define kNetworkDomain 					(-32764)			/*  All users configured to use a common network server has access to these resources. */
#define kUserDomain 					(-32763)			/*  Read/write. Resources that are private to the user. */
#define kClassicDomain 					(-32762)			/*  Domain referring to the currently configured Classic System Folder.  Not supported in Mac OS X Leopard and later. */
#define kFolderManagerLastDomain 		(-32760)

#define kLastDomainConstant 			(-32760)
#define kCreateFolder 					1
#define kDontCreateFolder 				0

#define kDesktopFolderType 				'desk'				/*  the desktop folder; objects in this folder show on the desktop.  */
#define kTrashFolderType 				'trsh'				/*  the trash folder; objects in this folder show up in the trash  */
#define kWhereToEmptyTrashFolderType 	'empt'				/*  the "empty trash" folder; Finder starts empty from here down  */
#define kFontsFolderType 				'font'				/*  Fonts go here  */
#define kPreferencesFolderType 			'pref'				/*  preferences for applications go here  */
#define kSystemPreferencesFolderType 	'sprf'				/*  the PreferencePanes folder, where Mac OS X Preference Panes go  */
#define kTemporaryFolderType 			'temp'				/*     On Mac OS X, each user has their own temporary items folder, and the Folder Manager attempts to set permissions of these */
															/*     folders such that other users can not access the data inside.  On Mac OS X 10.4 and later the data inside the temporary */
															/*     items folder is deleted at logout and at boot, but not otherwise.  Earlier version of Mac OS X would delete items inside */
															/*     the temporary items folder after a period of inaccess.  You can ask for a temporary item in a specific domain or on a  */
															/*     particular volume by FSVolumeRefNum.  If you want a location for temporary items for a short time, then use either */
															/*     ( kUserDomain, kkTemporaryFolderType ) or ( kSystemDomain, kTemporaryFolderType ).  The kUserDomain varient will always be */
															/*     on the same volume as the user's home folder, while the kSystemDomain version will be on the same volume as /var/tmp/ ( and */
															/*     will probably be on the local hard drive in case the user's home is a network volume ).  If you want a location for a temporary */
															/*     file or folder to use for saving a document, especially if you want to use FSpExchangeFile() to implement a safe-save, then */
															/*     ask for the temporary items folder on the same volume as the file you are safe saving. */
															/*     However, be prepared for a failure to find a temporary folder in any domain or on any volume.  Some volumes may not have */
															/*     a location for a temporary folder, or the permissions of the volume may be such that the Folder Manager can not return */
															/*     a temporary folder for the volume. */
															/*     If your application creates an item in a temporary items older you should delete that item as soon as it is not needed, */
															/*     and certainly before your application exits, since otherwise the item is consuming disk space until the user logs out or */
															/*     restarts.  Any items left inside a temporary items folder should be moved into a folder inside the Trash folder on the disk */
															/*     when the user logs in, inside a folder named "Recovered items", in case there is anything useful to the end user. */
#define kChewableItemsFolderType 		'flnt'				/*  similar to kTemporaryItemsFolderType, except items in this folder are deleted at boot or when the disk is unmounted  */
#define kTemporaryItemsInCacheDataFolderType  'vtmp'		/*  A folder inside the kCachedDataFolderType for the given domain which can be used for transient data */
#define kApplicationsFolderType 		'apps'				/*     Applications on Mac OS X are typically put in this folder ( or a subfolder ). */
#define kVolumeRootFolderType 			'root'				/*  root folder of a volume or domain  */
#define kDomainTopLevelFolderType 		'dtop'				/*  The top-level of a Folder domain, e.g. "/System" */
#define kDomainLibraryFolderType 		'dlib'				/*  the Library subfolder of a particular domain */
#define kUsersFolderType 				'usrs'				/*  "Users" folder, usually contains one folder for each user.  */
#define kCurrentUserFolderType 			'cusr'				/*  The folder for the currently logged on user; domain passed in is ignored.  */
#define kSharedUserDataFolderType 		'sdat'				/*  A Shared folder, readable & writeable by all users  */

#define kDocumentsFolderType 			'docs'				/*     User documents are typically put in this folder ( or a subfolder ). */
#define kPictureDocumentsFolderType 	'pdoc'				/*  Refers to the "Pictures" folder in a users home directory */
#define kMovieDocumentsFolderType 		'mdoc'				/*  Refers to the "Movies" folder in a users home directory */
#define kMusicDocumentsFolderType 		'µdoc'				/*  Refers to the "Music" folder in a users home directory */
#define kInternetSitesFolderType 		'site'				/*  Refers to the "Sites" folder in a users home directory */
#define kPublicFolderType 				'pubb'				/*  Refers to the "Public" folder in a users home directory */

#define kDropBoxFolderType 				'drop'				/*  Refers to the "Drop Box" folder inside the user's home directory */
#define kSharedLibrariesFolderType 		'Älib'				/*  for general shared libs.  */
#define kVoicesFolderType 				'fvoc'				/*  macintalk can live here  */
#define kUtilitiesFolderType 			'utiÄ'				/*  for Utilities folder  */
#define kThemesFolderType 				'thme'				/*  for Theme data files  */
#define kFavoritesFolderType 			'favs'				/*  Favorties folder for Navigation Services  */
#define kInternetSearchSitesFolderType 	'issf'				/*  Internet Search Sites folder  */
#define kInstallerLogsFolderType 		'ilgf'				/*  Installer Logs folder  */
#define kScriptsFolderType 				'scrÄ'				/*  Scripts folder  */
#define kFolderActionsFolderType 		'fasf'				/*  Folder Actions Scripts folder  */
#define kSpeakableItemsFolderType 		'spki'				/*  Speakable Items folder  */
#define kKeychainFolderType 			'kchn'				/*  Keychain folder  */

#define kColorSyncFolderType 			'sync'				/*  Contains ColorSync-related folders */
#define kColorSyncCMMFolderType 		'ccmm'				/*  ColorSync CMMs */
#define kColorSyncScriptingFolderType 	'cscr'				/*  ColorSync Scripting support */
#define kPrintersFolderType 			'impr'				/*  Contains Printing-related folders */
#define kSpeechFolderType 				'spch'				/*  Contains Speech-related folders */
#define kCarbonLibraryFolderType 		'carb'				/*  Contains Carbon-specific file */
#define kDocumentationFolderType 		'info'				/*  Contains Documentation files (not user documents) */
#define kISSDownloadsFolderType 		'issd'				/*  Contains Internet Search Sites downloaded from the Internet */
#define kUserSpecificTmpFolderType 		'utmp'				/*  Contains temporary items created on behalf of the current user */
#define kCachedDataFolderType 			'cach'				/*  Contains various cache files for different clients */
#define kFrameworksFolderType 			'fram'				/*  Contains MacOS X Framework folders */
#define kPrivateFrameworksFolderType 	'pfrm'				/*  Contains MacOS X Private Framework folders      */
#define kClassicDesktopFolderType 		'sdsk'				/*  MacOS 9 compatible desktop folder - same as kSystemDesktopFolderType but with a more appropriate name for Mac OS X code. */
#define kSystemSoundsFolderType 		'ssnd'				/*  Contains Mac OS X System Sound Files ( valid in kSystemDomain, kLocalDomain, and kUserDomain ) */
#define kComponentsFolderType 			'cmpd'				/*  Contains Mac OS X components   ( valid in kSystemDomain, kLocalDomain, and kUserDomain ) */
#define kQuickTimeComponentsFolderType 	'wcmp'				/*  Contains QuickTime components for Mac OS X ( valid in kSystemDomain, kLocalDomain, and kUserDomain ) */
#define kCoreServicesFolderType 		'csrv'				/*  Refers to the "/System/Library/CoreServices" folder on Mac OS X */
#define kAudioSupportFolderType 		'adio'				/*  Refers to the Audio support folder for Mac OS X */
#define kAudioPresetsFolderType 		'apst'				/*  "Presets" folder of "Audio" folder, Mac OS X 10.4 and later */
#define kAudioSoundsFolderType 			'asnd'				/*  Refers to the Sounds subfolder of Audio Support */
#define kAudioSoundBanksFolderType 		'bank'				/*  Refers to the Banks subfolder of the Sounds Folder */
#define kAudioAlertSoundsFolderType 	'alrt'				/*  Refers to the Alert Sounds subfolder of the Sound Folder */
#define kAudioPlugInsFolderType 		'aplg'				/*  Refers to the Plug-ins subfolder of the Audio Folder    */
#define kAudioComponentsFolderType 		'acmp'				/*  Refers to the Components subfolder of the Audio Plug-ins Folder     */
#define kKernelExtensionsFolderType 	'kext'				/*  Refers to the Kernel Extensions Folder on Mac OS X */
#define kDirectoryServicesFolderType 	'dsrv'				/*  Refers to the Directory Services folder on Mac OS X */
#define kDirectoryServicesPlugInsFolderType  'dplg'			/*  Refers to the Directory Services Plug-Ins folder on Mac OS X  */
#define kInstallerReceiptsFolderType 	'rcpt'				/*  Refers to the "Receipts" folder in Mac OS X */
#define kFileSystemSupportFolderType 	'fsys'				/*  Refers to the [domain]/Library/Filesystems folder in Mac OS X */
#define kAppleShareSupportFolderType 	'shar'				/*  Refers to the [domain]/Library/Filesystems/AppleShare folder in Mac OS X */
#define kAppleShareAuthenticationFolderType  'auth'			/*  Refers to the [domain]/Library/Filesystems/AppleShare/Authentication folder in Mac OS X */
#define kMIDIDriversFolderType 			'midi'				/*  Refers to the MIDI Drivers folder on Mac OS X */
#define kKeyboardLayoutsFolderType 		'klay'				/*  Refers to the [domain]/Library/KeyboardLayouts folder in Mac OS X */
#define kIndexFilesFolderType 			'indx'				/*  Refers to the [domain]/Library/Indexes folder in Mac OS X */
#define kFindByContentIndexesFolderType  'fbcx'				/*  Refers to the [domain]/Library/Indexes/FindByContent folder in Mac OS X */
#define kManagedItemsFolderType 		'mang'				/*  Refers to the Managed Items folder for Mac OS X  */
#define kBootTimeStartupItemsFolderType  'empz'				/*  Refers to the "StartupItems" folder of Mac OS X  */
#define kAutomatorWorkflowsFolderType 	'flow'				/*  Automator Workflows folder  */
#define kAutosaveInformationFolderType 	'asav'				/*  ~/Library/Autosaved Information/ folder, can be used to store autosave information for user's applications.  Available in Mac OS X 10.4 and later.   */
#define kSpotlightSavedSearchesFolderType  'spot'			/*  Usually ~/Library/Saved Searches/; used by Finder and Nav/Cocoa panels to find saved Spotlight searches  */
															/*  The following folder types are available in Mac OS X 10.5 and later  */
#define kSpotlightImportersFolderType 	'simp'				/*  Folder for Spotlight importers, usually /Library/Spotlight/ or ~/Library/Spotlight, etc.  */
#define kSpotlightMetadataCacheFolderType  'scch'			/*  Folder for Spotlight metadata caches, for example: ~/Library/Caches/Metadata/  */
#define kInputManagersFolderType 		'inpt'				/*  InputManagers  */
#define kInputMethodsFolderType 		'inpf'				/*  ../Library/Input Methods/  */
#define kLibraryAssistantsFolderType 	'astl'				/*  Refers to the [domain]/Library/Assistants folder */
#define kAudioDigidesignFolderType 		'adig'				/*  Refers to the Digidesign subfolder of the Audio Plug-ins folder */
#define kAudioVSTFolderType 			'avst'				/*  Refers to the VST subfolder of the Audio Plug-ins folder */
#define kColorPickersFolderType 		'cpkr'				/*  Refers to the ColorPickers folder */
#define kCompositionsFolderType 		'cmps'				/*  Refers to the Compositions folder */
#define kFontCollectionsFolderType 		'fncl'				/*  Refers to the FontCollections folder */
#define kiMovieFolderType 				'imov'				/*  Refers to the iMovie folder */
#define kiMoviePlugInsFolderType 		'impi'				/*  Refers to the Plug-ins subfolder of the iMovie Folder */
#define kiMovieSoundEffectsFolderType 	'imse'				/*  Refers to the Sound Effects subfolder of the iMovie Folder */
#define kDownloadsFolderType 			'down'				/*  Refers to the ~/Downloads folder */

#define kColorSyncProfilesFolderType 	'prof'				/*  for ColorSyncª Profiles  */
#define kApplicationSupportFolderType 	'asup'				/*  third-party items and folders  */
#define kTextEncodingsFolderType 		'Ätex'				/*  encoding tables  */
#define kPrinterDescriptionFolderType 	'ppdf'				/*  new folder at root of System folder for printer descs.  */
#define kPrinterDriverFolderType 		'Äprd'				/*  new folder at root of System folder for printer drivers  */
#define kScriptingAdditionsFolderType 	'Äscr'				/*  at root of system folder  */

#define kClassicPreferencesFolderType 	'cprf'				/*  "Classic" folder in ~/Library/ for redirected preference files.  */
#define kQuickLookFolderType 			'qlck'				/*  The QuickLook folder, supported in Mac OS X 10.6 and later.  */
#define kServicesFolderType 			'svcs'				/*  The services folder, supported in Mac OS X 10.7 and later.  */
															/*     The following selectors really only make sense when used within the Classic environment on Mac OS X. */
#define kSystemFolderType 				'macs'				/*  the system folder  */
#define kSystemDesktopFolderType 		'sdsk'				/*  the desktop folder at the root of the hard drive, never the redirected user desktop folder  */
#define kSystemTrashFolderType 			'strs'				/*  the trash folder at the root of the drive, never the redirected user trash folder  */
#define kPrintMonitorDocsFolderType 	'prnt'				/*  Print Monitor documents  */
#define kALMModulesFolderType 			'walk'				/*  for Location Manager Module files except type 'thng' (within kExtensionFolderType)  */
#define kALMPreferencesFolderType 		'trip'				/*  for Location Manager Preferences (within kPreferencesFolderType; contains kALMLocationsFolderType)  */
#define kALMLocationsFolderType 		'fall'				/*  for Location Manager Locations (within kALMPreferencesFolderType)  */
#define kAppleExtrasFolderType 			'aexÄ'				/*  for Apple Extras folder  */
#define kContextualMenuItemsFolderType 	'cmnu'				/*  for Contextual Menu items  */
#define kMacOSReadMesFolderType 		'morÄ'				/*  for MacOS ReadMes folder  */
#define kStartupFolderType 				'strt'				/*  Finder objects (applications, documents, DAs, aliases, to...) to open at startup go here  */
#define kShutdownFolderType 			'shdf'				/*  Finder objects (applications, documents, DAs, aliases, to...) to open at shutdown go here  */
#define kAppleMenuFolderType 			'amnu'				/*  Finder objects to put into the Apple menu go here  */
#define kControlPanelFolderType 		'ctrl'				/*  Control Panels go here (may contain INITs)  */
#define kSystemControlPanelFolderType 	'sctl'				/*  System control panels folder - never the redirected one, always "Control Panels" inside the System Folder  */
#define kExtensionFolderType 			'extn'				/*  System extensions go here  */
#define kExtensionDisabledFolderType 	'extD'
#define kControlPanelDisabledFolderType  'ctrD'
#define kSystemExtensionDisabledFolderType  'macD'
#define kStartupItemsDisabledFolderType  'strD'
#define kShutdownItemsDisabledFolderType  'shdD'
#define kAssistantsFolderType 			'astÄ'				/*  for Assistants (MacOS Setup Assistant, etc)  */
#define kStationeryFolderType 			'odst'				/*  stationery  */
#define kOpenDocFolderType 				'odod'				/*  OpenDoc root  */
#define kOpenDocShellPlugInsFolderType 	'odsp'				/*  OpenDoc Shell Plug-Ins in OpenDoc folder  */
#define kEditorsFolderType 				'oded'				/*  OpenDoc editors in MacOS Folder  */
#define kOpenDocEditorsFolderType 		'Äodf'				/*  OpenDoc subfolder of Editors folder  */
#define kOpenDocLibrariesFolderType 	'odlb'				/*  OpenDoc libraries folder  */
#define kGenEditorsFolderType 			'Äedi'				/*  CKH general editors folder at root level of Sys folder  */
#define kHelpFolderType 				'Ählp'				/*  CKH help folder currently at root of system folder  */
#define kInternetPlugInFolderType 		'Änet'				/*  CKH internet plug ins for browsers and stuff  */
#define kModemScriptsFolderType 		'Ämod'				/*  CKH modem scripts, get 'em OUT of the Extensions folder  */
#define kControlStripModulesFolderType 	'sdev'				/*  CKH for control strip modules  */
#define kInternetFolderType 			'intÄ'				/*  Internet folder (root level of startup volume)  */
#define kAppearanceFolderType 			'appr'				/*  Appearance folder (root of system folder)  */
#define kSoundSetsFolderType 			'snds'				/*  Sound Sets folder (in Appearance folder)  */
#define kDesktopPicturesFolderType 		'dtpÄ'				/*  Desktop Pictures folder (in Appearance folder)  */
#define kFindSupportFolderType 			'fnds'				/*  Find support folder  */
#define kRecentApplicationsFolderType 	'rapp'				/*  Recent Applications folder  */
#define kRecentDocumentsFolderType 		'rdoc'				/*  Recent Documents folder  */
#define kRecentServersFolderType 		'rsvr'				/*  Recent Servers folder  */
#define kLauncherItemsFolderType 		'laun'				/*  Launcher Items folder  */
#define kQuickTimeExtensionsFolderType 	'qtex'				/*  QuickTime Extensions Folder (in Extensions folder)  */
#define kDisplayExtensionsFolderType 	'dspl'				/*  Display Extensions Folder (in Extensions folder)  */
#define kMultiprocessingFolderType 		'mpxf'				/*  Multiprocessing Folder (in Extensions folder)  */
#define kPrintingPlugInsFolderType 		'pplg'				/*  Printing Plug-Ins Folder (in Extensions folder)  */
#define kAppleshareAutomountServerAliasesFolderType  'srvÄ'	/*  Appleshare puts volumes to automount inside this folder.  */
#define kVolumeSettingsFolderType 		'vsfd'				/*  Volume specific user information goes here  */
#define kPreMacOS91ApplicationsFolderType  'Œpps'			/*  The "Applications" folder, pre Mac OS 9.1  */
#define kPreMacOS91InstallerLogsFolderType  '”lgf'			/*  The "Installer Logs" folder, pre Mac OS 9.1  */
#define kPreMacOS91AssistantsFolderType  'ŒstÄ'				/*  The "Assistants" folder, pre Mac OS 9.1  */
#define kPreMacOS91UtilitiesFolderType 	'ŸtiÄ'				/*  The "Utilities" folder, pre Mac OS 9.1  */
#define kPreMacOS91AppleExtrasFolderType  'ŒexÄ'			/*  The "Apple Extras" folder, pre Mac OS 9.1  */
#define kPreMacOS91MacOSReadMesFolderType  'µorÄ'			/*  The "Mac OS ReadMes" folder, pre Mac OS 9.1  */
#define kPreMacOS91InternetFolderType 	'”ntÄ'				/*  The "Internet" folder, pre Mac OS 9.1  */
#define kPreMacOS91AutomountedServersFolderType  '§rvÄ'		/*  The "Servers" folder, pre Mac OS 9.1  */
#define kPreMacOS91StationeryFolderType  '¿dst'				/*  The "Stationery" folder, pre Mac OS 9.1  */
#define kLocalesFolderType 				'Äloc'				/*  PKE for Locales folder  */
#define kFindByContentPluginsFolderType  'fbcp'				/*  Find By Content Plug-ins  */
#define kFindByContentFolderType 		'fbcf'				/*  Find by content folder  */

#define kMagicTemporaryItemsFolderType 	'mtmp'
#define kTemporaryItemsInUserDomainFolderType  'temq'
#define kCurrentUserRemoteFolderLocation  'rusf'			/*  The remote folder for the currently logged on user  */
#define kCurrentUserRemoteFolderType 	'rusr'				/*  The remote folder location for the currently logged on user  */

#define kDeveloperDocsFolderType 		'ddoc'				/*  Deprecated in 10.5. Contains Developer Documentation files and folders */
#define kDeveloperHelpFolderType 		'devh'				/*  Deprecated in 10.5. Contains Developer Help related files */
#define kDeveloperFolderType 			'devf'				/*  Deprecated in 10.5. Contains MacOS X Developer Resources */
#define kDeveloperApplicationsFolderType  'dapp'			/*  Deprecated in 10.5. Contains Developer Applications */

#define kCreateFolderAtBoot 			0x00000002
#define kCreateFolderAtBootBit 			1
#define kFolderCreatedInvisible 		0x00000004
#define kFolderCreatedInvisibleBit 		2
#define kFolderCreatedNameLocked 		0x00000008
#define kFolderCreatedNameLockedBit 	3
#define kFolderCreatedAdminPrivs 		0x00000010
#define kFolderCreatedAdminPrivsBit 	4

#define kFolderInUserFolder 			0x00000020
#define kFolderInUserFolderBit 			5
#define kFolderTrackedByAlias 			0x00000040
#define kFolderTrackedByAliasBit 		6
#define kFolderInRemoteUserFolderIfAvailable  0x00000080
#define kFolderInRemoteUserFolderIfAvailableBit  7
#define kFolderNeverMatchedInIdentifyFolder  0x00000100
#define kFolderNeverMatchedInIdentifyFolderBit  8
#define kFolderMustStayOnSameVolume 	0x00000200
#define kFolderMustStayOnSameVolumeBit 	9
#define kFolderManagerFolderInMacOS9FolderIfMacOSXIsInstalledMask  0x00000400
#define kFolderManagerFolderInMacOS9FolderIfMacOSXIsInstalledBit  10
#define kFolderInLocalOrRemoteUserFolder  0x000000A0
#define kFolderManagerNotCreatedOnRemoteVolumesBit  11
#define kFolderManagerNotCreatedOnRemoteVolumesMask  0x0800
#define kFolderManagerNewlyCreatedFolderIsLocalizedBit  12
#define kFolderManagerNewlyCreatedFolderShouldHaveDotLocalizedCreatedWithinMask  0x1000

#define kRelativeFolder 				'relf'
#define kRedirectedRelativeFolder 		'rrel'
#define kSpecialFolder 					'spcf'

#define kBlessedFolder 					'blsf'
#define kRootFolder 					'rotf'

#define kCurrentUserFolderLocation 		'cusf'				/*     the magic 'Current User' folder location */
#define kDictionariesFolderType 		'dict'				/*  Dictionaries folder  */
#define kLogsFolderType 				'logs'				/*  Logs folder  */
#define kPreferencePanesFolderType 		'ppan'				/*  PreferencePanes folder, in .../Library/  */

#define kWidgetsFolderType 				'wdgt'				/*  Dashboard Widgets folder, in system, local, and user domains   */
#define kScreenSaversFolderType 		'scrn'				/*  Screen Savers folder, in system, local, and user domains  */


/* fld# ¥ list of folder names for Folder Mgr */

    type 'fld#' {
        array {
            literal     longint;                // folder type
            integer     inSystemFolder = 0;     // version
            fill byte;                          // high byte of data length
            pstring;                            // folder name
            align word;
        };
    };

#endif /* __FOLDERS_R__ */

