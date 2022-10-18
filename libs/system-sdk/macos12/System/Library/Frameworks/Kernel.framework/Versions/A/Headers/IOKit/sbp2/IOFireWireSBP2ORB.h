/*
 * Copyright (c) 1998-2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

/*!
  @header IOFireWireSBP2ORB
  Contains the class definition for IOFireWireSBP2ORB.
*/

#ifndef _IOKIT_IOFIREWIRESBP2ORB_H
#define _IOKIT_IOFIREWIRESBP2ORB_H

#include <IOKit/firewire/IOFireWireUnit.h>

#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IOUserClient.h>
#include <IOKit/IODMACommand.h>

enum
{
    kFWSBP2ConstraintForceDoubleBuffer		= (1 << 0)
};

// login option flags
enum
{
    kFWSBP2CommandCompleteNotify			= (1 << 0),
    kFWSBP2CommandTransferDataFromTarget	= (1 << 1),
    kFWSBP2CommandImmediate					= (1 << 2),
    
    kFWSBP2CommandNormalORB					= (1 << 5),
    kFWSBP2CommandReservedORB				= (1 << 6),
    kFWSBP2CommandVendorORB					= (1 << 7),
    kFWSBP2CommandDummyORB					= (1 << 8),
    kFWSBP2CommandCheckGeneration			= (1 << 9),
	
	kFWSBP2CommandFixedSize					= (1 << 10),
	kFWSBP2CommandVirtualORBs				= (1 << 11)     // handy for debugging
};

enum
{
    kFWSBP2MaxPageClusterSize = 0xf000
};

class IOFireWireSBP2ORB;
class IOFireWireSBP2LUN;
class IOFireWireSBP2Login;

/*!
    @class IOFireWireSBP2ORB
    @abstract Represents an SBP2 normal command ORB.  Supplies the APIs for configuring normal
    command ORBs.  This includes setting the command block and writing the page tables for I/O.
    The ORBs are executed using the submitORB method in IOFireWireSBP2Login.
*/

class IOFireWireSBP2ORB : public IOCommand
{
    OSDeclareDefaultStructors( IOFireWireSBP2ORB )

	friend class IOFireWireSBP2Login;
	friend class IOFireWireSBP2ManagementORB;
	
protected:
    
    typedef struct
    {
        UInt32		nextORBAddressHi;
        UInt32		nextORBAddressLo;
        UInt32		dataDescriptorHi;
        UInt32		dataDescriptorLo;
        UInt16		options;
        UInt16		dataSize;
        UInt32		commandBlock[1];  // will be variable sized
     } FWSBP2ORB;

    typedef struct
    {
        UInt16		segmentLength;
        UInt16		segmentBaseAddressHi;
        UInt32		segmentBaseAddressLo;
    } FWSBP2PTE;

private:

    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 3);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 4);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 5);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 6);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 7);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2ORB, 8);

protected:    

    virtual void deallocateBufferAddressSpace( void );
    virtual IOReturn allocateTimer( void );
    virtual void deallocateTimer( void );
    
    /*!
		@function deallocatePageTable
		@abstract Frees up memory allocated for the page table.
		@discussion	Frees all memory associated with the page table.  Undoes what allocatePageTable does. 
        Calling allocatePageTable again will automatically deallocate the existing page table before 
        allocating a new one, so this method is not used in most cases.
	*/
    
    virtual void deallocatePageTable( void );

protected:
	
    // IOFireWireSBP2Login methods
	virtual bool initWithLogin( IOFireWireSBP2Login * login );
    virtual void setNextORBAddress( FWAddress address );

public:

    /*!
		@function allocatePageTable
		@abstract Allocates memory for the page table.
		@discussion	Page table allocation is handle automatically by the ORB, except if the 
        kFWSBP2CommandFixedSize flags is set.  In this case we will fail to write a page table if we 
        need more page table space than we have already.  This method is exposed so these drivers 
        can preallocate as much page table as they need.  This is useful if your driver is part of 
        the paging path and cannot allow allocations to occur.
        @param entryCount number of entries of page table to be allocated.
	*/

    virtual IOReturn allocatePageTable( UInt32 entryCount );

protected:

    virtual bool isTimerSet( void );
    virtual void cancelTimer( void );
    
protected:
  
    // reserved for future use
    struct ExpansionData { };
    ExpansionData *reserved; 
       
    IOFireWireSBP2Login *	fLogin;
    IOFireWireSBP2LUN * 	fLUN;
    IOFireWireUnit *		fUnit;
    IOFireWireController *	fControl;
    IODMACommand *			fDMACommand;
	void *					fUnused2;
	
    UInt32			fCommandFlags;
    UInt32			fMaxPayloadSize;
    UInt32			fTimeoutDuration;
    UInt32			fGeneration;
    UInt64			fRefCon;

	//
    // orb
    //
	
	IOMemoryDescriptor *		fORBDescriptor;
    FWSBP2ORB *					fORBBuffer;
	
    FWAddress 					fORBPseudoAddress;
    IOFWAddressSpace *			fORBPseudoAddressSpace;

    IOFWAddressSpace *			fORBPhysicalAddressSpace;
	FWAddress					fORBPhysicalAddress;

	//
    // page table
    //
	
	UInt32						fPageTableSize;
    IOBufferMemoryDescriptor *	fPageTableDescriptor;

    IOFWAddressSpace *			fPageTablePhysicalAddressSpace;
	FWAddress					fPageTablePhysicalAddress;
	UInt32						fPageTablePhysicalLength;
	
	IOFWAddressSpace *			fPageTablePseudoAddressSpace;   
	FWAddress					fPageTablePseudoAddress;

	//
    // buffers
    //
	
	IOFWAddressSpace *		fBufferAddressSpace;
    bool					fBufferAddressSpaceAllocated;
    IOMemoryDescriptor *	fBufferDescriptor;
    
	//
    // timer
    //
	
	IOFWDelayCommand *		fTimeoutCommand;
    bool					fTimeoutTimerSet;

	bool					fInProgress;
	bool					fIsAppended;
	
    UInt32					fFetchAgentWriteRetries;
    UInt32					fPTECount;
    UInt32					fFetchAgentWriteRetryInterval;

	UInt32					fConstraintOptions;

    virtual IOReturn allocateResources( void );
    virtual void free( void ) APPLE_KEXT_OVERRIDE;

    // orb timeout handler
    static void orbTimeoutStatic( void *refcon, IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );
    virtual void orbTimeout( IOReturn status, IOFireWireBus *bus, IOFWBusCommand *fwCmd );

	// login friend class wrappers
	virtual IOFireWireUnit * getFireWireUnit( void );
	virtual IOFireWireSBP2LUN * getFireWireLUN( void );
  	virtual void deallocateORB( void );
	virtual IOReturn allocateORB( UInt32 orbSize );
    virtual IOReturn removeORB( IOFireWireSBP2ORB * orb );
    virtual void prepareORBForExecution( void );
    virtual void startTimer( void );
    virtual void sendTimeoutNotification( IOFireWireSBP2ORB * orb );

public:
 
    /*! 
        @function release
        @abstract Primary implementation of the release mechanism.
        @discussion See OSObject.h for more information. 
        @param when When retainCount == when then call free(). 
    */
       
	virtual void release() const APPLE_KEXT_OVERRIDE;

    /*! 
        @function getLogin
        @abstract Gets the login associated with this ORB.
        @discussion Returns the IOFireWireSBP2Login object associated with this ORB.
        @result Returns a pointer to an IOFireWireSBP2Login. 
    */
    
    virtual IOFireWireSBP2Login * getLogin( void );

    /*! 
        @function setCommandBuffersAsRanges
        @abstract Creates a page table from a list of ranges.
        @discussion Creates a page table with the given parameters. Any addresses mapped by this method 
        must remain valid until setCommandBuffers is called again or releaseCommandBuffers is called.  
        The SBP2 services do not release references to the command buffers just because the command 
        has completed.
        @param ranges An array of ranges representing the data to be transfered.
        @param withCount The number of ranges in the ranges array.
        @param withDirection An IODirection indicating the direction of data transfer.
        @param withTask The task that these adressses reside in.
        @param offset Offset in bytes into data to begin writing table at.
        @param length Number of bytes of data to map from offset.
        @result Returns KIOReturnSuccess if the page table was written successfully. 
    */
    
    virtual IOReturn setCommandBuffersAsRanges(  IOVirtualRange * ranges,
                                                 UInt32           withCount,
                                                 IODirection      withDirection,
                                                 task_t           withTask,
                                                 UInt32		offset = 0,
                                                 UInt32		length = 0 );
    /*! 
        @function setCommandBuffersAsRanges
        @abstract Creates a page table from a list of ranges.
        @discussion Creates a page table with the given parameters. Any addresses mapped by this method 
        must remain valid until setCommandBuffers is called again or releaseCommandBuffers is called.  
        The SBP2 services do not release references to the command buffers just because the command 
        has completed.
        @param memoryDescriptor IOMemoryDescriptor describe ranges to be written to a page table.
        @param offset Offset in bytes into data to begin writing table at.
        @param length Number of bytes of data to map from offset.
        @result Returns KIOReturnSuccess if the page table was written successfully. 
    */
    
    virtual IOReturn setCommandBuffers( IOMemoryDescriptor * memoryDescriptor, UInt32 offset = 0, 
																				UInt32 length = 0 );


    /*! 
        @function releaseCommandBuffers
        @abstract Releases SBP2's reference to the command buffers.
        @discussion When you create a page table with one of the variants of setCommandBuffers.  
        SBP2 holds on to a reference to the buffers until this method is called.  This means that 
        if a command completed and you manipulated the memory descriptor or released the buffers 
        without calling this method you could leave FW in an inconsistent state.  
        @result Returns KIOReturnSuccess if the page table was cleared successfully. 
    */

    virtual IOReturn releaseCommandBuffers( void );
    
    /*! 
        @function setCommandBlock
        @abstract Sets the command block portion of the ORB.
        @discussion Copys the data provided in the buffer to the command block portion of the ORB.
        @param buffer Pointer to buffer to copy command block from.
        @param length Number of bytes of data to copy.
        @result Returns KIOReturnSuccess if the command block was updated successfully. 
    */
    
    virtual IOReturn setCommandBlock( void * buffer, UInt32 length );
    
    /*! 
        @function setCommandBlock
        @abstract Sets the command block portion of the ORB.
        @discussion Copys the data provided in the buffer to the command block portion of the ORB.
        @param memory IOMemoryDescriptor representing the command block buffer.
        @result Returns KIOReturnSuccess if the command block was updated successfully. 
    */
    
    virtual IOReturn setCommandBlock( IOMemoryDescriptor * memory );
    
    /*! 
        @function getCommandBufferDescriptor
        @abstract Returns the memory descriptor representing the command buffer.
        @discussion Returns the IOMemoryDescriptor for the data mapped by setCommandBuffer variants.  
        Works for setCommandBuffersAsRanges too.
        @result Returns memory descriptor representing mapped data buffers. 
    */
    
	virtual IOMemoryDescriptor * getCommandBufferDescriptor( void );
	
    // accessors
    
    /*! 
        @function setCommandFlags
        @abstract Sets configuration flags for the ORB.
        @discussion Sets the configuration flags for the ORB.  These can be any of the following:
		<p>kFWSBP2CommandCompleteNotify - Set the notify bit as specified in SBP2 standard. Set to receive completion/timeout notification on this ORB.  You almost always want to set this.</p>
		<p>kFWSBP2CommandTransferDataFromTarget - Transfer direction as specified in SBP2 standard.  Set if data is to be written by the device into the host's memory.</p>
		<p>kFWSBP2CommandImmediate - Immediate Append.  ORB address will be written to fetch agent and not chained.  It is only legal to have one immediate ORB in progress at a time.</p>
		<p>kFWSBP2CommandNormalORB - ORB format 0 - Format specified by SBP2 standard.  Set this for most ORBs.</p>
		<p>kFWSBP2CommandReservedORB - ORB format 1 - Format reserved by SBP2 standard for future standardization.</p>
		<p>kFWSBP2CommandVendorORB - ORB format 2 - Format specified by SBP2 standard for vendor dependent ORBs.</p>
		<p>kFWSBP2CommandDummyORB - ORB format 3 - Format specified by SBP2 standard for dummy ORBs.</p>
		<p>kFWSBP2CommandCheckGeneration - If set upon submitORB, the ORB will only be appended if generation set with setCommandGeneration() matches the current generation.  Pretty much all SBP2 drivers need sophisticated logic to track login state, so this is generally not used. </p>
		<p>kFWSBP2CommandFixedSize - Do not allocate more memory for page table if needed.  If there is not enough space in the currently allocated page table, the setCommandBuffers call will fail.  This is important to set if your device is the backing store, as we don't want to cause memory allocations on the paging path. </p>
		<p>kFWSBP2CommandVirtualORBs - Normally ORBs are backed by physical address spaces.  Setting this flag makes this ORB backed by a pseudo address space.  This can make ORBs easier to see in a bus trace.  Virtual ORBs will have an address in the form of ffcX.XXXX.0000.0000.  Pseudo address space backed ORBs are slower, so you won't want to set for deployment builds.</p>
        @param flags The flags to be set.
    */
    
    virtual void setCommandFlags( UInt32 flags );

    /*! 
        @function getCommandFlags
        @abstract Sets configuration flags for the ORB.
        @discussion Returns the current configuration flags set on this ORB.
        @result Return The current ORB flags.
    */
    
    virtual UInt32 getCommandFlags( void );

    /*! 
        @function setMaxPayloadSize
        @abstract Sets max payload size for the ORB.
        @discussion This sets the maximum payload size for this ORB only.  This size is clipped by 
        the global max payload size set in the login object.
        @param maxPayloadSize The maximum payload size in bytes.
    */
    
    virtual void setMaxPayloadSize( UInt32 maxPayloadSize );

    /*! 
        @function getMaxPayloadSize
        @abstract Gets max payload size for the ORB.
        @discussion This gets the maximum payload size for this ORB only.
        @result Returns the maximum payload size in bytes.
    */
    
    virtual UInt32 getMaxPayloadSize( void );

    /*! 
        @function setCommandTimeout
        @abstract Sets the timeout of the ORB.
        @discussion This sets the timeout for the ORB in milliseconds. Note that ORBs without timeouts 
        can be "lost."  You will obviously not recieve timeout notification for timeouts of zero.  But 
        perhaps less obviously you will not recieve orb reset notification, which is really a sort of
        accelerated timeout notification for bus reset situations.  
        @param timeout The timeout duration in milliseconds.
    */
    
    virtual void setCommandTimeout( UInt32 timeout );
    
    /*! 
        @function getCommandTimeout
        @abstract Gets the timeout of the ORB.
        @discussion This method gets the timeout for this ORB in milliseconds.
        @result Returns the timeout for the orb in milliseconds.
    */
    
    virtual UInt32 getCommandTimeout( void );


    /*! 
        @function setCommandGeneration
        @abstract Sets the command generation.
        @discussion This sets the bus generation this ORB should be appended in.  It is only meaningful 
        when combined with the kFWSBP2CommandCheckGeneration flags above.
        @param gen The bus generation for command execution.
    */
    
    virtual void setCommandGeneration( UInt32 gen );
    
    /*! 
        @function getCommandGeneration
        @abstract Gets the command generation.
        @discussion This gets the bus generation this ORB should be appended in.
        @result Returns the bus generation for command execution.
    */
    
    virtual UInt32 getCommandGeneration( void );

    /*!
		@function setRefCon
		@abstract Sets the ORB refCon.
		@discussion Sets a user defined value on the ORB that can be retrieved later with the 
        method getRefCon.
        @param refCon a user defined value.
    */
    
    virtual void setRefCon( void * refCon );

    /*!
		@function getRefCon
		@abstract Returns the refCon set with setRefCon.
		@discussion Returns the user defined value previously stored in the ORB with setRefCon.
        @result Returns the previously stored user defined value.
	*/
    
    virtual void * getRefCon( void );

protected:    

    virtual void setToDummy( void );

public:	
    /*!
		@function getORBAddress
		@abstract Returns the FireWire address of this ORB.
		@discussion Returns the FireWire bus address of this ORB.  This is not the same as the Macintosh 
        address for the IOFireWireSBP2ORB.
        @result Returns the FireWire address of this ORB.
	*/
    
    virtual void getORBAddress( FWAddress * address );

protected:
	
	virtual bool isAppended( void );
	virtual void setIsAppended( bool state );
	virtual UInt32 getFetchAgentWriteRetries( void );
    virtual void setFetchAgentWriteRetries( UInt32 retries );

    virtual void prepareFastStartPacket( IOBufferMemoryDescriptor * descriptor );

	UInt32 getFetchAgentWriteRetryInterval( void );
    void setFetchAgentWriteRetryInterval( UInt32 interval );

	IOReturn completeBufferAddressSpace( void );
	IOReturn prepareBufferAddressSpace( IOMemoryDescriptor * memoryDescriptor );

public:

    /*!
		@function setBufferConstraints
		@abstract Configures page table generation parameters
		@discussion Sets the maximums size of any page table segment and the required alignemnt.  Double buffering
		may be used to satisfy these constraints. The only supported option is kFWSBP2ConstraintForceDoubleBuffer which
		forces a page aligned double buffering of the entire descriptor.
        @result May return an error if there is a problem allocating the underlying resources or if buffers are currently attached.
	*/

	IOReturn setBufferConstraints( UInt64 maxSegmentSize, UInt32 alignment, UInt32 options = 0);


    /*! 
        @function setCommandBuffersAsRanges64
        @abstract Creates a page table from a list of 64 bit ranges.
        @discussion Creates a page table with the given parameters. Any addresses mapped by this method 
        must remain valid until setCommandBuffers is called again or releaseCommandBuffers is called.  
        The SBP2 services do not release references to the command buffers just because the command 
        has completed. This is a 64 bit compatible version of setCommandBuffersAsRanges.
        @param ranges An array of ranges representing the data to be transfered.
        @param withCount The number of ranges in the ranges array.
        @param withDirection An IODirection indicating the direction of data transfer.
        @param withTask The task that these adressses reside in.
        @param offset Offset in bytes into data to begin writing table at.
        @param length Number of bytes of data to map from offset.
        @result Returns KIOReturnSuccess if the page table was written successfully. 
    */
	   
    IOReturn setCommandBuffersAsRanges64(	IOAddressRange *	ranges,
											uint64_t			withCount,
											IODirection			withDirection,
											task_t				withTask,
											uint64_t			offset = 0,
											uint64_t			length = 0);

    /*!
		@function setRefCon64
		@abstract Sets the ORB refCon as a 64 bit value.
		@discussion Sets a user defined value on the ORB that can be retrieved later with the 
        method getRefCon.
        @param refCon a user defined value.
    */
    
    virtual void setRefCon64( UInt64 refCon );

    /*!
		@function getRefCon64
		@abstract Returns the 64 bit refCon set with setRefCon64.
		@discussion Returns the user defined value previously stored in the ORB with setRefCon.
        @result Returns the previously stored user defined value.
	*/
    
    virtual UInt64 getRefCon64( void );

protected:
	
	UInt32	calculateTransferSizeLog( bool * clipping );


};
    
#endif
