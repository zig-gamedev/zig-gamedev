// JoltPhysicsC v0.0.6 - C API for Jolt Physics C++ library

#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>
#include <float.h>
//--------------------------------------------------------------------------------------------------
//
// Const
//
//--------------------------------------------------------------------------------------------------
#ifndef JPC_API
#define JPC_API
#endif

// Always turn on asserts in Debug mode
#if defined(_DEBUG) || defined(JPH_ENABLE_ASSERTS)
    #define JPC_ENABLE_ASSERTS 1
#else
    #define JPC_ENABLE_ASSERTS 0
#endif

#if defined(JPH_DOUBLE_PRECISION)
    #define JPC_DOUBLE_PRECISION 1
#else
    #define JPC_DOUBLE_PRECISION 0
#endif

#if JPC_DOUBLE_PRECISION == 1
typedef double JPC_Real;
#define JPC_RVEC_ALIGN alignas(32)
#else
typedef float JPC_Real;
#define JPC_RVEC_ALIGN alignas(16)
#endif

#if defined(JPH_DEBUG_RENDERER)
    #define JPC_DEBUG_RENDERER 1
#else
    #define JPC_DEBUG_RENDERER 0
#endif

#define JPC_PI 3.14159265358979323846f

#define JPC_COLLISION_GROUP_INVALID_GROUP 0xffffffff
#define JPC_COLLISION_GROUP_INVALID_SUB_GROUP 0xffffffff

#define JPC_BODY_ID_INVALID 0xffffffff
#define JPC_BODY_ID_INDEX_BITS 0x007fffff
#define JPC_BODY_ID_SEQUENCE_BITS 0xff000000
#define JPC_BODY_ID_SEQUENCE_SHIFT 24

#define JPC_SUB_SHAPE_ID_EMPTY 0xffffffff

#define JPC_FLT_EPSILON FLT_EPSILON

#ifdef __cplusplus
extern "C" {
#endif

// JPC_JobSystem_Create()
enum
{
    JPC_MAX_PHYSICS_JOBS     = 2048,
    JPC_MAX_PHYSICS_BARRIERS = 8
};

typedef uint8_t JPC_PhysicsUpdateError;
enum
{
    JPC_PHYSICS_UPDATE_NO_ERROR                 = 0,
    JPC_PHYSICS_UPDATE_MANIFOLD_CACHE_FULL      = 1 << 0,
    JPC_PHYSICS_UPDATE_BODY_PAIR_CACHE_FULL     = 1 << 1,
    JPC_PHYSICS_UPDATE_CONTACT_CONSTRAINTS_FULL = 1 << 2,
};

typedef uint8_t JPC_ShapeType;
enum
{
    JPC_SHAPE_TYPE_CONVEX       = 0,
    JPC_SHAPE_TYPE_COMPOUND     = 1,
    JPC_SHAPE_TYPE_DECORATED    = 2,
    JPC_SHAPE_TYPE_MESH         = 3,
    JPC_SHAPE_TYPE_HEIGHT_FIELD = 4,
    JPC_SHAPE_TYPE_USER1        = 5,
    JPC_SHAPE_TYPE_USER2        = 6,
    JPC_SHAPE_TYPE_USER3        = 7,
    JPC_SHAPE_TYPE_USER4        = 8
};

typedef uint8_t JPC_ShapeSubType;
enum
{
    JPC_SHAPE_SUB_TYPE_SPHERE                = 0,
    JPC_SHAPE_SUB_TYPE_BOX                   = 1,
    JPC_SHAPE_SUB_TYPE_TRIANGLE              = 2,
    JPC_SHAPE_SUB_TYPE_CAPSULE               = 3,
    JPC_SHAPE_SUB_TYPE_TAPERED_CAPSULE       = 4,
    JPC_SHAPE_SUB_TYPE_CYLINDER              = 5,
    JPC_SHAPE_SUB_TYPE_CONVEX_HULL           = 6,
    JPC_SHAPE_SUB_TYPE_STATIC_COMPOUND       = 7,
    JPC_SHAPE_SUB_TYPE_MUTABLE_COMPOUND      = 8,
    JPC_SHAPE_SUB_TYPE_ROTATED_TRANSLATED    = 9,
    JPC_SHAPE_SUB_TYPE_SCALED                = 10,
    JPC_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS = 11,
    JPC_SHAPE_SUB_TYPE_MESH                  = 12,
    JPC_SHAPE_SUB_TYPE_HEIGHT_FIELD          = 13,
    JPC_SHAPE_SUB_TYPE_USER1                 = 14,
    JPC_SHAPE_SUB_TYPE_USER2                 = 15,
    JPC_SHAPE_SUB_TYPE_USER3                 = 16,
    JPC_SHAPE_SUB_TYPE_USER4                 = 17,
    JPC_SHAPE_SUB_TYPE_USER5                 = 18,
    JPC_SHAPE_SUB_TYPE_USER6                 = 19,
    JPC_SHAPE_SUB_TYPE_USER7                 = 20,
    JPC_SHAPE_SUB_TYPE_USER8                 = 21,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX1          = 22,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX2          = 23,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX3          = 24,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX4          = 25,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX5          = 26,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX6          = 27,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX7          = 28,
    JPC_SHAPE_SUB_TYPE_USER_CONVEX8          = 29,
};

typedef enum JPC_ConstraintType
{
    JPC_CONSTRAINT_TYPE_CONSTRAINT          = 0,
    JPC_CONSTRAINT_TYPE_TWO_BODY_CONSTRAINT = 1,
    _JPC_CONSTRAINT_TYPE_FORCEU32           = 0x7fffffff
} JPC_ConstraintType;

typedef enum JPC_ConstraintSubType
{
    JPC_CONSTRAINT_SUB_TYPE_FIXED           = 0,
    JPC_CONSTRAINT_SUB_TYPE_POINT           = 1,
    JPC_CONSTRAINT_SUB_TYPE_HINGE           = 2,
    JPC_CONSTRAINT_SUB_TYPE_SLIDER          = 3,
    JPC_CONSTRAINT_SUB_TYPE_DISTANCE        = 4,
    JPC_CONSTRAINT_SUB_TYPE_CONE            = 5,
    JPC_CONSTRAINT_SUB_TYPE_SWING_TWIST     = 6,
    JPC_CONSTRAINT_SUB_TYPE_SIX_DOF         = 7,
    JPC_CONSTRAINT_SUB_TYPE_PATH            = 8,
    JPC_CONSTRAINT_SUB_TYPE_VEHICLE         = 9,
    JPC_CONSTRAINT_SUB_TYPE_RACK_AND_PINION = 10,
    JPC_CONSTRAINT_SUB_TYPE_GEAR            = 11,
    JPC_CONSTRAINT_SUB_TYPE_PULLEY          = 12,
    JPC_CONSTRAINT_SUB_TYPE_USER1           = 13,
    JPC_CONSTRAINT_SUB_TYPE_USER2           = 14,
    JPC_CONSTRAINT_SUB_TYPE_USER3           = 15,
    JPC_CONSTRAINT_SUB_TYPE_USER4           = 16,
    _JPC_CONSTRAINT_SUB_TYPE_FORCEU32       = 0x7fffffff
} JPC_ConstraintSubType;

typedef enum JPC_ConstraintSpace
{
    JPC_CONSTRAINT_SPACE_LOCAL_TO_BODY_COM = 0,
    JPC_CONSTRAINT_SPACE_WORLD_SPACE       = 1,
    _JPC_CONSTRAINT_SPACE_FORCEU32         = 0x7fffffff
} JPC_ConstraintSpace;

typedef uint8_t JPC_MotionType;
enum
{
    JPC_MOTION_TYPE_STATIC    = 0,
    JPC_MOTION_TYPE_KINEMATIC = 1,
    JPC_MOTION_TYPE_DYNAMIC   = 2
};

typedef uint8_t JPC_MotionQuality;
enum
{
    JPC_MOTION_QUALITY_DISCRETE    = 0,
    JPC_MOTION_QUALITY_LINEAR_CAST = 1
};

typedef uint8_t JPC_OverrideMassProperties;
enum
{
    JPC_OVERRIDE_MASS_PROPS_CALC_MASS_INERTIA     = 0,
    JPC_OVERRIDE_MASS_PROPS_CALC_INERTIA          = 1,
    JPC_OVERRIDE_MASS_PROPS_MASS_INERTIA_PROVIDED = 2
};

typedef enum JPC_CharacterGroundState
{
    JPC_CHARACTER_GROUND_STATE_ON_GROUND       = 0,
    JPC_CHARACTER_GROUND_STATE_ON_STEEP_GROUND = 1,
    JPC_CHARACTER_GROUND_STATE_NOT_SUPPORTED   = 2,
    JPC_CHARACTER_GROUND_STATE_IN_AIR          = 3,
    _JPC_CHARACTER_GROUND_FORCEU32             = 0x7fffffff
} JPC_CharacterGroundState;

typedef enum JPC_Activation
{
    JPC_ACTIVATION_ACTIVATE      = 0,
    JPC_ACTIVATION_DONT_ACTIVATE = 1,
    _JPC_ACTIVATION_FORCEU32     = 0x7fffffff
} JPC_Activation;

typedef enum JPC_ValidateResult
{
    JPC_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS = 0,
    JPC_VALIDATE_RESULT_ACCEPT_CONTACT      = 1,
    JPC_VALIDATE_RESULT_REJECT_CONTACT      = 2,
    JPC_VALIDATE_RESULT_REJECT_ALL_CONTACTS = 3,
    _JPC_VALIDATE_RESULT_FORCEU32           = 0x7fffffff
} JPC_ValidateResult;

typedef uint8_t JPC_BackFaceMode;
enum
{
    JPC_BACK_FACE_IGNORE  = 0,
    JPC_BACK_FACE_COLLIDE = 1
};

#if JPC_DEBUG_RENDERER == 1
typedef enum JPC_DebugRendererResult {
    JPC_DEBUGRENDERER_SUCCESS,
    JPC_DEBUGRENDERER_DUPLICATE_SINGLETON,
    JPC_DEBUGRENDERER_MISSING_SINGLETON,
    JPC_DEBUGRENDERER_INCOMPLETE_IMPL
} JPC_DebugRendererResult;

typedef enum JPC_CullMode {
    JPC_CULL_BACK_FACE    = 0,
    JPC_CULL_FRONT_FACE   = 1,
    JPC_CULLING_OFF       = 2,
    _JPC_CULLING_FORCEU32 = 0x7fffffff
} JPC_CullMode;

typedef enum JPC_CastShadow {
    JPC_CAST_SHADOW_ON        = 0,
    JPC_CAST_SHADOW_OFF       = 1,
    _JPC_CAST_SHADOW_FORCEU32 = 0x7fffffff
} JPC_CastShadow;

typedef enum JPC_DrawMode {
    JPC_DRAW_MODE_SOLID     = 0,
    JPC_DRAW_MODE_WIREFRAME = 1,
    _JPC_DRAW_MODE_FORCEU32 = 0x7fffffff
} JPC_DrawMode;

typedef enum JPC_ShapeColor {
    JPC_INSTANCE_COLOR,     // Random color per instance
    JPC_SHAPE_TYPE_COLOR,   // Convex = green, scaled = yellow, compound = orange, mesh = red
    JPC_MOTION_TYPE_COLOR,  // Static = grey, keyframed = green, dynamic = random color per instance
    JPC_SLEEP_COLOR,        // Static = grey, keyframed = green, dynamic = yellow, sleeping = red
    JPC_ISLAND_COLOR,       // Static = grey, active = random color per island, sleeping = light grey
    JPC_MATERIAL_COLOR,     // Color as defined by the PhysicsMaterial of the shape
} JPC_ShapeColor;
#endif //JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// Types
//
//--------------------------------------------------------------------------------------------------
typedef uint16_t JPC_ObjectLayer;
typedef uint8_t  JPC_BroadPhaseLayer;

// TODO: Consider using structures for IDs
typedef uint32_t JPC_BodyID;
typedef uint32_t JPC_SubShapeID;
typedef uint32_t JPC_CollisionGroupID;
typedef uint32_t JPC_CollisionSubGroupID;

// Must be 16 byte aligned
typedef void *(*JPC_AllocateFunction)(size_t in_size);
typedef void (*JPC_FreeFunction)(void *in_block);

typedef void *(*JPC_AlignedAllocateFunction)(size_t in_size, size_t in_alignment);
typedef void (*JPC_AlignedFreeFunction)(void *in_block);

typedef void (*JPC_TraceFunction)(const char *inFMT, ...);
typedef bool (*JPC_AssertFailedFunction)(
    const char* in_expression,
    const char* in_message,
    const char* in_file,
    uint32_t in_line);
//--------------------------------------------------------------------------------------------------
//
// Opaque Types
//
//--------------------------------------------------------------------------------------------------
typedef struct JPC_TempAllocator     JPC_TempAllocator;
typedef struct JPC_JobSystem         JPC_JobSystem;
typedef struct JPC_BodyInterface     JPC_BodyInterface;
typedef struct JPC_BodyLockInterface JPC_BodyLockInterface;
typedef struct JPC_NarrowPhaseQuery  JPC_NarrowPhaseQuery;

typedef struct JPC_ShapeSettings               JPC_ShapeSettings;
typedef struct JPC_ConvexShapeSettings         JPC_ConvexShapeSettings;
typedef struct JPC_BoxShapeSettings            JPC_BoxShapeSettings;
typedef struct JPC_SphereShapeSettings         JPC_SphereShapeSettings;
typedef struct JPC_TriangleShapeSettings       JPC_TriangleShapeSettings;
typedef struct JPC_CapsuleShapeSettings        JPC_CapsuleShapeSettings;
typedef struct JPC_TaperedCapsuleShapeSettings JPC_TaperedCapsuleShapeSettings;
typedef struct JPC_CylinderShapeSettings       JPC_CylinderShapeSettings;
typedef struct JPC_ConvexHullShapeSettings     JPC_ConvexHullShapeSettings;
typedef struct JPC_HeightFieldShapeSettings    JPC_HeightFieldShapeSettings;
typedef struct JPC_MeshShapeSettings           JPC_MeshShapeSettings;
typedef struct JPC_DecoratedShapeSettings      JPC_DecoratedShapeSettings;
typedef struct JPC_CompoundShapeSettings       JPC_CompoundShapeSettings;
typedef struct JPC_CharacterContactSettings    JPC_CharacterContactSettings;

typedef struct JPC_ConstraintSettings        JPC_ConstraintSettings;
typedef struct JPC_TwoBodyConstraintSettings JPC_TwoBodyConstraintSettings;
typedef struct JPC_FixedConstraintSettings   JPC_FixedConstraintSettings;

typedef struct JPC_PhysicsSystem JPC_PhysicsSystem;
typedef struct JPC_SharedMutex   JPC_SharedMutex;

typedef struct JPC_Shape           JPC_Shape;
typedef struct JPC_BoxShape        JPC_BoxShape;
typedef struct JPC_ConvexHullShape JPC_ConvexHullShape;

typedef struct JPC_Constraint       JPC_Constraint;
typedef struct JPC_PhysicsMaterial  JPC_PhysicsMaterial;
typedef struct JPC_GroupFilter      JPC_GroupFilter;
typedef struct JPC_Character        JPC_Character;
typedef struct JPC_CharacterVirtual JPC_CharacterVirtual;

#if JPC_DEBUG_RENDERER == 1
typedef struct JPC_BodyDrawFilter              JPC_BodyDrawFilter;
typedef struct JPC_DebugRenderer_TriangleBatch JPC_DebugRenderer_TriangleBatch;
typedef struct JPC_DebugRenderer_Primitive     JPC_DebugRenderer_Primitive;
#endif //JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// Structures
//
//--------------------------------------------------------------------------------------------------
// NOTE: Needs to be kept in sync with JPH::MassProperties
typedef struct JPC_MassProperties
{
    float             mass;
    alignas(16) float inertia[16];
} JPC_MassProperties;

// NOTE: Needs to be kept in sync with JPH::MotionProperties
typedef struct JPC_MotionProperties
{
    alignas(16) float  linear_velocity[4]; // 4th element is ignored
    alignas(16) float  angular_velocity[4]; // 4th element is ignored
    alignas(16) float  inv_inertia_diagonal[4]; // 4th element is ignored
    alignas(16) float  inertia_rotation[4];

    float              force[3];
    float              torque[3];
    float              inv_mass;
    float              linear_damping;
    float              angular_damping;
    float              max_linear_velocity;
    float              max_angular_velocity;
    float              gravity_factor;
    uint32_t           index_in_active_bodies;
    uint32_t           island_index;

    JPC_MotionQuality  motion_quality;
    bool               allow_sleeping;

#if JPC_DOUBLE_PRECISION == 1
    alignas(8) uint8_t reserved[76];
#else
    alignas(4) uint8_t reserved[52];
#endif

#if JPC_ENABLE_ASSERTS == 1
    JPC_MotionType     cached_motion_type;
#endif
} JPC_MotionProperties;

// NOTE: Needs to be kept in sync with JPH::CollisionGroup
typedef struct JPC_CollisionGroup
{
    const JPC_GroupFilter * filter;
    JPC_CollisionGroupID    group_id;
    JPC_CollisionSubGroupID sub_group_id;
} JPC_CollisionGroup;

// NOTE: Needs to be kept in sync with JPH::BodyCreationSettings
typedef struct JPC_BodyCreationSettings
{
    JPC_RVEC_ALIGN JPC_Real    position[4]; // 4th element is ignored
    alignas(16) float          rotation[4];
    alignas(16) float          linear_velocity[4]; // 4th element is ignored
    alignas(16) float          angular_velocity[4]; // 4th element is ignored
    uint64_t                   user_data;
    JPC_ObjectLayer            object_layer;
    JPC_CollisionGroup         collision_group;
    JPC_MotionType             motion_type;
    bool                       allow_dynamic_or_kinematic;
    bool                       is_sensor;
    bool                       use_manifold_reduction;
    JPC_MotionQuality          motion_quality;
    bool                       allow_sleeping;
    float                      friction;
    float                      restitution;
    float                      linear_damping;
    float                      angular_damping;
    float                      max_linear_velocity;
    float                      max_angular_velocity;
    float                      gravity_factor;
    JPC_OverrideMassProperties override_mass_properties;
    float                      inertia_multiplier;
    JPC_MassProperties         mass_properties_override;
    const void *               reserved;
    const JPC_Shape *          shape;
} JPC_BodyCreationSettings;

// NOTE: Needs to be kept in sync with JPH::Body
typedef struct JPC_Body
{
    JPC_RVEC_ALIGN JPC_Real position[4]; // 4th element is ignored
    alignas(16) float       rotation[4];
    alignas(16) float       bounds_min[4]; // 4th element is ignored
    alignas(16) float       bounds_max[4]; // 4th element is ignored

    const JPC_Shape *       shape;
    JPC_MotionProperties *  motion_properties; // will be NULL for static bodies
    uint64_t                user_data;
    JPC_CollisionGroup      collision_group;

    float                   friction;
    float                   restitution;
    JPC_BodyID              id;

    JPC_ObjectLayer         object_layer;

    JPC_BroadPhaseLayer     broad_phase_layer;
    JPC_MotionType          motion_type;
    uint8_t                 flags;
} JPC_Body;

// NOTE: Needs to be kept in sync
typedef struct JPC_CharacterBaseSettings
{
#   if defined(_MSC_VER)
        const void* __vtable_header[1];
#   else
        const void* __vtable_header[2];
#   endif
    alignas(16) float   up[4]; // 4th element is ignored
    alignas(16) float   supporting_volume[4];
    float               max_slope_angle;
    const JPC_Shape *   shape;
} JPC_CharacterBaseSettings;

// NOTE: Needs to be kept in sync
typedef struct JPC_CharacterSettings
{
    JPC_CharacterBaseSettings base;
    JPC_ObjectLayer layer;
    float mass;
    float friction;
    float gravity_factor;
} JPC_CharacterSettings;

// NOTE: Needs to be kept in sync
typedef struct JPC_CharacterVirtualSettings
{
    JPC_CharacterBaseSettings base;
    float               mass;
    float               max_strength;
    alignas(16) float   shape_offset[4];
    JPC_BackFaceMode    back_face_mode;
    float               predictive_contact_distance;
    uint32_t            max_collision_iterations;
    uint32_t            max_constraint_iterations;
    float               min_time_remaining;
    float               collision_tolerance;
    float               character_padding;
    uint32_t            max_num_hits;
    float               hit_reduction_cos_max_angle;
    float               penetration_recovery_speed;
} JPC_CharacterVirtualSettings;

// NOTE: Needs to be kept in sync with JPH::SubShapeIDCreator
typedef struct JPC_SubShapeIDCreator
{
    JPC_SubShapeID id;
    uint32_t       current_bit;
} JPC_SubShapeIDCreator;

// NOTE: Needs to be kept in sync with JPH::SubShapeIDPair
typedef struct JPC_SubShapeIDPair
{
    struct {
        JPC_BodyID     body_id;
        JPC_SubShapeID sub_shape_id;
    }                  first;
    struct {
        JPC_BodyID     body_id;
        JPC_SubShapeID sub_shape_id;
    }                  second;
} JPC_SubShapeIDPair;

// NOTE: Needs to be kept in sync with JPH::ContactManifold
typedef struct JPC_ContactManifold
{
    JPC_RVEC_ALIGN JPC_Real  base_offset[4]; // 4th element is ignored
    alignas(16) float        normal[4]; // 4th element is ignored; world space
    float                    penetration_depth;
    JPC_SubShapeID           shape1_sub_shape_id;
    JPC_SubShapeID           shape2_sub_shape_id;
    struct {
        alignas(16) uint32_t num_points;
        alignas(16) float    points[64][4]; // 4th element is ignored; world space
    }                        shape1_relative_contact;
    struct {
        alignas(16) uint32_t num_points;
        alignas(16) float    points[64][4]; // 4th element is ignored; world space
    }                        shape2_relative_contact;
} JPC_ContactManifold;

// NOTE: Needs to be kept in sync with JPH::ContactSettings
typedef struct JPC_ContactSettings
{
    float combined_friction;
    float combined_restitution;
    bool  is_sensor;
} JPC_ContactSettings;

// NOTE: Needs to be kept in sync with JPH::CollideShapeResult
typedef struct JPC_CollideShapeResult
{
    alignas(16) float        shape1_contact_point[4]; // 4th element is ignored; world space
    alignas(16) float        shape2_contact_point[4]; // 4th element is ignored; world space
    alignas(16) float        penetration_axis[4]; // 4th element is ignored; world space
    float                    penetration_depth;
    JPC_SubShapeID           shape1_sub_shape_id;
    JPC_SubShapeID           shape2_sub_shape_id;
    JPC_BodyID               body2_id;
    struct {
        alignas(16) uint32_t num_points;
        alignas(16) float    points[32][4]; // 4th element is ignored; world space
    }                        shape1_face;
    struct {
        alignas(16) uint32_t num_points;
        alignas(16) float    points[32][4]; // 4th element is ignored; world space
    }                        shape2_face;
} JPC_CollideShapeResult;

// NOTE: Needs to be kept in sync with JPH::TransformedShape
typedef struct JPC_TransformedShape
{
    JPC_RVEC_ALIGN JPC_Real shape_position_com[4]; // 4th element is ignored
    alignas(16) float       shape_rotation[4];
    const JPC_Shape *       shape;
    float                   shape_scale[3];
    JPC_BodyID              body_id;
    JPC_SubShapeIDCreator   sub_shape_id_creator;
} JPC_TransformedShape;

// NOTE: Needs to be kept in sync with JPH::BodyLockRead
typedef struct JPC_BodyLockRead
{
    const JPC_BodyLockInterface *lock_interface;
    JPC_SharedMutex *            mutex;
    const JPC_Body *             body;
} JPC_BodyLockRead;

// NOTE: Needs to be kept in sync with JPH::BodyLockWrite
typedef struct JPC_BodyLockWrite
{
    const JPC_BodyLockInterface *lock_interface;
    JPC_SharedMutex *            mutex;
    JPC_Body *                   body;
} JPC_BodyLockWrite;

// NOTE: Needs to be kept in sync with JPH::RayCast
typedef struct JPC_RayCast
{
    alignas(16) float origin[4]; // 4th element is ignored
    alignas(16) float direction[4]; // length of the vector is important; 4th element is ignored
} JPC_RayCast;

// NOTE: Needs to be kept in sync with JPH::RRayCast
typedef struct JPC_RRayCast
{
    JPC_RVEC_ALIGN JPC_Real origin[4]; // 4th element is ignored
    alignas(16) float       direction[4]; // length of the vector is important; 4th element is ignored
} JPC_RRayCast;

// NOTE: Needs to be kept in sync with JPH::RayCastResult
typedef struct JPC_RayCastResult
{
    JPC_BodyID     body_id; // JPC_BODY_ID_INVALID
    float          fraction; // 1.0 + JPC_FLT_EPSILON
    JPC_SubShapeID sub_shape_id;
} JPC_RayCastResult;

// NOTE: Needs to be kept in sync with JPH::RayCastSettings
typedef struct JPC_RayCastSettings
{
    JPC_BackFaceMode back_face_mode;
    bool             treat_convex_as_solid;
} JPC_RayCastSettings;

typedef struct JPC_AABox
{
    alignas(16) float min[3];
    alignas(16) float max[3];
} JPC_AABox;

typedef struct JPC_RMatrix
{
    alignas(16) float column_0[4];
    alignas(16) float column_1[4];
    alignas(16) float column_2[4];
    JPC_RVEC_ALIGN JPC_Real column_3[4];
} JPC_RMatrix;

typedef struct JPC_Shape_SupportingFace
{
    alignas(16) uint32_t num_points;
    alignas(16) float    points[32][4]; // 4th element is ignored; world space
} JPC_Shape_SupportingFace;

#if JPC_DEBUG_RENDERER == 1
// NOTE: Needs to be kept in sync with JPH::AABox

// NOTE: Needs to be kept in sync with JPH::Color
typedef union JPC_Color
{
    uint32_t u32;
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
} JPC_Color;

// NOTE: Needs to be kept in sync with JPH::DebugRenderer::Vertex
typedef struct JPC_DebugRenderer_Vertex
{
    float position[3];
    float normal[3];
    float uv[2];
    JPC_Color color;
} JPC_DebugRenderer_Vertex;

// NOTE: Needs to be kept in sync with JPH::DebugRenderer::Triangle
typedef struct JPC_DebugRenderer_Triangle
{
    JPC_DebugRenderer_Vertex v[3];
} JPC_DebugRenderer_Triangle;

// NOTE: Needs to be kept in sync with JPH::DebugRenderer::LOD
typedef struct JPC_DebugRenderer_LOD
{
    JPC_DebugRenderer_TriangleBatch *batch;
    float distance;
} JPC_DebugRenderer_LOD;

// NOTE: NOT kept in sync - some translation required due to JPH::DebugRenderer::Geometry using std::vector.
typedef struct JPC_DebugRenderer_Geometry
{
    JPC_DebugRenderer_LOD *LODs;
    uint64_t num_LODs;
    JPC_AABox *bounds;
} JPC_DebugRenderer_Geometry;

// NOTE: Needs to be kept in sync with JPH::BodyManager::DrawSettings
// For each boolean field, if it's true, that thing will be drawn.
typedef struct JPC_BodyManager_DrawSettings
{
    bool get_support_func;         // = false | Draw the GetSupport() function, used for convex collision detection
    bool get_support_dir;          // = false | If above true, also draw direction mapped to a specific support point
    bool get_supporting_face;      // = false | Draw the faces that were found colliding during collision detection
    bool shape;                    // = true  | Draw the shapes of all bodies
    bool shape_wireframe;          // = false | If 'shape' true, the shapes will be drawn in wireframe instead of solid.
    JPC_ShapeColor shape_color;    // = JPC_MOTION_TYPE_COLOR | Coloring scheme to use for shapes
    bool bounding_box;             // = false | Draw a bounding box per body
    bool center_of_mass_transform; // = false | Draw the center of mass for each body
    bool world_transform;          // = false | Draw the world transform (which can be different than CoM) for each body
    bool velocity;                 // = false | Draw the velocity vector for each body
    bool mass_and_inertia;         // = false | Draw the mass and inertia (as the box equivalent) for each body
    bool sleep_stats;              // = false | Draw stats regarding the sleeping algorithm of each body
} JPC_BodyManager_DrawSettings;

typedef bool (*JPC_BodyDrawFilterFunc)(const JPC_Body *);
#endif //JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// Interfaces (virtual tables)
//
//--------------------------------------------------------------------------------------------------
#if defined(_MSC_VER)
#define _JPC_VTABLE_HEADER const void* __vtable_header[1]
#else
#define _JPC_VTABLE_HEADER const void* __vtable_header[2]
#endif

typedef struct JPC_BroadPhaseLayerInterfaceVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    uint32_t
    (*GetNumBroadPhaseLayers)(const void *in_self);

#ifdef _MSC_VER
    // Required, *cannot* be NULL.
    const JPC_BroadPhaseLayer *
    (*GetBroadPhaseLayer)(const void *in_self, JPC_BroadPhaseLayer *out_layer, JPC_ObjectLayer in_layer);
#else
    // Required, *cannot* be NULL.
    JPC_BroadPhaseLayer
    (*GetBroadPhaseLayer)(const void *in_self, JPC_ObjectLayer in_layer);
#endif
} JPC_BroadPhaseLayerInterfaceVTable;

typedef struct JPC_ObjectVsBroadPhaseLayerFilterVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    bool
    (*ShouldCollide)(const void *in_self, JPC_ObjectLayer in_layer1, JPC_BroadPhaseLayer in_layer2);
} JPC_ObjectVsBroadPhaseLayerFilterVTable;

typedef struct JPC_BroadPhaseLayerFilterVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    bool
    (*ShouldCollide)(const void *in_self, JPC_BroadPhaseLayer in_layer);
} JPC_BroadPhaseLayerFilterVTable;

typedef struct JPC_ObjectLayerPairFilterVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    bool
    (*ShouldCollide)(const void *in_self, JPC_ObjectLayer in_layer1, JPC_ObjectLayer in_layer2);
} JPC_ObjectLayerPairFilterVTable;

typedef struct JPC_ObjectLayerFilterVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    bool
    (*ShouldCollide)(const void *in_self, JPC_ObjectLayer in_layer);
} JPC_ObjectLayerFilterVTable;

typedef struct JPC_BodyActivationListenerVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    void
    (*OnBodyActivated)(void *in_self, const JPC_BodyID *in_body_id, uint64_t in_user_data);

    // Required, *cannot* be NULL.
    void
    (*OnBodyDeactivated)(void *in_self, const JPC_BodyID *in_body_id, uint64_t in_user_data);
} JPC_BodyActivationListenerVTable;

typedef struct JPC_BodyFilterVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    bool
    (*ShouldCollide)(const void *in_self, const JPC_BodyID *in_body_id);

    // Required, *cannot* be NULL.
    bool
    (*ShouldCollideLocked)(const void *in_self, const JPC_Body *in_body);
} JPC_BodyFilterVTable;

typedef struct JPC_ShapeFilterVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    bool
    (*ShouldCollide)(const void *in_self, const JPC_Shape *in_shape, const JPC_SubShapeID *in_sub_shape_id);

    // Required, *cannot* be NULL.
    bool
    (*PairShouldCollide)(const void *in_self,
                         const JPC_Shape *in_shape1,
                         const JPC_SubShapeID *in_sub_shape_id1,
                         const JPC_Shape *in_shape2,
                         const JPC_SubShapeID *in_sub_shape_id2);

    // Set by the collision detection functions to the body ID of the "receiving" body before ShouldCollide is called.
    uint32_t bodyId2;
} JPC_ShapeFilterVTable;

typedef struct JPC_PhysicsStepListenerVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    void
    (*OnStep)(float in_delta_time, JPC_PhysicsSystem *in_physics_system);
} JPC_PhysicsStepListener;

// Made all callbacks required for this one for simplicity's sake, but can be modified to imitate ContactListener later.
typedef struct JPC_CharacterContactListenerVTable
{
    _JPC_VTABLE_HEADER;

    // Required, *cannot* be NULL.
    void
    (*OnAdjustBodyVelocity)(void *in_self,
                            const JPC_CharacterVirtual *in_character,
                            const JPC_Body *in_body2,
                            const float io_linear_velocity[3],
                            const float io_angular_velocity[3]);

    // Required, *cannot* be NULL.
    bool
    (*OnContactValidate)(void *in_self,
                         const JPC_CharacterVirtual *in_character,
                         const JPC_Body *in_body2,
                         const JPC_SubShapeID *sub_shape_id);

    // Required, *cannot* be NULL.
    void
    (*OnContactAdded)(void *in_self,
                      const JPC_CharacterVirtual *in_character,
                      const JPC_Body *in_body2,
                      const JPC_SubShapeID *sub_shape_id,
                      const JPC_Real contact_position[3],
                      const float contact_normal[3],
                      JPC_CharacterContactSettings *io_settings);

    // Required, *cannot* be NULL.
    void
    (*OnContactSolve)(void *in_self,
                      const JPC_CharacterVirtual *in_character,
                      const JPC_Body *in_body2,
                      const JPC_SubShapeID *sub_shape_id,
                      const JPC_Real contact_position[3],
                      const float contact_normal[3],
                      const float contact_velocity[3],
                      const JPC_PhysicsMaterial *contact_material,
                      const float character_velocity_in[3],
                      float character_velocity_out[3]);
} JPC_CharacterContactListenerVTable;

typedef struct JPC_ContactListenerVTable
{
    // Optional, can be NULL.
    JPC_ValidateResult
    (*OnContactValidate)(void *in_self,
                         const JPC_Body *in_body1,
                         const JPC_Body *in_body2,
                         const JPC_Real in_base_offset[3],
                         const JPC_CollideShapeResult *in_collision_result);

    // Optional, can be NULL.
    void
    (*OnContactAdded)(void *in_self,
                      const JPC_Body *in_body1,
                      const JPC_Body *in_body2,
                      const JPC_ContactManifold *in_manifold,
                      JPC_ContactSettings *io_settings);

    // Optional, can be NULL.
    void
    (*OnContactPersisted)(void *in_self,
                          const JPC_Body *in_body1,
                          const JPC_Body *in_body2,
                          const JPC_ContactManifold *in_manifold,
                          JPC_ContactSettings *io_settings);

    // Optional, can be NULL.
    void
    (*OnContactRemoved)(void *in_self, const JPC_SubShapeIDPair *in_sub_shape_pair);
} JPC_ContactListenerVTable;

#if JPC_DEBUG_RENDERER == 1
/// Although used similarly to the VTables above, this struct is not pointer-compatible with JPH::DebugRenderer
/// Instead, it's wrapped by the DebugRendererImpl inheritor class (as seen in JoltPhysicsC.cpp), because
/// of the design of JPH::DebugRenderer not playing as nicely with C as the other structures in Jolt.
/// Since debug rendering should never be used in production code, a wrapper seems ok in this case.
typedef struct JPC_DebugRendererVTable
{
    // Required, *cannot* be NULL.
    void
    (*DrawLine)(void *in_self, JPC_Real in_from[3], JPC_Real in_to[3], JPC_Color in_color);

    // Required, *cannot* be NULL.
    void
    (*DrawTriangle)(void *in_self, JPC_Real in_v1[3], JPC_Real in_v2[3], JPC_Real in_v3[3], JPC_Color in_color);

    // Required, *cannot* be NULL.
    JPC_DebugRenderer_TriangleBatch *
    (*CreateTriangleBatch)(void *in_self, const JPC_DebugRenderer_Triangle *in_triangles, uint32_t in_triangle_count);

    // Required, *cannot* be NULL.
    JPC_DebugRenderer_TriangleBatch *
    (*CreateTriangleBatchIndexed)(void *in_self,
                                  const JPC_DebugRenderer_Vertex *in_vertices,
                                  uint32_t in_vertex_count,
                                  const uint32_t *in_indices,
                                  uint32_t in_index_count);

    // Required, *cannot* be NULL.
    void
    (*DrawGeometry)(void *in_self,
                    const JPC_RMatrix* inModelMatrix,
                    const JPC_AABox *inWorldSpaceBounds,
                    float inLODScaleSq,
                    JPC_Color in_color,
                    const JPC_DebugRenderer_Geometry *in_geometry,
                    JPC_CullMode in_cull_mode,
                    JPC_CastShadow in_cast_shadow,
                    JPC_DrawMode in_draw_mode);

    // Required, *cannot* be NULL.
    void
    (*DrawText3D)(void *in_self, JPC_Real in_position[3], const char *in_string, JPC_Color in_color, float in_height);
} JPC_DebugRendererVTable;
#endif //JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// Misc functions
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_RegisterDefaultAllocator(void);

JPC_API void
JPC_RegisterCustomAllocator(JPC_AllocateFunction in_alloc,
                            JPC_FreeFunction in_free,
                            JPC_AlignedAllocateFunction in_aligned_alloc,
                            JPC_AlignedFreeFunction in_aligned_free);

JPC_API void
JPC_RegisterTrace(JPC_TraceFunction in_trace);

JPC_API void
JPC_RegisterAssertFailed(JPC_AssertFailedFunction in_assert_failed);

JPC_API void
JPC_CreateFactory(void);

JPC_API void
JPC_DestroyFactory(void);

JPC_API void
JPC_RegisterTypes(void);

JPC_API void
JPC_BodyCreationSettings_SetDefault(JPC_BodyCreationSettings *out_settings);

JPC_API void
JPC_BodyCreationSettings_Set(JPC_BodyCreationSettings *out_settings,
                             const JPC_Shape *in_shape,
                             const JPC_Real in_position[3],
                             const float in_rotation[4],
                             JPC_MotionType in_motion_type,
                             JPC_ObjectLayer in_layer);

#if JPC_DEBUG_RENDERER == 1
/// Provide an instantiated VTable to get wrapped by the singleton implementation of JPH::DebugRenderer. This should be
/// called only once, at program initialization, as when instantiating a DebugRenderer implementation in Jolt proper.
/// You may pass a pointer to any struct, as long as its first member is a pointer to your JPC_DebugRendererVTable.
JPC_API enum JPC_DebugRendererResult
JPC_CreateDebugRendererSingleton(void *in_debug_renderer);
/// Iff there is a debug renderer currently instantiated, destroy it. This may allow another call to CreateDebugRenderer
/// to be made without breaking things, but this isn't sufficiently tested to be a guarantee. This is used, for example,
/// in the unit tests when more than one test needs to instantiate a debug renderer. Shouldn't be necessary for a game.
JPC_API enum JPC_DebugRendererResult
JPC_DestroyDebugRendererSingleton();
#endif //JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// JPC_DebugRenderer_TriangleBatch
//
//--------------------------------------------------------------------------------------------------
#if JPC_DEBUG_RENDERER == 1
/// Within the user's DebugRendererVTable callbacks to create triangle batches, the user creates whatever
/// structure their rendering engine requires to represent the triangle batch Jolt requests. The user passes a
/// pointer to that structure into this function to be stored as one of Jolt's reference-counted objects internally.
///
/// \return An opaque JPC_DebugRenderer_TriangleBatch* for the user to keep as a handle to their primitive
JPC_API JPC_DebugRenderer_TriangleBatch *
JPC_DebugRenderer_TriangleBatch_Create(const void *in_c_primitive);

/// When Jolt calls the user's DrawGeometry, it passes the user a JPC_DebugRenderer_Geometry *. This structure
/// contains, among other things, at least one JPC_DebugRenderer_TriangleBatch * (inside LOD levels). The user
/// may retrieve the pointer to the corresponding primitive they made by passing the batch pointer to this function.
///
/// \return An opaque JPC_DebugRenderer_Primitive * wherein the user is keeping rendering data for that batch
JPC_API const JPC_DebugRenderer_Primitive *
JPC_DebugRenderer_TriangleBatch_GetPrimitive(const JPC_DebugRenderer_TriangleBatch *in_batch);

JPC_API void
JPC_DebugRenderer_TriangleBatch_AddRef(JPC_DebugRenderer_TriangleBatch *in_batch);

JPC_API void
JPC_DebugRenderer_TriangleBatch_Release(JPC_DebugRenderer_TriangleBatch *in_batch);

JPC_API uint32_t
JPC_DebugRenderer_TriangleBatch_GetRefCount(const JPC_DebugRenderer_TriangleBatch *in_batch);
#endif //JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// JPC_MotionProperties
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_MotionQuality
JPC_MotionProperties_GetMotionQuality(const JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_GetLinearVelocity(const JPC_MotionProperties *in_properties,
                                       float out_linear_velocity[3]);
JPC_API void
JPC_MotionProperties_SetLinearVelocity(JPC_MotionProperties *in_properties,
                                       const float in_linear_velocity[3]);
JPC_API void
JPC_MotionProperties_SetLinearVelocityClamped(JPC_MotionProperties *in_properties,
                                              const float in_linear_velocity[3]);
JPC_API void
JPC_MotionProperties_GetAngularVelocity(const JPC_MotionProperties *in_properties,
                                        float out_angular_velocity[3]);
JPC_API void
JPC_MotionProperties_SetAngularVelocity(JPC_MotionProperties *in_properties,
                                        const float in_angular_velocity[3]);
JPC_API void
JPC_MotionProperties_SetAngularVelocityClamped(JPC_MotionProperties *in_properties,
                                               const float in_angular_velocity[3]);
JPC_API void
JPC_MotionProperties_MoveKinematic(JPC_MotionProperties *in_properties,
                                   const float in_delta_position[3],
                                   const float in_delta_rotation[4],
                                   float in_delta_time);
JPC_API void
JPC_MotionProperties_ClampLinearVelocity(JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_ClampAngularVelocity(JPC_MotionProperties *in_properties);

JPC_API float
JPC_MotionProperties_GetLinearDamping(const JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_SetLinearDamping(JPC_MotionProperties *in_properties,
                                      float in_linear_damping);
JPC_API float
JPC_MotionProperties_GetAngularDamping(const JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_SetAngularDamping(JPC_MotionProperties *in_properties,
                                       float in_angular_damping);
JPC_API float
JPC_MotionProperties_GetGravityFactor(const JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_SetGravityFactor(JPC_MotionProperties *in_properties,
                                      float in_gravity_factor);
JPC_API void
JPC_MotionProperties_SetMassProperties(JPC_MotionProperties *in_properties,
                                       const JPC_MassProperties *in_mass_properties);
JPC_API float
JPC_MotionProperties_GetInverseMass(const JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_SetInverseMass(JPC_MotionProperties *in_properties, float in_inv_mass);

JPC_API void
JPC_MotionProperties_GetInverseInertiaDiagonal(const JPC_MotionProperties *in_properties,
                                               float out_inverse_inertia_diagonal[3]);
JPC_API void
JPC_MotionProperties_GetInertiaRotation(const JPC_MotionProperties *in_properties,
                                        float out_inertia_rotation[4]);
JPC_API void
JPC_MotionProperties_SetInverseInertia(JPC_MotionProperties *in_properties,
                                       const float in_diagonal[3],
                                       const float in_rotation[4]);
JPC_API void
JPC_MotionProperties_GetLocalSpaceInverseInertia(const JPC_MotionProperties *in_properties,
                                                 float out_matrix[16]);
JPC_API void
JPC_MotionProperties_GetInverseInertiaForRotation(const JPC_MotionProperties *in_properties,
                                                  const float in_rotation_matrix[16],
                                                  float out_matrix[16]);
JPC_API void
JPC_MotionProperties_MultiplyWorldSpaceInverseInertiaByVector(const JPC_MotionProperties *in_properties,
                                                              const float in_body_rotation[4],
                                                              const float in_vector[3],
                                                              float out_vector[3]);
JPC_API void
JPC_MotionProperties_GetPointVelocityCOM(const JPC_MotionProperties *in_properties,
                                         const float in_point_relative_to_com[3],
                                         float out_point[3]);
JPC_API float
JPC_MotionProperties_GetMaxLinearVelocity(const JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_SetMaxLinearVelocity(JPC_MotionProperties *in_properties,
                                          float in_max_linear_velocity);
JPC_API float
JPC_MotionProperties_GetMaxAngularVelocity(const JPC_MotionProperties *in_properties);

JPC_API void
JPC_MotionProperties_SetMaxAngularVelocity(JPC_MotionProperties *in_properties,
                                           float in_max_angular_velocity);
//--------------------------------------------------------------------------------------------------
//
// JPC_TempAllocator
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_TempAllocator *
JPC_TempAllocator_Create(uint32_t in_size);

JPC_API void
JPC_TempAllocator_Destroy(JPC_TempAllocator *in_allocator);
//--------------------------------------------------------------------------------------------------
//
// JPC_JobSystem
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_JobSystem *
JPC_JobSystem_Create(uint32_t in_max_jobs, uint32_t in_max_barriers, int in_num_threads);

JPC_API void
JPC_JobSystem_Destroy(JPC_JobSystem *in_job_system);
//--------------------------------------------------------------------------------------------------
//
// JPC_PhysicsSystem
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_PhysicsSystem *
JPC_PhysicsSystem_Create(uint32_t in_max_bodies,
                         uint32_t in_num_body_mutexes,
                         uint32_t in_max_body_pairs,
                         uint32_t in_max_contact_constraints,
                         const void *in_broad_phase_layer_interface,
                         const void *in_object_vs_broad_phase_layer_filter,
                         const void *in_object_layer_pair_filter);
JPC_API void
JPC_PhysicsSystem_Destroy(JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_PhysicsSystem_SetBodyActivationListener(JPC_PhysicsSystem *in_physics_system, void *in_listener);

JPC_API void *
JPC_PhysicsSystem_GetBodyActivationListener(const JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_PhysicsSystem_SetContactListener(JPC_PhysicsSystem *in_physics_system, void *in_listener);

JPC_API void *
JPC_PhysicsSystem_GetContactListener(const JPC_PhysicsSystem *in_physics_system);

JPC_API uint32_t
JPC_PhysicsSystem_GetNumBodies(const JPC_PhysicsSystem *in_physics_system);

JPC_API uint32_t
JPC_PhysicsSystem_GetNumActiveBodies(const JPC_PhysicsSystem *in_physics_system);

JPC_API uint32_t
JPC_PhysicsSystem_GetMaxBodies(const JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_PhysicsSystem_GetGravity(const JPC_PhysicsSystem *in_physics_system, float out_gravity[3]);

JPC_API void
JPC_PhysicsSystem_SetGravity(JPC_PhysicsSystem *in_physics_system, const float in_gravity[3]);

JPC_API JPC_BodyInterface *
JPC_PhysicsSystem_GetBodyInterface(JPC_PhysicsSystem *in_physics_system);

JPC_API JPC_BodyInterface *
JPC_PhysicsSystem_GetBodyInterfaceNoLock(JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_PhysicsSystem_OptimizeBroadPhase(JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_PhysicsSystem_AddStepListener(JPC_PhysicsSystem *in_physics_system, void *in_listener);

JPC_API void
JPC_PhysicsSystem_RemoveStepListener(JPC_PhysicsSystem *in_physics_system, void *in_listener);

JPC_API void
JPC_PhysicsSystem_AddConstraint(JPC_PhysicsSystem *in_physics_system, void *in_two_body_constraint);

JPC_API void
JPC_PhysicsSystem_RemoveConstraint(JPC_PhysicsSystem *in_physics_system, void *in_two_body_constraint);

JPC_API JPC_PhysicsUpdateError
JPC_PhysicsSystem_Update(JPC_PhysicsSystem *in_physics_system,
                         float in_delta_time,
                         int in_collision_steps,
                         int in_integration_sub_steps,
                         JPC_TempAllocator *in_temp_allocator,
                         JPC_JobSystem *in_job_system);

JPC_API const JPC_BodyLockInterface *
JPC_PhysicsSystem_GetBodyLockInterface(const JPC_PhysicsSystem *in_physics_system);

JPC_API const JPC_BodyLockInterface *
JPC_PhysicsSystem_GetBodyLockInterfaceNoLock(const JPC_PhysicsSystem *in_physics_system);

JPC_API const JPC_NarrowPhaseQuery *
JPC_PhysicsSystem_GetNarrowPhaseQuery(const JPC_PhysicsSystem *in_physics_system);

JPC_API const JPC_NarrowPhaseQuery *
JPC_PhysicsSystem_GetNarrowPhaseQueryNoLock(const JPC_PhysicsSystem *in_physics_system);

/// Get copy of the list of all bodies under protection of a lock.
JPC_API void
JPC_PhysicsSystem_GetBodyIDs(const JPC_PhysicsSystem *in_physics_system,
                             uint32_t in_max_body_ids,
                             uint32_t *out_num_body_ids,
                             JPC_BodyID *out_body_ids);

/// Get copy of the list of active bodies under protection of a lock.
JPC_API void
JPC_PhysicsSystem_GetActiveBodyIDs(const JPC_PhysicsSystem *in_physics_system,
                                   uint32_t in_max_body_ids,
                                   uint32_t *out_num_body_ids,
                                   JPC_BodyID *out_body_ids);
///
/// Low-level access for advanced usage and zero CPU overhead (access *not* protected by a lock)
///
/// Check if this is a valid body pointer.
/// When a body is freed the memory that the pointer occupies is reused to store a freelist.
#define _JPC_IS_FREED_BODY_BIT 0x1

#define JPC_IS_VALID_BODY_POINTER(body_ptr) (((uintptr_t)(body_ptr) & _JPC_IS_FREED_BODY_BIT) == 0)

/// Access a body, will return NULL if the body ID is no longer valid.
/// Use `JPC_PhysicsSystem_GetBodiesUnsafe()` to get an array of all body pointers.
#define JPC_TRY_GET_BODY(all_body_ptrs, body_id) \
    JPC_IS_VALID_BODY_POINTER(all_body_ptrs[body_id & JPC_BODY_ID_INDEX_BITS]) && \
    all_body_ptrs[body_id & JPC_BODY_ID_INDEX_BITS]->id == body_id ? \
    all_body_ptrs[body_id & JPC_BODY_ID_INDEX_BITS] : NULL

/// Get direct access to all bodies. Not protected by a lock. Use with great care!
JPC_API JPC_Body **
JPC_PhysicsSystem_GetBodiesUnsafe(JPC_PhysicsSystem *in_physics_system);

#if JPC_DEBUG_RENDERER == 1
JPC_API void
JPC_PhysicsSystem_DrawBodies(JPC_PhysicsSystem *in_physics_system,
                             const JPC_BodyManager_DrawSettings *in_draw_settings,
                             const JPC_BodyDrawFilter *in_draw_filter); // Can be NULL (no filter)

JPC_API void
JPC_PhysicsSystem_DrawConstraints(JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_PhysicsSystem_DrawConstraintLimits(JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_PhysicsSystem_DrawConstraintReferenceFrame(JPC_PhysicsSystem *in_physics_system);
#endif //JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyLockInterface
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BodyLockInterface_LockRead(const JPC_BodyLockInterface *in_lock_interface,
                               JPC_BodyID in_body_id,
                               JPC_BodyLockRead *out_lock);
JPC_API void
JPC_BodyLockInterface_UnlockRead(const JPC_BodyLockInterface *in_lock_interface,
                                 JPC_BodyLockRead *io_lock);
JPC_API void
JPC_BodyLockInterface_LockWrite(const JPC_BodyLockInterface *in_lock_interface,
                                JPC_BodyID in_body_id,
                                JPC_BodyLockWrite *out_lock);
JPC_API void
JPC_BodyLockInterface_UnlockWrite(const JPC_BodyLockInterface *in_lock_interface,
                                  JPC_BodyLockWrite *io_lock);
//--------------------------------------------------------------------------------------------------
//
// JPC_NarrowPhaseQuery
//
//--------------------------------------------------------------------------------------------------
JPC_API bool
JPC_NarrowPhaseQuery_CastRay(const JPC_NarrowPhaseQuery *in_query,
                             const JPC_RRayCast *in_ray,
                             JPC_RayCastResult *io_hit, // *Must* be default initialized (see JPC_RayCastResult)
                             const void *in_broad_phase_layer_filter, // Can be NULL (no filter)
                             const void *in_object_layer_filter, // Can be NULL (no filter)
                             const void *in_body_filter); // Can be NULL (no filter)
//--------------------------------------------------------------------------------------------------
//
// JPC_ShapeSettings
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ShapeSettings_AddRef(JPC_ShapeSettings *in_settings);

JPC_API void
JPC_ShapeSettings_Release(JPC_ShapeSettings *in_settings);

JPC_API uint32_t
JPC_ShapeSettings_GetRefCount(const JPC_ShapeSettings *in_settings);

/// First call creates the shape, subsequent calls return the same pointer and increments reference count.
/// Call `JPC_Shape_Release()` when you don't need returned pointer anymore.
JPC_API JPC_Shape *
JPC_ShapeSettings_CreateShape(const JPC_ShapeSettings *in_settings);

JPC_API uint64_t
JPC_ShapeSettings_GetUserData(const JPC_ShapeSettings *in_settings);

JPC_API void
JPC_ShapeSettings_SetUserData(JPC_ShapeSettings *in_settings, uint64_t in_user_data);
//--------------------------------------------------------------------------------------------------
//
// JPC_ConvexShapeSettings (-> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API const JPC_PhysicsMaterial *
JPC_ConvexShapeSettings_GetMaterial(const JPC_ConvexShapeSettings *in_settings);

JPC_API void
JPC_ConvexShapeSettings_SetMaterial(JPC_ConvexShapeSettings *in_settings,
                                    const JPC_PhysicsMaterial *in_material);

JPC_API float
JPC_ConvexShapeSettings_GetDensity(const JPC_ConvexShapeSettings *in_settings);

JPC_API void
JPC_ConvexShapeSettings_SetDensity(JPC_ConvexShapeSettings *in_settings, float in_density);
//--------------------------------------------------------------------------------------------------
//
// JPC_BoxShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_BoxShapeSettings *
JPC_BoxShapeSettings_Create(const float in_half_extent[3]);

JPC_API void
JPC_BoxShapeSettings_GetHalfExtent(const JPC_BoxShapeSettings *in_settings, float out_half_extent[3]);

JPC_API void
JPC_BoxShapeSettings_SetHalfExtent(JPC_BoxShapeSettings *in_settings, const float in_half_extent[3]);

JPC_API float
JPC_BoxShapeSettings_GetConvexRadius(const JPC_BoxShapeSettings *in_settings);

JPC_API void
JPC_BoxShapeSettings_SetConvexRadius(JPC_BoxShapeSettings *in_settings, float in_convex_radius);
//--------------------------------------------------------------------------------------------------
//
// JPC_SphereShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_SphereShapeSettings *
JPC_SphereShapeSettings_Create(float in_radius);

JPC_API float
JPC_SphereShapeSettings_GetRadius(const JPC_SphereShapeSettings *in_settings);

JPC_API void
JPC_SphereShapeSettings_SetRadius(JPC_SphereShapeSettings *in_settings, float in_radius);
//--------------------------------------------------------------------------------------------------
//
// JPC_TriangleShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_TriangleShapeSettings *
JPC_TriangleShapeSettings_Create(const float in_v1[3], const float in_v2[3], const float in_v3[3]);

JPC_API void
JPC_TriangleShapeSettings_SetVertices(JPC_TriangleShapeSettings *in_settings,
                                      const float in_v1[3],
                                      const float in_v2[3],
                                      const float in_v3[3]);
JPC_API void
JPC_TriangleShapeSettings_GetVertices(const JPC_TriangleShapeSettings *in_settings,
                                      float out_v1[3],
                                      float out_v2[3],
                                      float out_v3[3]);
JPC_API float
JPC_TriangleShapeSettings_GetConvexRadius(const JPC_TriangleShapeSettings *in_settings);

JPC_API void
JPC_TriangleShapeSettings_SetConvexRadius(JPC_TriangleShapeSettings *in_settings,
                                          float in_convex_radius);
//--------------------------------------------------------------------------------------------------
//
// JPC_CapsuleShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CapsuleShapeSettings *
JPC_CapsuleShapeSettings_Create(float in_half_height_of_cylinder, float in_radius);

JPC_API float
JPC_CapsuleShapeSettings_GetHalfHeight(const JPC_CapsuleShapeSettings *in_settings);

JPC_API void
JPC_CapsuleShapeSettings_SetHalfHeight(JPC_CapsuleShapeSettings *in_settings,
                                       float in_half_height_of_cylinder);
JPC_API float
JPC_CapsuleShapeSettings_GetRadius(const JPC_CapsuleShapeSettings *in_settings);

JPC_API void
JPC_CapsuleShapeSettings_SetRadius(JPC_CapsuleShapeSettings *in_settings, float in_radius);
//--------------------------------------------------------------------------------------------------
//
// JPC_TaperedCapsuleShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_TaperedCapsuleShapeSettings *
JPC_TaperedCapsuleShapeSettings_Create(float in_half_height, float in_top_radius, float in_bottom_radius);

JPC_API float
JPC_TaperedCapsuleShapeSettings_GetHalfHeight(const JPC_TaperedCapsuleShapeSettings *in_settings);

JPC_API void
JPC_TaperedCapsuleShapeSettings_SetHalfHeight(JPC_TaperedCapsuleShapeSettings *in_settings,
                                              float in_half_height);
JPC_API float
JPC_TaperedCapsuleShapeSettings_GetTopRadius(const JPC_TaperedCapsuleShapeSettings *in_settings);

JPC_API void
JPC_TaperedCapsuleShapeSettings_SetTopRadius(JPC_TaperedCapsuleShapeSettings *in_settings, float in_top_radius);

JPC_API float
JPC_TaperedCapsuleShapeSettings_GetBottomRadius(const JPC_TaperedCapsuleShapeSettings *in_settings);

JPC_API void
JPC_TaperedCapsuleShapeSettings_SetBottomRadius(JPC_TaperedCapsuleShapeSettings *in_settings,
                                                float in_bottom_radius);
//--------------------------------------------------------------------------------------------------
//
// JPC_CylinderShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CylinderShapeSettings *
JPC_CylinderShapeSettings_Create(float in_half_height, float in_radius);

JPC_API float
JPC_CylinderShapeSettings_GetConvexRadius(const JPC_CylinderShapeSettings *in_settings);

JPC_API void
JPC_CylinderShapeSettings_SetConvexRadius(JPC_CylinderShapeSettings *in_settings, float in_convex_radius);

JPC_API float
JPC_CylinderShapeSettings_GetHalfHeight(const JPC_CylinderShapeSettings *in_settings);

JPC_API void
JPC_CylinderShapeSettings_SetHalfHeight(JPC_CylinderShapeSettings *in_settings, float in_half_height);

JPC_API float
JPC_CylinderShapeSettings_GetRadius(const JPC_CylinderShapeSettings *in_settings);

JPC_API void
JPC_CylinderShapeSettings_SetRadius(JPC_CylinderShapeSettings *in_settings, float in_radius);
//--------------------------------------------------------------------------------------------------
//
// JPC_ConvexHullShapeSettings (-> JPC_ConvexShapeSettings -> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_ConvexHullShapeSettings *
JPC_ConvexHullShapeSettings_Create(const void *in_vertices, uint32_t in_num_vertices, uint32_t in_vertex_size);

JPC_API float
JPC_ConvexHullShapeSettings_GetMaxConvexRadius(const JPC_ConvexHullShapeSettings *in_settings);

JPC_API void
JPC_ConvexHullShapeSettings_SetMaxConvexRadius(JPC_ConvexHullShapeSettings *in_settings,
                                               float in_max_convex_radius);
JPC_API float
JPC_ConvexHullShapeSettings_GetMaxErrorConvexRadius(const JPC_ConvexHullShapeSettings *in_settings);

JPC_API void
JPC_ConvexHullShapeSettings_SetMaxErrorConvexRadius(JPC_ConvexHullShapeSettings *in_settings,
                                                    float in_max_err_convex_radius);
JPC_API float
JPC_ConvexHullShapeSettings_GetHullTolerance(const JPC_ConvexHullShapeSettings *in_settings);

JPC_API void
JPC_ConvexHullShapeSettings_SetHullTolerance(JPC_ConvexHullShapeSettings *in_settings,
                                             float in_hull_tolerance);
//--------------------------------------------------------------------------------------------------
//
// JPC_HeightFieldShapeSettings (-> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_HeightFieldShapeSettings *
JPC_HeightFieldShapeSettings_Create(const float *in_samples, uint32_t in_height_field_size);

JPC_API void
JPC_HeightFieldShapeSettings_GetOffset(const JPC_HeightFieldShapeSettings *in_settings, float out_offset[3]);

JPC_API void
JPC_HeightFieldShapeSettings_SetOffset(JPC_HeightFieldShapeSettings *in_settings, const float in_offset[3]);

JPC_API void
JPC_HeightFieldShapeSettings_GetScale(const JPC_HeightFieldShapeSettings *in_settings, float out_scale[3]);

JPC_API void
JPC_HeightFieldShapeSettings_SetScale(JPC_HeightFieldShapeSettings *in_settings, const float in_scale[3]);

JPC_API uint32_t
JPC_HeightFieldShapeSettings_GetBlockSize(const JPC_HeightFieldShapeSettings *in_settings);

JPC_API void
JPC_HeightFieldShapeSettings_SetBlockSize(JPC_HeightFieldShapeSettings *in_settings, uint32_t in_block_size);

JPC_API uint32_t
JPC_HeightFieldShapeSettings_GetBitsPerSample(const JPC_HeightFieldShapeSettings *in_settings);

JPC_API void
JPC_HeightFieldShapeSettings_SetBitsPerSample(JPC_HeightFieldShapeSettings *in_settings, uint32_t in_num_bits);
//--------------------------------------------------------------------------------------------------
//
// JPC_MeshShapeSettings (-> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_MeshShapeSettings *
JPC_MeshShapeSettings_Create(const void *in_vertices,
                             uint32_t in_num_vertices,
                             uint32_t in_vertex_size,
                             const uint32_t *in_indices,
                             uint32_t in_num_indices);
JPC_API uint32_t
JPC_MeshShapeSettings_GetMaxTrianglesPerLeaf(const JPC_MeshShapeSettings *in_settings);

JPC_API void
JPC_MeshShapeSettings_SetMaxTrianglesPerLeaf(JPC_MeshShapeSettings *in_settings, uint32_t in_max_triangles);

JPC_API void
JPC_MeshShapeSettings_Sanitize(JPC_MeshShapeSettings *in_settings);
//--------------------------------------------------------------------------------------------------
//
// JPC_DecoratedShapeSettings (-> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_DecoratedShapeSettings *
JPC_RotatedTranslatedShapeSettings_Create(const JPC_ShapeSettings *in_inner_shape_settings,
                                          const float in_rotated[4],
                                          const float in_translated[3]);

JPC_API JPC_DecoratedShapeSettings *
JPC_ScaledShapeSettings_Create(const JPC_ShapeSettings *in_inner_shape_settings,
                               const float in_scale[3]);

JPC_API JPC_DecoratedShapeSettings *
JPC_OffsetCenterOfMassShapeSettings_Create(const JPC_ShapeSettings *in_inner_shape_settings,
                                           const float in_center_of_mass[3]);
//--------------------------------------------------------------------------------------------------
//
// JPC_CompoundShapeSettings (-> JPC_ShapeSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CompoundShapeSettings *
JPC_StaticCompoundShapeSettings_Create();

JPC_API JPC_CompoundShapeSettings *
JPC_MutableCompoundShapeSettings_Create();

JPC_API void
JPC_CompoundShapeSettings_AddShape(JPC_CompoundShapeSettings *in_settings,
                                   const float in_position[3],
                                   const float in_rotation[4],
                                   const JPC_ShapeSettings *in_shape,
                                   const uint32_t in_user_data);
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyManager_DrawSettings
//
//--------------------------------------------------------------------------------------------------
#if JPC_DEBUG_RENDERER == 1
JPC_API JPC_BodyManager_DrawSettings *
JPC_BodyManager_DrawSettings_Create();

JPC_API void
JPC_BodyManager_DrawSettings_Destroy(JPC_BodyManager_DrawSettings *);
#endif // JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyDrawFilter
//
//--------------------------------------------------------------------------------------------------
#if JPC_DEBUG_RENDERER == 1
JPC_API JPC_BodyDrawFilter *
JPC_BodyDrawFilter_Create(const JPC_BodyDrawFilterFunc);

JPC_API void
JPC_BodyDrawFilter_Destroy(JPC_BodyDrawFilter *);
#endif // JPC_DEBUG_RENDERER
//--------------------------------------------------------------------------------------------------
//
// JPC_Shape
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Shape_AddRef(JPC_Shape *in_shape);

JPC_API void
JPC_Shape_Release(JPC_Shape *in_shape);

JPC_API uint32_t
JPC_Shape_GetRefCount(const JPC_Shape *in_shape);

JPC_API JPC_ShapeType
JPC_Shape_GetType(const JPC_Shape *in_shape);

JPC_API JPC_ShapeSubType
JPC_Shape_GetSubType(const JPC_Shape *in_shape);

JPC_API uint64_t
JPC_Shape_GetUserData(const JPC_Shape *in_shape);

JPC_API void
JPC_Shape_SetUserData(JPC_Shape *in_shape, uint64_t in_user_data);

JPC_API float
JPC_Shape_GetVolume(const JPC_Shape *in_shape);

JPC_API void
JPC_Shape_GetCenterOfMass(const JPC_Shape *in_shape, float out_position[3]);

JPC_API JPC_AABox
JPC_Shape_GetLocalBounds(const JPC_Shape *in_shape);

JPC_API void
JPC_Shape_GetSurfaceNormal(const JPC_Shape *in_shape,
                           JPC_SubShapeID in_sub_shape_id,
                           const float in_point[3],
                           float out_normal[3]);

JPC_API JPC_Shape_SupportingFace
JPC_Shape_GetSupportingFace(const JPC_Shape *in_shape,
                            JPC_SubShapeID in_sub_shape_id,
                            const float in_direction[3],
                            const float in_scale[3],
                            const float in_transform[16]);

JPC_API bool
JPC_Shape_CastRay(const JPC_Shape *in_shape,
                  const JPC_RayCast *in_ray,
                  const JPC_SubShapeIDCreator *in_id_creator,
                  JPC_RayCastResult *io_hit); // *Must* be default initialized (see JPC_RayCastResult)
//--------------------------------------------------------------------------------------------------
//
// JPC_BoxShape
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_BoxShape_GetHalfExtent(const JPC_BoxShape *in_shape, float out_half_extent[3]);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//
// JPC_ConvexHullShape
//
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_ConvexHullShape_GetNumPoints(const JPC_ConvexHullShape *in_shape);

JPC_API void
JPC_ConvexHullShape_GetPoint(const JPC_ConvexHullShape *in_shape, uint32_t in_point_index, float out_point[3]);

JPC_API uint32_t
JPC_ConvexHullShape_GetNumFaces(const JPC_ConvexHullShape *in_shape);

JPC_API uint32_t
JPC_ConvexHullShape_GetNumVerticesInFace(const JPC_ConvexHullShape *in_shape, uint32_t in_face_index);

JPC_API uint32_t
JPC_ConvexHullShape_GetFaceVertices(const JPC_ConvexHullShape *in_shape,
                                    uint32_t in_face_index,
                                    uint32_t in_max_vertices,
                                    uint32_t *out_vertices);
//--------------------------------------------------------------------------------------------------
//
// JPC_ConstraintSettings
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_ConstraintSettings_AddRef(JPC_ConstraintSettings *in_settings);

JPC_API void
JPC_ConstraintSettings_Release(JPC_ConstraintSettings *in_settings);

JPC_API uint32_t
JPC_ConstraintSettings_GetRefCount(const JPC_ConstraintSettings *in_settings);

JPC_API uint64_t
JPC_ConstraintSettings_GetUserData(const JPC_ConstraintSettings *in_settings);

JPC_API void
JPC_ConstraintSettings_SetUserData(JPC_ConstraintSettings *in_settings, uint64_t in_user_data);
//--------------------------------------------------------------------------------------------------
//
// JPC_TwoBodyConstraintSettings (-> JPC_ConstraintSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_Constraint *
JPC_TwoBodyConstraintSettings_CreateConstraint(const JPC_TwoBodyConstraintSettings *in_settings,
                                               JPC_Body *in_body1,
                                               JPC_Body *in_body2);
//--------------------------------------------------------------------------------------------------
//
// JPC_FixedConstraintSettings (-> JPC_TwoBodyConstraintSettings -> JPC_ConstraintSettings)
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_FixedConstraintSettings *
JPC_FixedConstraintSettings_Create();

JPC_API void
JPC_FixedConstraintSettings_SetSpace(JPC_FixedConstraintSettings *in_settings, JPC_ConstraintSpace in_space);

JPC_API void
JPC_FixedConstraintSettings_SetAutoDetectPoint(JPC_FixedConstraintSettings *in_settings, bool in_enabled);
//--------------------------------------------------------------------------------------------------
//
// JPC_Constraint
//
//--------------------------------------------------------------------------------------------------
JPC_API void
JPC_Constraint_AddRef(JPC_Constraint *in_shape);

JPC_API void
JPC_Constraint_Release(JPC_Constraint *in_shape);

JPC_API uint32_t
JPC_Constraint_GetRefCount(const JPC_Constraint *in_shape);

JPC_API JPC_ConstraintType
JPC_Constraint_GetType(const JPC_Constraint *in_shape);

JPC_API JPC_ConstraintSubType
JPC_Constraint_GetSubType(const JPC_Constraint *in_shape);

JPC_API uint64_t
JPC_Constraint_GetUserData(const JPC_Constraint *in_shape);

JPC_API void
JPC_Constraint_SetUserData(JPC_Constraint *in_shape, uint64_t in_user_data);
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyInterface
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_Body *
JPC_BodyInterface_CreateBody(JPC_BodyInterface *in_iface, const JPC_BodyCreationSettings *in_setting);

JPC_API JPC_Body *
JPC_BodyInterface_CreateBodyWithID(JPC_BodyInterface *in_iface,
                                   JPC_BodyID in_body_id,
                                   const JPC_BodyCreationSettings *in_settings);

JPC_API void
JPC_BodyInterface_DestroyBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API void
JPC_BodyInterface_AddBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, JPC_Activation in_mode);

JPC_API void
JPC_BodyInterface_RemoveBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API JPC_BodyID
JPC_BodyInterface_CreateAndAddBody(JPC_BodyInterface *in_iface,
                                   const JPC_BodyCreationSettings *in_settings,
                                   JPC_Activation in_mode);
JPC_API bool
JPC_BodyInterface_IsAdded(const JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API void
JPC_BodyInterface_SetLinearAndAngularVelocity(JPC_BodyInterface *in_iface,
                                              JPC_BodyID in_body_id,
                                              const float in_linear_velocity[3],
                                              const float in_angular_velocity[3]);
JPC_API void
JPC_BodyInterface_GetLinearAndAngularVelocity(const JPC_BodyInterface *in_iface,
                                              JPC_BodyID in_body_id,
                                              float out_linear_velocity[3],
                                              float out_angular_velocity[3]);
JPC_API void
JPC_BodyInterface_SetLinearVelocity(JPC_BodyInterface *in_iface,
                                    JPC_BodyID in_body_id,
                                    const float in_velocity[3]);
JPC_API void
JPC_BodyInterface_GetLinearVelocity(const JPC_BodyInterface *in_iface,
                                    JPC_BodyID in_body_id,
                                    float out_velocity[3]);
JPC_API void
JPC_BodyInterface_AddLinearVelocity(JPC_BodyInterface *in_iface,
                                    JPC_BodyID in_body_id,
                                    const float in_velocity[3]);
JPC_API void
JPC_BodyInterface_AddLinearAndAngularVelocity(JPC_BodyInterface *in_iface,
                                              JPC_BodyID in_body_id,
                                              const float in_linear_velocity[3],
                                              const float in_angular_velocity[3]);
JPC_API void
JPC_BodyInterface_SetAngularVelocity(JPC_BodyInterface *in_iface,
                                     JPC_BodyID in_body_id,
                                     const float in_velocity[3]);
JPC_API void
JPC_BodyInterface_GetAngularVelocity(const JPC_BodyInterface *in_iface,
                                     JPC_BodyID in_body_id,
                                     float out_velocity[3]);
JPC_API void
JPC_BodyInterface_GetPointVelocity(const JPC_BodyInterface *in_iface,
                                   JPC_BodyID in_body_id,
                                   const JPC_Real in_point[3],
                                   float out_velocity[3]);
JPC_API void
JPC_BodyInterface_GetPosition(const JPC_BodyInterface *in_iface,
                              JPC_BodyID in_body_id,
                              JPC_Real out_position[3]);
JPC_API void
JPC_BodyInterface_SetPosition(JPC_BodyInterface *in_iface,
                              JPC_BodyID in_body_id,
                              const JPC_Real in_position[3],
                              JPC_Activation in_activation);
JPC_API void
JPC_BodyInterface_GetCenterOfMassPosition(const JPC_BodyInterface *in_iface,
                                          JPC_BodyID in_body_id,
                                          JPC_Real out_position[3]);
JPC_API void
JPC_BodyInterface_GetRotation(const JPC_BodyInterface *in_iface,
                              JPC_BodyID in_body_id,
                              float out_rotation[4]);
JPC_API void
JPC_BodyInterface_SetRotation(JPC_BodyInterface *in_iface,
                              JPC_BodyID in_body_id,
                              const float in_rotation[4],
                              JPC_Activation in_activation);
JPC_API void
JPC_BodyInterface_ActivateBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API void
JPC_BodyInterface_DeactivateBody(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API bool
JPC_BodyInterface_IsActive(const JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API void
JPC_BodyInterface_SetPositionRotationAndVelocity(JPC_BodyInterface *in_iface,
                                                 JPC_BodyID in_body_id,
                                                 const JPC_Real in_position[3],
                                                 const float in_rotation[4],
                                                 const float in_linear_velocity[3],
                                                 const float in_angular_velocity[3]);
JPC_API void
JPC_BodyInterface_AddForce(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, const float in_force[3]);

JPC_API void
JPC_BodyInterface_AddForceAtPosition(JPC_BodyInterface *in_iface,
                                     JPC_BodyID in_body_id,
                                     const float in_force[3],
                                     const JPC_Real in_position[3]);
JPC_API void
JPC_BodyInterface_AddTorque(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, const float in_torque[3]);

JPC_API void
JPC_BodyInterface_AddForceAndTorque(JPC_BodyInterface *in_iface,
                                    JPC_BodyID in_body_id,
                                    const float in_force[3],
                                    const float in_torque[3]);
JPC_API void
JPC_BodyInterface_AddImpulse(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, const float in_impulse[3]);

JPC_API void
JPC_BodyInterface_AddImpulseAtPosition(JPC_BodyInterface *in_iface,
                                       JPC_BodyID in_body_id,
                                       const float in_impulse[3],
                                       const JPC_Real in_position[3]);
JPC_API void
JPC_BodyInterface_AddAngularImpulse(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, const float in_impulse[3]);

JPC_API JPC_MotionType
JPC_BodyInterface_GetMotionType(const JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API void
JPC_BodyInterface_SetMotionType(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, JPC_MotionType motion_type, JPC_Activation activation);

JPC_API JPC_ObjectLayer
JPC_BodyInterface_GetObjectLayer(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id);

JPC_API void
JPC_BodyInterface_SetObjectLayer(JPC_BodyInterface *in_iface, JPC_BodyID in_body_id, JPC_ObjectLayer in_layer);
//--------------------------------------------------------------------------------------------------
//
// JPC_Body
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_BodyID
JPC_Body_GetID(const JPC_Body *in_body);

JPC_API bool
JPC_Body_IsActive(const JPC_Body *in_body);

JPC_API bool
JPC_Body_IsStatic(const JPC_Body *in_body);

JPC_API bool
JPC_Body_IsKinematic(const JPC_Body *in_body);

JPC_API bool
JPC_Body_IsDynamic(const JPC_Body *in_body);

JPC_API bool
JPC_Body_CanBeKinematicOrDynamic(const JPC_Body *in_body);

JPC_API void
JPC_Body_SetIsSensor(JPC_Body *in_body, bool in_is_sensor);

JPC_API bool
JPC_Body_IsSensor(const JPC_Body *in_body);

JPC_API JPC_MotionType
JPC_Body_GetMotionType(const JPC_Body *in_body);

JPC_API void
JPC_Body_SetMotionType(JPC_Body *in_body, JPC_MotionType in_motion_type);

JPC_API JPC_BroadPhaseLayer
JPC_Body_GetBroadPhaseLayer(const JPC_Body *in_body);

JPC_API JPC_ObjectLayer
JPC_Body_GetObjectLayer(const JPC_Body *in_body);

JPC_API JPC_CollisionGroup *
JPC_Body_GetCollisionGroup(JPC_Body *in_body);

JPC_API void
JPC_Body_SetCollisionGroup(JPC_Body *in_body, const JPC_CollisionGroup *in_group);

JPC_API bool
JPC_Body_GetAllowSleeping(const JPC_Body *in_body);

JPC_API void
JPC_Body_SetAllowSleeping(JPC_Body *in_body, bool in_allow_sleeping);

JPC_API float
JPC_Body_GetFriction(const JPC_Body *in_body);

JPC_API void
JPC_Body_SetFriction(JPC_Body *in_body, float in_friction);

JPC_API float
JPC_Body_GetRestitution(const JPC_Body *in_body);

JPC_API void
JPC_Body_SetRestitution(JPC_Body *in_body, float in_restitution);

JPC_API void
JPC_Body_GetLinearVelocity(const JPC_Body *in_body, float out_linear_velocity[3]);

JPC_API void
JPC_Body_SetLinearVelocity(JPC_Body *in_body, const float in_linear_velocity[3]);

JPC_API void
JPC_Body_SetLinearVelocityClamped(JPC_Body *in_body, const float in_linear_velocity[3]);

JPC_API void
JPC_Body_GetAngularVelocity(const JPC_Body *in_body, float out_angular_velocity[3]);

JPC_API void
JPC_Body_SetAngularVelocity(JPC_Body *in_body, const float in_angular_velocity[3]);

JPC_API void
JPC_Body_SetAngularVelocityClamped(JPC_Body *in_body, const float in_angular_velocity[3]);

JPC_API void
JPC_Body_GetPointVelocityCOM(const JPC_Body *in_body,
                             const float in_point_relative_to_com[3],
                             float out_velocity[3]);
JPC_API void
JPC_Body_GetPointVelocity(const JPC_Body *in_body, const JPC_Real in_point[3], float out_velocity[3]);

JPC_API void
JPC_Body_AddForce(JPC_Body *in_body, const float in_force[3]);

JPC_API void
JPC_Body_AddForceAtPosition(JPC_Body *in_body, const float in_force[3], const JPC_Real in_position[3]);

JPC_API void
JPC_Body_AddTorque(JPC_Body *in_body, const float in_torque[3]);

JPC_API void
JPC_Body_GetInverseInertia(const JPC_Body *in_body, float out_inverse_inertia[16]);

JPC_API void
JPC_Body_AddImpulse(JPC_Body *in_body, const float in_impulse[3]);

JPC_API void
JPC_Body_AddImpulseAtPosition(JPC_Body *in_body, const float in_impulse[3], const JPC_Real in_position[3]);

JPC_API void
JPC_Body_AddAngularImpulse(JPC_Body *in_body, const float in_angular_impulse[3]);

JPC_API void
JPC_Body_MoveKinematic(JPC_Body *in_body,
                       const JPC_Real in_target_position[3],
                       const float in_target_rotation[4],
                       float in_delta_time);
JPC_API void
JPC_Body_ApplyBuoyancyImpulse(JPC_Body *in_body,
                              const JPC_Real in_surface_position[3],
                              const float in_surface_normal[3],
                              float in_buoyancy,
                              float in_linear_drag,
                              float in_angular_drag,
                              const float in_fluid_velocity[3],
                              const float in_gravity[3],
                              float in_delta_time);
JPC_API bool
JPC_Body_IsInBroadPhase(const JPC_Body *in_body);

JPC_API bool
JPC_Body_IsCollisionCacheInvalid(const JPC_Body *in_body);

JPC_API const JPC_Shape *
JPC_Body_GetShape(const JPC_Body *in_body);

JPC_API void
JPC_Body_GetPosition(const JPC_Body *in_body, JPC_Real out_position[3]);

JPC_API void
JPC_Body_GetRotation(const JPC_Body *in_body, float out_rotation[4]);

JPC_API void
JPC_Body_GetWorldTransform(const JPC_Body *in_body, float out_rotation[9], JPC_Real out_translation[3]);

JPC_API void
JPC_Body_GetCenterOfMassPosition(const JPC_Body *in_body, JPC_Real out_position[3]);

JPC_API void
JPC_Body_GetCenterOfMassTransform(const JPC_Body *in_body,
                                  float out_rotation[9],
                                  JPC_Real out_translation[3]);
JPC_API void
JPC_Body_GetInverseCenterOfMassTransform(const JPC_Body *in_body,
                                         float out_rotation[9],
                                         JPC_Real out_translation[3]);
JPC_API void
JPC_Body_GetWorldSpaceBounds(const JPC_Body *in_body, float out_min[3], float out_max[3]);

JPC_API JPC_MotionProperties *
JPC_Body_GetMotionProperties(JPC_Body *in_body);

JPC_API uint64_t
JPC_Body_GetUserData(const JPC_Body *in_body);

JPC_API void
JPC_Body_SetUserData(JPC_Body *in_body, uint64_t in_user_data);

JPC_API void
JPC_Body_GetWorldSpaceSurfaceNormal(const JPC_Body *in_body,
                                    JPC_SubShapeID in_sub_shape_id,
                                    const JPC_Real in_position[3], // world space
                                    float out_normal_vector[3]);
//--------------------------------------------------------------------------------------------------
//
// JPC_BodyID
//
//--------------------------------------------------------------------------------------------------
JPC_API uint32_t
JPC_BodyID_GetIndex(JPC_BodyID in_body_id);

JPC_API uint8_t
JPC_BodyID_GetSequenceNumber(JPC_BodyID in_body_id);

JPC_API bool
JPC_BodyID_IsInvalid(JPC_BodyID in_body_id);
//--------------------------------------------------------------------------------------------------
//
// JPC_CharacterSettings
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CharacterSettings *
JPC_CharacterSettings_Create();

JPC_API void
JPC_CharacterSettings_Release(JPC_CharacterSettings *in_settings);

JPC_API void
JPC_CharacterSettings_AddRef(JPC_CharacterSettings *in_settings);
//--------------------------------------------------------------------------------------------------
//
// JPC_Character
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_Character *
JPC_Character_Create(const JPC_CharacterSettings *in_settings,
                     const JPC_Real in_position[3],
                     const float in_rotation[4],
                     uint64_t in_user_data,
                     JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_Character_Destroy(JPC_Character *in_character);

JPC_API void
JPC_Character_AddToPhysicsSystem(JPC_Character *in_character, JPC_Activation in_activation, bool in_lock_bodies);

JPC_API void
JPC_Character_RemoveFromPhysicsSystem(JPC_Character *in_character, bool in_lock_bodies);

JPC_API void
JPC_Character_GetPosition(const JPC_Character *in_character, JPC_Real out_position[3]);

JPC_API void
JPC_Character_SetPosition(JPC_Character *in_character, const JPC_Real in_position[3]);

JPC_API void
JPC_Character_GetLinearVelocity(const JPC_Character *in_character, float out_linear_velocity[3]);

JPC_API void
JPC_Character_SetLinearVelocity(JPC_Character *in_character, const float in_linear_velocity[3]);
//--------------------------------------------------------------------------------------------------
//
// JPC_CharacterVirtualSettings
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CharacterVirtualSettings *
JPC_CharacterVirtualSettings_Create();

JPC_API void
JPC_CharacterVirtualSettings_Release(JPC_CharacterVirtualSettings *in_settings);
//--------------------------------------------------------------------------------------------------
//
// JPC_CharacterVirtual
//
//--------------------------------------------------------------------------------------------------
JPC_API JPC_CharacterVirtual *
JPC_CharacterVirtual_Create(const JPC_CharacterVirtualSettings *in_settings,
                            const JPC_Real in_position[3],
                            const float in_rotation[4],
                            JPC_PhysicsSystem *in_physics_system);

JPC_API void
JPC_CharacterVirtual_Destroy(JPC_CharacterVirtual *in_character);

JPC_API void
JPC_CharacterVirtual_Update(JPC_CharacterVirtual *in_character,
                            float in_delta_time,
                            const float in_gravity[3],
                            const void *in_broad_phase_layer_filter,
                            const void *in_object_layer_filter,
                            const void *in_body_filter,
                            const void *in_shape_filter,
                            JPC_TempAllocator *in_temp_allocator);

JPC_API void
JPC_CharacterVirtual_SetListener(JPC_CharacterVirtual *in_character, void *in_listener);

JPC_API void
JPC_CharacterVirtual_UpdateGroundVelocity(JPC_CharacterVirtual *in_character);

JPC_API void
JPC_CharacterVirtual_GetGroundVelocity(const JPC_CharacterVirtual *in_character, float out_ground_velocity[3]);

JPC_API JPC_CharacterGroundState
JPC_CharacterVirtual_GetGroundState(JPC_CharacterVirtual *in_character);

JPC_API void
JPC_CharacterVirtual_GetPosition(const JPC_CharacterVirtual *in_character, JPC_Real out_position[3]);

JPC_API void
JPC_CharacterVirtual_SetPosition(JPC_CharacterVirtual *in_character, const JPC_Real in_position[3]);

JPC_API void
JPC_CharacterVirtual_GetRotation(const JPC_CharacterVirtual *in_character, float out_rotation[4]);

JPC_API void
JPC_CharacterVirtual_SetRotation(JPC_CharacterVirtual *in_character, const float in_rotation[4]);

JPC_API void
JPC_CharacterVirtual_GetLinearVelocity(const JPC_CharacterVirtual *in_character, float out_linear_velocity[3]);

JPC_API void
JPC_CharacterVirtual_SetLinearVelocity(JPC_CharacterVirtual *in_character, const float in_linear_velocity[3]);
//--------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
