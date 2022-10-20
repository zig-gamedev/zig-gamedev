/*
     File:       QD/ATSUnicodeDirectAccess.h
 
     Contains:   Public Interfaces/Types for Low Level ATSUI
 
     Version:    Quickdraw-291~1
 
     Copyright:  � 2002-2008 by Apple Inc. all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSUNICODEDIRECTACCESS__
#define __ATSUNICODEDIRECTACCESS__

#ifndef __ATSUNICODETYPES__
#include <ATSUI/ATSUnicodeTypes.h>
#endif


/* ---------------------------------------------------------------------------- */
/* Constants                                                                    */
/* ---------------------------------------------------------------------------- */

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ATSUDirectDataSelector
 *  
 *  Summary:
 *    These are the data selectors used in the
 *    ATSUDirectGetLayoutDataArrayPtr function to get the needed layout
 *    data array pointer.
 */
typedef UInt32 ATSUDirectDataSelector;
enum {

  /*
   * Returns the parallel advance delta (delta X) array. (Array Type):
   * Fixed (Return Time): Constant, unless creation is necessary, or
   * unless requested by ATSUDirectGetLayoutDataArrayPtrFromTextLayout.
   * (Creation): This array is created only on demand. Thus, if any
   * changes are to be made iCreate should be set to true. If the array
   * had not been previously allocated it will be allocated and
   * zero-filled when iCreate is set to true.
   */
  kATSUDirectDataAdvanceDeltaFixedArray = 0,

  /*
   * Returns the parallel baseline delta (delta Y) array. (Array Type):
   * Fixed (Return Time): Constant, unless creation is necessary, or
   * unless requested by ATSUDirectGetLayoutDataArrayPtrFromTextLayout.
   * (Creation): This array is created only on demand. Thus, if any
   * changes are to be made iCreate should be set to true. If the array
   * had not been previously allocated it will be allocated and
   * zero-filled when iCreate is set to true.
   */
  kATSUDirectDataBaselineDeltaFixedArray = 1,

  /*
   * Returns the parallel device delta array for device- specific
   * tweaking. This is an array of values which are used to adjust
   * truncated fractional values for devices that do not accept
   * fractional positioning. It is also used to provide precise
   * positioning for connected scripts. (Array Type): SInt16 (Return
   * Time): Constant, unless creation is necessary, or unless requested
   * by ATSUDirectGetLayoutDataArrayPtrFromTextLayout. (Creation): This
   * array is created only on demand. Thus, if any changes are to be
   * made iCreate should be set to true. If the array had not been
   * previously allocated it will be allocated and zero-filled when
   * iCreate is set to true.
   */
  kATSUDirectDataDeviceDeltaSInt16Array = 2,

  /*
   * Returns the parallel style index array. The indexes setting in the
   * array are indexes into the the StyleSetting array, which can be
   * obtained using the
   * kATSUDirectDataStyleSettingATSUStyleSettingRefArray below. (Array
   * Type): UInt16 (Return Time): Constant, unless creation is
   * necessary, or unless requested by
   * ATSUDirectGetLayoutDataArrayPtrFromTextLayout. (Creation): This
   * array is created only on demand. Thus, if any changes are to be
   * made iCreate should be set to true. If the array had not been
   * previously allocated it will be allocated and zero-filled when
   * iCreate is set to true.
   */
  kATSUDirectDataStyleIndexUInt16Array = 3,

  /*
   * Returns the style setting ref array. (Array Type):
   * ATSUStyleSettingRef (Return Time): Linear, based on the number of
   * styles applied to the given line. (Creation): This array is always
   * present if the layout has any text assigned to it at all. Setting
   * iCreate has no effect.
   */
  kATSUDirectDataStyleSettingATSUStyleSettingRefArray = 4,

  /*
   * Returns the ATSLayoutRecord, version 1 array. This should not be
   * used directly at all. Rather, use the
   * kATSUDirectDataLayoutRecordATSLayoutRecordCurrent selector below.
   * This will ensure that the code will always be using the most
   * current version of the ATSLayoutRecord, should there ever be a
   * change. ATSUI will only ensure the most efficient processing will
   * occur for the latest version of ATSLayoutRecord. (Array Type):
   * ATSLayoutRecord, version 1 (Return Time): Constant, unless
   * creation is necessary, or unless requested by
   * ATSUDirectGetLayoutDataArrayPtrFromTextLayout. (Creation): This
   * array is always present if the layout has any text assigned to it
   * at all. Setting iCreate has no effect
   */
  kATSUDirectDataLayoutRecordATSLayoutRecordVersion1 = 100,

  /*
   * Returns the ATSLayoutRecord. This will return the most current
   * version of the ATSLayoutRecord, and the one that's defined in this
   * file. Always use kATSUDirectDataLayoutRecordATSLayoutRecordCurrent
   * to get the array of ATSLayoutRecords. (Array Type):
   * ATSLayoutRecord (Return Time): Constant, unless creation is
   * necessary, or unless requested by
   * ATSUDirectGetLayoutDataArrayPtrFromTextLayout. (Creation): This
   * array is always present if the layout has any text assigned to it
   * at all. Setting iCreate has no effect.
   */
  kATSUDirectDataLayoutRecordATSLayoutRecordCurrent = kATSUDirectDataLayoutRecordATSLayoutRecordVersion1
};

/* ---------------------------------------------------------------------------- */
/* Data Types                                                                   */
/* ---------------------------------------------------------------------------- */

/*
 *  ATSUStyleSettingRef
 *  
 *  Summary:
 *    A reference to a style setting object that represents an
 *    ATSUStyle plus any cached/set information about that style.
 */
typedef struct LLCStyleInfo*            ATSUStyleSettingRef;

#ifdef __cplusplus
}
#endif

#endif /* __ATSUNICODEDIRECTACCESS__ */

