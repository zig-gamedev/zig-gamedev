/*
     File:       TextEncodingPlugin.r
 
     Contains:   Required interface for Text Encoding Converter-Plugins
 
     Copyright:  © 1996-2011 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __TEXTENCODINGPLUGIN_R__
#define __TEXTENCODINGPLUGIN_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define kTECAvailableEncodingsResType 	'cven'
#define kTECAvailableSniffersResType 	'cvsf'
#define kTECSubTextEncodingsResType 	'cvsb'
#define kTECConversionInfoResType 		'cvif'
#define kTECMailEncodingsResType 		'cvml'
#define kTECWebEncodingsResType 		'cvwb'
#define kTECInternetNamesResType 		'cvmm'

#define kTECPluginType 					'ecpg'
#define kTECPluginCreator 				'encv'
#define kTECPluginOneToOne 				'otoo'
#define kTECPluginOneToMany 			'otom'
#define kTECPluginManyToOne 			'mtoo'
#define kTECPluginSniffObj 				'snif'

#define verUnspecified 					32767
#define kTECResourceID 					128

#define TECTextEncoding                         \
                    longint;    /* base     */  \
                    longint;    /* variant  */  \
                    longint;    /* format   */

#define TECLocale   int Region; /* Region   */

type kTECAvailableEncodingsResType {
    longint = $$CountOf (memberArray);
    Array memberArray {
        memberStart:
        TECTextEncoding /* encoding */
        memberEnd:
    };
};

type kTECAvailableSniffersResType {
    longint = $$CountOf (memberArray);
    Array memberArray {
        memberStart:
        TECTextEncoding /* encoding */
        memberEnd:
    };
};

type kTECSubTextEncodingsResType {
    longint = $$CountOf (memberArray); /* number of sets of subencodings in resource */
    Array memberArray {
        memberStart:
        ListStart:
        
        longint = (ListEnd[$$ArrayIndex(memberArray)] - ListStart[$$ArrayIndex(memberArray)]) / 8 - 4; /* offset to next item */
        TECTextEncoding /* search encoding */
        longint = $$CountOf (subEncodingsArray); /* number of encodings in resource */
        
        Array subEncodingsArray {
            TECTextEncoding /* search encoding */
        };
        
        ListEnd:
        memberEnd:
    };
};

type kTECConversionInfoResType {
    longint = $$CountOf (memberArray);
    Array memberArray {
        memberStart:
        TECTextEncoding /* source encoding */
        TECTextEncoding /* dest encoding */
        
        longint         res1;      /* reserved - free   */
        longint         res2;      /* reserved - free   */
        memberEnd:
    };
};

type kTECMailEncodingsResType {
    longint = $$CountOf (memberArray); /* number of sets in resource */
    Array memberArray {
        memberStart:
        ListStart:
        
        longint = (ListEnd[$$ArrayIndex(memberArray)] - ListStart[$$ArrayIndex(memberArray)]) / 8 - 4; /* offset to next item */
        longint = $$CountOf (localsArray); /* number of encodings in resource */
        Array localsArray {
            TECLocale    /* search locals */   
        };
        longint = $$CountOf (mailEncodingsArray); /* number of encodings in resource */
        Array mailEncodingsArray {
            TECTextEncoding /* mail encodings */
        };
        
        ListEnd:
        memberEnd:
    };
};

type kTECWebEncodingsResType {
    longint = $$CountOf (memberArray); /* number of sets in resource */
    Array memberArray {
        memberStart:
        ListStart:
        
        longint = (ListEnd[$$ArrayIndex(memberArray)] - ListStart[$$ArrayIndex(memberArray)]) / 8 - 4; /* offset to next item */
        longint = $$CountOf (localsArray); /* number of encodings in resource */
        Array localsArray {
            TECLocale    /* search locals */   
        };
        longint = $$CountOf (webEncodingsArray); /* number of encodings in resource */
        Array webEncodingsArray {
            TECTextEncoding /* web encodings */
        };
        
        ListEnd:
        memberEnd:
    };
};

type kTECInternetNamesResType {
    longint = $$CountOf (memberArray);
    Array memberArray {
        
        memberStart:
        ListStart:
        longint = (ListEnd[$$ArrayIndex(memberArray)] - ListStart[$$ArrayIndex(memberArray)]) / 8 - 4; /* offset to next item */
        TECTextEncoding  /* text encoding of name */
        
        pstring;        /* encoding name */
        align long;     /* match size to C structure size */
        
        ListEnd:
        memberEnd:
    };
};


#endif /* __TEXTENCODINGPLUGIN_R__ */

