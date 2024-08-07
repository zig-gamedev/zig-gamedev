#include "imgui.h"

#include "../node_editor/imgui_node_editor.h"

#ifndef ZGUI_API
#define ZGUI_API
#endif

//--------------------------------------------------------------------------------------------------
//
// ImGUI Node editor
//
//--------------------------------------------------------------------------------------------------
namespace ed = ax::NodeEditor;
extern "C"
{

    //
    // Editor
    //
    ZGUI_API ed::EditorContext *node_editor_CreateEditor(ed::Config *cfg)
    {
        return ed::CreateEditor(cfg);
    }

    ZGUI_API void node_editor_DestroyEditor(ed::EditorContext *editor)
    {
        return ed::DestroyEditor(editor);
    }

    ZGUI_API void node_editor_SetCurrentEditor(ed::EditorContext *editor)
    {
        return ed::SetCurrentEditor(editor);
    }

    ZGUI_API void node_editor_Begin(const char *id, const float size[2])
    {
        ed::Begin(id, ImVec2(size[0], size[1]));
    }

    ZGUI_API void node_editor_End()
    {
        ed::End();
    }

    ZGUI_API bool node_editor_ShowBackgroundContextMenu()
    {
        return ed::ShowBackgroundContextMenu();
    }

    ZGUI_API bool node_editor_ShowNodeContextMenu(ed::NodeId *id)
    {
        return ed::ShowNodeContextMenu(id);
    }

    ZGUI_API bool node_editor_ShowLinkContextMenu(ed::LinkId *id)
    {
        return ed::ShowLinkContextMenu(id);
    }

    ZGUI_API bool node_editor_ShowPinContextMenu(ed::PinId *id)
    {
        return ed::ShowPinContextMenu(id);
    }

    ZGUI_API void node_editor_Suspend()
    {
        ed::Suspend();
    }

    ZGUI_API void node_editor_Resume()
    {
        ed::Resume();
    }

    ZGUI_API void node_editor_NavigateToContent(float duration)
    {
        ed::NavigateToContent(duration);
    }

    ZGUI_API void node_editor_NavigateToSelection(bool zoomIn, float duration)
    {
        ed::NavigateToSelection(zoomIn, duration);
    }

    ZGUI_API void node_editor_SelectNode(ed::NodeId nodeId, bool append)
    {
        ed::SelectNode(nodeId, append);
    }

    ZGUI_API void node_editor_SelectLink(ed::LinkId linkId, bool append)
    {
        ed::SelectLink(linkId, append);
    }

    //
    // Node
    //
    ZGUI_API void node_editor_BeginNode(ed::NodeId id)
    {
        ed::BeginNode(id);
    }

    ZGUI_API void node_editor_EndNode()
    {
        ed::EndNode();
    }

    ZGUI_API void node_editor_SetNodePosition(ed::NodeId id, const float pos[2])
    {
        ed::SetNodePosition(id, ImVec2(pos[0], pos[1]));
    }

    ZGUI_API void node_editor_getNodePosition(ed::NodeId id, float *pos)
    {
        auto node_pos = ed::GetNodePosition(id);
        pos[0] = node_pos.x;
        pos[1] = node_pos.y;
    }

    ZGUI_API void node_editor_getNodeSize(ed::NodeId id, float *size)
    {
        auto node_size = ed::GetNodeSize(id);
        size[0] = node_size.x;
        size[1] = node_size.y;
    }

    ZGUI_API bool node_editor_DeleteNode(ed::NodeId id)
    {
        return ed::DeleteNode(id);
    }

    //
    // Pin
    //
    ZGUI_API void node_editor_BeginPin(ed::PinId id, ed::PinKind kind)
    {
        ed::BeginPin(id, kind);
    }

    ZGUI_API void node_editor_EndPin()
    {
        ed::EndPin();
    }

    ZGUI_API bool node_editor_PinHadAnyLinks(ed::PinId pinId)
    {
        return ed::PinHadAnyLinks(pinId);
    }

    ZGUI_API void node_editor_PinRect(const float a[2], const float b[2])
    {
        ed::PinRect(ImVec2(a[0], a[1]), ImVec2(b[0], b[1]));
    }

    ZGUI_API void node_editor_PinPivotRect(const float a[2], const float b[2])
    {
        ed::PinPivotRect(ImVec2(a[0], a[1]), ImVec2(b[0], b[1]));
    }

    ZGUI_API void node_editor_PinPivotSize(const float size[2])
    {
        ed::PinPivotSize(ImVec2(size[0], size[1]));
    }

    ZGUI_API void node_editor_PinPivotScale(const float scale[2])
    {
        ed::PinPivotScale(ImVec2(scale[0], scale[1]));
    }

    ZGUI_API void node_editor_PinPivotAlignment(const float alignment[2])
    {
        ed::PinPivotAlignment(ImVec2(alignment[0], alignment[1]));
    }

    //
    // Link
    //
    ZGUI_API bool node_editor_Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId, const float color[4], float thickness)
    {
        return ed::Link(id, startPinId, endPinId, ImVec4(color[0], color[1], color[2], color[3]), thickness);
    }

    ZGUI_API bool node_editor_DeleteLink(ed::LinkId id)
    {
        return ed::DeleteLink(id);
    }

    ZGUI_API int node_editor_BreakPinLinks(ed::PinId id)
    {
        return ed::BreakLinks(id);
    }

    // Groups
    ZGUI_API void node_editor_Group(const float size[2])
    {
        ed::Group(ImVec2(size[0], size[1]));
    }

    // Created
    ZGUI_API bool node_editor_BeginCreate()
    {
        return ed::BeginCreate();
    }

    ZGUI_API void node_editor_EndCreate()
    {
        ed::EndCreate();
    }

    ZGUI_API bool node_editor_QueryNewLink(ed::PinId *startId, ed::PinId *endId)
    {
        return ed::QueryNewLink(startId, endId);
    }

    ZGUI_API bool node_editor_AcceptNewItem(const float color[4], float thickness)
    {
        return ed::AcceptNewItem(ImVec4(color[0], color[1], color[2], color[3]), thickness);
    }

    ZGUI_API void node_editor_RejectNewItem(const float color[4], float thickness)
    {
        return ed::RejectNewItem(ImVec4(color[0], color[1], color[2], color[3]), thickness);
    }

    // Delete
    ZGUI_API bool node_editor_BeginDelete()
    {
        return ed::BeginDelete();
    }

    ZGUI_API void node_editor_EndDelete()
    {
        ed::EndDelete();
    }

    ZGUI_API bool node_editor_QueryDeletedLink(ed::LinkId *linkId, ed::PinId *startId, ed::PinId *endId)
    {
        return ed::QueryDeletedLink(linkId, startId, endId);
    }

    ZGUI_API bool node_editor_QueryDeletedNode(ed::NodeId *nodeId)
    {
        return ed::QueryDeletedNode(nodeId);
    }

    ZGUI_API bool node_editor_AcceptDeletedItem(bool deleteDependencies)
    {
        return ed::AcceptDeletedItem(deleteDependencies);
    }

    ZGUI_API void node_editor_RejectDeletedItem()
    {
        ed::RejectDeletedItem();
    }

    // Style
    ZGUI_API ax::NodeEditor::Style node_editor_GetStyle()
    {
        return ed::GetStyle();
    }

    ZGUI_API const char *node_editor_GetStyleColorName(ed::StyleColor colorIndex)
    {
        return ed::GetStyleColorName(colorIndex);
    }

    ZGUI_API void node_editor_PushStyleColor(ed::StyleColor colorIndex, const ImVec4 *color)
    {
        ed::PushStyleColor(colorIndex, *color);
    }

    ZGUI_API void node_editor_PopStyleColor(int count)
    {
        ed::PopStyleColor(count);
    }

    ZGUI_API void node_editor_PushStyleVarF(ed::StyleVar varIndex, float value)
    {
        ed::PushStyleVar(varIndex, value);
    }

    ZGUI_API void node_editor_PushStyleVar2f(ed::StyleVar varIndex, const ImVec2 *value)
    {
        ed::PushStyleVar(varIndex, *value);
    }

    ZGUI_API void node_editor_PushStyleVar4f(ed::StyleVar varIndex, const ImVec4 *value)
    {
        ed::PushStyleVar(varIndex, *value);
    }

    ZGUI_API void node_editor_PopStyleVar(int count)
    {
        ed::PopStyleVar(count);
    }

    // Selection

    ZGUI_API bool node_editor_HasSelectionChanged()
    {
        return ed::HasSelectionChanged();
    }

    ZGUI_API int node_editor_GetSelectedObjectCount()
    {
        return ed::GetSelectedObjectCount();
    }

    ZGUI_API void node_editor_ClearSelection()
    {
        return ed::ClearSelection();
    }

    ZGUI_API int node_editor_GetSelectedNodes(ed::NodeId *nodes, int size)
    {
        return ed::GetSelectedNodes(nodes, size);
    }

    ZGUI_API int node_editor_GetSelectedLinks(ed::LinkId *links, int size)
    {
        return ed::GetSelectedLinks(links, size);
    }

    ZGUI_API ImDrawList *node_editor_GetHintForegroundDrawList()
    {
        return ed::GetHintForegroundDrawList();
    }

    ZGUI_API ImDrawList *node_editor_GetHintBackgroundDrawList()
    {
        return ed::GetHintBackgroundDrawList();
    }

    ZGUI_API ImDrawList *node_editor_GetNodeBackgroundDrawList(ed::NodeId nodeId)
    {
        return ed::GetNodeBackgroundDrawList(nodeId);
    }
} /* extern "C" */