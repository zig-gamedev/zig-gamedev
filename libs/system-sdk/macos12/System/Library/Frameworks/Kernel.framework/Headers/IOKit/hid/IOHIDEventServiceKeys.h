/*
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 2019 Apple Computer, Inc.  All Rights Reserved.
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

#ifndef IOHIDEventServiceKeys_h
#define IOHIDEventServiceKeys_h

/*!
 * @define kIOHIDPointerAccelerationKey
 *
 * @abstract
 * Number property that contains the pointer acceleration value.
 */
#define kIOHIDPointerAccelerationKey "HIDPointerAcceleration"

/*!
 * @define kIOHIDPointerAccelerationTypeKey
 *
 * @abstract
 * String property containing the type of acceleration for pointer.
 * Supported types are:
 *      <code>kIOHIDPointerAccelerationKey</code>
 *      <code>kIOHIDMouseScrollAccelerationKey</code>
 *      <code>kIOHIDTrackpadAccelerationType</code>
 */
#define kIOHIDPointerAccelerationTypeKey "HIDPointerAccelerationType"

/*!
 * @define kIOHIDMouseScrollAccelerationKey
 *
 * @abstract
 * Number property that contains the mouse scroll acceleration value.
 */
#define kIOHIDMouseScrollAccelerationKey "HIDMouseScrollAcceleration"

/*!
 * @define kIOHIDMouseAccelerationTypeKey
 *
 * @abstract
 * Number property that contains the mouse acceleration value.
 */
#define kIOHIDMouseAccelerationTypeKey "HIDMouseAcceleration"

/*!
 * @define kIOHIDScrollAccelerationKey
 *
 * @abstract
 * Number property that contains the scroll acceleration value.
 */
#define kIOHIDScrollAccelerationKey "HIDScrollAcceleration"

/*!
 * @define kIOHIDScrollAccelerationTypeKey
 *
 * @abstract
 * Number property containing the type of acceleration for scroll.
 * Supported types are:
 *      <code>kIOHIDMouseScrollAccelerationKey</code>
 *      <code>kIOHIDTrackpadScrollAccelerationKey</code>
 */
#define kIOHIDScrollAccelerationTypeKey "HIDScrollAccelerationType"

/*!
 * @define kIOHIDDigitizerTipThresholdKey
 *
 * @abstract
 * Number property that describes the threshold percentage for when the tip
 * pressure of a digitizer stylus should change from hovering to dragging.
 *
 * @discussion
 * If a digitizer stylus supports the kHIDUsage_Dig_TipPressure (0x30) usage,
 * the service may optionally publish this key to describe the value at which
 * the pressure should change the pointer behavior from hovering to dragging.
 * The value is a percentage from 0 to 100, where 100 percent is equal to the
 * logical max that the stylus dispatches. If no value is provided, the default
 * value of 75 will be used.
 */
#define kIOHIDDigitizerTipThresholdKey "DigitizerTipThreshold"

/*!
 * @define kIOHIDSurfaceDimensionsKey
 *
 * @abstract
 * Dictionary property published on a service that describes the surface
 * dimensions for services that publish absolute X/Y values, such as digitizer
 * and pointer devices. The dictionary will contain the kIOHIDWidthKey and
 * kIOHIDHeightKey keys described below. Value is in millimeter represented
 * as IOFixed.
 */
#define kIOHIDSurfaceDimensionsKey "SurfaceDimensions"

/*!
 * @define kIOHIDWidthKey
 *
 * @abstract
 * Number property used in the surface dimensions dictionary described above.
 * Default value represents the physical max - physical min of the absolute
 * X value.
 */
#define kIOHIDWidthKey "Width"

/*!
 * @define kIOHIDHeightKey
 *
 * @abstract
 * Number property used in the surface dimensions dictionary described above.
 * Default value represents the physical max - physical min of the absolute
 * Y value.
 */
#define kIOHIDHeightKey "Height"

/*!
 * @define kIOHIDEventDriverHandlesReport
 *
 * @abstract
 * Boolean property used to let handleReport in an IOUserHIDEventDriver get the
 * report before any other processing is done in IOUserHIDEventService.
 * If this property is enabled the IOUserHIDEventService subclass should update
 * the elements with IOHIDInterface::processReport to update the IOHIDElements as
 * IOUserHIDEventService will not do this like when this property is not set.
 */
#define kIOHIDEventDriverHandlesReport "IOHIDEventDriverHandlesReport"

#endif /* IOHIDDeviceTypes_h */
