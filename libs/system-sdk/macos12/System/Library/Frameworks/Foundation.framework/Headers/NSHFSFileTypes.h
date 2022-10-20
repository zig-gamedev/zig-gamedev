/*
	NSHFSFileTypes.h
	Copyright (c) 2000-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObjCRuntime.h>
#import <CoreFoundation/CFBase.h>

@class NSString;

// Given an HFS file type code, return a string that represents the file type.  The string will have been autoreleased.  The format of the string is a private implementation detail, but such strings are suitable for inclusion in arrays that also contain file name extension strings.  Several Cocoa API methods take such arrays.
FOUNDATION_EXPORT NSString *NSFileTypeForHFSTypeCode(OSType hfsFileTypeCode);

// Given a string of the sort encoded by NSFileTypeForHFSTypeCode(), return the corresponding HFS file type code.  Return zero otherwise.
FOUNDATION_EXPORT OSType NSHFSTypeCodeFromFileType(NSString *fileTypeString);

// Given the full absolute path of a file, return a string that represents the file's HFS file type as described above, or nil if the operation is not successful.  The string will have been autoreleased.
FOUNDATION_EXPORT NSString *NSHFSTypeOfFile(NSString *fullFilePath);
