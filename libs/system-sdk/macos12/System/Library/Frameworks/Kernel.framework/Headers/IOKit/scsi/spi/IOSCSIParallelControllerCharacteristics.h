/*
 * Copyright (c) 2020 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
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


#ifndef _IO_SCSI_PARALLEL_CONTROLLER_CHARACTERISTICS_H_
#define _IO_SCSI_PARALLEL_CONTROLLER_CHARACTERISTICS_H_


/*! @header IOSCSIParallelControllerCharacteristics
SCSI controller specific typedefs and constants used inside DriverKit environment.
*/

// The Feature Selectors used to identify features of the SCSI Parallel
// Interface.  These are used by the DoesHBASupportSCSIParallelFeature
// to report whether the HBA supports a given SCSI Parallel Interface
// feature and are used for requesting negotiation and reporting negotiation
// results between the controller and the device.

// When the DoesHBASupportSCSIParallelFeature() member routine of the controller
// child class is called, it will return true if the HBA that it controls
// supports the specified SCSIParallelFeature or false if it does not.
typedef enum SCSIParallelFeature
{
    // The selector for support of Wide Data Transfers.  Only Wide16 is supported
    // as Wide32 has been obsoleted by the SPI-3 specification.
    kSCSIParallelFeature_WideDataTransfer                     = 0,
    
    // The selector for support of Synchronous Data Transfers.
    kSCSIParallelFeature_SynchronousDataTransfer             = 1,
    
    // The selector for support of Quick Arbitration and Selection (QAS).
    kSCSIParallelFeature_QuickArbitrationAndSelection         = 2,
    
    // The selector for support of Double Transition (DT) data transfers.
    kSCSIParallelFeature_DoubleTransitionDataTransfers         = 3,
    
    // The selector for SPI Information Unit (IU) transfers.
    kSCSIParallelFeature_InformationUnitTransfers             = 4,
    
    // Since the Feature selectors are zero base, this will always have the
    // correct total.
    kSCSIParallelFeature_TotalFeatureCount
} SCSIParallelFeature;

typedef enum SCSIParallelFeatureRequest
{
    // This selector indicates that current negotiation
    // should be used.
    kSCSIParallelFeature_NoNegotiation             = 0,
    
    // This selector indicates that the controller
    // should attempt negotiation for the feature
    kSCSIParallelFeature_AttemptNegotiation     = 1,
    
    // This selector indicates that the controller
    // should clear any negotiation for the feature
    kSCSIParallelFeature_ClearNegotiation         = 2
} SCSIParallelFeatureRequest;

typedef enum SCSIParallelFeatureResult
{
    kSCSIParallelFeature_NegotitiationUnchanged    = 0,
    kSCSIParallelFeature_NegotitiationCleared    = 1,
    kSCSIParallelFeature_NegotitiationSuccess    = 2
} SCSIParallelFeatureResult;

// Set this key with a value of true in UserReportHBAConstraints() to indicate support
// for full 8-byte LUN addressing
#define kIOHierarchicalLogicalUnitSupportKey        "SCSI Hierarchical Logical Unit Support"

// This is the alignment mask used when allocating per-task HBA data. It allows
// the HBA to declare whether or not it supports 64-bit addressability and what the
// minimum byte alignment is for the data. E.g. By specifying 0x0000FFFFFFFFFFFEULL,
// the controller would be indicating that it supports 48-bits of addressability, but
// at a minimum of being 2-byte aligned.
#define kIOMinimumHBADataAlignmentMaskKey            "HBA Data Alignment"

#endif /* _IO_SCSI_PARALLEL_CONTROLLER_CHARACTERISTICS_H_ */
