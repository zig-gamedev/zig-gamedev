/*	NSOperation.h
	Copyright (c) 2006-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSException.h>
#import <Foundation/NSProgress.h>
#import <sys/qos.h>
#import <dispatch/dispatch.h>

@class NSArray<ObjectType>, NSSet;

NS_ASSUME_NONNULL_BEGIN

#define NSOperationQualityOfService NSQualityOfService
#define NSOperationQualityOfServiceUserInteractive NSQualityOfServiceUserInteractive
#define NSOperationQualityOfServiceUserInitiated NSQualityOfServiceUserInitiated
#define NSOperationQualityOfServiceUtility NSQualityOfServiceUtility
#define NSOperationQualityOfServiceBackground NSQualityOfServiceBackground


API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSOperation : NSObject
#if !__OBJC2__
{
@private
    id _private;
    int32_t _private1;
#if __LP64__
    int32_t _private1b;
#endif
}
#endif

- (void)start;
- (void)main;

@property (readonly, getter=isCancelled) BOOL cancelled;
- (void)cancel;

@property (readonly, getter=isExecuting) BOOL executing;
@property (readonly, getter=isFinished) BOOL finished;
@property (readonly, getter=isConcurrent) BOOL concurrent; // To be deprecated; use and override 'asynchronous' below
@property (readonly, getter=isAsynchronous) BOOL asynchronous API_AVAILABLE(macos(10.8), ios(7.0), watchos(2.0), tvos(9.0));
@property (readonly, getter=isReady) BOOL ready;

- (void)addDependency:(NSOperation *)op;
- (void)removeDependency:(NSOperation *)op;

@property (readonly, copy) NSArray<NSOperation *> *dependencies;

typedef NS_ENUM(NSInteger, NSOperationQueuePriority) {
	NSOperationQueuePriorityVeryLow = -8L,
	NSOperationQueuePriorityLow = -4L,
	NSOperationQueuePriorityNormal = 0,
	NSOperationQueuePriorityHigh = 4,
	NSOperationQueuePriorityVeryHigh = 8
};

@property NSOperationQueuePriority queuePriority;

@property (nullable, copy) void (^completionBlock)(void) API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (void)waitUntilFinished API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@property double threadPriority API_DEPRECATED("Not supported", macos(10.6,10.10), ios(4.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));

@property NSQualityOfService qualityOfService API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@property (nullable, copy) NSString *name API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@end



API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSBlockOperation : NSOperation
#if !__OBJC2__
{
@private
    id _private2;
    void *_reserved2;
}
#endif

+ (instancetype)blockOperationWithBlock:(void (^)(void))block;

- (void)addExecutionBlock:(void (^)(void))block;
@property (readonly, copy) NSArray<void (^)(void)> *executionBlocks;

@end


API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0))
NS_SWIFT_UNAVAILABLE("NSInvocation and related APIs not available")
@interface NSInvocationOperation : NSOperation
#if !__OBJC2__
{
@private
    id _inv;
    id _exception;
    void *_reserved2;
}
#endif

- (nullable instancetype)initWithTarget:(id)target selector:(SEL)sel object:(nullable id)arg;
- (instancetype)initWithInvocation:(NSInvocation *)inv NS_DESIGNATED_INITIALIZER;

@property (readonly, retain) NSInvocation *invocation;

@property (nullable, readonly, retain) id result;

@end

FOUNDATION_EXPORT NSExceptionName const NSInvocationOperationVoidResultException API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSExceptionName const NSInvocationOperationCancelledException API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

static const NSInteger NSOperationQueueDefaultMaxConcurrentOperationCount = -1;

API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSOperationQueue : NSObject <NSProgressReporting>
#if !__OBJC2__
{
@private
    id _private;
    void *_reserved;
}
#endif

/// @property progress
/// @discussion     The `progress` property represents a total progress of the operations executed in the queue. By default NSOperationQueue
/// does not report progress until the `totalUnitCount` of the progress is set. When the `totalUnitCount` property of the progress is set the
/// queue then opts into participating in progress reporting. When enabled, each operation will contribute 1 unit of completion to the
/// overall progress of the queue for operations that are finished by the end of main (operations that override start and do not invoke super
/// will not contribute to progress). Special attention to race conditions should be made when updating the `totalUnitCount` of the progress
/// as well as care should be taken to avoid 'backwards progress'. For example; when a NSOperationQueue's progress is 5/10, representing 50%
/// completed, and there are 90 more operations about to be added and the `totalUnitCount` that would then make the progress report as 5/100
/// which represents 5%. In this example it would mean that any progress bar would jump from displaying 50% back to 5%, which might not be
/// desirable. In the cases where the `totalUnitCount` needs to be adjusted it is suggested to do this for thread-safety in a barrier by
/// using the `addBarrierBlock:` API. This ensures that no un-expected execution state occurs adjusting into a potentially backwards moving
/// progress scenario.
///
/// @example
/// NSOperationQueue *queue = [[NSOperationQueue alloc] init];
/// queue.progress.totalUnitCount = 10;
@property (readonly, strong) NSProgress *progress API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));

- (void)addOperation:(NSOperation *)op;
- (void)addOperations:(NSArray<NSOperation *> *)ops waitUntilFinished:(BOOL)wait API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)addOperationWithBlock:(void (^)(void))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) NS_SWIFT_DISABLE_ASYNC;

/// @method addBarrierBlock:
/// @param barrier      A block to execute
/// @discussion         The `addBarrierBlock:` method executes the block when the NSOperationQueue has finished all enqueued operations and
/// prevents any subsequent operations to be executed until the barrier has been completed. This acts similarly to the
/// `dispatch_barrier_async` function.
- (void)addBarrierBlock:(void (^)(void))barrier API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));

@property NSInteger maxConcurrentOperationCount;

@property (getter=isSuspended) BOOL suspended;

@property (nullable, copy) NSString *name API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@property NSQualityOfService qualityOfService API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@property (nullable, assign /* actually retain */) dispatch_queue_t underlyingQueue API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

- (void)cancelAllOperations;

- (void)waitUntilAllOperationsAreFinished;

@property (class, readonly, strong, nullable) NSOperationQueue *currentQueue API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (class, readonly, strong) NSOperationQueue *mainQueue API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

@interface NSOperationQueue (NSDeprecated)

// These two functions are inherently a race condition and should be avoided if possible

@property (readonly, copy) NSArray<__kindof NSOperation *> *operations API_DEPRECATED("access to operations is inherently a race condition, it should not be used. For barrier style behaviors please use addBarrierBlock: instead", macos(10.5, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

@property (readonly) NSUInteger operationCount API_DEPRECATED_WITH_REPLACEMENT("progress.completedUnitCount", macos(10.6, API_TO_BE_DEPRECATED), ios(4.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

@end

NS_ASSUME_NONNULL_END

