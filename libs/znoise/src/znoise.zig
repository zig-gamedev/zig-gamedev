// znoise - Zig bindings for FastNoiseLite

pub const FnlGenerator = extern struct {
    seed: i32 = 1337,
    frequency: f32 = 0.01,
    noise_type: NoiseType = .opensimplex2,
    rotation_type3: RotationType3 = .none,
    fractal_type: FractalType = .none,
    octaves: i32 = 3,
    lacunarity: f32 = 2.0,
    gain: f32 = 0.5,
    weighted_strength: f32 = 0.0,
    ping_pong_strength: f32 = 2.0,
    cellular_distance_func: CellularDistanceFunc = .euclideansq,
    cellular_return_type: CellularReturnType = .distance,
    cellular_jitter_mod: f32 = 1.0,
    domain_warp_type: DomainWarpType = .opensimplex2,
    domain_warp_amp: f32 = 1.0,

    pub const NoiseType = enum(c_int) {
        opensimplex2,
        opensimplex2s,
        cellular,
        perlin,
        value_cubic,
        value,
    };

    pub const RotationType3 = enum(c_int) {
        none,
        improve_xy_planes,
        improve_xz_planes,
    };

    pub const FractalType = enum(c_int) {
        none,
        fbm,
        ridged,
        pingpong,
        domain_warp_progressive,
        domain_warp_independent,
    };

    pub const CellularDistanceFunc = enum(c_int) {
        euclidean,
        euclideansq,
        manhattan,
        hybrid,
    };

    pub const CellularReturnType = enum(c_int) {
        cellvalue,
        distance,
        distance2,
        distance2add,
        distance2sub,
        distance2mul,
        distance2div,
    };

    pub const DomainWarpType = enum(c_int) {
        opensimplex2,
        opensimplex2_reduced,
        basicgrid,
    };

    pub const noise2 = fnlGetNoise2D;
    extern fn fnlGetNoise2D(gen: *const FnlGenerator, x: f32, y: f32) f32;

    pub const noise3 = fnlGetNoise3D;
    extern fn fnlGetNoise3D(gen: *const FnlGenerator, x: f32, y: f32, z: f32) f32;

    pub const domainWarp2 = fnlDomainWarp2D;
    extern fn fnlDomainWarp2D(gen: *const FnlGenerator, x: *f32, y: *f32) void;

    pub const domainWarp3 = fnlDomainWarp3D;
    extern fn fnlDomainWarp3D(gen: *const FnlGenerator, x: *f32, y: *f32, z: *f32) void;
};

test "znoise.basic" {
    const gen = FnlGenerator{ .fractal_type = .fbm };
    const n2 = gen.noise2(0.1, 0.2);
    const n3 = gen.noise3(1.0, 2.0, 3.0);
    _ = n2;
    _ = n3;
}
