/*
     File:       SearchKit/SearchKit.h
 
     Contains:   SearchKit Interfaces.
 
     Version:    SearchKit-417.1
 
     Copyright:  2003-2019 by Apple, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
/*
 *  This framework provides required capabilities to index, search, and analyze the
 *  text of documents. A document is simply an item from which terms can be extracted. 
 *  (i.e. a file on a computer, a record in a database, a sentence, ...)
 */
#ifndef __SEARCHKIT__
#define __SEARCHKIT__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif


#ifndef __SKDOCUMENT__
#include <SearchKit/SKDocument.h>
#endif

#ifndef __SKANALYSIS__
#include <SearchKit/SKAnalysis.h>
#endif

#ifndef __SKINDEX__
#include <SearchKit/SKIndex.h>
#endif

#ifndef __SKSEARCH__
#include <SearchKit/SKSearch.h>
#endif

#ifndef __SKSUMMARY__
#include <SearchKit/SKSummary.h>
#endif



#endif /* __SEARCHKIT__ */

