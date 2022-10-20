/*
 *       MDSchema.h
 *       Copyright (c) 2003-2019, Apple Inc. All rights reserved.
 */

/*!
	@header MDSchema

	Functions in MDSchema return meta data about attributes, for example
        the type of an attribute, and a localized string for an attribute that is
        sutable to display to a user.
*/

#if !defined(__MDSCHEMA__)
#define __MDSCHEMA__ 1

#include <CoreFoundation/CoreFoundation.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4

#include <Metadata/MDItem.h>

CF_IMPLICIT_BRIDGING_ENABLED
MD_BEGIN_C_DECLS

/*!
	@function MDSchemaCopyAttributesForContentType
        Returns an dictionary attributes to display or show the
                user for a given UTI type. This function does not walk up the
                UTI hiearchy and perform a union of the information.
        @param utiType the UTI type to be interrogated.
        @result A CFDictionaryRef with keys ==  to kMDAttributeDisplayValues etc..

*/
MD_EXPORT CFDictionaryRef MDSchemaCopyAttributesForContentType(CFStringRef contentTypeUTI) MD_AVAIL;

/*!
        @function MDSchemaCopyMetaAttributesForAttribute
        Returns an dictionary of the meta attributes of attribute
        @param name the attribute whose schema you are interested in.
        @result A CFDictionary of the description of the attribute.
*/
MD_EXPORT CFDictionaryRef MDSchemaCopyMetaAttributesForAttribute(CFStringRef name) MD_AVAIL;

/*!
        @function MDSchemaCopyAllAttributes
        Returns an array of all of the attributes defined in the schema
        @result A CFArray of the attribute names.
*/
MD_EXPORT CFArrayRef MDSchemaCopyAllAttributes(void) MD_AVAIL;

/*!
        @function MDSchemaCopyDisplayNameForAttribute
        Returns the localized name of an attribute
        @param name the attribute whose localization you are interested in
        @result the localized name of the passed in attribute, or NULL if there is
                 no localized name is avaliable.
*/
MD_EXPORT CFStringRef MDSchemaCopyDisplayNameForAttribute(CFStringRef name) MD_AVAIL;

/*!
        @function MDSchemaCopyDisplayDescriptionForAttribute
        Returns the localized description of an attribute.
        @param name the attribute whose localization you are interested in
        @result the localized description of the passed in attribute, or NULL if there is
                 no localized description is avaliable.
*/
MD_EXPORT CFStringRef MDSchemaCopyDisplayDescriptionForAttribute(CFStringRef name) MD_AVAIL;

    /* Returned by MDSchemaCopyAttributesForContentType */
MD_EXPORT const CFStringRef kMDAttributeDisplayValues MD_AVAIL;
                                                                /* Value == CFArray of CFString attribute names  or
                                                                 * NULL if the type  is not known by the system
                                                                 */
MD_EXPORT const CFStringRef kMDAttributeAllValues MD_AVAIL;     /* Value == CFArray of CFString attribute names  or
                                                                 * NULL if the type  is not known by the system
                                                                 */

MD_EXPORT const CFStringRef kMDAttributeReadOnlyValues MD_AVAIL_LEOPARD;
                                                                 /* Value == CFArray of CFString attribute names  or
                                                                  * NULL if the type  is not known by the system
                                                                  */

MD_EXPORT const CFStringRef kMDExporterAvaliable MD_AVAIL_LEOPARD;
                                                                 /* Value == CFBoolean
                                                                  * indicates if an exporter is avaliable for this
                                                                  * uti type
                                                                  */

/* Keys in the dictionary returned from the MDSchemaCopyMetaAttributesForAttribute call */
MD_EXPORT const CFStringRef  kMDAttributeName MD_AVAIL;           /* Value == name of attribute (CFStringRef) */
MD_EXPORT const CFStringRef  kMDAttributeType MD_AVAIL;           /* Value == type of Attribute (CFNumberRef, CFTypeID) */
MD_EXPORT const CFStringRef  kMDAttributeMultiValued MD_AVAIL;    /* Value == if multivalued (CFBooleanRef) */

MD_END_C_DECLS
CF_IMPLICIT_BRIDGING_DISABLED

#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */

#endif /* __MDSCHEMA__ */
