/*
	NSObjectController.h
	Application Kit
	Copyright (c) 2002-2021, Apple Inc.
	All rights reserved.
 */

#import <Foundation/NSArray.h>
#import <AppKit/NSController.h>
#import <AppKit/NSUserInterfaceValidation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSPredicate, NSManagedObjectContext, NSFetchRequest, NSError;

@interface NSObjectController : NSController

- (instancetype)initWithContent:(nullable id)content NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@property (nullable, strong) id content;

@property (readonly, strong) id selection;    // an object representing all objects to be affected by editing as a singleton, returning special marker objects like NSMultipleSelectionMarker if necessary - in the concrete case of NSObjectController, returns an object that is used to access the content object
@property (readonly, copy) NSArray *selectedObjects;    // array of all objects to be affected by editing (if controller supports a selection mechanisms, the selected objects, otherwise all content objects) - in the concrete case of NSObjectController, returns an array with the content object
@property BOOL automaticallyPreparesContent;    // if YES, controllers will automatically invoke prepareContent when loaded from a nib file
- (void)prepareContent;    // typically overridden in subclasses which know how to get their content (from the file system or so) - default implementation creates a new object and populates the controller with it
@property (null_resettable, assign) Class objectClass;   // sets the object class used when creating new objects
- (id)newObject;    // creates a new object when adding/inserting objects (default implementation assumes the object class has a standard init method without arguments) - the returned object should not be autoreleased
- (void)addObject:(id)object;    // sets the content object of the controller - if the controller's content is bound to another object or controller through a relationship key, the relationship of the 'master' object will be changed
- (void)removeObject:(id)object;    // if the object is the current content object of the controller, clears the content - if the controller's content is bound to another object or controller through a relationship key, the relationship of the 'master' object will be cleared
   // determines whether controller allows adding and removing objects
@property (getter=isEditable) BOOL editable;
- (IBAction)add:(nullable id)sender;    // creates a new object and adds it through addObject:
@property (readonly) BOOL canAdd;    // can be used in bindings controlling the enabling of buttons, for example
- (IBAction)remove:(nullable id)sender;    // removes content object through removeObject:
@property (readonly) BOOL canRemove;    // can be used in bindings controlling the enabling of buttons, for example
- (BOOL)validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)item;    // overridden to automatically disable user interface elements sending action methods to the controller (for example if canAdd returns NO, menu items with the add: action are disabled)

@end


@interface NSObjectController (NSManagedController)

@property (nullable, strong) NSManagedObjectContext *managedObjectContext;

@property (nullable, copy) NSString *entityName;
@property (nullable, strong) NSPredicate *fetchPredicate;

- (BOOL)fetchWithRequest:(nullable NSFetchRequest *)fetchRequest merge:(BOOL)merge error:(NSError **)error;    // subclasses can override this method to customize the fetch request, for example to specify fetch limits (passing nil for the fetch request will result in the default fetch request to be used; this method will never be invoked with a nil fetch request from within the standard Cocoa frameworks) - the merge flag determines whether the controller replaces the entire content with the fetch result or merges the existing content with the fetch result

- (IBAction)fetch:(nullable id)sender;


@property BOOL usesLazyFetching API_AVAILABLE(macos(10.5)); // defaults to NO. 

- (NSFetchRequest *)defaultFetchRequest API_AVAILABLE(macos(10.5));


@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END


