/*
     File:       ApplicationServices/ApplicationServices.h
 
     Contains:   Master include for ApplicationServices public framework
 
     Copyright:  (c) 2000-2012 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __APPLICATIONSERVICES__
#define __APPLICATIONSERVICES__

#ifndef __AVAILABILITY__
#include <Availability.h>
#endif

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __ATS__
#include <ATS/ATS.h>
#endif

#ifndef __COLORSYNC__
#include <ColorSync/ColorSync.h>
#endif

#ifndef __COREGRAPHICS__
#include <CoreGraphics/CoreGraphics.h>
#endif

#ifndef __CORETEXT__
#include <CoreText/CoreText.h>
#endif

#ifndef __HISERVICES__
#include <HIServices/HIServices.h>
#endif

#ifndef __IMAGEIO__
#include <ImageIO/ImageIO.h>
#endif

#if __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_12_0 || __LANGANALYSIS_FORCE_INCLUSION_FROM_APPLICATIONSERVICES__
	//	LangAnalysis.framework was fully removed in macOS 12.0, after being deprecated in macOS 10.5
	#ifndef __LANGANALYSIS__
		#if defined(__has_include) && __has_include(<LangAnalysis/LangAnalysis.h>)
			#include <LangAnalysis/LangAnalysis.h>
		#elif __LANGANALYSIS_FORCE_INCLUSION_FROM_APPLICATIONSERVICES__
			#include <LangAnalysis/LangAnalysis.h>
		#endif
	#endif
#endif

#ifndef __PRINTCORE__
#include <PrintCore/PrintCore.h>
#endif

#ifndef __QD__
#include <QD/QD.h>
#endif

#ifndef __SPEECHSYNTHESIS__
#include <SpeechSynthesis/SpeechSynthesis.h>
#endif

#endif /* __APPLICATIONSERVICES__ */
