//
// Created by toor on 11/8/23.
//

#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ATOM/Application.h"


namespace Atom {
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer") {
    }

    void ImGuiLayer::OnAttach() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::GetApp();
        GLFWwindow* window = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());


        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void ImGuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void ImGuiLayer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool dockingEnabled = true;
        if (dockingEnabled) {
            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
            if (opt_fullscreen) {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }


            //Style
            {
                ImGuiStyle& style = ImGui::GetStyle();

                style.WindowPadding = ImVec2(15, 15);
                style.WindowRounding = 5.0f;
                style.FramePadding = ImVec2(5, 5);
                style.FrameRounding = 4.0f;
                style.ItemSpacing = ImVec2(12, 8);
                style.ItemInnerSpacing = ImVec2(8, 6);
                style.IndentSpacing = 25.0f;
                style.ScrollbarSize = 15.0f;
                style.ScrollbarRounding = 10.0f;
                style.GrabMinSize = 5.0f;
                style.GrabRounding = 3.0f;

                style.TabRounding = 0.0f;


                style.Colors[ImGuiCol_Tab] = ImVec4(0.17f, 0.24f, 0.25f, 1.00f);
                style.Colors[ImGuiCol_TabHovered] = ImVec4(0.29f, 0.35f, 0.36f, 1.00f);
                style.Colors[ImGuiCol_TabActive] = ImVec4(0.17f, 0.24f, 0.25f, 1.00f);
                style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.46f, 0.56f, 0.55f, 0.70f);
                style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.20f, 0.21f, 1.00f);


                style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.54f);
                style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.43f, 0.48f, 0.54f);
                style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.43f, 0.48f, 0.54f);
                style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
                style.Colors[ImGuiCol_CheckMark] = ImVec4(0.23f, 0.67f, 0.29f, 0.84f);
                style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
                style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.70f, 0.70f, 0.71f, 1.00f);
                style.Colors[ImGuiCol_Button] = ImVec4(0.27f, 0.46f, 0.48f, 0.40f);
                style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.68f, 0.73f, 0.40f);
                style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.51f, 0.58f, 0.40f);
                style.Colors[ImGuiCol_Header] = ImVec4(0.50f, 0.53f, 0.57f, 0.31f);
                style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.72f, 0.72f, 0.31f);
                style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.60f, 0.86f, 0.88f, 0.31f);
                style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.53f, 0.54f, 0.67f);
                style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.32f, 0.64f, 0.65f, 0.67f);
                style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.19f, 0.19f, 0.20f);
                style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.27f, 0.27f, 0.28f, 0.67f);
                style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.45f, 0.45f, 0.95f);
                style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.31f, 0.44f, 0.46f, 1.00f);
                style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.44f, 0.55f, 0.69f, 1.00f);
            }
        }
    }

    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::GetApp();
        io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float) app.GetWindow().GetHeight());
        ImGui::End();
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
