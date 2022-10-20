/*
     File:       MacTypes.r
 
     Contains:   Basic Macintosh data types.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/

#ifndef __MACTYPES_R__
#define __MACTYPES_R__

#ifndef __SCRIPT_R__
#include <CarbonCore/Script.r> /* to get Region codes for 'vers' resource */
#endif
#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define normal 							0
#define bold 							1
#define italic 							2
#define underline 						4
#define outline 						8
#define shadow 							0x10
#define condense 						0x20
#define extend 							0x40

															/*  Version Release Stage Codes  */
#define developStage 					0x20
#define alphaStage 						0x40
#define betaStage 						0x60
#define finalStage 						0x80

/*----------------------------STR  ¥ Pascal-Style String--------------------------------*/
type 'STR ' {
        pstring;                                                /* String               */
};
/*----------------------------STR# ¥ Pascal-Style String List---------------------------*/
type 'STR#' {
        integer = $$Countof(StringArray);
        array StringArray {
                pstring;                                        /* String               */
        };
};

/*----------------------------RECT ¥ single rectangle-----------------------------------*/
type 'RECT' { rect; };

/*----------------------------vers ¥ Version--------------------------------------------*/
type 'vers' {
        hex byte;                                               /* Major revision in BCD*/
        hex byte;                                               /* Minor vevision in BCD*/
        hex byte    development = 0x20,                         /* Release stage        */
                    alpha = 0x40,
                    beta = 0x60,
                    final = 0x80, /* or */ release = 0x80;
        hex byte;                                               /* Non-final release #  */
        integer;                                                /* Region code          */
        pstring;                                                /* Short version number */
        pstring;                                                /* Long version number  */
};

/*----------------------------utxt ¥ Unicode text (analogous to 'TEXT')-----------------*/
type 'utxt' {
        wide array UnicodeText {
            unsigned hex integer;
        };
};
/*----------------------------utx# ¥ Unicode string list (analogous to 'STR#')----------*/
type 'utx#' {
        unsigned integer = $$Countof(StringArray);
        array StringArray {
            unsigned integer = $$Countof(UnicodeText);
            wide array UnicodeText {
                unsigned hex integer;
            };
        };
};


#endif /* __MACTYPES_R__ */

