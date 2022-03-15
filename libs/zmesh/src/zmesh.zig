// zmesh - Zig bindings for par_shapes

pub const Error = error{OutOfMemory};
pub const IndexType = u16;
pub const MeshHandle = *opaque {};

const ParMesh = extern struct {
    points: [*]f32,
    npoints: c_int,
    triangles: [*]IndexType,
    ntriangles: c_int,
    normals: ?[*]f32,
    tcoords: ?[*]f32,
};

pub const Mesh = struct {
    handle: MeshHandle,
    vertices: []f32,
    indices: []IndexType,
    normals: ?[]f32,
    texcoords: ?[]f32,

    fn saveToObj(mesh: Mesh, filename: [*:0]const u8) void {
        par_shapes_export(mesh.handle, filename);
    }
    extern fn par_shapes_export(mesh: MeshHandle, filename: [*:0]const u8) void;
};

fn createMesh(parmesh: *ParMesh) Mesh {
    return .{
        .handle = @ptrCast(MeshHandle, parmesh),
        .vertices = parmesh.points[0..@intCast(usize, parmesh.npoints * 3)],
        .indices = parmesh.triangles[0..@intCast(usize, parmesh.ntriangles * 3)],
        .normals = if (parmesh.normals == null)
            null
        else
            parmesh.normals.?[0..@intCast(usize, parmesh.npoints * 3)],
        .texcoords = if (parmesh.tcoords == null)
            null
        else
            parmesh.tcoords.?[0..@intCast(usize, parmesh.npoints * 2)],
    };
}

pub fn createCylinder(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_cylinder(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return createMesh(parmesh.?);
}
extern fn par_shapes_create_cylinder(slices: i32, stacks: i32) ?*ParMesh;

pub fn createCone(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_cone(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return createMesh(parmesh.?);
}
extern fn par_shapes_create_cone(slices: i32, stacks: i32) ?*ParMesh;

pub fn createParametricDisk(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_parametric_disk(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return createMesh(parmesh.?);
}
extern fn par_shapes_create_parametric_disk(slices: i32, stacks: i32) ?*ParMesh;

pub fn createTorus(slices: i32, stacks: i32, radius: f32) Error!Mesh {
    const parmesh = par_shapes_create_torus(slices, stacks, radius);
    if (parmesh == null)
        return error.OutOfMemory;
    return createMesh(parmesh.?);
}
extern fn par_shapes_create_torus(slices: i32, stacks: i32, radius: f32) ?*ParMesh;

pub fn createParametricSphere(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_parametric_sphere(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return createMesh(parmesh.?);
}
extern fn par_shapes_create_parametric_sphere(slices: i32, stacks: i32) ?*ParMesh;

pub fn createSubdividedSphere(num_subdivisions: i32) Error!Mesh {
    const parmesh = par_shapes_create_subdivided_sphere(num_subdivisions);
    if (parmesh == null)
        return error.OutOfMemory;
    return createMesh(parmesh.?);
}
extern fn par_shapes_create_subdivided_sphere(num_subdivisions: i32) ?*ParMesh;

test "zmesh.basic" {
    const cylinder = try createCylinder(10, 10);
    //cylinder.saveToObj("zmesh.cylinder.obj");
    _ = cylinder;

    const cone = try createCone(10, 10);
    //cone.saveToObj("zmesh.cone.obj");
    _ = cone;

    const disk = try createParametricDisk(10, 10);
    //disk.saveToObj("zmesh.disk.obj");
    _ = disk;

    const torus = try createTorus(10, 10, 0.2);
    //torus.saveToObj("zmesh.torus.obj");
    _ = torus;

    const psphere = try createParametricSphere(10, 10);
    //psphere.saveToObj("zmesh.psphere.obj");
    _ = psphere;

    const subdsphere = try createSubdividedSphere(3);
    //subdsphere.saveToObj("zmesh.subdsphere.obj");
    _ = subdsphere;
}
