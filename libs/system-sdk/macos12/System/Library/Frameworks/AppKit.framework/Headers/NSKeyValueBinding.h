/*
	NSKeyValueBinding.h
	Application Kit
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
 */

#import <AppKit/AppKitDefines.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <CoreData/NSManagedObjectContext.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSString, NSError, NSAttributeDescription;

typedef NSString * NSBindingName NS_TYPED_EXTENSIBLE_ENUM;
typedef NSString * NSBindingOption NS_TYPED_ENUM;

API_AVAILABLE(macos(10.14))
@interface NSBindingSelectionMarker : NSObject <NSCopying>
// Subclassing or instantiating NSBindingSelectionMarker is not supported.
- (instancetype)init NS_UNAVAILABLE;

@property (class, readonly, strong) NSBindingSelectionMarker *multipleValuesSelectionMarker API_AVAILABLE(macos(10.14));
@property (class, readonly, strong) NSBindingSelectionMarker *noSelectionMarker API_AVAILABLE(macos(10.14));
@property (class, readonly, strong) NSBindingSelectionMarker *notApplicableSelectionMarker API_AVAILABLE(macos(10.14));

+ (void)setDefaultPlaceholder:(nullable id)placeholder forMarker:(nullable NSBindingSelectionMarker *)marker onClass:(Class)objectClass withBinding:(NSBindingName)binding API_AVAILABLE(macos(10.15)); // marker can be nil or one of multipleValuesSelectionMarker, noSelectionMarker, notApplicableSelectionMarker
+ (nullable id)defaultPlaceholderForMarker:(nullable NSBindingSelectionMarker *)marker onClass:(Class)objectClass withBinding:(NSBindingName)binding API_AVAILABLE(macos(10.15)); // marker can be nil or one of multipleValuesSelectionMarker, noSelectionMarker, notApplicableSelectionMarker

@end

APPKIT_EXTERN id NSMultipleValuesMarker API_DEPRECATED_WITH_REPLACEMENT("NSBindingSelectionMarker.multipleValuesSelectionMarker", macos(10.0, 11.0));
APPKIT_EXTERN id NSNoSelectionMarker API_DEPRECATED_WITH_REPLACEMENT("NSBindingSelectionMarker.noSelectionMarker", macos(10.0, 11.0));
APPKIT_EXTERN id NSNotApplicableMarker API_DEPRECATED_WITH_REPLACEMENT("NSBindingSelectionMarker.notApplicableSelectionMarker", macos(10.0, 11.0));

APPKIT_EXTERN BOOL NSIsControllerMarker(_Nullable id object);

// keys for the returned dictionary of -infoForBinding:
typedef NSString * NSBindingInfoKey NS_TYPED_ENUM;
APPKIT_EXTERN NSBindingInfoKey NSObservedObjectKey;
APPKIT_EXTERN NSBindingInfoKey NSObservedKeyPathKey;
APPKIT_EXTERN NSBindingInfoKey NSOptionsKey;

@interface NSObject (NSKeyValueBindingCreation)

+ (void)exposeBinding:(NSBindingName)binding;    // bindings specified here will be exposed automatically in -exposedBindings (unless implementations explicitly filter them out, for example in subclasses)
@property (readonly, copy) NSArray<NSBindingName> *exposedBindings;   // for a new key exposed through this method, the default implementation simply falls back to key-value coding
- (nullable Class)valueClassForBinding:(NSBindingName)binding;    // optional - mostly for matching transformers

/* Bindings are considered to be a property of the object which is bound (the object the following two methods are sent to) and all information related to bindings should be retained by the object; all standard bindings on AppKit objects (views, cells, table columns, controllers) unbind their bindings automatically when they are released, but if you create key-value bindings for other kind of objects, you need to make sure that you remove those bindings when you release them (observed objects don't retain their observers, so controllers/model objects might continue referencing and messaging the objects that was bound to them).
*/
- (void)bind:(NSBindingName)binding toObject:(id)observable withKeyPath:(NSString *)keyPath options:(nullable NSDictionary<NSBindingOption, id> *)options;    // placeholders and value transformers are specified in options dictionary
- (void)unbind:(NSBindingName)binding;

/* Returns a dictionary with information about a binding or nil if the binding is not bound (this is mostly for use by subclasses which want to analyze the existing bindings of an object) - the dictionary contains three key/value pairs: NSObservedObjectKey: object bound, NSObservedKeyPathKey: key path bound, NSOptionsKey: dictionary with the options and their values for the bindings.
*/
- (nullable NSDictionary<NSBindingInfoKey, id> *)infoForBinding:(NSBindingName)binding;

/* Returns an array of NSAttributeDescriptions that describe the options for aBinding. The descriptions are used by Interface Builder to build the options editor UI of the bindings inspector. Each binding may have multiple options. The options and attribute descriptions have 3 properties in common: 

- The option "name" is derived from the attribute description name. 

- The type of UI built for the option is based on the attribute type. 

- The default value shown in the options editor comes from the attribute description's defaultValue.*/


- (NSArray<NSAttributeDescription *> *)optionDescriptionsForBinding:(NSBindingName)binding API_AVAILABLE(macos(10.5));


@end

@interface NSObject (NSPlaceholders)

+ (void)setDefaultPlaceholder:(nullable id)placeholder forMarker:(nullable id)marker withBinding:(NSBindingName)binding API_DEPRECATED_WITH_REPLACEMENT("+[NSBindingSelectionMarker setDefaultPlaceholder:forMarker:onClass:withBinding:]", macos(10.0, 11.0));
+ (nullable id)defaultPlaceholderForMarker:(nullable id)marker withBinding:(NSBindingName)binding API_DEPRECATED_WITH_REPLACEMENT("+[NSBindingSelectionMarker defaultPlaceholderForMarker:onClass:withBinding:]", macos(10.0, 11.0));

@end

// methods implemented by controllers, CoreData's managed object contexts, and user interface elements
@protocol NSEditor <NSObject>

- (void)discardEditing;    // forces changing to end (reverts back to the original value)
- (BOOL)commitEditing;    // returns whether end editing was successful (while trying to apply changes to a model object, there might be validation problems or so that prevent the operation from being successful


/* Given that the receiver has been registered with -objectDidBeginEditing: as the editor of some object, and not yet deregistered by a subsequent invocation of -objectDidEndEditing:, attempt to commit the result of the editing. When committing has either succeeded or failed, send the selected message to the specified object with the context info as the last parameter. The method selected by didCommitSelector must have the same signature as:

    - (void)editor:(id)editor didCommit:(BOOL)didCommit contextInfo:(void *)contextInfo;

If an error occurs while attempting to commit, because key-value coding validation fails for example, an implementation of this method should typically send the NSView in which editing is being done a -presentError:modalForWindow:delegate:didRecoverSelector:contextInfo: message, specifying the view's containing window.
*/
- (void)commitEditingWithDelegate:(nullable id)delegate didCommitSelector:(nullable SEL)didCommitSelector contextInfo:(nullable void *)contextInfo;


/* During autosaving, commit editing may fail, due to a pending edit.  Rather than interrupt the user with an unexpected alert, this method provides the caller with the option to either present the error or fail silently, leaving the pending edit in place and the user's editing uninterrupted.  This method attempts to commit editing, but if there is a failure the error is returned to the caller to be presented or ignored as appropriate.  If an error occurs while attempting to commit, an implementation of this method should return NO as well as the generated error by reference.  Returns YES if commit is successful.
 
 If you have enabled autosaving in your application, and your application has custom objects that implement or override the NSEditor protocol, you must also implement this method in those NSEditors.
 */
- (BOOL)commitEditingAndReturnError:(NSError **)error API_AVAILABLE(macos(10.7));

@end

// methods implemented by controllers, CoreData's managed object contexts (and potentially documents)
@protocol NSEditorRegistration <NSObject>
@optional
- (void)objectDidBeginEditing:(id<NSEditor>)editor;
- (void)objectDidEndEditing:(id<NSEditor>)editor;
@end

#if __swift__ < 40200
@interface NSObject (NSEditor)
- (void)discardEditing API_DEPRECATED("This is now a method of the NSEditor protocol.", macos(10.0, 11.0));
- (BOOL)commitEditing API_DEPRECATED("This is now a method of the NSEditor protocol.", macos(10.0, 11.0));
- (void)commitEditingWithDelegate:(nullable id)delegate didCommitSelector:(nullable SEL)didCommitSelector contextInfo:(nullable void *)contextInfo API_DEPRECATED("This is now a method of the NSEditor protocol.", macos(10.0, 11.0));
- (BOOL)commitEditingAndReturnError:(NSError **)error API_DEPRECATED("This is now a method of the NSEditor protocol.", macos(10.7, 11.0));
@end

@interface NSObject (NSEditorRegistration)
- (void)objectDidBeginEditing:(id<NSEditor>)editor API_DEPRECATED("This is now a method of the NSEditorRegistration protocol.", macos(10.0, 11.0));
- (void)objectDidEndEditing:(id<NSEditor>)editor API_DEPRECATED("This is now a method of the NSEditorRegistration protocol.", macos(10.0, 11.0));
@end
#endif

// constants for binding names
APPKIT_EXTERN NSBindingName NSAlignmentBinding;
APPKIT_EXTERN NSBindingName NSAlternateImageBinding;
APPKIT_EXTERN NSBindingName NSAlternateTitleBinding;
APPKIT_EXTERN NSBindingName NSAnimateBinding;
APPKIT_EXTERN NSBindingName NSAnimationDelayBinding;
APPKIT_EXTERN NSBindingName NSArgumentBinding;
APPKIT_EXTERN NSBindingName NSAttributedStringBinding;
APPKIT_EXTERN NSBindingName NSContentArrayBinding;
APPKIT_EXTERN NSBindingName NSContentArrayForMultipleSelectionBinding;
APPKIT_EXTERN NSBindingName NSContentBinding;
APPKIT_EXTERN NSBindingName NSContentDictionaryBinding		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingName NSContentHeightBinding;
APPKIT_EXTERN NSBindingName NSContentObjectBinding;
APPKIT_EXTERN NSBindingName NSContentObjectsBinding;
APPKIT_EXTERN NSBindingName NSContentSetBinding;
APPKIT_EXTERN NSBindingName NSContentValuesBinding;
APPKIT_EXTERN NSBindingName NSContentWidthBinding;
APPKIT_EXTERN NSBindingName NSCriticalValueBinding;
APPKIT_EXTERN NSBindingName NSDataBinding;
APPKIT_EXTERN NSBindingName NSDisplayPatternTitleBinding;
APPKIT_EXTERN NSBindingName NSDisplayPatternValueBinding;
APPKIT_EXTERN NSBindingName NSDocumentEditedBinding;
APPKIT_EXTERN NSBindingName NSDoubleClickArgumentBinding;
APPKIT_EXTERN NSBindingName NSDoubleClickTargetBinding;
APPKIT_EXTERN NSBindingName NSEditableBinding;
APPKIT_EXTERN NSBindingName NSEnabledBinding;
APPKIT_EXTERN NSBindingName NSExcludedKeysBinding		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingName NSFilterPredicateBinding;
APPKIT_EXTERN NSBindingName NSFontBinding;
APPKIT_EXTERN NSBindingName NSFontBoldBinding;
APPKIT_EXTERN NSBindingName NSFontFamilyNameBinding;
APPKIT_EXTERN NSBindingName NSFontItalicBinding;
APPKIT_EXTERN NSBindingName NSFontNameBinding;
APPKIT_EXTERN NSBindingName NSFontSizeBinding;
APPKIT_EXTERN NSBindingName NSHeaderTitleBinding;
APPKIT_EXTERN NSBindingName NSHiddenBinding;
APPKIT_EXTERN NSBindingName NSImageBinding;
APPKIT_EXTERN NSBindingName NSIncludedKeysBinding		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingName NSInitialKeyBinding		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingName NSInitialValueBinding		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingName NSIsIndeterminateBinding;
APPKIT_EXTERN NSBindingName NSLabelBinding;
APPKIT_EXTERN NSBindingName NSLocalizedKeyDictionaryBinding		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingName NSManagedObjectContextBinding;
APPKIT_EXTERN NSBindingName NSMaximumRecentsBinding;
APPKIT_EXTERN NSBindingName NSMaxValueBinding;
APPKIT_EXTERN NSBindingName NSMaxWidthBinding;
APPKIT_EXTERN NSBindingName NSMinValueBinding;
APPKIT_EXTERN NSBindingName NSMinWidthBinding;
APPKIT_EXTERN NSBindingName NSMixedStateImageBinding;
APPKIT_EXTERN NSBindingName NSOffStateImageBinding;
APPKIT_EXTERN NSBindingName NSOnStateImageBinding;
APPKIT_EXTERN NSBindingName NSPositioningRectBinding            API_AVAILABLE(macos(10.7));
APPKIT_EXTERN NSBindingName NSPredicateBinding;
APPKIT_EXTERN NSBindingName NSRecentSearchesBinding;
APPKIT_EXTERN NSBindingName NSRepresentedFilenameBinding;
APPKIT_EXTERN NSBindingName NSRowHeightBinding;
APPKIT_EXTERN NSBindingName NSSelectedIdentifierBinding;
APPKIT_EXTERN NSBindingName NSSelectedIndexBinding;
APPKIT_EXTERN NSBindingName NSSelectedLabelBinding;
APPKIT_EXTERN NSBindingName NSSelectedObjectBinding;
APPKIT_EXTERN NSBindingName NSSelectedObjectsBinding;
APPKIT_EXTERN NSBindingName NSSelectedTagBinding;
APPKIT_EXTERN NSBindingName NSSelectedValueBinding;
APPKIT_EXTERN NSBindingName NSSelectedValuesBinding;
APPKIT_EXTERN NSBindingName NSSelectionIndexesBinding;
APPKIT_EXTERN NSBindingName NSSelectionIndexPathsBinding;
APPKIT_EXTERN NSBindingName NSSortDescriptorsBinding;
APPKIT_EXTERN NSBindingName NSTargetBinding;
APPKIT_EXTERN NSBindingName NSTextColorBinding;
APPKIT_EXTERN NSBindingName NSTitleBinding;
APPKIT_EXTERN NSBindingName NSToolTipBinding;
APPKIT_EXTERN NSBindingName NSTransparentBinding		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingName NSValueBinding;
APPKIT_EXTERN NSBindingName NSValuePathBinding;
APPKIT_EXTERN NSBindingName NSValueURLBinding;
APPKIT_EXTERN NSBindingName NSVisibleBinding;
APPKIT_EXTERN NSBindingName NSWarningValueBinding;
APPKIT_EXTERN NSBindingName NSWidthBinding;

// constants for binding options
APPKIT_EXTERN NSBindingOption NSAllowsEditingMultipleValuesSelectionBindingOption;
APPKIT_EXTERN NSBindingOption NSAllowsNullArgumentBindingOption;
APPKIT_EXTERN NSBindingOption NSAlwaysPresentsApplicationModalAlertsBindingOption;
APPKIT_EXTERN NSBindingOption NSConditionallySetsEditableBindingOption;
APPKIT_EXTERN NSBindingOption NSConditionallySetsEnabledBindingOption;
APPKIT_EXTERN NSBindingOption NSConditionallySetsHiddenBindingOption;
APPKIT_EXTERN NSBindingOption NSContinuouslyUpdatesValueBindingOption;
APPKIT_EXTERN NSBindingOption NSCreatesSortDescriptorBindingOption;
APPKIT_EXTERN NSBindingOption NSDeletesObjectsOnRemoveBindingsOption;
APPKIT_EXTERN NSBindingOption NSDisplayNameBindingOption;
APPKIT_EXTERN NSBindingOption NSDisplayPatternBindingOption;
APPKIT_EXTERN NSBindingOption NSContentPlacementTagBindingOption		API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSBindingOption NSHandlesContentAsCompoundValueBindingOption;
APPKIT_EXTERN NSBindingOption NSInsertsNullPlaceholderBindingOption;
APPKIT_EXTERN NSBindingOption NSInvokesSeparatelyWithArrayObjectsBindingOption;
APPKIT_EXTERN NSBindingOption NSMultipleValuesPlaceholderBindingOption;
APPKIT_EXTERN NSBindingOption NSNoSelectionPlaceholderBindingOption;
APPKIT_EXTERN NSBindingOption NSNotApplicablePlaceholderBindingOption;
APPKIT_EXTERN NSBindingOption NSNullPlaceholderBindingOption;
APPKIT_EXTERN NSBindingOption NSRaisesForNotApplicableKeysBindingOption;
APPKIT_EXTERN NSBindingOption NSPredicateFormatBindingOption;
APPKIT_EXTERN NSBindingOption NSSelectorNameBindingOption;
APPKIT_EXTERN NSBindingOption NSSelectsAllWhenSettingContentBindingOption;
APPKIT_EXTERN NSBindingOption NSValidatesImmediatelyBindingOption;
APPKIT_EXTERN NSBindingOption NSValueTransformerNameBindingOption;
APPKIT_EXTERN NSBindingOption NSValueTransformerBindingOption;

/// NSManagedObjectContext implements the NSEditor and NSEditorRegistration protocols for use with NSControllers and NSDocument
@interface NSManagedObjectContext (NSEditorAndEditorRegistrationConformance) <NSEditor, NSEditorRegistration>
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END

