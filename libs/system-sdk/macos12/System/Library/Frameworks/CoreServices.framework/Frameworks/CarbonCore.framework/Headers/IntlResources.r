/*
     File:       IntlResources.r
 
     Contains:   International Resource definitions.
 
     Copyright:  © 1983-2011 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __INTLRESOURCES_R__
#define __INTLRESOURCES_R__

#ifndef __SCRIPT_R__
#include <CarbonCore/Script.r>
#endif
#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

/*
    Note:   Region, Language, and Script are no longer used.
            Instead, Script.r #defines each value for those types.
            Existing resouce source files should Rez just fine, but when
            DeRezed the numeric values will be used instead of the symbolic name.
*/
/*-------------------------------------Equates------------------------------------------*/
/*  The following are used to set characters used in INTL resources.
*/
#define periodSymbol        "."
#define commaSymbol         ","
#define semicolonSymbol     ";"
#define dollarsignSymbol    "$"
#define slashSymbol         "/"
#define colonSymbol         ":"


/*----------------------------INTL (0) ¥ International Formatting Information-----------*/
type 'INTL' (0) {
        char        period = periodSymbol;                      /* decimal pt sep       */
        char        comma = commaSymbol;                        /* thousands sep        */
        char        semicolon = semicolonSymbol;                /* list sep             */
        char        dollarsign = dollarsignSymbol;              /* currSym1             */
        char;                                                   /* currSym2             */
        char;                                                   /* currSym3             */

        /* currFmt */
        boolean     noLeadZero, leadingZero;                    /* leading unit zero    */
        boolean     noTrailingZero, trailingZero;               /* trailing dec zero    */
        boolean     paren, minusSign;                           /* negative rep         */
        boolean     trails, leads;                              /* curr sym position    */
        fill bit[4];                                            /* not used             */
        byte        monDayYear, dayMonYear, yearMonDay,         /* dateOrder            */
                    monYearDay, dayYearMon, yearDayMon;

        /* shrtDateFmt */
        boolean     noCentury, century;                         /* century incl         */
        boolean     noMonthLeadZero, monthLeadZero;             /* mon lead zero        */
        boolean     noDayLeadZero, dayLeadZero;                 /* day lead zero        */
        fill bit[5];                                            /* filler               */
        char        slash = slashSymbol;                        /* date sep             */
        byte        twentyFourHour, twelveHourZeroCycle,        /* timeCycle            */
                    twelveHour = 255;

        /* timeFmt */
        boolean     noHoursLeadZero, hoursLeadZero;             /* hours lead zero      */
        boolean     noMinutesLeadZero, minutesLeadZero;         /* min lead zero        */
        boolean     noSecondsLeadZero, secondsLeadZero;         /* sec lead zero        */
        fill bit[5];                                            /* filler               */
        string[4];                                              /* mornStr              */
        string[4];                                              /* eveStr               */
        char;                                                   /* timeSep              */
        char;                                                   /* time1Stuff           */
        char;                                                   /* time2Stuff           */
        char;                                                   /* time3Stuff           */
        char;                                                   /* time4Stuff           */
        char;                                                   /* time5Stuff           */
        char;                                                   /* time6Stuff           */
        char;                                                   /* time7Stuff           */
        char;                                                   /* time8Stuff           */
        byte        standard, metric = 255;                     /* metricSys            */
        byte;                                                   /* INTL0 region         */
        byte;                                                   /* version              */
};
/*----------------------------itl0 ¥ International Formatting Information---------------*/
type 'itl0' as 'INTL' (0);
/*----------------------------INTL (1) ¥ International Date/Time Information------------*/
type 'INTL' (1) {
        /* Day names */
        array [7] {
            pstring[15];                                        /* Sunday, Monday...    */
        };

        /* Month names */
        array [12] {
            pstring[15];                                        /* January, February... */
        };

        byte        dayName, none=255;                          /* suppressDay          */
        byte        dayMonYear, monDayYear = 255;               /* longDate format      */
        byte        noDayLeadZero, dayLeadZero = 255;           /* dayLeading0          */
        byte;                                                   /* abbrLen              */
        string[4];                                              /* st0                  */
        string[4];                                              /* st1                  */
        string[4];                                              /* st2                  */
        string[4];                                              /* st3                  */
        string[4];                                              /* st4                  */
        byte;                                                   /* INTL1 region         */
        byte;                                                   /* version              */
        hex string  DefaultReturn = "\$4E\$75";                 /* local routine - If
                                                                   you have one, use
                                                                   $$Resource to insert
                                                                   code here.  Otherwise,
                                                                   you must have a 68000
                                                                   return instruction.
                                                                   Use DefaultReturn.   */
};
/*----------------------------itl1 ¥ International Date/Time Information----------------*/
/* This is separated out because it is changed for 7.0 to be different than 'INTL' (1). */
/* The old INTL (1) format had a local routine field at the end. This was to be used    */
/* for localized string sorting code. However, for the last 4 years, string sorting has */
/* been handled with the 'itl2' resource, and the local routine field has been ignored. */
/* In most 'itl1' resources, it just contains a single RTS instruction (hex $4E75).     */
/* This field is now used to flag the 7.0 extensions. If it is $4E75, the old format is */
/* assumed. If it is $A89F, the new format is assumed: $A89F is unimplemented trap,     */
/* which would never have occurred as the first instruction in a local routine. Old     */
/* 'itl1' resources with a local routine will be handled correctly by the code, but can */
/* not be Rezzed or DeRezzed with this template. They should be updated to either have  */
/* $4E75 in the local routine field, with nothing else following, or to have $A89F in   */
/* the local routine filed, followed by extended data in the format given below. The    */
/* version field was not used to mark this change because various current 'itl1'        */
/* resources use very different version numbers, including 0, 1, and 128.               */

type 'itl1' {
        /* Day names */
        array [7] {
            pstring[15];                                        /* Sunday, Monday...    */
        };

        /* Month names */
        array [12] {
            pstring[15];                                        /* January, February... */
        };

        byte        dayName, none=255;                          /* suppressDay          */
        byte        dayMonYear, monDayYear = 255;               /* longDate format      */
        byte        noDayLeadZero, dayLeadZero = 255;           /* dayLeading0          */
        byte;                                                   /* abbrLen              */
        string[4];                                              /* st0                  */
        string[4];                                              /* st1                  */
        string[4];                                              /* st2                  */
        string[4];                                              /* st3                  */
        string[4];                                              /* st4                  */
        byte;                                                   /* region code          */
        byte;                                                   /* version              */
        switch  {
            case oldFormat:
                key hex integer = $4E75;                        /* old format key       */
            case extFormat:
                key hex integer = $A89F;                        /* extended format key  */
                hex integer;                                    /* version              */
                hex integer;                                    /* format               */
                integer;                                        /* calendar code        */
                unsigned longint = extraDays >> 3;                          /* offset to extraDays table    */
                unsigned longint = (endExtraDays - extraDays) >> 3;         /* length of extraDays table    */
                unsigned longint = extraMonths >> 3;                        /* offset to extraMonths table  */
                unsigned longint = (endExtraMonths - extraMonths) >> 3;     /* length of extraMonths table  */
                unsigned longint = abbrevDays >> 3;                         /* offset to abbrevDays table   */
                unsigned longint = (endAbbrevDays - abbrevDays) >> 3;       /* length of abbrevDays table   */
                unsigned longint = abbrevMonths >> 3;                       /* offset to abbrevMonths table */
                unsigned longint = (endAbbrevMonths - abbrevMonths) >> 3;   /* length of abbrevMonths table */
                unsigned longint = extraSeparators >> 3;                    /* offset to extraSeparators table  */  
                unsigned longint = (endExtraSeparators - extraSeparators) >> 3; /* len of extraSeparators table */  
extraDays:
                integer = $$CountOf(extraDaysArray);            /* number of extra days */
                array extraDaysArray {                          /* array of extra names     */
                    pstring;                                    /* extra day name string    */
                };
                align word;                                                                         
endExtraDays:
extraMonths:
                integer = $$CountOf(extraMonthArray);           /* number of extra months   */
                array extraMonthArray {                         /* array of extra names     */
                    pstring;                                    /* extra month name string  */
                };
                align word;                                                                         
endExtraMonths:
abbrevDays:
                integer = $$CountOf(abbrevDaysArray);           /* number of abbrev days    */
                array abbrevDaysArray {                         /* array of abbrev names    */
                    pstring;                                    /* abbrev day name string   */
                };
                align word;                                                                         
endAbbrevDays:
abbrevMonths:
                integer = $$CountOf(abbrevMonthArray);          /* number of abbrev months  */
                array abbrevMonthArray {                        /* array of abbrev names    */
                    pstring;                                    /* abbrev month name string */
                };
                align word;                                                                         
endAbbrevMonths:
extraSeparators:
                integer = $$CountOf(extraSeparatorsArray);      /* number of extra separators   */  
                array extraSeparatorsArray {                    /* array of separator strings   */  
                    pstring;                                    /* separator string             */  
                };
                align word;                                                                         
endExtraSeparators:
        };
};
/*----------------------------itl2 ¥ International String Comparison Package Hooks-----*/
/*
 * Note that this type assumes a particular ordering of code and tables, although
 * that order is not required in order for the resource to be used correctly by
 * the code.
 *
 * There is some ugly stuff at the end, because
 * 1. The itl2 resource has several different formats.
 * 2. The offset to the FindScriptTable is only present in one format: when length
 *    fields exist.
 * 3. Even in that format, the FindScriptTable itself is optional; if it is not
 *    present, the offset to it is 0.
 * 4. The optional FindScriptTable is preceded by the variable-length wrapTable.
 *    To determine the wrapTable length when DeRezzing, we need to use the offset
 *    to FindScriptTable if the offset exists and is nonzero. Otherwise, wrapTable
 *    extends to the end of the resource.
 * The single hex string at the end of the wrapTable is actually represented in
 * three different arrays whose dimensions are logical expressions. For a given
 * format, one of the arrays will have a dimension 1, and the others will have a
 * dimension 0.
 */

type 'itl2' {
        switch {
            case pre604:
                integer = initHook1 >> 3;                           /* init hook offset         */
                integer = fetchHook1 >> 3;                          /* fetch hook offset        */
                integer = vernierHook1 >> 3;                        /* vernier hook offset      */
                integer = projectHook1 >> 3;                        /* project hook offset      */
                bitstring[15] = rsvd1Hook1 >> 4;
                key boolean = 0;
                integer = rsvd2Hook1 >> 3;                          /* rsvd2 hook offset        */
    initHook1:
                hex string [(fetchHook1 - initHook1) >> 3];         /* initHook code            */
    fetchHook1:
                hex string [(vernierHook1 - fetchHook1) >> 3];      /* fetchHook code           */
    vernierHook1:
                hex string [(projectHook1 - vernierHook1) >> 3];    /* vernierHook code         */
    projectHook1:
                hex string [(rsvd1Hook1 - projectHook1) >> 3];      /* projectHook code         */
    rsvd1Hook1:
                hex string [(rsvd2Hook1 - rsvd1Hook1) >> 3];        /* rsvd1Hook code           */
    rsvd2Hook1:
                hex string;                                         /* rsvd2Hook/exitHook code  */
            case atLeast604:
                integer = initHook2 >> 3;                           /* init hook offset         */
                integer = fetchHook2 >> 3;                          /* fetch hook offset        */
                integer = vernierHook2 >> 3;                        /* vernier hook offset      */
                integer = projectHook2 >> 3;                        /* project hook offset      */
                bitstring[15] = -1;
                key boolean = 1;
                integer = rsvd2Hook2 >> 3;                          /* rsvd2 hook offset        */
                integer = typeList >> 3;                            /* TypeList offset          */
                integer = classArray >> 3;                          /* ClassArray offset        */
                integer = upperList >> 3;                           /* UpperList offset         */
                integer = lowerList >> 3;                           /* LowerList offset         */
                integer = upperNoMarkList >> 3;                     /* UpperNoMarkList offset   */
                integer = wordTable >> 3;                           /* WordTable offset         */
                integer = wrapTable >> 3;                           /* WrapTable offset         */
                integer = noMarkList >> 3;                          /* NoMarkList offset        */
                unsigned hex integer;                               /* version number           */
    formatCode:
                switch {
                    case noLengthFields:
                        key integer = 0;                            /* format code              */
    extraHdr:
                        hex string [(initHook2 - extraHdr) >> 3];   /* extra header, if any     */
                    case haveLengthFields:
                        key integer = 1;                            /* format code              */
                        wide array [14] {
                            integer;                                /* length fields            */
                        };
    findScriptTableOffset:
                        integer = findScriptTable[1,1] >> 3;        /* findScriptTable offset   */
                        integer;                                    /* findScriptTable length   */
                };
    initHook2:
                hex string [(fetchHook2 - initHook2) >> 3];         /* initHook code            */
    fetchHook2:
                hex string [(vernierHook2 - fetchHook2) >> 3];      /* fetchHook code           */
    vernierHook2:
                hex string [(projectHook2 - vernierHook2) >> 3];    /* vernierHook code         */
    projectHook2:
                hex string [(rsvd2Hook2 - projectHook2) >> 3];      /* projectHook code         */
    rsvd2Hook2:
                hex string [(classArray - rsvd2Hook2) >> 3];        /* rsvd2Hook/exitHook code  */                                                  /* code */
    classArray:
                hex string [(typeList - classArray) >> 3];          /* classArray table         */
    typeList:
                hex string [(upperList - typeList) >> 3];           /* typeList table           */
    upperList:
                hex string [(lowerList - upperList) >> 3];          /* upperList table          */
    lowerList:
                hex string [(upperNoMarkList - lowerList) >> 3];    /* lowerList table          */
    upperNoMarkList:
                hex string [(noMarkList - upperNoMarkList) >> 3];   /* upperNoMarkList table    */
    noMarkList:
                hex string [(wordTable - noMarkList) >> 3];         /* noMarkList table         */
    wordTable:
                switch {
                    case tupleFormat:
                        key boolean = 0;
                        hex bitstring[7];                           /* class for null           */
                        hex string [255];                           /* class for other chars    */
                        integer = $$CountOf(WordTuples) - 1;
                        wide array WordTuples {                     /* packed tuple defns       */
                            hex unsigned integer;
                        };
                    case stateFormat:
                        key boolean = 1;
                        bitstring[15];                              /* flags & format           */
                        unsigned hex integer;                       /* version                  */
                        integer = (wordClass - wordTable) >> 3;     /* offset to class table    */
                        integer = (wordAux - wordTable) >> 3;       /* offset to aux class tbl  */
                        integer = (wordBackwd - wordTable) >> 3;    /* offset to back state tbl */
                        integer = (wordForwd - wordTable) >> 3;     /* offset to forw state tbl */
                        integer;                                    /* doBackupMin              */
    wordLength:
                        integer;                                    /* length                   */
    wordClass:
                        hex string [(wordAux - wordClass) >> 3];    /* class table              */
    wordAux:
                        hex string [(wordBackwd - wordAux) >> 3];   /* aux class table          */
    wordBackwd:
                        hex string [(wordForwd - wordBackwd) >> 3]; /* state table for backup   */
    wordForwd:
                        hex string [$$Word(wordLength) - ((wordForwd - wordTable) >> 3)]; /* tbl */
                };
    endWordTable:
                hex string [(wrapTable - endWordTable) >> 3];       /* extra stuff, if any      */
    wrapTable:
                switch {
                    case tupleFormat:
                        key boolean = 0;
                        hex bitstring[7];                           /* class for null           */
                        hex string [255];                           /* class for other chars    */
                        integer = $$CountOf(WrapTuples) - 1;
                        wide array WrapTuples {                     /* packed tuple defns       */
                            hex unsigned integer;
                        };
                    case stateFormat:
                        key boolean = 1;
                        bitstring[15];                              /* flags & format           */
                        unsigned hex integer;                       /* version                  */
                        integer = (wrapClass - wrapTable) >> 3;     /* offset to class table    */
                        integer = (wrapAux - wrapTable) >> 3;       /* offset to aux class tbl  */
                        integer = (wrapBackwd - wrapTable) >> 3;    /* offset to back state tbl */
                        integer = (wrapForwd - wrapTable) >> 3;     /* offset to forw state tbl */
                        integer;                                    /* doBackupMin              */
    wrapLength:
                        integer;                                    /* length                   */
    wrapClass:
                        hex string [(wrapAux - wrapClass) >> 3];    /* class table              */
    wrapAux:
                        hex string [(wrapBackwd - wrapAux) >> 3];   /* aux class table          */
    wrapBackwd:
                        hex string [(wrapForwd - wrapBackwd) >> 3]; /* state table for backup   */
    wrapForwd:
                        hex string [$$Word(wrapLength) - ((wrapForwd - wrapTable) >> 3)]; /* tbl */
                };
    endWrapTable:
                array [$$Word(formatCode) == 0] {
                    hex string;                                     /* extra stuff, if any      */
                };
                array [$$Word(formatCode) == 1] {
                    array [$$Word(findScriptTableOffset) == 0] {
                        hex string;                                 /* extra stuff, if any      */
                    };
                    array [$$Word(findScriptTableOffset) != 0] {
                        hex string [(findScriptTable[1,1] - endWrapTable) >> 3]; /* xtra stuff? */
                    };
                };
                array [$$Word(formatCode) == 1] {
                    array [$$Word(findScriptTableOffset) != 0] {
    findScriptTable:
                        hex string;                                 /* findScriptTable          */
                    };
                };
        };
};
/*----------------------------itl4 ¥ International Tokenizer----------------------------*/
type 'itl4' {
        unsigned hex integer;                                       /* flags                    */
        literal longint = 'itl4';                                   /* resource type            */
        integer;                                                    /* resource ID              */
        unsigned hex integer;                                       /* version number           */
    formatCode:
        switch {
            case noLengthFields:
                key integer = 0;                                    /* format code              */
                integer = 0;                                        /* reserved                 */
                longint = 0;                                        /* reserved                 */
                integer = 8;                                        /* # of table entries       */
                unsigned longint = mapCharTable >> 3;               /* MapChar offset           */
                unsigned longint = stringCopyCode >> 3;             /* stringCopy offset        */
                unsigned longint = extendFetchCode >> 3;            /* extendFetch offset       */
                unsigned longint = unTokenTable >> 3;               /* unToken offset           */
                unsigned longint = defaultPartsTable >> 3;          /* defaultParts offset      */
                unsigned longint = whiteSpaceList[1] >> 3;          /* whiteSpaceList offset    */
                unsigned longint = 0;                               /* reserved offset          */
                unsigned longint = 0;                               /* reserved offset          */
            case haveLengthFields:
                key integer = 1;                                    /* format code              */
                integer = 0;                                        /* reserved                 */
                longint = 0;                                        /* reserved                 */
                integer = 8;                                        /* # of table entries       */
                unsigned longint = mapCharTable >> 3;               /* MapChar offset           */
                unsigned longint = stringCopyCode >> 3;             /* stringCopy offset        */
                unsigned longint = extendFetchCode >> 3;            /* extendFetch offset       */
                unsigned longint = unTokenTable >> 3;               /* unToken offset           */
                unsigned longint = defaultPartsTable >> 3;          /* defaultPartsTable offset */
                unsigned longint = whiteSpaceList[1] >> 3;          /* whiteSpaceList offset    */
                unsigned longint = 0;                               /* reserved offset          */
                unsigned longint = 0;                               /* reserved offset          */
                integer;                                            /* MapChar length           */
                integer;                                            /* StringCopy length        */
                integer;                                            /* extendFetch length       */
                integer;                                            /* unToken length           */
                integer;                                            /* defaultPartsTable length */
                integer;                                            /* whiteSpaceList length    */
                integer = 0;                                        /* reserved length          */
                integer = 0;                                        /* reserved length          */
        };


        unsigned longint;                                           /* extendFetch abort offset */
        longint = $$CountOf(MapChar);
    mapCharTable:
        array MapChar {
            byte;
        };

    extendFetchCode:
        hex string [(stringCopyCode - extendFetchCode) >> 3];       /* extendFetch code         */
    stringCopyCode:
        hex string [(unTokenTable - stringCopyCode) >> 3];          /* stringCopy code          */
    unTokenTable:
        integer = (defaultPartsTable - unTokenTable) >> 3;          /* unToken table size       */
    unTokenCount:
        integer = $$CountOf(unTokenStringOffsets) - 1;
        array unTokenStringOffsets {
            integer;                                                /* offset to token strings  */
        };
        array [$$Word(unTokenCount) + 1] {
            pstring;                                                /* token strings            */
            align word;
        };

    defaultPartsTable:
        integer;                                                    /* version number           */
        wide array [31] {
            unsigned byte;
            char;
        };
        integer;                                                    /* size                     */
        wide array [10] {
            unsigned byte;
            char;
        };
        integer;                                                    /* size                     */
        wide array [10] {
            unsigned byte;
            char;
        };
        integer;                                                    /* size                     */
        wide array [10] {
            unsigned byte;
            char;
        };
        integer;                                                    /* size                     */
        wide array [10] {
            unsigned byte;
            char;
        };
        hex string [20] = "";

        array [$$BitField(formatCode,30*8,32) != 0] {
    whiteSpaceList:
            integer = (whiteSpaceListEnd[1] - whiteSpaceList[1]) >> 3; /* whiteSpaceList size   */
    whiteSpaceListCount:
            integer = $$CountOf(whiteSpaceStringOffsets);
            array whiteSpaceStringOffsets {
                integer;                                            /* offset to string         */
            };
            array [$$Word(whiteSpaceListCount[1])] {
                pstring;                                            /* whitespace string        */
                align word;
            };
    whiteSpaceListEnd:
        };
};
/*----------------------------itlb ¥ International Script Bundle------------------------*/
type 'itlb' {
        integer;                                                /* itl0 id number       */              
        integer;                                                /* itl1 id number       */              
        integer;                                                /* itl2 id number       */              
        unsigned hex integer;                                   /* script flags - see
                                                                   smScriptFlags info
                                                                   in ScriptEqu.a       */
        integer;                                                /* itl4 id number       */              
        integer;                                                /* id number of optional itl5, or 0 */  
        unsigned integer;                                       /* Script language      */
        unsigned integer;                                       /* high byte - number
                                                                   representation code,
                                                                   low byte - date
                                                                   representation code  */
        integer;                                                /* KCHR id number       */              
        integer;                                                /* id no. of SICN or kcs#,kcs4,kcs8 */  
        unsigned longint;                                       /* size of script's local record    */  
        integer;                                                /* default monospace FOND ID        */  
        unsigned integer;                                       /* default monospace font size      */  
        integer;                                                /* preferred FOND ID                */  
        unsigned integer;                                       /* preferred font size              */  
        integer;                                                /* default small FOND ID            */  
        unsigned integer;                                       /* default small font size          */  
        integer;                                                /* default system FOND ID           */  
        unsigned integer;                                       /* default system font size         */  
        integer;                                                /* default application FOND ID      */  
        unsigned integer;                                       /* default application font size    */  
        integer;                                                /* default Help Mgr FOND ID         */  
        unsigned integer;                                       /* default Help Mgr font size       */  
        hex byte;                                               /* valid styles for script          */
        hex byte;                                               /* alias style for script           */  
};
/*----------------------------itlc ¥ International Configuration------------------------*/
type 'itlc' {
        unsigned integer;                                       /* system script code   */
        unsigned integer;                                       /* keyboard cache size  */
        byte            noFontForce, fontForce = 255;           /* font force flag      */
        byte            noIntlForce, intlForce = 255;           /* intl force flag      */
        byte            noOldKeyboard, oldKeyboard = 255;       /* old keyboard         */
        unsigned hex byte;                                      /* general flags - see
                                                                   smGenFlags info in
                                                                   ScriptEqu.a          */
        integer;                                                /* script icon offset   (not used in Sys 7 ) */
        byte            rightOffset, leftOffset = 255;          /* menu side for offset (not used in Sys 7 ) */
        byte;                                                   /* icon data (rsvd)     (not used in Sys 7 ) */
        unsigned integer;                                       /* pref. region code    */

        /* system flags */
        boolean         directionLeftRight, directionRightLeft; /* to set TESysJust     */  
        fill bit[15];                                           /* reserved             */  

        /* for future expansion */
        hex string [32];                                        /* reserved             */  
};
/*----------------------------itlk ¥ Keyboard-dependent Remapping-----------------------*/
type 'itlk' {
        integer = $$CountOf(RemapItemArray);
        wide array RemapItemArray {
            integer;                                            /* keyboard type        */
            hex byte;                                           /* old modifier bits    */
            byte;                                               /* old key code         */
            hex byte;                                           /* modifier bits mask   */
            byte;                                               /* key code mask        */
            hex byte;                                           /* new modifier bits    */
            byte;                                               /* new key code         */
        };
};
/*----------------------------itlm ¥ Multiscript mapping/sorting-----------------------*/
type 'itlm' {
        unsigned hex integer;                                   /* version number                   */
        unsigned hex integer;                                   /* format code                      */
        unsigned integer = 3;                                   /* number of offset/length pairs    */
        unsigned longint = scriptData >> 3;                     /* offset to scriptData table       */
        unsigned longint = (endScriptData - scriptData) >> 3;   /* length of scriptData table       */
        unsigned longint = langData >> 3;                       /* offset to langData table         */
        unsigned longint = (endLangData - langData) >> 3;       /* length of langData table         */
        unsigned longint = regionData >> 3;                     /* offset to regionData table       */
        unsigned longint = (endRegionData - regionData) >> 3;   /* length of regionData table       */

scriptData:
        integer;                                    /* max script code for script->lang mapping     */
        integer;                                    /* default lang code for scripts not in table   */
        integer = $$CountOf(scriptArray);           /* number of entries in table                   */
        array scriptArray {                         /* entries are ordered by script sort order     */
            integer;                                /* script code                                  */
            integer;                                /* default language code for script             */
        };
endScriptData:

langData:
        integer;                                    /* max lang code for lang->script mapping       */
        integer;                                    /* default script code for langs not in table   */
        integer = $$CountOf(langArray);             /* number of entries in table                   */
        array langArray {                           /* entries are ordered by lang sort order       */
            integer;                                /* language code                                */
            integer;                                /* code of script to which this lang belongs    */
        };
endLangData:

regionData:
        integer;                                    /* max region code for region->lang mapping     */
        integer;                                    /* default lang code for regions not in table   */
        integer = $$CountOf(regionArray);           /* number of entries in table                   */
        array regionArray {                         /* entries are ordered by region sort order     */
            integer;                                /* region code                                  */
            integer;                                /* parent language code for region              */
        };
endRegionData:
};


#endif /* __INTLRESOURCES_R__ */

