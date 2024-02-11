//
// Created by toor on 11/8/23.
//

#include "EditorLayer.h"


namespace Atom
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::DrawMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Fille"))
                {
                    ATLOG_INFO("Open Fille");
                    char path[1024];
                    FILE *f = popen("zenity --file-selection --width=720 --height=480 --modal --center", "r");
                    fgets(path, 1024, f);
                    ATLOG_INFO("path: {0}", path);

                };

                if(ImGui::MenuItem("Save File"))
                {
                    ATLOG_INFO("Save File");
                    char path[1024];
                    FILE *f = popen("zenity --file-selection --save", "r");
                    fgets(path, 1024, f);
                    // print filename and path
                    ATLOG_INFO("path: {0}", path);
                }


                if (ImGui::MenuItem("Exit"))
                    ATLOG_INFO("Close");
                ImGui::EndMenu();
            }


            if (ImGui::BeginMenu("Settings"))
            {
                if (ImGui::MenuItem("Open Settings"))
                {
                    ATLOG_INFO("Settings Open")
                };
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Script"))
            {
                if (ImGui::MenuItem("New Script", "Ctrl+Shift+n"))
                {
                    ATLOG_INFO("New Script");
                };

                if (ImGui::MenuItem("Save Script", "Ctrl+Shift+n"))
                {
                    ATLOG_INFO("Save Script");
                };


                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }
    }


    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate()
    {
    }

    void EditorLayer::OnFixedUpdate() {
        Layer::OnFixedUpdate();
    }

    void EditorLayer::OnImGuiRender()
    {

        DrawMenu();

        for (auto& func : m_FunctionsVenctor)
        {
            func();
        }


        // ImGui::Begin("Viewport");
        // ImVec2 windowSize = ImGui::GetContentRegionAvail();
        // if (windowSize.x != previousWindowSize.x || windowSize.y != previousWindowSize.y) {
        //     previousWindowSize = windowSize;
        //     if (m_ViewportCallbackFunction) {
        //         m_ViewportCallbackFunction(windowSize.x, windowSize.y);
        //         // m_FrameBuffer->SetFramebufferTextureSizeCallback(windowSize.x, windowSize.y);
        //         // glViewport(0, 0, windowSize.x, windowSize.y);
        //     }
        // }
        //
        // ImGui::Text("Renderer2D Stats:");
        // ImGui::End();
    }
}
