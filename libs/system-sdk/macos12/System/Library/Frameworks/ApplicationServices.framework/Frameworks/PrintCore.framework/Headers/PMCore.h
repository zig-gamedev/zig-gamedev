/*
     File:       PrintCore/PMCore.h
 
     Contains:   Carbon Printing Manager Interfaces.
 
     Copyright (c) 1998-2006,2008 Apple Inc. All Rights Reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PMCORE__
#define __PMCORE__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __COREGRAPHICS__
#include <CoreGraphics/CoreGraphics.h>
#endif

#ifndef __PMDEFINITIONS__
#include <PrintCore/PMDefinitions.h>
#endif

#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

CF_ASSUME_NONNULL_BEGIN

#ifdef __cplusplus
extern "C" {
#endif
	
#pragma mark
#pragma mark Retain/Release
#pragma mark

/*
 *  PMRetain()
 *  
 *  Summary:
 *    Increases a printing objects refcount by 1.
 *  
 *  Discussion:
 *    You should retain a printing object when you receive it from
 *    elsewhere (that is, you did not create or copy it) and you want
 *    it to persist. If you retain a printing object you are
 *    responsible for releasing it.
 *  
 *  Parameters:
 *    
 *    object:
 *      A Carbon Printing Manager object, such as a PMPrintSession
 *      object, a PMPageFormat object, a PMPrintSettings object, or
 *      PMPrinter object.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMRetain(PMObject __nullable object)							AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMRelease()
 *  
 *  Summary:
 *    Decreases a printing objects refcount by 1.
 *  
 *  Discussion:
 *    Your application should use the PMRelease function to release any
 *    printing objects it creates or retains. When an object’s
 *    reference count reaches 0, the object is deallocated. To
 *    terminate a printing session created with the function
 *    PMCreateSession, pass the associated PMPrintSession object to
 *    PMRelease. To release printing objects created with the functions
 *    PMCreatePageFormat and PMCreatePrintSettings, pass the associated
 *    PMPageFormat and PMPrintSettings objects to PMRelease. You can
 *    pass a NULL object but be aware a kPMInvalidParameter error will
 *    be returned.
 *  
 *  Parameters:
 *    
 *    object:
 *      A Carbon Printing Manager object, such as a PMPrintSession
 *      object, a PMPageFormat object, a PMPrintSettings object, or
 *      PMPrinter object.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMRelease(PMObject __nullable object)							AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#pragma mark
#pragma mark Session
#pragma mark

/*
 *  PMCreateSession()
 *  
 *  Summary:
 *    Creates and initializes a printing session object and creates a
 *    context for printing operations.
 *  
 *  Discussion:
 *    A session is created with a refcount of 1.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCreateSession(PMPrintSession __nullable * __nonnull printSession)					AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSessionError()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionError(PMPrintSession printSession)						AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSessionSetError()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionSetError(
  PMPrintSession   printSession,
  OSStatus         printError)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#pragma mark
#pragma mark Session: Printing Loop
#pragma mark

/*
 *  PMSessionBeginCGDocumentNoDialog()
 *  
 *  Summary:
 *    Begin a new print job for client drawing to a CoreGraphics context.
 *  
 *  Discussion:
 *    This is an updated version of the function
 *    PMSessionBeginDocumentNoDialog. The functionality is identical to
 *    PMSessionBeginDocumentNoDialog except that during a print job,
 *    the caller cannot obtain a Quickdraw grafPort for the printing
 *    context but can only obtain a Quartz graphics context
 *    (CGContextRef). This function should be used in conjunction with
 *    PMSessionGetCGGraphicsContext instead of
 *    PMSessionGetGraphicsContext.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionBeginCGDocumentNoDialog(
  PMPrintSession    printSession,
  PMPrintSettings   printSettings,
  PMPageFormat      pageFormat)									AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PMSessionEndDocumentNoDialog()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionEndDocumentNoDialog(PMPrintSession printSession)		AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 * SPECIAL AVAILABILITY note: This routine is available in ApplicationsServices.framework in
 * Mac OS X version 10.0 and later. On Mac OS X it is available to CFM applications through CarbonLib
 * starting with Mac OS X version 10.2 and later.
 *
 * On Mac OS 8/9 using CarbonLib, this routine returns kPMNotImplemented
 */
/*
 *  PMSessionBeginPageNoDialog()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionBeginPageNoDialog(
  PMPrintSession				printSession,
  PMPageFormat __nullable		pageFormat,
  const PMRect * __nullable		pageFrame)						AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 * SPECIAL AVAILABILITY note: This routine is available in ApplicationsServices.framework in
 * Mac OS X version 10.0 and later. On Mac OS X it is available to CFM applications through CarbonLib
 * starting with Mac OS X version 10.2 and later.
 *
 * On Mac OS 8/9 using CarbonLib, this routine returns kPMNotImplemented
 */
/*
 *  PMSessionEndPageNoDialog()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionEndPageNoDialog(PMPrintSession printSession)			AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSessionGetCGGraphicsContext()
 *  
 *  Summary:
 *    Return the CGContextRef for the current page in the printing
 *    session.
 *  
 *  Discussion:
 *    This function returns the CGContextRef for the printing session.
 *    This function must be called for each page. To use
 *    PMSessionGetCGGraphicsContext you MUST call
 *    PMSessionBeginCGDocument or PMSessionBeginCGDocumentNoDialog.
 *  
 *  Parameters:
 *    
 *    printSession:
 *      The session for the print job.
 *    
 *    context:
 *      A pointer to a caller supplied CGContextRef variable. If this
 *      function succeeds then *context will be filled in with the
 *      printing CGContextRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionGetCGGraphicsContext(
  PMPrintSession						printSession,
  CGContextRef __nullable * __nonnull	context)				AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

#pragma mark
#pragma mark Session: Other routines
#pragma mark -


/*
 *  PMSessionGetDestinationType()
 *  
 *  Summary:
 *    Hand back the destination type that will be used for a print job
 *    with the specified print settings and print session.
 *  
 *  Discussion:
 *    Currently there are five destination types:
 *    kPMDestinationPrinter, kPMDestinationFile, kPMDestinationFax and
 *    kPMDestinationPreview, and kPMDestinationProcessPDF.
 *  
 *  Parameters:
 *    
 *    printSession:
 *      The session to be used for a print job. The session holds the
 *      preview setting which can override the destination type in the
 *      print settings.
 *    
 *    printSettings:
 *      The print settings to be used for a print job. The print
 *      settings specify whether a job will be directed toward a
 *      printer or to file.
 *    
 *    destTypeP:
 *      A pointer to a caller supplied PMDestinationType variable. If
 *      this function succeeds then *'destTypeP' will be filled in with
 *      the destination type for a print job that used the specified
 *      session and print settings. If this function fails, then
 *      *'destType' will be set to kPMDestinationInvalid.
 *    
 *    SPECIAL_AVAILABILITY_NOTE:
 *      This routine is available in ApplicationsServices.framework in
 *      Mac OS X version 10.1 and later. On Mac OS X it is available to
 *      CFM applications through CarbonLib starting with Mac OS X
 *      version 10.2 and later.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionGetDestinationType(
  PMPrintSession       printSession,
  PMPrintSettings      printSettings,
  PMDestinationType *  destTypeP)								AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;
  
/*
 *  PMSessionCopyDestinationFormat()
 *  
 *  Summary:
 *    Hand back the destination output MIME type associated with the
 *    provided print session and print settings.
 *  
 *  Parameters:
 *    
 *    printSession:
 *      A currently open print session.
 *    
 *    printSettings:
 *      The print settings that are to be searched.
 *    
 *    destFormatP:
 *      A pointer to a caller allocated CFStringRef variable. If this
 *      routine returns noErr then *'destFormatP' will either be a copy
 *      of a CFStringRef specifying the output format for the print
 *      job, or NULL indicating that the default output format will be
 *      used. If this function return an error, then *'destFormatP'
 *      will be set to NULL.
 *    
 *    SPECIAL_AVAILABILITY_NOTE:
 *      This routine is available in ApplicationsServices.framework in
 *      Mac OS X version 10.1 and later. On Mac OS X it is available to
 *      CFM applications through CarbonLib starting with Mac OS X
 *      version 10.2 and later.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionCopyDestinationFormat(
  PMPrintSession						printSession,
  PMPrintSettings						printSettings,
  CFStringRef __nullable * __nonnull	destFormatP)			AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*
 *  PMSessionCopyDestinationLocation()
 *  
 *  Summary:
 *    Hand back the URL destination location given a print session and
 *    print settings.
 *  
 *  Discussion:
 *    Some destination types support a destination location which
 *    further defines where the output from a print job should be sent.
 *    The kPMDestinationFile destination type, for example, will use a
 *    file URL to determine where a new file should be created.
 *  
 *  Parameters:
 *    
 *    printSession:
 *      A currently open print session.
 *    
 *    printSettings:
 *      The print settings that are to be searched.
 *    
 *    destLocationP:
 *      A pointer to a caller allocated CFURLRef variable. If this
 *      routine returns noErr then *'outputFileP' will either be NULL
 *      indicating that the job is using the default destination
 *      location for the current destination type or a copy of a
 *      CFURLRef will be placed in *'destLocationP'. If this function
 *      returns an error then 'destLocationP' will be set to NULL.
 *    
 *    SPECIAL_AVAILABILITY_NOTE:
 *      This routine is available in ApplicationsServices.framework in
 *      Mac OS X version 10.1 and later. On Mac OS X it is available to
 *      CFM applications through CarbonLib starting with Mac OS X
 *      version 10.2 and later.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionCopyDestinationLocation(
  PMPrintSession					printSession,
  PMPrintSettings					printSettings,
  CFURLRef __nullable * __nonnull	destLocationP)				AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*
 *  PMSessionSetDestination()
 *  
 *  Summary:
 *    Alter a print session and print settings so that an associated
 *    print job is sent to the provided destination type in the,
 *    optional, MIME document format.
 *  
 *  Discussion:
 *    This function is most useful when an application would like to
 *    write its print output to disk without requiring user
 *    interaction. The list of MIME types that can be sent to the
 *    provided destination can be obtained from
 *    PMSessionCopyOutputFormatList and one of these passed to this
 *    function.
 *  
 *  Parameters:
 *    
 *    printSession:
 *      The session to be used for a print job. The session holds the
 *      preview setting which can override the destination type in the
 *      print settings.
 *    
 *    printSettings:
 *      The print settings to be used for a print job. The print
 *      settings specify whether a job will be directed toward a
 *      printer or to file. It also holds the requested MIME output
 *      type.
 *    
 *    destType:
 *      The destination type for a print job associated with the
 *      provided print session and print settings. Fax is currently not
 *      supported, but kPMDestinationPrinter, kPMDestinationFile, and
 *      kPMDestinationPreview can be set.
 *    
 *    destFormat:
 *      The MIME type to be generated for the provided destination
 *      type. This parameter can be NULL, in which case the default format
 *      for the requested destination type is used. To obtain a list of
 *      valid formats for a given destination type, use the function
 *      PMSessionCopyOutputFormatList.
 *    
 *    destLocation:
 *      Some destination types support a destination location. The
 *      clearest example is the kPMDestinationFile destination type
 *      which allows a caller to also supply a file URL specifying
 *      where the output file is to be created.
 *    
 *    SPECIAL_AVAILABILITY_NOTE:
 *      This routine is available in ApplicationsServices.framework in
 *      Mac OS X version 10.1 and later. On Mac OS X it is available to
 *      CFM applications through CarbonLib starting with Mac OS X
 *      version 10.2 and later.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionSetDestination(
  PMPrintSession			printSession,
  PMPrintSettings			printSettings,
  PMDestinationType			destType,
  CFStringRef __nullable	destFormat,
  CFURLRef __nullable		destLocation)						AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;
  
/*
 *  PMSessionCopyOutputFormatList()
 *  
 *  Summary:
 *    Hands back an an array of MIME types describing the possible
 *    output formats for the printer module associated with the current
 *    printer.
 *  
 *  Parameters:
 *    
 *    printSession:
 *      This session's current printer's printer module will be queried
 *      for its supported output MIME types.
 *    
 *    destType:
 *      A print job can have one of several possible destination types.
 *      The list of valid output formats is dependent upon the
 *      destination type. This parameter specifies destination type of
 *      interest when retrieving the output formats list.
 *    
 *    documentFormatP:
 *      A pointer to a caller's CFArrayRef variable. If this routine
 *      completes successfully, then *'documentFormatP' will be set to
 *      a CFArrayRef containing CFStringRefs. Each CFStringRef in the
 *      array is a MIME type specifying a type of output that can be
 *      generated by the printer module associated with the current
 *      printer.
 *    
 *    SPECIAL_AVAILABILITY_NOTE:
 *      This routine is available in ApplicationsServices.framework in
 *      Mac OS X version 10.1 and later. On Mac OS X it is available to
 *      CFM applications through CarbonLib starting with Mac OS X
 *      version 10.2 and later. On Mac OS 8/9 using CarbonLib, this
 *      routine returns kPMNotImplemented
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionCopyOutputFormatList(
  PMPrintSession					printSession,
  PMDestinationType					destType,
  CFArrayRef __nullable * __nonnull	documentFormatP)			AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;



/*
 *  PMSessionCreatePageFormatList()
 *  
 *  Summary:
 *    Hand back a list of page format instances. Each page format
 *    instance describes a paper size available on the specified
 *    printer.
 *  
 *  Parameters:
 *    
 *    printSession:
 *      A currently valid print session.
 *    
 *    printer:
 *      The printer whose page size list should be enumerated. 
 *    
 *    pageFormatList:
 *      If this function is successful then noErr will be returned and
 *      *'pageFormatList' will be set to a newly created CFArray. Each
 *      element in the array will be a PMPageFormat describing an
 *      available paper size for the specified printer. If this
 *      function fails then a non-zero error code will be returned and
 *      *'pageFormatList' will be set to NULL.
 *    
 *  Discussion:
 *	If you want to create the page format list for the session's current printer,
 *      pass the PMPrinter object returned by PMSessionGetCurrentPrinter() as the
 *	printer parameter. 
 *
 *    SPECIAL_AVAILABILITY_NOTE:
 *      This routine is available in ApplicationsServices.framework in
 *      Mac OS X version 10.1 and later. On Mac OS X it is available to
 *      CFM applications through CarbonLib starting with Mac OS X
 *      version 10.2 and later. On Mac OS 8/9 using CarbonLib, this
 *      routine returns kPMNotImplemented
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionCreatePageFormatList(
  PMPrintSession					printSession,
  PMPrinter __nullable				printer,
  CFArrayRef __nullable * __nonnull	pageFormatList)				AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*
 *  PMSessionCreatePrinterList()
 *
 *  See also: PMServerCreatePrinterList.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionCreatePrinterList(
  PMPrintSession					printSession,
  CFArrayRef __nullable * __nonnull	printerList,
  CFIndex * __nullable				currentIndex,
  PMPrinter __nullable * __nullable	currentPrinter)				AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;

/*
 *  PMSessionGetCurrentPrinter()
 *  
 *  Summary:
 *    Hand back the session's current printer.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionGetCurrentPrinter(
  PMPrintSession					printSession,
  PMPrinter __nullable * __nonnull	currentPrinter)				AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMSessionSetCurrentPMPrinter()
 *  
 *  Summary:
 *    Set the session's current printer to a specified PMPrinter.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionSetCurrentPMPrinter(
  PMPrintSession   session,
  PMPrinter        printer)										AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/*
 *  PMSessionGetDataFromSession()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionGetDataFromSession(
  PMPrintSession					printSession,
  CFStringRef						key,
  CFTypeRef __nullable * __nonnull	data)						AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMSessionSetDataInSession()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionSetDataInSession(
  PMPrintSession   printSession,
  CFStringRef      key,
  CFTypeRef        data)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#pragma mark -
#pragma mark PageFormat
#pragma mark

/*
 *  PMCreatePageFormat()
 *  
 *  Summary:
 *    Allocates memory for a new PMPageFormat object in your
 *    application’s memory space.
 *  
 *  Discussion:
 *    A pageformat is created with a refcount of 1.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCreatePageFormat(PMPageFormat __nullable * __nonnull pageFormat)	AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMSessionDefaultPageFormat()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionDefaultPageFormat(
  PMPrintSession   printSession,
  PMPageFormat     pageFormat)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMSessionValidatePageFormat()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionValidatePageFormat(
  PMPrintSession		printSession,
  PMPageFormat			pageFormat,
  Boolean * __nullable	changed)								AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMCopyPageFormat()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCopyPageFormat(
  PMPageFormat   formatSrc,
  PMPageFormat   formatDest)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMCreatePageFormatWithPMPaper()
 *  
 *  Summary:
 *    Create a pageformat with a specific paper.
 *  
 *  Parameters:
 *    
 *    pageFormat:
 *      On return, will contain the pageformat which was created
 *    
 *    paper:
 *      The paper that will be associate with the pageformat
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCreatePageFormatWithPMPaper(
  PMPageFormat __nullable * __nonnull	pageFormat,
  PMPaper								paper)					AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/*
 *  PMPageFormatCreateDataRepresentation()
 *  
 *  Summary:
 *    Returns a data representation of a PMPageFormat object as a CFDataRef.
 *    The caller is responsible for releasing the CFData object returned.
 *  
 *  Discussion:
 *    This function is similar to PMFlattenPageFormatToCFData but allows you
 *    to specify the format of the data that is returned. Use  
 *    PMPageFormatCreateWithDataRepresentation to create a PMPageFormat from 
 *    a CFDataRef created by this call.
 *  
 *    See PMDataFormat for information about the available data formats.  
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPageFormatCreateDataRepresentation(
  PMPageFormat						pageFormat,
  CFDataRef __nullable * __nonnull	data,
  PMDataFormat						format)						AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  PMPageFormatCreateWithDataRepresentation()
 *  
 *  Summary:
 *    Creates a PMPageFormat object from a data representation created with
 *	  PMPageFormatCreateDataRepresentation. The caller is responsible
 *    for releasing the PMPrintSettings object returned with PMRelease.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPageFormatCreateWithDataRepresentation(
	CFDataRef data, 
	PMPageFormat __nullable * __nonnull  pageFormat)				AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



#pragma mark -
/*
 *  PMGetAdjustedPageRect()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetAdjustedPageRect(
  PMPageFormat   pageFormat,
  PMRect *       pageRect)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetAdjustedPaperRect()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetAdjustedPaperRect(
  PMPageFormat   pageFormat,
  PMRect *       paperRect)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetOrientation()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetOrientation(
  PMPageFormat     pageFormat,
  PMOrientation *  orientation)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetPageFormatExtendedData()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetPageFormatExtendedData(
  PMPageFormat				pageFormat,
  OSType					dataID,
  UInt32 *  __nullable		size,
  void *  __nullable		extendedData)						AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMPageFormatGetPrinterID()
 *  
 *  Summary:
 *		Obtains the formatting printer for the pageformat.
 *
 *  Discussion:
 *		Will either return the formatting printer for the pageformat
 *		or will return NULL if the pageformat doesn't have that information.
 * 
 *  Parameters:
 *    
 *    pageFormat:
 *      The pageformat to obtain the information from.
 *
 *	  printerID:
 *		Where to store the name of the printer
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern  OSStatus
PMPageFormatGetPrinterID( 
 PMPageFormat						pageFormat,
 CFStringRef __nullable * __nonnull	printerID )					AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  PMGetScale()
 *  
 *  Summary:
 *    Obtains the scaling factor currently applied to the page and
 *    paper rectangles.
 *  
 *  Discussion:
 *    A value of 100.0 means 100% (no scaling). 50.0 means 50% scaling
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetScale(
  PMPageFormat   pageFormat,
  double *       scale)											AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetUnadjustedPageRect()
 *  
 *  Summary:
 *    Obtains the size of the imageable area in points, unaffected by
 *    orientation, resolution, or scaling.
 *  
 *  Discussion:
 *    This is the imageable area of the page without regard to
 *    resolution, orientation or scaling. Dimensions are returned as a 72dpi
 *    values.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetUnadjustedPageRect(
  PMPageFormat   pageFormat,
  PMRect *       pageRect)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetUnadjustedPaperRect()
 *  
 *  Summary:
 *    Obtains a rectangle that specifies the size of the paper in
 *    points, unaffected by rotation, resolution, or scaling.
 *  
 *  Discussion:
 *    This is the physical size of the paper without regard to
 *    resolution, orientation or scaling. Dimensions are returned as a 72dpi
 *    values.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetUnadjustedPaperRect(
  PMPageFormat   pageFormat,
  PMRect *       paperRect)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#pragma mark -
/************************/
/* PMSetxxx calls only save the value inside the printing object. They make no assumption on the */
/* validity of the value. This should be done using PMValidatePageFormat/PMSessionValidatePageFormat */
/* Any dependent settings are also updated during a validate call. */
/* For example: */
/* PMGetAdjustedPaperRect - returns a rect of a certain size */
/* PMSetScale( aPageFormat, 500.0 )  */
/* PMGetAdjustedPaperRect - returns the SAME rect as the first call  */
/**/
/* PMGetAdjustedPaperRect - returns a rect of a certain size */
/* PMSetScale( aPageFormat, 500.0 ) */
/* PMValidatePageFormat or PMSessionValidatePageFormat */
/* PMGetAdjustedPaperRect - returns a rect thats scaled 500% from the first call */
/************************/

/*
 *  PMSetOrientation()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetOrientation(
  PMPageFormat    pageFormat,
  PMOrientation   orientation,
  Boolean         lock)											AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSetPageFormatExtendedData()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetPageFormatExtendedData(
  PMPageFormat   pageFormat,
  OSType         dataID,
  UInt32         size,
  void *         extendedData)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSetScale()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetScale(
  PMPageFormat   pageFormat,
  double         scale)											AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#pragma mark -
#pragma mark PrintSettings
#pragma mark

/*
 *  PMCreatePrintSettings()
 *  
 *  Summary:
 *    Allocates memory for a new PMPrintSettings object in your
 *    application’s memory space.
 *  
 *  Discussion:
 *    A printSettings is created with a refcount of 1.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCreatePrintSettings(
	PMPrintSettings __nullable * __nonnull printSettings)		AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMSessionDefaultPrintSettings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionDefaultPrintSettings(
  PMPrintSession    printSession,
  PMPrintSettings   printSettings)								AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMSessionValidatePrintSettings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSessionValidatePrintSettings(
  PMPrintSession		printSession,
  PMPrintSettings		printSettings,
  Boolean * __nullable	changed)								AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMCopyPrintSettings()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCopyPrintSettings(
  PMPrintSettings   settingSrc,
  PMPrintSettings   settingDest)								AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  PMPrintSettingsCreateDataRepresentation()
 *  
 *  Summary:
 *    Returns a data representation of a PMPrintSettings object as a CFDataRef.
 *    The caller is responsible for releasing the CFData object returned.
 *  
 *  Discussion:
 *    This function is similar to PMFlattenPrintSettingsToCFData but allows you
 *    to specify the format of the data that is returned. Use  
 *    PMPrintSettingsCreateWithDataRepresentation to create a PMPrintSettings from 
 *    a CFDataRef created by this call.
 *
 *    See PMDataFormat for information about the available data formats.  
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsCreateDataRepresentation(
  PMPrintSettings					printSettings,
  CFDataRef __nullable * __nonnull	data,
  PMDataFormat						format)						AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  PMPrintSettingsCreateWithDataRepresentation()
 *  
 *  Summary:
 *    Creates a PMPrintSettings object from a data representation created with
 *	  PMPrintSettingsCreateDataRepresentation. The caller is responsible
 *    for releasing the PMPrintSettings object returned with PMRelease.
 *    
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsCreateWithDataRepresentation(
	CFDataRef								data,
	PMPrintSettings __nullable * __nonnull	printSettings)		AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


  
#pragma mark -
/*
 *  PMGetCollate()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetCollate(
  PMPrintSettings   printSettings,
  Boolean *         collate)									AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMGetCopies()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetCopies(
  PMPrintSettings   printSettings,
  UInt32 *          copies)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetDuplex()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetDuplex(
  PMPrintSettings   printSettings,
  PMDuplexMode *    duplexSetting)								AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PMGetFirstPage()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetFirstPage(
  PMPrintSettings   printSettings,
  UInt32 *          first)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetLastPage()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetLastPage(
  PMPrintSettings   printSettings,
  UInt32 *          last)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMGetPageRange()
 *  
 *  Summary:
 *    Obtains the valid range of pages that can be printed.
 *  
 *  Discussion:
 *    The default page range is 1 - (all pages). The page range is
 *    something that is set by the application. It is NOT the first and
 *    last page to print. It serves as limits for setting the first and
 *    last page.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetPageRange(
  PMPrintSettings	printSettings,
  UInt32 *			minPage,
  UInt32 *			maxPage)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMPrintSettingsGetJobName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsGetJobName(
  PMPrintSettings						printSettings,
  CFStringRef __nullable * __nonnull	name)					AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*
 *  PMPrintSettingsGetValue()
 *  
 *  Parameters:
 *    
 *    printSettings:
 *      The printsettings to retrieve the value from
 *    
 *    key:
 *      The key to look for
 *    
 *    value:
 *      The return value. Its either the value for the key or NULL
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsGetValue(
  PMPrintSettings					printSettings,
  CFStringRef						key,
  CFTypeRef __nullable * __nonnull	value)						AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

#pragma mark -

/*
 *  PMSetCollate()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetCollate(
  PMPrintSettings   printSettings,
  Boolean           collate)									AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMSetCopies()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetCopies(
  PMPrintSettings   printSettings,
  UInt32            copies,
  Boolean           lock)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSetDuplex()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetDuplex(
  PMPrintSettings   printSettings,
  PMDuplexMode      duplexSetting)								AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PMSetFirstPage()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetFirstPage(
  PMPrintSettings   printSettings,
  UInt32            first,
  Boolean           lock)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSetLastPage()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetLastPage(
  PMPrintSettings   printSettings,
  UInt32            last,
  Boolean           lock)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMSetPageRange()
 *  
 *  Summary:
 *    Sets the valid range of pages that can be printed.
 *  
 *  Discussion:
 *    The first and last page are immediately clipped to the new range.
 *    You may pass kPMPrintAllPages for the maxPage value to specified
 *    that all pages are available for printing.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMSetPageRange(
  PMPrintSettings   printSettings,
  UInt32            minPage,
  UInt32            maxPage)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMPrintSettingsSetJobName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsSetJobName(
  PMPrintSettings   printSettings,
  CFStringRef       name)										AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PMPrintSettingsSetValue()
 *  
 *  Parameters:
 *    
 *    printSettings:
 *      The printsettings in which to set the value
 *    
 *    key:
 *      The key to store the value in
 *    
 *    value:
 *      The value to store in the key. If NULL, any existing setting item with
 *      the specified key is removed.
 *    
 *    locked:
 *      A boolean value indicating whether the item being set should be
 *      locked. It is strongly recommended to pass false.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsSetValue(
  PMPrintSettings		printSettings,
  CFStringRef			key,
  CFTypeRef __nullable	value,
  Boolean				locked)									AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;
  
  
/*
 *  PMPrintSettingsCopyAsDictionary()
 *  
 *  Parameters:
 *    
 *    printSettings:
 *      Represent these print settings as a dictionary.
 *    
 *    settingsDictionary:
 *      On exit, if successful *'settingsDictionary' will contain a reference to
 *      a CFDictionary describing the print settings. The caller is
 *      responsible for releasing this reference. If this call returns
 *      an error, then *'settingsDictionary' will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus
PMPrintSettingsCopyAsDictionary(
  PMPrintSettings						printSettings,
  CFDictionaryRef __nullable * __nonnull	settingsDictionary)		AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*
 *  PMPrintSettingsCopyKeys()
 *  
 *  Parameters:
 *    
 *    printSettings:
 *      Return the keys for items in this print settings.
 *    
 *    settingsKeys:
 *      On exit, if successful *'settingsKeys' will contain a reference to
 *      a CFArray describing the item keys in the print settings. Each of these
 *		keys may be passed to PMPrintSettingsGetValue() to obtain a value.
 *		The caller is responsible for releasing this reference. If this call returns
 *      an error, then *'settingsKeys' will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus
PMPrintSettingsCopyKeys(
	PMPrintSettings						printSettings,
	CFArrayRef __nullable * __nonnull	settingsKeys)			AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



#pragma mark -
#pragma mark Printer
#pragma mark


/*!
 *
 * PMCreateGenericPrinter
 *  Summary:
 *	Creates a generic PMPrinter
 *
 *  Parameters:
 *		printer:
 *		On return *printer contains the generic priner
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus
PMCreateGenericPrinter(
	PMPrinter __nullable * __nonnull	printer)					AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*
 *  PMServerCreatePrinterList()
 *  
 *  Summary:
 *    Hand back an array of PMPrinter objects that represents the list of printers
 *    known to the specified print server.
 *
 *  Parameters:
 *    
 *    server:
 *      The print server for which you want to obtain the list of printers.
 *      Use kPMServerLocal for the local print server.
 *    
 *    printerList:
 *      On return, *printerList contains the array of PMPrinter objects returned. The
 *	caller is responsible for releasing this array. On error, *printerList is NULL.
 *    
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMServerCreatePrinterList(
  PMServer __nullable				server,
  CFArrayRef __nullable * __nonnull	printerList)				AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMServerLaunchPrinterBrowser()
 *  
 *  Summary:
 *    Launch the printer browser, browsing the printers available for
 *    the specified print server.
 *
 *  Parameters:
 *    
 *    server:
 *      The print server whose available printers the browser should browse.
 *      Pass kPMServerLocal for the local print server.
 *    
 *    options:
 *      A CFDictionaryRef specifying how the browser should be presented. 
 *      Passing NULL presents the browser in the default fashion. 
 *
 *  Discussion:	The local print server allows its printers to be browsed but
 *		not all print servers do. Passing in a server whose printers cannot be
 *      browsed returns the error code kPMInvalidParameter.
 *    
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMServerLaunchPrinterBrowser(
  PMServer __nullable			server,
  CFDictionaryRef __nullable	options)						AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*
 *  PMPrinterCreateFromPrinterID()
 *  
 *  Summary:
 *    Create a PMPrinter instance from the supplied printerID.
 *  
 *  Discussion:
 *    Returns a PMPrinter instance for the printer whose ID is
 *    printerID. The caller is responsible for releasing this instance
 *    with PMRelease. The PMPrinter instance returned will be NULL if
 *    there is no printer available which corresponds to the supplied
 *    printerID.
 *  
 *  Parameters:
 *    
 *    printerID:
 *      The printerID for the printer for which you want the PMPrinter.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern PMPrinter __nullable
PMPrinterCreateFromPrinterID(CFStringRef printerID)				AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PMPrinterCopyDescriptionURL()
 *  
 *  Summary:
 *    Hand back a URL to the printer's PostScript Printer Description (PPD) file in fileURL.
 *  
 *  Parameters:
 *    
 *    printer:
 *      The printer whose PPD will be returned.
 *    
 *    descriptionType:
 *      The type of description desired. Only kPMPPDDescriptionType is currently supported.
 *    
 *    fileURL:
 *      A pointer to storage for a CFURL reference that will be returned. The caller is responsible
 *      for releasing the reference returned. If this call returns an error, then *fileURL will
 *      be set to NULL.
 *
 *  Discussion:
 *    Only descriptionType of kPMPPDDescriptionType is supported, otherwise kPMInvalidParameter
 *    is returned.
 *
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterCopyDescriptionURL(
  PMPrinter							printer,
  CFStringRef						descriptionType,
  CFURLRef __nullable * __nonnull	fileURL)					AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PMPrinterCopyDeviceURI()
 *  
 *  Summary:
 *    Hand back the URI of the printer's device.
 *  
 *  Parameters:
 *    
 *    printer:
 *      The printer whose device URI is to be retrieved.
 *    
 *    deviceURI:
 *      On exit, if successful *'deviceURI' will contain a reference to
 *      a CFURL describing the printer's device. The caller is
 *      responsible for releasing this reference. If this call returns
 *      an error, then *'deviceURI' will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterCopyDeviceURI(
  PMPrinter							printer,
  CFURLRef __nullable * __nonnull	deviceURI)					AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  PMPrinterCopyHostName()
 *  
 *  Summary:
 *    Hand back the host name of the print server hosting the printer's print queue.
 *
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterCopyHostName(
  PMPrinter								printer,
  CFStringRef __nullable * __nonnull	hostNameP)					AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrinterCopyPresets()
 *  
 *  Summary:
 *    Provides a list of print settings presets for the specified
 *    printer.
 *  
 *  Discussion:
 *    A printer may have associated with it a list of preset settings.
 *    Each setting is optimized for a particular printing situation.
 *    This function returns all of the presets for a given printer. To
 *    obtain more information about a particular preset see
 *    PMPresetGetAttributes().
 *  
 *  Parameters:
 *    
 *    printer:
 *      Obtain the presets for this printer.
 *    
 *    presetList:
 *      On exit, *'presetList' is set to reference an array of presets.
 *      The caller must call CFRelease when it no longer needs the
 *      array. Each element of the array is a PMPreset. If this
 *      function fails, returning a non-zero error code, then
 *      *'presetList' will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterCopyPresets(
  PMPrinter							printer,
  CFArrayRef __nullable * __nonnull	presetList)					AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrinterGetCommInfo()
 *  
 *  Summary:
 *    Provides information about the comm channel characteristics for
 *    the printer.
 *  
 *  Discussion:
 *    This function is typically relevant only to PostScript capable
 *    printers. All PostScript printers, regardless of what
 *    communications channel is used to send data to them, support data
 *    in the range 0x20 - 0x7F. Many comm channels can support data
 *    outside this range. The Boolean returned in *supportsTransparentP
 *    indicates whether the comm channel to this printer supports bytes
 *    in the range 0x0 to 0x1F. The Boolean returned in
 *    *supportsEightBitP indicates whether the comm channel to this
 *    printer supports bytes with the high bit set, i.e. bytes in the
 *    range 0x80 - 0xFF.
 *  
 *  Parameters:
 *    
 *    printer:
 *      Obtain the comm information for this printer.
 *    
 *    supportsControlCharRangeP:
 *      Storage for the returned Boolean indicating whether the comm
 *      channel to this printer can accept data bytes in the range
 *      0x0 - 0x1F
 *    
 *    supportsEightBitP:
 *      Storage for the returned Boolean indicating whether the comm
 *      channel to this printer can accept data bytes in the range 
 *      0x80 - 0xFF
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetCommInfo(
  PMPrinter				printer,
  Boolean * __nullable	supportsControlCharRangeP,
  Boolean * __nullable	supportsEightBitP)						AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrinterGetID()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern CFStringRef __nullable
PMPrinterGetID(PMPrinter printer)								AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMPrinterGetLocation()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern CFStringRef __nullable
PMPrinterGetLocation(PMPrinter printer)							AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMPrinterGetDriverCreator()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetDriverCreator(
  PMPrinter   printer,
  OSType *    creator)											AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMPrinterGetDriverReleaseInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetDriverReleaseInfo(
  PMPrinter   printer,
  VersRec *   release) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER CF_SWIFT_UNAVAILABLE("No replacement available.");


/*
 *  PMPrinterGetPrinterResolutionCount()
 *  
 *  Summary:
 *    Provides the number of available hardware resolution settings a printer supports. 
 *  
 *  Parameters:
 *    
 *    printer:
 *      Obtain the resolution count for this printer.
 *    
 *    countP:
 *      Storage for the returned count indicating the number of available hardware
 *      resolution settings for the specified printer.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetPrinterResolutionCount(
  PMPrinter   printer,
  UInt32 *    countP)											AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  PMPrinterGetIndexedPrinterResolution()
 *  
 *  Summary:
 *    Together with PMPrinterGetPrinterResolutionCount, allows iterating over the
 *    hardware resolution settings a printer supports. 
 *  
 *  Parameters:
 *    
 *    printer:
 *      The printer of interest.
 *    
 *    index:
 *      The index of the resolution to return. The minimum value for index is 1 and
 *      the maximum value of index is the count returned by PMPrinterGetPrinterResolutionCount.
 *    
 *    resolutionP:
 *      Storage for the returned PMResolution data.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetIndexedPrinterResolution(
  PMPrinter       printer,
  UInt32          index,
  PMResolution *  resolutionP)									AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  PMPrinterGetOutputResolution()
 *  
 *  Summary:
 *    Obtain the printer hardware output resolution for the supplied
 *    printer and print settings. 
 *  
 *  Discussion:
 *    Some printers allow programmatic control of their hardware output
 *    resolution on a print job basis. The hardware resolution is determined
 *    by the combination of printer and print settings used for the print job.
 *    PMPrinterGetOutputResolution returns the best guess as to what printer
 *    resolution setting will be used for the destination print job. If the
 *    resolution setting cannot be reliably determined this function returns
 *    kPMKeyNotFound. 
 *  
 *    Most applications do not need to use this function since they draw the same
 *    content regardless of the destination device. For those few applications that
 *    do adjust their drawing based on the output device, they should only do so
 *    when the print job destination is kPMDestinationPrinter or kPMDestinationFax.
 *  
 *    This call should be used after displaying the print dialog to the user so
 *    that it correctly reflects settings changes performed prior to printing.
 *  
 *  Parameters:
 *    
 *    printer:
 *      The destination printer.
 *    
 *    printSettings:
 *      The print settings from which to obtain the printer hardware output resolution.
 *    
 *    resolutionP:
 *      Storage for the returned PMResolution data.
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
OSStatus PMPrinterGetOutputResolution(
  PMPrinter printer,  
  PMPrintSettings printSettings, 
  PMResolution *resolutionP)									AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*
 *  PMPrinterSetOutputResolution()
 *  
 *  Summary:
 *    Set the printer hardware output resolution in the print settings
 *    for the supplied printer. 
 *  
 *  Discussion:
 *    Some printers allow programmatic control of their hardware output
 *    resolution on a print job basis. The hardware resolution is determined
 *    by the combination of printer and print settings used for the print job.
 *    PMPrinterSetOutputResolution configures the print settings to the closest
 *    resolution setting that can be used for the destination print job. Note
 *    that not all printers allow control of their resolution setting.
 *
 *    This function is rarely used since most applications do not set the output 
 *    resolution but instead use the setting supplied by the user in the print dialog.
 *  
 *  Parameters:
 *    
 *    printer:
 *      The destination printer.
 *    
 *    printSettings:
 *      The print settings in which to set the hardware resolution.
 *    
 *    resolutionP:
 *      A pointer to the PMResolution to use to set the hardware output resolution.
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
OSStatus PMPrinterSetOutputResolution(
  PMPrinter printer, 
  PMPrintSettings printSettings, 
  const PMResolution *resolutionP)								AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  PMPrinterGetLanguageInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetLanguageInfo(
  PMPrinter         printer,
  PMLanguageInfo *  info)										AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER CF_SWIFT_UNAVAILABLE("No replacement available.");


/*
 *  PMPrinterGetMakeAndModelName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetMakeAndModelName(
  PMPrinter								printer,
  CFStringRef __nullable * __nonnull	makeAndModel)			AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMPrinterGetMimeTypes()
 *  
 *  Summary:
 *    Return the array of mime type supported by the printer for a
 *    given set of print settings.
 *  
 *  Parameters:
 *    
 *    printer:
 *      The printer.
 *    
 *    settings:
 *      The print settings for the print job. The part of the print
 *      settings that effects the available mime type is the
 *      destination. This parameter can be NULL.
 *    
 *    mimeTypes:
 *      If this function returns without error then *'mimeTypes' is
 *      filled in with a reference to an array of CFStrings. Each
 *      CFString names a mime type supported by the printer with the
 *      specified print settings. The caller must not release this
 *      reference without first doing a retain. If this function
 *      returns an error then 'mimeTypes' will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetMimeTypes(
  PMPrinter							printer,
  PMPrintSettings __nullable		settings,
  CFArrayRef __nullable * __nonnull	mimeTypes)					AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrinterGetName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern CFStringRef __nullable
PMPrinterGetName(PMPrinter printer)								AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMPrinterGetPaperList()
 *  
 *  Summary:
 *    Returns the list of papers available for a given printer.
 *  
 *  Parameters:
 *    
 *    printer:
 *      Obtain the paper list for this printer.
 *    
 *    paperList:
 *      If successful noErr is returned and *paperList is a CFArray of
 *      PMPapers representing the list of papers available for the
 *      printer.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetPaperList(
  PMPrinter							printer,
  CFArrayRef __nullable * __nonnull	paperList)					AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrinterGetState()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterGetState(
  PMPrinter         printer,
  PMPrinterState *  state)										AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMPrinterIsDefault()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern Boolean 
PMPrinterIsDefault(PMPrinter printer)							AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMPrinterIsFavorite()
 *  
 *  Summary:
 *    Return true if the printer is in the user's favorite printer list.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern Boolean 
PMPrinterIsFavorite(PMPrinter printer)							AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  PMPrinterIsPostScriptCapable()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern Boolean 
PMPrinterIsPostScriptCapable(PMPrinter printer)					AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;

/*!
 * @function	PMPrinterIsPostScriptPrinter
 * @abstract	Set *isPSPrinter true if the printer is a PostScript printer.
 *
 * @discussion	A PostScript printer is one whose driver takes PostScript directly.
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus
PMPrinterIsPostScriptPrinter(PMPrinter printer, Boolean *isPSPrinter)	AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  PMPrinterIsRemote()
 *  
 *  Summary:
 *    Hand back a boolean indicating whether the printer is hosted by remote print server.
 *  
 *  Discussion:
 *    If on return *isRemoteP is true, the print queue represents a printer hosted and
 *    managed by a remote print server.
 *
 *    If on return *isRemoteP is false, the print queue represents a directly connected
 *    printer, a network printer, or a remote printer that is locally managed. Consult
 *    the queue's device URI to determine the type of connection that is used to communicate
 *    with the printer.
 *
 *    Whether a printer is remote is derived from the CUPS printer-type attribute for the print queue.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterIsRemote(
  PMPrinter   printer,
  Boolean *   isRemoteP)										AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrinterSetDefault()
 *  
 *  Summary:
 *    Set the default printer for the current user.
 *  
 *  Parameters:
 *    
 *    printer:
 *      The printer to set as the default printer.
 *  
 *  Special considerations:
 *      It is not typical for an application to set the current default printer
 *      for the user; the printing system itself takes care of managing the default printer.
 *      This function should be used only in rare circumstances.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus PMPrinterSetDefault(PMPrinter printer)			AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


#pragma mark
#pragma mark Preset
#pragma mark
/*
 *  PMPresetCopyName()
 *  
 *  Summary:
 *    Hand back a copy of the localized name for the specified preset.
 *  
 *  Parameters:
 *    
 *    preset:
 *      The preset whose name is needed.
 *    
 *    name:
 *      On exit, if this routine succeeds, *'name' is filled in with a
 *      reference to a localized string with the preset's name. If this
 *      routine fails, then *'name' is set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPresetCopyName(
  PMPreset								preset,
  CFStringRef __nullable * __nonnull	name)					AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPresetCreatePrintSettings()
 *  
 *  Summary:
 *    Create a print settings conforming to the specified print
 *    settings preset.
 *  
 *  Parameters:
 *    
 *    preset:
 *      A preset specifying a set of initial print settings.
 *    
 *    session:
 *      A valid print session.
 *    
 *    printSettings:
 *      On exit, *'printSettings' is set to a newly created print
 *      settings that contains the settings specified by 'preset'. The
 *      caller is responsible for calling PMRelease when the print
 *      settings are no longer needed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPresetCreatePrintSettings(
  PMPreset									preset,
  PMPrintSession							session,
  PMPrintSettings __nullable * __nonnull	printSettings)			AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

	
	/*
 *  PMPresetGetAttributes()
 *  
 *  Summary:
 *    Hand back the meta-data describing a given preset.
 *  
 *  Discussion:
 *    Each preset has associated with it a dictionary containing
 *    meta-data. The meta-data provides the preset's id, the preset's
 *    localized names, and descriptions of the environment for which
 *    the preset it intended.
 *  
 *  Parameters:
 *    
 *    preset:
 *      A print settings preset as obtained from PMPrinterCopyPresets().
 *    
 *    attributes:
 *      On exit, *'attributes' is set to reference a dictionary
 *      containing the preset's meta-data. The caller is responsible
 *      for retaining this reference if it is to be used beyond the
 *      lifetime of 'preset'. If this function fails, returning a
 *      non-zero error code, then *'attributes' is set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPresetGetAttributes(
  PMPreset									preset,
  CFDictionaryRef __nullable * __nonnull	attributes)				AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#pragma mark
#pragma mark Paper
#pragma mark
/*
 *  PMGetPageFormatPaper()
 *  
 *  Summary:
 *    Returns the paper associated with a pageformat.
 *  
 *  Parameters:
 *    
 *    format:
 *      Obtain the paper for this pageformat.
 *    
 *    paper:
 *      If successful noErr is returned and *paper will contain a
 *      PMPaper object describing the current paper associated with the
 *      pageformat.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMGetPageFormatPaper(
  PMPageFormat						format,
  PMPaper __nullable * __nonnull	paper)							AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPaperCreateCustom()
 *  
 *  Summary:
 *    Create a new custom paper instance.
 *  
 *  Parameters:
 *    
 *    printer:
 *      The new paper size is appropriate for this printer.
 *    
 *    id:
 *      A unique identifier for this paper type.
 *    
 *    name:
 *      The name to display to the user for this paper type.
 *    
 *    width:
 *      The width, in points, of the paper.
 *    
 *    height:
 *      The height, in points, of the paper.
 *    
 *    margins:
 *      The unprintable margins on the paper.
 *    
 *    paperP:
 *      if this function is successful, returning noErr, then *'paperP'
 *      is set to be a reference to a newly created PMPaper instance.
 *      The caller is responsible for calling PMRelease when the
 *      instance is no longer needed. If this functions fails, it will
 *      return a non-zero error and set *'paperP' to NULL.
 *  
 *  Discussion:
 *		This function creates a new custom paper instance. To obtain one of the available
 *		built-in paper sizes for a given printer, use PMPrinterGetPaperList.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperCreateCustom(
  PMPrinter __nullable				printer,
  CFStringRef __nullable			id,
  CFStringRef __nullable			name,
  double							width,
  double							height,
  const PMPaperMargins *			margins,
  PMPaper __nullable * __nonnull	paperP)						AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*
 *  PMPaperGetWidth()
 *  
 *  Summary:
 *    Returns the width for a given paper.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Obtain the width for this paper.
 *    
 *    paperWidth:
 *      If successful noErr is returned and *paperWidth is set to the
 *      width of the paper.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperGetWidth(
  PMPaper   paper,
  double *  paperWidth)											AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/*
 *  PMPaperGetHeight()
 *  
 *  Summary:
 *    Returns the height for a given paper.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Obtain the height for this paper.
 *    
 *    paperHeight:
 *      If successful noErr is returned and *paperHeight is set to the
 *      height of the paper.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperGetHeight(
  PMPaper   paper,
  double *  paperHeight)										AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/*
 *  PMPaperGetMargins()
 *  
 *  Summary:
 *    Returns the margins for a given paper.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Obtain the margin information for this paper.
 *    
 *    paperMargins:
 *      If successful noErr is returned and *paperMargins is set to the
 *      margins of the paper.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperGetMargins(
  PMPaper           paper,
  PMPaperMargins *  paperMargins)								AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPaperGetID()
 *  
 *  Summary:
 *    Returns the id for a given paper.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Obtain the id for this paper.
 *    
 *    paperID:
 *      If successful noErr is returned and *paperID is set to the id
 *      of the paper.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperGetID(
  PMPaper								paper,
  CFStringRef __nullable * __nonnull	paperID)					AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/*
 *  PMPaperGetPPDPaperName()
 *  
 *  Summary:
 *    Returns the PPD name for a given paper.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Obtain the PPD name for this paper.
 *    
 *    paperName:
 *      If successful, noErr is returned and *paperName is set to the
 *      PPD name of the paper. The returned result may be NULL.
 *  
 *  Discussion:
 *      The Mac OS X printing system uses a PostScript Printer Description (PPD)
 *      file to describe a given printer and print queue for that printer. The
 *      PPD name is the name that uniquely identifies a given paper
 *      to the printer to which the paper corresponds. 
 *    
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperGetPPDPaperName(
  PMPaper								paper,
  CFStringRef __nullable * __nonnull	paperName)				AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  PMPaperCreateLocalizedName()
 *  
 *  Summary:
 *    Returns the localized name for a given paper. 
 *    Use of PMPaperCreateLocalizedName
 *    is recommended instead of PMPaperGetName.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Obtain the localized name for this paper.
 *    
 *    printer:
 *      The printer for which the localization should be performed.
 *    
 *    paperName:
 *      If successful, noErr is returned and *paperName is set to the
 *      localized name of the paper. The returned paper name is that appropriate
 *      to display to a user. The returned result may be NULL.
 *      If non-NULL, the caller is responsible for releasing the name returned. 
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperCreateLocalizedName(
  PMPaper								paper,
  PMPrinter								printer,
  CFStringRef __nullable * __nonnull	paperName)				AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*
 *  PMPaperGetPrinterID()
 *  
 *  Summary:
 *    Returns the printerID of the printer for which the paper corresponds.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Obtain the printer ID for this paper.
 *    
 *    printerID:
 *      If successful, noErr is returned and *printerID is set to the
 *      ID of the printer for which the paper corresponds. The returned
 *      result may be NULL. 
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPaperGetPrinterID(
  PMPaper								paper,
  CFStringRef __nullable * __nonnull	printerID)				AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/*
 *  PMPaperIsCustom()
 *  
 *  Summary:
 *    Returns true if the paper is a custom paper.
 *  
 *  Parameters:
 *    
 *    paper:
 *      Determines if this is a custom paper.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern Boolean 
PMPaperIsCustom(PMPaper   paper )								AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

#pragma mark
#pragma mark PDF Workflow
#pragma mark
/*
 *  PMWorkflowCopyItems()
 *  
 *  Summary:
 *    Hand back an array of dictionaries describing the PDF Workflow
 *    items installed on the system
 *  
 *  Parameters:
 *    
 *    workflowItems:
 *      If this function returns without error then *'workflowItems'
 *      will be filled in with a reference to an array. It is the
 *      caller's responsability to release the array when done with it.
 *      Each element in the array describes a PDF Workflow item or a
 *      folder holding workflow items. A dictionary describing a
 *      workflow item has, at least, the following keys and values:
 *      displayName - The user's diaplayable name for the workflow item
 *      itemURL - A CFURLRef pointing to the workflow item. A
 *      dictionary describing a workflow folder has at least the
 *      following keys: displayName - The user's diaplayable name for
 *      the workflow item folderURL - A CFURLRef pointing to the
 *      folder. items - A CFArrayRef describing the workflow items in
 *      the folder. If this function returns a non-zero error code then
 *      *'workflowItems' will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMWorkflowCopyItems(
	CFArrayRef __nullable * __nonnull	workflowItems)			AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMWorkflowSubmitPDFWithOptions()
 *  
 *  Summary:
 *    Submit a PDF file for workflow processing.
 *  
 *  Discussion:
 *    The print dialog uses this function is conjunction with
 *    PMWorkflowGetItems to implement the PDF workflow button. Caller's
 *    can use PMWorkflowGetItems to obtain a CFURLRef that can be
 *    passed to PMWorkflowPDF or they can create a CFURLRef to another
 *    file system item.
 *  
 *  Parameters:
 *    
 *    workflowItem:
 *      A file system URL pointing to the workflow item that will
 *      handle the PDF file. Here are the different types of workflow
 *      items currently supported: Folder alias:   The PDF is moved to
 *      the resolved folder. Application or application alias: The
 *      application is sent an open event along with a reference to the
 *      PDF file. Compiled data fork AppleScript: The applescript is
 *      run with an open event along with a reference to the PDF file.
 *      executable tool: The tool is run with the following parameters:
 *      title options pdfFile
 *    
 *    title:
 *      The user displayable name of the document.
 *    
 *    options:
 *      A string of CUPS style key-value pairs that may be passed to
 *      the PDF Workflow item. This parameter can be NULL in which case
 *      an empty string of options is used.
 *    
 *    pdfFile:
 *      A file system URL pointing to the file to be processed by the
 *      workflow item.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMWorkflowSubmitPDFWithOptions(
  CFURLRef					workflowItem,
  CFStringRef __nullable	title,
  const char* __nullable	options,
  CFURLRef					pdfFile)							AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMWorkflowSubmitPDFWithSettings()
 *  
 *  Summary:
 *    Submit a PDF file for workflow processing.
 *  
 *  Discussion:
 *    The print dialog uses this function is conjunction with
 *    PMWorkflowGetItems to implement the PDF workflow button. Caller's
 *    can use PMWorkflowGetItems to obtain a CFURLRef that can be
 *    passed to PMWorkflowPDF or they can create a CFURLRef to another
 *    file system item.
 *  
 *  Parameters:
 *    
 *    workflowItem:
 *      A file system URL pointing to the workflow item that will
 *      handle the PDF file. Here are the different types of workflow
 *      items currently supported: Folder alias:   The PDF is moved to
 *      the resolved folder. Application or application alias: The
 *      application is sent an open event along with a reference to the
 *      PDF file. Compiled data fork AppleScript: The applescript is
 *      run with an open event along with a reference to the PDF file.
 *      executable tool: The tool is run with the following parameters:
 *      title options pdfFile
 *    
 *    settings:
 *      The prints settings to apply to the PDF.
 *    
 *    pdfFile:
 *      A file system URL pointing to the file to be processed by the
 *      workflow item.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMWorkflowSubmitPDFWithSettings(
  CFURLRef          workflowItem,
  PMPrintSettings   settings,
  CFURLRef          pdfFile)									AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#pragma mark
#pragma mark Job Submission
#pragma mark
/*
 *  PMPrinterPrintWithProvider()
 *  
 *  Summary:
 *    Submit print data to a specified printer.
 *  
 *  Discussion:
 *    For using EPS data together with other application drawing
 *    using Quartz, see PMCGImageCreateWithEPSDataProvider.
 *
 *  Parameters:
 *    
 *    printer:
 *      The printer.
 *    
 *    settings:
 *      The print settings for the print job.
 *    
 *    format:
 *      The page format specifying the physical page size and orientation on which the document
 *      should be printed. This parameter can be NULL.
 *    
 *    mimeType:
 *      The mime type of the file to be printed. This parameter can not
 *      be NULL. Use PMPrinterPrintWithFile() if auto-typing is desired.
 *    
 *    provider:
 *      The data provider that supplies the print data.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterPrintWithProvider(
  PMPrinter					printer,
  PMPrintSettings			settings,
  PMPageFormat __nullable	format,
  CFStringRef				mimeType,
  CGDataProviderRef			provider)							AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrinterPrintWithFile()
 *  
 *  Summary:
 *    Submit a file for printing to a specified printer.
 *  
 *  Discussion:
 *
 *    One reason this function may fail is if the specified printer can
 *    not handle the file's mime type. Use PMPrinterGetMimeTypes() to
 *    check whether a mime type is supported.
 *  
 *    For using EPS data together with other application drawing
 *    using Quartz, see PMCGImageCreateWithEPSDataProvider.
 *
 *  Parameters:
 *    
 *    printer:
 *      The printer.
 *    
 *    settings:
 *      The print settings for the print job.
 *    
 *    format:
 *      The page format specifying the physical page size and orientation on which the document
 *      should be printed. This parameter can be NULL.
 *    
 *    mimeType:
 *      The mime type of the file to be printed. If this parameter is
 *      NULL then the supplied file will be auto-typed.
 *    
 *    fileURL:
 *      A file URL specifying the file to be printed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrinterPrintWithFile(
  PMPrinter					printer,
  PMPrintSettings			settings,
  PMPageFormat __nullable	format,
  CFStringRef __nullable	mimeType,
  CFURLRef					fileURL)							AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/*
 *  PMPrinterWritePostScriptToURL()
 *
 *  Summary:
 *    Convert an input file of the specified mimeType to printer ready PostScript for the destination printer.
 *
 *  Discussion:
 *
 *    The conversion of the input file to PostScript is performed before this function returns. This can take a significant
 *    amount of time for longer documents. The caller of PMPrinterWritePostScriptToURL may want to perform this operation
 *    on a thread other than the main application thread or fork a separate process for this purpose.
 *
 *    One reason this function may fail is if conversion from the input mimeType to PostScript cannot be performed.
 *
 *
 *  Parameters:
 *
 *    printer:
 *      The printer for which the printer ready PostScript will be generated.
 *
 *    settings:
 *      The print settings for the print job.
 *
 *    format:
 *      The page format specifying the physical page size and orientation on which the document
 *      should be printed.
 *
 *    mimeType:
 *      The mime type of the file to be printed. If NULL, the file is auto-typed.
 *
 *    sourceFileURL:
 *      A file URL specifying the input file to be converted to printer ready PostScript data. Only file based URLs
 *      are supported. This file is deleted when the conversion is completed.
 *
 *    destinationFileURL:
 *      A file URL specifying the destination file to be created. If the file already exists it will be overwritten. Only
 *      file based URLs are supported.
 *
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus
PMPrinterWritePostScriptToURL(
	  PMPrinter					printer,
	  PMPrintSettings			settings,
	  PMPageFormat __nullable	format,
	  CFStringRef __nullable	mimeType,
	  CFURLRef					sourceFileURL,
	  CFURLRef					destinationFileURL)				AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/*
 *  PMPrintSettingsToOptions()
 *  
 *  Summary:
 *    Convert print settings to a CUPS style options string.
 *  
 *  Parameters:
 *    
 *    settings:
 *      The print settings that should be converted to a CUPS style
 *      options string.
 *    
 *    options:
 *      On exit *'options' will be filled in with a malloc'ed C string
 *      describing the passed in print settings. It is the caller's
 *      responsibility to free this memory when done with it. If this
 *      function fails returning a non-zero error code then *'options'
 *      will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsToOptions(
  PMPrintSettings				settings,
  char* __nullable * __nonnull	options)						AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMPrintSettingsToOptionsWithPrinterAndPageFormat()
 *  
 *  Summary:
 *    Convert a print settings and page format to a CUPS style options string.
 *  
 *  Parameters:
 *    
 *    settings:
 *      The print settings that should be converted to a CUPS style
 *      options string. Must not be NULL.
 *    
 *    printer:
 *      The printer to use for converting the print settings. Must not be NULL.
 *
 *    format:
 *      The page format for use in creating the cups options. Can be NULL.
 *    
 *    options:
 *      On exit *'options' will be filled in with a malloc'd C string
 *      describing the passed in print settings and format. It is the caller's
 *      responsibility to free this memory when done with it. If this
 *      function fails returning a non-zero error code then *'options'
 *      will be set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMPrintSettingsToOptionsWithPrinterAndPageFormat(
  PMPrintSettings				settings,
  PMPrinter						printer,
  PMPageFormat __nullable		pageFormat,
  char* __nullable * __nonnull	options)						AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

#pragma mark
#pragma mark PrinterCommandSupport
#pragma mark


/*
*  PMPrinterSendCommand()
*
*  Summary:
*    Submit a command file containing the specified string data to a print queue.
*
*  Parameters:
*
*    printer:
*      The destination print queue.
*
*    commandString:
*      The contents of the command file as a CFStringRef.
*
*    jobTitle:
*      The title of the job associated with the command file. If NULL,
*      a job title will be automatically generated but it may not be
*      meaningful to a user.
*
*    options:
*      A dictionary containing options that apply to the job.
*      This parameter should be NULL; it is reserved for future expansion.
*
*  Availability:
*    Mac OS X:         in version 10.6 and later in ApplicationServices.framework
*    CarbonLib:        not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
*/
extern OSStatus
PMPrinterSendCommand(
  PMPrinter						printer,
  CFStringRef					commandString,
  CFStringRef __nullable		jobTitle,
  CFDictionaryRef __nullable	options)						AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*
*  PMPrinterCopyState()
*
*  Summary:
*    Copies the current "marker-change-time", "marker-colors",
*    "marker-high-levels", "marker-levels", "marker-low-levels",
*    "marker-message", "marker-names", "marker-types", "printer-state",
*    "printer-state-change-time", "printer-state-message", and
*    "printer-state-reasons" values for a print queue.  These attributes are
*    defined in the "CUPS Implementation of IPP" specification at:
*
*        http://www.cups.org/documentation.php/spec-ipp.html
*        http://localhost:631/help/spec-ipp.html
*
*    Each attribute is returned using the corresponding CoreFoundation
*    data type:
*
*        Attribute                    Type
*        ---------------------------  ------------------------
*        "marker-change-time"         CFDateRef
*        "marker-colors"              CFArrayRef + CFStringRef
*        "marker-high-levels"         CFArrayRef + CFNumberRef
*        "marker-levels"              CFArrayRef + CFNumberRef
*        "marker-low-levels"          CFArrayRef + CFNumberRef
*        "marker-message"             CFStringRef
*        "marker-names"               CFArrayRef + CFStringRef
*        "marker-types"               CFArrayRef + CFStringRef
*        "printer-state"              CFNumberRef
*        "printer-state-change-time"  CFDateRef
*        "printer-state-message"      CFStringRef
*        "printer-state-reasons"      CFArrayRef + CFStringRef
*
*  Localization Considerations:
*
*    The "marker-message" and "printer-state-message" attributes may be
*    localized by the printer driver using the language of the last processed
*    print job.
*
*    The "marker-names" and "printer-state-reasons" attributes are typically
*    English-language keywords that may be localized using the printer's PPD
*    file and CUPS ppdLocalizeMarkerName and ppdLocalizeIPPReason APIs,
*    respectively.
*
*    All other attributes are predefined keywords or values that are not
*    localized.
*
*  Parameters:
*
*    printer:
*      The destination print queue.
*
*    stateDict:
*      A pointer to a CFDictionaryRef variable to hold the values.
*      You must release the dictionary when you are done using it.
*      The dictionary returned is not mutable.
*
*  Availability:
*    Mac OS X:         in version 10.6 and later in ApplicationServices.framework
*    CarbonLib:        not available
*  
*  Compatibility:
*  	Not appropriate for CUPS filters, drivers, and backends.
*  
*/

extern OSStatus
PMPrinterCopyState(
  PMPrinter									printer,
  CFDictionaryRef __nullable * __nonnull	stateDict)				AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;


#pragma mark
#pragma mark PPD related
#pragma mark
/*
 *  PMCopyAvailablePPDs()
 *  
 *  Summary:
 *    Hand back the list of PPDs in the specified PPD domain.
 *  
 *  Parameters:
 *    
 *    domain:
 *      The domain to search for PPDs.
 *    
 *    ppds:
 *      If this function completes without error, *'ppds' is set to an
 *      array of CFURLs. Each CFURL specifies the location of a PPD
 *      file or a compressed PPD file. The caller is responsible for
 *      releasing the array. If this function returns a non-zero error
 *      code then *'ppds' is set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCopyAvailablePPDs(
  PMPPDDomain						domain,
  CFArrayRef __nullable * __nonnull	ppds)						AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMCopyLocalizedPPD()
 *  
 *  Summary:
 *    Hand back a reference to a localized PPD.
 *  
 *  Parameters:
 *    
 *    ppd:
 *      A PPD reference. Typically this is a CFURLRef returned from
 *      PMCopyAvailablePPDs().
 *    
 *    localizedPPD:
 *      If this function completes without error, *'localizedPPD' will
 *      be set to a CFURLRef referencing the PPD that should be used
 *      given the current user's language preferences. If this function
 *      returns an error then *'localizedPPD' will be set to to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCopyLocalizedPPD(
  CFURLRef							ppd,
  CFURLRef __nullable * __nonnull	localizedPPD)				AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  PMCopyPPDData()
 *  
 *  Summary:
 *    Hand back the uncompressed PPD data for a PPD or compressed PPD
 *    file.
 *  
 *  Parameters:
 *    
 *    ppd:
 *      A reference to a PPD or compressed PPD file. This reference is
 *      usually obtained from PMCopyAvailablePPDs() or from
 *      PMCopyLocalizedPPD().
 *    
 *    data:
 *      If this function completes without error then *'data' is set to
 *      reference the uncompressed PPD data from the PPD file. If this
 *      function returns a non-zero error then *'data is set to NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern OSStatus 
PMCopyPPDData(
  CFURLRef							ppd,
  CFDataRef __nullable * __nonnull	data)						AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#pragma mark
#pragma mark EPS related
#pragma mark
/*
 *  PMCGImageCreateWithEPSDataProvider()
 *  
 *  Summary:
 *    Create an image reference that references both the PostScript
 *    contents of an EPS file and a preview (proxy) image for that EPS
 *    file.
 *  
 *  Discussion:
 *    For OS X 10.1.0, this function ignores the passed in data
 *    provider. The passed in image reference is retained and then
 *    returned. For 10.1.1 and later, then the data provider is used
 *    and the returned image reference is different than the passed in
 *    image reference, so please be careful with your use of these
 *    references. It is likely that the data will not be read from the
 *    EPS data provider until well after this function returns. The
 *    caller should be careful not to free the underlying EPS data
 *    until the provider's release routine is invoked. Similarly the
 *    preview image's data may be needed long after you think it should
 *    be. Do not free the image data until the image data provider's
 *    release data function has been called. To make sure these data
 *    providers are properly reference counted, release your reference
 *    the EPS data provider and on the EPS image preview when they are
 *    no longer needed by your application. For Mac OS X 10.2 and
 *    later, the contents of the EPS provider at the time of this call
 *    can be dumped to a file if you first do the following, BEFORE
 *    running your application. 
 *
 *	  From the command line in terminal:
 *		defaults write NSGlobalDomain com.apple.print.eps.testProvider /tmp/dump.eps
 *    
 *	  causes a dump of the EPS data into a file /tmp/dump.eps.
 *  
 *  Parameters:
 *    
 *    epsDataProvider:
 *      A Core Graphics data provider that can supply the PostScript
 *      contents of the EPS file. Post OS X 10.1, there will be some
 *      checking done on the EPS data provided to the
 *      PMCGImageCreateWithEPSDataProvider() call. It is important that
 *      the EPS data begin with the EPSF required header and bounding
 *      box DSC comments.
 *    
 *    epsPreview:
 *      A Core Graphics image reference to the proxy image for the EPS
 *      file. When the image reference result of this function is
 *      rendered on screen or printed to a printer that can not render
 *      PostScript this proxy image is drawn instead.
 *  
 *  Result:
 *    an image reference capable of rendering either the EPS content or
 *    the proxy image depending upon the capabilities of the targeted
 *    context.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 *  
 *  Compatibility:
 *  	Not appropriate for CUPS filters, drivers, and backends.
 *  
 */
extern CGImageRef __nullable
PMCGImageCreateWithEPSDataProvider(
  CGDataProviderRef __nullable	epsDataProvider,
  CGImageRef					epsPreview)						AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


#ifdef __cplusplus
}
#endif

CF_ASSUME_NONNULL_END

#endif /* __PMCORE__ */

