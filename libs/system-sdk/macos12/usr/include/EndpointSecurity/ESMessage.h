#ifndef __ENDPOINT_SECURITY_MESSAGE_H
#define __ENDPOINT_SECURITY_MESSAGE_H

#ifndef __ENDPOINT_SECURITY_INDIRECT__
#error "Please #include <EndpointSecurity/EndpointSecurity.h> instead of this file directly."
#endif /* __ENDPOINT_SECURITY_INDIRECT__ */

#include <mach/message.h>
#include <stdbool.h>
#include <sys/acl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <os/availability.h>
#include <os/base.h>

#if !__DARWIN_64_BIT_INO_T
	#error This header requires __DARWIN_64_BIT_INO_T
#endif /* !__DARWIN_64_BIT_INO_T */

/**
 * The EndpointSecurity subsystem is responsible for creating, populating and
 * delivering these data structures to ES clients.
 */

/**
 * @brief es_file_t provides the stat information and path to a file that relates to a security
 * event. The path may be truncated, which is indicated by the path_truncated flag.
 *
 * @discussion Currently, path lengths are supported up to 16K in length (though this
 * number is subject to change at any time and users must not rely on this value being
 * constant). If a path is longer than the supported maximum length, it will be marked
 * as truncated via the `path_truncated` member.
 */
typedef struct {
	es_string_token_t path;
	bool path_truncated;
	struct stat stat;
} es_file_t;

/**
 * @brief Information related to a thread.
 *
 * @field thread_id The unique thread ID of the thread.
 */
typedef struct {
	uint64_t thread_id;
} es_thread_t;

/**
 * @brief Information related to a process. This is used both for describing processes that
 * instigated an event (e.g. in the case of the `es_message_t` `process` field, or are targets
 * of an event (e.g. for exec events this describes the new process being executed, for signal
 * events this describes the process that will receive the signal).
 *
 * @field audit_token Audit token of the process.
 * @field ppid Parent pid of the process. It is recommended to instead use the parent_audit_token field.
 *        @see parent_audit_token
 * @field original_ppid Original ppid of the process.  This field stays constant even in the event
 *        this process is reparented.
 * @field group_id Process group id the process belongs to.
 * @field session_id Session id the process belongs to.
 * @field codesigning_flags Code signing flags of the process.  The values for these flags can be
 *        found in the include file `cs_blobs.h` (`#include <kern/cs_blobs.h>`).
 * @field is_es_client Indicates this process has the Endpoint Security entitlement.
 * @field cdhash The code directory hash of the code signature associated with this process.
 * @field signing_id The signing id of the code signature associated with this process.
 * @field team_id The team id of the code signature associated with this process.
 * @field executable The executable file that is executing in this process.
 * @field tty The TTY this process is associated with, or NULL if the process does not have an
 *        associated TTY.
 *        Field available only if message version >= 2.
 * @field start_time Process start time, i.e. time of fork creating this process.
 *        Field available only if message version >= 3.
 * @field responsible_audit_token audit token of the process responsible for this process, which
 *        may be the process itself in case there is no responsible process or the responsible
 *        process has already exited.
 *        Field available only if message version >= 4.
 * @field parent_audit_token The audit token of the parent process
 *        Field available only if message version >= 4.
 *
 * @discussion
 * - Values such as PID, UID, GID, etc. can be extracted from audit tokens via API in libbsm.h.
 * - Clients should take caution when processing events where `is_es_client` is true. If multiple ES
 *   clients exist, actions taken by one client could trigger additional actions by the other client,
 *   causing a potentially infinite cycle.
 * - Fields related to code signing in the target `es_process_t` reflect the state of the process
 *   at the time the message is generated.  In the specific case of exec, this is after the exec
 *   completed in the kernel, but before any code in the process has started executing.  At that
 *   point, XNU has validated the signature itself and has verified that the CDHash is correct in
 *   that the hash of all the individual page hashes in the Code Directory matches the signed CDHash,
 *   essentially verifying the signature was not tampered with.  However, individual page hashes are
 *   not verified by XNU until the corresponding pages are paged in once they are accessed while the
 *   binary executes.  It is not until the individual pages are paged in that XNU determines if a
 *   binary has been tampered with and will update the code signing flags accordingly.
 *   EndpointSecurity provides clients the current state of the CS flags in the `codesigning_flags`
 *   member of the `es_process_t` struct.  The CS_VALID bit in the `codesigning_flags` means that
 *   everything the kernel has validated up to that point in time was valid, but not that there has
 *   been a full validation of all the pages in the executable file.  If page content has been
 *   tampered with in the executable, we won't know until that page is paged in.  At that time, the
 *   process will have its CS_VALID bit cleared and, if CS_KILL is set, the process will be killed,
 *   preventing any tampered code from being executed.  CS_KILL is generally set for platform
 *   binaries and for binaries having opted into the hardened runtime.  An ES client wishing to
 *   detect tampered code before it is paged in, for example at exec time, can use the Security
 *   framework to do so, but should be cautious of the potentially significant performance cost.  The
 *   EndpointSecurity subsystem itself has no role in verifying the validity of code signatures.
 */
typedef struct {
	audit_token_t audit_token;
	pid_t ppid;
	pid_t original_ppid;
	pid_t group_id;
	pid_t session_id;
	uint32_t codesigning_flags;
	bool is_platform_binary;
	bool is_es_client;
	uint8_t cdhash[20];
	es_string_token_t signing_id;
	es_string_token_t team_id;
	es_file_t * _Nonnull executable;
	es_file_t * _Nullable tty; /* field available only if message version >= 2 */
	struct timeval start_time; /* field available only if message version >= 3 */
	audit_token_t responsible_audit_token; /* field available only if message version >= 4 */
	audit_token_t parent_audit_token; /* field available only if message version >= 4 */
} es_process_t;


/**
 * @brief Describes machine-specific thread state as used by `thread_create_running` and other
 * Mach API functions.
 *
 * @field flavor Indicates the representation of the machine-specific thread state.
 * @field state The machine-specific thread state, equivalent to thread_state_t in Mach APIs.
 *
 * @note The size subfield of the state field is in bytes, NOT natural_t units.  Definitions
 * for working with thread state can be found in the include file `mach/thread_status.h` and
 * corresponding machine-dependent headers.
 */
typedef struct {
	int flavor;
	es_token_t state;
} es_thread_state_t;


/**
 * @brief Structure for describing an open file descriptor
 *
 * @field fd File descriptor number
 * @field fdtype File descriptor type, as libproc fdtype
 *
 * Fields available only if fdtype == PROX_FDTYPE_PIPE:
 * @field extra.pipe.pipe_id Unique id of the pipe for correlation with other
 *        file descriptors pointing to the same or other end of the same pipe.
 */
typedef struct {
	int32_t fd;
	uint32_t fdtype;
	union {
		struct {
			uint64_t pipe_id;
		} pipe;
	};
} es_fd_t;

/**
 * @brief Execute a new process
 *
 * @field target The new process that is being executed
 * @field reserved0 This field must not be accessed directly (see notes)
 * @field script Script being executed by interpreter. This field is only valid if a script was
 *        executed directly and not as an argument to the interpreter (e.g. `./foo.sh` not `/bin/sh ./foo.sh`)
 *        Field available only if message version >= 2.
 * @field cwd Current working directory at exec time.
 *        Field available only if message version >= 3.
 * @field last_fd Highest open file descriptor after the exec completed.
 *        This number is equal to or larger than the highest number of file descriptors available
 *        via `es_exec_fd_count` and `es_exec_fd`, in which case EndpointSecurity has capped the
 *        number of file descriptors available in the message.  File descriptors for open files are
 *        not necessarily contiguous.  The exact number of open file descriptors is not available.
 *        Field available only if message version >= 4.
 *
 * @note Process arguments, environment variables and file descriptors are packed, use API functions
 * to access them: `es_exec_arg`, `es_exec_arg_count`, `es_exec_env`, `es_exec_env_count`,
 * `es_exec_fd` and `es_exec_fd_count`.
 *
 * @note The API may only return descriptions for a subset of open file descriptors; how many and
 * which file descriptors are available as part of exec events is not considered API and can change
 * in future releases.
 *
 * @note Fields related to code signing in `target` represent kernel state for the process at the
 * point in time the exec has completed, but the binary has not started running yet.  Because code
 * pages are not validated until they are paged in, this means that modifications to code pages
 * would not have been detected yet at this point.  For a more thorough explanation, please see the
 * documentation for `es_process_t`.
 *
 * @note There are two `es_process_t` fields that are represented in an `es_message_t` that contains
 * an `es_event_exec_t`. The `es_process_t` within the `es_message_t` struct (named "process")
 * contains information about the program that calls execve(2) (or posix_spawn(2)). This information
 * is gathered prior to the program being replaced. The other `es_process_t`, within the
 * `es_event_exec_t` struct (named "target"), contains information about the program after the image
 * has been replaced by execve(2) (or posix_spawn(2)). This means that both `es_process_t` structs
 * refer to the same process, but not necessarily the same program. Also, note that the
 * `audit_token_t` structs contained in the two different `es_process_t` structs will not be
 * identical: the pidversion field will be updated, and the UID/GID values may be different if the
 * new program had setuid/setgid permission bits set.
 */
typedef struct {
	es_process_t * _Nonnull target;
	es_token_t reserved0;
	union {
		uint8_t reserved[64];
		struct {
			es_file_t * _Nullable script; /* field available only if message version >= 2 */
			es_file_t * _Nonnull cwd; /* field available only if message version >= 3 */
			int last_fd; /* field available only if message version >= 4 */
		};
	};
} es_event_exec_t;

/**
 * @brief Open a file system object
 *
 * @field fflag The desired flags to be used when opening `file` (see note)
 * @field file The file that will be opened
 *
 * @note: The `fflag` field represents the mask as applied by the kernel, not as represented by typical
 * open(2) `oflag` values. When responding to `ES_EVENT_TYPE_AUTH_OPEN` events using
 * es_respond_flags_result(), ensure that the same FFLAG values are used (e.g. FREAD, FWRITE instead
 * of O_RDONLY, O_RDWR, etc...).
 *
 * @see fcntl.h
 */
typedef struct {
	int32_t fflag;
	es_file_t * _Nonnull file;
	uint8_t reserved[64];
} es_event_open_t;

/**
 * @brief Load a kernel extension
 *
 * @field identifier The signing identifier of the kext being loaded
 */
typedef struct {
	es_string_token_t identifier;
	uint8_t reserved[64];
} es_event_kextload_t;

/**
 * @brief Unload a kernel extension
 *
 * @field identifier The signing identifier of the kext being unloaded
 */
typedef struct {
	es_string_token_t identifier;
	uint8_t reserved[64];
} es_event_kextunload_t;

/**
 * @brief Unlink a file system object
 *
 * @field target The object that will be removed
 * @field parent_dir The parent directory of the `target` file system object
 *
 * @note This event can fire multiple times for a single syscall, for example when the syscall
 *       has to be retried due to racing VFS operations.
 */
typedef struct {
	es_file_t * _Nonnull target;
	es_file_t * _Nonnull parent_dir;
	uint8_t reserved[64];
} es_event_unlink_t;

/**
 * @brief Memory map a file
 *
 * @field protection The protection (region accessibility) value
 * @field max_protection The maximum allowed protection value the operating system will respect
 * @field flags The type and attributes of the mapped file
 * @field file_pos The offset into `source` that will be mapped
 * @field source The file system object being mapped
 */
typedef struct {
	int32_t protection;
	int32_t max_protection;
	int32_t flags;
	uint64_t file_pos;
	es_file_t * _Nonnull source;
	uint8_t reserved[64];
} es_event_mmap_t;

/**
 * @brief Link to a file
 *
 * @field source The existing object to which a hard link will be created
 * @field target_dir The directory in which the link will be created
 * @field target_filename The name of the new object linked to `source`
 */
typedef struct {
	es_file_t * _Nonnull source;
	es_file_t * _Nonnull target_dir;
	es_string_token_t target_filename;
	uint8_t reserved[64];
} es_event_link_t;

/**
 * @brief Mount a file system
 *
 * @field statfs The file system stats for the file system being mounted
 */
typedef struct {
	struct statfs * _Nonnull statfs;
	uint8_t reserved[64];
} es_event_mount_t;

/**
 * @brief Unmount a file system
 *
 * @field statfs The file system stats for the file system being unmounted
 */
typedef struct {
	struct statfs * _Nonnull statfs;
	uint8_t reserved[64];
} es_event_unmount_t;

/**
 * @brief Remount a file system
 *
 * @field statfs The file system stats for the file system being remounted
 */
typedef struct {
	struct statfs * _Nonnull statfs;
	uint8_t reserved[64];
} es_event_remount_t;

/**
 * @brief Fork a new process
 *
 * @field child The child process that was created
 */
typedef struct {
	es_process_t * _Nonnull child;
	uint8_t reserved[64];
} es_event_fork_t;

/**
 * @brief Control protection of pages
 *
 * @field protection The desired new protection value
 * @field address The base address to which the protection value will apply
 * @field size The size of the memory region the protection value will apply
 */
typedef struct {
	int32_t protection;
	user_addr_t address;
	user_size_t size;
	uint8_t reserved[64];
} es_event_mprotect_t;

/**
 * @brief Send a signal to a process
 *
 * @field sig The signal number to be delivered
 * @field target The process that will receive the signal
 *
 * @note This event will not fire if a process sends a signal to itself.
 */
typedef struct {
	int sig;
	es_process_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_signal_t;

typedef enum {
	ES_DESTINATION_TYPE_EXISTING_FILE,
	ES_DESTINATION_TYPE_NEW_PATH,
} es_destination_type_t;

/**
 * @brief Rename a file system object
 *
 * @field source The source file that is being renamed
 * @field destination_type Whether or not the destination refers to an existing or new file
 * @field destination Information about the destination of the renamed file (see note)
 * @field existing_file The destination file that will be overwritten
 * @field new_path Information regarding the destination of a newly created file
 * @field dir The directory into which the file will be renamed
 * @field filename The name of the new file that will be created
 *
 * @note The `destination_type` field describes which member in the `destination` union should
 * accessed. `ES_DESTINATION_TYPE_EXISTING_FILE` means that `existing_file` should be used,
 * `ES_DESTINATION_TYPE_NEW_PATH` means that the `new_path` struct should be used.
 *
 * @note This event can fire multiple times for a single syscall, for example when the syscall
 *       has to be retried due to racing VFS operations.
 */
typedef struct {
	es_file_t * _Nonnull source;
	es_destination_type_t destination_type;
	union {
		es_file_t * _Nonnull existing_file;
		struct {
			es_file_t * _Nonnull dir;
			es_string_token_t filename;
		} new_path;
	} destination;
	uint8_t reserved[64];
} es_event_rename_t;

/**
 * @brief Set an extended attribute
 *
 * @field target The file for which the extended attribute will be set
 * @field extattr The extended attribute which will be set
 */
typedef struct {
	es_file_t * _Nonnull target;
	es_string_token_t extattr;
	uint8_t reserved[64];
} es_event_setextattr_t;

/**
 * @brief Retrieve an extended attribute
 *
 * @field target The file for which the extended attribute will be retrieved
 * @field extattr The extended attribute which will be retrieved
 */
typedef struct {
	es_file_t * _Nonnull target;
	es_string_token_t extattr;
	uint8_t reserved[64];
} es_event_getextattr_t;

/**
 * @brief Delete an extended attribute
 *
 * @field target The file for which the extended attribute will be deleted
 * @field extattr The extended attribute which will be deleted
 */
typedef struct {
	es_file_t * _Nonnull target;
	es_string_token_t extattr;
	uint8_t reserved[64];
} es_event_deleteextattr_t;

/**
 * @brief Modify file mode
 *
 * @field mode The desired new mode
 * @field target The file for which mode information will be modified
 *
 * @note The `mode` member is the desired new mode. The `target`
 * member's `stat` information contains the current mode.
 */
typedef struct {
	mode_t mode;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_setmode_t;

/**
 * @brief Modify file flags information
 *
 * @field flags The desired new flags
 * @field target The file for which flags information will be modified
 *
 * @note The `flags` member is the desired set of new flags. The `target`
 * member's `stat` information contains the current set of flags.
 */
typedef struct {
	uint32_t flags;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_setflags_t;

/**
 * @brief Modify file owner information
 *
 * @field uid The desired new UID
 * @field gid The desired new GID
 * @field target The file for which owner information will be modified
 *
 * @note The `uid` and `gid` members are the desired new values. The `target`
 * member's `stat` information contains the current uid and gid values.
 */
typedef struct {
	uid_t uid;
	gid_t gid;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_setowner_t;

/**
 * @brief Close a file descriptor
 *
 * @field modified Set to TRUE if the target file being closed has been modified
 * @field target The file that is being closed
 */
typedef struct {
	bool modified;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_close_t;

/**
 * @brief Create a file system object
 *
 * @field destination_type Whether or not the destination refers to an existing file (see note)
 * @field destination Information about the destination of the new file (see note)
 * @field existing_file The file system object that was created
 * @field dir The directory in which the new file system object will be created
 * @field filename The name of the new file system object to create
 * @field acl The ACL that the new file system object got or gets created with.
 *        May be NULL if the file system object gets created without ACL.
 *        @note The acl provided cannot be directly used by functions within
 *        the <sys/acl.h> header. These functions can mutate the struct passed
 *        into them, which is not compatible with the immutable nature of
 *        es_message_t. Additionally, because this field is minimally constructed,
 *        you must not use `acl_dup(3)` to get a mutable copy, as this can lead to
 *        out of bounds memory access. To obtain a acl_t struct that is able to be
 *        used with all functions within <sys/acl.h>, please use a combination of
 *        `acl_copy_ext(3)` followed by `acl_copy_int(3)`.
 *        Field available only if message version >= 2.
 *
 * @note If an object is being created but has not yet been created, the
 * `destination_type` will be `ES_DESTINATION_TYPE_NEW_PATH`.
 *
 * @note Typically `ES_EVENT_TYPE_NOTIFY_CREATE` events are fired after the
 * object has been created and the `destination_type` will be
 * `ES_DESTINATION_TYPE_EXISTING_FILE`. The exception to this is for
 * notifications that occur if an ES client responds to an
 * `ES_EVENT_TYPE_AUTH_CREATE` event with `ES_AUTH_RESULT_DENY`.
 *
 * @note This event can fire multiple times for a single syscall, for example when the syscall
 *       has to be retried due to racing VFS operations.
 */
typedef struct {
	es_destination_type_t destination_type;
	union {
		es_file_t * _Nonnull existing_file;
		struct {
			es_file_t * _Nonnull dir;
			es_string_token_t filename;
			mode_t mode;
		} new_path;
	} destination;
	uint8_t reserved2[16];
	union {
		uint8_t reserved[48];
		struct {
			acl_t _Nullable acl; /* field available only if message version >= 2 */
		};
	};
} es_event_create_t;

/**
 * @brief Terminate a process
 *
 * @field stat The exit status of a process (same format as wait(2))
 */
typedef struct {
	int stat;
	uint8_t reserved[64];
} es_event_exit_t;

/**
 * @brief Exchange data atomically between two files
 *
 * @field file1 The first file to be exchanged
 * @field file2 The second file to be exchanged
 */
typedef struct {
	es_file_t * _Nonnull file1;
	es_file_t * _Nonnull file2;
	uint8_t reserved[64];
} es_event_exchangedata_t;

/**
 * @brief Write to a file
 *
 * @field target The file being written to
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_write_t;

/**
 * @brief Truncate a file
 *
 * @field target The file that is being truncated
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_truncate_t;

/**
 * @brief Change directories
 *
 * @field target The desired new current working directory
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_chdir_t;

/**
 * @brief View stat information of a file
 *
 * @field target The file for which stat information will be retrieved
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_stat_t;

/**
 * @brief Change the root directory for a process
 *
 * @field target The directory which will be the new root
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_chroot_t;

/**
 * @brief List extended attributes of a file
 *
 * @field target The file for which extended attributes are being retrieved
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_listextattr_t;

/**
 * @brief Open a connection to an I/O Kit IOService
 *
 * @field user_client_type A constant specifying the type of connection to be
 *        created, interpreted only by the IOService's family.
 *        This field corresponds to the type argument to IOServiceOpen().
 * @field user_client_class Meta class name of the user client instance.
 *
 * This event is fired when a process calls IOServiceOpen() in order to open
 * a communications channel with an I/O Kit driver.  The event does not
 * correspond to driver <-> device communication and is neither providing
 * visibility nor access control into devices being attached.
 */
typedef struct {
	uint32_t user_client_type;
	es_string_token_t user_client_class;
	uint8_t reserved[64];
} es_event_iokit_open_t;

/**
 * @brief Get a process's task control port
 *
 * @field target The process for which the task control port will be retrieved.
 *
 * This event is fired when a process obtains a send right to a task control
 * port (e.g. task_for_pid(), task_identity_token_get_task_port(),
 * processor_set_tasks() and other means).
 *
 * @note Task control ports were formerly known as simply "task ports".
 */
typedef struct {
	es_process_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_get_task_t;

/**
 * @brief Get a process's task read port
 *
 * @field target The process for which the task read port will be retrieved.
 *
 * This event is fired when a process obtains a send right to a task read
 * port (e.g. task_read_for_pid(), task_identity_token_get_task_port()).
 */
typedef struct {
	es_process_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_get_task_read_t;

/**
 * @brief Get a process's task inspect port
 *
 * @field target The process for which the task inspect port will be retrieved.
 *
 * This event is fired when a process obtains a send right to a task inspect
 * port (e.g. task_inspect_for_pid(), task_identity_token_get_task_port()).
 */
typedef struct {
	es_process_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_get_task_inspect_t;

/**
 * @brief Get a process's task name port
 *
 * @field target The process for which the task name port will be retrieved.
 *
 * This event is fired when a process obtains a send right to a task name
 * port (e.g. task_name_for_pid(), task_identity_token_get_task_port()).
 */
typedef struct {
	es_process_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_get_task_name_t;

/**
 * @brief Retrieve file system attributes
 *
 * @field attrlist The attributes that will be retrieved
 * @field target The file for which attributes will be retrieved
 */
typedef struct {
	struct attrlist attrlist;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_getattrlist_t;

/**
 * @brief Set file system attributes
 *
 * @field attrlist The attributes that will be modified
 * @field target The file for which attributes will be modified
 */
typedef struct {
	struct attrlist attrlist;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_setattrlist_t;

/**
 * @brief Update file contents via the FileProvider framework
 *
 * @field source The staged file that has had its contents updated
 * @field target_path The destination that the staged `source` file will be moved to
 */
typedef struct {
	es_file_t * _Nonnull source;
	es_string_token_t target_path;
	uint8_t reserved[64];
} es_event_file_provider_update_t;

/**
 * @brief Materialize a file via the FileProvider framework
 *
 * @field source The staged file that has been materialized
 * @field target The destination of the staged `source` file
 */
typedef struct {
	es_process_t * _Nonnull instigator;
	es_file_t * _Nonnull source;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_file_provider_materialize_t;

/**
 * @brief Resolve a symbolic link
 *
 * @field source The symbolic link that is attempting to be resolved
 *
 * @note This is not limited only to readlink(2). Other operations such as path lookups
 * can also cause this event to be fired.
 */
typedef struct {
	es_file_t * _Nonnull source;
	uint8_t reserved[64];
} es_event_readlink_t;

/**
 * @brief Lookup a file system object
 *
 * @field source_dir The current directory
 * @field relative_target The path to lookup relative to the `source_dir`
 *
 * @note The `relative_target` data may contain untrusted user input.
 */
typedef struct {
	es_file_t * _Nonnull source_dir;
	es_string_token_t relative_target;
	uint8_t reserved[64];
} es_event_lookup_t;

/**
 * @brief Test file access
 *
 * @field mode Access permission to check
 * @field target The file to check for access
 */
typedef struct {
	int32_t mode;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_access_t;

/**
 * @brief Change file access and modification times (e.g. via utimes(2))
 *
 * @field target The path which will have its times modified
 * @field atime The desired new access time
 * @field mtime The desired new modification time
 */
typedef struct {
	es_file_t * _Nonnull target;
	struct timespec atime;
	struct timespec mtime;
	uint8_t reserved[64];
} es_event_utimes_t;

/**
 * @brief Clone a file
 *
 * @field source The file that will be cloned
 * @field target_dir The directory into which the `source` file will be cloned
 * @field target_name The name of the new file to which `source` will be cloned
 */
typedef struct {
	es_file_t * _Nonnull source;
	es_file_t * _Nonnull target_dir;
	es_string_token_t target_name;
	uint8_t reserved[64];
} es_event_clone_t;

/**
 * @brief Copy a file using the copyfile syscall
 *
 * @field source The file that will be cloned
 * @field target_file The file existing at the target path that will be overwritten
 *                    by the copyfile operation.  NULL if no such file exists.
 * @field target_dir The directory into which the `source` file will be copied
 * @field target_name The name of the new file to which `source` will be copied
 * @field mode Corresponds to mode argument of the copyfile syscall
 * @field flags Corresponds to flags argument of the copyfile syscall
 *
 * @note Not to be confused with copyfile(3).
 * @note Prior to macOS 12.0, the copyfile syscall fired open, unlink and auth
 *       create events, but no notify create, nor write or close events.
 */
typedef struct {
	es_file_t * _Nonnull source;
	es_file_t * _Nullable target_file;
	es_file_t * _Nonnull target_dir;
	es_string_token_t target_name;
	mode_t mode;
	int32_t flags;
	uint8_t reserved[56];
} es_event_copyfile_t;

/**
 * @brief File control
 *
 * @field target The target file on which the file control command will be performed
 * @field cmd The `cmd` argument given to fcntl(2)
 */
typedef struct {
	es_file_t * _Nonnull target;
	int32_t cmd;
	uint8_t reserved[64];
} es_event_fcntl_t;

/**
 * @brief Read directory entries
 *
 * @field target The directory whose contents will be read
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_readdir_t;

/**
 * @brief Retrieve file system path based on FSID
 *
 * @field target Describes the file system path that will be retrieved
 *
 * @note This event can fire multiple times for a single syscall, for example when the syscall
 *       has to be retried due to racing VFS operations.
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_fsgetpath_t;

/**
 * @brief Modify the system time
 *
 * @note This event is not fired if the program contains the entitlement
 * com.apple.private.settime. Additionally, even if an ES client responds to
 * ES_EVENT_TYPE_AUTH_SETTIME events with ES_AUTH_RESULT_ALLOW, the operation
 * may still fail for other reasons (e.g. unprivileged user).
 */
typedef struct {
	uint8_t reserved[64];
} es_event_settime_t;

/**
 * @brief Duplicate a file descriptor
 *
 * @field target Describes the file the duplicated file descriptor points to
 */
typedef struct {
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_dup_t;

/**
 * @brief Fired when a UNIX-domain socket is about to be bound to a path.
 *
 * @field dir Describes the directory the socket file is created in.
 * @field filename The filename of the socket file.
 * @field mode The mode of the socket file.
 */
typedef struct {
	es_file_t * _Nonnull dir;
	es_string_token_t filename;
	mode_t mode;
	uint8_t reserved[64];
} es_event_uipc_bind_t;

/**
 * @brief Fired when a UNIX-domain socket is about to be connected.
 *
 * @field file Describes the socket file that the socket is bound to.
 * @field domain The cmmunications domain of the socket (see socket(2)).
 * @field type The type of the socket (see socket(2)).
 * @field protocol The protocol of the socket (see socket(2)).
 */
typedef struct {
	es_file_t * _Nonnull file;
	int domain;
	int type;
	int protocol;
	uint8_t reserved[64];
} es_event_uipc_connect_t;

/**
 * @brief Set a file ACL.
 *
 * @field set_or_clear Describes whether or not the ACL on the `target` is being set or cleared
 * @field acl Union that is valid when `set_or_clear` is set to `ES_SET`
 * @field set The acl_t structure to be used by vairous acl(3) functions
 *        @note The acl provided cannot be directly used by functions within
 *        the <sys/acl.h> header. These functions can mutate the struct passed
 *        into them, which is not compatible with the immutable nature of
 *        es_message_t. Additionally, because this field is minimally constructed,
 *        you must not use `acl_dup(3)` to get a mutable copy, as this can lead to
 *        out of bounds memory access. To obtain a acl_t struct that is able to be
 *        used with all functions within <sys/acl.h>, please use a combination of
 *        `acl_copy_ext(3)` followed by `acl_copy_int(3)`.
 * @field target Describes the file whose ACL is being set.
 */
typedef struct {
	es_file_t * _Nonnull target;
	es_set_or_clear_t set_or_clear;
	union {
		acl_t _Nonnull set;
	} acl;
	uint8_t reserved[64];
} es_event_setacl_t;

/**
 * @brief Fired when a pseudoterminal control device is granted
 *
 * @field dev Major and minor numbers of device
 */
typedef struct {
	dev_t dev;
	uint8_t reserved[64];
} es_event_pty_grant_t;

/**
 * @brief Fired when a pseudoterminal control device is closed
 *
 * @field dev Major and minor numbers of device
 */
typedef struct {
	dev_t dev;
	uint8_t reserved[64];
} es_event_pty_close_t;

/**
 * @brief This enum describes the type of the es_event_proc_check_t event that are currently used
 *
 * @note ES_PROC_CHECK_TYPE_KERNMSGBUF, ES_PROC_CHECK_TYPE_TERMINATE and
 * ES_PROC_CHECK_TYPE_UDATA_INFO are deprecated and no proc_check messages will be generated
 * for the corresponding proc_info call numbers.
 * The terminate callnum is covered by the signal event.
 */
typedef enum {
	ES_PROC_CHECK_TYPE_LISTPIDS = 0x1,
	ES_PROC_CHECK_TYPE_PIDINFO = 0x2,
	ES_PROC_CHECK_TYPE_PIDFDINFO = 0x3,
	ES_PROC_CHECK_TYPE_KERNMSGBUF = 0x4,        // deprecated, not generated
	ES_PROC_CHECK_TYPE_SETCONTROL = 0x5,
	ES_PROC_CHECK_TYPE_PIDFILEPORTINFO = 0x6,
	ES_PROC_CHECK_TYPE_TERMINATE = 0x7,         // deprecated, not generated
	ES_PROC_CHECK_TYPE_DIRTYCONTROL = 0x8,
	ES_PROC_CHECK_TYPE_PIDRUSAGE = 0x9,
	ES_PROC_CHECK_TYPE_UDATA_INFO = 0xe,        // deprecated, not generated
} es_proc_check_type_t;

/**
 * @brief Access control check for retrieving process information.
 *
 * @field target The process for which the access will be checked
 * @field type The type of call number used to check the access on the target process
 * @field flavor The flavor used to check the access on the target process
 */
typedef struct {
	es_process_t * _Nullable target;
	es_proc_check_type_t type;
	int flavor;
	uint8_t reserved[64];
} es_event_proc_check_t;

/**
 * @brief Access control check for searching a volume or a mounted file system
 *
 * @field attrlist The attributes that will be used to do the search
 * @field target The volume whose contents will be searched
 */
typedef struct {
	struct attrlist attrlist;
	es_file_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_searchfs_t;

/**
 * @brief This enum describes the type of suspend/resume operations that are currently used.
 */
typedef enum {
	ES_PROC_SUSPEND_RESUME_TYPE_SUSPEND = 0,
	ES_PROC_SUSPEND_RESUME_TYPE_RESUME = 1,
	ES_PROC_SUSPEND_RESUME_TYPE_SHUTDOWN_SOCKETS = 3,
} es_proc_suspend_resume_type_t;

/**
 * @brief Fired when one of pid_suspend, pid_resume or pid_shutdown_sockets
 * is called on a process.
 *
 * @field target The process that is being suspended, resumed, or is the object
 * of a pid_shutdown_sockets call.
 * @field type The type of operation that was called on the target process.
 */
typedef struct {
	es_process_t * _Nullable target;
	es_proc_suspend_resume_type_t type;
	uint8_t reserved[64];
} es_event_proc_suspend_resume_t;

/**
 * @brief Code signing status for process was invalidated.
 *
 * @note This event fires when the CS_VALID bit is removed from a
 * process' CS flags, that is, when the first invalid page is paged in
 * for a process with an otherwise valid code signature, or when a
 * process is explicitly invalidated by a csops(CS_OPS_MARKINVALID)
 * syscall.  This event does not fire if CS_HARD was set, since CS_HARD
 * by design prevents the process from going invalid.
 */
typedef struct {
	uint8_t reserved[64];
} es_event_cs_invalidated_t;

/**
 * @brief Fired when one process attempts to attach to another process
 *
 * @field target The process that will be attached to by the process
 * that instigated the event
 *
 * @note This event can fire multiple times for a single trace attempt, for example
 * when the processes to which is being attached is reparented during the operation
 */
typedef struct {
	es_process_t * _Nonnull target;
	uint8_t reserved[64];
} es_event_trace_t;

/**
 * @brief Notification that a process has attempted to create a thread in
 * another process by calling one of the thread_create or thread_create_running
 * MIG routines.
 *
 * @field target The process in which a new thread was created
 * @field thread_state The new thread state in case of thread_create_running,
 * NULL in case of thread_create.
 */
typedef struct {
	es_process_t * _Nonnull target;
	es_thread_state_t * _Nullable thread_state;
	uint8_t reserved[64];
} es_event_remote_thread_create_t;

/**
 * @brief Notification that a process has called setuid().
 *
 * @field uid The uid argument to the setuid() syscall.
 */
typedef struct {
	uid_t uid;
	uint8_t reserved[64];
} es_event_setuid_t;

/**
 * @brief Notification that a process has called setgid().
 *
 * @field gid The gid argument to the setgid() syscall.
 */
typedef struct {
	uid_t gid;
	uint8_t reserved[64];
} es_event_setgid_t;

/**
 * @brief Notification that a process has called seteuid().
 *
 * @field euid The euid argument to the seteuid() syscall.
 */
typedef struct {
	uid_t euid;
	uint8_t reserved[64];
} es_event_seteuid_t;

/**
 * @brief Notification that a process has called setegid().
 *
 * @field egid The egid argument to the setegid() syscall.
 */
typedef struct {
	uid_t egid;
	uint8_t reserved[64];
} es_event_setegid_t;

/**
 * @brief Notification that a process has called setreuid().
 *
 * @field ruid The ruid argument to the setreuid() syscall.
 * @field euid The euid argument to the setreuid() syscall.
 */
typedef struct {
	uid_t ruid;
	uid_t euid;
	uint8_t reserved[64];
} es_event_setreuid_t;

/**
 * @brief Notification that a process has called setregid().
 *
 * @field rgid The rgid argument to the setregid() syscall.
 * @field egid The egid argument to the setregid() syscall.
 */
typedef struct {
	uid_t rgid;
	uid_t egid;
	uint8_t reserved[64];
} es_event_setregid_t;

/**
 * Union of all possible events that can appear in an es_message_t
 */
typedef union {
	es_event_access_t access;
	es_event_chdir_t chdir;
	es_event_chroot_t chroot;
	es_event_clone_t clone;
	es_event_close_t close;
	es_event_copyfile_t copyfile;
	es_event_create_t create;
	es_event_cs_invalidated_t cs_invalidated;
	es_event_deleteextattr_t deleteextattr;
	es_event_dup_t dup;
	es_event_exchangedata_t exchangedata;
	es_event_exec_t exec;
	es_event_exit_t exit;
	es_event_file_provider_materialize_t file_provider_materialize;
	es_event_file_provider_update_t file_provider_update;
	es_event_fcntl_t fcntl;
	es_event_fork_t fork;
	es_event_fsgetpath_t fsgetpath;
	es_event_get_task_t get_task;
	es_event_get_task_read_t get_task_read;
	es_event_get_task_inspect_t get_task_inspect;
	es_event_get_task_name_t get_task_name;
	es_event_getattrlist_t getattrlist;
	es_event_getextattr_t getextattr;
	es_event_iokit_open_t iokit_open;
	es_event_kextload_t kextload;
	es_event_kextunload_t kextunload;
	es_event_link_t link;
	es_event_listextattr_t listextattr;
	es_event_lookup_t lookup;
	es_event_mmap_t mmap;
	es_event_mount_t mount;
	es_event_mprotect_t mprotect;
	es_event_open_t open;
	es_event_proc_check_t proc_check;
	es_event_proc_suspend_resume_t proc_suspend_resume;
	es_event_pty_close_t pty_close;
	es_event_pty_grant_t pty_grant;
	es_event_readdir_t readdir;
	es_event_readlink_t readlink;
	es_event_remote_thread_create_t remote_thread_create;
	es_event_remount_t remount;
	es_event_rename_t rename;
	es_event_searchfs_t searchfs;
	es_event_setacl_t setacl;
	es_event_setattrlist_t setattrlist;
	es_event_setextattr_t setextattr;
	es_event_setflags_t setflags;
	es_event_setmode_t setmode;
	es_event_setowner_t setowner;
	es_event_settime_t settime;
	es_event_setuid_t setuid;
	es_event_setgid_t setgid;
	es_event_seteuid_t seteuid;
	es_event_setegid_t setegid;
	es_event_setreuid_t setreuid;
	es_event_setregid_t setregid;
	es_event_signal_t signal;
	es_event_stat_t stat;
	es_event_trace_t trace;
	es_event_truncate_t truncate;
	es_event_uipc_bind_t uipc_bind;
	es_event_uipc_connect_t uipc_connect;
	es_event_unlink_t unlink;
	es_event_unmount_t unmount;
	es_event_utimes_t utimes;
	es_event_write_t write;
} es_events_t;

/**
 * es_result_t indicates the result of the ES subsystem authorization process
 * The result_type field indicates if the result is an es_auth_result_t or a uint32_t (flags)
 */
typedef struct {
	es_result_type_t result_type;
	union {
		es_auth_result_t auth;
		uint32_t flags;
		uint8_t reserved[32];
	} result;
} es_result_t;

/**
 * @brief es_message_t is the top level datatype that encodes information sent
 * from the ES subsystem to its clients.  Each security event being processed
 * by the ES subsystem will be encoded in an es_message_t.  A message can be an
 * authorization request or a notification of an event that has already taken
 * place.
 *
 * @field version Indicates the message version; some fields are not available
 *        and must not be accessed unless the message version is equal to or
 *        higher than the message version at which the field was introduced.
 * @field time The time at which the event was generated.
 * @field mach_time The Mach absolute time at which the event was generated.
 * @field deadline The Mach absolute time before which an auth event must
 *        be responded to. If a client fails to respond to auth events prior to the `deadline`,
 *        the client will be killed.
 *        Each message can contain its own unique deadline, and some deadlines
 *        can vary substantially. Clients must take care to inspect the deadline
 *        value of each message to know how much time is allotted for processing.
 * @field process Describes the process that took the action.
 * @field seq_num Per-client, per-event-type sequence number that can be
 *        inspected to detect whether the kernel had to drop events for this
 *        client.  When no events are dropped for this client, seq_num
 *        increments by 1 for every message of that event type.  When events
 *        have been dropped, the difference between the last seen sequence
 *        number of that event type plus 1 and seq_num of the received message
 *        indicates the number of events that had to be dropped.
 *        Dropped events generally indicate that more events were generated in
 *        the kernel than the client was able to handle.
 *        Field available only if message version >= 2.
 *        @see global_seq_num
 * @field action_type Indicates if the action field is an auth or notify action.
 * @field action For auth events, contains the opaque auth ID that must be
 *        supplied when responding to the event.  For notify events, describes
 *        the result of the action.
 * @field event_type Indicates which event struct is defined in the event union.
 * @field event Contains data specific to the event type.
 * @field thread Describes the thread that took the action.  May be NULL when
 *        thread is not applicable, for example for trace events that describe
 *        the traced process calling ptrace(PT_TRACE_ME) or for cs invalidated
 *        events that are a result of another process calling
 *        csops(CS_OPS_MARKINVALID).
 *        Field available only if message version >= 4.
 * @field global_seq_num Per-client sequence number that can be inspected to
 *        detect whether the kernel had to drop events for this client. When no
 *        events are dropped for this client, global_seq_num increments by 1 for
 *        every message. When events have been dropped, the difference between
 *        the last seen global sequence number and the global_seq_num of the
 *        received message indicates the number of events that had to be dropped.
 *        Dropped events generally indicate that more events were generated in
 *        the kernel than the client was able to handle.
 *        Field available only if message version >= 4.
 *        @see seq_num
 * @field opaque Opaque data that must not be accessed directly.
 *
 * @note For events that can be authorized there are unique NOTIFY and AUTH
 * event types for the same event data, eg: event.exec is the correct union
 * label for both ES_EVENT_TYPE_AUTH_EXEC and ES_EVENT_TYPE_NOTIFY_EXEC event
 * types.
 *
 * @note For fields marked only available in specific message versions, all
 * access must be guarded at runtime by checking the value of the message
 * version field, e.g.
 * ```
 * if (msg->version >= 2) {
 *     acl = msg->event.create.acl;
 * }
 * ```
 *
 * @note Fields using Mach time are in the resolution matching the ES client's
 * architecture.  This means they can be compared to mach_absolute_time() and
 * converted to nanoseconds with the help of mach_timebase_info().  Further
 * note that on Apple silicon, x86_64 clients running under Rosetta 2 will see
 * Mach times in a different resolution than native arm64 clients.  For more
 * information on differences regarding Mach time on Apple silicon and Intel-based
 * Mac computers, see "Addressing Architectural Differences in Your macOS Code":
 * https://developer.apple.com/documentation/apple_silicon/addressing_architectural_differences_in_your_macos_code
 */
typedef struct {
	uint32_t version;
	struct timespec time;
	uint64_t mach_time;
	uint64_t deadline;
	es_process_t * _Nonnull process;
	uint64_t seq_num; /* field available only if message version >= 2 */
	es_action_type_t action_type;
	union {
		es_event_id_t auth;
		es_result_t notify;
	} action;
	es_event_type_t event_type;
	es_events_t event;
	es_thread_t * _Nullable thread; /* field available only if message version >= 4 */
	uint64_t global_seq_num; /* field available only if message version >= 4 */
	uint64_t opaque[]; /* Opaque data that must not be accessed directly */
} es_message_t;

__BEGIN_DECLS

/**
 * Calculate the size of an es_message_t.
 *
 * @warning This function MUST NOT be used in conjunction with attempting to copy an es_message_t (e.g.
 * by using the reported size in order to `malloc(3)` a buffer, and `memcpy(3)` an existing es_message_t
 * into that buffer). Doing so will result in use-after-free bugs.
 *
 * @deprecated Please use `es_retain_message` to retain an es_message_t.
 *
 * @param msg The message for which the size will be calculated
 * @return Size of the message
 */
OS_EXPORT
API_DEPRECATED("Please use es_retain_message to retain a message. Do not use this in conjunction with attempting to copy a message, doing so will result in use-after-free bugs.", macos(10.15, 11.0))
API_UNAVAILABLE(ios, tvos, watchos)
size_t
es_message_size(const es_message_t * _Nonnull msg);

/**
 * Retains an es_message_t, returning a non-const pointer to the given es_message_t for compatibility with
 * existing code.
 *
 * @warning It is invalid to attempt to write to the returned es_message_t, despite being non-const, and
 * doing so will result in a crash.
 *
 * @deprecated Use es_retain_message to retain a message.
 *
 * @param msg The message to be retained
 * @return non-const pointer to the retained es_message_t.
 *
 * @brief The caller must release the memory with `es_free_message`
 */
OS_EXPORT
API_DEPRECATED("Use es_retain_message to retain a message.", macos(10.15, 11.0))
API_UNAVAILABLE(ios, tvos, watchos)
es_message_t * _Nullable
es_copy_message(const es_message_t * _Nonnull msg);

/**
 * Releases the memory associated with the given es_message_t that was retained via `es_copy_message`
 *
 * @deprecated Use `es_release_message` to release a message.
 *
 * @param msg The message to be released
 */
OS_EXPORT
API_DEPRECATED("Use es_release_message to release a message.", macos(10.15, 11.0))
API_UNAVAILABLE(ios, tvos, watchos)
void
es_free_message(es_message_t * _Nonnull msg);

/**
 * Retains the given es_message_t, extending its lifetime until released with `es_release_message`.
 *
 * @param msg The message to be retained
 *
 * @note It is necessary to retain a message when the es_message_t provided in the event handler block of
 * `es_new_client` will be processed asynchronously.
 */
OS_EXPORT
API_AVAILABLE(macos(11.0)) API_UNAVAILABLE(ios, tvos, watchos)
void
es_retain_message(const es_message_t * _Nonnull msg);

/**
 * Releases the given es_message_t that was previously retained with `es_retain_message`
 *
 * @param msg The message to be released
 */
OS_EXPORT
API_AVAILABLE(macos(11.0)) API_UNAVAILABLE(ios, tvos, watchos)
void
es_release_message(const es_message_t * _Nonnull msg);

/**
 * Get the number of arguments in a message containing an es_event_exec_t
 * @param event The es_event_exec_t being inspected
 * @return The number of arguments
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
uint32_t
es_exec_arg_count(const es_event_exec_t * _Nonnull event);

/**
 * Get the number of environment variables in a message containing an es_event_exec_t
 * @param event The es_event_exec_t being inspected
 * @return The number of environment variables
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
uint32_t
es_exec_env_count(const es_event_exec_t * _Nonnull event);

/**
 * Get the number of file descriptors in a message containing an es_event_exec_t
 * @param event The es_event_exec_t being inspected
 * @return The number of file descriptors
 */
OS_EXPORT
API_AVAILABLE(macos(11.0)) API_UNAVAILABLE(ios, tvos, watchos)
uint32_t
es_exec_fd_count(const es_event_exec_t * _Nonnull event);

/**
 * Get the argument at the specified position in the message containing an es_event_exec_t
 * @param event The es_event_exec_t being inspected
 * @param index Index of the argument to retrieve (starts from 0)
 * @return  es_string_token_t containing a pointer to the argument and its length.
 *          This is a zero-allocation operation. The returned pointer must not outlive exec_event.
 * @brief Reading an an argument where `index` >= `es_exec_arg_count()` is undefined
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_string_token_t
es_exec_arg(const es_event_exec_t * _Nonnull event, uint32_t index);

/**
 * Get the environment variable at the specified position in the message containing an es_event_exec_t
 * @param event The es_event_exec_t being inspected
 * @param index Index of the environment variable to retrieve (starts from 0)
 * @return  es_string_token_t containing a pointer to the environment variable and its length.
 *          This is zero-allocation operation. The returned pointer must not outlive exec_event.
 * @brief Reading an an env where `index` >= `es_exec_env_count()` is undefined
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_string_token_t
es_exec_env(const es_event_exec_t * _Nonnull event, uint32_t index);

/**
 * Get the file descriptor at the specified position in the message containing an es_event_exec_t
 * @param event The es_event_exec_t being inspected
 * @param index Index of the file descriptor to retrieve (starts from 0)
 * @return Pointer to es_fd_t describing the file descriptor.
 *         This is zero-allocation operation. The returned pointer must not outlive exec_event.
 * @note Reading an fd where `index` >= `es_exec_fd_count()` is undefined
 */
OS_EXPORT
API_AVAILABLE(macos(11.0)) API_UNAVAILABLE(ios, tvos, watchos)
const es_fd_t * _Nonnull
es_exec_fd(const es_event_exec_t * _Nonnull event, uint32_t index);

/**
 * This typedef is no longer used, but exists for API backwards compatibility.
 */
typedef struct statfs es_statfs_t;

__END_DECLS

#endif /* __ENDPOINT_SECURITY_MESSAGE_H */
