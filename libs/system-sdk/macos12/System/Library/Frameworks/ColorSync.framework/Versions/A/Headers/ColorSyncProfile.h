/*
 * ColorSync - ColorSyncProfile.h
 * Copyright (c)  2008 Apple Inc.
 * All rights reserved.
 */

#ifndef __COLORSYNCPROFILE__
#define __COLORSYNCPROFILE__

#if !defined(__swift__)

#ifdef __cplusplus
extern "C" {
#endif

#include <ColorSync/ColorSyncBase.h>

typedef const struct CF_BRIDGED_TYPE(id) ColorSyncProfile* ColorSyncProfileRef;

typedef struct CF_BRIDGED_TYPE(id) ColorSyncProfile* ColorSyncMutableProfileRef;

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN
    
CSEXTERN CFStringRef kColorSyncGenericGrayProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);          /* com.apple.ColorSync.GenericGray  */
CSEXTERN CFStringRef kColorSyncGenericGrayGamma22Profile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* com.apple.ColorSync.GenericGrayGamma2.2  */
CSEXTERN CFStringRef kColorSyncGenericRGBProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);           /* com.apple.ColorSync.GenericRGB   */
CSEXTERN CFStringRef kColorSyncGenericCMYKProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);          /* com.apple.ColorSync.GenericCMYK  */
CSEXTERN CFStringRef kColorSyncDisplayP3Profile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);            /* com.apple.ColorSync.DisplayP3 */
CSEXTERN CFStringRef kColorSyncSRGBProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                 /* com.apple.ColorSync.sRGB         */
CSEXTERN CFStringRef kColorSyncAdobeRGB1998Profile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);         /* com.apple.ColorSync.AdobeRGB1998 */
CSEXTERN CFStringRef kColorSyncGenericLabProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);           /* com.apple.ColorSync.GenericLab */
CSEXTERN CFStringRef kColorSyncGenericXYZProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);           /* com.apple.ColorSync.GenericXYZ */

CSEXTERN CFStringRef kColorSyncACESCGLinearProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);         /* com.apple.ColorSync.ACESCGLinear */
CSEXTERN CFStringRef kColorSyncDCIP3Profile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                /* com.apple.ColorSync.DCIP3 */
CSEXTERN CFStringRef kColorSyncITUR709Profile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);              /* com.apple.ColorSync.ITUR709 */
CSEXTERN CFStringRef kColorSyncITUR2020Profile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);             /* com.apple.ColorSync.ITUR2020 */
CSEXTERN CFStringRef kColorSyncROMMRGBProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);              /* com.apple.ColorSync.ROMMRGB */

CSEXTERN CFStringRef kColorSyncProfileHeader CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);      /* com.apple.ColorSync.ProfileHeader */
CSEXTERN CFStringRef kColorSyncProfileClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);       /* com.apple.ColorSync.ProfileClass */
CSEXTERN CFStringRef kColorSyncProfileColorSpace CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);  /* com.apple.ColorSync.ProfileColorSpace */
CSEXTERN CFStringRef kColorSyncProfilePCS CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);         /* com.apple.ColorSync.PCS */
CSEXTERN CFStringRef kColorSyncProfileURL CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);         /* com.apple.ColorSync.ProfileURL */
CSEXTERN CFStringRef kColorSyncProfileDescription CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED); /* com.apple.ColorSync.ProfileDescription */
CSEXTERN CFStringRef kColorSyncProfileMD5Digest CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* com.apple.ColorSync.ProfileMD5Digest */

CSEXTERN CFStringRef kColorSyncSigAToB0Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x41324230L => CFSTR("A2B0")*/
CSEXTERN CFStringRef kColorSyncSigAToB1Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x41324231L => CFSTR("A2B1")*/
CSEXTERN CFStringRef kColorSyncSigAToB2Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x41324232L => CFSTR("A2B2")*/
CSEXTERN CFStringRef kColorSyncSigBToA0Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x42324130L => CFSTR("B2A0")*/
CSEXTERN CFStringRef kColorSyncSigBToA1Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x42324131L => CFSTR("B2A1")*/
CSEXTERN CFStringRef kColorSyncSigBToA2Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x42324132L => CFSTR("B2A2")*/
CSEXTERN CFStringRef kColorSyncSigCmykData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x434D594BL => CFSTR("CMYK")*/
CSEXTERN CFStringRef kColorSyncSigGrayData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x47524159L => CFSTR("GRAY")*/
CSEXTERN CFStringRef kColorSyncSigLabData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                        /* 0x4C616220L => CFSTR("Lab ")*/
CSEXTERN CFStringRef kColorSyncSigRgbData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                        /* 0x52474220L => CFSTR("RGB ")*/
CSEXTERN CFStringRef kColorSyncSigXYZData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                        /* 0x58595A20L => CFSTR("XYZ ")*/
CSEXTERN CFStringRef kColorSyncSigAbstractClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                  /* 0x61627374L => CFSTR("abst")*/
CSEXTERN CFStringRef kColorSyncSigBlueTRCTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                     /* 0x62545243L => CFSTR("bTRC")*/
CSEXTERN CFStringRef kColorSyncSigBlueColorantTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                /* 0x6258595AL => CFSTR("bXYZ")*/
CSEXTERN CFStringRef kColorSyncSigMediaBlackPointTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);             /* 0x626B7074L => CFSTR("bkpt")*/
CSEXTERN CFStringRef kColorSyncSigCopyrightTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                   /* 0x63707274L => CFSTR("cprt")*/
CSEXTERN CFStringRef kColorSyncSigProfileDescriptionTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);          /* 0x64657363L => CFSTR("desc")*/
CSEXTERN CFStringRef kColorSyncSigDeviceModelDescTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);             /* 0x646D6464L => CFSTR("dmdd")*/
CSEXTERN CFStringRef kColorSyncSigDeviceMfgDescTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);               /* 0x646D6E64L => CFSTR("dmnd")*/
CSEXTERN CFStringRef kColorSyncSigGreenTRCTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                    /* 0x67545243L => CFSTR("gTRC")*/
CSEXTERN CFStringRef kColorSyncSigGreenColorantTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);               /* 0x6758595AL => CFSTR("gXYZ")*/
CSEXTERN CFStringRef kColorSyncSigGamutTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                       /* 0x67616D74L => CFSTR("gamt")*/
CSEXTERN CFStringRef kColorSyncSigGrayTRCTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                     /* 0x6B545243L => CFSTR("kTRC")*/
CSEXTERN CFStringRef kColorSyncSigLinkClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                      /* 0x6C696E6BL => CFSTR("link")*/
CSEXTERN CFStringRef kColorSyncSigDisplayClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                   /* 0x6D6E7472L => CFSTR("mntr")*/
CSEXTERN CFStringRef kColorSyncSigNamedColor2Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                 /* 0x6E636C32L => CFSTR("ncl2")*/
CSEXTERN CFStringRef kColorSyncSigNamedColorClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                /* 0x6E6D636CL => CFSTR("nmcl")*/
CSEXTERN CFStringRef kColorSyncSigPreview0Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                    /* 0x70726530L => CFSTR("pre0")*/
CSEXTERN CFStringRef kColorSyncSigPreview1Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                    /* 0x70726531L => CFSTR("pre1")*/
CSEXTERN CFStringRef kColorSyncSigPreview2Tag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                    /* 0x70726532L => CFSTR("pre2")*/
CSEXTERN CFStringRef kColorSyncSigOutputClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                    /* 0x70727472L => CFSTR("prtr")*/
CSEXTERN CFStringRef kColorSyncSigProfileSequenceDescTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);         /* 0x70736571L => CFSTR("pseq")*/
CSEXTERN CFStringRef kColorSyncSigRedTRCTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                      /* 0x72545243L => CFSTR("rTRC")*/
CSEXTERN CFStringRef kColorSyncSigRedColorantTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                 /* 0x7258595AL => CFSTR("rXYZ")*/
CSEXTERN CFStringRef kColorSyncSigInputClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                     /* 0x73636E72L => CFSTR("scnr")*/
CSEXTERN CFStringRef kColorSyncSigColorSpaceClass CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                /* 0x73706163L => CFSTR("spac")*/
CSEXTERN CFStringRef kColorSyncSigTechnologyTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);                  /* 0x74656368L => CFSTR("tech")*/
CSEXTERN CFStringRef kColorSyncSigViewingConditionsTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);           /* 0x76696577L => CFSTR("view")*/
CSEXTERN CFStringRef kColorSyncSigViewingCondDescTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);             /* 0x76756564L => CFSTR("vued")*/
CSEXTERN CFStringRef kColorSyncSigMediaWhitePointTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);             /* 0x77747074L => CFSTR("wtpt")*/

    
CSEXTERN CFStringRef kColorSyncProfileComputerDomain CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED; /* com.apple.ColorSync.ProfileDomain.Computer */
CSEXTERN CFStringRef kColorSyncProfileUserDomain CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;     /* com.apple.ColorSync.ProfileDomain.User */
    
#define COLORSYNC_PROFILE_INSTALL_ENTITLEMENT   "com.apple.developer.ColorSync.profile.install"
    

CSEXTERN CFTypeID ColorSyncProfileGetTypeID(void) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    * returns the CFTypeID for ColorSyncProfiles.
    */

CSEXTERN ColorSyncProfileRef __nullable ColorSyncProfileCreate(CFDataRef data, CFErrorRef* __nullable error) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   data   - profile data
    *   error  - (optional) pointer to the error which will be returned in case of failure
    *   
    *   returns ColorSyncProfileRef or NULL in case of failure
    */

CSEXTERN ColorSyncProfileRef __nullable ColorSyncProfileCreateWithURL(CFURLRef url, CFErrorRef* __nullable error) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   url    - URL to the profile data.
    *   error  - (optional) pointer to the error which will be returned in case of failure
    *   
    *   returns ColorSyncProfileRef or NULL in case of failure
    */

CSEXTERN ColorSyncProfileRef __nullable ColorSyncProfileCreateWithName(CFStringRef name) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   name    - predefined profile name
    *   
    *   returns ColorSyncProfileRef or NULL in case of failure
    */

    
CSEXTERN ColorSyncProfileRef __nullable ColorSyncProfileCreateWithDisplayID(uint32_t displayID) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
   /*
    *   displayID - system-wide unique display ID (defined by IOKIt); pass 0 for main display.
    *   
    *   returns ColorSyncProfileRef or NULL in case of failure
    */

CSEXTERN ColorSyncProfileRef __nullable ColorSyncProfileCreateDeviceProfile(CFStringRef deviceClass, CFUUIDRef deviceID, CFTypeRef profileID) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
   /*
    *   deviceClass - ColorSync device class 
    *   deviceID    - deviceID registered with ColorSync
    *   profileID   - profileID registered with ColorSync; pass kColorSyncDeviceDefaultProfileID to get the default profile.
    *   
    *   See ColorSyncDevice.h for more info on deviceClass, deviceID and profileID
    *
    *   returns ColorSyncProfileRef or NULL in case of failure
    */
    

CSEXTERN ColorSyncMutableProfileRef __nullable ColorSyncProfileCreateMutable(void) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   returns empty ColorSyncMutableProfileRef or NULL in case of failure
    */

CSEXTERN ColorSyncMutableProfileRef __nullable ColorSyncProfileCreateMutableCopy(ColorSyncProfileRef prof) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *  prof  - profile from which profile data will be copied to the created profile.
    *   
    *   returns ColorSyncMutableProfileRef or NULL in case of failure
    */

CSEXTERN ColorSyncProfileRef __nullable ColorSyncProfileCreateLink (CFArrayRef profileInfo, CFDictionaryRef __nullable options) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   profileInfo  - array of dictionaries, each one containing a profile object and the
    *                       information on the usage of the profile in the transform.
    *               
    *               Required keys:
    *               ==============
    *                      kColorSyncProfile           : ColorSyncProfileRef
    *                      kColorSyncRenderingIntent   : CFStringRef defining rendering intent
    *                      kColorSyncTransformTag      : CFStringRef defining which tags to use 
    *               Optional key:
    *               =============
    *                    kColorSyncBlackPointCompensation : CFBooleanRef to enable/disable BPC
    *   
    *   options      - dictionary with additional public global options (e.g. preferred CMM, quality,
    *                       etc... It can also contain custom options that are CMM specific.
    *
    *   returns ColorSyncProfileRef or NULL in case of failure
    */

CSEXTERN bool ColorSyncProfileVerify (ColorSyncProfileRef prof, CFErrorRef* __nullable errors, CFErrorRef* __nullable warnings) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof    - profile to be verified
    *
    *   errors  - returns error strings in case problems are found which 
    *                  would prevent use of the profile.
    *  
    *   warnings - returns warning strings indicating problems due to lack of
    *                       conformance with the ICC specification, but not preventing
    *                       use of the profile.
    *
    *   returns true if profile can be used or false otherwise
    */

CSEXTERN bool ColorSyncProfileIsWideGamut (ColorSyncProfileRef) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    * A utility function estimating gamut of a display profile
    */

CSEXTERN bool ColorSyncProfileIsMatrixBased (ColorSyncProfileRef) CS_AVAILABLE_PUBLIC_STARTING(11.0, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
    /*
     * A utility function verifying if a profile is matrix-based
     */

CSEXTERN bool ColorSyncProfileIsPQBased (ColorSyncProfileRef) CS_AVAILABLE_PUBLIC_STARTING(11.0, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
    /*
     * A utility function verifying if a profile is using ITU BT.2100 PQ transfer functions
     */

CSEXTERN bool ColorSyncProfileIsHLGBased (ColorSyncProfileRef) CS_AVAILABLE_PUBLIC_STARTING(11.0, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
    /*
     * A utility function verifying if a profile is using ITU BT.2100 HLG transfer functions
     */

    
CSEXTERN float ColorSyncProfileEstimateGammaWithDisplayID (const int32_t displayID, CFErrorRef* __nullable error) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
   /*
    *   displayID - system-wide unique display ID (defined by IOKIt)
    *   error     - (optional) pointer to the error which will be returned in
    *               case of failure
    *   
    *   returns non-zero value if success or 0.0 in case of error.
    */

CSEXTERN float ColorSyncProfileEstimateGamma (ColorSyncProfileRef prof, CFErrorRef* __nullable error) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
    /*
    *   prof    - profile to perform estimation on
    *   error   - (optional) pointer to the error which will be returned in
    *             case of failure
    *   
    *   returns non-zero value if success or 0.0 in case of error
    */
    
CSEXTERN bool ColorSyncProfileGetDisplayTransferFormulaFromVCGT(ColorSyncProfileRef profile,
                                                                float* redMin,   float* redMax,   float* redGamma,
                                                                float* greenMin, float* greenMax, float* greenGamma,
                                                                float* blueMin,  float* blueMax,  float* blueGamma) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
    /*
     * An utility function converting vcgt tag (if vcgt tag exists in the profile and conversion possible)
     * to formula components used by CGSetDisplayTransferByFormula.
     */
    
    CSEXTERN CFDataRef __nullable ColorSyncProfileCreateDisplayTransferTablesFromVCGT(ColorSyncProfileRef profile, size_t *nSamplesPerChannel) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
    /*
     * An utility function creating three tables of floats (redTable, greenTable, blueTable)
     * each of size nSamplesPerChannel, packed into contiguous memory contained in the CFDataRef
     *  to be returned from the vcgt tag of the profile (if vcgt tag exists in the profile).
     * Used by CGSetDisplayTransferByTable.
     */
    

#define COLORSYNC_MD5_LENGTH  16

typedef struct { uint8_t digest[COLORSYNC_MD5_LENGTH]; } ColorSyncMD5;

CSEXTERN ColorSyncMD5 ColorSyncProfileGetMD5(ColorSyncProfileRef prof) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /* 
    *   returns MD5 digest for the profile calculated as defined by
    *           ICC specification or a "zero" signature (filled with zeros)
    *           in case of failure
    */

CSEXTERN CFDataRef ColorSyncProfileCopyData (ColorSyncProfileRef prof, CFErrorRef* __nullable error) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof    - profile to copy the flattened data from
    *   error  - (optional) pointer to the error which will be returned in case of failure
    *   
    *   returns CFDataRef if success or NULL in case of failure 
    */

CSEXTERN CFURLRef ColorSyncProfileGetURL (ColorSyncProfileRef prof, CFErrorRef* __nullable error) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof   - profile to get URL from
    *   error  - (optional) pointer to the error which will be returned in case of failure
    *   
    *   returns CFURLRef if success or NULL in case of failure 
    */

CSEXTERN CFDataRef ColorSyncProfileCopyHeader (ColorSyncProfileRef prof) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof    - profile from which to copy the header
    *   
    *   returns CFDataRef containing profile header (in host endianess) or NULL in case of failure 
    */

CSEXTERN void ColorSyncProfileSetHeader (ColorSyncMutableProfileRef prof, CFDataRef header) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof        - profile in which to set the header
    *   header  - CFDataRef containing the header data (must be in host endianess)
    */

CSEXTERN CFStringRef __nullable ColorSyncProfileCopyDescriptionString (ColorSyncProfileRef prof) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof    - profile from which to copy description string
    *   
    *   returns CFStringRef containing profile description localized to current locale
    */

CSEXTERN CFArrayRef __nullable ColorSyncProfileCopyTagSignatures (ColorSyncProfileRef prof) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof    - profile from which to copy tag signatures
    *   
    *   returns CFArray with signatures (CFStringRef) of tags in the profile 
    */

CSEXTERN bool ColorSyncProfileContainsTag (ColorSyncProfileRef prof, CFStringRef signature) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof        - profile in which to search for the tag
    *   signature   - signature of the tag to be searched for
    *   
    *   returns true if tag exists or false if does not 
    */

CSEXTERN CFDataRef __nullable ColorSyncProfileCopyTag (ColorSyncProfileRef prof, CFStringRef signature) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof             - profile from which to copy the tag
    *   signature   - signature of the tag to be copied
    *   
    *   returns CFDataRef containing tag data or NULL in case of failure 
    */

CSEXTERN void ColorSyncProfileSetTag (ColorSyncMutableProfileRef prof, CFStringRef signature, CFDataRef data) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof           - profile in which to set the tag
    *   signature - signature of the tag to be set in the profile
    *   data          - CFDataRef containing the tag data
    */

CSEXTERN void ColorSyncProfileRemoveTag (ColorSyncMutableProfileRef prof, CFStringRef signature) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   prof              - profile from which to remove the tag
    *   signature    - signature of the tag to be removed
    *   
    *   returns true if success or false in case of failure 
    */


typedef bool (*ColorSyncProfileIterateCallback)(CFDictionaryRef profileInfo, void* userInfo);
   /*
    * Notes:
    *   1. Only validated profiles will be passed to the caller
    *   2. if the ColorSyncProfileIterateCallback returns false, the iteration stops
    */

#define COLORSYNC_ITERATE_ASYNC

CSEXTERN CFStringRef kColorSyncProfileCacheSeed;
   /*
    * Current cache seed (uint32_t) sent along with kColorSyncProfileRepositoryChangeNotification
    */

CSEXTERN void ColorSyncIterateInstalledProfiles (ColorSyncProfileIterateCallback __nullable callBack,
                                                 uint32_t*                       __nullable seed,
                                                 void*                           __nullable userInfo,
                                                 CFErrorRef*                     __nullable error) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    * callBack - pointer to a client provided function (can be NULL)
    * seed     - pointer to a cache seed owned by the client (can be NULL)
    * error    - (optional) pointer to the error which will be returned in case of failure
    *
    * Note: When called for the first time this function will return only system profiles because
    * profile iteration is a slow process requiring multiple access to file system.
    * Clients are advised to register for kColorSyncProfileRepositoryChangeNotification
    * using CFNotificationCenter or NSNotificationCenter to obtain all installed profiles.
    * Notification callback will receive, if possible, a CFDictionary containing a new seed
    * value of the profile cache (key kColorSyncProfileCacheSeed / value kCFNumberSInt32Type).
    * Comparing old and new seed values may prevent unnecessary calls to iterate installed
    * profiles.
    * Alternatively, ColorSyncIterateInstalledProfilesWithOptions can be used with kColorSyncWaitForCacheReply
    * option set to kCFBooleanTrue to obtain all installed profiles in one call.
    */


CSEXTERN CFStringRef kColorSyncWaitForCacheReply  CS_AVAILABLE_STARTING(10.16) CS_UNAVAILABLE_EMBEDDED;

CSEXTERN void ColorSyncIterateInstalledProfilesWithOptions (ColorSyncProfileIterateCallback __nullable callBack,
                                                            uint32_t*                       __nullable seed,
                                                            void*                           __nullable userInfo,
                                                            CFDictionaryRef                 __nullable options,
                                                            CFErrorRef*                     __nullable error) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
   /*
    * callBack - pointer to a client provided function (can be NULL)
    * seed     - pointer to a cache seed owned by the client
    * options  = a dictionary with iteration options, e.g. kColorSyncWaitForCacheReply to skip updates
    * error    - (optional) pointer to the error that will be returned in case of failure
    *
    */

CSEXTERN bool ColorSyncProfileInstall(ColorSyncProfileRef profile, CFStringRef domain, CFStringRef subpath, CFErrorRef* __nullable error) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
   /*
    * profile   - profile to be installed
    * domain    - either kColorSyncProfileComputerDomain or kColorSyncProfileUserDomain.
    *             kColorSyncProfileComputerDomain is for sharing the profiles (from /Library/ColorSync/Profiles).
    *             kColorSyncProfileUserDomain is for user custom profiles (installed under home directory, i.e. in 
    *             ~/Library/ColorSync/Profiles.
    *             NULL is the same as kColorSyncProfileUserDomain.
    * subpath   - CFString created from the file system representation of the path of the file to contain the installed
    *             profile. The last component of the path is interpreted as a file name if it ends with the extension ".icc".
    *             Otherwise, the subpath is interpreted as the directory path and file name will be created from the 
    *             profile description tag, appended with the ".icc" extension.
    * error     - (optional) pointer to the error which will be returned in case of failure.
    *
    *             bool value true is returned if success or false in case of error.
    */
    
CSEXTERN bool ColorSyncProfileUninstall(ColorSyncProfileRef profile, CFErrorRef* __nullable error) CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED;
   /*
    * profile   - profile to be uninstalled. This profile must return a valid url for ColorSyncProfileGetURL,
    *             i.e. it must be created with ColorSyncProfileCreateWithURL. Also, the url
    *             must be in either in kColorSyncProfileComputerDomain or
    *             kColorSyncProfileUserDomain, including subfolders of those.
    * error     - (optional) pointer to the error which will be returned in case of failure.
    *
    *             bool value true is returned if success or false in case of error.
    */
    

    /********************************************************************************************
    *                                                                                           *
    *   For further information on ICC profiles refer to ICC profile specification published    *
    *   by International Color Consortium at www.color.org                                      *
    *                                                                                           *
    *   ICC header file (ICC.h) containing programmatic definitions of ICC data types can be    *
    *   downloaded from the Technical Notes web page of ICC website at                          *
    *   www.color.org/technotes.xalter                                                          *
    *                                                                                           *
    ********************************************************************************************/

#ifdef __cplusplus
}
#endif

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#else /* #if !defined(__swift__) */

#ifdef __cplusplus
extern "C" {
#endif
    
#include <ColorSync/ColorSyncBase.h>
    
    typedef const struct CF_BRIDGED_TYPE(id) ColorSyncProfile* ColorSyncProfileRef;
    
    typedef struct CF_BRIDGED_TYPE(id) ColorSyncProfile* ColorSyncMutableProfileRef;
    
    CSEXTERN CFStringRef kColorSyncGenericGrayProfile;          /* com.apple.ColorSync.GenericGray  */
    CSEXTERN CFStringRef kColorSyncGenericGrayGamma22Profile;   /* com.apple.ColorSync.GenericGrayGamma2.2  */
    CSEXTERN CFStringRef kColorSyncGenericRGBProfile;           /* com.apple.ColorSync.GenericRGB   */
    CSEXTERN CFStringRef kColorSyncGenericCMYKProfile;          /* com.apple.ColorSync.GenericCMYK  */
    CSEXTERN CFStringRef kColorSyncDisplayP3Profile;            /* com.apple.ColorSync.DisplayP3 */
    CSEXTERN CFStringRef kColorSyncSRGBProfile;                 /* com.apple.ColorSync.sRGB         */
    CSEXTERN CFStringRef kColorSyncAdobeRGB1998Profile;         /* com.apple.ColorSync.AdobeRGB1998 */
    CSEXTERN CFStringRef kColorSyncGenericLabProfile;           /* com.apple.ColorSync.GenericLab */
    CSEXTERN CFStringRef kColorSyncGenericXYZProfile;           /* com.apple.ColorSync.GenericXYZ */
    
    CSEXTERN CFStringRef kColorSyncACESCGLinearProfile;         /* com.apple.ColorSync.ACESCGLinear */
    CSEXTERN CFStringRef kColorSyncDCIP3Profile;                /* com.apple.ColorSync.DCIP3 */
    CSEXTERN CFStringRef kColorSyncITUR709Profile;              /* com.apple.ColorSync.ITUR709 */
    CSEXTERN CFStringRef kColorSyncITUR2020Profile;             /* com.apple.ColorSync.ITUR2020 */
    CSEXTERN CFStringRef kColorSyncROMMRGBProfile;              /* com.apple.ColorSync.ROMMRGB */
    
    CSEXTERN CFStringRef kColorSyncProfileHeader;      /* com.apple.ColorSync.ProfileHeader */
    CSEXTERN CFStringRef kColorSyncProfileClass;       /* com.apple.ColorSync.ProfileClass */
    CSEXTERN CFStringRef kColorSyncProfileColorSpace;  /* com.apple.ColorSync.ProfileColorSpace */
    CSEXTERN CFStringRef kColorSyncProfilePCS;         /* com.apple.ColorSync.PCS */
    CSEXTERN CFStringRef kColorSyncProfileURL;         /* com.apple.ColorSync.ProfileURL */
    CSEXTERN CFStringRef kColorSyncProfileDescription; /* com.apple.ColorSync.ProfileDescription */
    CSEXTERN CFStringRef kColorSyncProfileMD5Digest;   /* com.apple.ColorSync.ProfileMD5Digest */
    
    CSEXTERN CFStringRef kColorSyncSigAToB0Tag;                       /* 0x41324230L => CFSTR("A2B0")*/
    CSEXTERN CFStringRef kColorSyncSigAToB1Tag;                       /* 0x41324231L => CFSTR("A2B1")*/
    CSEXTERN CFStringRef kColorSyncSigAToB2Tag;                       /* 0x41324232L => CFSTR("A2B2")*/
    CSEXTERN CFStringRef kColorSyncSigBToA0Tag;                       /* 0x42324130L => CFSTR("B2A0")*/
    CSEXTERN CFStringRef kColorSyncSigBToA1Tag;                       /* 0x42324131L => CFSTR("B2A1")*/
    CSEXTERN CFStringRef kColorSyncSigBToA2Tag;                       /* 0x42324132L => CFSTR("B2A2")*/
    CSEXTERN CFStringRef kColorSyncSigCmykData;                       /* 0x434D594BL => CFSTR("CMYK")*/
    CSEXTERN CFStringRef kColorSyncSigGrayData;                       /* 0x47524159L => CFSTR("GRAY")*/
    CSEXTERN CFStringRef kColorSyncSigLabData;                        /* 0x4C616220L => CFSTR("Lab ")*/
    CSEXTERN CFStringRef kColorSyncSigRgbData;                        /* 0x52474220L => CFSTR("RGB ")*/
    CSEXTERN CFStringRef kColorSyncSigXYZData;                        /* 0x58595A20L => CFSTR("XYZ ")*/
    CSEXTERN CFStringRef kColorSyncSigAbstractClass;                  /* 0x61627374L => CFSTR("abst")*/
    CSEXTERN CFStringRef kColorSyncSigBlueTRCTag;                     /* 0x62545243L => CFSTR("bTRC")*/
    CSEXTERN CFStringRef kColorSyncSigBlueColorantTag;                /* 0x6258595AL => CFSTR("bXYZ")*/
    CSEXTERN CFStringRef kColorSyncSigMediaBlackPointTag;             /* 0x626B7074L => CFSTR("bkpt")*/
    CSEXTERN CFStringRef kColorSyncSigCopyrightTag;                   /* 0x63707274L => CFSTR("cprt")*/
    CSEXTERN CFStringRef kColorSyncSigProfileDescriptionTag;          /* 0x64657363L => CFSTR("desc")*/
    CSEXTERN CFStringRef kColorSyncSigDeviceModelDescTag;             /* 0x646D6464L => CFSTR("dmdd")*/
    CSEXTERN CFStringRef kColorSyncSigDeviceMfgDescTag;               /* 0x646D6E64L => CFSTR("dmnd")*/
    CSEXTERN CFStringRef kColorSyncSigGreenTRCTag;                    /* 0x67545243L => CFSTR("gTRC")*/
    CSEXTERN CFStringRef kColorSyncSigGreenColorantTag;               /* 0x6758595AL => CFSTR("gXYZ")*/
    CSEXTERN CFStringRef kColorSyncSigGamutTag;                       /* 0x67616D74L => CFSTR("gamt")*/
    CSEXTERN CFStringRef kColorSyncSigGrayTRCTag;                     /* 0x6B545243L => CFSTR("kTRC")*/
    CSEXTERN CFStringRef kColorSyncSigLinkClass;                      /* 0x6C696E6BL => CFSTR("link")*/
    CSEXTERN CFStringRef kColorSyncSigDisplayClass;                   /* 0x6D6E7472L => CFSTR("mntr")*/
    CSEXTERN CFStringRef kColorSyncSigNamedColor2Tag;                 /* 0x6E636C32L => CFSTR("ncl2")*/
    CSEXTERN CFStringRef kColorSyncSigNamedColorClass;                /* 0x6E6D636CL => CFSTR("nmcl")*/
    CSEXTERN CFStringRef kColorSyncSigPreview0Tag;                    /* 0x70726530L => CFSTR("pre0")*/
    CSEXTERN CFStringRef kColorSyncSigPreview1Tag;                    /* 0x70726531L => CFSTR("pre1")*/
    CSEXTERN CFStringRef kColorSyncSigPreview2Tag;                    /* 0x70726532L => CFSTR("pre2")*/
    CSEXTERN CFStringRef kColorSyncSigOutputClass;                    /* 0x70727472L => CFSTR("prtr")*/
    CSEXTERN CFStringRef kColorSyncSigProfileSequenceDescTag;         /* 0x70736571L => CFSTR("pseq")*/
    CSEXTERN CFStringRef kColorSyncSigRedTRCTag;                      /* 0x72545243L => CFSTR("rTRC")*/
    CSEXTERN CFStringRef kColorSyncSigRedColorantTag;                 /* 0x7258595AL => CFSTR("rXYZ")*/
    CSEXTERN CFStringRef kColorSyncSigInputClass;                     /* 0x73636E72L => CFSTR("scnr")*/
    CSEXTERN CFStringRef kColorSyncSigColorSpaceClass;                /* 0x73706163L => CFSTR("spac")*/
    CSEXTERN CFStringRef kColorSyncSigTechnologyTag;                  /* 0x74656368L => CFSTR("tech")*/
    CSEXTERN CFStringRef kColorSyncSigViewingConditionsTag;           /* 0x76696577L => CFSTR("view")*/
    CSEXTERN CFStringRef kColorSyncSigViewingCondDescTag;             /* 0x76756564L => CFSTR("vued")*/
    CSEXTERN CFStringRef kColorSyncSigMediaWhitePointTag;             /* 0x77747074L => CFSTR("wtpt")*/
    
    CSEXTERN CFStringRef kColorSyncProfileComputerDomain; /* com.apple.ColorSync.ProfileDomain.Computer */
    CSEXTERN CFStringRef kColorSyncProfileUserDomain;     /* com.apple.ColorSync.ProfileDomain.User */
    
#define COLORSYNC_PROFILE_INSTALL_ENTITLEMENT   "com.apple.developer.ColorSync.profile.install"
    
    CSEXTERN CFTypeID ColorSyncProfileGetTypeID(void);
    /*
     * returns the CFTypeID for ColorSyncProfiles.
     */
    
    CSEXTERN ColorSyncProfileRef ColorSyncProfileCreate(CFDataRef data, CFErrorRef* error);
    /*
     *   data   - profile data
     *   error  - (optional) pointer to the error which will be returned in case of failure
     *
     *   returns ColorSyncProfileRef or NULL in case of failure
     */
    
    CSEXTERN ColorSyncProfileRef ColorSyncProfileCreateWithURL(CFURLRef url, CFErrorRef* error);
    /*
     *   url    - URL to the profile data.
     *   error  - (optional) pointer to the error which will be returned in case of failure
     *
     *   returns ColorSyncProfileRef or NULL in case of failure
     */
    
    CSEXTERN ColorSyncProfileRef ColorSyncProfileCreateWithName(CFStringRef name);
    /*
     *   name    - predefined profile name
     *
     *   returns ColorSyncProfileRef or NULL in case of failure
     */
    
    CSEXTERN ColorSyncProfileRef ColorSyncProfileCreateWithDisplayID (uint32_t displayID);
    /*
     *   displayID - system-wide unique display ID (defined by IOKIt); pass 0 for main display.
     *
     *   returns ColorSyncProfileRef or NULL in case of failure
     */
    
    CSEXTERN ColorSyncProfileRef ColorSyncProfileCreateDeviceProfile (CFStringRef deviceClass, CFUUIDRef deviceID, CFTypeRef profileID);
    /*
     *   deviceClass - ColorSync device class
     *   deviceID    - deviceID registered with ColorSync
     *   profileID   - profileID registered with ColorSync; pass kColorSyncDeviceDefaultProfileID to get the default profile.
     *
     *   See ColorSyncDevice.h for more info on deviceClass, deviceID and profileID
     *
     *   returns ColorSyncProfileRef or NULL in case of failure
     */
    
    CSEXTERN ColorSyncMutableProfileRef ColorSyncProfileCreateMutable(void);
    /*
     *   returns empty ColorSyncMutableProfileRef or NULL in case of failure
     */
    
    CSEXTERN ColorSyncMutableProfileRef ColorSyncProfileCreateMutableCopy(ColorSyncProfileRef prof);
    /*
     *  prof  - profile from which profile data will be copied to the created profile.
     *
     *   returns ColorSyncMutableProfileRef or NULL in case of failure
     */
    
    CSEXTERN ColorSyncProfileRef ColorSyncProfileCreateLink (CFArrayRef profileInfo, CFDictionaryRef options);
    /*
     *   profileInfo  - array of dictionaries, each one containing a profile object and the
     *                       information on the usage of the profile in the transform.
     *
     *               Required keys:
     *               ==============
     *                      kColorSyncProfile           : ColorSyncProfileRef
     *                      kColorSyncRenderingIntent   : CFStringRef defining rendering intent
     *                      kColorSyncTransformTag      : CFStringRef defining which tags to use
     *               Optional key:
     *               =============
     *                    kColorSyncBlackPointCompensation : CFBooleanRef to enable/disable BPC
     *
     *   options      - dictionary with additional public global options (e.g. preferred CMM, quality,
     *                       etc... It can also contain custom options that are CMM specific.
     *
     *   returns ColorSyncProfileRef or NULL in case of failure
     */
    
    CSEXTERN bool ColorSyncProfileVerify (ColorSyncProfileRef prof, CFErrorRef* errors, CFErrorRef* warnings);
    /*
     *   prof    - profile to be verified
     *
     *   errors  - returns error strings in case problems are found which
     *                  would prevent use of the profile.
     *
     *   warnings - returns warning strings indicating problems due to lack of
     *                       conformance with the ICC specification, but not preventing
     *                       use of the profile.
     *
     *   returns true if profile can be used or false otherwise
     */
    
    CSEXTERN bool ColorSyncProfileIsWideGamut (ColorSyncProfileRef);
    /*
     * A utility function estimating gamut of a display profile
     */

    CSEXTERN bool ColorSyncProfileIsMatrixBased (ColorSyncProfileRef);
    /*
     * A utility function verifying if a profile is matrix-based
     */

    CSEXTERN bool ColorSyncProfileIsPQBased (ColorSyncProfileRef);
    /*
     * A utility function verifying if a profile is using ITU BT.2100 PQ transfer functions
     */

    CSEXTERN bool ColorSyncProfileIsHLGBased (ColorSyncProfileRef);
    /*
     * A utility function verifying if a profile is using ITU BT.2100 HLG transfer functions
     */

    CSEXTERN float ColorSyncProfileEstimateGammaWithDisplayID (const int32_t displayID, CFErrorRef* error);
    /*
     *   displayID - system-wide unique display ID (defined by IOKIt)
     *   error     - (optional) pointer to the error which will be returned in
     *               case of failure
     *
     *   returns non-zero value if success or 0.0 in case of error.
     */
    
    CSEXTERN float ColorSyncProfileEstimateGamma (ColorSyncProfileRef prof, CFErrorRef* error);
    /*
     *   prof    - profile to perform estimation on
     *   error   - (optional) pointer to the error which will be returned in
     *             case of failure
     *
     *   returns non-zero value if success or 0.0 in case of error
     */
    
#define COLORSYNC_MD5_LENGTH  16
    
    typedef struct { uint8_t digest[COLORSYNC_MD5_LENGTH]; } ColorSyncMD5;
    
    CSEXTERN ColorSyncMD5 ColorSyncProfileGetMD5(ColorSyncProfileRef prof);
    /*
     *   returns MD5 digest for the profile calculated as defined by
     *           ICC specification or a "zero" signature (filled with zeros)
     *           in case of failure
     */
    
    CSEXTERN CFDataRef ColorSyncProfileCopyData (ColorSyncProfileRef prof, CFErrorRef* error);
    /*
     *   prof    - profile to copy the flattened data from
     *   error  - (optional) pointer to the error which will be returned in case of failure
     *
     *   returns CFDataRef if success or NULL in case of failure
     */
    
    CSEXTERN CFURLRef ColorSyncProfileGetURL (ColorSyncProfileRef prof, CFErrorRef* error);
    /*
     *   prof   - profile to get URL from
     *   error  - (optional) pointer to the error which will be returned in case of failure
     *
     *   returns CFURLRef if success or NULL in case of failure
     */
    
    CSEXTERN CFDataRef ColorSyncProfileCopyHeader (ColorSyncProfileRef prof);
    /*
     *   prof    - profile from which to copy the header
     *
     *   returns CFDataRef containing profile header (in host endianess) or NULL in case of failure
     */
    
    CSEXTERN void ColorSyncProfileSetHeader (ColorSyncMutableProfileRef prof, CFDataRef header);
    /*
     *   prof        - profile in which to set the header
     *   header  - CFDataRef containing the header data (must be in host endianess)
     */
    
    CSEXTERN CFStringRef ColorSyncProfileCopyDescriptionString (ColorSyncProfileRef prof);
    /*
     *   prof    - profile from which to copy description string
     *
     *   returns CFStringRef containing profile description localized to current locale
     */
    
    CSEXTERN CFArrayRef ColorSyncProfileCopyTagSignatures (ColorSyncProfileRef prof);
    /*
     *   prof    - profile from which to copy tag signatures
     *
     *   returns CFArray with signatures (CFStringRef) of tags in the profile
     */
    
    CSEXTERN bool ColorSyncProfileContainsTag (ColorSyncProfileRef prof, CFStringRef signature);
    /*
     *   prof        - profile in which to search for the tag
     *   signature   - signature of the tag to be searched for
     *
     *   returns true if tag exists or false if does not
     */
    
    CSEXTERN CFDataRef ColorSyncProfileCopyTag (ColorSyncProfileRef prof, CFStringRef signature);
    /*
     *   prof             - profile from which to copy the tag
     *   signature   - signature of the tag to be copied
     *
     *   returns CFDataRef containing tag data or NULL in case of failure
     */
    
    CSEXTERN void ColorSyncProfileSetTag (ColorSyncMutableProfileRef prof, CFStringRef signature, CFDataRef data);
    /*
     *   prof           - profile in which to set the tag
     *   signature - signature of the tag to be set in the profile
     *   data          - CFDataRef containing the tag data
     */
    
    CSEXTERN void ColorSyncProfileRemoveTag (ColorSyncMutableProfileRef prof, CFStringRef signature);
    /*
     *   prof              - profile from which to remove the tag
     *   signature    - signature of the tag to be removed
     *
     *   returns true if success or false in case of failure
     */
    
    CSEXTERN bool ColorSyncProfileGetDisplayTransferFormulaFromVCGT(ColorSyncProfileRef profile,
                                                                    float* redMin,   float* redMax,   float* redGamma,
                                                                    float* greenMin, float* greenMax, float* greenGamma,
                                                                    float* blueMin,  float* blueMax,  float* blueGamma);
    /*
     * An utility function converting vcgt tag (if vcgt tag exists in the profile and conversion possible)
     * to formula components used by CGSetDisplayTransferByFormula.
     */
    
    CSEXTERN CFDataRef ColorSyncProfileCreateDisplayTransferTablesFromVCGT(ColorSyncProfileRef profile, size_t *nSamplesPerChannel);
    /*
     * An utility function creating three tables of floats (redTable, greenTable, blueTable)
     * each of size nSamplesPerChannel, packed into contiguous memory contained in the CFDataRef
     *  to be returned from the vcgt tag of the profile (if vcgt tag exists in the profile).
     * Used by CGSetDisplayTransferByTable.
     */
    
    
    typedef bool (*ColorSyncProfileIterateCallback)(CFDictionaryRef profileInfo, void* userInfo);
    /*
     * Notes:
     *   1. Only validated profiles will be passed to the caller
     *   2. if the ColorSyncProfileIterateCallback returns false, the iteration stops
     */
    
    #define COLORSYNC_ITERATE_ASYNC

    CSEXTERN CFStringRef kColorSyncProfileCacheSeed;
    /*
     * Current cache seed (uint32_t) sent along with kColorSyncProfileRepositoryChangeNotification
     */

    CSEXTERN void ColorSyncIterateInstalledProfiles (ColorSyncProfileIterateCallback __nullable callBack,
                                                     uint32_t*                       __nullable seed,
                                                     void*                           __nullable userInfo,
                                                     CFErrorRef*                     __nullable error);
   /*
    * callBack - pointer to a client provided function (can be NULL)
    * seed     - pointer to a cache seed owned by the client (can be NULL)
    * error    - (optional) pointer to the error which will be returned in case of failure
    *
    * Note: When called for the first time this function will return only system profiles because
    * profile iteration is a slow process requiring multiple access to file system.
    * Clients are advised to register for kColorSyncProfileRepositoryChangeNotification
    * using CFNotificationCenter or NSNotificationCenter to obtain all installed profiles.
    * Notification callback will receive, if possible, a CFDictionary containing a new seed
    * value of the profile cache (key kColorSyncProfileCacheSeed / value kCFNumberSInt32Type).
    * Comparing old and new seed values may prevent unnecessary calls to iterate installed
    * profiles.
    * Alternatively, ColorSyncIterateInstalledProfilesWithOptions can be used with kColorSyncWaitForCacheReply
    * option set to kCFBooleanTrue to obtain all installed profiles in one call.
    */

    CSEXTERN CFStringRef kColorSyncWaitForCacheReply;

    CSEXTERN void ColorSyncIterateInstalledProfilesWithOptions (ColorSyncProfileIterateCallback __nullable callBack,
                                                                uint32_t*                       __nullable seed,
                                                                void*                           __nullable userInfo,
                                                                CFDictionaryRef                 __nullable options,
                                                                CFErrorRef*                     __nullable error);
   /*
    * callBack - pointer to a client provided function (can be NULL)
    * seed     - pointer to a cache seed owned by the client
    * options  = a dictionary with iteration options, e.g. kColorSyncWaitForCacheReply to skip updates
    * error    - (optional) pointer to the error that will be returned in case of failure
    *
    */

    CSEXTERN bool ColorSyncProfileInstall(ColorSyncProfileRef profile, CFStringRef domain, CFStringRef subpath, CFErrorRef* error);
    /*
     * profile   - profile to be installed
     * domain    - either kColorSyncProfileComputerDomain or kColorSyncProfileUserDomain.
     *             kColorSyncProfileComputerDomain is for sharing the profiles (from /Library/ColorSync/Profiles).
     *             kColorSyncProfileUserDomain is for user custom profiles (installed under home directory, i.e. in
     *             ~/Library/ColorSync/Profiles.
     *             NULL is the same as kColorSyncProfileUserDomain.
     * subpath   - CFString created from the file system representation of the path of the file to contain the installed
     *             profile. The last component of the path is interpreted as a file name if it ends with the extension ".icc".
     *             Otherwise, the subpath is interpreted as the directory path and file name will be created from the
     *             profile description tag, appended with the ".icc" extension.
     * error     - (optional) pointer to the error which will be returned in case of failure.
     *
     *             bool value true is returned if success or false in case of error.
     */
    
    CSEXTERN bool ColorSyncProfileUninstall(ColorSyncProfileRef profile, CFErrorRef* error);
    /*
     * profile   - profile to be uninstalled. This profile must return a valid url for ColorSyncProfileGetURL,
     *             i.e. it must be created with ColorSyncProfileCreateWithURL. Also, the url
     *             must be in either in kColorSyncProfileComputerDomain or
     *             kColorSyncProfileUserDomain, including subfolders of those.
     * error     - (optional) pointer to the error which will be returned in case of failure.
     *
     *             bool value true is returned if success or false in case of error.
     */
    
    /********************************************************************************************
     *                                                                                           *
     *   For further information on ICC profiles refer to ICC profile specification published    *
     *   by International Color Consortium at www.color.org                                      *
     *                                                                                           *
     *   ICC header file (ICC.h) containing programmatic definitions of ICC data types can be    *
     *   downloaded from the Technical Notes web page of ICC website at                          *
     *   www.color.org/technotes.xalter                                                          *
     *                                                                                           *
     ********************************************************************************************/
    
#ifdef __cplusplus
}
#endif

#endif /* #if !defined(__swift__) */

#endif /* __COLORSYNCPROFILE__ */
