/* This is a place for the changes needed to get net_snmp runnning on darwin14
   Currently this just entails including darwin13.h
*/

#include "darwin13.h"

#ifndef darwin14
#   define darwin14 darwin
#endif


