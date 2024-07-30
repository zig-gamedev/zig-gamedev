const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const options = .{
        .use_double_precision = b.option(
            bool,
            "use_double_precision",
            "Enable double precision",
        ) orelse false,
        .enable_asserts = b.option(
            bool,
            "enable_asserts",
            "Enable assertions",
        ) orelse (optimize == .Debug),
        .enable_cross_platform_determinism = b.option(
            bool,
            "enable_cross_platform_determinism",
            "Enables cross-platform determinism",
        ) orelse true,
        .enable_debug_renderer = b.option(
            bool,
            "enable_debug_renderer",
            "Enable debug renderer",
        ) orelse false,
        .shared = b.option(
            bool,
            "shared",
            "Build JoltC as shared lib",
        ) orelse false,
    };

    const options_step = b.addOptions();
    inline for (std.meta.fields(@TypeOf(options))) |field| {
        options_step.addOption(field.type, field.name, @field(options, field.name));
    }

    const options_module = options_step.createModule();

    const zjolt = b.addModule("root", .{
        .root_source_file = b.path("src/zphysics.zig"),
        .imports = &.{
            .{ .name = "zphysics_options", .module = options_module },
        },
    });
    zjolt.addIncludePath(b.path("libs/JoltC"));

    const joltc = if (options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "joltc",
            .target = target,
            .optimize = optimize,
        });
        if (target.result.os.tag == .windows) {
            lib.defineCMacro("JPC_API", "extern __declspec(dllexport)");
        }
        break :blk lib;
    } else b.addStaticLibrary(.{
        .name = "joltc",
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(joltc);

    joltc.addIncludePath(b.path("libs"));
    joltc.addIncludePath(b.path("libs/JoltC"));
    joltc.linkLibC();
    if (target.result.abi != .msvc)
        joltc.linkLibCpp();

    const src_dir = "libs/Jolt";
    joltc.addCSourceFiles(.{
        .files = &.{
            "libs/JoltC/JoltPhysicsC.cpp",
            "libs/JoltC/JoltPhysicsC_Extensions.cpp",
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
        .flags = &.{
            "-std=c++17",
            if (@import("builtin").abi != .msvc) "-DJPH_COMPILER_MINGW" else "",
            if (options.enable_cross_platform_determinism) "-DJPH_CROSS_PLATFORM_DETERMINISTIC" else "",
            if (options.enable_debug_renderer) "-DJPH_DEBUG_RENDERER" else "",
            if (options.use_double_precision) "-DJPH_DOUBLE_PRECISION" else "",
            if (options.enable_asserts) "-DJPH_ENABLE_ASSERTS" else "",
            "-fno-access-control",
            "-fno-sanitize=undefined",
        },
    });

    const test_step = b.step("test", "Run zphysics tests");

    const tests = b.addTest(.{
        .name = "zphysics-tests",
        .root_source_file = b.path("src/zphysics.zig"),
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    // TODO: Problems with LTO on Windows.
    if (target.result.os.tag == .windows) {
        tests.want_lto = false;
    }

    tests.addCSourceFile(.{
        .file = b.path("libs/JoltC/JoltPhysicsC_Tests.c"),
        .flags = &.{
            if (@import("builtin").abi != .msvc) "-DJPH_COMPILER_MINGW" else "",
            if (options.enable_cross_platform_determinism) "-DJPH_CROSS_PLATFORM_DETERMINISTIC" else "",
            if (options.enable_debug_renderer) "-DJPH_DEBUG_RENDERER" else "",
            if (options.use_double_precision) "-DJPH_DOUBLE_PRECISION" else "",
            if (options.enable_asserts) "-DJPH_ENABLE_ASSERTS" else "",
            "-fno-sanitize=undefined",
        },
    });

    tests.root_module.addImport("zphysics_options", options_module);
    tests.addIncludePath(b.path("libs/JoltC"));
    tests.linkLibrary(joltc);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
