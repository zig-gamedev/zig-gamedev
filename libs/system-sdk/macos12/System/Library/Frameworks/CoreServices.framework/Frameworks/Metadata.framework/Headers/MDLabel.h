/*
 *       MDLabel.h
 *       Copyright (c) 2009-2019, Apple Inc. All rights reserved.
 */

/*!
 @header MDLabel
 @abstract An MDLabelRef represents a label which may be applied to a file for searches.
*/

#if !defined(__MDLABEL__)
#define __MDLABEL__ 1

#include <CoreFoundation/CoreFoundation.h>

#include <CoreFoundation/CoreFoundation.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7

#include <Metadata/MDItem.h>
#include <Metadata/MDQuery.h>

CF_IMPLICIT_BRIDGING_ENABLED
MD_BEGIN_C_DECLS

/*!
 @typedef MDLabelRef
 @abstract This is the type of a reference to an MDLabel.
 */
typedef struct __MDLabel * MDLabelRef;

MD_EXPORT CFTypeID MDLabelGetTypeID(void)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/* MDItem API additions for Labels */

/*!
 @function MDItemCopyLabels
 @abstract Returns an array of the labels set on the specified item.
 @param item The item to be interrogated.
 @result A CFArrayRef containing MDLabelRefs for the labels set on the item, or NULL on failure.
 */
MD_EXPORT CFArrayRef MDItemCopyLabels(MDItemRef item)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDItemSetLabel
 @abstract Sets a label on the specified item.
 @param item The item to be updated.
 @param label The label.
 @result True if the label was successfully set on the item, false otherwise.
 */
MD_EXPORT Boolean MDItemSetLabel(MDItemRef item, MDLabelRef label)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDItemRemoveLabel
 @abstract Removes a label from the specified item.
 @param item The item to be updated.
 @param label The label.
 @result True if the label was successfully removed from the item, false otherwise.
 */
MD_EXPORT Boolean MDItemRemoveLabel(MDItemRef item, MDLabelRef label)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @typedef MDLabelDomain
 @abstract These constants are used to specify a domain to MDLabelCreate().
 */
typedef enum {
    kMDLabelUserDomain,   // ~/Library/Metadata/
    kMDLabelLocalDomain,  // /Library/Metadata/
} MDLabelDomain;

/* MDLabel APIs */

/*!
 @function MDLabelCreate
 @abstract Returns a label with the specified parameters. If there is already a label that exactly matches the parameters, a reference to the existing label will be returned; otherwise this will attempt to create a new label.  A successful creation of a new private label definition will generate a kMDLabelAddedNotification. Note that this function can only create labels with "Private" visibility. Creating "Public" labels requires creating and installing a label bundle.
 @param allocator The CFAllocator which should be used to allocate memory for the label. This parameter may be NULL in which case the current default CFAllocator is used. Use kCFAllocatorNull to request a reference to an existing label only.
 @param displayName The label's display name.
 @param kind The label's kind string.
 @param domain The domain of the label (normally kMDLabelUserDomain).
 @result An MDLabelRef, or NULL on failure.
 */
MD_EXPORT MDLabelRef MDLabelCreate(CFAllocatorRef allocator,
                                   CFStringRef displayName,
                                   CFStringRef kind,
                                   MDLabelDomain domain)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDLabelCopyAttribute
 @abstract Copy the value of the named attribute of a label.
 @param label The label.
 @param name The name of the desired attribute.
 @result A CFTypeRef, or NULL on failure, or if the attribute does not exist, or if the attribute is not readable.
 */
MD_EXPORT CFTypeRef MDLabelCopyAttribute(MDLabelRef label,
                                         CFStringRef name)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDLabelCopyAttributeName
 @abstract Copy the MDItem attribute name of a label. The attribute name can be used in an MDQuery string to search for MDItems that have the label set. The value of the MDItem attribute is a CFDate corresponding to the time the label was set on the item.
 @param label The label.
 @result A CFStringRef, or NULL on failure.
 */
MD_EXPORT CFStringRef MDLabelCopyAttributeName(MDLabelRef label)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDLabelDelete
 @abstract Deletes the user's definition or override of the specified label from ~/Library/Metadata. Labels defined in bundles elsewhere in the filesystem cannot be deleted using this API. Deleting an override of a label merely makes the original definition of the label visible again and thus will generate a kMDLabelChangedNotification. A successful deletion of a private label definition will generate a kMDLabelRemovedNotification.
 @param label The label.
 @result True if a label definition or override was successfully deleted.
 */
MD_EXPORT Boolean MDLabelDelete(MDLabelRef label)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDLabelSetAttributes
 @abstract Updates the attributes of the specified label. Labels defined in ~/Library/Metadata are modified directly. Labels defined in bundles elsewhere in the filesystem are overridden by creating a private copy of the label definition in ~/Library/Metadata. The updated attributes are then stored in the private copy. A successful call to MDLabelSetAttributes() will generate a kMDLabelChangedNotification.
 @param label The label.
 @param attrs A dictionary containing the attributes to be modified. To remove an attribute, include it in the dictionary with kCFNull as its value.
 @result True if the label definition or override was successfully updated.
 */
MD_EXPORT Boolean MDLabelSetAttributes(MDLabelRef label,
                                       CFDictionaryRef attrs)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDCopyLabelKinds
 @abstract Copy the list of label kind strings.
 @result A CFArrayRef containing all of the label kind strings, or NULL on failure.
 */
MD_EXPORT CFArrayRef MDCopyLabelKinds(void)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDCopyLabelsMatchingExpression
 @abstract Copy the list of labels matching the specified query expression.
 @param simpleQueryString The query expression string.
 @result A CFArrayRef containing all of the matching labels, or NULL on failure.
 */
MD_EXPORT CFArrayRef MDCopyLabelsMatchingExpression(CFStringRef simpleQueryString)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDCopyLabelsWithKind
 @abstract Copy the list of labels with the specified kind string.
 @param kind The kind string, or NULL to copy all labels.
 @result A CFArrayRef containing all of the labels with the specified kind string, or NULL on failure.
 */
MD_EXPORT CFArrayRef MDCopyLabelsWithKind(CFStringRef kind)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDCopyLabelWithUUID
 @abstract Copy the label with the specified UUID.
 @param labelUUID The label UUID.
 @result An MDLabelRef, or NULL on failure.
 */
MD_EXPORT MDLabelRef MDCopyLabelWithUUID(CFUUIDRef labelUUID)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @constant kMDLabelBundleURL
 @abstract This is the bundle URL for the label.
 Type is a CFURL. This attribute is read-only.
 
 @constant kMDLabelContentChangeDate
 @abstract This is the date the the label content last changed.
 Type is a CFDate. This attribute is read-only, but its value will be altered by setting any of the writable attributes described below.
 
 @constant kMDLabelDisplayName
 @abstract This is the localized name of the label.
 Type is a CFString.
 
 @constant kMDLabelIconData
 @abstract This is the data that should be used to create an icon image source for the label. Pass this data to the ImageIO framework to create a CGImageSourceRef. To reset a label's icon to its default image, set kMDLabelIconData to kCFNull.
 Type is a CFData.
 
 @constant kMDLabelIconUUID
 @abstract This is the UUID of the icon image source for the label. Labels that share the same icon image source will have the same icon UUID.
 Type is a CFUUID. This attribute is read-only, but its value may be altered by setting the kMDLabelIconData attribute described above.
 
 @constant kMDLabelIsMutuallyExclusiveSetMember
 @abstract This attribute is true if the label kind specifies a mutually-exclusive set of labels.
 Type is a CFBoolean. This attribute is read-only.

 @constant kMDLabelKind
 @abstract This is the kind string for the label. The label kind is the name of the bundle the label is defined in.
 Type is a CFString. This attribute is read-only.
 
 @constant kMDLabelSetsFinderColor
 @abstract This attribute is optional and applicable only to labels with "Public" visibility. If it is present, then when the label is set on a file the file's Finder color will be set to the specified value (0 - 7). If the label is later removed, the file's Finder color will be reverted either to the color specified by the most-recently-set label with this attribute that remains set on the file, or to 0 (none).
 Type is a CFNumber. This attribute is ignored for unless the label's visibility is "Public".
 
 @constant kMDLabelUUID
 @abstract This is the UUID of the label.
 Type is a CFUUID. This attribute is read-only.
 
 @constant kMDLabelVisibility
 @abstract This is a constant describing the label's visibility, either "Public" (kMDPublicVisibility) or "Private" (kMDPrivateVisibility).
 Type is a CFString. This attribute is read-only.
 */
MD_EXPORT CFStringRef kMDLabelBundleURL                    // CFURL
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelContentChangeDate            // CFDate
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelDisplayName                  // CFString
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelIconData                     // CFData (kUTTypeAppleICNS)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelIconUUID                     // CFUUID
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelIsMutuallyExclusiveSetMember // CFBoolean
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelKind                         // CFString
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelSetsFinderColor              // CFNumber (0 - 7)
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelUUID                         // CFUUID
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelVisibility                   // CFString
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @constant kMDLabelKindIsMutuallyExclusiveSetKey
 @abstract This key is used in the Info.plist file of a label bundle to specify that the label kind constitutes a mutually exclusive set.
 The value is a CFBoolean.
 
 @constant kMDLabelKindVisibilityKey
 @abstract This key is used in the Info.plist file of a label bundle to specify the visibility of the labels defined by the bundle.
 The value is a CFString constant, either "Public" (kMDPublicVisibility) or "Private" (kMDPrivateVisibility).
 */
MD_EXPORT CFStringRef kMDLabelKindIsMutuallyExclusiveSetKey // Boolean
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDLabelKindVisibilityKey             // "Private" (the default) or "Public"
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDPrivateVisibility                  // "Private"
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_EXPORT CFStringRef kMDPublicVisibility                   // "Public"
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @constant kMDLabelAddedNotification
 @abstract The name of the notification sent when a label has been added. The notification object is the subject MDLabelRef. All label notifications are distributed to processes owned by the same uid that have initialized the Metadata framework label APIs.
 */
MD_EXPORT const CFStringRef kMDLabelAddedNotification
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @constant kMDLabelChangedNotification
 @abstract The name of the notification sent when a label has been changed. The notification object is the subject MDLabelRef. The label's new attributes can be retrieved using MDLabelCopyAttribute().
 */
MD_EXPORT const CFStringRef kMDLabelChangedNotification
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @constant kMDLabelRemovedNotification
 @abstract The name of the notification sent when a label has been deleted. The notification object is the subject MDLabelRef. The label's kMDLabelIconUUID, kMDLabelKind, kMDLabelKindBundleURL and kMDLabelUUID attributes can still be retrieved using MDLabelCopyAttribute(), and the label may still be passed to MDLabelCopyAttributeName().
 */
MD_EXPORT const CFStringRef kMDLabelRemovedNotification
API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

MD_END_C_DECLS
CF_IMPLICIT_BRIDGING_DISABLED

#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */

#endif /* __MDLABEL__ */
