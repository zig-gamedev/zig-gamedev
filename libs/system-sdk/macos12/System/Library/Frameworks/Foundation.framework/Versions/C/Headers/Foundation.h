/*	Foundation.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/


#include <CoreFoundation/CoreFoundation.h>

#import <Foundation/NSObjCRuntime.h>

#import <Foundation/NSArray.h>
#import <Foundation/NSAutoreleasePool.h>
#import <Foundation/NSBundle.h>
#import <Foundation/NSByteOrder.h>
#import <Foundation/NSCalendar.h>
#import <Foundation/NSCharacterSet.h>
#import <Foundation/NSCoder.h>
#import <Foundation/NSData.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSDateInterval.h>
#import <Foundation/NSDateFormatter.h>
#import <Foundation/NSDateIntervalFormatter.h>
#import <Foundation/NSISO8601DateFormatter.h>
#import <Foundation/NSMassFormatter.h>
#import <Foundation/NSLengthFormatter.h>
#import <Foundation/NSEnergyFormatter.h>
#import <Foundation/NSMeasurement.h>
#import <Foundation/NSMeasurementFormatter.h>
#import <Foundation/NSPersonNameComponents.h>
#import <Foundation/NSPersonNameComponentsFormatter.h>
#import <Foundation/NSRelativeDateTimeFormatter.h>
#import <Foundation/NSListFormatter.h>
#import <Foundation/NSDecimal.h>
#import <Foundation/NSDecimalNumber.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSEnumerator.h>
#import <Foundation/NSError.h>
#import <Foundation/NSException.h>
#import <Foundation/NSFileHandle.h>
#import <Foundation/NSFileManager.h>
#import <Foundation/NSFormatter.h>
#import <Foundation/NSHashTable.h>
#import <Foundation/NSHTTPCookie.h>
#import <Foundation/NSHTTPCookieStorage.h>
#import <Foundation/NSIndexPath.h>
#import <Foundation/NSIndexSet.h>
#import <Foundation/NSInflectionRule.h>
#import <Foundation/NSInvocation.h>
#import <Foundation/NSJSONSerialization.h>
#import <Foundation/NSKeyValueCoding.h>
#import <Foundation/NSKeyValueObserving.h>
#import <Foundation/NSKeyedArchiver.h>
#import <Foundation/NSLocale.h>
#import <Foundation/NSLock.h>
#import <Foundation/NSMapTable.h>
#import <Foundation/NSMethodSignature.h>
#import <Foundation/NSMorphology.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSNotificationQueue.h>
#import <Foundation/NSNull.h>
#import <Foundation/NSNumberFormatter.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSOperation.h>
#import <Foundation/NSOrderedSet.h>
#import <Foundation/NSOrthography.h>
#import <Foundation/NSPathUtilities.h>
#import <Foundation/NSPointerArray.h>
#import <Foundation/NSPointerFunctions.h>
#import <Foundation/NSPort.h>
#import <Foundation/NSProcessInfo.h>
#import <Foundation/NSPropertyList.h>
#import <Foundation/NSProxy.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSRegularExpression.h>
#import <Foundation/NSRunLoop.h>
#import <Foundation/NSScanner.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSSortDescriptor.h>
#import <Foundation/NSStream.h>
#import <Foundation/NSString.h>
#import <Foundation/NSTextCheckingResult.h>
#import <Foundation/NSThread.h>
#import <Foundation/NSTimeZone.h>
#import <Foundation/NSTimer.h>
#import <Foundation/NSUnit.h>
#import <Foundation/NSURL.h>
#import <Foundation/NSURLAuthenticationChallenge.h>
#import <Foundation/NSURLCache.h>
#import <Foundation/NSURLConnection.h>
#import <Foundation/NSURLCredential.h>
#import <Foundation/NSURLCredentialStorage.h>
#import <Foundation/NSURLError.h>
#import <Foundation/NSURLProtectionSpace.h>
#import <Foundation/NSURLProtocol.h>
#import <Foundation/NSURLRequest.h>
#import <Foundation/NSURLResponse.h>
#import <Foundation/NSUserDefaults.h>
#import <Foundation/NSValue.h>
#import <Foundation/NSValueTransformer.h>
#import <Foundation/NSXMLParser.h>
#import <Foundation/NSXPCConnection.h>
#import <Foundation/NSZone.h>

#import <Foundation/FoundationErrors.h>

#if TARGET_OS_OSX || TARGET_OS_IPHONE

#import <Foundation/NSAttributedString.h>
#import <Foundation/NSByteCountFormatter.h>
#import <Foundation/NSCache.h>
#import <Foundation/NSComparisonPredicate.h>
#import <Foundation/NSCompoundPredicate.h>
#import <Foundation/NSDateComponentsFormatter.h>
#import <Foundation/NSExpression.h>
#import <Foundation/NSExtensionContext.h>
#import <Foundation/NSExtensionItem.h>
#import <Foundation/NSExtensionRequestHandling.h>
#import <Foundation/NSFileCoordinator.h>
#import <Foundation/NSFilePresenter.h>
#import <Foundation/NSFileVersion.h>
#import <Foundation/NSFileWrapper.h>
#import <Foundation/NSItemProvider.h>
#import <Foundation/NSLinguisticTagger.h>
#import <Foundation/NSMetadata.h>
#import <Foundation/NSMetadataAttributes.h>
#import <Foundation/NSNetServices.h>
#import <Foundation/NSPredicate.h>
#import <Foundation/NSProgress.h>
#import <Foundation/NSUbiquitousKeyValueStore.h>
#import <Foundation/NSUndoManager.h>
#import <Foundation/NSURLSession.h>
#import <Foundation/NSUserActivity.h>
#import <Foundation/NSUUID.h>

#endif /* TARGET_OS_OSX || TARGET_OS_IPHONE */

#if TARGET_OS_OSX || TARGET_OS_MACCATALYST

#import <Foundation/NSAffineTransform.h>
#import <Foundation/NSAppleScript.h>
#import <Foundation/NSGeometry.h>

#import <Foundation/NSArchiver.h>
#import <Foundation/NSBackgroundActivityScheduler.h>
#import <Foundation/NSCalendarDate.h>
#import <Foundation/NSConnection.h>
#import <Foundation/NSDistantObject.h>
#import <Foundation/NSDistributedNotificationCenter.h>
#import <Foundation/NSPortCoder.h>
#import <Foundation/NSPortMessage.h>
#import <Foundation/NSPortNameServer.h>
#import <Foundation/NSProtocolChecker.h>
#import <Foundation/NSTask.h>
#import <Foundation/NSXMLDTD.h>
#import <Foundation/NSXMLDTDNode.h>
#import <Foundation/NSXMLDocument.h>
#import <Foundation/NSXMLElement.h>
#import <Foundation/NSXMLNode.h>
#import <Foundation/NSXMLNodeOptions.h>
#import <Foundation/NSURLDownload.h>
#import <Foundation/NSURLHandle.h>

#import <Foundation/NSAppleEventDescriptor.h>
#import <Foundation/NSAppleEventManager.h>
#import <Foundation/NSClassDescription.h>
#import <Foundation/NSDistributedLock.h>
#import <Foundation/NSGarbageCollector.h>
#import <Foundation/NSHFSFileTypes.h>
#import <Foundation/NSHost.h>
#import <Foundation/NSObjectScripting.h>
#import <Foundation/NSScriptClassDescription.h>
#import <Foundation/NSScriptCoercionHandler.h>
#import <Foundation/NSScriptCommand.h>
#import <Foundation/NSScriptCommandDescription.h>
#import <Foundation/NSScriptExecutionContext.h>
#import <Foundation/NSScriptKeyValueCoding.h>
#import <Foundation/NSScriptObjectSpecifiers.h>
#import <Foundation/NSScriptStandardSuiteCommands.h>
#import <Foundation/NSScriptSuiteRegistry.h>
#import <Foundation/NSScriptWhoseTests.h>
#import <Foundation/NSSpellServer.h>
#import <Foundation/NSUserNotification.h>
#import <Foundation/NSUserScriptTask.h>

#endif /* TARGET_OS_OSX || TARGET_OS_MACCATALYST */

#import <Foundation/FoundationLegacySwiftCompatibility.h>
