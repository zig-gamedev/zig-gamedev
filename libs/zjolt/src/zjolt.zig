const std = @import("std");
const assert = std.debug.assert;
//const c = @cImport(@cInclude("JoltC.h"));
//
//const layers = struct {
//    const non_moving: c.JPH_ObjectLayer = 0;
//    const moving: c.JPH_ObjectLayer = 1;
//    const num: u32 = 2;
//};
//
//const broad_phase_layers = struct {
//    const non_moving: c.JPH_BroadPhaseLayer = 0;
//    const moving: c.JPH_BroadPhaseLayer = 1;
//    const num: u32 = 2;
//};
//
//const BPLayerInterfaceImpl = extern struct {
//    vtable_ptr: *const c.JPH_BroadPhaseLayerInterfaceVTable = &vtable,
//    object_to_broad_phase: [layers.num]c.JPH_BroadPhaseLayer = undefined,
//
//    const vtable = c.JPH_BroadPhaseLayerInterfaceVTable{
//        .reserved0 = null,
//        .reserved1 = null,
//        .GetNumBroadPhaseLayers = getNumBroadPhaseLayers,
//        .GetBroadPhaseLayer = getBroadPhaseLayer,
//    };
//
//    fn init() BPLayerInterfaceImpl {
//        var layer_interface: BPLayerInterfaceImpl = .{};
//        layer_interface.object_to_broad_phase[layers.non_moving] = broad_phase_layers.non_moving;
//        layer_interface.object_to_broad_phase[layers.moving] = broad_phase_layers.moving;
//        return layer_interface;
//    }
//
//    fn getNumBroadPhaseLayers(self: ?*const anyopaque) callconv(.C) u32 {
//        const layer_interface = @ptrCast(*const BPLayerInterfaceImpl, @alignCast(@sizeOf(usize), self));
//        return @intCast(u32, layer_interface.object_to_broad_phase.len);
//    }
//
//    fn getBroadPhaseLayer(self: ?*const anyopaque, layer: c.JPH_ObjectLayer) callconv(.C) c.JPH_BroadPhaseLayer {
//        const layer_interface = @ptrCast(*const BPLayerInterfaceImpl, @alignCast(@sizeOf(usize), self));
//        assert(layer < layers.num);
//        return layer_interface.object_to_broad_phase[@intCast(usize, layer)];
//    }
//};
//
//fn myBroadPhaseCanCollide(inLayer1: c.JPH_ObjectLayer, inLayer2: c.JPH_BroadPhaseLayer) callconv(.C) bool {
//    return switch (inLayer1) {
//        layers.non_moving => inLayer2 == broad_phase_layers.moving,
//        layers.moving => true,
//        else => unreachable,
//    };
//}
//
//fn myObjectCanCollide(inObject1: c.JPH_ObjectLayer, inObject2: c.JPH_ObjectLayer) callconv(.C) bool {
//    return switch (inObject1) {
//        layers.non_moving => inObject2 == layers.moving,
//        layers.moving => true,
//        else => unreachable,
//    };
//}
//
const expect = std.testing.expect;

extern fn JoltCTest_Basic1() u32;
test "jolt_c.basic1" {
    const ret = JoltCTest_Basic1();
    try expect(ret != 0);
}

extern fn JoltCTest_Basic2() u32;
test "jolt_c.basic2" {
    const ret = JoltCTest_Basic2();
    try expect(ret != 0);
}

extern fn JoltCTest_HelloWorld() u32;
test "jolt_c.helloworld" {
    const ret = JoltCTest_HelloWorld();
    try expect(ret != 0);
}

//test "JoltC.basic" {
//    if (@import("builtin").target.os.tag == .macos and
//        @import("builtin").target.cpu.arch == .aarch64) return error.SkipZigTest;
//
//    c.JPH_RegisterDefaultAllocator();
//    c.JPH_CreateFactory();
//    defer c.JPH_DestroyFactory();
//    c.JPH_RegisterTypes();
//    const physics_system = c.JPH_PhysicsSystem_Create();
//    defer c.JPH_PhysicsSystem_Destroy(physics_system);
//
//    const max_bodies: u32 = 1024;
//    const num_body_mutexes: u32 = 0;
//    const max_body_pairs: u32 = 1024;
//    const max_contact_constraints: u32 = 1024;
//
//    const broad_phase_layer_interface = BPLayerInterfaceImpl.init();
//
//    c.JPH_PhysicsSystem_Init(
//        physics_system,
//        max_bodies,
//        num_body_mutexes,
//        max_body_pairs,
//        max_contact_constraints,
//        &broad_phase_layer_interface,
//        myBroadPhaseCanCollide,
//        myObjectCanCollide,
//    );
//
//    try expect(c.JPH_PhysicsSystem_GetNumBodies(physics_system) == 0);
//    try expect(c.JPH_PhysicsSystem_GetNumActiveBodies(physics_system) == 0);
//}
