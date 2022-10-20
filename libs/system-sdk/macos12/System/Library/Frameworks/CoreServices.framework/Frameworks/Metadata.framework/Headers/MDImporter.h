/*
 *       MDImporter.h
 *       Copyright (c) 2004-2019, Apple Inc. All rights reserved.
 */

/*!
        @header MDImporter.h
        MDImporter is the link between the metadata contained  within a
        file and an MDItem. The Server listens for changes to files
        and loads in a CFPlugIn (MDImporter) and the importer will then
        extract all interesting data out of the file to store in a MDItem.

*/
#if !defined(__MDIMPORTER__)
#define __MDIMPORTER__ 1

#include <CoreFoundation/CoreFoundation.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4

#include <Metadata/MDItem.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
        @constant kMDImporterTypeID The importer only loads CFPlugIns of type
        kMDImporterTypeID - 8B08C4BF-415B-11D8-B3F9-0003936726FC

        @constant kMDImporterInterfaceID Importers must implement this
        Interface - 6EBC27C4-899C-11D8-84A3-0003936726FC

        @constant kMDExporterInterfaceID Exporters can optionaly also implement this
        Interface - B41C6074-7DFB-4057-969D-31C8E861A8D4

        @constant kMDImporterURLInterfaceID Importers can optionaly also implement this
        Interface - B41C6074-7DFB-4057-969D-31C8E861A8D4
 
        @constant kMDImporterBundleWrapperURLInterfaceID Importers can optionaly also implement this
        Interface - CF76374B-0C83-47C5-AB2F-7B950884670A
 
*/

#define kMDImporterTypeID      CFUUIDGetConstantUUIDWithBytes(kCFAllocatorDefault,0x8B,0x08,0xC4,0xBF,0x41,0x5B,0x11,0xD8,0xB3,0xF9,0x00,0x03,0x93,0x67,0x26,0xFC)
#define kMDImporterInterfaceID CFUUIDGetConstantUUIDWithBytes(kCFAllocatorDefault,0x6E,0xBC,0x27,0xC4,0x89,0x9C,0x11,0xD8,0x84,0xAE,0x00,0x03,0x93,0x67,0x26,0xFC)
#define kMDExporterInterfaceID CFUUIDGetConstantUUIDWithBytes(kCFAllocatorDefault,0xB4,0x1C,0x60,0x74,0x7D,0xFB,0x40,0x57,0x96,0x9D,0x31,0xC8,0xE8,0x61,0xA8,0xD4)

#define kMDImporterURLInterfaceID CFUUIDGetConstantUUIDWithBytes(kCFAllocatorDefault, 0x13,0xF6,0x0F,0x02,0x36,0x22,0x4F,0x35,0x98,0x91,0xEC,0x10,0xE6,0xCD,0x08,0xF8)
#define kMDImporterBundleWrapperURLInterfaceID CFUUIDGetConstantUUIDWithBytes(kCFAllocatorDefault, 0xCF,0x76,0x37,0x4B,0x0C,0x83,0x47,0xC5,0xAB,0x2F,0x7B,0x95,0x08,0x84,0x67,0x0A)

    
   
    
/*!
        @typedef MDImporterInterfaceStruct
        Interface for the plugIn. The plugin needs to provide at least
        the ImporterImportData function.

        @function ImporterImportData
        Import data from the file given to by path into the system.

        @param thisInterface this plugin

        @param attributes any attributes that can be gotten from the
        file should be places in the attributes
        CFMutableDictionaryRef. For example if the file has a
        kMDItemDurationSeconds then the key would be kMDItemDurationSeconds and the
        value would be a CFNumberRef containing the duration of the
        song etc. If you want to remove an attribute, then in the
        dictionary place the attribute and set the value of the
        attribute to kCFNull.

        @param contentTypeUTI  The content type (a uniform type identifier)
        of the file that is given to this plugin.

        @param pathToFile location of the file on disk.

        @result Boolean indicating if the import was successful


        @typedef MDExporterInterfaceStruct
        Interface for exporting data from the mdimport system back to the file.

        @function ImporterExportData

        @param thisInterface this plugin

        @param attributes any attributes should be written back to the file

        @param contentTypeUTI  The content type (a uniform type identifier)
        of the file that is given to this plugin.

        @param pathToFile location of the file on disk.

        @result Boolean indicating if the export was successful
*/

#if defined(__COREFOUNDATION_CFPLUGINCOM__)
typedef struct {
    IUNKNOWN_C_GUTS;
    Boolean    (*ImporterImportData)(void *thisInterface,
                                     CFMutableDictionaryRef attributes,
                                     CFStringRef contentTypeUTI,
                                     CFStringRef pathToFile);
} MDImporterInterfaceStruct;

typedef struct {
    IUNKNOWN_C_GUTS;
    Boolean    (*ImporterExportData)(void *thisInterface,
                                     CFDictionaryRef attributes,
                                     CFStringRef contentTypeUTI,
                                     CFStringRef pathToFile);
} MDExporterInterfaceStruct;

typedef struct {
    IUNKNOWN_C_GUTS;
    Boolean    (*ImporterImportURLData)(void *thisInterface,
                                        CFMutableDictionaryRef attributes,
                                        CFStringRef contentTypeUTI,
                                        CFURLRef urlForFile);
} MDImporterURLInterfaceStruct;

typedef struct {
    IUNKNOWN_C_GUTS;
    Boolean    (*ImporterImportBundleWrapperURLData)(void *thisInterface,
                                                     CFMutableDictionaryRef attributes,
                                                     CFStringRef contentTypeUTI,
                                                     CFURLRef urlForFile);
} MDImporterBundleWrapperURLInterfaceStruct;

#endif

#if defined(__cplusplus)
}
#endif

#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */

#endif /* __MDIMPORTER__ */
