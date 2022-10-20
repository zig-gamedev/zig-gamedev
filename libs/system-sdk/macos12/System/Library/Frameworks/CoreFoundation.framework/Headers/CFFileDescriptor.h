/*	CFFileDescriptor.h
	Copyright (c) 2006-2019, Apple Inc. All rights reserved.
*/

#if !defined(__COREFOUNDATION_CFFILEDESCRIPTOR__)
#define __COREFOUNDATION_CFFILEDESCRIPTOR__ 1

#include <CoreFoundation/CFRunLoop.h>

CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN

typedef int CFFileDescriptorNativeDescriptor;

typedef struct CF_BRIDGED_MUTABLE_TYPE(id) __CFFileDescriptor * CFFileDescriptorRef;

/* Callback Reason Types */
CF_ENUM(CFOptionFlags) {
    kCFFileDescriptorReadCallBack = 1UL << 0,
    kCFFileDescriptorWriteCallBack = 1UL << 1
};

typedef void (*CFFileDescriptorCallBack)(CFFileDescriptorRef f, CFOptionFlags callBackTypes, void *info);

typedef struct {
    CFIndex	version;
    void *	info;
    void *	(*retain)(void *info);
    void	(*release)(void *info);
    CFStringRef	(*copyDescription)(void *info);
} CFFileDescriptorContext;

CF_EXPORT CFTypeID	CFFileDescriptorGetTypeID(void) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

CF_EXPORT CFFileDescriptorRef	CFFileDescriptorCreate(CFAllocatorRef allocator, CFFileDescriptorNativeDescriptor fd, Boolean closeOnInvalidate, CFFileDescriptorCallBack callout, const CFFileDescriptorContext *context) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

CF_EXPORT CFFileDescriptorNativeDescriptor	CFFileDescriptorGetNativeDescriptor(CFFileDescriptorRef f) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

CF_EXPORT void	 CFFileDescriptorGetContext(CFFileDescriptorRef f, CFFileDescriptorContext *context) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

CF_EXPORT void	 CFFileDescriptorEnableCallBacks(CFFileDescriptorRef f, CFOptionFlags callBackTypes) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
CF_EXPORT void	 CFFileDescriptorDisableCallBacks(CFFileDescriptorRef f, CFOptionFlags callBackTypes) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

CF_EXPORT void	 CFFileDescriptorInvalidate(CFFileDescriptorRef f) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
CF_EXPORT Boolean	CFFileDescriptorIsValid(CFFileDescriptorRef f) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

CF_EXPORT CFRunLoopSourceRef	CFFileDescriptorCreateRunLoopSource(CFAllocatorRef allocator, CFFileDescriptorRef f, CFIndex order) API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));


CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED

#endif /* ! __COREFOUNDATION_CFFILEDESCRIPTOR__ */

