const std = @import("std");

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

pub fn RingQueue(comptime T: type, comptime _capacity: u16) type {
    return struct {
        const Self = @This();

        pub const capacity = _capacity;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // zig fmt: off
        _len  : usize       = 0,
        _head : usize       = 0,
        _tail : usize       = 0,
        _data : [capacity]T = undefined,
        // zig fmt: on

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub inline fn len(self: *const Self) usize {
            return @atomicLoad(usize, &self._len, .Monotonic);
        }

        pub inline fn isEmpty(self: *const Self) bool {
            return self.len() == 0;
        }

        pub inline fn isFull(self: *const Self) bool {
            return self.len() == capacity;
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub inline fn dequeueIfNotEmpty(self: *Self) ?T {
            if (self.isEmpty()) return null;
            return self.dequeueUnchecked();
        }

        pub inline fn enqueueIfNotFull(self: *Self, value: T) bool {
            if (self.isFull()) return false;
            self.enqueueUnchecked(value);
            return true;
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        pub inline fn dequeueAssumeNotEmpty(self: *Self) T {
            std.debug.assert(!self.isEmpty());
            return self.dequeueUnchecked();
        }

        pub inline fn enqueueAssumeNotFull(self: *Self, value: T) void {
            std.debug.assert(!self.isFull());
            self.enqueueUnchecked(value);
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        inline fn enqueueUnchecked(self: *Self, value: T) void {
            const old_tail_index = self._tail % capacity;
            self._data[old_tail_index] = value;
            self._tail = (old_tail_index +% 1) % capacity;
            self._len += 1;
        }

        inline fn dequeueUnchecked(self: *Self) T {
            const old_head_index = self._head % capacity;
            const value = self._data[old_head_index];
            self._head = (old_head_index +% 1) % capacity;
            self._len -= 1;
            return value;
        }
    };
}

////////////////////////////////// T E S T S ///////////////////////////////////

test "RingQueue basics" {
    var q = RingQueue(u8, 4){};

    const expectEqual = std.testing.expectEqual;

    try expectEqual(@as(usize, 0), q.len());
    try expectEqual(true, q.isEmpty());
    try expectEqual(false, q.isFull());

    try expectEqual(true, q.enqueueIfNotFull('a'));
    try expectEqual(true, q.enqueueIfNotFull('b'));
    try expectEqual(true, q.enqueueIfNotFull('c'));
    try expectEqual(true, q.enqueueIfNotFull('d'));
    try expectEqual(false, q.enqueueIfNotFull('e'));

    try expectEqual(@as(usize, 4), q.len());
    try expectEqual(false, q.isEmpty());
    try expectEqual(true, q.isFull());

    try expectEqual(@as(u8, 'a'), q.dequeueIfNotEmpty().?);
    try expectEqual(@as(u8, 'b'), q.dequeueIfNotEmpty().?);
    try expectEqual(@as(u8, 'c'), q.dequeueIfNotEmpty().?);
    try expectEqual(@as(u8, 'd'), q.dequeueIfNotEmpty().?);
    try expectEqual(null, q.dequeueIfNotEmpty());

    try expectEqual(@as(usize, 0), q.len());
    try expectEqual(true, q.isEmpty());
    try expectEqual(false, q.isFull());
}

//------------------------------------------------------------------------------
