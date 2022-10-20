/*
     File:       ATS/SFNTTypes.h
 
     Contains:   Font file structures.
 
     Version:    ATS
 
     Copyright:  © 1994-2012 by Apple Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __ATS_SFNTTYPES__
#define __ATS_SFNTTYPES__

#include <CoreText/SFNTTypes.h>

/*   Deprecated "don't care" values - use kFontNoPlatformCode, kFontNoScriptCode, 
     kFontNoLanguageCode, kFontNoName instead
*/
enum {
  kFontNoPlatform                 = (unsigned int)(~0),
  kFontNoScript                   = (unsigned int)(~0),
  kFontNoLanguage                 = (unsigned int)(~0),
  kFontNoName                     = (unsigned int)(~0)
};

#endif /* __ATS_SFNTTYPES__ */
