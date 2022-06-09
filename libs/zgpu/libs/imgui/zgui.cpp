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
ZGUI_API bool zguiComboStr(
    const char* label,
    int* current_item,
    const char* items_separated_by_zeros,
    int popup_max_height_in_items
) {
    return ImGui::Combo(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
}
ZGUI_API bool zguiSliderFloat(
    const char* label,
    float* v,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderFloat(label, v, v_min, v_max, format, flags);
}
ZGUI_API void zguiBulletText(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::BulletTextV(fmt, args);
    va_end(args);
}
