//
//  ATSAvailability.h
//  ATSUI
//
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#ifndef __ATS_AVAILIBILITY__
#define __ATS_AVAILIBILITY__

// The "_R" variation of the macros is for "retroactive"
// Functions declared with it were not deprecated until Mojave, though they should have been by Tiger

#undef ATS_AVAILABLE_BUT_DEPRECATED

#if ATS_BUILDING_ATS

#define ATS_AVAILABLE_BUT_DEPRECATED(m0,m1)
#define ATS_AVAILABLE_BUT_DEPRECATED_R(m0,m1)

#else

#define ATS_AVAILABLE_BUT_DEPRECATED(m0,m1) API_DEPRECATED("No longer supported", macos(m0,m1))
#define ATS_AVAILABLE_BUT_DEPRECATED_R(m0,m1) API_DEPRECATED("No longer supported", macos(m0,m1))

#endif


#endif /* __ATS_AVAILIBILITY__ */
