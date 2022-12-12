const std = @import("std");

pub const BuildOptions = struct {
    use_double_precision: bool = false,
    enable_asserts: bool = false,
};

pub const BuildOptionsStep = struct {
    options: BuildOptions,
    step: *std.build.OptionsStep,

    pub fn init(b: *std.build.Builder, options: BuildOptions) BuildOptionsStep {
        const bos = .{
            .options = options,
            .step = b.addOptions(),
        };
        bos.step.addOption(bool, "use_double_precision", bos.options.use_double_precision);
        bos.step.addOption(bool, "enable_asserts", bos.options.enable_asserts);
        return bos;
    }

    pub fn getPkg(bos: BuildOptionsStep) std.build.Pkg {
        return bos.step.getPackage("zphysics_options");
    }

    fn addTo(bos: BuildOptionsStep, target_step: *std.build.LibExeObjStep) void {
        target_step.addOptions("zphysics_options", bos.step);
    }
};

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zphysics",
        .source = .{ .path = thisDir() ++ "/src/zphysics.zig" },
        .dependencies = dependencies,
    };
}

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target, .{});

    const test_step = b.step("test", "Run zphysics tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
    options: BuildOptions,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zphysics.zig");
    tests.addCSourceFile(
        thisDir() ++ "/libs/JoltC/JoltPhysicsC_Tests.c",
        &.{
            "-std=c11",
            if (options.use_double_precision) "-DJPH_DOUBLE_PRECISION" else "",
            if (options.enable_asserts or tests.build_mode == .Debug) "-DJPH_ENABLE_ASSERTS" else "",
        },
    );
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests, BuildOptionsStep.init(b, options));
    return tests;
}

pub fn link(exe: *std.build.LibExeObjStep, bos: BuildOptionsStep) void {
    bos.addTo(exe);

    exe.addIncludePath(thisDir() ++ "/libs");
    exe.addIncludePath(thisDir() ++ "/libs/JoltC");
    exe.linkSystemLibraryName("c");
    exe.linkSystemLibraryName("c++");

    const flags = &.{
        "-std=c++17",
        "-DJPH_COMPILER_MINGW",
        if (bos.options.use_double_precision) "-DJPH_DOUBLE_PRECISION" else "",
        if (bos.options.enable_asserts or exe.build_mode == .Debug) "-DJPH_ENABLE_ASSERTS" else "",
        "-fno-sanitize=undefined",
    };
    exe.addCSourceFile(thisDir() ++ "/libs/JoltC/JoltPhysicsC.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/JoltC/JoltPhysicsC_Extensions.cpp", flags);

    const src_dir = thisDir() ++ "/libs/Jolt";
    exe.addCSourceFile(src_dir ++ "/AABBTree/AABBTreeBuilder.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/Color.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/Factory.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/IssueReporting.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/JobSystemThreadPool.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/LinearCurve.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/Memory.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/Profiler.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/RTTI.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/StringTools.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Core/TickCounter.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Geometry/ConvexHullBuilder.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Geometry/ConvexHullBuilder2D.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Geometry/Indexify.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Geometry/OrientedBox.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Math/UVec4.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Math/Vec3.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/ObjectStream.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/ObjectStreamBinaryIn.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/ObjectStreamBinaryOut.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/ObjectStreamIn.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/ObjectStreamOut.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/ObjectStreamTextIn.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/ObjectStreamTextOut.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/SerializableObject.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/ObjectStream/TypeDeclarations.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Body/Body.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Body/BodyAccess.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Body/BodyCreationSettings.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Body/BodyInterface.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Body/BodyManager.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Body/MassProperties.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Body/MotionProperties.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Character/Character.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Character/CharacterBase.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Character/CharacterVirtual.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/BroadPhase/BroadPhase.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/BroadPhase/BroadPhaseBruteForce.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/BroadPhase/BroadPhaseQuadTree.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/BroadPhase/QuadTree.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/CastConvexVsTriangles.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/CastSphereVsTriangles.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/CollideConvexVsTriangles.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/CollideSphereVsTriangles.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/CollisionDispatch.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/CollisionGroup.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/GroupFilter.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/GroupFilterTable.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/ManifoldBetweenTwoFaces.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/NarrowPhaseQuery.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/NarrowPhaseStats.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/PhysicsMaterial.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/PhysicsMaterialSimple.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/BoxShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/CapsuleShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/CompoundShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/ConvexHullShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/ConvexShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/CylinderShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/DecoratedShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/HeightFieldShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/MeshShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/MutableCompoundShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/OffsetCenterOfMassShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/RotatedTranslatedShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/ScaledShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/Shape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/SphereShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/StaticCompoundShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/TaperedCapsuleShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/Shape/TriangleShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Collision/TransformedShape.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/ConeConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/Constraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/ConstraintManager.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/ContactConstraintManager.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/DistanceConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/FixedConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/GearConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/HingeConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/MotorSettings.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/PathConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/PathConstraintPath.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/PathConstraintPathHermite.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/PointConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/RackAndPinionConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/SixDOFConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/SliderConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/SwingTwistConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/TwoBodyConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Constraints/PulleyConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/DeterminismLog.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/IslandBuilder.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/PhysicsLock.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/PhysicsScene.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/PhysicsSystem.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/PhysicsUpdateContext.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Ragdoll/Ragdoll.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/StateRecorderImpl.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/TrackedVehicleController.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleAntiRollBar.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleCollisionTester.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleConstraint.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleController.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleDifferential.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleEngine.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleTrack.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/VehicleTransmission.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/Wheel.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Physics/Vehicle/WheeledVehicleController.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/RegisterTypes.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Renderer/DebugRenderer.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Renderer/DebugRendererPlayback.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Renderer/DebugRendererRecorder.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Skeleton/SkeletalAnimation.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Skeleton/Skeleton.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Skeleton/SkeletonMapper.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/Skeleton/SkeletonPose.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleGrouper/TriangleGrouperClosestCentroid.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleGrouper/TriangleGrouperMorton.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleSplitter/TriangleSplitter.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleSplitter/TriangleSplitterBinning.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleSplitter/TriangleSplitterFixedLeafSize.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleSplitter/TriangleSplitterLongestAxis.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleSplitter/TriangleSplitterMean.cpp", flags);
    exe.addCSourceFile(src_dir ++ "/TriangleSplitter/TriangleSplitterMorton.cpp", flags);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
