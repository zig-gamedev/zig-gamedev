/*
 *  CVBuffer.h
 *  CoreVideo
 *
 *  Copyright (c) 2004-2017,2021 Apple Inc. All rights reserved.
 *
 */
 
 /*! @header CVBuffer.h
	@copyright 2004-2015 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later, and iOS 4.0 or later
    @discussion CVBufferRef types are abstract and only define ways to attach meta data to buffers (such as timestamps,
	        colorspace information, etc.).    CVBufferRefs do not imply any particular kind of data storage.  It could
		be compressed data, image data, etc.
		
*/

#if !defined(__COREVIDEO_CVBUFFER_H__)
#define __COREVIDEO_CVBUFFER_H__ 1

#include <TargetConditionals.h>
#include <Availability.h>
#include <AvailabilityMacros.h>

#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVReturn.h>
#include <CoreFoundation/CFDictionary.h>
#if TARGET_OS_MAC
#include <stddef.h>
#include <stdint.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#pragma mark CVBufferRef attribute keys

/* The following two keys are useful with the CoreVideo pool and texture cache APIs so that you can specify
   an initial set of default buffer attachments to automatically be attached to the buffer when it is created. */
CV_EXPORT const CFStringRef CV_NONNULL kCVBufferPropagatedAttachmentsKey __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVBufferNonPropagatedAttachmentsKey __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

#pragma mark CVBufferRef attachment keys

CV_EXPORT const CFStringRef CV_NONNULL kCVBufferMovieTimeKey __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);	// Generally only available for frames emitted by QuickTime; CFDictionary containing these two keys:
CV_EXPORT const CFStringRef CV_NONNULL kCVBufferTimeValueKey __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVBufferTimeScaleKey __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);


#pragma mark CVBufferRef

typedef CF_ENUM(uint32_t, CVAttachmentMode)
{
	kCVAttachmentMode_ShouldNotPropagate    = 0,
	kCVAttachmentMode_ShouldPropagate       = 1
};

/*!
    @typedef	CVBufferRef
    @abstract   Base type for all CoreVideo buffers

*/
typedef struct CV_BRIDGED_TYPE(id) __CVBuffer *CVBufferRef;

/*!
    @function   CVBufferRetain
    @abstract   Retains a CVBuffer object
    @discussion Like CFRetain CVBufferRetain increments the retain count of a CVBuffer object. In contrast to the CF call it is NULL safe.
    @param      buffer A CVBuffer object that you want to retain.
    @result     A CVBuffer object that is the same as the passed in buffer.
*/
CV_EXPORT CVBufferRef CV_NULLABLE CVBufferRetain( CVBufferRef CV_NULLABLE buffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
/*!
    @function   CVBufferRelease
    @abstract   Release a CVBuffer object
    @discussion Like CFRetain CVBufferRetain decrements the retain count of a CVBuffer object. If that count consequently becomes zero the memory allocated to the object is deallocated and the object is destroyed. In contrast to the CF call it is NULL safe.
    @param      buffer A CVBuffer object that you want to release.
*/
CV_EXPORT void CVBufferRelease( CV_RELEASES_ARGUMENT CVBufferRef CV_NULLABLE buffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

#pragma mark CVBufferAttachment

/*!
    @function   CVBufferSetAttachment
    @abstract   Sets or adds a attachment of a CVBuffer object
    @discussion You can attach any CF object to a CVBuffer object to store additional information. CVBufferGetAttachment stores an attachement identified by a key. If the key doesn't exist, the attachment will be added. If the key does exist, the existing attachment will be replaced. In bouth cases the retain count of the attachment will be incremented. The value can be any CFType but nil has no defined behavior.
    @param      buffer  Target CVBuffer object.
    @param      key     Key in form of a CFString identifying the desired attachment.
    @param      value	Attachment in form af a CF object.
    @param      attachmentMode	Specifies which attachment mode is desired for this attachment.   A particular attachment key may only exist in
                                a single mode at a time.
*/
CV_EXPORT void	CVBufferSetAttachment( CVBufferRef CV_NONNULL buffer, CFStringRef CV_NONNULL key, CFTypeRef CV_NONNULL value, CVAttachmentMode attachmentMode ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);


/*!
    @function   CVBufferGetAttachment
    @abstract   Returns a specific attachment of a CVBuffer object
    @discussion You can attach any CF object to a CVBuffer object to store additional information. CVBufferGetAttachment retrieves an attachement identified by a key.
    @param      buffer  Target CVBuffer object.
    @param      key	Key in form of a CFString identifying the desired attachment.
    @param      attachmentMode.  Returns the mode of the attachment, if desired.  May be NULL.
    @result     If found the attachment object
*/
CV_EXPORT CFTypeRef CV_NULLABLE CVBufferGetAttachment( CVBufferRef CV_NONNULL buffer, CFStringRef CV_NONNULL key, CVAttachmentMode * CV_NULLABLE attachmentMode )  API_DEPRECATED_WITH_REPLACEMENT("CVBufferCopyAttachment", macos(10.4, 12.0), ios(4.0,15.0), tvos(9.0, 15.0), watchos(4.0, 8.0));

/*!
    @function   CVBufferRemoveAttachment
    @abstract   Removes a specific attachment of a CVBuffer object
    @discussion CVBufferRemoveAttachment removes an attachement identified by a key. If found the attachement is removed and the retain count decremented.
    @param      buffer  Target CVBuffer object.
    @param      key	Key in form of a CFString identifying the desired attachment.
*/
CV_EXPORT void	CVBufferRemoveAttachment( CVBufferRef CV_NONNULL buffer, CFStringRef CV_NONNULL key ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVBufferRemoveAllAttachments
    @abstract   Removes all attachments of a CVBuffer object
    @discussion While CVBufferRemoveAttachment removes a specific attachement identified by a key CVBufferRemoveAllAttachments removes all attachments of a buffer and decrements their retain counts.
    @param      buffer  Target CVBuffer object.
*/
CV_EXPORT void	CVBufferRemoveAllAttachments( CVBufferRef CV_NONNULL buffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVBufferGetAttachments
    @abstract   Returns all attachments of a CVBuffer object
    @discussion CVBufferGetAttachments is a convenience call that returns all attachments with their corresponding keys in a CFDictionary.
    @param      buffer  Target CVBuffer object.
    @result     A CFDictionary with all buffer attachments identified by there keys. If no attachment is present, the dictionary is empty.  Returns NULL
		for invalid attachment mode.
*/
CV_EXPORT CFDictionaryRef CF_RETURNS_NOT_RETAINED CV_NULLABLE CVBufferGetAttachments( CVBufferRef CV_NONNULL buffer, CVAttachmentMode attachmentMode ) API_DEPRECATED_WITH_REPLACEMENT("CVBufferCopyAttachments", macos(10.4, 12.0), ios(4.0,15.0), tvos(9.0, 15.0), watchos(4.0, 8.0));

/*!
    @function   CVBufferSetAttachments
    @abstract   Sets a set of attachments for a CVBuffer
    @discussion CVBufferSetAttachments is a convenience call that in turn calls CVBufferSetAttachment for each key and value in the given dictionary. All key value pairs must be in the root level of the dictionary.
    @param      buffer  Target CVBuffer object.
*/
CV_EXPORT void CVBufferSetAttachments( CVBufferRef CV_NONNULL buffer, CFDictionaryRef CV_NONNULL theAttachments, CVAttachmentMode attachmentMode ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVBufferPropagateAttachments
    @abstract   Copy all propagatable attachments from one buffer to another.
    @discussion CVBufferPropagateAttachments is a convenience call that copies all attachments with a mode of kCVAttachmentMode_ShouldPropagate from one
                buffer to another.
    @param      sourceBuffer  CVBuffer to copy attachments from.
    @param      destinationBuffer  CVBuffer to copy attachments to.
*/
CV_EXPORT void  CVBufferPropagateAttachments( CVBufferRef CV_NONNULL sourceBuffer, CVBufferRef CV_NONNULL destinationBuffer ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVBufferCopyAttachments
    @abstract   Returns a copy of all attachments of a CVBuffer object. It is the caller’s responsibility to release the returned dictionary.
    @discussion CVBufferCopyAttachments is a convenience call that returns a copy of all attachments with their corresponding keys in a CFDictionary.
    @param      buffer  Target CVBuffer object.
    @result     A CFDictionary with all buffer attachments identified by their keys. If no attachment is present or invalid attachment mode,   returns NULL
*/
CV_EXPORT CFDictionaryRef CF_RETURNS_RETAINED CV_NULLABLE CVBufferCopyAttachments( CVBufferRef CV_NONNULL buffer, CVAttachmentMode attachmentMode ) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

/*!
    @function   CVBufferCopyAttachment
    @abstract   Returns a retained specific attachment of a CVBuffer object. It is the caller’s responsibility to release the returned value.
    @discussion You can attach any CF object to a CVBuffer object to store additional information. CVBufferCopyAttachment retrieves a retained attachment identified by a key.
    @param      buffer  Target CVBuffer object.
    @param      key    Key in form of a CFString identifying the desired attachment.
    @param      attachmentMode.  Returns the mode of the attachment, if desired.  May be NULL.
    @result     If found the attachment object, return the value; otherwize, return NULL.
*/
CV_EXPORT CFTypeRef CV_NULLABLE CF_RETURNS_RETAINED CVBufferCopyAttachment( CVBufferRef CV_NONNULL buffer, CFStringRef CV_NONNULL key, CVAttachmentMode * CV_NULLABLE attachmentMode ) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

/*!
    @function   CVBufferHasAttachment
    @abstract   Returns true if an attachment with the passed key is present on a CVBuffer object.
    @param      buffer  Target CVBuffer object.
    @param      key    Key in form of a CFString identifying the desired attachment.
    @result     True if an attachment with this key is present, otherwise false.
*/
CV_EXPORT Boolean CVBufferHasAttachment( CVBufferRef CV_NONNULL buffer, CFStringRef CV_NONNULL key ) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

#if defined(__cplusplus)
}
#endif

#endif
