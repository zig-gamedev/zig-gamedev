/*
     File:       MacErrors.r
 
     Contains:   OSErr codes.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/

#ifndef __MACERRORS_R__
#define __MACERRORS_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define paramErr 						(-50)				/* error in user parameter list */
#define noHardwareErr 					(-200)				/* Sound Manager Error Returns */
#define notEnoughHardwareErr 			(-201)				/* Sound Manager Error Returns */
#define userCanceledErr 				(-128)
#define qErr 							(-1)				/* queue element not found during deletion */
#define vTypErr 						(-2)				/* invalid queue element */
#define corErr 							(-3)				/* core routine number out of range */
#define unimpErr 						(-4)				/* unimplemented core routine */
#define SlpTypeErr 						(-5)				/* invalid queue element */
#define seNoDB 							(-8)				/* no debugger installed to handle debugger command */
#define controlErr 						(-17)				/* I/O System Errors */
#define statusErr 						(-18)				/* I/O System Errors */
#define readErr 						(-19)				/* I/O System Errors */
#define writErr 						(-20)				/* I/O System Errors */
#define badUnitErr 						(-21)				/* I/O System Errors */
#define unitEmptyErr 					(-22)				/* I/O System Errors */
#define openErr 						(-23)				/* I/O System Errors */
#define closErr 						(-24)				/* I/O System Errors */
#define dRemovErr 						(-25)				/* tried to remove an open driver */
#define dInstErr 						(-26)				/* DrvrInstall couldn't find driver in resources */

#define abortErr 						(-27)				/* IO call aborted by KillIO */
#define iIOAbortErr 					(-27)				/* IO abort error (Printing Manager) */
#define notOpenErr 						(-28)				/* Couldn't rd/wr/ctl/sts cause driver not opened */
#define unitTblFullErr 					(-29)				/* unit table has no more entries */
#define dceExtErr 						(-30)				/* dce extension error */
#define slotNumErr 						(-360)				/* invalid slot # error */
#define gcrOnMFMErr 					(-400)				/* gcr format on high density media error */
#define dirFulErr 						(-33)				/* Directory full */
#define dskFulErr 						(-34)				/* disk full */
#define nsvErr 							(-35)				/* no such volume */
#define ioErr 							(-36)				/* I/O error (bummers) */
#define bdNamErr 						(-37)				/*  bad file name passed to routine; there may be no bad names in the final system! */
#define fnOpnErr 						(-38)				/* File not open */
#define eofErr 							(-39)				/* End of file */
#define posErr 							(-40)				/* tried to position to before start of file (r/w) */
#define mFulErr 						(-41)				/* memory full (open) or file won't fit (load) */
#define tmfoErr 						(-42)				/* too many files open */
#define fnfErr 							(-43)				/* File not found */
#define wPrErr 							(-44)				/* diskette is write protected. */
#define fLckdErr 						(-45)				/* file is locked */

#define vLckdErr 						(-46)				/* volume is locked */
#define fBsyErr 						(-47)				/* File is busy (delete) */
#define dupFNErr 						(-48)				/* duplicate filename (rename) */
#define opWrErr 						(-49)				/* file already open with with write permission */
#define rfNumErr 						(-51)				/* refnum error */
#define gfpErr 							(-52)				/* get file position error */
#define volOffLinErr 					(-53)				/* volume not on line error (was Ejected) */
#define permErr 						(-54)				/* permissions error (on file open) */
#define volOnLinErr 					(-55)				/* drive volume already on-line at MountVol */
#define nsDrvErr 						(-56)				/* no such drive (tried to mount a bad drive num) */
#define noMacDskErr 					(-57)				/* not a mac diskette (sig bytes are wrong) */
#define extFSErr 						(-58)				/* volume in question belongs to an external fs */
#define fsRnErr 						(-59)				/* file system internal error:during rename the old entry was deleted but could not be restored. */
#define badMDBErr 						(-60)				/* bad master directory block */
#define wrPermErr 						(-61)				/* write permissions error */
#define dirNFErr 						(-120)				/* Directory not found */
#define tmwdoErr 						(-121)				/* No free WDCB available */
#define badMovErr 						(-122)				/* Move into offspring error */
#define wrgVolTypErr 					(-123)				/* Wrong volume type error [operation not supported for MFS] */
#define volGoneErr 						(-124)				/* Server volume has been disconnected. */

#define fidNotFound 					(-1300)				/* no file thread exists. */
#define fidExists 						(-1301)				/* file id already exists */
#define notAFileErr 					(-1302)				/* directory specified */
#define diffVolErr 						(-1303)				/* files on different volumes */
#define catChangedErr 					(-1304)				/* the catalog has been modified */
#define desktopDamagedErr 				(-1305)				/* desktop database files are corrupted */
#define sameFileErr 					(-1306)				/* can't exchange a file with itself */
#define badFidErr 						(-1307)				/* file id is dangling or doesn't match with the file number */
#define notARemountErr 					(-1308)				/* when _Mount allows only remounts and doesn't get one */
#define fileBoundsErr 					(-1309)				/* file's EOF, offset, mark or size is too big */
#define fsDataTooBigErr 				(-1310)				/* file or volume is too big for system */
#define volVMBusyErr 					(-1311)				/* can't eject because volume is in use by VM */
#define badFCBErr 						(-1327)				/* FCBRecPtr is not valid */
#define errFSUnknownCall 				(-1400)				/*  selector is not recognized by this filesystem  */
#define errFSBadFSRef 					(-1401)				/*  FSRef parameter is bad  */
#define errFSBadForkName 				(-1402)				/*  Fork name parameter is bad  */
#define errFSBadBuffer 					(-1403)				/*  A buffer parameter was bad  */
#define errFSBadForkRef 				(-1404)				/*  A ForkRefNum parameter was bad  */
#define errFSBadInfoBitmap 				(-1405)				/*  A CatalogInfoBitmap or VolumeInfoBitmap has reserved or invalid bits set  */
#define errFSMissingCatInfo 			(-1406)				/*  A CatalogInfo parameter was NULL  */
#define errFSNotAFolder 				(-1407)				/*  Expected a folder, got a file  */
#define errFSForkNotFound 				(-1409)				/*  Named fork does not exist  */
#define errFSNameTooLong 				(-1410)				/*  File/fork name is too long to create/rename  */
#define errFSMissingName 				(-1411)				/*  A Unicode name parameter was NULL or nameLength parameter was zero  */
#define errFSBadPosMode 				(-1412)				/*  Newline bits set in positionMode  */
#define errFSBadAllocFlags 				(-1413)				/*  Invalid bits set in allocationFlags  */
#define errFSNoMoreItems 				(-1417)				/*  Iteration ran out of items to return  */
#define errFSBadItemCount 				(-1418)				/*  maximumItems was zero  */
#define errFSBadSearchParams 			(-1419)				/*  Something wrong with CatalogSearch searchParams  */
#define errFSRefsDifferent 				(-1420)				/*  FSCompareFSRefs; refs are for different objects  */
#define errFSForkExists 				(-1421)				/*  Named fork already exists.  */
#define errFSBadIteratorFlags 			(-1422)				/*  Flags passed to FSOpenIterator are bad  */
#define errFSIteratorNotFound 			(-1423)				/*  Passed FSIterator is not an open iterator  */
#define errFSIteratorNotSupported 		(-1424)				/*  The iterator's flags or container are not supported by this call  */
#define errFSQuotaExceeded 				(-1425)				/*  The user's quota of disk blocks has been exhausted.  */
#define errFSOperationNotSupported 		(-1426)				/*  The attempted operation is not supported  */
#define errFSAttributeNotFound 			(-1427)				/*  The requested attribute does not exist  */
#define errFSPropertyNotValid 			(-1428)				/*  The requested property is not valid (has not been set yet)  */
#define errFSNotEnoughSpaceForOperation  (-1429)			/*  There is not enough disk space to perform the requested operation  */
#define envNotPresent 					(-5500)				/* returned by glue. */
#define envBadVers 						(-5501)				/* Version non-positive */
#define envVersTooBig 					(-5502)				/* Version bigger than call can handle */
#define fontDecError 					(-64)				/* error during font declaration */
#define fontNotDeclared 				(-65)				/* font not declared */
#define fontSubErr 						(-66)				/* font substitution occurred */
#define fontNotOutlineErr 				(-32615)			/* bitmap font passed to routine that does outlines only */
#define firstDskErr 					(-84)				/* I/O System Errors */
#define lastDskErr 						(-64)				/* I/O System Errors */
#define noDriveErr 						(-64)				/* drive not installed */
#define offLinErr 						(-65)				/* r/w requested for an off-line drive */
#define noNybErr 						(-66)				/* couldn't find 5 nybbles in 200 tries */

#define noAdrMkErr 						(-67)				/* couldn't find valid addr mark */
#define dataVerErr 						(-68)				/* read verify compare failed */
#define badCksmErr 						(-69)				/* addr mark checksum didn't check */
#define badBtSlpErr 					(-70)				/* bad addr mark bit slip nibbles */
#define noDtaMkErr 						(-71)				/* couldn't find a data mark header */
#define badDCksum 						(-72)				/* bad data mark checksum */
#define badDBtSlp 						(-73)				/* bad data mark bit slip nibbles */
#define wrUnderrun 						(-74)				/* write underrun occurred */
#define cantStepErr 					(-75)				/* step handshake failed */
#define tk0BadErr 						(-76)				/* track 0 detect doesn't change */
#define initIWMErr 						(-77)				/* unable to initialize IWM */
#define twoSideErr 						(-78)				/* tried to read 2nd side on a 1-sided drive */
#define spdAdjErr 						(-79)				/* unable to correctly adjust disk speed */
#define seekErr 						(-80)				/* track number wrong on address mark */
#define sectNFErr 						(-81)				/* sector number never found on a track */
#define fmt1Err 						(-82)				/* can't find sector 0 after track format */
#define fmt2Err 						(-83)				/* can't get enough sync */
#define verErr 							(-84)				/* track failed to verify */
#define clkRdErr 						(-85)				/* unable to read same clock value twice */
#define clkWrErr 						(-86)				/* time written did not verify */
#define prWrErr 						(-87)				/* parameter ram written didn't read-verify */
#define prInitErr 						(-88)				/* InitUtil found the parameter ram uninitialized */
#define rcvrErr 						(-89)				/* SCC receiver error (framing; parity; OR) */
#define breakRecd 						(-90)				/* Break received (SCC) */

															/* Scrap Manager errors */
#define noScrapErr 						(-100)				/* No scrap exists error */
#define noTypeErr 						(-102)				/* No object of that type in scrap */

															/*  ENET error codes  */
#define eLenErr 						(-92)				/* Length error ddpLenErr */
#define eMultiErr 						(-91)				/* Multicast address error ddpSktErr */

#define ddpSktErr 						(-91)				/* error in soket number */
#define ddpLenErr 						(-92)				/* data length too big */
#define noBridgeErr 					(-93)				/* no network bridge for non-local send */
#define lapProtErr 						(-94)				/* error in attaching/detaching protocol */
#define excessCollsns 					(-95)				/* excessive collisions on write */
#define portNotPwr 						(-96)				/* serial port not currently powered */
#define portInUse 						(-97)				/* driver Open error code (port is in use) */
#define portNotCf 						(-98)				/* driver Open error code (parameter RAM not configured for this connection) */

															/*  Memory Manager errors */
#define memROZWarn 						(-99)				/* soft error in ROZ */
#define memROZError 					(-99)				/* hard error in ROZ */
#define memROZErr 						(-99)				/* hard error in ROZ */
#define memFullErr 						(-108)				/* Not enough room in heap zone */
#define nilHandleErr 					(-109)				/* Master Pointer was NIL in HandleZone or other */
#define memWZErr 						(-111)				/* WhichZone failed (applied to free block) */
#define memPurErr 						(-112)				/* trying to purge a locked or non-purgeable block */
#define memAdrErr 						(-110)				/* address was odd; or out of range */
#define memAZErr 						(-113)				/* Address in zone check failed */
#define memPCErr 						(-114)				/* Pointer Check failed */
#define memBCErr 						(-115)				/* Block Check failed */
#define memSCErr 						(-116)				/* Size Check failed */
#define memLockedErr 					(-117)				/* trying to move a locked block (MoveHHi) */

															/*  Printing Errors  */
#define iMemFullErr 					(-108)
#define iIOAbort 						(-27)

#define resourceInMemory 				(-188)				/* Resource already in memory */
#define writingPastEnd 					(-189)				/* Writing past end of file */
#define inputOutOfBounds 				(-190)				/* Offset of Count out of bounds */
#define resNotFound 					(-192)				/* Resource not found */
#define resFNotFound 					(-193)				/* Resource file not found */
#define addResFailed 					(-194)				/* AddResource failed */
#define addRefFailed 					(-195)				/* AddReference failed */
#define rmvResFailed 					(-196)				/* RmveResource failed */
#define rmvRefFailed 					(-197)				/* RmveReference failed */
#define resAttrErr 						(-198)				/* attribute inconsistent with operation */
#define mapReadErr 						(-199)				/* map inconsistent with operation */
#define CantDecompress 					(-186)				/* resource bent ("the bends") - can't decompress a compressed resource */
#define badExtResource 					(-185)				/* extended resource has a bad format. */
#define noMemForPictPlaybackErr 		(-145)
#define rgnOverflowErr 					(-147)
#define rgnTooBigError 					(-147)
#define pixMapTooDeepErr 				(-148)
#define insufficientStackErr 			(-149)
#define nsStackErr 						(-149)

#define evtNotEnb 						1					/* event not enabled at PostEvent */
#define cMatchErr 						(-150)				/* Color2Index failed to find an index */
#define cTempMemErr 					(-151)				/* failed to allocate memory for temporary structures */
#define cNoMemErr 						(-152)				/* failed to allocate memory for structure */
#define cRangeErr 						(-153)				/* range error on colorTable request */
#define cProtectErr 					(-154)				/* colorTable entry protection violation */
#define cDevErr 						(-155)				/* invalid type of graphics device */
#define cResErr 						(-156)				/* invalid resolution for MakeITable */
#define cDepthErr 						(-157)				/* invalid pixel depth  */
#define rgnTooBigErr 					(-500)				/*  should have never been added! (cf. rgnTooBigError = 147)  */
#define updPixMemErr 					(-125)				/* insufficient memory to update a pixmap */
#define pictInfoVersionErr 				(-11000)			/* wrong version of the PictInfo structure */
#define pictInfoIDErr 					(-11001)			/* the internal consistancy check for the PictInfoID is wrong */
#define pictInfoVerbErr 				(-11002)			/* the passed verb was invalid */
#define cantLoadPickMethodErr 			(-11003)			/* unable to load the custom pick proc */
#define colorsRequestedErr 				(-11004)			/* the number of colors requested was illegal */
#define pictureDataErr 					(-11005)			/* the picture data was invalid */

															/*  General Errors  */
#define cmProfileError 					(-170)
#define cmMethodError 					(-171)
#define cmMethodNotFound 				(-175)				/*  CMM not present  */
#define cmProfileNotFound 				(-176)				/*  Responder error  */
#define cmProfilesIdentical 			(-177)				/*  Profiles the same  */
#define cmCantConcatenateError 			(-178)				/*  Profile can't be concatenated  */
#define cmCantXYZ 						(-179)				/*  CMM cant handle XYZ space  */
#define cmCantDeleteProfile 			(-180)				/*  Responder error  */
#define cmUnsupportedDataType 			(-181)				/*  Responder error  */
#define cmNoCurrentProfile 				(-182)				/*  Responder error  */

															/* Sound Manager errors */
#define noHardware 						(-200)				/* obsolete spelling */
#define notEnoughHardware 				(-201)				/* obsolete spelling */
#define queueFull 						(-203)				/* Sound Manager Error Returns */
#define resProblem 						(-204)				/* Sound Manager Error Returns */
#define badChannel 						(-205)				/* Sound Manager Error Returns */
#define badFormat 						(-206)				/* Sound Manager Error Returns */
#define notEnoughBufferSpace 			(-207)				/* could not allocate enough memory */
#define badFileFormat 					(-208)				/* was not type AIFF or was of bad format,corrupt */
#define channelBusy 					(-209)				/* the Channel is being used for a PFD already */
#define buffersTooSmall 				(-210)				/* can not operate in the memory allowed */
#define channelNotBusy 					(-211)
#define noMoreRealTime 					(-212)				/* not enough CPU cycles left to add another task */
#define siVBRCompressionNotSupported 	(-213)				/* vbr audio compression not supported for this operation */
#define siNoSoundInHardware 			(-220)				/* no Sound Input hardware */
#define siBadSoundInDevice 				(-221)				/* invalid index passed to SoundInGetIndexedDevice */
#define siNoBufferSpecified 			(-222)				/* returned by synchronous SPBRecord if nil buffer passed */
#define siInvalidCompression 			(-223)				/* invalid compression type */
#define siHardDriveTooSlow 				(-224)				/* hard drive too slow to record to disk */
#define siInvalidSampleRate 			(-225)				/* invalid sample rate */
#define siInvalidSampleSize 			(-226)				/* invalid sample size */
#define siDeviceBusyErr 				(-227)				/* input device already in use */
#define siBadDeviceName 				(-228)				/* input device could not be opened */
#define siBadRefNum 					(-229)				/* invalid input device reference number */
#define siInputDeviceErr 				(-230)				/* input device hardware failure */
#define siUnknownInfoType 				(-231)				/* invalid info type selector (returned by driver) */
#define siUnknownQuality 				(-232)				/* invalid quality selector (returned by driver) */

#define noSynthFound 					(-240)
#define synthOpenFailed 				(-241)
#define synthNotReady 					(-242)
#define bufTooSmall 					(-243)
#define voiceNotFound 					(-244)
#define incompatibleVoice 				(-245)
#define badDictFormat 					(-246)
#define badInputText 					(-247)

#define midiNoClientErr 				(-250)				/* no client with that ID found */
#define midiNoPortErr 					(-251)				/* no port with that ID found */
#define midiTooManyPortsErr 			(-252)				/* too many ports already installed in the system */
#define midiTooManyConsErr 				(-253)				/* too many connections made */
#define midiVConnectErr 				(-254)				/* pending virtual connection created */
#define midiVConnectMade 				(-255)				/* pending virtual connection resolved */
#define midiVConnectRmvd 				(-256)				/* pending virtual connection removed */
#define midiNoConErr 					(-257)				/* no connection exists between specified ports */
#define midiWriteErr 					(-258)				/* MIDIWritePacket couldn't write to all connected ports */
#define midiNameLenErr 					(-259)				/* name supplied is longer than 31 characters */
#define midiDupIDErr 					(-260)				/* duplicate client ID */
#define midiInvalidCmdErr 				(-261)				/* command not supported for port type */

#define nmTypErr 						(-299)				/* Notification Manager:wrong queue type */
#define siInitSDTblErr 					1					/* slot int dispatch table could not be initialized. */
#define siInitVBLQsErr 					2					/* VBLqueues for all slots could not be initialized. */
#define siInitSPTblErr 					3					/* slot priority table could not be initialized. */
#define sdmJTInitErr 					10					/* SDM Jump Table could not be initialized. */
#define sdmInitErr 						11					/* SDM could not be initialized. */
#define sdmSRTInitErr 					12					/* Slot Resource Table could not be initialized. */
#define sdmPRAMInitErr 					13					/* Slot PRAM could not be initialized. */
#define sdmPriInitErr 					14					/* Cards could not be initialized. */

#define smSDMInitErr 					(-290)				/* Error; SDM could not be initialized. */
#define smSRTInitErr 					(-291)				/* Error; Slot Resource Table could not be initialized. */
#define smPRAMInitErr 					(-292)				/* Error; Slot Resource Table could not be initialized. */
#define smPriInitErr 					(-293)				/* Error; Cards could not be initialized. */
#define smEmptySlot 					(-300)				/* No card in slot */
#define smCRCFail 						(-301)				/* CRC check failed for declaration data */
#define smFormatErr 					(-302)				/* FHeader Format is not Apple's */
#define smRevisionErr 					(-303)				/* Wrong revison level */
#define smNoDir 						(-304)				/* Directory offset is Nil */
#define smDisabledSlot 					(-305)				/* This slot is disabled (-305 use to be smLWTstBad) */
#define smNosInfoArray 					(-306)				/* No sInfoArray. Memory Mgr error. */

#define smResrvErr 						(-307)				/* Fatal reserved error. Resreved field <> 0. */
#define smUnExBusErr 					(-308)				/* Unexpected BusError */
#define smBLFieldBad 					(-309)				/* ByteLanes field was bad. */
#define smFHBlockRdErr 					(-310)				/* Error occurred during _sGetFHeader. */
#define smFHBlkDispErr 					(-311)				/* Error occurred during _sDisposePtr (Dispose of FHeader block). */
#define smDisposePErr 					(-312)				/* _DisposePointer error */
#define smNoBoardSRsrc 					(-313)				/* No Board sResource. */
#define smGetPRErr 						(-314)				/* Error occurred during _sGetPRAMRec (See SIMStatus). */
#define smNoBoardId 					(-315)				/* No Board Id. */
#define smInitStatVErr 					(-316)				/* The InitStatusV field was negative after primary or secondary init. */
#define smInitTblVErr 					(-317)				/* An error occurred while trying to initialize the Slot Resource Table. */
#define smNoJmpTbl 						(-318)				/* SDM jump table could not be created. */
#define smReservedSlot 					(-318)				/* slot is reserved, VM should not use this address space. */
#define smBadBoardId 					(-319)				/* BoardId was wrong; re-init the PRAM record. */
#define smBusErrTO 						(-320)				/* BusError time out. */
															/*  These errors are logged in the  vendor status field of the sInfo record.  */
#define svTempDisable 					(-32768)			/* Temporarily disable card but run primary init. */
#define svDisabled 						(-32640)			/* Reserve range -32640 to -32768 for Apple temp disables. */
#define smBadRefId 						(-330)				/* Reference Id not found in List */
#define smBadsList 						(-331)				/* Bad sList: Id1 < Id2 < Id3 ...format is not followed. */
#define smReservedErr 					(-332)				/* Reserved field not zero */
#define smCodeRevErr 					(-333)				/* Code revision is wrong */

#define smCPUErr 						(-334)				/* Code revision is wrong */
#define smsPointerNil 					(-335)				/* LPointer is nil From sOffsetData. If this error occurs; check sInfo rec for more information. */
#define smNilsBlockErr 					(-336)				/* Nil sBlock error (Don't allocate and try to use a nil sBlock) */
#define smSlotOOBErr 					(-337)				/* Slot out of bounds error */
#define smSelOOBErr 					(-338)				/* Selector out of bounds error */
#define smNewPErr 						(-339)				/* _NewPtr error */
#define smBlkMoveErr 					(-340)				/* _BlockMove error */
#define smCkStatusErr 					(-341)				/* Status of slot = fail. */
#define smGetDrvrNamErr 				(-342)				/* Error occurred during _sGetDrvrName. */
#define smDisDrvrNamErr 				(-343)				/* Error occurred during _sDisDrvrName. */
#define smNoMoresRsrcs 					(-344)				/* No more sResources */
#define smsGetDrvrErr 					(-345)				/* Error occurred during _sGetDriver. */
#define smBadsPtrErr 					(-346)				/* Bad pointer was passed to sCalcsPointer */
#define smByteLanesErr 					(-347)				/* NumByteLanes was determined to be zero. */
#define smOffsetErr 					(-348)				/* Offset was too big (temporary error */
#define smNoGoodOpens 					(-349)				/* No opens were successfull in the loop. */
#define smSRTOvrFlErr 					(-350)				/* SRT over flow. */
#define smRecNotFnd 					(-351)				/* Record not found in the SRT. */

															/* Dictionary Manager errors */
#define notBTree 						(-410)				/* The file is not a dictionary. */
#define btNoSpace 						(-413)				/* Can't allocate disk space. */
#define btDupRecErr 					(-414)				/* Record already exists. */
#define btRecNotFnd 					(-415)				/* Record cannot be found. */
#define btKeyLenErr 					(-416)				/* Maximum key length is too long or equal to zero. */
#define btKeyAttrErr 					(-417)				/* There is no such a key attribute. */
#define unknownInsertModeErr 			(-20000)			/* There is no such an insert mode. */
#define recordDataTooBigErr 			(-20001)			/* The record data is bigger than buffer size (1024 bytes). */
#define invalidIndexErr 				(-20002)			/* The recordIndex parameter is not valid. */

#define fsmFFSNotFoundErr 				(-431)				/*  Foreign File system does not exist - new Pack2 could return this error too  */
#define fsmBusyFFSErr 					(-432)				/*  File system is busy, cannot be removed  */
#define fsmBadFFSNameErr 				(-433)				/*  Name length not 1 <= length <= 31  */
#define fsmBadFSDLenErr 				(-434)				/*  FSD size incompatible with current FSM vers  */
#define fsmDuplicateFSIDErr 			(-435)				/*  FSID already exists on InstallFS  */
#define fsmBadFSDVersionErr 			(-436)				/*  FSM version incompatible with FSD  */
#define fsmNoAlternateStackErr 			(-437)				/*  no alternate stack for HFS CI  */
#define fsmUnknownFSMMessageErr 		(-438)				/*  unknown message passed to FSM  */

															/*  Edition Mgr errors */
#define editionMgrInitErr 				(-450)				/* edition manager not inited by this app */
#define badSectionErr 					(-451)				/* not a valid SectionRecord */
#define notRegisteredSectionErr 		(-452)				/* not a registered SectionRecord */
#define badEditionFileErr 				(-453)				/* edition file is corrupt */
#define badSubPartErr 					(-454)				/* can not use sub parts in this release */
#define multiplePublisherWrn 			(-460)				/* A Publisher is already registered for that container */
#define containerNotFoundWrn 			(-461)				/* could not find editionContainer at this time */
#define containerAlreadyOpenWrn 		(-462)				/* container already opened by this section */
#define notThePublisherWrn 				(-463)				/* not the first registered publisher for that container */

#define teScrapSizeErr 					(-501)				/* scrap item too big for text edit record */
#define hwParamErr 						(-502)				/* bad selector for _HWPriv */
#define driverHardwareGoneErr 			(-503)				/* disk driver's hardware was disconnected */

															/* Process Manager errors */
#define procNotFound 					(-600)				/* no eligible process with specified descriptor */
#define memFragErr 						(-601)				/* not enough room to launch app w/special requirements */
#define appModeErr 						(-602)				/* memory mode is 32-bit, but app not 32-bit clean */
#define protocolErr 					(-603)				/* app made module calls in improper order */
#define hardwareConfigErr 				(-604)				/* hardware configuration not correct for call */
#define appMemFullErr 					(-605)				/* application SIZE not big enough for launch */
#define appIsDaemon 					(-606)				/* app is BG-only, and launch flags disallow this */
#define bufferIsSmall 					(-607)				/* error returns from Post and Accept  */
#define noOutstandingHLE 				(-608)
#define connectionInvalid 				(-609)
#define noUserInteractionAllowed 		(-610)				/*  no user interaction allowed  */

															/*  More Process Manager errors  */
#define wrongApplicationPlatform 		(-875)				/*  The application could not launch because the required platform is not available     */
#define appVersionTooOld 				(-876)				/*  The application's creator and version are incompatible with the current version of Mac OS.  */
#define notAppropriateForClassic 		(-877)				/*  This application won't or shouldn't run on Classic (Problem 2481058).  */

#define threadTooManyReqsErr 			(-617)
#define threadNotFoundErr 				(-618)
#define threadProtocolErr 				(-619)

#define threadBadAppContextErr 			(-616)
#define notEnoughMemoryErr 				(-620)				/* insufficient physical memory */
#define notHeldErr 						(-621)				/* specified range of memory is not held */
#define cannotMakeContiguousErr 		(-622)				/* cannot make specified range contiguous */
#define notLockedErr 					(-623)				/* specified range of memory is not locked */
#define interruptsMaskedErr 			(-624)				/* don’t call with interrupts masked */
#define cannotDeferErr 					(-625)				/* unable to defer additional functions */
#define noMMUErr 						(-626)				/* no MMU present */

#define vmMorePhysicalThanVirtualErr 	(-628)				/* VM could not start because there was more physical memory than virtual memory (bad setting in VM config resource) */
#define vmKernelMMUInitErr 				(-629)				/* VM could not start because VM_MMUInit kernel call failed */
#define vmOffErr 						(-630)				/* VM was configured off, or command key was held down at boot */
#define vmMemLckdErr 					(-631)				/* VM could not start because of a lock table conflict (only on non-SuperMario ROMs) */
#define vmBadDriver 					(-632)				/* VM could not start because the driver was incompatible */
#define vmNoVectorErr 					(-633)				/* VM could not start because the vector code could not load */

#define vmInvalidBackingFileIDErr 		(-640)				/*  invalid BackingFileID  */
#define vmMappingPrivilegesErr 			(-641)				/*  requested MappingPrivileges cannot be obtained  */
#define vmBusyBackingFileErr 			(-642)				/*  open views found on BackingFile  */
#define vmNoMoreBackingFilesErr 		(-643)				/*  no more BackingFiles were found  */
#define vmInvalidFileViewIDErr 			(-644)				/* invalid FileViewID  */
#define vmFileViewAccessErr 			(-645)				/*  requested FileViewAccess cannot be obtained  */
#define vmNoMoreFileViewsErr 			(-646)				/*  no more FileViews were found  */
#define vmAddressNotInFileViewErr 		(-647)				/*  address is not in a FileView  */
#define vmInvalidOwningProcessErr 		(-648)				/*  current process does not own the BackingFileID or FileViewID  */

#define rcDBNull 						(-800)
#define rcDBValue 						(-801)
#define rcDBError 						(-802)
#define rcDBBadType 					(-803)
#define rcDBBreak 						(-804)
#define rcDBExec 						(-805)
#define rcDBBadSessID 					(-806)
#define rcDBBadSessNum 					(-807)				/*  bad session number for DBGetConnInfo  */
#define rcDBBadDDEV 					(-808)				/*  bad ddev specified on DBInit  */
#define rcDBAsyncNotSupp 				(-809)				/*  ddev does not support async calls  */
#define rcDBBadAsyncPB 					(-810)				/*  tried to kill a bad pb  */
#define rcDBNoHandler 					(-811)				/*  no app handler for specified data type  */
#define rcDBWrongVersion 				(-812)				/*  incompatible versions  */
#define rcDBPackNotInited 				(-813)				/*  attempt to call other routine before InitDBPack  */

#define hmHelpDisabled 					(-850)				/*  Show Balloons mode was off, call to routine ignored  */
#define hmBalloonAborted 				(-853)				/*  Returned if mouse was moving or mouse wasn't in window port rect  */
#define hmSameAsLastBalloon 			(-854)				/*  Returned from HMShowMenuBalloon if menu & item is same as last time  */
#define hmHelpManagerNotInited 			(-855)				/*  Returned from HMGetHelpMenuHandle if help menu not setup  */
#define hmSkippedBalloon 				(-857)				/*  Returned from calls if helpmsg specified a skip balloon  */
#define hmWrongVersion 					(-858)				/*  Returned if help mgr resource was the wrong version  */
#define hmUnknownHelpType 				(-859)				/*  Returned if help msg record contained a bad type  */
#define hmOperationUnsupported 			(-861)				/*  Returned from HMShowBalloon call if bad method passed to routine  */
#define hmNoBalloonUp 					(-862)				/*  Returned from HMRemoveBalloon if no balloon was visible when call was made  */
#define hmCloseViewActive 				(-863)				/*  Returned from HMRemoveBalloon if CloseView was active  */

															/* PPC errors */
#define notInitErr 						(-900)				/* PPCToolBox not initialized */
#define nameTypeErr 					(-902)				/* Invalid or inappropriate locationKindSelector in locationName */
#define noPortErr 						(-903)				/* Unable to open port or bad portRefNum.  If you're calling  */
															/*  AESend, this is because your application does not have  */
															/*  the isHighLevelEventAware bit set in your SIZE resource.  */
#define noGlobalsErr 					(-904)				/* The system is hosed, better re-boot */
#define localOnlyErr 					(-905)				/* Network activity is currently disabled */
#define destPortErr 					(-906)				/* Port does not exist at destination */
#define sessTableErr 					(-907)				/* Out of session tables, try again later */
#define noSessionErr 					(-908)				/* Invalid session reference number */
#define badReqErr 						(-909)				/* bad parameter or invalid state for operation */
#define portNameExistsErr 				(-910)				/* port is already open (perhaps in another app) */
#define noUserNameErr 					(-911)				/* user name unknown on destination machine */
#define userRejectErr 					(-912)				/* Destination rejected the session request */
#define noMachineNameErr 				(-913)				/* user hasn't named his Macintosh in the Network Setup Control Panel */
#define noToolboxNameErr 				(-914)				/* A system resource is missing, not too likely */
#define noResponseErr 					(-915)				/* unable to contact destination */
#define portClosedErr 					(-916)				/* port was closed */
#define sessClosedErr 					(-917)				/* session was closed */
#define badPortNameErr 					(-919)				/* PPCPortRec malformed */
#define noDefaultUserErr 				(-922)				/* user hasn't typed in owners name in Network Setup Control Pannel */
#define notLoggedInErr 					(-923)				/* The default userRefNum does not yet exist */
#define noUserRefErr 					(-924)				/* unable to create a new userRefNum */
#define networkErr 						(-925)				/* An error has occurred in the network, not too likely */
#define noInformErr 					(-926)				/* PPCStart failed because destination did not have inform pending */
#define authFailErr 					(-927)				/* unable to authenticate user at destination */
#define noUserRecErr 					(-928)				/* Invalid user reference number */
#define badServiceMethodErr 			(-930)				/* illegal service type, or not supported */
#define badLocNameErr 					(-931)				/* location name malformed */
#define guestNotAllowedErr 				(-932)				/* destination port requires authentication */

#define kFMIterationCompleted 			(-980)
#define kFMInvalidFontFamilyErr 		(-981)
#define kFMInvalidFontErr 				(-982)
#define kFMIterationScopeModifiedErr 	(-983)
#define kFMFontTableAccessErr 			(-984)
#define kFMFontContainerAccessErr 		(-985)

#define noMaskFoundErr 					(-1000)				/* Icon Utilties Error */
#define nbpBuffOvr 						(-1024)				/* Buffer overflow in LookupName */
#define nbpNoConfirm 					(-1025)
#define nbpConfDiff 					(-1026)				/* Name confirmed at different socket */
#define nbpDuplicate 					(-1027)				/* Duplicate name exists already */
#define nbpNotFound 					(-1028)				/* Name not found on remove */
#define nbpNISErr 						(-1029)				/* Error trying to open the NIS */

#define aspBadVersNum 					(-1066)				/* Server cannot support this ASP version */
#define aspBufTooSmall 					(-1067)				/* Buffer too small */
#define aspNoMoreSess 					(-1068)				/* No more sessions on server */
#define aspNoServers 					(-1069)				/* No servers at that address */
#define aspParamErr 					(-1070)				/* Parameter error */
#define aspServerBusy 					(-1071)				/* Server cannot open another session */
#define aspSessClosed 					(-1072)				/* Session closed */
#define aspSizeErr 						(-1073)				/* Command block too big */
#define aspTooMany 						(-1074)				/* Too many clients (server error) */
#define aspNoAck 						(-1075)				/* No ack on attention request (server err) */

#define reqFailed 						(-1096)
#define tooManyReqs 					(-1097)
#define tooManySkts 					(-1098)
#define badATPSkt 						(-1099)
#define badBuffNum 						(-1100)
#define noRelErr 						(-1101)
#define cbNotFound 						(-1102)
#define noSendResp 						(-1103)
#define noDataArea 						(-1104)
#define reqAborted 						(-1105)

															/*  driver control ioResults  */
#define errRefNum 						(-1280)				/*  bad connection refNum  */
#define errAborted 						(-1279)				/*  control call was aborted  */
#define errState 						(-1278)				/*  bad connection state for this operation  */
#define errOpening 						(-1277)				/*  open connection request failed  */
#define errAttention 					(-1276)				/*  attention message too long  */
#define errFwdReset 					(-1275)				/*  read terminated by forward reset  */
#define errDSPQueueSize 				(-1274)				/*  DSP Read/Write Queue Too small  */
#define errOpenDenied 					(-1273)				/*  open connection request was denied  */

#define errAECoercionFail 				(-1700)				/*  bad parameter data or unable to coerce the data supplied  */
#define errAEDescNotFound 				(-1701)
#define errAECorruptData 				(-1702)
#define errAEWrongDataType 				(-1703)
#define errAENotAEDesc 					(-1704)
#define errAEBadListItem 				(-1705)				/*  the specified list item does not exist  */
#define errAENewerVersion 				(-1706)				/*  need newer version of the AppleEvent manager  */
#define errAENotAppleEvent 				(-1707)				/*  the event is not in AppleEvent format  */
#define errAEEventNotHandled 			(-1708)				/*  the AppleEvent was not handled by any handler  */
#define errAEReplyNotValid 				(-1709)				/*  AEResetTimer was passed an invalid reply parameter  */
#define errAEUnknownSendMode 			(-1710)				/*  mode wasn't NoReply, WaitReply, or QueueReply or Interaction level is unknown  */
#define errAEWaitCanceled 				(-1711)				/*  in AESend, the user cancelled out of wait loop for reply or receipt  */
#define errAETimeout 					(-1712)				/*  the AppleEvent timed out  */
#define errAENoUserInteraction 			(-1713)				/*  no user interaction is allowed  */
#define errAENotASpecialFunction 		(-1714)				/*  there is no special function for/with this keyword  */
#define errAEParamMissed 				(-1715)				/*  a required parameter was not accessed  */
#define errAEUnknownAddressType 		(-1716)				/*  the target address type is not known  */
#define errAEHandlerNotFound 			(-1717)				/*  no handler in the dispatch tables fits the parameters to AEGetEventHandler or AEGetCoercionHandler  */
#define errAEReplyNotArrived 			(-1718)				/*  the contents of the reply you are accessing have not arrived yet  */
#define errAEIllegalIndex 				(-1719)				/*  index is out of range in a put operation  */
#define errAEImpossibleRange 			(-1720)				/*  A range like 3rd to 2nd, or 1st to all.  */
#define errAEWrongNumberArgs 			(-1721)				/*  Logical op kAENOT used with other than 1 term  */
#define errAEAccessorNotFound 			(-1723)				/*  Accessor proc matching wantClass and containerType or wildcards not found  */
#define errAENoSuchLogical 				(-1725)				/*  Something other than AND, OR, or NOT  */
#define errAEBadTestKey 				(-1726)				/*  Test is neither typeLogicalDescriptor nor typeCompDescriptor  */
#define errAENotAnObjSpec 				(-1727)				/*  Param to AEResolve not of type 'obj '  */
#define errAENoSuchObject 				(-1728)				/*  e.g.,: specifier asked for the 3rd, but there are only 2. Basically, this indicates a run-time resolution error.  */
#define errAENegativeCount 				(-1729)				/*  CountProc returned negative value  */
#define errAEEmptyListContainer 		(-1730)				/*  Attempt to pass empty list as container to accessor  */
#define errAEUnknownObjectType 			(-1731)				/*  available only in version 1.0.1 or greater  */
#define errAERecordingIsAlreadyOn 		(-1732)				/*  available only in version 1.0.1 or greater  */
#define errAEReceiveTerminate 			(-1733)				/*  break out of all levels of AEReceive to the topmost (1.1 or greater)  */
#define errAEReceiveEscapeCurrent 		(-1734)				/*  break out of only lowest level of AEReceive (1.1 or greater)  */
#define errAEEventFiltered 				(-1735)				/*  event has been filtered, and should not be propogated (1.1 or greater)  */
#define errAEDuplicateHandler 			(-1736)				/*  attempt to install handler in table for identical class and id (1.1 or greater)  */
#define errAEStreamBadNesting 			(-1737)				/*  nesting violation while streaming  */
#define errAEStreamAlreadyConverted 	(-1738)				/*  attempt to convert a stream that has already been converted  */
#define errAEDescIsNull 				(-1739)				/*  attempting to perform an invalid operation on a null descriptor  */
#define errAEBuildSyntaxError 			(-1740)				/*  AEBuildDesc and friends detected a syntax error  */
#define errAEBufferTooSmall 			(-1741)				/*  buffer for AEFlattenDesc too small  */

#define errOSASystemError 				(-1750)
#define errOSAInvalidID 				(-1751)
#define errOSABadStorageType 			(-1752)
#define errOSAScriptError 				(-1753)
#define errOSABadSelector 				(-1754)
#define errOSASourceNotAvailable 		(-1756)
#define errOSANoSuchDialect 			(-1757)
#define errOSADataFormatObsolete 		(-1758)
#define errOSADataFormatTooNew 			(-1759)
#define errOSACorruptData 				(-1702)
#define errOSARecordingIsAlreadyOn 		(-1732)
#define errOSAComponentMismatch 		(-1761)				/*  Parameters are from 2 different components  */
#define errOSACantOpenComponent 		(-1762)				/*  Can't connect to scripting system with that ID  */

#define errOffsetInvalid 				(-1800)
#define errOffsetIsOutsideOfView 		(-1801)
#define errTopOfDocument 				(-1810)
#define errTopOfBody 					(-1811)
#define errEndOfDocument 				(-1812)
#define errEndOfBody 					(-1813)

															/*  Drag Manager error codes  */
#define badDragRefErr 					(-1850)				/*  unknown drag reference  */
#define badDragItemErr 					(-1851)				/*  unknown drag item reference  */
#define badDragFlavorErr 				(-1852)				/*  unknown flavor type  */
#define duplicateFlavorErr 				(-1853)				/*  flavor type already exists  */
#define cantGetFlavorErr 				(-1854)				/*  error while trying to get flavor data  */
#define duplicateHandlerErr 			(-1855)				/*  handler already exists  */
#define handlerNotFoundErr 				(-1856)				/*  handler not found  */
#define dragNotAcceptedErr 				(-1857)				/*  drag was not accepted by receiver  */
#define unsupportedForPlatformErr 		(-1858)				/*  call is for PowerPC only  */
#define noSuitableDisplaysErr 			(-1859)				/*  no displays support translucency  */
#define badImageRgnErr 					(-1860)				/*  bad translucent image region  */
#define badImageErr 					(-1861)				/*  bad translucent image PixMap  */
#define nonDragOriginatorErr 			(-1862)				/*  illegal attempt at originator only data  */

#define couldNotResolveDataRef 			(-2000)
#define badImageDescription 			(-2001)
#define badPublicMovieAtom 				(-2002)
#define cantFindHandler 				(-2003)
#define cantOpenHandler 				(-2004)
#define badComponentType 				(-2005)
#define noMediaHandler 					(-2006)
#define noDataHandler 					(-2007)
#define invalidMedia 					(-2008)
#define invalidTrack 					(-2009)
#define invalidMovie 					(-2010)
#define invalidSampleTable 				(-2011)
#define invalidDataRef 					(-2012)
#define invalidHandler 					(-2013)
#define invalidDuration 				(-2014)
#define invalidTime 					(-2015)
#define cantPutPublicMovieAtom 			(-2016)
#define badEditList 					(-2017)
#define mediaTypesDontMatch 			(-2018)
#define progressProcAborted 			(-2019)
#define movieToolboxUninitialized 		(-2020)
#define noRecordOfApp 					(-2020)				/*  replica  */
#define wfFileNotFound 					(-2021)
#define cantCreateSingleForkFile 		(-2022)				/*  happens when file already exists  */
#define invalidEditState 				(-2023)
#define nonMatchingEditState 			(-2024)
#define staleEditState 					(-2025)
#define userDataItemNotFound 			(-2026)
#define maxSizeToGrowTooSmall 			(-2027)
#define badTrackIndex 					(-2028)
#define trackIDNotFound 				(-2029)
#define trackNotInMovie 				(-2030)
#define timeNotInTrack 					(-2031)
#define timeNotInMedia 					(-2032)
#define badEditIndex 					(-2033)
#define internalQuickTimeError 			(-2034)
#define cantEnableTrack 				(-2035)
#define invalidRect 					(-2036)
#define invalidSampleNum 				(-2037)
#define invalidChunkNum 				(-2038)
#define invalidSampleDescIndex 			(-2039)
#define invalidChunkCache 				(-2040)
#define invalidSampleDescription 		(-2041)
#define dataNotOpenForRead 				(-2042)
#define dataNotOpenForWrite 			(-2043)
#define dataAlreadyOpenForWrite 		(-2044)
#define dataAlreadyClosed 				(-2045)
#define endOfDataReached 				(-2046)
#define dataNoDataRef 					(-2047)
#define noMovieFound 					(-2048)
#define invalidDataRefContainer 		(-2049)
#define badDataRefIndex 				(-2050)
#define noDefaultDataRef 				(-2051)
#define couldNotUseAnExistingSample 	(-2052)
#define featureUnsupported 				(-2053)
#define noVideoTrackInMovieErr 			(-2054)				/*  QT for Windows error  */
#define noSoundTrackInMovieErr 			(-2055)				/*  QT for Windows error  */
#define soundSupportNotAvailableErr 	(-2056)				/*  QT for Windows error  */
#define unsupportedAuxiliaryImportData 	(-2057)
#define auxiliaryExportDataUnavailable 	(-2058)
#define samplesAlreadyInMediaErr 		(-2059)
#define noSourceTreeFoundErr 			(-2060)
#define sourceNotFoundErr 				(-2061)
#define movieTextNotFoundErr 			(-2062)
#define missingRequiredParameterErr 	(-2063)
#define invalidSpriteWorldPropertyErr 	(-2064)
#define invalidSpritePropertyErr 		(-2065)
#define gWorldsNotSameDepthAndSizeErr 	(-2066)
#define invalidSpriteIndexErr 			(-2067)
#define invalidImageIndexErr 			(-2068)
#define invalidSpriteIDErr 				(-2069)

#define internalComponentErr 			(-2070)
#define notImplementedMusicOSErr 		(-2071)
#define cantSendToSynthesizerOSErr 		(-2072)
#define cantReceiveFromSynthesizerOSErr  (-2073)
#define illegalVoiceAllocationOSErr 	(-2074)
#define illegalPartOSErr 				(-2075)
#define illegalChannelOSErr 			(-2076)
#define illegalKnobOSErr 				(-2077)
#define illegalKnobValueOSErr 			(-2078)
#define illegalInstrumentOSErr 			(-2079)
#define illegalControllerOSErr 			(-2080)
#define midiManagerAbsentOSErr 			(-2081)
#define synthesizerNotRespondingOSErr 	(-2082)
#define synthesizerOSErr 				(-2083)
#define illegalNoteChannelOSErr 		(-2084)
#define noteChannelNotAllocatedOSErr 	(-2085)
#define tunePlayerFullOSErr 			(-2086)
#define tuneParseOSErr 					(-2087)
#define noExportProcAvailableErr 		(-2089)
#define videoOutputInUseErr 			(-2090)

#define componentDllLoadErr 			(-2091)				/*  Windows specific errors (when component is loading) */
#define componentDllEntryNotFoundErr 	(-2092)				/*  Windows specific errors (when component is loading) */
#define qtmlDllLoadErr 					(-2093)				/*  Windows specific errors (when qtml is loading) */
#define qtmlDllEntryNotFoundErr 		(-2094)				/*  Windows specific errors (when qtml is loading) */
#define qtmlUninitialized 				(-2095)
#define unsupportedOSErr 				(-2096)
#define unsupportedProcessorErr 		(-2097)
#define componentNotThreadSafeErr 		(-2098)				/*  component is not thread-safe */

#define cannotFindAtomErr 				(-2101)
#define notLeafAtomErr 					(-2102)
#define atomsNotOfSameTypeErr 			(-2103)
#define atomIndexInvalidErr 			(-2104)
#define duplicateAtomTypeAndIDErr 		(-2105)
#define invalidAtomErr 					(-2106)
#define invalidAtomContainerErr 		(-2107)
#define invalidAtomTypeErr 				(-2108)
#define cannotBeLeafAtomErr 			(-2109)
#define pathTooLongErr 					(-2110)
#define emptyPathErr 					(-2111)
#define noPathMappingErr 				(-2112)
#define pathNotVerifiedErr 				(-2113)
#define unknownFormatErr 				(-2114)
#define wackBadFileErr 					(-2115)
#define wackForkNotFoundErr 			(-2116)
#define wackBadMetaDataErr 				(-2117)
#define qfcbNotFoundErr 				(-2118)
#define qfcbNotCreatedErr 				(-2119)
#define AAPNotCreatedErr 				(-2120)
#define AAPNotFoundErr 					(-2121)
#define ASDBadHeaderErr 				(-2122)
#define ASDBadForkErr 					(-2123)
#define ASDEntryNotFoundErr 			(-2124)
#define fileOffsetTooBigErr 			(-2125)
#define notAllowedToSaveMovieErr 		(-2126)
#define qtNetworkAlreadyAllocatedErr 	(-2127)
#define urlDataHHTTPProtocolErr 		(-2129)
#define urlDataHHTTPNoNetDriverErr 		(-2130)
#define urlDataHHTTPURLErr 				(-2131)
#define urlDataHHTTPRedirectErr 		(-2132)
#define urlDataHFTPProtocolErr 			(-2133)
#define urlDataHFTPShutdownErr 			(-2134)
#define urlDataHFTPBadUserErr 			(-2135)
#define urlDataHFTPBadPasswordErr 		(-2136)
#define urlDataHFTPServerErr 			(-2137)
#define urlDataHFTPDataConnectionErr 	(-2138)
#define urlDataHFTPNoDirectoryErr 		(-2139)
#define urlDataHFTPQuotaErr 			(-2140)
#define urlDataHFTPPermissionsErr 		(-2141)
#define urlDataHFTPFilenameErr 			(-2142)
#define urlDataHFTPNoNetDriverErr 		(-2143)
#define urlDataHFTPBadNameListErr 		(-2144)
#define urlDataHFTPNeedPasswordErr 		(-2145)
#define urlDataHFTPNoPasswordErr 		(-2146)
#define urlDataHFTPServerDisconnectedErr  (-2147)
#define urlDataHFTPURLErr 				(-2148)
#define notEnoughDataErr 				(-2149)
#define qtActionNotHandledErr 			(-2157)
#define qtXMLParseErr 					(-2158)
#define qtXMLApplicationErr 			(-2159)

#define digiUnimpErr 					(-2201)				/*  feature unimplemented  */
#define qtParamErr 						(-2202)				/*  bad input parameter (out of range, etc)  */
#define matrixErr 						(-2203)				/*  bad matrix, digitizer did nothing  */
#define notExactMatrixErr 				(-2204)				/*  warning of bad matrix, digitizer did its best  */
#define noMoreKeyColorsErr 				(-2205)				/*  all key indexes in use  */
#define notExactSizeErr 				(-2206)				/*  Can’t do exact size requested  */
#define badDepthErr 					(-2207)				/*  Can’t digitize into this depth  */
#define noDMAErr 						(-2208)				/*  Can’t do DMA digitizing (i.e. can't go to requested dest  */
#define badCallOrderErr 				(-2209)				/*  Usually due to a status call being called prior to being setup first  */

#define kernelIncompleteErr 			(-2401)
#define kernelCanceledErr 				(-2402)
#define kernelOptionsErr 				(-2403)
#define kernelPrivilegeErr 				(-2404)
#define kernelUnsupportedErr 			(-2405)
#define kernelObjectExistsErr 			(-2406)
#define kernelWritePermissionErr 		(-2407)
#define kernelReadPermissionErr 		(-2408)
#define kernelExecutePermissionErr 		(-2409)
#define kernelDeletePermissionErr 		(-2410)
#define kernelExecutionLevelErr 		(-2411)
#define kernelAttributeErr 				(-2412)
#define kernelAsyncSendLimitErr 		(-2413)
#define kernelAsyncReceiveLimitErr 		(-2414)
#define kernelTimeoutErr 				(-2415)
#define kernelInUseErr 					(-2416)
#define kernelTerminatedErr 			(-2417)
#define kernelExceptionErr 				(-2418)
#define kernelIDErr 					(-2419)
#define kernelAlreadyFreeErr 			(-2421)
#define kernelReturnValueErr 			(-2422)
#define kernelUnrecoverableErr 			(-2499)

															/*  Text Services Mgr error codes  */
#define tsmComponentNoErr 				0					/*  component result = no error  */
#define tsmUnsupScriptLanguageErr 		(-2500)
#define tsmInputMethodNotFoundErr 		(-2501)
#define tsmNotAnAppErr 					(-2502)				/*  not an application error  */
#define tsmAlreadyRegisteredErr 		(-2503)				/*  want to register again error  */
#define tsmNeverRegisteredErr 			(-2504)				/*  app never registered error (not TSM aware)  */
#define tsmInvalidDocIDErr 				(-2505)				/*  invalid TSM documentation id  */
#define tsmTSMDocBusyErr 				(-2506)				/*  document is still active  */
#define tsmDocNotActiveErr 				(-2507)				/*  document is NOT active  */
#define tsmNoOpenTSErr 					(-2508)				/*  no open text service  */
#define tsmCantOpenComponentErr 		(-2509)				/*  can’t open the component  */
#define tsmTextServiceNotFoundErr 		(-2510)				/*  no text service found  */
#define tsmDocumentOpenErr 				(-2511)				/*  there are open documents  */
#define tsmUseInputWindowErr 			(-2512)				/*  not TSM aware because we are using input window  */
#define tsmTSHasNoMenuErr 				(-2513)				/*  the text service has no menu  */
#define tsmTSNotOpenErr 				(-2514)				/*  text service is not open  */
#define tsmComponentAlreadyOpenErr 		(-2515)				/*  text service already opened for the document  */
#define tsmInputMethodIsOldErr 			(-2516)				/*  returned by GetDefaultInputMethod  */
#define tsmScriptHasNoIMErr 			(-2517)				/*  script has no imput method or is using old IM  */
#define tsmUnsupportedTypeErr 			(-2518)				/*  unSupported interface type error  */
#define tsmUnknownErr 					(-2519)				/*  any other errors  */
#define tsmInvalidContext 				(-2520)				/*  Invalid TSMContext specified in call  */
#define tsmNoHandler 					(-2521)				/*  No Callback Handler exists for callback  */
#define tsmNoMoreTokens 				(-2522)				/*  No more tokens are available for the source text  */
#define tsmNoStem 						(-2523)				/*  No stem exists for the token  */
#define tsmDefaultIsNotInputMethodErr 	(-2524)				/*  Current Input source is KCHR or uchr, not Input Method  (GetDefaultInputMethod)  */
#define tsmDocPropertyNotFoundErr 		(-2528)				/*  Requested TSM Document property not found  */
#define tsmDocPropertyBufferTooSmallErr  (-2529)			/*  Buffer passed in for property value is too small  */
#define tsmCantChangeForcedClassStateErr  (-2530)			/*  Enabled state of a TextService class has been forced and cannot be changed  */
#define tsmComponentPropertyUnsupportedErr  (-2531)			/*  Component property unsupported (or failed to be set)  */
#define tsmComponentPropertyNotFoundErr  (-2532)			/*  Component property not found  */
#define tsmInputModeChangeFailedErr 	(-2533)				/*  Input Mode not changed  */

															/*  Mixed Mode error codes  */
#define mmInternalError 				(-2526)
#define nrLockedErr 					(-2536)
#define nrNotEnoughMemoryErr 			(-2537)
#define nrInvalidNodeErr 				(-2538)
#define nrNotFoundErr 					(-2539)
#define nrNotCreatedErr 				(-2540)
#define nrNameErr 						(-2541)
#define nrNotSlotDeviceErr 				(-2542)
#define nrDataTruncatedErr 				(-2543)
#define nrPowerErr 						(-2544)
#define nrPowerSwitchAbortErr 			(-2545)
#define nrTypeMismatchErr 				(-2546)
#define nrNotModifiedErr 				(-2547)
#define nrOverrunErr 					(-2548)
#define nrResultCodeBase 				(-2549)
#define nrPathNotFound 					(-2550)				/*  a path component lookup failed  */
#define nrPathBufferTooSmall 			(-2551)				/*  buffer for path is too small  */
#define nrInvalidEntryIterationOp 		(-2552)				/*  invalid entry iteration operation  */
#define nrPropertyAlreadyExists 		(-2553)				/*  property already exists  */
#define nrIterationDone 				(-2554)				/*  iteration operation is done  */
#define nrExitedIteratorScope 			(-2555)				/*  outer scope of iterator was exited  */
#define nrTransactionAborted 			(-2556)				/*  transaction was aborted  */
#define nrCallNotSupported 				(-2557)				/*  This call is not available or supported on this machine  */

#define invalidIconRefErr 				(-2580)				/*  The icon ref is not valid  */
#define noSuchIconErr 					(-2581)				/*  The requested icon could not be found  */
#define noIconDataAvailableErr 			(-2582)				/*  The necessary icon data is not available  */

#define errOSACantCoerce 				(-1700)				/*  Signaled when a value can't be coerced to the desired type.  */
#define errOSACantAccess 				(-1728)				/*  Signaled when an object is not found in a container */
#define errOSACantAssign 				(-10006)			/*  Signaled when an object cannot be set in a container. */
#define errOSAGeneralError 				(-2700)				/*  Signaled by user scripts or applications when no actual error code is to be returned. */
#define errOSADivideByZero 				(-2701)				/*  Signaled when there is an attempt to divide by zero */
#define errOSANumericOverflow 			(-2702)				/*  Signaled when integer or real value is too large to be represented */
#define errOSACantLaunch 				(-2703)				/*  Signaled when application can't be launched or when it is remote and program linking is not enabled */
#define errOSAAppNotHighLevelEventAware  (-2704)			/*  Signaled when an application can't respond to AppleEvents */
#define errOSACorruptTerminology 		(-2705)				/*  Signaled when an application's terminology resource is not readable */
#define errOSAStackOverflow 			(-2706)				/*  Signaled when the runtime stack overflows */
#define errOSAInternalTableOverflow 	(-2707)				/*  Signaled when a runtime internal data structure overflows */
#define errOSADataBlockTooLarge 		(-2708)				/*  Signaled when an intrinsic limitation is exceeded for the size of a value or data structure. */
#define errOSACantGetTerminology 		(-2709)
#define errOSACantCreate 				(-2710)

#define errOSATypeError 				(-1703)
#define OSAMessageNotUnderstood 		(-1708)				/*  Signaled when a message was sent to an object that didn't handle it */
#define OSAUndefinedHandler 			(-1717)				/*  Signaled when a function to be returned doesn't exist.  */
#define OSAIllegalAccess 				(-1723)				/*  Signaled when a container can never have the requested object */
#define OSAIllegalIndex 				(-1719)				/*  Signaled when index was out of range. Specialization of errOSACantAccess */
#define OSAIllegalRange 				(-1720)				/*  Signaled when a range is screwy. Specialization of errOSACantAccess */
#define OSAIllegalAssign 				(-10003)			/*  Signaled when an object can never be set in a container */
#define OSASyntaxError 					(-2740)				/*  Signaled when a syntax error occurs. (e.g. "Syntax error" or "<this> can't go after <that>") */
#define OSASyntaxTypeError 				(-2741)				/*  Signaled when another form of syntax was expected. (e.g. "expected a <type> but found <this>") */
#define OSATokenTooLong 				(-2742)				/*  Signaled when a name or number is too long to be parsed */
#define OSAMissingParameter 			(-1701)				/*  Signaled when a parameter is missing for a function invocation */
#define OSAParameterMismatch 			(-1721)				/*  Signaled when function is called with the wrong number of parameters, or a parameter pattern cannot be matched */
#define OSADuplicateParameter 			(-2750)				/*  Signaled when a formal parameter, local variable, or instance variable is specified more than once */
#define OSADuplicateProperty 			(-2751)				/*  Signaled when a formal parameter, local variable, or instance variable is specified more than once. */
#define OSADuplicateHandler 			(-2752)				/*  Signaled when more than one handler is defined with the same name in a scope where the language doesn't allow it */
#define OSAUndefinedVariable 			(-2753)				/*  Signaled when a variable is accessed that has no value */
#define OSAInconsistentDeclarations 	(-2754)				/*  Signaled when a variable is declared inconsistently in the same scope, such as both local and global */
#define OSAControlFlowError 			(-2755)				/*  Signaled when illegal control flow occurs in an application (no catcher for throw, non-lexical loop exit, etc.) */

#define errASCantConsiderAndIgnore 		(-2720)
#define errASCantCompareMoreThan32k 	(-2721)				/*  Parser/Compiler errors:  */
#define errASTerminologyNestingTooDeep 	(-2760)
#define errASIllegalFormalParameter 	(-2761)
#define errASParameterNotForEvent 		(-2762)
#define errASNoResultReturned 			(-2763)				/*     The range -2780 thru -2799 is reserved for dialect specific error codes. (Error codes from different dialects may overlap.)  */
#define errASInconsistentNames 			(-2780)				/*     English errors:  */

#define cfragFirstErrCode 				(-2800)				/*  The first value in the range of CFM errors. */
#define cfragContextIDErr 				(-2800)				/*  The context ID was not valid. */
#define cfragConnectionIDErr 			(-2801)				/*  The connection ID was not valid. */
#define cfragNoSymbolErr 				(-2802)				/*  The specified symbol was not found. */
#define cfragNoSectionErr 				(-2803)				/*  The specified section was not found. */
#define cfragNoLibraryErr 				(-2804)				/*  The named library was not found. */
#define cfragDupRegistrationErr 		(-2805)				/*  The registration name was already in use. */
#define cfragFragmentFormatErr 			(-2806)				/*  A fragment's container format is unknown. */
#define cfragUnresolvedErr 				(-2807)				/*  A fragment had "hard" unresolved imports. */
#define cfragNoPositionErr 				(-2808)				/*  The registration insertion point was not found. */
#define cfragNoPrivateMemErr 			(-2809)				/*  Out of memory for internal bookkeeping. */
#define cfragNoClientMemErr 			(-2810)				/*  Out of memory for fragment mapping or section instances. */
#define cfragNoIDsErr 					(-2811)				/*  No more CFM IDs for contexts, connections, etc. */
#define cfragInitOrderErr 				(-2812)				/*   */
#define cfragImportTooOldErr 			(-2813)				/*  An import library was too old for a client. */
#define cfragImportTooNewErr 			(-2814)				/*  An import library was too new for a client. */
#define cfragInitLoopErr 				(-2815)				/*  Circularity in required initialization order. */
#define cfragInitAtBootErr 				(-2816)				/*  A boot library has an initialization function.  (System 7 only) */
#define cfragLibConnErr 				(-2817)				/*   */
#define cfragCFMStartupErr 				(-2818)				/*  Internal error during CFM initialization. */
#define cfragCFMInternalErr 			(-2819)				/*  An internal inconstistancy has been detected. */
#define cfragFragmentCorruptErr 		(-2820)				/*  A fragment's container was corrupt (known format). */
#define cfragInitFunctionErr 			(-2821)				/*  A fragment's initialization routine returned an error. */
#define cfragNoApplicationErr 			(-2822)				/*  No application member found in the cfrg resource. */
#define cfragArchitectureErr 			(-2823)				/*  A fragment has an unacceptable architecture. */
#define cfragFragmentUsageErr 			(-2824)				/*  A semantic error in usage of the fragment. */
#define cfragFileSizeErr 				(-2825)				/*  A file was too large to be mapped. */
#define cfragNotClosureErr 				(-2826)				/*  The closure ID was actually a connection ID. */
#define cfragNoRegistrationErr 			(-2827)				/*  The registration name was not found. */
#define cfragContainerIDErr 			(-2828)				/*  The fragment container ID was not valid. */
#define cfragClosureIDErr 				(-2829)				/*  The closure ID was not valid. */
#define cfragAbortClosureErr 			(-2830)				/*  Used by notification handlers to abort a closure. */
#define cfragOutputLengthErr 			(-2831)				/*  An output parameter is too small to hold the value. */
#define cfragMapFileErr 				(-2851)				/*  A file could not be mapped. */
#define cfragExecFileRefErr 			(-2854)				/*  Bundle does not have valid executable file. */
#define cfragStdFolderErr 				(-2855)				/*  Could not find standard CFM folder. */
#define cfragRsrcForkErr 				(-2856)				/*  Resource fork could not be opened. */
#define cfragCFragRsrcErr 				(-2857)				/*  'cfrg' resource could not be loaded. */
#define cfragLastErrCode 				(-2899)				/*  The last value in the range of CFM errors. */

															/*  Reserved values for internal "warnings". */
#define cfragFirstReservedCode 			(-2897)
#define cfragReservedCode_3 			(-2897)
#define cfragReservedCode_2 			(-2898)
#define cfragReservedCode_1 			(-2899)

#if OLDROUTINENAMES
#define fragContextNotFound 			(-2800)
#define fragConnectionIDNotFound 		(-2801)
#define fragSymbolNotFound 				(-2802)
#define fragSectionNotFound 			(-2803)
#define fragLibNotFound 				(-2804)
#define fragDupRegLibName 				(-2805)
#define fragFormatUnknown 				(-2806)
#define fragHadUnresolveds 				(-2807)
#define fragNoMem 						(-2809)
#define fragNoAddrSpace 				(-2810)
#define fragNoContextIDs 				(-2811)
#define fragObjectInitSeqErr 			(-2812)
#define fragImportTooOld 				(-2813)
#define fragImportTooNew 				(-2814)
#define fragInitLoop 					(-2815)
#define fragInitRtnUsageErr 			(-2816)
#define fragLibConnErr 					(-2817)
#define fragMgrInitErr 					(-2818)
#define fragConstErr 					(-2819)
#define fragCorruptErr 					(-2820)
#define fragUserInitProcErr 			(-2821)
#define fragAppNotFound 				(-2822)
#define fragArchError 					(-2823)
#define fragInvalidFragmentUsage 		(-2824)
#define fragLastErrCode 				(-2899)

#endif  /* OLDROUTINENAMES */

#define invalidComponentID 				(-3000)
#define validInstancesExist 			(-3001)
#define componentNotCaptured 			(-3002)
#define componentDontRegister 			(-3003)
#define unresolvedComponentDLLErr 		(-3004)
#define retryComponentRegistrationErr 	(-3005)

#define invalidTranslationPathErr 		(-3025)				/* Source type to destination type not a valid path */
#define couldNotParseSourceFileErr 		(-3026)				/* Source document does not contain source type */
#define noTranslationPathErr 			(-3030)
#define badTranslationSpecErr 			(-3031)
#define noPrefAppErr 					(-3032)

#define buf2SmallErr 					(-3101)
#define noMPPErr 						(-3102)
#define ckSumErr 						(-3103)
#define extractErr 						(-3104)
#define readQErr 						(-3105)
#define atpLenErr 						(-3106)
#define atpBadRsp 						(-3107)
#define recNotFnd 						(-3108)
#define sktClosedErr 					(-3109)

#define kOTNoError 						0					/*  No Error occurred                     */
#define kOTOutOfMemoryErr 				(-3211)				/*  OT ran out of memory, may be a temporary       */
#define kOTNotFoundErr 					(-3201)				/*  OT generic not found error                */
#define kOTDuplicateFoundErr 			(-3216)				/*  OT generic duplicate found error              */
#define kOTBadAddressErr 				(-3150)				/*  XTI2OSStatus(TBADADDR) A Bad address was specified           */
#define kOTBadOptionErr 				(-3151)				/*  XTI2OSStatus(TBADOPT) A Bad option was specified              */
#define kOTAccessErr 					(-3152)				/*  XTI2OSStatus(TACCES) Missing access permission            */
#define kOTBadReferenceErr 				(-3153)				/*  XTI2OSStatus(TBADF) Bad provider reference                */
#define kOTNoAddressErr 				(-3154)				/*  XTI2OSStatus(TNOADDR) No address was specified            */
#define kOTOutStateErr 					(-3155)				/*  XTI2OSStatus(TOUTSTATE) Call issued in wrong state            */
#define kOTBadSequenceErr 				(-3156)				/*  XTI2OSStatus(TBADSEQ) Sequence specified does not exist          */
#define kOTSysErrorErr 					(-3157)				/*  XTI2OSStatus(TSYSERR) A system error occurred             */
#define kOTLookErr 						(-3158)				/*  XTI2OSStatus(TLOOK) An event occurred - call Look()          */
#define kOTBadDataErr 					(-3159)				/*  XTI2OSStatus(TBADDATA) An illegal amount of data was specified  */
#define kOTBufferOverflowErr 			(-3160)				/*  XTI2OSStatus(TBUFOVFLW) Passed buffer not big enough           */
#define kOTFlowErr 						(-3161)				/*  XTI2OSStatus(TFLOW) Provider is flow-controlled           */
#define kOTNoDataErr 					(-3162)				/*  XTI2OSStatus(TNODATA) No data available for reading           */
#define kOTNoDisconnectErr 				(-3163)				/*  XTI2OSStatus(TNODIS) No disconnect indication available          */
#define kOTNoUDErrErr 					(-3164)				/*  XTI2OSStatus(TNOUDERR) No Unit Data Error indication available  */
#define kOTBadFlagErr 					(-3165)				/*  XTI2OSStatus(TBADFLAG) A Bad flag value was supplied           */
#define kOTNoReleaseErr 				(-3166)				/*  XTI2OSStatus(TNOREL) No orderly release indication available    */
#define kOTNotSupportedErr 				(-3167)				/*  XTI2OSStatus(TNOTSUPPORT) Command is not supported            */
#define kOTStateChangeErr 				(-3168)				/*  XTI2OSStatus(TSTATECHNG) State is changing - try again later      */
#define kOTNoStructureTypeErr 			(-3169)				/*  XTI2OSStatus(TNOSTRUCTYPE) Bad structure type requested for OTAlloc     */
#define kOTBadNameErr 					(-3170)				/*  XTI2OSStatus(TBADNAME) A bad endpoint name was supplied          */
#define kOTBadQLenErr 					(-3171)				/*  XTI2OSStatus(TBADQLEN) A Bind to an in-use addr with qlen > 0    */
#define kOTAddressBusyErr 				(-3172)				/*  XTI2OSStatus(TADDRBUSY) Address requested is already in use        */
#define kOTIndOutErr 					(-3173)				/*  XTI2OSStatus(TINDOUT) Accept failed because of pending listen   */
#define kOTProviderMismatchErr 			(-3174)				/*  XTI2OSStatus(TPROVMISMATCH) Tried to accept on incompatible endpoint    */
#define kOTResQLenErr 					(-3175)				/*  XTI2OSStatus(TRESQLEN)                             */
#define kOTResAddressErr 				(-3176)				/*  XTI2OSStatus(TRESADDR)                             */
#define kOTQFullErr 					(-3177)				/*  XTI2OSStatus(TQFULL)                           */
#define kOTProtocolErr 					(-3178)				/*  XTI2OSStatus(TPROTO) An unspecified provider error occurred        */
#define kOTBadSyncErr 					(-3179)				/*  XTI2OSStatus(TBADSYNC) A synchronous call at interrupt time        */
#define kOTCanceledErr 					(-3180)				/*  XTI2OSStatus(TCANCELED) The command was cancelled             */
#define kEPERMErr 						(-3200)				/*  Permission denied             */
#define kENOENTErr 						(-3201)				/*  No such file or directory        */
#define kENORSRCErr 					(-3202)				/*  No such resource                */
#define kEINTRErr 						(-3203)				/*  Interrupted system service         */
#define kEIOErr 						(-3204)				/*  I/O error                  */
#define kENXIOErr 						(-3205)				/*  No such device or address        */
#define kEBADFErr 						(-3208)				/*  Bad file number                  */
#define kEAGAINErr 						(-3210)				/*  Try operation again later        */
#define kENOMEMErr 						(-3211)				/*  Not enough space                */
#define kEACCESErr 						(-3212)				/*  Permission denied             */
#define kEFAULTErr 						(-3213)				/*  Bad address                    */
#define kEBUSYErr 						(-3215)				/*  Device or resource busy           */
#define kEEXISTErr 						(-3216)				/*  File exists                    */
#define kENODEVErr 						(-3218)				/*  No such device                */
#define kEINVALErr 						(-3221)				/*  Invalid argument                */
#define kENOTTYErr 						(-3224)				/*  Not a character device           */
#define kEPIPEErr 						(-3231)				/*  Broken pipe                    */
#define kERANGEErr 						(-3233)				/*  Message size too large for STREAM   */
#define kEWOULDBLOCKErr 				(-3234)				/*  Call would block, so was aborted      */
#define kEDEADLKErr 					(-3234)				/*  or a deadlock would occur        */
#define kEALREADYErr 					(-3236)				/*                            */
#define kENOTSOCKErr 					(-3237)				/*  Socket operation on non-socket      */
#define kEDESTADDRREQErr 				(-3238)				/*  Destination address required       */
#define kEMSGSIZEErr 					(-3239)				/*  Message too long                */
#define kEPROTOTYPEErr 					(-3240)				/*  Protocol wrong type for socket      */
#define kENOPROTOOPTErr 				(-3241)				/*  Protocol not available           */
#define kEPROTONOSUPPORTErr 			(-3242)				/*  Protocol not supported           */
#define kESOCKTNOSUPPORTErr 			(-3243)				/*  Socket type not supported        */
#define kEOPNOTSUPPErr 					(-3244)				/*  Operation not supported on socket   */
#define kEADDRINUSEErr 					(-3247)				/*  Address already in use           */
#define kEADDRNOTAVAILErr 				(-3248)				/*  Can't assign requested address      */
#define kENETDOWNErr 					(-3249)				/*  Network is down                  */
#define kENETUNREACHErr 				(-3250)				/*  Network is unreachable           */
#define kENETRESETErr 					(-3251)				/*  Network dropped connection on reset     */
#define kECONNABORTEDErr 				(-3252)				/*  Software caused connection abort      */
#define kECONNRESETErr 					(-3253)				/*  Connection reset by peer           */
#define kENOBUFSErr 					(-3254)				/*  No buffer space available        */
#define kEISCONNErr 					(-3255)				/*  Socket is already connected          */
#define kENOTCONNErr 					(-3256)				/*  Socket is not connected           */
#define kESHUTDOWNErr 					(-3257)				/*  Can't send after socket shutdown      */
#define kETOOMANYREFSErr 				(-3258)				/*  Too many references: can't splice   */
#define kETIMEDOUTErr 					(-3259)				/*  Connection timed out              */
#define kECONNREFUSEDErr 				(-3260)				/*  Connection refused            */
#define kEHOSTDOWNErr 					(-3263)				/*  Host is down                 */
#define kEHOSTUNREACHErr 				(-3264)				/*  No route to host                */
#define kEPROTOErr 						(-3269)				/*  ••• fill out missing codes •••      */
#define kETIMEErr 						(-3270)				/*                            */
#define kENOSRErr 						(-3271)				/*                            */
#define kEBADMSGErr 					(-3272)				/*                            */
#define kECANCELErr 					(-3273)				/*                            */
#define kENOSTRErr 						(-3274)				/*                            */
#define kENODATAErr 					(-3275)				/*                            */
#define kEINPROGRESSErr 				(-3276)				/*                            */
#define kESRCHErr 						(-3277)				/*                            */
#define kENOMSGErr 						(-3278)				/*                            */
#define kOTClientNotInittedErr 			(-3279)				/*                            */
#define kOTPortHasDiedErr 				(-3280)				/*                            */
#define kOTPortWasEjectedErr 			(-3281)				/*                            */
#define kOTBadConfigurationErr 			(-3282)				/*                            */
#define kOTConfigurationChangedErr 		(-3283)				/*                            */
#define kOTUserRequestedErr 			(-3284)				/*                            */
#define kOTPortLostConnection 			(-3285)				/*                            */

#define kQDNoPalette 					(-3950)				/*  PaletteHandle is NULL */
#define kQDNoColorHWCursorSupport 		(-3951)				/*  CGSSystemSupportsColorHardwareCursors() returned false */
#define kQDCursorAlreadyRegistered 		(-3952)				/*  can be returned from QDRegisterNamedPixMapCursor() */
#define kQDCursorNotRegistered 			(-3953)				/*  can be returned from QDSetNamedPixMapCursor() */
#define kQDCorruptPICTDataErr 			(-3954)

#define firstPickerError 				(-4000)
#define invalidPickerType 				(-4000)
#define requiredFlagsDontMatch 			(-4001)
#define pickerResourceError 			(-4002)
#define cantLoadPicker 					(-4003)
#define cantCreatePickerWindow 			(-4004)
#define cantLoadPackage 				(-4005)
#define pickerCantLive 					(-4006)
#define colorSyncNotInstalled 			(-4007)
#define badProfileError 				(-4008)
#define noHelpForItem 					(-4009)

#define kNSL68kContextNotSupported 		(-4170)				/*  no 68k allowed */
#define kNSLSchedulerError 				(-4171)				/*  A custom thread routine encountered an error */
#define kNSLBadURLSyntax 				(-4172)				/*  URL contains illegal characters */
#define kNSLNoCarbonLib 				(-4173)
#define kNSLUILibraryNotAvailable 		(-4174)				/*  The NSL UI Library needs to be in the Extensions Folder */
#define kNSLNotImplementedYet 			(-4175)
#define kNSLErrNullPtrError 			(-4176)
#define kNSLSomePluginsFailedToLoad 	(-4177)				/*  (one or more plugins failed to load, but at least one did load; this error isn't fatal) */
#define kNSLNullNeighborhoodPtr 		(-4178)				/*  (client passed a null neighborhood ptr) */
#define kNSLNoPluginsForSearch 			(-4179)				/*  (no plugins will respond to search request; bad protocol(s)?) */
#define kNSLSearchAlreadyInProgress 	(-4180)				/*  (you can only have one ongoing search per clientRef) */
#define kNSLNoPluginsFound 				(-4181)				/*  (manager didn't find any valid plugins to load) */
#define kNSLPluginLoadFailed 			(-4182)				/*  (manager unable to load one of the plugins) */
#define kNSLBadProtocolTypeErr 			(-4183)				/*  (client is trying to add a null protocol type) */
#define kNSLNullListPtr 				(-4184)				/*  (client is trying to add items to a nil list) */
#define kNSLBadClientInfoPtr 			(-4185)				/*  (nil ClientAsyncInfoPtr; no reference available) */
#define kNSLCannotContinueLookup 		(-4186)				/*  (Can't continue lookup; error or bad state) */
#define kNSLBufferTooSmallForData 		(-4187)				/*  (Client buffer too small for data from plugin) */
#define kNSLNoContextAvailable 			(-4188)				/*  (ContinueLookup function ptr invalid) */
#define kNSLRequestBufferAlreadyInList 	(-4189)
#define kNSLInvalidPluginSpec 			(-4190)
#define kNSLNoSupportForService 		(-4191)
#define kNSLBadNetConnection 			(-4192)
#define kNSLBadDataTypeErr 				(-4193)
#define kNSLBadServiceTypeErr 			(-4194)
#define kNSLBadReferenceErr 			(-4195)
#define kNSLNoElementsInList 			(-4196)
#define kNSLInsufficientOTVer 			(-4197)
#define kNSLInsufficientSysVer 			(-4198)
#define kNSLNotInitialized 				(-4199)
#define kNSLInitializationFailed 		(-4200)				/*  UNABLE TO INITIALIZE THE MANAGER!!!!! DO NOT CONTINUE!!!! */

#define kDTPHoldJobErr 					(-4200)
#define kDTPStopQueueErr 				(-4201)
#define kDTPTryAgainErr 				(-4202)
#define kDTPAbortJobErr 				128

															/*  Profile Access Errors  */
#define cmElementTagNotFound 			(-4200)
#define cmIndexRangeErr 				(-4201)				/*  Tag index out of range  */
#define cmCantDeleteElement 			(-4202)
#define cmFatalProfileErr 				(-4203)
#define cmInvalidProfile 				(-4204)				/*  A Profile must contain a 'cs1 ' tag to be valid  */
#define cmInvalidProfileLocation 		(-4205)				/*  Operation not supported for this profile location  */
#define cmCantCopyModifiedV1Profile 	(-4215)				/*  Illegal to copy version 1 profiles that have been modified  */
															/*  Profile Search Errors  */
#define cmInvalidSearch 				(-4206)				/*  Bad Search Handle  */
#define cmSearchError 					(-4207)
#define cmErrIncompatibleProfile 		(-4208)				/*  Other ColorSync Errors  */
#define cmInvalidColorSpace 			(-4209)				/*  Profile colorspace does not match bitmap type  */
#define cmInvalidSrcMap 				(-4210)				/*  Source pix/bit map was invalid  */
#define cmInvalidDstMap 				(-4211)				/*  Destination pix/bit map was invalid  */
#define cmNoGDevicesError 				(-4212)				/*  Begin/End Matching -- no gdevices available  */
#define cmInvalidProfileComment 		(-4213)				/*  Bad Profile comment during drawpicture  */
#define cmRangeOverFlow 				(-4214)				/*  Color conversion warning that some output color values over/underflowed and were clipped  */
#define cmNamedColorNotFound 			(-4216)				/*  NamedColor not found  */
#define cmCantGamutCheckError 			(-4217)				/*  Gammut checking not supported by this ColorWorld  */

#define badFolderDescErr 				(-4270)
#define duplicateFolderDescErr 			(-4271)
#define noMoreFolderDescErr 			(-4272)
#define invalidFolderTypeErr 			(-4273)
#define duplicateRoutingErr 			(-4274)
#define routingNotFoundErr 				(-4275)
#define badRoutingSizeErr 				(-4276)

#define coreFoundationUnknownErr 		(-4960)
#define errCoreEndianDataTooShortForFormat  (-4940)
#define errCoreEndianDataTooLongForFormat  (-4941)
#define errCoreEndianDataDoesNotMatchFormat  (-4942)

#define internalScrapErr 				(-4988)
#define duplicateScrapFlavorErr 		(-4989)
#define badScrapRefErr 					(-4990)
#define processStateIncorrectErr 		(-4991)
#define scrapPromiseNotKeptErr 			(-4992)
#define noScrapPromiseKeeperErr 		(-4993)
#define nilScrapFlavorDataErr 			(-4994)
#define scrapFlavorFlagsMismatchErr 	(-4995)
#define scrapFlavorSizeMismatchErr 		(-4996)
#define illegalScrapFlavorFlagsErr 		(-4997)
#define illegalScrapFlavorTypeErr 		(-4998)
#define illegalScrapFlavorSizeErr 		(-4999)
#define scrapFlavorNotFoundErr 			(-102)				/*  == noTypeErr */
#define needClearScrapErr 				(-100)				/*  == noScrapErr */

															/*   AFP Protocol Errors  */
#define afpAccessDenied 				(-5000)				/*  Insufficient access privileges for operation  */
#define afpAuthContinue 				(-5001)				/*  Further information required to complete AFPLogin call  */
#define afpBadUAM 						(-5002)				/*  Unknown user authentication method specified  */
#define afpBadVersNum 					(-5003)				/*  Unknown AFP protocol version number specified  */
#define afpBitmapErr 					(-5004)				/*  Bitmap contained bits undefined for call  */
#define afpCantMove 					(-5005)				/*  Move destination is offspring of source, or root was specified  */
#define afpDenyConflict 				(-5006)				/*  Specified open/deny modes conflict with current open modes  */
#define afpDirNotEmpty 					(-5007)				/*  Cannot delete non-empty directory  */
#define afpDiskFull 					(-5008)				/*  Insufficient free space on volume for operation  */
#define afpEofError 					(-5009)				/*  Read beyond logical end-of-file  */
#define afpFileBusy 					(-5010)				/*  Cannot delete an open file  */
#define afpFlatVol 						(-5011)				/*  Cannot create directory on specified volume  */
#define afpItemNotFound 				(-5012)				/*  Unknown UserName/UserID or missing comment/APPL entry  */
#define afpLockErr 						(-5013)				/*  Some or all of requested range is locked by another user  */
#define afpMiscErr 						(-5014)				/*  Unexpected error encountered during execution  */
#define afpNoMoreLocks 					(-5015)				/*  Maximum lock limit reached  */
#define afpNoServer 					(-5016)				/*  Server not responding  */
#define afpObjectExists 				(-5017)				/*  Specified destination file or directory already exists  */
#define afpObjectNotFound 				(-5018)				/*  Specified file or directory does not exist  */
#define afpParmErr 						(-5019)				/*  A specified parameter was out of allowable range  */
#define afpRangeNotLocked 				(-5020)				/*  Tried to unlock range that was not locked by user  */
#define afpRangeOverlap 				(-5021)				/*  Some or all of range already locked by same user  */
#define afpSessClosed 					(-5022)				/*  Session closed */
#define afpUserNotAuth 					(-5023)				/*  No AFPLogin call has successfully been made for this session  */
#define afpCallNotSupported 			(-5024)				/*  Unsupported AFP call was made  */
#define afpObjectTypeErr 				(-5025)				/*  File/Directory specified where Directory/File expected  */
#define afpTooManyFilesOpen 			(-5026)				/*  Maximum open file count reached  */
#define afpServerGoingDown 				(-5027)				/*  Server is shutting down  */
#define afpCantRename 					(-5028)				/*  AFPRename cannot rename volume  */
#define afpDirNotFound 					(-5029)				/*  Unknown directory specified  */
#define afpIconTypeError 				(-5030)				/*  Icon size specified different from existing icon size  */
#define afpVolLocked 					(-5031)				/*  Volume is Read-Only  */
#define afpObjectLocked 				(-5032)				/*  Object is M/R/D/W inhibited */
#define afpContainsSharedErr 			(-5033)				/*  the folder being shared contains a shared folder */
#define afpIDNotFound 					(-5034)
#define afpIDExists 					(-5035)
#define afpDiffVolErr 					(-5036)
#define afpCatalogChanged 				(-5037)
#define afpSameObjectErr 				(-5038)
#define afpBadIDErr 					(-5039)
#define afpPwdSameErr 					(-5040)				/*  Someone tried to change their password to the same password on a mantadory password change  */
#define afpPwdTooShortErr 				(-5041)				/*  The password being set is too short: there is a minimum length that must be met or exceeded  */
#define afpPwdExpiredErr 				(-5042)				/*  The password being used is too old: this requires the user to change the password before log-in can continue  */
#define afpInsideSharedErr 				(-5043)				/*  The folder being shared is inside a shared folder OR the folder contains a shared folder and is being moved into a shared folder  */
															/*  OR the folder contains a shared folder and is being moved into the descendent of a shared folder. */
#define afpInsideTrashErr 				(-5044)				/*  The folder being shared is inside the trash folder OR the shared folder is being moved into the trash folder  */
															/*  OR the folder is being moved to the trash and it contains a shared folder  */
#define afpPwdNeedsChangeErr 			(-5045)				/*  The password needs to be changed */
#define afpPwdPolicyErr 				(-5046)				/*  Password does not conform to servers password policy  */
#define afpAlreadyLoggedInErr 			(-5047)				/*  User has been authenticated but is already logged in from another machine (and that's not allowed on this server)  */
#define afpCallNotAllowed 				(-5048)				/*  The server knows what you wanted to do, but won't let you do it just now  */

															/*   AppleShare Client Errors  */
#define afpBadDirIDType 				(-5060)
#define afpCantMountMoreSrvre 			(-5061)				/*  The Maximum number of server connections has been reached  */
#define afpAlreadyMounted 				(-5062)				/*  The volume is already mounted  */
#define afpSameNodeErr 					(-5063)				/*  An Attempt was made to connect to a file server running on the same machine  */

#define numberFormattingNotANumberErr 	(-5200)
#define numberFormattingOverflowInDestinationErr  (-5201)
#define numberFormattingBadNumberFormattingObjectErr  (-5202)
#define numberFormattingSpuriousCharErr  (-5203)
#define numberFormattingLiteralMissingErr  (-5204)
#define numberFormattingDelimiterMissingErr  (-5205)
#define numberFormattingEmptyFormatErr 	(-5206)
#define numberFormattingBadFormatErr 	(-5207)
#define numberFormattingBadOptionsErr 	(-5208)
#define numberFormattingBadTokenErr 	(-5209)
#define numberFormattingUnOrderedCurrencyRangeErr  (-5210)
#define numberFormattingBadCurrencyPositionErr  (-5211)
#define numberFormattingNotADigitErr 	(-5212)				/*  deprecated misspelled versions: */
#define numberFormattingUnOrdredCurrencyRangeErr  (-5210)
#define numberFortmattingNotADigitErr 	(-5212)

#define textParserBadParamErr 			(-5220)
#define textParserObjectNotFoundErr 	(-5221)
#define textParserBadTokenValueErr 		(-5222)
#define textParserBadParserObjectErr 	(-5223)
#define textParserParamErr 				(-5224)
#define textParserNoMoreTextErr 		(-5225)
#define textParserBadTextLanguageErr 	(-5226)
#define textParserBadTextEncodingErr 	(-5227)
#define textParserNoSuchTokenFoundErr 	(-5228)
#define textParserNoMoreTokensErr 		(-5229)

#define errUnknownAttributeTag 			(-5240)
#define errMarginWilllNotFit 			(-5241)
#define errNotInImagingMode 			(-5242)
#define errAlreadyInImagingMode 		(-5243)
#define errEngineNotFound 				(-5244)
#define errIteratorReachedEnd 			(-5245)
#define errInvalidRange 				(-5246)
#define errOffsetNotOnElementBounday 	(-5247)
#define errNoHiliteText 				(-5248)
#define errEmptyScrap 					(-5249)
#define errReadOnlyText 				(-5250)
#define errUnknownElement 				(-5251)
#define errNonContiuousAttribute 		(-5252)
#define errCannotUndo 					(-5253)

#define hrHTMLRenderingLibNotInstalledErr  (-5360)
#define hrMiscellaneousExceptionErr 	(-5361)
#define hrUnableToResizeHandleErr 		(-5362)
#define hrURLNotHandledErr 				(-5363)

#define errIANoErr 						0
#define errIAUnknownErr 				(-5380)
#define errIAAllocationErr 				(-5381)
#define errIAParamErr 					(-5382)
#define errIANoMoreItems 				(-5383)
#define errIABufferTooSmall 			(-5384)
#define errIACanceled 					(-5385)
#define errIAInvalidDocument 			(-5386)
#define errIATextExtractionErr 			(-5387)
#define errIAEndOfTextRun 				(-5388)

#define qtsBadSelectorErr 				(-5400)
#define qtsBadStateErr 					(-5401)
#define qtsBadDataErr 					(-5402)				/*  something is wrong with the data  */
#define qtsUnsupportedDataTypeErr 		(-5403)
#define qtsUnsupportedRateErr 			(-5404)
#define qtsUnsupportedFeatureErr 		(-5405)
#define qtsTooMuchDataErr 				(-5406)
#define qtsUnknownValueErr 				(-5407)
#define qtsTimeoutErr 					(-5408)
#define qtsConnectionFailedErr 			(-5420)
#define qtsAddressBusyErr 				(-5421)

															/* Gestalt error codes */
#define gestaltUnknownErr 				(-5550)				/* value returned if Gestalt doesn't know the answer */
#define gestaltUndefSelectorErr 		(-5551)				/* undefined selector was passed to Gestalt */
#define gestaltDupSelectorErr 			(-5552)				/* tried to add an entry that already existed */
#define gestaltLocationErr 				(-5553)				/* gestalt function ptr wasn't in sysheap */

#define menuPropertyInvalidErr 			(-5603)				/*  invalid property creator  */
#define menuPropertyInvalid 			(-5603)				/*  "menuPropertyInvalid" is deprecated  */
#define menuPropertyNotFoundErr 		(-5604)				/*  specified property wasn't found  */
#define menuNotFoundErr 				(-5620)				/*  specified menu or menu ID wasn't found  */
#define menuUsesSystemDefErr 			(-5621)				/*  GetMenuDefinition failed because the menu uses the system MDEF  */
#define menuItemNotFoundErr 			(-5622)				/*  specified menu item wasn't found */
#define menuInvalidErr 					(-5623)				/*  menu is invalid */

#define errInvalidWindowPtr 			(-5600)				/*  tried to pass a bad WindowRef argument */
#define errInvalidWindowRef 			(-5600)				/*  tried to pass a bad WindowRef argument */
#define errUnsupportedWindowAttributesForClass  (-5601)		/*  tried to create a window with WindowAttributes not supported by the WindowClass */
#define errWindowDoesNotHaveProxy 		(-5602)				/*  tried to do something requiring a proxy to a window which doesn’t have a proxy */
#define errInvalidWindowProperty 		(-5603)				/*  tried to access a property tag with private creator */
#define errWindowPropertyNotFound 		(-5604)				/*  tried to get a nonexistent property */
#define errUnrecognizedWindowClass 		(-5605)				/*  tried to create a window with a bad WindowClass */
#define errCorruptWindowDescription 	(-5606)				/*  tried to load a corrupt window description (size or version fields incorrect) */
#define errUserWantsToDragWindow 		(-5607)				/*  if returned from TrackWindowProxyDrag, you should call DragWindow on the window */
#define errWindowsAlreadyInitialized 	(-5608)				/*  tried to call InitFloatingWindows twice, or called InitWindows and then floating windows */
#define errFloatingWindowsNotInitialized  (-5609)			/*  called HideFloatingWindows or ShowFloatingWindows without calling InitFloatingWindows */
#define errWindowNotFound 				(-5610)				/*  returned from FindWindowOfClass */
#define errWindowDoesNotFitOnscreen 	(-5611)				/*  ConstrainWindowToScreen could not make the window fit onscreen */
#define windowAttributeImmutableErr 	(-5612)				/*  tried to change attributes which can't be changed */
#define windowAttributesConflictErr 	(-5613)				/*  passed some attributes that are mutually exclusive */
#define windowManagerInternalErr 		(-5614)				/*  something really weird happened inside the window manager */
#define windowWrongStateErr 			(-5615)				/*  window is not in a state that is valid for the current action */
#define windowGroupInvalidErr 			(-5616)				/*  WindowGroup is invalid */
#define windowAppModalStateAlreadyExistsErr  (-5617)		/*  we're already running this window modally */
#define windowNoAppModalStateErr 		(-5618)				/*  there's no app modal state for the window */
#define errWindowDoesntSupportFocus 	(-30583)
#define errWindowRegionCodeInvalid 		(-30593)

#define dialogNoTimeoutErr 				(-5640)
#define kNavWrongDialogStateErr 		(-5694)
#define kNavWrongDialogClassErr 		(-5695)
#define kNavInvalidSystemConfigErr 		(-5696)
#define kNavCustomControlMessageFailedErr  (-5697)
#define kNavInvalidCustomControlMessageErr  (-5698)
#define kNavMissingKindStringErr 		(-5699)

#define collectionItemLockedErr 		(-5750)
#define collectionItemNotFoundErr 		(-5751)
#define collectionIndexRangeErr 		(-5752)
#define collectionVersionErr 			(-5753)

#define kQTSSUnknownErr 				(-6150)
															/*  Display Manager error codes (-6220...-6269) */
#define kDMGenErr 						(-6220)				/* Unexpected Error */
															/*  Mirroring-Specific Errors  */
#define kDMMirroringOnAlready 			(-6221)				/* Returned by all calls that need mirroring to be off to do their thing. */
#define kDMWrongNumberOfDisplays 		(-6222)				/* Can only handle 2 displays for now. */
#define kDMMirroringBlocked 			(-6223)				/* DMBlockMirroring() has been called. */
#define kDMCantBlock 					(-6224)				/* Mirroring is already on, can’t Block now (call DMUnMirror() first). */
#define kDMMirroringNotOn 				(-6225)				/* Returned by all calls that need mirroring to be on to do their thing. */
															/*  Other Display Manager Errors  */
#define kSysSWTooOld 					(-6226)				/* Missing critical pieces of System Software. */
#define kDMSWNotInitializedErr 			(-6227)				/* Required software not initialized (eg windowmanager or display mgr). */
#define kDMDriverNotDisplayMgrAwareErr 	(-6228)				/* Video Driver does not support display manager. */
#define kDMDisplayNotFoundErr 			(-6229)				/* Could not find item (will someday remove). */
#define kDMNotFoundErr 					(-6229)				/* Could not find item. */
#define kDMDisplayAlreadyInstalledErr 	(-6230)				/* Attempt to add an already installed display. */
#define kDMMainDisplayCannotMoveErr 	(-6231)				/* Trying to move main display (or a display mirrored to it)  */
#define kDMNoDeviceTableclothErr 		(-6231)				/* obsolete */
#define kDMFoundErr 					(-6232)				/* Did not proceed because we found an item */

#define laTooSmallBufferErr 			(-6984)				/*  output buffer is too small to store any result  */
#define laEnvironmentBusyErr 			(-6985)				/*  specified environment is used  */
#define laEnvironmentNotFoundErr 		(-6986)				/*  can't fint the specified environment  */
#define laEnvironmentExistErr 			(-6987)				/*  same name environment is already exists  */
#define laInvalidPathErr 				(-6988)				/*  path is not correct  */
#define laNoMoreMorphemeErr 			(-6989)				/*  nothing to read */
#define laFailAnalysisErr 				(-6990)				/*  analysis failed */
#define laTextOverFlowErr 				(-6991)				/*  text is too long */
#define laDictionaryNotOpenedErr 		(-6992)				/*  the dictionary is not opened */
#define laDictionaryUnknownErr 			(-6993)				/*  can't use this dictionary with this environment */
#define laDictionaryTooManyErr 			(-6994)				/*  too many dictionaries */
#define laPropertyValueErr 				(-6995)				/*  Invalid property value */
#define laPropertyUnknownErr 			(-6996)				/*  the property is unknown to this environment */
#define laPropertyIsReadOnlyErr 		(-6997)				/*  the property is read only */
#define laPropertyNotFoundErr 			(-6998)				/*  can't find the property */
#define laPropertyErr 					(-6999)				/*  Error in properties */
#define laEngineNotFoundErr 			(-7000)				/*  can't find the engine */

#define kUSBNoErr 						0
#define kUSBNoTran 						0
#define kUSBNoDelay 					0
#define kUSBPending 					1

#define kUSBNotSent2Err 				(-6901)				/*   Transaction not sent  */
#define kUSBNotSent1Err 				(-6902)				/*   Transaction not sent  */
#define kUSBBufUnderRunErr 				(-6903)				/*   Host hardware failure on data out, PCI busy?  */
#define kUSBBufOvrRunErr 				(-6904)				/*   Host hardware failure on data in, PCI busy?  */
#define kUSBRes2Err 					(-6905)
#define kUSBRes1Err 					(-6906)
#define kUSBUnderRunErr 				(-6907)				/*   Less data than buffer  */
#define kUSBOverRunErr 					(-6908)				/*   Packet too large or more data than buffer  */
#define kUSBWrongPIDErr 				(-6909)				/*   Pipe stall, Bad or wrong PID  */
#define kUSBPIDCheckErr 				(-6910)				/*   Pipe stall, PID CRC error  */
#define kUSBNotRespondingErr 			(-6911)				/*   Pipe stall, No device, device hung  */
#define kUSBEndpointStallErr 			(-6912)				/*   Device didn't understand  */
#define kUSBDataToggleErr 				(-6913)				/*   Pipe stall, Bad data toggle  */
#define kUSBBitstufErr 					(-6914)				/*   Pipe stall, bitstuffing  */
#define kUSBCRCErr 						(-6915)				/*   Pipe stall, bad CRC  */
#define kUSBLinkErr 					(-6916)

#define kUSBQueueFull 					(-6948)				/*  Internal queue maxxed   */
#define kUSBNotHandled 					(-6987)				/*  Notification was not handled   (same as NotFound) */
#define kUSBUnknownNotification 		(-6949)				/*  Notification type not defined   */
#define kUSBBadDispatchTable 			(-6950)				/*  Improper driver dispatch table      */

#define kUSBInternalReserved10 			(-6951)
#define kUSBInternalReserved9 			(-6952)
#define kUSBInternalReserved8 			(-6953)
#define kUSBInternalReserved7 			(-6954)
#define kUSBInternalReserved6 			(-6955)
#define kUSBInternalReserved5 			(-6956)
#define kUSBInternalReserved4 			(-6957)
#define kUSBInternalReserved3 			(-6958)
#define kUSBInternalReserved2 			(-6959)
#define kUSBInternalReserved1 			(-6960)				/*  reserved */

#define kUSBPortDisabled 				(-6969)				/*  The port you are attached to is disabled, use USBDeviceReset. */
#define kUSBQueueAborted 				(-6970)				/*  Pipe zero stall cleared. */
#define kUSBTimedOut 					(-6971)				/*  Transaction timed out.  */
#define kUSBDeviceDisconnected 			(-6972)				/*  Disconnected during suspend or reset  */
#define kUSBDeviceNotSuspended 			(-6973)				/*  device is not suspended for resume  */
#define kUSBDeviceSuspended 			(-6974)				/*  Device is suspended  */
#define kUSBInvalidBuffer 				(-6975)				/*  bad buffer, usually nil  */
#define kUSBDevicePowerProblem 			(-6976)				/*   Device has a power problem  */
#define kUSBDeviceBusy 					(-6977)				/*   Device is already being configured  */
#define kUSBUnknownInterfaceErr 		(-6978)				/*   Interface ref not recognised  */
#define kUSBPipeStalledError 			(-6979)				/*   Pipe has stalled, error needs to be cleared  */
#define kUSBPipeIdleError 				(-6980)				/*   Pipe is Idle, it will not accept transactions  */
#define kUSBNoBandwidthError 			(-6981)				/*   Not enough bandwidth available  */
#define kUSBAbortedError 				(-6982)				/*   Pipe aborted  */
#define kUSBFlagsError 					(-6983)				/*   Unused flags not zeroed  */
#define kUSBCompletionError 			(-6984)				/*   no completion routine specified  */
#define kUSBPBLengthError 				(-6985)				/*   pbLength too small  */
#define kUSBPBVersionError 				(-6986)				/*   Wrong pbVersion  */
#define kUSBNotFound 					(-6987)				/*   Not found  */
#define kUSBOutOfMemoryErr 				(-6988)				/*   Out of memory  */
#define kUSBDeviceErr 					(-6989)				/*   Device error  */
#define kUSBNoDeviceErr 				(-6990)				/*   No device */
#define kUSBAlreadyOpenErr 				(-6991)				/*   Already open  */
#define kUSBTooManyTransactionsErr 		(-6992)				/*   Too many transactions  */
#define kUSBUnknownRequestErr 			(-6993)				/*   Unknown request  */
#define kUSBRqErr 						(-6994)				/*   Request error  */
#define kUSBIncorrectTypeErr 			(-6995)				/*   Incorrect type  */
#define kUSBTooManyPipesErr 			(-6996)				/*   Too many pipes  */
#define kUSBUnknownPipeErr 				(-6997)				/*   Pipe ref not recognised  */
#define kUSBUnknownDeviceErr 			(-6998)				/*   device ref not recognised  */
#define kUSBInternalErr 				(-6999)				/*  Internal error  */

#define dcmParamErr 					(-7100)				/*  bad parameter */
#define dcmNotDictionaryErr 			(-7101)				/*  not dictionary */
#define dcmBadDictionaryErr 			(-7102)				/*  invalid dictionary */
#define dcmPermissionErr 				(-7103)				/*  invalid permission */
#define dcmDictionaryNotOpenErr 		(-7104)				/*  dictionary not opened */
#define dcmDictionaryBusyErr 			(-7105)				/*  dictionary is busy */
#define dcmBlockFullErr 				(-7107)				/*  dictionary block full */
#define dcmNoRecordErr 					(-7108)				/*  no such record */
#define dcmDupRecordErr 				(-7109)				/*  same record already exist */
#define dcmNecessaryFieldErr 			(-7110)				/*  lack required/identify field */
#define dcmBadFieldInfoErr 				(-7111)				/*  incomplete information */
#define dcmBadFieldTypeErr 				(-7112)				/*  no such field type supported */
#define dcmNoFieldErr 					(-7113)				/*  no such field exist */
#define dcmBadKeyErr 					(-7115)				/*  bad key information */
#define dcmTooManyKeyErr 				(-7116)				/*  too many key field */
#define dcmBadDataSizeErr 				(-7117)				/*  too big data size */
#define dcmBadFindMethodErr 			(-7118)				/*  no such find method supported */
#define dcmBadPropertyErr 				(-7119)				/*  no such property exist */
#define dcmProtectedErr 				(-7121)				/*  need keyword to use dictionary */
#define dcmNoAccessMethodErr 			(-7122)				/*  no such AccessMethod */
#define dcmBadFeatureErr 				(-7124)				/*  invalid AccessMethod feature */
#define dcmIterationCompleteErr 		(-7126)				/*  no more item in iterator */
#define dcmBufferOverflowErr 			(-7127)				/*  data is larger than buffer size */

#define kRAInvalidParameter 			(-7100)
#define kRAInvalidPort 					(-7101)
#define kRAStartupFailed 				(-7102)
#define kRAPortSetupFailed 				(-7103)
#define kRAOutOfMemory 					(-7104)
#define kRANotSupported 				(-7105)
#define kRAMissingResources 			(-7106)
#define kRAIncompatiblePrefs 			(-7107)
#define kRANotConnected 				(-7108)
#define kRAConnectionCanceled 			(-7109)
#define kRAUnknownUser 					(-7110)
#define kRAInvalidPassword 				(-7111)
#define kRAInternalError 				(-7112)
#define kRAInstallationDamaged 			(-7113)
#define kRAPortBusy 					(-7114)
#define kRAUnknownPortState 			(-7115)
#define kRAInvalidPortState 			(-7116)
#define kRAInvalidSerialProtocol 		(-7117)
#define kRAUserLoginDisabled 			(-7118)
#define kRAUserPwdChangeRequired 		(-7119)
#define kRAUserPwdEntryRequired 		(-7120)
#define kRAUserInteractionRequired 		(-7121)
#define kRAInitOpenTransportFailed 		(-7122)
#define kRARemoteAccessNotReady 		(-7123)
#define kRATCPIPInactive 				(-7124)				/*  TCP/IP inactive, cannot be loaded */
#define kRATCPIPNotConfigured 			(-7125)				/*  TCP/IP not configured, could be loaded */
#define kRANotPrimaryInterface 			(-7126)				/*  when IPCP is not primary TCP/IP intf. */
#define kRAConfigurationDBInitErr 		(-7127)
#define kRAPPPProtocolRejected 			(-7128)
#define kRAPPPAuthenticationFailed 		(-7129)
#define kRAPPPNegotiationFailed 		(-7130)
#define kRAPPPUserDisconnected 			(-7131)
#define kRAPPPPeerDisconnected 			(-7132)
#define kRAPeerNotResponding 			(-7133)
#define kRAATalkInactive 				(-7134)
#define kRAExtAuthenticationFailed 		(-7135)
#define kRANCPRejectedbyPeer 			(-7136)
#define kRADuplicateIPAddr 				(-7137)
#define kRACallBackFailed 				(-7138)
#define kRANotEnabled 					(-7139)

#define kATSUInvalidTextLayoutErr 		(-8790)				/*     An attempt was made to use a ATSUTextLayout  */
															/*     which hadn't been initialized or is otherwise  */
															/*     in an invalid state.  */
#define kATSUInvalidStyleErr 			(-8791)				/*     An attempt was made to use a ATSUStyle which   */
															/*     hadn't been properly allocated or is otherwise   */
															/*     in an invalid state.   */
#define kATSUInvalidTextRangeErr 		(-8792)				/*     An attempt was made to extract information    */
															/*     from or perform an operation on a ATSUTextLayout  */
															/*     for a range of text not covered by the ATSUTextLayout.   */
#define kATSUFontsMatched 				(-8793)				/*     This is not an error code but is returned by     */
															/*     ATSUMatchFontsToText() when changes need to     */
															/*     be made to the fonts associated with the text.   */
#define kATSUFontsNotMatched 			(-8794)				/*     This value is returned by ATSUMatchFontsToText()     */
															/*     when the text contains Unicode characters which     */
															/*     cannot be represented by any installed font.   */
#define kATSUNoCorrespondingFontErr 	(-8795)				/*     This value is retrned by font ID conversion  */
															/*     routines ATSUFONDtoFontID() and ATSUFontIDtoFOND()  */
															/*     to indicate that the input font ID is valid but  */
															/*     there is no conversion possible.  For example,  */
															/*     data-fork fonts can only be used with ATSUI not  */
															/*     the FontManager, and so converting an ATSUIFontID  */
															/*     for such a font will fail.    */
#define kATSUInvalidFontErr 			(-8796)				/*     Used when an attempt was made to use an invalid font ID. */
#define kATSUInvalidAttributeValueErr 	(-8797)				/*     Used when an attempt was made to use an attribute with  */
															/*     a bad or undefined value.   */
#define kATSUInvalidAttributeSizeErr 	(-8798)				/*     Used when an attempt was made to use an attribute with a  */
															/*     bad size.   */
#define kATSUInvalidAttributeTagErr 	(-8799)				/*     Used when an attempt was made to use a tag value that */
															/*     was not appropriate for the function call it was used.   */
#define kATSUInvalidCacheErr 			(-8800)				/*     Used when an attempt was made to read in style data  */
															/*     from an invalid cache.  Either the format of the  */
															/*     cached data doesn't match that used by Apple Type  */
															/*     Services for Unicode™ Imaging, or the cached data  */
															/*     is corrupt.   */
#define kATSUNotSetErr 					(-8801)				/*     Used when the client attempts to retrieve an attribute,  */
															/*     font feature, or font variation from a style when it  */
															/*     hadn't been set.  In such a case, the default value will */
															/*     be returned for the attribute's value. */
#define kATSUNoStyleRunsAssignedErr 	(-8802)				/*     Used when an attempt was made to measure, highlight or draw */
															/*     a ATSUTextLayout object that has no styleRuns associated with it. */
#define kATSUQuickDrawTextErr 			(-8803)				/*     Used when QuickDraw Text encounters an error rendering or measuring */
															/*     a line of ATSUI text. */
#define kATSULowLevelErr 				(-8804)				/*     Used when an error was encountered within the low level ATS  */
															/*     mechanism performing an operation requested by ATSUI. */
#define kATSUNoFontCmapAvailableErr 	(-8805)				/*     Used when no CMAP table can be accessed or synthesized for the  */
															/*     font passed into a SetAttributes Font call. */
#define kATSUNoFontScalerAvailableErr 	(-8806)				/*     Used when no font scaler is available for the font passed */
															/*     into a SetAttributes Font call. */
#define kATSUCoordinateOverflowErr 		(-8807)				/*     Used to indicate the coordinates provided to an ATSUI routine caused */
															/*     a coordinate overflow (i.e. > 32K). */
#define kATSULineBreakInWord 			(-8808)				/*     This is not an error code but is returned by ATSUBreakLine to  */
															/*     indicate that the returned offset is within a word since there was */
															/*     only less than one word that could fit the requested width. */
#define kATSUBusyObjectErr 				(-8809)				/*     An ATSUI object is being used by another thread  */

															/*  general text errors */
#define kTextUnsupportedEncodingErr 	(-8738)				/*  specified encoding not supported for this operation */
#define kTextMalformedInputErr 			(-8739)				/*  in DBCS, for example, high byte followed by invalid low byte */
#define kTextUndefinedElementErr 		(-8740)				/*  text conversion errors */
#define kTECMissingTableErr 			(-8745)
#define kTECTableChecksumErr 			(-8746)
#define kTECTableFormatErr 				(-8747)
#define kTECCorruptConverterErr 		(-8748)				/*  invalid converter object reference */
#define kTECNoConversionPathErr 		(-8749)
#define kTECBufferBelowMinimumSizeErr 	(-8750)				/*  output buffer too small to allow processing of first input text element */
#define kTECArrayFullErr 				(-8751)				/*  supplied name buffer or TextRun, TextEncoding, or UnicodeMapping array is too small */
#define kTECBadTextRunErr 				(-8752)
#define kTECPartialCharErr 				(-8753)				/*  input buffer ends in the middle of a multibyte character, conversion stopped */
#define kTECUnmappableElementErr 		(-8754)
#define kTECIncompleteElementErr 		(-8755)				/*  text element may be incomplete or is too long for internal buffers */
#define kTECDirectionErr 				(-8756)				/*  direction stack overflow, etc. */
#define kTECGlobalsUnavailableErr 		(-8770)				/*  globals have already been deallocated (premature TERM) */
#define kTECItemUnavailableErr 			(-8771)				/*  item (e.g. name) not available for specified region (& encoding if relevant) */
															/*  text conversion status codes */
#define kTECUsedFallbacksStatus 		(-8783)
#define kTECNeedFlushStatus 			(-8784)
#define kTECOutputBufferFullStatus 		(-8785)				/*  output buffer has no room for conversion of next input text element (partial conversion) */
															/*  deprecated error & status codes for low-level converter */
#define unicodeChecksumErr 				(-8769)
#define unicodeNoTableErr 				(-8768)
#define unicodeVariantErr 				(-8767)
#define unicodeFallbacksErr 			(-8766)
#define unicodePartConvertErr 			(-8765)
#define unicodeBufErr 					(-8764)
#define unicodeCharErr 					(-8763)
#define unicodeElementErr 				(-8762)
#define unicodeNotFoundErr 				(-8761)
#define unicodeTableFormatErr 			(-8760)
#define unicodeDirectionErr 			(-8759)
#define unicodeContextualErr 			(-8758)
#define unicodeTextEncodingDataErr 		(-8757)

#define kUTCUnderflowErr 				(-8850)
#define kUTCOverflowErr 				(-8851)
#define kIllegalClockValueErr 			(-8852)

#define kATSUInvalidFontFallbacksErr 	(-8900)				/*     An attempt was made to use a ATSUFontFallbacks which hadn't  */
															/*     been initialized or is otherwise in an invalid state.  */
#define kATSUUnsupportedStreamFormatErr  (-8901)			/*     An attempt was made to use a ATSUFlattenedDataStreamFormat */
															/*     which is invalid is not compatible with this version of ATSUI. */
#define kATSUBadStreamErr 				(-8902)				/*     An attempt was made to use a stream which is incorrectly */
															/*     structured, contains bad or out of range values or is */
															/*     missing required information. */
#define kATSUOutputBufferTooSmallErr 	(-8903)				/*     An attempt was made to use an output buffer which was too small */
															/*     for the requested operation. */
#define kATSUInvalidCallInsideCallbackErr  (-8904)			/*     A call was made within the context of a callback that could */
															/*     potetially cause an infinite recursion */
#define kATSUNoFontNameErr 				(-8905)				/*     This error is returned when either ATSUFindFontName() or ATSUGetIndFontName()  */
															/*     function cannot find a corresponding font name given the input parameters */
#define kATSULastErr 					(-8959)				/*     The last ATSUI error code. */

#define codecErr 						(-8960)
#define noCodecErr 						(-8961)
#define codecUnimpErr 					(-8962)
#define codecSizeErr 					(-8963)
#define codecScreenBufErr 				(-8964)
#define codecImageBufErr 				(-8965)
#define codecSpoolErr 					(-8966)
#define codecAbortErr 					(-8967)
#define codecWouldOffscreenErr 			(-8968)
#define codecBadDataErr 				(-8969)
#define codecDataVersErr 				(-8970)
#define codecExtensionNotFoundErr 		(-8971)
#define scTypeNotFoundErr 				(-8971)
#define codecConditionErr 				(-8972)
#define codecOpenErr 					(-8973)
#define codecCantWhenErr 				(-8974)
#define codecCantQueueErr 				(-8975)
#define codecNothingToBlitErr 			(-8976)
#define codecNoMemoryPleaseWaitErr 		(-8977)
#define codecDisabledErr 				(-8978)				/*  codec disabled itself -- pass codecFlagReenable to reset */
#define codecNeedToFlushChainErr 		(-8979)
#define lockPortBitsBadSurfaceErr 		(-8980)
#define lockPortBitsWindowMovedErr 		(-8981)
#define lockPortBitsWindowResizedErr 	(-8982)
#define lockPortBitsWindowClippedErr 	(-8983)
#define lockPortBitsBadPortErr 			(-8984)
#define lockPortBitsSurfaceLostErr 		(-8985)
#define codecParameterDialogConfirm 	(-8986)
#define codecNeedAccessKeyErr 			(-8987)				/*  codec needs password in order to decompress */
#define codecOffscreenFailedErr 		(-8988)
#define codecDroppedFrameErr 			(-8989)				/*  returned from ImageCodecDrawBand  */
#define directXObjectAlreadyExists 		(-8990)
#define lockPortBitsWrongGDeviceErr 	(-8991)
#define codecOffscreenFailedPleaseRetryErr  (-8992)
#define badCodecCharacterizationErr 	(-8993)
#define noThumbnailFoundErr 			(-8994)

#define kBadAdapterErr 					(-9050)				/*  invalid adapter number */
#define kBadAttributeErr 				(-9051)				/*  specified attributes field value is invalid */
#define kBadBaseErr 					(-9052)				/*  specified base system memory address is invalid */
#define kBadEDCErr 						(-9053)				/*  specified EDC generator specified is invalid */
#define kBadIRQErr 						(-9054)				/*  specified IRQ level is invalid */
#define kBadOffsetErr 					(-9055)				/*  specified PC card memory array offset is invalid */
#define kBadPageErr 					(-9056)				/*  specified page is invalid */
#define kBadSizeErr 					(-9057)				/*  specified size is invalid */
#define kBadSocketErr 					(-9058)				/*  specified logical or physical socket number is invalid */
#define kBadTypeErr 					(-9059)				/*  specified window or interface type is invalid */
#define kBadVccErr 						(-9060)				/*  specified Vcc power level index is invalid */
#define kBadVppErr 						(-9061)				/*  specified Vpp1 or Vpp2 power level index is invalid */
#define kBadWindowErr 					(-9062)				/*  specified window is invalid */
#define kBadArgLengthErr 				(-9063)				/*  ArgLength argument is invalid */
#define kBadArgsErr 					(-9064)				/*  values in argument packet are invalid */
#define kBadHandleErr 					(-9065)				/*  clientHandle is invalid */
#define kBadCISErr 						(-9066)				/*  CIS on card is invalid */
#define kBadSpeedErr 					(-9067)				/*  specified speed is unavailable */
#define kReadFailureErr 				(-9068)				/*  unable to complete read request */
#define kWriteFailureErr 				(-9069)				/*  unable to complete write request */
#define kGeneralFailureErr 				(-9070)				/*  an undefined error has occurred */
#define kNoCardErr 						(-9071)				/*  no PC card in the socket */
#define kUnsupportedFunctionErr 		(-9072)				/*  function is not supported by this implementation */
#define kUnsupportedModeErr 			(-9073)				/*  mode is not supported */
#define kBusyErr 						(-9074)				/*  unable to process request at this time - try later */
#define kWriteProtectedErr 				(-9075)				/*  media is write-protected */
#define kConfigurationLockedErr 		(-9076)				/*  a configuration has already been locked */
#define kInUseErr 						(-9077)				/*  requested resource is being used by a client */
#define kNoMoreItemsErr 				(-9078)				/*  there are no more of the requested item */
#define kOutOfResourceErr 				(-9079)				/*  Card Services has exhausted the resource */
#define kNoCardSevicesSocketsErr 		(-9080)
#define kInvalidRegEntryErr 			(-9081)
#define kBadLinkErr 					(-9082)
#define kBadDeviceErr 					(-9083)
#define k16BitCardErr 					(-9084)
#define kCardBusCardErr 				(-9085)
#define kPassCallToChainErr 			(-9086)
#define kCantConfigureCardErr 			(-9087)
#define kPostCardEventErr 				(-9088)				/*  _PCCSLPostCardEvent failed and dropped an event  */
#define kInvalidDeviceNumber 			(-9089)
#define kUnsupportedVsErr 				(-9090)				/*  Unsupported Voltage Sense  */
#define kInvalidCSClientErr 			(-9091)				/*  Card Services ClientID is not registered  */
#define kBadTupleDataErr 				(-9092)				/*  Data in tuple is invalid  */
#define kBadCustomIFIDErr 				(-9093)				/*  Custom interface ID is invalid  */
#define kNoIOWindowRequestedErr 		(-9094)				/*  Request I/O window before calling configuration  */
#define kNoMoreTimerClientsErr 			(-9095)				/*  All timer callbacks are in use  */
#define kNoMoreInterruptSlotsErr 		(-9096)				/*  All internal Interrupt slots are in use  */
#define kNoClientTableErr 				(-9097)				/*  The client table has not be initialized yet  */
#define kUnsupportedCardErr 			(-9098)				/*  Card not supported by generic enabler */
#define kNoCardEnablersFoundErr 		(-9099)				/*  No Enablers were found */
#define kNoEnablerForCardErr 			(-9100)				/*  No Enablers were found that can support the card */
#define kNoCompatibleNameErr 			(-9101)				/*  There is no compatible driver name for this device */
#define kClientRequestDenied 			(-9102)				/*  CS Clients should return this code inorder to  */
															/*    deny a request-type CS Event                 */
#define kNotReadyErr 					(-9103)				/*  PC Card failed to go ready  */
#define kTooManyIOWindowsErr 			(-9104)				/*  device requested more than one I/O window  */
#define kAlreadySavedStateErr 			(-9105)				/*  The state has been saved on previous call  */
#define kAttemptDupCardEntryErr 		(-9106)				/*  The Enabler was asked to create a duplicate card entry  */
#define kCardPowerOffErr 				(-9107)				/*  Power to the card has been turned off  */
#define kNotZVCapableErr 				(-9108)				/*  This socket does not support Zoomed Video  */
#define kNoCardBusCISErr 				(-9109)				/*  No valid CIS exists for this CardBus card  */

#define noDeviceForChannel 				(-9400)
#define grabTimeComplete 				(-9401)
#define cantDoThatInCurrentMode 		(-9402)
#define notEnoughMemoryToGrab 			(-9403)
#define notEnoughDiskSpaceToGrab 		(-9404)
#define couldntGetRequiredComponent 	(-9405)
#define badSGChannel 					(-9406)
#define seqGrabInfoNotAvailable 		(-9407)
#define deviceCantMeetRequest 			(-9408)
#define badControllerHeight 			(-9994)
#define editingNotAllowed 				(-9995)
#define controllerBoundsNotExact 		(-9996)
#define cannotSetWidthOfAttachedController  (-9997)
#define controllerHasFixedHeight 		(-9998)
#define cannotMoveAttachedController 	(-9999)

#define errAEBadKeyForm 				(-10002)
#define errAECantHandleClass 			(-10010)
#define errAECantSupplyType 			(-10009)
#define errAECantUndo 					(-10015)
#define errAEEventFailed 				(-10000)
#define errAEIndexTooLarge 				(-10007)
#define errAEInTransaction 				(-10011)
#define errAELocalOnly 					(-10016)
#define errAENoSuchTransaction 			(-10012)
#define errAENotAnElement 				(-10008)
#define errAENotASingleObject 			(-10014)
#define errAENotModifiable 				(-10003)
#define errAENoUserSelection 			(-10013)
#define errAEPrivilegeError 			(-10004)
#define errAEReadDenied 				(-10005)
#define errAETypeError 					(-10001)
#define errAEWriteDenied 				(-10006)
#define errAENotAnEnumMember 			(-10023)			/*  enumerated value in SetData is not allowed for this property  */
#define errAECantPutThatThere 			(-10024)			/*  in make new, duplicate, etc. class can't be an element of container  */
#define errAEPropertiesClash 			(-10025)			/*  illegal combination of properties settings for Set Data, make new, or duplicate  */

#define telGenericError 				(-1)
#define telNoErr 						0
#define telNoTools 						8					/*  no telephone tools found in extension folder  */
#define telBadTermErr 					(-10001)			/*  invalid TELHandle or handle not found */
#define telBadDNErr 					(-10002)			/*  TELDNHandle not found or invalid  */
#define telBadCAErr 					(-10003)			/*  TELCAHandle not found or invalid  */
#define telBadHandErr 					(-10004)			/*  bad handle specified  */
#define telBadProcErr 					(-10005)			/*  bad msgProc specified  */
#define telCAUnavail 					(-10006)			/*  a CA is not available  */
#define telNoMemErr 					(-10007)			/*  no memory to allocate handle  */
#define telNoOpenErr 					(-10008)			/*  unable to open terminal  */
#define telBadHTypeErr 					(-10010)			/*  bad hook type specified  */
#define telHTypeNotSupp 				(-10011)			/*  hook type not supported by this tool  */
#define telBadLevelErr 					(-10012)			/*  bad volume level setting  */
#define telBadVTypeErr 					(-10013)			/*  bad volume type error  */
#define telVTypeNotSupp 				(-10014)			/*  volume type not supported by this tool */
#define telBadAPattErr 					(-10015)			/*  bad alerting pattern specified  */
#define telAPattNotSupp 				(-10016)			/*  alerting pattern not supported by tool */
#define telBadIndex 					(-10017)			/*  bad index specified  */
#define telIndexNotSupp 				(-10018)			/*  index not supported by this tool  */
#define telBadStateErr 					(-10019)			/*  bad device state specified  */
#define telStateNotSupp 				(-10020)			/*  device state not supported by tool  */
#define telBadIntExt 					(-10021)			/*  bad internal external error  */
#define telIntExtNotSupp 				(-10022)			/*  internal external type not supported by this tool  */
#define telBadDNDType 					(-10023)			/*  bad DND type specified  */
#define telDNDTypeNotSupp 				(-10024)			/*  DND type is not supported by this tool  */
#define telFeatNotSub 					(-10030)			/*  feature not subscribed  */
#define telFeatNotAvail 				(-10031)			/*  feature subscribed but not available  */
#define telFeatActive 					(-10032)			/*  feature already active  */
#define telFeatNotSupp 					(-10033)			/*  feature program call not supported by this tool  */
#define telConfLimitErr 				(-10040)			/*  limit specified is too high for this configuration  */
#define telConfNoLimit 					(-10041)			/*  no limit was specified but required */
#define telConfErr 						(-10042)			/*  conference was not prepared  */
#define telConfRej 						(-10043)			/*  conference request was rejected  */
#define telTransferErr 					(-10044)			/*  transfer not prepared  */
#define telTransferRej 					(-10045)			/*  transfer request rejected  */
#define telCBErr 						(-10046)			/*  call back feature not set previously  */
#define telConfLimitExceeded 			(-10047)			/*  attempt to exceed switch conference limits  */
#define telBadDNType 					(-10050)			/*  DN type invalid  */
#define telBadPageID 					(-10051)			/*  bad page ID specified */
#define telBadIntercomID 				(-10052)			/*  bad intercom ID specified  */
#define telBadFeatureID 				(-10053)			/*  bad feature ID specified  */
#define telBadFwdType 					(-10054)			/*  bad fwdType specified  */
#define telBadPickupGroupID 			(-10055)			/*  bad pickup group ID specified  */
#define telBadParkID 					(-10056)			/*  bad park id specified  */
#define telBadSelect 					(-10057)			/*  unable to select or deselect DN  */
#define telBadBearerType 				(-10058)			/*  bad bearerType specified  */
#define telBadRate 						(-10059)			/*  bad rate specified  */
#define telDNTypeNotSupp 				(-10060)			/*  DN type not supported by tool  */
#define telFwdTypeNotSupp 				(-10061)			/*  forward type not supported by tool  */
#define telBadDisplayMode 				(-10062)			/*  bad display mode specified  */
#define telDisplayModeNotSupp 			(-10063)			/*  display mode not supported by tool  */
#define telNoCallbackRef 				(-10064)			/*  no call back reference was specified, but is required  */
#define telAlreadyOpen 					(-10070)			/*  terminal already open  */
#define telStillNeeded 					(-10071)			/*  terminal driver still needed by someone else  */
#define telTermNotOpen 					(-10072)			/*  terminal not opened via TELOpenTerm  */
#define telCANotAcceptable 				(-10080)			/*  CA not "acceptable"  */
#define telCANotRejectable 				(-10081)			/*  CA not "rejectable"  */
#define telCANotDeflectable 			(-10082)			/*  CA not "deflectable"  */
#define telPBErr 						(-10090)			/*  parameter block error, bad format  */
#define telBadFunction 					(-10091)			/*  bad msgCode specified  */
															/*     telNoTools        = -10101,        unable to find any telephone tools  */
#define telNoSuchTool 					(-10102)			/*  unable to find tool with name specified  */
#define telUnknownErr 					(-10103)			/*  unable to set config  */
#define telNoCommFolder 				(-10106)			/*  Communications/Extensions ƒ not found  */
#define telInitFailed 					(-10107)			/*  initialization failed  */
#define telBadCodeResource 				(-10108)			/*  code resource not found  */
#define telDeviceNotFound 				(-10109)			/*  device not found  */
#define telBadProcID 					(-10110)			/*  invalid procID  */
#define telValidateFailed 				(-10111)			/*  telValidate failed  */
#define telAutoAnsNotOn 				(-10112)			/*  autoAnswer in not turned on  */
#define telDetAlreadyOn 				(-10113)			/*  detection is already turned on  */
#define telBadSWErr 					(-10114)			/*  Software not installed properly  */
#define telBadSampleRate 				(-10115)			/*  incompatible sample rate  */
#define telNotEnoughdspBW 				(-10116)			/*  not enough real-time for allocation  */

#define errTaskNotFound 				(-10780)			/*  no task with that task id exists  */
															/* Power Manager Errors */
#define pmBusyErr 						(-13000)			/* Power Mgr never ready to start handshake */
#define pmReplyTOErr 					(-13001)			/* Timed out waiting for reply */
#define pmSendStartErr 					(-13002)			/* during send, pmgr did not start hs */
#define pmSendEndErr 					(-13003)			/* during send, pmgr did not finish hs */
#define pmRecvStartErr 					(-13004)			/* during receive, pmgr did not start hs */
#define pmRecvEndErr 					(-13005)			/* during receive, pmgr did not finish hs configured for this connection */

#define kPowerHandlerExistsForDeviceErr  (-13006)
#define kPowerHandlerNotFoundForDeviceErr  (-13007)
#define kPowerHandlerNotFoundForProcErr  (-13008)
#define kPowerMgtMessageNotHandled 		(-13009)
#define kPowerMgtRequestDenied 			(-13010)
#define kCantReportProcessorTemperatureErr  (-13013)
#define kProcessorTempRoutineRequiresMPLib2  (-13014)
#define kNoSuchPowerSource 				(-13020)
#define kBridgeSoftwareRunningCantSleep  (-13038)

#define debuggingExecutionContextErr 	(-13880)			/*  routine cannot be called at this time  */
#define debuggingDuplicateSignatureErr 	(-13881)			/*  componentSignature already registered  */
#define debuggingDuplicateOptionErr 	(-13882)			/*  optionSelectorNum already registered  */
#define debuggingInvalidSignatureErr 	(-13883)			/*  componentSignature not registered  */
#define debuggingInvalidOptionErr 		(-13884)			/*  optionSelectorNum is not registered  */
#define debuggingInvalidNameErr 		(-13885)			/*  componentName or optionName is invalid (NULL)  */
#define debuggingNoCallbackErr 			(-13886)			/*  debugging component has no callback  */
#define debuggingNoMatchErr 			(-13887)			/*  debugging component or option not found at this index  */

#define kHIDVersionIncompatibleErr 		(-13909)
#define kHIDDeviceNotReady 				(-13910)			/*  The device is still initializing, try again later */

#define kHIDSuccess 					0
#define kHIDInvalidRangePageErr 		(-13923)
#define kHIDReportIDZeroErr 			(-13924)
#define kHIDReportCountZeroErr 			(-13925)
#define kHIDReportSizeZeroErr 			(-13926)
#define kHIDUnmatchedDesignatorRangeErr  (-13927)
#define kHIDUnmatchedStringRangeErr 	(-13928)
#define kHIDInvertedUsageRangeErr 		(-13929)
#define kHIDUnmatchedUsageRangeErr 		(-13930)
#define kHIDInvertedPhysicalRangeErr 	(-13931)
#define kHIDInvertedLogicalRangeErr 	(-13932)
#define kHIDBadLogicalMaximumErr 		(-13933)
#define kHIDBadLogicalMinimumErr 		(-13934)
#define kHIDUsagePageZeroErr 			(-13935)
#define kHIDEndOfDescriptorErr 			(-13936)
#define kHIDNotEnoughMemoryErr 			(-13937)
#define kHIDBadParameterErr 			(-13938)
#define kHIDNullPointerErr 				(-13939)
#define kHIDInvalidReportLengthErr 		(-13940)
#define kHIDInvalidReportTypeErr 		(-13941)
#define kHIDBadLogPhysValuesErr 		(-13942)
#define kHIDIncompatibleReportErr 		(-13943)
#define kHIDInvalidPreparsedDataErr 	(-13944)
#define kHIDNotValueArrayErr 			(-13945)
#define kHIDUsageNotFoundErr 			(-13946)
#define kHIDValueOutOfRangeErr 			(-13947)
#define kHIDBufferTooSmallErr 			(-13948)
#define kHIDNullStateErr 				(-13949)
#define kHIDBaseError 					(-13950)

#define kModemOutOfMemory 				(-14000)
#define kModemPreferencesMissing 		(-14001)
#define kModemScriptMissing 			(-14002)

#define kTXNEndIterationErr 			(-22000)			/*  Function was not able to iterate through the data contained by a text object */
#define kTXNCannotAddFrameErr 			(-22001)			/*  Multiple frames are not currently supported in MLTE */
#define kTXNInvalidFrameIDErr 			(-22002)			/*  The frame ID is invalid */
#define kTXNIllegalToCrossDataBoundariesErr  (-22003)		/*  Offsets specify a range that crosses a data type boundary */
#define kTXNUserCanceledOperationErr 	(-22004)			/*  A user canceled an operation before your application completed processing it */
#define kTXNBadDefaultFileTypeWarning 	(-22005)			/*  The text file is not in the format you specified */
#define kTXNCannotSetAutoIndentErr 		(-22006)			/*  Auto indentation is not available when word wrapping is enabled */
#define kTXNRunIndexOutofBoundsErr 		(-22007)			/*  An index you supplied to a function is out of bounds */
#define kTXNNoMatchErr 					(-22008)			/*  Returned by TXNFind when a match is not found */
#define kTXNAttributeTagInvalidForRunErr  (-22009)			/*  Tag for a specific run is not valid (the tag's dataValue is set to this) */
#define kTXNSomeOrAllTagsInvalidForRunErr  (-22010)			/*  At least one of the tags given is invalid */
#define kTXNInvalidRunIndex 			(-22011)			/*  Index is out of range for that run */
#define kTXNAlreadyInitializedErr 		(-22012)			/*  You already called the TXNInitTextension function */
#define kTXNCannotTurnTSMOffWhenUsingUnicodeErr  (-22013)	/*  Your application tried to turn off the Text Services Manager when using Unicode */
#define kTXNCopyNotAllowedInEchoModeErr  (-22014)			/*  Your application tried to copy text that was in echo mode */
#define kTXNDataTypeNotAllowedErr 		(-22015)			/*  Your application specified a data type that MLTE does not allow */
#define kTXNATSUIIsNotInstalledErr 		(-22016)			/*  Indicates that ATSUI is not installed on the system */
#define kTXNOutsideOfLineErr 			(-22017)			/*  Indicates a value that is beyond the length of the line */
#define kTXNOutsideOfFrameErr 			(-22018)			/*  Indicates a value that is outside of the text object's frame */

#define printerStatusOpCodeNotSupportedErr  (-25280)
#define errKCNotAvailable 				(-25291)
#define errKCReadOnly 					(-25292)
#define errKCAuthFailed 				(-25293)
#define errKCNoSuchKeychain 			(-25294)
#define errKCInvalidKeychain 			(-25295)
#define errKCDuplicateKeychain 			(-25296)
#define errKCDuplicateCallback 			(-25297)
#define errKCInvalidCallback 			(-25298)
#define errKCDuplicateItem 				(-25299)
#define errKCItemNotFound 				(-25300)
#define errKCBufferTooSmall 			(-25301)
#define errKCDataTooLarge 				(-25302)
#define errKCNoSuchAttr 				(-25303)
#define errKCInvalidItemRef 			(-25304)
#define errKCInvalidSearchRef 			(-25305)
#define errKCNoSuchClass 				(-25306)
#define errKCNoDefaultKeychain 			(-25307)
#define errKCInteractionNotAllowed 		(-25308)
#define errKCReadOnlyAttr 				(-25309)
#define errKCWrongKCVersion 			(-25310)
#define errKCKeySizeNotAllowed 			(-25311)
#define errKCNoStorageModule 			(-25312)
#define errKCNoCertificateModule 		(-25313)
#define errKCNoPolicyModule 			(-25314)
#define errKCInteractionRequired 		(-25315)
#define errKCDataNotAvailable 			(-25316)
#define errKCDataNotModifiable 			(-25317)
#define errKCCreateChainFailed 			(-25318)

#define kUCOutputBufferTooSmall 		(-25340)			/*  Output buffer too small for Unicode string result */
#define kUCTextBreakLocatorMissingType 	(-25341)			/*  Unicode text break error */

#define kUCTSNoKeysAddedToObjectErr 	(-25342)
#define kUCTSSearchListErr 				(-25343)

#define kUCTokenizerIterationFinished 	(-25344)
#define kUCTokenizerUnknownLang 		(-25345)
#define kUCTokenNotFound 				(-25346)

#define kMPIterationEndErr 				(-29275)
#define kMPPrivilegedErr 				(-29276)
#define kMPProcessCreatedErr 			(-29288)
#define kMPProcessTerminatedErr 		(-29289)
#define kMPTaskCreatedErr 				(-29290)
#define kMPTaskBlockedErr 				(-29291)
#define kMPTaskStoppedErr 				(-29292)			/*  A convention used with MPThrowException. */
#define kMPBlueBlockingErr 				(-29293)
#define kMPDeletedErr 					(-29295)
#define kMPTimeoutErr 					(-29296)
#define kMPTaskAbortedErr 				(-29297)
#define kMPInsufficientResourcesErr 	(-29298)
#define kMPInvalidIDErr 				(-29299)

#define kMPNanokernelNeedsMemoryErr 	(-29294)
#define kCollateAttributesNotFoundErr 	(-29500)
#define kCollateInvalidOptions 			(-29501)
#define kCollateMissingUnicodeTableErr 	(-29502)
#define kCollateUnicodeConvertFailedErr  (-29503)
#define kCollatePatternNotFoundErr 		(-29504)
#define kCollateInvalidChar 			(-29505)
#define kCollateBufferTooSmall 			(-29506)
#define kCollateInvalidCollationRef 	(-29507)

#define kFNSInvalidReferenceErr 		(-29580)			/*  ref. was NULL or otherwise bad  */
#define kFNSBadReferenceVersionErr 		(-29581)			/*  ref. version is out of known range  */
#define kFNSInvalidProfileErr 			(-29582)			/*  profile is NULL or otherwise bad  */
#define kFNSBadProfileVersionErr 		(-29583)			/*  profile version is out of known range  */
#define kFNSDuplicateReferenceErr 		(-29584)			/*  the ref. being added is already in the profile  */
#define kFNSMismatchErr 				(-29585)			/*  reference didn't match or wasn't found in profile  */
#define kFNSInsufficientDataErr 		(-29586)			/*  insufficient data for the operation  */
#define kFNSBadFlattenedSizeErr 		(-29587)			/*  flattened size didn't match input or was too small  */
#define kFNSNameNotFoundErr 			(-29589)			/*  The name with the requested paramters was not found  */

#define kLocalesBufferTooSmallErr 		(-30001)
#define kLocalesTableFormatErr 			(-30002)
#define kLocalesDefaultDisplayStatus 	(-30029)			/*  Requested display locale unavailable, used default */

#define kALMInternalErr 				(-30049)
#define kALMGroupNotFoundErr 			(-30048)
#define kALMNoSuchModuleErr 			(-30047)
#define kALMModuleCommunicationErr 		(-30046)
#define kALMDuplicateModuleErr 			(-30045)
#define kALMInstallationErr 			(-30044)
#define kALMDeferSwitchErr 				(-30043)
#define kALMRebootFlagsLevelErr 		(-30042)

#define kALMLocationNotFoundErr 		(-30048)			/*  Old name  */
#define kSSpInternalErr 				(-30340)
#define kSSpVersionErr 					(-30341)
#define kSSpCantInstallErr 				(-30342)
#define kSSpParallelUpVectorErr 		(-30343)
#define kSSpScaleToZeroErr 				(-30344)

#define kNSpInitializationFailedErr 	(-30360)
#define kNSpAlreadyInitializedErr 		(-30361)
#define kNSpTopologyNotSupportedErr 	(-30362)
#define kNSpPipeFullErr 				(-30364)
#define kNSpHostFailedErr 				(-30365)
#define kNSpProtocolNotAvailableErr 	(-30366)
#define kNSpInvalidGameRefErr 			(-30367)
#define kNSpInvalidParameterErr 		(-30369)
#define kNSpOTNotPresentErr 			(-30370)
#define kNSpOTVersionTooOldErr 			(-30371)
#define kNSpMemAllocationErr 			(-30373)
#define kNSpAlreadyAdvertisingErr 		(-30374)
#define kNSpNotAdvertisingErr 			(-30376)
#define kNSpInvalidAddressErr 			(-30377)
#define kNSpFreeQExhaustedErr 			(-30378)
#define kNSpRemovePlayerFailedErr 		(-30379)
#define kNSpAddressInUseErr 			(-30380)
#define kNSpFeatureNotImplementedErr 	(-30381)
#define kNSpNameRequiredErr 			(-30382)
#define kNSpInvalidPlayerIDErr 			(-30383)
#define kNSpInvalidGroupIDErr 			(-30384)
#define kNSpNoPlayersErr 				(-30385)
#define kNSpNoGroupsErr 				(-30386)
#define kNSpNoHostVolunteersErr 		(-30387)
#define kNSpCreateGroupFailedErr 		(-30388)
#define kNSpAddPlayerFailedErr 			(-30389)
#define kNSpInvalidDefinitionErr 		(-30390)
#define kNSpInvalidProtocolRefErr 		(-30391)
#define kNSpInvalidProtocolListErr 		(-30392)
#define kNSpTimeoutErr 					(-30393)
#define kNSpGameTerminatedErr 			(-30394)
#define kNSpConnectFailedErr 			(-30395)
#define kNSpSendFailedErr 				(-30396)
#define kNSpMessageTooBigErr 			(-30397)
#define kNSpCantBlockErr 				(-30398)
#define kNSpJoinFailedErr 				(-30399)

#define kISpInternalErr 				(-30420)
#define kISpSystemListErr 				(-30421)
#define kISpBufferToSmallErr 			(-30422)
#define kISpElementInListErr 			(-30423)
#define kISpElementNotInListErr 		(-30424)
#define kISpSystemInactiveErr 			(-30425)
#define kISpDeviceInactiveErr 			(-30426)
#define kISpSystemActiveErr 			(-30427)
#define kISpDeviceActiveErr 			(-30428)
#define kISpListBusyErr 				(-30429)

#define kDSpNotInitializedErr 			(-30440)
#define kDSpSystemSWTooOldErr 			(-30441)
#define kDSpInvalidContextErr 			(-30442)
#define kDSpInvalidAttributesErr 		(-30443)
#define kDSpContextAlreadyReservedErr 	(-30444)
#define kDSpContextNotReservedErr 		(-30445)
#define kDSpContextNotFoundErr 			(-30446)
#define kDSpFrameRateNotReadyErr 		(-30447)
#define kDSpConfirmSwitchWarning 		(-30448)
#define kDSpInternalErr 				(-30449)
#define kDSpStereoContextErr 			(-30450)

#define kFBCvTwinExceptionErr 			(-30500)			/* no telling what it was */
#define kFBCnoIndexesFound 				(-30501)
#define kFBCallocFailed 				(-30502)			/* probably low memory */
#define kFBCbadParam 					(-30503)
#define kFBCfileNotIndexed 				(-30504)
#define kFBCbadIndexFile 				(-30505)			/* bad FSSpec, or bad data in file */
#define kFBCcompactionFailed 			(-30506)			/* V-Twin exception caught */
#define kFBCvalidationFailed 			(-30507)			/* V-Twin exception caught */
#define kFBCindexingFailed 				(-30508)			/* V-Twin exception caught */
#define kFBCcommitFailed 				(-30509)			/* V-Twin exception caught */
#define kFBCdeletionFailed 				(-30510)			/* V-Twin exception caught */
#define kFBCmoveFailed 					(-30511)			/* V-Twin exception caught */
#define kFBCtokenizationFailed 			(-30512)			/* couldn't read from document or query */
#define kFBCmergingFailed 				(-30513)			/* couldn't merge index files */
#define kFBCindexCreationFailed 		(-30514)			/* couldn't create index */
#define kFBCaccessorStoreFailed 		(-30515)
#define kFBCaddDocFailed 				(-30516)
#define kFBCflushFailed 				(-30517)
#define kFBCindexNotFound 				(-30518)
#define kFBCnoSearchSession 			(-30519)
#define kFBCindexingCanceled 			(-30520)
#define kFBCaccessCanceled 				(-30521)
#define kFBCindexFileDestroyed 			(-30522)
#define kFBCindexNotAvailable 			(-30523)
#define kFBCsearchFailed 				(-30524)
#define kFBCsomeFilesNotIndexed 		(-30525)
#define kFBCillegalSessionChange 		(-30526)			/* tried to add/remove vols to a session */
															/* that has hits */
#define kFBCanalysisNotAvailable 		(-30527)
#define kFBCbadIndexFileVersion 		(-30528)
#define kFBCsummarizationCanceled 		(-30529)
#define kFBCindexDiskIOFailed 			(-30530)
#define kFBCbadSearchSession 			(-30531)
#define kFBCnoSuchHit 					(-30532)

#define notAQTVRMovieErr 				(-30540)
#define constraintReachedErr 			(-30541)
#define callNotSupportedByNodeErr 		(-30542)
#define selectorNotSupportedByNodeErr 	(-30543)
#define invalidNodeIDErr 				(-30544)
#define invalidViewStateErr 			(-30545)
#define timeNotInViewErr 				(-30546)
#define propertyNotSupportedByNodeErr 	(-30547)
#define settingNotSupportedByNodeErr 	(-30548)
#define limitReachedErr 				(-30549)
#define invalidNodeFormatErr 			(-30550)
#define invalidHotSpotIDErr 			(-30551)
#define noMemoryNodeFailedInitialize 	(-30552)
#define streamingNodeNotReadyErr 		(-30553)
#define qtvrLibraryLoadErr 				(-30554)
#define qtvrUninitialized 				(-30555)

#define themeInvalidBrushErr 			(-30560)			/*  pattern index invalid  */
#define themeProcessRegisteredErr 		(-30561)
#define themeProcessNotRegisteredErr 	(-30562)
#define themeBadTextColorErr 			(-30563)
#define themeHasNoAccentsErr 			(-30564)
#define themeBadCursorIndexErr 			(-30565)
#define themeScriptFontNotFoundErr 		(-30566)			/*  theme font requested for uninstalled script system  */
#define themeMonitorDepthNotSupportedErr  (-30567)			/*  theme not supported at monitor depth  */
#define themeNoAppropriateBrushErr 		(-30568)			/*  theme brush has no corresponding theme text color  */

#define errMessageNotSupported 			(-30580)
#define errDataNotSupported 			(-30581)
#define errControlDoesntSupportFocus 	(-30582)
#define errUnknownControl 				(-30584)
#define errCouldntSetFocus 				(-30585)
#define errNoRootControl 				(-30586)
#define errRootAlreadyExists 			(-30587)
#define errInvalidPartCode 				(-30588)
#define errControlsAlreadyExist 		(-30589)
#define errControlIsNotEmbedder 		(-30590)
#define errDataSizeMismatch 			(-30591)
#define errControlHiddenOrDisabled 		(-30592)
#define errCantEmbedIntoSelf 			(-30594)
#define errCantEmbedRoot 				(-30595)
#define errItemNotControl 				(-30596)
#define controlInvalidDataVersionErr 	(-30597)
#define controlPropertyInvalid 			(-5603)
#define controlPropertyNotFoundErr 		(-5604)
#define controlHandleInvalidErr 		(-30599)

#define kURLInvalidURLReferenceError 	(-30770)
#define kURLProgressAlreadyDisplayedError  (-30771)
#define kURLDestinationExistsError 		(-30772)
#define kURLInvalidURLError 			(-30773)
#define kURLUnsupportedSchemeError 		(-30774)
#define kURLServerBusyError 			(-30775)
#define kURLAuthenticationError 		(-30776)
#define kURLPropertyNotYetKnownError 	(-30777)
#define kURLUnknownPropertyError 		(-30778)
#define kURLPropertyBufferTooSmallError  (-30779)
#define kURLUnsettablePropertyError 	(-30780)
#define kURLInvalidCallError 			(-30781)
#define kURLFileEmptyError 				(-30783)
#define kURLExtensionFailureError 		(-30785)
#define kURLInvalidConfigurationError 	(-30786)
#define kURLAccessNotAvailableError 	(-30787)
#define kURL68kNotSupportedError 		(-30788)

#define errCppGeneral 					(-32000)
#define errCppbad_alloc 				(-32001)			/*  thrown by new  */
#define errCppbad_cast 					(-32002)			/*  thrown by dynamic_cast when fails with a referenced type  */
#define errCppbad_exception 			(-32003)			/*  thrown when an exception doesn't match any catch  */
#define errCppbad_typeid 				(-32004)			/*  thrown by typeid  */
#define errCpplogic_error 				(-32005)
#define errCppdomain_error 				(-32006)
#define errCppinvalid_argument 			(-32007)
#define errCpplength_error 				(-32008)
#define errCppout_of_range 				(-32009)
#define errCppruntime_error 			(-32010)
#define errCppoverflow_error 			(-32011)
#define errCpprange_error 				(-32012)
#define errCppunderflow_error 			(-32013)
#define errCppios_base_failure 			(-32014)
#define errCppLastSystemDefinedError 	(-32020)
#define errCppLastUserDefinedError 		(-32049)			/*  -32021 through -32049 are free for developer-defined exceptions */

#define badComponentInstance 			0x80008001			/*  when cast to an OSErr this is -32767 */
#define badComponentSelector 			0x80008002			/*  when cast to an OSErr this is -32766 */

#define dsBusError 						1					/* bus error */
#define dsAddressErr 					2					/* address error */
#define dsIllInstErr 					3					/* illegal instruction error */
#define dsZeroDivErr 					4					/* zero divide error */
#define dsChkErr 						5					/* check trap error */
#define dsOvflowErr 					6					/* overflow trap error */
#define dsPrivErr 						7					/* privilege violation error */
#define dsTraceErr 						8					/* trace mode error */
#define dsLineAErr 						9					/* line 1010 trap error */
#define dsLineFErr 						10					/* line 1111 trap error */
#define dsMiscErr 						11					/* miscellaneous hardware exception error */
#define dsCoreErr 						12					/* unimplemented core routine error */
#define dsIrqErr 						13					/* uninstalled interrupt error */
#define dsIOCoreErr 					14					/* IO Core Error */
#define dsLoadErr 						15					/* Segment Loader Error */
#define dsFPErr 						16					/* Floating point error */
#define dsNoPackErr 					17					/* package 0 not present */
#define dsNoPk1 						18					/* package 1 not present */
#define dsNoPk2 						19					/* package 2 not present */

#define dsNoPk3 						20					/* package 3 not present */
#define dsNoPk4 						21					/* package 4 not present */
#define dsNoPk5 						22					/* package 5 not present */
#define dsNoPk6 						23					/* package 6 not present */
#define dsNoPk7 						24					/* package 7 not present */
#define dsMemFullErr 					25					/* out of memory! */
#define dsBadLaunch 					26					/* can't launch file */
#define dsFSErr 						27					/* file system map has been trashed */
#define dsStknHeap 						28					/* stack has moved into application heap */
#define negZcbFreeErr 					33					/* ZcbFree has gone negative */
#define dsFinderErr 					41					/* can't load the Finder error */
#define dsBadSlotInt 					51					/* unserviceable slot interrupt */
#define dsBadSANEOpcode 				81					/* bad opcode given to SANE Pack4 */
#define dsBadPatchHeader 				83					/* SetTrapAddress saw the “come-from” header */
#define menuPrgErr 						84					/* happens when a menu is purged */
#define dsMBarNFnd 						85					/* Menu Manager Errors */
#define dsHMenuFindErr 					86					/* Menu Manager Errors */
#define dsWDEFNotFound 					87					/* could not load WDEF */
#define dsCDEFNotFound 					88					/* could not load CDEF */
#define dsMDEFNotFound 					89					/* could not load MDEF */

#define dsNoFPU 						90					/* an FPU instruction was executed and the machine doesn’t have one */
#define dsNoPatch 						98					/* Can't patch for particular Model Mac */
#define dsBadPatch 						99					/* Can't load patch resource */
#define dsParityErr 					101					/* memory parity error */
#define dsOldSystem 					102					/* System is too old for this ROM */
#define ds32BitMode 					103					/* booting in 32-bit on a 24-bit sys */
#define dsNeedToWriteBootBlocks 		104					/* need to write new boot blocks */
#define dsNotEnoughRAMToBoot 			105					/* must have at least 1.5MB of RAM to boot 7.0 */
#define dsBufPtrTooLow 					106					/* bufPtr moved too far during boot */
#define dsVMDeferredFuncTableFull 		112					/* VM's DeferUserFn table is full */
#define dsVMBadBackingStore 			113					/* Error occurred while reading or writing the VM backing-store file */
#define dsCantHoldSystemHeap 			114					/* Unable to hold the system heap during boot */
#define dsSystemRequiresPowerPC 		116					/* Startup disk requires PowerPC */
#define dsGibblyMovedToDisabledFolder 	117					/*  For debug builds only, signals that active gibbly was disabled during boot.  */
#define dsUnBootableSystem 				118					/*  Active system file will not boot on this system because it was designed only to boot from a CD.  */
#define dsMustUseFCBAccessors 			119					/*  FCBSPtr and FSFCBLen are invalid - must use FSM FCB accessor functions  */
#define dsMacOSROMVersionTooOld 		120					/*  The version of the "Mac OS ROM" file is too old to be used with the installed version of system software  */
#define dsLostConnectionToNetworkDisk 	121					/*  Lost communication with Netboot server  */
#define dsRAMDiskTooBig 				122					/*  The RAM disk is too big to boot safely; will be turned off  */
#define dsWriteToSupervisorStackGuardPage  128				/* the supervisor stack overflowed into its guard page  */
#define dsReinsert 						30					/* request user to reinsert off-line volume */
#define shutDownAlert 					42					/* handled like a shutdown error */
#define dsShutDownOrRestart 			20000				/* user choice between ShutDown and Restart */
#define dsSwitchOffOrRestart 			20001				/* user choice between switching off and Restart */
#define dsForcedQuit 					20002				/* allow the user to ExitToShell, return if Cancel */
#define dsRemoveDisk 					20003				/* request user to remove disk from manual eject drive */
#define dsDirtyDisk 					20004				/* request user to return a manually-ejected dirty disk */
#define dsShutDownOrResume 				20109				/* allow user to return to Finder or ShutDown */
#define dsSCSIWarn 						20010				/* Portable SCSI adapter warning. */
#define dsMBSysError 					29200				/* Media Bay replace warning. */
#define dsMBFlpySysError 				29201				/* Media Bay, floppy replace warning. */
#define dsMBATASysError 				29202				/* Media Bay, ATA replace warning. */
#define dsMBATAPISysError 				29203				/* Media Bay, ATAPI replace warning... */
#define dsMBExternFlpySysError 			29204				/* Media Bay, external floppy drive reconnect warning */
#define dsPCCardATASysError 			29205				/* PCCard has been ejected while still in use.  */

#define dsNoExtsMacsBug 				(-1)				/* not a SysErr, just a placeholder  */
#define dsNoExtsDisassembler 			(-2)				/* not a SysErr, just a placeholder  */
#define dsMacsBugInstalled 				(-10)				/* say “MacsBug Installed” */
#define dsDisassemblerInstalled 		(-11)				/* say “Disassembler Installed” */
#define dsExtensionsDisabled 			(-13)				/* say “Extensions Disabled” */
#define dsGreeting 						40					/* welcome to Macintosh greeting */
#define dsSysErr 						32767				/* general system error */
															/* old names here for compatibility’s sake */
#define WDEFNFnd 						87

#define CDEFNFnd 						88
#define dsNotThe1 						31					/* not the disk I wanted */
#define dsBadStartupDisk 				42					/* unable to mount boot volume (sad Mac only) */
#define dsSystemFileErr 				43					/* can’t find System file to open (sad Mac only) */
#define dsHD20Installed 				(-12)				/* say “HD20 Startup” */
#define mBarNFnd 						(-126)				/* system error code for MBDF not found */
#define fsDSIntErr 						(-127)				/* non-hardware Internal file system error */
#define hMenuFindErr 					(-127)				/* could not find HMenu's parent in MenuKey (wrong error code - obsolete) */
#define userBreak 						(-490)				/* user debugger break */
#define strUserBreak 					(-491)				/* user debugger break; display string on stack */
#define exUserBreak 					(-492)				/* user debugger break; execute debugger commands on stack */

															/*  DS Errors which are specific to the new runtime model introduced with PowerPC  */
#define dsBadLibrary 					1010				/*  Bad shared library  */
#define dsMixedModeFailure 				1011				/*  Internal Mixed Mode Failure  */

#define kPOSIXErrorBase 				100000
#define kPOSIXErrorEPERM 				100001				/*  Operation not permitted  */
#define kPOSIXErrorENOENT 				100002				/*  No such file or directory  */
#define kPOSIXErrorESRCH 				100003				/*  No such process  */
#define kPOSIXErrorEINTR 				100004				/*  Interrupted system call  */
#define kPOSIXErrorEIO 					100005				/*  Input/output error  */
#define kPOSIXErrorENXIO 				100006				/*  Device not configured  */
#define kPOSIXErrorE2BIG 				100007				/*  Argument list too long  */
#define kPOSIXErrorENOEXEC 				100008				/*  Exec format error  */
#define kPOSIXErrorEBADF 				100009				/*  Bad file descriptor  */
#define kPOSIXErrorECHILD 				100010				/*  No child processes  */
#define kPOSIXErrorEDEADLK 				100011				/*  Resource deadlock avoided  */
#define kPOSIXErrorENOMEM 				100012				/*  Cannot allocate memory  */
#define kPOSIXErrorEACCES 				100013				/*  Permission denied  */
#define kPOSIXErrorEFAULT 				100014				/*  Bad address  */
#define kPOSIXErrorENOTBLK 				100015				/*  Block device required  */
#define kPOSIXErrorEBUSY 				100016				/*  Device busy  */
#define kPOSIXErrorEEXIST 				100017				/*  File exists  */
#define kPOSIXErrorEXDEV 				100018				/*  Cross-device link  */
#define kPOSIXErrorENODEV 				100019				/*  Operation not supported by device  */
#define kPOSIXErrorENOTDIR 				100020				/*  Not a directory  */
#define kPOSIXErrorEISDIR 				100021				/*  Is a directory  */
#define kPOSIXErrorEINVAL 				100022				/*  Invalid argument  */
#define kPOSIXErrorENFILE 				100023				/*  Too many open files in system  */
#define kPOSIXErrorEMFILE 				100024				/*  Too many open files  */
#define kPOSIXErrorENOTTY 				100025				/*  Inappropriate ioctl for device  */
#define kPOSIXErrorETXTBSY 				100026				/*  Text file busy  */
#define kPOSIXErrorEFBIG 				100027				/*  File too large  */
#define kPOSIXErrorENOSPC 				100028				/*  No space left on device  */
#define kPOSIXErrorESPIPE 				100029				/*  Illegal seek  */
#define kPOSIXErrorEROFS 				100030				/*  Read-only file system  */
#define kPOSIXErrorEMLINK 				100031				/*  Too many links  */
#define kPOSIXErrorEPIPE 				100032				/*  Broken pipe  */
#define kPOSIXErrorEDOM 				100033				/*  Numerical argument out of domain  */
#define kPOSIXErrorERANGE 				100034				/*  Result too large  */
#define kPOSIXErrorEAGAIN 				100035				/*  Resource temporarily unavailable  */
#define kPOSIXErrorEINPROGRESS 			100036				/*  Operation now in progress  */
#define kPOSIXErrorEALREADY 			100037				/*  Operation already in progress  */
#define kPOSIXErrorENOTSOCK 			100038				/*  Socket operation on non-socket  */
#define kPOSIXErrorEDESTADDRREQ 		100039				/*  Destination address required  */
#define kPOSIXErrorEMSGSIZE 			100040				/*  Message too long  */
#define kPOSIXErrorEPROTOTYPE 			100041				/*  Protocol wrong type for socket  */
#define kPOSIXErrorENOPROTOOPT 			100042				/*  Protocol not available  */
#define kPOSIXErrorEPROTONOSUPPORT 		100043				/*  Protocol not supported  */
#define kPOSIXErrorESOCKTNOSUPPORT 		100044				/*  Socket type not supported  */
#define kPOSIXErrorENOTSUP 				100045				/*  Operation not supported  */
#define kPOSIXErrorEPFNOSUPPORT 		100046				/*  Protocol family not supported  */
#define kPOSIXErrorEAFNOSUPPORT 		100047				/*  Address family not supported by protocol family  */
#define kPOSIXErrorEADDRINUSE 			100048				/*  Address already in use  */
#define kPOSIXErrorEADDRNOTAVAIL 		100049				/*  Can't assign requested address  */
#define kPOSIXErrorENETDOWN 			100050				/*  Network is down  */
#define kPOSIXErrorENETUNREACH 			100051				/*  Network is unreachable  */
#define kPOSIXErrorENETRESET 			100052				/*  Network dropped connection on reset  */
#define kPOSIXErrorECONNABORTED 		100053				/*  Software caused connection abort  */
#define kPOSIXErrorECONNRESET 			100054				/*  Connection reset by peer  */
#define kPOSIXErrorENOBUFS 				100055				/*  No buffer space available  */
#define kPOSIXErrorEISCONN 				100056				/*  Socket is already connected  */
#define kPOSIXErrorENOTCONN 			100057				/*  Socket is not connected  */
#define kPOSIXErrorESHUTDOWN 			100058				/*  Can't send after socket shutdown  */
#define kPOSIXErrorETOOMANYREFS 		100059				/*  Too many references: can't splice  */
#define kPOSIXErrorETIMEDOUT 			100060				/*  Operation timed out  */
#define kPOSIXErrorECONNREFUSED 		100061				/*  Connection refused  */
#define kPOSIXErrorELOOP 				100062				/*  Too many levels of symbolic links  */
#define kPOSIXErrorENAMETOOLONG 		100063				/*  File name too long  */
#define kPOSIXErrorEHOSTDOWN 			100064				/*  Host is down  */
#define kPOSIXErrorEHOSTUNREACH 		100065				/*  No route to host  */
#define kPOSIXErrorENOTEMPTY 			100066				/*  Directory not empty  */
#define kPOSIXErrorEPROCLIM 			100067				/*  Too many processes  */
#define kPOSIXErrorEUSERS 				100068				/*  Too many users  */
#define kPOSIXErrorEDQUOT 				100069				/*  Disc quota exceeded  */
#define kPOSIXErrorESTALE 				100070				/*  Stale NFS file handle  */
#define kPOSIXErrorEREMOTE 				100071				/*  Too many levels of remote in path  */
#define kPOSIXErrorEBADRPC 				100072				/*  RPC struct is bad  */
#define kPOSIXErrorERPCMISMATCH 		100073				/*  RPC version wrong  */
#define kPOSIXErrorEPROGUNAVAIL 		100074				/*  RPC prog. not avail  */
#define kPOSIXErrorEPROGMISMATCH 		100075				/*  Program version wrong  */
#define kPOSIXErrorEPROCUNAVAIL 		100076				/*  Bad procedure for program  */
#define kPOSIXErrorENOLCK 				100077				/*  No locks available  */
#define kPOSIXErrorENOSYS 				100078				/*  Function not implemented  */
#define kPOSIXErrorEFTYPE 				100079				/*  Inappropriate file type or format  */
#define kPOSIXErrorEAUTH 				100080				/*  Authentication error  */
#define kPOSIXErrorENEEDAUTH 			100081				/*  Need authenticator  */
#define kPOSIXErrorEPWROFF 				100082				/*  Device power is off  */
#define kPOSIXErrorEDEVERR 				100083				/*  Device error, e.g. paper out  */
#define kPOSIXErrorEOVERFLOW 			100084				/*  Value too large to be stored in data type  */
#define kPOSIXErrorEBADEXEC 			100085				/*  Bad executable  */
#define kPOSIXErrorEBADARCH 			100086				/*  Bad CPU type in executable  */
#define kPOSIXErrorESHLIBVERS 			100087				/*  Shared library version mismatch  */
#define kPOSIXErrorEBADMACHO 			100088				/*  Malformed Macho file  */
#define kPOSIXErrorECANCELED 			100089				/*  Operation canceled  */
#define kPOSIXErrorEIDRM 				100090				/*  Identifier removed  */
#define kPOSIXErrorENOMSG 				100091				/*  No message of desired type  */
#define kPOSIXErrorEILSEQ 				100092				/*  Illegal byte sequence  */
#define kPOSIXErrorENOATTR 				100093				/*  Attribute not found  */
#define kPOSIXErrorEBADMSG 				100094				/*  Bad message  */
#define kPOSIXErrorEMULTIHOP 			100095				/*  Reserved  */
#define kPOSIXErrorENODATA 				100096				/*  No message available on STREAM  */
#define kPOSIXErrorENOLINK 				100097				/*  Reserved  */
#define kPOSIXErrorENOSR 				100098				/*  No STREAM resources  */
#define kPOSIXErrorENOSTR 				100099				/*  Not a STREAM  */
#define kPOSIXErrorEPROTO 				100100				/*  Protocol error  */
#define kPOSIXErrorETIME 				100101				/*  STREAM ioctl timeout  */
#define kPOSIXErrorEOPNOTSUPP 			100102				/*  Operation not supported on socket  */


#endif /* __MACERRORS_R__ */

