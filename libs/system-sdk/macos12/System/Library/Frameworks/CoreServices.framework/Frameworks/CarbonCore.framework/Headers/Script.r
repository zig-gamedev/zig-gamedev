/*
     File:       Script.r
 
     Contains:   Script Manager interfaces
                 The contents of this header file are deprecated.
 
     Copyright:  © 1986-2011 by Apple Inc. All rights reserved.
*/

#ifndef __SCRIPT_R__
#define __SCRIPT_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

#define smSystemScript 					(-1)				/*  designates system script. */
#define smCurrentScript 				(-2)				/*  designates current font script. */
#define smAllScripts 					(-3)				/*  designates any script */

#define smRoman 						0
#define smJapanese 						1
#define smTradChinese 					2					/*  Traditional Chinese */
#define smKorean 						3
#define smArabic 						4
#define smHebrew 						5
#define smGreek 						6
#define smCyrillic 						7
#define smRSymbol 						8					/*  Right-left symbol */
#define smDevanagari 					9
#define smGurmukhi 						10
#define smGujarati 						11
#define smOriya 						12
#define smBengali 						13
#define smTamil 						14
#define smTelugu 						15
#define smKannada 						16					/*  Kannada/Kanarese */
#define smMalayalam 					17
#define smSinhalese 					18
#define smBurmese 						19
#define smKhmer 						20					/*  Khmer/Cambodian */
#define smThai 							21
#define smLao 							22
#define smGeorgian 						23
#define smArmenian 						24
#define smSimpChinese 					25					/*  Simplified Chinese */
#define smTibetan 						26
#define smMongolian 					27
#define smEthiopic 						28
#define smGeez 							28					/*  Synonym for smEthiopic */
#define smCentralEuroRoman 				29					/*  For Czech, Slovak, Polish, Hungarian, Baltic langs */
#define smVietnamese 					30
#define smExtArabic 					31					/*  extended Arabic */
#define smUninterp 						32					/*  uninterpreted symbols, e.g. palette symbols */

#define smUnicodeScript 				0x7E
#define smChinese 						2					/*  (Use smTradChinese or smSimpChinese) */
#define smRussian 						7					/*  Use smCyrillic */
															/*  smMaldivian = 25: deleted, no code for Maldivian */
#define smLaotian 						22					/*  Use smLao                                      */
#define smAmharic 						28					/*  Use smEthiopic or smGeez */
#define smSlavic 						29					/*  Use smCentralEuroRoman */
#define smEastEurRoman 					29					/*  Use smCentralEuroRoman */
#define smSindhi 						31					/*  Use smExtArabic */
#define smKlingon 						32

#define langEnglish 					0					/*  smRoman script */
#define langFrench 						1					/*  smRoman script */
#define langGerman 						2					/*  smRoman script */
#define langItalian 					3					/*  smRoman script */
#define langDutch 						4					/*  smRoman script */
#define langSwedish 					5					/*  smRoman script */
#define langSpanish 					6					/*  smRoman script */
#define langDanish 						7					/*  smRoman script */
#define langPortuguese 					8					/*  smRoman script */
#define langNorwegian 					9					/*  (Bokmal) smRoman script */
#define langHebrew 						10					/*  smHebrew script */
#define langJapanese 					11					/*  smJapanese script */
#define langArabic 						12					/*  smArabic script */
#define langFinnish 					13					/*  smRoman script */
#define langGreek 						14					/*  Greek script (monotonic) using smRoman script code */
#define langIcelandic 					15					/*  modified smRoman/Icelandic script */
#define langMaltese 					16					/*  Roman script */
#define langTurkish 					17					/*  modified smRoman/Turkish script */
#define langCroatian 					18					/*  modified smRoman/Croatian script */
#define langTradChinese 				19					/*  Chinese (Mandarin) in traditional characters */
#define langUrdu 						20					/*  smArabic script */
#define langHindi 						21					/*  smDevanagari script */
#define langThai 						22					/*  smThai script */
#define langKorean 						23					/*  smKorean script */

#define langLithuanian 					24					/*  smCentralEuroRoman script */
#define langPolish 						25					/*  smCentralEuroRoman script */
#define langHungarian 					26					/*  smCentralEuroRoman script */
#define langEstonian 					27					/*  smCentralEuroRoman script */
#define langLatvian 					28					/*  smCentralEuroRoman script */
#define langSami 						29					/*  language of the Sami people of N. Scandinavia              */
#define langFaroese 					30					/*  modified smRoman/Icelandic script                       */
#define langFarsi 						31					/*  modified smArabic/Farsi script */
#define langPersian 					31					/*  Synonym for langFarsi */
#define langRussian 					32					/*  smCyrillic script */
#define langSimpChinese 				33					/*  Chinese (Mandarin) in simplified characters */
#define langFlemish 					34					/*  smRoman script */
#define langIrishGaelic 				35					/*  smRoman or modified smRoman/Celtic script (without dot above)    */
#define langAlbanian 					36					/*  smRoman script */
#define langRomanian 					37					/*  modified smRoman/Romanian script */
#define langCzech 						38					/*  smCentralEuroRoman script */
#define langSlovak 						39					/*  smCentralEuroRoman script */
#define langSlovenian 					40					/*  modified smRoman/Croatian script */
#define langYiddish 					41					/*  smHebrew script */
#define langSerbian 					42					/*  smCyrillic script */
#define langMacedonian 					43					/*  smCyrillic script */
#define langBulgarian 					44					/*  smCyrillic script */
#define langUkrainian 					45					/*  modified smCyrillic/Ukrainian script */
#define langByelorussian 				46					/*  smCyrillic script */
#define langBelorussian 				46					/*  Synonym for langByelorussian                           */

#define langUzbek 						47					/*  Cyrillic script */
#define langKazakh 						48					/*  Cyrillic script */
#define langAzerbaijani 				49					/*  Azerbaijani in Cyrillic script */
#define langAzerbaijanAr 				50					/*  Azerbaijani in Arabic script */
#define langArmenian 					51					/*  smArmenian script */
#define langGeorgian 					52					/*  smGeorgian script */
#define langMoldavian 					53					/*  smCyrillic script */
#define langKirghiz 					54					/*  Cyrillic script */
#define langTajiki 						55					/*  Cyrillic script */
#define langTurkmen 					56					/*  Cyrillic script */
#define langMongolian 					57					/*  Mongolian in smMongolian script */
#define langMongolianCyr 				58					/*  Mongolian in Cyrillic script */
#define langPashto 						59					/*  Arabic script */
#define langKurdish 					60					/*  smArabic script */
#define langKashmiri 					61					/*  Arabic script */
#define langSindhi 						62					/*  Arabic script */
#define langTibetan 					63					/*  smTibetan script */
#define langNepali 						64					/*  smDevanagari script */
#define langSanskrit 					65					/*  smDevanagari script */
#define langMarathi 					66					/*  smDevanagari script */
#define langBengali 					67					/*  smBengali script */
#define langAssamese 					68					/*  smBengali script */
#define langGujarati 					69					/*  smGujarati script */
#define langPunjabi 					70					/*  smGurmukhi script */

#define langOriya 						71					/*  smOriya script */
#define langMalayalam 					72					/*  smMalayalam script */
#define langKannada 					73					/*  smKannada script */
#define langTamil 						74					/*  smTamil script */
#define langTelugu 						75					/*  smTelugu script */
#define langSinhalese 					76					/*  smSinhalese script */
#define langBurmese 					77					/*  smBurmese script */
#define langKhmer 						78					/*  smKhmer script */
#define langLao 						79					/*  smLao script */
#define langVietnamese 					80					/*  smVietnamese script */
#define langIndonesian 					81					/*  smRoman script */
#define langTagalog 					82					/*  Roman script */
#define langMalayRoman 					83					/*  Malay in smRoman script */
#define langMalayArabic 				84					/*  Malay in Arabic script */
#define langAmharic 					85					/*  smEthiopic script */
#define langTigrinya 					86					/*  smEthiopic script */
#define langOromo 						87					/*  smEthiopic script */
#define langSomali 						88					/*  smRoman script */
#define langSwahili 					89					/*  smRoman script */
#define langKinyarwanda 				90					/*  smRoman script */
#define langRuanda 						90					/*  synonym for langKinyarwanda */
#define langRundi 						91					/*  smRoman script */
#define langNyanja 						92					/*  smRoman script */
#define langChewa 						92					/*  synonym for langNyanja */
#define langMalagasy 					93					/*  smRoman script */
#define langEsperanto 					94					/*  Roman script */

#define langWelsh 						128					/*  modified smRoman/Celtic script */
#define langBasque 						129					/*  smRoman script */
#define langCatalan 					130					/*  smRoman script */
#define langLatin 						131					/*  smRoman script */
#define langQuechua 					132					/*  smRoman script */
#define langGuarani 					133					/*  smRoman script */
#define langAymara 						134					/*  smRoman script */
#define langTatar 						135					/*  Cyrillic script */
#define langUighur 						136					/*  Arabic script */
#define langDzongkha 					137					/*  (lang of Bhutan) smTibetan script */
#define langJavaneseRom 				138					/*  Javanese in smRoman script */
#define langSundaneseRom 				139					/*  Sundanese in smRoman script */
#define langGalician 					140					/*  smRoman script */
#define langAfrikaans 					141					/*  smRoman script                                    */

#define langBreton 						142					/*  smRoman or modified smRoman/Celtic script                  */
#define langInuktitut 					143					/*  Inuit script using smEthiopic script code                  */
#define langScottishGaelic 				144					/*  smRoman or modified smRoman/Celtic script                  */
#define langManxGaelic 					145					/*  smRoman or modified smRoman/Celtic script                  */
#define langIrishGaelicScript 			146					/*  modified smRoman/Gaelic script (using dot above)                */
#define langTongan 						147					/*  smRoman script                                    */
#define langGreekAncient 				148					/*  Classical Greek, polytonic orthography                     */
#define langGreenlandic 				149					/*  smRoman script                                    */
#define langAzerbaijanRoman 			150					/*  Azerbaijani in Roman script                              */
#define langNynorsk 					151					/*  Norwegian Nyorsk in smRoman */

#define langUnspecified 				32767				/*  Special code for use in resources (such as 'itlm')            */
#define langPortugese 					8					/*  Use langPortuguese */
#define langMalta 						16					/*  Use langMaltese */
#define langYugoslavian 				18					/*  (use langCroatian, langSerbian, etc.) */
#define langChinese 					19					/*  (use langTradChinese or langSimpChinese) */
#define langLettish 					28					/*  Use langLatvian                                      */
#define langLapponian 					29					/*  Use langSami */
#define langLappish 					29					/*  Use langSami */
#define langSaamisk 					29					/*  Use langSami                                     */
#define langFaeroese 					30					/*  Use langFaroese                                      */
#define langIrish 						35					/*  Use langIrishGaelic                                   */
#define langGalla 						87					/*  Use langOromo                                  */
#define langAfricaans 					141					/*  Use langAfrikaans                                 */
#define langGreekPoly 					148					/*  Use langGreekAncient */

															/*  P/N    ISO    codes  comments */
#define verUS 							0					/*        en_US */
#define verFrance 						1					/*  F  fr_FR */
#define verBritain 						2					/*  B  en_GB */
#define verGermany 						3					/*  D  de_DE */
#define verItaly 						4					/*  T  it_IT */
#define verNetherlands 					5					/*  N  nl_NL */
#define verFlemish 						6					/*  FN nl_BE     Flemish (Dutch) for Belgium                  */
#define verSweden 						7					/*  S  sv_SE */
#define verSpain 						8					/*  E  es_ES       Spanish for Spain */
#define verDenmark 						9					/*  DK da_DK */
#define verPortugal 					10					/*  PO pt_PT     Portuguese for Portugal */
#define verFrCanada 					11					/*  C  fr_CA       French for Canada */
#define verNorway 						12					/*  H  nb_NO       Bokmål */
#define verIsrael 						13					/*  HB he_IL     Hebrew */
#define verJapan 						14					/*  J  ja_JP */
#define verAustralia 					15					/*  X  en_AU       English for Australia */
#define verArabic 						16					/*  AB ar       Arabic for N Africa, Arabian peninsula, Levant */
#define verFinland 						17					/*  K  fi_FI */
#define verFrSwiss 						18					/*  SF fr_CH     French Swiss */
#define verGrSwiss 						19					/*  SD de_CH     German Swiss */
#define verGreece 						20					/*  GR el_GR     Monotonic Greek (modern) */
#define verIceland 						21					/*  IS is_IS */
#define verMalta 						22					/*  MA mt_MT */
#define verCyprus 						23					/*  CY el_CY */
#define verTurkey 						24					/*  TU tr_TR */
#define verYugoCroatian 				25					/*  YU hr_HR     Croatian for Yugoslavia; now use verCroatia (68) */

#define verNetherlandsComma 			26					/*               ID for KCHR resource - Dutch */
#define verFlemishPoint 				27					/*               ID for KCHR resource - Belgium */
#define verCanadaComma 					28					/*               ID for KCHR resource - Canadian ISO */
#define verCanadaPoint 					29					/*               ID for KCHR resource - Canadian; now unused */
#define vervariantPortugal 				30					/*               ID for resource; now unused */
#define vervariantNorway 				31					/*               ID for resource; now unused */
#define vervariantDenmark 				32					/*               ID for KCHR resource - Danish Mac Plus */

#define verIndiaHindi 					33					/*        hi_IN     Hindi for India */
#define verPakistanUrdu 				34					/*  UR ur_PK     Urdu for Pakistan                         */
#define verTurkishModified 				35					/*        tr_TR */
#define verItalianSwiss 				36					/*  ST it_CH     Italian Swiss */
#define verInternational 				37					/*  Z  en-ascii English for international use; ASCII chars only      */
															/*               38 is unassigned */
#define verRomania 						39					/*  RO ro_RO */
#define verGreekAncient 				40					/*        grc      Ancient Greek, polytonic orthography           */
#define verLithuania 					41					/*  LT lt_LT */
#define verPoland 						42					/*  PL pl_PL */
#define verHungary 						43					/*  MG hu_HU */
#define verEstonia 						44					/*  EE et_EE */
#define verLatvia 						45					/*  LV lv_LV */
#define verSami 						46					/*        se                                         */
#define verFaroeIsl 					47					/*  FA fo_FO                                     */
#define verIran 						48					/*  PS fa_IR     Persian/Farsi */
#define verRussia 						49					/*  RS ru_RU     Russian */
#define verIreland 						50					/*  GA ga_IE     Irish Gaelic for Ireland (without dot above)      */
#define verKorea 						51					/*  KH ko_KR */
#define verChina 						52					/*  CH zh_CN     Simplified Chinese */
#define verTaiwan 						53					/*  TA zh_TW     Traditional Chinese */
#define verThailand 					54					/*  TH th_TH */
#define verScriptGeneric 				55					/*  SS          Generic script system (no language or script)        */
#define verCzech 						56					/*  CZ cs_CZ */
#define verSlovak 						57					/*  SL sk_SK */
#define verEastAsiaGeneric 				58					/*  FE          Generic East Asia system (no language or script)  */
#define verMagyar 						59					/*        hu_HU     Unused; see verHungary */
#define verBengali 						60					/*        bn         Bangladesh or India */
#define verBelarus 						61					/*  BY be_BY */

#define verUkraine 						62					/*  UA uk_UA */
															/*               63 is unassigned */
#define verGreeceAlt 					64					/*        el_GR     unused                               */
#define verSerbian 						65					/*  SR sr_CS                                     */
#define verSlovenian 					66					/*  SV sl_SI                                     */
#define verMacedonian 					67					/*  MD mk_MK                                     */
#define verCroatia 						68					/*  CR hr_HR */
															/*               69 is unassigned */
#define verGermanReformed 				70					/*        de-1996     Reformed orthography (used formerly unassigned 70)    */
#define verBrazil 						71					/*  BR pt_BR     Portuguese for Brazil */
#define verBulgaria 					72					/*  BG bg_BG */
#define verCatalonia 					73					/*  CA ca_ES     Catalan for Spain */
#define verMultilingual 				74					/*  ZM mul        (no language or script) */
#define verScottishGaelic 				75					/*  GD gd */
#define verManxGaelic 					76					/*  GV gv       Isle of Man */
#define verBreton 						77					/*  BZ br */
#define verNunavut 						78					/*  IU iu_CA     Inuktitut for Canada */
#define verWelsh 						79					/*  CU cy */
															/*               80 is ID for KCHR resource - Canadian CSA */
#define verIrishGaelicScript 			81					/*  GS ga-Latg_IE  Irish Gaelic for Ireland (using dot above) */
#define verEngCanada 					82					/*  V  en_CA       English for Canada */
#define verBhutan 						83					/*  BH dz_BT     Dzongkha for Bhutan */
#define verArmenian 					84					/*  HY hy_AM */
#define verGeorgian 					85					/*  KR ka_GE */
#define verSpLatinAmerica 				86					/*  LA es_XL     Spanish for Latin America (private ISO 3166 code) */
															/*               87 is ID for KCHR resource - Spanish ISO */
#define verTonga 						88					/*  TS to_TO */
															/*               89 is ID for KCHR resource - Polish Modified */
															/*               90 is ID for KCHR resource - Catalan ISO */
#define verFrenchUniversal 				91					/*        fr         French generic */
#define verAustria 						92					/*  AU de_AT     German for Austria */
															/*  Y          93 is unused alternate for verSpLatinAmerica */
#define verGujarati 					94					/*        gu_IN */
#define verPunjabi 						95					/*        pa         Pakistan or India */
#define verIndiaUrdu 					96					/*        ur_IN     Urdu for India */
#define verVietnam 						97					/*        vi_VN */

#define verFrBelgium 					98					/*  BF fr_BE     French for Belgium                        */
#define verUzbek 						99					/*  BD uz_UZ                                     */
#define verSingapore 					100					/*  SG en_SG     (Assume English, not Chinese or Malay)                                  */
#define verNynorsk 						101					/*  NY nn_NO     Norwegian Nynorsk                         */
#define verAfrikaans 					102					/*  AK af_ZA                                     */
#define verEsperanto 					103					/*        eo                                         */
#define verMarathi 						104					/*        mr_IN                                       */
#define verTibetan 						105					/*        bo                                         */
#define verNepal 						106					/*        ne_NP                                       */
#define verGreenland 					107					/*        kl                                         */
#define verIrelandEnglish 				108					/*        en_IE     English for Ireland, with Euro for currency */

#define verFrBelgiumLux 				6					/*  Incorrect; 6 is Flemish, not French, for Belgium; use verFlemish      */
#define verBelgiumLux 					6					/*  Use verFlemish */
#define verArabia 						16					/*  Use verArabic */
#define verYugoslavia 					25					/*  Use verYugoCroatian (same number, now unused), or newer verCroatia */
#define verBelgiumLuxPoint 				27					/*  Use verFlemishPoint */
#define verIndia 						33					/*  Use verIndiaHindi */
#define verPakistan 					34					/*  Use verPakistanUrdu                                   */
#define verRumania 						39					/*  Alternate for verRomania                               */
#define verGreecePoly 					40					/*  Use verGreekAncient                                   */
#define verLapland 						46					/*  Use verSami                                        */
#define verFaeroeIsl 					47					/*  Use verFaroeIsl                                      */
#define verGenericFE 					58					/*  Use verEastAsiaGeneric                               */
#define verFarEastGeneric 				58					/*  Use verEastAsiaGeneric */
#define verByeloRussian 				61					/*  Alternate for verBelarus                               */
#define verUkrania 						62					/*  Use verUkraine */
#define verAlternateGr 					64					/*  Use verGreeceAlt                                    */
#define verSerbia 						65					/*  Alternate for verSerbian                               */
#define verSlovenia 					66					/*  Alternate for verSlovenian                             */
#define verMacedonia 					67					/*  Alternate for verMacedonian                              */
#define verBrittany 					77					/*  Alternate for verBreton                               */
#define verWales 						79					/*  Alternate for verWelsh                               */
#define verArmenia 						84					/*  Alternate for verArmenian                            */
#define verGeorgia 						85					/*  Alternate for verGeorgian                            */
#define verAustriaGerman 				92					/*  Use verAustria                                    */
#define verTibet 						105					/*  Use verTibetan                                    */

/*----------------------------KSWP - Keyboard Swapping----------------------------------*/
type 'KSWP' {
        /* The expression below that calculates the number of elements in the
           array is complicated because of the way that $$ResourceSize works.
           $$ResourceSize returns the size of the resource.  When derez'ing a
           resource, the size of the resource is known from the start.  When
           rez'ing a resource, however, the size starts out at zero and is
           incremented each time a field is appended to the resource data.  In
           other words, while rez'ing, $$ResourceSize rarely returns the final
           size of the resource.  When rez'ing a KSWP, the array size expression
           is not evaluated until all of the array elements have been parsed.
           Since each array element is 4 bytes long (if you add up all the fields),
           the number of array elements is the size of the resource at that point
           divided by four.  Since the preprocessor value of "DeRez" is zero when
           Rez'ing, the expression is equivalent to $$ResourceSize / 4.  When
           derez'ing a KSWP, the value of $$ResourceSize is constant: always the
           total size of the resource, in bytes.  Since the resource contains 4
           bytes of fill at the end (which happens to be the size of an array
           element), we have to take that in consideration when calculating the
           size of the array.  Note that the preprocessor value of "DeRez" is one,
           when derez'ing.
        */
        wide array [$$ResourceSize / 4 - DeRez]{
            hex integer     Roman, Japanese, TradChinese,       /* script code or verb  */
                            Chinese = 2, Korean, Arabic,
                            Hebrew, Greek, Cyrillic,
                            Thai = 21, SimpChinese = 25,
                            CentralEuroRoman = 29,
                            Rotate = -1, System = -2,
                            Alternate = -3, RotateKybd = -4,    /* <20> */
                            ToggleDirection = -9,
                            SetDirLeftRight = -15,
                            SetDirRightLeft = -16,
                            RomanIfOthers = -17;                /* <25> */
            unsigned byte;                                      /* virtual key code     */
            /* Modifiers */
            fill bit;                                           /* rControlOn,rControlOff*/
            fill bit;                                           /* rOptionOn,rOptionOff */
            fill bit;                                           /* rShiftOn,rShiftOff   */
            boolean     controlOff, controlOn;
            boolean     optionOff, optionOn;
            fill bit;                                           /* capsLockOn,capsLockOff*/
            boolean     shiftOff, shiftOn;
            boolean     commandOff, commandOn;
        };
        fill long;
};

#endif /* __SCRIPT_R__ */

