#include "entity_gui.h"

#include <glm/gtc/quaternion.hpp>
#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "components.h"

void ShowEntityEditor(bool* p_open, Camera* camera, entityx::EntityManager* entity_manager)
{
    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Entity editor", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    ImGui::Columns(2);
    ImGui::Separator();

    struct funcs
    {
        static void ShowCamera(Camera* camera)
        {
            ImGui::PushID(camera);
            ImGui::AlignFirstTextHeightToWidgets();
            bool node_open = ImGui::TreeNode("Camera", "Camera");
            ImGui::NextColumn();
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::NextColumn();
            if (node_open)
            {
                int i = 0;
                ImGui::PushID(i++);
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Bullet();
                ImGui::Selectable("Position");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat3("##value", &camera->position[0], 0.01f);
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                ImGui::PopID();

                ImGui::PushID(i++);
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Bullet();
                ImGui::Selectable("Rotation");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                ImGui::DragQuat("##value", &camera->orientation);
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                ImGui::PopID();

                ImGui::PushID(i++);
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Bullet();
                ImGui::Selectable("Rotation Quat");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat4("##value", &camera->orientation[0], 0.01f);
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                ImGui::PopID();

                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        static void ShowTranform(Transform* transform, int& imgui_id)
        {
            ImGui::PushID(imgui_id++);
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::Bullet();
            ImGui::Selectable("Position");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat3("##value", &transform->pos[0], 0.01f);
            ImGui::PopItemWidth();
            ImGui::NextColumn();
            ImGui::PopID();

            ImGui::PushID(imgui_id++);
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::Bullet();
            ImGui::Selectable("Rotation");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::DragQuat("##value", &transform->orientation);
            ImGui::PopItemWidth();
            ImGui::NextColumn();
            ImGui::PopID();

            ImGui::PushID(imgui_id++);
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::Bullet();
            ImGui::Selectable("Rotation Quat");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat4("##value", &transform->orientation[0], 0.01f);
            ImGui::PopItemWidth();
            ImGui::NextColumn();
            ImGui::PopID();

            if (transform->parent) {
                ImGui::PushID(imgui_id++);
                ImGui::AlignFirstTextHeightToWidgets();
                bool node_open = ImGui::TreeNode("Parent", "Parent");
                ImGui::NextColumn();
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::NextColumn();

                if (node_open) {
                    ShowTranform(transform->parent, imgui_id);
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
        }

        static void ShowEntity(entityx::Entity& ent)
        {
            ImGui::PushID(ent.id().id());                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
            ImGui::AlignFirstTextHeightToWidgets();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
            bool node_open = ImGui::TreeNode("Entity", "Entity %u", ent.id().id());
            ImGui::NextColumn();
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::NextColumn();
            if (node_open)
            {
                int i = 0;
                auto transform_handle = ent.component<Transform>();
                if (transform_handle) {
                    ShowTranform(transform_handle.get(), i);
                }

                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    };

    funcs::ShowCamera(camera);
    for (entityx::Entity entity : entity_manager->entities_for_debugging()) {
        funcs::ShowEntity(entity);
        
    }

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}

namespace ImGui
{
    bool ImGui::DragQuat(const char* label, glm::quat* quat)
    {
        glm::vec3 euler_rot = glm::degrees(glm::eulerAngles(*quat));
        glm::mod(euler_rot, glm::vec3(360.f));
        if (euler_rot.x < 0) euler_rot.x += 360;
        if (euler_rot.y < 0) euler_rot.y += 360;
        if (euler_rot.z < 0) euler_rot.z += 360;
        
        if (DragFloatN(label, &euler_rot[0], 3, 1.0f, 0.0f, 360.0f, "%.3f", 1.0f)) {
            *quat = glm::quat(glm::radians(euler_rot));
            return true;
        }
        return false;
    }
}
