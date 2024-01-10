const std = @import("std");

pub const Options = struct {
    use_double_precision: bool = false,
    enable_asserts: bool = false,
    enable_cross_platform_determinism: bool = true,
    enable_debug_renderer: bool = false,
};

pub const Package = struct {
    options: Options,
    zphysics: *std.Build.Module,
    zphysics_options: *std.Build.Module,
    zphysics_c_cpp: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addIncludePath(.{ .path = thisDir() ++ "/libs/JoltC" });
        exe.root_module.linkLibrary(pkg.zphysics_c_cpp);
        exe.root_module.addImport("zphysics", pkg.zphysics);
        exe.root_module.addImport("zphysics_options", pkg.zphysics_options);
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "use_double_precision", args.options.use_double_precision);
    step.addOption(bool, "enable_asserts", args.options.enable_asserts);
    step.addOption(
        bool,
        "enable_cross_platform_determinism",
        args.options.enable_cross_platform_determinism,
    );
    step.addOption(bool, "enable_debug_renderer", args.options.enable_debug_renderer);

    const zphysics_options = step.createModule();

    const zphysics = b.addModule("zphysics", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zphysics.zig" },
        .imports = &.{
            .{ .name = "zphysics_options", .module = zphysics_options },
        },
    });

    // Below necessary to avoid missing cInclude errors in project `build.zig`
    zphysics.addIncludePath(.{ .path = thisDir() ++ "/libs" });
    zphysics.addIncludePath(.{ .path = thisDir() ++ "/libs/JoltC" });

    const zphysics_c_cpp = b.addStaticLibrary(.{
        .name = "zphysics",
        .target = target,
        .optimize = optimize,
    });

    zphysics_c_cpp.root_module.addIncludePath(.{ .path = thisDir() ++ "/libs" });
    zphysics_c_cpp.root_module.addIncludePath(.{ .path = thisDir() ++ "/libs/JoltC" });
    zphysics_c_cpp.root_module.link_libc = true;
    if (target.result.abi != .msvc)
        zphysics_c_cpp.linkLibCpp();

    const flags = &.{
        "-std=c++17",
        if (target.result.abi != .msvc) "-DJPH_COMPILER_MINGW" else "",
        if (args.options.enable_cross_platform_determinism) "-DJPH_CROSS_PLATFORM_DETERMINISTIC" else "",
        if (args.options.enable_debug_renderer) "-DJPH_DEBUG_RENDERER" else "",
        if (args.options.use_double_precision) "-DJPH_DOUBLE_PRECISION" else "",
        if (args.options.enable_asserts or optimize == .Debug) "-DJPH_ENABLE_ASSERTS" else "",
        "-fno-access-control",
        "-fno-sanitize=undefined",
    };

    const src_dir = thisDir() ++ "/libs/Jolt";
    zphysics_c_cpp.root_module.addCSourceFiles(.{
        .files = &.{
            thisDir() ++ "/libs/JoltC/JoltPhysicsC.cpp",
            thisDir() ++ "/libs/JoltC/JoltPhysicsC_Extensions.cpp",
            src_dir ++ "/AABBTree/AABBTreeBuilder.cpp",
            src_dir ++ "/Core/Color.cpp",
            src_dir ++ "/Core/Factory.cpp",
            src_dir ++ "/Core/IssueReporting.cpp",
            src_dir ++ "/Core/JobSystemThreadPool.cpp",
            src_dir ++ "/Core/JobSystemWithBarrier.cpp",
            src_dir ++ "/Core/LinearCurve.cpp",
            src_dir ++ "/Core/Memory.cpp",
            src_dir ++ "/Core/Profiler.cpp",
            src_dir ++ "/Core/RTTI.cpp",
            src_dir ++ "/Core/Semaphore.cpp",
            src_dir ++ "/Core/StringTools.cpp",
            src_dir ++ "/Core/TickCounter.cpp",
            src_dir ++ "/Geometry/ConvexHullBuilder.cpp",
            src_dir ++ "/Geometry/ConvexHullBuilder2D.cpp",
            src_dir ++ "/Geometry/Indexify.cpp",
            src_dir ++ "/Geometry/OrientedBox.cpp",
            src_dir ++ "/Math/UVec4.cpp",
            src_dir ++ "/Math/Vec3.cpp",
            src_dir ++ "/ObjectStream/ObjectStream.cpp",
            src_dir ++ "/ObjectStream/ObjectStreamBinaryIn.cpp",
            src_dir ++ "/ObjectStream/ObjectStreamBinaryOut.cpp",
            src_dir ++ "/ObjectStream/ObjectStreamIn.cpp",
            src_dir ++ "/ObjectStream/ObjectStreamOut.cpp",
            src_dir ++ "/ObjectStream/ObjectStreamTextIn.cpp",
            src_dir ++ "/ObjectStream/ObjectStreamTextOut.cpp",
            src_dir ++ "/ObjectStream/SerializableObject.cpp",
            src_dir ++ "/ObjectStream/TypeDeclarations.cpp",
            src_dir ++ "/Physics/Body/Body.cpp",
            src_dir ++ "/Physics/Body/BodyAccess.cpp",
            src_dir ++ "/Physics/Body/BodyCreationSettings.cpp",
            src_dir ++ "/Physics/Body/BodyInterface.cpp",
            src_dir ++ "/Physics/Body/BodyManager.cpp",
            src_dir ++ "/Physics/Body/MassProperties.cpp",
            src_dir ++ "/Physics/Body/MotionProperties.cpp",
            src_dir ++ "/Physics/Character/Character.cpp",
            src_dir ++ "/Physics/Character/CharacterBase.cpp",
            src_dir ++ "/Physics/Character/CharacterVirtual.cpp",
            src_dir ++ "/Physics/Collision/BroadPhase/BroadPhase.cpp",
            src_dir ++ "/Physics/Collision/BroadPhase/BroadPhaseBruteForce.cpp",
            src_dir ++ "/Physics/Collision/BroadPhase/BroadPhaseQuadTree.cpp",
            src_dir ++ "/Physics/Collision/BroadPhase/QuadTree.cpp",
            src_dir ++ "/Physics/Collision/CastConvexVsTriangles.cpp",
            src_dir ++ "/Physics/Collision/CastSphereVsTriangles.cpp",
            src_dir ++ "/Physics/Collision/CollideConvexVsTriangles.cpp",
            src_dir ++ "/Physics/Collision/CollideSphereVsTriangles.cpp",
            src_dir ++ "/Physics/Collision/CollisionDispatch.cpp",
            src_dir ++ "/Physics/Collision/CollisionGroup.cpp",
            src_dir ++ "/Physics/Collision/GroupFilter.cpp",
            src_dir ++ "/Physics/Collision/GroupFilterTable.cpp",
            src_dir ++ "/Physics/Collision/ManifoldBetweenTwoFaces.cpp",
            src_dir ++ "/Physics/Collision/NarrowPhaseQuery.cpp",
            src_dir ++ "/Physics/Collision/NarrowPhaseStats.cpp",
            src_dir ++ "/Physics/Collision/PhysicsMaterial.cpp",
            src_dir ++ "/Physics/Collision/PhysicsMaterialSimple.cpp",
            src_dir ++ "/Physics/Collision/Shape/BoxShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/CapsuleShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/CompoundShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/ConvexHullShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/ConvexShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/CylinderShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/DecoratedShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/HeightFieldShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/MeshShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/MutableCompoundShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/OffsetCenterOfMassShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/RotatedTranslatedShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/ScaledShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/Shape.cpp",
            src_dir ++ "/Physics/Collision/Shape/SphereShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/StaticCompoundShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/TaperedCapsuleShape.cpp",
            src_dir ++ "/Physics/Collision/Shape/TriangleShape.cpp",
            src_dir ++ "/Physics/Collision/TransformedShape.cpp",
            src_dir ++ "/Physics/Constraints/ConeConstraint.cpp",
            src_dir ++ "/Physics/Constraints/Constraint.cpp",
            src_dir ++ "/Physics/Constraints/ConstraintManager.cpp",
            src_dir ++ "/Physics/Constraints/ContactConstraintManager.cpp",
            src_dir ++ "/Physics/Constraints/DistanceConstraint.cpp",
            src_dir ++ "/Physics/Constraints/FixedConstraint.cpp",
            src_dir ++ "/Physics/Constraints/GearConstraint.cpp",
            src_dir ++ "/Physics/Constraints/HingeConstraint.cpp",
            src_dir ++ "/Physics/Constraints/MotorSettings.cpp",
            src_dir ++ "/Physics/Constraints/PathConstraint.cpp",
            src_dir ++ "/Physics/Constraints/PathConstraintPath.cpp",
            src_dir ++ "/Physics/Constraints/PathConstraintPathHermite.cpp",
            src_dir ++ "/Physics/Constraints/PointConstraint.cpp",
            src_dir ++ "/Physics/Constraints/RackAndPinionConstraint.cpp",
            src_dir ++ "/Physics/Constraints/SixDOFConstraint.cpp",
            src_dir ++ "/Physics/Constraints/SliderConstraint.cpp",
            src_dir ++ "/Physics/Constraints/SwingTwistConstraint.cpp",
            src_dir ++ "/Physics/Constraints/TwoBodyConstraint.cpp",
            src_dir ++ "/Physics/Constraints/PulleyConstraint.cpp",
            src_dir ++ "/Physics/DeterminismLog.cpp",
            src_dir ++ "/Physics/IslandBuilder.cpp",
            src_dir ++ "/Physics/LargeIslandSplitter.cpp",
            src_dir ++ "/Physics/PhysicsScene.cpp",
            src_dir ++ "/Physics/PhysicsSystem.cpp",
            src_dir ++ "/Physics/PhysicsUpdateContext.cpp",
            src_dir ++ "/Physics/PhysicsLock.cpp",
            src_dir ++ "/Physics/Ragdoll/Ragdoll.cpp",
            src_dir ++ "/Physics/StateRecorderImpl.cpp",
            src_dir ++ "/Physics/Vehicle/TrackedVehicleController.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleAntiRollBar.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleCollisionTester.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleConstraint.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleController.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleDifferential.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleEngine.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleTrack.cpp",
            src_dir ++ "/Physics/Vehicle/VehicleTransmission.cpp",
            src_dir ++ "/Physics/Vehicle/Wheel.cpp",
            src_dir ++ "/Physics/Vehicle/WheeledVehicleController.cpp",
            src_dir ++ "/Physics/Vehicle/MotorcycleController.cpp",
            src_dir ++ "/RegisterTypes.cpp",
            src_dir ++ "/Renderer/DebugRenderer.cpp",
            src_dir ++ "/Renderer/DebugRendererPlayback.cpp",
            src_dir ++ "/Renderer/DebugRendererRecorder.cpp",
            src_dir ++ "/Skeleton/SkeletalAnimation.cpp",
            src_dir ++ "/Skeleton/Skeleton.cpp",
            src_dir ++ "/Skeleton/SkeletonMapper.cpp",
            src_dir ++ "/Skeleton/SkeletonPose.cpp",
            src_dir ++ "/TriangleGrouper/TriangleGrouperClosestCentroid.cpp",
            src_dir ++ "/TriangleGrouper/TriangleGrouperMorton.cpp",
            src_dir ++ "/TriangleSplitter/TriangleSplitter.cpp",
            src_dir ++ "/TriangleSplitter/TriangleSplitterBinning.cpp",
            src_dir ++ "/TriangleSplitter/TriangleSplitterFixedLeafSize.cpp",
            src_dir ++ "/TriangleSplitter/TriangleSplitterLongestAxis.cpp",
            src_dir ++ "/TriangleSplitter/TriangleSplitterMean.cpp",
            src_dir ++ "/TriangleSplitter/TriangleSplitterMorton.cpp",
        },
        .flags = flags,
    });

    return .{
        .options = args.options,
        .zphysics = zphysics,
        .zphysics_options = zphysics_options,
        .zphysics_c_cpp = zphysics_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zphysics tests");
    test_step.dependOn(runTests(b, optimize, target));

    _ = package(b, target, optimize, .{
        .options = .{
            .use_double_precision = b.option(bool, "use_double_precision", "Enable double precision") orelse false,
            .enable_asserts = b.option(bool, "enable_asserts", "Enable assertions") orelse false,
            .enable_cross_platform_determinism = b.option(
                bool,
                "enable_cross_platform_determinism",
                "Enables cross-platform determinism",
            ) orelse true,
            .enable_debug_renderer = b.option(bool, "enable_debug_renderer", "Enable debug renderer") orelse false,
        },
    });
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const parent_step = b.allocator.create(std.Build.Step) catch @panic("OOM");
    parent_step.* = std.Build.Step.init(.{ .id = .custom, .name = "zphysics-tests", .owner = b });

    const test0 = testStep(b, "zphysics-tests-f32", optimize, target, .{
        .use_double_precision = false,
        .enable_debug_renderer = true,
    });
    parent_step.dependOn(&test0.step);

    // const test1 = testStep(b, "zphysics-tests-f64", optimize, target, .{
    //     .use_double_precision = true,
    //     .enable_debug_renderer = true,
    // });
    // parent_step.dependOn(&test1.step);

    return parent_step;
}

fn testStep(
    b: *std.Build,
    name: []const u8,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
    options: Options,
) *std.Build.Step.Run {
    const test_exe = b.addTest(.{
        .name = name,
        .root_source_file = .{ .path = thisDir() ++ "/src/zphysics.zig" },
        .target = target,
        .optimize = optimize,
    });

    // TODO: Problems with LTO on Windows.
    test_exe.want_lto = false;

    test_exe.root_module.addCSourceFile(.{
        .file = .{ .path = thisDir() ++ "/libs/JoltC/JoltPhysicsC_Tests.c" },
        .flags = &.{
            "-std=c11",
            if (target.result.abi != .msvc) "-DJPH_COMPILER_MINGW" else "",
            if (options.use_double_precision) "-DJPH_DOUBLE_PRECISION" else "",
            if (options.enable_asserts or optimize == .Debug) "-DJPH_ENABLE_ASSERTS" else "",
            if (options.enable_cross_platform_determinism) "-DJPH_CROSS_PLATFORM_DETERMINISTIC" else "",
            if (options.enable_debug_renderer) "-DJPH_DEBUG_RENDERER" else "",
            "-fno-sanitize=undefined",
        },
    });

    const zphysics_pkg = package(b, target, optimize, .{ .options = options });
    zphysics_pkg.link(test_exe);

    test_exe.root_module.addImport("zphysics_options", zphysics_pkg.zphysics_options);

    return b.addRunArtifact(test_exe);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
