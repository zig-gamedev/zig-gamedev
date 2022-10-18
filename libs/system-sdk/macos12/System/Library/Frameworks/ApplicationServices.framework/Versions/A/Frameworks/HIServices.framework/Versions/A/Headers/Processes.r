/*
     File:       Processes.r
 
     Contains:   Process Manager Interfaces.
 
     Copyright:  © 1989-2012 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __PROCESSES_R__
#define __PROCESSES_R__

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif


/*----------------------------SIZE ¥ MultiFinder Size Information-----------------------*/
#define ignoreAppDiedEvents         ignoreChildDiedEvents
#define acceptAppDiedEvents         acceptChildDiedEvents
#define needsActivateOnFGSwitch     notMultiFinderAware         /* for compatibility    */
#define doesActivateOnFGSwitch      multiFinderAware            /* for compatibility    */
#define dontSaveScreen              reserved                    /* for compatibility    */
#define saveScreen                  true                        /* for compatibility    */
#define enableOptionSwitch          reserved                    /* for compatibility    */
#define disableOptionSwitch         true                        /* for compatibility    */

#undef reserved
type 'SIZE' {
        boolean                 reserved;
        boolean                 ignoreSuspendResumeEvents,      /* suspend-resume       */
                                acceptSuspendResumeEvents;
        boolean                 reserved;
        boolean                 cannotBackground,
                                canBackground;                  /* Can properly use back-
                                                                   ground null events   */
        boolean                 needsActivateOnFGSwitch,        /* activate/deactivate  */
                                doesActivateOnFGSwitch;         /* on resume/suspend    */
        boolean                 backgroundAndForeground,        /* Application does not */
                                onlyBackground;                 /* have a user interface*/
        boolean                 dontGetFrontClicks,             /* Get mouse down/up    */
                                getFrontClicks;                 /* when suspended       */
        boolean                 ignoreAppDiedEvents,            /* Apps use this.       */
                                acceptAppDiedEvents;            /* Debuggers use this.  */
        boolean                 not32BitCompatible,             /* Works with 24bit addr*/
                                is32BitCompatible;              /* Works with 24 or 32  */
                                                                /* bit addresses        */
/* next four bits are new for system 7.0 */
        boolean                 notHighLevelEventAware,
                                isHighLevelEventAware;          /* does Post/AcceptHighLevelEvent */
        boolean                 onlyLocalHLEvents,              /* paranoid users' flag */
                                localAndRemoteHLEvents;
        boolean                 notStationeryAware,             /* checks stationery bit */
                                isStationeryAware;              /* when opening documents */
        boolean                 dontUseTextEditServices,        /* can use text services */
                                useTextEditServices;            /* through TextEdit.     */

        boolean                 notDisplayManagerAware,         /* All windows redrawn when */
                                                                /* monitor(s) change.       */
                                isDisplayManagerAware;          /* App will register a     */
                                                                /* notification proc with DM*/
                                                                /* to handle monitor changes*/

        boolean                 reserved;                       /* These 2 bits are reserved.*/
        boolean                 reserved;                       

        /* Memory sizes are in bytes */
        unsigned longint;                                       /* preferred mem size   */
        unsigned longint;                                       /* minimum mem size     */

        // If we ever define one of the two reserved bits above, the "reserved"
        // enumeration wouldn't appear on the newly defined bit. By defining "reserved"
        // below, old resource SIZE declarations will still compile.
#define reserved        false
};

#endif /* __PROCESSES_R__ */

