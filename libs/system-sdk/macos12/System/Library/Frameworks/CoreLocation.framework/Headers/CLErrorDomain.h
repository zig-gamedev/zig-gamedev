/*
 *  CLErrorDomain.h
 *  CoreLocation
 *
 *  Copyright 2008 Apple, Inc. All rights reserved.
 *
 */

#import <CoreLocation/CLAvailability.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  kCLErrorDomain
 *  
 *  Discussion:
 *    Error returned as the domain to NSError from CoreLocation.
 *
 *  The file CLError.h defines constants for the errors in kCLErrorDomain.
 */
CL_EXTERN NSString *const kCLErrorDomain;

NS_ASSUME_NONNULL_END
