/*
 *  AXValue.h
 *  Accessibility
 *
 *  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 */

#ifndef __AXVALUE__
#define __AXVALUE__

#include <AvailabilityMacros.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
    
CF_ASSUME_NONNULL_BEGIN
CF_IMPLICIT_BRIDGING_ENABLED
    
/* Need discussion for following */

/*! @header AXValue.h
	@discussion
		This header contains functions and data types for
		working with AXValueType wrappers.
*/

/*!
  @enum AXValueType Wrappers
  
  @discussion
  These are AXValueType wrappers for other structures. You must use the AXValueCreate
  and AXValueGetValue functions to convert between the wrapped structure and the native structure.
  
  @constant kAXValueTypeCGPoint a wrapper for CGPoint; see CoreGraphics.h
  @constant kAXValueTypeCGSize a wrapper for CGSize; see CoreGraphics.h
  @constant kAXValueTypeCGRect a wrapper for CGRect; see CoreGraphics.h
  @constant kAXValueTypeCFRange a wrapper for CFRange; see CFBase.h
  @constant kAXValueTypeAXError See AXError.h
  @constant kAXValueTypeIllegal a wrapper for unsupported structures
*/
typedef CF_ENUM(UInt32, AXValueType) {
    kAXValueTypeCGPoint      CF_ENUM_AVAILABLE_MAC(10_11) = 1,
    kAXValueTypeCGSize       CF_ENUM_AVAILABLE_MAC(10_11) = 2,
    kAXValueTypeCGRect       CF_ENUM_AVAILABLE_MAC(10_11) = 3,
    kAXValueTypeCFRange      CF_ENUM_AVAILABLE_MAC(10_11) = 4,
    kAXValueTypeAXError      CF_ENUM_AVAILABLE_MAC(10_11) = 5,
    kAXValueTypeIllegal      CF_ENUM_AVAILABLE_MAC(10_11) = 0,
};

// Legacy constants will be deprecated shortly. Please switch to using AXValueType
static const UInt32 kAXValueCGPointType = kAXValueTypeCGPoint;
static const UInt32 kAXValueCGSizeType = kAXValueTypeCGSize;
static const UInt32 kAXValueCGRectType = kAXValueTypeCGRect;
static const UInt32 kAXValueCFRangeType = kAXValueTypeCFRange;
static const UInt32 kAXValueAXErrorType = kAXValueTypeAXError;
static const UInt32 kAXValueIllegalType = kAXValueTypeIllegal;

    
/* Need discussion for following */

/*!
    @typedef AXValueRef
    
*/
typedef const struct CF_BRIDGED_TYPE(id) __AXValue *AXValueRef;

/* Need discussion for following */

/*!
	@function AXValueGetTypeID
		
	@result
	
	@availability Mac OS X version 10.3 or later
*/
extern CFTypeID		AXValueGetTypeID() AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*!
	@function AXValueCreate
	
	@discussion
	Encodes a structure pointed to by valuePtr into a CFTypeRef.
	
	@param theType
	@param valuePtr
	
	@result
*/
extern AXValueRef 	__nullable AXValueCreate (AXValueType theType, const void *valuePtr);

/*!
	@function AXValueGetType
	
	@discussion
	Returns the structure type encoded in value. If the type is not recognized, it returns kAXValueIllegalType.
	
	@param value
		
	@result
*/
extern AXValueType 	AXValueGetType(AXValueRef value);

/*!
	@function AXValueGetValue
	
	@discussion
	Decodes the structure stored in value and copies it into valuePtr. If the structure stored in value is not
	the same as requested by theType, the function returns false.
	
	@param value
		
	@result
*/
extern Boolean 		AXValueGetValue(AXValueRef value, AXValueType theType, void *valuePtr);

CF_IMPLICIT_BRIDGING_DISABLED
CF_ASSUME_NONNULL_END
    
#ifdef __cplusplus
}
#endif

#endif // __AXVALUE__
