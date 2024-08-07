#include "imgui.h"

#include "imgui_te_engine.h"
#include "imgui_te_context.h"
#include "imgui_te_ui.h"
#include "imgui_te_utils.h"
#include "imgui_te_exporters.h"

#include "imgui_internal.h"

#ifndef ZGUI_API
#define ZGUI_API
#endif

//--------------------------------------------------------------------------------------------------
//
// ImGUI Test Engine
//
//--------------------------------------------------------------------------------------------------
extern "C"
{

    ZGUI_API void *zguiTe_CreateContext(void)
    {
        ImGuiTestEngine *e = ImGuiTestEngine_CreateContext();

        ImGuiTestEngine_Start(e, ImGui::GetCurrentContext());
        ImGuiTestEngine_InstallDefaultCrashHandler();

        return e;
    }

    ZGUI_API void zguiTe_DestroyContext(ImGuiTestEngine *engine)
    {
        ImGuiTestEngine_DestroyContext(engine);
    }

    ZGUI_API void zguiTe_EngineSetRunSpeed(ImGuiTestEngine *engine, ImGuiTestRunSpeed speed)
    {
        ImGuiTestEngine_GetIO(engine).ConfigRunSpeed = speed;
    }

    ZGUI_API void zguiTe_EngineExportJunitResult(ImGuiTestEngine *engine, const char *filename)
    {
        ImGuiTestEngine_GetIO(engine).ExportResultsFilename = filename;
        ImGuiTestEngine_GetIO(engine).ExportResultsFormat = ImGuiTestEngineExportFormat_JUnitXml;
    }

    ZGUI_API void zguiTe_TryAbortEngine(ImGuiTestEngine *engine)
    {
        ImGuiTestEngine_TryAbortEngine(engine);
    }

    ZGUI_API void zguiTe_Stop(ImGuiTestEngine *engine)
    {
        ImGuiTestEngine_Stop(engine);
    }

    ZGUI_API void zguiTe_PostSwap(ImGuiTestEngine *engine)
    {
        ImGuiTestEngine_PostSwap(engine);
    }

    ZGUI_API bool zguiTe_IsTestQueueEmpty(ImGuiTestEngine *engine)
    {
        return ImGuiTestEngine_IsTestQueueEmpty(engine);
    }

    ZGUI_API void zguiTe_GetResult(ImGuiTestEngine *engine, int *count_tested, int *count_success)
    {
        int ct = 0;
        int cs = 0;
        ImGuiTestEngine_GetResult(engine, ct, cs);
        *count_tested = ct;
        *count_success = cs;
    }

    ZGUI_API void zguiTe_PrintResultSummary(ImGuiTestEngine *engine)
    {
        ImGuiTestEngine_PrintResultSummary(engine);
    }

    ZGUI_API void zguiTe_QueueTests(ImGuiTestEngine *engine, ImGuiTestGroup group, const char *filter_str, ImGuiTestRunFlags run_flags)
    {
        ImGuiTestEngine_QueueTests(engine, group, filter_str, run_flags);
    }

    ZGUI_API void zguiTe_ShowTestEngineWindows(ImGuiTestEngine *engine, bool *p_open)
    {
        ImGuiTestEngine_ShowTestEngineWindows(engine, p_open);
    }

    ZGUI_API void *zguiTe_RegisterTest(ImGuiTestEngine *engine, const char *category, const char *name, const char *src_file, int src_line, ImGuiTestGuiFunc *gui_fce, ImGuiTestTestFunc *gui_test_fce)
    {
        auto t = ImGuiTestEngine_RegisterTest(engine, category, name, src_file, src_line);
        t->GuiFunc = gui_fce;
        t->TestFunc = gui_test_fce;
        return t;
    }

    ZGUI_API bool zguiTe_Check(const char *file, const char *func, int line, ImGuiTestCheckFlags flags, bool result, const char *expr)
    {
        return ImGuiTestEngine_Check(file, func, line, flags, result, expr);
    }

    // CONTEXT

    ZGUI_API void zguiTe_ContextSetRef(ImGuiTestContext *ctx, const char *ref)
    {
        ctx->SetRef(ref);
    }

    ZGUI_API void zguiTe_ContextWindowFocus(ImGuiTestContext *ctx, const char *ref)
    {
        ctx->WindowFocus(ref);
    }

    ZGUI_API void zguiTe_ContextItemAction(ImGuiTestContext *ctx, ImGuiTestAction action, const char *ref, ImGuiTestOpFlags flags = 0, void *action_arg = NULL)
    {
        ctx->ItemAction(action, ref, flags, action_arg);
    }

    ZGUI_API void zguiTe_ContextItemInputStrValue(ImGuiTestContext *ctx, const char *ref, const char *value)
    {
        ctx->ItemInputValue(ref, value);
    }

    ZGUI_API void zguiTe_ContextItemInputIntValue(ImGuiTestContext *ctx, const char *ref, int value)
    {
        ctx->ItemInputValue(ref, value);
    }

    ZGUI_API void zguiTe_ContextItemInputFloatValue(ImGuiTestContext *ctx, const char *ref, float value)
    {
        ctx->ItemInputValue(ref, value);
    }

    ZGUI_API void zguiTe_ContextYield(ImGuiTestContext *ctx, int frame_count)
    {
        ctx->Yield(frame_count);
    }

    ZGUI_API void zguiTe_ContextMenuAction(ImGuiTestContext *ctx, ImGuiTestAction action, const char *ref)
    {
        ctx->MenuAction(action, ref);
    }

    ZGUI_API void zguiTe_ContextDragAndDrop(ImGuiTestContext *ctx, const char *ref_src, const char *ref_dst, ImGuiMouseButton button)
    {
        ctx->ItemDragAndDrop(ref_src, ref_dst, button);
    }

    ZGUI_API void zguiTe_ContextKeyDown(ImGuiTestContext *ctx, ImGuiKeyChord key_chord)
    {
        ctx->KeyDown(key_chord);
    }

    ZGUI_API void zguiTe_ContextKeyUp(ImGuiTestContext *ctx, ImGuiKeyChord key_chord)
    {
        ctx->KeyUp(key_chord);
    }

} /* extern "C" */
