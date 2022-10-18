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

#ifndef __IOCONFIGDIRECTORY_H__
#define __IOCONFIGDIRECTORY_H__

#include <libkern/c++/OSObject.h>
#include <IOKit/IOReturn.h>

#include <IOKit/firewire/IOFireWireFamilyCommon.h>

class OSData;
class OSString;
class OSIterator;
class IOFireWireDevice;

/*! @class IOConfigDirectory 
*/
class IOConfigDirectory : public OSObject
{
    OSDeclareAbstractStructors(IOConfigDirectory);

protected:
    int fStart;			// Offset into ROM of start of this dir
    int fType;			// Directory type
    int fNumEntries;		// In directory
    
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of the class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData *reserved;

    virtual bool initWithOffset(int start, int type);

    virtual const UInt32 *getBase() = 0;
    virtual IOReturn createIterator(UInt32 testVal, UInt32 testMask,
                                    OSIterator *&iterator);
    virtual IOConfigDirectory *getSubDir(int start, int type) = 0;

public:
    /*!
        @function update
        makes sure that the ROM has at least the specified capacity,
        and that the ROM is uptodate from its start to at least the
        specified quadlet offset.
        @result kIOReturnSuccess if the specified offset is now
        accessable at romBase[offset].
    */
    virtual IOReturn update(UInt32 offset, const UInt32 *&romBase) = 0;

    /*!
        @function getKeyType
        Gets the data type for the specified key
        @param type on return, set to the data type
        @result kIOReturnSuccess if the key exists in the dictionary
    */
    virtual IOReturn getKeyType(int key, IOConfigKeyType &type);
    /*!
        @function getKeyValue
        Gets the value for the specified key, in a variety of forms.
        @param value on return, set to the data type
        @param text if non-zero, on return points to the
        string description of the field, or NULL if no text found.
        @result kIOReturnSuccess if the key exists in the dictionary
        and is of a type appropriate for the value parameter
        @param value reference to variable to store the entry's value
    */
    virtual IOReturn getKeyValue(int key, UInt32 &value, OSString** text = NULL);
    virtual IOReturn getKeyValue(int key, OSData *&value,
                                 OSString** text = NULL);
    virtual IOReturn getKeyValue(int key, IOConfigDirectory *&value,
                                 OSString** text = NULL);
    virtual IOReturn getKeyOffset(int key, FWAddress &value,
                                  OSString** text = NULL);


    /*!
        @function getIndexType
        Gets the data type for entry at the specified index
        @param type on return, set to the data type
        @result kIOReturnSuccess if the index exists in the dictionary
    */
    virtual IOReturn getIndexType(int index, IOConfigKeyType &type);
    /*!
        @function getIndexKey
        Gets the key for entry at the specified index
        @param key on return, set to the key
        @result kIOReturnSuccess if the index exists in the dictionary
    */
    virtual IOReturn getIndexKey(int index, int &key);

    /*!
        @function getIndexValue
        Gets the value at the specified index of the directory,
        in a variety of forms.
        @param type on return, set to the data type
        @result kIOReturnSuccess if the index exists in the dictionary
        and is of a type appropriate for the value parameter
        @param value reference to variable to store the entry's value
    */
    virtual IOReturn getIndexValue(int index, UInt32 &value);
    virtual IOReturn getIndexValue(int index, OSData *&value);
    virtual IOReturn getIndexValue(int index, OSString *&value);
    virtual IOReturn getIndexValue(int index, IOConfigDirectory *&value);
    virtual IOReturn getIndexOffset(int index, FWAddress &value);
    virtual IOReturn getIndexOffset(int index, UInt32 &value);

    /*!
        @function getIndexEntry
        Gets the entry at the specified index of the directory,
        as a raw UInt32.
        @param entry on return, set to the entry value
        @result kIOReturnSuccess if the index exists in the dictionary
        @param value reference to variable to store the entry's value
    */
    virtual IOReturn getIndexEntry(int index, UInt32 &value);

    /*!
        @function getSubdirectories
        Creates an iterator over the subdirectories of the directory.
        @param iterator on return, set to point to an OSIterator
        @result kIOReturnSuccess if the iterator could be created
    */
    virtual IOReturn getSubdirectories(OSIterator *&iterator);

    /*!
        @function getKeySubdirectories
        Creates an iterator over subdirectories of a given type of the directory.
        @param key type of subdirectory to iterate over
        @param iterator on return, set to point to an OSIterator
        @result kIOReturnSuccess if the iterator could be created
    */
    virtual IOReturn getKeySubdirectories(int key, OSIterator *&iterator);

    int getType() const;
	
    int getNumEntries() const;

protected:
	
	virtual const UInt32 * lockData( void ) = 0;
	virtual void unlockData( void ) = 0;
	virtual IOReturn updateROMCache( UInt32 offset, UInt32 length ) = 0;
	virtual IOReturn checkROMState( void ) = 0;
	
private:
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 0);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 1);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 2);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 3);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 4);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 5);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 6);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 7);
    OSMetaClassDeclareReservedUnused(IOConfigDirectory, 8);

};

#endif /* __IOCONFIGDIRECTORY_H__ */
