#ifndef __ENDPOINT_SECURITY_TYPES_H
#define __ENDPOINT_SECURITY_TYPES_H

#include <sys/types.h>
#include <stdint.h>
#include <os/base.h>
#include <mach/message.h>

typedef struct {
	uint8_t reserved[32];
} es_event_id_t;

typedef enum {
    ES_ACTION_TYPE_AUTH
  , ES_ACTION_TYPE_NOTIFY
} es_action_type_t;

typedef enum {
    ES_SET
  , ES_CLEAR
} es_set_or_clear_t;

/**
 * The valid event types recognized by EndpointSecurity
 *
 * @discussion When a program subscribes to and receives an AUTH-related event, it must respond
 * with an appropriate result indicating whether or not the operation should be allowed to continue.
 * The valid API options are:
 * - es_respond_auth_result
 * - es_respond_flags_result
 *
 * Currently, only ES_EVENT_TYPE_AUTH_OPEN must use es_respond_flags_result. All other AUTH events
 * must use es_respond_auth_result.
 */
typedef enum {
    // The following events are available beginning in macOS 10.15
    ES_EVENT_TYPE_AUTH_EXEC
  , ES_EVENT_TYPE_AUTH_OPEN
  , ES_EVENT_TYPE_AUTH_KEXTLOAD
  , ES_EVENT_TYPE_AUTH_MMAP
  , ES_EVENT_TYPE_AUTH_MPROTECT
  , ES_EVENT_TYPE_AUTH_MOUNT
  , ES_EVENT_TYPE_AUTH_RENAME
  , ES_EVENT_TYPE_AUTH_SIGNAL
  , ES_EVENT_TYPE_AUTH_UNLINK
  , ES_EVENT_TYPE_NOTIFY_EXEC
  , ES_EVENT_TYPE_NOTIFY_OPEN
  , ES_EVENT_TYPE_NOTIFY_FORK
  , ES_EVENT_TYPE_NOTIFY_CLOSE
  , ES_EVENT_TYPE_NOTIFY_CREATE
  , ES_EVENT_TYPE_NOTIFY_EXCHANGEDATA
  , ES_EVENT_TYPE_NOTIFY_EXIT
  , ES_EVENT_TYPE_NOTIFY_GET_TASK
  , ES_EVENT_TYPE_NOTIFY_KEXTLOAD
  , ES_EVENT_TYPE_NOTIFY_KEXTUNLOAD
  , ES_EVENT_TYPE_NOTIFY_LINK
  , ES_EVENT_TYPE_NOTIFY_MMAP
  , ES_EVENT_TYPE_NOTIFY_MPROTECT
  , ES_EVENT_TYPE_NOTIFY_MOUNT
  , ES_EVENT_TYPE_NOTIFY_UNMOUNT
  , ES_EVENT_TYPE_NOTIFY_IOKIT_OPEN
  , ES_EVENT_TYPE_NOTIFY_RENAME
  , ES_EVENT_TYPE_NOTIFY_SETATTRLIST
  , ES_EVENT_TYPE_NOTIFY_SETEXTATTR
  , ES_EVENT_TYPE_NOTIFY_SETFLAGS
  , ES_EVENT_TYPE_NOTIFY_SETMODE
  , ES_EVENT_TYPE_NOTIFY_SETOWNER
  , ES_EVENT_TYPE_NOTIFY_SIGNAL
  , ES_EVENT_TYPE_NOTIFY_UNLINK
  , ES_EVENT_TYPE_NOTIFY_WRITE
  , ES_EVENT_TYPE_AUTH_FILE_PROVIDER_MATERIALIZE
  , ES_EVENT_TYPE_NOTIFY_FILE_PROVIDER_MATERIALIZE
  , ES_EVENT_TYPE_AUTH_FILE_PROVIDER_UPDATE
  , ES_EVENT_TYPE_NOTIFY_FILE_PROVIDER_UPDATE
  , ES_EVENT_TYPE_AUTH_READLINK
  , ES_EVENT_TYPE_NOTIFY_READLINK
  , ES_EVENT_TYPE_AUTH_TRUNCATE
  , ES_EVENT_TYPE_NOTIFY_TRUNCATE
  , ES_EVENT_TYPE_AUTH_LINK
  , ES_EVENT_TYPE_NOTIFY_LOOKUP
  , ES_EVENT_TYPE_AUTH_CREATE
  , ES_EVENT_TYPE_AUTH_SETATTRLIST
  , ES_EVENT_TYPE_AUTH_SETEXTATTR
  , ES_EVENT_TYPE_AUTH_SETFLAGS
  , ES_EVENT_TYPE_AUTH_SETMODE
  , ES_EVENT_TYPE_AUTH_SETOWNER
    // The following events are available beginning in macOS 10.15.1
  , ES_EVENT_TYPE_AUTH_CHDIR
  , ES_EVENT_TYPE_NOTIFY_CHDIR
  , ES_EVENT_TYPE_AUTH_GETATTRLIST
  , ES_EVENT_TYPE_NOTIFY_GETATTRLIST
  , ES_EVENT_TYPE_NOTIFY_STAT
  , ES_EVENT_TYPE_NOTIFY_ACCESS
  , ES_EVENT_TYPE_AUTH_CHROOT
  , ES_EVENT_TYPE_NOTIFY_CHROOT
  , ES_EVENT_TYPE_AUTH_UTIMES
  , ES_EVENT_TYPE_NOTIFY_UTIMES
  , ES_EVENT_TYPE_AUTH_CLONE
  , ES_EVENT_TYPE_NOTIFY_CLONE
  , ES_EVENT_TYPE_NOTIFY_FCNTL
  , ES_EVENT_TYPE_AUTH_GETEXTATTR
  , ES_EVENT_TYPE_NOTIFY_GETEXTATTR
  , ES_EVENT_TYPE_AUTH_LISTEXTATTR
  , ES_EVENT_TYPE_NOTIFY_LISTEXTATTR
  , ES_EVENT_TYPE_AUTH_READDIR
  , ES_EVENT_TYPE_NOTIFY_READDIR
  , ES_EVENT_TYPE_AUTH_DELETEEXTATTR
  , ES_EVENT_TYPE_NOTIFY_DELETEEXTATTR
  , ES_EVENT_TYPE_AUTH_FSGETPATH
  , ES_EVENT_TYPE_NOTIFY_FSGETPATH
  , ES_EVENT_TYPE_NOTIFY_DUP
  , ES_EVENT_TYPE_AUTH_SETTIME
  , ES_EVENT_TYPE_NOTIFY_SETTIME
  , ES_EVENT_TYPE_NOTIFY_UIPC_BIND
  , ES_EVENT_TYPE_AUTH_UIPC_BIND
  , ES_EVENT_TYPE_NOTIFY_UIPC_CONNECT
  , ES_EVENT_TYPE_AUTH_UIPC_CONNECT
  , ES_EVENT_TYPE_AUTH_EXCHANGEDATA
  , ES_EVENT_TYPE_AUTH_SETACL
  , ES_EVENT_TYPE_NOTIFY_SETACL
	// The following events are available beginning in macOS 10.15.4
  , ES_EVENT_TYPE_NOTIFY_PTY_GRANT
  , ES_EVENT_TYPE_NOTIFY_PTY_CLOSE
  , ES_EVENT_TYPE_AUTH_PROC_CHECK
  , ES_EVENT_TYPE_NOTIFY_PROC_CHECK
  , ES_EVENT_TYPE_AUTH_GET_TASK
	// The following events are available beginning in macOS 11.0
  , ES_EVENT_TYPE_AUTH_SEARCHFS
  , ES_EVENT_TYPE_NOTIFY_SEARCHFS
  , ES_EVENT_TYPE_AUTH_FCNTL
  , ES_EVENT_TYPE_AUTH_IOKIT_OPEN
  , ES_EVENT_TYPE_AUTH_PROC_SUSPEND_RESUME
  , ES_EVENT_TYPE_NOTIFY_PROC_SUSPEND_RESUME
  , ES_EVENT_TYPE_NOTIFY_CS_INVALIDATED
  , ES_EVENT_TYPE_NOTIFY_GET_TASK_NAME
  , ES_EVENT_TYPE_NOTIFY_TRACE
  , ES_EVENT_TYPE_NOTIFY_REMOTE_THREAD_CREATE
  , ES_EVENT_TYPE_AUTH_REMOUNT
  , ES_EVENT_TYPE_NOTIFY_REMOUNT
	// The following events are available beginning in macOS 11.3
  , ES_EVENT_TYPE_AUTH_GET_TASK_READ
  , ES_EVENT_TYPE_NOTIFY_GET_TASK_READ
  , ES_EVENT_TYPE_NOTIFY_GET_TASK_INSPECT
	// The following events are available beginning in macOS 12.0
  , ES_EVENT_TYPE_NOTIFY_SETUID
  , ES_EVENT_TYPE_NOTIFY_SETGID
  , ES_EVENT_TYPE_NOTIFY_SETEUID
  , ES_EVENT_TYPE_NOTIFY_SETEGID
  , ES_EVENT_TYPE_NOTIFY_SETREUID
  , ES_EVENT_TYPE_NOTIFY_SETREGID
  , ES_EVENT_TYPE_AUTH_COPYFILE
  , ES_EVENT_TYPE_NOTIFY_COPYFILE
    // ES_EVENT_TYPE_LAST is not a valid event type but a convenience
    // value for operating on the range of defined event types.
    // This value may change between releases and was available
    // beginning in macOS 10.15
  , ES_EVENT_TYPE_LAST
} es_event_type_t;

/**
 * @brief Valid authorization values to be used when responding to a es_message_t auth event
 */
typedef enum {
	/// The event is authorized and should be allowed to continue
	ES_AUTH_RESULT_ALLOW
    /// The event is not authorized and should be blocked
  , ES_AUTH_RESULT_DENY
} es_auth_result_t;

/**
 * @brief Valid values for the result_type of es_result_t to indicate the appropriate union member to use
 */
typedef enum {
	/// The result is an auth result
	ES_RESULT_TYPE_AUTH
    /// The result is a flags result
  , ES_RESULT_TYPE_FLAGS
} es_result_type_t;

/**
 @brief Return value for functions that can only fail in one way
 */
typedef enum {
	ES_RETURN_SUCCESS
  , ES_RETURN_ERROR
} es_return_t;

/**
 @brief Error conditions for responding to a message
 */
typedef enum {
	ES_RESPOND_RESULT_SUCCESS
	///One or more invalid arguments were provided
  , ES_RESPOND_RESULT_ERR_INVALID_ARGUMENT
	///Communication with the ES subsystem failed
  , ES_RESPOND_RESULT_ERR_INTERNAL
	///The message being responded to could not be found
  , ES_RESPOND_RESULT_NOT_FOUND
	///The provided message has been responded to more than once
  , ES_RESPOND_RESULT_ERR_DUPLICATE_RESPONSE
	///Either an inappropriate response API was used for the event type (ensure using proper
	///es_respond_auth_result or es_respond_flags_result function) or the event is notification only.
  , ES_RESPOND_RESULT_ERR_EVENT_TYPE
} es_respond_result_t;

/**
 @brief Error conditions for creating a new client
 */
typedef enum {
	ES_NEW_CLIENT_RESULT_SUCCESS
	///One or more invalid arguments were provided
  , ES_NEW_CLIENT_RESULT_ERR_INVALID_ARGUMENT
	///Communication with the ES subsystem failed
  , ES_NEW_CLIENT_RESULT_ERR_INTERNAL
	///The caller is not properly entitled to connect
  , ES_NEW_CLIENT_RESULT_ERR_NOT_ENTITLED
	///The caller is not permitted to connect. They lack Transparency, Consent, and Control (TCC) approval form the user.
  , ES_NEW_CLIENT_RESULT_ERR_NOT_PERMITTED
	///The caller is not running as root
  , ES_NEW_CLIENT_RESULT_ERR_NOT_PRIVILEGED
	/// The caller has reached the maximum number of allowed simultaneously connected clients
  , ES_NEW_CLIENT_RESULT_ERR_TOO_MANY_CLIENTS
} es_new_client_result_t;

/**
 @brief Error conditions for clearing the authorisation caches
 */
typedef enum {
	ES_CLEAR_CACHE_RESULT_SUCCESS
	///Communication with the ES subsystem failed
  , ES_CLEAR_CACHE_RESULT_ERR_INTERNAL
	///Rate of calls is too high. Slow down.
  , ES_CLEAR_CACHE_RESULT_ERR_THROTTLE
} es_clear_cache_result_t;

/**
 * @brief Structure buffer with size
 */
typedef struct {
	// Size of the `data` field in bytes
	size_t size;
	const uint8_t * data;
} es_token_t;

/**
 * @brief Structure for handling strings
 */
typedef struct {
	// Length of the `data` field. Equivalent to strlen().
	size_t length;
	const char * data;
} es_string_token_t;

/*
 * @brief Values that will be paired with path strings to describe the type of the path
 */
typedef enum {
	/// Value to describe a path prefix
	ES_MUTE_PATH_TYPE_PREFIX
	/// Value to describe a path literal
  , ES_MUTE_PATH_TYPE_LITERAL
} es_mute_path_type_t;

/*
 * Structure to describe attributes of a muted path.
 *
 * @field type Indicates if the path is a prefix or literal.
 * @field event_count The number of events contained in the `events` array.
 * @field events Array of event types for which the path is muted.
 * @field path The muted path. (Note: es_string_token_t is a char array and length)
 */
typedef struct {
	es_mute_path_type_t type;
	size_t event_count;
	const es_event_type_t *events;
	es_string_token_t path;
} es_muted_path_t;

/*
 * Structure for a set of muted paths.
 *
 * @field count The number of elements in the `paths` array.
 * @field paths Array of muted paths.
 */
typedef struct {
	size_t count;
	const es_muted_path_t *paths;
} es_muted_paths_t;

/*
 * Structure to describe attributes of a muted process.
 *
 * @field token The audit token of a muted process.
 * @field event_count The number of events contained in the `events` array.
 * @field events Array of event types for which the process is muted.
 */
typedef struct {
	audit_token_t audit_token;
	size_t event_count;
	const es_event_type_t *events;
} es_muted_process_t;

/*
 * Structure for a set of muted processes.
 *
 * @field count The number of elements in the `processes` array.
 * @field processes Array of muted processes.
 */
typedef struct {
	size_t count;
	const es_muted_process_t *processes;
} es_muted_processes_t;

#endif /* __ENDPOINT_SECURITY_TYPES_H */
