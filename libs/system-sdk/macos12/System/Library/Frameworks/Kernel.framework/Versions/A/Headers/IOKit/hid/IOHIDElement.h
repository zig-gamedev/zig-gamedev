/*
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_HID_IOHIDELEMENT_H
#define _IOKIT_HID_IOHIDELEMENT_H

#include <libkern/c++/OSArray.h>
#include <libkern/c++/OSData.h>
#include <IOKit/hid/IOHIDKeys.h>

//===========================================================================
// An object that describes a single HID element.

#if defined(KERNEL) && !defined(KERNEL_PRIVATE)
class __deprecated_msg("Use DriverKit") IOHIDElement : public OSCollection
#else
class IOHIDElement : public OSCollection
#endif
{
    OSDeclareAbstractStructors( IOHIDElement )
    
    /* Internal use only  */
    struct ExpansionData { 
    };
    ExpansionData *             _reserved;


public:

    virtual IOHIDElementCookie              getCookie()            = 0;
    virtual IOHIDElement *                  getParentElement()     = 0;
    virtual OSArray *                       getChildElements()     = 0;
    virtual IOHIDElementType                getType()              = 0;
    virtual IOHIDElementCollectionType      getCollectionType()    = 0;
    virtual UInt32                          getUsagePage()         = 0;
    virtual UInt32                          getUsage()             = 0;
    virtual UInt32                          getLogicalMin()        = 0;
    virtual UInt32                          getLogicalMax()        = 0;
    virtual UInt32                          getPhysicalMin()       = 0;
    virtual UInt32                          getPhysicalMax()       = 0;
    virtual UInt32                          getUnitExponent()      = 0;
    virtual UInt32                          getUnit()              = 0;
    virtual UInt32                          getReportSize()        = 0;
    virtual UInt32                          getReportCount()       = 0;
    virtual UInt32                          getReportID()          = 0;
    virtual UInt32                          getFlags()             = 0;
    virtual AbsoluteTime                    getTimeStamp()         = 0;
    virtual UInt32                          getValue()             = 0;
    virtual OSData *                        getDataValue()         = 0;
    virtual void                            setValue(UInt32 value)  = 0;
    virtual void                            setDataValue(OSData * value) = 0;
  
    OSMetaClassDeclareReservedUsed(IOHIDElement,  0);
    virtual bool                            conformsTo(UInt32 usagePage, UInt32 usage=0) = 0;
    
    OSMetaClassDeclareReservedUsed(IOHIDElement,  1);
    virtual void                            setCalibration(UInt32 min=0, UInt32 max=0, UInt32 saturationMin=0, UInt32 saturationMax=0, UInt32 deadZoneMin=0, UInt32 deadZoneMax=0, IOFixed granularity=0) = 0;
    
    OSMetaClassDeclareReservedUsed(IOHIDElement,  2);
    virtual UInt32                          getScaledValue(IOHIDValueScaleType type=kIOHIDValueScaleTypePhysical) = 0;
    
    OSMetaClassDeclareReservedUsed(IOHIDElement,  3);
    virtual IOFixed                         getScaledFixedValue(IOHIDValueScaleType type=kIOHIDValueScaleTypePhysical) = 0;

    OSMetaClassDeclareReservedUsed(IOHIDElement,  4);
    virtual UInt32                          getValue(IOOptionBits options) = 0;

    OSMetaClassDeclareReservedUsed(IOHIDElement,  5);
    virtual OSData *                        getDataValue(IOOptionBits options) = 0;
    
    OSMetaClassDeclareReservedUsed(IOHIDElement,  6);
    virtual boolean_t                       isVariableSize()        = 0;

    OSMetaClassDeclareReservedUsed(IOHIDElement,  7);
    virtual IOFixed                         getScaledFixedValue(IOHIDValueScaleType type, IOOptionBits options)  = 0;

    OSMetaClassDeclareReservedUsed(IOHIDElement,  8);
    virtual void                            setValue(UInt32 value, IOOptionBits options) = 0;
    OSMetaClassDeclareReservedUnused(IOHIDElement,  9);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 10);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 11);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 12);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 13);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 14);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 15);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 16);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 17);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 18);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 19);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 20);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 21);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 22);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 23);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 24);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 25);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 26);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 27);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 28);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 29);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 30);
    OSMetaClassDeclareReservedUnused(IOHIDElement, 31);

};

#endif /* !_IOKIT_HID_IOHIDELEMENT_H */
