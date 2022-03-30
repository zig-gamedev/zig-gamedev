// This API and many of the comments in this file come from the ENet library, which bears the following license:
//
// Copyright (c) 2002-2020 Lee Salzman
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

const root = @import("root");

pub const pl = if (builtin.os.tag == .windows) WindowsPlatform else UnixPlatform;

// win32.h
const WindowsPlatform = struct {
    const ws2 = std.os.windows.ws2_32;

    pub const SocketHandle = ws2.SOCKET;

    pub const SOCKET_NULL = ws2.INVALID_SOCKET;

    pub const Buffer = extern struct {
        dataLength: usize,
        data: [*]u8,
    };

    const FD_SETSIZE = 64;
    /// This structure is binary compatible with fd_set
    pub const SocketSet = extern struct {
        fd_count: c_uint = 0,
        fd_array: [FD_SETSIZE]SocketHandle align(8) = undefined,

        /// AKA FD_ZERO
        pub fn empty(self: *SocketSet) void {
            self.fd_count = 0;
        }

        /// AKA FD_SET
        /// Mimics the winsock2 version, which checks for duplicates.
        pub fn add(self: *SocketSet, socket: SocketHandle) void {
            for (self.fd_array[0..self.fd_count]) |handle| {
                if (handle == socket) break;
            } else if (self.fd_count < FD_SETSIZE) {
                self.fd_array[self.fd_count] = socket;
                self.fd_count += 1;
            }
        }

        /// AKA FD_CLR
        pub fn remove(self: *SocketSet, socket: SocketHandle) void {
            var i: c_uint = 0;
            while (i < self.fd_count) : (i += 1) {
                if (self.fd_array[i] == socket) {
                    self.fd_count -= 1;
                    while (i < self.fd_count) {
                        self.fd_array[i] = self.fd_array[i + 1];
                    }
                    break;
                }
            }
        }

        /// AKA FD_ISSET
        pub fn check(self: *SocketSet, socket: SocketHandle) c_int {
            return __WSAFDIsSet(socket, self);
        }

        extern "ws2_32" fn __WSAFDIsSet(socket: SocketHandle, set: *SocketSet) c_int;
    };
};

// unix.h
const UnixPlatform = struct {
    pub const SocketHandle = c_int;
    pub const SOCKET_NULL: SocketHandle = -1;

    pub const Buffer = extern struct {
        data: [*]u8,
        dataLength: usize,
    };

    const SocketSet = extern struct {
        const max_fd = 1024;
        const Mask = c_long;
        const MaskShift = std.math.Log2Int(Mask);
        const mask_bits = 8 * @sizeOf(Mask);
        const num_masks = @divExact(max_fd, mask_bits);

        fds_bits: [num_masks]Mask = [_]Mask{0} ** num_masks,

        /// FD_ZERO
        pub fn empty(self: *SocketSet) void {
            self.* = .{};
        }

        /// FD_SET
        pub fn add(self: *SocketSet, socket: SocketHandle) void {
            assert(socket >= 0);
            assert(socket < max_fd);
            self.fds_bits[index(socket)] |= mask(socket);
        }

        /// FD_CLR
        pub fn remove(self: *SocketSet, socket: SocketHandle) void {
            assert(socket >= 0);
            assert(socket < max_fd);
            self.fds_bits[index(socket)] &= ~mask(socket);
        }

        /// FD_ISSET
        pub fn check(self: *SocketSet, socket: SocketHandle) bool {
            return (self.fds_bits[index(socket)] & mask(socket)) != 0;
        }

        fn index(fd: SocketHandle) usize {
            return @intCast(usize, @divFloor(fd, mask_bits));
        }
        fn mask(fd: SocketHandle) Mask {
            return @shlExact(@as(Mask, 1), @intCast(MaskShift, @mod(fd, mask_bits)));
        }
    };
};

pub inline fn HOST_TO_NET(a: anytype) @TypeOf(a) {
    if (builtin.endian == .little) {
        return @byteSwap(@TypeOf(a), a);
    } else {
        return a;
    }
}

pub inline fn NET_TO_HOST(a: anytype) @TypeOf(a) {
    if (builtin.endian == .little) {
        return @byteSwap(@TypeOf(a), a);
    } else {
        return a;
    }
}

pub const time = struct {
    pub const OVERFLOW = 86400000;

    pub inline fn less(a: u32, b: u32) bool {
        return (a -% b) >= OVERFLOW;
    }
    pub inline fn greater(a: u32, b: u32) bool {
        return (b -% a) >= OVERFLOW;
    }
    pub inline fn less_equal(a: u32, b: u32) bool {
        return !greater(a, b);
    }
    pub inline fn greater_equal(a: u32, b: u32) bool {
        return !less(a, b);
    }
};

// enet/protocol.h
pub const Protocol = packed union {
    pub const MINIMUM_MTU = 576;
    pub const MAXIMUM_MTU = 4096;
    pub const MAXIMUM_PACKET_COMMANDS = 32;
    pub const MINIMUM_WINDOW_SIZE = 4096;
    pub const MAXIMUM_WINDOW_SIZE = 65536;
    pub const MINIMUM_CHANNEL_COUNT = 1;
    pub const MAXIMUM_CHANNEL_COUNT = 255;
    pub const MAXIMUM_PEER_ID = 0xFFF;
    pub const MAXIMUM_FRAGMENT_COUNT = 1024 * 1024;

    header: CommandHeader,
    acknowledge: Acknowledge,
    connect: Connect,
    verifyConnect: VerifyConnect,
    disconnect: Disconnect,
    ping: Ping,
    sendReliable: SendReliable,
    sendUnreliable: SendUnreliable,
    sendUnsequenced: SendUnsequenced,
    sendFragment: SendFragment,
    bandwidthLimit: BandwidthLimit,
    throttleConfigure: ThrottleConfigure,

    pub const Command = enum(u8) {
        none = 0,
        acknowledge = 1,
        connect = 2,
        verify_connect = 3,
        disconnect = 4,
        ping = 5,
        send_reliable = 6,
        send_unreliable = 7,
        send_fragment = 8,
        send_unsequenced = 9,
        bandwidth_limit = 10,
        throttle_configure = 11,
        send_unreliable_fragment = 12,

        pub const count = 13;
        pub const mask = 0xF;
    };

    pub const Flags = packed struct {
        __pad0: u6 = 0,
        command_unsequenced: bool = false,
        command_acknowledge: bool = false,
        __pad1: u4 = 0,
        header_session: u2 = 0,
        header_compressed: bool = false,
        header_sent_time: bool = false,
        __pad2: u16 = 0,

        pub const header_mask = Flags{ .header_compressed = true, .header_sent_time = true };
    };

    pub const Header = packed struct {
        peerID: u16,
        sentTime: u16,
    };

    pub const CommandHeader = packed struct {
        command: Command,
        channelID: u8,
        reliableSequenceNumber: u16,
    };

    pub const Acknowledge = packed struct {
        header: CommandHeader,
        receivedReliableSequenceNumber: u16,
        receivedSentTime: u16,
    };

    pub const Connect = packed struct {
        header: CommandHeader,
        outgoingPeerID: u16,
        incomingSessionID: u8,
        outgoingSessionID: u8,
        mtu: u32,
        windowSize: u32,
        channelCount: u32,
        incomingBandwidth: u32,
        outgoingBandwidth: u32,
        packetThrottleInterval: u32,
        packetThrottleAcceleration: u32,
        packetThrottleDeceleration: u32,
        connectID: u32,
        data: u32,
    };

    pub const VerifyConnect = packed struct {
        header: CommandHeader,
        outgoingPeerID: u16,
        incomingSessionID: u8,
        outgoingSessionID: u8,
        mtu: u32,
        windowSize: u32,
        channelCount: u32,
        incomingBandwidth: u32,
        outgoingBandwidth: u32,
        packetThrottleInterval: u32,
        packetThrottleAcceleration: u32,
        packetThrottleDeceleration: u32,
        connectID: u32,
    };

    pub const BandwidthLimit = packed struct {
        header: CommandHeader,
        incomingBandwidth: u32,
        outgoingBandwidth: u32,
    };

    pub const ThrottleConfigure = packed struct {
        header: CommandHeader,
        packetThrottleInterval: u32,
        packetThrottleAcceleration: u32,
        packetThrottleDeceleration: u32,
    };

    pub const Disconnect = packed struct {
        header: CommandHeader,
        data: u32,
    };

    pub const Ping = packed struct {
        header: CommandHeader,
    };

    pub const SendReliable = packed struct {
        header: CommandHeader,
        dataLength: u16,
    };

    pub const SendUnreliable = packed struct {
        header: CommandHeader,
        unreliableSequenceNumber: u16,
        dataLength: u16,
    };

    pub const SendUnsequenced = packed struct {
        header: CommandHeader,
        unsequencedGroup: u16,
        dataLength: u16,
    };

    pub const SendFragment = packed struct {
        header: CommandHeader,
        startSequenceNumber: u16,
        dataLength: u16,
        fragmentCount: u32,
        fragmentNumber: u32,
        totalLength: u32,
        fragmentOffset: u32,
    };
};

// enet/list.h

pub const ListNode = extern struct {
    next: *ListNode,
    previous: *ListNode,

    /// Insert a new node before this node
    /// Returns the new node.
    pub fn insert_previous(self: *ListNode, new_node: *ListNode) *ListNode {
        new_node.previous = self.previous;
        new_node.next = self;

        new_node.previous.next = new_node;
        self.previous = new_node;

        return new_node;
    }

    /// Remove this node from the list.
    /// Returns this node
    pub fn remove(self: *ListNode) *ListNode {
        self.previous.next = self.next;
        self.next.previous = self.previous;
        return self;
    }

    /// Insert a range of nodes into this list, before this node.
    /// Returns the first node in the moved list
    pub fn move_previous(self: *ListNode, first: *ListNode, last: *ListNode) *ListNode {
        first.previous.next = last.next;
        last.next.previous = first.previous;

        first.previous = self.previous;
        last.next = self;

        first.previous.next = first;
        self.previous = last;

        return first;
    }
};

pub fn List(comptime T: type) type {
    return extern struct {
        sentinel: ListNode,

        /// Empties the list
        pub inline fn clear(self: *@This()) void {
            self.sentinel.next = &self.sentinel;
            self.sentinel.previous = &self.sentinel;
        }
        /// Checks if the list is empty
        pub inline fn empty(self: *@This()) bool {
            return self.sentinel.next == &self.sentinel;
        }
        /// Computes the size of the list
        pub fn size(self: *@This()) usize {
            var count: usize = 0;
            var curr = self.begin();
            const sentinel = self.end();
            while (curr != sentinel) {
                curr = curr.next;
                count += 1;
            }
            return count;
        }

        /// Returns the first item in the list.
        pub inline fn front(self: *@This()) *T {
            assert(!self.empty());
            return @intToPtr(*T, @ptrToInt(self.sentinel.next));
        }
        /// Returns the last item in the list.
        pub inline fn back(self: *@This()) *T {
            assert(!self.empty());
            return @intToPtr(*T, @ptrToInt(self.sentinel.previous));
        }

        /// Returns a pointer to the first node in the list
        pub inline fn begin(self: *@This()) *ListNode {
            return self.sentinel.next;
        }
        /// Returns a pointer to one past the last node in the list
        /// or one before the first node in the list
        pub inline fn end(self: *@This()) *ListNode {
            return &self.sentinel;
        }
        /// Returns an iterator
        pub fn iterator(self: *@This()) Iterator {
            return .{ .next = self.begin(), .end = self.end() };
        }

        const Iterator = struct {
            next: *ListNode,
            end: *ListNode,

            pub fn next(self: *Iterator) ?*T {
                if (self.next == self.end) return null;
                const curr = self.next;
                self.next = curr.next;
                return @intToPtr(*T, @ptrToInt(curr));
            }
        };
    };
}

// enet/callbacks.h

pub const Callbacks = extern struct {
    malloc: ?fn (size: usize) callconv(.C) *anyopaque = null,
    free: ?fn (memory: *anyopaque) callconv(.C) void = null,
    no_memory: ?fn () callconv(.C) void = null,
};

// enet/enet.h

pub const VERSION_MAJOR = 1;
pub const VERSION_MINOR = 3;
pub const VERSION_PATCH = 16;

pub fn VERSION_CREATE(major: u8, minor: u8, patch: u8) Version {
    return @shlExact(@as(Version, major), 16) |
        @shlExact(@as(Version, minor), 8) |
        @shlExact(@as(Version, patch), 0);
}

pub fn VERSION_GET_MAJOR(version: Version) u8 {
    return @truncate(u8, version >> 16);
}

pub fn VERSION_GET_MINOR(version: Version) u8 {
    return @truncate(u8, version >> 8);
}

pub fn VERSION_GET_PATCH(version: Version) u8 {
    return @truncate(u8, version >> 0);
}

pub const VERSION = VERSION_CREATE(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

pub const Version = u32;

pub const SocketType = enum(c_int) {
    Stream = 1,
    Datagram = 2,
};

pub const SocketWait = packed struct {
    send: bool = false,
    receive: bool = false,
    interrupt: bool = false,
    __pad: u29 = 0,
};
comptime {
    assert(@sizeOf(SocketWait) == 4);
}

pub const SocketOption = enum(c_int) {
    nonblock = 1,
    broadcast = 2,
    rcvbuf = 3,
    sndbuf = 4,
    reuseaddr = 5,
    rcvtimeo = 6,
    sndtimeo = 7,
    opt_error = 8, // changed from "error" because that's a keyword
    nodelay = 9,
};

pub const SocketShutdown = enum(c_int) {
    read = 0,
    write = 1,
    read_write = 2,
};

pub const Socket = extern struct {
    handle: pl.SocketHandle,

    pub fn create(kind: SocketType) !Socket {
        const handle = raw.enet_socket_create(kind);
        if (handle == pl.SOCKET_NULL) return error.ENetError;
        return Socket{ .handle = handle };
    }

    pub fn bind(self: Socket, address: Address) !void {
        const rc = raw.enet_socket_bind(self.handle, &address);
        if (rc < 0) return error.ENetError;
    }

    pub fn get_address(self: Socket) !Address {
        var address: Address = undefined;
        const rc = raw.enet_socket_get_address(self.handle, &address);
        if (rc < 0) return error.ENetError;
        return address;
    }

    pub fn listen(self: Socket, backlog: c_int) !void {
        const rc = raw.enet_socket_listen(self.handle, backlog);
        if (rc < 0) return error.ENetError;
    }

    pub fn accept(self: Socket, out_address: ?*Address) !Socket {
        const socket = raw.enet_socket_accept(self.handle, out_address);
        if (socket == pl.SOCKET_NULL) return error.ENetError;
        return Socket{ .handle = socket };
    }

    pub fn connect(self: Socket, address: Address) !void {
        const rc = raw.enet_socket_connect(self.handle, &address);
        if (rc < 0) return error.ENetError;
    }

    pub fn send(self: Socket, buffers: []const pl.Buffer) !usize {
        const rc = raw.enet_socket_send(self.handle, null, buffers.ptr, buffers.len);
        if (rc < 0) return error.ENetError;
        return @intCast(usize, rc);
    }

    pub fn sendTo(self: Socket, address: Address, buffers: []const pl.Buffer) !usize {
        const rc = raw.enet_socket_send(self.handle, &address, buffers.ptr, buffers.len);
        if (rc < 0) return error.ENetError;
        return @intCast(usize, rc);
    }

    pub fn receive(self: Socket, out_address: ?*Address, buffers: []pl.Buffer) !usize {
        const rc = raw.enet_socket_receive(self.handle, out_address, buffers.ptr, buffers.len);
        if (rc < 0) return error.ENetError;
        return @intCast(usize, rc);
    }

    pub fn wait(self: Socket, condition: SocketWait, timeout: u32) !SocketWait {
        var mut_cond = @bitCast(u32, condition);
        const rc = raw.enet_socket_wait(self.handle, &mut_cond, timeout);
        if (rc < 0) return error.ENetError;
        return @bitCast(SocketWait, mut_cond);
    }

    pub fn set_option(self: Socket, option: SocketOption, value: c_int) !void {
        const rc = raw.enet_socket_set_option(self.handle, option, value);
        if (rc < 0) return error.ENetError;
    }

    pub fn get_option(self: Socket, option: SocketOption) !c_int {
        var value: c_int = undefined;
        const rc = raw.enet_socket_get_option(self.handle, option, &value);
        if (rc < 0) return error.ENetError;
        return value;
    }

    pub fn shutdown(self: Socket, how: SocketShutdown) !void {
        const rc = raw.enet_socket_shutdown(self.handle, how);
        if (rc < 0) return error.ENetError;
    }

    pub fn destroy(self: Socket) void {
        raw.enet_socket_destroy(self.handle);
    }
};

pub const HOST_ANY = 0;
pub const HOST_BROADCAST = 0xFFFFFFFF;
pub const PORT_ANY = 0;

/// Portable internet address structure.
///
/// The host must be specified in network byte-order, and the port must be in host
/// byte-order. The constant HOST_ANY may be used to specify the default
/// server host. The constant HOST_BROADCAST may be used to specify the
/// broadcast address (255.255.255.255).  This makes sense for enet_host_connect,
/// but not for enet_host_create.  Once a server responds to a broadcast, the
/// address is updated from HOST_BROADCAST to the server's actual IP address.
pub const Address = extern struct {
    host: u32,
    port: u16,

    /// Attempts to parse the printable form of the IP address in the parameter hostName
    /// and sets the host field in the address parameter if successful.
    /// @param address destination to store the parsed IP address
    /// @param hostName IP address to parse
    pub fn set_host_ip(self: *Address, hostName: [*:0]const u8) !void {
        const rc = raw.enet_address_set_host_ip(self, hostName);
        if (rc < 0) return error.ENetError;
    }

    /// Attempts to resolve the host named by the parameter hostName and sets
    /// the host field in the address parameter if successful.
    /// @param address destination to store resolved address
    /// @param hostName host name to lookup
    pub fn set_host(self: *Address, hostName: [*:0]const u8) !void {
        const rc = raw.enet_address_set_host(self, hostName);
        if (rc < 0) return error.ENetError;
    }

    /// Gives the printable form of the IP address specified in the address parameter.
    /// @param address    address printed
    /// @param buffer     destination for name
    /// @returns the null-terminated name of the host on success
    pub fn get_host_ip(self: Address, buffer: []u8) ![*:0]u8 {
        const rc = raw.enet_address_get_host_ip(&self, buffer.ptr, buffer.len);
        if (rc < 0) return error.ENetError;
        return @ptrCast([*:0]u8, buffer.ptr);
    }

    /// Attempts to do a reverse lookup of the host field in the address parameter.
    /// @param address    address used for reverse lookup
    /// @param buffer     destination for name, must not be NULL
    /// @returns the null-terminated name of the host on success
    pub fn get_host(self: Address, buffer: []u8) ![*:0]u8 {
        const rc = raw.enet_address_get_host(&self, buffer.ptr, buffer.len);
        if (rc < 0) return error.ENetError;
        return @ptrCast([*:0]u8, buffer.ptr);
    }
};

/// Packet flag bit constants.
///
/// The host must be specified in network byte-order, and the port must be in
/// host byte-order. The constant HOST_ANY may be used to specify the
/// default server host.
pub const PacketFlags = packed struct {
    /// packet must be received by the target peer and resend attempts should be
    /// made until the packet is delivered
    reliable: bool = false,

    /// packet will not be sequenced with other packets
    /// not supported for reliable packets
    unsequenced: bool = false,

    /// packet will not allocate data, and user must supply it instead
    no_allocate: bool = false,

    /// packet will be fragmented using unreliable (instead of reliable) sends
    /// if it exceeds the MTU
    unreliable_fragment: bool = false,

    __pad0: u4 = 0,

    /// whether the packet has been sent from all queues it has been entered into
    sent: bool = false,

    // split padding to avoid packed struct bugs
    __pad1: u7 = 0,
    __pad2: u16 = 0,
};
comptime {
    assert(@sizeOf(PacketFlags) == 4);
}

pub const PacketFreeCallback = fn (*Packet) callconv(.C) void;

/// ENet packet structure.
///
/// An ENet data packet that may be sent to or received from a peer. The shown
/// fields should only be read and never modified. The data field contains the
/// allocated data for the packet. The dataLength fields specifies the length
/// of the allocated data.  The flags field is either 0 (specifying no flags),
/// or a bitwise-or of any combination of the following flags:
///
///    reliable - packet must be received by the target peer
///    and resend attempts should be made until the packet is delivered
///
///    unsequenced - packet will not be sequenced with other packets
///    (not supported for reliable packets)
///
///    no_allocate - packet will not allocate data, and user must supply it instead
///
///    unreliable_fragment - packet will be fragmented using unreliable
///    (instead of reliable) sends if it exceeds the MTU
///
///    sent - whether the packet has been sent from all queues it has been entered into
///    @sa PacketFlag
pub const Packet = extern struct {
    /// internal use only
    referenceCount: usize,

    /// packet flags
    flags: PacketFlags,

    /// allocated data for packet
    data: ?[*]u8,

    /// length of data
    dataLength: usize,

    /// function to be called when the packet is no longer in use
    freeCallback: ?PacketFreeCallback,

    /// application private data, may be freely modified
    userData: ?*anyopaque,

    /// Creates a packet that may be sent to a peer.
    /// @param data    initial contents of the packet's data
    pub fn create(data: []u8, flags: PacketFlags) !*Packet {
        const packet = raw.enet_packet_create(data.ptr, data.len, @bitCast(u32, flags));
        if (packet) |p| return p;
        return error.ENetError;
    }

    /// Creates a packet that may be sent to a peer.
    /// @param len    length of the packet's data
    pub fn create_uninitialized(len: usize, flags: PacketFlags) !*Packet {
        const packet = raw.enet_packet_create(null, len, @bitCast(u32, flags));
        if (packet) |p| return p;
        return error.ENetError;
    }

    /// Destroys the packet and deallocates its data.
    pub fn destroy(self: *Packet) void {
        raw.enet_packet_destroy(self);
    }

    /// Attempts to resize the data in the packet to length specified in the
    /// dataLength parameter
    /// @param dataLength new size for the packet data
    pub fn resize(self: *Packet, len: usize) !void {
        const rc = raw.enet_packet_resize(self, len);
        if (rc < 0) return error.ENetError;
    }
};

pub const Acknowledgement = extern struct {
    acknowledgementList: ListNode,
    sentTime: u32,
    command: Protocol,
};

pub const OutgoingCommand = extern struct {
    outgoingCommandList: ListNode,
    reliableSequenceNumber: u16,
    unreliableSequenceNumber: u16,
    sentTime: u32,
    roundTripTimeout: u32,
    roundTripTimeoutLimit: u32,
    fragmentOffset: u32,
    fragmentLength: u16,
    sendAttempts: u16,
    command: Protocol,
    packet: ?*Packet,
};

pub const IncomingCommand = extern struct {
    incomingCommandList: ListNode,
    reliableSequenceNumber: u16,
    unreliableSequenceNumber: u16,
    command: Protocol,
    fragmentCount: u32,
    fragmentsRemaining: u32,
    fragments: ?[*]u32,
    packet: ?*Packet,
};

pub const PeerState = enum(c_int) {
    disconnected = 0,
    connecting = 1,
    acknowledging_connect = 2,
    connection_pending = 3,
    connection_succeeded = 4,
    connected = 5,
    disconnect_later = 6,
    disconnecting = 7,
    acknowledging_disconnect = 8,
    zombie = 9,
};

pub const BUFFER_MAXIMUM = if (@hasDecl(root, "ENET_BUFFER_MAXIMUM")) root.ENET_BUFFER_MAXIMUM else (1 + 2 * Protocol.MAXIMUM_PACKET_COMMANDS);

pub const HOST_RECEIVE_BUFFER_SIZE = 256 * 1024;
pub const HOST_SEND_BUFFER_SIZE = 256 * 1024;
pub const HOST_BANDWIDTH_THROTTLE_INTERVAL = 1000;
pub const HOST_DEFAULT_MTU = 1400;
pub const HOST_DEFAULT_MAXIMUM_PACKET_SIZE = 32 * 1024 * 1024;
pub const HOST_DEFAULT_MAXIMUM_WAITING_DATA = 32 * 1024 * 1024;

pub const PEER_DEFAULT_ROUND_TRIP_TIME = 500;
pub const PEER_DEFAULT_PACKET_THROTTLE = 32;
pub const PEER_PACKET_THROTTLE_SCALE = 32;
pub const PEER_PACKET_THROTTLE_COUNTER = 7;
pub const PEER_PACKET_THROTTLE_ACCELERATION = 2;
pub const PEER_PACKET_THROTTLE_DECELERATION = 2;
pub const PEER_PACKET_THROTTLE_INTERVAL = 5000;
pub const PEER_PACKET_LOSS_SCALE = (1 << 16);
pub const PEER_PACKET_LOSS_INTERVAL = 10000;
pub const PEER_WINDOW_SIZE_SCALE = 64 * 1024;
pub const PEER_TIMEOUT_LIMIT = 32;
pub const PEER_TIMEOUT_MINIMUM = 5000;
pub const PEER_TIMEOUT_MAXIMUM = 30000;
pub const PEER_PING_INTERVAL = 500;
pub const PEER_UNSEQUENCED_WINDOWS = 64;
pub const PEER_UNSEQUENCED_WINDOW_SIZE = 1024;
pub const PEER_FREE_UNSEQUENCED_WINDOWS = 32;
pub const PEER_RELIABLE_WINDOWS = 16;
pub const PEER_RELIABLE_WINDOW_SIZE = 0x1000;
pub const PEER_FREE_RELIABLE_WINDOWS = 8;

pub const Channel = extern struct {
    outgoingReliableSequenceNumber: u16,
    outgoingUnreliableSequenceNumber: u16,
    usedReliableWindows: u16,
    reliableWindows: [PEER_RELIABLE_WINDOWS]u16,
    incomingReliableSequenceNumber: u16,
    incomingUnreliableSequenceNumber: u16,
    incomingReliableCommands: List(IncomingCommand),
    incomingUnreliableCommands: List(IncomingCommand),
};

pub const PeerFlags = packed struct {
    needs_dispatch: bool = false,
    __pad0: u15 = 0,
};
comptime {
    assert(@sizeOf(PeerFlags) == 2);
}

/// An ENet peer which data packets may be sent or received from.
///
/// No fields should be modified unless otherwise specified.
pub const Peer = extern struct {
    dispatchList: ListNode,
    host: ?*Host,
    outgoingPeerID: u16,
    incomingPeerID: u16,
    connectID: u32,
    outgoingSessionID: u8,
    incomingSessionID: u8,

    /// Internet address of the peer
    address: Address,

    /// Application private data, may be freely modified
    data: ?*anyopaque,

    state: PeerState,
    channels: ?[*]Channel,

    /// Number of channels allocated for communication with peer
    channelCount: usize,

    /// Downstream bandwidth of the client in bytes/second
    incomingBandwidth: u32,

    /// Upstream bandwidth of the client in bytes/second
    outgoingBandwidth: u32,

    incomingBandwidthThrottleEpoch: u32,
    outgoingBandwidthThrottleEpoch: u32,
    incomingDataTotal: u32,
    outgoingDataTotal: u32,
    lastSendTime: u32,
    lastReceiveTime: u32,
    nextTimeout: u32,
    earliestTimeout: u32,
    packetLossEpoch: u32,
    packetsSent: u32,
    packetsLost: u32,

    /// mean packet loss of reliable packets as a ratio with respect to the constant PEER_PACKET_LOSS_SCALE
    packetLoss: u32,

    packetLossVariance: u32,
    packetThrottle: u32,
    packetThrottleLimit: u32,
    packetThrottleCounter: u32,
    packetThrottleEpoch: u32,
    packetThrottleAcceleration: u32,
    packetThrottleDeceleration: u32,
    packetThrottleInterval: u32,
    pingInterval: u32,
    timeoutLimit: u32,
    timeoutMinimum: u32,
    timeoutMaxiumum: u32,
    lastRoundTripTime: u32,
    lowestRoundTripTime: u32,
    lastRoundTripTimeVariance: u32,
    highestRoundTripTimeVariance: u32,
    /// mean round trip time (RTT), in milliseconds, between sending a reliable packet and receiving its acknowledgement
    roundTripTime: u32,
    roundTripTimeVariance: u32,
    mtu: u32,
    windowSize: u32,
    reliableDataInTransit: u32,
    outgoingReliableSequenceNumber: u16,
    acknowledgements: List(Acknowledgement),
    sendReliableCommands: List(OutgoingCommand),
    sentUnreliableCommands: List(OutgoingCommand),
    outgoingCommands: List(OutgoingCommand),
    dispatchedCommands: List(OutgoingCommand),
    flags: PeerFlags,
    reserved: u16,
    incomingUnsequencedGroup: u16,
    outgoingUnsequencedGroup: u16,
    unsequencedWindow: [@divExact(PEER_UNSEQUENCED_WINDOW_SIZE, 32)]u32,
    eventData: u32,
    totalWaitingData: usize,

    /// Queues a packet to be sent.
    /// @param peer destination for the packet
    /// @param channelID channel on which to send
    /// @param packet packet to send
    pub fn send(self: *Peer, channelID: u8, packet: *Packet) !void {
        const rc = raw.enet_peer_send(self, channelID, packet);
        if (rc < 0) return error.ENetError;
    }

    /// Attempts to dequeue any incoming queued packet.
    /// @param peer peer to dequeue packets from
    /// @param channelID holds the channel ID of the channel the packet was received on success
    /// @returns a pointer to the packet, or NULL if there are no available incoming queued packets
    pub fn receive(self: *Peer, out_channelID: *u8) ?*Packet {
        return raw.enet_peer_receive(self, out_channelID);
    }

    /// Sends a ping request to a peer.
    /// @param peer destination for the ping request
    /// @remarks ping requests factor into the mean round trip time as designated by the
    /// roundTripTime field in the Peer structure.  ENet automatically pings all connected
    /// peers at regular intervals, however, this function may be called to ensure more
    /// frequent ping requests.
    pub fn ping(self: *Peer) void {
        raw.enet_peer_ping(self);
    }

    /// Sets the interval at which pings will be sent to a peer.
    ///
    /// Pings are used both to monitor the liveness of the connection and also to dynamically
    /// adjust the throttle during periods of low traffic so that the throttle has reasonable
    /// responsiveness during traffic spikes.
    /// @param peer the peer to adjust
    /// @param pingInterval the interval at which to send pings; defaults to PEER_PING_INTERVAL if 0
    pub fn ping_interval(self: *Peer, interval: u32) void {
        raw.enet_peer_ping_interval(self, interval);
    }

    /// Sets the timeout parameters for a peer.
    ///
    /// The timeout parameter control how and when a peer will timeout from a failure to acknowledge
    /// reliable traffic. Timeout values use an exponential backoff mechanism, where if a reliable
    /// packet is not acknowledge within some multiple of the average RTT plus a variance tolerance,
    /// the timeout will be doubled until it reaches a set limit. If the timeout is thus at this
    /// limit and reliable packets have been sent but not acknowledged within a certain minimum time
    /// period, the peer will be disconnected. Alternatively, if reliable packets have been sent
    /// but not acknowledged for a certain maximum time period, the peer will be disconnected regardless
    /// of the current timeout limit value.
    ///
    /// @param peer the peer to adjust
    /// @param timeoutLimit the timeout limit; defaults to PEER_TIMEOUT_LIMIT if 0
    /// @param timeoutMinimum the timeout minimum; defaults to PEER_TIMEOUT_MINIMUM if 0
    /// @param timeoutMaximum the timeout maximum; defaults to PEER_TIMEOUT_MAXIMUM if 0
    pub fn timeout(self: *Peer, timeoutLimit: u32, timeoutMinimum: u32, timeoutMaximum: u32) void {
        raw.enet_peer_timeout(self, timeoutLimit, timeoutMinimum, timeoutMaximum);
    }

    /// Forcefully disconnects a peer.
    /// @param peer peer to forcefully disconnect
    /// @remarks The foreign host represented by the peer is not notified of the disconnection and will timeout
    /// on its connection to the local host.
    pub fn reset(self: *Peer) void {
        raw.enet_peer_reset(self);
    }

    /// Request a disconnection from a peer.
    /// @param peer peer to request a disconnection
    /// @param data data describing the disconnection
    /// @remarks A .disconnect event will be generated by Host.service()
    /// once the disconnection is complete.
    pub fn disconnect(self: *Peer, data: u32) void {
        raw.enet_peer_disconnect(self, data);
    }

    /// Force an immediate disconnection from a peer.
    /// @param peer peer to disconnect
    /// @param data data describing the disconnection
    /// @remarks No .disconnect event will be generated. The foreign peer is not
    /// guaranteed to receive the disconnect notification, and is reset immediately upon
    /// return from this function.
    pub fn disconnect_now(self: *Peer, data: u32) void {
        raw.enet_peer_disconnect_now(self, data);
    }

    /// Request a disconnection from a peer, but only after all queued outgoing packets are sent.
    /// @param peer peer to request a disconnection
    /// @param data data describing the disconnection
    /// @remarks A .disconnect event will be generated by Host.service()
    /// once the disconnection is complete.
    pub fn disconnect_later(self: *Peer, data: u32) void {
        raw.enet_peer_disconnect_later(self, data);
    }

    /// Configures throttle parameter for a peer.
    ///
    /// Unreliable packets are dropped by ENet in response to the varying conditions
    /// of the Internet connection to the peer.  The throttle represents a probability
    /// that an unreliable packet should not be dropped and thus sent by ENet to the peer.
    /// The lowest mean round trip time from the sending of a reliable packet to the
    /// receipt of its acknowledgement is measured over an amount of time specified by
    /// the interval parameter in milliseconds.  If a measured round trip time happens to
    /// be significantly less than the mean round trip time measured over the interval,
    /// then the throttle probability is increased to allow more traffic by an amount
    /// specified in the acceleration parameter, which is a ratio to the PEER_PACKET_THROTTLE_SCALE
    /// constant.  If a measured round trip time happens to be significantly greater than
    /// the mean round trip time measured over the interval, then the throttle probability
    /// is decreased to limit traffic by an amount specified in the deceleration parameter, which
    /// is a ratio to the PEER_PACKET_THROTTLE_SCALE constant.  When the throttle has
    /// a value of PEER_PACKET_THROTTLE_SCALE, no unreliable packets are dropped by
    /// ENet, and so 100% of all unreliable packets will be sent.  When the throttle has a
    /// value of 0, all unreliable packets are dropped by ENet, and so 0% of all unreliable
    /// packets will be sent.  Intermediate values for the throttle represent intermediate
    /// probabilities between 0% and 100% of unreliable packets being sent.  The bandwidth
    /// limits of the local and foreign hosts are taken into account to determine a
    /// sensible limit for the throttle probability above which it should not raise even in
    /// the best of conditions.
    ///
    /// @param peer peer to configure
    /// @param interval interval, in milliseconds, over which to measure lowest mean RTT; the default value is PEER_PACKET_THROTTLE_INTERVAL.
    /// @param acceleration rate at which to increase the throttle probability as mean RTT declines
    /// @param deceleration rate at which to decrease the throttle probability as mean RTT increases
    pub fn throttle_configure(self: *Peer, interval: u32, acceleration: u32, deceleration: u32) void {
        raw.enet_peer_throttle_configure(self, interval, acceleration, deceleration);
    }
};

pub const Compressor = extern struct {
    /// Context data for the compressor. Must be non-NULL.
    context: *anyopaque,

    /// Compresses from inBuffers[0..inBufferCount], containing inLimit bytes, to outData, outputting at most outLimit bytes. Should return 0 on failure.
    compress: fn (
        context: *anyopaque,
        inBuffers: [*]const pl.Buffer,
        inBufferCount: usize,
        inLimit: usize,
        outData: [*]u8,
        outLimit: usize,
    ) callconv(.C) usize,

    /// Decompresses from inData, containing inLimit bytes, to outData, outputting at most outLimit bytes. Should return 0 on failure.
    decompress: fn (
        context: *anyopaque,
        inData: [*]const u8,
        inLimit: usize,
        outData: [*]u8,
        outLimit: usize,
    ) callconv(.C) usize,

    /// Destroys the context when compression is disabled or the host is destroyed. May be NULL.
    destroy: ?fn (
        context: *anyopaque,
    ) callconv(.C) void,
};

/// Callback that computes the checksum of the data held in buffers[0..bufferCount]
pub const ChecksumCallback = fn (buffers: [*]const pl.Buffer, bufferCount: usize) callconv(.C) u32;

/// Callback for intercepting received raw UDP packets. Should return 1 to intercept, 0 to ignore, or -1 to propagate an error.
pub const InterceptCallback = fn (host: ?*Host, event: ?*Event) callconv(.C) c_int;

/// An ENet host for communicating with peers.
///
/// No fields should be modified unless otherwise stated.
pub const Host = extern struct {
    socket: Socket,
    /// Internet address of the host
    address: Address,
    /// downstream bandwidth of the host
    incomingBandwidth: u32,
    /// upstream bandwidth of the host
    outgoingBandwidth: u32,
    bandwidthThrottleEpoch: u32,
    mtu: u32,
    randomSeed: u32,
    recalculateBandwidthLimits: c_int,
    /// array of peers allocated for this host
    peers: ?[*]Peer,
    /// number of peers allocated for this host
    peerCount: usize,
    /// maximum number of channels allowed for connected peers
    channelLimit: usize,
    serviceTime: u32,
    dispatchQueue: List(Peer),
    continueSending: c_int,
    packetSize: usize,
    headerFlags: u16, // TODO is this a flag type?
    commands: [Protocol.MAXIMUM_PACKET_COMMANDS]Protocol,
    commandCount: usize,
    buffers: [BUFFER_MAXIMUM]pl.Buffer,
    bufferCount: usize,
    /// callback the user can set to enable packet checksums for this host
    checksum: ?ChecksumCallback,
    compressor: Compressor,
    packetData: [2][Protocol.MAXIMUM_MTU]u8,
    receivedAddress: Address,
    receivedData: ?[*]u8,
    receivedDataLength: usize,
    /// total data sent, user should reset to 0 as needed to prevent overflow
    totalSentData: u32,
    /// total UDP packets sent, user should reset to 0 as needed to prevent overflow
    totalSentPackets: u32,
    /// total data received, user should reset to 0 as needed to prevent overflow
    totalReceivedData: u32,
    /// total UDP packets received, user should reset to 0 as needed to prevent overflow
    totalReceivedPackets: u32,
    /// callback the user can set to intercept received raw UDP packets
    intercept: ?InterceptCallback,
    connectedPeers: usize,
    bandwidthLimitedPeers: usize,
    /// optional number of allowed peers from duplicate IPs, defaults to Protocol.MAXIMUM_PEER_ID
    duplicatePeers: usize,
    /// the maximum allowable packet size that may be sent or received on a peer
    maximumPacketSize: usize,
    /// the maximum aggregate amount of buffer space a peer may use waiting for packets to be delivered
    maximumWaitingData: usize,

    /// Creates a host for communicating to peers.
    /// @param address   the address at which other peers may connect to this host.  If NULL, then no peers may connect to the host.
    /// @param peerCount the maximum number of peers that should be allocated for the host.
    /// @param channelLimit the maximum number of channels allowed; if 0, then this is equivalent to PROTOCOL_MAXIMUM_CHANNEL_COUNT
    /// @param incomingBandwidth downstream bandwidth of the host in bytes/second; if 0, ENet will assume unlimited bandwidth.
    /// @param outgoingBandwidth upstream bandwidth of the host in bytes/second; if 0, ENet will assume unlimited bandwidth.
    /// @returns the host on success and error.ENetError on failure
    /// @remarks ENet will strategically drop packets on specific sides of a connection between hosts
    /// to ensure the host's bandwidth is not overwhelmed.  The bandwidth parameters also determine
    /// the window size of a connection which limits the amount of reliable packets that may be in transit
    /// at any given time.
    pub fn create(address: ?Address, peerCount: usize, channelLimit: usize, incomingBandwidth: u32, outgoingBandwidth: u32) !*Host {
        const addressPtr: ?*const Address = if (address) |*a| a else null;
        const host = raw.enet_host_create(addressPtr, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
        if (host) |h| return h;
        return error.ENetError;
    }

    /// Destroys the host and all resources associated with it.
    /// @param host pointer to the host to destroy
    pub fn destroy(self: *Host) void {
        raw.enet_host_destroy(self);
    }

    /// Initiates a connection to a foreign host.
    /// @param host host seeking the connection
    /// @param address destination for the connection
    /// @param channelCount number of channels to allocate
    /// @param data user data supplied to the receiving host
    /// @returns a peer representing the foreign host on success, error.ENetError on failure
    /// @remarks The peer returned will have not completed the connection until service()
    /// notifies of a .connect event for the peer.
    pub fn connect(self: *Host, address: Address, channelCount: usize, data: u32) !*Peer {
        const peer = raw.enet_host_connect(self, &address, channelCount, data);
        if (peer) |p| return p;
        return error.ENetError;
    }

    /// Checks for any queued events on the host and dispatches one if available.
    /// @param host    host to check for events
    /// @param event   an event structure where event details will be placed if available
    /// If no event was dispatched, the returned event has type .none
    pub fn check_events(self: *Host) !Event {
        var event: Event = undefined;
        const rc = raw.enet_host_check_events(self, &event);
        if (rc < 0) return error.ENetError;
        return event;
    }

    /// Waits for events on the host specified and shuttles packets between
    /// the host and its peers.
    /// @param host    host to service
    /// @param event   an event structure where event details will be placed if one occurs
    ///                if event == null then no events will be delivered
    /// @param timeout number of milliseconds that ENet should wait for events
    /// @retval true if an event occurred within the specified time limit
    /// @retval false if no event occurred
    /// @retval error.ENetError on failure
    /// @remarks service() should be called fairly regularly for adequate performance
    pub fn service(self: *Host, event: ?*Event, timeout: u32) !bool {
        const rc = raw.enet_host_service(self, event, timeout);
        if (rc < 0) return error.ENetError;
        return rc > 0;
    }

    /// Sends any queued packets on the host specified to its designated peers.
    /// @param host   host to flush
    /// @remarks this function need only be used in circumstances where one wishes to send queued packets earlier than in a call to enet_host_service().
    pub fn flush(self: *Host) void {
        raw.enet_host_flush(self);
    }

    /// Queues a packet to be sent to all peers associated with the host.
    /// @param host host on which to broadcast the packet
    /// @param channelID channel on which to broadcast
    /// @param packet packet to broadcast
    pub fn broadcast(self: *Host, channelID: u8, packet: *Packet) void {
        raw.enet_host_broadcast(self, channelID, packet);
    }

    /// Sets the packet compressor the host should use to compress and decompress packets.
    /// @param host host to enable or disable compression for
    /// @param compressor callbacks for for the packet compressor; if NULL, then compression is disabled
    pub fn compress(self: *Host, compressor: ?*const Compressor) void {
        raw.enet_host_compress(self, compressor);
    }

    /// Sets the packet compressor the host should use to the default range coder.
    /// @param host host to enable the range coder for
    pub fn compress_with_range_coder(self: *Host) !void {
        const rc = raw.enet_host_compress_with_range_coder(self);
        if (rc < 0) return error.ENetError;
    }

    /// Limits the maximum allowed channels of future incoming connections.
    /// @param host host to limit
    /// @param channelLimit the maximum number of channels allowed; if 0, then this is equivalent to PROTOCOL_MAXIMUM_CHANNEL_COUNT
    pub fn channel_limit(self: *Host, limit: usize) void {
        raw.enet_host_channel_limit(self, limit);
    }

    /// Adjusts the bandwidth limits of a host.
    /// @param host host to adjust
    /// @param incomingBandwidth new incoming bandwidth
    /// @param outgoingBandwidth new outgoing bandwidth
    /// @remarks the incoming and outgoing bandwidth parameters are identical in function to those
    /// specified in Host.create().
    pub fn bandwidth_limit(self: *Host, incomingBandwidth: u32, outgoingBandwidth: u32) void {
        raw.enet_host_bandwidth_limit(self, incomingBandwidth, outgoingBandwidth);
    }
};

/// An ENet event type, as specified in @ref Event
pub const EventType = enum(c_int) {
    /// no event occurred within the specified time limit
    none = 0,

    /// a connection request initiated by host_connect has completed.
    /// The peer field contains the peer which successfully connected.
    connect = 1,

    /// a peer has disconnected.  This event is generated on a successful
    /// completion of a disconnect initiated by peer_disconnect, if
    /// a peer has timed out, or if a connection request intialized by
    /// host_connect has timed out.  The peer field contains the peer
    /// which disconnected. The data field contains user supplied data
    /// describing the disconnection, or 0, if none is available.
    disconnect = 2,

    /// a packet has been received from a peer.  The peer field specifies the
    /// peer which sent the packet.  The channelID field specifies the channel
    /// number upon which the packet was received.  The packet field contains
    /// the packet that was received; this packet must be destroyed with
    /// packet_destroy after use.
    receive = 3,
};

/// An ENet event as returned by host_service().
///
/// @sa host_service
pub const Event = extern struct {
    /// type of the event
    type: EventType,
    /// peer that generated a connect, disconnect or receive event
    peer: ?*Peer,
    /// channel on the peer that generated the event, if appropriate
    channelID: u8,
    /// data associated with the event, if appropriate
    data: u32,
    /// packet associated with the event, if appropriate
    packet: ?*Packet,
};

// @defgroup global ENet global functions
// @{

/// fn initialize() c_int
/// Initializes ENet globally.  Must be called prior to using any functions in
/// ENet.
pub fn initialize() !void {
    const rc = raw.enet_initialize();
    if (rc < 0) return error.ENetError;
}

/// fn initialize_with_callbacks(inits: *const Callbacks) c_int
/// Initializes ENet globally and supplies user-overridden callbacks.
/// Must be called prior to using any functions in ENet. Do not use enet_initialize() if you use this variant.
/// Make sure the ENetCallbacks structure is zeroed out so that any additional
/// callbacks added in future versions will be properly ignored.
/// @param inits user-overridden callbacks where any NULL callbacks will use ENet's defaults
/// @returns 0 on success, < 0 on failure
pub fn initialize_with_callbacks(inits: *const Callbacks) !void {
    const rc = raw.enet_initialize_with_callbacks(VERSION, inits);
    if (rc < 0) return error.ENetError;
}

/// fn deinitialize() void
/// Shuts down ENet globally.  Should be called when a program that has
/// initialized ENet exits.
pub const deinitialize = raw.enet_deinitialize;

/// fn linked_version() Version
/// Gives the linked version of the ENet library.
/// @returns the version number
pub const linked_version = raw.enet_linked_version;

// @}
// @defgroup private ENet private implementation functions

/// fn time_get() u32
/// Returns the wall-time in milliseconds.  Its initial value is unspecified
/// unless otherwise set.
pub const time_get = raw.enet_time_get;

/// fn time_set(u32) void
/// Sets the current wall-time in milliseconds.
pub const time_set = raw.enet_time_set;

/// fn crc32([]Buffer) u32
/// Computes the crc32 hash of a series of buffers
pub fn crc32(buffers: []pl.Buffer) u32 {
    return raw.enet_crc32(buffers.ptr, buffers.len);
}

pub fn select(max_socket: pl.SocketHandle, in_out_read: ?*pl.SocketSet, in_out_write: ?*pl.SocketSet, timeout: u32) !void {
    const rc = raw.enet_socketset_select(max_socket, in_out_read, in_out_write, timeout);
    if (rc < 0) return error.ENetError;
}

pub const raw = struct {
    pub extern fn enet_initialize() callconv(.C) c_int;
    pub extern fn enet_initialize_with_callbacks(version: Version, inits: *const Callbacks) callconv(.C) c_int;
    pub extern fn enet_deinitialize() callconv(.C) void;
    pub extern fn enet_linked_version() callconv(.C) Version;

    pub extern fn enet_time_get() callconv(.C) u32;
    pub extern fn enet_time_set(time: u32) callconv(.C) void;

    pub extern fn enet_socket_create(SocketType) callconv(.C) pl.SocketHandle;
    pub extern fn enet_socket_bind(pl.SocketHandle, *const Address) callconv(.C) c_int;
    pub extern fn enet_socket_get_address(pl.SocketHandle, *Address) callconv(.C) c_int;
    pub extern fn enet_socket_listen(pl.SocketHandle, c_int) callconv(.C) c_int;
    pub extern fn enet_socket_accept(pl.SocketHandle, ?*Address) callconv(.C) pl.SocketHandle;
    pub extern fn enet_socket_connect(pl.SocketHandle, *const Address) callconv(.C) c_int;
    pub extern fn enet_socket_send(pl.SocketHandle, ?*const Address, [*]const pl.Buffer, usize) callconv(.C) c_int;
    pub extern fn enet_socket_receive(pl.SocketHandle, ?*Address, [*]pl.Buffer, usize) callconv(.C) c_int;
    pub extern fn enet_socket_wait(pl.SocketHandle, *u32, u32) callconv(.C) c_int;
    pub extern fn enet_socket_set_option(pl.SocketHandle, SocketOption, c_int) callconv(.C) c_int;
    pub extern fn enet_socket_get_option(pl.SocketHandle, SocketOption, *c_int) callconv(.C) c_int;
    pub extern fn enet_socket_shutdown(pl.SocketHandle, SocketShutdown) callconv(.C) c_int;
    pub extern fn enet_socket_destroy(pl.SocketHandle) callconv(.C) void;
    pub extern fn enet_socketset_select(pl.SocketHandle, ?*pl.SocketSet, ?*pl.SocketSet, u32) callconv(.C) c_int;

    pub extern fn enet_address_set_host_ip(address: *Address, hostName: [*:0]const u8) callconv(.C) c_int;
    pub extern fn enet_address_set_host(address: *Address, hostName: [*:0]const u8) callconv(.C) c_int;
    pub extern fn enet_address_get_host_ip(address: *const Address, hostName: [*]u8, nameLength: usize) callconv(.C) c_int;
    pub extern fn enet_address_get_host(address: *const Address, hostName: [*]u8, nameLength: usize) callconv(.C) c_int;

    pub extern fn enet_packet_create(?[*]const u8, usize, u32) callconv(.C) ?*Packet;
    pub extern fn enet_packet_destroy(*Packet) callconv(.C) void;
    pub extern fn enet_packet_resize(*Packet, usize) callconv(.C) c_int;
    pub extern fn enet_crc32([*]const pl.Buffer, usize) callconv(.C) u32;

    pub extern fn enet_host_create(?*const Address, usize, usize, u32, u32) callconv(.C) ?*Host;
    pub extern fn enet_host_destroy(*Host) callconv(.C) void;
    pub extern fn enet_host_connect(*Host, *const Address, usize, u32) callconv(.C) ?*Peer;
    pub extern fn enet_host_check_events(*Host, *Event) callconv(.C) c_int;
    pub extern fn enet_host_service(*Host, ?*Event, u32) callconv(.C) c_int;
    pub extern fn enet_host_flush(*Host) callconv(.C) void;
    pub extern fn enet_host_broadcast(*Host, u8, *Packet) callconv(.C) void;
    pub extern fn enet_host_compress(*Host, ?*const Compressor) callconv(.C) void;
    pub extern fn enet_host_compress_with_range_coder(*Host) callconv(.C) c_int;
    pub extern fn enet_host_channel_limit(*Host, usize) callconv(.C) void;
    pub extern fn enet_host_bandwidth_limit(*Host, u32, u32) callconv(.C) void;

    pub extern fn enet_peer_send(*Peer, u8, *Packet) callconv(.C) c_int;
    pub extern fn enet_peer_receive(*Peer, channelID: *u8) callconv(.C) ?*Packet;
    pub extern fn enet_peer_ping(*Peer) callconv(.C) void;
    pub extern fn enet_peer_ping_interval(*Peer, u32) callconv(.C) void;
    pub extern fn enet_peer_timeout(*Peer, u32, u32, u32) callconv(.C) void;
    pub extern fn enet_peer_reset(*Peer) callconv(.C) void;
    pub extern fn enet_peer_disconnect(*Peer, u32) callconv(.C) void;
    pub extern fn enet_peer_disconnect_now(*Peer, u32) callconv(.C) void;
    pub extern fn enet_peer_disconnect_later(*Peer, u32) callconv(.C) void;
    pub extern fn enet_peer_throttle_configure(*Peer, u32, u32, u32) callconv(.C) void;

    pub extern fn enet_range_coder_create() callconv(.C) *anyopaque;
    pub extern fn enet_range_coder_destroy(*anyopaque) callconv(.C) void;
    pub extern fn enet_range_coder_compress(*anyopaque, *const pl.Buffer, usize, usize, [*]u8, usize) callconv(.C) usize;
    pub extern fn enet_range_coder_decompress(*anyopaque, [*]const u8, usize, [*]u8, usize) callconv(.C) usize;

    /// These functions are declared in enet.h but are not exported in DLL builds.
    /// They probably shouldn't be used.
    pub const secret = struct {
        pub extern fn enet_host_bandwidth_throttle(*Host) callconv(.C) void;
        pub extern fn enet_host_random_seed() callconv(.C) u32;
        pub extern fn enet_peer_throttle(*Peer, u32) callconv(.C) c_int;
        pub extern fn enet_peer_reset_queues(*Peer) callconv(.C) void;
        pub extern fn enet_peer_setup_outgoing_command(*Peer, *OutgoingCommand) callconv(.C) void;
        pub extern fn enet_peer_queue_outgoing_command(*Peer, *const Protocol, *Packet, u32, u16) callconv(.C) *OutgoingCommand;
        pub extern fn enet_peer_queue_incoming_command(*Peer, *const Protocol, [*]const u8, usize, u32, u32) callconv(.C) *IncomingCommand;
        pub extern fn enet_peer_queue_acknowledgement(*Peer, *const Protocol, u16) callconv(.C) *Acknowledgement;
        pub extern fn enet_peer_dispatch_incoming_unreliable_commands(*Peer, *Channel, *IncomingCommand) callconv(.C) void;
        pub extern fn enet_peer_dispatch_incoming_reliable_commands(*Peer, *Channel, *IncomingCommand) callconv(.C) void;
        pub extern fn enet_peer_on_connect(*Peer) callconv(.C) void;
        pub extern fn enet_peer_on_disconnect(*Peer) callconv(.C) void;
        pub extern fn enet_protocol_command_size(u8) callconv(.C) usize;
        pub extern fn enet_malloc(usize) callconv(.C) ?*anyopaque;
        pub extern fn enet_free(?*anyopaque) callconv(.C) void;
    };
};

comptime {
    _ = PacketFreeCallback;
    _ = InterceptCallback;
    std.testing.refAllDecls(@This());
    std.testing.refAllDecls(@This().raw);
    std.testing.refAllDecls(@This().Host);
    std.testing.refAllDecls(@This().Peer);
    std.testing.refAllDecls(@This().Protocol);
    std.testing.refAllDecls(@This().Socket);
    std.testing.refAllDecls(@This().ListNode);
    std.testing.refAllDecls(@This().WindowsPlatform);
    std.testing.refAllDecls(@This().UnixPlatform);
}

test "zenet.init" {
    try initialize();
    defer deinitialize();
}
