/*
     File:       PMPrintSettingsKeys.h
 
     Contains:   Mac OS X Printing Manager Print Settings Keys.
 
     Version:    Technology: Mac OS X
                 Release:    1.0
 
     Copyright  (c) 2008 by Apple Inc. All Rights Reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PMPRINTSETTINGSKEYS__
#define __PMPRINTSETTINGSKEYS__

#ifdef __cplusplus
extern "C" {
#endif

#define kPMCopiesStr                        "com.apple.print.PrintSettings.PMCopies"
#define kPMCopiesKey                        CFSTR( kPMCopiesStr )                       /* CFNumber, kCFNumberSInt32Type, number of copies to print. */

#define kPMCopyCollateStr                   "com.apple.print.PrintSettings.PMCopyCollate"
#define kPMCopyCollateKey                   CFSTR( kPMCopyCollateStr )                  /* CFBoolean, Turns on collating */

#define kPMOutputOrderStr		   "OutputOrder"
#define kPMOutputOrderKey		    CFSTR(kPMOutputOrderStr)			/* CFString, Reverse or Normal. default is Printer Specific */

#define kPMPageSetStr		   	    "page-set"
#define kPMPageSetKey			    CFSTR(kPMPageSetStr)	/* CFString, even, odd, or all. default is all */

#define kPMMirrorStr		   	    "mirror"
#define kPMMirrorKey			    CFSTR(kPMMirrorStr)		/* CFString, true or false. default is false */


#define kPMPrintSelectionOnlyStr            "com.apple.print.PrintSettings.PMPrintSelectionOnly"
#define kPMPrintSelectionOnlyKey            CFSTR( kPMPrintSelectionOnlyStr )            /* CFBoolean - True if only current selection should be printed. */

#define kPMBorderStr                        "com.apple.print.PrintSettings.PMBorder"
#define kPMBorderKey                        CFSTR( kPMBorderStr )                       /* CFBoolean - If true, we do borders. */

#define kPMBorderTypeStr                    "com.apple.print.PrintSettings.PMBorderType"
#define kPMBorderTypeKey                    CFSTR( kPMBorderTypeStr )                   /* CFNumber - kCFNumberSInt32Type, Enum (PMBorderType) */

#define kPMLayoutNUpStr                     "com.apple.print.PrintSettings.PMLayoutNUp"
#define kPMLayoutNUpKey                     CFSTR( kPMLayoutNUpStr )                    /* CFBoolean, Turns on N-Up layout. */

#define kPMLayoutRowsStr                    "com.apple.print.PrintSettings.PMLayoutRows"
#define kPMLayoutRowsKey                    CFSTR( kPMLayoutRowsStr )                   /* CFNumber - kCFNumberSInt32Type, indicates number of layout rows. */

#define kPMLayoutColumnsStr                 "com.apple.print.PrintSettings.PMLayoutColumns"
#define kPMLayoutColumnsKey                 CFSTR( kPMLayoutColumnsStr )                /* CFNumber - kCFNumberSInt32Type, indicates number of layout columns. */

#define kPMLayoutDirectionStr               "com.apple.print.PrintSettings.PMLayoutDirection"
#define kPMLayoutDirectionKey               CFSTR( kPMLayoutDirectionStr )              /* CFNumber - kCFNumberSInt32Type, Enum (PMLayoutDirection) */

#define kPMLayoutTileOrientationStr         "com.apple.print.PrintSettings.PMLayoutTileOrientation"
#define kPMLayoutTileOrientationKey         CFSTR( kPMLayoutTileOrientationStr )        /* CFNumber - kCFNumberSInt32Type, PMOrientation, 1 = portrait, 2 = landscape, etc. */

#define kPMJobStateStr                      "com.apple.print.PrintSettings.PMJobState"
#define kPMJobStateKey                      CFSTR( kPMJobStateStr )                     /* CFNumber - kCFNumberSInt32Type, Enum, active = 0, pending, hold until, hold indefinitely, aborted, finished */

#define kPMJobHoldUntilTimeStr              "com.apple.print.PrintSettings.PMJobHoldUntilTime"
#define kPMJobHoldUntilTimeKey              CFSTR( kPMJobHoldUntilTimeStr )             /* CFDate - Time we expect to print the job. */

#define kPMJobPriorityStr                   "com.apple.print.PrintSettings.PMJobPriority"
#define kPMJobPriorityKey                   CFSTR( kPMJobPriorityStr )                  /* CFNumber - kCFNumberSInt32Type, Enum, Low = 0, normal, urgent */

#define kPMDuplexingStr                     "com.apple.print.PrintSettings.PMDuplexing"
#define kPMDuplexingKey                     CFSTR( kPMDuplexingStr )                    /* CFNumber - kCFNumberSInt32Type, Enum, kPMDuplexNone,  kPMDuplexNoTumble, kPMDuplexTumble, kPMSimplexTumble */

#define kPMColorSyncProfileIDStr            "com.apple.print.PrintSettings.PMColorSyncProfileID"
#define kPMColorSyncProfileIDKey            CFSTR( kPMColorSyncProfileIDStr )           /* CFNumber - kCFNumberSInt32Type, ID of profile to use. */

#define kPMPrimaryPaperFeedStr				"com.apple.print.PrintSettings.PMPrimaryPaperFeed"
#define kPMPrimaryPaperFeedKey				CFSTR( kPMPrimaryPaperFeedStr )				/* CFArray - main & option PPD key for input paper feed */

#define kPMSecondaryPaperFeedStr			"com.apple.print.PrintSettings.PMSecondaryPaperFeed"
#define kPMSecondaryPaperFeedKey			CFSTR( kPMSecondaryPaperFeedStr )			/* CFArray - main & option PPD key for input paper feed */

#define kPMPSErrorHandlerStr				"com.apple.print.PrintSettings.PMPSErrorHandler"
#define kPMPSErrorHandlerKey				CFSTR( kPMPSErrorHandlerStr )				/* CFNumber - kCFNumberSInt32Type  */

#define kPMPSTraySwitchStr					"com.apple.print.PrintSettings.PMPSTraySwitch"
#define kPMPSTraySwitchKey					CFSTR( kPMPSTraySwitchStr )					/* CFArray - main & option PPD key for tray switching */

#define kPMTotalBeginPagesStr		    	"com.apple.print.PrintSettings.PMTotalBeginPages"
#define kPMTotalBeginPagesKey		    	CFSTR( kPMTotalBeginPagesStr )			/* CFNumber the total number of times beginpage was called */

#define kPMTotalSidesImagedStr		    	"com.apple.print.PrintSettings.PMTotalSidesImaged"
#define kPMTotalSidesImagedKey		    	CFSTR( kPMTotalSidesImagedStr )			/* CFNumber the total number of sides that will printed. Does not take into account duplex and collation */

#define kPMFitToPageStr						"fit-to-page"
#define kPMFitToPageKey						CFSTR( kPMFitToPageStr )			/* CFString - If CFSTR("true"), orient, scale to fit and/or center each page as appropriate. Default value: CFSTR("false"). */
  
#define kPMUseOptionalPINStr	    		"com.apple.print.PrintSettings.PMUseOptionalPIN"
#define kPMUseOptionalPINKey		    	CFSTR( kPMUseOptionalPINStr )			/* CFBoolean - requires PIN to print */

#define kPMUseOptionalAccountIDStr	    	"com.apple.print.PrintSettings.PMUseOptionalAccountID"
#define kPMUseOptionalAccountIDKey		    CFSTR( kPMUseOptionalAccountIDStr )		/* CFBoolean - Uses optional AccountID for job */
	
/* Fax Related */
#define kPMFaxNumberStr						"phone"
#define kPMFaxNumberKey		    			CFSTR( kPMFaxNumberStr )			/* CFString - fax number to dial */

#define kPMFaxToStr							"faxTo"
#define kPMFaxToKey		    				CFSTR( kPMFaxToStr )				/* CFString - entire fax to line */

#define kPMFaxPrefixStr						"faxPrefix"
#define kPMFaxPrefixKey		    			CFSTR( kPMFaxPrefixStr )			/* CFString - fax prefix to dial */

#define kPMFaxSubjectStr					"faxSubject"
#define kPMFaxSubjectKey		    		CFSTR( kPMFaxSubjectStr )			/* CFString - fax subject linee*/

#define kPMFaxCoverSheetStr					"faxCoverSheet"
#define kPMFaxCoverSheetKey			   		CFSTR( kPMFaxCoverSheetStr )		/* CFString - fax cover sheet */

#define kPMFaxCoverSheetMessageStr			"faxCoverSheetMessage"
#define kPMFaxCoverSheetMessageKey	   		CFSTR( kPMFaxCoverSheetMessageStr )	/* CFString - fax cover sheet message*/

#define kPMFaxToneDialingStr				"faxToneDialing"
#define kPMFaxToneDialingKey		   		CFSTR( kPMFaxToneDialingStr )		/* CFString - fax use tone dialing */

#define kPMFaxUseSoundStr					"faxUseSound"
#define kPMFaxUseSoundKey		    		CFSTR( kPMFaxUseSoundStr )			/* CFString - fax use sound */

#define kPMFaxWaitForDialToneStr			"faxWaitForDialTone"
#define kPMFaxWaitForDialToneKey		    CFSTR( kPMFaxWaitForDialToneStr )	/* CFString - fax wait for dial tone */

#define kPMFaxToLabelStr					"faxToLabel"
#define kPMFaxToLabelKey					CFSTR( kPMFaxToLabelStr )			/* CFString - To: label */

#define kPMFaxFromLabelStr					"faxFromLabel"
#define kPMFaxFromLabelKey					CFSTR( kPMFaxFromLabelStr )			/* CFString - From: label */

#define kPMFaxDateLabelStr					"faxDateLabel"
#define kPMFaxDateLabelKey					CFSTR( kPMFaxDateLabelStr )			/* CFString - Date: label */

#define kPMFaxSubjectLabelStr				"faxSubjectLabel"
#define kPMFaxSubjectLabelKey				CFSTR( kPMFaxSubjectLabelStr )		/* CFString - Subject: label */

#define kPMFaxSheetsLabelStr				"faxSheetsLabel"
#define kPMFaxSheetsLabelKey				CFSTR( kPMFaxSheetsLabelStr )		/* CFString - Sheets to Follow: label */


/* Coverpage Related */
#define kPMCoverPageStr                     "com.apple.print.PrintSettings.PMCoverPage"
#define kPMCoverPageKey                     CFSTR( kPMCoverPageStr )                    /* CFNumber - kCFNumberSInt32Type, Enum, kPMCoverPageNone,  kPMCoverPageBefore, kPMCoverPageAfter */

/* The values for kPMCoverPageKey */

enum {
	kPMCoverPageNone = 1,	
	// Print a cover page before printing the document.
    	kPMCoverPageBefore = 2,
	// Print a cover page after printing the document.
	kPMCoverPageAfter = 3
};
/* If the kPMDuplexingKey is not in a print settings then kPMDuplexDefault should be assumed.
 */
#define	kPMCoverPageDefault	(kPMCoverPageNone)


#define kPMCoverPageSourceStr				"com.apple.print.PrintSettings.PMCoverPageSource"
#define kPMCoverPageSourceKey				CFSTR( kPMCoverPageSourceStr )				/* CFArray - main & option PPD key for cover page paper source */


#define kPMDestinationPrinterIDStr		"DestinationPrinterID"
#define kPMDestinationPrinterIDKey		CFSTR( kPMDestinationPrinterIDStr )	/* CFStringRef - the printer ID corresponding to the destination printer */

#define kPMInlineWorkflowStr				"inlineWorkflow"
#define kPMInlineWorkflowKey				CFSTR( kPMInlineWorkflowStr )	/* CFStringRef - the URL for the inline workflow item that will process this job */

#define kPMPageToPaperMappingTypeStr	    "com.apple.print.PageToPaperMappingType"
#define kPMPageToPaperMappingTypeKey	    CFSTR(kPMPageToPaperMappingTypeStr) /* a CFNumber - values from PMPageToPaperMappingType */

#define kPMPageToPaperMediaNameStr	    "com.apple.print.PageToPaperMappingMediaName" 
#define kPMPageToPaperMediaNameKey	    CFSTR(kPMPageToPaperMediaNameStr) /* a CFString - the untranslated media name for the destination sheet */

#define kPMPageToPaperMappingAllowScalingUpStr	    "com.apple.print.PageToPaperMappingAllowScalingUp"
#define kPMPageToPaperMappingAllowScalingUpKey	    CFSTR(kPMPageToPaperMappingAllowScalingUpStr) /* a CFBoolean - if true, allow scaling up to fit
												    destination sheet, otherwise do not scale
												    up if destination sheet is larger than formatting
												    sheet. Default value: false. */

/*
    The kPMCustomProfilePathKey key stores a CFString that corresponds to a custom profile setting for a given printer.
*/
#define kPMCustomProfilePathStr	"PMCustomProfilePath"
#define kPMCustomProfilePathKey	CFSTR( kPMCustomProfilePathStr )

/* Page to Paper Mapping Types */
typedef enum{
    kPMPageToPaperMappingNone = 1,
    kPMPageToPaperMappingScaleToFit,
}PMPageToPaperMappingType;



/* Possible values for the kPMColorMatchingModeKey*/
#define kPMVendorColorMatchingStr	    "AP_VendorColorMatching"
#define kPMVendorColorMatching		    CFSTR( kPMVendorColorMatchingStr )
#define kPMApplicationColorMatchingStr      "AP_ApplicationColorMatching"
#define kPMApplicationColorMatching	    CFSTR( kPMApplicationColorMatchingStr )

#define kPMColorMatchingModeStr		    "AP_ColorMatchingMode"
#define kPMColorMatchingModeKey		    CFSTR( kPMColorMatchingModeStr )   /* Value is CFStringRef - one of kPMColorSyncMatching (deprecated), 
										kPMVendorColorMatching, kPMApplicationColorMatching */


/* Begin: Use of these keys is discouraged. Use PMSessionSetDestination, PMSessionGetDestinationType, PMSessionCopyDestinationFormat, and PMSessionCopyDestinationLocation instead */
#define kPMDestinationTypeStr               "com.apple.print.PrintSettings.PMDestinationType"
#define kPMDestinationTypeKey               CFSTR( kPMDestinationTypeStr )              /* CFNumber, kCFNumberSInt32Type kPMDestinationPrinter kPMDestinationFile kPMDestinationFax, etc. */
#define kPMOutputFilenameStr                "com.apple.print.PrintSettings.PMOutputFilename"
#define kPMOutputFilenameKey                CFSTR( kPMOutputFilenameStr )               /* CFString - URL for the output filename. */
/* End: Use of these keys is discouraged. Use PMSessionSetDestination, PMSessionGetDestinationType, PMSessionCopyDestinationFormat, and PMSessionCopyDestinationLocation instead */

#ifdef __cplusplus
}
#endif

#endif		/* __PMPRINTSETTINGSKEYS__ */ 
