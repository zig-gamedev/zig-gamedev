/* iig(DriverKit-191.30.2) generated from IOUserClient.iig */

/* IOUserClient.iig:1-156 */
/*
 * Copyright (c) 2019-2019 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#if !__IIG
#include <IOKit/IOUserClient.h>
#endif

#ifndef _IOKIT_UIOUSERCLIENT_H
#define _IOKIT_UIOUSERCLIENT_H

#include <DriverKit/OSAction.h>  /* .iig include */
#include <DriverKit/IOService.h>  /* .iig include */
#include <DriverKit/IOBufferMemoryDescriptor.h>  /* .iig include */


enum {
	kIOUserClientScalarArrayCountMax  = 16,
};
typedef uint64_t IOUserClientScalarArray[kIOUserClientScalarArrayCountMax];

enum {
	kIOUserClientAsyncReferenceCountMax  = 16,
};
typedef uint64_t IOUserClientAsyncReferenceArray[kIOUserClientAsyncReferenceCountMax];

enum {
	kIOUserClientAsyncArgumentsCountMax  = 16,
};
typedef uint64_t IOUserClientAsyncArgumentsArray[kIOUserClientAsyncArgumentsCountMax];

// CopyClientMemoryForType options
enum {
	kIOUserClientMemoryReadOnly  = 0x00000001,
};

#define kIOUserClientQueueNameExternalMethod  "IOUserClientQueueExternalMethod"


/*! @enum
 *   @abstract Constant to denote a variable length structure argument to IOUserClient.
 *   @constant kIOUserClientVariableStructureSize Use in the structures IOUserClientMethodDispatch to specify the size of the structure is variable.
 */
enum {
	kIOUserClientVariableStructureSize = 0xffffffff
};


enum {
#define IO_USER_CLIENT_METHOD_ARGUMENTS_CURRENT_VERSION    2
	kIOUserClientMethodArgumentsCurrentVersion = IO_USER_CLIENT_METHOD_ARGUMENTS_CURRENT_VERSION
};

/*!
 * @struct      IOUserClientMethodArguments
 * @brief       Holds arguments from IOKit.framework IOConnectMethod calls.
 * @discussion  Any argument may be passed as NULL if not passed by the caller.
 * @field       selector Selector argument to IOConnectMethod.
 * @field       scalarInput Array of scalars from caller.
 * @field       scalarInputCount Count of valid scalars in scalarInput.
 * @field       structureInput OSData object containing structure input from IOConnectMethod.
 * @field       structureInputDescriptor IOMemoryDescriptor containing structure input from IOConnectMethod.
 *				This parameter is only set for large structures, and if set structureInput will be NULL.
 * @field       scalarOutput Array of scalars to return to the caller.
 * @field       scalarOutputCount Count of scalars to return to the caller in scalarOutput.
 * @field       structureOutput An OSData to be returned to the caller as structure output.
 *              This field should be set by the driver to an OSData object it created with
 *              the data to be returned, and the OSData instance will be released by the OS.
 *              It is an error for the driver to set this field if structureOutputDescriptor was passed in
 * @field       structureOutputDescriptor A IOMemoryDescriptor specified by the caller for structure output.
 * @field       structureOutputMaximumSize Maximum size of structure output specified by caller
 *              or kIOUserClientVariableStructureSize.
 * @field       completion For IOConnectAsyncMethod, an OSAction used to deliver async data to the caller.
 *              It is only retained during the invocation of ExternalMethod and should be retained if
 *              used beyond then.
 */

struct IOUserClientMethodArguments {
	uint64_t            			version;
	uint64_t                        selector;
	OSAction                      * completion;
    const uint64_t			      * scalarInput;
	uint32_t						scalarInputCount;
	OSData 						  * structureInput;
	IOMemoryDescriptor            * structureInputDescriptor;
	uint64_t                      * scalarOutput;
	uint32_t                        scalarOutputCount;
	OSData                        * structureOutput;
	IOMemoryDescriptor            * structureOutputDescriptor;
	uint64_t                        structureOutputMaximumSize;
	uint64_t                        __reserved[30];
};

typedef kern_return_t (*IOUserClientMethodFunction)(
	OSObject * target,
	void * reference,
    IOUserClientMethodArguments * arguments);

/*!
 * @struct      IOUserClientMethodDispatch
 * @brief       Used to check fields in IOUserClientMethodArguments
 * @field       function to invoke after making the checks specified below. If NULL and all checks pass,
 *              kIOReturnNoCompletion will be returned for the caller to implement the method.
 * @field       checkCompletionExists
 *              if true completion field must be set,
 *              if false must be zero,
 *              if -1U don't care
 * @field       checkScalarInputCount
 *              if has value kIOUserClientVariableStructureSize don't care,
 *              otherwise must equal args->scalarInputCount
 * @field       checkStructureInputSize
 *              if has value kIOUserClientVariableStructureSize don't care,
 *              otherwise must equal length of structureInput or structureInputDescriptor
 * @field       checkScalarOutputCount
 *              if has value kIOUserClientVariableStructureSize don't care,
 *              otherwise must equal args->scalarOutputCount
 * @field       checkStructureOutputSize
 *              if has value kIOUserClientVariableStructureSize don't care,
 *              otherwise must equal length of structureOutputMaximumSize
 */

struct IOUserClientMethodDispatch {
	IOUserClientMethodFunction function;
	uint32_t			       checkCompletionExists;
	uint32_t			       checkScalarInputCount;
	uint32_t			       checkStructureInputSize;
	uint32_t			       checkScalarOutputCount;
	uint32_t			       checkStructureOutputSize;
};

/* source class IOUserClient IOUserClient.iig:157-300 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

/*!
 * @class IOUserClient
 *
 * @abstract
 * IOUserClient represents a connection opened by IOServiceOpen in the IOKit.framework.
 *
 * @discussion
 * An application may open an IOUserClient by calling IOServiceOpen(). This results in a call
 * to the IOService::NewUserClient API to create an instance representing the connection.
 * and to receive untyped data via IOConnectMethod/IOConnectAsyncMethod.
 * As an IOService subclass, IOUserClient receives the normal Start()/Stop() lifecyle calls.
 *

*/

class KERNEL IOUserClient : public IOService
{
public:
	virtual bool
	init() override;

	virtual void
	free() override;

	/*!
	 * @brief       Receive arguments from IOKit.framework IOConnectMethod calls.
	 * @discussion  IOConnectMethod calls from the owner of the connection come here.
	 *              Any argument may be passed as NULL if not passed by the caller.
	 *              The method runs on a queue set by IOService::SetDispatchQueuue()
	 *              with the name kIOUserClientQueueNameExternalMethod, or the default
	 *              queue for the IOUserClient object if one was not set.
	 * @param       selector Selector argument to IOConnectMethod.
	 * @param       arguments Structure describing all arguments being passed to IOConnectMethod.
	 * 				          See the IOUserClientMethodArguments definition.
	 * @param       dispatch NULL when called in the driver. The IOUserClient::ExternalMethod()
	 *				         implementation may be called with a non-NULL argument to check
	 *				         certain fields of the arguments structure before calling a target procedure
	 *				         specified by the dispatch structure 'function' field, and the
	 *				         'target' and 'reference' parameters to this method.
	 *				         See the IOUserClientMethodDispatch definition.
	 * @param       target Target for the dispatch function
	 * @param       reference Reference constant for the dispatch function
	 * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
	 */

	virtual kern_return_t
	ExternalMethod(
		uint64_t                            selector,
	    IOUserClientMethodArguments       * arguments,
	    const IOUserClientMethodDispatch  * dispatch,
	    OSObject                          * target,
	    void                              * reference) LOCALONLY
	QUEUENAME(IOUserClientQueueExternalMethod);


    /*!
     * @brief       Send asynchronous arguments to a completion supplied by ExternalMethod().
     * @discussion  IOConnectAsyncMethod calls from the owner of the connection come will pass an OSAction instance.
     *              To deliver the asynchronous results the driver calls AsyncCompletion().
     * @param       action OSAction passed to IOExternalMethod().
     * @param       status An IOReturn status value to be sent.
     * @param       asyncData An array of scalar data to be sent.
     * @param       asyncDataCount Count of valid data in asyncData.
     */
    virtual void
    AsyncCompletion(
        OSAction                            * action TARGET,
        IOReturn                              status,
        const IOUserClientAsyncArgumentsArray asyncData,
		uint32_t                              asyncDataCount) = 0;

    /*!
     * @brief       Return an IOMemoryDescriptor to be mapped into the client task.
     * @discussion  IOConnectMapMemory()/UnmapMemory() will result in a call to this method to obtain
     *              an IOMemoryDescriptor instance for shared memory. For a given IOUserClient instance, calling
     *              CopyClientMemoryForType() with a given type, should return the same IOMemoryDescriptor instance.
     * @param       type Type parameter IOConnectMapMemory()/UnmapMemory().
     * @param       options Set kIOUserClientMemoryReadOnly for memory to be mapped read only in the client.
     * @param       memory An instance of IOMemoryDescriptor on success. One reference will be consumed by the caller
     *              of this method.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	CopyClientMemoryForType(
		uint64_t                              type,
	    uint64_t                            * options,
	    IOMemoryDescriptor                 ** memory) = 0;

    /*!
     * @brief       Create a memory descriptor that describes a set of virtual ranges in
     *              the client task of the user client.
     * @param       memoryDescriptorCreateOptions
	 *              kIOMemoryDirectionIn memory described will be writable
	 *              kIOMemoryDirectionOut memory described will be readable
	 * @param       segmentsCount Number of valid address ranges being passed
	 *              in the segments array.
	 * @param       segments Array of address ranges.
	 * @param       memory Returned IOMemoryDescriptor object with +1 retain count.
     * @return      kIOReturnSuccess on success. See IOReturn.h for error codes.
     */
	virtual kern_return_t
	CreateMemoryDescriptorFromClient(
		uint64_t memoryDescriptorCreateOptions,
		uint32_t segmentsCount,
		const IOAddressSegment segments[32],
		IOMemoryDescriptor ** memory) __attribute__((availability(driverkit,introduced=20.0)));

   /*!
    * @function CopyClientEntitlements
    * @abstract Return owning task's entitlements dictionary.
    * @param    entitlements Dictionary of entitlements given to the owning task. To be released by caller.
    * @return   kIOReturnSuccess on success. See IOReturn.h for error codes.
	*/
	virtual kern_return_t
	CopyClientEntitlements(OSDictionary ** entitlements) LOCAL;


private:
	virtual kern_return_t
	_ExternalMethod(
		uint64_t                              selector,
		const IOUserClientScalarArray		  scalarInput,
		uint32_t							  scalarInputCount,
		OSData 							    * structureInput,
		IOMemoryDescriptor                  * structureInputDescriptor,
		IOUserClientScalarArray               scalarOutput,
		uint32_t                            * scalarOutputCount,
		uint64_t                              structureOutputMaximumSize,
		OSData                             ** structureOutput,
		IOMemoryDescriptor                  * structureOutputDescriptor,
        OSAction                            * completion TYPE(IOUserClient::AsyncCompletion)) LOCAL
	QUEUENAME(IOUserClientQueueExternalMethod);

    virtual void
    KernelCompletion(
        OSAction                            * action TARGET,
        IOReturn                              status,
        const IOUserClientAsyncArgumentsArray asyncData,
		uint32_t                              asyncDataCount)
        KERNEL
        TYPE(IOUserClient::AsyncCompletion);
};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class IOUserClient IOUserClient.iig:157-300 */

#define IOUserClient_AsyncCompletion_ID            0xdbc5b2e5d2b446f4ULL
#define IOUserClient_CopyClientMemoryForType_ID            0x8399bdb3d0b4f474ULL
#define IOUserClient_CreateMemoryDescriptorFromClient_ID            0xf2fa2faa5cc11191ULL
#define IOUserClient_CopyClientEntitlements_ID            0xcaf3bd8932c8486fULL
#define IOUserClient__ExternalMethod_ID            0xcfe0c99e739d92f9ULL
#define IOUserClient_KernelCompletion_ID            0xf609f134c9046444ULL

#define IOUserClient_AsyncCompletion_Args \
        OSAction * action, \
        IOReturn status, \
        const unsigned long long * asyncData, \
        uint32_t asyncDataCount

#define IOUserClient_CopyClientMemoryForType_Args \
        uint64_t type, \
        uint64_t * options, \
        IOMemoryDescriptor ** memory

#define IOUserClient_CreateMemoryDescriptorFromClient_Args \
        uint64_t memoryDescriptorCreateOptions, \
        uint32_t segmentsCount, \
        const IOAddressSegment * segments, \
        IOMemoryDescriptor ** memory

#define IOUserClient_CopyClientEntitlements_Args \
        OSDictionary ** entitlements

#define IOUserClient__ExternalMethod_Args \
        uint64_t selector, \
        const unsigned long long * scalarInput, \
        uint32_t scalarInputCount, \
        OSData * structureInput, \
        IOMemoryDescriptor * structureInputDescriptor, \
        unsigned long long * scalarOutput, \
        uint32_t * scalarOutputCount, \
        uint64_t structureOutputMaximumSize, \
        OSData ** structureOutput, \
        IOMemoryDescriptor * structureOutputDescriptor, \
        OSAction * completion

#define IOUserClient_KernelCompletion_Args \
        OSAction * action, \
        IOReturn status, \
        const unsigned long long * asyncData, \
        uint32_t asyncDataCount

#define IOUserClient_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(IOUserClient * self, const IORPC rpc);\
\
    void\
    AsyncCompletion(\
        OSAction * action,\
        IOReturn status,\
        const IOUserClientAsyncArgumentsArray asyncData,\
        uint32_t asyncDataCount,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CopyClientMemoryForType(\
        uint64_t type,\
        uint64_t * options,\
        IOMemoryDescriptor ** memory,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateMemoryDescriptorFromClient(\
        uint64_t memoryDescriptorCreateOptions,\
        uint32_t segmentsCount,\
        const IOAddressSegment * segments,\
        IOMemoryDescriptor ** memory,\
        OSDispatchMethod supermethod = NULL) __attribute__((availability(driverkit,introduced=20.0)));\
\
    kern_return_t\
    CopyClientEntitlements(\
        OSDictionary ** entitlements,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    _ExternalMethod(\
        uint64_t selector,\
        const IOUserClientScalarArray scalarInput,\
        uint32_t scalarInputCount,\
        OSData * structureInput,\
        IOMemoryDescriptor * structureInputDescriptor,\
        IOUserClientScalarArray scalarOutput,\
        uint32_t * scalarOutputCount,\
        uint64_t structureOutputMaximumSize,\
        OSData ** structureOutput,\
        IOMemoryDescriptor * structureOutputDescriptor,\
        OSAction * completion,\
        OSDispatchMethod supermethod = NULL);\
\
    kern_return_t\
    CreateActionKernelCompletion(size_t referenceSize, OSAction ** action);\
\
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    CopyClientEntitlements_Impl(IOUserClient_CopyClientEntitlements_Args);\
\
    kern_return_t\
    _ExternalMethod_Impl(IOUserClient__ExternalMethod_Args);\
\
\
public:\
    /* _Invoke methods */\
\
    typedef void (*AsyncCompletion_Handler)(OSMetaClassBase * target, IOUserClient_AsyncCompletion_Args);\
    static kern_return_t\
    AsyncCompletion_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        AsyncCompletion_Handler func,\
        const OSMetaClass * targetActionClass);\
\
    static kern_return_t\
    AsyncCompletion_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        AsyncCompletion_Handler func);\
\
    typedef kern_return_t (*CopyClientMemoryForType_Handler)(OSMetaClassBase * target, IOUserClient_CopyClientMemoryForType_Args);\
    static kern_return_t\
    CopyClientMemoryForType_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyClientMemoryForType_Handler func);\
\
    typedef kern_return_t (*CreateMemoryDescriptorFromClient_Handler)(OSMetaClassBase * target, IOUserClient_CreateMemoryDescriptorFromClient_Args);\
    static kern_return_t\
    CreateMemoryDescriptorFromClient_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CreateMemoryDescriptorFromClient_Handler func);\
\
    typedef kern_return_t (*CopyClientEntitlements_Handler)(OSMetaClassBase * target, IOUserClient_CopyClientEntitlements_Args);\
    static kern_return_t\
    CopyClientEntitlements_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        CopyClientEntitlements_Handler func);\
\
    typedef kern_return_t (*_ExternalMethod_Handler)(OSMetaClassBase * target, IOUserClient__ExternalMethod_Args);\
    static kern_return_t\
    _ExternalMethod_Invoke(const IORPC rpc,\
        OSMetaClassBase * target,\
        _ExternalMethod_Handler func);\
\


#define IOUserClient_KernelMethods \
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    CreateMemoryDescriptorFromClient_Impl(IOUserClient_CreateMemoryDescriptorFromClient_Args);\
\
    void\
    KernelCompletion_Impl(IOUserClient_KernelCompletion_Args);\
\


#define IOUserClient_VirtualMethods \
\
public:\
\
    virtual bool\
    init(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    free(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    ExternalMethod(\
        uint64_t selector,\
        IOUserClientMethodArguments * arguments,\
        const IOUserClientMethodDispatch * dispatch,\
        OSObject * target,\
        void * reference) APPLE_KEXT_OVERRIDE;\
\





#define OSAction_IOUserClient_KernelCompletion_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(OSAction_IOUserClient_KernelCompletion * self, const IORPC rpc);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\


#define OSAction_IOUserClient_KernelCompletion_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define OSAction_IOUserClient_KernelCompletion_VirtualMethods \
\
public:\
\



class OSAction_IOUserClient_KernelCompletionInterface : public OSInterface
{
public:
};

struct OSAction_IOUserClient_KernelCompletion_IVars;
struct OSAction_IOUserClient_KernelCompletion_LocalIVars;

class __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer"))) OSAction_IOUserClient_KernelCompletion : public OSAction, public OSAction_IOUserClient_KernelCompletionInterface
{
    OSDeclareDefaultStructorsWithDispatch(OSAction_IOUserClient_KernelCompletion);


public:
#ifdef OSAction_IOUserClient_KernelCompletion_DECLARE_IVARS
OSAction_IOUserClient_KernelCompletion_DECLARE_IVARS
#else /* OSAction_IOUserClient_KernelCompletion_DECLARE_IVARS */
    union
    {
        OSAction_IOUserClient_KernelCompletion_IVars * ivars;
        OSAction_IOUserClient_KernelCompletion_LocalIVars * lvars;
    };
#endif /* OSAction_IOUserClient_KernelCompletion_DECLARE_IVARS */

    using super = OSAction;


    OSAction_IOUserClient_KernelCompletion_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */

/* IOUserClient.iig:302- */

#endif /* ! _IOKIT_UIOUSERCLIENT_H */
