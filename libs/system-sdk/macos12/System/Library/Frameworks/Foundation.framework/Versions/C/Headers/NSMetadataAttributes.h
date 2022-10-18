/*	NSMetadataAttributes.h
        Copyright (c) 2004-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSObject.h>

@class NSString;

NS_ASSUME_NONNULL_BEGIN

// The following NSMetadataItem attributes are available on Mac OS and iOS.

FOUNDATION_EXPORT NSString * const NSMetadataItemFSNameKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemDisplayNameKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemURLKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // NSURL
FOUNDATION_EXPORT NSString * const NSMetadataItemPathKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemFSSizeKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // file size in bytes; unsigned long long NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemFSCreationDateKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemFSContentChangeDateKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemContentTypeKey API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0)); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemContentTypeTreeKey API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0)); // NSArray of NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemIsUbiquitousKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // boolean NSNumber

FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemHasUnresolvedConflictsKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemIsDownloadedKey API_DEPRECATED("Use NSMetadataUbiquitousItemDownloadingStatusKey instead", macos(10.7,10.9), ios(5.0,7.0), watchos(2.0,2.0), tvos(9.0,9.0)); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemDownloadingStatusKey API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // NSString ; download status of this item. The values are the three strings defined below:
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemDownloadingStatusNotDownloaded  API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // this item has not been downloaded yet. Use startDownloadingUbiquitousItemAtURL:error: to download it.
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemDownloadingStatusDownloaded     API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // there is a local version of this item available. The most current version will get downloaded as soon as possible.
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemDownloadingStatusCurrent        API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // there is a local version of this item and it is the most up-to-date version known to this device.
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemIsDownloadingKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemIsUploadedKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemIsUploadingKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemPercentDownloadedKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // double NSNumber; range [0..100]
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemPercentUploadedKey API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // double NSNumber; range [0..100]
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemDownloadingErrorKey API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // NSError; the error when downloading the item from iCloud failed, see the NSUbiquitousFile section in FoundationErrors.h
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemUploadingErrorKey API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)); // NSError; the error when uploading the item to iCloud failed, see the NSUbiquitousFile section in FoundationErrors.h
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemDownloadRequestedKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemIsExternalDocumentKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemContainerDisplayNameKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemURLInLocalContainerKey API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)); // NSURL

FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousItemIsSharedKey API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // true if the ubiquitous item is shared. (value type boolean NSNumber)
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemCurrentUserRoleKey API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // returns the current user's role for this shared item, or nil if not shared. (value type NSString). Possible values below.
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemCurrentUserPermissionsKey API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // returns the permissions for the current user, or nil if not shared. (value type NSString). Possible values below.
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemOwnerNameComponentsKey API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // returns a NSPersonNameComponents, or nil if the current user. (value type NSPersonNameComponents)
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemMostRecentEditorNameComponentsKey API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // returns a NSPersonNameComponents for the most recent editor of the document, or nil if it is the current user. (Read-only, value type NSPersonNameComponents)

/* The values returned for the NSMetadataUbiquitousSharedItemCurrentUserRoleKey
 */
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemRoleOwner API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // the current user is the owner of this shared item.
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemRoleParticipant API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // the current user is a participant of this shared item.

/* The values returned for the NSMetadataUbiquitousSharedItemCurrentUserPermissionsKey
 */
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemPermissionsReadOnly API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // the current user is only allowed to read this item
FOUNDATION_EXPORT NSString * const NSMetadataUbiquitousSharedItemPermissionsReadWrite API_AVAILABLE(macosx(10.12), ios(10.0)) API_UNAVAILABLE(watchos, tvos); // the current user is allowed to both read and write this item


// The following NSMetadataItem attributes are available on Mac OS for non-ubiquitious items only. The constants are equal to the corresponding ones in <Metadata/MDItem.h>.

FOUNDATION_EXPORT NSString * const NSMetadataItemAttributeChangeDateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemKeywordsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemTitleKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAuthorsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemEditorsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemParticipantsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemProjectsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemDownloadedDateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemWhereFromsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemCommentKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemCopyrightKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemLastUsedDateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemContentCreationDateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemContentModificationDateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemDateAddedKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemDurationSecondsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemContactKeywordsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemVersionKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemPixelHeightKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemPixelWidthKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemPixelCountKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemColorSpaceKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemBitsPerSampleKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemFlashOnOffKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemFocalLengthKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemAcquisitionMakeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAcquisitionModelKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemISOSpeedKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemOrientationKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemLayerNamesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemWhiteBalanceKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemApertureKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemProfileNameKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemResolutionWidthDPIKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemResolutionHeightDPIKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemExposureModeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemExposureTimeSecondsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemEXIFVersionKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemCameraOwnerKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemFocalLength35mmKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemLensModelKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemEXIFGPSVersionKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAltitudeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemLatitudeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemLongitudeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemSpeedKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemTimestampKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSTrackKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemImageDirectionKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemNamedLocationKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemGPSStatusKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSMeasureModeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSDOPKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSMapDatumKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSDestLatitudeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSDestLongitudeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSDestBearingKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSDestDistanceKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSProcessingMethodKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSAreaInformationKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSDateStampKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemGPSDifferentalKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber

FOUNDATION_EXPORT NSString * const NSMetadataItemCodecsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemMediaTypesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemStreamableKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemTotalBitRateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemVideoBitRateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemAudioBitRateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemDeliveryTypeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemAlbumKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemHasAlphaChannelKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemRedEyeOnOffKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemMeteringModeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemMaxApertureKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemFNumberKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemExposureProgramKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemExposureTimeStringKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemHeadlineKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemInstructionsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemCityKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemStateOrProvinceKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemCountryKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemTextContentKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemAudioSampleRateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemAudioChannelCountKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemTempoKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemKeySignatureKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemTimeSignatureKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAudioEncodingApplicationKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemComposerKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemLyricistKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAudioTrackNumberKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemRecordingDateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemMusicalGenreKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemIsGeneralMIDISequenceKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemRecordingYearKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber

FOUNDATION_EXPORT NSString * const NSMetadataItemOrganizationsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemLanguagesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemRightsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemPublishersKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemContributorsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemCoverageKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemSubjectKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemThemeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemDescriptionKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemIdentifierKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAudiencesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings

FOUNDATION_EXPORT NSString * const NSMetadataItemNumberOfPagesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemPageWidthKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemPageHeightKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemSecurityMethodKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemCreatorKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemEncodingApplicationsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemDueDateKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSDate
FOUNDATION_EXPORT NSString * const NSMetadataItemStarRatingKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemPhoneNumbersKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemEmailAddressesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemInstantMessageAddressesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemKindKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemRecipientsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemFinderCommentKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemFontsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemAppleLoopsRootKeyKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAppleLoopsKeyFilterTypeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAppleLoopsLoopModeKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAppleLoopDescriptorsKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSStrings
FOUNDATION_EXPORT NSString * const NSMetadataItemMusicalInstrumentCategoryKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemMusicalInstrumentNameKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemCFBundleIdentifierKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemInformationKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemDirectorKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemProducerKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemGenreKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemPerformersKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemOriginalFormatKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemOriginalSourceKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAuthorEmailAddressesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemRecipientEmailAddressesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemAuthorAddressesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemRecipientAddressesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemIsLikelyJunkKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // boolean NSNumber
FOUNDATION_EXPORT NSString * const NSMetadataItemExecutableArchitecturesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemExecutablePlatformKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSString
FOUNDATION_EXPORT NSString * const NSMetadataItemApplicationCategoriesKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // NSArray of NSString

FOUNDATION_EXPORT NSString * const NSMetadataItemIsApplicationManagedKey API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // boolean NSNumber

NS_ASSUME_NONNULL_END
