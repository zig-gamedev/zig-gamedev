#if (defined(__USE_PUBLIC_HEADERS__) && __USE_PUBLIC_HEADERS__) || (defined(USE_AUDIOTOOLBOX_PUBLIC_HEADERS) && USE_AUDIOTOOLBOX_PUBLIC_HEADERS) || !__has_include(<AudioToolboxCore/AudioFileComponent.h>)
/*!
	@file		AudioFileComponent.h
	@framework	AudioToolbox.framework
	@copyright	(c) 2004-2015 by Apple, Inc., all rights reserved.

	@abstract	Interfaces for components which implement knowledge of audio file formats.
    @discussion
    	Audio file components are not for the use of clients. Rather, they are called by the
    	implementation of the AudioFile API to implement the various semantics of that API.
		Most of these calls match a call in the AudioFile API which calls through to the component.

		A component may be used in two ways, either associated with a file or not. If a component is
		not associated with a file, it may be used to answer questions about the file type in
		general and whether some data is recognized by that file type. A component is associated
		with a file by calling one of the AudioFile Create, Open or Initialize calls. If a component
		is associated with a file, then it can also be asked to perform any of the calls that
		implement the AudioFile API.
*/

#ifndef AudioToolbox_AudioFileComponent_h
#define AudioToolbox_AudioFileComponent_h

//==================================================================================================
//	Includes
//==================================================================================================

#include <AudioToolbox/AudioComponent.h>
#include <AudioToolbox/AudioFile.h>

CF_ASSUME_NONNULL_BEGIN

#if defined(__cplusplus)
extern "C"
{
#endif

/*!
    @typedef	AudioFileComponent
    @abstract		represents an instance of an AudioFileComponent.
*/
typedef AudioComponentInstance	AudioFileComponent;
/*!
    @typedef	AudioFileComponentPropertyID
    @abstract		a four char code for a property ID.
*/
typedef UInt32				AudioFileComponentPropertyID;


/*!
    @function	AudioFileComponentCreateURL
    @abstract   creates a new (or initialises an existing) audio file specified by the URL.
    @discussion	creates a new (or initialises an existing) audio file specified by the URL.
    @param inComponent		an AudioFileComponent
    @param inFileRef		an CFURLRef fully specifying the path of the file to create/initialise
    @param inFormat			an AudioStreamBasicDescription describing the data format that will be
							added to the audio file.
    @param inFlags			relevant flags for creating/opening the file. 
								if kAudioFileFlags_EraseFile is set, it will erase an existing file
								 if not set, then the Create call will fail if the URL is an existing file
    @result					returns noErr if successful.
*/
extern OSStatus	
AudioFileComponentCreateURL (
							AudioFileComponent					inComponent,
							CFURLRef							inFileRef,
							const AudioStreamBasicDescription	*inFormat,
							UInt32								inFlags)		API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function				AudioFileComponentOpenURL
    @abstract				Open an existing audio file.
    @discussion				Open an existing audio file for reading or reading and writing.
    @param inComponent		an AudioFileComponent.
    @param inFileRef		the CFURLRef of an existing audio file.
    @param inPermissions	use the permission constants.
    @param inFileDescriptor	an open file descriptor.
    @result					returns noErr if successful.
*/
extern OSStatus	
AudioFileComponentOpenURL (	
					AudioFileComponent			inComponent,
					CFURLRef					inFileRef, 
					SInt8						inPermissions, 
					int							inFileDescriptor)				API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function	AudioFileComponentOpenWithCallbacks
    @abstract   implements AudioFileOpenWithCallbacks
    @param inComponent		an AudioFileComponent
    @param inClientData 	a constant that will be passed to your callbacks.
	@param inReadFunc		a function that will be called when AudioFile needs to read data.
	@param inWriteFunc		a function that will be called when AudioFile needs to write data.
	@param inGetSizeFunc	a function that will be called when AudioFile needs to know the file size.
	@param inSetSizeFunc	a function that will be called when AudioFile needs to set the file size.
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentOpenWithCallbacks(
								AudioFileComponent					inComponent,
								void								*inClientData, 
								AudioFile_ReadProc					inReadFunc, 
								AudioFile_WriteProc					inWriteFunc, 
								AudioFile_GetSizeProc				inGetSizeFunc,
								AudioFile_SetSizeProc				inSetSizeFunc)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function	AudioFileComponentInitializeWithCallbacks
    @abstract   implements AudioFileInitializeWithCallbacks
    @param inComponent		an AudioFileComponent
    @param inClientData 	a constant that will be passed to your callbacks.
	@param inReadFunc		a function that will be called when AudioFile needs to read data.
	@param inWriteFunc		a function that will be called when AudioFile needs to write data.
	@param inGetSizeFunc	a function that will be called when AudioFile needs to know the file size.
	@param inSetSizeFunc	a function that will be called when AudioFile needs to set the file size.
    @param inFileType 		an AudioFileTypeID indicating the type of audio file to which to initialize the file. 
    @param inFormat			an AudioStreamBasicDescription describing the data format that will be
							added to the audio file.
    @param inFlags			relevant flags for creating/opening the file. Currently zero.
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentInitializeWithCallbacks(
								AudioFileComponent					inComponent,
								void								*inClientData, 
								AudioFile_ReadProc					inReadFunc, 
								AudioFile_WriteProc					inWriteFunc, 
								AudioFile_GetSizeProc				inGetSizeFunc,
								AudioFile_SetSizeProc				inSetSizeFunc,
								UInt32								inFileType,
								const AudioStreamBasicDescription	*inFormat,
								UInt32								inFlags)		API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);


/*!
    @function	AudioFileComponentCloseFile
    @abstract   implements AudioFileClose.
    @param inComponent		an AudioFileComponent
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentCloseFile(
								AudioFileComponent					inComponent)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);
				
/*!
    @function	AudioFileComponentOptimize
    @abstract   implements AudioFileOptimize.
    @param inComponent		an AudioFileComponent
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentOptimize(
								AudioFileComponent					inComponent)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);
				
/*!
    @function	AudioFileComponentReadBytes
    @abstract   implements AudioFileReadBytes. 
				
    @discussion				Returns kAudioFileEndOfFileError when read encounters end of file.
    @param inComponent		an AudioFileComponent
    @param inUseCache 		true if it is desired to cache the data upon read, else false
    @param inStartingByte	the byte offset of the audio data desired to be returned
    @param ioNumBytes 		on input, the number of bytes to read, on output, the number of
							bytes actually read.
    @param outBuffer 		outBuffer should be a void * to user allocated memory large enough for the requested bytes. 
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentReadBytes(		
								AudioFileComponent				inComponent,
								Boolean							inUseCache,
								SInt64							inStartingByte, 
								UInt32							*ioNumBytes, 
								void							*outBuffer)		API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);
						
/*!
    @function				AudioFileComponentWriteBytes
    @abstract				implements AudioFileWriteBytes.
    @param inComponent		an AudioFileComponent
    @param inUseCache 		true if it is desired to cache the data upon write, else false
    @param inStartingByte	the byte offset where the audio data should be written
    @param ioNumBytes 		on input, the number of bytes to write, on output, the number of
							bytes actually written.
    @param inBuffer 		inBuffer should be a void * containing the bytes to be written 
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentWriteBytes(		
								AudioFileComponent				inComponent,
								Boolean							inUseCache,
								SInt64							inStartingByte, 
								UInt32							*ioNumBytes, 
								const void						*inBuffer)		API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);
						
/*!
    @function	AudioFileComponentReadPackets
    @abstract   implements AudioFileReadPackets.
    @discussion For all uncompressed formats, packets == frames.
				ioNumPackets less than requested indicates end of file.

    @param inComponent				an AudioFileComponent
    @param inUseCache 				true if it is desired to cache the data upon read, else false
    @param outNumBytes				on output, the number of bytes actually returned
    @param outPacketDescriptions 	on output, an array of packet descriptions describing
									the packets being returned. NULL may be passed for this
									parameter. Nothing will be returned for linear pcm data.   
    @param inStartingPacket 		the packet index of the first packet desired to be returned
    @param ioNumPackets 			on input, the number of packets to read, on output, the number of
									packets actually read.
    @param outBuffer 				outBuffer should be a pointer to user allocated memory of size: 
									number of packets requested times file's maximum (or upper bound on)
									packet size.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentReadPackets(		
								AudioFileComponent				inComponent,
								Boolean							inUseCache,
								UInt32							*outNumBytes,
								AudioStreamPacketDescription * __nullable outPacketDescriptions,
								SInt64							inStartingPacket, 
								UInt32  						*ioNumPackets, 
								void							*outBuffer)		API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);
									

/*!
    @function	AudioFileComponentReadPacketData
    @abstract   implements AudioFileReadPacketData.
    @discussion For all uncompressed formats, packets == frames.
				If the byte size of the number packets requested is 
				less than the buffer size, ioNumBytes will be reduced.
				If the buffer is too small for the number of packets 
				requested, ioNumPackets and ioNumBytes will be reduced 
				to the number of packets that can be accommodated and their byte size.
				Returns kAudioFileEndOfFileError when read encounters end of file.

    @param inComponent				an AudioFileComponent
    @param inUseCache 				true if it is desired to cache the data upon read, else false
    @param ioNumBytes				on input the size of outBuffer in bytes. 
									on output, the number of bytes actually returned.
    @param outPacketDescriptions 	on output, an array of packet descriptions describing
									the packets being returned. NULL may be passed for this
									parameter. Nothing will be returned for linear pcm data.   
    @param inStartingPacket 		the packet index of the first packet desired to be returned
    @param ioNumPackets 			on input, the number of packets to read, on output, the number of
									packets actually read.
    @param outBuffer 				outBuffer should be a pointer to user allocated memory.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentReadPacketData(		
								AudioFileComponent				inComponent,
								Boolean							inUseCache,
								UInt32							*ioNumBytes,
								AudioStreamPacketDescription * __nullable outPacketDescriptions,
								SInt64							inStartingPacket, 
								UInt32  						*ioNumPackets, 
								void							*outBuffer)		API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);
									
/*!
    @function	AudioFileComponentWritePackets
    @abstract   implements AudioFileWritePackets.
    @discussion For all uncompressed formats, packets == frames.
    @param inComponent				an AudioFileComponent
    @param inUseCache 				true if it is desired to cache the data upon write, else false
    @param inNumBytes				the number of bytes being provided for write
    @param inPacketDescriptions 	an array of packet descriptions describing the packets being 
									provided. Not all formats require packet descriptions to be 
									provided. NULL may be passed if no descriptions are required.   
    @param inStartingPacket 		the packet index of where the first packet provided should be placed.
    @param ioNumPackets 			on input, the number of packets to write, on output, the number of
									packets actually written.
    @param inBuffer 				a void * to user allocated memory containing the packets to write.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentWritePackets(	
								AudioFileComponent					inComponent,
								Boolean								inUseCache,
								UInt32								inNumBytes,
								const AudioStreamPacketDescription * __nullable inPacketDescriptions,
								SInt64								inStartingPacket, 
								UInt32								*ioNumPackets, 
								const void							*inBuffer)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);


/*!
    @function	AudioFileComponentGetPropertyInfo
    @abstract   implements AudioFileGetPropertyInfo.
    @param		inComponent			an AudioFileComponent
    @param      inPropertyID		an AudioFileProperty constant.
    @param      outPropertySize		the size in bytes of the current value of the property. In order to get the property value, 
									you will need a buffer of this size.
    @param      outWritable			will be set to 1 if writable, or 0 if read only.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentGetPropertyInfo(	
								AudioFileComponent				inComponent,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32 * __nullable				outPropertySize,
								UInt32 * __nullable				outWritable)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);


/*!
    @function	AudioFileComponentGetProperty
    @abstract   implements AudioFileGetProperty.
    @param		inComponent			an AudioFileComponent
    @param      inPropertyID		an AudioFileProperty constant.
    @param      ioPropertyDataSize	on input the size of the outPropertyData buffer. On output the number of bytes written to the buffer.
    @param      outPropertyData		the buffer in which to write the property data.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentGetProperty(	
								AudioFileComponent				inComponent,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							*ioPropertyDataSize,
								void							*outPropertyData)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function	AudioFileComponentSetProperty
    @abstract   implements AudioFileSetProperty.
    @param		inComponent			an AudioFileComponent
    @param      inPropertyID		an AudioFileProperty constant.
    @param      inPropertyDataSize	the size of the property data.
    @param      inPropertyData		the buffer containing the property data.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentSetProperty(	
								AudioFileComponent				inComponent,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							inPropertyDataSize,
								const void						*inPropertyData)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);


/*!
    @function	AudioFileComponentCountUserData
    @abstract   implements AudioFileCountUserData
    @discussion		"User Data" refers to chunks in AIFF, CAF and WAVE files, or resources 
					in Sound Designer II files, and possibly other things in other files.
					For simplicity, referred to below as "chunks".
    @param inComponent				an AudioFileComponent
    @param inUserDataID				the four char code of the chunk.
    @param outNumberItems			on output, if successful, number of chunks of this type in the file.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentCountUserData(	
								AudioFileComponent				inComponent,
								UInt32							inUserDataID,
								UInt32							*outNumberItems)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function	AudioFileComponentGetUserDataSize
    @abstract   implements AudioFileGetUserDataSize
    @param inComponent				an AudioFileComponent
    @param inUserDataID				the four char code of the chunk.
    @param inIndex					an index specifying which chunk if there are more than one.
    @param outUserDataSize			on output, if successful, the size of the user data chunk.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentGetUserDataSize(	
								AudioFileComponent				inComponent,
								UInt32							inUserDataID,
								UInt32							inIndex,
								UInt32							*outUserDataSize)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function	AudioFileGetUserData
    @abstract   implements AudioFileGetUserData.
    @param		inComponent			an AudioFileComponent
    @param      inUserDataID		the four char code of the chunk.
    @param      inIndex				an index specifying which chunk if there are more than one.
	@param		ioUserDataSize		the size of the buffer on input, size of bytes copied to buffer on output 
    @param      outUserData			a pointer to a buffer in which to copy the chunk data.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentGetUserData(	
								AudioFileComponent				inComponent,
								UInt32							inUserDataID,
								UInt32							inIndex,
								UInt32							*ioUserDataSize,
								void							*outUserData)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function	AudioFileComponentSetUserData
    @abstract   implements AudioFileSetUserData.
    @param		inComponent			an AudioFileComponent
    @param      inUserDataID		the four char code of the chunk.
    @param      inIndex				an index specifying which chunk if there are more than one.
	@param		inUserDataSize		on input the size of the data to copy, on output, size of bytes copied from the buffer  
    @param      inUserData			a pointer to a buffer from which to copy the chunk data 
									(only the contents of the chunk, not including the chunk header).
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentSetUserData(	
								AudioFileComponent				inComponent,
								UInt32							inUserDataID,
								UInt32							inIndex,
								UInt32							inUserDataSize,
								const void						*inUserData)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);


/*!
    @function	AudioFileComponentRemoveUserData
    @abstract   implements AudioFileRemoveUserData.
    @param		inComponent			an AudioFileComponent
    @param      inUserDataID		the four char code of the chunk.
    @param      inIndex				an index specifying which chunk if there are more than one.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentRemoveUserData(	
								AudioFileComponent				inComponent,
								UInt32							inUserDataID,
								UInt32							inIndex)		API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//	The following calls are not made on AudioFile instances.
//  These calls are used to determine the audio file type of some data. 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
/*!
    @function	AudioFileComponentExtensionIsThisFormat
    @abstract   used by the AudioFile API to determine if this component is appropriate for handling a file.
    @param		inComponent			an AudioFileComponent
    @param      inExtension			a CFString containing a file name extension.
    @param      outResult			on output, is set to 1 if the extension is recognized by this component, 0 if not.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentExtensionIsThisFormat(	
								AudioFileComponent				inComponent,
								CFStringRef 					inExtension,
								UInt32							*outResult)		API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);


/*!
    @function	AudioFileComponentFileDataIsThisFormat
    @abstract   used by the AudioFile API to determine if this component is appropriate for handling a file.
    @param		inComponent			an AudioFileComponent
    @param      inDataByteSize		the size of inData in bytes.
    @param      inData				a pointer to a buffer of audio file data.
    @param      outResult			on output, is set to 1 if the file is recognized by this component, 0 if not.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentFileDataIsThisFormat(	
								AudioFileComponent				inComponent,
								UInt32							inDataByteSize,
								const void*						inData,
								UInt32							*outResult)		API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//	The following two calls are deprecated. 
//  Please implement AudioFileComponentFileDataIsThisFormat instead.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
/*!
    @function	AudioFileComponentFileIsThisFormat
    @abstract   deprecated. use AudioFileComponentFileDataIsThisFormat instead.
    @param		inComponent			an AudioFileComponent
    @param      inFileRefNum		a refNum of a file.
    @param      outResult			on output, is set to 1 if the file is recognized by this component, 0 if not.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentFileIsThisFormat(	
								AudioFileComponent				inComponent,
								SInt16							inFileRefNum,
								UInt32							*outResult)		API_DEPRECATED("no longer supported", macos(10.4, 10.5)) API_UNAVAILABLE(ios, watchos, tvos); 
	
/*!
    @function	AudioFileComponentDataIsThisFormat
    @abstract   deprecated. use AudioFileComponentFileDataIsThisFormat instead.
    @param		inComponent			an AudioFileComponent
    @param		inClientData		a constant that will be passed to your callbacks.
	@param		inReadFunc			a function that will be called when AudioFile needs to read data.
	@param		inWriteFunc			a function that will be called when AudioFile needs to write data.
	@param		inGetSizeFunc		a function that will be called when AudioFile needs to know the file size.
	@param		inSetSizeFunc		a function that will be called when AudioFile needs to set the file size.
    @param      outResult			on output, is set to 1 if the file data is recognized by this component, 0 if not.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentDataIsThisFormat(	
								AudioFileComponent				inComponent,
								void * __nullable				inClientData,
								AudioFile_ReadProc __nullable	inReadFunc, 
								AudioFile_WriteProc	__nullable	inWriteFunc,
								AudioFile_GetSizeProc __nullable inGetSizeFunc,
								AudioFile_SetSizeProc __nullable inSetSizeFunc,
								UInt32							*outResult)		API_DEPRECATED("no longer supported", macos(10.4, 10.5)) API_UNAVAILABLE(ios, watchos, tvos); 


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//	The following calls are not made on AudioFile instances.
//  They implement the AudioFileGetGlobalInfo calls.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

/*!
    @function	AudioFileComponentGetGlobalInfoSize
    @abstract   implements AudioFileGetGlobalInfoSize.
    @param		inComponent			an AudioFileComponent
    @param      inPropertyID		an AudioFileGlobalInfo property constant.
    @param      inSpecifierSize		The size of the specifier data.
    @param      inSpecifier			A specifier is a buffer of data used as an input argument to some of the global info properties.
    @param      outPropertySize		the size in bytes of the current value of the property. In order to get the property value, 
									you will need a buffer of this size.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentGetGlobalInfoSize(	
								AudioFileComponent				inComponent,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							inSpecifierSize,
								const void * __nullable			inSpecifier,
								UInt32							*outPropertySize)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

/*!
    @function	AudioFileComponentGetGlobalInfo
    @abstract   implements AudioFileGetGlobalInfo.
    @param		inComponent			an AudioFileComponent
    @param      inPropertyID		an AudioFileGlobalInfo property constant.
    @param      inSpecifierSize		The size of the specifier data.
    @param      inSpecifier			A specifier is a buffer of data used as an input argument to some of the global info properties.
    @param      ioPropertyDataSize	on input the size of the outPropertyData buffer. On output the number of bytes written to the buffer.
    @param      outPropertyData		the buffer in which to write the property data.
    @result							returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentGetGlobalInfo(	
								AudioFileComponent				inComponent,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							inSpecifierSize,
								const void * __nullable			inSpecifier,
								UInt32							*ioPropertyDataSize,
								void							*outPropertyData)	API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos);

//==================================================================================================
//	Properties for AudioFileComponentGetGlobalInfo. 
//==================================================================================================

/*!
    @enum AudioFileComponent specific properties
    @constant   kAudioFileComponent_CanRead
					Is file type readable? Returns a UInt32 1 or 0. 
    @constant   kAudioFileComponent_CanWrite
					Is file type writeable? Returns a UInt32 1 or 0. 
    @constant   kAudioFileComponent_FileTypeName
					Returns a CFString containing the name for the file type. 
    @constant   kAudioFileComponent_ExtensionsForType
					Returns a CFArray of CFStrings containing the file extensions 
					that are recognized for this file type. 
    @constant   kAudioFileComponent_UTIsForType
					Returns a CFArray of CFStrings containing the universal type identifiers 
					for this file type. 
    @constant   kAudioFileComponent_MIMETypesForType
					Returns a CFArray of CFStrings containing the MIME types 
					for this file type. 
    @constant   kAudioFileComponent_AvailableFormatIDs
					Returns a array of format IDs for formats that can be read. 
    @constant   kAudioFileComponent_AvailableStreamDescriptionsForFormat
					The specifier is the format ID for the requested format.
					Returns an array of AudioStreamBasicDescriptions which have all of the 
					formats for a particular file type and format ID. The AudioStreamBasicDescriptions
					have the following fields filled in: mFormatID, mFormatFlags, mBitsPerChannel
    @constant   kAudioFileComponent_FastDispatchTable
					Deprecated. This selector is no longer called by the implementation. 
    @constant   kAudioFileComponent_HFSTypeCodesForType
					Returns an array of HFSTypeCodes corresponding to this file type.
					The first type in the array is the preferred one for use when
					writing new files.
*/
CF_ENUM(AudioFilePropertyID)
{
	kAudioFileComponent_CanRead									= 'cnrd',
	kAudioFileComponent_CanWrite								= 'cnwr',
	kAudioFileComponent_FileTypeName							= 'ftnm',
	kAudioFileComponent_UTIsForType								= 'futi',
	kAudioFileComponent_MIMETypesForType						= 'fmim',
	kAudioFileComponent_ExtensionsForType						= 'fext',
	kAudioFileComponent_AvailableFormatIDs						= 'fmid',
	kAudioFileComponent_AvailableStreamDescriptionsForFormat	= 'sdid',
	kAudioFileComponent_FastDispatchTable						= 'fdft',
	kAudioFileComponent_HFSTypeCodesForType						= 'fhfs'
};

//==================================================================================================
//	Selectors for the component routines 
//==================================================================================================

enum
{
	kAudioFileCreateSelect						= 0x0001,
	kAudioFileOpenSelect						= 0x0002,
	kAudioFileInitializeSelect					= 0x0003,
	kAudioFileOpenWithCallbacksSelect			= 0x0004,
	kAudioFileInitializeWithCallbacksSelect		= 0x0005,
	kAudioFileCloseSelect						= 0x0006,
	kAudioFileOptimizeSelect					= 0x0007,
	kAudioFileReadBytesSelect					= 0x0008,
	kAudioFileWriteBytesSelect					= 0x0009,
	kAudioFileReadPacketsSelect					= 0x000A,
	kAudioFileWritePacketsSelect				= 0x000B,
	kAudioFileGetPropertyInfoSelect				= 0x000C,
	kAudioFileGetPropertySelect					= 0x000D,
	kAudioFileSetPropertySelect					= 0x000E,
	
	kAudioFileExtensionIsThisFormatSelect		= 0x000F,
	kAudioFileFileIsThisFormatSelect			= 0x0010,
	kAudioFileDataIsThisFormatSelect			= 0x0011,
	kAudioFileGetGlobalInfoSizeSelect			= 0x0012,
	kAudioFileGetGlobalInfoSelect				= 0x0013,

	kAudioFileCountUserDataSelect				= 0x0014,
	kAudioFileGetUserDataSizeSelect				= 0x0015,
	kAudioFileGetUserDataSelect					= 0x0016,
	kAudioFileSetUserDataSelect					= 0x0017,
	kAudioFileRemoveUserDataSelect				= 0x0018,
	kAudioFileCreateURLSelect					= 0x0019,
	kAudioFileOpenURLSelect						= 0x001A,
	kAudioFileFileDataIsThisFormatSelect		= 0x001B,
	kAudioFileReadPacketDataSelect				= 0x001C,
};


#pragma mark -
#pragma mark Deprecated

//==================================================================================================
// Fast Dispatch Function typedefs. Deprecated. These are no longer used by the implementation.
//==================================================================================================

typedef OSStatus (*ReadBytesFDF)(	void			*inComponentStorage,
									Boolean			inUseCache,
									SInt64			inStartingByte, 
									UInt32			*ioNumBytes, 
									void			*outBuffer);
								
typedef OSStatus (*WriteBytesFDF)(	void			*inComponentStorage,
									Boolean			inUseCache,
									SInt64			inStartingByte, 
									UInt32			*ioNumBytes, 
									const void		*inBuffer);
							
typedef OSStatus (*ReadPacketsFDF)(	void							*inComponentStorage,
									Boolean							inUseCache,
									UInt32							*outNumBytes,
									AudioStreamPacketDescription * __nullable outPacketDescriptions,
									SInt64							inStartingPacket, 
									UInt32  						*ioNumPackets, 
									void							*outBuffer);

typedef OSStatus (*ReadPacketDataFDF)(	void						*inComponentStorage,
									Boolean							inUseCache,
									UInt32							*ioNumBytes,
									AudioStreamPacketDescription * __nullable outPacketDescriptions,
									SInt64							inStartingPacket, 
									UInt32  						*ioNumPackets, 
									void							*outBuffer);

typedef OSStatus (*WritePacketsFDF)(	void								*inComponentStorage,
										Boolean								inUseCache,
										UInt32								inNumBytes,
										const AudioStreamPacketDescription * __nullable inPacketDescriptions,
										SInt64								inStartingPacket, 
										UInt32								*ioNumPackets, 
										const void							*inBuffer);
								
typedef OSStatus (*GetPropertyInfoFDF)(	void					*inComponentStorage,
										AudioFilePropertyID		inPropertyID,
										UInt32 * __nullable		outDataSize,
										UInt32 * __nullable		isWritable);
								
typedef OSStatus (*GetPropertyFDF)(		void					*inComponentStorage,
										AudioFilePropertyID		inPropertyID,
										UInt32					*ioDataSize,
										void					*ioPropertyData);
							
typedef OSStatus (*SetPropertyFDF)(		void					*inComponentStorage,
										AudioFilePropertyID		inPropertyID,
										UInt32					inDataSize,
										const void				*inPropertyData);

typedef OSStatus (*CountUserDataFDF)(   void					*inComponentStorage,
										UInt32					inUserDataID,
										UInt32					*outNumberItems);

typedef OSStatus (*GetUserDataSizeFDF)( void					*inComponentStorage,
										UInt32					inUserDataID,
										UInt32					inIndex,
										UInt32					*outDataSize);

typedef OSStatus (*GetUserDataFDF)(		void					*inComponentStorage,
										UInt32					inUserDataID,
										UInt32					inIndex,
										UInt32					*ioUserDataSize,
										void					*outUserData);

typedef OSStatus (*SetUserDataFDF)(		void					*inComponentStorage,
										UInt32					inUserDataID,
										UInt32					inIndex,
										UInt32					inUserDataSize,
										const void				*inUserData);
										
/* no fast dispatch for kAudioFileRemoveUserDataSelect */

#pragma mark -
#pragma mark Deprecated

//==================================================================================================
// Fast Dispatch Function tables. Deprecated. These are no longer used by the implementation.
//==================================================================================================

typedef struct AudioFileFDFTable
{
	void				*mComponentStorage;
	ReadBytesFDF 		mReadBytesFDF;
	WriteBytesFDF 		mWriteBytesFDF;
	ReadPacketsFDF		mReadPacketsFDF;
	WritePacketsFDF		mWritePacketsFDF;

	GetPropertyInfoFDF	mGetPropertyInfoFDF;
	GetPropertyFDF		mGetPropertyFDF;
	SetPropertyFDF		mSetPropertyFDF;
	
	CountUserDataFDF	mCountUserDataFDF;
	GetUserDataSizeFDF  mGetUserDataSizeFDF;
	GetUserDataFDF		mGetUserDataFDF;
	SetUserDataFDF		mSetUserDataFDF;
} AudioFileFDFTable API_DEPRECATED("no longer supported", macos(10.4, 10.7)) API_UNAVAILABLE(ios, watchos, tvos);

typedef struct AudioFileFDFTableExtended
{
	void				*mComponentStorage;
	ReadBytesFDF 		mReadBytesFDF;
	WriteBytesFDF 		mWriteBytesFDF;
	ReadPacketsFDF		mReadPacketsFDF;
	WritePacketsFDF		mWritePacketsFDF;

	GetPropertyInfoFDF	mGetPropertyInfoFDF;
	GetPropertyFDF		mGetPropertyFDF;
	SetPropertyFDF		mSetPropertyFDF;
	
	CountUserDataFDF	mCountUserDataFDF;
	GetUserDataSizeFDF  mGetUserDataSizeFDF;
	GetUserDataFDF		mGetUserDataFDF;
	SetUserDataFDF		mSetUserDataFDF;

	ReadPacketDataFDF	mReadPacketDataFDF;
} AudioFileFDFTableExtended API_DEPRECATED("no longer supported", macos(10.4, 10.7)) API_UNAVAILABLE(ios, watchos, tvos);

struct FSRef;

/*!
	@functiongroup Deprecated AFComponent
	@discussion		These API calls are no longer called on Snow Leopard, instead the URL versions are used.
					They can be provided by the file component for compatibility with Leopard and Tiger systems
*/

/*!
    @function	AudioFileComponentCreate
    @abstract   implements AudioFileCreate
    @param inComponent		an AudioFileComponent
    @param inParentRef		an FSRef to the directory where  the new file should be created.
    @param inFileName		a CFStringRef containing the name of the file to be created.
    @param inFormat			an AudioStreamBasicDescription describing the data format that will be
							added to the audio file.
    @param inFlags			relevant flags for creating/opening the file. Currently zero.
    @param outNewFileRef	if successful, the FSRef of the newly created file.
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentCreate(
								AudioFileComponent					inComponent,
								const struct FSRef					*inParentRef, 
								CFStringRef							inFileName,
								const AudioStreamBasicDescription	*inFormat,
								UInt32								inFlags,
								struct FSRef						*outNewFileRef)		API_DEPRECATED("no longer supported", macos(10.4, 10.6)) API_UNAVAILABLE(ios, watchos, tvos);
                                

/*!
    @function	AudioFileComponentInitialize
    @abstract   implements AudioFileInitialize
    @param inComponent		an AudioFileComponent
    @param inFileRef		the FSRef of an existing audio file.
    @param inFormat			an AudioStreamBasicDescription describing the data format that will be
							added to the audio file.
    @param inFlags			flags for creating/opening the file. Currently zero.
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentInitialize(
								AudioFileComponent					inComponent,
								const struct FSRef					*inFileRef,
								const AudioStreamBasicDescription	*inFormat,
								UInt32								inFlags)		API_DEPRECATED("no longer supported", macos(10.4, 10.6)) API_UNAVAILABLE(ios, watchos, tvos);
							
/*!
    @function	AudioFileComponentOpenFile
    @abstract   implements AudioFileOpen
    @param inComponent		an AudioFileComponent
    @param inFileRef		the FSRef of an existing audio file.
    @param inPermissions	use the permission constants
    @param inRefNum			the file refNum for the opened file. This avoids opening the file twice
							- once to determine which file type to which to delegate and once to parse it.
    @result					returns noErr if successful.
*/
extern OSStatus 
AudioFileComponentOpenFile(
								AudioFileComponent					inComponent,
								const struct FSRef					*inFileRef, 
								SInt8  								inPermissions,
								SInt16								inRefNum)		API_DEPRECATED("no longer supported", macos(10.4, 10.6)) API_UNAVAILABLE(ios, watchos, tvos);

#if defined(__cplusplus)
}
#endif

#if !TARGET_OS_IPHONE
//=====================================================================================================================

typedef	OSStatus	(*AudioFileComponentCreateURLProc)(
								void								*self, 
								CFURLRef							inFileRef, 
								const AudioStreamBasicDescription*	inFormat, 
								UInt32								inFlags);
typedef	OSStatus	(*AudioFileComponentOpenURLProc)(
								void								*self, 
								CFURLRef					inFileRef, 
								SInt8						inPermissions, 
								int							inFileDescriptor);
								
typedef	OSStatus	(*AudioFileComponentOpenWithCallbacksProc)(
								void								*self,
								void								*inClientData, 
								AudioFile_ReadProc					inReadFunc, 
								AudioFile_WriteProc					inWriteFunc, 
								AudioFile_GetSizeProc				inGetSizeFunc,
								AudioFile_SetSizeProc				inSetSizeFunc);

typedef	OSStatus	(*AudioFileComponentInitializeWithCallbacksProc)(
								void								*self,
								void								*inClientData, 
								AudioFile_ReadProc					inReadFunc, 
								AudioFile_WriteProc					inWriteFunc, 
								AudioFile_GetSizeProc				inGetSizeFunc,
								AudioFile_SetSizeProc				inSetSizeFunc,
								UInt32								inFileType,
								const AudioStreamBasicDescription	*inFormat,
								UInt32								inFlags);

typedef	OSStatus	(*AudioFileComponentCloseProc)(
								void								*self);
				
typedef	OSStatus	(*AudioFileComponentOptimizeProc)(
								void								*self);
				
typedef	OSStatus	(*AudioFileComponentReadBytesProc)(		
								void							*self,
								Boolean							inUseCache,
								SInt64							inStartingByte, 
								UInt32							*ioNumBytes, 
								void							*outBuffer);	
						
typedef	OSStatus	(*AudioFileComponentWriteBytesProc)(		
								void							*self,
								Boolean							inUseCache,
								SInt64							inStartingByte, 
								UInt32							*ioNumBytes, 
								const void						*inBuffer);	
						
typedef	OSStatus	(*AudioFileComponentReadPacketsProc)(		
								void							*self,
								Boolean							inUseCache,
								UInt32							*outNumBytes,
								AudioStreamPacketDescription * __nullable outPacketDescriptions,
								SInt64							inStartingPacket, 
								UInt32  						*ioNumPackets, 
								void							*outBuffer);	
									

typedef	OSStatus	(*AudioFileComponentReadPacketDataProc)(		
								void							*self,
								Boolean							inUseCache,
								UInt32							*ioNumBytes,
								AudioStreamPacketDescription * __nullable outPacketDescriptions,
								SInt64							inStartingPacket, 
								UInt32  						*ioNumPackets, 
								void							*outBuffer);	
									
typedef	OSStatus	(*AudioFileComponentWritePacketsProc)(	
								void								*self,
								Boolean								inUseCache,
								UInt32								inNumBytes,
								const AudioStreamPacketDescription * __nullable inPacketDescriptions,
								SInt64								inStartingPacket, 
								UInt32								*ioNumPackets, 
								const void							*inBuffer);


typedef	OSStatus	(*AudioFileComponentGetPropertyInfoProc)(	
								void							*self,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32 * __nullable				outPropertySize,
								UInt32 * __nullable				outWritable);


typedef	OSStatus	(*AudioFileComponentGetPropertyProc)(	
								void							*self,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							*ioPropertyDataSize,
								void							*outPropertyData);

typedef	OSStatus	(*AudioFileComponentSetPropertyProc)(	
								void							*self,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							inPropertyDataSize,
								const void						*inPropertyData);


typedef	OSStatus	(*AudioFileComponentCountUserDataProc)(	
								void							*self,
								UInt32							inUserDataID,
								UInt32							*outNumberItems);

typedef	OSStatus	(*AudioFileComponentGetUserDataSizeProc)(	
								void							*self,
								UInt32							inUserDataID,
								UInt32							inIndex,
								UInt32							*outUserDataSize);

typedef	OSStatus	(*AudioFileComponentGetUserDataProc)(	
								void							*self,
								UInt32							inUserDataID,
								UInt32							inIndex,
								UInt32							*ioUserDataSize,
								void							*outUserData);

typedef	OSStatus	(*AudioFileComponentSetUserDataProc)(	
								void							*self,
								UInt32							inUserDataID,
								UInt32							inIndex,
								UInt32							inUserDataSize,
								const void						*inUserData);


typedef	OSStatus	(*AudioFileComponentRemoveUserDataProc)(	
								void							*self,
								UInt32							inUserDataID,
								UInt32							inIndex);

typedef	OSStatus	(*AudioFileComponentExtensionIsThisFormatProc)(	
								void							*self,
								CFStringRef 					inExtension,
								UInt32							*outResult);	


typedef	OSStatus	(*AudioFileComponentFileDataIsThisFormatProc)(	
								void							*self,
								UInt32							inDataByteSize,
								const void*						inData,
								UInt32							*outResult);	


typedef	OSStatus	(*AudioFileComponentGetGlobalInfoSizeProc)(	
								void							*self,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							inSpecifierSize,
								const void * __nullable			inSpecifier,
								UInt32							*outPropertySize);

typedef	OSStatus	(*AudioFileComponentGetGlobalInfoProc)(	
								void							*self,
								AudioFileComponentPropertyID	inPropertyID,
								UInt32							inSpecifierSize,
								const void * __nullable			inSpecifier,
								UInt32							*ioPropertyDataSize,
								void							*outPropertyData);
#endif

CF_ASSUME_NONNULL_END

#endif // AudioToolbox_AudioFileComponent_h

#else
#include <AudioToolboxCore/AudioFileComponent.h>
#endif
