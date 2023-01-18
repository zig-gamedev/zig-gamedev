#include "./imgui/imgui.h"
#include "./imgui/implot.h"

#define ZGUI_API extern "C"

/*
#include <stdio.h>

ZGUI_API float zguiGetFloatMin(void) {
    printf("__FLT_MIN__ %.32e\n", __FLT_MIN__);
    return __FLT_MIN__;
}

ZGUI_API float zguiGetFloatMax(void) {
    printf("__FLT_MAX__ %.32e\n", __FLT_MAX__);
    return __FLT_MAX__;
}
*/

ZGUI_API void zguiSetAllocatorFunctions(
    void* (*alloc_func)(size_t, void*),
    void (*free_func)(void*, void*)
) {
    ImGui::SetAllocatorFunctions(alloc_func, free_func, nullptr);
}

ZGUI_API void zguiSetNextWindowPos(float x, float y, ImGuiCond cond, float pivot_x, float pivot_y) {
    ImGui::SetNextWindowPos({ x, y }, cond, { pivot_x, pivot_y });
}

ZGUI_API void zguiSetNextWindowSize(float w, float h, ImGuiCond cond) {
    ImGui::SetNextWindowSize({ w, h }, cond);
}

ZGUI_API void zguiSetNextWindowCollapsed(bool collapsed, ImGuiCond cond) {
    ImGui::SetNextWindowCollapsed(collapsed, cond);
}

ZGUI_API void zguiSetNextWindowFocus(void) {
    ImGui::SetNextWindowFocus();
}

ZGUI_API void zguiSetNextWindowBgAlpha(float alpha) {
    ImGui::SetNextWindowBgAlpha(alpha);
}

ZGUI_API void zguiSetKeyboardFocusHere(int offset) {
    ImGui::SetKeyboardFocusHere(offset);
}

ZGUI_API bool zguiBegin(const char* name, bool* p_open, ImGuiWindowFlags flags) {
    return ImGui::Begin(name, p_open, flags);
}

ZGUI_API void zguiEnd(void) {
    ImGui::End();
}

ZGUI_API bool zguiBeginChild(const char* str_id, float w, float h, bool border, ImGuiWindowFlags flags) {
    return ImGui::BeginChild(str_id, { w, h }, border, flags);
}

ZGUI_API bool zguiBeginChildId(ImGuiID id, float w, float h, bool border, ImGuiWindowFlags flags) {
    return ImGui::BeginChild(id, { w, h }, border, flags);
}

ZGUI_API void zguiEndChild(void) {
    ImGui::EndChild();
}

ZGUI_API float zguiGetScrollX(void) {
    return ImGui::GetScrollX();
}

ZGUI_API float zguiGetScrollY(void) {
    return ImGui::GetScrollY();
}

ZGUI_API void zguiSetScrollX(float scroll_x) {
    ImGui::SetScrollX(scroll_x);
}

ZGUI_API void zguiSetScrollY(float scroll_y) {
    ImGui::SetScrollY(scroll_y);
}

ZGUI_API float zguiGetScrollMaxX(void) {
    return ImGui::GetScrollMaxX();
}

ZGUI_API float zguiGetScrollMaxY(void) {
    return ImGui::GetScrollMaxY();
}

ZGUI_API void zguiSetScrollHereX(float center_x_ratio) {
    ImGui::SetScrollHereX(center_x_ratio);
}

ZGUI_API void zguiSetScrollHereY(float center_y_ratio) {
    ImGui::SetScrollHereY(center_y_ratio);
}

ZGUI_API void zguiSetScrollFromPosX(float local_x, float center_x_ratio) {
    ImGui::SetScrollFromPosX(local_x, center_x_ratio);
}

ZGUI_API void zguiSetScrollFromPosY(float local_y, float center_y_ratio) {
    ImGui::SetScrollFromPosY(local_y, center_y_ratio);
}

ZGUI_API bool zguiIsWindowAppearing(void) {
    return ImGui::IsWindowAppearing();
}

ZGUI_API bool zguiIsWindowCollapsed(void) {
    return ImGui::IsWindowCollapsed();
}

ZGUI_API bool zguiIsWindowFocused(ImGuiFocusedFlags flags) {
    return ImGui::IsWindowFocused(flags);
}

ZGUI_API bool zguiIsWindowHovered(ImGuiHoveredFlags flags) {
    return ImGui::IsWindowHovered(flags);
}

ZGUI_API void zguiGetWindowPos(float pos[2]) {
    const ImVec2 p = ImGui::GetWindowPos();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API void zguiGetWindowSize(float size[2]) {
    const ImVec2 s = ImGui::GetWindowSize();
    size[0] = s.x;
    size[1] = s.y;
}

ZGUI_API float zguiGetWindowWidth(void) {
    return ImGui::GetWindowWidth();
}

ZGUI_API float zguiGetWindowHeight(void) {
    return ImGui::GetWindowHeight();
}

ZGUI_API void zguiGetMouseDragDelta(ImGuiMouseButton button, float lock_threshold, float delta[2]) {
    const ImVec2 d = ImGui::GetMouseDragDelta(button, lock_threshold);
    delta[0] = d.x;
    delta[1] = d.y;
}

ZGUI_API void zguiResetMouseDragDelta(ImGuiMouseButton button) {
    ImGui::ResetMouseDragDelta(button);
}

ZGUI_API void zguiSpacing(void) {
    ImGui::Spacing();
}

ZGUI_API void zguiNewLine(void) {
    ImGui::NewLine();
}

ZGUI_API void zguiIndent(float indent_w) {
    ImGui::Indent(indent_w);
}

ZGUI_API void zguiUnindent(float indent_w) {
    ImGui::Unindent(indent_w);
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

ZGUI_API void zguiBeginGroup(void) {
    ImGui::BeginGroup();
}

ZGUI_API void zguiEndGroup(void) {
    ImGui::EndGroup();
}

ZGUI_API void zguiGetItemRectMax(float rect[2]) {
    const ImVec2 r = ImGui::GetItemRectMax();
    rect[0] = r.x;
    rect[1] = r.y;
}

ZGUI_API void zguiGetItemRectMin(float rect[2]) {
    const ImVec2 r = ImGui::GetItemRectMin();
    rect[0] = r.x;
    rect[1] = r.y;
}

ZGUI_API void zguiGetCursorPos(float pos[2]) {
    const ImVec2 p = ImGui::GetCursorPos();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API float zguiGetCursorPosX(void) {
    return ImGui::GetCursorPosX();
}

ZGUI_API float zguiGetCursorPosY(void) {
    return ImGui::GetCursorPosY();
}

ZGUI_API void zguiSetCursorPos(float local_x, float local_y) {
    ImGui::SetCursorPos({ local_x, local_y });
}

ZGUI_API void zguiSetCursorPosX(float local_x) {
    ImGui::SetCursorPosX(local_x);
}

ZGUI_API void zguiSetCursorPosY(float local_y) {
    ImGui::SetCursorPosY(local_y);
}

ZGUI_API void zguiGetCursorStartPos(float pos[2]) {
    const ImVec2 p = ImGui::GetCursorStartPos();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API void zguiGetCursorScreenPos(float pos[2]) {
    const ImVec2 p = ImGui::GetCursorScreenPos();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API void zguiSetCursorScreenPos(float screen_x, float screen_y) {
    ImGui::SetCursorScreenPos({ screen_x, screen_y });
}

ZGUI_API int zguiGetMouseCursor(void) {
    return ImGui::GetMouseCursor();
}

ZGUI_API void zguiSetMouseCursor(int cursor) {
    ImGui::SetMouseCursor(cursor);
}

ZGUI_API void zguiAlignTextToFramePadding(void) {
    ImGui::AlignTextToFramePadding();
}

ZGUI_API float zguiGetTextLineHeight(void) {
    return ImGui::GetTextLineHeight();
}

ZGUI_API float zguiGetTextLineHeightWithSpacing(void) {
    return ImGui::GetTextLineHeightWithSpacing();
}

ZGUI_API float zguiGetFrameHeight(void) {
    return ImGui::GetFrameHeight();
}

ZGUI_API float zguiGetFrameHeightWithSpacing(void) {
    return ImGui::GetFrameHeightWithSpacing();
}

ZGUI_API bool zguiDragFloat(
    const char* label,
    float* v,
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragFloat2(
    const char* label,
    float v[2],
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragFloat3(
    const char* label,
    float v[3],
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragFloat4(
    const char* label,
    float v[4],
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragFloatRange2(
    const char* label,
    float* v_current_min,
    float* v_current_max,
    float v_speed,
    float v_min,
    float v_max,
    const char* format,
    const char* format_max,
    ImGuiSliderFlags flags
) {
    return ImGui::DragFloatRange2(
        label,
        v_current_min,
        v_current_max,
        v_speed,
        v_min,
        v_max,
        format,
        format_max,
        flags
    );
}

ZGUI_API bool zguiDragInt(
    const char* label,
    int* v,
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragInt(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragInt2(
    const char* label,
    int v[2],
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragInt2(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragInt3(
    const char* label,
    int v[3],
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragInt3(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragInt4(
    const char* label,
    int v[4],
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragInt4(label, v, v_speed, v_min, v_max, format, flags);
}

ZGUI_API bool zguiDragIntRange2(
    const char* label,
    int* v_current_min,
    int* v_current_max,
    float v_speed,
    int v_min,
    int v_max,
    const char* format,
    const char* format_max,
    ImGuiSliderFlags flags
) {
    return ImGui::DragIntRange2(
        label,
        v_current_min,
        v_current_max,
        v_speed,
        v_min,
        v_max,
        format,
        format_max,
        flags
    );
}

ZGUI_API bool zguiDragScalar(
    const char* label,
    ImGuiDataType data_type,
    void* p_data,
    float v_speed,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragScalar(label, data_type, p_data, v_speed, p_min, p_max, format, flags);
}

ZGUI_API bool zguiDragScalarN(
    const char* label,
    ImGuiDataType data_type,
    void* p_data,
    int components,
    float v_speed,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::DragScalarN(label, data_type, p_data, components, v_speed, p_min, p_max, format, flags);
}

ZGUI_API bool zguiCombo(
    const char* label,
    int* current_item,
    const char* items_separated_by_zeros,
    int popup_max_height_in_items
) {
    return ImGui::Combo(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
}

ZGUI_API bool zguiBeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags) {
    return ImGui::BeginCombo(label, preview_value, flags);
}

ZGUI_API void zguiEndCombo(void) {
    ImGui::EndCombo();
}

ZGUI_API bool zguiBeginListBox(const char* label, float w, float h) {
    return ImGui::BeginListBox(label, { w, h });
}

ZGUI_API void zguiEndListBox(void) {
    ImGui::EndListBox();
}

ZGUI_API bool zguiSelectable(const char* label, bool selected, ImGuiSelectableFlags flags, float w, float h) {
    return ImGui::Selectable(label, selected, flags, { w, h });
}

ZGUI_API bool zguiSelectableStatePtr(
    const char* label,
    bool* p_selected,
    ImGuiSelectableFlags flags,
    float w,
    float h
) {
    return ImGui::Selectable(label, p_selected, flags, { w, h });
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

ZGUI_API bool zguiSliderFloat2(
    const char* label,
    float v[2],
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderFloat2(label, v, v_min, v_max, format, flags);
}

ZGUI_API bool zguiSliderFloat3(
    const char* label,
    float v[3],
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderFloat3(label, v, v_min, v_max, format, flags);
}

ZGUI_API bool zguiSliderFloat4(
    const char* label,
    float v[4],
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderFloat4(label, v, v_min, v_max, format, flags);
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

ZGUI_API bool zguiSliderInt2(
    const char* label,
    int v[2],
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderInt2(label, v, v_min, v_max, format, flags);
}

ZGUI_API bool zguiSliderInt3(
    const char* label,
    int v[3],
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderInt3(label, v, v_min, v_max, format, flags);
}

ZGUI_API bool zguiSliderInt4(
    const char* label,
    int v[4],
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderInt4(label, v, v_min, v_max, format, flags);
}

ZGUI_API bool zguiSliderScalar(
    const char* label,
    ImGuiDataType data_type,
    void* p_data,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderScalar(label, data_type, p_data, p_min, p_max, format, flags);
}

ZGUI_API bool zguiSliderScalarN(
    const char* label,
    ImGuiDataType data_type,
    void* p_data,
    int components,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderScalarN(label, data_type, p_data, components, p_min, p_max, format, flags);
}

ZGUI_API bool zguiVSliderFloat(
    const char* label,
    float w,
    float h,
    float* v,
    float v_min,
    float v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::VSliderFloat(label, { w, h }, v, v_min, v_max, format, flags);
}

ZGUI_API bool zguiVSliderInt(
    const char* label,
    float w,
    float h,
    int* v,
    int v_min,
    int v_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::VSliderInt(label, { w, h }, v, v_min, v_max, format, flags);
}

ZGUI_API bool zguiVSliderScalar(
    const char* label,
    float w,
    float h,
    ImGuiDataType data_type,
    void* p_data,
    const void* p_min,
    const void* p_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::VSliderScalar(label, { w, h }, data_type, p_data, p_min, p_max, format, flags);
}

ZGUI_API bool zguiSliderAngle(
    const char* label,
    float* v_rad,
    float v_degrees_min,
    float v_degrees_max,
    const char* format,
    ImGuiSliderFlags flags
) {
    return ImGui::SliderAngle(label, v_rad, v_degrees_min, v_degrees_max, format, flags);
}

ZGUI_API ImGuiInputTextCallbackData zguiInputTextCallbackData_Init(void) {
    return ImGuiInputTextCallbackData();
}

ZGUI_API void zguiInputTextCallbackData_DeleteChars(
    ImGuiInputTextCallbackData* data,
    int pos,
    int bytes_count
) {
    data->DeleteChars(pos, bytes_count);
}

ZGUI_API void zguiInputTextCallbackData_InsertChars(
    ImGuiInputTextCallbackData* data,
    int pos,
    const char* text,
    const char* text_end
) {
    data->InsertChars(pos, text, text_end);
}

ZGUI_API bool zguiInputText(
    const char* label,
    char* buf,
    size_t buf_size,
    ImGuiInputTextFlags flags,
    ImGuiInputTextCallback callback,
    void* user_data
) {
    return ImGui::InputText(label, buf, buf_size, flags, callback, user_data);
}

ZGUI_API bool zguiInputTextMultiline(
    const char* label,
    char* buf,
    size_t buf_size,
    float w,
    float h,
    ImGuiInputTextFlags flags,
    ImGuiInputTextCallback callback,
    void* user_data
) {
    return ImGui::InputTextMultiline(label, buf, buf_size, { w, h }, flags, callback, user_data);
}

ZGUI_API bool zguiInputTextWithHint(
    const char* label,
    const char* hint,
    char* buf,
    size_t buf_size,
    ImGuiInputTextFlags flags,
    ImGuiInputTextCallback callback,
    void* user_data
) {
    return ImGui::InputTextWithHint(label, hint, buf, buf_size, flags, callback, user_data);
}

ZGUI_API bool zguiInputFloat(
    const char* label,
    float* v,
    float step,
    float step_fast,
    const char* format,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputFloat(label, v, step, step_fast, format, flags);
}

ZGUI_API bool zguiInputFloat2(
    const char* label,
    float v[2],
    const char* format,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputFloat2(label, v, format, flags);
}

ZGUI_API bool zguiInputFloat3(
    const char* label,
    float v[3],
    const char* format,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputFloat3(label, v, format, flags);
}

ZGUI_API bool zguiInputFloat4(
    const char* label,
    float v[4],
    const char* format,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputFloat4(label, v, format, flags);
}

ZGUI_API bool zguiInputInt(
    const char* label,
    int* v,
    int step,
    int step_fast,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputInt(label, v, step, step_fast, flags);
}

ZGUI_API bool zguiInputInt2(const char* label, int v[2], ImGuiInputTextFlags flags) {
    return ImGui::InputInt2(label, v, flags);
}

ZGUI_API bool zguiInputInt3(const char* label, int v[3], ImGuiInputTextFlags flags) {
    return ImGui::InputInt3(label, v, flags);
}

ZGUI_API bool zguiInputInt4(const char* label, int v[4], ImGuiInputTextFlags flags) {
    return ImGui::InputInt4(label, v, flags);
}

ZGUI_API bool zguiInputDouble(
    const char* label,
    double* v,
    double step,
    double step_fast,
    const char* format,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputDouble(label, v, step, step_fast, format, flags);
}

ZGUI_API bool zguiInputScalar(
    const char* label,
    ImGuiDataType data_type,
    void* p_data,
    const void* p_step,
    const void* p_step_fast,
    const char* format,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputScalar(label, data_type, p_data, p_step, p_step_fast, format, flags);
}

ZGUI_API bool zguiInputScalarN(
    const char* label,
    ImGuiDataType data_type,
    void* p_data,
    int components,
    const void* p_step,
    const void* p_step_fast,
    const char* format,
    ImGuiInputTextFlags flags
) {
    return ImGui::InputScalarN(label, data_type, p_data, components, p_step, p_step_fast, format, flags);
}

ZGUI_API bool zguiColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags) {
    return ImGui::ColorEdit3(label, col, flags);
}

ZGUI_API bool zguiColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags) {
    return ImGui::ColorEdit4(label, col, flags);
}

ZGUI_API bool zguiColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags) {
    return ImGui::ColorPicker3(label, col, flags);
}

ZGUI_API bool zguiColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col) {
    return ImGui::ColorPicker4(label, col, flags, ref_col);
}

ZGUI_API bool zguiColorButton(const char* desc_id, const float col[4], ImGuiColorEditFlags flags, float w, float h) {
    return ImGui::ColorButton(desc_id, { col[0], col[1], col[2], col[3] }, flags, { w, h });
}

ZGUI_API void zguiTextUnformatted(const char* text, const char* text_end) {
    ImGui::TextUnformatted(text, text_end);
}

ZGUI_API void zguiTextColored(const float col[4], const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextColoredV({ col[0], col[1], col[2], col[3] }, fmt, args);
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

ZGUI_API void zguiCalcTextSize(
    const char* txt,
    const char* txt_end,
    bool hide_text_after_double_hash,
    float wrap_width,
    float* out_w,
    float* out_h
) {
    assert(out_w && out_h);
    const ImVec2 s = ImGui::CalcTextSize(txt, txt_end, hide_text_after_double_hash, wrap_width);
    *out_w = s.x;
    *out_h = s.y;
}

ZGUI_API bool zguiButton(const char* label, float x, float y) {
    return ImGui::Button(label, { x, y });
}

ZGUI_API bool zguiSmallButton(const char* label) {
    return ImGui::SmallButton(label);
}

ZGUI_API bool zguiInvisibleButton(const char* str_id, float w, float h, ImGuiButtonFlags flags) {
    return ImGui::InvisibleButton(str_id, { w, h }, flags);
}

ZGUI_API bool zguiArrowButton(const char* str_id, ImGuiDir dir) {
    return ImGui::ArrowButton(str_id, dir);
}

ZGUI_API void zguiImage(
    ImTextureID user_texture_id,
    float w,
    float h,
    const float uv0[2],
    const float uv1[2],
    const float tint_col[4],
    const float border_col[4]
) {
    ImGui::Image(
        user_texture_id,
        { w, h },
        { uv0[0], uv0[1] },
        { uv1[0], uv1[1] },
        { tint_col[0], tint_col[1], tint_col[2], tint_col[3] },
        { border_col[0], border_col[1], border_col[2], border_col[3] }
    );
}

ZGUI_API bool zguiImageButton(
    const char* str_id,
    ImTextureID user_texture_id,
    float w,
    float h,
    const float uv0[2],
    const float uv1[2],
    const float bg_col[4],
    const float tint_col[4]
) {
    return ImGui::ImageButton(
        str_id,
        user_texture_id,
        { w, h },
        { uv0[0], uv0[1] },
        { uv1[0], uv1[1] },
        { bg_col[0], bg_col[1], bg_col[2], bg_col[3] },
        { tint_col[0], tint_col[1], tint_col[2], tint_col[3] }
    );
}

ZGUI_API void zguiBullet(void) {
    ImGui::Bullet();
}

ZGUI_API bool zguiRadioButton(const char* label, bool active) {
    return ImGui::RadioButton(label, active);
}

ZGUI_API bool zguiRadioButtonStatePtr(const char* label, int* v, int v_button) {
    return ImGui::RadioButton(label, v, v_button);
}

ZGUI_API bool zguiCheckbox(const char* label, bool* v) {
    return ImGui::Checkbox(label, v);
}

ZGUI_API bool zguiCheckboxBits(const char* label, unsigned int* bits, unsigned int bits_value) {
    return ImGui::CheckboxFlags(label, bits, bits_value);
}

ZGUI_API void zguiProgressBar(float fraction, float w, float h, const char* overlay) {
    return ImGui::ProgressBar(fraction, { w, h }, overlay);
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

ZGUI_API ImGuiStyle* zguiGetStyle(void) {
    return &ImGui::GetStyle();
}

ZGUI_API ImGuiStyle zguiStyle_Init(void) {
    return ImGuiStyle();
}

ZGUI_API void zguiStyle_ScaleAllSizes(ImGuiStyle* style, float scale_factor) {
    style->ScaleAllSizes(scale_factor);
}

ZGUI_API void zguiPushStyleColor4f(ImGuiCol idx, const float col[4]) {
    ImGui::PushStyleColor(idx, { col[0], col[1], col[2], col[3] });
}

ZGUI_API void zguiPushStyleColor1u(ImGuiCol idx, unsigned int col) {
    ImGui::PushStyleColor(idx, col);
}

ZGUI_API void zguiPopStyleColor(int count) {
    ImGui::PopStyleColor(count);
}

ZGUI_API void zguiPushStyleVar1f(ImGuiStyleVar idx, float var) {
    ImGui::PushStyleVar(idx, var);
}

ZGUI_API void zguiPushStyleVar2f(ImGuiStyleVar idx, const float var[2]) {
    ImGui::PushStyleVar(idx, { var[0], var[1] });
}

ZGUI_API void zguiPopStyleVar(int count) {
    ImGui::PopStyleVar(count);
}

ZGUI_API void zguiPushItemWidth(float item_width) {
    ImGui::PushItemWidth(item_width);
}

ZGUI_API void zguiPopItemWidth(void) {
    ImGui::PopItemWidth();
}

ZGUI_API void zguiSetNextItemWidth(float item_width) {
    ImGui::SetNextItemWidth(item_width);
}

ZGUI_API ImFont* zguiGetFont(void) {
    return ImGui::GetFont();
}

ZGUI_API float zguiGetFontSize(void) {
    return ImGui::GetFontSize();
}

ZGUI_API void zguiPushFont(ImFont* font) {
    ImGui::PushFont(font);
}

ZGUI_API void zguiPopFont(void) {
    ImGui::PopFont();
}

ZGUI_API bool zguiTreeNode(const char* label) {
    return ImGui::TreeNode(label);
}

ZGUI_API bool zguiTreeNodeStrId(const char* str_id, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const bool ret = ImGui::TreeNodeV(str_id, fmt, args);
    va_end(args);
    return ret;
}

ZGUI_API bool zguiTreeNodeStrIdFlags(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const bool ret = ImGui::TreeNodeExV(str_id, flags, fmt, args);
    va_end(args);
    return ret;
}

ZGUI_API bool zguiTreeNodePtrId(const void* ptr_id, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const bool ret = ImGui::TreeNodeV(ptr_id, fmt, args);
    va_end(args);
    return ret;
}

ZGUI_API bool zguiTreeNodePtrIdFlags(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    const bool ret = ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
    va_end(args);
    return ret;
}

ZGUI_API bool zguiCollapsingHeader(const char* label, ImGuiTreeNodeFlags flags) {
    return ImGui::CollapsingHeader(label, flags);
}

ZGUI_API bool zguiCollapsingHeaderStatePtr(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags) {
    return ImGui::CollapsingHeader(label, p_visible, flags);
}

ZGUI_API void zguiSetNextItemOpen(bool is_open, ImGuiCond cond) {
    ImGui::SetNextItemOpen(is_open, cond);
}

ZGUI_API void zguiTreePushStrId(const char* str_id) {
    ImGui::TreePush(str_id);
}

ZGUI_API void zguiTreePushPtrId(const void* ptr_id) {
    ImGui::TreePush(ptr_id);
}

ZGUI_API void zguiTreePop(void) {
    ImGui::TreePop();
}

ZGUI_API void zguiPushStrId(const char* str_id_begin, const char* str_id_end) {
    ImGui::PushID(str_id_begin, str_id_end);
}

ZGUI_API void zguiPushStrIdZ(const char* str_id) {
    ImGui::PushID(str_id);
}

ZGUI_API void zguiPushPtrId(const void* ptr_id) {
    ImGui::PushID(ptr_id);
}

ZGUI_API void zguiPushIntId(int int_id) {
    ImGui::PushID(int_id);
}

ZGUI_API void zguiPopId(void) {
    ImGui::PopID();
}

ZGUI_API ImGuiID zguiGetStrId(const char* str_id_begin, const char* str_id_end) {
    return ImGui::GetID(str_id_begin, str_id_end);
}

ZGUI_API ImGuiID zguiGetStrIdZ(const char* str_id) {
    return ImGui::GetID(str_id);
}

ZGUI_API ImGuiID zguiGetPtrId(const void* ptr_id) {
    return ImGui::GetID(ptr_id);
}

ZGUI_API void zguiSetClipboardText(const char* text) {
    ImGui::SetClipboardText(text);
}

ZGUI_API const char* zguiGetClipboardText() {
    return ImGui::GetClipboardText();
}

ZGUI_API ImFont* zguiIoAddFontFromFileWithConfig(
    const char* filename,
    float size_pixels,
    const ImFontConfig* config,
    const ImWchar* ranges
) {
    return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size_pixels, config, ranges);
}

ZGUI_API ImFont* zguiIoAddFontFromFile(const char* filename, float size_pixels) {
    return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size_pixels, nullptr, nullptr);
}

ZGUI_API ImFont* zguiIoAddFontFromMemoryWithConfig(
    void* font_data,
    int font_size,
    float size_pixels,
    const ImFontConfig* config,
    const ImWchar* ranges
) {
    return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(font_data, font_size, size_pixels, config, ranges);
}

ZGUI_API ImFont* zguiIoAddFontFromMemory(void* font_data, int font_size, float size_pixels) {
    ImFontConfig config = ImFontConfig();
    config.FontDataOwnedByAtlas = false;
    return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(font_data, font_size, size_pixels, &config, nullptr);
}

ZGUI_API ImFontConfig zguiFontConfig_Init(void) {
    return ImFontConfig();
}

ZGUI_API ImFont* zguiIoGetFont(unsigned int index) {
    return ImGui::GetIO().Fonts->Fonts[index];
}

ZGUI_API void zguiIoSetDefaultFont(ImFont* font) {
    ImGui::GetIO().FontDefault = font;
}

ZGUI_API unsigned char *zguiIoGetFontsTexDataAsRgba32(int *width, int *height) {
    unsigned char *font_pixels;
    int font_width, font_height;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
    *width = font_width;
    *height = font_height;
    return font_pixels;
}

ZGUI_API void zguiIoSetFontsTexId(ImTextureID id) {
    ImGui::GetIO().Fonts->TexID = id;
}

ZGUI_API ImTextureID zguiIoGetFontsTexId(void) {
    return ImGui::GetIO().Fonts->TexID;
}

ZGUI_API bool zguiIoGetWantCaptureMouse(void) {
    return ImGui::GetIO().WantCaptureMouse;
}

ZGUI_API bool zguiIoGetWantCaptureKeyboard(void) {
    return ImGui::GetIO().WantCaptureKeyboard;
}

ZGUI_API void zguiIoSetIniFilename(const char* filename) {
    ImGui::GetIO().IniFilename = filename;
}

ZGUI_API void zguiIoSetConfigFlags(ImGuiConfigFlags flags) {
    ImGui::GetIO().ConfigFlags = flags;
}

ZGUI_API void zguiIoSetDisplaySize(float width, float height) {
    ImGui::GetIO().DisplaySize = { width, height };
}

ZGUI_API void zguiIoGetDisplaySize(float size[2]) {
    const ImVec2 ds = ImGui::GetIO().DisplaySize;
    size[0] = ds[0];
    size[1] = ds[1];
}

ZGUI_API void zguiIoSetDisplayFramebufferScale(float sx, float sy) {
    ImGui::GetIO().DisplayFramebufferScale = { sx, sy };
}

ZGUI_API void zguiIoSetDeltaTime(float delta_time) {
    ImGui::GetIO().DeltaTime = delta_time;
}

ZGUI_API void zguiIoAddFocusEvent(bool focused) {
    ImGui::GetIO().AddFocusEvent(focused);
}

ZGUI_API void zguiIoAddMousePositionEvent(float x, float y) {
    ImGui::GetIO().AddMousePosEvent(x, y);
}

ZGUI_API void zguiIoAddMouseButtonEvent(ImGuiMouseButton button, bool down) {
    ImGui::GetIO().AddMouseButtonEvent(button, down);
}

ZGUI_API void zguiIoAddMouseWheelEvent(float x, float y) {
    ImGui::GetIO().AddMouseWheelEvent(x, y);
}

ZGUI_API void zguiIoAddKeyEvent(ImGuiKey key, bool down) {
    ImGui::GetIO().AddKeyEvent(key, down);
}

ZGUI_API void zguiIoAddInputCharactersUTF8(const char* utf8_chars) {
    ImGui::GetIO().AddInputCharactersUTF8(utf8_chars);
}

ZGUI_API void zguiIoSetKeyEventNativeData(ImGuiKey key, int keycode, int scancode) {
    ImGui::GetIO().SetKeyEventNativeData(key, keycode, scancode);
}

ZGUI_API void zguiIoAddCharacterEvent(int c) {
    ImGui::GetIO().AddInputCharacter(c);
}


ZGUI_API bool zguiIsItemHovered(ImGuiHoveredFlags flags) {
    return ImGui::IsItemHovered(flags);
}

ZGUI_API bool zguiIsItemActive(void) {
    return ImGui::IsItemActive();
}

ZGUI_API bool zguiIsItemFocused(void) {
    return ImGui::IsItemFocused();
}

ZGUI_API bool zguiIsItemClicked(ImGuiMouseButton mouse_button) {
    return ImGui::IsItemClicked(mouse_button);
}

ZGUI_API bool zguiIsMouseDoubleClicked(ImGuiMouseButton button) {
    return ImGui::IsMouseDoubleClicked(button);
}

ZGUI_API bool zguiIsItemVisible(void) {
    return ImGui::IsItemVisible();
}

ZGUI_API bool zguiIsItemEdited(void) {
    return ImGui::IsItemEdited();
}

ZGUI_API bool zguiIsItemActivated(void) {
    return ImGui::IsItemActivated();
}

ZGUI_API bool zguiIsItemDeactivated(void) {
    return ImGui::IsItemDeactivated();
}

ZGUI_API bool zguiIsItemDeactivatedAfterEdit(void) {
    return ImGui::IsItemDeactivatedAfterEdit();
}

ZGUI_API bool zguiIsItemToggledOpen(void) {
    return ImGui::IsItemToggledOpen();
}

ZGUI_API bool zguiIsAnyItemHovered(void) {
    return ImGui::IsAnyItemHovered();
}

ZGUI_API bool zguiIsAnyItemActive(void) {
    return ImGui::IsAnyItemActive();
}

ZGUI_API bool zguiIsAnyItemFocused(void) {
    return ImGui::IsAnyItemFocused();
}

ZGUI_API void zguiGetContentRegionAvail(float pos[2]) {
    const ImVec2 p = ImGui::GetContentRegionAvail();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API void zguiGetContentRegionMax(float pos[2]) {
    const ImVec2 p = ImGui::GetContentRegionMax();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API void zguiGetWindowContentRegionMin(float pos[2]) {
    const ImVec2 p = ImGui::GetWindowContentRegionMin();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API void zguiGetWindowContentRegionMax(float pos[2]) {
    const ImVec2 p = ImGui::GetWindowContentRegionMax();
    pos[0] = p.x;
    pos[1] = p.y;
}

ZGUI_API void zguiPushTextWrapPos(float wrap_pos_x) {
    ImGui::PushTextWrapPos(wrap_pos_x);
}

ZGUI_API bool zguiBeginTabBar(const char* string, ImGuiTabBarFlags flags) {
    return ImGui::BeginTabBar(string, flags);
}

ZGUI_API bool zguiBeginTabItem(const char* string, bool* p_open, ImGuiTabItemFlags flags) {
    return ImGui::BeginTabItem(string, p_open, flags);
}

ZGUI_API void zguiEndTabItem(void) {
    ImGui::EndTabItem();
}

ZGUI_API void zguiEndTabBar(void) {
    ImGui::EndTabBar();
}

ZGUI_API void zguiSetTabItemClosed(const char* tab_or_docked_window_label) {
    ImGui::SetTabItemClosed(tab_or_docked_window_label);
}

ZGUI_API bool zguiBeginMenuBar(void) {
    return ImGui::BeginMenuBar();
}

ZGUI_API void zguiEndMenuBar(void) {
    ImGui::EndMenuBar();
}

ZGUI_API bool zguiBeginMainMenuBar(void) {
    return ImGui::BeginMainMenuBar();
}

ZGUI_API void zguiEndMainMenuBar(void) {
    ImGui::EndMainMenuBar();
}

ZGUI_API bool zguiBeginMenu(const char* label, bool enabled) {
    return ImGui::BeginMenu(label, enabled);
}

ZGUI_API void zguiEndMenu(void) {
    ImGui::EndMenu();
}

ZGUI_API bool zguiMenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
    return ImGui::MenuItem(label, shortcut, selected, enabled);
}

ZGUI_API void zguiBeginTooltip(void) {
    ImGui::BeginTooltip();
}

ZGUI_API void zguiEndTooltip(void) {
    ImGui::EndTooltip();
}

ZGUI_API bool zguiBeginPopupContextWindow(void) {
    return ImGui::BeginPopupContextWindow();
}

ZGUI_API bool zguiBeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags) {
    return ImGui::BeginPopupModal(name, p_open, flags);
}

ZGUI_API void zguiEndPopup(void) {
    ImGui::EndPopup();
}

ZGUI_API void zguiOpenPopup(const char* str_id, ImGuiPopupFlags popup_flags) {
    ImGui::OpenPopup(str_id, popup_flags);
}

ZGUI_API void zguiCloseCurrentPopup(void) {
    ImGui::CloseCurrentPopup();
}
//--------------------------------------------------------------------------------------------------
//
// Tables
//
//--------------------------------------------------------------------------------------------------
ZGUI_API void zguiBeginTable(
    const char* str_id,
    int column,
    ImGuiTableFlags flags,
    const float outer_size[2],
    float inner_width
) {
    ImGui::BeginTable(str_id, column, flags, { outer_size[0], outer_size[1] }, inner_width);
}

ZGUI_API void zguiEndTable(void) {
    ImGui::EndTable();
}

ZGUI_API void zguiTableNextRow(ImGuiTableRowFlags row_flags, float min_row_height) {
    ImGui::TableNextRow(row_flags, min_row_height);
}

ZGUI_API bool zguiTableNextColumn(void) {
    return ImGui::TableNextColumn();
}

ZGUI_API bool zguiTableSetColumnIndex(int column_n) {
    return ImGui::TableSetColumnIndex(column_n);
}

ZGUI_API void zguiTableSetupColumn(
    const char* label,
    ImGuiTableColumnFlags flags,
    float init_width_or_height,
    ImGuiID user_id
) {
    ImGui::TableSetupColumn(label, flags, init_width_or_height, user_id);
}

ZGUI_API void zguiTableSetupScrollFreeze(int cols, int rows) {
    ImGui::TableSetupScrollFreeze(cols, rows);
}

ZGUI_API void zguiTableHeadersRow(void) {
    ImGui::TableHeadersRow();
}

ZGUI_API void zguiTableHeader(const char* label) {
    ImGui::TableHeader(label);
}

ZGUI_API ImGuiTableSortSpecs* zguiTableGetSortSpecs(void) {
    return ImGui::TableGetSortSpecs();
}

ZGUI_API int zguiTableGetColumnCount(void) {
    return ImGui::TableGetColumnCount();
}

ZGUI_API int zguiTableGetColumnIndex(void) {
    return ImGui::TableGetColumnIndex();
}

ZGUI_API int zguiTableGetRowIndex(void) {
    return ImGui::TableGetRowIndex();
}

ZGUI_API const char* zguiTableGetColumnName(int column_n) {
    return ImGui::TableGetColumnName(column_n);
}

ZGUI_API ImGuiTableColumnFlags zguiTableGetColumnFlags(int column_n) {
    return ImGui::TableGetColumnFlags(column_n);
}

ZGUI_API void zguiTableSetColumnEnabled(int column_n, bool v) {
    ImGui::TableSetColumnEnabled(column_n, v);
}

ZGUI_API void zguiTableSetBgColor(ImGuiTableBgTarget target, unsigned int color, int column_n) {
    ImGui::TableSetBgColor(target, color, column_n);
}
//--------------------------------------------------------------------------------------------------
//
// Color Utilities
//
//--------------------------------------------------------------------------------------------------
ZGUI_API void zguiColorConvertU32ToFloat4(ImU32 in, float rgba[4]) {
    const ImVec4 c = ImGui::ColorConvertU32ToFloat4(in);
    rgba[0] = c.x;
    rgba[1] = c.y;
    rgba[2] = c.z;
    rgba[3] = c.w;
}

ZGUI_API ImU32 zguiColorConvertFloat4ToU32(const float in[4]) {
    return ImGui::ColorConvertFloat4ToU32({ in[0], in[1], in[2], in[3] });
}

ZGUI_API void zguiColorConvertRGBtoHSV(float r, float g, float b, float* out_h, float* out_s, float* out_v) {
    return ImGui::ColorConvertRGBtoHSV(r, g, b, *out_h, *out_s, *out_v);
}

ZGUI_API void zguiColorConvertHSVtoRGB(float h, float s, float v, float* out_r, float* out_g, float* out_b) {
    return ImGui::ColorConvertHSVtoRGB(h, s, v, *out_r, *out_g, *out_b);
}
//--------------------------------------------------------------------------------------------------
//
// DrawList
//
//--------------------------------------------------------------------------------------------------
ZGUI_API ImDrawList *zguiGetWindowDrawList(void) {
    return ImGui::GetWindowDrawList();
}

ZGUI_API ImDrawList *zguiGetBackgroundDrawList(void) {
    return ImGui::GetBackgroundDrawList();
}

ZGUI_API ImDrawList *zguiGetForegroundDrawList(void) {
    return ImGui::GetForegroundDrawList();
}

ZGUI_API ImDrawList *zguiCreateDrawList(void) {
    return IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());
}

ZGUI_API void zguiDestroyDrawList(ImDrawList *draw_list) {
  IM_DELETE(draw_list);
}

ZGUI_API const char *zguiDrawList_GetOwnerName(ImDrawList *draw_list) {
    return draw_list->_OwnerName;
}

ZGUI_API void zguiDrawList_ResetForNewFrame(ImDrawList *draw_list) {
    draw_list->_ResetForNewFrame();
}

ZGUI_API int zguiDrawList_GetVertexBufferLength(ImDrawList *draw_list) {
    return draw_list->VtxBuffer.size();
}
ZGUI_API ImDrawVert *zguiDrawList_GetVertexBufferData(ImDrawList *draw_list) {
    return draw_list->VtxBuffer.begin();
}

ZGUI_API int zguiDrawList_GetIndexBufferLength(ImDrawList *draw_list) {
    return draw_list->IdxBuffer.size();
}
ZGUI_API ImDrawIdx *zguiDrawList_GetIndexBufferData(ImDrawList *draw_list) {
    return draw_list->IdxBuffer.begin();
}

ZGUI_API int zguiDrawList_GetCmdBufferLength(ImDrawList *draw_list) {
    return draw_list->CmdBuffer.size();
}
ZGUI_API ImDrawCmd *zguiDrawList_GetCmdBufferData(ImDrawList *draw_list) {
    return draw_list->CmdBuffer.begin();
}

ZGUI_API void zguiDrawList_SetFlags(ImDrawList *draw_list, ImDrawListFlags flags) {
    draw_list->Flags = flags;
}
ZGUI_API ImDrawListFlags zguiDrawList_GetFlags(ImDrawList *draw_list) {
    return draw_list->Flags;
}

ZGUI_API void zguiDrawList_PushClipRect(
    ImDrawList* draw_list,
    const float clip_rect_min[2],
    const float clip_rect_max[2],
    bool intersect_with_current_clip_rect
) {
    draw_list->PushClipRect(
        { clip_rect_min[0], clip_rect_min[1] },
        { clip_rect_max[0], clip_rect_max[1] },
        intersect_with_current_clip_rect
    );
}

ZGUI_API void zguiDrawList_PushClipRectFullScreen(ImDrawList* draw_list) {
    draw_list->PushClipRectFullScreen();
}

ZGUI_API void zguiDrawList_PopClipRect(ImDrawList* draw_list) {
    draw_list->PopClipRect();
}

ZGUI_API void zguiDrawList_PushTextureId(ImDrawList* draw_list, ImTextureID texture_id) {
    draw_list->PushTextureID(texture_id);
}

ZGUI_API void zguiDrawList_PopTextureId(ImDrawList* draw_list) {
    draw_list->PopTextureID();
}

ZGUI_API void zguiDrawList_GetClipRectMin(ImDrawList* draw_list, float clip_min[2]) {
    const ImVec2 c = draw_list->GetClipRectMin();
    clip_min[0] = c.x;
    clip_min[1] = c.y;
}

ZGUI_API void zguiDrawList_GetClipRectMax(ImDrawList* draw_list, float clip_max[2]) {
    const ImVec2 c = draw_list->GetClipRectMax();
    clip_max[0] = c.x;
    clip_max[1] = c.y;
}

ZGUI_API void zguiDrawList_AddLine(
    ImDrawList* draw_list,
    const float p1[2],
    const float p2[2],
    unsigned int col,
    float thickness
) {
    draw_list->AddLine({ p1[0], p1[1] }, { p2[0], p2[1] }, col, thickness);
}

ZGUI_API void zguiDrawList_AddRect(
    ImDrawList* draw_list,
    const float pmin[2],
    const float pmax[2],
    unsigned int col,
    float rounding,
    ImDrawFlags flags,
    float thickness
) {
    draw_list->AddRect({ pmin[0], pmin[1] }, { pmax[0], pmax[1] }, col, rounding, flags, thickness);
}

ZGUI_API void zguiDrawList_AddRectFilled(
    ImDrawList* draw_list,
    const float pmin[2],
    const float pmax[2],
    unsigned int col,
    float rounding,
    ImDrawFlags flags
) {
    draw_list->AddRectFilled({ pmin[0], pmin[1] }, { pmax[0], pmax[1] }, col, rounding, flags);
}

ZGUI_API void zguiDrawList_AddRectFilledMultiColor(
    ImDrawList* draw_list,
    const float pmin[2],
    const float pmax[2],
    unsigned int col_upr_left,
    unsigned int col_upr_right,
    unsigned int col_bot_right,
    unsigned int col_bot_left
) {
    draw_list->AddRectFilledMultiColor(
        { pmin[0], pmin[1] },
        { pmax[0], pmax[1] },
        col_upr_left,
        col_upr_right,
        col_bot_right,
        col_bot_left
    );
}

ZGUI_API void zguiDrawList_AddQuad(
    ImDrawList* draw_list,
    const float p1[2],
    const float p2[2],
    const float p3[2],
    const float p4[2],
    unsigned int col,
    float thickness
) {
    draw_list->AddQuad({ p1[0], p1[1] }, { p2[0], p2[1] }, { p3[0], p3[1] }, { p4[0], p4[1] }, col, thickness);
}

ZGUI_API void zguiDrawList_AddQuadFilled(
    ImDrawList* draw_list,
    const float p1[2],
    const float p2[2],
    const float p3[2],
    const float p4[2],
    unsigned int col
) {
    draw_list->AddQuadFilled({ p1[0], p1[1] }, { p2[0], p2[1] }, { p3[0], p3[1] }, { p4[0], p4[1] }, col);
}

ZGUI_API void zguiDrawList_AddTriangle(
    ImDrawList* draw_list,
    const float p1[2],
    const float p2[2],
    const float p3[2],
    unsigned int col,
    float thickness
) {
    draw_list->AddTriangle({ p1[0], p1[1] }, { p2[0], p2[1] }, { p3[0], p3[1] }, col, thickness);
}

ZGUI_API void zguiDrawList_AddTriangleFilled(
    ImDrawList* draw_list,
    const float p1[2],
    const float p2[2],
    const float p3[2],
    unsigned int col
) {
    draw_list->AddTriangleFilled({ p1[0], p1[1] }, { p2[0], p2[1] }, { p3[0], p3[1] }, col);
}

ZGUI_API void zguiDrawList_AddCircle(
    ImDrawList* draw_list,
    const float center[2],
    float radius,
    unsigned int col,
    int num_segments,
    float thickness
) {
    draw_list->AddCircle({ center[0], center[1] }, radius, col, num_segments, thickness);
}

ZGUI_API void zguiDrawList_AddCircleFilled(
    ImDrawList* draw_list,
    const float center[2],
    float radius,
    unsigned int col,
    int num_segments
) {
    draw_list->AddCircleFilled({ center[0], center[1] }, radius, col, num_segments);
}

ZGUI_API void zguiDrawList_AddNgon(
    ImDrawList* draw_list,
    const float center[2],
    float radius,
    unsigned int col,
    int num_segments,
    float thickness
) {
    draw_list->AddNgon({ center[0], center[1] }, radius, col, num_segments, thickness);
}

ZGUI_API void zguiDrawList_AddNgonFilled(
    ImDrawList* draw_list,
    const float center[2],
    float radius,
    unsigned int col,
    int num_segments
) {
    draw_list->AddNgonFilled({ center[0], center[1] }, radius, col, num_segments);
}

ZGUI_API void zguiDrawList_AddText(
    ImDrawList* draw_list,
    const float pos[2],
    unsigned int col,
    const char* text_begin,
    const char* text_end
) {
    draw_list->AddText({ pos[0], pos[1] }, col, text_begin, text_end);
}

ZGUI_API void zguiDrawList_AddPolyline(
    ImDrawList* draw_list,
    const float points[][2],
    int num_points,
    unsigned int col,
    ImDrawFlags flags,
    float thickness
) {
    draw_list->AddPolyline((const ImVec2*)&points[0][0], num_points, col, flags, thickness);
}

ZGUI_API void zguiDrawList_AddConvexPolyFilled(
    ImDrawList* draw_list,
    const float points[][2],
    int num_points,
    unsigned int col
) {
    draw_list->AddConvexPolyFilled((const ImVec2*)&points[0][0], num_points, col);
}

ZGUI_API void zguiDrawList_AddBezierCubic(
    ImDrawList* draw_list,
    const float p1[2],
    const float p2[2],
    const float p3[2],
    const float p4[2],
    unsigned int col,
    float thickness,
    int num_segments
) {
    draw_list->AddBezierCubic(
        { p1[0], p1[1] }, { p2[0], p2[1] }, { p3[0], p3[1] }, { p4[0], p4[1] }, col, thickness, num_segments
    );
}

ZGUI_API void zguiDrawList_AddBezierQuadratic(
    ImDrawList* draw_list,
    const float p1[2],
    const float p2[2],
    const float p3[2],
    unsigned int col,
    float thickness,
    int num_segments
) {
    draw_list->AddBezierQuadratic(
        { p1[0], p1[1] }, { p2[0], p2[1] }, { p3[0], p3[1] }, col, thickness, num_segments
    );
}

ZGUI_API void zguiDrawList_AddImage(
    ImDrawList* draw_list,
    ImTextureID user_texture_id,
    const float pmin[2],
    const float pmax[2],
    const float uvmin[2],
    const float uvmax[2],
    unsigned int col
) {
    draw_list->AddImage(
        user_texture_id,
        { pmin[0], pmin[1] },
        { pmax[0], pmax[1] },
        { uvmin[0], uvmin[1] },
        { uvmax[0], uvmax[1] },
        col
    );
}

ZGUI_API void zguiDrawList_AddImageQuad(
    ImDrawList* draw_list,
    ImTextureID user_texture_id,
    const float p1[2],
    const float p2[2],
    const float p3[2],
    const float p4[2],
    const float uv1[2],
    const float uv2[2],
    const float uv3[2],
    const float uv4[2],
    unsigned int col
) {
    draw_list->AddImageQuad(
        user_texture_id,
        { p1[0], p1[1] },
        { p2[0], p2[1] },
        { p3[0], p3[1] },
        { p4[0], p4[1] },
        { uv1[0], uv1[1] },
        { uv2[0], uv2[1] },
        { uv3[0], uv3[1] },
        { uv4[0], uv4[1] },
        col
    );
}

ZGUI_API void zguiDrawList_AddImageRounded(
    ImDrawList* draw_list,
    ImTextureID user_texture_id,
    const float pmin[2],
    const float pmax[2],
    const float uvmin[2],
    const float uvmax[2],
    unsigned int col,
    float rounding,
    ImDrawFlags flags
) {
    draw_list->AddImageRounded(
        user_texture_id,
        { pmin[0], pmin[1] },
        { pmax[0], pmax[1] },
        { uvmin[0], uvmin[1] },
        { uvmax[0], uvmax[1] },
        col,
        rounding,
        flags
    );
}

ZGUI_API void zguiDrawList_PathClear(ImDrawList* draw_list) {
    draw_list->PathClear();
}

ZGUI_API void zguiDrawList_PathLineTo(ImDrawList* draw_list, const float pos[2]) {
    draw_list->PathLineTo({ pos[0], pos[1] });
}

ZGUI_API void zguiDrawList_PathLineToMergeDuplicate(ImDrawList* draw_list, const float pos[2]) {
    draw_list->PathLineToMergeDuplicate({ pos[0], pos[1] });
}

ZGUI_API void zguiDrawList_PathFillConvex(ImDrawList* draw_list, unsigned int col) {
    draw_list->PathFillConvex(col);
}

ZGUI_API void zguiDrawList_PathStroke(ImDrawList* draw_list, unsigned int col, ImDrawFlags flags, float thickness) {
    draw_list->PathStroke(col, flags, thickness);
}

ZGUI_API void zguiDrawList_PathArcTo(
    ImDrawList* draw_list,
    const float center[2],
    float radius,
    float a_min,
    float a_max,
    int num_segments
) {
    draw_list->PathArcTo({ center[0], center[1] }, radius, a_min, a_max, num_segments);
}

ZGUI_API void zguiDrawList_PathArcToFast(
    ImDrawList* draw_list,
    const float center[2],
    float radius,
    int a_min_of_12,
    int a_max_of_12
) {
    draw_list->PathArcToFast({ center[0], center[1] }, radius, a_min_of_12, a_max_of_12);
}

ZGUI_API void zguiDrawList_PathBezierCubicCurveTo(
    ImDrawList* draw_list,
    const float p2[2],
    const float p3[2],
    const float p4[2],
    int num_segments
) {
    draw_list->PathBezierCubicCurveTo({ p2[0], p2[1] }, { p3[0], p3[1] }, { p4[0], p4[1] }, num_segments);
}

ZGUI_API void zguiDrawList_PathBezierQuadraticCurveTo(
    ImDrawList* draw_list,
    const float p2[2],
    const float p3[2],
    int num_segments
) {
    draw_list->PathBezierQuadraticCurveTo({ p2[0], p2[1] }, { p3[0], p3[1] }, num_segments);
}

ZGUI_API void zguiDrawList_PathRect(
    ImDrawList* draw_list,
    const float rect_min[2],
    const float rect_max[2],
    float rounding,
    ImDrawFlags flags
) {
    draw_list->PathRect({ rect_min[0], rect_min[1] }, { rect_max[0], rect_max[1] }, rounding, flags);
}
//--------------------------------------------------------------------------------------------------
//
// Viewport
//
//--------------------------------------------------------------------------------------------------
ZGUI_API ImGuiViewport* zguiGetMainViewport(void) {
    return ImGui::GetMainViewport();
}

ZGUI_API void zguiViewport_GetPos(ImGuiViewport* viewport, float p[2]) {
    const ImVec2 pos = viewport->Pos;
    p[0] = pos.x;
    p[1] = pos.y;
}

ZGUI_API void zguiViewport_GetSize(ImGuiViewport* viewport, float p[2]) {
    const ImVec2 sz = viewport->Size;
    p[0] = sz.x;
    p[1] = sz.y;
}

ZGUI_API void zguiViewport_GetWorkPos(ImGuiViewport* viewport, float p[2]) {
    const ImVec2 pos = viewport->WorkPos;
    p[0] = pos.x;
    p[1] = pos.y;
}

ZGUI_API void zguiViewport_GetWorkSize(ImGuiViewport* viewport, float p[2]) {
    const ImVec2 sz = viewport->WorkSize;
    p[0] = sz.x;
    p[1] = sz.y;
}
//--------------------------------------------------------------------------------------------------
//
// ImPlot
//
//--------------------------------------------------------------------------------------------------
ZGUI_API ImPlotContext* zguiPlot_CreateContext(void) {
    return ImPlot::CreateContext();
}

ZGUI_API void zguiPlot_DestroyContext(ImPlotContext* ctx) {
    ImPlot::DestroyContext(ctx);
}

ZGUI_API ImPlotContext* zguiPlot_GetCurrentContext(void) {
    return ImPlot::GetCurrentContext();
}

ZGUI_API ImPlotStyle zguiPlotStyle_Init(void) {
    return ImPlotStyle();
}

ZGUI_API ImPlotStyle* zguiPlot_GetStyle(void) {
    return &ImPlot::GetStyle();
}

ZGUI_API void zguiPlot_PushStyleColor4f(ImPlotCol idx, const float col[4]) {
    ImPlot::PushStyleColor(idx, { col[0], col[1], col[2], col[3] });
}

ZGUI_API void zguiPlot_PushStyleColor1u(ImPlotCol idx, unsigned int col) {
    ImPlot::PushStyleColor(idx, col);
}

ZGUI_API void zguiPlot_PopStyleColor(int count) {
    ImPlot::PopStyleColor(count);
}

ZGUI_API void zguiPlot_PushStyleVar1i(ImPlotStyleVar idx, int var) {
    ImPlot::PushStyleVar(idx, var);
}

ZGUI_API void zguiPlot_PushStyleVar1f(ImPlotStyleVar idx, float var) {
    ImPlot::PushStyleVar(idx, var);
}

ZGUI_API void zguiPlot_PushStyleVar2f(ImPlotStyleVar idx, const float var[2]) {
    ImPlot::PushStyleVar(idx, { var[0], var[1] });
}

ZGUI_API void zguiPlot_PopStyleVar(int count) {
    ImPlot::PopStyleVar(count);
}

ZGUI_API void zguiPlot_SetupLegend(ImPlotLocation location, ImPlotLegendFlags flags) {
    ImPlot::SetupLegend(location, flags);
}

ZGUI_API void zguiPlot_SetupAxis(ImAxis axis, const char* label, ImPlotAxisFlags flags) {
    ImPlot::SetupAxis(axis, label, flags);
}

ZGUI_API void zguiPlot_SetupAxisLimits(ImAxis axis, double v_min, double v_max, ImPlotCond cond) {
    ImPlot::SetupAxisLimits(axis, v_min, v_max, cond);
}

ZGUI_API void zguiPlot_SetupFinish(void) {
    ImPlot::SetupFinish();
}

ZGUI_API bool zguiPlot_BeginPlot(const char* title_id, float width, float height, ImPlotFlags flags) {
    return ImPlot::BeginPlot(title_id, { width, height }, flags);
}

ZGUI_API void zguiPlot_PlotLineValues(
    const char* label_id,
    ImGuiDataType data_type,
    const void* values,
    int count,
    double xscale,
    double x0,
    ImPlotLineFlags flags,
    int offset,
    int stride
) {
    if (data_type == ImGuiDataType_S8)
        ImPlot::PlotLine(label_id, (const ImS8*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_U8)
        ImPlot::PlotLine(label_id, (const ImU8*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_S16)
        ImPlot::PlotLine(label_id, (const ImS16*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_U16)
        ImPlot::PlotLine(label_id, (const ImU16*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_S32)
        ImPlot::PlotLine(label_id, (const ImS32*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_U32)
        ImPlot::PlotLine(label_id, (const ImU32*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_Float)
        ImPlot::PlotLine(label_id, (const float*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_Double)
        ImPlot::PlotLine(label_id, (const double*)values, count, xscale, x0, flags, offset, stride);
    else
        assert(false);
}

ZGUI_API void zguiPlot_PlotLine(
    const char* label_id,
    ImGuiDataType data_type,
    const void* xv,
    const void* yv,
    int count,
    ImPlotLineFlags flags,
    int offset,
    int stride
) {
    if (data_type == ImGuiDataType_S8)
        ImPlot::PlotLine(label_id, (const ImS8*)xv, (const ImS8*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_U8)
        ImPlot::PlotLine(label_id, (const ImU8*)xv, (const ImU8*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_S16)
        ImPlot::PlotLine(label_id, (const ImS16*)xv, (const ImS16*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_U16)
        ImPlot::PlotLine(label_id, (const ImU16*)xv, (const ImU16*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_S32)
        ImPlot::PlotLine(label_id, (const ImS32*)xv, (const ImS32*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_U32)
        ImPlot::PlotLine(label_id, (const ImU32*)xv, (const ImU32*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_Float)
        ImPlot::PlotLine(label_id, (const float*)xv, (const float*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_Double)
        ImPlot::PlotLine(label_id, (const double*)xv, (const double*)yv, count, flags, offset, stride);
    else
        assert(false);
}

ZGUI_API void zguiPlot_PlotScatter(
    const char* label_id,
    ImGuiDataType data_type,
    const void* xv,
    const void* yv,
    int count,
    ImPlotScatterFlags flags,
    int offset,
    int stride
) {
    if (data_type == ImGuiDataType_S8)
        ImPlot::PlotScatter(label_id, (const ImS8*)xv, (const ImS8*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_U8)
        ImPlot::PlotScatter(label_id, (const ImU8*)xv, (const ImU8*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_S16)
        ImPlot::PlotScatter(label_id, (const ImS16*)xv, (const ImS16*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_U16)
        ImPlot::PlotScatter(label_id, (const ImU16*)xv, (const ImU16*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_S32)
        ImPlot::PlotScatter(label_id, (const ImS32*)xv, (const ImS32*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_U32)
        ImPlot::PlotScatter(label_id, (const ImU32*)xv, (const ImU32*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_Float)
        ImPlot::PlotScatter(label_id, (const float*)xv, (const float*)yv, count, flags, offset, stride);
    else if (data_type == ImGuiDataType_Double)
        ImPlot::PlotScatter(label_id, (const double*)xv, (const double*)yv, count, flags, offset, stride);
    else
        assert(false);
}

ZGUI_API void zguiPlot_PlotScatterValues(
    const char* label_id,
    ImGuiDataType data_type,
    const void* values,
    int count,
    double xscale,
    double x0,
    ImPlotScatterFlags flags,
    int offset,
    int stride
) {
    if (data_type == ImGuiDataType_S8)
        ImPlot::PlotScatter(label_id, (const ImS8*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_U8)
        ImPlot::PlotScatter(label_id, (const ImU8*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_S16)
        ImPlot::PlotScatter(label_id, (const ImS16*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_U16)
        ImPlot::PlotScatter(label_id, (const ImU16*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_S32)
        ImPlot::PlotScatter(label_id, (const ImS32*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_U32)
        ImPlot::PlotScatter(label_id, (const ImU32*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_Float)
        ImPlot::PlotScatter(label_id, (const float*)values, count, xscale, x0, flags, offset, stride);
    else if (data_type == ImGuiDataType_Double)
        ImPlot::PlotScatter(label_id, (const double*)values, count, xscale, x0, flags, offset, stride);
    else
        assert(false);
}

ZGUI_API void zguiPlot_EndPlot(void) {
    ImPlot::EndPlot();
}
//--------------------------------------------------------------------------------------------------
