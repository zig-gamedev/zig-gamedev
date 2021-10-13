pub const DynamicsWorldHandle = *opaque {};
pub const Vector3 = [3]f32;
pub const Quaternion = [4]f32;

extern fn cbtCreateDynamicsWorld() DynamicsWorldHandle;
extern fn cbtDeleteDynamicsWorld(handle: DynamicsWorldHandle) void;

pub const createDynamicsWorld = cbtCreateDynamicsWorld;
pub const deleteDynamicsWorld = cbtDeleteDynamicsWorld;
