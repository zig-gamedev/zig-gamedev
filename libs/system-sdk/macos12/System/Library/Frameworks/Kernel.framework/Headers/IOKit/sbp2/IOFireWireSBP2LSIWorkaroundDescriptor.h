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

#if __ppc__

/*!
  @header IOFireWireSBP2LSIWorkaroundDescriptor
  Contains the class definition for IOFireWireSBP2LSIWorkaroundDescriptor.
*/

#ifndef _IOKIT_IOFIREWIRESBP2LSIWORKAROUNDDESCRIPTOR_H
#define _IOKIT_IOFIREWIRESBP2LSIWORKAROUNDDESCRIPTOR_H

#include <IOKit/IOMemoryDescriptor.h>

class IOFireWireSBP2LSIRange;

/*!
    @class IOFireWireSBP2LSIWorkaroundDescriptor
    @abstract This is a subclass of IOGeneralMemoryDescriptor. It is designed to work around
    a hardware problem in a common SBP2 target.  It takes a memory descriptor as
	an argument.  It retains this descriptor and returns a new one with resegmented
	physical segments and potentially some "unfixable" segments double buffered.

    If the transfer will cause data to be sent from the Mac, syncBuffersForOutput
    must be called before the transfer.
    
    If the transfer will cause data to be recieve by the Mac, syncBuffersForInput
    must be called after the transfer.

    This class calculates the new segmentation only once when it is created.
    Any changes to the structure of the original memory descriptor will render
    this one useless.  It must be released and a new IOFireWireSBP2LSIWorkaroundDescriptor
    can be created.
    
    LSI Bridge Workaround Algorithm Details
    
    Goals:
    1. Avoid LSI Logic "< 16 byte" bug - prevent any data packet smaller than 16 bytes
    2. Minimize double-buffering
    3. Support non-multiple-of-512-byte devices, e.g. CD Audio
    
    Solution:
    Write a page table such that the bridge will nor use packets smaller than 16 bytes.
    In other words, rearrange the memory descriptor to avoid the bug, and do it such
    that the SBP-2 layer will not break up a segment (du ro the 64k-1 limit) and
    re-introduces the problem

    SBP-2 defines the kFWSBP2MaxPageClusterSize constant.   We simply make sure
    none of our segments are larger than this size and SBP-2 will not break them
    up when it writes the SBP-2 page table.
    
    Notes:
    - Some double buffering is unavoidable.  Discontiguous pages may yield page
    fragments at the start or end of the buffer (or both, with non-512x buffers).
    solution uses less than 33 bytes of double-buffer per segment in original
    memory descriptor.
    - If driver must break up IO to meet ATA limit of 255 (250?) blocks, assume
    the driver does this at a higher level (before applying workaround).
    - It is expected that the original memory descriptor has been prepared (wired)
*/

class IOFireWireSBP2LSIWorkaroundDescriptor : public IOGeneralMemoryDescriptor
{
	OSDeclareDefaultStructors(IOFireWireSBP2LSIWorkaroundDescriptor)

	friend class IOFireWireSBP2LSIRange;

protected:

    // reserved for future use
    struct ExpansionData { };
    ExpansionData *reserved; 
    
	bool	fFixedCapacity; 	// for both allocators
	
	//////////////////////////////////////
	// for range allocator

	OSArray *				fPermanentRanges;
	UInt32					fAllocatedRangesCount;

	virtual IOReturn rangeAllocatorInitialize( UInt32 rangeCount );
	virtual void rangeAllocatorDeallocateAllRanges( void );
	virtual IOFireWireSBP2LSIRange * rangeAllocatorNewRange( void );
	virtual void rangeAllocatorFree( void );

	//////////////////////////////////////
	// for buffer allocator
	
	OSArray *				fBufferDescriptors;
	UInt32					fPermanentPages;
	IOByteCount				fAllocatedBytesCount;

	virtual IOReturn bufferAllocatorInitialize( IOByteCount requestedBufferSize );
	virtual void bufferAllocatorDeallocateAllBuffers( void );
	virtual void * bufferAllocatorNewBuffer( IOPhysicalAddress * address );
	virtual void bufferAllocatorFree( void );

	//////////////////////////////////////
	// for range table allocator
	
	IOPhysicalRange * 		fRangeTable;
	IOByteCount				fRangeTableSize;

	virtual IOReturn rangeTableAllocatorInitialize( UInt32 requestedBufferSize );
	virtual IOPhysicalRange * rangeTableAllocatorNewTable( UInt32 entries );
	virtual void rangeTableAllocatorFree( void );

	//////////////////////////////////////
	// for workaround

	IOMemoryDescriptor * 	fOriginalDesc;	
	OSArray *				fRanges;	
	IOByteCount 			fOffset;
	IOByteCount				fLength;
	IODirection				fDirection;
	
						  
	virtual bool initWithCapacity
					( UInt32 permanentRanges, IOByteCount permanentBufferSpace, bool fixedCapacity );

	virtual void free();
	
	virtual IOReturn resetToInitialCapacity( void );
	virtual IOReturn initializeRangesArray( void );
	virtual IOReturn recalculateSmallSegments( void );
	virtual IOReturn splitLargeSegments( void );
	virtual IOReturn resegmentOddLengthSegments( void );
	virtual IOReturn initializeBuffers( void );

	virtual bool initWithAddress( void *      address,       /* not supported */
                                  IOByteCount withLength,
                                  IODirection withDirection );

    virtual bool initWithAddress( vm_address_t address,      /* not supported */
                                  IOByteCount  withLength,
                                  IODirection  withDirection,
                                  task_t       withTask );

    virtual bool initWithPhysicalAddress( 
                                  IOPhysicalAddress address, /* not supported */
                                  IOByteCount       withLength,
                                  IODirection       withDirection );

    virtual bool initWithPhysicalRanges( 
                                  IOPhysicalRange * ranges,  /* not supported */
                                  UInt32            withCount,
                                  IODirection       withDirection,
                                  bool              asReference = false );

    virtual bool initWithRanges(  IOVirtualRange * ranges,   /* not supported */
                                  UInt32           withCount,
                                  IODirection      withDirection,
                                  task_t           withTask,
                                  bool             asReference = false );

public:
	
	// static factory methods and intializers 
	
	//
	// create a new instance
	//
	
	
    /*!
		@function withDescriptor
		@abstract Creates a new IOFireWireSBP2LSIWorkaroundDescriptor.
		@discussion	Create a IOFireWireSBP2LSIWorkaroundDescriptor with no permanent capacity then 
        inits it the given descriptor. This is basicly a short cut for calling 
        withCapacity( 0, 0, false) and the initWithDescriptor()
        @param desc Original memory descriptor.
        @param offset Offset of data to "fix" in bytes from beginning of descriptor.
        @param len Length of data in bytes to "fix"
        @param direction IODirection of data transfer.
        @result Returns a new IOFireWireSBP2LSIWorkaroundDescriptor if successful.
	*/
    
	static IOFireWireSBP2LSIWorkaroundDescriptor * withDescriptor
					( IOMemoryDescriptor * desc, IOByteCount offset = 0,  
					  IOByteCount len = 0, IODirection direction = kIODirectionInOut );
	
	// initialize with descriptor				  
	
    /*!
		@function initWithDescriptor
		@abstract Initialize an IOFireWireSBP2LSIWorkaroundDescriptor with the given descriptor.
		@discussion	Initialize the workaround descriptor with the given descriptor. 
        @param desc Original memory descriptor.
        @param offset Offset of data to "fix" in bytes from beginning of descriptor.
        @param len Length of data in bytes to "fix"
        @param direction IODirection of data transfer.
        @result Returns true if the initialization was successful.
	*/
    
    virtual bool initWithDescriptor( IOMemoryDescriptor * desc, IOByteCount offset = 0,  
								     IOByteCount len = 0, IODirection direction = kIODirectionInOut );
	
	
	///////////////////////////////////
	
    
    /*!
		@function withCapacity
		@abstract Create a new descriptor with possibly a permanent capacity.
		@discussion	Create and IOFireWireSBP2LSIWorkaroundDescriptor with a permanent fixed capacity. 
        You should call initWithDescriptor afterward. permanentRanges is number of ranges to keep 
        permanently allocated for use by the algorithm.  If fixedCapacity is false additional ranges 
        may be allocated and deallocated dyanmicly if needed. The algorithm may require more or less 
        ranges than either the original descriptor or the final fixed may decriptor contain.
        permanentBufferSpace is the number of bytes of permanent buffer to keep arround. If fixedCapacity 
        is false additional buffer space may be allocated and deallocated dynamically. permanentBufferSpace 
        should generally be set to 32 * maximum number of ranges. fixedCapacity is a flag indicating 
        whether dynamic allocations are allowed. When making decisions about the maximum amount of 
        buffer space to keep around, it should be noted tha the maximum number of ranges maybe different 
        from permanentRanges if fixedCapcity is false.
        @param permanentRanges Count of permanent ranges.
        @param permanentBufferSpace Byte count of permanent buffers.
        @param fixedCapacity bool indicating if dynamic allocations can be made.
        @result Returns true if the initialization was successful.
	*/
    
	static IOFireWireSBP2LSIWorkaroundDescriptor * withCapacity
						( UInt32 permanentRanges, IOByteCount permanentBufferSpace, bool fixedCapacity );


	///////////////////////////////////

	//
	// manipulate buffers for IO
	//

    /*!
		@function syncBuffersForOutput
		@abstract Synchronize the buffers for output.
		@discussion	Since double buffering may be invovled in the workaround.  The driver needs to 
        indicate when these buffers should be syncronized with the original descriptor.  For data 
        that will be output syncBuffersForOutput should be called before submiting the ORB.
        @result Returns kIOReturnSuccess if sync was successful.
	*/
    	
	virtual IOReturn syncBuffersForOutput( void );  // call before output

    /*!
		@function syncBuffersForInput
		@abstract Synchronize the buffers for input.
		@discussion	Since double buffering may be invovled in the workaround.  The driver needs to 
        indicate when these buffers should be syncronized with the original descriptor.  For data 
        that will be input syncBuffersForOutput should be called after receiving completion status 
        for the ORB.
        @result Returns kIOReturnSuccess if sync was successful.
	*/
    
    virtual IOReturn syncBuffersForInput( void );	// call after input
    
private:

    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 0);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 1);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 2);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 3);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 4);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 5);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 6);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 7);
    OSMetaClassDeclareReservedUnused(IOFireWireSBP2LSIWorkaroundDescriptor, 8);
    
};

#endif

#endif
