/* This is a place for the changes needed to get net_snmp runnning on darwin12 
   Currently this just entails including darwin11.h
*/

#include "darwin11.h"

#ifndef darwin12
#   define darwin12 darwin
#endif
