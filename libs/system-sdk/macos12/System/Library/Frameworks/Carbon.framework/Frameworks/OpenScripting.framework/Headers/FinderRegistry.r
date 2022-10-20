/*
     File:       FinderRegistry.r
 
     Contains:   Data types for Finder AppleEvents
 
     Version:    OpenScripting
 
     Copyright:  © 1991-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __FINDERREGISTRY_R__
#define __FINDERREGISTRY_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kAEFinderSuite 					'fndr'
#define kAECleanUp 						'fclu'
#define kAEEject 						'ejct'
#define kAEEmpty 						'empt'
#define kAEErase 						'fera'
#define kAEGestalt 						'gstl'
#define kAEPutAway 						'ptwy'
#define kAERebuildDesktopDB 			'rddb'
#define kAESync 						'fupd'
#define kAEInterceptOpen 				'fopn'

#define kAEDatabaseSuite 				'DATA'
#define kAESort 						'SORT'

#define cInternalFinderObject 			'obj '				/*  cReference - used to distinguish objects used inside the Finder only */
															/*  We do not use class cItem from AERegistry.r. Instead our class Item is a cObject */
															/*          cItem                        = 'citm',   // defined in AERegistry.r */
															/*           cFile                    = 'file',  // defined in AERegistry.r */
#define cAliasFile 						'alia'
#define cApplicationFile 				'appf'
#define cControlPanelFile 				'ccdv'
#define cDeskAccessoryFile 				'dafi'
#define cDocumentFile 					'docf'
#define cFontFile 						'fntf'
#define cSoundFile 						'sndf'
#define cClippingFile 					'clpf'
#define cContainer 						'ctnr'
#define cDesktop 						'cdsk'
#define cSharableContainer 				'sctr'
#define cDisk 							'cdis'
#define cFolder 						'cfol'
#define cSuitcase 						'stcs'
#define cAccessorySuitcase 				'dsut'
#define cFontSuitcase 					'fsut'
#define cTrash 							'ctrs'
#define cDesktopPrinter 				'dskp'
#define cPackage 						'pack'
#define cContentSpace 					'dwnd'				/*           cWindow                    = 'cwin',       // defined in AERegistry.r */
#define cContainerWindow 				'cwnd'
#define cInfoWindow 					'iwnd'
#define cSharingWindow 					'swnd'
#define cStatusWindow 					'qwnd'
#define cClippingWindow 				'lwnd'
#define cPreferencesWindow 				'pwnd'
#define cDTPWindow 						'dtpw'
#define cProcess 						'prcs'
#define cAccessoryProcess 				'pcda'
#define cApplicationProcess 			'pcap'
#define cGroup 							'sgrp'
#define cUser 							'cuse'				/*          cApplication                  = 'capp',     // defined in AERegistry.r */
#define cSharingPrivileges 				'priv'
#define cPreferences 					'cprf'
#define cLabel 							'clbl'
#define cSound 							'snd '
#define cAliasList 						'alst'
#define cSpecialFolders 				'spfl'				/*  For use by viewer search engines: */
#define cOnlineDisk 					'cods'
#define cOnlineLocalDisk 				'clds'
#define cOnlineRemoteDisk 				'crds'				/*  Miscellaneous class definitions */
#define cEntireContents 				'ects'
#define cIconFamily 					'ifam'

															/*     pBounds                        = 'pbnd',       // defined in AERegistry.r */
#define pComment 						'comt'
#define pContainer 						'ctnr'
#define pContentSpace 					'dwnd'
#define pCreationDateOld 				'crtd'				/*  to support pre-Finder 8 scripts */
#define pCreationDate 					'ascd'				/*  from File Commands OSAX */
#define pDescription 					'dscr'
#define pDisk 							'cdis'
#define pFolderOld 						'cfol'				/*  to support pre-Finder 8 scripts */
#define pFolder 						'asdr'				/*  from File Commands OSAX */
#define pIconBitmap 					'iimg'				/*     pID                           = 'ID  ',        // defined in AERegistry.r */
#define pInfoWindow 					'iwnd'
#define pKind 							'kind'
#define pLabelIndex 					'labi'
#define pModificationDateOld 			'modd'				/*  to support pre-Finder 8 scripts */
#define pModificationDate 				'asmo'				/*  from File Commands OSAX */
															/*     pName                      = 'pnam',         // defined in AERegistry.r */
#define pPhysicalSize 					'phys'
#define pPosition 						'posn'
#define pIsSelected 					'issl'
#define pSize 							'ptsz'				/*  pPointSize defined in AERegistry.r */
#define pWindow 						'cwin'
#define pPreferencesWindow 				'pwnd'

#define pFileCreator 					'fcrt'
#define pFileType 						'asty'				/*  from File Commands OSAX */
#define pFileTypeOld 					'fitp'				/*  to support pre-Finder 8 scripts */
#define pIsLocked 						'aslk'				/*  from File Commands OSAX */
#define pIsLockedOld 					'islk'				/*  to support pre-Finder 8 scripts */
															/*     pIsStationeryPad               = 'pspd',         // defined in AERegistry.r                 */
															/*     pVersion                    = 'vers',       // defined in AERegistry.r */
#define pProductVersion 				'ver2'

#define pOriginalItem 					'orig'
#define pMinAppPartition 				'mprt'
#define pAppPartition 					'appt'
#define pSuggestedAppPartition 			'sprt'
#define pIsScriptable 					'isab'

#define pInternetLocation 				'iloc'
#define pSound 							'snd '
#define pShowFolderSize 				'sfsz'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pShowComment 					'scom'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pShowDate 						'sdat'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pShowCreationDate 				'scda'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pShowKind 						'sknd'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pShowLabel 						'slbl'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pShowSize 						'ssiz'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pShowVersion 					'svrs'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pSortDirection 					'sord'
#define pShowDiskInfo 					'sdin'				/*  Always on in Finder 8.0 HIS */
#define pListViewIconSize 				'lvis'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pGridIcons 						'fgrd'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pStaggerIcons 					'fstg'				/*  No longer part of the Finder 8.0 HIS */
#define pViewFont 						'vfnt'
#define pViewFontSize 					'vfsz'

#define pCompletelyExpanded 			'pexc'
#define pContainerWindow 				'cwnd'
#define pEntireContents 				'ects'
#define pExpandable 					'pexa'
#define pExpanded 						'pexp'
#define pPreviousView 					'svew'				/*     pSelection                    = 'sele',       // defined in AERegistry.r */
#define pView 							'pvew'
#define pIconSize 						'lvis'				/*  defined above */
#define pKeepArranged 					'arrg'				/*  OBSOLETE in Finder 9 or later */
#define pKeepArrangedBy 				'arby'				/*  OBSOLETE in Finder 9 or later */

#define pStartupDisk 					'sdsk'
#define pTrash 							'trsh'

#define pOwner 							'sown'
#define pOwnerPrivileges 				'ownr'
#define pGroup 							'sgrp'
#define pGroupPrivileges 				'gppr'
#define pGuestPrivileges 				'gstp'
#define pArePrivilegesInherited 		'iprv'
#define pExported 						'sexp'
#define pMounted 						'smou'
#define pSharingProtection 				'spro'
#define pSharing 						'shar'
#define pSharingWindow 					'swnd'

#define pCapacity 						'capa'
#define pEjectable 						'isej'
#define pFreeSpace 						'frsp'
#define pLocal 							'isrv'
#define pIsStartup 						'istd'

#define pWarnOnEmpty 					'warn'
															/*     pBounds                        = 'pbnd',   // defined in AERegistry.r */
															/*     pHasCloseBox                = 'hclb',     // defined in AERegistry.r */
															/*     pIsFloating                    = 'isfl',     // defined in AERegistry.r */
															/*     pIndex                     = 'pidx',     // defined in AERegistry.r */
															/*     pIsModal                    = 'pmod',   // defined in AERegistry.r */
															/*     pPosition                    = 'posn',     // defined above */
															/*     pIsResizable                = 'prsz',     // defined in AERegistry.r */
															/*     pHasTitleBar                = 'ptit',     // defined in AERegistry.r */
															/*     pVisible                    = 'pvis',   // defined in AERegistry.r */
															/*     pIsZoomable                    = 'iszm',     // defined in AERegistry.r */
															/*     pIsZoomed                    = 'pzum',     // defined in AERegistry.r */
#define pIsZoomedFull 					'zumf'
#define pIsPopup 						'drwr'
#define pIsPulledOpen 					'pull'				/*  only applies to popup windows */
#define pIsCollapsed 					'wshd'				/*  only applies to normal windows */

#define pObject 						'cobj'
#define pSharableContainer 				'sctr'
#define pInfoPanel 						'panl'
#define pFileShareOn 					'fshr'
#define pFileShareStartingUp 			'fsup'
#define pProgramLinkingOn 				'iac '

															/*     pShowFolderSize                   = 'sfsz',         // defined above for Views CP */
															/*     pShowComment                = 'scom',      // defined above for Views CP */
#define pShowModificationDate 			'sdat'				/*  pShowDate defined above for Views CP */
															/*     pShowKind                    = 'sknd',        // defined above for Views CP */
															/*     pShowLabel                    = 'slbl',         // defined above for Views CP */
															/*     pShowSize                    = 'ssiz',        // defined above for Views CP */
															/*     pShowVersion                = 'svrs',      // defined above for Views CP */
															/*     pShowCreationDate             = 'scda',      // Removed from Finder 8.0 HIS */
															/*     pShowFileType                 = 'sfty',       // Removed from Finder 8.0 HIS */
															/*     pShowFileCreator               = 'sfcr',         // Removed from Finder 8.0 HIS */
															/*     pListViewIconSize             = 'lvis',      // defined above for Views CP */
															/*     pGridIcons                    = 'fgrd',         // defined above for Views CP */
															/*     pStaggerIcons                 = 'fstg',       // defined above for Views CP */
															/*     pViewFont                    = 'vfnt',        // defined above for Views CP */
															/*     pViewFontSize                 = 'vfsz',       // defined above for Views CP */
#define pUseRelativeDate 				'urdt'				/*  Moved to a per-folder basis in Finder 8.0 HIS */
#define pDelayBeforeSpringing 			'dela'
#define pSpringOpenFolders 				'sprg'
#define pUseShortMenus 					'usme'
#define pUseWideGrid 					'uswg'
#define pLabel1 						'lbl1'
#define pLabel2 						'lbl2'
#define pLabel3 						'lbl3'
#define pLabel4 						'lbl4'
#define pLabel5 						'lbl5'
#define pLabel6 						'lbl6'
#define pLabel7 						'lbl7'
#define pDefaultIconViewIconSize 		'iisz'
#define pDefaultButtonViewIconSize 		'bisz'
#define pDefaultListViewIconSize 		'lisz'				/*  old use of this name is now pIconSize */
#define pIconViewArrangement 			'iarr'
#define pButtonViewArrangement 			'barr'

#define pNoArrangement 					'narr'
#define pSnapToGridArrangement 			'grda'
#define pByNameArrangement 				'nama'
#define pByModificationDateArrangement 	'mdta'
#define pByCreationDateArrangement 		'cdta'
#define pBySizeArrangement 				'siza'
#define pByKindArrangement 				'kina'
#define pByLabelArrangement 			'laba'

															/*     pName                      = 'pnam',         // defined in AERegistry.r */
#define pFile 							'file'				/*     pCreatorType                = 'fcrt',      // defined above */
															/*     pFileType                    = 'asty',        // defined above */
															/*     pIsFrontProcess                   = 'pisf',         // defined in AERegistry.r */
															/*     pAppPartition                 = 'appt',       // defined above */
#define pPartitionSpaceUsed 			'pusd'				/*     pIsScriptable                 = 'isab',       // defined in AERegistry.r */
															/*     pVisible                    = 'pvis'      // defined in AERegistry.r */
#define pLocalAndRemoteEvents 			'revt'
#define pHasScriptingTerminology 		'hscr'

#define pDeskAccessoryFile 				'dafi'
#define pApplicationFile 				'appf'
															/*     pBounds */
															/*     pIconBitmap */
															/*     pLabelIndex */
															/*     pName */
															/*     pPosition */
															/*     pWindow                        = cWindow,        // defined above */
#define pCanConnect 					'ccon'
#define pCanChangePassword 				'ccpw'
#define pCanDoProgramLinking 			'ciac'
#define pIsOwner 						'isow'
#define pARADialIn 						'arad'
#define pShouldCallBack 				'calb'
#define pCallBackNumber 				'cbnm'

#define pAboutMacintosh 				'abbx'
#define pAppleMenuItemsFolder 			'amnu'				/*  kAppleMenuFolderType */
															/*     pClipboard                    = 'pcli',         // defined in AERegistry.r */
#define pControlPanelsFolder 			'ctrl'				/*  kControlPanelFolderType */
#define pDesktop 						'desk'				/*  kDesktopFolderType */
#define pExtensionsFolder 				'extn'				/*  kExtensionFolderType */
															/*     pFileShareOn                = 'fshr',      // defined above */
#define pFinderPreferences 				'pfrp'
#define pFontsFolder 					'font'
#define pFontsFolderPreAllegro 			'ffnt'				/*  DO NOT USE THIS - FOR BACKWARDS COMPAT ONLY */
															/*     pIsFrontProcess                   = 'pisf',         // defined in AERegistry.r */
															/*     pInsertionLoc                 = 'pins',       // defined in AERegistry.r */
#define pLargestFreeBlock 				'mfre'
#define pPreferencesFolder 				'pref'				/*  kPreferencesFolderType */
															/*     pProductVersion                   = 'ver2',         // defined above */
															/*     pUserSelection                  = 'pusl',        // defined in AERegistry.r */
															/*     pFileShareStartingUp             = 'fsup',        // defined above */
#define pShortCuts 						'scut'
#define pShutdownFolder 				'shdf'
#define pStartupItemsFolder 			'strt'				/*  kStartupFolderType */
#define pSystemFolder 					'macs'				/*  kSystemFolderType */
#define pTemporaryFolder 				'temp'				/*  kTemporaryFolderType */
															/*     pVersion                    = 'vers',       // defined in AERegistry.r */
#define pViewPreferences 				'pvwp'				/*     pVisible                    = 'pvis',       // defined in AERegistry.r */
#define pStartingUp 					'awak'				/*  private property to tell whether the Finder is fully up and running */

#define pSeeFiles 						'prvr'
#define pSeeFolders 					'prvs'
#define pMakeChanges 					'prvw'

#define pSmallIcon 						'smic'
#define pSmallButton 					'smbu'
#define pLargeButton 					'lgbu'
#define pGrid 							'grid'

#define enumViewBy 						'vwby'
#define enumGestalt 					'gsen'
#define enumConflicts 					'cflc'
#define enumExistingItems 				'exsi'
#define enumOlderItems 					'oldr'

#define enumDate 						'enda'
#define enumAnyDate 					'anyd'
#define enumToday 						'tday'
#define enumYesterday 					'yday'
#define enumThisWeek 					'twek'
#define enumLastWeek 					'lwek'
#define enumThisMonth 					'tmon'
#define enumLastMonth 					'lmon'
#define enumThisYear 					'tyer'
#define enumLastYear 					'lyer'
#define enumBeforeDate 					'bfdt'
#define enumAfterDate 					'afdt'
#define enumBetweenDate 				'btdt'
#define enumOnDate 						'ondt'

#define enumAllDocuments 				'alld'
#define enumFolders 					'fold'
#define enumAliases 					'alia'
#define enumStationery 					'stat'

#define enumWhere 						'wher'
#define enumAllLocalDisks 				'aldk'
#define enumAllRemoteDisks 				'ardk'
#define enumAllDisks 					'alld'
#define enumAllOpenFolders 				'aofo'

#define enumIconSize 					'isiz'
#define enumSmallIconSize 				'smic'
#define enumMiniIconSize 				'miic'
#define enumLargeIconSize 				'lgic'

#define enumSortDirection 				'sodr'
#define enumSortDirectionNormal 		'snrm'
#define enumSortDirectionReverse 		'srvs'

#define enumArrangement 				'earr'
#define enumInfoWindowPanel 			'ipnl'
#define enumGeneralPanel 				'gpnl'
#define enumSharingPanel 				'spnl'
#define enumStatusNConfigPanel 			'scnl'
#define enumFontsPanel 					'fpnl'
#define enumMemoryPanel 				'mpnl'

#define enumPrefsWindowPanel 			'pple'
#define enumPrefsGeneralPanel 			'pgnp'
#define enumPrefsLabelPanel 			'plbp'
#define enumPrefsIconViewPanel 			'pivp'
#define enumPrefsButtonViewPanel 		'pbvp'
#define enumPrefsListViewPanel 			'plvp'

#define typeIconFamily 					'ifam'				/*  An AEList of typeIconAndMask, type8BitIcon, & c. */
#define typeIconAndMask 				'ICN#'
#define type8BitMask 					'l8mk'
#define type32BitIcon 					'il32'
#define type8BitIcon 					'icl8'
#define type4BitIcon 					'icl4'
#define typeSmallIconAndMask 			'ics#'
#define typeSmall8BitMask 				's8mk'
#define typeSmall32BitIcon 				'is32'
#define typeSmall8BitIcon 				'ics8'
#define typeSmall4BitIcon 				'ics4'
#define typeRelativeTime 				'rtim'
#define typeConceptualTime 				'timc'

#define keyIconAndMask 					'ICN#'
#define key32BitIcon 					'il32'
#define key8BitIcon 					'icl8'
#define key4BitIcon 					'icl4'
#define key8BitMask 					'l8mk'
#define keySmallIconAndMask 			'ics#'
#define keySmall8BitIcon 				'ics8'
#define keySmall4BitIcon 				'ics4'
#define keySmall32BitIcon 				'is32'
#define keySmall8BitMask 				's8mk'
#define keyMini1BitMask 				'icm#'
#define keyMini4BitIcon 				'icm4'
#define keyMini8BitIcon 				'icm8'
#define keyAEUsing 						'usin'
#define keyAEReplacing 					'alrp'
#define keyAENoAutoRouting 				'rout'
#define keyLocalPositionList 			'mvpl'
#define keyGlobalPositionList 			'mvpg'
#define keyRedirectedDocumentList 		'fpdl'

#define keyASPrepositionHas 			'has '
#define keyAll 							'kyal'
#define keyOldFinderItems 				'fsel'

#define formAlias 						'alis'
#define formCreator 					'fcrt'

#define errFinderIsBusy 				(-15260)
#define errFinderWindowNotOpen 			(-15261)
#define errFinderCannotPutAway 			(-15262)
#define errFinderWindowMustBeIconView 	(-15263)			/*  RequireWindowInIconView */
#define errFinderWindowMustBeListView 	(-15264)			/*  RequireWindowInListView */
#define errFinderCantMoveToDestination 	(-15265)
#define errFinderCantMoveSource 		(-15266)
#define errFinderCantOverwrite 			(-15267)
#define errFinderIncestuousMove 		(-15268)			/*  Could just use errFinderCantMoveSource */
#define errFinderCantMoveToAncestor 	(-15269)			/*  Could also use errFinderCantMoveSource */
#define errFinderCantUseTrashedItems 	(-15270)
#define errFinderItemAlreadyInDest 		(-15271)			/*  Move from folder A to folder A */
#define errFinderUnknownUser 			(-15272)			/*  Includes unknown group */
#define errFinderSharePointsCantInherit  (-15273)
#define errFinderWindowWrongType 		(-15274)
#define errFinderPropertyNowWindowBased  (-15275)
#define errFinderAppFolderProtected 	(-15276)			/*  used by General controls when folder protection is on */
#define errFinderSysFolderProtected 	(-15277)			/*  used by General controls when folder protection is on */
#define errFinderBoundsWrong 			(-15278)
#define errAEValueOutOfRange 			(-15279)
#define errFinderPropertyDoesNotApply 	(-15280)
#define errFinderFileSharingMustBeOn 	(-15281)
#define errFinderMustBeActive 			(-15282)
#define errFinderVolumeNotFound 		(-15283)			/*  more descriptive than what we get with nsvErr */
#define errFinderLockedItemsInTrash 	(-15284)			/*  there are some locked items in the trash */
#define errFinderOnlyLockedItemsInTrash  (-15285)			/*  all the items (except folders) in the trash are locked */
#define errFinderProgramLinkingMustBeOn  (-15286)
#define errFinderWindowMustBeButtonView  (-15287)
#define errFinderBadPackageContents 	(-15288)			/*  something is wrong within the package    */
#define errFinderUnsupportedInsidePackages  (-15289)		/*  operation cannot be used on items within a package      */
#define errFinderCorruptOpenFolderList 	(-15290)			/*  was -15276 in Finder 8.6 and earlier, but that conflicted with General Controls */
#define errFinderNoInvisibleFiles 		(-15291)			/*  was -15277 in Finder 8.6 and earlier, but that conflicted with General Controls */
#define errFinderCantDeleteImmediately 	(-15292)			/*  cannot delete immediately via scripting */
#define errFinderLastReserved 			(-15379)


#endif /* __FINDERREGISTRY_R__ */

