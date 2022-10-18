/*
     File:       PMPrintAETypes.h
 
     Contains:   Mac OS X Printing Manager AE definitions.
 
     Version:    Technology: Mac OS X
                 Release:    1.0
 
     Copyright (c) 2003,2008 by Apple Inc. All Rights Reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
*/

#ifndef __PMPrintAETypes__
#define __PMPrintAETypes__

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define	kPMPrintSettingsAEType			'pset'
#define kPMShowPrintDialogAEType		'pdlg'
#define kPMPrinterAEType				'trpr'

#define kPMCopiesAEProp					"copies"
#define kPMCopiesAEKey					'lwcp'
#define kPMCopieAEType					typeSInt32

#define kPMCollateAEProp				"collating"
#define kPMCollateAEKey					'lwcl'
#define kPMCollateAEType				typeBoolean

#define kPMFirstPageAEProp				"starting page"
#define kPMFirstPageAEKey				'lwfp'
#define kPMFirstPageAEType				typeSInt32

#define kPMLastPageAEProp				"ending page"
#define kPMLastPageAEKey				'lwlp'
#define kPMLastPageAEType				typeSInt32

#define kPMLayoutAcrossAEProp				"pages across"
#define kPMLayoutAcrossAEKey				'lwla'
#define kPMLayoutAcrossAEType				typeSInt32

#define kPMLayoutDownAEProp				"pages down"
#define kPMLayoutDownAEKey				'lwld'
#define kPMLayoutDownAEType				typeSInt32

#define kPMErrorHandlingAEProp				"error handling"
#define kPMErrorHandlingAEKey				'lweh'
#define kPMErrorHandlingAEType				typeEnumerated

#define kPMPrintTimeAEProp				"requested print time"
#define kPMPrintTimeAEKey				'lwqt'
#define kPMPrintTimeAEType				cLongDateTime

#define kPMFeatureAEProp				"printer features"
#define kPMFeatureAEKey					'lwpf'
#define kPMFeatureAEType				typeAEList

#define	kPMFaxNumberAEProp				"fax number"
#define kPMFaxNumberAEKey				'faxn'
#define kPMFaxNumberAEType				typeChar

#define kPMTargetPrinterAEProp			"target printer"
#define kPMTargetPrinterAEKey			'trpr'
#define kPMTargetPrinterAEType			typeChar

// Overrides all other settings 
#define kPMPDFWorkFlowAEProp		"PDF work flow"
#define kPMPDFWorkFlowAEKey			'wrkf'
#define kPMPDFWorkFlowAEType		typeUTF8Text

// Overrides all other settings except kPMPDFWorkFlowAEProp
// Can optionally target a specific printer's preset by setting kPMTargetPrinterAEProp
// Search order is as follows: custom per printer presets, general printer presets, global presets
#define kPMPresetAEProp				"preset"
#define kPMPresetAEKey				'prst'
#define kPMPresetAEType				typeUTF8Text


// Pass in where to save the file Ex. "/tmp/foo.pdf"
#define kPMSaveAsPDFAEProp			"save as PDF"
#define kPMSaveAsPDFAEKey			'spdf'
#define kPMSaveAsPDFAEType			typeFileURL

// Pass in where to save the file Ex. "/tmp/foo.ps"
#define kPMSaveAsPSAEProp			"save as PS"
#define kPMSaveAsPSAEKey			'sps '
#define kPMSaveAsPSAEType			typeFileURL

/*** Enumerations ***/

/* For kPMErrorHandlingAEType */
#define kPMErrorHandlingStandardEnum		'lwst'
#define kPMErrorHandlingDetailedEnum		'lwdt'

#ifdef __cplusplus
}
#endif

#endif		// __PRINTAETYPES__
