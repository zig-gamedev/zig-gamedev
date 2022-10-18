/*
* Copyright (c) 1998-2002 Apple Computer, Inc. All rights reserved.
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

#ifndef __IOLOCALCONFIGDIRECTORY_H__
#define __IOLOCALCONFIGDIRECTORY_H__

#include <libkern/c++/OSObject.h>
#include <IOKit/IOReturn.h>
#include <IOKit/firewire/IOFireWireFamilyCommon.h>
#include <IOKit/firewire/IOConfigDirectory.h>

class OSArray;
class OSData;
class IOFireWireController;
class IOFWUserObjectExporter ;

/*! @class IOLocalConfigDirectory
*/
class IOLocalConfigDirectory : public IOConfigDirectory
{
	friend class IOFireWireController;
	friend class IOFireWireUserClient ;

	OSDeclareDefaultStructors(IOLocalConfigDirectory);

protected:
	OSArray *fEntries;	// Entries for this directory.
	OSData *fROM;	// Local ROM, if compiled.
	UInt32 fHeader;	// Num entries and CRC.
	
/*! @struct ExpansionData
	@discussion This structure will be used to expand the capablilties of the class in the future.
	*/    
	struct ExpansionData { };

/*! @var reserved
	Reserved for future use.  (Internal use only)  */
	ExpansionData *reserved;

	virtual bool init(void) APPLE_KEXT_OVERRIDE;
	virtual void free(void) APPLE_KEXT_OVERRIDE;

	virtual const UInt32 *getBase(void) APPLE_KEXT_OVERRIDE;
	virtual IOConfigDirectory *getSubDir(int start, int type) APPLE_KEXT_OVERRIDE;

public:
	static IOLocalConfigDirectory *create();

	/*!
		@function update
		makes sure that the ROM has at least the specified capacity,
		and that the ROM is uptodate from its start to at least the
		specified quadlet offset.
		@result kIOReturnSuccess if the specified offset is now
		accessable at romBase[offset].
	*/
	virtual IOReturn update(UInt32 offset, const UInt32 *&romBase) APPLE_KEXT_OVERRIDE;

	virtual IOReturn compile(OSData *rom);
	
	// All flavours of addEntry eat a retain of the desc string
	virtual IOReturn addEntry(int key, UInt32 value, OSString *desc = NULL);
	virtual IOReturn addEntry(int key, IOLocalConfigDirectory *value,
							OSString *desc = NULL);
	virtual IOReturn addEntry(int key, OSData *value, OSString *desc = NULL);
	virtual IOReturn addEntry(int key, FWAddress value, OSString *desc = NULL);
	virtual IOReturn removeSubDir(IOLocalConfigDirectory *value);
	const OSArray *getEntries() const;

	virtual IOReturn getIndexValue(int index, IOConfigDirectory *&value) APPLE_KEXT_OVERRIDE;

protected:

	virtual const UInt32 * lockData( void ) APPLE_KEXT_OVERRIDE;
	virtual void unlockData( void ) APPLE_KEXT_OVERRIDE;
	virtual IOReturn updateROMCache( UInt32 offset, UInt32 length ) APPLE_KEXT_OVERRIDE;
	virtual IOReturn checkROMState( void ) APPLE_KEXT_OVERRIDE;

	// call eats a retain count
	virtual IOReturn addEntry(OSString *desc);

	IOReturn	incrementGeneration( void );
	static void	exporterCleanup( const OSObject * self, IOFWUserObjectExporter * exporter ) ;
		
private:
	OSMetaClassDeclareReservedUsed(IOLocalConfigDirectory, 0);
	OSMetaClassDeclareReservedUnused(IOLocalConfigDirectory, 1);
	OSMetaClassDeclareReservedUnused(IOLocalConfigDirectory, 2);
};

#endif /* __IOLOCALCONFIGDIRECTORY_H__ */
