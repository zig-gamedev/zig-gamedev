/*
 *       MDQuery.h
 *       Copyright (c) 2003-2019, Apple Inc. All rights reserved.
 */

/*!
        @header MDQuery

        MDQuery is a CF-compliant object, and follows the CF conventions,
        and can be used with the CF polymorphic functions, like CFRetain().

        MDQuery encapsulates all queries against the MetaData
        database. Queries gather results or process updates only while
        the current thread's run loop is running. Queries normally
        operate asynchronously, and only send out progress
        notificiations as the list is being collected. The query list
        is kept up to date with respect to value lists and sorting as
        the progress notifications are sent out, so the query is in a
        good state during those events.

        Result Retreval
                An MDQueryRef presents its results as if it were a simple
                array object.  The results can either be MDItemRefs (the
                default) or custom structs, CFTypeRefs, or Objective C
                objects.  For example if the result objects are represented
                as 'FSNodes' (a type defined in the client application) a
                call to MDQueryGetResultAtIndex() will return an FSNode
                object. This is useful, because it allows the Metadata
                library to maintain the results in a form that is directly
                useable in the application.

        Query Sorting
                Sorting the results from a query can be performed in one  of
                two ways. First is to let the library sort the results for you
                by passing an array of attributes to sort on to
                MDQueryCreate(). The default sort provided by MDQueryCreate()
                is a assending sort strings are compared using
                CFStringCompare() with the options kCFCompareNonliteral |
                kCFCompareLocalized | kCFCompareNumerically. CFDataRefs are
                compared by using memcmp() of the data pointers. If an Item
                does not have the attribute, it compares less than the
                attribute that is there.

                If you need to do a more complex sort (like a case insensitive
                sort on CFStringRefs) then you will need to also provide a
                comparitor function and call MDQuerySetSortComparator(), and
                also provide which attributes to sort on to MDQueryCreate().

        Undefined Behavior
                For functions which take an MDQueryRef parameter, if this
                parameter is not a valid MDQueryRef, the behavior is
                undefined. NULL is not a valid MDQueryRef.

                For functions which take CF*Ref parameters, such as
                CFStringRef and CFArrayRef, if this parameter is not a
                valid CF object of the correct type, the behavior is
                undefined. NULL is not a valid CF*Ref.

                Additional constraints or allowed values on parameters
                are noted with the specific functions.

*/

#if !defined(__MDQUERY__)
#define __MDQUERY__ 1

#include <CoreFoundation/CoreFoundation.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4

#include <Metadata/MDItem.h>

CF_IMPLICIT_BRIDGING_ENABLED
MD_BEGIN_C_DECLS


/*!
        @typedef MDQueryRef
        This is the type of a reference to MDQuerys.
*/
typedef struct CF_BRIDGED_TYPE(id) __MDQuery * MDQueryRef;

typedef enum {
    kMDQuerySynchronous = 1,
    kMDQueryWantsUpdates = 4,
    kMDQueryAllowFSTranslation = 8
} MDQueryOptionFlags;


/*!
        @enum MDQueryOptionFlags
        @constant kMDQuerySynchronous Block during the gathering phase.
                If this parameter is true, the function will not return
                until the query has finished gathering the initial
                results. The run loop will run in the default mode, which
                will allow anything registered in that mode with this run
                loop to execute as well. If this parameter is false, the
                function returns immediately after starting the query
                asychronously.
        @constant kMDQueryWantsUpdates When set, after gathering the initial results
                the query will watch for changes in the system which should
                update the list of results. This can be changes which cause
                new files to now match the query, or changes which cause
                files in the result list to continue to match, or no longer
                match, the query. Files which begin to match the query are
                added to the result list, and files which no longer match
                the query expression are removed from the result list.
                Currently, this parameter is ignored if the synchronous
                parameter is true. This is subject to change, and you
                should always pass in the value you want here.
*/

/*!
        @function MDQueryGetTypeID
        Returns the type identifier of all MDQuery instances.
*/
MD_EXPORT CFTypeID MDQueryGetTypeID(void) MD_AVAIL;

/*!
        @function MDQueryCreate
        Creates a new query with the given query expression.
        @param allocator The CFAllocator which should be used to allocate
                memory for the query and its sub-storage. This
                parameter may be NULL in which case the current default
                CFAllocator is used.
        @param queryString The query expression string for this query. The
                syntax for query expressions is explained above in the
                header overview documentation.
        @param valueListAttrs An optional array of attribute names. The
                query will collect the values of these attributes into
                uniqued lists, which can be used or displayed to summarize
                the results of the query, or allow a user to further
                qualify the items for which they are searching. This
                parameter may be NULL if no value lists are desired. Value
                list collection increases CPU usage and significantly
                increases the memory usage of an MDQuery. The attribute
                names are CFStrings.
        @param sortingAttrs An optional array of attribute names. The
                query will results of the query based on the values of
                these attributes. The first name in the array is used as
                the primary sort key, the second as the secondary key, and
                so on. The comparison of like-typed values is a simple,
                literal comparison. This parameter may be NULL if no
                sorting is desired. Sorting increases memory usage and
                significantly increases the CPU usage of an MDQuery.
                However, when possible, it is almost always cheaper to have
                the MDQuery do the sorting, rather than you fetching all
                the results and attributes from each of them and doing the
                sorting yourself. The attribute names are CFStrings.
        @result An MDQueryRef, or NULL on failure. If the query string
                is empty or malformed (invalid syntax), returns NULL.
*/
MD_EXPORT MDQueryRef MDQueryCreate(CFAllocatorRef allocator, CFStringRef queryString, CFArrayRef valueListAttrs, CFArrayRef sortingAttrs) MD_AVAIL;

/*!
        @function MDQueryCreateSubset
        Creates a new query, which is a subset of the given query. Only
                results matched by the given query can be matched by the
                query expression of this query.
        @param allocator The CFAllocator which should be used to allocate
                memory for the query and its sub-storage. This
                parameter may be NULL in which case the current default
                CFAllocator is used.
        @param query The parent query of the new query.
        @param queryString The query expression string for this query.
                This expression in effect may further restrict the matches
                found by the parent query. If the string is empty the
                behavior is undefined.
        @param valueListAttrs An optional array of attribute names. The
                query will collect the values of these attributes into
                uniqued lists, which can be used or displayed to summarize
                the results of the query, or allow a user to further
                qualify the items for which they are searching. This
                parameter may be NULL if no value lists are desired. Value
                list collection increases CPU usage and significantly
                increases the memory usage of an MDQuery. The attribute
                names are CFStrings.
        @param sortingAttrs An optional array of attribute names. The
                query will sort results of the query based on the values of
                these attributes. The first name in the array is used as
                the primary sort key, the second as the secondary key, and
                so on. The comparison of like-typed values is a simple,
                literal comparison. This parameter may be NULL if no
                sorting is desired. Sorting increases memory usage and
                significantly increases the CPU usage of an MDQuery.
                However, when possible, it is almost always cheaper to have
                the MDQuery do the sorting, rather than you fetching all
                the results and attributes from each of them and doing the
                sorting yourself. The attribute names are CFStrings.
        @result An MDQueryRef, or NULL on failure. If the query string
                is empty or malformed (invalid syntax), returns NULL.
*/
MD_EXPORT MDQueryRef MDQueryCreateSubset(CFAllocatorRef allocator, MDQueryRef query, CFStringRef queryString, CFArrayRef valueListAttrs, CFArrayRef sortingAttrs) MD_AVAIL;

/*!
 @function MDQueryCreateForItems
 Creates a new query with the given query expression.
 @param allocator The CFAllocator which should be used to allocate
 memory for the query and its sub-storage. This
 parameter may be NULL in which case the current default
 CFAllocator is used.
 @param queryString The query expression string for this query. The
 syntax for query expressions is explained above in the
 header overview documentation.
 @param valueListAttrs An optional array of attribute names. The
 query will collect the values of these attributes into
 uniqued lists, which can be used or displayed to summarize
 the results of the query, or allow a user to further
 qualify the items for which they are searching. This
 parameter may be NULL if no value lists are desired. Value
 list collection increases CPU usage and significantly
 increases the memory usage of an MDQuery. The attribute
 names are CFStrings.
 @param sortingAttrs An optional array of attribute names. The
 query will results of the query based on the values of
 these attributes. The first name in the array is used as
 the primary sort key, the second as the secondary key, and
 so on. The comparison of like-typed values is a simple,
 literal comparison. This parameter may be NULL if no
 sorting is desired. Sorting increases memory usage and
 significantly increases the CPU usage of an MDQuery.
 However, when possible, it is almost always cheaper to have
 the MDQuery do the sorting, rather than you fetching all
 the results and attributes from each of them and doing the
 sorting yourself. The attribute names are CFStrings.
 @param items An array of items. The query will only return results
 in this set.
 @result An MDQueryRef, or NULL on failure. If the query string
 is empty or malformed (invalid syntax), returns NULL.
 */
MD_EXPORT MDQueryRef MDQueryCreateForItems(CFAllocatorRef allocator,
										   CFStringRef queryString,
										   CFArrayRef valueListAttrs,
										   CFArrayRef sortingAttrs,
										   CFArrayRef items) API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );


/*!
        @function MDQueryCopyQueryString
        Returns the query string of the query.
        @param query The query to be interrogated.
        @result The query string of the query.
*/
MD_EXPORT CFStringRef MDQueryCopyQueryString(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQueryCopyValueListAttributes
        Returns the list of attribute names for which the query is
                collecting the lists of values.
        @param query The query to be interrogated.
        @result The list of value list attribute names of the query.
*/
MD_EXPORT CFArrayRef MDQueryCopyValueListAttributes(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQueryCopySortingAttributes
        Returns the list of attribute names the query is using to sort
                the results.
        @param query The query to be interrogated.
        @result The list of sorting attribute names of the query.
*/
MD_EXPORT CFArrayRef MDQueryCopySortingAttributes(MDQueryRef query) MD_AVAIL;

/*!
        @typedef MDQueryBatchingParams
        Structure containing the progress notification batching
                parameters of an MDQuery. The first notification can be
                triggered by the either first_max_num or first_max_ms limit
                being exceeded. Subsequent notifications are triggered by
                either the progress_max_num or progress_max_ms limit. The
                default batching parameters are undefined and subject to
                change.
        @field first_max_num The maximum number of results that can
                accumulate before a progress notification is sent out
                by the MDQuery, for the first notification.
        @field first_max_ms The maximum number of milliseconds that can
                pass before a progress notification is sent out. This
                value is advisory, in that the notification will be
                triggered "at some point after first_max_ms milliseconds
                have passed since the query began accumulating results",
                but generally not very long after, for the first
                progress notification.
        @field progress_max_num The maximum number of results that can
                accumulate before a progress notification is sent out
                by the MDQuery, for notifications after the first,
                during the initial gathering phase of the query.
        @field progress_max_ms The maximum number of milliseconds that can
                pass before a progress notification is sent out. This
                value is advisory, in that the notification will be
                triggered "at some point after first_max_ms milliseconds
                have passed since the query began accumulating results",
                but generally not very long after, for progress
                notifications after the first, during the initial
                gathering phase of the query.
        @field update_max_num The maximum number of results that can
                accumulate before an update notification is sent out
                by the MDQuery, for notifications after the gathering
                phase of the query has finished.
        @field update_max_ms The maximum number of milliseconds that can
                pass before a progress notification is sent out. This
                value is advisory, in that the notification will be
                triggered "at some point after first_max_ms milliseconds
                have passed since the query began accumulating results",
                but generally not very long after, for update notifications
                after the gathering phase of the query has finished.
*/
typedef struct {
    size_t first_max_num;
    size_t first_max_ms;
    size_t progress_max_num;
    size_t progress_max_ms;
    size_t update_max_num;
    size_t update_max_ms;
} MDQueryBatchingParams;

/*!
        @function MDQueryGetBatchingParameters
        Returns the current parameters that control batching of progress
                notifications.
        @param query The query to be interrogated.
        @result An MDQueryBatchingParams structure with the current
                batching parameters.
*/
MD_EXPORT MDQueryBatchingParams MDQueryGetBatchingParameters(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQuerySetBatchingParameters
        @param query The query whose batching parameters are to be set.
        @param params An MDQueryBatchingParams structure with the batching
                parameters to set.
*/
MD_EXPORT void MDQuerySetBatchingParameters(MDQueryRef query, MDQueryBatchingParams params) MD_AVAIL;

/*!
        @typedef MDQueryCreateResultFunction
        Type of the callback function used to create the result objects
                stored and returned by an MDQuery. The function may
                hold onto the given MDItemRef in some other data
                structure, but must retain it for it to remain valid.
                The create-result function is called lazily as results
                are requested from a query, so it will not generally
                be called on all results, if in fact any. This avoids
                the cost of creating potentially hundreds of thousands
                of what might be temporary objects.
        @param query The MDQuery instance.
        @param item The default MDItemRef for the result.
        @param context The user-defined context parameter given to
                MDQuerySetCreateResultFunction().
        @result The function must return a pointer-sized value that can
                be managed with the callbacks which were set at the same
                time the create function was given to the query. The
                value must be returned with a reference (such as if the
                retain callback had been called on it), as implied by the
                Create name. If this function doesn't wish to create a
                new object, it can return the given MDItemRef, but must
                also return it with a new retain, and the callbacks must
                be able to handle an MDItemRef as an input value. If
                this function returns NULL, NULL will be stored for the
                moment in the query, MDQueryGetResultAtIndex() may return
                NULL for that result, and the next time the query wants
                the result, it will call this function again.
*/
typedef const void *(*MDQueryCreateResultFunction)(MDQueryRef query, MDItemRef item, void *context);

/*!
        @function MDQuerySetCreateResultFunction
        Sets the function used to create the result objects of the
                MDQuery. If no create function is set on an MDQuery,
                the default result objects are MDItemRefs. Results
                created after a create function is set will be created
                through the given create function, but values created
                before the function was set (or after it is unset) are
                not modified. Therefore it is not advisable to change
                this function after MDQueryExecute() has been called
                with the query. The create-result function is called
                lazily as results are requested from a query, so it will
                not generally be called on all results, if in fact any.
                This avoids the cost of creating potentially hundreds
                of thousands of what might be temporary objects.
        @param query The query to whose result create function is to be set.
        @param func The callback function the MDQuery will use to
                create its results, such as those returned from
                MDQueryGetResultAtIndex(). This parameter
                may be NULL, in which case any previous result creation
                settings are cancelled, and the MDQuery will subsequently
                produce MDItemRefs. If the function (when the parameter is
                not NULL) is not of type MDQueryCreateResultFunction or
                does not behave as a MDQueryCreateResultFunction must,
                the behavior is undefined.
        @param context A pointer-sized user-defined value, which is
                passed as the third parameter to the create function,
                but is otherwise unused by MDQuery. The MDQuery does
                not retain the context in any way, so it must remain
                valid for at least the lifetime of the query. If the
                context is not what is expected by the create function,
                the behavior is undefined.
        @param cb A pointer to a CFArrayCallBacks structure
                initialized with the callbacks for the query to use to
                manage the created result objects. A copy of the
                contents of the callbacks structure is made, so that a
                pointer to a structure on the stack can be passed in, or
                can be reused for multiple query creations. Only version
                0 of the CFArrayCallBacks is supported. The retain field
                may be NULL, in which case the MDQuery will do nothing to
                add a retain to the created results for the query. The
                release field may be NULL, in which case the MDQuery will
                do nothing to remove the query's retain (such as the one
                it gets from the create function) on the result objects
                when the query is destroyed. If the copyDescription field
                is NULL, the query will create a simple description for
                the result objects. If the equal field is NULL, the query
                will use pointer equality to test for equality of results.
                This callbacks parameter itself may be NULL, which is
                treated as if a valid structure of version 0 with all
                fields NULL had been passed in. Otherwise, if any of the
                fields are not valid pointers to functions of the correct
                type, or this parameter is not a valid pointer to a
                CFArrayCallBacks callbacks structure, the behavior is
                undefined. If any of the value values returned from the
                create function is not one understood by one or more of
                the callback functions, the behavior when those callback
                functions are used is undefined. For example, if the create
                function can return NULL, then NULL must be understood by
                the callback functions as a possible parameter. The retain
                and release callbacks must be a matched set -- do not
                assume that the retain function will be unused or that
                additional reference counts will not be taken on the
                created results.
*/
MD_EXPORT void MDQuerySetCreateResultFunction(MDQueryRef query, MDQueryCreateResultFunction func, void *context, const CFArrayCallBacks *cb) MD_AVAIL;

/*!
        @typedef MDQueryCreateValueFunction
        Type of the callback function used to create the value objects
                stored and returned by an MDQuery. The function may
                hold onto the given attribute name and/or value in some
                other data structure, but must retain them for them to
                remain valid.
        @param query The MDQuery instance.
        @param attrName The attribute name of the value.
        @param attrValue The default value of the value.
        @param context The user-defined context parameter given to
                MDQuerySetCreateValueFunction().
        @result The function must return a pointer-sized value that can
                be managed with the callbacks which were set at the same
                time the create function was given to the query. The
                value must be returned with a reference (such as if the
                retain callback had been called on it), as implied by the
                Create name. If this function doesn't wish to create a
                new object, it can return the given CFTypeRef, but must
                also return it with a new retain, and the callbacks must
                be able to handle a CFTypeRef as an input value.
*/
typedef const void *(*MDQueryCreateValueFunction)(MDQueryRef query, CFStringRef attrName, CFTypeRef attrValue, void *context);

/*!
        @function MDQuerySetCreateValueFunction
        Sets the function used to create the value objects of the
                MDQuery. These are the values of the value lists that
                were requested when the query was created. If no create
                function is set on an MDQuery, the default value objects
                are the CFTypeRef values of the attributes. Values
                created after a create function is set will be created
                through the given create function, but values created
                before the function was set (or after it is unset)
                are not modified. Therefore it is not advisable to
                change this function after MDQueryExecute() has been
                called with the query.
        @param query The query to whose value create function is to be set.
        @param func The callback function the MDQuery will use to
                create the value list values, such as those returned from
                MDQueryCopyValuesOfAttribute(). This parameter
                may be NULL, in which case any previous value creation
                settings are cancelled, and the MDQuery will subsequently
                produce the default CFTypeRefs. If the function (when the
                parameter is not NULL) is not of type
                MDQueryCreateValueFunction or does not behave as a
                MDQueryCreateValueFunction must, the behavior is undefined.
        @param context A pointer-sized user-defined value, which is
                passed as the fourth parameter to the create function,
                but is otherwise unused by MDQuery. The MDQuery does
                not retain the context in any way, so it must remain
                valid for at least the lifetime of the query. If the
                context is not what is expected by the create function,
                the behavior is undefined.
        @param cb A pointer to a CFArrayCallBacks structure
                initialized with the callbacks for the query to use to
                manage the created value objects. A copy of the
                contents of the callbacks structure is made, so that a
                pointer to a structure on the stack can be passed in, or
                can be reused for multiple query creations. Only version
                0 of the CFArrayCallBacks is supported. The retain field
                may be NULL, in which case the MDQuery will do nothing to
                add a retain to the created values for the query. The
                release field may be NULL, in which case the MDQuery will
                do nothing to remove the query's retain (such as the one
                it gets from the create function) on the value objects
                when the query is destroyed. If the copyDescription field
                is NULL, the query will create a simple description for
                the value objects. If the equal field is NULL, the query
                will use pointer equality to test for equality of values.
                This callbacks parameter itself may be NULL, which is
                treated as if a valid structure of version 0 with all
                fields NULL had been passed in. Otherwise, if any of the
                fields are not valid pointers to functions of the correct
                type, or this parameter is not a valid pointer to a
                CFArrayCallBacks callbacks structure, the behavior is
                undefined. If any of the value values returned from the
                create function is not one understood by one or more of
                the callback functions, the behavior when those callback
                functions are used is undefined. For example, if the
                create function can return NULL, then NULL must be
                understood by the callback functions as a possible
                parameter. The retain and release callbacks must be a
                matched set -- do not assume that the retain function will
                be unused or that additional reference counts will not be
                taken on the created values.
*/
MD_EXPORT void MDQuerySetCreateValueFunction(MDQueryRef query, MDQueryCreateValueFunction func, void *context, const CFArrayCallBacks *cb) MD_AVAIL;

/*!
	@function MDQuerySetDispatchQueue
	Set the dispatch queue on which query results will be delivered
				by MDQueryExecute. It is not advisable to change set 
				dispatch queue after MDQueryExecute() has been called with
				the query. Setting the dispatch queue for a synchronous 
				query (kMDQuerySynchronous) has no effect.
	@param query The query for which the dispatch queue should be set.
	@param queue The dispatch queue on which results should be delivered.
 */

MD_EXPORT void MDQuerySetDispatchQueue(MDQueryRef query, dispatch_queue_t queue) API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
        @function MDQueryExecute
        Run the query, and populate the query with the results. Queries
                only gather results or process updates while the current
                thread's run loop is running. Queries normally operate
                asynchronously, and send out progress and update
                notifications to report changes to the list of results
                that has been collected. Queries have two phases: the
                initial gathering of all currently matching results, and
                a second live-update phase where queries monitor the
                state of the system and update themselves to external
                changes in files or the operating environment (for example,
                as time advances, files which did not match the query
                when it was started may later match the query). Query
                notifications are posted within the context of the same
                thread which executes the query.
                [[There are three operational modes: (1) synchronous static
                queries, which collect the list of current results and then
                do not watch for updates to the results, (2) asynchronous
                static queries, which collect the results asychronously
                after this function returns, and then do not watch for
                updates to the results, and (3) asynchronous live queries
                which collect the initial results asychronously after this
                function returns, and then do watch for updates to the
                results, until the query is destroyed. There is little
                reason not to allow the fourth case, synchronous collection
                of initial results, followed by asynchronous monitoring
                for updates, so this may change in the future.]]
        @param query The query to execute.
        @param optionFlags Bitwise or of MDQueryOptionFlags
        @result Returns true if the query was started (executed in the case
                of a synchronous query), false otherwise. Queries cannot be
                executed more than once.
*/
MD_EXPORT Boolean MDQueryExecute(MDQueryRef query, CFOptionFlags optionFlags) MD_AVAIL;

/*!
        @function MDQueryStop
        Stops the query from ever generating more results. Queries may be
                executed only once, so a stopped query cannot be
                restarted. The query will also not generate any result
                updates. The query is static after this function returns.
                The query will do final processing of results that have
                come in but not yet been processed (because, say, the
                batching parameters hasn't triggered that yet). That may
                trigger a progress notification, so be aware of that if
                you are stopping a query from within your progress note
                handler; that is, during this function, a recursive
                progress and/or finished notification might occur, which
                might recursively call your notification handler. It is
                safe to call this function recursively. You would call
                this function to stop a query that is generating way too
                many results to be useful, but still want to access the
                results that have come in so far. If a query is stopped
                before the gathering phase finishes, it will not report
                itself as finished, nor will it send out a finished
                notification.
        @param query The query to stop.
*/
MD_EXPORT void MDQueryStop(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQueryDisableUpdates
        Disables updates to the query result list. This should be called
                before iterating through the list of results to prevent
                the result list from changing during the iteration. The
                disabled state is a counter, and disabling can be done
                recursively and from different threads.
        @param query The query for which updates are to be disabled.
		@result The generation number of the query. This changes each time the query's 
				result set has changed.
*/
MD_EXPORT void MDQueryDisableUpdates(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQueryEnableUpdates
        Re-enables updates to the query result list. This should be called
                when finished iterating through the list of results, to
                allow changes to the result list to occur. Changes will
                be allowed when all the disables have been matched by a
                corresponding enable.
        @param query The query for which updates are to be enabled.
*/
MD_EXPORT void MDQueryEnableUpdates(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQueryIsGatheringComplete
        Returns true if the first phase of a query, the initial result
                gathering, has finished.
        @param query The query to be interrogated.
        @result A boolean indicating whether or not the first phase
                of a query has completed.
 */
MD_EXPORT Boolean MDQueryIsGatheringComplete(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQueryGetResultCount
        Returns the number of results currently collected by the query.
                Note that the number of results in a query will change
                over time as the query's result list is updated.
        @param query The query to be interrogated.
        @result The number of results in the query.
*/
MD_EXPORT CFIndex MDQueryGetResultCount(MDQueryRef query) MD_AVAIL;

/*!
        @function MDQueryGetResultAtIndex
        Returns the current result at the given index. This function
                causes the result object to be created if it hasn't
                been created already. For performance reasons, it is
                not advisable to ask for results that you don't need,
                to avoid the cost of creating them. If possible, call
                this function to fetch only the results you need to
                display or otherwise process. Note that the index of
                a particular result will change over time, as the
                query's result list is updated.
        @param query The query to be interrogated.
        @param idx The index into the query's result list. If the index is
                negative, or is equal to or larger than the current
                number of results in the query, the behavior is undefined.
        @result Returns the MDItemRef currently at the given index, or
                if a result-create function has been set, returns the
                result returned by that function.
*/
MD_EXPORT const void *MDQueryGetResultAtIndex(MDQueryRef query, CFIndex idx) MD_AVAIL;

/*!
        @function MDQueryGetIndexOfResult
        Returns the current index of the given result. If a result-create
                function has been set, and the equal callback is non-NULL,
                it will be used to test the query's results against the
                candidate result. Note that the index of a result will
                change over time, as the query's result list is updated.
        @param query The query to be interrogated.
        @param result The candidate result object for which to search.
                If a custom create-result function has been set, and this
                parameter is not a valid result object that the provided
                callbacks can handle, the behavior is undefined. If a custom
                create-result function has not been set, this parameter
                must be a valid MDItemRef.
        @result The index of the given result, or kCFNotFound if the
                value is not one of the query's existing results. If
                you provided a custom result creation function, 
                as well as a custom object comparator function,
                result will be objects created by that function.
*/
MD_EXPORT CFIndex MDQueryGetIndexOfResult(MDQueryRef query, const void *result) MD_AVAIL;

/*!
        @function MDQueryGetAttributeValueOfResultAtIndex
        Returns the value of the named attribute for the result at
                the given index.
        @param query The query to be interrogated.
        @param name The attribute name for which to return the values.
                If the attribute is not one of those requested in the
                valueListAttrs or sortingAttrs parameters to one of
                the query creation functions, the result will be NULL.
        @param idx The index into the query's result list. If the index is
                negative, or is equal to or larger than the current
                number of results in the query, the behavior is undefined.
        @result The value of the attribute, or NULL if the attribute
                doesn't exist in the query on that result.
*/
MD_EXPORT void *MDQueryGetAttributeValueOfResultAtIndex(MDQueryRef query, CFStringRef name, CFIndex idx) MD_AVAIL;

/*!
        @function MDQueryCopyValuesOfAttribute
        Returns the list of values, from the results of the query, of the
                named attribute. The list is not ordered in any way. The
                list contains only one occurrence of each value. Note that
                this list may change over time, as the query's result list
                is updated.
        @param query The query to be interrogated.
        @param name The attribute name for which to return the values.
                If the attribute is not one of those requested in the
                valueListAttrs parameter to one of the query creation
                functions, the behavior is undefined.
        @result A CFArray holding the value objects for that attribute.
*/
MD_EXPORT CFArrayRef MDQueryCopyValuesOfAttribute(MDQueryRef query, CFStringRef name) MD_AVAIL;

/*!
        @function MDQueryGetCountOfResultsWithAttributeValue
        Returns the number of results which have the given attribute and
                attribute value. Note that this count may change over time,
                as the query's result list is updated.
        @param query The query to be interrogated.
        @param name The attribute name for which to return the number
                of results with the given value. If the attribute is not
                one of those requested in the valueListAttrs parameter to
                one of the query creation functions, the behavior is
                undefined.
        @param value The attribute value for which to return the number
                of results with that value. This parameter may be NULL,
                in which case the number of results that do not contain
                the named attribute is returned.
        @result The number of results with that attribute and value.
*/
MD_EXPORT CFIndex MDQueryGetCountOfResultsWithAttributeValue(MDQueryRef query, CFStringRef name, CFTypeRef value) MD_AVAIL;

/*!
 @function MDQuerySetSortOrder
 Sets the sort order for a query.
 @param query The query for which the sort order is to be set.
 @param sortingAttrs An array of attribute names, as in MDQueryCreate. 
  The query's result set will be sorted according to the order of
  these attributes. All names in the array have to have been passed
  as sortingAttrs when the query was created. The attribute names 
  are CFStrings
 @result A boolean, true on success, false on failure.
 */
MD_EXPORT Boolean MDQuerySetSortOrder(MDQueryRef query, CFArrayRef sortingAttrs) API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!   
 @enum MDQuerySortOptionFlags
 @constant kMDQueryReverseSortOrderFlag Sort the attribute in reverse order.
 */
typedef enum {
    kMDQueryReverseSortOrderFlag    = (1<<0),
} MDQuerySortOptionFlags;


/*!
 @function MDQuerySetSortOptionFlagsForAttribute
 Sets the sort flags for a query.
 @param query The query for which the sort flags is to be set.
 @param fieldName The attribute name for which sort option flags are to be set.  
  The attribute name must have been part of the sortingFlags when the query was created.
 @param flags A uint32_t containing MDQuerySortOptionFlags to be applied to the attibute
 @result A boolean, true on success, false on failure.
 */
MD_EXPORT Boolean MDQuerySetSortOptionFlagsForAttribute(MDQueryRef query, CFStringRef fieldName, uint32_t flags) API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDQueryGetSortOptionFlagsForAttribute
 Gets the sort option flags for a sorting attribute.
 @param query The query for which fetch sort option flags.
 @param fieldName The attribute name for which sort option flags are to be fetched.  
 @result A uint32_t, with MDQuerySortOptionFlags set for the attribute.
 */
MD_EXPORT uint32_t MDQueryGetSortOptionFlagsForAttribute(MDQueryRef query, CFStringRef fieldName) API_AVAILABLE( macos(10.7) ) API_UNAVAILABLE( ios, tvos, watchos );


/*!
        @typedef MDQuerySortComparatorFunction
        Type of the callback function used to sort the results of an
                MDQuery.
        @param query The MDQuery instance.
        @param attrs1 A C array of attribute values for a result. The
                values occur in the array in the same order and position
                that the attribute names were passed in the sortingAttrs
                array when the query was created. The values of the
                attributes might be NULL, if the attribute doesn't exist
                on a result or if read access to that attribute is not
                allowed.
        @param attrs2 A C array of attribute values for a result. The
                values occur in the array in the same order and position
                that the attribute names were passed in the sortingAttrs
                array when the query was created. The values of the
                attributes might be NULL, if the attribute doesn't exist
                on a result or if read access to that attribute is not
                allowed.
        @param context The user-defined context parameter given to
                MDQuerySetSortComparator().
        @result The function must return one of the CFComparisonResults
                kCFCompareLessThan, kCFCompareEqualTo, or
                kCFCompareGreaterThan. There is no provision for unordered
                results. The comparison must be a total order relation,
                and additionally produce temporally identical results (that
                is, produce the same results for the same inputs in the
                future as now), for the sort results to be predictable.
*/
typedef CFComparisonResult (*MDQuerySortComparatorFunction)(const CFTypeRef attrs1[], const CFTypeRef attrs2[], void *context);

/*!
        @function MDQuerySetSortComparator
        Sets the function used to sort the results of an MDQuery. You
                may set the comparator function as many times as you
                like, even while the query is executing. Whenever the
                comparator function is set, all results are re-sorted
                using the new comparator function before the function
                returns. The function pointer can be NULL to cancel
                custom sorting and revert to the default sorting.
                The default sort provided by MDQueryCreate()
                is a assending sort strings are compared using
                CFStringCompare() with the options kCFCompareNonliteral |
                kCFCompareLocalized | kCFCompareNumerically. CFDataRefs are
                compared by using memcmp() of the data pointers.
        @param query The query to whose result sort function is to be set.
        @param func The callback function the MDQuery will use to
                sort its results. If the function (when the parameter is
                not NULL) is not of type MDQuerySortComparatorFunction or
                does not behave as a MDQuerySortComparatorFunction must,
                the behavior is undefined. The function pointer may
                be NULL to cancel any custom comparator.
        @param context A pointer-sized user-defined value, which is
                passed as the third parameter to the sort function,
                but is otherwise unused by MDQuery. The MDQuery does
                not retain the context in any way, so it must remain
                valid for the lifetime of the query or until the sort
                function is set again. If the context is not what is
                expected by the comparator, the behavior is undefined.
*/
MD_EXPORT void MDQuerySetSortComparator(MDQueryRef query, MDQuerySortComparatorFunction comparator, void *context)  MD_AVAIL;

/*!
        @function MDQuerySetSortComparatorBlock
        Sets the block used to sort the results of an MDQuery. You
                may set the comparator block as many times as you
                like, even while the query is executing. Whenever the
                comparator block is set, all results are re-sorted
                using the new comparator block before the function
                returns. The block can be NULL to cancel
                custom sorting and revert to the default sorting.
                The default sort provided by MDQueryCreate()
                is a assending sort strings are compared using
                CFStringCompare() with the options kCFCompareNonliteral |
                kCFCompareLocalized | kCFCompareNumerically. CFDataRefs are
                compared by using memcmp() of the data pointers.
        @param query The query to whose result sort block is to be set.
        @param comparator The callback block the MDQuery will use to
                sort its results. The comparator may be called on multiple threads in 
                parallel, and must be reentrant. To take advantage of parallel 
                sorting, it is best to avoid any locking in the comparator. 
                The block may be NULL to cancel any custom comparator. 
*/

#ifdef __BLOCKS__
MD_EXPORT void MDQuerySetSortComparatorBlock(MDQueryRef query, CFComparisonResult (^comparator)(const CFTypeRef attrs1[], const CFTypeRef attrs2[])) API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );
#endif

/*!
        @constant kMDQueryProgressNotification
        The name of the notification sent to indicate changes to the
                query's results list during the initial gathering phase
                of a query's execution. Mostly adds will occur during
                this phase, but removals and changes can also occur, as
                in any update. This info dictionary parameter of the
                notification can carry the kMDQueryUpdateChangedItems
                and kMDQueryUpdateRemovedItems keys. Note that these
                keys may be have empty arrays for values, or be missing,
                if there are no changes of that particular type. For
                performance reasons, added results are not indicated in
                progress notifications (to avoid the cost of creating
                the result objects). These notifications are sent out
                by a query before the kMDQueryDidFinishNotification.
*/
MD_EXPORT const CFStringRef kMDQueryProgressNotification MD_AVAIL;

/*!
        @constant kMDQueryDidFinishNotification
        The name of the notification sent to indicate that the query has
                finished with the initial result-gathering phase, and may
                now proceed into the live-update phase (if that option
                was chosen when the query was executed). This notification
                often shortly follows after the last progress notification.
                It is usually not necessary to update any displayed UI in
                response to this notification, since it doesn't indicate
                any change in the result list of a query.
*/
MD_EXPORT const CFStringRef kMDQueryDidFinishNotification MD_AVAIL;

/*!
        @constant kMDQueryDidUpdateNotification
        The name of the notification sent to indicate changes to the
                query's results list during the second, live-update, phase
                of a query's execution. This notification can carry the
                kMDQueryUpdateAddedItems, kMDQueryUpdateChangedItems,
                and kMDQueryUpdateRemovedItems keys in the info
                dictionary parameter of the notification. Note that these
                keys may be have empty arrays for values, or be missing,
                if there are no changes of that particular type. These
                notifications are sent out by a query after the
                kMDQueryDidUpdateNotification.
*/
MD_EXPORT const CFStringRef kMDQueryDidUpdateNotification MD_AVAIL;

/*!
        @constant kMDQueryUpdateAddedItems
        The name of the key in a query notification's info dictionary
                which identifies the list of added results. A result is
                added if the file contents or some metadata attribute
                of it is changed, and it now matches the query. Result
                objects are created for the newly added results, to be
                put in the list.
*/
MD_EXPORT const CFStringRef kMDQueryUpdateAddedItems MD_AVAIL;

/*!
        @constant kMDQueryUpdateChangedItems
        The name of the key in a query notification's info dictionary
                which identifies the list of changed results. A result
                is changed if the file contents or some metadata
                attribute of it is changed, but it still matches the
                query. The list only contains result objects which have
                previously been created, and does not indicate results
                which have been changed for which result objects have
                not been created.
                [[This is for performance reasons, to avoid creating
                result objects just to represent a change of a result
                which has not been looked at, but this semantic may
                change.]]
*/
MD_EXPORT const CFStringRef kMDQueryUpdateChangedItems MD_AVAIL;

/*!
        @constant kMDQueryUpdateRemovedItems
        The name of the key in a query notification's info dictionary
                which identifies the list of removed results. A result
                can be removed if it no longer matches the query. The
                list only contains result objects which have previously
                been created, and does not indicate results which have
                been removed for which result objects have not been
                created.
                [[This is for performance reasons, to avoid creating
                temporary result objects just to represent the deletion
                of the result, but this semantic may change.]]
*/
MD_EXPORT const CFStringRef kMDQueryUpdateRemovedItems MD_AVAIL;

/*!
        @constant kMDQueryResultContentRelevance
        The name of a query-specific attribute for use in sorting.
                The relevance of an item is a CFNumberRef with a
                floating point value. This is the relevance for
                content searches.
                The maximum and minimum values for a particular
                search cannot be determined until all of the results
                have been returned.  If there are multiple
                kMDItemTextContent predicates in the query, no
                relevance is returned.
                This is an attribute of a result item that is
                specific to the item in the context of the query.
                Also, the relevance does not compare the result
                relative to the other results of a query, but is
                computed just on the result item itself. Finally,
                this is only the relevance value for content,
                not a relevance for the item as a whole. The
                relevance attribute may not even be computed for
                an item if the item is found to match the query
                through evaluation of other attributes of the
                item than its contents. If the value is not
                computed, it is treated as an attribute on the
                item which does not exist (for sorting purposes,
                for example).
*/
MD_EXPORT const CFStringRef kMDQueryResultContentRelevance MD_AVAIL;

/*!
    @function MDQuerySetSearchScope
    @discussion Use MDQuerySetSearchScope to limit the results
             returned by the query engine to those MDItemRefs that
             appear  within the specified directories.  This may be
             used to limit searching to particular volumes. Tilde
			 paths, or environment variables are not expanded.
			 Calling this multiple times will replace the previous
			 options. This must be called before the query is executed.
    @param query The query object to modify.
    @param scopeDirectories a CFArray of CFStringRef or CFURLRef objects which
           specify where to search.  For conveinience, the kMDQueryScopeHome,
			kMDQueryScopeComputer and kMDQueryScopeNetwork constants may also
			be present in this array.
    @param scopeOptions additional options for modifying the search.
           Currently, pass 0 (zero).
 */

MD_EXPORT void MDQuerySetSearchScope(MDQueryRef query, CFArrayRef scopeDirectories, OptionBits scopeOptions) MD_AVAIL;

/*!
@constant kMDQueryScopeHome
	A constant, which can be passed in the scopeDirectories array, to specify
	that the search should be restricted to the volume and directory that contains
	the current user's home directory
 */
MD_EXPORT const CFStringRef kMDQueryScopeHome MD_AVAIL;

/*!
@constant kMDQueryScopeComputer
	A constant, which can be passed in the scopeDirectories array, to specify
	that the search should be restricted to all locally mounted volumes, plus the user's
	home directory (which may be on a remote volume).
 */
MD_EXPORT const CFStringRef kMDQueryScopeComputer MD_AVAIL;

/*!
@constant kMDQueryScopeNetwork
	A constant, which can be passed in the scopeDirectories array, to specify
	that the search should include all user mounted remote volumes.
 */
MD_EXPORT const CFStringRef kMDQueryScopeNetwork MD_AVAIL;

/*!
 @constant kMDQueryScopeAllIndexed
 A constant, which can be passed in the scopeDirectories array, to specify
 that the search should be restricted to indexed, locally mounted volumes and
 indexed user mounted remote volumes, plus the user's home directory.
 */
MD_EXPORT const CFStringRef kMDQueryScopeAllIndexed API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @constant kMDQueryScopeComputerIndexed
 A constant, which can be passed in the scopeDirectories array, to specify
 that the search should be restricted to indexed, locally mounted volumes, plus the user's
 home directory (which may be on a remote volume).
 */
MD_EXPORT const CFStringRef kMDQueryScopeComputerIndexed API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @constant kMDQueryScopeNetworkIndexed
 A constant, which can be passed in the scopeDirectories array, to specify
 that the search should include indexed user mounted remote volumes.
 */
MD_EXPORT const CFStringRef kMDQueryScopeNetworkIndexed API_AVAILABLE( macos(10.6) ) API_UNAVAILABLE( ios, tvos, watchos );

/*!
 @function MDQuerySetMaxCount
 @discussion Use MDQuerySetMaxCount to limit the number of results
 returned by the query engine.  This must be called before the query is executed.
 @param query The query object to modify.
 @param size The maximum number of results desired.
 */
MD_EXPORT void MDQuerySetMaxCount(MDQueryRef query, CFIndex size) MD_AVAIL_LEOPARD;

MD_END_C_DECLS
CF_IMPLICIT_BRIDGING_DISABLED

#endif /* MAC_OS_X_VERSION_MAX_ALLOWED */

#endif /* __MDQUERY__ */
