/*
     File:       CarbonCore/TextCommon.h
 
     Contains:   TextEncoding-related types and constants, and prototypes for related functions
 
     Copyright:  © 1995-2020 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTCOMMON__
#define __TEXTCOMMON__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)


/*
 *  Generic Text Alignment Constants
 *  
 *  Summary:
 *    These constants are implemented to supplant the old TextEdit
 *    Manager constants ( teFlushDefault, teCenter teFlushRight,
 *    teFlushLeft ) These constants are used outside the context of the
 *    legacy TextEdit Manager Framework. Use these as you would use the
 *    old TextEdit.h constants to specify how text should be justified
 *    (word aligned.) The new constants use the same values as the the
 *    old TextEdit ones, for backwards compatibility.
 */
enum {

  /*
   * Flush according to the line direction
   */
  kTextFlushDefault             = 0,

  /*
   * Center justify (word alignment)
   */
  kTextCenter                   = 1,

  /*
   * Flush right
   */
  kTextFlushRight               = -1,

  /*
   * Flush left
   */
  kTextFlushLeft                = -2
};

/* TextEncodingBase type & values */
/* (values 0-32 correspond to the Script Codes defined in Inside Macintosh: Text pages 6-52 and 6-53 */
typedef UInt32                          TextEncodingBase;
enum {
                                        /* Mac OS encodings*/
  kTextEncodingMacRoman         = 0,
  kTextEncodingMacJapanese      = 1,
  kTextEncodingMacChineseTrad   = 2,
  kTextEncodingMacKorean        = 3,
  kTextEncodingMacArabic        = 4,
  kTextEncodingMacHebrew        = 5,
  kTextEncodingMacGreek         = 6,
  kTextEncodingMacCyrillic      = 7,
  kTextEncodingMacDevanagari    = 9,
  kTextEncodingMacGurmukhi      = 10,
  kTextEncodingMacGujarati      = 11,
  kTextEncodingMacOriya         = 12,
  kTextEncodingMacBengali       = 13,
  kTextEncodingMacTamil         = 14,
  kTextEncodingMacTelugu        = 15,
  kTextEncodingMacKannada       = 16,
  kTextEncodingMacMalayalam     = 17,
  kTextEncodingMacSinhalese     = 18,
  kTextEncodingMacBurmese       = 19,
  kTextEncodingMacKhmer         = 20,
  kTextEncodingMacThai          = 21,
  kTextEncodingMacLaotian       = 22,
  kTextEncodingMacGeorgian      = 23,
  kTextEncodingMacArmenian      = 24,
  kTextEncodingMacChineseSimp   = 25,
  kTextEncodingMacTibetan       = 26,
  kTextEncodingMacMongolian     = 27,
  kTextEncodingMacEthiopic      = 28,
  kTextEncodingMacCentralEurRoman = 29,
  kTextEncodingMacVietnamese    = 30,
  kTextEncodingMacExtArabic     = 31,   /* The following use script code 0, smRoman*/
  kTextEncodingMacSymbol        = 33,
  kTextEncodingMacDingbats      = 34,
  kTextEncodingMacTurkish       = 35,
  kTextEncodingMacCroatian      = 36,
  kTextEncodingMacIcelandic     = 37,
  kTextEncodingMacRomanian      = 38,
  kTextEncodingMacCeltic        = 39,
  kTextEncodingMacGaelic        = 40,
  kTextEncodingMacKeyboardGlyphs = 41
};

/* The following are older names for backward compatibility*/
enum {
  kTextEncodingMacTradChinese   = kTextEncodingMacChineseTrad,
  kTextEncodingMacRSymbol       = 8,
  kTextEncodingMacSimpChinese   = kTextEncodingMacChineseSimp,
  kTextEncodingMacGeez          = kTextEncodingMacEthiopic,
  kTextEncodingMacEastEurRoman  = kTextEncodingMacCentralEurRoman,
  kTextEncodingMacUninterp      = 32
};


/*
   Beginning in Mac OS 8.5, the following meta-value is used to indicate Unicode in some parts
   of the Mac OS which previously only expected a Mac OS script code. In some of these places,
   only 7 bits are available to indicate encoding (script code), so kTextEncodingUnicodeDefault
   cannot be used. For example, kTextEncodingMacUnicode can be used to indicate Unicode in the
   7-bit script code field of a Unicode input method's ComponentDescription.componentFlags field;
   it can also be used to indicate Unicode in the 16-bit script code field of an AppleEvent's
   typeIntlWritingCode text tag.
*/
enum {
  kTextEncodingMacUnicode       = 0x7E  /* Meta-value, Unicode as a Mac encoding*/
};

/* Variant Mac OS encodings that use script codes other than 0*/
enum {
                                        /* The following use script code 4, smArabic*/
  kTextEncodingMacFarsi         = 0x8C, /* Like MacArabic but uses Farsi digits*/
                                        /* The following use script code 7, smCyrillic*/
  kTextEncodingMacUkrainian     = 0x98, /* Meta-value in TEC 1.5 & later; maps to kTextEncodingMacCyrillic variant    */
                                        /* The following use script code 28, smEthiopic*/
  kTextEncodingMacInuit         = 0xEC, /* The following use script code 32, smUnimplemented*/
  kTextEncodingMacVT100         = 0xFC  /* VT100/102 font from Comm Toolbox: Latin-1 repertoire + box drawing etc*/
};

/* Special Mac OS encodings*/
enum {
  kTextEncodingMacHFS           = 0xFF  /* Meta-value, should never appear in a table.*/
};

/* Unicode & ISO UCS encodings begin at 0x100*/
enum {
  kTextEncodingUnicodeDefault   = 0x0100, /* Meta-value, should never appear in a table.*/
  kTextEncodingUnicodeV1_1      = 0x0101,
  kTextEncodingISO10646_1993    = 0x0101, /* Code points identical to Unicode 1.1*/
  kTextEncodingUnicodeV2_0      = 0x0103, /* New location for Korean Hangul*/
  kTextEncodingUnicodeV2_1      = 0x0103, /* We treat both Unicode 2.0 and Unicode 2.1 as 2.1*/
  kTextEncodingUnicodeV3_0      = 0x0104,
  kTextEncodingUnicodeV3_1      = 0x0105, /* Adds characters requiring surrogate pairs in UTF-16*/
  kTextEncodingUnicodeV3_2      = 0x0106,
  kTextEncodingUnicodeV4_0      = 0x0108,
  kTextEncodingUnicodeV5_0      = 0x010A,
  kTextEncodingUnicodeV5_1      = 0x010B, /* No constant for Unicode 5.2, but leave an opening.*/
  kTextEncodingUnicodeV6_0      = 0x010D, /* Adds many symbols, including emoji support.*/
  kTextEncodingUnicodeV6_1      = 0x010E, /* Adds emoji variation sequences, properties changes.*/
  kTextEncodingUnicodeV6_3      = 0x0110, /* Adds new bidi controls.*/
  kTextEncodingUnicodeV7_0      = 0x0111, /* Adds RUBLE SIGN, symbols from Wingdings/Webdings.*/
  kTextEncodingUnicodeV8_0      = 0x0112, /* Adds LARI SIGN, lowercase Cherokee, emoji modifiers, CJK Ext E, 6 scripts.*/
  kTextEncodingUnicodeV9_0      = 0x0113, /* Adds Tangut and 5 other scripts, 72 emoji.*/
  kTextEncodingUnicodeV10_0     = 0x0114, /* Adds CJK Ext F, Hentaigana, 4 scripts, Bitcoin sign, 56 emoji...*/
  kTextEncodingUnicodeV11_0     = 0x0115, /* Adds Georgian Mtavruli capitals, 7 scripts, Copyleft, 66 emoji...*/
  kTextEncodingUnicodeV12_1     = 0x0116, /* Adds 4 new scripts, more letters for 5, Marca reg., 61 emoji...*/
  kTextEncodingUnicodeV13_0     = 0x0117  /* Adds 4 new scripts, more letters for 3, CJK Ext G, 55 emoji...*/
};

/* ISO 8-bit and 7-bit encodings begin at 0x200*/
enum {
  kTextEncodingISOLatin1        = 0x0201, /* ISO 8859-1, Western European*/
  kTextEncodingISOLatin2        = 0x0202, /* ISO 8859-2, Central European*/
  kTextEncodingISOLatin3        = 0x0203, /* ISO 8859-3, South European (Maltese...)*/
  kTextEncodingISOLatin4        = 0x0204, /* ISO 8859-4, North European & some Baltic*/
  kTextEncodingISOLatinCyrillic = 0x0205, /* ISO 8859-5*/
  kTextEncodingISOLatinArabic   = 0x0206, /* ISO 8859-6, = ASMO 708, =DOS CP 708*/
  kTextEncodingISOLatinGreek    = 0x0207, /* ISO 8859-7*/
  kTextEncodingISOLatinHebrew   = 0x0208, /* ISO 8859-8*/
  kTextEncodingISOLatin5        = 0x0209, /* ISO 8859-9, Turkish*/
  kTextEncodingISOLatin6        = 0x020A, /* ISO 8859-10, Nordic                    */
  kTextEncodingISOLatin7        = 0x020D, /* ISO 8859-13, Baltic Rim                   */
  kTextEncodingISOLatin8        = 0x020E, /* ISO 8859-14, Celtic                    */
  kTextEncodingISOLatin9        = 0x020F, /* ISO 8859-15, 8859-1 changed for EURO & CP1252 letters  */
  kTextEncodingISOLatin10       = 0x0210 /* ISO 8859-16, Romanian*/
};

/* MS-DOS & Windows encodings begin at 0x400*/
enum {
  kTextEncodingDOSLatinUS       = 0x0400, /* code page 437*/
  kTextEncodingDOSGreek         = 0x0405, /* code page 737 (formerly code page 437G)*/
  kTextEncodingDOSBalticRim     = 0x0406, /* code page 775*/
  kTextEncodingDOSLatin1        = 0x0410, /* code page 850, "Multilingual"*/
  kTextEncodingDOSGreek1        = 0x0411, /* code page 851*/
  kTextEncodingDOSLatin2        = 0x0412, /* code page 852, Slavic*/
  kTextEncodingDOSCyrillic      = 0x0413, /* code page 855, IBM Cyrillic*/
  kTextEncodingDOSTurkish       = 0x0414, /* code page 857, IBM Turkish*/
  kTextEncodingDOSPortuguese    = 0x0415, /* code page 860*/
  kTextEncodingDOSIcelandic     = 0x0416, /* code page 861*/
  kTextEncodingDOSHebrew        = 0x0417, /* code page 862*/
  kTextEncodingDOSCanadianFrench = 0x0418, /* code page 863*/
  kTextEncodingDOSArabic        = 0x0419, /* code page 864*/
  kTextEncodingDOSNordic        = 0x041A, /* code page 865*/
  kTextEncodingDOSRussian       = 0x041B, /* code page 866*/
  kTextEncodingDOSGreek2        = 0x041C, /* code page 869, IBM Modern Greek*/
  kTextEncodingDOSThai          = 0x041D, /* code page 874, also for Windows*/
  kTextEncodingDOSJapanese      = 0x0420, /* code page 932, also for Windows; Shift-JIS with additions*/
  kTextEncodingDOSChineseSimplif = 0x0421, /* code page 936, also for Windows; was EUC-CN, now GBK (EUC-CN extended)*/
  kTextEncodingDOSKorean        = 0x0422, /* code page 949, also for Windows; Unified Hangul Code (EUC-KR extended)*/
  kTextEncodingDOSChineseTrad   = 0x0423, /* code page 950, also for Windows; Big-5*/
  kTextEncodingWindowsLatin1    = 0x0500, /* code page 1252*/
  kTextEncodingWindowsANSI      = 0x0500, /* code page 1252 (alternate name)*/
  kTextEncodingWindowsLatin2    = 0x0501, /* code page 1250, Central Europe*/
  kTextEncodingWindowsCyrillic  = 0x0502, /* code page 1251, Slavic Cyrillic*/
  kTextEncodingWindowsGreek     = 0x0503, /* code page 1253*/
  kTextEncodingWindowsLatin5    = 0x0504, /* code page 1254, Turkish*/
  kTextEncodingWindowsHebrew    = 0x0505, /* code page 1255*/
  kTextEncodingWindowsArabic    = 0x0506, /* code page 1256*/
  kTextEncodingWindowsBalticRim = 0x0507, /* code page 1257*/
  kTextEncodingWindowsVietnamese = 0x0508, /* code page 1258*/
  kTextEncodingWindowsKoreanJohab = 0x0510 /* code page 1361, for Windows NT*/
};

/* Various national standards begin at 0x600*/
enum {
  kTextEncodingUS_ASCII         = 0x0600,
  kTextEncodingANSEL            = 0x0601, /* ANSEL (ANSI Z39.47) for library use*/
  kTextEncodingJIS_X0201_76     = 0x0620, /* JIS Roman and 1-byte katakana (halfwidth)*/
  kTextEncodingJIS_X0208_83     = 0x0621,
  kTextEncodingJIS_X0208_90     = 0x0622,
  kTextEncodingJIS_X0212_90     = 0x0623,
  kTextEncodingJIS_C6226_78     = 0x0624,
  kTextEncodingShiftJIS_X0213   = 0x0628, /* Shift-JIS format encoding of JIS X0213 planes 1 and 2*/
  kTextEncodingJIS_X0213_MenKuTen = 0x0629, /* JIS X0213 in plane-row-column notation (3 bytes)*/
  kTextEncodingGB_2312_80       = 0x0630,
  kTextEncodingGBK_95           = 0x0631, /* annex to GB 13000-93; for Windows 95; EUC-CN extended*/
  kTextEncodingGB_18030_2000    = 0x0632, /* This is actually implemented as GB_18030_2005*/
  kTextEncodingGB_18030_2005    = 0x0632,
  kTextEncodingKSC_5601_87      = 0x0640, /* same as KSC 5601-92 without Johab annex*/
  kTextEncodingKSC_5601_92_Johab = 0x0641, /* KSC 5601-92 Johab annex*/
  kTextEncodingCNS_11643_92_P1  = 0x0651, /* CNS 11643-1992 plane 1*/
  kTextEncodingCNS_11643_92_P2  = 0x0652, /* CNS 11643-1992 plane 2*/
  kTextEncodingCNS_11643_92_P3  = 0x0653 /* CNS 11643-1992 plane 3 (was plane 14 in 1986 version)*/
};

/* ISO 2022 collections begin at 0x800*/
enum {
  kTextEncodingISO_2022_JP      = 0x0820, /* RFC 1468*/
  kTextEncodingISO_2022_JP_2    = 0x0821, /* RFC 1554*/
  kTextEncodingISO_2022_JP_1    = 0x0822, /* RFC 2237*/
  kTextEncodingISO_2022_JP_3    = 0x0823, /* JIS X0213*/
  kTextEncodingISO_2022_CN      = 0x0830, /* RFC 1922*/
  kTextEncodingISO_2022_CN_EXT  = 0x0831, /* RFC 1922*/
  kTextEncodingISO_2022_KR      = 0x0840 /* RFC 1557*/
};

/* EUC collections begin at 0x900*/
enum {
  kTextEncodingEUC_JP           = 0x0920, /* ISO 646, 1-byte katakana, JIS 208, JIS 212*/
  kTextEncodingEUC_CN           = 0x0930, /* ISO 646, GB 2312-80*/
  kTextEncodingEUC_TW           = 0x0931, /* ISO 646, CNS 11643-1992 Planes 1-16*/
  kTextEncodingEUC_KR           = 0x0940 /* RFC 1557: ISO 646, KS C 5601-1987*/
};

/* Misc standards begin at 0xA00*/
enum {
  kTextEncodingShiftJIS         = 0x0A01, /* plain Shift-JIS*/
  kTextEncodingKOI8_R           = 0x0A02, /* RFC 1489, Russian internet standard*/
  kTextEncodingBig5             = 0x0A03, /* Big-5 (has variants)*/
  kTextEncodingMacRomanLatin1   = 0x0A04, /* Mac OS Roman permuted to align with ISO Latin-1*/
  kTextEncodingHZ_GB_2312       = 0x0A05, /* HZ (RFC 1842, for Chinese mail & news)*/
  kTextEncodingBig5_HKSCS_1999  = 0x0A06, /* Big-5 with Hong Kong special char set supplement*/
  kTextEncodingVISCII           = 0x0A07, /* RFC 1456, Vietnamese*/
  kTextEncodingKOI8_U           = 0x0A08, /* RFC 2319, Ukrainian*/
  kTextEncodingBig5_E           = 0x0A09 /* Taiwan Big-5E standard*/
};

/* Other platform encodings*/
enum {
  kTextEncodingNextStepLatin    = 0x0B01, /* NextStep Latin encoding*/
  kTextEncodingNextStepJapanese = 0x0B02 /* NextStep Japanese encoding (variant of EUC-JP)*/
};

/* EBCDIC & IBM host encodings begin at 0xC00*/
enum {
  kTextEncodingEBCDIC_LatinCore = 0x0C01, /* Common base subset of EBCDIC Latin encodings*/
  kTextEncodingEBCDIC_CP037     = 0x0C02 /* code page 037, extended EBCDIC (Latin-1 set) for US,Canada...*/
};

/* Special values*/
enum {
  kTextEncodingMultiRun         = 0x0FFF, /* Multi-encoding text with external run info*/
  kTextEncodingUnknown          = 0xFFFF /* Unknown or unspecified                  */
};

/* The following are older names for backward compatibility*/
enum {
  kTextEncodingEBCDIC_US        = 0x0C01
};


/* TextEncodingVariant type & values */
typedef UInt32                          TextEncodingVariant;
/* Default TextEncodingVariant, for any TextEncodingBase*/
enum {
  kTextEncodingDefaultVariant   = 0
};

/* Variants of kTextEncodingMacRoman                                                        */
enum {
  kMacRomanDefaultVariant       = 0,    /* meta value, maps to 1 or 2 depending on System */
  kMacRomanCurrencySignVariant  = 1,    /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN*/
  kMacRomanEuroSignVariant      = 2     /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
};

/* Variants of kTextEncodingMacCyrillic (for TEC 1.5 and later)                             */
enum {
  kMacCyrillicDefaultVariant    = 0,    /* meta value, maps to 1, 2, or 3 depending on System*/
  kMacCyrillicCurrSignStdVariant = 1,   /* Mac OS < 9.0 (RU,BG), 0xFF = CURRENCY SIGN, 0xA2/0xB6 = CENT / PARTIAL DIFF.*/
  kMacCyrillicCurrSignUkrVariant = 2,   /* Mac OS < 9.0 (UA,LangKit), 0xFF = CURRENCY SIGN, 0xA2/0xB6 = GHE WITH UPTURN*/
  kMacCyrillicEuroSignVariant   = 3     /* Mac OS >= 9.0, 0xFF is EURO SIGN, 0xA2/0xB6 = GHE WITH UPTURN*/
};

/* Variants of kTextEncodingMacIcelandic                                                    */
enum {
  kMacIcelandicStdDefaultVariant = 0,   /* meta value, maps to 2 or 4 depending on System */
  kMacIcelandicTTDefaultVariant = 1,    /* meta value, maps to 3 or 5 depending on System */
                                        /* The following are for Mac OS version < 8.5, 0xDB is CURRENCY SIGN             */
  kMacIcelandicStdCurrSignVariant = 2,  /* 0xBB/0xBC are fem./masc. ordinal indicators*/
  kMacIcelandicTTCurrSignVariant = 3,   /* 0xBB/0xBC are fi/fl ligatures*/
                                        /* The following are for Mac OS version >= 8.5, 0xDB is EURO SIGN                  */
  kMacIcelandicStdEuroSignVariant = 4,  /* 0xBB/0xBC are fem./masc. ordinal indicators*/
  kMacIcelandicTTEuroSignVariant = 5    /* 0xBB/0xBC are fi/fl ligatures*/
};

/* Variants of kTextEncodingMacCroatian                                                     */
enum {
  kMacCroatianDefaultVariant    = 0,    /* meta value, maps to 1 or 2 depending on System */
  kMacCroatianCurrencySignVariant = 1,  /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN       */
  kMacCroatianEuroSignVariant   = 2     /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
};


/* Variants of kTextEncodingMacRomanian                                                     */
enum {
  kMacRomanianDefaultVariant    = 0,    /* meta value, maps to 1 or 2 depending on System */
  kMacRomanianCurrencySignVariant = 1,  /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN       */
  kMacRomanianEuroSignVariant   = 2     /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
};


/* Variants of kTextEncodingMacJapanese*/
enum {
  kMacJapaneseStandardVariant   = 0,
  kMacJapaneseStdNoVerticalsVariant = 1,
  kMacJapaneseBasicVariant      = 2,
  kMacJapanesePostScriptScrnVariant = 3,
  kMacJapanesePostScriptPrintVariant = 4,
  kMacJapaneseVertAtKuPlusTenVariant = 5
};

/* Variants of kTextEncodingMacArabic*/
enum {
  kMacArabicStandardVariant     = 0,    /* 0xC0 is 8-spoke asterisk, 0x2A & 0xAA are asterisk (e.g. Cairo)*/
  kMacArabicTrueTypeVariant     = 1,    /* 0xC0 is asterisk, 0x2A & 0xAA are multiply signs (e.g. Baghdad)*/
  kMacArabicThuluthVariant      = 2,    /* 0xC0 is Arabic five-point star, 0x2A & 0xAA are multiply signs*/
  kMacArabicAlBayanVariant      = 3     /* 8-spoke asterisk, multiply sign, Koranic ligatures & parens*/
};

/* Variants of kTextEncodingMacFarsi*/
enum {
  kMacFarsiStandardVariant      = 0,    /* 0xC0 is 8-spoke asterisk, 0x2A & 0xAA are asterisk (e.g. Tehran)*/
  kMacFarsiTrueTypeVariant      = 1     /* asterisk, multiply signs, Koranic ligatures, geometric shapes*/
};

/* Variants of kTextEncodingMacHebrew*/
enum {
  kMacHebrewStandardVariant     = 0,
  kMacHebrewFigureSpaceVariant  = 1
};

/* Variants of kTextEncodingMacGreek*/
enum {
  kMacGreekDefaultVariant       = 0,    /* meta value, maps to 1 or 2 depending on System*/
  kMacGreekNoEuroSignVariant    = 1,    /* Mac OS version < 9.2.2, 0x9C is SOFT HYPHEN, 0xFF is undefined*/
  kMacGreekEuroSignVariant      = 2     /* Mac OS version >= 9.2.2, 0x9C is EURO SIGN, 0xFF is SOFT HYPHEN*/
};

/* Variants of kTextEncodingMacVT100                                                        */
enum {
  kMacVT100DefaultVariant       = 0,    /* meta value, maps to 1 or 2 depending on System */
  kMacVT100CurrencySignVariant  = 1,    /* Mac OS version < 8.5, 0xDB is CURRENCY SIGN       */
  kMacVT100EuroSignVariant      = 2     /* Mac OS version >= 8.5, 0xDB is EURO SIGN      */
};

/* Variants of Unicode & ISO 10646 encodings*/
enum {
  kUnicodeNoSubset              = 0,
  kUnicodeNormalizationFormD    = 5,    /* canonical decomposition (NFD); excludes composed chars*/
  kUnicodeNormalizationFormC    = 3,    /* canonical composition (NFC); uses the composed chars as of Unicode 3.1*/
  kUnicodeHFSPlusDecompVariant  = 8,    /* decomposition for HFS+; doesn't decompose in 2000-2FFF, F900-FAFF, 2F800-2FAFF*/
  kUnicodeHFSPlusCompVariant    = 9     /* composition based on HFS+ decomposition*/
};

/* Variants of kTextEncodingISOLatin1*/
enum {
  kISOLatin1StandardVariant     = 0,
  kISOLatin1MusicCDVariant      = 1
};

/*
   Variants of kTextEncodingISOLatinArabic, kTextEncodingISOLatinHebrew.
   Per RFC 1556 and ECMA TR/53, there are three ways of handling bidirectional text
   in the ISO character sets 8859-6 (Arabic) and 8859-8 (Hebrew).
   1. Implicit or Logical order is "a presentation method in which the direction is
   determined by an algorithm according to the type of characters and their position
   relative to the adjacent characters and according to their primary direction." This
   is the method normally used for Unicode and for the Mac OS and Windows Arabic and
   Hebrew encodings.
   2. Visual order assumes the text is already ordered such that it can be displayed
   in a left-to-right display direction with no further directional processing. This
   is equivalent to treating all characters as having strong left-right directionality.
   This is the default assumed for internet Hebrew text encoded in ISO 8859-8, unless
   the charset label suffix specifically indicates implicit (-i) or explicit (-e)
   ordering.
   3. Explicit order is "a presentation method in which the direction is explicitly
   defined by using control sequences which are interleaved within the text and are
   used for direction determination."
*/
enum {
  kISOLatinArabicImplicitOrderVariant = 0,
  kISOLatinArabicVisualOrderVariant = 1,
  kISOLatinArabicExplicitOrderVariant = 2
};

enum {
  kISOLatinHebrewImplicitOrderVariant = 0,
  kISOLatinHebrewVisualOrderVariant = 1,
  kISOLatinHebrewExplicitOrderVariant = 2
};

/* Variants of kTextEncodingWindowsLatin1*/
enum {
  kWindowsLatin1StandardVariant = 0,
  kWindowsLatin1PalmVariant     = 1     /* PalmSource variant of cp1252*/
};

/* Variants of kTextEncodingDOSJapanese*/
enum {
  kDOSJapaneseStandardVariant   = 0,
  kDOSJapanesePalmVariant       = 1     /* PalmSource variant of cp932*/
};

/*
   Variants of EUC_CN
   The DOSVariant is like kTextEncodingDOSChineseSimplif, but with the
   basic EUC_CN part mapped as per kTextEncodingEUC_CN.
*/
enum {
  kEUC_CN_BasicVariant          = 0,
  kEUC_CN_DOSVariant            = 1
};

/*
   Variants of EUC_KR
   The DOSVariant is like kTextEncodingDOSKorean, but with the
   basic EUC_KR part mapped as per kTextEncodingEUC_KR.
*/
enum {
  kEUC_KR_BasicVariant          = 0,
  kEUC_KR_DOSVariant            = 1
};

/*
   Variants of ShiftJIS
   The DOSVariant is like kTextEncodingDOSJapanese, but with the
   basic ShiftJIS part mapped as per kTextEncodingShiftJIS.
*/
enum {
  kShiftJIS_BasicVariant        = 0,
  kShiftJIS_DOSVariant          = 1,
  kShiftJIS_MusicCDVariant      = 2     /* MusicShiftJIS, per RIS-506 (RIAJ)*/
};

/*
   Variants of Big-5 encoding
   The DOSVariant is like kTextEncodingDOSChineseTrad, but with the
   basic Big5 part mapped as per kTextEncodingBig5.
*/
enum {
  kBig5_BasicVariant            = 0,
  kBig5_StandardVariant         = 1,    /* 0xC6A1-0xC7FC: kana, Cyrillic, enclosed numerics*/
  kBig5_ETenVariant             = 2,    /* adds kana, Cyrillic, radicals, etc with hi bytes C6-C8,F9*/
  kBig5_DOSVariant              = 3
};

/* Variants of MacRomanLatin1                                                               */
enum {
  kMacRomanLatin1DefaultVariant = 0,    /* meta value, maps to others depending on System*/
  kMacRomanLatin1StandardVariant = 2,   /* permuted MacRoman, EuroSignVariant*/
  kMacRomanLatin1TurkishVariant = 6,    /* permuted MacTurkish*/
  kMacRomanLatin1CroatianVariant = 8,   /* permuted MacCroatian, EuroSignVariant*/
  kMacRomanLatin1IcelandicVariant = 11, /* permuted MacIcelandic, StdEuroSignVariant*/
  kMacRomanLatin1RomanianVariant = 14   /* permuted MacRomanian, EuroSignVariant*/
};

/* Unicode variants not yet supported (and not fully defined)*/
enum {
  kUnicodeNoCompatibilityVariant = 1,
  kUnicodeNoCorporateVariant    = 4
};

/* The following are older names for backward compatibility*/
enum {
  kMacRomanStandardVariant      = 0,
  kMacIcelandicStandardVariant  = 0,
  kMacIcelandicTrueTypeVariant  = 1,
  kJapaneseStandardVariant      = 0,
  kJapaneseStdNoVerticalsVariant = 1,
  kJapaneseBasicVariant         = 2,
  kJapanesePostScriptScrnVariant = 3,
  kJapanesePostScriptPrintVariant = 4,
  kJapaneseVertAtKuPlusTenVariant = 5,
  kTextEncodingShiftJIS_X0213_00 = 0x0628, /* Shift-JIS format encoding of JIS X0213 planes 1 and 2*/
                                        /* kJapaneseStdNoOneByteKanaVariant = 6,  // replaced by kJapaneseNoOneByteKanaOption*/
                                        /* kJapaneseBasicNoOneByteKanaVariant = 7,    // replaced by kJapaneseNoOneByteKanaOption    */
  kHebrewStandardVariant        = 0,
  kHebrewFigureSpaceVariant     = 1,    /* Old Unicode variants. Variant 2 (kUnicodeCanonicalDecompVariant, kUnicodeMaxDecomposedVariant) is ambiguous and means*/
                                        /* different things in different contexts. When normalizing (using ConvertFromUnicodeToText to convert from arbitrary*/
                                        /* Unicode to a normalized form), Unicode variant 2 means the same thing as kUnicodeNormalizationFormD (i.e. NFD).*/
                                        /* However, when converting between Unicode and traditional Mac OS encodings, Unicode variant 2 means the same thing as*/
                                        /* kUnicodeHFSPlusDecompVariant (i.e. the special HFS decomposition which excludes some character ranges from normalization).*/
                                        /* For clarity, please use the less ambiguous constants: kUnicodeNormalizationFormD = 5, kUnicodeHFSPlusDecompVariant = 8.*/
                                        /* */
  kUnicodeCanonicalDecompVariant = 2,   /* use kUnicodeNormalizationFormD or kUnicodeHFSPlusDecompVariant*/
  kUnicodeMaxDecomposedVariant  = 2,    /* use kUnicodeNormalizationFormD or kUnicodeHFSPlusDecompVariant*/
  kUnicodeCanonicalCompVariant  = 3,    /* replaced by kUnicodeNormalizationFormC*/
  kUnicodeNoComposedVariant     = 3     /* this really meant NoComposing; replaced by kUnicodeNormalizationFormC*/
};

/* TextEncodingFormat type & values */
typedef UInt32                          TextEncodingFormat;
enum {
                                        /* Default TextEncodingFormat for any TextEncodingBase*/
  kTextEncodingDefaultFormat    = 0,    /* Formats for Unicode & ISO 10646*/
  kUnicodeUTF16Format           = 0,    /* UTF16 form (16-bit units), native or external byte order (see below)*/
  kUnicodeUTF7Format            = 1,    /* UTF7 form*/
  kUnicodeUTF8Format            = 2,    /* UTF8 form*/
  kUnicodeUTF32Format           = 3,    /* UTF32 form (32-bit units), native or external byte order (see below)*/
  kUnicodeUTF16BEFormat         = 4,    /* UTF16 form, explicit big-endian byte order, no BOM*/
  kUnicodeUTF16LEFormat         = 5,    /* UTF16 form, explicit little-endian byte order, no BOM*/
  kUnicodeUTF32BEFormat         = 6,    /* UTF32 form, explicit big-endian byte order, no BOM*/
  kUnicodeUTF32LEFormat         = 7,    /* UTF32 form, explicit little-endian byte order, no BOM*/
  kUnicodeSCSUFormat            = 8,    /* Std. Compression Scheme for Unicode, Unicode Tech Std. #6*/
                                        /* Note for kUnicodeUTF16Format and kUnicodeUTF32Format:*/
                                        /* - An array of UTF16Char (UniChar) or UTF32Char is normally understood to use "internal" or*/
                                        /* platform-native byte ordering for kUnicodeUTF16Format and kUnicodeUTF32Format; the array MAY*/
                                        /* begin with byte-order mark (BOM), but the BOM should match the internal ordering.*/
                                        /* - If an array of bytes (such as char *) that can be in various encodings is specified to be*/
                                        /* in Unicode with kUnicodeUTF16Format or kUnicodeUTF32Format (not explicitly BE or LE), then it*/
                                        /* is assumed to use "external" byte ordering, which means: If there is a BOM at the beginning*/
                                        /* of text, the BOM specifies the byte ordering, otherwise big-endian is assumed.*/
                                        /* Synonyms for some Unicode formats*/
  kUnicode16BitFormat           = 0,
  kUnicode32BitFormat           = 3
};

/* TextEncoding type */
typedef UInt32                          TextEncoding;
/* name part selector for GetTextEncodingName*/
typedef UInt32                          TextEncodingNameSelector;
enum {
  kTextEncodingFullName         = 0,
  kTextEncodingBaseName         = 1,
  kTextEncodingVariantName      = 2,
  kTextEncodingFormatName       = 3
};

/* Types used in conversion */
struct TextEncodingRun {
  ByteOffset          offset;
  TextEncoding        textEncoding;
};
typedef struct TextEncodingRun          TextEncodingRun;
typedef TextEncodingRun *               TextEncodingRunPtr;
typedef const TextEncodingRun *         ConstTextEncodingRunPtr;
struct ScriptCodeRun {
  ByteOffset          offset;
  ScriptCode          script;
};
typedef struct ScriptCodeRun            ScriptCodeRun;
typedef ScriptCodeRun *                 ScriptCodeRunPtr;
typedef const ScriptCodeRun *           ConstScriptCodeRunPtr;
typedef UInt8 *                         TextPtr;
typedef const UInt8 *                   ConstTextPtr;
/* Basic types for Unicode characters and strings:*/
typedef UniChar *                       UniCharArrayPtr;
typedef const UniChar *                 ConstUniCharArrayPtr;
/*
   UniCharArrayHandle is a handle type to correspond to UniCharArrayPtr,
   i.e. a handle to an array of UniChars (UInt16s).
*/
typedef UniCharArrayPtr *               UniCharArrayHandle;
/*
   UniCharArrayOffset is used to indicate an edge offset in an array
   of UniChars (UInt16s).  
*/
typedef unsigned long                   UniCharArrayOffset;
/* enums for TextEncoding Conversion routines*/
enum {
  kTextScriptDontCare           = -128,
  kTextLanguageDontCare         = -128,
  kTextRegionDontCare           = -128
};

/* struct for TECGetInfo*/

struct TECInfo {
  UInt16              format;                 /* format code for this struct*/
  UInt16              tecVersion;             /* TEC version in BCD, e.g. 0x0121 for 1.2.1*/
  UInt32              tecTextConverterFeatures; /* bitmask indicating TEC features/fixes*/
  UInt32              tecUnicodeConverterFeatures; /* bitmask indicating UnicodeConverter features/fixes*/
  UInt32              tecTextCommonFeatures;  /* bitmask indicating TextCommon features/fixes*/
  Str31               tecTextEncodingsFolderName; /* localized name of Text Encodings folder (pascal string)*/
  Str31               tecExtensionFileName;   /* localized name of TEC extension (pascal string)*/
  UInt16              tecLowestTEFileVersion; /* Lowest version (BCD) of all files in Text Encodings folder*/
  UInt16              tecHighestTEFileVersion; /* Highest version (BCD) of all files in Text Encodings folder*/
};
typedef struct TECInfo                  TECInfo;
typedef TECInfo *                       TECInfoPtr;
typedef TECInfoPtr *                    TECInfoHandle;
/* Value for TECInfo format code*/
enum {
  kTECInfoCurrentFormat         = 2     /* any future formats will just add fields at the end*/
};

/*
   Defined feature/fix bits for tecUnicodeConverterFeatures field
   Bit:                             Meaning if set:
   ----                             ---------------
   kTECKeepInfoFixBit               Unicode Converter no longer ignores other control flags if
                                    kUnicodeKeepInfoBit is set. Bug fix in TEC Manager 1.2.1.
   kTECFallbackTextLengthFixBit     Unicode Converter honors the *srcConvLen and *destConvLen
                                    returned by caller-supplied fallback handler for any status it
                                    returns except for kTECUnmappableElementErr (previously it only
                                    honored these values if noErr was returned). Bug fix in TEC
                                    Manager 1.2.1.
   kTECTextRunBitClearFixBit        ConvertFromUnicodeToTextRun & ConvertFromUnicodeToScriptCodeRun
                                    function correctly if the kUnicodeTextRunBit is set (previously
                                    their determination of best target encoding was incorrect). Bug
                                    fix in TEC Manager 1.3.
   kTECTextToUnicodeScanFixBit      ConvertFromTextToUnicode uses an improved scanner and maintains
                                    some resulting state information, which it uses for mapping.
                                    This has several effects:
                                    - Improved mapping of 0x30-0x39 digits in Mac OS Arabic, fewer
                                      direction overrides when mapping Mac OS Arabic & Hebrew, and
                                      improved mapping of certain characters in Indic encodings.
                                    - Malformed input produces kTextMalformedInputErr.
                                    - ConvertFromTextToUnicode accepts and uses the control flags
                                      kUnicodeKeepInfoMask and kUnicodeStringUnterminatedMask.
                                    Bug fix and enhancement in TEC Manager 1.3.
   kTECAddForceASCIIChangesBit      Define new control flag bits kUnicodeForceASCIIRangeBit and
                                    kUnicodeNoHalfwidthCharsBit for use with
                                    ConvertFromTextToUnicode, ConvertFromUnicodeToText, etc.
                                    Enhancement in TEC Manager 1.4.
   kTECPreferredEncodingFixBit      CreateUnicodeToTextRunInfo and related functions fix a problem
                                    that occurred when a preferred encoding was specified that did
                                    not match the System script; the preferred script was not
                                    actually placed first in the ordered list of encodings to use.
                                    Bug fix in TEC Manager 1.4.
   kTECAddTextRunHeuristicsBit      Define new control flag bit kUnicodeTextRunHeuristicsBit for
                                    use with ConvertFromUnicodeToTextRun.
   kTECAddFallbackInterruptBit      Define new option kUnicodeFallbackInterruptSafeMask for use
                                    with SetFallbackUnicodeToText. If a client fallback handler is
                                    installed without specifying this bit, ConvertFromUnicodeToText
                                    will HLock the tables it uses (in case the fallback handler
                                    moves memory); otherwise, it won't.
*/

enum {
  kTECKeepInfoFixBit            = 0,
  kTECFallbackTextLengthFixBit  = 1,
  kTECTextRunBitClearFixBit     = 2,
  kTECTextToUnicodeScanFixBit   = 3,
  kTECAddForceASCIIChangesBit   = 4,
  kTECPreferredEncodingFixBit   = 5,
  kTECAddTextRunHeuristicsBit   = 6,
  kTECAddFallbackInterruptBit   = 7
};

enum {
  kTECKeepInfoFixMask           = 1L << kTECKeepInfoFixBit,
  kTECFallbackTextLengthFixMask = 1L << kTECFallbackTextLengthFixBit,
  kTECTextRunBitClearFixMask    = 1L << kTECTextRunBitClearFixBit,
  kTECTextToUnicodeScanFixMask  = 1L << kTECTextToUnicodeScanFixBit,
  kTECAddForceASCIIChangesMask  = 1L << kTECAddForceASCIIChangesBit,
  kTECPreferredEncodingFixMask  = 1L << kTECPreferredEncodingFixBit,
  kTECAddTextRunHeuristicsMask  = 1L << kTECAddTextRunHeuristicsBit,
  kTECAddFallbackInterruptMask  = 1L << kTECAddFallbackInterruptBit
};

/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS for common and special Unicode code values
   -------------------------------------------------------------------------------------------------
*/

enum {
  kUnicodeByteOrderMark         = 0xFEFF,
  kUnicodeObjectReplacement     = 0xFFFC, /* placeholder for non-text object*/
  kUnicodeReplacementChar       = 0xFFFD, /* Unicode replacement for unconvertable input char*/
  kUnicodeSwappedByteOrderMark  = 0xFFFE, /* not a Unicode char; byte-swapped version of FEFF*/
  kUnicodeNotAChar              = 0xFFFF /* not a Unicode char; may be used as a terminator*/
};


/*
   -------------------------------------------------------------------------------------------------
   CONSTANTS & DATA STRUCTURES for Unicode Properties
   -------------------------------------------------------------------------------------------------
*/

typedef SInt32                          UCCharPropertyType;
enum {
  kUCCharPropTypeGenlCategory   = 1,    /* requests enumeration value*/
  kUCCharPropTypeCombiningClass = 2,    /* requests numeric value 0..255*/
  kUCCharPropTypeBidiCategory   = 3,    /* requests enumeration value*/
  kUCCharPropTypeDecimalDigitValue = 4  /* requests numeric value 0..9 for decimal digit chars (get err for others)*/
};

typedef UInt32                          UCCharPropertyValue;
/* General Category enumeration values (requested by kUCCharPropTypeGenlCategory)*/
enum {
                                        /* Normative categories:*/
  kUCGenlCatOtherNotAssigned    = 0,    /* Cn Other, Not Assigned*/
  kUCGenlCatOtherControl        = 1,    /* Cc Other, Control*/
  kUCGenlCatOtherFormat         = 2,    /* Cf Other, Format*/
  kUCGenlCatOtherSurrogate      = 3,    /* Cs Other, Surrogate*/
  kUCGenlCatOtherPrivateUse     = 4,    /* Co Other, Private Use*/
  kUCGenlCatMarkNonSpacing      = 5,    /* Mn Mark, Non-Spacing*/
  kUCGenlCatMarkSpacingCombining = 6,   /* Mc Mark, Spacing Combining*/
  kUCGenlCatMarkEnclosing       = 7,    /* Me Mark, Enclosing*/
  kUCGenlCatNumberDecimalDigit  = 8,    /* Nd Number, Decimal Digit*/
  kUCGenlCatNumberLetter        = 9,    /* Nl Number, Letter*/
  kUCGenlCatNumberOther         = 10,   /* No Number, Other*/
  kUCGenlCatSeparatorSpace      = 11,   /* Zs Separator, Space*/
  kUCGenlCatSeparatorLine       = 12,   /* Zl Separator, Line*/
  kUCGenlCatSeparatorParagraph  = 13,   /* Zp Separator, Paragraph*/
  kUCGenlCatLetterUppercase     = 14,   /* Lu Letter, Uppercase*/
  kUCGenlCatLetterLowercase     = 15,   /* Ll Letter, Lowercase*/
  kUCGenlCatLetterTitlecase     = 16,   /* Lt Letter, Titlecase*/
                                        /* Informative categories:*/
  kUCGenlCatLetterModifier      = 17,   /* Lm Letter, Modifier*/
  kUCGenlCatLetterOther         = 18,   /* Lo Letter, Other*/
  kUCGenlCatPunctConnector      = 20,   /* Pc Punctuation, Connector*/
  kUCGenlCatPunctDash           = 21,   /* Pd Punctuation, Dash*/
  kUCGenlCatPunctOpen           = 22,   /* Ps Punctuation, Open*/
  kUCGenlCatPunctClose          = 23,   /* Pe Punctuation, Close*/
  kUCGenlCatPunctInitialQuote   = 24,   /* Pi Punctuation, Initial quote*/
  kUCGenlCatPunctFinalQuote     = 25,   /* Pf Punctuation, Final quote*/
  kUCGenlCatPunctOther          = 26,   /* Po Punctuation, Other*/
  kUCGenlCatSymbolMath          = 28,   /* Sm Symbol, Math*/
  kUCGenlCatSymbolCurrency      = 29,   /* Sc Symbol, Currency*/
  kUCGenlCatSymbolModifier      = 30,   /* Sk Symbol, Modifier*/
  kUCGenlCatSymbolOther         = 31    /* So Symbol, Other*/
};

/* Bidirectional Category enumeration values (requested by kUCCharPropTypeBidiCategory)*/
enum {
  kUCBidiCatNotApplicable       = 0,    /* for now use this for unassigned*/
                                        /* Strong types:*/
  kUCBidiCatLeftRight           = 1,    /* L  Left-to-Right*/
  kUCBidiCatRightLeft           = 2,    /* R  Right-to-Left*/
                                        /* Weak types:*/
  kUCBidiCatEuroNumber          = 3,    /* EN European Number*/
  kUCBidiCatEuroNumberSeparator = 4,    /* ES European Number Separator*/
  kUCBidiCatEuroNumberTerminator = 5,   /* ET European Number Terminator*/
  kUCBidiCatArabicNumber        = 6,    /* AN Arabic Number*/
  kUCBidiCatCommonNumberSeparator = 7,  /* CS Common Number Separator*/
                                        /* Separators:*/
  kUCBidiCatBlockSeparator      = 8,    /* B  Paragraph Separator (was Block Separator)*/
  kUCBidiCatSegmentSeparator    = 9,    /* S  Segment Separator*/
                                        /* Neutrals:*/
  kUCBidiCatWhitespace          = 10,   /* WS Whitespace*/
  kUCBidiCatOtherNeutral        = 11,   /* ON Other Neutrals (unassigned codes could use this)*/
                                        /* New categories for Unicode 3.0*/
  kUCBidiCatRightLeftArabic     = 12,   /* AL Right-to-Left Arabic (was Arabic Letter)*/
  kUCBidiCatLeftRightEmbedding  = 13,   /* LRE    Left-to-Right Embedding*/
  kUCBidiCatRightLeftEmbedding  = 14,   /* RLE    Right-to-Left Embedding*/
  kUCBidiCatLeftRightOverride   = 15,   /* LRO    Left-to-Right Override*/
  kUCBidiCatRightLeftOverride   = 16,   /* RLO    Right-to-Left Override*/
  kUCBidiCatPopDirectionalFormat = 17,  /* PDF    Pop Directional Format*/
  kUCBidiCatNonSpacingMark      = 18,   /* NSM    Non-Spacing Mark*/
  kUCBidiCatBoundaryNeutral     = 19,   /* BN Boundary Neutral*/
                                        /* New categories for Unicode 6.3*/
  kUCBidiCatLeftRightIsolate    = 20,   /* LRI    Left-to-Right Isolate*/
  kUCBidiCatRightLeftIsolate    = 21,   /* RLI    Right-to-Left Isolate*/
  kUCBidiCatFirstStrongIsolate  = 22,   /* FSI    First Strong Isolate*/
  kUCBidiCatPopDirectionalIsolate = 23  /* PDI    Pop Directional Isolate*/
};


/*
   -------------------------------------------------------------------------------------------------
   Prototypes for TextEncoding functions
   -------------------------------------------------------------------------------------------------
*/


/*
 *  CreateTextEncoding()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern TextEncoding 
CreateTextEncoding(
  TextEncodingBase      encodingBase,
  TextEncodingVariant   encodingVariant,
  TextEncodingFormat    encodingFormat)                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  GetTextEncodingBase()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern TextEncodingBase 
GetTextEncodingBase(TextEncoding encoding)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  GetTextEncodingVariant()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern TextEncodingVariant 
GetTextEncodingVariant(TextEncoding encoding)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  GetTextEncodingFormat()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern TextEncodingFormat 
GetTextEncodingFormat(TextEncoding encoding)                  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  ResolveDefaultTextEncoding()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern TextEncoding 
ResolveDefaultTextEncoding(TextEncoding encoding)             __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  GetTextEncodingName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern OSStatus 
GetTextEncodingName(
  TextEncoding               iEncoding,
  TextEncodingNameSelector   iNamePartSelector,
  RegionCode                 iPreferredRegion,
  TextEncoding               iPreferredEncoding,
  ByteCount                  iOutputBufLen,
  ByteCount *                oNameLength,
  RegionCode *               oActualRegion,            /* can be NULL */
  TextEncoding *             oActualEncoding,          /* can be NULL */
  TextPtr                    oEncodingName)                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  TECGetInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.2.1 and later
 */
extern OSStatus 
TECGetInfo(TECInfoHandle * tecInfo)                           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/*
 *  UpgradeScriptInfoToTextEncoding()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern OSStatus 
UpgradeScriptInfoToTextEncoding(
  ScriptCode         iTextScriptID,
  LangCode           iTextLanguageID,
  RegionCode         iRegionID,
  ConstStr255Param   iTextFontname,
  TextEncoding *     oEncoding)                               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  RevertTextEncodingToScriptInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.1 and later
 */
extern OSStatus 
RevertTextEncodingToScriptInfo(
  TextEncoding   iEncoding,
  ScriptCode *   oTextScriptID,
  LangCode *     oTextLanguageID,       /* can be NULL */
  Str255         oTextFontname)         /* can be NULL */     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  GetTextEncodingFromScriptInfo()
 *  
 *  Summary:
 *    Converts any combination of a Mac OS script code, a language
 *    code, and a region code to a text encoding.
 *  
 *  Discussion:
 *    This function is almost identical to
 *    UpgradeScriptInfoToTextEncoding except it doesn't take a font
 *    name and it is available in CoreServices.
 *  
 *  Parameters:
 *    
 *    iTextScriptID:
 *      A valid Script Manager script code. The Mac OS Script Manager
 *      defines constants for script codes using this format: smXxx. To
 *      designate the system script, specify the meta-value of
 *      smSystemScript. To indicate that you do not want to provide a
 *      script code for this parameter, specify the constant
 *      kTextScriptDontCare.
 *    
 *    iTextLanguageID:
 *      A valid Script Manager language code. The Mac OS Script Manager
 *      defines constants for language codes using this format:
 *      langXxx. To indicate that you do not want to provide a language
 *      code for this parameter, specify the constant
 *      kTextLanguageDontCare.
 *    
 *    iTextRegionID:
 *      A valid Script Manager region code. The Mac OS Script Manager
 *      defines constants for region codes using this format: verXxx.
 *      To indicate that you do not want to provide a region code for
 *      this parameter, specify the constant kTextRegionDontCare.
 *    
 *    oEncoding:
 *      A pointer to a value of type TextEncoding. On return, this
 *      value holds the text encoding specification that the function
 *      created from the other values you provided.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetTextEncodingFromScriptInfo(
  ScriptCode      iTextScriptID,
  LangCode        iTextLanguageID,
  RegionCode      iTextRegionID,
  TextEncoding *  oEncoding)                                  __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);


/*
 *  GetScriptInfoFromTextEncoding()
 *  
 *  Summary:
 *    Converts the given Mac OS text encoding specification to the
 *    corresponding script code and, if possible, language code.
 *  
 *  Discussion:
 *    This function is almost identical to
 *    RevertTextEncodingToScriptInfo except it doesn't return a font
 *    name and it is available in CoreServices.
 *  
 *  Parameters:
 *    
 *    iEncoding:
 *      The text encoding specification to be converted.
 *    
 *    oTextScriptID:
 *      A pointer to a value of type ScriptCode. On return, a Mac OS
 *      script code that corresponds to the text encoding specification
 *      you identified in the iEncoding parameter. If you do not pass a
 *      pointer for this parameter, the function returns a paramErr
 *      result code.
 *    
 *    oTextLanguageID:
 *      A pointer to a value of type LangCode. On input, if you do not
 *      want the function to return the language code, specify NULL as
 *      the value of this parameter. On return, the appropriate
 *      language code, if the language can be unambiguously derived
 *      from the text encoding specification, for example, Japanese,
 *      and you did not set the parameter to NULL. If you do not
 *      specify NULL on input and the language is ambiguous—that is,
 *      the function cannot accurately derive it from the text encoding
 *      specification—the function returns a value of
 *      kTextLanguageDontCare.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetScriptInfoFromTextEncoding(
  TextEncoding   iEncoding,
  ScriptCode *   oTextScriptID,
  LangCode *     oTextLanguageID)       /* can be NULL */     __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);



/*
 *  NearestMacTextEncodings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.5 and later
 */
extern OSStatus 
NearestMacTextEncodings(
  TextEncoding    generalEncoding,
  TextEncoding *  bestMacEncoding,
  TextEncoding *  alternateMacEncoding)                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
 *  UCGetCharProperty()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in TextCommon 1.5 and later
 */
extern OSStatus 
UCGetCharProperty(
  const UniChar *        charPtr,
  UniCharCount           textLength,
  UCCharPropertyType     propType,
  UCCharPropertyValue *  propValue)                           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/*
   -------------------------------------------------------------------------------------------------
   Surrogate pair utilities
   -------------------------------------------------------------------------------------------------
*/



#if !defined(UC_INLINE)
    #if defined(__GNUC__)
 #define UC_INLINE static __inline__
    #elif defined(__MWERKS__) || defined(__cplusplus)
  #define UC_INLINE static inline
    #endif
#endif

// surrogate ranges
enum {
  kUCHighSurrogateRangeStart  = 0xD800,
  kUCHighSurrogateRangeEnd    = 0xDBFF,
  kUCLowSurrogateRangeStart   = 0xDC00,
  kUCLowSurrogateRangeEnd     = 0xDFFF
};


/*!
   @function UCIsSurrogateHighCharacter
   Reports whether or not the character is a high surrogate.
  @param character  The character to be checked.
 @result true, if character is a high surrogate, otherwise false.
*/
UC_INLINE Boolean UCIsSurrogateHighCharacter( UniChar character ) {
    /* return ( ( character >= kUCHighSurrogateRangeStart ) && (character <= kUCHighSurrogateRangeEnd ) ? true : false ); */
   return ( ( character & 0xFC00 ) == kUCHighSurrogateRangeStart );
}

/*!
    @function UCIsSurrogateLowCharacter
    Reports whether or not the character is a low surrogate.
   @param character  The character to be checked.
 @result true, if character is a low surrogate, otherwise false.
*/
UC_INLINE Boolean UCIsSurrogateLowCharacter( UniChar character ) {
  /* return ( ( character >= kUCLowSurrogateRangeStart ) && ( character <= kUCLowSurrogateRangeEnd ) ? true : false ); */
    return ( ( character & 0xFC00 ) == kUCLowSurrogateRangeStart );
}

/*!
 @function UCGetUnicodeScalarValueForSurrogatePair
  Returns the UTF-32 value corresponding to the surrogate pair passed in.
    @param surrogateHigh  The high surrogate character.  If this parameter
         is not a valid high surrogate character, the behavior is undefined.
    @param surrogateLow  The low surrogate character.  If this parameter
           is not a valid low surrogate character, the behavior is undefined.
 @result The UTF-32 value for the surrogate pair.
*/
UC_INLINE UnicodeScalarValue UCGetUnicodeScalarValueForSurrogatePair( UniChar surrogateHigh, UniChar surrogateLow ) {
	return ( (UnicodeScalarValue)( surrogateHigh - kUCHighSurrogateRangeStart ) << 10 ) + ( surrogateLow - kUCLowSurrogateRangeStart ) + 0x0010000;
}



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TEXTCOMMON__ */

