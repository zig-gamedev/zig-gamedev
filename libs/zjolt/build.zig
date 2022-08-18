const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zjolt",
    .source = .{ .path = thisDir() ++ "/src/zjolt.zig" },
};

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zjolt tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zjolt.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zjolt", thisDir() ++ "/src/zjolt.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.addIncludeDir(thisDir() ++ "/libs");
    lib.addIncludeDir(thisDir() ++ "/libs/JoltC");
    lib.linkSystemLibrary("c");
    lib.linkSystemLibrary("c++");

    const root = thisDir() ++ "/libs/Jolt";
    const flags = &.{
        "-std=c++17",
        "-DJPH_COMPILER_MINGW",
        "-fno-sanitize=undefined",
    };
    lib.addIncludeDir(root ++ "/Math");

    lib.addCSourceFile(thisDir() ++ "/libs/JoltC/JoltC.cpp", flags);
    lib.addCSourceFile(root ++ "/AABBTree/AABBTreeBuilder.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/Color.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/Factory.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/IssueReporting.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/JobSystemThreadPool.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/LinearCurve.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/Memory.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/Profiler.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/RTTI.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/StringTools.cpp", flags);
    lib.addCSourceFile(root ++ "/Core/TickCounter.cpp", flags);
    lib.addCSourceFile(root ++ "/Geometry/ConvexHullBuilder.cpp", flags);
    lib.addCSourceFile(root ++ "/Geometry/ConvexHullBuilder2D.cpp", flags);
    lib.addCSourceFile(root ++ "/Geometry/Indexify.cpp", flags);
    lib.addCSourceFile(root ++ "/Geometry/OrientedBox.cpp", flags);
    lib.addCSourceFile(root ++ "/Math/UVec4.cpp", flags);
    lib.addCSourceFile(root ++ "/Math/Vec3.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/ObjectStream.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/ObjectStreamBinaryIn.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/ObjectStreamBinaryOut.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/ObjectStreamIn.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/ObjectStreamOut.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/ObjectStreamTextIn.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/ObjectStreamTextOut.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/SerializableObject.cpp", flags);
    lib.addCSourceFile(root ++ "/ObjectStream/TypeDeclarations.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Body/Body.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Body/BodyAccess.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Body/BodyCreationSettings.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Body/BodyInterface.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Body/BodyManager.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Body/MassProperties.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Body/MotionProperties.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Character/Character.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Character/CharacterBase.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Character/CharacterVirtual.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/BroadPhase/BroadPhase.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/BroadPhase/BroadPhaseBruteForce.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/BroadPhase/BroadPhaseQuadTree.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/BroadPhase/QuadTree.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/CastConvexVsTriangles.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/CastSphereVsTriangles.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/CollideConvexVsTriangles.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/CollideSphereVsTriangles.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/CollisionDispatch.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/CollisionGroup.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/GroupFilter.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/GroupFilterTable.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/ManifoldBetweenTwoFaces.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/NarrowPhaseQuery.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/NarrowPhaseStats.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/PhysicsMaterial.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/PhysicsMaterialSimple.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/BoxShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/CapsuleShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/CompoundShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/ConvexHullShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/ConvexShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/CylinderShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/DecoratedShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/HeightFieldShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/MeshShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/MutableCompoundShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/OffsetCenterOfMassShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/RotatedTranslatedShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/ScaledShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/Shape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/SphereShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/StaticCompoundShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/TaperedCapsuleShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/Shape/TriangleShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Collision/TransformedShape.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/ConeConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/Constraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/ConstraintManager.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/ContactConstraintManager.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/DistanceConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/FixedConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/GearConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/HingeConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/MotorSettings.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/PathConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/PathConstraintPath.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/PathConstraintPathHermite.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/PointConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/RackAndPinionConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/SixDOFConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/SliderConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/SwingTwistConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Constraints/TwoBodyConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/DeterminismLog.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/IslandBuilder.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/PhysicsLock.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/PhysicsScene.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/PhysicsSystem.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/PhysicsUpdateContext.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Ragdoll/Ragdoll.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/StateRecorderImpl.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/TrackedVehicleController.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleAntiRollBar.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleCollisionTester.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleConstraint.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleController.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleDifferential.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleEngine.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleTrack.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/VehicleTransmission.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/Wheel.cpp", flags);
    lib.addCSourceFile(root ++ "/Physics/Vehicle/WheeledVehicleController.cpp", flags);
    lib.addCSourceFile(root ++ "/RegisterTypes.cpp", flags);
    lib.addCSourceFile(root ++ "/Renderer/DebugRenderer.cpp", flags);
    lib.addCSourceFile(root ++ "/Renderer/DebugRendererPlayback.cpp", flags);
    lib.addCSourceFile(root ++ "/Renderer/DebugRendererRecorder.cpp", flags);
    lib.addCSourceFile(root ++ "/Skeleton/SkeletalAnimation.cpp", flags);
    lib.addCSourceFile(root ++ "/Skeleton/Skeleton.cpp", flags);
    lib.addCSourceFile(root ++ "/Skeleton/SkeletonMapper.cpp", flags);
    lib.addCSourceFile(root ++ "/Skeleton/SkeletonPose.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleGrouper/TriangleGrouperClosestCentroid.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleGrouper/TriangleGrouperMorton.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleSplitter/TriangleSplitter.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleSplitter/TriangleSplitterBinning.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleSplitter/TriangleSplitterFixedLeafSize.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleSplitter/TriangleSplitterLongestAxis.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleSplitter/TriangleSplitterMean.cpp", flags);
    lib.addCSourceFile(root ++ "/TriangleSplitter/TriangleSplitterMorton.cpp", flags);

    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
    exe.addIncludeDir(thisDir() ++ "/libs/JoltC");
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
