/*
     File:       CarbonCore/DriverSynchronization.h
 
     Contains:   Driver Synchronization Interfaces.
                 The contents of this header file are deprecated.
                 Use OSAtomic API instead.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/
#ifndef __DRIVERSYNCHRONIZATION__
#define __DRIVERSYNCHRONIZATION__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  CompareAndSwap()
 *  
 *  Summary:
 *    Compare and swap operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The CompareAndSwap function compares the value at the specified
 *    address with oldVal. The value of newValue is written to the
 *    address only if oldValue and the value at the address are equal.
 *    CompareAndSwap returns true if newValue is written to the
 *    address; otherwise, it returns false.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    oldValue:
 *      The value to compare at address.
 *    
 *    newValue:
 *      The value to write to address if oldValue compares true.
 *    
 *    address:
 *      The 4-byte aligned address of the data to update atomically.
 *  
 *  Result:
 *    true if newValue was written to the address.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern Boolean 
CompareAndSwap(
  UInt32    oldValue,
  UInt32    newValue,
  UInt32 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  TestAndClear()
 *  
 *  Summary:
 *    Bit test and clear operation, performed atomically with respect
 *    to all devices that participate in the coherency architecture of
 *    the platform.
 *  
 *  Discussion:
 *    The TestAndClear function clears a single bit in a byte at a
 *    specified address. It returns false if the bit was already clear,
 *    true otherwise.
 *     ------------------------------------------------------------
 *     THIS ROUTINE WAS DOCUMENTED AS RETURNING TRUE IF THE BIT WAS
 *    ALREADY CLEAR AND FALSE OTHERWISE, and on MAC OS 9.x and earlier
 *    it did have this behavior, but on Mac OS X 10.0 and later it has
 *    always returned the state of the bit before the operation ( false
 *    if the bit was clear; true if it was set ).  We have decided that
 *    changing the documentation ( leaving the implementation as is )
 *    is less risky than changing the implementation to match the
 *    documented behavior.
 *     ------------------------------------------------------------
 *     This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    bit:
 *      The bit number in the range 0 through 7.
 *    
 *    address:
 *      The address of the byte to update atomically.
 *  
 *  Result:
 *    true if the bit was already clear, false otherwise.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern Boolean 
TestAndClear(
  UInt32   bit,
  UInt8 *  address)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  TestAndSet()
 *  
 *  Summary:
 *    Bit test and set operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Discussion:
 *    The TestAndSet function sets a single bit in a byte at a
 *    specified address. It returns true if the bit was already set,
 *    false otherwise.
 *  
 *  Parameters:
 *    
 *    bit:
 *      The bit number in the range 0 through 7.
 *    
 *    address:
 *      The address of the byte to update atomically.
 *  
 *  Result:
 *    true if the bit was already set, false otherwise.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern Boolean 
TestAndSet(
  UInt32   bit,
  UInt8 *  address)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  IncrementAtomic8()
 *  
 *  Summary:
 *    8-bit increment operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The IncrementAtomic8 function increments the value at the
 *    specified address by one and returns the original value. 
 *     This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    address:
 *      The address of the value to update atomically.
 *  
 *  Result:
 *    The value before the increment.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt8 
IncrementAtomic8(SInt8 * address)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DecrementAtomic8()
 *  
 *  Summary:
 *    8-bit decrement operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The DecrementAtomic8 function decrements the value at the
 *    specified address by one and returns the original value.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    address:
 *      The address of the value to update atomically.
 *  
 *  Result:
 *    The value before the decrement.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt8 
DecrementAtomic8(SInt8 * address)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AddAtomic8()
 *  
 *  Summary:
 *    8-bit add operation, performed atomically with respect to all
 *    devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The AddAtomic8 function adds the specified amount to the value at
 *    the specified address and returns the original value.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    amount:
 *      The amount to add.
 *    
 *    address:
 *      The address of the value to update atomically.
 *  
 *  Result:
 *    The value before the addition
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt8 
AddAtomic8(
  SInt32   amount,
  SInt8 *  address)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitAndAtomic8()
 *  
 *  Summary:
 *    8-bit logical and operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The BitAndAtomic8 function logically ands the bits of the
 *    specified mask into the value at the specified address and
 *    returns the original value.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically and with the value.
 *    
 *    address:
 *      The address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt8 
BitAndAtomic8(
  UInt32   mask,
  UInt8 *  address)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitOrAtomic8()
 *  
 *  Summary:
 *    8-bit logical or operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Discussion:
 *    The BitOrAtomic8 function logically ors the bits of the specified
 *    mask into the value at the specified address and returns the
 *    original value.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically or with the value.
 *    
 *    address:
 *      The address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt8 
BitOrAtomic8(
  UInt32   mask,
  UInt8 *  address)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitXorAtomic8()
 *  
 *  Summary:
 *    8-bit logical xor operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Discussion:
 *    The BitXorAtomic8 function logically xors the bits of the
 *    specified mask into the value at the specified address and
 *    returns the original value.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically or with the value.
 *    
 *    address:
 *      The address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt8 
BitXorAtomic8(
  UInt32   mask,
  UInt8 *  address)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  IncrementAtomic16()
 *  
 *  Summary:
 *    16-bit increment operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The IncrementAtomic16 function increments the value at the
 *    specified address by one and returns the original value. 
 *     This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    address:
 *      The 2-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the increment.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt16 
IncrementAtomic16(SInt16 * address)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DecrementAtomic16()
 *  
 *  Summary:
 *    16-bit decrement operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The DecrementAtomic16 function decrements the value at the
 *    specified address by one and returns the original value. 
 *     This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    address:
 *      The 2-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the decrement.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt16 
DecrementAtomic16(SInt16 * address)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AddAtomic16()
 *  
 *  Summary:
 *    16-bit add operation, performed atomically with respect to all
 *    devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The AddAtomic16 function adds the specified amount to the value
 *    at the specified address and returns the original value.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    amount:
 *      The amount to add.
 *    
 *    address:
 *      The 2-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the addition
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt16 
AddAtomic16(
  SInt32    amount,
  SInt16 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitAndAtomic16()
 *  
 *  Summary:
 *    16-bit logical and operation, performed atomically with respect
 *    to all devices that participate in the coherency architecture of
 *    the platform.
 *  
 *  Discussion:
 *    The BitAndAtomic16 function logically ands the bits of the
 *    specified mask into the value at the specified address and
 *    returns the original value.
 *    This function guarantees atomicity only with main system memory.
 *    It is specifically unsuitable for use on noncacheable memory such
 *    as that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically and with the value.
 *    
 *    address:
 *      The 2-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt16 
BitAndAtomic16(
  UInt32    mask,
  UInt16 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitOrAtomic16()
 *  
 *  Summary:
 *    16-bit logical or operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The BitOrAtomic16 function logically ors the bits of the
 *    specified mask into the value at the specified address and
 *    returns the original value. This function guarantees atomicity
 *    only with main system memory. It is specifically unsuitable for
 *    use on noncacheable memory such as that in devices; this function
 *    cannot guarantee atomicity, for example, on memory mapped from a
 *    PCI device.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically or with the value.
 *    
 *    address:
 *      The 2-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt16 
BitOrAtomic16(
  UInt32    mask,
  UInt16 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitXorAtomic16()
 *  
 *  Summary:
 *    16-bit logical xor operation, performed atomically with respect
 *    to all devices that participate in the coherency architecture of
 *    the platform.
 *  
 *  Discussion:
 *    The BitXorAtomic16 function logically xors the bits of the
 *    specified mask into the value at the specified address and
 *    returns the original value. This function guarantees atomicity
 *    only with main system memory. It is specifically unsuitable for
 *    use on noncacheable memory such as that in devices; this function
 *    cannot guarantee atomicity, for example, on memory mapped from a
 *    PCI device.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically or with the value.
 *    
 *    address:
 *      The 2-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt16 
BitXorAtomic16(
  UInt32    mask,
  UInt16 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  IncrementAtomic()
 *  
 *  Summary:
 *    32-bit increment operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The IncrementAtomic function increments the value at the
 *    specified address by one and returns the original value. This
 *    function guarantees atomicity only with main system memory. It is
 *    specifically unsuitable for use on noncacheable memory such as
 *    that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    address:
 *      The 4-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the increment.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt32 
IncrementAtomic(SInt32 * address)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DecrementAtomic()
 *  
 *  Summary:
 *    32-bit decrement operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The DecrementAtomic function decrements the value at the
 *    specified address by one and returns the original value. This
 *    function guarantees atomicity only with main system memory. It is
 *    specifically unsuitable for use on noncacheable memory such as
 *    that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    address:
 *      The 4-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the decrement.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt32 
DecrementAtomic(SInt32 * address)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AddAtomic()
 *  
 *  Summary:
 *    32-bit add operation, performed atomically with respect to all
 *    devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The AddAtomic function adds the specified amount to the value at
 *    the specified address and returns the original value. This
 *    function guarantees atomicity only with main system memory. It is
 *    specifically unsuitable for use on noncacheable memory such as
 *    that in devices; this function cannot guarantee atomicity, for
 *    example, on memory mapped from a PCI device.
 *  
 *  Parameters:
 *    
 *    amount:
 *      The amount to add.
 *    
 *    address:
 *      The 4-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the addition
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern SInt32 
AddAtomic(
  SInt32    amount,
  SInt32 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitAndAtomic()
 *  
 *  Summary:
 *    32-bit logical and operation, performed atomically with respect
 *    to all devices that participate in the coherency architecture of
 *    the platform.
 *  
 *  Discussion:
 *    The BitAndAtomic function logically ands the bits of the
 *    specified mask into the value at the specified address and
 *    returns the original value. This function guarantees atomicity
 *    only with main system memory. It is specifically unsuitable for
 *    use on noncacheable memory such as that in devices; this function
 *    cannot guarantee atomicity, for example, on memory mapped from a
 *    PCI device.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically and with the value.
 *    
 *    address:
 *      The 4-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt32 
BitAndAtomic(
  UInt32    mask,
  UInt32 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitOrAtomic()
 *  
 *  Summary:
 *    32-bit logical or operation, performed atomically with respect to
 *    all devices that participate in the coherency architecture of the
 *    platform.
 *  
 *  Discussion:
 *    The BitOrAtomic function logically ors the bits of the specified
 *    mask into the value at the specified address and returns the
 *    original value. This function guarantees atomicity only with main
 *    system memory. It is specifically unsuitable for use on
 *    noncacheable memory such as that in devices; this function cannot
 *    guarantee atomicity, for example, on memory mapped from a PCI
 *    device.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically or with the value.
 *    
 *    address:
 *      The 4-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt32 
BitOrAtomic(
  UInt32    mask,
  UInt32 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  BitXorAtomic()
 *  
 *  Summary:
 *    32-bit logical xor operation, performed atomically with respect
 *    to all devices that participate in the coherency architecture of
 *    the platform. This function guarantees atomicity only with main
 *    system memory. It is specifically unsuitable for use on
 *    noncacheable memory such as that in devices; this function cannot
 *    guarantee atomicity, for example, on memory mapped from a PCI
 *    device.
 *  
 *  Discussion:
 *    The BitXorAtomic function logically xors the bits of the
 *    specified mask into the value at the specified address and
 *    returns the original value.
 *  
 *  Parameters:
 *    
 *    mask:
 *      The mask to logically or with the value.
 *    
 *    address:
 *      The 4-byte aligned address of the value to update atomically.
 *  
 *  Result:
 *    The value before the bitwise operation.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.5 and later
 */
extern UInt32 
BitXorAtomic(
  UInt32    mask,
  UInt32 *  address)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



#ifdef __cplusplus
}
#endif

#endif /* __DRIVERSYNCHRONIZATION__ */

