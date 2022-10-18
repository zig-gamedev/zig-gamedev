/* This is a place for the changes needed to get net_snmp runnning on darwin16
   Currently this just entails including darwin15.h
*/

#include "darwin15.h"

#ifndef darwin16
#   define darwin16 darwin
#endif


