#include "imgui.h"

#include "ImGuizmo.h"

#include "imgui_internal.h"

#ifndef ZGUI_API
#define ZGUI_API
#endif

//--------------------------------------------------------------------------------------------------
//
// ImGuizmo
//
//--------------------------------------------------------------------------------------------------
extern "C"
{

    ZGUI_API void zguiGizmo_SetDrawlist(ImDrawList *drawlist)
    {
        ImGuizmo::SetDrawlist(drawlist);
    }

    ZGUI_API void zguiGizmo_BeginFrame()
    {
        ImGuizmo::BeginFrame();
    }

    ZGUI_API void zguiGizmo_SetImGuiContext(ImGuiContext *ctx)
    {
        ImGuizmo::SetImGuiContext(ctx);
    }

    ZGUI_API bool zguiGizmo_IsOver()
    {
        return ImGuizmo::IsOver();
    }

    ZGUI_API bool zguiGizmo_IsUsing()
    {
        return ImGuizmo::IsUsing();
    }

    ZGUI_API bool zguiGizmo_IsUsingAny()
    {
        return ImGuizmo::IsUsingAny();
    }

    ZGUI_API void zguiGizmo_Enable(bool enable)
    {
        ImGuizmo::Enable(enable);
    }

    ZGUI_API void zguiGizmo_DecomposeMatrixToComponents(
        const float *matrix,
        float *translation,
        float *rotation,
        float *scale)
    {
        ImGuizmo::DecomposeMatrixToComponents(matrix, translation, rotation, scale);
    }

    ZGUI_API void zguiGizmo_RecomposeMatrixFromComponents(
        const float *translation,
        const float *rotation,
        const float *scale,
        float *matrix)
    {
        ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, matrix);
    }

    ZGUI_API void zguiGizmo_SetRect(float x, float y, float width, float height)
    {
        ImGuizmo::SetRect(x, y, width, height);
    }

    ZGUI_API void zguiGizmo_SetOrthographic(bool isOrthographic)
    {
        ImGuizmo::SetOrthographic(isOrthographic);
    }

    ZGUI_API void zguiGizmo_DrawCubes(const float *view, const float *projection, const float *matrices, int matrixCount)
    {
        ImGuizmo::DrawCubes(view, projection, matrices, matrixCount);
    }

    ZGUI_API void zguiGizmo_DrawGrid(
        const float *view,
        const float *projection,
        const float *matrix,
        const float gridSize)
    {
        ImGuizmo::DrawGrid(view, projection, matrix, gridSize);
    }

    ZGUI_API bool zguiGizmo_Manipulate(
        const float *view,
        const float *projection,
        ImGuizmo::OPERATION operation,
        ImGuizmo::MODE mode,
        float *matrix,
        float *deltaMatrix = NULL,
        const float *snap = NULL,
        const float *localBounds = NULL,
        const float *boundsSnap = NULL)
    {
        return ImGuizmo::Manipulate(view, projection, operation, mode, matrix, deltaMatrix, snap, localBounds, boundsSnap);
    }

    //
    // Please note that this cubeview is patented by Autodesk : https://patents.google.com/patent/US7782319B2/en
    // It seems to be a defensive patent in the US. I don't think it will bring troubles using it as
    // other software are using the same mechanics. But just in case, you are now warned!
    //
    ZGUI_API void zguiGizmo_ViewManipulate(
        float *view,
        float length,
        const float position[2],
        const float size[2],
        ImU32 backgroundColor)
    {
        const ImVec2 p(position[0], position[1]);
        const ImVec2 s(size[0], size[1]);
        ImGuizmo::ViewManipulate(view, length, p, s, backgroundColor);
    }
    // use this version if you did not call Manipulate before and you are just using ViewManipulate
    ZGUI_API void zguiGizmo_ViewManipulateIndependent(
        float *view,
        const float *projection,
        ImGuizmo::OPERATION operation,
        ImGuizmo::MODE mode,
        float *matrix,
        float length,
        const float position[2],
        const float size[2],
        ImU32 backgroundColor)
    {
        const ImVec2 p(position[0], position[1]);
        const ImVec2 s(size[0], size[1]);
        ImGuizmo::ViewManipulate(view, projection, operation, mode, matrix, length, p, s, backgroundColor);
    }

    ZGUI_API void zguiGizmo_SetID(int id)
    {
        ImGuizmo::SetID(id);
    }

    ZGUI_API bool zguiGizmo_IsOverOperation(ImGuizmo::OPERATION op)
    {
        return ImGuizmo::IsOver(op);
    }

    ZGUI_API void zguiGizmo_AllowAxisFlip(bool value)
    {
        ImGuizmo::AllowAxisFlip(value);
    }

    ZGUI_API void zguiGizmo_SetAxisLimit(float value)
    {
        ImGuizmo::SetAxisLimit(value);
    }

    ZGUI_API void zguiGizmo_SetPlaneLimit(float value)
    {
        ImGuizmo::SetPlaneLimit(value);
    }

    ZGUI_API ImGuizmo::Style *zguiGizmo_GetStyle()
    {
        return &ImGuizmo::GetStyle();
    }

} /* extern "C" */