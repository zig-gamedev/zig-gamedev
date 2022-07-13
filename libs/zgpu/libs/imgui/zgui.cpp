#include "./imgui/imgui.h"

#define ZGUI_API extern "C"



ZGUI_API bool zguiBegin(const char* name, bool* p_open, ImGuiWindowFlags flags) {
    return ImGui::Begin(name, p_open, flags);
}

ZGUI_API void zguiEnd(void) {
    ImGui::End();
}

ZGUI_API void zguiSpacing(void) {
    ImGui::Spacing();
}

ZGUI_API void zguiNewLine(void) {
    ImGui::NewLine();
}

ZGUI_API void zguiSeparator(void) {
    ImGui::Separator();
}

ZGUI_API void zguiSameLine(float offset_from_start_x, float spacing) {
    ImGui::SameLine(offset_from_start_x, spacing);
}

ZGUI_API void zguiDummy(float w, float h) {
    ImGui::Dummy({ w, h });
}

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

ZGUI_API bool zguiSliderInt(
    const char* label,
    int* v,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderInt(label, v, v_min, v_max, format, flags);
}

// Widgets: Text

ZGUI_API void zguiTextUnformatted(const char* text, const char* text_end) {
    ImGui::TextUnformatted(text, text_end);
}

ZGUI_API void zguiTextColored(float color[4], const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextColoredV(ImVec4(color[0], color[1], color[2], color[3]), fmt, args);
    va_end(args);
}

ZGUI_API void zguiTextDisabled(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextDisabledV(fmt, args);
    va_end(args);
}

ZGUI_API void zguiTextWrapped(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextWrappedV(fmt, args);
    va_end(args);
}

ZGUI_API void zguiBulletText(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::BulletTextV(fmt, args);
    va_end(args);
}

ZGUI_API void zguiLabelText(const char* label, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::LabelTextV(label, fmt, args);
    va_end(args);
}

// Widgets: Main

ZGUI_API bool zguiButton(const char* label, float x, float y) {
    return ImGui::Button(label, { x, y });
}

ZGUI_API bool zguiSmallButton(const char* label) {
    return ImGui::SmallButton(label);
}

ZGUI_API bool zguiInvisibleButton(const char* str_id, float w, float h, ImGuiButtonFlags flags) {
    return ImGui::InvisibleButton(str_id, ImVec2(w, h), flags);
}

ZGUI_API bool zguiArrowButton(const char* label, ImGuiDir dir) {
    return ImGui::ArrowButton(label, dir);
}

ZGUI_API void zguiBullet(void) {
    ImGui::Bullet();
}


ZGUI_API bool zguiRadioButtonIntPtr(const char* label, int* v, int v_button) {
    return ImGui::RadioButton(label, v, v_button);
}

ZGUI_API bool zguiCheckbox(const char* label, bool* v) {
    return ImGui::Checkbox(label, v);
}

ZGUI_API ImGuiContext* zguiCreateContext(ImFontAtlas* shared_font_atlas) {
    return ImGui::CreateContext(shared_font_atlas);
}

ZGUI_API void zguiDestroyContext(ImGuiContext* ctx) {
    ImGui::DestroyContext(ctx);
}

ZGUI_API ImGuiContext* zguiGetCurrentContext(void) {
    return ImGui::GetCurrentContext();
}

ZGUI_API void zguiSetCurrentContext(ImGuiContext* ctx) {
    ImGui::SetCurrentContext(ctx);
}

ZGUI_API void zguiNewFrame(void) {
    ImGui::NewFrame();
}

ZGUI_API void zguiRender(void) {
    ImGui::Render();
}

ZGUI_API ImDrawData* zguiGetDrawData(void) {
    return ImGui::GetDrawData();
}

ZGUI_API void zguiShowDemoWindow(bool* p_open) {
    ImGui::ShowDemoWindow(p_open);
}

ZGUI_API void zguiBeginDisabled(bool disabled) {
    ImGui::BeginDisabled(disabled);
}

ZGUI_API void zguiEndDisabled(void) {
    ImGui::EndDisabled();
}

ZGUI_API void zguiPushStyleColor(ImGuiCol_ idx, float color[4]) {
    ImGui::PushStyleColor(idx, ImVec4(color[0], color[1], color[2], color[3]));
}

ZGUI_API void zguiPopStyleColor(int count) {
    ImGui::PopStyleColor(count);
}

ZGUI_API bool zguiIoGetWantCaptureMouse(void) {
    return ImGui::GetIO().WantCaptureMouse;
}

ZGUI_API bool zguiIoGetWantCaptureKeyboard(void) {
    return ImGui::GetIO().WantCaptureKeyboard;
}

ZGUI_API void zguiIoAddFontFromFile(const char* filename, float size_pixels) {
    ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size_pixels, nullptr, nullptr);
}

ZGUI_API void zguiIoSetIniFilename(const char* filename) {
    ImGui::GetIO().IniFilename = filename;
}

ZGUI_API void zguiIoSetDisplaySize(float width, float height) {
    ImGui::GetIO().DisplaySize = { width, height };
}

ZGUI_API void zguiIoSetDisplayFramebufferScale(float sx, float sy) {
    ImGui::GetIO().DisplayFramebufferScale = { sx, sy };
}
