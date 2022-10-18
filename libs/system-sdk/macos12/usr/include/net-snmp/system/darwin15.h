/* This is a place for the changes needed to get net_snmp runnning on darwin15
   Currently this just entails including darwin14.h
*/

#include "darwin14.h"

#ifndef darwin15
#   define darwin15 darwin
#endif


