//
//  USBDriverKitDebug.h
//  USBDriverKit
//
//  Created by Shawn Lennox on 3/12/19.
//

#ifndef USBDriverKitDebug_h
#define USBDriverKitDebug_h

#include <stdio.h>
#include <os/log.h>



#define USBDKLogDebug(fmt, args...) \
os_log_debug(OS_LOG_DEFAULT, "USBDriverKit:%s" fmt "\n",__func__, ##args)

#define USBDKLogInfo(fmt, args...) \
os_log_info(OS_LOG_DEFAULT, "USBDriverKit:%s" fmt "\n",__func__, ##args)

#define USBDKLog(fmt, args...) \
os_log(OS_LOG_DEFAULT, "USBDriverKit:%s" fmt "\n",__func__, ##args)

#define USBDKLogError(fmt, args...) \
os_log(OS_LOG_DEFAULT, "USBDriverKit:%s" fmt "\n",__func__, ##args)

#endif /* USBDriverKitDebug_h */
