/* This is a place for the changes needed to get net_snmp runnning on darwin13 
   Currently this just entails including darwin12.h
*/

/* for now copy the constant into our sources, see <rdar://problem/12801140>  */
#ifndef PR_SLOWHZ
#define PR_SLOWHZ       2               /* 2 slow timeouts per second */
#endif

#include "darwin12.h"

#ifndef darwin13
#   define darwin13 darwin
#endif

