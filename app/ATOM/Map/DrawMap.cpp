//
// Created by toor on 1/19/24.
//

#include "DrawMap.h"

#include "ATOM/Application.h"


namespace Atom {
    void MatToTexture(const cv::Mat &mat, GLuint &texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    DrawMap::DrawMap()
        : Layer("DrawMap") {
        m_Window = (GLFWwindow *) Application::GetApp().GetWindow().GetNativeWindow();

        //open image
        m_ImgBackground = cv::imread("ASSETS/track.png");
        if (m_ImgBackground.empty()) {
            ATLOG_CRITICAL("Error to open image")
        }
        m_ImgColorBackground = cv::imread("ASSETS/colortrack.png");
        if (m_ImgColorBackground.empty()) {
            ATLOG_CRITICAL("Error to open image")
        }


        //flip both images horizontaly
        cv::flip(m_ImgBackground, m_ImgBackground, 0);
        cv::flip(m_ImgColorBackground, m_ImgColorBackground, 0);

        cv::resize(m_ImgBackground, m_ImgBackground, cv::Size(640, 480));
        cv::resize(m_ImgColorBackground, m_ImgColorBackground, cv::Size(640, 480));

        m_Width = m_ImgBackground.cols;
        m_Height = m_ImgBackground.rows;
        m_AspectRatio = (float) m_Width / (float) m_Height;

        m_DeltaDashLineX = (int) std::round(m_Width / m_LinesX);
        m_DeltaDashLineY = (int) std::round(m_Height / m_LinesY);

        m_Matrix = new MatrixElement *[m_LinesX];
        for (int i = 0; i < m_LinesX; ++i) {
            m_Matrix[i] = new MatrixElement[m_LinesY];
        }


        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    DrawMap::~DrawMap() {
    }

    void DrawMap::OnAttach() {
//        MatToTexture(m_ImgBackground, textureID);
    }

    void DrawMap::OnDetach() {
    }

    void DrawMap::OnUpdate() {
        if(!m_ImgColorBackground.empty()){
            if (!m_UpdateTexture) {
                MatToTexture(m_ImgBackground, textureID);
                m_UpdateTexture = true;
            }
        }
    }

    void DrawMap::OnImGuiRender() {
        ImGui::Begin("DrawMap");

        isWindowFocused = ImGui::IsWindowFocused();

        //get mause position inside current imgui window
        ImVec2 mousePos = ImGui::GetMousePos();
        mousePos.x -= ImGui::GetCursorScreenPos().x;
        mousePos.y -= ImGui::GetCursorScreenPos().y;
        m_MousePos = glm::vec2(mousePos.x, mousePos.y);
        ImVec2 available = ImGui::GetContentRegionAvail();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (available.y / 2 - available.x / m_AspectRatio / 2));


        static ImVector<ImVec2> points;
        static ImVec2 scrolling(0.0f, 0.0f);
        static bool opt_enable_grid = true;
        static bool opt_enable_context_menu = true;
        static bool adding_line = false;

        ImGui::Checkbox("Enable grid", &opt_enable_grid);
        ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
        ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

        // Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
        // Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
        // To use a child window instead we could use, e.g:
        //      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
        //      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
        //      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border, ImGuiWindowFlags_NoMove);
        //      ImGui::PopStyleColor();
        //      ImGui::PopStyleVar();
        //      [...]
        //      ImGui::EndChild();

        // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to what's available
        if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

        // Draw border and background color
        ImGuiIO &io = ImGui::GetIO();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
        draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));


        //draw texture to background
        draw_list->AddImage((void *) textureID, canvas_p0, canvas_p1, ImVec2(0, 1), ImVec2(1, 0));

        // This will catch our interactions
        ImGui::InvisibleButton("canvas", canvas_sz,
                               ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool is_hovered = ImGui::IsItemHovered(); // Hovered
        const bool is_active = ImGui::IsItemActive(); // Held
        const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

        // Add first and second point
        if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            points.push_back(mouse_pos_in_canvas);
            points.push_back(mouse_pos_in_canvas);
            adding_line = true;
        }
        if (adding_line) {
            points.back() = mouse_pos_in_canvas;
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                adding_line = false;
        }

        // Pan (we use a zero mouse threshold when there's no context menu)
        // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
        const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
        if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan)) {
            scrolling.x += io.MouseDelta.x;
            scrolling.y += io.MouseDelta.y;
        }

        // Context menu (under default mouse threshold)
        ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
            ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        if (ImGui::BeginPopup("context")) {
            if (adding_line)
                points.resize(points.size() - 2);
            adding_line = false;
            if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
            if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
            ImGui::EndPopup();
        }

        // Draw grid + all lines in the canvas
        draw_list->PushClipRect(canvas_p0, canvas_p1, true);
        if (opt_enable_grid) {
            const float GRID_STEP = 64.0f;
            for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y),
                                   IM_COL32(200, 200, 200, 40));
            for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y),
                                   IM_COL32(200, 200, 200, 40));
        }
        for (int n = 0; n < points.Size; n += 2)
            draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y),
                               ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y),
                               IM_COL32(255, 255, 0, 255), 2.0f);
        draw_list->PopClipRect();


        ImGui::End();
    }

    void DrawMap::DrawDashLine(int x1, int y1, int x2, int y2, int dash, int gap, glm::vec4 color) {
    }

    void DrawMap::DrawFilledCircle(int centerX, int centerY, int radius, glm::vec4 color) {
    }
}
