#include "./imgui/imgui.h"
#include "./imgui/imgui_internal.h"

#define ZGUI_API extern "C"

ZGUI_API bool zguiButton(const char* label, float x, float y) { return ImGui::Button(label, { x, y }); }
ZGUI_API bool zguiBegin(const char* name, bool* p_open, ImGuiWindowFlags flags) {
    return ImGui::Begin(name, p_open, flags);
}
ZGUI_API void zguiEnd() { ImGui::End(); }
ZGUI_API void zguiSpacing() { ImGui::Spacing(); }
ZGUI_API void zguiNewLine() { ImGui::NewLine(); }
ZGUI_API void zguiSeparator() { ImGui::Separator(); }
ZGUI_API void zguiSameLine(float offset_from_start_x, float spacing) {
    ImGui::SameLine(offset_from_start_x, spacing);
}
ZGUI_API void zguiDummy(float w, float h) { ImGui::Dummy({ w, h }); }
