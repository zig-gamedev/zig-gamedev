/* This is a place for the changes needed to get net_snmp runnning on darwin21
   Currently this just entails including darwin20.h, Yes we are still kicking this can down the road.
*/

#include "darwin20.h"

#ifndef darwin21
#   define darwin21 darwin
#endif


