/*
     File:       QD/ATSUnicodeFlattening.h
 
     Contains:   Public interfaces for Apple Type Services for Unicode Imaging
 
     Version:    Quickdraw-291~1
 
     Copyright:  © 2002-2008 by Apple Inc. all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSUNICODEFLATTENING__
#define __ATSUNICODEFLATTENING__

#ifndef __ATSUNICODETYPES__
#include <ATSUI/ATSUnicodeTypes.h>
#endif


/* ---------------------------------------------------------------------------- */
/* Constants                                                                    */
/* ---------------------------------------------------------------------------- */
/*
   ATSUFlattenedDataStreamFormat is used to inform the APIs which flatten and
   unflatten style runs exactly what type of data that they should be generating
   or parsing.
*/

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

typedef UInt32 ATSUFlattenedDataStreamFormat;
enum {
  kATSUDataStreamUnicodeStyledText = 'ustl'
};


/*
   ATSUFlattenStyleRunOptions is a bitfield list of options that can be passed
   into the ATSUFlattenStyleRunsToStream API. Currently, there are no options. 
   This is here for future expansion.
*/
typedef UInt32 ATSUFlattenStyleRunOptions;
enum {
  kATSUFlattenOptionNoOptionsMask = 0x00000000
};

/*
   ATSUUnFlattenStyleRunOptions is a bitfield list of options that can be passed
   into the ATSUUnFlattenStyleRunsToStream API. Currently, there are no options. 
   This is here for future expansion.
*/
typedef UInt32 ATSUUnFlattenStyleRunOptions;
enum {
  kATSUUnFlattenOptionNoOptionsMask = 0x00000000
};


/* ---------------------------------------------------------------------------- */
/* Data Types                                                                   */
/* ---------------------------------------------------------------------------- */

/*
   ATSUStyleRunInfo is a structure that contains an index into an array of 
   unique ATSUStyle objects as well as the length of the run that the style run 
   object covers. This structure is utilized by ATSUUnflattenStyleRunsFromStream() 
   to return the style run info to the caller. 
*/
struct ATSUStyleRunInfo {
  UInt32              runLength;
  UInt32              styleObjectIndex;
};
typedef struct ATSUStyleRunInfo         ATSUStyleRunInfo;
/* ---------------------------------------------------------------------------- */
/* 'ustl' structure data structures and definitions                             */
/* ---------------------------------------------------------------------------- */
/*
   The 'ustl' data structure follows this format:
   1. Main Data Structure Block Header
   2. Flattened Text Layout Data
   3. Flattened Style Run Data
   4. Flattened Style Data
   Per the 'ustl' spec, these structures should maintain four-byte alignment. 
   For things that are variable width (such as font names), padding bytes must
   be added to ensure that this alignment is always kept.
*/

/*
   structure versioning - the version of the 'ustl' that the ATSUI parsing
   and generating functions will handle is version 2 or greater. Earlier
   versions were not completly specified and have been obsoleted.
*/
enum {
  kATSFlatDataUstlVersion0      = 0,
  kATSFlatDataUstlVersion1      = 1,
  kATSFlatDataUstlVersion2      = 2,
  kATSFlatDataUstlCurrentVersion = kATSFlatDataUstlVersion2
};

/* ------------------ */
/* Block 1 Structures */
/* ------------------ */

/*
   This is the main data structure block header. It describes the rest
   of the data and how it is structured.
*/
struct ATSFlatDataMainHeaderBlock {

                                              /* the 'ustl' version number. This needs to be the first item in the*/
                                              /* data block do as not to confuse parsers of earlier (and possibly*/
                                              /* later) versions of the spec *|*/
  UInt32              version;

                                              /* the total size of the stream in bytes, including the four bytes in*/
                                              /* the version above*/
  UInt32              sizeOfDataBlock;

                                              /* offset from the beginning of the stream to the flattened text layout data.*/
                                              /* This can be set to 0 if there are no text layouts stored in the stream.*/
  UInt32              offsetToTextLayouts;

                                              /* offset from the beginning of the stream to the flattened style run data. */
                                              /* This can be set to 0 if there is no flattened style run data in the stream*/
  UInt32              offsetToStyleRuns;

                                              /* offset to the flattened style list data. This can be set to 0 if there*/
                                              /* is no flattened style list data*/
  UInt32              offsetToStyleList;
};
typedef struct ATSFlatDataMainHeaderBlock ATSFlatDataMainHeaderBlock;
/* ------------------ */
/* Block 2 Structures */
/* ------------------ */
/*
   The Block 2 Structures are not currently used by any of ATSUI's internal parsing
   or packing routines. They are, however, part of the 'ustl' standard and are put
   here for developer conveniance, as well as to properly define the standard.
*/

/*
   This is the header that is attached to each flattened text layout. The
   number of flattened text layouts in the stucture is specified by the
   ATSFlatDataTextLayoutHeader structure that is below.
*/
struct ATSFlatDataTextLayoutDataHeader {

                                              /* the total size of this particular flattened text layout, including any*/
                                              /* padding bytes and such. */
  UInt32              sizeOfLayoutData;

                                              /* the number of characters covered by this flattened text layout*/
  UInt32              textLayoutLength;

                                              /* the byte offset relative to the start of this structure to the flattened*/
                                              /* layout control data. This can be set to zero if there are no layout*/
                                              /* controls.*/
  UInt32              offsetToLayoutControls;

                                              /* the byte offset, relative to the start of this structure to the*/
                                              /* flattened line info. This can be set to zero if there is no line info */
                                              /* in this layout.*/
  UInt32              offsetToLineInfo;

                                              /* if the offsetToLayoutControls is non-zero, then following this block*/
                                              /* there will be a ATSFlattenedLayoutDataFlattenedLayoutControlsHeader*/
                                              /* followed by an array of ATSFlattenedLayoutDataFlattenedLayoutControls*/
                                              /* structures. If the offsetToLineInfo is non-zero, then following the*/
                                              /* flattened layout controls will be a ATSFlatDataLineInfoHeader*/
                                              /* structure.*/
};
typedef struct ATSFlatDataTextLayoutDataHeader ATSFlatDataTextLayoutDataHeader;
/*
   This is the the main header for block 2. If there is a block 2, then there
   needs to be one of these. This structure is what the offsetToTextLayouts
   points to in block 1.
*/
struct ATSFlatDataTextLayoutHeader {

                                              /* the total number of flattened text layouts stored in this block.*/
                                              /* This must be non-zero, as if there were no flattened text layouts, the*/
                                              /* entire block 2 would not exist*/
  UInt32              numFlattenedTextLayouts;

                                              /* first of possibly many flattened text layouts. There should be one of*/
                                              /* these for each flattened text layout as determined by the*/
                                              /* numFlattenedTextLayouts above. */
  ATSFlatDataTextLayoutDataHeader  flattenedTextLayouts[1];
};
typedef struct ATSFlatDataTextLayoutHeader ATSFlatDataTextLayoutHeader;
/*
   This is the header for the flattened layout controls structure. This is
   the structure that a non-zero offsetToLayoutControls points to in the
   ATSFlatDataTextLayoutDataHeader
*/
struct ATSFlatDataLayoutControlsDataHeader {

                                              /* the number of flattened layout controls. It is suggested that there be*/
                                              /* at least one layout control to output the line direction for the layout*/
  UInt32              numberOfLayoutControls;

                                              /* first of possibly many flattened layout controls. There should be one */
                                              /* of these for each layout control as determined by the*/
                                              /* numberOfLayoutControls above. Of course, if there are no layout controls,*/
                                              /* then this structure shouldn't even exist. Each attribute info structure*/
                                              /* in the array could be followed by additional padding bytes in order*/
                                              /* to maintain four-byte alignment. These padding bytes are not to be*/
                                              /* included in the fValueSize member of each structure. */
  ATSUAttributeInfo   controlArray[1];
};
typedef struct ATSFlatDataLayoutControlsDataHeader ATSFlatDataLayoutControlsDataHeader;
struct ATSFlatDataLineInfoData {

                                              /* the length of this particular line in UniChars*/
  UInt32              lineLength;

                                              /* the number of line controls applied to this line. This can be set*/
                                              /* to zero if there are no special line controls applied to this line.*/
  UInt32              numberOfLineControls;

                                              /* the numberOfLineControls is non-zero, then following this structure*/
                                              /* must be an array of ATSUAttributeInfo structures. There must be one*/
                                              /* ATSUAttributeInfo structure for each numberOfLineControls above.*/
};
typedef struct ATSFlatDataLineInfoData  ATSFlatDataLineInfoData;
/*
   This structure is the main data header for the flattened line info data. This
   is what a non-zero offsetToLineInfo points to in the 
   ATSFlatDataTextLayoutDataHeader structure above.
*/
struct ATSFlatDataLineInfoHeader {

                                              /* the number of flattened line info structures that are stored in this*/
                                              /* block. This value should really be equal to the number of soft line*/
                                              /* breaks in the layout + 1. Of course if numberOfLines is zero, then*/
                                              /* this structure shouldn't even be used.*/
  UInt32              numberOfLines;

                                              /* the first in a array of ATSFlatDataLineInfoData structures. There*/
                                              /* needs to be a ATSFlatDataLineInfoData for each numberOfLines*/
                                              /* specified above.*/
  ATSFlatDataLineInfoData  lineInfoArray[1];
};
typedef struct ATSFlatDataLineInfoHeader ATSFlatDataLineInfoHeader;
/* ------------------ */
/* Block 3 Structures */
/* ------------------ */
/*
   The block 3 structures are used by ATSUI style run flattening and parsing
   functions, ATSUFlattenStyleRunsToStream and ATSUUnflattenStyleRunsFromStream
   to represent flattened style run information. These structures go hand and
   hand with the block 4 structures.
*/

/*
   This is the data header that appears before the style run data structures.
   This structure is what a non-zero offsetToStyleRuns in the
   ATSFlatDataMainHeaderBlock points to in block 1.
*/
struct ATSFlatDataStyleRunDataHeader {

                                              /* the number of style run data structures stored in this block*/
  UInt32              numberOfStyleRuns;

                                              /* the first in an array of ATSUStyleRunInfo structures. There needs to*/
                                              /* be a ATSUStyleRunInfo structure for each numberOfStyleRuns specified*/
                                              /* above. This structure is defined in ATSUnicode.h*/
  ATSUStyleRunInfo    styleRunArray[1];
};
typedef struct ATSFlatDataStyleRunDataHeader ATSFlatDataStyleRunDataHeader;
/* ------------------ */
/* Block 4 Structures */
/* ------------------ */
/*
   The block 4 structures store flattened ATSUStyle objects. This too, is
   currently used by the ATSUI style run flattening and parsing functions,
   ATSUFlattenStyleRunsToStream and ATSUUnflattenStyleRunsFromStream. 
*/

/*
   this structure forms the beginning of an individually flattened ATSUStyle
   object. 
*/
struct ATSFlatDataStyleListStyleDataHeader {

                                              /* the size of this flattened style object, including these four bytes and*/
                                              /* any padding bytes at the end of the structure. Basically, this can be*/
                                              /* used to determine where the next structure in the array begins.*/
  UInt32              sizeOfStyleInfo;

                                              /* the number of attributes set in this flattened style object. This should */
                                              /* be at least one for the font data, although it can be 0 if this is to be*/
                                              /* unspecfied.*/
  UInt32              numberOfSetAttributes;

                                              /* the number of font features set in the flattened style object. This can*/
                                              /* be set to 0 if there are no font features set in the style object. */
  UInt32              numberOfSetFeatures;

                                              /* the number of font variations set in the flattened style object. This*/
                                              /* can be set to 0 if there are no font variations set in the style object.*/
  UInt32              numberOfSetVariations;

                                              /* after this structure header, there is the following data in this block:*/

                                              /* 1. if the numberOfSetAttributes is non-zero, then there will be an*/
                                              /*       array of ATSUAttributeInfo structures immediately following the*/
                                              /*       above header data to store the style attributes. This is a variable*/
                                              /*       structure array. There must be one ATSUAttributeInfo for*/
                                              /*       for each numberOfSetAttributes. If numberOfSetAttributes is zero,*/
                                              /*       then skip to the next data section 2.*/

                                              /* 2. if the numberOfSetFeatures is non-zero, then there will be an array*/
                                              /*       of ATSFlatDataStyleListFeatureData structures immediately after*/
                                              /*       the ATSUAttributeInfo array above (if any). There must be one*/
                                              /*       ATSFlatDataStyleListFeatureData structure for each */
                                              /*       numberOfSetFeatures set in the header above. If numberOfSetFeatures*/
                                              /*       is zero, then skip to the next data section 3.*/

                                              /* 3. if the numberOfSetVariations is non-zero, then there will be an*/
                                              /*       array of ATSFlatDataStyleListVariationData immediately after the*/
                                              /*       ATSFlatDataStyleListFeatureData array above (if any). There must be*/
                                              /*       one ATSFlatDataStyleListVariationData structure for each */
                                              /*       numberOfSetVariations set in the header above.*/
};
typedef struct ATSFlatDataStyleListStyleDataHeader ATSFlatDataStyleListStyleDataHeader;
/*
   this structure is the main header for this block. This structure is what a
   non-zero offsetToStyleList in the ATSFlatDataMainHeaderBlock points to in
   block 1.
*/
struct ATSFlatDataStyleListHeader {

                                              /* the total number of flattened style objects stored in this block*/
  UInt32              numberOfStyles;

                                              /* the first in an array of flattned style entries. The data stored*/
                                              /* in them is variably sized, so a simply array access won't do for*/
                                              /* iterating through these. However, there must be one of these*/
                                              /* ATSFlatDataStyleListStyleDataHeader structures for each */
                                              /* numberOfStyles above.*/
  ATSFlatDataStyleListStyleDataHeader  styleDataArray[1];

};
typedef struct ATSFlatDataStyleListHeader ATSFlatDataStyleListHeader;
/*
   this structure stores flattened font feature data. An array of these comes
   after the array of font data attributes (if any) if the numberOfSetFeatures is
   non-zero. There must be one of these structures for each numberOfSetFeatures.
*/
struct ATSFlatDataStyleListFeatureData {

                                              /* the font feature type*/
  ATSUFontFeatureType  theFeatureType;

                                              /* the font feature selector*/
  ATSUFontFeatureSelector  theFeatureSelector;
};
typedef struct ATSFlatDataStyleListFeatureData ATSFlatDataStyleListFeatureData;
/*
   this structure stores the flattened font variation data. An array of these 
   comes after the array of ATSFlatDataStyleListFeatureData structures (if any) 
   if the numberOfSetVariations is non-zero. There must be one of these
   structures for each numberOfSetFeatures.
*/
struct ATSFlatDataStyleListVariationData {

                                              /* the variation axis*/
  ATSUFontVariationAxis  theVariationAxis;

                                              /* the variation value*/
  ATSUFontVariationValue  theVariationValue;
};
typedef struct ATSFlatDataStyleListVariationData ATSFlatDataStyleListVariationData;
/* ------------------------ */
/* Flattened Font Data Info */
/* ------------------------ */
/*
   This is somewhat of an addendum to the 'ustl' structure above. These flattened 
   data structures are stored in block 4 as a font attribute with the attribute 
   tag of kATSUFontTag. They can store font data in a few different ways, such as
   by a FontSync reference or by simple raw font name data. Just as with the 
   'ustl' above, this structure must maintain four byte alignment.
*/


/* these are the currenly supported font specifiers. */
typedef UInt32 ATSFlatDataFontSpeciferType;
enum {
                                        /* this specifier allows the storage of font data based on name data. This*/
                                        /* uses the stuctures below to store the actual data itself.*/
  kATSFlattenedFontSpecifierRawNameData = 'namd'
};

/*
   this is the main header for the font data. It dictates what type of data
   is stored. The actual data stored must match the type specified by the
   nameSpecType. 
*/
struct ATSFlatDataFontNameDataHeader {

                                              /* the type of data that is flattened in this structure*/
  ATSFlatDataFontSpeciferType  nameSpecifierType;

                                              /* the size of the data that is flattened in this structre, not including */
                                              /* any padding bytes that may be necessary to achive the four byte */
                                              /* alignment of the data, unless they are specified as part of structure, */
                                              /* such as with the ATSFlatDataFontSpecRawNameData structure.*/
  UInt32              nameSpecifierSize;

                                              /* after this header comes the flattened font name data which matches*/
                                              /* the type specified by the nameSpecifierType above. For instance, if */
                                              /* the nameSpecType is kATSFlattenedFontNameSpecifierRawNameData, the*/
                                              /* structure that immediately follows this would be a*/
                                              /* ATSFlatDataFontNameRawNameDataHeader structure. */

};
typedef struct ATSFlatDataFontNameDataHeader ATSFlatDataFontNameDataHeader;
/*
   the next two structures are only used when the nameSpecType is set to
   kATSFlattenedFontSpecifierRawNameData. They are setup to store multiple
   font name table entries for the purposes of reconstructing an ATSUFontID
   for (hopefully) the same font some time in the future.
*/
/* this is the structure in which raw font name data is actually stored. */
struct ATSFlatDataFontSpecRawNameData {

                                              /* the type of name being specified*/
  FontNameCode        fontNameType;

                                              /* the platform type of the font name, whether it be Unicode, Mac, etc.  */
                                              /* This should be specified if known. If not known, then specify*/
                                              /* kFontNoPlatform, but then all matching will be done based on the first*/
                                              /* font in the name table matching the other parameters.*/
  FontPlatformCode    fontNamePlatform;

                                              /* the script code of the font’s name based on the platform that was passed*/
                                              /* in above. If set to kFontNoScript, then the name will be matched based*/
                                              /* on the first font in the name table matching the other font name*/
                                              /* parameters.*/
  FontScriptCode      fontNameScript;

                                              /* the language of the font name. If set to kFontNoLanguage, then the name */
                                              /* will be matched based on the first font in the name table matching the*/
                                              /* other font name parameters.*/
  FontLanguageCode    fontNameLanguage;

                                              /* the length of the font name in bytes, not including any padding bytes*/
                                              /* added to maintain the four byte alignment*/
  UInt32              fontNameLength;

                                              /* after the name length comes the actual font name data itself, plus any*/
                                              /* padding bytes needed to maintain the four byte alignment.*/

};
typedef struct ATSFlatDataFontSpecRawNameData ATSFlatDataFontSpecRawNameData;
/*
   this is a header structure that defines some things constant throughout
   the entire search for the font name, as well as the array of
   ATSFlatDataFontNameData structures. In order to gaurantee that the same font 
   will be used, more than one name specifer should be stored. The standard ATSUI
   style run flattening and parsing functions, ATSUFlattenStyleRunsToStream and
   ATSUUnflattenStyleRunsFromStream. These will store both the font's full name
   (kFontFullName) as well as the font's manufacturer name (kFontManufacturerName)
   and match fonts based on both of
   these. 
*/
struct ATSFlatDataFontSpecRawNameDataHeader {

                                              /* the number of flattened font names. There must be at least one flattened */
                                              /* font name, otherwise the structure is malformed.*/
  UInt32              numberOfFlattenedNames;

                                              /* the first in an array of possibly many font name specifiers - depending*/
                                              /* on how specific the caller wants this. There must be one */
                                              /* ATSFlatDataFontNameData structure for each numberOfFlattenedNames*/
                                              /* above.*/
  ATSFlatDataFontSpecRawNameData  nameDataArray[1];

};
typedef struct ATSFlatDataFontSpecRawNameDataHeader ATSFlatDataFontSpecRawNameDataHeader;
/* ---------------------------------------------------------------------------- */
/* Style Flattening and Parsing Functions                                       */
/* ---------------------------------------------------------------------------- */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __ATSUNICODEFLATTENING__ */

