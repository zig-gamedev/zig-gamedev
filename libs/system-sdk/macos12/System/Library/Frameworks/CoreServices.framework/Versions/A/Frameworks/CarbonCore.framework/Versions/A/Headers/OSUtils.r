/*
     File:       OSUtils.r
 
     Contains:   OS Utilities Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/

#ifndef __OSUTILS_R__
#define __OSUTILS_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif


/* The following are from the SysTypes.r file and don't have a good home yet */


/*-----------------------accl ¥ Mouse Acceleration Tables ------------------------------*/
type 'accl' {
    literal longint                     /* Device identifier or device class */
        classAbsolute,                  /* A flat-response device */
        classMouse,                     /* Mechanical or optical mouse */
        classTrackball,                 /* Trackball */
        classScratchPad,                /* e. g. Midas */
        classJoystick,                  /* e. g. HomeRow */
        classRelTablet;                 /* Relative tablet */

    integer = $$CountOf(AcclTable);     /* Number of tables for this device */
    array AcclTable {                   /* Entries sorted by first value; must have at least 0.0 and 1.0 tables */
        unsigned hex longint;           /* Acceleration provided by this table (Fixed) */

        integer = $$CountOf(AcclPoint); /* Number of control points for this device */
        wide array AcclPoint {          /* Entries sorted by first value; implicit first entry (0.0, 0.0); at least one more entry required */
            unsigned hex longint;       /* Device speed (inches per second) (Fixed) */
            unsigned hex longint;       /* Cursor speed (inches per second) (Fixed) */
        };
    };
};

/*----------------------------mcky ¥ Mouse Tracking-------------------------------------*/
type 'mcky' {
        array [8] {
            unsigned byte;
        };
};
/*----------------------------KCAP ¥ Physical Layout of Keyboard------------------------*/
type 'KCAP' {
        rect;                                                   /* boundsRect           */
        rect;                                                   /* textRect             */
        integer = $$CountOf(MainArray);
        array MainArray {
            integer = $$CountOf(ShapeArray) - 1;
            wide array ShapeArray {
                point;                                          /* shapePoint           */
            };
            integer = $$CountOf(KeyArray) - 1;
            wide array KeyArray {
                byte;                                           /* mask                 */
                boolean             or, and;
                bitstring[7];                                   /* keyCode              */
                integer;                                        /* dv                   */
                integer;                                        /* dh                   */
            };
        };
};
/*----------------------------KCHR ¥ ASCII Mapping (software)---------------------------*/
type 'KCHR' {
        integer;                                                /* Version              */
        wide array [$100] {                                     /* Indexes              */
            byte;
        };
        integer = $$CountOf(TableArray);
        array TableArray {
            wide array [$80] {                                  /* ASCII characters     */
                char;
            };
        };
        integer = $$CountOf(DeadArray);
        array DeadArray {
            byte;                                               /* Table number         */
            byte;                                               /* Virtual keycode      */
            integer = $$CountOf(CompletorArray);
            wide array CompletorArray {
                char;                                           /* Completor char       */
                char;                                           /* Substituting char    */
            };
            char;                                               /* No match char        */
            char;                                               /* 16-bits for the times
                                                                   when 8 isn't enough  */
        };
};
/*----------------------------KMAP ¥ Keyboard Mapping (hardware)------------------------*/
type 'KMAP' {
        integer;                                                /* ID                   */
        integer;                                                /* Version              */
        wide array [$80] {                                      /* Raw to virtual       */
            byte;                                               /*  keycode map         */
        };
        integer = $$CountOf(ExceptionArray);
        wide array ExceptionArray {
            byte;                                               /* Raw keycode          */
            boolean         noXor, Xor;
            fill bit[3];
            bitstring[4];                                       /* ADB op               */
            pstring;
        };
};

#endif /* __OSUTILS_R__ */

