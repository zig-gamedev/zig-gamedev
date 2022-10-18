/*
     File:       PrintCore/PMDefinitions.h
 
     Contains:   Carbon Printing Manager Interfaces.
 
     Copyright (c) 1998-2006,2008 by Apple Inc. All Rights Reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PMDEFINITIONS__
#define __PMDEFINITIONS__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif


/* Printing objects */
typedef const void *                    PMObject;
typedef struct OpaquePMPrintSettings*   PMPrintSettings;
typedef struct OpaquePMPageFormat*      PMPageFormat;
typedef struct OpaquePMPrintSession*    PMPrintSession;
typedef struct OpaquePMPrinter*         PMPrinter;
typedef struct OpaquePMServer*          PMServer;
typedef struct OpaquePMPreset*          PMPreset;
typedef struct OpaquePMPaper*           PMPaper;
enum {
  kPMCancel                     = 0x0080 /* user hit cancel button in dialog */
};

#define kPMNoData           NULL        /* for general use */
#define kPMDontWantSize     NULL        /* for parameters which return size information */
#define kPMDontWantData     NULL        /* for parameters which return data */
#define kPMDontWantBoolean  NULL        /* for parameters which take a boolean reference */
#define kPMNoReference      NULL        /* for parameters which take an address pointer */
#define kPMDuplexDefault    kPMDuplexNone   /* Default duplex value */
/* for parameters which take a PrintSettings reference */
#define kPMNoPrintSettings              ((PMPrintSettings)NULL)
/* for parameters which take a PageFormat reference */
#define kPMNoPageFormat                 ((PMPageFormat)NULL)
/* for parameters which take a Server reference */
#define kPMServerLocal                  ((PMServer)NULL)
typedef UInt16 PMDestinationType;
enum {
  kPMDestinationInvalid         = 0,
  kPMDestinationPrinter         = 1,
  kPMDestinationFile            = 2,
  kPMDestinationFax             = 3,
  kPMDestinationPreview         = 4,
  kPMDestinationProcessPDF      = 5
};

#define kPMDestinationTypeDefault       kPMDestinationPrinter

typedef UInt16 PMOrientation;
enum {
  kPMPortrait                   = 1,
  kPMLandscape                  = 2,
  kPMReversePortrait            = 3,    /* will revert to kPortrait for current drivers */
  kPMReverseLandscape           = 4     /* will revert to kLandscape for current drivers */
};

/* Printer states */
typedef UInt16 PMPrinterState;
enum {
  kPMPrinterIdle                = 3,
  kPMPrinterProcessing          = 4,
  kPMPrinterStopped             = 5
};

typedef UInt32 PMColorSpaceModel;
enum {
  kPMUnknownColorSpaceModel     = 0,
  kPMGrayColorSpaceModel        = 1,
  kPMRGBColorSpaceModel         = 2,
  kPMCMYKColorSpaceModel        = 3,
  kPMDevNColorSpaceModel        = 4
};

#define kPMColorSpaceModelCount 4   /* total number of color space models supported */

/* Print quality modes "standard options" */
typedef UInt32 PMQualityMode;
enum {
  kPMQualityLowest              = 0x0000, /* Absolute lowest print quality */
  kPMQualityInkSaver            = 0x0001, /* Saves ink but may be slower */
  kPMQualityDraft               = 0x0004, /* Print at highest speed, ink used is secondary consideration */
  kPMQualityNormal              = 0x0008, /* Print in printers "general usage" mode for good balance between quality and speed */
  kPMQualityPhoto               = 0x000B, /* Optimize quality of photos on the page. Speed is not a concern */
  kPMQualityBest                = 0x000D, /* Get best quality output for all objects and photos. */
  kPMQualityHighest             = 0x000F /* Absolute highest quality attained from a printers */
};


/* Constants for our "standard" paper types */
typedef UInt32 PMPaperType;
enum {
  kPMPaperTypeUnknown           = 0x0000, /* Not sure yet what paper type we have. */
  kPMPaperTypePlain             = 0x0001, /* Plain paper */
  kPMPaperTypeCoated            = 0x0002, /* Has a special coating for sharper images and text */
  kPMPaperTypePremium           = 0x0003, /* Special premium coated paper */
  kPMPaperTypeGlossy            = 0x0004, /* High gloss special coating */
  kPMPaperTypeTransparency      = 0x0005, /* Used for overheads */
  kPMPaperTypeTShirt            = 0x0006 /* Used to iron on t-shirts */
};

/* Scaling alignment: */
typedef UInt16 PMScalingAlignment;
enum {
  kPMScalingPinTopLeft          = 1,
  kPMScalingPinTopRight         = 2,
  kPMScalingPinBottomLeft       = 3,
  kPMScalingPinBottomRight      = 4,
  kPMScalingCenterOnPaper       = 5,
  kPMScalingCenterOnImgArea     = 6
};

/* Duplex Mode: */
typedef UInt32 PMDuplexMode;
enum {
  kPMDuplexNone                 = 0x0001, /* Print only on one side of sheet of paper */
  kPMDuplexNoTumble             = 0x0002, /* Print on both sides of the paper, with no tumbling. */
  kPMDuplexTumble               = 0x0003, /* Print on both sides of the paper, tumbling on. */
  kPMSimplexTumble              = 0x0004 /* Print on only one side of the paper, but tumble the images while printing. */
};

/* Layout directions: */
typedef UInt16 PMLayoutDirection;
enum {
                                        /* Horizontal-major directions: */
  kPMLayoutLeftRightTopBottom   = 1,    /* English reading direction. */
  kPMLayoutLeftRightBottomTop   = 2,
  kPMLayoutRightLeftTopBottom   = 3,
  kPMLayoutRightLeftBottomTop   = 4,    /* Vertical-major directions: */
  kPMLayoutTopBottomLeftRight   = 5,
  kPMLayoutTopBottomRightLeft   = 6,
  kPMLayoutBottomTopLeftRight   = 7,
  kPMLayoutBottomTopRightLeft   = 8
};

/* Page borders: */
typedef UInt16 PMBorderType;
enum {
  kPMBorderSingleHairline       = 1,
  kPMBorderDoubleHairline       = 2,
  kPMBorderSingleThickline      = 3,
  kPMBorderDoubleThickline      = 4
};

/* 
 Options for which items to show inline in the print dialog
 This is only meant to be used in Carbon environment
 */
typedef OptionBits PMPrintDialogOptionFlags;
enum {                   
	kPMHideInlineItems					= (0L << 0), /* show nothing in the inline portion of print dialog */
	kPMShowDefaultInlineItems			= (1L << 15), /* show the default set of items (copies & pages) in the inline portion of print dialog */
	kPMShowInlineCopies					= (1L << 0), /* show Copies edit text, Collate check box and Two Sided check box (if printer supports it) in top portion of print dialog */
	kPMShowInlinePageRange				= (1L << 1), /* show Paper Range buttons and From & To Page edit text items in top portion of print dialog */
	kPMShowInlinePageRangeWithSelection	= (1L << 6), /* show Paper Range buttons with the addition of a Selection button and the From & To Page edit text items in top portion of print dialog */
	kPMShowInlinePaperSize				= (1L << 2), /* show Paper Size popup menu in top portion of print dialog */
	kPMShowInlineOrientation			= (1L << 3), /* show Orientation buttons in top portion of print dialog */
	kPMShowInlineScale					= (1L << 7), /* show Scaling edit text in top portion of print dialog */
	kPMShowPageAttributesPDE			= (1L << 8), /* add a PDE to the print dialog that contains the Page Setup information (paper size, orientation and scale) */
};

typedef UInt16 PMPPDDomain;
enum {
  kAllPPDDomains                = 1,
  kSystemPPDDomain              = 2,
  kLocalPPDDomain               = 3,
  kNetworkPPDDomain             = 4,
  kUserPPDDomain                = 5,
  kCUPSPPDDomain                = 6
};


/* Description types */
#define kPMPPDDescriptionType           CFSTR("PMPPDDescriptionType")
/* Document format strings */
#define kPMDocumentFormatDefault        CFSTR("com.apple.documentformat.default")
#define kPMDocumentFormatPDF            CFSTR("application/pdf")
#define kPMDocumentFormatPostScript     CFSTR("application/postscript")
/* Graphic context strings */
#define kPMGraphicsContextDefault       CFSTR("com.apple.graphicscontext.default")
#define kPMGraphicsContextCoreGraphics  CFSTR("com.apple.graphicscontext.coregraphics")
/* PDF Workflow Keys */
#define kPDFWorkflowItemURLKey          CFSTR("itemURL")
#define kPDFWorkflowDisplayNameKey      CFSTR("displayName")
#define kPDFWorkflowItemsKey            CFSTR("items")
#define kPDFWorkflowModifiedKey         CFSTR("wasModifiedInline")

/* Print Selection Title string */
#define kPMPrintSelectionTitleKey		CFSTR("com.apple.printSelection.title")

/* OSStatus return codes */
enum {
  kPMNoError                    = noErr,
  kPMGeneralError               = -30870,
  kPMOutOfScope                 = -30871, /* an API call is out of scope */
  kPMInvalidParameter           = paramErr, /* a required parameter is missing or invalid */
  kPMNoDefaultPrinter           = -30872, /* no default printer selected */
  kPMNotImplemented             = -30873, /* this API call is not supported */
  kPMNoSuchEntry                = -30874, /* no such entry */
  kPMInvalidPrintSettings       = -30875, /* the printsettings reference is invalid */
  kPMInvalidPageFormat          = -30876, /* the pageformat reference is invalid */
  kPMValueOutOfRange            = -30877, /* a value passed in is out of range */
};

enum {
  kPMInvalidPrintSession        = -30879, /* the print session is invalid */
  kPMInvalidPrinter             = -30880, /* the printer reference is invalid */
  kPMObjectInUse                = -30881, /* the object is in use */
  kPMInvalidPreset              = -30899/* the preset is invalid */
};


enum {
  kPMPrintAllPages              = -1
};

enum {
  kPMUnlocked                   = false
};

struct PMRect {
  double              top;
  double              left;
  double              bottom;
  double              right;
};
typedef struct PMRect                   PMRect;
struct PMResolution {
  double              hRes;
  double              vRes;
};
typedef struct PMResolution             PMResolution;
struct PMLanguageInfo {
  Str32               level;
  Str32               version;
  Str32               release;
};
typedef struct PMLanguageInfo           PMLanguageInfo;

typedef PMRect                          PMPaperMargins;

/*
	PMDataFormat is used with PMPrintSettingsCreateWithDataRepresentation and 
	PMPageFormatCreateDataRepresentation to specify the format of the data representation created.
		
	kPMDataFormatXMLDefault specifies a data format that is compatible with all Mac OS X versions. Data in
	this format can be used with the PMUnflattenXXX routines present in all versions of Mac OS X prior to 10.5.
	However, this data representation is much larger than the more modern data representations described below.
	
	kPMDataFormatXMLMinimal is only compatible and usable with Mac OS X version 10.5 and later. 
	Data in this format can be only be reconsistuted into the equivalent printing manager object with 
	the appropriate PMXXXCreateWithDataRepresentation function. The data representation produced when
	using kPMDataFormatXMLMinimal is approximately 3-5 times smaller than kPMDataFormatXMLDefault. This
	format is a good choice when execution on versions of Mac OS X prior to 10.5 is not necessary and
	an uncompressed XML representation of the data is needed. 
	
	kPMDataFormatXMLCompressed is only compatible and usable with Mac OS X version 10.5 and later. 
	Data in this format can be only be reconsistuted into the equivalent printing manager object with the 
	appropriate PMXXXCreateWithDataRepresentation function. The data representation produced when
	using kPMDataFormatXMLCompressed is approximately 20 times smaller than kPMDataFormatXMLDefault.
	This format is a good choice when execution on versions of Mac OS X prior to 10.5 is not necessary and
	the minimum data size is important.
*/
enum PMDataFormat {
  kPMDataFormatXMLDefault				= 0,
  kPMDataFormatXMLMinimal				= 1,
  kPMDataFormatXMLCompressed			= 2
};
typedef enum PMDataFormat PMDataFormat;

/* PMPreset related */
/*
	kPMPresetGraphicsTypeKey is a PMPreset attribute that specifies the graphics type of a given preset
*/
#define kPMPresetGraphicsTypeKey		CFSTR("com.apple.print.preset.graphicsType")

/*
	kPMPresetGraphicsTypePhoto is the graphics type of presets appropriate for printing photos.
*/
#define kPMPresetGraphicsTypePhoto 		CFSTR("Photo")

/*
	kPMPresetGraphicsTypeAll includes all graphics types.
*/
#define kPMPresetGraphicsTypeAll 		CFSTR("All")
/*
	kPMPresetGraphicsTypeGeneral is a graphics type that is not specific to any type of document printing.
*/
#define kPMPresetGraphicsTypeGeneral 	CFSTR("General")
/*
	kPMPresetGraphicsTypeNone excludes all graphics types.
*/
#define kPMPresetGraphicsTypeNone 		CFSTR("None")

#endif /* __PMDEFINITIONS__ */

