/* This is a place for the changes needed to get net_snmp runnning on darwin17
   Currently this just entails including darwin16.h
*/

#include "darwin16.h"

#ifndef darwin17
#   define darwin17 darwin
#endif


