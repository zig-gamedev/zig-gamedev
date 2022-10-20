//
//  SharedFileList.h
//  SharedFileList
//
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//



#ifndef __SHAREDFILELIST__
#define __SHAREDFILELIST__
#include <TargetConditionals.h>
#if TARGET_OS_OSX
#include <SharedFileList/LSSharedFileList.h>
#endif // TARGET_OS_OSX
#endif // __SHAREDFILELIST__
