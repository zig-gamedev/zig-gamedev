/* This is a place for the changes needed to get net_snmp runnning on darwin11 including
   support for the new UNAME_RELEASE version
   Currently this just enatils including darwin10.h
*/

#include "darwin10.h"

#ifndef darwin11
#   define darwin11 darwin
#endif
